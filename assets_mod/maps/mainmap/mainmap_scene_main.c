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
SceneCmd mainmap_scene_header00[] = {
    SCENE_CMD_ALTERNATE_HEADER_LIST(mainmap_scene_alternateHeaders),
    SCENE_CMD_SOUND_SETTINGS(0x00, 0x13, NA_BGM_INSIDE_DEKU_TREE),
    SCENE_CMD_ROOM_LIST(6, mainmap_scene_roomList),
    SCENE_CMD_MISC_SETTINGS(0x00, 0x00),
    SCENE_CMD_COL_HEADER(&mainmap_collisionHeader),
    SCENE_CMD_SPECIAL_FILES(0x00, OBJECT_GAMEPLAY_DANGEON_KEEP),
    SCENE_CMD_SKYBOX_SETTINGS(0x01, 0x00, LIGHT_MODE_SETTINGS),
    SCENE_CMD_ENTRANCE_LIST(mainmap_scene_header00_entranceList),
    SCENE_CMD_SPAWN_LIST(5, mainmap_scene_header00_playerEntryList),
    SCENE_CMD_ENV_LIGHT_SETTINGS(1, mainmap_scene_header00_lightSettings),
    SCENE_CMD_TRANSITION_ACTOR_LIST(7, mainmap_scene_header00_transitionActors),
    SCENE_CMD_END(),
};

SceneCmd* mainmap_scene_alternateHeaders[] = {
    NULL,
    NULL,
    NULL,
    mainmap_scene_header04,
};

RomFile mainmap_scene_roomList[] = {
    { (u32)_mainmap_room_0SegmentRomStart, (u32)_mainmap_room_0SegmentRomEnd },
    { (u32)_mainmap_room_1SegmentRomStart, (u32)_mainmap_room_1SegmentRomEnd },
    { (u32)_mainmap_room_2SegmentRomStart, (u32)_mainmap_room_2SegmentRomEnd },
    { (u32)_mainmap_room_3SegmentRomStart, (u32)_mainmap_room_3SegmentRomEnd },
    { (u32)_mainmap_room_4SegmentRomStart, (u32)_mainmap_room_4SegmentRomEnd },
    { (u32)_mainmap_room_5SegmentRomStart, (u32)_mainmap_room_5SegmentRomEnd },
};

ActorEntry mainmap_scene_header00_playerEntryList[] = {
    {
        /* Actor ID   */ ACTOR_PLAYER,
        /* Position   */ { -259, -123, -218 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(338.665), DEG_TO_BINANG(0.000) },
        /* Parameters */ 0x0FFF
    },
    {
        /* Actor ID   */ ACTOR_PLAYER,
        /* Position   */ { 1448, -251, 792 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(52.504), DEG_TO_BINANG(0.000) },
        /* Parameters */ 0x0FFF
    },
    {
        /* Actor ID   */ ACTOR_PLAYER,
        /* Position   */ { 928, -290, 1273 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(275.999), DEG_TO_BINANG(0.000) },
        /* Parameters */ 0x0FFF
    },
    {
        /* Actor ID   */ ACTOR_PLAYER,
        /* Position   */ { 961, -249, 685 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(275.999), DEG_TO_BINANG(0.000) },
        /* Parameters */ 0x0FFF
    },
    {
        /* Actor ID   */ ACTOR_PLAYER,
        /* Position   */ { 343, 1462, -483 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(86.375), DEG_TO_BINANG(0.000) },
        /* Parameters */ 0x0FFF
    },
};

