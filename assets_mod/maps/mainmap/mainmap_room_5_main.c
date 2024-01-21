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
#define LENGTH_MAINMAP_ROOM_5_HEADER00_LIGHTINFOLIST 1
#define LENGTH_MAINMAP_ROOM_5_HEADER00_OBJECTLIST 1
#define LENGTH_MAINMAP_ROOM_5_HEADER00_ACTORLIST 6
SceneCmd mainmap_room_5_header00[] = {
    SCENE_CMD_ECHO_SETTINGS(0x00),
    SCENE_CMD_ROOM_BEHAVIOR_PL(0x00, 0x00, false, false, true),
    SCENE_CMD_SKYBOX_DISABLES(false, true),
    SCENE_CMD_TIME_SETTINGS(0, 0, 0),
    SCENE_CMD_ROOM_SHAPE(&mainmap_room_5_shapeHeader),
    SCENE_CMD_LIGHT_LIST(LENGTH_MAINMAP_ROOM_5_HEADER00_LIGHTINFOLIST, mainmap_room_5_header00_lightInfoList),
    SCENE_CMD_OBJECT_LIST(LENGTH_MAINMAP_ROOM_5_HEADER00_OBJECTLIST, mainmap_room_5_header00_objectList),
    SCENE_CMD_ACTOR_LIST(LENGTH_MAINMAP_ROOM_5_HEADER00_ACTORLIST, mainmap_room_5_header00_actorList),
    SCENE_CMD_END(),
};

LightInfo mainmap_room_5_header00_lightInfoList[LENGTH_MAINMAP_ROOM_5_HEADER00_LIGHTINFOLIST] = {
    {
        LIGHT_POINT_NOGLOW,
        .params.point = {
            516, -300, 1379,
            { 255, 213, 130 },
            0,
            1162
        }
    },
};

s16 mainmap_room_5_header00_objectList[LENGTH_MAINMAP_ROOM_5_HEADER00_OBJECTLIST] = {
    OBJECT_TORCH2,
};

ActorEntry mainmap_room_5_header00_actorList[LENGTH_MAINMAP_ROOM_5_HEADER00_ACTORLIST] = {
    // Custom Actor
    {
        /* Actor ID   */ ACTOR_PROP_FLAME,
        /* Position   */ { 816, -359, 1156 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ 50
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_MUH_DARK_LINK,
        /* Position   */ { 636, -359, 1343 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ 0x0000
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_OUR_TEXT_SPOT,
        /* Position   */ { 792, -312, 1678 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ 202
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_OUR_TEXT_SPOT,
        /* Position   */ { 799, -314, 1389 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ 204
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_OUR_TEXT_SPOT,
        /* Position   */ { 468, -316, 1382 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ 208
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_OUR_TEXT_SPOT,
        /* Position   */ { 481, -314, 1685 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ 209
    },
};

