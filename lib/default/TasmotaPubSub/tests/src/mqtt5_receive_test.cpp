/*
  mqtt5_receive_test.cpp - MQTT 5.0 inbound PUBLISH parsing (task 7.1).

  Feature: tasmota-pubsub-mqtt5

  TEST_SUITE("mqtt5"), body guarded by `#if MQTT_VERSION == MQTT_VERSION_5_0`, so
  the file compiles in both selections and contributes cases only to the 5.0
  binary.

  Scope at task 7.1 is the wire layout and its bounds checks:

    fixed header | TopicName | [PacketIdentifier(2) if QoS>0] | Properties | payload

  The payload is whatever remains after the Properties field, so the delivered
  length is invariant to how large that field is (Requirements 6.4, 6.5, 12.2,
  12.3), and a remainder of 0 is an empty payload rather than an error. Every
  offset is validated against the bytes ACTUALLY RECEIVED before use, so an
  over-declared Property Length rejects the packet through the failure funnel
  without invoking the callback (Requirements 6.7, 13.4, 13.5).

  Task 7.2 adds the accessor cases at the end of the file: `inboundProperties()`
  and `forEachInboundUserProperty()`, which are what make the decoded values
  observable (Requirements 11.4, 11.7, 12.4, 12.5, 12.6).

  These are example cases, deliberately few: the exhaustive sweeps over QoS,
  payload size and property set are Property 9 (task 7.4) and Property 10
  (task 7.5). What is pinned here is that the offsets are right, that an overrun
  is refused, and that the accessors report what was received.

  Task 7.4 adds the Property 9 curated sweep further down: QoS, payload size and
  property-set size varied together, routed through `CallbackContractAdapter` so
  the driver's `mqtt_data[data_len] = 0` write participates in the ASan boundary
  check, plus the exact-buffer-boundary window (mirroring `findings_test.cpp`
  F-01, adapted for the 5.0 layout). Inbound QoS 2 delivery is out of scope until
  task 12's inbound QoS 2 registry lands; today qos > 1 is a bare disconnect with
  no callback, which the QoS 2 case pins as current behavior rather than
  asserting delivery.

  Requirements: 6.4, 6.5, 6.6, 6.7, 11.4, 11.7, 11.11, 12.2, 12.3, 12.4, 12.5,
                12.6, 13.4, 13.5
*/

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "doctest.h"

#include "CallbackContractAdapter.h"
#include "MockClient.h"
#include "MqttPacket.h"
#include "TestClock.h"
#include "PubSubClient.h"

TEST_SUITE("mqtt5") {

#if MQTT_VERSION == MQTT_VERSION_5_0

namespace mqtt5_receive {

// What the callback saw, captured by value so the assertions cannot depend on the
// library buffer still holding the bytes.
struct Capture {
    int count = 0;
    std::string topic;
    std::vector<uint8_t> payload;
    unsigned int length = 0;
};

Capture g_capture;

void callback(char* topic, uint8_t* payload, unsigned int length) {
    g_capture.count++;
    g_capture.topic = (topic != nullptr) ? std::string(topic) : std::string();
    g_capture.length = length;
    g_capture.payload.assign(payload, payload + length);
}

// A connected client with the capturing callback registered. MqttPacket::connack()
// is selection-aware, so this scripts a 5.0 CONNACK here.
void connect(MockClient& client, PubSubClient& psc) {
    g_capture = Capture();
    client.pushPacket(MqttPacket::connack(0));
    psc.setServer("broker.example", 1883);
    psc.setCallback(callback);
    REQUIRE(psc.connect("mqtt5-recv"));
    REQUIRE(psc.connected());
    client.clearOutbound();
    psc.setSocketTimeout(1);
}

// Build a 5.0 inbound PUBLISH by hand: task 7.3 adds MqttPacket::publish5(), which
// takes an arbitrary MqttProps; until then the fixture is assembled here so an
// adversarial Property Length is expressible.
std::vector<uint8_t> publish5(const std::string& topic,
                              const std::vector<uint8_t>& props,
                              const std::vector<uint8_t>& payload,
                              uint8_t qos = 0,
                              uint16_t packetId = 0) {
    std::vector<uint8_t> body;
    body.push_back(static_cast<uint8_t>(topic.size() >> 8));
    body.push_back(static_cast<uint8_t>(topic.size() & 0xFF));
    body.insert(body.end(), topic.begin(), topic.end());
    if (qos > 0) {
        body.push_back(static_cast<uint8_t>(packetId >> 8));
        body.push_back(static_cast<uint8_t>(packetId & 0xFF));
    }
    body.insert(body.end(), props.begin(), props.end());
    body.insert(body.end(), payload.begin(), payload.end());

    std::vector<uint8_t> bytes;
    bytes.push_back(static_cast<uint8_t>(static_cast<uint8_t>(MQTTPUBLISH)
                                         | static_cast<uint8_t>((qos & 0x03) << 1)));
    const std::vector<uint8_t> rl =
        MqttPacket::encodeRemainingLength(static_cast<uint32_t>(body.size()));
    bytes.insert(bytes.end(), rl.begin(), rl.end());
    bytes.insert(bytes.end(), body.begin(), body.end());
    return bytes;
}

}  // namespace mqtt5_receive

// Requirements 6.4, 6.5, 12.2, 12.3: the reported payload starts after the
// Properties field, and its length is the same whether that field is one byte or
// forty - none of its bytes are inside the delivered payload.
TEST_CASE("inbound PUBLISH payload starts after the Properties field, at any field size") {
    const std::string topic = "tele/dev/SENSOR";
    const std::vector<uint8_t> payload{'{', '"', 'a', '"', ':', '1', '}'};

    struct Case { const char* label; std::vector<uint8_t> props; };
    const Case cases[] = {
        {"empty property set", MqttProps().bytes()},
        {"one byte property", MqttProps().byte_(0x01, 1).bytes()},
        {"string + binary + two user properties",
         MqttProps().str(0x03, "application/json")
                    .bin(0x09, std::vector<uint8_t>{0x00, 0x01, 0x02})
                    .pair(0x26, "k1", "v1")
                    .pair(0x26, "k2", "v2")
                    .bytes()},
    };

    for (const Case& c : cases) {
        CAPTURE(c.label);
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        mqtt5_receive::connect(client, psc);

        client.pushPacket(MqttPacket::raw(mqtt5_receive::publish5(topic, c.props, payload)));
        REQUIRE(psc.loop());

        REQUIRE(mqtt5_receive::g_capture.count == 1);
        CHECK(mqtt5_receive::g_capture.topic == topic);          // NUL-terminated topic
        CHECK(mqtt5_receive::g_capture.length == payload.size());
        CHECK(mqtt5_receive::g_capture.payload == payload);
        CHECK(psc.connected());
        CHECK_FALSE(client.stopCalled());
    }
}

// Requirement 6.4: nothing after the Properties field is an EMPTY payload, not an
// error - at QoS 0 and at QoS 1, where the Packet Identifier sits in between.
TEST_CASE("inbound PUBLISH with no bytes after the Properties field is an empty payload") {
    const std::string topic = "cmnd/dev/POWER";

    SUBCASE("QoS 0") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        mqtt5_receive::connect(client, psc);

        client.pushPacket(MqttPacket::raw(
            mqtt5_receive::publish5(topic, MqttProps().bytes(), {})));
        REQUIRE(psc.loop());

        REQUIRE(mqtt5_receive::g_capture.count == 1);
        CHECK(mqtt5_receive::g_capture.topic == topic);
        CHECK(mqtt5_receive::g_capture.length == 0);
        CHECK(psc.connected());
    }

    SUBCASE("QoS 1, and the PUBACK still echoes the identifier") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        mqtt5_receive::connect(client, psc);

        client.pushPacket(MqttPacket::raw(
            mqtt5_receive::publish5(topic, MqttProps().byte_(0x01, 0).bytes(), {},
                                    /*qos=*/1, /*packetId=*/0x1234)));
        REQUIRE(psc.loop());

        REQUIRE(mqtt5_receive::g_capture.count == 1);
        CHECK(mqtt5_receive::g_capture.length == 0);
        const std::vector<uint8_t> out = client.outbound();
        REQUIRE(out.size() >= 4);
        CHECK(out[0] == static_cast<uint8_t>(MQTTPUBACK));
        CHECK(out[2] == 0x12);
        CHECK(out[3] == 0x34);
        CHECK(psc.connected());
    }
}

