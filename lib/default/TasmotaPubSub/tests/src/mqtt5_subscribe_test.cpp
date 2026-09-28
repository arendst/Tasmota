/*
  mqtt5_subscribe_test.cpp - MQTT 5.0 SUBSCRIBE / UNSUBSCRIBE emission and
  SUBACK / UNSUBACK reporting (tasks 9.2 and 9.3).

  Feature: tasmota-pubsub-mqtt5

  TEST_SUITE("mqtt5"), body guarded by `#if MQTT_VERSION == MQTT_VERSION_5_0`, so the
  file compiles in both selections and contributes cases only to the 5.0 binary.

  Emission (task 9.2):

    SUBSCRIBE:   PacketIdentifier(2) | Properties | ( TopicFilter | SubscriptionOptions(1) )+
    UNSUBSCRIBE: PacketIdentifier(2) | Properties | TopicFilter+

  The Properties field sits between the Packet Identifier and the first filter in both
  packet types (Requirements 9.1, 9.3), and each SUBSCRIBE filter is followed by exactly
  one Subscription Options byte (Requirement 9.2). A default-constructed option set
  reproduces the trailing QoS byte the MQTT 3.1.1 build has always written. Requested
  Maximum QoS 0..2 is accepted and anything above 2 is refused with nothing transmitted
  (Requirements 9.8, 9.9), and a null topic is refused before any length computation
  (Requirement 9.7).

  Reporting (task 9.3):

    SUBACK:   PacketIdentifier(2) | Properties | ReasonCode(1)+
    UNSUBACK: PacketIdentifier(2) | Properties | ReasonCode(1)+

  The Reason Code list is validated against the received bytes, correlated to the
  outstanding entry `subscribe`/`unsubscribe` created, and reported per topic through
  `lastSubscriptionResult()` and the registered subscribe callback - naming the FULL,
  untruncated filter this client sent for that Packet Identifier (Requirements 9.4, 9.5,
  9.6). This closes hardening finding F-10.

  These are EXAMPLE cases. The exhaustive sweeps - the full 36-combination options domain
  and all 256 Reason Code bytes for both acknowledgement types - are the optional property
  tests of this section (Properties 17 and 18) and are deliberately not duplicated here.

  Task 9.6 appends Property 17 (SUBSCRIBE / UNSUBSCRIBE round-trip with Subscription
  Options) at the end of the file: the full legal options domain - Maximum QoS 0..2 x No
  Local {0,1} x Retain As Published {0,1} x Retain Handling 0..2, 36 combinations - swept
  across a representative set of Topic Filters for SUBSCRIBE, the same filters for
  UNSUBSCRIBE, the Requirement 9.9 rejection vectors for Maximum QoS 3..255, and the
  Requirement 9.7 null-topic example cases. The representative cases above pin one shape
  each and stay as the emission/reporting net; Property 17 is the exhaustive sweep.

  This file also carries the MQTT 5.0 equivalent of the two frozen F-08 exact-buffer
  SUBSCRIBE cases, which the Makefile excludes from the v5 run because their capacity
  arithmetic is the literal 3.1.1 bound (see V5_EXCLUDE): the boundary at the 5.0 bound
  under ASan, plus the null-topic rejection those cases bundle in.

  Requirements: 9.1, 9.2, 9.3, 9.4, 9.5, 9.6, 9.7, 9.8, 9.9
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

namespace mqtt5_sub {

void connect(MockClient& client, PubSubClient& psc) {
    client.pushPacket(MqttPacket::connack(0));
    psc.setServer("broker.example", 1883);
    REQUIRE(psc.connect("mqtt5-sub"));
    REQUIRE(psc.connected());
    client.clearOutbound();
    psc.setSocketTimeout(1);
}

// --- Emitted SUBSCRIBE / UNSUBSCRIBE decoder -------------------------------
//
// Written here rather than reusing MqttParser::decodeSubscribe, which is the 3.1.1
// decoder and knows nothing of the Properties field. The generic framing is still checked
// through MqttParser, so this only carves out the 5.0-specific field walk.
struct Sub5 {
    bool                     valid    = false;
    uint8_t                  flags    = 0;
    uint16_t                 packetId = 0;
    DecodedProperties        props;
    std::vector<std::string> filters;
    std::vector<uint8_t>     options;   // one per filter; empty for UNSUBSCRIBE
};

bool readString(const std::vector<uint8_t>& body, size_t& pos, std::string& out) {
    if (pos + 2 > body.size()) {
        return false;
    }
    const size_t len = (static_cast<size_t>(body[pos]) << 8) | body[pos + 1];
    pos += 2;
    if (pos + len > body.size()) {
        return false;
    }
    out.assign(reinterpret_cast<const char*>(body.data() + pos), len);
    pos += len;
    return true;
}

Sub5 decodeSub5(const std::vector<uint8_t>& bytes, bool hasOptions) {
    Sub5 r;
    const DecodedPacket generic = MqttParser::decode(bytes);
    if (!generic.valid) {
        return r;
    }
    r.flags = generic.flags;
    const std::vector<uint8_t>& body = generic.payload;
    size_t pos = 0;
    if (body.size() < 2) {
        return r;
    }
    r.packetId = static_cast<uint16_t>((static_cast<uint16_t>(body[0]) << 8) | body[1]);
    pos = 2;
    r.props = MqttParser::decodeProperties(body, pos);
    if (!r.props.valid) {
        return r;
    }
    pos += r.props.totalBytes;
    while (pos < body.size()) {
        std::string filter;
        if (!readString(body, pos, filter)) {
            return r;
        }
        r.filters.push_back(filter);
        if (hasOptions) {
            if (pos >= body.size()) {
                return r;
            }
            r.options.push_back(body[pos++]);
        }
    }
    r.valid = (pos == body.size()) && !r.filters.empty();
    return r;
}

// --- SUBACK / UNSUBACK fixture --------------------------------------------
//
// body = PacketIdentifier(2) | Properties | ReasonCode(1)+ . The Remaining Length is
// derived from the body, so the packet is always self-consistent and only the body's
// SHAPE is under test.
std::vector<uint8_t> subAck(uint8_t type, uint16_t packetId,
                            const std::vector<uint8_t>& codes,
                            const MqttProps& props = MqttProps()) {
    std::vector<uint8_t> body{static_cast<uint8_t>(packetId >> 8),
                              static_cast<uint8_t>(packetId & 0xFF)};
    const std::vector<uint8_t> p = props.bytes();
    body.insert(body.end(), p.begin(), p.end());
    body.insert(body.end(), codes.begin(), codes.end());

    std::vector<uint8_t> bytes{type};
    const std::vector<uint8_t> rl =
        MqttPacket::encodeRemainingLength(static_cast<uint32_t>(body.size()));
    bytes.insert(bytes.end(), rl.begin(), rl.end());
    bytes.insert(bytes.end(), body.begin(), body.end());
    return bytes;
}

// Records every MqttSubscriptionResult the library reports, so the callback's arguments
// can be asserted after loop() returns. `filter` is COPIED here on purpose: the
// `topicFilter` pointer is a borrowed view with a documented lifetime.
struct Recorder {
    struct Entry {
        uint16_t    packetId = 0;
        uint8_t     reasonCode = 0;
        bool        granted = false;
        bool        hasFilter = false;
        std::string filter;
    };
    std::vector<Entry> entries;
};

void record(const MqttSubscriptionResult& result, void* ctx) {
    Recorder* r = static_cast<Recorder*>(ctx);
    Recorder::Entry e;
    e.packetId = result.packetId;
    e.reasonCode = result.reasonCode;
    e.granted = result.granted;
    e.hasFilter = (result.topicFilter != nullptr);
    if (e.hasFilter) {
        e.filter = result.topicFilter;
    }
    r->entries.push_back(e);
}

// Snapshot every public result an unexpected subscription acknowledgement could otherwise
// mutate. Task 18.5 requires the packet to be completely inert: no callback, no reported
// result or reason, and no emitted byte. The caller proves identifier ownership separately
// by sending the operation's correct acknowledgement afterward.
void expectIgnoredSubscriptionAck(MockClient& client, PubSubClient& psc, Recorder& recorder,
                                  uint8_t type, uint16_t packetId) {
    const MqttSubscriptionResult beforeResult = psc.lastSubscriptionResult();
    const MqttError beforeError = psc.lastError();
    const uint8_t beforeReason = psc.lastReasonCode();
    const uint16_t beforePacketId = psc.lastPacketId();
    const std::vector<uint8_t> beforeOutbound = client.outbound();

    client.pushInbound(subAck(type, packetId, {0x00}));
    CHECK(psc.loop());
    CHECK(psc.connected());
    CHECK_FALSE(client.stopCalled());
    CHECK(client.outbound() == beforeOutbound);
    CHECK(psc.lastError() == beforeError);
    CHECK(psc.lastReasonCode() == beforeReason);
    CHECK(psc.lastPacketId() == beforePacketId);

    const MqttSubscriptionResult afterResult = psc.lastSubscriptionResult();
    CHECK(afterResult.packetId == beforeResult.packetId);
    CHECK(afterResult.reasonCode == beforeResult.reasonCode);
    CHECK(afterResult.granted == beforeResult.granted);
    CHECK(afterResult.topicFilter == beforeResult.topicFilter);
    CHECK(recorder.entries.empty());
}

}  // namespace mqtt5_sub

// ===========================================================================
//  Task 9.2 - SUBSCRIBE / UNSUBSCRIBE emission
// ===========================================================================

// Requirements 9.1, 9.2: the Properties field sits between the Packet Identifier and the
// first filter, and exactly one Subscription Options byte follows the filter, with the
// requested Maximum QoS in bits 0-1, No Local in bit 2, Retain As Published in bit 3,
// Retain Handling in bits 4-5 and bits 6-7 always zero.
TEST_CASE("SUBSCRIBE carries an empty Properties field and one Subscription Options byte") {
    struct Vector {
        const char*          name;
        MqttSubscribeOptions options;
        uint8_t              expected;
    };

    std::vector<Vector> vectors;
    {
        MqttSubscribeOptions o;                                    // all defaults
        vectors.push_back({"default options", o, 0x00});
    }
    for (uint8_t qos = 0; qos <= 2; qos++) {
        MqttSubscribeOptions o;
        o.maxQos = qos;
        vectors.push_back({"maximum QoS only", o, qos});
    }
    {
        MqttSubscribeOptions o;
        o.noLocal = true;
        vectors.push_back({"No Local", o, 0x04});
    }
    {
        MqttSubscribeOptions o;
        o.retainAsPublished = true;
        vectors.push_back({"Retain As Published", o, 0x08});
    }
    for (uint8_t rh = 0; rh <= 2; rh++) {
        MqttSubscribeOptions o;
        o.retainHandling = rh;
        vectors.push_back({"Retain Handling only", o, static_cast<uint8_t>(rh << 4)});
    }
    {
        MqttSubscribeOptions o;
        o.maxQos = 2;
        o.noLocal = true;
        o.retainAsPublished = true;
        o.retainHandling = 2;
        vectors.push_back({"every field set", o, 0x2E});           // 0b0010'1110
    }

    for (const Vector& v : vectors) {
        CAPTURE(v.name);
        CAPTURE(v.options.maxQos);
        CAPTURE(v.options.retainHandling);
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        mqtt5_sub::connect(client, psc);

        REQUIRE(psc.subscribe("tele/dev/SENSOR", v.options));

        const std::vector<uint8_t>& out = client.outbound();
        REQUIRE(MqttParser::isStructurallyWellFramed(out));
        const mqtt5_sub::Sub5 s = mqtt5_sub::decodeSub5(out, true);
        REQUIRE(s.valid);
        // The 0x02 is the mandatory reserved-bit value, not a requested QoS.
        CHECK(s.flags == static_cast<uint8_t>(MQTTQOS1));
        CHECK(s.packetId != 0);
        // Requirement 9.1: present, and empty - a Property Length of 0.
        CHECK(s.props.declaredLength == 0);
        CHECK(s.props.totalBytes == 1);
        CHECK(s.props.entries.empty());
        REQUIRE(s.filters.size() == 1);
        CHECK(s.filters[0] == "tele/dev/SENSOR");
        // Requirement 9.2: exactly one options byte, with the expected bit layout.
        REQUIRE(s.options.size() == 1);
        CHECK(s.options[0] == v.expected);
        CHECK((s.options[0] & 0xC0) == 0);          // bits 6-7 reserved, always zero
    }
}

// Requirement 9.3: UNSUBSCRIBE carries the same Properties field in the same place, and
// its filters are bare - no Subscription Options byte.
TEST_CASE("UNSUBSCRIBE carries an empty Properties field and no options byte") {
    const std::string filters[] = {"a", "cmnd/dev/POWER", "tele/+/SENSOR", "stat/#"};

    for (const std::string& filter : filters) {
        CAPTURE(filter);
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        mqtt5_sub::connect(client, psc);

        REQUIRE(psc.unsubscribe(filter.c_str()));

        const std::vector<uint8_t>& out = client.outbound();
        REQUIRE(MqttParser::isStructurallyWellFramed(out));
        const mqtt5_sub::Sub5 u = mqtt5_sub::decodeSub5(out, false);
        REQUIRE(u.valid);
        CHECK(u.flags == static_cast<uint8_t>(MQTTQOS1));
        CHECK(u.packetId != 0);
        CHECK(u.props.declaredLength == 0);
        CHECK(u.props.totalBytes == 1);
        REQUIRE(u.filters.size() == 1);
        CHECK(u.filters[0] == filter);
        CHECK(u.options.empty());
    }
}

// The default-constructed options must reproduce today's trailing QoS byte value exactly,
// so routing the preserved overloads through MqttSubscribeOptions changes no emitted byte.
TEST_CASE("the preserved subscribe overloads emit the same options byte as before") {
    struct Vector {
        const char* name;
        int         qos;        // -1 = subscribe(topic), i.e. no explicit QoS
        uint8_t     expected;
    };
    const Vector vectors[] = {
        {"subscribe(topic)", -1, 0x00},
        {"subscribe(topic, 0)", 0, 0x00},
        {"subscribe(topic, 1)", 1, 0x01},
        {"subscribe(topic, 2)", 2, 0x02},   // Requirement 9.8: QoS 2 accepted in 5.0
    };

    for (const Vector& v : vectors) {
        CAPTURE(v.name);
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        mqtt5_sub::connect(client, psc);

        if (v.qos < 0) {
            REQUIRE(psc.subscribe("tele/dev/SENSOR"));
        } else {
            REQUIRE(psc.subscribe("tele/dev/SENSOR", static_cast<uint8_t>(v.qos)));
        }

        const mqtt5_sub::Sub5 s = mqtt5_sub::decodeSub5(client.outbound(), true);
        REQUIRE(s.valid);
        REQUIRE(s.options.size() == 1);
        CHECK(s.options[0] == v.expected);
    }
}

// Requirement 9.9: a requested Maximum QoS above 2 is rejected, through either entry
// point, with not one byte transmitted and the connection left usable.
TEST_CASE("a requested Maximum QoS above 2 is rejected with nothing transmitted") {
    const uint8_t badQos[] = {3, 4, 127, 128, 255};

    for (uint8_t qos : badQos) {
        CAPTURE(qos);
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        mqtt5_sub::connect(client, psc);

        CHECK_FALSE(psc.subscribe("tele/dev/SENSOR", qos));
        CHECK(client.outbound().empty());

        MqttSubscribeOptions options;
        options.maxQos = qos;
        CHECK_FALSE(psc.subscribe("tele/dev/SENSOR", options));
        CHECK(client.outbound().empty());

        // Refusal leaves the connection state unchanged.
        CHECK(psc.connected());
        CHECK_FALSE(client.stopCalled());
    }

    // A retain-handling value outside 0..2 is refused the same way: the field is two bits
    // wide, so a larger value cannot be encoded without corrupting its neighbours.
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_sub::connect(client, psc);
    MqttSubscribeOptions options;
    options.retainHandling = 3;
    CHECK_FALSE(psc.subscribe("tele/dev/SENSOR", options));
    CHECK(client.outbound().empty());
    CHECK(psc.connected());
}

// Requirement 9.7: a null topic is rejected before any length computation, through every
// entry point, with nothing emitted. Under ASan a missing guard would fault inside
// strnlen(). This is the 5.0 home of the null-topic half of the frozen F-08 case that the
// Makefile excludes from the v5 run.
TEST_CASE("a null topic filter is rejected before any length computation") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_sub::connect(client, psc);

    MqttSubscribeOptions options;
    options.maxQos = 2;
    CHECK_FALSE(psc.subscribe(nullptr));
    CHECK_FALSE(psc.subscribe(nullptr, static_cast<uint8_t>(0)));
    CHECK_FALSE(psc.subscribe(nullptr, static_cast<uint8_t>(2)));
    CHECK_FALSE(psc.subscribe(nullptr, options));
    CHECK_FALSE(psc.unsubscribe(nullptr));

    CHECK(client.outbound().empty());
    CHECK(psc.connected());
}

// The 5.0 equivalent of the frozen F-08 exact-buffer SUBSCRIBE cases. The 5.0 bound is one
// byte larger than 3.1.1's - header(5) + Packet Identifier(2) + Properties(1) +
// topic-length(2) + topic + options(1) = 11 + topicLength - so at exactly that capacity the
// options byte lands at buffer[bufferSize - 1] and ASan proves nothing is written at
// buffer[bufferSize]. One byte below it, the call must be refused rather than truncated.
TEST_CASE("exact-buffer SUBSCRIBE at the MQTT 5.0 bound stays in bounds") {
    const size_t topicLengths[] = {1, 5, 16, 50, 117};
    const uint8_t qosValues[] = {0, 1, 2};

    for (size_t topicLen : topicLengths) {
        for (uint8_t qos : qosValues) {
            CAPTURE(topicLen);
            CAPTURE(qos);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            mqtt5_sub::connect(client, psc);

            const uint16_t exactCapacity = static_cast<uint16_t>(11 + topicLen);
            REQUIRE(psc.setBufferSize(exactCapacity));
            REQUIRE(psc.getBufferSize() == exactCapacity);

            const std::string topic(topicLen, 'a');
            REQUIRE(psc.subscribe(topic.c_str(), qos));

            const mqtt5_sub::Sub5 s = mqtt5_sub::decodeSub5(client.outbound(), true);
            REQUIRE(s.valid);
            REQUIRE(s.filters.size() == 1);
            CHECK(s.filters[0] == topic);
            REQUIRE(s.options.size() == 1);
            CHECK(s.options[0] == qos);

            // One byte below the bound the packet cannot fit, so it is refused outright.
            client.clearOutbound();
            REQUIRE(psc.setBufferSize(static_cast<uint16_t>(exactCapacity - 1)));
            CHECK_FALSE(psc.subscribe(topic.c_str(), qos));
            CHECK(client.outbound().empty());
            CHECK(psc.connected());
        }
    }

    // UNSUBSCRIBE's bound is one byte smaller: it carries no options byte.
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_sub::connect(client, psc);
    const std::string topic(20, 'u');
    REQUIRE(psc.setBufferSize(static_cast<uint16_t>(10 + topic.size())));
    REQUIRE(psc.unsubscribe(topic.c_str()));
    const mqtt5_sub::Sub5 u = mqtt5_sub::decodeSub5(client.outbound(), false);
    REQUIRE(u.valid);
    REQUIRE(u.filters.size() == 1);
    CHECK(u.filters[0] == topic);
    client.clearOutbound();
    REQUIRE(psc.setBufferSize(static_cast<uint16_t>(9 + topic.size())));
    CHECK_FALSE(psc.unsubscribe(topic.c_str()));
    CHECK(client.outbound().empty());
}

// ===========================================================================
//  Task 9.3 - SUBACK / UNSUBACK reporting
// ===========================================================================

// Requirements 9.4, 9.5: a SUBACK reports success ONLY for the granted-QoS codes 0x00,
// 0x01 and 0x02, and a failure names the filter this client sent for that Packet
// Identifier. Reported both through lastSubscriptionResult() and through the registered
// callback, which receives its stored ctx back unchanged.
TEST_CASE("SUBACK reports the granted QoS or the failure and names the filter") {
    struct Vector {
        uint8_t code;
        bool    granted;
    };
    const Vector vectors[] = {
        {0x00, true},    // Granted QoS 0
        {0x01, true},    // Granted QoS 1
        {0x02, true},    // Granted QoS 2
        {0x80, false},   // Unspecified error
        {0x83, false},   // Implementation specific error
        {0x87, false},   // Not authorized
        {0x8F, false},   // Topic Filter invalid
        {0x91, false},   // Packet Identifier in use
        {0x9E, false},   // Shared Subscriptions not supported
        {0xA2, false},   // Wildcard Subscriptions not supported
    };

    for (const Vector& v : vectors) {
        CAPTURE(v.code);
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        mqtt5_sub::connect(client, psc);

        mqtt5_sub::Recorder recorder;
        psc.setSubscribeCallback(&mqtt5_sub::record, &recorder);

        REQUIRE(psc.subscribe("tele/dev/SENSOR", static_cast<uint8_t>(1)));
        const mqtt5_sub::Sub5 s = mqtt5_sub::decodeSub5(client.outbound(), true);
        REQUIRE(s.valid);
        client.clearOutbound();

        const std::vector<uint8_t> packet =
            mqtt5_sub::subAck(static_cast<uint8_t>(MQTTSUBACK), s.packetId, {v.code});
        REQUIRE(MqttParser::isStructurallyWellFramed(packet));
        client.pushInbound(packet);

        CHECK(psc.loop());
        // A rejected subscription is a normal broker answer, not a protocol error: the
        // connection stays up and nothing is sent back.
        CHECK(psc.connected());
        CHECK_FALSE(client.stopCalled());
        CHECK(client.outbound().empty());

        const MqttSubscriptionResult result = psc.lastSubscriptionResult();
        CHECK(result.packetId == s.packetId);
        CHECK(result.reasonCode == v.code);
        CHECK(result.granted == v.granted);
        REQUIRE(result.topicFilter != nullptr);
        CHECK(std::string(result.topicFilter) == "tele/dev/SENSOR");

        REQUIRE(recorder.entries.size() == 1);
        CHECK(recorder.entries[0].packetId == s.packetId);
        CHECK(recorder.entries[0].reasonCode == v.code);
        CHECK(recorder.entries[0].granted == v.granted);
        REQUIRE(recorder.entries[0].hasFilter);
        CHECK(recorder.entries[0].filter == "tele/dev/SENSOR");
    }
}

// Requirement 9.6: an UNSUBACK carries per-topic Reason Codes too, and BOTH 0x00 (Success)
// and 0x11 (No subscription existed) are non-failures - the subscription is gone either
// way.
TEST_CASE("UNSUBACK treats 0x00 and 0x11 as non-failures and everything else as failure") {
    struct Vector {
        uint8_t code;
        bool    granted;
    };
    const Vector vectors[] = {
        {0x00, true},    // Success
        {0x11, true},    // No subscription existed
        {0x80, false},   // Unspecified error
        {0x83, false},   // Implementation specific error
        {0x87, false},   // Not authorized
        {0x8F, false},   // Topic Filter invalid
        {0x91, false},   // Packet Identifier in use
    };

    for (const Vector& v : vectors) {
        CAPTURE(v.code);
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        mqtt5_sub::connect(client, psc);

        mqtt5_sub::Recorder recorder;
        psc.setSubscribeCallback(&mqtt5_sub::record, &recorder);

        REQUIRE(psc.unsubscribe("cmnd/dev/POWER"));
        const mqtt5_sub::Sub5 u = mqtt5_sub::decodeSub5(client.outbound(), false);
        REQUIRE(u.valid);
        client.clearOutbound();

        const std::vector<uint8_t> packet =
            mqtt5_sub::subAck(static_cast<uint8_t>(MQTTUNSUBACK), u.packetId, {v.code});
        REQUIRE(MqttParser::isStructurallyWellFramed(packet));
        client.pushInbound(packet);

        CHECK(psc.loop());
        CHECK(psc.connected());
        CHECK(client.outbound().empty());

        const MqttSubscriptionResult result = psc.lastSubscriptionResult();
        CHECK(result.packetId == u.packetId);
        CHECK(result.reasonCode == v.code);
        CHECK(result.granted == v.granted);
        REQUIRE(result.topicFilter != nullptr);
        CHECK(std::string(result.topicFilter) == "cmnd/dev/POWER");

        REQUIRE(recorder.entries.size() == 1);
        CHECK(recorder.entries[0].reasonCode == v.code);
        CHECK(recorder.entries[0].granted == v.granted);
    }
}

// Requirement 9.5: the filter is named from the FULL filter retained in the outstanding
// entry, so a failure report is never truncated - not at 48 bytes, not at any length. The
// withdrawn MQTT_SUBACK_TOPIC_LEN would have clipped every one of these.
TEST_CASE("a failing SUBACK names the complete filter however long it is") {
    const size_t lengths[] = {1, 47, 48, 49, 120, 300};

    for (size_t len : lengths) {
        CAPTURE(len);
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        mqtt5_sub::connect(client, psc);
        // The default 1200-byte buffer holds every one of these filters.

        const std::string filter(len, 'f');
        REQUIRE(psc.subscribe(filter.c_str()));
        const mqtt5_sub::Sub5 s = mqtt5_sub::decodeSub5(client.outbound(), true);
        REQUIRE(s.valid);
        REQUIRE(s.filters.size() == 1);
        CHECK(s.filters[0] == filter);
        client.clearOutbound();

        client.pushInbound(mqtt5_sub::subAck(static_cast<uint8_t>(MQTTSUBACK), s.packetId,
                                             {0x87}));
        CHECK(psc.loop());

        const MqttSubscriptionResult result = psc.lastSubscriptionResult();
        CHECK_FALSE(result.granted);
        CHECK(result.reasonCode == 0x87);
        REQUIRE(result.topicFilter != nullptr);
        // The whole filter, byte for byte - not a prefix of it.
        CHECK(std::string(result.topicFilter).size() == len);
        CHECK(std::string(result.topicFilter) == filter);
    }
}

// A one-filter SUBACK carries exactly one Reason Code. It may still carry Properties
// between the Packet Identifier and that one code.
TEST_CASE("a one-filter SUBACK reports its one Reason Code after Properties") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_sub::connect(client, psc);

    mqtt5_sub::Recorder recorder;
    psc.setSubscribeCallback(&mqtt5_sub::record, &recorder);

    REQUIRE(psc.subscribe("tele/dev/SENSOR"));
    const mqtt5_sub::Sub5 s = mqtt5_sub::decodeSub5(client.outbound(), true);
    REQUIRE(s.valid);
    client.clearOutbound();

    // Reason String (0x1F) and a User Property (0x26) are both legal in a SUBACK.
    const MqttProps props = MqttProps().str(0x1F, "partially granted").pair(0x26, "k", "v");
    const std::vector<uint8_t> packet =
        mqtt5_sub::subAck(static_cast<uint8_t>(MQTTSUBACK), s.packetId,
                          {0x87}, props);
    REQUIRE(MqttParser::isStructurallyWellFramed(packet));
    client.pushInbound(packet);

    CHECK(psc.loop());
    CHECK(psc.connected());

    REQUIRE(recorder.entries.size() == 1);
    CHECK(recorder.entries[0].reasonCode == 0x87);
    CHECK_FALSE(recorder.entries[0].granted);
    CHECK(recorder.entries[0].packetId == s.packetId);
    REQUIRE(recorder.entries[0].hasFilter);
    CHECK(recorder.entries[0].filter == "tele/dev/SENSOR");
    CHECK(psc.lastSubscriptionResult().reasonCode == 0x87);
}

// Task 18.6 / Finding 6: every outbound subscription operation is tracked until its own
// terminal acknowledgement. The default envelope covers Tasmota's six core reconnect filters
// plus three fixed extension-shaped operations, then atomically refuses any request that would
// exceed the configured tracked capacity rather than evicting an unanswered exchange.
TEST_CASE("tracked subscriptions survive the Tasmota burst and refuse saturation atomically") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_sub::connect(client, psc);

    const std::string coreBurst[] = {
        "cmnd/device/#", "cmnd/group1/#", "cmnd/group2/#", "cmnd/group3/#",
        "cmnd/group4/#", "cmnd/fallback/#", "domoticz/out", "display/result/#",
        "homeassistant/status/#"
    };
    static_assert(sizeof(coreBurst) / sizeof(coreBurst[0]) == 9,
                  "Task 18.6 fixed non-Mesh Tasmota burst is nine operations");

    std::vector<uint16_t> burstIds;
    for (const std::string& filter : coreBurst) {
        client.clearOutbound();
        REQUIRE(psc.subscribe(filter.c_str()));
        const mqtt5_sub::Sub5 sent = mqtt5_sub::decodeSub5(client.outbound(), true);
        REQUIRE(sent.valid);
        burstIds.push_back(sent.packetId);
    }
    CHECK(psc.inFlightCount() == burstIds.size());

    // ACK in reverse order: every core/extension operation must still retain its own full
    // filter despite all nine having been emitted before any broker response arrived.
    for (size_t i = burstIds.size(); i > 0; --i) {
        const size_t index = i - 1;
        client.pushInbound(mqtt5_sub::subAck(static_cast<uint8_t>(MQTTSUBACK), burstIds[index],
                                             {0x87}));
        CHECK(psc.loop());
        const MqttSubscriptionResult result = psc.lastSubscriptionResult();
        CHECK(result.packetId == burstIds[index]);
        REQUIRE(result.topicFilter != nullptr);
        CHECK(std::string(result.topicFilter) == coreBurst[index]);
    }
    CHECK(psc.inFlightCount() == 0);

    // Fill the whole fixed tracked envelope, then prove both subscription entry points refuse
    // without an outbound byte, state change, or identifier release.
    std::vector<uint16_t> ids;
    std::vector<std::string> filters;
    for (uint16_t i = 0; i < MQTT_MAX_OUTSTANDING; ++i) {
        const std::string filter = "tele/saturated/" + std::to_string(i);
        client.clearOutbound();
        REQUIRE(psc.subscribe(filter.c_str()));
        const mqtt5_sub::Sub5 sent = mqtt5_sub::decodeSub5(client.outbound(), true);
        REQUIRE(sent.valid);
        ids.push_back(sent.packetId);
        filters.push_back(filter);
    }
    REQUIRE(psc.inFlightCount() == MQTT_MAX_OUTSTANDING);
    const MqttSubscriptionResult before = psc.lastSubscriptionResult();

    client.clearOutbound();
    CHECK_FALSE(psc.subscribe("tele/saturated/overflow"));
    CHECK(psc.lastError() == MqttError::PacketIdUnavailable);
    CHECK(client.outbound().empty());
    CHECK(psc.inFlightCount() == MQTT_MAX_OUTSTANDING);
    CHECK_FALSE(psc.unsubscribe("tele/saturated/overflow"));
    CHECK(client.outbound().empty());
    CHECK(psc.inFlightCount() == MQTT_MAX_OUTSTANDING);
    const MqttSubscriptionResult after = psc.lastSubscriptionResult();
    CHECK(after.packetId == before.packetId);
    CHECK(after.reasonCode == before.reasonCode);
    CHECK(after.granted == before.granted);
    CHECK(after.topicFilter == before.topicFilter);

    // All original operations remain live after the failed requests and complete normally.
    for (size_t i = ids.size(); i > 0; --i) {
        const size_t index = i - 1;
        client.pushInbound(mqtt5_sub::subAck(static_cast<uint8_t>(MQTTSUBACK), ids[index],
                                             {0x87}));
        CHECK(psc.loop());
        const MqttSubscriptionResult result = psc.lastSubscriptionResult();
        CHECK(result.packetId == ids[index]);
        REQUIRE(result.topicFilter != nullptr);
        CHECK(std::string(result.topicFilter) == filters[index]);
    }
    CHECK(psc.inFlightCount() == 0);
}

// Task 18.5 / Finding 1: a valid acknowledgement whose identifier is not held by the
// expected operation is ignored. In particular, an unknown identifier must not manufacture
// a subscription result or callback merely because the packet itself is well formed.
TEST_CASE("SUBACK and UNSUBACK for an unknown Packet Identifier are ignored") {
    const uint8_t types[] = {static_cast<uint8_t>(MQTTSUBACK),
                             static_cast<uint8_t>(MQTTUNSUBACK)};

    for (uint8_t type : types) {
        CAPTURE(type);
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        mqtt5_sub::connect(client, psc);

        mqtt5_sub::Recorder recorder;
        psc.setSubscribeCallback(&mqtt5_sub::record, &recorder);
        const MqttSubscriptionResult before = psc.lastSubscriptionResult();

        client.pushInbound(mqtt5_sub::subAck(type, 0x4321, {0x00}));

        CHECK(psc.loop());
        CHECK(psc.connected());
        CHECK_FALSE(client.stopCalled());
        CHECK(psc.lastError() == MqttError::None);
        CHECK(client.outbound().empty());

        const MqttSubscriptionResult after = psc.lastSubscriptionResult();
        CHECK(after.packetId == before.packetId);
        CHECK(after.reasonCode == before.reasonCode);
        CHECK(after.granted == before.granted);
        CHECK(after.topicFilter == before.topicFilter);
        CHECK(recorder.entries.empty());
    }
}

// The Reason Code list length is validated against the RECEIVED bytes, and anything that
// does not fit the layout is rejected through the standard failure funnel: one DISCONNECT
// carrying the Reason Code while the socket is writable, then close.
TEST_CASE("a malformed SUBACK / UNSUBACK is rejected through the failure funnel") {
    struct Vector {
        const char*          name;
        std::vector<uint8_t> body;    // the complete body, framed verbatim
        MqttError            error;
        uint8_t              reasonCode;
    };

    std::vector<Vector> vectors;
    vectors.push_back({"Remaining Length 0", {}, MqttError::MalformedPacket, 0x81});
    vectors.push_back({"Packet Identifier only", {0x00, 0x01},
                       MqttError::MalformedPacket, 0x81});
    // Packet Identifier + an empty Properties field, and NO Reason Code: the list must
    // carry at least one entry.
    vectors.push_back({"empty Reason Code list", {0x00, 0x01, 0x00},
                       MqttError::MalformedPacket, 0x81});
    {
        // The same defect behind a POPULATED Properties field, so the packet is long
        // enough to pass the minimum-size check and the empty list is caught by measuring
        // the list against the received bytes rather than by the packet's total length.
        std::vector<uint8_t> body{0x00, 0x01};
        const std::vector<uint8_t> props = MqttProps().str(0x1F, "ok").bytes();
        body.insert(body.end(), props.begin(), props.end());
        vectors.push_back({"Properties field consuming the whole payload", body,
                           MqttError::MalformedPacket, 0x81});
    }
    {
        // A Property Length declaring more body than the packet carries.
        std::vector<uint8_t> body{0x00, 0x01};
        const std::vector<uint8_t> props = MqttProps().str(0x1F, "ok").rawLength(40).bytes();
        body.insert(body.end(), props.begin(), props.end());
        body.push_back(0x00);
        vectors.push_back({"over-declared Property Length", body,
                           MqttError::MalformedPacket, 0x81});
    }
    {
        // Content Type is a PUBLISH property and illegal in a SUBACK / UNSUBACK.
        std::vector<uint8_t> body{0x00, 0x01};
        const std::vector<uint8_t> props = MqttProps().str(0x03, "text/plain").bytes();
        body.insert(body.end(), props.begin(), props.end());
        body.push_back(0x00);
        vectors.push_back({"property illegal in a SUBACK", body,
                           MqttError::ProtocolError, 0x82});
    }
    {
        // Authentication Method is refused in every packet type, with 0x8C.
        std::vector<uint8_t> body{0x00, 0x01};
        const std::vector<uint8_t> props = MqttProps().str(0x15, "SCRAM-SHA-1").bytes();
        body.insert(body.end(), props.begin(), props.end());
        body.push_back(0x00);
        vectors.push_back({"Authentication Method property", body,
                           MqttError::ProtocolError, 0x8C});
    }

    const uint8_t types[2] = {static_cast<uint8_t>(MQTTSUBACK),
                              static_cast<uint8_t>(MQTTUNSUBACK)};

    for (const Vector& v : vectors) {
        for (uint8_t type : types) {
            CAPTURE(v.name);
            CAPTURE(type);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            mqtt5_sub::connect(client, psc);

            std::vector<uint8_t> packet{type};
            const std::vector<uint8_t> rl =
                MqttPacket::encodeRemainingLength(static_cast<uint32_t>(v.body.size()));
            packet.insert(packet.end(), rl.begin(), rl.end());
            packet.insert(packet.end(), v.body.begin(), v.body.end());
            client.pushInbound(packet);

            CHECK_FALSE(psc.loop());
            CHECK_FALSE(psc.connected());
            CHECK(client.stopCalled());
            CHECK(psc.lastError() == v.error);
            CHECK(psc.lastReasonCode() == v.reasonCode);

            const std::vector<uint8_t> out = client.outbound();
            REQUIRE(out.size() == 3);
            CHECK(out[0] == static_cast<uint8_t>(MQTTDISCONNECT));
            CHECK(out[1] == 0x01);
            CHECK(out[2] == v.reasonCode);
        }
    }
}

// ===========================================================================
//  Task 9.6 - Property 17: SUBSCRIBE / UNSUBSCRIBE round-trip with
//  Subscription Options
// ===========================================================================

namespace mqtt5_p17 {

// Representative Topic Filters: empty is not legal for subscribe/unsubscribe (the
// library's own buffer-capacity math still requires at least the framing overhead, but
// a zero-length filter is a well-formed UTF-8 string), a single character, a typical
// Tasmota-shaped filter, one carrying every wildcard form, and multi-byte UTF-8 so a
// byte-count/character-count mixup would show up in the emitted length.
const char* const kFilters[] = {
    "",
    "a",
    "tele/dev/SENSOR",
    "cmnd/+/POWER",
    "stat/#",
    "e\xC3\xA9\xE2\x82\xAC",   // "e" + U+00E9 (2 bytes) + U+20AC (3 bytes)
};

}  // namespace mqtt5_p17

// Feature: tasmota-pubsub-mqtt5, Property 17: For all topic filters and for all option combinations (Maximum QoS 0..2, No Local, Retain As Published, Retain Handling 0..2), the emitted SUBSCRIBE carries a nonzero Packet Identifier, a Properties field between the identifier and the first filter, each filter unchanged, and exactly one Subscription Options byte per filter whose bits 0-1 hold the requested Maximum QoS, bit 2 the No Local flag, bit 3 the Retain As Published flag, bits 4-5 the retain-handling value and bits 6-7 zero; the emitted UNSUBSCRIBE carries a nonzero identifier, a Properties field before the first filter, and each filter unchanged; and for all requested Maximum QoS values above 2 the call is rejected with no byte transmitted.
TEST_CASE("Property 17: SUBSCRIBE and UNSUBSCRIBE round-trip with Subscription Options") {
    using namespace mqtt5_sub;

    SUBCASE("SUBSCRIBE: the full 36-combination legal options domain") {
        for (const char* filter : mqtt5_p17::kFilters) {
            for (uint8_t maxQos = 0; maxQos <= 2; maxQos++) {
                for (int noLocal = 0; noLocal <= 1; noLocal++) {
                    for (int rap = 0; rap <= 1; rap++) {
                        for (uint8_t rh = 0; rh <= 2; rh++) {
                            CAPTURE(filter);
                            CAPTURE(maxQos);
                            CAPTURE(noLocal);
                            CAPTURE(rap);
                            CAPTURE(rh);

                            TestClock::instance().reset();
                            MockClient client;
                            PubSubClient psc(client);
                            connect(client, psc);

                            MqttSubscribeOptions options;
                            options.maxQos = maxQos;
                            options.noLocal = (noLocal != 0);
                            options.retainAsPublished = (rap != 0);
                            options.retainHandling = rh;

                            REQUIRE(psc.subscribe(filter, options));

                            const std::vector<uint8_t>& out = client.outbound();
                            REQUIRE(MqttParser::isStructurallyWellFramed(out));
                            const Sub5 s = decodeSub5(out, /*hasOptions=*/true);
                            REQUIRE(s.valid);

                            // Nonzero Packet Identifier.
                            CHECK(s.packetId != 0);

                            // A Properties field between the identifier and the first
                            // filter (Requirement 9.1): present, and empty, since this
                            // client sends no SUBSCRIBE property.
                            CHECK(s.props.declaredLength == 0);
                            CHECK(s.props.totalBytes == 1);
                            CHECK(s.props.entries.empty());

                            // The filter, unchanged.
                            REQUIRE(s.filters.size() == 1);
                            CHECK(s.filters[0] == filter);

                            // Exactly one Subscription Options byte per filter
                            // (Requirement 9.2), with the exact bit layout.
                            REQUIRE(s.options.size() == 1);
                            const uint8_t opt = s.options[0];
                            CHECK((opt & 0x03) == maxQos);                    // bits 0-1
                            CHECK(((opt >> 2) & 0x01) == (noLocal ? 1 : 0));  // bit 2
                            CHECK(((opt >> 3) & 0x01) == (rap ? 1 : 0));      // bit 3
                            CHECK(((opt >> 4) & 0x03) == rh);                 // bits 4-5
                            CHECK((opt & 0xC0) == 0);                        // bits 6-7
                        }
                    }
                }
            }
        }
    }

    SUBCASE("UNSUBSCRIBE: the same filters, no options byte") {
        for (const char* filter : mqtt5_p17::kFilters) {
            CAPTURE(filter);

            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            connect(client, psc);

            REQUIRE(psc.unsubscribe(filter));

            const std::vector<uint8_t>& out = client.outbound();
            REQUIRE(MqttParser::isStructurallyWellFramed(out));
            const Sub5 u = decodeSub5(out, /*hasOptions=*/false);
            REQUIRE(u.valid);

            // Nonzero identifier.
            CHECK(u.packetId != 0);

            // A Properties field before the first filter (Requirement 9.3), empty.
            CHECK(u.props.declaredLength == 0);
            CHECK(u.props.totalBytes == 1);
            CHECK(u.props.entries.empty());

            // The filter, unchanged. No options byte at all for UNSUBSCRIBE.
            REQUIRE(u.filters.size() == 1);
            CHECK(u.filters[0] == filter);
            CHECK(u.options.empty());
        }
    }

    SUBCASE("rejection: Maximum QoS above 2 sends nothing, through both entry points") {
        // Requirement 9.9. Boundary just above the legal range, a mid-range value, and
        // the top of the uint8_t domain.
        const uint8_t badQos[] = {3, 4, 10, 100, 200, 254, 255};

        for (uint8_t qos : badQos) {
            CAPTURE(qos);

            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            connect(client, psc);

            CHECK_FALSE(psc.subscribe("tele/dev/SENSOR", qos));
            CHECK(client.outbound().empty());

            MqttSubscribeOptions options;
            options.maxQos = qos;
            CHECK_FALSE(psc.subscribe("tele/dev/SENSOR", options));
            CHECK(client.outbound().empty());

            // Refusal leaves the connection state unchanged.
            CHECK(psc.connected());
            CHECK_FALSE(client.stopCalled());
        }
    }

    SUBCASE("Requirement 9.7: a null topic is rejected before any length computation") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connect(client, psc);

        MqttSubscribeOptions options;
        options.maxQos = 2;
        CHECK_FALSE(psc.subscribe(nullptr));
        CHECK_FALSE(psc.subscribe(nullptr, static_cast<uint8_t>(0)));
        CHECK_FALSE(psc.subscribe(nullptr, static_cast<uint8_t>(2)));
        CHECK_FALSE(psc.subscribe(nullptr, options));
        CHECK_FALSE(psc.unsubscribe(nullptr));

        CHECK(client.outbound().empty());
        CHECK(psc.connected());
    }
}

