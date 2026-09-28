/*
  MqttPacket.h - Structural MQTT 3.1.1 control-packet builder for the
  TasmotaPubSub host test system (task 7.1).

  Tests express intent structurally rather than hard-coding brittle magic-byte
  arrays: a builder produces a complete, correctly framed control packet whose
  bytes can be scripted as inbound (via MockClient::pushPacket / pushInbound) or
  compared against recorded outbound bytes.

  The Remaining Length codec here MIRRORS the library's PubSubClient::buildHeader
  exactly (1..4 length bytes, bounded so a value can never emit a fifth byte), so
  fixtures stay in sync with the code under test. Packet-type constants are
  REUSED from PubSubClient.h (MQTTPUBLISH, MQTTCONNACK, ...) rather than
  redefined, so the fixtures automatically track any change in the library's own
  definitions.

  The decoder / structural validators (MqttParser) are added in task 7.2 at
  the bottom of this header. They let tests assert on recorded MockClient
  outbound bytes by DECODING them structurally (round-trip against the public
  API arguments), never by comparing brittle magic-byte arrays. Every assertion
  therefore goes through decoded wire bytes only, so the suite stays durable
  across a future MQTT 5 migration (Requirement 19.1).

  Task 4.4 (tasmota-pubsub-mqtt5) adds the MQTT 5.0 layer: MqttProps builds a
  Properties field (including adversarial ones via rawLength), MqttParser::
  decodeProperties reads one back, and MqttParser::isStructurallyWellFramed
  generalizes isStructurallyValidPublish to every packet type. All of it is
  written WITHOUT calling the library's own properties writer or reader, so
  round-trip tests compare two independent implementations rather than a function
  against itself (Requirements 15.4, 15.5).
*/

#ifndef TASMOTA_PUBSUB_TEST_MQTT_PACKET_H
#define TASMOTA_PUBSUB_TEST_MQTT_PACKET_H

#include <cstdint>
#include <string>
#include <vector>

// Reuse the library's own packet-type / QoS constants so fixtures cannot drift
// out of sync with the code under test. This header pulls in the Arduino shim
// transitively (Arduino.h / Client.h / Stream.h / IPAddress.h), which is fine
// on the host include path (src/lib is searched first).
#include "PubSubClient.h"

// ===========================================================================
//  MqttProps - MQTT 5.0 Properties-field fixture builder (task 4.4)
// ===========================================================================
//
// Builds the bytes of a single MQTT 5.0 Properties field: `<VBI length><body>`.
// Entries are appended in call order, so a fixture controls the exact wire order
// of the identifiers it declares (which matters for the repeatable identifiers
// 0x26 User Property and 0x0B Subscription Identifier).
//
// The builder is deliberately UNVALIDATING: it never checks that an identifier
// is legal for a packet type, that a single-occurrence identifier appears once,
// or that a value width matches the identifier's type. That is precisely what
// makes it usable for the adversarial corpus - the library under test is the one
// required to reject those packets, and the fixture must be able to express
// them. Legality is asserted on the library's side, never enforced here.
//
// It is also independent of the library: nothing here calls
// PubSubClient::writeProperties / MqttPropertyWriter, so a round-trip test
// compares two implementations rather than one function against itself
// (Requirements 15.4, 15.5).
//
// Every product is a sized std::vector<uint8_t>, never a fixed C array, so the
// fixture code itself can introduce no out-of-bounds access.
class MqttProps {
public:
    MqttProps() = default;

    // --- Entry builders (each appends `<id><value>` to the body) -------------

    // Byte value (e.g. 0x01 Payload Format Indicator, 0x24 Maximum QoS).
    MqttProps& byte_(uint8_t id, uint8_t v);
    // Two Byte Integer, big-endian (e.g. 0x21 Receive Maximum, 0x23 Topic Alias).
    MqttProps& u16(uint8_t id, uint16_t v);
    // Four Byte Integer, big-endian (e.g. 0x27 Maximum Packet Size).
    MqttProps& u32(uint8_t id, uint32_t v);
    // Variable Byte Integer (e.g. 0x0B Subscription Identifier). Encoded with
    // encodeRemainingLength, NOT encodeVbi, so a fixture can still express an
    // over-16,383 value the library is required to reject.
    MqttProps& varint(uint8_t id, uint32_t v);
    // UTF-8 string: 2-byte big-endian length prefix + bytes.
    MqttProps& str(uint8_t id, const std::string& s);
    // Binary Data: 2-byte big-endian length prefix + bytes (0x00 bytes allowed).
    MqttProps& bin(uint8_t id, const std::vector<uint8_t>& d);
    // UTF-8 string pair (0x26 User Property): key string then value string.
    MqttProps& pair(uint8_t id, const std::string& k, const std::string& v);