// Requirements 6.7, 13.4, 13.5: a Property Length that would require reading past
// the received bytes rejects the packet - no callback, failure reported, closed -
// and the failure goes out through the funnel as `E0 01 81`.
TEST_CASE("inbound PUBLISH whose Property Length overruns the received bytes is rejected") {
    const std::string topic = "tele/dev/SENSOR";

    struct Case { const char* label; std::vector<uint8_t> props; };
    const Case cases[] = {
        // Declares 40 body bytes, carries 3.
        {"over-declared length", MqttProps().byte_(0x01, 1).rawLength(40).bytes()},
        // Declares the whole 16,383 the codec can express.
        {"maximum declared length", MqttProps().byte_(0x01, 1).rawLength(16383).bytes()},
        // A Property Length VBI that never terminates within 2 bytes.
        {"non-terminating length VBI", std::vector<uint8_t>{0x80, 0x80}},
    };

    for (const Case& c : cases) {
        CAPTURE(c.label);
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        mqtt5_receive::connect(client, psc);

        client.pushPacket(MqttPacket::raw(
            mqtt5_receive::publish5(topic, c.props, std::vector<uint8_t>{'x'})));
        CHECK_FALSE(psc.loop());

        CHECK(mqtt5_receive::g_capture.count == 0);
        CHECK(psc.lastError() == MqttError::MalformedPacket);
        CHECK(psc.lastReasonCode() == 0x81);
        CHECK_FALSE(psc.connected());
        CHECK(client.stopCalled());

        const std::vector<uint8_t> out = client.outbound();
        const std::vector<uint8_t> expected{static_cast<uint8_t>(MQTTDISCONNECT), 0x01, 0x81};
        CHECK(out == expected);
    }
}

// Requirement 3.7 in the inbound direction: an identifier that is not legal for a
// PUBLISH is a Protocol Error, so the reason code is 0x82 rather than 0x81. Server
// Keep Alive (0x13) is a CONNACK-only property.
TEST_CASE("inbound PUBLISH carrying a property illegal for PUBLISH is a protocol error") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_receive::connect(client, psc);

    client.pushPacket(MqttPacket::raw(
        mqtt5_receive::publish5("tele/dev/SENSOR", MqttProps().u16(0x13, 30).bytes(),
                                std::vector<uint8_t>{'x'})));
    CHECK_FALSE(psc.loop());

    CHECK(mqtt5_receive::g_capture.count == 0);
    CHECK(psc.lastError() == MqttError::ProtocolError);
    CHECK(psc.lastReasonCode() == 0x82);
    CHECK_FALSE(psc.connected());
}

// Requirements 3.10, 11.11, 11.12, 13.7, 13.8: a Correlation Data value whose
// DECLARED length is greater than MQTT_CORRELATION_DATA_MAX (16) is a protocol
// error, not a truncation and not a silent drop - the callback is never invoked,
// the failure is reported through lastError()/lastReasonCode(), and the funnel
// emits the `E0 01 82` DISCONNECT before closing. 17 is one byte over the cap; 255
// is comfortably clear of it in the 1-byte-declared-length range this fixture uses.
TEST_CASE("inbound PUBLISH with over-cap Correlation Data is a protocol error") {
    const std::string topic = "tele/dev/SENSOR";

    struct Case { const char* label; uint16_t declaredLen; };
    const Case cases[] = {
        {"one byte over the cap", 17},
        {"comfortably over the cap", 255},
    };

    for (const Case& c : cases) {
        CAPTURE(c.label);
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        mqtt5_receive::connect(client, psc);

        client.pushPacket(MqttPacket::raw(
            mqtt5_receive::publish5(topic,
                MqttProps().bin(0x09, std::vector<uint8_t>(c.declaredLen, 0xAB)).bytes(),
                std::vector<uint8_t>{'x'})));
        CHECK_FALSE(psc.loop());

        CHECK(mqtt5_receive::g_capture.count == 0);
        CHECK(psc.lastError() == MqttError::ProtocolError);
        CHECK(psc.lastReasonCode() == 0x82);
        CHECK_FALSE(psc.connected());
        CHECK(client.stopCalled());

        const std::vector<uint8_t> out = client.outbound();
        const std::vector<uint8_t> expected{static_cast<uint8_t>(MQTTDISCONNECT), 0x01, 0x82};
        CHECK(out == expected);
    }
}

// Requirements 3.11, 16.5, 16.6: Authentication Method (0x15) and Authentication
// Data (0x16) are rejected at identifier dispatch in ANY packet type, before the
// value's encoding is even consulted - PUBLISH included, with no per-context
// reasoning. The callback is never invoked, the value is never decoded, and the
// DISCONNECT carries the more specific Reason Code 0x8C (Bad authentication
// method) rather than the generic 0x82 an ordinary illegal-for-PUBLISH property
// gets (contrast with the Server Keep Alive case just above).
TEST_CASE("inbound PUBLISH carrying an Enhanced Authentication property is a protocol error with 0x8C") {
    struct Case { const char* label; std::vector<uint8_t> props; };
    const Case cases[] = {
        {"Authentication Method (0x15)", MqttProps().str(0x15, "SCRAM-SHA-1").bytes()},
        // 0x16 is Binary Data on the wire; this is what proves it never reaches the
        // Binary decode path, leaving Correlation Data (0x09) as the only Binary
        // identifier this client ever decodes.
        {"Authentication Data (0x16)", MqttProps().bin(0x16, {0x01, 0x00, 0x02}).bytes()},
    };

    for (const Case& c : cases) {
        CAPTURE(c.label);
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        mqtt5_receive::connect(client, psc);

        client.pushPacket(MqttPacket::raw(
            mqtt5_receive::publish5("tele/dev/SENSOR", c.props, std::vector<uint8_t>{'x'})));
        CHECK_FALSE(psc.loop());

        CHECK(mqtt5_receive::g_capture.count == 0);
        CHECK(psc.lastError() == MqttError::ProtocolError);
        CHECK(psc.lastReasonCode() == 0x8C);
        CHECK_FALSE(psc.connected());
        CHECK(client.stopCalled());

        const std::vector<uint8_t> out = client.outbound();
        const std::vector<uint8_t> expected{static_cast<uint8_t>(MQTTDISCONNECT), 0x01, 0x8C};
        CHECK(out == expected);
    }
}

// Requirement 8.13: QoS 3 is a malformed fixed header, and in 5.0 mode it leaves
// through the failure funnel - `E0 01 81`, then close - with no callback.
TEST_CASE("inbound PUBLISH with QoS 3 fails as a malformed packet") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_receive::connect(client, psc);

    client.pushPacket(MqttPacket::raw(
        mqtt5_receive::publish5("tele/dev/SENSOR", MqttProps().bytes(),
                                std::vector<uint8_t>{'x'}, /*qos=*/3, /*packetId=*/7)));
    CHECK_FALSE(psc.loop());

    CHECK(mqtt5_receive::g_capture.count == 0);
    CHECK(psc.lastError() == MqttError::MalformedPacket);
    CHECK(psc.lastReasonCode() == 0x81);
    CHECK_FALSE(psc.connected());
    const std::vector<uint8_t> out = client.outbound();
    const std::vector<uint8_t> expected{static_cast<uint8_t>(MQTTDISCONNECT), 0x01, 0x81};
    CHECK(out == expected);
}

