/*
  MqttCorpus.cpp - Systematic malformed MQTT 5.0 inbound-packet corpus.

  This deliberately builds mutations from conformant packet producers, then
  replaces one structural element at a time. It avoids hand-maintained byte
  arrays and keeps every fixture in a std::vector<uint8_t>.
*/

#include "MqttCorpus.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <iterator>
#include <utility>

namespace {

constexpr uint8_t kMalformedReason = 0x81;
constexpr uint8_t kProtocolReason = 0x82;
constexpr uint8_t kBadAuthenticationReason = 0x8C;

using Bytes = std::vector<uint8_t>;
using Corpus = std::vector<MqttCorpusEntry>;

void add(Corpus& corpus, const std::string& label, const Bytes& bytes,
         MqttError error = MqttError::MalformedPacket,
         uint8_t reason = kMalformedReason) {
    corpus.push_back({label, MqttPacket::raw(bytes), error, reason});
}

void add(Corpus& corpus, const std::string& label, const MqttPacket& packet,
         MqttError error = MqttError::MalformedPacket,
         uint8_t reason = kMalformedReason) {
    add(corpus, label, packet.bytes(), error, reason);
}

bool remainingLength(const Bytes& bytes, size_t& lengthOffset, size_t& bodyOffset,
                     uint32_t& value) {
    if (bytes.size() < 2) {
        return false;
    }
    lengthOffset = 1;
    value = 0;
    uint32_t multiplier = 1;
    for (size_t i = 0; i < 4 && lengthOffset + i < bytes.size(); ++i) {
        const uint8_t digit = bytes[lengthOffset + i];
        value += static_cast<uint32_t>(digit & 0x7F) * multiplier;
        if ((digit & 0x80) == 0) {
            bodyOffset = lengthOffset + i + 1;
            return true;
        }
        multiplier <<= 7;
    }
    return false;
}

Bytes replaceField(const Bytes& bytes, size_t offset, size_t width,
                   const Bytes& replacement) {
    Bytes result = bytes;
    result.erase(result.begin() + static_cast<ptrdiff_t>(offset),
                 result.begin() + static_cast<ptrdiff_t>(offset + width));
    result.insert(result.begin() + static_cast<ptrdiff_t>(offset),
                  replacement.begin(), replacement.end());
    return result;
}

Bytes replaceRemainingLength(const Bytes& bytes, uint32_t value) {
    size_t lengthOffset = 0;
    size_t bodyOffset = 0;
    uint32_t ignored = 0;
    if (!remainingLength(bytes, lengthOffset, bodyOffset, ignored)) {
        return bytes;
    }
    return replaceField(bytes, lengthOffset, bodyOffset - lengthOffset,
                        MqttPacket::encodeRemainingLength(value));
}

size_t findSequence(const Bytes& haystack, const Bytes& needle) {
    const auto it = std::search(haystack.begin(), haystack.end(),
                                needle.begin(), needle.end());
    return (it == haystack.end()) ? haystack.size()
                                  : static_cast<size_t>(it - haystack.begin());
}

enum class PropertyFrame : uint8_t {
    Connack,
    PublishQos0,
    PublishQos1,
    PublishQos2,
    Puback,
    Pubrec,
    Pubrel,
    Pubcomp,
    Suback,
    Unsuback,
    Disconnect,
    Auth,
};

MqttPacket withProperties(PropertyFrame frame, const MqttProps& props) {
    switch (frame) {
    case PropertyFrame::Connack:
        return MqttPacket::connack5(0, false, props);
    case PropertyFrame::PublishQos0:
        return MqttPacket::publish5("topic", {'p'}, 0, false, false, 0, props);
    case PropertyFrame::PublishQos1:
        return MqttPacket::publish5("topic", {'p'}, 1, false, false, 1, props);
    case PropertyFrame::PublishQos2:
        return MqttPacket::publish5("topic", {'p'}, 2, false, false, 1, props);
    case PropertyFrame::Puback:
        return MqttPacket::ack5(MQTTPUBACK, 1, 0, props);
    case PropertyFrame::Pubrec:
        return MqttPacket::ack5(MQTTPUBREC, 1, 0, props);
    case PropertyFrame::Pubrel:
        return MqttPacket::ack5(MQTTPUBREL, 1, 0, props);
    case PropertyFrame::Pubcomp:
        return MqttPacket::ack5(MQTTPUBCOMP, 1, 0, props);
    case PropertyFrame::Suback:
        return MqttPacket::suback5(1, {0}, props);
    case PropertyFrame::Unsuback:
        return MqttPacket::unsuback5(1, {0}, props);
    case PropertyFrame::Disconnect:
        return MqttPacket::disconnect5(0, props);
    case PropertyFrame::Auth:
        return MqttPacket::auth(0, props);
    }
    return MqttPacket::raw({});
}

const char* frameName(PropertyFrame frame) {
    static const char* const names[] = {
        "CONNACK", "PUBLISH QoS 0", "PUBLISH QoS 1", "PUBLISH QoS 2",
        "PUBACK", "PUBREC", "PUBREL", "PUBCOMP", "SUBACK", "UNSUBACK",
        "DISCONNECT", "AUTH",
    };
    return names[static_cast<unsigned>(frame)];
}

void addRemainingLengthMutations(Corpus& corpus, const std::string& name,
                                 const MqttPacket& packet) {
    size_t lengthOffset = 0;
    size_t bodyOffset = 0;
    uint32_t actual = 0;
    if (!remainingLength(packet.bytes(), lengthOffset, bodyOffset, actual)) {
        return;
    }
    // A shorter Remaining Length makes the trailing bytes a separate stream
    // frame, which a TCP client cannot attribute to this packet. Keep only
    // unambiguously malformed declarations: a body that cannot arrive in full,
    // an implausibly large body, and VBI encodings beyond the supported width.
    add(corpus, name + " remaining length=" + std::to_string(actual + 1u),
        replaceRemainingLength(packet.bytes(), actual + 1u), MqttError::ReceiveTimeout, 0);
    add(corpus, name + " remaining length=65535",
        replaceRemainingLength(packet.bytes(), 0xFFFFu));

    // A second continuation bit implies a third byte; 3-/4-byte values exercise
    // the same narrowed-codec rejection without relying on a hand-written VBI.
    add(corpus, name + " remaining length has a third byte",
        replaceField(packet.bytes(), lengthOffset, bodyOffset - lengthOffset,
                     Bytes{0x80, 0x80, 0x00}));
    for (uint32_t value : {300000u, 5000000u, 200000000u}) {
        add(corpus, name + " remaining length over-long " + std::to_string(value),
            replaceRemainingLength(packet.bytes(), value));
    }
}

void addPropertyLengthAndValueMutations(Corpus& corpus) {
    MqttProps value;
    value.str(0x03, "value");  // Content Type is a legal PUBLISH UTF-8 property.
    const MqttPacket packet = withProperties(PropertyFrame::PublishQos0, value);
    const Bytes props = value.bytes();
    const size_t propertyOffset = findSequence(packet.bytes(), props);
    if (propertyOffset == packet.bytes().size()) {
        return;
    }

    // An under-declared Properties length merely turns the remaining bytes into
    // PUBLISH payload on a stream. Exercise declarations that overrun the
    // received property region instead, which are unambiguously malformed.
    add(corpus, "PUBLISH Property Length=" + std::to_string(props.size()),
        replaceField(packet.bytes(), propertyOffset, 1,
                     MqttPacket::encodeRemainingLength(static_cast<uint32_t>(props.size()))),
        MqttError::ProtocolError, kProtocolReason);
    add(corpus, "PUBLISH Property Length=65535",
        replaceField(packet.bytes(), propertyOffset, 1,
                     MqttPacket::encodeRemainingLength(0xFFFFu)));
    add(corpus, "PUBLISH Property Length has a third byte",
        replaceField(packet.bytes(), propertyOffset, 1, Bytes{0x80, 0x80, 0x00}));

    // The first property is 0x03 followed by its two-byte UTF-8 length. Mutate
    // that value length without changing the surrounding Properties field.
    // A shorter UTF-8 value length simply changes where the payload begins in
    // the byte stream. Use an overrun declaration, which cannot be interpreted
    // as a distinct well-framed PUBLISH.
    const size_t stringLengthOffset = propertyOffset + 2;
    const uint16_t valueLength = 0xFFFF;
    add(corpus, "PUBLISH Content Type length=" + std::to_string(valueLength),
        replaceField(packet.bytes(), stringLengthOffset, 2,
                     Bytes{static_cast<uint8_t>(valueLength >> 8),
                           static_cast<uint8_t>(valueLength)}));
}

void addTopicLengthMutations(Corpus& corpus) {
    const MqttPacket packet = MqttPacket::publish5("topic", {'p'}, 0);
    size_t lengthOffset = 0;
    size_t bodyOffset = 0;
    uint32_t ignored = 0;
    if (!remainingLength(packet.bytes(), lengthOffset, bodyOffset, ignored)) {
        return;
    }
    for (uint16_t value : {uint16_t(0), uint16_t(1), uint16_t(4),
                           uint16_t(6), uint16_t(0xFFFF)}) {
        add(corpus, "PUBLISH topic length=" + std::to_string(value),
            replaceField(packet.bytes(), bodyOffset, 2,
                         Bytes{static_cast<uint8_t>(value >> 8),
                               static_cast<uint8_t>(value)}));
    }
}

void addPropertyRegionMutations(Corpus& corpus) {
    const std::array<PropertyFrame, 12> frames = {
        PropertyFrame::Connack, PropertyFrame::PublishQos0, PropertyFrame::PublishQos1,
        PropertyFrame::PublishQos2, PropertyFrame::Puback, PropertyFrame::Pubrec,
        PropertyFrame::Pubrel, PropertyFrame::Pubcomp, PropertyFrame::Suback,
        PropertyFrame::Unsuback, PropertyFrame::Disconnect, PropertyFrame::Auth,
    };

    for (PropertyFrame frame : frames) {
        const std::string name = frameName(frame);
        const uint8_t semanticReason =
            (frame == PropertyFrame::Auth) ? kBadAuthenticationReason : kProtocolReason;
        MqttProps illegal;
        illegal.byte_(frame == PropertyFrame::Connack ? 0x01 : 0x24, 0);
        add(corpus, name + " illegal property identifier", withProperties(frame, illegal),
            MqttError::ProtocolError, semanticReason);

        MqttProps duplicate;
        if (frame == PropertyFrame::Connack) {
            duplicate.u16(0x13, 1).u16(0x13, 1);
        } else if (frame == PropertyFrame::PublishQos0 ||
                   frame == PropertyFrame::PublishQos1 ||
                   frame == PropertyFrame::PublishQos2) {
            duplicate.byte_(0x01, 1).byte_(0x01, 1);
        } else {
            duplicate.str(0x1F, "a").str(0x1F, "b");
        }
        add(corpus, name + " duplicate single-occurrence property",
            withProperties(frame, duplicate), MqttError::ProtocolError, semanticReason);

        MqttProps wrongWidth;
        wrongWidth.byte_(0x21, 0);  // Receive Maximum must be a two-byte value.
        add(corpus, name + " wrong-width property value", withProperties(frame, wrongWidth),
            MqttError::ProtocolError, semanticReason);

        for (uint8_t authId : {uint8_t(0x15), uint8_t(0x16)}) {
            for (uint16_t valueLength : {uint16_t(0), uint16_t(1), uint16_t(16),
                                         uint16_t(17), uint16_t(0xFFFF)}) {
                const size_t storedLength = std::min<size_t>(valueLength, 32u);
                const Bytes data(storedLength, 0xA5);
                for (const char* position : {"first", "middle", "last"}) {
                    MqttProps props;
                    if (std::string(position) != "first") {
                        props.pair(0x26, "before", "value");
                    }
                    if (authId == 0x15) {
                        props.str(authId, std::string(storedLength, 'a'));
                    } else {
                        props.bin(authId, data);
                    }
                    if (std::string(position) == "middle") {
                        props.pair(0x26, "after", "value");
                    }
                    const MqttPacket packet = withProperties(frame, props);
                    Bytes bytes = packet.bytes();
                    const auto auth = std::find(bytes.begin() + 2, bytes.end(), authId);
                    if (auth != bytes.end() && std::distance(auth, bytes.end()) > 2) {
                        *(auth + 1) = static_cast<uint8_t>(valueLength >> 8);
                        *(auth + 2) = static_cast<uint8_t>(valueLength);
                    }
                    add(corpus, name + " authentication property " +
                                    std::to_string(authId) + " " + position +
                                    " length=" + std::to_string(valueLength),
                        bytes, MqttError::ProtocolError, kBadAuthenticationReason);
                }
            }
        }
    }
}

void addCorrelationDataMutations(Corpus& corpus) {
    for (uint16_t declared : {uint16_t(17), uint16_t(18), uint16_t(255), uint16_t(0xFFFF)}) {
        const size_t actual = declared;
        MqttProps props;
        props.bin(0x09, Bytes(actual, 0xC3));
        MqttPacket packet = withProperties(PropertyFrame::PublishQos0, props);
        const bool exceedsVbiLimit = declared == 0xFFFF;
        add(corpus, "PUBLISH Correlation Data length=" + std::to_string(declared),
            packet,
            exceedsVbiLimit ? MqttError::MalformedPacket : MqttError::ProtocolError,
            exceedsVbiLimit ? kMalformedReason : kProtocolReason);

        Bytes overrun = packet.bytes();
        const auto it = std::find(overrun.begin() + 2, overrun.end(), uint8_t(0x09));
        if (it != overrun.end() && std::distance(it, overrun.end()) > 2) {
            *(it + 1) = 0xFF;
            *(it + 2) = 0xFF;
            add(corpus, "PUBLISH Correlation Data declared length overruns packet " +
                            std::to_string(declared), overrun,
                exceedsVbiLimit ? MqttError::MalformedPacket : MqttError::ProtocolError,
                exceedsVbiLimit ? kMalformedReason : kProtocolReason);
        }
    }
}

// Task 18.4 semantic rejects. The companion Property 32 also exercises the legal half of
// each complete low-nibble and Reason Code domain; this corpus retains every invalid member
// so the sanitizer path independently proves it closes without dispatching state.
bool allowsPublishAckReason(uint8_t code) {
    switch (code) {
        case 0x00: case 0x10: case 0x80: case 0x83: case 0x87:
        case 0x90: case 0x91: case 0x97: case 0x99:
            return true;
        default:
            return false;
    }
}

bool allowsPubrelCompReason(uint8_t code) {
    return code == 0x00 || code == 0x92;
}

bool allowsSubackReason(uint8_t code) {
    switch (code) {
        case 0x00: case 0x01: case 0x02: case 0x80: case 0x83: case 0x87:
        case 0x8F: case 0x91: case 0x97: case 0x9E: case 0xA1: case 0xA2:
            return true;
        default:
            return false;
    }
}

bool allowsUnsubackReason(uint8_t code) {
    switch (code) {
        case 0x00: case 0x11: case 0x80: case 0x83: case 0x87: case 0x8F: case 0x91:
            return true;
        default:
            return false;
    }
}

bool allowsDisconnectReason(uint8_t code) {
    switch (code) {
        case 0x00: case 0x04: case 0x80: case 0x81: case 0x82: case 0x83: case 0x87:
        case 0x89: case 0x8B: case 0x8D: case 0x8E: case 0x8F: case 0x90: case 0x93:
        case 0x94: case 0x95: case 0x96: case 0x97: case 0x98: case 0x99: case 0x9A:
        case 0x9B: case 0x9C: case 0x9D: case 0x9E: case 0x9F: case 0xA0: case 0xA1:
        case 0xA2:
            return true;
        default:
            return false;
    }
}

MqttPacket semanticSeed(uint8_t type) {
    switch (type) {
        case MQTTCONNACK:    return MqttPacket::connack5(0);
        case MQTTPUBLISH:    return MqttPacket::publish5("topic", {'p'}, 0);
        case MQTTPUBACK:
        case MQTTPUBREC:
        case MQTTPUBREL:
        case MQTTPUBCOMP:    return MqttPacket::ack5(type, 1, -1);
        case MQTTSUBACK:     return MqttPacket::suback5(1, {0});
        case MQTTUNSUBACK:   return MqttPacket::unsuback5(1, {0});
        case MQTTPINGRESP:   return MqttPacket::pingresp();
        case MQTTDISCONNECT: return MqttPacket::disconnect5(-1);
        default:             return MqttPacket::auth(0);
    }
}

void addSemanticValidationMutations(Corpus& corpus) {
    struct FixedHeaderRule { uint8_t type; uint8_t flags; };
    const FixedHeaderRule rules[] = {
        {MQTTCONNACK, 0}, {MQTTPUBLISH, 0}, {MQTTPUBACK, 0}, {MQTTPUBREC, 0},
        {MQTTPUBREL, 2}, {MQTTPUBCOMP, 0}, {MQTTSUBACK, 0}, {MQTTUNSUBACK, 0},
        {MQTTPINGRESP, 0}, {MQTTDISCONNECT, 0}, {MQTTAUTH, 0},
    };
    for (const FixedHeaderRule& rule : rules) {
        for (uint8_t flags = 0; flags < 16; ++flags) {
            if (flags == rule.flags) {
                continue;
            }
            if (rule.type == MQTTPUBLISH) {
                const uint8_t qos = (uint8_t)((flags >> 1) & 0x03);
                if ((qos != 3) && !((qos == 0) && ((flags & 0x08) != 0))) {
                    continue;
                }
            }
            Bytes bytes = semanticSeed(rule.type).bytes();
            bytes[0] = (uint8_t)(rule.type | flags);
            add(corpus, "semantic invalid fixed-header flags " + std::to_string(rule.type) +
                            "/" + std::to_string(flags), bytes);
        }
    }

    add(corpus, "semantic PUBLISH QoS 1 zero Packet Identifier",
        MqttPacket::publish5("topic", {'p'}, 1, false, false, 0),
        MqttError::ProtocolError, kProtocolReason);
    add(corpus, "semantic PUBLISH QoS 2 zero Packet Identifier",
        MqttPacket::publish5("topic", {'p'}, 2, false, false, 0),
        MqttError::ProtocolError, kProtocolReason);
    for (uint8_t type : {uint8_t(MQTTPUBACK), uint8_t(MQTTPUBREC),
                         uint8_t(MQTTPUBREL), uint8_t(MQTTPUBCOMP)}) {
        add(corpus, "semantic zero Packet Identifier " + std::to_string(type),
            MqttPacket::ack5(type, 0, -1), MqttError::ProtocolError, kProtocolReason);
    }
    add(corpus, "semantic SUBACK zero Packet Identifier", MqttPacket::suback5(0, {0}),
        MqttError::ProtocolError, kProtocolReason);
    add(corpus, "semantic UNSUBACK zero Packet Identifier", MqttPacket::unsuback5(0, {0}),
        MqttError::ProtocolError, kProtocolReason);
    add(corpus, "semantic PINGRESP nonzero Remaining Length",
        Bytes{uint8_t(MQTTPINGRESP), 0x01, 0x00});

    for (unsigned value = 0; value <= 0xFF; ++value) {
        const uint8_t code = (uint8_t)value;
        for (uint8_t type : {uint8_t(MQTTPUBACK), uint8_t(MQTTPUBREC),
                             uint8_t(MQTTPUBREL), uint8_t(MQTTPUBCOMP)}) {
            const bool allowed = (type == MQTTPUBACK || type == MQTTPUBREC)
                ? allowsPublishAckReason(code) : allowsPubrelCompReason(code);
            if (!allowed) {
                add(corpus, "semantic invalid acknowledgement Reason Code " +
                                std::to_string(type) + "/" + std::to_string(code),
                    MqttPacket::ack5(type, 1, code), MqttError::ProtocolError, kProtocolReason);
            }
        }
        if (!allowsSubackReason(code)) {
            add(corpus, "semantic invalid SUBACK Reason Code " + std::to_string(code),
                MqttPacket::suback5(1, {code}), MqttError::ProtocolError, kProtocolReason);
        }
        if (!allowsUnsubackReason(code)) {
            add(corpus, "semantic invalid UNSUBACK Reason Code " + std::to_string(code),
                MqttPacket::unsuback5(1, {code}), MqttError::ProtocolError, kProtocolReason);
        }
        if (!allowsDisconnectReason(code)) {
            add(corpus, "semantic invalid DISCONNECT Reason Code " + std::to_string(code),
                MqttPacket::disconnect5(code), MqttError::ProtocolError, kProtocolReason);
        }
    }
}

}  // namespace

