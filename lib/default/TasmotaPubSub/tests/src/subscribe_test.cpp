/*
  subscribe_test.cpp - subscribe / unsubscribe characterization + hardening for
  the TasmotaPubSub host test system (task 10.4).

  Baseline (TEST_SUITE("baseline")):
    - subscribe() records a structurally valid SUBSCRIBE packet (Requirement 10.1).
    - unsubscribe() records a structurally valid UNSUBSCRIBE packet (Req 10.2).
    - Property 5 (SUBSCRIBE/UNSUBSCRIBE field round-trip: nonzero message id, the
      trailing requested-QoS byte for SUBSCRIBE, and the topic filter equal to the
      input) is folded in as a single deterministic, data-driven case (SUBCASE /
      table loops, no randomized generators) over curated QoS {0,1} and topic
      vectors. The explicit-QoS subscribe overload is covered here (Req 18.6).

  Hardening (TEST_SUITE("hardening")):
    - F-08 (Requirement 10.3, expected PASS): a SUBSCRIBE whose topic length brings
      the packet to exactly the working-buffer capacity does not write the trailing
      QoS byte out of bounds. The buffer is sized to exactly 10 + topicLength (the
      library's own bound: header(5) + msgId(2) + topic-length(2) + topic + qos(1)),
      so the QoS byte lands at buffer[bufferSize - 1]; an off-by-one would write at
      buffer[bufferSize] and trip AddressSanitizer. Marked FINDING_MARKER(F08).
    - Null-topic rejection (Requirement 10.5, expected PASS): subscribe() and
      unsubscribe() reject a null topic pointer before any length computation
      dereferences it (no strnlen on nullptr, no packet emitted). Plain hardening
      case (expected to pass against the current fork).
    - F-10 (Requirement 10.4, expected FAIL): a subscribe operation should reflect
      the SUBACK return code rather than reporting success unconditionally. The
      current fork ignores SUBACK entirely and returns the transport write result,
      so the case is marked FINDING_MARKER(F10) (should_fail).

  Every assertion goes through the public API and decoded MockClient.outbound()
  wire bytes - never private members - so the baseline stays durable across a
  future MQTT 5 migration (Requirement 19.1).
*/

#include <cstdint>
#include <string>
#include <vector>

#include "doctest.h"

#include "FindingStatus.h"
#include "MockClient.h"
#include "MqttPacket.h"
#include "TestClock.h"
#include "PubSubClient.h"

namespace {

// Connect the client/psc pair with a scripted CONNACK and clear the recorded
// CONNECT bytes so only the packet-under-test remains in outbound().
void connectAndClear(MockClient& client, PubSubClient& psc) {
    client.pushPacket(MqttPacket::connack(0));
    psc.setServer("broker.example", 1883);
    REQUIRE(psc.connect("sub-client"));
    REQUIRE(psc.connected());
    client.clearOutbound();
}

}  // namespace

