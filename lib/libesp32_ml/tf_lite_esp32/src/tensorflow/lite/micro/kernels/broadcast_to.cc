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
#include "tensorflow/lite/kernels/internal/reference/broadcast_to.h"

#include <stdint.h>

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/kernels/internal/tensor_ctypes.h"
#include "tensorflow/lite/kernels/kernel_util.h"
#include "tensorflow/lite/micro/kernels/kernel_util.h"
#include "tensorflow/lite/micro/micro_context.h"

namespace tflite {

namespace {
constexpr int kInputTensor = 0;
constexpr int kShapeTensor = 1;
constexpr int kOutputTensor = 0;
// Support a maximum of 5 dimensions in TFLM.
constexpr int kMaxDims = 5;

TfLiteStatus ValidateOutputTensor(TfLiteContext* context, TfLiteTensor* input,
                                  TfLiteTensor* shape, TfLiteTensor* output) {
  // Ensures the shape is 1D tensor.
  TF_LITE_ENSURE_EQ(context, NumDimensions(shape), 1);

  // Ensure output dims is not less than input dims.
  int input_num_dims = NumDimensions(input);
  int output_num_dims = NumDimensions(output);
  int shape_num_dims = SizeOfDimension(shape, 0);
  TF_LITE_ENSURE_MSG(context, output_num_dims == shape_num_dims,
                     "Output must match with the expected shape dimension.");
  TF_LITE_ENSURE_MSG(context, input_num_dims <= output_num_dims,
                     "Output shape must be broadcastable from input shape.");
  TF_LITE_ENSURE_MSG(context, output_num_dims <= kMaxDims,
                     "BroadcastTo only supports 1-5D tensor.");

  // Check if output shape is broadcastable from input shape.
  auto get_shape_data = [shape](int i) -> int32_t {
    if (shape->type == kTfLiteInt32) {
      return GetTensorData<int32_t>(shape)[i];
    } else {
      return GetTensorData<int64_t>(shape)[i];
    }
  };

  int extending_dims = output_num_dims - input_num_dims;
  for (int idx = 0; idx < input_num_dims; ++idx) {
    TF_LITE_ENSURE_MSG(
        context,
        (SizeOfDimension(input, idx) == 1 ||
         SizeOfDimension(input, idx) == get_shape_data(extending_dims + idx)),
        "Output shape must be broadcastable from input shape.");
  }

  // Validating the shape of the output tensor.
  tflite::RuntimeShape output_shape = tflite::GetTensorShape(output);
  for (int idx = 0; idx < output_num_dims; ++idx) {
    TF_LITE_ENSURE(context, output_shape.Dims(idx) == get_shape_data(idx));
  }
  return kTfLiteOk;
}

TfLiteStatus BroadcastToPrepare(TfLiteContext* context, TfLiteNode* node) {
  TF_LITE_ENSURE(context, NumInputs(node) == 2);
  TF_LITE_ENSURE_EQ(context, NumOutputs(node), 1);
  MicroContext* micro_context = GetMicroContext(context);
  TfLiteTensor* input =
      micro_context->AllocateTempInputTensor(node, kInputTensor);
  TfLiteTensor* shape =
      micro_context->AllocateTempInputTensor(node, kShapeTensor);
  TfLiteTensor* output =
      micro_context->AllocateTempOutputTensor(node, kOutputTensor);

  TF_LITE_ENSURE_MSG(context, (NumDimensions(input) <= kMaxDims),
                     "BroadcastTo only supports 1-5D tensor.");

  TF_LITE_ENSURE(context,
                 shape->type == kTfLiteInt32 || shape->type == kTfLiteInt64);
  TF_LITE_ENSURE_EQ(context, input->type, output->type);

  // Does not support String type due to its variable size. This limitation is
  // the same as TFLite.
  TF_LITE_ENSURE(context, input->type != kTfLiteString);

  TF_LITE_ENSURE_STATUS(ValidateOutputTensor(context, input, shape, output));
  micro_context->DeallocateTempTfLiteTensor(input);
  micro_context->DeallocateTempTfLiteTensor(shape);
  micro_context->DeallocateTempTfLiteTensor(output);
  return kTfLiteOk;
}

TfLiteStatus BroadcastToEval(TfLiteContext* context, TfLiteNode* node) {
  const TfLiteEvalTensor* input =
      micro::GetEvalInput(context, node, kInputTensor);
  TfLiteEvalTensor* output = micro::GetEvalOutput(context, node, kOutputTensor);

  // BroadcastTo op support upto 5 dims, different from 8 dims in TFLite.
  reference_ops::BroadcastTo<kMaxDims>(
      micro::GetTensorShape(input), input->data.raw,
      micro::GetTensorShape(output), output->data.raw, input->type);
  return kTfLiteOk;
}
}  // namespace

TfLiteRegistration Register_BROADCAST_TO() {
  return tflite::micro::RegisterOp(nullptr, BroadcastToPrepare,
                                   BroadcastToEval);
}

}  // namespace tflite
