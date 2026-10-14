/*
  mqtt5_qos_test.cpp - the shared Packet Identifier allocator, the Outstanding_Message
  table's lifetime, and the acknowledgement abandonment deadline (task 11.1).

  Feature: tasmota-pubsub-mqtt5

  TEST_SUITE("mqtt5"), body guarded by `#if MQTT_VERSION == MQTT_VERSION_5_0`, so the
  file compiles in both selections and contributes cases only to the 5.0 binary.

  ONE allocator serves PUBLISH, SUBSCRIBE and UNSUBSCRIBE, so an identifier can never be
  shared between a publish and a subscription (Requirement 8.4). `nextMsgId` is the rotor:
  each allocation advances it, wraps 65535 -> 1 so the illegal value 0 is never yielded,
  and skips any candidate the Outstanding table already holds. An entry's state - and its
  identifier - is retained until its acknowledgement sequence completes or the entry is
  abandoned, which happens on the MQTT_ACK_TIMEOUT deadline and on disconnect
  (Requirement 8.6); the released identifier is reported through lastPacketId() with a
  locally synthesized failure code through lastReasonCode().

  Everything asserted here is observable through the public API - inFlightCount(),
  lastPacketId(), lastReasonCode() and the emitted bytes.

  Task 11.3 adds the outbound QoS 1 correlation cases at the end: a PUBACK completes the
  matching Outstanding_Message and releases its identifier (Requirement 8.16), the numeric
  Reason Code is exposed through lastReasonCode() with the identifier through lastPacketId()
  (Requirements 7.3, 7.4), a code of 0x80 or above is terminal rather than a reason to
  retransmit, and a PUBACK naming no outstanding QoS 1 publish is discarded with every
  outstanding message left untouched (Requirement 7.8).

  Task 11.4 adds the outbound QoS 2 handshake cases after those: a PUBREC with a Reason Code
  below 0x80 produces exactly one PUBREL carrying the same identifier with the mandatory
  fixed-header flags 0x02 and leaves the entry holding its identifier until the PUBCOMP
  completes and releases it (Requirement 8.12); a PUBREC of 0x80 or above sends no PUBREL,
  releases the identifier and exposes that code with the released identifier as the failure
  result (Requirement 8.14).

  Those are EXAMPLE cases. Task 11.6 adds Property 12 at the end of the file: the curated
  interleaving table over all four packet types that allocate an identifier, the identifier
  vectors at both ends of the legal range and either side of the byte boundary, the full
  rotor wrap that shows a held identifier being skipped and a released one being reissued,
  and table saturation refusing a publish with nothing transmitted.

  The 256-reason-code × three-form sweep of the QoS 1 lifecycle is Property 13 (task 11.7)
  and the 256-reason-code sweep of the QoS 2 handshake is Property 14 (task 11.8); neither is
  duplicated here.

  Task 18.8 extends Property 16 and replaces Property 31: retention is atomic
  admission for buffered QoS 1/2 publishes, so a budget or allocation shortfall
  refuses before the first wire byte rather than creating a non-replayable entry.

  Requirements: 7.3, 7.4, 8.4, 8.5, 8.6, 8.12, 8.14, 8.16
*/

#include <cstdint>
#include <set>
#include <string>
#include <vector>

#include "doctest.h"

#include "MockClient.h"
#include "MqttPacket.h"
#include "TestClock.h"
#include "PubSubClient.h"

TEST_SUITE("mqtt5") {

#if MQTT_VERSION == MQTT_VERSION_5_0

namespace mqtt5_qos {

void connect(MockClient& client, PubSubClient& psc) {
    client.pushPacket(MqttPacket::connack(0));
    psc.setServer("broker.example", 1883);
    REQUIRE(psc.connect("mqtt5-qos"));
    REQUIRE(psc.connected());
    client.clearOutbound();
}

// The Packet Identifier of an emitted SUBSCRIBE / UNSUBSCRIBE: fixed header, one
// Remaining Length byte for these short packets, then the two identifier bytes.
uint16_t emittedPacketId(const std::vector<uint8_t>& out) {
    REQUIRE(out.size() >= 4);
    return static_cast<uint16_t>((static_cast<uint16_t>(out[2]) << 8) | out[3]);
}

// Subscribe once and return the identifier that went out.
uint16_t subscribeAndReadId(MockClient& client, PubSubClient& psc, const char* filter) {
    client.clearOutbound();
    REQUIRE(psc.subscribe(filter));
    return emittedPacketId(client.outbound());
}

// Publish once at QoS 1 and return the Packet Identifier the emitted PUBLISH carries,
// decoded by the harness rather than read at a hand-computed offset - the identifier's
// position depends on the topic length.
uint16_t publishQos1AndReadId(MockClient& client, PubSubClient& psc, const char* topic) {
    client.clearOutbound();
    REQUIRE(psc.publish(topic, (const uint8_t*)"ON", 2, false, 1));
    const DecodedPublish5 p = MqttParser::decodePublish5(client.outbound());
    REQUIRE(p.valid);
    REQUIRE(p.qos == 1);
    REQUIRE_FALSE(p.dup);          // first transmission, so no DUP flag
    REQUIRE(p.msgId != 0);
    return p.msgId;
}

// Publish once at QoS 2 and return the Packet Identifier the emitted PUBLISH carries.
uint16_t publishQos2AndReadId(MockClient& client, PubSubClient& psc, const char* topic) {
    client.clearOutbound();
    REQUIRE(psc.publish(topic, (const uint8_t*)"ON", 2, false, 2));
    const DecodedPublish5 p = MqttParser::decodePublish5(client.outbound());
    REQUIRE(p.valid);
    REQUIRE(p.qos == 2);
    REQUIRE_FALSE(p.dup);          // first transmission, so no DUP flag
    REQUIRE(p.msgId != 0);
    return p.msgId;
}

}  // namespace mqtt5_qos

TEST_CASE("the shared allocator issues unique nonzero identifiers while they are held") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_qos::connect(client, psc);

    std::set<uint16_t> ids;
    for (int i = 0; i < MQTT_MAX_INFLIGHT; i++) {
        const std::string filter = "cmnd/dev-" + std::to_string(i) + "/#";
        const uint16_t id = mqtt5_qos::subscribeAndReadId(client, psc, filter.c_str());
        CHECK(id != 0);
        CHECK(ids.insert(id).second);
    }
    CHECK(psc.inFlightCount() == MQTT_MAX_INFLIGHT);

    // The publish-sized portion of the envelope does not evict subscriptions. An
    // UNSUBSCRIBE receives its own distinct held identifier while capacity remains.
    client.clearOutbound();
    REQUIRE(psc.unsubscribe("cmnd/dev-0/#"));
    const uint16_t unsubId = mqtt5_qos::emittedPacketId(client.outbound());
    CHECK(unsubId != 0);
    CHECK(ids.insert(unsubId).second);
    CHECK(psc.inFlightCount() == MQTT_MAX_INFLIGHT + 1);
}

TEST_CASE("an acknowledged operation releases its identifier and its table slot") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_qos::connect(client, psc);

    const uint16_t id = mqtt5_qos::subscribeAndReadId(client, psc, "tele/dev/SENSOR");
    CHECK(psc.inFlightCount() == 1);

    // Requirement 8.6's first end state: the acknowledgement sequence completed.
    client.pushPacket(MqttPacket::suback5(id, {0x00}));
    REQUIRE(psc.loop());
    CHECK(psc.inFlightCount() == 0);
    CHECK(psc.lastSubscriptionResult().packetId == id);
    CHECK(psc.lastSubscriptionResult().granted);

    // The retained filter survives the release, which is the lifetime documented on
    // MqttSubscriptionResult - readable after loop() has returned.
    REQUIRE(psc.lastSubscriptionResult().topicFilter != nullptr);
    CHECK(std::string(psc.lastSubscriptionResult().topicFilter) == "tele/dev/SENSOR");
}

TEST_CASE("a timeout reports once while the live operation retains its Packet Identifier") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_qos::connect(client, psc);
    psc.setKeepAlive(0);

    const uint16_t id = mqtt5_qos::subscribeAndReadId(client, psc, "cmnd/dev/#");
    REQUIRE(psc.inFlightCount() == 1);

    TestClock::instance().advance((MQTT_ACK_TIMEOUT * 1000UL) + 1UL);
    REQUIRE(psc.loop());
    CHECK(psc.inFlightCount() == 1);
    CHECK(psc.lastPacketId() == id);
    CHECK(psc.lastReasonCode() == PubSubClient::MQTT_RC_UNSPECIFIED_ERROR);
    CHECK(psc.connected());
    CHECK_FALSE(client.stopCalled());

    // Repeated sweeps preserve the original operation and do not manufacture a new result.
    const uint16_t reportedId = psc.lastPacketId();
    TestClock::instance().advance((MQTT_ACK_TIMEOUT * 1000UL) + 1UL);
    REQUIRE(psc.loop());
    CHECK(psc.inFlightCount() == 1);
    CHECK(psc.lastPacketId() == reportedId);

    // A late valid acknowledgement still completes the exact original subscription.
    client.pushPacket(MqttPacket::suback5(id, {0x00}));
    REQUIRE(psc.loop());
    CHECK(psc.inFlightCount() == 0);
    CHECK(psc.lastSubscriptionResult().packetId == id);
    REQUIRE(psc.lastSubscriptionResult().topicFilter != nullptr);
    CHECK(std::string(psc.lastSubscriptionResult().topicFilter) == "cmnd/dev/#");
}

TEST_CASE("disconnect abandons every outstanding operation") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_qos::connect(client, psc);

    const uint16_t id = mqtt5_qos::subscribeAndReadId(client, psc, "cmnd/dev/#");
    REQUIRE(psc.inFlightCount() == 1);

    psc.disconnect(false);

    CHECK(psc.inFlightCount() == 0);
    CHECK(psc.lastPacketId() == id);
    CHECK(psc.lastReasonCode() == PubSubClient::MQTT_RC_UNSPECIFIED_ERROR);
}

TEST_CASE("a protocol failure reports its own reason code, not the abandonment code") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_qos::connect(client, psc);

    const uint16_t id = mqtt5_qos::subscribeAndReadId(client, psc, "cmnd/dev/#");
    REQUIRE(psc.inFlightCount() == 1);

    // A SUBACK with no Reason Code list at all: malformed, so the connection closes with
    // 0x81. The outstanding entry is abandoned as part of that teardown, but the code that
    // CAUSED the closure is the more informative diagnostic and must survive it.
    client.pushPacket(MqttPacket::suback5(id, {}));
    CHECK_FALSE(psc.loop());

    CHECK(psc.lastError() == MqttError::MalformedPacket);
    CHECK(psc.lastReasonCode() == PubSubClient::MQTT_RC_MALFORMED_PACKET);
    CHECK(psc.inFlightCount() == 0);
}

// ===========================================================================
//  Outbound QoS 1: PUBACK correlation (task 11.3)
// ===========================================================================

TEST_CASE("a PUBACK completes the outbound QoS 1 publish and releases its identifier") {
    // All three acknowledgement forms carry Reason Code 0x00 - implied by the short form,
    // explicit in the other two - so all three must land on the same state.
    struct Form {
        const char* name;
        int         reasonCode;   // < 0 selects ack5()'s short form
        bool        withProps;
    };
    const Form forms[] = {
        {"short form, Reason Code implied 0x00", -1,   false},
        {"mid form, explicit Reason Code 0x00",  0x00, false},
        {"long form, Reason Code 0x00 + props",  0x00, true},
    };

    for (const Form& form : forms) {
        SUBCASE(form.name) {
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            mqtt5_qos::connect(client, psc);

            const uint16_t id = mqtt5_qos::publishQos1AndReadId(client, psc, "tele/dev/STATE");
            REQUIRE(psc.inFlightCount() == 1);

            client.clearOutbound();
            client.pushPacket(form.withProps
                                  ? MqttPacket::ack5(MQTTPUBACK, id, form.reasonCode,
                                                     MqttProps().str(0x1F, "ok"))
                                  : MqttPacket::ack5(MQTTPUBACK, id, form.reasonCode));
            REQUIRE(psc.loop());

            // Requirement 8.16: complete, and the identifier back in the free set.
            CHECK(psc.inFlightCount() == 0);
            // Requirements 7.3, 7.4: which message, and with which code - an absent Reason
            // Code reading as 0x00.
            CHECK(psc.lastPacketId() == id);
            CHECK(psc.lastReasonCode() == 0x00);
            // A completed publish is not a transport failure and answers nothing.
            CHECK(psc.lastError() == MqttError::None);
            CHECK(psc.connected());
            CHECK(client.outbound().empty());
        }
    }
}

TEST_CASE("a released identifier is genuinely reusable and a late PUBACK finds nothing") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_qos::connect(client, psc);

    // Fill all but one table slot with QoS 1 publishes, then acknowledge one: this checks
    // identifier release and reuse without entering Receive Maximum back-pressure (which
    // has its dedicated saturation case below).
    uint16_t first = 0;
    for (int i = 0; i < MQTT_MAX_INFLIGHT - 1; i++) {
        const std::string topic = "tele/dev-" + std::to_string(i) + "/STATE";
        const uint16_t id = mqtt5_qos::publishQos1AndReadId(client, psc, topic.c_str());
        if (i == 0) {
            first = id;
        }
    }
    REQUIRE(psc.inFlightCount() == MQTT_MAX_INFLIGHT - 1);

    client.pushPacket(MqttPacket::ack5(MQTTPUBACK, first, -1));
    REQUIRE(psc.loop());
    CHECK(psc.inFlightCount() == MQTT_MAX_INFLIGHT - 2);

    // The freed slot is usable again, which is the observable half of the release.
    CHECK(psc.publish("tele/dev/again", (const uint8_t*)"ON", 2, false, 1));
    CHECK(psc.inFlightCount() == MQTT_MAX_INFLIGHT - 1);

    // A second PUBACK for the already-completed identifier now matches nothing, so it is
    // discarded with every remaining entry untouched (Requirement 7.8).
    client.pushPacket(MqttPacket::ack5(MQTTPUBACK, first, 0x00));
    REQUIRE(psc.loop());
    CHECK(psc.inFlightCount() == MQTT_MAX_INFLIGHT - 1);
    CHECK(psc.connected());
}

