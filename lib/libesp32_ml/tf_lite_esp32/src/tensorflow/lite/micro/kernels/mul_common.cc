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
#include "tensorflow/lite/kernels/internal/reference/integer_ops/mul.h"
#include "tensorflow/lite/kernels/internal/reference/mul.h"
#include "tensorflow/lite/kernels/internal/reference/process_broadcast_shapes.h"
#include "tensorflow/lite/kernels/internal/tensor_ctypes.h"
#include "tensorflow/lite/kernels/kernel_util.h"
#include "tensorflow/lite/micro/kernels/kernel_util.h"
#include "tensorflow/lite/micro/kernels/mul.h"
#include "tensorflow/lite/micro/memory_helpers.h"

namespace tflite {

const int kMulInput1Tensor = 0;
const int kMulInput2Tensor = 1;
const int kMulOutputTensor = 0;

void* MulInit(TfLiteContext* context, const char* buffer, size_t length) {
  TFLITE_DCHECK(context->AllocatePersistentBuffer != nullptr);
  return context->AllocatePersistentBuffer(context, sizeof(OpDataMul));
}

TfLiteStatus CalculateOpDataMul(TfLiteContext* context, TfLiteNode* node,
                                TfLiteMulParams* params, OpDataMul* data) {
  MicroContext* micro_context = GetMicroContext(context);

  TfLiteTensor* input1 =
      micro_context->AllocateTempInputTensor(node, kMulInput1Tensor);
  TF_LITE_ENSURE(context, input1 != nullptr);
  TfLiteTensor* input2 =
      micro_context->AllocateTempInputTensor(node, kMulInput2Tensor);
  TF_LITE_ENSURE(context, input2 != nullptr);
  TfLiteTensor* output =
      micro_context->AllocateTempOutputTensor(node, kMulOutputTensor);
  TF_LITE_ENSURE(context, output != nullptr);

  TF_LITE_ENSURE_EQ(context, NumInputs(node), 2);
  TF_LITE_ENSURE_EQ(context, NumOutputs(node), 1);

  TF_LITE_ENSURE_TYPES_EQ(context, input1->type, input2->type);

  if (output->type == kTfLiteInt8) {
    TF_LITE_ENSURE_STATUS(CalculateActivationRangeQuantized(
        context, params->activation, output, &data->output_activation_min,
        &data->output_activation_max));

    double real_multiplier = static_cast<double>(input1->params.scale) *
                             static_cast<double>(input2->params.scale) /
                             static_cast<double>(output->params.scale);
    QuantizeMultiplier(real_multiplier, &data->output_multiplier,
                       &data->output_shift);

    data->input1_zero_point = input1->params.zero_point;
    data->input2_zero_point = input2->params.zero_point;
    data->output_zero_point = output->params.zero_point;
  } else if (output->type == kTfLiteInt32) {
    CalculateActivationRange(params->activation, &data->output_activation_min,
                             &data->output_activation_max);
  } else {
    CalculateActivationRange(params->activation,
                             &data->output_activation_min_f32,
                             &data->output_activation_max_f32);
  }

  micro_context->DeallocateTempTfLiteTensor(input1);
  micro_context->DeallocateTempTfLiteTensor(input2);
  micro_context->DeallocateTempTfLiteTensor(output);
  return kTfLiteOk;
}

TfLiteStatus MulPrepare(TfLiteContext* context, TfLiteNode* node) {
  TFLITE_DCHECK(node->builtin_data != nullptr);
  auto* params = reinterpret_cast<TfLiteMulParams*>(node->builtin_data);

  TFLITE_DCHECK(node->user_data != nullptr);
  OpDataMul* data = static_cast<OpDataMul*>(node->user_data);

  return CalculateOpDataMul(context, node, params, data);
}

void EvalMulQuantizedReference(TfLiteContext* context, TfLiteNode* node,
                               const OpDataMul* data,
                               const TfLiteEvalTensor* input1,
                               const TfLiteEvalTensor* input2,
                               TfLiteEvalTensor* output) {
  tflite::ArithmeticParams op_params = {};
  op_params.quantized_activation_min = data->output_activation_min;
  op_params.quantized_activation_max = data->output_activation_max;
  op_params.float_activation_max = data->output_activation_max_f32;
  op_params.input1_offset = -data->input1_zero_point;
  op_params.input2_offset = -data->input2_zero_point;
  op_params.output_offset = data->output_zero_point;
  op_params.output_multiplier = data->output_multiplier;
  op_params.output_shift = data->output_shift;

  bool need_broadcast = reference_ops::ProcessBroadcastShapes(
      tflite::micro::GetTensorShape(input1),
      tflite::micro::GetTensorShape(input2), &op_params);

  if (input1->type == kTfLiteInt8) {
    if (need_broadcast) {
      reference_integer_ops::BroadcastMul4DSlow(
          op_params, tflite::micro::GetTensorShape(input1),
          tflite::micro::GetTensorData<int8_t>(input1),
          tflite::micro::GetTensorShape(input2),
          tflite::micro::GetTensorData<int8_t>(input2),
          tflite::micro::GetTensorShape(output),
          tflite::micro::GetTensorData<int8_t>(output));
    } else {
      reference_integer_ops::Mul(op_params,
                                 tflite::micro::GetTensorShape(input1),
                                 tflite::micro::GetTensorData<int8_t>(input1),
                                 tflite::micro::GetTensorShape(input2),
                                 tflite::micro::GetTensorData<int8_t>(input2),
                                 tflite::micro::GetTensorShape(output),
                                 tflite::micro::GetTensorData<int8_t>(output));
    }
  } else if (input1->type == kTfLiteInt32) {
    if (need_broadcast) {
      reference_ops::BroadcastMul4DSlow(
          op_params, tflite::micro::GetTensorShape(input1),
          tflite::micro::GetTensorData<int32_t>(input1),
          tflite::micro::GetTensorShape(input2),
          tflite::micro::GetTensorData<int32_t>(input2),
          tflite::micro::GetTensorShape(output),
          tflite::micro::GetTensorData<int32_t>(output));
    } else {
      reference_ops::Mul(op_params, tflite::micro::GetTensorShape(input1),
                         tflite::micro::GetTensorData<int32_t>(input1),
                         tflite::micro::GetTensorShape(input2),
                         tflite::micro::GetTensorData<int32_t>(input2),
                         tflite::micro::GetTensorShape(output),
                         tflite::micro::GetTensorData<int32_t>(output));
    }
  }
}

void EvalMulFloatReference(TfLiteContext* context, TfLiteNode* node,
                           TfLiteMulParams* params, const OpDataMul* data,
                           const TfLiteEvalTensor* input1,
                           const TfLiteEvalTensor* input2,
                           TfLiteEvalTensor* output) {
  tflite::ArithmeticParams op_params = {};
  op_params.float_activation_min = data->output_activation_min_f32;
  op_params.float_activation_max = data->output_activation_max_f32;

  bool need_broadcast = reference_ops::ProcessBroadcastShapes(
      tflite::micro::GetTensorShape(input1),
      tflite::micro::GetTensorShape(input2), &op_params);

  if (need_broadcast) {
    reference_ops::BroadcastMul4DSlow(
        op_params, tflite::micro::GetTensorShape(input1),
        tflite::micro::GetTensorData<float>(input1),
        tflite::micro::GetTensorShape(input2),
        tflite::micro::GetTensorData<float>(input2),
        tflite::micro::GetTensorShape(output),
        tflite::micro::GetTensorData<float>(output));
  } else {
    reference_ops::Mul(op_params, tflite::micro::GetTensorShape(input1),
                       tflite::micro::GetTensorData<float>(input1),
                       tflite::micro::GetTensorShape(input2),
                       tflite::micro::GetTensorData<float>(input2),
                       tflite::micro::GetTensorShape(output),
                       tflite::micro::GetTensorData<float>(output));
  }
}

}  // namespace tflite
