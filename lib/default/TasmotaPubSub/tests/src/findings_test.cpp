/*
  findings_test.cpp - one consolidated adversarial/boundary fixture per
  static-analysis finding F-01..F-12 for the TasmotaPubSub host test system
  (task 10.9).

  Feature: tasmota-pubsub-tests

  Requirement 14.1 asks that EVERY documented finding (F-01..F-12) have at least
  one corresponding executable check. Several findings are already exercised in
  the behavior-specific suites (connect/publish/streaming/subscribe/receive/
  keepalive/buffer/api-surface); this file provides one clear, self-contained
  fixture per finding in a single place so the finding->test mapping is obvious
  and auditable.

  Rules honored throughout:
    - Adversarial fixtures are built ONLY via MqttPacket / sized
      std::vector<uint8_t>, so the test code itself performs no out-of-bounds
      writes (Requirement 14.3). Any over-read/over-write is therefore the
      library's, and AddressSanitizer (on by default) is the detector
      (Requirement 14.2).
    - F-01 uses inbound PUBLISH fixtures at total packet sizes bufferSize-1,
      bufferSize and bufferSize+1 (Requirement 14.4); setBufferSize shrinks the
      working buffer AFTER connecting so these boundaries are small and precise.
    - Every assertion goes through the public API, the registered callback, and
      decoded MockClient.outbound() wire bytes - never private members
      (Requirement 19.1).
    - Expected-FAIL findings (F-05, F-10, F-11 and the F-03 packet-wide deadline)
      carry FINDING_MARKER(...) expanding to should_fail(); the rest are expected
      to pass against the already-partially-hardened fork.

  Observed status is verified empirically by `make hardening` / `make test`; the
  FINDING_MARKER for each case matches the FindingStatus.h seeding.
*/

#include <cstdint>
#include <string>
#include <vector>

#include "doctest.h"

#include "AllocInterposer.h"
#include "CallbackContractAdapter.h"
#include "FindingStatus.h"
#include "MockClient.h"
#include "MqttPacket.h"
#include "TestClock.h"
#include "PubSubClient.h"

namespace {

// Records every inbound PUBLISH the library dispatches to the callback so a
// rejection can be proven by the callback never firing.
struct CallbackCapture {
    int                  count = 0;
    std::string          topic;
    unsigned int         length = 0;
    std::vector<uint8_t> payload;
};

// Connect the client/psc pair with a scripted CONNACK, register a recording
// callback, and clear the recorded CONNECT bytes so only the packet-under-test
// remains in outbound().
void connectWithCallback(MockClient& client, PubSubClient& psc, CallbackCapture& cap) {
    client.pushPacket(MqttPacket::connack(0));
    psc.setServer("broker.example", 1883);
    psc.setCallback([&cap](char* topic, uint8_t* payload, unsigned int len) {
        cap.count++;
        cap.topic = topic;
        cap.length = len;
        cap.payload.assign(payload, payload + len);
    });
    REQUIRE(psc.connect("findings-client"));
    REQUIRE(psc.connected());
    client.clearOutbound();
}

// Connect the client/psc pair with a scripted CONNACK and clear the recorded
// CONNECT bytes; no callback registered.
void connectAndClear(MockClient& client, PubSubClient& psc) {
    client.pushPacket(MqttPacket::connack(0));
    psc.setServer("broker.example", 1883);
    REQUIRE(psc.connect("findings-client"));
    REQUIRE(psc.connected());
    client.clearOutbound();
}

// Deterministic payload of length n (includes 0x00 bytes so round-trips prove
// binary-safety, not C-string termination).
std::vector<uint8_t> makePayload(size_t n) {
    std::vector<uint8_t> v(n);
    for (size_t i = 0; i < n; ++i) {
        v[i] = static_cast<uint8_t>((i * 31u + 7u) & 0xFFu);
    }
    return v;
}

}  // namespace

