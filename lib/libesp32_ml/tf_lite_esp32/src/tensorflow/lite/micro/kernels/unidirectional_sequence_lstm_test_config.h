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
#ifndef TENSORFLOW_LITE_MICRO_KERNELS_UNIDIRECTIONAL_SEQUENCE_LSTM_TEST_CONFIG_H_
#define TENSORFLOW_LITE_MICRO_KERNELS_UNIDIRECTIONAL_SEQUENCE_LSTM_TEST_CONFIG_H_

#include "tensorflow/lite/c/common.h"

namespace tflite {
namespace testing {

// TODO(b/230666079) enable below tests for xtensa when the xtensa
// kernel is reconciled with reference kernel
#if !defined(XTENSA)

struct LstmIntegerTestConfig {
  const int n_batch;
  const int n_input;
  const int n_cell;
  const int n_output;
  const int sequence_length;
  const bool time_major;
  const bool use_cifg;
  const bool use_peephole;
  const bool use_projection_weights;
  const bool use_projection_bias;
  const bool use_layer_norm;
  const bool use_8x8_8_implementation;
  float intermediate_scale[5][2];
  int intermediate_zp[5][2];
  TfLiteAffineQuantization* intermediate_qparam;

  const float* input;
  int8_t* input_quant;

  const float* input_to_input_weights;
  int8_t* lstm_i2i_quant;
  const float* input_to_forget_weights;
  int8_t* lstm_i2f_quant;
  const float* input_to_cell_weights;
  int8_t* lstm_i2c_quant;
  const float* input_to_output_weights;
  int8_t* lstm_i2o_quant;

  const float* recurrent_to_input_weights;
  int8_t* lstm_r2i_quant;
  const float* recurrent_to_forget_weights;
  int8_t* lstm_r2f_quant;
  const float* recurrent_to_cell_weights;
  int8_t* lstm_r2c_quant;
  const float* recurrent_to_output_weights;
  int8_t* lstm_r2o_quant;

  const float* cell_to_input_weights;
  int16_t* lstm_c2i_quant;
  const float* cell_to_forget_weights;
  int16_t* lstm_c2f_quant;
  const float* cell_to_output_weights;
  int16_t* lstm_c2o_quant;

  const float* input_gate_bias;
  int32_t* lstm_igate_bias_quant;
  const float* forget_gate_bias;
  int32_t* lstm_fgate_bias_quant;
  const float* cell_gate_bias;
  int32_t* lstm_cgate_bias_quant;
  const float* output_gate_bias;
  int32_t* lstm_ogate_bias_quant;

  const float* projection_weights;
  int8_t* lstm_proj_w_quant;
  const float* projection_bias;
  int32_t* projection_bias_quant;

  int16_t* output_state;
  int16_t* cell_state;

  const float* input_layer_norm_coefficients;
  int16_t* lstm_input_layer_norm_coeff_quant;
  const float* forget_layer_norm_coefficients;
  int16_t* lstm_forget_layer_norm_coeff_quant;
  const float* cell_layer_norm_coefficients;
  int16_t* lstm_cell_layer_norm_coeff_quant;
  const float* output_layer_norm_coefficients;
  int16_t* lstm_output_layer_norm_coeff_quant;

  int8_t* output;
  const int8_t* expected_output;

  bool asymmetric_quantize_inputs;
  const float ranges[25][2];
};

struct LstmFloatTestConfig {
  const int n_batch;
  const int n_input;
  const int n_cell;
  const int n_output;
  const int sequence_length;
  const bool time_major;
  const bool use_cifg;
  const bool use_peephole;
  const bool use_projection_weights;
  const bool use_projection_bias;
  const bool use_layer_norm;
  const float cell_clip;
  const float proj_clip;

  const float* input_original;
  float* input;

  const float* input_to_input_weights;
  const float* input_to_forget_weights;
  const float* input_to_cell_weights;
  const float* input_to_output_weights;

  const float* recurrent_to_input_weights;
  const float* recurrent_to_forget_weights;
  const float* recurrent_to_cell_weights;
  const float* recurrent_to_output_weights;

