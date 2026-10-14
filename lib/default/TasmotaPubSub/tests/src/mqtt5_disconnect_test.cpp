/*
  mqtt5_disconnect_test.cpp - MQTT 5.0 DISCONNECT handling and AUTH rejection (task 9.4).

  Feature: tasmota-pubsub-mqtt5

  TEST_SUITE("mqtt5"), body guarded by `#if MQTT_VERSION == MQTT_VERSION_5_0`, so the
  file compiles in both selections and contributes cases only to the 5.0 binary.

  Outbound (Requirements 10.3, 10.4):

    disconnect(true)  -> E0 00        the short form: Reason Code 0x00 implied, no
                                      Properties field, and byte-for-byte what the
                                      MQTT 3.1.1 build has always written
    disconnect()      -> nothing      default argument, unchanged

  Inbound (Requirements 10.1, 10.2), all three forms accepted:

    E0 00                             Reason Code implied 0x00
    E0 01 <rc>                        Reason Code only
    E0 [rl] <rc> <Properties>         Reason Code with Properties

  The Reason Code is exposed through serverDisconnectReasonCode(), lastError() becomes
  MqttError::ServerDisconnect, the connection closes, and NO DISCONNECT goes back - the
  broker has already said goodbye.

  Rejection (Requirements 14.1, 16.4): inbound AUTH (packet type 15) is a protocol error
  carrying 0x8C, and any other packet type that is invalid for a client to receive is a
  protocol error carrying 0x82. Both are resolved inside the single loop() invocation that
  read the packet.

  Those are EXAMPLE cases. Task 9.8 adds the exhaustive sweep at the bottom of this file:
  Property 24, all 256 Reason Codes x the three inbound forms, asserting the Reason Code
  exposed, the connection closed, and an outbound record that grows by zero bytes.

  Requirements: 10.1, 10.2, 10.3, 10.4, 14.1, 16.4
*/

#include <array>
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

namespace mqtt5_disc {

void connect(MockClient& client, PubSubClient& psc) {
    client.pushPacket(MqttPacket::connack(0));
    psc.setServer("broker.example", 1883);
    REQUIRE(psc.connect("mqtt5-disc"));
    REQUIRE(psc.connected());
    client.clearOutbound();
    psc.setSocketTimeout(1);
}

// Frame a packet of `type` around `body`, deriving the Remaining Length from the body so
// the fixture is always self-consistent and only the body's SHAPE is under test.
std::vector<uint8_t> framed(uint8_t type, const std::vector<uint8_t>& body) {
    std::vector<uint8_t> bytes{type};
    const std::vector<uint8_t> rl =
        MqttPacket::encodeRemainingLength(static_cast<uint32_t>(body.size()));
    bytes.insert(bytes.end(), rl.begin(), rl.end());
    bytes.insert(bytes.end(), body.begin(), body.end());
    return bytes;
}

// An inbound DISCONNECT. `reasonCode` below 0 selects the short form (Remaining Length 0);
// `withProps` appends a Properties field after the Reason Code.
std::vector<uint8_t> disconnect5(int reasonCode, const MqttProps* props = nullptr) {
    std::vector<uint8_t> body;
    if (reasonCode >= 0) {
        body.push_back(static_cast<uint8_t>(reasonCode));
        if (props != nullptr) {
            const std::vector<uint8_t> p = props->bytes();
            body.insert(body.end(), p.begin(), p.end());
        }
    }
    return framed(static_cast<uint8_t>(MQTTDISCONNECT), body);
}

}  // namespace mqtt5_disc

// ===========================================================================
//  Outbound DISCONNECT (Requirements 10.3, 10.4)
// ===========================================================================

