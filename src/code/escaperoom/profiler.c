#include "global.h"

extern u32 gRDPPipeTime;
extern u32 gRDPBufTime;
extern u32 gRDPTMEMTime;

u32 gProfilerEnabled = false;

static void Profiler_RingBufferUpdate(Profiler* profiler) {
    u32 cur = profiler->bufferIndex;

    profiler->buffer[cur] = profiler->time;

    u32 next = cur + 1;
    if (next >= PROF_RINGBUFFER_LEN) {
        next = 0;
    }
    profiler->bufferIndex = next;
}

static f32 Profiler_CalcFPS(Profiler* profiler) {
    f32 avg = 0.0f;
    for (u32 i = 0; i < PROF_RINGBUFFER_LEN; i++) {
        avg += (f32)(u32)profiler->buffer[i];
    }
    // <OSTime[PROF_RINGBUFFER_LEN]> to 1/sec
    return (OS_CPU_COUNTER * PROF_RINGBUFFER_LEN) / avg;
}

static f32 Profiler_CalcUsec(Profiler* profiler) {
    f32 avg = 0.0f;
    for (u32 i = 0; i < PROF_RINGBUFFER_LEN; i++) {
        avg += (f32)(u32)profiler->buffer[i];
    }
    // <OSTime[PROF_RINGBUFFER_LEN]> to usec
    return (1000000 * avg) / (OS_CPU_COUNTER * PROF_RINGBUFFER_LEN);
}

#define RCP_CYCLES_TO_USEC(c)   (((u64)(c) * (1000000LL / 15625LL)) / (OS_CLOCK_RATE / 15625LL))

Profiler gPlayUpdateProfiler;

Profiler gCollisionCheckProfiler;
Profiler gActorProfiler;
Profiler gEnvironmentProfiler;
Profiler gCameraUpdateProfiler;

Profiler gPlayDrawProfiler;
Profiler gSceneRoomDrawProfiler;
Profiler gActorDrawProfiler;
Profiler gOverlayElementsDrawProfiler;

void Profiler_UpdateAndDraw(GraphicsContext* gfxCtx) {
    if (!gProfilerEnabled) {
        return;
    }

    static Profiler fpsProfiler;
    static Profiler rspGfxProfiler;
    static Profiler rspAudioProfiler;
    static Profiler rdpTotalProfiler;
    static Profiler cpuAudioThreadTime;
    static Profiler cpuGraphThreadTime;
    static Profiler cpuGraphPeriod;

    static Profiler* sMiscProfilers[] = {
        // FPS
        &fpsProfiler,

        // RCP
        &rspGfxProfiler,
        &rspAudioProfiler,
        &rdpTotalProfiler,

        &cpuAudioThreadTime,
        &cpuGraphThreadTime,
        &cpuGraphPeriod,
    };

    static struct {
        Profiler* prof;
        const char* name;
    } sCpuProfilers[] = {
        { &gPlayUpdateProfiler, "Play_Update"},
        // { &gCollisionCheckProfiler, "Collision Check"},
        { &gActorProfiler, "Actor_UpdateAll"},
        // { &gEnvironmentProfiler, "Environment_Update"},
        // { &gCameraUpdateProfiler, "Camera Updates"},
        { &gPlayDrawProfiler, "Play_Draw"},
        // { &gSceneRoomDrawProfiler, "Scene/Room Draw"},
        { &gActorDrawProfiler, "Actor Drawing"},
        // { &gOverlayElementsDrawProfiler, "Overlay Elements"},
    };

    // FPS times
    Profiler_End(&fpsProfiler);
    Profiler_Start(&fpsProfiler);

    // Scheduler times
    rspGfxProfiler.time = gRSPGfxTimeTotal;
    rspAudioProfiler.time = gRSPAudioTimeTotal;
    rdpTotalProfiler.time = gRDPTimeTotal;

    // Thread times
    cpuAudioThreadTime.time = gAudioThreadUpdateTimeTotalPerGfxTask;
    cpuGraphThreadTime.time = gGfxTaskSentToNextReadyMinusAudioThreadUpdateTime;
    cpuGraphPeriod.time = gGraphUpdatePeriod;

    // Update ringbuffers
    for (u32 i = 0; i < ARRAY_COUNT(sMiscProfilers); i++) {
        Profiler_RingBufferUpdate(sMiscProfilers[i]);
    }
    for (u32 i = 0; i < ARRAY_COUNT(sCpuProfilers); i++) {
        Profiler_RingBufferUpdate(sCpuProfilers[i].prof);
    }

    // Present

    GfxPrint gfxP;
    Gfx* dl;
    Gfx* polyOpaP;

    OPEN_DISPS(gfxCtx);

    dl = Graph_GfxPlusOne(polyOpaP = POLY_OPA_DISP);
    gSPDisplayList(OVERLAY_DISP++, dl);

    GfxPrint_Init(&gfxP);
    GfxPrint_SetBasePosPx(&gfxP, 1 * GFX_CHAR_X_SPACING, 1 * GFX_CHAR_Y_SPACING);
    GfxPrint_SetPos(&gfxP, 0, 0);
    GfxPrint_Open(&gfxP, dl);
    GfxPrint_SetColor(&gfxP, 255, 255, 0, 255);
    gfxP.flags &= ~GFXP_FLAG_SHADOW; // halves the number of gbi commands

    GfxPrint_Printf(&gfxP, "FPS: %.2f\n", Profiler_CalcFPS(&fpsProfiler));

    GfxPrint_Printf(&gfxP, "\n"
                           "\n"
                           "CPU:\n"
                           "    AUDIO PERIOD  = %.2fus\n"
                           "    GRAPH - AUDIO = %.2fus\n"
                           "    GRAPH PERIOD  = %.2fus\n",
                    Profiler_CalcUsec(&cpuAudioThreadTime),
                    Profiler_CalcUsec(&cpuGraphThreadTime),
                    Profiler_CalcUsec(&cpuGraphPeriod));

    for (u32 i = 0; i < ARRAY_COUNT(sCpuProfilers); i++) {
        GfxPrint_Printf(&gfxP, "    %s = %.2fus\n",
                        sCpuProfilers[i].name, Profiler_CalcUsec(sCpuProfilers[i].prof));
    }

    GfxPrint_Printf(&gfxP, "\n"
                           "\n"
                           "RSP:\n"
                           "    GFX   = %.2fus\n"
                           "    AUDIO = %.2fus\n",
                    Profiler_CalcUsec(&rspGfxProfiler),
                    Profiler_CalcUsec(&rspAudioProfiler));

    GfxPrint_Printf(&gfxP, "\n"
                           "\n"
                           "RDP:\n"
                           "    TOTAL = %.2fus\n"
                           "    PIPE  = %lluus\n"
                           "    BUF   = %lluus\n"
                           "    TMEM  = %lluus\n",
                    Profiler_CalcUsec(&rdpTotalProfiler),
                    RCP_CYCLES_TO_USEC(gRDPPipeTime),
                    RCP_CYCLES_TO_USEC(gRDPBufTime),
                    RCP_CYCLES_TO_USEC(gRDPTMEMTime));

    dl = GfxPrint_Close(&gfxP);
    GfxPrint_Destroy(&gfxP);

    gSPEndDisplayList(dl++);
    Graph_BranchDlist(polyOpaP, dl);
    POLY_OPA_DISP = dl;

    CLOSE_DISPS(gfxCtx);
}