TEST_SUITE("hardening") {

    // =======================================================================
    //  F-01 - Exact-buffer inbound one-byte heap overflow (Requirement 14.4)
    // =======================================================================
    //
    // A PUBLISH whose complete encoded packet is exactly bufferSize bytes would,
    // in the unpatched code, let the consumer write a NUL at buffer[bufferSize] -
    // one byte past the allocation. The fork reserves a sentinel byte: readPacket
    // rejects total_packet >= bufferSize (and idx >= bufferSize). So:
    //   total == bufferSize-1 : ACCEPTED  -> callback fires, NUL lands at the
    //                                        reserved buffer[bufferSize-1] (in-bounds)
    //   total == bufferSize   : REJECTED  -> connection closed, no callback
    //   total == bufferSize+1 : REJECTED  -> connection closed, no callback
    //
    // The CallbackContractAdapter reproduces the real Tasmota `mqtt_data[data_len]
    // = 0` write into the delivered (in-library) buffer, so the accepted case
    // exercises the exact consumer-side boundary write under AddressSanitizer. If
    // the guard were off-by-one, the accepted case's NUL write would hit
    // buffer[bufferSize] and ASan would abort. F-01 is hardened (expected PASS).
    TEST_CASE("F-01 exact-buffer inbound PUBLISH boundaries stay in-bounds under ASan"
              * FINDING_MARKER(F01)) {
        // A small, precise working buffer so the three boundary totals are exact.
        // The buffer is shrunk AFTER connect (CONNECT needs the default buffer).
        const uint16_t kBufferSize = 64;
        const std::string topic = "t";                 // topicLen == 1
        // total wire size of a QoS 0 PUBLISH = 1 (fixed) + 1 (RL byte, RL<128)
        //   + 2 (topic-length field) + topicLen + payloadLen = 4 + topicLen + payloadLen.
        // With topicLen == 1 => total = 5 + payloadLen, so payloadLen = total - 5.
        struct Case { const char* label; int totalDelta; bool accepted; };
        const Case cases[] = {
            {"bufferSize-1 (accepted)", -1, true},
            {"bufferSize   (rejected)",  0, false},
            {"bufferSize+1 (rejected)", +1, false},
        };

        for (const Case& c : cases) {
            CAPTURE(c.label);
            TestClock::instance().reset();
            AllocInterposer::reset();
            MockClient client;
            PubSubClient psc(client);

            // Register the faithful MqttDataHandler-contract adapter as the
            // callback so the accepted case performs the real NUL-termination
            // write into the library buffer under ASan.
            CallbackContractAdapter adapter;
            client.pushPacket(MqttPacket::connack(0));
            psc.setServer("broker.example", 1883);
            psc.setCallback(adapter.callback());
            REQUIRE(psc.connect("f01-client"));
            REQUIRE(psc.connected());
            client.clearOutbound();

            // Precisely size the buffer for the boundary math.
            REQUIRE(psc.setBufferSize(kBufferSize));
            REQUIRE(psc.getBufferSize() == kBufferSize);

            const int total = static_cast<int>(kBufferSize) + c.totalDelta;
            const size_t payloadLen = static_cast<size_t>(total) - 5;
            const std::vector<uint8_t> payload = makePayload(payloadLen);

            // Built structurally via the packet builder (sized vectors only).
            const MqttPacket pkt = MqttPacket::publish(topic, payload, /*qos=*/0);
            REQUIRE(pkt.size() == static_cast<size_t>(total));  // fixture is exact
            client.pushPacket(pkt);

            psc.setSocketTimeout(1);   // bound any read as a safety net
            psc.loop();

            if (c.accepted) {
                // Delivered exactly once with the fixture payload; the NUL write
                // (adapter, enabled by default) landed on the reserved sentinel
                // byte buffer[bufferSize-1] - clean under ASan.
                REQUIRE(adapter.count() == 1);
                CHECK(adapter.last().length == payloadLen);
                CHECK(adapter.last().payload == payload);
                CHECK(psc.connected());
                CHECK_FALSE(client.stopCalled());
            } else {
                // Rejected before dispatch: no callback, connection closed.
                CHECK(adapter.count() == 0);
                CHECK_FALSE(psc.connected());
                CHECK(client.stopCalled());
            }
        }
    }

    // =======================================================================
    //  F-02 - PUBLISH parsing trusts topic length instead of packet length
    // =======================================================================
    //
    // A QoS 1 PUBLISH whose 2-byte topic-length field claims far more bytes than
    // the packet actually contains must be rejected without reading past the
    // received bytes, and a short PUBLISH whose length arithmetic would underflow
    // must not reach the callback with a huge length. The fork's guard
    // (header_len = llen + 3 + tl + (qos?2:0) > len) fires first. All reads stay
    // within the scripted bytes, so ASan proves no over-read. Expected PASS.
    TEST_CASE("F-02 PUBLISH with claimed topic length exceeding the packet is rejected (no over-read)"
              * FINDING_MARKER(F02)) {
        // {fixed header, Remaining Length byte, claimed topic length, body bytes}.
        // The claimed topic length far exceeds the body, and for the underflow
        // cases the QoS 0 payload length (len - llen - 3 - tl) would wrap.
        struct Case { const char* label; uint8_t fixedHeader; uint8_t bodyLen; uint16_t claimedTopicLen; };
        const Case cases[] = {
            {"QoS1 tl=500 body=4",   static_cast<uint8_t>(MQTTPUBLISH | MQTTQOS1), 4,  500},
            {"QoS1 tl=0x7FFF body=10",static_cast<uint8_t>(MQTTPUBLISH | MQTTQOS1),10, 0x7FFF},
            {"QoS0 underflow tl=10 body=5", static_cast<uint8_t>(MQTTPUBLISH),      5,  10},
            {"QoS0 underflow tl=5 body=2",  static_cast<uint8_t>(MQTTPUBLISH),      2,  5},
        };

        for (const Case& c : cases) {
            CAPTURE(c.label);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            CallbackCapture cap;
            connectWithCallback(client, psc, cap);

            // Sized vector => no OOB in the test code itself.
            std::vector<uint8_t> frame;
            frame.push_back(c.fixedHeader);
            frame.push_back(c.bodyLen);                                       // RL < 128
            frame.push_back(static_cast<uint8_t>(c.claimedTopicLen >> 8));    // topic len hi
            frame.push_back(static_cast<uint8_t>(c.claimedTopicLen & 0xFF));  // topic len lo
            for (uint8_t i = 2; i < c.bodyLen; ++i) {
                frame.push_back(static_cast<uint8_t>('x'));
            }
            client.pushPacket(MqttPacket::raw(frame));

            psc.setSocketTimeout(1);
            psc.loop();

            // No callback (so no over-read and no underflowed length), closed.
            CHECK(cap.count == 0);
            CHECK_FALSE(psc.connected());
            CHECK(client.stopCalled());
        }
    }

    // =======================================================================
    //  F-03 (a) - Oversized inbound packet closes promptly (Requirement 13.4)
    // =======================================================================
    //
    // An inbound packet declaring a Remaining Length larger than the accepted
    // capacity must close the connection after a bounded prefix rather than
    // draining the whole declared body. The fixture supplies ONLY the fixed
    // header + Remaining Length bytes (no body): a prompt-close implementation
    // calls stop() as soon as it sees the oversized length; a drain-the-body
    // implementation would block on the empty queue and time out WITHOUT calling
    // stop(). Asserting stopCalled() proves the prompt close. Expected PASS.
    TEST_CASE("F-03 oversized declared inbound packet closes the connection promptly"
              * FINDING_MARKER(F03_PROMPT_CLOSE)) {
        const uint32_t declaredLengths[] = {2000, 300000, 5000000, 200000000};

        for (uint32_t rl : declaredLengths) {
            CAPTURE(rl);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            connectAndClear(client, psc);

            std::vector<uint8_t> frame;
            frame.push_back(static_cast<uint8_t>(MQTTPUBLISH));
            const std::vector<uint8_t> rlBytes = MqttPacket::encodeRemainingLength(rl);
            frame.insert(frame.end(), rlBytes.begin(), rlBytes.end());
            client.pushInbound(frame);

            psc.setSocketTimeout(1);
            const bool looped = psc.loop();

            CHECK(client.stopCalled());
            CHECK_FALSE(looped);
            CHECK_FALSE(psc.connected());
            CHECK(psc.state() == MQTT_DISCONNECTED);
        }
    }

    // =======================================================================
    //  F-03 (b) - Packet-wide read deadline (Requirement 13.5) - EXPECTED FAIL
    // =======================================================================
    //
    // Trickle-fed inbound bytes should be bounded by a packet-wide deadline, not
    // only by a per-byte socket timeout. Here the broker trickles one byte per
    // simulated second while the per-byte timeout is five seconds, so no single
    // byte ever times out. A hardened implementation would still abandon and
    // close the packet once a packet-wide deadline elapsed; the current fork
    // enforces ONLY the per-byte timeout and reads the whole packet to
    // completion, dispatching the callback. The assertions encode the hardened
    // expectation, so this reports an EXPECTED FAILURE.
    TEST_CASE("F-03 trickle-fed inbound bytes are bounded by a packet-wide deadline"
              * FINDING_MARKER(F03_DEADLINE)) {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        CallbackCapture cap;

        psc.setServer("broker.example", 1883);
        psc.setKeepAlive(120);          // keep keepalive well clear of the read
        psc.setCallback([&cap](char* topic, uint8_t* payload, unsigned int len) {
            cap.count++;
            cap.topic = topic;
            cap.length = len;
            cap.payload.assign(payload, payload + len);
        });
        client.pushPacket(MqttPacket::connack(0));
        REQUIRE(psc.connect("f03-deadline-client"));
        REQUIRE(psc.connected());
        client.clearOutbound();

        // A small well-formed QoS 0 PUBLISH trickled one byte per simulated
        // second, inside a 5 s per-byte window. Virtual time advances via the
        // shim's delay(), so this consumes no real wall-clock time.
        const std::string topic = "t";
        const std::vector<uint8_t> payload = {'h', 'i'};
        client.pushPacket(MqttPacket::publish(topic, payload, /*qos=*/0));
        psc.setSocketTimeout(5);
        client.setTrickle(/*bytesPerReveal=*/1, /*msPerReveal=*/1000);

        psc.loop();

        // Hardened expectation (fails against the current fork => expected fail).
        CHECK(client.stopCalled());
        CHECK(cap.count == 0);
    }

    // =======================================================================
    //  F-04 - Streaming publish length 16-bit truncation (Requirement 9.4)
    // =======================================================================
    //
    // beginPublish() buffers only the header (fixed header + Remaining Length +
    // topic) and streams the payload separately, so a very large *declared*
    // length can be framed without allocating the payload. The emitted Remaining
    // Length must be the full 32-bit value (plength + 2 + topicLen), not a 16-bit
    // truncation that would desynchronize framing. The fork uses a uint32_t
    // buildHeader bounded to four length bytes. Expected PASS.
    TEST_CASE("F-04 large declared streaming length frames Remaining Length without truncation"
              * FINDING_MARKER(F04)) {
        const std::string topic = "t";                 // topicLen == 1
        const size_t topicLen = topic.size();
        // Declared payload lengths above the 16-bit range, spanning the 2->3 and
        // 3->4 byte Remaining Length transitions and the MQTT maximum.
        const unsigned int plengths[] = {
            65533u,               // RL = 65536   (first value needing 3 bytes)
            100000u,              // RL = 100003  (3 bytes)
            2097149u,             // RL = 2097152 (first value needing 4 bytes)
            268435455u - 2u - 1u, // RL = 268435455 (MQTT maximum, 4 bytes)
        };

        for (unsigned int plen : plengths) {
            CAPTURE(plen);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            connectAndClear(client, psc);

            REQUIRE(psc.beginPublish(topic.c_str(), plen, false));

            const std::vector<uint8_t>& out = client.outbound();
            REQUIRE(out.size() >= 2);
            CHECK(static_cast<uint8_t>(out[0] & 0xF0) == static_cast<uint8_t>(MQTTPUBLISH));

            uint32_t rl = 0;
            size_t rlBytes = 0;
            REQUIRE(MqttParser::decodeRemainingLength(out, 1, rl, rlBytes));

            const uint32_t expected =
                static_cast<uint32_t>(plen) + 2u + static_cast<uint32_t>(topicLen);
            CHECK(rl == expected);                     // full 32-bit value
            CHECK(rl != (expected & 0xFFFFu));         // NOT a 16-bit truncation

            // Header framing is self-consistent: fixed(1) + RL bytes + topic
            // length prefix(2) + topic. No payload streamed yet.
            CHECK(out.size() == 1u + rlBytes + 2u + topicLen);
        }
    }

    // =======================================================================
    //  F-05 - Partial write corrupts the stream, connection reused - EXPECTED FAIL
    // =======================================================================
    //
    // With a partial transport write injected (setWriteLimit), the publish path
    // should report failure and leave the connection unusable so no later packet
    // is written onto a desynchronized stream. The current fork returns success
    // from endPublish() and keeps the connection, so the hardened assertions
    // below report an EXPECTED FAILURE. Cut points are strictly less than the
    // full packet length.
    TEST_CASE("F-05 partial transport write fails the publish and disables reuse"
              * FINDING_MARKER(F05)) {
        const std::string topic = "s";                 // topicLen == 1
        const std::vector<uint8_t> payload = makePayload(300);
        // Header = fixed(1) + RL(2, RL=303) + topic-len(2) + topic(1) = 6; full
        // packet = 306. Cut points >= 6 (header write completes) and < 300 (the
        // payload write is truncated) - all strictly less than 306.
        const size_t cutPoints[] = {10, 50, 150, 299};

        for (size_t cut : cutPoints) {
            CAPTURE(cut);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            connectAndClear(client, psc);

            client.setWriteLimit(cut);
            REQUIRE(psc.beginPublish(topic.c_str(),
                                     static_cast<unsigned int>(payload.size()), false));

            const size_t written = psc.write(payload.data(), payload.size());
            REQUIRE(written < payload.size());         // a genuine partial write

            // Hardened contract: endPublish reports failure and the connection is
            // left unusable. The current fork does neither => expected failure.
            CHECK(psc.endPublish() == 0);
            CHECK_FALSE(psc.connected());
        }
    }

    // =======================================================================
    //  F-06 - CONNACK response validation (Requirement 8.3)
    // =======================================================================
    //
    // A four-byte inbound frame that is not a well-formed CONNACK (packet type
    // other than 0x20, nonzero fixed flags, or Remaining Length other than 2)
    // must be rejected as a failed connection, regardless of the fourth byte.
    // The fork requires len==4 && buffer[0]==0x20 && buffer[1]==2. Expected PASS.
    TEST_CASE("F-06 malformed four-byte CONNACK frames are rejected" * FINDING_MARKER(F06)) {
        struct Case { const char* label; uint8_t b0; uint8_t b1; };
        const Case malformations[] = {
            {"type CONNECT (0x10)",   0x10, 0x02},
            {"type PUBLISH (0x30)",   0x30, 0x02},
            {"type PINGRESP (0xD0)",  0xD0, 0x02},
            {"nonzero flags (0x21)",  0x21, 0x02},
            {"nonzero flags (0x2F)",  0x2F, 0x02},
            {"remaining length 1",    0x20, 0x01},
            {"remaining length 3",    0x20, 0x03},
            {"remaining length 0",    0x20, 0x00},
        };
        const uint8_t fourthBytes[] = {0x00, 0xFF};

        for (const Case& m : malformations) {
            for (uint8_t fourth : fourthBytes) {
                CAPTURE(m.label);
                CAPTURE(fourth);
                TestClock::instance().reset();
                MockClient client;
                client.pushInbound({m.b0, m.b1, 0x00, fourth});

                PubSubClient psc(client);
                psc.setServer("broker.example", 1883);
                psc.setSocketTimeout(1);

                CHECK_FALSE(psc.connect("f06-client"));
                CHECK_FALSE(psc.connected());
                CHECK(psc.state() != MQTT_CONNECTED);
            }
        }
    }

    // =======================================================================
    //  F-07 - Allocation-failure buffer state (Requirement 13.2)
    // =======================================================================
    //
    // With the allocation interposer armed to fail the next allocation, the
    // realloc inside setBufferSize returns NULL. The library must NOT commit the
    // new size: getBufferSize keeps reporting the previous (valid) size and the
    // existing buffer stays usable. If it committed a nonzero size backed by a
    // null buffer, the subsequent publish would dereference it and ASan would
    // abort. Expected PASS.
    TEST_CASE("F-07 a failed setBufferSize leaves no nonzero capacity backed by a null buffer"
              * FINDING_MARKER(F07)) {
        TestClock::instance().reset();
        AllocInterposer::reset();
        MockClient client;
        PubSubClient psc(client);

        const uint16_t before = psc.getBufferSize();
        REQUIRE(before == MQTT_MAX_PACKET_SIZE);

        AllocInterposer::failNextAllocation();
        const bool grew = psc.setBufferSize(4096);
        AllocInterposer::reset();

        CHECK_FALSE(grew);
        CHECK(psc.getBufferSize() == before);          // size unchanged

        // Prove the buffer behind that size is still valid and usable (clean
        // under ASan - no null/dangling buffer left by the failed realloc).
        connectAndClear(client, psc);
        REQUIRE(psc.publish("tele/dev/STATE", "online"));

        DecodedPublish pub = MqttParser::decodePublish(client.outbound());
        REQUIRE(pub.valid);
        CHECK(pub.topic == "tele/dev/STATE");
        const std::string payload(pub.payload.begin(), pub.payload.end());
        CHECK(payload == "online");
        CHECK(psc.connected());
    }

    // =======================================================================
    //  F-08 - SUBSCRIBE off-by-one exact-buffer write + null-topic checks
    // =======================================================================
    //
    // A SUBSCRIBE whose topic length brings the packet to exactly the working
    // buffer capacity must not write the trailing QoS byte out of bounds. The
    // library's bound is bufferSize >= 10 + topicLength (header(5) + msgId(2) +
    // topic-length(2) + topic + qos(1)); at exact capacity the QoS byte lands at
    // buffer[bufferSize-1]. An off-by-one would write buffer[bufferSize] and ASan
    // would abort. Null topics must be rejected before any strnlen dereference.
    // Expected PASS.
    TEST_CASE("F-08 exact-buffer SUBSCRIBE stays in-bounds and null topics are rejected"
              * FINDING_MARKER(F08)) {
        const size_t topicLengths[] = {1, 5, 16, 50, 117};
        const uint8_t qosValues[] = {0, 1};

        for (size_t topicLen : topicLengths) {
            for (uint8_t qos : qosValues) {
                CAPTURE(topicLen);
                CAPTURE(qos);
                TestClock::instance().reset();
                MockClient client;
                PubSubClient psc(client);
                connectAndClear(client, psc);

                // Shrink the buffer to EXACTLY the SUBSCRIBE bound (10 + topicLen).
                const uint16_t exactCapacity = static_cast<uint16_t>(10 + topicLen);
                REQUIRE(psc.setBufferSize(exactCapacity));
                REQUIRE(psc.getBufferSize() == exactCapacity);

                const std::string topic(topicLen, 'a');
                // ASan proves the QoS byte is written at buffer[bufferSize-1], not
                // buffer[bufferSize].
                REQUIRE(psc.subscribe(topic.c_str(), qos));

                DecodedSubscribe s = MqttParser::decodeSubscribe(client.outbound());
                REQUIRE(s.valid);
                REQUIRE(s.filters.size() == 1);
                CHECK(s.filters[0] == topic);
                REQUIRE(s.requestedQos.size() == 1);
                CHECK(s.requestedQos[0] == qos);
            }
        }

        // Null-topic rejection (no strnlen(nullptr) dereference): subscribe /
        // unsubscribe return false and emit nothing.
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);

        CHECK_FALSE(psc.subscribe(nullptr));
        CHECK_FALSE(psc.subscribe(nullptr, 1));
        CHECK_FALSE(psc.unsubscribe(nullptr));
        CHECK(client.outbound().empty());
        CHECK(psc.connected());
    }

    // =======================================================================
    //  F-09 - Unsupported/malformed QoS in inbound PUBLISH (Req 11.5, 11.6)
    // =======================================================================
    //
    // An inbound PUBLISH whose QoS bits encode 2 (unsupported) or 3 (illegal)
    // must be rejected rather than parsed as QoS 0. Each fixture is otherwise
    // well-framed, so a naive parser would dispatch it; the fork rejects qos > 1
    // up front. Expected PASS.
    TEST_CASE("F-09 inbound PUBLISH with QoS 2 or QoS 3 is rejected" * FINDING_MARKER(F09)) {
        const uint8_t badQos[] = {2, 3};

        for (uint8_t qos : badQos) {
            CAPTURE(qos);
            TestClock::instance().reset();
            MockClient client;
            PubSubClient psc(client);
            CallbackCapture cap;
            connectWithCallback(client, psc, cap);

            const std::string topic = "tele/dev/SENSOR";
            const std::vector<uint8_t> payload = makePayload(16);
            // qos > 0 => the builder emits a packet identifier, so the frame is
            // well-formed apart from the illegal/unsupported QoS.
            client.pushPacket(MqttPacket::publish(topic, payload, qos,
                                                  /*retained=*/false, /*msgId=*/0x0042));

            psc.setSocketTimeout(1);
            psc.loop();

            CHECK(cap.count == 0);                 // not dispatched as QoS 0
            CHECK_FALSE(psc.connected());
            CHECK(client.stopCalled());
        }
    }

    // =======================================================================
    //  F-10 - SUBACK / session-present tracking (Requirement 10.4) - EXPECTED FAIL
    // =======================================================================
    //
    // A subscribe operation should reflect the SUBACK return code rather than
    // reporting success unconditionally. A hardened implementation, on receiving
    // a SUBACK with the failure return code (0x80), would surface the failure.
    // The current fork ignores SUBACK and returns the transport write result, so
    // the hardened assertion reports an EXPECTED FAILURE.
    TEST_CASE("F-10 subscribe reflects the SUBACK return code" * FINDING_MARKER(F10)) {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);

        // SUBACK for the first packet identifier (nextMsgId advances to 1) with
        // the MQTT "failure" return code 0x80.
        client.pushPacket(MqttPacket::suback(1, 0x80));

        CHECK_FALSE(psc.subscribe("tele/dev/SENSOR"));
    }

    // =======================================================================
    //  F-11 - Graceful disconnect default (Requirement 8.4) - EXPECTED FAIL
    // =======================================================================
    //
    // disconnect() with no argument should transmit an MQTT DISCONNECT packet.
    // The current fork defaults to an ungraceful transport close and writes
    // nothing, so the hardened assertion reports an EXPECTED FAILURE.
    TEST_CASE("F-11 disconnect() with no argument transmits a DISCONNECT packet"
              * FINDING_MARKER(F11)) {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);

        psc.disconnect();   // default argument: no DISCONNECT packet in current fork

        DecodedPacket d = MqttParser::decode(client.outbound());
        REQUIRE(d.valid);
        CHECK(d.type == static_cast<uint8_t>(MQTTDISCONNECT));
        CHECK(d.remainingLength == 0);
    }

    // =======================================================================
    //  F-12 - Test-suite quality meta-finding
    // =======================================================================
    //
    // F-12 is not a library bug but a property of the TEST SYSTEM: the original
    // tests contained their own out-of-bounds stack writes and missed the
    // dangerous boundaries. This test system addresses it by construction -
    // adversarial fixtures are built via sized std::vector<uint8_t> / MqttPacket
    // (never a raw over-indexed array), memory-safety fixtures run under ASan/
    // UBSan (on by default), and an integration boundary exercises the real
    // callback contract. This fixture documents and verifies that meta-finding:
    // it builds a battery of adversarial frames the way the whole suite does,
    // confirms each is a properly sized container (no OOB in the test code
    // itself, Requirement 14.3), and drives the worst-case fixture through the
    // library so a regression that read past the received bytes would trip ASan.
    TEST_CASE("F-12 adversarial fixtures are built via sized containers and run clean under sanitizers") {
        // A battery of adversarial frames, each built ONLY through MqttPacket /
        // sized std::vector<uint8_t>. Indexing stays within .size() throughout.
        std::vector<std::vector<uint8_t>> fixtures;

        // Malformed CONNACK (F-06 shape).
        fixtures.push_back({0x30, 0x02, 0x00, 0x00});
        // QoS 1 PUBLISH claiming a topic far larger than the packet (F-02 shape).
        {
            std::vector<uint8_t> f = {static_cast<uint8_t>(MQTTPUBLISH | MQTTQOS1),
                                      0x04, 0x01, 0xF4, 'x', 'y'};
            fixtures.push_back(f);
        }
        // Oversized declared Remaining Length, no body (F-03 shape).
        {
            std::vector<uint8_t> f = {static_cast<uint8_t>(MQTTPUBLISH)};
            const std::vector<uint8_t> rl = MqttPacket::encodeRemainingLength(5000000);
            f.insert(f.end(), rl.begin(), rl.end());
            fixtures.push_back(f);
        }
        // Well-formed QoS 3 PUBLISH (F-09 shape), built structurally.
        fixtures.push_back(MqttPacket::publish("t", std::vector<uint8_t>{'a'},
                                               /*qos=*/3, /*retained=*/false,
                                               /*msgId=*/0x0001).bytes());

        // Meta-assertion: every fixture is a properly sized container and every
        // byte is addressable within bounds (the test code performs no OOB
        // access when constructing or reading its own fixtures).
        for (const std::vector<uint8_t>& f : fixtures) {
            CHECK(f.size() >= 2);
            size_t seen = 0;
            for (size_t i = 0; i < f.size(); ++i) {
                (void)f[i];       // in-bounds by construction (i < f.size())
                ++seen;
            }
            CHECK(seen == f.size());
        }

        // Drive the worst-case over-claiming fixture through the library. If a
        // regression read beyond the received bytes, ASan/UBSan would abort the
        // process; reaching the end of this case cleanly is the verification.
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        CallbackCapture cap;
        connectWithCallback(client, psc, cap);

        client.pushPacket(MqttPacket::raw(fixtures[1]));   // the F-02 over-claim
        psc.setSocketTimeout(1);
        psc.loop();

        // The adversarial packet is rejected without dispatching the callback and
        // without any out-of-bounds access (ASan-clean run).
        CHECK(cap.count == 0);
        CHECK_FALSE(psc.connected());
        CHECK(client.stopCalled());
    }
}
