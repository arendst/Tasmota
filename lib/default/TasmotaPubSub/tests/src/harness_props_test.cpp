/*
  harness_props_test.cpp - Self-tests for the harness's MQTT 5.0 property
  fixtures and the generalized structural validator (task 4.4), plus the
  CONNECT / CONNACK fixtures and decoder added in task 6.4.

  Feature: tasmota-pubsub-mqtt5

  The harness is the reference the library is compared against, so it has to be
  independently trustworthy: MqttProps, MqttParser::decodeProperties,
  MqttParser::isStructurallyWellFramed, MqttPacket::connack5 and
  MqttParser::decodeConnect5 are all written without calling the library, and
  these cases pin their behavior on their own (Requirements 15.4, 15.5).

  Deliberately NOT wrapped in a TEST_SUITE, the same convention vbi_test.cpp and
  error_model_test.cpp use, so the cases run in BOTH selections: the fixture
  builder and the properties decoder are version-neutral code compiled into both
  binaries. Tagging `baseline` would exclude them from the v5 binary and would
  also perturb the frozen baseline reference count; tagging `mqtt5` would exclude
  them from the v311 binary.

  Where a case necessarily asserts selection-specific behavior - the structural
  validator skips the Properties walk in the 3.1.1 selection, because 3.1.1 has
  no Properties field on any packet - the body is guarded by
  `#if MQTT_VERSION == ...`.
*/

#include <cstdint>
#include <string>
#include <vector>

#include "doctest.h"

#include "MqttPacket.h"
#include "PubSubClient.h"

namespace harness_props {

// Frame `body` behind a fixed header, using the harness's own Remaining Length
// encoder. Local to this file so the cases below express packet shapes directly
// without needing a builder for every MQTT 5.0 type (those arrive in tasks 6.4,
// 7.3 and 9.5).
static std::vector<uint8_t> frame(uint8_t fixedHeader, const std::vector<uint8_t>& body) {
    std::vector<uint8_t> out;
    out.push_back(fixedHeader);
    const std::vector<uint8_t> rl =
        MqttPacket::encodeRemainingLength(static_cast<uint32_t>(body.size()));
    out.insert(out.end(), rl.begin(), rl.end());
    out.insert(out.end(), body.begin(), body.end());
    return out;
}

static void append(std::vector<uint8_t>& out, const std::vector<uint8_t>& more) {
    out.insert(out.end(), more.begin(), more.end());
}

// 2-byte big-endian length-prefixed string, the MQTT string layout.
static void appendStr(std::vector<uint8_t>& out, const std::string& s) {
    out.push_back(static_cast<uint8_t>(s.size() >> 8));
    out.push_back(static_cast<uint8_t>(s.size() & 0xFF));
    out.insert(out.end(), s.begin(), s.end());
}

}  // namespace harness_props

// ===========================================================================
//  MqttProps - fixture builder
// ===========================================================================

TEST_CASE("MqttProps emits a Variable Byte Integer length prefix ahead of the body") {
    // The empty set is the single byte 0x00: a zero-length body with a
    // zero-valued length prefix.
    const std::vector<uint8_t> emptySet = MqttProps().bytes();
    CHECK(emptySet == std::vector<uint8_t>{0x00});

    // One Byte-valued property: length 2, then <id><value>.
    MqttProps one;
    one.byte_(0x01, 0x01);
    CHECK(one.body() == std::vector<uint8_t>{0x01, 0x01});
    CHECK(one.bytes() == std::vector<uint8_t>{0x02, 0x01, 0x01});

    // A body above 127 bytes takes a 2-byte length prefix. Each "k"/"v" user
    // property is 7 bytes (identifier + two 2-byte-prefixed 1-byte strings), so
    // 20 of them is 140 bytes.
    MqttProps big;
    for (int i = 0; i < 20; ++i) {
        big.pair(0x26, "k", "v");
    }
    REQUIRE(big.body().size() == 140u);
    const std::vector<uint8_t> bigBytes = big.bytes();
    REQUIRE(bigBytes.size() == 142u);
    CHECK(bigBytes[0] == 0x8C);   // 140 = 0x8C 0x01 as a VBI
    CHECK(bigBytes[1] == 0x01);
    // Cross-check the prefix against the independent 2-byte VBI mirror.
    CHECK(MqttPacket::encodeVbi(140) == std::vector<uint8_t>{0x8C, 0x01});
}