TEST_CASE("a PUBACK Reason Code of 0x80 or above is terminal, not a reason to retransmit") {
    // 0x80 is the bottom of the failure range; 0x97 (Quota exceeded) and 0x99
    // (Payload format invalid) are packet-specific PUBACK failure codes.
    const uint8_t failureCodes[] = {0x80, 0x97, 0x99};

    for (uint8_t rc : failureCodes) {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        mqtt5_qos::connect(client, psc);
        // Keepalive off, so the only bytes that could appear below would be a
        // retransmission.
        psc.setKeepAlive(0);

        const uint16_t id = mqtt5_qos::publishQos1AndReadId(client, psc, "tele/dev/STATE");
        REQUIRE(psc.inFlightCount() == 1);

        client.clearOutbound();
        client.pushPacket(MqttPacket::ack5(MQTTPUBACK, id, (int)rc));
        REQUIRE(psc.loop());

        // Requirement 7.4: the numeric failure code is what the caller reads, together with
        // the identifier it belongs to.
        CHECK(psc.lastPacketId() == id);
        CHECK(psc.lastReasonCode() == rc);
        // Acknowledged: the entry is gone and the identifier released exactly as on success,
        // so there is nothing left for a retransmission to pick up.
        CHECK(psc.inFlightCount() == 0);
        // A refused message is a normal broker answer: the connection stays up, no error
        // state is latched, and no byte goes out - in particular no retransmitted PUBLISH.
        CHECK(psc.lastError() == MqttError::None);
        CHECK(psc.connected());
        CHECK_FALSE(client.stopCalled());
        CHECK(client.outbound().empty());

        // Still nothing later: the deadline sweep has no entry left to abandon either.
        TestClock::instance().advance((MQTT_ACK_TIMEOUT + 1) * 1000UL);
        REQUIRE(psc.loop());
        CHECK(client.outbound().empty());
        CHECK(psc.inFlightCount() == 0);
    }
}

TEST_CASE("a PUBACK matching no outstanding QoS 1 publish leaves every entry unchanged") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_qos::connect(client, psc);
    psc.setKeepAlive(0);

    const uint16_t subId = mqtt5_qos::subscribeAndReadId(client, psc, "cmnd/dev/#");
    const uint16_t pubId = mqtt5_qos::publishQos1AndReadId(client, psc, "tele/dev/STATE");
    REQUIRE(subId != pubId);
    REQUIRE(psc.inFlightCount() == 2);

    SUBCASE("an identifier no entry holds") {
        // Pick one that is neither outstanding identifier.
        uint16_t stray = 1;
        while ((stray == subId) || (stray == pubId)) {
            stray++;
        }
        client.clearOutbound();
        client.pushPacket(MqttPacket::ack5(MQTTPUBACK, stray, 0x87));
        REQUIRE(psc.loop());

        CHECK(psc.inFlightCount() == 2);
        CHECK(psc.lastPacketId() == 0);        // nothing reported: nothing correlated
        CHECK(psc.lastReasonCode() == 0);
        CHECK(client.outbound().empty());
        CHECK(psc.connected());
    }

    SUBCASE("an identifier held by a SUBSCRIBE, not by a QoS 1 publish") {
        // The shared allocator makes identifiers unique across packet types, so this is the
        // broker acknowledging a QoS 1 PUBLISH that was never sent. The subscription must
        // still be waiting for its SUBACK afterwards.
        client.pushPacket(MqttPacket::ack5(MQTTPUBACK, subId, 0x00));
        REQUIRE(psc.loop());

        CHECK(psc.inFlightCount() == 2);
        CHECK(psc.lastPacketId() == 0);
        CHECK(psc.lastReasonCode() == 0);

        // Still correlatable: the SUBACK that does arrive still names the retained filter.
        client.pushPacket(MqttPacket::suback5(subId, {0x00}));
        REQUIRE(psc.loop());
        CHECK(psc.lastSubscriptionResult().packetId == subId);
        REQUIRE(psc.lastSubscriptionResult().topicFilter != nullptr);
        CHECK(std::string(psc.lastSubscriptionResult().topicFilter) == "cmnd/dev/#");
        CHECK(psc.inFlightCount() == 1);
    }
}

// ===========================================================================
//  Outbound QoS 2: the PUBREC / PUBREL / PUBCOMP handshake (task 11.4)
// ===========================================================================

TEST_CASE("a PUBREC below 0x80 answers with one PUBREL and the PUBCOMP completes it") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_qos::connect(client, psc);
    // Keepalive off, so every byte recorded below belongs to the handshake.
    psc.setKeepAlive(0);

    const uint16_t id = mqtt5_qos::publishQos2AndReadId(client, psc, "tele/dev/STATE");
    REQUIRE(psc.inFlightCount() == 1);

    client.clearOutbound();
    client.pushPacket(MqttPacket::ack5(MQTTPUBREC, id, -1));
    REQUIRE(psc.loop());

    // Requirement 8.12: exactly one PUBREL, carrying the SAME identifier. Requirement 7.7:
    // its fixed-header flags are 0x02, and the success code takes the short form, so the
    // whole answer is four bytes.
    const std::vector<uint8_t> pubrel = client.outbound();
    REQUIRE(pubrel.size() == 4);
    CHECK(pubrel[0] == (uint8_t)(MQTTPUBREL | 0x02));
    const DecodedAck5 rel = MqttParser::decodeAck5(pubrel, MQTTPUBREL);
    REQUIRE(rel.valid);
    CHECK(rel.packetId == id);
    CHECK(rel.form == 0);

    // The exchange has advanced but is NOT over: the entry still holds its identifier, and
    // nothing is latched as a result yet.
    CHECK(psc.inFlightCount() == 1);
    CHECK(psc.connected());
    CHECK(psc.lastError() == MqttError::None);

    // Requirement 8.12: the PUBCOMP ends it - complete, identifier released, code reported
    // with the identifier it belongs to, and no further byte out.
    client.clearOutbound();
    client.pushPacket(MqttPacket::ack5(MQTTPUBCOMP, id, -1));
    REQUIRE(psc.loop());

    CHECK(psc.inFlightCount() == 0);
    CHECK(psc.lastPacketId() == id);
    CHECK(psc.lastReasonCode() == 0x00);
    CHECK(psc.lastError() == MqttError::None);
    CHECK(psc.connected());
    CHECK(client.outbound().empty());
}

TEST_CASE("a PUBREC of 0x80 or above sends no PUBREL and reports the failure") {
    // 0x80 is the bottom of the failure range; 0x87 (Not authorized) and 0x99
    // (Payload format invalid) are packet-specific PUBREC failure codes.
    const uint8_t failureCodes[] = {0x80, 0x87, 0x99};

    for (uint8_t rc : failureCodes) {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        mqtt5_qos::connect(client, psc);
        psc.setKeepAlive(0);

        const uint16_t id = mqtt5_qos::publishQos2AndReadId(client, psc, "tele/dev/STATE");
        REQUIRE(psc.inFlightCount() == 1);

        client.clearOutbound();
        client.pushPacket(MqttPacket::ack5(MQTTPUBREC, id, (int)rc));
        REQUIRE(psc.loop());

        // Requirement 8.14: no PUBREL - not one byte - the identifier released, and the
        // received code exposed together with that released identifier.
        CHECK(client.outbound().empty());
        CHECK(psc.inFlightCount() == 0);
        CHECK(psc.lastPacketId() == id);
        CHECK(psc.lastReasonCode() == rc);
        // A refused message is a normal broker answer, so the connection stays up.
        CHECK(psc.lastError() == MqttError::None);
        CHECK(psc.connected());
        CHECK_FALSE(client.stopCalled());

        // A PUBCOMP for the abandoned identifier now correlates to nothing and is discarded.
        client.pushPacket(MqttPacket::ack5(MQTTPUBCOMP, id, -1));
        REQUIRE(psc.loop());
        CHECK(psc.inFlightCount() == 0);
        CHECK(client.outbound().empty());
    }
}

TEST_CASE("a duplicate PUBREC does not produce a second PUBREL") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_qos::connect(client, psc);
    psc.setKeepAlive(0);

    const uint16_t id = mqtt5_qos::publishQos2AndReadId(client, psc, "tele/dev/STATE");

    client.clearOutbound();
    client.pushPacket(MqttPacket::ack5(MQTTPUBREC, id, 0x00));
    REQUIRE(psc.loop());
    REQUIRE(client.outbound().size() == 4);      // the one PUBREL

    // The entry now sits awaiting the PUBCOMP, so a repeated PUBREC correlates to nothing in
    // the state a PUBREL answers - "exactly one PUBREL" (Requirement 8.12).
    client.clearOutbound();
    client.pushPacket(MqttPacket::ack5(MQTTPUBREC, id, 0x00));
    REQUIRE(psc.loop());
    CHECK(client.outbound().empty());
    CHECK(psc.inFlightCount() == 1);

    // And the PUBCOMP still completes it.
    client.pushPacket(MqttPacket::ack5(MQTTPUBCOMP, id, -1));
    REQUIRE(psc.loop());
    CHECK(psc.inFlightCount() == 0);
    CHECK(psc.lastPacketId() == id);
}

TEST_CASE("a QoS 2 acknowledgement matching nothing leaves every entry unchanged") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_qos::connect(client, psc);
    psc.setKeepAlive(0);

    const uint16_t subId = mqtt5_qos::subscribeAndReadId(client, psc, "cmnd/dev/#");
    const uint16_t pubId = mqtt5_qos::publishQos2AndReadId(client, psc, "tele/dev/STATE");
    REQUIRE(subId != pubId);
    REQUIRE(psc.inFlightCount() == 2);

    SUBCASE("a PUBREC naming a SUBSCRIBE's identifier") {
        client.clearOutbound();
        client.pushPacket(MqttPacket::ack5(MQTTPUBREC, subId, 0x00));
        REQUIRE(psc.loop());

        // Requirement 7.8: discarded, so no PUBREL and the subscription still waits.
        CHECK(client.outbound().empty());
        CHECK(psc.inFlightCount() == 2);
        CHECK(psc.lastPacketId() == 0);
        CHECK(psc.connected());
    }

    SUBCASE("a PUBCOMP arriving before any PUBREL was sent") {
        // The publish is awaiting its PUBREC, so the broker has skipped a step: discarded,
        // and the entry keeps its identifier until the real PUBREC arrives.
        client.clearOutbound();
        client.pushPacket(MqttPacket::ack5(MQTTPUBCOMP, pubId, 0x00));
        REQUIRE(psc.loop());

        CHECK(psc.inFlightCount() == 2);
        CHECK(psc.lastPacketId() == 0);

        // Still advanceable through the proper sequence.
        client.pushPacket(MqttPacket::ack5(MQTTPUBREC, pubId, 0x00));
        REQUIRE(psc.loop());
        CHECK(client.outbound().size() == 4);
        CHECK(psc.inFlightCount() == 2);
    }
}

// ===========================================================================
//  Task 11.6 - Property 12: Packet Identifier uniqueness and release
// ===========================================================================

namespace mqtt5_p12 {

// The four entry points that allocate a Packet Identifier. All four draw from the ONE
// shared allocator, which is what makes "differs from the identifier of every other
// Outstanding_Message regardless of which packet type created it" a claim about the whole
// table rather than one per packet type.
enum class Kind { PublishQos1, PublishQos2, Subscribe, Unsubscribe };

const char* kindName(Kind k) {
    switch (k) {
    case Kind::PublishQos1: return "publish QoS 1";
    case Kind::PublishQos2: return "publish QoS 2";
    case Kind::Subscribe:   return "subscribe";
    case Kind::Unsubscribe: return "unsubscribe";
    }
    return "?";
}

// One started-but-unfinished operation as the TEST models it: the identifier read back off
// the wire, plus how far its acknowledgement sequence has got. The library's table is never
// inspected - inFlightCount() is compared against this model instead.
struct Live {
    Kind     kind = Kind::PublishQos1;
    uint16_t id = 0;
    bool     pubrelSent = false;      // QoS 2 only: PUBREC seen, PUBREL emitted
};

// A connected client plus the model of what it currently holds. Every mutator asserts the
// property's invariant afterwards, so a script reads as a sequence of operations and the
// uniqueness / release checks happen at every instant in between without being written out.
struct Session {
    MockClient        client;
    PubSubClient      psc;
    std::vector<Live> live;

    // Where the allocator's rotor stands, tracked purely from identifiers observed on the
    // wire. A successful CONNACK resets it to 1, so the first allocation yields 2.
    uint16_t rotor = 1;

    // Aggregated verdict of the rotor walks below. They run tens of thousands of steps, so
    // asserting per step would add tens of thousands of assertions to the run; the walk
    // records a single flag instead and the caller checks it once.
    bool walkOk = true;

    Session() : psc(client) {
        TestClock::instance().reset();
        mqtt5_qos::connect(client, psc);
        // Keepalive off: no PINGREQ may appear among the bytes these cases decode, and the
        // rotor walks advance no virtual time.
        psc.setKeepAlive(0);
    }

    // The property's invariant, at this instant: the count the library reports is exactly
    // the set of operations still waiting, every identifier in it is legal (1..65535, i.e.
    // never 0 - the type bounds the top), and no two of them are equal.
    void checkInvariant() const {
        CHECK(psc.inFlightCount() == live.size());
        std::set<uint16_t> seen;
        for (const Live& l : live) {
            CHECK(l.id != 0);
            CHECK(seen.insert(l.id).second);
        }
    }

    size_t indexOf(uint16_t id) const {
        for (size_t i = 0; i < live.size(); i++) {
            if (live[i].id == id) {
                return i;
            }
        }
        REQUIRE_MESSAGE(false, "the model holds no entry for this identifier");
        return 0;
    }

    void release(uint16_t id) {
        live.erase(live.begin() + static_cast<long>(indexOf(id)));
    }

