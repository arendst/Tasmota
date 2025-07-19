/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

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

#ifndef TENSORFLOW_LITE_MICRO_MICRO_MEMORY_PLANNER_MEMORY_PLANNER_H_
#define TENSORFLOW_LITE_MICRO_MICRO_MEMORY_PLANNER_MEMORY_PLANNER_H_

#include "tensorflow/lite/c/common.h"

namespace tflite {

// Interface class for planning the layout of memory buffers during the
// execution of a graph.
// It's designed to be used by a client that iterates in any order through the
// buffers it wants to lay out, and then calls the getter functions for
// information about the calculated layout. For example:
//
// SomeMemoryPlanner planner;
// planner.AddBuffer(100, 0, 1);  // Buffer 0
// planner.AddBuffer(50, 2, 3);   // Buffer 1
// planner.AddBuffer(50, 2, 3);   // Buffer 2
//
// int offset0;
// TF_EXPECT_OK(planner.GetOffsetForBuffer(0, &offset0));
// int offset1;
// TF_EXPECT_OK(planner.GetOffsetForBuffer(1, &offset1));
// int offset2;
// TF_EXPECT_OK(planner.GetOffsetForBuffer(2, &offset2));
// const int arena_size_needed = planner.GetMaximumMemorySize();
//
// The goal is for applications to be able to experiment with different layout
// strategies without changing their client code, by swapping out classes that
// implement this interface.=
class MicroMemoryPlanner {
 public:
  MicroMemoryPlanner() {}
  virtual ~MicroMemoryPlanner() {}

  // Pass information about a buffer's size and lifetime to the layout
  // algorithm. The order this is called implicitly assigns an index to the
  // result, so the buffer information that's passed into the N-th call of
  // this method will be used as the buffer_index argument to
  // GetOffsetForBuffer().
  virtual TfLiteStatus AddBuffer(int size, int first_time_used,
                                 int last_time_used) = 0;

  // Record details of an offline planned buffer offset we want to place.
  // offline_offset is the buffer offset from the start of the arena.
  // This is to support offline memory planning from the flatbuffer metadata.
  // By default, it returns an error.
  virtual TfLiteStatus AddBuffer(int size, int first_time_used,
                                 int last_time_used, int offline_offset) {
    return kTfLiteError;
  }

  // The largest contiguous block of memory that's needed to hold the layout.
  virtual size_t GetMaximumMemorySize() = 0;
  // How many buffers have been added to the planner.
  virtual int GetBufferCount() = 0;
  // Calculated layout offset for the N-th buffer added to the planner.
  virtual TfLiteStatus GetOffsetForBuffer(int buffer_index, int* offset) = 0;

  // Provides the scratch buffer in case that the memory planner needs it.
  // The lifetime of scratch buffers lifetime lasts until the static memory plan
  // is committed.
  // The default implementation is for the memory planner that does not need
  // scratch buffer and simply returns ok.
  virtual TfLiteStatus Init(unsigned char* scratch_buffer,
                            int scratch_buffer_size) {
    return kTfLiteOk;
  }

  virtual void PrintMemoryPlan() {
    // Default does nothing.
  }
};

}  // namespace tflite

#endif  // TENSORFLOW_LITE_MICRO_MICRO_MEMORY_PLANNER_MEMORY_PLANNER_H_
