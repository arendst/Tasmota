/*
  mqtt311_parity_test.cpp - MQTT 3.1.1 wire-parity property (task 16.3).

  This is intentionally not a TEST_SUITE("baseline") case. The frozen baseline
  suite remains the 104-case byte-identity gate; Property 27 is an additional,
  selector-specific characterization compiled only into the MQTT 3.1.1 binary.
*/

#include <cstdint>
#include <initializer_list>
#include <string>
#include <vector>

#include "doctest.h"

#include "MockClient.h"
#include "MqttPacket.h"
#include "TestClock.h"
#include "PubSubClient.h"

#if MQTT_VERSION == MQTT_VERSION_3_1_1

namespace mqtt311_parity {

using Bytes = std::vector<uint8_t>;

void requireBytes(const MockClient& client, std::initializer_list<uint8_t> expected) {
    CHECK(client.outbound() == Bytes(expected));
}

void connectAndClear(MockClient& client, PubSubClient& psc, const char* id = "p") {
    client.pushPacket(MqttPacket::connack(0));
    psc.setServer("broker.example", 1883);
    REQUIRE(psc.connect(id));
    REQUIRE(psc.connected());
    client.clearOutbound();
    psc.setSocketTimeout(1);
}

}  // namespace mqtt311_parity

// Feature: tasmota-pubsub-mqtt5, Property 27: For all public-API operations and argument combinations, a build with the selector set to MQTT 3.1.1 emits exactly the bytes the pre-migration library emits and accepts exactly the packets it accepts.
//
// Property 27: The MQTT 3.1.1 selection is byte-identical
//
// The shared Remaining Length codec deliberately refuses values greater than
// 16,383 in both selections. This property therefore covers the accepted domain
// only and pins the over-limit streaming call as a total refusal; it never
// asserts retired three- or four-byte Remaining Length framing.
//
// Validates: Requirements 2.3, 15.3
TEST_CASE("Property 27: MQTT 3.1.1 public API parity") {
    using namespace mqtt311_parity;

    // CONNECT has protocol level 4 and no CONNECT or Will Properties field.
    // The five overloads are represented by their distinct payload/flag forms.
    {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        psc.setKeepAlive(30u);
        client.pushPacket(MqttPacket::connack(0));
        psc.setServer("broker.example", 1883);
        REQUIRE(psc.connect("id"));
        requireBytes(client, {0x10, 0x0E, 0x00, 0x04, 'M', 'Q', 'T', 'T', 0x04, 0x02,
                              0x00, 0x1E, 0x00, 0x02, 'i', 'd'});
    }
    {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        psc.setKeepAlive(30u);
        client.pushPacket(MqttPacket::connack(0));
        psc.setServer("broker.example", 1883);
        REQUIRE(psc.connect("id", "u", "p"));
        requireBytes(client, {0x10, 0x14, 0x00, 0x04, 'M', 'Q', 'T', 'T', 0x04, 0xC2,
                              0x00, 0x1E, 0x00, 0x02, 'i', 'd', 0x00, 0x01, 'u',
                              0x00, 0x01, 'p'});
    }
    {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        psc.setKeepAlive(30u);
        client.pushPacket(MqttPacket::connack(0));
        psc.setServer("broker.example", 1883);
        REQUIRE(psc.connect("id", "w", 1u, true, "m"));
        requireBytes(client, {0x10, 0x14, 0x00, 0x04, 'M', 'Q', 'T', 'T', 0x04, 0x2E,
                              0x00, 0x1E, 0x00, 0x02, 'i', 'd', 0x00, 0x01, 'w',
                              0x00, 0x01, 'm'});
    }
    {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        psc.setKeepAlive(30u);
        client.pushPacket(MqttPacket::connack(0));
        psc.setServer("broker.example", 1883);
        REQUIRE(psc.connect("id", "u", "p", "w", 1u, true, "m"));
        requireBytes(client, {0x10, 0x1A, 0x00, 0x04, 'M', 'Q', 'T', 'T', 0x04, 0xEE,
                              0x00, 0x1E, 0x00, 0x02, 'i', 'd', 0x00, 0x01, 'w',
                              0x00, 0x01, 'm', 0x00, 0x01, 'u', 0x00, 0x01, 'p'});
    }
    {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        psc.setKeepAlive(30u);
        client.pushPacket(MqttPacket::connack(0));
        psc.setServer("broker.example", 1883);
        REQUIRE(psc.connect("id", "u", "p", "w", 1u, true, "m", false));
        requireBytes(client, {0x10, 0x1A, 0x00, 0x04, 'M', 'Q', 'T', 'T', 0x04, 0xEC,
                              0x00, 0x1E, 0x00, 0x02, 'i', 'd', 0x00, 0x01, 'w',
                              0x00, 0x01, 'm', 0x00, 0x01, 'u', 0x00, 0x01, 'p'});
    }

    // All buffered, PROGMEM and streaming PUBLISH routes retain the legacy
    // layout: topic length and topic directly followed by the payload.
    struct PublishVector {
        uint8_t path;
        bool retained;
    };
    const PublishVector publishes[] = {
        {0u, false}, {1u, true}, {2u, false}, {3u, true}, {4u, false},
        {5u, true}, {6u, false}, {7u, true}, {8u, false}, {9u, false},
        {10u, false},
    };
    for (const PublishVector& vector : publishes) {
        CAPTURE(vector.path);
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);

        const uint8_t bytePayload = 'x';
        boolean emitted = false;
        switch (vector.path) {
        case 0: emitted = psc.publish("t", "x"); break;
        case 1: emitted = psc.publish("t", "x", vector.retained); break;
        case 2: emitted = psc.publish("t", &bytePayload, 1u); break;
        case 3: emitted = psc.publish("t", &bytePayload, 1u, vector.retained); break;
        case 4: emitted = psc.publish("t", &bytePayload, 1u, false, 0u); break;
        case 5: emitted = psc.publish("t", "x", vector.retained, 0u); break;
        case 6: {
            MqttPublishRequest request;
            request.topic = "t";
            request.payload = &bytePayload;
            request.plength = 1u;
            request.qos = 0u;
            request.properties.contentType = "ignored-in-v311";
            emitted = psc.publish(request);
            break;
        }
        case 7: emitted = psc.publish_P("t", "x", vector.retained); break;
        case 8: emitted = psc.publish_P("t", &bytePayload, 1u, false); break;
        case 9:
            REQUIRE(psc.beginPublish("t", 1u, false));
            REQUIRE(psc.write(static_cast<uint8_t>('x')) == 1u);
            emitted = psc.endPublish() == 1;
            break;
        default:
            REQUIRE(psc.beginPublish("t", 1u, false));
            REQUIRE(psc.write(&bytePayload, 1u) == 1u);
            emitted = psc.endPublish() == 1;
            break;
        }
        REQUIRE(emitted);
        requireBytes(client, vector.retained
                                 ? std::initializer_list<uint8_t>{0x31, 0x04, 0x00, 0x01, 't', 'x'}
                                 : std::initializer_list<uint8_t>{0x30, 0x04, 0x00, 0x01, 't', 'x'});
    }

    // MQTT 3.1.1 has no outbound QoS 1/2 machinery. The additive entry points
    // exist for source compatibility, but must reject any QoS above zero before
    // producing a partial PUBLISH.
    for (uint8_t qos : {1u, 2u}) {
        CAPTURE(qos);
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);
        const uint8_t payload = 'x';
        CHECK_FALSE(psc.publish("t", &payload, 1u, false, qos));
        CHECK(client.outbound().empty());
        CHECK(psc.connected());
    }

    // Every subscription overload retains the old packet body: Packet Identifier,
    // topic filter and trailing requested-QoS byte. QoS values above one are a
    // total refusal in this selection.
    {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);
        REQUIRE(psc.subscribe("t"));
        requireBytes(client, {0x82, 0x06, 0x00, 0x02, 0x00, 0x01, 't', 0x00});
    }
    {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);
        REQUIRE(psc.subscribe("t", 1u));
        requireBytes(client, {0x82, 0x06, 0x00, 0x02, 0x00, 0x01, 't', 0x01});
    }
    {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);
        MqttSubscribeOptions options;
        options.maxQos = 1u;
        REQUIRE(psc.subscribe("t", options));
        requireBytes(client, {0x82, 0x06, 0x00, 0x02, 0x00, 0x01, 't', 0x01});
        client.clearOutbound();
        options.maxQos = 2u;
        CHECK_FALSE(psc.subscribe("t", options));
        CHECK(client.outbound().empty());
        CHECK(psc.connected());
    }
    {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);
        REQUIRE(psc.unsubscribe("t"));
        requireBytes(client, {0xA2, 0x05, 0x00, 0x02, 0x00, 0x01, 't'});
        client.clearOutbound();
        CHECK_FALSE(psc.subscribe("t", 2u));
        CHECK_FALSE(psc.subscribe("t", 3u));
        CHECK(client.outbound().empty());
        CHECK(psc.connected());
    }

    // A packet whose Remaining Length would require a third byte is refused in
    // both selections; nothing is sent and a live connection remains usable.
    {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);
        CHECK_FALSE(psc.beginPublish("t", 16381u, false)); // 2 + 1 + 16381 = 16384
        CHECK(client.outbound().empty());
        CHECK(psc.connected());
    }

    // loop() preserves the MQTT 3.1.1 PING and QoS 1 acknowledgement forms:
    // no reason-code byte and no Properties field are emitted.
    {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        psc.setKeepAlive(1u);
        connectAndClear(client, psc);
        TestClock::instance().advance(2001UL);
        REQUIRE(psc.loop());
        requireBytes(client, {0xC0, 0x00});
        client.clearOutbound();
        client.pushInbound({0xD0, 0x00});
        REQUIRE(psc.loop());
        CHECK(client.outbound().empty());
    }
    {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);
        // The legacy parser invokes its QoS 1 acknowledgement path only while
        // dispatching to an installed callback.
        psc.setCallback([](char*, uint8_t*, unsigned int) {});
        client.pushPacket(MqttPacket::publish("t", std::vector<uint8_t>{'x'}, 1u, false,
                                              0x1234u));
        REQUIRE(psc.loop());
        requireBytes(client, {0x40, 0x02, 0x12, 0x34});
        client.clearOutbound();
        client.pushInbound({0xC0, 0x00});
        REQUIRE(psc.loop());
        requireBytes(client, {0xD0, 0x00});
    }

    // The explicit legacy DISCONNECT request sends E0 00. The default still
    // closes locally without sending a packet, which remains source-compatible.
    {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);
        psc.disconnect(true);
        requireBytes(client, {0xE0, 0x00});
        CHECK_FALSE(psc.connected());
    }
    {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);
        psc.disconnect();
        CHECK(client.outbound().empty());
        CHECK_FALSE(psc.connected());
    }
}

#endif  // MQTT_VERSION == MQTT_VERSION_3_1_1
