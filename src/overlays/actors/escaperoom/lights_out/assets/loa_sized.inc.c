/*
 * assets that depend on the board cell count
 */

// MESH

#define TC(x) ((x)*32)
#define TCB(x) (((x)-0.5) * 32)

#define TC_TOP_MAX_X (LIGHTSOUT_GRID_X * 32.0)
#define TC_TOP_MAX_Y (LIGHTSOUT_GRID_Y * 32.0)

// This shift is required to maximize the texture coordinate precision for point-sampling a low-resolution texture.
// The larger the shift the more precision, best possible for given N is S = log2(32767/(32*N)) = 9 - log2(N)
#define TC_OVL_SFT_X 6
#define TC_OVL_SFT_Y 6
#define TC_OVL_MAX_X (LIGHTSOUT_GRID_X * (1 << TC_OVL_SFT_X))
#define TC_OVL_MAX_Y (LIGHTSOUT_GRID_Y * (1 << TC_OVL_SFT_Y))

// clang-format off
Vtx LIGHTSOUT_SIZED_ASSET_NAME(sLightsOutBoard_Vtx)[20] = {
    // EDGES
    {{ {  25000,   0,  25000 }, 0, { TCB(           336.0), TCB(            32.0) }, { 255, 255, 255, 255 } }},
    {{ {  25000, 750,  25000 }, 0, { TCB(           336.0), TCB(             0.0) }, { 255, 255, 255, 255 } }},
    {{ { -25000, 750,  25000 }, 0, { TCB(          -304.0), TCB(             0.0) }, { 255, 255, 255, 255 } }},
    {{ { -25000,   0,  25000 }, 0, { TCB(          -304.0), TCB(            32.0) }, { 255, 255, 255, 255 } }},
    {{ {  25000,   0, -25000 }, 0, { TCB(           336.0), TCB(            32.0) }, { 255, 255, 255, 255 } }},
    {{ {  25000, 750,  25000 }, 0, { TCB(          -304.0), TCB(             0.0) }, { 255, 255, 255, 255 } }},
    {{ {  25000,   0,  25000 }, 0, { TCB(          -304.0), TCB(            32.0) }, { 255, 255, 255, 255 } }},
    {{ {  25000, 750, -25000 }, 0, { TCB(           336.0), TCB(             0.0) }, { 255, 255, 255, 255 } }},
    {{ { -25000,   0, -25000 }, 0, { TCB(          -304.0), TCB(            32.0) }, { 255, 255, 255, 255 } }},
    {{ { -25000, 750, -25000 }, 0, { TCB(          -304.0), TCB(             0.0) }, { 255, 255, 255, 255 } }},
    {{ { -25000,   0,  25000 }, 0, { TCB(           336.0), TCB(            32.0) }, { 255, 255, 255, 255 } }},
    {{ { -25000, 750,  25000 }, 0, { TCB(           336.0), TCB(             0.0) }, { 255, 255, 255, 255 } }},

    // TOP
    {{ { -25000, 750, -25000 }, 0, { TCB(         0.0-1.5), TCB(         0.0-1.5) }, { 255, 255, 255, 255 } }},
    {{ { -25000, 750,  25000 }, 0, { TCB(         0.0-1.5), TCB(TC_TOP_MAX_Y-1.5) }, { 255, 255, 255, 255 } }},
    {{ {  25000, 750,  25000 }, 0, { TCB(TC_TOP_MAX_X-1.5), TCB(TC_TOP_MAX_Y-1.5) }, { 255, 255, 255, 255 } }},
    {{ {  25000, 750, -25000 }, 0, { TCB(TC_TOP_MAX_X-1.5), TCB(         0.0-1.5) }, { 255, 255, 255, 255 } }},

    // OVERLAY
    {{ { -25000, 750, -25000 }, 0, { TC (             0.0), TC (             0.0) }, { 255, 255, 255, 255 } }},
    {{ { -25000, 750,  25000 }, 0, { TC (             0.0), TC (    TC_OVL_MAX_Y) }, { 255, 255, 255, 255 } }},
    {{ {  25000, 750,  25000 }, 0, { TC (    TC_OVL_MAX_X), TC (    TC_OVL_MAX_Y) }, { 255, 255, 255, 255 } }},
    {{ {  25000, 750, -25000 }, 0, { TC (    TC_OVL_MAX_X), TC (             0.0) }, { 255, 255, 255, 255 } }},
};
// clang-format on

