/*
  MqttPacket.cpp - Implementation of the structural MQTT 3.1.1 control-packet
  builder (task 7.1).

  Every builder produces the exact bytes a conformant peer would put on the
  wire. The Remaining Length encoder mirrors PubSubClient::buildHeader so the
  fixtures frame packets identically to the library under test.
*/

#include "MqttPacket.h"

// --- Remaining Length codec (mirrors PubSubClient::buildHeader) -------------

std::vector<uint8_t> MqttPacket::encodeRemainingLength(uint32_t length) {
    std::vector<uint8_t> out;
    uint8_t llen = 0;
    uint32_t len = length;
    // Same loop shape as buildHeader: emit at least one byte, continuation bit
    // set while more remains, bounded to 4 bytes so the encoding can never run
    // past the MQTT maximum of four Remaining Length bytes.
    do {
        uint8_t digit = len & 127;   // digit = len % 128
        len >>= 7;                   // len = len / 128
        if (len > 0) {
            digit |= 0x80;           // continuation bit
        }
        out.push_back(digit);
        llen++;
    } while (len > 0 && llen < 4);
    return out;
}

// --- Independent Variable Byte Integer mirror (task 2.2) --------------------
//
// Deliberately NOT implemented in terms of encodeRemainingLength() above and NOT
// delegating to PubSubClient::encodeVariableByteInteger: the round-trip property
// test cross-checks the library against this mirror, so the two must be
// independent derivations. This one follows the MQTT specification's own
// arithmetic formulation - collect base-128 digits by repeated division, then set
// the continuation bit on every digit except the last - with the range check
// applied up front so an out-of-range value produces no bytes at all.
std::vector<uint8_t> MqttPacket::encodeVbi(uint32_t value) {
    static const uint32_t kVbiMax = 16383u;   // 0x3FFF, the 2-byte limit

    if (value > kVbiMax) {
        return std::vector<uint8_t>();   // refusal is total: nothing emitted
    }

    // Base-128 digits, least significant first.
    std::vector<uint8_t> digits;
    uint32_t remaining = value;
    do {
        digits.push_back(static_cast<uint8_t>(remaining % 128u));
        remaining /= 128u;
    } while (remaining != 0u);

    // Every digit but the last carries the continuation bit.
    std::vector<uint8_t> out(digits.size());
    for (size_t i = 0; i < digits.size(); ++i) {
        const bool last = (i + 1 == digits.size());
        out[i] = static_cast<uint8_t>(last ? digits[i] : (digits[i] | 0x80));
    }
    return out;
}

// ===========================================================================
//  MqttProps - MQTT 5.0 Properties-field fixture builder (task 4.4)
// ===========================================================================
//
// Each builder appends `<identifier><value>` to the body in call order; bytes()
// then prefixes the length. Nothing is validated (see the header): an identifier
// illegal for a packet type, a duplicated single-occurrence identifier and a
// wrong-width value are all expressible on purpose, because the adversarial
// corpus needs them and the library is the component required to reject them.

namespace {

// 2-byte big-endian length-prefixed UTF-8 string, the MQTT string layout.
void appendPropString(std::vector<uint8_t>& out, const std::string& s) {
    const uint16_t len = static_cast<uint16_t>(s.size());
    out.push_back(static_cast<uint8_t>(len >> 8));
    out.push_back(static_cast<uint8_t>(len & 0xFF));
    out.insert(out.end(), s.begin(), s.end());
}

// Binary Data has the same 2-byte big-endian length prefix as a string, but the
// bytes are arbitrary (0x00 included).
void appendPropBinary(std::vector<uint8_t>& out, const std::vector<uint8_t>& d) {
    const uint16_t len = static_cast<uint16_t>(d.size());
    out.push_back(static_cast<uint8_t>(len >> 8));
    out.push_back(static_cast<uint8_t>(len & 0xFF));
    out.insert(out.end(), d.begin(), d.end());
}

}  // namespace

MqttProps& MqttProps::byte_(uint8_t id, uint8_t v) {
    _body.push_back(id);
    _body.push_back(v);
    return *this;
}

MqttProps& MqttProps::u16(uint8_t id, uint16_t v) {
    _body.push_back(id);
    _body.push_back(static_cast<uint8_t>(v >> 8));
    _body.push_back(static_cast<uint8_t>(v & 0xFF));
    return *this;
}

MqttProps& MqttProps::u32(uint8_t id, uint32_t v) {
    _body.push_back(id);
    _body.push_back(static_cast<uint8_t>((v >> 24) & 0xFF));
    _body.push_back(static_cast<uint8_t>((v >> 16) & 0xFF));
    _body.push_back(static_cast<uint8_t>((v >> 8) & 0xFF));
    _body.push_back(static_cast<uint8_t>(v & 0xFF));
    return *this;
}

MqttProps& MqttProps::varint(uint8_t id, uint32_t v) {
    _body.push_back(id);
    // encodeRemainingLength (up to 4 bytes), NOT the 2-byte-limited encodeVbi:
    // the corpus must be able to declare a Subscription Identifier above 16,383
    // so the library's refusal path can be exercised.
    const std::vector<uint8_t> vbi = MqttPacket::encodeRemainingLength(v);
    _body.insert(_body.end(), vbi.begin(), vbi.end());
    return *this;
}

MqttProps& MqttProps::str(uint8_t id, const std::string& s) {
    _body.push_back(id);
    appendPropString(_body, s);
    return *this;
}

MqttProps& MqttProps::bin(uint8_t id, const std::vector<uint8_t>& d) {
    _body.push_back(id);
    appendPropBinary(_body, d);
    return *this;
}

MqttProps& MqttProps::pair(uint8_t id, const std::string& k, const std::string& v) {
    _body.push_back(id);
    appendPropString(_body, k);
    appendPropString(_body, v);
    return *this;
}

MqttProps& MqttProps::rawLength(uint32_t declared) {
    _rawLength = true;
    _declaredLength = declared;
    return *this;
}

std::vector<uint8_t> MqttProps::bytes() const {
    const uint32_t declared =
        _rawLength ? _declaredLength : static_cast<uint32_t>(_body.size());

    // encodeRemainingLength again rather than encodeVbi: for any declared length
    // inside the 2-byte range the two produce identical bytes, but only this one
    // can express the deliberately over-long declarations the corpus needs.
    std::vector<uint8_t> out = MqttPacket::encodeRemainingLength(declared);
    out.insert(out.end(), _body.begin(), _body.end());
    return out;
}

// --- Internal helpers -------------------------------------------------------

void MqttPacket::appendString(std::vector<uint8_t>& out, const std::string& s) {
    const uint16_t len = static_cast<uint16_t>(s.size());
    out.push_back(static_cast<uint8_t>(len >> 8));
    out.push_back(static_cast<uint8_t>(len & 0xFF));
    out.insert(out.end(), s.begin(), s.end());
}

