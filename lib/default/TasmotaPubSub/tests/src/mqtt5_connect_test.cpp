/*
  mqtt5_connect_test.cpp - MQTT 5.0 CONNECT emission (task 6.1).

  TEST_SUITE("mqtt5"), body guarded by `#if MQTT_VERSION == MQTT_VERSION_5_0`, so the
  file compiles in both selections and contributes cases only to the 5.0 binary.

  What is pinned here (Requirements 1.1, 1.2, 1.3, 13.6, 16.1, 16.2, 16.3):
    - Protocol name "MQTT", protocol level 5.
    - A Properties field closes the variable header, immediately after the keepalive
      bytes, carrying exactly Receive Maximum (0x21) = MQTT_MAX_INBOUND_INFLIGHT and
      Maximum Packet Size (0x27) = maxIncomingPacketSize, or bufferSize - 1 when no
      inbound cap is configured.
    - Neither Authentication Method (0x15) nor Authentication Data (0x16) is ever
      emitted, and Topic Alias Maximum (0x22) is omitted.
    - The Will Properties field is the single byte 0x00 and sits in the PAYLOAD,
      immediately before the Will Topic.
    - An empty configured User Name - null pointer OR zero-length string - omits both
      the User Name and the Password field and clears both flag bits (Requirement 16.2,
      Appendix B entry 1). That contract applies in both selections, so the shared cases
      live in connect_credentials_test.cpp; what is pinned here is the 5.0 shape around
      the omission.

  What is pinned here for CONNACK (task 6.2 and remediation task 18.3):
    - Only allowed Reason Code 0x00 can connect; an allowed nonzero code closes with
      its mapped legacy state, while a reserved or packet-inapplicable code is rejected.
    - Ack Flags bits 7–1 are zero; Session Present is accepted only for a compatible
      local session after Clean Start clear, and only then remains readable for that
      connection lifetime.
    - The socketTimeout-bounded CONNACK wait loop still fails a silent broker.
    - Malformed (wrong type / non-zero flags / Remaining Length < 3 / trailing byte)
      and adversarial-Properties CONNACKs are rejected as failed connects.

  What is pinned here for the negotiated limits (task 6.3, Requirements 5.1, 5.4, 5.7):
    - Each of Server Keep Alive (0x13), Maximum Packet Size (0x27), Topic Alias
      Maximum (0x22) and Receive Maximum (0x21) is latched from the CONNACK, and an
      omitted property leaves its MQTT 5.0 default (requested keepalive, 0, 0, 65535).
    - Server Keep Alive lands on `keepAlive` itself, so the existing loop() keepalive
      arithmetic and its `keepAlive == 0` disable guard drive the negotiated interval.
    - All four reset at the start of every connection attempt, so the next CONNECT
      requests the configured keepalive rather than the previous broker's.
    - A CONNACK the acceptance test refuses moves no limit.
    Enforcement of the three non-keepalive limits is task 14's, and is not asserted
    here; Property 19 (the exhaustive 16-subset sweep) belongs with it.

  The outbound bytes are decoded with local helpers plus the version-neutral harness
  (`MqttParser::decode`, `MqttParser::decodeProperties`,
  `MqttParser::isStructurallyWellFramed`), not with the library's own reader, so the
  assertions compare two independent implementations. CONNACK fixtures are built with
  MqttPacket::connack5 and MqttProps (adversarial shapes via rawLength()), or scripted
  as raw inbound bytes for structurally malformed frames.

  Task 6.5 appends Property 5 (CONNECT field round-trip) at the end of the file: the
  representative cases above pin one shape each, while Property 5 sweeps a curated
  table of argument and configuration combinations - client ids and Will topics that
  are empty, typical, multi-byte UTF-8 or sized to exactly fill the buffer, Will QoS
  0..2 x retain on/off, clean session on/off, credentials present/absent/empty, and
  buffer/cap pairs - and asserts the whole field set of each emitted CONNECT through
  MqttParser::decodeConnect5 plus isStructurallyWellFramed.

  Task 6.6 appends Property 6 (CONNACK outcome and Session Present round-trip) after
  it: the exhaustive sweep over all 256 reason-code bytes x Session Present {0,1}, the
  same outcome contract re-run over valid CONNACK property sets, and one example case
  per truncation prefix of a complete CONNACK for the Requirement 1.7 timeout. The
  representative CONNACK cases above pin one shape each and stay as the mapping and
  rejection net.
*/

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#include "doctest.h"

#include "MockClient.h"
#include "MqttPacket.h"
#include "TestClock.h"
#include "PubSubClient.h"

#if MQTT_VERSION == MQTT_VERSION_5_0

namespace {

// Read a 2-byte-length-prefixed UTF-8 string at `pos`, advancing `pos` past it.
// Returns false when the field does not fit `bytes`.
bool readString(const std::vector<uint8_t>& bytes, size_t& pos, std::string& out) {
    if (pos + 2 > bytes.size()) { return false; }
    const size_t n = (static_cast<size_t>(bytes[pos]) << 8) | bytes[pos + 1];
    pos += 2;
    if (pos + n > bytes.size()) { return false; }
    out.assign(reinterpret_cast<const char*>(bytes.data()) + pos, n);
    pos += n;
    return true;
}

// Offset of the CONNECT variable header inside the recorded bytes: the fixed header
// byte plus however many bytes the Remaining Length took.
bool variableHeaderStart(const std::vector<uint8_t>& bytes, size_t& out) {
    uint32_t rl = 0;
    size_t used = 0;
    if (!MqttParser::decodeVbi(bytes, 1, rl, used)) { return false; }
    out = 1 + used;
    return true;
}

// Find one property entry by identifier. Returns nullptr when absent.
const DecodedProperty* find(const DecodedProperties& props, uint8_t id) {
    for (const DecodedProperty& e : props.entries) {
        if (e.id == id) { return &e; }
    }
    return nullptr;
}

}  // namespace

