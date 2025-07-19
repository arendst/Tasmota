/* Copyright 2021 The TensorFlow Authors. All Rights Reserved.

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

#ifndef TENSORFLOW_LITE_MICRO_MEMORY_PLANNER_MEMORY_PLAN_STRUCT_H_
#define TENSORFLOW_LITE_MICRO_MEMORY_PLANNER_MEMORY_PLAN_STRUCT_H_

#include <stddef.h>
#include <stdint.h>

#include "tensorflow/lite/micro/micro_utils.h"

namespace tflite {

// This is an experimental feature and subjected to change.
// More description is available at
// tensorflow/lite/micro/docs/offline_memory_plan.md.

// Describes a buffer's layout inside an arena. This struct should be kept as
// small as possible for memory footprint sensitive applications and should use
// only primitive fields, making it easy to adjust offline.
struct BufferDescriptor {
  // Starting offset inside an arena for this buffer.
  // Offset is the minimum information needed for the buffer.  The user knows
  // the model and the size of each buffer in order to lay out a valid buffer
  // plan.
  int32_t offset;
};

// A structure describing the lay out of buffers inside an arena.
struct BufferPlan {
  // Number of buffers described in this plan.
  int32_t buffer_count;

  // Each element describes one buffer.
  // Buffer index is implicit by the order of AddBuffer() call.
  // Specifically, indices of activation tensors are 0 … N-1 where N is the
  // number of activation tensors.
  // The rest are based on the order of OP requests.
  //
  // This is a flexible array member and should ideally be
  // arena_entries[]; However, in order to support a variety
  // of compilers (and without needing to add ifdef's), we
  // are implementing the flexible array member with an array of
  // length 1 as the last member of the struct. When the size of a BufferPlan
  // is needed, use the provided SizeOfBufferPlan(buffer_count) that
  // accounts for this implemenatation caveat.
  BufferDescriptor buffer_plan_entries[1];
};

// Returns size of a BufferPlan given a buffer count. This size is compile time
// known if buffer_count is a compile time constant.
constexpr size_t SizeOfBufferPlan(int32_t buffer_count) {
  // Minus 1 because a BufferPlan struct have a BufferDescriptor already.
  // Max to provide a lower bound for the corner case of buffer_count = 0.
  return sizeof(BufferPlan) +
         sizeof(BufferDescriptor) * Max(buffer_count - 1, 0);
}

}  // namespace tflite

#endif  // TENSORFLOW_LITE_MICRO_MEMORY_PLANNER_MEMORY_PLAN_STRUCT_H_