    // Start one operation and return the Packet Identifier its emitted packet carries,
    // decoded from the outbound bytes rather than assumed.
    uint16_t start(Kind kind, const std::string& topic) {
        // Scripts stay inside the complete tracked envelope. The public capacity tests below
        // cover atomic refusal once that envelope is full.
        REQUIRE(live.size() < static_cast<size_t>(MQTT_MAX_OUTSTANDING));

        uint16_t id = 0;
        client.clearOutbound();
        switch (kind) {
        case Kind::PublishQos1:
        case Kind::PublishQos2: {
            const uint8_t qos = (kind == Kind::PublishQos1) ? 1 : 2;
            REQUIRE(psc.publish(topic.c_str(), (const uint8_t*)"ON", 2, false, qos));
            const DecodedPublish5 p = MqttParser::decodePublish5(client.outbound());
            REQUIRE(p.valid);
            REQUIRE(p.qos == qos);
            id = p.msgId;
            break;
        }
        case Kind::Subscribe: {
            REQUIRE(psc.subscribe(topic.c_str()));
            const DecodedSubscribe5 s = MqttParser::decodeSubscribe5(client.outbound());
            REQUIRE(s.valid);
            id = s.msgId;
            break;
        }
        case Kind::Unsubscribe:
            REQUIRE(psc.unsubscribe(topic.c_str()));
            id = mqtt5_qos::emittedPacketId(client.outbound());
            break;
        }

        rotor = id;
        live.push_back(Live{kind, id, false});
        checkInvariant();
        return id;
    }

    // --- Acknowledgement steps ---------------------------------------------
    //
    // Each one completes or advances exactly one sequence and asserts the release semantics
    // for it: the identifier is reported to the caller, the entry is gone from the table,
    // and nothing else moved.

    void puback(uint16_t id, uint8_t rc = 0x00) {
        client.clearOutbound();
        client.pushPacket(MqttPacket::ack5(MQTTPUBACK, id, static_cast<int>(rc)));
        REQUIRE(psc.loop());
        release(id);
        CHECK(psc.lastPacketId() == id);
        CHECK(psc.lastReasonCode() == rc);
        CHECK(client.outbound().empty());
        checkInvariant();
    }

    // PUBREC below 0x80 advances the handshake and the entry KEEPS its identifier; 0x80 or
    // above ends it there, releasing the identifier with no PUBREL sent.
    void pubrec(uint16_t id, uint8_t rc = 0x00) {
        client.clearOutbound();
        client.pushPacket(MqttPacket::ack5(MQTTPUBREC, id, static_cast<int>(rc)));
        REQUIRE(psc.loop());
        if (rc < 0x80) {
            const DecodedAck5 rel = MqttParser::decodeAck5(client.outbound(), MQTTPUBREL);
            REQUIRE(rel.valid);
            CHECK(rel.packetId == id);
            live[indexOf(id)].pubrelSent = true;
        } else {
            CHECK(client.outbound().empty());
            release(id);
            CHECK(psc.lastPacketId() == id);
            CHECK(psc.lastReasonCode() == rc);
        }
        checkInvariant();
    }

    void pubcomp(uint16_t id, uint8_t rc = 0x00) {
        REQUIRE(live[indexOf(id)].pubrelSent);
        client.clearOutbound();
        client.pushPacket(MqttPacket::ack5(MQTTPUBCOMP, id, static_cast<int>(rc)));
        REQUIRE(psc.loop());
        release(id);
        CHECK(psc.lastPacketId() == id);
        CHECK(psc.lastReasonCode() == rc);
        CHECK(client.outbound().empty());
        checkInvariant();
    }

    void suback(uint16_t id, uint8_t rc = 0x00) {
        client.clearOutbound();
        client.pushPacket(MqttPacket::suback5(id, {rc}));
        REQUIRE(psc.loop());
        release(id);
        CHECK(psc.lastSubscriptionResult().packetId == id);
        CHECK(client.outbound().empty());
        checkInvariant();
    }

    void unsuback(uint16_t id, uint8_t rc = 0x00) {
        client.clearOutbound();
        client.pushPacket(MqttPacket::unsuback5(id, {rc}));
        REQUIRE(psc.loop());
        release(id);
        CHECK(psc.lastSubscriptionResult().packetId == id);
        CHECK(client.outbound().empty());
        checkInvariant();
    }

    // A local timeout is diagnostic-only: every operation remains in the model and keeps its
    // identifier so a late terminal acknowledgement can still complete it.
    void timeoutAll() {
        REQUIRE_FALSE(live.empty());
        TestClock::instance().advance((static_cast<unsigned long>(MQTT_ACK_TIMEOUT) + 1UL)
                                     * 1000UL);
        REQUIRE(psc.loop());
        checkInvariant();
        CHECK(psc.connected());
        CHECK_FALSE(client.stopCalled());
    }

    // --- Rotor walking ------------------------------------------------------

    // Complete each walk operation immediately. This permits a full identifier-space walk
    // without ever sending an untracked subscription, while any modelled held identifier
    // remains in the allocator's skip set throughout the walk.
    uint16_t burn() {
        client.clearOutbound();
        if (!psc.unsubscribe("t")) {
            walkOk = false;
            return 0;
        }
        const std::vector<uint8_t>& out = client.outbound();
        if (out.size() < 4) {
            walkOk = false;
            return 0;
        }
        const uint16_t id = static_cast<uint16_t>((static_cast<uint16_t>(out[2]) << 8) | out[3]);
        if (id == 0) {
            walkOk = false;
            return 0;
        }
        client.clearOutbound();
        client.pushPacket(MqttPacket::unsuback5(id, {0x00}));
        if (!psc.loop() || !client.outbound().empty()) {
            walkOk = false;
            return 0;
        }
        rotor = id;
        return id;
    }

    // Every rotor walk operation terminally ACKs itself, so the table can only be empty when
    // the model is empty too; a timeout must never be used as a local clearing shortcut.
    void emptyTable() {
        REQUIRE(live.empty());
        REQUIRE(psc.inFlightCount() == 0);
    }

    // Walk the rotor until its next allocation will yield `target`. The table is emptied
    // first (so `target - 1` cannot be one of the held values the rotor skips) and again
    // afterwards (so the walk's own entries hold nothing when the operation under test
    // allocates).
    void primeTo(uint16_t target) {
        emptyTable();
        const uint16_t previous = (target == 1) ? 65535 : static_cast<uint16_t>(target - 1);
        for (uint32_t steps = 0; (rotor != previous) && (steps <= 70000u); steps++) {
            burn();
        }
        CHECK(walkOk);
        REQUIRE(rotor == previous);
        emptyTable();
    }
};

}  // namespace mqtt5_p12

// Feature: tasmota-pubsub-mqtt5, Property 12: For all interleavings of QoS 1 publishes, QoS 2 publishes, subscribes and unsubscribes with their acknowledgements, every Packet Identifier held by an Outstanding_Message at any instant is in 1..65535 and differs from the identifier of every other Outstanding_Message regardless of which packet type created it; an identifier becomes available for reuse exactly when its acknowledgement sequence completes or its entry is abandoned; and when no identifier can be allocated the operation is rejected with a failure indication and no byte of that packet is transmitted.
//
// Validates: Requirements 8.4, 8.5, 8.6, 8.16
TEST_CASE("Property 12: Packet Identifier uniqueness and release") {
    using mqtt5_p12::Kind;
    using mqtt5_p12::Session;

    SUBCASE("curated interleavings of the four packet types with their acknowledgements") {
        SUBCASE("all four in flight at once, acknowledged in a different order") {
            Session s;
            const uint16_t a = s.start(Kind::PublishQos1, "tele/a/STATE");
            const uint16_t b = s.start(Kind::Subscribe,   "cmnd/b/#");
            const uint16_t c = s.start(Kind::PublishQos2, "tele/c/STATE");
            const uint16_t d = s.start(Kind::Unsubscribe, "cmnd/d/#");
            // Four identifiers from one allocator, one per packet type - the uniqueness is
            // asserted by checkInvariant() after every step above.
            CHECK(s.live.size() == 4);

            s.suback(b);
            s.pubrec(c);            // c keeps its identifier: the handshake is not over
            CHECK(s.psc.inFlightCount() == 3);
            s.puback(a);
            s.pubcomp(c);           // now it is
            s.unsuback(d);
            CHECK(s.psc.inFlightCount() == 0);
        }

        SUBCASE("strictly alternating start and acknowledgement, twice round the kinds") {
            Session s;
            for (int round = 0; round < 2; round++) {
                CAPTURE(round);

                const uint16_t two = s.start(Kind::PublishQos2, "tele/alt/STATE");
                s.pubrec(two);
                s.pubcomp(two);

                s.puback(s.start(Kind::PublishQos1, "tele/alt/STATE"));
                s.suback(s.start(Kind::Subscribe, "cmnd/alt/#"));
                s.unsuback(s.start(Kind::Unsubscribe, "cmnd/alt/#"));

                // The table is empty between every pair, so the identifiers are free to
                // repeat - and the rotor still hands out a different one every time.
                CHECK(s.psc.inFlightCount() == 0);
            }
        }

        SUBCASE("timeouts retain every live operation for late terminal acknowledgements") {
            Session s;
            const uint16_t a = s.start(Kind::PublishQos1, "tele/a/STATE");
            const uint16_t b = s.start(Kind::PublishQos2, "tele/b/STATE");
            const uint16_t c = s.start(Kind::Subscribe,   "cmnd/c/#");
            const uint16_t d = s.start(Kind::Unsubscribe, "cmnd/d/#");

            s.timeoutAll();
            CHECK(s.psc.lastPacketId() != 0);
            // Repeated sweeps are inert for held exchanges and emit no retransmission.
            s.client.clearOutbound();
            s.timeoutAll();
            CHECK(s.client.outbound().empty());

            // Every late ACK still reaches its original live operation.
            s.puback(a);
            s.pubrec(b);
            s.pubcomp(b);
            s.suback(c);
            s.unsuback(d);
            CHECK(s.psc.inFlightCount() == 0);

            // Terminal completion, rather than expiry, makes capacity available again.
            const uint16_t e = s.start(Kind::PublishQos1, "tele/e/STATE");
            const uint16_t f = s.start(Kind::PublishQos2, "tele/f/STATE");
            const uint16_t g = s.start(Kind::Subscribe,   "cmnd/g/#");
            const uint16_t h = s.start(Kind::Unsubscribe, "cmnd/h/#");
            s.pubrec(f);
            s.puback(e);
            s.suback(g);
            s.pubcomp(f);
            s.unsuback(h);
            CHECK(s.psc.inFlightCount() == 0);
        }

        SUBCASE("a failing PUBREC releases only its own identifier") {
            Session s;
            const uint16_t a = s.start(Kind::PublishQos2, "tele/a/STATE");
            const uint16_t b = s.start(Kind::PublishQos2, "tele/b/STATE");
            const uint16_t c = s.start(Kind::Subscribe,   "cmnd/c/#");

            // 0x87 Not authorized: abandoned without a PUBREL, identifier released
            // (Requirement 8.14), and the other two entries untouched.
            s.pubrec(a, 0x87);
            CHECK(s.psc.inFlightCount() == 2);

            s.pubrec(b);
            s.pubcomp(b);
            s.suback(c);
            CHECK(s.psc.inFlightCount() == 0);
        }

        SUBCASE("acknowledgements for identifiers nothing holds change nothing") {
            Session s;
            const uint16_t a = s.start(Kind::PublishQos1, "tele/a/STATE");
            const uint16_t b = s.start(Kind::PublishQos2, "tele/b/STATE");

            // An identifier no entry holds, and one held by the WRONG kind of operation:
            // neither completes anything, so no identifier becomes available.
            uint16_t stray = 1;
            while ((stray == a) || (stray == b)) {
                stray++;
            }
            const uint8_t types[] = {static_cast<uint8_t>(MQTTPUBACK),
                                     static_cast<uint8_t>(MQTTPUBREC),
                                     static_cast<uint8_t>(MQTTPUBCOMP)};
            for (uint8_t type : types) {
                CAPTURE(type);
                s.client.clearOutbound();
                s.client.pushPacket(MqttPacket::ack5(type, stray, 0x00));
                REQUIRE(s.psc.loop());
                s.checkInvariant();
                CHECK(s.client.outbound().empty());
            }

            // A PUBACK naming the QoS 2 publish's identifier: the wrong sequence for that
            // entry, so it is discarded and the identifier stays held.
            s.client.pushPacket(MqttPacket::ack5(MQTTPUBACK, b, 0x00));
            REQUIRE(s.psc.loop());
            s.checkInvariant();

            // Both still complete through their own sequences.
            s.puback(a);
            s.pubrec(b);
            s.pubcomp(b);
            CHECK(s.psc.inFlightCount() == 0);
        }
    }

    SUBCASE("the identifier vectors 1, 2, 0x00FF, 0x0100, 0xFFFE, 0xFFFF") {
        // Ascending, so ONE walk of the identifier space serves the whole table: reaching
        // 0xFFFF leaves the rotor one step from the wrap, which is how identifier 1 is
        // reached last without sweeping the space a second time. The kinds rotate, so each
        // boundary value is exercised by a different packet type.
        struct Vector {
            uint16_t id;
            Kind     kind;
        };
        const Vector vectors[] = {
            {2,      Kind::PublishQos1},    // the first identifier a fresh connection issues
            {0x00FF, Kind::PublishQos2},    // the byte boundary, from below
            {0x0100, Kind::Subscribe},      // and from above
            {0xFFFE, Kind::Unsubscribe},
            {0xFFFF, Kind::PublishQos1},    // the top of the legal range
            {1,      Kind::PublishQos2},    // what the rotor wraps to, since 0 is illegal
        };

        Session s;
        for (const Vector& v : vectors) {
            CAPTURE(v.id);
            CAPTURE(mqtt5_p12::kindName(v.kind));

            s.primeTo(v.id);
            const uint16_t id = s.start(v.kind, "tele/vector/STATE");
            CHECK(id == v.id);

            switch (v.kind) {
            case Kind::PublishQos1: s.puback(id); break;
            case Kind::PublishQos2: s.pubrec(id); s.pubcomp(id); break;
            case Kind::Subscribe:   s.suback(id); break;
            case Kind::Unsubscribe: s.unsuback(id); break;
            }
            CHECK(s.psc.inFlightCount() == 0);
        }
    }

    SUBCASE("a held identifier is skipped for a whole wrap; a released one is reissued") {
        Session s;
        const uint16_t released = s.start(Kind::PublishQos1, "tele/released/STATE");
        const uint16_t held     = s.start(Kind::PublishQos2, "tele/held/STATE");
        REQUIRE(released != held);

        // One sequence completes, the other is left mid-handshake. From here the two
        // identifiers must behave in opposite ways for as long as that lasts.
        s.puback(released);
        REQUIRE(s.psc.inFlightCount() == 1);

        // Walk the whole identifier space once. No virtual time passes and loop() is never
        // called, so nothing abandons the held entry along the way.
        bool sawHeld = false;
        bool sawReleased = false;
        bool sawZero = false;
        uint16_t maximum = 0;
        for (uint32_t i = 0; i < 65540u; i++) {
            const uint16_t id = s.burn();
            if (id == 0)        { sawZero = true; }
            if (id == held)     { sawHeld = true; }
            if (id == released) { sawReleased = true; }
            if (id > maximum)   { maximum = id; }
        }
        CHECK(s.walkOk);

        // Every identifier issued is legal, and the whole range is covered - so the two
        // checks below really are about the whole space, not about a short walk.
        CHECK_FALSE(sawZero);
        CHECK(maximum == 65535);
        // Held by an Outstanding_Message, therefore never reissued (Requirement 8.4) ...
        CHECK_FALSE(sawHeld);
        // ... while the one whose sequence completed came back round for reuse
        // (Requirement 8.16). Together: available for reuse exactly on release.
        CHECK(sawReleased);

        // The held entry kept its identifier across the whole wrap (Requirement 8.6), so its
        // handshake still completes with that identifier on the PUBREL. Asserted directly
        // rather than through the model, since the walk left unmodelled UNSUBSCRIBE entries
        // in the table.
        s.client.clearOutbound();
        s.client.pushPacket(MqttPacket::ack5(MQTTPUBREC, held, 0x00));
        REQUIRE(s.psc.loop());
        const DecodedAck5 rel = MqttParser::decodeAck5(s.client.outbound(), MQTTPUBREL);
        REQUIRE(rel.valid);
        CHECK(rel.packetId == held);

        s.client.clearOutbound();
        s.client.pushPacket(MqttPacket::ack5(MQTTPUBCOMP, held, -1));
        REQUIRE(s.psc.loop());
        CHECK(s.psc.lastPacketId() == held);
        CHECK(s.psc.lastReasonCode() == 0x00);
    }

    SUBCASE("table saturation applies Receive Maximum back-pressure without transmission") {
        for (uint8_t qos = 1; qos <= 2; qos++) {
            CAPTURE(qos);
            // Each vector starts a fresh connection because the intentional wait advances
            // the virtual clock through the acknowledgement window. No old operation is
            // allowed to be abandoned as a side effect of proving this refusal.
            Session s;
            for (int i = 0; i < MQTT_MAX_INFLIGHT; i++) {
                const Kind kind = ((i % 2) == 0) ? Kind::PublishQos1 : Kind::PublishQos2;
                s.start(kind, "tele/dev-" + std::to_string(i) + "/STATE");
            }
            REQUIRE(s.psc.inFlightCount() == MQTT_MAX_INFLIGHT);

            s.client.clearOutbound();
            // With no acknowledgement, the in-place wait reaches its deadline. The
            // borrowed payload has not been consumed and no PUBLISH byte reaches wire.
            CHECK_FALSE(s.psc.publish("tele/full/STATE", (const uint8_t*)"ON", 2, false, qos));
            CHECK(s.psc.lastError() == MqttError::ReceiveMaximumReached);
            CHECK(s.client.outbound().empty());
            CHECK(s.psc.inFlightCount() == MQTT_MAX_INFLIGHT);
            CHECK(s.psc.connected());
            CHECK_FALSE(s.client.stopCalled());
        }
    }
}

