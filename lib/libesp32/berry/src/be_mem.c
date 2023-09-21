/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "be_mem.h"
#include "be_exec.h"
#include "be_vm.h"
#include "be_gc.h"
#include <stdlib.h>
#include <string.h>

#define GC_ALLOC    (1 << 2) /* GC in alloc */

#ifdef BE_EXPLICIT_MALLOC
  #define malloc                BE_EXPLICIT_MALLOC
#endif

#ifdef BE_EXPLICIT_FREE
  #define free                  BE_EXPLICIT_FREE
#endif

#ifdef BE_EXPLICIT_REALLOC
  #define realloc               BE_EXPLICIT_REALLOC
#endif

static void* malloc_from_pool(bvm *vm, size_t size);
static void free_from_pool(bvm *vm, void* ptr, size_t old_size);

#define POOL16_SIZE     16
#define POOL32_SIZE     32

BERRY_API void* be_os_malloc(size_t size)
{
    return malloc(size);
}

BERRY_API void be_os_free(void *ptr)
{
    free(ptr);
}

BERRY_API void* be_os_realloc(void *ptr, size_t size)
{
    return realloc(ptr, size);
}

BERRY_API void* be_realloc(bvm *vm, void *ptr, size_t old_size, size_t new_size)
{
    void *block = NULL;
    // serial_debug("be_realloc ptr=%p old_size=%i new_size=%i\n", ptr, old_size, new_size);

    bbool gc_occured = bfalse;   /* if allocation failed, retry after forced GC */
    if (old_size == new_size) { /* the block unchanged, this also captures creation of a zero byte object */
        return ptr;
    }
    /* from now on, block == NULL means allocation failure */

    while (1) {
        /* Case 1: new allocation */
#if BE_USE_PERF_COUNTERS
        vm->counter_mem_alloc++;
#endif
        if (!ptr || (old_size == 0)) {
            block = malloc_from_pool(vm, new_size);
        }
    
        /* Case 2: deallocate */
        else if (new_size == 0) {
#if BE_USE_PERF_COUNTERS
            vm->counter_mem_free++;
#endif
            if (ptr == NULL) { return NULL; }   /* safeguard */
#if BE_USE_DEBUG_GC
            memset(ptr, 0xFF, old_size); /* fill the structure with invalid pointers */
#endif
            free_from_pool(vm, ptr, old_size);
            break;    /* early exit */
        }

        /* Case 3: reallocate with a different size */
        else if (new_size && old_size) {        // TODO we already know they are not null TODO
#if BE_USE_PERF_COUNTERS
            vm->counter_mem_realloc++;
#endif
            if (new_size <= POOL32_SIZE || old_size <=POOL32_SIZE) {
                /* complex case with different pools */
                if (new_size <= POOL16_SIZE && old_size <= POOL16_SIZE) {
                    // no change of slot
                    block = ptr;
                    break;
                } else if (new_size > POOL16_SIZE && old_size > POOL16_SIZE && new_size <= POOL32_SIZE && old_size <= POOL32_SIZE) {
                    // no change of slot
                    block = ptr;
                    break;
                } else {
                    /* one of the buffer is out of pool, the other not */
                    block = malloc_from_pool(vm, new_size);
                    if (block) {
                        /* copy memory */
                        size_t min_size = old_size < new_size ? old_size : new_size;
                        memmove(block, ptr, min_size);
                        // serial_debug("memmove from %p to %p size=%i\n", ptr, block, min_size);
                        free_from_pool(vm, ptr, old_size);
                    }
                }
            } else {
                block = realloc(ptr, new_size);
                // serial_debug("realloc from %p to %p size=%i", ptr, block, new_size);
            }
        } /* end of reallocation */

        /* exit allocator, do we need to GC ? */
        if (block) { break; }               /* all good */
        if (gc_occured) {                   /* already tried gc, can't do much anymore */
            be_throw(vm, BE_MALLOC_FAIL);   /* lack of heap space */
        }
        // serial_debug("be_realloc force_gc\n");
        /* force GC now */
        vm->gc.status |= GC_ALLOC;
        be_gc_collect(vm); /* try to allocate again after GC */
        vm->gc.status &= ~GC_ALLOC;
        gc_occured = btrue;     /* don't try again GC */
    }
    vm->gc.usage = vm->gc.usage + new_size - old_size; /* update allocated count */

    // serial_debug("be_realloc ret=%p\n", block);
    return block;
}

BERRY_API void* be_move_to_aligned(bvm *vm, void *ptr, size_t size) {
    (void)vm;
    (void)size;
#if BE_USE_MEM_ALIGNED
    if (size <= POOL32_SIZE) {
        return ptr;     /* if in memory pool, don't move it so be_free() will continue to work */
    }
    void* iram = berry_malloc32(size);
    if (iram) {
        memcpy(iram, ptr, size);   /* new_size is always smaller than initial mem zone */
        free(ptr);      // TODO gc size is now wrong
        return iram;
    }
#endif
    return ptr;
}

