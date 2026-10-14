/*
  mqtt5_publish_test.cpp - MQTT 5.0 outbound PUBLISH framing (task 8.1).

  Feature: tasmota-pubsub-mqtt5

  TEST_SUITE("mqtt5"), body guarded by `#if MQTT_VERSION == MQTT_VERSION_5_0`, so
  the file compiles in both selections and contributes cases only to the 5.0
  binary.

  Scope at task 8.1 is the layout the shared assembler produces for all three
  publish paths:

    0x30 | (DUP<<3) | (QoS<<1) | Retain | TopicName |
        [PacketIdentifier(2) if QoS>0] | Properties | payload

  The Properties field follows the Packet Identifier when one is present and the
  Topic Name when one is not (Requirements 6.1, 6.2), and the empty set that the
  public overloads supply today is the single byte 0x00 (Requirement 6.3). Public
  QoS > 0 entry points arrive with task 11, so every case here is QoS 0; the
  assembler is QoS-capable but unreachable at QoS > 0 from the public API.

  The last case is the one that carries real weight beyond layout. The narrowed
  Variable Byte Integer codec refuses a Remaining Length above 16,383, and the
  extra Properties byte moves the largest accepted `beginPublish` plength down by
  one in the 5.0 selection. The 3.1.1-shaped boundary subcase of the two frozen
  F-04 hardening cases therefore cannot hold here and is excluded from the v5 run
  in the Makefile; this case is its 5.0 replacement, pinning the boundary from
  both sides with the 5.0 overhead (Requirement 8.17, Property 28).

  These are example cases, deliberately few: the exhaustive sweep over topics,
  payload sizes, QoS values, retain flags and property sets is Property 7
  (task 11.10, appended below), and the outbound property set itself is
  Property 8 (task 11.11).

  Task 11.10 appends Property 7 at the end of the file: a curated table drives
  all four publish entry points - the buffered `publish()`, the full-form
  `publish(const MqttPublishRequest&)` (the only one that reaches QoS > 0 or a
  non-empty message property set), `publish_P()`, and the streaming
  `beginPublish`/`write`/`endPublish` - across topics (including empty and
  multi-byte UTF-8), payload sizes 0, 1, 127 and 128, retain on/off, QoS 0..2
  where reachable, and a curated set of message property sets. Layout is
  checked two ways: through `decodePublish5` (QoS bits, retain, DUP, Packet
  Identifier presence, payload) and through an independent byte walk that
  starts from the front of the packet without calling decodePublish5, so
  "Properties begins immediately after X" is asserted against raw offsets
  rather than inferred from the convenience decoder's own walk. A separate
  SUBCASE pins the working-buffer boundary for the two buffered entry points.

  Requirements: 6.1, 6.2, 6.3, 8.1, 8.3, 8.17, 4.9, 11.5, 11.8, 11.9, 13.2
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

namespace mqtt5_publish {

// A connected client with a cleared outbound record. MqttPacket::connack() is
// selection-aware, so this scripts a 5.0 CONNACK.
void connectAndClear(MockClient& client, PubSubClient& psc) {
    client.pushPacket(MqttPacket::connack(0));
    psc.setServer("broker.example", 1883);
    REQUIRE(psc.connect("mqtt5-pub"));
    REQUIRE(psc.connected());
    client.clearOutbound();
}

// What every accepted PUBLISH in this file must satisfy: an empty Properties
// field is present, and it is the single byte 0x00.
void checkEmptyPropertiesField(const DecodedPublish5& d) {
    CHECK(d.props.valid);
    CHECK(d.props.declaredLength == 0u);
    CHECK(d.props.totalBytes == 1u);
    CHECK(d.props.entries.empty());
}

}  // namespace mqtt5_publish

TEST_CASE("buffered publish frames an empty Properties field after the Topic Name") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_publish::connectAndClear(client, psc);

    REQUIRE(psc.publish("tele/dev/STATE", "online"));

    const std::vector<uint8_t>& out = client.outbound();
    CHECK(MqttParser::isStructurallyWellFramed(out));

    const DecodedPublish5 d = MqttParser::decodePublish5(out);
    REQUIRE(d.valid);
    CHECK(d.qos == 0u);
    CHECK_FALSE(d.dup);
    CHECK_FALSE(d.retain);
    CHECK(d.msgId == 0u);
    CHECK(d.topic == "tele/dev/STATE");
    mqtt5_publish::checkEmptyPropertiesField(d);

    // The payload starts after the Properties field, not inside it: a decoder
    // that ignored the field would report a leading 0x00 here.
    CHECK(std::string(d.payload.begin(), d.payload.end()) == "online");

    // Spelled out on the wire: fixed header, 1-byte Remaining Length, topic
    // length prefix, topic, the 0x00 Properties field, then the payload.
    const std::string topic = "tele/dev/STATE";
    const std::string payload = "online";
    const size_t remaining = 2u + topic.size() + 1u + payload.size();
    REQUIRE(out.size() == 2u + remaining);
    CHECK(out[0] == static_cast<uint8_t>(MQTTPUBLISH));
    CHECK(out[1] == static_cast<uint8_t>(remaining));
    CHECK(out[2u + 2u + topic.size()] == 0x00);
}

TEST_CASE("buffered publish carries the retain flag and an empty payload") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_publish::connectAndClear(client, psc);

    REQUIRE(psc.publish("cmnd/dev/POWER", reinterpret_cast<const uint8_t*>(""), 0u, true));

    const DecodedPublish5 d = MqttParser::decodePublish5(client.outbound());
    REQUIRE(d.valid);
    CHECK(d.retain);
    CHECK(d.qos == 0u);
    CHECK(d.topic == "cmnd/dev/POWER");
    mqtt5_publish::checkEmptyPropertiesField(d);
    // An empty payload is a length of 0, not a missing Properties field.
    CHECK(d.payload.empty());
}

TEST_CASE("publish_P frames the same layout with a streamed payload") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_publish::connectAndClear(client, psc);

    const std::string payload = "23.5";
    REQUIRE(psc.publish_P("tele/dev/SENSOR",
                          reinterpret_cast<const uint8_t*>(payload.data()),
                          static_cast<unsigned int>(payload.size()), false));

    const std::vector<uint8_t>& out = client.outbound();
    CHECK(MqttParser::isStructurallyWellFramed(out));

    const DecodedPublish5 d = MqttParser::decodePublish5(out);
    REQUIRE(d.valid);
    CHECK(d.qos == 0u);
    CHECK_FALSE(d.retain);
    CHECK(d.topic == "tele/dev/SENSOR");
    mqtt5_publish::checkEmptyPropertiesField(d);
    CHECK(std::string(d.payload.begin(), d.payload.end()) == payload);
}

TEST_CASE("streaming publish frames the Properties field before the first payload byte") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    mqtt5_publish::connectAndClear(client, psc);

    const std::string topic = "tele/dev/FILE";
    const std::string payload = "chunk-one";

    REQUIRE(psc.beginPublish(topic.c_str(), static_cast<unsigned int>(payload.size()), false));

    // beginPublish emits the header only: the Properties byte is part of it, so
    // the declared Remaining Length has to account for it.
    {
        const std::vector<uint8_t>& hdr = client.outbound();
        const size_t remaining = 2u + topic.size() + 1u + payload.size();
        REQUIRE(hdr.size() == 2u + 2u + topic.size() + 1u);
        CHECK(hdr[0] == static_cast<uint8_t>(MQTTPUBLISH));
        CHECK(hdr[1] == static_cast<uint8_t>(remaining));
        CHECK(hdr.back() == 0x00);
    }

    CHECK(psc.write(reinterpret_cast<const uint8_t*>(payload.data()), payload.size()) ==
          payload.size());
    CHECK(psc.endPublish() == 1);

    const std::vector<uint8_t>& out = client.outbound();
    CHECK(MqttParser::isStructurallyWellFramed(out));

    const DecodedPublish5 d = MqttParser::decodePublish5(out);
    REQUIRE(d.valid);
    CHECK(d.topic == topic);
    mqtt5_publish::checkEmptyPropertiesField(d);
    CHECK(std::string(d.payload.begin(), d.payload.end()) == payload);
}

// Requirement 8.17 / Property 28, at the 5.0 boundary. This is the 5.0
// replacement for the boundary SUBCASE of the two frozen F-04 hardening cases,
// which derive their plength from the 3.1.1 overhead (`2 + topicLen`) and are
// excluded from the v5 run for exactly that reason - see V5_EXCLUDE in the
// Makefile. The refusal subcase of those cases is version-neutral and still runs
// in both selections.
TEST_CASE("streaming publish refuses a declared length that overflows the 2-byte Remaining Length") {
    const std::string topic = "t";                     // topicLen == 1
    // Remaining Length = 2 + topicLen + 1 (Properties) + plength.
    const unsigned int overhead = static_cast<unsigned int>(2u + topic.size() + 1u);

    // Named so it cannot be caught by the Makefile's `-sce` exclusion of the
    // 3.1.1-shaped F-04 boundary subcases, which this case replaces for v5.
    SUBCASE("5.0 boundary: Remaining Length 16382 and 16383 frame in two length bytes") {
        const uint32_t remainingLengths[] = {16382u, 16383u};

        for (uint32_t rlWanted : remainingLengths) {
            CAPTURE(rlWanted);
            const unsigned int plen = static_cast<unsigned int>(rlWanted - overhead);
            CAPTURE(plen);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            mqtt5_publish::connectAndClear(client, psc);

            REQUIRE(psc.beginPublish(topic.c_str(), plen, false));

            const std::vector<uint8_t>& out = client.outbound();
            REQUIRE(out.size() >= 3u);
            CHECK(static_cast<uint8_t>(out[0] & 0xF0) == static_cast<uint8_t>(MQTTPUBLISH));

            uint32_t rl = 0;
            size_t rlBytes = 0;
            REQUIRE(MqttParser::decodeRemainingLength(out, 1, rl, rlBytes));
            CHECK(rl == rlWanted);
            CHECK(rlBytes == 2u);

            // Header only, and the Properties byte is the last of it: fixed
            // header + Remaining Length + topic length prefix + topic + 0x00.
            CHECK(out.size() == 1u + rlBytes + 2u + topic.size() + 1u);
            CHECK(out.back() == 0x00);
        }
    }

    SUBCASE("above the limit: refused, nothing emitted, connection intact") {
        const unsigned int plengths[] = {
            16384u - overhead,      // Remaining Length 16,384 - the first refused value
            65533u,
            100000u,
            2097149u,
            268435455u - overhead,
        };

        for (unsigned int plen : plengths) {
            CAPTURE(plen);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            mqtt5_publish::connectAndClear(client, psc);

            CHECK_FALSE(psc.beginPublish(topic.c_str(), plen, false));

            // The refusal is total: no truncated Remaining Length reaches the
            // wire, so the outbound stream cannot desynchronize.
            CHECK(client.outbound().empty());
            CHECK(psc.lastError() == MqttError::PacketTooLarge);

            // And the connection is untouched by it.
            CHECK(psc.connected());
            CHECK_FALSE(client.stopCalled());
        }
    }
}

// === Property 7: PUBLISH field round-trip and layout (task 11.10) ==========

namespace mqtt5_p7 {

// A topic of exactly n 'a' bytes, for length-boundary coverage.
std::string makeTopic(size_t n) { return std::string(n, 'a'); }

// A payload of exactly n bytes, deterministic and 0x00-free so it can also be
// used as a C string by publish_P()/publish(const char*).
std::string makePayload(size_t n) { return std::string(n, 'x'); }

// Fixed backing storage for the message property sets below. `properties`
// borrows every pointer for the duration of the one publish() call that
// consumes it, and this storage outlives the whole test binary, which is more
// than that requires.
static const uint8_t kCorrelation[4] = {0xDE, 0xAD, 0x00, 0xEF};
static const char* const kUserKeys[3]   = {"k1", "k1", "k2"};
static const char* const kUserValues[3] = {"v1", "v2", "v3"};

enum class PropKind { None, PayloadFormatAndContentType, ResponseAndCorrelation, UserProperties };

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
        props.responseTopic      = "reply/to";
        props.correlationData    = kCorrelation;
        props.correlationDataLen = 4;
        break;
    case PropKind::UserProperties:
        props.userPropertyKeys   = kUserKeys;
        props.userPropertyValues = kUserValues;
        props.userPropertyCount  = 3;
        break;
    }
}

// Which publish entry point a curated vector is routed through. `PublishRequest`
// is the only one that can carry QoS > 0 or a non-empty property set; the other
// three are reachable only at QoS 0, no properties (Requirements 8.1, 8.3).
enum class Entry { Publish, PublishRequest, PublishP, Streaming };

// One publish vector: topic, payload, retain flag, QoS, property set and which
// entry point to drive it through - the "for all topics, payloads, retain
// flags, QoS values in 0..2, and message property sets" the property
// quantifies over.
struct Vector {
    std::string name;
    Entry       entry;
    std::string topic;
    std::string payload;
    bool        retained;
    uint8_t     qos;
    PropKind    propKind;
};

std::vector<Vector> curatedVectors() {
    std::vector<Vector> v;
    auto add = [&v](Vector x) { v.push_back(x); };

    // --- publish(): QoS 0 only, no properties, across topic/payload shapes -
    add({"publish(): empty topic and payload", Entry::Publish,
        makeTopic(0), makePayload(0), false, 0, PropKind::None});
    add({"publish(): single-char topic, single-byte payload", Entry::Publish,
        makeTopic(1), makePayload(1), false, 0, PropKind::None});
    add({"publish(): topic/payload at the 127/128 boundary, retained", Entry::Publish,
        makeTopic(127), makePayload(128), true, 0, PropKind::None});
    add({"publish(): payload of 128 bytes, not retained", Entry::Publish,
        makeTopic(8), makePayload(128), false, 0, PropKind::None});
    add({"publish(): multi-byte UTF-8 topic and payload", Entry::Publish,
        "t/\xC3\xA9\xE2\x82\xAC", "p/\xC3\xA9\xE2\x82\xAC", false, 0, PropKind::None});

    // --- publish_P(): QoS 0 only, no properties, mirrors publish() ---------
    add({"publish_P(): empty payload", Entry::PublishP,
        makeTopic(6), makePayload(0), false, 0, PropKind::None});
    add({"publish_P(): payload at the 127/128 boundary, retained", Entry::PublishP,
        makeTopic(6), makePayload(128), true, 0, PropKind::None});

    // --- streaming (beginPublish/write/endPublish): QoS 0 only, no properties
    add({"streaming: empty payload", Entry::Streaming,
        makeTopic(9), makePayload(0), false, 0, PropKind::None});
    add({"streaming: single-byte payload, retained", Entry::Streaming,
        makeTopic(9), makePayload(1), true, 0, PropKind::None});
    add({"streaming: payload at the 127/128 boundary", Entry::Streaming,
        makeTopic(9), makePayload(128), false, 0, PropKind::None});

    // --- publish(const MqttPublishRequest&): QoS 0..2, retain on/off, and
    //     every curated message property set -----------------------------
    // QoS 1/2 vectors exist only when USE_MQTT_QOS is compiled in; the QoS-0-only build
    // refuses them, so the corpus stops at QoS 0 there.
#ifdef USE_MQTT_QOS
    const uint8_t kMaxRequestQos = 2;
#else
    const uint8_t kMaxRequestQos = 0;
#endif
    for (uint8_t qos = 0; qos <= kMaxRequestQos; qos++) {
        add({"request: QoS " + std::to_string((unsigned)qos) + ", no properties, empty payload",
            Entry::PublishRequest, makeTopic(5), makePayload(0), false, qos, PropKind::None});
        add({"request: QoS " + std::to_string((unsigned)qos) +
                ", retained, payload format + content type",
            Entry::PublishRequest, makeTopic(5), makePayload(16), true, qos,
            PropKind::PayloadFormatAndContentType});
    }
#ifdef USE_MQTT_QOS
    add({"request: QoS 1, response topic paired with correlation data",
        Entry::PublishRequest, makeTopic(12), makePayload(4), false, 1,
        PropKind::ResponseAndCorrelation});
    add({"request: QoS 2, retained, user properties with a duplicate key",
        Entry::PublishRequest, makeTopic(12), makePayload(128), true, 2,
        PropKind::UserProperties});
#endif  // USE_MQTT_QOS
    add({"request: QoS 0, multi-byte UTF-8 topic, payload at the 127/128 boundary",
        Entry::PublishRequest, "t/\xC3\xA9\xE2\x82\xAC", makePayload(127), false, 0,
        PropKind::None});

    return v;
}

// A connected client with a cleared outbound record.
void connect(MockClient& client, PubSubClient& psc) {
    client.pushPacket(MqttPacket::connack(0));
    psc.setServer("broker.example", 1883);
    REQUIRE(psc.connect("mqtt5-p7"));
    REQUIRE(psc.connected());
    client.clearOutbound();
}

// Drive one curated vector through its entry point. Returns whether the
// publish was accepted - every curated vector is expected to be, so the
// caller REQUIREs the result rather than skipping on failure.
bool drive(MockClient& /*client*/, PubSubClient& psc, const Vector& v) {
    switch (v.entry) {
    case Entry::Publish:
        return psc.publish(v.topic.c_str(),
                           reinterpret_cast<const uint8_t*>(v.payload.data()),
                           static_cast<unsigned int>(v.payload.size()), v.retained);
    case Entry::PublishP:
        return psc.publish_P(v.topic.c_str(),
                             reinterpret_cast<const uint8_t*>(v.payload.data()),
                             static_cast<unsigned int>(v.payload.size()), v.retained);
    case Entry::Streaming: {
        if (!psc.beginPublish(v.topic.c_str(),
                              static_cast<unsigned int>(v.payload.size()), v.retained)) {
            return false;
        }
        if (!v.payload.empty()) {
            if (psc.write(reinterpret_cast<const uint8_t*>(v.payload.data()),
                          v.payload.size()) != v.payload.size()) {
                return false;
            }
        }
        return psc.endPublish() == 1;
    }
    case Entry::PublishRequest: {
        MqttPublishRequest req;
        req.topic    = v.topic.c_str();
        req.payload  = reinterpret_cast<const uint8_t*>(v.payload.data());
        req.plength  = static_cast<unsigned int>(v.payload.size());
        req.retained = v.retained;
        req.qos      = v.qos;
        fillProps(v.propKind, req.properties);
        return psc.publish(req);
    }
    }
    return false;   // unreachable
}

