/*
  mqtt5_ack_test.cpp - MQTT 5.0 acknowledgement framing and parsing (task 9.1).

  Feature: tasmota-pubsub-mqtt5

  TEST_SUITE("mqtt5"), body guarded by `#if MQTT_VERSION == MQTT_VERSION_5_0`, so the
  file compiles in both selections and contributes cases only to the 5.0 binary.

  Scope at task 9.1 is the shared assembler and the shared parser for PUBACK, PUBREC,
  PUBREL and PUBCOMP:

    Remaining Length 2 : PacketIdentifier(2)                        short form
    Remaining Length 3 : PacketIdentifier(2) | ReasonCode(1)
    Remaining Length >3: PacketIdentifier(2) | ReasonCode(1) | Properties

  All three are accepted, an absent Reason Code reads back as 0x00 (Requirements 7.3,
  7.5), and anything outside the three forms is rejected through
  `protocolFailure(MalformedPacket, 0x81)` (Requirement 7.9). A well-formed
  acknowledgement whose Packet Identifier matches no outstanding message is discarded
  with no state change (Requirement 7.8) - which, until task 11 introduces the
  outstanding table, is every acknowledgement the client can receive.

  Emission is exercised through the one path that reaches it today, the PUBACK for an
  inbound QoS 1 PUBLISH: short form, Remaining Length 2, no Reason Code byte
  (Requirements 7.1, 7.2). PUBREL's mandatory 0x02 fixed-header flags (Requirement 7.7)
  and the reason-code-carrying form are not reachable from the public API until the QoS 2
  handshake lands, so they are pinned by the tasks that make them reachable.

  These are example cases: the exhaustive sweeps over reason codes and property sets
  belong to the acknowledged-delivery properties of section 11.

  Requirements: 7.1, 7.2, 7.3, 7.5, 7.6, 7.8, 7.9
*/

#include <cstdint>
#include <string>
#include <vector>

#include "doctest.h"

#include "MockClient.h"
#include "MqttPacket.h"
#include "TestClock.h"
#include "PubSubClient.h"

