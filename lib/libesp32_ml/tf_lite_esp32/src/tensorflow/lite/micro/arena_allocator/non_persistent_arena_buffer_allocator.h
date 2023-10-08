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
#ifndef TENSORFLOW_LITE_MICRO_ARENA_ALLOCATOR_NON_PERSISTENT_ARENA_BUFFER_ALLOCATOR_H_
#define TENSORFLOW_LITE_MICRO_ARENA_ALLOCATOR_NON_PERSISTENT_ARENA_BUFFER_ALLOCATOR_H_

#include <cstddef>
#include <cstdint>

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/micro/arena_allocator/ibuffer_allocator.h"
#include "tensorflow/lite/micro/compatibility.h"

namespace tflite {

// Implement INonPersistentBufferAllocator on an arena that is dedicated for
// non-persistent buffers.
class NonPersistentArenaBufferAllocator : public INonPersistentBufferAllocator {
 public:
  NonPersistentArenaBufferAllocator(uint8_t* buffer, size_t buffer_size);
  virtual ~NonPersistentArenaBufferAllocator();

  // Allocates a temporary buffer. This buffer is not resizable.
  uint8_t* AllocateTemp(size_t size, size_t alignment) override;

  // Signals that a temporary buffer is no longer needed.
  void DeallocateTemp(uint8_t* buf) override;

  // Returns true if all temporary buffers are already deallocated.
  bool IsAllTempDeallocated() override;

  // Signals that all temporary allocations can be reclaimed. TFLM calls this
  // API when it knows that all temporary buffers that it requested has been
  // deallocated.
  TfLiteStatus ResetTempAllocations() override;

  // Returns a buffer that is resizable viable ResizeBuffer().
  uint8_t* AllocateResizableBuffer(size_t size, size_t alignment) override;

  // Resizes a buffer that is previously returned by the
  // AllocateResizableBuffer.
  TfLiteStatus ResizeBuffer(uint8_t* resizable_buf, size_t size,
                            size_t alignment) override;

  // Frees up the memory occupied by the resizable buffer.
  TfLiteStatus DeallocateResizableBuffer(uint8_t* resizable_buf) override;

  // Returns a pointer pointing to the start of the overlay memory, which is
  // used for activation tensors and scratch buffers by kernels at Invoke stage.
  uint8_t* GetOverlayMemoryAddress() const override;

  // Reserves the size of the overlay memory. This overlay is reserved for the
  // kernels at Invoke stage. This is referred to as the overlay because before
  // Invoket state, the same memory can be used for temp buffers. The layout of
  // the memory is planned by the memory planner separately at Invoke stage.
  TfLiteStatus ReserveNonPersistentOverlayMemory(size_t size,
                                                 size_t alignment) override;

  // Returns the size of non-persistent buffer in use.
  size_t GetNonPersistentUsedBytes() const override;

  // Returns the number of bytes available with a given alignment. This number
  // takes in account any temporary allocations.
  size_t GetAvailableMemory(size_t alignment) const override;

  TF_LITE_REMOVE_VIRTUAL_DELETE

 private:
  // The memory arena that this allocator manages.
  uint8_t* const buffer_head_;
  uint8_t* const buffer_tail_;

  // The whole region is split into two parts:
  // buffer_head_ to head_temp_ - 1 belongs to the only resizable buffer.
  // head_temp_ to buffer_tail_ can be used for (non-resizable) temp buffers.
  uint8_t* head_temp_;

  // next_temp_ points to the next available temp buffer allocation address and
  // its range is between head_temp_ and buffer_tail_
  uint8_t* next_temp_;

  // XOR Check sum for outstanding temp buffers.
  // If all temp buffers are deallocated OR no temp buffers are allocated,
  // temp_buffer_ptr_check_sum_ == nullptr.
  intptr_t temp_buffer_ptr_check_sum_ = 0;
  // Count of outstanding temp buffers.
  int temp_buffer_count_ = 0;
  bool resizable_buffer_allocated_ = false;
};

}  // namespace tflite

#endif  // TENSORFLOW_LITE_MICRO_ARENA_ALLOCATOR_NON_PERSISTENT_ARENA_BUFFER_ALLOCATOR_H_