// Find one property entry by identifier. Returns nullptr when absent.
const DecodedProperty* find(const DecodedProperties& props, uint8_t id) {
    for (const DecodedProperty& e : props.entries) {
        if (e.id == id) { return &e; }
    }
    return nullptr;
}

// Assert that `d.props` equals exactly the property set `fillProps(kind, ...)`
// builds, independent of the entry point that produced it.
void checkProperties(PropKind kind, const DecodedProperties& props) {
    REQUIRE(props.valid);
    switch (kind) {
    case PropKind::None:
        CHECK(props.declaredLength == 0u);
        CHECK(props.totalBytes == 1u);
        CHECK(props.entries.empty());
        break;
    case PropKind::PayloadFormatAndContentType: {
        REQUIRE(props.entries.size() == 2);
        const DecodedProperty* pf = find(props, MQTT_PROP_PAYLOAD_FORMAT_INDICATOR);
        REQUIRE(pf != nullptr);
        CHECK(pf->value == 1u);
        const DecodedProperty* ct = find(props, MQTT_PROP_CONTENT_TYPE);
        REQUIRE(ct != nullptr);
        CHECK(ct->s == "application/json");
        break;
    }
    case PropKind::ResponseAndCorrelation: {
        REQUIRE(props.entries.size() == 2);
        const DecodedProperty* rt = find(props, MQTT_PROP_RESPONSE_TOPIC);
        REQUIRE(rt != nullptr);
        CHECK(rt->s == "reply/to");
        const DecodedProperty* cd = find(props, MQTT_PROP_CORRELATION_DATA);
        REQUIRE(cd != nullptr);
        CHECK(cd->bin == std::vector<uint8_t>(kCorrelation, kCorrelation + 4));
        break;
    }
    case PropKind::UserProperties: {
        std::vector<const DecodedProperty*> ups;
        for (const DecodedProperty& e : props.entries) {
            if (e.id == MQTT_PROP_USER_PROPERTY) { ups.push_back(&e); }
        }
        REQUIRE(ups.size() == 3);
        CHECK(ups[0]->s == "k1"); CHECK(ups[0]->s2 == "v1");
        CHECK(ups[1]->s == "k1"); CHECK(ups[1]->s2 == "v2");
        CHECK(ups[2]->s == "k2"); CHECK(ups[2]->s2 == "v3");
        break;
    }
    }
}

}  // namespace mqtt5_p7