    // Adversarial hook: force the emitted Property Length prefix to `declared`
    // instead of the actual body length, so a fixture can declare more bytes
    // than it carries (over-run) or fewer (under-run). Entries added after this
    // call still go into the body; the declared length stays as set.
    MqttProps& rawLength(uint32_t declared);

    // --- Products -----------------------------------------------------------

    // The complete Properties field: `<VBI length><body>`. The length prefix is
    // the actual body length unless rawLength() overrode it. An empty set
    // therefore yields the single byte 0x00.
    std::vector<uint8_t> bytes() const;

    // The body alone (no length prefix), for tests that need to compare the
    // declared length against what is really there.
    const std::vector<uint8_t>& body() const { return _body; }
    bool empty() const { return _body.empty(); }

private:
    std::vector<uint8_t> _body;
    bool     _rawLength = false;
    uint32_t _declaredLength = 0;
};

// A single, immutable MQTT control packet represented as its exact wire bytes.
//
// Instances are produced only through the static builder functions below; the
// value can then be handed to MockClient::pushPacket() (inbound scripting) or
// its bytes() compared against decoded outbound bytes.
class MqttPacket {
public:
    // --- Builders ----------------------------------------------------------

    // CONNACK in the shape the *selected* protocol accepts (task 6.2): the 3.1.1
    // layout below in the 3.1.1 build, and connack5(returnCode, sessionPresent) - so
    // an empty Properties field, Remaining Length 3 - in the MQTT 5.0 build. Every
    // fixture that just wants a scripted successful connect therefore stays correct
    // in both selections without being edited.
    //
    // 3.1.1 layout: 0x20 0x02 <ackFlags> <returnCode>.
    // ackFlags bit0 is the Session Present flag; all other bits are zero.
    static MqttPacket connack(uint8_t returnCode, bool sessionPresent = false);

    // CONNACK, MQTT 5.0 shape (task 6.4):
    //   variable header = AckFlags(1, bit0 = Session Present)
    //                     | ReasonCode(1)
    //                     | Properties
    // There is no payload. The Properties field is ALWAYS present in 5.0, so the
    // default empty set still emits one byte, 0x00 - a 5.0 CONNACK therefore has
    // a minimum Remaining Length of 3, never 2.
    //
    // `props` is taken as an MqttProps so an adversarial fixture (over-declared
    // or under-declared Property Length, illegal identifier, duplicated
    // single-occurrence identifier) is expressible; nothing here validates it.
    static MqttPacket connack5(uint8_t reasonCode,
                               bool sessionPresent = false,
                               const MqttProps& props = MqttProps());

    // PUBLISH with a raw byte payload.
    //   fixed header = MQTTPUBLISH | (qos << 1) | (retained ? 1 : 0)
    //   variable header = topic (2-byte length prefix + bytes)
    //                     + packet identifier (2 bytes) ONLY when qos > 0
    //   payload         = raw bytes
    // msgId is emitted only for qos > 0 (a QoS 0 PUBLISH has no packet id).
    static MqttPacket publish(const std::string& topic,
                              const std::vector<uint8_t>& payload,
                              uint8_t qos = 0,
                              bool retained = false,
                              uint16_t msgId = 0);

    // PUBLISH convenience overload taking a string payload.
    static MqttPacket publish(const std::string& topic,
                              const std::string& payload,
                              uint8_t qos = 0,
                              bool retained = false,
                              uint16_t msgId = 0);

