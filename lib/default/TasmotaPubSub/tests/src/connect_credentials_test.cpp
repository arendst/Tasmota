/*
  connect_credentials_test.cpp - CONNECT credential omission, Requirement 16.2.

  Feature: tasmota-pubsub-mqtt5

  Requirement 16 criterion 2 applies to BOTH selections: an empty configured User
  Name - a null pointer OR a zero-length string - omits both the User Name and the
  Password field from the CONNECT payload and clears both flag bits. This file is
  therefore deliberately NOT wrapped in a TEST_SUITE, following the convention of
  vbi_test.cpp / error_model_test.cpp / harness_props_test.cpp, so its cases run in
  both binaries. Tagging it `baseline` would exclude it from the v5 run
  (`make test` runs the v5 binary with -tse=baseline); tagging it `mqtt5` would
  exclude it from the v311 run.

  In the MQTT 3.1.1 selection this is a deliberate, enumerated deviation from the
  pre-migration library, which set the User Name flag and emitted a zero-length
  string for user == "": Appendix B entry 1 of the requirements. It is the only
  place where the 3.1.1 build emits different bytes than the fork it replaces, so
  these cases exist partly to stop a future reader from "fixing" it back.

  The packet is walked byte by byte here rather than handed to the library's own
  reader, and cross-checked against the harness decoder for the active selection
  (decodeConnect5 in 5.0, decodeConnect in 3.1.1). decodeConnect5 reads a Password
  whenever its flag is set, independent of the User Name flag, so a stray password
  left in the payload with the flag set would surface as a field, not as a silent
  framing error.
*/

#include <cstdint>
#include <string>
#include <vector>

#include "doctest.h"

#include "MockClient.h"
#include "MqttPacket.h"
#include "TestClock.h"
#include "PubSubClient.h"

namespace connect_creds {

// Read a 2-byte-length-prefixed UTF-8 string at `pos`, advancing past it.
bool readString(const std::vector<uint8_t>& bytes, size_t& pos, std::string& out) {
    if (pos + 2 > bytes.size()) { return false; }
    const size_t n = (static_cast<size_t>(bytes[pos]) << 8) | bytes[pos + 1];
    pos += 2;
    if (pos + n > bytes.size()) { return false; }
    out.assign(reinterpret_cast<const char*>(bytes.data()) + pos, n);
    pos += n;
    return true;
}

// One CONNECT, walked field by field. The walk is selection-aware in exactly two
// places - the variable-header Properties field and the Will Properties field -
// which is what lets the same cases assert the same credential contract against
// both wire shapes.
struct ConnectView {
    bool        walked = false;
    uint8_t     protocolLevel = 0;
    uint8_t     flags = 0;
    bool        userFlag = false;
    bool        passwordFlag = false;
    bool        willFlag = false;
    std::string clientId;
    std::string willTopic;
    std::string willMessage;
    // `endsAfterLastField` is the point of these cases: an omitted field is not
    // just an unset flag, it is a byte that is not there. The walk consumes every
    // field the flags claim and then requires the packet to be over.
    bool        endsAfterLastField = false;
};

ConnectView walkConnect(const std::vector<uint8_t>& out) {
    ConnectView v;

    uint32_t rl = 0;
    size_t used = 0;
    if (!MqttParser::decodeVbi(out, 1, rl, used)) { return v; }
    size_t pos = 1 + used;

    std::string name;
    if (!readString(out, pos, name)) { return v; }
    if (name != "MQTT") { return v; }
    if (pos + 4 > out.size()) { return v; }
    v.protocolLevel = out[pos++];
    v.flags = out[pos++];
    pos += 2;   // keepalive

    v.userFlag = (v.flags & 0x80) != 0;
    v.passwordFlag = (v.flags & 0x40) != 0;
    v.willFlag = (v.flags & 0x04) != 0;

    if (kMqtt5) {
        const DecodedProperties props = MqttParser::decodeProperties(out, pos);
        if (!props.valid) { return v; }
        pos += props.totalBytes;
    }

    if (!readString(out, pos, v.clientId)) { return v; }

    if (v.willFlag) {
        if (kMqtt5) {
            const DecodedProperties willProps = MqttParser::decodeProperties(out, pos);
            if (!willProps.valid) { return v; }
            pos += willProps.totalBytes;
        }
        if (!readString(out, pos, v.willTopic)) { return v; }
        if (!readString(out, pos, v.willMessage)) { return v; }
    }

    // Deliberately NOT gated on userFlag: when a field is supposed to be omitted,
    // the walk must notice bytes that are still there.
    std::string user;
    std::string pass;
    if (v.userFlag && !readString(out, pos, user)) { return v; }
    if (v.passwordFlag && !readString(out, pos, pass)) { return v; }

    v.endsAfterLastField = (pos == out.size());
    v.walked = true;
    return v;
}

// Cross-check with the harness decoder for the shape the active selection emits.
// The 5.0 decoder additionally requires the packet to end exactly on its last
// field, so it is a second, independent witness for the omission.
DecodedConnect5 decodeForSelection(const std::vector<uint8_t>& out) {
#if MQTT_VERSION == MQTT_VERSION_5_0
    return MqttParser::decodeConnect5(out);
#else
    DecodedConnect5 c5;
    static_cast<DecodedConnect&>(c5) = MqttParser::decodeConnect(out);
    return c5;
#endif
}

// A connected client whose single outbound packet is the CONNECT under test.
struct Session {
    MockClient client;
    PubSubClient psc{client};

    Session() {
        TestClock::instance().reset();
        client.pushPacket(MqttPacket::connack(0));
        psc.setServer("broker.example", 1883);
    }
};

}  // namespace connect_creds