// disconnect(bool) keeps its signature and its default: the short form E0 00 when asked
// for a packet, and nothing at all otherwise. Both close the transport.
TEST_CASE("disconnect(true) emits exactly E0 00 and disconnect() emits nothing") {
    {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        mqtt5_disc::connect(client, psc);

        psc.disconnect(true);

        const std::vector<uint8_t> expected{static_cast<uint8_t>(MQTTDISCONNECT), 0x00};
        CHECK(client.outbound() == expected);
        CHECK(MqttParser::isStructurallyWellFramed(client.outbound()));
        CHECK(client.stopCalled());
        CHECK_FALSE(psc.connected());
        CHECK(psc.state() == MQTT_DISCONNECTED);
    }
    {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        mqtt5_disc::connect(client, psc);

        psc.disconnect();                          // default argument: no packet

        CHECK(client.outbound().empty());
        CHECK(client.stopCalled());
        CHECK_FALSE(psc.connected());
    }
}

// ===========================================================================
//  Inbound DISCONNECT (Requirements 10.1, 10.2)
// ===========================================================================

// Every form is absorbed: the Reason Code becomes readable (0x00 when absent), the
// connection closes, state() reports disconnected, and NOT ONE byte goes back.
TEST_CASE("an inbound DISCONNECT exposes its Reason Code and is absorbed silently") {
    struct Vector {
        const char* name;
        int         reasonCode;   // < 0 = short form
        uint8_t     expected;
    };
    const Vector vectors[] = {
        {"short form, Reason Code implied", -1, 0x00},
        {"Reason Code 0x00 (Normal disconnection)", 0x00, 0x00},
        {"Reason Code 0x04 (Disconnect with Will message)", 0x04, 0x04},
        {"Reason Code 0x81 (Malformed packet)", 0x81, 0x81},
        {"Reason Code 0x8D (Keep Alive timeout)", 0x8D, 0x8D},
        {"Reason Code 0x93 (Receive Maximum exceeded)", 0x93, 0x93},
        {"Reason Code 0x9C (Use another server)", 0x9C, 0x9C},
        {"Reason Code 0xA2 (Wildcard subscriptions not supported)", 0xA2, 0xA2},
    };

    for (const Vector& v : vectors) {
        CAPTURE(v.name);
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        mqtt5_disc::connect(client, psc);

        const std::vector<uint8_t> packet = mqtt5_disc::disconnect5(v.reasonCode);
        REQUIRE(MqttParser::isStructurallyWellFramed(packet));
        client.pushInbound(packet);

        CHECK_FALSE(psc.loop());
        CHECK_FALSE(psc.connected());
        CHECK(client.stopCalled());
        CHECK(psc.state() == MQTT_DISCONNECTED);
        // Requirement 10.1: the code is readable through the Public_API.
        CHECK(psc.serverDisconnectReasonCode() == v.expected);
        CHECK(psc.lastError() == MqttError::ServerDisconnect);
        CHECK(psc.lastReasonCode() == v.expected);
        // Requirement 10.2: no return DISCONNECT, no anything.
        CHECK(client.outbound().empty());
    }
}

// The long form's Properties field is parsed rather than skipped, and the outcome is the
// same: code exposed, connection closed, nothing sent back.
TEST_CASE("an inbound DISCONNECT carrying Properties is parsed and absorbed") {
    struct Vector {
        const char* name;
        MqttProps   props;
    };

    std::vector<Vector> vectors;
    vectors.push_back({"empty Properties field", MqttProps()});
    vectors.push_back({"Reason String", MqttProps().str(0x1F, "server shutting down")});
    vectors.push_back({"Server Reference", MqttProps().str(0x1C, "broker2.example:1883")});
    vectors.push_back({"Session Expiry Interval", MqttProps().u32(0x11, 3600)});
    vectors.push_back({"two User Properties",
                       MqttProps().pair(0x26, "a", "1").pair(0x26, "b", "2")});

    for (const Vector& v : vectors) {
        CAPTURE(v.name);
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        mqtt5_disc::connect(client, psc);

        const std::vector<uint8_t> packet = mqtt5_disc::disconnect5(0x8E, &v.props);
        REQUIRE(MqttParser::isStructurallyWellFramed(packet));
        client.pushInbound(packet);

        CHECK_FALSE(psc.loop());
        CHECK_FALSE(psc.connected());
        CHECK(psc.serverDisconnectReasonCode() == 0x8E);
        CHECK(psc.lastError() == MqttError::ServerDisconnect);
        CHECK(client.outbound().empty());
    }
}

