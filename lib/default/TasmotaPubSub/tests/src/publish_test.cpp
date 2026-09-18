/*
  publish_test.cpp - publish / publish_P / retained-flag characterization for
  the TasmotaPubSub host test system (task 10.2).

  Baseline (TEST_SUITE("baseline")):
    - publish() with a topic and payload records a structurally valid PUBLISH
      packet (Requirement 9.1).
    - publish_P() records a structurally valid PUBLISH packet (Requirement 9.2).
    - Retained-message flag handling: the decoded retain bit matches the
      requested flag across both publish paths (Requirement 18.4).

    Property 4 (PUBLISH field round-trip: decoded topic == input topic, decoded
    payload == input payload, retain bit == requested flag) and Property 1
    (remaining-length codec round-trip) are folded in as single, deterministic,
    data-driven cases over curated boundary tables (no randomized generators).

  Curated boundary coverage (design "Property validation via curated
  data-driven tests"): payload/topic lengths 0, 1, 127, 128, 16383, 16384,
  65535, 65536 and near-buffer sizes; topics/payloads that are empty, typical,
  max-fitting, and multi-byte/UTF-8; retained flag on and off.

  The default working buffer is MQTT_MAX_PACKET_SIZE = 1200 and the buffer size
  is a uint16_t, so the buffered publish() path cannot frame very large
  payloads; this file characterizes the library's *actual* observable behavior
  there (publish() returns false and emits no packet) rather than assuming a
  packet is produced. The streamed publish_P() path frames large payloads
  because its payload is not buffered, so it is used to exercise the 2-/3-byte
  Remaining Length boundaries against the library's real emitted bytes; the
  4-byte boundary is driven through the MqttPacket/MqttParser codec.

  Every assertion goes through the public API and decoded MockClient.outbound()
  wire bytes - never private members - so the baseline stays durable across a
  future MQTT 5 migration (Requirement 19.1).
*/

#include <cstdint>
#include <string>
#include <vector>

#include "doctest.h"

#include "MockClient.h"
#include "MqttPacket.h"
#include "TestClock.h"
#include "PubSubClient.h"

namespace {

// Deterministic payload of length n. The pattern intentionally produces 0x00
// bytes so the round-trip also proves the paths are binary-safe (no reliance on
// C-string NUL termination for the explicit-length / streamed overloads).
std::vector<uint8_t> makePayload(size_t n) {
    std::vector<uint8_t> v(n);
    for (size_t i = 0; i < n; ++i) {
        v[i] = static_cast<uint8_t>((i * 31u + 7u) & 0xFFu);
    }
    return v;
}

// A topic of exactly n 'a' bytes (used for length-boundary coverage).
std::string makeTopic(size_t n) {
    return std::string(n, 'a');
}

// Connect the client/psc pair with a scripted CONNACK and clear the recorded
// CONNECT bytes so only the packet-under-test remains in outbound(). The buffer
// size can be pre-set by the caller before invoking this helper.
void connectAndClear(MockClient& client, PubSubClient& psc) {
    client.pushPacket(MqttPacket::connack(0));
    psc.setServer("broker.example", 1883);
    REQUIRE(psc.connect("pub-client"));
    REQUIRE(psc.connected());
    client.clearOutbound();
}

}  // namespace

