#ifndef COMMON_H
#define COMMON_H

// ALLOCATORS

#ifdef NDEBUG
// TODO NDEBUG alternatives
#define GameAlloc_MallocDebug(this, size, ...)      GameAlloc_Malloc(this, size)
#else
#define GameState_Alloc(gameState, size, ...)       GameState_Alloc(gameState, size, __FILE__, __LINE__)
#define GameAlloc_MallocDebug(this, size, ...)      GameAlloc_MallocDebug(this, size, __FILE__, __LINE__)
#endif

// DEBUG / FAULT

#define ASSERT_SOFT(cond) ((cond) ? (void)0 : __assert_soft(#cond, __FILE__, __LINE__))

#ifdef NDEBUG
#define FrameAdvance_IsEnabled(play)    (0)
#endif

// Remove original debug prints
#define osSyncPrintf(...) (void)0

// This one we keep even under NDEBUG
#define Fault_AddHungupAndCrash(...) Fault_AddHungupAndCrash(__FILE__, __LINE__)

// Logging utilities, remove if NDEBUG
#ifndef NDEBUG

#define LogUtils_LogThreadId(...)                       LogUtils_LogThreadId(__FILE__, __LINE__)
#define LogUtils_HungupThread(...)                      LogUtils_HungupThread(__FILE__, __LINE__)
#define LogUtils_LogPointer(value, max, ptr, ...)       LogUtils_LogPointer(value, max, ptr, #ptr, __FILE__, __LINE__)
#define LogUtils_CheckBoundary(name, value, msbit, ...) LogUtils_CheckBoundary(#value, value, msbit, __FILE__, __LINE__)
#define LogUtils_CheckNullPointer(exp, ptr, ...)        LogUtils_CheckNullPointer(#ptr, ptr, __FILE__, __LINE__)
#define LogUtils_CheckValidPointer(exp, ptr, ...)       LogUtils_CheckValidPointer(#ptr, ptr, __FILE__, __LINE__)

#define LOG(exp, value, format, ...)                \
    do {                                            \
        LogUtils_LogThreadId();                     \
        osSyncPrintf(exp " = " format "\n", value); \
    } while (0)

#define LOG_STRING(string, ...)      LOG(#string, string, "%s")
#define LOG_ADDRESS(exp, value, ...) LOG(exp, value, "%08x")
#define LOG_TIME(exp, value, ...)    LOG(exp, value, "%lld")
#define LOG_NUM(exp, value, ...)     LOG(exp, value, "%d")
#define LOG_HEX(exp, value, ...)     LOG(exp, value, "%x")
#define LOG_HEX32(exp, value, ...)   LOG(exp, value, "%08x")
#define LOG_FLOAT(exp, value, ...)   LOG(exp, value, "%f")

#else

#define LogUtils_LogThreadId(...)                       (void)0
#define LogUtils_LogHexDump(...)                        (void)0
#define LogUtils_HungupThread(...)                      (void)0
#define LogUtils_LogPointer(value, max, ptr, ...)       (void)0
#define LogUtils_CheckBoundary(name, value, msbit, ...) (void)0
#define LogUtils_CheckNullPointer(exp, ptr, ...)        (void)0
#define LogUtils_CheckValidPointer(exp, ptr, ...)       (void)0

#define LOG(exp, value, format, ...) (void)0

#define LOG_STRING(string, ...)      (void)0
#define LOG_ADDRESS(exp, value, ...) (void)0
#define LOG_TIME(exp, value, ...)    (void)0
#define LOG_NUM(exp, value, ...)     (void)0
#define LOG_HEX(exp, value, ...)     (void)0
#define LOG_HEX32(exp, value, ...)   (void)0
#define LOG_FLOAT(exp, value, ...)   (void)0

#define rmonPrintf(...) (void)0

#endif

// DMA
// TODO NDEBUG alternatives

#define DmaMgr_RequestSyncDebug(ram, vrom, size, ...) \
    DmaMgr_RequestSyncDebug(ram, vrom, size, __FILE__, __LINE__)

#define DmaMgr_RequestAsync(req, ram, vrom, size, unk5, queue, msg, ...) \
    DmaMgr_RequestAsync(req, ram, vrom, size, unk5, queue, msg, __FILE__, __LINE__)

// GFX

#define Graph_OpenDisps(dispRefs, gfxCtx, ...)      Graph_OpenDisps(dispRefs, gfxCtx, __FILE__, __LINE__)
#define Graph_CloseDisps(dispRefs, gfxCtx, ...)     Graph_CloseDisps(dispRefs, gfxCtx, __FILE__, __LINE__)

#define WORK_DISP       __gfxCtx->work.p
#define POLY_OPA_DISP   __gfxCtx->polyOpa.p
#define POLY_XLU_DISP   __gfxCtx->polyXlu.p
#define OVERLAY_DISP    __gfxCtx->overlay.p

#ifdef NDEBUG
#define OPEN_DISPS(gfxCtx, ...)             \
    {                                       \
        GraphicsContext* __gfxCtx;          \
        __gfxCtx = gfxCtx;                  \
        (void)__gfxCtx;                     \
        (void)0

#define CLOSE_DISPS(gfxCtx, ...)            \
    }                                       \
    (void)0
#else
#define OPEN_DISPS(gfxCtx, ...)             \
    {                                       \
        GraphicsContext* __gfxCtx;          \
        Gfx* dispRefs[4];                   \
        __gfxCtx = gfxCtx;                  \
        (void)__gfxCtx;                     \
        Graph_OpenDisps(dispRefs, gfxCtx);  \
        (void)0

#define CLOSE_DISPS(gfxCtx, ...)            \
        Graph_CloseDisps(dispRefs, gfxCtx); \
    }                                       \
    (void)0
#endif

#define Matrix_ToMtx(dst, ...) Matrix_ToMtx(dst, __FILE__, __LINE__)
#define Matrix_NewMtx(gfxCtx, ...) Matrix_NewMtx(gfxCtx, __FILE__, __LINE__)
#define Matrix_CheckFloats(mf, ...) Matrix_CheckFloats(mf, __FILE__, __LINE__)

#ifdef NDEBUG
#define Graph_Alloc(gfxCtx, size) ((void*)((gfxCtx)->polyOpa.d = (Gfx*)((u8*)(gfxCtx)->polyOpa.d - ALIGN16(size))))
#else
#define Graph_Alloc(gfxCtx, size) Graph_Alloc(gfxCtx, size)
#endif

#ifndef NDEBUG
#define LightContext_InsertLight(play, lightCtx, info) LightContext_InsertLight(play, lightCtx, info, __FILE__, __LINE__)
#define LightContext_InsertLightList(play, lightCtx, lightList, numLights) LightContext_InsertLightList(play, lightCtx, lightList, numLights, __FILE__, __LINE__)
#endif

#define gFrameBuffer0   ((void*)0x80600000)
#define gFrameBuffer1   ((void*)0x80700000)

#endif