// A DISCONNECT that does not fit any of the three forms is a malformed packet, so it goes
// through the standard failure funnel instead - which does emit its own DISCONNECT while
// the socket is writable.
TEST_CASE("a malformed inbound DISCONNECT is rejected through the failure funnel") {
    struct Vector {
        const char*          name;
        std::vector<uint8_t> body;
        MqttError            error;
        uint8_t              reasonCode;
    };

    std::vector<Vector> vectors;
    {
        // A Property Length declaring more body than the packet carries.
        std::vector<uint8_t> body{0x8E};
        const std::vector<uint8_t> props = MqttProps().str(0x1F, "ok").rawLength(40).bytes();
        body.insert(body.end(), props.begin(), props.end());
        vectors.push_back({"over-declared Property Length", body,
                           MqttError::MalformedPacket, 0x81});
    }
    {
        // A byte after the Properties field, which is DISCONNECT's last field.
        std::vector<uint8_t> body{0x8E, 0x00, 0xFF};
        vectors.push_back({"trailing byte after the Properties field", body,
                           MqttError::MalformedPacket, 0x81});
    }
    {
        // Content Type is a PUBLISH property and illegal in a DISCONNECT.
        std::vector<uint8_t> body{0x8E};
        const std::vector<uint8_t> props = MqttProps().str(0x03, "text/plain").bytes();
        body.insert(body.end(), props.begin(), props.end());
        vectors.push_back({"property illegal in a DISCONNECT", body,
                           MqttError::ProtocolError, 0x82});
    }
    {
        // Authentication Data is refused in every packet type, with 0x8C.
        std::vector<uint8_t> body{0x8E};
        const std::vector<uint8_t> props = MqttProps().bin(0x16, {0x01, 0x02}).bytes();
        body.insert(body.end(), props.begin(), props.end());
        vectors.push_back({"Authentication Data property", body,
                           MqttError::ProtocolError, 0x8C});
    }

    for (const Vector& v : vectors) {
        CAPTURE(v.name);
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        mqtt5_disc::connect(client, psc);

        client.pushInbound(mqtt5_disc::framed(static_cast<uint8_t>(MQTTDISCONNECT), v.body));

        CHECK_FALSE(psc.loop());
        CHECK_FALSE(psc.connected());
        CHECK(client.stopCalled());
        CHECK(psc.lastError() == v.error);
        CHECK(psc.lastReasonCode() == v.reasonCode);

        const std::vector<uint8_t> out = client.outbound();
        REQUIRE(out.size() == 3);
        CHECK(out[0] == static_cast<uint8_t>(MQTTDISCONNECT));
        CHECK(out[1] == 0x01);
        CHECK(out[2] == v.reasonCode);
    }
}

// ===========================================================================
//  AUTH and other packet types a client may not receive
// ===========================================================================

// Requirement 16.4: Enhanced Authentication is a non-goal in full, so an inbound AUTH is a
// protocol error carrying 0x8C (Bad authentication method), resolved inside this single
// loop() invocation - well within the 1-second bound at the driver's 50 ms loop() cadence.
// The library initiates no reconnection; that stays the driver's decision.
TEST_CASE("an inbound AUTH is a protocol error carrying 0x8C") {
    struct Vector {
        const char*          name;
        std::vector<uint8_t> body;
    };

    std::vector<Vector> vectors;
    vectors.push_back({"AUTH with no body", {}});
    vectors.push_back({"AUTH, Reason Code only", {0x18}});   // Continue authentication
    {
        std::vector<uint8_t> body{0x18};
        const std::vector<uint8_t> props = MqttProps().str(0x15, "SCRAM-SHA-1").bytes();
        body.insert(body.end(), props.begin(), props.end());
        vectors.push_back({"AUTH with an Authentication Method property", body});
    }
    {
        std::vector<uint8_t> body{0x00};                     // Success
        const std::vector<uint8_t> props = MqttProps().bin(0x16, {0xDE, 0xAD}).bytes();
        body.insert(body.end(), props.begin(), props.end());
        vectors.push_back({"AUTH with Authentication Data", body});
    }

    for (const Vector& v : vectors) {
        CAPTURE(v.name);
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        mqtt5_disc::connect(client, psc);

        client.pushInbound(mqtt5_disc::framed(static_cast<uint8_t>(MQTTAUTH), v.body));

        CHECK_FALSE(psc.loop());
        CHECK_FALSE(psc.connected());
        CHECK(client.stopCalled());
        CHECK(psc.lastError() == MqttError::ProtocolError);
        CHECK(psc.lastReasonCode() == 0x8C);
        // The packet's own contents are never processed: it is refused on its type alone,
        // so the Reason Code is 0x8C whatever body it carried.
        const std::vector<uint8_t> out = client.outbound();
        REQUIRE(out.size() == 3);
        CHECK(out[0] == static_cast<uint8_t>(MQTTDISCONNECT));
        CHECK(out[1] == 0x01);
        CHECK(out[2] == 0x8C);
    }
}

