/*
  tasmota_stubs.cpp - Stub implementations for missing libstdc++ symbols
  with the xtensa-esp-elf GCC 15.x toolchain (nano libstdc++ variant).

  On embedded targets there is no OS-level exception unwinding. These stubs
  replace the formatted-message exception helpers that GCC 15 references from
  std::vector::at() and similar bounds-checked accessors.  A bounds violation
  at runtime will trap/crash rather than throw, which is the same observable
  behaviour as with the previous toolchain.

  Copyright (C) 2024  Tasmota contributors
  SPDX-License-Identifier: Apache-2.0
*/

#ifdef ESP8266

#include <cstdarg>
#include <cstdio>

// Provide a stub for __throw_out_of_range_fmt which is referenced by
// std::vector::at() in GCC 15.x nano libstdc++ but not present in the
// ESP8266 nano library archive.
namespace std {
  void __throw_out_of_range_fmt(const char*, ...) __attribute__((__noreturn__));
  void __throw_out_of_range_fmt(const char* fmt, ...) {
    // On embedded targets we cannot throw; just trap.
    __builtin_trap();
  }
}

#endif  // ESP8266
