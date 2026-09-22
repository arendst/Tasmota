/*
  error_model_test.cpp - Unit tests for the MqttError model and the public error
  accessors (task 3.1).

  Feature: tasmota-pubsub-mqtt5

  The error model is version-neutral: `MqttError` and the four accessors
  (lastError / lastReasonCode / lastPacketId / serverDisconnectReasonCode) are
  compiled into BOTH selections, because the codecs and bounds checks that report
  them are themselves version-neutral. This file is therefore deliberately NOT
  wrapped in a TEST_SUITE, so it runs in both binaries - the same convention
  vbi_test.cpp uses.

  Scope at task 3.1 is the model plus the accessors plus their reset at the start
  of every connection attempt. Nothing yet *sets* a non-None value: the single
  failure funnel `protocolFailure()` arrives in task 3.2, and the tests that drive
  each MqttError value onto the accessors come with the tasks that raise them.
  What is pinned here is the contract every one of those later tests relies on:
  the accessors are const, they report `None`/0 before anything fails, and a
  connection attempt always starts from that cleared state.

  Requirements: 14.2
*/

#include <cstdint>
#include <type_traits>

#include "doctest.h"

#include "MockClient.h"
#include "MqttPacket.h"
#include "TestClock.h"
#include "PubSubClient.h"

namespace error_model_test {

// The four accessors must be usable on a const client - a driver holding a const
// reference has to be able to read the diagnostics. Taking them through a const
// reference here makes a missing `const` a compile error rather than a silent
// API regression.
void readAll(const PubSubClient& psc, MqttError& err, uint8_t& rc, uint16_t& pid, uint8_t& dc) {
    err = psc.lastError();
    rc  = psc.lastReasonCode();
    pid = psc.lastPacketId();
    dc  = psc.serverDisconnectReasonCode();
}

// All four report "nothing has failed".
void checkCleared(const PubSubClient& psc) {
    MqttError err = MqttError::ServerDisconnect;
    uint8_t rc = 0xFF, dc = 0xFF;
    uint16_t pid = 0xFFFF;
    readAll(psc, err, rc, pid, dc);
    CHECK(err == MqttError::None);
    CHECK(rc == 0);
    CHECK(pid == 0);
    CHECK(dc == 0);
}

}  // namespace error_model_test

TEST_CASE("MqttError is a uint8_t-backed enum class with None as the zero value") {
    // The underlying type is part of the model: the value is stored in a single
    // byte member, and `None` must be the zero value so a zero-initialized member
    // means "no error".
    static_assert(std::is_same<std::underlying_type<MqttError>::type, uint8_t>::value,
                  "MqttError must be backed by uint8_t");
    static_assert(!std::is_convertible<MqttError, int>::value,
                  "MqttError must be a scoped enum (enum class)");
    CHECK(static_cast<uint8_t>(MqttError::None) == 0);

    // Every value the design names is present and distinct. Listing them here
    // means removing or renaming one is a compile error.
    const MqttError all[] = {
        MqttError::None,
        MqttError::NotConnected,
        MqttError::BufferTooSmall,
        MqttError::PacketTooLarge,
        MqttError::MalformedPacket,
        MqttError::ProtocolError,
        MqttError::PacketIdUnavailable,
        MqttError::ReceiveMaximumReached,
        MqttError::RetentionUnavailable,
        MqttError::WriteFailed,
        MqttError::ReceiveTimeout,
        MqttError::ServerDisconnect,
    };
    const size_t count = sizeof(all) / sizeof(all[0]);
    CHECK(count == 12);
    for (size_t i = 0; i < count; i++) {
        for (size_t j = i + 1; j < count; j++) {
            CAPTURE(i);
            CAPTURE(j);
            CHECK(all[i] != all[j]);
        }
    }
}

TEST_CASE("a fresh client reports no error through all four accessors") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    error_model_test::checkCleared(psc);

    // Also true for the default-constructed form, which has no client at all.
    PubSubClient bare;
    error_model_test::checkCleared(bare);
}

