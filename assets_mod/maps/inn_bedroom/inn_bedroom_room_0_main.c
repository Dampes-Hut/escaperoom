#include "ultra64.h"
#include "z64.h"
#include "macros.h"
#include "inn_bedroom_scene.h"
#include "segment_symbols.h"
#include "command_macros_base.h"
#include "z64cutscene_commands.h"
#include "variables.h"

/**
 * Header Child Day (Default)
*/
#define LENGTH_INN_BEDROOM_ROOM_0_HEADER00_OBJECTLIST 3
#define LENGTH_INN_BEDROOM_ROOM_0_HEADER00_ACTORLIST 2
SceneCmd inn_bedroom_room_0_header00[] = {
    SCENE_CMD_ALTERNATE_HEADER_LIST(inn_bedroom_room_0_alternateHeaders),
    SCENE_CMD_ECHO_SETTINGS(0x00),
    SCENE_CMD_ROOM_BEHAVIOR_PL(0x00, 0x00, false, false, true),
    SCENE_CMD_SKYBOX_DISABLES(false, true),
    SCENE_CMD_TIME_SETTINGS(0, 0, 0),
    SCENE_CMD_ROOM_SHAPE(&inn_bedroom_room_0_shapeHeader),
    SCENE_CMD_OBJECT_LIST(LENGTH_INN_BEDROOM_ROOM_0_HEADER00_OBJECTLIST, inn_bedroom_room_0_header00_objectList),
    SCENE_CMD_ACTOR_LIST(LENGTH_INN_BEDROOM_ROOM_0_HEADER00_ACTORLIST, inn_bedroom_room_0_header00_actorList),
    SCENE_CMD_END(),
};

SceneCmd* inn_bedroom_room_0_alternateHeaders[] = {
    NULL,
    NULL,
    NULL,
    inn_bedroom_room_0_header04,
    inn_bedroom_room_0_header05,
};

s16 inn_bedroom_room_0_header00_objectList[LENGTH_INN_BEDROOM_ROOM_0_HEADER00_OBJECTLIST] = {
    OBJECT_POH,
    OBJECT_INN_PAINTING,
    OBJECT_CHANDELIER,
};

ActorEntry inn_bedroom_room_0_header00_actorList[LENGTH_INN_BEDROOM_ROOM_0_HEADER00_ACTORLIST] = {
    // Custom Actor
    {
        /* Actor ID   */ ACTOR_CHANDELIER_LIGHT,
        /* Position   */ { -114, 58, 122 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ 0x0000
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_INN_PAINTING,
        /* Position   */ { 0, -7, 237 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(184.620), DEG_TO_BINANG(0.000) },
        /* Parameters */ 0x0000
    },
};

/**
 * Header Cutscene No. 1
*/
#define LENGTH_INN_BEDROOM_ROOM_0_HEADER04_OBJECTLIST 4
#define LENGTH_INN_BEDROOM_ROOM_0_HEADER04_ACTORLIST 3
SceneCmd inn_bedroom_room_0_header04[] = {
    SCENE_CMD_ECHO_SETTINGS(0x00),
    SCENE_CMD_ROOM_BEHAVIOR_PL(0x00, 0x00, false, false, true),
    SCENE_CMD_SKYBOX_DISABLES(false, true),
    SCENE_CMD_TIME_SETTINGS(0, 0, 0),
    SCENE_CMD_ROOM_SHAPE(&inn_bedroom_room_0_shapeHeader),
    SCENE_CMD_OBJECT_LIST(LENGTH_INN_BEDROOM_ROOM_0_HEADER04_OBJECTLIST, inn_bedroom_room_0_header04_objectList),
    SCENE_CMD_ACTOR_LIST(LENGTH_INN_BEDROOM_ROOM_0_HEADER04_ACTORLIST, inn_bedroom_room_0_header04_actorList),
    SCENE_CMD_END(),
};

s16 inn_bedroom_room_0_header04_objectList[LENGTH_INN_BEDROOM_ROOM_0_HEADER04_OBJECTLIST] = {
    OBJECT_POH,
    OBJECT_INN_PAINTING,
    OBJECT_CHANDELIER,
    OBJECT_MAG,
};

ActorEntry inn_bedroom_room_0_header04_actorList[LENGTH_INN_BEDROOM_ROOM_0_HEADER04_ACTORLIST] = {
    // Custom Actor
    {
        /* Actor ID   */ ACTOR_CHANDELIER_LIGHT,
        /* Position   */ { -114, 58, 122 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ 0x0000
    },

    // Title Screen Actor
    {
        /* Actor ID   */ ACTOR_EN_MAG,
        /* Position   */ { -300, -74, 46 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ 0x0000
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_INN_PAINTING,
        /* Position   */ { 0, -7, 237 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(184.620), DEG_TO_BINANG(0.000) },
        /* Parameters */ 0x0000
    },
};

/**
 * Header Cutscene No. 2
*/
#define LENGTH_INN_BEDROOM_ROOM_0_HEADER05_OBJECTLIST 3
#define LENGTH_INN_BEDROOM_ROOM_0_HEADER05_ACTORLIST 2
SceneCmd inn_bedroom_room_0_header05[] = {
    SCENE_CMD_ECHO_SETTINGS(0x00),
    SCENE_CMD_ROOM_BEHAVIOR_PL(0x00, 0x00, false, false, true),
    SCENE_CMD_SKYBOX_DISABLES(false, true),
    SCENE_CMD_TIME_SETTINGS(0, 0, 0),
    SCENE_CMD_ROOM_SHAPE(&inn_bedroom_room_0_shapeHeader),
    SCENE_CMD_OBJECT_LIST(LENGTH_INN_BEDROOM_ROOM_0_HEADER05_OBJECTLIST, inn_bedroom_room_0_header05_objectList),
    SCENE_CMD_ACTOR_LIST(LENGTH_INN_BEDROOM_ROOM_0_HEADER05_ACTORLIST, inn_bedroom_room_0_header05_actorList),
    SCENE_CMD_END(),
};

s16 inn_bedroom_room_0_header05_objectList[LENGTH_INN_BEDROOM_ROOM_0_HEADER05_OBJECTLIST] = {
    OBJECT_POH,
    OBJECT_INN_PAINTING,
    OBJECT_CHANDELIER,
};

ActorEntry inn_bedroom_room_0_header05_actorList[LENGTH_INN_BEDROOM_ROOM_0_HEADER05_ACTORLIST] = {
    // Custom Actor
    {
        /* Actor ID   */ ACTOR_CHANDELIER_LIGHT,
        /* Position   */ { -114, 58, 122 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ 0x0000
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_INN_PAINTING,
        /* Position   */ { 0, -7, 237 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(184.620), DEG_TO_BINANG(0.000) },
        /* Parameters */ 0x0000
    },
};