// --- Property 9: inbound PUBLISH delivery round-trip and buffer contract --
//
// task 7.4. Task 7.1 above pins the offsets with a handful of example cases;
// this is the curated sweep the design calls Property 9: QoS, payload size and
// property-set size are varied together, delivery is routed through
// `CallbackContractAdapter` so the driver's `mqtt_data[data_len] = 0` write
// participates in the ASan boundary check, and a second sweep drives the exact
// buffer-boundary window the way `findings_test.cpp` F-01 does for 3.1.1,
// adapted for the 5.0 wire layout (topic + optional Packet Identifier +
// Properties + payload).
//
// QoS 2 caveat: inbound QoS 2 delivery (the inbound QoS 2 registry) is task 12
// and does not exist yet. Today `loop()` routes qos > 1 to a bare
// `_state = MQTT_DISCONNECTED; _client->stop();` with no callback, for BOTH QoS
// 2 and QoS 3 (QoS 3 additionally goes through `protocolFailure` in 5.0 mode -
// see the QoS 3 case above). So "arbitrary QoS in 0..2" is swept as delivery
// assertions at QoS 0 and QoS 1 here, and QoS 2 gets its own case pinning the
// CURRENT documented behavior (disconnect, no callback) rather than a delivery
// assertion that would be false today. That case is expected to be replaced by
// a real QoS 2 delivery case when task 12 lands.
namespace mqtt5_receive {

// Deterministic payload of length n, including 0x00 bytes so a round-trip
// proves binary-safety rather than C-string termination (mirrors
// findings_test.cpp's makePayload).
std::vector<uint8_t> makePayload(size_t n) {
    std::vector<uint8_t> v(n);
    for (size_t i = 0; i < n; ++i) {
        v[i] = static_cast<uint8_t>((i * 31u + 7u) & 0xFFu);
    }
    return v;
}

// A "large" property set: a UTF-8 string, Binary Data with an embedded 0x00,
// and two User Properties - big enough that its size clearly dwarfs the
// payload for the smallest cases, proving the reported length is invariant to
// the Properties field's size (Requirements 6.4, 6.5, 12.2, 12.3).
MqttProps largeProps() {
    return MqttProps().str(0x03, "application/json")
                      .bin(0x09, std::vector<uint8_t>{0x00, 0x01, 0x02, 0x00})
                      .pair(0x26, "k1", "v1")
                      .pair(0x26, "k2", "v2");
}

// Computes the payload length that makes a 5.0 PUBLISH (topic length `tl`,
// the given `qos` and `props`) encode to exactly `desiredTotal` wire bytes.
// Tries both Remaining Length size classes (1 byte for RL 0..127, 2 bytes for
// RL 128..16383) and returns whichever is internally consistent - i.e. the
// class assumed to compute `payloadLen` is the same class the resulting RL
// actually falls into. Returns -1 if no consistent payload length exists (the
// desired total is smaller than the fixed overhead can produce).
int payloadLenForTotal(size_t tl, uint8_t qos, const MqttProps& props, int desiredTotal) {
    const int propsLen = static_cast<int>(props.bytes().size());
    const int variableHeaderRest =
        2 + static_cast<int>(tl) + (qos > 0 ? 2 : 0) + propsLen;
    for (int llen = 1; llen <= 2; ++llen) {
        const int payloadLen = desiredTotal - 1 - llen - variableHeaderRest;
        if (payloadLen < 0) { continue; }
        const int rl = variableHeaderRest + payloadLen;
        const int expectedLlen = (rl <= 127) ? 1 : ((rl <= 16383) ? 2 : -1);
        if (expectedLlen == llen) { return payloadLen; }
    }
    return -1;
}

}  // namespace mqtt5_receive

// Requirements 6.4, 6.5, 6.6, 12.2, 12.3: over QoS 0 and QoS 1, payload sizes
// spanning 0, 1, 127 and 128, and property sets from empty to large, the
// callback (here CallbackContractAdapter, so the boundary write is real) sees
// a NUL-terminated topic equal to the Topic Name, a payload equal to the
// packet's application payload, and a length equal to that payload's byte
// count - independent of how large the Properties field was.
TEST_CASE("Property 9: inbound PUBLISH delivery round-trips topic/payload/length "
          "through CallbackContractAdapter across QoS, payload size and property "
          "set") {
    const std::string topic = "tele/dev/SENSOR";

    struct Case {
        const char* label;
        uint8_t qos;
        size_t payloadLen;
        MqttProps props;
    };
    const Case cases[] = {
        {"QoS 0, empty payload, empty properties", 0, 0, MqttProps()},
        {"QoS 0, 1-byte payload, one property", 0, 1, MqttProps().byte_(0x01, 1)},
        {"QoS 0, 127-byte payload, large properties", 0, 127, mqtt5_receive::largeProps()},
        {"QoS 0, 128-byte payload, empty properties", 0, 128, MqttProps()},
        {"QoS 1, empty payload, large properties", 1, 0, mqtt5_receive::largeProps()},
        {"QoS 1, 1-byte payload, empty properties", 1, 1, MqttProps()},
        {"QoS 1, 127-byte payload, one property", 1, 127, MqttProps().byte_(0x01, 0)},
        {"QoS 1, 128-byte payload, large properties", 1, 128, mqtt5_receive::largeProps()},
    };

    for (const Case& c : cases) {
        CAPTURE(c.label);
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);

        CallbackContractAdapter adapter;
        client.pushPacket(MqttPacket::connack(0));
        psc.setServer("broker.example", 1883);
        psc.setCallback(adapter.callback());
        REQUIRE(psc.connect("mqtt5-recv-p9"));
        REQUIRE(psc.connected());
        client.clearOutbound();
        psc.setSocketTimeout(1);

        const std::vector<uint8_t> payload = mqtt5_receive::makePayload(c.payloadLen);
        const uint16_t packetId = (c.qos > 0) ? 0xABCD : 0;
        const MqttPacket pkt = MqttPacket::publish5(topic, payload, c.qos,
                                                    /*retained=*/false, /*dup=*/false,
                                                    packetId, c.props);
        client.pushPacket(pkt);
        REQUIRE(psc.loop());

        REQUIRE(adapter.count() == 1);
        CHECK(adapter.last().topic == topic);
        CHECK(adapter.last().length == c.payloadLen);
        CHECK(adapter.last().payload == payload);
        CHECK(psc.connected());
        CHECK_FALSE(client.stopCalled());

        if (c.qos == 1) {
            // The PUBACK still went out, matching the identifier - delivery and
            // acknowledgement are independent of the property set's size.
            const std::vector<uint8_t> out = client.outbound();
            REQUIRE(out.size() >= 4);
            CHECK(out[0] == static_cast<uint8_t>(MQTTPUBACK));
            CHECK(out[2] == 0xAB);
            CHECK(out[3] == 0xCD);
        }
    }
}

