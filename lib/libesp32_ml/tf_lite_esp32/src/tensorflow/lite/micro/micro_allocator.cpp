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

#include "tensorflow/lite/micro/micro_allocator.h"

#include <cstddef>
#include <cstdint>

#include "third_party/flatbuffers/include/flatbuffers/flatbuffers.h"
#include "tensorflow/lite/c/c_api_types.h"
#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/core/api/flatbuffer_conversions.h"
#include "tensorflow/lite/core/api/op_resolver.h"
#include "tensorflow/lite/core/api/tensor_utils.h"
#include "tensorflow/lite/kernels/internal/compatibility.h"
#include "tensorflow/lite/micro/arena_allocator/non_persistent_arena_buffer_allocator.h"
#include "tensorflow/lite/micro/arena_allocator/persistent_arena_buffer_allocator.h"
#include "tensorflow/lite/micro/arena_allocator/single_arena_buffer_allocator.h"
#include "tensorflow/lite/micro/compatibility.h"
#include "tensorflow/lite/micro/flatbuffer_utils.h"
#include "tensorflow/lite/micro/memory_helpers.h"
#include "tensorflow/lite/micro/memory_planner/greedy_memory_planner.h"
#include "tensorflow/lite/micro/memory_planner/micro_memory_planner.h"
#include "tensorflow/lite/micro/micro_allocation_info.h"
#include "tensorflow/lite/micro/micro_arena_constants.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/schema/schema_utils.h"

namespace tflite {

namespace {

// Maximum number of scratch buffer requests per operator. Operator kernels that
// request more than this value will receive an exception.
constexpr size_t kMaxScratchBuffersPerOp = 12;

// Sentinel value used as a placeholder to mark a ScratchBufferRequest request
// needs a node id assignment.
constexpr int kUnassignedScratchBufferRequestIndex = -1;

const TfLiteIntArray kZeroLengthIntArray = {};

class MicroBuiltinDataAllocator : public BuiltinDataAllocator {
 public:
  explicit MicroBuiltinDataAllocator(
      IPersistentBufferAllocator* persistent_allocator)
      : persistent_allocator_(persistent_allocator) {}

  void* Allocate(size_t size, size_t alignment_hint) override {
    return persistent_allocator_->AllocatePersistentBuffer(size,
                                                           alignment_hint);
  }
  void Deallocate(void* data) override {
    // Do not deallocate, builtin data needs to be available for the life time
    // of the model.
  }

  TF_LITE_REMOVE_VIRTUAL_DELETE