  const float* cell_to_input_weights;
  const float* cell_to_forget_weights;
  const float* cell_to_output_weights;

  const float* input_gate_bias;
  const float* forget_gate_bias;
  const float* cell_gate_bias;
  const float* output_gate_bias;

  const float* projection_weights;
  const float* projection_bias;

  float* output_state;
  float* cell_state;

  const float* input_layer_norm_coefficients;
  const float* forget_layer_norm_coefficients;
  const float* cell_layer_norm_coefficients;
  const float* output_layer_norm_coefficients;

  float* output;
  const float* expected_output_original;
  float* expected_output;
};

struct LstmWeightQuantizationBuffers {
  int8_t* lstm_i2i_quant;
  float* lstm_i2i_scale;
  int* lstm_i2i_zp;
  TfLiteAffineQuantization* lstm_i2i_qparam;

  int8_t* lstm_i2f_quant;
  float* lstm_i2f_scale;
  int* lstm_i2f_zp;
  TfLiteAffineQuantization* lstm_i2f_qparam;

  int8_t* lstm_i2c_quant;
  float* lstm_i2c_scale;
  int* lstm_i2c_zp;
  TfLiteAffineQuantization* lstm_i2c_qparam;

  int8_t* lstm_i2o_quant;
  float* lstm_i2o_scale;
  int* lstm_i2o_zp;
  TfLiteAffineQuantization* lstm_i2o_qparam;

  int8_t* lstm_r2i_quant;
  float* lstm_r2i_scale;
  int* lstm_r2i_zp;
  TfLiteAffineQuantization* lstm_r2i_qparam;

  int8_t* lstm_r2f_quant;
  float* lstm_r2f_scale;
  int* lstm_r2f_zp;
  TfLiteAffineQuantization* lstm_r2f_qparam;

  int8_t* lstm_r2c_quant;
  float* lstm_r2c_scale;
  int* lstm_r2c_zp;
  TfLiteAffineQuantization* lstm_r2c_qparam;

  int8_t* lstm_r2o_quant;
  float* lstm_r2o_scale;
  int* lstm_r2o_zp;
  TfLiteAffineQuantization* lstm_r2o_qparam;

  int8_t* lstm_c2i_quant;
  float* lstm_c2i_scale;
  int* lstm_c2i_zp;
  TfLiteAffineQuantization* lstm_c2i_qparam;

  int8_t* lstm_c2f_quant;
  float* lstm_c2f_scale;
  int* lstm_c2f_zp;
  TfLiteAffineQuantization* lstm_c2f_qparam;

  int8_t* lstm_c2o_quant;
  float* lstm_c2o_scale;
  int* lstm_c2o_zp;
  TfLiteAffineQuantization* lstm_c2o_qparam;

  int8_t* lstm_proj_w_quant;
  float* lstm_proj_w_scale;
  int* lstm_proj_w_zp;
  TfLiteAffineQuantization* lstm_proj_w_qparam;
};

extern LstmIntegerTestConfig lstm_integer_no_peephole_config;

extern LstmIntegerTestConfig lstm_integer_peephole_config;

extern LstmFloatTestConfig lstm_no_cifg_no_peephole_no_proj_config;

extern LstmFloatTestConfig lstm_cifg_peephole_no_proj_config;

extern LstmFloatTestConfig lstm_no_cifg_peephole_proj_config;

extern LstmFloatTestConfig lstm_no_cifg_peephole_proj_bias_config;

extern LstmWeightQuantizationBuffers lstm_no_cifg_no_peephole_no_proj_buffers;

extern LstmWeightQuantizationBuffers lstm_cifg_peephole_no_proj_buffers;

extern LstmWeightQuantizationBuffers lstm_no_cifg_peephole_proj_buffers;

extern LstmFloatTestConfig cifg_peephole_no_proj_config_layer_norm;

#endif  // !defined(XTENSA)
}  // namespace testing
}  // namespace tflite

#endif  // TENSORFLOW_LITE_MICRO_KERNELS_UNIDIRECTIONAL_SEQUENCE_LSTM_TEST_CONFIG_H_
