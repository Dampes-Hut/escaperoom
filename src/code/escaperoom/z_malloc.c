#include "global.h"

#define LOG_SEVERITY_NOLOG 0
#define LOG_SEVERITY_ERROR 2
#define LOG_SEVERITY_VERBOSE 3

s32 gZeldaArenaLogSeverity = LOG_SEVERITY_ERROR;
Arena sZeldaArena;

static void ZeldaArena_CheckPointer(void* ptr, u32 size, const char* name, const char* action) {
    if (ptr == NULL) {
        if (gZeldaArenaLogSeverity >= LOG_SEVERITY_ERROR) {
            rmonPrintf("[z_malloc] %s: %u bytes %s failed to allocate\n", name, size, action);
            __osDisplayArena(&sZeldaArena);
        }
    } else if (gZeldaArenaLogSeverity >= LOG_SEVERITY_VERBOSE) {
        rmonPrintf("[z_malloc] %s: %u bytes %s allocated\n", name, size, action);
    }
}

void* ZeldaArena_Malloc(u32 size FILE_LINE_DECL) {
    void* ptr = __osMalloc(&sZeldaArena, size FILE_LINE_ARGS);

    ZeldaArena_CheckPointer(ptr, size, "ZeldaArena_Malloc", "ALLOC");
    return ptr;
}

void* ZeldaArena_MallocR(u32 size FILE_LINE_DECL) {
    void* ptr = __osMallocR(&sZeldaArena, size FILE_LINE_ARGS);

    ZeldaArena_CheckPointer(ptr, size, "ZeldaArena_MallocR", "ALLOC");
    return ptr;
}

void* ZeldaArena_Realloc(void* ptr, u32 newSize FILE_LINE_DECL) {
    ptr = __osRealloc(&sZeldaArena, ptr, newSize FILE_LINE_ARGS);
    ZeldaArena_CheckPointer(ptr, newSize, "ZeldaArena_Realloc", "REALLOC");
    return ptr;
}

void ZeldaArena_Free(void* ptr FILE_LINE_DECL) {
    __osFree(&sZeldaArena, ptr FILE_LINE_ARGS);
}

void* ZeldaArena_Calloc(u32 num, u32 size FILE_LINE_DECL) {
    void* ret;
    u32 n = num * size;

    ret = __osMalloc(&sZeldaArena, n FILE_LINE_ARGS);
    if (ret != NULL) {
        bzero(ret, n);
    }

    ZeldaArena_CheckPointer(ret, n, "ZeldaArena_Calloc", "CALLOC");
    return ret;
}

void ZeldaArena_Display(void) {
    rmonPrintf("[z_malloc] ZELDA ARENA DISPLAY\n");
    __osDisplayArena(&sZeldaArena);
}

void ZeldaArena_GetSizes(u32* outMaxFree, u32* outFree, u32* outAlloc) {
    ArenaImpl_GetSizes(&sZeldaArena, outMaxFree, outFree, outAlloc);
}

void ZeldaArena_Check(void) {
    __osCheckArena(&sZeldaArena);
}

void ZeldaArena_Init(void* start, u32 size) {
    gZeldaArenaLogSeverity = LOG_SEVERITY_NOLOG;
    __osMallocInit(&sZeldaArena, start, size);
}

void ZeldaArena_Cleanup(void) {
    gZeldaArenaLogSeverity = LOG_SEVERITY_NOLOG;
    __osMallocCleanup(&sZeldaArena);
}

u8 ZeldaArena_IsInitialized(void) {
    return __osMallocIsInitialized(&sZeldaArena);
}
