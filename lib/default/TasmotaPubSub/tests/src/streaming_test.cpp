/*
  streaming_test.cpp - beginPublish / write / endPublish streaming-publish
  characterization + hardening for the TasmotaPubSub host test system (task 10.3).

  Baseline (TEST_SUITE("baseline")):
    - A payload streamed via beginPublish() + write() + endPublish() is reflected
      in the recorded outbound bytes (Requirement 9.3). beginPublish() emits only
      the fixed header + Remaining Length + topic; the payload is streamed
      afterwards through write(), so the recorded outbound bytes form one complete
      PUBLISH packet whose decoded topic/payload equal the input.
    - Property 4 (PUBLISH field round-trip: decoded topic == input, decoded
      payload == input, retain bit == requested flag) is folded in for the
      streaming path as a single deterministic, data-driven case (SUBCASE / table
      loops, no randomized generators) covering both the single-byte write(data)
      and the buffered write(buf,size) forms, various payload lengths, and the
      retained flag on/off.

  Hardening (TEST_SUITE("hardening")):
    - F-04 (Requirement 9.4, expected PASS): a declared payload length exceeding
      the 16-bit range must not emit a truncated Remaining Length that desyncs the
      framing. beginPublish() takes a uint32_t Remaining Length and buildHeader is
      bounded to four length bytes. Because beginPublish() buffers only the header
      (the payload is streamed via write()), a very large *declared* length is
      feasible to frame without allocating the payload, so the emitted fixed-header
      Remaining Length is verified to equal plength + 2 + topicLen (full 32-bit)
      rather than a 16-bit truncation. Marked FINDING_MARKER(F04).
    - Property 13 / F-05 (Requirement 9.5, expected FAIL): with a partial transport
      write injected (MockClient::setWriteLimit), the publish path should report
      failure and leave the connection unusable with intact framing so no later
      packet is written onto a desynchronized stream. The current fork returns
      success from endPublish() and reuses the connection, so the case is marked
      FINDING_MARKER(F05) (should_fail). Cut points are strictly less than the full
      packet length.

  Every assertion goes through the public API and decoded MockClient.outbound()
  wire bytes - never private members - so the baseline stays durable across a
  future MQTT 5 migration (Requirement 19.1).
*/

#include <cstdint>
#include <string>
#include <vector>

#include "doctest.h"

#include "FindingStatus.h"
#include "MockClient.h"
#include "MqttPacket.h"
#include "TestClock.h"
#include "PubSubClient.h"

namespace {

// Deterministic payload of length n. The pattern intentionally produces 0x00
// bytes so the round-trip also proves the streaming path is binary-safe (no
// reliance on C-string NUL termination).
std::vector<uint8_t> makePayload(size_t n) {
    std::vector<uint8_t> v(n);
    for (size_t i = 0; i < n; ++i) {
        v[i] = static_cast<uint8_t>((i * 31u + 7u) & 0xFFu);
    }
    return v;
}

// Connect the client/psc pair with a scripted CONNACK and clear the recorded
// CONNECT bytes so only the packet-under-test remains in outbound().
void connectAndClear(MockClient& client, PubSubClient& psc) {
    client.pushPacket(MqttPacket::connack(0));
    psc.setServer("broker.example", 1883);
    REQUIRE(psc.connect("stream-client"));
    REQUIRE(psc.connected());
    client.clearOutbound();
}

// How the payload is fed to the library after beginPublish().
enum class WriteMode { Single, Buffered };

// Perform a complete streamed publish through the public API only:
// beginPublish() -> write() (per WriteMode) -> endPublish(). Returns true when
// every step reported success. All bytes written are recorded by the MockClient
// and, together with the header emitted by beginPublish(), form the full PUBLISH.
bool streamPublish(PubSubClient& psc, const std::string& topic,
                   const std::vector<uint8_t>& payload, bool retained,
                   WriteMode mode) {
    if (!psc.beginPublish(topic.c_str(),
                          static_cast<unsigned int>(payload.size()), retained)) {
        return false;
    }
    if (mode == WriteMode::Single) {
        for (uint8_t b : payload) {
            if (psc.write(b) != 1) {
                return false;
            }
        }
    } else {  // WriteMode::Buffered
        if (!payload.empty()) {
            if (psc.write(payload.data(), payload.size()) != payload.size()) {
                return false;
            }
        }
    }
    return psc.endPublish() != 0;
}

}  // namespace

