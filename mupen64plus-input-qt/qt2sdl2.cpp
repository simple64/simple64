#include "qt2sdl2.h"
#include <SDL2/SDL.h>

int QT2SDL2MOD(Qt::KeyboardModifiers modifiers)
{
    int value = 0;
    if (modifiers & Qt::ShiftModifier)
        value |= KMOD_SHIFT;
    if (modifiers & Qt::ControlModifier)
        value |= KMOD_CTRL;
    if (modifiers & Qt::AltModifier)
        value |= KMOD_ALT;
    if (modifiers & Qt::MetaModifier)
        value |= KMOD_GUI;
    return value;
}

int QT2SDL2(int qtKey)
{
    SDL_Scancode returnValue;
    switch (qtKey) {
    case Qt::Key_Escape:
        returnValue = SDL_SCANCODE_ESCAPE;
        break;
    case Qt::Key_Tab:
        returnValue = SDL_SCANCODE_TAB;
        break;
    case Qt::Key_Backspace:
        returnValue = SDL_SCANCODE_BACKSPACE;
        break;
    case Qt::Key_Return:
        returnValue = SDL_SCANCODE_RETURN;
        break;
    case Qt::Key_Enter:
        returnValue = SDL_SCANCODE_KP_ENTER;
        break;
    case Qt::Key_Insert:
        returnValue = SDL_SCANCODE_INSERT;
        break;
    case Qt::Key_Delete:
        returnValue = SDL_SCANCODE_DELETE;
        break;
    case Qt::Key_Pause:
        returnValue = SDL_SCANCODE_PAUSE;
        break;
    case Qt::Key_Print:
        returnValue = SDL_SCANCODE_PRINTSCREEN;
        break;
    case Qt::Key_SysReq:
        returnValue = SDL_SCANCODE_SYSREQ;
        break;
    case Qt::Key_Clear:
        returnValue = SDL_SCANCODE_CLEAR;
        break;
    case Qt::Key_Home:
        returnValue = SDL_SCANCODE_HOME;
        break;
    case Qt::Key_End:
        returnValue = SDL_SCANCODE_END;
        break;
    case Qt::Key_Left:
        returnValue = SDL_SCANCODE_LEFT;
        break;
    case Qt::Key_Right:
        returnValue = SDL_SCANCODE_RIGHT;
        break;
    case Qt::Key_Up:
        returnValue = SDL_SCANCODE_UP;
        break;
    case Qt::Key_Down:
        returnValue = SDL_SCANCODE_DOWN;
        break;
    case Qt::Key_PageUp:
        returnValue = SDL_SCANCODE_PAGEUP;
        break;
    case Qt::Key_PageDown:
        returnValue = SDL_SCANCODE_PAGEDOWN;
        break;
    case Qt::Key_Shift:
        returnValue = SDL_SCANCODE_LSHIFT;
        break;
    case Qt::Key_Control:
        returnValue = SDL_SCANCODE_LCTRL;
        break;
    case Qt::Key_Meta:
        returnValue = SDL_SCANCODE_LGUI;
        break;
    case Qt::Key_Alt:
        returnValue = SDL_SCANCODE_LALT;
        break;
    case Qt::Key_AltGr:
        returnValue = SDL_SCANCODE_RALT;
        break;
    case Qt::Key_CapsLock:
        returnValue = SDL_SCANCODE_CAPSLOCK;
        break;
    case Qt::Key_NumLock:
        returnValue = SDL_SCANCODE_NUMLOCKCLEAR;
        break;
    case Qt::Key_ScrollLock:
        returnValue = SDL_SCANCODE_SCROLLLOCK;
        break;
    case Qt::Key_F1:
        returnValue = SDL_SCANCODE_F1;
        break;
    case Qt::Key_F2:
        returnValue = SDL_SCANCODE_F2;
        break;
    case Qt::Key_F3:
        returnValue = SDL_SCANCODE_F3;
        break;
    case Qt::Key_F4:
        returnValue = SDL_SCANCODE_F4;
        break;
    case Qt::Key_F5:
        returnValue = SDL_SCANCODE_F5;
        break;
    case Qt::Key_F6:
        returnValue = SDL_SCANCODE_F6;
        break;
    case Qt::Key_F7:
        returnValue = SDL_SCANCODE_F7;
        break;
    case Qt::Key_F8:
        returnValue = SDL_SCANCODE_F8;
        break;
    case Qt::Key_F9:
        returnValue = SDL_SCANCODE_F9;
        break;
    case Qt::Key_F10:
        returnValue = SDL_SCANCODE_F10;
        break;
    case Qt::Key_F11:
        returnValue = SDL_SCANCODE_F11;
        break;
    case Qt::Key_F12:
        returnValue = SDL_SCANCODE_F12;
        break;
    case Qt::Key_F13:
        returnValue = SDL_SCANCODE_F13;
        break;
    case Qt::Key_F14:
        returnValue = SDL_SCANCODE_F14;
        break;
    case Qt::Key_F15:
        returnValue = SDL_SCANCODE_F15;
        break;
    case Qt::Key_F16:
        returnValue = SDL_SCANCODE_F16;
        break;
    case Qt::Key_F17:
        returnValue = SDL_SCANCODE_F17;
        break;
    case Qt::Key_F18:
        returnValue = SDL_SCANCODE_F18;
        break;
    case Qt::Key_F19:
        returnValue = SDL_SCANCODE_F19;
        break;
    case Qt::Key_F20:
        returnValue = SDL_SCANCODE_F20;
        break;
    case Qt::Key_F21:
        returnValue = SDL_SCANCODE_F21;
        break;
    case Qt::Key_F22:
        returnValue = SDL_SCANCODE_F22;
        break;
    case Qt::Key_F23:
        returnValue = SDL_SCANCODE_F23;
        break;
    case Qt::Key_F24:
        returnValue = SDL_SCANCODE_F24;
        break;
    case Qt::Key_Space:
        returnValue = SDL_SCANCODE_SPACE;
        break;
    case Qt::Key_0:
        returnValue = SDL_SCANCODE_0;
        break;
    case Qt::Key_1:
        returnValue = SDL_SCANCODE_1;
        break;
    case Qt::Key_2:
        returnValue = SDL_SCANCODE_2;
        break;
    case Qt::Key_3:
        returnValue = SDL_SCANCODE_3;
        break;
    case Qt::Key_4:
        returnValue = SDL_SCANCODE_4;
        break;
    case Qt::Key_5:
        returnValue = SDL_SCANCODE_5;
        break;
    case Qt::Key_6:
        returnValue = SDL_SCANCODE_6;
        break;
    case Qt::Key_7:
        returnValue = SDL_SCANCODE_7;
        break;
    case Qt::Key_8:
        returnValue = SDL_SCANCODE_8;
        break;
    case Qt::Key_9:
        returnValue = SDL_SCANCODE_9;
        break;
    case Qt::Key_A:
        returnValue = SDL_SCANCODE_A;
        break;
    case Qt::Key_B:
        returnValue = SDL_SCANCODE_B;
        break;
    case Qt::Key_C:
        returnValue = SDL_SCANCODE_C;
        break;
    case Qt::Key_D:
        returnValue = SDL_SCANCODE_D;
        break;
    case Qt::Key_E:
        returnValue = SDL_SCANCODE_E;
        break;
    case Qt::Key_F:
        returnValue = SDL_SCANCODE_F;
        break;
    case Qt::Key_G:
        returnValue = SDL_SCANCODE_G;
        break;
    case Qt::Key_H:
        returnValue = SDL_SCANCODE_H;
        break;
    case Qt::Key_I:
        returnValue = SDL_SCANCODE_I;
        break;
    case Qt::Key_J:
        returnValue = SDL_SCANCODE_J;
        break;
    case Qt::Key_K:
        returnValue = SDL_SCANCODE_K;
        break;
    case Qt::Key_L:
        returnValue = SDL_SCANCODE_L;
        break;
    case Qt::Key_M:
        returnValue = SDL_SCANCODE_M;
        break;
    case Qt::Key_N:
        returnValue = SDL_SCANCODE_N;
        break;
    case Qt::Key_O:
        returnValue = SDL_SCANCODE_O;
        break;
    case Qt::Key_P:
        returnValue = SDL_SCANCODE_P;
        break;
    case Qt::Key_Q:
        returnValue = SDL_SCANCODE_Q;
        break;
    case Qt::Key_R:
        returnValue = SDL_SCANCODE_R;
        break;
    case Qt::Key_S:
        returnValue = SDL_SCANCODE_S;
        break;
    case Qt::Key_T:
        returnValue = SDL_SCANCODE_T;
        break;
    case Qt::Key_U:
        returnValue = SDL_SCANCODE_U;
        break;
    case Qt::Key_V:
        returnValue = SDL_SCANCODE_V;
        break;
    case Qt::Key_W:
        returnValue = SDL_SCANCODE_W;
        break;
    case Qt::Key_X:
        returnValue = SDL_SCANCODE_X;
        break;
    case Qt::Key_Y:
        returnValue = SDL_SCANCODE_Y;
        break;
    case Qt::Key_Z:
        returnValue = SDL_SCANCODE_Z;
        break;
    case Qt::Key_BracketLeft:
        returnValue = SDL_SCANCODE_LEFTBRACKET;
        break;
    case Qt::Key_BracketRight:
        returnValue = SDL_SCANCODE_RIGHTBRACKET;
        break;
    case Qt::Key_Minus:
        returnValue = SDL_SCANCODE_MINUS;
        break;
    case Qt::Key_Semicolon:
        returnValue = SDL_SCANCODE_SEMICOLON;
        break;
    case Qt::Key_Slash:
        returnValue = SDL_SCANCODE_SLASH;
        break;
    case Qt::Key_Backslash:
        returnValue = SDL_SCANCODE_BACKSLASH;
        break;
    case Qt::Key_Apostrophe:
        returnValue = SDL_SCANCODE_APOSTROPHE;
        break;
    case Qt::Key_Comma:
        returnValue = SDL_SCANCODE_COMMA;
        break;
    case Qt::Key_Period:
        returnValue = SDL_SCANCODE_PERIOD;
        break;
    case Qt::Key_Equal:
        returnValue = SDL_SCANCODE_EQUALS;
        break;
    case Qt::Key_QuoteLeft:
        returnValue = SDL_SCANCODE_GRAVE;
        break;
    default:
        returnValue = SDL_SCANCODE_UNKNOWN;
        break;
    }

    return returnValue;
}
