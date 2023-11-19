#include "global.h"
#include "terminal.h"

#define KALEIDO_OVERLAY(name, nameString)                                                     \
    {                                                                                         \
        NULL, (uintptr_t)_ovl_##name##SegmentRomStart, (uintptr_t)_ovl_##name##SegmentRomEnd, \
            _ovl_##name##SegmentStart, _ovl_##name##SegmentEnd, 0, nameString,                \
    }

KaleidoMgrOverlay gKaleidoMgrOverlayTable[] = {
    KALEIDO_OVERLAY(kaleido_scope, "kaleido_scope"),
    KALEIDO_OVERLAY(player_actor, "player_actor"),
};

void* sKaleidoAreaPtr = NULL;
KaleidoMgrOverlay* gKaleidoMgrCurOvl = NULL;
u8 gBossMarkState = 0;

FaultAddrConvClient sKaleidoAddrConvClient;

uintptr_t KaleidoManager_FaultAddrConv(uintptr_t addr, void* param) {
    u8* ptr = (u8*)addr;
    KaleidoMgrOverlay* overlayEntry = &gKaleidoMgrOverlayTable[0];
    u8* ramStart;
    u8* ramEnd;
    size_t size;
    size_t offset;
    s32 i;

    for (i = 0; i < ARRAY_COUNT(gKaleidoMgrOverlayTable); i++, overlayEntry++) {
        size = (u8*)overlayEntry->vramEnd - (u8*)overlayEntry->vramStart;
        ramStart = overlayEntry->loadedRamAddr;
        ramEnd = ramStart + size;
        offset = (u8*)overlayEntry->vramStart - ramStart;
        if (ramStart != NULL) {
            if (ptr >= ramStart && ptr < ramEnd) {
                return (uintptr_t)ptr + offset;
            }
        }
    }
    return 0;
}

void KaleidoManager_LoadOvl(KaleidoMgrOverlay* ovl) {
    LogUtils_CheckNullPointer("KaleidoArea_allocp", sKaleidoAreaPtr, "../z_kaleido_manager.c", 99);

    ovl->loadedRamAddr = sKaleidoAreaPtr;
    Overlay_Load(ovl->vromStart, ovl->vromEnd, ovl->vramStart, ovl->vramEnd, ovl->loadedRamAddr);

    osSyncPrintf(VT_FGCOL(GREEN));
    osSyncPrintf("OVL(k):Seg:%08x-%08x Ram:%08x-%08x Off:%08x %s\n", ovl->vramStart, ovl->vramEnd, ovl->loadedRamAddr,
                 (uintptr_t)ovl->loadedRamAddr + (uintptr_t)ovl->vramEnd - (uintptr_t)ovl->vramStart,
                 (uintptr_t)ovl->vramStart - (uintptr_t)ovl->loadedRamAddr, ovl->name);
    osSyncPrintf(VT_RST);

    ovl->offset = (uintptr_t)ovl->loadedRamAddr - (uintptr_t)ovl->vramStart;
    gKaleidoMgrCurOvl = ovl;
}

void KaleidoManager_ClearOvl(KaleidoMgrOverlay* ovl) {
    if (ovl->loadedRamAddr != NULL) {
        ovl->offset = 0;
        bzero(ovl->loadedRamAddr, (uintptr_t)ovl->vramEnd - (uintptr_t)ovl->vramStart);
        ovl->loadedRamAddr = NULL;
        gKaleidoMgrCurOvl = NULL;
    }
}

void KaleidoManager_Init(PlayState* play) {
    s32 largestSize = 0;
    s32 size;
    u32 i;

    for (i = 0; i < ARRAY_COUNT(gKaleidoMgrOverlayTable); i++) {
        size = (uintptr_t)gKaleidoMgrOverlayTable[i].vramEnd - (uintptr_t)gKaleidoMgrOverlayTable[i].vramStart;
        if (size > largestSize) {
            largestSize = size;
        }
    }

    osSyncPrintf(VT_FGCOL(GREEN));
    osSyncPrintf("KaleidoArea の最大サイズは %d バイトを確保します\n", largestSize);
    osSyncPrintf(VT_RST);

    sKaleidoAreaPtr = GameState_Alloc(&play->state, largestSize, "../z_kaleido_manager.c", 150);
    LogUtils_CheckNullPointer("KaleidoArea_allocp", sKaleidoAreaPtr, "../z_kaleido_manager.c", 151);

    osSyncPrintf(VT_FGCOL(GREEN));
    osSyncPrintf("KaleidoArea %08x - %08x\n", sKaleidoAreaPtr, (uintptr_t)sKaleidoAreaPtr + largestSize);
    osSyncPrintf(VT_RST);

    gKaleidoMgrCurOvl = NULL;

    Fault_AddAddrConvClient(&sKaleidoAddrConvClient, KaleidoManager_FaultAddrConv, NULL);
}

void KaleidoManager_Destroy(void) {
    if (gKaleidoMgrCurOvl != NULL) {
        KaleidoManager_ClearOvl(gKaleidoMgrCurOvl);
        gKaleidoMgrCurOvl = NULL;
    }

    sKaleidoAreaPtr = NULL;

    Fault_RemoveAddrConvClient(&sKaleidoAddrConvClient);
}

// NOTE: this function looks messed up and probably doesn't work how it was intended to
void* KaleidoManager_GetRamAddr(void* vram) {
    KaleidoMgrOverlay* iter = gKaleidoMgrCurOvl;
    KaleidoMgrOverlay* ovl = iter;
    u32 i;

    if (ovl == NULL) {
        iter = &gKaleidoMgrOverlayTable[0];
        for (i = 0; i < ARRAY_COUNT(gKaleidoMgrOverlayTable); i++) {
            if (((uintptr_t)vram >= (uintptr_t)iter->vramStart) && ((uintptr_t)iter->vramEnd >= (uintptr_t)vram)) {
                KaleidoManager_LoadOvl(iter);
                ovl = iter;
                goto KaleidoManager_GetRamAddr_end;
            }
            //! @bug Probably missing iter++ here
        }

        osSyncPrintf("異常\n"); // "Abnormal"
        return NULL;
    }

KaleidoManager_GetRamAddr_end:
    if ((ovl == NULL) || ((uintptr_t)vram < (uintptr_t)ovl->vramStart) ||
        ((uintptr_t)vram >= (uintptr_t)ovl->vramEnd)) {
        return NULL;
    }

    return (void*)((uintptr_t)vram + ovl->offset);
}