// --- Requirement 16.2: an empty User Name omits both credential fields ------

TEST_CASE("Requirement 16.2: an empty User Name omits User Name and Password") {
    using namespace connect_creds;

    // Every input the requirement calls "empty": the null pointer the Tasmota
    // driver passes, and the zero-length string it forbids nobody from passing.
    // The password column varies independently, because the omission has to take
    // the Password with it even when one was supplied - that is the half the old
    // `user != NULL` branch got wrong.
    struct Case { const char* label; const char* user; const char* pass; };
    const Case cases[] = {
        {"null user, null password",        nullptr, nullptr},
        {"null user, password supplied",    nullptr, "secret"},
        {"empty user, null password",       "",      nullptr},
        {"empty user, password supplied",   "",      "secret"},
        {"empty user, empty password",      "",      ""},
    };

    for (const Case& c : cases) {
        CAPTURE(c.label);
        Session s;
        REQUIRE(s.psc.connect("dev-42", c.user, c.pass, nullptr, 0, false, nullptr, true));

        const std::vector<uint8_t>& out = s.client.outbound();
        CHECK(MqttParser::isStructurallyWellFramed(out));

        const ConnectView v = walkConnect(out);
        REQUIRE(v.walked);
        CHECK(v.protocolLevel == kMqttProtocolLevel);

        // Both flag bits clear...
        CHECK_FALSE(v.userFlag);
        CHECK_FALSE(v.passwordFlag);
        // ...and neither field on the wire: the packet is over after the client id.
        CHECK(v.clientId == "dev-42");
        CHECK(v.endsAfterLastField);

        const DecodedConnect5 d = decodeForSelection(out);
        REQUIRE(d.valid);
        CHECK_FALSE(d.userFlag);
        CHECK_FALSE(d.passwordFlag);
        CHECK(d.username.empty());
        CHECK(d.password.empty());
        CHECK(d.clientId == "dev-42");
    }
}

TEST_CASE("Requirement 16.2: the omission leaves the rest of the payload intact") {
    using namespace connect_creds;

    // The credential fields are last in the payload, so dropping them must not
    // disturb the client id or the Will fields ahead of them - including the
    // MQTT 5.0 Will Properties byte, which sits immediately before the Will Topic.
    struct Case { const char* label; const char* user; const char* pass; };
    const Case cases[] = {
        {"null user with a Will",                 nullptr, nullptr},
        {"empty user with a Will",                "",      nullptr},
        {"empty user, password supplied, Will",   "",      "secret"},
    };

    for (const Case& c : cases) {
        CAPTURE(c.label);
        for (uint8_t qos = 0; qos <= 2; qos++) {
            CAPTURE(qos);
            Session s;
            REQUIRE(s.psc.connect("dev-42", c.user, c.pass,
                                  "tele/dev/LWT", qos, true, "Offline", true));

            const std::vector<uint8_t>& out = s.client.outbound();
            CHECK(MqttParser::isStructurallyWellFramed(out));

            const ConnectView v = walkConnect(out);
            REQUIRE(v.walked);
            CHECK(v.willFlag);
            CHECK(((v.flags >> 3) & 0x03) == qos);
            CHECK((v.flags & 0x20) != 0);          // will retain
            CHECK(v.clientId == "dev-42");
            CHECK(v.willTopic == "tele/dev/LWT");
            CHECK(v.willMessage == "Offline");

            CHECK_FALSE(v.userFlag);
            CHECK_FALSE(v.passwordFlag);
            CHECK(v.endsAfterLastField);           // nothing after the Will message

            const DecodedConnect5 d = decodeForSelection(out);
            REQUIRE(d.valid);
            CHECK(d.willTopic == "tele/dev/LWT");
            CHECK(d.willMessage == "Offline");
            CHECK(d.willQos == qos);
            CHECK(d.willRetain);
            CHECK_FALSE(d.userFlag);
            CHECK_FALSE(d.passwordFlag);
        }
    }
}

// --- Requirement 16.1: a non-empty User Name is unaffected ------------------

TEST_CASE("Requirement 16.1: a non-empty User Name is still emitted, with or without a Password") {
    using namespace connect_creds;

    // The narrowing must be exactly the empty case. A single-character user name
    // is the nearest non-empty neighbour, so it is included alongside a typical one.
    struct Case { const char* user; const char* pass; };
    const Case cases[] = {
        {"user-1", "pass-1"},
        {"user-1", nullptr},
        {"user-1", ""},          // zero-length password with a real user name: still sent
        {"u",      "p"},
    };

    for (const Case& c : cases) {
        CAPTURE(c.user);
        CAPTURE(c.pass == nullptr ? "<null>" : c.pass);
        Session s;
        REQUIRE(s.psc.connect("dev-42", c.user, c.pass, nullptr, 0, false, nullptr, true));

        const std::vector<uint8_t>& out = s.client.outbound();
        CHECK(MqttParser::isStructurallyWellFramed(out));

        const ConnectView v = walkConnect(out);
        REQUIRE(v.walked);
        CHECK(v.userFlag);
        CHECK(v.passwordFlag == (c.pass != nullptr));
        CHECK(v.endsAfterLastField);

        const DecodedConnect5 d = decodeForSelection(out);
        REQUIRE(d.valid);
        CHECK(d.clientId == "dev-42");
        CHECK(d.username == std::string(c.user));
        if (c.pass != nullptr) {
            CHECK(d.password == std::string(c.pass));
        } else {
            CHECK(d.password.empty());
        }
    }
}