TEST_CASE("the error diagnostics are cleared at the start of every connection attempt") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);

    // An attempt that fails before any packet is assembled (no server configured)
    // still starts from the cleared state.
    CHECK_FALSE(psc.connect("client-1"));
    error_model_test::checkCleared(psc);

    // And so does a successful one.
    client.pushPacket(MqttPacket::connack(0));
    psc.setServer("broker.example", 1883);
    REQUIRE(psc.connect("client-1"));
    error_model_test::checkCleared(psc);

    // The accessors stay readable for the whole connection lifetime.
    CHECK(psc.connected());
    error_model_test::checkCleared(psc);
}

// ---------------------------------------------------------------------------
// Task 3.2: the protocolFailure() funnel.
//
// Also version-neutral in the sense that the funnel itself is compiled in both
// selections - what differs is the wire emission, which is MQTT 5.0 only and
// therefore branched on kMqtt5 below. In the 3.1.1 build a protocol failure
// closes silently, exactly as today.
//
// Requirements: 10.5, 10.6, 14.1, 14.2
// ---------------------------------------------------------------------------

namespace error_model_test {

// Bring a client up on a scripted CONNACK so the failure paths start from a live
// connection with an empty outbound record.
void connectOk(PubSubClient& psc, MockClient& client) {
    client.pushPacket(MqttPacket::connack(0));
    psc.setServer("broker.example", 1883);
    REQUIRE(psc.connect("client-1"));
    client.clearOutbound();
}

}  // namespace error_model_test

TEST_CASE("protocolFailure records the failure, emits E0 01 <rc> when writable, and closes") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    error_model_test::connectOk(psc, client);

    psc.protocolFailure(MqttError::MalformedPacket, PubSubClient::MQTT_RC_MALFORMED_PACKET);

    // The diagnostics survive the close - they are what the driver reads afterwards.
    CHECK(psc.lastError() == MqttError::MalformedPacket);
    CHECK(psc.lastReasonCode() == 0x81);

    if (kMqtt5) {
        // Requirement 10.5: exactly one DISCONNECT with the error Reason Code, no properties.
        const std::vector<uint8_t> expected = {0xE0, 0x01, 0x81};
        CHECK(client.outbound() == expected);
    } else {
        // The 3.1.1 selection emits nothing: no DISCONNECT reason codes exist there.
        CHECK(client.outbound().empty());
    }

    // Closed, and reported as such.
    CHECK(client.stopCalled());
    CHECK(client.stopCount() == 1);
    CHECK_FALSE(psc.connected());
    CHECK(psc.state() == MQTT_DISCONNECTED);
}

TEST_CASE("protocolFailure on an unwritable connection closes without emitting a byte") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    error_model_test::connectOk(psc, client);

    // The transport is gone underneath us (Requirement 10.6).
    client.setConnected(false);

    psc.protocolFailure(MqttError::ProtocolError, PubSubClient::MQTT_RC_PROTOCOL_ERROR);

    CHECK(client.outbound().empty());
    CHECK(psc.lastError() == MqttError::ProtocolError);
    CHECK(psc.lastReasonCode() == 0x82);
    CHECK(client.stopCalled());
    CHECK(psc.state() == MQTT_DISCONNECTED);
}

TEST_CASE("protocolFailure is safe with no transport and no buffer") {
    TestClock::instance().reset();
    // Default-constructed: no client, no buffer. Nothing to write to, nothing to close.
    PubSubClient bare;
    bare.protocolFailure(MqttError::MalformedPacket);
    CHECK(bare.lastError() == MqttError::MalformedPacket);
    CHECK(bare.lastReasonCode() == 0x81);
    CHECK(bare.state() == MQTT_DISCONNECTED);
}

