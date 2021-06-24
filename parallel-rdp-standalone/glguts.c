#include "glguts.h"
#include "gfx_m64p.h"
#include "parallel_imp.h"

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <math.h>

/* definitions of pointers to Core video extension functions */
static ptr_VidExt_Init CoreVideo_Init = NULL;
static ptr_VidExt_Quit CoreVideo_Quit = NULL;
static ptr_VidExt_ListFullscreenModes CoreVideo_ListFullscreenModes = NULL;
static ptr_VidExt_SetVideoMode CoreVideo_SetVideoMode = NULL;
static ptr_VidExt_SetCaption CoreVideo_SetCaption = NULL;
static ptr_VidExt_ToggleFullScreen CoreVideo_ToggleFullScreen = NULL;
static ptr_VidExt_ResizeWindow CoreVideo_ResizeWindow = NULL;
static ptr_VidExt_GL_GetProcAddress CoreVideo_GL_GetProcAddress = NULL;
static ptr_VidExt_GL_SetAttribute CoreVideo_GL_SetAttribute = NULL;
static ptr_VidExt_GL_GetAttribute CoreVideo_GL_GetAttribute = NULL;
static ptr_VidExt_GL_SwapBuffers CoreVideo_GL_SwapBuffers = NULL;

static PFNGLCREATESHADERPROC glCreateShader;
static PFNGLSHADERSOURCEPROC glShaderSource;
static PFNGLCOMPILESHADERPROC glCompileShader;
static PFNGLGETSHADERIVPROC glGetShaderiv;
static PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
static PFNGLCREATEPROGRAMPROC glCreateProgram;
static PFNGLATTACHSHADERPROC glAttachShader;
static PFNGLLINKPROGRAMPROC glLinkProgram;
static PFNGLGETPROGRAMIVPROC glGetProgramiv;
static PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
static PFNGLDELETESHADERPROC glDeleteShader;
static PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
static PFNGLDELETEPROGRAMPROC glDeleteProgram;
static PFNGLUSEPROGRAMPROC glUseProgram;
static PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
static PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
static PFNGLBINDBUFFERPROC glBindBuffer;
static PFNGLGENBUFFERSPROC glGenBuffers;
static PFNGLDELETEBUFFERSPROC glDeleteBuffers;
static PFNGLBUFFERSTORAGEPROC glBufferStorage;
static PFNGLMAPBUFFERRANGEPROC glMapBufferRange;
static PFNGLUNMAPBUFFERPROC glUnmapBuffer;
static PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
static PFNGLUNIFORM1IPROC glUniform1i;

static bool toggle_fs;
static int rotate_buffer;

// framebuffer texture states
int32_t window_width;
int32_t window_height;
int32_t window_fullscreen;
int32_t window_widescreen;

#define SHADER_HEADER "#version 330 core\n"
#define TEX_FORMAT GL_RGBA
#define TEX_TYPE GL_UNSIGNED_BYTE
#define TEX_NUM 3

static GLuint program;
static GLuint vao;
static GLuint buffer;
static GLuint texture[TEX_NUM];
static uint8_t *buffer_data;
static uint32_t buffer_size = (640*8) * (480*8) * sizeof(uint32_t);

int32_t tex_width[TEX_NUM];
int32_t tex_height[TEX_NUM];
int display_width;
int display_height;

#ifdef GL_DEBUG
static void gl_check_errors(void)
{
    GLenum err;
    static int32_t invalid_op_count = 0;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        // if gl_check_errors is called from a thread with no valid
        // GL context, it would be stuck in an infinite loop here, since
        // glGetError itself causes GL_INVALID_OPERATION, so check for a few
        // cycles and abort if there are too many errors of that kind
        if (err == GL_INVALID_OPERATION)
        {
            if (++invalid_op_count >= 100)
            {
                printf("gl_check_errors: invalid OpenGL context!");
            }
        }
        else
        {
            invalid_op_count = 0;
        }

        char *err_str;
        switch (err)
        {
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
        printf("gl_check_errors: %d (%s)", err, err_str);
    }
}
#else
#define gl_check_errors(...)
#endif

static GLuint gl_shader_compile(GLenum type, const GLchar *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint param;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &param);

    if (!param)
    {
        GLchar log[4096];
        glGetShaderInfoLog(shader, sizeof(log), NULL, log);
        printf("%s shader error: %s\n", type == GL_FRAGMENT_SHADER ? "Frag" : "Vert", log);
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

    if (!param)
    {
        GLchar log[4096];
        glGetProgramInfoLog(program, sizeof(log), NULL, log);
        printf("Shader link error: %s\n", log);
    }

    glDeleteShader(frag);
    glDeleteShader(vert);

    return program;
}

