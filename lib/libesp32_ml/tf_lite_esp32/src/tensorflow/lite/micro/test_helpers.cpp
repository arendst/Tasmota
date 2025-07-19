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

#include "tensorflow/lite/micro/test_helpers.h"

#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <new>

#include "third_party/flatbuffers/include/flatbuffers/flatbuffers.h"
#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/kernels/internal/compatibility.h"
#include "tensorflow/lite/kernels/internal/tensor_ctypes.h"
#include "tensorflow/lite/kernels/kernel_util.h"
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/kernels/kernel_util.h"
#include "tensorflow/lite/micro/memory_helpers.h"
#include "tensorflow/lite/micro/micro_arena_constants.h"
#include "tensorflow/lite/micro/micro_utils.h"
#include "tensorflow/lite/micro/test_helper_custom_ops.h"
#include "tensorflow/lite/schema/schema_generated.h"

// TODO(b/170464050): Use TFLM test only version of schema_utils.

namespace tflite {
namespace testing {
namespace {

class StackAllocator : public flatbuffers::Allocator {
 public:
  StackAllocator(size_t alignment) : data_size_(0) {
    data_ = AlignPointerUp(data_backing_, alignment);
  }

  uint8_t* allocate(size_t size) override {
    TFLITE_DCHECK((data_size_ + size) <= kStackAllocatorSize);
    uint8_t* result = data_;
    data_ += size;
    data_size_ += size;
    return result;
  }

  void deallocate(uint8_t* p, size_t) override {}

  static StackAllocator& instance(size_t alignment = 1) {
    // Avoid using true dynamic memory allocation to be portable to bare metal.
    static char inst_memory[sizeof(StackAllocator)];
    static StackAllocator* inst = new (inst_memory) StackAllocator(alignment);
    return *inst;
  }

  static constexpr size_t kStackAllocatorSize = 8192;

 private:
  uint8_t data_backing_[kStackAllocatorSize];
  uint8_t* data_;
  int data_size_;

  TF_LITE_REMOVE_VIRTUAL_DELETE
};

flatbuffers::FlatBufferBuilder* BuilderInstance() {
  static char inst_memory[sizeof(flatbuffers::FlatBufferBuilder)];
  static flatbuffers::FlatBufferBuilder* inst =
      new (inst_memory) flatbuffers::FlatBufferBuilder(
          StackAllocator::kStackAllocatorSize,
          &StackAllocator::instance(MicroArenaBufferAlignment()));
  return inst;
}

// A wrapper around FlatBuffer API to help build model easily.
class ModelBuilder {
 public:
  typedef int32_t Tensor;
  typedef int Operator;
  typedef int Node;

  // `builder` needs to be available until BuildModel is called.
  explicit ModelBuilder(flatbuffers::FlatBufferBuilder* builder)
      : builder_(builder) {}

  // Registers an operator that will be used in the model.
  Operator RegisterOp(BuiltinOperator op, const char* custom_code);

  // Adds a tensor to the model.
  Tensor AddTensor(TensorType type, std::initializer_list<int32_t> shape) {
    return AddTensorImpl(type, /* is_variable */ false, shape);
  }

  // Adds a variable tensor to the model.
  Tensor AddVariableTensor(TensorType type,
                           std::initializer_list<int32_t> shape) {
    return AddTensorImpl(type, /* is_variable */ true, shape);
  }

  // Adds a node to the model with given input and output Tensors.
  Node AddNode(Operator op, std::initializer_list<Tensor> inputs,
               std::initializer_list<Tensor> outputs,
               std::initializer_list<Tensor> intermediates =
                   std::initializer_list<Tensor>{});

  void AddMetadata(const char* description_string,
                   const int32_t* metadata_buffer_data, size_t num_elements);

  // Constructs the flatbuffer model using `builder_` and return a pointer to
  // it. The returned model has the same lifetime as `builder_`.
  // Note the default value of 0 for num_subgraph_inputs means all tensor inputs
  // are in subgraph input list.
  const Model* BuildModel(std::initializer_list<Tensor> inputs,
                          std::initializer_list<Tensor> outputs,
                          size_t num_subgraph_inputs = 0);

 private:
  // Adds a tensor to the model.
  Tensor AddTensorImpl(TensorType type, bool is_variable,
                       std::initializer_list<int32_t> shape);

  flatbuffers::FlatBufferBuilder* builder_;

  static constexpr int kMaxOperatorCodes = 10;
  flatbuffers::Offset<tflite::OperatorCode> operator_codes_[kMaxOperatorCodes];
  int next_operator_code_id_ = 0;

  static constexpr int kMaxOperators = 50;
  flatbuffers::Offset<tflite::Operator> operators_[kMaxOperators];
  int next_operator_id_ = 0;

  static constexpr int kMaxTensors = 50;
  flatbuffers::Offset<tflite::Tensor> tensors_[kMaxTensors];

  static constexpr int kMaxMetadataBuffers = 10;

  static constexpr int kMaxMetadatas = 10;
  flatbuffers::Offset<Metadata> metadata_[kMaxMetadatas];

  flatbuffers::Offset<Buffer> metadata_buffers_[kMaxMetadataBuffers];

  int nbr_of_metadata_buffers_ = 0;

