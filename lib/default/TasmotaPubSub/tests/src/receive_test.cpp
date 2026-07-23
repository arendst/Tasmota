/*
  receive_test.cpp - inbound PUBLISH parsing and callback dispatch
  characterization + hardening for the TasmotaPubSub host test system (task 10.5).

  Inbound PUBLISH bytes are scripted into the MockClient and delivered to the
  *unmodified* library by calling psc.loop() (the callback is registered via
  psc.setCallback; the std::function signature is enabled by the -DESP32 build
  flag, so lambdas with captures can be bound).

  Baseline (TEST_SUITE("baseline")):
    - A well-formed QoS 0 PUBLISH scripted inbound => the registered callback is
      invoked with the correct topic, payload pointer, and payload length
      (Requirement 11.1).
    - A well-formed QoS 1 PUBLISH => the callback is invoked AND a PUBACK is
      recorded outbound echoing the message id (Requirement 11.2).
    - Property 6 (inbound PUBLISH callback round-trip for QoS {0,1}) is folded in
      as a single deterministic, data-driven case (SUBCASE / table loops, no
      randomized generators) over curated topic/payload vectors: empty payload,
      typical, multi-byte/binary, and various topic lengths. The callback
      captures (topic, payload copy, length) for assertion (Requirement 18.5).

  Hardening (TEST_SUITE("hardening")), Property 11 / F-02 + F-09
  (Requirements 11.3-11.6), all expected PASS under AddressSanitizer:
    - A QoS 1 PUBLISH whose claimed topic length exceeds the received packet is
      rejected without reading beyond the received bytes (Req 11.3).
    - A short PUBLISH whose length arithmetic would underflow the payload length
      is rejected without invoking the callback with an underflowed length
      (Req 11.4).
    - A PUBLISH with QoS value 3 is rejected (Req 11.5).
    - A PUBLISH with QoS value 2 is rejected rather than parsed as QoS 0
      (Req 11.6).
  On every rejected packet the callback is NOT invoked, and the library closes
  the connection. Adversarial fixtures are built via MqttPacket / sized
  std::vector<uint8_t> so the test code itself performs no out-of-bounds access;
  AddressSanitizer proves the library reads no bytes beyond those received.

  Every assertion goes through the public API, the registered callback, and
  decoded MockClient.outbound() wire bytes - never private members - so the
  baseline stays durable across a future MQTT 5 migration (Requirement 19.1).
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

// Records every inbound PUBLISH the library dispatches to the callback. The
// callback signature is std::function<void(char*,uint8_t*,unsigned int)> under
// -DESP32, so a lambda capturing this struct by reference can be registered.
struct CallbackCapture {
    int                  count = 0;       // number of callback invocations
    std::string          topic;           // last topic (NUL-terminated C string)
    const uint8_t*       payloadPtr = nullptr;  // last payload pointer as delivered
    unsigned int         length = 0;      // last payload length as delivered
    std::vector<uint8_t> payload;         // copy of the last payload bytes
};

// Connect the client/psc pair with a scripted CONNACK, register a callback that
// records into `cap`, and clear the recorded CONNECT bytes so only the
// packet-under-test remains in outbound().
void connectWithCallback(MockClient& client, PubSubClient& psc, CallbackCapture& cap) {
    client.pushPacket(MqttPacket::connack(0));
    psc.setServer("broker.example", 1883);
    psc.setCallback([&cap](char* topic, uint8_t* payload, unsigned int len) {
        cap.count++;
        cap.topic = topic;                 // library NUL-terminates the topic
        cap.payloadPtr = payload;
        cap.length = len;
        // Copy exactly `len` bytes; for len == 0 this dereferences nothing.
        cap.payload.assign(payload, payload + len);
    });
    REQUIRE(psc.connect("recv-client"));
    REQUIRE(psc.connected());
    client.clearOutbound();
}

// Deterministic binary payload of length n (includes 0x00 bytes so the
// round-trip proves the dispatch is binary-safe and length-driven, not
// dependent on C-string termination).
std::vector<uint8_t> makePayload(size_t n) {
    std::vector<uint8_t> v(n);
    for (size_t i = 0; i < n; ++i) {
        v[i] = static_cast<uint8_t>((i * 31u + 7u) & 0xFFu);
    }
    return v;
}

}  // namespace

TEST_SUITE("baseline") {

    // --- 11.1: well-formed QoS 0 PUBLISH => callback dispatch ---------------

    TEST_CASE("inbound QoS 0 PUBLISH invokes the callback with correct topic/payload/length") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        CallbackCapture cap;
        connectWithCallback(client, psc, cap);

        const std::string topic = "tele/dev/SENSOR";
        const std::string msg = "hello world";
        const std::vector<uint8_t> payload(msg.begin(), msg.end());

        client.pushPacket(MqttPacket::publish(topic, payload, /*qos=*/0));
        REQUIRE(psc.loop());

        // The callback fired exactly once with the fixture topic, a non-null
        // payload pointer, and the exact payload bytes and length.
        CHECK(cap.count == 1);
        CHECK(cap.topic == topic);
        CHECK(cap.payloadPtr != nullptr);
        CHECK(cap.length == payload.size());
        CHECK(cap.payload == payload);

        // A QoS 0 PUBLISH is not acknowledged, so nothing is written back.
        CHECK(client.outbound().empty());
        CHECK(psc.connected());
    }

    // --- 11.2: well-formed QoS 1 PUBLISH => callback + PUBACK ---------------

    TEST_CASE("inbound QoS 1 PUBLISH invokes the callback and records a PUBACK echoing the msgId") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        CallbackCapture cap;
        connectWithCallback(client, psc, cap);

        const std::string topic = "cmnd/dev/POWER";
        const std::string msg = "ON";
        const std::vector<uint8_t> payload(msg.begin(), msg.end());
        const uint16_t msgId = 0x1234;

        client.pushPacket(MqttPacket::publish(topic, payload, /*qos=*/1,
                                              /*retained=*/false, msgId));
        REQUIRE(psc.loop());

        // Callback dispatched with the fixture fields.
        CHECK(cap.count == 1);
        CHECK(cap.topic == topic);
        CHECK(cap.payloadPtr != nullptr);
        CHECK(cap.length == payload.size());
        CHECK(cap.payload == payload);

        // A PUBACK echoing the message id is recorded outbound.
        DecodedPacket ack = MqttParser::decode(client.outbound());
        REQUIRE(ack.valid);
        CHECK(ack.type == static_cast<uint8_t>(MQTTPUBACK));
        CHECK(ack.remainingLength == 2);
        REQUIRE(ack.payload.size() == 2);
        const uint16_t ackMsgId =
            static_cast<uint16_t>((ack.payload[0] << 8) | ack.payload[1]);
        CHECK(ackMsgId == msgId);

        CHECK(psc.connected());
    }

    // --- Property 6: inbound PUBLISH callback round-trip (QoS 0 and 1) -------

    // Feature: tasmota-pubsub-tests, Property 6: for all well-formed inbound
    // PUBLISH fixtures with QoS in {0,1}, arbitrary nonempty topic, and arbitrary
    // payload, the registered callback is invoked exactly once with a topic
    // string equal to the fixture topic and a payload region whose bytes and
    // length equal the fixture payload; and for QoS 1 a PUBACK echoing the
    // fixture message id is recorded outbound. Validated deterministically over a
    // curated table of topic/payload vectors (no randomized generators).
    TEST_CASE("Property 6: inbound PUBLISH callback round-trip over curated topics/payloads") {
        struct Case { const char* label; std::string topic; std::vector<uint8_t> payload; };

        // A multi-byte / UTF-8 topic and payload (degree sign + checkmark).
        const std::string utf8Topic = "tele/\xC2\xB0" "C/\xE2\x9C\x93";
        const std::string utf8PayloadStr = "temp=21\xC2\xB0" "C \xE2\x9C\x93";
        const std::vector<uint8_t> utf8Payload(utf8PayloadStr.begin(), utf8PayloadStr.end());

        std::vector<Case> cases = {
            {"single-char topic, empty payload", "a",              {}},
            {"typical topic, short payload",     "tele/dev/STATE", {'O','N'}},
            {"single-char topic, binary payload","t",              makePayload(64)},
            {"long topic, empty payload",
                 "tele/very/deeply/nested/topic/filter/name",      {}},
            {"long topic, binary payload",
                 "stat/tasmota_ABCDEF/RESULT",                     makePayload(200)},
            {"utf8 topic + utf8 payload",        utf8Topic,        utf8Payload},
        };

        const uint8_t qosValues[] = {0, 1};

        for (const Case& c : cases) {
            for (uint8_t qos : qosValues) {
                CAPTURE(c.label);
                CAPTURE(qos);
                TestClock::instance().reset();
                MockClient client;
                PubSubClient psc(client);
                CallbackCapture cap;
                connectWithCallback(client, psc, cap);

                // A distinct nonzero message id per QoS-1 fixture.
                const uint16_t msgId = static_cast<uint16_t>(0x2000 + c.topic.size());
                client.pushPacket(MqttPacket::publish(c.topic, c.payload, qos,
                                                      /*retained=*/false,
                                                      qos ? msgId : 0));
                REQUIRE(psc.loop());

                // Callback invoked exactly once with the fixture topic/payload.
                CHECK(cap.count == 1);
                CHECK(cap.topic == c.topic);
                CHECK(cap.payloadPtr != nullptr);
                CHECK(cap.length == c.payload.size());
                CHECK(cap.payload == c.payload);

                if (qos == 1) {
                    // QoS 1 => a PUBACK echoing the fixture message id.
                    DecodedPacket ack = MqttParser::decode(client.outbound());
                    REQUIRE(ack.valid);
                    CHECK(ack.type == static_cast<uint8_t>(MQTTPUBACK));
                    CHECK(ack.remainingLength == 2);
                    REQUIRE(ack.payload.size() == 2);
                    const uint16_t ackMsgId =
                        static_cast<uint16_t>((ack.payload[0] << 8) | ack.payload[1]);
                    CHECK(ackMsgId == msgId);
                } else {
                    // QoS 0 => no acknowledgement is written.
                    CHECK(client.outbound().empty());
                }

                CHECK(psc.connected());
            }
        }
    }
}

