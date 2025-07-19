/* Copyright 2022 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#include "tensorflow/lite/micro/arena_allocator/non_persistent_arena_buffer_allocator.h"

#include "tensorflow/lite/micro/memory_helpers.h"
#include "tensorflow/lite/micro/micro_log.h"

namespace tflite {

NonPersistentArenaBufferAllocator::NonPersistentArenaBufferAllocator(
    uint8_t* buffer, size_t buffer_size)
    : buffer_head_(buffer),
      buffer_tail_(buffer + buffer_size),
      head_temp_(buffer),
      next_temp_(buffer) {}

NonPersistentArenaBufferAllocator::~NonPersistentArenaBufferAllocator() {}

// Allocates a temporary buffer. This buffer is not resizable.
uint8_t* NonPersistentArenaBufferAllocator::AllocateTemp(size_t size,
                                                         size_t alignment) {
  uint8_t* const aligned_result = AlignPointerUp(next_temp_, alignment);
  const size_t available_memory = buffer_tail_ - aligned_result;
  if (available_memory < size) {
    MicroPrintf(
        "Failed to allocate temp memory. Requested: %u, "
        "available %u, missing: %u",
        size, available_memory, size - available_memory);
    return nullptr;
  }
  next_temp_ = aligned_result + size;
  temp_buffer_ptr_check_sum_ ^= reinterpret_cast<intptr_t>(aligned_result);
  temp_buffer_count_++;
  return aligned_result;
}

// Signals that a temporary buffer is no longer needed.
void NonPersistentArenaBufferAllocator::DeallocateTemp(uint8_t* temp_buf) {
  temp_buffer_ptr_check_sum_ ^= reinterpret_cast<intptr_t>(temp_buf);
  temp_buffer_count_--;
}

// Returns true if all temporary buffers are already deallocated.
bool NonPersistentArenaBufferAllocator::IsAllTempDeallocated() {
  if (temp_buffer_count_ != 0 || temp_buffer_ptr_check_sum_ != 0) {
    MicroPrintf(
        "Number of allocated temp buffers: %d. Checksum passing status: %d",
        temp_buffer_count_, !temp_buffer_ptr_check_sum_);
    return false;
  }
  return true;
}

// Signals that all temporary allocations can be reclaimed. TFLM calls this
// API when it knows that all temporary buffers that it requested has been
// deallocated. The goal of API is to facilitate implementations of
// INonPersistentBufferAllocator can reuse buffer with some reasonable
// complexity.
TfLiteStatus NonPersistentArenaBufferAllocator::ResetTempAllocations() {
  if (!IsAllTempDeallocated()) {
    MicroPrintf(
        "All temp buffers must be freed before calling ResetTempAllocations()");
    return kTfLiteError;
  }
  next_temp_ = head_temp_;
  return kTfLiteOk;
}

// Returns a buffer that is resizable viable ResizeBuffer().
uint8_t* NonPersistentArenaBufferAllocator::AllocateResizableBuffer(
    size_t size, size_t alignment) {
  // Only supports one resizable buffer, which starts at the buffer head.
  uint8_t* expected_resizable_buf = AlignPointerUp(buffer_head_, alignment);

  if (resizable_buffer_allocated_) {
    MicroPrintf(
        "Cannot allocate a new resizable buffer when one is already allocated");
    return nullptr;
  }

  if (ResizeBuffer(expected_resizable_buf, size, alignment) == kTfLiteOk) {
    resizable_buffer_allocated_ = true;
    return expected_resizable_buf;
  }
  return nullptr;
}

// Resizes a buffer that is previously returned by the AllocateResizableBuffer.
// Note that ResizeBuffer(old_resizable_buf, 0, 1) effectively deallocates
// a previous allocated resizable buffer.
TfLiteStatus NonPersistentArenaBufferAllocator::ResizeBuffer(
    uint8_t* resizable_buf, size_t size, size_t alignment) {
  // Only supports one resizable buffer, which starts at the buffer head.
  uint8_t* expect_resizable_buf = AlignPointerUp(buffer_head_, alignment);
  if (resizable_buf != expect_resizable_buf) {
    MicroPrintf("Internal error: buffer is not resizable");
    return kTfLiteError;
  }
  if (head_temp_ != next_temp_) {
    MicroPrintf("ResetTempAllocations() is not called before ResizeBuffer().");
    return kTfLiteError;
  }

  const size_t available_memory = buffer_tail_ - expect_resizable_buf;
  if (available_memory < size) {
    MicroPrintf(
        "Failed to resize buffer. Requested: %u, available %u, missing: %u",
        size, available_memory, size - available_memory);
    return kTfLiteError;
  }
  head_temp_ = expect_resizable_buf + size;
  next_temp_ = head_temp_;

  return kTfLiteOk;
}

// Frees up the memory occupied by the resizable buffer.
TfLiteStatus NonPersistentArenaBufferAllocator::DeallocateResizableBuffer(
    uint8_t* resizable_buf) {
  TfLiteStatus status = ResizeBuffer(resizable_buf, 0, 1);
  if (status == kTfLiteOk) {
    resizable_buffer_allocated_ = false;
  }
  return status;
}

// Returns a pointer pointing to the start of the overlay memory, which is
// used for activation tensors and scratch buffers by kernels at Invoke stage.
uint8_t* NonPersistentArenaBufferAllocator::GetOverlayMemoryAddress() const {
  return buffer_head_;
}

// Reserves the size of the overlay memory. This overlay is reserved for the
// kernels at Invoke stage. This is referred to as the overlay because before
// Invoket state, the same memory can be used for temp buffers. The layout of
// the memory is planned by the memory planner separately at Invoke stage.
TfLiteStatus
NonPersistentArenaBufferAllocator::ReserveNonPersistentOverlayMemory(
    size_t size, size_t alignment) {
  uint8_t* expect_resizable_buf = AlignPointerUp(buffer_head_, alignment);
  return ResizeBuffer(expect_resizable_buf, size, alignment);
}

// Returns the size of non-persistent buffer in use.
size_t NonPersistentArenaBufferAllocator::GetNonPersistentUsedBytes() const {
  return (next_temp_ - buffer_head_);
}

// Returns the number of bytes available with a given alignment. This number
// takes in account any temporary allocations.
size_t NonPersistentArenaBufferAllocator::GetAvailableMemory(
    size_t alignment) const {
  uint8_t* const aligned_temp = AlignPointerUp(next_temp_, alignment);
  uint8_t* const aligned_tail = AlignPointerDown(buffer_tail_, alignment);
  return aligned_tail - aligned_temp;
}

}  // namespace tflite
