/*
  MqttParser_test.cpp - Unit checks for the MqttParser decoder and the
  structural PUBLISH validator (task 7.2).

  Two kinds of checks, both tagged into the baseline suite:
    1. Round-trip: build a control packet with the MqttPacket builder, decode it
       with MqttParser, and assert the decoded fields equal the original inputs.
       A subset also drives the *unmodified* PubSubClient to emit real outbound
       bytes and decodes those, proving the parser reads genuine library output.
    2. Validator: isStructurallyValidPublish accepts well-framed PUBLISH packets
       and rejects truncated / malformed ones.

  Every assertion goes through decoded wire bytes only (no private members), so
  these stay durable across a future MQTT 5 migration (Requirement 19.1).
*/

#include <cstdint>
#include <string>
#include <vector>

#include "doctest.h"

#include "MockClient.h"
#include "MqttPacket.h"
#include "TestClock.h"
#include "PubSubClient.h"

TEST_SUITE("baseline") {

    // --- Remaining Length codec: decode round-trips encode -----------------

    TEST_CASE("MqttParser decodeRemainingLength round-trips encodeRemainingLength") {
        // Curated boundary values spanning the 1..4 byte encodings.
        const uint32_t values[] = {
            0, 1, 127, 128, 16383, 16384, 2097151, 2097152, 268435455
        };
        for (uint32_t n : values) {
            const std::vector<uint8_t> enc = MqttPacket::encodeRemainingLength(n);
            uint32_t decoded = 0;
            size_t consumed = 0;
            REQUIRE(MqttParser::decodeRemainingLength(enc, 0, decoded, consumed));
            CHECK(decoded == n);
            CHECK(consumed == enc.size());
            CHECK(consumed <= 4);
        }
    }

    TEST_CASE("MqttParser decodeRemainingLength rejects a 5-byte continuation and truncation") {
        uint32_t value = 0;
        size_t consumed = 0;
        // Continuation bit still set after four bytes.
        const std::vector<uint8_t> tooLong = {0x80, 0x80, 0x80, 0x80, 0x01};
        CHECK_FALSE(MqttParser::decodeRemainingLength(tooLong, 0, value, consumed));
        // Field truncated mid-continuation.
        const std::vector<uint8_t> truncated = {0x80};
        CHECK_FALSE(MqttParser::decodeRemainingLength(truncated, 0, value, consumed));
    }

    // --- Generic decode framing -------------------------------------------

    TEST_CASE("MqttParser decode splits fixed header and validates framing") {
        MqttPacket p = MqttPacket::publish("a/b", std::string("hi"), 1, true, 0x1234);
        DecodedPacket d = MqttParser::decode(p.bytes());
        REQUIRE(d.valid);
        CHECK(d.type == static_cast<uint8_t>(MQTTPUBLISH));
        CHECK(d.flags == 0x03);                  // qos1 (0x02) | retain (0x01)
        CHECK(d.remainingLength == d.payload.size());
    }

    TEST_CASE("MqttParser decode rejects truncated and over-long frames") {
        MqttPacket p = MqttPacket::publish("topic", std::string("payload"));
        std::vector<uint8_t> bytes = p.bytes();

        // Well-formed decodes cleanly.
        CHECK(MqttParser::decode(bytes).valid);

        // Drop the last byte: declared Remaining Length now exceeds trailing bytes.
        std::vector<uint8_t> truncated = bytes;
        truncated.pop_back();
        CHECK_FALSE(MqttParser::decode(truncated).valid);

        // Append a stray byte: trailing bytes now exceed declared Remaining Length.
        std::vector<uint8_t> overlong = bytes;
        overlong.push_back(0x00);
        CHECK_FALSE(MqttParser::decode(overlong).valid);

        // Empty input is invalid.
        CHECK_FALSE(MqttParser::decode(std::vector<uint8_t>{}).valid);
    }

    // --- CONNECT round-trip (builder side + real library output) -----------

    TEST_CASE("MqttParser decodePublish round-trips PUBLISH fields") {
        struct Case { std::string topic; std::string payload; uint8_t qos; bool retain; uint16_t msgId; };
        const Case cases[] = {
            {"a/b",            "hi",        0, false, 0},
            {"sensor/temp",    "23.5",      1, true,  0x0102},
            {"x",              "",          0, false, 0},        // empty payload
            {"long/topic/name","payload-2", 1, false, 0xFFFF},
        };
        for (const Case& c : cases) {
            MqttPacket p = MqttPacket::publish(c.topic, c.payload, c.qos, c.retain, c.msgId);
            DecodedPublish d = MqttParser::decodePublish(p.bytes());
            REQUIRE(d.valid);
            CHECK(d.topic == c.topic);
            CHECK(d.qos == c.qos);
            CHECK(d.retain == c.retain);
            const std::vector<uint8_t> expectedPayload(c.payload.begin(), c.payload.end());
            CHECK(d.payload == expectedPayload);
            if (c.qos > 0) {
                CHECK(d.msgId == c.msgId);
            }
        }
    }

    TEST_CASE("MqttParser decodeSubscribe / decodeUnsubscribe round-trip filters and msgId") {
        MqttPacket sub = MqttPacket::raw({
            0x82, 0x08,          // SUBSCRIBE | qos1, remaining length 8
            0x00, 0x2A,          // msgId 42
            0x00, 0x03, 'a', '/', 'b',  // topic filter "a/b"
            0x01                 // requested qos 1
        });
        DecodedSubscribe ds = MqttParser::decodeSubscribe(sub.bytes());
        REQUIRE(ds.valid);
        CHECK(ds.msgId == 42);
        REQUIRE(ds.filters.size() == 1);
        CHECK(ds.filters[0] == "a/b");
        REQUIRE(ds.requestedQos.size() == 1);
        CHECK(ds.requestedQos[0] == 1);

        MqttPacket unsub = MqttPacket::raw({
            0xA2, 0x07,          // UNSUBSCRIBE | qos1, remaining length 7
            0x00, 0x2A,          // msgId 42
            0x00, 0x03, 'a', '/', 'b'   // topic filter "a/b"
        });
        DecodedUnsubscribe du = MqttParser::decodeUnsubscribe(unsub.bytes());
        REQUIRE(du.valid);
        CHECK(du.msgId == 42);
        REQUIRE(du.filters.size() == 1);
        CHECK(du.filters[0] == "a/b");
    }

    TEST_CASE("MqttParser decodeConnect round-trips a real library CONNECT packet") {
        // Drive the unmodified library so the parser reads genuine wire bytes.
        TestClock::instance().reset();
        MockClient client;
        // Script a valid CONNACK so connect() completes in finite virtual time.
        client.pushPacket(MqttPacket::connack(0));

        PubSubClient psc(client);
        psc.setServer("broker.example", 1883);
        const bool ok = psc.connect("client-42", "alice", "s3cret");
        REQUIRE(ok);

        DecodedConnect c = MqttParser::decodeConnect(client.outbound());
        REQUIRE(c.valid);
        CHECK(c.protocolName == "MQTT");
        CHECK(c.protocolLevel == MQTT_VERSION_3_1_1);
        CHECK(c.clientId == "client-42");
        CHECK(c.userFlag);
        CHECK(c.passwordFlag);
        CHECK(c.username == "alice");
        CHECK(c.password == "s3cret");
        CHECK(c.cleanSession);
        CHECK_FALSE(c.willFlag);
    }

    TEST_CASE("MqttParser decodePublish round-trips a real library PUBLISH packet") {
        // A successful connect is required for connected() to report true, which
        // publish() guards on. Script a CONNACK, connect, then clear the CONNECT
        // bytes so only the PUBLISH remains in the outbound record.
        TestClock::instance().reset();
        MockClient client;
        client.pushPacket(MqttPacket::connack(0));
        PubSubClient psc(client);
        psc.setServer("broker.example", 1883);
        REQUIRE(psc.connect("dev"));
        client.clearOutbound();

        REQUIRE(psc.publish("stat/dev/RESULT", "{\"POWER\":\"ON\"}", true));

        DecodedPublish d = MqttParser::decodePublish(client.outbound());
        REQUIRE(d.valid);
        CHECK(d.topic == "stat/dev/RESULT");
        CHECK(d.retain);
        CHECK(d.qos == 0);
        const std::string expected = "{\"POWER\":\"ON\"}";
        CHECK(std::string(d.payload.begin(), d.payload.end()) == expected);
    }

    TEST_CASE("MqttParser decodeSubscribe round-trips a real library SUBSCRIBE packet") {
        TestClock::instance().reset();
        MockClient client;
        client.pushPacket(MqttPacket::connack(0));
        PubSubClient psc(client);
        psc.setServer("broker.example", 1883);
        REQUIRE(psc.connect("dev"));
        client.clearOutbound();

        REQUIRE(psc.subscribe("cmnd/dev/#", 1));

        DecodedSubscribe d = MqttParser::decodeSubscribe(client.outbound());
        REQUIRE(d.valid);
        CHECK(d.msgId != 0);                      // library assigns a nonzero id
        REQUIRE(d.filters.size() == 1);
        CHECK(d.filters[0] == "cmnd/dev/#");
        REQUIRE(d.requestedQos.size() == 1);
        CHECK(d.requestedQos[0] == 1);
    }

    // --- Structural PUBLISH validator: accept / reject ---------------------

    TEST_CASE("isStructurallyValidPublish accepts well-formed PUBLISH packets") {
        CHECK(MqttParser::isStructurallyValidPublish(
            MqttPacket::publish("a/b", std::string("hi")).bytes()));
        CHECK(MqttParser::isStructurallyValidPublish(
            MqttPacket::publish("t", std::string("")).bytes()));            // empty payload
        CHECK(MqttParser::isStructurallyValidPublish(
            MqttPacket::publish("a/b", std::string("hi"), 1, false, 7).bytes()));  // qos1 + msgId
        // A 2-byte Remaining Length PUBLISH (payload pushes it past 127).
        CHECK(MqttParser::isStructurallyValidPublish(
            MqttPacket::publish("t", std::vector<uint8_t>(200, 0xAB)).bytes()));
    }

    TEST_CASE("isStructurallyValidPublish rejects wrong packet type") {
        // CONNACK, PINGRESP, SUBACK are not PUBLISH.
        CHECK_FALSE(MqttParser::isStructurallyValidPublish(MqttPacket::connack(0).bytes()));
        CHECK_FALSE(MqttParser::isStructurallyValidPublish(MqttPacket::pingresp().bytes()));
        CHECK_FALSE(MqttParser::isStructurallyValidPublish(MqttPacket::suback(1, 0).bytes()));
    }

    TEST_CASE("isStructurallyValidPublish rejects truncated packets") {
        std::vector<uint8_t> bytes = MqttPacket::publish("a/b", std::string("hi")).bytes();

        // Remaining Length says more bytes than are present.
        std::vector<uint8_t> truncated = bytes;
        truncated.pop_back();
        CHECK_FALSE(MqttParser::isStructurallyValidPublish(truncated));

        // Empty input.
        CHECK_FALSE(MqttParser::isStructurallyValidPublish(std::vector<uint8_t>{}));

        // Fixed header only, no Remaining Length byte.
        CHECK_FALSE(MqttParser::isStructurallyValidPublish(std::vector<uint8_t>{0x30}));
    }

    TEST_CASE("isStructurallyValidPublish rejects a topic length that overruns the body") {
        // remaining length = 3, but the declared topic length is 0x00FF which
        // cannot fit: 2 (prefix) + 255 (topic) > 3.
        const std::vector<uint8_t> bad = {0x30, 0x03, 0x00, 0xFF, 0x00};
        CHECK_FALSE(MqttParser::isStructurallyValidPublish(bad));
    }

    TEST_CASE("isStructurallyValidPublish rejects a QoS>0 packet with no room for the packet id") {
        // qos1 fixed header (0x32), remaining length 4: topic length prefix (2)
        // + a 2-byte topic leaves no room for the 2-byte packet identifier.
        const std::vector<uint8_t> bad = {0x32, 0x04, 0x00, 0x02, 'a', 'b'};
        CHECK_FALSE(MqttParser::isStructurallyValidPublish(bad));
    }
}