TEST_CASE("MqttProps lays out every MQTT 5.0 property value type on the wire") {
    SUBCASE("Byte") {
        CHECK(MqttProps().byte_(0x24, 0x02).body() == std::vector<uint8_t>{0x24, 0x02});
    }
    SUBCASE("Two Byte Integer is big-endian") {
        CHECK(MqttProps().u16(0x21, 0x1234).body()
              == std::vector<uint8_t>{0x21, 0x12, 0x34});
    }
    SUBCASE("Four Byte Integer is big-endian") {
        CHECK(MqttProps().u32(0x27, 0x01020304).body()
              == std::vector<uint8_t>{0x27, 0x01, 0x02, 0x03, 0x04});
    }
    SUBCASE("Variable Byte Integer uses the 1-byte class below 128") {
        CHECK(MqttProps().varint(0x0B, 127).body() == std::vector<uint8_t>{0x0B, 0x7F});
    }
    SUBCASE("Variable Byte Integer uses the 2-byte class at 128") {
        CHECK(MqttProps().varint(0x0B, 128).body()
              == std::vector<uint8_t>{0x0B, 0x80, 0x01});
    }
    SUBCASE("Variable Byte Integer above 16383 is still expressible for the corpus") {
        // encodeVbi would refuse this; the fixture builder must not, because the
        // library's refusal path has to be given something to refuse.
        const std::vector<uint8_t> b = MqttProps().varint(0x0B, 16384).body();
        REQUIRE(b.size() == 4u);
        CHECK(b[0] == 0x0B);
        CHECK(MqttPacket::encodeVbi(16384).empty());
    }
    SUBCASE("UTF-8 string carries a 2-byte length prefix") {
        CHECK(MqttProps().str(0x03, "ab").body()
              == std::vector<uint8_t>{0x03, 0x00, 0x02, 'a', 'b'});
    }
    SUBCASE("empty UTF-8 string is a bare zero length") {
        CHECK(MqttProps().str(0x03, "").body() == std::vector<uint8_t>{0x03, 0x00, 0x00});
    }
    SUBCASE("Binary Data may contain 0x00 bytes") {
        CHECK(MqttProps().bin(0x09, {0x00, 0xFF, 0x00}).body()
              == std::vector<uint8_t>{0x09, 0x00, 0x03, 0x00, 0xFF, 0x00});
    }
    SUBCASE("UTF-8 string pair is key then value") {
        CHECK(MqttProps().pair(0x26, "a", "bc").body()
              == std::vector<uint8_t>{0x26, 0x00, 0x01, 'a', 0x00, 0x02, 'b', 'c'});
    }
}

TEST_CASE("MqttProps::rawLength declares a length that differs from the body") {
    MqttProps over;
    over.byte_(0x01, 0x01).rawLength(99);
    const std::vector<uint8_t> overBytes = over.bytes();
    CHECK(overBytes[0] == 99);                    // declared
    CHECK(over.body().size() == 2u);              // actual
    CHECK(overBytes.size() == 3u);                // prefix + actual body only

    MqttProps under;
    under.byte_(0x01, 0x01).byte_(0x17, 0x00).rawLength(2);
    const std::vector<uint8_t> underBytes = under.bytes();
    CHECK(underBytes[0] == 2);
    CHECK(under.body().size() == 4u);

    // Entries added after rawLength() still land in the body; the declaration
    // stays put, which is what makes an under-run fixture possible.
    MqttProps later;
    later.rawLength(2).byte_(0x01, 0x01).byte_(0x17, 0x00);
    CHECK(later.bytes()[0] == 2);
    CHECK(later.body().size() == 4u);
}

// ===========================================================================
//  MqttParser::decodeProperties
// ===========================================================================

TEST_CASE("decodeProperties round-trips every MqttProps value type") {
    MqttProps p;
    p.byte_(0x01, 0x01)
     .u16(0x21, 0xFFFF)
     .u32(0x27, 0xFFFFFFFFu)
     .varint(0x0B, 16383)
     .str(0x08, "resp/topic")
     .bin(0x09, {0x00, 0x01, 0x00})
     .pair(0x26, "k1", "v1")
     .pair(0x26, "k1", "v2");

    const std::vector<uint8_t> wire = p.bytes();
    const DecodedProperties d = MqttParser::decodeProperties(wire, 0);

    REQUIRE(d.valid);
    CHECK(d.declaredLength == p.body().size());
    CHECK(d.totalBytes == wire.size());
    REQUIRE(d.entries.size() == 8u);

    CHECK(d.entries[0].id == 0x01);
    CHECK(d.entries[0].value == 0x01u);
    CHECK(d.entries[1].value == 0xFFFFu);
    CHECK(d.entries[2].value == 0xFFFFFFFFu);
    CHECK(d.entries[3].value == 16383u);
    CHECK(d.entries[4].s == "resp/topic");
    CHECK(d.entries[5].bin == std::vector<uint8_t>{0x00, 0x01, 0x00});

    // Repeated User Properties keep their wire order, duplicate keys included.
    CHECK(d.entries[6].id == 0x26);
    CHECK(d.entries[6].s == "k1");
    CHECK(d.entries[6].s2 == "v1");
    CHECK(d.entries[7].s == "k1");
    CHECK(d.entries[7].s2 == "v2");
}

