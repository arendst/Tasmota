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

#ifndef TENSORFLOW_LITE_MICRO_KERNELS_POOLING_H_
#define TENSORFLOW_LITE_MICRO_KERNELS_POOLING_H_

#include <cstdint>

#include "tensorflow/lite/c/builtin_op_data.h"
#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/micro/kernels/micro_ops.h"

namespace tflite {

extern const int kPoolingInputTensor;
extern const int kPoolingOutputTensor;

struct OpDataPooling {
  TfLitePaddingValues padding;
  int32_t activation_min;
  int32_t activation_max;
  float activation_min_f32;
  float activation_max_f32;
};

TfLiteStatus CalculateOpDataPooling(const TfLiteContext* context,
                                    const TfLitePoolParams* params,
                                    const TfLiteTensor* input,
                                    const TfLiteTensor* output,
                                    OpDataPooling* data);

TfLiteStatus PoolingPrepare(TfLiteContext* context, TfLiteNode* node);

void AveragePoolingEvalFloat(const TfLiteContext* context,
                             const TfLiteNode* node,
                             const TfLitePoolParams* params,
                             const OpDataPooling* data,
                             const TfLiteEvalTensor* input,
                             TfLiteEvalTensor* output);

void AveragePoolingEvalQuantized(TfLiteContext* context, const TfLiteNode* node,
                                 const TfLitePoolParams* params,
                                 const OpDataPooling* data,
                                 const TfLiteEvalTensor* input,
                                 TfLiteEvalTensor* output);

void MaxPoolingEvalFloat(TfLiteContext* context, TfLiteNode* node,
                         TfLitePoolParams* params, const OpDataPooling* data,
                         const TfLiteEvalTensor* input,
                         TfLiteEvalTensor* output);

void MaxPoolingEvalQuantized(TfLiteContext* context, TfLiteNode* node,
                             TfLitePoolParams* params,
                             const OpDataPooling* data,
                             const TfLiteEvalTensor* input,
                             TfLiteEvalTensor* output);

#if defined(CMSIS_NN)
TfLiteRegistration Register_AVERAGE_POOL_2D_INT8();

TfLiteRegistration Register_MAX_POOL_2D_INT8();
#else
inline TfLiteRegistration Register_AVERAGE_POOL_2D_INT8() {
  return tflite::Register_AVERAGE_POOL_2D();
}

inline TfLiteRegistration Register_MAX_POOL_2D_INT8() {
  return tflite::Register_MAX_POOL_2D();
}
#endif
}  // namespace tflite

#endif  // TENSORFLOW_LITE_MICRO_KERNELS_POOLING_H_
