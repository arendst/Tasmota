/*
  mqtt5_retransmit_test.cpp - in-flight retention of the Retained_Packet_Body, DUP
  retransmission encoding, and the PUBREL resend (tasks 11.5, 11.5a).

  Feature: tasmota-pubsub-mqtt5

  TEST_SUITE("mqtt5"), body guarded by `#if MQTT_VERSION == MQTT_VERSION_5_0`, so the file
  compiles in both selections and contributes cases to the ordinary
  build/pubsub_tests_v5 binary. Retention is UNCONDITIONAL in the MQTT 5.0 selection
  (Requirement 8.19): there is no on/off macro, no build in which it is compiled out, and
  therefore no second variant to enable it in (Requirement 15.8).
  MQTT_INFLIGHT_COPY_BUDGET survives as a BYTE CEILING only, defaulting to 2048, and every
  budget state below is reached by varying BODY SIZES against that compiled-in default -
  never by varying the macro.

  What is pinned (Requirements 8.7, 8.19 - 8.24 plus the design's "in-flight payload
  retention" decision):

  - A retransmission sets fixed-header bit 3 (DUP) and reuses the Packet Identifier of the
    original transmission. Because the retained unit is the ASSEMBLED PACKET BODY rather
    than the payload alone, the resend differs from the first transmission in that one bit
    and in nothing else (Requirement 8.7).
  - Retransmission fires ONLY on reconnect with Session Present 1 (Requirement 8.22).
    [MQTT-4.4.0-1] permits no other moment, so an acknowledgement deadline expiring on a
    live connection ABANDONS the message rather than resending it.
  - An entry awaiting PUBCOMP - PUBREC received, PUBREL sent, no PUBCOMP - resends its
    PUBREL on a Session Present 1 reconnect, from its Packet Identifier alone, at no
    budget cost (Requirement 8.23). Such an entry therefore SURVIVES a connection drop
    too, which is the one expectation task 11.5's opt-in shape had the other way round.
  - Session Present 0 means the broker kept no session, so every carried-over entry is
    abandoned and reported instead of resent. This is the path Tasmota's stock driver
    always takes: it passes Clean Start 1 unless `flag5.mqtt_persistent` is set.
  - A buffered QoS 1/2 message whose assembled body cannot fit the remaining budget is
    refused before its first write with `MqttError::RetentionUnavailable`.
  - The streaming paths remain QoS 0 only; a future QoS-capable form requires persistent
    replay storage before its first write.

  These are EXAMPLE cases. The exhaustive forms - Property 16, comparing the two recorded
  byte sequences for equality outside bit 3 of the fixed header across the input domain
  (task 11.9), and Property 31, the retention bounds and allocation accounting
  (task 11.12) - are not duplicated here.

  Requirements: 8.7, 8.19, 8.20, 8.21, 8.22, 8.23, 8.24
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

namespace mqtt5_retransmit {

// A scripted successful connect. `sessionPresent` is the CONNACK ack-flags bit 0, which is
// the whole decision this task turns on.
void connect(MockClient& client, PubSubClient& psc, bool sessionPresent = false) {
    client.pushPacket(MqttPacket::connack(0, sessionPresent));
    psc.setServer("broker.example", 1883);
    REQUIRE(psc.connect("mqtt5-retx", nullptr, nullptr, nullptr, 0, false, nullptr,
                        /*cleanSession=*/false));
    REQUIRE(psc.connected());
    client.clearOutbound();
}

// Drop the connection the way a network failure would, leaving the client free to
// reconnect. `disconnect()` runs the same teardown - resetConnectionState() - that every
// other close path runs.
void drop(MockClient& client, PubSubClient& psc) {
    psc.disconnect();
    REQUIRE_FALSE(psc.connected());
    client.clearOutbound();
    client.clearInbound();
}