TEST_CASE("decodeProperties accepts the empty set and decodes at an offset") {
    const DecodedProperties empty = MqttParser::decodeProperties({0x00}, 0);
    CHECK(empty.valid);
    CHECK(empty.declaredLength == 0u);
    CHECK(empty.totalBytes == 1u);
    CHECK(empty.entries.empty());

    // A Properties field never starts at offset 0 in a real packet, so decoding
    // mid-buffer has to work: two leading bytes, then the field, then a trailer.
    std::vector<uint8_t> buf{0xAA, 0xBB};
    harness_props::append(buf, MqttProps().byte_(0x01, 0x01).bytes());
    buf.push_back(0xCC);

    const DecodedProperties d = MqttParser::decodeProperties(buf, 2);
    REQUIRE(d.valid);
    CHECK(d.totalBytes == 3u);
    REQUIRE(d.entries.size() == 1u);
    CHECK(d.entries[0].id == 0x01);
}

TEST_CASE("decodeProperties rejects structurally broken Properties fields") {
    SUBCASE("declared length runs past the supplied bytes") {
        const std::vector<uint8_t> wire = MqttProps().byte_(0x01, 0x01).rawLength(9).bytes();
        const DecodedProperties d = MqttParser::decodeProperties(wire, 0);
        CHECK_FALSE(d.valid);
        CHECK(d.declaredLength == 9u);   // still reported, for diagnosis
    }
    SUBCASE("value runs past the declared body end") {
        // Declares 2 bytes but the entry is a Two Byte Integer needing 3.
        std::vector<uint8_t> wire{0x02, 0x21, 0x12, 0x34};
        CHECK_FALSE(MqttParser::decodeProperties(wire, 0).valid);
    }
    SUBCASE("identifier absent from the MQTT 5.0 table") {
        std::vector<uint8_t> wire{0x02, 0x7F, 0x00};
        CHECK_FALSE(MqttParser::decodeProperties(wire, 0).valid);
    }
    SUBCASE("string length prefix overruns the declared body") {
        std::vector<uint8_t> wire{0x04, 0x03, 0x00, 0x05, 'a'};
        CHECK_FALSE(MqttParser::decodeProperties(wire, 0).valid);
    }
    SUBCASE("non-minimal length prefix") {
        // 0x82 0x80 0x00 would be a 3-byte encoding of 2; only the 1-byte form
        // is legal, and the decoder must not admit the padded one.
        std::vector<uint8_t> wire{0x82, 0x80, 0x00, 0x01, 0x01};
        CHECK_FALSE(MqttParser::decodeProperties(wire, 0).valid);
    }
    SUBCASE("truncated length prefix") {
        std::vector<uint8_t> wire{0x80};
        CHECK_FALSE(MqttParser::decodeProperties(wire, 0).valid);
    }
    SUBCASE("offset past the end of the buffer") {
        std::vector<uint8_t> wire{0x00};
        CHECK_FALSE(MqttParser::decodeProperties(wire, 1).valid);
        CHECK_FALSE(MqttParser::decodeProperties(wire, 5).valid);
    }
}

// ===========================================================================
//  MqttParser::isStructurallyWellFramed - version-neutral invariants
// ===========================================================================

TEST_CASE("isStructurallyWellFramed enforces the Remaining Length invariant") {
    // PINGREQ / PINGRESP carry no Properties field in either selection.
    CHECK(MqttParser::isStructurallyWellFramed(MqttPacket::pingreq().bytes()));
    CHECK(MqttParser::isStructurallyWellFramed(MqttPacket::pingresp().bytes()));

    SUBCASE("empty input") {
        CHECK_FALSE(MqttParser::isStructurallyWellFramed({}));
    }
    SUBCASE("packet type 0 is forbidden") {
        CHECK_FALSE(MqttParser::isStructurallyWellFramed({0x00, 0x00}));
    }
    SUBCASE("declared Remaining Length exceeds the trailing byte count") {
        CHECK_FALSE(MqttParser::isStructurallyWellFramed({0xC0, 0x01}));
    }
    SUBCASE("trailing bytes exceed the declared Remaining Length") {
        CHECK_FALSE(MqttParser::isStructurallyWellFramed({0xC0, 0x00, 0x00}));
    }
    SUBCASE("PINGREQ with a non-zero body") {
        CHECK_FALSE(MqttParser::isStructurallyWellFramed({0xC0, 0x01, 0x00}));
    }
    SUBCASE("truncated Remaining Length field") {
        CHECK_FALSE(MqttParser::isStructurallyWellFramed({0xC0}));
        CHECK_FALSE(MqttParser::isStructurallyWellFramed({0xC0, 0x80}));
    }
    SUBCASE("acknowledgement short form: packet identifier only") {
        // Legal in both selections (3.1.1 PUBACK, MQTT 5.0 short-form PUBACK).
        CHECK(MqttParser::isStructurallyWellFramed(MqttPacket::puback(1).bytes()));
        CHECK_FALSE(MqttParser::isStructurallyWellFramed({0x40, 0x01, 0x00}));
    }
}

#if MQTT_VERSION == MQTT_VERSION_3_1_1

