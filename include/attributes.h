#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#if !defined(__GNUC__) && !defined(__attribute__)
#define __attribute__(x)
#endif

#define UNUSED                  __attribute__((unused))
#define FALLTHROUGH             __attribute__((fallthrough))
#define NORETURN                __attribute__((noreturn))
#define FORMAT_PRINTF(fmt,args) __attribute__((format(printf, fmt, args)))
#define FORMAT_VPRINTF(fmt)     __attribute__((format(printf, fmt, 0)))

#endif
