#include "gfx_1.3.h"
#include "config.h"
#include "resource.h"

#include "core/common.h"
#include "core/n64video.h"
#include "core/version.h"
#include "core/msg.h"

#include "output/screen.h"
#include "output/vdac.h"

#include <stdio.h>
#include <ctype.h>

GFX_INFO gfx;
static bool m_warn_hle;
static char m_screenshot_path[MAX_PATH];

static bool is_valid_ptr(void *ptr, uint32_t bytes)
{
    SIZE_T dwSize;
    MEMORY_BASIC_INFORMATION meminfo;
    if (!ptr) {
        return false;
    }
    memset(&meminfo, 0x00, sizeof(meminfo));
    dwSize = VirtualQuery(ptr, &meminfo, sizeof(meminfo));
    if (!dwSize) {
        return false;
    }
    if (MEM_COMMIT != meminfo.State) {
        return false;
    }
    if (!(meminfo.Protect & (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY))) {
        return false;
    }
    if (bytes > meminfo.RegionSize) {
        return false;
    }
    if ((uint64_t)((char*)ptr - (char*)meminfo.BaseAddress) > (uint64_t)(meminfo.RegionSize - bytes)) {
        return false;
    }
    return true;
}

static char filter_char(char c)
{
    // only allow valid ASCII chars
    if (c & 0x80) {
        return ' ';
    }

    // only allow certain ASCII chars
    if (!isalnum(c) && c != '_' && c != '-') {
        return ' ';
    }

    return c;
}

static char* get_rom_name(void)
{
    static char rom_name[128];

    // copy game name from ROM header, which is encoded in Shift_JIS.
    // most games just use the ASCII subset, so filter out the rest.
    // TODO: convert Shift_JIS string to UTF-16 for Win32 API?
    int i = 0;
    for (; i < 20; i++) {
        rom_name[i] = filter_char(gfx.HEADER[(32 + i) ^ BYTE_ADDR_XOR]);
    }

    // make sure there's at least one whitespace that will terminate the string
    // below
    rom_name[i] = ' ';

    // trim trailing whitespaces
    for (; i > 0; i--) {
        if (rom_name[i] != ' ') {
            i++;
            break;
        }
    }

    if (i == 0) {
        // game title is empty or invalid, use safe fallback using the
        // four-character game ID
        for (; i < 4; i++) {
            rom_name[i] = filter_char(gfx.HEADER[(59 + i) ^ BYTE_ADDR_XOR]);
        }
    }

    // terminate string
    rom_name[i] = '\0';

    return rom_name;
}

static void mi_intr(void)
{
    gfx.CheckInterrupts();
    config_update();
}

static void write_screenshot(char* path)
{
    struct n64video_frame_buffer fb = { 0 };
    vdac_read(&fb, true);

    // prepare bitmap headers
    BITMAPINFOHEADER ihdr = {0};
    ihdr.biSize = sizeof(ihdr);
    ihdr.biWidth = fb.width;
    ihdr.biHeight = fb.height;
    ihdr.biPlanes = 1;
    ihdr.biBitCount = 32;
    ihdr.biSizeImage = fb.width * fb.height * sizeof(int32_t);

    BITMAPFILEHEADER fhdr = {0};
    fhdr.bfType = 'B' | ('M' << 8);
    fhdr.bfOffBits = sizeof(fhdr) + sizeof(ihdr) + 10;
    fhdr.bfSize = ihdr.biSizeImage + fhdr.bfOffBits;

    FILE* fp = fopen(path, "wb");

    if (!fp) {
        msg_warning("Can't open screenshot file %s!", path);
        return;
    }

    // write bitmap headers
    fwrite(&fhdr, sizeof(fhdr), 1, fp);
    fwrite(&ihdr, sizeof(ihdr), 1, fp);

    // write bitmap contents
    fseek(fp, fhdr.bfOffBits, SEEK_SET);

    fb.pixels = malloc(ihdr.biSizeImage);
    vdac_read(&fb, true);

    // convert RGBA to BGRA
    for (uint32_t i = 0; i < fb.width * fb.height; i++) {
        struct n64video_pixel* pixel = &fb.pixels[i];
        uint8_t tmp = pixel->r;
        pixel->r = pixel->b;
        pixel->b = tmp;
    }

    fwrite(fb.pixels, ihdr.biSizeImage, 1, fp);
    free(fb.pixels);

    fclose(fp);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    UNUSED(lpvReserved);
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            config_init(hinstDLL);
            break;
    }
    return TRUE;
}