// Reconnect and return the bytes the library wrote AFTER the CONNECT/CONNACK exchange -
// i.e. any retransmission and nothing else. The CONNECT itself is discarded by taking the
// outbound record from the byte count reached once the CONNACK has been consumed.
std::vector<uint8_t> reconnect(MockClient& client, PubSubClient& psc, bool sessionPresent) {
    client.pushPacket(MqttPacket::connack(0, sessionPresent));
    client.clearOutbound();
    REQUIRE(psc.connect("mqtt5-retx", nullptr, nullptr, nullptr, 0, false, nullptr,
                        /*cleanSession=*/false));
    REQUIRE(psc.connected());
    CHECK(psc.sessionPresent() == sessionPresent);
    const std::vector<uint8_t>& out = client.outbound();
    // The CONNECT is the first packet in the record; everything after it is what this task
    // emitted. Skip it by its own Remaining Length.
    REQUIRE(out.size() >= 2);
    REQUIRE((out[0] & 0xF0) == MQTTCONNECT);
    size_t pos = 1;
    uint32_t remaining = 0;
    uint32_t multiplier = 1;
    uint8_t encoded = 0;
    do {
        REQUIRE(pos < out.size());
        encoded = out[pos++];
        remaining += (uint32_t)(encoded & 0x7F) * multiplier;
        multiplier *= 128;
    } while ((encoded & 0x80) != 0);
    pos += remaining;
    REQUIRE(pos <= out.size());
    return std::vector<uint8_t>(out.begin() + (long)pos, out.end());
}

// Split a byte stream into whole MQTT packets, so a run of retransmissions can be counted
// and decoded one by one.
std::vector<std::vector<uint8_t>> splitPackets(const std::vector<uint8_t>& bytes) {
    std::vector<std::vector<uint8_t>> packets;
    size_t pos = 0;
    while (pos < bytes.size()) {
        const size_t start = pos;
        pos++;                                   // fixed header byte
        uint32_t remaining = 0;
        uint32_t multiplier = 1;
        uint8_t encoded = 0;
        do {
            REQUIRE(pos < bytes.size());
            encoded = bytes[pos++];
            remaining += (uint32_t)(encoded & 0x7F) * multiplier;
            multiplier *= 128;
        } while ((encoded & 0x80) != 0);
        pos += remaining;
        REQUIRE(pos <= bytes.size());
        packets.push_back(std::vector<uint8_t>(bytes.begin() + (long)start,
                                               bytes.begin() + (long)pos));
    }
    return packets;
}

}  // namespace mqtt5_retransmit

TEST_CASE("a retransmission sets DUP, reuses the identifier, and changes nothing else") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_retransmit::connect(client, psc);

    uint8_t requestedQos = 0;
    SUBCASE("QoS 1") { requestedQos = 1; }
    SUBCASE("QoS 2") { requestedQos = 2; }

    // First transmission. Retained in full, because this is the buffered publish path.
    const char* topic = "stat/dev/RESULT";
    const uint8_t payload[] = {'{', '"', 'P', '"', ':', '1', '}'};
    REQUIRE(psc.publish(topic, payload, sizeof(payload), true, requestedQos));
    const std::vector<uint8_t> first = client.outbound();
    const DecodedPublish5 firstDecoded = MqttParser::decodePublish5(first);
    REQUIRE(firstDecoded.valid);
    CHECK(firstDecoded.qos == requestedQos);
    CHECK(firstDecoded.retain);
    CHECK_FALSE(firstDecoded.dup);              // a first transmission is never a duplicate
    REQUIRE(firstDecoded.msgId != 0);
    CHECK(psc.inFlightCount() == 1);

    // The connection drops with the PUBLISH unacknowledged. The entry SURVIVES, because it
    // holds a retained copy: the only permitted retransmission comes after the next CONNACK.
    mqtt5_retransmit::drop(client, psc);
    CHECK(psc.inFlightCount() == 1);

    // Reconnect to a broker that still holds the session.
    const std::vector<uint8_t> resent = mqtt5_retransmit::reconnect(client, psc, true);

    // Requirement 8.7, both halves. Byte 0 differs by exactly bit 3; every other byte,
    // including the two Packet Identifier bytes, is the original's.
    REQUIRE(resent.size() == first.size());
    CHECK(resent[0] == (uint8_t)(first[0] | 0x08));
    CHECK((resent[0] & ~0x08) == (first[0] & ~0x08));
    for (size_t i = 1; i < first.size(); i++) {
        CHECK(resent[i] == first[i]);
    }

    const DecodedPublish5 resentDecoded = MqttParser::decodePublish5(resent);
    REQUIRE(resentDecoded.valid);
    CHECK(resentDecoded.dup);
    CHECK(resentDecoded.msgId == firstDecoded.msgId);
    CHECK(resentDecoded.qos == requestedQos);
    CHECK(resentDecoded.retain);
    CHECK(resentDecoded.topic == firstDecoded.topic);
    CHECK(resentDecoded.payload == firstDecoded.payload);

    // Still unacknowledged, so still in flight and still holding its identifier.
    CHECK(psc.inFlightCount() == 1);
}

