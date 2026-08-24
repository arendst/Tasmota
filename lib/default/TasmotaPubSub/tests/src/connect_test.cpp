/*
  connect_test.cpp - Connect / CONNACK, constructor forms, and setServer
  characterization + hardening for the TasmotaPubSub host test system (task 10.1).

  Baseline (TEST_SUITE("baseline")):
    - A valid scripted CONNACK drives the *unmodified* library into the connected
      state (Requirement 8.1).
    - The recorded outbound bytes decode into a valid CONNECT packet (Requirement 8.2).
    - Property 2 (outbound packets are structurally well-framed) and Property 3
      (CONNECT field round-trip) are exercised as single, deterministic,
      data-driven cases over curated combinations of client id / user / password /
      Last Will / cleanSession (no randomized generators).
    - Constructor forms and setServer overloads are characterized (Requirements
      18.1, 18.3) and each connect() overload is decoded from the wire.

  Hardening (TEST_SUITE("hardening")):
    - Property 10 / F-06: a four-byte inbound frame that is not a well-formed
      CONNACK (wrong packet type, nonzero fixed flags, or Remaining Length != 2)
      is rejected as a failed connection, independent of the fourth byte value
      (expected PASS per the finding table).
    - F-11 (expected FAIL): disconnect() with no argument should transmit an MQTT
      DISCONNECT packet; the current fork sends nothing, so the case is marked
      FINDING_MARKER(F11) (should_fail).

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

TEST_SUITE("baseline") {

    // --- 8.1: valid CONNACK => connected state ------------------------------

    TEST_CASE("connect with a valid scripted CONNACK enters the connected state") {
        TestClock::instance().reset();
        MockClient client;
        client.pushPacket(MqttPacket::connack(0));   // return code 0 = accepted

        PubSubClient psc(client);
        psc.setServer("broker.example", 1883);

        const bool ok = psc.connect("client-1");
        REQUIRE(ok);
        CHECK(psc.connected());
        CHECK(psc.state() == MQTT_CONNECTED);
    }

    TEST_CASE("connect surfaces a CONNACK non-zero return code as the state and stays disconnected") {
        // Characterize the documented CONNACK return codes 1..5: a nonzero code
        // is reflected in state() and the client is not connected (Requirement 18.8).
        struct Case { uint8_t code; int expectedState; };
        const Case cases[] = {
            {1, MQTT_CONNECT_BAD_PROTOCOL},
            {2, MQTT_CONNECT_BAD_CLIENT_ID},
            {3, MQTT_CONNECT_UNAVAILABLE},
            {4, MQTT_CONNECT_BAD_CREDENTIALS},
            {5, MQTT_CONNECT_UNAUTHORIZED},
        };
        for (const Case& c : cases) {
            CAPTURE(c.code);
            TestClock::instance().reset();
            MockClient client;
            client.pushPacket(MqttPacket::connack(c.code));

            PubSubClient psc(client);
            psc.setServer("broker.example", 1883);

            CHECK_FALSE(psc.connect("client-1"));
            CHECK(psc.state() == c.expectedState);
            CHECK_FALSE(psc.connected());
        }
    }

    // --- 8.2 + Property 2: recorded outbound is a valid CONNECT packet ------

    // Feature: tasmota-pubsub-tests, Property 2: outbound packets are
    // structurally well-framed - the recorded CONNECT decodes into a complete
    // control packet whose declared Remaining Length equals the trailing bytes.
    TEST_CASE("connect records a structurally well-framed CONNECT packet") {
        TestClock::instance().reset();
        MockClient client;
        client.pushPacket(MqttPacket::connack(0));

        PubSubClient psc(client);
        psc.setServer("broker.example", 1883);
        REQUIRE(psc.connect("dev-42"));

        const std::vector<uint8_t>& out = client.outbound();

        // Generic framing: the outbound bytes are one complete, self-consistent
        // control packet whose Remaining Length matches the trailing byte count.
        DecodedPacket generic = MqttParser::decode(out);
        REQUIRE(generic.valid);
        CHECK(generic.type == static_cast<uint8_t>(MQTTCONNECT));
        CHECK(generic.flags == 0x00);
        CHECK(generic.remainingLength == generic.payload.size());

        // CONNECT-specific: protocol name / level and client id round-trip.
        DecodedConnect c = MqttParser::decodeConnect(out);
        REQUIRE(c.valid);
        CHECK(c.protocolName == "MQTT");
        CHECK(c.protocolLevel == MQTT_VERSION_3_1_1);
        CHECK(c.keepAlive == MQTT_KEEPALIVE);
        CHECK(c.clientId == "dev-42");
        CHECK(c.cleanSession);
    }

    // --- Property 3: CONNECT field round-trip -------------------------------

    // Feature: tasmota-pubsub-tests, Property 3: for all valid combinations of
    // client id, optional user, optional password, and optional Last Will with a
    // cleanSession flag, the outbound CONNECT decodes to a packet whose protocol
    // name/level, flags, keepalive, and embedded strings match the arguments.
    // Validated deterministically over a curated table (no randomized generators).
    TEST_CASE("CONNECT field round-trip over curated argument combinations") {
        struct Case {
            const char* clientId;
            const char* user;      // nullptr => no user
            const char* pass;      // nullptr => no password (only set when user set)
            const char* willTopic; // nullptr => no will
            uint8_t     willQos;
            bool        willRetain;
            const char* willMessage;
            bool        cleanSession;
        };
        const Case cases[] = {
            // clientId only, clean session.
            {"c-min", nullptr, nullptr, nullptr, 0, false, nullptr, true},
            // clientId, clean session false (persistent session).
            {"c-persist", nullptr, nullptr, nullptr, 0, false, nullptr, false},
            // user + password.
            {"c-auth", "alice", "s3cret", nullptr, 0, false, nullptr, true},
            // user only (no password).
            {"c-user", "bob", nullptr, nullptr, 0, false, nullptr, true},
            // Last Will, qos 0, not retained.
            {"c-will0", nullptr, nullptr, "tele/dev/LWT", 0, false, "Offline", true},
            // Last Will, qos 1, retained.
            {"c-will1", nullptr, nullptr, "tele/dev/LWT", 1, true, "Offline", true},
            // Last Will, qos 2, retained (qos accepted by connect()).
            {"c-will2", nullptr, nullptr, "tele/dev/LWT", 2, true, "bye", false},
            // Everything at once: user + password + retained will + clean session.
            {"c-full", "alice", "s3cret", "tele/dev/LWT", 1, true, "Offline", true},
            // Empty client id (allowed with clean session) + empty will message.
            {"", nullptr, nullptr, "tele/x/LWT", 0, false, "", true},
        };

        for (const Case& c : cases) {
            CAPTURE(c.clientId);
            TestClock::instance().reset();
            MockClient client;
            client.pushPacket(MqttPacket::connack(0));

            PubSubClient psc(client);
            psc.setServer("broker.example", 1883);

            // Drive every combination through the fully-explicit overload so the
            // cleanSession flag can be varied directly.
            const bool ok = psc.connect(c.clientId, c.user, c.pass,
                                        c.willTopic, c.willQos, c.willRetain,
                                        c.willMessage, c.cleanSession);
            REQUIRE(ok);

            DecodedConnect d = MqttParser::decodeConnect(client.outbound());
            REQUIRE(d.valid);

            // Fixed protocol identity.
            CHECK(d.protocolName == "MQTT");
            CHECK(d.protocolLevel == MQTT_VERSION_3_1_1);
            CHECK(d.keepAlive == MQTT_KEEPALIVE);

            // Client id round-trip.
            CHECK(d.clientId == std::string(c.clientId));

            // cleanSession flag round-trip.
            CHECK(d.cleanSession == c.cleanSession);

            // User / password flags and embedded strings.
            const bool wantUser = (c.user != nullptr);
            const bool wantPass = (c.user != nullptr && c.pass != nullptr);
            CHECK(d.userFlag == wantUser);
            CHECK(d.passwordFlag == wantPass);
            if (wantUser) { CHECK(d.username == std::string(c.user)); }
            if (wantPass) { CHECK(d.password == std::string(c.pass)); }

            // Last Will flags and embedded strings.
            const bool wantWill = (c.willTopic != nullptr);
            CHECK(d.willFlag == wantWill);
            if (wantWill) {
                CHECK(d.willQos == c.willQos);
                CHECK(d.willRetain == c.willRetain);
                CHECK(d.willTopic == std::string(c.willTopic));
                CHECK(d.willMessage == std::string(c.willMessage));
            }
        }
    }

    // --- 18.3: each connect() overload ------------------------------------

    TEST_CASE("connect overloads set the expected CONNECT flags") {
        // Each overload delegates to the 8-arg form with a fixed cleanSession=1
        // (except the explicit form). Decode the wire to confirm the flag layout.

        SUBCASE("connect(id) - clean session, no user/will") {
            TestClock::instance().reset();
            MockClient client;
            client.pushPacket(MqttPacket::connack(0));
            PubSubClient psc(client);
            psc.setServer("broker.example", 1883);
            REQUIRE(psc.connect("id-a"));

            DecodedConnect d = MqttParser::decodeConnect(client.outbound());
            REQUIRE(d.valid);
            CHECK(d.clientId == "id-a");
            CHECK(d.cleanSession);
            CHECK_FALSE(d.userFlag);
            CHECK_FALSE(d.passwordFlag);
            CHECK_FALSE(d.willFlag);
        }

        SUBCASE("connect(id, user, pass)") {
            TestClock::instance().reset();
            MockClient client;
            client.pushPacket(MqttPacket::connack(0));
            PubSubClient psc(client);
            psc.setServer("broker.example", 1883);
            REQUIRE(psc.connect("id-b", "u", "p"));

            DecodedConnect d = MqttParser::decodeConnect(client.outbound());
            REQUIRE(d.valid);
            CHECK(d.userFlag);
            CHECK(d.passwordFlag);
            CHECK(d.username == "u");
            CHECK(d.password == "p");
            CHECK_FALSE(d.willFlag);
        }

        SUBCASE("connect(id, willTopic, willQos, willRetain, willMessage)") {
            TestClock::instance().reset();
            MockClient client;
            client.pushPacket(MqttPacket::connack(0));
            PubSubClient psc(client);
            psc.setServer("broker.example", 1883);
            REQUIRE(psc.connect("id-c", "tele/c/LWT", 1, true, "Offline"));

            DecodedConnect d = MqttParser::decodeConnect(client.outbound());
            REQUIRE(d.valid);
            CHECK(d.willFlag);
            CHECK(d.willQos == 1);
            CHECK(d.willRetain);
            CHECK(d.willTopic == "tele/c/LWT");
            CHECK(d.willMessage == "Offline");
            CHECK_FALSE(d.userFlag);
        }

        SUBCASE("connect(id, user, pass, willTopic, willQos, willRetain, willMessage)") {
            TestClock::instance().reset();
            MockClient client;
            client.pushPacket(MqttPacket::connack(0));
            PubSubClient psc(client);
            psc.setServer("broker.example", 1883);
            REQUIRE(psc.connect("id-d", "u", "p", "tele/d/LWT", 0, false, "bye"));

            DecodedConnect d = MqttParser::decodeConnect(client.outbound());
            REQUIRE(d.valid);
            CHECK(d.userFlag);
            CHECK(d.passwordFlag);
            CHECK(d.willFlag);
            CHECK(d.cleanSession);       // this overload forces cleanSession=1
        }

        SUBCASE("connect(id, ..., cleanSession=false)") {
            TestClock::instance().reset();
            MockClient client;
            client.pushPacket(MqttPacket::connack(0));
            PubSubClient psc(client);
            psc.setServer("broker.example", 1883);
            REQUIRE(psc.connect("id-e", "u", "p", "tele/e/LWT", 2, false, "bye", false));

            DecodedConnect d = MqttParser::decodeConnect(client.outbound());
            REQUIRE(d.valid);
            CHECK_FALSE(d.cleanSession);
            CHECK(d.willQos == 2);
        }
    }

    // --- 18.1: constructor forms -------------------------------------------

    TEST_CASE("constructor forms have a stable initial disconnected state") {
        TestClock::instance().reset();
        MockClient c1, c2, c3, c4, c5;
        IPAddress ip(192, 168, 1, 10);

        PubSubClient def;                              // default
        PubSubClient fromClient(c1);                   // client only
        PubSubClient fromIp(ip, 1883, c2);             // ip + port + client
        PubSubClient fromDomain("broker.example", 1883, c3);  // domain + port + client
        auto cb = [](char*, uint8_t*, unsigned int) {};
        PubSubClient fromIpCb(ip, 1883, cb, c4);       // ip + port + callback + client
        PubSubClient fromDomainCb("broker.example", 1883, cb, c5);  // domain + callback

        for (PubSubClient* p : {&def, &fromClient, &fromIp, &fromDomain, &fromIpCb, &fromDomainCb}) {
            CHECK(p->state() == MQTT_DISCONNECTED);
            CHECK_FALSE(p->connected());
            CHECK(p->getBufferSize() == MQTT_MAX_PACKET_SIZE);
        }
    }

    TEST_CASE("constructor-supplied endpoint is used by connect (ip and domain forms)") {
        // IP-form constructor: connect routes to the constructor IP/port.
        {
            TestClock::instance().reset();
            MockClient client;
            client.pushPacket(MqttPacket::connack(0));
            IPAddress ip(10, 0, 0, 5);
            PubSubClient psc(ip, 1883, client);
            REQUIRE(psc.connect("dev"));
            CHECK(psc.connected());
            CHECK(client.connectCalled());
            CHECK(client.lastIp() == ip);
            CHECK(client.lastPort() == 1883);
        }
        // Domain-form constructor: connect routes to the constructor host/port.
        {
            TestClock::instance().reset();
            MockClient client;
            client.pushPacket(MqttPacket::connack(0));
            PubSubClient psc("mqtt.local", 8883, client);
            REQUIRE(psc.connect("dev"));
            CHECK(client.connectCalled());
            CHECK(client.lastHost() == "mqtt.local");
            CHECK(client.lastPort() == 8883);
        }
    }

    // --- setServer overloads -----------------------------------------------

    TEST_CASE("setServer overloads route connect to the configured endpoint") {
        SUBCASE("setServer(IPAddress, port)") {
            TestClock::instance().reset();
            MockClient client;
            client.pushPacket(MqttPacket::connack(0));
            PubSubClient psc(client);
            IPAddress ip(172, 16, 0, 9);
            psc.setServer(ip, 1883);
            REQUIRE(psc.connect("dev"));
            CHECK(client.lastIp() == ip);
            CHECK(client.lastPort() == 1883);
        }

        SUBCASE("setServer(uint8_t*, port)") {
            TestClock::instance().reset();
            MockClient client;
            client.pushPacket(MqttPacket::connack(0));
            PubSubClient psc(client);
            uint8_t ipbytes[4] = {192, 0, 2, 44};
            psc.setServer(ipbytes, 1883);
            REQUIRE(psc.connect("dev"));
            CHECK(client.lastIp() == IPAddress(192, 0, 2, 44));
            CHECK(client.lastPort() == 1883);
        }

        SUBCASE("setServer(const char* domain, port)") {
            TestClock::instance().reset();
            MockClient client;
            client.pushPacket(MqttPacket::connack(0));
            PubSubClient psc(client);
            psc.setServer("broker.example", 1884);
            REQUIRE(psc.connect("dev"));
            CHECK(client.lastHost() == "broker.example");
            CHECK(client.lastPort() == 1884);
        }
    }
}

TEST_SUITE("hardening") {

    // Feature: tasmota-pubsub-tests, Property 10: for all four-byte inbound
    // frames that are not a well-formed CONNACK (packet type other than 0x20,
    // nonzero fixed flags, or Remaining Length other than 2), connect() reports
    // failure and does not enter the connected state, regardless of the fourth
    // byte value. F-06 is hardened in the current fork (expected PASS).
    TEST_CASE("F-06 malformed four-byte CONNACK frames are rejected" * FINDING_MARKER(F06)) {
        struct Case { const char* label; uint8_t b0; uint8_t b1; };
        const Case malformations[] = {
            // Wrong packet type (Remaining Length 2, but not CONNACK 0x20).
            {"type CONNECT (0x10)",   0x10, 0x02},
            {"type PUBLISH (0x30)",   0x30, 0x02},
            {"type PINGRESP (0xD0)",  0xD0, 0x02},
            // Correct type nibble (0x2) but nonzero fixed flags in the low nibble.
            {"nonzero flags (0x21)",  0x21, 0x02},
            {"nonzero flags (0x2F)",  0x2F, 0x02},
            // Correct type + zero flags, but Remaining Length != 2.
            {"remaining length 1",    0x20, 0x01},
            {"remaining length 3",    0x20, 0x03},
            {"remaining length 0",    0x20, 0x00},
        };
        // The fourth byte (CONNACK return-code position) must not influence the
        // decision for a malformed frame - exercise both 0x00 and 0xFF.
        const uint8_t fourthBytes[] = {0x00, 0xFF};

        for (const Case& m : malformations) {
            for (uint8_t fourth : fourthBytes) {
                CAPTURE(m.label);
                CAPTURE(fourth);
                TestClock::instance().reset();
                MockClient client;
                // Bound any over-declared-length read to a short virtual timeout.
                client.pushInbound({m.b0, m.b1, 0x00, fourth});

                PubSubClient psc(client);
                psc.setServer("broker.example", 1883);
                psc.setSocketTimeout(1);

                CHECK_FALSE(psc.connect("client-x"));
                CHECK_FALSE(psc.connected());
                CHECK(psc.state() != MQTT_CONNECTED);
            }
        }
    }

    // F-11 (Requirement 8.4): disconnect() with no argument SHOULD transmit an
    // MQTT DISCONNECT packet. The current fork only flushes/stops the transport
    // and writes nothing, so this is expected to fail until F-11 is hardened.
    TEST_CASE("F-11 disconnect() with no argument transmits a DISCONNECT packet" * FINDING_MARKER(F11)) {
        TestClock::instance().reset();
        MockClient client;
        client.pushPacket(MqttPacket::connack(0));
        PubSubClient psc(client);
        psc.setServer("broker.example", 1883);
        REQUIRE(psc.connect("dev"));

        client.clearOutbound();
        psc.disconnect();   // default argument: no DISCONNECT packet in current fork

        // A hardened implementation writes the 2-byte DISCONNECT frame (0xE0 0x00).
        DecodedPacket d = MqttParser::decode(client.outbound());
        REQUIRE(d.valid);
        CHECK(d.type == static_cast<uint8_t>(MQTTDISCONNECT));
        CHECK(d.remainingLength == 0);
    }
}
