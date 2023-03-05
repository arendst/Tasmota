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

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/kernels/internal/quantization_util.h"
#include "tensorflow/lite/kernels/internal/reference/leaky_relu.h"
#include "tensorflow/lite/kernels/internal/reference/process_broadcast_shapes.h"
#include "tensorflow/lite/kernels/internal/types.h"
#include "tensorflow/lite/kernels/kernel_util.h"
#include "tensorflow/lite/micro/kernels/kernel_util.h"
#include "tensorflow/lite/micro/kernels/leaky_relu.h"

namespace tflite {

// Input/output tensor index.
const int kInputTensor = 0;
const int kOutputTensor = 0;

TfLiteStatus CalculateOpDataLeakyRelu(TfLiteContext* context,
                                      TfLiteNode* node) {
  MicroContext* micro_context = GetMicroContext(context);

  TF_LITE_ENSURE_EQ(context, NumInputs(node), 1);
  TF_LITE_ENSURE_EQ(context, NumOutputs(node), 1);
  TfLiteTensor* input =
      micro_context->AllocateTempInputTensor(node, kInputTensor);
  TF_LITE_ENSURE(context, input != nullptr);
  TfLiteTensor* output =
      micro_context->AllocateTempOutputTensor(node, kOutputTensor);
  TF_LITE_ENSURE(context, output != nullptr);
  TF_LITE_ENSURE_TYPES_EQ(context, input->type, output->type);

  if (output->type == kTfLiteInt8 || output->type == kTfLiteInt16) {
    LeakyReluOpData* data = static_cast<LeakyReluOpData*>(node->user_data);
    const auto* params =
        static_cast<TfLiteLeakyReluParams*>(node->builtin_data);

    data->input_zero_point = input->params.zero_point;
    data->output_zero_point = output->params.zero_point;

    int output_shift_alpha;
    double alpha_multiplier = static_cast<double>(input->params.scale) *
                              static_cast<double>(params->alpha) /
                              static_cast<double>(output->params.scale);
    QuantizeMultiplier(alpha_multiplier, &data->output_multiplier_alpha,
                       &output_shift_alpha);
    data->output_shift_alpha = static_cast<int32_t>(output_shift_alpha);

    int output_shift_identity;
    double identity_multiplier = static_cast<double>(input->params.scale) /
                                 static_cast<double>(output->params.scale);
    QuantizeMultiplier(identity_multiplier, &data->output_multiplier_identity,
                       &output_shift_identity);
    data->output_shift_identity = static_cast<int32_t>(output_shift_identity);
  }

  micro_context->DeallocateTempTfLiteTensor(input);
  micro_context->DeallocateTempTfLiteTensor(output);

  return kTfLiteOk;
}

TfLiteStatus LeakyReluPrepare(TfLiteContext* context, TfLiteNode* node) {
  return CalculateOpDataLeakyRelu(context, node);
}

}  // namespace tflite
