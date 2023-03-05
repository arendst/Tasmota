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

#ifndef TFLITE_MICRO_TENSORFLOW_LITE_MICRO_MICRO_RESOURCE_H_
#define TFLITE_MICRO_TENSORFLOW_LITE_MICRO_MICRO_RESOURCE_H_

#include <cstdint>

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/micro/micro_allocator.h"

namespace tflite {

class MicroResourceVariables {
 public:
  // Create
  static MicroResourceVariables* Create(MicroAllocator* allocator,
                                        int num_variables);

  // Creates a resource variable if none is available for the given container
  // and shared name pair. Returns the resource ID corresponding to the
  // container and shared name pair. If allocation fails, the returned resource
  // ID will be negative. The the container and shared_name must outlive this
  // class.
  int CreateIdIfNoneFound(const char* container, const char* shared_name);

  // Read the resource buffer associated with the given ID into the given
  // tensor.
  TfLiteStatus Read(int id, const TfLiteEvalTensor* tensor);

  // Allocates the resource buffer if none has been allocated, based on the
  // length of the input tensor. Copies input tensor contents to the resource
  // buffer.
  TfLiteStatus Allocate(int id, TfLiteContext* context,
                        const TfLiteTensor* tensor);

  // Copies input tensor contents to the resource buffer.
  // AllocateResourceVariable with a TFLite tensor must have been called first
  // in order to allocate the resource buffer.
  TfLiteStatus Assign(int id, const TfLiteEvalTensor* tensor);

  // Zeros out all resource buffers.
  TfLiteStatus ResetAll();

 private:
  int FindId(const char* container, const char* shared_name);

  // Micro resource contains the mapping between resource container/name strings
  // and resouce IDs. Each resource ID corresponds to a resource buffer pointer.
  // The resouce ID is created during the VAR_HANDLE operator preparation stage.
  // The resource buffer pointer is created during ASSIGN_VARIABLE preparation
  // stage based on the size of the TFLiteTensor being assigned.
  struct MicroResourceVariable {
    const char* container;
    const char* shared_name;
    void* resource_buffer;

    // This is only for verifying read size.
    size_t bytes;
  };

  MicroResourceVariables(MicroResourceVariable* variables,
                         int max_variable_count)
      : resource_variables_(variables),
        max_variable_count_(max_variable_count),
        num_resource_variables_(0) {}

  MicroResourceVariable* resource_variables_;
  int max_variable_count_;
  int num_resource_variables_;
};

}  // namespace tflite

#endif  // TFLITE_MICRO_TENSORFLOW_LITE_MICRO_MICRO_RESOURCE_H_
