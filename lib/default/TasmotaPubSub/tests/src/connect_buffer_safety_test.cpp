/*
  connect_buffer_safety_test.cpp - task 18.2 CONNECT assembly hardening.

  The test is deliberately unguarded: `make test` runs it in both MQTT 3.1.1 and
  MQTT 5.0 selections under ASan/UBSan. It sweeps every capacity from one byte through
  one byte beyond each shape's first valid CONNECT capacity. A failed assembly must
  happen after opening but before writing the transport, close that transport, report
  BufferTooSmall, and leave the client reusable.
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

struct ConnectShape {
    const char* name;
    std::string clientId;
    std::string user;
    std::string password;
    bool credentials = false;
    bool will = false;
    uint8_t willQos = 0;
    std::string willTopic;
    std::string willMessage;
};

uint16_t firstValidCapacity(const ConnectShape& shape) {
    uint32_t capacity = MQTT_CONNECT_MINIMUM_BUFFER_SIZE + shape.clientId.size();
    if (shape.credentials) {
        capacity += 2 + shape.user.size();
        capacity += 2 + shape.password.size();
    }
    if (shape.will) {
#if MQTT_VERSION == MQTT_VERSION_5_0
        capacity += 1;  // empty Will Properties field
#endif
        capacity += 2 + shape.willTopic.size();
        capacity += 2 + shape.willMessage.size();
    }
    REQUIRE(capacity <= 0xFFFFu);
    return static_cast<uint16_t>(capacity);
}

bool connectWith(const ConnectShape& shape, PubSubClient& psc) {
    return psc.connect(shape.clientId.c_str(),
                       shape.credentials ? shape.user.c_str() : nullptr,
                       shape.credentials ? shape.password.c_str() : nullptr,
                       shape.will ? shape.willTopic.c_str() : nullptr,
                       shape.willQos,
                       false,
                       shape.will ? shape.willMessage.c_str() : nullptr,
                       true);
}

void checkSuccessfulFrame(const std::vector<uint8_t>& bytes) {
    REQUIRE_FALSE(bytes.empty());
#if MQTT_VERSION == MQTT_VERSION_5_0
    CHECK(MqttParser::isStructurallyWellFramed(bytes));
#else
    const DecodedPacket packet = MqttParser::decode(bytes);
    CHECK(packet.valid);
    CHECK(packet.type == static_cast<uint8_t>(MQTTCONNECT));
    CHECK(packet.flags == 0);
#endif
}

}  // namespace

TEST_SUITE("mqtt5") {

// Feature: tasmota-pubsub-mqtt5, Property 5 / Finding 8: For every CONNECT shape
// (no credentials, credentials, Will QoS 0/1/2, and a maximum-fitting Client Identifier)
// and every buffer capacity from 1 through one byte above that shape's first valid
// capacity, CONNECT either refuses atomically with BufferTooSmall and zero wire bytes,
// or succeeds with a structurally valid frame at and above the first valid capacity.
TEST_CASE("CONNECT buffer capacity sweep is atomic and reusable") {
    std::vector<ConnectShape> shapes;
    shapes.push_back({"no credentials", "", "", "", false, false, 0, "", ""});
    shapes.push_back({"credentials", "", "user", "password", true, false, 0, "", ""});
    for (uint8_t qos = 0; qos <= 2; ++qos) {
        shapes.push_back({"Will", "", "", "", false, true, qos, "will/topic", "offline"});
    }

    // This client identifier fills a deliberately chosen 96-byte working buffer exactly.
    // Its own first-valid capacity is therefore the configured cap, proving that string
    // capacity is checked as one complete field instead of copied until it overflows.
    ConnectShape maximumFitting = {"maximum-fitting Client Identifier", "", "", "",
                                   false, false, 0, "", ""};
    maximumFitting.clientId.assign(96 - MQTT_CONNECT_MINIMUM_BUFFER_SIZE, 'i');
    shapes.push_back(maximumFitting);

    for (const ConnectShape& shape : shapes) {
        const uint16_t firstValid = firstValidCapacity(shape);
        CAPTURE(shape.name);
        CAPTURE(firstValid);

        for (uint16_t capacity = 1; capacity <= static_cast<uint16_t>(firstValid + 1);
             ++capacity) {
            CAPTURE(capacity);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            psc.setServer("broker.example", 1883);
            REQUIRE(psc.setBufferSize(capacity));

            if (capacity < firstValid) {
                CHECK_FALSE(connectWith(shape, psc));
                CHECK(client.connectCalled());
                CHECK(client.stopCalled());
                CHECK(client.outbound().empty());
                CHECK(psc.lastError() == MqttError::BufferTooSmall);
                CHECK_FALSE(psc.connected());
                CHECK(psc.state() != MQTT_CONNECTED);

                // A refused CONNECT must not poison state needed by the next attempt.
                const unsigned stopsAfterRefusal = client.stopCount();
                REQUIRE(psc.setBufferSize(firstValid));
                client.pushPacket(MqttPacket::connack(0));
                REQUIRE(connectWith(shape, psc));
                CHECK(client.stopCount() == stopsAfterRefusal);
                checkSuccessfulFrame(client.outbound());
            } else {
                client.pushPacket(MqttPacket::connack(0));
                REQUIRE(connectWith(shape, psc));
                CHECK_FALSE(client.stopCalled());
                CHECK(psc.lastError() == MqttError::None);
                checkSuccessfulFrame(client.outbound());
            }
        }
    }
}

}  // TEST_SUITE("mqtt5")
