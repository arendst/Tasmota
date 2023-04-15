/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

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
#ifndef TENSORFLOW_LITE_MICRO_KERNELS_LSTM_SHARED_H_
#define TENSORFLOW_LITE_MICRO_KERNELS_LSTM_SHARED_H_

namespace tflite {

// Input Tensors of size {n_batch, n_input}
constexpr int kLstmInputTensor = 0;

// Input weight tensors of size: {n_cell, n_input}
constexpr int kLstmInputToInputWeightsTensor = 1;  // Optional
constexpr int kLstmInputToForgetWeightsTensor = 2;
constexpr int kLstmInputToCellWeightsTensor = 3;
constexpr int kLstmInputToOutputWeightsTensor = 4;

// Recurrent weight tensors of size {n_cell, n_output}
constexpr int kLstmRecurrentToInputWeightsTensor = 5;  // Optional
constexpr int kLstmRecurrentToForgetWeightsTensor = 6;
constexpr int kLstmRecurrentToCellWeightsTensor = 7;
constexpr int kLstmRecurrentToOutputWeightsTensor = 8;

// Peephole weights tensors of size {n_cell}, representing a diagonal matrix.
constexpr int kLstmCellToInputWeightsTensor = 9;    // Optional
constexpr int kLstmCellToForgetWeightsTensor = 10;  // Optional
constexpr int kLstmCellToOutputWeightsTensor = 11;  // Optional

// Gates bias tensors of size {n_cell}
constexpr int kLstmInputGateBiasTensor = 12;  // Optional
constexpr int kLstmForgetGateBiasTensor = 13;
constexpr int kLstmCellGateBiasTensor = 14;
constexpr int kLstmOutputGateBiasTensor = 15;

// Projection weight tensor of size {n_output, n_cell}
constexpr int kLstmProjectionWeightsTensor = 16;  // Optional
// Projection bias tensor of size {n_output}
constexpr int kLstmProjectionBiasTensor = 17;  // Optional

// These state tensors are defined as variable tensors, and will be modified by
// this op.
constexpr int kLstmOutputStateTensor = 18;
constexpr int kLstmCellStateTensor = 19;

// Layer norm coefficient tensors of size {n_cell}, representing a diagonal
// matrix.
constexpr int kLstmInputLayerNormCoefficientsTensor = 20;   // Optional
constexpr int kLstmForgetLayerNormCoefficientsTensor = 21;  // Optional
constexpr int kLstmCellLayerNormCoefficientsTensor = 22;    // Optional
constexpr int kLstmOutputLayerNormCoefficientsTensor = 23;  // Optional

// Output tensors.
constexpr int kLstmOutputTensor = 0;

}  // namespace tflite
#endif  // TENSORFLOW_LITE_MICRO_KERNELS_LSTM_SHARED_H_
