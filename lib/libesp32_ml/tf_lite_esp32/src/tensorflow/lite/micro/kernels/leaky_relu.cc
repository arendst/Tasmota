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

#include "tensorflow/lite/kernels/internal/reference/leaky_relu.h"

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/kernels/internal/quantization_util.h"
#include "tensorflow/lite/kernels/internal/reference/process_broadcast_shapes.h"
#include "tensorflow/lite/kernels/internal/types.h"
#include "tensorflow/lite/kernels/kernel_util.h"
#include "tensorflow/lite/micro/kernels/kernel_util.h"
#include "tensorflow/lite/micro/kernels/leaky_relu.h"
#include "tensorflow/lite/micro/micro_log.h"

namespace tflite {

template <typename T>
void QuantizeLeakyRelu(const LeakyReluOpData& data,
                       const TfLiteEvalTensor* input,
                       TfLiteEvalTensor* output) {
  LeakyReluParams op_params = {};

  op_params.input_offset = data.input_zero_point;
  op_params.output_offset = data.output_zero_point;
  op_params.output_multiplier_alpha = data.output_multiplier_alpha;
  op_params.output_shift_alpha = data.output_shift_alpha;
  op_params.output_multiplier_identity = data.output_multiplier_identity;
  op_params.output_shift_identity = data.output_shift_identity;
  reference_ops::QuantizeLeakyRelu(op_params,
                                   tflite::micro::GetTensorShape(input),
                                   tflite::micro::GetTensorData<T>(input),
                                   tflite::micro::GetTensorShape(output),
                                   tflite::micro::GetTensorData<T>(output));
}

void* LeakyReluInit(TfLiteContext* context, const char* buffer, size_t length) {
  TFLITE_DCHECK(context->AllocatePersistentBuffer != nullptr);
  return context->AllocatePersistentBuffer(context, sizeof(LeakyReluOpData));
}

TfLiteStatus LeakyReluEval(TfLiteContext* context, TfLiteNode* node) {
  const TfLiteEvalTensor* input =
      tflite::micro::GetEvalInput(context, node, kInputTensor);
  TfLiteEvalTensor* output =
      tflite::micro::GetEvalOutput(context, node, kOutputTensor);
  const LeakyReluOpData& data = *static_cast<LeakyReluOpData*>(node->user_data);

  switch (input->type) {
    case kTfLiteFloat32: {
      LeakyReluParams op_params = {};
      const auto* params =
          static_cast<TfLiteLeakyReluParams*>(node->builtin_data);

      op_params.alpha = params->alpha;
      reference_ops::LeakyRelu(op_params, tflite::micro::GetTensorShape(input),
                               tflite::micro::GetTensorData<float>(input),
                               tflite::micro::GetTensorShape(output),
                               tflite::micro::GetTensorData<float>(output));
      return kTfLiteOk;
    } break;
    case kTfLiteInt8: {
      QuantizeLeakyRelu<int8_t>(data, input, output);
      return kTfLiteOk;
    } break;
    case kTfLiteInt16: {
      QuantizeLeakyRelu<int16_t>(data, input, output);
      return kTfLiteOk;
    } break;
    default:
      MicroPrintf("Only float32, int8 are supported by LEAKY_RELU, got %s.",
                  TfLiteTypeGetName(input->type));
      return kTfLiteError;
  }

  return kTfLiteError;
}

TfLiteRegistration Register_LEAKY_RELU() {
  return tflite::micro::RegisterOp(LeakyReluInit, LeakyReluPrepare,
                                   LeakyReluEval);
}

}  // namespace tflite
