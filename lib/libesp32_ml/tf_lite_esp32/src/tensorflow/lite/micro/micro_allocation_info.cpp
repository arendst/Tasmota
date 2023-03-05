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
#include "tensorflow/lite/micro/micro_allocation_info.h"

#include "tensorflow/lite/c/c_api_types.h"
#include "tensorflow/lite/kernels/internal/compatibility.h"
#include "tensorflow/lite/kernels/kernel_util.h"
#include "tensorflow/lite/micro/memory_helpers.h"
#include "tensorflow/lite/micro/memory_planner/greedy_memory_planner.h"
#include "tensorflow/lite/micro/micro_log.h"

namespace tflite {

namespace {
constexpr char kOfflineMemAllocMetadata[] = "OfflineMemoryAllocation";
constexpr int kUninitializedLifetime = -1;
}  // namespace

// Mark the given Allocation info as first created at the specified allocation
// scope count. Only the first creation must be recorded since the allocation
// scope count monotonically increases throughout the lifetime marking process.
void AllocationInfoBuilder::UpdateFirstCreated(AllocationInfo* current,
                                               int allocation_scope_count) {
  TFLITE_DCHECK(current->first_created <= allocation_scope_count);
  if (current->first_created == kUninitializedLifetime) {
    current->first_created = allocation_scope_count;
  }
}

// Mark the given AllocationInfo as last used at the specified allocation scope
// count. Update the last used marker every time, since the allocation scope
// count monotonically increases through the lifetime marking process.
void AllocationInfoBuilder::UpdateLastUsed(AllocationInfo* current,
                                           int allocation_scope_count) {
  TFLITE_DCHECK(current->last_used <= allocation_scope_count);
  current->last_used = allocation_scope_count;
}

TfLiteStatus AllocationInfoBuilder::MarkSubgraphLifetimesIfNecessary(
    const Operator* op, internal::ScratchBufferRequest* scratch_buffer_requests,
    ScratchBufferHandle* scratch_buffer_handles,
    SubgraphAllocations* allocations) {
  int first_subgraph_index = -1;
  int second_subgraph_index = -1;
  const OperatorCode* opcode =
      model_->operator_codes()->Get(op->opcode_index());
  switch (opcode->builtin_code()) {
    case BuiltinOperator_IF: {
      first_subgraph_index =
          op->builtin_options_as_IfOptions()->then_subgraph_index();
      second_subgraph_index =
          op->builtin_options_as_IfOptions()->else_subgraph_index();
      break;
    }
    case BuiltinOperator_CALL_ONCE: {
      first_subgraph_index =
          op->builtin_options_as_CallOnceOptions()->init_subgraph_index();
      break;
    }
    case BuiltinOperator_WHILE: {
      first_subgraph_index =
          op->builtin_options_as_WhileOptions()->cond_subgraph_index();
      second_subgraph_index =
          op->builtin_options_as_WhileOptions()->body_subgraph_index();
      break;
    }
    default: {
      break;
    }
  }
  if (first_subgraph_index != -1) {
    // Enter a new allocation scope for each subgraph.
    allocation_scope_count_++;
    TF_LITE_ENSURE_STATUS(
        MarkAllocationLifetimes(first_subgraph_index, scratch_buffer_requests,
                                scratch_buffer_handles, allocations));
  }
  if (second_subgraph_index != -1) {
    // Enter a new allocation scope for each subgraph.
    allocation_scope_count_++;
    TF_LITE_ENSURE_STATUS(
        MarkAllocationLifetimes(second_subgraph_index, scratch_buffer_requests,
                                scratch_buffer_handles, allocations));
  }
  return kTfLiteOk;
}

TfLiteStatus AllocationInfoBuilder::CreateAllocationInfo(
    int scratch_buffer_request_count) {
  size_t subgraph_offsets_length = model_->subgraphs()->size() * sizeof(size_t);
  info_.subgraph_offsets =
      reinterpret_cast<size_t*>(non_persistent_allocator_->AllocateTemp(
          subgraph_offsets_length, alignof(size_t)));
  if (info_.subgraph_offsets == nullptr) {
    MicroPrintf(
        "Failed to allocate memory for memory planning, %d bytes required",
        subgraph_offsets_length);
    return kTfLiteError;
  }
  size_t tensor_count = 0;
  for (size_t subgraph_idx = 0; subgraph_idx < model_->subgraphs()->size();
       subgraph_idx++) {
    // Add all tensors in each subgraph to the AllocationInfo array. Even weight
    // tensors are added but marked with needs_allocating = false. Including all
    // tensors in the graph here simplifies logic.
    info_.subgraph_offsets[subgraph_idx] = tensor_count;
    tensor_count += model_->subgraphs()->Get(subgraph_idx)->tensors()->size();
  }
  info_.tensor_count = tensor_count;

  // Scratch buffer allocations follow tensor allocations, so the scratch offset
  // is equal to the number of tensor allocations.
  info_.scratch_offset = tensor_count;
  info_.allocation_info_count = tensor_count + scratch_buffer_request_count;
  info_.scratch_buffer_count = scratch_buffer_request_count;
  size_t bytes = sizeof(AllocationInfo) * info_.allocation_info_count;

  // Allocate an array of AllocationInfo structs from the temp section. This
  // struct will be used by AllocationInfoBuilder to find buffer usage.
  info_.allocation_info = reinterpret_cast<AllocationInfo*>(
      non_persistent_allocator_->AllocateTemp(bytes, alignof(AllocationInfo)));
  if (info_.allocation_info == nullptr) {
    MicroPrintf(
        "Failed to allocate memory for memory planning, %d bytes required",
        bytes);
    return kTfLiteError;
  }
  return kTfLiteOk;
}

TfLiteStatus AllocationInfoBuilder::FreeAllocationInfo() {
  non_persistent_allocator_->DeallocateTemp(
      reinterpret_cast<uint8_t*>(info_.allocation_info));
  non_persistent_allocator_->DeallocateTemp(
      reinterpret_cast<uint8_t*>(info_.subgraph_offsets));
  return kTfLiteOk;
}

TfLiteStatus AllocationInfoBuilder::ValidateSubgraph(
    const SubGraph* subgraph, TfLiteEvalTensor* eval_tensors) {
  uint32_t operators_size = NumSubgraphOperators(subgraph);

  for (uint32_t i = 0; i < operators_size; i++) {
    const auto op = subgraph->operators()->Get(i);
    for (size_t n = 0;
         op->intermediates() != nullptr && n < op->intermediates()->size();
         n++) {
      const int tensor_index = op->intermediates()->Get(n);
      size_t tensor_size = -1;
      TF_LITE_ENSURE_STATUS(TfLiteEvalTensorByteLength(
          &eval_tensors[tensor_index], &tensor_size));
      if (tensor_size != 0) {
        MicroPrintf(
            "Does not support intermediate tensor with non-zero size: %d",
            tensor_size);
        return kTfLiteError;
      }
    }
  }
  return kTfLiteOk;
}

TfLiteStatus AllocationInfoBuilder::InitializeAllocationInfo(
    const int32_t* offline_offsets, SubgraphAllocations* allocations) {
  AllocationInfo* allocation_info = info_.allocation_info;
  // Initialize allocation info for every tensor in every subgraph.
  for (size_t subgraph_idx = 0; subgraph_idx < model_->subgraphs()->size();
       subgraph_idx++) {
    const SubGraph* subgraph = model_->subgraphs()->Get(subgraph_idx);
    TfLiteEvalTensor* eval_tensors = allocations[subgraph_idx].tensors;
    AllocationInfo* subgraph_allocation_info =
        &allocation_info[info_.subgraph_offsets[subgraph_idx]];

    // Ensure constraints are met.
    TF_LITE_ENSURE_STATUS(ValidateSubgraph(subgraph, eval_tensors));

    for (size_t i = 0; i < subgraph->tensors()->size(); ++i) {
      AllocationInfo* current = &subgraph_allocation_info[i];
      current->output_ptr = &(eval_tensors[i].data.data);

      TF_LITE_ENSURE_STATUS(
          TfLiteEvalTensorByteLength(&eval_tensors[i], &current->bytes));

      current->first_created = kUninitializedLifetime;
      current->last_used = kUninitializedLifetime;
      current->needs_allocating =
          (eval_tensors[i].data.data == nullptr) &&
          (!subgraph->tensors()->Get(i)->is_variable()) &&
          (current->bytes != 0);
      if (offline_offsets) {
        current->offline_offset = offline_offsets[i];
      } else {
        current->offline_offset = kOnlinePlannedBuffer;
      }
    }
  }
  // Initialize allocation info for every scratch buffer.
  AllocationInfo* scratch_allocation_info =
      &allocation_info[info_.scratch_offset];
  for (size_t i = 0; i < info_.scratch_buffer_count; i++) {
    AllocationInfo* current = &scratch_allocation_info[i];
    current->first_created = kUninitializedLifetime;
    current->last_used = kUninitializedLifetime;
    current->needs_allocating = true;
    current->offline_offset = kOnlinePlannedBuffer;
  }
  return kTfLiteOk;
}

TfLiteStatus AllocationInfoBuilder::MarkAllocationLifetimes(
    int subgraph_idx, internal::ScratchBufferRequest* scratch_buffer_requests,
    ScratchBufferHandle* scratch_buffer_handles,
    SubgraphAllocations* allocations) {
  const SubGraph* subgraph = model_->subgraphs()->Get(subgraph_idx);

  AllocationInfo* allocation_info = info_.allocation_info;
  // Each subgraph's tensor allocations are in a contiguous block starting at
  // subgraph_offsets_[subgraph index] with one entry per tensor.
  AllocationInfo* subgraph_allocation_info =
      &allocation_info[info_.subgraph_offsets[subgraph_idx]];

  uint32_t operators_size = NumSubgraphOperators(subgraph);
  // Mark all inputs as created at the start of the subgraph invocation.
  for (size_t i = 0;
       subgraph->inputs() != nullptr && i < subgraph->inputs()->size(); ++i) {
    const int tensor_index = subgraph->inputs()->Get(i);
    AllocationInfo* current = &subgraph_allocation_info[tensor_index];
    UpdateFirstCreated(current, allocation_scope_count_);
  }

  for (uint32_t i = 0; i < operators_size; i++) {
    // Each operator has a new allocation scope.
    allocation_scope_count_++;
    const auto* op = subgraph->operators()->Get(i);
    // Figure out when the first creation and use of each tensor is.
    for (size_t n = 0; op->outputs() != nullptr && n < op->outputs()->size();
         ++n) {
      const int tensor_index = op->outputs()->Get(n);
      AllocationInfo* current = &subgraph_allocation_info[tensor_index];
      UpdateFirstCreated(current, allocation_scope_count_);
    }

    // Keep track of scope count before any subgraphs, so that scratch buffers'
    // lifetime within a control flow op properly overlaps with all subgraphs.
    int start_allocation_scope_count = allocation_scope_count_;

    // Control flow operators can invoke subgraphs. Plan these subgraphs
    // before continuing on to the rest of the graph.
    MarkSubgraphLifetimesIfNecessary(op, scratch_buffer_requests,
                                     scratch_buffer_handles, allocations);

    // Figure out when the last use of each tensor is.
    for (size_t n = 0; op->inputs() != nullptr && n < op->inputs()->size();
         ++n) {
      const int tensor_index = op->inputs()->Get(n);
      // Optional bias tensors can have an index of -1 when they are omitted.
      if (tensor_index >= 0) {
        AllocationInfo* current = &subgraph_allocation_info[tensor_index];
        // No need to update creation since it is either marked by the subgraph
        // or producer op, or it is not part of the memory plan (weight, bias
        // tensor).
        UpdateLastUsed(current, allocation_scope_count_);
      }
    }
    for (size_t n = 0; op->outputs() != nullptr && n < op->outputs()->size();
         ++n) {
      const int tensor_index = op->outputs()->Get(n);
      AllocationInfo* current = &subgraph_allocation_info[tensor_index];
      UpdateLastUsed(current, allocation_scope_count_);
    }

    // Mark thse lifetime of scratch buffers belonging to the current node. This
    // operation is O(N * M) where N is the total number of visited nodes and M
    // is the total number of scratch buffers.
    // TODO(b/217794030): Optimize this memory planning code.
    AllocationInfo* scratch_allocation_info =
        &allocation_info[info_.scratch_offset];
    for (size_t scratch_idx = 0; scratch_idx < info_.scratch_buffer_count;
         scratch_idx++) {
      internal::ScratchBufferRequest request =
          scratch_buffer_requests[scratch_idx];
      AllocationInfo* current = &scratch_allocation_info[scratch_idx];
      if (request.node_idx == static_cast<int>(i) &&
          request.subgraph_idx == static_cast<int>(subgraph_idx)) {
        ScratchBufferHandle* current_handle =
            &(scratch_buffer_handles[scratch_idx]);
        current->output_ptr = reinterpret_cast<void**>(&current_handle->data);
        current->bytes = request.bytes;
        UpdateFirstCreated(current, start_allocation_scope_count);
        UpdateLastUsed(current, allocation_scope_count_);
      }
    }
  }

  // Mark all outputs as persistent to the end of the subgraph invocation.
  for (size_t i = 0;
       subgraph->outputs() != nullptr && i < subgraph->outputs()->size(); ++i) {
    const int tensor_index = subgraph->outputs()->Get(i);
    AllocationInfo* current = &subgraph_allocation_info[tensor_index];
    UpdateLastUsed(current, allocation_scope_count_);
  }
  return kTfLiteOk;
}

// Get offline tensors allocation plan. See
// micro/docs/memory_management.md for more info.
TfLiteStatus AllocationInfoBuilder::GetOfflinePlannedOffsets(
    const int32_t** offline_planner_offsets) {
  if (model_->metadata()) {
    for (size_t i = 0; i < model_->metadata()->size(); ++i) {
      auto metadata = model_->metadata()->Get(i);
      const size_t metadata_name_size = (size_t)metadata->name()->size();

      if ((strncmp(metadata->name()->c_str(), kOfflineMemAllocMetadata,
                   std::min(metadata_name_size,
                            strlen(kOfflineMemAllocMetadata))) == 0) &&
          metadata_name_size == strlen(kOfflineMemAllocMetadata)) {
        const flatbuffers::Vector<flatbuffers::Offset<Buffer>>* buffers =
            model_->buffers();
        auto* buffer = (*buffers)[metadata->buffer()];
        auto* array = buffer->data();
        const uint32_t* metadata_buffer =
            reinterpret_cast<const uint32_t*>(array->data());
        const size_t nbr_tensors = static_cast<size_t>(metadata_buffer[2]);
        *offline_planner_offsets =
            reinterpret_cast<const int32_t*>(&metadata_buffer[3]);

        if (info_.tensor_count != nbr_tensors) {
          MicroPrintf(
              "Nbr of offline buffer offsets (%d) in metadata "
              "not equal nbr tensors (%d)\n",
              nbr_tensors, info_.tensor_count);
          return kTfLiteError;
        }
      }
    }
  }
  return kTfLiteOk;
}

}  // namespace tflite
