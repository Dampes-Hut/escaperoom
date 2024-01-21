#include "global.h"
#include "terminal.h"

#define FILL_ALLOCBLOCK (1 << 0)
#define FILL_FREEBLOCK (1 << 1)
#define CHECK_FREE_BLOCK (1 << 2)

#define NODE_MAGIC (0x7373)

#define BLOCK_UNINIT_MAGIC (0xAB)
#define BLOCK_UNINIT_MAGIC_32 (0xABABABAB)
#define BLOCK_ALLOC_MAGIC (0xCD)
#define BLOCK_ALLOC_MAGIC_32 (0xCDCDCDCD)
#define BLOCK_FREE_MAGIC (0xEF)
#define BLOCK_FREE_MAGIC_32 (0xEFEFEFEF)

u32 __osMalloc_FreeBlockTest_Enable;

static u32 ArenaImpl_GetFillAllocBlock(Arena* arena) {
    return (arena->flag & FILL_ALLOCBLOCK) != 0;
}
static u32 ArenaImpl_GetFillFreeBlock(Arena* arena) {
    return (arena->flag & FILL_FREEBLOCK) != 0;
}
static u32 ArenaImpl_GetCheckFreeBlock(Arena* arena) {
    return (arena->flag & CHECK_FREE_BLOCK) != 0;
}

static void ArenaImpl_SetFillAllocBlock(Arena* arena) {
    arena->flag |= FILL_ALLOCBLOCK;
}
static void ArenaImpl_SetFillFreeBlock(Arena* arena) {
    arena->flag |= FILL_FREEBLOCK;
}
static void ArenaImpl_SetCheckFreeBlock(Arena* arena) {
    arena->flag |= CHECK_FREE_BLOCK;
}

static void ArenaImpl_UnsetFillAllocBlock(Arena* arena) {
    arena->flag &= ~FILL_ALLOCBLOCK;
}
static void ArenaImpl_UnsetFillFreeBlock(Arena* arena) {
    arena->flag &= ~FILL_FREEBLOCK;
}
static void ArenaImpl_UnsetCheckFreeBlock(Arena* arena) {
    arena->flag &= ~CHECK_FREE_BLOCK;
}

static OSMesg sArenaLockMsg;

static void ArenaImpl_LockInit(Arena* arena) {
    osCreateMesgQueue(&arena->lockQueue, &sArenaLockMsg, 1);
}

static void ArenaImpl_Lock(Arena* arena) {
    osSendMesg(&arena->lockQueue, NULL, OS_MESG_BLOCK);
}

static void ArenaImpl_Unlock(Arena* arena) {
    osRecvMesg(&arena->lockQueue, NULL, OS_MESG_BLOCK);
}

static ArenaNode* ArenaImpl_GetNextBlock(ArenaNode* node) {
    ArenaNode* next = node->next;

#ifndef NDEBUG
    if (next != NULL && next->magic != NODE_MAGIC) {
        rmonPrintf(VT_COL(RED, WHITE) "[__osMalloc] Corrupt next heap block! @ %08X\n" VT_RST, next);
        next = NULL;
        node->next = NULL;
    }
#endif
    return next;
}

static ArenaNode* ArenaImpl_GetPrevBlock(ArenaNode* node) {
    ArenaNode* prev = node->prev;

#ifndef NDEBUG
    if (prev != NULL && prev->magic != NODE_MAGIC) {
        rmonPrintf(VT_COL(RED, WHITE) "[__osMalloc] Corrupt next heap block! @ %08X\n" VT_RST, prev);
        prev = NULL;
        node->prev = NULL;
    }
#endif
    return prev;
}

static ArenaNode* ArenaImpl_GetLastBlock(Arena* arena) {
    ArenaNode* last = NULL;
    ArenaNode* iter;

    if (arena != NULL && arena->head != NULL && arena->head->magic == NODE_MAGIC) {
        iter = arena->head;
        while (iter != NULL) {
            last = iter;
            iter = ArenaImpl_GetNextBlock(iter);
        }
    }
    return last;
}

void __osMallocInit(Arena* arena, void* start, u32 size) {
    bzero(arena, sizeof(Arena));

    ArenaImpl_LockInit(arena);

    if (start != NULL) {
        s32 diff;
        s32 size2;
        ArenaNode* firstNode;
        ArenaNode* lastNode;

        firstNode = (ArenaNode*)ALIGN16((u32)start);
        diff = (s32)firstNode - (s32)start;
        size2 = (size - diff) & ~0xF;

        if (size2 > (s32)sizeof(ArenaNode)) {
            __osMemset(firstNode, BLOCK_UNINIT_MAGIC, size2);
            firstNode->next = NULL;
            firstNode->prev = NULL;
            firstNode->size = size2 - sizeof(ArenaNode);
            firstNode->isFree = true;
            firstNode->magic = NODE_MAGIC;
            arena->head = arena->tail = firstNode;
            arena->start = start;
        }
    }

    arena->isInit = true;
}