// Requirement 14.1: a packet type that is not valid for a client to receive is a protocol
// error - discarded without processing its payload, 0x82, connection closed. CONNECT,
// SUBSCRIBE, UNSUBSCRIBE and PINGREQ are client-to-server only, and type 0 is Reserved.
TEST_CASE("packet types invalid for a client to receive are protocol errors") {
    struct Vector {
        const char*          name;
        uint8_t              type;
        std::vector<uint8_t> body;
    };

    std::vector<Vector> vectors;
    vectors.push_back({"reserved type 0", 0x00, {}});
    vectors.push_back({"CONNECT", static_cast<uint8_t>(MQTTCONNECT),
                       {0x00, 0x04, 'M', 'Q', 'T', 'T', 0x05, 0x02, 0x00, 0x0F, 0x00}});
    vectors.push_back({"SUBSCRIBE", static_cast<uint8_t>(MQTTSUBSCRIBE) | 0x02,
                       {0x00, 0x01, 0x00, 0x00, 0x01, 'a', 0x00}});
    vectors.push_back({"UNSUBSCRIBE", static_cast<uint8_t>(MQTTUNSUBSCRIBE) | 0x02,
                       {0x00, 0x01, 0x00, 0x00, 0x01, 'a'}});
    // PINGREQ is answered with a PINGRESP by the MQTT 3.1.1 build, but it is a
    // client-to-server packet, so in MQTT 5.0 a client receiving one is a protocol error.
    vectors.push_back({"PINGREQ", static_cast<uint8_t>(MQTTPINGREQ), {}});

    for (const Vector& v : vectors) {
        CAPTURE(v.name);
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        mqtt5_disc::connect(client, psc);

        client.pushInbound(mqtt5_disc::framed(v.type, v.body));

        CHECK_FALSE(psc.loop());
        CHECK_FALSE(psc.connected());
        CHECK(client.stopCalled());
        CHECK(psc.lastError() == MqttError::ProtocolError);
        CHECK(psc.lastReasonCode() == 0x82);

        const std::vector<uint8_t> out = client.outbound();
        REQUIRE(out.size() == 3);
        CHECK(out[0] == static_cast<uint8_t>(MQTTDISCONNECT));
        CHECK(out[1] == 0x01);
        CHECK(out[2] == 0x82);
    }
}

// PINGRESP stays a normal, expected inbound packet: it clears the outstanding ping rather
// than being swept up by the invalid-type branch.
TEST_CASE("PINGRESP is still accepted and clears the outstanding ping") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    psc.setKeepAlive(2);
    mqtt5_disc::connect(client, psc);

    TestClock::instance().advance(2001);
    REQUIRE(psc.loop());
    // One PINGREQ went out and a ping is now outstanding.
    const std::vector<uint8_t> ping{static_cast<uint8_t>(MQTTPINGREQ), 0x00};
    REQUIRE(client.outbound() == ping);
    client.clearOutbound();

    client.pushPacket(MqttPacket::pingresp());
    CHECK(psc.loop());
    CHECK(psc.connected());
    CHECK(psc.lastError() == MqttError::None);

    // The outstanding ping was cleared, so the next idle interval issues a further
    // PINGREQ instead of declaring a timeout.
    TestClock::instance().advance(2001);
    CHECK(psc.loop());
    CHECK(client.outbound() == ping);
    CHECK(psc.connected());
}