MqttPacket MqttPacket::framed(uint8_t fixedHeader, const std::vector<uint8_t>& body) {
    std::vector<uint8_t> bytes;
    bytes.push_back(fixedHeader);
    const std::vector<uint8_t> rl = encodeRemainingLength(static_cast<uint32_t>(body.size()));
    bytes.insert(bytes.end(), rl.begin(), rl.end());
    bytes.insert(bytes.end(), body.begin(), body.end());
    return MqttPacket(std::move(bytes));
}

// --- Builders ---------------------------------------------------------------

// "The CONNACK the library under test accepts", which is a per-selection notion: in
// MQTT 5.0 a CONNACK always carries a Properties field, so this delegates to connack5()
// there (task 6.2). That keeps every existing fixture that scripts a successful connect
// - including the frozen baseline and hardening sources, which must not be edited -
// speaking the wire format of the selection it is compiled into.
//
// A test that specifically wants the 3.1.1 shape in a 5.0 build must build it by hand;
// this builder tracks the selection on purpose.
MqttPacket MqttPacket::connack(uint8_t returnCode, bool sessionPresent) {
#if MQTT_VERSION == MQTT_VERSION_5_0
    return connack5(returnCode, sessionPresent);
#else
    std::vector<uint8_t> body;
    body.push_back(sessionPresent ? 0x01 : 0x00);   // connack acknowledge flags
    body.push_back(returnCode);
    return framed(MQTTCONNACK, body);
#endif
}

// MQTT 5.0 CONNACK (task 6.4): ack flags, reason code, then the Properties
// field, which is never optional in 5.0 - an empty set still puts 0x00 on the
// wire, so the minimum Remaining Length is 3. Nothing about `props` is checked:
// the adversarial corpus needs to be able to declare a broken Property Length
// here, and the library is the component required to reject it.
MqttPacket MqttPacket::connack5(uint8_t reasonCode,
                                bool sessionPresent,
                                const MqttProps& props) {
    std::vector<uint8_t> body;
    body.push_back(sessionPresent ? 0x01 : 0x00);   // AckFlags, bit0 = Session Present
    body.push_back(reasonCode);
    const std::vector<uint8_t> p = props.bytes();
    body.insert(body.end(), p.begin(), p.end());
    return framed(MQTTCONNACK, body);
}

// "The PUBLISH the library under test accepts", which - exactly like connack() above -
// is a per-selection notion: in MQTT 5.0 a PUBLISH always carries a Properties field
// between the variable header and the payload, so this builder emits the empty field
// (the single byte 0x00) there. That keeps every existing fixture that scripts an
// inbound publish - including the frozen baseline and hardening sources, which must not
// be edited - speaking the wire format of the selection it is compiled into.
//
// A test that specifically wants the 3.1.1 shape in a 5.0 build must build it by hand;
// this builder tracks the selection on purpose. Task 7.3 adds publish5(), which takes an
// arbitrary MqttProps, and this builder becomes its empty-property-set special case.
MqttPacket MqttPacket::publish(const std::string& topic,
                               const std::vector<uint8_t>& payload,
                               uint8_t qos,
                               bool retained,
                               uint16_t msgId) {
    uint8_t header = static_cast<uint8_t>(MQTTPUBLISH)
                     | static_cast<uint8_t>((qos & 0x03) << 1)
                     | (retained ? 0x01 : 0x00);

    std::vector<uint8_t> body;
    appendString(body, topic);
    if (qos > 0) {
        // Packet identifier is present only for QoS 1 and QoS 2.
        body.push_back(static_cast<uint8_t>(msgId >> 8));
        body.push_back(static_cast<uint8_t>(msgId & 0xFF));
    }
#if MQTT_VERSION == MQTT_VERSION_5_0
    body.push_back(0x00);   // empty Properties field
#endif
    body.insert(body.end(), payload.begin(), payload.end());
    return framed(header, body);
}

MqttPacket MqttPacket::publish(const std::string& topic,
                               const std::string& payload,
                               uint8_t qos,
                               bool retained,
                               uint16_t msgId) {
    const std::vector<uint8_t> bytes(payload.begin(), payload.end());
    return publish(topic, bytes, qos, retained, msgId);
}

// MQTT 5.0 PUBLISH (task 7.3): topic, packet id (QoS > 0 only), THEN the
// Properties field, then the raw payload. This is the same field order the
// library's inbound parser (task 7.1) and outbound assembler use, and matches
// the empty-Properties special case already emitted by publish() above.
// `props` is not validated here (see MqttProps): the adversarial corpus needs
// to express illegal or malformed property sets, and the library is the
// component required to reject them.
MqttPacket MqttPacket::publish5(const std::string& topic,
                                const std::vector<uint8_t>& payload,
                                uint8_t qos,
                                bool retained,
                                bool dup,
                                uint16_t packetId,
                                const MqttProps& props) {
    uint8_t header = static_cast<uint8_t>(MQTTPUBLISH)
                     | (dup ? 0x08 : 0x00)
                     | static_cast<uint8_t>((qos & 0x03) << 1)
                     | (retained ? 0x01 : 0x00);

    std::vector<uint8_t> body;
    appendString(body, topic);
    if (qos > 0) {
        // Packet identifier is present only for QoS 1 and QoS 2.
        body.push_back(static_cast<uint8_t>(packetId >> 8));
        body.push_back(static_cast<uint8_t>(packetId & 0xFF));
    }
    const std::vector<uint8_t> p = props.bytes();
    body.insert(body.end(), p.begin(), p.end());
    body.insert(body.end(), payload.begin(), payload.end());
    return framed(header, body);
}

MqttPacket MqttPacket::puback(uint16_t msgId) {
    std::vector<uint8_t> body;
    body.push_back(static_cast<uint8_t>(msgId >> 8));
    body.push_back(static_cast<uint8_t>(msgId & 0xFF));
    return framed(MQTTPUBACK, body);
}

MqttPacket MqttPacket::suback(uint16_t msgId, uint8_t code) {
    std::vector<uint8_t> body;
    body.push_back(static_cast<uint8_t>(msgId >> 8));
    body.push_back(static_cast<uint8_t>(msgId & 0xFF));
    body.push_back(code);
    return framed(MQTTSUBACK, body);
}

MqttPacket MqttPacket::unsuback(uint16_t msgId) {
    std::vector<uint8_t> body;
    body.push_back(static_cast<uint8_t>(msgId >> 8));
    body.push_back(static_cast<uint8_t>(msgId & 0xFF));
    return framed(MQTTUNSUBACK, body);
}

MqttPacket MqttPacket::pingreq() {
    return framed(MQTTPINGREQ, {});
}

