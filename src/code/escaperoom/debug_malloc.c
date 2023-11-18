#include "global.h"

#define LOG_SEVERITY_NOLOG 0
#define LOG_SEVERITY_ERROR 2
#define LOG_SEVERITY_VERBOSE 3

s32 gDebugArenaLogSeverity = LOG_SEVERITY_ERROR;
Arena sDebugArena;

static void DebugArena_CheckPointer(void* ptr, u32 size, const char* name, const char* action) {
    if (ptr == NULL) {
        if (gDebugArenaLogSeverity >= LOG_SEVERITY_ERROR) {
            rmonPrintf("[debug_malloc] %s: %u bytes %s failed to allocate\n", name, size, action);
            __osDisplayArena(&sDebugArena);
        }
    } else if (gDebugArenaLogSeverity >= LOG_SEVERITY_VERBOSE) {
        rmonPrintf("[debug_malloc] %s: %u bytes %s allocated\n", name, size, action);
    }
}

void* DebugArena_Malloc(u32 size FILE_LINE_DECL) {
    void* ptr = __osMalloc(&sDebugArena, size FILE_LINE_ARGS);

    DebugArena_CheckPointer(ptr, size, "DebugArena_Malloc", "ALLOC");
    return ptr;
}

void* DebugArena_MallocR(u32 size FILE_LINE_DECL) {
    void* ptr = __osMallocR(&sDebugArena, size FILE_LINE_ARGS);

    DebugArena_CheckPointer(ptr, size, "DebugArena_MallocR", "ALLOC");
    return ptr;
}

void* DebugArena_Realloc(void* ptr, u32 newSize FILE_LINE_DECL) {
    ptr = __osRealloc(&sDebugArena, ptr, newSize FILE_LINE_ARGS);
    DebugArena_CheckPointer(ptr, newSize, "DebugArena_Realloc", "REALLOC");
    return ptr;
}

void DebugArena_Free(void* ptr FILE_LINE_DECL) {
    __osFree(&sDebugArena, ptr FILE_LINE_ARGS);
}

void* DebugArena_Calloc(u32 num, u32 size FILE_LINE_DECL) {
    void* ret;
    u32 n = num * size;

    ret = __osMalloc(&sDebugArena, n FILE_LINE_ARGS);
    if (ret != NULL) {
        bzero(ret, n);
    }

    DebugArena_CheckPointer(ret, n, "DebugArena_Calloc", "CALLOC");
    return ret;
}

void DebugArena_Display(void) {
    rmonPrintf("[debug_malloc] DEBUG ARENA DISPLAY\n");
    __osDisplayArena(&sDebugArena);
}

void DebugArena_GetSizes(u32* outMaxFree, u32* outFree, u32* outAlloc) {
    ArenaImpl_GetSizes(&sDebugArena, outMaxFree, outFree, outAlloc);
}

void DebugArena_Check(void) {
    __osCheckArena(&sDebugArena);
}

void DebugArena_Init(void* start, u32 size) {
    gDebugArenaLogSeverity = LOG_SEVERITY_NOLOG;
    __osMallocInit(&sDebugArena, start, size);
}

void DebugArena_Cleanup(void) {
    gDebugArenaLogSeverity = LOG_SEVERITY_NOLOG;
    __osMallocCleanup(&sDebugArena);
}

u8 DebugArena_IsInitialized(void) {
    return __osMallocIsInitialized(&sDebugArena);
}
