/*
  mqtt5_runtime_protocol_test.cpp - Per-instance MQTT 5 to 3.1.1 fallback.

  Feature: tasmota-pubsub-mqtt5, Requirement 24
*/

#include <cstdint>
#include <vector>

#include "doctest.h"
#include "MockClient.h"
#include "MockStream.h"
#include "PubSubClient.h"
#include "TestClock.h"

TEST_SUITE("mqtt5") {

#if MQTT_VERSION == MQTT_VERSION_5_0

namespace runtime_protocol {

using Bytes = std::vector<uint8_t>;

const Bytes kConnect5 = {
    0x10, 0x17,
    0x00, 0x04, 'M', 'Q', 'T', 'T', 0x05, 0x02, 0x00, 0x1E,
    0x08, 0x21, 0x00, 0x08, 0x27, 0x00, 0x00, 0x04, 0xAF,
    0x00, 0x02, 'i', 'd'
};

const Bytes kConnect311 = {
    0x10, 0x0E,
    0x00, 0x04, 'M', 'Q', 'T', 'T', 0x04, 0x02, 0x00, 0x1E,
    0x00, 0x02, 'i', 'd'
};

const Bytes kConnack5Success = {0x20, 0x03, 0x00, 0x00, 0x00};
const Bytes kConnack5Unsupported = {0x20, 0x03, 0x00, 0x84, 0x00};
const Bytes kConnack311Unsupported = {0x20, 0x02, 0x00, 0x01};
const Bytes kConnack311Success = {0x20, 0x02, 0x00, 0x00};

Bytes joined(const Bytes& first, const Bytes& second) {
    Bytes result = first;
    result.insert(result.end(), second.begin(), second.end());
    return result;
}

void configure(PubSubClient& client) {
    client.setServer("broker.example", 1883);
    client.setKeepAlive(30);
    client.setSocketTimeout(1);
}

}  // namespace runtime_protocol

// Property 33: only explicit Unsupported Version evidence authorizes fallback.
TEST_CASE("Property 33: explicit unsupported-version evidence falls back on a fresh transport") {
    using namespace runtime_protocol;

    const Bytes evidence[] = {
        kConnack5Unsupported,
        // Valid 0x84 refusals with legal non-empty property sets.
        {0x20, 0x06, 0x00, 0x84, 0x03, 0x21, 0x00, 0x01},
        {0x20, 0x05, 0x00, 0x84, 0x02, 0x24, 0x01},
        kConnack311Unsupported,
    };

    for (const Bytes& refusal : evidence) {
        CAPTURE(refusal);
        TestClock::instance().reset();
        MockClient transport;
        PubSubClient client(transport);
        configure(client);
        transport.pushInbound(refusal);
        transport.pushInbound(kConnack311Success);

        REQUIRE(client.protocolVersion() == MQTT_VERSION_5_0);
        REQUIRE(client.connect("id"));
        CHECK(client.protocolVersion() == MQTT_VERSION_3_1_1);
        CHECK(client.state() == MQTT_CONNECTED);
        CHECK(client.connackReasonCode() == 0);
        CHECK(transport.connectCount() == 2);
        CHECK(transport.stopCount() == 1);
        CHECK(transport.connectionEvents() ==
              std::vector<MockClient::ConnectionEvent>{
                  MockClient::ConnectionEvent::Connect,
                  MockClient::ConnectionEvent::Stop,
                  MockClient::ConnectionEvent::Connect,
              });
        CHECK(transport.outbound() == joined(kConnect5, kConnect311));

        // Runtime level 4 controls the whole protocol, not just CONNECT.
        transport.clearOutbound();
        REQUIRE(client.publish("t", "x"));
        REQUIRE(client.subscribe("t", 1));
        REQUIRE(client.unsubscribe("t"));
        CHECK(transport.outbound() == Bytes{
            0x30, 0x04, 0x00, 0x01, 't', 'x',
            0x82, 0x06, 0x00, 0x02, 0x00, 0x01, 't', 0x01,
            0xA2, 0x05, 0x00, 0x03, 0x00, 0x01, 't'
        });

        transport.clearOutbound();
        REQUIRE(client.beginPublish("t", 1, false));
        REQUIRE(client.write(static_cast<uint8_t>('x')) == 1);
        REQUIRE(client.endPublish() == 1);
        CHECK(transport.outbound() == Bytes{0x30, 0x04, 0x00, 0x01, 't', 'x'});

        transport.clearOutbound();
        const uint8_t payload = 'x';
        CHECK_FALSE(client.publish("t", &payload, 1, false, 1));
        MqttSubscribeOptions mqtt5Options;
        mqtt5Options.noLocal = true;
        CHECK_FALSE(client.subscribe("t", mqtt5Options));
        CHECK(transport.outbound().empty());

        client.setCallback([](char*, uint8_t*, unsigned int) {});
        transport.pushInbound({0x32, 0x06, 0x00, 0x01, 't', 0x12, 0x34, 'x'});
        REQUIRE(client.loop());
        CHECK(transport.outbound() == Bytes{0x40, 0x02, 0x12, 0x34});

        transport.clearOutbound();
        client.disconnect(true);
        CHECK(transport.outbound() == Bytes{0xE0, 0x00});
    }
}

TEST_CASE("Property 34: ambiguous failures and other refusals never downgrade") {
    using namespace runtime_protocol;

    SUBCASE("transport open failure") {
        TestClock::instance().reset();
        MockClient transport;
        transport.setConnectResult(0);
        PubSubClient client(transport);
        configure(client);

        CHECK_FALSE(client.connect("id"));
        CHECK(client.protocolVersion() == MQTT_VERSION_5_0);
        CHECK(transport.connectCount() == 1);
        CHECK(transport.outbound().empty());
    }

    SUBCASE("CONNACK timeout") {
        TestClock::instance().reset();
        MockClient transport;
        PubSubClient client(transport);
        configure(client);

        CHECK_FALSE(client.connect("id"));
        CHECK(client.state() == MQTT_CONNECTION_TIMEOUT);
        CHECK(client.protocolVersion() == MQTT_VERSION_5_0);
        CHECK(transport.connectCount() == 1);
        CHECK(transport.outbound() == kConnect5);
    }

    SUBCASE("malformed near matches") {
        const Bytes malformed[] = {
            {0x21, 0x03, 0x00, 0x84, 0x00},                 // fixed-header flags
            {0x20, 0x03, 0x01, 0x84, 0x00},                 // Session Present on refusal
            {0x20, 0x04, 0x00, 0x84, 0x00, 0xFF},           // trailing byte
            {0x20, 0x83, 0x00, 0x00, 0x84, 0x00},           // non-minimal Remaining Length
            {0x20, 0x04, 0x00, 0x84, 0x80, 0x00},           // non-minimal Property Length
            {0x20, 0x06, 0x00, 0x84, 0x03, 0x21, 0x00, 0x00}, // Receive Maximum zero
            {0x20, 0x08, 0x00, 0x84, 0x05, 0x27, 0, 0, 0, 0}, // Maximum Packet Size zero
            {0x20, 0x05, 0x00, 0x84, 0x02, 0x24, 0x02},     // Maximum QoS outside 0..1
            {0x20, 0x05, 0x00, 0x84, 0x02, 0x25, 0x02},     // boolean property outside 0..1
            {0x20, 0x02, 0x01, 0x01},                       // legacy refusal flags
        };
        for (const Bytes& reply : malformed) {
            CAPTURE(reply);
            TestClock::instance().reset();
            MockClient transport;
            PubSubClient client(transport);
            configure(client);
            transport.pushInbound(reply);

            CHECK_FALSE(client.connect("id"));
            CHECK(client.protocolVersion() == MQTT_VERSION_5_0);
            CHECK(transport.connectCount() == 1);
            CHECK(transport.outbound() == kConnect5);
        }
    }

    SUBCASE("oversized streamed CONNACK prefixes never connect or downgrade") {
        for (uint8_t reason : {uint8_t(0x00), uint8_t(0x84)}) {
            CAPTURE(reason);
            TestClock::instance().reset();
            MockClient transport;
            MockStream stream;
            PubSubClient client(transport);
            configure(client);
            REQUIRE(client.setBufferSize(32));
            client.setStream(stream);

            // A complete-looking flags/reason/properties prefix fits the buffer, but the
            // declared CONNACK body has trailing bytes that readPacket() consumes without
            // retaining in stream mode. The parser must compare the declaration with the
            // retained length instead of accepting that prefix as a complete packet.
            Bytes oversized = {0x20, 0x28, 0x00, reason, 0x00};
            oversized.resize(42, 0xFF);  // fixed header + RL byte + 40-byte body
            transport.pushInbound(oversized);

            CHECK_FALSE(client.connect("id"));
            CHECK(client.protocolVersion() == MQTT_VERSION_5_0);
            CHECK(transport.connectCount() == 1);
            CHECK(client.lastError() == MqttError::MalformedPacket);
        }
    }

    SUBCASE("valid non-version refusals") {
        const uint8_t reasons[] = {0x85, 0x86, 0x87, 0x88, 0x89};
        for (uint8_t reason : reasons) {
            CAPTURE(reason);
            TestClock::instance().reset();
            MockClient transport;
            PubSubClient client(transport);
            configure(client);
            transport.pushInbound({0x20, 0x03, 0x00, reason, 0x00});

            CHECK_FALSE(client.connect("id"));
            CHECK(client.connackReasonCode() == reason);
            CHECK(client.protocolVersion() == MQTT_VERSION_5_0);
            CHECK(transport.connectCount() == 1);
            CHECK(transport.outbound() == kConnect5);
        }
    }

    SUBCASE("the fallback transport open is fresh and may fail independently") {
        TestClock::instance().reset();
        MockClient transport;
        transport.pushConnectResult(1);
        transport.pushConnectResult(0);
        PubSubClient client(transport);
        configure(client);
        transport.pushInbound(kConnack5Unsupported);

        CHECK_FALSE(client.connect("id"));
        CHECK(client.state() == MQTT_CONNECT_FAILED);
        CHECK(client.protocolVersion() == MQTT_VERSION_3_1_1);
        CHECK(transport.connectCount() == 2);
        CHECK(transport.outbound() == kConnect5);
        CHECK(transport.connectionEvents() ==
              std::vector<MockClient::ConnectionEvent>{
                  MockClient::ConnectionEvent::Connect,
                  MockClient::ConnectionEvent::Stop,
                  MockClient::ConnectionEvent::Connect,
              });
    }

    SUBCASE("a failed fallback is final and leaves level 4 selected") {
        TestClock::instance().reset();
        MockClient transport;
        PubSubClient client(transport);
        configure(client);
        transport.pushInbound(kConnack5Unsupported);

        CHECK_FALSE(client.connect("id"));
        CHECK(client.state() == MQTT_CONNECTION_TIMEOUT);
        CHECK(client.protocolVersion() == MQTT_VERSION_3_1_1);
        CHECK(transport.connectCount() == 2);
        CHECK(transport.outbound() == joined(kConnect5, kConnect311));
    }
}

TEST_CASE("Property 35: learned protocol is remembered per instance") {
    using namespace runtime_protocol;

    TestClock::instance().reset();
    MockClient transportA;
    PubSubClient clientA(transportA);
    configure(clientA);
    transportA.pushInbound(kConnack5Unsupported);
    transportA.pushInbound(kConnack311Success);
    REQUIRE(clientA.connect("id"));
    REQUIRE(clientA.protocolVersion() == MQTT_VERSION_3_1_1);

    clientA.disconnect();
    transportA.clearOutbound();
    transportA.pushInbound(kConnack311Success);
    const unsigned attemptsBeforeReconnect = transportA.connectCount();
    REQUIRE(clientA.connect("id"));
    CHECK(transportA.connectCount() == attemptsBeforeReconnect + 1);
    CHECK(transportA.outbound() == kConnect311);

    MockClient transportB;
    PubSubClient clientB(transportB);
    configure(clientB);
    transportB.pushInbound(kConnack5Success);
    REQUIRE(clientB.protocolVersion() == MQTT_VERSION_5_0);
    REQUIRE(clientB.connect("id"));
    CHECK(clientB.protocolVersion() == MQTT_VERSION_5_0);
    CHECK(transportB.connectCount() == 1);
    CHECK(transportB.outbound() == kConnect5);
}

#ifdef USE_MQTT_QOS
// This case establishes MQTT 5 local session state through an in-flight QoS 1 publish, so
// it only applies to a build with outbound QoS 1/2 compiled in.
TEST_CASE("Property 36: fallback invalidates MQTT 5 local session state") {
    using namespace runtime_protocol;

    TestClock::instance().reset();
    MockClient transport;
    PubSubClient client(transport);
    configure(client);

    transport.pushInbound(kConnack5Success);
    REQUIRE(client.connect("id", nullptr, nullptr, nullptr, 0, false, nullptr, false));
    const uint8_t payload = 'x';
    REQUIRE(client.publish("held", &payload, 1, false, 1));
    REQUIRE(client.inFlightCount() == 1);

    client.disconnect();
    REQUIRE(client.inFlightCount() == 1);
    transport.clearOutbound();
    transport.pushInbound(kConnack5Unsupported);
    transport.pushInbound(kConnack311Success);

    REQUIRE(client.connect("id", nullptr, nullptr, nullptr, 0, false, nullptr, false));
    CHECK(client.protocolVersion() == MQTT_VERSION_3_1_1);
    CHECK(client.inFlightCount() == 0);
}
#endif  // USE_MQTT_QOS

#endif  // MQTT_VERSION == MQTT_VERSION_5_0

}  // TEST_SUITE("mqtt5")
