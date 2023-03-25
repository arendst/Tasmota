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

#include "tensorflow/lite/micro/arena_allocator/single_arena_buffer_allocator.h"

#include <cstddef>
#include <cstdint>
#include <new>

#include "tensorflow/lite/c/c_api_types.h"
#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/kernels/internal/compatibility.h"
#include "tensorflow/lite/kernels/op_macros.h"
#include "tensorflow/lite/micro/memory_helpers.h"
#include "tensorflow/lite/micro/micro_log.h"

namespace tflite {

SingleArenaBufferAllocator::SingleArenaBufferAllocator(uint8_t* buffer_head,
                                                       uint8_t* buffer_tail)
    : buffer_head_(buffer_head),
      buffer_tail_(buffer_tail),
      head_(buffer_head),
      tail_(buffer_tail),
      temp_(buffer_head_) {}

SingleArenaBufferAllocator::SingleArenaBufferAllocator(uint8_t* buffer,
                                                       size_t buffer_size)
    : SingleArenaBufferAllocator(buffer, buffer + buffer_size) {}

/* static */
SingleArenaBufferAllocator* SingleArenaBufferAllocator::Create(
    uint8_t* buffer_head, size_t buffer_size) {
  TFLITE_DCHECK(buffer_head != nullptr);
  SingleArenaBufferAllocator tmp =
      SingleArenaBufferAllocator(buffer_head, buffer_size);

  // Allocate enough bytes from the buffer to create a
  // SingleArenaBufferAllocator. The new instance will use the current adjusted
  // tail buffer from the tmp allocator instance.
  uint8_t* allocator_buffer = tmp.AllocatePersistentBuffer(
      sizeof(SingleArenaBufferAllocator), alignof(SingleArenaBufferAllocator));
  // Use the default copy constructor to populate internal states.
  return new (allocator_buffer) SingleArenaBufferAllocator(tmp);
}

SingleArenaBufferAllocator::~SingleArenaBufferAllocator() {}

uint8_t* SingleArenaBufferAllocator::AllocateResizableBuffer(size_t size,
                                                             size_t alignment) {
  // Only supports one resizable buffer, which starts at the buffer head.
  uint8_t* expect_resizable_buf = AlignPointerUp(buffer_head_, alignment);
  if (ResizeBuffer(expect_resizable_buf, size, alignment) == kTfLiteOk) {
    return expect_resizable_buf;
  }
  return nullptr;
}

TfLiteStatus SingleArenaBufferAllocator::DeallocateResizableBuffer(
    uint8_t* resizable_buf) {
  return ResizeBuffer(resizable_buf, 0, 1);
}

TfLiteStatus SingleArenaBufferAllocator::ReserveNonPersistentOverlayMemory(
    size_t size, size_t alignment) {
  uint8_t* expect_resizable_buf = AlignPointerUp(buffer_head_, alignment);
  return ResizeBuffer(expect_resizable_buf, size, alignment);
}

TfLiteStatus SingleArenaBufferAllocator::ResizeBuffer(uint8_t* resizable_buf,
                                                      size_t size,
                                                      size_t alignment) {
  // Only supports one resizable buffer, which starts at the buffer head.
  uint8_t* expect_resizable_buf = AlignPointerUp(buffer_head_, alignment);
  if (head_ != temp_ || resizable_buf != expect_resizable_buf) {
    MicroPrintf(
        "Internal error: either buffer is not resizable or "
        "ResetTempAllocations() is not called before ResizeBuffer().");
    return kTfLiteError;
  }

  uint8_t* const aligned_result = AlignPointerUp(buffer_head_, alignment);
  const size_t available_memory = tail_ - aligned_result;
  if (available_memory < size) {
    MicroPrintf(
        "Failed to resize buffer. Requested: %u, available %u, missing: %u",
        size, available_memory, size - available_memory);
    return kTfLiteError;
  }
  head_ = aligned_result + size;
  temp_ = head_;

  return kTfLiteOk;
}

uint8_t* SingleArenaBufferAllocator::AllocatePersistentBuffer(
    size_t size, size_t alignment) {
  uint8_t* const aligned_result = AlignPointerDown(tail_ - size, alignment);
  if (aligned_result < head_) {
#ifndef TF_LITE_STRIP_ERROR_STRINGS
    const size_t missing_memory = head_ - aligned_result;
    MicroPrintf(
        "Failed to allocate tail memory. Requested: %u, "
        "available %u, missing: %u",
        size, size - missing_memory, missing_memory);
#endif
    return nullptr;
  }
  tail_ = aligned_result;
  return aligned_result;
}

uint8_t* SingleArenaBufferAllocator::AllocateTemp(size_t size,
                                                  size_t alignment) {
  uint8_t* const aligned_result = AlignPointerUp(temp_, alignment);
  const size_t available_memory = tail_ - aligned_result;
  if (available_memory < size) {
    MicroPrintf(
        "Failed to allocate temp memory. Requested: %u, "
        "available %u, missing: %u",
        size, available_memory, size - available_memory);
    return nullptr;
  }
  temp_ = aligned_result + size;
  temp_buffer_ptr_check_sum_ ^= (reinterpret_cast<intptr_t>(aligned_result));
  temp_buffer_count_++;
  return aligned_result;
}

void SingleArenaBufferAllocator::DeallocateTemp(uint8_t* temp_buf) {
  temp_buffer_ptr_check_sum_ ^= (reinterpret_cast<intptr_t>(temp_buf));
  temp_buffer_count_--;
}

bool SingleArenaBufferAllocator::IsAllTempDeallocated() {
  if (temp_buffer_count_ != 0 || temp_buffer_ptr_check_sum_ != 0) {
    MicroPrintf(
        "Number of allocated temp buffers: %d. Checksum passing status: %d",
        temp_buffer_count_, !temp_buffer_ptr_check_sum_);
    return false;
  }
  return true;
}

TfLiteStatus SingleArenaBufferAllocator::ResetTempAllocations() {
  // TODO(b/209453859): enable error check based on IsAllTempDeallocated after
  // all AllocateTemp have been paird with DeallocateTemp
  if (!IsAllTempDeallocated()) {
    MicroPrintf(
        "All temp buffers must be freed before calling ResetTempAllocations()");
    return kTfLiteError;
  }
  temp_ = head_;
  return kTfLiteOk;
}

uint8_t* SingleArenaBufferAllocator::GetOverlayMemoryAddress() const {
  return buffer_head_;
}

size_t SingleArenaBufferAllocator::GetNonPersistentUsedBytes() const {
  return std::max(head_ - buffer_head_, temp_ - buffer_head_);
}

size_t SingleArenaBufferAllocator::GetPersistentUsedBytes() const {
  return buffer_tail_ - tail_;
}

size_t SingleArenaBufferAllocator::GetAvailableMemory(size_t alignment) const {
  uint8_t* const aligned_temp = AlignPointerUp(temp_, alignment);
  uint8_t* const aligned_tail = AlignPointerDown(tail_, alignment);
  return aligned_tail - aligned_temp;
}

size_t SingleArenaBufferAllocator::GetUsedBytes() const {
  return GetPersistentUsedBytes() + GetNonPersistentUsedBytes();
}

size_t SingleArenaBufferAllocator::GetBufferSize() const {
  return buffer_tail_ - buffer_head_;
}

uint8_t* SingleArenaBufferAllocator::head() const { return head_; }

uint8_t* SingleArenaBufferAllocator::tail() const { return tail_; }

}  // namespace tflite
