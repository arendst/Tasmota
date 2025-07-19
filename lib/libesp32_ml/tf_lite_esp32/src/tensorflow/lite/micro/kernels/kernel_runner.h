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

#ifndef TENSORFLOW_LITE_MICRO_KERNELS_KERNEL_RUNNER_H_
#define TENSORFLOW_LITE_MICRO_KERNELS_KERNEL_RUNNER_H_

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/kernels/internal/compatibility.h"
#include "tensorflow/lite/micro/arena_allocator/single_arena_buffer_allocator.h"
#include "tensorflow/lite/micro/fake_micro_context.h"
#include "tensorflow/lite/micro/mock_micro_graph.h"

namespace tflite {
namespace micro {

// Helper class to perform a simulated kernel (i.e. TfLiteRegistration)
// lifecycle (init, prepare, invoke). All internal allocations are handled by
// this class. Simply pass in the registration, list of required tensors, inputs
// array, outputs array, and any pre-builtin data. Calling Invoke() will
// automatically walk the kernel and outputs will be ready on the TfLiteTensor
// output provided during construction.
class KernelRunner {
 public:
  KernelRunner(const TfLiteRegistration& registration, TfLiteTensor* tensors,
               int tensors_size, TfLiteIntArray* inputs,
               TfLiteIntArray* outputs, void* builtin_data,
               TfLiteIntArray* intermediates = nullptr);

  // Calls init and prepare on the kernel (i.e. TfLiteRegistration) struct. Any
  // exceptions will be DebugLog'd and returned as a status code.
  TfLiteStatus InitAndPrepare(const char* init_data = nullptr,
                              size_t length = 0);

  // Calls init, prepare, and invoke on a given TfLiteRegistration pointer.
  // After successful invoke, results will be available in the output tensor as
  // passed into the constructor of this class.
  TfLiteStatus Invoke();

  // Calls Free on a given TfLiteRegistration pointer(if it's implemented).
  // After successful Free, kTfLiteOk status will be returned. If Free is not
  // implemented for a given kernel kTfLiteError will be returned.
  TfLiteStatus Free();

  // Returns a pointer to the internal MockMicroGraph which KernelRunner uses
  // to stub out MicroGraph methods and track invocations on each subgraph.
  MockMicroGraph* GetMockGraph() { return &mock_micro_graph_; }

  // Returns true if all temp buffer in tests are deallocated.
  // TODO(b/209453859): move this function to private after deallocation checks
  // are enabled for all kernel tests.
  bool ValidateTempBufferDeallocated();

 private:
  static constexpr int kKernelRunnerBufferSize_ = 10000;
  static uint8_t kKernelRunnerBuffer_[kKernelRunnerBufferSize_];

  TfLiteContext context_ = {};
  TfLiteNode node_ = {};
  const TfLiteRegistration& registration_;

  SingleArenaBufferAllocator* allocator_;
  MockMicroGraph mock_micro_graph_;
  FakeMicroContext fake_micro_context_;
};

}  // namespace micro
}  // namespace tflite

#endif  // TENSORFLOW_LITE_MICRO_KERNELS_KERNEL_RUNNER_H_
