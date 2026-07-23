/*
  buffer_test.cpp - buffer sizing and incoming-packet-cap characterization +
  hardening for the TasmotaPubSub host test system (task 10.7).

  Baseline (TEST_SUITE("baseline")):
    - setBufferSize with a valid size (allocation succeeds) => getBufferSize
      returns that size (Requirement 13.1).
    - Property 7 (setBufferSize / getBufferSize round-trip) is folded in as a
      single deterministic, data-driven case over a curated table of sizes
      (no randomized generators). A rejected size (0) leaves the prior size
      unchanged.
    - setMaxIncomingPacketSize / getMaxIncomingPacketSize round-trip, and the
      OBSERVED behavior of the current implementation when an inbound packet
      declares a total size larger than the configured cap (Requirement 13.3):
      the connection is closed and loop() reports the disconnect.

  Hardening (TEST_SUITE("hardening")):
    - F-07 (Requirement 13.2, expected PASS): with the malloc/realloc interposer
      armed to fail the next allocation, a failed setBufferSize does NOT leave a
      nonzero reported capacity backed by a null/failed buffer. The library
      commits the new size only after a successful realloc, so getBufferSize
      still reports the previous (valid) size and the buffer stays usable - proven
      by a subsequent publish exercised under AddressSanitizer.
      Marked FINDING_MARKER(F07).
    - Property 12 / F-03 prompt-close (Requirement 13.4, expected PASS): an
      inbound packet declaring a Remaining Length larger than the accepted
      capacity closes the connection after a bounded prefix (assert stopCalled())
      rather than draining the whole declared body. Marked
      FINDING_MARKER(F03_PROMPT_CLOSE).
    - F-03 packet-wide deadline (Requirement 13.5, expected FAIL): trickle-fed
      inbound bytes should be bounded by a packet-wide deadline, not only by the
      per-byte socket timeout. The current fork enforces only a per-byte timeout,
      so a byte-per-second trickle is read to completion and dispatched. Marked
      FINDING_MARKER(F03_DEADLINE) (should_fail).

  Every assertion goes through the public API and decoded MockClient.outbound()
  wire bytes - never private members - so the baseline stays durable across a
  future MQTT 5 migration (Requirement 19.1).
*/

#include <cstdint>
#include <string>
#include <vector>

#include "doctest.h"

#include "AllocInterposer.h"
#include "FindingStatus.h"
#include "MockClient.h"
#include "MqttPacket.h"
#include "TestClock.h"
#include "PubSubClient.h"

namespace {

// Records inbound PUBLISH dispatches so the deadline test can observe whether a
// trickle-fed packet was delivered (current behavior) or refused (hardened).
struct CallbackCapture {
    int         count = 0;
    std::string topic;
    unsigned    length = 0;
};

// Connect the client/psc pair with a scripted CONNACK and clear the recorded
// CONNECT bytes so only the packet(s) under test remain in outbound().
void connectAndClear(MockClient& client, PubSubClient& psc) {
    client.pushPacket(MqttPacket::connack(0));   // return code 0 = accepted
    psc.setServer("broker.example", 1883);
    REQUIRE(psc.connect("buffer-client"));
    REQUIRE(psc.connected());
    client.clearOutbound();
}

}  // namespace

