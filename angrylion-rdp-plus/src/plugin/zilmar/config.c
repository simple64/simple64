#include "config.h"
#include "resource.h"

#include "core/common.h"
#include "core/version.h"
#include "core/n64video.h"
#include "output/screen.h"
#include "output/vdac.h"

#include <Commctrl.h>
#include <Shlwapi.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SECTION_GENERAL "General"
#define SECTION_VIDEO_INTERFACE "VideoInterface"
#define SECTION_DISPLAY_PROCESSOR "DisplayProcessor"

#define KEY_GEN_PARALLEL "parallel"
#define KEY_GEN_NUM_WORKERS "num_workers"

#define KEY_VI_MODE "mode"
#define KEY_VI_INTERP "interpolation"
#define KEY_VI_WIDESCREEN "widescreen"
#define KEY_VI_HIDE_OVERSCAN "hide_overscan"
#define KEY_VI_EXCLUSIVE "exclusive"
#define KEY_VI_VSYNC "vsync"

#define KEY_DP_COMPAT "compat"

#define CONFIG_FILE_NAME CORE_SIMPLE_NAME "-config.ini"

#define CONFIG_DLG_INIT_CHECKBOX(id, var, config) \
    var = GetDlgItem(hwnd, id); \
    SendMessage(var, BM_SETCHECK, (WPARAM)config, 0);

static HINSTANCE inst;
static struct n64video_config config;
static bool config_stale;
static char config_path[MAX_PATH + 1];

static HWND dlg_combo_vi_mode;
static HWND dlg_combo_vi_interp;
static HWND dlg_check_trace;
static HWND dlg_check_multithread;
static HWND dlg_check_vi_widescreen;
static HWND dlg_check_vi_overscan;
static HWND dlg_check_vi_exclusive;
static HWND dlg_check_vi_vsync;
static HWND dlg_check_vi_integer_scaling;
static HWND dlg_combo_dp_compat;
static HWND dlg_spin_workers;
static HWND dlg_edit_workers;

static void config_dialog_update_multithread(void)
{
    BOOL check = (BOOL)SendMessage(dlg_check_multithread, BM_GETCHECK, 0, 0);
    EnableWindow(dlg_spin_workers, check);
    EnableWindow(dlg_edit_workers, check);
    EnableWindow(dlg_combo_dp_compat, check);
}

static void config_dialog_update_vi_mode(void)
{
    LRESULT sel = SendMessage(dlg_combo_vi_mode, CB_GETCURSEL, 0, 0);
    EnableWindow(dlg_check_vi_overscan, sel == VI_MODE_NORMAL);
}

static void config_dialog_fill_combo(HWND dialog, char** entries, size_t num_entries, uint32_t selected)
{
    SendMessage(dialog, CB_RESETCONTENT, 0, 0);
    for (size_t i = 0; i < num_entries; i++) {
        SendMessage(dialog, CB_ADDSTRING, i, (LPARAM)entries[i]);
    }
    SendMessage(dialog, CB_SETCURSEL, (WPARAM)selected, 0);
}

