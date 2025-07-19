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

#ifndef TENSORFLOW_LITE_MICRO_MICRO_CONTEXT_H_
#define TENSORFLOW_LITE_MICRO_MICRO_CONTEXT_H_

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/micro/micro_allocator.h"
#include "tensorflow/lite/micro/micro_graph.h"

namespace tflite {
// MicroContext is eventually going to become the API between TFLM and the
// kernels, replacing all the functions in TfLiteContext. The end state is code
// kernels to have code like:
//
// MicroContext* micro_context = GetMicroContext(context);
// micro_context-><TFLM kernel API>
class MicroContext {
 public:
  // Does not take any ownership, and all pointers must refer to valid objects
  // that outlive the one constructed.
  explicit MicroContext(MicroAllocator* allocator, const Model* model,
                        MicroGraph* graph);
  virtual ~MicroContext();

  // Allocate persistent buffer which has the same life time as the interpreter.
  // Returns nullptr on failure.
  // The memory is allocated from the tail.
  // This method is only available in Init or Prepare stage.
  // Virtual so that it can be faked for kernel tests.
  virtual void* AllocatePersistentBuffer(size_t bytes);

  // Request a scratch buffer in the arena through static memory planning.
  // This method is only available in Prepare stage and the buffer is allocated
  // by the interpreter between Prepare and Eval stage. In Eval stage,
  // GetScratchBuffer API can be used to fetch the address.
  // Virtual so that it can be faked for kernel tests.
  virtual TfLiteStatus RequestScratchBufferInArena(size_t bytes,
                                                   int* buffer_idx);

  // Get the scratch buffer pointer.
  // This method is only available in Eval stage.
  // Virtual so that it can be faked for kernel tests.
  virtual void* GetScratchBuffer(int buffer_idx);

  // Returns a temporary TfLiteTensor struct for a given index.
  // Virtual so that it can be faked for kernel tests.
  virtual TfLiteTensor* AllocateTempTfLiteTensor(int tensor_idx);

  // Returns a temporary TfLiteTensor struct for the specified input tensor of a
  // given mode. This is the recommended API over the deprecated
  // GetInput/GetInputSafe to get a temp input tensor. The returned tensor shall
  // be freed via calling DeallocateTempTfLiteTensor.
  virtual TfLiteTensor* AllocateTempInputTensor(const TfLiteNode* node,
                                                int index);

  // Returns a temporary TfLiteTensor struct for the specified output tensor of
  // a given mode. This is the recommended API over the deprecated
  // GetOutput/GetOutputSafe to get a temp output tensor. The returned tensor
  // shall be freed via calling DeallocateTempTfLiteTensor.
  virtual TfLiteTensor* AllocateTempOutputTensor(const TfLiteNode* node,
                                                 int index);

  // Returns a temporary TfLiteTensor struct for the specified intermediate
  // tensor of a given mode. This is the recommended API over the deprecated
  // GetIntermediates/GetIntermediatesSafe to get a temp intermediate tensor.
  // The returned tensor shall be freed via calling DeallocateTempTfLiteTensor.
  virtual TfLiteTensor* AllocateTempIntermediateTensor(const TfLiteNode* node,
                                                       int index);

  // Deallocates a temp TfLiteTensor.
  // Virtual so that it can be faked for kernel tests.
  virtual void DeallocateTempTfLiteTensor(TfLiteTensor* tensor);

  // Returns a TfLiteEvalTensor struct for a given index.
  // Virtual so that it can be faked for kernel tests.
  virtual TfLiteEvalTensor* GetEvalTensor(int tensor_idx);

  // Does not take ownership of the pointer and the pointer must refer to valid
  // an object that outlive this class instance.
  // This can only be called once to set one external context.
  TfLiteStatus set_external_context(void* external_context_payload);

  void* external_context() { return external_context_payload_; }

  MicroGraph& graph() { return graph_; }

  // Sets the pointer to a list of ScratchBufferHandle instances.
  // Not API between TFLM and kernels. Primarily used by the framework for
  // housekeeping in MicroContext.
  void SetScratchBufferHandles(ScratchBufferHandle* scratch_buffer_handles);

 private:
  // Return the tensor index as tensor_indices[index]. tensor_indices is of
  // max_size. Return -1 if index is not in the valid range of tensor_indices.
  int GetTensorIndex(int index, int max_size, const int* tensor_indices);

  MicroAllocator& allocator_;
  MicroGraph& graph_;
  const Model* model_;

  ScratchBufferHandle* scratch_buffer_handles_ = nullptr;
  void* external_context_payload_ = nullptr;

  TF_LITE_REMOVE_VIRTUAL_DELETE
};

inline MicroContext* GetMicroContext(const struct TfLiteContext* context) {
  return reinterpret_cast<MicroContext*>(context->impl_);
}

// Deprecated API. Prefer to using the MicroContext API directly from the
// kernels.
// TODO(b/213010668): migrate all existing kernels to use MicroContext, delete
// these functions, and remove corresponding members from the TfLiteContext
// struct for TFLM.
inline void* MicroContextAllocatePersistentBuffer(TfLiteContext* ctx,
                                                  size_t bytes) {
  return GetMicroContext(ctx)->AllocatePersistentBuffer(bytes);
}
inline TfLiteStatus MicroContextRequestScratchBufferInArena(TfLiteContext* ctx,
                                                            size_t bytes,
                                                            int* buffer_idx) {
  return GetMicroContext(ctx)->RequestScratchBufferInArena(bytes, buffer_idx);
}
inline void* MicroContextGetScratchBuffer(TfLiteContext* ctx, int buffer_idx) {
  return GetMicroContext(ctx)->GetScratchBuffer(buffer_idx);
}
inline TfLiteTensor* MicroContextGetTensor(const struct TfLiteContext* context,
                                           int tensor_idx) {
  return GetMicroContext(context)->AllocateTempTfLiteTensor(tensor_idx);
}
inline TfLiteEvalTensor* MicroContextGetEvalTensor(
    const struct TfLiteContext* context, int tensor_idx) {
  return GetMicroContext(context)->GetEvalTensor(tensor_idx);
}
inline TfLiteExternalContext* MicroContextGetExternalContext(
    TfLiteContext* context, TfLiteExternalContextType unused) {
  return reinterpret_cast<TfLiteExternalContext*>(
      GetMicroContext(context)->external_context());
}

// Requests that an error be reported with format string msg.
void MicroContextReportOpError(struct TfLiteContext* context,
                               const char* format, ...);

}  // namespace tflite

#endif  // TENSORFLOW_LITE_MICRO_MICRO_CONTEXT_H_
