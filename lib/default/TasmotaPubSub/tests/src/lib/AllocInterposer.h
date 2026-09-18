/*
  AllocInterposer.h - Allocation-failure injection for the TasmotaPubSub host
  test system (support for F-07 / Requirement 13.2).

  The *unmodified* library allocates its working buffer with malloc()/realloc()
  in setBufferSize(). To exercise the allocation-failure path deterministically
  on the host, the library translation unit is compiled with the malloc/realloc
  calls routed through the C-linkage wrappers declared below (see
  src/lib/AllocShim.h, force-included ONLY when compiling PubSubClient.cpp - so
  no other translation unit, the shim, doctest, or the tests are affected).

  Tests arm the next allocation(s) to fail via the AllocInterposer API. When
  disarmed (the default), every wrapper call passes straight through to the real
  allocator, so behavior is identical to a normal build. When armed, the wrapper
  returns nullptr WITHOUT calling the real allocator - modelling malloc()/
  realloc() returning NULL. Because realloc() leaving its original block intact
  on failure is the real contract, an armed realloc wrapper also leaves the
  caller's pointer untouched (it simply never calls realloc()).

  This is intentionally single-threaded (the test binary is single-threaded) and
  keeps counters so tests can assert how many allocations the library performed.
*/

#ifndef TASMOTA_PUBSUB_TEST_ALLOC_INTERPOSER_H
#define TASMOTA_PUBSUB_TEST_ALLOC_INTERPOSER_H

#include <cstddef>

namespace AllocInterposer {

// Arm exactly the next routed allocation (malloc or realloc) to fail.
void failNextAllocation();

// Arm the next `n` routed allocations to fail (n == 0 disarms).
void failNextAllocations(unsigned n);

// Disarm any pending failures and reset the malloc/realloc call counters. A
// test should call this before arming and again once the failure has been
// exercised, so later tests in the same process are never affected.
void reset();

// How many armed failures are still pending (0 when disarmed).
unsigned pendingFailures();

// Total number of routed malloc()/realloc() calls seen since the last reset().
unsigned mallocCount();
unsigned reallocCount();

}  // namespace AllocInterposer

// C-linkage wrappers the library TU's malloc()/realloc() calls are routed to
// (via the macros in AllocShim.h). Declared here so both the library shim and
// the interposer implementation agree on the signatures.
extern "C" void* tpubsub_test_malloc(size_t size);
extern "C" void* tpubsub_test_realloc(void* ptr, size_t size);

#endif  // TASMOTA_PUBSUB_TEST_ALLOC_INTERPOSER_H