// Feature: tasmota-pubsub-mqtt5, Property 7: For all topics, payloads (including empty), retain flags, QoS values in 0..2, and message property sets, the emitted PUBLISH decodes to a packet whose QoS bits equal the requested QoS, whose retain bit equals the requested flag, whose DUP bit is clear on first transmission, which carries a Packet Identifier if and only if QoS is greater than 0, whose Properties field begins immediately after the Packet Identifier when present and immediately after the Topic Name otherwise, whose properties round-trip to the supplied set (with a single 0x00 byte when none were supplied), and whose payload equals the supplied payload.
//
// Validates: Requirements 6.1, 6.2, 6.3, 8.1, 8.3, 11.5, 11.8, 11.9
TEST_CASE("Property 7: PUBLISH field round-trip and layout") {
    using namespace mqtt5_p7;

    for (const Vector& v : curatedVectors()) {
        SUBCASE(v.name.c_str()) {
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            connect(client, psc);

            REQUIRE(drive(client, psc, v));

            const std::vector<uint8_t>& out = client.outbound();
            CHECK(MqttParser::isStructurallyWellFramed(out));

            const DecodedPublish5 d = MqttParser::decodePublish5(out);
            REQUIRE(d.valid);

            // QoS bits, retain bit, and DUP clear on first transmission.
            CHECK(d.qos == v.qos);
            CHECK(d.retain == v.retained);
            CHECK_FALSE(d.dup);

            // A Packet Identifier is present if and only if QoS > 0.
            if (v.qos > 0) {
                CHECK(d.msgId != 0u);
            } else {
                CHECK(d.msgId == 0u);
            }

            CHECK(d.topic == v.topic);
            checkProperties(v.propKind, d.props);
            CHECK(std::string(d.payload.begin(), d.payload.end()) == v.payload);

            // Positional half: an independent walk from the front of the
            // packet, without calling decodePublish5, so "Properties begins
            // immediately after the Packet Identifier / Topic Name" is
            // asserted against raw byte offsets rather than inferred from the
            // convenience decoder's own walk.
            size_t pos = 1;   // past the fixed-header byte
            uint32_t remainingLength = 0;
            size_t rlUsed = 0;
            REQUIRE(MqttParser::decodeVbi(out, pos, remainingLength, rlUsed));
            pos += rlUsed;

            REQUIRE(pos + 2 <= out.size());
            const size_t topicLen =
                (static_cast<size_t>(out[pos]) << 8) | out[pos + 1];
            pos += 2 + topicLen;
            REQUIRE(pos <= out.size());

            if (v.qos > 0) {
                pos += 2;   // Packet Identifier
                REQUIRE(pos <= out.size());
            }

            // Whatever sits at `pos` right now is the Properties field: its
            // length prefix decodes, and consuming it lands exactly where
            // decodePublish5's own `props.totalBytes` says it should.
            REQUIRE(pos < out.size());
            const DecodedProperties atPos = MqttParser::decodeProperties(out, pos);
            REQUIRE(atPos.valid);
            CHECK(atPos.totalBytes == d.props.totalBytes);
            CHECK(atPos.entries.size() == d.props.entries.size());
        }
    }
}

