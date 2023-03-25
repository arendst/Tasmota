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

#ifndef TENSORFLOW_LITE_MICRO_KERNELS_CONV_H_
#define TENSORFLOW_LITE_MICRO_KERNELS_CONV_H_

#include <cstdint>

#include "tensorflow/lite/c/builtin_op_data.h"
#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/kernels/internal/types.h"

namespace tflite {

struct OpDataConv {
  TfLitePaddingValues padding;

  // Cached tensor zero point values for quantized operations.
  int32_t input_zero_point;
  int32_t filter_zero_point;
  int32_t output_zero_point;

  // The scaling factor from input to output (aka the 'real multiplier') can
  // be represented as a fixed point multiplier plus a left shift.
  int32_t output_multiplier;
  int output_shift;

  // Per channel output multiplier and shift.
  int32_t* per_channel_output_multiplier;
  int32_t* per_channel_output_shift;

  // The range of the fused activation layer. For example for kNone and
  // uint8_t these would be 0 and 255.
  int32_t output_activation_min;
  int32_t output_activation_max;

  // A buffer used to store unpacked filter values. This is used if the source
  // tensor is of n-bit precision that cannot be easily processed by kernels.
  int filter_buffer_index;
};

extern const int kConvInputTensor;
extern const int kConvWeightsTensor;
extern const int kConvBiasTensor;
extern const int kConvOutputTensor;
extern const int kConvQuantizedDimension;

// Returns a ConvParams struct with all the parameters needed for a
// float computation.
ConvParams ConvParamsFloat(const TfLiteConvParams& params,
                           const OpDataConv& data);

// Returns a ConvParams struct with all the parameters needed for a
// quantized computation.
ConvParams ConvParamsQuantized(const TfLiteConvParams& params,
                               const OpDataConv& data);

TfLiteStatus CalculateOpDataConv(TfLiteContext* context, TfLiteNode* node,
                                 const TfLiteConvParams& params, int width,
                                 int height, int filter_width,
                                 int filter_height, int out_width,
                                 int out_height, const TfLiteType data_type,
                                 OpDataConv* data);

TfLiteStatus ConvPrepare(TfLiteContext* context, TfLiteNode* node);

// This is the most generic TfLiteRegistration. The actual supported types may
// still be target dependent. The only requirement is that every implementation
// (reference or optimized) must define this function.
TfLiteRegistration Register_CONV_2D();

#if defined(XTENSA)
// Returns a TfLiteRegistration struct for kernel variant that only supports
// int8 activations and int8 weights and always calls the reference
// implementation.
TfLiteRegistration Register_CONV_2D_INT8REF();
#else
inline TfLiteRegistration Register_CONV_2D_INT8REF() {
  return Register_CONV_2D();
}
#endif

#if defined(CMSIS_NN)
// Returns a TfLiteRegistration struct for kernel variant that only supports
// int8 activations and int8 weights and uses the latency optimized
// implementations.
TfLiteRegistration Register_CONV_2D_INT8();

// Returns a TfLiteRegistration struct for kernel variant that only supports
// int16 activations and int8 weights and uses the latency optimized
// implementations.
TfLiteRegistration Register_CONV_2D_INT16();

#else
inline TfLiteRegistration Register_CONV_2D_INT8() { return Register_CONV_2D(); }

inline TfLiteRegistration Register_CONV_2D_INT16() {
  return Register_CONV_2D();
}
#endif

}  // namespace tflite

#endif  // TENSORFLOW_LITE_MICRO_KERNELS_CONV_H_
