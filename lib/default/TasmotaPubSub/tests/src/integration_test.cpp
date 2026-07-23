/*
  integration_test.cpp - Tasmota callback-contract integration boundary tests
  for the TasmotaPubSub host test system (task 10.10).

  Feature: tasmota-pubsub-tests

  These tests exercise the *seam between the library and its real consumer*: an
  inbound PUBLISH is scripted into the MockClient, delivered to the *unmodified*
  library via psc.loop(), and dispatched to the CallbackContractAdapter. The
  adapter faithfully reproduces the Tasmota `MqttDataHandler` contract from
  xdrv_02_9_mqtt.ino by writing a NUL byte at index `data_len` of the delivered
  payload buffer (`mqtt_data[data_len] = 0`, Requirement 15.1). Because that
  payload pointer points INTO the library's own working-buffer allocation, the
  NUL write lands relative to that allocation and is therefore validated under
  AddressSanitizer (on by default). This is what catches consumer-side boundary
  bugs such as F-01 (Requirement 15).

  Baseline (TEST_SUITE("baseline")), Requirement 15.2:
    - A normal delivered QoS 0 / QoS 1 PUBLISH runs the adapter's NUL-write path
      end to end (delivered through the library, NUL terminator written into the
      library buffer) with the recorded (topic, payload, length) matching the
      fixture. ASan proves the NUL write is in-bounds.

  Hardening (TEST_SUITE("hardening")), Requirement 15.3 / F-01:
    - Using setBufferSize to create a precise, small working buffer, inbound
      PUBLISH fixtures are delivered at total wire sizes bufferSize-1,
      bufferSize and bufferSize+1. Because the fork reserves a sentinel byte
      (readPacket rejects total_packet >= bufferSize and idx >= bufferSize), the
      exact-buffer and over-buffer packets are REJECTED before dispatch, so the
      delivered length is bounded and the adapter's NUL write stays in-bounds:
      no byte is written past the library buffer allocation (ASan proves it).
      The accepted (bufferSize-1) case runs the real NUL write on the reserved
      spare byte; the recorded length is asserted to be within the buffer bounds
      and the callback dispatch is asserted to match only the accepted case.
      F-01 is hardened, so this is an expected PASS (FINDING_MARKER(F01)).

  Every assertion goes through the public API, the adapter's recorded values,
  and decoded MockClient.outbound() wire bytes - never private members - so the
  suite stays durable across a future MQTT 5 migration (Requirement 19.1). The
  library under test is NOT modified.
*/

#include <cstdint>
#include <string>
#include <vector>

#include "doctest.h"

#include "CallbackContractAdapter.h"
#include "FindingStatus.h"
#include "MockClient.h"
#include "MqttPacket.h"
#include "TestClock.h"
#include "PubSubClient.h"

namespace {

// Connect the client/psc pair with a scripted CONNACK, register the faithful
// MqttDataHandler-contract adapter as the callback, and clear the recorded
// CONNECT bytes so only the packet-under-test remains in outbound().
void connectWithAdapter(MockClient& client, PubSubClient& psc,
                        CallbackContractAdapter& adapter) {
    client.pushPacket(MqttPacket::connack(0));
    psc.setServer("broker.example", 1883);
    psc.setCallback(adapter.callback());
    REQUIRE(psc.connect("integration-client"));
    REQUIRE(psc.connected());
    client.clearOutbound();
}

// Deterministic binary payload of length n (includes 0x00 bytes so the
// round-trip proves the dispatch is binary-safe and length-driven, not
// dependent on C-string termination).
std::vector<uint8_t> makePayload(size_t n) {
    std::vector<uint8_t> v(n);
    for (size_t i = 0; i < n; ++i) {
        v[i] = static_cast<uint8_t>((i * 31u + 7u) & 0xFFu);
    }
    return v;
}

}  // namespace

