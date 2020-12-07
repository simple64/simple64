#include "QtKeyToHID.h"
#include "keycode/keycode.h"

unsigned int QtKeyToHID(quint32 key)
{
    unsigned int returnValue = KEY_None;

    switch (key)
    {
        default:
            returnValue = KEY_None;
            break;
        case Qt::Key::Key_A:
            returnValue = KEY_A;
            break;
        case Qt::Key::Key_B:
            returnValue = KEY_B;
            break;
        case Qt::Key::Key_C:
            returnValue = KEY_C;
            break;
        case Qt::Key::Key_D:
            returnValue = KEY_D;
            break;
        case Qt::Key::Key_E:
            returnValue = KEY_E;
            break;
        case Qt::Key::Key_F:
            returnValue = KEY_F;
            break;
        case Qt::Key::Key_G:
            returnValue = KEY_G;
            break;
        case Qt::Key::Key_H:
            returnValue = KEY_H;
            break;
        case Qt::Key::Key_I:
            returnValue = KEY_I;
            break;
        case Qt::Key::Key_J:
            returnValue = KEY_J;
            break;
        case Qt::Key::Key_K:
            returnValue = KEY_K;
            break;
        case Qt::Key::Key_L:
            returnValue = KEY_L;
            break;
        case Qt::Key::Key_M:
            returnValue = KEY_M;
            break;
        case Qt::Key::Key_N:
            returnValue = KEY_N;
            break;
        case Qt::Key::Key_O:
            returnValue = KEY_O;
            break;
        case Qt::Key::Key_P:
            returnValue = KEY_P;
            break;
        case Qt::Key::Key_Q:
            returnValue = KEY_Q;
            break;
        case Qt::Key::Key_R:
            returnValue = KEY_R;
            break;
        case Qt::Key::Key_S:
            returnValue = KEY_S;
            break;
        case Qt::Key::Key_T:
            returnValue = KEY_T;
            break;
        case Qt::Key::Key_U:
            returnValue = KEY_U;
            break;
        case Qt::Key::Key_V:
            returnValue = KEY_V;
            break;
        case Qt::Key::Key_W:
            returnValue = KEY_W;
            break;
        case Qt::Key::Key_X:
            returnValue = KEY_X;
            break;
        case Qt::Key::Key_Y:
            returnValue = KEY_Y;
            break;
        case Qt::Key::Key_Z:
            returnValue = KEY_Z;
            break;
        case Qt::Key::Key_1:
            returnValue = KEY_1;
            break;
        case Qt::Key::Key_2:
            returnValue = KEY_2;
            break;
        case Qt::Key::Key_3:
            returnValue = KEY_3;
            break;
        case Qt::Key::Key_4:
            returnValue = KEY_4;
            break;
        case Qt::Key::Key_5:
            returnValue = KEY_5;
            break;
        case Qt::Key::Key_6:
            returnValue = KEY_6;
            break;
        case Qt::Key::Key_7:
            returnValue = KEY_7;
            break;
        case Qt::Key::Key_8:
            returnValue = KEY_8;
            break;
        case Qt::Key::Key_9:
            returnValue = KEY_9;
            break;
        case Qt::Key::Key_0:
            returnValue = KEY_0;
            break;
        case Qt::Key::Key_Escape:
            returnValue = KEY_Escape;
            break;
        case Qt::Key::Key_Delete:
            returnValue = KEY_Delete;
            break;
        case Qt::Key::Key_Tab:
            returnValue = KEY_Tab;
            break;
        case Qt::Key::Key_Space:
            returnValue = KEY_Space;
            break;
        case Qt::Key::Key_Minus:
            returnValue = KEY_Minus;
            break;
        case Qt::Key::Key_Equal:
            returnValue = KEY_Equals;
            break;
        case Qt::Key::Key_BracketLeft:
            returnValue = KEY_LeftBracket;
            break;
        case Qt::Key::Key_BracketRight:
            returnValue = KEY_RightBracket;
            break;
        case Qt::Key::Key_Backslash:
            returnValue = KEY_Backslash;
            break;
        case Qt::Key::Key_Semicolon:
            returnValue = KEY_Semicolon;
            break;
        case Qt::Key::Key_QuoteDbl:
            returnValue = KEY_Quote;
            break;
        case Qt::Key::Key_Dead_Grave:
            returnValue = KEY_Grave;
            break;
        case Qt::Key::Key_Comma:
            returnValue = KEY_Comma;
            break;
        case Qt::Key::Key_Period:
            returnValue = KEY_Period;
            break;
        case Qt::Key::Key_Slash:
            returnValue = KEY_Slash;
            break;
        case Qt::Key::Key_CapsLock:
            returnValue = KEY_CapsLock;
            break;
        case Qt::Key::Key_F1:
            returnValue = KEY_F1;
            break;
        case Qt::Key::Key_F2:
            returnValue = KEY_F2;
            break;
        case Qt::Key::Key_F3:
            returnValue = KEY_F3;
            break;
        case Qt::Key::Key_F4:
            returnValue = KEY_F4;
            break;
        case Qt::Key::Key_F5:
            returnValue = KEY_F5;
            break;
        case Qt::Key::Key_F6:
            returnValue = KEY_F6;
            break;
        case Qt::Key::Key_F7:
            returnValue = KEY_F7;
            break;
        case Qt::Key::Key_F8:
            returnValue = KEY_F8;
            break;
        case Qt::Key::Key_F9:
            returnValue = KEY_F9;
            break;
        case Qt::Key::Key_F10:
            returnValue = KEY_F10;
            break;
        case Qt::Key::Key_F11:
            returnValue = KEY_F11;
            break;
        case Qt::Key::Key_F12:
            returnValue = KEY_F12;
            break;
        /* TODO, this is a META key 
        case Qt::Key::Key_Screen:
            returnValue = KEY_PrintScreen;
            break;*/
        case Qt::Key::Key_ScrollLock:
            returnValue = KEY_ScrollLock;
            break;
        case Qt::Key::Key_Pause:
            returnValue = KEY_Pause;
            break;
        case Qt::Key::Key_Insert:
            returnValue = KEY_Insert;
            break;
        case Qt::Key::Key_Home:
            returnValue = KEY_Home;
            break;
        case Qt::Key::Key_PageUp:
            returnValue = KEY_PageUp;
            break;
        /* ???
        case Qt::Key::Key_Delete:
            returnValue = KEY_DeleteForward;
            break;*/
        case Qt::Key::Key_End:
            returnValue = KEY_End;
            break;
        case Qt::Key::Key_PageDown:
            returnValue = KEY_PageDown;
            break;
        case Qt::Key::Key_Right:
            returnValue = KEY_Right;
            break;
        case Qt::Key::Key_Left:
            returnValue = KEY_Left;
            break;
        case Qt::Key::Key_Down:
            returnValue = KEY_Down;
            break;
        case Qt::Key::Key_Up:
            returnValue = KEY_Up;
            break;
        case Qt::Key::Key_NumLock:
            returnValue = KP_NumLock;
            break;
        case Qt::Key::Key_division:
            returnValue = KP_Divide;
            break;
        case Qt::Key::Key_multiply:
            returnValue = KP_Multiply;
            break;
        /* ???
        case Qt::Key::Key_Minus:
            returnValue = KP_Subtract;
            break; */
        case Qt::Key::Key_Plus:
            returnValue = KP_Add;
            break;
        case Qt::Key::Key_Enter:
            returnValue = KP_Enter;
            break;
        /* see https://bugreports.qt.io/browse/QTBUG-2913
        case Qt::Key::
            returnValue = KP_1;
            break;
        case Qt::Key::
            returnValue = KP_2;
            break;
        case Qt::Key::
            returnValue = KP_3;
            break;
        case Qt::Key::
            returnValue = KP_4;
            break;
        case Qt::Key::
            returnValue = KP_5;
            break;
        case Qt::Key::
            returnValue = KP_6;
            break;
        case Qt::Key::
            returnValue = KP_7;
            break;
        case Qt::Key::
            returnValue = KP_8;
            break;
        case Qt::Key::
            returnValue = KP_9;
            break;
        case Qt::Key::
            returnValue = KP_0;
            break;
        case Qt::Key::
            returnValue = KP_Point;
            break;
        // ???
        case Qt::Key::
            returnValue = KEY_NonUSBackslash;
            break;
        case Qt::Key::
            returnValue = KP_Equals;
            break;
        */
        case Qt::Key::Key_F13:
            returnValue = KEY_F13;
            break;
        case Qt::Key::Key_F14:
            returnValue = KEY_F14;
            break;
        case Qt::Key::Key_F15:
            returnValue = KEY_F15;
            break;
        case Qt::Key::Key_F16:
            returnValue = KEY_F16;
            break;
        case Qt::Key::Key_F17:
            returnValue = KEY_F17;
            break;
        case Qt::Key::Key_F18:
            returnValue = KEY_F18;
            break;
        case Qt::Key::Key_F19:
            returnValue = KEY_F19;
            break;
        case Qt::Key::Key_F20:
            returnValue = KEY_F20;
            break;
        case Qt::Key::Key_F21:
            returnValue = KEY_F21;
            break;
        case Qt::Key::Key_F22:
            returnValue = KEY_F22;
            break;
        case Qt::Key::Key_F23:
            returnValue = KEY_F23;
            break;
        case Qt::Key::Key_F24:
            returnValue = KEY_F24;
            break;
        case Qt::Key::Key_Help:
            returnValue = KEY_Help;
            break;
        case Qt::Key::Key_Menu:
            returnValue = KEY_Menu;
            break;
        case Qt::Key::Key_VolumeMute:
            returnValue = KEY_Mute;
            break;
        case Qt::Key::Key_SysReq:
            returnValue = KEY_SysReq;
            break;
        case Qt::Key::Key_Return:
            returnValue = KEY_Return;
            break;
        case Qt::Key::Key_Clear:
            returnValue = KP_Clear;
            break;
        /* ???
        case Qt::Key::Key_d:
            returnValue = KP_Decimal;
            break; */
        case Qt::Key::Key_Control:
            returnValue = KEY_LeftControl;
            break;
        case Qt::Key::Key_Shift:
            returnValue = KEY_LeftShift;
            break;
        case Qt::Key::Key_Alt:
            returnValue = KEY_LeftAlt;
            break;
        case Qt::Key::Key_Super_L:
            returnValue = KEY_LeftGUI;
            break;
        /*case Qt::Key::Key_Control:
            returnValue = KEY_RightControl;
            break;*/
        /*case Qt::Key::Key_Shift:
            returnValue = KEY_RightShift;
            break;*/
        case Qt::Key::Key_AltGr:
            returnValue = KEY_RightAlt;
            break;
        case Qt::Key::Key_Super_R:
            returnValue = KEY_RightGUI;
            break;

    }

    return returnValue;
}
