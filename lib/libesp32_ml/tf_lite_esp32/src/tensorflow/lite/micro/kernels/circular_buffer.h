/* Copyright 2020 The TensorFlow Authors. All Rights Reserved.

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

#ifndef TENSORFLOW_LITE_MICRO_KERNELS_CIRCULAR_BUFFER_H_
#define TENSORFLOW_LITE_MICRO_KERNELS_CIRCULAR_BUFFER_H_

#include "tensorflow/lite/c/builtin_op_data.h"
#include "tensorflow/lite/c/common.h"

namespace tflite {

// The CircularBuffer op has one input and one output tensor.
extern const int kCircularBufferInputTensor;
extern const int kCircularBufferOutputTensor;

// Indices into the init flexbuffer's vector.
// The parameter's name is in the comment that follows.
// Elements in the vectors are ordered alphabetically by parameter name.
extern const int kCircularBufferCyclesMaxIndex;  // 'cycles_max'

// TODO(b/149795762): Add this to TfLiteStatus enum.
extern const TfLiteStatus kTfLiteAbort;

// These fields control the stride period of a strided streaming model. This op
// returns kTfLiteAbort until cycles_until_run-- is zero.  At this time,
// cycles_until_run is reset to cycles_max.
struct OpDataCircularBuffer {
  int cycles_until_run;
  int cycles_max;
};

TfLiteStatus CircularBufferPrepare(TfLiteContext* context, TfLiteNode* node);

}  // namespace tflite

#endif  // TENSORFLOW_LITE_MICRO_KERNELS_CIRCULAR_BUFFER_H_
