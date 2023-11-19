#ifndef FUNCTIONS2_H
#define FUNCTIONS2_H

#define SysCfb_GetFbPtr(idx) ((idx & 1) ? gFrameBuffer1 : gFrameBuffer0)

void Gfx_SetupFrame(GraphicsContext* gfxCtx, s32 clearFB, u8 r, u8 g, u8 b);

void Skybox_Draw(SkyboxContext* skyboxCtx, GraphicsContext* gfxCtx, LightContext* lightCtx, s16 skyboxId, s16 blend,
                 f32 x, f32 y, f32 z);

#endif