TEST_SUITE("baseline") {

    // --- 9.1: publish() records a valid PUBLISH packet ----------------------

    TEST_CASE("publish with a topic and payload records a valid PUBLISH packet") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);

        REQUIRE(psc.publish("tele/dev/SENSOR", "hello world"));

        const std::vector<uint8_t>& out = client.outbound();
        REQUIRE(MqttParser::isStructurallyValidPublish(out));

        DecodedPublish d = MqttParser::decodePublish(out);
        REQUIRE(d.valid);
        CHECK(d.qos == 0);
        CHECK(d.msgId == 0);            // QoS 0 => no packet identifier
        CHECK_FALSE(d.retain);
        CHECK(d.topic == "tele/dev/SENSOR");
        const std::string msg = "hello world";
        const std::vector<uint8_t> expected(msg.begin(), msg.end());
        CHECK(d.payload == expected);
    }

    // --- 9.2: publish_P() records a valid PUBLISH packet --------------------

    TEST_CASE("publish_P records a valid PUBLISH packet") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);

        const std::string topic = "tele/dev/STATE";
        const std::vector<uint8_t> payload = makePayload(64);
        REQUIRE(psc.publish_P(topic.c_str(), payload.data(),
                              static_cast<unsigned int>(payload.size()), false));

        const std::vector<uint8_t>& out = client.outbound();
        REQUIRE(MqttParser::isStructurallyValidPublish(out));

        DecodedPublish d = MqttParser::decodePublish(out);
        REQUIRE(d.valid);
        CHECK(d.qos == 0);
        CHECK(d.msgId == 0);
        CHECK_FALSE(d.retain);
        CHECK(d.topic == topic);
        CHECK(d.payload == payload);
    }

    // --- 18.4: retained-message flag handling -------------------------------

    TEST_CASE("retained flag round-trips through publish and publish_P") {
        const bool retainedFlags[] = {false, true};
        for (bool retained : retainedFlags) {
            CAPTURE(retained);

            // publish(topic, payload, retained) - C-string overload.
            {
                TestClock::instance().reset();
                MockClient client;
                PubSubClient psc(client);
                connectAndClear(client, psc);

                REQUIRE(psc.publish("tele/dev/POWER", "ON", retained));

                DecodedPublish d = MqttParser::decodePublish(client.outbound());
                REQUIRE(d.valid);
                CHECK(d.retain == retained);
                CHECK(d.topic == "tele/dev/POWER");
                const std::vector<uint8_t> expected{'O', 'N'};
                CHECK(d.payload == expected);
            }

            // publish(topic, payload, plength, retained) - explicit-length overload.
            {
                TestClock::instance().reset();
                MockClient client;
                PubSubClient psc(client);
                connectAndClear(client, psc);

                const std::vector<uint8_t> payload = makePayload(32);
                REQUIRE(psc.publish("stat/dev/RESULT", payload.data(),
                                    static_cast<unsigned int>(payload.size()), retained));

                DecodedPublish d = MqttParser::decodePublish(client.outbound());
                REQUIRE(d.valid);
                CHECK(d.retain == retained);
                CHECK(d.payload == payload);
            }

            // publish_P(topic, payload, plength, retained).
            {
                TestClock::instance().reset();
                MockClient client;
                PubSubClient psc(client);
                connectAndClear(client, psc);

                const std::vector<uint8_t> payload = makePayload(48);
                REQUIRE(psc.publish_P("cmnd/dev/Backlog", payload.data(),
                                      static_cast<unsigned int>(payload.size()), retained));

                DecodedPublish d = MqttParser::decodePublish(client.outbound());
                REQUIRE(d.valid);
                CHECK(d.retain == retained);
                CHECK(d.payload == payload);
            }
        }
    }

    // --- Property 4: PUBLISH field round-trip over curated payload lengths ---

    // Feature: tasmota-pubsub-tests, Property 4: for all topics and payloads
    // (including the retained flag and empty payloads), every publish path emits
    // a PUBLISH whose decoded topic equals the input topic, whose decoded payload
    // equals the input payload, and whose retain bit equals the requested flag.
    // Validated deterministically over a curated boundary table (no randomized
    // generators). The streamed publish_P() path frames every size because its
    // payload is not buffered.
    TEST_CASE("Property 4: publish_P PUBLISH field round-trip over payload-length boundaries") {
        // Payload-length boundaries: empty, single byte, the 1->2 and 2->3 byte
        // Remaining Length transitions, and sizes that exceed the 16-bit range.
        const size_t payloadLengths[] = {0, 1, 127, 128, 16383, 16384, 65535, 65536};
        const bool retainedFlags[] = {false, true};
        const std::string topic = "tele/dev/SENSOR";

        for (size_t plen : payloadLengths) {
            for (bool retained : retainedFlags) {
                CAPTURE(plen);
                CAPTURE(retained);
                TestClock::instance().reset();
                MockClient client;
                PubSubClient psc(client);
                connectAndClear(client, psc);

                const std::vector<uint8_t> payload = makePayload(plen);
                REQUIRE(psc.publish_P(topic.c_str(), payload.data(),
                                      static_cast<unsigned int>(payload.size()), retained));

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

    // Property 4 for the buffered publish() path over the payload lengths that
    // fit a working buffer. Small sizes use the default 1200-byte buffer; the
    // 16383/16384 sizes use an enlarged buffer (still within the uint16_t buffer
    // size limit) so the real buffered framing path is exercised end to end.
    TEST_CASE("Property 4: buffered publish PUBLISH field round-trip within buffer capacity") {
        struct Case { size_t plen; uint16_t bufferSize; };
        const Case cases[] = {
            {0,     MQTT_MAX_PACKET_SIZE},
            {1,     MQTT_MAX_PACKET_SIZE},
            {127,   MQTT_MAX_PACKET_SIZE},
            {128,   MQTT_MAX_PACKET_SIZE},
            {16383, 20000},
            {16384, 20000},
        };
        const bool retainedFlags[] = {false, true};
        const std::string topic = "tele/dev/SENSOR";

        for (const Case& c : cases) {
            for (bool retained : retainedFlags) {
                CAPTURE(c.plen);
                CAPTURE(retained);
                TestClock::instance().reset();
                MockClient client;
                PubSubClient psc(client);
                REQUIRE(psc.setBufferSize(c.bufferSize));
                REQUIRE(psc.getBufferSize() == c.bufferSize);
                connectAndClear(client, psc);

                const std::vector<uint8_t> payload = makePayload(c.plen);
                REQUIRE(psc.publish(topic.c_str(), payload.data(),
                                    static_cast<unsigned int>(payload.size()), retained));

                const std::vector<uint8_t>& out = client.outbound();
                REQUIRE(MqttParser::isStructurallyValidPublish(out));

                DecodedPublish d = MqttParser::decodePublish(out);
                REQUIRE(d.valid);
                CHECK(d.qos == 0);
                CHECK(d.retain == retained);
                CHECK(d.topic == topic);
                CHECK(d.payload == payload);
            }
        }
    }

    // Property 4 over curated topic-length and multi-byte/UTF-8 vectors: empty,
    // single byte, the 127/128 boundary, a near-buffer topic, and UTF-8 topics
    // and payloads. Both publish paths are checked for each vector.
    TEST_CASE("Property 4: PUBLISH topic round-trip incl. multi-byte/UTF-8 and near-buffer") {
        struct Case { const char* label; std::string topic; std::vector<uint8_t> payload; };
        // A UTF-8 topic (degree sign + checkmark) and a UTF-8 payload.
        const std::string utf8Topic  = "tele/\xC2\xB0" "C/\xE2\x9C\x93";
        const std::string utf8PayloadStr = "temp=21\xC2\xB0" "C \xE2\x9C\x93";
        const std::vector<uint8_t> utf8Payload(utf8PayloadStr.begin(), utf8PayloadStr.end());

        std::vector<Case> cases = {
            {"empty topic",      makeTopic(0),    makePayload(4)},
            {"single-char topic",makeTopic(1),    makePayload(4)},
            {"topic len 127",    makeTopic(127),  makePayload(8)},
            {"topic len 128",    makeTopic(128),  makePayload(8)},
            {"near-buffer topic",makeTopic(1000), makePayload(8)},
            {"utf8 topic+payload", utf8Topic,     utf8Payload},
        };

        for (const Case& c : cases) {
            CAPTURE(c.label);

            // Streamed path (publish_P): frames every case (topic fits 1200).
            {
                TestClock::instance().reset();
                MockClient client;
                PubSubClient psc(client);
                connectAndClear(client, psc);

                REQUIRE(psc.publish_P(c.topic.c_str(), c.payload.data(),
                                      static_cast<unsigned int>(c.payload.size()), false));

                const std::vector<uint8_t>& out = client.outbound();
                REQUIRE(MqttParser::isStructurallyValidPublish(out));
                DecodedPublish d = MqttParser::decodePublish(out);
                REQUIRE(d.valid);
                CHECK(d.topic == c.topic);
                CHECK(d.payload == c.payload);
            }

            // Buffered path (publish): all these cases fit the default 1200 buffer.
            {
                TestClock::instance().reset();
                MockClient client;
                PubSubClient psc(client);
                connectAndClear(client, psc);

                REQUIRE(psc.publish(c.topic.c_str(), c.payload.data(),
                                    static_cast<unsigned int>(c.payload.size()), false));

                const std::vector<uint8_t>& out = client.outbound();
                REQUIRE(MqttParser::isStructurallyValidPublish(out));
                DecodedPublish d = MqttParser::decodePublish(out);
                REQUIRE(d.valid);
                CHECK(d.topic == c.topic);
                CHECK(d.payload == c.payload);
            }
        }
    }

    // --- Buffered publish oversized-payload behavior (characterization) ------

    // The buffered publish() path must frame the whole packet in the working
    // buffer. Characterize the library's real behavior when the payload cannot
    // fit: publish() returns false and emits no bytes. The buffer size is a
    // uint16_t, so payloads at/above ~65529 bytes can never fit any buffer.
    TEST_CASE("buffered publish refuses payloads that exceed the working buffer") {
        SUBCASE("payload just over the default buffer capacity emits nothing") {
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);              // default buffer 1200
            connectAndClear(client, psc);

            // topic "t" (len 1): fits when 1200 >= 7 + 1 + plength, i.e. plength <= 1192.
            const std::vector<uint8_t> fits = makePayload(1192);
            REQUIRE(psc.publish("t", fits.data(),
                                static_cast<unsigned int>(fits.size()), false));
            REQUIRE(MqttParser::isStructurallyValidPublish(client.outbound()));

            client.clearOutbound();
            const std::vector<uint8_t> tooBig = makePayload(1193);
            CHECK_FALSE(psc.publish("t", tooBig.data(),
                                    static_cast<unsigned int>(tooBig.size()), false));
            CHECK(client.outbound().empty());
        }

        SUBCASE("payloads >= 16-bit buffer limit cannot be buffered even at max buffer") {
            const size_t hugeLengths[] = {16384, 65535, 65536};
            for (size_t plen : hugeLengths) {
                CAPTURE(plen);
                TestClock::instance().reset();
                MockClient client;
                PubSubClient psc(client);
                REQUIRE(psc.setBufferSize(65535));  // largest a uint16_t buffer allows
                connectAndClear(client, psc);

                // 16384 fits a 65535 buffer; 65535/65536 cannot (need > uint16_t).
                const std::vector<uint8_t> payload = makePayload(plen);
                const bool ok = psc.publish("t", payload.data(),
                                            static_cast<unsigned int>(payload.size()), false);
                if (plen + 8 <= 65535) {
                    CHECK(ok);
                    CHECK(MqttParser::isStructurallyValidPublish(client.outbound()));
                } else {
                    CHECK_FALSE(ok);
                    CHECK(client.outbound().empty());
                }
            }
        }
    }

    // --- Property 1: remaining-length codec round-trip ----------------------

    // Feature: tasmota-pubsub-tests, Property 1: for all integers n in the MQTT
    // range 0..268435455, encoding n as an MQTT Remaining Length and decoding it
    // yields n using at most four bytes. Exercised deterministically at the
    // 1/2/3/4-byte boundaries via the MqttPacket/MqttParser codec, which mirrors
    // the library's buildHeader.
    TEST_CASE("Property 1: Remaining Length codec round-trip at the byte-count boundaries") {
        struct RLCase { uint32_t value; size_t bytes; };
        const RLCase cases[] = {
            {0,         1},   // minimum
            {1,         1},
            {127,       1},   // last 1-byte value
            {128,       2},   // first 2-byte value
            {16383,     2},   // last 2-byte value
            {16384,     3},   // first 3-byte value
            {2097151,   3},   // last 3-byte value
            {2097152,   4},   // first 4-byte value
            {268435455, 4},   // MQTT maximum Remaining Length
        };

        for (const RLCase& c : cases) {
            CAPTURE(c.value);
            const std::vector<uint8_t> enc = MqttPacket::encodeRemainingLength(c.value);
            CHECK(enc.size() == c.bytes);

            uint32_t decoded = 0xFFFFFFFFu;
            size_t consumed = 0;
            REQUIRE(MqttParser::decodeRemainingLength(enc, 0, decoded, consumed));
            CHECK(decoded == c.value);
            CHECK(consumed == c.bytes);
        }
    }

    // Property 1 against the library's real emitted bytes: for PUBLISH packets
    // whose Remaining Length crosses the 1->2 and 2->3 byte transitions, the
    // decoded Remaining Length equals the actual trailing byte count and equals
    // 2 + topicLen + plength. The streamed publish_P() path is used so the
    // larger sizes are framed (the buffered path cannot hold them).
    TEST_CASE("Property 1: emitted PUBLISH Remaining Length equals trailing byte count") {
        const std::string topic = "t";           // topicLen == 1
        const size_t topicLen = topic.size();
        // Remaining Length = 2 + topicLen + plength. Choose plength so RL lands
        // on each byte-count boundary.
        struct Case { size_t plen; size_t expectedRlBytes; };
        const Case cases[] = {
            {127 - 2 - 1,   1},   // RL = 127  (1 byte)
            {128 - 2 - 1,   2},   // RL = 128  (2 bytes)
            {16383 - 2 - 1, 2},   // RL = 16383 (2 bytes)
            {16384 - 2 - 1, 3},   // RL = 16384 (3 bytes)
        };

        for (const Case& c : cases) {
            CAPTURE(c.plen);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            connectAndClear(client, psc);

            const std::vector<uint8_t> payload = makePayload(c.plen);
            REQUIRE(psc.publish_P(topic.c_str(), payload.data(),
                                  static_cast<unsigned int>(payload.size()), false));

            const std::vector<uint8_t>& out = client.outbound();
            REQUIRE(MqttParser::isStructurallyValidPublish(out));

            // Decode the Remaining Length field directly from the emitted bytes.
            uint32_t rl = 0;
            size_t rlBytes = 0;
            REQUIRE(MqttParser::decodeRemainingLength(out, 1, rl, rlBytes));
            CHECK(rlBytes == c.expectedRlBytes);

            // Declared Remaining Length equals the actual trailing byte count...
            const size_t trailing = out.size() - (1 + rlBytes);
            CHECK(rl == trailing);
            // ...and equals 2 + topicLen + plength (topic-length prefix + topic + payload).
            CHECK(rl == 2 + topicLen + c.plen);
        }
    }
}