// ===========================================================================
//  Task 9.7 - Property 18: SUBACK and UNSUBACK reason codes are reported
//  per topic
// ===========================================================================

// Feature: tasmota-pubsub-mqtt5, Property 18: A structurally and semantically valid
// SUBACK or UNSUBACK reports its sole legal Reason Code only when its Packet Identifier
// belongs to the pending operation of that exact type. A SUBACK reports success only for
// granted-QoS codes 0x00, 0x01 and 0x02; an UNSUBACK treats 0x00 and 0x11 as non-failures.
// Every wrong-type or unknown acknowledgement leaves observable results, callbacks, reason
// codes, packet ownership, and transport output unchanged; a subsequent correct type still
// completes the original operation with its full filter.
TEST_CASE("Property 18: legal subscription ACKs require exact pending operation correlation") {
    using namespace mqtt5_sub;

    SUBCASE("SUBACK: every legal Reason Code") {
        const uint8_t legalCodes[] = {
            0x00, 0x01, 0x02, 0x80, 0x83, 0x87, 0x8F, 0x91, 0x97, 0x9E, 0xA1, 0xA2
        };
        for (uint8_t rc : legalCodes) {
            CAPTURE(rc);

            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            connect(client, psc);

            Recorder recorder;
            psc.setSubscribeCallback(&record, &recorder);

            REQUIRE(psc.subscribe("tele/dev/SENSOR", static_cast<uint8_t>(1)));
            const Sub5 s = decodeSub5(client.outbound(), /*hasOptions=*/true);
            REQUIRE(s.valid);
            client.clearOutbound();

            const std::vector<uint8_t> packet =
                subAck(static_cast<uint8_t>(MQTTSUBACK), s.packetId, {rc});
            REQUIRE(MqttParser::isStructurallyWellFramed(packet));
            client.pushInbound(packet);

            CHECK(psc.loop());
            // A Reason Code carrying a failure is a normal broker answer, not a
            // protocol error: the connection stays up either way.
            CHECK(psc.connected());
            CHECK_FALSE(client.stopCalled());
            CHECK(client.outbound().empty());

            // Requirement 9.4: granted only for the granted-QoS codes 0x00, 0x01, 0x02 -
            // never merely "below 0x80". 0x03..0x7F grant no QoS despite being < 0x80.
            const bool expectedGranted = (rc <= 0x02);

            const MqttSubscriptionResult result = psc.lastSubscriptionResult();
            CHECK(result.packetId == s.packetId);
            CHECK(result.reasonCode == rc);
            CHECK(result.granted == expectedGranted);
            REQUIRE(result.topicFilter != nullptr);
            CHECK(std::string(result.topicFilter) == "tele/dev/SENSOR");

            REQUIRE(recorder.entries.size() == 1);
            CHECK(recorder.entries[0].packetId == s.packetId);
            CHECK(recorder.entries[0].reasonCode == rc);
            CHECK(recorder.entries[0].granted == expectedGranted);
            REQUIRE(recorder.entries[0].hasFilter);
            CHECK(recorder.entries[0].filter == "tele/dev/SENSOR");
        }
    }

    SUBCASE("UNSUBACK: every legal Reason Code") {
        const uint8_t legalCodes[] = {0x00, 0x11, 0x80, 0x83, 0x87, 0x8F, 0x91};
        for (uint8_t rc : legalCodes) {
            CAPTURE(rc);

            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            connect(client, psc);

            Recorder recorder;
            psc.setSubscribeCallback(&record, &recorder);

            REQUIRE(psc.unsubscribe("cmnd/dev/POWER"));
            const Sub5 u = decodeSub5(client.outbound(), /*hasOptions=*/false);
            REQUIRE(u.valid);
            client.clearOutbound();

            const std::vector<uint8_t> packet =
                subAck(static_cast<uint8_t>(MQTTUNSUBACK), u.packetId, {rc});
            REQUIRE(MqttParser::isStructurallyWellFramed(packet));
            client.pushInbound(packet);

            CHECK(psc.loop());
            CHECK(psc.connected());
            CHECK_FALSE(client.stopCalled());
            CHECK(client.outbound().empty());

            // Requirement 9.6: 0x00 (Success) and 0x11 (No subscription existed) are both
            // non-failures; everything else is a failure.
            const bool expectedGranted = (rc == 0x00) || (rc == 0x11);

            const MqttSubscriptionResult result = psc.lastSubscriptionResult();
            CHECK(result.packetId == u.packetId);
            CHECK(result.reasonCode == rc);
            CHECK(result.granted == expectedGranted);
            REQUIRE(result.topicFilter != nullptr);
            CHECK(std::string(result.topicFilter) == "cmnd/dev/POWER");

            REQUIRE(recorder.entries.size() == 1);
            CHECK(recorder.entries[0].packetId == u.packetId);
            CHECK(recorder.entries[0].reasonCode == rc);
            CHECK(recorder.entries[0].granted == expectedGranted);
            REQUIRE(recorder.entries[0].hasFilter);
            CHECK(recorder.entries[0].filter == "cmnd/dev/POWER");
        }
    }
}