TEST_CASE("isStructurallyWellFramed accepts the 3.1.1 fixtures, which carry no properties") {
    CHECK(MqttParser::isStructurallyWellFramed(MqttPacket::connack(0, true).bytes()));
    CHECK(MqttParser::isStructurallyWellFramed(
        MqttPacket::publish("a/b", std::string("hi"), 0, false, 0).bytes()));
    CHECK(MqttParser::isStructurallyWellFramed(
        MqttPacket::publish("a/b", std::string("hi"), 1, false, 7).bytes()));
    CHECK(MqttParser::isStructurallyWellFramed(MqttPacket::suback(3, 1).bytes()));
    CHECK(MqttParser::isStructurallyWellFramed(MqttPacket::unsuback(4).bytes()));
    CHECK(MqttParser::isStructurallyWellFramed(MqttPacket::disconnect().bytes()));

    // A 3.1.1 SUBSCRIBE: packet id, then (filter, requested QoS) pairs.
    std::vector<uint8_t> sub{0x00, 0x05};
    harness_props::appendStr(sub, "a/b");
    sub.push_back(0x01);
    CHECK(MqttParser::isStructurallyWellFramed(
        harness_props::frame(static_cast<uint8_t>(MQTTSUBSCRIBE) | 0x02, sub)));

    // Missing the requested-QoS byte is not well framed.
    std::vector<uint8_t> noQos{0x00, 0x05};
    harness_props::appendStr(noQos, "a/b");
    CHECK_FALSE(MqttParser::isStructurallyWellFramed(
        harness_props::frame(static_cast<uint8_t>(MQTTSUBSCRIBE) | 0x02, noQos)));
}

#endif  // MQTT_VERSION == MQTT_VERSION_3_1_1

#if MQTT_VERSION == MQTT_VERSION_5_0

