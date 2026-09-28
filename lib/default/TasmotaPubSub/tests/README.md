# TasmotaPubSub Host Test System

A host-based (no ESP32/ESP8266 hardware required) unit-test system for the
TasmotaPubSub `PubSubClient` MQTT library. It compiles both guarded implementation
sources (`../src/PubSubClient_311.cpp` and `../src/PubSubClient_5.cpp`) against a
small host-side Arduino environment shim and produces one binary per protocol profile.
The MQTT 3.1.1 binary validates the restored compact implementation; the MQTT 5 binary
validates the extended profile with the
[doctest](https://github.com/doctest/doctest) single-header framework.

Assertions use public APIs and recorded/scripted wire bytes from the mock transport.
Profile-specific tests are compiled only where their public types and APIs exist.

## Layout

```
tests/
  Makefile              # dual-profile build + run targets
  README.md             # this file
  src/
    test_main.cpp       # the one TU that defines doctest's main()
    *_test.cpp          # test suites (baseline + hardening cases)
    lib/                # Arduino shim + test support library
      doctest.h         # vendored single-header framework
      Arduino.h ...     # host Arduino environment shim (Client/Stream/String/...)
      TestClock.*       # virtual millis()/delay() clock
      MockClient.*      # scriptable mock transport (inbound/outbound, faults)
      MockStream.*      # stream-mode mock
      MqttPacket.*      # MQTT packet builder + parser
      CallbackContractAdapter.*  # Tasmota MqttDataHandler NUL-write contract
      AllocShim.h       # malloc/realloc interposer (F-07 alloc-failure tests)
      FindingStatus.h   # expected-fail registry (see below)
  build/                # generated; git-ignored
```

## Requirements

- A C++17 host compiler: Apple `clang`/`clang++` or GNU `g++`. The Makefile
  defaults to `CXX = c++`.
- `make`.

No third-party dependencies: doctest is vendored at `src/lib/doctest.h`.

## Building and running

Run everything from the `tests/` directory.

```sh
make            # build both protocol binaries
make test       # run the applicable suites for both profiles
make check      # authoritative selector, source, baseline, and MQTT 5 gate
make baseline   # run the restored MQTT 3.1.1 baseline
make mqtt5      # run the mandatory MQTT 5 suite
make hardening-v5  # run MQTT 5 hardening cases
make clean      # remove the build/ directory
```

The build produces `build/pubsub_tests_v311` and `build/pubsub_tests_v5`. Both
implementation `.cpp` files are compiled in both profiles, matching production source
discovery; the nonselected source emits no definitions because of its whole-file guard.
The compact MQTT 3.1.1 build excludes tests that require MQTT 5-only public types.

## Sanitizers (the `SANITIZE` toggle)

AddressSanitizer and UndefinedBehaviorSanitizer are **on by default**
(`-fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all`).
Any memory or undefined-behavior violation aborts the run with a nonzero exit
status. This is the primary detector for the memory-safety findings (F-01, F-02,
F-08, and the callback-contract boundary).

Turn sanitizers off with:

```sh
make SANITIZE=0            # build without ASan/UBSan
make SANITIZE=0 test       # ... and run
```

## Suite selection (`-ts=`)

The main suites are `baseline`, `hardening`, and `mqtt5`. Run profile-specific
filters against the matching binary:

```sh
./build/pubsub_tests_v311 -ts=baseline
./build/pubsub_tests_v5 -ts=mqtt5
./build/pubsub_tests_v5 -ts=hardening
./build/pubsub_tests_v5 -ts=mqtt5 -tc="*publish*"
```

Extra doctest flags can also be passed through the Makefile:

```sh
make test ARGS=--no-colors
make mqtt5 ARGS=-s
```

## Baseline vs Hardening

- **Baseline suite** — characterization tests for the restored MQTT 3.1.1
  implementation and its compact public API. `make baseline` must be green.

- **Hardening and MQTT 5 suites** — validation for the extended MQTT 5 profile.
  The genuinely open findings are marked **expected-to-fail** (see the registry
  below), so expected behavior remains visible without failing `make hardening-v5`.

### Current expected-fail findings

As of the last empirical verification (task 12.1), the open findings — the ones
whose hardening cases are expected to fail against the current library — are:

| Finding | Area | Hardening case(s) |
|---|---|---|
| F-05 | Partial transport write reuses a desynchronized connection | `streaming_test.cpp`, `findings_test.cpp` |
| F-10 | `subscribe` ignores the SUBACK return code (reports success unconditionally) | `subscribe_test.cpp`, `findings_test.cpp` |
| F-11 | `disconnect()` with no argument sends no DISCONNECT packet | `connect_test.cpp`, `findings_test.cpp` |
| F-03 (deadline) | Trickle-fed inbound bytes are bounded only by a per-byte timeout, not a packet-wide deadline | `buffer_test.cpp`, `findings_test.cpp` |

All other findings (F-01, F-02, F-04, F-06, F-07, F-08, F-09, and the F-03
oversized prompt-close aspect) are already hardened and their cases pass.

## Expected-fail registry workflow (`FindingStatus.h`)

The expected-fail state of each finding lives in a single header,
`src/lib/FindingStatus.h`. Each finding maps to a doctest test-case decorator via
`FINDING_MARKER(Fxx)`:

- `FINDING_OPEN` → `doctest::should_fail()` — the case is expected to fail
  (library not yet hardened). A failure is reported as *expected* and does not
  set a nonzero exit status. If such a case unexpectedly **passes**, doctest
  reports an *unexpected pass* and fails the run — that is the signal that the
  finding has been hardened.
- `FINDING_HARDENED` → `doctest::skip(false)` — a no-op decorator: the case still
  runs and is expected to pass.

We deliberately use `should_fail()` and never `may_fail()`, because `may_fail()`
would hide an unexpected pass.

Hardening cases reference their finding like this:

```cpp
TEST_CASE("F-05 partial write disables reuse" * FINDING_MARKER(F05)) { ... }
```

### Flipping a finding when the library gets hardened

When the library is patched so a previously-open finding now behaves correctly,
`make hardening` will surface its case as an **unexpected pass** (and fail the
run). To resolve it, flip that finding's marker in `FindingStatus.h` — a
one-line edit:

```c
// before (open):
#define FINDING_MARKER_F05 FINDING_OPEN
// after (hardened):
#define FINDING_MARKER_F05 FINDING_HARDENED
```

Then re-run `make hardening` and confirm it is green again with no unexpected
passes. The reverse edit (`FINDING_HARDENED` → `FINDING_OPEN`) applies if a
regression re-opens a finding. Always confirm the marker matches the empirically
observed behavior of the current library before committing.
