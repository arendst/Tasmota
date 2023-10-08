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

#ifndef TENSORFLOW_LITE_MICRO_MICRO_PROFILER_INTERFACE_H_
#define TENSORFLOW_LITE_MICRO_MICRO_PROFILER_INTERFACE_H_

#include <cstdint>

namespace tflite {

// Interface class that the TFLM framework relies on for profiling.
class MicroProfilerInterface {
 public:
  virtual ~MicroProfilerInterface() {}

  // Marks the start of a new event and returns an event handle that can be used
  // to mark the end of the event via EndEvent.
  virtual uint32_t BeginEvent(const char* tag) = 0;

  // Marks the end of an event associated with event_handle.
  virtual void EndEvent(uint32_t event_handle) = 0;
};

}  // namespace tflite

#endif  // TENSORFLOW_LITE_MICRO_MICRO_PROFILER_INTERFACE_H_
