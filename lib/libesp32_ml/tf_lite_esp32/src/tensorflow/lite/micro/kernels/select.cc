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
#include "tensorflow/lite/kernels/internal/reference/select.h"

#include <stddef.h>
#include <stdint.h>

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/kernels/internal/tensor_ctypes.h"
#include "tensorflow/lite/kernels/kernel_util.h"
#include "tensorflow/lite/micro/kernels/kernel_util.h"
#include "tensorflow/lite/micro/micro_log.h"

namespace tflite {

constexpr int kInputTensorCondition = 0;
constexpr int kInputTensorX = 1;
constexpr int kInputTensorY = 2;
constexpr int kOutputTensor = 0;

struct OpData {
  bool requires_broadcast;
  // True if input condition is scalar or input condition has rank one and
  // matches the first dimension of other inputs.
  bool has_low_rank_input_condition;
};

void* SelectInit(TfLiteContext* context, const char* buffer, size_t length) {
  TFLITE_DCHECK(context->AllocatePersistentBuffer != nullptr);
  auto* data = static_cast<OpData*>(
      context->AllocatePersistentBuffer(context, sizeof(OpData)));
  data->requires_broadcast = false;
  data->has_low_rank_input_condition = false;
  return data;
}

TfLiteStatus CheckBroadcastShape(TfLiteContext* context,
                                 const TfLiteTensor* input1,
                                 const TfLiteTensor* input2,
                                 const TfLiteTensor* input3,
                                 const TfLiteIntArray* output_shape) {
  const int dims1 = NumDimensions(input1);
  const int dims2 = NumDimensions(input2);
  const int dims3 = NumDimensions(input3);
  const int out_dims = std::max(std::max(dims1, dims2), dims3);
  TF_LITE_ENSURE_EQ(context, out_dims, output_shape->size);

  for (int i = 0; i < out_dims; ++i) {
    const int d1 = i >= dims1 ? 1 : SizeOfDimension(input1, dims1 - i - 1);
    const int d2 = i >= dims2 ? 1 : SizeOfDimension(input2, dims2 - i - 1);
    const int d3 = i >= dims3 ? 1 : SizeOfDimension(input3, dims3 - i - 1);
    const int min_value = std::min(std::min(d1, d2), d3);
    int max_value = std::max(std::max(d1, d2), d3);
    // If one dimention is 0, others must be 0 or 1.
    if (min_value == 0) max_value = 0;
    if (!(d1 == 1 || d1 == max_value) || !(d2 == 1 || d2 == max_value) ||
        !(d3 == 1 || d3 == max_value)) {
      MicroPrintf("Given shapes are not broadcastable.");
      return kTfLiteError;
    }
    TF_LITE_ENSURE_EQ(context, output_shape->data[out_dims - i - 1], max_value);
  }
  return kTfLiteOk;
}

TfLiteStatus SelectPrepare(TfLiteContext* context, TfLiteNode* node) {
  OpData* data = reinterpret_cast<OpData*>(node->user_data);

  TF_LITE_ENSURE_EQ(context, NumInputs(node), 3);
  TF_LITE_ENSURE_EQ(context, NumOutputs(node), 1);

  MicroContext* micro_context = GetMicroContext(context);
  TfLiteTensor* input_condition =
      micro_context->AllocateTempInputTensor(node, kInputTensorCondition);

  TfLiteTensor* input_x =
      micro_context->AllocateTempInputTensor(node, kInputTensorX);

  TfLiteTensor* input_y =
      micro_context->AllocateTempInputTensor(node, kInputTensorY);

  TfLiteTensor* output =
      micro_context->AllocateTempOutputTensor(node, kOutputTensor);

  // Input must be bool.
  TF_LITE_ENSURE_TYPES_EQ(context, input_condition->type, kTfLiteBool);
  TF_LITE_ENSURE_TYPES_EQ(context, input_x->type, input_y->type);
  output->type = input_x->type;

  // Respect the original output shape when there are mixed shapes to represent
  // a scalar data.
  if (GetTensorShape(input_condition).FlatSize() == 1 &&
      GetTensorShape(input_x).FlatSize() == 1 &&
      GetTensorShape(input_y).FlatSize() == 1 &&
      GetTensorShape(output).FlatSize() == 1) {
    return kTfLiteOk;
  }

  bool same_shape = HaveSameShapes(input_condition, input_x) &&
                    HaveSameShapes(input_x, input_y);
  if (!same_shape) {
    TF_LITE_ENSURE_OK(
        context, CheckBroadcastShape(context, input_condition, input_x, input_y,
                                     output->dims));
    data->requires_broadcast = true;
  }

  micro_context->DeallocateTempTfLiteTensor(input_condition);
  micro_context->DeallocateTempTfLiteTensor(input_x);
  micro_context->DeallocateTempTfLiteTensor(input_y);
  micro_context->DeallocateTempTfLiteTensor(output);

  return kTfLiteOk;
}

TfLiteStatus SelectEval(TfLiteContext* context, TfLiteNode* node) {
  OpData* data = static_cast<OpData*>(node->user_data);
  MicroContext* micro_context = GetMicroContext(context);

  TfLiteTensor* input_condition =
      micro_context->AllocateTempInputTensor(node, kInputTensorCondition);

  TfLiteTensor* input_x =
      micro_context->AllocateTempInputTensor(node, kInputTensorX);

  TfLiteTensor* input_y =
      micro_context->AllocateTempInputTensor(node, kInputTensorY);

  TfLiteTensor* output =
      micro_context->AllocateTempOutputTensor(node, kOutputTensor);

#define TF_LITE_SELECT(type, op)                                           \
  reference_ops::op(GetTensorShape(input_condition),                       \
                    GetTensorData<bool>(input_condition),                  \
                    GetTensorShape(input_x), GetTensorData<type>(input_x), \
                    GetTensorShape(input_y), GetTensorData<type>(input_y), \
                    GetTensorShape(output), GetTensorData<type>(output));

#define TF_LITE_SWITCH(type, op)                                     \
  switch (type) {                                                    \
    case kTfLiteFloat32:                                             \
      TF_LITE_SELECT(float, op);                                     \
      break;                                                         \
    case kTfLiteInt8:                                                \
      TF_LITE_SELECT(int8_t, op);                                    \
      break;                                                         \
    case kTfLiteInt16:                                               \
      TF_LITE_SELECT(int16_t, op);                                   \
      break;                                                         \
    default:                                                         \
      MicroPrintf("Does not support type other than %s, but got %s", \
                  "int8|int16|float32", TfLiteTypeGetName(type));    \
      return kTfLiteError;                                           \
  }

  if (data->has_low_rank_input_condition) {
    MicroPrintf("Not yet implemented.");
    return kTfLiteError;
  } else if (data->requires_broadcast) {
    TF_LITE_SWITCH(input_x->type, BroadcastSelect5DSlow);
  } else {
    TF_LITE_SWITCH(input_x->type, Select);
  }

#undef TF_LITE_SELECT
#undef TF_LITE_SWITCH
  micro_context->DeallocateTempTfLiteTensor(input_condition);
  micro_context->DeallocateTempTfLiteTensor(input_x);
  micro_context->DeallocateTempTfLiteTensor(input_y);
  micro_context->DeallocateTempTfLiteTensor(output);

  return kTfLiteOk;
}

// SelectV2 op selects values of 'x' if the corresponding value of 'condition'
// is true or the value of 'y' if false. There are valid condition input sizes:
//
// 1. Either the same shape (in which case the select is elementwise), or
// 2. Broadcastable shapes between 'condition', 'x' and 'y'.
TfLiteRegistration Register_SELECT_V2() {
  return tflite::micro::RegisterOp(tflite::SelectInit, tflite::SelectPrepare,
                                   tflite::SelectEval);
}

}  // namespace tflite