TEST_CASE("matching persistent sessions retain identity and pending operations until CONNACK") {
    SUBCASE("an idle matching session accepts Session Present without replay") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        psc.setKeepAlive(0);
        mqtt5_retransmit::connect(client, psc);

        mqtt5_retransmit::drop(client, psc);
        CHECK(psc.inFlightCount() == 0);
        CHECK(mqtt5_retransmit::reconnect(client, psc, true).empty());
        CHECK(psc.sessionPresent());
    }

    for (bool unsubscribe : {false, true}) {
        CAPTURE(unsubscribe);

        SUBCASE("Session Present 1 preserves the identifier through its matching ACK") {
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            psc.setKeepAlive(0);
            mqtt5_retransmit::connect(client, psc);

            REQUIRE((unsubscribe ? psc.unsubscribe("cmnd/dev/#")
                                 : psc.subscribe("cmnd/dev/#")));
            const DecodedPacket sent = MqttParser::decode(client.outbound());
            REQUIRE(sent.valid);
            REQUIRE(sent.payload.size() >= 2);
            const uint16_t id = static_cast<uint16_t>(
                (static_cast<uint16_t>(sent.payload[0]) << 8) | sent.payload[1]);
            REQUIRE(id != 0);
            CHECK(psc.inFlightCount() == 1);

            mqtt5_retransmit::drop(client, psc);
            CHECK(psc.inFlightCount() == 1);
            CHECK(mqtt5_retransmit::reconnect(client, psc, true).empty());
            CHECK(psc.inFlightCount() == 1);

            client.clearOutbound();
            client.pushPacket(unsubscribe ? MqttPacket::unsuback5(id, {0x00})
                                          : MqttPacket::suback5(id, {0x00}));
            REQUIRE(psc.loop());
            CHECK(psc.inFlightCount() == 0);
            const MqttSubscriptionResult result = psc.lastSubscriptionResult();
            CHECK(result.packetId == id);
            CHECK(result.granted);
        }

        SUBCASE("Session Present 0 definitively releases the pending identifier") {
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            psc.setKeepAlive(0);
            mqtt5_retransmit::connect(client, psc);

            REQUIRE((unsubscribe ? psc.unsubscribe("cmnd/dev/#")
                                 : psc.subscribe("cmnd/dev/#")));
            const DecodedPacket sent = MqttParser::decode(client.outbound());
            REQUIRE(sent.valid);
            REQUIRE(sent.payload.size() >= 2);
            const uint16_t id = static_cast<uint16_t>(
                (static_cast<uint16_t>(sent.payload[0]) << 8) | sent.payload[1]);
            REQUIRE(id != 0);

            mqtt5_retransmit::drop(client, psc);
            CHECK(psc.inFlightCount() == 1);
            CHECK(mqtt5_retransmit::reconnect(client, psc, false).empty());
            CHECK(psc.inFlightCount() == 0);
            CHECK(psc.lastPacketId() == id);
            CHECK(psc.lastReasonCode() == PubSubClient::MQTT_RC_UNSPECIFIED_ERROR);
        }
    }
}

