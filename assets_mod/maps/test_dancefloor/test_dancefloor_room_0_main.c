#include "ultra64.h"
#include "z64.h"
#include "macros.h"
#include "test_dancefloor_scene.h"
#include "segment_symbols.h"
#include "command_macros_base.h"
#include "z64cutscene_commands.h"
#include "variables.h"

/**
 * Header Child Day (Default)
*/
#define LENGTH_TEST_DANCEFLOOR_ROOM_0_HEADER00_ACTORLIST 3
SceneCmd test_dancefloor_room_0_header00[] = {
    SCENE_CMD_ECHO_SETTINGS(0x00),
    SCENE_CMD_ROOM_BEHAVIOR(0x00, 0x00, false, false),
    SCENE_CMD_SKYBOX_DISABLES(false, false),
    SCENE_CMD_TIME_SETTINGS(0xFF, 0xFF, 10),
    SCENE_CMD_ROOM_SHAPE(&test_dancefloor_room_0_shapeHeader),
    SCENE_CMD_ACTOR_LIST(LENGTH_TEST_DANCEFLOOR_ROOM_0_HEADER00_ACTORLIST, test_dancefloor_room_0_header00_actorList),
    SCENE_CMD_END(),
};

ActorEntry test_dancefloor_room_0_header00_actorList[LENGTH_TEST_DANCEFLOOR_ROOM_0_HEADER00_ACTORLIST] = {
    // Custom Actor
    {
        /* Actor ID   */ ACTOR_LIGHTS_OUT_FANCEDLOOR,
        /* Position   */ { 319, -120, 222 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ 0
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_LIGHTS_OUT,
        /* Position   */ { -323, -120, 289 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ 0
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_LIGHTS_OUT_TARGET,
        /* Position   */ { 303, -120, -291 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ 1
    },
};