TEST_SUITE("mqtt5") {

    // --- Variable-header Properties field (Requirements 1.1, 1.2, 13.6, 16.3) ---

    TEST_CASE("MQTT 5.0 CONNECT closes the variable header with a Properties field") {
        TestClock::instance().reset();
        MockClient client;
        client.pushPacket(MqttPacket::connack(0));

        PubSubClient psc(client);
        psc.setServer("broker.example", 1883);
        REQUIRE(psc.connect("dev-42"));

        const std::vector<uint8_t>& out = client.outbound();
        CHECK(MqttParser::isStructurallyWellFramed(out));

        DecodedPacket generic = MqttParser::decode(out);
        REQUIRE(generic.valid);
        CHECK(generic.type == static_cast<uint8_t>(MQTTCONNECT));
        CHECK(generic.flags == 0x00);

        size_t pos = 0;
        REQUIRE(variableHeaderStart(out, pos));

        std::string name;
        REQUIRE(readString(out, pos, name));
        CHECK(name == "MQTT");
        REQUIRE(pos + 3 <= out.size());
        CHECK(out[pos] == MQTT_VERSION_5_0);           // Requirement 1.1
        const uint8_t connectFlags = out[pos + 1];
        CHECK((connectFlags & 0x02) != 0);             // clean start
        const uint16_t keepAlive =
            static_cast<uint16_t>((out[pos + 2] << 8) | out[pos + 3]);
        CHECK(keepAlive == MQTT_KEEPALIVE);
        pos += 4;

        // Requirement 1.2: the Properties field follows the keepalive bytes.
        DecodedProperties props = MqttParser::decodeProperties(out, pos);
        REQUIRE(props.valid);
        CHECK(props.entries.size() == 2);

        // Requirement 13.6: both values are derived from the client's own limits.
        const DecodedProperty* rm = find(props, 0x21);
        REQUIRE(rm != nullptr);
        CHECK(rm->value == static_cast<uint32_t>(MQTT_MAX_INBOUND_INFLIGHT));
        const DecodedProperty* mps = find(props, 0x27);
        REQUIRE(mps != nullptr);
        // No inbound cap configured by default, so the buffer size less the spare byte.
        CHECK(mps->value == static_cast<uint32_t>(psc.getBufferSize() - 1));

        // Requirement 16.3 / non-goals: no enhanced authentication, no inbound aliases.
        CHECK(find(props, 0x15) == nullptr);           // Authentication Method
        CHECK(find(props, 0x16) == nullptr);           // Authentication Data
        CHECK(find(props, 0x22) == nullptr);           // Topic Alias Maximum

        // The client id follows the Properties field.
        pos += props.totalBytes;
        std::string clientId;
        REQUIRE(readString(out, pos, clientId));
        CHECK(clientId == "dev-42");
        CHECK(pos == out.size());                      // nothing else was emitted
    }

    TEST_CASE("Maximum Packet Size follows the effective inbound capacity") {
        struct Case { uint16_t buffer; uint32_t cap; uint32_t expected; };
        const Case cases[] = {
            {1200, 0,    1199},    // no cap: bufferSize - 1
            {1200, 512,  512},     // cap below the buffer
            {256,  0,    255},
            {256,  4096, 255},     // non-stream input reserves one sentinel byte
        };
        for (const Case& c : cases) {
            CAPTURE(c.buffer);
            CAPTURE(c.cap);
            TestClock::instance().reset();
            MockClient client;
            client.pushPacket(MqttPacket::connack(0));

            PubSubClient psc(client);
            psc.setServer("broker.example", 1883);
            REQUIRE(psc.setBufferSize(c.buffer));
            psc.setMaxIncomingPacketSize(c.cap);
            REQUIRE(psc.connect("dev-42"));

            const std::vector<uint8_t>& out = client.outbound();
            size_t pos = 0;
            REQUIRE(variableHeaderStart(out, pos));
            pos += 6 + 1 + 1 + 2;   // "MQTT" + level + flags + keepalive

            DecodedProperties props = MqttParser::decodeProperties(out, pos);
            REQUIRE(props.valid);
            const DecodedProperty* mps = find(props, 0x27);
            REQUIRE(mps != nullptr);
            CHECK(mps->value == c.expected);
            const DecodedProperty* rm = find(props, 0x21);
            REQUIRE(rm != nullptr);
            CHECK(rm->value == static_cast<uint32_t>(MQTT_MAX_INBOUND_INFLIGHT));
        }
    }

    // --- Will Properties in the payload (Requirement 1.3) -------------------

    TEST_CASE("Will Properties is an empty field in the payload before the Will Topic") {
        for (uint8_t qos = 0; qos <= 2; qos++) {
            for (int retain = 0; retain <= 1; retain++) {
                CAPTURE(qos);
                CAPTURE(retain);
                TestClock::instance().reset();
                MockClient client;
                client.pushPacket(MqttPacket::connack(0));

                PubSubClient psc(client);
                psc.setServer("broker.example", 1883);
                REQUIRE(psc.connect("dev-42", "user-1", "pass-1",
                                    "tele/dev/LWT", qos, retain != 0, "Offline", true));

                const std::vector<uint8_t>& out = client.outbound();
                CHECK(MqttParser::isStructurallyWellFramed(out));

                size_t pos = 0;
                REQUIRE(variableHeaderStart(out, pos));
                std::string name;
                REQUIRE(readString(out, pos, name));
                const uint8_t connectFlags = out[pos + 1];
                CHECK((connectFlags & 0x04) != 0);                        // will flag
                CHECK(((connectFlags >> 3) & 0x03) == qos);
                CHECK((((connectFlags >> 5) & 0x01) != 0) == (retain != 0));
                CHECK((connectFlags & 0x80) != 0);                        // user name
                CHECK((connectFlags & 0x40) != 0);                        // password
                pos += 4;

                DecodedProperties props = MqttParser::decodeProperties(out, pos);
                REQUIRE(props.valid);
                pos += props.totalBytes;

                std::string clientId;
                REQUIRE(readString(out, pos, clientId));
                CHECK(clientId == "dev-42");

                // Requirement 1.3: the Will Properties field sits here, immediately
                // before the Will Topic, and is the empty set encoded as 0x00.
                REQUIRE(pos < out.size());
                CHECK(out[pos] == 0x00);
                DecodedProperties willProps = MqttParser::decodeProperties(out, pos);
                REQUIRE(willProps.valid);
                CHECK(willProps.declaredLength == 0);
                CHECK(willProps.entries.empty());
                CHECK(willProps.totalBytes == 1);
                pos += willProps.totalBytes;

                std::string willTopic;
                std::string willMessage;
                REQUIRE(readString(out, pos, willTopic));
                REQUIRE(readString(out, pos, willMessage));
                CHECK(willTopic == "tele/dev/LWT");
                CHECK(willMessage == "Offline");

                std::string user;
                std::string pass;
                REQUIRE(readString(out, pos, user));
                REQUIRE(readString(out, pos, pass));
                CHECK(user == "user-1");
                CHECK(pass == "pass-1");
                CHECK(pos == out.size());
            }
        }
    }

    // --- Credentials unchanged (Requirements 16.1, 16.2) -------------------

    TEST_CASE("A null user omits both User Name and Password") {
        TestClock::instance().reset();
        MockClient client;
        client.pushPacket(MqttPacket::connack(0));

        PubSubClient psc(client);
        psc.setServer("broker.example", 1883);
        REQUIRE(psc.connect("dev-42"));

        const std::vector<uint8_t>& out = client.outbound();
        size_t pos = 0;
        REQUIRE(variableHeaderStart(out, pos));
        std::string name;
        REQUIRE(readString(out, pos, name));
        const uint8_t connectFlags = out[pos + 1];
        CHECK((connectFlags & 0x80) == 0);   // no User Name flag
        CHECK((connectFlags & 0x40) == 0);   // no Password flag
        pos += 4;

        DecodedProperties props = MqttParser::decodeProperties(out, pos);
        REQUIRE(props.valid);
        pos += props.totalBytes;
        std::string clientId;
        REQUIRE(readString(out, pos, clientId));
        // Nothing follows the client id: no Will, no User Name, no Password.
        CHECK(pos == out.size());
    }

    TEST_CASE("An empty-string user omits both User Name and Password") {
        // Requirement 16.2 / Appendix B entry 1: "empty" covers the zero-length
        // string as well as the null pointer, and the Password goes with it even
        // when one was supplied. Decoded with decodeConnect5, which reads a
        // Password whenever its flag is set - independent of the User Name flag -
        // so a field left behind under a stale flag shows up as a field.
        struct Case { const char* label; const char* pass; };
        const Case cases[] = {
            {"empty user, null password",     nullptr},
            {"empty user, password supplied", "pass-1"},
        };

        for (const Case& c : cases) {
            CAPTURE(c.label);
            TestClock::instance().reset();
            MockClient client;
            client.pushPacket(MqttPacket::connack(0));

            PubSubClient psc(client);
            psc.setServer("broker.example", 1883);
            REQUIRE(psc.connect("dev-42", "", c.pass,
                                "tele/dev/LWT", 1, false, "Offline", true));

            const std::vector<uint8_t>& out = client.outbound();
            CHECK(MqttParser::isStructurallyWellFramed(out));

            DecodedConnect5 d = MqttParser::decodeConnect5(out);
            REQUIRE(d.valid);            // ends exactly on its last field
            CHECK(d.protocolLevel == MQTT_VERSION_5_0);
            CHECK_FALSE(d.userFlag);
            CHECK_FALSE(d.passwordFlag);
            CHECK(d.username.empty());
            CHECK(d.password.empty());

            // The 5.0 shape around the omission is untouched: both Properties
            // fields and the Will pair still decode where they belong.
            CHECK(d.props.valid);
            CHECK(d.props.entries.size() == 2);
            CHECK(d.willProps.valid);
            CHECK(d.willProps.declaredLength == 0);
            CHECK(d.willProps.entries.empty());
            CHECK(d.clientId == "dev-42");
            CHECK(d.willTopic == "tele/dev/LWT");
            CHECK(d.willMessage == "Offline");

            size_t pos = 0;
            REQUIRE(variableHeaderStart(out, pos));
            std::string name;
            REQUIRE(readString(out, pos, name));
            CHECK((out[pos + 1] & 0x80) == 0);   // User Name flag clear
            CHECK((out[pos + 1] & 0x40) == 0);   // Password flag clear
        }
    }

    // --- Refusal leaves nothing on the wire (Requirement 13.2) -------------

    TEST_CASE("A buffer too small for the CONNECT Properties field refuses without emitting") {
        TestClock::instance().reset();
        MockClient client;
        client.pushPacket(MqttPacket::connack(0));

        PubSubClient psc(client);
        psc.setServer("broker.example", 1883);
        // 16 bytes holds the 5 reserved header bytes plus the 10 bytes of protocol name,
        // level, flags and keepalive, but not the 9-byte Properties field that follows
        // (1 length byte + 3 for Receive Maximum + 5 for Maximum Packet Size).
        REQUIRE(psc.setBufferSize(16));

        CHECK_FALSE(psc.connect("dev-42"));
        CHECK(client.outbound().empty());
        CHECK_FALSE(psc.connected());
        CHECK(psc.lastError() == MqttError::BufferTooSmall);
    }

    // --- CONNACK acceptance and outcome (Requirements 1.4, 1.5, 1.6, 1.7) ---
    //
    // Task 6.2 coverage: the reason-code -> state() mapping from the design's
    // table, the Session Present latch and its lifetime, the socketTimeout-bounded
    // wait loop, and the malformed / truncated / adversarial-Properties shapes the
    // acceptance test must reject. This stays REPRESENTATIVE on purpose - Property 6
    // (task 6.6) is the exhaustive 256-code x session-present sweep and the
    // per-truncation-prefix timeout; here we pin the mapped codes, one unmapped code
    // on each side of the collapse, and one instance of every rejection shape.

    // Requirement 17: only Reason Code 0x00 connects. Allowed nonzero codes close
    // with their mapped legacy state; raw bytes remain available for valid refusals.
    TEST_CASE("CONNACK reason code maps onto state() and connected()") {
        struct Case { uint8_t rc; bool ok; int state; };
        const Case cases[] = {
            {0x00, true,  MQTT_CONNECTED},               //     Success
            // 0x84 initiates Requirement 24 fallback and is covered by Property 33.
            {0x85, false, MQTT_CONNECT_BAD_CLIENT_ID},   //  2  Client Identifier not valid
            {0x86, false, MQTT_CONNECT_BAD_CREDENTIALS}, //  4  Bad User Name or Password
            {0x87, false, MQTT_CONNECT_UNAUTHORIZED},    //  5  Not authorized
            {0x88, false, MQTT_CONNECT_UNAVAILABLE},     //  3  Server unavailable
            {0x89, false, MQTT_CONNECT_UNAVAILABLE},     //  3  Server busy
            {0x80, false, MQTT_CONNECT_FAILED},          // -2  Unspecified error (unmapped)
            {0x9F, false, MQTT_CONNECT_FAILED},          // -2  Connection rate exceeded (unmapped)
        };
        for (const Case& c : cases) {
            CAPTURE(c.rc);
            TestClock::instance().reset();
            MockClient client;
            client.pushPacket(MqttPacket::connack5(c.rc));

            PubSubClient psc(client);
            psc.setServer("broker.example", 1883);

            const bool connected = psc.connect("dev-42");
            CHECK(connected == c.ok);
            CHECK(psc.state() == c.state);
            // The raw Reason Code is preserved either way, so codes that collapse
            // onto one state() value (0x88 and 0x89 both map to 3) stay distinct.
            CHECK(psc.connackReasonCode() == c.rc);
            if (c.ok) {
                CHECK(psc.connected());
                CHECK_FALSE(client.stopCalled());
            } else {
                CHECK_FALSE(psc.connected());
                CHECK(client.stopCalled());       // Requirement 1.5: closed
            }
        }
    }

    // A fresh Clean Start has no compatible local session state. Session Present is
    // therefore accepted only when clear, even for Reason Code 0x00.
    TEST_CASE("Session Present requires a compatible local session") {
        for (int sp = 0; sp <= 1; sp++) {
            CAPTURE(sp);
            TestClock::instance().reset();
            MockClient client;
            client.pushPacket(MqttPacket::connack5(0x00, sp != 0));

            PubSubClient psc(client);
            psc.setServer("broker.example", 1883);
            CHECK(psc.connect("dev-42") == (sp == 0));
            CHECK_FALSE(psc.sessionPresent());
            CHECK(psc.lastError() == (sp == 0 ? MqttError::None
                                               : MqttError::ProtocolError));
        }
    }

    // A semantically rejected Session Present response leaves no outcome latched; the
    // next attempt begins with the same clean diagnostic state.
    TEST_CASE("Rejected Session Present leaves no CONNACK outcome latched") {
        TestClock::instance().reset();
        MockClient client;
        client.pushPacket(MqttPacket::connack5(0x00, /*sessionPresent=*/true));

        PubSubClient psc(client);
        psc.setServer("broker.example", 1883);
        CHECK_FALSE(psc.connect("dev-42"));
        CHECK(psc.lastError() == MqttError::ProtocolError);
        CHECK_FALSE(psc.sessionPresent());
        CHECK(psc.connackReasonCode() == 0);

        client.setConnectResult(0);
        CHECK_FALSE(psc.connect("dev-42"));
        CHECK_FALSE(psc.sessionPresent());
        CHECK(psc.connackReasonCode() == 0);
    }

    // Requirement 1.7: no complete CONNACK within the socket timeout is a failed
    // connect, with the connection closed.
    TEST_CASE("No CONNACK within the socket timeout fails the connect") {
        TestClock::instance().reset();
        MockClient client;                 // nothing scripted inbound
        PubSubClient psc(client);
        psc.setServer("broker.example", 1883);
        psc.setSocketTimeout(1);           // bound the wait in virtual time

        CHECK_FALSE(psc.connect("dev-42"));
        CHECK(psc.state() == MQTT_CONNECTION_TIMEOUT);
        CHECK_FALSE(psc.connected());
        CHECK(client.stopCalled());
    }

    // Requirements 1.4/1.5 acceptance test: a CONNACK that fails the structural
    // checks (wrong type, non-zero fixed-header flags, Remaining Length below 3, or
    // a stray trailing byte reachable only by a short read) is rejected, the
    // connection closed, and a failed connect reported with a malformed-packet
    // diagnostic. Raw bytes are scripted so the exact broken shape is on the wire.
    TEST_CASE("A structurally malformed CONNACK is rejected and closes the connection") {
        struct Case { const char* label; std::vector<uint8_t> raw; };
        const Case cases[] = {
            // Type is CONNACK (0x2_) but a fixed-header flag bit is set: the
            // acceptance test compares the whole byte against 0x20.
            {"non-zero fixed-header flags", {0x21, 0x03, 0x00, 0x00, 0x00}},
            // Wrong packet type entirely (SUBACK-shaped bytes).
            {"wrong packet type",           {0x90, 0x03, 0x00, 0x00, 0x00}},
            // Remaining Length below 3: no room for the mandatory Properties byte.
            {"remaining length below 3",    {0x20, 0x02, 0x00, 0x00}},
        };
        for (const Case& c : cases) {
            CAPTURE(c.label);
            TestClock::instance().reset();
            MockClient client;
            client.pushInbound(c.raw);

            PubSubClient psc(client);
            psc.setServer("broker.example", 1883);
            psc.setSocketTimeout(1);

            CHECK_FALSE(psc.connect("dev-42"));
            CHECK(psc.state() == MQTT_CONNECT_FAILED);
            CHECK_FALSE(psc.connected());
            CHECK(client.stopCalled());
            CHECK(psc.lastError() == MqttError::MalformedPacket);
        }
    }

    // Check 2 of the acceptance test in isolation: `len < vh + 3u`, i.e. at least 3
    // bytes of variable header (ack flags + reason code + a Properties field, which
    // is never optional in 5.0).
    //
    // WHY THIS CASE EXISTS AS ITS OWN CASE - do not fold it back in or delete it as
    // redundant with the "remaining length below 3" fixture above. The three lengths
    // exercise three DIFFERENT internal paths to the same observable outcome, and no
    // one of them stands in for the others:
    //
    //   - Remaining Length 2 (len == 4, propsOff == 4) proves rejection but NOT which
    //     check rejected. Relax the bound to `len < vh + 2u` - which is exactly
    //     `len < propsOff` - and this fixture is still rejected, only now by check 3:
    //     decodeProperties() is handed avail == 0 and fails on the missing mandatory
    //     Property Length byte. Verified by perturbation: at +2u all 39 mqtt5 cases
    //     stayed green. So this length alone leaves check 2 unpinned, and it is kept
    //     here to pin the boundary from the inside - 2 is the largest length refused.
    //   - Remaining Length 1 and 0 (len == 3 and 2) are the shapes where propsOff
    //     EXCEEDS len, so `len - propsOff` is a uint32_t subtraction that underflows.
    //     Check 2 is the only guard against that: delete it (verified by perturbation:
    //     `if (false)`) and decodeProperties() is handed avail == 0xFFFFFFFF and
    //     0xFFFFFFFE respectively, reading bytes the broker never sent.
    //
    // What the perturbation ALSO showed, and what a future reader should know before
    // trusting these fixtures for more than they carry: with check 2 deleted the
    // CONNACK is still rejected and ASan still reports nothing. The over-read lands on
    // the residual bytes of the outbound CONNECT that readPacket() overwrote only the
    // first three of, all inside the same working-buffer allocation. buffer[4] is that
    // CONNECT's Remaining Length byte, read as a plausible Property Length, and
    // buffer[5] is the high byte of the "MQTT" protocol-name length - 0x00, never a
    // legal identifier - so decodeProperties() stops with ProtocolError before it can
    // walk off the allocation, and check 4 would refuse the frame anyway since
    // propsOff + consumed can never equal a len below propsOff. The underflow is
    // therefore real but latent here, and no fixture reachable through the public API
    // can turn it into a sanitizer report: check 4 makes acceptance impossible, and
    // the bytes at the underflowed offset are not ours to choose.
    //
    // These three fixtures are consequently a shape-coverage net, not a mutation
    // discriminator: they pin the observable contract (any CONNACK with fewer than 3
    // variable-header bytes is a failed connect reporting MalformedPacket, whatever
    // the internal reason) across all three lengths, so a future rewrite of the
    // acceptance test that starts trusting a short frame - or that reaches the
    // underflowed decodeProperties() call with a buffer whose residual contents are
    // NOT a fast ProtocolError - fails here first. Kept as its own case so the failure
    // output names the boundary rather than a generic malformed-shape table.
    TEST_CASE("A CONNACK with fewer than 3 variable-header bytes is rejected") {
        struct Case { const char* label; std::vector<uint8_t> raw; };
        const Case cases[] = {
            // Ack flags + reason code, no Properties field. propsOff == len, so
            // decodeProperties would see avail == 0.
            {"remaining length 2 (properties field missing)", {0x20, 0x02, 0x00, 0x00}},
            // Ack flags only. propsOff == len + 1 -> `len - propsOff` underflows.
            {"remaining length 1 (reason code missing)",      {0x20, 0x01, 0x00}},
            // Empty variable header. propsOff == len + 2 -> underflows.
            {"remaining length 0 (empty variable header)",    {0x20, 0x00}},
        };
        for (const Case& c : cases) {
            CAPTURE(c.label);
            TestClock::instance().reset();
            MockClient client;
            client.pushInbound(c.raw);

            PubSubClient psc(client);
            psc.setServer("broker.example", 1883);
            psc.setSocketTimeout(1);

            CHECK_FALSE(psc.connect("dev-42"));
            CHECK(psc.state() == MQTT_CONNECT_FAILED);
            CHECK_FALSE(psc.connected());
            CHECK(client.stopCalled());
            CHECK(psc.lastError() == MqttError::MalformedPacket);
        }
    }

    // Acceptance condition "the Properties field parses cleanly within the received
    // bytes": each fixture is well-framed at the fixed-header level, so only the
    // Properties check can reject it. Adversarial fields are built with MqttProps,
    // including declared-length over-run/under-run via rawLength().
    TEST_CASE("A CONNACK whose Properties field does not parse is rejected") {
        struct Case { const char* label; MqttProps props; MqttError error; };
        std::vector<Case> cases;
        // 0x01 Payload Format Indicator is a PUBLISH property, illegal in CONNACK.
        cases.push_back({"illegal identifier for CONNACK", MqttProps().byte_(0x01, 1),
                         MqttError::ProtocolError});
        // 0x21 Receive Maximum is a single-occurrence identifier.
        cases.push_back({"duplicated single-occurrence identifier",
                         MqttProps().u16(0x21, 10).u16(0x21, 20),
                         MqttError::ProtocolError});
        // Declared Property Length runs past the bytes actually received.
        cases.push_back({"property length overruns the packet",
                         MqttProps().u16(0x21, 10).rawLength(50),
                         MqttError::MalformedPacket});
        // Declared Property Length shorter than the body leaves trailing bytes a
        // CONNACK (which has no payload) cannot account for.
        cases.push_back({"trailing byte after the properties field",
                         MqttProps().u16(0x21, 10).rawLength(0),
                         MqttError::MalformedPacket});
        // Requirements 3.11, 16.5, 16.6: Authentication Method / Authentication Data
        // are rejected in ANY packet type, including one CONNACK marks legal for other
        // identifiers - CONNACK is one of the two contexts the pre-rejection table row
        // used to list as legal for these identifiers, so this is the case that proves
        // the rejection is unconditional rather than merely absent from the emitted set.
        cases.push_back({"authentication method in CONNACK",
                         MqttProps().str(0x15, "SCRAM-SHA-1"), MqttError::ProtocolError});
        cases.push_back({"authentication data in CONNACK",
                         MqttProps().bin(0x16, {0x00, 0x7F}), MqttError::ProtocolError});

        for (const Case& c : cases) {
            CAPTURE(c.label);
            TestClock::instance().reset();
            MockClient client;
            client.pushPacket(MqttPacket::connack5(0x00, false, c.props));

            PubSubClient psc(client);
            psc.setServer("broker.example", 1883);
            psc.setSocketTimeout(1);

            CHECK_FALSE(psc.connect("dev-42"));
            CHECK(psc.state() == MQTT_CONNECT_FAILED);
            CHECK_FALSE(psc.connected());
            CHECK(client.stopCalled());
            CHECK(psc.lastError() == c.error);
        }
    }

    // --- Negotiated limits latched from CONNACK (Requirements 5.1, 5.4, 5.7) ---
    //
    // Task 6.3 coverage: parse and latch only. What each limit then DOES to an
    // outbound packet is task 14's, and is not asserted here - except for Server Keep
    // Alive, whose whole point is that it lands on `keepAlive` itself and therefore
    // drives the existing loop() arithmetic with no new code, so that IS asserted.
    // Property 19 (the exhaustive 16-subset sweep) belongs to the enforcement task.

    // Requirement 5.7: a CONNACK that declares nothing leaves every default standing.
    TEST_CASE("A CONNACK with no properties leaves the MQTT 5.0 defaults in place") {
        TestClock::instance().reset();
        MockClient client;
        client.pushPacket(MqttPacket::connack5(0x00));

        PubSubClient psc(client);
        psc.setServer("broker.example", 1883);
        psc.setKeepAlive(37);
        REQUIRE(psc.connect("dev-42"));

        CHECK(psc.getKeepAlive() == 37);                  // requested value kept
        CHECK(psc.serverMaximumPacketSize() == 0);        // no packet-size limit
        CHECK(psc.serverTopicAliasMaximum() == 0);
        CHECK(psc.getServerReceiveMaximum() == 65535);
    }

    // Requirements 5.1, 5.4, 5.7: each property is latched from the value the broker
    // declared, and the ones it left out keep their defaults. Boundary values included
    // so a width or sign mistake shows up.
    TEST_CASE("Each negotiated CONNACK property is latched independently") {
        struct Case {
            const char* label;
            MqttProps   props;
            uint16_t    keepAlive;      // expected effective keepalive
            uint32_t    maxPacket;
            uint16_t    aliasMax;
            uint16_t    receiveMax;
        };
        std::vector<Case> cases;
        // Server Keep Alive alone, across its whole range. 0 is a legal declared value
        // and means "keepalive disabled", which is why presence cannot be inferred
        // from the value.
        cases.push_back({"server keep alive 0",     MqttProps().u16(0x13, 0),     0,     0, 0, 65535});
        cases.push_back({"server keep alive 1",     MqttProps().u16(0x13, 1),     1,     0, 0, 65535});
        cases.push_back({"server keep alive 30",    MqttProps().u16(0x13, 30),    30,    0, 0, 65535});
        cases.push_back({"server keep alive 65535", MqttProps().u16(0x13, 65535), 65535, 0, 0, 65535});
        // Maximum Packet Size alone (Four Byte).
        cases.push_back({"maximum packet size 1",       MqttProps().u32(0x27, 1),          20, 1,          0, 65535});
        cases.push_back({"maximum packet size 512",     MqttProps().u32(0x27, 512),        20, 512,        0, 65535});
        cases.push_back({"maximum packet size huge",    MqttProps().u32(0x27, 268435455u), 20, 268435455u, 0, 65535});
        // Topic Alias Maximum alone. 0 IS meaningful here - "send me no alias" - so it
        // is latched as declared rather than treated as absent.
        cases.push_back({"topic alias maximum 0",     MqttProps().u16(0x22, 0),     20, 0, 0,     65535});
        cases.push_back({"topic alias maximum 1",     MqttProps().u16(0x22, 1),     20, 0, 1,     65535});
        cases.push_back({"topic alias maximum 65535", MqttProps().u16(0x22, 65535), 20, 0, 65535, 65535});
        // Receive Maximum alone.
        cases.push_back({"receive maximum 1",     MqttProps().u16(0x21, 1),     20, 0, 0, 1});
        cases.push_back({"receive maximum 2",     MqttProps().u16(0x21, 2),     20, 0, 0, 2});
        cases.push_back({"receive maximum 65535", MqttProps().u16(0x21, 65535), 20, 0, 0, 65535});
        // All four at once, plus an unrelated property that must be ignored rather
        // than disturb the latch.
        cases.push_back({"all four declared",
                         MqttProps().u16(0x13, 45).u32(0x27, 900).u16(0x22, 8).u16(0x21, 3),
                         45, 900, 8, 3});
        cases.push_back({"all four plus an unrelated property",
                         MqttProps().u16(0x13, 45).str(0x1C, "other/broker")
                                    .u32(0x27, 900).u16(0x22, 8).u16(0x21, 3),
                         45, 900, 8, 3});

        for (const Case& c : cases) {
            CAPTURE(c.label);
            TestClock::instance().reset();
            MockClient client;
            client.pushPacket(MqttPacket::connack5(0x00, false, c.props));

            PubSubClient psc(client);
            psc.setServer("broker.example", 1883);
            psc.setKeepAlive(20);                  // the requested value
            REQUIRE(psc.connect("dev-42"));

            CHECK(psc.getKeepAlive() == c.keepAlive);
            CHECK(psc.serverMaximumPacketSize() == c.maxPacket);
            CHECK(psc.serverTopicAliasMaximum() == c.aliasMax);
            CHECK(psc.getServerReceiveMaximum() == c.receiveMax);
        }
    }

    // Requirement 5.1: adopting Server Keep Alive into `keepAlive` itself is what makes
    // the existing loop() arithmetic - and its `keepAlive == 0` disable guard - apply to
    // the negotiated interval with no new code. Asserted through the emitted PINGREQ,
    // which is the only observable the arithmetic has.
    TEST_CASE("Server Keep Alive drives the existing loop() keepalive arithmetic") {
        SUBCASE("a shorter negotiated interval pings sooner than the requested one") {
            TestClock::instance().reset();
            MockClient client;
            client.pushPacket(MqttPacket::connack5(0x00, false, MqttProps().u16(0x13, 10)));

            PubSubClient psc(client);
            psc.setServer("broker.example", 1883);
            psc.setKeepAlive(600);                 // requested: 10 minutes
            REQUIRE(psc.connect("dev-42"));
            CHECK(psc.getKeepAlive() == 10);
            client.clearOutbound();

            // Past the negotiated 10 s but nowhere near the requested 600 s.
            TestClock::instance().advance(10001);
            CHECK(psc.loop());
            const std::vector<uint8_t> ping = {MQTTPINGREQ, 0x00};
            CHECK(client.outbound() == ping);
        }

        SUBCASE("Server Keep Alive 0 disables keepalive entirely") {
            TestClock::instance().reset();
            MockClient client;
            client.pushPacket(MqttPacket::connack5(0x00, false, MqttProps().u16(0x13, 0)));

            PubSubClient psc(client);
            psc.setServer("broker.example", 1883);
            psc.setKeepAlive(10);                  // would have pinged at 10 s
            REQUIRE(psc.connect("dev-42"));
            CHECK(psc.getKeepAlive() == 0);
            client.clearOutbound();

            // Well past any interval the client could have kept: the guard holds.
            TestClock::instance().advance(3600000);
            CHECK(psc.loop());
            CHECK(client.outbound().empty());
            CHECK(psc.connected());
        }
    }

    // Requirement 5.7: the reset boundary is the START of a connection attempt, so
    // nothing a previous broker declared can govern the next connection - and the next
    // CONNECT requests the keepalive setKeepAlive() was given, not the interval the
    // previous broker imposed.
    TEST_CASE("Negotiated limits reset at the start of every connection attempt") {
        TestClock::instance().reset();
        MockClient client;
        client.pushPacket(MqttPacket::connack5(
            0x00, false,
            MqttProps().u16(0x13, 45).u32(0x27, 900).u16(0x22, 8).u16(0x21, 3)));

        PubSubClient psc(client);
        psc.setServer("broker.example", 1883);
        psc.setKeepAlive(20);
        REQUIRE(psc.connect("dev-42"));
        CHECK(psc.getKeepAlive() == 45);
        CHECK(psc.serverMaximumPacketSize() == 900);
        CHECK(psc.serverTopicAliasMaximum() == 8);
        CHECK(psc.getServerReceiveMaximum() == 3);

        psc.disconnect();

        // Second attempt against a broker that declares nothing: every limit is back to
        // its default, including the keepalive, which returns to the requested 20.
        client.clearOutbound();
        client.clearInbound();
        client.pushPacket(MqttPacket::connack5(0x00));
        REQUIRE(psc.connect("dev-42"));
        CHECK(psc.getKeepAlive() == 20);
        CHECK(psc.serverMaximumPacketSize() == 0);
        CHECK(psc.serverTopicAliasMaximum() == 0);
        CHECK(psc.getServerReceiveMaximum() == 65535);

        // And the CONNECT it emitted requested 20 s, not the 45 s the first broker
        // imposed - the reset happens before the packet is assembled.
        DecodedConnect5 d = MqttParser::decodeConnect5(client.outbound());
        REQUIRE(d.valid);
        CHECK(d.keepAlive == 20);
    }

    // A CONNACK the acceptance test refuses must not move a limit: the collector runs
    // during validation, but latching happens only after every check has passed.
    TEST_CASE("A refused CONNACK leaves the negotiated limits at their defaults") {
        TestClock::instance().reset();
        MockClient client;
        // Well-framed properties carrying all four limits, but a trailing byte the
        // Properties length does not account for, so check 4 rejects the packet after
        // the visitor has already seen every entry.
        client.pushPacket(MqttPacket::connack5(
            0x00, false,
            MqttProps().u16(0x13, 45).u32(0x27, 900).u16(0x22, 8).u16(0x21, 3)
                       .rawLength(3)));

        PubSubClient psc(client);
        psc.setServer("broker.example", 1883);
        psc.setKeepAlive(20);
        psc.setSocketTimeout(1);

        CHECK_FALSE(psc.connect("dev-42"));
        CHECK(psc.lastError() == MqttError::MalformedPacket);
        CHECK(psc.getKeepAlive() == 20);
        CHECK(psc.serverMaximumPacketSize() == 0);
        CHECK(psc.serverTopicAliasMaximum() == 0);
        CHECK(psc.getServerReceiveMaximum() == 65535);
    }

    TEST_CASE("Session Expiry maps legacy cleanSession modes and configured boundaries") {
        struct Case { uint32_t expiry; bool persistent; };
        const Case cases[] = {
            {0, false}, {0, true}, {1, true}, {3600, true}, {0xFFFFFFFFu, true},
        };
        for (const Case& c : cases) {
            CAPTURE(c.expiry);
            CAPTURE(c.persistent);
            TestClock::instance().reset();
            MockClient client;
            client.pushPacket(MqttPacket::connack5(0x00));
            PubSubClient psc(client);
            psc.setServer("broker.example", 1883);
            psc.setSessionExpiryInterval(c.expiry);
            CHECK(psc.getSessionExpiryInterval() == c.expiry);

// The unchanged eight-argument overload is what the Tasmota driver calls. It must
// preserve Clean Start false and emit the selected Session Expiry Interval.
            REQUIRE(psc.connect("tasmota-persistent", nullptr, nullptr, nullptr, 0,
                                false, nullptr, !c.persistent));
            const DecodedConnect5 decoded = MqttParser::decodeConnect5(client.outbound());
            REQUIRE(decoded.valid);
            CHECK(decoded.cleanSession == !c.persistent);
            const DecodedProperty* expiry = find(decoded.props, 0x11);
            if (c.persistent) {
                REQUIRE(expiry != nullptr);
                CHECK(expiry->value == c.expiry);
            } else {
                CHECK(expiry == nullptr);
            }
        }
    }

    // === Property 5: CONNECT field round-trip (task 6.5) ====================

namespace mqtt5_p5 {

// One CONNECT invocation, plus the configuration it runs under. Defaults describe the
// simplest accepted call (`connect("dev-42")` with a clean session, no Will and no
// credentials); each case below overrides only what it is about.
struct Case {
    std::string name;

    std::string id = "dev-42";

    // Pointer semantics are preserved deliberately: nullptr and "" are DIFFERENT
    // inputs for the user name (Requirement 16.2 collapses them onto the same wire
    // shape, which is exactly what has to be checked rather than assumed).
    const char* user = nullptr;
    const char* pass = nullptr;

    bool        will = false;
    std::string willTopic;
    uint8_t     willQos = 0;
    bool        willRetain = false;
    std::string willMessage = "Offline";

    bool     cleanSession = true;
    uint16_t keepAlive = MQTT_KEEPALIVE;

    // 0 with exactFit false means "leave the library default buffer alone".
    uint16_t buffer = 0;
    // exactFit sizes the buffer to the assembled packet, so the id or topic in that
    // case is the longest one that still fits.
    bool     exactFit = false;

    uint32_t cap = 0;      // setMaxIncomingPacketSize(); 0 = no cap configured
};

// Clean Start 1 emits the two universal properties. Clean Start 0 also carries the
// five-byte Session Expiry Interval property (identifier plus four-byte value).
static constexpr size_t kBasePropsBodyBytes  = 3 + 5;
static constexpr size_t kSessionExpiryBytes  = 5;
static size_t propsBodyBytes(const Case& c) {
    return kBasePropsBodyBytes + (c.cleanSession ? 0 : kSessionExpiryBytes);
}
static size_t propsTotalBytes(const Case& c) {
    return 1 + propsBodyBytes(c);
}

// Requirement 16.2: an empty user name - null OR zero-length - omits both fields.
static bool hasUser(const Case& c) {
    return (c.user != nullptr) && (c.user[0] != '\0');
}

// Bytes the assembly occupies in the working buffer, which is what the buffer-capacity
// checks in connect() compare against: the reserved header area, the variable header,
// then the payload fields in wire order.
static size_t assembledSize(const Case& c) {
    size_t n = MQTT_MAX_HEADER_SIZE;   // reserved for the fixed header
    n += 7;                            // 00 04 'M' 'Q' 'T' 'T' <level>
    n += 1;                            // connect flags
    n += 2;                            // keepalive
    n += propsTotalBytes(c);          // variable-header Properties
    n += 2 + c.id.size();              // client id
    if (c.will) {
        n += 1;                                  // Will Properties (empty set)
        n += 2 + c.willTopic.size();
        n += 2 + c.willMessage.size();
    }
    if (hasUser(c)) {
        n += 2 + strlen(c.user);
        if (c.pass != nullptr) {
            n += 2 + strlen(c.pass);
        }
    }
    return n;
}

}  // namespace mqtt5_p5

// Feature: tasmota-pubsub-mqtt5, Property 5: For all valid combinations of client id, optional user name, optional password, optional Will (topic, QoS 0..2, retain, message), clean-session flag, configured buffer size and configured inbound packet cap, the emitted CONNECT decodes to a packet whose protocol name is "MQTT", whose protocol level is 5, whose flags and keepalive match the arguments, whose variable-header Properties field declares Receive Maximum and Maximum Packet Size derived from the configured limits, whose Will Properties field appears in the payload immediately before the Will Topic, and whose embedded strings equal the supplied arguments — with User Name and Password both omitted whenever the user name is absent or empty.
TEST_CASE("Property 5: CONNECT field round-trip") {
    using namespace mqtt5_p5;

    // "e" + U+00E9 (2 bytes) + U+20AC (3 bytes): a length in bytes that differs from
    // the length in characters, so a decoder counting the wrong one shows up.
    const std::string kUtf8 = "e\xC3\xA9\xE2\x82\xAC";

    std::vector<Case> cases;
    auto add = [&cases](Case c) { cases.push_back(c); };
    auto base = [](const std::string& name) { Case c; c.name = name; return c; };

    // --- Client ids -------------------------------------------------------
    add(base("typical client id"));
    { Case c = base("empty client id");            c.id = "";                     add(c); }
    { Case c = base("empty CONNECT at documented minimum capacity");
      c.id = ""; c.buffer = MQTT_CONNECT_MINIMUM_BUFFER_SIZE_5_0;                  add(c); }
    { Case c = base("single-character client id"); c.id = "x";                    add(c); }
    { Case c = base("multi-byte UTF-8 client id"); c.id = kUtf8;                  add(c); }
    { Case c = base("client id needing a 2-byte Remaining Length");
      c.id = std::string(200, 'i'); c.buffer = 1200;                              add(c); }
    // Max-fitting: the buffer is sized to the assembled packet, so one more byte of
    // client id would be refused by the capacity check.
    { Case c = base("client id exactly filling the buffer");
      c.id = std::string(120, 'i'); c.exactFit = true;                            add(c); }

    // --- Will: QoS 0..2 x retain on/off ----------------------------------
    for (uint8_t qos = 0; qos <= 2; qos++) {
        for (int retain = 0; retain <= 1; retain++) {
            Case c = base("will QoS " + std::to_string((unsigned)qos) +
                          (retain ? ", retain on" : ", retain off"));
            c.will = true;
            c.willTopic = "tele/dev/LWT";
            c.willQos = qos;
            c.willRetain = (retain != 0);
            c.willMessage = "Offline";
            add(c);
        }
    }
    { Case c = base("empty will topic and message");
      c.will = true; c.willTopic = ""; c.willMessage = "";                        add(c); }
    { Case c = base("multi-byte UTF-8 will topic and message");
      c.will = true; c.willTopic = kUtf8; c.willMessage = kUtf8; c.willQos = 2;   add(c); }
    { Case c = base("will payload carrying 0x00-free binary-ish text");
      c.will = true; c.willTopic = "tele/dev/LWT";
      c.willMessage = std::string("\x01\x02\x7F\xFF");                            add(c); }
    { Case c = base("will topic exactly filling the buffer");
      c.will = true; c.willTopic = std::string(80, 't'); c.willQos = 1;
      c.willRetain = true; c.exactFit = true;                                     add(c); }

    // --- Credentials (Requirements 16.1, 16.2) ---------------------------
    { Case c = base("user and password");        c.user = "user-1"; c.pass = "pass-1"; add(c); }
    { Case c = base("user without password");    c.user = "user-1";                    add(c); }
    { Case c = base("empty user with password"); c.user = ""; c.pass = "pass-1";       add(c); }
    { Case c = base("empty user without password"); c.user = "";                       add(c); }
    { Case c = base("null user with password");  c.pass = "pass-1";                    add(c); }
    { Case c = base("multi-byte UTF-8 credentials");
      c.user = "us\xC3\xA9r"; c.pass = "p\xE2\x82\xACss";                              add(c); }
    { Case c = base("will plus credentials");
      c.will = true; c.willTopic = "tele/dev/LWT"; c.willQos = 2; c.willRetain = true;
      c.user = "user-1"; c.pass = "pass-1";                                            add(c); }
    { Case c = base("credentials exactly filling the buffer");
      c.user = "user-1"; c.pass = "pass-1"; c.exactFit = true;                         add(c); }

    // --- Clean session and keepalive -------------------------------------
    { Case c = base("clean session off");             c.cleanSession = false;           add(c); }
    { Case c = base("clean session off with a will");
      c.cleanSession = false; c.will = true; c.willTopic = "tele/dev/LWT";              add(c); }
    { Case c = base("keepalive 0");                   c.keepAlive = 0;                  add(c); }
    { Case c = base("keepalive 1");                   c.keepAlive = 1;                  add(c); }
    { Case c = base("keepalive 65535");               c.keepAlive = 65535;              add(c); }

    // --- Buffer / cap combinations (Requirement 13.6) --------------------
    { Case c = base("buffer 64, no cap");        c.buffer = 64;                    add(c); }
    { Case c = base("buffer 256, no cap");       c.buffer = 256;                   add(c); }
    { Case c = base("buffer 1200, no cap");      c.buffer = 1200;                  add(c); }
    // A tiny cap is still a valid configuration, but it also bounds the INBOUND
    // CONNACK: 5 bytes for the empty-properties shape, so 8 is about as small as a
    // cap can be and still admit a successful connect.
    { Case c = base("buffer 256, cap 8");        c.buffer = 256;  c.cap = 8;       add(c); }
    { Case c = base("buffer 256, cap 512");      c.buffer = 256;  c.cap = 512;     add(c); }
    { Case c = base("buffer 256, cap above the buffer");
      c.buffer = 256; c.cap = 4096;                                               add(c); }
    { Case c = base("buffer 1200, cap 65535");   c.buffer = 1200; c.cap = 65535;   add(c); }
    { Case c = base("buffer 1200, cap 0xFFFFFFFF");
      c.buffer = 1200; c.cap = 0xFFFFFFFFu;                                       add(c); }
    { Case c = base("exact-fit buffer with a cap");
      c.user = "user-1"; c.pass = "pass-1"; c.cap = 300; c.exactFit = true;        add(c); }

    for (const Case& c : cases) {
        SUBCASE(c.name.c_str()) {
            TestClock::instance().reset();
            MockClient client;
            client.pushPacket(MqttPacket::connack(0));

            PubSubClient psc(client);
            psc.setServer("broker.example", 1883);
            psc.setKeepAlive(c.keepAlive);
            if (c.exactFit) {
                REQUIRE(psc.setBufferSize((uint16_t)assembledSize(c)));
            } else if (c.buffer != 0) {
                REQUIRE(psc.setBufferSize(c.buffer));
            }
            if (c.cap != 0) {
                psc.setMaxIncomingPacketSize(c.cap);
            }
            // Every case is a combination the configuration accepts, so the connect
            // itself is part of the property: a refusal here is a failure, not a skip.
            REQUIRE(psc.connect(c.id.c_str(), c.user, c.pass,
                                c.will ? c.willTopic.c_str() : nullptr,
                                c.willQos, c.willRetain,
                                c.will ? c.willMessage.c_str() : nullptr,
                                c.cleanSession));

            const std::vector<uint8_t>& out = client.outbound();
            CHECK(MqttParser::isStructurallyWellFramed(out));

            // Size arithmetic: fixed-header byte + Remaining Length bytes + body. This
            // is what makes the exact-fit cases meaningful - the assembly used every
            // byte of the configured buffer bar the unused header slack.
            const size_t body = assembledSize(c) - MQTT_MAX_HEADER_SIZE;
            const std::vector<uint8_t> rl = MqttPacket::encodeVbi((uint32_t)body);
            REQUIRE_FALSE(rl.empty());
            CHECK(out.size() == 1 + rl.size() + body);

            DecodedConnect5 d = MqttParser::decodeConnect5(out);
            REQUIRE(d.valid);

            // Requirement 1.1: protocol name and level.
            CHECK(d.protocolName == "MQTT");
            CHECK(d.protocolLevel == MQTT_VERSION_5_0);

            // Flags and keepalive match the arguments.
            CHECK(d.keepAlive == c.keepAlive);
            CHECK(d.cleanSession == c.cleanSession);
            CHECK(d.willFlag == c.will);
            CHECK(d.willQos == (c.will ? c.willQos : (uint8_t)0));
            CHECK(d.willRetain == (c.will && c.willRetain));
            CHECK((d.connectFlags & 0x01) == 0);        // reserved bit stays clear

            // Requirements 16.1, 16.2: both credential fields go together, and an
            // empty user name - null or zero-length - omits both.
            const bool user = hasUser(c);
            CHECK(d.userFlag == user);
            CHECK(d.passwordFlag == (user && (c.pass != nullptr)));
            CHECK(d.username == (user ? std::string(c.user) : std::string()));
            CHECK(d.password == ((user && c.pass != nullptr) ? std::string(c.pass)
                                                             : std::string()));

            // Requirements 1.2, 13.6, 18.7: the variable-header Properties field
            // declares the two limits, plus Session Expiry only for Clean Start 0.
            REQUIRE(d.props.valid);
            CHECK(d.props.declaredLength == propsBodyBytes(c));
            CHECK(d.props.totalBytes == propsTotalBytes(c));
            REQUIRE(d.props.entries.size() == (c.cleanSession ? 2u : 3u));
            const DecodedProperty* rm = find(d.props, 0x21);
            REQUIRE(rm != nullptr);
            CHECK(rm->value == (uint32_t)MQTT_MAX_INBOUND_INFLIGHT);
            const DecodedProperty* mps = find(d.props, 0x27);
            REQUIRE(mps != nullptr);
            const uint32_t bufferedMaxPacket =
                (uint32_t)psc.getBufferSize() - 1;
            const uint32_t expectedMaxPacket =
                (c.cap != 0 && c.cap < bufferedMaxPacket) ? c.cap : bufferedMaxPacket;
            CHECK(mps->value == expectedMaxPacket);
            const DecodedProperty* expiry = find(d.props, 0x11);
            if (c.cleanSession) {
                CHECK(expiry == nullptr);
            } else {
                REQUIRE(expiry != nullptr);
                CHECK(expiry->value == MQTT_PERSISTENT_SESSION_EXPIRY_INTERVAL);
            }
            // Non-goals: no enhanced authentication, no inbound topic aliases.
            CHECK(find(d.props, 0x15) == nullptr);
            CHECK(find(d.props, 0x16) == nullptr);
            CHECK(find(d.props, 0x22) == nullptr);

            // The embedded strings equal the supplied arguments.
            CHECK(d.clientId == c.id);
            if (c.will) {
                CHECK(d.willTopic == c.willTopic);
                CHECK(d.willMessage == c.willMessage);
                // Requirement 1.3: the Will Properties field is the empty set.
                REQUIRE(d.willProps.valid);
                CHECK(d.willProps.declaredLength == 0);
                CHECK(d.willProps.entries.empty());
                CHECK(d.willProps.totalBytes == 1);
            } else {
                CHECK(d.willTopic.empty());
                CHECK(d.willMessage.empty());
                // No Will means no Will Properties field at all, which the harness
                // reports as "not decoded" rather than as an empty field.
                CHECK_FALSE(d.willProps.valid);
            }

            // Requirement 1.3, positional half: an independent walk from the front of
            // the packet, so "immediately before the Will Topic" is asserted against
            // the byte offsets rather than inferred from decodeConnect5's own walk.
            size_t pos = 0;
            REQUIRE(variableHeaderStart(out, pos));
            std::string name;
            REQUIRE(readString(out, pos, name));
            CHECK(name == "MQTT");
            REQUIRE(pos + 4 <= out.size());
            CHECK(out[pos] == MQTT_VERSION_5_0);
            pos += 1 + 1 + 2;                      // level, flags, keepalive
            DecodedProperties vh = MqttParser::decodeProperties(out, pos);
            REQUIRE(vh.valid);
            pos += vh.totalBytes;                  // Properties closes the variable header
            std::string decodedId;
            REQUIRE(readString(out, pos, decodedId));
            CHECK(decodedId == c.id);
            if (c.will) {
                REQUIRE(pos < out.size());
                CHECK(out[pos] == 0x00);           // the Will Properties field itself
                pos += 1;
                std::string topic;
                REQUIRE(readString(out, pos, topic));
                CHECK(topic == c.willTopic);       // and the Will Topic right after it
            }
        }
    }
}

    // === Property 6: CONNACK outcome and Session Present round-trip (task 6.6) ===
    //
    // The task 6.2 cases above pin the mapped reason codes one at a time; this sweeps
    // the WHOLE reason-code byte space (all 256 values x Session Present {0,1}), then
    // re-runs the outcome over valid CONNACK property sets to show that what the
    // broker declares alongside the reason code cannot move the outcome, and finally
    // walks every truncation prefix of a complete CONNACK for Requirement 1.7.

namespace mqtt5_p6 {

// The design's Reason Code -> state() table, written out as the LITERAL legacy
// numbers instead of through the MQTT_CONNECT_* macros the library maps with. The
// numbering is what the driver's diagnostic switch reads, so spelling it out here
// makes this an independent check rather than a restatement of the library's own
// constants: renaming a macro cannot silently move a code onto a different value.
static bool isAllowedConnackReason(uint8_t rc) {
    switch (rc) {
        case 0x00: case 0x80: case 0x81: case 0x82: case 0x83: case 0x84:
        case 0x85: case 0x86: case 0x87: case 0x88: case 0x89: case 0x8A:
        case 0x8C: case 0x90: case 0x95: case 0x97: case 0x99: case 0x9A:
        case 0x9B: case 0x9C: case 0x9D: case 0x9F:
            return true;
        default:
            return false;
    }
}

static int expectedState(uint8_t rc) {
    if (rc == 0x00) { return 0; }                // MQTT_CONNECTED
    switch (rc) {
        case 0x84: return 1;                     // Unsupported Protocol Version
        case 0x85: return 2;                     // Client Identifier not valid
        case 0x86: return 4;                     // Bad User Name or Password
        case 0x87: return 5;                     // Not authorized
        case 0x88:                               // Server unavailable
        case 0x89: return 3;                     // Server busy
        default:   return -2;                    // MQTT_CONNECT_FAILED
    }
}

// One CONNACK delivered to a fresh client, with every observable the property talks
// about checked on the way out. Returns nothing: everything is asserted here.
//
// The Session Present lifetime is checked in the same pass, because "remains readable
// until the next connection attempt begins" is a statement about a boundary and needs
// both sides of it: the value survives the connection ending (disconnect(), or the
// close the library itself performed for a refusing reason code) and is cleared only
// once the next attempt starts. The next attempt here fails at the transport, so it
// can never latch a value of its own - a false reading afterwards therefore proves the
// clear came from the attempt boundary rather than from a fresh CONNACK.
static void checkOutcome(uint8_t rc, bool sessionPresent, const MqttProps& props) {
    // This helper sends Clean Start 1 and has no retained local state. Therefore a
    // Session Present bit is illegal even for the otherwise successful reason code.
    const bool semanticallyValid = isAllowedConnackReason(rc) && !sessionPresent;
    const bool ok = semanticallyValid && (rc == 0x00);

    TestClock::instance().reset();
    MockClient client;
    const MqttPacket fixture = MqttPacket::connack5(rc, sessionPresent, props);
    REQUIRE(MqttParser::isStructurallyWellFramed(fixture.bytes()));
    client.pushPacket(fixture);

    PubSubClient psc(client);
    psc.setServer("broker.example", 1883);
    psc.setSocketTimeout(1);

    CHECK(psc.connect("dev-42") == ok);
    CHECK(psc.state() == (semanticallyValid ? expectedState(rc) : MQTT_CONNECT_FAILED));
    CHECK(psc.connected() == ok);
    CHECK(client.stopCalled() == !ok);
    // Only a complete semantically valid CONNACK latches its diagnostic reason code.
    CHECK(psc.connackReasonCode() == (semanticallyValid ? rc : 0));
    CHECK_FALSE(psc.sessionPresent());
    CHECK(psc.lastError() == (semanticallyValid ? MqttError::None
                                                 : MqttError::ProtocolError));

    psc.disconnect();
    CHECK_FALSE(psc.connected());
    CHECK_FALSE(psc.sessionPresent());
    CHECK(psc.connackReasonCode() == (semanticallyValid ? rc : 0));

    client.setConnectResult(0);
    CHECK_FALSE(psc.connect("dev-42"));
    CHECK_FALSE(psc.sessionPresent());
    CHECK(psc.connackReasonCode() == 0);
}

static MqttPacket connackWithAckFlags(uint8_t ackFlags, uint8_t reasonCode,
                                      const MqttProps& props = MqttProps()) {
    std::vector<uint8_t> body;
    body.push_back(ackFlags);
    body.push_back(reasonCode);
    const std::vector<uint8_t> propertyBytes = props.bytes();
    body.insert(body.end(), propertyBytes.begin(), propertyBytes.end());

    std::vector<uint8_t> bytes;
    bytes.push_back(MQTTCONNACK);
    const std::vector<uint8_t> remaining = MqttPacket::encodeVbi((uint32_t)body.size());
    REQUIRE_FALSE(remaining.empty());
    bytes.insert(bytes.end(), remaining.begin(), remaining.end());
    bytes.insert(bytes.end(), body.begin(), body.end());
    return MqttPacket::raw(bytes);
}

static void establishResumableLocalState(MockClient& client, PubSubClient& psc) {
    client.pushPacket(MqttPacket::connack5(0x00, false));
    REQUIRE(psc.connect("dev-42", nullptr, nullptr, nullptr, 0, false, nullptr,
                        /*cleanSession=*/false));
// The persistent-session identity captured by a cleanSession=false connect is itself the
// resumable local state Property 6 needs. A QoS 1 in-flight publish is added on top of it
// only when USE_MQTT_QOS is compiled in; without it there is no outbound QoS state to hold.
#ifdef USE_MQTT_QOS
    const uint8_t payload = 'q';
    REQUIRE(psc.publish("tele/dev/STATE", &payload, 1, false, 1));
    REQUIRE(psc.inFlightCount() == 1);
#endif  // USE_MQTT_QOS
    psc.disconnect();
    client.clearInbound();
    client.clearOutbound();
}

}  // namespace mqtt5_p6

// Feature: tasmota-pubsub-mqtt5, Property 6: For all CONNACK Reason Codes, Ack Flags, Session Present values, Clean Start values, local-session-state values, and valid property subsets, the client establishes a connection only for Reason Code 0x00 with zero reserved Ack Flags and a Session Present value permitted by Clean Start and compatible local state. Every rejected combination leaves negotiated limits and replay state unchanged.
TEST_CASE("Property 6: CONNACK outcome and Session Present round-trip") {
    using namespace mqtt5_p6;

    // Exhaustive over the reason-code byte, so the 0x80 boundary and every mapped code
    // are covered by enumeration rather than by sampling.
    SUBCASE("every reason code x session present") {
        for (unsigned rc = 0; rc <= 0xFF; rc++) {
            // Requirement 24 gives 0x84 a separate two-attempt outcome, covered by
            // Property 33; this matrix retains every terminal CONNACK outcome.
            if (rc == 0x84) { continue; }
            for (int sp = 0; sp <= 1; sp++) {
                CAPTURE(rc);
                CAPTURE(sp);
                checkOutcome((uint8_t)rc, sp != 0, MqttProps());
            }
        }
    }

    SUBCASE("every Ack Flags value is validated before state changes") {
        for (unsigned flags = 0; flags <= 0xFF; ++flags) {
            CAPTURE(flags);
            TestClock::instance().reset();
            MockClient client;
            client.pushPacket(connackWithAckFlags((uint8_t)flags, 0x00));

            PubSubClient psc(client);
            psc.setServer("broker.example", 1883);
            // Clean Start is clear here, so flag 1 is rejected specifically because
            // there is no compatible local session state; every other nonzero flag
            // also has reserved bits set.
            const bool ok = flags == 0;
            CHECK(psc.connect("dev-42", nullptr, nullptr, nullptr, 0, false,
                              nullptr, /*cleanSession=*/false) == ok);
            CHECK(psc.sessionPresent() == false);
            CHECK(psc.connackReasonCode() == (ok ? 0 : 0));
            CHECK(psc.lastError() == (ok ? MqttError::None : MqttError::ProtocolError));
            CHECK(client.stopCalled() == !ok);
        }
    }

    SUBCASE("Session Present requires Clean Start clear and compatible local state") {
        for (int clean = 0; clean <= 1; ++clean) {
            for (int local = 0; local <= 1; ++local) {
                for (int sp = 0; sp <= 1; ++sp) {
                    CAPTURE(clean);
                    CAPTURE(local);
                    CAPTURE(sp);
                    TestClock::instance().reset();
                    MockClient client;
                    PubSubClient psc(client);
                    psc.setServer("broker.example", 1883);
                    if (local != 0) {
                        establishResumableLocalState(client, psc);
                    }
                    client.pushPacket(MqttPacket::connack5(0x00, sp != 0));

                    const bool ok = (sp == 0) || ((clean == 0) && (local != 0));
                    CHECK(psc.connect("dev-42", nullptr, nullptr, nullptr, 0, false,
                                      nullptr, clean != 0) == ok);
                    CHECK(psc.sessionPresent() == (ok && (sp != 0)));
                    if (ok && (sp != 0)) {
#ifdef USE_MQTT_QOS
                        // A compatible Session Present response performs the one allowed
                        // replay. The replayed QoS 1 PUBLISH carries DUP in its header.
                        // Without outbound QoS 1/2 there is no in-flight publish to replay;
                        // the persistent session identity alone makes Session Present valid.
                        bool sawDuplicatePublish = false;
                        for (uint8_t byte : client.outbound()) {
                            if (byte == (MQTTPUBLISH | MQTTQOS1 | 0x08)) {
                                sawDuplicatePublish = true;
                            }
                        }
                        CHECK(sawDuplicatePublish);
#endif  // USE_MQTT_QOS
                    } else if (!ok) {
                        // Invalid combinations send only the CONNECT; they neither apply
                        // candidates nor invoke replay on the retained local operation.
                        CHECK(MqttParser::isStructurallyWellFramed(client.outbound()));
                    }
                }
            }
        }
    }

    SUBCASE("invalid CONNACK combinations do not apply limits or replay") {
        TestClock::instance().reset();
        MockClient client;
        PubSubClient psc(client);
        psc.setServer("broker.example", 1883);
        psc.setKeepAlive(20);
        establishResumableLocalState(client, psc);
        client.pushPacket(connackWithAckFlags(
            0x03, 0x00, MqttProps().u16(0x13, 45).u32(0x27, 900)
                                  .u16(0x22, 8).u16(0x21, 3)));

        CHECK_FALSE(psc.connect("dev-42", nullptr, nullptr, nullptr, 0, false,
                                nullptr, /*cleanSession=*/false));
        CHECK(psc.lastError() == MqttError::ProtocolError);
        CHECK(psc.getKeepAlive() == 20);
        CHECK(psc.serverMaximumPacketSize() == 0);
        CHECK(psc.serverTopicAliasMaximum() == 0);
        CHECK(psc.getServerReceiveMaximum() == 65535);
        CHECK(MqttParser::isStructurallyWellFramed(client.outbound()));
    }

    // The same semantic matrix, now with valid CONNACK property subsets. Properties
    // can never turn an invalid reason/flag/session combination into an accepted one,
    // and they are committed only on the successful 0x00 outcome.
    SUBCASE("valid CONNACK property sets do not disturb the outcome") {
        // "e" + U+00E9 + U+20AC: byte length differs from character length.
        const std::string kUtf8 = "e\xC3\xA9\xE2\x82\xAC";

        struct Set { const char* label; MqttProps props; };
        std::vector<Set> sets;
        sets.push_back({"empty set", MqttProps()});
        sets.push_back({"session expiry interval", MqttProps().u32(0x11, 3600)});
        sets.push_back({"assigned client identifier", MqttProps().str(0x12, "auto-7")});
        sets.push_back({"server keep alive", MqttProps().u16(0x13, 30)});
        // Authentication Method (0x15) and Authentication Data (0x16) are NOT in this
        // list: they are rejected identifiers (Requirements 3.11, 16.5, 16.6), not
        // valid CONNACK properties, so their coverage is the dedicated rejection test
        // below rather than this "does not disturb the outcome" sweep.
        sets.push_back({"response information", MqttProps().str(0x1A, "resp/dev")});
        sets.push_back({"server reference", MqttProps().str(0x1C, "other.example:1883")});
        sets.push_back({"reason string", MqttProps().str(0x1F, "not today")});
        sets.push_back({"multi-byte UTF-8 reason string", MqttProps().str(0x1F, kUtf8)});
        sets.push_back({"empty reason string", MqttProps().str(0x1F, "")});
        sets.push_back({"receive maximum", MqttProps().u16(0x21, 20)});
        sets.push_back({"topic alias maximum", MqttProps().u16(0x22, 8)});
        sets.push_back({"maximum qos", MqttProps().byte_(0x24, 1)});
        sets.push_back({"retain available", MqttProps().byte_(0x25, 0)});
        sets.push_back({"maximum packet size", MqttProps().u32(0x27, 900)});
        sets.push_back({"one user property",
                        MqttProps().pair(0x26, "k", "v")});
        // 0x26 is the repeatable identifier, including with a duplicated key.
        sets.push_back({"three user properties, one key repeated",
                        MqttProps().pair(0x26, "a", "1")
                                   .pair(0x26, "a", "2")
                                   .pair(0x26, "b", "3")});
        sets.push_back({"empty user property key and value",
                        MqttProps().pair(0x26, "", "")});
        sets.push_back({"the three availability flags",
                        MqttProps().byte_(0x28, 1).byte_(0x29, 0).byte_(0x2A, 1)});
        sets.push_back({"every CONNACK identifier at once",
                        MqttProps().u32(0x11, 3600).str(0x12, "auto-7").u16(0x13, 30)
                                   .str(0x1A, "resp/dev").str(0x1C, "other:1883")
                                   .str(0x1F, "why").u16(0x21, 20).u16(0x22, 8)
                                   .byte_(0x24, 1).byte_(0x25, 1).u32(0x27, 900)
                                   .pair(0x26, "k1", "v1").pair(0x26, "k2", "v2")
                                   .byte_(0x28, 1).byte_(0x29, 1).byte_(0x2A, 1)});

        // Both sides of the 0x80 boundary, every mapped code, and unmapped codes at
        // each end of the failing range.
        const uint8_t codes[] = {0x00, 0x01, 0x7F, 0x80, 0x85, 0x86,
                                 0x87, 0x88, 0x89, 0x9F, 0xFF};

        for (const Set& s : sets) {
            for (uint8_t rc : codes) {
                for (int sp = 0; sp <= 1; sp++) {
                    CAPTURE(s.label);
                    CAPTURE(rc);
                    CAPTURE(sp);
                    checkOutcome(rc, sp != 0, s.props);
                }
            }
        }
    }

    // Requirement 1.7: no COMPLETE CONNACK within the socket timeout is a failed
    // connect with the connection closed. Every truncation prefix of a well-framed
    // CONNACK is an incomplete one, so all of them must fail - and none of them may
    // latch an outcome, since there is no complete packet to read one from.
    //
    // The prefixes split into two internal paths, and the split is asserted rather
    // than papered over with "any failing state will do":
    //   - 0 bytes: connect()'s own `while (!available())` wait expires, which is the
    //     literal socket-timeout path, so state() is MQTT_CONNECTION_TIMEOUT.
    //   - 1..len-1 bytes: available() is non-zero so that wait falls through, and
    //     readPacket() then times out inside readByte() mid-packet and returns 0. That
    //     lands on the same acceptance test as a malformed frame, so state() is
    //     MQTT_CONNECT_FAILED with a MalformedPacket diagnostic.
    // Either way the virtual clock has moved past the configured socket timeout, which
    // is what makes these timeout cases rather than instant rejections - the assertion
    // on TestClock is what would catch a future short-circuit that failed the connect
    // without ever waiting.
    SUBCASE("every truncation prefix of a CONNACK fails the connect on the timeout") {
        const std::vector<uint8_t> full =
            MqttPacket::connack5(0x00, /*sessionPresent=*/true,
                                 MqttProps().u16(0x13, 30)).bytes();
        REQUIRE(MqttParser::isStructurallyWellFramed(full));
        REQUIRE(full.size() == 8);          // 0x20 0x06 | flags rc | 03 13 00 1E

        for (size_t n = 0; n < full.size(); n++) {
            CAPTURE(n);
            TestClock::instance().reset();
            MockClient client;
            if (n > 0) {
                client.pushInbound(std::vector<uint8_t>(full.begin(),
                                                        full.begin() + (long)n));
            }

            PubSubClient psc(client);
            psc.setServer("broker.example", 1883);
            psc.setSocketTimeout(1);        // 1000 ms of virtual time

            CHECK_FALSE(psc.connect("dev-42"));
            CHECK_FALSE(psc.connected());
            CHECK(client.stopCalled());
            // The wait really was bounded by socketTimeout, not short-circuited.
            CHECK(TestClock::instance().millis() >= 1000);

            if (n == 0) {
                CHECK(psc.state() == MQTT_CONNECTION_TIMEOUT);
            } else {
                CHECK(psc.state() == MQTT_CONNECT_FAILED);
                CHECK(psc.lastError() == MqttError::MalformedPacket);
            }

            // Nothing was latched: an incomplete CONNACK carries no outcome, even
            // though its Session Present bit and Reason Code bytes may already have
            // arrived (prefixes of 3 and 4 bytes contain both).
            CHECK_FALSE(psc.sessionPresent());
            CHECK(psc.connackReasonCode() == 0);
        }
    }
}
}

#endif   // MQTT_VERSION == MQTT_VERSION_5_0
