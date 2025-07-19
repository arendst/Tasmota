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

#ifndef THIRD_PARTY_TFLITE_MICRO_TENSORFLOW_LITE_MICRO_FLATBUFFER_UTILS_H_
#define THIRD_PARTY_TFLITE_MICRO_TENSORFLOW_LITE_MICRO_FLATBUFFER_UTILS_H_

#include "third_party/flatbuffers/include/flatbuffers/flatbuffers.h"
#include "third_party/flatbuffers/include/flatbuffers/flexbuffers.h"
#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/schema/schema_generated.h"

namespace tflite {
// Kernels use flexbuffers::Map to pack their init parameters in a tflite file,
// with the parameter names as map keys and the parameter values as the
// corresponding map values.
// Accessing the map values using the flexbuffers:Map class is inline heavy,
// which can cause the code size to bloat beyond what's reasonable for a micro
// application. Use this class instead, when possible.
// FlexbufferWrapper takes advantage of the following properties of
// flexbuffers::Map:
// 1. It can be viewed as a flexbuffers::Vector of the values.
// 2. The values in the vector are ordered alphabetically by their keys.
// 3. All integer and Boolean values are stored as 64-bit numbers.
// 4. All floating point values are stored as double precision numbers.
// The properties are mentioned in the flexbuffers docs, but we rely on
// a unit test to catch design changes.
class FlexbufferWrapper : public flexbuffers::Vector {
 public:
  // Construct with a serialized flexbuffer 'buffer' of 'size' bytes
  explicit FlexbufferWrapper(const uint8_t* buffer, size_t size);
  int64_t ElementAsInt64(size_t i) const;
  uint64_t ElementAsUInt64(size_t i) const;
  int32_t ElementAsInt32(size_t i) const;
  bool ElementAsBool(size_t i) const;
  double ElementAsDouble(size_t i) const;
  float ElementAsFloat(size_t i) const;
};

// Return the number of operators in a subgraph tflite
uint32_t NumSubgraphOperators(const SubGraph* subgraph);
uint32_t NumSubgraphOperators(const Model* model, int subgraph_idx);

// Converts a flatbuffer array to a TfLiteArray.
// TODO(b/188459715): These function convert a const input to a non-const via a
// const_cast. It is unclear exactly why this is required.
TfLiteIntArray* FlatBufferVectorToTfLiteTypeArray(
    const flatbuffers::Vector<int32_t>* flatbuffer_array);
TfLiteFloatArray* FlatBufferVectorToTfLiteTypeArray(
    const flatbuffers::Vector<float>* flatbuffer_array);

}  // namespace tflite

#endif  // THIRD_PARTY_TFLITE_MICRO_TENSORFLOW_LITE_MICRO_FLATBUFFER_UTILS_H_