    // PUBLISH, MQTT 5.0 shape (task 7.3):
    //   fixed header = MQTTPUBLISH | (dup ? 0x08 : 0) | (qos << 1) | (retained ? 1 : 0)
    //   variable header = topic (2-byte length prefix + bytes)
    //                     + packet identifier (2 bytes) ONLY when qos > 0
    //                     + Properties
    //   payload         = raw bytes
    // The Properties field is ALWAYS present in 5.0, exactly as connack5()
    // above - an empty `props` still emits the single byte 0x00, placed after
    // the Packet Identifier when qos > 0 and immediately after the Topic Name
    // when qos == 0 (there is no Packet Identifier to come after). This is the
    // same placement the library's inbound parser (task 7.1) and outbound
    // assembler use.
    //
    // `props` is taken as an MqttProps so an adversarial fixture (duplicated
    // single-occurrence identifier, an identifier illegal for PUBLISH, a
    // broken Property Length) is expressible; nothing here validates it.
    static MqttPacket publish5(const std::string& topic,
                               const std::vector<uint8_t>& payload,
                               uint8_t qos = 0,
                               bool retained = false,
                               bool dup = false,
                               uint16_t packetId = 0,
                               const MqttProps& props = MqttProps());

    // PUBACK: 0x40 0x02 <msgId hi> <msgId lo>.
    static MqttPacket puback(uint16_t msgId);

    // SUBACK: 0x90 0x03 <msgId hi> <msgId lo> <returnCode>.
    static MqttPacket suback(uint16_t msgId, uint8_t code);

    // UNSUBACK: 0xB0 0x02 <msgId hi> <msgId lo>.
    static MqttPacket unsuback(uint16_t msgId);

    // PINGREQ: 0xC0 0x00.
    static MqttPacket pingreq();

    // PINGRESP: 0xD0 0x00.
    static MqttPacket pingresp();

    // DISCONNECT: 0xE0 0x00.
    static MqttPacket disconnect();

    // PUBACK / PUBREC / PUBREL / PUBCOMP, MQTT 5.0 shape (task 9.5). One
    // builder for all four acknowledgement types, mirroring the library's own
    // single assembler (PubSubClient::sendAcknowledgement) and the parser's
    // three accepted forms (task 9.1). The form is selected purely from
    // `reasonCode` and `props`:
    //
    //   reasonCode <  0                   -> short form, Remaining Length 2:
    //                                         <type> 02 <id hi> <id lo>
    //   reasonCode >= 0 && props.empty()  -> mid form, Remaining Length 3:
    //                                         <type> 03 <id hi> <id lo> <rc>
    //                                         (no Properties field at all)
    //   reasonCode >= 0 && !props.empty() -> long form, Remaining Length 3+N:
    //                                         <type> <RL> <id hi> <id lo> <rc> <props>
    //
    // `packetType` is the fixed-header type byte (MQTTPUBACK .. MQTTPUBCOMP);
    // PUBREL's mandatory reserved-bit flags (0x02) are added automatically, so
    // a caller never has to remember to OR them in.
    //
    // `props` is taken as an MqttProps so an adversarial fixture (a broken
    // Property Length, an identifier illegal for the packet type) is
    // expressible; nothing here validates it.
    static MqttPacket ack5(uint8_t packetType,
                          uint16_t packetId,
                          int reasonCode,
                          const MqttProps& props = MqttProps());

    // SUBACK, MQTT 5.0 shape (task 9.5):
    //   variable header = PacketIdentifier(2) | Properties
    //   payload         = one Reason Code byte per requested Topic Filter
    // The Properties field is ALWAYS present in 5.0, exactly as connack5()
    // and publish5() above - an empty `props` still emits the single byte
    // 0x00. `reasonCodes` is written verbatim and unvalidated, so an empty
    // list or a count that disagrees with the original SUBSCRIBE's filter
    // count is expressible for the adversarial corpus.
    static MqttPacket suback5(uint16_t packetId,
                             const std::vector<uint8_t>& reasonCodes,
                             const MqttProps& props = MqttProps());

    // UNSUBACK, MQTT 5.0 shape (task 9.5): identical layout to suback5()
    // above - PacketIdentifier, Properties, then one Reason Code byte per
    // filter. 3.1.1's UNSUBACK (unsuback() above) has neither a Properties
    // field nor a Reason Code list; this is the 5.0-only shape that gained
    // both.
    static MqttPacket unsuback5(uint16_t packetId,
                               const std::vector<uint8_t>& reasonCodes,
                               const MqttProps& props = MqttProps());

