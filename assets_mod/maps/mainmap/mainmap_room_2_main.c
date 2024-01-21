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
#define LENGTH_MAINMAP_ROOM_2_HEADER00_LIGHTINFOLIST 1
SceneCmd mainmap_room_2_header00[] = {
    SCENE_CMD_ECHO_SETTINGS(0x00),
    SCENE_CMD_ROOM_BEHAVIOR_PL(0x00, 0x00, false, false, true),
    SCENE_CMD_SKYBOX_DISABLES(false, true),
    SCENE_CMD_TIME_SETTINGS(0, 0, 0),
    SCENE_CMD_ROOM_SHAPE(&mainmap_room_2_shapeHeader),
    SCENE_CMD_LIGHT_LIST(LENGTH_MAINMAP_ROOM_2_HEADER00_LIGHTINFOLIST, mainmap_room_2_header00_lightInfoList),
    SCENE_CMD_END(),
};

LightInfo mainmap_room_2_header00_lightInfoList[LENGTH_MAINMAP_ROOM_2_HEADER00_LIGHTINFOLIST] = {
    {
        LIGHT_POINT_NOGLOW,
        .params.point = {
            1137, -151, 911,
            { 255, 213, 130 },
            0,
            495
        }
    },
};