TransitionActorEntry mainmap_scene_header00_transitionActors[] = {
    // Wooden Door
    {
        /* Room & Cam Index (Front, Back) */ { 0, 0x00, 1, 0x00 },
        /* Actor ID                       */ ACTOR_EN_DOOR,
        /* Position                       */ { 513, -151, -113 },
        /* Rotation Y                     */ DEG_TO_BINANG(227.681),
        /* Parameters                     */ 5<<7/*modded DOOR_CHECKABLE*/
    },

    // Wooden Door
    {
        /* Room & Cam Index (Front, Back) */ { 2, 0x00, 0, 0x00 },
        /* Actor ID                       */ ACTOR_EN_DOOR,
        /* Position                       */ { 1145, -175, 580 },
        /* Rotation Y                     */ DEG_TO_BINANG(180.709),
        /* Parameters                     */ (1<<7) | 0 /*locked, switchflag0*/
    },

    // Wooden Door
    {
        /* Room & Cam Index (Front, Back) */ { 2, 0x00, 3, 0x00 },
        /* Actor ID                       */ ACTOR_EN_DOOR,
        /* Position                       */ { 1030, -249, 897 },
        /* Rotation Y                     */ DEG_TO_BINANG(270.000),
        /* Parameters                     */ 0x0000
    },

    // Wooden Door
    {
        /* Room & Cam Index (Front, Back) */ { 5, 0x00, 2, 0x00 },
        /* Actor ID                       */ ACTOR_EN_DOOR,
        /* Position                       */ { 1043, -249, 1274 },
        /* Rotation Y                     */ DEG_TO_BINANG(90.000),
        /* Parameters                     */ (1<<7) | 1 /*locked, switchflag1*/
    },

    // Wooden Door
    {
        /* Room & Cam Index (Front, Back) */ { 2, 0x00, 4, 0x00 },
        /* Actor ID                       */ ACTOR_EN_DOOR,
        /* Position                       */ { 1269, -249, 889 },
        /* Rotation Y                     */ DEG_TO_BINANG(90.000),
        /* Parameters                     */ 0x0000
    },

    // Wooden Door
    {
        /* Room & Cam Index (Front, Back) */ { 2, 0x00, 3, 0x00 },
        /* Actor ID                       */ ACTOR_EN_DOOR,
        /* Position                       */ { 1039, -63, 1179 },
        /* Rotation Y                     */ DEG_TO_BINANG(270.000),
        /* Parameters                     */ 0x0000
    },

    // Wooden Door
    {
        /* Room & Cam Index (Front, Back) */ { 2, 0x00, 4, 0x00 },
        /* Actor ID                       */ ACTOR_EN_DOOR,
        /* Position                       */ { 1278, -63, 1171 },
        /* Rotation Y                     */ DEG_TO_BINANG(90.000),
        /* Parameters                     */ 0x0000
    },
};

Spawn mainmap_scene_header00_entranceList[] = {
    // { Spawn Actor List Index, Room Index }
    { 0, 0 },
    { 1, 4 },
    { 2, 5 },
    { 3, 3 },
    { 4, 0 },
};

EnvLightSettings mainmap_scene_header00_lightSettings[1] = {
    // Indoor No. 1 Lighting
    {
        {    40,    36,    57 },   // Ambient Color
        {    73,    73,    73 },   // Diffuse0 Direction
        {     0,     0,     0 },   // Diffuse0 Color
        {   -73,   -73,   -73 },   // Diffuse1 Direction
        {     0,     0,     0 },   // Diffuse1 Color
        {    70,    54,    79 },   // Fog Color
        ((1 << 10) | 993),         // Blend Rate & Fog Near
        3000,                      // Fog Far
    },
};

/**
 * Header Cutscene No. 1
*/
SceneCmd mainmap_scene_header04[] = {
    SCENE_CMD_SOUND_SETTINGS(0x00, 0x00, NA_BGM_NATURE_AMBIENCE),
    SCENE_CMD_ROOM_LIST(6, mainmap_scene_roomList),
    SCENE_CMD_MISC_SETTINGS(0x00, 0x00),
    SCENE_CMD_COL_HEADER(&mainmap_collisionHeader),
    SCENE_CMD_SPECIAL_FILES(0x00, OBJECT_GAMEPLAY_DANGEON_KEEP),
    SCENE_CMD_SKYBOX_SETTINGS(0x01, 0x00, LIGHT_MODE_SETTINGS),
    SCENE_CMD_ENTRANCE_LIST(mainmap_scene_header04_entranceList),
    SCENE_CMD_SPAWN_LIST(5, mainmap_scene_header04_playerEntryList),
    SCENE_CMD_ENV_LIGHT_SETTINGS(1, mainmap_scene_header04_lightSettings),
    SCENE_CMD_TRANSITION_ACTOR_LIST(7, mainmap_scene_header04_transitionActors),
    SCENE_CMD_CUTSCENE_DATA(gIntroPart2Cs),
    SCENE_CMD_END(),
};

