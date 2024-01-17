#include "ultra64.h"
#include "z64.h"
#include "macros.h"
#include "test_darklink_puzzle_scene.h"
#include "segment_symbols.h"
#include "command_macros_base.h"
#include "z64cutscene_commands.h"
#include "variables.h"

Gfx test_darklink_puzzle_room_0_entry_0_opaque[] = {
	gsSPDisplayList(test_darklink_puzzle_dl_Floor_mesh_layer_Opaque),
	gsSPEndDisplayList(),
};

Vtx test_darklink_puzzle_dl_Floor_mesh_layer_Opaque_vtx_cull[8] = {
	{{ {-438, -122, 644}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-438, 66, 644}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-438, 66, -438}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-438, -122, -438}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {889, -122, 644}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {889, 66, 644}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {889, 66, -438}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {889, -122, -438}, 0, {0, 0}, {0, 0, 0, 0} }},
};

Vtx test_darklink_puzzle_dl_Floor_mesh_layer_Opaque_vtx_0[10] = {
	{{ {-438, -120, 438}, 0, {-16, 1008}, {0, 127, 0, 255} }},
	{{ {438, -120, -438}, 0, {1008, -16}, {0, 127, 0, 255} }},
	{{ {-438, -120, -438}, 0, {-16, -16}, {0, 127, 0, 255} }},
	{{ {438, -120, 438}, 0, {1008, 1008}, {0, 127, 0, 255} }},
	{{ {-145, -120, 644}, 0, {-16, 1008}, {0, 127, 0, 255} }},
	{{ {110, -120, 644}, 0, {1008, 1008}, {0, 127, 0, 255} }},
	{{ {438, -120, -438}, 0, {1008, -16}, {208, 117, 0, 255} }},
	{{ {438, -120, 438}, 0, {1008, 1008}, {208, 117, 0, 255} }},
	{{ {889, 66, 438}, 0, {1008, 1008}, {208, 117, 0, 255} }},
	{{ {889, 66, -438}, 0, {1008, -16}, {208, 117, 0, 255} }},
};

Gfx test_darklink_puzzle_dl_Floor_mesh_layer_Opaque_tri_0[] = {
	gsSPVertex(test_darklink_puzzle_dl_Floor_mesh_layer_Opaque_vtx_0 + 0, 10, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 3, 1, 0),
	gsSP2Triangles(3, 0, 4, 0, 3, 4, 5, 0),
	gsSP2Triangles(6, 7, 8, 0, 6, 8, 9, 0),
	gsSPEndDisplayList(),
};