void __osMallocCleanup(Arena* arena) {
    ArenaNode* iter;
    ArenaNode* next;

    ArenaImpl_Lock(arena);

    iter = arena->head;
    while (iter != NULL) {
        next = ArenaImpl_GetNextBlock(iter);
#ifndef NDEBUG
        __osMemset(iter, BLOCK_UNINIT_MAGIC, iter->size + sizeof(ArenaNode));
#endif
        iter = next;
    }

    ArenaImpl_Unlock(arena);

    bzero(arena, sizeof(Arena));
}

void __osMalloc_FreeBlockTest(Arena* arena, ArenaNode* node) {
    ArenaNode* node2 = node;
    u32* start;
    u32* end;
    u32* iter;

    if (__osMalloc_FreeBlockTest_Enable) {
        start = (u32*)((u32)node + sizeof(ArenaNode));
        end = (u32*)((u32)start + node2->size);
        iter = start;

        while (iter < end) {
            if (*iter != BLOCK_UNINIT_MAGIC_32 && *iter != BLOCK_FREE_MAGIC_32) {
                osSyncPrintf(
                    VT_COL(RED, WHITE) "緊急事態！メモリリーク検出！ (block=%08x s=%08x e=%08x p=%08x)\n" VT_RST, node,
                    start, end, iter);
                __osDisplayArena(arena);
                return;
            }
            iter++;
        }
    }
}

void* __osMalloc_NoLock(Arena* arena, u32 size FILE_LINE_DECL) {
    ArenaNode* iter;
    u32 blockSize;
    ArenaNode* newNode;
    void* alloc = NULL;
    ArenaNode* next;

    iter = arena->head;
    size = ALIGN16(size);
    blockSize = ALIGN16(size) + sizeof(ArenaNode);

    while (iter != NULL) {

        if (iter->isFree && iter->size >= size) {
            if (arena->flag & CHECK_FREE_BLOCK) {
                __osMalloc_FreeBlockTest(arena, iter);
            }

            if (blockSize < iter->size) {
                newNode = (ArenaNode*)((u32)iter + blockSize);
                newNode->next = ArenaImpl_GetNextBlock(iter);
                newNode->prev = iter;
                newNode->size = iter->size - blockSize;
                newNode->isFree = true;
                newNode->magic = NODE_MAGIC;

                iter->next = newNode;
                iter->size = size;
                next = ArenaImpl_GetNextBlock(newNode);
                if (next) {
                    next->prev = newNode;
                }
            }

            iter->isFree = false;
#ifndef NDEBUG
            iter->filename = file;
            iter->line = line;
            iter->threadId = osGetThreadId(NULL);
            iter->arena = arena;
            iter->time = osGetTime();
#endif
            alloc = (void*)((u32)iter + sizeof(ArenaNode));
            if (arena->flag & FILL_ALLOCBLOCK) {
                __osMemset(alloc, BLOCK_ALLOC_MAGIC, size);
            }
            break;
        }

        iter = ArenaImpl_GetNextBlock(iter);
    }

    return alloc;
}

void* __osMalloc(Arena* arena, u32 size FILE_LINE_DECL) {
    void* alloc;

    ArenaImpl_Lock(arena);
    alloc = __osMalloc_NoLock(arena, size FILE_LINE_ARGS);
    ArenaImpl_Unlock(arena);

    return alloc;
}

void* __osMallocR(Arena* arena, u32 size FILE_LINE_DECL) {
    ArenaNode* iter;
    ArenaNode* newNode;
    u32 blockSize;
    ArenaNode* next;
    void* alloc = NULL;

    size = ALIGN16(size);
    ArenaImpl_Lock(arena);
    iter = ArenaImpl_GetLastBlock(arena);

    while (iter != NULL) {
        if (iter->isFree && iter->size >= size) {
            if (arena->flag & CHECK_FREE_BLOCK) {
                __osMalloc_FreeBlockTest(arena, iter);
            }

            blockSize = ALIGN16(size) + sizeof(ArenaNode);
            if (blockSize < iter->size) {
                newNode = (ArenaNode*)((u32)iter + (iter->size - size));
                newNode->next = ArenaImpl_GetNextBlock(iter);
                newNode->prev = iter;
                newNode->size = size;
                newNode->magic = NODE_MAGIC;

                iter->next = newNode;
                iter->size -= blockSize;
                next = ArenaImpl_GetNextBlock(newNode);
                if (next) {
                    next->prev = newNode;
                }
                iter = newNode;
            }

            iter->isFree = false;
#ifndef NDEBUG
            iter->filename = file;
            iter->line = line;
            iter->threadId = osGetThreadId(NULL);
            iter->arena = arena;
            iter->time = osGetTime();
#endif
            alloc = (void*)((u32)iter + sizeof(ArenaNode));
            if (arena->flag & FILL_ALLOCBLOCK) {
                __osMemset(alloc, BLOCK_ALLOC_MAGIC, size);
            }
            break;
        }
        iter = ArenaImpl_GetPrevBlock(iter);
    }
    ArenaImpl_Unlock(arena);

    return alloc;
}

