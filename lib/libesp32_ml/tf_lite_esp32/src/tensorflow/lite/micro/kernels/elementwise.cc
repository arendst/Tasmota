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

#include <cmath>

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/kernels/internal/common.h"
#include "tensorflow/lite/kernels/internal/quantization_util.h"
#include "tensorflow/lite/kernels/internal/tensor_ctypes.h"
#include "tensorflow/lite/kernels/kernel_util.h"
#include "tensorflow/lite/micro/kernels/kernel_util.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "tensorflow/lite/micro/micro_utils.h"

namespace tflite {
namespace ops {
namespace micro {
namespace elementwise {
namespace {

constexpr int kAbsNameId = 0;
constexpr int kRsrqtNameId = 1;

const int kElementwiseInputTensor = 0;
const int kElementwiseOutputTensor = 0;

struct OpDataAbsRsqrt {
  int32_t multiplier;
  int shift;
  int input_offset;
  int output_offset;
  bool needs_rescale;
  TfLiteQuantizationType input_quantization_type;
  TfLiteType input_type;
};

bool IsNumericSupportedType(const TfLiteType type) {
  return type == kTfLiteFloat32;
}

bool IsLogicalSupportedType(const TfLiteType type) {
  return type == kTfLiteBool;
}

bool IsAbsSupportedType(const TfLiteType type) {
  return type == kTfLiteFloat32 || type == kTfLiteInt8 || type == kTfLiteInt16;
}

bool IsRsqrtSupportedType(const TfLiteType type) {
  return type == kTfLiteFloat32 || type == kTfLiteInt8;
}

inline void SetAbsOutputMultiplier(const float input_scale,
                                   const float output_scale,
                                   int32_t* multiplier, int* shift) {
  QuantizeMultiplier(static_cast<double>(input_scale / output_scale),
                     multiplier, shift);
}

inline void SetRsqrtOutputMultiplier(const float input_scale,
                                     const float output_scale,
                                     int32_t* multiplier, int* shift) {
  const double scale =
      1. / static_cast<double>((std::sqrt(input_scale) * output_scale));
  QuantizeMultiplier(scale, multiplier, shift);
}

typedef bool (*IsSupportedType)(TfLiteType);
template <IsSupportedType>
TfLiteStatus GenericPrepare(TfLiteContext* context, TfLiteNode* node) {
  MicroContext* micro_context = GetMicroContext(context);
  TF_LITE_ENSURE_EQ(context, NumInputs(node), 1);
  TF_LITE_ENSURE_EQ(context, NumOutputs(node), 1);
  TfLiteTensor* input =
      micro_context->AllocateTempInputTensor(node, kElementwiseInputTensor);
  TF_LITE_ENSURE(context, input != nullptr);
  TfLiteTensor* output =
      micro_context->AllocateTempOutputTensor(node, kElementwiseOutputTensor);
  TF_LITE_ENSURE(context, output != nullptr);
  TF_LITE_ENSURE_TYPES_EQ(context, input->type, output->type);
  if (!IsSupportedType(input->type)) {
    MicroPrintf("Input data type %s (%d) is not supported.",
                TfLiteTypeGetName(input->type), input->type);
    return kTfLiteError;
  }

  micro_context->DeallocateTempTfLiteTensor(input);
  micro_context->DeallocateTempTfLiteTensor(output);
  return kTfLiteOk;
}

typedef bool (*IsSupportedType)(TfLiteType);
template <IsSupportedType, const int op_nameid>
TfLiteStatus PrepareAbsRsqrt(TfLiteContext* context, TfLiteNode* node) {
  MicroContext* micro_context = GetMicroContext(context);
  TF_LITE_ENSURE_EQ(context, NumInputs(node), 1);
  TF_LITE_ENSURE_EQ(context, NumOutputs(node), 1);
  TfLiteTensor* input = micro_context->AllocateTempInputTensor(node, 0);
  TF_LITE_ENSURE(context, input != nullptr);
  TfLiteTensor* output = micro_context->AllocateTempOutputTensor(node, 0);
  TF_LITE_ENSURE(context, output != nullptr);
  TF_LITE_ENSURE_TYPES_EQ(context, input->type, output->type);
  if (!IsSupportedType(input->type)) {
    MicroPrintf("Input data type %s (%d) is not supported.",
                TfLiteTypeGetName(input->type), input->type);
    return kTfLiteError;
  }

  auto* op_data = static_cast<OpDataAbsRsqrt*>(node->user_data);
  op_data->input_type = input->type;

  // For int16 type input, we support both quantized and non-quantized
  // evaluation.
  if (op_nameid == kAbsNameId) {
    op_data->input_quantization_type = input->quantization.type;
  }

  if (input->type == kTfLiteInt8 ||
      (input->type == kTfLiteInt16 &&
       input->quantization.type != kTfLiteNoQuantization)) {
    TF_LITE_ENSURE_EQ(context, input->quantization.type,
                      kTfLiteAffineQuantization);
    TF_LITE_ENSURE_EQ(context, output->quantization.type,
                      kTfLiteAffineQuantization);
    const auto* input_params =
        reinterpret_cast<TfLiteAffineQuantization*>(input->quantization.params);
    const auto* output_params = reinterpret_cast<TfLiteAffineQuantization*>(
        output->quantization.params);
    TF_LITE_ENSURE(context, input_params != nullptr);
    TF_LITE_ENSURE(context, input_params->scale != nullptr);
    TF_LITE_ENSURE(context, input_params->scale->size > 0);
    TF_LITE_ENSURE(context, input_params->zero_point->size > 0);
    TF_LITE_ENSURE(context, output_params != nullptr);
    TF_LITE_ENSURE(context, output_params->scale != nullptr);
    TF_LITE_ENSURE(context, output_params->scale->size > 0);
    TF_LITE_ENSURE(context, output_params->zero_point->size > 0);
    op_data->input_offset = input_params->zero_point->data[0];
    op_data->output_offset = output_params->zero_point->data[0];
    if (input->type == kTfLiteInt16) {
      TF_LITE_ENSURE_EQ(context, op_data->input_offset, 0);
      TF_LITE_ENSURE_EQ(context, op_data->output_offset, 0);
    }
    const float input_scale = input_params->scale->data[0];
    const float output_scale = output_params->scale->data[0];
    op_data->needs_rescale = input_scale != output_scale;
    if (op_nameid == kAbsNameId && op_data->needs_rescale) {
      SetAbsOutputMultiplier(input_scale, output_scale, &op_data->multiplier,
                             &op_data->shift);
    } else if (op_nameid == kRsrqtNameId) {
      SetRsqrtOutputMultiplier(input_scale, output_scale, &op_data->multiplier,
                               &op_data->shift);
    }
  }
  micro_context->DeallocateTempTfLiteTensor(input);
  micro_context->DeallocateTempTfLiteTensor(output);
  return kTfLiteOk;
}

template <typename T>
inline TfLiteStatus EvalImplQuantized(
    TfLiteContext* context, TfLiteNode* node,
    T func(TfLiteContext*, TfLiteNode*, T),
    TfLiteStatus validate_input_func(TfLiteContext*, TfLiteNode*, T),
    TfLiteType expected_type) {
  const TfLiteEvalTensor* input = tflite::micro::GetEvalInput(context, node, 0);
  TfLiteEvalTensor* output = tflite::micro::GetEvalOutput(context, node, 0);
  TF_LITE_ENSURE_TYPES_EQ(context, input->type, expected_type);
  const size_t num_elements = ElementCount(*input->dims);
  const T* in_data = tflite::micro::GetTensorData<T>(input);
  T* out_data = tflite::micro::GetTensorData<T>(output);
  for (size_t i = 0; i < num_elements; ++i) {
    if (validate_input_func) {
      TF_LITE_ENSURE_OK(context,
                        validate_input_func(context, node, in_data[i]));
    }
    out_data[i] = func(context, node, in_data[i]);
  }
  return kTfLiteOk;
}

template <typename T>
inline T AbsHelper(T i) {
  return std::abs(i);
}

template <typename T>
inline TfLiteStatus EvalImpl(TfLiteContext* context, TfLiteNode* node,
                             T func(T), TfLiteStatus validate_input_func(T),
                             TfLiteType expected_type) {
  const TfLiteEvalTensor* input = tflite::micro::GetEvalInput(context, node, 0);
  TfLiteEvalTensor* output = tflite::micro::GetEvalOutput(context, node, 0);
  TF_LITE_ENSURE_TYPES_EQ(context, input->type, expected_type);
  const size_t num_elements = ElementCount(*input->dims);
  const T* in_data = tflite::micro::GetTensorData<T>(input);
  T* out_data = tflite::micro::GetTensorData<T>(output);
  for (size_t i = 0; i < num_elements; ++i) {
    if (validate_input_func) {
      TF_LITE_ENSURE_OK(context, validate_input_func(in_data[i]));
    }
    out_data[i] = func(in_data[i]);
  }
  return kTfLiteOk;
}

inline TfLiteStatus EvalNumeric(TfLiteContext* context, TfLiteNode* node,
                                float float_func(float)) {
  return EvalImpl<float>(context, node, float_func,
                         /*validate_input_func=*/nullptr, kTfLiteFloat32);
}

inline TfLiteStatus EvalLogical(TfLiteContext* context, TfLiteNode* node,

                                bool bool_func(bool)) {
  return EvalImpl<bool>(context, node, bool_func,
                        /*validate_input_func=*/nullptr, kTfLiteBool);
}

void* ElementWiseAbsRsqrtInit(TfLiteContext* context, const char* buffer,
                              size_t length) {
  TFLITE_DCHECK(context->AllocatePersistentBuffer != nullptr);
  return context->AllocatePersistentBuffer(context, sizeof(OpDataAbsRsqrt));
}

template <typename T>
inline T AbsEvalQuantized(TfLiteContext* context, TfLiteNode* node, T i) {
  const auto* op_data = static_cast<const OpDataAbsRsqrt*>(node->user_data);
  const int kMin = std::numeric_limits<T>::min();
  const int kMax = std::numeric_limits<T>::max();

  const int32_t value = std::abs(i - op_data->input_offset);
  if (!op_data->needs_rescale) {
    return static_cast<T>(
        std::min(std::max(static_cast<long int>(value + op_data->output_offset),
                          static_cast<long int>(kMin)),
                 static_cast<long int>(kMax)));
  }

  const int32_t output = tflite::MultiplyByQuantizedMultiplier(
                             value, op_data->multiplier, op_data->shift) +
                         op_data->output_offset;
  return static_cast<T>(std::min(
      std::max(static_cast<long int>(output), static_cast<long int>(kMin)),
      static_cast<long int>(kMax)));
}

template <typename T>
inline T RsqrtEvalQuantized(TfLiteContext* context, TfLiteNode* node, T i) {
  const auto* op_data = static_cast<const OpDataAbsRsqrt*>(node->user_data);
  const int kMin = std::numeric_limits<T>::min();
  const int kMax = std::numeric_limits<T>::max();

  const int32_t value = (i - op_data->input_offset);
  const int32_t kShift = 20;  // Shift to keep value integer.
  if (value == 0) {
    // Assume that any value close to 0 represents the max output value.
    return static_cast<T>(kMax);
  }
  int32_t inv_sqrt_multiplier;
  int inv_sqrt_shift;
  GetInvSqrtQuantizedMultiplierExp(value, kReverseShift, &inv_sqrt_multiplier,
                                   &inv_sqrt_shift);
  const int32_t data = tflite::MultiplyByQuantizedMultiplier(
      static_cast<int32_t>(1), inv_sqrt_multiplier, inv_sqrt_shift + kShift);
  const int32_t output =
      tflite::MultiplyByQuantizedMultiplier(data, op_data->multiplier,
                                            op_data->shift - kShift) +
      op_data->output_offset;
  return static_cast<T>(std::min(
      std::max(static_cast<long int>(output), static_cast<long int>(kMin)),
      static_cast<long int>(kMax)));
}

template <typename T>
TfLiteStatus validate_input_func(TfLiteContext* context, TfLiteNode* node,
                                 T i) {
  const auto* op_data = static_cast<const OpDataAbsRsqrt*>(node->user_data);

  TF_LITE_ENSURE_MSG(context, i >= op_data->input_offset,
                     "Rsqrt is only defined for positive values");
  return static_cast<TfLiteStatus>(kTfLiteOk);
}

TfLiteStatus AbsEval(TfLiteContext* context, TfLiteNode* node) {
  OpDataAbsRsqrt* op_data = reinterpret_cast<OpDataAbsRsqrt*>(node->user_data);
  TfLiteType type = op_data->input_type;
  TfLiteQuantizationType input_quantization_type =
      op_data->input_quantization_type;
  TfLiteStatus eval_result;

  switch (type) {
    case kTfLiteFloat32:
      eval_result = EvalNumeric(context, node, std::abs);
      break;
    case kTfLiteInt8:
      eval_result =
          EvalImplQuantized<int8_t>(context, node, AbsEvalQuantized,
                                    /*validate_input_func=*/nullptr, type);
      break;
    case kTfLiteInt16:
      eval_result =
          input_quantization_type == kTfLiteNoQuantization
              ? EvalImpl<int16_t>(context, node, AbsHelper,
                                  /*validate_input_func=*/nullptr, type)
              : EvalImplQuantized<int16_t>(context, node, AbsEvalQuantized,
                                           /*validate_input_func=*/nullptr,
                                           type);
      break;
    default:
      MicroPrintf("Current data type %s is not supported.",
                  TfLiteTypeGetName(type));
      return kTfLiteError;
      break;
  }
  return eval_result;
}

TfLiteStatus SinEval(TfLiteContext* context, TfLiteNode* node) {
  return EvalNumeric(context, node, std::sin);
}

TfLiteStatus CosEval(TfLiteContext* context, TfLiteNode* node) {
  return EvalNumeric(context, node, std::cos);
}

TfLiteStatus LogEval(TfLiteContext* context, TfLiteNode* node) {
  return EvalNumeric(context, node, std::log);
}

TfLiteStatus SqrtEval(TfLiteContext* context, TfLiteNode* node) {
  return EvalNumeric(context, node, std::sqrt);
}

TfLiteStatus RsqrtEval(TfLiteContext* context, TfLiteNode* node) {
  const auto* op_data = static_cast<const OpDataAbsRsqrt*>(node->user_data);
  TfLiteType type = op_data->input_type;
  switch (type) {
    case kTfLiteFloat32:
      return EvalImpl<float>(
          context, node, [](float f) { return 1.f / std::sqrt(f); },
          /*validate_input_func=*/nullptr, type);
    case kTfLiteInt8:
      return EvalImplQuantized<int8_t>(context, node,
                                       elementwise::RsqrtEvalQuantized,
                                       elementwise::validate_input_func, type);

    default:
      MicroPrintf("Current data type %s is not supported.",
                  TfLiteTypeGetName(type));
      return kTfLiteError;
  }
}

TfLiteStatus SquareEval(TfLiteContext* context, TfLiteNode* node) {
  return EvalNumeric(context, node, [](float f) { return f * f; });
}

TfLiteStatus LogicalNotEval(TfLiteContext* context, TfLiteNode* node) {
  return EvalLogical(context, node, [](bool v) { return !v; });
}

}  // namespace
}  // namespace elementwise

TfLiteRegistration Register_ABS() {
  return tflite::micro::RegisterOp(
      elementwise::ElementWiseAbsRsqrtInit,
      elementwise::PrepareAbsRsqrt<elementwise::IsAbsSupportedType,
                                   elementwise::kAbsNameId>,
      elementwise::AbsEval);
}

TfLiteRegistration Register_SIN() {
  return tflite::micro::RegisterOp(
      nullptr, elementwise::GenericPrepare<elementwise::IsNumericSupportedType>,
      elementwise::SinEval);
}

TfLiteRegistration Register_COS() {
  return tflite::micro::RegisterOp(
      nullptr, elementwise::GenericPrepare<elementwise::IsNumericSupportedType>,
      elementwise::CosEval);
}

TfLiteRegistration Register_LOG() {
  return tflite::micro::RegisterOp(
      nullptr, elementwise::GenericPrepare<elementwise::IsNumericSupportedType>,
      elementwise::LogEval);
}

TfLiteRegistration Register_SQRT() {
  return tflite::micro::RegisterOp(
      nullptr, elementwise::GenericPrepare<elementwise::IsNumericSupportedType>,
      elementwise::SqrtEval);
}

TfLiteRegistration Register_RSQRT() {
  return tflite::micro::RegisterOp(
      elementwise::ElementWiseAbsRsqrtInit,
      elementwise::PrepareAbsRsqrt<elementwise::IsRsqrtSupportedType,
                                   elementwise::kRsrqtNameId>,
      elementwise::RsqrtEval);
}

TfLiteRegistration Register_SQUARE() {
  return tflite::micro::RegisterOp(
      nullptr, elementwise::GenericPrepare<elementwise::IsNumericSupportedType>,
      elementwise::SquareEval);
}

TfLiteRegistration Register_LOGICAL_NOT() {
  return tflite::micro::RegisterOp(
      nullptr, elementwise::GenericPrepare<elementwise::IsLogicalSupportedType>,
      elementwise::LogicalNotEval);
}

}  // namespace micro
}  // namespace ops
}  // namespace tflite
