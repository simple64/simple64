#include "core/msg.h"
#include "core/version.h"
#include "gfx_m64p.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define MSG_BUFFER_LEN 256

void msg_error(const char * err, ...)
{
    if (debug_callback == NULL)
        return;

    va_list arg;
    va_start(arg, err);
    char buf[MSG_BUFFER_LEN];
    vsprintf(buf, err, arg);

    (*debug_callback)(debug_call_context, M64MSG_ERROR, buf);

    va_end(arg);
    exit(0);
}

void msg_warning(const char* err, ...)
{
    if (debug_callback == NULL)
        return;

    va_list arg;
    va_start(arg, err);
    char buf[MSG_BUFFER_LEN];
    vsprintf(buf, err, arg);

    (*debug_callback)(debug_call_context, M64MSG_WARNING, buf);

    va_end(arg);
}

void msg_debug(const char* err, ...)
{
    if (debug_callback == NULL)
        return;

    va_list arg;
    va_start(arg, err);
    char buf[MSG_BUFFER_LEN];
    vsprintf(buf, err, arg);

    (*debug_callback)(debug_call_context, M64MSG_VERBOSE, buf);

    va_end(arg);
}
