#include <assert.h>
#include <esp_attr.h>
#include <esp_heap_caps.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "line_queue.h"
#include "render_method.h"

static inline int ceil_div(int x, int y) {
    return x / y + (x % y != 0);
}

/// Initialize the line queue and allocate memory.
LineQueue_t lq_init(int queue_len, int element_size) {
    LineQueue_t queue;
    queue.element_size = element_size;
    queue.size = queue_len;
    queue.current = 0;
    queue.last = 0;

    int elem_buf_size = ceil_div(element_size, 16) * 16;

    queue.bufs = calloc(queue.size, elem_buf_size);
    assert(queue.bufs != NULL);

    for (int i = 0; i < queue.size; i++) {
        queue.bufs[i] = heap_caps_aligned_alloc(16, elem_buf_size, MALLOC_CAP_INTERNAL);
        assert(queue.bufs[i] != NULL);
    }

    return queue;
}

/// Deinitialize the line queue and free memory.
void lq_free(LineQueue_t* queue) {
    for (int i = 0; i < queue->size; i++) {
        heap_caps_free(queue->bufs[i]);
    }

    free(queue->bufs);
}

uint8_t* IRAM_ATTR lq_current(LineQueue_t* queue) {
    int current = atomic_load_explicit(&queue->current, memory_order_acquire);
    int last = atomic_load_explicit(&queue->last, memory_order_acquire);

    if ((current + 1) % queue->size == last) {
        return NULL;
    }
    return queue->bufs[current];
}

void IRAM_ATTR lq_commit(LineQueue_t* queue) {
    int current = atomic_load_explicit(&queue->current, memory_order_acquire);

    if (current == queue->size - 1) {
        queue->current = 0;
    } else {
        atomic_fetch_add(&queue->current, 1);
    }
}

int IRAM_ATTR lq_read(LineQueue_t* queue, uint8_t* dst) {
    int current = atomic_load_explicit(&queue->current, memory_order_acquire);
    int last = atomic_load_explicit(&queue->last, memory_order_acquire);

    if (current == last) {
        return -1;
    }

    memcpy(dst, queue->bufs[last], queue->element_size);

    if (last == queue->size - 1) {
        queue->last = 0;
    } else {
        atomic_fetch_add(&queue->last, 1);
    }
    return 0;
}

void IRAM_ATTR lq_reset(LineQueue_t* queue) {
    queue->current = 0;
    queue->last = 0;
}