TEST_CASE("isStructurallyWellFramed checks the Properties length of every 5.0 packet type") {
    // --- CONNACK: ack flags + reason code, then Properties -------------------
    SUBCASE("CONNACK") {
        std::vector<uint8_t> ok{0x00, 0x00};
        harness_props::append(ok, MqttProps().u16(0x21, 8).bytes());
        CHECK(MqttParser::isStructurallyWellFramed(
            harness_props::frame(static_cast<uint8_t>(MQTTCONNACK), ok)));

        // Over-declared Properties length: the field claims more bytes than the
        // packet has left.
        std::vector<uint8_t> over{0x00, 0x00};
        harness_props::append(over, MqttProps().u16(0x21, 8).rawLength(20).bytes());
        CHECK_FALSE(MqttParser::isStructurallyWellFramed(
            harness_props::frame(static_cast<uint8_t>(MQTTCONNACK), over)));

        // Under-declared Properties length: the field ends early, so a stray
        // byte is left over where the packet should have ended.
        std::vector<uint8_t> under{0x00, 0x00};
        harness_props::append(under,
                              MqttProps().u16(0x21, 8).byte_(0x24, 2).rawLength(3).bytes());
        CHECK_FALSE(MqttParser::isStructurallyWellFramed(
            harness_props::frame(static_cast<uint8_t>(MQTTCONNACK), under)));

        // A missing Properties field entirely (3.1.1-shaped CONNACK) is not a
        // well-framed 5.0 packet. Built by hand rather than with MqttPacket::connack(),
        // which follows the selection and emits the 5.0 shape in this build (task 6.2).
        CHECK_FALSE(MqttParser::isStructurallyWellFramed(
            harness_props::frame(static_cast<uint8_t>(MQTTCONNACK), {0x00, 0x00})));
    }

    // --- CONNECT: variable-header Properties, plus Will Properties in the
    //     payload immediately before the Will Topic ---------------------------
    SUBCASE("CONNECT without a Will") {
        std::vector<uint8_t> body;
        harness_props::appendStr(body, "MQTT");
        body.push_back(5);       // protocol level
        body.push_back(0x02);    // clean start
        body.push_back(0x00);    // keepalive hi
        body.push_back(0x3C);    // keepalive lo
        harness_props::append(body, MqttProps().u16(0x21, 8).u32(0x27, 1199).bytes());
        harness_props::appendStr(body, "client");
        CHECK(MqttParser::isStructurallyWellFramed(
            harness_props::frame(static_cast<uint8_t>(MQTTCONNECT), body)));
    }

    SUBCASE("CONNECT with a Will carries two Properties fields") {
        std::vector<uint8_t> body;
        harness_props::appendStr(body, "MQTT");
        body.push_back(5);
        body.push_back(static_cast<uint8_t>(0x02 | 0x04 | 0xC0));   // clean, will, user+pass
        body.push_back(0x00);
        body.push_back(0x3C);
        harness_props::append(body, MqttProps().u16(0x21, 8).bytes());
        harness_props::appendStr(body, "client");
        harness_props::append(body, MqttProps().bytes());   // Will Properties (empty)
        harness_props::appendStr(body, "will/topic");
        harness_props::appendStr(body, "bye");
        harness_props::appendStr(body, "user");
        harness_props::appendStr(body, "pass");
        CHECK(MqttParser::isStructurallyWellFramed(
            harness_props::frame(static_cast<uint8_t>(MQTTCONNECT), body)));

        // Drop the Will Properties byte: every following field shifts and the
        // walk no longer lands on the end of the packet.
        std::vector<uint8_t> bad;
        harness_props::appendStr(bad, "MQTT");
        bad.push_back(5);
        bad.push_back(static_cast<uint8_t>(0x02 | 0x04));
        bad.push_back(0x00);
        bad.push_back(0x3C);
        harness_props::append(bad, MqttProps().u16(0x21, 8).bytes());
        harness_props::appendStr(bad, "client");
        harness_props::appendStr(bad, "will/topic");
        harness_props::appendStr(bad, "bye");
        CHECK_FALSE(MqttParser::isStructurallyWellFramed(
            harness_props::frame(static_cast<uint8_t>(MQTTCONNECT), bad)));
    }

    // --- PUBLISH: topic, packet id when QoS > 0, then Properties -------------
    SUBCASE("PUBLISH at QoS 0 and QoS 2") {
        std::vector<uint8_t> q0;
        harness_props::appendStr(q0, "a/b");
        harness_props::append(q0, MqttProps().bytes());
        q0.push_back('x');
        CHECK(MqttParser::isStructurallyWellFramed(
            harness_props::frame(static_cast<uint8_t>(MQTTPUBLISH), q0)));

        std::vector<uint8_t> q2;
        harness_props::appendStr(q2, "a/b");
        q2.push_back(0x00);
        q2.push_back(0x09);   // packet identifier
        harness_props::append(q2, MqttProps().byte_(0x01, 0x01).str(0x03, "text/plain").bytes());
        CHECK(MqttParser::isStructurallyWellFramed(
            harness_props::frame(static_cast<uint8_t>(MQTTPUBLISH) | 0x04, q2)));

        // Properties length reaching past the end of the packet.
        std::vector<uint8_t> bad;
        harness_props::appendStr(bad, "a/b");
        harness_props::append(bad, MqttProps().byte_(0x01, 0x01).rawLength(40).bytes());
        CHECK_FALSE(MqttParser::isStructurallyWellFramed(
            harness_props::frame(static_cast<uint8_t>(MQTTPUBLISH), bad)));
    }

    // --- Acknowledgements: three legal forms --------------------------------
    SUBCASE("acknowledgement long form") {
        std::vector<uint8_t> full{0x00, 0x07, 0x00};   // id + reason code
        harness_props::append(full, MqttProps().str(0x1F, "ok").bytes());
        CHECK(MqttParser::isStructurallyWellFramed(
            harness_props::frame(static_cast<uint8_t>(MQTTPUBREC), full)));

        std::vector<uint8_t> bad{0x00, 0x07, 0x00};
        harness_props::append(bad, MqttProps().str(0x1F, "ok").rawLength(30).bytes());
        CHECK_FALSE(MqttParser::isStructurallyWellFramed(
            harness_props::frame(static_cast<uint8_t>(MQTTPUBREC), bad)));
    }

    // --- SUBSCRIBE / SUBACK / UNSUBSCRIBE / UNSUBACK -------------------------
    SUBCASE("SUBSCRIBE and SUBACK") {
        std::vector<uint8_t> sub{0x00, 0x05};
        harness_props::append(sub, MqttProps().varint(0x0B, 3).bytes());
        harness_props::appendStr(sub, "a/b");
        sub.push_back(0x01);   // subscription options
        CHECK(MqttParser::isStructurallyWellFramed(
            harness_props::frame(static_cast<uint8_t>(MQTTSUBSCRIBE) | 0x02, sub)));

        std::vector<uint8_t> suback{0x00, 0x05};
        harness_props::append(suback, MqttProps().bytes());
        suback.push_back(0x01);   // one reason code
        CHECK(MqttParser::isStructurallyWellFramed(
            harness_props::frame(static_cast<uint8_t>(MQTTSUBACK), suback)));

        // SUBACK with no reason code at all is not well framed.
        std::vector<uint8_t> empty{0x00, 0x05};
        harness_props::append(empty, MqttProps().bytes());
        CHECK_FALSE(MqttParser::isStructurallyWellFramed(
            harness_props::frame(static_cast<uint8_t>(MQTTSUBACK), empty)));
    }

    SUBCASE("UNSUBSCRIBE and UNSUBACK") {
        std::vector<uint8_t> unsub{0x00, 0x06};
        harness_props::append(unsub, MqttProps().bytes());
        harness_props::appendStr(unsub, "a/b");
        CHECK(MqttParser::isStructurallyWellFramed(
            harness_props::frame(static_cast<uint8_t>(MQTTUNSUBSCRIBE) | 0x02, unsub)));

        std::vector<uint8_t> unsuback{0x00, 0x06};
        harness_props::append(unsuback, MqttProps().bytes());
        unsuback.push_back(0x00);
        CHECK(MqttParser::isStructurallyWellFramed(
            harness_props::frame(static_cast<uint8_t>(MQTTUNSUBACK), unsuback)));
    }

    // --- DISCONNECT / AUTH: short form, reason code only, or with properties -
    SUBCASE("DISCONNECT and AUTH forms") {
        CHECK(MqttParser::isStructurallyWellFramed({0xE0, 0x00}));               // short
        CHECK(MqttParser::isStructurallyWellFramed({0xE0, 0x01, 0x81}));         // rc only
        std::vector<uint8_t> withProps{0x81};
        harness_props::append(withProps, MqttProps().str(0x1F, "malformed").bytes());
        CHECK(MqttParser::isStructurallyWellFramed(
            harness_props::frame(static_cast<uint8_t>(MQTTDISCONNECT), withProps)));

        std::vector<uint8_t> auth{0x18};
        harness_props::append(auth, MqttProps().str(0x15, "SCRAM").bytes());
        CHECK(MqttParser::isStructurallyWellFramed(
            harness_props::frame(static_cast<uint8_t>(MQTTAUTH), auth)));

        std::vector<uint8_t> bad{0x81};
        harness_props::append(bad, MqttProps().str(0x1F, "x").rawLength(50).bytes());
        CHECK_FALSE(MqttParser::isStructurallyWellFramed(
            harness_props::frame(static_cast<uint8_t>(MQTTDISCONNECT), bad)));
    }
}