TEST_CASE("the reason-code selection table maps every closing failure category") {
    // The categories that close with a code of their own.
    CHECK(PubSubClient::defaultReasonCode(MqttError::MalformedPacket) == 0x81);
    CHECK(PubSubClient::defaultReasonCode(MqttError::ProtocolError) == 0x82);
    CHECK(PubSubClient::defaultReasonCode(MqttError::ReceiveMaximumReached) == 0x93);
    CHECK(PubSubClient::defaultReasonCode(MqttError::PacketTooLarge) == 0x95);

    // Every emitted code is a failure code, i.e. 0x80 or above (Requirement 10.5).
    const MqttError closing[] = {
        MqttError::MalformedPacket,
        MqttError::ProtocolError,
        MqttError::ReceiveMaximumReached,
        MqttError::PacketTooLarge,
    };
    for (MqttError e : closing) {
        CHECK(PubSubClient::defaultReasonCode(e) >= 0x80);
    }

    // The named constants are the table's entries, including the two that are only
    // ever passed explicitly.
    CHECK(PubSubClient::MQTT_RC_NORMAL_DISCONNECTION == 0x00);
    CHECK(PubSubClient::MQTT_RC_BAD_AUTH_METHOD == 0x8C);

    // Categories that do not close with a reason code report none.
    CHECK(PubSubClient::defaultReasonCode(MqttError::None) == 0);
    CHECK(PubSubClient::defaultReasonCode(MqttError::NotConnected) == 0);
    CHECK(PubSubClient::defaultReasonCode(MqttError::BufferTooSmall) == 0);
    CHECK(PubSubClient::defaultReasonCode(MqttError::PacketIdUnavailable) == 0);
    CHECK(PubSubClient::defaultReasonCode(MqttError::WriteFailed) == 0);
    CHECK(PubSubClient::defaultReasonCode(MqttError::ReceiveTimeout) == 0);
    CHECK(PubSubClient::defaultReasonCode(MqttError::ServerDisconnect) == 0);
}

TEST_CASE("an AUTH-style failure carries 0x8C while still reporting ProtocolError") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    error_model_test::connectOk(psc, client);

    // The AUTH trigger is the one ProtocolError whose code is not the category default.
    psc.protocolFailure(MqttError::ProtocolError, PubSubClient::MQTT_RC_BAD_AUTH_METHOD);

    CHECK(psc.lastError() == MqttError::ProtocolError);
    CHECK(psc.lastReasonCode() == 0x8C);
    if (kMqtt5) {
        const std::vector<uint8_t> expected = {0xE0, 0x01, 0x8C};
        CHECK(client.outbound() == expected);
    } else {
        CHECK(client.outbound().empty());
    }
}

TEST_CASE("a connection attempt after protocolFailure proceeds normally") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    error_model_test::connectOk(psc, client);

    psc.protocolFailure(MqttError::MalformedPacket);
    REQUIRE(psc.state() == MQTT_DISCONNECTED);

    // Requirement 14.2: nothing about the object blocks the driver's next connect.
    client.clearOutbound();
    client.setConnected(true);
    client.pushPacket(MqttPacket::connack(0));
    REQUIRE(psc.connect("client-1"));
    CHECK(psc.connected());
    // And the diagnostics were cleared by the new attempt.
    error_model_test::checkCleared(psc);

    // A keepalive-driven loop() right after reconnecting still works, which is the
    // observable consequence of the funnel having cleared the outstanding ping.
    CHECK(psc.loop());
}

TEST_CASE("disconnect() keeps emitting the short form E0 00 and nothing when not requested") {
    TestClock::instance().reset();
    MockClient client;
    PubSubClient psc(client);
    error_model_test::connectOk(psc, client);

    // Default argument: no packet at all, just a close.
    psc.disconnect();
    CHECK(client.outbound().empty());
    CHECK(client.stopCalled());
    CHECK(psc.state() == MQTT_DISCONNECTED);

    // Explicit packet: the 2-byte short form, identical in both selections.
    MockClient client2;
    PubSubClient psc2(client2);
    error_model_test::connectOk(psc2, client2);
    psc2.disconnect(true);
    const std::vector<uint8_t> expected = {0xE0, 0x00};
    CHECK(client2.outbound() == expected);
    CHECK(client2.stopCalled());
}
