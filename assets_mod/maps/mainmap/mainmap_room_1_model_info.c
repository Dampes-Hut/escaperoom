#include "ultra64.h"
#include "z64.h"
#include "macros.h"
#include "mainmap_scene.h"
#include "segment_symbols.h"
#include "command_macros_base.h"
#include "z64cutscene_commands.h"
#include "variables.h"

RoomShapeNormal mainmap_room_1_shapeHeader = {
    ROOM_SHAPE_TYPE_NORMAL,
    ARRAY_COUNT(mainmap_room_1_shapeDListEntry),
    mainmap_room_1_shapeDListEntry,
    mainmap_room_1_shapeDListEntry + ARRAY_COUNT(mainmap_room_1_shapeDListEntry)
};

RoomShapeDListsEntry mainmap_room_1_shapeDListEntry[1] = {
    { mainmap_room_1_entry_0_opaque, NULL },
};

