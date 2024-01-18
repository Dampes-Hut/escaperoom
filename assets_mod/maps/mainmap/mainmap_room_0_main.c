#include "ultra64.h"
#include "z64.h"
#include "macros.h"
#include "mainmap_scene.h"
#include "segment_symbols.h"
#include "command_macros_base.h"
#include "z64cutscene_commands.h"
#include "variables.h"

/**
 * Header Child Day (Default)
*/
#define LENGTH_MAINMAP_ROOM_0_HEADER00_LIGHTINFOLIST 2
#define LENGTH_MAINMAP_ROOM_0_HEADER00_OBJECTLIST 1
#define LENGTH_MAINMAP_ROOM_0_HEADER00_ACTORLIST 12
SceneCmd mainmap_room_0_header00[] = {
    SCENE_CMD_ECHO_SETTINGS(0x00),
    SCENE_CMD_ROOM_BEHAVIOR_PL(0x00, 0x00, false, false, true),
    SCENE_CMD_SKYBOX_DISABLES(false, true),
    SCENE_CMD_TIME_SETTINGS(0, 0, 0),
    SCENE_CMD_ROOM_SHAPE(&mainmap_room_0_shapeHeader),
    SCENE_CMD_LIGHT_LIST(LENGTH_MAINMAP_ROOM_0_HEADER00_LIGHTINFOLIST, mainmap_room_0_header00_lightInfoList),
    SCENE_CMD_OBJECT_LIST(LENGTH_MAINMAP_ROOM_0_HEADER00_OBJECTLIST, mainmap_room_0_header00_objectList),
    SCENE_CMD_ACTOR_LIST(LENGTH_MAINMAP_ROOM_0_HEADER00_ACTORLIST, mainmap_room_0_header00_actorList),
    SCENE_CMD_END(),
};

LightInfo mainmap_room_0_header00_lightInfoList[LENGTH_MAINMAP_ROOM_0_HEADER00_LIGHTINFOLIST] = {
    {
        LIGHT_POINT_NOGLOW,
        .params.point = {
            607, 38, -185,
            { 105, 29, 0 },
            0,
            1317
        }
    },
    {
        LIGHT_POINT_NOGLOW,
        .params.point = {
            440, 38, -19,
            { 255, 208, 151 },
            0,
            1317
        }
    },
};

s16 mainmap_room_0_header00_objectList[LENGTH_MAINMAP_ROOM_0_HEADER00_OBJECTLIST] = {
    OBJECT_HAKA,
};

ActorEntry mainmap_room_0_header00_actorList[LENGTH_MAINMAP_ROOM_0_HEADER00_ACTORLIST] = {
    // Custom Actor
    {
        /* Actor ID   */ ACTOR_COURTYARD_TOMB_STONE,
        /* Position   */ { 457, -225, 334 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(123.107), DEG_TO_BINANG(0.000) },
        /* Parameters */ 0
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_COURTYARD_TOMB_STONE,
        /* Position   */ { 580, -225, 423 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(160.373), DEG_TO_BINANG(0.000) },
        /* Parameters */ 1
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_COURTYARD_TOMB_STONE,
        /* Position   */ { 742, -226, 444 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(180.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ 2
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_COURTYARD_TOMB_STONE,
        /* Position   */ { 909, -217, 443 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(180.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ 3
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_COURTYARD_TOMB_STONE,
        /* Position   */ { 1341, -203, 411 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(219.018), DEG_TO_BINANG(0.000) },
        /* Parameters */ 4
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_COURTYARD_TOMB_STONE,
        /* Position   */ { 795, -166, -231 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(48.483), DEG_TO_BINANG(0.000) },
        /* Parameters */ 5
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_COURTYARD_TOMB_STONE,
        /* Position   */ { 946, -162, -458 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(54.657), DEG_TO_BINANG(0.000) },
        /* Parameters */ 6
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_COURTYARD_TOMB_STONE,
        /* Position   */ { 841, -163, -380 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(33.766), DEG_TO_BINANG(0.000) },
        /* Parameters */ 7
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_COURTYARD_TOMB_STONE,
        /* Position   */ { 1148, -182, 19 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(156.517), DEG_TO_BINANG(0.000) },
        /* Parameters */ 8
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_COURTYARD_TOMB_STONE,
        /* Position   */ { 1277, -166, -319 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(343.954), DEG_TO_BINANG(0.000) },
        /* Parameters */ 9
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_PROP_FLAME,
        /* Position   */ { 599, 22, -190 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ 50
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_PROP_FLAME,
        /* Position   */ { 440, 22, -19 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ 50
    },
};

