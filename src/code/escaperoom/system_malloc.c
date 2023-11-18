#include "global.h"

#define LOG_SEVERITY_NOLOG 0
#define LOG_SEVERITY_ERROR 2
#define LOG_SEVERITY_VERBOSE 3

s32 gSystemArenaLogSeverity = LOG_SEVERITY_NOLOG;
Arena gSystemArena;

static void SystemArena_CheckPointer(void* ptr, u32 size, const char* name, const char* action) {
    if (ptr == NULL) {
        if (gSystemArenaLogSeverity >= LOG_SEVERITY_ERROR) {
            rmonPrintf("[system_malloc] %s: %u bytes %s failed to allocate\n", name, size, action);
            __osDisplayArena(&gSystemArena);
        }
    } else if (gSystemArenaLogSeverity >= LOG_SEVERITY_VERBOSE) {
        rmonPrintf("[system_malloc] %s: %u bytes %s allocated\n", name, size, action);
    }
}

void* SystemArena_Malloc(u32 size FILE_LINE_DECL) {
    void* ptr = __osMalloc(&gSystemArena, size FILE_LINE_ARGS);

    SystemArena_CheckPointer(ptr, size, "SystemArena_Malloc", "ALLOC");
    return ptr;
}

void* SystemArena_MallocR(u32 size FILE_LINE_DECL) {
    void* ptr = __osMallocR(&gSystemArena, size FILE_LINE_ARGS);

    SystemArena_CheckPointer(ptr, size, "SystemArena_MallocR", "ALLOC");
    return ptr;
}

void* SystemArena_Realloc(void* ptr, u32 newSize FILE_LINE_DECL) {
    ptr = __osRealloc(&gSystemArena, ptr, newSize FILE_LINE_ARGS);
    SystemArena_CheckPointer(ptr, newSize, "SystemArena_Realloc", "REALLOC");
    return ptr;
}

void SystemArena_Free(void* ptr FILE_LINE_DECL) {
    __osFree(&gSystemArena, ptr FILE_LINE_ARGS);
}

void* SystemArena_Calloc(u32 num, u32 size FILE_LINE_DECL) {
    void* ret;
    u32 n = num * size;

    ret = __osMalloc(&gSystemArena, n FILE_LINE_ARGS);
    if (ret != NULL) {
        bzero(ret, n);
    }

    SystemArena_CheckPointer(ret, n, "SystemArena_Calloc", "CALLOC");
    return ret;
}

void SystemArena_Display(void) {
    rmonPrintf("[system_malloc] SYSTEM ARENA DISPLAY\n");
    __osDisplayArena(&gSystemArena);
}

void SystemArena_GetSizes(u32* outMaxFree, u32* outFree, u32* outAlloc) {
    ArenaImpl_GetSizes(&gSystemArena, outMaxFree, outFree, outAlloc);
}

void SystemArena_Check(void) {
    __osCheckArena(&gSystemArena);
}

void SystemArena_Init(void* start, u32 size) {
    gSystemArenaLogSeverity = LOG_SEVERITY_NOLOG;
    __osMallocInit(&gSystemArena, start, size);
}

void SystemArena_Cleanup(void) {
    gSystemArenaLogSeverity = LOG_SEVERITY_NOLOG;
    __osMallocCleanup(&gSystemArena);
}

u8 SystemArena_IsInitialized(void) {
    return __osMallocIsInitialized(&gSystemArena);
}
