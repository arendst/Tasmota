"""
Berry vector/stack data structure.
Mirrors: src/be_vector.c / src/be_vector.h

The bvector is a dynamic array used throughout the Berry interpreter for
call stacks, exception stacks, and other growable collections. The bstack
type is an alias for bvector with push/pop semantics.

In the C implementation, bvector stores raw bytes with a configurable
element size. In the Python port, we use a Python list as the backing
store, so the 'size' field is informational only (preserved for API
compatibility and gc.usage tracking).
"""

# /********************************************************************
# ** Copyright (c) 2018-2020 Guan Wenliang
# ** This file is part of the Berry default interpreter.
# ** skiars@qq.com, https://github.com/Skiars/berry
# ** See Copyright Notice in the LICENSE file or at
# ** https://github.com/Skiars/berry/blob/master/LICENSE
# ********************************************************************/

from berry_port.be_mem import be_malloc, be_free, be_realloc


# ============================================================================
# bvector class — Python equivalent of the C struct bvector
# ============================================================================

class bvector:
    """Dynamic array used throughout the Berry interpreter.

    Mirrors: struct bvector (be_vector.h)
    """
    __slots__ = ('capacity', 'size', 'count', 'data', 'end', '_mem')

    def __init__(self):
        self.capacity = 0
        self.size = 0
        self.count = 0
        self.data = None
        self.end = -1
        self._mem = None


# ============================================================================
# be_nextsize — capacity growth strategy (from be_vector.c)
# ============================================================================

# /* use binary search to find the vector capacity between 0-1024 */
# static int binary_search(int value)
# {
#     static const uint16_t tab[] = {
#         0, 2, 4, 6, 8, 10, 12, 14, 16,
#         20, 24, 28, 32, 40, 48, 64, 96, 128,
#         192, 256, 384, 512, 768, 1024
#     };
#     const uint16_t *low = tab;
#     const uint16_t *high = tab + array_count(tab) - 1;
#     while (low <= high) {
#         const uint16_t *mid = low + ((high - low) >> 1);
#         if (*mid == value) {
#             return mid[1];
#         }
#         if (*mid < value) {
#             low = mid + 1;
#         } else {
#             high = mid - 1;
#         }
#     }
#     return *low;
# }
_BINARY_SEARCH_TAB = [
    0, 2, 4, 6, 8, 10, 12, 14, 16,
    20, 24, 28, 32, 40, 48, 64, 96, 128,
    192, 256, 384, 512, 768, 1024
]

def _binary_search(value):
    """Binary search the capacity table for the next size above *value*."""
    tab = _BINARY_SEARCH_TAB
    low = 0
    high = len(tab) - 1
    while low <= high:
        mid = low + ((high - low) >> 1)
        if tab[mid] == value:
            return tab[mid + 1]
        if tab[mid] < value:
            low = mid + 1
        else:
            high = mid - 1
    return tab[low]


# static int nextpow(int value)
# {
#     value |= value >> 1;
#     value |= value >> 2;
#     value |= value >> 4;
#     value |= value >> 8;
#     value |= value >> 16;
#     return value + 1;
# }
def _nextpow(value):
    """Round up to the next power of two."""
    value |= value >> 1
    value |= value >> 2
    value |= value >> 4
    value |= value >> 8
    value |= value >> 16
    return value + 1


# int be_nextsize(int size)
# {
#     if (size < 1024) {
#         return binary_search(size);
#     }
#     return nextpow(size);
# }
def be_nextsize(size):
    """Compute the next vector capacity for a given size.

    For sizes < 1024, uses a binary search table of preferred sizes.
    For sizes >= 1024, rounds up to the next power of two.
    """
    if size < 1024:
        return _binary_search(size)
    return _nextpow(size)


# ============================================================================
# Vector operations (from be_vector.c)
# ============================================================================

# /* initialize a vector, the vector structure itself is usually allocated
#  * on the stack, and the data is allocated from the heap.
#  **/
# void be_vector_init(bvm *vm, bvector *vector, int size)
# {
#     vector->capacity = 2; /* the default capacity */
#     vector->size = size;
#     vector->count = 0;
#     vector->data = be_malloc(vm, (size_t)vector->capacity * size);
#     vector->end = (char*)vector->data - size;
#     memset(vector->data, 0, (size_t)vector->capacity * size);
# }
def be_vector_init(vm, vector, size):
    """Initialize a vector with default capacity of 2.

    *size* is the element byte size (informational in Python; used for
    gc.usage tracking via be_malloc/be_realloc).
    """
    vector.capacity = 2  # the default capacity
    vector.size = size
    vector.count = 0
    # Allocate backing memory for gc.usage tracking
    vector._mem = be_malloc(vm, vector.capacity * size)
    # Python list as actual data store
    vector.data = [None] * vector.capacity
    vector.end = -1  # index of last element; -1 means "before data[0]"