TEST_SUITE("baseline") {

    // --- 9.3: streamed payload is reflected in the recorded outbound bytes ---

    TEST_CASE("streamed publish (single-byte write) is reflected in the outbound PUBLISH") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);

        const std::string topic = "tele/dev/SENSOR";
        const std::string msg = "streamed hello";
        const std::vector<uint8_t> payload(msg.begin(), msg.end());

        REQUIRE(psc.beginPublish(topic.c_str(),
                                 static_cast<unsigned int>(payload.size()), false));
        for (uint8_t b : payload) {
            REQUIRE(psc.write(b) == 1);
        }
        REQUIRE(psc.endPublish() != 0);

        const std::vector<uint8_t>& out = client.outbound();
        REQUIRE(MqttParser::isStructurallyValidPublish(out));

        DecodedPublish d = MqttParser::decodePublish(out);
        REQUIRE(d.valid);
        CHECK(d.qos == 0);
        CHECK(d.msgId == 0);           // streaming publish is QoS 0 (no packet id)
        CHECK_FALSE(d.retain);
        CHECK(d.topic == topic);
        CHECK(d.payload == payload);
    }

    TEST_CASE("streamed publish (buffered write) is reflected in the outbound PUBLISH") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);

        const std::string topic = "stat/dev/RESULT";
        const std::vector<uint8_t> payload = makePayload(200);

        REQUIRE(psc.beginPublish(topic.c_str(),
                                 static_cast<unsigned int>(payload.size()), true));
        REQUIRE(psc.write(payload.data(), payload.size()) == payload.size());
        REQUIRE(psc.endPublish() != 0);

        const std::vector<uint8_t>& out = client.outbound();
        REQUIRE(MqttParser::isStructurallyValidPublish(out));

        DecodedPublish d = MqttParser::decodePublish(out);
        REQUIRE(d.valid);
        CHECK(d.qos == 0);
        CHECK(d.retain);               // retained flag requested
        CHECK(d.topic == topic);
        CHECK(d.payload == payload);
    }

    // The payload may be streamed across several write() calls; the recorded
    // outbound bytes must still assemble into one complete, well-framed PUBLISH.
    TEST_CASE("streamed publish across chunked writes assembles one framed PUBLISH") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);

        const std::string topic = "tele/dev/CHUNKED";
        const std::vector<uint8_t> payload = makePayload(50);

        REQUIRE(psc.beginPublish(topic.c_str(),
                                 static_cast<unsigned int>(payload.size()), false));
        // Mix single-byte and buffered writes to model an incremental producer.
        REQUIRE(psc.write(payload[0]) == 1);
        REQUIRE(psc.write(payload.data() + 1, 9) == 9);
        for (size_t i = 10; i < 20; ++i) {
            REQUIRE(psc.write(payload[i]) == 1);
        }
        REQUIRE(psc.write(payload.data() + 20, payload.size() - 20)
                == payload.size() - 20);
        REQUIRE(psc.endPublish() != 0);

        const std::vector<uint8_t>& out = client.outbound();
        REQUIRE(MqttParser::isStructurallyValidPublish(out));
        DecodedPublish d = MqttParser::decodePublish(out);
        REQUIRE(d.valid);
        CHECK(d.topic == topic);
        CHECK(d.payload == payload);
    }

    // --- Property 4: streaming PUBLISH field round-trip ---------------------

    // Feature: tasmota-pubsub-tests, Property 4: for all topics and payloads
    // (including the retained flag and empty payloads), every publish path emits
    // a PUBLISH whose decoded topic equals the input topic, whose decoded payload
    // equals the input payload, and whose retain bit equals the requested flag.
    // Validated deterministically for the streaming path over a curated table of
    // payload lengths, both write modes, and the retained flag (no randomized
    // generators).
    TEST_CASE("Property 4: streaming PUBLISH field round-trip over write modes and lengths") {
        // Payload-length boundaries reachable through the default 1200-byte
        // working buffer's header (the payload itself is streamed, not buffered):
        // empty, single byte, the 1->2 byte Remaining Length transition, and a
        // few larger sizes.
        const size_t payloadLengths[] = {0, 1, 2, 127, 128, 255, 1000};
        const bool retainedFlags[] = {false, true};
        const WriteMode modes[] = {WriteMode::Single, WriteMode::Buffered};
        const std::string topic = "tele/dev/SENSOR";

        for (WriteMode mode : modes) {
            for (size_t plen : payloadLengths) {
                for (bool retained : retainedFlags) {
                    CAPTURE(static_cast<int>(mode));
                    CAPTURE(plen);
                    CAPTURE(retained);
                    TestClock::instance().reset();
                    MockClient client;
                    PubSubClient psc(client);
                    connectAndClear(client, psc);

                    const std::vector<uint8_t> payload = makePayload(plen);
                    REQUIRE(streamPublish(psc, topic, payload, retained, mode));

                    const std::vector<uint8_t>& out = client.outbound();
                    REQUIRE(MqttParser::isStructurallyValidPublish(out));

                    DecodedPublish d = MqttParser::decodePublish(out);
                    REQUIRE(d.valid);
                    CHECK(d.qos == 0);
                    CHECK(d.msgId == 0);
                    CHECK(d.retain == retained);
                    CHECK(d.topic == topic);
                    CHECK(d.payload == payload);
                }
            }
        }
    }

    // Property 4 for streamed payloads that cross the 16-bit Remaining Length
    // boundary, asserted against the library's *real* emitted bytes. The buffered
    // publish() path cannot frame these (its payload must fit the working
    // buffer), but the streaming path frames them because the payload is not
    // buffered. This exercises the 2->3 byte Remaining Length transition and
    // payloads above 65535 bytes end to end.
    TEST_CASE("Property 4: streaming PUBLISH field round-trip across the 16-bit boundary") {
        const size_t payloadLengths[] = {16383, 16384, 65535, 65536};
        const bool retainedFlags[] = {false, true};
        const std::string topic = "t";  // small topic keeps the header tiny

        for (size_t plen : payloadLengths) {
            for (bool retained : retainedFlags) {
                CAPTURE(plen);
                CAPTURE(retained);
                TestClock::instance().reset();
                MockClient client;
                PubSubClient psc(client);
                connectAndClear(client, psc);

                const std::vector<uint8_t> payload = makePayload(plen);
                // Buffered write is used so the large payload is emitted in one
                // call; the recorded outbound bytes are the full PUBLISH.
                REQUIRE(streamPublish(psc, topic, payload, retained, WriteMode::Buffered));

                const std::vector<uint8_t>& out = client.outbound();
                REQUIRE(MqttParser::isStructurallyValidPublish(out));

                DecodedPublish d = MqttParser::decodePublish(out);
                REQUIRE(d.valid);
                CHECK(d.retain == retained);
                CHECK(d.topic == topic);
                CHECK(d.payload == payload);
            }
        }
    }
}