TEST_SUITE("mqtt5") {

#if MQTT_VERSION == MQTT_VERSION_5_0

namespace mqtt5_ack {

// A connected client with no callback registered: acknowledgement handling is
// independent of the message callback, and leaving it unset proves it.
void connect(MockClient& client, PubSubClient& psc) {
    client.pushPacket(MqttPacket::connack(0));
    psc.setServer("broker.example", 1883);
    REQUIRE(psc.connect("mqtt5-ack"));
    REQUIRE(psc.connected());
    client.clearOutbound();
    psc.setSocketTimeout(1);
}

// Frame `body` as an acknowledgement of `type`. The Remaining Length is derived from the
// body, so the packet is always self-consistent and only the body's SHAPE is under test.
std::vector<uint8_t> ack(uint8_t type, const std::vector<uint8_t>& body) {
    std::vector<uint8_t> bytes;
    bytes.push_back(type);
    const std::vector<uint8_t> rl =
        MqttPacket::encodeRemainingLength(static_cast<uint32_t>(body.size()));
    bytes.insert(bytes.end(), rl.begin(), rl.end());
    bytes.insert(bytes.end(), body.begin(), body.end());
    return bytes;
}

std::vector<uint8_t> idBytes(uint16_t packetId) {
    return {static_cast<uint8_t>(packetId >> 8), static_cast<uint8_t>(packetId & 0xFF)};
}

const uint8_t kTypes[4] = {
    static_cast<uint8_t>(MQTTPUBACK),
    static_cast<uint8_t>(MQTTPUBREC),
    static_cast<uint8_t>(MQTTPUBREL) | 0x02,   // PUBREL's mandatory fixed-header flags
    static_cast<uint8_t>(MQTTPUBCOMP)
};

}  // namespace mqtt5_ack

// Requirements 7.3, 7.5, 7.8: every one of the three forms parses, and because nothing
// is outstanding yet, every one is then discarded - the connection stays up, no byte
// goes out in response, and no error is recorded.
TEST_CASE("all three acknowledgement forms are accepted and discarded when nothing matches") {
    struct Vector {
        const char* name;
        std::vector<uint8_t> tail;   // bytes after the Packet Identifier
    };

    std::vector<Vector> vectors;
    vectors.push_back({"short form: identifier only", {}});
    vectors.push_back({"reason code 0x00, no properties", {0x00}});
    // 0x00 is valid for all four acknowledgement packet types. Packet-specific
    // failure-code allowlists are exercised by the semantic-validator coverage.
    {
        // Reason Code plus an EMPTY Properties field - the single byte 0x00.
        std::vector<uint8_t> tail{0x00};
        const std::vector<uint8_t> props = MqttProps().bytes();
        tail.insert(tail.end(), props.begin(), props.end());
        vectors.push_back({"reason code with an empty Properties field", tail});
    }
    {
        // Reason Code plus a populated Properties field: Reason String and a User
        // Property are both legal in all four acknowledgement types.
        std::vector<uint8_t> tail{0x00};
        const std::vector<uint8_t> props =
            MqttProps().str(0x1F, "no matching subscribers").pair(0x26, "k", "v").bytes();
        tail.insert(tail.end(), props.begin(), props.end());
        vectors.push_back({"reason code with Reason String and User Property", tail});
    }

    for (const Vector& v : vectors) {
        for (uint8_t type : mqtt5_ack::kTypes) {
            CAPTURE(v.name);
            CAPTURE(type);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            mqtt5_ack::connect(client, psc);

            std::vector<uint8_t> body = mqtt5_ack::idBytes(0x1234);
            body.insert(body.end(), v.tail.begin(), v.tail.end());
            const std::vector<uint8_t> packet = mqtt5_ack::ack(type, body);
            // The fixture itself must be a legal acknowledgement, checked by the
            // harness's independent structural validator.
            REQUIRE(MqttParser::isStructurallyWellFramed(packet));
            client.pushInbound(packet);

            CHECK(psc.loop());
            CHECK(psc.connected());
            CHECK_FALSE(client.stopCalled());
            CHECK(psc.lastError() == MqttError::None);
            // Discarded means discarded: nothing is sent back.
            CHECK(client.outbound().empty());
        }
    }
}

// Requirement 7.9: anything that is not one of the three forms is malformed. The failure
// funnel records it, emits one DISCONNECT carrying the Reason Code while the socket is
// still writable, and closes.
TEST_CASE("malformed acknowledgements are rejected through the failure funnel") {
    struct Vector {
        const char* name;
        std::vector<uint8_t> body;
        MqttError            error;
        uint8_t              reasonCode;
    };

    std::vector<Vector> vectors;
    // Remaining Length below 2: there is no room for a Packet Identifier at all.
    vectors.push_back({"Remaining Length 0", {}, MqttError::MalformedPacket, 0x81});
    vectors.push_back({"Remaining Length 1", {0x12}, MqttError::MalformedPacket, 0x81});
    {
        // A Property Length declaring more body than the packet actually carries.
        std::vector<uint8_t> body = mqtt5_ack::idBytes(7);
        body.push_back(0x00);
        const std::vector<uint8_t> props = MqttProps().str(0x1F, "ok").rawLength(40).bytes();
        body.insert(body.end(), props.begin(), props.end());
        vectors.push_back({"over-declared Property Length", body, MqttError::MalformedPacket, 0x81});
    }
    {
        // A byte after the Properties field, which is the last field of an
        // acknowledgement, so it belongs to nothing.
        std::vector<uint8_t> body = mqtt5_ack::idBytes(7);
        body.push_back(0x00);
        body.push_back(0x00);   // empty Properties field
        body.push_back(0xFF);   // trailing garbage
        vectors.push_back({"trailing byte after the Properties field", body,
                           MqttError::MalformedPacket, 0x81});
    }
    {
        // Content Type is a PUBLISH property and illegal in an acknowledgement, so this
        // is parseable but semantically wrong: ProtocolError, Reason Code 0x82.
        std::vector<uint8_t> body = mqtt5_ack::idBytes(7);
        body.push_back(0x00);
        const std::vector<uint8_t> props = MqttProps().str(0x03, "text/plain").bytes();
        body.insert(body.end(), props.begin(), props.end());
        vectors.push_back({"property illegal in an acknowledgement", body,
                           MqttError::ProtocolError, 0x82});
    }

    for (const Vector& v : vectors) {
        for (uint8_t type : mqtt5_ack::kTypes) {
            CAPTURE(v.name);
            CAPTURE(type);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            mqtt5_ack::connect(client, psc);

            client.pushInbound(mqtt5_ack::ack(type, v.body));

            CHECK_FALSE(psc.loop());
            CHECK_FALSE(psc.connected());
            CHECK(client.stopCalled());
            CHECK(psc.lastError() == v.error);
            CHECK(psc.lastReasonCode() == v.reasonCode);
            // Exactly one DISCONNECT, error form, carrying that Reason Code.
            const std::vector<uint8_t> out = client.outbound();
            REQUIRE(out.size() == 3);
            CHECK(out[0] == static_cast<uint8_t>(MQTTDISCONNECT));
            CHECK(out[1] == 0x01);
            CHECK(out[2] == v.reasonCode);
        }
    }
}

// Requirements 7.1, 7.2, 7.6: the assembler's short form is what the client emits. The
// only path that reaches it today is the PUBACK for an inbound QoS 1 PUBLISH, and its
// four bytes carry no Reason Code byte and no Properties field.
TEST_CASE("the emitted PUBACK uses the short form and echoes the identifier") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_ack::connect(client, psc);
    psc.setCallback([](char*, uint8_t*, unsigned int) {});

    client.pushPacket(MqttPacket::publish5("tele/dev/STATE", {'o', 'n'}, 1, false, false,
                                           0x0102));
    CHECK(psc.loop());
    CHECK(psc.connected());

    const std::vector<uint8_t> expected{static_cast<uint8_t>(MQTTPUBACK), 0x02, 0x01, 0x02};
    CHECK(client.outbound() == expected);
}

#endif  // MQTT_VERSION == MQTT_VERSION_5_0

}  // TEST_SUITE("mqtt5")