void __osFree(Arena* arena, void* ptr FILE_LINE_DECL) {
    ArenaNode* node;
    ArenaNode* next;
    ArenaNode* prev;
    ArenaNode* newNext;

    if (ptr == NULL) {
        return;
    }

    ArenaImpl_Lock(arena);

    node = (ArenaNode*)((u32)ptr - sizeof(ArenaNode));
    if (node == NULL || node->magic != NODE_MAGIC) {
        // "__osFree: Unauthorized release (%08x)"
        osSyncPrintf(VT_COL(RED, WHITE) "__osFree:不正解放(%08x)\n" VT_RST, ptr);
        goto exit;
    }
    if (node->isFree) {
        osSyncPrintf(VT_COL(RED, WHITE) "__osFree:二重解放(%08x)\n" VT_RST, ptr); // "__osFree: Double release (%08x)"
        goto exit;
    }
#ifndef NDEBUG
    if (arena != node->arena && arena != NULL) {
        // "__osFree:Tried to release in a different way than when it was secured (%08x:%08x)"
        osSyncPrintf(VT_COL(RED, WHITE) "__osFree:確保時と違う方法で解放しようとした (%08x:%08x)\n" VT_RST, arena,
                     node->arena);
        goto exit;
    }
#endif

    next = ArenaImpl_GetNextBlock(node);
    prev = ArenaImpl_GetPrevBlock(node);
#ifndef NDEBUG
    node->isFree = true;
    node->filename = file;
    node->line = line;
    node->threadId = osGetThreadId(NULL);
    node->arena = arena;
    node->time = osGetTime();
#endif

    if (arena->flag & FILL_FREEBLOCK) {
        __osMemset((void*)((u32)node + sizeof(ArenaNode)), BLOCK_FREE_MAGIC, node->size);
    }

    newNext = next;
    if ((u32)next == (u32)node + sizeof(ArenaNode) + node->size && next->isFree) {
        newNext = ArenaImpl_GetNextBlock(next);
        if (newNext != NULL) {
            newNext->prev = node;
        }

        node->size += next->size + sizeof(ArenaNode);
        if (arena->flag & FILL_FREEBLOCK) {
            __osMemset(next, BLOCK_FREE_MAGIC, sizeof(ArenaNode));
        }
        node->next = newNext;
        next = newNext;
    }

    if (prev != NULL && prev->isFree && (u32)node == (u32)prev + sizeof(ArenaNode) + prev->size) {
        if (next) {
            next->prev = prev;
        }
        prev->next = next;
        prev->size += node->size + sizeof(ArenaNode);
        if (arena->flag & FILL_FREEBLOCK) {
            __osMemset(node, BLOCK_FREE_MAGIC, sizeof(ArenaNode));
        }
    }

exit:
    ArenaImpl_Unlock(arena);
}

