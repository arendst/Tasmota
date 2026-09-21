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
    - F-04 is deliberately WITHDRAWN by the tasmota-pubsub-mqtt5 design. F-04 used
      to guarantee that a declared payload length above the 16-bit range was framed
      across a 3- or 4-byte Remaining Length instead of being truncated. The MQTT 5.0
      migration hard-limits the Variable Byte Integer codec to 2 bytes
      (MQTT_VBI_MAX == 16383), so large streamed payloads are no longer framed at
      all: an over-limit beginPublish() is REFUSED outright. What replaces F-04 is
      Property 28 (Requirements 4.9, 4.10, 8.17) - refusal is total, never
      truncated: zero bytes are transmitted for the refused packet and the
      connection state is left unchanged, so the outbound byte stream stays
      synchronized. The case below therefore asserts refusal above 16,383 and
      correct 2-byte framing at and just below it, pinning the boundary from both
      sides. Still marked FINDING_MARKER(F04) so the withdrawal stays traceable.
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

constexpr uint32_t kLibraryRemainingLengthMax =
    kMqtt5 ? 16383u : 268435455u;

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

    // Property 4 for streamed payloads at the Remaining Length limit, asserted
    // against the library's *real* emitted bytes. The streaming path is not
    // bounded by the working buffer (only its header is buffered), so the binding
    // limit depends on the selected profile. MQTT 5 uses a two-byte Variable
    // Byte Integer (`kLibraryRemainingLengthMax == 16383`), while the restored
    // MQTT 3.1.1 implementation retains one-to-four-byte Remaining Length support.
    // Payloads within the selected limit round-trip end to end; larger values are
    // refused without emitting bytes.
    TEST_CASE("Property 4: streaming PUBLISH field round-trip across the 16-bit boundary") {
        const size_t payloadLengths[] = {16379, 16380, 16381, 16383, 16384, 65535, 65536};
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

                // Remaining Length = 2 + topicLen + plength.
                const size_t rl = 2 + topic.size() + plen;
                const std::vector<uint8_t> payload = makePayload(plen);
                // Buffered write is used so the payload is emitted in one call;
                // the recorded outbound bytes are the full PUBLISH.
                const bool ok = streamPublish(psc, topic, payload, retained, WriteMode::Buffered);

                if (rl > kLibraryRemainingLengthMax) {
                    CHECK_FALSE(ok);
                    CHECK(client.outbound().empty());
                    CHECK(psc.connected());
                    continue;
                }
                REQUIRE(ok);

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

    // F-04 WITHDRAWN - replaced by Property 28 (Requirements 4.9, 4.10, 8.17).
    //
    // F-04 originally pinned this behavior: a declared payload length above the
    // 16-bit range had to be framed across a 3- or 4-byte Remaining Length rather
    // than truncated, because beginPublish() buffers only the header and streams
    // the payload, so a very large *declared* length was feasible to frame.
    //
    // The tasmota-pubsub-mqtt5 design deliberately retires that contract. The
    // Variable Byte Integer codec is hard-limited to 2 bytes (MQTT_VBI_MAX ==
    // 16383), so a Remaining Length above 16,383 is not framed at all - it is
    // refused. Property 28 replaces F-04's value: refusal is total, never
    // truncated. For every over-limit declared length the client transmits zero
    // bytes, returns failure to the caller, and leaves the connection state
    // unchanged, so the outbound byte stream stays synchronized (Requirement 8.17
    // for the streaming path specifically, 4.9/4.10 generally).
    //
    // The boundary is pinned from both sides: Remaining Lengths of 16,382 and
    // 16,383 still frame correctly in exactly two length bytes, 16,384 is refused.
    TEST_CASE("F-04 withdrawn: an over-limit declared payload length is refused, never truncated"
              * FINDING_MARKER(F04)) {
        const std::string topic = "t";          // topicLen == 1
        const size_t topicLen = topic.size();
        // Remaining Length = plength + 2 + topicLen, so plength = RL - 2 - topicLen.
        const unsigned int overhead = static_cast<unsigned int>(2u + topicLen);

        SUBCASE("declared lengths above the 2-byte limit are refused with nothing emitted") {
            // The original F-04 vectors, now expected to be refused outright.
            const unsigned int plengths[] = {
                65533u,                 // RL = 65536    (used to need 3 bytes)
                65536u,                 // RL = 65539    (3 bytes)
                100000u,                // RL = 100003   (3 bytes)
                2097149u,               // RL = 2097152  (used to need 4 bytes)
                10000000u,              // RL = 10000003 (4 bytes)
                268435455u - 2u - 1u,   // RL = 268435455 (old MQTT maximum)
                16384u - overhead,      // RL = 16384, the first refused value
            };

            for (unsigned int plen : plengths) {
                CAPTURE(plen);
                TestClock::instance().reset();
                MockClient client;
                PubSubClient psc(client);
                connectAndClear(client, psc);

                // Refused: the Remaining Length does not fit two bytes.
                CHECK_FALSE(psc.beginPublish(topic.c_str(), plen, false));

                // Not one byte of the refused packet reaches the transport, so a
                // truncated or wrapped Remaining Length can never desync the stream.
                CHECK(client.outbound().empty());

                // The connection state is untouched by the refusal.
                CHECK(psc.connected());
                CHECK_FALSE(client.stopCalled());
            }
        }

        SUBCASE("declared lengths at and just below the limit still frame in two length bytes") {
            const uint32_t remainingLengths[] = {16382u, 16383u};

            for (uint32_t rlWanted : remainingLengths) {
                CAPTURE(rlWanted);
                const unsigned int plen = static_cast<unsigned int>(rlWanted - overhead);
                CAPTURE(plen);
                TestClock::instance().reset();
                MockClient client;
                PubSubClient psc(client);
                connectAndClear(client, psc);

                REQUIRE(psc.beginPublish(topic.c_str(), plen, false));

                const std::vector<uint8_t>& out = client.outbound();
                REQUIRE(out.size() >= 3);

                // Fixed-header high nibble is PUBLISH, retain bit clear.
                CHECK(static_cast<uint8_t>(out[0] & 0xF0) == static_cast<uint8_t>(MQTTPUBLISH));
                CHECK((out[0] & 0x01) == 0x00);

                // Exactly two Remaining Length bytes, decoding to the wanted value.
                uint32_t rl = 0;
                size_t rlBytes = 0;
                REQUIRE(MqttParser::decodeRemainingLength(out, 1, rl, rlBytes));
                CHECK(rl == rlWanted);
                CHECK(rlBytes == 2u);

                // The expected two length bytes, spelled out.
                CHECK(out[1] == static_cast<uint8_t>((rlWanted & 0x7Fu) | 0x80u));
                CHECK(out[2] == static_cast<uint8_t>(rlWanted >> 7));

                // Header framing stays self-consistent: fixed header (1) +
                // Remaining Length (2) + topic length prefix (2) + topic bytes.
                // No payload has been streamed yet.
                CHECK(out.size() == 1u + rlBytes + 2u + topicLen);

                const size_t topicLenPos = 1u + rlBytes;
                REQUIRE(out.size() >= topicLenPos + 2u);
                const uint16_t emittedTopicLen = static_cast<uint16_t>(
                    (out[topicLenPos] << 8) | out[topicLenPos + 1]);
                CHECK(emittedTopicLen == topicLen);
            }
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
