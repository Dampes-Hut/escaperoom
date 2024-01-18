#include "ultra64.h"
#include "z64.h"
#include "macros.h"
#include "test_dancefloor_scene.h"
#include "segment_symbols.h"
#include "command_macros_base.h"
#include "z64cutscene_commands.h"
#include "variables.h"

Gfx test_dancefloor_room_0_entry_0_opaque[] = {
	gsSPDisplayList(test_dancefloor_dl_Floor_mesh_layer_Opaque),
	gsSPEndDisplayList(),
};

Vtx test_dancefloor_dl_Floor_mesh_layer_Opaque_vtx_cull[8] = {
	{{ {-822, -120, 822}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-822, -120, 822}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-822, -120, -822}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-822, -120, -822}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {822, -120, 822}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {822, -120, 822}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {822, -120, -822}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {822, -120, -822}, 0, {0, 0}, {0, 0, 0, 0} }},
};

Vtx test_dancefloor_dl_Floor_mesh_layer_Opaque_vtx_0[4] = {
	{{ {-822, -120, 822}, 0, {-16, 1008}, {0, 127, 0, 255} }},
	{{ {822, -120, 822}, 0, {1008, 1008}, {0, 127, 0, 255} }},
	{{ {822, -120, -822}, 0, {1008, -16}, {0, 127, 0, 255} }},
	{{ {-822, -120, -822}, 0, {-16, -16}, {0, 127, 0, 255} }},
};

Gfx test_dancefloor_dl_Floor_mesh_layer_Opaque_tri_0[] = {
	gsSPVertex(test_dancefloor_dl_Floor_mesh_layer_Opaque_vtx_0 + 0, 4, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSPEndDisplayList(),
};

Gfx mat_test_dancefloor_dl_floor_mat_layerOpaque[] = {
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, 1, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
	gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
	gsSPSetOtherMode(G_SETOTHERMODE_H, 4, 20, G_AD_NOISE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE),
	gsSPSetOtherMode(G_SETOTHERMODE_L, 0, 32, G_AC_NONE | G_ZS_PIXEL | G_RM_FOG_SHADE_A | G_RM_AA_ZB_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
	gsSPEndDisplayList(),
};

Gfx test_dancefloor_dl_Floor_mesh_layer_Opaque[] = {
	gsSPClearGeometryMode(G_LIGHTING),
	gsSPVertex(test_dancefloor_dl_Floor_mesh_layer_Opaque_vtx_cull + 0, 8, 0),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPCullDisplayList(0, 7),
	gsSPDisplayList(mat_test_dancefloor_dl_floor_mat_layerOpaque),
	gsSPDisplayList(test_dancefloor_dl_Floor_mesh_layer_Opaque_tri_0),
	gsSPEndDisplayList(),
};