// ===========================================================================
//  Task 11.7 - Property 13: Outbound QoS 1 lifecycle
// ===========================================================================

namespace mqtt5_p13 {

// One of the three encodings ack5() can produce for a PUBACK. The short form has no room
// for a Reason Code byte at all - it is Remaining Length 2, identifier only - so it can
// only ever imply 0x00; it is therefore exercised only when the Reason Code under test IS
// 0x00 (below), rather than attempted for every code and skipped when it does not fit.
enum class Form { Short, Mid, Long };

MqttPacket buildPuback(uint16_t id, uint8_t rc, Form form) {
    switch (form) {
    case Form::Short: return MqttPacket::ack5(MQTTPUBACK, id, -1);
    case Form::Mid:   return MqttPacket::ack5(MQTTPUBACK, id, static_cast<int>(rc));
    case Form::Long:
    default:
        return MqttPacket::ack5(MQTTPUBACK, id, static_cast<int>(rc),
                                MqttProps().str(0x1F, "reason"));
    }
}

// Reconnect and return only the bytes written AFTER the CONNECT/CONNACK exchange - i.e.
// any retransmission and nothing else. Mirrors mqtt5_retransmit::reconnect, kept local to
// this file so this property test does not reach across test files for it.
std::vector<uint8_t> reconnectAndReadExtra(MockClient& client, PubSubClient& psc,
                                           bool sessionPresent) {
    client.pushPacket(MqttPacket::connack(0, sessionPresent));
    client.clearOutbound();
    REQUIRE(psc.connect("mqtt5-p13", nullptr, nullptr, nullptr, 0, false, nullptr,
                        /*cleanSession=*/false));
    REQUIRE(psc.connected());
    CHECK(psc.sessionPresent() == sessionPresent);
    const std::vector<uint8_t>& out = client.outbound();
    REQUIRE(out.size() >= 2);
    REQUIRE((out[0] & 0xF0) == MQTTCONNECT);
    size_t pos = 1;
    uint32_t remaining = 0;
    uint32_t multiplier = 1;
    uint8_t encoded = 0;
    do {
        REQUIRE(pos < out.size());
        encoded = out[pos++];
        remaining += static_cast<uint32_t>(encoded & 0x7F) * multiplier;
        multiplier *= 128;
    } while ((encoded & 0x80) != 0);
    pos += remaining;
    REQUIRE(pos <= out.size());
    return std::vector<uint8_t>(out.begin() + static_cast<long>(pos), out.end());
}

// One full lifecycle: connect, publish at QoS 1, deliver the PUBACK in the given form
// carrying the given Reason Code, and check every observable the property names -
// completion, identifier release, and the exposed Packet Identifier and Reason Code
// (Requirements 7.3, 7.4, 8.16). Then, because the property specifically calls a Reason
// Code of 0x80 or above terminal, drop the connection and reconnect with Session Present
// 1 - the one moment a retransmission could legally appear - and confirm nothing goes out
// for this message: the concrete, wire-level meaning of "treated as acknowledged and
// never retransmitted" (Requirement 8.9), checked for every code so the 0x80 boundary is
// shown from both sides rather than assumed.
void checkLifecycle(uint8_t rc, Form form) {
    CAPTURE(rc);
    CAPTURE(static_cast<int>(form));

    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_qos::connect(client, psc);
    psc.setKeepAlive(0);

    const uint16_t id = mqtt5_qos::publishQos1AndReadId(client, psc, "tele/dev/STATE");
    REQUIRE(psc.inFlightCount() == 1);

    client.clearOutbound();
    client.pushPacket(buildPuback(id, rc, form));
    REQUIRE(psc.loop());

    // Requirement 8.16: the Outstanding_Message is completed and the identifier released.
    CHECK(psc.inFlightCount() == 0);
    // Requirements 7.3, 7.4: which message, and with which numeric code - exposed
    // identically whether the code falls in the success or the failure range.
    CHECK(psc.lastPacketId() == id);
    CHECK(psc.lastReasonCode() == rc);
    // A PUBACK, of either range, is a normal broker answer rather than a transport
    // failure: no error is latched, the connection stays up, and nothing goes out for it.
    CHECK(psc.lastError() == MqttError::None);
    CHECK(psc.connected());
    CHECK_FALSE(client.stopCalled());
    CHECK(client.outbound().empty());

    // Requirement 8.9: terminal means gone, not merely quiet - a reconnect that resumes
    // the session is the only moment [MQTT-4.4.0-1] permits a retransmission at all, and
    // even there this completed message contributes zero bytes. True for every code, and
    // this is the direct check for the 0x80-and-above range the property calls out.
    psc.disconnect();
    REQUIRE_FALSE(psc.connected());
    client.clearOutbound();
    client.clearInbound();
    const std::vector<uint8_t> afterReconnect =
        reconnectAndReadExtra(client, psc, /*sessionPresent=*/false);
    CHECK(afterReconnect.empty());
    CHECK(psc.inFlightCount() == 0);
}

}  // namespace mqtt5_p13

// Feature: tasmota-pubsub-mqtt5, Property 13: For all Packet Identifiers and for all PUBACK Reason Codes, an outbound QoS 1 PUBLISH's Outstanding_Message is completed and its identifier released when the matching PUBACK arrives; the numeric Reason Code is exposed to the caller; and a Reason Code of 0x80 or above is terminal (the message is treated as acknowledged and never retransmitted).
//
// Validates: Requirements 7.3, 7.4, 8.9, 8.16
TEST_CASE("Property 13: Outbound QoS 1 lifecycle") {
    using mqtt5_p13::Form;

    // Exhaustive over the whole Reason Code byte, crossed with the mid and long forms -
    // both of which can represent ANY Reason Code - so every one of the 256 codes gets a
    // full lifecycle check, including the reconnect-based terminal check, in each of those
    // two encodings.
    SUBCASE("every PUBACK Reason Code, in the mid and long forms") {
        const uint8_t legalReasonCodes[] = {
            0x00, 0x10, 0x80, 0x83, 0x87, 0x90, 0x91, 0x97, 0x99
        };
        for (const uint8_t rc : legalReasonCodes) {
            mqtt5_p13::checkLifecycle(rc, Form::Mid);
            mqtt5_p13::checkLifecycle(rc, Form::Long);
        }
    }

    // The short form has no room for a Reason Code byte at all, so it can only ever
    // represent 0x00 - this is the one Reason Code it is exercised at, completing the
    // "three acknowledgement forms" coverage the task names.
    SUBCASE("Reason Code 0x00, in the short form") {
        mqtt5_p13::checkLifecycle(0x00, Form::Short);
    }
}

// ===========================================================================
//  Task 11.8 - Property 14: Outbound QoS 2 handshake
// ===========================================================================

namespace mqtt5_p14 {

// One full outbound QoS 2 handshake at the given PUBREC Reason Code: connect, publish at
// QoS 2, deliver the PUBREC, and check every observable the property names - the PUBREL
// framing and correlation on the success side (Requirement 8.12), or the no-PUBREL
// failure exposure on the other (Requirement 8.14).
void checkHandshake(uint8_t rc) {
    CAPTURE(rc);

    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_qos::connect(client, psc);
    // Keepalive off, so every byte recorded below belongs to the handshake.
    psc.setKeepAlive(0);

    const uint16_t id = mqtt5_qos::publishQos2AndReadId(client, psc, "tele/dev/STATE");
    REQUIRE(psc.inFlightCount() == 1);

    client.clearOutbound();
    client.pushPacket(MqttPacket::ack5(MQTTPUBREC, id, static_cast<int>(rc)));
    REQUIRE(psc.loop());

    if (rc < 0x80) {
        // Requirement 8.12: exactly one PUBREL, carrying the SAME identifier, with the
        // mandatory fixed-header flags 0x02 - so the whole answer is four bytes.
        const std::vector<uint8_t> pubrel = client.outbound();
        REQUIRE(pubrel.size() == 4);
        CHECK(pubrel[0] == static_cast<uint8_t>(MQTTPUBREL | 0x02));
        const DecodedAck5 rel = MqttParser::decodeAck5(pubrel, MQTTPUBREL);
        REQUIRE(rel.valid);
        CHECK(rel.packetId == id);
        CHECK(rel.form == 0);

        // The exchange has advanced but is NOT over yet: the entry still holds its
        // identifier, and nothing is latched as a result.
        CHECK(psc.inFlightCount() == 1);
        CHECK(psc.connected());
        CHECK(psc.lastError() == MqttError::None);

        // The PUBCOMP ends it - complete, identifier released, code reported with the
        // identifier it belongs to, and no further byte out.
        client.clearOutbound();
        client.pushPacket(MqttPacket::ack5(MQTTPUBCOMP, id, -1));
        REQUIRE(psc.loop());

        CHECK(psc.inFlightCount() == 0);
        CHECK(psc.lastPacketId() == id);
        CHECK(psc.lastReasonCode() == 0x00);
        CHECK(psc.lastError() == MqttError::None);
        CHECK(psc.connected());
        CHECK(client.outbound().empty());
    } else {
        // Requirement 8.14: no PUBREL - not one byte - the identifier released, and the
        // received code exposed together with that released identifier.
        CHECK(client.outbound().empty());
        CHECK(psc.inFlightCount() == 0);
        CHECK(psc.lastPacketId() == id);
        CHECK(psc.lastReasonCode() == rc);
        // A refused message is a normal broker answer: the connection stays up and no
        // error state is latched.
        CHECK(psc.lastError() == MqttError::None);
        CHECK(psc.connected());
        CHECK_FALSE(client.stopCalled());

        // A PUBCOMP for the abandoned identifier now correlates to nothing and is
        // discarded.
        client.pushPacket(MqttPacket::ack5(MQTTPUBCOMP, id, -1));
        REQUIRE(psc.loop());
        CHECK(psc.inFlightCount() == 0);
        CHECK(client.outbound().empty());
    }
}

}  // namespace mqtt5_p14