EXPORT void CALL CaptureScreen(char* directory)
{
    char* rom_name = get_rom_name();

    for (int32_t i = 0; i < 10000; i++) {
        sprintf(m_screenshot_path, "%s\\%s_%04d.bmp", directory, rom_name, i);
        DWORD dwAttrib = GetFileAttributes(m_screenshot_path);
        if (dwAttrib == INVALID_FILE_ATTRIBUTES || (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)) {
            break;
        }
    }
}

EXPORT void CALL ChangeWindow(void)
{
    screen_toggle_fullscreen();
}

EXPORT void CALL CloseDLL(void)
{
}

EXPORT void CALL DllAbout(HWND hParent)
{
    UNUSED(hParent);

    msg_warning(
        CORE_NAME "\n\n"
        "Branch: " GIT_BRANCH "\n"
        "Commit hash: " GIT_COMMIT_HASH "\n"
        "Commit date: " GIT_COMMIT_DATE "\n"
        "Build date: " __DATE__ " " __TIME__ "\n\n"
        "https://github.com/ata4/angrylion-rdp-plus"
    );
}

EXPORT void CALL DllConfig(HWND hParent)
{
    config_dialog(hParent);
}

EXPORT void CALL ReadScreen(void **dest, long *width, long *height)
{
    UNUSED(dest);
    UNUSED(width);
    UNUSED(height);
}

EXPORT void CALL DrawScreen(void)
{
}

EXPORT void CALL GetDllInfo(PLUGIN_INFO* PluginInfo)
{
    PluginInfo->Version = 0x0103;
    PluginInfo->Type  = PLUGIN_TYPE_GFX;
    sprintf(PluginInfo->Name, CORE_NAME);

    PluginInfo->NormalMemory = TRUE;
    PluginInfo->MemoryBswaped = TRUE;
}

EXPORT BOOL CALL InitiateGFX(GFX_INFO Gfx_Info)
{
    gfx = Gfx_Info;

    return TRUE;
}

EXPORT void CALL MoveScreen(int xpos, int ypos)
{
    UNUSED(xpos);
    UNUSED(ypos);
}

EXPORT void CALL ProcessDList(void)
{
    if (!m_warn_hle) {
        msg_warning("Please disable 'Graphic HLE' in the plugin settings.");
        m_warn_hle = true;
    }
}

EXPORT void CALL ProcessRDPList(void)
{
    n64video_process_list();
}

EXPORT void CALL RomClosed(void)
{
    vdac_close();
    n64video_close();
}

EXPORT void CALL RomOpen(void)
{
    config_load();
    struct n64video_config* config = config_get();

    config->gfx.rdram = gfx.RDRAM;
    config->gfx.rdram_size = RDRAM_MAX_SIZE;

    // Zilmar's API doesn't provide a way to check the amount of RDRAM available.
    // It can only be 4 MiB or 8 MiB, so check if the last 16 bytes of the provided
    // buffer in the 8 MiB range are valid. If not, it must be 4 MiB.
    if (!is_valid_ptr(&gfx.RDRAM[0x7f0000], 16)) {
        config->gfx.rdram_size /= 2;
    }

    config->gfx.dmem = gfx.DMEM;
    config->gfx.mi_intr_reg = (uint32_t*)gfx.MI_INTR_REG;
    config->gfx.mi_intr_cb = mi_intr;

    config->gfx.vi_reg = (uint32_t**)&gfx.VI_STATUS_REG;
    config->gfx.dp_reg = (uint32_t**)&gfx.DPC_START_REG;

    n64video_init(config);
    vdac_init(config);
}

EXPORT void CALL ShowCFB(void)
{
}

EXPORT void CALL UpdateScreen(void)
{
    struct n64video_frame_buffer fb;
    n64video_update_screen(&fb);

    if (fb.valid) {
        vdac_write(&fb);
    }

    vdac_sync(fb.valid);

    // write screenshot file if requested
    if (m_screenshot_path[0]) {
        write_screenshot(m_screenshot_path);
        m_screenshot_path[0] = 0;
    }
}

EXPORT void CALL ViStatusChanged(void)
{
}

EXPORT void CALL ViWidthChanged(void)
{
}

EXPORT void CALL FBWrite(DWORD addr, DWORD val)
{
    UNUSED(addr);
    UNUSED(val);
}

EXPORT void CALL FBWList(FrameBufferModifyEntry *plist, DWORD size)
{
    UNUSED(plist);
    UNUSED(size);
}

EXPORT void CALL FBRead(DWORD addr)
{
    UNUSED(addr);
}

EXPORT void CALL FBGetFrameBufferInfo(void *pinfo)
{
    UNUSED(pinfo);
}