Vtx test_darklink_puzzle_dl_Floor_mesh_layer_Opaque_vtx_1[120] = {
	{{ {109, -122, -114}, 0, {368, 1008}, {129, 0, 0, 255} }},
	{{ {109, -78, -114}, 0, {624, 1008}, {129, 0, 0, 255} }},
	{{ {109, -78, -158}, 0, {624, 752}, {129, 0, 0, 255} }},
	{{ {109, -122, -158}, 0, {368, 752}, {129, 0, 0, 255} }},
	{{ {109, -122, -158}, 0, {368, 752}, {0, 0, 129, 255} }},
	{{ {109, -78, -158}, 0, {624, 752}, {0, 0, 129, 255} }},
	{{ {153, -78, -158}, 0, {624, 496}, {0, 0, 129, 255} }},
	{{ {153, -122, -158}, 0, {368, 496}, {0, 0, 129, 255} }},
	{{ {153, -122, -158}, 0, {368, 496}, {127, 0, 0, 255} }},
	{{ {153, -78, -158}, 0, {624, 496}, {127, 0, 0, 255} }},
	{{ {153, -78, -114}, 0, {624, 240}, {127, 0, 0, 255} }},
	{{ {153, -122, -114}, 0, {368, 240}, {127, 0, 0, 255} }},
	{{ {153, -122, -114}, 0, {368, 240}, {0, 0, 127, 255} }},
	{{ {153, -78, -114}, 0, {624, 240}, {0, 0, 127, 255} }},
	{{ {109, -78, -114}, 0, {624, -16}, {0, 0, 127, 255} }},
	{{ {109, -122, -114}, 0, {368, -16}, {0, 0, 127, 255} }},
	{{ {109, -122, -158}, 0, {112, 496}, {0, 129, 0, 255} }},
	{{ {153, -122, -158}, 0, {368, 496}, {0, 129, 0, 255} }},
	{{ {153, -122, -114}, 0, {368, 240}, {0, 129, 0, 255} }},
	{{ {109, -122, -114}, 0, {112, 240}, {0, 129, 0, 255} }},
	{{ {153, -78, -158}, 0, {624, 496}, {0, 127, 0, 255} }},
	{{ {109, -78, -158}, 0, {880, 496}, {0, 127, 0, 255} }},
	{{ {109, -78, -114}, 0, {880, 240}, {0, 127, 0, 255} }},
	{{ {153, -78, -114}, 0, {624, 240}, {0, 127, 0, 255} }},
	{{ {148, -122, -76}, 0, {368, 1008}, {129, 0, 0, 255} }},
	{{ {148, -78, -76}, 0, {624, 1008}, {129, 0, 0, 255} }},
	{{ {148, -78, -120}, 0, {624, 752}, {129, 0, 0, 255} }},
	{{ {148, -122, -120}, 0, {368, 752}, {129, 0, 0, 255} }},
	{{ {148, -122, -120}, 0, {368, 752}, {0, 0, 129, 255} }},
	{{ {148, -78, -120}, 0, {624, 752}, {0, 0, 129, 255} }},
	{{ {192, -78, -120}, 0, {624, 496}, {0, 0, 129, 255} }},
	{{ {192, -122, -120}, 0, {368, 496}, {0, 0, 129, 255} }},
	{{ {192, -122, -120}, 0, {368, 496}, {127, 0, 0, 255} }},
	{{ {192, -78, -120}, 0, {624, 496}, {127, 0, 0, 255} }},
	{{ {192, -78, -76}, 0, {624, 240}, {127, 0, 0, 255} }},
	{{ {192, -122, -76}, 0, {368, 240}, {127, 0, 0, 255} }},
	{{ {192, -122, -76}, 0, {368, 240}, {0, 0, 127, 255} }},
	{{ {192, -78, -76}, 0, {624, 240}, {0, 0, 127, 255} }},
	{{ {148, -78, -76}, 0, {624, -16}, {0, 0, 127, 255} }},
	{{ {148, -122, -76}, 0, {368, -16}, {0, 0, 127, 255} }},
	{{ {148, -122, -120}, 0, {112, 496}, {0, 129, 0, 255} }},
	{{ {192, -122, -120}, 0, {368, 496}, {0, 129, 0, 255} }},
	{{ {192, -122, -76}, 0, {368, 240}, {0, 129, 0, 255} }},
	{{ {148, -122, -76}, 0, {112, 240}, {0, 129, 0, 255} }},
	{{ {192, -78, -120}, 0, {624, 496}, {0, 127, 0, 255} }},
	{{ {148, -78, -120}, 0, {880, 496}, {0, 127, 0, 255} }},
	{{ {148, -78, -76}, 0, {880, 240}, {0, 127, 0, 255} }},
	{{ {192, -78, -76}, 0, {624, 240}, {0, 127, 0, 255} }},
	{{ {109, -122, 121}, 0, {368, 1008}, {129, 0, 0, 255} }},
	{{ {109, -78, 121}, 0, {624, 1008}, {129, 0, 0, 255} }},
	{{ {109, -78, 77}, 0, {624, 752}, {129, 0, 0, 255} }},
	{{ {109, -122, 77}, 0, {368, 752}, {129, 0, 0, 255} }},
	{{ {109, -122, 77}, 0, {368, 752}, {0, 0, 129, 255} }},
	{{ {109, -78, 77}, 0, {624, 752}, {0, 0, 129, 255} }},
	{{ {153, -78, 77}, 0, {624, 496}, {0, 0, 129, 255} }},
	{{ {153, -122, 77}, 0, {368, 496}, {0, 0, 129, 255} }},
	{{ {153, -122, 77}, 0, {368, 496}, {127, 0, 0, 255} }},
	{{ {153, -78, 77}, 0, {624, 496}, {127, 0, 0, 255} }},
	{{ {153, -78, 121}, 0, {624, 240}, {127, 0, 0, 255} }},
	{{ {153, -122, 121}, 0, {368, 240}, {127, 0, 0, 255} }},
	{{ {153, -122, 121}, 0, {368, 240}, {0, 0, 127, 255} }},
	{{ {153, -78, 121}, 0, {624, 240}, {0, 0, 127, 255} }},
	{{ {109, -78, 121}, 0, {624, -16}, {0, 0, 127, 255} }},
	{{ {109, -122, 121}, 0, {368, -16}, {0, 0, 127, 255} }},
	{{ {109, -122, 77}, 0, {112, 496}, {0, 129, 0, 255} }},
	{{ {153, -122, 77}, 0, {368, 496}, {0, 129, 0, 255} }},
	{{ {153, -122, 121}, 0, {368, 240}, {0, 129, 0, 255} }},
	{{ {109, -122, 121}, 0, {112, 240}, {0, 129, 0, 255} }},
	{{ {153, -78, 77}, 0, {624, 496}, {0, 127, 0, 255} }},
	{{ {109, -78, 77}, 0, {880, 496}, {0, 127, 0, 255} }},
	{{ {109, -78, 121}, 0, {880, 240}, {0, 127, 0, 255} }},
	{{ {153, -78, 121}, 0, {624, 240}, {0, 127, 0, 255} }},
	{{ {-137, -122, -16}, 0, {368, 1008}, {129, 0, 0, 255} }},
	{{ {-137, -78, -16}, 0, {624, 1008}, {129, 0, 0, 255} }},
	{{ {-137, -78, -180}, 0, {624, 752}, {129, 0, 0, 255} }},
	{{ {-137, -122, -180}, 0, {368, 752}, {129, 0, 0, 255} }},
	{{ {-137, -122, -180}, 0, {368, 752}, {0, 0, 129, 255} }},
	{{ {-137, -78, -180}, 0, {624, 752}, {0, 0, 129, 255} }},
	{{ {-93, -78, -180}, 0, {624, 496}, {0, 0, 129, 255} }},
	{{ {-93, -122, -180}, 0, {368, 496}, {0, 0, 129, 255} }},
	{{ {-93, -122, -180}, 0, {368, 496}, {127, 0, 0, 255} }},
	{{ {-93, -78, -180}, 0, {624, 496}, {127, 0, 0, 255} }},
	{{ {-93, -78, -16}, 0, {624, 240}, {127, 0, 0, 255} }},
	{{ {-93, -122, -16}, 0, {368, 240}, {127, 0, 0, 255} }},
	{{ {-93, -122, -16}, 0, {368, 240}, {0, 0, 127, 255} }},
	{{ {-93, -78, -16}, 0, {624, 240}, {0, 0, 127, 255} }},
	{{ {-137, -78, -16}, 0, {624, -16}, {0, 0, 127, 255} }},
	{{ {-137, -122, -16}, 0, {368, -16}, {0, 0, 127, 255} }},
	{{ {-137, -122, -180}, 0, {112, 496}, {0, 129, 0, 255} }},
	{{ {-93, -122, -180}, 0, {368, 496}, {0, 129, 0, 255} }},
	{{ {-93, -122, -16}, 0, {368, 240}, {0, 129, 0, 255} }},
	{{ {-137, -122, -16}, 0, {112, 240}, {0, 129, 0, 255} }},
	{{ {-93, -78, -180}, 0, {624, 496}, {0, 127, 0, 255} }},
	{{ {-137, -78, -180}, 0, {880, 496}, {0, 127, 0, 255} }},
	{{ {-137, -78, -16}, 0, {880, 240}, {0, 127, 0, 255} }},
	{{ {-93, -78, -16}, 0, {624, 240}, {0, 127, 0, 255} }},
	{{ {-24, -122, 224}, 0, {368, 1008}, {174, 0, 97, 255} }},
	{{ {-24, -78, 224}, 0, {624, 1008}, {174, 0, 97, 255} }},
	{{ {-150, -78, 118}, 0, {624, 752}, {174, 0, 97, 255} }},
	{{ {-150, -122, 118}, 0, {368, 752}, {174, 0, 97, 255} }},
	{{ {-150, -122, 118}, 0, {368, 752}, {159, 0, 174, 255} }},
	{{ {-150, -78, 118}, 0, {624, 752}, {159, 0, 174, 255} }},
	{{ {-122, -78, 84}, 0, {624, 496}, {159, 0, 174, 255} }},
	{{ {-122, -122, 84}, 0, {368, 496}, {159, 0, 174, 255} }},
	{{ {-122, -122, 84}, 0, {368, 496}, {82, 0, 159, 255} }},
	{{ {-122, -78, 84}, 0, {624, 496}, {82, 0, 159, 255} }},
	{{ {4, -78, 190}, 0, {624, 240}, {82, 0, 159, 255} }},
	{{ {4, -122, 190}, 0, {368, 240}, {82, 0, 159, 255} }},
	{{ {4, -122, 190}, 0, {368, 240}, {97, 0, 82, 255} }},
	{{ {4, -78, 190}, 0, {624, 240}, {97, 0, 82, 255} }},
	{{ {-24, -78, 224}, 0, {624, -16}, {97, 0, 82, 255} }},
	{{ {-24, -122, 224}, 0, {368, -16}, {97, 0, 82, 255} }},
	{{ {-150, -122, 118}, 0, {112, 496}, {0, 129, 0, 255} }},
	{{ {-122, -122, 84}, 0, {368, 496}, {0, 129, 0, 255} }},
	{{ {4, -122, 190}, 0, {368, 240}, {0, 129, 0, 255} }},
	{{ {-24, -122, 224}, 0, {112, 240}, {0, 129, 0, 255} }},
	{{ {-122, -78, 84}, 0, {624, 496}, {0, 127, 0, 255} }},
	{{ {-150, -78, 118}, 0, {880, 496}, {0, 127, 0, 255} }},
	{{ {-24, -78, 224}, 0, {880, 240}, {0, 127, 0, 255} }},
	{{ {4, -78, 190}, 0, {624, 240}, {0, 127, 0, 255} }},
};