// Feature: tasmota-pubsub-mqtt5, Property 14: For all Packet Identifiers, an outbound QoS
// 2 PUBLISH answered by a PUBREC with a Reason Code below 0x80 produces exactly one
// PUBREL carrying that identifier and completes on the matching PUBCOMP with the
// identifier released; and answered by a PUBREC with a Reason Code of 0x80 or above it
// produces no PUBREL, releases the identifier, and exposes both the Reason Code and the
// released identifier as a failure result.
//
// Validates: Requirements 8.12, 8.14
TEST_CASE("Property 14: Outbound QoS 2 handshake") {
    // Exhaustive over the whole Reason Code byte, so every one of the 256 possible PUBREC
    // codes gets a full handshake check on whichever side of the 0x80 boundary it falls.
    SUBCASE("every PUBREC Reason Code, 0x00 through 0xFF") {
        const uint8_t legalReasonCodes[] = {
            0x00, 0x10, 0x80, 0x83, 0x87, 0x90, 0x91, 0x97, 0x99
        };
        for (const uint8_t rc : legalReasonCodes) {
            mqtt5_p14::checkHandshake(rc);
        }
    }

    // The identifier boundary vectors: 1 (what the rotor wraps to), 2 (the first
    // identifier a fresh connection issues), the byte boundary from both sides, and the
    // top of the legal range. mqtt5_p12::Session already primes the rotor to an exact
    // target and models the same PUBREC/PUBCOMP sequence this property is about, so it is
    // reused rather than duplicated: the handshake behavior is exercised at each boundary
    // identifier on both sides of the Reason Code split.
    SUBCASE("the identifier boundary vectors 1, 2, 0x00FF, 0x0100, 0xFFFE, 0xFFFF") {
        using mqtt5_p12::Kind;
        using mqtt5_p12::Session;

        const uint16_t vectors[] = {1, 2, 0x00FF, 0x0100, 0xFFFE, 0xFFFF};
        for (uint16_t target : vectors) {
            CAPTURE(target);

            // The success side: PUBREC below 0x80 produces exactly one correctly framed
            // PUBREL carrying this exact identifier, and the PUBCOMP completes it.
            {
                Session s;
                s.primeTo(target);
                const uint16_t id = s.start(Kind::PublishQos2, "tele/vector/STATE");
                REQUIRE(id == target);

                s.pubrec(id, 0x00);
                const std::vector<uint8_t> pubrel = s.client.outbound();
                REQUIRE(pubrel.size() == 4);
                CHECK(pubrel[0] == static_cast<uint8_t>(MQTTPUBREL | 0x02));
                const DecodedAck5 rel = MqttParser::decodeAck5(pubrel, MQTTPUBREL);
                REQUIRE(rel.valid);
                CHECK(rel.packetId == id);

                s.pubcomp(id, 0x00);
                CHECK(s.psc.inFlightCount() == 0);
            }

            // The failure side, at the same identifier: PUBREC 0x80 or above produces no
            // PUBREL and releases the identifier with the Reason Code exposed.
            {
                Session s;
                s.primeTo(target);
                const uint16_t id = s.start(Kind::PublishQos2, "tele/vector/STATE");
                REQUIRE(id == target);

                s.pubrec(id, 0x87);
                CHECK(s.psc.inFlightCount() == 0);
                CHECK(s.psc.lastPacketId() == id);
                CHECK(s.psc.lastReasonCode() == 0x87);
            }
        }
    }
}

// ===========================================================================
//  Task 12.4 - Property 11: Acknowledgement short and long forms are equivalent
// ===========================================================================

namespace mqtt5_p11 {

enum class PacketType { Puback, Pubrec, Pubrel, Pubcomp };
enum class Form { Short, Mid, Long };

struct Outcome {
    uint8_t             inFlight = 0;
    uint16_t            lastPacketId = 0;
    uint8_t             lastReasonCode = 0;
    MqttError           lastError = MqttError::None;
    bool                connected = false;
    std::vector<uint8_t> outbound;
};

const char* packetTypeName(PacketType type) {
    switch (type) {
    case PacketType::Puback:  return "PUBACK";
    case PacketType::Pubrec:  return "PUBREC";
    case PacketType::Pubrel:  return "PUBREL";
    case PacketType::Pubcomp: return "PUBCOMP";
    }
    return "?";
}

uint8_t fixedHeaderType(PacketType type) {
    switch (type) {
    case PacketType::Puback:  return MQTTPUBACK;
    case PacketType::Pubrec:  return MQTTPUBREC;
    case PacketType::Pubrel:  return MQTTPUBREL;
    case PacketType::Pubcomp: return MQTTPUBCOMP;
    }
    return 0;
}

MqttPacket buildAck(PacketType type, uint16_t packetId, uint8_t reasonCode, Form form) {
    const uint8_t packetType = fixedHeaderType(type);
    switch (form) {
    case Form::Short:
        return MqttPacket::ack5(packetType, packetId, -1);
    case Form::Mid:
        return MqttPacket::ack5(packetType, packetId, static_cast<int>(reasonCode));
    case Form::Long:
        return MqttPacket::ack5(packetType, packetId, static_cast<int>(reasonCode),
                                MqttProps().str(0x1F, "form-equivalence"));
    }
    return MqttPacket::ack5(packetType, packetId, -1);
}

void checkEmission(PacketType type, uint16_t packetId, uint8_t incomingReasonCode,
                   const std::vector<uint8_t>& outbound) {
    if ((type == PacketType::Pubrec) && (incomingReasonCode < 0x80)) {
        REQUIRE(outbound.size() == 4);
        CHECK(outbound[0] == static_cast<uint8_t>(MQTTPUBREL | 0x02));
        const DecodedAck5 ack = MqttParser::decodeAck5(outbound, MQTTPUBREL);
        REQUIRE(ack.valid);
        CHECK(ack.form == 0);
        CHECK(ack.packetId == packetId);
        CHECK(ack.reasonCode < 0x80);
    } else if (type == PacketType::Pubrel) {
        REQUIRE(outbound.size() == 4);
        const DecodedAck5 ack = MqttParser::decodeAck5(outbound, MQTTPUBCOMP);
        REQUIRE(ack.valid);
        CHECK(ack.form == 0);
        CHECK(ack.packetId == packetId);
        CHECK(ack.reasonCode < 0x80);
    } else {
        CHECK(outbound.empty());
    }
}

Outcome run(PacketType type, uint8_t reasonCode, Form form) {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_qos::connect(client, psc);
    psc.setKeepAlive(0);

    uint16_t packetId = 0;
    switch (type) {
    case PacketType::Puback:
        packetId = mqtt5_qos::publishQos1AndReadId(client, psc, "tele/p11/STATE");
        break;
    case PacketType::Pubrec:
        packetId = mqtt5_qos::publishQos2AndReadId(client, psc, "tele/p11/STATE");
        break;
    case PacketType::Pubrel:
        packetId = 0x1234;
        client.clearOutbound();
        client.pushPacket(MqttPacket::publish5("tele/p11/inbound", {'q'},
                                               /*qos=*/2, /*retained=*/false,
                                               /*dup=*/false, packetId));
        REQUIRE(psc.loop());
        {
            const DecodedAck5 pubrec = MqttParser::decodeAck5(client.outbound(), MQTTPUBREC);
            REQUIRE(pubrec.valid);
            CHECK(pubrec.packetId == packetId);
        }
        break;
    case PacketType::Pubcomp:
        packetId = mqtt5_qos::publishQos2AndReadId(client, psc, "tele/p11/STATE");
        client.clearOutbound();
        client.pushPacket(MqttPacket::ack5(MQTTPUBREC, packetId, -1));
        REQUIRE(psc.loop());
        {
            const DecodedAck5 pubrel = MqttParser::decodeAck5(client.outbound(), MQTTPUBREL);
            REQUIRE(pubrel.valid);
            CHECK(pubrel.packetId == packetId);
        }
        break;
    }

    client.clearOutbound();
    client.pushPacket(buildAck(type, packetId, reasonCode, form));
    REQUIRE(psc.loop());

    Outcome outcome;
    outcome.inFlight = psc.inFlightCount();
    outcome.lastPacketId = psc.lastPacketId();
    outcome.lastReasonCode = psc.lastReasonCode();
    outcome.lastError = psc.lastError();
    outcome.connected = psc.connected();
    outcome.outbound = client.outbound();
    checkEmission(type, packetId, reasonCode, outcome.outbound);
    return outcome;
}

void checkEquivalent(const Outcome& actual, const Outcome& expected) {
    CHECK(actual.inFlight == expected.inFlight);
    CHECK(actual.lastPacketId == expected.lastPacketId);
    CHECK(actual.lastReasonCode == expected.lastReasonCode);
    CHECK(actual.lastError == expected.lastError);
    CHECK(actual.connected == expected.connected);
    CHECK(actual.outbound == expected.outbound);
}

}  // namespace mqtt5_p11

// Feature: tasmota-pubsub-mqtt5, Property 11: For all PUBACK, PUBREC, PUBREL and
// PUBCOMP packets, and for all three encodings (Packet Identifier only; identifier plus
// Reason Code; identifier plus Reason Code plus Properties), parsing advances the delivery
// state machine identically for a given Reason Code, with an absent Reason Code interpreted
// as 0x00; and every acknowledgement this client emits carries the received Packet Identifier
// with a Reason Code below 0x80, with PUBREL always carrying fixed-header flags 0x02.
//
// Property 11: Acknowledgement short and long forms are equivalent
TEST_CASE("Property 11: Acknowledgement short and long forms are equivalent") {
    using mqtt5_p11::Form;
    using mqtt5_p11::Outcome;
    using mqtt5_p11::PacketType;

    const PacketType packetTypes[] = {
        PacketType::Puback, PacketType::Pubrec, PacketType::Pubrel, PacketType::Pubcomp,
    };
    // 0x00 is the only Reason Code shared by all four acknowledgement types;
    // packet-specific nonzero allowlists are validated separately.
    const uint8_t reasonCodes[] = {0x00};

    for (const PacketType packetType : packetTypes) {
        CAPTURE(mqtt5_p11::packetTypeName(packetType));

        // The short form is the only encoding with an absent Reason Code, so it means
        // 0x00. It must produce exactly the same observable state and outbound response as
        // both explicit-zero forms.
        const Outcome absent = mqtt5_p11::run(packetType, 0x00, Form::Short);
        const Outcome midZero = mqtt5_p11::run(packetType, 0x00, Form::Mid);
        const Outcome longZero = mqtt5_p11::run(packetType, 0x00, Form::Long);
        mqtt5_p11::checkEquivalent(midZero, absent);
        mqtt5_p11::checkEquivalent(longZero, absent);

        // A nonzero Reason Code cannot fit the short form. The mid and long forms can
        // represent every byte value, and must advance the state machine identically.
        for (const uint8_t reasonCode : reasonCodes) {
            CAPTURE(reasonCode);
            const Outcome mid = mqtt5_p11::run(packetType, reasonCode, Form::Mid);
            const Outcome longForm = mqtt5_p11::run(packetType, reasonCode, Form::Long);
            mqtt5_p11::checkEquivalent(longForm, mid);
        }
    }
}

// ===========================================================================
//  Task 11.9 - Property 16: Retransmission differs only by the DUP flag, and
//  happens only on a Session Present 1 reconnect
// ===========================================================================

