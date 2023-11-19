#include "global.h"

Gfx sFillSetupDL[] = {
    gsDPPipeSync(),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsDPSetOtherMode(G_AD_DISABLE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE |
                         G_TD_CLAMP | G_TP_PERSP | G_CYC_FILL | G_PM_NPRIMITIVE,
                     G_AC_NONE | G_ZS_PIXEL | G_RM_NOOP | G_RM_NOOP2),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH),
    gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT),
    gsDPSetBlendColor(0, 0, 0, 8),
    gsSPClipRatio(FRUSTRATIO_2),
    gsSPEndDisplayList(),
};

/**
 * Sets up the frame for drawing.
 * Initializes the scissor region to full screen.
 * Set up the framebuffer and z-buffer.
 * The whole screen is filled with the color supplied as arguments.
 * Letterbox is also applied here, and will share the color of the screen base.
 */
void Gfx_SetupFrame(GraphicsContext* gfxCtx, s32 clearFB, u8 r, u8 g, u8 b) {
    OPEN_DISPS(gfxCtx, "../z_rcp.c", 2386);

    // Set up the RDP render state for rectangles in FILL mode
    gSPDisplayList(POLY_OPA_DISP++, sFillSetupDL);
    gSPDisplayList(POLY_XLU_DISP++, sFillSetupDL);
    gSPDisplayList(OVERLAY_DISP++, sFillSetupDL);

    // Set the scissor region to the full screen
    gDPSetScissor(POLY_OPA_DISP++, G_SC_NON_INTERLACE, 0, 0, gScreenWidth, gScreenHeight);
    gDPSetScissor(POLY_XLU_DISP++, G_SC_NON_INTERLACE, 0, 0, gScreenWidth, gScreenHeight);
    gDPSetScissor(OVERLAY_DISP++, G_SC_NON_INTERLACE, 0, 0, gScreenWidth, gScreenHeight);

    // Set up the framebuffer, primitives will be drawn here
    gDPSetColorImage(POLY_OPA_DISP++, G_IM_FMT_RGBA, G_IM_SIZ_16b, gScreenWidth, gfxCtx->curFrameBuffer);
    gDPSetColorImage(POLY_OPA_DISP++, G_IM_FMT_RGBA, G_IM_SIZ_16b, gScreenWidth, gfxCtx->curFrameBuffer);
    gDPSetColorImage(POLY_XLU_DISP++, G_IM_FMT_RGBA, G_IM_SIZ_16b, gScreenWidth, gfxCtx->curFrameBuffer);
    gDPSetColorImage(OVERLAY_DISP++, G_IM_FMT_RGBA, G_IM_SIZ_16b, gScreenWidth, gfxCtx->curFrameBuffer);

    // Set up the z-buffer
    gDPSetDepthImage(POLY_OPA_DISP++, gZBuffer);
    gDPSetDepthImage(POLY_XLU_DISP++, gZBuffer);
    gDPSetDepthImage(OVERLAY_DISP++, gZBuffer);

    if ((R_PAUSE_BG_PRERENDER_STATE <= PAUSE_BG_PRERENDER_SETUP) && (gTransitionTileState <= TRANS_TILE_SETUP)) {
        s32 letterboxSize = Letterbox_GetSize();

        if (R_HREG_MODE == HREG_MODE_SETUP_FRAME) {
            if (R_SETUP_FRAME_INIT != HREG_MODE_SETUP_FRAME) {
                R_SETUP_FRAME_GET = (SETUP_FRAME_LETTERBOX_SIZE_FLAG | SETUP_FRAME_BASE_COLOR_FLAG);
                R_SETUP_FRAME_SET = (SETUP_FRAME_LETTERBOX_SIZE_FLAG | SETUP_FRAME_BASE_COLOR_FLAG);
                R_SETUP_FRAME_LETTERBOX_SIZE = 0;
                R_SETUP_FRAME_BASE_COLOR_R = 0;
                R_SETUP_FRAME_BASE_COLOR_G = 0;
                R_SETUP_FRAME_BASE_COLOR_B = 0;

                // these regs are not used in this mode
                HREG(87) = 0;
                HREG(88) = 0;
                HREG(89) = 0;
                HREG(90) = 0;
                HREG(91) = 0;
                HREG(92) = 0;
                HREG(93) = 0;
                HREG(94) = 0;

                R_SETUP_FRAME_INIT = HREG_MODE_SETUP_FRAME;
            }

            if (R_SETUP_FRAME_GET & SETUP_FRAME_LETTERBOX_SIZE_FLAG) {
                R_SETUP_FRAME_LETTERBOX_SIZE = letterboxSize;
            }

            if (R_SETUP_FRAME_GET & SETUP_FRAME_BASE_COLOR_FLAG) {
                R_SETUP_FRAME_BASE_COLOR_R = r;
                R_SETUP_FRAME_BASE_COLOR_G = g;
                R_SETUP_FRAME_BASE_COLOR_B = b;
            }

            if (R_SETUP_FRAME_SET & SETUP_FRAME_LETTERBOX_SIZE_FLAG) {
                letterboxSize = R_SETUP_FRAME_LETTERBOX_SIZE;
            }

            if (R_SETUP_FRAME_SET & SETUP_FRAME_BASE_COLOR_FLAG) {
                r = R_SETUP_FRAME_BASE_COLOR_R;
                g = R_SETUP_FRAME_BASE_COLOR_G;
                b = R_SETUP_FRAME_BASE_COLOR_B;
            }
        }

        // Set the whole z buffer to maximum depth
        // Skip pixels that are being covered by the letterbox
        gDPSetColorImage(POLY_OPA_DISP++, G_IM_FMT_RGBA, G_IM_SIZ_16b, gScreenWidth, gZBuffer);
        gDPSetFillColor(POLY_OPA_DISP++, (GPACK_ZDZ(G_MAXFBZ, 0) << 16) | GPACK_ZDZ(G_MAXFBZ, 0));
        gDPFillRectangle(POLY_OPA_DISP++, 0, letterboxSize, gScreenWidth - 1, gScreenHeight - letterboxSize - 1);
        gDPPipeSync(POLY_OPA_DISP++);

        gDPSetColorImage(POLY_OPA_DISP++, G_IM_FMT_RGBA, G_IM_SIZ_16b, gScreenWidth, gfxCtx->curFrameBuffer);
        if (clearFB) {
            // Fill the whole screen with the base color, only done when there is no skybox or it is a solid color
            // Skip pixels that are being covered by the letterbox
            gDPSetFillColor(POLY_OPA_DISP++, (GPACK_RGBA5551(r, g, b, 1) << 16) | GPACK_RGBA5551(r, g, b, 1));
            gDPFillRectangle(POLY_OPA_DISP++, 0, letterboxSize, gScreenWidth - 1, gScreenHeight - letterboxSize - 1);
            gDPPipeSync(POLY_OPA_DISP++);
        }

        // Set new scissor bounds to exclude the letterbox region except on overlay, since nothing should draw over the
        // scissor region except overlay drawing.
        gDPSetScissor(POLY_OPA_DISP++, G_SC_NON_INTERLACE, 0, letterboxSize, gScreenWidth, gScreenHeight - letterboxSize);
        gDPSetScissor(POLY_XLU_DISP++, G_SC_NON_INTERLACE, 0, letterboxSize, gScreenWidth, gScreenHeight - letterboxSize);
        gDPSetScissor(OVERLAY_DISP++, G_SC_NON_INTERLACE, 0, 0, gScreenWidth, gScreenHeight);

        // Draw the letterbox if applicable, always black
        if (letterboxSize > 0) {
            gDPSetFillColor(OVERLAY_DISP++, (GPACK_RGBA5551(0, 0, 0, 1) << 16) | GPACK_RGBA5551(0, 0, 0, 1));
            gDPFillRectangle(OVERLAY_DISP++, 0, 0, gScreenWidth - 1, letterboxSize - 1);
            gDPFillRectangle(OVERLAY_DISP++, 0, gScreenHeight - letterboxSize, gScreenWidth - 1, gScreenHeight - 1);
            gDPPipeSync(OVERLAY_DISP++);
        }
    }

    CLOSE_DISPS(gfxCtx, "../z_rcp.c", 2497);
}