TEST_SUITE("baseline") {

    // --- 13.1: setBufferSize (allocation succeeds) => getBufferSize ---------

    TEST_CASE("setBufferSize with a valid size makes getBufferSize return that size") {
        TestClock::instance().reset();
        AllocInterposer::reset();
        MockClient client;
        PubSubClient psc(client);

        // The constructor already sized the buffer to MQTT_MAX_PACKET_SIZE.
        CHECK(psc.getBufferSize() == MQTT_MAX_PACKET_SIZE);

        REQUIRE(psc.setBufferSize(512));
        CHECK(psc.getBufferSize() == 512);
    }

    // --- Property 7: setBufferSize / getBufferSize round-trip ---------------

    // Feature: tasmota-pubsub-tests, Property 7: for all valid buffer sizes,
    // calling setBufferSize with that size (when allocation succeeds) makes
    // getBufferSize return exactly that size. Validated deterministically over a
    // curated table of sizes spanning small, power-of-two, remaining-length
    // boundaries, and the uint16_t maximum (no randomized generators).
    TEST_CASE("Property 7: setBufferSize / getBufferSize round-trip over curated sizes") {
        TestClock::instance().reset();
        AllocInterposer::reset();
        MockClient client;
        PubSubClient psc(client);

        const uint16_t sizes[] = {
            1, 2, 16, 64, 127, 128, 255, 256, 512, 1024,
            MQTT_MAX_PACKET_SIZE, 2048, 8192, 16384, 65535,
        };

        for (uint16_t size : sizes) {
            CAPTURE(size);
            REQUIRE(psc.setBufferSize(size));      // allocation succeeds
            CHECK(psc.getBufferSize() == size);    // reports exactly that size
        }

        // A size of 0 is rejected and leaves the previously committed size intact
        // (the library refuses to shrink the capacity to zero).
        const uint16_t last = psc.getBufferSize();
        CHECK_FALSE(psc.setBufferSize(0));
        CHECK(psc.getBufferSize() == last);
    }

    // --- 13.3: setMaxIncomingPacketSize characterization --------------------

    TEST_CASE("setMaxIncomingPacketSize / getMaxIncomingPacketSize round-trip") {
        TestClock::instance().reset();
        AllocInterposer::reset();
        MockClient client;
        PubSubClient psc(client);

        // Default: the cap is disabled (0).
        CHECK(psc.getMaxIncomingPacketSize() == 0);

        const uint32_t caps[] = {1, 64, 256, 1200, 100000, 0};
        for (uint32_t cap : caps) {
            CAPTURE(cap);
            psc.setMaxIncomingPacketSize(cap);
            CHECK(psc.getMaxIncomingPacketSize() == cap);
        }
    }

    TEST_CASE("an inbound packet declaring a total size over the cap closes the connection") {
        // Characterize the CURRENT behavior (Requirement 13.3): with a small cap
        // configured, a packet whose declared total wire size exceeds the cap is
        // refused and the connection is closed - the library does not attempt to
        // drain or buffer the oversized body.
        TestClock::instance().reset();
        AllocInterposer::reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);

        psc.setMaxIncomingPacketSize(50);
        CHECK(psc.getMaxIncomingPacketSize() == 50);

        // A PUBLISH fixed header (0x30) followed by a single-byte Remaining
        // Length of 100. total = 1 (fixed) + 1 (length byte) + 100 = 102 > 50, so
        // the cap fires right after the length is decoded, before any body read.
        // Only the two-byte prefix is supplied; a drain-the-body implementation
        // would instead block on the (empty) queue and time out without closing.
        std::vector<uint8_t> frame = {static_cast<uint8_t>(MQTTPUBLISH), 100};
        client.pushInbound(frame);

        psc.setSocketTimeout(1);   // bound any unexpected read as a safety net
        const bool looped = psc.loop();

        CHECK_FALSE(looped);
        CHECK_FALSE(psc.connected());
        CHECK(psc.state() == MQTT_DISCONNECTED);
        CHECK(client.stopCalled());
    }
}

