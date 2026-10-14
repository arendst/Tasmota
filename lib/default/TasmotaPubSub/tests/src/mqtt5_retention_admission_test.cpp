/*
  mqtt5_retention_admission_test.cpp - Task 18.8 atomic replay-storage admission.

  Feature: tasmota-pubsub-mqtt5

  Buffered QoS 1/2 publishes must retain their complete replay body before their
  first transport write. A budget or allocation failure is retryable: it sends no
  bytes, leaves the connection usable, and releases the tentative slot and packet
  identifier. QoS 0 remains allocation-free on this path.
*/

#include <cstdint>
#include <vector>

#include "doctest.h"

#include "AllocInterposer.h"
#include "MockClient.h"
#include "MqttPacket.h"
#include "PubSubClient.h"
#include "TestClock.h"

TEST_SUITE("mqtt5") {

#if MQTT_VERSION == MQTT_VERSION_5_0

namespace mqtt5_retention_admission {

void connect(MockClient& client, PubSubClient& psc) {
    client.pushPacket(MqttPacket::connack5(0x00, false));
    psc.setServer("broker.example", 1883);
    REQUIRE(psc.connect("mqtt5-retention-admission", nullptr, nullptr, nullptr,
                        0, false, nullptr, /*cleanSession=*/false));
    REQUIRE(psc.connected());
    psc.setKeepAlive(0);
    client.clearOutbound();
}

uint16_t publish(MockClient& client, PubSubClient& psc, const std::vector<uint8_t>& payload) {
    REQUIRE(psc.publish("t", payload.data(), static_cast<unsigned int>(payload.size()),
                        false, 1));
    const DecodedPublish5 sent = MqttParser::decodePublish5(client.outbound());
    REQUIRE(sent.valid);
    REQUIRE(sent.msgId != 0);
    return sent.msgId;
}

}  // namespace mqtt5_retention_admission

// Feature: tasmota-pubsub-mqtt5, Property 31: For all buffered QoS 1/2 publish
// attempts whose complete replay body cannot fit the remaining Inflight_Retention_Budget
// or cannot be allocated, the client returns RetentionUnavailable before its first write,
// emits zero bytes, retains no tentative entry or identifier, leaves budget accounting and
// the connection usable, and accepts a retry after capacity is restored. QoS 0 does not
// perform a retention allocation.
TEST_CASE("Property 31: replay storage is an atomic admission requirement") {
    using namespace mqtt5_retention_admission;

    SUBCASE("budget exhaustion refuses atomically and succeeds after acknowledgement restores capacity") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connect(client, psc);

        const size_t bodyOverhead = 2 + 1 + 2 + 1;  // topic prefix + t + id + empty props
        const size_t budget = static_cast<size_t>(MQTT_INFLIGHT_COPY_BUDGET);
        REQUIRE(budget >= bodyOverhead);
        REQUIRE(psc.setBufferSize(static_cast<uint16_t>(MQTT_MAX_HEADER_SIZE + budget + 64)));

        const std::vector<uint8_t> fillsBudget(budget - bodyOverhead, 'a');
        const uint16_t first = publish(client, psc, fillsBudget);
        CHECK(psc.inFlightCount() == 1);

        client.clearOutbound();
        const uint8_t retryPayload[] = {'r'};
        CHECK_FALSE(psc.publish("t", retryPayload, sizeof(retryPayload), false, 1));
        CHECK(psc.lastError() == MqttError::RetentionUnavailable);
        CHECK(client.outbound().empty());
        CHECK(psc.connected());
        CHECK(psc.inFlightCount() == 1);

        client.pushPacket(MqttPacket::ack5(MQTTPUBACK, first, -1));
        REQUIRE(psc.loop());
        CHECK(psc.inFlightCount() == 0);

        client.clearOutbound();
        const uint16_t retried = publish(client, psc, std::vector<uint8_t>{'r'});
        CHECK(retried != 0);
        CHECK(psc.inFlightCount() == 1);
    }

    SUBCASE("every retention allocation failure refuses before the wire and leaves no tracked slot") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connect(client, psc);

        const uint8_t qos0Payload[] = {'0'};
        AllocInterposer::reset();
        REQUIRE(psc.publish("qos0", qos0Payload, sizeof(qos0Payload), false, 0));
        CHECK(AllocInterposer::mallocCount() == 0);

        client.clearOutbound();
        AllocInterposer::failNextAllocation();
        const uint8_t qos1Payload[] = {'1'};
        CHECK_FALSE(psc.publish("qos1", qos1Payload, sizeof(qos1Payload), false, 1));
        CHECK(AllocInterposer::mallocCount() == 1);
        CHECK(AllocInterposer::pendingFailures() == 0);
        CHECK(psc.lastError() == MqttError::RetentionUnavailable);
        CHECK(client.outbound().empty());
        CHECK(psc.connected());
        CHECK(psc.inFlightCount() == 0);

        // A full publish envelope after the failed attempt proves the tentative identifier
        // and table slot were released rather than leaked.
        AllocInterposer::reset();
        std::vector<uint16_t> ids;
        for (uint8_t i = 0; i < MQTT_MAX_INFLIGHT; ++i) {
            client.clearOutbound();
            ids.push_back(publish(client, psc, std::vector<uint8_t>{i}));
        }
        CHECK(psc.inFlightCount() == MQTT_MAX_INFLIGHT);
        for (uint16_t id : ids) {
            client.pushPacket(MqttPacket::ack5(MQTTPUBACK, id, -1));
            REQUIRE(psc.loop());
        }
        CHECK(psc.inFlightCount() == 0);
        AllocInterposer::reset();
    }
}

#endif  // MQTT_VERSION == MQTT_VERSION_5_0

}  // TEST_SUITE("mqtt5")