TEST_CASE("changed persistent session identity clears replay state before CONNECT") {
    struct Case {
        const char* label;
        const char* host;
        uint16_t port;
        const char* clientId;
        uint32_t expiry;
        bool cleanStart;
    };
    const Case cases[] = {
        {"host",              "other-broker.example", 1883, "mqtt5-retx", 0xFFFFFFFFu, false},
        {"port",              "broker.example",       8883, "mqtt5-retx", 0xFFFFFFFFu, false},
        {"client identifier", "broker.example",       1883, "another-client", 0xFFFFFFFFu, false},
        {"expiry policy",     "broker.example",       1883, "mqtt5-retx", 3600, false},
        {"Clean Start",       "broker.example",       1883, "mqtt5-retx", 0xFFFFFFFFu, true},
    };

    for (const Case& c : cases) {
        CAPTURE(c.label);
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        mqtt5_retransmit::connect(client, psc);
        const uint8_t payload[] = {'s'};
        REQUIRE(psc.publish("tele/dev/STATE", payload, sizeof(payload), false, 1));
        REQUIRE(psc.inFlightCount() == 1);
        mqtt5_retransmit::drop(client, psc);
        REQUIRE(psc.inFlightCount() == 1);

        psc.setServer(c.host, c.port);
        psc.setSessionExpiryInterval(c.expiry);
        client.pushPacket(MqttPacket::connack5(0x00, false));
        client.clearOutbound();
        REQUIRE(psc.connect(c.clientId, nullptr, nullptr, nullptr, 0, false, nullptr,
                            c.cleanStart));
        CHECK_FALSE(psc.sessionPresent());
        CHECK(psc.inFlightCount() == 0);

// A changed identity may establish a new session, but the old retained PUBLISH must
// never be appended after its CONNECT. The single outbound frame is CONNECT itself.
        const DecodedPacket only = MqttParser::decode(client.outbound());
        REQUIRE(only.valid);
        CHECK(only.type == static_cast<uint8_t>(MQTTCONNECT));
    }
}

TEST_CASE("an entry awaiting PUBCOMP resends its PUBREL on a Session Present 1 reconnect") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    // Keepalive off, so every byte recorded below belongs to the handshake or the resend.
    psc.setKeepAlive(0);
    mqtt5_retransmit::connect(client, psc);

    // Buffered QoS 2 admission always retains its PUBLISH body before the first write.
    const size_t payloadLen = 2;

    const char* topic = "tele/dev/STATE";
    const std::vector<uint8_t> payload(payloadLen, 'y');
    REQUIRE(psc.publish(topic, payload.data(), (unsigned int)payload.size(), false, 2));
    const DecodedPublish5 sent = MqttParser::decodePublish5(client.outbound());
    REQUIRE(sent.valid);
    REQUIRE(sent.qos == 2);
    const uint16_t id = sent.msgId;
    REQUIRE(id != 0);

    // PUBREC in, exactly one PUBREL out: the entry now awaits its PUBCOMP, which is the state
    // whose resend is a fresh PUBREL rather than a PUBLISH replay.
    client.clearOutbound();
    client.pushPacket(MqttPacket::ack5(MQTTPUBREC, id, -1));
    REQUIRE(psc.loop());
    const std::vector<uint8_t> firstPubrel = client.outbound();
    REQUIRE(firstPubrel.size() == 4);
    REQUIRE(firstPubrel[0] == (uint8_t)(MQTTPUBREL | 0x02));
    CHECK(psc.inFlightCount() == 1);

    // The connection drops with the PUBCOMP still missing. The entry SURVIVES - this is the
    // expectation task 11.5's opt-in shape had the other way round, because its predicate
    // excluded AwaitingPubcomp and let a teardown abandon such an entry.
    mqtt5_retransmit::drop(client, psc);
    CHECK(psc.inFlightCount() == 1);

    // Session Present 1: exactly one PUBREL, carrying the original Packet Identifier with the
    // mandatory fixed-header flags 0x02, and NO PUBLISH replay - the broker took
    // responsibility for the PUBLISH when it sent the PUBREC, so there is nothing to replay
    // and nothing was retained for it either.
    const std::vector<uint8_t> resent = mqtt5_retransmit::reconnect(client, psc, true);
    CHECK(resent == firstPubrel);
    const std::vector<std::vector<uint8_t>> packets = mqtt5_retransmit::splitPackets(resent);
    REQUIRE(packets.size() == 1);
    CHECK((packets[0][0] & 0xF0) == MQTTPUBREL);
    const DecodedAck5 rel = MqttParser::decodeAck5(packets[0], MQTTPUBREL);
    REQUIRE(rel.valid);
    CHECK(rel.packetId == id);
    CHECK(rel.form == 0);                        // short form, success implied

    // Still awaiting the PUBCOMP, which still completes it on the new connection.
    CHECK(psc.inFlightCount() == 1);
    client.clearOutbound();
    client.pushPacket(MqttPacket::ack5(MQTTPUBCOMP, id, -1));
    REQUIRE(psc.loop());
    CHECK(psc.inFlightCount() == 0);
    CHECK(psc.lastPacketId() == id);
    CHECK(client.outbound().empty());
}

