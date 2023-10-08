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
#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/kernels/internal/quantization_util.h"
#include "tensorflow/lite/kernels/internal/reference/binary_function.h"
#include "tensorflow/lite/kernels/internal/reference/integer_ops/add.h"
#include "tensorflow/lite/kernels/kernel_util.h"
#include "tensorflow/lite/micro/kernels/kernel_util.h"
#include "tensorflow/lite/micro/micro_context.h"
#include "tensorflow/lite/micro/micro_log.h"

namespace tflite {
namespace {
constexpr int kInputTensor1 = 0;
constexpr int kInputTensor2 = 1;
constexpr int kOutputTensor = 0;

struct OpData {
  bool requires_broadcast;
  ArithmeticParams arithmetic_params;
};

template <typename T>
T SquaredDifference(T input1, T input2) {
  const T difference = input1 - input2;
  return difference * difference;
}

void* SquaredDifferenceInit(TfLiteContext* context, const char* buffer,
                            size_t length) {
  TFLITE_DCHECK(context->AllocatePersistentBuffer != nullptr);
  return context->AllocatePersistentBuffer(context, sizeof(OpData));
}

TfLiteStatus SquaredDifferencePrepare(TfLiteContext* context,
                                      TfLiteNode* node) {
  TFLITE_DCHECK(node->user_data != nullptr);
  OpData* data = reinterpret_cast<OpData*>(node->user_data);
  data->requires_broadcast = false;

  TF_LITE_ENSURE_EQ(context, NumInputs(node), 2);
  TF_LITE_ENSURE_EQ(context, NumOutputs(node), 1);

  MicroContext* micro_context = GetMicroContext(context);

  TfLiteTensor* input1 =
      micro_context->AllocateTempInputTensor(node, kInputTensor1);
  TF_LITE_ENSURE(context, input1 != nullptr);
  TfLiteTensor* input2 =
      micro_context->AllocateTempInputTensor(node, kInputTensor2);
  TF_LITE_ENSURE(context, input2 != nullptr);
  TfLiteTensor* output =
      micro_context->AllocateTempOutputTensor(node, kOutputTensor);
  TF_LITE_ENSURE(context, output != nullptr);

  TF_LITE_ENSURE_TYPES_EQ(context, input1->type, input2->type);
  output->type = input2->type;

  // Ensure the quantization parameters are equivalent.
  if (input1->type == kTfLiteInt8) {
    const auto& input1_quantization_params = input1->params;
    const auto& input2_quantization_params = input2->params;
    const auto& output_quantization_params = output->params;
    const int32_t integer_type_min = std::numeric_limits<int8_t>::min();
    const int32_t integer_type_max = std::numeric_limits<int8_t>::max();
    TF_LITE_ENSURE(context,
                   input1_quantization_params.zero_point >= integer_type_min);
    TF_LITE_ENSURE(context,
                   input1_quantization_params.zero_point <= integer_type_max);
    TF_LITE_ENSURE(context,
                   input2_quantization_params.zero_point >= integer_type_min);
    TF_LITE_ENSURE(context,
                   input2_quantization_params.zero_point <= integer_type_max);
    TF_LITE_ENSURE(context,
                   output_quantization_params.zero_point >= integer_type_min);
    TF_LITE_ENSURE(context,
                   output_quantization_params.zero_point <= integer_type_max);
    data->arithmetic_params.input1_offset =
        -input1_quantization_params.zero_point;
    data->arithmetic_params.input2_offset =
        -input2_quantization_params.zero_point;
    data->arithmetic_params.output_offset =
        output_quantization_params.zero_point;

    // shift to make integer for scales.
    // 7 is selected so that maximum shifted result 255^2 * (1 << (7 * 2 ))
    // does not overflow signed 32-bit integer
    data->arithmetic_params.left_shift = 7;
    const double twice_max_input_scale =
        2.0 * static_cast<double>(std::max(input1_quantization_params.scale,
                                           input2_quantization_params.scale));
    const double real_input1_multiplier =
        static_cast<double>(input1_quantization_params.scale) /
        twice_max_input_scale;
    double real_input2_multiplier =
        static_cast<double>(input2_quantization_params.scale) /
        twice_max_input_scale;
    const double real_output_multiplier =
        (twice_max_input_scale * twice_max_input_scale) /
        static_cast<double>((1 << data->arithmetic_params.left_shift * 2) *
                            output_quantization_params.scale);
    QuantizeMultiplierSmallerThanOneExp(
        real_input1_multiplier, &data->arithmetic_params.input1_multiplier,
        &data->arithmetic_params.input1_shift);
    QuantizeMultiplierSmallerThanOneExp(
        real_input2_multiplier, &data->arithmetic_params.input2_multiplier,
        &data->arithmetic_params.input2_shift);
    QuantizeMultiplierSmallerThanOneExp(
        real_output_multiplier, &data->arithmetic_params.output_multiplier,
        &data->arithmetic_params.output_shift);
    data->arithmetic_params.quantized_activation_min =
        std::numeric_limits<int8_t>::min();
    data->arithmetic_params.quantized_activation_max =
        std::numeric_limits<int8_t>::max();
  }

  data->requires_broadcast = !HaveSameShapes(input1, input2);

  micro_context->DeallocateTempTfLiteTensor(input1);
  micro_context->DeallocateTempTfLiteTensor(input2);
  micro_context->DeallocateTempTfLiteTensor(output);
  return kTfLiteOk;
}

inline int8_t SquaredDifference(int8_t x, int8_t y,
                                const ArithmeticParams& params) {
  const int32_t input1_val = params.input1_offset + x;
  const int32_t input2_val = params.input2_offset + y;
  const int32_t shifted_input1_val = input1_val * (1 << params.left_shift);
  const int32_t shifted_input2_val = input2_val * (1 << params.left_shift);
  const int32_t scaled_input1_val =
      MultiplyByQuantizedMultiplierSmallerThanOneExp(
          shifted_input1_val, params.input1_multiplier, params.input1_shift);
  const int32_t scaled_input2_val =
      MultiplyByQuantizedMultiplierSmallerThanOneExp(
          shifted_input2_val, params.input2_multiplier, params.input2_shift);
  const int32_t raw_diff = scaled_input1_val - scaled_input2_val;

  // Max of this is 255^2 * (1 << 14), so won't overflow 32 bits.
  const int32_t squared_raw_diff = raw_diff * raw_diff;
  const int32_t raw_output =
      MultiplyByQuantizedMultiplierSmallerThanOneExp(
          squared_raw_diff, params.output_multiplier, params.output_shift) +
      params.output_offset;
  const int32_t clamped_output =
      std::min(params.quantized_activation_max,
               std::max(params.quantized_activation_min, raw_output));
  return static_cast<int8_t>(clamped_output);
}

template <typename T>
void EvalQuantizedSquaredDifference(TfLiteContext* context, TfLiteNode* node,
                                    const OpData* data,
                                    const TfLiteEvalTensor* input1,
                                    const TfLiteEvalTensor* input2,
                                    TfLiteEvalTensor* output) {
  const auto* op_data = static_cast<const OpData*>(node->user_data);
  if (data->requires_broadcast) {
    reference_integer_ops::BroadcastBinaryFunction4DSlow(
        op_data->arithmetic_params, tflite::micro::GetTensorShape(input1),
        tflite::micro::GetTensorData<T>(input1),
        tflite::micro::GetTensorShape(input2),
        tflite::micro::GetTensorData<T>(input2),
        tflite::micro::GetTensorShape(output),
        tflite::micro::GetTensorData<T>(output),
        reference_integer_ops::CheckArithmeticParams, SquaredDifference);
  } else {
    const int flat_size = tflite::micro::GetTensorShape(input1).FlatSize();
    reference_integer_ops::ElementWise(
        flat_size, op_data->arithmetic_params,
        tflite::micro::GetTensorData<int8_t>(input1),
        tflite::micro::GetTensorData<int8_t>(input2),
        tflite::micro::GetTensorData<int8_t>(output),
        reference_integer_ops::CheckArithmeticParams, SquaredDifference);
  }
}

template <typename T>
void EvalSquaredDifference(TfLiteContext* context, TfLiteNode* node,
                           const OpData* data, const TfLiteEvalTensor* input1,
                           const TfLiteEvalTensor* input2,
                           TfLiteEvalTensor* output) {
  if (data->requires_broadcast) {
    reference_ops::BroadcastBinaryFunction4DSlow<T, T, T>(
        tflite::micro::GetTensorShape(input1),
        tflite::micro::GetTensorData<T>(input1),
        tflite::micro::GetTensorShape(input2),
        tflite::micro::GetTensorData<T>(input2),
        tflite::micro::GetTensorShape(output),
        tflite::micro::GetTensorData<T>(output), SquaredDifference<T>);
  } else {
    reference_ops::BinaryFunction<T, T, T>(
        tflite::micro::GetTensorShape(input1),
        tflite::micro::GetTensorData<T>(input1),
        tflite::micro::GetTensorShape(input2),
        tflite::micro::GetTensorData<T>(input2),
        tflite::micro::GetTensorShape(output),
        tflite::micro::GetTensorData<T>(output), SquaredDifference<T>);
  }
}

TfLiteStatus SquaredDifferenceEval(TfLiteContext* context, TfLiteNode* node) {
  OpData* data = reinterpret_cast<OpData*>(node->user_data);

  const TfLiteEvalTensor* input1 =
      tflite::micro::GetEvalInput(context, node, kInputTensor1);
  const TfLiteEvalTensor* input2 =
      tflite::micro::GetEvalInput(context, node, kInputTensor2);
  TfLiteEvalTensor* output =
      tflite::micro::GetEvalOutput(context, node, kOutputTensor);

  if (output->type == kTfLiteFloat32) {
    EvalSquaredDifference<float>(context, node, data, input1, input2, output);
  } else if (output->type == kTfLiteInt32) {
    EvalSquaredDifference<int32_t>(context, node, data, input1, input2, output);
  } else if (output->type == kTfLiteInt8) {
    EvalQuantizedSquaredDifference<int8_t>(context, node, data, input1, input2,
                                           output);
  } else {
    MicroPrintf(
        "SquaredDifference only supports FLOAT32, INT32 and INT8 now, got %d.",
        output->type);
    return kTfLiteError;
  }

  return kTfLiteOk;
}
}  // namespace

TfLiteRegistration Register_SQUARED_DIFFERENCE() {
  return tflite::micro::RegisterOp(
      SquaredDifferenceInit, SquaredDifferencePrepare, SquaredDifferenceEval);
}

}  // namespace tflite
