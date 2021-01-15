#include "osal_keys.h"

#include <linux/limits.h>
#include <linux/input.h>
#include <dirent.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

static const unsigned char LINUX_HID_TO_NATIVE[256] = {
	255, 255, 255, 255, KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, 
	KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, 
	KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, 
	KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z, KEY_1, KEY_2, KEY_3,
	KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0, 255, KEY_ESC,
	KEY_DELETE, KEY_TAB, KEY_SPACE, KEY_MINUS, KEY_EQUAL, KEY_LEFTBRACE,
	KEY_RIGHTBRACE, KEY_BACKSLASH, 255, KEY_SEMICOLON, KEY_APOSTROPHE,
	KEY_GRAVE, KEY_COMMA, KEY_DOT, KEY_SLASH, KEY_CAPSLOCK,
	KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9,
	KEY_F10, KEY_F11, KEY_F12, KEY_PRINT, KEY_SCROLLLOCK, KEY_PAUSE,
	KEY_INSERT, KEY_HOME, KEY_PAGEUP, KEY_DELETE,
	KEY_END, KEY_PAGEDOWN, KEY_RIGHT, KEY_LEFT, KEY_DOWN, KEY_UP,
	KEY_NUMLOCK, KEY_KPSLASH, KEY_KPASTERISK, KEY_KPMINUS,
	KEY_KPPLUS, KEY_KPENTER, KEY_KP1, KEY_KP2, KEY_KP3, KEY_KP4,
	KEY_KP5, KEY_KP6, KEY_KP7, KEY_KP8, KEY_KP9, KEY_KP0, KEY_KPDOT,
	KEY_BACKSLASH, KEY_KPEQUAL, KEY_F13, KEY_F14, KEY_F15, KEY_F16,
	KEY_F17, KEY_F18, KEY_F19, KEY_F20, KEY_F21, KEY_F22, KEY_F23,
	KEY_F24, 255, KEY_HELP, KEY_MENU, 255, 255, 255, 255, 255, 255, 
	255, 255, KEY_MUTE, KEY_SYSRQ, KEY_ENTER, 
	255 /* KP_Clear */, KEY_KPCOMMA,
	KEY_LEFTCTRL, KEY_LEFTSHIFT, KEY_LEFTALT, KEY_LEFTMETA,
	KEY_RIGHTCTRL, KEY_RIGHTSHIFT, KEY_RIGHTALT, KEY_RIGHTMETA,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255
};

static const char* LINUX_HID_NAME[256] = {
	0, 0, 0, 0, "A", "B", "C", "D", "E", 
	"F", "G", "H", "I", "J", "K", "L", "M", 
	"N", "O", "P", "Q", "R", "S", "T", "U", 
	"V", "W", "X", "Y", "Z", "1", "2", "3",
	"4", "5", "6", "7", "8", "9", "0", 0,
	"Escape", "Delete", "Tab", "Space", "-", "=",
	"[", "]", "Backslash", 0, ";",
	"'", "Grave", ",", ".", "/", "Capslock", 
	"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9",
	"F10", "F11", "F12", "Print", "Scrolllock", "Pause",
	"Insert", "Home", "Page Up", "Delete", "End", "Page Down",
	"Right", "Left", "Down", "Up", "Numlock", "/", "*", "-", "+", 
	"Enter", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", ".",
	"\\", "=", "F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20",
	"F21", "F22", "F23", "F24", 0, "Help", "Menu", 0, 0, 0, 0, 0, 0, 
	0, 0, "Mute", "Sysrq", "Enter", "Clear", ",", "Left Control", "Left Shift",
	"Left Alt", "Left Meta", "Right Control", "Right Shift", "Right Alt", "Right Meta",
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

typedef struct {
	FILE* file;
	char key_map[KEY_MAX/8 + 1];
	int last_key;
} keyboard_t;

#define KEYBOARD_MAX 4
#define DEVINPUTPATH "/dev/input/by-id"
#define KEYBOARD_VAR "GLIDEN64_KEYBOARD"

static keyboard_t l_Keyboards[KEYBOARD_MAX] = { 0 };
static int l_KeyBoardCount = 0;

EXPORT void CALL osal_keys_init()
{
	DIR* dir;
	struct dirent* entry;
	char file_name[PATH_MAX];
	char real_file_name[PATH_MAX];
	char* env_var;
	FILE* kbd_file;
	int kbd_index = 0;

	/* if a keyboard file has been specified, use that */
	if ((env_var = getenv(KEYBOARD_VAR)) != NULL)
	{
		l_Keyboards[0].file = fopen(env_var, "r");
		l_KeyBoardCount++;
		return;
	}

	dir = opendir(DEVINPUTPATH);
	if (dir == NULL)
		return;

	while ((entry = readdir(dir)) != NULL) {
		sprintf(file_name, "%s/%s", DEVINPUTPATH, entry->d_name);

		/* check if file_name contains kbd */
		if (strstr(file_name, "kbd") != NULL) { 
			/* follow symlink and get full path */
			if (realpath(file_name, real_file_name) == NULL)
				continue;

			/* attempt to open the file */
			kbd_file = l_Keyboards[kbd_index].file = fopen(real_file_name, "r");
			if (kbd_file != NULL) { 
				/* if we can open it, use it */
				if (kbd_index++ >= KEYBOARD_MAX)
					break;
			}
		}
	}

	l_KeyBoardCount = kbd_index;
	closedir(dir);
}

EXPORT void CALL osal_keys_quit()
{
	FILE* file;
	for (int i = 0; i < l_KeyBoardCount; i++) {
		file = l_Keyboards[i].file;
		if (file != NULL)
			fclose(file);
	}
}

EXPORT void CALL osal_keys_update_state()
{
#ifdef OS_LINUX
	keyboard_t* keyboard;
	for (int i = 0; i < l_KeyBoardCount; i++) {
		keyboard = &l_Keyboards[i];
		if (keyboard->file == NULL)
			continue;
		
		// query keyboard state
		ioctl(fileno(keyboard->file), EVIOCGKEY(sizeof(keyboard->key_map)), keyboard->key_map);
	}
#endif // OS_LINUX
}

EXPORT unsigned int CALL osal_is_key_pressed(unsigned int _key, unsigned int _mask)
{
	if (_key == 0 || _key > 255)
		return 0;

	int key = LINUX_HID_TO_NATIVE[_key];

	keyboard_t* keyboard;
	int keys, mask;
	for (int i = 0; i < l_KeyBoardCount; i++) {
		keyboard = &l_Keyboards[i];
		if (keyboard->file == NULL)
			continue;
		
		keys = keyboard->key_map[key/8];
		mask = 1 << (key % 8);

		if ((keys & mask)) {
			/* if key is pressed, but key hasn't been released yet, return */
			if (keyboard->last_key == key)
				return 0;
			keyboard->last_key = key;
			return 1;
		}
		else if (keyboard->last_key == key) {
			/* if key has been let go, remove last_key */
			keyboard->last_key = 0;
		}
	}

	return 0;
}

EXPORT const char * CALL osal_keycode_name(unsigned int _hidCode)
{
	return LINUX_HID_NAME[_hidCode];
}

#ifdef __cplusplus
}
#endif