    // DISCONNECT, MQTT 5.0 shape (task 9.5). Mirrors the three forms the
    // library's own parser accepts (PubSubClient::parseServerDisconnect5):
    //
    //   reasonCode <  0                   -> short form, Remaining Length 0:
    //                                         E0 00 (no body at all)
    //   reasonCode >= 0 && props.empty()  -> Reason Code only, Remaining
    //                                         Length 1: E0 01 <rc>
    //   reasonCode >= 0 && !props.empty() -> long form, Remaining Length
    //                                         1+N: E0 <RL> <rc> <props>
    //
    // The short form carries no body at all, not even a Reason Code, which is
    // why `reasonCode < 0` (rather than `props.empty()`, as ack5() above
    // uses) is what selects it.
    static MqttPacket disconnect5(int reasonCode,
                                 const MqttProps& props = MqttProps());

    // AUTH, MQTT 5.0 shape (task 9.5). Enhanced Authentication is a non-goal
    // in full, so this client never sends or expects one; the builder exists
    // only to script packet type 15 arriving so the rejection it must trigger
    // (Requirement 16.4) can be tested. Unlike disconnect5() there is no
    // short form: a Reason Code and the (always-present) Properties field are
    // both written every time.
    static MqttPacket auth(uint8_t reasonCode,
                          const MqttProps& props = MqttProps());

    // Arbitrary bytes verbatim, for adversarial / malformed fixtures
    // (F-01/F-02/F-06/F-09 etc.). No framing is added or validated.
    static MqttPacket raw(const std::vector<uint8_t>& bytes);

    // --- Accessors ---------------------------------------------------------

    const std::vector<uint8_t>& bytes() const { return _bytes; }
    size_t size() const { return _bytes.size(); }

    // --- Remaining Length codec (mirrors PubSubClient::buildHeader) ---------

    // Encode an MQTT Remaining Length into 1..4 bytes. The loop is bounded to
    // four bytes exactly as the library's buildHeader is, so a value outside the
    // 0..268435455 range is clamped to a 4-byte encoding rather than emitting a
    // fifth byte.
    static std::vector<uint8_t> encodeRemainingLength(uint32_t length);

    // --- Independent Variable Byte Integer mirror (task 2.2) ----------------

    // Encode `value` as an MQTT 5.0 Variable Byte Integer under the NARROWED
    // 2-byte contract the library now implements: 1 byte for 0..127, 2 bytes for
    // 128..16383, and an EMPTY vector for anything above 16383 (refusal is total,
    // never a truncated or wrapped encoding).
    //
    // This is written from the MQTT specification's own arithmetic formulation
    // (repeated divide-by-128 on an explicit digit list) rather than by calling
    // PubSubClient::encodeVariableByteInteger or reusing encodeRemainingLength
    // above, so a round-trip test compares TWO INDEPENDENT IMPLEMENTATIONS
    // instead of checking one function against itself (Requirements 15.4, 15.5).
    //
    // NOTE: deliberately separate from encodeRemainingLength(), which stays
    // unrestricted at 4 bytes because the harness must still be able to BUILD
    // oversized inbound fixtures (declared Remaining Lengths of 300000, 5000000,
    // 200000000) that the library is then required to reject.
    static std::vector<uint8_t> encodeVbi(uint32_t value);

private:
    explicit MqttPacket(std::vector<uint8_t> bytes) : _bytes(std::move(bytes)) {}

    // Build a fixed header (packet type + Remaining Length) followed by the
    // given variable-header-plus-payload body.
    static MqttPacket framed(uint8_t fixedHeader, const std::vector<uint8_t>& body);

    // Append a 2-byte big-endian length-prefixed UTF-8 string, exactly as the
    // library's writeString lays it out on the wire.
    static void appendString(std::vector<uint8_t>& out, const std::string& s);

    std::vector<uint8_t> _bytes;
};

// ===========================================================================
//  MqttParser - structural decoder + validators (task 7.2)
// ===========================================================================

