"""
Berry memory management wrappers.
Mirrors: src/be_mem.c / src/be_mem.h

In the C implementation, be_realloc is the central allocator that tracks
gc.usage and triggers GC when allocation fails. The Python port wraps
Python's native allocation, maintaining the gc.usage counter for GC
threshold logic. Memory pools (pool16/pool32) are skipped — Python's
allocator handles small objects efficiently.
"""

# /********************************************************************
# ** Copyright (c) 2018-2020 Guan Wenliang
# ** This file is part of the Berry default interpreter.
# ** skiars@qq.com, https://github.com/Skiars/berry
# ** See Copyright Notice in the LICENSE file or at
# ** https://github.com/Skiars/berry/blob/master/LICENSE
# ********************************************************************/

from berry_port.be_object import BE_MALLOC_FAIL
from berry_port.berry_conf import BE_USE_DEBUG_GC

# /* GC status flag for "allocation in progress" */
# #define GC_ALLOC    (1 << 2)
GC_ALLOC = (1 << 2)


# ============================================================================
# OS-level allocation wrappers (be_mem.h / be_mem.c)
# ============================================================================

# BERRY_API void* be_os_malloc(size_t size)
# {
#     return malloc(size);
# }
def be_os_malloc(size):
    """Allocate a new block of *size* bytes.

    In C this calls malloc(). In Python we return a bytearray as a
    mutable memory block, or None for zero-size requests.
    """
    if size <= 0:
        return None
    return bytearray(size)


# BERRY_API void be_os_free(void *ptr)
# {
#     free(ptr);
# }
def be_os_free(ptr):
    """Free a previously allocated block.

    In Python, releasing the reference is sufficient — the Python GC
    will reclaim the memory.
    """
    pass  # Python GC handles deallocation


# BERRY_API void* be_os_realloc(void *ptr, size_t size)
# {
#     return realloc(ptr, size);
# }
def be_os_realloc(ptr, size):
    """Reallocate *ptr* to *size* bytes.

    Returns a new bytearray with the content of *ptr* (truncated or
    zero-extended to *size*).
    """
    if ptr is None:
        return be_os_malloc(size)
    if size <= 0:
        return None
    old_len = len(ptr) if ptr is not None else 0
    new_buf = bytearray(size)
    copy_len = min(old_len, size)
    new_buf[:copy_len] = ptr[:copy_len]
    return new_buf


# ============================================================================
# Berry-level allocator with GC tracking (be_mem.c)
# ============================================================================

# BERRY_API void* be_realloc(bvm *vm, void *ptr, size_t old_size, size_t new_size)
# {
#     void *block = NULL;
#     bbool gc_occured = bfalse;
#     if (old_size == new_size) {
#         return ptr;
#     }
#     while (1) {
#         if (!ptr || (old_size == 0)) {
#             block = malloc_from_pool(vm, new_size);
#         }
#         else if (new_size == 0) {
#             if (ptr == NULL) { return NULL; }
#             if (BE_USE_DEBUG_GC || comp_is_gc_debug(vm)) {
#                 memset(ptr, 0xFF, old_size);
#             }
#             free_from_pool(vm, ptr, old_size);
#             break;
#         }
#         else if (new_size && old_size) {
#             block = realloc(ptr, new_size);
#         }
#         if (block) { break; }
#         if (gc_occured) {
#             be_throw(vm, BE_MALLOC_FAIL);
#         }
#         vm->gc.status |= GC_ALLOC;
#         be_gc_collect(vm);
#         vm->gc.status &= ~GC_ALLOC;
#         gc_occured = btrue;
#     }
#     vm->gc.usage = vm->gc.usage + new_size - old_size;
#     return block;
# }
def be_realloc(vm, ptr, old_size, new_size):
    """Central Berry allocator with gc.usage tracking.

    Wraps Python allocation. On failure, triggers a GC cycle and
    retries once before raising BE_MALLOC_FAIL via be_throw.

    Memory pools are skipped — Python's allocator handles small objects.
    """
    if old_size == new_size:
        return ptr

    block = None
    gc_occurred = False

    while True:
        # Case 1: new allocation (ptr is None or old_size == 0)
        if ptr is None or old_size == 0:
            if new_size > 0:
                try:
                    block = bytearray(new_size)
                except MemoryError:
                    block = None
            else:
                block = None

        # Case 2: deallocation (new_size == 0)
        elif new_size == 0:
            # In Python, just drop the reference; nothing to explicitly free.
            # In debug mode the C code memsets to 0xFF — skip in Python.
            block = None
            break  # early exit

        # Case 3: reallocation (both old_size and new_size > 0)
        else:
            try:
                new_buf = bytearray(new_size)
                copy_len = min(old_size, new_size)
                if ptr is not None:
                    new_buf[:copy_len] = ptr[:copy_len]
                block = new_buf
            except MemoryError:
                block = None

        # Check result
        if block is not None:
            break

        if gc_occurred:
            # Already tried GC, can't recover — raise allocation failure.
            # Import here to avoid circular imports at module level.
            from berry_port.be_exec import be_throw
            be_throw(vm, BE_MALLOC_FAIL)

        # Force a GC cycle and retry
        vm.gc.status |= GC_ALLOC
        from berry_port.be_gc import be_gc_collect
        be_gc_collect(vm)
        vm.gc.status &= ~GC_ALLOC
        gc_occurred = True

    # Update allocated-bytes counter
    vm.gc.usage = vm.gc.usage + new_size - old_size
    return block


# ============================================================================
# Convenience macros (be_mem.h)
# ============================================================================

# #define be_malloc(vm, size)         be_realloc((vm), NULL, 0, (size))
def be_malloc(vm, size):
    """Allocate *size* bytes (convenience wrapper around be_realloc)."""
    return be_realloc(vm, None, 0, size)


# #define be_free(vm, ptr, size)      be_realloc((vm), (ptr), (size), 0)
def be_free(vm, ptr, size):
    """Free a block of *size* bytes (convenience wrapper around be_realloc)."""
    return be_realloc(vm, ptr, size, 0)


# ============================================================================
# Memory pool stubs (skipped per design decision)
# ============================================================================

# Memory pools (pool16/pool32) are not implemented in the Python port.
# Python's allocator handles small objects efficiently, so the pool
# infrastructure from be_mem.c is unnecessary. The following functions
# are provided as no-ops for API compatibility.

# BERRY_API void be_gc_init_memory_pools(bvm *vm) {
#     vm->gc.pool16 = NULL;
#     vm->gc.pool32 = NULL;
# }
def be_gc_init_memory_pools(vm):
    """No-op — memory pools are not used in the Python port."""
    pass


# BERRY_API void be_gc_free_memory_pools(bvm *vm) { ... }
def be_gc_free_memory_pools(vm):
    """No-op — memory pools are not used in the Python port."""
    pass


# BERRY_API void be_gc_memory_pools(bvm *vm) { ... }
def be_gc_memory_pools(vm):
    """No-op — memory pools are not used in the Python port."""
    pass


# BERRY_API void be_gc_memory_pools_info(bvm *vm, size_t* slots_used, size_t* slots_allocated)
def be_gc_memory_pools_info(vm):
    """Return (slots_used, slots_allocated) — always (0, 0) in Python port."""
    return (0, 0)