#endif  // MQTT_VERSION == MQTT_VERSION_5_0

// ===========================================================================
//  MqttPacket::connack5 / MqttParser::decodeConnect5 (task 6.4)
// ===========================================================================
//
// Both the builder and the decoder are version-neutral harness code: they always
// express the MQTT 5.0 shape, whatever the compile-time selection is, so these
// cases run unguarded in BOTH binaries. Only the cross-checks against
// isStructurallyWellFramed - which skips the Properties walk in the 3.1.1
// selection - are guarded further down.

namespace harness_props {

// Assemble an MQTT 5.0 CONNECT from its parts, in specification order. Written
// out longhand rather than via a builder so the field ORDER (in particular: Will
// Properties in the payload, immediately before the Will Topic) is visible in
// the fixture itself and cannot be silently inherited from the decoder.
struct Connect5Fixture {
    uint8_t     level = 5;
    uint8_t     flags = 0x02;          // Clean Start
    uint16_t    keepAlive = 60;
    MqttProps   props;
    std::string clientId = "client";
    bool        willPropsField = true;  // emit the Will Properties field at all
    MqttProps   willProps;
    std::string willTopic;
    std::string willMessage;
    std::string username;
    std::string password;
    bool        willPropsAfterTopic = false;   // the classic misplacement

    std::vector<uint8_t> bytes() const {
        std::vector<uint8_t> body;
        appendStr(body, "MQTT");
        body.push_back(level);
        body.push_back(flags);
        body.push_back(static_cast<uint8_t>(keepAlive >> 8));
        body.push_back(static_cast<uint8_t>(keepAlive & 0xFF));
        append(body, props.bytes());
        appendStr(body, clientId);
        if ((flags & 0x04) != 0) {   // Will flag
            if (willPropsField && !willPropsAfterTopic) {
                append(body, willProps.bytes());
            }
            appendStr(body, willTopic);
            if (willPropsField && willPropsAfterTopic) {
                append(body, willProps.bytes());
            }
            appendStr(body, willMessage);
        }
        if ((flags & 0x80) != 0) { appendStr(body, username); }
        if ((flags & 0x40) != 0) { appendStr(body, password); }
        return frame(static_cast<uint8_t>(MQTTCONNECT), body);
    }
};

}  // namespace harness_props

TEST_CASE("MqttPacket::connack5 frames ack flags, reason code and properties") {
    SUBCASE("empty property set still emits the 0x00 length byte") {
        // Remaining Length 3, never 2: the Properties field is not optional.
        CHECK(MqttPacket::connack5(0x00).bytes()
              == std::vector<uint8_t>{0x20, 0x03, 0x00, 0x00, 0x00});
    }
    SUBCASE("Session Present is bit 0 of the ack flags") {
        CHECK(MqttPacket::connack5(0x00, true).bytes()
              == std::vector<uint8_t>{0x20, 0x03, 0x01, 0x00, 0x00});
        CHECK(MqttPacket::connack5(0x87, false).bytes()
              == std::vector<uint8_t>{0x20, 0x03, 0x00, 0x87, 0x00});
    }
    SUBCASE("properties follow the reason code") {
        const std::vector<uint8_t> wire =
            MqttPacket::connack5(0x00, true,
                                 MqttProps().u16(0x13, 30).byte_(0x24, 1)).bytes();

        const DecodedPacket p = MqttParser::decode(wire);
        REQUIRE(p.valid);
        CHECK(p.type == static_cast<uint8_t>(MQTTCONNACK));
        CHECK(p.flags == 0x00);
        REQUIRE(p.payload.size() >= 2u);
        CHECK(p.payload[0] == 0x01);   // Session Present
        CHECK(p.payload[1] == 0x00);   // Success

        // The Properties field starts at offset 2 of the variable header.
        const DecodedProperties d = MqttParser::decodeProperties(p.payload, 2);
        REQUIRE(d.valid);
        REQUIRE(d.entries.size() == 2u);
        CHECK(d.entries[0].id == 0x13);      // Server Keep Alive
        CHECK(d.entries[0].value == 30u);
        CHECK(d.entries[1].id == 0x24);      // Maximum QoS
        CHECK(d.entries[1].value == 1u);
        CHECK(2u + d.totalBytes == p.payload.size());
    }
    SUBCASE("an adversarial property length is passed through unvalidated") {
        const std::vector<uint8_t> wire =
            MqttPacket::connack5(0x00, false,
                                 MqttProps().byte_(0x24, 1).rawLength(40)).bytes();
        // Framing at the packet level is still self-consistent; it is the
        // Properties field inside that is broken, which is what the library has
        // to catch.
        CHECK(MqttParser::decode(wire).valid);
        CHECK_FALSE(MqttParser::decodeProperties(MqttParser::decode(wire).payload, 2).valid);
    }
}