// Requirement 8.10, 8.11, 8.15: an inbound QoS 2 PUBLISH is delivered only on
// first receipt, and every receipt is answered with a PUBREC carrying the same identifier.
// Its matching PUBREL receives PUBCOMP and releases the identifier, while a duplicate's
// DUP bit never changes the exactly-once delivery rule.
TEST_CASE("inbound QoS 2 delivers once, re-sends PUBREC for duplicates, and completes on PUBREL") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);

    CallbackContractAdapter adapter;
    client.pushPacket(MqttPacket::connack(0));
    psc.setServer("broker.example", 1883);
    psc.setCallback(adapter.callback());
    REQUIRE(psc.connect("mqtt5-recv-qos2"));
    REQUIRE(psc.connected());
    client.clearOutbound();
    psc.setSocketTimeout(1);

    const uint16_t packetId = 0x1234;
    const std::vector<uint8_t> payload{'x'};
    client.pushPacket(MqttPacket::publish5("tele/dev/SENSOR", payload,
                                           /*qos=*/2, /*retained=*/false, /*dup=*/false,
                                           packetId, MqttProps()));
    REQUIRE(psc.loop());
    REQUIRE(adapter.count() == 1);
    CHECK(adapter.last().payload == payload);
    DecodedAck5 ack = MqttParser::decodeAck5(client.outbound(), MQTTPUBREC);
    REQUIRE(ack.valid);
    CHECK(ack.form == 0);
    CHECK(ack.packetId == packetId);
    CHECK(psc.connected());

    // Repeated deliveries, with either DUP-bit value, are protocol retries rather than
    // new application messages. Both get another PUBREC and neither reaches the callback.
    for (const bool dup : {false, true}) {
        CAPTURE(dup);
        client.clearOutbound();
        client.pushPacket(MqttPacket::publish5("tele/dev/SENSOR", payload,
                                               /*qos=*/2, /*retained=*/false, dup,
                                               packetId, MqttProps()));
        REQUIRE(psc.loop());
        CHECK(adapter.count() == 1);
        ack = MqttParser::decodeAck5(client.outbound(), MQTTPUBREC);
        REQUIRE(ack.valid);
        CHECK(ack.packetId == packetId);
    }

    client.clearOutbound();
    client.pushPacket(MqttPacket::ack5(MQTTPUBREL, packetId, -1));
    REQUIRE(psc.loop());
    ack = MqttParser::decodeAck5(client.outbound(), MQTTPUBCOMP);
    REQUIRE(ack.valid);
    CHECK(ack.form == 0);
    CHECK(ack.packetId == packetId);
    CHECK(adapter.count() == 1);
    CHECK(psc.connected());
}

TEST_CASE("inbound QoS 2 registry follows persistent Session Present disposition") {
    bool sessionPresent = true;
    bool repeatPublish = false;
    SUBCASE("Session Present 1 suppresses a duplicate PUBLISH before completing PUBREL") {
        sessionPresent = true;
        repeatPublish = true;
    }
    SUBCASE("Session Present 1 completes a resumed exchange directly on PUBREL") {
        sessionPresent = true;
        repeatPublish = false;
    }
    SUBCASE("Session Present 0 clears the old identifier and delivers it in the new session") {
        sessionPresent = false;
        repeatPublish = true;
    }

    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    CallbackContractAdapter adapter;
    psc.setServer("broker.example", 1883);
    psc.setCallback(adapter.callback());
    psc.setKeepAlive(0);

    const uint16_t packetId = 0x3456;
    const std::vector<uint8_t> payload{'p'};
    client.pushPacket(MqttPacket::connack5(0x00, false));
    REQUIRE(psc.connect("mqtt5-recv-session", nullptr, nullptr, nullptr, 0, false,
                        nullptr, /*cleanSession=*/false));
    client.clearOutbound();

    client.pushPacket(MqttPacket::publish5("tele/dev/SENSOR", payload,
                                           /*qos=*/2, /*retained=*/false, /*dup=*/false,
                                           packetId));
    REQUIRE(psc.loop());
    REQUIRE(adapter.count() == 1);
    DecodedAck5 ack = MqttParser::decodeAck5(client.outbound(), MQTTPUBREC);
    REQUIRE(ack.valid);
    CHECK(ack.packetId == packetId);

    psc.disconnect();
    REQUIRE_FALSE(psc.connected());
    client.clearInbound();
    client.clearOutbound();
    client.pushPacket(MqttPacket::connack5(0x00, sessionPresent));
    REQUIRE(psc.connect("mqtt5-recv-session", nullptr, nullptr, nullptr, 0, false,
                        nullptr, /*cleanSession=*/false));
    CHECK(psc.sessionPresent() == sessionPresent);
    client.clearOutbound();

    if (repeatPublish) {
        client.pushPacket(MqttPacket::publish5("tele/dev/SENSOR", payload,
                                               /*qos=*/2, /*retained=*/false, /*dup=*/true,
                                               packetId));
        REQUIRE(psc.loop());
        CHECK(adapter.count() == (sessionPresent ? 1 : 2));
        ack = MqttParser::decodeAck5(client.outbound(), MQTTPUBREC);
        REQUIRE(ack.valid);
        CHECK(ack.packetId == packetId);
        client.clearOutbound();
    }

    client.pushPacket(MqttPacket::ack5(MQTTPUBREL, packetId, -1));
    REQUIRE(psc.loop());
    ack = MqttParser::decodeAck5(client.outbound(), MQTTPUBCOMP);
    REQUIRE(ack.valid);
    CHECK(ack.packetId == packetId);
    CHECK(adapter.count() == (sessionPresent ? 1 : 2));
}

TEST_CASE("zero Session Expiry clears state on transport loss and rejects Session Present") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    CallbackContractAdapter adapter;
    psc.setServer("broker.example", 1883);
    psc.setCallback(adapter.callback());
    psc.setKeepAlive(0);
    psc.setSessionExpiryInterval(0);

    const uint16_t packetId = 0x4567;
    const std::vector<uint8_t> payload{'z'};
    client.pushPacket(MqttPacket::connack5(0x00, false));
    REQUIRE(psc.connect("mqtt5-recv-expiry-zero", nullptr, nullptr, nullptr, 0, false,
                        nullptr, /*cleanSession=*/false));
    client.clearOutbound();
    client.pushPacket(MqttPacket::publish5("tele/dev/SENSOR", payload,
                                           /*qos=*/2, /*retained=*/false, /*dup=*/false,
                                           packetId));
    REQUIRE(psc.loop());
    REQUIRE(adapter.count() == 1);

    client.setConnected(false);
    CHECK_FALSE(psc.connected());
    CHECK(psc.state() == MQTT_CONNECTION_LOST);
    client.clearInbound();
    client.clearOutbound();

    client.pushPacket(MqttPacket::connack5(0x00, true));
    CHECK_FALSE(psc.connect("mqtt5-recv-expiry-zero", nullptr, nullptr, nullptr, 0, false,
                            nullptr, /*cleanSession=*/false));
    CHECK(psc.lastError() == MqttError::ProtocolError);
    CHECK_FALSE(psc.sessionPresent());

    client.clearInbound();
    client.clearOutbound();
    client.pushPacket(MqttPacket::connack5(0x00, false));
    REQUIRE(psc.connect("mqtt5-recv-expiry-zero", nullptr, nullptr, nullptr, 0, false,
                        nullptr, /*cleanSession=*/false));
    client.clearOutbound();
    client.pushPacket(MqttPacket::publish5("tele/dev/SENSOR", payload,
                                           /*qos=*/2, /*retained=*/false, /*dup=*/true,
                                           packetId));
    REQUIRE(psc.loop());
    CHECK(adapter.count() == 2);
    const DecodedAck5 ack = MqttParser::decodeAck5(client.outbound(), MQTTPUBREC);
    REQUIRE(ack.valid);
    CHECK(ack.packetId == packetId);
}

