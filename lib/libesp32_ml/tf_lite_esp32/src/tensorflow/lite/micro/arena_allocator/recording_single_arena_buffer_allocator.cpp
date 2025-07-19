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

#include "tensorflow/lite/micro/arena_allocator/recording_single_arena_buffer_allocator.h"

#include <new>

#include "tensorflow/lite/kernels/internal/compatibility.h"

namespace tflite {

RecordingSingleArenaBufferAllocator::RecordingSingleArenaBufferAllocator(
    uint8_t* buffer_head, size_t buffer_size)
    : SingleArenaBufferAllocator(buffer_head, buffer_size),
      requested_head_bytes_(0),
      requested_tail_bytes_(0),
      used_bytes_(0),
      alloc_count_(0) {}

RecordingSingleArenaBufferAllocator::~RecordingSingleArenaBufferAllocator() {}

RecordingSingleArenaBufferAllocator*
RecordingSingleArenaBufferAllocator::Create(uint8_t* buffer_head,
                                            size_t buffer_size) {
  TFLITE_DCHECK(buffer_head != nullptr);
  RecordingSingleArenaBufferAllocator tmp =
      RecordingSingleArenaBufferAllocator(buffer_head, buffer_size);

  uint8_t* allocator_buffer = tmp.AllocatePersistentBuffer(
      sizeof(RecordingSingleArenaBufferAllocator),
      alignof(RecordingSingleArenaBufferAllocator));
  // Use the default copy constructor to populate internal states.
  return new (allocator_buffer) RecordingSingleArenaBufferAllocator(tmp);
}

size_t RecordingSingleArenaBufferAllocator::GetRequestedBytes() const {
  return requested_head_bytes_ + requested_tail_bytes_;
}

size_t RecordingSingleArenaBufferAllocator::GetUsedBytes() const {
  return used_bytes_;
}

size_t RecordingSingleArenaBufferAllocator::GetAllocatedCount() const {
  return alloc_count_;
}

TfLiteStatus RecordingSingleArenaBufferAllocator::ResizeBuffer(
    uint8_t* resizable_buf, size_t size, size_t alignment) {
  const uint8_t* previous_head = head();
  TfLiteStatus status =
      SingleArenaBufferAllocator::ResizeBuffer(resizable_buf, size, alignment);
  if (status == kTfLiteOk) {
    used_bytes_ += head() - previous_head;
    requested_head_bytes_ = size;
  }
  return status;
}

uint8_t* RecordingSingleArenaBufferAllocator::AllocatePersistentBuffer(
    size_t size, size_t alignment) {
  const uint8_t* previous_tail = tail();
  uint8_t* result =
      SingleArenaBufferAllocator::AllocatePersistentBuffer(size, alignment);
  if (result != nullptr) {
    used_bytes_ += previous_tail - tail();
    requested_tail_bytes_ += size;
    alloc_count_++;
  }
  return result;
}

}  // namespace tflite