Gfx LIGHTSOUT_SIZED_ASSET_NAME(gLightsOutBoardDL)[] = {
    gsSPClearGeometryMode(G_LIGHTING | G_FOG | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPVertex(sLightsOutBoard_CullVtx, ARRAY_COUNT(sLightsOutBoard_CullVtx), 0),
    gsSPCullDisplayList(0, 7),

    // EDGES

    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_SHADING_SMOOTH),
    gsSPTexture(65535, 65535, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetOtherMode(G_AD_NOISE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE |
                         G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE,
                     G_AC_NONE | G_ZS_PIXEL | G_RM_FOG_SHADE_A | G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 1, PRIMITIVE, 0, 0, 0, COMBINED, 0, 0, 0, COMBINED),

    gsDPSetTextureImage(G_IM_FMT_I, G_IM_SIZ_8b_LOAD_BLOCK, 1, sLightsOutBoardEdgeTex),
    gsDPSetTile(G_IM_FMT_I, G_IM_SIZ_8b_LOAD_BLOCK, 0, 0x000 / 8, G_TX_LOADTILE, 0, G_TX_WRAP | G_TX_NOMIRROR,
                G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 511, 512),
    gsDPSetTile(G_IM_FMT_I, G_IM_SIZ_8b, 4, 0x000 / 8, G_TX_RENDERTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD,
                G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD),
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, 31 << G_TEXTURE_IMAGE_FRAC, 31 << G_TEXTURE_IMAGE_FRAC),

    gsSPVertex(LIGHTSOUT_SIZED_ASSET_NAME(sLightsOutBoard_Vtx), ARRAY_COUNT(LIGHTSOUT_SIZED_ASSET_NAME(sLightsOutBoard_Vtx)), 0),

    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSP2Triangles(4, 5, 6, 0, 4, 7, 5, 0),
    gsSP2Triangles(8, 7, 4, 0, 8, 9, 7, 0),
    gsSP2Triangles(10, 9, 8, 0, 10, 11, 9, 0),

    // TOP

    gsDPPipeSync(),
    gsDPSetOtherMode(G_AD_NOISE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE |
                         G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE,
                     G_AC_NONE | G_ZS_PIXEL | Z_CMP | G_RM_FOG_SHADE_A | G_RM_AA_OPA_SURF2), // Z-Compare only

    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, sLightsOutBoardTopTex),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, G_TX_LOADTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK,
                G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 1023, 256),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0x000 / 8, G_TX_RENDERTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD,
                G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD),
    gsDPSetTileSize(G_TX_RENDERTILE, 122, 122, 31 << G_TEXTURE_IMAGE_FRAC, 31 << G_TEXTURE_IMAGE_FRAC),

    gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),

    // OVERLAY

    gsDPPipeSync(),
    gsDPSetOtherMode(G_AD_NOISE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_RGBA16 | G_TL_TILE |
                         G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_NPRIMITIVE,
                     G_AC_NONE | G_ZS_PIXEL | AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | FORCE_BL | ZMODE_XLU |
                         GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA) |
                         GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)),
    gsDPSetCombineLERP(0, 0, 0, TEXEL0, 0, 0, 0, PRIMITIVE, 0, 0, 0, TEXEL0, 0, 0, 0, PRIMITIVE),

    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, 0x09000000),
    gsDPSetTile(0, 0, 0, 0x800 / 8, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR,
                G_TX_NOMASK, G_TX_NOLOD),
    gsDPLoadTLUTCmd(5, LIGHTSOUT_NUM_STATES - 1),

#define TX_LINE(width, siz) ((((width) * (siz)) + 7) >> 3)

    gsDPSetTextureImage(G_IM_FMT_CI, G_IM_SIZ_8b, LIGHTSOUT_GRID_X, 0x08000000),
    gsDPSetTile(G_IM_FMT_CI, G_IM_SIZ_8b, TX_LINE(LIGHTSOUT_GRID_X, G_IM_SIZ_8b_TILE_BYTES), 0x000 / 8, G_TX_LOADTILE,
                0, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK,
                G_TX_NOLOD),
    gsDPLoadTile(G_TX_LOADTILE, 0, 0, LIGHTSOUT_GRID_X << G_TEXTURE_IMAGE_FRAC,
                 LIGHTSOUT_GRID_Y << G_TEXTURE_IMAGE_FRAC),
    gsDPSetTile(G_IM_FMT_CI, G_IM_SIZ_8b, TX_LINE(LIGHTSOUT_GRID_X, G_IM_SIZ_8b_LINE_BYTES), 0x000 / 8, G_TX_RENDERTILE,
                0, G_TX_CLAMP | G_TX_NOMIRROR, G_TX_NOMASK, TC_OVL_SFT_X, G_TX_CLAMP | G_TX_NOMIRROR, G_TX_NOMASK,
                TC_OVL_SFT_Y),
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, LIGHTSOUT_GRID_X << G_TEXTURE_IMAGE_FRAC,
                    LIGHTSOUT_GRID_Y << G_TEXTURE_IMAGE_FRAC),

    gsSP2Triangles(16, 17, 18, 0, 16, 18, 19, 0),

    gsSPEndDisplayList(),
};
