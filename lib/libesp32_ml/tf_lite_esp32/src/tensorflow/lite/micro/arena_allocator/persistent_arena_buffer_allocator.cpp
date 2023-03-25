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
#include "tensorflow/lite/micro/arena_allocator/persistent_arena_buffer_allocator.h"

#include "tensorflow/lite/micro/memory_helpers.h"
#include "tensorflow/lite/micro/micro_log.h"

namespace tflite {

PersistentArenaBufferAllocator::PersistentArenaBufferAllocator(
    uint8_t* buffer, size_t buffer_size)
    : buffer_head_(buffer),
      buffer_tail_(buffer + buffer_size),
      tail_temp_(buffer_tail_) {}

PersistentArenaBufferAllocator::~PersistentArenaBufferAllocator() {}

uint8_t* PersistentArenaBufferAllocator::AllocatePersistentBuffer(
    size_t size, size_t alignment) {
  uint8_t* const aligned_result =
      AlignPointerDown(tail_temp_ - size, alignment);
  if (aligned_result < buffer_head_) {
#ifndef TF_LITE_STRIP_ERROR_STRINGS
    const size_t missing_memory = buffer_head_ - aligned_result;
    MicroPrintf(
        "Failed to allocate tail memory. Requested: %u, "
        "available %u, missing: %u",
        size, size - missing_memory, missing_memory);
#endif
    return nullptr;
  }
  tail_temp_ = aligned_result;
  return aligned_result;
}

size_t PersistentArenaBufferAllocator::GetPersistentUsedBytes() const {
  return buffer_tail_ - tail_temp_;
}

}  // namespace tflite