TEST_SUITE("baseline") {

    // --- 10.1: subscribe() records a valid SUBSCRIBE packet -----------------

    TEST_CASE("subscribe records a structurally well-framed SUBSCRIBE packet") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);

        REQUIRE(psc.subscribe("tele/dev/SENSOR"));

        const std::vector<uint8_t>& out = client.outbound();

        // Generic framing: one complete control packet whose declared Remaining
        // Length equals the trailing byte count, SUBSCRIBE type, QoS1 fixed flags
        // (the library always sends MQTTSUBSCRIBE | MQTTQOS1).
        DecodedPacket generic = MqttParser::decode(out);
        REQUIRE(generic.valid);
        CHECK(generic.type == static_cast<uint8_t>(MQTTSUBSCRIBE));
        CHECK(generic.flags == static_cast<uint8_t>(MQTTQOS1));
        CHECK(generic.remainingLength == generic.payload.size());

        // SUBSCRIBE-specific fields.
        DecodedSubscribe s = MqttParser::decodeSubscribe(out);
        REQUIRE(s.valid);
        CHECK(s.msgId != 0);                       // packet identifier is nonzero
        REQUIRE(s.filters.size() == 1);
        CHECK(s.filters[0] == "tele/dev/SENSOR");
        REQUIRE(s.requestedQos.size() == 1);
        CHECK(s.requestedQos[0] == 0);             // subscribe(topic) defaults QoS 0
    }

    // --- 10.2: unsubscribe() records a valid UNSUBSCRIBE packet -------------

    TEST_CASE("unsubscribe records a structurally well-framed UNSUBSCRIBE packet") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);

        REQUIRE(psc.unsubscribe("cmnd/dev/POWER"));

        const std::vector<uint8_t>& out = client.outbound();

        DecodedPacket generic = MqttParser::decode(out);
        REQUIRE(generic.valid);
        CHECK(generic.type == static_cast<uint8_t>(MQTTUNSUBSCRIBE));
        CHECK(generic.flags == static_cast<uint8_t>(MQTTQOS1));
        CHECK(generic.remainingLength == generic.payload.size());

        DecodedUnsubscribe u = MqttParser::decodeUnsubscribe(out);
        REQUIRE(u.valid);
        CHECK(u.msgId != 0);                       // packet identifier is nonzero
        REQUIRE(u.filters.size() == 1);
        CHECK(u.filters[0] == "cmnd/dev/POWER");
    }

    // --- Property 5: SUBSCRIBE/UNSUBSCRIBE field round-trip -----------------

    // Feature: tasmota-pubsub-tests, Property 5: for all topics and requested QoS
    // in {0,1}, subscribe emits a SUBSCRIBE with a nonzero message id, the topic
    // filter equal to the input, and a trailing requested-QoS byte equal to the
    // argument; unsubscribe emits an UNSUBSCRIBE with a nonzero message id and the
    // topic filter equal to the input. Validated deterministically over a curated
    // table of QoS values and topic vectors (no randomized generators). The
    // explicit-QoS subscribe overload (Requirement 18.6) is exercised for QoS 1.
    TEST_CASE("Property 5: SUBSCRIBE/UNSUBSCRIBE field round-trip over curated QoS and topics") {
        // Curated topic filters: single level, multi-level, wildcards, a
        // single-character topic, and a longer nested filter.
        const std::string topics[] = {
            "a",
            "tele/dev/SENSOR",
            "cmnd/tasmota_ABCDEF/+",
            "stat/#",
            "tele/very/deeply/nested/topic/filter/name",
        };
        const uint8_t qosValues[] = {0, 1};

        for (const std::string& topic : topics) {
            // --- SUBSCRIBE for each requested QoS in {0,1} ------------------
            for (uint8_t qos : qosValues) {
                CAPTURE(topic);
                CAPTURE(qos);
                TestClock::instance().reset();
                MockClient client;
                PubSubClient psc(client);
                connectAndClear(client, psc);

                // Exercise the explicit-QoS overload (Requirement 18.6).
                REQUIRE(psc.subscribe(topic.c_str(), qos));

                const std::vector<uint8_t>& out = client.outbound();
                DecodedPacket generic = MqttParser::decode(out);
                REQUIRE(generic.valid);
                CHECK(generic.type == static_cast<uint8_t>(MQTTSUBSCRIBE));
                CHECK(generic.flags == static_cast<uint8_t>(MQTTQOS1));
                CHECK(generic.remainingLength == generic.payload.size());

                DecodedSubscribe s = MqttParser::decodeSubscribe(out);
                REQUIRE(s.valid);
                CHECK(s.msgId != 0);
                REQUIRE(s.filters.size() == 1);
                CHECK(s.filters[0] == topic);
                REQUIRE(s.requestedQos.size() == 1);
                CHECK(s.requestedQos[0] == qos);
            }

            // --- UNSUBSCRIBE (no requested-QoS byte on the wire) ------------
            {
                CAPTURE(topic);
                TestClock::instance().reset();
                MockClient client;
                PubSubClient psc(client);
                connectAndClear(client, psc);

                REQUIRE(psc.unsubscribe(topic.c_str()));

                const std::vector<uint8_t>& out = client.outbound();
                DecodedPacket generic = MqttParser::decode(out);
                REQUIRE(generic.valid);
                CHECK(generic.type == static_cast<uint8_t>(MQTTUNSUBSCRIBE));
                CHECK(generic.flags == static_cast<uint8_t>(MQTTQOS1));
                CHECK(generic.remainingLength == generic.payload.size());

                DecodedUnsubscribe u = MqttParser::decodeUnsubscribe(out);
                REQUIRE(u.valid);
                CHECK(u.msgId != 0);
                REQUIRE(u.filters.size() == 1);
                CHECK(u.filters[0] == topic);
            }
        }
    }

    // Successive subscribe/unsubscribe calls advance the packet identifier, so
    // each emitted packet carries its own nonzero message id (characterization of
    // the observable nextMsgId behavior across the public API).
    TEST_CASE("successive subscribe/unsubscribe calls carry distinct nonzero message ids") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);

        REQUIRE(psc.subscribe("tele/a/#"));
        DecodedSubscribe s1 = MqttParser::decodeSubscribe(client.outbound());
        REQUIRE(s1.valid);
        client.clearOutbound();

        REQUIRE(psc.subscribe("tele/b/#"));
        DecodedSubscribe s2 = MqttParser::decodeSubscribe(client.outbound());
        REQUIRE(s2.valid);
        client.clearOutbound();

        REQUIRE(psc.unsubscribe("tele/a/#"));
        DecodedUnsubscribe u1 = MqttParser::decodeUnsubscribe(client.outbound());
        REQUIRE(u1.valid);

        CHECK(s1.msgId != 0);
        CHECK(s2.msgId != 0);
        CHECK(u1.msgId != 0);
        CHECK(s2.msgId != s1.msgId);   // identifier advances between calls
        CHECK(u1.msgId != s2.msgId);
    }
}

