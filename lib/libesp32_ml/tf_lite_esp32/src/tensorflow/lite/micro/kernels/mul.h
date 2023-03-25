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

#ifndef TENSORFLOW_LITE_MICRO_KERNELS_MUL_H_
#define TENSORFLOW_LITE_MICRO_KERNELS_MUL_H_

#include <cstdint>

#include "tensorflow/lite/c/builtin_op_data.h"
#include "tensorflow/lite/c/common.h"

namespace tflite {

extern const int kMulInput1Tensor;
extern const int kMulInput2Tensor;
extern const int kMulOutputTensor;

struct OpDataMul {
  int32_t input1_zero_point;
  int32_t input2_zero_point;

  int32_t output_activation_min;
  int32_t output_activation_max;
  int32_t output_zero_point;
  int32_t output_multiplier;
  int output_shift;

  float output_activation_min_f32;
  float output_activation_max_f32;
};

void* MulInit(TfLiteContext* context, const char* buffer, size_t length);

TfLiteStatus CalculateOpDataMul(TfLiteContext* context, TfLiteNode* node,
                                TfLiteMulParams* params, OpDataMul* data);

TfLiteStatus MulPrepare(TfLiteContext* context, TfLiteNode* node);

void EvalMulQuantizedReference(TfLiteContext* context, TfLiteNode* node,
                               const OpDataMul* data,
                               const TfLiteEvalTensor* input1,
                               const TfLiteEvalTensor* input2,
                               TfLiteEvalTensor* output);

void EvalMulFloatReference(TfLiteContext* context, TfLiteNode* node,
                           TfLiteMulParams* params, const OpDataMul* data,
                           const TfLiteEvalTensor* input1,
                           const TfLiteEvalTensor* input2,
                           TfLiteEvalTensor* output);

// Generic must define registration function.
TfLiteRegistration Register_MUL();

#if defined(CMSIS_NN)
TfLiteRegistration Register_MUL_INT8();
#else
// Fallback registration
inline TfLiteRegistration Register_MUL_INT8() { return Register_MUL(); }
#endif
}  // namespace tflite

#endif  // TENSORFLOW_LITE_MICRO_KERNELS_MUL_H_
