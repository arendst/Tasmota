// FindingStatus.h — Expected-fail registry for the TasmotaPubSub Hardening_Suite.
//
// Feature: tasmota-pubsub-tests
//
// This header maps each static-analysis finding (F-01..F-12, plus the two
// distinct aspects of F-03) to a single doctest test-case decorator. Hardening
// test cases reference a finding via FINDING_MARKER(Fxx), e.g.:
//
//     TEST_CASE("F-05 partial write disables reuse" * FINDING_MARKER(F05)) { ... }
//
// A finding that is still OPEN (the library is NOT yet hardened for it) expands
// to `doctest::should_fail()`, so the run reports an *expected* failure and does
// NOT set a nonzero exit status for that failure alone (Requirement 4.6). If the
// case unexpectedly PASSES, doctest reports an unexpected pass and fails the run
// (Requirement 4.7) — the signal to flip the finding to hardened here.
//
// A finding that is already HARDENED (expected to pass) expands to a no-op
// decorator: `doctest::skip(false)` — a decorator that does NOT skip the case,
// so the test always runs and is expected to pass. We deliberately use
// `should_fail()` (never `may_fail()`, which would hide an unexpected pass).
//
// Flipping a finding from open to hardened is a one-line edit: change its
// FINDING_MARKER_* definition below from FINDING_OPEN to FINDING_HARDENED.
//
// The finding-status seeding below is the starting hypothesis derived from the
// design's finding-status table; task 12.1 confirms each empirically and
// finalizes these markers.

#ifndef TASMOTA_PUBSUB_TESTS_FINDING_STATUS_H
#define TASMOTA_PUBSUB_TESTS_FINDING_STATUS_H

#include "doctest.h"

// Semantic aliases for the two decorator states.
//   FINDING_OPEN      -> expected-FAIL (library not yet hardened for this finding)
//   FINDING_HARDENED  -> expected-PASS (no-op decorator that always runs the case)
#define FINDING_OPEN doctest::should_fail()
#define FINDING_HARDENED doctest::skip(false)

// Indirection so FINDING_MARKER(F05) resolves to the per-finding status macro.
#define FINDING_MARKER(id) FINDING_MARKER_##id

// ---------------------------------------------------------------------------
// Finding status seeding (starting hypothesis; finalized empirically in 12.1).
// ---------------------------------------------------------------------------

// Expected-FAIL (open findings).
#define FINDING_MARKER_F05 FINDING_OPEN          // partial-write reuse
#define FINDING_MARKER_F10 FINDING_OPEN          // SUBACK / session-present tracking
#define FINDING_MARKER_F11 FINDING_OPEN          // graceful disconnect default
#define FINDING_MARKER_F03_DEADLINE FINDING_OPEN // F-03 packet-wide read deadline

// Expected-PASS (already hardened).
#define FINDING_MARKER_F01 FINDING_HARDENED           // exact-buffer inbound overflow
#define FINDING_MARKER_F02 FINDING_HARDENED           // PUBLISH structural checks
#define FINDING_MARKER_F04 FINDING_HARDENED           // streaming length 16-bit truncation
#define FINDING_MARKER_F06 FINDING_HARDENED           // CONNACK validation
#define FINDING_MARKER_F07 FINDING_HARDENED           // alloc-failure buffer state
#define FINDING_MARKER_F08 FINDING_HARDENED           // SUBSCRIBE off-by-one + null checks
#define FINDING_MARKER_F09 FINDING_HARDENED           // QoS / fixed-header validation
#define FINDING_MARKER_F03_PROMPT_CLOSE FINDING_HARDENED // F-03 oversized prompt-close

#endif // TASMOTA_PUBSUB_TESTS_FINDING_STATUS_H
