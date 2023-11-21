#include "ultra64.h"
#include "z64.h"
#include "macros.h"
#include "bed_scene.h"
#include "segment_symbols.h"
#include "command_macros_base.h"
#include "z64cutscene_commands.h"
#include "variables.h"

RoomShapeNormal bed_room_0_shapeHeader = {
    ROOM_SHAPE_TYPE_NORMAL,
    ARRAY_COUNT(bed_room_0_shapeDListEntry),
    bed_room_0_shapeDListEntry,
    bed_room_0_shapeDListEntry + ARRAY_COUNT(bed_room_0_shapeDListEntry)
};

RoomShapeDListsEntry bed_room_0_shapeDListEntry[1] = {
    { bed_room_0_entry_0_opaque, NULL },
};