MqttPacket MqttPacket::pingresp() {
    return framed(MQTTPINGRESP, {});
}

MqttPacket MqttPacket::disconnect() {
    return framed(MQTTDISCONNECT, {});
}

// MQTT 5.0 acknowledgement (task 9.5): one builder for PUBACK/PUBREC/PUBREL/
// PUBCOMP, mirroring PubSubClient::sendAcknowledgement's three accepted
// forms. `reasonCode < 0` is the sentinel for the short form - a reason code
// cannot be represented as a negative byte on the wire, so it doubles as
// "omit the Reason Code (and therefore the Properties field) entirely".
// PUBREL's mandatory reserved-bit flags (0x02) are ORed in automatically so a
// caller never has to remember them; the other three types carry flags 0.
MqttPacket MqttPacket::ack5(uint8_t packetType,
                           uint16_t packetId,
                           int reasonCode,
                           const MqttProps& props) {
    std::vector<uint8_t> body;
    body.push_back(static_cast<uint8_t>(packetId >> 8));
    body.push_back(static_cast<uint8_t>(packetId & 0xFF));
    if (reasonCode >= 0) {
        body.push_back(static_cast<uint8_t>(reasonCode));
        if (!props.empty()) {
            // Long form only: the mid form (Remaining Length 3) has no
            // Properties field at all, not even an empty one - unlike
            // CONNACK/PUBLISH, where the field is always present in 5.0.
            const std::vector<uint8_t> p = props.bytes();
            body.insert(body.end(), p.begin(), p.end());
        }
    }
    uint8_t header = packetType;
    if (packetType == static_cast<uint8_t>(MQTTPUBREL)) {
        header |= 0x02;   // mandatory reserved-bit flags
    }
    return framed(header, body);
}

// MQTT 5.0 SUBACK (task 9.5): PacketIdentifier, THEN the Properties field
// (always present in 5.0, exactly as connack5()/publish5() above), then the
// raw Reason Code list. `reasonCodes` is written verbatim and unvalidated,
// so an adversarial fixture can supply an empty list or a count that
// disagrees with the SUBSCRIBE it is nominally answering; the library is the
// component required to reject those.
MqttPacket MqttPacket::suback5(uint16_t packetId,
                              const std::vector<uint8_t>& reasonCodes,
                              const MqttProps& props) {
    std::vector<uint8_t> body;
    body.push_back(static_cast<uint8_t>(packetId >> 8));
    body.push_back(static_cast<uint8_t>(packetId & 0xFF));
    const std::vector<uint8_t> p = props.bytes();
    body.insert(body.end(), p.begin(), p.end());
    body.insert(body.end(), reasonCodes.begin(), reasonCodes.end());
    return framed(MQTTSUBACK, body);
}

// MQTT 5.0 UNSUBACK (task 9.5): identical layout to suback5() above. 3.1.1's
// UNSUBACK (unsuback() above) has neither a Properties field nor a Reason
// Code list; this is the 5.0-only shape that gained both.
MqttPacket MqttPacket::unsuback5(uint16_t packetId,
                                const std::vector<uint8_t>& reasonCodes,
                                const MqttProps& props) {
    std::vector<uint8_t> body;
    body.push_back(static_cast<uint8_t>(packetId >> 8));
    body.push_back(static_cast<uint8_t>(packetId & 0xFF));
    const std::vector<uint8_t> p = props.bytes();
    body.insert(body.end(), p.begin(), p.end());
    body.insert(body.end(), reasonCodes.begin(), reasonCodes.end());
    return framed(MQTTUNSUBACK, body);
}

// MQTT 5.0 DISCONNECT (task 9.5): mirrors the three forms
// PubSubClient::parseServerDisconnect5 accepts. `reasonCode < 0` selects the
// short form - Remaining Length 0, no body at all, not even a Reason Code -
// which is why the sentinel is on `reasonCode` here rather than on
// `props.empty()` as ack5() above uses: the short form has no room for a
// Reason Code byte, let alone a Properties field.
MqttPacket MqttPacket::disconnect5(int reasonCode, const MqttProps& props) {
    std::vector<uint8_t> body;
    if (reasonCode >= 0) {
        body.push_back(static_cast<uint8_t>(reasonCode));
        if (!props.empty()) {
            // As with ack5(): the mid form (Reason Code only) has no
            // Properties field at all.
            const std::vector<uint8_t> p = props.bytes();
            body.insert(body.end(), p.begin(), p.end());
        }
    }
    return framed(MQTTDISCONNECT, body);
}

// MQTT 5.0 AUTH (task 9.5): Reason Code then the (always-present) Properties
// field. Enhanced Authentication is a non-goal in full - this client never
// sends or expects an AUTH - so the only use for this builder is scripting
// one arriving inbound, to exercise the rejection Requirement 16.4 requires.
// Unlike disconnect5() there is no short form: both fields are always
// written.
MqttPacket MqttPacket::auth(uint8_t reasonCode, const MqttProps& props) {
    std::vector<uint8_t> body;
    body.push_back(reasonCode);
    const std::vector<uint8_t> p = props.bytes();
    body.insert(body.end(), p.begin(), p.end());
    return framed(static_cast<uint8_t>(15u << 4), body);
}

MqttPacket MqttPacket::raw(const std::vector<uint8_t>& bytes) {
    return MqttPacket(bytes);
}

// ===========================================================================
//  MqttParser - structural decoder + validators (task 7.2)
// ===========================================================================

bool MqttParser::decodeRemainingLength(const std::vector<uint8_t>& bytes,
                                       size_t offset,
                                       uint32_t& value,
                                       size_t& bytesConsumed) {
    // Mirror of the wire codec: up to four continuation-flagged bytes, low 7
    // bits each, little-endian by septet. A fifth continuation byte is illegal.
    uint32_t multiplier = 1;
    uint32_t result = 0;
    size_t i = 0;
    for (; i < 4; ++i) {
        if (offset + i >= bytes.size()) {
            return false;   // truncated Remaining Length field
        }
        const uint8_t digit = bytes[offset + i];
        result += static_cast<uint32_t>(digit & 0x7F) * multiplier;
        multiplier <<= 7;
        if ((digit & 0x80) == 0) {
            value = result;
            bytesConsumed = i + 1;
            return true;
        }
    }
    return false;   // continuation bit still set after four bytes
}

