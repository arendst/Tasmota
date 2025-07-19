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
#include "tensorflow/lite/micro/micro_context.h"
#include "tensorflow/lite/micro/micro_graph.h"
#include "tensorflow/lite/schema/schema_generated.h"

namespace tflite {

namespace {

struct OpData {
  int then_subgraph_index;
  int else_subgraph_index;
};

void* Init(TfLiteContext* context, const char* buffer, size_t length) {
  TFLITE_DCHECK(context->AllocatePersistentBuffer != nullptr);
  return context->AllocatePersistentBuffer(context, sizeof(OpData));
}

TfLiteStatus Prepare(TfLiteContext* context, TfLiteNode* node) {
  OpData* op_data = reinterpret_cast<OpData*>(node->user_data);
  const auto* params =
      reinterpret_cast<const TfLiteIfParams*>(node->builtin_data);
  op_data->then_subgraph_index = params->then_subgraph_index;
  op_data->else_subgraph_index = params->else_subgraph_index;

  TF_LITE_ENSURE(context, node->inputs->size > 0);

  // The first input is the condition.
  tflite::MicroContext* micro_context = tflite::GetMicroContext(context);
  TfLiteTensor* cond = micro_context->AllocateTempInputTensor(node, 0);

  TF_LITE_ENSURE(context, cond != nullptr);
  TF_LITE_ENSURE_EQ(context, cond->type, kTfLiteBool);
  TF_LITE_ENSURE_EQ(context, NumElements(cond), 1);

  micro_context->DeallocateTempTfLiteTensor(cond);

  // The first input of the node is the condition. The rest of inputs are
  // passed to the branch subgraphs. Therefore, the number of subgraph inputs
  // will be the number of node inputs - 1.
  size_t num_inputs = node->inputs->size - 1;
  size_t num_outputs = node->outputs->size;

  MicroGraph& graph_info = micro_context->graph();

  TF_LITE_ENSURE(context,
                 op_data->then_subgraph_index < graph_info.NumSubgraphs());
  TF_LITE_ENSURE(context,
                 op_data->else_subgraph_index < graph_info.NumSubgraphs());

  TF_LITE_ENSURE_EQ(context, num_inputs,
                    graph_info.NumSubgraphInputs(op_data->then_subgraph_index));
  TF_LITE_ENSURE_EQ(
      context, num_outputs,
      graph_info.NumSubgraphOutputs(op_data->then_subgraph_index));

  return kTfLiteOk;
}

TfLiteStatus Eval(TfLiteContext* context, TfLiteNode* node) {
  const OpData* op_data = reinterpret_cast<OpData*>(node->user_data);

  tflite::MicroContext* micro_context = tflite::GetMicroContext(context);
  TfLiteTensor* cond = micro_context->AllocateTempInputTensor(node, 0);

  TF_LITE_ENSURE(context, cond != nullptr);
  bool cond_value = cond->data.b[0];
  micro_context->DeallocateTempTfLiteTensor(cond);

  MicroGraph* graph_info = &micro_context->graph();
  // Currently we copy the input / output between the subgraphs.
  int active_branch_subgraph_index =
      cond_value ? op_data->then_subgraph_index : op_data->else_subgraph_index;

  TF_LITE_ENSURE_OK(context,
                    tflite::micro::CopyOpInputsToSubgraphInputs(
                        context, node, graph_info, active_branch_subgraph_index,
                        /*first_tensor_idx=*/1));

  TF_LITE_ENSURE_OK(context,
                    graph_info->InvokeSubgraph(active_branch_subgraph_index));

  TF_LITE_ENSURE_OK(
      context, tflite::micro::CopySubgraphOutputsToOpOutputs(
                   context, node, graph_info, active_branch_subgraph_index));

  return kTfLiteOk;
}

}  // namespace.

TfLiteRegistration Register_IF() {
  return tflite::micro::RegisterOp(Init, Prepare, Eval);
}

}  // namespace tflite
