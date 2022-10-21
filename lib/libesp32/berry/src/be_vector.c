/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "be_vector.h"
#include "be_mem.h"
#include <string.h>

/* initialize a vector, the vector structure itself is usually allocated
 * on the stack, and the data is allocated from the heap.
 **/
void be_vector_init(bvm *vm, bvector *vector, int size)
{
    vector->capacity = 2; /* the default capacity */
    vector->size = size;
    vector->count = 0;
    vector->data = be_malloc(vm, (size_t)vector->capacity * size);
    vector->end = (char*)vector->data - size;
    memset(vector->data, 0, (size_t)vector->capacity * size);
}

void be_vector_delete(bvm *vm, bvector *vector)
{
    be_free(vm, vector->data, (size_t)vector->capacity * vector->size);
}

void* be_vector_at(bvector *vector, int index)
{
    return (char*)vector->data + (size_t)index * vector->size;
}

void be_vector_push(bvm *vm, bvector *vector, void *data)
{
    size_t size = vector->size;
    size_t capacity = vector->capacity;
    size_t count = vector->count++;
    if (count >= capacity) {
        int newcap = be_nextsize(vector->capacity);
        vector->data = be_realloc(vm,
                vector->data, vector->capacity * size, newcap * size);
        vector->end = (char*)vector->data + count * size;
        vector->capacity = newcap;
    } else {
        vector->end = (char*)vector->end + size;
    }
    if (data != NULL) {
        memcpy(vector->end, data, size);
    }
}

/* clear the expanded portion if the memory expands */
void be_vector_push_c(bvm *vm, bvector *vector, void *data)
{
    int capacity = vector->capacity + 1;
    be_vector_push(vm, vector, data);
    if (vector->capacity > capacity) {
        size_t size = ((size_t)vector->capacity - capacity) * vector->size;
        memset(be_vector_at(vector, capacity), 0, size);
    }
}

void be_vector_remove_end(bvector *vector)
{
    be_assert(vector->count > 0);
    vector->count--;
    vector->end = (char*)vector->end - vector->size;
}

void be_vector_resize(bvm *vm, bvector *vector, int count)
{
    size_t size = vector->size;
    be_assert(count >= 0);
    if (count != be_vector_count(vector)) {
        int newcap = be_nextsize(count);
        if (newcap > vector->capacity) { /* extended capacity */
            vector->data = be_realloc(vm,
                vector->data, vector->capacity * size, newcap * size);
            vector->capacity = newcap;
        }
        vector->count = count;
        if (count == 0) {
            vector->end = (char*)vector->data - size;
        } else {
            vector->end = (char*)vector->data + size * ((size_t)count - 1);
        }
    }
}

void be_vector_clear(bvector *vector)
{
    vector->count = 0;
    vector->end = (char*)vector->data - vector->size;
}

/* free not used */
void* be_vector_release(bvm *vm, bvector *vector)
{
    size_t size = vector->size;
    int count = be_vector_count(vector);
    if (count == 0) {
        be_free(vm, vector->data, vector->capacity * size);
        vector->capacity = 0;
        vector->data = NULL;
        vector->end = NULL;
    } else if (count < vector->capacity) {
        vector->data = be_realloc(vm,
            vector->data, vector->capacity * size, count * size);
        vector->end = (char*)vector->data + ((size_t)count - 1) * size;
        vector->capacity = count;
    }
    return vector->data;
}

/* use binary search to find the vector capacity between 0-1024 */
static int binary_search(int value)
{
    static const uint16_t tab[] = {
        0, 2, 4, 6, 8, 10, 12, 14, 16,
        20, 24, 28, 32, 40, 48, 64, 96, 128,
        192, 256, 384, 512, 768, 1024
    };
    const uint16_t *low = tab;
    const uint16_t *high = tab + array_count(tab) - 1;
    while (low <= high) {
        const uint16_t *mid = low + ((high - low) >> 1);
        if (*mid == value) {
            return mid[1];
        }
        if (*mid < value) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return *low;
}

static int nextpow(int value)
{
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    return value + 1;
}

int be_nextsize(int size)
{
    if (size < 1024) {
        return binary_search(size);
    }
    return nextpow(size);
}
