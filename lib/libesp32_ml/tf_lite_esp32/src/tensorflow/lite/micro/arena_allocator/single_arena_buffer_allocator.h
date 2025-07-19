/* Copyright 2020 The TensorFlow Authors. All Rights Reserved.

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

#ifndef TENSORFLOW_LITE_MICRO_ARENA_ALLOCATOR_SINGLE_ARENA_BUFFER_ALLOCATOR_H_
#define TENSORFLOW_LITE_MICRO_ARENA_ALLOCATOR_SINGLE_ARENA_BUFFER_ALLOCATOR_H_

#include <cstddef>
#include <cstdint>

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/core/api/error_reporter.h"
#include "tensorflow/lite/micro/arena_allocator/ibuffer_allocator.h"
#include "tensorflow/lite/micro/compatibility.h"

namespace tflite {

// TODO(petewarden): This allocator never frees up or reuses  any memory, even
// though we have enough information about lifetimes of the tensors to do so.
// This makes it pretty wasteful, so we should use a more intelligent method.
class SingleArenaBufferAllocator : public INonPersistentBufferAllocator,
                                   public IPersistentBufferAllocator {
 public:
  // TODO(b/246776144): Will be removed with http://b/246776144
  SingleArenaBufferAllocator(ErrorReporter* error_reporter,
                             uint8_t* buffer_head, uint8_t* buffer_tail)
      : SingleArenaBufferAllocator(buffer_head, buffer_tail) {
    (void)error_reporter;
  }
  SingleArenaBufferAllocator(ErrorReporter* error_reporter, uint8_t* buffer,
                             size_t buffer_size)
      : SingleArenaBufferAllocator(buffer, buffer_size) {
    (void)error_reporter;
  }

  // TODO(b/157615197): Cleanup constructors/destructor and use factory
  // functions.
  SingleArenaBufferAllocator(uint8_t* buffer_head, uint8_t* buffer_tail);
  SingleArenaBufferAllocator(uint8_t* buffer, size_t buffer_size);
  virtual ~SingleArenaBufferAllocator();

  // TODO(b/246776144): Will be removed with http://b/246776144
  static SingleArenaBufferAllocator* Create(ErrorReporter* error_reporter,
                                            uint8_t* buffer_head,
                                            size_t buffer_size) {
    (void)error_reporter;
    return SingleArenaBufferAllocator::Create(buffer_head, buffer_size);
  }

  // Creates a new SingleArenaBufferAllocator from a given buffer head and size.
  static SingleArenaBufferAllocator* Create(uint8_t* buffer_head,
                                            size_t buffer_size);

  // Resizes a buffer that is previously returned by the
  // AllocateResizableBuffer. In current implementation, it Adjusts the head
  // (lowest address and moving upwards) memory allocation to a given size.
  // Calls to this method will also invalidate all temporary allocation values
  // (it sets the location of temp space at the end of the head section). This
  // call will fail if a chain of allocations through AllocateTemp() have not
  // been cleaned up with a call to ResetTempAllocations().
  virtual TfLiteStatus ResizeBuffer(uint8_t* resizable_buf, size_t size,
                                    size_t alignment) override;

  // Returns a buffer that is resizable viable ResizeBuffer(). Only one
  // resizable buffer is currently supported.
  virtual uint8_t* AllocateResizableBuffer(size_t size,
                                           size_t alignment) override;

  // Frees up the memory occupied by the resizable buffer
  virtual TfLiteStatus DeallocateResizableBuffer(
      uint8_t* resizable_buf) override;

  // Reserves the non-persistent memory that is planned by the memory planner.
  virtual TfLiteStatus ReserveNonPersistentOverlayMemory(
      size_t size, size_t alignment) override;

  // Allocates persistent memory starting at the tail of the arena (highest
  // address and moving downwards).
  virtual uint8_t* AllocatePersistentBuffer(size_t size,
                                            size_t alignment) override;

  // Allocates a temporary buffer from the head of the arena (lowest address and
  // moving upwards) but does not update the actual head allocation size or
  // position. The returned buffer is guaranteed until either
  // ResetTempAllocations() is called or another call to AllocateFromHead().
  // Repeat calls to this function will create a chain of temp allocations. All
  // calls to AllocateTemp() must end with a call to ResetTempAllocations(). If
  // AllocateFromHead() is called before a call to ResetTempAllocations(), it
  // will fail with an error message.
  virtual uint8_t* AllocateTemp(size_t size, size_t alignment) override;

  // Signals that a temporary buffer is no longer needed. This is currently for
  // book-keeping purpose and the memory region are not immediately available
  // for re-use. The deallocated memory region are only reclaimed after
  // ResetTempAllocations is called as it is right now.
  virtual void DeallocateTemp(uint8_t* buf) override;

  // Returns true if all temporary buffers are already deallocated.
  virtual bool IsAllTempDeallocated() override;

  // Resets a chain of temporary allocations back to the current head of the
  // arena (lowest address).
  virtual TfLiteStatus ResetTempAllocations() override;

  // Returns a pointer to the buffer currently assigned to the head section.
  // This buffer is set by calling SetHeadSize().
  uint8_t* GetOverlayMemoryAddress() const override;

  // Returns the size of the head section in bytes.
  size_t GetNonPersistentUsedBytes() const override;

  // Returns the size of all allocations in the tail section in bytes.
  size_t GetPersistentUsedBytes() const override;

  // Returns the number of bytes available with a given alignment. This number
  // takes in account any temporary allocations.
  size_t GetAvailableMemory(size_t alignment) const override;

  // Returns the number of used bytes in the allocator. This number takes in
  // account any temporary allocations.
  size_t GetUsedBytes() const;

  TF_LITE_REMOVE_VIRTUAL_DELETE

 protected:
  // Returns a pointer to the current end of the head buffer.
  uint8_t* head() const;

  // Returns a pointer to the current end of the tail buffer.
  uint8_t* tail() const;

 private:
  size_t GetBufferSize() const;
  uint8_t* buffer_head_;
  uint8_t* buffer_tail_;
  uint8_t* head_;
  uint8_t* tail_;
  uint8_t* temp_;

  // The combination of the checksum of outstanding temporary buffer pointers
  // AND the count of outstanding temporary buffer provide a low cost mechanism
  // to audit temporary buffers' allocation and deallocation.
  //
  // XOR Check sum for outstanding temp buffers.
  // If all temp buffers are deallocated OR no temp buffers are allocated,
  // temp_buffer_ptr_check_sum_ == nullptr.
  intptr_t temp_buffer_ptr_check_sum_ = 0;
  // Count of outstanding temp buffers.
  int temp_buffer_count_ = 0;
};

}  // namespace tflite

#endif  // TENSORFLOW_LITE_MICRO_ARENA_ALLOCATOR_SINGLE_ARENA_BUFFER_ALLOCATOR_H_