// Output of the generic decoder. `type` is the fixed-header high nibble (e.g.
// MQTTPUBLISH), `flags` the low nibble (dup/qos/retain), `remainingLength` the
// decoded 1..4 byte value. Because splitting a variable header from the payload
// is packet-type specific, the generic decode places the ENTIRE decoded body
// (everything after the fixed header + Remaining Length field) into `payload`
// and leaves `variableHeader` empty; the type-specific decoders below carve out
// the individual fields. `valid` is true only when the framing is
// self-consistent: a Remaining Length that decodes within 1..4 bytes and whose
// value exactly equals the number of trailing bytes actually present.
struct DecodedPacket {
    uint8_t  type = 0;               // high nibble, e.g. MQTTPUBLISH
    uint8_t  flags = 0;              // low nibble (dup/qos/retain)
    uint32_t remainingLength = 0;    // decoded 1..4 byte value
    std::vector<uint8_t> variableHeader;
    std::vector<uint8_t> payload;
    bool     valid = false;          // framing self-consistent
};

// Decoded CONNECT fields (MQTT 3.1.1 variable header + payload).
struct DecodedConnect {
    bool        valid = false;
    std::string protocolName;        // "MQTT" for 3.1.1
    uint8_t     protocolLevel = 0;   // 4 for 3.1.1
    uint8_t     connectFlags = 0;
    uint16_t    keepAlive = 0;
    bool        cleanSession = false;
    bool        willFlag = false;
    uint8_t     willQos = 0;
    bool        willRetain = false;
    bool        userFlag = false;
    bool        passwordFlag = false;
    std::string clientId;
    std::string willTopic;
    std::string willMessage;
    std::string username;
    std::string password;
};

// Decoded PUBLISH fields.
struct DecodedPublish {
    bool        valid = false;
    bool        dup = false;
    uint8_t     qos = 0;
    bool        retain = false;
    std::string topic;
    uint16_t    msgId = 0;           // 0 when qos == 0 (no packet identifier)
    std::vector<uint8_t> payload;
};

// Decoded SUBSCRIBE fields. `filters[i]` carries requested QoS `requestedQos[i]`.
struct DecodedSubscribe {
    bool        valid = false;
    uint16_t    msgId = 0;
    std::vector<std::string> filters;
    std::vector<uint8_t>     requestedQos;
};

// Decoded UNSUBSCRIBE fields.
struct DecodedUnsubscribe {
    bool        valid = false;
    uint16_t    msgId = 0;
    std::vector<std::string> filters;
};

// --- MQTT 5.0 Properties decoding (task 4.4) -------------------------------

// One decoded property entry. Which member carries the value depends on the
// identifier's type:
//   Byte / Two Byte / Four Byte / Variable Byte Integer -> `value`
//   UTF-8 string                                        -> `s`
//   Binary Data                                         -> `bin`
//   UTF-8 string pair (0x26 User Property)              -> `s` (key), `s2` (value)
struct DecodedProperty {
    uint8_t              id = 0;
    uint32_t             value = 0;
    std::string          s;
    std::string          s2;
    std::vector<uint8_t> bin;
};

// A decoded Properties field. `declaredLength` is the value read from the
// Property Length prefix; `totalBytes` is the prefix byte count plus
// `declaredLength`, i.e. how far the field extends from its start offset.
// `entries` are in wire order, duplicates included, so a test can assert both
// the multiplicity and the ordering of repeatable identifiers.
//
// `valid` is a purely STRUCTURAL verdict: the length prefix decodes and is
// minimally encoded, the declared body fits inside the supplied bytes, and every
// entry's identifier is known and its value lies entirely inside the declared
// body. Identifier legality per packet type and single-occurrence rules are NOT
// checked here - those are the library's job, and the harness must stay able to
// build packets that violate them.
struct DecodedProperties {
    bool                         valid = false;
    uint32_t                     declaredLength = 0;
    size_t                       totalBytes = 0;
    std::vector<DecodedProperty> entries;
};

// --- MQTT 5.0 acknowledgement / subscribe / disconnect decoding (task 9.5) -