Gfx test_darklink_puzzle_dl_Floor_mesh_layer_Opaque_tri_1[] = {
	gsSPVertex(test_darklink_puzzle_dl_Floor_mesh_layer_Opaque_vtx_1 + 0, 32, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
	gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0),
	gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
	gsSP2Triangles(16, 17, 18, 0, 16, 18, 19, 0),
	gsSP2Triangles(20, 21, 22, 0, 20, 22, 23, 0),
	gsSP2Triangles(24, 25, 26, 0, 24, 26, 27, 0),
	gsSP2Triangles(28, 29, 30, 0, 28, 30, 31, 0),
	gsSPVertex(test_darklink_puzzle_dl_Floor_mesh_layer_Opaque_vtx_1 + 32, 32, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
	gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0),
	gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
	gsSP2Triangles(16, 17, 18, 0, 16, 18, 19, 0),
	gsSP2Triangles(20, 21, 22, 0, 20, 22, 23, 0),
	gsSP2Triangles(24, 25, 26, 0, 24, 26, 27, 0),
	gsSP2Triangles(28, 29, 30, 0, 28, 30, 31, 0),
	gsSPVertex(test_darklink_puzzle_dl_Floor_mesh_layer_Opaque_vtx_1 + 64, 32, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
	gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0),
	gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
	gsSP2Triangles(16, 17, 18, 0, 16, 18, 19, 0),
	gsSP2Triangles(20, 21, 22, 0, 20, 22, 23, 0),
	gsSP2Triangles(24, 25, 26, 0, 24, 26, 27, 0),
	gsSP2Triangles(28, 29, 30, 0, 28, 30, 31, 0),
	gsSPVertex(test_darklink_puzzle_dl_Floor_mesh_layer_Opaque_vtx_1 + 96, 24, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
	gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0),
	gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
	gsSP2Triangles(16, 17, 18, 0, 16, 18, 19, 0),
	gsSP2Triangles(20, 21, 22, 0, 20, 22, 23, 0),
	gsSPEndDisplayList(),
};

