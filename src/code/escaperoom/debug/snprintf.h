#ifndef SNPRINTF_H
#define SNPRINTF_H

#include "ultra64.h"

int vsnprintf(char* dst, size_t size, const char* fmt, va_list args);
int snprintf(char* dst, size_t size, const char* fmt, ...);

#endif
