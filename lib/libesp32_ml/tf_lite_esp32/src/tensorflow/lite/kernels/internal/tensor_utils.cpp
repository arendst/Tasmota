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
==============================================================================
*/

// internal/reference/portable_tensor_utils.h has the implementation of the
// functions declared in internal/portable_tensor_utils.h. This somewhat
// confusing setup is derived from how the code is organized in TfLite where it
// is used to select between NEON, SSE and portable implementaitons. See
// https://github.com/tensorflow/tensorflow/blob/d76c23975c4a3a0d7987cfe3f45c76566df06180/tensorflow/lite/kernels/internal/tensor_utils.cc
// for how the code is written in TfLite.

#include "tensorflow/lite/kernels/internal/portable_tensor_utils.h"
#include "tensorflow/lite/kernels/internal/reference/portable_tensor_utils.h"
