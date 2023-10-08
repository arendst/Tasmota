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

#include <stddef.h>

#include <cstring>

#include "tensorflow/lite/c/builtin_op_data.h"
#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/kernels/internal/compatibility.h"
#include "tensorflow/lite/kernels/kernel_util.h"
#include "tensorflow/lite/micro/kernels/kernel_util.h"
#include "tensorflow/lite/micro/memory_helpers.h"
#include "tensorflow/lite/micro/micro_graph.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "tensorflow/lite/micro/micro_resource_variable.h"
#include "tensorflow/lite/schema/schema_generated.h"

namespace tflite {

namespace {

constexpr int kInputVariableId = 0;
constexpr int kOutputValue = 0;

TfLiteStatus Prepare(TfLiteContext* context, TfLiteNode* node) {
  TFLITE_DCHECK(NumInputs(node) == 1);
  TFLITE_DCHECK(NumOutputs(node) == 1);

  MicroContext* micro_context = GetMicroContext(context);

  TfLiteTensor* input_resource_id_tensor =
      micro_context->AllocateTempInputTensor(node, kInputVariableId);

  TFLITE_DCHECK(input_resource_id_tensor != nullptr);
  TFLITE_DCHECK(input_resource_id_tensor->type == kTfLiteResource);
  TFLITE_DCHECK(NumElements(input_resource_id_tensor) == 1);

  micro_context->DeallocateTempTfLiteTensor(input_resource_id_tensor);

  return kTfLiteOk;
}

TfLiteStatus Eval(TfLiteContext* context, TfLiteNode* node) {
  const TfLiteEvalTensor* input_resource_id_tensor =
      tflite::micro::GetEvalInput(context, node, kInputVariableId);
  TFLITE_DCHECK(input_resource_id_tensor != nullptr);

  TfLiteEvalTensor* output_value =
      tflite::micro::GetEvalOutput(context, node, kOutputValue);
  TFLITE_DCHECK(output_value != nullptr);

  tflite::MicroContext* micro_context = tflite::GetMicroContext(context);
  MicroGraph& graph_info = micro_context->graph();

  MicroResourceVariables* resources = graph_info.GetResourceVariables();
  if (resources == nullptr) {
    MicroPrintf(
        "READ_VARIABLE requires resource variables. Please create "
        "ResourceVariables and pass it to the interpreter.");
    return kTfLiteError;
  }
  TF_LITE_ENSURE_OK(
      context,
      resources->Read(input_resource_id_tensor->data.i32[0], output_value));
  return kTfLiteOk;
}

}  // namespace.

TfLiteRegistration Register_READ_VARIABLE() {
  return tflite::micro::RegisterOp(nullptr, Prepare, Eval);
}

}  // namespace tflite
