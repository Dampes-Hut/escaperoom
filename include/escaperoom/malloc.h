#ifndef MALLOC_H
#define MALLOC_H

#include "ultra64.h"

struct ArenaNode;

typedef struct Arena {
    /* 0x00 */ struct ArenaNode* head;
    /* 0x04 */ struct ArenaNode* tail;
    /* 0x08 */ void* start;
    /* 0x0C */ OSMesgQueue lockQueue;
    /* 0x24 */ u8 unk_20;
    /* 0x25 */ u8 isInit;
    /* 0x26 */ u8 flag;
} Arena; // size = 0x28

typedef struct ArenaNode {
    /* 0x00 */ s16 magic;
    /* 0x02 */ s16 isFree;
    /* 0x04 */ u32 size;
    /* 0x08 */ struct ArenaNode* next;
    /* 0x0C */ struct ArenaNode* prev;
#ifndef NDEBUG
    const char* filename;
    int line;
    OSId threadId;
    Arena* arena;
    OSTime time;
    u8 padding[8]; // up to 0x10 byte boundary
#endif
} ArenaNode;

extern u32 __osMalloc_FreeBlockTest_Enable;
extern Arena gSystemArena;

#ifdef NDEBUG
#define FILE_LINE_ARGS
#define FILE_LINE_DECL
#else
#define FILE_LINE_ARGS , file, line
#define FILE_LINE_DECL , const char* file, int line
#endif

void __osMallocInit(Arena* arena, void* start, u32 size);
void __osMallocAddBlock(Arena* arena, void* start, s32 size);
void ArenaImpl_RemoveAllBlocks(Arena* arena);
void __osMallocCleanup(Arena* arena);
void __osMalloc_FreeBlockTest(Arena* arena, ArenaNode* node);
void* __osMalloc(Arena* arena, u32 size FILE_LINE_DECL);
void* __osMallocR(Arena* arena, u32 size FILE_LINE_DECL);
void __osFree(Arena* arena, void* ptr FILE_LINE_DECL);
void* __osRealloc(Arena* arena, void* ptr, u32 newSize FILE_LINE_DECL);
void ArenaImpl_GetSizes(Arena* arena, u32* outMaxFree, u32* outFree, u32* outAlloc);
void __osDisplayArena(Arena* arena);
void ArenaImpl_FaultClient(Arena* arena);
u32 __osCheckArena(Arena* arena);
u8 func_800FF334(Arena* arena);

#define __osMallocIsInitialized(arena) ((arena)->isInit)



void* SystemArena_Malloc(u32 size FILE_LINE_DECL);
void* SystemArena_MallocR(u32 size FILE_LINE_DECL);
void* SystemArena_Realloc(void* ptr, u32 newSize FILE_LINE_DECL);
void SystemArena_Free(void* ptr FILE_LINE_DECL);
void* SystemArena_Calloc(u32 num, u32 size FILE_LINE_DECL);
void SystemArena_Display(void);
void SystemArena_GetSizes(u32* outMaxFree, u32* outFree, u32* outAlloc);
void SystemArena_Check(void);
void SystemArena_Init(void* start, u32 size);
void SystemArena_Cleanup(void);
u8 SystemArena_IsInitialized(void);

void* ZeldaArena_Malloc(u32 size FILE_LINE_DECL);
void* ZeldaArena_MallocR(u32 size FILE_LINE_DECL);
void* ZeldaArena_Realloc(void* ptr, u32 newSize FILE_LINE_DECL);
void ZeldaArena_Free(void* ptr FILE_LINE_DECL);
void* ZeldaArena_Calloc(u32 num, u32 size FILE_LINE_DECL);
void ZeldaArena_Display(void);
void ZeldaArena_GetSizes(u32* outMaxFree, u32* outFree, u32* outAlloc);
void ZeldaArena_Check(void);
void ZeldaArena_Init(void* start, u32 size);
void ZeldaArena_Cleanup(void);
u8 ZeldaArena_IsInitialized(void);

void* DebugArena_Malloc(u32 size FILE_LINE_DECL);
void* DebugArena_MallocR(u32 size FILE_LINE_DECL);
void* DebugArena_Realloc(void* ptr, u32 newSize FILE_LINE_DECL);
void DebugArena_Free(void* ptr FILE_LINE_DECL);
void* DebugArena_Calloc(u32 num, u32 size FILE_LINE_DECL);
void DebugArena_Display(void);
void DebugArena_GetSizes(u32* outMaxFree, u32* outFree, u32* outAlloc);
void DebugArena_Check(void);
void DebugArena_Init(void* start, u32 size);
void DebugArena_Cleanup(void);
u8 DebugArena_IsInitialized(void);

