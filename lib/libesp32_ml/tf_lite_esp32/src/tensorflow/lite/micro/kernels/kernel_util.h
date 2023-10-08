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

#ifndef TENSORFLOW_LITE_MICRO_KERNELS_KERNEL_UTIL_H_
#define TENSORFLOW_LITE_MICRO_KERNELS_KERNEL_UTIL_H_

#include <cstdint>

#include "tensorflow/lite/c/builtin_op_data.h"
#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/kernels/internal/compatibility.h"
#include "tensorflow/lite/kernels/internal/tensor_ctypes.h"
#include "tensorflow/lite/kernels/internal/types.h"
#include "tensorflow/lite/micro/micro_context.h"

namespace tflite {
namespace micro {

TfLiteRegistration RegisterOp(
    void* (*init)(TfLiteContext* context, const char* buffer, size_t length),
    TfLiteStatus (*prepare)(TfLiteContext* context, TfLiteNode* node),
    TfLiteStatus (*invoke)(TfLiteContext* context, TfLiteNode* node),
    void (*free)(TfLiteContext* context, void* buffer) = nullptr);

// Prints out n bytes in a int8_t buffer as hex
void PrintNBytes(const int8_t* tensor_data, int n_bytes,
                 const char* prefix = nullptr);

// Prints out the the n bytes in a TfLiteEvalTensor as hex
void PrintNBytes(const TfLiteEvalTensor* tensor, int n_bytes,
                 const char* prefix = nullptr);

// Prints out the the n bytes in a TfLiteTensor as hex
void PrintNBytes(const TfLiteTensor* tensor, int n_bytes,
                 const char* prefix = nullptr);

// Returns a mutable tensor for a given input index. is_variable must be checked
// during prepare when the full TfLiteTensor is available.
TfLiteEvalTensor* GetMutableEvalInput(const TfLiteContext* context,
                                      const TfLiteNode* node, int index);

// Returns the TfLiteEvalTensor struct for a given input index in a node.
const TfLiteEvalTensor* GetEvalInput(const TfLiteContext* context,
                                     const TfLiteNode* node, int index);

// Returns the TfLiteEvalTensor struct for a given output index in a node.
TfLiteEvalTensor* GetEvalOutput(const TfLiteContext* context,
                                const TfLiteNode* node, int index);

// Returns data for a TfLiteEvalTensor struct that are expected to exist.
template <typename T>
T* GetTensorData(TfLiteEvalTensor* tensor) {
  TFLITE_DCHECK(tensor != nullptr);
  return reinterpret_cast<T*>(tensor->data.raw);
}

// Returns const data for a TfLiteEvalTensor struct that are expected to exist.
template <typename T>
const T* GetTensorData(const TfLiteEvalTensor* tensor) {
  TFLITE_DCHECK(tensor != nullptr);
  return reinterpret_cast<const T*>(tensor->data.raw);
}

// Returns data for a TfLiteEvalTensor struct that could be null.
template <typename T>
T* GetOptionalTensorData(TfLiteEvalTensor* tensor) {
  return tensor == nullptr ? nullptr : reinterpret_cast<T*>(tensor->data.raw);
}

// Returns const data for a TfLiteEvalTensor struct that could be null.
template <typename T>
const T* GetOptionalTensorData(const TfLiteEvalTensor* tensor) {
  return tensor == nullptr ? nullptr
                           : reinterpret_cast<const T*>(tensor->data.raw);
}

// Returns the shape of a TfLiteEvalTensor struct.
const RuntimeShape GetTensorShape(const TfLiteEvalTensor* tensor);

// Return true if the given tensors have the same shape.
bool HaveSameShapes(const TfLiteEvalTensor* input1,
                    const TfLiteEvalTensor* input2);

PaddingType RuntimePaddingType(TfLitePadding padding);

// Relocate tensor dims from FlatBuffer to the persistent storage arena.
// The old dims data is copied to the new storage area.
// The tensor and eval_tensor must be the same tensor.
// Only use during Prepare phase.
TfLiteStatus CreateWritableTensorDimsWithCopy(TfLiteContext* context,
                                              TfLiteTensor* tensor,
                                              TfLiteEvalTensor* eval_tensor);

// Copy all op input tensors to op output tensors. Requires all op input tensor
// shapes and types to be identical to op output tensor shapes and types.
TfLiteStatus CopyOpInputsToOpOutputs(TfLiteContext* context, TfLiteNode* node);

// Copy all op input tensors to subgraph input tensors. Requires all op input
// tensor shapes and types to be identical to subgraph input tensor shapes and
// types.
TfLiteStatus CopyOpInputsToSubgraphInputs(TfLiteContext* context,
                                          TfLiteNode* node,
                                          MicroGraph* graph_info,
                                          int subgraph_idx,
                                          int first_tensor_idx);

// Copy all op output tensors to subgraph input tensors. Requires all op output
// tensor shapes and types to be identical to subgraph input tensor shapes and
// types.
TfLiteStatus CopyOpOutputsToSubgraphInputs(TfLiteContext* context,
                                           TfLiteNode* node,
                                           MicroGraph* graph_info,
                                           int subgraph_idx);

// Copy all subgraph output tensors to op outputs. Requires all subgraph output
// tensor shapes and types to be identical to op output tensor shapes and types.
TfLiteStatus CopySubgraphOutputsToOpOutputs(TfLiteContext* context,
                                            TfLiteNode* node,
                                            MicroGraph* graph_info,
                                            int subgraph_idx);

}  // namespace micro
}  // namespace tflite

#endif  // TENSORFLOW_LITE_MICRO_KERNELS_KERNEL_UTIL_H_