/* Special allocator for structures under 32 bytes */
typedef uint8_t mem16[16];      /* memory line of 16 bytes */
typedef uint8_t mem32[32];      /* memory line of 32 bytes */
#define POOL16_SLOTS   31
#define POOL16_BITMAP_FULL  ((1UL<<POOL16_SLOTS)-1)      /* 31 bits set to 1 - 0x7FFFFFFF */
#define POOL32_SLOTS   15
#define POOL32_BITMAP_FULL  ((1UL<<POOL32_SLOTS)-1)      /* 15 bits set to 1 - 0x7FFF*/

typedef struct gc16_t {
    uint32_t bitmap;            /* bitmap or allocated lines, 0 is free */
    struct gc16_t*  next;
    mem16    lines[POOL16_SLOTS];
} gc16_t;

typedef struct gc32_t {
    uint32_t bitmap;            /* bitmap or allocated lines, 0 is free */
    struct gc32_t*  next;
    mem32    lines[POOL32_SLOTS];
} gc32_t;

static void* malloc_from_pool(bvm *vm, size_t size) {
    if (size == 0) return NULL;
    if (size <= POOL16_SIZE) {
        /* allocate in pool 16 */
        gc16_t* pool16 = vm->gc.pool16;
        while (pool16)  {
            /* look for an empty slot */
            if (pool16->bitmap != 0x0000) {
                /* there is a free slot */
#ifdef __GNUC__
                int bit = __builtin_ffs(pool16->bitmap) - 1;
#else
                int bit = ffs(pool16->bitmap) - 1;
#endif
                if (bit >= 0) {
                    /* we found a free slot */
                    // bitClear(pool16->bitmap, bit);
                    pool16->bitmap &= ~(1UL << bit);
                    // serial_debug("malloc_from_pool found slot in pool %p, bit %i, ptr=%p\n", pool16, bit, &pool16->lines[bit]);
                    return &pool16->lines[bit];
                }
            }
            pool16 = pool16->next;
        }
        /* no slot available, we allocate a new pool */
        pool16 = (gc16_t*) malloc(sizeof(gc16_t));
        if (!pool16) { return NULL; } /* out of memory */
        pool16->next = vm->gc.pool16;
        pool16->bitmap = POOL16_BITMAP_FULL - 1;      /* allocate first line */
        vm->gc.pool16 = pool16;          /* insert at head of linked list */
        // serial_debug("malloc_from_pool allocated new pool %p, size=%i p=%p\n", pool16, sizeof(gc16_t), &pool16->lines[0]);
        return &pool16->lines[0];
    }

    if (size <= POOL32_SIZE) {
        /* allocate in pool 32 */
        gc32_t* pool32 = vm->gc.pool32;
        while (pool32)  {
            /* look for an empty slot */
            if (pool32->bitmap != 0x0000) {
                /* there is a free slot */
#ifdef __GNUC__
                int bit = __builtin_ffs(pool32->bitmap) - 1;
#else
                int bit = ffs(pool32->bitmap) - 1;
#endif
                if (bit >= 0) {
                    /* we found a free slot */
                    // bitClear(pool32->bitmap, bit);
                    pool32->bitmap &= ~(1UL << bit);
                    // serial_debug("malloc_from_pool found slot in pool %p, bit %i, ptr=%p\n", pool32, bit, &pool32->lines[bit]);
                    return &pool32->lines[bit];
                }
            }
            pool32 = pool32->next;
        }
        /* no slot available, we allocate a new pool */
        pool32 = (gc32_t*) malloc(sizeof(gc32_t));
        if (!pool32) { return NULL; } /* out of memory */
        pool32->next = vm->gc.pool32;
        pool32->bitmap = POOL32_BITMAP_FULL - 1;      /* allocate first line */
        vm->gc.pool32 = pool32;          /* insert at head of linked list */
        // serial_debug("malloc_from_pool allocated new pool %p, size=%i p=%p\n", pool32, sizeof(gc32_t), &pool32->lines[0]);
        return &pool32->lines[0];
    }

    return malloc(size);    /* default to system malloc */
}

