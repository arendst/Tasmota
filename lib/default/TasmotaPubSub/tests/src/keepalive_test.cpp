/*
  keepalive_test.cpp - keepalive / ping cycle characterization for the
  TasmotaPubSub host test system (task 10.6).

  Keepalive is driven entirely in VIRTUAL time: the shim's delay() advances the
  same TestClock that the library's millis() reads, so advancing the clock past
  the keepalive interval and calling loop() exercises the ping machinery with no
  real elapsed wall-clock time (Requirement 7.3). setKeepAlive() (seconds) is
  configured BEFORE connect so the CONNECT packet and the loop() timing both use
  the chosen interval.

  Baseline (TEST_SUITE("baseline")):
    - Advancing TestClock beyond the keepalive interval then calling loop()
      records a PINGREQ (Requirement 12.1).
    - A scripted PINGRESP clears the outstanding-ping state, so the connection
      stays alive and a later idle interval issues a further PINGREQ instead of
      declaring a timeout (Requirement 12.2).
    - Advancing beyond the interval a second time with no PINGRESP reports a lost
      connection (Requirement 12.3).

  Properties (folded in as single deterministic, data-driven cases over a curated
  interval table {15 s default, 2 s short} - NO randomized generators):
    - Property 8: exactly one PINGREQ is issued per idle interval, and after a
      matching PINGRESP another PINGREQ is issued on the next interval rather
      than a timeout being declared (Requirements 12.1, 12.2, 18.9).
    - Property 9: advancing beyond two consecutive intervals with no PINGRESP
      reports a lost/timed-out connection (Requirements 12.3, 18.8, 18.9).

  Every assertion goes through the public API and decoded MockClient.outbound()
  wire bytes - never private members - so the baseline stays durable across a
  future MQTT 5 migration (Requirement 19.1).
*/

#include <cstdint>
#include <vector>

#include "doctest.h"

#include "MockClient.h"
#include "MqttPacket.h"
#include "TestClock.h"
#include "PubSubClient.h"

namespace {

// Connect the client/psc pair with a scripted CONNACK using the supplied
// keepalive interval (seconds, set BEFORE connect), then clear the recorded
// CONNECT bytes so only the packets emitted by loop() remain in outbound().
void connectAndClear(MockClient& client, PubSubClient& psc, uint16_t keepAliveSecs) {
    client.pushPacket(MqttPacket::connack(0));   // return code 0 = accepted
    psc.setServer("broker.example", 1883);
    psc.setKeepAlive(keepAliveSecs);
    REQUIRE(psc.connect("keepalive-client"));
    REQUIRE(psc.connected());
    client.clearOutbound();
}

// Move virtual time strictly beyond one keepalive interval (the library's guard
// is `t - lastActivity > keepAlive*1000`). A generous 1 s margin also absorbs
// any small virtual time consumed during connect().
void advanceBeyondInterval(uint16_t keepAliveSecs) {
    TestClock::instance().advance(static_cast<unsigned long>(keepAliveSecs) * 1000UL + 1000UL);
}

// True when `out` is exactly one MQTT PINGREQ control packet (0xC0 0x00).
bool isSinglePingreq(const std::vector<uint8_t>& out) {
    DecodedPacket d = MqttParser::decode(out);
    return d.valid &&
           d.type == static_cast<uint8_t>(MQTTPINGREQ) &&
           d.flags == 0x00 &&
           d.remainingLength == 0 &&
           out.size() == 2;
}

}  // namespace

