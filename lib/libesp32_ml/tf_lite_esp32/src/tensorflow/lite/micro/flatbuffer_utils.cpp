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

#include "tensorflow/lite/micro/flatbuffer_utils.h"

namespace tflite {

FlexbufferWrapper::FlexbufferWrapper(const uint8_t* buffer, size_t size)
    : flexbuffers::Vector(flexbuffers::GetRoot(buffer, size).AsVector()) {}

int64_t FlexbufferWrapper::ElementAsInt64(size_t i) const {
  const uint8_t* elem = data_ + i * byte_width_;
  return ::flexbuffers::ReadInt64(elem, byte_width_);
}

uint64_t FlexbufferWrapper::ElementAsUInt64(size_t i) const {
  const uint8_t* elem = data_ + i * byte_width_;
  return ::flexbuffers::ReadUInt64(elem, byte_width_);
}

int32_t FlexbufferWrapper::ElementAsInt32(size_t i) const {
  return static_cast<int32_t>(ElementAsInt64(i));
}

bool FlexbufferWrapper::ElementAsBool(size_t i) const {
  return static_cast<bool>(ElementAsUInt64(i));
}

double FlexbufferWrapper::ElementAsDouble(size_t i) const {
  const uint8_t* elem = data_ + i * byte_width_;
  return ::flexbuffers::ReadDouble(elem, byte_width_);
}

float FlexbufferWrapper::ElementAsFloat(size_t i) const {
  return static_cast<float>(FlexbufferWrapper::ElementAsDouble(i));
}

// TODO(b/192589496): Ops must always be there. Remove this function when fixed
uint32_t NumSubgraphOperators(const SubGraph* subgraph) {
  if (subgraph->operators() != nullptr) {
    return subgraph->operators()->size();
  } else {
    return 0;
  }
}
// TODO(b/192589496): Ops must always be there. Remove this function when fixed
uint32_t NumSubgraphOperators(const Model* model, int subgraph_idx) {
  const SubGraph* subgraph = model->subgraphs()->Get(subgraph_idx);
  return NumSubgraphOperators(subgraph);
}

TfLiteIntArray* FlatBufferVectorToTfLiteTypeArray(
    const flatbuffers::Vector<int32_t>* flatbuffer_array) {
  // On little-endian machines, TfLiteIntArray happens to have the same memory
  // layout as flatbuffers:Vector<int32_t>, so we can reinterpret_cast the
  // flatbuffer vector and avoid a copy and malloc.
  // TODO(b/188459715): audit this usage of const_cast.
  return const_cast<TfLiteIntArray*>(
      reinterpret_cast<const TfLiteIntArray*>(flatbuffer_array));
}

TfLiteFloatArray* FlatBufferVectorToTfLiteTypeArray(
    const flatbuffers::Vector<float>* flatbuffer_array) {
  // On little-endian machines, TfLiteFloatArray happens to have the same memory
  // layout as flatbuffers:Vector<float>, so we can reinterpret_cast the
  // flatbuffer vector and avoid a copy and malloc.
  // TODO(b/188459715): audit this usage of const_cast.
  return const_cast<TfLiteFloatArray*>(
      reinterpret_cast<const TfLiteFloatArray*>(flatbuffer_array));
}

}  // namespace tflite