 private:
  IPersistentBufferAllocator* persistent_allocator_;
};

TfLiteStatus CreatePlan(MicroMemoryPlanner* planner,
                        const AllocationInfo* allocation_info,
                        size_t allocation_info_size) {
  // Add the tensors to our allocation plan.
  for (size_t i = 0; i < allocation_info_size; ++i) {
    const AllocationInfo* current = &allocation_info[i];
    if (current->needs_allocating) {
      size_t aligned_bytes_required =
          AlignSizeUp(current->bytes, MicroArenaBufferAlignment());
      if (current->offline_offset == kOnlinePlannedBuffer) {
        TF_LITE_ENSURE_STATUS(planner->AddBuffer(aligned_bytes_required,
                                                 current->first_created,
                                                 current->last_used));
      } else {
        TF_LITE_ENSURE_STATUS(
            planner->AddBuffer(aligned_bytes_required, current->first_created,
                               current->last_used, current->offline_offset));
      }
    }
  }
  return kTfLiteOk;
}

TfLiteStatus CommitPlan(MicroMemoryPlanner* planner, uint8_t* starting_point,
                        const AllocationInfo* allocation_info,
                        size_t allocation_info_size) {
  // Figure out the actual memory addresses for each buffer, based on the plan.
  int planner_index = 0;
  for (size_t i = 0; i < allocation_info_size; ++i) {
    const AllocationInfo* current = &allocation_info[i];
    if (current->needs_allocating) {
      int offset = -1;
      TF_LITE_ENSURE_STATUS(
          planner->GetOffsetForBuffer(planner_index, &offset));
      *current->output_ptr = reinterpret_cast<void*>(starting_point + offset);
      ++planner_index;
    }
  }
  return kTfLiteOk;
}

IPersistentBufferAllocator* CreatePersistentArenaAllocator(uint8_t* buffer_head,
                                                           size_t buffer_size) {
  // Align the actually used area by the tail because persistent buffer grows
  // from the bottom to top.
  uint8_t* aligned_buffer_tail =
      AlignPointerDown(buffer_head + buffer_size, MicroArenaBufferAlignment());
  size_t aligned_buffer_size = aligned_buffer_tail - buffer_head;
  PersistentArenaBufferAllocator tmp =
      PersistentArenaBufferAllocator(buffer_head, aligned_buffer_size);

  // Allocate enough bytes from the buffer to create a
  // SingleArenaBufferAllocator. The new instance will use the current adjusted
  // tail buffer from the tmp allocator instance.
  uint8_t* allocator_buffer =
      tmp.AllocatePersistentBuffer(sizeof(PersistentArenaBufferAllocator),
                                   alignof(PersistentArenaBufferAllocator));
  // Use the default copy constructor to populate internal states.
  return new (allocator_buffer) PersistentArenaBufferAllocator(tmp);
}

// NonPersistentBufferAllocator instance is created in the persistent buffer
// because it has to be persistent to keep track of the non-persistent buffer
// information.
INonPersistentBufferAllocator* CreateNonPersistentArenaAllocator(
    uint8_t* buffer_head, size_t buffer_size,
    IPersistentBufferAllocator* persistent_buffer_allocator) {
  uint8_t* allocator_buffer =
      persistent_buffer_allocator->AllocatePersistentBuffer(
          sizeof(NonPersistentArenaBufferAllocator),
          alignof(NonPersistentArenaBufferAllocator));
  // Align the actually used area by the head because persistent buffer grows
  // from the head to bottom.
  uint8_t* aligned_buffer_head =
      AlignPointerUp(buffer_head, MicroArenaBufferAlignment());
  size_t aligned_buffer_size = buffer_head + buffer_size - aligned_buffer_head;

  INonPersistentBufferAllocator* non_persistent_buffer_allocator =
      new (allocator_buffer) NonPersistentArenaBufferAllocator(
          aligned_buffer_head, aligned_buffer_size);
  return non_persistent_buffer_allocator;
}

}  // namespace

namespace internal {

// Returns a pointer to any buffer associated with the flatbuffer tensor. Can
// return nullptr if no buffer is found.
void* GetFlatbufferTensorBuffer(
    const tflite::Tensor& flatbuffer_tensor,
    const flatbuffers::Vector<flatbuffers::Offset<Buffer>>* buffers) {
  // We need to figure out where the actual contents of this tensor are stored
  // in memory. We'll check to see if there's a serialized buffer (pretty much
  // the same as a constant op in TensorFlow) associated with this tensor first,
  // and if there is update the runtime structure to point to its location in
  // memory.
  // First see if there's any buffer information in the serialized tensor.
  // TODO(b/170379532): Add better unit tests to validate flatbuffer values.
  void* out_buffer = nullptr;
  if (auto* buffer = (*buffers)[flatbuffer_tensor.buffer()]) {
    // If we've found a buffer, does it have any data?
    if (auto* array = buffer->data()) {
      // If it has any data, is the data size larger than zero?
      if (array->size()) {
        // We've found a buffer with valid data, so update the runtime tensor
        // data structure to point to it.
        out_buffer = const_cast<void*>(static_cast<const void*>(array->data()));
      }
    }
    // TODO(petewarden): It's not clear in what circumstances we could have a
    // buffer in the serialized tensor, but it doesn't have any data in it. Is
    // that a validly-generated file, and if so what does it mean, or is it an
    // error condition? It would be good to tighten up the specification to make
    // it less ambiguous.
  }
  return out_buffer;
}

TfLiteStatus InitializeTfLiteTensorFromFlatbuffer(
    IPersistentBufferAllocator* persistent_buffer_allocator,
    INonPersistentBufferAllocator* non_persistent_buffer_allocator,
    bool allocate_temp, const tflite::Tensor& flatbuffer_tensor,
    const flatbuffers::Vector<flatbuffers::Offset<Buffer>>* buffers,
    TfLiteTensor* result) {
  TFLITE_DCHECK(result != nullptr);

  *result = {};
  // Make sure the serialized type is one we know how to deal with, and convert
  // it from a flatbuffer enum into a constant used by the kernel C API.
  TF_LITE_ENSURE_STATUS(ConvertTensorType(flatbuffer_tensor.type(),
                                          &result->type,
                                          tflite::GetMicroErrorReporter()));
  // Make sure we remember if the serialized tensor is designated as a variable.
  result->is_variable = flatbuffer_tensor.is_variable();

  result->data.data = GetFlatbufferTensorBuffer(flatbuffer_tensor, buffers);

  // TODO(petewarden): Some of these paths aren't getting enough testing
  // coverage, so we should figure out some tests that exercise them.
  if (result->data.data == nullptr) {
    // The tensor contents haven't been set from a serialized buffer, so
    // make a note that they will be allocated from memory. The actual
    // allocation won't happen until later.
    result->allocation_type = kTfLiteArenaRw;
  } else {
    // We set the data from a serialized buffer, so record tha.
    result->allocation_type = kTfLiteMmapRo;
  }

  // Figure out what the size in bytes of the buffer is and store it.
  size_t type_size;
  TF_LITE_ENSURE_STATUS(
      BytesRequiredForTensor(flatbuffer_tensor, &result->bytes, &type_size));

  if (flatbuffer_tensor.shape() == nullptr) {
    // flatbuffer_tensor.shape() can return a nullptr in the case of a scalar
    // tensor.
    // TODO(b/188459715): figure out why const_cast is required here.
    result->dims = const_cast<TfLiteIntArray*>(&kZeroLengthIntArray);
  } else {
    // TFLM doesn't allow reshaping the tensor which requires dynamic memory
    // allocation so it is safe to drop the const qualifier. In the future, if
    // we really want to update the tensor shape, we can always pass in a new
    // TfLiteIntArray - especially we have to do so if the dimension is
    result->dims = FlatBufferVectorToTfLiteTypeArray(flatbuffer_tensor.shape());
  }

  // Copy the quantization information from the serialized data.
  const auto* src_quantization = flatbuffer_tensor.quantization();
  if (src_quantization && src_quantization->scale() &&
      (src_quantization->scale()->size() > 0) &&
      src_quantization->zero_point() &&
      (src_quantization->zero_point()->size() > 0)) {
    // Always populate the TfLiteTensor.params field, even if there are
    // per-channel quantization parameters.
    result->params.scale = src_quantization->scale()->Get(0);
    // Note that the zero_point field in the FlatBuffers schema is a 64-bit
    // integer, but the zero_point field in the TfLiteQuantizationParams struct
    // is a 32-bit integer.
    result->params.zero_point =
        static_cast<int32_t>(src_quantization->zero_point()->Get(0));

    // Populate per-channel quantization params.
    int channels = src_quantization->scale()->size();
    TfLiteAffineQuantization* quantization =
        allocate_temp
            ? reinterpret_cast<TfLiteAffineQuantization*>(
                  non_persistent_buffer_allocator->AllocateTemp(
                      sizeof(TfLiteAffineQuantization),
                      alignof(TfLiteAffineQuantization)))
            : reinterpret_cast<TfLiteAffineQuantization*>(
                  persistent_buffer_allocator->AllocatePersistentBuffer(
                      sizeof(TfLiteAffineQuantization),
                      alignof(TfLiteAffineQuantization)));
    if (quantization == nullptr) {
      MicroPrintf("Unable to allocate TfLiteAffineQuantization.\n");
      return kTfLiteError;
    }

    // TODO(b/153688719): Reduce tail allocation by using a global zero-point
    // buffer. This value can not be reused from the flatbuffer since the
    // zero_point is stored as a int64_t.
    quantization->zero_point =
        allocate_temp
            ? reinterpret_cast<TfLiteIntArray*>(
                  non_persistent_buffer_allocator->AllocateTemp(
                      TfLiteIntArrayGetSizeInBytes(channels),
                      alignof(TfLiteIntArray)))
            : reinterpret_cast<TfLiteIntArray*>(
                  persistent_buffer_allocator->AllocatePersistentBuffer(
                      TfLiteIntArrayGetSizeInBytes(channels),
                      alignof(TfLiteIntArray)));
    if (quantization->zero_point == nullptr) {
      MicroPrintf("Unable to allocate quantization->zero_point.\n");
      return kTfLiteError;
    }

    quantization->scale =
        FlatBufferVectorToTfLiteTypeArray(src_quantization->scale());

    quantization->zero_point->size = channels;
    int* zero_point_data = quantization->zero_point->data;
    for (int i = 0; i < channels; i++) {
      // As a space-saving optimization, zero point arrays for weights can be
      // reduced to a single value, since all zero points for weights are 0.
      zero_point_data[i] = src_quantization->zero_point()->size() ==
                                   src_quantization->scale()->size()
                               ? src_quantization->zero_point()->Get(i)
                               : src_quantization->zero_point()->Get(0);
    }
    // TODO(rocky): Need to add a micro_allocator test case that fails when
    // this is not copied:
    quantization->quantized_dimension = src_quantization->quantized_dimension();

    result->quantization = {kTfLiteAffineQuantization, quantization};
  }
  return kTfLiteOk;
}

TfLiteStatus InitializeTfLiteEvalTensorFromFlatbuffer(
    const tflite::Tensor& flatbuffer_tensor,
    const flatbuffers::Vector<flatbuffers::Offset<Buffer>>* buffers,
    TfLiteEvalTensor* result) {
  *result = {};
  // Make sure the serialized type is one we know how to deal with, and convert
  // it from a flatbuffer enum into a constant used by the kernel C API.
  TF_LITE_ENSURE_STATUS(ConvertTensorType(flatbuffer_tensor.type(),
                                          &result->type,
                                          tflite::GetMicroErrorReporter()));

  result->data.data = GetFlatbufferTensorBuffer(flatbuffer_tensor, buffers);

  if (flatbuffer_tensor.shape() == nullptr) {
    // flatbuffer_tensor.shape() can return a nullptr in the case of a scalar
    // tensor.
    result->dims = const_cast<TfLiteIntArray*>(&kZeroLengthIntArray);
  } else {
    result->dims = FlatBufferVectorToTfLiteTypeArray(flatbuffer_tensor.shape());
  }
  return kTfLiteOk;
}

}  // namespace internal

size_t MicroAllocator::GetDefaultTailUsage(bool is_memory_planner_given) {
  // TODO(b/208703041): a template version of AlignSizeUp to make expression
  // shorter.
  size_t total_size =
      AlignSizeUp(sizeof(SingleArenaBufferAllocator),
                  alignof(SingleArenaBufferAllocator)) +
      AlignSizeUp(sizeof(MicroAllocator), alignof(MicroAllocator)) +
      AlignSizeUp(sizeof(MicroBuiltinDataAllocator),
                  alignof(MicroBuiltinDataAllocator)) +
      AlignSizeUp(sizeof(SubgraphAllocations), alignof(SubgraphAllocations));
  if (!is_memory_planner_given) {
    total_size +=
        AlignSizeUp(sizeof(GreedyMemoryPlanner), alignof(GreedyMemoryPlanner));
  }
  return total_size;
}

MicroAllocator::MicroAllocator(SingleArenaBufferAllocator* memory_allocator,
                               MicroMemoryPlanner* memory_planner)
    : non_persistent_buffer_allocator_(memory_allocator),
      persistent_buffer_allocator_(memory_allocator),
      memory_planner_(memory_planner),
      model_is_allocating_(false) {}

MicroAllocator::MicroAllocator(
    IPersistentBufferAllocator* persistent_buffer_allocator,
    INonPersistentBufferAllocator* non_persistent_buffer_allocator,
    MicroMemoryPlanner* memory_planner)
    : non_persistent_buffer_allocator_(non_persistent_buffer_allocator),
      persistent_buffer_allocator_(persistent_buffer_allocator),
      memory_planner_(memory_planner),
      model_is_allocating_(false) {}

MicroAllocator::~MicroAllocator() {}

MicroAllocator* MicroAllocator::Create(uint8_t* tensor_arena, size_t arena_size,
                                       MicroMemoryPlanner* memory_planner) {
  uint8_t* aligned_arena =
      AlignPointerUp(tensor_arena, MicroArenaBufferAlignment());
  size_t aligned_arena_size = tensor_arena + arena_size - aligned_arena;
  SingleArenaBufferAllocator* memory_allocator =
      SingleArenaBufferAllocator::Create(aligned_arena, aligned_arena_size);

  return Create(memory_allocator, memory_planner);
}

MicroAllocator* MicroAllocator::Create(uint8_t* tensor_arena,
                                       size_t arena_size) {
  uint8_t* aligned_arena =
      AlignPointerUp(tensor_arena, MicroArenaBufferAlignment());
  size_t aligned_arena_size = tensor_arena + arena_size - aligned_arena;
  SingleArenaBufferAllocator* memory_allocator =
      SingleArenaBufferAllocator::Create(aligned_arena, aligned_arena_size);

  // By default create GreedyMemoryPlanner.
  // If a different MemoryPlanner is needed, use the other api.
  uint8_t* memory_planner_buffer = memory_allocator->AllocatePersistentBuffer(
      sizeof(GreedyMemoryPlanner), alignof(GreedyMemoryPlanner));
  GreedyMemoryPlanner* memory_planner =
      new (memory_planner_buffer) GreedyMemoryPlanner();

  return Create(memory_allocator, memory_planner);
}

MicroAllocator* MicroAllocator::Create(
    SingleArenaBufferAllocator* memory_allocator,
    MicroMemoryPlanner* memory_planner) {
  TFLITE_DCHECK(memory_allocator != nullptr);
  TFLITE_DCHECK(memory_planner != nullptr);

  uint8_t* allocator_buffer = memory_allocator->AllocatePersistentBuffer(
      sizeof(MicroAllocator), alignof(MicroAllocator));
  MicroAllocator* allocator = new (allocator_buffer)
      MicroAllocator(memory_allocator, memory_allocator, memory_planner);
  return allocator;
}

MicroAllocator* MicroAllocator::Create(uint8_t* persistent_tensor_arena,
                                       size_t persistent_arena_size,
                                       uint8_t* non_persistent_tensor_arena,
                                       size_t non_persistent_arena_size) {
  TFLITE_DCHECK(persistent_tensor_arena != nullptr);
  TFLITE_DCHECK(non_persistent_tensor_arena != nullptr);
  TFLITE_DCHECK(persistent_tensor_arena != non_persistent_tensor_arena);

  IPersistentBufferAllocator* persistent_buffer_allocator =
      CreatePersistentArenaAllocator(persistent_tensor_arena,
                                     persistent_arena_size);
  INonPersistentBufferAllocator* non_persistent_buffer_allocator =
      CreateNonPersistentArenaAllocator(non_persistent_tensor_arena,
                                        non_persistent_arena_size,
                                        persistent_buffer_allocator);

  uint8_t* memory_planner_buffer =
      persistent_buffer_allocator->AllocatePersistentBuffer(
          sizeof(GreedyMemoryPlanner), alignof(GreedyMemoryPlanner));
  GreedyMemoryPlanner* memory_planner =
      new (memory_planner_buffer) GreedyMemoryPlanner();

  uint8_t* micro_allocator_buffer =
      persistent_buffer_allocator->AllocatePersistentBuffer(
          sizeof(MicroAllocator), alignof(MicroAllocator));
  MicroAllocator* allocator = new (micro_allocator_buffer)
      MicroAllocator(persistent_buffer_allocator,
                     non_persistent_buffer_allocator, memory_planner);
  return allocator;
}

SubgraphAllocations* MicroAllocator::StartModelAllocation(const Model* model) {
  TFLITE_DCHECK(model != nullptr);

  if (model_is_allocating_) {
    MicroPrintf(
        "MicroAllocator: Model allocation started before "
        "finishing previously allocated model");
    return nullptr;
  }

  model_is_allocating_ = true;

  uint8_t* data_allocator_buffer =
      persistent_buffer_allocator_->AllocatePersistentBuffer(
          sizeof(MicroBuiltinDataAllocator),
          alignof(MicroBuiltinDataAllocator));
  builtin_data_allocator_ = new (data_allocator_buffer)
      MicroBuiltinDataAllocator(persistent_buffer_allocator_);

  if (InitScratchBufferData() != kTfLiteOk) {
    return nullptr;
  }

  // Allocate struct to store eval tensors, nodes and registrations.
  SubgraphAllocations* output = reinterpret_cast<SubgraphAllocations*>(
      persistent_buffer_allocator_->AllocatePersistentBuffer(
          sizeof(SubgraphAllocations) * model->subgraphs()->size(),
          alignof(SubgraphAllocations)));
  if (output == nullptr) {
    MicroPrintf("Failed to allocate memory for model metadata.");
    return nullptr;
  }

  if (AllocateTfLiteEvalTensors(model, output) != kTfLiteOk ||
      AllocateNodeAndRegistrations(model, output) != kTfLiteOk) {
    return nullptr;
  }
  return output;
}

TfLiteStatus MicroAllocator::FinishModelAllocation(
    const Model* model, SubgraphAllocations* subgraph_allocations,
    ScratchBufferHandle** scratch_buffer_handles) {
  if (!model_is_allocating_) {
    MicroPrintf(
        "MicroAllocator: Model allocation finished before "
        "starting allocating model");
    return kTfLiteError;
  }

  // Allocate scratch buffer metadata.
  TF_LITE_ENSURE_STATUS(AllocateScratchBufferHandles(
      scratch_buffer_handles, scratch_buffer_request_count_));

  // Allocate buffers for variable tensors.
  for (size_t subgraph_idx = 0; subgraph_idx < model->subgraphs()->size();
       subgraph_idx++) {
    const SubGraph* subgraph = model->subgraphs()->Get(subgraph_idx);
    TFLITE_DCHECK(subgraph != nullptr);
    TF_LITE_ENSURE_STATUS(AllocateVariables(
        subgraph, subgraph_allocations[subgraph_idx].tensors));
  }

  // Plan all subgraphs and scratch buffers together.
  TF_LITE_ENSURE_STATUS(CommitStaticMemoryPlan(model, subgraph_allocations,
                                               *scratch_buffer_handles));
  model_is_allocating_ = false;
  return kTfLiteOk;
}

void* MicroAllocator::AllocatePersistentBuffer(size_t bytes) {
  return persistent_buffer_allocator_->AllocatePersistentBuffer(
      bytes, MicroArenaBufferAlignment());
}

TfLiteStatus MicroAllocator::RequestScratchBufferInArena(size_t bytes,
                                                         int subgraph_idx,
                                                         int* buffer_idx) {
  // All scratch buffer requests are stored in the head section of the arena
  // when a model is in the prepare phase. First align a scratch buffer request
  // pointer to the start of the head:
  internal::ScratchBufferRequest* requests = GetScratchBufferRequests();

  // Count the number of requested scratch buffers for the current node:
  size_t current_node_request_count = 0;
  for (size_t i = 0; i < scratch_buffer_request_count_; ++i) {
    if (requests[i].node_idx == kUnassignedScratchBufferRequestIndex) {
      ++current_node_request_count;
    }
  }

  // First, ensure that the per-kernel request has not exceeded the limit:
  if (current_node_request_count >= kMaxScratchBuffersPerOp) {
    MicroPrintf("Scratch buffer request exeeds limit per operator (%d)",
                kMaxScratchBuffersPerOp);
    return kTfLiteError;
  }

  // Initialize and assign values for the request at the current index:
  internal::ScratchBufferRequest* current_request =
      &requests[scratch_buffer_request_count_];
  *current_request = {};
  // Assign -1 as a sentinel value that will be updated when the node finishes
  // allocating:
  current_request->bytes = bytes;
  current_request->node_idx = kUnassignedScratchBufferRequestIndex;
  current_request->subgraph_idx = subgraph_idx;

  // Assign the current request index to the out-param:
  *buffer_idx = scratch_buffer_request_count_;

  // Bump the request count to prepare for the next request:
  ++scratch_buffer_request_count_;
  return kTfLiteOk;
}

TfLiteStatus MicroAllocator::FinishPrepareNodeAllocations(int node_id) {
  // When a node has finished preparing, all temp allocations performed by the
  // kernel should be cleaned up:
  TF_LITE_ENSURE_STATUS(ResetTempAllocations());

  // Find and update any new scratch buffer requests for the current node:
  internal::ScratchBufferRequest* requests = GetScratchBufferRequests();

  for (size_t i = 0; i < scratch_buffer_request_count_; ++i) {
    // A request with a node_idx of -1 is a sentinel value used to indicate this
    // was a new request for the current node. The allocator finally knows the
    // node index at this point. Assign the value and update the list of new
    // requests so the head section can be adjusted to allow for the next kernel
    // to allocate at most kMaxScratchBuffersPerOp requests:
    if (requests[i].node_idx == kUnassignedScratchBufferRequestIndex) {
      requests[i].node_idx = node_id;
    }
  }

  // Ensure that the head is re-adjusted to allow for another at-most
  // kMaxScratchBuffersPerOp scratch buffer requests in the next operator:
  TF_LITE_ENSURE_STATUS(non_persistent_buffer_allocator_->ResizeBuffer(
      scratch_buffer_head_,
      sizeof(internal::ScratchBufferRequest) *
          (scratch_buffer_request_count_ + kMaxScratchBuffersPerOp),
      alignof(internal::ScratchBufferRequest)));

  return kTfLiteOk;
}

size_t MicroAllocator::used_bytes() const {
  return non_persistent_buffer_allocator_->GetNonPersistentUsedBytes() +
         persistent_buffer_allocator_->GetPersistentUsedBytes();
}

TfLiteStatus MicroAllocator::AllocateNodeAndRegistrations(
    const Model* model, SubgraphAllocations* subgraph_allocations) {
  TFLITE_DCHECK(subgraph_allocations != nullptr);

  for (size_t subgraph_idx = 0; subgraph_idx < model->subgraphs()->size();
       subgraph_idx++) {
    const SubGraph* subgraph = model->subgraphs()->Get(subgraph_idx);
    TFLITE_DCHECK(subgraph != nullptr);

    uint32_t operators_size = NumSubgraphOperators(subgraph);

    // Initialize NodeAndRegistrations for the subgraph.
    NodeAndRegistration* output = reinterpret_cast<NodeAndRegistration*>(
        persistent_buffer_allocator_->AllocatePersistentBuffer(
            sizeof(NodeAndRegistration) * operators_size,
            alignof(NodeAndRegistration)));
    if (output == nullptr) {
      MicroPrintf("Failed to allocate memory for node_and_registrations.");
      return kTfLiteError;
    }
    subgraph_allocations[subgraph_idx].node_and_registrations = output;
  }
  return kTfLiteOk;
}

TfLiteTensor* MicroAllocator::AllocatePersistentTfLiteTensor(
    const Model* model, const SubgraphAllocations* subgraph_allocations,
    int tensor_index, int subgraph_index) {
  const SubGraph* subgraph = model->subgraphs()->Get(subgraph_index);
  TFLITE_DCHECK(subgraph != nullptr);

  // This value is allocated from persistent arena space. It is guaranteed to be
  // around for the lifetime of the application.
  TfLiteTensor* tensor = AllocatePersistentTfLiteTensorInternal();

  // Populate any fields from the flatbuffer, since this TfLiteTensor struct is
  // allocated in the persistent section of the arena, ensure that additional
  // allocations also take place in that section of the arena.
  if (PopulateTfLiteTensorFromFlatbuffer(
          model, tensor, tensor_index, subgraph_index,
          /*allocate_temp=*/false) != kTfLiteOk) {
    MicroPrintf(
        "Failed to populate a persistent TfLiteTensor struct "
        "from flatbuffer data!");
    return nullptr;
  }

  if (subgraph_allocations != nullptr) {
    // Tensor buffers that are allocated at runtime (e.g. non-weight buffers)
    // and not located in the flatbuffer are stored on the pre-allocated list of
    // TfLiteEvalTensors structs. These structs are the source of truth, simply
    // point the corresponding buffer to the new TfLiteTensor data value.
    tensor->data.data =
        subgraph_allocations[subgraph_index].tensors[tensor_index].data.data;
    // TfLiteEvalTensor structs must also be the source of truth for the
    // TfLiteTensor dims.
    tensor->dims =
        subgraph_allocations[subgraph_index].tensors[tensor_index].dims;
  }
  return tensor;
}

void MicroAllocator::DeallocateTempTfLiteTensor(TfLiteTensor* tensor) {
  TFLITE_DCHECK(tensor != nullptr);

  if (tensor->quantization.type == kTfLiteAffineQuantization) {
    TFLITE_DCHECK(tensor->quantization.params != nullptr);
    TfLiteAffineQuantization* quantization =
        reinterpret_cast<TfLiteAffineQuantization*>(
            tensor->quantization.params);

    non_persistent_buffer_allocator_->DeallocateTemp(
        reinterpret_cast<uint8_t*>(quantization->zero_point));
    non_persistent_buffer_allocator_->DeallocateTemp(
        reinterpret_cast<uint8_t*>(quantization));
  }

  // Clear the data in case someone still access tensor arena by mistake
  tensor->quantization.type = kTfLiteNoQuantization;
  tensor->quantization.params = nullptr;
  tensor->data.data = nullptr;
  tensor->dims = nullptr;
  non_persistent_buffer_allocator_->DeallocateTemp(
      reinterpret_cast<uint8_t*>(tensor));
}

TfLiteTensor* MicroAllocator::AllocateTempTfLiteTensor(
    const Model* model, const SubgraphAllocations* subgraph_allocations,
    int tensor_index, int subgraph_index) {
  const SubGraph* subgraph = model->subgraphs()->Get(subgraph_index);
  TFLITE_DCHECK(subgraph != nullptr);

  // This value is allocated from temporary arena space. It is guaranteed to be
  // around for at least the scope of the calling function. Since this struct
  // allocation takes place in temp space, no need to own or cleanup.
  TfLiteTensor* tensor = reinterpret_cast<TfLiteTensor*>(
      non_persistent_buffer_allocator_->AllocateTemp(sizeof(TfLiteTensor),
                                                     alignof(TfLiteTensor)));

  // Populate any fields from the flatbuffer, since this TfLiteTensor struct is
  // allocated in the temp section of the arena, ensure that additional
  // allocations also take place in that section of the arena.
  if (PopulateTfLiteTensorFromFlatbuffer(model, tensor, tensor_index,
                                         subgraph_index,
                                         /*allocate_temp=*/true) != kTfLiteOk) {
    MicroPrintf(
        "Failed to populate a temp TfLiteTensor struct from flatbuffer data!");
    return nullptr;
  }

  if (subgraph_allocations != nullptr) {
    // Tensor buffers that are allocated at runtime (e.g. non-weight buffers)
    // and not located in the flatbuffer are stored on the pre-allocated list of
    // TfLiteEvalTensors structs. These structs are the source of truth, simply
    // point the corresponding buffer to the new TfLiteTensor data value.
    tensor->data.data =
        subgraph_allocations[subgraph_index].tensors[tensor_index].data.data;
    // TfLiteEvalTensor structs must also be the source of truth for the
    // TfLiteTensor dims.
    tensor->dims =
        subgraph_allocations[subgraph_index].tensors[tensor_index].dims;
  }
  return tensor;
}

TfLiteStatus MicroAllocator::ResetTempAllocations() {
  return non_persistent_buffer_allocator_->ResetTempAllocations();
}

bool MicroAllocator::IsAllTempDeallocated() {
  return non_persistent_buffer_allocator_->IsAllTempDeallocated();
}

TfLiteStatus MicroAllocator::AllocateTfLiteEvalTensors(
    const Model* model, SubgraphAllocations* subgraph_allocations) {
  TFLITE_DCHECK(subgraph_allocations != nullptr);

  for (size_t subgraph_idx = 0; subgraph_idx < model->subgraphs()->size();
       subgraph_idx++) {
    const SubGraph* subgraph = model->subgraphs()->Get(subgraph_idx);
    TFLITE_DCHECK(subgraph != nullptr);

    size_t alloc_count = subgraph->tensors()->size();
    TfLiteEvalTensor* tensors = reinterpret_cast<TfLiteEvalTensor*>(
        persistent_buffer_allocator_->AllocatePersistentBuffer(
            sizeof(TfLiteEvalTensor) * alloc_count, alignof(TfLiteEvalTensor)));
    if (tensors == nullptr) {
      MicroPrintf(
          "Failed to allocate memory for context->eval_tensors, "
          "%d bytes required",
          sizeof(TfLiteEvalTensor) * alloc_count);
      return kTfLiteError;
    }

    for (size_t i = 0; i < alloc_count; ++i) {
      TfLiteStatus status = internal::InitializeTfLiteEvalTensorFromFlatbuffer(
          *subgraph->tensors()->Get(i), model->buffers(), &tensors[i]);
      if (status != kTfLiteOk) {
        MicroPrintf("Failed to initialize tensor %d", i);
        return kTfLiteError;
      }
    }
    subgraph_allocations[subgraph_idx].tensors = tensors;
  }
  return kTfLiteOk;
}

TfLiteStatus MicroAllocator::AllocateVariables(const SubGraph* subgraph,
                                               TfLiteEvalTensor* eval_tensors) {
  for (size_t i = 0; i < subgraph->tensors()->size(); ++i) {
    auto* tensor = subgraph->tensors()->Get(i);
    if (tensor->is_variable()) {
      size_t buffer_size;
      TF_LITE_ENSURE_STATUS(
          TfLiteEvalTensorByteLength(&eval_tensors[i], &buffer_size));

      eval_tensors[i].data.data =
          persistent_buffer_allocator_->AllocatePersistentBuffer(
              buffer_size, MicroArenaBufferAlignment());

      if (eval_tensors[i].data.data == nullptr) {
        MicroPrintf("Failed to allocate variable tensor of size %d",
                    buffer_size);
        return kTfLiteError;
      }
    }
  }
  return kTfLiteOk;
}

TfLiteTensor* MicroAllocator::AllocatePersistentTfLiteTensorInternal() {
  return reinterpret_cast<TfLiteTensor*>(
      persistent_buffer_allocator_->AllocatePersistentBuffer(
          sizeof(TfLiteTensor), alignof(TfLiteTensor)));
}

TfLiteStatus MicroAllocator::PopulateTfLiteTensorFromFlatbuffer(
    const Model* model, TfLiteTensor* tensor, int tensor_index,
    int subgraph_idx, bool allocate_temp) {
  // TODO(b/162311891): This method serves as a stub to ensure quantized
  // allocations in the tail can be recorded. Once the interpreter has APIs for
  // accessing buffers on TfLiteEvalTensor this method can be dropped.
  return internal::InitializeTfLiteTensorFromFlatbuffer(
      persistent_buffer_allocator_, non_persistent_buffer_allocator_,
      allocate_temp,
      *model->subgraphs()->Get(subgraph_idx)->tensors()->Get(tensor_index),
      model->buffers(), tensor);
}

TfLiteStatus MicroAllocator::CommitStaticMemoryPlan(
    const Model* model, SubgraphAllocations* allocations,
    ScratchBufferHandle* scratch_buffer_handles) {
  size_t head_usage = 0;
  // Create static memory plan
  // 1. Calculate AllocationInfo to know the lifetime of each tensor/buffer.
  // 2. Add them into the planner (such as the GreedyMemoryPlanner).
  // 3. Static memory planning using the planner.
  // 4. Set tensor/buffer pointers based on the offsets from the previous step.
  //
  // Note that AllocationInfo is only needed for creating the plan. It will be
  // allocated from the temp section and cleaned up at the bottom of this
  // function.

  // Use the AllocationInfoBuilder class to help determine where buffers are
  // used in the subgraph.
  AllocationInfoBuilder builder(model, non_persistent_buffer_allocator_);
  TF_LITE_ENSURE_STATUS(
      builder.CreateAllocationInfo(scratch_buffer_request_count_));

  const int32_t* offline_planner_offsets = nullptr;
  TF_LITE_ENSURE_STATUS(
      builder.GetOfflinePlannedOffsets(&offline_planner_offsets));
  TF_LITE_ENSURE_STATUS(
      builder.InitializeAllocationInfo(offline_planner_offsets, allocations));

  internal::ScratchBufferRequest* scratch_buffer_requests =
      GetScratchBufferRequests();
  TF_LITE_ENSURE_STATUS(builder.MarkAllocationLifetimes(
      0, scratch_buffer_requests, scratch_buffer_handles, allocations));
  int allocation_info_count = builder.AllocationCount();
  AllocationInfo* allocation_info = builder.Finish();

  // Remaining arena size that memory planner can use for calculating offsets.
  size_t remaining_arena_size =
      non_persistent_buffer_allocator_->GetAvailableMemory(
          MicroArenaBufferAlignment());
  uint8_t* planner_arena = non_persistent_buffer_allocator_->AllocateTemp(
      remaining_arena_size, MicroArenaBufferAlignment());
  TF_LITE_ENSURE(tflite::GetMicroErrorReporter(), planner_arena != nullptr);
  memory_planner_->Init(planner_arena, remaining_arena_size);
  TF_LITE_ENSURE_STATUS(
      CreatePlan(memory_planner_, allocation_info, allocation_info_count));

  // Commit the plan.
  TF_LITE_ENSURE_STATUS(
      CommitPlan(memory_planner_,
                 non_persistent_buffer_allocator_->GetOverlayMemoryAddress(),
                 allocation_info, allocation_info_count));

  // Reset all temp allocations used above:
  builder.FreeAllocationInfo();
  non_persistent_buffer_allocator_->DeallocateTemp(planner_arena);
  TF_LITE_ENSURE_STATUS(
      non_persistent_buffer_allocator_->ResetTempAllocations());
  TF_LITE_ENSURE_STATUS(
      non_persistent_buffer_allocator_->DeallocateResizableBuffer(
          scratch_buffer_head_));

#ifdef TF_LITE_SHOW_MEMORY_USE
  memory_planner_->PrintMemoryPlan();
#endif
  head_usage = memory_planner_->GetMaximumMemorySize();

  // The head is used to store memory plans for one model at a time during the
  // model preparation stage, and is re-purposed to store scratch buffer handles
  // during model invocation. The head must be as large as the greater of the
  // largest model memory plan's size and the total space required for all
  // scratch buffer handles.
  if (max_head_buffer_usage_ < head_usage) {
    max_head_buffer_usage_ = head_usage;
  }

  // The head is used for storing scratch buffer allocations before finalizing a
  // memory plan in this function. Ensure that the head is set to the largest
  // memory plan sent through the allocator:
  TF_LITE_ENSURE_STATUS(
      non_persistent_buffer_allocator_->ReserveNonPersistentOverlayMemory(
          max_head_buffer_usage_, MicroArenaBufferAlignment()));
  return kTfLiteOk;
}

TfLiteStatus MicroAllocator::AllocateScratchBufferHandles(
    ScratchBufferHandle** scratch_buffer_handles, size_t handle_count) {
  TFLITE_DCHECK(scratch_buffer_handles != nullptr);

  if (scratch_buffer_request_count_ == 0) {
    // No scratch buffer requests were requested during model allocation.
    return kTfLiteOk;
  }

  // Allocate a consecutive block of memory store the scratch buffer handles.
  // This alignment ensures quick lookup during inference time for the model:
  *scratch_buffer_handles = reinterpret_cast<ScratchBufferHandle*>(
      persistent_buffer_allocator_->AllocatePersistentBuffer(
          sizeof(ScratchBufferHandle) * handle_count,
          alignof(ScratchBufferHandle)));

  return kTfLiteOk;
}

TfLiteStatus MicroAllocator::InitScratchBufferData() {
  // A model is preparing to allocate resources, ensure that scratch buffer
  // request counter is cleared:
  scratch_buffer_request_count_ = 0;

  // All requests will be stored in the head section. Each kernel is allowed at
  // most kMaxScratchBuffersPerOp requests. Adjust the head to reserve at most
  // that many requests to begin:
  scratch_buffer_head_ =
      non_persistent_buffer_allocator_->AllocateResizableBuffer(
          sizeof(internal::ScratchBufferRequest) * kMaxScratchBuffersPerOp,
          alignof(internal::ScratchBufferRequest));
  if (scratch_buffer_head_ == nullptr) {
    return kTfLiteError;
  }

  return kTfLiteOk;
}

internal::ScratchBufferRequest* MicroAllocator::GetScratchBufferRequests() {
  return reinterpret_cast<internal::ScratchBufferRequest*>(AlignPointerUp(
      scratch_buffer_head_, alignof(internal::ScratchBufferRequest)));
}

BuiltinDataAllocator* MicroAllocator::GetBuiltinDataAllocator() {
  return builtin_data_allocator_;
}

}  // namespace tflite