// ===========================================================================
//  Task 9.8 - Property 24: Server-initiated DISCONNECT is absorbed silently
// ===========================================================================

namespace mqtt5_p24 {

// The three inbound forms the library's parser accepts. The SHORT form carries no Reason
// Code byte at all, so whatever code the sweep is on, the exposed code must read back as
// the implied 0x00 - which is exactly the "0x00 when absent" half of the property.
enum class Form { ShortForm, ReasonCodeOnly, WithProperties };

const char* formName(Form form) {
    switch (form) {
        case Form::ShortForm:      return "short form (Remaining Length 0)";
        case Form::ReasonCodeOnly: return "Reason Code only (Remaining Length 1)";
        default:                   return "Reason Code with Properties";
    }
}

// A Properties field for the long form, rotated by Reason Code so the sweep covers the
// width of what a DISCONNECT may legally carry rather than one fixed shape. Index 0 is
// handled by the caller: an EXPLICITLY EMPTY Properties field cannot be expressed through
// MqttPacket::disconnect5(), which selects the Reason-Code-only form when props.empty().
MqttProps propsFor(uint8_t reasonCode) {
    switch (reasonCode % 5) {
        case 1:  return MqttProps().str(0x1F, "server shutting down");        // Reason String
        case 2:  return MqttProps().u32(0x11, 3600);                  // Session Expiry Interval
        case 3:  return MqttProps().str(0x1C, "broker2.example:1883");     // Server Reference
        default: return MqttProps().pair(0x26, "k", "v").pair(0x26, "shed", "load");
    }
}

// The packet for one (form, Reason Code) point of the sweep. Built through the harness
// fixture (task 9.5) rather than the library's own writer, so the assertion compares two
// independent implementations.
std::vector<uint8_t> packetFor(Form form, uint8_t reasonCode) {
    switch (form) {
        case Form::ShortForm:
            return MqttPacket::disconnect5(-1).bytes();
        case Form::ReasonCodeOnly:
            return MqttPacket::disconnect5(static_cast<int>(reasonCode)).bytes();
        default:
            if ((reasonCode % 5) == 0) {
                // E0 02 <rc> 00 - the long form carrying a Properties field that declares
                // zero entries. Still the "with Properties" form on the wire.
                return mqtt5_disc::framed(static_cast<uint8_t>(MQTTDISCONNECT),
                                          {reasonCode, 0x00});
            }
            return MqttPacket::disconnect5(static_cast<int>(reasonCode),
                                          propsFor(reasonCode)).bytes();
    }
}

}  // namespace mqtt5_p24

