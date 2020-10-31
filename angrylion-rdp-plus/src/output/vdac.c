#include "vdac.h"
#include "screen.h"
#include "core/msg.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef GLES
#include <GLES3/gl3.h>
#define SHADER_HEADER "#version 300 es\nprecision lowp float;\n"
#else
#include "gl_core_3_3.h"
#define SHADER_HEADER "#version 330 core\n"
#endif

#define TEX_FORMAT GL_RGBA
#define TEX_TYPE GL_UNSIGNED_BYTE

static GLuint m_program;
static GLuint m_vao;
static GLuint m_texture;

static uint32_t m_tex_width;
static uint32_t m_tex_height;

static uint32_t m_tex_height_out;

#ifdef _DEBUG
static void gl_check_errors(void)
{
    GLenum err;
    static int32_t invalid_op_count = 0;
    while ((err = glGetError()) != GL_NO_ERROR) {
        // if gl_check_errors is called from a thread with no valid
        // GL context, it would be stuck in an infinite loop here, since
        // glGetError itself causes GL_INVALID_OPERATION, so check for a few
        // cycles and abort if there are too many errors of that kind
        if (err == GL_INVALID_OPERATION) {
            if (++invalid_op_count >= 100) {
                msg_error("gl_check_errors: invalid OpenGL context!");
            }
        } else {
            invalid_op_count = 0;
        }

        char* err_str;
        switch (err) {
            case GL_INVALID_OPERATION:
                err_str = "INVALID_OPERATION";
                break;
            case GL_INVALID_ENUM:
                err_str = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                err_str = "INVALID_VALUE";
                break;
            case GL_OUT_OF_MEMORY:
                err_str = "OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                err_str = "INVALID_FRAMEBUFFER_OPERATION";
                break;
            default:
                err_str = "unknown";
        }
        msg_debug("gl_check_errors: %d (%s)", err, err_str);
    }
}
#else
#define gl_check_errors(...)
#endif

static bool gl_shader_load_file(GLuint shader, const char* path)
{
    bool success = false;
    GLchar* source = NULL;
    FILE* fp = fopen(path, "rb");
    if (!fp) {
        // fail quietly
        goto end;
    }

    // get file size
    fseek(fp, 0, SEEK_END);
    uint32_t source_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // allocate buffer for shader code
    source = malloc(source_size + 1);
    if (source == NULL) {
        msg_error("Can't allocate memory for shader file %s", path);
        goto end;
    }

    // read shader code
    if (fread(source, source_size, 1, fp) != 1) {
        msg_warning("Can't read shader file %s", path);
        goto end;
    }

    // terminate shader code string
    source[source_size] = 0;

    // send string to OpenGL
    const GLchar* source_ptr = source;
    glShaderSource(shader, 1, &source_ptr, NULL);

    success = true;

end:
    if (fp) {
        fclose(fp);
    }
    if (source) {
        free(source);
    }

    return success;
}

static GLuint gl_shader_compile(GLenum type, const GLchar* source, const char* path)
{
    GLuint shader = glCreateShader(type);

    // try to load external shader file first, otherwise use embedded fallback shader
    if (!gl_shader_load_file(shader, path)) {
        glShaderSource(shader, 1, &source, NULL);
    }

    glCompileShader(shader);

    GLint param;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &param);

    if (!param) {
        GLchar log[4096];
        glGetShaderInfoLog(shader, sizeof(log), NULL, log);
        msg_error("%s shader error: %s\n", type == GL_FRAGMENT_SHADER ? "Frag" : "Vert", log);
    }

    return shader;
}

static GLuint gl_shader_link(GLuint vert, GLuint frag)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    GLint param;
    glGetProgramiv(program, GL_LINK_STATUS, &param);

    if (!param) {
        GLchar log[4096];
        glGetProgramInfoLog(program, sizeof(log), NULL, log);
        msg_error("Shader link error: %s\n", log);
    }

    glDeleteShader(frag);
    glDeleteShader(vert);

    return program;
}