// Task 18.5 / Finding 1: both SUBACK and UNSUBACK must target their exact pending
// operation. In particular, neither can complete a QoS publish, and the opposite
// subscription acknowledgement cannot release the original operation or lose its filter.
TEST_CASE("wrong-type subscription acknowledgements preserve pending operations") {
    using namespace mqtt5_sub;
    const uint8_t suback = static_cast<uint8_t>(MQTTSUBACK);
    const uint8_t unsuback = static_cast<uint8_t>(MQTTUNSUBACK);
#ifdef USE_MQTT_QOS
    const uint8_t payload[] = {'x'};

    SUBCASE("QoS 1 publish ignores SUBACK and UNSUBACK before its PUBACK") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connect(client, psc);
        Recorder recorder;
        psc.setSubscribeCallback(&record, &recorder);

        REQUIRE(psc.publish("tele/dev/RESULT", payload, sizeof(payload), false, 1));
        const DecodedPublish5 publish = MqttParser::decodePublish5(client.outbound());
        REQUIRE(publish.valid);
        REQUIRE(publish.msgId != 0);
        client.clearOutbound();

        expectIgnoredSubscriptionAck(client, psc, recorder, suback, publish.msgId);
        expectIgnoredSubscriptionAck(client, psc, recorder, unsuback, publish.msgId);
        client.pushPacket(MqttPacket::ack5(MQTTPUBACK, publish.msgId, -1));
        CHECK(psc.loop());
        CHECK(psc.lastPacketId() == publish.msgId);
        CHECK(psc.lastReasonCode() == 0x00);
    }

    SUBCASE("QoS 2 publish ignores SUBACK and UNSUBACK before PUBREC and PUBCOMP") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connect(client, psc);
        Recorder recorder;
        psc.setSubscribeCallback(&record, &recorder);

        REQUIRE(psc.publish("tele/dev/RESULT", payload, sizeof(payload), false, 2));
        const DecodedPublish5 publish = MqttParser::decodePublish5(client.outbound());
        REQUIRE(publish.valid);
        REQUIRE(publish.msgId != 0);
        client.clearOutbound();

        expectIgnoredSubscriptionAck(client, psc, recorder, suback, publish.msgId);
        expectIgnoredSubscriptionAck(client, psc, recorder, unsuback, publish.msgId);
        client.pushPacket(MqttPacket::ack5(MQTTPUBREC, publish.msgId, -1));
        CHECK(psc.loop());
        client.clearOutbound();  // PUBREL
        client.pushPacket(MqttPacket::ack5(MQTTPUBCOMP, publish.msgId, -1));
        CHECK(psc.loop());
        CHECK(psc.lastPacketId() == publish.msgId);
        CHECK(psc.lastReasonCode() == 0x00);
    }
