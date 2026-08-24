/*
  Stream.cpp - Host-side Arduino `Stream` shim implementation.

  Most of `Stream` is pure-virtual/inline; only the non-pure `flush()` needs an
  out-of-line definition. Kept present because the design lists Stream.h/.cpp.
*/

#include "Stream.h"

void Stream::flush() {
    // No-op default, matching the harmless Arduino base behavior.
}