// Feature: tasmota-pubsub-mqtt5, Property 24: For all inbound DISCONNECT packets — short form, Reason Code only, and Reason Code with Properties — the client exposes the Reason Code (0x00 when absent), closes the network connection, reports a disconnected state, and transmits no DISCONNECT in return.
TEST_CASE("Property 24: server-initiated DISCONNECT is absorbed silently") {
    using namespace mqtt5_p24;

    SUBCASE("all 256 Reason Codes x the three inbound forms") {
        const Form forms[] = {Form::ShortForm, Form::ReasonCodeOnly, Form::WithProperties};

        const uint8_t legalReasonCodes[] = {
            0x00, 0x04, 0x80, 0x81, 0x82, 0x83, 0x87, 0x89, 0x8B, 0x8D, 0x8E,
            0x8F, 0x90, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B,
            0x9C, 0x9D, 0x9E, 0x9F, 0xA0, 0xA1, 0xA2
        };
        for (Form form : forms) {
            for (uint8_t rc : legalReasonCodes) {
                CAPTURE(formName(form));
                CAPTURE(rc);

                TestClock::instance().reset();
                MockClient client;
                PubSubClient psc(client);
                mqtt5_disc::connect(client, psc);

                // The outbound record is empty at this point (connect() clears it); the
                // property is that it grows by ZERO bytes, so measure rather than assume.
                const size_t outboundBefore = client.outbound().size();

                const std::vector<uint8_t> packet = packetFor(form, rc);
                REQUIRE(MqttParser::isStructurallyWellFramed(packet));
                client.pushInbound(packet);

                // loop() returns false: the connection is gone by the time it returns.
                CHECK_FALSE(psc.loop());

                // Requirement 10.1: the Reason Code is exposed, 0x00 when absent.
                const uint8_t expected = (form == Form::ShortForm) ? 0x00 : rc;
                CHECK(psc.serverDisconnectReasonCode() == expected);
                CHECK(psc.lastError() == MqttError::ServerDisconnect);
                CHECK(psc.lastReasonCode() == expected);

                // Requirement 10.2: the network connection closes and the state reports
                // disconnected...
                CHECK(client.stopCalled());
                CHECK_FALSE(psc.connected());
                CHECK(psc.state() == MQTT_DISCONNECTED);

                // ...and NOT ONE byte goes back - no return DISCONNECT, no anything.
                CHECK(client.outbound().size() == outboundBefore);
            }
        }
    }

    // Example cases for the outbound half of Requirement 10 (criteria 10.3, 10.4), which
    // the property above does not reach: a client-initiated disconnect is the short form
    // E0 00 - Reason Code 0x00 (Normal disconnection) implied, no Properties field - and
    // the transport is flushed and closed after it.
    SUBCASE("example (Requirements 10.3, 10.4): disconnect(true) emits exactly E0 00") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        mqtt5_disc::connect(client, psc);

        psc.disconnect(true);

        const std::vector<uint8_t> expected{static_cast<uint8_t>(MQTTDISCONNECT), 0x00};
        CHECK(client.outbound() == expected);
        CHECK(MqttParser::isStructurallyWellFramed(client.outbound()));
        // The bytes are in the record AND the transport is closed, so the packet was
        // written before the close: flushed once, stopped once.
        CHECK(client.flushCount() == 1);
        CHECK(client.stopCount() == 1);
        CHECK_FALSE(psc.connected());
        CHECK(psc.state() == MQTT_DISCONNECTED);
    }

    SUBCASE("example: disconnect() still emits nothing and still closes") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        mqtt5_disc::connect(client, psc);

        psc.disconnect();                          // default argument: no packet

        CHECK(client.outbound().empty());
        CHECK(client.flushCount() == 1);
        CHECK(client.stopCount() == 1);
        CHECK_FALSE(psc.connected());
    }
}