// Requirement 13.2 / working-buffer boundary, for the two buffered entry
// points (`publish()` and `publish(const MqttPublishRequest&)`) that copy the
// payload into the working buffer and so are bounded by it - `publish_P()`
// streams from flash and `beginPublish` streams from the caller, so neither is
// bounded by the buffer in the same way (Property 4 / task 8.1 already covers
// their own limits).
//
// The gate compares assembled bytes IN THE BUFFER - reserved header space +
// Topic Name + Packet Identifier + Properties + payload - against the
// configured buffer size, not the emitted wire size (the reserved header
// space is wider than the 1-2 bytes the Remaining Length actually occupies on
// the wire). A buffer sized to exactly that assembled total is accepted; one
// byte less is refused with nothing transmitted and the connection intact.
TEST_CASE("Property 7: buffered publish entry points stay within the working buffer") {
    using namespace mqtt5_p7;

    const std::string topic = "t";           // topic length 1: minimal, explicit overhead
    const size_t payloadLen = 190;
    const std::string payload(payloadLen, 'y');

    // Assembled bytes in the buffer: MQTT_MAX_HEADER_SIZE (reserved) + topic
    // length prefix + topic + Properties (1 byte, the empty set) + payload.
    // No Packet Identifier: both entry points are driven at QoS 0 here.
    const uint16_t capacityNeeded =
        static_cast<uint16_t>(MQTT_MAX_HEADER_SIZE + 2 + topic.size() + 1 + payloadLen);

    for (bool useRequest : {false, true}) {
        CAPTURE(useRequest);

        SUBCASE("exactly enough capacity is accepted") {
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            connect(client, psc);
            REQUIRE(psc.setBufferSize(capacityNeeded));

            bool ok;
            if (useRequest) {
                MqttPublishRequest req;
                req.topic   = topic.c_str();
                req.payload = reinterpret_cast<const uint8_t*>(payload.data());
                req.plength = static_cast<unsigned int>(payload.size());
                ok = psc.publish(req);
            } else {
                ok = psc.publish(topic.c_str(),
                                 reinterpret_cast<const uint8_t*>(payload.data()),
                                 static_cast<unsigned int>(payload.size()), false);
            }
            REQUIRE(ok);

            const DecodedPublish5 d = MqttParser::decodePublish5(client.outbound());
            REQUIRE(d.valid);
            CHECK(d.topic == topic);
            CHECK(std::string(d.payload.begin(), d.payload.end()) == payload);
        }

        SUBCASE("one byte short of capacity is refused, nothing transmitted") {
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            connect(client, psc);
            REQUIRE(psc.setBufferSize(static_cast<uint16_t>(capacityNeeded - 1)));

            bool ok;
            if (useRequest) {
                MqttPublishRequest req;
                req.topic   = topic.c_str();
                req.payload = reinterpret_cast<const uint8_t*>(payload.data());
                req.plength = static_cast<unsigned int>(payload.size());
                ok = psc.publish(req);
            } else {
                ok = psc.publish(topic.c_str(),
                                 reinterpret_cast<const uint8_t*>(payload.data()),
                                 static_cast<unsigned int>(payload.size()), false);
            }
            CHECK_FALSE(ok);
            CHECK(client.outbound().empty());
            CHECK(psc.connected());
            CHECK_FALSE(client.stopCalled());
        }
    }
}