// --- Independent Variable Byte Integer mirror (task 2.2) --------------------
//
// Straight-line 2-byte decoder, written independently of both
// decodeRemainingLength() above (a 4-byte multiplier loop) and
// PubSubClient::decodeVariableByteInteger, so the round-trip property test
// compares two implementations rather than one against itself.
//
// Failure modes, mirroring the library exactly:
//   * truncation within the available bytes - the last available byte still has
//     its continuation bit set. No byte beyond `bytes.size()` is ever examined.
//   * a 3rd byte is implied - the 2nd byte still sets the continuation bit.
//   * a non-minimal 2-byte representation encodes a value below 128.
bool MqttParser::decodeVbi(const std::vector<uint8_t>& bytes,
                           size_t off,
                           uint32_t& value,
                           size_t& used) {
    value = 0;
    used = 0;

    if (off >= bytes.size()) {
        return false;   // nothing available at all
    }

    const uint8_t b0 = bytes[off];
    if ((b0 & 0x80) == 0) {
        value = static_cast<uint32_t>(b0);   // 0..127, single byte
        used = 1;
        return true;
    }

    // A 2nd byte is required; refuse rather than read past the available bytes.
    if (off + 1 >= bytes.size()) {
        return false;   // truncated field
    }

    const uint8_t b1 = bytes[off + 1];
    if ((b1 & 0x80) != 0) {
        return false;   // 2nd byte implies a 3rd: outside the 2-byte contract
    }
    if ((b1 & 0x7F) == 0) {
        return false;   // values below 128 have a mandatory one-byte form
    }

    value = static_cast<uint32_t>(b0 & 0x7F)
            + (static_cast<uint32_t>(b1 & 0x7F) << 7);
    used = 2;
    return true;
}

DecodedPacket MqttParser::decode(const std::vector<uint8_t>& bytes) {
    DecodedPacket p;
    if (bytes.empty()) {
        return p;   // valid == false
    }

    p.type = static_cast<uint8_t>(bytes[0] & 0xF0);
    p.flags = static_cast<uint8_t>(bytes[0] & 0x0F);

    uint32_t remaining = 0;
    size_t rlBytes = 0;
    if (!decodeRemainingLength(bytes, 1, remaining, rlBytes)) {
        return p;   // malformed Remaining Length field
    }
    p.remainingLength = remaining;

    const size_t bodyStart = 1 + rlBytes;
    const size_t trailing = bytes.size() - bodyStart;

    // Framing is self-consistent only when the declared Remaining Length equals
    // the number of trailing bytes actually present.
    if (remaining != trailing) {
        return p;   // valid == false: truncated or over-long
    }

    p.payload.assign(bytes.begin() + static_cast<std::ptrdiff_t>(bodyStart), bytes.end());
    p.valid = true;
    return p;
}

// Read a 2-byte big-endian length-prefixed string from `body` at `pos`.
// Advances `pos` past the string. Returns false if it would run past the body.
static bool readString(const std::vector<uint8_t>& body, size_t& pos, std::string& out) {
    if (pos + 2 > body.size()) {
        return false;
    }
    const uint16_t len = static_cast<uint16_t>((body[pos] << 8) | body[pos + 1]);
    pos += 2;
    if (pos + len > body.size()) {
        return false;
    }
    out.assign(body.begin() + static_cast<std::ptrdiff_t>(pos),
               body.begin() + static_cast<std::ptrdiff_t>(pos + len));
    pos += len;
    return true;
}

DecodedConnect MqttParser::decodeConnect(const std::vector<uint8_t>& bytes) {
    DecodedConnect c;
    const DecodedPacket p = decode(bytes);
    if (!p.valid || p.type != static_cast<uint8_t>(MQTTCONNECT)) {
        return c;
    }

    const std::vector<uint8_t>& body = p.payload;
    size_t pos = 0;

    // Variable header: protocol name, level, connect flags, keep alive.
    if (!readString(body, pos, c.protocolName)) {
        return c;
    }
    if (pos + 4 > body.size()) {   // level(1) + flags(1) + keepAlive(2)
        return c;
    }
    c.protocolLevel = body[pos++];
    c.connectFlags = body[pos++];
    c.keepAlive = static_cast<uint16_t>((body[pos] << 8) | body[pos + 1]);
    pos += 2;

    c.cleanSession = (c.connectFlags & 0x02) != 0;
    c.willFlag = (c.connectFlags & 0x04) != 0;
    c.willQos = static_cast<uint8_t>((c.connectFlags >> 3) & 0x03);
    c.willRetain = (c.connectFlags & 0x20) != 0;
    c.userFlag = (c.connectFlags & 0x80) != 0;
    c.passwordFlag = (c.connectFlags & 0x40) != 0;

    // Payload: client id, [will topic, will message], [username], [password].
    if (!readString(body, pos, c.clientId)) {
        return c;
    }
    if (c.willFlag) {
        if (!readString(body, pos, c.willTopic) || !readString(body, pos, c.willMessage)) {
            return c;
        }
    }
    if (c.userFlag) {
        if (!readString(body, pos, c.username)) {
            return c;
        }
        if (c.passwordFlag && !readString(body, pos, c.password)) {
            return c;
        }
    }

    c.valid = true;
    return c;
}

// ---------------------------------------------------------------------------
//  MQTT 5.0 CONNECT decoder (task 6.4)
// ---------------------------------------------------------------------------
//
// The one thing that is easy to get wrong here is WHERE the two Properties
// fields sit, so both are decoded explicitly and in place:
//
//   variable header : "MQTT" | level | flags | keepalive(2) | Properties   <-- #1
//   payload         : ClientId
//                     [ WillProperties <-- #2 | WillTopic | WillPayload ]
//                     [ UserName ] [ Password ]
//
// The Will Properties field is part of the PAYLOAD and comes immediately BEFORE
// the Will Topic (Requirement 1.3), not in the variable header and not after the
// topic. Getting that placement wrong shifts every subsequent field, which is
// exactly why it is decoded as its own step rather than folded into the Will
// branch's string reads.
//
// The decoder is version-neutral harness code: it always reads the 5.0 shape,
// whatever the compile-time selection is, so it can be used to assert what the
// library emitted in the 5.0 selection and to assert that a 3.1.1 CONNECT is
// NOT of this shape.
DecodedConnect5 MqttParser::decodeConnect5(const std::vector<uint8_t>& bytes) {
    DecodedConnect5 c;
    const DecodedPacket p = decode(bytes);
    if (!p.valid || p.type != static_cast<uint8_t>(MQTTCONNECT)) {
        return c;
    }

    const std::vector<uint8_t>& body = p.payload;
    size_t pos = 0;

    // --- Variable header ----------------------------------------------------
    if (!readString(body, pos, c.protocolName)) {
        return c;
    }
    if (pos + 4 > body.size()) {   // level(1) + flags(1) + keepAlive(2)
        return c;
    }
    c.protocolLevel = body[pos++];
    c.connectFlags = body[pos++];
    c.keepAlive = static_cast<uint16_t>((body[pos] << 8) | body[pos + 1]);
    pos += 2;

    // Bit layout is unchanged from 3.1.1; bit 1 is named Clean Start in 5.0.
    c.cleanSession = (c.connectFlags & 0x02) != 0;
    c.willFlag = (c.connectFlags & 0x04) != 0;
    c.willQos = static_cast<uint8_t>((c.connectFlags >> 3) & 0x03);
    c.willRetain = (c.connectFlags & 0x20) != 0;
    c.userFlag = (c.connectFlags & 0x80) != 0;
    c.passwordFlag = (c.connectFlags & 0x40) != 0;

    // Properties #1: last field of the variable header, after Keep Alive.
    c.props = decodeProperties(body, pos);
    if (!c.props.valid) {
        return c;
    }
    pos += c.props.totalBytes;

    // --- Payload ------------------------------------------------------------
    if (!readString(body, pos, c.clientId)) {
        return c;
    }
    if (c.willFlag) {
        // Properties #2: the Will Properties, in the payload and BEFORE the Will
        // Topic. Left at its default (valid == false) when the Will flag is
        // clear, so "absent" stays distinguishable from "present but empty".
        c.willProps = decodeProperties(body, pos);
        if (!c.willProps.valid) {
            return c;
        }
        pos += c.willProps.totalBytes;
        if (!readString(body, pos, c.willTopic) || !readString(body, pos, c.willMessage)) {
            return c;
        }
    }
    // Each credential field is governed by its own flag, independently: 5.0
    // allows a Password with no User Name.
    if (c.userFlag && !readString(body, pos, c.username)) {
        return c;
    }
    if (c.passwordFlag && !readString(body, pos, c.password)) {
        return c;
    }

    // The walk must land exactly on the end of the packet; leftover bytes mean
    // some declared length was short.
    if (pos != body.size()) {
        return c;
    }

    c.valid = true;
    return c;
}