// Decoded PUBACK/PUBREC/PUBREL/PUBCOMP fields (task 9.5).
//
// `form` names which of the three wire forms was actually decoded, so a test
// can assert the SHAPE the peer chose rather than inferring it back out of
// `props.valid` and a reason code that defaults to 0 either way:
//   0 = short form   (Remaining Length 2: id only, Reason Code implied 0x00)
//   1 = mid form      (Remaining Length 3: id + Reason Code, no Properties)
//   2 = long form      (Remaining Length > 3: id + Reason Code + Properties)
// `reasonCode` reads back 0x00 for the short form, exactly as the library's
// own parser (parseAcknowledgement5) defaults it. `props` is left at its
// default (valid == false) for the short and mid forms, which is how a test
// distinguishes "no Properties field at all" from "present but empty".
struct DecodedAck5 {
    bool              valid = false;
    int               form = -1;        // 0 short, 1 mid, 2 long
    uint16_t          packetId = 0;
    uint8_t           reasonCode = 0;   // 0x00 for the short form
    DecodedProperties props;
};

// Decoded MQTT 5.0 SUBSCRIBE fields (task 9.5).
//
// Field order: PacketIdentifier, Properties, then one (Topic Filter,
// Subscription Options byte) pair per requested filter - the same layout
// PubSubClient::sendSubscription() writes. `options[i]` is the raw
// Subscription Options byte for `filters[i]`, left undecoded (bits 0-1
// Maximum QoS, bit 2 No Local, bit 3 Retain As Published, bits 4-5 Retain
// Handling) so a test can assert on whichever bits it cares about without
// this decoder imposing its own bit-field split.
struct DecodedSubscribe5 {
    bool                      valid = false;
    uint16_t                  msgId = 0;
    DecodedProperties         props;
    std::vector<std::string>  filters;
    std::vector<uint8_t>      options;
};

// Decoded SUBACK / UNSUBACK fields (task 9.5).
//
// Both packet types share this layout in MQTT 5.0: PacketIdentifier,
// Properties, then one Reason Code byte per requested Topic Filter - see
// PubSubClient::parseSubscriptionAck5(). One struct serves both; nothing here
// tells a SUBACK's Reason Codes apart from an UNSUBACK's, since the caller
// already knows which builder/type it decoded.
struct DecodedSuback5 {
    bool                  valid = false;
    uint16_t              msgId = 0;
    DecodedProperties     props;
    std::vector<uint8_t>  reasonCodes;
};

// Decoded MQTT 5.0 CONNECT fields (task 6.4).
//
// Derives from DecodedConnect rather than duplicating its fifteen members, so
// the 3.1.1 field set has exactly ONE definition: a field added to or fixed in
// DecodedConnect cannot silently diverge here, and a test reads `c.clientId`
// with the same spelling for both decoders. Public inheritance from a plain
// aggregate keeps the members flat (no `c.base.clientId` indirection) and
// DecodedConnect5 remains an aggregate in C++17.
//
// `props` is the variable-header Properties field (after Keep Alive);
// `willProps` is the SECOND Properties field, which sits in the PAYLOAD
// immediately before the Will Topic and is decoded only when the Will flag is
// set (Requirement 1.3). When the Will flag is clear, `willProps` is left at its
// default (valid == false, no entries), which is how a test distinguishes
// "absent" from "present but empty" - an empty Will Properties field still
// decodes as valid with declaredLength 0.
struct DecodedConnect5 : DecodedConnect {
    DecodedProperties props;
    DecodedProperties willProps;
};

// Decoded MQTT 5.0 PUBLISH fields (task 7.3).
//
// Derives from DecodedPublish rather than duplicating its six members, for the
// same reason DecodedConnect5 derives from DecodedConnect: the 3.1.1 field set
// stays defined exactly once, and a test reads `p.topic` / `p.qos` / `p.dup`
// with the same spelling for both decoders. `dup` already exists on the base
// struct (the DUP flag is part of the fixed header in both protocol versions),
// so it is not redeclared here.
//
// `props` is the Properties field that sits after the Packet Identifier (QoS >
// 0) or immediately after the Topic Name (QoS 0), before the payload - the
// only Properties field a PUBLISH carries. `topicAlias` mirrors the library's
// own convention (PubSubClient.h `MqttInboundProperties::topicAlias`): 0 means
// no Topic Alias property (0x23) was present, since 0 is not itself a legal
// Topic Alias value on the wire. `props.valid` still distinguishes "no
// Properties field at all" (never true for a well-formed 5.0 PUBLISH) from
// "present but empty" (topicAlias stays 0 either way).
struct DecodedPublish5 : DecodedPublish {
    DecodedProperties props;
    uint16_t          topicAlias = 0;   // 0 = no Topic Alias (0x23) property present
};