// === Property 8: Outbound message properties encode independently and
// faithfully (task 11.11) ===================================================

namespace mqtt5_p8 {

// A connected client with a cleared outbound record.
void connect(MockClient& client, PubSubClient& psc) {
    client.pushPacket(MqttPacket::connack(0));
    psc.setServer("broker.example", 1883);
    REQUIRE(psc.connect("mqtt5-p8"));
    REQUIRE(psc.connected());
    client.clearOutbound();
}

const DecodedProperty* find(const DecodedProperties& props, uint8_t id) {
    for (const DecodedProperty& e : props.entries) {
        if (e.id == id) { return &e; }
    }
    return nullptr;
}

std::vector<const DecodedProperty*> findAll(const DecodedProperties& props, uint8_t id) {
    std::vector<const DecodedProperty*> out;
    for (const DecodedProperty& e : props.entries) {
        if (e.id == id) { out.push_back(&e); }
    }
    return out;
}

// Deterministic Correlation Data content of `len` bytes with an embedded 0x00
// near the middle, mirroring mqtt5_properties_test.cpp's inbound makeContent() -
// proving binary-safety on the outbound side too. The caller overrides this for
// the named boundaries: len == 1 to a single 0x00 byte and len == 16 to
// all-0xFF, exactly as the inbound sweep does.
std::vector<uint8_t> makeCorrelation(size_t len) {
    std::vector<uint8_t> v(len, 0xAB);
    if (len >= 2) {
        v[len / 2] = 0x00;
    }
    return v;
}

// Fixed backing storage for the User Property vectors below: five entries with
// a duplicate key at index 2, so the ordering and multiplicity checks have
// something to distinguish from a plain set. `properties` borrows every
// pointer for the duration of the one publish() call that consumes it, and
// this storage outlives the whole test binary, which is more than that
// requires.
static const char* const kUserKeys[5]   = {"k1", "k2", "k1", "k3", "k4"};
static const char* const kUserValues[5] = {"v1", "v2", "v1b", "v3", "v4"};

static const uint8_t kPayload[] = {'x'};

// One publish through the full-form request, decoded and returned so a caller
// can inspect `props` without repeating the connect / publish / decode
// boilerplate.
DecodedPublish5 publishAndDecode(MockClient& client, PubSubClient& psc,
                                 MqttPublishRequest& req) {
    req.topic   = "tele/p8/STATE";
    req.payload = kPayload;
    req.plength = static_cast<unsigned int>(sizeof(kPayload));
    REQUIRE(psc.publish(req));
    const DecodedPublish5 d = MqttParser::decodePublish5(client.outbound());
    REQUIRE(d.valid);
    REQUIRE(d.props.valid);
    return d;
}

}  // namespace mqtt5_p8