TEST_CASE("decodeConnect5 round-trips a CONNECT with variable-header properties") {
    harness_props::Connect5Fixture f;
    f.props.u16(0x21, 8).u32(0x27, 1199);
    f.clientId = "tasmota-1";
    f.flags = static_cast<uint8_t>(0x02 | 0x80 | 0x40);   // clean start, user, password
    f.username = "user";
    f.password = "secret";
    f.keepAlive = 120;

    const DecodedConnect5 c = MqttParser::decodeConnect5(f.bytes());
    REQUIRE(c.valid);
    CHECK(c.protocolName == "MQTT");
    CHECK(c.protocolLevel == 5);
    CHECK(c.cleanSession);
    CHECK_FALSE(c.willFlag);
    CHECK(c.userFlag);
    CHECK(c.passwordFlag);
    CHECK(c.keepAlive == 120);
    CHECK(c.clientId == "tasmota-1");
    CHECK(c.username == "user");
    CHECK(c.password == "secret");

    // Variable-header Properties, in wire order.
    REQUIRE(c.props.valid);
    REQUIRE(c.props.entries.size() == 2u);
    CHECK(c.props.entries[0].id == 0x21);           // Receive Maximum
    CHECK(c.props.entries[0].value == 8u);
    CHECK(c.props.entries[1].id == 0x27);           // Maximum Packet Size
    CHECK(c.props.entries[1].value == 1199u);

    // No Will, so the Will Properties field is absent, not merely empty.
    CHECK_FALSE(c.willProps.valid);
    CHECK(c.willProps.entries.empty());

    SUBCASE("an empty variable-header property set is valid and carries no entries") {
        harness_props::Connect5Fixture bare;
        const DecodedConnect5 d = MqttParser::decodeConnect5(bare.bytes());
        REQUIRE(d.valid);
        CHECK(d.props.valid);
        CHECK(d.props.declaredLength == 0u);
        CHECK(d.props.entries.empty());
        CHECK(d.clientId == "client");
    }

    SUBCASE("a Password with no User Name is decoded on its own flag") {
        harness_props::Connect5Fixture pwOnly;
        pwOnly.flags = static_cast<uint8_t>(0x02 | 0x40);
        pwOnly.password = "pw";
        const DecodedConnect5 d = MqttParser::decodeConnect5(pwOnly.bytes());
        REQUIRE(d.valid);
        CHECK_FALSE(d.userFlag);
        CHECK(d.passwordFlag);
        CHECK(d.username.empty());
        CHECK(d.password == "pw");
    }
}

TEST_CASE("decodeConnect5 reads the Will Properties from the payload before the Will Topic") {
    harness_props::Connect5Fixture f;
    // Clean start, Will flag, Will QoS 1, Will Retain, user + password.
    f.flags = static_cast<uint8_t>(0x02 | 0x04 | (1 << 3) | 0x20 | 0x80 | 0x40);
    f.props.u16(0x21, 8);
    f.willProps.u32(0x18, 45).pair(0x26, "wk", "wv");   // Will Delay Interval + User Property
    f.willTopic = "tele/dev/LWT";
    f.willMessage = "Offline";
    f.username = "u";
    f.password = "p";

    const DecodedConnect5 c = MqttParser::decodeConnect5(f.bytes());
    REQUIRE(c.valid);
    CHECK(c.willFlag);
    CHECK(c.willQos == 1);
    CHECK(c.willRetain);
    CHECK(c.willTopic == "tele/dev/LWT");
    CHECK(c.willMessage == "Offline");
    CHECK(c.username == "u");
    CHECK(c.password == "p");

    // The two Properties fields are kept apart: the variable-header one has the
    // Receive Maximum, the payload one has the Will-only identifiers.
    REQUIRE(c.props.valid);
    REQUIRE(c.props.entries.size() == 1u);
    CHECK(c.props.entries[0].id == 0x21);
    REQUIRE(c.willProps.valid);
    REQUIRE(c.willProps.entries.size() == 2u);
    CHECK(c.willProps.entries[0].id == 0x18);
    CHECK(c.willProps.entries[0].value == 45u);
    CHECK(c.willProps.entries[1].id == 0x26);
    CHECK(c.willProps.entries[1].s == "wk");
    CHECK(c.willProps.entries[1].s2 == "wv");

    SUBCASE("an empty Will Properties field is present-but-empty, not absent") {
        harness_props::Connect5Fixture g;
        g.flags = static_cast<uint8_t>(0x02 | 0x04);
        g.willTopic = "a/lwt";
        g.willMessage = "bye";
        const DecodedConnect5 d = MqttParser::decodeConnect5(g.bytes());
        REQUIRE(d.valid);
        CHECK(d.willProps.valid);
        CHECK(d.willProps.declaredLength == 0u);
        CHECK(d.willProps.entries.empty());
    }

    SUBCASE("omitting the Will Properties field shifts every following field") {
        harness_props::Connect5Fixture g = f;
        g.willPropsField = false;
        CHECK_FALSE(MqttParser::decodeConnect5(g.bytes()).valid);
    }

    SUBCASE("placing the Will Properties after the Will Topic is not a valid CONNECT") {
        harness_props::Connect5Fixture g = f;
        g.willPropsAfterTopic = true;
        CHECK_FALSE(MqttParser::decodeConnect5(g.bytes()).valid);
    }
}