#endif  // USE_MQTT_QOS

    SUBCASE("UNSUBACK cannot complete SUBSCRIBE, then SUBACK retains its full filter") {
        const std::string filter = "tele/device/very/long/filter/that/must/survive";
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connect(client, psc);
        Recorder recorder;
        psc.setSubscribeCallback(&record, &recorder);

        REQUIRE(psc.subscribe(filter.c_str()));
        const Sub5 sent = decodeSub5(client.outbound(), true);
        REQUIRE(sent.valid);
        client.clearOutbound();

        expectIgnoredSubscriptionAck(client, psc, recorder, unsuback, sent.packetId);
        client.pushInbound(subAck(suback, sent.packetId, {0x87}));
        CHECK(psc.loop());
        REQUIRE(recorder.entries.size() == 1);
        CHECK(recorder.entries[0].packetId == sent.packetId);
        CHECK_FALSE(recorder.entries[0].granted);
        CHECK(recorder.entries[0].filter == filter);
    }

    SUBCASE("SUBACK cannot complete UNSUBSCRIBE, then UNSUBACK retains its full filter") {
        const std::string filter = "cmnd/device/very/long/filter/that/must/survive";
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connect(client, psc);
        Recorder recorder;
        psc.setSubscribeCallback(&record, &recorder);

        REQUIRE(psc.unsubscribe(filter.c_str()));
        const Sub5 sent = decodeSub5(client.outbound(), false);
        REQUIRE(sent.valid);
        client.clearOutbound();

        expectIgnoredSubscriptionAck(client, psc, recorder, suback, sent.packetId);
        client.pushInbound(subAck(unsuback, sent.packetId, {0x80}));
        CHECK(psc.loop());
        REQUIRE(recorder.entries.size() == 1);
        CHECK(recorder.entries[0].packetId == sent.packetId);
        CHECK_FALSE(recorder.entries[0].granted);
        CHECK(recorder.entries[0].filter == filter);
    }
}