// Feature: tasmota-pubsub-mqtt5, Property 8: For all supplied message property sets, every supplied Payload Format Indicator (0 or 1), Content Type, Response Topic, Correlation Data, and User Property pair appears in the emitted Properties field with its supplied value and, for User Properties, in supplied order. Response Topic and Correlation Data are independently optional, and Correlation Data remains binary-safe for lengths 0..16 including embedded 0x00 bytes.
//
// Validates: Requirements 11.3, 11.5, 11.6, 11.8, 11.9
TEST_CASE("Property 8: Outbound message properties encode independently and faithfully") {
    using namespace mqtt5_p8;

    SUBCASE("Payload Format Indicator and Content Type carry their supplied values") {
        for (uint8_t pfi : {static_cast<uint8_t>(0), static_cast<uint8_t>(1)}) {
            CAPTURE(pfi);

            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            connect(client, psc);

            MqttPublishRequest req;
            req.properties.hasPayloadFormat = true;
            req.properties.payloadFormat    = pfi;
            req.properties.contentType      = "application/json";

            const DecodedPublish5 d = publishAndDecode(client, psc, req);

            const DecodedProperty* pf = find(d.props, MQTT_PROP_PAYLOAD_FORMAT_INDICATOR);
            REQUIRE(pf != nullptr);
            CHECK(pf->value == pfi);

            const DecodedProperty* ct = find(d.props, MQTT_PROP_CONTENT_TYPE);
            REQUIRE(ct != nullptr);
            CHECK(ct->s == "application/json");
        }
    }

    SUBCASE("User Properties appear in supplied order, at counts 0, 1, 2 and 5 "
            "with a duplicate key") {
        for (uint8_t count : {static_cast<uint8_t>(0), static_cast<uint8_t>(1),
                              static_cast<uint8_t>(2), static_cast<uint8_t>(5)}) {
            CAPTURE(count);

            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            connect(client, psc);

            MqttPublishRequest req;
            if (count > 0) {
                req.properties.userPropertyKeys   = kUserKeys;
                req.properties.userPropertyValues = kUserValues;
                req.properties.userPropertyCount  = count;
            }

            const DecodedPublish5 d = publishAndDecode(client, psc, req);

            const std::vector<const DecodedProperty*> ups =
                findAll(d.props, MQTT_PROP_USER_PROPERTY);
            REQUIRE(ups.size() == count);
            for (uint8_t i = 0; i < count; i++) {
                CHECK(ups[i]->s == kUserKeys[i]);
                CHECK(ups[i]->s2 == kUserValues[i]);
            }
        }
    }

    SUBCASE("Response Topic and Correlation Data are independently optional") {
        enum class Selection { Neither, ResponseOnly, CorrelationOnly, Both };
        struct Case { const char* name; Selection selection; };
        const Case cases[] = {
            {"neither supplied", Selection::Neither},
            {"Response Topic only", Selection::ResponseOnly},
            {"Correlation Data only", Selection::CorrelationOnly},
            {"both supplied", Selection::Both},
        };

        static const uint8_t kCorrelation[4] = {0xDE, 0xAD, 0x00, 0xEF};

        for (const Case& c : cases) {
            SUBCASE(c.name) {
                TestClock::instance().reset();
                MockClient client;
                PubSubClient psc(client);
                connect(client, psc);

                MqttPublishRequest req;
                const bool hasResponse =
                    c.selection == Selection::ResponseOnly || c.selection == Selection::Both;
                const bool hasCorrelation =
                    c.selection == Selection::CorrelationOnly || c.selection == Selection::Both;
                if (hasResponse) {
                    req.properties.responseTopic = "reply/to";
                }
                if (hasCorrelation) {
                    req.properties.correlationData    = kCorrelation;
                    req.properties.correlationDataLen = sizeof(kCorrelation);
                }

                const DecodedPublish5 d = publishAndDecode(client, psc, req);

                const DecodedProperty* rt = find(d.props, MQTT_PROP_RESPONSE_TOPIC);
                const DecodedProperty* cd = find(d.props, MQTT_PROP_CORRELATION_DATA);

                if (hasResponse) {
                    REQUIRE(rt != nullptr);
                    CHECK(rt->s == "reply/to");
                } else {
                    CHECK(rt == nullptr);
                }
                if (hasCorrelation) {
                    REQUIRE(cd != nullptr);
                    CHECK(cd->bin == std::vector<uint8_t>(kCorrelation,
                                                          kCorrelation + sizeof(kCorrelation)));
                } else {
                    CHECK(cd == nullptr);
                }
            }
        }
    }

    SUBCASE("Correlation Data is exhaustive over 0..16 bytes, paired with a "
            "Response Topic") {
        for (size_t len = 0; len <= 16; len++) {
            CAPTURE(len);

            std::vector<uint8_t> content;
            if (len == 1) {
                content = {0x00};                       // single NUL byte
            } else if (len == 16) {
                content.assign(16, 0xFF);                // all-0xFF, the upper boundary
            } else {
                content = makeCorrelation(len);          // embedded 0x00 for len >= 2
            }

            if (len == 0) {
                SUBCASE("boundary len=0 (empty Correlation Data)") {}
            } else if (len == 1) {
                SUBCASE("boundary len=1 (single NUL byte)") {}
            } else if (len == 15) {
                SUBCASE("boundary len=15 (embedded NUL, just below the inbound cap)") {}
            } else if (len == 16) {
                SUBCASE("boundary len=16 (all-0xFF, at the inbound cap - the outbound "
                        "path has none, Requirement 11.10)") {}
            }

            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            connect(client, psc);

            // `content.data()` on an empty vector is not guaranteed non-null. A
            // zero-length Correlation Data value is present only when supplied through
            // a non-null pointer, so use stable storage for the empty boundary.
            static const uint8_t kEmptyGuard = 0xFF;
            MqttPublishRequest req;
            req.properties.responseTopic      = "reply/to";
            req.properties.correlationData    = content.empty() ? &kEmptyGuard : content.data();
            req.properties.correlationDataLen = static_cast<uint16_t>(content.size());

            const DecodedPublish5 d = publishAndDecode(client, psc, req);

            const DecodedProperty* rt = find(d.props, MQTT_PROP_RESPONSE_TOPIC);
            REQUIRE(rt != nullptr);
            CHECK(rt->s == "reply/to");

            const DecodedProperty* cd = find(d.props, MQTT_PROP_CORRELATION_DATA);
            REQUIRE(cd != nullptr);
            CHECK(cd->bin == content);
        }
    }
}

