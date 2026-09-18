/*
  AllocShim.h - malloc/realloc routing header, force-included ONLY when compiling
  the library under test (PubSubClient.cpp). See tests/Makefile ($(LIB_OBJ)).

  Purpose: route the *unmodified* library's malloc()/realloc() calls through the
  test interposer (AllocInterposer) so the F-07 allocation-failure path can be
  exercised deterministically (Requirement 13.2), WITHOUT modifying the library
  source itself.

  Safety: every standard header the library pulls in is included HERE, before the
  macros are defined. That way the function-like `malloc(`/`realloc(` macros
  only ever rewrite the library's OWN call sites and can never corrupt a
  declaration inside <cstdlib>, <functional>, <string>, etc. (those are all
  processed while the macros are still undefined, and their include guards make
  the library's later #includes no-ops). The shim headers the library includes
  (Arduino.h / Client.h / Stream.h / IPAddress.h) contain no malloc/realloc call
  sites, so they are unaffected.

  Scope: because this header is force-included ONLY for the library object, no
  other translation unit (the shim .cpp files, doctest, or the *_test.cpp files)
  sees these macros. AllocInterposer.cpp therefore calls the real allocators.
*/

#ifndef TASMOTA_PUBSUB_TEST_ALLOC_SHIM_H
#define TASMOTA_PUBSUB_TEST_ALLOC_SHIM_H

// Lock in every standard header the library TU depends on BEFORE defining the
// macros, so no system declaration of malloc/realloc is ever rewritten.
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <functional>
#include <string>

#include "AllocInterposer.h"

// Route the library's own malloc()/realloc() call sites to the interposer.
// free() is intentionally left as the real deallocator: the wrappers return
// blocks obtained from the real malloc()/realloc(), so plain free() releases
// them correctly (including the destructor's free(this->buffer)).
#define malloc(sz)      tpubsub_test_malloc((sz))
#define realloc(p, sz)  tpubsub_test_realloc((p), (sz))

#endif  // TASMOTA_PUBSUB_TEST_ALLOC_SHIM_H