// ---------------------------------------------------------------------------
//  MQTT 5.0 PUBLISH decoder (task 7.3)
// ---------------------------------------------------------------------------
//
// Field order: Topic Name, Packet Identifier (QoS > 0 only), Properties, then
// the payload (whatever remains). The Properties field's decode is delegated
// to decodeProperties() below (Requirement 15.4, 15.5: written independently
// of the library's own reader), and `topicAlias` is pulled out of the decoded
// entries the same way the library's inboundPropertyCollector does - the last
// occurrence wins, though 0x23 is not a repeatable identifier so only one is
// ever legally present.
DecodedPublish5 MqttParser::decodePublish5(const std::vector<uint8_t>& bytes) {
    DecodedPublish5 r;
    const DecodedPacket p = decode(bytes);
    if (!p.valid || p.type != static_cast<uint8_t>(MQTTPUBLISH)) {
        return r;
    }

    r.dup = (p.flags & 0x08) != 0;
    r.qos = static_cast<uint8_t>((p.flags >> 1) & 0x03);
    r.retain = (p.flags & 0x01) != 0;

    const std::vector<uint8_t>& body = p.payload;
    size_t pos = 0;
    if (!readString(body, pos, r.topic)) {
        return r;
    }
    if (r.qos > 0) {
        // Packet identifier is present only for QoS 1 and QoS 2.
        if (pos + 2 > body.size()) {
            return r;
        }
        r.msgId = static_cast<uint16_t>((body[pos] << 8) | body[pos + 1]);
        pos += 2;
    }

    r.props = decodeProperties(body, pos);
    if (!r.props.valid) {
        return r;
    }
    pos += r.props.totalBytes;

    for (const DecodedProperty& e : r.props.entries) {
        if (e.id == 0x23) {   // Topic Alias
            r.topicAlias = static_cast<uint16_t>(e.value);
        }
    }

    r.payload.assign(body.begin() + static_cast<std::ptrdiff_t>(pos), body.end());
    r.valid = true;
    return r;
}

DecodedPublish MqttParser::decodePublish(const std::vector<uint8_t>& bytes) {
    DecodedPublish r;
    const DecodedPacket p = decode(bytes);
    if (!p.valid || p.type != static_cast<uint8_t>(MQTTPUBLISH)) {
        return r;
    }

    r.dup = (p.flags & 0x08) != 0;
    r.qos = static_cast<uint8_t>((p.flags >> 1) & 0x03);
    r.retain = (p.flags & 0x01) != 0;

    const std::vector<uint8_t>& body = p.payload;
    size_t pos = 0;
    if (!readString(body, pos, r.topic)) {
        return r;
    }
    if (r.qos > 0) {
        // Packet identifier is present only for QoS 1 and QoS 2.
        if (pos + 2 > body.size()) {
            return r;
        }
        r.msgId = static_cast<uint16_t>((body[pos] << 8) | body[pos + 1]);
        pos += 2;
    }
    r.payload.assign(body.begin() + static_cast<std::ptrdiff_t>(pos), body.end());
    r.valid = true;
    return r;
}

DecodedSubscribe MqttParser::decodeSubscribe(const std::vector<uint8_t>& bytes) {
    DecodedSubscribe s;
    const DecodedPacket p = decode(bytes);
    if (!p.valid || p.type != static_cast<uint8_t>(MQTTSUBSCRIBE)) {
        return s;
    }

    const std::vector<uint8_t>& body = p.payload;
    size_t pos = 0;
    if (pos + 2 > body.size()) {
        return s;
    }
    s.msgId = static_cast<uint16_t>((body[pos] << 8) | body[pos + 1]);
    pos += 2;

    // One or more (topic filter, requested QoS) pairs.
    while (pos < body.size()) {
        std::string filter;
        if (!readString(body, pos, filter)) {
            return s;   // valid == false: truncated filter
        }
        if (pos >= body.size()) {
            return s;   // missing requested-QoS byte
        }
        s.filters.push_back(filter);
        s.requestedQos.push_back(body[pos++]);
    }
    if (s.filters.empty()) {
        return s;   // a SUBSCRIBE must carry at least one filter
    }

    s.valid = true;
    return s;
}

DecodedUnsubscribe MqttParser::decodeUnsubscribe(const std::vector<uint8_t>& bytes) {
    DecodedUnsubscribe u;
    const DecodedPacket p = decode(bytes);
    if (!p.valid || p.type != static_cast<uint8_t>(MQTTUNSUBSCRIBE)) {
        return u;
    }

    const std::vector<uint8_t>& body = p.payload;
    size_t pos = 0;
    if (pos + 2 > body.size()) {
        return u;
    }
    u.msgId = static_cast<uint16_t>((body[pos] << 8) | body[pos + 1]);
    pos += 2;

    // One or more topic filters (no QoS byte in UNSUBSCRIBE).
    while (pos < body.size()) {
        std::string filter;
        if (!readString(body, pos, filter)) {
            return u;   // valid == false: truncated filter
        }
        u.filters.push_back(filter);
    }
    if (u.filters.empty()) {
        return u;   // an UNSUBSCRIBE must carry at least one filter
    }

    u.valid = true;
    return u;
}