namespace mqtt5_alias {

constexpr uint8_t kTopicAliasMaximum = 0x22;

void connect(MockClient& client, PubSubClient& psc, uint16_t aliasMaximum) {
    MqttProps props;
    props.u16(kTopicAliasMaximum, aliasMaximum);
    client.pushPacket(MqttPacket::connack5(0x00, false, props));
    psc.setServer("broker.example", 1883);
    REQUIRE(psc.connect("mqtt5-alias"));
    REQUIRE(psc.connected());
    REQUIRE(psc.serverTopicAliasMaximum() == aliasMaximum);
    client.clearOutbound();
}

DecodedPublish5 publish(MockClient& client, PubSubClient& psc, const std::string& topic) {
    MqttPublishRequest request;
    request.topic = topic.c_str();
    request.payload = reinterpret_cast<const uint8_t*>("x");
    request.plength = 1;
    request.useTopicAlias = true;
    REQUIRE(psc.publish(request));
    DecodedPublish5 decoded = MqttParser::decodePublish5(client.outbound());
    REQUIRE(decoded.valid);
    REQUIRE(decoded.props.valid);
    client.clearOutbound();
    return decoded;
}

}  // namespace mqtt5_alias

// Feature: tasmota-pubsub-mqtt5, Property 21: For all sequences of publishes with aliasing
// enabled and for all Topic Alias Maximum values greater than zero, the first publish of a topic
// carries the full Topic Name together with a Topic Alias property whose value is in 1..Topic
// Alias Maximum, every later publish of that same topic carries a zero-length Topic Name with the
// same alias value, and once the number of distinct aliases in use reaches the maximum, a publish
// of a new topic carries the full Topic Name with no Topic Alias property and every existing
// mapping remains unchanged.
//
// Property 21: Outbound topic aliasing preserves the mapping table
TEST_CASE("Property 21: outbound topic aliases establish, reuse, and saturate") {
    using namespace mqtt5_alias;

    // The host harness compiles the library with a small nonzero node cap. These vectors
    // exercise every negotiated limit up to that cap, including the full-table path where
    // no eviction is permitted.
    const uint16_t aliasMaximums[] = {1u, 2u, MQTT_MAX_TOPIC_ALIASES};
    for (uint16_t aliasMaximum : aliasMaximums) {
        CAPTURE(aliasMaximum);
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connect(client, psc, aliasMaximum);

        std::vector<std::string> mappedTopics;
        std::vector<uint16_t> aliases;
        for (uint16_t i = 0; i < aliasMaximum; ++i) {
            const std::string topic = "tele/alias/" + std::to_string(i);
            const DecodedPublish5 first = publish(client, psc, topic);
            CHECK(first.topic == topic);
            REQUIRE(first.topicAlias >= 1u);
            CHECK(first.topicAlias <= aliasMaximum);
            mappedTopics.push_back(topic);
            aliases.push_back(first.topicAlias);
        }

        // The table is full. A distinct topic remains explicit and must not replace any
        // established alias mapping.
        const std::string overflowTopic = "tele/alias/overflow";
        const DecodedPublish5 overflow = publish(client, psc, overflowTopic);
        CHECK(overflow.topic == overflowTopic);
        CHECK(overflow.topicAlias == 0u);

        // Every original mapping still produces an empty Topic Name with precisely the
        // alias assigned when it was established.
        for (size_t i = 0; i < mappedTopics.size(); ++i) {
            const DecodedPublish5 reused = publish(client, psc, mappedTopics[i]);
            CHECK(reused.topic.empty());
            CHECK(reused.topicAlias == aliases[i]);
        }

        // The rejected new mapping was not silently installed as an alias.
        const DecodedPublish5 overflowAgain = publish(client, psc, overflowTopic);
        CHECK(overflowAgain.topic == overflowTopic);
        CHECK(overflowAgain.topicAlias == 0u);
        CHECK(psc.connected());
    }
}

#endif   // MQTT_VERSION == MQTT_VERSION_5_0

}  // TEST_SUITE("mqtt5")
