/*
  mqtt5_framing_test.cpp - MQTT 5.0 outbound structural framing (task 16.2).

  Feature: tasmota-pubsub-mqtt5

  This is deliberately one cross-entry-point property rather than a replacement
  for the detailed packet-specific tests.  It drives every public path that can
  emit an MQTT 5 control packet and validates the captured bytes with the
  harness's independent structural parser.
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

namespace mqtt5_framing {

void requireFramed(const std::vector<uint8_t>& bytes) {
    REQUIRE_FALSE(bytes.empty());
    CHECK(MqttParser::isStructurallyWellFramed(bytes));
}

void connectAndClear(MockClient& client, PubSubClient& psc, const char* id = "mqtt5-frame") {
    client.pushPacket(MqttPacket::connack(0));
    psc.setServer("broker.example", 1883);
    REQUIRE(psc.connect(id));
    REQUIRE(psc.connected());
    client.clearOutbound();
    psc.setSocketTimeout(1);
}

#ifdef USE_MQTT_QOS
uint16_t publishQos2AndReadId(MockClient& client, PubSubClient& psc) {
    const uint8_t payload = 'q';
    REQUIRE(psc.publish("tele/frame/qos2", &payload, 1u, false, 2u));
    requireFramed(client.outbound());
    const DecodedPublish5 decoded = MqttParser::decodePublish5(client.outbound());
    REQUIRE(decoded.valid);
    REQUIRE(decoded.msgId != 0u);
    return decoded.msgId;
}
#endif  // USE_MQTT_QOS

}  // namespace mqtt5_framing

// Feature: tasmota-pubsub-mqtt5, Property 4: For all public-API operations that emit a packet (connect, all publish paths including the streaming one, subscribe, unsubscribe, every acknowledgement, keepalive ping, disconnect), the recorded outbound bytes decode into complete control packets whose declared Remaining Length exactly equals the number of trailing bytes actually present, and whose Properties field's declared length exactly equals its actual body byte count.
//
// Property 4: Outbound packets are structurally well-framed
//
// Validates: Requirements 1.2, 1.3, 6.1, 6.2, 6.3, 7.1, 7.2, 7.6, 7.7, 9.1,
// 9.2, 9.3, 10.3, 10.4, 13.1
TEST_CASE("Property 4: outbound packets are structurally well-framed") {
    using namespace mqtt5_framing;

    // Every CONNECT overload emits one complete CONNECT. The Will cases make
    // the independent walker consume both the variable-header and payload
    // Properties fields.
    for (uint8_t form = 0; form < 5; ++form) {
        CAPTURE(form);
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        client.pushPacket(MqttPacket::connack(0));
        psc.setServer("broker.example", 1883);

        boolean connected = false;
        switch (form) {
        case 0: connected = psc.connect("frame-connect"); break;
        case 1: connected = psc.connect("frame-connect", "user", "pass"); break;
        case 2: connected = psc.connect("frame-connect", "will/topic", 1, true, "will"); break;
        case 3:
            connected = psc.connect("frame-connect", "user", "pass", "will/topic", 1,
                                    true, "will");
            break;
        default:
            connected = psc.connect("frame-connect", "user", "pass", "will/topic", 1,
                                    true, "will", false);
            break;
        }
        REQUIRE(connected);
        requireFramed(client.outbound());
    }

    // The table covers each buffered, PROGMEM, QoS-capable, request, and
    // streaming publish entry point. Payload lengths cover 0, 1, 127 and 128;
    // the final streaming vector reaches the 16,383 Remaining Length boundary.
    enum class PublishPath {
        CString,
        CStringRetained,
        Bytes,
        BytesRetained,
        BytesQos,
        CStringQos,
        Request,
        ProgmemCString,
        ProgmemBytes,
        StreamingByte,
        StreamingBuffer
    };
    struct PublishVector {
        PublishPath path;
        size_t payloadLength;
        bool retained;
        uint8_t qos;
    };
    const PublishVector publishes[] = {
        {PublishPath::CString, 0u, false, 0u},
        {PublishPath::CStringRetained, 1u, true, 0u},
        {PublishPath::Bytes, 127u, false, 0u},
        {PublishPath::BytesRetained, 128u, true, 0u},
#ifdef USE_MQTT_QOS
        {PublishPath::BytesQos, 1u, false, 1u},
        {PublishPath::CStringQos, 1u, true, 2u},
        {PublishPath::Request, 128u, false, 2u},
#endif  // USE_MQTT_QOS
        {PublishPath::ProgmemCString, 1u, false, 0u},
        {PublishPath::ProgmemBytes, 127u, true, 0u},
        {PublishPath::StreamingByte, 1u, false, 0u},
        // Remaining Length = 2-byte topic length + one-byte topic + empty
        // Properties + payload. This is the maximum complete packet Property 4
        // can frame under the deliberately narrowed VBI contract.
        {PublishPath::StreamingBuffer, 16379u, false, 0u},
    };

    for (const PublishVector& v : publishes) {
        CAPTURE(static_cast<unsigned>(v.path));
        CAPTURE(v.payloadLength);
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);

        const std::string topic = "t";
        const std::string payload(v.payloadLength, 'x');
        boolean emitted = false;
        switch (v.path) {
        case PublishPath::CString:
            emitted = psc.publish(topic.c_str(), payload.c_str());
            break;
        case PublishPath::CStringRetained:
            emitted = psc.publish(topic.c_str(), payload.c_str(), v.retained);
            break;
        case PublishPath::Bytes:
            emitted = psc.publish(topic.c_str(),
                                  reinterpret_cast<const uint8_t*>(payload.data()),
                                  static_cast<unsigned int>(payload.size()));
            break;
        case PublishPath::BytesRetained:
            emitted = psc.publish(topic.c_str(),
                                  reinterpret_cast<const uint8_t*>(payload.data()),
                                  static_cast<unsigned int>(payload.size()), v.retained);
            break;
        case PublishPath::BytesQos:
            emitted = psc.publish(topic.c_str(),
                                  reinterpret_cast<const uint8_t*>(payload.data()),
                                  static_cast<unsigned int>(payload.size()), v.retained, v.qos);
            break;
        case PublishPath::CStringQos:
            emitted = psc.publish(topic.c_str(), payload.c_str(), v.retained, v.qos);
            break;
        case PublishPath::Request: {
            MqttPublishRequest request;
            request.topic = topic.c_str();
            request.payload = reinterpret_cast<const uint8_t*>(payload.data());
            request.plength = static_cast<unsigned int>(payload.size());
            request.retained = v.retained;
            request.qos = v.qos;
            request.properties.contentType = "text/plain";
            emitted = psc.publish(request);
            break;
        }
        case PublishPath::ProgmemCString:
            emitted = psc.publish_P(topic.c_str(), payload.c_str(), v.retained);
            break;
        case PublishPath::ProgmemBytes:
            emitted = psc.publish_P(topic.c_str(),
                                    reinterpret_cast<const uint8_t*>(payload.data()),
                                    static_cast<unsigned int>(payload.size()), v.retained);
            break;
        case PublishPath::StreamingByte:
            REQUIRE(psc.beginPublish(topic.c_str(), 1u, v.retained));
            REQUIRE(psc.write(static_cast<uint8_t>('x')) == 1u);
            emitted = psc.endPublish() == 1;
            break;
        case PublishPath::StreamingBuffer:
            REQUIRE(psc.beginPublish(topic.c_str(), static_cast<unsigned int>(payload.size()),
                                     v.retained));
            REQUIRE(psc.write(reinterpret_cast<const uint8_t*>(payload.data()), payload.size()) ==
                    payload.size());
            emitted = psc.endPublish() == 1;
            break;
        }
        REQUIRE(emitted);
        requireFramed(client.outbound());
    }

    // The buffered PUBLISH path must still frame successfully at its exact
    // working-buffer capacity, not merely at the default capacity.
    {
        const std::string topic = "b";
        const std::string payload(190u, 'b');
        const uint16_t exactCapacity = static_cast<uint16_t>(
            MQTT_MAX_HEADER_SIZE + 2u + topic.size() + 1u + payload.size());
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);
        REQUIRE(psc.setBufferSize(exactCapacity));
        REQUIRE(psc.publish(topic.c_str(), reinterpret_cast<const uint8_t*>(payload.data()),
                            static_cast<unsigned int>(payload.size()), false));
        requireFramed(client.outbound());
    }

    // Every subscription emitter has a Properties field after its Packet
    // Identifier. The exact-fit SUBSCRIBE case covers its buffer boundary too.
    for (uint8_t form = 0; form < 4; ++form) {
        CAPTURE(form);
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);

        const std::string filter = (form == 2) ? std::string(20u, 's') : "tele/frame/#";
        boolean emitted = false;
        if (form == 0) {
            emitted = psc.subscribe(filter.c_str());
        } else if (form == 1) {
            emitted = psc.subscribe(filter.c_str(), 2u);
        } else if (form == 2) {
            MqttSubscribeOptions options;
            options.maxQos = 2;
            options.noLocal = true;
            const uint16_t exactCapacity = static_cast<uint16_t>(11u + filter.size());
            REQUIRE(psc.setBufferSize(exactCapacity));
            emitted = psc.subscribe(filter.c_str(), options);
        } else {
            emitted = psc.unsubscribe(filter.c_str());
        }
        REQUIRE(emitted);
        requireFramed(client.outbound());
    }

    // Inbound QoS 1 and QoS 2 traffic drives PUBACK, PUBREC, and PUBCOMP;
    // receiving PUBREC for an outbound QoS 2 PUBLISH drives PUBREL.
    {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);
        client.pushPacket(MqttPacket::publish5("tele/frame/ack", {'a'}, 1u, false, false,
                                               0x0101u));
        REQUIRE(psc.loop());
        requireFramed(client.outbound());  // PUBACK
    }
    {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);
        client.pushPacket(MqttPacket::publish5("tele/frame/ack", {'a'}, 2u, false, false,
                                               0x0102u));
        REQUIRE(psc.loop());
        requireFramed(client.outbound());  // PUBREC
        client.clearOutbound();
        client.pushPacket(MqttPacket::ack5(MQTTPUBREL, 0x0102u, -1));
        REQUIRE(psc.loop());
        requireFramed(client.outbound());  // PUBCOMP
    }
#ifdef USE_MQTT_QOS
    {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);
        const uint16_t packetId = publishQos2AndReadId(client, psc);
        client.clearOutbound();
        client.pushPacket(MqttPacket::ack5(MQTTPUBREC, packetId, -1));
        REQUIRE(psc.loop());
        requireFramed(client.outbound());  // PUBREL
    }
#endif  // USE_MQTT_QOS

    // A keepalive PINGREQ and an explicit normal DISCONNECT are both framed
    // control packets with Remaining Length zero.
    {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        psc.setKeepAlive(2u);
        connectAndClear(client, psc);
        TestClock::instance().advance(3000UL);
        REQUIRE(psc.loop());
        requireFramed(client.outbound());
    }
    {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);
        psc.disconnect(true);
        requireFramed(client.outbound());
    }
}

#endif  // MQTT_VERSION == MQTT_VERSION_5_0

}  // TEST_SUITE("mqtt5")
