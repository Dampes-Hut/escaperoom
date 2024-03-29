#ifndef ULTRA64_LIBC_H
#define ULTRA64_LIBC_H

#include "stddef.h"

#ifdef osSyncPrintf
#error "bad header order"
#endif

#ifdef __GNUC__
__attribute__((format(printf, 1, 2)))
#endif
void osSyncPrintf(const char* fmt, ...);

void bzero(void* __s, size_t __n);
int bcmp(const void* __sl, const void* __s2, size_t __n);
void bcopy(const void* __src, void* __dest, size_t __n);

#endif
