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

#include "tensorflow/lite/micro/test_helper_custom_ops.h"

#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <new>

#include "third_party/flatbuffers/include/flatbuffers/flatbuffers.h"
#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/kernels/internal/compatibility.h"
#include "tensorflow/lite/kernels/internal/tensor_ctypes.h"
#include "tensorflow/lite/kernels/kernel_util.h"
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/kernels/kernel_util.h"
#include "tensorflow/lite/micro/micro_utils.h"
#include "tensorflow/lite/schema/schema_generated.h"

// TODO(b/170464050): Use TFLM test only version of schema_utils.

namespace tflite {
namespace testing {

const TfLiteRegistration* PackerOp::getRegistration() {
  return GetMutableRegistration();
}

TfLiteRegistration* PackerOp::GetMutableRegistration() {
  static TfLiteRegistration r;
  r.init = Init;
  r.prepare = Prepare;
  r.invoke = Invoke;
  r.free = Free;
  return &r;
}

void* PackerOp::Init(TfLiteContext* context, const char* buffer,
                     size_t length) {
  freed_ = false;
  // Do nothing.
  return nullptr;
}

void PackerOp::Free(TfLiteContext* context, void* buffer) { freed_ = true; }

TfLiteStatus PackerOp::Prepare(TfLiteContext* context, TfLiteNode* node) {
  return kTfLiteOk;
}

TfLiteStatus PackerOp::Invoke(TfLiteContext* context, TfLiteNode* node) {
  const TfLiteEvalTensor* input1 =
      tflite::micro::GetEvalInput(context, node, 0);
  TF_LITE_ENSURE(context, input1 != nullptr);
  const int32_t* input1_data = input1->data.i32;
  TF_LITE_ENSURE_EQ(context, input1->dims->size, 1);
  const int32_t input1_len = input1->dims->data[0];

  const TfLiteEvalTensor* input2 =
      tflite::micro::GetEvalInput(context, node, 1);
  TF_LITE_ENSURE(context, input2 != nullptr);
  const int32_t* input2_data = input2->data.i32;
  TF_LITE_ENSURE_EQ(context, input2->dims->size, 1);
  const int32_t input2_len = input2->dims->data[0];

  TfLiteEvalTensor* output = tflite::micro::GetEvalOutput(context, node, 0);
  TF_LITE_ENSURE(context, output != nullptr);
  int32_t* output_data = output->data.i32;
  int32_t output_len = output->dims->data[0];

  // Fill output with input: first with the first tensor, then with the second
  // tensor up to the size of the output tensor.
  int cnt = 0;
  int i;
  for (i = 0; i < input1_len && cnt < output_len; i++, cnt++) {
    output_data[cnt] = input1_data[i];
  }
  if (cnt >= output_len) {
    return kTfLiteOk;
  }

  for (i = 0; i < input2_len && cnt < output_len; i++, cnt++) {
    output_data[cnt] = input2_data[i];
  }
  if (cnt >= output_len) {
    return kTfLiteOk;
  }

  for (; cnt < output_len; cnt++) {
    output_data[cnt] = 0;
  }
  return kTfLiteOk;
}

bool PackerOp::freed_ = false;

}  // namespace testing
}  // namespace tflite