class MqttParser {
public:
    // Decode a single MQTT control packet from the front of `bytes`. The
    // returned DecodedPacket is `valid` only when the framing is
    // self-consistent (see DecodedPacket).
    static DecodedPacket decode(const std::vector<uint8_t>& bytes);

    // Type-specific decoders. Each first runs the generic decode, checks the
    // fixed-header nibble, then parses the body. The returned struct's `valid`
    // flag is false if the bytes are not a well-formed packet of that type.
    static DecodedConnect     decodeConnect(const std::vector<uint8_t>& bytes);
    static DecodedPublish     decodePublish(const std::vector<uint8_t>& bytes);
    static DecodedSubscribe   decodeSubscribe(const std::vector<uint8_t>& bytes);
    static DecodedUnsubscribe decodeUnsubscribe(const std::vector<uint8_t>& bytes);

    // --- MQTT 5.0 acknowledgement / subscribe / disconnect decoders (task 9.5) --

    // Decode a PUBACK/PUBREC/PUBREL/PUBCOMP built by any of the three forms
    // ack5() can produce. `expectedType` is the fixed-header high nibble the
    // caller expects (e.g. MQTTPUBACK); the low nibble (PUBREL's mandatory
    // 0x02) is not checked here, since it carries no information this
    // decoder needs. `valid` requires the Properties field, when present, to
    // decode structurally AND to consume every trailing byte - a long-form
    // acknowledgement's Properties field is the packet's last field, so
    // bytes left over after it mean some length was under-declared.
    //
    // Written without calling the library's own acknowledgement parser
    // (parseAcknowledgement5), so a round-trip test compares two independent
    // implementations (Requirements 15.4, 15.5).
    static DecodedAck5 decodeAck5(const std::vector<uint8_t>& bytes,
                                  uint8_t expectedType);

    // Decode an MQTT 5.0 SUBSCRIBE:
    //   variable header  PacketIdentifier(2) | Properties
    //   payload          (Topic Filter, Subscription Options byte)+
    // `valid` requires at least one filter and the walk to land exactly on
    // the packet's last byte, mirroring decodeSubscribe() above but with the
    // Properties field spliced in between the identifier and the first
    // filter (Requirements 9.1, 9.3).
    //
    // Written without calling the library's own SUBSCRIBE assembler
    // (sendSubscription), so a round-trip test compares two independent
    // implementations (Requirements 15.4, 15.5).
    static DecodedSubscribe5 decodeSubscribe5(const std::vector<uint8_t>& bytes);

    // Decode a SUBACK or an UNSUBACK built by suback5() / unsuback5().
    // `expectedType` is the fixed-header high nibble the caller expects
    // (MQTTSUBACK or MQTTUNSUBACK). `valid` requires at least one Reason
    // Code to follow the Properties field - a list of zero entries is
    // malformed, matching the library's own parser
    // (parseSubscriptionAck5), which requires the same.
    //
    // Written without calling the library's own parser, so a round-trip test
    // compares two independent implementations (Requirements 15.4, 15.5).
    static DecodedSuback5 decodeSuback5(const std::vector<uint8_t>& bytes,
                                        uint8_t expectedType);

    // --- MQTT 5.0 CONNECT decoder (task 6.4) --------------------------------

    // Decode an MQTT 5.0 CONNECT:
    //   variable header  00 04 'M' 'Q' 'T' 'T' | level | flags | keepalive(2)
    //                    | Properties
    //   payload          ClientId
    //                    [ WillProperties | WillTopic | WillPayload ]  (Will flag)
    //                    [ UserName ]                                  (User flag)
    //                    [ Password ]                                  (Password flag)
    //
    // `valid` requires every declared field to lie inside the packet AND the walk
    // to end exactly on the last byte: trailing bytes after the final field mean
    // some length was under-declared, which is a framing defect, not a packet
    // with a tail. Unlike decodeConnect (which mirrors the 3.1.1 library's
    // behavior of reading a Password only inside the User Name branch), the
    // Password field here is read whenever its flag is set, independent of the
    // User Name flag, which is what the specification prescribes.
    //
    // Written without calling the library's CONNECT writer or reader, so a
    // round-trip test compares two independent implementations (Req 15.4, 15.5).
    static DecodedConnect5    decodeConnect5(const std::vector<uint8_t>& bytes);