TEST_SUITE("baseline") {

    // --- 15.2: the adapter NUL-write path runs on a normal delivered PUBLISH -

    // A normal QoS 0 PUBLISH delivered through the library to the adapter runs
    // the real `mqtt_data[data_len] = 0` write into the library working buffer.
    // The recorded (topic, payload, length) matches the fixture and the NUL
    // write is in-bounds (ASan proves it, since the write targets the reserved
    // spare byte of the library allocation).
    TEST_CASE("integration: adapter NUL-write path runs on a normal QoS 0 PUBLISH") {
        struct Case { const char* label; std::string topic; std::vector<uint8_t> payload; };
        std::vector<Case> cases = {
            {"empty payload",   "tele/dev/STATE",  {}},
            {"short payload",   "cmnd/dev/POWER",   {'O','N'}},
            {"binary payload",  "tele/dev/SENSOR",  makePayload(128)},
            {"single-char topic","t",               makePayload(40)},
        };

        for (const Case& c : cases) {
            CAPTURE(c.label);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            CallbackContractAdapter adapter;
            connectWithAdapter(client, psc, adapter);

            client.pushPacket(MqttPacket::publish(c.topic, c.payload, /*qos=*/0));
            REQUIRE(psc.loop());

            // The adapter recorded exactly one invocation (the payload copy is
            // captured BEFORE the NUL write) and then performed the NUL write
            // into the delivered library buffer - clean under ASan.
            REQUIRE(adapter.performsNulWrite());
            REQUIRE(adapter.count() == 1);
            CHECK(adapter.last().topic == c.topic);
            CHECK(adapter.last().length == c.payload.size());
            CHECK(adapter.last().payload == c.payload);

            // A QoS 0 PUBLISH is not acknowledged; the connection stays healthy.
            CHECK(client.outbound().empty());
            CHECK(psc.connected());
            CHECK_FALSE(client.stopCalled());
        }
    }

    // A QoS 1 PUBLISH exercises the same adapter NUL-write contract while the
    // library additionally records a PUBACK outbound. This confirms the
    // adapter path is driven correctly on the acknowledged receive path too.
    TEST_CASE("integration: adapter NUL-write path runs on a QoS 1 PUBLISH and a PUBACK is emitted") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        CallbackContractAdapter adapter;
        connectWithAdapter(client, psc, adapter);

        const std::string topic = "cmnd/dev/POWER";
        const std::vector<uint8_t> payload = makePayload(64);
        const uint16_t msgId = 0x1234;

        client.pushPacket(MqttPacket::publish(topic, payload, /*qos=*/1,
                                              /*retained=*/false, msgId));
        REQUIRE(psc.loop());

        REQUIRE(adapter.count() == 1);
        CHECK(adapter.last().topic == topic);
        CHECK(adapter.last().length == payload.size());
        CHECK(adapter.last().payload == payload);

        // The PUBACK echoing the message id is recorded outbound.
        DecodedPacket ack = MqttParser::decode(client.outbound());
        REQUIRE(ack.valid);
        CHECK(ack.type == static_cast<uint8_t>(MQTTPUBACK));
        CHECK(ack.remainingLength == 2);
        REQUIRE(ack.payload.size() == 2);
        const uint16_t ackMsgId =
            static_cast<uint16_t>((ack.payload[0] << 8) | ack.payload[1]);
        CHECK(ackMsgId == msgId);

        CHECK(psc.connected());
    }
}