TEST_SUITE("hardening") {

    // F-08 (Requirement 10.3): a SUBSCRIBE whose topic length brings the packet to
    // exactly the buffer capacity must not write the trailing QoS byte beyond the
    // buffer bounds. The library's own bound is bufferSize >= 10 + topicLength
    // (header(5) + msgId(2) + topic-length(2) + topic + qos(1)); at exact capacity
    // the QoS byte is written at buffer[9 + topicLength] == buffer[bufferSize - 1].
    // An off-by-one would write at buffer[bufferSize] and AddressSanitizer would
    // abort. The buffer is set AFTER connecting (the CONNECT packet needs the full
    // default buffer) so the exact-capacity boundary applies only to the SUBSCRIBE.
    // F-08 is hardened in the current fork, so this is expected PASS.
    TEST_CASE("F-08 exact-buffer SUBSCRIBE does not write the QoS byte out of bounds"
              * FINDING_MARKER(F08)) {
        // Topic lengths kept small so the Remaining Length stays a single byte;
        // the point is the exact-capacity boundary, not large packets.
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

                // Shrink the working buffer to EXACTLY the SUBSCRIBE bound. The
                // realloc succeeds; the connection state is unaffected.
                const uint16_t exactCapacity =
                    static_cast<uint16_t>(10 + topicLen);
                REQUIRE(psc.setBufferSize(exactCapacity));
                REQUIRE(psc.getBufferSize() == exactCapacity);

                const std::string topic(topicLen, 'a');

                // The QoS byte is written at buffer[bufferSize - 1]; ASan proves
                // no write at buffer[bufferSize].
                REQUIRE(psc.subscribe(topic.c_str(), qos));

                // The emitted packet is still a well-framed SUBSCRIBE carrying the
                // exact topic and requested QoS.
                const std::vector<uint8_t>& out = client.outbound();
                DecodedSubscribe s = MqttParser::decodeSubscribe(out);
                REQUIRE(s.valid);
                CHECK(s.msgId != 0);
                REQUIRE(s.filters.size() == 1);
                CHECK(s.filters[0] == topic);
                REQUIRE(s.requestedQos.size() == 1);
                CHECK(s.requestedQos[0] == qos);
            }
        }
    }

    // Requirement 10.5: subscribe() and unsubscribe() must reject a null topic
    // pointer before any length computation dereferences it. A hardened
    // implementation returns false and emits nothing (no strnlen(nullptr, ...)).
    // The current fork already guards this, so it is expected PASS. Run under
    // ASan, a missing guard would fault while computing the topic length.
    TEST_CASE("subscribe / unsubscribe reject a null topic pointer") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);

        CHECK_FALSE(psc.subscribe(nullptr));
        CHECK_FALSE(psc.subscribe(nullptr, 0));
        CHECK_FALSE(psc.subscribe(nullptr, 1));
        CHECK_FALSE(psc.unsubscribe(nullptr));

        // No packet was written for any rejected call.
        CHECK(client.outbound().empty());

        // The connection remains usable after the rejections.
        CHECK(psc.connected());
    }

    // F-10 (Requirement 10.4): a subscribe operation should reflect the SUBACK
    // return code rather than reporting success unconditionally. A hardened
    // implementation, on receiving a SUBACK carrying a failure return code
    // (0x80), would surface the failure to the caller. The current fork ignores
    // SUBACK entirely and returns the transport write result (true), so this case
    // is expected to fail until F-10 is hardened.
    TEST_CASE("F-10 subscribe reflects the SUBACK return code" * FINDING_MARKER(F10)) {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        connectAndClear(client, psc);

        // Script a SUBACK with the MQTT "failure" return code (0x80) for the
        // packet identifier the first subscribe() will use (nextMsgId advances to
        // 1 on the first call after connect).
        client.pushPacket(MqttPacket::suback(1, 0x80));

        // Hardened contract: subscribe reflects the SUBACK failure and reports
        // failure. The current fork returns true unconditionally.
        CHECK_FALSE(psc.subscribe("tele/dev/SENSOR"));
    }
}
