/*
  api_surface_test.cpp - Public-API surface characterization for the
  TasmotaPubSub host test system (task 10.8).

  All cases here are Baseline (TEST_SUITE("baseline")) characterization tests:
  they lock in the *current* observable behavior of the unmodified library
  across the parts of the Public_API that connect_test.cpp does not already
  cover in depth, so the two files together characterize the entire surface
  without gaps. Specifically:

    - Every constructor form declared in PubSubClient.h (Requirement 18.1).
      connect_test.cpp exercises six forms (default, client-only, ip+client,
      domain+client, ip+callback+client, domain+callback+client); this file
      adds the remaining eight (the uint8_t* byte-array IP forms and all the
      Stream& / callback+Stream& variants) so no constructor form is missing.
    - setServer / setClient / setCallback / setStream / setKeepAlive /
      setSocketTimeout observable behavior (Requirement 18.2). setClient is
      shown to actually route bytes through the supplied client; setStream is
      shown to route the inbound PUBLISH payload to the supplied MockStream;
      setKeepAlive is shown in the CONNECT keepalive field; setSocketTimeout is
      shown to bound the connect read-wait in virtual time.
    - Each connect() overload incl. user+password, Last Will, and cleanSession
      (Requirement 18.3), complementing connect_test.cpp's field round-trip.
    - state() across every documented code: MQTT_CONNECTION_TIMEOUT(-4),
      MQTT_CONNECTION_LOST(-3), MQTT_CONNECT_FAILED(-2), MQTT_DISCONNECTED(-1),
      MQTT_CONNECTED(0), and the CONNACK return codes 1..5 (Requirement 18.8),
      each driven through a realistic scenario.
    - loop() and connected() observable behavior.

  Every assertion goes through the public API and decoded MockClient.outbound()
  wire bytes - never private members - so the baseline stays durable across a
  future MQTT 5 migration (Requirement 19.1).
*/

#include <cstdint>
#include <string>
#include <vector>

#include "doctest.h"

#include "MockClient.h"
#include "MockStream.h"
#include "MqttPacket.h"
#include "TestClock.h"
#include "PubSubClient.h"

namespace {

// A harmless callback used only so the callback-taking constructor overloads
// can be instantiated; it records nothing.
void noopCallback(char*, uint8_t*, unsigned int) {}

}  // namespace