TEST_CASE("sessions without a resumable identity clear state on transport loss") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    CallbackContractAdapter adapter;
    psc.setServer("broker.example", 1883);
    psc.setCallback(adapter.callback());
    psc.setKeepAlive(0);

    client.pushPacket(MqttPacket::connack5(0x00, false));
    REQUIRE(psc.connect("mqtt5-recv-no-identity"));

    REQUIRE(psc.publish("tele/outbound", reinterpret_cast<const uint8_t*>("q"),
                        1, false, 1));
    REQUIRE(psc.inFlightCount() == 1);
    client.clearOutbound();
    client.pushPacket(MqttPacket::publish5("tele/inbound", {'i'}, /*qos=*/2,
                                           /*retained=*/false, /*dup=*/false, 0x5678));
    REQUIRE(psc.loop());
    REQUIRE(adapter.count() == 1);

    client.setConnected(false);
    CHECK_FALSE(psc.connected());
    CHECK(psc.inFlightCount() == 0);

    client.clearInbound();
    client.clearOutbound();
    client.pushPacket(MqttPacket::connack5(0x00, true));
    CHECK_FALSE(psc.connect("mqtt5-recv-no-identity", nullptr, nullptr, nullptr, 0, false,
                            nullptr, /*cleanSession=*/false));
    CHECK(psc.lastError() == MqttError::ProtocolError);
}

// Feature: tasmota-pubsub-mqtt5, Property 15: For all inbound PUBLISH sequences,
// a QoS 0 packet is delivered exactly once with no acknowledgement; a QoS 1 packet is
// delivered exactly once and answered with a PUBACK carrying its identifier; and for
// all repetition counts and DUP flag values, a QoS 2 identifier is delivered to the
// callback exactly once no matter how many times its PUBLISH is repeated, each
// repetition producing a further PUBREC with that identifier, with a PUBCOMP emitted
// on PUBREL.
//
// Property 15: Inbound QoS delivery is exactly-once at every QoS level
TEST_CASE("Property 15: inbound QoS delivery is exactly-once at every QoS level") {
    const std::string topic = "tele/dev/SENSOR";
    const std::vector<uint8_t> payload{'q', 'o', 's'};
    const uint16_t packetIds[] = {0x0001, 0x007F, 0x0080, 0xFFFF};

    // QoS 0 has no Packet Identifier and therefore no acknowledgement. It still reaches
    // the application exactly once.
    {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        mqtt5_receive::connect(client, psc);

        client.pushPacket(MqttPacket::publish5(topic, payload, /*qos=*/0));
        REQUIRE(psc.loop());
        CHECK(mqtt5_receive::g_capture.count == 1);
        CHECK(mqtt5_receive::g_capture.topic == topic);
        CHECK(mqtt5_receive::g_capture.payload == payload);
        CHECK(client.outbound().empty());
        CHECK(psc.connected());
    }

    // QoS 1 delivers once and returns a short-form PUBACK for every boundary identifier.
    for (const uint16_t packetId : packetIds) {
        CAPTURE(packetId);
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        mqtt5_receive::connect(client, psc);

        client.pushPacket(MqttPacket::publish5(topic, payload, /*qos=*/1,
                                               /*retained=*/false, /*dup=*/false,
                                               packetId));
        REQUIRE(psc.loop());
        CHECK(mqtt5_receive::g_capture.count == 1);
        CHECK(mqtt5_receive::g_capture.topic == topic);
        CHECK(mqtt5_receive::g_capture.payload == payload);
        const DecodedAck5 ack = MqttParser::decodeAck5(client.outbound(), MQTTPUBACK);
        REQUIRE(ack.valid);
        CHECK(ack.form == 0);
        CHECK(ack.packetId == packetId);
        CHECK(psc.connected());
    }

    // QoS 2 holds the identifier until PUBREL. Every PUBLISH receipt gets a PUBREC, but
    // only the first reaches the application, irrespective of repeat count or DUP bit.
    for (const uint16_t packetId : packetIds) {
        for (const uint8_t repeats : {1, 2, 3, 4}) {
            for (const bool dup : {false, true}) {
                CAPTURE(packetId);
                CAPTURE(repeats);
                CAPTURE(dup);
                TestClock::instance().reset();
                MockClient client;
                PubSubClient psc(client);
                mqtt5_receive::connect(client, psc);

                for (uint8_t receipt = 0; receipt < repeats; ++receipt) {
                    client.clearOutbound();
                    client.pushPacket(MqttPacket::publish5(topic, payload, /*qos=*/2,
                                                           /*retained=*/false, dup,
                                                           packetId));
                    REQUIRE(psc.loop());
                    CHECK(mqtt5_receive::g_capture.count == 1);
                    const DecodedAck5 ack =
                        MqttParser::decodeAck5(client.outbound(), MQTTPUBREC);
                    REQUIRE(ack.valid);
                    CHECK(ack.form == 0);
                    CHECK(ack.packetId == packetId);
                    CHECK(psc.connected());
                }

                client.clearOutbound();
                client.pushPacket(MqttPacket::ack5(MQTTPUBREL, packetId, -1));
                REQUIRE(psc.loop());
                const DecodedAck5 ack = MqttParser::decodeAck5(client.outbound(), MQTTPUBCOMP);
                REQUIRE(ack.valid);
                CHECK(ack.form == 0);
                CHECK(ack.packetId == packetId);
                CHECK(mqtt5_receive::g_capture.count == 1);
                CHECK(psc.connected());
            }
        }
    }
}

// Requirement 8.15: the fixed inbound registry is the Receive Maximum the client
// declared in CONNECT. A new identifier beyond that capacity is a broker violation,
// so it is rejected before callback delivery with a 0x93 DISCONNECT.
TEST_CASE("an inbound QoS 2 registry overflow is a Receive Maximum protocol failure") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);

    CallbackContractAdapter adapter;
    client.pushPacket(MqttPacket::connack(0));
    psc.setServer("broker.example", 1883);
    psc.setCallback(adapter.callback());
    REQUIRE(psc.connect("mqtt5-recv-qos2-full"));
    REQUIRE(psc.connected());
    client.clearOutbound();

    for (uint16_t id = 1; id <= MQTT_MAX_INBOUND_INFLIGHT; ++id) {
        client.pushPacket(MqttPacket::publish5("tele/dev/SENSOR", {'x'},
                                               /*qos=*/2, /*retained=*/false, /*dup=*/false,
                                               id, MqttProps()));
        REQUIRE(psc.loop());
        CHECK(adapter.count() == id);
        const DecodedAck5 ack = MqttParser::decodeAck5(client.outbound(), MQTTPUBREC);
        REQUIRE(ack.valid);
        CHECK(ack.packetId == id);
        client.clearOutbound();
    }

    client.pushPacket(MqttPacket::publish5("tele/dev/SENSOR", {'x'},
                                           /*qos=*/2, /*retained=*/false, /*dup=*/false,
                                           static_cast<uint16_t>(MQTT_MAX_INBOUND_INFLIGHT + 1),
                                           MqttProps()));
    CHECK_FALSE(psc.loop());
    CHECK(adapter.count() == MQTT_MAX_INBOUND_INFLIGHT);
    CHECK(psc.lastError() == MqttError::ProtocolError);
    CHECK(psc.lastReasonCode() == PubSubClient::MQTT_RC_RECEIVE_MAX_EXCEEDED);
    CHECK_FALSE(psc.connected());
    CHECK(client.stopCalled());
    const std::vector<uint8_t> expected{static_cast<uint8_t>(MQTTDISCONNECT), 0x01,
                                        PubSubClient::MQTT_RC_RECEIVE_MAX_EXCEEDED};
    CHECK(client.outbound() == expected);
}