TEST_SUITE("hardening") {

    // --- 15.3 / F-01: exact-buffer inbound PUBLISH boundary under ASan -------

    // Feature: tasmota-pubsub-tests
    //
    // An exact-buffer inbound PUBLISH must not cause the adapter's
    // `mqtt_data[data_len] = 0` write to access memory beyond the library
    // buffer allocation (Requirement 15.3). setBufferSize creates precise, small
    // working buffers; PUBLISH fixtures are delivered at total wire sizes
    // bufferSize-1, bufferSize and bufferSize+1:
    //   total == bufferSize-1 : ACCEPTED -> callback fires; the adapter NUL write
    //                                       lands on the reserved spare byte
    //                                       buffer[bufferSize-1] (in-bounds).
    //   total == bufferSize   : REJECTED -> no callback; connection closed.
    //   total == bufferSize+1 : REJECTED -> no callback; connection closed.
    // Because the exact/over-buffer packets are rejected before dispatch, the
    // delivered length is bounded and no byte is ever written past the library
    // allocation. AddressSanitizer proves the boundary; the assertions also pin
    // the recorded length to within the buffer bounds and require the callback
    // to fire only for the accepted case. F-01 is hardened => expected PASS.
    TEST_CASE("F-01 integration: exact-buffer inbound PUBLISH keeps the adapter NUL write in-bounds"
              * FINDING_MARKER(F01)) {
        // Several precise buffer sizes. topicLen == 1 => total wire size of a
        // QoS 0 PUBLISH = 1 (fixed) + 1 (RL byte, RL < 128) + 2 (topic-length
        // field) + 1 (topic) + payloadLen = 5 + payloadLen, so payloadLen =
        // total - 5. Sizes are kept small enough that the Remaining Length stays
        // in a single byte (RL = 2 + topicLen + payloadLen < 128).
        const uint16_t bufferSizes[] = {32, 64, 100};
        const std::string topic = "t";  // topicLen == 1

        struct Case { const char* label; int totalDelta; bool accepted; };
        const Case cases[] = {
            {"bufferSize-1 (accepted)", -1, true},
            {"bufferSize   (rejected)",  0, false},
            {"bufferSize+1 (rejected)", +1, false},
        };

        for (uint16_t kBufferSize : bufferSizes) {
            for (const Case& c : cases) {
                CAPTURE(kBufferSize);
                CAPTURE(c.label);
                TestClock::instance().reset();
                MockClient client;
                PubSubClient psc(client);
                CallbackContractAdapter adapter;

                // Register the faithful MqttDataHandler-contract adapter so the
                // accepted case performs the real NUL-termination write into the
                // library buffer under ASan.
                connectWithAdapter(client, psc, adapter);

                // Precisely size the working buffer AFTER connect (CONNECT needs
                // the default buffer); the three boundary totals are then exact.
                REQUIRE(psc.setBufferSize(kBufferSize));
                REQUIRE(psc.getBufferSize() == kBufferSize);

                const int total = static_cast<int>(kBufferSize) + c.totalDelta;
                const size_t payloadLen = static_cast<size_t>(total) - 5;
                const std::vector<uint8_t> payload = makePayload(payloadLen);

                // Built structurally via the packet builder (sized vectors only,
                // so the test code performs no out-of-bounds writes).
                const MqttPacket pkt = MqttPacket::publish(topic, payload, /*qos=*/0);
                REQUIRE(pkt.size() == static_cast<size_t>(total));  // fixture is exact
                client.pushPacket(pkt);

                psc.setSocketTimeout(1);  // bound any read as a safety net
                psc.loop();

                if (c.accepted) {
                    // Delivered exactly once with the fixture payload; the NUL
                    // write (adapter, enabled by default) landed on the reserved
                    // sentinel byte buffer[bufferSize-1] - no byte written past
                    // the library allocation (ASan proves it).
                    REQUIRE(adapter.count() == 1);
                    CHECK(adapter.last().length == payloadLen);
                    CHECK(adapter.last().payload == payload);
                    // The delivered length + its NUL terminator stay within the
                    // library buffer bounds (data_len < bufferSize).
                    CHECK(adapter.last().length < kBufferSize);
                    CHECK(psc.connected());
                    CHECK_FALSE(client.stopCalled());
                } else {
                    // Rejected before dispatch: the adapter is never invoked, so
                    // no NUL write occurs at or beyond buffer[bufferSize]; the
                    // connection is closed.
                    CHECK(adapter.count() == 0);
                    CHECK_FALSE(psc.connected());
                    CHECK(client.stopCalled());
                }
            }
        }
    }
}
