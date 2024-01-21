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
#define LENGTH_MAINMAP_ROOM_4_HEADER00_LIGHTINFOLIST 1
#define LENGTH_MAINMAP_ROOM_4_HEADER00_ACTORLIST 7
SceneCmd mainmap_room_4_header00[] = {
    SCENE_CMD_ECHO_SETTINGS(0x00),
    SCENE_CMD_ROOM_BEHAVIOR_PL(0x00, 0x00, false, false, true),
    SCENE_CMD_SKYBOX_DISABLES(false, true),
    SCENE_CMD_TIME_SETTINGS(0, 0, 0),
    SCENE_CMD_ROOM_SHAPE(&mainmap_room_4_shapeHeader),
    SCENE_CMD_LIGHT_LIST(LENGTH_MAINMAP_ROOM_4_HEADER00_LIGHTINFOLIST, mainmap_room_4_header00_lightInfoList),
    SCENE_CMD_ACTOR_LIST(LENGTH_MAINMAP_ROOM_4_HEADER00_ACTORLIST, mainmap_room_4_header00_actorList),
    SCENE_CMD_END(),
};

LightInfo mainmap_room_4_header00_lightInfoList[LENGTH_MAINMAP_ROOM_4_HEADER00_LIGHTINFOLIST] = {
    {
        LIGHT_POINT_NOGLOW,
        .params.point = {
            1628, -100, 906,
            { 255, 213, 130 },
            0,
            913
        }
    },
};

ActorEntry mainmap_room_4_header00_actorList[LENGTH_MAINMAP_ROOM_4_HEADER00_ACTORLIST] = {
    // Collectable Item
    {
        /* Actor ID   */ ACTOR_EN_ITEM00,
        /* Position   */ { 1297, -249, 1222 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ ITEM00_SMALL_KEY | (1 << 8 /*collectibleflag1*/)
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_OUR_TEXT_SPOT,
        /* Position   */ { 1320, -206, 614 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ 200
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_OUR_TEXT_SPOT,
        /* Position   */ { 1580, -228, 895 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ 201
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_OUR_TEXT_SPOT,
        /* Position   */ { 1420, -190, 1210 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ 203
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_OUR_TEXT_SPOT,
        /* Position   */ { 1893, -193, 681 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ 205
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_OUR_TEXT_SPOT,
        /* Position   */ { 1620, -194, 1184 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ 206
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_OUR_TEXT_SPOT,
        /* Position   */ { 1916, -183, 989 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ 207
    },
};