namespace mqtt5_p26 {

enum class ConnectForm { Plain, Credentials, Will, CredentialsAndWill };

std::vector<std::vector<uint8_t>> splitOutbound(const std::vector<uint8_t>& stream) {
    std::vector<std::vector<uint8_t>> packets;
    size_t pos = 0;
    while (pos < stream.size()) {
        const size_t start = pos++;
        uint32_t remaining = 0;
        size_t remainingBytes = 0;
        REQUIRE(MqttParser::decodeRemainingLength(stream, pos, remaining, remainingBytes));
        const size_t end = pos + remainingBytes + static_cast<size_t>(remaining);
        REQUIRE(end <= stream.size());
        packets.emplace_back(stream.begin() + static_cast<std::ptrdiff_t>(start),
                             stream.begin() + static_cast<std::ptrdiff_t>(end));
        pos = end;
    }
    return packets;
}

void checkProperties(const DecodedProperties& properties) {
    REQUIRE(properties.valid);
    for (const DecodedProperty& property : properties.entries) {
        CHECK(property.id != MQTT_PROP_AUTHENTICATION_METHOD);
        CHECK(property.id != MQTT_PROP_AUTHENTICATION_DATA);
    }
}

void checkNoEnhancedAuthentication(const std::vector<uint8_t>& stream) {
    const std::vector<std::vector<uint8_t>> packets = splitOutbound(stream);
    REQUIRE_FALSE(packets.empty());

    for (const std::vector<uint8_t>& packet : packets) {
        REQUIRE(MqttParser::isStructurallyWellFramed(packet));
        const DecodedPacket decoded = MqttParser::decode(packet);
        REQUIRE(decoded.valid);
        CHECK(decoded.type != MQTTAUTH);

        switch (decoded.type) {
            case MQTTCONNECT: {
                const DecodedConnect5 connect = MqttParser::decodeConnect5(packet);
                REQUIRE(connect.valid);
                checkProperties(connect.props);
                if (connect.willFlag) {
                    checkProperties(connect.willProps);
                }
                break;
            }
            case MQTTCONNACK: {
                REQUIRE(decoded.payload.size() >= 2);
                checkProperties(MqttParser::decodeProperties(decoded.payload, 2));
                break;
            }
            case MQTTPUBLISH: {
                const DecodedPublish5 publish = MqttParser::decodePublish5(packet);
                REQUIRE(publish.valid);
                checkProperties(publish.props);
                break;
            }
            case MQTTPUBACK:
            case MQTTPUBREC:
            case MQTTPUBREL:
            case MQTTPUBCOMP: {
                const DecodedAck5 acknowledgement = MqttParser::decodeAck5(packet, decoded.type);
                REQUIRE(acknowledgement.valid);
                if (acknowledgement.form == 2) {
                    checkProperties(acknowledgement.props);
                }
                break;
            }
            case MQTTSUBSCRIBE: {
                const DecodedSubscribe5 subscribe = MqttParser::decodeSubscribe5(packet);
                REQUIRE(subscribe.valid);
                checkProperties(subscribe.props);
                break;
            }
            case MQTTSUBACK:
            case MQTTUNSUBACK: {
                const DecodedSuback5 acknowledgement =
                    MqttParser::decodeSuback5(packet, decoded.type);
                REQUIRE(acknowledgement.valid);
                checkProperties(acknowledgement.props);
                break;
            }
            case MQTTUNSUBSCRIBE:
                REQUIRE(decoded.payload.size() >= 2);
                checkProperties(MqttParser::decodeProperties(decoded.payload, 2));
                break;
            case MQTTDISCONNECT:
                if (decoded.payload.size() > 1) {
                    checkProperties(MqttParser::decodeProperties(decoded.payload, 1));
                }
                break;
            case MQTTPINGREQ:
            case MQTTPINGRESP:
            case MQTTAUTH:
                break;
            default:
                CHECK(false);
                break;
        }
    }
}

void connectRetainingOutbound(MockClient& client, PubSubClient& psc, ConnectForm form) {
    client.pushPacket(MqttPacket::connack5(0));
    psc.setServer("broker.example", 1883);
    bool connected = false;
    switch (form) {
        case ConnectForm::Plain:
            connected = psc.connect("mqtt5-property-26");
            break;
        case ConnectForm::Credentials:
            connected = psc.connect("mqtt5-property-26", "user", "password");
            break;
        case ConnectForm::Will:
            connected = psc.connect("mqtt5-property-26", "tele/will", 1, true, "offline");
            break;
        case ConnectForm::CredentialsAndWill:
            connected = psc.connect("mqtt5-property-26", "user", "password",
                                    "tele/will", 1, true, "offline", true);
            break;
    }
    REQUIRE(connected);
    REQUIRE(psc.connected());
}

const char* connectFormName(ConnectForm form) {
    switch (form) {
        case ConnectForm::Plain:              return "no credentials or Will";
        case ConnectForm::Credentials:        return "credentials";
        case ConnectForm::Will:               return "Will";
        case ConnectForm::CredentialsAndWill: return "credentials and Will";
    }
    return "unknown";
}

}  // namespace mqtt5_p26

