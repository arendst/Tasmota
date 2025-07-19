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
#include "tensorflow/lite/micro/micro_utils.h"

namespace tflite {

namespace {

struct OpData {
  int cond_subgraph_index;
  int body_subgraph_index;
};

void* Init(TfLiteContext* context, const char* buffer, size_t length) {
  TFLITE_DCHECK(context->AllocatePersistentBuffer != nullptr);
  return context->AllocatePersistentBuffer(context, sizeof(OpData));
}

TfLiteStatus Prepare(TfLiteContext* context, TfLiteNode* node) {
  OpData* op_data = reinterpret_cast<OpData*>(node->user_data);
  const auto* params =
      reinterpret_cast<const TfLiteWhileParams*>(node->builtin_data);

  op_data->cond_subgraph_index = params->cond_subgraph_index;
  op_data->body_subgraph_index = params->body_subgraph_index;

  // The first input is the condition.
  tflite::MicroContext* micro_context = tflite::GetMicroContext(context);

  size_t num_inputs = node->inputs->size;
  size_t num_outputs = node->outputs->size;

  MicroGraph& graph_info = micro_context->graph();

  TF_LITE_ENSURE(context,
                 op_data->cond_subgraph_index < graph_info.NumSubgraphs());
  TF_LITE_ENSURE(context,
                 op_data->body_subgraph_index < graph_info.NumSubgraphs());

  TF_LITE_ENSURE_EQ(context, num_inputs,
                    graph_info.NumSubgraphInputs(op_data->cond_subgraph_index));
  TF_LITE_ENSURE_EQ(context, num_inputs,
                    graph_info.NumSubgraphInputs(op_data->body_subgraph_index));
  TF_LITE_ENSURE_EQ(context, num_inputs, num_outputs);
  TF_LITE_ENSURE_EQ(
      context, num_outputs,
      graph_info.NumSubgraphOutputs(op_data->body_subgraph_index));

  return kTfLiteOk;
}

TfLiteStatus Eval(TfLiteContext* context, TfLiteNode* node) {
  const OpData* op_data = reinterpret_cast<OpData*>(node->user_data);

  tflite::MicroContext* micro_context = tflite::GetMicroContext(context);
  MicroGraph* graph_info = &micro_context->graph();

  TF_LITE_ENSURE_OK(context,
                    tflite::micro::CopyOpInputsToSubgraphInputs(
                        context, node, graph_info, op_data->cond_subgraph_index,
                        /*first_tensor_idx=*/0));

  TF_LITE_ENSURE_OK(context,
                    graph_info->InvokeSubgraph(op_data->cond_subgraph_index));

  TfLiteEvalTensor* cond_subgraph_output = graph_info->GetSubgraphOutput(
      op_data->cond_subgraph_index, /*tensor_idx=*/0);
  bool cond_value = cond_subgraph_output->data.b[0];

  TF_LITE_ENSURE_OK(context,
                    tflite::micro::CopyOpInputsToSubgraphInputs(
                        context, node, graph_info, op_data->body_subgraph_index,
                        /*first_tensor_idx=*/0));
  TF_LITE_ENSURE_OK(context,
                    tflite::micro::CopyOpInputsToOpOutputs(context, node));

  while (cond_value == true) {
    // Copy output of this iteration back to the body input.
    TF_LITE_ENSURE_OK(
        context, tflite::micro::CopyOpOutputsToSubgraphInputs(
                     context, node, graph_info, op_data->body_subgraph_index));
    TF_LITE_ENSURE_OK(context,
                      graph_info->InvokeSubgraph(op_data->body_subgraph_index));

    TF_LITE_ENSURE_OK(
        context, tflite::micro::CopySubgraphOutputsToOpOutputs(
                     context, node, graph_info, op_data->body_subgraph_index));
    TF_LITE_ENSURE_OK(
        context, tflite::micro::CopyOpOutputsToSubgraphInputs(
                     context, node, graph_info, op_data->cond_subgraph_index));
    TF_LITE_ENSURE_OK(context,
                      graph_info->InvokeSubgraph(op_data->cond_subgraph_index));

    cond_subgraph_output = graph_info->GetSubgraphOutput(
        op_data->cond_subgraph_index, /*tensor_idx=*/0);
    cond_value = cond_subgraph_output->data.b[0];
  }

  return kTfLiteOk;
}

}  // namespace.

TfLiteRegistration Register_WHILE() {
  return tflite::micro::RegisterOp(Init, Prepare, Eval);
}

}  // namespace tflite