    // --- MQTT 5.0 PUBLISH decoder (task 7.3) --------------------------------

    // Decode an MQTT 5.0 PUBLISH:
    //   fixed header    MQTTPUBLISH | dup | qos | retain
    //   variable header Topic Name
    //                   [ Packet Identifier(2) ]   (qos > 0 only)
    //                   Properties
    //   payload         whatever remains
    //
    // `valid` requires the Properties field to decode structurally (see
    // DecodedProperties); the payload is simply everything after it, so an
    // empty payload is not itself an error. `dup`/`qos`/`retain` come straight
    // off the fixed header, exactly as decodePublish() above reads them.
    //
    // Written without calling the library's PUBLISH writer or reader, so a
    // round-trip test compares two independent implementations (Req 15.4, 15.5).
    static DecodedPublish5    decodePublish5(const std::vector<uint8_t>& bytes);

    // --- MQTT 5.0 Properties decoder (task 4.4) -----------------------------

    // Decode the Properties field that starts at `body[offset]`, i.e. the
    // Property Length prefix followed by that many body bytes. See
    // DecodedProperties for the exact meaning of `valid`.
    //
    // Written independently of PubSubClient::decodeProperties (its own
    // identifier-to-type table, its own walk), so a round-trip test compares two
    // implementations rather than one against itself (Requirements 15.4, 15.5).
    static DecodedProperties decodeProperties(const std::vector<uint8_t>& body,
                                              size_t offset);

    // Structural validator for PUBLISH: verifies the fixed-header high nibble is
    // MQTTPUBLISH, that the decoded Remaining Length equals the actual trailing
    // byte count, and that the 2-byte topic length fits within the body (leaving
    // room for the packet identifier when QoS > 0). Rejects truncated packets.
    static bool isStructurallyValidPublish(const std::vector<uint8_t>& bytes);

    // --- Generalized structural validator (task 4.4) ------------------------

    // Generalization of isStructurallyValidPublish above to EVERY packet type.
    // Two invariants are checked:
    //   1. the declared Remaining Length decodes and equals the actual number of
    //      trailing bytes present after the fixed header;
    //   2. every declared Properties length equals its actual body length, i.e.
    //      the Properties field ends exactly where the next field begins.
    //
    // Where the Properties field SITS is packet-type dependent, so the walk is
    // explicit per type (see the implementation's comment block). In the MQTT
    // 3.1.1 selection there is no Properties field on any packet, so only
    // invariant 1 applies and the properties walk is compiled out.
    //
    // This is a FRAMING check, not a conformance check: it does not judge
    // identifier legality, reason-code values, QoS semantics or string contents.
    static bool isStructurallyWellFramed(const std::vector<uint8_t>& bytes);

    // Decode a Remaining Length starting at `bytes[offset]`. Returns true on
    // success and sets `value` (decoded length) and `bytesConsumed` (1..4). A
    // value needing more than four bytes, or a truncated field, returns false.
    static bool decodeRemainingLength(const std::vector<uint8_t>& bytes,
                                      size_t offset,
                                      uint32_t& value,
                                      size_t& bytesConsumed);

    // --- Independent Variable Byte Integer mirror (task 2.2) ----------------

    // Decode a Variable Byte Integer starting at `bytes[off]` under the NARROWED
    // 2-byte contract: on success sets `value` (0..16383) and `used` (1..2) and
    // returns true. Returns false in exactly two cases, matching the library:
    //   * the field is truncated within the available bytes (the last available
    //     byte still has its continuation bit set, and no byte beyond the
    //     available range is ever read);
    //   * the 2nd byte still sets the continuation bit and thereby implies a
    //     3rd byte, e.g. {0x80, 0x80}.
    //
    // Independent of PubSubClient::decodeVariableByteInteger and of
    // decodeRemainingLength above (which stays a 4-byte decoder), so the
    // round-trip test cross-checks two implementations (Requirements 15.4, 15.5).
    static bool decodeVbi(const std::vector<uint8_t>& bytes,
                          size_t off,
                          uint32_t& value,
                          size_t& used);
};

#endif  // TASMOTA_PUBSUB_TEST_MQTT_PACKET_H