TEST_CASE("Session Present 0 abandons the carried-over entry instead of resending it") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    psc.setKeepAlive(0);
    mqtt5_retransmit::connect(client, psc);

    // Both kinds of entry that survive a teardown are abandoned here: a retained QoS 1
    // PUBLISH, and a QoS 2 entry awaiting its PUBCOMP. Session Present 0 means the broker
    // kept no session, so neither can ever be acknowledged.
    uint8_t qos = 1;
    bool driveToPubcomp = false;
    SUBCASE("a retained QoS 1 PUBLISH") {
        qos = 1;
        driveToPubcomp = false;
    }
    SUBCASE("a QoS 2 entry awaiting PUBCOMP") {
        qos = 2;
        driveToPubcomp = true;
    }

    REQUIRE(psc.publish("stat/dev/RESULT", (const uint8_t*)"ON", 2, false, qos));
    const DecodedPublish5 sent = MqttParser::decodePublish5(client.outbound());
    REQUIRE(sent.valid);
    const uint16_t id = sent.msgId;
    REQUIRE(id != 0);

    if (driveToPubcomp) {
        client.clearOutbound();
        client.pushPacket(MqttPacket::ack5(MQTTPUBREC, id, -1));
        REQUIRE(psc.loop());
        REQUIRE(client.outbound().size() == 4);   // the one PUBREL
    }

    mqtt5_retransmit::drop(client, psc);
    CHECK(psc.inFlightCount() == 1);            // survived the drop, awaiting the CONNACK

    // Nothing is resent, the identifier is released, and the abandonment is reported through
    // the same two accessors every other abandonment uses.
    const std::vector<uint8_t> after = mqtt5_retransmit::reconnect(client, psc, false);
    CHECK(after.empty());
    CHECK(psc.inFlightCount() == 0);
    CHECK(psc.lastPacketId() == id);
    CHECK(psc.lastReasonCode() == PubSubClient::MQTT_RC_UNSPECIFIED_ERROR);
}

TEST_CASE("nothing is retransmitted while the connection is up") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    // Keepalive off, so loop() has no PINGREQ to emit and the outbound record shows only
    // what a retransmission would have written.
    psc.setKeepAlive(0);
    mqtt5_retransmit::connect(client, psc);

    // Both surviving kinds again, because the split this task introduces is between the
    // CONNACK and everything else: Session Present 1 resends, but the MQTT_ACK_TIMEOUT
    // deadline on a LIVE connection still abandons, unchanged for either kind.
    uint8_t qos = 1;
    bool driveToPubcomp = false;
    SUBCASE("a QoS 1 publish awaiting its PUBACK") {
        qos = 1;
        driveToPubcomp = false;
    }
    SUBCASE("a QoS 2 publish awaiting its PUBCOMP") {
        qos = 2;
        driveToPubcomp = true;
    }

    REQUIRE(psc.publish("stat/dev/RESULT", (const uint8_t*)"ON", 2, false, qos));
    const DecodedPublish5 sent = MqttParser::decodePublish5(client.outbound());
    REQUIRE(sent.valid);
    const uint16_t id = sent.msgId;
    client.clearOutbound();

    if (driveToPubcomp) {
        client.pushPacket(MqttPacket::ack5(MQTTPUBREC, id, -1));
        REQUIRE(psc.loop());
        REQUIRE(client.outbound().size() == 4);   // the one PUBREL
        client.clearOutbound();
    }

    // Past the acknowledgement deadline with no acknowledgement in sight. [MQTT-4.4.0-1]
    // still forbids a live-connection resend, but Task 18.6 keeps the exchange and Packet
    // Identifier held after reporting the local timeout so a late terminal ACK remains valid.
    TestClock::instance().advance((MQTT_ACK_TIMEOUT * 1000UL) + 1000UL);
    REQUIRE(psc.loop());
    CHECK(client.outbound().empty());
    CHECK(psc.inFlightCount() == 1);
    CHECK(psc.lastPacketId() == id);
    CHECK(psc.lastReasonCode() == PubSubClient::MQTT_RC_UNSPECIFIED_ERROR);
}