namespace mqtt5_owned_string {

constexpr uint8_t kTopicAliasMaximum = 0x22;

void verify(size_t filterLength, size_t topicLength) {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    MqttProps props;
    props.u16(kTopicAliasMaximum, 2);
    client.pushPacket(MqttPacket::connack5(0x00, false, props));
    psc.setServer("broker.example", 1883);
    REQUIRE(psc.connect("mqtt5-owned-string"));
    REQUIRE(psc.connected());
    REQUIRE(psc.serverTopicAliasMaximum() == 2u);
    client.clearOutbound();

    const std::string filter(filterLength, 'f');

    // Both acknowledgement packet types report the retained filter after a failure.
    // Check size as well as equality: a fixed buffer that happens to contain the prefix
    // would otherwise evade an equality-only test when the expected string is shortened.
    REQUIRE(psc.subscribe(filter.c_str()));
    const mqtt5_sub::Sub5 subscribe = mqtt5_sub::decodeSub5(client.outbound(), true);
    REQUIRE(subscribe.valid);
    client.clearOutbound();
    client.pushInbound(mqtt5_sub::subAck(static_cast<uint8_t>(MQTTSUBACK),
                                         subscribe.packetId, {0x87}));
    REQUIRE(psc.loop());
    MqttSubscriptionResult result = psc.lastSubscriptionResult();
    REQUIRE(result.topicFilter != nullptr);
    CHECK(std::string(result.topicFilter).size() == filterLength);
    CHECK(std::string(result.topicFilter) == filter);

    REQUIRE(psc.unsubscribe(filter.c_str()));
    const mqtt5_sub::Sub5 unsubscribe = mqtt5_sub::decodeSub5(client.outbound(), false);
    REQUIRE(unsubscribe.valid);
    client.clearOutbound();
    client.pushInbound(mqtt5_sub::subAck(static_cast<uint8_t>(MQTTUNSUBACK),
                                         unsubscribe.packetId, {0x80}));
    REQUIRE(psc.loop());
    result = psc.lastSubscriptionResult();
    REQUIRE(result.topicFilter != nullptr);
    CHECK(std::string(result.topicFilter).size() == filterLength);
    CHECK(std::string(result.topicFilter) == filter);

    // Establish two aliases whose topic names differ only in their final byte. Any return
    // to a fixed 64-byte (or other prefix) store would make these collide at the named
    // boundaries below; full String ownership keeps both mappings distinct.
    std::string firstTopic(topicLength, 'a');
    std::string secondTopic(topicLength, 'a');
    firstTopic.back() = 'A';
    secondTopic.back() = 'B';
    const uint8_t payload = 'x';
    MqttPublishRequest request;
    request.payload = &payload;
    request.plength = 1;
    request.useTopicAlias = true;

    request.topic = firstTopic.c_str();
    REQUIRE(psc.publish(request));
    DecodedPublish5 first = MqttParser::decodePublish5(client.outbound());
    REQUIRE(first.valid);
    CHECK(first.topic.size() == topicLength);
    CHECK(first.topic == firstTopic);
    REQUIRE(first.topicAlias >= 1u);
    client.clearOutbound();

    request.topic = secondTopic.c_str();
    REQUIRE(psc.publish(request));
    DecodedPublish5 second = MqttParser::decodePublish5(client.outbound());
    REQUIRE(second.valid);
    CHECK(second.topic.size() == topicLength);
    CHECK(second.topic == secondTopic);
    REQUIRE(second.topicAlias >= 1u);
    CHECK(second.topicAlias != first.topicAlias);
    client.clearOutbound();

    request.topic = firstTopic.c_str();
    REQUIRE(psc.publish(request));
    const DecodedPublish5 firstReused = MqttParser::decodePublish5(client.outbound());
    REQUIRE(firstReused.valid);
    CHECK(firstReused.topic.empty());
    CHECK(firstReused.topicAlias == first.topicAlias);
    client.clearOutbound();

    request.topic = secondTopic.c_str();
    REQUIRE(psc.publish(request));
    const DecodedPublish5 secondReused = MqttParser::decodePublish5(client.outbound());
    REQUIRE(secondReused.valid);
    CHECK(secondReused.topic.empty());
    CHECK(secondReused.topicAlias == second.topicAlias);
    CHECK(psc.connected());
}

}  // namespace mqtt5_owned_string