TEST_SUITE("hardening") {

    // F-04 (Requirement 9.4): a declared payload length exceeding the 16-bit
    // range must not emit a truncated Remaining Length that desynchronizes the
    // packet framing. beginPublish() buffers only the header (fixed header +
    // Remaining Length + topic) and streams the payload separately, so a very
    // large *declared* length can be framed without allocating the payload.
    // Verify the emitted Remaining Length equals plength + 2 + topicLen using the
    // full 32-bit value rather than a 16-bit truncation, and that the header's
    // byte layout stays self-consistent. F-04 is hardened in the current fork
    // (32-bit buildHeader bounded to four length bytes), so this is expected PASS.
    TEST_CASE("F-04 large declared payload length frames the Remaining Length without truncation"
              * FINDING_MARKER(F04)) {
        const std::string topic = "t";          // topicLen == 1
        const size_t topicLen = topic.size();
        // Declared lengths that force Remaining Length above the 16-bit range,
        // including the 2->3 and 3->4 byte transitions and the MQTT maximum.
        // Remaining Length = plength + 2 + topicLen.
        const unsigned int plengths[] = {
            65533u,                 // RL = 65536  (first value needing 3 bytes)
            65536u,                 // RL = 65539  (3 bytes)
            100000u,                // RL = 100003 (3 bytes)
            2097149u,               // RL = 2097152 (first value needing 4 bytes)
            10000000u,              // RL = 10000003 (4 bytes)
            268435455u - 2u - 1u,   // RL = 268435455 (MQTT maximum, 4 bytes)
        };

        for (unsigned int plen : plengths) {
            CAPTURE(plen);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            connectAndClear(client, psc);

            // beginPublish emits the header only; with no write limit the mock
            // accepts every byte so beginPublish reports success.
            REQUIRE(psc.beginPublish(topic.c_str(), plen, false));

            const std::vector<uint8_t>& out = client.outbound();
            REQUIRE(out.size() >= 2);

            // Fixed-header high nibble is PUBLISH, retain bit clear.
            CHECK(static_cast<uint8_t>(out[0] & 0xF0) == static_cast<uint8_t>(MQTTPUBLISH));
            CHECK((out[0] & 0x01) == 0x00);

            // Decode the Remaining Length field directly from the emitted header.
            uint32_t rl = 0;
            size_t rlBytes = 0;
            REQUIRE(MqttParser::decodeRemainingLength(out, 1, rl, rlBytes));

            const uint32_t expected =
                static_cast<uint32_t>(plen) + 2u + static_cast<uint32_t>(topicLen);

            // The Remaining Length is the full 32-bit value, not a 16-bit
            // truncation that would desync framing.
            CHECK(rl == expected);
            if (expected > 0xFFFFu) {
                CHECK(rl != (expected & 0xFFFFu));
            }

            // Header framing stays self-consistent: beginPublish emits exactly
            // the fixed header (1) + Remaining Length bytes + topic length prefix
            // (2) + topic bytes. No payload has been streamed yet.
            CHECK(out.size() == 1u + rlBytes + 2u + topicLen);

            // The emitted 2-byte topic length prefix matches the topic.
            const size_t topicLenPos = 1u + rlBytes;
            REQUIRE(out.size() >= topicLenPos + 2u);
            const uint16_t emittedTopicLen = static_cast<uint16_t>(
                (out[topicLenPos] << 8) | out[topicLenPos + 1]);
            CHECK(emittedTopicLen == topicLen);
        }
    }

    // Feature: tasmota-pubsub-tests, Property 13: for all partial-write cut
    // points strictly less than the full packet length, the publish path reports
    // failure and leaves the connection unusable (lost/closed) so that no
    // subsequent packet is written onto a desynchronized stream. The current fork
    // returns success from endPublish() and keeps the connection, so this is
    // expected to fail until F-05 is hardened.
    TEST_CASE("F-05 partial transport write fails the streaming publish and disables reuse"
              * FINDING_MARKER(F05)) {
        const std::string topic = "s";                 // topicLen == 1
        const std::vector<uint8_t> payload = makePayload(300);
        // Header = fixed(1) + Remaining Length(2, since RL=303) + topic len(2)
        // + topic(1) = 6 bytes; full packet length = 6 + 300 = 306. Cut points
        // are >= 6 (so beginPublish's header write completes) and < 300 (so the
        // buffered payload write is truncated) - all strictly less than 306.
        const size_t cutPoints[] = {10, 50, 150, 299};

        for (size_t cut : cutPoints) {
            CAPTURE(cut);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            connectAndClear(client, psc);

            client.setWriteLimit(cut);

            // The header (6 bytes) fits within the cut, so beginPublish succeeds.
            REQUIRE(psc.beginPublish(topic.c_str(),
                                     static_cast<unsigned int>(payload.size()), false));

            // The transport accepts fewer payload bytes than requested: a genuine
            // partial write is injected here.
            const size_t written = psc.write(payload.data(), payload.size());
            REQUIRE(written < payload.size());

            // Hardened contract: the publish path reports failure...
            const int endRc = psc.endPublish();
            CHECK(endRc == 0);

            // ...and leaves the connection unusable so no later packet can be
            // written onto the now-desynchronized stream.
            CHECK_FALSE(psc.connected());
        }
    }
}
