#include "snprintf.h"

typedef struct {
    char* dst;    // current dst
    char* dstEnd; // end dst
} NPrintf;

static void* proutNSprintf(void* arg, const char* str, size_t size) {
    NPrintf* np = (NPrintf*)arg;

    if (np->dst >= np->dstEnd || np->dst == NULL) {
        // reached or passed the end, don't write out but continue
        // in order to get the full string length
        return np;
    }
    // ensure size does not end up out of bounds
    size = MIN(size, (u32)(np->dstEnd - np->dst));
    np->dst = (char*)memcpy(np->dst, str, size) + size;

    return np;
}

int vsnprintf(char* dst, size_t size, const char* fmt, va_list args) {
    NPrintf np;
    int len;

	// dst can be null
    np.dst = dst;
    // size can be zero
    np.dstEnd = &dst[(size == 0) ? 0 : (size - 1)];

    len = _Printf(proutNSprintf, &np, fmt, args);
    if (dst != NULL && size > 0 && len > -1) {
        // len returned by vsnprintf may not always be in the buffer, 
        // so terminate at min of the two
        dst[MIN((size_t)len, size - 1)] = '\0';
    }
    return len;
}

int snprintf(char* dst, size_t size, const char* fmt, ...) {
    NPrintf np;
    int len;
    va_list args;
    va_start(args, fmt);

    np.dst = dst;
    np.dstEnd = &dst[(size == 0) ? 0 : (size - 1)];

    len = _Printf(proutNSprintf, &np, fmt, args);
    if (dst != NULL && size > 0 && len > -1) {
        dst[MIN((size_t)len, size - 1)] = '\0';
    }
    va_end(args);
    return len;
}
