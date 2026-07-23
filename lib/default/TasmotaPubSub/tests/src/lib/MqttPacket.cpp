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

MqttPacket MqttPacket::connack(uint8_t returnCode, bool sessionPresent) {
    std::vector<uint8_t> body;
    body.push_back(sessionPresent ? 0x01 : 0x00);   // connack acknowledge flags
    body.push_back(returnCode);
    return framed(MQTTCONNACK, body);
}

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