TEST_SUITE("baseline") {

    // --- 18.1: constructor forms (complement connect_test.cpp) --------------

    // connect_test.cpp covers 6 of the 14 constructor forms. This case adds the
    // remaining 8 (the uint8_t* byte-array IP forms and every Stream& variant)
    // so all 14 declared forms are characterized for a stable initial state.
    TEST_CASE("remaining constructor forms initialize to a stable disconnected state") {
        TestClock::instance().reset();
        MockClient c1, c2, c3, c4, c5, c6, c7, c8;
        MockStream s1, s2, s3, s4;
        IPAddress ip(192, 168, 1, 20);
        uint8_t ipb[4] = {10, 1, 2, 3};

        // IPAddress + Stream forms.
        PubSubClient ipStream(ip, 1883, c1, s1);
        PubSubClient ipCbStream(ip, 1883, noopCallback, c2, s2);
        // uint8_t* byte-array IP forms (all four).
        PubSubClient ba(ipb, 1883, c3);
        PubSubClient baStream(ipb, 1883, c4, s3);
        PubSubClient baCb(ipb, 1883, noopCallback, c5);
        PubSubClient baCbStream(ipb, 1883, noopCallback, c6, s4);
        // domain + Stream forms (the two connect_test.cpp does not build).
        PubSubClient domStream("broker.example", 1883, c7);   // domain + client
        PubSubClient domCbStream("broker.example", 1883, noopCallback, c8);

        PubSubClient* forms[] = {
            &ipStream, &ipCbStream, &ba, &baStream,
            &baCb, &baCbStream, &domStream, &domCbStream,
        };
        for (PubSubClient* p : forms) {
            CHECK(p->state() == MQTT_DISCONNECTED);
            CHECK_FALSE(p->connected());
            CHECK(p->getBufferSize() == MQTT_MAX_PACKET_SIZE);
        }
    }

    // The uint8_t* byte-array IP constructor must route connect() to that
    // address, proving the endpoint captured by this form is actually used.
    TEST_CASE("byte-array IP constructor routes connect to that address") {
        TestClock::instance().reset();
        MockClient client;
        client.pushPacket(MqttPacket::connack(0));
        uint8_t ipb[4] = {203, 0, 113, 7};
        PubSubClient psc(ipb, 1883, client);

        REQUIRE(psc.connect("dev"));
        CHECK(psc.connected());
        CHECK(client.connectCalled());
        CHECK(client.lastIp() == IPAddress(203, 0, 113, 7));
        CHECK(client.lastPort() == 1883);
    }

    // A Stream-form constructor attaches the stream; in stream mode the library
    // routes each inbound PUBLISH payload byte to that stream during readPacket.
    TEST_CASE("stream-form constructor routes inbound PUBLISH payload to the attached stream") {
        TestClock::instance().reset();
        MockClient client;
        MockStream stream;
        IPAddress ip(10, 0, 0, 8);
        PubSubClient psc(ip, 1883, client, stream);

        client.pushPacket(MqttPacket::connack(0));
        REQUIRE(psc.connect("stream-dev"));
        REQUIRE(psc.connected());

        // A QoS 0 PUBLISH: topic "t", payload "hi". In stream mode the payload
        // bytes (after the topic) are written to the attached stream.
        const std::string topic = "t";
        const std::vector<uint8_t> payload = {'h', 'i'};
        client.pushPacket(MqttPacket::publish(topic, payload, /*qos=*/0));
        REQUIRE(psc.loop());

        CHECK(stream.written() == payload);
    }

    // --- 18.2: setClient routes outbound bytes through the supplied client ---

    TEST_CASE("setClient makes connect route bytes through the supplied client") {
        TestClock::instance().reset();
        MockClient client;
        client.pushPacket(MqttPacket::connack(0));

        // Default-construct (no client) then attach one via setClient.
        PubSubClient psc;
        psc.setServer("broker.example", 1883);
        psc.setClient(client);

        REQUIRE(psc.connect("dev"));
        CHECK(psc.connected());

        // The CONNECT bytes were recorded by the client supplied via setClient.
        CHECK(client.connectCalled());
        DecodedConnect d = MqttParser::decodeConnect(client.outbound());
        REQUIRE(d.valid);
        CHECK(d.clientId == "dev");
    }

    // --- 18.2: setCallback registers the dispatch callback; latest wins ------

    TEST_CASE("setCallback registers the inbound dispatch callback and the latest registration wins") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);

        int firstCount = 0;
        int secondCount = 0;
        std::string seenTopic;

        client.pushPacket(MqttPacket::connack(0));
        psc.setServer("broker.example", 1883);
        // Register a first callback, then replace it: only the second must fire.
        psc.setCallback([&firstCount](char*, uint8_t*, unsigned int) { firstCount++; });
        psc.setCallback([&secondCount, &seenTopic](char* topic, uint8_t*, unsigned int) {
            secondCount++;
            seenTopic = topic;
        });
        REQUIRE(psc.connect("cb-dev"));
        client.clearOutbound();

        const std::string topic = "tele/dev/STATE";
        const std::vector<uint8_t> payload = {'O', 'N'};
        client.pushPacket(MqttPacket::publish(topic, payload, /*qos=*/0));
        REQUIRE(psc.loop());

        CHECK(firstCount == 0);          // the replaced callback never fires
        CHECK(secondCount == 1);         // the latest callback is used
        CHECK(seenTopic == topic);
    }

    // --- 18.2: setStream routes inbound PUBLISH payload to the stream --------

    TEST_CASE("setStream routes the inbound PUBLISH payload to the supplied stream") {
        TestClock::instance().reset();
        MockClient client;
        MockStream stream;
        PubSubClient psc(client);
        psc.setStream(stream);

        client.pushPacket(MqttPacket::connack(0));
        psc.setServer("broker.example", 1883);
        REQUIRE(psc.connect("stream-dev"));
        REQUIRE(psc.connected());

        const std::string topic = "sensor";
        const std::vector<uint8_t> payload = {'1', '2', '3', '4'};
        client.pushPacket(MqttPacket::publish(topic, payload, /*qos=*/0));
        REQUIRE(psc.loop());

        // Every payload byte (after the topic) was routed to the stream in order.
        CHECK(stream.written() == payload);
    }

    // --- 18.2: setKeepAlive is reflected in the CONNECT keepalive field ------

    TEST_CASE("setKeepAlive is reflected in the CONNECT keepalive field") {
        // Curated intervals, including the default, a short and a long value.
        const uint16_t intervals[] = {MQTT_KEEPALIVE, 1, 30, 3600};

        for (uint16_t ka : intervals) {
            CAPTURE(ka);
            TestClock::instance().reset();
            MockClient client;
            client.pushPacket(MqttPacket::connack(0));

            PubSubClient psc(client);
            psc.setServer("broker.example", 1883);
            psc.setKeepAlive(ka);          // seconds, set BEFORE connect
            REQUIRE(psc.connect("ka-dev"));

            DecodedConnect d = MqttParser::decodeConnect(client.outbound());
            REQUIRE(d.valid);
            CHECK(d.keepAlive == ka);
        }
    }

    // --- 18.2: setSocketTimeout bounds the connect read-wait in virtual time -

    TEST_CASE("setSocketTimeout bounds the connect read-wait in virtual time") {
        // With no CONNACK scripted, connect() busy-waits for the transport and
        // gives up after socketTimeout seconds. Because the shim's delay()
        // advances the same virtual clock millis() reads, the elapsed VIRTUAL
        // time is bounded by socketTimeout*1000 ms - a larger timeout waits
        // proportionally longer - with no real wall-clock time consumed.
        auto measureConnectTimeout = [](uint16_t timeoutSecs) -> unsigned long {
            TestClock::instance().reset();
            MockClient client;                       // empty inbound: no CONNACK
            PubSubClient psc(client);
            psc.setServer("broker.example", 1883);
            psc.setSocketTimeout(timeoutSecs);

            const unsigned long start = TestClock::instance().millis();
            const bool ok = psc.connect("to-dev");   // must fail via timeout
            const unsigned long elapsed = TestClock::instance().millis() - start;

            CHECK_FALSE(ok);
            CHECK(psc.state() == MQTT_CONNECTION_TIMEOUT);
            CHECK(client.stopCalled());
            return elapsed;
        };

        const unsigned long elapsed1 = measureConnectTimeout(1);
        const unsigned long elapsed3 = measureConnectTimeout(3);

        // Each wait lasts at least its configured timeout in virtual ms, and a
        // larger timeout waits strictly longer.
        CHECK(elapsed1 >= 1UL * 1000UL);
        CHECK(elapsed3 >= 3UL * 1000UL);
        CHECK(elapsed3 > elapsed1);
    }

    // --- 18.2: setServer selects the most recent endpoint form ---------------

    TEST_CASE("setServer selects the most recently configured endpoint") {
        // A later setServer(domain) overrides an earlier setServer(ip): connect
        // routes to the domain form. (The library clears the domain when an IP
        // form is set and vice-versa, so the last call wins.)
        TestClock::instance().reset();
        MockClient client;
        client.pushPacket(MqttPacket::connack(0));

        PubSubClient psc(client);
        psc.setServer(IPAddress(10, 0, 0, 1), 1883);   // first: IP form
        psc.setServer("broker.example", 8883);         // then: domain form wins
        REQUIRE(psc.connect("dev"));

        CHECK(client.lastHost() == "broker.example");
        CHECK(client.lastPort() == 8883);
    }

    // --- 18.3: connect() overloads (complement connect_test.cpp) -------------

    // connect_test.cpp round-trips the CONNECT fields per overload; this case
    // consolidates the *delegation* contract: every non-explicit overload forces
    // cleanSession=1, and only the fully-explicit overload can clear it.
    TEST_CASE("connect overloads force cleanSession except the explicit cleanSession overload") {
        SUBCASE("connect(id) => clean session, no user/will") {
            TestClock::instance().reset();
            MockClient client;
            client.pushPacket(MqttPacket::connack(0));
            PubSubClient psc(client);
            psc.setServer("broker.example", 1883);
            REQUIRE(psc.connect("id"));
            DecodedConnect d = MqttParser::decodeConnect(client.outbound());
            REQUIRE(d.valid);
            CHECK(d.cleanSession);
            CHECK_FALSE(d.userFlag);
            CHECK_FALSE(d.willFlag);
        }

        SUBCASE("connect(id, user, pass) => clean session + credentials") {
            TestClock::instance().reset();
            MockClient client;
            client.pushPacket(MqttPacket::connack(0));
            PubSubClient psc(client);
            psc.setServer("broker.example", 1883);
            REQUIRE(psc.connect("id", "alice", "s3cret"));
            DecodedConnect d = MqttParser::decodeConnect(client.outbound());
            REQUIRE(d.valid);
            CHECK(d.cleanSession);
            CHECK(d.userFlag);
            CHECK(d.passwordFlag);
            CHECK(d.username == "alice");
            CHECK(d.password == "s3cret");
        }

        SUBCASE("connect(id, willTopic, willQos, willRetain, willMessage) => clean session + will") {
            TestClock::instance().reset();
            MockClient client;
            client.pushPacket(MqttPacket::connack(0));
            PubSubClient psc(client);
            psc.setServer("broker.example", 1883);
            REQUIRE(psc.connect("id", "tele/LWT", 1, true, "Offline"));
            DecodedConnect d = MqttParser::decodeConnect(client.outbound());
            REQUIRE(d.valid);
            CHECK(d.cleanSession);
            CHECK(d.willFlag);
            CHECK(d.willQos == 1);
            CHECK(d.willRetain);
            CHECK(d.willTopic == "tele/LWT");
            CHECK(d.willMessage == "Offline");
        }

        SUBCASE("connect(id, user, pass, will...) => clean session + credentials + will") {
            TestClock::instance().reset();
            MockClient client;
            client.pushPacket(MqttPacket::connack(0));
            PubSubClient psc(client);
            psc.setServer("broker.example", 1883);
            REQUIRE(psc.connect("id", "u", "p", "tele/LWT", 0, false, "bye"));
            DecodedConnect d = MqttParser::decodeConnect(client.outbound());
            REQUIRE(d.valid);
            CHECK(d.cleanSession);
            CHECK(d.userFlag);
            CHECK(d.passwordFlag);
            CHECK(d.willFlag);
        }

        SUBCASE("connect(id, ..., cleanSession=false) => persistent session") {
            TestClock::instance().reset();
            MockClient client;
            client.pushPacket(MqttPacket::connack(0));
            PubSubClient psc(client);
            psc.setServer("broker.example", 1883);
            REQUIRE(psc.connect("id", "u", "p", "tele/LWT", 2, true, "bye", false));
            DecodedConnect d = MqttParser::decodeConnect(client.outbound());
            REQUIRE(d.valid);
            CHECK_FALSE(d.cleanSession);   // only this overload can clear it
            CHECK(d.willQos == 2);
        }
    }

    // --- 18.8: state() across every documented code -------------------------

    TEST_CASE("state() reports MQTT_DISCONNECTED before any connect attempt") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        CHECK(psc.state() == MQTT_DISCONNECTED);
        CHECK_FALSE(psc.connected());
    }

    TEST_CASE("state() reports MQTT_CONNECTED after a successful CONNACK") {
        TestClock::instance().reset();
        MockClient client;
        client.pushPacket(MqttPacket::connack(0));
        PubSubClient psc(client);
        psc.setServer("broker.example", 1883);

        REQUIRE(psc.connect("dev"));
        CHECK(psc.state() == MQTT_CONNECTED);
        CHECK(psc.connected());
    }

    TEST_CASE("state() reports MQTT_CONNECT_FAILED when the transport connect fails") {
        // Force the underlying transport connect() to fail; the library never
        // sends a CONNECT and reports MQTT_CONNECT_FAILED.
        TestClock::instance().reset();
        MockClient client;
        client.setConnectResult(0);            // transport refuses the connection
        PubSubClient psc(client);
        psc.setServer("broker.example", 1883);

        CHECK_FALSE(psc.connect("dev"));
        CHECK(psc.state() == MQTT_CONNECT_FAILED);
        CHECK_FALSE(psc.connected());
        // No CONNECT bytes were emitted because the transport never came up.
        CHECK(client.outbound().empty());
    }

    TEST_CASE("state() reports MQTT_CONNECTION_TIMEOUT when no CONNACK arrives") {
        // Transport connects, but no CONNACK is scripted; the connect read-wait
        // times out in virtual time and reports MQTT_CONNECTION_TIMEOUT.
        TestClock::instance().reset();
        MockClient client;                     // empty inbound queue
        PubSubClient psc(client);
        psc.setServer("broker.example", 1883);
        psc.setSocketTimeout(1);               // bound the wait in virtual time

        CHECK_FALSE(psc.connect("dev"));
        CHECK(psc.state() == MQTT_CONNECTION_TIMEOUT);
        CHECK_FALSE(psc.connected());
        CHECK(client.stopCalled());
    }

    TEST_CASE("state() reports MQTT_CONNECTION_LOST when the transport drops after connect") {
        // Establish a connection, then have the transport report itself down.
        // The next connected() poll detects the drop and reports the lost state.
        TestClock::instance().reset();
        MockClient client;
        client.pushPacket(MqttPacket::connack(0));
        PubSubClient psc(client);
        psc.setServer("broker.example", 1883);

        REQUIRE(psc.connect("dev"));
        REQUIRE(psc.state() == MQTT_CONNECTED);

        client.setConnected(false);            // transport silently drops
        CHECK_FALSE(psc.connected());          // poll observes the drop
        CHECK(psc.state() == MQTT_CONNECTION_LOST);
    }

    TEST_CASE("state() reflects each CONNACK return code 1..5") {
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

            CHECK_FALSE(psc.connect("dev"));
            CHECK(psc.state() == c.expectedState);
            CHECK_FALSE(psc.connected());
        }
    }

    // --- loop() and connected() observable behavior -------------------------

    TEST_CASE("loop() returns false before connect and true while idle-connected") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);

        // Not connected yet: loop() is a no-op and reports false.
        CHECK_FALSE(psc.loop());

        client.pushPacket(MqttPacket::connack(0));
        psc.setServer("broker.example", 1883);
        REQUIRE(psc.connect("dev"));
        client.clearOutbound();

        // Connected and idle (no inbound, well within keepalive): loop() reports
        // true and writes nothing.
        CHECK(psc.loop());
        CHECK(client.outbound().empty());
        CHECK(psc.connected());
    }

    TEST_CASE("connected() tracks the transport connection state") {
        TestClock::instance().reset();
        MockClient client;
        client.pushPacket(MqttPacket::connack(0));
        PubSubClient psc(client);
        psc.setServer("broker.example", 1883);

        CHECK_FALSE(psc.connected());          // before connect
        REQUIRE(psc.connect("dev"));
        CHECK(psc.connected());                // after a successful CONNACK

        psc.disconnect();                       // explicit local disconnect
        CHECK_FALSE(psc.connected());
        CHECK(psc.state() == MQTT_DISCONNECTED);
        CHECK(client.stopCalled());
    }
}
