# TasmotaPubSub Host Test System

A host-based (no ESP32/ESP8266 hardware required) unit-test system for the
TasmotaPubSub `PubSubClient` MQTT library. It compiles the **unmodified**
library (`../src/PubSubClient.cpp` / `.h`) against a small host-side Arduino
environment shim, drives it through a scriptable mock transport, and validates
its observable MQTT 3.1.1 wire behavior with the
[doctest](https://github.com/doctest/doctest) single-header framework.

All assertions go through the public API and the recorded/scripted wire bytes of
the mock transport — never through private members — so the suite stays
re-runnable unchanged across future refactors of the library (including a later
MQTT 5 migration).

> The library under test is never modified. If a test fails, the fix belongs in
> the test (or the finding gets hardened in the library separately) — not in the
> library source to make a test pass.

## Layout

```
tests/
  Makefile              # single-binary build + run targets
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
make            # build build/pubsub_tests (default target: all)
make test       # build + run the whole binary
make baseline   # build + run only the Baseline suite  (-ts=baseline)
make hardening  # build + run only the Hardening suite  (-ts=hardening)
make clean      # remove the build/ directory
```

The build produces a single binary, `build/pubsub_tests`, which links the
unmodified `PubSubClient.cpp`, the Arduino shim, the test support library, and
every `src/*_test.cpp`. A compilation failure yields a nonzero exit status and
produces no binary.

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

Every test case is tagged into exactly one doctest test suite: `baseline` or
`hardening`. The `make baseline` / `make hardening` targets pass the matching
`-ts=` selector to the binary. You can also select suites (or any other doctest
flag) directly:

```sh
./build/pubsub_tests -ts=baseline        # only baseline cases
./build/pubsub_tests -ts=hardening       # only hardening cases
./build/pubsub_tests                     # everything
./build/pubsub_tests -ts=baseline -tc="*publish*"   # filter within a suite
```

You can also pass extra flags through the Makefile:

```sh
make test TS=-ts=baseline ARGS=--no-colors
make hardening ARGS=-s          # -s = show successful assertions
```

## Baseline vs Hardening

- **Baseline suite** — characterization tests that lock in the library's
  *current* observable MQTT 3.1.1 behavior across the full public API and the
  wire protocol (CONNECT/CONNACK, PUBLISH/PUBACK, SUBSCRIBE/SUBACK,
  UNSUBSCRIBE/UNSUBACK, PINGREQ/PINGRESP, DISCONNECT). These are a non-regression
  safety net and **must all pass** against the current library. `make baseline`
  is expected to be entirely green.

- **Hardening suite** — tests that encode the *correct/hardened* behavior derived
  from the static-analysis findings F-01..F-12. The current fork is already
  partially hardened, so most hardening cases pass today. The genuinely open
  findings are marked **expected-to-fail** (see the registry below) so their
  failure is reported as *expected* and does not fail the run. `make hardening`
  is expected to exit zero, reporting only the designated expected failures and
  **no unexpected passes**.

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