// Feature: tasmota-pubsub-mqtt5, Property 30: For all topic filters and topic names the
// client retains beyond a single packet — the outstanding entry's filter reported with a SUBACK
// or UNSUBACK result, and the outbound topic alias map's stored topic — and for all lengths up
// to what the working buffer admits, including lengths past the 48- and 64-byte capacities the
// withdrawn fixed arrays imposed, the value read back through the Public_API equals the whole
// supplied string at its full supplied length, with no prefix truncation at any fixed capacity.
//
// Property 30: Owned strings grow to the stored value
TEST_CASE("Property 30: retained filters and aliases retain whole owned strings") {
    using mqtt5_owned_string::verify;

    SUBCASE("length 1") { verify(1, 1); }
    SUBCASE("length 47") { verify(47, 47); }
    SUBCASE("length 48 (withdrawn MQTT_SUBACK_TOPIC_LEN boundary)") { verify(48, 48); }
    SUBCASE("length 49") { verify(49, 49); }
    SUBCASE("length 63") { verify(63, 63); }
    SUBCASE("length 64 (withdrawn MQTT_TOPIC_ALIAS_TOPIC_LEN boundary)") { verify(64, 64); }
    SUBCASE("length 65") { verify(65, 65); }
    SUBCASE("length 200") { verify(200, 200); }
    SUBCASE("largest default working-buffer values") {
        // SUBSCRIBE's 11-byte assembly overhead admits a 1,189-byte filter;
        // first-use aliasing adds a three-byte property and therefore admits a
        // 1,188-byte topic with the same one-byte payload.
        verify(MQTT_MAX_PACKET_SIZE - 11u, MQTT_MAX_PACKET_SIZE - 12u);
    }
}

#endif  // MQTT_VERSION == MQTT_VERSION_5_0

}  // TEST_SUITE("mqtt5")