void* __osRealloc(Arena* arena, void* ptr, u32 newSize FILE_LINE_DECL) {
    ArenaNode* node;
    ArenaNode* next;
    ArenaNode* newNext;
    ArenaNode* overNext;

    newSize = ALIGN16(newSize);
    osSyncPrintf("__osRealloc(%08x, %d)\n", ptr, newSize);

    if (ptr == NULL) {
        return __osMalloc(arena, newSize FILE_LINE_ARGS);
    }

    if (newSize == 0) {
        __osFree(arena, ptr FILE_LINE_ARGS);
        return NULL;
    }

    // Get arena node for this memory
    node = (ArenaNode*)((u32)ptr - sizeof(ArenaNode));

#ifndef NDEBUG
    if (node->magic != NODE_MAGIC) {
        rmonPrintf("[__osMalloc] Corrupt node in realloc\n");
        return NULL;
    }
#endif

    if (newSize == node->size) {
        // Size did not change, nothing to do
        return ptr;
    }

    if (node->size < newSize) {
        u32 sizeDiff;
        // Realloc size is larger than current block size, need to move.

        // Lock the arena while we mess with nodes
        ArenaImpl_Lock(arena);

        next = ArenaImpl_GetNextBlock(node);
        sizeDiff = newSize - node->size;

        if ((u32)next == ((u32)node + node->size + sizeof(ArenaNode)) && next->isFree && next->size >= sizeDiff) {
            // The next block is free, merge with this block and create a new node
            next->size -= sizeDiff;
            newNext = (ArenaNode*)((u32)next + sizeDiff);

            // Update the next-next block with the new next block, if applicable
            overNext = ArenaImpl_GetNextBlock(next);
            if (overNext != NULL) {
                overNext->prev = newNext;
            }

            // Update current block
            node->next = newNext;
            node->size = newSize;

            // Copy next block to new next bock
            *newNext = *next;

            // Unlock, we're done
            ArenaImpl_Unlock(arena);
        } else {
            void* newAlloc;
            // Unlock for the moment, will re-lock shortly
            ArenaImpl_Unlock(arena);

            // Allocate totally new block and move current memory contents into that block

            // Make the new allocation
            newAlloc = __osMalloc(arena, newSize FILE_LINE_ARGS);
            if (newAlloc != NULL) {
                // Copy the data
                bcopy(ptr, newAlloc, node->size);

                // Free the old block
                __osFree(arena, ptr FILE_LINE_ARGS);
            }
            ptr = newAlloc;
        }
    } else if (newSize < node->size) {
        // New size is less than current block size, no need to move.

        ArenaImpl_Lock(arena);

        next = ArenaImpl_GetNextBlock(node);
        if (next != NULL && next->isFree) {
            // The next block is free, enlarge the next block to fill the gap
            ArenaNode localCopy;

            newNext = (ArenaNode*)((u32)node + ALIGN16(newSize) + sizeof(ArenaNode));

            // First copy to stack then to destination since we want to copy in reverse (XXX could call memmove instead)
            localCopy = *next;
            *newNext = localCopy;
            // Update size after the copy
            newNext->size += node->size - newSize;

            // Update current block
            node->next = newNext;
            node->size = newSize;

            // Update next-next node, if applicable
            overNext = ArenaImpl_GetNextBlock(newNext);
            if (overNext != NULL) {
                overNext->prev = newNext;
            }
        } else if (newSize + sizeof(ArenaNode) < node->size) {
            // The next block is occupied (or doesn't exist?), add a completely new block to fill the gap
            u32 blockSize = ALIGN16(newSize) + sizeof(ArenaNode);
            newNext = (ArenaNode*)((u32)node + blockSize);

            // Create a new next block
            newNext->next = ArenaImpl_GetNextBlock(node);
            newNext->prev = node;
            newNext->size = node->size - blockSize;
            newNext->isFree = true;
            newNext->magic = NODE_MAGIC;

            // Update current block
            node->next = newNext;
            node->size = newSize;

            // Update next-next node, if applicable
            overNext = ArenaImpl_GetNextBlock(newNext);
            if (overNext != NULL) {
                overNext->prev = newNext;
            }
        } else {
            // "There is no room to generate free blocks"
            rmonPrintf("[__osMalloc] No room for new blocks?\n");
            ptr = NULL;
        }

        ArenaImpl_Unlock(arena);
    }

    return ptr;
}

void ArenaImpl_GetSizes(Arena* arena, u32* outMaxFree, u32* outFree, u32* outAlloc) {
    ArenaNode* iter;

    ArenaImpl_Lock(arena);

    *outMaxFree = 0;
    *outFree = 0;
    *outAlloc = 0;

    iter = arena->head;
    while (iter != NULL) {
        if (iter->isFree) {
            *outFree += iter->size;
            if (*outMaxFree < iter->size) {
                *outMaxFree = iter->size;
            }
        } else {
            *outAlloc += iter->size;
        }

        iter = ArenaImpl_GetNextBlock(iter);
    }

    ArenaImpl_Unlock(arena);
}