void screen_write(struct frame_buffer *fb)
{
    bool buffer_size_changed = tex_width[rotate_buffer] != fb->width || tex_height[rotate_buffer] != fb->height;
    char* offset = NULL;
    offset += rotate_buffer * buffer_size;

    glBindTexture(GL_TEXTURE_2D, texture[rotate_buffer]);
    // check if the framebuffer size has changed
    if (buffer_size_changed)
    {
        tex_width[rotate_buffer] = fb->width;
        tex_height[rotate_buffer] = fb->height;
        // set pitch for all unpacking operations
        glPixelStorei(GL_UNPACK_ROW_LENGTH, fb->pitch);
        // reallocate texture buffer on GPU
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, tex_width[rotate_buffer],
                     tex_height[rotate_buffer], 0, TEX_FORMAT, TEX_TYPE, offset);
    }
    else
    {
        // copy local buffer to GPU texture buffer
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, tex_width[rotate_buffer], tex_height[rotate_buffer],
                        TEX_FORMAT, TEX_TYPE, offset);
    }

    rotate_buffer = (rotate_buffer + 1) % 3;
}

void screen_read(struct frame_buffer *fb, bool alpha)
{
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);

    fb->width = vp[2];
    fb->height = vp[3];
    fb->pitch = fb->width;

    if (fb->pixels)
    {
        glReadPixels(vp[0], vp[1], vp[2], vp[3], alpha ? GL_RGBA : GL_RGB, TEX_TYPE, fb->pixels);
    }
}

void gl_screen_render()
{
    display_width = (window_widescreen ? 854 : 640) * vk_rescaling;
    display_height = 480 * vk_rescaling;

    int win_width = window_width;
    int win_height = window_height;
    int win_x = 0;
    int win_y = 0;
    int32_t hw = display_height * win_width;
    int32_t wh = display_width * win_height;

    // add letterboxes or pillarboxes if the window has a different aspect ratio
    // than the current display mode
    if (hw > wh) {
        int32_t w_max = wh / display_height;
        win_x += (win_width - w_max) / 2;
        win_width = w_max;
    } else if (hw < wh) {
        int32_t h_max = hw / display_width;
        win_y += (win_height - h_max) / 2;
        win_height = h_max;
    }
    // configure viewport
    glViewport(win_x, win_y, win_width, win_height);

    // draw fullscreen triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void gl_screen_clear(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void gl_screen_close(void)
{
    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
    glDeleteTextures(TEX_NUM, &texture[0]);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &buffer);
    glDeleteProgram(program);
}

uint8_t* screen_get_texture_data()
{
    return buffer_data + (rotate_buffer * buffer_size);
}