namespace mqtt5_p16 {

// Fixed backing storage for the property sets below. MqttMessageProperties borrows
// every pointer for the duration of the ONE publish() call that consumes it, and these
// arrays outlive the whole test binary, which is more than that requires.
static const uint8_t kCorrelation[4] = {0xDE, 0xAD, 0xBE, 0xEF};
static const char* const kUserKeys[5]   = {"k1", "k2", "k1", "k3", "k4"};
static const char* const kUserValues[5] = {"v1", "v2", "v1b", "v3", "v4"};

// Which message property set a curated vector below attaches to its publish.
enum class PropKind {
    None,
    PayloadFormatAndContentType,
    ResponseAndCorrelation,
    FiveUserProperties,
    AllCombined
};

void fillProps(PropKind kind, MqttMessageProperties& props) {
    switch (kind) {
    case PropKind::None:
        break;
    case PropKind::PayloadFormatAndContentType:
        props.hasPayloadFormat = true;
        props.payloadFormat    = 1;
        props.contentType      = "application/json";
        break;
    case PropKind::ResponseAndCorrelation:
        // Supply both independently optional properties here so the combined property set
        // exercises Binary Data (0x09) alongside Response Topic (0x08).
        props.responseTopic      = "reply/to";
        props.correlationData    = kCorrelation;
        props.correlationDataLen = 4;
        break;
    case PropKind::FiveUserProperties:
        props.userPropertyKeys   = kUserKeys;
        props.userPropertyValues = kUserValues;
        props.userPropertyCount  = 5;
        break;
    case PropKind::AllCombined:
        props.hasPayloadFormat   = true;
        props.payloadFormat      = 0;
        props.contentType        = "text/plain";
        props.responseTopic      = "reply/to";
        props.correlationData    = kCorrelation;
        props.correlationDataLen = 4;
        props.userPropertyKeys   = kUserKeys;
        props.userPropertyValues = kUserValues;
        props.userPropertyCount  = 5;
        break;
    }
}

// One publish vector for the byte-identity sweep: topic, payload, retain flag, QoS and
// which message property set to attach - the "for all topic names, message property
// sets, payloads and QoS values in {1, 2}" the property quantifies over.
struct Vector {
    const char* name;
    const char* topic;
    std::string payload;
    bool        retained;
    uint8_t     qos;
    PropKind    propKind;
};

std::vector<Vector> curatedVectors() {
    return {
        {"QoS 1, no properties, empty payload",
         "tele/a/STATE", "", false, 1, PropKind::None},
        {"QoS 1, retained, short payload, payload format + content type",
         "stat/b/RESULT", "{\"P\":1}", true, 1, PropKind::PayloadFormatAndContentType},
        {"QoS 2, response topic paired with correlation data",
         "cmnd/c/BACKLOG", "go", false, 2, PropKind::ResponseAndCorrelation},
        {"QoS 2, retained, five user properties with a duplicate key",
         "tele/d/SENSOR", "23.5", true, 2, PropKind::FiveUserProperties},
        {"QoS 1, long topic and payload, every property kind combined",
         "tele/very/long/topic/segment/for/coverage/STATE",
         std::string(200, 'x'), false, 1, PropKind::AllCombined},
    };
}

// Fill a QoS-capable publish request from one curated vector. `req.properties` borrows
// from the fixed storage above, so the request is only ever valid for the one publish()
// call it is built for.
void fillRequest(const Vector& v, MqttPublishRequest& req) {
    req.topic    = v.topic;
    req.payload  = reinterpret_cast<const uint8_t*>(v.payload.data());
    req.plength  = static_cast<unsigned int>(v.payload.size());
    req.retained = v.retained;
    req.qos      = v.qos;
    fillProps(v.propKind, req.properties);
}

// A connected client whose CONNACK reports the given Session Present bit - default
// false, which is all a first connection ever needs.
void connect(MockClient& client, PubSubClient& psc, bool sessionPresent = false) {
    client.pushPacket(MqttPacket::connack(0, sessionPresent));
    psc.setServer("broker.example", 1883);
    REQUIRE(psc.connect("mqtt5-p16", nullptr, nullptr, nullptr, 0, false, nullptr,
                        /*cleanSession=*/false));
    REQUIRE(psc.connected());
    client.clearOutbound();
}

// Drop the connection the way a network failure would, leaving whatever the library
// decided to carry over in place for the next connect() to resolve.
void drop(MockClient& client, PubSubClient& psc) {
    psc.disconnect();
    REQUIRE_FALSE(psc.connected());
    client.clearOutbound();
    client.clearInbound();
}

// Reconnect and return only the bytes written AFTER the CONNECT/CONNACK exchange - i.e.
// any retransmission and nothing else.
std::vector<uint8_t> reconnect(MockClient& client, PubSubClient& psc, bool sessionPresent) {
    client.pushPacket(MqttPacket::connack(0, sessionPresent));
    client.clearOutbound();
    REQUIRE(psc.connect("mqtt5-p16", nullptr, nullptr, nullptr, 0, false, nullptr,
                        /*cleanSession=*/false));
    REQUIRE(psc.connected());
    CHECK(psc.sessionPresent() == sessionPresent);
    const std::vector<uint8_t>& out = client.outbound();
    REQUIRE(out.size() >= 2);
    REQUIRE((out[0] & 0xF0) == MQTTCONNECT);
    size_t pos = 1;
    uint32_t remaining = 0;
    uint32_t multiplier = 1;
    uint8_t encoded = 0;
    do {
        REQUIRE(pos < out.size());
        encoded = out[pos++];
        remaining += static_cast<uint32_t>(encoded & 0x7F) * multiplier;
        multiplier *= 128;
    } while ((encoded & 0x80) != 0);
    pos += remaining;
    REQUIRE(pos <= out.size());
    return std::vector<uint8_t>(out.begin() + static_cast<long>(pos), out.end());
}

// Split a byte stream into whole MQTT packets, so a run of retransmissions can be
// counted and inspected one by one.
std::vector<std::vector<uint8_t>> splitPackets(const std::vector<uint8_t>& bytes) {
    std::vector<std::vector<uint8_t>> packets;
    size_t pos = 0;
    while (pos < bytes.size()) {
        const size_t start = pos;
        pos++;
        uint32_t remaining = 0;
        uint32_t multiplier = 1;
        uint8_t encoded = 0;
        do {
            REQUIRE(pos < bytes.size());
            encoded = bytes[pos++];
            remaining += static_cast<uint32_t>(encoded & 0x7F) * multiplier;
            multiplier *= 128;
        } while ((encoded & 0x80) != 0);
        pos += remaining;
        REQUIRE(pos <= bytes.size());
        packets.push_back(std::vector<uint8_t>(bytes.begin() + static_cast<long>(start),
                                               bytes.begin() + static_cast<long>(pos)));
    }
    return packets;
}

// Return the byte index immediately after the CONNECT frame in `bytes`. Reconnect tests
// use it to isolate the retransmission suffix, including a deliberately partial final packet.
size_t connectPacketEnd(const std::vector<uint8_t>& bytes) {
    REQUIRE(bytes.size() >= 2);
    REQUIRE((bytes[0] & 0xF0) == MQTTCONNECT);
    size_t pos = 1;
    uint32_t remaining = 0;
    uint32_t multiplier = 1;
    uint8_t encoded = 0;
    do {
        REQUIRE(pos < bytes.size());
        encoded = bytes[pos++];
        remaining += static_cast<uint32_t>(encoded & 0x7F) * multiplier;
        multiplier *= 128;
    } while ((encoded & 0x80) != 0);
    pos += remaining;
    REQUIRE(pos <= bytes.size());
    return pos;
}

}  // namespace mqtt5_p16

// Feature: tasmota-pubsub-mqtt5, Property 16: For all buffered QoS 1/2 retained entries,
// retransmission occurs only immediately after a matching persistent Session Present 1
// reconnect. Every replay differs only by DUP (or is a PUBREL for AwaitingPubcomp), is
// rechecked against the newly negotiated Maximum Packet Size and current buffer, and stops at
// the first framing or transport failure. A failed replay makes connect() return false, records
// its Packet Identifier and specific error, emits no later replay, and never reaches a changed
// session identity.
//
// Validates: Requirements 5.2, 5.3, 8.7, 8.19, 8.22, 8.23, 13.1, 13.2, 14.2
TEST_CASE("Property 16: Retransmission differs only by the DUP flag, and happens only on a Session Present 1 reconnect") {

    SUBCASE("a retained PUBLISH resends byte-identical bytes but for the DUP flag, across "
            "topics, payloads, retain flags, QoS and property sets") {
        for (const mqtt5_p16::Vector& v : mqtt5_p16::curatedVectors()) {
            CAPTURE(v.name);

            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            mqtt5_p16::connect(client, psc);

            MqttPublishRequest req;
            mqtt5_p16::fillRequest(v, req);
            REQUIRE(psc.publish(req));
            const std::vector<uint8_t> first = client.outbound();
            REQUIRE(psc.inFlightCount() == 1);

            // The connection drops with the PUBLISH unacknowledged. The entry SURVIVES,
            // because it holds a retained copy: the only permitted retransmission comes
            // after the next CONNACK (Requirement 8.22).
            mqtt5_p16::drop(client, psc);
            CHECK(psc.inFlightCount() == 1);

            const std::vector<uint8_t> resent = mqtt5_p16::reconnect(client, psc, true);

            // Requirement 8.7, both halves. Byte 0 differs by exactly bit 3; every other
            // byte - including the Packet Identifier, whichever property set was
            // supplied, and the payload - is the original's, unchanged.
            REQUIRE(resent.size() == first.size());
            CHECK(resent[0] == static_cast<uint8_t>(first[0] | 0x08));
            CHECK((resent[0] & ~0x08) == (first[0] & ~0x08));
            for (size_t i = 1; i < first.size(); i++) {
                CHECK(resent[i] == first[i]);
            }

            // Still unacknowledged, so still in flight and still holding its identifier -
            // this is a resend, not a completion.
            CHECK(psc.inFlightCount() == 1);
        }
    }

    SUBCASE("nothing is retransmitted while the connection is up, across keepalive cycles "
            "and an unrelated acknowledgement arrival") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        // A short interval, so several idle cycles fit in the case while the two publishes
        // below stay unanswered throughout.
        psc.setKeepAlive(2);
        mqtt5_p16::connect(client, psc);

        const std::vector<mqtt5_p16::Vector> vectors = mqtt5_p16::curatedVectors();

        MqttPublishRequest q1;
        mqtt5_p16::fillRequest(vectors[1], q1);          // QoS 1, retained
        REQUIRE(psc.publish(q1));

        client.clearOutbound();
        MqttPublishRequest q2;
        mqtt5_p16::fillRequest(vectors[2], q2);          // QoS 2
        REQUIRE(psc.publish(q2));
        REQUIRE(psc.inFlightCount() == 2);
        client.clearOutbound();

        // An acknowledgement arrival for an UNRELATED operation, while both publishes
        // above sit unanswered: it must correlate to its own identifier only, and must
        // never be mistaken for a reason to resend the other two.
        REQUIRE(psc.subscribe("cmnd/e/#"));
        const uint16_t subId = mqtt5_qos::emittedPacketId(client.outbound());
        client.clearOutbound();
        client.pushPacket(MqttPacket::suback5(subId, {0x00}));
        REQUIRE(psc.loop());
        CHECK(client.outbound().empty());
        CHECK(psc.inFlightCount() == 2);     // only the subscription's entry was released

        // Two idle keepalive cycles pass with the connection up and neither publish
        // acknowledged. Every byte the library writes during them must be a PINGREQ -
        // never a PUBLISH.
        for (int cycle = 0; cycle < 2; cycle++) {
            client.clearOutbound();
            TestClock::instance().advance(2500UL);
            REQUIRE(psc.loop());
            REQUIRE(client.outbound().size() >= 2);
            const DecodedPacket d = MqttParser::decode(client.outbound());
            CHECK(d.valid);
            CHECK(d.type == static_cast<uint8_t>(MQTTPINGREQ));
            client.clearOutbound();
            client.pushPacket(MqttPacket::pingresp());
            REQUIRE(psc.loop());
            CHECK(client.outbound().empty());
        }

        // Past the acknowledgement deadline with no PUBACK/PUBREC in sight for either
        // message. A live-connection timeout must not retransmit, but it also must not free
        // either identifier: both exchanges remain available for their late acknowledgements.
        client.clearOutbound();
        TestClock::instance().advance((static_cast<unsigned long>(MQTT_ACK_TIMEOUT) + 1UL) *
                                     1000UL);
        REQUIRE(psc.loop());
        CHECK(psc.inFlightCount() == 2);
        // Whatever this loop() call wrote is at most a PINGREQ - never a PUBLISH byte for
        // either abandoned entry.
        if (!client.outbound().empty()) {
            const DecodedPacket d = MqttParser::decode(client.outbound());
            CHECK(d.valid);
            CHECK(d.type == static_cast<uint8_t>(MQTTPINGREQ));
        }
    }

    SUBCASE("Session Present 0 abandons every surviving entry instead of resending") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        psc.setKeepAlive(0);
        mqtt5_p16::connect(client, psc);

        const std::vector<mqtt5_p16::Vector> vectors = mqtt5_p16::curatedVectors();

        MqttPublishRequest q1;
        mqtt5_p16::fillRequest(vectors[1], q1);          // QoS 1, retained
        REQUIRE(psc.publish(q1));
        REQUIRE(MqttParser::decodePublish5(client.outbound()).valid);

        client.clearOutbound();
        MqttPublishRequest q2;
        mqtt5_p16::fillRequest(vectors[3], q2);          // QoS 2, retained, user properties
        REQUIRE(psc.publish(q2));
        REQUIRE(MqttParser::decodePublish5(client.outbound()).valid);

        REQUIRE(psc.inFlightCount() == 2);

        mqtt5_p16::drop(client, psc);
        CHECK(psc.inFlightCount() == 2);   // both survived the drop, awaiting the CONNACK

        // Nothing is resent, both identifiers are released, and the abandonment is
        // reported through the same two accessors every other abandonment uses.
        const std::vector<uint8_t> after = mqtt5_p16::reconnect(client, psc, false);
        CHECK(after.empty());
        CHECK(psc.inFlightCount() == 0);
        CHECK(psc.lastReasonCode() == PubSubClient::MQTT_RC_UNSPECIFIED_ERROR);
    }

    SUBCASE("an entry awaiting PUBCOMP resends exactly one PUBREL") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        psc.setKeepAlive(0);
        mqtt5_p16::connect(client, psc);

        // Every buffered QoS 2 publish is replayable at admission. Once PUBREC arrives,
        // its retained PUBLISH body is released because a later reconnect needs only PUBREL.
        const std::string payload = "y";

        MqttPublishRequest req;
        req.topic   = "tele/pubcomp/STATE";
        req.payload = reinterpret_cast<const uint8_t*>(payload.data());
        req.plength = static_cast<unsigned int>(payload.size());
        req.qos     = 2;
        REQUIRE(psc.publish(req));
        const DecodedPublish5 sent = MqttParser::decodePublish5(client.outbound());
        REQUIRE(sent.valid);
        const uint16_t id = sent.msgId;
        REQUIRE(id != 0);

        // PUBREC in, exactly one PUBREL out: the entry now awaits its PUBCOMP, which is
        // the state whose resend is a fresh PUBREL rather than a PUBLISH replay - and
        // whose retained body, if any, is released right here regardless of `pastBudget`
        // (Requirement 8.23's premise: the PUBLISH is done once the broker sent PUBREC).
        client.clearOutbound();
        client.pushPacket(MqttPacket::ack5(MQTTPUBREC, id, -1));
        REQUIRE(psc.loop());
        const std::vector<uint8_t> firstPubrel = client.outbound();
        REQUIRE(firstPubrel.size() == 4);
        CHECK(psc.inFlightCount() == 1);

        // The connection drops with the PUBCOMP still missing. The entry SURVIVES either
        // way: resendsOnSessionResume() accepts AwaitingPubcomp unconditionally.
        mqtt5_p16::drop(client, psc);
        CHECK(psc.inFlightCount() == 1);

        // Session Present 1: exactly one PUBREL, carrying the original Packet Identifier
        // with the mandatory fixed-header flags 0x02, and NO PUBLISH replay.
        const std::vector<uint8_t> resent = mqtt5_p16::reconnect(client, psc, true);
        CHECK(resent == firstPubrel);
        const std::vector<std::vector<uint8_t>> packets = mqtt5_p16::splitPackets(resent);
        REQUIRE(packets.size() == 1);
        CHECK((packets[0][0] & 0xF0) == static_cast<uint8_t>(MQTTPUBREL));
        const DecodedAck5 rel = MqttParser::decodeAck5(packets[0], MQTTPUBREL);
        REQUIRE(rel.valid);
        CHECK(rel.packetId == id);

        // Still awaiting the PUBCOMP, which still completes it on the new connection.
        CHECK(psc.inFlightCount() == 1);
        client.clearOutbound();
        client.pushPacket(MqttPacket::ack5(MQTTPUBCOMP, id, -1));
        REQUIRE(psc.loop());
        CHECK(psc.inFlightCount() == 0);
        CHECK(psc.lastPacketId() == id);
        CHECK(client.outbound().empty());
    }

    SUBCASE("replay write failures stop at the first PUBLISH and preserve its state") {
        for (size_t failureIndex = 0; failureIndex < 3; ++failureIndex) {
            CAPTURE(failureIndex);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            psc.setKeepAlive(0);
            mqtt5_p16::connect(client, psc);

            std::vector<std::vector<uint8_t>> initial;
            std::vector<uint16_t> ids;
            for (size_t i = 0; i < 3; ++i) {
                const std::string topic = "tele/replay/" + std::to_string(i);
                const std::string payload = "body-" + std::to_string(i);
                MqttPublishRequest req;
                req.topic = topic.c_str();
                req.payload = reinterpret_cast<const uint8_t*>(payload.data());
                req.plength = static_cast<unsigned int>(payload.size());
                req.qos = 1;
                REQUIRE(psc.publish(req));
                initial.push_back(client.outbound());
                const DecodedPublish5 sent = MqttParser::decodePublish5(initial.back());
                REQUIRE(sent.valid);
                ids.push_back(sent.msgId);
                client.clearOutbound();
            }
            REQUIRE(psc.inFlightCount() == 3);
            mqtt5_p16::drop(client, psc);

            client.pushPacket(MqttPacket::connack5(0x00, true));
            client.clearOutbound();
// CONNECT is the first write of this attempt; let it and the selected number of earlier
// replays complete, then accept only the first byte of the failing replay.
            client.setWriteLimitAfter(1 + failureIndex, 1);
            CHECK_FALSE(psc.connect("mqtt5-p16", nullptr, nullptr, nullptr, 0, false,
                                    nullptr, /*cleanSession=*/false));
            CHECK_FALSE(psc.connected());
            CHECK(psc.lastError() == MqttError::WriteFailed);
            CHECK(psc.lastPacketId() == ids[failureIndex]);
            CHECK(psc.inFlightCount() == 3);

            const std::vector<uint8_t>& out = client.outbound();
            const size_t replayStart = mqtt5_p16::connectPacketEnd(out);
            std::vector<uint8_t> expected;
            for (size_t i = 0; i < failureIndex; ++i) {
                std::vector<uint8_t> replay = initial[i];
                replay[0] |= 0x08;
                expected.insert(expected.end(), replay.begin(), replay.end());
            }
            expected.push_back(static_cast<uint8_t>(initial[failureIndex][0] | 0x08));
            CHECK(std::vector<uint8_t>(out.begin() + static_cast<long>(replayStart), out.end())
                  == expected);
        }
    }

    SUBCASE("a PUBREL replay short write fails the attempt without a later packet") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        psc.setKeepAlive(0);
        mqtt5_p16::connect(client, psc);

        const std::string payload = "pubrel-replay";
        MqttPublishRequest req;
        req.topic = "tele/replay/pubrel";
        req.payload = reinterpret_cast<const uint8_t*>(payload.data());
        req.plength = static_cast<unsigned int>(payload.size());
        req.qos = 2;
        REQUIRE(psc.publish(req));
        const uint16_t id = MqttParser::decodePublish5(client.outbound()).msgId;
        client.clearOutbound();
        client.pushPacket(MqttPacket::ack5(MQTTPUBREC, id, -1));
        REQUIRE(psc.loop());
        client.clearOutbound();
        mqtt5_p16::drop(client, psc);

        client.pushPacket(MqttPacket::connack5(0x00, true));
        client.clearOutbound();
        client.setWriteLimitAfter(1, 1);
        CHECK_FALSE(psc.connect("mqtt5-p16", nullptr, nullptr, nullptr, 0, false,
                                nullptr, /*cleanSession=*/false));
        CHECK_FALSE(psc.connected());
        CHECK(psc.lastError() == MqttError::WriteFailed);
        CHECK(psc.lastPacketId() == id);
        const std::vector<uint8_t>& out = client.outbound();
        const size_t replayStart = mqtt5_p16::connectPacketEnd(out);
        CHECK(std::vector<uint8_t>(out.begin() + static_cast<long>(replayStart), out.end())
              == std::vector<uint8_t>{static_cast<uint8_t>(MQTTPUBREL | MQTTQOS1)});
    }

    SUBCASE("replay rechecks limits, current buffer, and session identity") {
        // A newly negotiated small Maximum Packet Size prevents the retained PUBLISH from
        // reaching the transport and makes the full connection attempt fail.
        {
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            psc.setKeepAlive(0);
            mqtt5_p16::connect(client, psc);
            const std::string payload = "limited";
            MqttPublishRequest req;
            req.topic = "tele/replay/limit";
            req.payload = reinterpret_cast<const uint8_t*>(payload.data());
            req.plength = static_cast<unsigned int>(payload.size());
            req.qos = 1;
            REQUIRE(psc.publish(req));
            const uint16_t id = MqttParser::decodePublish5(client.outbound()).msgId;
            mqtt5_p16::drop(client, psc);
            client.pushPacket(MqttPacket::connack5(0x00, true, MqttProps().u32(0x27, 4)));
            client.clearOutbound();
            CHECK_FALSE(psc.connect("mqtt5-p16", nullptr, nullptr, nullptr, 0, false,
                                    nullptr, /*cleanSession=*/false));
            CHECK(psc.lastError() == MqttError::PacketTooLarge);
            CHECK(psc.lastPacketId() == id);
            const size_t replayStart = mqtt5_p16::connectPacketEnd(client.outbound());
            CHECK(replayStart == client.outbound().size());
        }

        // A buffer that remains large enough for persistent CONNECT but not the retained
        // body is also a connection-attempt failure, rather than an abandoned replay.
        {
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            psc.setKeepAlive(0);
            mqtt5_p16::connect(client, psc);
            const std::string payload(80, 'x');
            MqttPublishRequest req;
            req.topic = "tele/replay/buffer";
            req.payload = reinterpret_cast<const uint8_t*>(payload.data());
            req.plength = static_cast<unsigned int>(payload.size());
            req.qos = 1;
            REQUIRE(psc.publish(req));
            const uint16_t id = MqttParser::decodePublish5(client.outbound()).msgId;
            mqtt5_p16::drop(client, psc);
            REQUIRE(psc.setBufferSize(45));
            client.pushPacket(MqttPacket::connack5(0x00, true));
            client.clearOutbound();
            CHECK_FALSE(psc.connect("mqtt5-p16", nullptr, nullptr, nullptr, 0, false,
                                    nullptr, /*cleanSession=*/false));
            CHECK(psc.lastError() == MqttError::BufferTooSmall);
            CHECK(psc.lastPacketId() == id);
            const size_t replayStart = mqtt5_p16::connectPacketEnd(client.outbound());
            CHECK(replayStart == client.outbound().size());
        }

        // Changing endpoint identity clears retained state before CONNECT, so even a fresh
        // peer that reports no session receives no replay from the old broker.
        {
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            psc.setKeepAlive(0);
            mqtt5_p16::connect(client, psc);
            const std::string payload = "identity";
            MqttPublishRequest req;
            req.topic = "tele/replay/identity";
            req.payload = reinterpret_cast<const uint8_t*>(payload.data());
            req.plength = static_cast<unsigned int>(payload.size());
            req.qos = 1;
            REQUIRE(psc.publish(req));
            mqtt5_p16::drop(client, psc);
            psc.setServer("other-broker.example", 1884);
            client.pushPacket(MqttPacket::connack5(0x00, false));
            client.clearOutbound();
            REQUIRE(psc.connect("mqtt5-p16", nullptr, nullptr, nullptr, 0, false,
                                nullptr, /*cleanSession=*/false));
            const size_t replayStart = mqtt5_p16::connectPacketEnd(client.outbound());
            CHECK(replayStart == client.outbound().size());
            CHECK(psc.inFlightCount() == 0);
        }

        // A reduced but sufficient maximum permits every retained entry to replay; the
        // limit is checked per entry rather than disabling replay wholesale.
        {
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            psc.setKeepAlive(0);
            mqtt5_p16::connect(client, psc);
            for (size_t i = 0; i < 2; ++i) {
                const std::string topic = "tele/replay/small/" + std::to_string(i);
                const std::string payload = "ok";
                MqttPublishRequest req;
                req.topic = topic.c_str();
                req.payload = reinterpret_cast<const uint8_t*>(payload.data());
                req.plength = static_cast<unsigned int>(payload.size());
                req.qos = 1;
                REQUIRE(psc.publish(req));
                client.clearOutbound();
            }
            mqtt5_p16::drop(client, psc);
            client.pushPacket(MqttPacket::connack5(0x00, true, MqttProps().u32(0x27, 64)));
            client.clearOutbound();
            REQUIRE(psc.connect("mqtt5-p16", nullptr, nullptr, nullptr, 0, false,
                                nullptr, /*cleanSession=*/false));
            const size_t replayStart = mqtt5_p16::connectPacketEnd(client.outbound());
            const std::vector<uint8_t> replay(client.outbound().begin() +
                                               static_cast<long>(replayStart),
                                               client.outbound().end());
            CHECK(mqtt5_p16::splitPackets(replay).size() == 2);
        }
    }
}

