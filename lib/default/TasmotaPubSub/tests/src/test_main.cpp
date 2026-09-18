/*
  test_main.cpp - doctest entry point for the TasmotaPubSub host test system.

  This is the SINGLE translation unit in the whole test binary that defines
  DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN before including the doctest single header,
  so doctest's main() and implementation are emitted here exactly once. Every
  other *_test.cpp includes "doctest.h" only.

  It also carries the first trivial baseline test that constructs the unmodified
  PubSubClient library against the Arduino shim, proving the whole toolchain
  (library + shim + support lib + doctest) compiles, links, runs, and reports
  pass/fail/skip counts under ASan/UBSan (task 4.2 / Requirements 1.2, 1.3,
  3.1, 3.2, 3.3).
*/

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "TestClock.h"
#include "PubSubClient.h"

// Every case is tagged into either the "baseline" or "hardening" suite so the
// two groups can be selected independently via -ts=baseline / -ts=hardening.
TEST_SUITE("baseline") {

    // Characterization test: a freshly default-constructed PubSubClient has a
    // stable, well-defined initial observable state. Per PubSubClient.h the
    // _state member is initialized to MQTT_DISCONNECTED (-1) and no transport
    // is attached, so connected() must report false.
    TEST_CASE("PubSubClient default construction has a stable initial state") {
        // Reset the virtual clock for determinism/isolation (optional here).
        TestClock::instance().reset();

        PubSubClient client;

        CHECK(client.state() == MQTT_DISCONNECTED);
        CHECK(client.state() == -1);
        CHECK(client.connected() == false);
    }
}
