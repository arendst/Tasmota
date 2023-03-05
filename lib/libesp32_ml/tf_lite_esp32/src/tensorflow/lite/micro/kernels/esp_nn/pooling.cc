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
#include "tensorflow/lite/kernels/internal/reference/pooling.h"

#include "tensorflow/lite/c/builtin_op_data.h"
#include "tensorflow/lite/kernels/kernel_util.h"
#include "tensorflow/lite/micro/kernels/kernel_util.h"
#include "tensorflow/lite/micro/kernels/pooling.h"
#include "tensorflow/lite/micro/micro_log.h"

#if ESP_NN
// #include <esp_nn.h>
#include "esp-nn/include/esp_nn.h"
#endif

#include <esp_timer.h>

long long pooling_total_time = 0;

namespace tflite {

namespace {
#if ESP_NN
void AverageEvalQuantized(TfLiteContext* context, const TfLiteNode* node,
                          const TfLitePoolParams* params, const OpDataPooling* data,
                          const TfLiteEvalTensor* input,
                          TfLiteEvalTensor* output) {

  const int stride_height = params->stride_height;
  const int stride_width = params->stride_width;
  const int filter_height = params->filter_height;
  const int filter_width = params->filter_width;
  const int activation_min = data->activation_min;
  const int activation_max = data->activation_max;
  const int pad_height = data->padding.height;
  const int pad_width = data->padding.width;

  const RuntimeShape& input_shape = tflite::micro::GetTensorShape(input);
  const RuntimeShape& output_shape = tflite::micro::GetTensorShape(output);
  TFLITE_DCHECK_LE(activation_min, activation_max);
  TFLITE_DCHECK_EQ(input_shape.DimensionsCount(), 4);
  TFLITE_DCHECK_EQ(output_shape.DimensionsCount(), 4);
  const int batches = MatchingDim(input_shape, 0, output_shape, 0);
  const int depth = MatchingDim(input_shape, 3, output_shape, 3);
  const int input_height = input_shape.Dims(1);
  const int input_width = input_shape.Dims(2);
  const int output_height = output_shape.Dims(1);
  const int output_width = output_shape.Dims(2);

  const int8_t *input_data = tflite::micro::GetTensorData<int8_t>(input);
  int8_t *output_data = tflite::micro::GetTensorData<int8_t>(output);

  const int input_size = input_width * input_height * depth;
  const int output_size = output_width * output_height * depth;

  if (depth % 4 == 0) { // S3 version only supports channels multiple of 4
    for (int batch = 0; batch < batches; ++batch) {
      esp_nn_avg_pool_s8(input_data, input_width, input_height,
                         output_data, output_width, output_height,
                         stride_width, stride_height,
                         filter_width, filter_height,
                         pad_width, pad_height,
                         activation_min, activation_max, depth);
      input_data += input_size;
      output_data += output_size;
    }
  } else {
    for (int batch = 0; batch < batches; ++batch) {
      esp_nn_avg_pool_s8_ansi(input_data, input_width, input_height,
                              output_data, output_width, output_height,
                              stride_width, stride_height,
                              filter_width, filter_height,
                              pad_width, pad_height,
                              activation_min, activation_max, depth);
      input_data += input_size;
      output_data += output_size;
    }
  }
}

void MaxEvalQuantized(TfLiteContext* context, TfLiteNode* node,
                      TfLitePoolParams* params, const OpDataPooling* data,
                      const TfLiteEvalTensor* input, TfLiteEvalTensor* output) {

  const int stride_height = params->stride_height;
  const int stride_width = params->stride_width;
  const int filter_height = params->filter_height;
  const int filter_width = params->filter_width;
  const int activation_min = data->activation_min;
  const int activation_max = data->activation_max;
  const int pad_height = data->padding.height;
  const int pad_width = data->padding.width;

  const RuntimeShape& input_shape = tflite::micro::GetTensorShape(input);
  const RuntimeShape& output_shape = tflite::micro::GetTensorShape(output);
  TFLITE_DCHECK_LE(activation_min, activation_max);
  TFLITE_DCHECK_EQ(input_shape.DimensionsCount(), 4);
  TFLITE_DCHECK_EQ(output_shape.DimensionsCount(), 4);
  const int batches = MatchingDim(input_shape, 0, output_shape, 0);
  const int depth = MatchingDim(input_shape, 3, output_shape, 3);
  const int input_height = input_shape.Dims(1);
  const int input_width = input_shape.Dims(2);
  const int output_height = output_shape.Dims(1);
  const int output_width = output_shape.Dims(2);

  const int8_t *input_data = tflite::micro::GetTensorData<int8_t>(input);
  int8_t *output_data = tflite::micro::GetTensorData<int8_t>(output);

  const int input_size = input_width * input_height * depth;
  const int output_size = output_width * output_height * depth;
  if (depth % 4 == 0) { // S3 version only supports channels multiple of 4
    for (int batch = 0; batch < batches; ++batch) {
      esp_nn_max_pool_s8(input_data, input_width, input_height,
                         output_data, output_width, output_height,
                         stride_width, stride_height,
                         filter_width, filter_height,
                         pad_width, pad_height,
                         activation_min, activation_max, depth);
      input_data += input_size;
      output_data += output_size;
    }
  } else {
    for (int batch = 0; batch < batches; ++batch) {
      esp_nn_max_pool_s8_ansi(input_data, input_width, input_height,
                              output_data, output_width, output_height,
                              stride_width, stride_height,
                              filter_width, filter_height,
                              pad_width, pad_height,
                              activation_min, activation_max, depth);
      input_data += input_size;
      output_data += output_size;
    }
  }
}
#endif

TfLiteStatus AverageEval(TfLiteContext* context, TfLiteNode* node) {
  TFLITE_DCHECK(node->builtin_data != nullptr);
  auto* params = reinterpret_cast<TfLitePoolParams*>(node->builtin_data);

  TFLITE_DCHECK(node->user_data != nullptr);
  const OpDataPooling* data =
      static_cast<const OpDataPooling*>(node->user_data);

  const TfLiteEvalTensor* input =
      micro::GetEvalInput(context, node, kPoolingInputTensor);
  TfLiteEvalTensor* output =
      micro::GetEvalOutput(context, node, kPoolingOutputTensor);

  long long start_time = esp_timer_get_time();
  // Inputs and outputs share the same type, guaranteed by the converter.
  switch (input->type) {
    case kTfLiteFloat32:
      AveragePoolingEvalFloat(context, node, params, data, input, output);
      break;
    case kTfLiteInt8:
#if ESP_NN
      AverageEvalQuantized(context, node, params, data, input, output);
#else
      AveragePoolingEvalQuantized(context, node, params, data, input, output);
#endif
      break;
    default:
      TF_LITE_KERNEL_LOG(context, "Input type %s is not currently supported",
                         TfLiteTypeGetName(input->type));
      return kTfLiteError;
  }
  pooling_total_time += esp_timer_get_time() - start_time;
  return kTfLiteOk;
}

TfLiteStatus MaxEval(TfLiteContext* context, TfLiteNode* node) {
  TFLITE_DCHECK(node->builtin_data != nullptr);
  auto* params = reinterpret_cast<TfLitePoolParams*>(node->builtin_data);

  TFLITE_DCHECK(node->user_data != nullptr);
  const OpDataPooling* data =
      static_cast<const OpDataPooling*>(node->user_data);

  const TfLiteEvalTensor* input =
      micro::GetEvalInput(context, node, kPoolingInputTensor);
  TfLiteEvalTensor* output =
      micro::GetEvalOutput(context, node, kPoolingOutputTensor);

  long long start_time = esp_timer_get_time();
  switch (input->type) {
    case kTfLiteFloat32:
      MaxPoolingEvalFloat(context, node, params, data, input, output);
      break;
    case kTfLiteInt8:
#if ESP_NN
      MaxEvalQuantized(context, node, params, data, input, output);
#else
      MaxPoolingEvalQuantized(context, node, params, data, input, output);
#endif
      break;
    default:
      TF_LITE_KERNEL_LOG(context, "Type %s not currently supported.",
                         TfLiteTypeGetName(input->type));
      return kTfLiteError;
  }
  pooling_total_time += esp_timer_get_time() - start_time;
  return kTfLiteOk;
}

void* Init(TfLiteContext* context, const char* buffer, size_t length) {
  TFLITE_DCHECK(context->AllocatePersistentBuffer != nullptr);
  return context->AllocatePersistentBuffer(context, sizeof(OpDataPooling));
}

}  // namespace

TfLiteRegistration Register_AVERAGE_POOL_2D() {
  return tflite::micro::RegisterOp(Init, PoolingPrepare, AverageEval);
}

TfLiteRegistration Register_MAX_POOL_2D() {
  return tflite::micro::RegisterOp(Init, PoolingPrepare, MaxEval);
}

}  // namespace tflite