// Cleanup vanilla calls

#ifdef NDEBUG
#define __osMallocDebug(arena, size, ...)           __osMalloc(arena, size)
#define __osMallocRDebug(arena, size, ...)          __osMallocR(arena, size)
#define __osFreeDebug(arena, ptr, ...)              __osFree(arena, size)
#define __osReallocDebug(arena, ptr, newSize, ...)  __osRealloc(arena, size)

#define SystemArena_MallocDebug(size, ...)          SystemArena_Malloc(size)
#define SystemArena_MallocRDebug(size, ...)         SystemArena_MallocR(size)
#define SystemArena_ReallocDebug(ptr, newSize, ...) SystemArena_Realloc(ptr, newSize)
#define SystemArena_FreeDebug(ptr, ...)             SystemArena_Free(ptr)
#define SystemArena_CallocDebug(num, size, ...)     SystemArena_Calloc(num, size)

#define ZeldaArena_MallocDebug(size, ...)           ZeldaArena_Malloc(size)
#define ZeldaArena_MallocRDebug(size, ...)          ZeldaArena_MallocR(size)
#define ZeldaArena_ReallocDebug(ptr, newSize, ...)  ZeldaArena_Realloc(ptr, newSize)
#define ZeldaArena_FreeDebug(ptr, ...)              ZeldaArena_Free(ptr)
#define ZeldaArena_CallocDebug(num, size, ...)      ZeldaArena_Calloc(num, size)

#define DebugArena_MallocDebug(size, ...)           DebugArena_Malloc(size)
#define DebugArena_MallocRDebug(size, ...)          DebugArena_MallocR(size)
#define DebugArena_ReallocDebug(ptr, newSize, ...)  DebugArena_Realloc(ptr, newSize)
#define DebugArena_FreeDebug(ptr, ...)              DebugArena_Free(ptr)
#define DebugArena_CallocDebug(num, size, ...)      DebugArena_Calloc(num, size)
#else
#define __osMallocDebug(arena, size, ...)           __osMalloc(arena, size, __FILE__, __LINE__)
#define __osMallocRDebug(arena, size, ...)          __osMallocR(arena, size, __FILE__, __LINE__)
#define __osFreeDebug(arena, ptr, ...)              __osFree(arena, size, __FILE__, __LINE__)
#define __osReallocDebug(arena, ptr, newSize, ...)  __osRealloc(arena, size, __FILE__, __LINE__)

#define SystemArena_MallocDebug(size, ...)          SystemArena_Malloc(size, __FILE__, __LINE__)
#define SystemArena_MallocRDebug(size, ...)         SystemArena_MallocR(size, __FILE__, __LINE__)
#define SystemArena_ReallocDebug(ptr, newSize, ...) SystemArena_Realloc(ptr, newSize, __FILE__, __LINE__)
#define SystemArena_FreeDebug(ptr, ...)             SystemArena_Free(ptr, __FILE__, __LINE__)
#define SystemArena_CallocDebug(num, size, ...)     SystemArena_Calloc(num, size, __FILE__, __LINE__)

#define ZeldaArena_MallocDebug(size, ...)           ZeldaArena_Malloc(size, __FILE__, __LINE__)
#define ZeldaArena_MallocRDebug(size, ...)          ZeldaArena_MallocR(size, __FILE__, __LINE__)
#define ZeldaArena_ReallocDebug(ptr, newSize, ...)  ZeldaArena_Realloc(ptr, newSize, __FILE__, __LINE__)
#define ZeldaArena_FreeDebug(ptr, ...)              ZeldaArena_Free(ptr, __FILE__, __LINE__)
#define ZeldaArena_CallocDebug(num, size, ...)      ZeldaArena_Calloc(num, size, __FILE__, __LINE__)

#define DebugArena_MallocDebug(size, ...)           DebugArena_Malloc(size, __FILE__, __LINE__)
#define DebugArena_MallocRDebug(size, ...)          DebugArena_MallocR(size, __FILE__, __LINE__)
#define DebugArena_ReallocDebug(ptr, newSize, ...)  DebugArena_Realloc(ptr, newSize, __FILE__, __LINE__)
#define DebugArena_FreeDebug(ptr, ...)              DebugArena_Free(ptr, __FILE__, __LINE__)
#define DebugArena_CallocDebug(num, size, ...)      DebugArena_Calloc(num, size, __FILE__, __LINE__)
#endif

#endif
