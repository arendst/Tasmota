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

#ifndef TENSORFLOW_LITE_MICRO_KERNELS_ADD_H_
#define TENSORFLOW_LITE_MICRO_KERNELS_ADD_H_

#include <cstdint>

#include "tensorflow/lite/c/builtin_op_data.h"
#include "tensorflow/lite/c/common.h"

namespace tflite {

extern const int kAddInputTensor1;
extern const int kAddInputTensor2;
extern const int kAddOutputTensor;

struct OpDataAdd {
  bool requires_broadcast;

  // These fields are used in both the general 8-bit -> 8bit quantized path,
  // and the special 16-bit -> 16bit quantized path
  int input1_shift;
  int input2_shift;
  int32_t output_activation_min;
  int32_t output_activation_max;

  // These fields are used only in the general 8-bit -> 8bit quantized path
  int32_t input1_multiplier;
  int32_t input2_multiplier;
  int32_t output_multiplier;
  int output_shift;
  int left_shift;
  int32_t input1_offset;
  int32_t input2_offset;
  int32_t output_offset;

  // Used only for float evals:
  float output_activation_min_f32;
  float output_activation_max_f32;
};

TfLiteStatus CalculateOpDataAdd(TfLiteContext* context, TfLiteAddParams* params,
                                const TfLiteTensor* input1,
                                const TfLiteTensor* input2,
                                TfLiteTensor* output, OpDataAdd* data);

TfLiteStatus AddPrepare(TfLiteContext* context, TfLiteNode* node);

// Generic must define registration function.
TfLiteRegistration Register_ADD();

#if defined(CMSIS_NN)
TfLiteRegistration Register_ADD_INT8();

TfLiteRegistration Register_ADD_INT16();
#else
// Fallback registration
inline TfLiteRegistration Register_ADD_INT8() { return Register_ADD(); }

inline TfLiteRegistration Register_ADD_INT16() { return Register_ADD(); }
#endif
}  // namespace tflite

#endif  // TENSORFLOW_LITE_MICRO_KERNELS_ADD_H_