std::vector<MqttCorpusEntry> mqtt5MalformedCorpus() {
    Corpus corpus;

    // Well-formed seed shapes cover every inbound type and every acknowledgement
    // representation. Every proper prefix is malformed because it cannot satisfy
    // the preceding packet's declared structure.
    MqttProps userProperty;
    userProperty.pair(0x26, "key", "value");
    const std::vector<std::pair<std::string, MqttPacket>> seeds = {
        {"CONNACK", MqttPacket::connack5(0)},
        {"PUBLISH QoS 0", MqttPacket::publish5("topic", {'p'}, 0)},
        {"PUBLISH QoS 1", MqttPacket::publish5("topic", {'p'}, 1, false, false, 1)},
        {"PUBLISH QoS 2", MqttPacket::publish5("topic", {'p'}, 2, false, false, 1)},
        {"PUBACK short", MqttPacket::ack5(MQTTPUBACK, 1, -1)},
        {"PUBACK mid", MqttPacket::ack5(MQTTPUBACK, 1, 0)},
        {"PUBACK long", MqttPacket::ack5(MQTTPUBACK, 1, 0, userProperty)},
        {"PUBREC short", MqttPacket::ack5(MQTTPUBREC, 1, -1)},
        {"PUBREC mid", MqttPacket::ack5(MQTTPUBREC, 1, 0)},
        {"PUBREC long", MqttPacket::ack5(MQTTPUBREC, 1, 0, userProperty)},
        {"PUBREL short", MqttPacket::ack5(MQTTPUBREL, 1, -1)},
        {"PUBREL mid", MqttPacket::ack5(MQTTPUBREL, 1, 0)},
        {"PUBREL long", MqttPacket::ack5(MQTTPUBREL, 1, 0, userProperty)},
        {"PUBCOMP short", MqttPacket::ack5(MQTTPUBCOMP, 1, -1)},
        {"PUBCOMP mid", MqttPacket::ack5(MQTTPUBCOMP, 1, 0)},
        {"PUBCOMP long", MqttPacket::ack5(MQTTPUBCOMP, 1, 0, userProperty)},
        {"SUBACK", MqttPacket::suback5(1, {0})},
        {"UNSUBACK", MqttPacket::unsuback5(1, {0})},
        {"PINGREQ", MqttPacket::pingreq()},
        {"PINGRESP", MqttPacket::pingresp()},
        {"DISCONNECT", MqttPacket::disconnect5(-1)},
        {"AUTH", MqttPacket::auth(0)},
    };

    for (const auto& seed : seeds) {
        // The empty prefix is indistinguishable from no inbound packet at all, so
        // there is no protocol frame for the client to reject. Every observable
        // non-empty proper prefix starts a packet read and must time out safely.
        for (size_t prefix = 1; prefix < seed.second.size(); ++prefix) {
            add(corpus, seed.first + " proper prefix " + std::to_string(prefix),
                Bytes(seed.second.bytes().begin(), seed.second.bytes().begin() +
                      static_cast<ptrdiff_t>(prefix)),
                MqttError::ReceiveTimeout, 0);
        }
        addRemainingLengthMutations(corpus, seed.first, seed.second);
    }

    addTopicLengthMutations(corpus);
    addPropertyLengthAndValueMutations(corpus);
    addPropertyRegionMutations(corpus);
    addCorrelationDataMutations(corpus);
    addSemanticValidationMutations(corpus);

    // SUBACK/UNSUBACK require at least one reason code after their Properties
    // field; these encode the empty-list boundary separately from RL mutations.
    add(corpus, "SUBACK empty reason-code list", MqttPacket::suback5(1, {}));
    add(corpus, "UNSUBACK empty reason-code list", MqttPacket::unsuback5(1, {}));

    // Reserved/invalid inbound packet types, plus a PUBLISH fixed header with
    // QoS 3, all preserve a syntactically complete frame around the bad selector.
    for (uint8_t type : {uint8_t(0x00), uint8_t(MQTTCONNECT), uint8_t(MQTTSUBSCRIBE),
                         uint8_t(MQTTUNSUBSCRIBE), uint8_t(MQTTPINGREQ)}) {
        add(corpus, "invalid inbound packet type " + std::to_string(type), Bytes{type, 0},
            MqttError::ProtocolError, kProtocolReason);
    }
    Bytes qos3 = MqttPacket::publish5("topic", {'p'}, 0).bytes();
    qos3[0] = static_cast<uint8_t>((qos3[0] & 0xF9) | 0x06);
    add(corpus, "PUBLISH QoS 3 fixed header", qos3);

    return corpus;
}