// ---------------------------------------------------------------------------
//  MQTT 5.0 acknowledgement / subscribe / disconnect decoders (task 9.5)
// ---------------------------------------------------------------------------
//
// Written without calling the library's own parsers (parseAcknowledgement5,
// sendSubscription, parseSubscriptionAck5), so a round-trip test compares two
// independent implementations rather than one against itself
// (Requirements 15.4, 15.5).

// Decode a PUBACK/PUBREC/PUBREL/PUBCOMP built by any of ack5()'s three forms.
// Layout: PacketIdentifier(2) [ ReasonCode(1) [ Properties ] ].
DecodedAck5 MqttParser::decodeAck5(const std::vector<uint8_t>& bytes,
                                   uint8_t expectedType) {
    DecodedAck5 r;
    const DecodedPacket p = decode(bytes);
    if (!p.valid || p.type != expectedType) {
        return r;
    }

    const std::vector<uint8_t>& body = p.payload;
    if (body.size() < 2) {
        return r;   // Remaining Length below 2: no room for a packet id
    }
    r.packetId = static_cast<uint16_t>((body[0] << 8) | body[1]);

    if (body.size() == 2) {
        r.form = 0;          // short form: id only, Reason Code implied 0x00
        r.valid = true;
        return r;
    }

    r.reasonCode = body[2];
    if (body.size() == 3) {
        r.form = 1;          // mid form: id + Reason Code, no Properties
        r.valid = true;
        return r;
    }

    // Long form: the Properties field is the packet's last field, so it must
    // consume every remaining byte.
    r.props = decodeProperties(body, 3);
    if (!r.props.valid || (3 + r.props.totalBytes != body.size())) {
        return r;
    }
    r.form = 2;
    r.valid = true;
    return r;
}

// Decode an MQTT 5.0 SUBSCRIBE: PacketIdentifier, Properties, then one
// (Topic Filter, Subscription Options byte) pair per requested filter.
DecodedSubscribe5 MqttParser::decodeSubscribe5(const std::vector<uint8_t>& bytes) {
    DecodedSubscribe5 s;
    const DecodedPacket p = decode(bytes);
    if (!p.valid || p.type != static_cast<uint8_t>(MQTTSUBSCRIBE)) {
        return s;
    }

    const std::vector<uint8_t>& body = p.payload;
    size_t pos = 0;
    if (pos + 2 > body.size()) {
        return s;
    }
    s.msgId = static_cast<uint16_t>((body[pos] << 8) | body[pos + 1]);
    pos += 2;

    s.props = decodeProperties(body, pos);
    if (!s.props.valid) {
        return s;
    }
    pos += s.props.totalBytes;

    while (pos < body.size()) {
        std::string filter;
        if (!readString(body, pos, filter)) {
            return s;   // valid == false: truncated filter
        }
        if (pos >= body.size()) {
            return s;   // missing Subscription Options byte
        }
        s.filters.push_back(filter);
        s.options.push_back(body[pos++]);
    }
    if (s.filters.empty()) {
        return s;   // a SUBSCRIBE must carry at least one filter
    }

    s.valid = true;
    return s;
}

// Decode a SUBACK or an UNSUBACK built by suback5() / unsuback5():
// PacketIdentifier, Properties, then the raw Reason Code list (one byte per
// requested filter, at least one required).
DecodedSuback5 MqttParser::decodeSuback5(const std::vector<uint8_t>& bytes,
                                         uint8_t expectedType) {
    DecodedSuback5 r;
    const DecodedPacket p = decode(bytes);
    if (!p.valid || p.type != expectedType) {
        return r;
    }

    const std::vector<uint8_t>& body = p.payload;
    size_t pos = 0;
    if (pos + 2 > body.size()) {
        return r;
    }
    r.msgId = static_cast<uint16_t>((body[pos] << 8) | body[pos + 1]);
    pos += 2;

    r.props = decodeProperties(body, pos);
    if (!r.props.valid) {
        return r;
    }
    pos += r.props.totalBytes;

    if (pos >= body.size()) {
        return r;   // at least one Reason Code is required
    }
    r.reasonCodes.assign(body.begin() + static_cast<std::ptrdiff_t>(pos), body.end());

    r.valid = true;
    return r;
}

bool MqttParser::isStructurallyValidPublish(const std::vector<uint8_t>& bytes) {
    if (bytes.empty()) {
        return false;
    }
    // Fixed-header high nibble must be PUBLISH.
    if (static_cast<uint8_t>(bytes[0] & 0xF0) != static_cast<uint8_t>(MQTTPUBLISH)) {
        return false;
    }

    uint32_t remaining = 0;
    size_t rlBytes = 0;
    if (!decodeRemainingLength(bytes, 1, remaining, rlBytes)) {
        return false;
    }

    // Decoded Remaining Length must equal the actual trailing byte count.
    const size_t bodyStart = 1 + rlBytes;
    if (bodyStart > bytes.size()) {
        return false;
    }
    if (remaining != bytes.size() - bodyStart) {
        return false;
    }

    // The 2-byte topic length must fit within the body.
    if (remaining < 2) {
        return false;   // no room for the topic length prefix
    }
    const uint16_t topicLen =
        static_cast<uint16_t>((bytes[bodyStart] << 8) | bytes[bodyStart + 1]);

    // QoS > 0 reserves an extra 2-byte packet identifier after the topic.
    const uint8_t qos = static_cast<uint8_t>((bytes[0] >> 1) & 0x03);
    const size_t reserved = (qos > 0) ? 2u : 0u;

    // topic length prefix (2) + topic bytes + packet id (if any) must fit.
    if (static_cast<size_t>(2) + topicLen + reserved > remaining) {
        return false;
    }

    return true;
}

// ===========================================================================
//  MQTT 5.0 Properties decoder (task 4.4)
// ===========================================================================