  int next_tensor_id_ = 0;
};

ModelBuilder::Operator ModelBuilder::RegisterOp(BuiltinOperator op,
                                                const char* custom_code) {
  TFLITE_DCHECK(next_operator_code_id_ <= kMaxOperatorCodes);
  operator_codes_[next_operator_code_id_] = tflite::CreateOperatorCodeDirect(
      *builder_, /*deprecated_builtin_code=*/0, custom_code, /*version=*/0, op);
  next_operator_code_id_++;
  return next_operator_code_id_ - 1;
}

ModelBuilder::Node ModelBuilder::AddNode(
    ModelBuilder::Operator op,
    std::initializer_list<ModelBuilder::Tensor> inputs,
    std::initializer_list<ModelBuilder::Tensor> outputs,
    std::initializer_list<ModelBuilder::Tensor> intermediates) {
  TFLITE_DCHECK(next_operator_id_ <= kMaxOperators);
  operators_[next_operator_id_] = tflite::CreateOperator(
      *builder_, op, builder_->CreateVector(inputs.begin(), inputs.size()),
      builder_->CreateVector(outputs.begin(), outputs.size()),
      BuiltinOptions_NONE,
      /*builtin_options=*/0,
      /*custom_options=*/0, tflite::CustomOptionsFormat_FLEXBUFFERS,
      /*mutating_variable_inputs =*/0,
      builder_->CreateVector(intermediates.begin(), intermediates.size()));
  next_operator_id_++;
  return next_operator_id_ - 1;
}

void ModelBuilder::AddMetadata(const char* description_string,
                               const int32_t* metadata_buffer_data,
                               size_t num_elements) {
  metadata_[ModelBuilder::nbr_of_metadata_buffers_] =
      CreateMetadata(*builder_, builder_->CreateString(description_string),
                     1 + ModelBuilder::nbr_of_metadata_buffers_);

  metadata_buffers_[nbr_of_metadata_buffers_] = tflite::CreateBuffer(
      *builder_, builder_->CreateVector((uint8_t*)metadata_buffer_data,
                                        sizeof(uint32_t) * num_elements));

  ModelBuilder::nbr_of_metadata_buffers_++;
}

const Model* ModelBuilder::BuildModel(
    std::initializer_list<ModelBuilder::Tensor> inputs,
    std::initializer_list<ModelBuilder::Tensor> outputs,
    size_t num_subgraph_inputs) {
  // Model schema requires an empty buffer at idx 0.
  size_t buffer_size = 1 + ModelBuilder::nbr_of_metadata_buffers_;
  flatbuffers::Offset<Buffer> buffers[kMaxMetadataBuffers];
  buffers[0] = tflite::CreateBuffer(*builder_);

  // Place the metadata buffers first in the buffer since the indices for them
  // have already been set in AddMetadata()
  for (int i = 1; i < ModelBuilder::nbr_of_metadata_buffers_ + 1; ++i) {
    buffers[i] = metadata_buffers_[i - 1];
  }

  // Default to single subgraph model.
  constexpr size_t subgraphs_size = 1;

  // Find out number of subgraph inputs.
  if (num_subgraph_inputs == 0) {
    // This is the default case.
    num_subgraph_inputs = inputs.size();
  } else {
    // A non-zero value of num_subgraph_inputs means that some of
    // the operator input tensors are not subgraph inputs.
    TFLITE_DCHECK(num_subgraph_inputs <= inputs.size());
  }

  const flatbuffers::Offset<SubGraph> subgraphs[subgraphs_size] = {
      tflite::CreateSubGraph(
          *builder_, builder_->CreateVector(tensors_, next_tensor_id_),
          builder_->CreateVector(inputs.begin(), num_subgraph_inputs),
          builder_->CreateVector(outputs.begin(), outputs.size()),
          builder_->CreateVector(operators_, next_operator_id_),
          builder_->CreateString("test_subgraph"))};

  flatbuffers::Offset<Model> model_offset;
  if (ModelBuilder::nbr_of_metadata_buffers_ > 0) {
    model_offset = tflite::CreateModel(
        *builder_, 0,
        builder_->CreateVector(operator_codes_, next_operator_code_id_),
        builder_->CreateVector(subgraphs, subgraphs_size),
        builder_->CreateString("teset_model"),
        builder_->CreateVector(buffers, buffer_size), 0,
        builder_->CreateVector(metadata_,
                               ModelBuilder::nbr_of_metadata_buffers_));
  } else {
    model_offset = tflite::CreateModel(
        *builder_, 0,
        builder_->CreateVector(operator_codes_, next_operator_code_id_),
        builder_->CreateVector(subgraphs, subgraphs_size),
        builder_->CreateString("teset_model"),
        builder_->CreateVector(buffers, buffer_size));
  }

  tflite::FinishModelBuffer(*builder_, model_offset);
  void* model_pointer = builder_->GetBufferPointer();
  const Model* model = flatbuffers::GetRoot<Model>(model_pointer);
  return model;
}

ModelBuilder::Tensor ModelBuilder::AddTensorImpl(
    TensorType type, bool is_variable, std::initializer_list<int32_t> shape) {
  TFLITE_DCHECK(next_tensor_id_ <= kMaxTensors);
  tensors_[next_tensor_id_] = tflite::CreateTensor(
      *builder_, builder_->CreateVector(shape.begin(), shape.size()), type,
      /* buffer */ 0, /* name */ 0, /* quantization */ 0,
      /* is_variable */ is_variable,
      /* sparsity */ 0);
  next_tensor_id_++;
  return next_tensor_id_ - 1;
}

const Model* BuildSimpleStatefulModel() {
  using flatbuffers::Offset;
  flatbuffers::FlatBufferBuilder* fb_builder = BuilderInstance();

  ModelBuilder model_builder(fb_builder);

  const int op_id =
      model_builder.RegisterOp(BuiltinOperator_CUSTOM, "simple_stateful_op");
  const int input_tensor = model_builder.AddTensor(TensorType_INT8, {3});
  const int median_tensor = model_builder.AddTensor(TensorType_INT8, {3});
  const int invoke_count_tensor =
      model_builder.AddTensor(TensorType_INT32, {1});
  const int intermediate_tensor =
      model_builder.AddTensor(TensorType_FLOAT32, {0});

  model_builder.AddNode(op_id, {input_tensor},
                        {median_tensor, invoke_count_tensor},
                        {intermediate_tensor});
  return model_builder.BuildModel({input_tensor},
                                  {median_tensor, invoke_count_tensor});
}

const Model* BuildSimpleModelWithBranch() {
  using flatbuffers::Offset;
  flatbuffers::FlatBufferBuilder* fb_builder = BuilderInstance();

  ModelBuilder model_builder(fb_builder);
  /* Model structure
           | t0
    +------|
    |      v
    |   +---------+
    |   |   n0    |
    |   |         |
    |   +---------+
    v           +
                |
  +---------+   | t1
  |   n1    |   |
  |         |   |
  +---------+   |
     |          |
 t2  |          v
     |   +---------+
     +-->|    n2   |
         |         |
         +-------|-+
                 |t3
                 v
  */
  const int op_id =
      model_builder.RegisterOp(BuiltinOperator_CUSTOM, "mock_custom");
  const int t0 = model_builder.AddTensor(TensorType_FLOAT32, {2, 2, 3});
  const int t1 = model_builder.AddTensor(TensorType_FLOAT32, {2, 2, 3});
  const int t2 = model_builder.AddTensor(TensorType_FLOAT32, {2, 2, 3});
  const int t3 = model_builder.AddTensor(TensorType_FLOAT32, {2, 2, 3});
  model_builder.AddNode(op_id, {t0}, {t1});      // n0
  model_builder.AddNode(op_id, {t0}, {t2});      // n1
  model_builder.AddNode(op_id, {t1, t2}, {t3});  // n2
  return model_builder.BuildModel({t0}, {t3});
}

const Model* BuildModelWithOfflinePlanning(int number_of_tensors,
                                           const int32_t* metadata_buffer,
                                           NodeConnection* node_conn,
                                           int num_conns,
                                           int num_subgraph_inputs) {
  using flatbuffers::Offset;
  flatbuffers::FlatBufferBuilder* fb_builder = BuilderInstance();

  ModelBuilder model_builder(fb_builder);

  const int op_id =
      model_builder.RegisterOp(BuiltinOperator_CUSTOM, "mock_custom");

  for (int i = 0; i < number_of_tensors; ++i) {
    model_builder.AddTensor(TensorType_FLOAT32, {2, 2, 3});
  }

  for (int i = 0; i < num_conns; ++i) {
    model_builder.AddNode(op_id, node_conn[i].input, node_conn[i].output);
  }

  model_builder.AddMetadata(
      "OfflineMemoryAllocation", metadata_buffer,
      number_of_tensors + tflite::testing::kOfflinePlannerHeaderSize);

  return model_builder.BuildModel(
      node_conn[0].input, node_conn[num_conns - 1].output, num_subgraph_inputs);
}

const Model* BuildModelWithUnusedInputs() {
  using flatbuffers::Offset;
  flatbuffers::FlatBufferBuilder* builder = BuilderInstance();

  constexpr size_t buffers_size = 1;
  const Offset<Buffer> buffers[buffers_size] = {CreateBuffer(*builder)};
  constexpr size_t tensor_shape_size = 2;
  const int32_t tensor_shape[tensor_shape_size] = {1, 64};
  constexpr size_t tensors_size = 4;
  const Offset<Tensor> tensors[tensors_size] = {
      CreateTensor(*builder,
                   builder->CreateVector(tensor_shape, tensor_shape_size),
                   TensorType_INT8, 0,
                   builder->CreateString("test_input_tensor"), 0, false),
      CreateTensor(*builder,
                   builder->CreateVector(tensor_shape, tensor_shape_size),
                   TensorType_INT8, 0,
                   builder->CreateString("test_unused_input_tensor"), 0, false),
      CreateTensor(*builder,
                   builder->CreateVector(tensor_shape, tensor_shape_size),
                   TensorType_INT8, 0,
                   builder->CreateString("test_output_tensor"), 0, false),
      CreateTensor(*builder,
                   builder->CreateVector(tensor_shape, tensor_shape_size),
                   TensorType_INT8, 0,
                   builder->CreateString("test_unused_tensor"), 0, false),
  };
  constexpr size_t inputs_size = 2;
  const int32_t inputs[inputs_size] = {0, 1};
  constexpr size_t outputs_size = 1;
  const int32_t outputs[outputs_size] = {2};
  constexpr size_t operator_inputs_size = 1;
  const int32_t operator_inputs[operator_inputs_size] = {0};
  constexpr size_t operator_outputs_size = 1;
  const int32_t operator_outputs[operator_outputs_size] = {2};
  constexpr size_t operators_size = 1;
  const Offset<Operator> operators[operators_size] = {
      CreateOperator(
          *builder, 0,
          builder->CreateVector(operator_inputs, operator_inputs_size),
          builder->CreateVector(operator_outputs, operator_outputs_size),
          BuiltinOptions_NONE),
  };
  constexpr size_t subgraphs_size = 1;
  const Offset<SubGraph> subgraphs[subgraphs_size] = {
      CreateSubGraph(*builder, builder->CreateVector(tensors, tensors_size),
                     builder->CreateVector(inputs, inputs_size),
                     builder->CreateVector(outputs, outputs_size),
                     builder->CreateVector(operators, operators_size),
                     builder->CreateString("test_subgraph"))};
  constexpr size_t operator_codes_size = 1;
  const Offset<OperatorCode> operator_codes[operator_codes_size] = {
      CreateOperatorCodeDirect(*builder, /*deprecated_builtin_code=*/0,
                               "mock_custom",
                               /*version=*/0, BuiltinOperator_CUSTOM)};
  const Offset<Model> model_offset = CreateModel(
      *builder, 0, builder->CreateVector(operator_codes, operator_codes_size),
      builder->CreateVector(subgraphs, subgraphs_size),
      builder->CreateString("test_model"),
      builder->CreateVector(buffers, buffers_size));
  FinishModelBuffer(*builder, model_offset);
  void* model_pointer = builder->GetBufferPointer();
  const Model* model = flatbuffers::GetRoot<Model>(model_pointer);
  return model;
}

const Model* BuildModelWithUnusedOperatorOutputs() {
  using flatbuffers::Offset;
  flatbuffers::FlatBufferBuilder* builder = BuilderInstance();

  constexpr size_t buffers_size = 1;
  const Offset<Buffer> buffers[buffers_size] = {CreateBuffer(*builder)};
  constexpr size_t tensor_shape_size = 2;
  const int32_t tensor_shape[tensor_shape_size] = {1, 64};
  constexpr size_t tensors_size = 2;
  const Offset<Tensor> tensors[tensors_size] = {
      CreateTensor(*builder,
                   builder->CreateVector(tensor_shape, tensor_shape_size),
                   TensorType_INT8, 0,
                   builder->CreateString("test_input_tensor"), 0, false),
      CreateTensor(
          *builder, builder->CreateVector(tensor_shape, tensor_shape_size),
          TensorType_INT8, 0,
          builder->CreateString("test_unused_output_tensor"), 0, false)};
  constexpr size_t inputs_size = 0;
  const int32_t inputs[inputs_size] = {};
  constexpr size_t outputs_size = 1;
  const int32_t outputs[outputs_size] = {0};
  constexpr size_t operator_inputs_size = 0;
  const int32_t operator_inputs[operator_inputs_size] = {};
  constexpr size_t operator_outputs_size = 2;
  const int32_t operator_outputs[operator_outputs_size] = {0, 1};
  constexpr size_t operators_size = 1;
  const Offset<Operator> operators[operators_size] = {
      CreateOperator(
          *builder, 0,
          builder->CreateVector(operator_inputs, operator_inputs_size),
          builder->CreateVector(operator_outputs, operator_outputs_size),
          BuiltinOptions_NONE),
  };
  constexpr size_t subgraphs_size = 1;
  const Offset<SubGraph> subgraphs[subgraphs_size] = {
      CreateSubGraph(*builder, builder->CreateVector(tensors, tensors_size),
                     builder->CreateVector(inputs, inputs_size),
                     builder->CreateVector(outputs, outputs_size),
                     builder->CreateVector(operators, operators_size),
                     builder->CreateString("test_subgraph"))};
  constexpr size_t operator_codes_size = 1;
  const Offset<OperatorCode> operator_codes[operator_codes_size] = {
      CreateOperatorCodeDirect(*builder, /*deprecated_builtin_code=*/0,
                               "mock_custom",
                               /*version=*/0, BuiltinOperator_CUSTOM)};
  const Offset<Model> model_offset = CreateModel(
      *builder, 0, builder->CreateVector(operator_codes, operator_codes_size),
      builder->CreateVector(subgraphs, subgraphs_size),
      builder->CreateString("test_model"),
      builder->CreateVector(buffers, buffers_size));
  FinishModelBuffer(*builder, model_offset);
  void* model_pointer = builder->GetBufferPointer();
  const Model* model = flatbuffers::GetRoot<Model>(model_pointer);
  return model;
}

const Model* BuildModelWith256x256Tensor() {
  using flatbuffers::Offset;
  flatbuffers::FlatBufferBuilder* fb_builder = BuilderInstance();

  ModelBuilder model_builder(fb_builder);

  const int op_id =
      model_builder.RegisterOp(BuiltinOperator_CUSTOM, "mock_custom");
  const int input1_tensor =
      model_builder.AddTensor(TensorType_INT8, {256, 256});
  const int input2_tensor =
      model_builder.AddTensor(TensorType_INT8, {256, 256});
  const int output_tensor =
      model_builder.AddTensor(TensorType_INT8, {256, 256});

  model_builder.AddNode(op_id, {input1_tensor, input2_tensor}, {output_tensor});
  return model_builder.BuildModel({input1_tensor, input2_tensor},
                                  {output_tensor});
}

const Model* BuildSimpleMockModel() {
  using flatbuffers::Offset;
  flatbuffers::FlatBufferBuilder* builder = BuilderInstance();

  constexpr size_t buffer_data_size = 1;
  const uint8_t buffer_data[buffer_data_size] = {21};
  constexpr size_t buffers_size = 2;
  const Offset<Buffer> buffers[buffers_size] = {
      CreateBuffer(*builder),
      CreateBuffer(*builder,
                   builder->CreateVector(buffer_data, buffer_data_size))};
  constexpr size_t tensor_shape_size = 1;
  const int32_t tensor_shape[tensor_shape_size] = {1};
  constexpr size_t tensors_size = 4;
  const Offset<Tensor> tensors[tensors_size] = {
      CreateTensor(*builder,
                   builder->CreateVector(tensor_shape, tensor_shape_size),
                   TensorType_INT32, 0,
                   builder->CreateString("test_input_tensor"), 0, false),
      CreateTensor(*builder,
                   builder->CreateVector(tensor_shape, tensor_shape_size),
                   TensorType_INT8, 1,
                   builder->CreateString("test_weight_tensor"), 0, false),
      CreateTensor(*builder,
                   builder->CreateVector(tensor_shape, tensor_shape_size),
                   TensorType_INT32, 0,
                   builder->CreateString("test_output_tensor"), 0, false),
      CreateTensor(*builder,
                   builder->CreateVector(tensor_shape, tensor_shape_size),
                   TensorType_INT32, 0,
                   builder->CreateString("test_output2_tensor"), 0, false),
  };
  constexpr size_t inputs_size = 1;
  const int32_t inputs[inputs_size] = {0};
  constexpr size_t outputs_size = 2;
  const int32_t outputs[outputs_size] = {2, 3};
  constexpr size_t operator_inputs_size = 2;
  const int32_t operator_inputs[operator_inputs_size] = {0, 1};
  constexpr size_t operator_outputs_size = 1;
  const int32_t operator_outputs[operator_outputs_size] = {2};
  const int32_t operator2_outputs[operator_outputs_size] = {3};
  constexpr size_t operators_size = 2;
  const Offset<Operator> operators[operators_size] = {
      CreateOperator(
          *builder, 0,
          builder->CreateVector(operator_inputs, operator_inputs_size),
          builder->CreateVector(operator_outputs, operator_outputs_size),
          BuiltinOptions_NONE),
      CreateOperator(
          *builder, 0,
          builder->CreateVector(operator_inputs, operator_inputs_size),
          builder->CreateVector(operator2_outputs, operator_outputs_size),
          BuiltinOptions_NONE),
  };
  constexpr size_t subgraphs_size = 1;
  const Offset<SubGraph> subgraphs[subgraphs_size] = {
      CreateSubGraph(*builder, builder->CreateVector(tensors, tensors_size),
                     builder->CreateVector(inputs, inputs_size),
                     builder->CreateVector(outputs, outputs_size),
                     builder->CreateVector(operators, operators_size),
                     builder->CreateString("test_subgraph"))};
  constexpr size_t operator_codes_size = 1;
  const Offset<OperatorCode> operator_codes[operator_codes_size] = {
      CreateOperatorCodeDirect(*builder, /*deprecated_builtin_code=*/0,
                               "mock_custom",
                               /*version=*/0, BuiltinOperator_CUSTOM)};
  const Offset<Model> model_offset = CreateModel(
      *builder, 0, builder->CreateVector(operator_codes, operator_codes_size),
      builder->CreateVector(subgraphs, subgraphs_size),
      builder->CreateString("test_model"),
      builder->CreateVector(buffers, buffers_size));
  FinishModelBuffer(*builder, model_offset);
  void* model_pointer = builder->GetBufferPointer();
  const Model* model = flatbuffers::GetRoot<Model>(model_pointer);
  return model;
}

const Model* BuildComplexMockModel() {
  using flatbuffers::Offset;
  flatbuffers::FlatBufferBuilder* builder = BuilderInstance();

  constexpr size_t buffer_data_size = 1;
  const uint8_t buffer_data_1[buffer_data_size] = {21};
  const uint8_t buffer_data_2[buffer_data_size] = {21};
  const uint8_t buffer_data_3[buffer_data_size] = {21};
  constexpr size_t buffers_size = 7;
  const Offset<Buffer> buffers[buffers_size] = {
      // Op 1 buffers:
      CreateBuffer(*builder),
      CreateBuffer(*builder),
      CreateBuffer(*builder,
                   builder->CreateVector(buffer_data_1, buffer_data_size)),
      // Op 2 buffers:
      CreateBuffer(*builder),
      CreateBuffer(*builder,
                   builder->CreateVector(buffer_data_2, buffer_data_size)),
      // Op 3 buffers:
      CreateBuffer(*builder),
      CreateBuffer(*builder,
                   builder->CreateVector(buffer_data_3, buffer_data_size)),
  };
  constexpr size_t tensor_shape_size = 1;
  const int32_t tensor_shape[tensor_shape_size] = {1};

  constexpr size_t tensors_size = 10;
  const Offset<Tensor> tensors[tensors_size] = {
      // Op 1 inputs:
      CreateTensor(
          *builder, builder->CreateVector(tensor_shape, tensor_shape_size),
          TensorType_INT32, 0, builder->CreateString("test_input_tensor_1"), 0,
          false /* is_variable */),
      CreateTensor(
          *builder, builder->CreateVector(tensor_shape, tensor_shape_size),
          TensorType_INT32, 1, builder->CreateString("test_variable_tensor_1"),
          0, true /* is_variable */),
      CreateTensor(
          *builder, builder->CreateVector(tensor_shape, tensor_shape_size),
          TensorType_INT8, 2, builder->CreateString("test_weight_tensor_1"), 0,
          false /* is_variable */),
      // Op 1 output / Op 2 input:
      CreateTensor(
          *builder, builder->CreateVector(tensor_shape, tensor_shape_size),
          TensorType_INT32, 0, builder->CreateString("test_output_tensor_1"), 0,
          false /* is_variable */),
      // Op 2 inputs:
      CreateTensor(
          *builder, builder->CreateVector(tensor_shape, tensor_shape_size),
          TensorType_INT32, 1, builder->CreateString("test_variable_tensor_2"),
          0, true /* is_variable */),
      CreateTensor(
          *builder, builder->CreateVector(tensor_shape, tensor_shape_size),
          TensorType_INT8, 2, builder->CreateString("test_weight_tensor_2"), 0,
          false /* is_variable */),
      // Op 2 output / Op 3 input:
      CreateTensor(
          *builder, builder->CreateVector(tensor_shape, tensor_shape_size),
          TensorType_INT32, 0, builder->CreateString("test_output_tensor_2"), 0,
          false /* is_variable */),
      // Op 3 inputs:
      CreateTensor(
          *builder, builder->CreateVector(tensor_shape, tensor_shape_size),
          TensorType_INT32, 1, builder->CreateString("test_variable_tensor_3"),
          0, true /* is_variable */),
      CreateTensor(
          *builder, builder->CreateVector(tensor_shape, tensor_shape_size),
          TensorType_INT8, 2, builder->CreateString("test_weight_tensor_3"), 0,
          false /* is_variable */),
      // Op 3 output:
      CreateTensor(
          *builder, builder->CreateVector(tensor_shape, tensor_shape_size),
          TensorType_INT32, 0, builder->CreateString("test_output_tensor_3"), 0,
          false /* is_variable */),
  };

  constexpr size_t operators_size = 3;
  Offset<Operator> operators[operators_size];
  {
    // Set Op 1 attributes:
    constexpr size_t operator_inputs_size = 3;
    const int32_t operator_inputs[operator_inputs_size] = {0, 1, 2};
    constexpr size_t operator_outputs_size = 1;
    const int32_t operator_outputs[operator_outputs_size] = {3};

    operators[0] = {CreateOperator(
        *builder, 0,
        builder->CreateVector(operator_inputs, operator_inputs_size),
        builder->CreateVector(operator_outputs, operator_outputs_size),
        BuiltinOptions_NONE)};
  }

  {
    // Set Op 2 attributes
    constexpr size_t operator_inputs_size = 3;
    const int32_t operator_inputs[operator_inputs_size] = {3, 4, 5};
    constexpr size_t operator_outputs_size = 1;
    const int32_t operator_outputs[operator_outputs_size] = {6};

    operators[1] = {CreateOperator(
        *builder, 0,
        builder->CreateVector(operator_inputs, operator_inputs_size),
        builder->CreateVector(operator_outputs, operator_outputs_size),
        BuiltinOptions_NONE)};
  }

  {
    // Set Op 3 attributes
    constexpr size_t operator_inputs_size = 3;
    const int32_t operator_inputs[operator_inputs_size] = {6, 7, 8};
    constexpr size_t operator_outputs_size = 1;
    const int32_t operator_outputs[operator_outputs_size] = {9};

    operators[2] = {CreateOperator(
        *builder, 0,
        builder->CreateVector(operator_inputs, operator_inputs_size),
        builder->CreateVector(operator_outputs, operator_outputs_size),
        BuiltinOptions_NONE)};
  }

  constexpr size_t inputs_size = 1;
  const int32_t inputs[inputs_size] = {0};
  constexpr size_t outputs_size = 1;
  const int32_t outputs[outputs_size] = {9};

  constexpr size_t subgraphs_size = 1;
  const Offset<SubGraph> subgraphs[subgraphs_size] = {
      CreateSubGraph(*builder, builder->CreateVector(tensors, tensors_size),
                     builder->CreateVector(inputs, inputs_size),
                     builder->CreateVector(outputs, outputs_size),
                     builder->CreateVector(operators, operators_size),
                     builder->CreateString("test_subgraph"))};

  constexpr size_t operator_codes_size = 1;
  const Offset<OperatorCode> operator_codes[operator_codes_size] = {
      CreateOperatorCodeDirect(*builder, /*deprecated_builtin_code=*/0,
                               "mock_custom",
                               /*version=*/0, BuiltinOperator_CUSTOM)};

  const Offset<Model> model_offset = CreateModel(
      *builder, 0, builder->CreateVector(operator_codes, operator_codes_size),
      builder->CreateVector(subgraphs, subgraphs_size),
      builder->CreateString("test_model"),
      builder->CreateVector(buffers, buffers_size));

  FinishModelBuffer(*builder, model_offset);
  void* model_pointer = builder->GetBufferPointer();
  const Model* model = flatbuffers::GetRoot<Model>(model_pointer);
  return model;
}

const Model* BuildSimpleMultipleInputsModel() {
  using flatbuffers::Offset;
  flatbuffers::FlatBufferBuilder* builder = BuilderInstance();

  constexpr size_t buffers_size = 1;
  const Offset<Buffer> buffers[buffers_size] = {
      CreateBuffer(*builder),
  };
  constexpr size_t tensor_shape_size = 1;
  const int32_t tensor_shape[tensor_shape_size] = {1};
  constexpr size_t tensors_size = 4;
  const Offset<Tensor> tensors[tensors_size] = {
      CreateTensor(*builder,
                   builder->CreateVector(tensor_shape, tensor_shape_size),
                   TensorType_INT32, 0,
                   builder->CreateString("test_input_tensor1"), 0, false),
      CreateTensor(*builder,
                   builder->CreateVector(tensor_shape, tensor_shape_size),
                   TensorType_INT8, 0,
                   builder->CreateString("test_input_tensor2"), 0, false),
      CreateTensor(*builder,
                   builder->CreateVector(tensor_shape, tensor_shape_size),
                   TensorType_INT32, 0,
                   builder->CreateString("test_input_tensor3"), 0, false),
      CreateTensor(*builder,
                   builder->CreateVector(tensor_shape, tensor_shape_size),
                   TensorType_INT32, 0,
                   builder->CreateString("test_output_tensor"), 0, false),
  };
  constexpr size_t inputs_size = 3;
  const int32_t inputs[inputs_size] = {0, 1, 2};
  constexpr size_t outputs_size = 1;
  const int32_t outputs[outputs_size] = {3};
  constexpr size_t operator_inputs_size = 3;
  const int32_t operator_inputs[operator_inputs_size] = {0, 1, 2};
  constexpr size_t operator_outputs_size = 1;
  const int32_t operator_outputs[operator_outputs_size] = {3};
  constexpr size_t operators_size = 1;
  const Offset<Operator> operators[operators_size] = {
      CreateOperator(
          *builder, 0,
          builder->CreateVector(operator_inputs, operator_inputs_size),
          builder->CreateVector(operator_outputs, operator_outputs_size),
          BuiltinOptions_NONE),
  };
  constexpr size_t subgraphs_size = 1;
  const Offset<SubGraph> subgraphs[subgraphs_size] = {
      CreateSubGraph(*builder, builder->CreateVector(tensors, tensors_size),
                     builder->CreateVector(inputs, inputs_size),
                     builder->CreateVector(outputs, outputs_size),
                     builder->CreateVector(operators, operators_size),
                     builder->CreateString("test_subgraph"))};
  constexpr size_t operator_codes_size = 1;
  const Offset<OperatorCode> operator_codes[operator_codes_size] = {
      CreateOperatorCodeDirect(*builder, /*deprecated_builtin_code=*/0,
                               "multiple_inputs_op",
                               /*version=*/0, BuiltinOperator_CUSTOM)};
  const Offset<Model> model_offset = CreateModel(
      *builder, 0, builder->CreateVector(operator_codes, operator_codes_size),
      builder->CreateVector(subgraphs, subgraphs_size),
      builder->CreateString("test_model"),
      builder->CreateVector(buffers, buffers_size));
  FinishModelBuffer(*builder, model_offset);
  void* model_pointer = builder->GetBufferPointer();
  const Model* model = flatbuffers::GetRoot<Model>(model_pointer);
  return model;
}

const Model* BuildSimpleModelWithSubgraphsAndIf() {
  using flatbuffers::Offset;
  flatbuffers::FlatBufferBuilder* builder = BuilderInstance();

  constexpr size_t buffers_size = 1;
  const Offset<Buffer> buffers[buffers_size] = {
      CreateBuffer(*builder),
  };
  const int32_t condition_tensor_shape[] = {1};
  const int32_t data_tensor_shape[] = {1, 2};
  constexpr size_t tensors_size = 4;
  const Offset<Tensor> subgraph1_tensors[tensors_size] = {
      CreateTensor(*builder, builder->CreateVector(condition_tensor_shape, 1),
                   TensorType_BOOL, 0,
                   builder->CreateString("condition tensor"), 0, false),
      CreateTensor(*builder, builder->CreateVector(data_tensor_shape, 2),
                   TensorType_FLOAT32, 0,
                   builder->CreateString("input_tensor1"), 0, false),
      CreateTensor(*builder, builder->CreateVector(data_tensor_shape, 2),
                   TensorType_FLOAT32, 0,
                   builder->CreateString("input_tensor2"), 0, false),
      CreateTensor(*builder, builder->CreateVector(data_tensor_shape, 2),
                   TensorType_FLOAT32, 0,
                   builder->CreateString("output_tensor"), 0, false),
  };
  const Offset<Tensor> subgraph2_tensors[tensors_size] = {
      CreateTensor(*builder, builder->CreateVector(data_tensor_shape, 2),
                   TensorType_FLOAT32, 0,
                   builder->CreateString("input_tensor1"), 0, false),
      CreateTensor(*builder, builder->CreateVector(data_tensor_shape, 2),
                   TensorType_FLOAT32, 0,
                   builder->CreateString("input_tensor2"), 0, false),
      CreateTensor(*builder, builder->CreateVector(data_tensor_shape, 2),
                   TensorType_FLOAT32, 0,
                   builder->CreateString("output_tensor"), 0, false),
  };
  const Offset<Tensor> subgraph3_tensors[tensors_size] = {
      CreateTensor(*builder, builder->CreateVector(data_tensor_shape, 2),
                   TensorType_FLOAT32, 0,
                   builder->CreateString("input_tensor1"), 0, false),
      CreateTensor(*builder, builder->CreateVector(data_tensor_shape, 2),
                   TensorType_FLOAT32, 0,
                   builder->CreateString("input_tensor2"), 0, false),
      CreateTensor(*builder, builder->CreateVector(data_tensor_shape, 2),
                   TensorType_FLOAT32, 0,
                   builder->CreateString("output_tensor"), 0, false),
  };

  constexpr size_t if_inputs_size = 3;
  const int32_t if_inputs[if_inputs_size] = {0, 1, 2};
  constexpr size_t outputs_size = 1;
  const int32_t if_outputs[outputs_size] = {3};
  constexpr size_t operator_inputs_size = 2;
  const int32_t operator_inputs[operator_inputs_size] = {0, 1};
  const int32_t operator_outputs[outputs_size] = {2};
  constexpr size_t operators_size = 1;
  const Offset<Operator> subgraph1_operators[operators_size] = {
      CreateOperator(
          *builder, 0, builder->CreateVector(if_inputs, if_inputs_size),
          builder->CreateVector(if_outputs, outputs_size),
          BuiltinOptions_IfOptions, CreateIfOptions(*builder, 1, 2).Union()),
  };
  const Offset<Operator> subgraph2_operators[operators_size] = {
      CreateOperator(
          *builder, 1,
          builder->CreateVector(operator_inputs, operator_inputs_size),
          builder->CreateVector(operator_outputs, outputs_size),
          BuiltinOptions_NONE),
  };
  const Offset<Operator> subgraph3_operators[operators_size] = {
      CreateOperator(
          *builder, 2,
          builder->CreateVector(operator_inputs, operator_inputs_size),
          builder->CreateVector(operator_outputs, outputs_size),
          BuiltinOptions_NONE),
  };
  constexpr size_t subgraphs_size = 3;
  const Offset<SubGraph> subgraphs[subgraphs_size] = {
      CreateSubGraph(*builder, builder->CreateVector(subgraph1_tensors, 4),
                     builder->CreateVector(if_inputs, if_inputs_size),
                     builder->CreateVector(if_outputs, outputs_size),
                     builder->CreateVector(subgraph1_operators, operators_size),
                     builder->CreateString("if_subgraph")),
      CreateSubGraph(
          *builder, builder->CreateVector(subgraph2_tensors, 3),
          builder->CreateVector(operator_inputs, operator_inputs_size),
          builder->CreateVector(operator_outputs, outputs_size),
          builder->CreateVector(subgraph2_operators, operators_size),
          builder->CreateString("then_subgraph")),
      CreateSubGraph(
          *builder, builder->CreateVector(subgraph3_tensors, 3),
          builder->CreateVector(operator_inputs, operator_inputs_size),
          builder->CreateVector(operator_outputs, outputs_size),
          builder->CreateVector(subgraph3_operators, operators_size),
          builder->CreateString("else_subgraph")),
  };
  constexpr size_t operator_codes_size = 3;
  const Offset<OperatorCode> operator_codes[operator_codes_size] = {
      CreateOperatorCodeDirect(*builder, /*deprecated_builtin_code=*/0,
                               "multiple_inputs_op",
                               /*version=*/0, BuiltinOperator_IF),
      CreateOperatorCodeDirect(*builder, /*deprecated_builtin_code=*/0,
                               "multiple_inputs_op",
                               /*version=*/0, BuiltinOperator_ADD),
      CreateOperatorCodeDirect(*builder, /*deprecated_builtin_code=*/0,
                               "multiple_inputs_op",
                               /*version=*/0, BuiltinOperator_MUL),
  };
  const Offset<Model> model_offset = CreateModel(
      *builder, 0, builder->CreateVector(operator_codes, operator_codes_size),
      builder->CreateVector(subgraphs, subgraphs_size),
      builder->CreateString("test_model"),
      builder->CreateVector(buffers, buffers_size));
  FinishModelBuffer(*builder, model_offset);
  void* model_pointer = builder->GetBufferPointer();
  const Model* model = flatbuffers::GetRoot<Model>(model_pointer);
  return model;
}

const Model* BuildSimpleModelWithSubgraphsAndWhile() {
  using flatbuffers::Offset;
  flatbuffers::FlatBufferBuilder* builder = BuilderInstance();

  constexpr size_t buffers_size = 1;
  const Offset<Buffer> buffers[buffers_size] = {
      CreateBuffer(*builder),
  };
  const int32_t data_tensor_shape[] = {1, 1};
  constexpr size_t while_tensors_size = 4;
  constexpr size_t op_tensors_size = 3;
  const Offset<Tensor> subgraph0_tensors[while_tensors_size] = {
      CreateTensor(*builder, builder->CreateVector(data_tensor_shape, 1),
                   TensorType_FLOAT32, 0,
                   builder->CreateString("input_tensor0"), 0, false),
      CreateTensor(*builder, builder->CreateVector(data_tensor_shape, 1),
                   TensorType_FLOAT32, 0,
                   builder->CreateString("input_tensor1"), 0, false),
      CreateTensor(*builder, builder->CreateVector(data_tensor_shape, 1),
                   TensorType_FLOAT32, 0,
                   builder->CreateString("output_tensor0"), 0, false),
      CreateTensor(*builder, builder->CreateVector(data_tensor_shape, 1),
                   TensorType_FLOAT32, 0,
                   builder->CreateString("output_tensor1"), 0, false),
  };
  const Offset<Tensor> subgraph1_tensors[op_tensors_size] = {
      CreateTensor(*builder, builder->CreateVector(data_tensor_shape, 1),
                   TensorType_FLOAT32, 0,
                   builder->CreateString("input_tensor1"), 0, false),
      CreateTensor(*builder, builder->CreateVector(data_tensor_shape, 1),
                   TensorType_FLOAT32, 0,
                   builder->CreateString("input_tensor2"), 0, false),
      CreateTensor(*builder, builder->CreateVector(data_tensor_shape, 1),
                   TensorType_BOOL, 0,
                   builder->CreateString("condition_tensor"), 0, false),
  };
  const Offset<Tensor> subgraph2_tensors[op_tensors_size] = {
      CreateTensor(*builder, builder->CreateVector(data_tensor_shape, 1),
                   TensorType_FLOAT32, 0,
                   builder->CreateString("input_tensor0"), 0, false),
      CreateTensor(*builder, builder->CreateVector(data_tensor_shape, 1),
                   TensorType_FLOAT32, 0,
                   builder->CreateString("input_tensor1"), 0, false),
      CreateTensor(*builder, builder->CreateVector(data_tensor_shape, 1),
                   TensorType_FLOAT32, 0,
                   builder->CreateString("output_tensor0"), 0, false),
  };

  constexpr size_t inputs_size = 2;
  const int32_t inputs[inputs_size] = {0, 1};
  constexpr size_t while_outputs_size = 2;
  const int32_t while_outputs[while_outputs_size] = {2, 3};
  constexpr size_t cond_outputs_size = 1;
  const int32_t cond_outputs[cond_outputs_size] = {2};
  constexpr size_t add_outputs_size = 1;
  const int32_t add_outputs[add_outputs_size] = {2};
  constexpr size_t add_subgraph_outputs_size = 2;
  const int32_t add_subgraph_outputs[add_subgraph_outputs_size] = {2, 1};
  constexpr size_t operators_size = 1;
  const Offset<Operator> subgraph0_operators[operators_size] = {
      CreateOperator(*builder, 0, builder->CreateVector(inputs, inputs_size),
                     builder->CreateVector(while_outputs, while_outputs_size),
                     BuiltinOptions_WhileOptions,
                     CreateWhileOptions(*builder, 1, 2).Union()),
  };
  const Offset<Operator> subgraph1_operators[operators_size] = {
      CreateOperator(*builder, 1, builder->CreateVector(inputs, inputs_size),
                     builder->CreateVector(cond_outputs, cond_outputs_size),
                     BuiltinOptions_NONE),
  };
  const Offset<Operator> subgraph2_operators[operators_size] = {
      CreateOperator(*builder, 2, builder->CreateVector(inputs, inputs_size),
                     builder->CreateVector(add_outputs, add_outputs_size),
                     BuiltinOptions_NONE),
  };
  constexpr size_t subgraphs_size = 3;
  const Offset<SubGraph> subgraphs[subgraphs_size] = {
      CreateSubGraph(*builder, builder->CreateVector(subgraph0_tensors, 4),
                     builder->CreateVector(inputs, inputs_size),
                     builder->CreateVector(while_outputs, while_outputs_size),
                     builder->CreateVector(subgraph0_operators, operators_size),
                     builder->CreateString("while_subgraph")),
      CreateSubGraph(*builder, builder->CreateVector(subgraph1_tensors, 3),
                     builder->CreateVector(inputs, inputs_size),
                     builder->CreateVector(cond_outputs, cond_outputs_size),
                     builder->CreateVector(subgraph1_operators, operators_size),
                     builder->CreateString("cond_subgraph")),
      CreateSubGraph(*builder, builder->CreateVector(subgraph2_tensors, 3),
                     builder->CreateVector(inputs, inputs_size),
                     builder->CreateVector(add_subgraph_outputs,
                                           add_subgraph_outputs_size),
                     builder->CreateVector(subgraph2_operators, operators_size),
                     builder->CreateString("body_subgraph")),
  };
  constexpr size_t operator_codes_size = 3;
  const Offset<OperatorCode> operator_codes[operator_codes_size] = {
      CreateOperatorCodeDirect(*builder, /*deprecated_builtin_code=*/0,
                               "multiple_inputs_op",
                               /*version=*/0, BuiltinOperator_WHILE),
      CreateOperatorCodeDirect(*builder, /*deprecated_builtin_code=*/0,
                               "multiple_inputs_op",
                               /*version=*/0, BuiltinOperator_LESS),
      CreateOperatorCodeDirect(*builder, /*deprecated_builtin_code=*/0,
                               "multiple_inputs_op",
                               /*version=*/0, BuiltinOperator_ADD),
  };
  const Offset<Model> model_offset = CreateModel(
      *builder, 0, builder->CreateVector(operator_codes, operator_codes_size),
      builder->CreateVector(subgraphs, subgraphs_size),
      builder->CreateString("test_model"),
      builder->CreateVector(buffers, buffers_size));
  FinishModelBuffer(*builder, model_offset);
  void* model_pointer = builder->GetBufferPointer();
  const Model* model = flatbuffers::GetRoot<Model>(model_pointer);
  return model;
}

// Build a model with If and two subgraphs: two data tensors A1 of size 2, A2 of
// size 4 are first concatenated, then cut to a new tensor A3 of size 3; the new
// tensor A3 of size 3 is then concatenated with A2 tensor of size 4 to produce
// a final output tensor A4. This model is specially crafted to capture the
// corner case outlined in go/avoid-memory-corruption-in-if-operator.
//
//                Subgraph0
//            A0(1) A2_0(4)  A1_0(2)
//             |      |      | ---+
//             v      v      v    |
//            +--------------+    |
//            |     IF       |    |
//            +------+-------+    |
//                   | A3_0(3)      |
//                   v            |
//            +--------------+    |
//            |    CUSTOM    |<---+
//            +------+-------+
//                   |
//                   v
//                    A4_0(8)
//
//                Subgraph1/2
//              A1_1(2)      A2_1(4)
//                 |         |
//                 v         v
//             +---------------+
//             |   CUSTOM      |
//             +-------+-------+
//                     |
//                     v A3_1(3)
//
// And it leads to memory plan as below
//
//                  Subgraph0 Layout
//
//
//   <------------A4_0        -------------> <----- A2_0-------> <----A3_0 --->
//  +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//  |    |    |    |    |    |    |    |    | 3  | 4  | 5  |  6 |    |    |    |
//  +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//
//  +----+----+----+
//  | 1  |  2 | A0 |
//  +----+----+----+
//  <---A1_0-->
//
//                 Subgraph 1 Layout
//
//  +----+----+----+----+----+----+----+----+----+
//  |    |    |    |    |    |    |    |    |    |
//  +----+----+----+----+----+----+----+----+----+
//
//
//  <------A2_1 -------><----A3_1  ---><--A1_1--->
//
//
// A1_1 of subgraph 1 will overlap with A2_0 of subgraph 0.
// In a buggy implementation of IF, two overwrite may happen:
// 1. copying input from A1_0 to A1_1 overwrites A2_0 before A2_0 is copied to
// A2_1; thus subgraph 1 produce incorrect output.
// 2. copying output from A3_1 to A4_0 overwrites A1_0, which should remain
// intact so that it can be used by the OP after the IF operator in subgraph 0
//

const Model* BuildModelWithIfAndSubgraphInputTensorOverlap() {
  using flatbuffers::Offset;
  flatbuffers::FlatBufferBuilder* builder = BuilderInstance();

  constexpr TensorType kTensorType = TensorType_INT32;
  constexpr int kBlockSize =
      tflite::MicroArenaBufferAlignment() / sizeof(int32_t);
  constexpr size_t kBuffersCount = 1;
  const Offset<Buffer> buffers[kBuffersCount] = {
      CreateBuffer(*builder),
  };
  const int32_t kConditionTensorShape[] = {1};
  const int32_t kIfInput1TensorShape[] = {2 * kBlockSize};
  const int32_t kIfInput2TensorShape[] = {4 * kBlockSize};
  const int32_t kIfOutputTensorShape[] = {3 * kBlockSize};
  const int32_t kFinalOutputTensorShape[] = {8 * kBlockSize};
  constexpr size_t kSubgraph0TensorsCount = 5;
  const Offset<Tensor> kSubgraph0Tensors[kSubgraph0TensorsCount] = {
      CreateTensor(*builder, builder->CreateVector(kConditionTensorShape, 1),
                   TensorType_BOOL, 0,
                   builder->CreateString("condition tensor"), 0, false),
      CreateTensor(*builder, builder->CreateVector(kIfInput1TensorShape, 1),
                   kTensorType, 0, builder->CreateString("if_input_tensor1"), 0,
                   false),
      CreateTensor(*builder, builder->CreateVector(kIfInput2TensorShape, 1),
                   kTensorType, 0, builder->CreateString("if_input_tensor2"), 0,
                   false),
      CreateTensor(*builder, builder->CreateVector(kIfOutputTensorShape, 1),
                   kTensorType, 0, builder->CreateString("if_output_tensor"), 0,
                   false),
      CreateTensor(*builder, builder->CreateVector(kFinalOutputTensorShape, 1),
                   kTensorType, 0, builder->CreateString("final_output_tensor"),
                   0, false),
  };

  // Subgraph 1 is the chosen path if condition tensor in IF is true.
  constexpr size_t kSubgraph1TensorsCount = 3;
  const Offset<Tensor> kSubgraph1Tensors[kSubgraph1TensorsCount] = {
      CreateTensor(*builder, builder->CreateVector(kIfInput1TensorShape, 1),
                   kTensorType, 0,
                   builder->CreateString("subgraph1_input_tensor1"), 0, false),
      CreateTensor(*builder, builder->CreateVector(kIfInput2TensorShape, 1),
                   kTensorType, 0,
                   builder->CreateString("subgraph1_input_tensor2"), 0, false),
      CreateTensor(*builder, builder->CreateVector(kIfOutputTensorShape, 1),
                   kTensorType, 0,
                   builder->CreateString("subgraph1_output_tensor"), 0, false),
  };

  // Subgraph 2 is the chosen path if condition tensor in IF is false
  constexpr size_t kSubgraph2TensorsCount = 3;
  const Offset<Tensor> kSubgraph2Tensors[kSubgraph2TensorsCount] = {
      CreateTensor(*builder, builder->CreateVector(kIfInput1TensorShape, 1),
                   kTensorType, 0, builder->CreateString("if_input_tensor1"), 0,
                   false),
      CreateTensor(*builder, builder->CreateVector(kIfInput2TensorShape, 1),
                   kTensorType, 0, builder->CreateString("if_input_tensor2"), 0,
                   false),
      CreateTensor(*builder, builder->CreateVector(kIfOutputTensorShape, 1),
                   kTensorType, 0, builder->CreateString("if_output_tensor"), 0,
                   false),
  };

  constexpr int kIfOpCodeIndex = 0;
  constexpr int kCustomOpCodeIndex = 1;

  constexpr size_t kIfInputsCount = 3;
  const int32_t kIfInputs[kIfInputsCount] = {0, 1, 2};
  constexpr size_t kOutputsCount = 1;
  const int32_t kIfOutputs[kOutputsCount] = {3};
  constexpr size_t kOpAfterIfInputsCount = 2;
  const int32_t kOpAfterIfInputs[kOpAfterIfInputsCount] = {3, 2};
  const int32_t kOpAfterIfOutputs[kOutputsCount] = {4};
  constexpr size_t kOperatorsCount = 2;
  const Offset<Operator> kSubgraph0Operators[kOperatorsCount] = {
      CreateOperator(*builder, kIfOpCodeIndex,
                     builder->CreateVector(kIfInputs, kIfInputsCount),
                     builder->CreateVector(kIfOutputs, kOutputsCount),
                     BuiltinOptions_IfOptions,
                     CreateIfOptions(*builder, 1, 2).Union()),
      CreateOperator(
          *builder, kCustomOpCodeIndex,
          builder->CreateVector(kOpAfterIfInputs, kOpAfterIfInputsCount),
          builder->CreateVector(kOpAfterIfOutputs, kOutputsCount)),
  };

  constexpr size_t kSubgraph1InputsCount = 2;
  const int32_t kSubgraph1Inputs[kSubgraph1InputsCount] = {0, 1};
  constexpr size_t kSubgraph1OutputsCount = 1;
  const int32_t kSubgraph1Outputs[kSubgraph1OutputsCount] = {2};
  constexpr size_t kSubgraph1OperatorsCount = 1;
  const Offset<Operator> kSubgraph1Operators[kSubgraph1OperatorsCount] = {
      CreateOperator(
          *builder, kCustomOpCodeIndex,
          builder->CreateVector(kSubgraph1Inputs, kSubgraph1InputsCount),
          builder->CreateVector(kSubgraph1Outputs, kSubgraph1OutputsCount),
          BuiltinOptions_NONE),
  };

  constexpr size_t kSubgraph2InputsCount = 2;
  const int32_t kSubgraph2Inputs[kSubgraph2InputsCount] = {0, 1};
  constexpr size_t kSubgraph2OutputsCount = 1;
  const int32_t kSubgraph2Outputs[kSubgraph2OutputsCount] = {2};
  constexpr size_t kSubgraph2OperatorsCount = 1;
  const Offset<Operator> kSubgraph2Operators[kSubgraph2OperatorsCount] = {
      CreateOperator(
          *builder, kCustomOpCodeIndex,
          builder->CreateVector(kSubgraph2Inputs, kSubgraph2InputsCount),
          builder->CreateVector(kSubgraph2Outputs, kSubgraph2OutputsCount),
          BuiltinOptions_NONE),
  };

  constexpr size_t kSubgraphsCount = 3;
  const Offset<SubGraph> kSubgraphs[kSubgraphsCount] = {
      CreateSubGraph(
          *builder,
          builder->CreateVector(kSubgraph0Tensors, kSubgraph0TensorsCount),
          builder->CreateVector(kIfInputs, kIfInputsCount),
          builder->CreateVector(kOpAfterIfOutputs, kOutputsCount),
          builder->CreateVector(kSubgraph0Operators, kOperatorsCount),
          builder->CreateString("if_subgraph")),
      CreateSubGraph(
          *builder,
          builder->CreateVector(kSubgraph1Tensors, kSubgraph1TensorsCount),
          builder->CreateVector(kSubgraph1Inputs, kSubgraph1InputsCount),
          builder->CreateVector(kSubgraph1Outputs, kSubgraph1OutputsCount),
          builder->CreateVector(kSubgraph1Operators, kSubgraph1OperatorsCount),
          builder->CreateString("then_subgraph")),
      CreateSubGraph(
          *builder,
          builder->CreateVector(kSubgraph2Tensors, kSubgraph2TensorsCount),
          builder->CreateVector(kSubgraph2Inputs, kSubgraph2InputsCount),
          builder->CreateVector(kSubgraph2Outputs, kSubgraph2OutputsCount),
          builder->CreateVector(kSubgraph2Operators, kSubgraph2OperatorsCount),
          builder->CreateString("else_subgraph")),
  };

  constexpr size_t kOperatorCodesCount = 2;
  const Offset<OperatorCode> kOperatorCodes[kOperatorCodesCount] = {
      CreateOperatorCodeDirect(*builder, /*deprecated_builtin_code=*/0, "if",
                               /*version=*/0, BuiltinOperator_IF),
      CreateOperatorCodeDirect(*builder, /*deprecated_builtin_code=*/0,
                               "custom_packer_op",
                               /*version=*/0, BuiltinOperator_CUSTOM),
  };
  const Offset<Model> kModelOffset = CreateModel(
      *builder, 0, builder->CreateVector(kOperatorCodes, kOperatorCodesCount),
      builder->CreateVector(kSubgraphs, kSubgraphsCount),
      builder->CreateString("test_model"),
      builder->CreateVector(buffers, kBuffersCount));
  FinishModelBuffer(*builder, kModelOffset);
  void* model_pointer = builder->GetBufferPointer();
  const Model* model = flatbuffers::GetRoot<Model>(model_pointer);
  return model;
}

// Mock model with one main subgraph containing a single CALL_ONCE op (with null
// inputs and outputs) which invokes a second subgraph which has null inputs and
// outputs.
const Model* BuildSimpleMockModelWithNullInputsOutputs() {
  using flatbuffers::Offset;
  flatbuffers::FlatBufferBuilder* builder = BuilderInstance();

  constexpr size_t buffers_size = 1;
  const Offset<Buffer> buffers[buffers_size] = {
      CreateBuffer(*builder),
  };
  constexpr size_t tensor_shape_size = 1;
  const int32_t tensor_shape[tensor_shape_size] = {0};
  constexpr size_t tensors_size = 1;
  const Offset<Tensor> tensors[tensors_size] = {
      CreateTensor(*builder,
                   builder->CreateVector(tensor_shape, tensor_shape_size),
                   TensorType_INT32, 0,
                   builder->CreateString("test_input_tensor1"), 0, false),
  };
  constexpr size_t subgraph0_inputs_size = 1;
  const int32_t subgraph0_inputs[subgraph0_inputs_size] = {0};
  constexpr size_t subgraph0_outputs_size = 1;
  const int32_t subgraph0_outputs[subgraph0_outputs_size] = {0};
  constexpr size_t operators_size = 1;
  const Offset<Operator> subgraph0_operators[operators_size] = {
      CreateOperator(*builder, 0, {}, {}, BuiltinOptions_CallOnceOptions,
                     CreateCallOnceOptions(*builder, 1).Union()),
  };
  const Offset<Operator> subgraph1_operators[operators_size] = {
      CreateOperator(*builder, 1, {}, {}, BuiltinOptions_NONE)};
  constexpr size_t subgraphs_size = 2;
  const Offset<SubGraph> subgraphs[subgraphs_size] = {
      CreateSubGraph(
          *builder, builder->CreateVector(tensors, tensors_size),
          builder->CreateVector(subgraph0_inputs, subgraph0_inputs_size),
          builder->CreateVector(subgraph0_outputs, subgraph0_outputs_size),
          builder->CreateVector(subgraph0_operators, operators_size),
          builder->CreateString("main_subgraph")),
      CreateSubGraph(*builder, builder->CreateVector(tensors, tensors_size), {},
                     {},
                     builder->CreateVector(subgraph1_operators, operators_size),
                     builder->CreateString("secondary subgraph")),
  };
  constexpr size_t operator_codes_size = 2;
  const Offset<OperatorCode> operator_codes[operator_codes_size] = {
      CreateOperatorCodeDirect(*builder, /*deprecated_builtin_code=*/0,
                               "call_once_op",
                               /*version=*/0, BuiltinOperator_CALL_ONCE),
      CreateOperatorCodeDirect(*builder, /*deprecated_builtin_code=*/0, "no_op",
                               /*version=*/0, BuiltinOperator_CUSTOM)};
  const Offset<Model> model_offset = CreateModel(
      *builder, 0, builder->CreateVector(operator_codes, operator_codes_size),
      builder->CreateVector(subgraphs, subgraphs_size),
      builder->CreateString("test_model"),
      builder->CreateVector(buffers, buffers_size));
  FinishModelBuffer(*builder, model_offset);
  void* model_pointer = builder->GetBufferPointer();
  const Model* model = flatbuffers::GetRoot<Model>(model_pointer);
  return model;
}

}  // namespace

const TfLiteRegistration* SimpleStatefulOp::getRegistration() {
  return GetMutableRegistration();
}

TfLiteRegistration* SimpleStatefulOp::GetMutableRegistration() {
  static TfLiteRegistration r;
  r.init = Init;
  r.prepare = Prepare;
  r.invoke = Invoke;
  return &r;
}

void* SimpleStatefulOp::Init(TfLiteContext* context, const char* buffer,
                             size_t length) {
  TFLITE_DCHECK(context->AllocateBufferForEval == nullptr);
  TFLITE_DCHECK(context->GetScratchBuffer == nullptr);
  TFLITE_DCHECK(context->RequestScratchBufferInArena == nullptr);

  void* raw = context->AllocatePersistentBuffer(context, sizeof(OpData));
  OpData* data = reinterpret_cast<OpData*>(raw);
  *data = {};
  return raw;
}

TfLiteStatus SimpleStatefulOp::Prepare(TfLiteContext* context,
                                       TfLiteNode* node) {
  OpData* data = reinterpret_cast<OpData*>(node->user_data);

  // Make sure that the input is in uint8_t with at least 1 data entry.
  MicroContext* micro_context = GetMicroContext(context);
  TfLiteTensor* input =
      micro_context->AllocateTempInputTensor(node, kInputTensor);
  TF_LITE_ENSURE(context, input != nullptr);

  if (input->type != kTfLiteInt8) return kTfLiteError;
  if (NumElements(input->dims) == 0) return kTfLiteError;

  // Allocate a temporary buffer with the same size of input for sorting.
  TF_LITE_ENSURE_STATUS(context->RequestScratchBufferInArena(
      context, sizeof(uint8_t) * NumElements(input->dims),
      &data->sorting_buffer));
  // We can interleave scratch / persistent buffer allocation.
  data->invoke_count = reinterpret_cast<int*>(
      context->AllocatePersistentBuffer(context, sizeof(int)));
  *data->invoke_count = 0;

  micro_context->DeallocateTempTfLiteTensor(input);
  return kTfLiteOk;
}

TfLiteStatus SimpleStatefulOp::Invoke(TfLiteContext* context,
                                      TfLiteNode* node) {
  OpData* data = reinterpret_cast<OpData*>(node->user_data);
  *data->invoke_count += 1;

  const TfLiteEvalTensor* input =
      tflite::micro::GetEvalInput(context, node, kInputTensor);
  TF_LITE_ENSURE(context, input != nullptr);
  const uint8_t* input_data = input->data.uint8;
  int size = NumElements(input->dims);

  uint8_t* sorting_buffer = reinterpret_cast<uint8_t*>(
      context->GetScratchBuffer(context, data->sorting_buffer));
  // Copy inputs data to the sorting buffer. We don't want to mutate the input
  // tensor as it might be used by a another node.
  for (int i = 0; i < size; i++) {
    sorting_buffer[i] = input_data[i];
  }

  // In place insertion sort on `sorting_buffer`.
  for (int i = 1; i < size; i++) {
    for (int j = i; j > 0 && sorting_buffer[j] < sorting_buffer[j - 1]; j--) {
      std::swap(sorting_buffer[j], sorting_buffer[j - 1]);
    }
  }

  TfLiteEvalTensor* median =
      tflite::micro::GetEvalOutput(context, node, kMedianTensor);
  TF_LITE_ENSURE(context, median != nullptr);
  uint8_t* median_data = median->data.uint8;
  TfLiteEvalTensor* invoke_count =
      tflite::micro::GetEvalOutput(context, node, kInvokeCount);
  TF_LITE_ENSURE(context, invoke_count != nullptr);
  int32_t* invoke_count_data = invoke_count->data.i32;

  median_data[0] = sorting_buffer[size / 2];
  invoke_count_data[0] = *data->invoke_count;
  return kTfLiteOk;
}

const TfLiteRegistration* MockCustom::getRegistration() {
  return GetMutableRegistration();
}

TfLiteRegistration* MockCustom::GetMutableRegistration() {
  static TfLiteRegistration r;
  r.init = Init;
  r.prepare = Prepare;
  r.invoke = Invoke;
  r.free = Free;
  return &r;
}

void* MockCustom::Init(TfLiteContext* context, const char* buffer,
                       size_t length) {
  // We don't support delegate in TFL micro. This is a weak check to test if
  // context struct being zero-initialized.
  TFLITE_DCHECK(context->ReplaceNodeSubsetsWithDelegateKernels == nullptr);
  freed_ = false;
  // Do nothing.
  return nullptr;
}

void MockCustom::Free(TfLiteContext* context, void* buffer) { freed_ = true; }

TfLiteStatus MockCustom::Prepare(TfLiteContext* context, TfLiteNode* node) {
  return kTfLiteOk;
}

TfLiteStatus MockCustom::Invoke(TfLiteContext* context, TfLiteNode* node) {
  const TfLiteEvalTensor* input = tflite::micro::GetEvalInput(context, node, 0);
  TF_LITE_ENSURE(context, input != nullptr);
  const int32_t* input_data = input->data.i32;
  const TfLiteEvalTensor* weight =
      tflite::micro::GetEvalInput(context, node, 1);
  TF_LITE_ENSURE(context, weight != nullptr);
  const uint8_t* weight_data = weight->data.uint8;
  TfLiteEvalTensor* output = tflite::micro::GetEvalOutput(context, node, 0);
  TF_LITE_ENSURE(context, output != nullptr);
  int32_t* output_data = output->data.i32;
  output_data[0] =
      0;  // Catch output tensor sharing memory with an input tensor
  output_data[0] = input_data[0] + weight_data[0];
  return kTfLiteOk;
}

bool MockCustom::freed_ = false;

const TfLiteRegistration* MultipleInputs::getRegistration() {
  return GetMutableRegistration();
}

TfLiteRegistration* MultipleInputs::GetMutableRegistration() {
  static TfLiteRegistration r;
  r.init = Init;
  r.prepare = Prepare;
  r.invoke = Invoke;
  r.free = Free;
  return &r;
}

void* MultipleInputs::Init(TfLiteContext* context, const char* buffer,
                           size_t length) {
  // We don't support delegate in TFL micro. This is a weak check to test if
  // context struct being zero-initialized.
  TFLITE_DCHECK(context->ReplaceNodeSubsetsWithDelegateKernels == nullptr);
  freed_ = false;
  // Do nothing.
  return nullptr;
}

void MultipleInputs::Free(TfLiteContext* context, void* buffer) {
  freed_ = true;
}

TfLiteStatus MultipleInputs::Prepare(TfLiteContext* context, TfLiteNode* node) {
  return kTfLiteOk;
}

TfLiteStatus MultipleInputs::Invoke(TfLiteContext* context, TfLiteNode* node) {
  const TfLiteEvalTensor* input = tflite::micro::GetEvalInput(context, node, 0);
  TF_LITE_ENSURE(context, input != nullptr);
  const int32_t* input_data = input->data.i32;
  const TfLiteEvalTensor* input1 =
      tflite::micro::GetEvalInput(context, node, 1);
  TF_LITE_ENSURE(context, input1 != nullptr);
  const int32_t* input_data1 = input1->data.i32;
  const TfLiteEvalTensor* input2 =
      tflite::micro::GetEvalInput(context, node, 2);
  TF_LITE_ENSURE(context, input2 != nullptr);
  const int32_t* input_data2 = input2->data.i32;

  TfLiteEvalTensor* output = tflite::micro::GetEvalOutput(context, node, 0);
  TF_LITE_ENSURE(context, output != nullptr);
  int32_t* output_data = output->data.i32;
  output_data[0] =
      0;  // Catch output tensor sharing memory with an input tensor
  output_data[0] = input_data[0] + input_data1[0] + input_data2[0];
  return kTfLiteOk;
}

bool MultipleInputs::freed_ = false;

const TfLiteRegistration* NoOp::getRegistration() {
  return GetMutableRegistration();
}

TfLiteRegistration* NoOp::GetMutableRegistration() {
  static TfLiteRegistration r;
  r.init = Init;
  r.prepare = Prepare;
  r.invoke = Invoke;
  r.free = Free;
  return &r;
}

void* NoOp::Init(TfLiteContext* context, const char* buffer, size_t length) {
  // We don't support delegate in TFL micro. This is a weak check to test if
  // context struct being zero-initialized.
  TFLITE_DCHECK(context->ReplaceNodeSubsetsWithDelegateKernels == nullptr);
  freed_ = false;
  // Do nothing.
  return nullptr;
}

void NoOp::Free(TfLiteContext* context, void* buffer) { freed_ = true; }

TfLiteStatus NoOp::Prepare(TfLiteContext* context, TfLiteNode* node) {
  return kTfLiteOk;
}

TfLiteStatus NoOp::Invoke(TfLiteContext* context, TfLiteNode* node) {
  return kTfLiteOk;
}

bool NoOp::freed_ = false;

AllOpsResolver GetOpResolver() {
  AllOpsResolver op_resolver;
  op_resolver.AddCustom("mock_custom", MockCustom::GetMutableRegistration());
  op_resolver.AddCustom("simple_stateful_op",
                        SimpleStatefulOp::GetMutableRegistration());
  op_resolver.AddCustom("multiple_inputs_op",
                        MultipleInputs::GetMutableRegistration());
  op_resolver.AddCustom("no_op", NoOp::GetMutableRegistration());
  op_resolver.AddCustom("custom_packer_op", PackerOp::GetMutableRegistration());
  return op_resolver;
}

const Model* GetModelWithUnusedInputs() {
  static Model* model = nullptr;
  if (!model) {
    model = const_cast<Model*>(BuildModelWithUnusedInputs());
  }
  return model;
}

const Model* GetModelWithUnusedOperatorOutputs() {
  static Model* model = nullptr;
  if (!model) {
    model = const_cast<Model*>(BuildModelWithUnusedOperatorOutputs());
  }
  return model;
}

const Model* GetModelWith256x256Tensor() {
  static const Model* model = BuildModelWith256x256Tensor();
  return model;
}

const Model* GetSimpleMockModel() {
  static Model* model = nullptr;
  if (!model) {
    model = const_cast<Model*>(BuildSimpleMockModel());
  }
  return model;
}

const Model* GetSimpleMultipleInputsModel() {
  static Model* model = nullptr;
  if (!model) {
    model = const_cast<Model*>(BuildSimpleMultipleInputsModel());
  }
  return model;
}

const Model* GetSimpleModelWithSubgraphsAndIf() {
  static Model* model = nullptr;
  if (!model) {
    model = const_cast<Model*>(BuildSimpleModelWithSubgraphsAndIf());
  }
  return model;
}

const Model* GetSimpleModelWithSubgraphsAndWhile() {
  static Model* model = nullptr;
  if (!model) {
    model = const_cast<Model*>(BuildSimpleModelWithSubgraphsAndWhile());
  }
  return model;
}

const Model* GetModelWithIfAndSubgraphInputTensorOverlap() {
  static Model* model = nullptr;
  if (!model) {
    model = const_cast<Model*>(BuildModelWithIfAndSubgraphInputTensorOverlap());
  }
  return model;
}

const Model* GetSimpleModelWithNullInputsAndOutputs() {
  static Model* model = nullptr;
  if (!model) {
    model = const_cast<Model*>(BuildSimpleMockModelWithNullInputsOutputs());
  }
  return model;
}

const Model* GetComplexMockModel() {
  static Model* model = nullptr;
  if (!model) {
    model = const_cast<Model*>(BuildComplexMockModel());
  }
  return model;
}

const Model* GetSimpleModelWithBranch() {
  static Model* model = nullptr;
  if (!model) {
    model = const_cast<Model*>(BuildSimpleModelWithBranch());
  }
  return model;
}

const Model* GetModelWithOfflinePlanning(int num_tensors,
                                         const int32_t* metadata_buffer,
                                         NodeConnection* node_conn,
                                         int num_conns,
                                         int num_subgraph_inputs) {
  const Model* model = BuildModelWithOfflinePlanning(
      num_tensors, metadata_buffer, node_conn, num_conns, num_subgraph_inputs);
  return model;
}

const Model* GetSimpleStatefulModel() {
  static Model* model = nullptr;
  if (!model) {
    model = const_cast<Model*>(BuildSimpleStatefulModel());
  }
  return model;
}

const Tensor* Create1dFlatbufferTensor(int size, bool is_variable) {
  using flatbuffers::Offset;
  flatbuffers::FlatBufferBuilder* builder = BuilderInstance();
  constexpr size_t tensor_shape_size = 1;
  const int32_t tensor_shape[tensor_shape_size] = {size};
  const Offset<Tensor> tensor_offset = CreateTensor(
      *builder, builder->CreateVector(tensor_shape, tensor_shape_size),
      TensorType_INT32, 0, builder->CreateString("test_tensor"), 0,
      is_variable);
  builder->Finish(tensor_offset);
  void* tensor_pointer = builder->GetBufferPointer();
  const Tensor* tensor = flatbuffers::GetRoot<Tensor>(tensor_pointer);
  return tensor;
}

const Tensor* CreateQuantizedFlatbufferTensor(int size) {
  using flatbuffers::Offset;
  flatbuffers::FlatBufferBuilder* builder = BuilderInstance();
  constexpr size_t quant_params_size = 1;
  const float min_array[quant_params_size] = {0.1f};
  const float max_array[quant_params_size] = {0.2f};
  const float scale_array[quant_params_size] = {0.3f};
  const int64_t zero_point_array[quant_params_size] = {100ll};

  const Offset<QuantizationParameters> quant_params =
      CreateQuantizationParameters(
          *builder,
          /*min=*/builder->CreateVector<float>(min_array, quant_params_size),
          /*max=*/builder->CreateVector<float>(max_array, quant_params_size),
          /*scale=*/
          builder->CreateVector<float>(scale_array, quant_params_size),
          /*zero_point=*/
          builder->CreateVector<int64_t>(zero_point_array, quant_params_size));

  constexpr size_t tensor_shape_size = 1;
  const int32_t tensor_shape[tensor_shape_size] = {size};
  const Offset<Tensor> tensor_offset = CreateTensor(
      *builder, builder->CreateVector(tensor_shape, tensor_shape_size),
      TensorType_INT32, 0, builder->CreateString("test_tensor"), quant_params,
      false);
  builder->Finish(tensor_offset);
  void* tensor_pointer = builder->GetBufferPointer();
  const Tensor* tensor = flatbuffers::GetRoot<Tensor>(tensor_pointer);
  return tensor;
}

const Tensor* CreateMissingQuantizationFlatbufferTensor(int size) {
  using flatbuffers::Offset;
  flatbuffers::FlatBufferBuilder* builder = BuilderInstance();
  const Offset<QuantizationParameters> quant_params =
      CreateQuantizationParameters(*builder, 0, 0, 0, 0,
                                   QuantizationDetails_NONE, 0, 0);
  constexpr size_t tensor_shape_size = 1;
  const int32_t tensor_shape[tensor_shape_size] = {size};
  const Offset<Tensor> tensor_offset = CreateTensor(
      *builder, builder->CreateVector(tensor_shape, tensor_shape_size),
      TensorType_INT32, 0, builder->CreateString("test_tensor"), quant_params,
      false);
  builder->Finish(tensor_offset);
  void* tensor_pointer = builder->GetBufferPointer();
  const Tensor* tensor = flatbuffers::GetRoot<Tensor>(tensor_pointer);
  return tensor;
}

const flatbuffers::Vector<flatbuffers::Offset<Buffer>>*
CreateFlatbufferBuffers() {
  using flatbuffers::Offset;
  flatbuffers::FlatBufferBuilder* builder = BuilderInstance();
  constexpr size_t buffers_size = 1;
  const Offset<Buffer> buffers[buffers_size] = {
      CreateBuffer(*builder),
  };
  const flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Buffer>>>
      buffers_offset = builder->CreateVector(buffers, buffers_size);
  builder->Finish(buffers_offset);
  void* buffers_pointer = builder->GetBufferPointer();
  const flatbuffers::Vector<flatbuffers::Offset<Buffer>>* result =
      flatbuffers::GetRoot<flatbuffers::Vector<flatbuffers::Offset<Buffer>>>(
          buffers_pointer);
  return result;
}

int TestStrcmp(const char* a, const char* b) {
  if ((a == nullptr) || (b == nullptr)) {
    return -1;
  }
  while ((*a != 0) && (*a == *b)) {
    a++;
    b++;
  }
  return *reinterpret_cast<const unsigned char*>(a) -
         *reinterpret_cast<const unsigned char*>(b);
}

// Wrapper to forward kernel errors to the interpreter's error reporter.
void ReportOpError(struct TfLiteContext* context, const char* format, ...) {
#ifndef TF_LITE_STRIP_ERROR_STRINGS
  ErrorReporter* error_reporter = static_cast<ErrorReporter*>(context->impl_);
  va_list args;
  va_start(args, format);
  TF_LITE_REPORT_ERROR(error_reporter, format, args);
  va_end(args);
#endif
}

// Create a TfLiteIntArray from an array of ints.  The first element in the
// supplied array must be the size of the array expressed as an int.
TfLiteIntArray* IntArrayFromInts(int* int_array) {
  return reinterpret_cast<TfLiteIntArray*>(int_array);
}

// Create a TfLiteFloatArray from an array of floats.  The first element in the
// supplied array must be the size of the array expressed as a float.
TfLiteFloatArray* FloatArrayFromFloats(const float* floats) {
  static_assert(sizeof(float) == sizeof(int),
                "assumes sizeof(float) == sizeof(int) to perform casting");
  int size = static_cast<int>(floats[0]);
  *reinterpret_cast<int32_t*>(const_cast<float*>(floats)) = size;
  return reinterpret_cast<TfLiteFloatArray*>(const_cast<float*>(floats));
}

TfLiteTensor CreateQuantizedBiasTensor(const float* data, int16_t* quantized,
                                       TfLiteIntArray* dims, float input_scale,
                                       float weights_scale, bool is_variable) {
  float bias_scale = input_scale * weights_scale;
  tflite::SymmetricQuantize(data, quantized, ElementCount(*dims), bias_scale);

  // Quantized int16_t tensors always have a zero point of 0, since the range of
  // int16_t values is large, and because zero point costs extra cycles during
  // processing.
  TfLiteTensor result =
      CreateQuantizedTensor(quantized, dims, bias_scale, 0, is_variable);
  return result;
}

TfLiteTensor CreateQuantizedBiasTensor(const float* data, int32_t* quantized,
                                       TfLiteIntArray* dims, float input_scale,
                                       float weights_scale, bool is_variable) {
  float bias_scale = input_scale * weights_scale;
  tflite::SymmetricQuantize(data, quantized, ElementCount(*dims), bias_scale);

  // Quantized int32_t tensors always have a zero point of 0, since the range of
  // int32_t values is large, and because zero point costs extra cycles during
  // processing.
  TfLiteTensor result =
      CreateQuantizedTensor(quantized, dims, bias_scale, 0, is_variable);
  return result;
}

TfLiteTensor CreateQuantizedBiasTensor(const float* data,
                                       std::int64_t* quantized,
                                       TfLiteIntArray* dims, float input_scale,
                                       float weights_scale, bool is_variable) {
  float bias_scale = input_scale * weights_scale;
  tflite::SymmetricQuantize(data, quantized, ElementCount(*dims), bias_scale);

  // Quantized int32_t tensors always have a zero point of 0, since the range of
  // int32_t values is large, and because zero point costs extra cycles during
  // processing.
  TfLiteTensor result =
      CreateQuantizedTensor(quantized, dims, bias_scale, 0, is_variable);
  return result;
}

// Quantizes int32_t bias tensor with per-channel weights determined by input
// scale multiplied by weight scale for each channel.
template <typename T>
TfLiteTensor CreatePerChannelQuantizedBiasTensor(
    const float* input, T* quantized, TfLiteIntArray* dims, float input_scale,
    float* weight_scales, float* scales, int* zero_points,
    TfLiteAffineQuantization* affine_quant, int quantized_dimension,
    bool is_variable) {
  int input_size = ElementCount(*dims);
  int num_channels = dims->data[quantized_dimension];
  // First element is reserved for array length
  zero_points[0] = num_channels;
  scales[0] = static_cast<float>(num_channels);
  float* scales_array = &scales[1];
  for (int i = 0; i < num_channels; i++) {
    scales_array[i] = input_scale * weight_scales[i];
    zero_points[i + 1] = 0;
  }

  SymmetricPerChannelQuantize<T>(input, quantized, input_size, num_channels,
                                 scales_array);

  affine_quant->scale = FloatArrayFromFloats(scales);
  affine_quant->zero_point = IntArrayFromInts(zero_points);
  affine_quant->quantized_dimension = quantized_dimension;

  TfLiteTensor result = CreateTensor(quantized, dims, is_variable);
  result.quantization = {kTfLiteAffineQuantization, affine_quant};
  return result;
}

TfLiteTensor CreatePerChannelQuantizedBiasTensor(
    const float* input, int32_t* quantized, TfLiteIntArray* dims,
    float input_scale, float* weight_scales, float* scales, int* zero_points,
    TfLiteAffineQuantization* affine_quant, int quantized_dimension,
    bool is_variable) {
  return CreatePerChannelQuantizedBiasTensor<int32_t>(
      input, quantized, dims, input_scale, weight_scales, scales, zero_points,
      affine_quant, quantized_dimension, is_variable);
}

TfLiteTensor CreatePerChannelQuantizedBiasTensor(
    const float* input, std::int64_t* quantized, TfLiteIntArray* dims,
    float input_scale, float* weight_scales, float* scales, int* zero_points,
    TfLiteAffineQuantization* affine_quant, int quantized_dimension,
    bool is_variable) {
  return CreatePerChannelQuantizedBiasTensor<std::int64_t>(
      input, quantized, dims, input_scale, weight_scales, scales, zero_points,
      affine_quant, quantized_dimension, is_variable);
}

TfLiteTensor CreateSymmetricPerChannelQuantizedTensor(
    const float* input, int8_t* quantized, TfLiteIntArray* dims, float* scales,
    int* zero_points, TfLiteAffineQuantization* affine_quant,
    int quantized_dimension, bool is_variable) {
  int channel_count = dims->data[quantized_dimension];
  scales[0] = static_cast<float>(channel_count);
  zero_points[0] = channel_count;

  SignedSymmetricPerChannelQuantize(input, dims, quantized_dimension, quantized,
                                    &scales[1]);

  for (int i = 0; i < channel_count; i++) {
    zero_points[i + 1] = 0;
  }

  affine_quant->scale = FloatArrayFromFloats(scales);
  affine_quant->zero_point = IntArrayFromInts(zero_points);
  affine_quant->quantized_dimension = quantized_dimension;

  TfLiteTensor result = CreateTensor(quantized, dims, is_variable);
  result.quantization = {kTfLiteAffineQuantization, affine_quant};
  return result;
}

size_t GetModelTensorCount(const Model* model) {
  auto* subgraphs = model->subgraphs();
  if (subgraphs) {
    return (*subgraphs)[0]->tensors()->size();
  }
  return 0;
}

}  // namespace testing
}  // namespace tflite
