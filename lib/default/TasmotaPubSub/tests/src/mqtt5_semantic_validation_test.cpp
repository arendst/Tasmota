/*
  mqtt5_semantic_validation_test.cpp - Task 18.4 inbound MQTT 5 semantic gate.

  Feature: tasmota-pubsub-mqtt5
*/

#include <cstdint>
#include <vector>

#include "doctest.h"

#include "MockClient.h"
#include "MqttPacket.h"
#include "PubSubClient.h"
#include "TestClock.h"

TEST_SUITE("mqtt5") {

#if MQTT_VERSION == MQTT_VERSION_5_0

namespace mqtt5_semantic {

void connect(MockClient& client, PubSubClient& psc) {
    client.pushPacket(MqttPacket::connack5(0));
    psc.setServer("broker.example", 1883);
    REQUIRE(psc.connect("mqtt5-semantic"));
    REQUIRE(psc.connected());
    client.clearOutbound();
    psc.setKeepAlive(0);
}

void requireProtocolFailure(MockClient& client, PubSubClient& psc,
                            const std::vector<uint8_t>& packet,
                            MqttError error = MqttError::ProtocolError) {
    client.pushPacket(MqttPacket::raw(packet));
    CHECK_FALSE(psc.loop());
    CHECK_FALSE(psc.connected());
    CHECK(client.stopCalled());
    CHECK(psc.lastError() == error);
    CHECK(psc.lastReasonCode() == (error == MqttError::MalformedPacket ? 0x81 : 0x82));
    CHECK(client.outbound() == std::vector<uint8_t>{
        static_cast<uint8_t>(MQTTDISCONNECT), 0x01,
        static_cast<uint8_t>(error == MqttError::MalformedPacket ? 0x81 : 0x82)});
}

bool allowedPublishAck(uint8_t code) {
    switch (code) {
        case 0x00: case 0x10: case 0x80: case 0x83: case 0x87:
        case 0x90: case 0x91: case 0x97: case 0x99:
            return true;
        default:
            return false;
    }
}

bool allowedPubrelOrComp(uint8_t code) {
    return code == 0x00 || code == 0x92;
}

bool allowedSuback(uint8_t code) {
    switch (code) {
        case 0x00: case 0x01: case 0x02: case 0x80: case 0x83: case 0x87:
        case 0x8F: case 0x91: case 0x97: case 0x9E: case 0xA1: case 0xA2:
            return true;
        default:
            return false;
    }
}

bool allowedUnsuback(uint8_t code) {
    switch (code) {
        case 0x00: case 0x11: case 0x80: case 0x83: case 0x87: case 0x8F: case 0x91:
            return true;
        default:
            return false;
    }
}

bool allowedDisconnect(uint8_t code) {
    switch (code) {
        case 0x00: case 0x04: case 0x80: case 0x81: case 0x82: case 0x83: case 0x87:
        case 0x89: case 0x8B: case 0x8D: case 0x8E: case 0x8F: case 0x90: case 0x93:
        case 0x94: case 0x95: case 0x96: case 0x97: case 0x98: case 0x99: case 0x9A:
        case 0x9B: case 0x9C: case 0x9D: case 0x9E: case 0x9F: case 0xA0: case 0xA1:
        case 0xA2:
            return true;
        default:
            return false;
    }
}

std::vector<uint8_t> packetForFlags(uint8_t type) {
    switch (type) {
        case MQTTCONNACK:
            return MqttPacket::connack5(0).bytes();
        case MQTTPUBLISH:
            return MqttPacket::publish5("topic", {'p'}, 0).bytes();
        case MQTTPUBACK:
        case MQTTPUBREC:
        case MQTTPUBREL:
        case MQTTPUBCOMP:
            return MqttPacket::ack5(type, 1, -1).bytes();
        case MQTTSUBACK:
            return MqttPacket::suback5(1, {0}).bytes();
        case MQTTUNSUBACK:
            return MqttPacket::unsuback5(1, {0}).bytes();
        case MQTTPINGRESP:
            return MqttPacket::pingresp().bytes();
        case MQTTDISCONNECT:
            return MqttPacket::disconnect5(-1).bytes();
        default:
            return MqttPacket::auth(0).bytes();
    }
}

}  // namespace mqtt5_semantic

// Feature: tasmota-pubsub-mqtt5, Property 32: Every MQTT 5 inbound control packet is
// semantically validated before dispatch. Every incorrect fixed-header low nibble, zero
// required Packet Identifier, invalid PUBLISH DUP/QoS combination, malformed PINGRESP,
// second CONNACK, and packet-inapplicable Reason Code is rejected through the failure
// funnel without advancing a protocol state machine.
TEST_CASE("Property 32: inbound MQTT 5 semantic validation precedes dispatch") {
    using namespace mqtt5_semantic;

    SUBCASE("every inbound control type rejects all fifteen incorrect fixed-header nibbles") {
        const uint8_t types[] = {
            MQTTCONNACK, MQTTPUBLISH, MQTTPUBACK, MQTTPUBREC, MQTTPUBREL, MQTTPUBCOMP,
            MQTTSUBACK, MQTTUNSUBACK, MQTTPINGRESP, MQTTDISCONNECT, MQTTAUTH
        };
        const uint8_t requiredFlags[] = {0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0};

        for (size_t i = 0; i < sizeof(types) / sizeof(types[0]); ++i) {
            for (uint8_t flags = 0; flags < 16; ++flags) {
                if (flags == requiredFlags[i]) {
                    continue;
                }
                if (types[i] == MQTTPUBLISH) {
                    const uint8_t qos = static_cast<uint8_t>((flags >> 1) & 0x03);
                    const bool validPublishFlags = (qos != 3) &&
                                                   !((qos == 0) && ((flags & 0x08) != 0));
                    if (validPublishFlags) {
                        continue;
                    }
                }
                CAPTURE(types[i]);
                CAPTURE(flags);
                TestClock::instance().reset();
                MockClient client;
                PubSubClient psc(client);
                connect(client, psc);
                std::vector<uint8_t> packet = packetForFlags(types[i]);
                packet[0] = static_cast<uint8_t>(types[i] | flags);
                requireProtocolFailure(client, psc, packet, MqttError::MalformedPacket);
            }
        }
    }

    SUBCASE("server-to-client direction rejects client-originated control packets") {
        const std::vector<std::vector<uint8_t>> packets = {
            {static_cast<uint8_t>(MQTTCONNECT), 0x00},
            {static_cast<uint8_t>(MQTTSUBSCRIBE | MQTTQOS1), 0x00},
            {static_cast<uint8_t>(MQTTUNSUBSCRIBE | MQTTQOS1), 0x00},
            {static_cast<uint8_t>(MQTTPINGREQ), 0x00},
        };
        for (const std::vector<uint8_t>& packet : packets) {
            CAPTURE(packet[0]);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            connect(client, psc);
            requireProtocolFailure(client, psc, packet, MqttError::ProtocolError);
        }
    }

    SUBCASE("zero identifiers and invalid PUBLISH headers are rejected before delivery") {
        std::vector<std::vector<uint8_t>> packets = {
            MqttPacket::publish5("topic", {'p'}, 1, false, false, 0).bytes(),
            MqttPacket::publish5("topic", {'p'}, 2, false, false, 0).bytes(),
            MqttPacket::ack5(MQTTPUBACK, 0, -1).bytes(),
            MqttPacket::ack5(MQTTPUBREC, 0, -1).bytes(),
            MqttPacket::ack5(MQTTPUBREL, 0, -1).bytes(),
            MqttPacket::ack5(MQTTPUBCOMP, 0, -1).bytes(),
            MqttPacket::suback5(0, {0}).bytes(),
            MqttPacket::unsuback5(0, {0}).bytes(),
        };
        std::vector<uint8_t> qosZeroDup = MqttPacket::publish5("topic", {'p'}, 0).bytes();
        qosZeroDup[0] |= 0x08;
        packets.push_back(qosZeroDup);
        std::vector<uint8_t> qosThree = MqttPacket::publish5("topic", {'p'}, 0).bytes();
        qosThree[0] = static_cast<uint8_t>((qosThree[0] & 0xF9) | 0x06);
        packets.push_back(qosThree);

        for (const std::vector<uint8_t>& packet : packets) {
            CAPTURE(packet[0]);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            connect(client, psc);
            requireProtocolFailure(client, psc, packet,
                                   ((packet[0] & 0x06) == 0x06 ||
                                    ((packet[0] & 0x0F) == 0x08))
                                       ? MqttError::MalformedPacket
                                       : MqttError::ProtocolError);
        }
    }

    SUBCASE("all 256 reason codes follow their packet-specific allowlist") {
        struct AckKind { uint8_t type; bool (*allows)(uint8_t); };
        const AckKind acknowledgements[] = {
            {MQTTPUBACK, allowedPublishAck}, {MQTTPUBREC, allowedPublishAck},
            {MQTTPUBREL, allowedPubrelOrComp}, {MQTTPUBCOMP, allowedPubrelOrComp},
            {MQTTSUBACK, allowedSuback}, {MQTTUNSUBACK, allowedUnsuback},
        };
        for (const AckKind& kind : acknowledgements) {
            for (unsigned value = 0; value <= 0xFF; ++value) {
                const uint8_t code = static_cast<uint8_t>(value);
                CAPTURE(kind.type);
                CAPTURE(code);
                TestClock::instance().reset();
                MockClient client;
                PubSubClient psc(client);
                connect(client, psc);
                MqttPacket packet = (kind.type == MQTTSUBACK)
                    ? MqttPacket::suback5(1, {code})
                    : (kind.type == MQTTUNSUBACK)
                        ? MqttPacket::unsuback5(1, {code})
                        : MqttPacket::ack5(kind.type, 1, static_cast<int>(code));
                client.pushPacket(packet);
                if (kind.allows(code)) {
                    CHECK(psc.loop());
                    CHECK(psc.connected());
                    CHECK(psc.lastError() == MqttError::None);
                } else {
                    CHECK_FALSE(psc.loop());
                    CHECK(psc.lastError() == MqttError::ProtocolError);
                    CHECK(psc.lastReasonCode() == 0x82);
                }
            }
        }

        for (unsigned value = 0; value <= 0xFF; ++value) {
            const uint8_t code = static_cast<uint8_t>(value);
            CAPTURE(code);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            connect(client, psc);
            client.pushPacket(MqttPacket::disconnect5(static_cast<int>(code)));
            CHECK_FALSE(psc.loop());
            if (allowedDisconnect(code)) {
                CHECK(psc.lastError() == MqttError::ServerDisconnect);
                CHECK(psc.serverDisconnectReasonCode() == code);
                CHECK(client.outbound().empty());
            } else {
                CHECK(psc.lastError() == MqttError::ProtocolError);
                CHECK(psc.lastReasonCode() == 0x82);
            }
        }
    }

    SUBCASE("PINGRESP shape and a post-CONNACK CONNACK are never dispatched") {
        const std::vector<std::vector<uint8_t>> packets = {
            {static_cast<uint8_t>(MQTTPINGRESP), 0x01, 0x00},
            {static_cast<uint8_t>(MQTTPINGRESP | 0x01), 0x00},
            MqttPacket::connack5(0).bytes(),
        };
        for (const std::vector<uint8_t>& packet : packets) {
            CAPTURE(packet[0]);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            connect(client, psc);
            requireProtocolFailure(client, psc, packet,
                                   ((packet[0] & 0xF0) == MQTTCONNACK)
                                       ? MqttError::ProtocolError
                                       : MqttError::MalformedPacket);
        }
    }
}

#endif  // MQTT_VERSION == MQTT_VERSION_5_0

}  // TEST_SUITE("mqtt5")