// Requirements 6.4, 6.5, 6.6, 12.2, 12.3, buffer contract half: mirrors
// findings_test.cpp F-01 for the 5.0 wire layout (the frozen F-01 case itself
// is excluded from the v5 build per the task 7.1 scope amendment, precisely
// because its fixture is 3.1.1-shaped). At the exact buffer-boundary window -
// total wire size bufferSize-1, bufferSize and bufferSize+1 - the accepted
// case must deliver through CallbackContractAdapter with the real
// `mqtt_data[data_len] = 0` write landing in-bounds under ASan, and the two
// rejected cases must produce no callback and a closed connection. Swept
// across QoS 0/1 and empty/large property sets so the boundary math is proven
// for both PUBLISH layouts (with and without a Packet Identifier) and is not
// an artifact of an unusually small Properties field.
TEST_CASE("Property 9: inbound PUBLISH stays within the allocated buffer at the "
          "exact-buffer boundary (MQTT 5.0 layout)") {
    const uint16_t kBufferSize = 200;
    const std::string topic = "t";   // topic length 1, so the overhead is minimal and explicit

    struct Case { const char* label; uint8_t qos; MqttProps props; };
    const Case cases[] = {
        {"QoS 0, empty properties", 0, MqttProps()},
        {"QoS 1, empty properties", 1, MqttProps()},
        {"QoS 0, large properties", 0, mqtt5_receive::largeProps()},
        {"QoS 1, large properties", 1, mqtt5_receive::largeProps()},
    };

    struct Boundary { const char* label; int totalDelta; bool accepted; };
    const Boundary boundaries[] = {
        {"bufferSize-1 (accepted)", -1, true},
        {"bufferSize   (rejected)",  0, false},
        {"bufferSize+1 (rejected)", +1, false},
    };

    for (const Case& c : cases) {
        for (const Boundary& b : boundaries) {
            CAPTURE(c.label);
            CAPTURE(b.label);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);

            CallbackContractAdapter adapter;
            client.pushPacket(MqttPacket::connack(0));
            psc.setServer("broker.example", 1883);
            psc.setCallback(adapter.callback());
            REQUIRE(psc.connect("mqtt5-recv-p9-boundary"));
            REQUIRE(psc.connected());
            client.clearOutbound();

            REQUIRE(psc.setBufferSize(kBufferSize));
            REQUIRE(psc.getBufferSize() == kBufferSize);
            psc.setSocketTimeout(1);

            const int total = static_cast<int>(kBufferSize) + b.totalDelta;
            const int payloadLen =
                mqtt5_receive::payloadLenForTotal(topic.size(), c.qos, c.props, total);
            REQUIRE(payloadLen >= 0);
            const std::vector<uint8_t> payload =
                mqtt5_receive::makePayload(static_cast<size_t>(payloadLen));
            const uint16_t packetId = (c.qos > 0) ? 0x1234 : 0;

            const MqttPacket pkt = MqttPacket::publish5(topic, payload, c.qos,
                                                        /*retained=*/false, /*dup=*/false,
                                                        packetId, c.props);
            REQUIRE(pkt.size() == static_cast<size_t>(total));   // fixture is exact
            client.pushPacket(pkt);

            psc.loop();

            if (b.accepted) {
                REQUIRE(adapter.count() == 1);
                CHECK(adapter.last().length == static_cast<unsigned int>(payloadLen));
                CHECK(adapter.last().payload == payload);
                CHECK(psc.connected());
                CHECK_FALSE(client.stopCalled());
            } else {
                CHECK(adapter.count() == 0);
                CHECK_FALSE(psc.connected());
                CHECK(client.stopCalled());
            }
        }
    }
}

// --- Inbound property accessors (task 7.2) --------------------------------
//
// Example cases for `inboundProperties()` and `forEachInboundUserProperty()`. The
// exhaustive sweep over property sets is Property 10 (task 7.5); what is pinned
// here is the no-data indication, the valid-but-empty case, the borrowed values of
// a populated set, and that the User Property walk is a re-walk of the buffer
// rather than a fixed-size cache.
namespace mqtt5_receive {

// Where a visitor records what it saw. Copied out by value, so the assertions do
// not depend on the library buffer still holding the bytes.
struct UserProps {
    std::vector<std::pair<std::string, std::string>> seen;
    int stopAfter = -1;              // -1 = never abort
};

bool collectUserProps(MqttStringView key, MqttStringView value, void* ctx) {
    UserProps* out = static_cast<UserProps*>(ctx);
    out->seen.emplace_back(std::string(key.data, key.len),
                           std::string(value.data, value.len));
    if ((out->stopAfter >= 0) && ((int)out->seen.size() >= out->stopAfter)) {
        return false;                // abort the walk
    }
    return true;
}

std::string asString(const MqttStringView& v) {
    return v.present() ? std::string(v.data, v.len) : std::string();
}

}  // namespace mqtt5_receive

// Requirement 12.5: before any PUBLISH has been delivered the set is empty and
// carries the no-data indication, and there is nothing to walk.
TEST_CASE("inbound property accessor before any delivery reports no data") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_receive::connect(client, psc);

    const MqttInboundProperties& props = psc.inboundProperties();
    CHECK_FALSE(props.valid);
    CHECK_FALSE(props.hasPayloadFormat);
    CHECK_FALSE(props.contentType.present());
    CHECK_FALSE(props.responseTopic.present());
    CHECK_FALSE(props.hasCorrelationData);
    CHECK(props.topicAlias == 0);
    CHECK(props.userPropertyCount == 0);

    mqtt5_receive::UserProps walk;
    CHECK_FALSE(psc.forEachInboundUserProperty(mqtt5_receive::collectUserProps, &walk));
    CHECK(walk.seen.empty());
}

// Requirement 12.6: a PUBLISH carrying no properties yields a VALID but empty set,
// and the walk succeeds having visited nothing.
TEST_CASE("property-free inbound PUBLISH yields a valid but empty property set") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_receive::connect(client, psc);

    client.pushPacket(MqttPacket::raw(
        mqtt5_receive::publish5("tele/dev/SENSOR", MqttProps().bytes(),
                                std::vector<uint8_t>{'x'})));
    REQUIRE(psc.loop());
    REQUIRE(mqtt5_receive::g_capture.count == 1);

    const MqttInboundProperties& props = psc.inboundProperties();
    CHECK(props.valid);
    CHECK_FALSE(props.hasPayloadFormat);
    CHECK_FALSE(props.contentType.present());
    CHECK_FALSE(props.responseTopic.present());
    CHECK_FALSE(props.hasCorrelationData);
    CHECK(props.topicAlias == 0);
    CHECK(props.userPropertyCount == 0);

    mqtt5_receive::UserProps walk;
    CHECK(psc.forEachInboundUserProperty(mqtt5_receive::collectUserProps, &walk));
    CHECK(walk.seen.empty());
}

// Requirements 11.7, 12.4: every present value is reported with its received value,
// including a zero-length one, and Correlation Data survives embedded 0x00 bytes.
TEST_CASE("inbound property accessor reports every present value") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_receive::connect(client, psc);

    const std::vector<uint8_t> correlation{0x00, 0xFF, 0x00, 0x41};
    client.pushPacket(MqttPacket::raw(mqtt5_receive::publish5(
        "tele/dev/SENSOR",
        MqttProps().byte_(0x01, 1)                        // Payload Format Indicator
                   .str(0x03, "application/json")         // Content Type
                   .str(0x08, "")                         // Response Topic, zero-length
                   .bin(0x09, correlation)                // Correlation Data
                   .u16(0x23, 7)                          // Topic Alias
                   .bytes(),
        std::vector<uint8_t>{'y'})));
    REQUIRE(psc.loop());
    REQUIRE(mqtt5_receive::g_capture.count == 1);

    const MqttInboundProperties& props = psc.inboundProperties();
    CHECK(props.valid);
    CHECK(props.hasPayloadFormat);
    CHECK(props.payloadFormat == 1);
    REQUIRE(props.contentType.present());
    CHECK(mqtt5_receive::asString(props.contentType) == "application/json");
    // A zero-length value is PRESENT, and distinguishable from an absent one.
    REQUIRE(props.responseTopic.present());
    CHECK(props.responseTopic.len == 0);
    REQUIRE(props.hasCorrelationData);
    REQUIRE(props.correlationDataLen == correlation.size());
    CHECK(std::vector<uint8_t>(props.correlationData,
                               props.correlationData + props.correlationDataLen)
          == correlation);
    CHECK(props.topicAlias == 7);
    CHECK(props.userPropertyCount == 0);
}

