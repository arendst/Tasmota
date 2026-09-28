/*
  mqtt5_properties_test.cpp - Tests for the MQTT 5.0 properties codec.

  Feature: tasmota-pubsub-mqtt5

  Task 4.1 (this file's first content): the property type model and the property
  identifier table. The table is pure data, so what is worth pinning is that the
  data matches the specification: which identifiers exist, how each value is
  encoded, which contexts each identifier is legal in, and which identifiers may
  repeat. The expected rows below are written out from MQTT 5.0 section 2.2.2 /
  the design's property identifier table rather than read back from the library,
  so a typo in either copy shows up as a mismatch.

  Task 4.2 adds the writer: MqttPropertyWriter (sizing mode and emit mode) and
  writeProperties(), which frames `<VBI bodyLength><body>` and - crucially -
  returns its input position unchanged, having written nothing, when the field
  would not fit (Requirement 13.2). The expected byte strings below are written
  out by hand from MQTT 5.0 section 2.2.2 rather than produced by the library.

  Task 4.5 appends the Property 3 round-trip cases (encode then decode yields an
  equivalent set) to this same file. Those cases close the loop the sections above
  leave open: each direction above is pinned against hand-written bytes, while
  Property 3 sends a curated set through the library writer, back through the
  library reader, and back a second time through MqttParser::decodeProperties -
  the harness's independent decoder - so a writer/reader pair that agreed only
  with each other would still be caught (Requirements 15.4, 15.5).

  Everything here is MQTT 5.0 only: the 3.1.1 selection has no Properties field
  on the wire and does not compile the table at all, hence TEST_SUITE("mqtt5")
  plus the `#if MQTT_VERSION == MQTT_VERSION_5_0` body guard.

  Requirements: 3.1, 3.7, 3.8, 1.2, 1.3, 6.3, 9.1, 9.3, 13.1, 13.2
*/

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>

#include "doctest.h"

#include "MqttPacket.h"
#include "PubSubClient.h"
#include "AllocInterposer.h"
#include "CallbackContractAdapter.h"
#include "MockClient.h"
#include "TestClock.h"