ActorEntry mainmap_scene_header04_playerEntryList[] = {
    {
        /* Actor ID   */ ACTOR_PLAYER,
        /* Position   */ { -259, -123, -218 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(338.665), DEG_TO_BINANG(0.000) },
        /* Parameters */ 0x0FFF
    },
    {
        /* Actor ID   */ ACTOR_PLAYER,
        /* Position   */ { 1448, -251, 792 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(52.504), DEG_TO_BINANG(0.000) },
        /* Parameters */ 0x0FFF
    },
    {
        /* Actor ID   */ ACTOR_PLAYER,
        /* Position   */ { 928, -290, 1273 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(275.999), DEG_TO_BINANG(0.000) },
        /* Parameters */ 0x0FFF
    },
    {
        /* Actor ID   */ ACTOR_PLAYER,
        /* Position   */ { 961, -249, 685 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(275.999), DEG_TO_BINANG(0.000) },
        /* Parameters */ 0x0FFF
    },
    {
        /* Actor ID   */ ACTOR_PLAYER,
        /* Position   */ { 343, 1462, -483 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(86.375), DEG_TO_BINANG(0.000) },
        /* Parameters */ 0x0FFF
    },
};

TransitionActorEntry mainmap_scene_header04_transitionActors[] = {
    // Wooden Door
    {
        /* Room & Cam Index (Front, Back) */ { 0, 0x00, 1, 0x00 },
        /* Actor ID                       */ ACTOR_EN_DOOR,
        /* Position                       */ { 513, -151, -113 },
        /* Rotation Y                     */ DEG_TO_BINANG(227.681),
        /* Parameters                     */ 5<<7/*modded DOOR_CHECKABLE*/
    },

    // Wooden Door
    {
        /* Room & Cam Index (Front, Back) */ { 2, 0x00, 0, 0x00 },
        /* Actor ID                       */ ACTOR_EN_DOOR,
        /* Position                       */ { 1145, -175, 580 },
        /* Rotation Y                     */ DEG_TO_BINANG(180.709),
        /* Parameters                     */ (1<<7) | 0 /*locked, switchflag0*/
    },

    // Wooden Door
    {
        /* Room & Cam Index (Front, Back) */ { 2, 0x00, 3, 0x00 },
        /* Actor ID                       */ ACTOR_EN_DOOR,
        /* Position                       */ { 1030, -249, 897 },
        /* Rotation Y                     */ DEG_TO_BINANG(270.000),
        /* Parameters                     */ 0x0000
    },

    // Wooden Door
    {
        /* Room & Cam Index (Front, Back) */ { 5, 0x00, 2, 0x00 },
        /* Actor ID                       */ ACTOR_EN_DOOR,
        /* Position                       */ { 1043, -249, 1274 },
        /* Rotation Y                     */ DEG_TO_BINANG(90.000),
        /* Parameters                     */ (1<<7) | 1 /*locked, switchflag1*/
    },

    // Wooden Door
    {
        /* Room & Cam Index (Front, Back) */ { 2, 0x00, 4, 0x00 },
        /* Actor ID                       */ ACTOR_EN_DOOR,
        /* Position                       */ { 1269, -249, 889 },
        /* Rotation Y                     */ DEG_TO_BINANG(90.000),
        /* Parameters                     */ 0x0000
    },

    // Wooden Door
    {
        /* Room & Cam Index (Front, Back) */ { 2, 0x00, 3, 0x00 },
        /* Actor ID                       */ ACTOR_EN_DOOR,
        /* Position                       */ { 1039, -63, 1179 },
        /* Rotation Y                     */ DEG_TO_BINANG(270.000),
        /* Parameters                     */ 0x0000
    },

    // Wooden Door
    {
        /* Room & Cam Index (Front, Back) */ { 2, 0x00, 4, 0x00 },
        /* Actor ID                       */ ACTOR_EN_DOOR,
        /* Position                       */ { 1278, -63, 1171 },
        /* Rotation Y                     */ DEG_TO_BINANG(90.000),
        /* Parameters                     */ 0x0000
    },
};

Spawn mainmap_scene_header04_entranceList[] = {
    // { Spawn Actor List Index, Room Index }
    { 0, 0 },
    { 1, 4 },
    { 2, 5 },
    { 3, 3 },
    { 4, 0 },
};

EnvLightSettings mainmap_scene_header04_lightSettings[1] = {
    // Indoor No. 1 Lighting
    {
        {    40,    36,    57 },   // Ambient Color
        {    73,    73,    73 },   // Diffuse0 Direction
        {     0,     0,     0 },   // Diffuse0 Color
        {   -73,   -73,   -73 },   // Diffuse1 Direction
        {     0,     0,     0 },   // Diffuse1 Color
        {    70,    54,    79 },   // Fog Color
        ((1 << 10) | 993),         // Blend Rate & Fog Near
        3000,                      // Fog Far
    },
};

