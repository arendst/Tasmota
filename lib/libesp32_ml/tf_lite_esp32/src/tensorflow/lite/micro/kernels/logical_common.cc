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
#include "tensorflow/lite/kernels/internal/reference/binary_function.h"
#include "tensorflow/lite/kernels/internal/tensor_ctypes.h"
#include "tensorflow/lite/kernels/op_macros.h"
#include "tensorflow/lite/micro/kernels/kernel_util.h"
#include "tensorflow/lite/micro/kernels/logical.h"

namespace tflite {

// Input/output tensor index.
const int kLogicalInputTensor1 = 0;
const int kLogicalInputTensor2 = 1;
const int kLogicalOutputTensor = 0;

TfLiteStatus LogicalImpl(TfLiteContext* context, TfLiteNode* node,
                         bool (*func)(bool, bool)) {
  const TfLiteEvalTensor* input1 =
      tflite::micro::GetEvalInput(context, node, kLogicalInputTensor1);
  const TfLiteEvalTensor* input2 =
      tflite::micro::GetEvalInput(context, node, kLogicalInputTensor2);
  TfLiteEvalTensor* output =
      tflite::micro::GetEvalOutput(context, node, kLogicalOutputTensor);

  if (tflite::micro::HaveSameShapes(input1, input2)) {
    reference_ops::BinaryFunction<bool, bool, bool>(
        tflite::micro::GetTensorShape(input1),
        tflite::micro::GetTensorData<bool>(input1),
        tflite::micro::GetTensorShape(input2),
        tflite::micro::GetTensorData<bool>(input2),
        tflite::micro::GetTensorShape(output),
        tflite::micro::GetTensorData<bool>(output), func);
  } else {
    reference_ops::BroadcastBinaryFunction4DSlow<bool, bool, bool>(
        tflite::micro::GetTensorShape(input1),
        tflite::micro::GetTensorData<bool>(input1),
        tflite::micro::GetTensorShape(input2),
        tflite::micro::GetTensorData<bool>(input2),
        tflite::micro::GetTensorShape(output),
        tflite::micro::GetTensorData<bool>(output), func);
  }

  return kTfLiteOk;
}

bool LogicalOr(bool x, bool y) { return x || y; }

bool LogicalAnd(bool x, bool y) { return x && y; }

}  // namespace tflite