// ===========================================================================
//  Task 18.8 - Property 31: In-flight retention is bounded and atomic
// ===========================================================================

namespace mqtt5_p31 {

// A connected client with a cleared outbound record and keepalive off, so no
// PINGREQ can appear among the bytes a step decodes.
void connect(MockClient& client, PubSubClient& psc) {
    client.pushPacket(MqttPacket::connack(0));
    psc.setServer("broker.example", 1883);
    REQUIRE(psc.connect("mqtt5-p31", nullptr, nullptr, nullptr, 0, false, nullptr,
                        /*cleanSession=*/false));
    REQUIRE(psc.connected());
    psc.setKeepAlive(0);
    client.clearOutbound();
}

// Drop the connection the way a network failure would.
void drop(MockClient& client, PubSubClient& psc) {
    psc.disconnect();
    REQUIRE_FALSE(psc.connected());
    client.clearOutbound();
    client.clearInbound();
}

// Reconnect and return only the bytes written AFTER the CONNECT/CONNACK
// exchange - i.e. any retransmission and nothing else.
std::vector<uint8_t> reconnect(MockClient& client, PubSubClient& psc, bool sessionPresent) {
    client.pushPacket(MqttPacket::connack(0, sessionPresent));
    client.clearOutbound();
    REQUIRE(psc.connect("mqtt5-p31", nullptr, nullptr, nullptr, 0, false, nullptr,
                        /*cleanSession=*/false));
    REQUIRE(psc.connected());
    CHECK(psc.sessionPresent() == sessionPresent);
    const std::vector<uint8_t>& out = client.outbound();
    REQUIRE(out.size() >= 2);
    REQUIRE((out[0] & 0xF0) == MQTTCONNECT);
    size_t pos = 1;
    uint32_t remaining = 0;
    uint32_t multiplier = 1;
    uint8_t encoded = 0;
    do {
        REQUIRE(pos < out.size());
        encoded = out[pos++];
        remaining += static_cast<uint32_t>(encoded & 0x7F) * multiplier;
        multiplier *= 128;
    } while ((encoded & 0x80) != 0);
    pos += remaining;
    REQUIRE(pos <= out.size());
    return std::vector<uint8_t>(out.begin() + static_cast<long>(pos), out.end());
}

// A QoS 1 or QoS 2 buffered publish whose assembled body is exactly
// `bodyLen` bytes: the topic is fixed at 1 byte so the overhead
// (2 + topicLen + 2 (Packet Identifier) + 1 (empty Properties)) is a known
// constant and the payload absorbs the rest. Returns the Packet Identifier
// the emitted PUBLISH carries.
uint16_t publishOfBodyLen(MockClient& client, PubSubClient& psc, uint8_t qos,
                          size_t bodyLen) {
    static const char* const topic = "t";   // topicLen == 1
    const size_t overhead = 2 + 1 + 2 + 1;  // topic prefix+byte, PacketId, empty Properties
    REQUIRE(bodyLen >= overhead);
    const size_t payloadLen = bodyLen - overhead;
    const std::vector<uint8_t> payload(payloadLen, 'z');

    client.clearOutbound();
    REQUIRE(psc.publish(topic, payload.data(), static_cast<unsigned int>(payload.size()),
                        false, qos));
    const DecodedPublish5 p = MqttParser::decodePublish5(client.outbound());
    REQUIRE(p.valid);
    REQUIRE(p.qos == qos);
    REQUIRE(p.msgId != 0);
    return p.msgId;
}

}  // namespace mqtt5_p31

