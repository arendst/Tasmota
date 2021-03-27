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

static void* _realloc(void *ptr, size_t old_size, size_t new_size)
{
    if (old_size == new_size) { /* the block unchanged */
        return ptr;
    }
    if (ptr && new_size) { /* realloc block */
        return realloc(ptr, new_size);
    }
    if (new_size) { /* alloc a new block */
        be_assert(ptr == NULL && old_size == 0);
        return malloc(new_size);
    }
    be_assert(new_size == 0);

#if BE_USE_DEBUG_GC
    memset(ptr, 0xFF, old_size); /* fill the structure with invalid pointers */
#endif
    free(ptr);
    return NULL;
}

BERRY_API void* be_realloc(bvm *vm, void *ptr, size_t old_size, size_t new_size)
{
    void *block = _realloc(ptr, old_size, new_size);
    if (!block && new_size) { /* allocation failure */
        vm->gc.status |= GC_ALLOC;
        be_gc_collect(vm); /* try to allocate again after GC */
        vm->gc.status &= ~GC_ALLOC;
        block = _realloc(ptr, old_size, new_size);
        if (!block) { /* lack of heap space */
            be_throw(vm, BE_MALLOC_FAIL);
        }
    }
    vm->gc.usage = vm->gc.usage + new_size - old_size; /* update allocated count */
    return block;
}
