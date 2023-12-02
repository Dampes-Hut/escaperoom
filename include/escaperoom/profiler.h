#ifndef PROFILER_H
#define PROFILER_H

#include "functions.h"

#define PROF_RINGBUFFER_LEN 20

typedef struct {
    OSTime buffer[PROF_RINGBUFFER_LEN];
    u32 bufferIndex;
    OSTime start;
    OSTime time;
} Profiler;

extern u32 gProfilerEnabled;

extern Profiler gPlayUpdateProfiler;
extern Profiler gCollisionCheckProfiler;
extern Profiler gActorProfiler;
extern Profiler gEnvironmentProfiler;
extern Profiler gCameraUpdateProfiler;

extern Profiler gPlayDrawProfiler;
extern Profiler gSceneRoomDrawProfiler;
extern Profiler gActorDrawProfiler;
extern Profiler gOverlayElementsDrawProfiler;

#ifndef NDEBUG
static inline __attribute__((always_inline))
void Profiler_Start(Profiler* prof) {
    prof->start = osGetTime();
}

static inline __attribute__((always_inline))
void Profiler_End(Profiler* prof) {
    prof->time = osGetTime() - prof->start;
}
#else
#define Profiler_Start(prof)
#define Profiler_End(prof)
#endif

#endif
