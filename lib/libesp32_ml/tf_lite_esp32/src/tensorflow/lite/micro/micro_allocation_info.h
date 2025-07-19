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
#ifndef TENSORFLOW_LITE_MICRO_MICRO_ALLOCATION_INFO_H_
#define TENSORFLOW_LITE_MICRO_MICRO_ALLOCATION_INFO_H_

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/core/api/flatbuffer_conversions.h"
#include "tensorflow/lite/micro/compatibility.h"
#include "tensorflow/lite/micro/flatbuffer_utils.h"
#include "tensorflow/lite/micro/micro_allocator.h"
#include "tensorflow/lite/schema/schema_generated.h"

namespace tflite {

// Used to hold information used during allocation calculations.
struct AllocationInfo {
  size_t bytes;
  void** output_ptr;
  int first_created;
  int last_used;
  int32_t offline_offset;
  bool needs_allocating;
};

// Used to hold the allocation info list and related metadata for the entire
// graph (including subgraphs). Since all subgraphs are planned together, the
// allocation info list contains allocations for all subgraphs. Track the offset
// into this list for each subgraph then reserve space to track all allocations.
//
// The AllocationInfo list is a contiguous list of allocations across all
// subgraphs and scratch buffers. Each element here is marked as
// s<subgraph index>t<tensor index>. The following is a possible
// AllocationInfo list:
// [s0t0, s0t1, s1t0, s2t1, s1t2, s3t0, s3t1, scratch0, scratch1, scratch2]
//
// For this example, the subgraph offsets would be [0, 2, 5] and the scratch
// offset would be 7.
struct GraphAllocationInfo {
  AllocationInfo* allocation_info;
  size_t allocation_info_count;
  size_t* subgraph_offsets;
  size_t scratch_offset;
  size_t tensor_count;
  size_t scratch_buffer_count;
};

// A helper class to construct AllocationInfo array. This array contains the
// lifetime of tensors / scratch_buffer and will be used to calculate the memory
// plan. Methods need to be called in order from `Create`, Init`, `Add*`, to
// `Finish`.
class AllocationInfoBuilder {
 public:
  AllocationInfoBuilder(const Model* model,
                        INonPersistentBufferAllocator* non_persistent_allocator)
      : model_(model), non_persistent_allocator_(non_persistent_allocator) {}

  // Check if model contains offline planned buffer offsets.
  //  - If there's no metadata available, offline_planner_offsets is not set
  //  - If there's metadata available, offline_planner_offsets will point to the
  //    first offset in the metadata buffer list.
  TfLiteStatus GetOfflinePlannedOffsets(
      const int32_t** offline_planner_offsets);

  // Allocate memory for the allocation info array as well as offsets into that
  // array for each subgraph.
  TfLiteStatus CreateAllocationInfo(int scratch_buffer_request_count);

  // Release memory used for the allocation info array.
  TfLiteStatus FreeAllocationInfo();

  // Initialize AllocationInfo for all tensors and scratch buffers in the graph.
  TfLiteStatus InitializeAllocationInfo(const int32_t* offline_offsets,
                                        SubgraphAllocations* allocations);

  // Mark the scope of each tensor and scratch buffer across the graph. Enter
  // all possible subgraphs invoked by each control flow operator. This method
  // marks the maximum lifetime of each buffer so that tensors are correctly
  // planned for all valid invocation flows.
  TfLiteStatus MarkAllocationLifetimes(
      int subgraph_idx, internal::ScratchBufferRequest* scratch_buffer_request,
      ScratchBufferHandle* scratch_buffer_handles,
      SubgraphAllocations* allocations);

  // Identify control flow operators and recursively mark all subgraphs which
  // that operator can invoke. The lifetime of all tensors within a subgraph
  // can only be extended. The order of subgraph invocation does not matter
  // since subgraphs within the same control flow operator are executed
  // within their own allocation scope (planned buffers in a subgraph cannot
  // persist beyond the end of that subgraph's invocation).
  TfLiteStatus MarkSubgraphLifetimesIfNecessary(
      const Operator* op,
      internal::ScratchBufferRequest* scratch_buffer_requests,
      ScratchBufferHandle* scratch_buffer_handles,
      SubgraphAllocations* allocations);

  // Returns the number of allocations.
  int AllocationCount() const { return info_.allocation_info_count; }

  // Returns a pointer to the built AllocationInfo array.
  AllocationInfo* Finish() const { return info_.allocation_info; }

 private:
  // Mark the given Allocation info as first created at the specified allocation
  // scope count. Only the first creation must be recorded since the allocation
  // scope count monotonically increases throughout the lifetime marking
  // process.
  void UpdateFirstCreated(AllocationInfo* current, int allocation_scope_count);

  // Mark the given AllocationInfo as last used at the specified allocation
  // scope
  // count. Update the last used marker every time, since the allocation scope
  // count monotonically increases through the lifetime marking process.
  void UpdateLastUsed(AllocationInfo* current, int allocation_scope_count);

  // Validate if a subgraph satisfies assumptions.
  TfLiteStatus ValidateSubgraph(const SubGraph* subgraph,
                                TfLiteEvalTensor* eval_tensors);

  const tflite::Model* model_ = nullptr;
  INonPersistentBufferAllocator* non_persistent_allocator_ = nullptr;
  GraphAllocationInfo info_;
  int allocation_scope_count_ = 0;
};

}  // namespace tflite

#endif  // TENSORFLOW_LITE_MICRO_MICRO_ALLOCATION_INFO_H_