// Requirement 11.4: every User Property is enumerated in received order, duplicate
// keys included, and a visitor that returns false aborts the walk.
TEST_CASE("inbound User Properties are enumerated in received order") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_receive::connect(client, psc);

    client.pushPacket(MqttPacket::raw(mqtt5_receive::publish5(
        "tele/dev/SENSOR",
        MqttProps().pair(0x26, "k", "1")
                   .str(0x03, "text/plain")
                   .pair(0x26, "k", "2")
                   .pair(0x26, "other", "")
                   .pair(0x26, "k", "3")
                   .bytes(),
        std::vector<uint8_t>{'z'})));
    REQUIRE(psc.loop());
    REQUIRE(mqtt5_receive::g_capture.count == 1);

    CHECK(psc.inboundProperties().userPropertyCount == 4);

    mqtt5_receive::UserProps walk;
    CHECK(psc.forEachInboundUserProperty(mqtt5_receive::collectUserProps, &walk));
    REQUIRE(walk.seen.size() == 4);
    CHECK(walk.seen[0] == std::make_pair(std::string("k"), std::string("1")));
    CHECK(walk.seen[1] == std::make_pair(std::string("k"), std::string("2")));
    CHECK(walk.seen[2] == std::make_pair(std::string("other"), std::string("")));
    CHECK(walk.seen[3] == std::make_pair(std::string("k"), std::string("3")));

    // A second walk re-reads the buffer, so it yields the same entries again.
    mqtt5_receive::UserProps again;
    CHECK(psc.forEachInboundUserProperty(mqtt5_receive::collectUserProps, &again));
    CHECK(again.seen.size() == 4);

    // Aborting after the second entry stops the walk and is reported as false.
    mqtt5_receive::UserProps aborting;
    aborting.stopAfter = 2;
    CHECK_FALSE(psc.forEachInboundUserProperty(mqtt5_receive::collectUserProps, &aborting));
    CHECK(aborting.seen.size() == 2);

    // A null visitor has nothing to call and is refused.
    CHECK_FALSE(psc.forEachInboundUserProperty(nullptr, nullptr));
}

// Requirement 12.5, rejection path: the set is dropped before validation so a PUBLISH
// rejected on its properties cannot leave the previous packet's values readable as if
// they were current.
TEST_CASE("a rejected inbound PUBLISH clears the previous property set") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_receive::connect(client, psc);

    client.pushPacket(MqttPacket::raw(mqtt5_receive::publish5(
        "tele/dev/SENSOR", MqttProps().str(0x03, "application/json").bytes(),
        std::vector<uint8_t>{'a'})));
    REQUIRE(psc.loop());
    REQUIRE(psc.inboundProperties().valid);
    REQUIRE(psc.inboundProperties().contentType.present());

    // Over-declared Property Length: rejected, so nothing of it becomes readable.
    client.pushPacket(MqttPacket::raw(mqtt5_receive::publish5(
        "tele/dev/SENSOR", MqttProps().byte_(0x01, 1).rawLength(40).bytes(),
        std::vector<uint8_t>{'b'})));
    CHECK_FALSE(psc.loop());

    CHECK_FALSE(psc.inboundProperties().valid);
    CHECK_FALSE(psc.inboundProperties().contentType.present());
    mqtt5_receive::UserProps walk;
    CHECK_FALSE(psc.forEachInboundUserProperty(mqtt5_receive::collectUserProps, &walk));
}

// --- Property 10: Inbound message property set round-trip (task 7.5) ------
//
// Task 7.2's example cases above pin the accessor's shape with a handful of
// hand-picked packets: no-data before delivery, valid-but-empty after a
// property-free PUBLISH, one packet with every value present, and one packet
// with several User Properties. This is the exhaustive sweep the design calls
// Property 10: every present/absent combination of the five single-valued
// properties, 0/1/2/5 User Properties including duplicate keys, and every
// Correlation Data length in the accepted domain 0..16.
//
// Requirements 11.11 and 12.6's Correlation-Data-length and property-free
// cases are the same accessor this file already exercises above; what this
// section adds is the EXHAUSTIVE sweep, not a new accessor path. Correlation
// Data lengths above 16 are Property 29's territory (task 7.10) and are
// deliberately not exercised here.
namespace mqtt5_receive {

// What Property 10 expects the accessor to report for one packet. A field's
// `has*` flag false means "must be reported as not present"; true means
// "must be reported with exactly this value" (Requirement 11.7).
struct ExpectedProps {
    bool        hasPayloadFormat = false;
    uint8_t     payloadFormat    = 0;
    bool        hasContentType   = false;
    std::string contentType;
    bool        hasResponseTopic = false;
    std::string responseTopic;
    bool        hasCorrelationData = false;
    std::vector<uint8_t> correlationData;
    bool        hasTopicAlias = false;
    uint16_t    topicAlias    = 0;
    // In received order, duplicates included (Requirement 11.4).
    std::vector<std::pair<std::string, std::string>> userProps;
};

// Asserts the accessor and the User Property walk match `exp` exactly: every
// present value with its received value, every absent value reported as not
// present, and every User Property enumerated in order.
void checkInboundProps(PubSubClient& psc, const ExpectedProps& exp) {
    const MqttInboundProperties& p = psc.inboundProperties();
    CHECK(p.valid);

    if (exp.hasPayloadFormat) {
        CHECK(p.hasPayloadFormat);
        CHECK(p.payloadFormat == exp.payloadFormat);
    } else {
        CHECK_FALSE(p.hasPayloadFormat);
    }

    if (exp.hasContentType) {
        REQUIRE(p.contentType.present());
        CHECK(asString(p.contentType) == exp.contentType);
    } else {
        CHECK_FALSE(p.contentType.present());
    }

    if (exp.hasResponseTopic) {
        REQUIRE(p.responseTopic.present());
        CHECK(asString(p.responseTopic) == exp.responseTopic);
    } else {
        CHECK_FALSE(p.responseTopic.present());
    }

    if (exp.hasCorrelationData) {
        REQUIRE(p.hasCorrelationData);
        REQUIRE(p.correlationDataLen == exp.correlationData.size());
        CHECK(std::vector<uint8_t>(p.correlationData,
                                   p.correlationData + p.correlationDataLen)
              == exp.correlationData);
    } else {
        CHECK_FALSE(p.hasCorrelationData);
    }

    if (exp.hasTopicAlias) {
        CHECK(p.topicAlias == exp.topicAlias);
    } else {
        CHECK(p.topicAlias == 0);
    }

    CHECK(p.userPropertyCount == exp.userProps.size());
    UserProps walk;
    CHECK(psc.forEachInboundUserProperty(collectUserProps, &walk));
    REQUIRE(walk.seen.size() == exp.userProps.size());
    for (size_t i = 0; i < exp.userProps.size(); ++i) {
        CHECK(walk.seen[i] == exp.userProps[i]);
    }
}

}  // namespace mqtt5_receive