# void be_vector_delete(bvm *vm, bvector *vector)
# {
#     be_free(vm, vector->data, (size_t)vector->capacity * vector->size);
# }
def be_vector_delete(vm, vector):
    """Free the vector's backing storage."""
    be_free(vm, vector._mem, vector.capacity * vector.size)
    vector._mem = None
    vector.data = None
    vector.end = -1
    vector.count = 0
    vector.capacity = 0


# void* be_vector_at(bvector *vector, int index)
# {
#     return (char*)vector->data + (size_t)index * vector->size;
# }
def be_vector_at(vector, index):
    """Return the element at *index*."""
    return vector.data[index]


# void be_vector_push(bvm *vm, bvector *vector, void *data)
# {
#     size_t size = vector->size;
#     size_t capacity = vector->capacity;
#     size_t count = vector->count++;
#     if (count >= capacity) {
#         int newcap = be_nextsize(vector->capacity);
#         vector->data = be_realloc(vm,
#                 vector->data, vector->capacity * size, newcap * size);
#         vector->end = (char*)vector->data + count * size;
#         vector->capacity = newcap;
#     } else {
#         vector->end = (char*)vector->end + size;
#     }
#     if (data != NULL) {
#         memcpy(vector->end, data, size);
#     }
# }
def be_vector_push(vm, vector, data):
    """Append *data* to the vector, growing capacity if needed."""
    size = vector.size
    capacity = vector.capacity
    count = vector.count
    vector.count = count + 1

    if count >= capacity:
        newcap = be_nextsize(vector.capacity)
        # Track memory via be_realloc
        vector._mem = be_realloc(vm, vector._mem,
                                 vector.capacity * size, newcap * size)
        # Grow the Python list (or create fresh if released to None)
        if vector.data is None:
            vector.data = [None] * newcap
        else:
            vector.data.extend([None] * (newcap - capacity))
        vector.end = count
        vector.capacity = newcap
    else:
        vector.end = count

    if data is not None:
        vector.data[vector.end] = data


# /* clear the expanded portion if the memory expands */
# void be_vector_push_c(bvm *vm, bvector *vector, void *data)
# {
#     int capacity = vector->capacity + 1;
#     be_vector_push(vm, vector, data);
#     if (vector->capacity > capacity) {
#         size_t size = ((size_t)vector->capacity - capacity) * vector->size;
#         memset(be_vector_at(vector, capacity), 0, size);
#     }
# }
def be_vector_push_c(vm, vector, data):
    """Push with clearing of any newly expanded portion."""
    old_capacity_plus1 = vector.capacity + 1
    be_vector_push(vm, vector, data)
    if vector.capacity > old_capacity_plus1:
        # Clear the expanded portion (set to None)
        for i in range(old_capacity_plus1, vector.capacity):
            vector.data[i] = None


# void be_vector_remove_end(bvector *vector)
# {
#     be_assert(vector->count > 0);
#     vector->count--;
#     vector->end = (char*)vector->end - vector->size;
# }
def be_vector_remove_end(vector):
    """Remove the last element from the vector."""
    assert vector.count > 0, "be_vector_remove_end: vector is empty"
    vector.count -= 1
    vector.end -= 1


# void be_vector_resize(bvm *vm, bvector *vector, int count)
# {
#     size_t size = vector->size;
#     be_assert(count >= 0);
#     if (count != be_vector_count(vector)) {
#         int newcap = be_nextsize(count);
#         if (newcap > vector->capacity) { /* extended capacity */
#             vector->data = be_realloc(vm,
#                 vector->data, vector->capacity * size, newcap * size);
#             vector->capacity = newcap;
#         }
#         vector->count = count;
#         if (count == 0) {
#             vector->end = (char*)vector->data - size;
#         } else {
#             vector->end = (char*)vector->data + size * ((size_t)count - 1);
#         }
#     }
# }
def be_vector_resize(vm, vector, count):
    """Resize the vector to exactly *count* elements."""
    size = vector.size
    assert count >= 0, "be_vector_resize: count must be >= 0"
    if count != vector.count:
        newcap = be_nextsize(count)
        if newcap > vector.capacity:
            # Extended capacity — track via be_realloc
            vector._mem = be_realloc(vm, vector._mem,
                                     vector.capacity * size, newcap * size)
            # Extend the Python list
            vector.data.extend([None] * (newcap - vector.capacity))
            vector.capacity = newcap
        vector.count = count
        if count == 0:
            vector.end = -1
        else:
            vector.end = count - 1