namespace {

// Value type of a property identifier. The table below is the harness's OWN
// derivation from the MQTT 5.0 specification; it deliberately does not consult
// PubSubClient's PROPERTY_TABLE, so a round-trip test compares two independent
// implementations (Requirements 15.4, 15.5).
enum class PropKind : uint8_t {
    Unknown, Byte, TwoByte, FourByte, VarInt, Utf8, Binary, StringPair
};

PropKind propKind(uint8_t id) {
    switch (id) {
        case 0x01: return PropKind::Byte;        // Payload Format Indicator
        case 0x02: return PropKind::FourByte;    // Message Expiry Interval
        case 0x03: return PropKind::Utf8;        // Content Type
        case 0x08: return PropKind::Utf8;        // Response Topic
        case 0x09: return PropKind::Binary;      // Correlation Data
        case 0x0B: return PropKind::VarInt;      // Subscription Identifier
        case 0x11: return PropKind::FourByte;    // Session Expiry Interval
        case 0x12: return PropKind::Utf8;        // Assigned Client Identifier
        case 0x13: return PropKind::TwoByte;     // Server Keep Alive
        case 0x15: return PropKind::Utf8;        // Authentication Method
        case 0x16: return PropKind::Binary;      // Authentication Data
        case 0x17: return PropKind::Byte;        // Request Problem Information
        case 0x18: return PropKind::FourByte;    // Will Delay Interval
        case 0x19: return PropKind::Byte;        // Request Response Information
        case 0x1A: return PropKind::Utf8;        // Response Information
        case 0x1C: return PropKind::Utf8;        // Server Reference
        case 0x1F: return PropKind::Utf8;        // Reason String
        case 0x21: return PropKind::TwoByte;     // Receive Maximum
        case 0x22: return PropKind::TwoByte;     // Topic Alias Maximum
        case 0x23: return PropKind::TwoByte;     // Topic Alias
        case 0x24: return PropKind::Byte;        // Maximum QoS
        case 0x25: return PropKind::Byte;        // Retain Available
        case 0x26: return PropKind::StringPair;  // User Property
        case 0x27: return PropKind::FourByte;    // Maximum Packet Size
        case 0x28: return PropKind::Byte;        // Wildcard Subscription Available
        case 0x29: return PropKind::Byte;        // Subscription Identifier Available
        case 0x2A: return PropKind::Byte;        // Shared Subscription Available
        default:   return PropKind::Unknown;
    }
}

// Read a 2-byte big-endian length-prefixed byte run that must lie entirely
// inside [pos, end). Advances pos past it.
bool readLengthPrefixed(const std::vector<uint8_t>& body, size_t& pos, size_t end,
                        std::vector<uint8_t>& out) {
    if (pos + 2 > end) {
        return false;
    }
    const size_t len = static_cast<size_t>((body[pos] << 8) | body[pos + 1]);
    pos += 2;
    if (pos + len > end) {
        return false;
    }
    out.assign(body.begin() + static_cast<std::ptrdiff_t>(pos),
               body.begin() + static_cast<std::ptrdiff_t>(pos + len));
    pos += len;
    return true;
}

bool readPropString(const std::vector<uint8_t>& body, size_t& pos, size_t end,
                    std::string& out) {
    std::vector<uint8_t> raw;
    if (!readLengthPrefixed(body, pos, end, raw)) {
        return false;
    }
    out.assign(raw.begin(), raw.end());
    return true;
}

}  // namespace

DecodedProperties MqttParser::decodeProperties(const std::vector<uint8_t>& body,
                                               size_t offset) {
    DecodedProperties r;

    if (offset > body.size()) {
        return r;   // valid == false
    }

    // --- Property Length prefix --------------------------------------------
    uint32_t declared = 0;
    size_t prefixBytes = 0;
    if (!decodeRemainingLength(body, offset, declared, prefixBytes)) {
        return r;   // truncated or over-long length field
    }
    // Reject a non-minimal encoding (e.g. 0x85 0x80 0x00 for the value 5). The
    // library must refuse those, and admitting them here would make the corpus
    // entry look well framed.
    if (MqttPacket::encodeRemainingLength(declared).size() != prefixBytes) {
        return r;
    }
    r.declaredLength = declared;

    const size_t bodyStart = offset + prefixBytes;
    if (declared > body.size() - bodyStart) {
        return r;   // the declared body runs past the bytes actually supplied
    }
    const size_t end = bodyStart + declared;

    // --- Entries ------------------------------------------------------------
    size_t pos = bodyStart;
    while (pos < end) {
        DecodedProperty e;
        e.id = body[pos++];

        switch (propKind(e.id)) {
            case PropKind::Byte: {
                if (pos + 1 > end) { return r; }
                e.value = body[pos++];
                break;
            }
            case PropKind::TwoByte: {
                if (pos + 2 > end) { return r; }
                e.value = static_cast<uint32_t>((body[pos] << 8) | body[pos + 1]);
                pos += 2;
                break;
            }
            case PropKind::FourByte: {
                if (pos + 4 > end) { return r; }
                e.value = (static_cast<uint32_t>(body[pos]) << 24)
                          | (static_cast<uint32_t>(body[pos + 1]) << 16)
                          | (static_cast<uint32_t>(body[pos + 2]) << 8)
                          | static_cast<uint32_t>(body[pos + 3]);
                pos += 4;
                break;
            }
            case PropKind::VarInt: {
                uint32_t v = 0;
                size_t used = 0;
                if (!decodeRemainingLength(body, pos, v, used)) { return r; }
                if (pos + used > end) { return r; }   // field spills past the body
                e.value = v;
                pos += used;
                break;
            }
            case PropKind::Utf8: {
                if (!readPropString(body, pos, end, e.s)) { return r; }
                break;
            }
            case PropKind::Binary: {
                if (!readLengthPrefixed(body, pos, end, e.bin)) { return r; }
                break;
            }
            case PropKind::StringPair: {
                if (!readPropString(body, pos, end, e.s)) { return r; }
                if (!readPropString(body, pos, end, e.s2)) { return r; }
                break;
            }
            case PropKind::Unknown:
            default:
                return r;   // identifier not in the MQTT 5.0 table
        }

        r.entries.push_back(e);
    }

    // pos == end exactly: the last entry ended on the declared boundary.
    r.totalBytes = prefixBytes + declared;
    r.valid = true;
    return r;
}

// ===========================================================================
//  Generalized structural validator (task 4.4)
// ===========================================================================
//
// WHERE THE PROPERTIES FIELD SITS, per packet type (MQTT 5.0):
//
//   CONNECT (1)      variable header: "MQTT" + level + flags + keepalive,
//                    THEN Properties. Payload: client id, then - only when the
//                    Will flag is set - a SECOND Properties field (the Will
//                    Properties) immediately before the Will Topic, then the
//                    Will Message, then user name / password per their flags.
//                    This is the only packet carrying two Properties fields.
//   CONNACK (2)      ack flags + reason code, THEN Properties. No payload.
//   PUBLISH (3)      topic, packet id (QoS > 0 only), THEN Properties, then the
//                    payload, which is simply the remainder.
//   PUBACK/PUBREC/
//   PUBREL/PUBCOMP   packet id, then the OPTIONAL reason code, then the OPTIONAL
//   (4..7)           Properties. Three legal forms: Remaining Length 2 (id
//                    only), 3 (id + reason code), > 3 (id + reason code +
//                    Properties).
//   SUBSCRIBE (8)    packet id, THEN Properties, then (filter, options) pairs.
//   SUBACK (9)       packet id, THEN Properties, then the reason-code list.
//   UNSUBSCRIBE (10) packet id, THEN Properties, then the filter list.
//   UNSUBACK (11)    packet id, THEN Properties, then the reason-code list.
//   PINGREQ (12) /
//   PINGRESP (13)    no Properties, no payload: Remaining Length must be 0.
//   DISCONNECT (14)  Remaining Length 0 is the short form; otherwise reason
//                    code, then Properties when more bytes follow.
//   AUTH (15)        same shape as DISCONNECT: reason code, then Properties.
//
// In the MQTT 3.1.1 selection none of these Properties fields exist, so the
// walks below skip them (kPropsOnWire == false) and the function reduces to the
// Remaining Length invariant plus the per-type field layout of 3.1.1.