Gfx mat_test_darklink_puzzle_dl_floor_mat_layerOpaque[] = {
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, 1, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
	gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
	gsSPSetOtherMode(G_SETOTHERMODE_H, 4, 20, G_AD_NOISE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE),
	gsSPSetOtherMode(G_SETOTHERMODE_L, 0, 32, G_AC_NONE | G_ZS_PIXEL | G_RM_FOG_SHADE_A | G_RM_AA_ZB_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 147, 255, 127, 255),
	gsSPEndDisplayList(),
};

Gfx mat_test_darklink_puzzle_dl_f3dlite_material_layerOpaque[] = {
	gsDPPipeSync(),
	gsDPSetCombineLERP(1, 0, SHADE, 0, 0, 0, 0, 1, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
	gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
	gsSPSetOtherMode(G_SETOTHERMODE_H, 4, 20, G_AD_NOISE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE),
	gsSPSetOtherMode(G_SETOTHERMODE_L, 0, 32, G_AC_NONE | G_ZS_PIXEL | G_RM_FOG_SHADE_A | G_RM_AA_ZB_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 193, 201, 255, 255),
	gsSPEndDisplayList(),
};

Gfx test_darklink_puzzle_dl_Floor_mesh_layer_Opaque[] = {
	gsSPClearGeometryMode(G_LIGHTING),
	gsSPVertex(test_darklink_puzzle_dl_Floor_mesh_layer_Opaque_vtx_cull + 0, 8, 0),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPCullDisplayList(0, 7),
	gsSPDisplayList(mat_test_darklink_puzzle_dl_floor_mat_layerOpaque),
	gsSPDisplayList(test_darklink_puzzle_dl_Floor_mesh_layer_Opaque_tri_0),
	gsSPDisplayList(mat_test_darklink_puzzle_dl_f3dlite_material_layerOpaque),
	gsSPDisplayList(test_darklink_puzzle_dl_Floor_mesh_layer_Opaque_tri_1),
	gsSPEndDisplayList(),
};