# void be_vector_clear(bvector *vector)
# {
#     vector->count = 0;
#     vector->end = (char*)vector->data - vector->size;
# }
def be_vector_clear(vector):
    """Reset the vector to empty without freeing memory."""
    vector.count = 0
    vector.end = -1


# /* free not used */
# void* be_vector_release(bvm *vm, bvector *vector)
# {
#     size_t size = vector->size;
#     int count = be_vector_count(vector);
#     if (count == 0) {
#         be_free(vm, vector->data, vector->capacity * size);
#         vector->capacity = 0;
#         vector->data = NULL;
#         vector->end = NULL;
#     } else if (count < vector->capacity) {
#         vector->data = be_realloc(vm,
#             vector->data, vector->capacity * size, count * size);
#         vector->end = (char*)vector->data + ((size_t)count - 1) * size;
#         vector->capacity = count;
#     }
#     return vector->data;
# }
def be_vector_release(vm, vector):
    """Shrink the vector's backing storage to fit its current count.

    Returns the data list (or None if empty).
    """
    size = vector.size
    count = vector.count
    if count == 0:
        be_free(vm, vector._mem, vector.capacity * size)
        vector._mem = None
        vector.capacity = 0
        vector.data = None
        vector.end = -1
    elif count < vector.capacity:
        vector._mem = be_realloc(vm, vector._mem,
                                 vector.capacity * size, count * size)
        vector.data = vector.data[:count]
        vector.end = count - 1
        vector.capacity = count
    return vector.data


# ============================================================================
# Accessor macros (from be_vector.h)
# ============================================================================

# #define be_vector_data(vector)          ((vector)->data)
def be_vector_data(vector):
    """Return the vector's data list."""
    return vector.data

# #define be_vector_first(vector)         ((vector)->data)
def be_vector_first(vector):
    """Return the first element (alias for data[0])."""
    return vector.data[0] if vector.count > 0 else None

# #define be_vector_isend(vector, item)   ((item) > (vector)->end)
def be_vector_isend(vector, index):
    """Check if *index* is past the last element."""
    return index > vector.end

# #define be_vector_isempty(vector)       (!(vector)->count)
def be_vector_isempty(vector):
    """Check if the vector is empty."""
    return vector.count == 0

# #define be_vector_end(vector)           ((vector)->end)
def be_vector_end(vector):
    """Return the last element of the vector (mirrors C pointer to last element)."""
    if vector.end >= 0 and vector.end < len(vector.data):
        return vector.data[vector.end]
    return None

# #define be_vector_count(vector)         ((vector)->count)
def be_vector_count(vector):
    """Return the number of elements."""
    return vector.count

# #define be_vector_capacity(vector)      ((vector)->capacity)
def be_vector_capacity(vector):
    """Return the current capacity."""
    return vector.capacity


# ============================================================================
# Stack aliases (from be_vector.h)
# ============================================================================

# #define be_stack_init(vm, stack, size)  be_vector_init(vm, stack, size)
def be_stack_init(vm, stack, size):
    """Initialize a stack (alias for be_vector_init)."""
    be_vector_init(vm, stack, size)

# #define be_stack_delete(vm, stack)      be_vector_delete(vm, stack)
def be_stack_delete(vm, stack):
    """Delete a stack (alias for be_vector_delete)."""
    be_vector_delete(vm, stack)

# #define be_stack_clear(stack)           be_vector_clear(stack)
def be_stack_clear(stack):
    """Clear a stack (alias for be_vector_clear)."""
    be_vector_clear(stack)

# #define be_stack_push(vm, stack, data)  be_vector_push(vm, stack, data)
def be_stack_push(vm, stack, data):
    """Push onto a stack (alias for be_vector_push)."""
    be_vector_push(vm, stack, data)

# #define be_stack_pop(stack)             be_vector_remove_end(stack)
def be_stack_pop(stack):
    """Pop from a stack (alias for be_vector_remove_end)."""
    be_vector_remove_end(stack)

# #define be_stack_top(stack)             be_vector_end(stack)
def be_stack_top(stack):
    """Return the index of the top element on the stack."""
    return be_vector_end(stack)

# #define be_stack_base(stack)            be_vector_first(stack)
def be_stack_base(stack):
    """Return the first element of the stack."""
    return be_vector_first(stack)

# #define be_stack_count(stack)           be_vector_count(stack)
def be_stack_count(stack):
    """Return the number of elements on the stack."""
    return be_vector_count(stack)

# #define be_stack_isempty(stack)         be_vector_isempty(stack)
def be_stack_isempty(stack):
    """Check if the stack is empty."""
    return be_vector_isempty(stack)