INT_PTR CALLBACK config_dialog_proc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    UNUSED(lParam);

    switch (iMessage) {
        case WM_INITDIALOG: {
            SetWindowText(hwnd, CORE_BASE_NAME " Config");

            config_load();

            char* vi_mode_strings[] = {
                "Filtered",   // VI_MODE_NORMAL
                "Unfiltered", // VI_MODE_COLOR
                "Depth",      // VI_MODE_DEPTH
                "Coverage"    // VI_MODE_COVERAGE
            };

            dlg_combo_vi_mode = GetDlgItem(hwnd, IDC_COMBO_VI_MODE);
            config_dialog_fill_combo(dlg_combo_vi_mode, vi_mode_strings, VI_MODE_NUM, config.vi.mode);

            char* vi_interp_strings[] = {
                "Blocky (nearest-neightbor)",   // VI_INTERP_NEAREST
                "Blurry (bilinear)",            // VI_INTERP_LINEAR
                "Soft (bilinear + NN)"          // VI_INTERP_HYBRID
            };

            dlg_combo_vi_interp = GetDlgItem(hwnd, IDC_COMBO_VI_INTERP);
            config_dialog_fill_combo(dlg_combo_vi_interp, vi_interp_strings, VI_INTERP_NUM, config.vi.interp);

            char* dp_compat_strings[] = {
                "Fast, most glitches",      // DP_COMPAT_LOW
                "Moderate, some glitches",  // DP_COMPAT_MEDIUM
                "Slow, few glitches"        // DP_COMPAT_HIGH
            };

            dlg_combo_dp_compat = GetDlgItem(hwnd, IDC_COMBO_DP_COMPAT);
            config_dialog_fill_combo(dlg_combo_dp_compat, dp_compat_strings, DP_COMPAT_NUM, config.dp.compat);

            CONFIG_DLG_INIT_CHECKBOX(IDC_CHECK_MULTITHREAD, dlg_check_multithread, config.parallel);
            CONFIG_DLG_INIT_CHECKBOX(IDC_CHECK_VI_WIDESCREEN, dlg_check_vi_widescreen, config.vi.widescreen);
            CONFIG_DLG_INIT_CHECKBOX(IDC_CHECK_VI_OVERSCAN, dlg_check_vi_overscan, config.vi.hide_overscan);
            CONFIG_DLG_INIT_CHECKBOX(IDC_CHECK_VI_EXCLUSIVE, dlg_check_vi_exclusive, config.vi.exclusive);
            CONFIG_DLG_INIT_CHECKBOX(IDC_CHECK_VI_VSYNC, dlg_check_vi_vsync, config.vi.vsync);
            CONFIG_DLG_INIT_CHECKBOX(IDC_CHECK_VI_INTEGER_SCALING, dlg_check_vi_integer_scaling, config.vi.integer_scaling);

            dlg_edit_workers = GetDlgItem(hwnd, IDC_EDIT_WORKERS);
            SetDlgItemInt(hwnd, IDC_EDIT_WORKERS, config.num_workers, FALSE);

            dlg_spin_workers = GetDlgItem(hwnd, IDC_SPIN_WORKERS);
            SendMessage(dlg_spin_workers, UDM_SETRANGE, 0, MAKELPARAM(128, 0));

            // update enabled/disabled state
            config_dialog_update_multithread();
            config_dialog_update_vi_mode();

            break;
        }
        case WM_COMMAND: {
            WORD cmdid = LOWORD(wParam);
            switch (cmdid) {
                // disable or enable multithreading options when the checkbox is
                // checked or unchecked
                case IDC_CHECK_MULTITHREAD:
                    config_dialog_update_multithread();
                    break;

                // disable overscan option if a non-compatible mode is selected
                case IDC_COMBO_VI_MODE:
                    switch (HIWORD(wParam)) {
                        case CBN_SELCHANGE:
                            config_dialog_update_vi_mode();
                            break;
                    }
                    break;

                // button events
                case IDOK:
                case IDAPPLY:
                    config.vi.mode = SendMessage(dlg_combo_vi_mode, CB_GETCURSEL, 0, 0);
                    config.vi.interp = SendMessage(dlg_combo_vi_interp, CB_GETCURSEL, 0, 0);
                    config.vi.widescreen = SendMessage(dlg_check_vi_widescreen, BM_GETCHECK, 0, 0);
                    config.vi.hide_overscan = SendMessage(dlg_check_vi_overscan, BM_GETCHECK, 0, 0);
                    config.vi.exclusive = SendMessage(dlg_check_vi_exclusive, BM_GETCHECK, 0, 0);
                    config.vi.vsync = SendMessage(dlg_check_vi_vsync, BM_GETCHECK, 0, 0);
                    config.vi.integer_scaling = SendMessage(dlg_check_vi_integer_scaling, BM_GETCHECK, 0, 0);
                    config.dp.compat = SendMessage(dlg_combo_dp_compat, CB_GETCURSEL, 0, 0);
                    config.parallel = SendMessage(dlg_check_multithread, BM_GETCHECK, 0, 0);
                    config.num_workers = GetDlgItemInt(hwnd, IDC_EDIT_WORKERS, FALSE, FALSE);
                    config_stale = true;
                    config_save();

                    // don't close dialog if "Apply" was pressed
                    if (cmdid == IDAPPLY) {
                        break;
                    }
                case IDCANCEL:
                    EndDialog(hwnd, 0);
                    break;
            }
            break;
        }
        default:
            return FALSE;
    }
    return TRUE;
}

