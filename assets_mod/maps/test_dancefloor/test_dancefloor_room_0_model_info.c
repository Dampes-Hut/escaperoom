#include "ultra64.h"
#include "z64.h"
#include "macros.h"
#include "test_dancefloor_scene.h"
#include "segment_symbols.h"
#include "command_macros_base.h"
#include "z64cutscene_commands.h"
#include "variables.h"

RoomShapeNormal test_dancefloor_room_0_shapeHeader = {
    ROOM_SHAPE_TYPE_NORMAL,
    ARRAY_COUNT(test_dancefloor_room_0_shapeDListEntry),
    test_dancefloor_room_0_shapeDListEntry,
    test_dancefloor_room_0_shapeDListEntry + ARRAY_COUNT(test_dancefloor_room_0_shapeDListEntry)
};

RoomShapeDListsEntry test_dancefloor_room_0_shapeDListEntry[1] = {
    { test_dancefloor_room_0_entry_0_opaque, NULL },
};