static void free_from_pool(bvm *vm, void* ptr, size_t old_size) {
    if (old_size <= POOL16_SIZE) {
        gc16_t* pool16 = vm->gc.pool16;
        while (pool16) {
            int32_t offset = (uint8_t*)ptr - (uint8_t*) &pool16->lines[0];
            // serial_debug("free_from_pool ptr=%p pool=%p offset=%i\n", ptr,pool16, offset);
            if ((offset >= 0) && (offset < POOL16_SLOTS*16) && ((offset & 0x0F) == 0)) {
                int bit = offset >> 4;
                // serial_debug("free_from_pool ptr=%p fond pool=%p bit=%i\n", ptr, pool16, bit);
                // bitSet(pool16->bitmap, bit);
                pool16->bitmap |= 1UL << bit;
                return;
            }
            pool16 = pool16->next;
        }
    }
    else if (old_size <= POOL32_SIZE) {
        gc32_t* pool32 = vm->gc.pool32;
        while (pool32) {
            int32_t offset = (uint8_t*)ptr - (uint8_t*) &pool32->lines[0];
            // serial_debug("free_from_pool pool=%p offset=%i\n", pool32, offset);
            if ((offset >= 0) && (offset < POOL16_SLOTS*16) && ((offset & 0x1F) == 0)) {
                int bit = offset >> 5;
                // serial_debug("free_from_pool ptr=%p fond pool=%p bit=%i\n", ptr, pool32, bit);
                // bitSet(pool32->bitmap, bit);
                pool32->bitmap |= 1UL << bit;
                return;
            }
            pool32 = pool32->next;
        }
    }
    else {
        // serial_debug("free_from_pool free=%p\n", ptr);
        free(ptr);
    }
}

BERRY_API void be_gc_memory_pools(bvm *vm) {
    gc16_t** prev16 = &vm->gc.pool16;
    gc16_t* pool16 = vm->gc.pool16;
    while (pool16) {
        if (pool16->bitmap == POOL16_BITMAP_FULL) {
            /* pool is empty, we can free it */
            *prev16 = pool16->next;
            gc16_t* pool_to_freed = pool16;
            pool16 = pool16->next;  /* move to next */
            free(pool_to_freed);
        } else {
            prev16 = &pool16->next;
            pool16 = pool16->next;  /* move to next */
        }
    }
    gc32_t** prev32 = &vm->gc.pool32;
    gc32_t* pool32 = vm->gc.pool32;
    while (pool32) {
        if (pool32->bitmap == POOL32_BITMAP_FULL) {
            /* pool is empty, we can free it */
            *prev32 = pool32->next;
            gc32_t* pool_to_freed = pool32;
            pool32 = pool32->next;  /* move to next */
            free(pool_to_freed);
        } else {
            prev32 = &pool32->next;
            pool32 = pool32->next;  /* move to next */
        }
    }
}

BERRY_API void be_gc_init_memory_pools(bvm *vm) {
    vm->gc.pool16 = NULL;
    vm->gc.pool32 = NULL;
}

BERRY_API void be_gc_free_memory_pools(bvm *vm) {
    gc16_t* pool16 = vm->gc.pool16;
    while (pool16) {
        gc16_t* pool_to_freed = pool16;
        pool16 = pool16->next;
        be_os_free(pool_to_freed);
    }
    vm->gc.pool16 = NULL;

    gc32_t* pool32 = vm->gc.pool32;
    while (pool32) {
        gc32_t* pool_to_freed = pool32;
        pool32 = pool32->next;
        be_os_free(pool_to_freed);
    }
    vm->gc.pool32 = NULL;
}

/* https://github.com/hcs0/Hackers-Delight/blob/master/pop.c.txt - count number of 1-bits */
static int pop0(uint32_t n) __attribute__((unused));
static int pop0(uint32_t n) {
    n = (n & 0x55555555u) + ((n >> 1) & 0x55555555u);
    n = (n & 0x33333333u) + ((n >> 2) & 0x33333333u);
    n = (n & 0x0f0f0f0fu) + ((n >> 4) & 0x0f0f0f0fu);
    n = (n & 0x00ff00ffu) + ((n >> 8) & 0x00ff00ffu);
    n = (n & 0x0000ffffu) + ((n >>16) & 0x0000ffffu);
    return n;
}

#ifdef __GNUC__
  #define count_bits_1(v)   __builtin_popcount(v)
#else
  #define count_bits_1(v)   pop0(v)
#endif


BERRY_API void be_gc_memory_pools_info(bvm *vm, size_t* slots_used, size_t* slots_allocated) {
    size_t used = 0;
    size_t allocated = 0;

    gc16_t* pool16 = vm->gc.pool16;
    while (pool16) {
        allocated += POOL16_SLOTS;
        used += POOL16_SLOTS - count_bits_1(pool16->bitmap);
        pool16 = pool16->next;
    }

    gc32_t* pool32 = vm->gc.pool32;
    while (pool32) {
        allocated += POOL32_SLOTS;
        used += POOL32_SLOTS - count_bits_1(pool32->bitmap);
        pool32 = pool32->next;
    }
    if (slots_used) { *slots_used = used; }
    if (slots_allocated) { *slots_allocated = allocated; }
}