void __osDisplayArena(Arena* arena) {
    u32 freeSize;
    u32 allocatedSize;
    u32 maxFree;
    ArenaNode* iter;
    ArenaNode* next;

    if (!__osMallocIsInitialized(arena)) {
        rmonPrintf("[__osMalloc] Tried to call __osDisplayArena on uninitialized arena %08X!\n", arena);
        return;
    }

    rmonPrintf("\n[__osMalloc] Arena contents for %08X\n", arena);
    rmonPrintf("Heap nodes list | start-end:status size [time:TID:file:line]\n");

    maxFree = 0;
    freeSize = 0;
    allocatedSize = 0;

    ArenaImpl_Lock(arena);

    iter = arena->head;
    while (iter != NULL) {
        if (iter->magic != NODE_MAGIC) {
            rmonPrintf("[__osMalloc] Block @ %08X is invalid!\n", iter);
            break;
        }

        uintptr_t end = (uintptr_t)iter + sizeof(ArenaNode) + iter->size;

        char marker = ' ';
        if (iter->next == NULL) {
            marker = '$';
        } else if (iter != iter->next->prev) {
            marker = '!';
        }

        const char* freeMarker = iter->isFree ? "FREE" : "USED";

#ifndef NDEBUG
        rmonPrintf("%08X-%08X:%c %s %08X", (uintptr_t)iter, end, marker, freeMarker, iter->size);
        if (!iter->isFree) {
            const char* fileName = iter->filename != NULL ? iter->filename : "**NULL**";
            rmonPrintf(" [%016llu:%2d:%s:%d]", OS_CYCLES_TO_NSEC(iter->time), iter->threadId, fileName, iter->line);
        }
        rmonPrintf("\n");
#else
        rmonPrintf("%08X-%08X:%c %s %08X\n", (uintptr_t)iter, end, marker, freeMarker, iter->size);
#endif

        if (iter->isFree) {
            freeSize += iter->size;
            if (maxFree < iter->size) {
                maxFree = iter->size;
            }
        } else {
            allocatedSize += iter->size;
        }

        iter = iter->next;
    }

    ArenaImpl_Unlock(arena);

    rmonPrintf("[__osMalloc] Total reserved space: %08X\n", allocatedSize);
    rmonPrintf("[__osMalloc] Total free space: %08X\n", freeSize);
    rmonPrintf("[__osMalloc] Largest free block size: %08X\n\n", maxFree);
}

void ArenaImpl_FaultClient(Arena* arena) {
    u32 freeSize;
    u32 allocatedSize;
    u32 maxFree;
    ArenaNode* iter;
    ArenaNode* next;

    FaultDrawer_Printf("ARENA INFO (0x%08x)\n", arena);
    if (!__osMallocIsInitialized(arena)) {
        FaultDrawer_Printf("Arena is uninitalized\n", arena);
        return;
    }

    maxFree = 0;
    freeSize = 0;
    allocatedSize = 0;

    FaultDrawer_Printf("Memory Block Region status size\n");

    iter = arena->head;
    while (iter != NULL) {
        if (iter != NULL && iter->magic == NODE_MAGIC) {
            next = iter->next;
            FaultDrawer_Printf("%08x-%08x%c %s %08x", iter, ((u32)iter + sizeof(ArenaNode) + iter->size),
                               (!next) ? '$' : (iter != next->prev ? '!' : ' '), iter->isFree ? "F" : "A", iter->size);

            FaultDrawer_Printf("\n");

            if (iter->isFree) {
                freeSize += iter->size;
                if (maxFree < iter->size) {
                    maxFree = iter->size;
                }
            } else {
                allocatedSize += iter->size;
            }
        } else {
            FaultDrawer_SetFontColor(0xF801);
            FaultDrawer_Printf("%08x Block Invalid\n", iter);
            next = NULL;
        }
        iter = next;
    }

    FaultDrawer_SetFontColor(0x7F1);
    FaultDrawer_Printf("Total Alloc Block Size  %08x\n", allocatedSize);
    FaultDrawer_Printf("Total Free Block Size   %08x\n", freeSize);
    FaultDrawer_Printf("Largest Free Block Size %08x\n", maxFree);
}

u32 __osCheckArena(Arena* arena) {
    ArenaNode* iter;
    u32 error = 0;

    ArenaImpl_Lock(arena);
    // "Checking the contents of the arena. . ． (%08x)"
    osSyncPrintf("アリーナの内容をチェックしています．．． (%08x)\n", arena);
    iter = arena->head;
    while (iter != NULL) {
        if (iter && iter->magic == NODE_MAGIC) {
            // "Oops!! (%08x %08x)"
            osSyncPrintf(VT_COL(RED, WHITE) "おおっと！！ (%08x %08x)\n" VT_RST, iter, iter->magic);
            error = 1;
            break;
        }
        iter = ArenaImpl_GetNextBlock(iter);
    }
    if (error == 0) {
        osSyncPrintf("アリーナはまだ、いけそうです\n"); // "The arena is still going well"
    }
    ArenaImpl_Unlock(arena);

    return error;
}

u8 func_800FF334(Arena* arena) {
    return arena->unk_20;
}
