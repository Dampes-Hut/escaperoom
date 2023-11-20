#include "global.h"

NORETURN void __assert(const char* assertion, const char* file, int line) {
    char msg[256];

    osSyncPrintf("Assertion failed: %s, file %s, line %d, thread %d\n", assertion, file, line,
                 (int)osGetThreadId(NULL));
    sprintf(msg, "ASSERT: %s:%d(%d)", file, line, (int)osGetThreadId(NULL));
    Fault_AddHungupAndCrashImpl(msg, assertion);
}

void __assert_soft(const char* assertion, const char* file, int line) {
    char msg[256];

    osSyncPrintf("Soft Assertion failed: %s, file %s, line %d, thread %d\n", assertion, file, line,
                 (int)osGetThreadId(NULL));
    sprintf(msg, "SOFT_ASSERT: %s:%d(%d)", file, line, (int)osGetThreadId(NULL));
    Fault_AddHungupAndCrashRestartable(msg, assertion);
}
