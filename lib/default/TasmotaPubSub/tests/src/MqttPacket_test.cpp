/*
  MqttPacket_test.cpp - Unit checks for the MqttPacket builder and the
  Remaining Length codec (task 7.1).

  These verify the fixture builder itself (not the library under test): each
  builder must emit the exact wire byte layout of the MQTT 3.1.1 control packet,
  and the Remaining Length encoder must match PubSubClient::buildHeader across
  the 1..4 byte boundaries. Tagged into the baseline suite so they run as part
  of the standard non-regression pass.
*/

#include <cstdint>
#include <string>
#include <vector>

#include "doctest.h"

#include "MockClient.h"
#include "MqttPacket.h"

TEST_SUITE("baseline") {

    // --- Remaining Length codec (mirrors buildHeader) ----------------------

    TEST_CASE("MqttPacket encodeRemainingLength matches the MQTT boundaries") {
        using V = std::vector<uint8_t>;

        // 1-byte range: 0..127.
        CHECK(MqttPacket::encodeRemainingLength(0) == V{0x00});
        CHECK(MqttPacket::encodeRemainingLength(1) == V{0x01});
        CHECK(MqttPacket::encodeRemainingLength(127) == V{0x7F});

        // 2-byte range: 128..16383.
        CHECK(MqttPacket::encodeRemainingLength(128) == V{0x80, 0x01});
        CHECK(MqttPacket::encodeRemainingLength(16383) == V{0xFF, 0x7F});

        // 3-byte range: 16384..2097151.
        CHECK(MqttPacket::encodeRemainingLength(16384) == V{0x80, 0x80, 0x01});
        CHECK(MqttPacket::encodeRemainingLength(2097151) == V{0xFF, 0xFF, 0x7F});

        // 4-byte range: 2097152..268435455.
        CHECK(MqttPacket::encodeRemainingLength(2097152) == V{0x80, 0x80, 0x80, 0x01});
        CHECK(MqttPacket::encodeRemainingLength(268435455) == V{0xFF, 0xFF, 0xFF, 0x7F});
    }

    TEST_CASE("MqttPacket encodeRemainingLength is bounded to four bytes") {
        // Values above the MQTT maximum are clamped to a 4-byte encoding (never
        // a fifth byte), exactly as the library's buildHeader loop is bounded.
        CHECK(MqttPacket::encodeRemainingLength(0xFFFFFFFFu).size() == 4);
    }

    // --- Builders: exact byte layouts --------------------------------------

    TEST_CASE("MqttPacket connack layout") {
        using V = std::vector<uint8_t>;
        CHECK(MqttPacket::connack(0).bytes() == V{0x20, 0x02, 0x00, 0x00});
        CHECK(MqttPacket::connack(5).bytes() == V{0x20, 0x02, 0x00, 0x05});
        // Session-present flag sets bit0 of the acknowledge flags byte.
        CHECK(MqttPacket::connack(0, true).bytes() == V{0x20, 0x02, 0x01, 0x00});
    }

    TEST_CASE("MqttPacket publish QoS 0 has no packet identifier") {
        using V = std::vector<uint8_t>;
        // topic "a/b" (len 3), payload "hi" (len 2) => remaining length 7.
        MqttPacket p = MqttPacket::publish("a/b", std::string("hi"));
        const V expected = {
            0x30,              // MQTTPUBLISH, qos 0, no retain
            0x07,              // remaining length = 2 + 3 + 2
            0x00, 0x03,        // topic length
            'a', '/', 'b',     // topic
            'h', 'i'           // payload
        };
        CHECK(p.bytes() == expected);
    }

    TEST_CASE("MqttPacket publish retained flag sets bit0 of the fixed header") {
        MqttPacket p = MqttPacket::publish("t", std::string(""), 0, true);
        CHECK(p.bytes().at(0) == 0x31);   // MQTTPUBLISH | retain
    }

    TEST_CASE("MqttPacket publish QoS 1 embeds the packet identifier after the topic") {
        using V = std::vector<uint8_t>;
        MqttPacket p = MqttPacket::publish("a/b", std::string("hi"), 1, false, 0x1234);
        const V expected = {
            0x32,              // MQTTPUBLISH | qos1 (1<<1)
            0x09,              // remaining length = 2 + 3 + 2(msgId) + 2
            0x00, 0x03,        // topic length
            'a', '/', 'b',     // topic
            0x12, 0x34,        // packet identifier
            'h', 'i'           // payload
        };
        CHECK(p.bytes() == expected);
    }

    TEST_CASE("MqttPacket publish empty payload frames correctly") {
        using V = std::vector<uint8_t>;
        MqttPacket p = MqttPacket::publish("x", std::vector<uint8_t>{});
        const V expected = {0x30, 0x03, 0x00, 0x01, 'x'};
        CHECK(p.bytes() == expected);
    }

    TEST_CASE("MqttPacket publish frames a 2-byte remaining length past 127") {
        // A payload that pushes the total remaining length above 127 must use a
        // 2-byte Remaining Length. topic "t" (len 1) => body = 2 + 1 + plen.
        const size_t plen = 200;
        MqttPacket p = MqttPacket::publish("t", std::vector<uint8_t>(plen, 0xAB));
        const uint32_t remaining = 2 + 1 + plen;   // 203
        REQUIRE(p.bytes().size() >= 2);
        CHECK(p.bytes().at(0) == 0x30);
        CHECK(p.bytes().at(1) == 0xCB);            // 203 & 127 | 0x80 = 0xCB
        CHECK(p.bytes().at(2) == 0x01);            // 203 >> 7 = 1
        // Total wire size = 1 (fixed) + 2 (remaining length) + body.
        CHECK(p.bytes().size() == 1 + 2 + remaining);
    }

    TEST_CASE("MqttPacket ack/ping/disconnect layouts") {
        using V = std::vector<uint8_t>;
        CHECK(MqttPacket::puback(0x0102).bytes() == V{0x40, 0x02, 0x01, 0x02});
        CHECK(MqttPacket::suback(0x0102, 0x01).bytes() == V{0x90, 0x03, 0x01, 0x02, 0x01});
        CHECK(MqttPacket::unsuback(0x0102).bytes() == V{0xB0, 0x02, 0x01, 0x02});
        CHECK(MqttPacket::pingreq().bytes() == V{0xC0, 0x00});
        CHECK(MqttPacket::pingresp().bytes() == V{0xD0, 0x00});
        CHECK(MqttPacket::disconnect().bytes() == V{0xE0, 0x00});
    }

    TEST_CASE("MqttPacket raw passes bytes through verbatim") {
        using V = std::vector<uint8_t>;
        const V adversarial = {0x30, 0xFF, 0xFF, 0xFF, 0x7F};   // huge declared length
        CHECK(MqttPacket::raw(adversarial).bytes() == adversarial);
    }

    // --- Compatibility with MockClient::pushPacket -------------------------

    TEST_CASE("MockClient pushPacket enqueues the packet's exact wire bytes") {
        MockClient c;
        MqttPacket connack = MqttPacket::connack(0);
        c.pushPacket(connack);

        CHECK(c.available() == static_cast<int>(connack.size()));
        for (uint8_t expected : connack.bytes()) {
            CHECK(c.read() == expected);
        }
        CHECK(c.available() == 0);
    }
}