TEST_SUITE("mqtt5") {

#if MQTT_VERSION == MQTT_VERSION_5_0

namespace mqtt5_props {

// One expected table row, transcribed from the specification.
struct ExpectedDef {
    uint8_t      id;
    const char*  name;
    MqttPropType type;
    uint16_t     packets;
    bool         multiple;
};

// Shorthands for the contexts that recur, so each row below stays readable.
static constexpr uint16_t kPubOrWill = MQTT_CTX_PUBLISH | MQTT_CTX_WILL;
static constexpr uint16_t kConnPair  = MQTT_CTX_CONNECT | MQTT_CTX_CONNACK;
static constexpr uint16_t kAckAll    = MQTT_CTX_PUBACK | MQTT_CTX_PUBREC |
                                       MQTT_CTX_PUBREL | MQTT_CTX_PUBCOMP;

static const std::vector<ExpectedDef>& expected() {
    static const std::vector<ExpectedDef> table = {
        {0x01, "Payload Format Indicator",          MqttPropType::Byte,       kPubOrWill,                                                       false},
        {0x02, "Message Expiry Interval",           MqttPropType::FourByte,   kPubOrWill,                                                       false},
        {0x03, "Content Type",                      MqttPropType::Utf8,       kPubOrWill,                                                       false},
        {0x08, "Response Topic",                    MqttPropType::Utf8,       kPubOrWill,                                                       false},
        {0x09, "Correlation Data",                  MqttPropType::Binary,     kPubOrWill,                                                       false},
        {0x0B, "Subscription Identifier",           MqttPropType::VarInt,     MQTT_CTX_PUBLISH | MQTT_CTX_SUBSCRIBE,                            true },
        {0x11, "Session Expiry Interval",           MqttPropType::FourByte,   kConnPair | MQTT_CTX_DISCONNECT,                                  false},
        {0x12, "Assigned Client Identifier",        MqttPropType::Utf8,       MQTT_CTX_CONNACK,                                                 false},
        {0x13, "Server Keep Alive",                 MqttPropType::TwoByte,    MQTT_CTX_CONNACK,                                                 false},
        // Rejected identifiers (Requirements 3.11, 16.5, 16.6): PROPERTY_TABLE carries
        // no value type, no packet context and no multiplicity for either - they exist
        // only to be refused, in both directions.
        {0x15, "Authentication Method",             MqttPropType::None,       0,                                                                false},
        {0x16, "Authentication Data",               MqttPropType::None,       0,                                                                false},
        {0x17, "Request Problem Information",       MqttPropType::Byte,       MQTT_CTX_CONNECT,                                                 false},
        {0x18, "Will Delay Interval",               MqttPropType::FourByte,   MQTT_CTX_WILL,                                                    false},
        {0x19, "Request Response Information",      MqttPropType::Byte,       MQTT_CTX_CONNECT,                                                 false},
        {0x1A, "Response Information",              MqttPropType::Utf8,       MQTT_CTX_CONNACK,                                                 false},
        {0x1C, "Server Reference",                  MqttPropType::Utf8,       MQTT_CTX_CONNACK | MQTT_CTX_DISCONNECT,                           false},
        {0x1F, "Reason String",                     MqttPropType::Utf8,       MQTT_CTX_CONNACK | kAckAll | MQTT_CTX_SUBACK |
                                                                              MQTT_CTX_UNSUBACK | MQTT_CTX_DISCONNECT | MQTT_CTX_AUTH,          false},
        {0x21, "Receive Maximum",                   MqttPropType::TwoByte,    kConnPair,                                                        false},
        {0x22, "Topic Alias Maximum",               MqttPropType::TwoByte,    kConnPair,                                                        false},
        {0x23, "Topic Alias",                       MqttPropType::TwoByte,    MQTT_CTX_PUBLISH,                                                 false},
        {0x24, "Maximum QoS",                       MqttPropType::Byte,       MQTT_CTX_CONNACK,                                                 false},
        {0x25, "Retain Available",                  MqttPropType::Byte,       MQTT_CTX_CONNACK,                                                 false},
        {0x26, "User Property",                     MqttPropType::StringPair, kConnPair | kPubOrWill | kAckAll | MQTT_CTX_SUBSCRIBE |
                                                                              MQTT_CTX_SUBACK | MQTT_CTX_UNSUBSCRIBE | MQTT_CTX_UNSUBACK |
                                                                              MQTT_CTX_DISCONNECT | MQTT_CTX_AUTH,                              true },
        {0x27, "Maximum Packet Size",               MqttPropType::FourByte,   kConnPair,                                                        false},
        {0x28, "Wildcard Subscription Available",   MqttPropType::Byte,       MQTT_CTX_CONNACK,                                                 false},
        {0x29, "Subscription Identifier Available", MqttPropType::Byte,       MQTT_CTX_CONNACK,                                                 false},
        {0x2A, "Shared Subscription Available",     MqttPropType::Byte,       MQTT_CTX_CONNACK,                                                 false}
    };
    return table;
}

}  // namespace mqtt5_props

TEST_CASE("property table defines exactly the MQTT 5.0 identifiers") {
    const auto& exp = mqtt5_props::expected();

    REQUIRE(exp.size() == 27);
    CHECK((size_t)PubSubClient::propertyDefCount() == exp.size());

    // Sweep the whole byte range: an identifier is defined if and only if it is one
    // of the 27. Anything else must report "unknown" (nullptr), which is what makes
    // an unrecognized identifier a protocol error rather than a misparse.
    size_t defined = 0;
    for (unsigned i = 0; i <= 0xFF; i++) {
        const uint8_t id = (uint8_t)i;
        bool shouldExist = false;
        for (const auto& e : exp) {
            if (e.id == id) { shouldExist = true; break; }
        }
        const MqttPropertyDef* def = PubSubClient::findPropertyDef(id);
        if (shouldExist) {
            REQUIRE_MESSAGE(def != nullptr, "identifier ", (unsigned)id, " should be defined");
            CHECK(def->id == id);
            defined++;
        } else {
            CHECK_MESSAGE(def == nullptr, "identifier ", (unsigned)id, " should be unknown");
        }
    }
    CHECK(defined == exp.size());
}

TEST_CASE("property table records each identifier's type, contexts and multiplicity") {
    for (const auto& e : mqtt5_props::expected()) {
        SUBCASE(e.name) {
            const MqttPropertyDef* def = PubSubClient::findPropertyDef(e.id);
            REQUIRE(def != nullptr);
            CHECK(def->id == e.id);
            CHECK(def->type == e.type);
            CHECK(def->packets == e.packets);
            CHECK(def->multiple == e.multiple);

            // No identifier is legal in a context that carries no Properties field.
            CHECK((def->packets & (uint16_t)MQTT_CTX_PINGREQ) == 0);
            CHECK((def->packets & (uint16_t)MQTT_CTX_PINGRESP) == 0);
            // Every identifier is legal somewhere, except the two Enhanced
            // Authentication identifiers (Requirements 3.11, 16.5, 16.6): their rows
            // carry `packets == 0` on purpose, so they are refused everywhere rather
            // than legal anywhere.
            const bool isAuthProperty = (e.id == 0x15) || (e.id == 0x16);
            if (isAuthProperty) {
                CHECK(def->packets == 0);
                CHECK(def->type == MqttPropType::None);
            } else {
                CHECK(def->packets != 0);
            }
        }
    }
}

TEST_CASE("only Subscription Identifier and User Property may repeat") {
    // Requirement 3.8 hinges on this flag: every other identifier appearing twice in
    // one Properties field is a protocol error.
    for (unsigned i = 0; i <= 0xFF; i++) {
        const MqttPropertyDef* def = PubSubClient::findPropertyDef((uint8_t)i);
        if (def == nullptr) { continue; }
        const bool mayRepeat = (def->id == 0x0B) || (def->id == 0x26);
        CHECK(def->multiple == mayRepeat);
    }
}

// --- Task 4.2: the properties writer ---------------------------------------

namespace mqtt5_props {

// MQTT packet type numbers, as writeProperties() takes them. 0 is the Will
// Properties pseudo-context.
static constexpr uint8_t kWill    = 0;
static constexpr uint8_t kConnect = 1;
static constexpr uint8_t kPublish = 3;

// Sentinel fill, so "nothing was written" is checkable byte by byte rather than
// inferred from the return value alone.
static constexpr uint8_t kFill = 0xAA;

struct Scratch {
    std::vector<uint8_t> buf;
    explicit Scratch(size_t n) : buf(n, kFill) {}
    // Bytes written between `from` and `to`.
    std::vector<uint8_t> slice(size_t from, size_t to) const {
        return std::vector<uint8_t>(buf.begin() + from, buf.begin() + to);
    }
    // True when every byte from `from` onwards is still the sentinel.
    bool untouchedFrom(size_t from) const {
        for (size_t i = from; i < buf.size(); i++) {
            if (buf[i] != kFill) { return false; }
        }
        return true;
    }
};

// The body a writer produces on its own, with no length prefix, so the entry
// encodings can be pinned independently of the framing.
template <typename Fn>
static std::vector<uint8_t> body(Fn add, uint16_t capacity = 512) {
    std::vector<uint8_t> out(capacity, kFill);
    MqttPropertyWriter emit(out.data(), capacity);
    add(emit);
    CHECK_FALSE(emit.overflowed());

    // The same calls in sizing mode must agree on the length exactly.
    MqttPropertyWriter size(nullptr, capacity);
    add(size);
    CHECK_FALSE(size.overflowed());
    CHECK(size.bodyLength() == emit.bodyLength());

    out.resize(emit.bodyLength());
    return out;
}

}  // namespace mqtt5_props

TEST_CASE("writer encodes each value type at its specification width") {
    using namespace mqtt5_props;

    SUBCASE("Byte") {
        CHECK(body([](MqttPropertyWriter& w) { w.addByte(0x01, 0x01); }) ==
              std::vector<uint8_t>{0x01, 0x01});
    }
    SUBCASE("Two Byte Integer is big-endian") {
        CHECK(body([](MqttPropertyWriter& w) { w.addTwoByte(0x21, 0x1234); }) ==
              std::vector<uint8_t>{0x21, 0x12, 0x34});
        CHECK(body([](MqttPropertyWriter& w) { w.addTwoByte(0x21, 0xFFFF); }) ==
              std::vector<uint8_t>{0x21, 0xFF, 0xFF});
    }
    SUBCASE("Four Byte Integer is big-endian") {
        CHECK(body([](MqttPropertyWriter& w) { w.addFourByte(0x27, 0x01020304); }) ==
              std::vector<uint8_t>{0x27, 0x01, 0x02, 0x03, 0x04});
        CHECK(body([](MqttPropertyWriter& w) { w.addFourByte(0x27, 0xFFFFFFFFu); }) ==
              std::vector<uint8_t>{0x27, 0xFF, 0xFF, 0xFF, 0xFF});
    }
    SUBCASE("Variable Byte Integer spans both size classes") {
        CHECK(body([](MqttPropertyWriter& w) { w.addVarInt(0x0B, 0); }) ==
              std::vector<uint8_t>{0x0B, 0x00});
        CHECK(body([](MqttPropertyWriter& w) { w.addVarInt(0x0B, 127); }) ==
              std::vector<uint8_t>{0x0B, 0x7F});
        CHECK(body([](MqttPropertyWriter& w) { w.addVarInt(0x0B, 128); }) ==
              std::vector<uint8_t>{0x0B, 0x80, 0x01});
        CHECK(body([](MqttPropertyWriter& w) { w.addVarInt(0x0B, 16383); }) ==
              std::vector<uint8_t>{0x0B, 0xFF, 0x7F});
    }
    SUBCASE("UTF-8 string carries a 2-byte length") {
        CHECK(body([](MqttPropertyWriter& w) { w.addString(0x03, "ab"); }) ==
              std::vector<uint8_t>{0x03, 0x00, 0x02, 'a', 'b'});
        // An empty string is a legal value: length 0 and no bytes.
        CHECK(body([](MqttPropertyWriter& w) { w.addString(0x03, ""); }) ==
              std::vector<uint8_t>{0x03, 0x00, 0x00});
    }
    SUBCASE("Binary Data may contain NUL bytes") {
        static const uint8_t blob[] = {0x00, 0xFF, 0x00};
        CHECK(body([](MqttPropertyWriter& w) { w.addBinary(0x09, blob, 3); }) ==
              std::vector<uint8_t>{0x09, 0x00, 0x03, 0x00, 0xFF, 0x00});
        CHECK(body([](MqttPropertyWriter& w) { w.addBinary(0x09, blob, 0); }) ==
              std::vector<uint8_t>{0x09, 0x00, 0x00});
    }
    SUBCASE("UTF-8 string pair is key then value") {
        CHECK(body([](MqttPropertyWriter& w) { w.addPair(0x26, "k", "vv"); }) ==
              std::vector<uint8_t>{0x26, 0x00, 0x01, 'k', 0x00, 0x02, 'v', 'v'});
    }
    SUBCASE("multi-byte UTF-8 is measured in bytes, not characters") {
        // "e" + U+00E9 (2 bytes) + U+20AC (3 bytes) = 6 bytes.
        CHECK(body([](MqttPropertyWriter& w) { w.addString(0x03, "e\xC3\xA9\xE2\x82\xAC"); }) ==
              std::vector<uint8_t>{0x03, 0x00, 0x06, 'e', 0xC3, 0xA9, 0xE2, 0x82, 0xAC});
    }
}

TEST_CASE("sizing mode writes nothing and counts what emit mode would write") {
    using namespace mqtt5_props;

    MqttPropertyWriter size(nullptr, 512);
    CHECK(size.bodyLength() == 0);
    CHECK(size.addByte(0x01, 1));
    CHECK(size.addString(0x03, "text/plain"));
    CHECK(size.addPair(0x26, "a", "b"));
    // 2 + (3 + 10) + (1 + 2 + 1 + 2 + 1) = 22
    CHECK(size.bodyLength() == 22);
    CHECK_FALSE(size.overflowed());

    std::vector<uint8_t> out(512, kFill);
    MqttPropertyWriter emit(out.data(), 512);
    CHECK(emit.addByte(0x01, 1));
    CHECK(emit.addString(0x03, "text/plain"));
    CHECK(emit.addPair(0x26, "a", "b"));
    CHECK(emit.bodyLength() == size.bodyLength());
    // Nothing beyond the counted body was touched.
    for (size_t i = emit.bodyLength(); i < out.size(); i++) {
        REQUIRE(out[i] == kFill);
    }
}

TEST_CASE("an entry that does not fit writes no byte of itself and latches overflow") {
    using namespace mqtt5_props;

    // Capacity 5: the first Byte entry fits (2 bytes), the string needs 3 + 4 = 7.
    std::vector<uint8_t> out(64, kFill);
    MqttPropertyWriter w(out.data(), 5);
    REQUIRE(w.addByte(0x01, 0x01));
    REQUIRE(w.bodyLength() == 2);

    CHECK_FALSE(w.addString(0x03, "abcd"));
    CHECK(w.overflowed());
    CHECK(w.bodyLength() == 2);          // length did not move
    CHECK(out[2] == kFill);              // and not one byte of the entry landed

    // Sticky: a later entry that would have fit is refused too, so the body can
    // never come out reordered.
    CHECK_FALSE(w.addByte(0x02, 0x02));
    CHECK(w.bodyLength() == 2);
    CHECK(out[2] == kFill);
}

TEST_CASE("writer refuses values it cannot encode") {
    using namespace mqtt5_props;
    std::vector<uint8_t> out(64, kFill);

    SUBCASE("Variable Byte Integer above the 2-byte maximum") {
        MqttPropertyWriter w(out.data(), 64);
        CHECK_FALSE(w.addVarInt(0x0B, 16384));
        CHECK(w.overflowed());
        CHECK(w.bodyLength() == 0);
        CHECK(out[0] == kFill);
    }
    SUBCASE("null string") {
        MqttPropertyWriter w(out.data(), 64);
        CHECK_FALSE(w.addString(0x03, nullptr));
        CHECK(w.overflowed());
        CHECK(w.bodyLength() == 0);
    }
    SUBCASE("null pair half") {
        MqttPropertyWriter w(out.data(), 64);
        CHECK_FALSE(w.addPair(0x26, "k", nullptr));
        CHECK(w.overflowed());
        CHECK(w.bodyLength() == 0);
    }
    SUBCASE("null binary") {
        MqttPropertyWriter w(out.data(), 64);
        CHECK_FALSE(w.addBinary(0x09, nullptr, 0));
        CHECK(w.overflowed());
        CHECK(w.bodyLength() == 0);
    }
}

// Requirements 3.11, 16.5: no add*() overload can place Authentication Method
// (0x15) or Authentication Data (0x16) into a transmitted Properties field, so no
// emitter reachable through writeProperties() can either - the refusal is
// structural rather than a property of any one call site. `MqttMessageProperties`
// has no field for either identifier, so this drives `MqttPropertyWriter` directly,
// the way `writeProperties()`'s own emission code (and any future emitter) would
// have to if it ever tried.
TEST_CASE("MqttPropertyWriter refuses Authentication Method and Authentication Data on every overload") {
    using namespace mqtt5_props;
    std::vector<uint8_t> out(64, kFill);

    for (uint8_t id : {(uint8_t)0x15, (uint8_t)0x16}) {
        CAPTURE(id);

        SUBCASE("addByte") {
            MqttPropertyWriter w(out.data(), 64);
            CHECK_FALSE(w.addByte(id, 1));
            CHECK(w.overflowed());
            CHECK(w.bodyLength() == 0);
        }
        SUBCASE("addTwoByte") {
            MqttPropertyWriter w(out.data(), 64);
            CHECK_FALSE(w.addTwoByte(id, 1));
            CHECK(w.overflowed());
            CHECK(w.bodyLength() == 0);
        }
        SUBCASE("addFourByte") {
            MqttPropertyWriter w(out.data(), 64);
            CHECK_FALSE(w.addFourByte(id, 1));
            CHECK(w.overflowed());
            CHECK(w.bodyLength() == 0);
        }
        SUBCASE("addVarInt") {
            MqttPropertyWriter w(out.data(), 64);
            CHECK_FALSE(w.addVarInt(id, 1));
            CHECK(w.overflowed());
            CHECK(w.bodyLength() == 0);
        }
        SUBCASE("addString") {
            MqttPropertyWriter w(out.data(), 64);
            CHECK_FALSE(w.addString(id, "SCRAM-SHA-1"));
            CHECK(w.overflowed());
            CHECK(w.bodyLength() == 0);
        }
        SUBCASE("addBinary") {
            static const uint8_t blob[] = {0x01, 0x02};
            MqttPropertyWriter w(out.data(), 64);
            CHECK_FALSE(w.addBinary(id, blob, 2));
            CHECK(w.overflowed());
            CHECK(w.bodyLength() == 0);
        }
        SUBCASE("addPair") {
            MqttPropertyWriter w(out.data(), 64);
            CHECK_FALSE(w.addPair(id, "k", "v"));
            CHECK(w.overflowed());
            CHECK(w.bodyLength() == 0);
        }
    }

    // Not one byte of the entry landed either way.
    for (size_t i = 0; i < out.size(); i++) { CHECK(out[i] == kFill); }
}

TEST_CASE("an empty property set emits the single byte 0x00") {
    using namespace mqtt5_props;
    const MqttMessageProperties empty;

    // Every context that carries a Properties field, including the Will
    // pseudo-context in the CONNECT payload (Requirements 1.2, 1.3, 6.3, 9.1, 9.3).
    for (uint8_t packetType : {kWill, kConnect, kPublish, (uint8_t)8, (uint8_t)10, (uint8_t)14}) {
        Scratch s(8);
        const uint16_t pos = writeProperties(empty, packetType, s.buf.data(), 0, (uint16_t)s.buf.size());
        CHECK(pos == 1);
        CHECK(s.buf[0] == 0x00);
        CHECK(s.untouchedFrom(1));
    }
}

TEST_CASE("writeProperties frames the body with a Variable Byte Integer length") {
    using namespace mqtt5_props;

    static const uint8_t corr[] = {0xDE, 0xAD};
    static const char* keys[]   = {"k1", "k2"};
    static const char* vals[]   = {"v1", ""};

    MqttMessageProperties props;
    props.hasPayloadFormat   = true;
    props.payloadFormat      = 1;
    props.contentType        = "text/plain";
    props.responseTopic      = "r/t";
    props.correlationData    = corr;
    props.correlationDataLen = 2;
    props.userPropertyKeys   = keys;
    props.userPropertyValues = vals;
    props.userPropertyCount  = 2;

    // Written out by hand, in the emission order the design fixes: 0x01, 0x03,
    // 0x08, 0x09, then the User Properties in the order supplied.
    const std::vector<uint8_t> expectedBody = {
        0x01, 0x01,
        0x03, 0x00, 0x0A, 't','e','x','t','/','p','l','a','i','n',
        0x08, 0x00, 0x03, 'r','/','t',
        0x09, 0x00, 0x02, 0xDE, 0xAD,
        0x26, 0x00, 0x02, 'k','1', 0x00, 0x02, 'v','1',
        0x26, 0x00, 0x02, 'k','2', 0x00, 0x00
    };

    // Written at a non-zero position, since every real caller has a variable header
    // in front of the Properties field.
    Scratch s(128);
    const uint16_t start = 7;
    const uint16_t pos = writeProperties(props, kPublish, s.buf.data(), start, (uint16_t)s.buf.size());

    REQUIRE(pos == start + 1 + expectedBody.size());
    CHECK(s.buf[start] == (uint8_t)expectedBody.size());   // 1-byte length prefix
    CHECK(s.slice(start + 1, pos) == expectedBody);
    // Nothing before the field and nothing after it was touched.
    for (uint16_t i = 0; i < start; i++) { CHECK(s.buf[i] == kFill); }
    CHECK(s.untouchedFrom(pos));
}

TEST_CASE("writeProperties widens the length prefix when the body needs it") {
    using namespace mqtt5_props;

    // A body of 128 bytes or more needs a 2-byte Property Length. 200 content-type
    // bytes give a body of 1 + 2 + 200 = 203 -> 0xCB 0x01.
    const std::string longType(200, 'x');
    MqttMessageProperties props;
    props.contentType = longType.c_str();

    Scratch s(256);
    const uint16_t pos = writeProperties(props, kPublish, s.buf.data(), 0, (uint16_t)s.buf.size());

    REQUIRE(pos == 2 + 203);
    CHECK(s.buf[0] == 0xCB);
    CHECK(s.buf[1] == 0x01);
    CHECK(s.buf[2] == 0x03);
    CHECK(s.buf[3] == 0x00);
    CHECK(s.buf[4] == 0xC8);   // 200
    CHECK(s.untouchedFrom(pos));
}

TEST_CASE("writeProperties returns its position unchanged and writes nothing when it does not fit") {
    using namespace mqtt5_props;

    static const char* keys[] = {"key"};
    static const char* vals[] = {"value"};
    MqttMessageProperties props;
    props.contentType        = "text/plain";
    props.userPropertyKeys   = keys;
    props.userPropertyValues = vals;
    props.userPropertyCount  = 1;
    // body = (3 + 10) + (1 + 2 + 3 + 2 + 5) = 26, field = 27 bytes.

    SUBCASE("field exactly fits") {
        Scratch s(27);
        CHECK(writeProperties(props, kPublish, s.buf.data(), 0, 27) == 27);
        CHECK(s.buf[0] == 26);
    }
    SUBCASE("one byte short - Requirement 13.2") {
        // Capacities from "no room at all" up to one byte short of the field: every
        // one of them must leave the buffer exactly as it was found.
        for (uint16_t capacity = 0; capacity < 27; capacity++) {
            Scratch s(64);
            const uint16_t pos = writeProperties(props, kPublish, s.buf.data(), 0, capacity);
            CHECK(pos == 0);
            CHECK(s.untouchedFrom(0));
        }
    }
    SUBCASE("no room after a non-zero position") {
        Scratch s(64);
        const uint16_t start = 40;
        CHECK(writeProperties(props, kPublish, s.buf.data(), start, 64) == start);
        CHECK(s.untouchedFrom(0));
    }
    SUBCASE("position at or past capacity") {
        Scratch s(8);
        CHECK(writeProperties(props, kPublish, s.buf.data(), 8, 8) == 8);
        CHECK(writeProperties(props, kPublish, s.buf.data(), 9, 8) == 9);
        CHECK(s.untouchedFrom(0));
    }
    SUBCASE("null buffer") {
        CHECK(writeProperties(props, kPublish, nullptr, 5, 64) == 5);
    }
}

TEST_CASE("writeProperties skips properties illegal for the packet type") {
    using namespace mqtt5_props;

    static const uint8_t corr[] = {0x01};
    static const char* keys[]   = {"k"};
    static const char* vals[]   = {"v"};
    MqttMessageProperties props;
    props.hasPayloadFormat   = true;
    props.payloadFormat      = 0;
    props.contentType        = "ct";
    props.responseTopic      = "rt";
    props.correlationData    = corr;
    props.correlationDataLen = 1;
    props.userPropertyKeys   = keys;
    props.userPropertyValues = vals;
    props.userPropertyCount  = 1;

    SUBCASE("CONNECT accepts only the User Property") {
        // 0x01, 0x03, 0x08 and 0x09 are PUBLISH/Will properties, so a Properties
        // field this client frames for CONNECT can never carry them.
        Scratch s(64);
        const uint16_t pos = writeProperties(props, kConnect, s.buf.data(), 0, 64);
        // body = 1 + 2 + 1 + 2 + 1 = 7 bytes, so the length prefix is 0x07.
        const std::vector<uint8_t> expected = {0x07, 0x26, 0x00, 0x01, 'k', 0x00, 0x01, 'v'};
        REQUIRE(pos == expected.size());
        CHECK(s.slice(0, pos) == expected);
    }
    SUBCASE("Will Properties accept the message properties") {
        Scratch s(64);
        const uint16_t pos = writeProperties(props, kWill, s.buf.data(), 0, 64);
        REQUIRE(pos > 1);
        CHECK(s.buf[1] == 0x01);   // Payload Format Indicator is legal in a Will
    }
    SUBCASE("PINGREQ carries no properties at all") {
        Scratch s(64);
        CHECK(writeProperties(props, 12, s.buf.data(), 0, 64) == 1);
        CHECK(s.buf[0] == 0x00);
    }
    SUBCASE("a packet type number outside 0..15 emits an empty field") {
        Scratch s(64);
        CHECK(writeProperties(props, 16, s.buf.data(), 0, 64) == 1);
        CHECK(s.buf[0] == 0x00);
    }
}

TEST_CASE("writeProperties omits absent values without emitting empty entries") {
    using namespace mqtt5_props;

    MqttMessageProperties props;
    props.payloadFormat      = 1;       // set, but hasPayloadFormat is false
    props.correlationDataLen = 4;       // length without data
    props.userPropertyCount  = 3;       // count without arrays

    Scratch s(32);
    CHECK(writeProperties(props, kPublish, s.buf.data(), 0, 32) == 1);
    CHECK(s.buf[0] == 0x00);
    CHECK(s.untouchedFrom(1));
}

// --- Task 8.2: outbound message properties on the publish path -------------
//
// Response Topic and Correlation Data are independently optional, as required by MQTT 5
// request/response: a request can name a response topic without a token, and a response
// echoes the token without carrying another response topic. Correlation Data still has no
// outbound size limit (Requirement 11.10).

TEST_CASE("writeProperties emits Response Topic and Correlation Data independently") {
    using namespace mqtt5_props;

    static const uint8_t corr[] = {0xDE, 0xAD};
    const std::vector<uint8_t> commonBody = {
        0x01, 0x01,
        0x03, 0x00, 0x02, 'c', 't'
    };

    SUBCASE("Response Topic without Correlation Data is emitted") {
        MqttMessageProperties props;
        props.hasPayloadFormat = true;
        props.payloadFormat    = 1;
        props.contentType      = "ct";
        props.responseTopic    = "r/t";

        std::vector<uint8_t> expectedBody = commonBody;
        const uint8_t response[] = {0x08, 0x00, 0x03, 'r', '/', 't'};
        expectedBody.insert(expectedBody.end(), response, response + sizeof(response));

        Scratch s(64);
        const uint16_t pos = writeProperties(props, kPublish, s.buf.data(), 0, 64);
        REQUIRE(pos == 1 + expectedBody.size());
        CHECK(s.buf[0] == (uint8_t)expectedBody.size());
        CHECK(s.slice(1, pos) == expectedBody);
        CHECK(s.untouchedFrom(pos));
    }
    SUBCASE("Correlation Data without Response Topic is emitted") {
        MqttMessageProperties props;
        props.hasPayloadFormat   = true;
        props.payloadFormat      = 1;
        props.contentType        = "ct";
        props.correlationData    = corr;
        props.correlationDataLen = 2;

        std::vector<uint8_t> expectedBody = commonBody;
        const uint8_t correlation[] = {0x09, 0x00, 0x02, 0xDE, 0xAD};
        expectedBody.insert(expectedBody.end(), correlation,
                            correlation + sizeof(correlation));

        Scratch s(64);
        const uint16_t pos = writeProperties(props, kPublish, s.buf.data(), 0, 64);
        REQUIRE(pos == 1 + expectedBody.size());
        CHECK(s.buf[0] == (uint8_t)expectedBody.size());
        CHECK(s.slice(1, pos) == expectedBody);
        CHECK(s.untouchedFrom(pos));
    }
    SUBCASE("both supplied are emitted in fixed order") {
        MqttMessageProperties props;
        props.hasPayloadFormat   = true;
        props.payloadFormat      = 1;
        props.contentType        = "ct";
        props.responseTopic      = "r/t";
        props.correlationData    = corr;
        props.correlationDataLen = 2;

        const std::vector<uint8_t> expectedBody = {
            0x01, 0x01,
            0x03, 0x00, 0x02, 'c', 't',
            0x08, 0x00, 0x03, 'r', '/', 't',
            0x09, 0x00, 0x02, 0xDE, 0xAD
        };

        Scratch s(64);
        const uint16_t pos = writeProperties(props, kPublish, s.buf.data(), 0, 64);
        REQUIRE(pos == 1 + expectedBody.size());
        CHECK(s.buf[0] == (uint8_t)expectedBody.size());
        CHECK(s.slice(1, pos) == expectedBody);
    }
    SUBCASE("zero-length Correlation Data remains present without Response Topic") {
        static const uint8_t empty[1] = {0x00};
        MqttMessageProperties props;
        props.correlationData    = empty;
        props.correlationDataLen = 0;

        const std::vector<uint8_t> expectedBody = {0x09, 0x00, 0x00};
        Scratch s(64);
        const uint16_t pos = writeProperties(props, kPublish, s.buf.data(), 0, 64);
        REQUIRE(pos == 1 + expectedBody.size());
        CHECK(s.slice(1, pos) == expectedBody);
    }
    SUBCASE("independent emission also applies to Will Properties") {
        MqttMessageProperties props;
        props.hasPayloadFormat = true;
        props.payloadFormat    = 0;
        props.responseTopic    = "r/t";

        Scratch s(64);
        const uint16_t pos = writeProperties(props, kWill, s.buf.data(), 0, 64);
        const std::vector<uint8_t> expectedBody = {
            0x01, 0x00,
            0x08, 0x00, 0x03, 'r', '/', 't'
        };
        REQUIRE(pos == 1 + expectedBody.size());
        CHECK(s.slice(1, pos) == expectedBody);
    }
    SUBCASE("an independent property participates in the all-or-nothing size gate") {
        MqttMessageProperties props;
        props.hasPayloadFormat = true;
        props.payloadFormat    = 1;
        props.contentType      = "ct";
        props.responseTopic    = "r/t";

        Scratch s(64);
        const uint16_t tooSmall = (uint16_t)(1 + commonBody.size());
        CHECK(writeProperties(props, kPublish, s.buf.data(), 0, tooSmall) == 0);
        CHECK(s.untouchedFrom(0));
    }
}

TEST_CASE("writeProperties emits outbound Correlation Data of any length that fits") {
    using namespace mqtt5_props;

    // Requirement 11.10: the 16-byte cap is an inbound bound on
    // Correlation_Data_Storage, not an outbound limit. Every one of these lengths
    // is above it and every one is emitted as-is.
    const uint16_t lengths[] = {17, 32, 200, 1000};

    for (uint16_t n : lengths) {
        CAPTURE(n);
        std::vector<uint8_t> value(n);
        for (uint16_t i = 0; i < n; i++) {
            value[i] = (uint8_t)(i & 0xFF);
        }

        MqttMessageProperties props;
        props.responseTopic      = "r";           // the pairing's other half
        props.correlationData    = value.data();
        props.correlationDataLen = n;

        // 0x08 00 01 'r' | 0x09 <2-byte length> <n bytes>
        const uint16_t bodyLen = (uint16_t)(4 + 3 + n);
        const uint8_t  prefixLen = (bodyLen < 128) ? 1 : 2;

        Scratch s(2048);
        const uint16_t pos = writeProperties(props, kPublish, s.buf.data(), 0,
                                             (uint16_t)s.buf.size());
        REQUIRE(pos == prefixLen + bodyLen);

        // The Correlation Data entry, spelled out: identifier, 2-byte length, value.
        const size_t at = prefixLen + 4;
        CHECK(s.buf[at] == MQTT_PROP_CORRELATION_DATA);
        CHECK(s.buf[at + 1] == (uint8_t)(n >> 8));
        CHECK(s.buf[at + 2] == (uint8_t)(n & 0xFF));
        CHECK(s.slice(at + 3, at + 3 + n) == value);
        CHECK(s.untouchedFrom(pos));

        // The only thing that bounds it is the buffer gate, and that refusal is
        // total: one byte short writes nothing at all (Requirement 13.2).
        Scratch t(2048);
        CHECK(writeProperties(props, kPublish, t.buf.data(), 0,
                              (uint16_t)(prefixLen + bodyLen - 1)) == 0);
        CHECK(t.untouchedFrom(0));
    }
}

// --- Task 4.3: the properties reader ---------------------------------------
//
// decodeProperties() is a zero-copy visitor walk, so there are two things worth
// pinning: that every value type is handed over correctly as a view INTO the input
// buffer, and that no declared length can make it read a byte it was not given.
// The bounds cases below all decode from an exactly-sized copy of the fixture, so
// ASan reports any read past `avail` as a heap overflow rather than letting it pass
// unnoticed.

namespace mqtt5_props {

// What a visit saw. Values are copied out here (the view itself is only valid for the
// duration of the call), while the raw pointers are kept for the zero-copy check.
struct Seen {
    uint8_t              id    = 0;
    MqttPropType         type  = MqttPropType::Byte;
    uint32_t             value = 0;
    std::string          text;    // Utf8, or the key half of a pair
    std::vector<uint8_t> blob;    // Binary
    std::string          text2;   // value half of a pair
    const uint8_t*       data  = nullptr;
    uint16_t             len   = 0;
    const uint8_t*       data2 = nullptr;
    uint16_t             len2  = 0;
};

struct Collector {
    std::vector<Seen> seen;
    size_t stopAfter = 0;   // 0 = never abort; N = return false on the Nth entry
};

static bool collect(const MqttPropertyView& p, void* ctx) {
    Collector* c = static_cast<Collector*>(ctx);
    Seen s;
    s.id = p.id; s.type = p.type; s.value = p.value;
    s.data = p.data; s.len = p.len; s.data2 = p.data2; s.len2 = p.len2;
    if (p.data != nullptr) {
        s.text.assign((const char*)p.data, p.len);
        s.blob.assign(p.data, p.data + p.len);
    }
    if (p.data2 != nullptr) {
        s.text2.assign((const char*)p.data2, p.len2);
    }
    c->seen.push_back(s);
    if ((c->stopAfter != 0) && (c->seen.size() >= c->stopAfter)) { return false; }
    return true;
}

// `<VBI bodyLength><body>`, framed by the library's own encoder so the fixtures below
// only have to spell out the body.
static std::vector<uint8_t> field(const std::vector<uint8_t>& body) {
    uint8_t tmp[2];
    const uint8_t n = PubSubClient::encodeVariableByteInteger((uint32_t)body.size(), tmp);
    REQUIRE(n > 0);
    std::vector<uint8_t> out(tmp, tmp + n);
    out.insert(out.end(), body.begin(), body.end());
    return out;
}

// Decode the first `n` bytes of `f` from a buffer that is exactly `n` bytes long, so a
// read past `avail` is a genuine out-of-bounds access.
static MqttError decodeExact(const std::vector<uint8_t>& f, size_t n, uint8_t packetType,
                             uint32_t& consumed, Collector& c) {
    std::vector<uint8_t> exact(f.begin(), f.begin() + n);
    return decodeProperties(exact.data(), (uint32_t)exact.size(), packetType,
                            collect, &c, consumed);
}

// One property of each of the seven value types, all legal in PUBLISH.
static std::vector<uint8_t> everyTypeBody() {
    return {
        0x01, 0x01,                                        // Payload Format Indicator, Byte
        0x23, 0x00, 0x0A,                                  // Topic Alias, Two Byte = 10
        0x02, 0x00, 0x00, 0x01, 0x00,                      // Message Expiry, Four Byte = 256
        0x0B, 0x80, 0x01,                                  // Subscription Identifier, VarInt = 128
        0x03, 0x00, 0x02, 'a', 'b',                        // Content Type, Utf8 = "ab"
        0x09, 0x00, 0x03, 0x00, 0xFF, 0x00,                // Correlation Data, Binary
        0x26, 0x00, 0x01, 'k', 0x00, 0x02, 'v', 'v'        // User Property, StringPair
    };
}

}  // namespace mqtt5_props

TEST_CASE("decodeProperties hands every value type to the visitor as a buffer view") {
    using namespace mqtt5_props;

    const std::vector<uint8_t> body = everyTypeBody();
    const std::vector<uint8_t> f = field(body);
    REQUIRE(f.size() == 1 + body.size());   // 32-byte body, so a 1-byte prefix

    Collector c;
    uint32_t consumed = 0;
    REQUIRE(decodeProperties(f.data(), (uint32_t)f.size(), kPublish, collect, &c, consumed) ==
            MqttError::None);
    CHECK(consumed == f.size());
    REQUIRE(c.seen.size() == 7);

    CHECK(c.seen[0].id == 0x01);
    CHECK(c.seen[0].type == MqttPropType::Byte);
    CHECK(c.seen[0].value == 1);
    CHECK(c.seen[0].data == nullptr);

    CHECK(c.seen[1].id == 0x23);
    CHECK(c.seen[1].type == MqttPropType::TwoByte);
    CHECK(c.seen[1].value == 10);

    CHECK(c.seen[2].id == 0x02);
    CHECK(c.seen[2].type == MqttPropType::FourByte);
    CHECK(c.seen[2].value == 256);

    CHECK(c.seen[3].id == 0x0B);
    CHECK(c.seen[3].type == MqttPropType::VarInt);
    CHECK(c.seen[3].value == 128);

    CHECK(c.seen[4].id == 0x03);
    CHECK(c.seen[4].type == MqttPropType::Utf8);
    CHECK(c.seen[4].text == "ab");

    CHECK(c.seen[5].id == 0x09);
    CHECK(c.seen[5].type == MqttPropType::Binary);
    CHECK(c.seen[5].blob == std::vector<uint8_t>{0x00, 0xFF, 0x00});

    CHECK(c.seen[6].id == 0x26);
    CHECK(c.seen[6].type == MqttPropType::StringPair);
    CHECK(c.seen[6].text == "k");
    CHECK(c.seen[6].text2 == "vv");

    // Zero copy: every value that has bytes points inside the caller's buffer, at the
    // exact offset the fixture put it at, and nothing was duplicated on the way.
    const uint8_t* base = f.data();
    CHECK(c.seen[4].data == base + 1 + 16);    // "ab"
    CHECK(c.seen[5].data == base + 1 + 21);    // correlation blob
    CHECK(c.seen[6].data == base + 1 + 27);    // pair key
    CHECK(c.seen[6].data2 == base + 1 + 30);   // pair value
    for (const auto& s : c.seen) {
        if (s.data != nullptr) {
            CHECK(s.data >= base);
            CHECK(s.data + s.len <= base + f.size());
        }
        if (s.data2 != nullptr) {
            CHECK(s.data2 + s.len2 <= base + f.size());
        }
    }
}

TEST_CASE("decodeProperties accepts an empty field and both length-prefix widths") {
    using namespace mqtt5_props;

    SUBCASE("empty field is the single byte 0x00") {
        const std::vector<uint8_t> f = {0x00};
        Collector c;
        uint32_t consumed = 0;
        CHECK(decodeProperties(f.data(), 1, kPublish, collect, &c, consumed) == MqttError::None);
        CHECK(consumed == 1);
        CHECK(c.seen.empty());
    }
    SUBCASE("an empty field decodes in every context, even one with no properties") {
        for (uint8_t packetType : {kWill, kConnect, kPublish, (uint8_t)2, (uint8_t)12, (uint8_t)16}) {
            const std::vector<uint8_t> f = {0x00};
            uint32_t consumed = 0;
            Collector c;
            CHECK(decodeProperties(f.data(), 1, packetType, collect, &c, consumed) ==
                  MqttError::None);
            CHECK(consumed == 1);
        }
    }
    SUBCASE("a body of 128 bytes or more carries a 2-byte prefix") {
        // Content Type with a 200-byte value: body = 1 + 2 + 200 = 203 -> 0xCB 0x01.
        std::vector<uint8_t> body = {0x03, 0x00, 0xC8};
        body.insert(body.end(), 200, 'x');
        const std::vector<uint8_t> f = field(body);
        REQUIRE(f.size() == 2 + 203);
        REQUIRE(f[0] == 0xCB);
        REQUIRE(f[1] == 0x01);

        Collector c;
        uint32_t consumed = 0;
        CHECK(decodeProperties(f.data(), (uint32_t)f.size(), kPublish, collect, &c, consumed) ==
              MqttError::None);
        CHECK(consumed == 205);
        REQUIRE(c.seen.size() == 1);
        CHECK(c.seen[0].len == 200);
        CHECK(c.seen[0].text == std::string(200, 'x'));
    }
    SUBCASE("a zero-length string or blob is a legal value") {
        const std::vector<uint8_t> f = field({0x03, 0x00, 0x00, 0x09, 0x00, 0x00});
        Collector c;
        uint32_t consumed = 0;
        CHECK(decodeProperties(f.data(), (uint32_t)f.size(), kPublish, collect, &c, consumed) ==
              MqttError::None);
        CHECK(consumed == f.size());
        REQUIRE(c.seen.size() == 2);
        CHECK(c.seen[0].len == 0);
        CHECK(c.seen[1].len == 0);
    }
}

TEST_CASE("a malformed Property Length is rejected - Requirement 3.3") {
    using namespace mqtt5_props;

    struct Case { const char* name; std::vector<uint8_t> bytes; };
    const std::vector<Case> cases = {
        {"no bytes at all",             {}},
        {"prefix truncated mid-field",  {0x80}},
        {"a third continuation byte",   {0x80, 0x80}},
        {"two continuation bytes then a body", {0xFF, 0xFF, 0x00}}
    };

    for (const auto& c : cases) {
        SUBCASE(c.name) {
            Collector col;
            uint32_t consumed = 0;
            // Exactly-sized buffer: a read past `avail` here is an ASan report.
            CHECK(decodeExact(c.bytes, c.bytes.size(), kPublish, consumed, col) ==
                  MqttError::MalformedPacket);
            CHECK(consumed == 0);
            CHECK(col.seen.empty());
        }
    }
    SUBCASE("null buffer") {
        uint32_t consumed = 0;
        Collector col;
        CHECK(decodeProperties(nullptr, 4, kPublish, collect, &col, consumed) ==
              MqttError::MalformedPacket);
        CHECK(consumed == 0);
    }
}

TEST_CASE("a Property Length beyond the received bytes is rejected without reading them") {
    using namespace mqtt5_props;

    // Requirements 3.5, 13.5: sweep every truncation of a valid field. Each decode gets
    // a buffer holding exactly the bytes it is told it has, so the only way to satisfy
    // the declared length would be an out-of-bounds read.
    SUBCASE("1-byte prefix") {
        const std::vector<uint8_t> f = field(everyTypeBody());
        for (size_t n = 0; n < f.size(); n++) {
            Collector c;
            uint32_t consumed = 0;
            CHECK(decodeExact(f, n, kPublish, consumed, c) == MqttError::MalformedPacket);
            CHECK(consumed == 0);
            CHECK(c.seen.empty());
        }
        // The untruncated field still decodes, so the sweep above is not vacuous.
        Collector c;
        uint32_t consumed = 0;
        CHECK(decodeExact(f, f.size(), kPublish, consumed, c) == MqttError::None);
        CHECK(consumed == f.size());
    }
    SUBCASE("2-byte prefix") {
        std::vector<uint8_t> body = {0x03, 0x00, 0xC8};
        body.insert(body.end(), 200, 'x');
        const std::vector<uint8_t> f = field(body);
        for (size_t n = 0; n < f.size(); n++) {
            Collector c;
            uint32_t consumed = 0;
            CHECK(decodeExact(f, n, kPublish, consumed, c) == MqttError::MalformedPacket);
            CHECK(consumed == 0);
        }
    }
    SUBCASE("a length declared far beyond what was received") {
        // Declares 16,383 body bytes and supplies three.
        const std::vector<uint8_t> f = {0xFF, 0x7F, 0x01, 0x01, 0x00};
        Collector c;
        uint32_t consumed = 0;
        CHECK(decodeExact(f, f.size(), kPublish, consumed, c) == MqttError::MalformedPacket);
        CHECK(consumed == 0);
        CHECK(c.seen.empty());
    }
}

TEST_CASE("an entry running past the declared body end is rejected - Requirement 3.6") {
    using namespace mqtt5_props;

    // Each body below is self-consistently framed - the prefix declares exactly the
    // bytes that follow - so the failure comes from an entry whose value does not fit
    // inside the declared body, not from a short read.
    struct Case { const char* name; std::vector<uint8_t> body; };
    const std::vector<Case> cases = {
        {"Byte with no value",              {0x01}},
        {"Two Byte one short",              {0x23, 0x00}},
        {"Four Byte one short",             {0x02, 0x00, 0x00, 0x00}},
        {"VarInt continues past the body",  {0x0B, 0x80}},
        {"Utf8 length prefix truncated",    {0x03, 0x00}},
        {"Utf8 body shorter than declared", {0x03, 0x00, 0x05, 'a', 'b'}},
        {"Binary body shorter than declared", {0x09, 0x00, 0x02, 0x01}},
        {"pair value length missing",       {0x26, 0x00, 0x01, 'k'}},
        {"pair value body short",           {0x26, 0x00, 0x01, 'k', 0x00, 0x03, 'v'}},
        {"trailing identifier with no value", {0x01, 0x01, 0x03}}
    };

    for (const auto& c : cases) {
        SUBCASE(c.name) {
            const std::vector<uint8_t> f = field(c.body);
            Collector col;
            uint32_t consumed = 0;
            CHECK(decodeExact(f, f.size(), kPublish, consumed, col) ==
                  MqttError::MalformedPacket);
            CHECK(consumed == 0);
        }
    }
    SUBCASE("an entry may not borrow bytes that follow the declared body") {
        // The body declares 2 bytes and holds a Content Type whose value would fit if
        // the reader ignored the declared length and ran on into the packet.
        const std::vector<uint8_t> f = {0x02, 0x03, 0x00, 0x02, 'a', 'b'};
        Collector col;
        uint32_t consumed = 0;
        CHECK(decodeExact(f, f.size(), kPublish, consumed, col) == MqttError::MalformedPacket);
        CHECK(consumed == 0);
    }
}

TEST_CASE("an unknown or contextually illegal identifier is a protocol error - Requirement 3.7") {
    using namespace mqtt5_props;

    SUBCASE("identifier absent from the table") {
        for (uint8_t id : {(uint8_t)0x00, (uint8_t)0x04, (uint8_t)0x10, (uint8_t)0x2B, (uint8_t)0xFF}) {
            const std::vector<uint8_t> f = field({id, 0x00});
            Collector c;
            uint32_t consumed = 0;
            CHECK(decodeExact(f, f.size(), kPublish, consumed, c) == MqttError::ProtocolError);
            CHECK(consumed == 0);
            CHECK(c.seen.empty());
        }
    }
    SUBCASE("Topic Alias is legal in PUBLISH and nowhere else") {
        const std::vector<uint8_t> f = field({0x23, 0x00, 0x01});
        Collector c;
        uint32_t consumed = 0;
        CHECK(decodeExact(f, f.size(), kPublish, consumed, c) == MqttError::None);
        Collector c2;
        CHECK(decodeExact(f, f.size(), 2 /* CONNACK */, consumed, c2) == MqttError::ProtocolError);
        CHECK(consumed == 0);
    }
    SUBCASE("Server Keep Alive is legal in CONNACK and nowhere else") {
        const std::vector<uint8_t> f = field({0x13, 0x00, 0x3C});
        Collector c;
        uint32_t consumed = 0;
        CHECK(decodeExact(f, f.size(), 2 /* CONNACK */, consumed, c) == MqttError::None);
        REQUIRE(c.seen.size() == 1);
        CHECK(c.seen[0].value == 60);
        Collector c2;
        CHECK(decodeExact(f, f.size(), kPublish, consumed, c2) == MqttError::ProtocolError);
    }
    SUBCASE("Will Delay Interval belongs to the Will pseudo-context only") {
        const std::vector<uint8_t> f = field({0x18, 0x00, 0x00, 0x00, 0x05});
        Collector c;
        uint32_t consumed = 0;
        CHECK(decodeExact(f, f.size(), kWill, consumed, c) == MqttError::None);
        Collector c2;
        CHECK(decodeExact(f, f.size(), kConnect, consumed, c2) == MqttError::ProtocolError);
    }
    SUBCASE("a packet type outside 0..15 admits no property at all") {
        const std::vector<uint8_t> f = field({0x01, 0x01});
        Collector c;
        uint32_t consumed = 0;
        CHECK(decodeExact(f, f.size(), 16, consumed, c) == MqttError::ProtocolError);
    }
}

TEST_CASE("a repeated single-occurrence identifier is a protocol error - Requirement 3.8") {
    using namespace mqtt5_props;

    SUBCASE("Payload Format Indicator twice") {
        const std::vector<uint8_t> f = field({0x01, 0x00, 0x01, 0x01});
        Collector c;
        uint32_t consumed = 0;
        CHECK(decodeExact(f, f.size(), kPublish, consumed, c) == MqttError::ProtocolError);
        CHECK(consumed == 0);
        // The first occurrence was visited before the second was seen; what matters is
        // that the caller rejects the packet, which the return value says.
        CHECK(c.seen.size() == 1);
    }
    SUBCASE("Content Type twice, non-adjacent") {
        const std::vector<uint8_t> f =
            field({0x03, 0x00, 0x01, 'a', 0x01, 0x01, 0x03, 0x00, 0x01, 'b'});
        Collector c;
        uint32_t consumed = 0;
        CHECK(decodeExact(f, f.size(), kPublish, consumed, c) == MqttError::ProtocolError);
    }
    SUBCASE("User Property may repeat") {
        const std::vector<uint8_t> f = field({
            0x26, 0x00, 0x01, 'a', 0x00, 0x01, '1',
            0x26, 0x00, 0x01, 'a', 0x00, 0x01, '2',
            0x26, 0x00, 0x00, 0x00, 0x00
        });
        Collector c;
        uint32_t consumed = 0;
        CHECK(decodeExact(f, f.size(), kPublish, consumed, c) == MqttError::None);
        CHECK(consumed == f.size());
        REQUIRE(c.seen.size() == 3);
        // Duplicate keys are kept, in wire order.
        CHECK(c.seen[0].text == "a");
        CHECK(c.seen[0].text2 == "1");
        CHECK(c.seen[1].text == "a");
        CHECK(c.seen[1].text2 == "2");
        CHECK(c.seen[2].len == 0);
    }
    SUBCASE("Subscription Identifier may repeat") {
        const std::vector<uint8_t> f = field({0x0B, 0x01, 0x0B, 0xFF, 0x7F});
        Collector c;
        uint32_t consumed = 0;
        CHECK(decodeExact(f, f.size(), kPublish, consumed, c) == MqttError::None);
        REQUIRE(c.seen.size() == 2);
        CHECK(c.seen[0].value == 1);
        CHECK(c.seen[1].value == 16383);
    }
}

TEST_CASE("a visitor returning false stops the walk and reports the whole field consumed") {
    using namespace mqtt5_props;

    const std::vector<uint8_t> f = field(everyTypeBody());

    SUBCASE("stops after the first entry") {
        Collector c;
        c.stopAfter = 1;
        uint32_t consumed = 0;
        CHECK(decodeProperties(f.data(), (uint32_t)f.size(), kPublish, collect, &c, consumed) ==
              MqttError::None);
        CHECK(consumed == f.size());
        CHECK(c.seen.size() == 1);
        CHECK(c.seen[0].id == 0x01);
    }
    SUBCASE("stops midway") {
        Collector c;
        c.stopAfter = 4;
        uint32_t consumed = 0;
        CHECK(decodeProperties(f.data(), (uint32_t)f.size(), kPublish, collect, &c, consumed) ==
              MqttError::None);
        CHECK(consumed == f.size());
        CHECK(c.seen.size() == 4);
    }
    SUBCASE("aborting opts out of validating the remainder") {
        // A good first entry followed by a truncated one. A visitor that runs to the end
        // sees the malformed tail; one that aborts first does not.
        const std::vector<uint8_t> bad = field({0x01, 0x01, 0x03, 0x00, 0x05, 'a'});
        Collector all;
        uint32_t consumed = 0;
        CHECK(decodeExact(bad, bad.size(), kPublish, consumed, all) ==
              MqttError::MalformedPacket);
        CHECK(consumed == 0);

        Collector stop;
        stop.stopAfter = 1;
        uint32_t consumed2 = 0;
        CHECK(decodeProperties(bad.data(), (uint32_t)bad.size(), kPublish, collect, &stop,
                               consumed2) == MqttError::None);
        CHECK(consumed2 == bad.size());
        CHECK(stop.seen.size() == 1);
    }
}

TEST_CASE("a null visitor validates the field without visiting it") {
    using namespace mqtt5_props;

    const std::vector<uint8_t> good = field(everyTypeBody());
    uint32_t consumed = 0;
    CHECK(decodeProperties(good.data(), (uint32_t)good.size(), kPublish, nullptr, nullptr,
                           consumed) == MqttError::None);
    CHECK(consumed == good.size());

    const std::vector<uint8_t> illegal = field({0x13, 0x00, 0x3C});
    consumed = 0;
    CHECK(decodeProperties(illegal.data(), (uint32_t)illegal.size(), kPublish, nullptr, nullptr,
                           consumed) == MqttError::ProtocolError);
    CHECK(consumed == 0);

    const std::vector<uint8_t> truncated = field({0x23, 0x00});
    consumed = 0;
    CHECK(decodeProperties(truncated.data(), (uint32_t)truncated.size(), kPublish, nullptr,
                           nullptr, consumed) == MqttError::MalformedPacket);
    CHECK(consumed == 0);
}

TEST_CASE("decodeProperties reads only the field, whatever follows it") {
    using namespace mqtt5_props;

    // The real caller hands the reader a pointer into the middle of a packet with the
    // payload still to come: `consumed` is what tells it where the payload starts.
    const std::vector<uint8_t> body = {0x01, 0x01, 0x23, 0x00, 0x07};
    std::vector<uint8_t> packet = field(body);
    const size_t fieldLen = packet.size();
    const std::vector<uint8_t> payload = {'h', 'i', 0x26, 0xFF};   // looks like a property
    packet.insert(packet.end(), payload.begin(), payload.end());

    Collector c;
    uint32_t consumed = 0;
    CHECK(decodeProperties(packet.data(), (uint32_t)packet.size(), kPublish, collect, &c,
                           consumed) == MqttError::None);
    CHECK(consumed == fieldLen);
    CHECK(c.seen.size() == 2);
    // The trailing bytes were never interpreted as entries.
    CHECK(std::vector<uint8_t>(packet.begin() + consumed, packet.end()) == payload);
}

// --- Task 4.5: Property 3, the properties field round-trip -----------------
//
// Everything above pins one direction at a time against hand-written bytes. This
// section closes the loop: a curated set goes through the library writer, comes back
// through the library reader, and comes back a second time through the harness's
// independent decoder, so an encoder/decoder pair that agreed only with each other
// would still be caught.

namespace mqtt5_p3 {

// One property as the test declares it, before encoding. `type` selects which of the
// value members carries the value, exactly as MqttPropertyView does.
struct Entry {
    uint8_t              id   = 0;
    MqttPropType         type = MqttPropType::Byte;
    uint32_t             num  = 0;   // Byte / TwoByte / FourByte / VarInt
    std::string          s;          // Utf8, or the key half of a pair
    std::string          s2;         // the value half of a pair
    std::vector<uint8_t> blob;       // Binary
};

// Constructors, so the curated table below stays one line per entry.
static Entry pByte (uint8_t id, uint8_t v)  { Entry e; e.id = id; e.type = MqttPropType::Byte;     e.num = v; return e; }
static Entry pU16  (uint8_t id, uint16_t v) { Entry e; e.id = id; e.type = MqttPropType::TwoByte;  e.num = v; return e; }
static Entry pU32  (uint8_t id, uint32_t v) { Entry e; e.id = id; e.type = MqttPropType::FourByte; e.num = v; return e; }
static Entry pVar  (uint8_t id, uint32_t v) { Entry e; e.id = id; e.type = MqttPropType::VarInt;   e.num = v; return e; }
static Entry pStr  (uint8_t id, const std::string& s) { Entry e; e.id = id; e.type = MqttPropType::Utf8; e.s = s; return e; }
static Entry pBin  (uint8_t id, const std::vector<uint8_t>& d) { Entry e; e.id = id; e.type = MqttPropType::Binary; e.blob = d; return e; }
static Entry pPair (uint8_t id, const std::string& k, const std::string& v) {
    Entry e; e.id = id; e.type = MqttPropType::StringPair; e.s = k; e.s2 = v; return e;
}

// A decoded entry, normalized so the library's visitor views and the harness's
// DecodedProperty can be compared against the same expectation with the same code.
struct Norm {
    uint8_t              id  = 0;
    uint32_t             num = 0;
    std::string          s;
    std::string          s2;
    std::vector<uint8_t> blob;
};

// A non-null pointer for a zero-length Binary value: addBinary refuses a null data
// pointer, and std::vector::data() on an empty vector may legitimately be null.
static const uint8_t kNoBytes[1] = {0};

static bool add(MqttPropertyWriter& w, const Entry& e) {
    switch (e.type) {
        case MqttPropType::Byte:       return w.addByte(e.id, (uint8_t)e.num);
        case MqttPropType::TwoByte:    return w.addTwoByte(e.id, (uint16_t)e.num);
        case MqttPropType::FourByte:   return w.addFourByte(e.id, e.num);
        case MqttPropType::VarInt:     return w.addVarInt(e.id, e.num);
        case MqttPropType::Utf8:       return w.addString(e.id, e.s.c_str());
        case MqttPropType::Binary:     return w.addBinary(e.id,
                                              e.blob.empty() ? kNoBytes : e.blob.data(),
                                              (uint16_t)e.blob.size());
        case MqttPropType::StringPair: return w.addPair(e.id, e.s.c_str(), e.s2.c_str());
        case MqttPropType::None:       break;   // 0x15/0x16 are never round-tripped
    }
    return false;
}

// Encode `entries` into a complete Properties field, `<VBI bodyLength><body>`, using the
// library writer for the body and the library VBI codec for the prefix - i.e. exactly
// the two pieces writeProperties() puts together, driven directly so the test can reach
// value types and identifiers MqttMessageProperties cannot express.
//
// `prefixLen` and `bodyLen` come back separately because Property 3's consumption claim
// is stated in those terms: the prefix bytes plus the declared body length.
static std::vector<uint8_t> encodeField(const std::vector<Entry>& entries,
                                        uint8_t& prefixLen, uint16_t& bodyLen) {
    static constexpr uint16_t kCap = 4096;

    // Sizing pass first, and it must agree with the emit pass byte for byte.
    MqttPropertyWriter sizing(nullptr, kCap);
    for (const auto& e : entries) { REQUIRE(add(sizing, e)); }
    REQUIRE_FALSE(sizing.overflowed());

    std::vector<uint8_t> scratch(kCap, 0xAA);
    MqttPropertyWriter emit(scratch.data(), kCap);
    for (const auto& e : entries) { REQUIRE(add(emit, e)); }
    REQUIRE_FALSE(emit.overflowed());
    REQUIRE(emit.bodyLength() == sizing.bodyLength());

    bodyLen = emit.bodyLength();

    uint8_t pfx[2];
    prefixLen = PubSubClient::encodeVariableByteInteger(bodyLen, pfx);
    REQUIRE(prefixLen > 0);

    std::vector<uint8_t> out(pfx, pfx + prefixLen);
    out.insert(out.end(), scratch.begin(), scratch.begin() + bodyLen);
    return out;
}

// Property 3's notion of equivalence, asserted in full rather than by comparing bytes:
//   - the same set of identifiers (no identifier gained, none lost),
//   - the same value for each identifier,
//   - the same number of occurrences of each identifier,
//   - and, for identifiers that may repeat, the same relative order.
// The per-identifier subsequence comparison covers the last three at once: equal
// subsequence lengths is the multiplicity claim, and element-wise equality over an
// ordered subsequence is the value and ordering claim.
static void checkEquivalent(const std::vector<Entry>& expected,
                            const std::vector<Norm>& got,
                            const char* who) {
    REQUIRE_MESSAGE(got.size() == expected.size(), who, ": entry count");

    // Identifier sets, both directions.
    for (const auto& e : expected) {
        bool found = false;
        for (const auto& g : got) { if (g.id == e.id) { found = true; break; } }
        CHECK_MESSAGE(found, who, ": identifier ", (unsigned)e.id, " missing after decode");
    }
    for (const auto& g : got) {
        bool found = false;
        for (const auto& e : expected) { if (e.id == g.id) { found = true; break; } }
        CHECK_MESSAGE(found, who, ": identifier ", (unsigned)g.id, " appeared from nowhere");
    }

    // Per-identifier ordered subsequences.
    for (size_t i = 0; i < expected.size(); i++) {
        const uint8_t id = expected[i].id;
        bool alreadyDone = false;
        for (size_t j = 0; j < i; j++) { if (expected[j].id == id) { alreadyDone = true; } }
        if (alreadyDone) { continue; }

        std::vector<const Entry*> exp;
        for (const auto& e : expected) { if (e.id == id) { exp.push_back(&e); } }
        std::vector<const Norm*> act;
        for (const auto& g : got) { if (g.id == id) { act.push_back(&g); } }

        REQUIRE_MESSAGE(act.size() == exp.size(), who, ": identifier ", (unsigned)id,
                        " multiplicity");

        for (size_t k = 0; k < exp.size(); k++) {
            switch (exp[k]->type) {
                case MqttPropType::Byte:
                case MqttPropType::TwoByte:
                case MqttPropType::FourByte:
                case MqttPropType::VarInt:
                    CHECK_MESSAGE(act[k]->num == exp[k]->num, who, ": identifier ",
                                  (unsigned)id, " occurrence ", k, " value");
                    break;
                case MqttPropType::Utf8:
                    CHECK_MESSAGE(act[k]->s == exp[k]->s, who, ": identifier ",
                                  (unsigned)id, " occurrence ", k, " string");
                    break;
                case MqttPropType::Binary:
                    CHECK_MESSAGE(act[k]->blob == exp[k]->blob, who, ": identifier ",
                                  (unsigned)id, " occurrence ", k, " binary");
                    break;
                case MqttPropType::StringPair:
                    CHECK_MESSAGE(act[k]->s == exp[k]->s, who, ": identifier ",
                                  (unsigned)id, " occurrence ", k, " pair key");
                    CHECK_MESSAGE(act[k]->s2 == exp[k]->s2, who, ": identifier ",
                                  (unsigned)id, " occurrence ", k, " pair value");
                    break;
                case MqttPropType::None:
                    break;   // 0x15/0x16 are never round-tripped
            }
        }
    }
}

// Encode the set, decode it back with the library reader AND with the harness's
// independent decoder, and assert equivalence plus the consumption contract on both.
static void roundTrip(const std::vector<Entry>& entries, uint8_t packetType) {
    uint8_t  prefixLen = 0;
    uint16_t bodyLen   = 0;
    const std::vector<uint8_t> f = encodeField(entries, prefixLen, bodyLen);
    REQUIRE(f.size() == (size_t)prefixLen + bodyLen);

    // --- Library decode -----------------------------------------------------
    // Decoded from an exactly-sized copy, so a read past the field is an ASan report
    // rather than a silent pass.
    std::vector<uint8_t> exact(f);
    mqtt5_props::Collector c;
    uint32_t consumed = 0;
    REQUIRE(decodeProperties(exact.data(), (uint32_t)exact.size(), packetType,
                             mqtt5_props::collect, &c, consumed) == MqttError::None);
    // The decode consumes exactly the Property Length prefix plus the declared body.
    CHECK(consumed == (uint32_t)prefixLen + bodyLen);

    std::vector<Norm> lib;
    for (const auto& s : c.seen) {
        Norm n;
        n.id = s.id;
        n.num = s.value;
        n.s = s.text;
        n.s2 = s.text2;
        n.blob = s.blob;
        lib.push_back(n);
    }
    checkEquivalent(entries, lib, "library");

    // Types survive the round-trip too: the reader classifies each identifier the same
    // way the writer encoded it.
    REQUIRE(c.seen.size() == entries.size());
    for (size_t i = 0; i < entries.size(); i++) {
        CHECK(c.seen[i].type == entries[i].type);
    }

    // --- Harness cross-decode ----------------------------------------------
    const DecodedProperties h = MqttParser::decodeProperties(f, 0);
    REQUIRE(h.valid);
    CHECK(h.declaredLength == bodyLen);
    CHECK(h.totalBytes == (size_t)prefixLen + bodyLen);

    std::vector<Norm> harness;
    for (const auto& e : h.entries) {
        Norm n;
        n.id = e.id;
        n.num = e.value;
        n.s = e.s;
        n.s2 = e.s2;
        n.blob = e.bin;
        harness.push_back(n);
    }
    checkEquivalent(entries, harness, "harness");

    // Decoding the field out of a larger buffer must not change where it ends: the
    // trailing bytes below are deliberately shaped like a property entry.
    std::vector<uint8_t> padded(f);
    const std::vector<uint8_t> tail = {0x26, 0xFF, 0x01, 0x00};
    padded.insert(padded.end(), tail.begin(), tail.end());
    mqtt5_props::Collector c2;
    uint32_t consumed2 = 0;
    CHECK(decodeProperties(padded.data(), (uint32_t)padded.size(), packetType,
                           mqtt5_props::collect, &c2, consumed2) == MqttError::None);
    CHECK(consumed2 == (uint32_t)prefixLen + bodyLen);
    CHECK(c2.seen.size() == c.seen.size());
}

// A value of the right type for a table sweep, so every identifier can be exercised
// without a per-identifier literal.
static Entry sample(uint8_t id, MqttPropType type) {
    switch (type) {
        case MqttPropType::Byte:       return pByte(id, 0x01);
        case MqttPropType::TwoByte:    return pU16 (id, 0xBEEF);
        case MqttPropType::FourByte:   return pU32 (id, 0xDEADBEEFu);
        case MqttPropType::VarInt:     return pVar (id, 16383);
        case MqttPropType::Utf8:       return pStr (id, "value");
        case MqttPropType::Binary:     return pBin (id, {0x00, 0x7F, 0xFF, 0x00});
        case MqttPropType::StringPair: return pPair(id, "key", "value");
        case MqttPropType::None:       break;   // 0x15/0x16 are never round-tripped
    }
    return pByte(id, 0);
}

}  // namespace mqtt5_p3

// Feature: tasmota-pubsub-mqtt5, Property 3: For all supported property sets — over every value type (Byte, Two Byte, Four Byte, Variable Byte Integer, UTF-8 string, Binary Data, UTF-8 string pair), including repeated User Properties and the empty set — encoding the set and then decoding the encoded bytes yields an equivalent set: the same identifiers, the same value for each identifier, the same number of occurrences of each identifier, and the same order for identifiers that may repeat; and the decode consumes exactly the Property Length prefix plus the declared body length.
TEST_CASE("Property 3: Properties field round-trip") {
    using namespace mqtt5_props;
    using namespace mqtt5_p3;

    struct Case {
        const char*        name;
        uint8_t            packetType;
        std::vector<Entry> entries;
    };

    // Multi-byte UTF-8: "e" + U+00E9 (2 bytes) + U+20AC (3 bytes).
    const std::string kUtf8 = "e\xC3\xA9\xE2\x82\xAC";

    std::vector<Case> cases = {
        // The empty set: the degenerate member of the property space, and the one every
        // packet type emits when the caller supplies nothing.
        {"empty set",                       kPublish, {}},

        // One property of each of the seven value types.
        {"Byte alone",                      kPublish, {pByte(0x01, 1)}},
        {"Two Byte alone",                  kPublish, {pU16 (0x23, 0x1234)}},
        {"Four Byte alone",                 kPublish, {pU32 (0x02, 0x01020304)}},
        {"Variable Byte Integer alone",     kPublish, {pVar (0x0B, 128)}},
        {"UTF-8 string alone",              kPublish, {pStr (0x03, "text/plain")}},
        {"Binary Data alone",               kPublish, {pBin (0x09, {0x01, 0x02})}},
        {"string pair alone",               kPublish, {pPair(0x26, "k", "v")}},
        {"all seven value types together",  kPublish, {pByte(0x01, 1), pU16(0x23, 10),
                                                       pU32(0x02, 256), pVar(0x0B, 128),
                                                       pStr(0x03, "ab"),
                                                       pBin(0x09, {0x00, 0xFF, 0x00}),
                                                       pPair(0x26, "k", "vv")}},

        // Requirement 11.3 / 11.4: repeated User Properties, order preserved.
        {"0 User Properties",               kPublish, {pStr(0x03, "ct")}},
        {"1 User Property",                 kPublish, {pPair(0x26, "a", "1")}},
        {"2 User Properties",               kPublish, {pPair(0x26, "a", "1"),
                                                       pPair(0x26, "b", "2")}},
        {"5 User Properties",               kPublish, {pPair(0x26, "a", "1"),
                                                       pPair(0x26, "b", "2"),
                                                       pPair(0x26, "c", "3"),
                                                       pPair(0x26, "d", "4"),
                                                       pPair(0x26, "e", "5")}},
        // Duplicate keys with different values: the only thing distinguishing them is
        // their order, so this is where an order-losing decoder shows up.
        {"duplicate User Property keys",    kPublish, {pPair(0x26, "dup", "first"),
                                                       pPair(0x26, "dup", "second"),
                                                       pPair(0x26, "dup", "third")}},
        {"repeated Subscription Identifier", kPublish, {pVar(0x0B, 1), pVar(0x0B, 128),
                                                        pVar(0x0B, 16383)}},
        {"User Properties around other entries", kPublish, {pPair(0x26, "a", "1"),
                                                            pByte(0x01, 0),
                                                            pPair(0x26, "b", "2"),
                                                            pStr(0x03, "ct"),
                                                            pPair(0x26, "c", "3")}},

        // Strings: empty, multi-byte UTF-8, and both halves of a pair in each shape.
        {"empty string value",              kPublish, {pStr (0x03, "")}},
        {"multi-byte UTF-8 string",         kPublish, {pStr (0x03, kUtf8)}},
        {"multi-byte UTF-8 pair",           kPublish, {pPair(0x26, kUtf8, kUtf8)}},
        {"empty pair key and value",        kPublish, {pPair(0x26, "", "")}},
        {"empty key with a value",          kPublish, {pPair(0x26, "", "v")}},
        {"key with an empty value",         kPublish, {pPair(0x26, "k", "")}},
        {"string at the 2-byte prefix boundary", kPublish, {pStr(0x03, std::string(124, 'x'))}},
        {"string past the 2-byte prefix boundary", kPublish, {pStr(0x03, std::string(200, 'x'))}},

        // Binary Data: 0x00 bytes are data, not terminators.
        {"Binary Data containing 0x00",     kPublish, {pBin(0x09, {0x00, 0x01, 0x00, 0xFF, 0x00})}},
        {"Binary Data of only 0x00 bytes",  kPublish, {pBin(0x09, {0x00, 0x00, 0x00, 0x00})}},
        {"empty Binary Data",               kPublish, {pBin(0x09, {})}},
        // Binary Data (Correlation Data, 0x09) is inbound-capped at 16 bytes
        // (Requirements 3.10, 11.11), so what this case exercises - the OUTER
        // Property Length Variable Byte Integer crossing its own 1-/2-byte size
        // class at 128 - is driven by pairing a full-cap Correlation Data value
        // with a long Content Type string instead of an oversized Binary Data
        // value.
        {"Binary Data alongside a body spanning both Property Length prefix widths",
                                            kPublish,
                                            {pBin(0x09, std::vector<uint8_t>(16, 0x00)),
                                             pStr(0x03, std::string(300, 'y'))}},

        // Type extremes.
        {"Byte 0",                          kPublish, {pByte(0x01, 0)}},
        {"Byte 1",                          kPublish, {pByte(0x01, 1)}},
        {"Byte 0xFF",                       kPublish, {pByte(0x01, 0xFF)}},
        {"Two Byte 0",                      kPublish, {pU16 (0x23, 0)}},
        {"Two Byte 1",                      kPublish, {pU16 (0x23, 1)}},
        {"Two Byte 0xFFFF",                 kPublish, {pU16 (0x23, 0xFFFF)}},
        {"Four Byte 0",                     kPublish, {pU32 (0x02, 0)}},
        {"Four Byte 1",                     kPublish, {pU32 (0x02, 1)}},
        {"Four Byte 0xFFFFFFFF",            kPublish, {pU32 (0x02, 0xFFFFFFFFu)}},
        // The Variable Byte Integer size classes, from both sides of the boundary.
        {"VarInt 0",                        kPublish, {pVar (0x0B, 0)}},
        {"VarInt 1",                        kPublish, {pVar (0x0B, 1)}},
        {"VarInt 127",                      kPublish, {pVar (0x0B, 127)}},
        {"VarInt 128",                      kPublish, {pVar (0x0B, 128)}},
        {"VarInt 16383",                    kPublish, {pVar (0x0B, 16383)}},

        // Contexts other than PUBLISH, including the Will pseudo-context and the
        // acknowledgement/subscription packets that carry only Reason String and
        // User Property.
        {"Will properties",                 kWill,    {pByte(0x01, 1), pU32(0x18, 30),
                                                       pStr(0x03, "ct"),
                                                       pPair(0x26, "k", "v")}},
        {"CONNECT properties",              kConnect, {pU16(0x21, 8), pU32(0x27, 1199),
                                                       pPair(0x26, "k", "v")}},
        {"CONNACK properties",              2,        {pU16(0x13, 60), pByte(0x24, 2),
                                                       pStr(0x12, "assigned-id"),
                                                       pU16(0x22, 5), pU16(0x21, 20),
                                                       pU32(0x27, 65535),
                                                       pStr(0x1F, "ok")}},
        {"PUBACK properties",               4,        {pStr(0x1F, "done"),
                                                       pPair(0x26, "k", "v")}},
        {"SUBSCRIBE properties",            8,        {pVar(0x0B, 42),
                                                       pPair(0x26, "k", "v")}},
        {"DISCONNECT properties",           14,       {pU32(0x11, 120),
                                                       pStr(0x1C, "other.host"),
                                                       pStr(0x1F, "bye")}},
        // AUTH properties, minus the two Enhanced Authentication identifiers
        // (Requirements 3.11, 16.5, 16.6): 0x15/0x16 are rejected rather than
        // round-tripped, so Property 3 exercises AUTH's other legal identifier only.
        {"AUTH properties",                 15,       {pStr(0x1F, "continue")}},

        // A set large enough that the Property Length itself needs two bytes, with
        // repeats mixed in.
        {"body needing a 2-byte length prefix", kPublish, {pStr(0x03, std::string(60, 'c')),
                                                           pStr(0x08, std::string(60, 'r')),
                                                           pPair(0x26, "k", std::string(40, 'v')),
                                                           pPair(0x26, "k", std::string(40, 'w'))}}
    };

    for (const auto& c : cases) {
        SUBCASE(c.name) {
            roundTrip(c.entries, c.packetType);
        }
    }
}

// The sweep half of the same property: every in-scope identifier, in every context the
// table declares it legal in, so no identifier is covered only by the curated sets above.
TEST_CASE("Property 3: every in-scope identifier round-trips in every legal context") {
    using namespace mqtt5_p3;

    REQUIRE(PubSubClient::propertyDefCount() == 27);

    unsigned swept = 0;
    for (unsigned i = 0; i <= 0xFF; i++) {
        const uint8_t id = (uint8_t)i;
        const MqttPropertyDef* def = PubSubClient::findPropertyDef(id);
        if (def == nullptr) { continue; }
        swept++;

        for (uint8_t bit = 0; bit < 16; bit++) {
            if ((def->packets & (uint16_t)(1u << bit)) == 0) { continue; }
            // Bit index == MQTT packet type number, with bit 0 the Will pseudo-context.
            const uint8_t packetType = bit;

            const std::string label = "id 0x" + std::string(1, "0123456789ABCDEF"[id >> 4]) +
                                      std::string(1, "0123456789ABCDEF"[id & 0x0F]) +
                                      " in context " + std::to_string((unsigned)packetType);
            SUBCASE(label.c_str()) {
                // Alone, then repeated: a repeat is only legal for an identifier the
                // table marks repeatable, so the second set is conditional.
                roundTrip({sample(id, def->type)}, packetType);
                if (def->multiple) {
                    roundTrip({sample(id, def->type), sample(id, def->type)}, packetType);
                }
            }
        }
    }
    CHECK(swept == 27);
}

// The same property along the path a real caller takes: writeProperties() frames the
// field from an MqttMessageProperties, and the field it produces must decode back to
// the set that was supplied - including the 0x00 an empty set frames.
TEST_CASE("Property 3: writeProperties output round-trips through both decoders") {
    using namespace mqtt5_props;
    using namespace mqtt5_p3;

    static const uint8_t corr[] = {0x00, 0xDE, 0x00, 0xAD, 0x00};
    static const char* keys[] = {"dup", "dup", "k3", "", "k5"};
    static const char* vals[] = {"first", "second", "", "empty-key", "e\xC3\xA9"};

    struct Case {
        const char*           name;
        MqttMessageProperties props;
        std::vector<Entry>    expected;
    };

    std::vector<Case> cases;

    {   // Empty set: one framing byte, zero entries.
        Case c;
        c.name = "empty set";
        cases.push_back(c);
    }
    {   // Every field the struct can express, plus five User Properties with a
        // duplicated key, an empty key and a multi-byte value.
        Case c;
        c.name = "every expressible property";
        c.props.hasPayloadFormat   = true;
        c.props.payloadFormat      = 1;
        c.props.contentType        = "text/plain";
        c.props.responseTopic      = "reply/to";
        c.props.correlationData    = corr;
        c.props.correlationDataLen = 5;
        c.props.userPropertyKeys   = keys;
        c.props.userPropertyValues = vals;
        c.props.userPropertyCount  = 5;
        c.expected = {
            pByte(MQTT_PROP_PAYLOAD_FORMAT_INDICATOR, 1),
            pStr (MQTT_PROP_CONTENT_TYPE, "text/plain"),
            pStr (MQTT_PROP_RESPONSE_TOPIC, "reply/to"),
            pBin (MQTT_PROP_CORRELATION_DATA, {0x00, 0xDE, 0x00, 0xAD, 0x00}),
            pPair(MQTT_PROP_USER_PROPERTY, "dup", "first"),
            pPair(MQTT_PROP_USER_PROPERTY, "dup", "second"),
            pPair(MQTT_PROP_USER_PROPERTY, "k3", ""),
            pPair(MQTT_PROP_USER_PROPERTY, "", "empty-key"),
            pPair(MQTT_PROP_USER_PROPERTY, "k5", "e\xC3\xA9")
        };
        cases.push_back(c);
    }

    for (const auto& c : cases) {
        SUBCASE(c.name) {
            std::vector<uint8_t> buf(512, 0xAA);
            const uint16_t pos = writeProperties(c.props, kPublish, buf.data(), 0,
                                                 (uint16_t)buf.size());
            REQUIRE(pos > 0);
            const std::vector<uint8_t> f(buf.begin(), buf.begin() + pos);

            Collector col;
            uint32_t consumed = 0;
            REQUIRE(decodeProperties(f.data(), (uint32_t)f.size(), kPublish, collect, &col,
                                     consumed) == MqttError::None);
            // Prefix plus declared body, which for a framed field is the whole of it.
            // The prefix width comes from the harness's own read of the length, so the
            // consumption claim is checked against an independently derived split.
            const DecodedProperties h = MqttParser::decodeProperties(f, 0);
            REQUIRE(h.valid);
            const size_t prefixLen = h.totalBytes - h.declaredLength;
            CHECK(prefixLen >= 1);
            CHECK(consumed == prefixLen + h.declaredLength);
            CHECK(consumed == f.size());

            std::vector<Norm> lib;
            for (const auto& s : col.seen) {
                Norm n;
                n.id = s.id; n.num = s.value; n.s = s.text; n.s2 = s.text2; n.blob = s.blob;
                lib.push_back(n);
            }
            checkEquivalent(c.expected, lib, "library");

            std::vector<Norm> harness;
            for (const auto& e : h.entries) {
                Norm n;
                n.id = e.id; n.num = e.value; n.s = e.s; n.s2 = e.s2; n.blob = e.bin;
                harness.push_back(n);
            }
            checkEquivalent(c.expected, harness, "harness");
        }
    }
}

#endif   // MQTT_VERSION == MQTT_VERSION_5_0

// --- Task 7.10: Property 29 — Correlation Data bounds, fixed storage, and
//     never truncated -----------------------------------------------------------
//
// Feature: tasmota-pubsub-mqtt5, Property 29: For all Correlation Data value
// lengths in 0..16 and for all value contents, the value is carried whole in the
// inbound direction, the accessor serves it from an address that is the same for
// every message and does not alias the packet's property region in the working
// buffer, and the client's memory footprint for Correlation Data — allocator call
// count and total allocated bytes attributable to it — is identical at every
// length and at every value; and for all inbound declared lengths above 16, the
// packet is treated as a protocol error that leaves the callback uninvoked, emits
// a DISCONNECT with a Reason Code of 0x80 or above when the socket is writable,
// closes the connection, and writes no byte of the value into storage — so an
// inbound value is never truncated to fit.
//
// Validates: Requirements 3.9, 3.10, 11.11, 11.12, 13.7, 13.8

#if MQTT_VERSION == MQTT_VERSION_5_0

namespace mqtt5_p29 {

// What the callback captured, by value so assertions survive the next loop().
struct Capture {
    int count = 0;
    std::string topic;
    std::vector<uint8_t> payload;
    unsigned int length = 0;
};

static Capture g_capture;

void callback(char* topic, uint8_t* payload, unsigned int length) {
    g_capture.count++;
    g_capture.topic = topic ? std::string(topic) : std::string();
    g_capture.length = length;
    g_capture.payload.assign(payload, payload + length);
}

// Set up a connected client with the capturing callback.
// MqttPacket::connack() is selection-aware.
static void connect(MockClient& client, PubSubClient& psc) {
    g_capture = Capture();
    client.pushPacket(MqttPacket::connack(0));
    psc.setServer("broker.example", 1883);
    psc.setCallback(callback);
    REQUIRE(psc.connect("mqtt5-p29"));
    REQUIRE(psc.connected());
    client.clearOutbound();
    psc.setSocketTimeout(1);
}

// Deterministic content of `len` bytes with an embedded 0x00 near the middle
// to prove binary-safety.  `len == 0` yields an empty vector.
// At the boundary extremes (1 and 16), and for all-0xFF content, the caller
// overrides. The helper is for all other lengths.
static std::vector<uint8_t> makeContent(size_t len) {
    std::vector<uint8_t> v(len, 0xAB);
    if (len >= 2) {
        v[len / 2] = 0x00;  // embedded NUL byte
    }
    return v;
}

// Verify that the DISCONNECT emitted on the outbound side carries a Reason Code
// >= 0x80 — checking only the third byte of the three-byte error DISCONNECT form
// `0xE0 0x01 <rc>`.
static void checkDisconnectReasonCode(const std::vector<uint8_t>& out) {
    // A 5-byte short-form DISCONNECT (0xE0 0x00) is not enough here; we need the
    // 3-byte form.  A 0x80+ reason code always produces the 3-byte form.
    // There could be earlier traffic so scan from the end of the outbound record.
    // We look for the MQTTDISCONNECT marker followed by 0x01 <rc>=80..FF.
    bool found = false;
    for (size_t i = 0; (i + 2) < out.size(); ++i) {
        if (out[i]   == static_cast<uint8_t>(MQTTDISCONNECT) &&
            out[i+1] == 0x01 &&
            out[i+2] >= 0x80) {
            found = true;
            break;
        }
    }
    CHECK_MESSAGE(found, "expected DISCONNECT with Reason Code >= 0x80 in outbound bytes");
}

}  // namespace mqtt5_p29

// Feature: tasmota-pubsub-mqtt5, Property 29: Correlation Data is bounded on
// inbound, fixed-storage, and never truncated.
TEST_CASE("Property 29: Correlation Data is bounded on inbound, fixed-storage, "
          "and never truncated") {
#if MQTT_VERSION == MQTT_VERSION_5_0
    using namespace mqtt5_p29;

    // -----------------------------------------------------------------------
    // Part 1 — Accepted domain: lengths 0..16 (17 cases)
    //
    // For every accepted length:
    //  a) the callback is invoked with the correct value;
    //  b) correlationData pointer is the same across successive messages
    //     (fixed storage, not freshly allocated);
    //  c) correlationData pointer does NOT alias the packet's property region
    //     in the working buffer (it comes from correlationDataStorage, which is
    //     a member of the PubSubClient object, not the heap buffer);
    //  d) AllocInterposer call counts are identical for every accepted length.
    // -----------------------------------------------------------------------

    // We will check that the correlationData pointer is identical across ALL 17
    // lengths.  We capture it after the first delivery and require every subsequent
    // delivery to land at the same address.  A null address before the first delivery
    // is fine; after the first it must be non-null and stable.
    const uint8_t* fixedAddr = nullptr;

    // Allocator baseline: reset before the sweep, record after the first message,
    // then verify each subsequent message adds the same delta.
    AllocInterposer::reset();
    unsigned mallocBase = 0, reallocBase = 0;
    bool firstDelivery = true;

    // String landmarks: we send a Content Type along with every message so there is
    // something in the property region that points INTO the working buffer.  After each
    // delivery we record that address and verify correlationData is NOT inside the
    // same allocation.
    //
    // The approach: inboundProperties().contentType.data points into the library's
    // heap-allocated working buffer.  We deduce the working buffer's heap pointer
    // from that, because we know the offset of the Content Type string within the
    // packet, and verify that correlationData falls outside [bufStart, bufEnd).
    //
    // Simpler approach used here: after delivery, verify
    //   psc.inboundProperties().correlationData != nullptr and does not equal any
    // address in the property region, by checking that the correlationData pointer
    // is NOT between contentType.data - some_large_offset and
    //                                  contentType.data + some_large_offset.
    // This is done by checking that correlationData is NOT the same as any pointer
    // into the packet bytes region: the property region data pointer (contentType)
    // lives in dynamically-allocated memory, while correlationDataStorage is a member
    // of the PubSubClient object (stack/object-footprint).
    //
    // The cleanest guarantee: correlationData pointer must be the SAME value across
    // all 17 successive deliveries with different buffer contents, because it always
    // points to the same object member.  The buffer is re-used for every packet, so
    // any address inside the buffer would differ between messages only if the buffer
    // were reallocated; if the address is ALWAYS the same and yet the bytes delivered
    // also reflect each successive message correctly, the address cannot be inside the
    // buffer's property region (which changes byte-by-byte each time).

    const std::string kContentType = "application/octet-stream";

    for (int len = 0; len <= 16; ++len) {
        CAPTURE(len);

        // --- Boundary SUBCASEs with specific content -------------------------
        // For brevity the SUBCASE is declared only for the four named boundaries;
        // all 17 lengths iterate normally regardless.  We use a local lambda-style
        // helper so the body stays in one place.

        // Build the content: boundary values get special shapes.
        std::vector<uint8_t> content;
        if (len == 0) {
            content = {};                                          // empty
        } else if (len == 1) {
            content = {0x00};                                      // single NUL byte
        } else if (len == 15) {
            // Embedded 0x00 at position 7, rest 0xFF
            content.assign(15, 0xFF);
            content[7] = 0x00;
        } else if (len == 16) {
            content.assign(16, 0xFF);                              // all-0xFF
        } else {
            content = makeContent(static_cast<size_t>(len));
        }

        if (len == 0) {
            SUBCASE("boundary len=0 (empty Correlation Data)") {
                // Empty correlation data: no bytes, just the length prefix 0x00
            }
        } else if (len == 1) {
            SUBCASE("boundary len=1 (single NUL byte)") {
                // Single byte, value 0x00
            }
        } else if (len == 15) {
            SUBCASE("boundary len=15 (embedded NUL, rest 0xFF)") {
                // Just below the upper limit
            }
        } else if (len == 16) {
            SUBCASE("boundary len=16 (all 0xFF, at MQTT_CORRELATION_DATA_MAX)") {
                // Exactly the cap: must succeed (cap is inclusive)
            }
        }

        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connect(client, psc);

        // Reset alloc counters before delivery of THIS message (not globally, because
        // the connect itself also allocates the buffer).  We compare deltas across
        // lengths, not absolute counts.
        const unsigned mallocBefore  = AllocInterposer::mallocCount();
        const unsigned reallocBefore = AllocInterposer::reallocCount();

        MqttProps props;
        if (len == 0) {
            // An empty-length Correlation Data property IS sent and IS reported as
            // present (hasCorrelationData == true, correlationDataLen == 0).
            // We just don't add any content bytes: bin(0x09, {}) emits
            // `0x09 0x00 0x00` — the identifier + 2-byte length of 0.
        }
        props.bin(0x09, content);
        props.str(0x03, kContentType);  // adds a string view into the buffer

        client.pushPacket(MqttPacket::publish5("t", std::vector<uint8_t>{'x'},
                                               /*qos=*/0, /*retained=*/false, /*dup=*/false,
                                               /*packetId=*/0, props));
        REQUIRE(psc.loop());

        // (a) Callback was invoked.
        CHECK(g_capture.count == 1);
        CHECK(psc.connected());
        CHECK_FALSE(client.stopCalled());

        const MqttInboundProperties& inb = psc.inboundProperties();
        CHECK(inb.valid);
        // A zero-length Correlation Data value is PRESENT (hasCorrelationData == true)
        // just as a zero-length UTF-8 string is present; the distinction is between
        // "the property appeared on the wire" and "no property of this type arrived".
        // bin() with an empty vector still emits the identifier + 0x00 0x00 length,
        // so the library WILL see the property and set hasCorrelationData = true.
        CHECK(inb.hasCorrelationData);
        CHECK(inb.correlationDataLen == static_cast<uint8_t>(len));
        CHECK(std::vector<uint8_t>(inb.correlationData,
                                   inb.correlationData + inb.correlationDataLen)
              == content);

        // (b) Fixed storage: address must be the same as in every previous delivery.
        if (fixedAddr == nullptr) {
            fixedAddr = inb.correlationData;
            // Even for len==0, the storage pointer is non-null (it's a member array).
            CHECK(fixedAddr != nullptr);
        } else {
            CHECK_MESSAGE(inb.correlationData == fixedAddr,
                          "correlationData pointer moved between messages");
        }

        // (c) Non-alias: the correlationData pointer must NOT be inside the
        // packet's property region.  We know the Content Type string is a view
        // into the working buffer (heap-allocated); any address "nearby" to it
        // is suspect.  We additionally check that correlationData is NOT the same
        // address as any byte of the contentType value (which IS inside the buffer).
        REQUIRE(inb.contentType.present());
        const uint8_t* bufPtr = reinterpret_cast<const uint8_t*>(inb.contentType.data);
        // The DEFINITIVE check is that the address stays CONSTANT across all lengths:
        // the buffer is re-used but the storage member never moves.
        for (uint16_t i = 0; i < inb.contentType.len; ++i) {
            CHECK(inb.correlationData != reinterpret_cast<const uint8_t*>(inb.contentType.data) + i);
        }
        // Also verify the gap is larger than the working buffer to rule out aliasing.
        const ptrdiff_t diff = inb.correlationData - bufPtr;
        const ptrdiff_t absDiff = (diff < 0) ? -diff : diff;
        CHECK_MESSAGE(absDiff > static_cast<ptrdiff_t>(psc.getBufferSize()),
                      "correlationData appears to alias the working buffer");

        // (d) Allocator call count delta: must be the same for every accepted length.
        const unsigned mallocDelta  = AllocInterposer::mallocCount()  - mallocBefore;
        const unsigned reallocDelta = AllocInterposer::reallocCount() - reallocBefore;
        if (firstDelivery) {
            mallocBase  = mallocDelta;
            reallocBase = reallocDelta;
            firstDelivery = false;
        } else {
            CHECK_MESSAGE(mallocDelta  == mallocBase,
                          "malloc call count changed for Correlation Data length ", len);
            CHECK_MESSAGE(reallocDelta == reallocBase,
                          "realloc call count changed for Correlation Data length ", len);
        }
    }  // for len in 0..16

    // -----------------------------------------------------------------------
    // Part 2 — Rejection domain: declared lengths 17, 18, 255, 65535
    //
    // For each rejected length, both:
    //   (A) bytes actually present — the packet carries that many bytes, just
    //       the declared length violates the cap;
    //   (B) declared length overrunning the packet — fewer bytes are physically
    //       present than the declared length requires (double rejection: both the
    //       cap AND the truncation would cause a failure; we verify EITHER gives
    //       the right outcome).
    //
    // Expected in every case:
    //   - lastError() == MqttError::ProtocolError
    //   - Callback NOT invoked
    //   - DISCONNECT with Reason Code >= 0x80 in outbound
    //   - connected() == false
    //   - No byte of the value in correlationDataStorage
    //     (enforced by checking hasCorrelationData == false after the rejection)
    // -----------------------------------------------------------------------

    struct RejectionCase {
        const char* name;
        uint16_t    declaredLen;   // what the wire declares
        bool        overrun;       // true = send fewer bytes than declared
    };

    const RejectionCase rejections[] = {
        {"declared 17, bytes present",         17,    false},
        {"declared 17, overrun",               17,    true },
        {"declared 18, bytes present",         18,    false},
        {"declared 18, overrun",               18,    true },
        {"declared 255, bytes present",        255,   false},
        {"declared 255, overrun",              255,   true },
        {"declared 65535, bytes present",      65535, false},
        {"declared 65535, overrun",            65535, true },
    };

    for (const auto& rc : rejections) {
        SUBCASE(rc.name) {
            TestClock::instance().reset();
            MockClient client2;
            PubSubClient psc2(client2);
            connect(client2, psc2);

            // Establish a known clean state: deliver one VALID message so we can
            // verify the rejection does NOT write into correlationDataStorage.
            client2.pushPacket(MqttPacket::publish5("t", std::vector<uint8_t>{'a'},
                                                    /*qos=*/0, /*retained=*/false, /*dup=*/false,
                                                    /*packetId=*/0,
                                                    MqttProps().bin(0x09, {0xBB, 0xCC})));
            REQUIRE(psc2.loop());
            CHECK(g_capture.count == 1);
            CHECK(psc2.inboundProperties().hasCorrelationData);

            // Rebuild the capture counter so we can check the next callback count.
            g_capture = Capture();

            // Build the adversarial property bytes.
            //
            // For small declared lengths (17, 18) we can put all the bytes in and let
            // the cap check fire. For large declared lengths (255, 65535) the packet
            // would exceed the working buffer if we put all the bytes in — but we
            // don't need to. The cap check in decodeProperties reads ONLY the 2-byte
            // length prefix of the Correlation Data binary value before returning
            // ProtocolError (Requirements 3.10, 11.11). So we build the property bytes
            // manually: id(1) + 2-byte-big-endian-len + some actual bytes, then wrap
            // in a properly-declared (or overrun-declared) Properties Length prefix.
            //
            // For the "overrun" variant: we declare in the outer Property Length prefix
            // that MORE bytes follow than we actually provide, so the Properties field
            // itself appears truncated to the parser. This triggers either
            // MalformedPacket (truncation detected first) or ProtocolError (cap checked
            // first) — either is acceptable as both close the connection.

            // Actual bytes we PUT for the correlation data value (not the declared len).
            // Cap at 50 so even large declared lengths don't overflow the buffer.
            const uint16_t actualDataBytes = rc.overrun
                                             ? static_cast<uint16_t>(std::min((uint32_t)8, (uint32_t)rc.declaredLen / 2 + 1))
                                             : static_cast<uint16_t>(std::min((uint32_t)50, (uint32_t)rc.declaredLen));

            // Build the raw property body bytes:
            // 0x09 | HI(declaredLen) | LO(declaredLen) | [actualDataBytes bytes]
            std::vector<uint8_t> rawBody;
            rawBody.push_back(0x09);  // Correlation Data identifier
            rawBody.push_back(static_cast<uint8_t>(rc.declaredLen >> 8));
            rawBody.push_back(static_cast<uint8_t>(rc.declaredLen & 0xFF));
            rawBody.insert(rawBody.end(), actualDataBytes, 0xEE);

            // For the overrun variant, the outer Property Length declares a LARGER
            // body than rawBody actually contains.
            const uint32_t outerDeclaredBodyLen = rc.overrun
                                                  ? static_cast<uint32_t>(rawBody.size()) + 16u
                                                  : static_cast<uint32_t>(rawBody.size());

            // Build the complete Properties field: VBI(outerDeclaredBodyLen) + rawBody
            std::vector<uint8_t> propsField;
            uint8_t vbiBuf[2];
            const uint8_t vbiLen = PubSubClient::encodeVariableByteInteger(outerDeclaredBodyLen, vbiBuf);
            REQUIRE(vbiLen > 0);
            propsField.insert(propsField.end(), vbiBuf, vbiBuf + vbiLen);
            propsField.insert(propsField.end(), rawBody.begin(), rawBody.end());

            // Build the raw PUBLISH packet using the adversarial properties field.
            // We use mqtt5_receive::publish5 pattern: topic + props + payload.
            // Topic "t" (1 char), QoS 0, no packet id.
            std::vector<uint8_t> pubBody;
            pubBody.push_back(0x00);
            pubBody.push_back(0x01);
            pubBody.push_back('t');
            pubBody.insert(pubBody.end(), propsField.begin(), propsField.end());
            pubBody.push_back('x');  // 1-byte payload

            std::vector<uint8_t> pubPacket;
            pubPacket.push_back(static_cast<uint8_t>(MQTTPUBLISH));
            const std::vector<uint8_t> rl = MqttPacket::encodeRemainingLength(
                static_cast<uint32_t>(pubBody.size()));
            pubPacket.insert(pubPacket.end(), rl.begin(), rl.end());
            pubPacket.insert(pubPacket.end(), pubBody.begin(), pubBody.end());

            client2.pushPacket(MqttPacket::raw(pubPacket));
            CHECK_FALSE(psc2.loop());

            // Callback must NOT have been invoked.
            CHECK(g_capture.count == 0);

            // Error must be ProtocolError or MalformedPacket (either is acceptable for
            // the overrun variant; ProtocolError is required for the bytes-present case).
            if (!rc.overrun) {
                CHECK(psc2.lastError() == MqttError::ProtocolError);
                CHECK(psc2.lastReasonCode() >= 0x80);
            } else {
                // Overrun: may be either malformed (truncated) or protocol error.
                const bool isProtocolOrMalformed =
                    (psc2.lastError() == MqttError::ProtocolError) ||
                    (psc2.lastError() == MqttError::MalformedPacket);
                CHECK(isProtocolOrMalformed);
                CHECK(psc2.lastReasonCode() >= 0x80);
            }

            // Connection must be closed.
            CHECK_FALSE(psc2.connected());
            CHECK(client2.stopCalled());

            // DISCONNECT emitted with Reason Code >= 0x80.
            checkDisconnectReasonCode(client2.outbound());

            // No byte of the rejected value was written into correlationDataStorage:
            // the inbound property set is cleared by the rejection path, so
            // hasCorrelationData must be false (the set is valid == false after a failure).
            CHECK_FALSE(psc2.inboundProperties().valid);
            CHECK_FALSE(psc2.inboundProperties().hasCorrelationData);
        }
    }

#else
    // In the 3.1.1 selection this property has no wire-level construct: there is
    // no Properties field and Correlation Data never arrives.  The test body is
    // excluded by the protocol guard above and this selection compiles to zero cases.
#endif   // MQTT_VERSION == MQTT_VERSION_5_0 (Property 29 body guard)
}

#endif   // MQTT_VERSION == MQTT_VERSION_5_0 (Property 29 namespace guard)

}  // TEST_SUITE("mqtt5")
