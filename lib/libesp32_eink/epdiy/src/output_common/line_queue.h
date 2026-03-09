#pragma once

#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/// Circular line queue with atomic read / write operations
/// and accelerated masking on the output buffer.
typedef struct {
    int size;
    atomic_int current;
    atomic_int last;
    uint8_t** bufs;
    // size of an element
    size_t element_size;
} LineQueue_t;

/// Initialize the line queue and allocate memory.
LineQueue_t lq_init(int queue_len, int element_size);

/// Deinitialize the line queue and free memory.
void lq_free(LineQueue_t* queue);

/// Pointer to the next empty element in the line queue.
///
/// NULL if the queue is currently full.
uint8_t* lq_current(LineQueue_t* queue);

/// Advance the line queue.
void lq_commit(LineQueue_t* queue);

/// Read from the line queue.
///
/// Returns 0 for a successful read to `dst`, -1 for a failed read (empty queue).
int lq_read(LineQueue_t* queue, uint8_t* dst);

/// Reset the queue into an empty state.
/// This operation is *not* atomic!
void lq_reset(LineQueue_t* queue);