namespace {

#if MQTT_VERSION == MQTT_VERSION_5_0
constexpr bool kPropsOnWire = true;
#else
constexpr bool kPropsOnWire = false;
#endif

// Consume a Properties field at `pos` and require it to be internally
// consistent: its declared length must equal its actual body length, which is
// exactly what DecodedProperties::valid reports.
bool consumeProps(const std::vector<uint8_t>& body, size_t& pos) {
    if (!kPropsOnWire) {
        return true;   // 3.1.1: no Properties field on any packet
    }
    const DecodedProperties p = MqttParser::decodeProperties(body, pos);
    if (!p.valid) {
        return false;
    }
    pos += p.totalBytes;
    return true;
}

bool consumeU16(const std::vector<uint8_t>& body, size_t& pos) {
    if (pos + 2 > body.size()) {
        return false;
    }
    pos += 2;
    return true;
}

}  // namespace

bool MqttParser::isStructurallyWellFramed(const std::vector<uint8_t>& bytes) {
    if (bytes.empty()) {
        return false;
    }

    const uint8_t typeNibble = static_cast<uint8_t>((bytes[0] >> 4) & 0x0F);
    const uint8_t flags = static_cast<uint8_t>(bytes[0] & 0x0F);

    // --- Invariant 1: declared Remaining Length == actual trailing bytes -----
    uint32_t remaining = 0;
    size_t rlBytes = 0;
    if (!decodeRemainingLength(bytes, 1, remaining, rlBytes)) {
        return false;
    }
    const size_t bodyStart = 1 + rlBytes;
    if (bodyStart > bytes.size()) {
        return false;
    }
    if (remaining != bytes.size() - bodyStart) {
        return false;
    }

    const std::vector<uint8_t> body(bytes.begin() + static_cast<std::ptrdiff_t>(bodyStart),
                                    bytes.end());
    size_t pos = 0;

    // --- Invariant 2: every declared Properties length == its body length ----
    switch (typeNibble) {
        case 1: {   // CONNECT
            std::string protocolName;
            if (!readString(body, pos, protocolName)) { return false; }
            if (pos + 4 > body.size()) { return false; }   // level + flags + keepalive
            const uint8_t connectFlags = body[pos + 1];
            pos += 4;
            if (!consumeProps(body, pos)) { return false; }
            std::string s;
            if (!readString(body, pos, s)) { return false; }   // client id
            if ((connectFlags & 0x04) != 0) {                  // Will flag
                if (!consumeProps(body, pos)) { return false; }   // Will Properties
                if (!readString(body, pos, s)) { return false; }  // Will Topic
                if (!readString(body, pos, s)) { return false; }  // Will Message
            }
            if ((connectFlags & 0x80) != 0 && !readString(body, pos, s)) { return false; }
            if ((connectFlags & 0x40) != 0 && !readString(body, pos, s)) { return false; }
            return pos == body.size();
        }
        case 2: {   // CONNACK
            if (pos + 2 > body.size()) { return false; }   // ack flags + reason code
            pos += 2;
            if (!consumeProps(body, pos)) { return false; }
            return pos == body.size();
        }
        case 3: {   // PUBLISH
            std::string topic;
            if (!readString(body, pos, topic)) { return false; }
            const uint8_t qos = static_cast<uint8_t>((flags >> 1) & 0x03);
            if (qos > 0 && !consumeU16(body, pos)) { return false; }
            if (!consumeProps(body, pos)) { return false; }
            // Whatever remains is the payload, including nothing at all. There is
            // no following field to pin the Properties length against, so an
            // over-declared length is caught only when it runs past the packet.
            return pos <= body.size();
        }
        case 4:     // PUBACK
        case 5:     // PUBREC
        case 6:     // PUBREL
        case 7: {   // PUBCOMP
            if (!consumeU16(body, pos)) { return false; }
            if (body.size() == 2) { return true; }   // short form: id only
            if (body.size() == 3) { return true; }   // id + reason code
            pos += 1;                                // reason code
            if (!consumeProps(body, pos)) { return false; }
            return pos == body.size();
        }
        case 8: {   // SUBSCRIBE
            if (!consumeU16(body, pos)) { return false; }
            if (!consumeProps(body, pos)) { return false; }
            size_t filters = 0;
            while (pos < body.size()) {
                std::string filter;
                if (!readString(body, pos, filter)) { return false; }
                if (pos >= body.size()) { return false; }   // missing options byte
                pos += 1;
                filters++;
            }
            return filters > 0;
        }
        case 9: {   // SUBACK
            if (!consumeU16(body, pos)) { return false; }
            if (!consumeProps(body, pos)) { return false; }
            return pos < body.size();   // at least one return / reason code
        }
        case 11: {   // UNSUBACK
            if (!consumeU16(body, pos)) { return false; }
            if (!kPropsOnWire) {
                // 3.1.1 UNSUBACK is the packet identifier and nothing else - it
                // gained its reason-code list only in 5.0.
                return pos == body.size();
            }
            if (!consumeProps(body, pos)) { return false; }
            return pos < body.size();   // at least one reason code
        }
        case 10: {   // UNSUBSCRIBE
            if (!consumeU16(body, pos)) { return false; }
            if (!consumeProps(body, pos)) { return false; }
            size_t filters = 0;
            while (pos < body.size()) {
                std::string filter;
                if (!readString(body, pos, filter)) { return false; }
                filters++;
            }
            return filters > 0;
        }
        case 12:     // PINGREQ
        case 13:     // PINGRESP
            return body.empty();
        case 14:     // DISCONNECT
        case 15: {   // AUTH (type 15 is Reserved in 3.1.1)
            if (body.empty()) { return true; }   // short form
            pos += 1;                            // reason code
            if (pos == body.size()) { return true; }
            if (!consumeProps(body, pos)) { return false; }
            return pos == body.size();
        }
        default:
            return false;   // type 0 is forbidden
    }
}
