#include "ultra64.h"
#include "z64.h"
#include "macros.h"
#include "test_darklink_puzzle_scene.h"
#include "segment_symbols.h"
#include "command_macros_base.h"
#include "z64cutscene_commands.h"
#include "variables.h"

/**
 * Header Child Day (Default)
*/
#define LENGTH_TEST_DARKLINK_PUZZLE_ROOM_0_HEADER00_OBJECTLIST 1
#define LENGTH_TEST_DARKLINK_PUZZLE_ROOM_0_HEADER00_ACTORLIST 1
SceneCmd test_darklink_puzzle_room_0_header00[] = {
    SCENE_CMD_ECHO_SETTINGS(0x00),
    SCENE_CMD_ROOM_BEHAVIOR(0x00, 0x00, false, false),
    SCENE_CMD_SKYBOX_DISABLES(false, false),
    SCENE_CMD_TIME_SETTINGS(0xFF, 0xFF, 10),
    SCENE_CMD_ROOM_SHAPE(&test_darklink_puzzle_room_0_shapeHeader),
    SCENE_CMD_OBJECT_LIST(LENGTH_TEST_DARKLINK_PUZZLE_ROOM_0_HEADER00_OBJECTLIST, test_darklink_puzzle_room_0_header00_objectList),
    SCENE_CMD_ACTOR_LIST(LENGTH_TEST_DARKLINK_PUZZLE_ROOM_0_HEADER00_ACTORLIST, test_darklink_puzzle_room_0_header00_actorList),
    SCENE_CMD_END(),
};

s16 test_darklink_puzzle_room_0_header00_objectList[LENGTH_TEST_DARKLINK_PUZZLE_ROOM_0_HEADER00_OBJECTLIST] = {
    OBJECT_TORCH2,
};

ActorEntry test_darklink_puzzle_room_0_header00_actorList[LENGTH_TEST_DARKLINK_PUZZLE_ROOM_0_HEADER00_ACTORLIST] = {
    // Custom Actor
    {
        /* Actor ID   */ ACTOR_MUH_DARK_LINK,
        /* Position   */ { 8, -120, 124 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ 0x0000
    },
};

