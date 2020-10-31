#pragma once

typedef void (*funcptr)(void);

funcptr IntGetProcAddress(const char* name);
