/*
  mqtt5_malformed_test.cpp - Generated malformed MQTT 5.0 corpus (task 15.4).

  Feature: tasmota-pubsub-mqtt5

  Property 22 drives every systematically generated malformed inbound fixture
  through the real receive path with the Tasmota callback contract adapter
  installed. Sanitizers are enabled by the host Makefile, so the adapter's
  mqtt_data[data_len] = 0 write is included in the bounds check.
*/

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "doctest.h"

#include "CallbackContractAdapter.h"
#include "MockClient.h"
#include "MqttCorpus.h"
#include "MqttPacket.h"
#include "PubSubClient.h"
#include "TestClock.h"

TEST_SUITE("mqtt5") {

#if MQTT_VERSION == MQTT_VERSION_5_0

namespace mqtt5_malformed {

void connect(MockClient& client, PubSubClient& psc, CallbackContractAdapter& adapter) {
    client.pushPacket(MqttPacket::connack5(0));
    psc.setServer("broker.example", 1883);
    psc.setCallback(adapter.callback());
    REQUIRE(psc.connect("mqtt5-malformed"));
    REQUIRE(psc.connected());
    client.clearOutbound();
    psc.setSocketTimeout(1);
}

bool isConnackEntry(const MqttCorpusEntry& entry) {
    return entry.label.rfind("CONNACK", 0) == 0;
}

bool isAuthenticationEntry(const MqttCorpusEntry& entry) {
    return entry.label.find("authentication property") != std::string::npos;
}

bool isOverCapCorrelationDataEntry(const MqttCorpusEntry& entry) {
    return entry.label.find("Correlation Data") != std::string::npos;
}

void checkRejectedInboundProperties(const PubSubClient& psc) {
    // Failed property parsing leaves no published property set. In particular,
    // an over-cap Correlation Data value cannot have reached the client's fixed
    // 16-byte correlationDataStorage array.
    const MqttInboundProperties& properties = psc.inboundProperties();
    CHECK_FALSE(properties.valid);
    CHECK_FALSE(properties.hasCorrelationData);
    CHECK(properties.correlationData == nullptr);
    CHECK(properties.correlationDataLen == 0);
}

}  // namespace mqtt5_malformed

// Feature: tasmota-pubsub-mqtt5, Property 22: For all inbound byte sequences produced
// by mutating or truncating a well-formed packet of any type — including Variable Byte
// Integers that do not terminate within 2 bytes or decode above 16,383, Remaining
// Lengths whose encoding exceeds 2 bytes or which exceed the configured cap, Property
// Lengths that overrun or under-run the received bytes, property identifiers illegal for
// their packet type, repeated single-occurrence properties, Authentication Method and
// Authentication Data properties in any packet type, Correlation Data properties
// declaring more than 16 bytes, topic or Packet Identifier lengths that overrun the
// packet, QoS 3 PUBLISH packets, packet types a client may not receive, AUTH packets,
// and malformed acknowledgements — the client rejects the packet, does not invoke the
// registered callback, does not read or write any buffer byte beyond the bytes actually
// received, reports the failure through the public error indication, and closes the
// connection, with zero sanitizer-reported violations.
TEST_CASE("Property 22: generated malformed inbound corpus is rejected safely") {
    const std::vector<MqttCorpusEntry> corpus = mqtt5MalformedCorpus();
    REQUIRE_FALSE(corpus.empty());

    for (const MqttCorpusEntry& entry : corpus) {
        CAPTURE(entry.label);
        CAPTURE(entry.packet.size());
        TestClock::instance().reset();

        MockClient client;
        PubSubClient psc(client);
        CallbackContractAdapter adapter;

        // CONNACK is only meaningful during CONNECT. The client intentionally
        // closes that failed handshake without an error DISCONNECT because no
        // MQTT session has been established yet.
        if (mqtt5_malformed::isConnackEntry(entry)) {
            psc.setServer("broker.example", 1883);
            psc.setCallback(adapter.callback());
            client.pushPacket(entry.packet);

            CHECK_FALSE(psc.connect("mqtt5-malformed-connack"));
            CHECK(adapter.count() == 0);
            const bool wrongWidth = std::string(entry.label).find("wrong-width") != std::string::npos;
            const MqttError expectedConnackError =
                (entry.expectedError == MqttError::ReceiveTimeout || wrongWidth)
                    ? MqttError::MalformedPacket
                    : entry.expectedError;
            CHECK(psc.lastError() == expectedConnackError);
            CHECK_FALSE(psc.connected());
            CHECK(client.stopCalled());
            mqtt5_malformed::checkRejectedInboundProperties(psc);
            continue;
        }

        mqtt5_malformed::connect(client, psc, adapter);
        client.pushPacket(entry.packet);

        CHECK_FALSE(psc.loop());
        CHECK(adapter.count() == 0);
        CHECK(psc.lastError() == entry.expectedError);
        CHECK(psc.lastReasonCode() == entry.expectedDisconnectReason);
        CHECK_FALSE(psc.connected());
        CHECK(client.stopCalled());

        if (entry.expectedDisconnectReason == 0) {
            // A proper prefix starts a transport read but never forms a complete
            // packet. The receive timeout is a local transport close, so it is
            // deliberately not followed by a protocol DISCONNECT.
            CHECK(client.outbound().empty());
        } else {
            // The transport is writable for every complete malformed frame, so
            // protocolFailure emits its error DISCONNECT before closing.
            const std::vector<uint8_t> expectedDisconnect = {
                static_cast<uint8_t>(MQTTDISCONNECT), 0x01, entry.expectedDisconnectReason};
            CHECK(client.outbound() == expectedDisconnect);
        }

        mqtt5_malformed::checkRejectedInboundProperties(psc);

        if (mqtt5_malformed::isAuthenticationEntry(entry)) {
            // Authentication identifiers are rejected at property dispatch,
            // before their UTF-8 or Binary Data value is decoded.
            CHECK(entry.expectedError == MqttError::ProtocolError);
            CHECK(entry.expectedDisconnectReason == PubSubClient::MQTT_RC_BAD_AUTH_METHOD);
            CHECK_FALSE(psc.inboundProperties().valid);
        }

        if (mqtt5_malformed::isOverCapCorrelationDataEntry(entry)) {
            const bool rejectsCorrelationData =
                entry.expectedError == MqttError::ProtocolError ||
                entry.expectedError == MqttError::MalformedPacket;
            CHECK(rejectsCorrelationData);
            CHECK_FALSE(psc.inboundProperties().hasCorrelationData);
            CHECK(psc.inboundProperties().correlationData == nullptr);
            CHECK(psc.inboundProperties().correlationDataLen == 0);
        }
    }
}

// Feature: tasmota-pubsub-mqtt5, Property 23: For all protocol-error triggers,
// when the network connection is writable the client emits exactly one DISCONNECT
// carrying a Reason Code of 0x80 or above before closing, and when it is not
// writable it closes without emitting any bytes; in both cases the error is
// recorded through the public error indication and a subsequent connection
// attempt can proceed normally.
TEST_CASE("Property 23: protocol-error closure emits a DISCONNECT only when writable") {
    struct ProtocolFailureTrigger {
        const char* label;
        MqttError error;
        uint8_t reasonCode;
    };

    // These are the complete wire-level classes routed through protocolFailure().
    // Property 22 exercises the malformed corpus members that select the first
    // three; this matrix also covers the registry-full and inbound-size classes.
    const std::array<ProtocolFailureTrigger, 5> triggers = {{
        {"malformed packet", MqttError::MalformedPacket,
         PubSubClient::MQTT_RC_MALFORMED_PACKET},
        {"protocol violation", MqttError::ProtocolError,
         PubSubClient::MQTT_RC_PROTOCOL_ERROR},
        {"enhanced authentication", MqttError::ProtocolError,
         PubSubClient::MQTT_RC_BAD_AUTH_METHOD},
        {"receive maximum exceeded", MqttError::ReceiveMaximumReached,
         PubSubClient::MQTT_RC_RECEIVE_MAX_EXCEEDED},
        {"inbound packet too large", MqttError::PacketTooLarge,
         PubSubClient::MQTT_RC_PACKET_TOO_LARGE},
    }};
    const std::array<bool, 2> writability = {{true, false}};

    for (const ProtocolFailureTrigger& trigger : triggers) {
        for (const bool writable : writability) {
            CAPTURE(trigger.label);
            CAPTURE(trigger.reasonCode);
            CAPTURE(writable);
            TestClock::instance().reset();

            MockClient client;
            PubSubClient psc(client);
            CallbackContractAdapter adapter;
            mqtt5_malformed::connect(client, psc, adapter);

            // A disconnected socket cannot deliver another packet to loop(), so
            // drive the same public failure funnel that an inbound trigger uses.
            if (!writable) {
                client.setConnected(false);
            }
            psc.protocolFailure(trigger.error, trigger.reasonCode);

            CHECK(psc.lastError() == trigger.error);
            CHECK(psc.lastReasonCode() == trigger.reasonCode);
            CHECK_FALSE(psc.connected());
            CHECK(client.stopCalled());
            CHECK(trigger.reasonCode >= 0x80);

            if (writable) {
                // Equality proves there was exactly one three-byte MQTT 5
                // DISCONNECT, its Reason Code is a failure code, and no byte
                // followed it before the transport closed.
                const std::vector<uint8_t> expectedDisconnect = {
                    static_cast<uint8_t>(MQTTDISCONNECT), 0x01, trigger.reasonCode};
                CHECK(client.outbound() == expectedDisconnect);
            } else {
                CHECK(client.outbound().empty());
            }

            // protocolFailure() must leave no sticky state that stops the
            // driver's next connection attempt, regardless of socket writability.
            client.clearOutbound();
            client.setConnected(true);
            client.pushPacket(MqttPacket::connack5(0));
            CHECK(psc.connect("mqtt5-protocol-reconnect"));
            CHECK(psc.connected());
        }
    }
}

// Feature: tasmota-pubsub-mqtt5, Property 25: For all incremental (trickled)
// deliveries of an inbound packet, the elapsed time from the first received byte
// to either packet completion or abandonment is bounded by the single packet
// deadline regardless of the per-byte pacing or the packet size; and when the
// deadline elapses first, the partial packet is abandoned and the connection closed.
TEST_CASE("Property 25: a packet-wide deadline bounds incremental receipt") {
    struct TrickleCase {
        const char* label;
        size_t bytesPerReveal;
        unsigned long msPerReveal;
        size_t payloadSize;
        bool completes;
    };

    constexpr unsigned long kPacketDeadlineMs = 2000;
    const std::array<TrickleCase, 5> cases = {{
        {"small byte-wise delivery completes", 1, 100, 2, true},
        {"large chunked delivery completes", 8, 200, 32, true},
        {"slow byte-wise delivery expires", 1, 500, 2, false},
        {"large sparse delivery expires", 3, 750, 32, false},
        // The deadline check is inclusive: the final chunk arriving at 2000 ms
        // is abandoned rather than delivered.
        {"delivery exactly at the deadline expires", 3, 1000, 2, false},
    }};

    for (const TrickleCase& test : cases) {
        CAPTURE(test.label);
        CAPTURE(test.bytesPerReveal);
        CAPTURE(test.msPerReveal);
        CAPTURE(test.payloadSize);
        TestClock::instance().reset();
        TestClock::instance().setAutoStep(1);

        MockClient client;
        PubSubClient psc(client);
        CallbackContractAdapter adapter;
        mqtt5_malformed::connect(client, psc, adapter);

        // Each inter-byte wait is permitted to last five seconds, while the
        // independent packet deadline bounds assembly to two seconds.
        psc.setSocketTimeout(5).setPacketTimeout(kPacketDeadlineMs / 1000);
        const MqttPacket packet = MqttPacket::publish5(
            "t", std::vector<uint8_t>(test.payloadSize, 'p'));
        const size_t chunkCount =
            (packet.size() + test.bytesPerReveal - 1) / test.bytesPerReveal;
        const unsigned long completionMs =
            static_cast<unsigned long>(chunkCount - 1) * test.msPerReveal;
        CHECK(test.completes == (completionMs < kPacketDeadlineMs));

        client.pushPacket(packet);
        client.setTrickle(test.bytesPerReveal, test.msPerReveal);
        const unsigned long start = TestClock::instance().millis();
        const bool loopResult = psc.loop();
        const unsigned long elapsed = TestClock::instance().millis() - start;

        if (test.completes) {
            CHECK(loopResult);
            CHECK(adapter.count() == 1);
            CHECK(psc.lastError() == MqttError::None);
            CHECK(psc.lastReasonCode() == 0);
            CHECK(psc.connected());
            CHECK_FALSE(client.stopCalled());
            CHECK(elapsed < kPacketDeadlineMs);
        } else {
            CHECK_FALSE(loopResult);
            CHECK(adapter.count() == 0);
            CHECK(psc.lastError() == MqttError::ReceiveTimeout);
            CHECK(psc.lastReasonCode() == 0);
            CHECK_FALSE(psc.connected());
            CHECK(client.flushCalled());
            CHECK(client.stopCalled());
            CHECK(client.outbound().empty());
            CHECK(elapsed <= kPacketDeadlineMs);
        }
    }
}

#endif  // MQTT_VERSION == MQTT_VERSION_5_0

}  // TEST_SUITE("mqtt5")