void vdac_init(struct n64video_config* config)
{
    screen_init(config);

#ifndef GLES
    // load OpenGL function pointers
    ogl_LoadFunctions();
#endif

    msg_debug("%s: GL_VERSION='%s'", __FUNCTION__, glGetString(GL_VERSION));
    msg_debug("%s: GL_VENDOR='%s'", __FUNCTION__, glGetString(GL_VENDOR));
    msg_debug("%s: GL_RENDERER='%s'", __FUNCTION__, glGetString(GL_RENDERER));
    msg_debug("%s: GL_SHADING_LANGUAGE_VERSION='%s'", __FUNCTION__, glGetString(GL_SHADING_LANGUAGE_VERSION));

    // shader sources for drawing a clipped full-screen triangle. the geometry
    // is defined by the vertex ID, so a VBO is not required.
    const GLchar* vert_shader =
        SHADER_HEADER
        "out vec2 uv;\n"
        "void main(void) {\n"
        "    uv = vec2((gl_VertexID << 1) & 2, gl_VertexID & 2);\n"
        "    gl_Position = vec4(uv * vec2(2.0, -2.0) + vec2(-1.0, 1.0), 0.0, 1.0);\n"
        "}\n";

    const GLchar* frag_shader =
        SHADER_HEADER
        "in vec2 uv;\n"
        "layout(location = 0) out vec4 color;\n"
        "uniform sampler2D tex0;\n"
        "void main(void) {\n"
        "    color = texture(tex0, uv);\n"
        "}\n";

    // compile and link OpenGL program
    GLuint vert = gl_shader_compile(GL_VERTEX_SHADER, vert_shader, "alp_screen.vert");
    GLuint frag = gl_shader_compile(GL_FRAGMENT_SHADER, frag_shader, "alp_screen.frag");
    m_program = gl_shader_link(vert, frag);
    glUseProgram(m_program);

    // prepare dummy VAO
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // prepare texture
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    // select interpolation method
    GLint filter;
    switch (config->vi.interp) {
        case VI_INTERP_LINEAR:
            filter = GL_LINEAR;
            break;
        case VI_INTERP_NEAREST:
        default:
            filter = GL_NEAREST;
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

    // check if there was an error when using any of the commands above
    gl_check_errors();
}

void vdac_read(struct n64video_frame_buffer* fb, bool alpha)
{
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);

    fb->width = vp[2];
    fb->height = vp[3];
    fb->pitch = fb->width;

    if (fb->pixels) {
        glReadPixels(vp[0], vp[1], vp[2], vp[3], alpha ? GL_RGBA : GL_RGB, TEX_TYPE, fb->pixels);
    }
}

void vdac_write(struct n64video_frame_buffer* fb)
{
    bool buffer_size_changed = m_tex_width != fb->width || m_tex_height != fb->height;

    // check if the framebuffer size has changed
    if (buffer_size_changed) {
        m_tex_width = fb->width;
        m_tex_height = fb->height;

        // set pitch for all unpacking operations
        glPixelStorei(GL_UNPACK_ROW_LENGTH, fb->pitch);

        // reallocate texture buffer on GPU
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_tex_width,
            m_tex_height, 0, TEX_FORMAT, TEX_TYPE, fb->pixels);

        msg_debug("%s: resized framebuffer texture: %dx%d", __FUNCTION__, m_tex_width, m_tex_height);
    } else {
        // copy local buffer to GPU texture buffer
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_tex_width, m_tex_height,
            TEX_FORMAT, TEX_TYPE, fb->pixels);
    }

    // update output size
    m_tex_height_out = fb->height_out;
}

void vdac_sync(bool valid)
{
    // clear old buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // get current window size and position
    int32_t win_width;
    int32_t win_height;
    int32_t win_x;
    int32_t win_y;

    screen_adjust(m_tex_width, m_tex_height_out, &win_width, &win_height, &win_x, &win_y);

    // if the screen is invalid or hidden, do nothing
    if (win_width <= 0 || win_height <= 0) {
        return;
    }

    // skip rendering and leave buffer blank if there's no valid input
    if (!valid) {
        screen_update();
        return;
    }

    int32_t hw = m_tex_height_out * win_width;
    int32_t wh = m_tex_width * win_height;

    // add letterboxes or pillarboxes if the window has a different aspect ratio
    // than the current display mode
    if (hw > wh) {
        int32_t w_max = wh / m_tex_height_out;
        win_x += (win_width - w_max) / 2;
        win_width = w_max;
    } else if (hw < wh) {
        int32_t h_max = hw / m_tex_width;
        win_y += (win_height - h_max) / 2;
        win_height = h_max;
    }

    // configure viewport
    glViewport(win_x, win_y, win_width, win_height);

    // draw fullscreen triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // check if there was an error when using any of the commands above
    gl_check_errors();

    // refresh screen with new frame
    screen_update();
}

void vdac_close(void)
{
    m_tex_width = 0;
    m_tex_height = 0;

    m_tex_height_out = 0;

    glDeleteTextures(1, &m_texture);
    glDeleteVertexArrays(1, &m_vao);
    glDeleteProgram(m_program);

    screen_close();
}
