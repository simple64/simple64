#include "core/msg.h"
#include "core/version.h"

#include <Windows.h>

#include <stdio.h>
#include <stdarg.h>

#define MSG_BUFFER_LEN 256

void msg_error(const char * err, ...)
{
    va_list arg;
    va_start(arg, err);
    char buf[MSG_BUFFER_LEN];
    vsprintf_s(buf, sizeof(buf), err, arg);
    MessageBoxA(0, buf, CORE_SIMPLE_NAME ": fatal error", MB_OK);
    va_end(arg);
    exit(0);
}

void msg_warning(const char* err, ...)
{
    va_list arg;
    va_start(arg, err);
    char buf[MSG_BUFFER_LEN];
    vsprintf_s(buf, sizeof(buf), err, arg);
    MessageBoxA(0, buf, CORE_SIMPLE_NAME ": warning", MB_OK);
    va_end(arg);
}

void msg_debug(const char* err, ...)
{
    va_list arg;
    va_start(arg, err);
    char buf[MSG_BUFFER_LEN];
    vsprintf_s(buf, sizeof(buf), err, arg);
    strcat_s(buf, sizeof(buf), "\n");
    OutputDebugStringA(buf);
    va_end(arg);
}
