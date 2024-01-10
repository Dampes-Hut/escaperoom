#include "ultra64.h"
#include "z64.h"
#include "macros.h"
#include "inn_bedroom_scene.h"
#include "segment_symbols.h"
#include "command_macros_base.h"
#include "z64cutscene_commands.h"
#include "variables.h"

RoomShapeNormal inn_bedroom_room_0_shapeHeader = {
    ROOM_SHAPE_TYPE_NORMAL,
    ARRAY_COUNT(inn_bedroom_room_0_shapeDListEntry),
    inn_bedroom_room_0_shapeDListEntry,
    inn_bedroom_room_0_shapeDListEntry + ARRAY_COUNT(inn_bedroom_room_0_shapeDListEntry)
};

RoomShapeDListsEntry inn_bedroom_room_0_shapeDListEntry[1] = {
    { inn_bedroom_room_0_entry_0_opaque, inn_bedroom_room_0_entry_0_transparent },
};