TEST_CASE("decodeConnect5 rejects malformed CONNECT packets") {
    SUBCASE("empty input") {
        CHECK_FALSE(MqttParser::decodeConnect5({}).valid);
    }
    SUBCASE("wrong packet type") {
        CHECK_FALSE(MqttParser::decodeConnect5(MqttPacket::connack5(0).bytes()).valid);
    }
    SUBCASE("Remaining Length disagrees with the trailing byte count") {
        std::vector<uint8_t> wire = harness_props::Connect5Fixture().bytes();
        wire.push_back(0x00);
        CHECK_FALSE(MqttParser::decodeConnect5(wire).valid);
    }
    SUBCASE("variable header truncated after the protocol name") {
        std::vector<uint8_t> body;
        harness_props::appendStr(body, "MQTT");
        body.push_back(5);
        body.push_back(0x02);
        CHECK_FALSE(MqttParser::decodeConnect5(
            harness_props::frame(static_cast<uint8_t>(MQTTCONNECT), body)).valid);
    }
    SUBCASE("variable-header Properties length runs past the packet") {
        harness_props::Connect5Fixture f;
        f.props.u16(0x21, 8).rawLength(60);
        CHECK_FALSE(MqttParser::decodeConnect5(f.bytes()).valid);
    }
    SUBCASE("trailing byte after the last declared field") {
        harness_props::Connect5Fixture f;
        std::vector<uint8_t> body;
        harness_props::appendStr(body, "MQTT");
        body.push_back(5);
        body.push_back(0x02);
        body.push_back(0x00);
        body.push_back(0x3C);
        harness_props::append(body, MqttProps().bytes());
        harness_props::appendStr(body, "client");
        body.push_back(0xFF);   // nothing declares this byte
        CHECK_FALSE(MqttParser::decodeConnect5(
            harness_props::frame(static_cast<uint8_t>(MQTTCONNECT), body)).valid);
    }
    SUBCASE("a 3.1.1-shaped CONNECT has no Properties field and is not a 5.0 CONNECT") {
        std::vector<uint8_t> body;
        harness_props::appendStr(body, "MQTT");
        body.push_back(4);      // 3.1.1 protocol level
        body.push_back(0x02);
        body.push_back(0x00);
        body.push_back(0x3C);
        harness_props::appendStr(body, "client");   // no Properties in between
        CHECK_FALSE(MqttParser::decodeConnect5(
            harness_props::frame(static_cast<uint8_t>(MQTTCONNECT), body)).valid);
    }
}

#if MQTT_VERSION == MQTT_VERSION_5_0

TEST_CASE("the 5.0 CONNECT/CONNACK fixtures agree with the structural validator") {
    CHECK(MqttParser::isStructurallyWellFramed(MqttPacket::connack5(0x00, true).bytes()));
    CHECK(MqttParser::isStructurallyWellFramed(
        MqttPacket::connack5(0x87, false, MqttProps().str(0x1F, "nope")).bytes()));
    CHECK_FALSE(MqttParser::isStructurallyWellFramed(
        MqttPacket::connack5(0x00, false, MqttProps().byte_(0x24, 1).rawLength(40)).bytes()));

    harness_props::Connect5Fixture f;
    f.flags = static_cast<uint8_t>(0x02 | 0x04 | 0x80 | 0x40);
    f.props.u16(0x21, 8);
    f.willTopic = "a/lwt";
    f.willMessage = "bye";
    f.username = "u";
    f.password = "p";
    CHECK(MqttParser::isStructurallyWellFramed(f.bytes()));
    CHECK(MqttParser::decodeConnect5(f.bytes()).valid);

    harness_props::Connect5Fixture g = f;
    g.willPropsField = false;
    CHECK_FALSE(MqttParser::isStructurallyWellFramed(g.bytes()));
}

#endif  // MQTT_VERSION == MQTT_VERSION_5_0
