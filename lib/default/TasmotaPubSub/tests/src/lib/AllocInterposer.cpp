/*
  AllocInterposer.cpp - Implementation of the allocation-failure injection used
  by the F-07 hardening test (Requirement 13.2).

  This translation unit is compiled with the NORMAL build rule (no AllocShim.h
  force-include), so `malloc`/`realloc` below are the real C-library allocators.
  Only the library TU (PubSubClient.cpp) has its malloc/realloc macro-routed to
  the tpubsub_test_* wrappers defined here.
*/

#include "AllocInterposer.h"

#include <cstdlib>

namespace {

// Number of upcoming routed allocations still armed to fail. Single-threaded.
unsigned g_failCountdown = 0;
unsigned g_mallocCount = 0;
unsigned g_reallocCount = 0;

}  // namespace

namespace AllocInterposer {

void failNextAllocation() {
    g_failCountdown = 1;
}

void failNextAllocations(unsigned n) {
    g_failCountdown = n;
}

void reset() {
    g_failCountdown = 0;
    g_mallocCount = 0;
    g_reallocCount = 0;
}

unsigned pendingFailures() {
    return g_failCountdown;
}

unsigned mallocCount() {
    return g_mallocCount;
}

unsigned reallocCount() {
    return g_reallocCount;
}

}  // namespace AllocInterposer

extern "C" void* tpubsub_test_malloc(size_t size) {
    ++g_mallocCount;
    if (g_failCountdown > 0) {
        --g_failCountdown;
        return nullptr;  // model malloc() returning NULL
    }
    return malloc(size);
}

extern "C" void* tpubsub_test_realloc(void* ptr, size_t size) {
    ++g_reallocCount;
    if (g_failCountdown > 0) {
        --g_failCountdown;
        // Model realloc() failing: return NULL and leave the original block
        // (ptr) untouched, exactly as the C contract specifies.
        return nullptr;
    }
    return realloc(ptr, size);
}
