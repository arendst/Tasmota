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

#include "tensorflow/lite/micro/micro_context.h"

#include <cstdarg>
#include <cstddef>
#include <cstdint>

#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_log.h"

namespace tflite {
MicroContext::MicroContext(MicroAllocator* allocator, const Model* model,
                           MicroGraph* graph)
    : allocator_(*allocator), graph_(*graph), model_(model) {}

MicroContext::~MicroContext() {}

void* MicroContext::AllocatePersistentBuffer(size_t bytes) {
  return allocator_.AllocatePersistentBuffer(bytes);
}

TfLiteStatus MicroContext::RequestScratchBufferInArena(size_t bytes,
                                                       int* buffer_idx) {
  return allocator_.RequestScratchBufferInArena(
      bytes, graph_.GetCurrentSubgraphIndex(), buffer_idx);
}

void* MicroContext::GetScratchBuffer(int buffer_idx) {
  ScratchBufferHandle* handle = scratch_buffer_handles_ + buffer_idx;
  return handle->data;
}

TfLiteTensor* MicroContext::AllocateTempTfLiteTensor(int tensor_idx) {
  return allocator_.AllocateTempTfLiteTensor(model_, graph_.GetAllocations(),
                                             tensor_idx,
                                             graph_.GetCurrentSubgraphIndex());
}

int MicroContext::GetTensorIndex(int index, int max_size,
                                 const int* tensor_indices) {
  if (index >= 0 && index < max_size) {
    const int tensor_index = tensor_indices[index];
    if (tensor_index != kTfLiteOptionalTensor) {
      return tensor_index;
    }
  }
  return -1;
}

TfLiteTensor* MicroContext::AllocateTempInputTensor(const TfLiteNode* node,
                                                    int index) {
  const int tensor_index =
      GetTensorIndex(index, node->inputs->size, node->inputs->data);
  if (tensor_index < 0) {
    return nullptr;
  }
  return AllocateTempTfLiteTensor(tensor_index);
}

TfLiteTensor* MicroContext::AllocateTempOutputTensor(const TfLiteNode* node,
                                                     int index) {
  const int tensor_index =
      GetTensorIndex(index, node->outputs->size, node->outputs->data);
  if (tensor_index < 0) {
    return nullptr;
  }
  return AllocateTempTfLiteTensor(tensor_index);
}

TfLiteTensor* MicroContext::AllocateTempIntermediateTensor(
    const TfLiteNode* node, int index) {
  const int tensor_index = GetTensorIndex(index, node->intermediates->size,
                                          node->intermediates->data);
  if (tensor_index < 0) {
    return nullptr;
  }
  return AllocateTempTfLiteTensor(tensor_index);
}

void MicroContext::DeallocateTempTfLiteTensor(TfLiteTensor* tensor) {
  return allocator_.DeallocateTempTfLiteTensor(tensor);
}

TfLiteEvalTensor* MicroContext::GetEvalTensor(int tensor_idx) {
  return &graph_.GetAllocations()[graph_.GetCurrentSubgraphIndex()]
              .tensors[tensor_idx];
}

void MicroContext::SetScratchBufferHandles(
    ScratchBufferHandle* scratch_buffer_handles) {
  scratch_buffer_handles_ = scratch_buffer_handles;
}

TfLiteStatus MicroContext::set_external_context(
    void* external_context_payload) {
  if (external_context_payload == nullptr ||
      external_context_payload_ != nullptr) {
    MicroPrintf(
        "Attempting to set external context to %x but it was %x already",
        external_context_payload, external_context_payload_);
    return kTfLiteError;
  }

  external_context_payload_ = external_context_payload;
  return kTfLiteOk;
}

void MicroContextReportOpError(struct TfLiteContext* context,
                               const char* format, ...) {
  va_list args;
  va_start(args, format);
  GetMicroErrorReporter()->Report(format, args);
  va_end(args);
}

}  // namespace tflite
