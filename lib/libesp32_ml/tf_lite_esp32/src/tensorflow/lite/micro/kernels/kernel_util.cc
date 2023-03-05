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

#include "tensorflow/lite/micro/kernels/kernel_util.h"

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/micro/memory_helpers.h"
#include "tensorflow/lite/micro/micro_log.h"

namespace tflite {
namespace micro {

namespace {

int ValidateTensorIndexing(const TfLiteContext* context, int index,
                           int max_size, const int* tensor_indices) {
  if (index >= 0 && index < max_size) {
    const int tensor_index = tensor_indices[index];
    if (tensor_index != kTfLiteOptionalTensor) {
      return tensor_index;
    }
  }
  return -1;
}

}  // namespace

TfLiteRegistration RegisterOp(
    void* (*init)(TfLiteContext* context, const char* buffer, size_t length),
    TfLiteStatus (*prepare)(TfLiteContext* context, TfLiteNode* node),
    TfLiteStatus (*invoke)(TfLiteContext* context, TfLiteNode* node),
    void (*free)(TfLiteContext* context, void* buffer)) {
  return {/*init=*/init,
          /*free=*/free,
          /*prepare=*/prepare,
          /*invoke=*/invoke,
          /*profiling_string=*/nullptr,
          /*builtin_code=*/0,
          /*custom_name=*/nullptr,
          /*version=*/0,
          /*registration_external=*/nullptr};
}

// Returns a mutable tensor for a given input index. is_variable must be checked
// during prepare when the full TfLiteTensor is available.
TfLiteEvalTensor* GetMutableEvalInput(const TfLiteContext* context,
                                      const TfLiteNode* node, int index) {
  TFLITE_DCHECK(context != nullptr);
  TFLITE_DCHECK(node != nullptr);
  const int tensor_index = ValidateTensorIndexing(
      context, index, node->inputs->size, node->inputs->data);

  if (tensor_index < 0) {
    return nullptr;
  }

  return context->GetEvalTensor(context, node->inputs->data[index]);
}

// Returns the TfLiteEvalTensor struct for a given input index in a node.
const TfLiteEvalTensor* GetEvalInput(const TfLiteContext* context,
                                     const TfLiteNode* node, int index) {
  return GetMutableEvalInput(context, node, index);
}

// Returns the TfLiteEvalTensor struct for a given output index in a node.
TfLiteEvalTensor* GetEvalOutput(const TfLiteContext* context,
                                const TfLiteNode* node, int index) {
  TFLITE_DCHECK(context != nullptr);
  TFLITE_DCHECK(node != nullptr);
  return context->GetEvalTensor(context, node->outputs->data[index]);
}

bool HaveSameShapes(const TfLiteEvalTensor* input1,
                    const TfLiteEvalTensor* input2) {
  TFLITE_DCHECK(input1 != nullptr);
  TFLITE_DCHECK(input2 != nullptr);
  return TfLiteIntArrayEqual(input1->dims, input2->dims);
}

const RuntimeShape GetTensorShape(const TfLiteEvalTensor* tensor) {
  if (tensor == nullptr || tensor->dims == nullptr) {
    return RuntimeShape();
  }
  TfLiteIntArray* dims = tensor->dims;
  const int dims_size = dims->size;
  const int32_t* dims_data = reinterpret_cast<const int32_t*>(dims->data);
  return RuntimeShape(dims_size, dims_data);
}

PaddingType RuntimePaddingType(TfLitePadding padding) {
  switch (padding) {
    case TfLitePadding::kTfLitePaddingSame:
      return PaddingType::kSame;
    case TfLitePadding::kTfLitePaddingValid:
      return PaddingType::kValid;
    case TfLitePadding::kTfLitePaddingUnknown:
    default:
      return PaddingType::kNone;
  }
}

// Relocate tensor dims from FlatBuffer to the persistent storage arena.
// The old dims data is copied to the new storage area.
// The tensor and eval_tensor must be the same tensor.
// Only use during Prepare phase.
TfLiteStatus CreateWritableTensorDimsWithCopy(TfLiteContext* context,
                                              TfLiteTensor* tensor,
                                              TfLiteEvalTensor* eval_tensor) {
  TF_LITE_ENSURE(context, tensor != nullptr);
  TF_LITE_ENSURE(context, eval_tensor != nullptr);
  TF_LITE_ENSURE(context, context->AllocatePersistentBuffer != nullptr);
  int ranks = tensor->dims->size;
  size_t alloc_size = TfLiteIntArrayGetSizeInBytes(ranks);
  TfLiteIntArray* new_dims = static_cast<TfLiteIntArray*>(
      context->AllocatePersistentBuffer(context, alloc_size));
  TfLiteIntArray* old_dims = tensor->dims;
  new_dims->size = ranks;
  tensor->dims = new_dims;
  eval_tensor->dims = new_dims;
  for (int i = 0; i < ranks; i++) {
    new_dims->data[i] = old_dims->data[i];
  }

  return kTfLiteOk;
}

// Verify that both tensors have the same type and size, then return the size
// of both tensors in bytes if they are the same, or -1 if they are different.
size_t ValidateAndGetTensorSizes(const TfLiteEvalTensor* tensor1,
                                 const TfLiteEvalTensor* tensor2) {
  TFLITE_DCHECK(tensor1->type == tensor2->type);
  size_t tensor1_size = 0;
  size_t tensor2_size = 0;
  TfLiteEvalTensorByteLength(tensor1, &tensor1_size);
  TfLiteEvalTensorByteLength(tensor2, &tensor2_size);
  return (tensor1_size == tensor2_size) ? tensor1_size : -1;
}

TfLiteStatus CopyOpInputsToOpOutputs(TfLiteContext* context, TfLiteNode* node) {
  TF_LITE_ENSURE(context, node->inputs->size == node->outputs->size);
  for (int i = 0; i < node->inputs->size; i++) {
    const TfLiteEvalTensor* input =
        tflite::micro::GetEvalInput(context, node, i);
    TfLiteEvalTensor* output = tflite::micro::GetEvalOutput(context, node, i);
    int bytes = ValidateAndGetTensorSizes(input, output);
    TF_LITE_ENSURE(context, bytes >= 0);
    memcpy(output->data.raw, input->data.raw, bytes);
  }
  return kTfLiteOk;
}

//  Args:
//    1. int8_t tensor_data - int8_t buffer of unknown size who's data you'd
//    like
//  to print
//    2. int n_btyes -  a small int representing number of bytes you want to
//    print
//  to debug output. It should always be <= tensor_data's size.
//    3. prefix - optional message you'd like to print before printing bytes
//
//  Purpose:
//    Function takes in paramaters above and prints n_bytes bytes from the
//  tensor_data buffer. This can be use to debug  the output of a model and it's
//  op.

void PrintNBytes(const int8_t* tensor_data, int n_bytes, const char* prefix) {
  if (prefix != nullptr) {
    MicroPrintf("%s", prefix);
  }

  for (int i = 0; i < n_bytes; ++i) {
    MicroPrintf(" %x", tensor_data[i]);
  }
  MicroPrintf("\n");
}

// same as the PrintNBytes above but the buffer needs to be extracted out of the
// TfLiteEvalTensor*
void PrintNBytes(const TfLiteEvalTensor* tensor, int n_bytes,
                 const char* prefix) {
  const int8_t* tensor_data = tflite::micro::GetTensorData<int8_t>(tensor);
  PrintNBytes(tensor_data, n_bytes, prefix);
}

// same as the PrintNBytes above but the buffer needs to be extracted out of the
// TfLiteEvalTensor*
void PrintNBytes(const TfLiteTensor* tensor, int n_bytes, const char* prefix) {
  const int8_t* tensor_data = tflite::GetTensorData<int8_t>(tensor);
  PrintNBytes(tensor_data, n_bytes, prefix);
}

TfLiteStatus CopyOpInputsToSubgraphInputs(TfLiteContext* context,
                                          TfLiteNode* node,
                                          MicroGraph* graph_info,
                                          int subgraph_idx,
                                          int first_tensor_idx) {
  TF_LITE_ENSURE(context,
                 static_cast<size_t>(node->inputs->size - first_tensor_idx) ==
                     graph_info->NumSubgraphInputs(subgraph_idx));
  for (int i = 0; i < node->inputs->size - first_tensor_idx; i++) {
    const TfLiteEvalTensor* input =
        tflite::micro::GetEvalInput(context, node, i + first_tensor_idx);
    TfLiteEvalTensor* subgraph_input =
        graph_info->GetSubgraphInput(subgraph_idx, i);
    int bytes = ValidateAndGetTensorSizes(input, subgraph_input);
    TF_LITE_ENSURE(context, bytes >= 0);
    memcpy(subgraph_input->data.raw, input->data.raw, bytes);
  }
  return kTfLiteOk;
}

TfLiteStatus CopyOpOutputsToSubgraphInputs(TfLiteContext* context,
                                           TfLiteNode* node,
                                           MicroGraph* graph_info,
                                           int subgraph_idx) {
  TF_LITE_ENSURE(context, static_cast<size_t>(node->outputs->size) ==
                              graph_info->NumSubgraphInputs(subgraph_idx));
  for (int i = 0; i < node->outputs->size; i++) {
    TfLiteEvalTensor* output = tflite::micro::GetEvalOutput(context, node, i);
    TfLiteEvalTensor* subgraph_input =
        graph_info->GetSubgraphInput(subgraph_idx, i);
    int bytes = ValidateAndGetTensorSizes(output, subgraph_input);
    TF_LITE_ENSURE(context, bytes >= 0);
    memcpy(subgraph_input->data.raw, output->data.raw, bytes);
  }
  return kTfLiteOk;
}

TfLiteStatus CopySubgraphOutputsToOpOutputs(TfLiteContext* context,
                                            TfLiteNode* node,
                                            MicroGraph* graph_info,
                                            int subgraph_idx) {
  TF_LITE_ENSURE(context, static_cast<size_t>(node->outputs->size) ==
                              graph_info->NumSubgraphOutputs(subgraph_idx));
  for (int i = 0; i < node->outputs->size; i++) {
    TfLiteEvalTensor* output = tflite::micro::GetEvalOutput(context, node, i);
    TfLiteEvalTensor* subgraph_output =
        graph_info->GetSubgraphOutput(subgraph_idx, i);
    int bytes = ValidateAndGetTensorSizes(output, subgraph_output);
    TF_LITE_ENSURE(context, bytes >= 0);
    memcpy(output->data.raw, subgraph_output->data.raw, bytes);
  }
  return kTfLiteOk;
}

}  // namespace micro
}  // namespace tflite