void screen_init()
{
    tex_width[0] = 0;
    tex_height[0] = 0;
    tex_width[1] = 0;
    tex_height[1] = 0;
    toggle_fs = false;
    rotate_buffer = 0;
    /* Get the core Video Extension function pointers from the library handle */
    CoreVideo_Init = (ptr_VidExt_Init)DLSYM(CoreLibHandle, "VidExt_Init");
    CoreVideo_Quit = (ptr_VidExt_Quit)DLSYM(CoreLibHandle, "VidExt_Quit");
    CoreVideo_ListFullscreenModes = (ptr_VidExt_ListFullscreenModes)DLSYM(CoreLibHandle, "VidExt_ListFullscreenModes");
    CoreVideo_SetVideoMode = (ptr_VidExt_SetVideoMode)DLSYM(CoreLibHandle, "VidExt_SetVideoMode");
    CoreVideo_SetCaption = (ptr_VidExt_SetCaption)DLSYM(CoreLibHandle, "VidExt_SetCaption");
    CoreVideo_ToggleFullScreen = (ptr_VidExt_ToggleFullScreen)DLSYM(CoreLibHandle, "VidExt_ToggleFullScreen");
    CoreVideo_ResizeWindow = (ptr_VidExt_ResizeWindow)DLSYM(CoreLibHandle, "VidExt_ResizeWindow");
    CoreVideo_GL_GetProcAddress = (ptr_VidExt_GL_GetProcAddress)DLSYM(CoreLibHandle, "VidExt_GL_GetProcAddress");
    CoreVideo_GL_SetAttribute = (ptr_VidExt_GL_SetAttribute)DLSYM(CoreLibHandle, "VidExt_GL_SetAttribute");
    CoreVideo_GL_GetAttribute = (ptr_VidExt_GL_GetAttribute)DLSYM(CoreLibHandle, "VidExt_GL_GetAttribute");
    CoreVideo_GL_SwapBuffers = (ptr_VidExt_GL_SwapBuffers)DLSYM(CoreLibHandle, "VidExt_GL_SwapBuffers");

    CoreVideo_Init();

    CoreVideo_GL_SetAttribute(M64P_GL_CONTEXT_PROFILE_MASK, M64P_GL_CONTEXT_PROFILE_CORE);
    CoreVideo_GL_SetAttribute(M64P_GL_CONTEXT_MAJOR_VERSION, 3);
    CoreVideo_GL_SetAttribute(M64P_GL_CONTEXT_MINOR_VERSION, 3);

    CoreVideo_SetVideoMode(window_width, window_height, 0, window_fullscreen ? M64VIDEO_FULLSCREEN : M64VIDEO_WINDOWED, M64VIDEOFLAG_SUPPORT_RESIZING);

    CoreVideo_SetCaption("Mupen64Plus-Parallel");

    glCreateShader = (PFNGLCREATESHADERPROC) CoreVideo_GL_GetProcAddress("glCreateShader");
    glShaderSource = (PFNGLSHADERSOURCEPROC) CoreVideo_GL_GetProcAddress("glShaderSource");
    glCompileShader = (PFNGLCOMPILESHADERPROC) CoreVideo_GL_GetProcAddress("glCompileShader");
    glGetShaderiv = (PFNGLGETSHADERIVPROC) CoreVideo_GL_GetProcAddress("glGetShaderiv");
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) CoreVideo_GL_GetProcAddress("glGetShaderInfoLog");
    glCreateProgram = (PFNGLCREATEPROGRAMPROC) CoreVideo_GL_GetProcAddress("glCreateProgram");
    glAttachShader = (PFNGLATTACHSHADERPROC) CoreVideo_GL_GetProcAddress("glAttachShader");
    glLinkProgram = (PFNGLLINKPROGRAMPROC) CoreVideo_GL_GetProcAddress("glLinkProgram");
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC) CoreVideo_GL_GetProcAddress("glGetProgramiv");
    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC) CoreVideo_GL_GetProcAddress("glGetProgramInfoLog");
    glDeleteShader = (PFNGLDELETESHADERPROC) CoreVideo_GL_GetProcAddress("glDeleteShader");
    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC) CoreVideo_GL_GetProcAddress("glDeleteVertexArrays");
    glDeleteProgram = (PFNGLDELETEPROGRAMPROC) CoreVideo_GL_GetProcAddress("glDeleteProgram");
    glUseProgram = (PFNGLUSEPROGRAMPROC) CoreVideo_GL_GetProcAddress("glUseProgram");
    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC) CoreVideo_GL_GetProcAddress("glGenVertexArrays");
    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC) CoreVideo_GL_GetProcAddress("glBindVertexArray");
    glBindBuffer = (PFNGLBINDBUFFERPROC) CoreVideo_GL_GetProcAddress("glBindBuffer");
    glGenBuffers = (PFNGLGENBUFFERSPROC) CoreVideo_GL_GetProcAddress("glGenBuffers");
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC) CoreVideo_GL_GetProcAddress("glDeleteBuffers");
    glBufferStorage = (PFNGLBUFFERSTORAGEPROC) CoreVideo_GL_GetProcAddress("glBufferStorage");
    glMapBufferRange = (PFNGLMAPBUFFERRANGEPROC) CoreVideo_GL_GetProcAddress("glMapBufferRange");
    glUnmapBuffer = (PFNGLUNMAPBUFFERPROC) CoreVideo_GL_GetProcAddress("glUnmapBuffer");
    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) CoreVideo_GL_GetProcAddress("glGetUniformLocation");
    glUniform1i = (PFNGLUNIFORM1IPROC) CoreVideo_GL_GetProcAddress("glUniform1i");

    // shader sources for drawing a clipped full-screen triangle. the geometry
    // is defined by the vertex ID, so a VBO is not required.
    const GLchar *vert_shader =
        SHADER_HEADER
        "out vec2 uv;\n"
        "void main(void) {\n"
        "    uv = vec2((gl_VertexID << 1) & 2, gl_VertexID & 2);\n"
        "    gl_Position = vec4(uv * vec2(2.0, -2.0) + vec2(-1.0, 1.0), 0.0, 1.0);\n"
        "}\n";

    const GLchar *frag_shader =
        SHADER_HEADER
        "in vec2 uv;\n"
        "layout(location = 0) out vec4 color;\n"
        "uniform sampler2D tex0;\n"
        "void main(void) {\n"
        "color = texture(tex0, uv);\n"
        "}\n";

    // compile and link OpenGL program
    GLuint vert = gl_shader_compile(GL_VERTEX_SHADER, vert_shader);
    GLuint frag = gl_shader_compile(GL_FRAGMENT_SHADER, frag_shader);
    program = gl_shader_link(vert, frag);
    glUseProgram(program);

    // prepare dummy VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // prepare texture
    glGenTextures(TEX_NUM, &texture[0]);
    for (int i = 0; i < TEX_NUM; ++i)
    {
        glBindTexture(GL_TEXTURE_2D, texture[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    glGenBuffers(1, &buffer);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, buffer);
    glBufferStorage(GL_PIXEL_UNPACK_BUFFER, buffer_size * TEX_NUM, 0, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    buffer_data = glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, buffer_size * TEX_NUM, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

    // check if there was an error when using any of the commands above
    gl_check_errors();
}

void screen_swap(bool blank)
{
    if (toggle_fs)
    {
        CoreVideo_ToggleFullScreen();
        toggle_fs = false;
    }

    gl_screen_clear();

    if (!blank)
    {
        gl_screen_render();
    }

    (*render_callback)(1);
    CoreVideo_GL_SwapBuffers();
}

void screen_set_fullscreen(bool _fullscreen)
{
    toggle_fs = true;
}

bool screen_get_fullscreen()
{
    return false;
}

void screen_toggle_fullscreen()
{
    screen_set_fullscreen(!screen_get_fullscreen());
}

void screen_close(void)
{
    gl_screen_close();

    CoreVideo_Quit();
}
