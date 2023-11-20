#include "global.h"

// Linker symbol declarations (used in the table below)
#define DEFINE_ACTOR(name, _1, _2, _3) DECLARE_OVERLAY_SEGMENT(name)
#define DEFINE_ACTOR_INTERNAL(_0, _1, _2, _3)
#define DEFINE_ACTOR_UNSET(_0)

#include "tables/actor_table.h"

#undef DEFINE_ACTOR
#undef DEFINE_ACTOR_INTERNAL
#undef DEFINE_ACTOR_UNSET

// Init Vars declarations (also used in the table below)
#define DEFINE_ACTOR(name, _1, _2, _3) extern ActorInit name##_InitVars;
#define DEFINE_ACTOR_INTERNAL(name, _1, _2, _3) extern ActorInit name##_InitVars;
#define DEFINE_ACTOR_UNSET(_0)

#include "tables/actor_table.h"

#undef DEFINE_ACTOR
#undef DEFINE_ACTOR_INTERNAL
#undef DEFINE_ACTOR_UNSET

// Actor Overlay Table definition
#define DEFINE_ACTOR(name, _1, allocType, nameString) \
    { (uintptr_t)_ovl_##name##SegmentRomStart,        \
      (uintptr_t)_ovl_##name##SegmentRomEnd,          \
      _ovl_##name##SegmentStart,                      \
      _ovl_##name##SegmentEnd,                        \
      NULL,                                           \
      &name##_InitVars,                               \
      nameString,                                     \
      allocType,                                      \
      0 },

#define DEFINE_ACTOR_INTERNAL(name, _1, allocType, nameString) \
    { 0, 0, NULL, NULL, NULL, &name##_InitVars, nameString, allocType, 0 },

#define DEFINE_ACTOR_UNSET(_0) { 0 },

ActorOverlay gActorOverlayTable[] = {
#include "tables/actor_table.h"
};

#undef DEFINE_ACTOR
#undef DEFINE_ACTOR_INTERNAL
#undef DEFINE_ACTOR_UNSET

s32 gMaxActorId = 0;

static FaultClient sFaultClient;
static FaultAddrConvClient sFaultAddrConvClient;

void ActorOverlayTable_LogPrint(void) {
    ActorOverlay* overlayEntry;
    u32 i;

    osSyncPrintf("actor_dlftbls %u\n", gMaxActorId);
    osSyncPrintf("RomStart RomEnd   SegStart SegEnd   allocp   profile  segname\n");

    for (i = 0, overlayEntry = &gActorOverlayTable[0]; i < (u32)gMaxActorId; i++, overlayEntry++) {
        osSyncPrintf("%08x %08x %08x %08x %08x %08x %s\n", overlayEntry->vromStart, overlayEntry->vromEnd,
                     overlayEntry->vramStart, overlayEntry->vramEnd, overlayEntry->loadedRamAddr,
                     &overlayEntry->initInfo->id, overlayEntry->name != NULL ? overlayEntry->name : "?");
    }
}

s32 ActorOverlayTable_FaultPrint(void* arg0, void* arg1) {
    ActorOverlay* overlayEntry;
    u32 overlaySize;
    s32 i;

    FaultDrawer_SetCharPad(-2, 0);

    FaultDrawer_Printf("actor_dlftbls %u\n", gMaxActorId);
    FaultDrawer_Printf("No. RamStart- RamEnd cn  Name\n");

    for (i = 0, overlayEntry = &gActorOverlayTable[0]; i < gMaxActorId; i++, overlayEntry++) {
        overlaySize = (uintptr_t)overlayEntry->vramEnd - (uintptr_t)overlayEntry->vramStart;
        if (overlayEntry->loadedRamAddr != NULL) {
            FaultDrawer_Printf("%3d %08x-%08x %3d %s\n", i, overlayEntry->loadedRamAddr,
                               (uintptr_t)overlayEntry->loadedRamAddr + overlaySize, overlayEntry->numLoaded,
                               overlayEntry->name != NULL ? overlayEntry->name : "");
        }
    }
    return false;
}

uintptr_t ActorOverlayTable_FaultAddrConv(uintptr_t addr, void* param) {
    u8* ptr = (u8*)addr;
    ActorOverlay* overlayEntry = &gActorOverlayTable[0];
    u8* ramStart;
    u8* ramEnd;
    size_t size;
    size_t offset;
    s32 i;

    for (i = 0; i < gMaxActorId; i++, overlayEntry++) {
        size = (u8*)overlayEntry->vramEnd - (u8*)overlayEntry->vramStart;
        ramStart = overlayEntry->loadedRamAddr;
        ramEnd = ramStart + size;
        offset = (u8*)overlayEntry->vramStart - ramStart;
        if (ramStart != NULL && ptr >= ramStart && ptr < ramEnd) {
            return (uintptr_t)ptr + offset;
        }
    }
    return 0;
}

void ActorOverlayTable_Init(void) {
    gMaxActorId = ACTOR_ID_MAX;
    Fault_AddClient(&sFaultClient, ActorOverlayTable_FaultPrint, NULL, NULL);
    Fault_AddAddrConvClient(&sFaultAddrConvClient, ActorOverlayTable_FaultAddrConv, NULL);
}

void ActorOverlayTable_Cleanup(void) {
    Fault_RemoveClient(&sFaultClient);
    Fault_RemoveAddrConvClient(&sFaultAddrConvClient);
    gMaxActorId = 0;
}