// Feature: tasmota-pubsub-mqtt5, Property 10: For all inbound PUBLISH property sets, the property accessor reports every present value (Payload Format Indicator, Content Type, Response Topic, Correlation Data, Topic Alias) with its received value, reports every absent value as not present, and enumerates every received User Property in received order; for all received Correlation Data lengths in 0..16 the accessor reports that exact length and those exact bytes; and a PUBLISH carrying an empty Properties field yields a valid but empty property set.
TEST_CASE("Property 10: Inbound message property set round-trip") {
    using mqtt5_receive::ExpectedProps;

    SUBCASE("accessor before any delivery reports no data (Requirement 12.5)") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        mqtt5_receive::connect(client, psc);

        const MqttInboundProperties& p = psc.inboundProperties();
        CHECK_FALSE(p.valid);
        CHECK_FALSE(p.hasPayloadFormat);
        CHECK_FALSE(p.contentType.present());
        CHECK_FALSE(p.responseTopic.present());
        CHECK_FALSE(p.hasCorrelationData);
        CHECK(p.topicAlias == 0);
        CHECK(p.userPropertyCount == 0);

        mqtt5_receive::UserProps walk;
        CHECK_FALSE(psc.forEachInboundUserProperty(mqtt5_receive::collectUserProps, &walk));
        CHECK(walk.seen.empty());
    }

    SUBCASE("property-free PUBLISH yields a valid but empty set (Requirement 12.6)") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        mqtt5_receive::connect(client, psc);

        client.pushPacket(MqttPacket::publish5("tele/dev/SENSOR", std::vector<uint8_t>{'x'},
                                               /*qos=*/0, /*retained=*/false, /*dup=*/false,
                                               /*packetId=*/0, MqttProps()));
        REQUIRE(psc.loop());
        REQUIRE(mqtt5_receive::g_capture.count == 1);

        mqtt5_receive::checkInboundProps(psc, ExpectedProps());
    }

    SUBCASE("every present/absent combination of the single-valued properties") {
        struct Case { const char* label; MqttProps props; ExpectedProps expect; };

        ExpectedProps e;
        std::vector<Case> cases;

        e = ExpectedProps();
        e.hasPayloadFormat = true; e.payloadFormat = 1;
        cases.push_back({"only Payload Format Indicator present",
                         MqttProps().byte_(0x01, 1), e});

        e = ExpectedProps();
        e.hasContentType = true; e.contentType = "application/json";
        cases.push_back({"only Content Type present",
                         MqttProps().str(0x03, "application/json"), e});

        e = ExpectedProps();
        e.hasResponseTopic = true; e.responseTopic = "resp/topic";
        cases.push_back({"only Response Topic present",
                         MqttProps().str(0x08, "resp/topic"), e});

        e = ExpectedProps();
        e.hasResponseTopic = true; e.responseTopic = "";
        cases.push_back({"only Response Topic present, zero-length value",
                         MqttProps().str(0x08, ""), e});

        e = ExpectedProps();
        e.hasCorrelationData = true; e.correlationData = {0x01, 0x02, 0x03};
        cases.push_back({"only Correlation Data present",
                         MqttProps().bin(0x09, {0x01, 0x02, 0x03}), e});

        e = ExpectedProps();
        e.hasTopicAlias = true; e.topicAlias = 3;
        cases.push_back({"only Topic Alias present",
                         MqttProps().u16(0x23, 3), e});

        e = ExpectedProps();
        e.userProps = {{"k1", "v1"}, {"k2", "v2"}};
        cases.push_back({"only User Properties present",
                         MqttProps().pair(0x26, "k1", "v1").pair(0x26, "k2", "v2"), e});

        e = ExpectedProps();
        e.hasPayloadFormat = true; e.payloadFormat = 0;
        e.hasContentType = true; e.contentType = "text/plain";
        e.hasResponseTopic = true; e.responseTopic = "reply/here";
        e.hasCorrelationData = true; e.correlationData = {0x00, 0xFF, 0x00};
        e.hasTopicAlias = true; e.topicAlias = 9;
        e.userProps = {{"a", "1"}, {"b", "2"}};
        cases.push_back({"all five properties plus two User Properties present",
                         MqttProps().byte_(0x01, 0)
                                    .str(0x03, "text/plain")
                                    .str(0x08, "reply/here")
                                    .bin(0x09, {0x00, 0xFF, 0x00})
                                    .u16(0x23, 9)
                                    .pair(0x26, "a", "1")
                                    .pair(0x26, "b", "2"), e});

        for (const Case& c : cases) {
            CAPTURE(c.label);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            mqtt5_receive::connect(client, psc);

            client.pushPacket(MqttPacket::publish5("tele/dev/SENSOR", std::vector<uint8_t>{'x'},
                                                   /*qos=*/0, /*retained=*/false, /*dup=*/false,
                                                   /*packetId=*/0, c.props));
            REQUIRE(psc.loop());
            REQUIRE(mqtt5_receive::g_capture.count == 1);

            mqtt5_receive::checkInboundProps(psc, c.expect);
        }
    }

    SUBCASE("0/1/2/5 User Properties, including duplicate keys, preserve received order") {
        struct Case {
            const char* label;
            MqttProps props;
            std::vector<std::pair<std::string, std::string>> expected;
        };

        const std::vector<Case> cases = {
            {"0 User Properties", MqttProps(), {}},
            {"1 User Property", MqttProps().pair(0x26, "a", "1"),
             {{"a", "1"}}},
            {"2 User Properties", MqttProps().pair(0x26, "a", "1").pair(0x26, "b", "2"),
             {{"a", "1"}, {"b", "2"}}},
            {"5 User Properties with duplicate keys",
             MqttProps().pair(0x26, "dup", "1")
                        .pair(0x26, "other", "x")
                        .pair(0x26, "dup", "2")
                        .pair(0x26, "dup", "3")
                        .pair(0x26, "other", "y"),
             {{"dup", "1"}, {"other", "x"}, {"dup", "2"}, {"dup", "3"}, {"other", "y"}}},
        };

        for (const Case& c : cases) {
            CAPTURE(c.label);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            mqtt5_receive::connect(client, psc);

            client.pushPacket(MqttPacket::publish5("tele/dev/SENSOR", std::vector<uint8_t>{'x'},
                                                   /*qos=*/0, /*retained=*/false, /*dup=*/false,
                                                   /*packetId=*/0, c.props));
            REQUIRE(psc.loop());
            REQUIRE(mqtt5_receive::g_capture.count == 1);

            CHECK(psc.inboundProperties().userPropertyCount == c.expected.size());

            mqtt5_receive::UserProps walk;
            CHECK(psc.forEachInboundUserProperty(mqtt5_receive::collectUserProps, &walk));
            REQUIRE(walk.seen.size() == c.expected.size());
            for (size_t i = 0; i < c.expected.size(); ++i) {
                CHECK(walk.seen[i] == c.expected[i]);
            }
        }
    }

    SUBCASE("Correlation Data is reported whole and exact over every length 0..16") {
        // Exhaustive over the accepted domain: 17 lengths, 0 through 16 inclusive.
        // Most lengths embed a 0x00 byte to prove binary-safety; length 16 (the
        // upper boundary) is filled with all-0xFF so the sweep also covers that
        // content shape at the domain's edge.
        for (int len = 0; len <= 16; ++len) {
            CAPTURE(len);
            std::vector<uint8_t> content(static_cast<size_t>(len), 0xFF);
            if (len >= 1 && len != 16) {
                content[static_cast<size_t>(len) / 2] = 0x00;   // embedded 0x00 byte
            }

            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            mqtt5_receive::connect(client, psc);

            client.pushPacket(MqttPacket::publish5(
                "tele/dev/SENSOR", std::vector<uint8_t>{'x'}, /*qos=*/0,
                /*retained=*/false, /*dup=*/false, /*packetId=*/0,
                MqttProps().bin(0x09, content)));
            REQUIRE(psc.loop());
            REQUIRE(mqtt5_receive::g_capture.count == 1);

            const MqttInboundProperties& p = psc.inboundProperties();
            REQUIRE(p.hasCorrelationData);
            // The full length is reported, never a prefix of it.
            REQUIRE(p.correlationDataLen == content.size());
            CHECK(std::vector<uint8_t>(p.correlationData,
                                       p.correlationData + p.correlationDataLen)
                  == content);
        }
    }
}

#endif   // MQTT_VERSION == MQTT_VERSION_5_0

}  // TEST_SUITE("mqtt5")