static void config_handle(const char* key, const char* value, const char* section)
{
    if (!_strcmpi(section, SECTION_GENERAL)) {
        if (!_strcmpi(key, KEY_GEN_PARALLEL)) {
            config.parallel = strtol(value, NULL, 0) != 0;
        }
        if (!_strcmpi(key, KEY_GEN_NUM_WORKERS)) {
            config.num_workers = strtoul(value, NULL, 0);
        }
    } else if (!_strcmpi(section, SECTION_VIDEO_INTERFACE)) {
        if (!_strcmpi(key, KEY_VI_MODE)) {
            config.vi.mode = strtol(value, NULL, 0);
        } else if (!_strcmpi(key, KEY_VI_INTERP)) {
            config.vi.interp = strtol(value, NULL, 0);
        } else if (!_strcmpi(key, KEY_VI_WIDESCREEN)) {
            config.vi.widescreen = strtol(value, NULL, 0) != 0;
        } else if (!_strcmpi(key, KEY_VI_HIDE_OVERSCAN)) {
            config.vi.hide_overscan = strtol(value, NULL, 0) != 0;
        } else if (!_strcmpi(key, KEY_VI_EXCLUSIVE)) {
            config.vi.exclusive = strtol(value, NULL, 0) != 0;
        } else if (!_strcmpi(key, KEY_VI_VSYNC)) {
            config.vi.vsync = strtol(value, NULL, 0) != 0;
        }
    } else if (!_strcmpi(section, SECTION_DISPLAY_PROCESSOR)) {
        if (!_strcmpi(key, KEY_DP_COMPAT)) {
            config.dp.compat = strtol(value, NULL, 0);
        }
    }
}

void config_init(HINSTANCE hInst)
{
    inst = hInst;
    config_path[0] = 0;

    // create path to config file relative to the DLL path
    GetModuleFileName(inst, config_path, sizeof(config_path));
    PathRemoveFileSpec(config_path);
    PathAppend(config_path, CONFIG_FILE_NAME);

    // load default config
    n64video_config_init(&config);
}

void config_dialog(HWND hParent)
{
    DialogBox(inst, MAKEINTRESOURCE(IDD_DIALOG1), hParent, config_dialog_proc);
}

struct n64video_config* config_get(void)
{
    return &config;
}

bool config_load()
{
    FILE* fp = fopen(config_path, "r");
    if (!fp) {
        return false;
    }

    char line[128];
    char section[128];
    while (fgets(line, sizeof(line), fp) != NULL) {
        // remove newline characters
        size_t trim_pos = strcspn(line, "\r\n");
        line[trim_pos] = 0;

        // ignore blank lines
        size_t len = strlen(line);
        if (!len) {
            continue;
        }

        // key-values
        char* eq_ptr = strchr(line, '=');
        if (eq_ptr) {
            *eq_ptr = 0;
            char* key = line;
            char* value = eq_ptr + 1;
            config_handle(key, value, section);
            continue;
        }

        // sections
        if (line[0] == '[' && line[len - 1] == ']') {
            section[0] = 0;
            strncat(section, line + 1, len - 2);
            continue;
        }
    }

    fclose(fp);

    return true;
}

static void config_write_section(FILE* fp, const char* section)
{
    fprintf(fp, "[%s]\n", section);
}

static void config_write_uint32(FILE* fp, const char* key, uint32_t value)
{
    fprintf(fp, "%s=%u\n", key, value);
}

static void config_write_int32(FILE* fp, const char* key, int32_t value)
{
    fprintf(fp, "%s=%d\n", key, value);
}

bool config_save(void)
{
    FILE* fp = fopen(config_path, "w");
    if (!fp) {
        return false;
    }

    config_write_section(fp, SECTION_GENERAL);
    config_write_int32(fp, KEY_GEN_PARALLEL, config.parallel);
    config_write_uint32(fp, KEY_GEN_NUM_WORKERS, config.num_workers);
    fputs("\n", fp);

    config_write_section(fp, SECTION_VIDEO_INTERFACE);
    config_write_int32(fp, KEY_VI_MODE, config.vi.mode);
    config_write_int32(fp, KEY_VI_INTERP, config.vi.interp);
    config_write_int32(fp, KEY_VI_WIDESCREEN, config.vi.widescreen);
    config_write_int32(fp, KEY_VI_HIDE_OVERSCAN, config.vi.hide_overscan);
    config_write_int32(fp, KEY_VI_EXCLUSIVE, config.vi.exclusive);
    config_write_int32(fp, KEY_VI_VSYNC, config.vi.vsync);
    fputs("\n", fp);

    config_write_section(fp, SECTION_DISPLAY_PROCESSOR);
    config_write_int32(fp, KEY_DP_COMPAT, config.dp.compat);

    fclose(fp);

    return true;
}

void config_update(void)
{
    if (config_stale) {
        vdac_close();
        screen_close();
        n64video_close();

        n64video_init(&config);
        screen_init(&config);
        vdac_init(&config);

        config_stale = false;
    }
}
