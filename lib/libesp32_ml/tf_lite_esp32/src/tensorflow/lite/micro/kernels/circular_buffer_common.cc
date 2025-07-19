/* Copyright 2020 The TensorFlow Authors. All Rights Reserved.

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

#include "tensorflow/lite/c/builtin_op_data.h"
#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/kernels/internal/compatibility.h"
#include "tensorflow/lite/kernels/internal/quantization_util.h"
#include "tensorflow/lite/kernels/internal/tensor_ctypes.h"
#include "tensorflow/lite/kernels/kernel_util.h"
#include "tensorflow/lite/kernels/op_macros.h"
#include "tensorflow/lite/micro/flatbuffer_utils.h"
#include "tensorflow/lite/micro/kernels/circular_buffer.h"
#include "tensorflow/lite/micro/kernels/kernel_util.h"

namespace tflite {

// The CircularBuffer op has one input and one output tensor.
const int kCircularBufferInputTensor = 0;
const int kCircularBufferOutputTensor = 0;

// Indices into the init flexbuffer's vector.
// The parameter's name is in the comment that follows.
// Elements in the vectors are ordered alphabetically by parameter name.
const int kCircularBufferCyclesMaxIndex = 0;  // 'cycles_max'

// TODO(b/149795762): Add this to TfLiteStatus enum.
const TfLiteStatus kTfLiteAbort = static_cast<TfLiteStatus>(-9);

TfLiteStatus CircularBufferPrepare(TfLiteContext* context, TfLiteNode* node) {
  MicroContext* micro_context = GetMicroContext(context);

  TfLiteTensor* input =
      micro_context->AllocateTempInputTensor(node, kCircularBufferInputTensor);
  TfLiteTensor* output = micro_context->AllocateTempOutputTensor(
      node, kCircularBufferOutputTensor);

  TFLITE_DCHECK(node->user_data != nullptr);
  OpDataCircularBuffer* op_data =
      static_cast<OpDataCircularBuffer*>(node->user_data);

  TF_LITE_ENSURE(context, input != nullptr);
  TF_LITE_ENSURE(context, output != nullptr);
  TF_LITE_ENSURE_EQ(context, input->dims->data[0], output->dims->data[0]);
  TF_LITE_ENSURE_EQ(context, 1, input->dims->data[1]);
  TF_LITE_ENSURE_EQ(context, input->dims->data[2], output->dims->data[2]);
  TF_LITE_ENSURE_EQ(context, output->dims->data[3], input->dims->data[3]);

  TF_LITE_ENSURE_TYPES_EQ(context, input->type, output->type);

  // The circular buffer custom operator currently only supports int8.
  TF_LITE_ENSURE_TYPES_EQ(context, input->type, kTfLiteInt8);

  if (op_data->cycles_max <= 0) {
    // The last circular buffer layer simply accumulates outputs, and does not
    // run periodically.
    // TODO(b/150001379): Move this special case logic to the tflite flatbuffer.
    static int cb_prepare_count = 0;
    cb_prepare_count++;
    // These checks specifically work for the only two streaming models
    // supported on TFLM. They use the shape of the output tensor along with the
    // layer number to determine if the circular buffer period should be 1 or 2.

    // These models are outlined int the following documents:
    // https://docs.google.com/document/d/1lc_G2ZFhjiKFo02UHjBaljye1xsL0EkfybkaVELEE3Q/edit?usp=sharing
    // https://docs.google.com/document/d/1pGc42PuWyrk-Jy1-9qeqtggvsmHr1ifz8Lmqfpr2rKA/edit?usp=sharing
    if (output->dims->data[1] == 5 || output->dims->data[1] == 13 ||
        output->dims->data[1] == 25 ||
        (cb_prepare_count == 5 && output->dims->data[2] == 2 &&
         output->dims->data[3] == 96)) {
      op_data->cycles_max = 1;
      cb_prepare_count = 0;
    } else {
      op_data->cycles_max = 2;
    }
  }
  op_data->cycles_until_run = op_data->cycles_max;
  node->user_data = op_data;

  micro_context->DeallocateTempTfLiteTensor(input);
  micro_context->DeallocateTempTfLiteTensor(output);

  return kTfLiteOk;
}

}  // namespace tflite
