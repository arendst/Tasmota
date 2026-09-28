/*
  mqtt5_limits_test.cpp - negotiated MQTT 5.0 server-limit property coverage.

  Feature: tasmota-pubsub-mqtt5

  Property 19 is deliberately expressed through the public API and decoded wire bytes:
  every one of the 16 subsets of Server Keep Alive, Maximum Packet Size, Topic Alias
  Maximum and Receive Maximum appears in a CONNACK, and each present value is swept over
  its stated boundary set. The test checks both the latching/default rule and enforcement
  without reaching into the client's private state.
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

namespace mqtt5_limits {

constexpr uint8_t kServerKeepAlive = 0x13;
constexpr uint8_t kReceiveMaximum = 0x21;
constexpr uint8_t kTopicAliasMaximum = 0x22;
constexpr uint8_t kMaximumPacketSize = 0x27;
constexpr uint8_t kTopicAlias = 0x23;

constexpr uint8_t kHasKeepAlive = 0x01;
constexpr uint8_t kHasMaximumPacketSize = 0x02;
constexpr uint8_t kHasTopicAliasMaximum = 0x04;
constexpr uint8_t kHasReceiveMaximum = 0x08;
constexpr uint16_t kRequestedKeepAlive = 2;

bool hasProperty(const DecodedProperties& properties, uint8_t id) {
    for (const DecodedProperty& entry : properties.entries) {
        if (entry.id == id) {
            return true;
        }
    }
    return false;
}

#ifdef USE_MQTT_QOS
uint16_t publishAndReadId(MockClient& client, PubSubClient& psc, const char* topic) {
    client.clearOutbound();
    REQUIRE(psc.publish(topic, reinterpret_cast<const uint8_t*>("x"), 1, false, 1));
    const DecodedPublish5 publish = MqttParser::decodePublish5(client.outbound());
    REQUIRE(publish.valid);
    REQUIRE(publish.qos == 1u);
    REQUIRE(publish.msgId != 0u);
    return publish.msgId;
}
#endif  // USE_MQTT_QOS

}  // namespace mqtt5_limits

// Feature: tasmota-pubsub-mqtt5, Property 19: For all subsets of {Server Keep Alive,
// Maximum Packet Size, Topic Alias Maximum, Receive Maximum} present in a CONNACK, and
// for all values of the present properties, the effective keepalive is the server's value
// when present and the requested value otherwise (0 disabling keepalive entirely); no
// emitted packet's total wire size exceeds a present Maximum Packet Size (with no limit
// applied when absent); no emitted Topic Alias exceeds a present Topic Alias Maximum (no
// alias emitted at all when absent or 0); and the count of concurrently unacknowledged
// outbound QoS 1 and QoS 2 publishes never exceeds a present Receive Maximum (65535
// assumed when absent), a publish that would exceed it transmitting no bytes until an
// acknowledgement frees a slot.
//
// Property 19: Negotiated server limits are honored, with defaults for absent properties
TEST_CASE("Property 19: negotiated server limits honor every presence subset and boundary") {
    using namespace mqtt5_limits;

    const uint16_t keepAliveValues[] = {0, 1, 65535};
    const uint32_t maximumPacketSizes[] = {1, 16, 268435455u};
    const uint16_t topicAliasMaximums[] = {0, 1, 65535};
    const uint16_t receiveMaximums[] = {1, 2, 65535};

    // Three rounds are enough for every present property to take every boundary value;
    // all 16 masks in every round ensures the default behavior is tested in every subset
    // in which a property is absent.
    for (uint8_t round = 0; round < 3; round++) {
        for (uint8_t mask = 0; mask < 16; mask++) {
            CAPTURE(round);
            CAPTURE(mask);

            const bool hasKeepAlive = (mask & kHasKeepAlive) != 0;
            const bool hasMaxPacket = (mask & kHasMaximumPacketSize) != 0;
            const bool hasAliasMax = (mask & kHasTopicAliasMaximum) != 0;
            const bool hasReceiveMax = (mask & kHasReceiveMaximum) != 0;
            const uint16_t keepAlive = keepAliveValues[round];
            const uint32_t maximumPacketSize = maximumPacketSizes[round];
            const uint16_t topicAliasMaximum = topicAliasMaximums[round];
            const uint16_t receiveMaximum = receiveMaximums[round];

            MqttProps properties;
            if (hasKeepAlive) {
                properties.u16(kServerKeepAlive, keepAlive);
            }
            if (hasMaxPacket) {
                properties.u32(kMaximumPacketSize, maximumPacketSize);
            }
            if (hasAliasMax) {
                properties.u16(kTopicAliasMaximum, topicAliasMaximum);
            }
            if (hasReceiveMax) {
                properties.u16(kReceiveMaximum, receiveMaximum);
            }

            TestClock::instance().reset();
            TestClock::instance().setAutoStep(1);
            MockClient client;
            client.pushPacket(MqttPacket::connack5(0x00, false, properties));
            PubSubClient psc(client);
            psc.setServer("broker.example", 1883);
            psc.setKeepAlive(kRequestedKeepAlive);
            REQUIRE(psc.connect("mqtt5-limits"));
            REQUIRE(psc.connected());

            CHECK(psc.getKeepAlive() == (hasKeepAlive ? keepAlive : kRequestedKeepAlive));
            CHECK(psc.serverMaximumPacketSize() == (hasMaxPacket ? maximumPacketSize : 0u));
            CHECK(psc.serverTopicAliasMaximum() == (hasAliasMax ? topicAliasMaximum : 0u));
            CHECK(psc.getServerReceiveMaximum() == (hasReceiveMax ? receiveMaximum : 65535u));
            client.clearOutbound();

            // Drive negotiated keepalive with the virtual clock whenever the packet cap
            // permits a two-byte PINGREQ. A zero keepalive disables it completely.
            if (!hasMaxPacket || (maximumPacketSize >= 2u)) {
                if (hasKeepAlive && (keepAlive == 0)) {
                    TestClock::instance().advance(3600000);
                    CHECK(psc.loop());
                    CHECK(client.outbound().empty());
                } else if (hasKeepAlive && (keepAlive == 1)) {
                    TestClock::instance().advance(1001);
                    CHECK(psc.loop());
                    CHECK(client.outbound() == std::vector<uint8_t>{MQTTPINGREQ, 0x00});
                    client.clearOutbound();
                } else if (hasKeepAlive && (keepAlive == 65535)) {
                    TestClock::instance().advance(65535001UL);
                    CHECK(psc.loop());
                    CHECK(client.outbound() == std::vector<uint8_t>{MQTTPINGREQ, 0x00});
                    client.clearOutbound();
                }
            }

            // The smallest QoS 0 PUBLISH below is seven bytes on the wire. A broker cap
            // of one must refuse it atomically; the small and huge boundaries, and an
            // absent cap, permit it and its measured wire size must fit the negotiated cap.
            MqttPublishRequest qos0;
            qos0.topic = "t";
            qos0.payload = reinterpret_cast<const uint8_t*>("x");
            qos0.plength = 1;
            qos0.useTopicAlias = true;
            const bool packetFits = !hasMaxPacket || (maximumPacketSize >= 7u);
            const bool emitted = psc.publish(qos0);
            CHECK(emitted == packetFits);
            if (!packetFits) {
                CHECK(psc.lastError() == MqttError::PacketTooLarge);
                CHECK(client.outbound().empty());
                continue;
            }

            const std::vector<uint8_t>& qos0Wire = client.outbound();
            REQUIRE(MqttParser::isStructurallyWellFramed(qos0Wire));
            if (hasMaxPacket) {
                CHECK(qos0Wire.size() <= maximumPacketSize);
            }
            const DecodedPublish5 decodedQos0 = MqttParser::decodePublish5(qos0Wire);
            REQUIRE(decodedQos0.valid);
            // The host build enables a three-node alias table for Property 21. Aliases
            // remain absent when the broker omitted the limit or declared zero; otherwise
            // the first requested topic establishes an alias within both negotiated caps.
            const uint16_t aliasLimit = hasAliasMax
                                            ? ((topicAliasMaximum < MQTT_MAX_TOPIC_ALIASES)
                                                   ? topicAliasMaximum
                                                   : MQTT_MAX_TOPIC_ALIASES)
                                            : 0;
            if (aliasLimit == 0) {
                CHECK_FALSE(hasProperty(decodedQos0.props, kTopicAlias));
            } else {
                CHECK(decodedQos0.topicAlias >= 1u);
                CHECK(decodedQos0.topicAlias <= aliasLimit);
            }
            client.clearOutbound();

            // QoS 1 packets are nine bytes for this topic/payload. Fill exactly the
            // effective credit. This whole Receive Maximum enforcement section is outbound
            // QoS 1/2, so it is exercised only when USE_MQTT_QOS is compiled in.
#ifdef USE_MQTT_QOS
            // Admission is nonblocking: at capacity a further publish is
            // refused immediately (it never pumps loop()), so the caller runs loop() itself
            // to consume a preloaded PUBACK before retrying. In-flight count therefore never
            // exceeds min(server Receive Maximum, MQTT_MAX_INFLIGHT).
            const bool qos1Fits = !hasMaxPacket || (maximumPacketSize >= 9u);
            if (!qos1Fits) {
                continue;
            }
            const uint16_t effectiveCredit = hasReceiveMax
                                                 ? ((receiveMaximum < MQTT_MAX_INFLIGHT)
                                                        ? receiveMaximum
                                                        : MQTT_MAX_INFLIGHT)
                                                 : MQTT_MAX_INFLIGHT;
            std::vector<uint16_t> packetIds;
            for (uint16_t i = 0; i < effectiveCredit; i++) {
                packetIds.push_back(publishAndReadId(client, psc, "q"));
                CHECK(psc.inFlightCount() <= effectiveCredit);
            }
            REQUIRE(psc.inFlightCount() == effectiveCredit);

            // At capacity the publish is refused with no byte emitted and the credit
            // unchanged, rather than blocking to drain an acknowledgement.
            client.clearOutbound();
            REQUIRE_FALSE(psc.publish("q", reinterpret_cast<const uint8_t*>("x"), 1, false, 1));
            REQUIRE(psc.lastError() == MqttError::ReceiveMaximumReached);
            REQUIRE(psc.inFlightCount() == effectiveCredit);
            REQUIRE(client.outbound().empty());

            // Feed one PUBACK and let the caller's own loop() consume it, freeing a credit.
            // The retry then goes out on the wire.
            client.pushPacket(MqttPacket::ack5(MQTTPUBACK, packetIds.front(), -1));
            REQUIRE(psc.loop());
            REQUIRE(psc.inFlightCount() == effectiveCredit - 1u);
            client.clearOutbound();
            REQUIRE(psc.publish("q", reinterpret_cast<const uint8_t*>("x"), 1, false, 1));
            REQUIRE(MqttParser::isStructurallyWellFramed(client.outbound()));
            if (hasMaxPacket) {
                CHECK(client.outbound().size() <= maximumPacketSize);
            }
            CHECK(psc.inFlightCount() == effectiveCredit);
            CHECK(psc.connected());
#endif  // USE_MQTT_QOS
        }
    }
}

namespace mqtt5_limits {

enum class EmittingPath : uint8_t {
    BufferedPublish,
    FlashPublish,
    StreamingPublish,
    Subscribe,
    Unsubscribe,
};

constexpr EmittingPath kEmittingPaths[] = {
    EmittingPath::BufferedPublish,
    EmittingPath::FlashPublish,
    EmittingPath::StreamingPublish,
    EmittingPath::Subscribe,
    EmittingPath::Unsubscribe,
};

const char* pathName(EmittingPath path) {
    switch (path) {
    case EmittingPath::BufferedPublish:  return "publish";
    case EmittingPath::FlashPublish:     return "publish_P";
    case EmittingPath::StreamingPublish: return "beginPublish/write/endPublish";
    case EmittingPath::Subscribe:        return "subscribe";
    case EmittingPath::Unsubscribe:      return "unsubscribe";
    }
    return "unknown";
}

uint32_t remainingLength(EmittingPath path, size_t topicLength, size_t payloadLength) {
    switch (path) {
    case EmittingPath::BufferedPublish:
    case EmittingPath::FlashPublish:
    case EmittingPath::StreamingPublish:
        return static_cast<uint32_t>(2u + topicLength + 1u + payloadLength);
    case EmittingPath::Subscribe:
        return static_cast<uint32_t>(2u + 1u + 2u + topicLength + 1u);
    case EmittingPath::Unsubscribe:
        return static_cast<uint32_t>(2u + 1u + 2u + topicLength);
    }
    return 0;
}

uint32_t totalWireSize(EmittingPath path, size_t topicLength, size_t payloadLength) {
    const uint32_t bodyLength = remainingLength(path, topicLength, payloadLength);
    return 1u + PubSubClient::variableByteIntegerSize(bodyLength) + bodyLength;
}

uint16_t requiredBufferSize(EmittingPath path, size_t topicLength, size_t payloadLength) {
    switch (path) {
    case EmittingPath::BufferedPublish:
        return static_cast<uint16_t>(MQTT_MAX_HEADER_SIZE + 2u + topicLength + 1u +
                                     payloadLength);
    case EmittingPath::FlashPublish:
    case EmittingPath::StreamingPublish:
        return static_cast<uint16_t>(MQTT_MAX_HEADER_SIZE + 2u + topicLength + 1u);
    case EmittingPath::Subscribe:
        return static_cast<uint16_t>(MQTT_MAX_HEADER_SIZE + 2u + 1u + 2u + topicLength +
                                     1u);
    case EmittingPath::Unsubscribe:
        return static_cast<uint16_t>(MQTT_MAX_HEADER_SIZE + 2u + 1u + 2u + topicLength);
    }
    return 0;
}

void connectWithMaximumPacketSize(MockClient& client, PubSubClient& psc, uint32_t maximum) {
    MqttProps props;
    props.u32(kMaximumPacketSize, maximum);
    client.pushPacket(MqttPacket::connack5(0x00, false, props));
    psc.setServer("broker.example", 1883);
    REQUIRE(psc.connect("mqtt5-atomic-limits"));
    REQUIRE(psc.connected());
    client.clearOutbound();
}

bool emit(EmittingPath path, PubSubClient& psc, const char* topic, const std::string& payload) {
    switch (path) {
    case EmittingPath::BufferedPublish:
        return psc.publish(topic, reinterpret_cast<const uint8_t*>(payload.data()),
                           static_cast<unsigned int>(payload.size()), false);
    case EmittingPath::FlashPublish:
        return psc.publish_P(topic, reinterpret_cast<const uint8_t*>(payload.data()),
                             static_cast<unsigned int>(payload.size()), false);
    case EmittingPath::StreamingPublish:
        if (!psc.beginPublish(topic, static_cast<unsigned int>(payload.size()), false)) {
            return false;
        }
        if (psc.write(reinterpret_cast<const uint8_t*>(payload.data()), payload.size()) !=
            payload.size()) {
            return false;
        }
        return psc.endPublish() == 1;
    case EmittingPath::Subscribe:
        return psc.subscribe(topic);
    case EmittingPath::Unsubscribe:
        return psc.unsubscribe(topic);
    }
    return false;
}

void checkAtomicRefusal(const MockClient& client, PubSubClient& psc,
                        bool sent, MqttError error, const std::vector<uint8_t>& before) {
    CHECK_FALSE(sent);
    CHECK(psc.lastError() == error);
    CHECK(client.outbound() == before);
    CHECK(psc.connected());
}

}  // namespace mqtt5_limits

// Feature: tasmota-pubsub-mqtt5, Property 20: For all outbound operations whose assembled
// packet would exceed either the configured buffer size or the broker's declared Maximum Packet
// Size, the operation returns a failure status carrying the size-violation indication, adds no
// byte to the outbound stream, and leaves the connection state unchanged.
//
// Property 20: Size-limit refusal is atomic
// Property 28: A length that does not fit is refused, never truncated
TEST_CASE("Property 20/28: size-limit and VBI refusal are atomic across emit paths") {
    using namespace mqtt5_limits;

    const std::string payload(16, 'p');
    const std::string topic = "limit/topic";

    // Each public caller-visible packet emitter crosses its own exact single-buffer
    // boundary. The three values below it must be all-or-nothing refusals; the exact
    // boundary and one value above it must frame a complete packet.
    for (EmittingPath path : kEmittingPaths) {
        const uint16_t minimumBuffer = requiredBufferSize(path, topic.size(), payload.size());
        CAPTURE(pathName(path));
        CAPTURE(minimumBuffer);
        for (int delta = -3; delta <= 1; ++delta) {
            CAPTURE(delta);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            connectWithMaximumPacketSize(client, psc, 65535u);
            REQUIRE(psc.setBufferSize(static_cast<uint16_t>(minimumBuffer + delta)));

            const std::vector<uint8_t> before = client.outbound();
            const bool sent = emit(path, psc, topic.c_str(), payload);
            if (delta < 0) {
                checkAtomicRefusal(client, psc, sent, MqttError::BufferTooSmall, before);
            } else {
                REQUIRE(sent);
                CHECK(MqttParser::isStructurallyWellFramed(client.outbound()));
                CHECK(psc.connected());
            }
        }
    }

    // Repeat the same five-point window at the broker-declared total-wire-size
    // boundary. This verifies that the cap includes fixed header, Remaining Length,
    // variable header, Properties, and payload rather than only the packet body.
    for (EmittingPath path : kEmittingPaths) {
        const uint32_t wireSize = totalWireSize(path, topic.size(), payload.size());
        CAPTURE(pathName(path));
        CAPTURE(wireSize);
        for (int delta = -3; delta <= 1; ++delta) {
            CAPTURE(delta);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            connectWithMaximumPacketSize(client, psc,
                                         static_cast<uint32_t>(static_cast<int>(wireSize) + delta));

            const std::vector<uint8_t> before = client.outbound();
            const bool sent = emit(path, psc, topic.c_str(), payload);
            if (delta < 0) {
                checkAtomicRefusal(client, psc, sent, MqttError::PacketTooLarge, before);
            } else {
                REQUIRE(sent);
                CHECK(MqttParser::isStructurallyWellFramed(client.outbound()));
                CHECK(client.outbound().size() <= wireSize + static_cast<uint32_t>(delta));
                CHECK(psc.connected());
            }
        }
    }

    // Property 28 covers the only path that can reach the narrowed VBI boundary
    // in normal use. It still sweeps all three PUBLISH assemblers: buffered,
    // flash-streamed, and beginPublish/write/endPublish. A refusal is followed by
    // a valid publish to prove it left no truncated byte in the outbound stream.
    const std::string vbiTopic = "t";
    const uint32_t publishOverhead = static_cast<uint32_t>(2u + vbiTopic.size() + 1u);
    const uint32_t remainingLengths[] = {16382u, 16383u, 16384u, 16385u};
    const EmittingPath publishPaths[] = {
        EmittingPath::BufferedPublish,
        EmittingPath::FlashPublish,
        EmittingPath::StreamingPublish,
    };
    for (EmittingPath path : publishPaths) {
        for (uint32_t wantedLength : remainingLengths) {
            CAPTURE(pathName(path));
            CAPTURE(wantedLength);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            connectWithMaximumPacketSize(client, psc, 65535u);
            if (path == EmittingPath::BufferedPublish) {
                REQUIRE(psc.setBufferSize(20000));
            }
            const std::string vbiPayload(wantedLength - publishOverhead, 'v');
            const bool shouldFit = wantedLength <= PubSubClient::MQTT_VBI_MAX;
            const bool sent = emit(path, psc, vbiTopic.c_str(), vbiPayload);
            if (shouldFit) {
                REQUIRE(sent);
                uint32_t decodedLength = 0;
                size_t lengthBytes = 0;
                REQUIRE(MqttParser::decodeRemainingLength(client.outbound(), 1,
                                                          decodedLength, lengthBytes));
                CHECK(decodedLength == wantedLength);
                CHECK(lengthBytes == 2u);
                CHECK(MqttParser::isStructurallyWellFramed(client.outbound()));
            } else {
                checkAtomicRefusal(client, psc, sent, MqttError::PacketTooLarge, {});
                REQUIRE(psc.publish("after/refusal", "ok"));
                CHECK(MqttParser::isStructurallyWellFramed(client.outbound()));
            }
        }
    }
}

#endif  // MQTT_VERSION == MQTT_VERSION_5_0

}  // TEST_SUITE("mqtt5")