// Feature: tasmota-pubsub-mqtt5, Property 31: For all buffered QoS 1/2 publish
// attempts, the sum of held retained bodies never exceeds Inflight_Retention_Budget; a
// body that cannot be retained because the budget is exhausted is refused before its first
// transport write with no held slot or identifier, and succeeds after capacity is restored.
// Retained success-path publishes replay with only DUP changed and replay/PUBREL resend take
// no new allocation. The QoS 0 streaming path creates no outstanding state.
//
// Validates: Requirements 8.19-8.24, 13.11
TEST_CASE("Property 31: In-flight retention is bounded and admission is atomic") {
    using namespace mqtt5_p31;

    SUBCASE("assembled body sizes 1, 512, 2047, 2048 and 2049 against the 2048-byte budget: retained when admitted, refused otherwise") {
        const size_t bodyLens[] = {1, 512, 2047, 2048, 2049};

        for (size_t bodyLen : bodyLens) {
            CAPTURE(bodyLen);
            // bodyLen 1 is below the fixed overhead (6 bytes) for a QoS 1/2
            // publish at topic length 1, so it is reached at a larger buffer
            // relative to the working buffer's own minimum rather than by
            // shrinking the topic further - the property is about the RETAINED
            // total, not about framing a 1-byte body.
            const size_t effectiveBodyLen = (bodyLen < 6) ? 6 : bodyLen;

            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            connect(client, psc);
            // Room for the assembled PUBLISH body plus reserved header space, and
            // wide enough to still frame the CONNECT this case's own reconnect()
            // sends (client id "mqtt5-p31" plus MQTT 5.0 CONNECT properties).
            REQUIRE(psc.setBufferSize(
                static_cast<uint16_t>(MQTT_MAX_HEADER_SIZE + effectiveBodyLen + 64)));

            const bool fitsBudget = effectiveBodyLen <= static_cast<size_t>(MQTT_INFLIGHT_COPY_BUDGET);

            if (fitsBudget) {
                const uint16_t id = publishOfBodyLen(client, psc, 1, effectiveBodyLen);
                CHECK(psc.inFlightCount() == 1);

                // Holds a retained body, so a drop leaves it in place - the only
                // permitted retransmission is after the NEXT CONNACK - and a
                // Session Present 1 reconnect resends it with DUP set.
                drop(client, psc);
                CHECK(psc.inFlightCount() == 1);

                const std::vector<uint8_t> resent = reconnect(client, psc, true);
                const DecodedPublish5 r = MqttParser::decodePublish5(resent);
                REQUIRE(r.valid);
                CHECK(r.dup);
                CHECK(r.msgId == id);
                CHECK(psc.inFlightCount() == 1);
                // Cleanup: acknowledge it so the next iteration starts clean.
                client.pushPacket(MqttPacket::ack5(MQTTPUBACK, id, -1));
                REQUIRE(psc.loop());
            } else {
                // A body larger than the remaining budget is never wire-visible: the
                // retained copy is a precondition for buffered QoS 1/2 transmission.
                client.clearOutbound();
                const char* const topic = "t";
                const size_t overhead = 2 + 1 + 2 + 1;
                const std::vector<uint8_t> payload(effectiveBodyLen - overhead, 'z');
                CHECK_FALSE(psc.publish(topic, payload.data(),
                                        static_cast<unsigned int>(payload.size()), false, 1));
                CHECK(psc.lastError() == MqttError::RetentionUnavailable);
                CHECK(client.outbound().empty());
                CHECK(psc.connected());
                CHECK(psc.inFlightCount() == 0);
            }
            CHECK(psc.inFlightCount() == 0);
        }
    }

    SUBCASE("four sub-512-byte publishes: all fit and all retain") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connect(client, psc);
        REQUIRE(MQTT_MAX_INFLIGHT >= 4);

        std::vector<uint16_t> ids;
        for (int i = 0; i < 4; i++) {
            ids.push_back(publishOfBodyLen(client, psc, 1, 400));
        }
        CHECK(psc.inFlightCount() == 4);
        // 4 * 400 = 1600, comfortably within the 2048-byte default budget.
        REQUIRE(4u * 400u <= static_cast<unsigned>(MQTT_INFLIGHT_COPY_BUDGET));

        drop(client, psc);
        CHECK(psc.inFlightCount() == 4);      // every entry retained a body, all survive

        const std::vector<uint8_t> resent = reconnect(client, psc, true);
        std::vector<std::vector<uint8_t>> packets;
        {
            size_t pos = 0;
            while (pos < resent.size()) {
                const size_t start = pos;
                pos++;
                uint32_t remaining = 0;
                uint32_t multiplier = 1;
                uint8_t encoded = 0;
                do {
                    REQUIRE(pos < resent.size());
                    encoded = resent[pos++];
                    remaining += static_cast<uint32_t>(encoded & 0x7F) * multiplier;
                    multiplier *= 128;
                } while ((encoded & 0x80) != 0);
                pos += remaining;
                REQUIRE(pos <= resent.size());
                packets.push_back(std::vector<uint8_t>(resent.begin() + static_cast<long>(start),
                                                       resent.begin() + static_cast<long>(pos)));
            }
        }
        REQUIRE(packets.size() == 4);
        for (const std::vector<uint8_t>& pkt : packets) {
            const DecodedPublish5 p = MqttParser::decodePublish5(pkt);
            REQUIRE(p.valid);
            CHECK(p.dup);
        }
    }

    SUBCASE("committing the budget refuses a new publish until acknowledgement restores capacity") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connect(client, psc);
        REQUIRE(psc.setBufferSize(static_cast<uint16_t>(MQTT_INFLIGHT_COPY_BUDGET + 512)));

        const uint16_t committed = publishOfBodyLen(client, psc, 1,
                                                     static_cast<size_t>(MQTT_INFLIGHT_COPY_BUDGET));
        CHECK(psc.inFlightCount() == 1);

        client.clearOutbound();
        const uint8_t payload[] = {'x', 'x'};
        CHECK_FALSE(psc.publish("t", payload, sizeof(payload), false, 1));
        CHECK(psc.lastError() == MqttError::RetentionUnavailable);
        CHECK(client.outbound().empty());
        CHECK(psc.connected());
        CHECK(psc.inFlightCount() == 1);

        client.pushPacket(MqttPacket::ack5(MQTTPUBACK, committed, -1));
        REQUIRE(psc.loop());
        client.clearOutbound();
        const uint16_t retried = publishOfBodyLen(client, psc, 1, 8);
        CHECK(retried != committed);
        CHECK(psc.inFlightCount() == 1);
    }

    SUBCASE("the streaming path at QoS 0 creates no Outstanding_Message and "
            "retains nothing") {
        // beginPublish() exposes no QoS argument, so it is QoS 0 by construction
        // (Requirement 8.24's premise): it never claims a table slot at all, and
        // there is therefore nothing for a Session Present 1 reconnect to resend.
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connect(client, psc);

        REQUIRE(psc.beginPublish("tele/p31/FILE", 4, false));
        CHECK(psc.write(reinterpret_cast<const uint8_t*>("data"), 4) == 4);
        CHECK(psc.endPublish() == 1);
        CHECK(psc.inFlightCount() == 0);

        drop(client, psc);
        CHECK(psc.inFlightCount() == 0);
        CHECK(reconnect(client, psc, false).empty());
    }

    SUBCASE("entries in AwaitingPuback, AwaitingPubrec and AwaitingPubcomp, "
            "crossed with Session Present 1 and 0") {
        for (bool sessionPresent : {true, false}) {
            CAPTURE(sessionPresent);

            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            connect(client, psc);

            const uint16_t puback = publishOfBodyLen(client, psc, 1, 64);
            const uint16_t pubrec = publishOfBodyLen(client, psc, 2, 64);

            const uint16_t pubcomp = publishOfBodyLen(client, psc, 2, 64);
            client.clearOutbound();
            client.pushPacket(MqttPacket::ack5(MQTTPUBREC, pubcomp, -1));
            REQUIRE(psc.loop());
            REQUIRE(client.outbound().size() == 4);   // the one PUBREL
            client.clearOutbound();

            CHECK(psc.inFlightCount() == 3);

            drop(client, psc);
            CHECK(psc.inFlightCount() == 3);   // all three survive: two hold bodies,
                                               // one is awaiting PUBCOMP

            const std::vector<uint8_t> resent = reconnect(client, psc, sessionPresent);

            if (sessionPresent) {
                // Two PUBLISH replays (DUP set, original identifiers) and exactly
                // one PUBREL (for the AwaitingPubcomp entry), no PUBLISH for it.
                std::vector<std::vector<uint8_t>> packets;
                size_t pos = 0;
                while (pos < resent.size()) {
                    const size_t start = pos;
                    pos++;
                    uint32_t remaining = 0;
                    uint32_t multiplier = 1;
                    uint8_t encoded = 0;
                    do {
                        REQUIRE(pos < resent.size());
                        encoded = resent[pos++];
                        remaining += static_cast<uint32_t>(encoded & 0x7F) * multiplier;
                        multiplier *= 128;
                    } while ((encoded & 0x80) != 0);
                    pos += remaining;
                    REQUIRE(pos <= resent.size());
                    packets.push_back(std::vector<uint8_t>(resent.begin() + static_cast<long>(start),
                                                           resent.begin() + static_cast<long>(pos)));
                }
                REQUIRE(packets.size() == 3);

                int publishCount = 0;
                int pubrelCount = 0;
                std::set<uint16_t> resentPublishIds;
                for (const std::vector<uint8_t>& pkt : packets) {
                    const uint8_t type = static_cast<uint8_t>(pkt[0] & 0xF0);
                    if (type == static_cast<uint8_t>(MQTTPUBLISH)) {
                        publishCount++;
                        const DecodedPublish5 p = MqttParser::decodePublish5(pkt);
                        REQUIRE(p.valid);
                        CHECK(p.dup);
                        resentPublishIds.insert(p.msgId);
                    } else if (type == static_cast<uint8_t>(MQTTPUBREL)) {
                        pubrelCount++;
                        const DecodedAck5 rel = MqttParser::decodeAck5(pkt, MQTTPUBREL);
                        REQUIRE(rel.valid);
                        CHECK(rel.packetId == pubcomp);
                    }
                }
                CHECK(publishCount == 2);
                CHECK(pubrelCount == 1);
                CHECK(resentPublishIds.count(puback) == 1);
                CHECK(resentPublishIds.count(pubrec) == 1);
                CHECK(psc.inFlightCount() == 3);

                // Clean up: complete every sequence.
                client.pushPacket(MqttPacket::ack5(MQTTPUBACK, puback, -1));
                REQUIRE(psc.loop());
                client.pushPacket(MqttPacket::ack5(MQTTPUBREC, pubrec, -1));
                REQUIRE(psc.loop());
                client.pushPacket(MqttPacket::ack5(MQTTPUBCOMP, pubrec, -1));
                REQUIRE(psc.loop());
                client.pushPacket(MqttPacket::ack5(MQTTPUBCOMP, pubcomp, -1));
                REQUIRE(psc.loop());
                CHECK(psc.inFlightCount() == 0);
            } else {
                // Session Present 0: nothing resent, everything abandoned.
                CHECK(resent.empty());
                CHECK(psc.inFlightCount() == 0);
            }
        }
    }

    SUBCASE("the allocator is called once per buffered publish, never for a "
            "retransmission or a PUBREL resend, and never on the inbound path") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connect(client, psc);

        // The rotor is the only externally observable trace of an allocation:
        // it advances by exactly one step per call. Read it off the wire before
        // and after each phase and count the steps rather than inspecting a
        // private counter (Requirement 15.6's "no test touches a private
        // member").
        auto nextId = [&](uint16_t previous, uint16_t current) -> int {
            if (current > previous) { return static_cast<int>(current - previous); }
            // Wrapped past 65535 back through 1: count the distance the long way.
            return static_cast<int>((65535 - previous) + current);
        };

        const uint16_t idPuback = publishOfBodyLen(client, psc, 1, 64);
        const uint16_t afterPublish1 = idPuback;

        const uint16_t idPubrec = publishOfBodyLen(client, psc, 2, 64);
        // Exactly one allocator step per publish call.
        CHECK(nextId(afterPublish1, idPubrec) == 1);

        // An inbound QoS 1/2 PUBLISH must not touch the outbound allocator at
        // all: deliver one and confirm the rotor position is unaffected by
        // reading it via the next OUTBOUND allocation.
        client.pushPacket(MqttPacket::publish5("cmnd/p31/IN", std::vector<uint8_t>{'y'},
                                               /*qos=*/1, /*retained=*/false, /*dup=*/false,
                                               /*packetId=*/777));
        REQUIRE(psc.loop());   // delivers, sends PUBACK - no outbound allocation
        client.clearOutbound();

        const uint16_t idAfterInbound = publishOfBodyLen(client, psc, 1, 64);
        CHECK(nextId(idPubrec, idAfterInbound) == 1);   // still exactly one step

        CHECK(psc.inFlightCount() == 3);

        // A retransmission and a PUBREL resend must not allocate either: drive
        // one entry to AwaitingPubcomp, drop, and reconnect with Session
        // Present 1 - none of this should move the rotor.
        client.pushPacket(MqttPacket::ack5(MQTTPUBREC, idPubrec, -1));
        REQUIRE(psc.loop());
        client.clearOutbound();

        drop(client, psc);
        reconnect(client, psc, true);
        client.clearOutbound();

        const uint16_t idAfterResend = publishOfBodyLen(client, psc, 1, 64);
        CHECK(nextId(idAfterInbound, idAfterResend) == 1);   // exactly one more step,
                                                             // for THIS publish only

        // Clean up every sequence so the case ends with the table empty.
        client.pushPacket(MqttPacket::ack5(MQTTPUBACK, idPuback, -1));
        REQUIRE(psc.loop());
        client.pushPacket(MqttPacket::ack5(MQTTPUBCOMP, idPubrec, -1));
        REQUIRE(psc.loop());
        client.pushPacket(MqttPacket::ack5(MQTTPUBACK, idAfterInbound, -1));
        REQUIRE(psc.loop());
        client.pushPacket(MqttPacket::ack5(MQTTPUBACK, idAfterResend, -1));
        REQUIRE(psc.loop());
        CHECK(psc.inFlightCount() == 0);
    }

    SUBCASE("accounting returns to its starting point once every entry completes "
            "or is abandoned, with buffer capacity unchanged throughout") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connect(client, psc);
        const uint16_t bufferSizeBefore = psc.getBufferSize();

        std::vector<uint16_t> ids;
        for (int i = 0; i < 3; i++) {
            ids.push_back(publishOfBodyLen(client, psc, 1, 300));
        }
        CHECK(psc.inFlightCount() == 3);
        CHECK(psc.getBufferSize() == bufferSizeBefore);

        for (uint16_t id : ids) {
            client.pushPacket(MqttPacket::ack5(MQTTPUBACK, id, -1));
            REQUIRE(psc.loop());
        }
        CHECK(psc.inFlightCount() == 0);
        CHECK(psc.getBufferSize() == bufferSizeBefore);

        // A fresh publish after a full drain succeeds exactly as the first one
        // did: the retention accounting really did return to its starting point
        // rather than drifting.
        const uint16_t again = publishOfBodyLen(client, psc, 1, 300);
        CHECK(psc.inFlightCount() == 1);
        drop(client, psc);
        const std::vector<uint8_t> resent = reconnect(client, psc, true);
        const DecodedPublish5 r = MqttParser::decodePublish5(resent);
        REQUIRE(r.valid);
        CHECK(r.msgId == again);
        CHECK(psc.getBufferSize() == bufferSizeBefore);
        client.pushPacket(MqttPacket::ack5(MQTTPUBACK, again, -1));
        REQUIRE(psc.loop());
        CHECK(psc.inFlightCount() == 0);
    }
}

#endif  // MQTT_VERSION == MQTT_VERSION_5_0

}  // TEST_SUITE("mqtt5")
