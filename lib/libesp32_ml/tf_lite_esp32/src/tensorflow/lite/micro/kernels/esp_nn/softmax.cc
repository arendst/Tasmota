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

#include "tensorflow/lite/micro/kernels/softmax.h"

#include "tensorflow/lite/c/builtin_op_data.h"
#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/kernels/internal/common.h"
#include "tensorflow/lite/kernels/internal/quantization_util.h"
#include "tensorflow/lite/kernels/internal/reference/softmax.h"
#include "tensorflow/lite/kernels/internal/tensor_ctypes.h"
#include "tensorflow/lite/kernels/kernel_util.h"
#include "tensorflow/lite/kernels/op_macros.h"
#include "tensorflow/lite/micro/kernels/kernel_util.h"
#include "tensorflow/lite/micro/micro_log.h"

#include <esp_timer.h>

#if ESP_NN
// #include <esp_nn.h>
#include "esp-nn/include/esp_nn.h"
#endif

long long softmax_total_time = 0;

namespace tflite {
namespace {
// Softmax parameter data that persists in user_data
const int kInt16LUTArraySize = 513;

struct NodeData {
  SoftmaxParams op_data;
#if ESP_NN
  int buffer_idx;
#endif
};

static void* Init(TfLiteContext* context, const char* buffer, size_t length) {
  TFLITE_DCHECK(context->AllocatePersistentBuffer != nullptr);
  return context->AllocatePersistentBuffer(context, sizeof(NodeData));
}

void SoftmaxQuantized(TfLiteContext* context, const TfLiteEvalTensor* input,
                      TfLiteEvalTensor* output, const NodeData* data) {
  if (input->type == kTfLiteInt8) {
    if (output->type == kTfLiteInt16) {
      tflite::reference_ops::Softmax(
          data->op_data, tflite::micro::GetTensorShape(input),
          tflite::micro::GetTensorData<int8_t>(input),
          tflite::micro::GetTensorShape(output),
          tflite::micro::GetTensorData<int16_t>(output));
    } else {
#if ESP_NN
      const int32_t input_beta_multiplier = data->op_data.input_multiplier;
      const int32_t input_beta_left_shift = data->op_data.input_left_shift;
      const int diff_min = data->op_data.diff_min;
      const RuntimeShape input_shape = tflite::micro::GetTensorShape(input);
      const RuntimeShape output_shape = tflite::micro::GetTensorShape(output);
      const int trailing_dim = input_shape.DimensionsCount() - 1;
      const int outer_size =
          MatchingFlatSizeSkipDim(input_shape, trailing_dim, output_shape);
      const int depth =
          MatchingDim(input_shape, trailing_dim, output_shape, trailing_dim);
      const int8_t *in_ptr = tflite::micro::GetTensorData<int8_t>(input);
      int8_t *out_ptr = tflite::micro::GetTensorData<int8_t>(output);
      void *scratch_buf = NULL;
      if (data->buffer_idx > -1) {
        scratch_buf = context->GetScratchBuffer(context, data->buffer_idx);
      }
      esp_nn_set_softmax_scratch_buf(scratch_buf);
      esp_nn_softmax_s8(in_ptr, outer_size, depth, input_beta_multiplier,
                        input_beta_left_shift, diff_min, out_ptr);
#else
      tflite::reference_ops::Softmax(
          data->op_data, tflite::micro::GetTensorShape(input),
          tflite::micro::GetTensorData<int8_t>(input),
          tflite::micro::GetTensorShape(output),
          tflite::micro::GetTensorData<int8_t>(output));
#endif
    }
  } else {
    tflite::reference_ops::SoftmaxInt16(
        data->op_data, tflite::micro::GetTensorShape(input),
        tflite::micro::GetTensorData<int16_t>(input),
        tflite::micro::GetTensorShape(output),
        tflite::micro::GetTensorData<int16_t>(output));
  }
}

static TfLiteStatus Eval(TfLiteContext* context, TfLiteNode* node) {
  const TfLiteEvalTensor* input = tflite::micro::GetEvalInput(context, node, 0);
  TfLiteEvalTensor* output = tflite::micro::GetEvalOutput(context, node, 0);

  TFLITE_DCHECK(node->user_data != nullptr);
  NodeData data = *static_cast<NodeData*>(node->user_data);

  long long start_time = esp_timer_get_time();
  switch (input->type) {
    case kTfLiteFloat32: {
      tflite::reference_ops::Softmax(
          data.op_data, tflite::micro::GetTensorShape(input),
          tflite::micro::GetTensorData<float>(input),
          tflite::micro::GetTensorShape(output),
          tflite::micro::GetTensorData<float>(output));
    }
    break;
    case kTfLiteInt8:
    case kTfLiteInt16: {
      SoftmaxQuantized(context, input, output, &data);
    }
    break;
    default:
      TF_LITE_KERNEL_LOG(context, "Type %s (%d) not supported.",
                         TfLiteTypeGetName(input->type), input->type);
      return kTfLiteError;
  }
  softmax_total_time += esp_timer_get_time() - start_time;
  return kTfLiteOk;
}

static TfLiteStatus Prepare(TfLiteContext* context, TfLiteNode* node) {
  MicroContext* micro_context = GetMicroContext(context);

  TF_LITE_ENSURE_EQ(context, NumInputs(node), 1);
  TF_LITE_ENSURE_EQ(context, NumOutputs(node), 1);
  TfLiteTensor* input = micro_context->AllocateTempInputTensor(node, 0);
  TF_LITE_ENSURE(context, input != nullptr);
  TF_LITE_ENSURE(context, NumDimensions(input) >= 1);
  TfLiteTensor* output = micro_context->AllocateTempOutputTensor(node, 0);
  TF_LITE_ENSURE(context, output != nullptr);

  TF_LITE_ENSURE(context, node->user_data != nullptr);
  NodeData* data = static_cast<NodeData*>(node->user_data);
  // Only allocate LUTs for KTfLiteInt16 data type
  if (input->type == kTfLiteInt16) {
    void* raw_exp_lut = context->AllocatePersistentBuffer(
        context, sizeof(int16_t) * kInt16LUTArraySize);
    TF_LITE_ENSURE(context, raw_exp_lut != nullptr);
    data->op_data.exp_lut = reinterpret_cast<int16_t*>(raw_exp_lut);
    void* one_over_one_plus_x_lut = context->AllocatePersistentBuffer(
        context, sizeof(int16_t) * kInt16LUTArraySize);
    TF_LITE_ENSURE(context, one_over_one_plus_x_lut != nullptr);
    data->op_data.one_over_one_plus_x_lut =
        reinterpret_cast<int16_t*>(one_over_one_plus_x_lut);
  }

  if (output->type == kTfLiteInt16) {
    TF_LITE_ENSURE(context,
                   input->type == kTfLiteInt8 || input->type == kTfLiteInt16);
  } else {
    TF_LITE_ENSURE_EQ(context, input->type, output->type);
  }

  // Populate LUT if required
  if (input->type == kTfLiteInt16) {
    TF_LITE_ENSURE_EQ(context, output->params.zero_point, 0);
    // exp LUT only used on negative values
    // we consider exp(-10.0) is insignificant to accumulation
    gen_lut<float, int16_t, int16_t>(
        [](float value) { return std::exp(value); }, -10.0f, 0.0f, -1.0f, 1.0f,
        data->op_data.exp_lut);
    gen_lut<float, int16_t, int16_t>(
        [](float value) { return 1.0f / (1.0f + value); }, 0.0f, 1.0f, -1.0f,
        1.0f, data->op_data.one_over_one_plus_x_lut);
    data->op_data.zero_point = output->params.zero_point;
    data->op_data.scale = output->params.scale;
  }

  auto* params = static_cast<TfLiteSoftmaxParams*>(node->builtin_data);
  auto ret_val =
      CalculateSoftmaxParams(context, input, output, params, &data->op_data);

#if ESP_NN
  if (output->type == kTfLiteInt8 && input->type == kTfLiteInt8) {
    const int32_t input_width = input->dims->data[1];
    const int32_t input_height = input->dims->data[2];
    int scratch_buf_size = esp_nn_get_softmax_scratch_size(input_width,
                                                           input_height);
    if (scratch_buf_size > 0) {
      TF_LITE_ENSURE_STATUS(context->RequestScratchBufferInArena(
        context, scratch_buf_size, &data->buffer_idx));
    }
  }
#endif

  micro_context->DeallocateTempTfLiteTensor(input);
  micro_context->DeallocateTempTfLiteTensor(output);
  return ret_val;
}

}  // namespace

TfLiteRegistration Register_SOFTMAX() {
  return tflite::micro::RegisterOp(Init, Prepare, Eval);
}

}  // namespace tflite