TEST_CASE("a message past the remaining copy budget is refused before transmission") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_retransmit::connect(client, psc);

    const char* topic = "stat/dev/RESULT";
    const size_t bodyOverhead = 2 + strlen(topic) + 2 + 1;
    const size_t budget = static_cast<size_t>(MQTT_INFLIGHT_COPY_BUDGET);
    const size_t bodyLen = ((budget / 4) + (budget / 3) + 1) / 2;
    REQUIRE(3 * bodyLen <= budget);
    REQUIRE(4 * bodyLen > budget);
    REQUIRE(bodyLen > bodyOverhead);
    const std::vector<uint8_t> payload(bodyLen - bodyOverhead, 'x');

    std::vector<uint16_t> ids;
    for (int i = 0; i < 3; ++i) {
        client.clearOutbound();
        REQUIRE(psc.publish(topic, payload.data(), static_cast<unsigned int>(payload.size()), false, 1));
        const DecodedPublish5 sent = MqttParser::decodePublish5(client.outbound());
        REQUIRE(sent.valid);
        ids.push_back(sent.msgId);
    }
    CHECK(psc.inFlightCount() == 3);

    client.clearOutbound();
    CHECK_FALSE(psc.publish(topic, payload.data(), static_cast<unsigned int>(payload.size()), false, 1));
    CHECK(psc.lastError() == MqttError::RetentionUnavailable);
    CHECK(client.outbound().empty());
    CHECK(psc.connected());
    CHECK(psc.inFlightCount() == 3);

    // A terminal acknowledgement frees both the retained body and its budget, so retrying
    // the same operation is accepted and remains replayable.
    client.pushPacket(MqttPacket::ack5(MQTTPUBACK, ids.front(), -1));
    REQUIRE(psc.loop());
    client.clearOutbound();
    REQUIRE(psc.publish(topic, payload.data(), static_cast<unsigned int>(payload.size()), false, 1));
    const DecodedPublish5 retried = MqttParser::decodePublish5(client.outbound());
    REQUIRE(retried.valid);
    CHECK(psc.inFlightCount() == 3);
}

TEST_CASE("the streaming publish path retains nothing") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_retransmit::connect(client, psc);

    // beginPublish() exposes no QoS argument, so it is QoS 0 by construction - the payload
    // arrives incrementally and is gone once written - which means it holds no
    // Outstanding_Message and retains nothing, even though retention is unconditional on the
    // buffered path (Requirements 8.19, 8.24).
    REQUIRE(psc.beginPublish("stat/dev/FILE", 4, false));
    CHECK(psc.write((const uint8_t*)"data", 4) == 4);
    CHECK(psc.endPublish() == 1);
    CHECK(psc.inFlightCount() == 0);

    mqtt5_retransmit::drop(client, psc);
    CHECK(psc.inFlightCount() == 0);
    CHECK(mqtt5_retransmit::reconnect(client, psc, false).empty());
}

#endif   // MQTT_VERSION == MQTT_VERSION_5_0

}  // TEST_SUITE("mqtt5")
