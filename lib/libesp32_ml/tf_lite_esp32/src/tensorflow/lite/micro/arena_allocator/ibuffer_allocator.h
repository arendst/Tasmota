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
#ifndef TENSORFLOW_LITE_MICRO_ARENA_ALLOCATOR_IBUFFER_ALLOCATOR_H_
#define TENSORFLOW_LITE_MICRO_ARENA_ALLOCATOR_IBUFFER_ALLOCATOR_H_

#include <cstddef>
#include <cstdint>

#include "tensorflow/lite/c/c_api_types.h"

namespace tflite {
// Interface classes that the TFLM framework relies on to get buffers it needs.
// There are two types of buffers that the TFLM framework requires: persistent
// and non-persistent. Persistent buffers, once allocated, are never freed by
// the TFLM framework. Non-persist buffers can be allocated and deallocated by
// the TFLM framework. This file defines two interfaces classes that TFLM
// framework will rely on to manage these buffers.

// Interface class for managing persistent buffers.
class IPersistentBufferAllocator {
 public:
  IPersistentBufferAllocator() {}
  virtual ~IPersistentBufferAllocator() {}

  // Allocates persistent memory. The persistent buffer is never freed.
  virtual uint8_t* AllocatePersistentBuffer(size_t size, size_t alignment) = 0;

  // Returns the size of all persistent allocations in bytes.
  virtual size_t GetPersistentUsedBytes() const = 0;
};

// Interface class for managing non-persistent buffers.
// The default non-persistent buffers are temp buffers that are not resizable.
// Support of at least one resizable buffer is required.
class INonPersistentBufferAllocator {
 public:
  INonPersistentBufferAllocator() {}
  virtual ~INonPersistentBufferAllocator() {}

  // Allocates a temporary buffer. This buffer is not resizable.
  virtual uint8_t* AllocateTemp(size_t size, size_t alignment) = 0;

  // Signals that a temporary buffer is no longer needed.
  virtual void DeallocateTemp(uint8_t* buf) = 0;

  // Returns true if all temporary buffers are already deallocated.
  virtual bool IsAllTempDeallocated() = 0;

  // Signals that all temporary allocations can be reclaimed. TFLM calls this
  // API when it knows that all temporary buffers that it requested has been
  // deallocated. The goal of API is to facilitate implementations of
  // INonPersistentBufferAllocator can reuse buffer with some reasonable
  // complexity.
  virtual TfLiteStatus ResetTempAllocations() = 0;

  // Returns a buffer that is resizable viable ResizeBuffer().
  virtual uint8_t* AllocateResizableBuffer(size_t size, size_t alignment) = 0;

  // Resizes a buffer that is previously returned by the
  // AllocateResizableBuffer.
  virtual TfLiteStatus ResizeBuffer(uint8_t* resizable_buf, size_t size,
                                    size_t alignment) = 0;

  // Frees up the memory occupied by the resizable buffer.
  virtual TfLiteStatus DeallocateResizableBuffer(uint8_t* resizable_buf) = 0;

  // Returns a pointer pointing to the start of the overlay memory, which is
  // used for activation tensors and scratch buffers by kernels at Invoke stage.
  virtual uint8_t* GetOverlayMemoryAddress() const = 0;

  // Reserves the size of the overlay memory. This overlay is reserved for the
  // kernels at Invoke stage. This is referred to as the overlay because before
  // Invoket state, the same memory can be used for temp buffers. The layout of
  // the memory is planned by the memory planner separately at Invoke stage.
  virtual TfLiteStatus ReserveNonPersistentOverlayMemory(size_t size,
                                                         size_t alignment) = 0;

  // Returns the size of non-persistent buffer in use.
  virtual size_t GetNonPersistentUsedBytes() const = 0;

  // Returns the number of bytes available with a given alignment. This number
  // takes in account any temporary allocations.
  virtual size_t GetAvailableMemory(size_t alignment) const = 0;
};

}  // namespace tflite

#endif  // TENSORFLOW_LITE_MICRO_ARENA_ALLOCATOR_IBUFFER_ALLOCATOR_H_
