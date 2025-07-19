/* Copyright 2022 The TensorFlow Authors. All Rights Reserved.

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
#include "tensorflow/lite/kernels/internal/reference/broadcast_args.h"

#include <stdint.h>

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/kernels/internal/tensor_ctypes.h"
#include "tensorflow/lite/kernels/kernel_util.h"
#include "tensorflow/lite/micro/kernels/kernel_util.h"
#include "tensorflow/lite/micro/micro_context.h"

namespace tflite {
namespace {
constexpr int kShape1Tensor = 0;
constexpr int kShape2Tensor = 1;
constexpr int kOutputTensor = 0;

TfLiteStatus BroadcastArgsPrepare(TfLiteContext* context, TfLiteNode* node) {
  TF_LITE_ENSURE(context, NumInputs(node) == 2);
  TF_LITE_ENSURE_EQ(context, NumOutputs(node), 1);

  MicroContext* micro_context = GetMicroContext(context);
  TfLiteTensor* shape1 =
      micro_context->AllocateTempInputTensor(node, kShape1Tensor);
  TfLiteTensor* shape2 =
      micro_context->AllocateTempInputTensor(node, kShape2Tensor);
  TfLiteTensor* output =
      micro_context->AllocateTempOutputTensor(node, kOutputTensor);

  TF_LITE_ENSURE(context,
                 shape1->type == kTfLiteInt32 || shape1->type == kTfLiteInt64);
  TF_LITE_ENSURE_EQ(context, shape1->type, shape2->type);
  TF_LITE_ENSURE_EQ(context, shape1->type, output->type);

  // Ensures the shapes are 1D tensor.
  TF_LITE_ENSURE_EQ(context, NumDimensions(shape1), 1);
  TF_LITE_ENSURE_EQ(context, NumDimensions(shape2), 1);

  // Ensure the shape of the output tensor is compatible
  TF_LITE_ENSURE_EQ(context, NumDimensions(output), 1);

  micro_context->DeallocateTempTfLiteTensor(shape1);
  micro_context->DeallocateTempTfLiteTensor(shape2);
  micro_context->DeallocateTempTfLiteTensor(output);

  return kTfLiteOk;
}

TfLiteStatus BroadcastArgsEval(TfLiteContext* context, TfLiteNode* node) {
  const TfLiteEvalTensor* shape1 =
      micro::GetEvalInput(context, node, kShape1Tensor);
  const TfLiteEvalTensor* shape2 =
      micro::GetEvalInput(context, node, kShape2Tensor);
  TfLiteEvalTensor* output = micro::GetEvalOutput(context, node, kOutputTensor);

  if (output->type == kTfLiteInt32) {
    reference_ops::BroadcastArgs(
        micro::GetTensorShape(shape1), micro::GetTensorData<int32_t>(shape1),
        micro::GetTensorShape(shape2), micro::GetTensorData<int32_t>(shape2),
        micro::GetTensorShape(output), micro::GetTensorData<int32_t>(output));
  } else {
    reference_ops::BroadcastArgs(
        micro::GetTensorShape(shape1), micro::GetTensorData<int64_t>(shape1),
        micro::GetTensorShape(shape2), micro::GetTensorData<int64_t>(shape2),
        micro::GetTensorShape(output), micro::GetTensorData<int64_t>(output));
  }

  return kTfLiteOk;
}

}  // namespace

TfLiteRegistration Register_BROADCAST_ARGS() {
  return tflite::micro::RegisterOp(nullptr, BroadcastArgsPrepare,
                                   BroadcastArgsEval);
}

}  // namespace tflite