TEST_SUITE("baseline") {

    // --- 12.1: idle beyond the interval + loop() => PINGREQ recorded --------

    TEST_CASE("advancing beyond the keepalive interval and calling loop records a PINGREQ") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc, MQTT_KEEPALIVE);   // default 15 s

        advanceBeyondInterval(MQTT_KEEPALIVE);
        REQUIRE(psc.loop());                            // still connected

        // The recorded outbound is exactly one well-framed PINGREQ.
        CHECK(isSinglePingreq(client.outbound()));
        CHECK(psc.connected());
    }

    // --- 12.2: a scripted PINGRESP clears the outstanding-ping state --------

    TEST_CASE("a scripted PINGRESP clears the outstanding-ping state and keeps the connection alive") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc, MQTT_KEEPALIVE);

        // First idle interval: a PINGREQ goes out and a ping is now outstanding.
        advanceBeyondInterval(MQTT_KEEPALIVE);
        REQUIRE(psc.loop());
        REQUIRE(isSinglePingreq(client.outbound()));

        // Deliver the matching PINGRESP; loop() consumes it and clears the
        // outstanding-ping state. No bytes are written back for a PINGRESP.
        client.clearOutbound();
        client.pushPacket(MqttPacket::pingresp());
        REQUIRE(psc.loop());
        CHECK(client.outbound().empty());
        CHECK(psc.connected());

        // Because the outstanding ping was cleared, the next idle interval issues
        // a FURTHER PINGREQ instead of declaring a timeout - the observable proof
        // that the outstanding-ping state was reset.
        client.clearOutbound();
        advanceBeyondInterval(MQTT_KEEPALIVE);
        REQUIRE(psc.loop());
        CHECK(isSinglePingreq(client.outbound()));
        CHECK(psc.connected());
    }

    // --- 12.3: idle beyond the interval with no PINGRESP => lost connection --

    TEST_CASE("advancing beyond the interval without a PINGRESP reports a lost connection") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc, MQTT_KEEPALIVE);

        // First interval: PINGREQ sent, ping now outstanding.
        advanceBeyondInterval(MQTT_KEEPALIVE);
        REQUIRE(psc.loop());
        REQUIRE(isSinglePingreq(client.outbound()));

        // Second interval with no PINGRESP delivered: the library detects the
        // unanswered ping and reports a lost/timed-out connection.
        advanceBeyondInterval(MQTT_KEEPALIVE);
        CHECK_FALSE(psc.loop());
        CHECK_FALSE(psc.connected());
        CHECK(psc.state() == MQTT_CONNECTION_TIMEOUT);
        CHECK(client.stopCalled());
    }

    // --- Property 8: exactly one PINGREQ per idle interval ------------------

    // Feature: tasmota-pubsub-tests, Property 8: for all keepalive intervals,
    // when the virtual clock is advanced beyond the interval with no other
    // activity and loop() is called, exactly one PINGREQ is recorded; and after a
    // matching PINGRESP is delivered, advancing another interval issues a further
    // PINGREQ rather than declaring a timeout. Validated deterministically over a
    // curated interval table (default 15 s and a short 2 s), no randomized
    // generators.
    TEST_CASE("Property 8: keepalive issues exactly one PINGREQ per idle interval") {
        const uint16_t intervals[] = {MQTT_KEEPALIVE, 2};

        for (uint16_t keepAliveSecs : intervals) {
            CAPTURE(keepAliveSecs);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            connectAndClear(client, psc, keepAliveSecs);

            // One idle interval => exactly one PINGREQ.
            advanceBeyondInterval(keepAliveSecs);
            REQUIRE(psc.loop());
            CHECK(isSinglePingreq(client.outbound()));

            // Calling loop() again WITHOUT advancing must not emit a second
            // PINGREQ within the same idle interval.
            client.clearOutbound();
            REQUIRE(psc.loop());
            CHECK(client.outbound().empty());
            CHECK(psc.connected());

            // Deliver the matching PINGRESP so the outstanding ping is cleared.
            client.clearOutbound();
            client.pushPacket(MqttPacket::pingresp());
            REQUIRE(psc.loop());
            CHECK(client.outbound().empty());
            CHECK(psc.connected());

            // The next idle interval issues a FURTHER PINGREQ (not a timeout).
            client.clearOutbound();
            advanceBeyondInterval(keepAliveSecs);
            REQUIRE(psc.loop());
            CHECK(isSinglePingreq(client.outbound()));
            CHECK(psc.connected());
        }
    }

    // --- Property 9: missing PINGRESP produces a lost connection ------------

    // Feature: tasmota-pubsub-tests, Property 9: for all keepalive intervals,
    // advancing the virtual clock beyond two consecutive intervals without
    // delivering a PINGRESP causes the library to report a lost/timed-out
    // connection. Validated deterministically over a curated interval table
    // (default 15 s and a short 2 s), no randomized generators.
    TEST_CASE("Property 9: missing PINGRESP over two intervals reports a lost connection") {
        const uint16_t intervals[] = {MQTT_KEEPALIVE, 2};

        for (uint16_t keepAliveSecs : intervals) {
            CAPTURE(keepAliveSecs);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            connectAndClear(client, psc, keepAliveSecs);

            // First interval: PINGREQ sent, connection still alive.
            advanceBeyondInterval(keepAliveSecs);
            REQUIRE(psc.loop());
            CHECK(isSinglePingreq(client.outbound()));
            CHECK(psc.connected());

            // Second interval with no PINGRESP: the unanswered ping trips the
            // keepalive timeout and the connection is reported lost.
            advanceBeyondInterval(keepAliveSecs);
            CHECK_FALSE(psc.loop());
            CHECK_FALSE(psc.connected());
            CHECK(psc.state() == MQTT_CONNECTION_TIMEOUT);
            CHECK(client.stopCalled());
        }
    }
}