TEST_SUITE("hardening") {

    // Feature: tasmota-pubsub-tests, Property 11: for all inbound PUBLISH
    // fixtures whose QoS bits encode 2 or 3, or whose claimed topic length
    // exceeds the received packet length, or whose length arithmetic would
    // underflow the payload length, the library rejects the packet without
    // invoking the callback and without reading beyond the received bytes
    // (verified clean under AddressSanitizer).
    //
    // F-02 (Requirement 11.3): a QoS 1 PUBLISH whose claimed topic length exceeds
    // the received packet is rejected. The fixture is a fully well-framed frame
    // (Remaining Length equals the trailing byte count) whose 2-byte topic-length
    // field claims far more bytes than the packet contains, so the library's
    // guard header_len = llen + 3 + tl + 2 > len fires. All reads stay within the
    // scripted bytes, so ASan proves no over-read. F-02 is hardened (expected PASS).
    TEST_CASE("F-02 QoS 1 PUBLISH with claimed topic length exceeding the packet is rejected"
              * FINDING_MARKER(F02)) {
        // Each fixture: fixed header PUBLISH|QoS1 (0x32), a single Remaining
        // Length byte, then `bodyLen` body bytes whose first two bytes are the
        // (oversized) topic-length field. The claimed topic length far exceeds
        // the body, so header_len > len.
        struct Case { uint8_t bodyLen; uint16_t claimedTopicLen; };
        const Case cases[] = {
            {4,  500},     // tl=500 but only 4 body bytes present
            {6,  1000},
            {10, 0x7FFF},  // near-max claimed topic length
            {2,  300},     // only the topic-length field present, no topic bytes
        };

        for (const Case& c : cases) {
            CAPTURE(c.bodyLen);
            CAPTURE(c.claimedTopicLen);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            CallbackCapture cap;
            connectWithCallback(client, psc, cap);

            // Build the adversarial frame in a sized vector (no OOB in test code).
            std::vector<uint8_t> frame;
            frame.push_back(static_cast<uint8_t>(MQTTPUBLISH | MQTTQOS1));  // 0x32
            frame.push_back(c.bodyLen);                                     // RL (< 128)
            frame.push_back(static_cast<uint8_t>(c.claimedTopicLen >> 8));  // topic len hi
            frame.push_back(static_cast<uint8_t>(c.claimedTopicLen & 0xFF));// topic len lo
            // Remaining body filler so the frame length matches the declared RL.
            for (uint8_t i = 2; i < c.bodyLen; ++i) {
                frame.push_back(static_cast<uint8_t>('x'));
            }
            client.pushPacket(MqttPacket::raw(frame));

            // Bound any read to a short virtual timeout as a safety net.
            psc.setSocketTimeout(1);
            psc.loop();

            // The packet is rejected: no callback, connection closed.
            CHECK(cap.count == 0);
            CHECK_FALSE(psc.connected());
            CHECK(client.stopCalled());
        }
    }

    // F-02 (Requirement 11.4): a short PUBLISH whose length arithmetic would
    // underflow the payload length (payload length = len - llen - 3 - tl for
    // QoS 0) must not invoke the callback with an underflowed (huge) length. The
    // library's guard header_len > len rejects the frame before the subtraction,
    // so the callback is never reached. F-02 is hardened (expected PASS).
    TEST_CASE("F-02 short PUBLISH that would underflow the payload length is rejected"
              * FINDING_MARKER(F02)) {
        // QoS 0 frames whose claimed topic length is just large enough that the
        // header (llen + 3 + tl) exceeds the received length: without the guard
        // the payload length len - llen - 3 - tl would wrap to a huge value.
        struct Case { uint8_t bodyLen; uint16_t claimedTopicLen; };
        const Case cases[] = {
            {5,  10},   // len=7, header_len=1+3+10=14 > 7
            {3,  8},    // len=5, header_len=1+3+8=12  > 5
            {2,  5},    // len=4, header_len=1+3+5=9   > 4 (no topic bytes at all)
            {8,  20},   // len=10, header_len=1+3+20=24 > 10
        };

        for (const Case& c : cases) {
            CAPTURE(c.bodyLen);
            CAPTURE(c.claimedTopicLen);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            CallbackCapture cap;
            connectWithCallback(client, psc, cap);

            std::vector<uint8_t> frame;
            frame.push_back(static_cast<uint8_t>(MQTTPUBLISH));             // 0x30, QoS 0
            frame.push_back(c.bodyLen);                                     // RL (< 128)
            frame.push_back(static_cast<uint8_t>(c.claimedTopicLen >> 8));  // topic len hi
            frame.push_back(static_cast<uint8_t>(c.claimedTopicLen & 0xFF));// topic len lo
            for (uint8_t i = 2; i < c.bodyLen; ++i) {
                frame.push_back(static_cast<uint8_t>('y'));
            }
            client.pushPacket(MqttPacket::raw(frame));

            psc.setSocketTimeout(1);
            psc.loop();

            // No callback (so no underflowed length was ever delivered), and the
            // connection is closed.
            CHECK(cap.count == 0);
            CHECK_FALSE(psc.connected());
            CHECK(client.stopCalled());
        }
    }

    // F-09 (Requirement 11.5): a PUBLISH whose QoS bits encode 3 (a protocol
    // violation) is rejected. The fixture is otherwise well-framed (valid topic
    // and payload), so a naive parser would dispatch it; the library rejects
    // qos > 1 up front. F-09 is hardened (expected PASS).
    TEST_CASE("F-09 inbound PUBLISH with QoS 3 is rejected" * FINDING_MARKER(F09)) {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        CallbackCapture cap;
        connectWithCallback(client, psc, cap);

        const std::string topic = "tele/dev/SENSOR";
        const std::vector<uint8_t> payload{'d', 'a', 't', 'a'};
        // qos=3 sets both QoS bits; the builder emits a message id (qos > 0) so
        // the frame is well-formed apart from the illegal QoS.
        client.pushPacket(MqttPacket::publish(topic, payload, /*qos=*/3,
                                              /*retained=*/false, /*msgId=*/0x0007));

        psc.setSocketTimeout(1);
        psc.loop();

        CHECK(cap.count == 0);
        CHECK_FALSE(psc.connected());
        CHECK(client.stopCalled());
    }

    // F-09 (Requirement 11.6): an inbound QoS 2 PUBLISH is rejected rather than
    // parsed as QoS 0. The fixture is a fully well-framed QoS 2 PUBLISH (topic,
    // packet identifier, payload); if the library ignored the QoS bits it would
    // dispatch the callback. Because it rejects qos > 1, the callback never
    // fires. F-09 is hardened (expected PASS).
    TEST_CASE("F-09 inbound PUBLISH with QoS 2 is rejected rather than parsed as QoS 0"
              * FINDING_MARKER(F09)) {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        CallbackCapture cap;
        connectWithCallback(client, psc, cap);

        const std::string topic = "tele/dev/SENSOR";
        const std::vector<uint8_t> payload = makePayload(16);
        client.pushPacket(MqttPacket::publish(topic, payload, /*qos=*/2,
                                              /*retained=*/false, /*msgId=*/0x0042));

        psc.setSocketTimeout(1);
        psc.loop();

        // Not dispatched as QoS 0 (or anything else) - callback never fired.
        CHECK(cap.count == 0);
        CHECK_FALSE(psc.connected());
        CHECK(client.stopCalled());
    }
}
