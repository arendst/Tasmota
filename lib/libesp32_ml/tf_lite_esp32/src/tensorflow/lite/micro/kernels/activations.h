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

#ifndef TENSORFLOW_LITE_MICRO_KERNELS_ACTIVATIONS_H_
#define TENSORFLOW_LITE_MICRO_KERNELS_ACTIVATIONS_H_

#include <cstdint>

#include "tensorflow/lite/c/builtin_op_data.h"
#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/kernels/internal/types.h"

namespace tflite {

extern const int kActivationsInputTensor;
extern const int kActivationsOutputTensor;

struct ReluOpData {
  ReluParams params;
};

struct Relu6OpData {
  int8_t six_int8;
  int8_t zero_int8;
};

void ReluQuantized(const ReluOpData& data, const RuntimeShape& input_shape,
                   const RuntimeShape& output_shape, const int8_t* input_data,
                   int8_t* output_data);

template <typename T>
void CalculateReluOpData(const TfLiteTensor* input, TfLiteTensor* output,
                         ReluOpData* data);

void ReluFloat(const RuntimeShape& input_shape, const float* input_data,
               const RuntimeShape& output_shape, float* output_data);

void Relu6Float(const RuntimeShape& input_shape, const float* input_data,
                const RuntimeShape& output_shape, float* output_data);

void Relu6Quantized(int8_t lower, int8_t upper, const RuntimeShape& input_shape,
                    const int8_t* input_data, const RuntimeShape& output_shape,
                    int8_t* output_data);

TfLiteStatus ReluPrepare(TfLiteContext* context, TfLiteNode* node);

TfLiteStatus Relu6Prepare(TfLiteContext* context, TfLiteNode* node);

}  // namespace tflite

#endif  // TENSORFLOW_LITE_MICRO_KERNELS_ACTIVATIONS_H_
