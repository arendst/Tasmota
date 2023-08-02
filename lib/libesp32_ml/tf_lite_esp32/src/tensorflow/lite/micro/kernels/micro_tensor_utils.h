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

// This file and the associated .cc file is branched from
// tensorflow/lite/kernels/internal/reference/portable_tensor_utils*
// TFLM needs to create its own because the original files are coupled with
// the tensor_utils module, which we cannot reuse due to its use of the
// Eigen library.

#ifndef TENSORFLOW_LITE_MICRO_KERNELS_MICRO_TENSOR_UTILS_H_
#define TENSORFLOW_LITE_MICRO_KERNELS_MICRO_TENSOR_UTILS_H_

#include <algorithm>
#include <cmath>
#include <cstdint>

#include "tensorflow/lite/c/builtin_op_data.h"
#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/kernels/internal/portable_tensor_utils.h"

#if defined(_MSC_VER)
#define __restrict__ __restrict
#endif

namespace tflite {

// Not all backends support CpuBackendContext usage, so forward declare to avoid
// pulling in its implementation.
// TODO(b/230666277): consider removing this since micro does not utilize it
class CpuBackendContext;

// Apply sigmoid to elements of a vector.
void PortableApplySigmoidToVector(const float* vector, int v_size,
                                  float* result);
// Apply tanh to elements of a vector
void PortableApplyTanhToVector(const float* vector, int v_size, float* result);
// Apply appropriate activation function to elements of a vector.
void PortableApplyActivationToVector(const float* vector, int v_size,
                                     TfLiteFusedActivation activation,
                                     float* result);

}  // namespace tflite

#endif  // TENSORFLOW_LITE_MICRO_KERNELS_MICRO_TENSOR_UTILS_H_