// Feature: tasmota-pubsub-mqtt5, Property 26: For all scenarios the client can be driven
// through, and for all Properties fields in the recorded outbound stream — CONNECT, Will,
// PUBLISH, every acknowledgement, SUBSCRIBE, UNSUBSCRIBE and DISCONNECT alike — the stream
// contains no AUTH packet, no Authentication Method property (0x15) and no Authentication
// Data property (0x16).
TEST_CASE("Property 26: Enhanced Authentication never appears outbound") {
    using namespace mqtt5_p26;

    // Keep every CONNECT frame intact so the scanner reaches both its ordinary
    // Properties field and, when present, the distinct Will Properties field.
    const std::array<ConnectForm, 4> connectForms = {{
        ConnectForm::Plain, ConnectForm::Credentials,
        ConnectForm::Will, ConnectForm::CredentialsAndWill,
    }};
    for (const ConnectForm form : connectForms) {
        CAPTURE(connectFormName(form));
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectRetainingOutbound(client, psc, form);
        checkNoEnhancedAuthentication(client.outbound());
    }

    SUBCASE("PUBLISH at every QoS, subscriptions, and keepalive") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        psc.setKeepAlive(2);
        connectRetainingOutbound(client, psc, ConnectForm::CredentialsAndWill);

        const uint8_t correlation[] = {0xC0, 0xDE};
        const char* const keys[] = {"source"};
        const char* const values[] = {"property-26"};
        MqttPublishRequest richPublish;
        richPublish.topic = "tele/property-26/STATE";
        richPublish.payload = reinterpret_cast<const uint8_t*>("ok");
        richPublish.plength = 2;
        richPublish.properties.contentType = "text/plain";
        richPublish.properties.responseTopic = "cmnd/property-26/RESULT";
        richPublish.properties.correlationData = correlation;
        richPublish.properties.correlationDataLen = sizeof(correlation);
        richPublish.properties.userPropertyKeys = keys;
        richPublish.properties.userPropertyValues = values;
        richPublish.properties.userPropertyCount = 1;
        REQUIRE(psc.publish(richPublish));
#ifdef USE_MQTT_QOS
        REQUIRE(psc.publish("tele/property-26/QOS1", "one", false, 1));
        REQUIRE(psc.publish("tele/property-26/QOS2", "two", false, 2));
#endif  // USE_MQTT_QOS
        REQUIRE(psc.subscribe("cmnd/property-26/#", 1));
        REQUIRE(psc.unsubscribe("cmnd/property-26/#"));

        TestClock::instance().advance(2001);
        REQUIRE(psc.loop());
        checkNoEnhancedAuthentication(client.outbound());
    }

    SUBCASE("every outbound acknowledgement") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectRetainingOutbound(client, psc, ConnectForm::CredentialsAndWill);

        client.pushPacket(MqttPacket::publish5("tele/property-26/in-1", {'a'}, 1,
                                               false, false, 0x1111));
        REQUIRE(psc.loop());                          // PUBACK
        client.pushPacket(MqttPacket::publish5("tele/property-26/in-2", {'b'}, 2,
                                               false, false, 0x2222));
        REQUIRE(psc.loop());                          // PUBREC
        client.pushPacket(MqttPacket::ack5(MQTTPUBREL, 0x2222, -1));
        REQUIRE(psc.loop());                          // PUBCOMP

#ifdef USE_MQTT_QOS
        // Outbound QoS 2 drives PUBREL only when the feature is compiled in.
        REQUIRE(psc.publish("tele/property-26/out-2", "out", false, 2));
        const std::vector<std::vector<uint8_t>> beforePubrec = splitOutbound(client.outbound());
        const DecodedPublish5 outboundQos2 = MqttParser::decodePublish5(beforePubrec.back());
        REQUIRE(outboundQos2.valid);
        REQUIRE(outboundQos2.qos == 2);
        REQUIRE(outboundQos2.msgId != 0);
        client.pushPacket(MqttPacket::ack5(MQTTPUBREC, outboundQos2.msgId, -1));
        REQUIRE(psc.loop());                          // PUBREL
#endif  // USE_MQTT_QOS

        checkNoEnhancedAuthentication(client.outbound());
    }

    SUBCASE("normal DISCONNECT") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectRetainingOutbound(client, psc, ConnectForm::CredentialsAndWill);
        psc.disconnect(true);
        checkNoEnhancedAuthentication(client.outbound());
    }

    SUBCASE("protocol-failure DISCONNECT") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectRetainingOutbound(client, psc, ConnectForm::CredentialsAndWill);
        psc.protocolFailure(MqttError::ProtocolError, PubSubClient::MQTT_RC_PROTOCOL_ERROR);
        checkNoEnhancedAuthentication(client.outbound());
    }
}

#endif  // MQTT_VERSION == MQTT_VERSION_5_0

}  // TEST_SUITE("mqtt5")