TEST_SUITE("hardening") {

    // --- F-07 (13.2): failed setBufferSize must not report null-backed size --

    // With the allocation interposer armed to fail the next allocation, the
    // realloc() inside setBufferSize() returns NULL. The library must NOT commit
    // the new size: getBufferSize() keeps reporting the previous (valid) size and
    // the existing buffer remains intact and usable. If instead it committed the
    // size while buffer became null, the subsequent publish would dereference a
    // null/invalid buffer and AddressSanitizer would abort. F-07 is hardened
    // (expected PASS).
    TEST_CASE("F-07 a failed setBufferSize leaves no nonzero capacity backed by a null buffer"
              * FINDING_MARKER(F07)) {
        TestClock::instance().reset();
        AllocInterposer::reset();
        MockClient client;
        PubSubClient psc(client);

        // Baseline: the constructor allocated the working buffer successfully.
        const uint16_t before = psc.getBufferSize();
        REQUIRE(before == MQTT_MAX_PACKET_SIZE);

        // Arm the next allocation (the realloc in setBufferSize) to fail.
        AllocInterposer::failNextAllocation();
        const bool grew = psc.setBufferSize(4096);
        AllocInterposer::reset();

        // The (re)allocation failed, so setBufferSize reports failure and the
        // reported size is UNCHANGED - never a nonzero size backed by a failed
        // allocation.
        CHECK_FALSE(grew);
        CHECK(psc.getBufferSize() == before);

        // Prove the buffer behind that reported size is still valid and usable:
        // connect and publish, decoding the emitted PUBLISH from the wire. Under
        // AddressSanitizer this also proves no null/dangling buffer was left
        // behind by the failed realloc.
        connectAndClear(client, psc);
        REQUIRE(psc.publish("tele/dev/STATE", "online"));

        DecodedPublish pub = MqttParser::decodePublish(client.outbound());
        REQUIRE(pub.valid);
        CHECK(pub.topic == "tele/dev/STATE");
        const std::string payload(pub.payload.begin(), pub.payload.end());
        CHECK(payload == "online");
        CHECK(psc.connected());
    }

    // --- Property 12 / F-03 prompt-close (13.4) -----------------------------

    // Feature: tasmota-pubsub-tests, Property 12: for all inbound packets
    // declaring a Remaining Length larger than the accepted capacity, the library
    // closes the connection after consuming only a bounded prefix rather than
    // draining the entire declared body.
    //
    // The fixture supplies ONLY the fixed header + Remaining Length bytes and no
    // body at all. A prompt-close implementation calls _client->stop() as soon as
    // it sees the oversized declared length (so stopCalled() is true almost
    // immediately). A drain-the-body implementation would instead loop reading
    // the declared body, exhaust the (empty) queue, and time out - a path that
    // returns without calling stop(), so stopCalled() would be false. Asserting
    // stopCalled() therefore proves the prompt close. F-03 prompt-close is
    // hardened (expected PASS).
    TEST_CASE("Property 12: an oversized declared inbound packet closes the connection promptly"
              * FINDING_MARKER(F03_PROMPT_CLOSE)) {
        // Declared Remaining Lengths well beyond the buffer capacity (default
        // 1200), each encoded in 1..4 Remaining-Length bytes.
        const uint32_t declaredLengths[] = {2000, 300000, 5000000, 200000000};

        for (uint32_t rl : declaredLengths) {
            CAPTURE(rl);
            TestClock::instance().reset();
            AllocInterposer::reset();
            MockClient client;
            PubSubClient psc(client);
            connectAndClear(client, psc);

            // Fixed header (PUBLISH) + Remaining Length only; no body bytes.
            std::vector<uint8_t> frame;
            frame.push_back(static_cast<uint8_t>(MQTTPUBLISH));
            const std::vector<uint8_t> rlBytes = MqttPacket::encodeRemainingLength(rl);
            frame.insert(frame.end(), rlBytes.begin(), rlBytes.end());
            client.pushInbound(frame);

            psc.setSocketTimeout(1);   // a drain path would time out fast (no stop)
            const bool looped = psc.loop();

            // Closed promptly after the bounded prefix, not after draining.
            CHECK(client.stopCalled());
            CHECK_FALSE(looped);
            CHECK_FALSE(psc.connected());
            CHECK(psc.state() == MQTT_DISCONNECTED);
        }
    }

    // --- F-03 packet-wide deadline (13.5) - expected FAIL --------------------

    // A packet-wide deadline should bound how long the library spends assembling
    // a single inbound packet, independent of how often individual bytes arrive.
    // Here the broker trickles one byte per simulated second while the per-byte
    // socket timeout is five seconds, so no single byte ever times out. A
    // hardened implementation would still abandon and close the connection once
    // the packet-wide deadline elapsed; the current fork enforces ONLY the
    // per-byte timeout, so it reads the whole packet to completion and dispatches
    // the callback. The assertions below encode the hardened expectation, so the
    // case reports an EXPECTED FAILURE against the current implementation.
    TEST_CASE("F-03 trickle-fed inbound bytes are bounded by a packet-wide deadline"
              * FINDING_MARKER(F03_DEADLINE)) {
        TestClock::instance().reset();
        AllocInterposer::reset();
        MockClient client;
        PubSubClient psc(client);
        CallbackCapture cap;

        psc.setServer("broker.example", 1883);
        psc.setKeepAlive(120);          // keep keepalive well clear of the read
        psc.setCallback([&cap](char* topic, uint8_t* /*payload*/, unsigned int len) {
            cap.count++;
            cap.topic = topic;
            cap.length = len;
        });
        client.pushPacket(MqttPacket::connack(0));
        REQUIRE(psc.connect("deadline-client"));
        REQUIRE(psc.connected());
        client.clearOutbound();

        // A small, well-formed QoS 0 PUBLISH (topic "t", payload "hi").
        const std::string topic = "t";
        const std::vector<uint8_t> payload = {'h', 'i'};
        client.pushPacket(MqttPacket::publish(topic, payload, /*qos=*/0));

        // Per-byte timeout 5 s; trickle one byte per simulated second. Each byte
        // arrives inside the per-byte window, so only a packet-wide deadline could
        // stop the read. Virtual time advances via the shim's delay(), so this
        // consumes no real wall-clock time.
        psc.setSocketTimeout(5);
        client.setTrickle(/*bytesPerReveal=*/1, /*msPerReveal=*/1000);

        psc.loop();

        // Hardened expectation: the packet-wide deadline elapsed while the bytes
        // trickled in, so the library abandoned the packet and closed the
        // connection WITHOUT dispatching it. The current fork instead delivers the
        // packet, so both checks fail here => reported as an expected failure.
        CHECK(client.stopCalled());
        CHECK(cap.count == 0);
    }
}
