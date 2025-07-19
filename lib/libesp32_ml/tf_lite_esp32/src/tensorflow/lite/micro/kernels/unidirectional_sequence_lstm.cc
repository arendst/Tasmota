/* Copyright 2017 The TensorFlow Authors. All Rights Reserved.

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
#include <cstddef>

#include "tensorflow/lite/c/builtin_op_data.h"
#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/kernels/internal/compatibility.h"
#include "tensorflow/lite/kernels/internal/portable_tensor_utils.h"
#include "tensorflow/lite/kernels/internal/quantization_util.h"
#include "tensorflow/lite/kernels/internal/tensor_ctypes.h"
#include "tensorflow/lite/kernels/kernel_util.h"
#include "tensorflow/lite/micro/kernels/kernel_util.h"
#include "tensorflow/lite/micro/kernels/lstm_eval.h"
#include "tensorflow/lite/micro/kernels/lstm_shared.h"
#include "tensorflow/lite/micro/kernels/micro_tensor_utils.h"
#include "tensorflow/lite/micro/micro_log.h"

namespace tflite {

namespace {

constexpr int scratch_index_size = 12;

struct UnidirectionalSequenceLstmOpData {
  // If the lstm is layer norm.
  bool use_layer_norm;
  // The scratch index.
  int scratch_index[scratch_index_size];

  int32_t row_sums_size;
  int32_t* row_sums;
  bool compute_row_sums = false;

  int32_t input_zero_point;
  int32_t output_state_zero_point;

  IntegerLstmParameter integer_lstm_param;
};

TfLiteStatus PopulateQuantizedLstmParams8x8_16(
    TfLiteContext* context, TfLiteNode* node,
    IntegerLstmParameter* integer_lstm_param) {
  MicroContext* micro_context = GetMicroContext(context);

  // Calculate quantized clip for projection and cell.
  const auto* params =
      static_cast<TfLiteUnidirectionalSequenceLSTMParams*>(node->builtin_data);
  const float cell_clip = params->cell_clip;
  const float proj_clip = params->proj_clip;

  TfLiteTensor* cell_state =
      micro_context->AllocateTempInputTensor(node, kLstmCellStateTensor);
  TF_LITE_ENSURE(context, cell_state != nullptr);
  TF_LITE_ENSURE(context, cell_state->is_variable);
  TfLiteTensor* output_tensor =
      micro_context->AllocateTempOutputTensor(node, kLstmOutputTensor);

  TF_LITE_ENSURE(context,
                 cell_state->quantization.type != kTfLiteNoQuantization);
  auto* cell_state_params =
      static_cast<TfLiteAffineQuantization*>(cell_state->quantization.params);
  TF_LITE_ENSURE(context,
                 output_tensor->quantization.type != kTfLiteNoQuantization);
  auto* proj_params = static_cast<TfLiteAffineQuantization*>(
      output_tensor->quantization.params);
  if (cell_clip > 0.0f) {
    integer_lstm_param->quantized_cell_clip = static_cast<int16_t>(std::min(
        std::max(cell_clip / cell_state_params->scale->data[0], -32768.0f),
        32767.0f));
  } else {
    integer_lstm_param->quantized_cell_clip = 0;
  }
  if (proj_clip > 0.0f) {
    integer_lstm_param->quantized_proj_clip = static_cast<int8_t>(std::min(
        std::max(proj_clip / proj_params->scale->data[0], -128.0f), 127.0f));
  } else {
    integer_lstm_param->quantized_proj_clip = 0;
  }

  // Calculate effective scales.
  UnidirectionalSequenceLstmOpData* op_data =
      static_cast<UnidirectionalSequenceLstmOpData*>(node->user_data);
  const bool use_layer_norm = op_data->use_layer_norm;

  TfLiteTensor* input =
      micro_context->AllocateTempInputTensor(node, kLstmInputTensor);

  TfLiteTensor* input_to_input_weights = micro_context->AllocateTempInputTensor(
      node, kLstmInputToInputWeightsTensor);
  TfLiteTensor* input_to_forget_weights =
      micro_context->AllocateTempInputTensor(node,
                                             kLstmInputToForgetWeightsTensor);
  TfLiteTensor* input_to_cell_weights = micro_context->AllocateTempInputTensor(
      node, kLstmInputToCellWeightsTensor);
  TfLiteTensor* input_to_output_weights =
      micro_context->AllocateTempInputTensor(node,
                                             kLstmInputToOutputWeightsTensor);

  TfLiteTensor* recurrent_to_input_weights =
      micro_context->AllocateTempInputTensor(
          node, kLstmRecurrentToInputWeightsTensor);
  TfLiteTensor* recurrent_to_forget_weights =
      micro_context->AllocateTempInputTensor(
          node, kLstmRecurrentToForgetWeightsTensor);
  TfLiteTensor* recurrent_to_cell_weights =
      micro_context->AllocateTempInputTensor(node,
                                             kLstmRecurrentToCellWeightsTensor);
  TfLiteTensor* recurrent_to_output_weights =
      micro_context->AllocateTempInputTensor(
          node, kLstmRecurrentToOutputWeightsTensor);

  TfLiteTensor* cell_to_input_weights = micro_context->AllocateTempInputTensor(
      node, kLstmCellToInputWeightsTensor);
  TfLiteTensor* cell_to_forget_weights = micro_context->AllocateTempInputTensor(
      node, kLstmCellToForgetWeightsTensor);
  TfLiteTensor* cell_to_output_weights = micro_context->AllocateTempInputTensor(
      node, kLstmCellToOutputWeightsTensor);

  TfLiteTensor* input_layer_norm_coefficients =
      micro_context->AllocateTempInputTensor(
          node, kLstmInputLayerNormCoefficientsTensor);
  TfLiteTensor* forget_layer_norm_coefficients =
      micro_context->AllocateTempInputTensor(
          node, kLstmForgetLayerNormCoefficientsTensor);
  TfLiteTensor* cell_layer_norm_coefficients =
      micro_context->AllocateTempInputTensor(
          node, kLstmCellLayerNormCoefficientsTensor);
  TfLiteTensor* output_layer_norm_coefficients =
      micro_context->AllocateTempInputTensor(
          node, kLstmOutputLayerNormCoefficientsTensor);

  TfLiteTensor* projection_weights = micro_context->AllocateTempInputTensor(
      node, kLstmProjectionWeightsTensor);

  TfLiteTensor* output_state =
      micro_context->AllocateTempInputTensor(node, kLstmOutputStateTensor);
  TF_LITE_ENSURE(context, output_state != nullptr);
  TF_LITE_ENSURE(context, output_state->is_variable);

  // Since we have already checked that weights are all there or none, we can
  // check the existence of only one to get the condition.
  const bool use_cifg = (input_to_input_weights == nullptr);
  const bool use_peephole = (cell_to_output_weights != nullptr);
  const bool use_projection = (projection_weights != nullptr);

  // Get intermediate scales and zero points.
  float intermediate_scale[5];
  int32_t intermediate_zp[5];
  for (int i = 0; i < 4; ++i) {
    if (use_layer_norm) {
      TfLiteTensor* intermediate =
          micro_context->AllocateTempIntermediateTensor(node, i);
      TF_LITE_ENSURE(context,
                     intermediate->quantization.type != kTfLiteNoQuantization);
      auto* params_intermediate = static_cast<TfLiteAffineQuantization*>(
          intermediate->quantization.params);
      intermediate_scale[i] = params_intermediate->scale->data[0];
      intermediate_zp[i] = params_intermediate->zero_point->data[0];
      if (intermediate != nullptr) {
        micro_context->DeallocateTempTfLiteTensor(intermediate);
      }
    } else {
      // Q3.12 for activation functions.
      intermediate_scale[i] = std::pow(2.0f, -12.0f);
      intermediate_zp[i] = 0;
    }
  }
  // In the absence of projection, hidden becomes otuput and this intermediate
  // is ignored.
  TfLiteTensor* hidden = micro_context->AllocateTempIntermediateTensor(node, 4);
  TF_LITE_ENSURE(context, hidden->quantization.type != kTfLiteNoQuantization);
  auto* hidden_params =
      static_cast<TfLiteAffineQuantization*>(hidden->quantization.params);
  intermediate_scale[4] = hidden_params->scale->data[0];
  intermediate_zp[4] = hidden_params->zero_point->data[0];
  if (hidden != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(hidden);
  }

  // Scales.
  const float default_scale = 1.0;
  float input_scale = default_scale;
  float input_to_input_weight_scale = default_scale;
  float recurrent_to_input_weight_scale = default_scale;
  float cell_to_input_weight_scale = default_scale;
  float input_to_forget_weight_scale = default_scale;
  float recurrent_to_forget_weight_scale = default_scale;
  float cell_to_forget_weight_scale = default_scale;
  float input_to_cell_weight_scale = default_scale;
  float recurrent_to_cell_weight_scale = default_scale;
  float input_to_output_weight_scale = default_scale;
  float recurrent_to_output_weight_scale = default_scale;
  float cell_to_output_weight_scale = default_scale;
  float projection_weight_scale = default_scale;
  float layer_norm_input_scale = default_scale;
  float layer_norm_forget_scale = default_scale;
  float layer_norm_cell_scale = default_scale;
  float layer_norm_output_scale = default_scale;
  float output_state_scale = default_scale;
  int cell_scale = 1;

  // Effective scales.
  float effective_input_to_input_scale = default_scale;
  float effective_recurrent_to_input_scale = default_scale;
  float effective_cell_to_input_scale = default_scale;
  float effective_input_to_forget_scale = default_scale;
  float effective_recurrent_to_forget_scale = default_scale;
  float effective_cell_to_forget_scale = default_scale;
  float effective_input_to_cell_scale = default_scale;
  float effective_recurrent_to_cell_scale = default_scale;
  float effective_input_to_output_scale = default_scale;
  float effective_recurrent_to_output_scale = default_scale;
  float effective_cell_to_output_scale = default_scale;
  float effective_proj_scale = default_scale;
  float effective_hidden_scale = default_scale;

  // Populate scales.
  if (!use_cifg) {
    input_to_input_weight_scale = input_to_input_weights->params.scale;
    recurrent_to_input_weight_scale = recurrent_to_input_weights->params.scale;
  }

  if (use_peephole) {
    if (!use_cifg) {
      cell_to_input_weight_scale = cell_to_input_weights->params.scale;
    }
    cell_to_forget_weight_scale = cell_to_forget_weights->params.scale;
    cell_to_output_weight_scale = cell_to_output_weights->params.scale;
  }

  if (use_layer_norm) {
    if (!use_cifg) {
      layer_norm_input_scale = input_layer_norm_coefficients->params.scale;
    }
    layer_norm_forget_scale = forget_layer_norm_coefficients->params.scale;
    layer_norm_cell_scale = cell_layer_norm_coefficients->params.scale;
    layer_norm_output_scale = output_layer_norm_coefficients->params.scale;
  }

  if (use_projection) {
    projection_weight_scale = projection_weights->params.scale;
  }
  output_state_scale = output_state->params.scale;

  input_to_forget_weight_scale = input_to_forget_weights->params.scale;
  input_to_cell_weight_scale = input_to_cell_weights->params.scale;
  input_to_output_weight_scale = input_to_output_weights->params.scale;
  recurrent_to_forget_weight_scale = recurrent_to_forget_weights->params.scale;
  recurrent_to_cell_weight_scale = recurrent_to_cell_weights->params.scale;
  recurrent_to_output_weight_scale = recurrent_to_output_weights->params.scale;

  // Check cell state (already used above)
  TF_LITE_ENSURE(context, CheckedLog2(cell_state->params.scale, &cell_scale));
  // TF_LITE_ENSURE(context, cell_scale <= -9);
  integer_lstm_param->cell_scale = cell_scale;
  input_scale = input->params.scale;

  // Calculate effective scales.
  if (!use_cifg) {
    effective_input_to_input_scale =
        input_to_input_weight_scale * input_scale / intermediate_scale[0];
    effective_recurrent_to_input_scale = recurrent_to_input_weight_scale *
                                         output_state_scale /
                                         intermediate_scale[0];
  }
  effective_input_to_forget_scale =
      input_to_forget_weight_scale * input_scale / intermediate_scale[1];
  effective_recurrent_to_forget_scale = recurrent_to_forget_weight_scale *
                                        output_state_scale /
                                        intermediate_scale[1];

  effective_input_to_cell_scale =
      input_to_cell_weight_scale * input_scale / intermediate_scale[2];
  effective_recurrent_to_cell_scale = recurrent_to_cell_weight_scale *
                                      output_state_scale /
                                      intermediate_scale[2];

  effective_input_to_output_scale =
      input_to_output_weight_scale * input_scale / intermediate_scale[3];
  effective_recurrent_to_output_scale = recurrent_to_output_weight_scale *
                                        output_state_scale /
                                        intermediate_scale[3];

  effective_hidden_scale =
      std::pow(2.0f, -15.0f) / intermediate_scale[4] * std::pow(2.0f, -15.0f);

  effective_proj_scale =
      projection_weight_scale * intermediate_scale[4] / output_state_scale;

  if (use_peephole) {
    if (!use_cifg) {
      effective_cell_to_input_scale =
          std::pow(2.0f, static_cast<float>(cell_scale)) *
          cell_to_input_weight_scale / intermediate_scale[0];
    }
    effective_cell_to_forget_scale =
        std::pow(2.0f, static_cast<float>(cell_scale)) *
        cell_to_forget_weight_scale / intermediate_scale[1];
    effective_cell_to_output_scale =
        std::pow(2.0f, static_cast<float>(cell_scale)) *
        cell_to_output_weight_scale / intermediate_scale[3];
  }

  // Decompose scales.
  int shift_output;
  QuantizeMultiplier(static_cast<double>(effective_input_to_input_scale),
                     &integer_lstm_param->effective_input_to_input_scale_a,
                     &shift_output);
  integer_lstm_param->effective_input_to_input_scale_b =
      static_cast<int32_t>(shift_output);
  QuantizeMultiplier(static_cast<double>(effective_recurrent_to_input_scale),
                     &integer_lstm_param->effective_recurrent_to_input_scale_a,
                     &shift_output);
  integer_lstm_param->effective_recurrent_to_input_scale_b =
      static_cast<int32_t>(shift_output);
  QuantizeMultiplier(static_cast<double>(effective_cell_to_input_scale),
                     &integer_lstm_param->effective_cell_to_input_scale_a,
                     &shift_output);
  integer_lstm_param->effective_cell_to_input_scale_b =
      static_cast<int32_t>(shift_output);
  QuantizeMultiplier(static_cast<double>(effective_input_to_forget_scale),
                     &integer_lstm_param->effective_input_to_forget_scale_a,
                     &shift_output);
  integer_lstm_param->effective_input_to_forget_scale_b =
      static_cast<int32_t>(shift_output);
  QuantizeMultiplier(static_cast<double>(effective_recurrent_to_forget_scale),
                     &integer_lstm_param->effective_recurrent_to_forget_scale_a,
                     &shift_output);
  integer_lstm_param->effective_recurrent_to_forget_scale_b =
      static_cast<int32_t>(shift_output);
  QuantizeMultiplier(static_cast<double>(effective_cell_to_forget_scale),
                     &integer_lstm_param->effective_cell_to_forget_scale_a,
                     &shift_output);
  integer_lstm_param->effective_cell_to_forget_scale_b =
      static_cast<int32_t>(shift_output);
  QuantizeMultiplier(static_cast<double>(effective_input_to_cell_scale),
                     &integer_lstm_param->effective_input_to_cell_scale_a,
                     &shift_output);
  integer_lstm_param->effective_input_to_cell_scale_b =
      static_cast<int32_t>(shift_output);
  QuantizeMultiplier(static_cast<double>(effective_recurrent_to_cell_scale),
                     &integer_lstm_param->effective_recurrent_to_cell_scale_a,
                     &shift_output);
  integer_lstm_param->effective_recurrent_to_cell_scale_b =
      static_cast<int32_t>(shift_output);
  QuantizeMultiplier(static_cast<double>(effective_input_to_output_scale),
                     &integer_lstm_param->effective_input_to_output_scale_a,
                     &shift_output);
  integer_lstm_param->effective_input_to_output_scale_b =
      static_cast<int32_t>(shift_output);
  QuantizeMultiplier(static_cast<double>(effective_recurrent_to_output_scale),
                     &integer_lstm_param->effective_recurrent_to_output_scale_a,
                     &shift_output);
  integer_lstm_param->effective_recurrent_to_output_scale_b =
      static_cast<int32_t>(shift_output);
  QuantizeMultiplier(static_cast<double>(effective_cell_to_output_scale),
                     &integer_lstm_param->effective_cell_to_output_scale_a,
                     &shift_output);
  integer_lstm_param->effective_cell_to_output_scale_b =
      static_cast<int32_t>(shift_output);
  QuantizeMultiplier(static_cast<double>(effective_proj_scale),
                     &integer_lstm_param->effective_proj_scale_a,
                     &shift_output);
  integer_lstm_param->effective_proj_scale_b =
      static_cast<int32_t>(shift_output);
  QuantizeMultiplier(static_cast<double>(effective_hidden_scale),
                     &integer_lstm_param->effective_hidden_scale_a,
                     &shift_output);
  integer_lstm_param->effective_hidden_scale_b =
      static_cast<int32_t>(shift_output);
  QuantizeMultiplier(static_cast<double>(layer_norm_input_scale),
                     &integer_lstm_param->layer_norm_input_scale_a,
                     &shift_output);
  integer_lstm_param->layer_norm_input_scale_b =
      static_cast<int32_t>(shift_output);
  QuantizeMultiplier(static_cast<double>(layer_norm_forget_scale),
                     &integer_lstm_param->layer_norm_forget_scale_a,
                     &shift_output);
  integer_lstm_param->layer_norm_forget_scale_b =
      static_cast<int32_t>(shift_output);
  QuantizeMultiplier(static_cast<double>(layer_norm_cell_scale),
                     &integer_lstm_param->layer_norm_cell_scale_a,
                     &shift_output);
  integer_lstm_param->layer_norm_cell_scale_b =
      static_cast<int32_t>(shift_output);
  QuantizeMultiplier(static_cast<double>(layer_norm_output_scale),
                     &integer_lstm_param->layer_norm_output_scale_a,
                     &shift_output);
  integer_lstm_param->layer_norm_output_scale_b =
      static_cast<int32_t>(shift_output);

  integer_lstm_param->hidden_zp = intermediate_zp[4];

  // 10000 is used to make sure the kernel logic does not overflow.
  if (!use_cifg) {
    integer_lstm_param->input_variance_guard =
        std::max(1, static_cast<int>(10000 * layer_norm_input_scale));
  }
  integer_lstm_param->forget_variance_guard =
      std::max(1, static_cast<int>(10000 * layer_norm_forget_scale));
  integer_lstm_param->cell_variance_guard =
      std::max(1, static_cast<int>(10000 * layer_norm_cell_scale));
  integer_lstm_param->output_variance_guard =
      std::max(1, static_cast<int>(10000 * layer_norm_output_scale));

  if (cell_state != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(cell_state);
  }
  if (output_tensor != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(output_tensor);
  }
  if (input != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(input);
  }
  if (input_to_input_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(input_to_input_weights);
  }
  if (input_to_forget_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(input_to_forget_weights);
  }
  if (input_to_cell_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(input_to_cell_weights);
  }
  if (input_to_output_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(input_to_output_weights);
  }
  if (recurrent_to_input_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(recurrent_to_input_weights);
  }
  if (recurrent_to_forget_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(recurrent_to_forget_weights);
  }
  if (recurrent_to_cell_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(recurrent_to_cell_weights);
  }
  if (recurrent_to_output_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(recurrent_to_output_weights);
  }
  if (cell_to_input_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(cell_to_input_weights);
  }
  if (cell_to_forget_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(cell_to_forget_weights);
  }
  if (cell_to_output_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(cell_to_output_weights);
  }
  if (input_layer_norm_coefficients != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(input_layer_norm_coefficients);
  }
  if (forget_layer_norm_coefficients != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(forget_layer_norm_coefficients);
  }
  if (cell_layer_norm_coefficients != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(cell_layer_norm_coefficients);
  }
  if (output_layer_norm_coefficients != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(output_layer_norm_coefficients);
  }
  if (projection_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(projection_weights);
  }
  if (output_state != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(output_state);
  }

  return kTfLiteOk;
}

// Temporary buffers used for hybrid mode
enum HybridTempBuffer {
  kPrimaryScratchBuffer = 0,
  kInputQuantized = 1,
  kOutputStateQuantized = 2,
  kCellStateQuantized = 3,
  kInputScalingFactors = 4,
  kOutputStateScalingFactors = 5,
  kProductScalingFactors = 6,
  kRecoveredCellWeights = 7,
  kAccumScratch = 8,
  kInputZeroPoints = 9,
  kOutputStateZeroPoints = 10,
  kScales = 11,
  kNumHybridTempBuffers = 12,
};

void* UnidirectionalSequenceLstmInit(TfLiteContext* context, const char* buffer,
                                     size_t length) {
  TFLITE_DCHECK(context->AllocatePersistentBuffer != nullptr);
  return context->AllocatePersistentBuffer(
      context, sizeof(UnidirectionalSequenceLstmOpData));
}

// Check that input tensor dimensions matches with each other.
TfLiteStatus CheckInputTensorDimensions(TfLiteContext* context,
                                        TfLiteNode* node, int n_input,
                                        int n_output, int n_cell,
                                        bool use_layer_norm, bool is_integer) {
  MicroContext* micro_context = GetMicroContext(context);

  const auto* params = reinterpret_cast<TfLiteLSTMParams*>(node->builtin_data);

  // Making sure clipping parameters have valid values.
  // == 0 means no clipping
  //  > 0 means clipping
  TF_LITE_ENSURE(context, params->cell_clip >= 0);
  TF_LITE_ENSURE(context, params->proj_clip >= 0);

  TfLiteTensor* input_to_input_weights = micro_context->AllocateTempInputTensor(
      node, kLstmInputToInputWeightsTensor);
  if (input_to_input_weights != nullptr) {
    TF_LITE_ENSURE_EQ(context, input_to_input_weights->dims->size, 2);
    TF_LITE_ENSURE_EQ(context, input_to_input_weights->dims->data[0], n_cell);
    TF_LITE_ENSURE_EQ(context, input_to_input_weights->dims->data[1], n_input);
  }

  TfLiteTensor* input_to_forget_weights =
      micro_context->AllocateTempInputTensor(node,
                                             kLstmInputToForgetWeightsTensor);
  TF_LITE_ENSURE_EQ(context, input_to_forget_weights->dims->size, 2);
  TF_LITE_ENSURE_EQ(context, input_to_forget_weights->dims->data[0], n_cell);
  TF_LITE_ENSURE_EQ(context, input_to_forget_weights->dims->data[1], n_input);

  TfLiteTensor* input_to_cell_weights = micro_context->AllocateTempInputTensor(
      node, kLstmInputToCellWeightsTensor);
  TF_LITE_ENSURE_EQ(context, input_to_cell_weights->dims->size, 2);
  TF_LITE_ENSURE_EQ(context, input_to_cell_weights->dims->data[0], n_cell);
  TF_LITE_ENSURE_EQ(context, input_to_cell_weights->dims->data[1], n_input);

  TfLiteTensor* recurrent_to_input_weights =
      micro_context->AllocateTempInputTensor(
          node, kLstmRecurrentToInputWeightsTensor);
  if (recurrent_to_input_weights != nullptr) {
    TF_LITE_ENSURE_EQ(context, recurrent_to_input_weights->dims->size, 2);
    TF_LITE_ENSURE_EQ(context, recurrent_to_input_weights->dims->data[0],
                      n_cell);
    TF_LITE_ENSURE_EQ(context, recurrent_to_input_weights->dims->data[1],
                      n_output);
  }

  TfLiteTensor* recurrent_to_forget_weights =
      micro_context->AllocateTempInputTensor(
          node, kLstmRecurrentToForgetWeightsTensor);
  TF_LITE_ENSURE_EQ(context, recurrent_to_forget_weights->dims->size, 2);
  TF_LITE_ENSURE_EQ(context, recurrent_to_forget_weights->dims->data[0],
                    n_cell);
  TF_LITE_ENSURE_EQ(context, recurrent_to_forget_weights->dims->data[1],
                    n_output);

  TfLiteTensor* recurrent_to_cell_weights =
      micro_context->AllocateTempInputTensor(node,
                                             kLstmRecurrentToCellWeightsTensor);
  TF_LITE_ENSURE_EQ(context, recurrent_to_cell_weights->dims->size, 2);
  TF_LITE_ENSURE_EQ(context, recurrent_to_cell_weights->dims->data[0], n_cell);
  TF_LITE_ENSURE_EQ(context, recurrent_to_cell_weights->dims->data[1],
                    n_output);

  // We make sure the input-gate's parameters are either both present (regular
  // LSTM) or not at all (CIFG-LSTM).
  const bool cifg_weights_all_or_none =
      ((input_to_input_weights != nullptr) &&
       (recurrent_to_input_weights != nullptr)) ||
      ((input_to_input_weights == nullptr) &&
       (recurrent_to_input_weights == nullptr));
  TF_LITE_ENSURE(context, cifg_weights_all_or_none == true);

  TfLiteTensor* cell_to_input_weights = micro_context->AllocateTempInputTensor(
      node, kLstmCellToInputWeightsTensor);
  if (cell_to_input_weights != nullptr) {
    TF_LITE_ENSURE_EQ(context, cell_to_input_weights->dims->size, 1);
    TF_LITE_ENSURE_EQ(context, cell_to_input_weights->dims->data[0], n_cell);
    TF_LITE_ENSURE_TYPES_EQ(
        context, cell_to_input_weights->type,
        is_integer ? kTfLiteInt16 : input_to_forget_weights->type);
  }

  TfLiteTensor* cell_to_forget_weights = micro_context->AllocateTempInputTensor(
      node, kLstmCellToForgetWeightsTensor);
  if (cell_to_forget_weights != nullptr) {
    TF_LITE_ENSURE_EQ(context, cell_to_forget_weights->dims->size, 1);
    TF_LITE_ENSURE_EQ(context, cell_to_forget_weights->dims->data[0], n_cell);
    TF_LITE_ENSURE_TYPES_EQ(
        context, cell_to_forget_weights->type,
        is_integer ? kTfLiteInt16 : input_to_forget_weights->type);
  }

  TfLiteTensor* cell_to_output_weights = micro_context->AllocateTempInputTensor(
      node, kLstmCellToOutputWeightsTensor);
  if (cell_to_output_weights != nullptr) {
    TF_LITE_ENSURE_EQ(context, cell_to_output_weights->dims->size, 1);
    TF_LITE_ENSURE_EQ(context, cell_to_output_weights->dims->data[0], n_cell);
    TF_LITE_ENSURE_TYPES_EQ(
        context, cell_to_output_weights->type,
        is_integer ? kTfLiteInt16 : input_to_forget_weights->type);
  }

  // Making sure the peephole weights are there all or none.
  const bool use_cifg = (input_to_input_weights == nullptr);
  const bool peephole_weights_all_or_none =
      ((cell_to_input_weights != nullptr || use_cifg) &&
       (cell_to_forget_weights != nullptr) &&
       (cell_to_output_weights != nullptr)) ||
      ((cell_to_input_weights == nullptr) &&
       (cell_to_forget_weights == nullptr) &&
       (cell_to_output_weights == nullptr));
  TF_LITE_ENSURE(context, peephole_weights_all_or_none == true);

  // Make sure the input gate bias is present only when not a CIFG-LSTM.
  TfLiteTensor* input_gate_bias =
      micro_context->AllocateTempInputTensor(node, kLstmInputGateBiasTensor);
  if (use_cifg) {
    TF_LITE_ENSURE_EQ(context, input_gate_bias, nullptr);
  } else {
    TF_LITE_ENSURE_EQ(context, input_gate_bias->dims->size, 1);
    TF_LITE_ENSURE_EQ(context, input_gate_bias->dims->data[0], n_cell);
    if (is_integer) {
      TF_LITE_ENSURE_TYPES_EQ(context, input_gate_bias->type, kTfLiteInt32);
    } else {
      TF_LITE_ENSURE_TYPES_EQ(context, input_gate_bias->type, kTfLiteFloat32);
    }
  }

  TfLiteTensor* forget_gate_bias =
      micro_context->AllocateTempInputTensor(node, kLstmForgetGateBiasTensor);
  TF_LITE_ENSURE_EQ(context, forget_gate_bias->dims->size, 1);
  TF_LITE_ENSURE_EQ(context, forget_gate_bias->dims->data[0], n_cell);
  if (is_integer) {
    TF_LITE_ENSURE_TYPES_EQ(context, forget_gate_bias->type, kTfLiteInt32);
  } else {
    TF_LITE_ENSURE_TYPES_EQ(context, forget_gate_bias->type, kTfLiteFloat32);
  }

  TfLiteTensor* cell_gate_bias =
      micro_context->AllocateTempInputTensor(node, kLstmCellGateBiasTensor);
  TF_LITE_ENSURE_EQ(context, cell_gate_bias->dims->size, 1);
  TF_LITE_ENSURE_EQ(context, cell_gate_bias->dims->data[0], n_cell);
  if (is_integer) {
    TF_LITE_ENSURE_TYPES_EQ(context, cell_gate_bias->type, kTfLiteInt32);
  } else {
    TF_LITE_ENSURE_TYPES_EQ(context, cell_gate_bias->type, kTfLiteFloat32);
  }

  TfLiteTensor* output_gate_bias =
      micro_context->AllocateTempInputTensor(node, kLstmOutputGateBiasTensor);
  TF_LITE_ENSURE_EQ(context, output_gate_bias->dims->size, 1);
  TF_LITE_ENSURE_EQ(context, output_gate_bias->dims->data[0], n_cell);
  if (is_integer) {
    TF_LITE_ENSURE_TYPES_EQ(context, output_gate_bias->type, kTfLiteInt32);
  } else {
    TF_LITE_ENSURE_TYPES_EQ(context, output_gate_bias->type, kTfLiteFloat32);
  }

  TfLiteTensor* projection_weights = micro_context->AllocateTempInputTensor(
      node, kLstmProjectionWeightsTensor);
  if (projection_weights != nullptr) {
    TF_LITE_ENSURE_EQ(context, projection_weights->dims->size, 2);
    TF_LITE_ENSURE_EQ(context, projection_weights->dims->data[0], n_output);
    TF_LITE_ENSURE_EQ(context, projection_weights->dims->data[1], n_cell);
  }

  TfLiteTensor* projection_bias =
      micro_context->AllocateTempInputTensor(node, kLstmProjectionBiasTensor);
  if (projection_bias != nullptr) {
    TF_LITE_ENSURE_EQ(context, projection_bias->dims->size, 1);
    TF_LITE_ENSURE_EQ(context, projection_bias->dims->data[0], n_output);
    if (is_integer) {
      TF_LITE_ENSURE_TYPES_EQ(context, projection_bias->type, kTfLiteInt32);
    } else {
      TF_LITE_ENSURE_TYPES_EQ(context, projection_bias->type, kTfLiteFloat32);
    }
  }

  // Making sure the projection tensors are consistent:
  // 1) If projection weight is not present, then projection bias should not be
  // present.
  // 2) If projection weight is present, then projection bias is optional.
  const bool projecton_tensors_consistent =
      ((projection_weights != nullptr) || (projection_bias == nullptr));
  TF_LITE_ENSURE(context, projecton_tensors_consistent == true);

  if (use_layer_norm) {
    TfLiteTensor* input_layer_norm_coefficients =
        micro_context->AllocateTempInputTensor(
            node, kLstmInputLayerNormCoefficientsTensor);
    if (use_cifg) {
      TF_LITE_ENSURE_EQ(context, input_layer_norm_coefficients, nullptr);
    } else {
      TF_LITE_ENSURE(context, input_layer_norm_coefficients != nullptr);
      TF_LITE_ENSURE_EQ(context, input_layer_norm_coefficients->dims->size, 1);
      TF_LITE_ENSURE_EQ(context, input_layer_norm_coefficients->dims->data[0],
                        n_cell);
      if (is_integer) {
        TF_LITE_ENSURE_TYPES_EQ(context, input_layer_norm_coefficients->type,
                                kTfLiteInt16);
      } else {
        TF_LITE_ENSURE_TYPES_EQ(context, input_layer_norm_coefficients->type,
                                kTfLiteFloat32);
      }
    }

    TfLiteTensor* forget_layer_norm_coefficients =
        micro_context->AllocateTempInputTensor(
            node, kLstmForgetLayerNormCoefficientsTensor);
    TF_LITE_ENSURE_EQ(context, forget_layer_norm_coefficients->dims->size, 1);
    TF_LITE_ENSURE_EQ(context, forget_layer_norm_coefficients->dims->data[0],
                      n_cell);
    if (is_integer) {
      TF_LITE_ENSURE_TYPES_EQ(context, forget_layer_norm_coefficients->type,
                              kTfLiteInt16);
    } else {
      TF_LITE_ENSURE_TYPES_EQ(context, forget_layer_norm_coefficients->type,
                              kTfLiteFloat32);
    }

    TfLiteTensor* cell_layer_norm_coefficients =
        micro_context->AllocateTempInputTensor(
            node, kLstmCellLayerNormCoefficientsTensor);
    TF_LITE_ENSURE_EQ(context, cell_layer_norm_coefficients->dims->size, 1);
    TF_LITE_ENSURE_EQ(context, cell_layer_norm_coefficients->dims->data[0],
                      n_cell);
    if (is_integer) {
      TF_LITE_ENSURE_TYPES_EQ(context, cell_layer_norm_coefficients->type,
                              kTfLiteInt16);
    } else {
      TF_LITE_ENSURE_TYPES_EQ(context, cell_layer_norm_coefficients->type,
                              kTfLiteFloat32);
    }

    TfLiteTensor* output_layer_norm_coefficients =
        micro_context->AllocateTempInputTensor(
            node, kLstmOutputLayerNormCoefficientsTensor);
    TF_LITE_ENSURE_EQ(context, output_layer_norm_coefficients->dims->size, 1);
    TF_LITE_ENSURE_EQ(context, output_layer_norm_coefficients->dims->data[0],
                      n_cell);
    if (is_integer) {
      TF_LITE_ENSURE_TYPES_EQ(context, output_layer_norm_coefficients->type,
                              kTfLiteInt16);
    } else {
      TF_LITE_ENSURE_TYPES_EQ(context, output_layer_norm_coefficients->type,
                              kTfLiteFloat32);
    }
    if (input_layer_norm_coefficients != nullptr) {
      micro_context->DeallocateTempTfLiteTensor(input_layer_norm_coefficients);
    }
    if (forget_layer_norm_coefficients != nullptr) {
      micro_context->DeallocateTempTfLiteTensor(forget_layer_norm_coefficients);
    }
    if (cell_layer_norm_coefficients != nullptr) {
      micro_context->DeallocateTempTfLiteTensor(cell_layer_norm_coefficients);
    }
    if (output_layer_norm_coefficients != nullptr) {
      micro_context->DeallocateTempTfLiteTensor(output_layer_norm_coefficients);
    }
  }

  if (input_to_input_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(input_to_input_weights);
  }
  if (input_to_forget_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(input_to_forget_weights);
  }
  if (input_to_cell_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(input_to_cell_weights);
  }
  if (recurrent_to_input_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(recurrent_to_input_weights);
  }
  if (recurrent_to_forget_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(recurrent_to_forget_weights);
  }
  micro_context->DeallocateTempTfLiteTensor(recurrent_to_cell_weights);
  if (cell_to_input_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(cell_to_input_weights);
  }
  if (cell_to_forget_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(cell_to_forget_weights);
  }
  if (cell_to_output_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(cell_to_output_weights);
  }
  if (input_gate_bias != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(input_gate_bias);
  }
  if (forget_gate_bias != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(forget_gate_bias);
  }
  if (cell_gate_bias != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(cell_gate_bias);
  }
  if (output_gate_bias != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(output_gate_bias);
  }
  if (projection_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(projection_weights);
  }
  if (projection_bias != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(projection_bias);
  }

  return kTfLiteOk;
}

TfLiteStatus PrecomputeZeroPointTimesWeightWithBias(
    TfLiteContext* context, int32_t zero_point,
    const TfLiteTensor* weight_tensor, const TfLiteTensor* bias_tensor,
    int32_t** output) {
  if (weight_tensor == nullptr) {
    return kTfLiteOk;
  }

  const RuntimeShape& weight_shape = GetTensorShape(weight_tensor);
  TF_LITE_ENSURE_EQ(context, weight_shape.DimensionsCount(), 2);
  const int row = weight_shape.Dims(0);
  const int col = weight_shape.Dims(1);
  TFLITE_DCHECK(context->AllocatePersistentBuffer != nullptr);
  *output = static_cast<int32_t*>(
      context->AllocatePersistentBuffer(context, row * sizeof(int32_t)));

  if (bias_tensor == nullptr) {
    memset(*output, 0, row * sizeof(int32_t));
  } else {
    const int32_t* bias = GetTensorData<int32_t>(bias_tensor);
    memcpy(*output, bias, row * sizeof(int32_t));
  }
  if (zero_point != 0) {
    const int8_t* weight = GetTensorData<int8_t>(weight_tensor);
    tflite::tensor_utils::MatrixScalarMultiplyAccumulate(weight, zero_point,
                                                         row, col, *output);
  }
  return kTfLiteOk;
}

TfLiteStatus PopulatePrecomputedZPTimesWeightsWithBias(
    TfLiteContext* context, UnidirectionalSequenceLstmOpData* op_data,
    TfLiteNode* node) {
  MicroContext* micro_context = GetMicroContext(context);

  TfLiteTensor* input =
      micro_context->AllocateTempInputTensor(node, kLstmInputTensor);
  TfLiteTensor* output_state =
      micro_context->AllocateTempInputTensor(node, kLstmOutputStateTensor);
  TF_LITE_ENSURE(context, output_state != nullptr);
  TF_LITE_ENSURE(context, output_state->is_variable);

  const int32_t input_zero_point = -input->params.zero_point;
  const int32_t output_state_zero_point = -output_state->params.zero_point;

  TfLiteTensor* input_to_input_weights = micro_context->AllocateTempInputTensor(
      node, kLstmInputToInputWeightsTensor);
  TfLiteTensor* input_to_forget_weights =
      micro_context->AllocateTempInputTensor(node,
                                             kLstmInputToForgetWeightsTensor);
  TfLiteTensor* input_to_cell_weights = micro_context->AllocateTempInputTensor(
      node, kLstmInputToCellWeightsTensor);
  TfLiteTensor* input_to_output_weights =
      micro_context->AllocateTempInputTensor(node,
                                             kLstmInputToOutputWeightsTensor);

  TfLiteTensor* recurrent_to_input_weights =
      micro_context->AllocateTempInputTensor(
          node, kLstmRecurrentToInputWeightsTensor);
  TfLiteTensor* recurrent_to_forget_weights =
      micro_context->AllocateTempInputTensor(
          node, kLstmRecurrentToForgetWeightsTensor);
  TfLiteTensor* recurrent_to_cell_weights =
      micro_context->AllocateTempInputTensor(node,
                                             kLstmRecurrentToCellWeightsTensor);
  TfLiteTensor* recurrent_to_output_weights =
      micro_context->AllocateTempInputTensor(
          node, kLstmRecurrentToOutputWeightsTensor);

  TfLiteTensor* projection_weights = micro_context->AllocateTempInputTensor(
      node, kLstmProjectionWeightsTensor);
  TfLiteTensor* projection_bias =
      micro_context->AllocateTempInputTensor(node, kLstmProjectionBiasTensor);

  IntegerLstmParameter* integer_lstm_params = &op_data->integer_lstm_param;

  TfLiteTensor* intermediate =
      micro_context->AllocateTempIntermediateTensor(node, 4);
  TF_LITE_ENSURE(context,
                 intermediate->quantization.type != kTfLiteNoQuantization);
  const auto* params =
      static_cast<TfLiteAffineQuantization*>(intermediate->quantization.params);
  const int32_t hidden_zp = params->zero_point->data[0];

  // Get bias and perform zero point calculation.
  // When there is layer normalization, the gate bias does not apply to matmul
  // directly:
  //      y = ln(w * x + w * r + w * c) + b.
  const bool is_layer_norm = op_data->use_layer_norm;

  // Forget gate.
  TfLiteTensor* forget_gate_bias = is_layer_norm
                                       ? nullptr
                                       : micro_context->AllocateTempInputTensor(
                                             node, kLstmForgetGateBiasTensor);
  TF_LITE_ENSURE_OK(
      context,
      PrecomputeZeroPointTimesWeightWithBias(
          context, input_zero_point, input_to_forget_weights, forget_gate_bias,
          &(integer_lstm_params->input_to_forget_effective_bias)));

  TF_LITE_ENSURE_OK(
      context,
      PrecomputeZeroPointTimesWeightWithBias(
          context, output_state_zero_point, recurrent_to_forget_weights,
          nullptr, &(integer_lstm_params->recurrent_to_forget_effective_bias)));

  // Modulation gate.
  TfLiteTensor* cell_gate_bias = is_layer_norm
                                     ? nullptr
                                     : micro_context->AllocateTempInputTensor(
                                           node, kLstmCellGateBiasTensor);
  TF_LITE_ENSURE_OK(
      context,
      PrecomputeZeroPointTimesWeightWithBias(
          context, input_zero_point, input_to_cell_weights, cell_gate_bias,
          &(integer_lstm_params->input_to_cell_effective_bias)));
  TF_LITE_ENSURE_OK(
      context,
      PrecomputeZeroPointTimesWeightWithBias(
          context, output_state_zero_point, recurrent_to_cell_weights, nullptr,
          &(integer_lstm_params->recurrent_to_cell_effective_bias)));

  // Output gate.
  TfLiteTensor* output_gate_bias = is_layer_norm
                                       ? nullptr
                                       : micro_context->AllocateTempInputTensor(
                                             node, kLstmOutputGateBiasTensor);
  TF_LITE_ENSURE_OK(
      context,
      PrecomputeZeroPointTimesWeightWithBias(
          context, input_zero_point, input_to_output_weights, output_gate_bias,
          &(integer_lstm_params->input_to_output_effective_bias)));

  TF_LITE_ENSURE_OK(
      context,
      PrecomputeZeroPointTimesWeightWithBias(
          context, output_state_zero_point, recurrent_to_output_weights,
          nullptr, &(integer_lstm_params->recurrent_to_output_effective_bias)));

  // Input gate. The calculation is only meaningful for non-cifg case.
  TfLiteTensor* input_gate_bias = is_layer_norm
                                      ? nullptr
                                      : micro_context->AllocateTempInputTensor(
                                            node, kLstmInputGateBiasTensor);
  TF_LITE_ENSURE_OK(
      context,
      PrecomputeZeroPointTimesWeightWithBias(
          context, input_zero_point, input_to_input_weights, input_gate_bias,
          &(integer_lstm_params->input_to_input_effective_bias)));
  TF_LITE_ENSURE_OK(
      context,
      PrecomputeZeroPointTimesWeightWithBias(
          context, output_state_zero_point, recurrent_to_input_weights, nullptr,
          &(integer_lstm_params->recurrent_to_input_effective_bias)));

  // Projection bias. The calculation is only meaningful for with projection.
  TF_LITE_ENSURE_OK(context,
                    PrecomputeZeroPointTimesWeightWithBias(
                        context, hidden_zp, projection_weights, projection_bias,
                        &(integer_lstm_params->projection_effective_bias)));

  if (input != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(input);
  }
  if (output_state != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(output_state);
  }
  if (input_to_input_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(input_to_input_weights);
  }
  if (input_to_forget_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(input_to_forget_weights);
  }
  if (input_to_cell_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(input_to_cell_weights);
  }
  if (input_to_output_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(input_to_output_weights);
  }
  if (recurrent_to_input_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(recurrent_to_input_weights);
  }
  if (recurrent_to_forget_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(recurrent_to_forget_weights);
  }
  if (recurrent_to_cell_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(recurrent_to_cell_weights);
  }
  if (recurrent_to_output_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(recurrent_to_output_weights);
  }
  if (projection_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(projection_weights);
  }
  if (projection_bias != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(projection_bias);
  }
  if (forget_gate_bias != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(forget_gate_bias);
  }
  if (cell_gate_bias != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(cell_gate_bias);
  }
  if (output_gate_bias != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(output_gate_bias);
  }
  if (input_gate_bias != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(input_gate_bias);
  }

  if (intermediate != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(intermediate);
  }

  return kTfLiteOk;
}

// Resize the output and  state tensors based on the sizes of the input tensors.
// Allocate a temporary scratch tensor. Also check that the sizes of the input
// tensors match each other.
TfLiteStatus UnidirectionalSequenceLstmPrepare(TfLiteContext* context,
                                               TfLiteNode* node) {
  UnidirectionalSequenceLstmOpData* op_data =
      reinterpret_cast<UnidirectionalSequenceLstmOpData*>(node->user_data);

  MicroContext* micro_context = GetMicroContext(context);

  // Check we have all the inputs and outputs we need.
  bool use_layer_norm = false;
  if (node->inputs->size == 24) {
    TfLiteTensor* forget_layer_norm_coefficients =
        micro_context->AllocateTempInputTensor(
            node, kLstmForgetLayerNormCoefficientsTensor);
    if (forget_layer_norm_coefficients == nullptr) {
      use_layer_norm = false;
    } else {
      use_layer_norm = true;
    }
    if (forget_layer_norm_coefficients != nullptr) {
      micro_context->DeallocateTempTfLiteTensor(forget_layer_norm_coefficients);
    }
  } else if (node->inputs->size == 20) {
    // This is deprecated and is only kept here for backward compatibility.
    use_layer_norm = false;
  } else {
    MicroPrintf("The LSTM Full kernel expects 20 or 24 inputs. Got %d inputs",
                node->inputs->size);
    return kTfLiteError;
  }
  TF_LITE_ENSURE_EQ(context, node->outputs->size, 1);
  op_data->use_layer_norm = use_layer_norm;

  // Inferring batch size, number of outputs and sequence length and
  // number of cells from the input tensors.
  TfLiteTensor* input =
      micro_context->AllocateTempInputTensor(node, kLstmInputTensor);
  op_data->input_zero_point = input->params.zero_point;
  const bool is_integer = input->type == kTfLiteInt8;
  TF_LITE_ENSURE(context, input->dims->size > 1);
  const auto* params =
      reinterpret_cast<TfLiteUnidirectionalSequenceLSTMParams*>(
          node->builtin_data);
  const bool time_major = params->time_major;
  const int n_batch = time_major ? input->dims->data[1] : input->dims->data[0];
  const int n_input = input->dims->data[2];

  TfLiteTensor* input_to_output_weights =
      micro_context->AllocateTempInputTensor(node,
                                             kLstmInputToOutputWeightsTensor);
  const int n_cell = input_to_output_weights->dims->data[0];
  TF_LITE_ENSURE_EQ(context, input_to_output_weights->dims->size, 2);
  TF_LITE_ENSURE_EQ(context, input_to_output_weights->dims->data[1], n_input);

  TfLiteTensor* recurrent_to_output_weights =
      micro_context->AllocateTempInputTensor(
          node, kLstmRecurrentToOutputWeightsTensor);
  TF_LITE_ENSURE_EQ(context, recurrent_to_output_weights->dims->size, 2);
  TF_LITE_ENSURE_EQ(context, recurrent_to_output_weights->dims->data[0],
                    n_cell);
  const int n_output = recurrent_to_output_weights->dims->data[1];

  // Check that input tensor dimensions matches with each other.
  TF_LITE_ENSURE_OK(
      context, CheckInputTensorDimensions(context, node, n_input, n_output,
                                          n_cell, use_layer_norm, is_integer));

  // Get the pointer to output, output_state and cell_state buffer tensors.
  TfLiteTensor* output =
      micro_context->AllocateTempOutputTensor(node, kLstmOutputTensor);

  TfLiteTensor* output_state =
      micro_context->AllocateTempInputTensor(node, kLstmOutputStateTensor);
  TF_LITE_ENSURE(context, output_state != nullptr);
  TF_LITE_ENSURE(context, output_state->is_variable);
  op_data->output_state_zero_point = output_state->params.zero_point;
  TfLiteTensor* cell_state =
      micro_context->AllocateTempInputTensor(node, kLstmCellStateTensor);
  TF_LITE_ENSURE(context, cell_state != nullptr);
  TF_LITE_ENSURE(context, cell_state->is_variable);

  // Check the shape of input state tensors.
  // These tensor may be 1D or 2D. It's fine as long as the total size is
  // correct.
  TF_LITE_ENSURE_EQ(context, NumElements(output_state), n_batch * n_output);
  TF_LITE_ENSURE_EQ(context, NumElements(cell_state), n_batch * n_cell);

  // Check the shape of output tensor against that of input tensor
  TF_LITE_ENSURE_EQ(context, output->dims->size, 3);
  TF_LITE_ENSURE_EQ(context, input->dims->data[0], output->dims->data[0]);
  TF_LITE_ENSURE_EQ(context, input->dims->data[1], output->dims->data[1]);
  TF_LITE_ENSURE_EQ(context, output->dims->data[2], n_output);

  if (is_integer) {
    const int num_intermediate_tensors = node->intermediates->size;
    TF_LITE_ENSURE(context, num_intermediate_tensors == 5);
  }

  TfLiteTensor* input_to_input_weights = micro_context->AllocateTempInputTensor(
      node, kLstmInputToInputWeightsTensor);

  const bool use_cifg = (input_to_input_weights == nullptr);

  // Create a primary scratch buffer for hybrid and float
  // If is_integer, primary scratch buffer has a different size
  if (!is_integer) {
    int scratch_buffer_size[2];
    scratch_buffer_size[0] = n_batch;

    if (use_cifg) {
      // Reserving space for Cell, Forget, Output gates
      scratch_buffer_size[1] = n_cell * 3;
    } else {
      // Reserving space for Input, Cell, Forget, Output gates
      scratch_buffer_size[1] = n_cell * 4;
    }

    TF_LITE_ENSURE_OK(context,
                      context->RequestScratchBufferInArena(
                          context,
                          scratch_buffer_size[0] * scratch_buffer_size[1] *
                              TfLiteTypeGetSize(input->type),
                          &(op_data->scratch_index[kPrimaryScratchBuffer])));
  }

  if (is_integer) {
    // Integer UnidirectionalSequenceLSTM prepare function for 8x8->16.
    // This code path needs 5 intermediate tensors per Op.
    // Populate quantization parameters.
    PopulateQuantizedLstmParams8x8_16(context, node,
                                      &op_data->integer_lstm_param);
    // Allocate scratch buffer. Need 4 16-bit buffer with size n_batch * n_cell
    // and 1 8-bit buffer with size n_batch * n_cell. For integer
    // UnidirectionalSequenceLSTM, we do not need the extra 32-bit buffer.
    for (int i = 0; i < 5; ++i) {
      TfLiteType buffer_type = kTfLiteInt16;

      if (i == 4) {
        buffer_type = kTfLiteInt8;
      }

      TF_LITE_ENSURE_OK(
          context,
          context->RequestScratchBufferInArena(
              context, n_batch * n_cell * TfLiteTypeGetSize(buffer_type),
              &(op_data->scratch_index[i])));
    }

    // Populate precomputed zp * weight.
    TF_LITE_ENSURE_OK(context, PopulatePrecomputedZPTimesWeightsWithBias(
                                   context, op_data, node));
  }

  if (input != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(input);
  }
  if (input_to_output_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(input_to_output_weights);
  }
  if (recurrent_to_output_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(recurrent_to_output_weights);
  }
  if (output != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(output);
  }
  if (output_state != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(output_state);
  }
  if (cell_state != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(cell_state);
  }

  if (input_to_input_weights != nullptr) {
    micro_context->DeallocateTempTfLiteTensor(input_to_input_weights);
  }
  return kTfLiteOk;
}

TfLiteStatus UnidirectionalSequenceLstmEval(TfLiteContext* context,
                                            TfLiteNode* node) {
  TFLITE_DCHECK(context->GetScratchBuffer != nullptr);

  const auto* params =
      reinterpret_cast<TfLiteUnidirectionalSequenceLSTMParams*>(
          node->builtin_data);
  const UnidirectionalSequenceLstmOpData* op_data =
      reinterpret_cast<UnidirectionalSequenceLstmOpData*>(node->user_data);
  const bool use_layer_norm = op_data->use_layer_norm;
  const bool time_major = params->time_major;

  const TfLiteEvalTensor* input =
      tflite::micro::GetEvalInput(context, node, kLstmInputTensor);

  const TfLiteEvalTensor* input_to_input_weights = tflite::micro::GetEvalInput(
      context, node, kLstmInputToInputWeightsTensor);

  const TfLiteEvalTensor* input_to_forget_weights = tflite::micro::GetEvalInput(
      context, node, kLstmInputToForgetWeightsTensor);

  const TfLiteEvalTensor* input_to_cell_weights =
      tflite::micro::GetEvalInput(context, node, kLstmInputToCellWeightsTensor);

  const TfLiteEvalTensor* input_to_output_weights = tflite::micro::GetEvalInput(
      context, node, kLstmInputToOutputWeightsTensor);

  const TfLiteEvalTensor* recurrent_to_input_weights =
      tflite::micro::GetEvalInput(context, node,
                                  kLstmRecurrentToInputWeightsTensor);

  const TfLiteEvalTensor* recurrent_to_forget_weights =
      tflite::micro::GetEvalInput(context, node,
                                  kLstmRecurrentToForgetWeightsTensor);

  const TfLiteEvalTensor* recurrent_to_cell_weights =
      tflite::micro::GetEvalInput(context, node,
                                  kLstmRecurrentToCellWeightsTensor);

  const TfLiteEvalTensor* recurrent_to_output_weights =
      tflite::micro::GetEvalInput(context, node,
                                  kLstmRecurrentToOutputWeightsTensor);

  const TfLiteEvalTensor* cell_to_input_weights =
      tflite::micro::GetEvalInput(context, node, kLstmCellToInputWeightsTensor);

  const TfLiteEvalTensor* cell_to_forget_weights = tflite::micro::GetEvalInput(
      context, node, kLstmCellToForgetWeightsTensor);

  const TfLiteEvalTensor* cell_to_output_weights = tflite::micro::GetEvalInput(
      context, node, kLstmCellToOutputWeightsTensor);

  const TfLiteEvalTensor* input_gate_bias =
      tflite::micro::GetEvalInput(context, node, kLstmInputGateBiasTensor);

  const TfLiteEvalTensor* forget_gate_bias =
      tflite::micro::GetEvalInput(context, node, kLstmForgetGateBiasTensor);

  const TfLiteEvalTensor* cell_gate_bias =
      tflite::micro::GetEvalInput(context, node, kLstmCellGateBiasTensor);

  const TfLiteEvalTensor* output_gate_bias =
      tflite::micro::GetEvalInput(context, node, kLstmOutputGateBiasTensor);

  const TfLiteEvalTensor* projection_weights =
      tflite::micro::GetEvalInput(context, node, kLstmProjectionWeightsTensor);

  const TfLiteEvalTensor* projection_bias =
      tflite::micro::GetEvalInput(context, node, kLstmProjectionBiasTensor);

  TfLiteEvalTensor* output_state =
      tflite::micro::GetMutableEvalInput(context, node, kLstmOutputStateTensor);

  TfLiteEvalTensor* cell_state =
      tflite::micro::GetMutableEvalInput(context, node, kLstmCellStateTensor);

  TFLITE_DCHECK(cell_state != nullptr);

  const TfLiteEvalTensor* input_layer_norm_coefficients =
      use_layer_norm ? tflite::micro::GetEvalInput(
                           context, node, kLstmInputLayerNormCoefficientsTensor)
                     : nullptr;
  const TfLiteEvalTensor* forget_layer_norm_coefficients =
      use_layer_norm
          ? tflite::micro::GetEvalInput(context, node,
                                        kLstmForgetLayerNormCoefficientsTensor)
          : nullptr;
  const TfLiteEvalTensor* cell_layer_norm_coefficients =
      use_layer_norm ? tflite::micro::GetEvalInput(
                           context, node, kLstmCellLayerNormCoefficientsTensor)
                     : nullptr;
  const TfLiteEvalTensor* output_layer_norm_coefficients =
      use_layer_norm
          ? tflite::micro::GetEvalInput(context, node,
                                        kLstmOutputLayerNormCoefficientsTensor)
          : nullptr;

  TfLiteEvalTensor* output =
      tflite::micro::GetEvalOutput(context, node, kLstmOutputTensor);

  // Copy out the LSTM specific params so they can be passed in the function.
  TfLiteLSTMParams lstm_params;
  lstm_params.activation = params->activation;
  lstm_params.cell_clip = params->cell_clip;
  lstm_params.proj_clip = params->proj_clip;
  lstm_params.asymmetric_quantize_inputs = params->asymmetric_quantize_inputs;

  switch (input_to_output_weights->type) {
    case kTfLiteFloat32: {
      // Index the scratch buffers pointers to the global scratch buffer.
      return EvalFloatLstm(
          input, input_to_input_weights, input_to_forget_weights,
          input_to_cell_weights, input_to_output_weights,
          recurrent_to_input_weights, recurrent_to_forget_weights,
          recurrent_to_cell_weights, recurrent_to_output_weights,
          cell_to_input_weights, cell_to_forget_weights, cell_to_output_weights,
          input_layer_norm_coefficients, forget_layer_norm_coefficients,
          cell_layer_norm_coefficients, output_layer_norm_coefficients,
          /*aux_input=*/nullptr,
          /*aux_input_to_input_weights=*/nullptr,
          /*aux_input_to_forget_weights=*/nullptr,
          /*aux_input_to_cell_weights=*/nullptr,
          /*aux_input_to_output_weights=*/nullptr, input_gate_bias,
          forget_gate_bias, cell_gate_bias, output_gate_bias,
          projection_weights, projection_bias, &lstm_params,
          /*forward_sequence=*/true, time_major,
          /*output_offset=*/0,
          reinterpret_cast<float*>(context->GetScratchBuffer(
              context, op_data->scratch_index[kPrimaryScratchBuffer])),
          output_state, cell_state, output);
    } break;
    case kTfLiteUInt8:
    case kTfLiteInt8: {
      return EvalInteger8x8_16Lstm(
          input, input_to_input_weights, input_to_forget_weights,
          input_to_cell_weights, input_to_output_weights,
          recurrent_to_input_weights, recurrent_to_forget_weights,
          recurrent_to_cell_weights, recurrent_to_output_weights,
          cell_to_input_weights, cell_to_forget_weights, cell_to_output_weights,
          input_layer_norm_coefficients, forget_layer_norm_coefficients,
          cell_layer_norm_coefficients, output_layer_norm_coefficients,
          input_gate_bias, forget_gate_bias, cell_gate_bias, output_gate_bias,
          projection_weights, projection_bias, &lstm_params,
          /*forward_sequence=*/true, time_major, &op_data->integer_lstm_param,
          op_data->output_state_zero_point, output_state, cell_state, output,
          reinterpret_cast<int16_t*>(
              context->GetScratchBuffer(context, op_data->scratch_index[0])),
          reinterpret_cast<int16_t*>(
              context->GetScratchBuffer(context, op_data->scratch_index[1])),
          reinterpret_cast<int16_t*>(
              context->GetScratchBuffer(context, op_data->scratch_index[2])),
          reinterpret_cast<int16_t*>(
              context->GetScratchBuffer(context, op_data->scratch_index[3])),
          reinterpret_cast<int8_t*>(
              context->GetScratchBuffer(context, op_data->scratch_index[4])),
          nullptr);
    } break;
    default:
      MicroPrintf("Type %s is not currently supported.",
                  TfLiteTypeGetName(input_to_output_weights->type));
      return kTfLiteError;
  }
}

}  // namespace

TfLiteRegistration Register_UNIDIRECTIONAL_SEQUENCE_LSTM() {
  return tflite::micro::RegisterOp(UnidirectionalSequenceLstmInit,
                                   UnidirectionalSequenceLstmPrepare,
                                   UnidirectionalSequenceLstmEval);
}

}  // namespace tflite
