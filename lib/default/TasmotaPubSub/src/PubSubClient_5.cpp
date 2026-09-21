/*
  PubSubClient.cpp - A simple client for MQTT.

  TasmotaPubSub - Tasmota fork of the PubSubClient MQTT library.

  Original author:
    Nick O'Leary - http://knolleary.net
  Tasmota fork maintained by Theo Arends and the Tasmota contributors.

  SPDX-FileCopyrightText: 2008-2020 Nicholas O'Leary
  SPDX-FileCopyrightText: 2020-2025 Theo Arends and Tasmota contributors

  SPDX-License-Identifier: MIT
*/

#include "PubSubClient.h"
#include "Arduino.h"

#if MQTT_VERSION == MQTT_VERSION_5_0

// The effective protocol level is selected per instance. MQTT5-capable clients start
// at level 5 and may permanently select runtime level-4 fallback for that instance.
#define MQTT_RUNTIME_PROTOCOL() \
    ((this->sessionIdentity.protocolLevel == MQTT_VERSION_3_1_1) \
         ? MQTT_VERSION_3_1_1 : MQTT_VERSION_5_0)
#define MQTT_RUNTIME_IS_5() (MQTT_RUNTIME_PROTOCOL() == MQTT_VERSION_5_0)

PubSubClient::PubSubClient() {
    this->_state = MQTT_DISCONNECTED;
    this->_client = NULL;
    this->stream = NULL;
    setCallback(NULL);
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);
}

PubSubClient::PubSubClient(Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setClient(client);
    this->stream = NULL;
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);
}

PubSubClient::PubSubClient(IPAddress addr, uint16_t port, Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setServer(addr, port);
    setClient(client);
    this->stream = NULL;
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);
}
PubSubClient::PubSubClient(IPAddress addr, uint16_t port, Client& client, Stream& stream) {
    this->_state = MQTT_DISCONNECTED;
    setServer(addr,port);
    setClient(client);
    setStream(stream);
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);
}
PubSubClient::PubSubClient(IPAddress addr, uint16_t port, MQTT_CALLBACK_SIGNATURE, Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setServer(addr, port);
    setCallback(callback);
    setClient(client);
    this->stream = NULL;
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);
}
PubSubClient::PubSubClient(IPAddress addr, uint16_t port, MQTT_CALLBACK_SIGNATURE, Client& client, Stream& stream) {
    this->_state = MQTT_DISCONNECTED;
    setServer(addr,port);
    setCallback(callback);
    setClient(client);
    setStream(stream);
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);
}

PubSubClient::PubSubClient(uint8_t *ip, uint16_t port, Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setServer(ip, port);
    setClient(client);
    this->stream = NULL;
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);
}
PubSubClient::PubSubClient(uint8_t *ip, uint16_t port, Client& client, Stream& stream) {
    this->_state = MQTT_DISCONNECTED;
    setServer(ip,port);
    setClient(client);
    setStream(stream);
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);
}
PubSubClient::PubSubClient(uint8_t *ip, uint16_t port, MQTT_CALLBACK_SIGNATURE, Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setServer(ip, port);
    setCallback(callback);
    setClient(client);
    this->stream = NULL;
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);
}
PubSubClient::PubSubClient(uint8_t *ip, uint16_t port, MQTT_CALLBACK_SIGNATURE, Client& client, Stream& stream) {
    this->_state = MQTT_DISCONNECTED;
    setServer(ip,port);
    setCallback(callback);
    setClient(client);
    setStream(stream);
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);
}

PubSubClient::PubSubClient(const char* domain, uint16_t port, Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setServer(domain,port);
    setClient(client);
    this->stream = NULL;
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);
}
PubSubClient::PubSubClient(const char* domain, uint16_t port, Client& client, Stream& stream) {
    this->_state = MQTT_DISCONNECTED;
    setServer(domain,port);
    setClient(client);
    setStream(stream);
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);
}
PubSubClient::PubSubClient(const char* domain, uint16_t port, MQTT_CALLBACK_SIGNATURE, Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setServer(domain,port);
    setCallback(callback);
    setClient(client);
    this->stream = NULL;
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);
}
PubSubClient::PubSubClient(const char* domain, uint16_t port, MQTT_CALLBACK_SIGNATURE, Client& client, Stream& stream) {
    this->_state = MQTT_DISCONNECTED;
    setServer(domain,port);
    setCallback(callback);
    setClient(client);
    setStream(stream);
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);
}

PubSubClient::~PubSubClient() {
#ifdef USE_MQTT_QOS
  if (MQTT_RUNTIME_IS_5()) {
// Persistent local Session State may outlive a connection teardown until the next CONNACK
// decides whether the broker retained the session. Any retained packet bodies still owned by
// that state are finally released here if the client is destroyed first.
    for (uint8_t i = 0; i < MQTT_MAX_OUTSTANDING; i++) {
      releasePacketCopy(this->outstanding[i]);
    }
  }
#endif  // USE_MQTT_QOS
  free(this->buffer);
}

// --- CONNECT variable-header Properties field (Requirements 1.2, 13.6, 16.3) ---
//
// The CONNECT properties are fixed by the client's own configuration, not supplied by
// the caller, so they are not expressible through MqttMessageProperties (which models
// the per-message set of a PUBLISH). They are assembled here with the same two-pass
// discipline writeProperties() uses: a sizing pass to learn the body length, then the
// length prefix at its true width followed by the body, and `pos` returned unchanged -
// nothing written at all - when the field would not fit `capacity` (Requirement 13.2).
//
// Clean Start 0 additionally emits Session Expiry Interval (0x11), using the
// configured value exactly, including zero. Clean Start 1 omits it, whose MQTT 5.0
// default is zero.
static void emitConnectPropertySet(uint16_t receiveMaximum, uint32_t maximumPacketSize,
                                   bool persistentSession, uint32_t sessionExpiryInterval,
                                   MqttPropertyWriter& w) {
    w.addTwoByte(MQTT_PROP_RECEIVE_MAXIMUM, receiveMaximum);
    w.addFourByte(MQTT_PROP_MAXIMUM_PACKET_SIZE, maximumPacketSize);
    if (persistentSession) {
        w.addFourByte(0x11, sessionExpiryInterval);  // Session Expiry Interval
    }
}

static uint16_t writeConnectProperties(uint16_t receiveMaximum, uint32_t maximumPacketSize,
                                       bool persistentSession, uint32_t sessionExpiryInterval,
                                       uint8_t* buf, uint16_t pos, uint16_t capacity) {
    if ((buf == nullptr) || (pos >= capacity)) {
        return pos;   // nowhere to write even the length byte
    }
    uint16_t avail = (uint16_t)(capacity - pos);

// Pass 1 - sizing. The body capacity offered is what remains after the smallest
// possible length prefix, so a set that only fits because the prefix was ignored is
// rejected here rather than half-emitted below.
    MqttPropertyWriter sizer(nullptr, (uint16_t)(avail - 1));
    emitConnectPropertySet(receiveMaximum, maximumPacketSize, persistentSession,
                           sessionExpiryInterval, sizer);
    if (sizer.overflowed()) {
        return pos;
    }
    uint16_t bodyLen = sizer.bodyLength();

    uint8_t plen = PubSubClient::variableByteIntegerSize(bodyLen);
    if ((plen == 0) || ((uint32_t)plen + bodyLen > avail)) {
        return pos;
    }

// Pass 2 - emit the body at its final position, then the length prefix in front of it.
    MqttPropertyWriter emitter(buf + pos + plen, bodyLen);
    emitConnectPropertySet(receiveMaximum, maximumPacketSize, persistentSession,
                           sessionExpiryInterval, emitter);
    if (emitter.overflowed() || (emitter.bodyLength() != bodyLen)) {
        return pos;
    }
    PubSubClient::encodeVariableByteInteger(bodyLen, buf + pos);
    return (uint16_t)(pos + plen + bodyLen);
}

// CONNECT is assembled before its first transport write, so each field append must prove
// the whole field fits before touching the working buffer. On failure `pos` remains
// unchanged; connect() then closes the just-opened transport with BufferTooSmall and
// emits no bytes. These intentionally local helpers keep this discipline limited to
// CONNECT rather than changing the established semantics of generic packet writers.
static bool appendConnectBytes(uint8_t* buf, uint16_t capacity, uint16_t& pos,
                               const uint8_t* source, uint16_t count) {
    if ((buf == nullptr) || ((source == nullptr) && (count != 0)) ||
        (pos > capacity) || (count > (uint16_t)(capacity - pos))) {
        return false;
    }
    if (count != 0) {
        memcpy(buf + pos, source, count);
    }
    pos = (uint16_t)(pos + count);
    return true;
}

static bool appendConnectByte(uint8_t* buf, uint16_t capacity, uint16_t& pos,
                              uint8_t value) {
    return appendConnectBytes(buf, capacity, pos, &value, 1);
}

static bool appendConnectString(uint8_t* buf, uint16_t capacity, uint16_t& pos,
                                const char* value) {
    if (value == nullptr) {
        return false;
    }
    const size_t stringLength = strnlen(value, capacity);
    const uint32_t fieldLength = 2u + (uint32_t)stringLength;
    if ((stringLength > 0xFFFFu) || (pos > capacity) ||
        (fieldLength > (uint32_t)(capacity - pos))) {
        return false;
    }

    const uint8_t lengthBytes[2] = {
        (uint8_t)(stringLength >> 8),
        (uint8_t)(stringLength & 0xFF)
    };
    const uint16_t originalPos = pos;
    if (!appendConnectBytes(buf, capacity, pos, lengthBytes, sizeof(lengthBytes)) ||
        !appendConnectBytes(buf, capacity, pos,
                            reinterpret_cast<const uint8_t*>(value),
                            (uint16_t)stringLength)) {
        pos = originalPos;
        return false;
    }
    return true;
}

static bool appendConnectProperties(uint16_t receiveMaximum, uint32_t maximumPacketSize,
                                    bool persistentSession, uint32_t sessionExpiryInterval,
                                    uint8_t* buf, uint16_t capacity, uint16_t& pos) {
    const uint16_t end = writeConnectProperties(receiveMaximum, maximumPacketSize,
                                                persistentSession, sessionExpiryInterval,
                                                buf, pos, capacity);
    if (end == pos) {
        return false;
    }
    pos = end;
    return true;
}

// --- CONNACK acceptance (Requirements 1.4, 1.5, 1.6) -----------------------
//
// MQTT 5.0 CONNACK variable header, and there is no payload:
//
//   AckFlags(1, bit0 = Session Present) | ReasonCode(1) | Properties
//
// The 3.1.1 acceptance test (`len == 4 && buffer[1] == 2`) cannot express that shape,
// so in MQTT 5.0 mode it is replaced by the four checks below, in this order, each one
// before the read it authorises (Requirements 13.4, 13.5):
//
//   1. packet type is CONNACK *and* all four fixed-header flag bits are clear - one
//      whole-byte comparison against MQTTCONNACK covers both;
//   2. at least 3 bytes of variable header were received, i.e. Remaining Length >= 3
//      (2 for ack flags + reason code, and at least 1 for the Properties field, which
//      is never optional in 5.0: the empty set is still the byte 0x00);
//   3. the Properties field parses cleanly within the bytes actually received, in the
//      CONNACK context - identifier legality and duplicate rules included;
//   4. the field ends exactly where the packet ends. A CONNACK has no payload, so a
//      trailing byte means the framing does not add up (Requirement 14.3).
//
// `len` is what readPacket() returned: bytes ACTUALLY RECEIVED into `buf`, never a
// declared length. Bounds are therefore checked against real bytes, so a broker that
// declares more Properties than it sends is rejected by check 3 rather than trusted.
//
// Ack Flags bits 7–1 are reserved and MUST be zero. Bit 0 is Session Present, which
// is valid only on a successful CONNACK sent in response to Clean Start clear and only
// when compatible local session state exists. The parser enforces those conditions before
// it exposes any candidate state to connect().

// --- Negotiated limits carried out of the CONNACK (Requirements 5.1, 5.4, 5.7) ----
//
// The four values the CONNACK Properties field may declare, pre-loaded with their MQTT
// 5.0 defaults so that a field which omits a property leaves that property's default in
// place while the ones present are honored (Requirement 5.7).
//
// They are collected into this local struct rather than written straight onto the client
// because the walk is only ONE of the acceptance checks: checks 3 and 4 can still reject
// the packet after the visitor has run. Latching happens in connect(), after the whole
// acceptance test has passed, so a CONNACK the client refuses never changes a limit.
//
// Server Keep Alive needs a presence flag because there is no value of the property that
// means "absent": 0 is legal and means "keepalive disabled" (Requirement 5.1), which is
// not the same as "keep the requested interval".
struct Connack5Limits {
    bool     propertiesValid = true;
    bool     hasKeepAlive  = false;
    uint16_t keepAlive     = 0;
    uint32_t maxPacketSize = 0;       // 0 = no limit
    uint16_t topicAliasMax = 0;
    uint16_t receiveMaximum = 65535;
};

// Collect negotiated limits while decodeProperties() validates the complete field. Value
// domains that the generic type/placement table cannot express are latched here and checked
// transactionally by parseConnack5(). The visitor always continues so later entries still
// receive duplicate, placement, authentication, and bounds validation.
static bool connackLimitVisitor(const MqttPropertyView& prop, void* ctx) {
    Connack5Limits* out = static_cast<Connack5Limits*>(ctx);
    if (out == nullptr) {
        return true;
    }
    switch (prop.id) {
        case 0x13:                                      // Server Keep Alive: any uint16
            out->hasKeepAlive = true;
            out->keepAlive = (uint16_t)prop.value;
            break;
        case 0x27:                                      // Maximum Packet Size: 1..2^32-1
            if (prop.value == 0) {
                out->propertiesValid = false;
            } else {
                out->maxPacketSize = prop.value;
            }
            break;
        case 0x22:                                      // Topic Alias Maximum: any uint16
            out->topicAliasMax = (uint16_t)prop.value;
            break;
        case 0x21:                                      // Receive Maximum: 1..65535
            if (prop.value == 0) {
                out->propertiesValid = false;
            } else {
                out->receiveMaximum = (uint16_t)prop.value;
            }
            break;
        case 0x24:                                      // Maximum QoS: 0 or 1
            if (prop.value > 1) {
                out->propertiesValid = false;
            }
            break;
        case 0x25:                                      // Retain Available: boolean
        case 0x28:                                      // Wildcard Subscription Available
        case 0x29:                                      // Subscription Identifier Available
        case 0x2A:                                      // Shared Subscription Available
            if (prop.value > 1) {
                out->propertiesValid = false;
            }
            break;
        default:
            break;
    }
    return true;
}

static bool isAllowedConnackReasonCode(uint8_t reasonCode) {
    switch (reasonCode) {
        case 0x00:  // Success
        case 0x80:  // Unspecified error
        case 0x81:  // Malformed Packet
        case 0x82:  // Protocol Error
        case 0x83:  // Implementation specific error
        case 0x84:  // Unsupported Protocol Version
        case 0x85:  // Client Identifier not valid
        case 0x86:  // Bad User Name or Password
        case 0x87:  // Not authorized
        case 0x88:  // Server unavailable
        case 0x89:  // Server busy
        case 0x8A:  // Banned
        case 0x8C:  // Bad authentication method
        case 0x90:  // Topic Name invalid
        case 0x95:  // Packet too large
        case 0x97:  // Quota exceeded
        case 0x99:  // Payload format invalid
        case 0x9A:  // Retain not supported
        case 0x9B:  // QoS not supported
        case 0x9C:  // Use another server
        case 0x9D:  // Server moved
        case 0x9F:  // Connection rate exceeded
            return true;
        default:
            return false;
    }
}

static bool isMqtt311FallbackResponse(const uint8_t* buf, uint32_t len, uint8_t llen) {
    // Some MQTT 3.1.1-only brokers answer an unsupported level-5 CONNECT with their
    // native four-byte refusal rather than an MQTT 5 CONNACK. Accept only the exact
    // legal 3.1.1 shape; every near-match remains malformed and cannot downgrade.
    return (buf != nullptr) && (len == 4u) && (llen == 1u) &&
           (buf[0] == MQTTCONNACK) && (buf[1] == 0x02) &&
           (buf[2] == 0x00) && (buf[3] == MQTT_CONNECT_BAD_PROTOCOL);
}

// Validate the complete CONNACK without changing client state. Structural failures are
// MalformedPacket; reserved flags, illegal reason codes, and illegal Session Present
// combinations are ProtocolError. The caller commits the output candidates only after
// this function returns None.
static MqttError parseConnack5(const uint8_t* buf, uint32_t len, uint8_t llen,
                               bool cleanSession, bool hasLocalSessionState,
                               uint8_t& reasonCode, bool& sessionPresent,
                               Connack5Limits& limits) {
    if ((buf == nullptr) || (len == 0)) {
        return MqttError::MalformedPacket;
    }
// Check 1: type 2 with zero fixed-header flags.
    if (buf[0] != MQTTCONNACK) {
        return MqttError::MalformedPacket;
    }
// readPacket() already decoded Remaining Length. Reconstruct its bounded one/two-byte
// value directly so canonical width and equality with the retained packet are both checked
// without paying for a second general VBI decoder call. The equality matters in stream mode,
// where readPacket() may consume more non-PUBLISH bytes than fit in the working buffer.
    if (len < 1u + (uint32_t)llen) {
        return MqttError::MalformedPacket;
    }
    const uint32_t declaredRemaining =
        (uint32_t)(buf[1] & 0x7F) +
        ((llen == 2u) ? ((uint32_t)(buf[2] & 0x7F) << 7) : 0u);
    if ((PubSubClient::variableByteIntegerSize(declaredRemaining) != llen) ||
        (declaredRemaining != len - 1u - (uint32_t)llen)) {
        return MqttError::MalformedPacket;
    }
// First variable-header byte: past the fixed-header byte and the Remaining Length bytes.
    const uint32_t vh = 1u + (uint32_t)llen;
// Check 2: Remaining Length >= 3.
    if (len < vh + 3u) {
        return MqttError::MalformedPacket;
    }

    const uint8_t ackFlags = buf[vh];
    const uint8_t candidateReasonCode = buf[vh + 1];
    const bool candidateSessionPresent = (ackFlags & 0x01) != 0;
    if ((ackFlags & 0xFE) != 0 || !isAllowedConnackReasonCode(candidateReasonCode) ||
        ((candidateReasonCode != 0) && candidateSessionPresent) ||
        (cleanSession && candidateSessionPresent) ||
        (candidateSessionPresent && !hasLocalSessionState)) {
        return MqttError::ProtocolError;
    }

// Check 3: the Properties field, validated and walked in one pass. The visitor collects
// candidate limits into `limits`; no member changes until the caller accepts this result.
    const uint32_t propsOff = vh + 2u;
    uint32_t consumed = 0;
    const MqttError propertyError = decodeProperties(
        buf + propsOff, len - propsOff,
        (uint8_t)(MQTTCONNACK >> 4), connackLimitVisitor, &limits, consumed);
    if (propertyError != MqttError::None) {
        return propertyError;
    }
    if (!limits.propertiesValid) {
        return MqttError::ProtocolError;
    }
// Check 4: nothing after the Properties field.
    if (propsOff + consumed != len) {
        return MqttError::MalformedPacket;
    }

    reasonCode = candidateReasonCode;
    sessionPresent = candidateSessionPresent;
    return MqttError::None;
}

// --- Inbound PUBLISH Properties field (Requirements 6.4, 6.7, 13.4, 13.5) --
//
// MQTT 5.0 inbound PUBLISH layout, as it sits in the working buffer AFTER the existing
// topic shift (`llen` = Remaining Length bytes, `tl` = topic length):
//
//   offset 0            fixed header byte
//   offset 1 .. llen    Remaining Length bytes
//   offset llen+2       topic, shifted back one byte over its own length prefix,
//                       NUL written at llen+2+tl
//   offset h            Packet Identifier (2 bytes) when QoS > 0,  h = llen + 3 + tl
//   offset h (+2)       Properties: <VBI length><TLV body>
//   offset h (+2) + pl  payload, running to `len`
//
// The shift only ever touches bytes at or before llen+2+tl, so the Properties region -
// which starts at `h`, or two bytes later when QoS > 0 - is never disturbed by it.
//
// `propsOff` is `h` (+2 for the Packet Identifier), already validated against `len` by
// the caller's header check. `len` is what readPacket() returned: bytes ACTUALLY
// RECEIVED, never a declared length, so decodeProperties() bounds every declared length
// against real bytes and reads nothing at or beyond buffer[len] (Requirements 13.4,
// 13.5). The payload is then simply what remains: `len - (propsOff + consumed)`, and a
// remainder of 0 is an empty payload rather than an error (Requirement 6.4).
//
// The same single pass that validates the field also collects the values the accessors
// expose (Requirements 11.7, 12.4): `out` is filled by inboundPropertyCollector below,
// which never aborts the walk, so every entry still goes through the Requirement
// 3.6/3.7/3.8 checks. `out` may be null to validate without collecting.
// Correlation Data (Requirements 11.11, 13.7, 13.8) is served from the client's fixed
// Correlation_Data_Storage rather than borrowed from the working buffer, so the
// collector below needs write access to that storage as well as to the
// MqttInboundProperties it fills in. `inboundPropertyCollector` is a free function with
// no access to PubSubClient's private members, so the caller (a PubSubClient method)
// bundles pointers to both into this small context. The common loop code is compiled
// once, and this context is populated only while runtime protocol level 5 is active.
struct InboundPropertyCollectorCtx {
    MqttInboundProperties* props              = nullptr;
    uint8_t*                correlationStorage = nullptr;   // client->correlationDataStorage
    uint8_t*                correlationLen     = nullptr;   // &client->correlationDataLen
    bool*                   hasCorrelation     = nullptr;   // &client->hasCorrelationData
};

// Copy the properties the Public_API exposes out of each decoded entry.
//
// User Property entries are COUNTED here and not stored: there is no fixed-size cache
// to overflow, and forEachInboundUserProperty() re-walks the same region to enumerate
// them in received order (Requirement 11.4). The count saturates at 255 rather than
// wrapping, so a packet carrying more than that reports 255 while the walk still visits
// every one of them.
//
// Views point into the caller's buffer - the working buffer, here - so nothing is
// copied and nothing is allocated (Requirement 13.1). A zero-length value yields a
// non-null one-past-the-value pointer, so present() reports it as received.
//
// The length check against MQTT_CORRELATION_DATA_MAX guards the Correlation Data copy
// itself so an over-long declared value can never overrun `correlationStorage` - the
// fixed-size destination is a hard bound regardless of what the wire declares. This is
// now belt-and-suspenders: decodeProperties() itself already rejects a Correlation Data
// entry whose declared length exceeds the cap as a ProtocolError before any visitor -
// this collector included - is ever invoked with it (Requirements 3.10, 11.11, 11.12,
// 13.7, 13.8), so the branch below is never reached with `prop.len` above the cap. It
// stays as the collector's own defense in depth rather than being removed.
static bool inboundPropertyCollector(const MqttPropertyView& prop, void* ctx) {
    InboundPropertyCollectorCtx* c = static_cast<InboundPropertyCollectorCtx*>(ctx);
    if ((c == nullptr) || (c->props == nullptr)) {
        return true;
    }
    MqttInboundProperties* out = c->props;
    switch (prop.id) {
        case MQTT_PROP_PAYLOAD_FORMAT_INDICATOR:
            out->hasPayloadFormat = true;
            out->payloadFormat = (uint8_t)prop.value;
            break;
        case MQTT_PROP_CONTENT_TYPE:
            out->contentType.data = (const char*)prop.data;
            out->contentType.len  = prop.len;
            break;
        case MQTT_PROP_RESPONSE_TOPIC:
            out->responseTopic.data = (const char*)prop.data;
            out->responseTopic.len  = prop.len;
            break;
        case MQTT_PROP_CORRELATION_DATA:
            if ((c->correlationStorage != nullptr) && (c->correlationLen != nullptr) &&
                (c->hasCorrelation != nullptr) && (prop.len <= PubSubClient::MQTT_CORRELATION_DATA_MAX)) {
                memcpy(c->correlationStorage, prop.data, prop.len);
                *c->correlationLen = (uint8_t)prop.len;
                *c->hasCorrelation = true;
                out->hasCorrelationData = true;
                out->correlationData    = c->correlationStorage;
                out->correlationDataLen = (uint8_t)prop.len;
            }
            break;
        case MQTT_PROP_TOPIC_ALIAS:
            out->topicAlias = (uint16_t)prop.value;
            break;
        case MQTT_PROP_USER_PROPERTY:
            if (out->userPropertyCount < 255) {
                out->userPropertyCount++;
            }
            break;
        default:
            break;                                      // not exposed by the accessors
    }
    return true;
}

// `outReasonCode`, when the return is `ProtocolError`, carries the specific DISCONNECT
// Reason Code the caller should pass to `protocolFailure()` INSTEAD OF the generic
// category default - set only for the Enhanced Authentication rejection (0x8C rather
// than 0x82), and left untouched for every other outcome. See decodeProperties()'s own
// `outReasonCode` parameter, which this simply forwards (Requirements 3.11, 16.5, 16.6).
static MqttError parsePublishProperties5(const uint8_t* buf, uint32_t len,
                                         uint32_t propsOff, InboundPropertyCollectorCtx* out,
                                         uint32_t& consumed, uint8_t* outReasonCode) {
    consumed = 0;
    if ((buf == nullptr) || (propsOff >= len)) {
// A PUBLISH whose bytes run out before the Properties field starts is malformed: the
// field is never optional in 5.0, an empty set is still the byte 0x00. `>=` rather than
// `>` because at least that one byte must be there.
        return MqttError::MalformedPacket;
    }
    return decodeProperties(buf + propsOff, len - propsOff,
                            (uint8_t)(MQTTPUBLISH >> 4),   // packet type number = 3
                            inboundPropertyCollector, out, consumed, outReasonCode);
}

// --- Acknowledgement parser (Requirements 7.3, 7.5, 7.9) -------------------
//
// The single parser for all four acknowledgement packet types. MQTT 5.0 defines three
// forms, and this accepts every one of them (Requirement 7.5):
//
//   Remaining Length 2 : PacketIdentifier(2)                            short form
//   Remaining Length 3 : PacketIdentifier(2) | ReasonCode(1)            mid form
//   Remaining Length >3: PacketIdentifier(2) | ReasonCode(1) | Properties
//
// An ABSENT Reason Code reads back as 0x00 (Success), which is what the specification
// says the short form means (Requirements 7.3, 7.5) - so a caller never has to know
// which form arrived.
//
// `len` is what readPacket() returned: bytes ACTUALLY RECEIVED, never a declared length.
// The form is therefore derived from the bytes that are really there, which makes every
// read bounds-checked by construction: no offset is touched before `len` has been shown
// to cover it, and nothing at or beyond buf[len] is ever read (Requirements 13.4, 13.5).
//
// Anything that does not fit one of the three forms is malformed (Requirement 7.9): a
// Remaining Length below 2, a Properties field that fails to decode, or a Properties
// field that ends short of the packet's last byte - the field is the final one in an
// acknowledgement, so trailing bytes after it cannot be accounted for.
//
// `outReasonCode` forwards decodeProperties()'s override for the one failure that
// carries a more specific DISCONNECT Reason Code than its category - Authentication
// Method / Authentication Data, 0x8C rather than 0x82 (Requirements 3.11, 16.5, 16.6).
static MqttError parseAcknowledgement5(const uint8_t* buf, uint32_t len, uint8_t llen,
                                       uint8_t packetTypeNumber, uint16_t& packetId,
                                       uint8_t& reasonCode, uint8_t* outReasonCode) {
    packetId = 0;
    reasonCode = 0;                       // the short form's implied Success
    if (buf == nullptr) {
        return MqttError::MalformedPacket;
    }
    const uint32_t off = (uint32_t)llen + 1;          // start of the variable header
    if (len < off + 2) {
        return MqttError::MalformedPacket;            // Remaining Length below 2
    }
    packetId = (uint16_t)(((uint16_t)buf[off] << 8) | (uint16_t)buf[off + 1]);
    uint32_t pos = off + 2;
    if (pos == len) {
        return MqttError::None;                       // short form
    }
    reasonCode = buf[pos++];
    if (pos == len) {
        return MqttError::None;                       // mid form, no Properties field
    }
    uint32_t consumed = 0;
    const MqttError err = decodeProperties(buf + pos, len - pos, packetTypeNumber,
                                           nullptr, nullptr, consumed, outReasonCode);
    if (err != MqttError::None) {
        return err;
    }
    if (pos + consumed != len) {
// The Properties field is the last field of an acknowledgement, so bytes after it belong
// to no field at all.
        return MqttError::MalformedPacket;
    }
    return MqttError::None;
}

// --- SUBACK / UNSUBACK parser (Requirements 9.4, 9.5, 9.6) -----------------
//
// Both packet types share one layout:
//
//   PacketIdentifier(2) | Properties | ReasonCode(1)+
//
// The Reason Code list is the payload, and it runs to the end of the packet. Its LENGTH
// IS VALIDATED AGAINST THE RECEIVED BYTES rather than taken from a declared field: `len`
// is what readPacket() returned - bytes ACTUALLY RECEIVED - so `rcCount` is derived from
// real bytes and every read is bounds-checked by construction (Requirements 13.4, 13.5).
// At least one Reason Code must be present; a list of zero entries is malformed, because
// the specification requires one entry per Topic Filter of the request.
//
// `rcOff` is the offset of the first Reason Code within `buf`, so the caller reports
// straight out of the working buffer with nothing copied.
//
// `outReasonCode` forwards decodeProperties()'s override for the one failure that carries
// a more specific DISCONNECT Reason Code than its category - Authentication Method /
// Authentication Data, 0x8C rather than 0x82 (Requirements 3.11, 16.5, 16.6).
static MqttError parseSubscriptionAck5(const uint8_t* buf, uint32_t len, uint8_t llen,
                                       uint8_t packetTypeNumber, uint16_t& packetId,
                                       uint32_t& rcOff, uint32_t& rcCount,
                                       uint8_t* outReasonCode) {
    packetId = 0;
    rcOff = 0;
    rcCount = 0;
    if (buf == nullptr) {
        return MqttError::MalformedPacket;
    }
    const uint32_t off = (uint32_t)llen + 1;          // start of the variable header
// Packet Identifier (2) + at least one Properties byte + at least one Reason Code.
    if (len < off + 4) {
        return MqttError::MalformedPacket;
    }
    packetId = (uint16_t)(((uint16_t)buf[off] << 8) | (uint16_t)buf[off + 1]);
    const uint32_t propsOff = off + 2;
    uint32_t consumed = 0;
    const MqttError err = decodeProperties(buf + propsOff, len - propsOff, packetTypeNumber,
                                           nullptr, nullptr, consumed, outReasonCode);
    if (err != MqttError::None) {
        return err;
    }
    const uint32_t listOff = propsOff + consumed;
// decodeProperties() bounded the field against the received bytes, so listOff <= len.
// Equality means the packet carried no Reason Code at all.
    if (listOff >= len) {
        return MqttError::MalformedPacket;
    }
    rcOff = listOff;
    rcCount = len - listOff;
    return MqttError::None;
}

// --- Inbound DISCONNECT parser (Requirements 10.1, 10.2) -------------------
//
// Three forms, all accepted:
//
//   Remaining Length 0 : (nothing)                     Reason Code implied 0x00
//   Remaining Length 1 : ReasonCode(1)
//   Remaining Length >1: ReasonCode(1) | Properties
//
// An ABSENT Reason Code reads back as 0x00 (Normal disconnection), so a caller never has
// to know which form arrived. As everywhere else, the form is derived from `len` - bytes
// actually received - so no offset is touched before `len` has been shown to cover it.
// The Properties field is the last field, so bytes after it belong to no field at all.
static MqttError parseServerDisconnect5(const uint8_t* buf, uint32_t len, uint8_t llen,
                                        uint8_t& reasonCode, uint8_t* outReasonCode) {
    reasonCode = 0;                       // the short form's implied Normal disconnection
    if (buf == nullptr) {
        return MqttError::MalformedPacket;
    }
    const uint32_t off = (uint32_t)llen + 1;
    if (len < off) {
        return MqttError::MalformedPacket;
    }
    if (len == off) {
        return MqttError::None;                       // short form
    }
    reasonCode = buf[off];
    const uint32_t propsOff = off + 1;
    if (propsOff == len) {
        return MqttError::None;                       // Reason Code only
    }
    uint32_t consumed = 0;
    const MqttError err = decodeProperties(buf + propsOff, len - propsOff,
                                           (uint8_t)(MQTTDISCONNECT >> 4),   // 14
                                           nullptr, nullptr, consumed, outReasonCode);
    if (err != MqttError::None) {
        return err;
    }
    if (propsOff + consumed != len) {
        return MqttError::MalformedPacket;
    }
    return MqttError::None;
}

// --- Inbound MQTT 5 semantic validation (Requirement 18) -----------------
//
// This deliberately runs before loop() masks the fixed header to a packet type or
// calls a state-changing dispatcher. The rules and Reason Code allowlists are compact
// flash-resident tables on ESP8266; validation itself uses only scalar locals and the
// existing zero-copy property decoder.
#if defined(ESP8266)
#define MQTT_SEMANTIC_TABLE_STORAGE PROGMEM
#else
#define MQTT_SEMANTIC_TABLE_STORAGE
#endif

// The admission table is indexed directly by the high nibble. 0xFF means
// client-originated/reserved; 0xFE delegates PUBLISH's variable flags to its compact
// QoS/DUP rule. On ESP8266 this costs 16 bytes of flash instead of a searched table.
static const uint8_t MQTT_INBOUND_FLAGS[16] MQTT_SEMANTIC_TABLE_STORAGE = {
    0xFF, 0xFF, 0x00, 0xFE, 0x00, 0x00, 0x02, 0x00,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00
};

static const uint8_t MQTT_RC_PUBLISH_ACK[] MQTT_SEMANTIC_TABLE_STORAGE = {
    0x00, 0x10, 0x80, 0x83, 0x87, 0x90, 0x91, 0x97, 0x99
};
static const uint8_t MQTT_RC_PUBREL_COMP[] MQTT_SEMANTIC_TABLE_STORAGE = {
    0x00, 0x92
};
static const uint8_t MQTT_RC_SUBACK[] MQTT_SEMANTIC_TABLE_STORAGE = {
    0x00, 0x01, 0x02, 0x80, 0x83, 0x87, 0x8F, 0x91, 0x97, 0x9E, 0xA1, 0xA2
};
static const uint8_t MQTT_RC_UNSUBACK[] MQTT_SEMANTIC_TABLE_STORAGE = {
    0x00, 0x11, 0x80, 0x83, 0x87, 0x8F, 0x91
};
static const uint8_t MQTT_RC_DISCONNECT[] MQTT_SEMANTIC_TABLE_STORAGE = {
    0x00, 0x04, 0x80, 0x81, 0x82, 0x83, 0x87, 0x89, 0x8B, 0x8D, 0x8E, 0x8F,
    0x90, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D,
    0x9E, 0x9F, 0xA0, 0xA1, 0xA2
};

static bool mqtt5TableByteEquals(const uint8_t* table, size_t index, uint8_t value) {
#if defined(ESP8266)
    return pgm_read_byte(table + index) == value;
#else
    return table[index] == value;
#endif
}

static bool mqtt5AllowsReasonCode(const uint8_t* table, size_t count, uint8_t value) {
    for (size_t i = 0; i < count; ++i) {
        if (mqtt5TableByteEquals(table, i, value)) {
            return true;
        }
    }
    return false;
}

static uint8_t mqtt5InboundFlags(uint8_t packetType) {
#if defined(ESP8266)
    return pgm_read_byte(&MQTT_INBOUND_FLAGS[packetType]);
#else
    return MQTT_INBOUND_FLAGS[packetType];
#endif
}

static bool mqtt5AllowsAcknowledgementReason(uint8_t packetType, uint8_t reasonCode) {
    switch (packetType) {
        case (uint8_t)(MQTTPUBACK >> 4):
        case (uint8_t)(MQTTPUBREC >> 4):
            return mqtt5AllowsReasonCode(MQTT_RC_PUBLISH_ACK,
                                         sizeof(MQTT_RC_PUBLISH_ACK), reasonCode);
        case (uint8_t)(MQTTPUBREL >> 4):
        case (uint8_t)(MQTTPUBCOMP >> 4):
            return mqtt5AllowsReasonCode(MQTT_RC_PUBREL_COMP,
                                         sizeof(MQTT_RC_PUBREL_COMP), reasonCode);
        default:
            return false;
    }
}

static bool mqtt5AllowsSubscriptionReason(uint8_t packetType, uint8_t reasonCode) {
    return (packetType == (uint8_t)(MQTTSUBACK >> 4))
        ? mqtt5AllowsReasonCode(MQTT_RC_SUBACK, sizeof(MQTT_RC_SUBACK), reasonCode)
        : mqtt5AllowsReasonCode(MQTT_RC_UNSUBACK, sizeof(MQTT_RC_UNSUBACK), reasonCode);
}

static bool mqtt5AllowsDisconnectReason(uint8_t reasonCode) {
    return mqtt5AllowsReasonCode(MQTT_RC_DISCONNECT, sizeof(MQTT_RC_DISCONNECT), reasonCode);
}

// Admission is deliberately limited to values that can be checked without parsing a
// packet-specific payload. The existing packet parsers below validate identifiers,
// properties, Reason Codes, and SUBACK cardinality exactly once, immediately before their
// dispatchers mutate protocol state.
static MqttError validateInboundPacket5(const uint8_t* buf, uint32_t len, uint8_t llen,
                                        uint8_t* outReasonCode) {
    if (outReasonCode != nullptr) {
        *outReasonCode = 0;
    }
    if ((buf == nullptr) || (len < 1u + (uint32_t)llen)) {
        return MqttError::MalformedPacket;
    }

    const uint8_t packetType = (uint8_t)(buf[0] >> 4);
    const uint8_t flags = (uint8_t)(buf[0] & 0x0F);
    const uint8_t requiredFlags = mqtt5InboundFlags(packetType);
    if (requiredFlags == 0xFF) {
        return MqttError::ProtocolError;
    }
    if (requiredFlags == 0xFE) {
        const uint8_t qos = (uint8_t)((flags >> 1) & 0x03);
        return ((qos == 3) || ((qos == 0) && ((flags & 0x08) != 0)))
            ? MqttError::MalformedPacket : MqttError::None;
    }
    if (flags != requiredFlags) {
        return MqttError::MalformedPacket;
    }

    const uint32_t variableHeader = 1u + (uint32_t)llen;
    if (packetType == (uint8_t)(MQTTCONNACK >> 4)) {
        // loop() is reachable only after a successful CONNACK, so any further one is
        // invalid rather than a connection outcome to be parsed again.
        return MqttError::ProtocolError;
    }
    if (packetType == (uint8_t)(MQTTPINGRESP >> 4)) {
        return (len == variableHeader) ? MqttError::None : MqttError::MalformedPacket;
    }
    if (packetType == (uint8_t)(MQTTAUTH >> 4)) {
        if (outReasonCode != nullptr) {
            *outReasonCode = PubSubClient::MQTT_RC_BAD_AUTH_METHOD;
        }
        return MqttError::ProtocolError;
    }
    return MqttError::None;
}

#undef MQTT_SEMANTIC_TABLE_STORAGE

// --- Inbound Property enumeration -----------------------------------------
struct InboundPropertyWalk {
    MqttPropertyVisitor visit = nullptr;
    void*               ctx = nullptr;
    bool                aborted = false;
};

static bool inboundPropertyVisitor(const MqttPropertyView& prop, void* ctx) {
    InboundPropertyWalk* walk = static_cast<InboundPropertyWalk*>(ctx);
    if ((walk == nullptr) || (walk->visit == nullptr)) {
        return false;
    }
    if (!walk->visit(prop, walk->ctx)) {
        walk->aborted = true;
        return false;
    }
    return true;
}

bool PubSubClient::forEachInboundProperty(MqttPropertyVisitor visit, void* ctx) const {
    if ((visit == nullptr) || !this->inboundProps.valid ||
        (this->buffer == nullptr) || (this->inboundPropsLen == 0) ||
        ((uint32_t)this->inboundPropsOff + (uint32_t)this->inboundPropsLen >
         (uint32_t)this->bufferSize)) {
        return false;
    }
    InboundPropertyWalk walk;
    walk.visit = visit;
    walk.ctx = ctx;
    uint32_t consumed = 0;
    const MqttError error = decodeProperties(
        this->buffer + this->inboundPropsOff, this->inboundPropsLen,
        (uint8_t)(MQTTPUBLISH >> 4), inboundPropertyVisitor, &walk, consumed);
    return (error == MqttError::None) && !walk.aborted &&
           (consumed == this->inboundPropsLen);
}

//
// The caller's visitor plus its context, carried through decodeProperties()'s own
// visitor as the opaque ctx. `aborted` is what distinguishes a walk the visitor stopped
// from one that ran out of entries, since decodeProperties() reports both as success.
struct InboundUserPropertyWalk {
    MqttUserPropertyVisitor visit = nullptr;
    void*                   ctx   = nullptr;
    bool                    aborted = false;
};

// Forward every User Property entry to the caller's visitor and skip everything else.
// Returning false here aborts decodeProperties()'s walk, which is how the caller's
// `false` propagates without unwinding through a return value the reader does not have.
static bool inboundUserPropertyVisitor(const MqttPropertyView& prop, void* ctx) {
    InboundUserPropertyWalk* walk = static_cast<InboundUserPropertyWalk*>(ctx);
    if ((walk == nullptr) || (walk->visit == nullptr)) {
        return false;
    }
    if (prop.id != MQTT_PROP_USER_PROPERTY) {
        return true;
    }
    MqttStringView key;
    key.data = (const char*)prop.data;
    key.len  = prop.len;
    MqttStringView value;
    value.data = (const char*)prop.data2;
    value.len  = prop.len2;
    if (!walk->visit(key, value, walk->ctx)) {
        walk->aborted = true;
        return false;
    }
    return true;
}

// Re-walk `buf[0 .. avail-1]` - the recorded Properties field of the last inbound
// PUBLISH - and hand each User Property to `visit` in received order. The region parsed
// cleanly once already, so a decode failure here would mean the buffer moved underneath
// us; it is reported as false rather than treated as a protocol failure, because no new
// bytes have been received and the connection is not at fault.
static bool walkInboundUserProperties5(const uint8_t* buf, uint32_t avail,
                                       MqttUserPropertyVisitor visit, void* ctx) {
    InboundUserPropertyWalk walk;
    walk.visit = visit;
    walk.ctx   = ctx;
    uint32_t consumed = 0;
    if (decodeProperties(buf, avail, (uint8_t)(MQTTPUBLISH >> 4),
                         inboundUserPropertyVisitor, &walk, consumed) != MqttError::None) {
        return false;
    }
    return !walk.aborted;
}

bool PubSubClient::forEachInboundUserProperty(MqttUserPropertyVisitor visit, void* ctx) const {
    if (!MQTT_RUNTIME_IS_5()) {
        return false;
    }
    if ((visit == nullptr) || !this->inboundProps.valid ||
        (this->buffer == nullptr) || (this->inboundPropsLen == 0)) {
        return false;
    }
// Re-validate the recorded extent against the CURRENT buffer: a setBufferSize() between
// the callback and this call reallocates the buffer, so the offset is only usable while
// it still lies inside it.
    if ((uint32_t)this->inboundPropsOff + (uint32_t)this->inboundPropsLen >
        (uint32_t)this->bufferSize) {
        return false;
    }
    return walkInboundUserProperties5(this->buffer + this->inboundPropsOff,
                                      this->inboundPropsLen, visit, ctx);
}

// Map a failing CONNACK Reason Code (>= 0x80) onto the legacy state() numbering, per
// the design's table, so the driver's existing diagnostic switch keeps producing
// meaningful values. The raw byte stays available through connackReasonCode(), which
// is what distinguishes 0x88 from 0x89 once both have collapsed onto 3.
static int connackFailureState(uint8_t reasonCode) {
    switch (reasonCode) {
        case 0x84: return MQTT_CONNECT_BAD_PROTOCOL;      //  1  Unsupported Protocol Version
        case 0x85: return MQTT_CONNECT_BAD_CLIENT_ID;     //  2  Client Identifier not valid
        case 0x86: return MQTT_CONNECT_BAD_CREDENTIALS;   //  4  Bad User Name or Password
        case 0x87: return MQTT_CONNECT_UNAUTHORIZED;      //  5  Not authorized
        case 0x88:                                        //     Server unavailable
        case 0x89: return MQTT_CONNECT_UNAVAILABLE;       //  3  Server busy
        default:   return MQTT_CONNECT_FAILED;            // -2  any other >= 0x80
    }
}

void PubSubClient::selectMqtt311Fallback() {
    // Clear the identity first so the common teardown cannot preserve MQTT 5 Session
    // State. Run it while level 5 is still selected, then switch the one-byte selector.
    if (_client != nullptr) {
        _client->stop();
    }
    clearSessionIdentity();
    resetConnectionState();
    resetInboundProperties();
    this->lastSubResult = MqttSubscriptionResult();
    this->sessionIdentity.protocolLevel = MQTT_VERSION_3_1_1;
    resetErrorState();
    resetConnackState();
}

boolean PubSubClient::connect(const char *id) {
    return connect(id,NULL,NULL,0,0,0,0,1);
}

boolean PubSubClient::connect(const char *id, const char *user, const char *pass) {
    return connect(id,user,pass,0,0,0,0,1);
}

boolean PubSubClient::connect(const char *id, const char* willTopic, uint8_t willQos, boolean willRetain, const char* willMessage) {
    return connect(id,NULL,NULL,willTopic,willQos,willRetain,willMessage,1);
}

boolean PubSubClient::connect(const char *id, const char *user, const char *pass, const char* willTopic, uint8_t willQos, boolean willRetain, const char* willMessage) {
    return connect(id,user,pass,willTopic,willQos,willRetain,willMessage,1);
}

boolean PubSubClient::connect(const char *id, const char *user, const char *pass, const char* willTopic, uint8_t willQos, boolean willRetain, const char* willMessage, boolean cleanSession) {
    if (!connected()) {
        // Requirement 14.2: the error diagnostics describe the current connection
        // attempt only, so they are cleared before anything else can set them.
        resetErrorState();
        // Requirement 1.6: "readable until the next connection attempt begins" - this
        // is that boundary, and the only place the CONNACK outcome is cleared. Note it
        // is cleared here rather than in resetConnectionState(): the latter also runs
        // on disconnect() and on protocolFailure(), and clearing it there would make
        // the value unreadable from the moment the connection ended.
        resetConnackState();

        // The loop can continue only once: selectMqtt311Fallback() changes the effective
        // level to 4, and no level-4 response is eligible to enter either fallback branch.
        for (;;) {
        const bool mqtt5 = MQTT_RUNTIME_IS_5();
        if (mqtt5) {
// Requirement 5.7: every negotiated limit goes back to its MQTT 5.0 default before the
// attempt starts, so nothing the previous broker declared can govern this connection -
// and, for Server Keep Alive specifically, so the CONNECT below requests the interval
// setKeepAlive() was given rather than whatever the previous broker imposed.
            resetNegotiatedLimits();
            clearTopicAliases();
// The subscription outcome describes the connection it was reported on, so it never
// crosses a CONNECT boundary.
            this->lastSubResult = MqttSubscriptionResult();

// Task 18.7: only the exact persistent-session identity may carry local Session State
// across the next CONNECT. Clean Start, a changed endpoint/port/client/protocol/policy,
// or an initial persistent connect clears every old entry before this CONNECT can reach a
// peer. An unchanged persistent identity keeps all held operations for CONNACK's Session
// Present decision; only replayable publish states are emitted again if it reports 1.
            const bool preserveResumable =
                !cleanSession && this->sessionIdentityMatches(id, cleanSession);
            if (!preserveResumable) {
                clearOutstanding(false);
                clearInboundQos2();
                clearSessionIdentity();
            } else {
                clearOutstanding();
            }
        }

        int result = 0;

        if (_client == nullptr) {
            return false;
        }
        // Without a working buffer the CONNECT packet below would be assembled through
        // a null pointer. bufferSize == 0 means the allocation failed.
        if ((this->buffer == nullptr) || (this->bufferSize == 0)) {
            return false;
        }

        // A null client id would crash the later strnlen()/writeString(). MQTT allows a
        // zero-length id (with clean session) but not a null pointer.
        if (id == nullptr) {
            return false;
        }
        // Reject a Will with an out-of-range QoS before it is shifted into the CONNECT
        // flags byte, and a Will topic without a message.
        if (willTopic != nullptr) {
            if (willQos > 2 || willMessage == nullptr) {
                return false;
            }
        }

        if (_client->connected()) {
            result = 1;

// port == 0 means no server was configured, or setServer() failed to keep the host
// name (out of memory). Connecting anyway would target an unintended endpoint.
        } else if (this->port == 0) {
            _state = MQTT_CONNECT_FAILED;
            return false;

        } else {
            if (domain.length() != 0) {
                result = _client->connect(this->domain.c_str(), this->port);

            } else {
                result = _client->connect(this->ip, this->port);
            }
        }

        if (result == 1) {
            // `setBufferSize()` remains source-compatible for small allocations, but an
            // empty CONNECT needs this much assembly storage. Check after opening the
            // transport so the just-opened socket is closed on refusal, and before any
            // working-buffer byte is written.
            // A persistent MQTT 5 CONNECT adds Session Expiry Interval (identifier plus
            // four-byte value) to the otherwise empty CONNECT Properties field.
            const uint16_t minimumConnectBuffer =
                mqtt5
                    ? (uint16_t)(MQTT_CONNECT_MINIMUM_BUFFER_SIZE_5_0 +
                                 (!cleanSession ? 5u : 0u))
                    : (uint16_t)MQTT_CONNECT_MINIMUM_BUFFER_SIZE_3_1_1;
            if (this->bufferSize < minimumConnectBuffer) {
                this->errorState = MqttError::BufferTooSmall;
                _client->stop();
                return false;
            }

            // Leave room in the buffer for the fixed header and Remaining Length.
            uint16_t length = MQTT_MAX_HEADER_SIZE;
            const uint8_t protocolHeader[7] = {
                0x00, 0x04, 'M', 'Q', 'T', 'T',
                (uint8_t)(MQTT_VERSION_3_1_1 + (mqtt5 ? 1u : 0u))
            };
            if (!appendConnectBytes(this->buffer, this->bufferSize, length,
                                    protocolHeader, sizeof(protocolHeader))) {
                this->errorState = MqttError::BufferTooSmall;
                _client->stop();
                return false;
            }

            uint8_t v;
            if (willTopic) {
                v = 0x04|(willQos<<3)|(willRetain<<5);
            } else {
                v = 0x00;
            }
            if (cleanSession) {
                v = v|0x02;
            }

// Requirement 16.2: an empty configured User Name - a null pointer OR a zero-length
// string - omits both the User Name and the Password field and clears both flag bits.
// This applies at both runtime protocol levels, deliberately: it is Appendix B entry 1, the one
// enumerated wire deviation from the pre-migration 3.1.1 library, which set bit 7 and
// emitted a zero-length User Name string for user == "". A zero-length User Name is
// meaningless on the wire and a strict broker may reject it, so do NOT "restore" the
// old behavior here - it was retired on purpose, and one credential rule across both
// runtime protocol levels is worth the single divergence.
            const boolean hasUser = (user != NULL) && (user[0] != '\0');

            if (hasUser) {
                v = v|0x80;

                if (pass != NULL) {
                    v = v|(0x80>>1);
                }
            }
            if (!appendConnectByte(this->buffer, this->bufferSize, length, v)) {
                this->errorState = MqttError::BufferTooSmall;
                _client->stop();
                return false;
            }

            const uint8_t keepAliveBytes[2] = {
                (uint8_t)(this->keepAlive >> 8),
                (uint8_t)(this->keepAlive & 0xFF)
            };
            if (!appendConnectBytes(this->buffer, this->bufferSize, length,
                                    keepAliveBytes, sizeof(keepAliveBytes))) {
                this->errorState = MqttError::BufferTooSmall;
                _client->stop();
                return false;
            }

            if (mqtt5) {
// Requirement 1.2: the CONNECT variable header ends with a Properties field, immediately
// after the keepalive bytes. The two-pass property writer first proves the complete field
// fits, so this append either advances `length` by the whole field or leaves it unchanged.
                uint32_t maxPacket = (this->maxIncomingPacketSize != 0)
                                        ? this->maxIncomingPacketSize
                                        : (uint32_t)(this->bufferSize - 1);
                // Buffered callbacks need one spare byte at buffer[len] so consumers
                // such as Tasmota can safely NUL-terminate the delivered payload.
                if (!this->stream && maxPacket >= (uint32_t)this->bufferSize) {
                    maxPacket = (uint32_t)this->bufferSize - 1;
                }
                if (!appendConnectProperties((uint16_t)MQTT_MAX_INBOUND_INFLIGHT,
                                             maxPacket, !cleanSession,
                                             this->persistentSessionExpiryInterval,
                                             this->buffer, this->bufferSize, length)) {
                    this->errorState = MqttError::BufferTooSmall;
                    _client->stop();
                    return false;
                }
            }

            if (!appendConnectString(this->buffer, this->bufferSize, length, id)) {
                this->errorState = MqttError::BufferTooSmall;
                _client->stop();
                return false;
            }
            if (willTopic) {
                if (mqtt5) {
// Requirement 1.3: the Will Properties field lives in the PAYLOAD, immediately before
// the Will Topic - not in the variable header. Every Will-related MQTT 5.0 property
// (Will Delay Interval, Message Expiry Interval) is an explicit non-goal, so the field
// is always the empty set, encoded as the single length byte 0x00.
                    if (!appendConnectByte(this->buffer, this->bufferSize, length, 0x00)) {
                        this->errorState = MqttError::BufferTooSmall;
                        _client->stop();
                        return false;
                    }
                }
                if (!appendConnectString(this->buffer, this->bufferSize, length, willTopic) ||
                    !appendConnectString(this->buffer, this->bufferSize, length, willMessage)) {
                    this->errorState = MqttError::BufferTooSmall;
                    _client->stop();
                    return false;
                }
            }

// Requirement 16.2 / Appendix B entry 1: mirrors the flag decision above, so the payload
// and the flags byte can never disagree. An empty user name emits neither field, and the
// Password is omitted with it even when one was supplied.
            if (hasUser) {
                if (!appendConnectString(this->buffer, this->bufferSize, length, user) ||
                    ((pass != NULL) &&
                     !appendConnectString(this->buffer, this->bufferSize, length, pass))) {
                    this->errorState = MqttError::BufferTooSmall;
                    _client->stop();
                    return false;
                }
            }

            if (mqtt5) {
// Remaining Length gate: the assembled body must be encodable as a 2-byte Variable Byte
// Integer. write() would refuse it anyway and transmit nothing, but then this function
// would go on to wait out the CONNACK timeout on a CONNECT that never left. Refuse here
// instead, with the failure recorded and the connection state unchanged.
                if ((uint32_t)(length - MQTT_MAX_HEADER_SIZE) > MQTT_VBI_MAX) {
                    this->errorState = MqttError::PacketTooLarge;
                    _client->stop();
                    return false;
                }
            }

            // No state that identifies an exchange changes until every CONNECT field
            // has been assembled successfully.
            nextMsgId = 1;
            write(MQTTCONNECT,this->buffer,length-MQTT_MAX_HEADER_SIZE);

            lastInActivity = lastOutActivity = millis();

            while (!_client->available()) {
                delay(0);  // Prevent watchdog crashes

                unsigned long t = millis();
                if (t-lastInActivity >= ((int32_t) this->socketTimeout*1000UL)) {
                    _state = MQTT_CONNECTION_TIMEOUT;
                    _client->stop();
                    return false;
                }
            }
            uint8_t llen;
            uint32_t len = readPacket(&llen);

            if (mqtt5 &&
                isMqtt311FallbackResponse(this->buffer, len, llen)) {
                selectMqtt311Fallback();
                continue;
            }

            if (mqtt5) {
// Requirements 1.4, 1.5, 1.6. See parseConnack5() above for the acceptance test that
// replaces the 3.1.1 one, and connackFailureState() for the Reason Code mapping.
                uint8_t reasonCode = 0;
                bool present = false;
                Connack5Limits limits;
                const bool resumableLocalState =
                    this->hasResumableLocalSessionState(id, cleanSession);
                const MqttError connackError = parseConnack5(
                    this->buffer, len, llen, cleanSession, resumableLocalState,
                    reasonCode, present, limits);
                if (connackError != MqttError::None) {
// No MQTT session exists yet, so an invalid CONNACK is closed without sending a
// DISCONNECT. Candidates remained local to parseConnack5(), so no limits, outcome,
// outstanding-state disposition, or replay state has been changed.
                    this->errorState = connackError;
                    _state = MQTT_CONNECT_FAILED;
                } else if (reasonCode != 0x00) {
                    // A fully validated Unsupported Protocol Version refusal is the only
                    // MQTT 5 response that authorizes the one fresh 3.1.1 attempt.
                    if (reasonCode == 0x84) {
                        selectMqtt311Fallback();
                        continue;
                    }
// This is a structurally and semantically valid refusal, not a successful session.
// Preserve the broker's allowed diagnostic byte, but do not apply its limits or touch
// the retained local state; the shared close below ends the transport.
                    this->connackRc = reasonCode;
                    _state = connackFailureState(reasonCode);
                } else {
// The only successful CONNACK Reason Code is 0x00. Commit every candidate only now,
// after fixed header, Ack Flags, reason code, Session Present, local state, and
// Properties validation have all completed.
                    this->sessionPresentFlag = present;
                    this->connackRc = reasonCode;
                    if (limits.hasKeepAlive) {
                        this->keepAlive = limits.keepAlive;
                    }
                    this->serverMaxPacketSize = limits.maxPacketSize;
                    this->serverTopicAliasMax = limits.topicAliasMax;
                    this->serverReceiveMaximum = limits.receiveMaximum;

// Session Present 0 definitively says the broker has no compatible session. This is
// the first state-disposition point, after the complete successful CONNACK committed.
                    if (!present) {
                        discardRetransmittable();
                        clearInboundQos2();
                    }

                    lastInActivity = millis();
                    pingOutstanding = false;
                    _state = MQTT_CONNECTED;
// Task 18.9: retransmission is part of connection establishment. Keep the prior
// identity intact while replay verifies each entry belongs to this persistent session;
// replace it with this successful connection's identity only after every replay writes.
// Without USE_MQTT_QOS there is no replayable outbound state (SUBSCRIBE/UNSUBSCRIBE are
// never resent), so Session Present 1 needs no replay pass.
#ifdef USE_MQTT_QOS
                    if (present && !retransmitOutstanding(id, cleanSession)) {
                        return false;
                    }
#endif  // USE_MQTT_QOS
// A successful persistent CONNECT now owns local session state with the exact identity
// used on the wire. Clean Start deliberately stores no resumable identity.
                    captureSessionIdentity(id, cleanSession);
                    return connected();
                }
            } else {
// Only accept a well-formed CONNACK: exact packet type (0x20), Remaining Length 2.
// Previously any 4-byte frame ending in 0 was treated as a successful connection.
                if (len == 4 && (buffer[0] == MQTTCONNACK) && (buffer[1] == 2)) {
                    if (buffer[3] == 0) {
                        lastInActivity = millis();
                        pingOutstanding = false;
                        _state = MQTT_CONNECTED;
                        return true;
                    } else {
                        _state = buffer[3];
                    }
                }
            }
            _client->stop();
        } else {
            _state = MQTT_CONNECT_FAILED;
        }
        return false;
        }
    }
    return true;
}

// True while the single deadline governing the packet currently being assembled
// has elapsed. The signed subtraction form remains correct when millis() wraps.
bool PubSubClient::packetDeadlineExpired() const {
    return (this->packetDeadline != 0) &&
           (static_cast<long>(millis() - this->packetDeadline) >= 0);
}

// A receive timeout is transport-local rather than a broker protocol violation:
// no DISCONNECT is added after a partially received packet. Record it, discard every
// per-connection transient and close so that no fragment can be reused next loop().
void PubSubClient::abortPacketRead() {
    this->errorState = MqttError::ReceiveTimeout;
    this->errorReasonCode = 0;
    if (this->_client != nullptr) {
        this->_client->flush();
        this->_client->stop();
    }
    this->resetConnectionState();
}

// A partial write has already put an incomplete MQTT control packet on the wire.
// Sending a DISCONNECT would itself append another packet to that desynchronized
// stream, so this deliberately records and closes silently.
void PubSubClient::transportWriteFailed() {
    this->errorState = MqttError::WriteFailed;
    this->errorReasonCode = 0;
    if (this->_client != nullptr) {
        this->_client->flush();
        this->_client->stop();
    }
    this->resetConnectionState();
}

#ifdef USE_MQTT_DETAILED_LOGGING_BINARY
// --- Raw wire packet logging (USE_MQTT_DETAILED_LOGGING_BINARY) --------------
//
// Report `data[0..len-1]` to the registered callback, if any. Called with the bytes
// about to be written to the transport (`tx == true`) or the complete packet just
// read from it (`tx == false`). Nothing is copied or allocated: the callback must
// consume the bytes within the call.
void PubSubClient::logWirePacket(bool tx, const uint8_t* data, uint32_t len) {
    if ((this->wireLogCb != nullptr) && (data != nullptr) && (len > 0)) {
        this->wireLogCb(tx, data, len);
    }
}

// Single outbound choke point: log the bytes, then hand them to the transport
// unchanged and return exactly what the transport reported. Every former
// `_client->write()` call site in this file now routes through here.
size_t PubSubClient::transportWrite(const uint8_t* buf, size_t size) {
    logWirePacket(true, buf, (uint32_t)size);
    return this->_client->write(buf, size);
}

size_t PubSubClient::transportWrite(uint8_t b) {
    logWirePacket(true, &b, 1);
    return this->_client->write(b);
}
#endif  // USE_MQTT_DETAILED_LOGGING_BINARY

// reads a byte into result
boolean PubSubClient::readByte(uint8_t * result) {
   if (_client == nullptr) {
     return false;
   }

   uint32_t previousMillis = millis();
   while (!_client->available()) {
     if (packetDeadlineExpired()) {
       return false;
     }
     delay(1);  // Prevent watchdog crashes

     uint32_t currentMillis = millis();
     if (packetDeadlineExpired() ||
         currentMillis - previousMillis >= ((int32_t) this->socketTimeout * 1000)) {
       return false;
     }
   }

// read() returns -1 when no byte could be retrieved even though available() was
// non-zero (reachable with TLS clients at record boundaries). Storing that as 0xFF
// would silently inject a bogus byte into the packet being parsed.
   int rc = _client->read();
   if (rc < 0) {
     return false;
   }
   *result = (uint8_t)rc;

   return true;
}

// reads a byte into result[*index] and increments index
boolean PubSubClient::readByte(uint8_t * result, uint16_t * index) {
  uint16_t current_index = *index;
  uint8_t * write_address = &(result[current_index]);
  if (readByte(write_address)) {
    *index = current_index + 1;
    return true;
  }
  return false;
}

uint32_t PubSubClient::readPacket(uint8_t* lengthLength) {
    // The deadline begins when this receive operation begins and is cleared on
    // every exit path, including malformed-frame and buffer-cap rejections.
    this->packetDeadline = millis() + (static_cast<unsigned long>(this->packetTimeout) * 1000UL);
    struct PacketDeadlineReset {
        unsigned long& deadline;
        ~PacketDeadlineReset() { deadline = 0; }
    } deadlineReset{this->packetDeadline};
    auto readFailed = [this]() -> uint32_t {
        if (this->packetDeadlineExpired()) {
            this->abortPacketRead();
        }
        return 0;
    };
    // A complete framing violation is distinguishable from a byte that simply
    // has not arrived yet. The former is a protocol error while connected, so
    // MQTT 5.0 sends its mandatory error DISCONNECT before closing; during the
    // initial CONNECT/CONNACK exchange no session exists to answer, so record
    // the diagnostic and close silently for connect() to report.
    auto rejectMalformed = [this]() -> uint32_t {
        if (MQTT_RUNTIME_IS_5() && this->_state == MQTT_CONNECTED) {
            this->protocolFailure(MqttError::MalformedPacket, MQTT_RC_MALFORMED_PACKET);
        } else {
            this->errorState = MqttError::MalformedPacket;
            this->errorReasonCode = MQTT_RC_MALFORMED_PACKET;
            this->_state = MQTT_DISCONNECTED;
            if (this->_client != nullptr) {
                this->_client->stop();
            }
        }
        return 0;
    };

    uint16_t len = 0;

// The fixed header plus the Remaining Length field occupy up to MQTT_MAX_HEADER_SIZE
// bytes, which are always written to the front of the buffer. A buffer smaller than
// that cannot hold any packet header, so refuse instead of overrunning it.
    if ((this->buffer == nullptr) || (this->bufferSize < MQTT_MAX_HEADER_SIZE)) {
        _state = MQTT_DISCONNECTED;
        if (_client != nullptr) { _client->stop(); }
        return 0;
    }

    if (!readByte(this->buffer, &len)) { return readFailed(); }
    bool isPublish = (this->buffer[0]&0xF0) == MQTTPUBLISH;
    uint32_t multiplier = 1;
    uint32_t length = 0;
    uint8_t digit = 0;
    uint16_t skip = 0;
    uint32_t start = 0;

    do {
        // MQTT 5.0 support is intentionally limited to the two-byte VBI range.
        // A continued second byte would require a third byte and is malformed;
        // refuse before reading that byte so no over-long frame is ever accepted.
        if (len >= 3u) {
            return rejectMalformed();
        }
        if (!readByte(&digit)) { return readFailed(); }
        this->buffer[len++] = digit;
        length += (digit & 127) * multiplier;
        multiplier <<=7; //multiplier *= 128
    } while ((digit & 128) != 0);

    *lengthLength = len-1;

// DoS mitigation + sentinel byte:
// In non-stream mode, if the declared packet cannot fit the buffer while leaving
// at least one spare byte (needed by consumers that NUL-terminate at buffer[len]),
// close the connection immediately instead of draining the whole Remaining Length
// byte-by-byte. Draining a large or trickle-fed body would block the event loop.
    // total wire packet size = fixed header (1) + length bytes (llen) + Remaining Length
    uint32_t total_packet = (uint32_t)1 + (uint32_t)(*lengthLength) + length;

    // Configurable hard cap, independent of buffer allocation (0 = disabled).
    // Applies to both stream and non-stream mode: a larger packet is refused and the
    // connection closed rather than draining/streaming an unbounded body.
    if (this->maxIncomingPacketSize != 0 && total_packet > this->maxIncomingPacketSize) {
        return rejectMalformed();
    }

    // Non-stream: the packet must also fit the buffer, leaving one spare byte for a
    // downstream NUL terminator. Draining an oversized body would block the
    // event loop, so reject it through the malformed-frame failure funnel.
    if (!this->stream && total_packet >= (uint32_t)this->bufferSize) {
        return rejectMalformed();
    }

    if (isPublish) {
        // Read in topic length to calculate bytes to skip over for Stream writing
        if (!readByte(this->buffer, &len)) { return readFailed(); }
        if (!readByte(this->buffer, &len)) { return readFailed(); }
        skip = (this->buffer[*lengthLength+1]<<8)+this->buffer[*lengthLength+2];
        start = 2;
        if ((this->buffer[0] & 0x06) != 0) {
            // Both QoS 1 and QoS 2 PUBLISH packets carry a Packet Identifier.
            skip += 2;
        }
    }
    uint32_t idx = len;

    for (uint32_t i = start;i<length;i++) {
        if (!readByte(&digit)) { return readFailed(); }
        if (this->stream) {
            if (isPublish && idx-*lengthLength-2>skip) {
                this->stream->write(digit);
            }
        }

        if (len < this->bufferSize) {
            this->buffer[len] = digit;
            len++;
        }
        idx++;

// Periodically yield while consuming a large (typically streamed) body so the
// watchdog is not starved. Non-stream packets are < bufferSize so this rarely fires.
        if ((i & 0x3FF) == 0) { delay(0); }
    }

// Sentinel byte: use >= so an exact-buffer packet is also ignored, guaranteeing a spare byte for
// downstream NUL-termination at buffer[len]. Non-stream oversized packets are
// normally already rejected above; this is defense-in-depth.
    if (!this->stream && idx >= this->bufferSize) {
        len = 0; // This will cause the packet to be ignored.
    }
#ifdef USE_MQTT_DETAILED_LOGGING_BINARY
    // Report the complete received packet (fixed header + Remaining Length bytes +
    // body) now held in `this->buffer`. Only the non-stream, non-ignored case carries
    // the whole packet here; a streamed body was already handed to `this->stream`.
    if (len > 0) {
        logWirePacket(false, this->buffer, len);
    }
#endif  // USE_MQTT_DETAILED_LOGGING_BINARY
    return len;
}

// --- Inbound QoS 2 registry (Requirements 8.10, 8.11, 8.15) --------------
//
// An entry is kept from the first QoS 2 PUBLISH until its matching PUBREL. The
// registry is fixed-size and 0 denotes an unused entry because MQTT Packet
// Identifiers are nonzero. It is separate from `outstanding`, which tracks
// client-originated operations and must never be affected by broker delivery.
bool PubSubClient::inboundQos2Contains(uint16_t packetId) const {
    if (packetId == 0) {
        return false;
    }
    for (uint16_t id : this->inboundQos2) {
        if (id == packetId) {
            return true;
        }
    }
    return false;
}

bool PubSubClient::registerInboundQos2(uint16_t packetId) {
    if (packetId == 0) {
        return false;
    }
    for (uint16_t i = 0; i < MQTT_MAX_INBOUND_INFLIGHT; i++) {
        if (this->inboundQos2[i] == 0) {
            this->inboundQos2[i] = packetId;
            return true;
        }
    }
    return false;
}

bool PubSubClient::releaseInboundQos2(uint16_t packetId) {
    for (uint16_t i = 0; i < MQTT_MAX_INBOUND_INFLIGHT; i++) {
        if (this->inboundQos2[i] == packetId) {
            this->inboundQos2[i] = 0;
            return true;
        }
    }
    return false;
}

void PubSubClient::clearInboundQos2() {
    memset(this->inboundQos2, 0, sizeof(this->inboundQos2));
}

boolean PubSubClient::loop() {
    if (connected()) {
        const bool mqtt5 = MQTT_RUNTIME_IS_5();
        unsigned long t = millis();
// keepAlive == 0 disables the keepalive mechanism (as advertised by MQTT itself).
// Without the guard the interval collapses to 0 ms, so every loop() sent a PINGREQ and
// the next one tore the connection down with MQTT_CONNECTION_TIMEOUT.
        if (this->keepAlive &&
            ((t - lastInActivity > this->keepAlive*1000UL) || (t - lastOutActivity > this->keepAlive*1000UL))) {
            if (pingOutstanding) {
                this->_state = MQTT_CONNECTION_TIMEOUT;
                _client->stop();
                return false;
            } else {
                this->buffer[0] = MQTTPINGREQ;
                this->buffer[1] = 0;

                if (transportWrite(this->buffer, 2) != 2) {
                  transportWriteFailed();
                  return false;
                }
                lastOutActivity = t;
                lastInActivity = t;
                pingOutstanding = true;
            }
        }
        if (_client->available()) {
            uint8_t llen;
            uint16_t len = readPacket(&llen);
            uint16_t msgId = 0;
            uint8_t *payload;
            if (len > 0) {
                lastInActivity = t;
                // The public inbound-property view borrows this working buffer. Drop its
                // prior contents before another PUBLISH can overwrite those bytes, even
                // when the new packet is rejected by the semantic gate below.
                if (mqtt5 && ((this->buffer[0] & 0xF0) == MQTTPUBLISH)) {
                    resetInboundProperties();
                }
                if (mqtt5) {
                    uint8_t semanticReasonCode = 0;
                    const MqttError semanticError = validateInboundPacket5(
                        this->buffer, len, llen, &semanticReasonCode);
                    if (semanticError != MqttError::None) {
                        protocolFailure(semanticError,
                                        (semanticReasonCode != 0)
                                            ? semanticReasonCode
                                            : defaultReasonCode(semanticError));
                        return false;
                    }
                }
                uint8_t type = this->buffer[0]&0xF0;
                if (type == MQTTPUBLISH) {
                    if (callback || mqtt5) {
                        uint16_t tl = (this->buffer[llen+1]<<8)+this->buffer[llen+2]; /* topic length in bytes */

// Observed heap corruption in some cases since v10.0.0
// Also see https://github.com/knolleary/pubsubclient/pull/843
//
// Decode QoS from bits 1-2 and reject unsupported/illegal values
//       (2 = unsupported, 3 = protocol violation) before parsing.
// Validate the topic and (for QoS 1) the message-id bytes against the
//       number of bytes actually received (len), not just the buffer capacity.
//       This prevents out-of-bounds reads and payload-length underflow when a
//       broker sends a topic length that overruns the packet.
                        uint8_t qos = (this->buffer[0] & 0x06) >> 1;
                        if (qos > 1 && (!mqtt5 || qos == 3)) {
                          if (mqtt5 && (qos == 3)) {
// Requirement 8.13: QoS 3 is not a level, it is a malformed fixed header, so in MQTT 5.0
// mode it goes out through the standard failure funnel - `E0 01 81` while the socket is
// writable, then close - instead of the bare stop() the 3.1.1 path uses. The callback is
// never invoked either way. MQTT 5.0 QoS 2 proceeds into the inbound handshake below;
// after runtime level-4 fallback, `MQTT_RUNTIME_IS_5()` is false.
                            protocolFailure(MqttError::MalformedPacket, MQTT_RC_MALFORMED_PACKET);
                            return false;
                          }
                          _state = MQTT_DISCONNECTED;
                          _client->stop();
                          return false;
                        }

                        // Bytes required before the payload starts:
                        //   fixed header (1) + length bytes (llen) + topic length field (2)
                        //   + topic (tl) + message id for QoS 1 (2)
                        uint32_t header_len = (uint32_t)llen + 3 + tl + (qos ? 2 : 0);
                        // Topic must be non-empty, the header must fit within the received
                        // bytes, and (redundantly) within the buffer with room for the NUL.
                        if ((tl == 0) ||
                            (header_len > (uint32_t)len) ||
                            ((uint32_t)llen + 3 + tl > (uint32_t)this->bufferSize)) {
                          if (mqtt5) {
                            protocolFailure(MqttError::MalformedPacket, MQTT_RC_MALFORMED_PACKET);
                          } else {
                            _state = MQTT_DISCONNECTED;
                            _client->stop();
                          }
                          return false;
                        }

                        if (mqtt5 && (qos > 0) &&
                            (this->buffer[llen + 3 + tl] == 0) &&
                            (this->buffer[llen + 3 + tl + 1] == 0)) {
                            protocolFailure(MqttError::ProtocolError, MQTT_RC_PROTOCOL_ERROR);
                            return false;
                        }

                        memmove(this->buffer+llen+2,this->buffer+llen+3,tl); /* move topic inside buffer 1 byte to front */
                        this->buffer[llen+2+tl] = 0; /* end the topic as a 'C' string with \x00 */
                        char *topic = (char*) this->buffer+llen+2;

// Where the application payload starts. In 3.1.1 that is the end of the variable header,
// i.e. `header_len`; in 5.0 the Properties field sits in between, so its width is added
// once it has been validated against the bytes actually received.
//
// Requirements 6.5 and 12.3: the reported length is `len - payload_off`, so no topic,
// Packet Identifier or property byte is ever inside the delivered payload, and the
// length is invariant to how large the Properties field was. Requirement 6.6 is
// untouched: readPacket() still keeps buffer[len] inside the allocation, and
// payload_off <= len, so the driver's `mqtt_data[data_len] = 0` stays in bounds.
                        uint32_t payload_off = header_len;
                        if (mqtt5) {
                            uint32_t props_len = 0;
                            InboundPropertyCollectorCtx propsCtx;
                            propsCtx.props              = &this->inboundProps;
                            propsCtx.correlationStorage = this->correlationDataStorage;
                            propsCtx.correlationLen     = &this->correlationDataLen;
                            propsCtx.hasCorrelation     = &this->hasCorrelationData;
                            uint8_t authRc = 0;
                            MqttError perr = parsePublishProperties5(this->buffer, len,
                                                                     header_len,
                                                                     &propsCtx,
                                                                     props_len,
                                                                     &authRc);
                            if (perr != MqttError::None) {
// Requirement 6.7: an overrunning Topic Name, Packet Identifier or Property Length
// rejects the packet without invoking the callback, reports the failure through
// lastError(), and closes. MalformedPacket carries 0x81, an illegal or repeated property
// carries 0x82 - defaultReasonCode() picks from the same table either way. An
// Authentication Method or Authentication Data property (Requirements 3.11, 16.5,
// 16.6) is the one ProtocolError with a more specific code: `authRc` is 0x8C only for
// that trigger and 0 otherwise, so it is used when set and the category default
// otherwise.
                                protocolFailure(perr, (authRc != 0) ? authRc : defaultReasonCode(perr));
                                return false;
                            }
// decodeProperties() has already bounded the field against the received bytes, so this
// cannot exceed `len`.
                            payload_off += props_len;
// The property region parsed cleanly, so - and only now - the collected set becomes
// readable (Requirements 12.4, 12.6) and its extent is recorded for
// forEachInboundUserProperty() to re-walk (Requirement 11.4). Both offsets fit uint16_t
// because they are bounded by `len`, which readPacket() already kept within
// `bufferSize`. A property-free PUBLISH lands here too, with props_len == 1 and every
// field absent, which is exactly the valid-but-empty set of Requirement 12.6.
                            this->inboundPropsOff = (uint16_t)header_len;
                            this->inboundPropsLen = (uint16_t)props_len;
                            this->inboundProps.valid = true;
                        }

                        // QoS 1 and QoS 2 both carry a Packet Identifier after the topic.
                        if (qos > 0) {
                            msgId = (this->buffer[llen+3+tl]<<8)+this->buffer[llen+3+tl+1];
                        }
                        if (mqtt5) {
// Complete the scalar PUBLISH envelope before any existing message callback observes it.
// resetInboundProperties() already restored these fields to their QoS 0 defaults at the
// start of this PUBLISH parse.
                            this->inboundProps.qos = qos;
                            this->inboundProps.retained = (this->buffer[0] & 0x01) != 0;
                            this->inboundProps.duplicate = (this->buffer[0] & 0x08) != 0;
                            this->inboundProps.packetId = msgId;
                        }
                        if (qos == 1) {
                            payload = this->buffer+payload_off;
                            if (callback) {
                                callback(topic,payload,len-payload_off);
                            }

// Requirement 7.1: the PUBACK echoes the inbound PUBLISH's Packet Identifier and carries
// a success Reason Code - 0x00, implied by the short form. Framed by the shared
// acknowledgement assembler, whose four bytes here are identical to the hand-rolled
// PUBACK this used to write, at both runtime protocol levels.
                            if (sendAcknowledgement(MQTTPUBACK, msgId, 0)) {
                              lastOutActivity = t;
                            }
                        } else if (qos == 2) {
// Requirements 8.10, 8.11, 8.15: retain an inbound QoS 2 Packet Identifier until the
// matching PUBREL. The first PUBLISH delivers once and records the identifier before the
// PUBREC goes out. A repeated PUBLISH - regardless of its DUP bit - only re-sends PUBREC.
                            if (msgId == 0) {
                                protocolFailure(MqttError::ProtocolError, MQTT_RC_PROTOCOL_ERROR);
                                return false;
                            }
                            if (!inboundQos2Contains(msgId)) {
                                if (!registerInboundQos2(msgId)) {
// The broker exceeded the Receive Maximum this client declared in CONNECT. No callback
// runs for the over-cap message, and protocolFailure() emits 0x93 before closing.
                                    protocolFailure(MqttError::ProtocolError,
                                                    MQTT_RC_RECEIVE_MAX_EXCEEDED);
                                    return false;
                                }
                                payload = this->buffer+payload_off;
                                if (callback) {
                                callback(topic,payload,len-payload_off);
                            }
                            }
                            if (sendAcknowledgement(MQTTPUBREC, msgId, 0)) {
                                lastOutActivity = t;
                            }
                        } else {
                            payload = this->buffer+payload_off;
                            if (callback) {
                                callback(topic,payload,len-payload_off);
                            }
                        }
                    }
                } else if (mqtt5 && ((type == MQTTPUBACK) || (type == MQTTPUBREC) ||
                                      (type == MQTTPUBREL) || (type == MQTTPUBCOMP))) {
// --- Inbound acknowledgements (Requirements 7.3, 7.5, 7.8, 7.9) ------------
//
// MQTT 5.0 only: runtime level-4 fallback still reads these four types into the buffer
// and drops them without dispatch because `mqtt5` is false.
                    uint16_t ackId = 0;
                    uint8_t  ackRc = 0;
                    uint8_t  authRc = 0;
                    const MqttError aerr = parseAcknowledgement5(this->buffer, len, llen,
                                                                 (uint8_t)(type >> 4),
                                                                 ackId, ackRc, &authRc);
                    if (aerr != MqttError::None) {
// Requirement 7.9: a malformed acknowledgement is rejected through the standard failure
// funnel, which records the error for lastError(), emits `E0 01 <rc>` while the socket is
// writable and closes. MalformedPacket carries 0x81; an illegal property inside a
// long-form acknowledgement is a ProtocolError carrying 0x82, or 0x8C for the Enhanced
// Authentication identifiers, which is what `authRc` overrides with when set.
                        protocolFailure(aerr, (authRc != 0) ? authRc : defaultReasonCode(aerr));
                        return false;
                    }
                    if ((ackId == 0) ||
                        !mqtt5AllowsAcknowledgementReason((uint8_t)(type >> 4), ackRc)) {
                        protocolFailure(MqttError::ProtocolError, MQTT_RC_PROTOCOL_ERROR);
                        return false;
                    }
                    if (type == MQTTPUBACK) {
// Requirements 7.3, 7.4, 8.16: the PUBACK completes the outbound QoS 1 PUBLISH holding
// `ackId`, releases its identifier, and exposes the Reason Code through lastReasonCode()
// with the identifier through lastPacketId(). A code of 0x80 or above is terminal there -
// released like any other, never retransmitted.
#ifdef USE_MQTT_QOS
                        reportPublishAck(ackId, ackRc);
#else
// Without outbound QoS 1/2 this client never sent a QoS 1 PUBLISH, so a PUBACK correlates
// to nothing. It was framing-validated above; discard it (Requirement 7.8).
                        (void)ackRc;
#endif  // USE_MQTT_QOS
                    } else if (type == MQTTPUBREC) {
// Requirements 8.12, 8.14: step one of the OUTBOUND QoS 2 handshake. A Reason Code below
// 0x80 answers with exactly one PUBREL carrying `ackId` and leaves the entry holding its
// identifier until the PUBCOMP; a code of 0x80 or above sends no PUBREL, releases the
// identifier and exposes the code with that identifier as the failure result. Returns true
// only when a PUBREL actually reached the transport, which is the one case that counts as
// outbound activity.
#ifdef USE_MQTT_QOS
                        if (reportPublishRec(ackId, ackRc, t)) {
                            lastOutActivity = t;
                        }
#else
// No outbound QoS 2 PUBLISH exists to answer a PUBREC; discard it (Requirement 7.8).
                        (void)ackRc;
#endif  // USE_MQTT_QOS
                    } else if (type == MQTTPUBCOMP) {
// Requirement 8.12: step two. The exchange completes, the identifier is released, and the
// Reason Code is exposed through the same pair of accessors as a PUBACK's.
#ifdef USE_MQTT_QOS
                        reportPublishComp(ackId, ackRc);
#else
// No outbound QoS 2 handshake exists to complete; discard it (Requirement 7.8).
                        (void)ackRc;
#endif  // USE_MQTT_QOS
                    } else {  // MQTTPUBREL
// Requirements 8.10 and 8.11: PUBREL completes an inbound QoS 2 exchange only when its
// identifier is in the dedicated inbound registry. An unmatched PUBREL is discarded under
// Requirement 7.8, leaving the registry untouched. Keep the entry if PUBCOMP cannot be
// written so a retransmitted PUBREL can still be answered.
                        if (inboundQos2Contains(ackId) &&
                            sendAcknowledgement(MQTTPUBCOMP, ackId, 0)) {
                            lastOutActivity = t;
                            releaseInboundQos2(ackId);
                        }
                    }
                } else if (mqtt5 && ((type == MQTTSUBACK) || (type == MQTTUNSUBACK))) {
// --- SUBACK / UNSUBACK (Requirements 9.4, 9.5, 9.6) ------------------------
//
// This closes hardening finding F-10, which was exactly "SUBACK is read into the buffer
// and dropped without being looked at". Dispatch remains bypassed after runtime level-4
// fallback because `mqtt5` is false.
                    uint16_t subId = 0;
                    uint32_t rcOff = 0;
                    uint32_t rcCount = 0;
                    uint8_t  authRc = 0;
                    const MqttError serr = parseSubscriptionAck5(this->buffer, len, llen,
                                                                 (uint8_t)(type >> 4),
                                                                 subId, rcOff, rcCount,
                                                                 &authRc);
                    if (serr != MqttError::None) {
// A Reason Code list that does not fit the received bytes, or a Properties field that
// fails to decode, is rejected through the standard failure funnel.
                        protocolFailure(serr, (authRc != 0) ? authRc : defaultReasonCode(serr));
                        return false;
                    }
                    const bool legalSubReason =
                        mqtt5AllowsSubscriptionReason((uint8_t)(type >> 4), this->buffer[rcOff]);
                    if ((subId == 0) || (rcCount != 1) || !legalSubReason) {
                        protocolFailure(MqttError::ProtocolError, MQTT_RC_PROTOCOL_ERROR);
                        return false;
                    }
// Requirement 9.5: the failing filter is named from the FULL filter retained in the
// outstanding entry created by subscribe()/unsubscribe(), so a failure report is never
// truncated. An identifier matching no entry still reports its Reason Codes, with no
// filter to name.
                    reportSubscriptionAck(type, subId, this->buffer + rcOff, rcCount);
                } else if (mqtt5 && (type == MQTTDISCONNECT)) {
// --- Server-initiated DISCONNECT (Requirements 10.1, 10.2) -----------------
//
// Parse the Reason Code (0x00 when absent) and any Properties field, expose the code
// through serverDisconnectReasonCode(), then close WITHOUT a return DISCONNECT - which is
// why this does not go through protocolFailure(): the broker has already said goodbye, and
// answering would be one packet too many.
                    uint8_t discRc = 0;
                    uint8_t authRc = 0;
                    const MqttError derr = parseServerDisconnect5(this->buffer, len, llen,
                                                                  discRc, &authRc);
                    if (derr != MqttError::None) {
                        protocolFailure(derr, (authRc != 0) ? authRc : defaultReasonCode(derr));
                        return false;
                    }
                    if (!mqtt5AllowsDisconnectReason(discRc)) {
                        protocolFailure(MqttError::ProtocolError, MQTT_RC_PROTOCOL_ERROR);
                        return false;
                    }
// Requirement 14.2: recorded before the close, so the driver reads why the connection went
// away after loop() reports the failure.
                    this->errorState = MqttError::ServerDisconnect;
                    this->errorReasonCode = discRc;
                    this->disconnectReasonCode = discRc;
                    if (_client != nullptr) {
                        _client->stop();
                    }
// Reports MQTT_DISCONNECTED through state() and clears the per-connection transient
// state, exactly as every other teardown path does.
                    resetConnectionState();
                    return false;
                } else if ((!mqtt5) && (type == MQTTPINGREQ)) {
// PINGREQ is a client-to-server packet, so a client receiving one is a protocol error in
// MQTT 5.0 (Requirement 14.1) and falls through to the invalid-type branch below there.
// Runtime level-4 fallback keeps answering it because `!mqtt5` is true.
                    this->buffer[0] = MQTTPINGRESP;
                    this->buffer[1] = 0;
                    transportWrite(this->buffer,2);
                } else if (type == MQTTPINGRESP) {
                    pingOutstanding = false;
                }
            } else if (!connected()) {
                // readPacket has closed the connection
                return false;
            }
        }
        if (mqtt5) {
// Outstanding entries keep their Packet Identifiers after a local timeout. The sweep emits
// at most one diagnostic (and, for publishes, one nonterminal lifecycle event) per phase;
// terminal acknowledgement or definitive session abandonment performs the eventual release.
//
// Swept once per loop(), AFTER any inbound packet has been dispatched: an acknowledgement
// already waiting in the socket is one that arrived earlier in real time, so processing it
// first means a deadline can only fire on an operation that genuinely has no answer. It uses
// loop()'s already-sampled `t` rather than re-reading millis(), so the whole invocation
// judges every entry against one instant. Runtime level-4 fallback bypasses this sweep
// because `mqtt5` is false and the table is never populated.
            abandonExpiredOutstanding(t);
        }
        return true;
    }
    return false;
}

// --- Outbound PUBLISH Properties field (Requirements 6.1, 6.2, 6.3) --------
//
// Emit the PUBLISH Properties field at buf[pos..], returning the new position, or `pos`
// unchanged when it would not fit - the same all-or-nothing contract writeProperties()
// itself has, so the assembler can abort with nothing written (Requirement 13.2).
//
// A null `props` is the empty set, which encodes as the single length byte 0x00
// (Requirement 6.3). A non-null `props` is emitted in full by writeProperties() -
// Payload Format Indicator, Content Type, Response Topic, Correlation Data, then every
// User Property in supplied order. Response Topic and Correlation Data are independently
// optional (Requirements 11.3, 11.5, 11.6, 11.8, 11.9). The four preserved public overloads pass null; the QoS-capable entry points pass
// the caller's set, and both reach the wire through this one function. Response Topic
// and Correlation Data are emitted independently: requests may omit Correlation Data,
// and responses echo Correlation Data without carrying another Response Topic.
static uint16_t writePublishProperties(const MqttMessageProperties* props, uint16_t topicAlias,
                                       uint8_t* buf, uint16_t pos, uint16_t capacity);

// --- Outbound topic alias table (Requirements 11.1, 11.2, 5.4) -------------

uint16_t PubSubClient::outboundTopicAliasLimit() const {
    if (!MQTT_RUNTIME_IS_5() || (MQTT_MAX_TOPIC_ALIASES == 0) || (this->serverTopicAliasMax == 0)) {
        return 0;
    }
    return (this->serverTopicAliasMax < MQTT_MAX_TOPIC_ALIASES)
               ? this->serverTopicAliasMax
               : MQTT_MAX_TOPIC_ALIASES;
}

MqttTopicAliasEntry* PubSubClient::findTopicAlias(const char* topic) {
    if (topic == nullptr) {
        return nullptr;
    }
    for (MqttTopicAliasEntry& entry : this->topicAliases) {
        if (entry.topic == topic) {
            return &entry;
        }
    }
    return nullptr;
}

// --- Outbound PUBLISH assembler (Requirements 6.1, 6.2, 6.3, 8.3, 8.17) ----
//
// Layout, per the specification and identical for all three publish paths:
//
//   fixed header byte : 0x30 | (DUP << 3) | (QoS << 1) | Retain      (Requirement 8.3)
//   Topic Name        : 2-byte length prefix + bytes
//   Packet Identifier : 2 bytes, ONLY when QoS > 0
//   Properties        : MQTT 5.0 only - immediately after the Packet Identifier when one
//                       is present and immediately after the Topic Name otherwise
//                       (Requirements 6.1, 6.2); the empty set is the single byte 0x00
//                       (Requirement 6.3)
//
// The three gates all run here, before the caller hands a single byte to the transport,
// so a refusal transmits nothing and leaves the connection untouched (Requirements 13.2,
// 4.9). Bytes scribbled into the working buffer are scratch - the buffer is the client's
// own assembly area and carries no connection state.
//
// The four preserved publish overloads, publish_P() and beginPublish() all pass qos 0 and
// packetId 0; the QoS-capable entry points added alongside them reach QoS 1 and 2 through
// this same assembler, which is why adding them changed no framing code.
uint16_t PubSubClient::buildPublishHeader(const char* topic, uint8_t qos, boolean dup,
                                         boolean retained, uint16_t packetId,
                                         const MqttMessageProperties* props, boolean useTopicAlias,
                                         uint32_t payloadLength, boolean payloadBuffered,
                                         uint8_t& headerByte) {
    headerByte = MQTTPUBLISH;
    if (dup) {
        headerByte |= 0x08;
    }
    headerByte |= (uint8_t)((qos & 0x03) << 1);
    if (retained) {
        headerByte |= 0x01;
    }

    if (topic == nullptr) {
        return 0;
    }
    const bool mqtt5 = MQTT_RUNTIME_IS_5();

// Topic aliases are opt-in per request. An existing mapping sends an empty Topic Name;
// a new mapping is committed only after every local framing and broker-size gate has
// accepted the packet, so a refusal cannot change connection state.
    const char* topicName = topic;
    uint16_t topicAlias = 0;
    bool addTopicAlias = false;
    if (mqtt5 && useTopicAlias) {
        MqttTopicAliasEntry* mapped = findTopicAlias(topic);
        if (mapped != nullptr) {
            topicAlias = mapped->alias;
            topicName = "";
        } else {
            const uint16_t aliasLimit = outboundTopicAliasLimit();
            if (this->topicAliases.length() < aliasLimit) {
                topicAlias = (uint16_t)(this->topicAliases.length() + 1);
                addTopicAlias = true;
            }
        }
    }

    size_t tlen = strnlen(topicName, this->bufferSize);
    uint16_t idLen = (qos > 0) ? 2 : 0;

// Buffer gate, first half: reserved header space + Topic Name (with its 2-byte length
// prefix) + Packet Identifier when there is one, plus the payload for the buffered paths.
// With qos 0 and no Properties field this is the same arithmetic each caller used before
// MQTT 5.0, so the 3.1.1 refusal boundary does not move (Requirement 2.3).
    uint32_t projected = (uint32_t)MQTT_MAX_HEADER_SIZE + 2 + tlen + idLen;
    if (payloadBuffered) {
        projected += payloadLength;
    }
    if (projected > this->bufferSize) {
        this->errorState = MqttError::BufferTooSmall;
        return 0;
    }

    uint16_t pos = writeString(topicName, this->buffer, MQTT_MAX_HEADER_SIZE);

    if (qos > 0) {
        this->buffer[pos++] = (uint8_t)(packetId >> 8);
        this->buffer[pos++] = (uint8_t)(packetId & 0xFF);
    }

    if (mqtt5) {
        uint16_t propsEnd = writePublishProperties(props, topicAlias, this->buffer, pos,
                                                   this->bufferSize);
// An accepted field is at least the one length byte, so an unchanged position can only
// mean refusal.
        if (propsEnd == pos) {
            this->errorState = MqttError::BufferTooSmall;
            return 0;
        }
        pos = propsEnd;
    }

// Buffer gate, second half: the Properties field's width is only known once it has been
// assembled, so the buffered paths re-check that their payload still fits behind it.
    if (payloadBuffered && ((uint32_t)pos + payloadLength > this->bufferSize)) {
        this->errorState = MqttError::BufferTooSmall;
        return 0;
    }

// Remaining Length gate (Requirements 4.9, 8.17, Property 28): variable header plus the
// DECLARED payload must be encodable as a 2-byte Variable Byte Integer. buildHeader()
// would refuse it too, but only after beginPublish() had already committed to a packet;
// checking the projected total here is what makes a refusal total - zero transmitted
// bytes, connection state unchanged - for the one path the narrowed codec actually makes
// reachable, since every buffered path is bounded by the working buffer long before
// 16,383.
    uint32_t remainingLength = (uint32_t)(pos - MQTT_MAX_HEADER_SIZE) + payloadLength;
    if (remainingLength > MQTT_VBI_MAX) {
        this->errorState = MqttError::PacketTooLarge;
        return 0;
    }
    if (exceedsServerMaximumPacketSize(remainingLength)) {
        return 0;
    }

// The node and its owned String are allocated only for the first accepted use of a topic.
// A mapped topic reaches none of this code, so reuse is a bounded linear lookup with no
// allocation. No mapping is evicted when the table is full.
    if (addTopicAlias) {
        MqttTopicAliasEntry& entry = this->topicAliases.addToLast();
        entry.alias = topicAlias;
        entry.topic = topic;
    }

    return pos;
}

boolean PubSubClient::publish(const char* topic, const char* payload) {
    return publish(topic,(const uint8_t*)payload, payload ? strnlen(payload, this->bufferSize) : 0,false);
}

boolean PubSubClient::publish(const char* topic, const char* payload, boolean retained) {
    return publish(topic,(const uint8_t*)payload, payload ? strnlen(payload, this->bufferSize) : 0,retained);
}

boolean PubSubClient::publish(const char* topic, const uint8_t* payload, unsigned int plength) {
    return publish(topic, payload, plength, false);
}

boolean PubSubClient::publish(const char* topic, const uint8_t* payload, unsigned int plength, boolean retained) {
    if (connected()) {
        if ((topic == nullptr) || ((payload == nullptr) && (plength != 0))) {
            return false;
        }
// Variable header through the shared assembler: it applies the buffer gate (payload
// included, since this path copies it) and the Remaining Length gate, and returns 0
// having transmitted nothing when either refuses.
        uint8_t header = 0;
        uint16_t length = buildPublishHeader(topic, 0, false, retained, 0, nullptr, false,
                                             plength, true, header);
        if (length == 0) {
            return false;
        }

        // Add payload
        for (unsigned int i=0;i<plength;i++) {
            this->buffer[length++] = payload[i];
        }

        return write(header,this->buffer,length-MQTT_MAX_HEADER_SIZE);
    }
    return false;
}

// --- QoS-capable publish entry points (Requirements 8.1, 8.2, 8.3, 8.5) ----
//
// The four overloads above are preserved verbatim and still publish at QoS 0 with no
// properties. These three are new (Requirement 2.6) and are the only way to ask for
// QoS 1 or QoS 2, which the pre-migration library had no entry point for at all.
//
// Both convenience forms fill in a MqttPublishRequest and hand it to the full form, so
// there is exactly ONE implementation of the QoS-capable buffered publish and exactly
// one place the gates and the outstanding-entry bookkeeping live.

boolean PubSubClient::publish(const char* topic, const uint8_t* payload,
                              unsigned int plength, boolean retained, uint8_t qos) {
// Stack-only: the request is a handful of borrowed pointers and scalars with default
// member initialisers, so this costs a few bytes of frame and no allocation.
    MqttPublishRequest request;
    request.topic = topic;
    request.payload = payload;
    request.plength = plength;
    request.retained = retained;
    request.qos = qos;
    return publish(request);
}

boolean PubSubClient::publish(const char* topic, const char* payload, boolean retained,
                              uint8_t qos) {
// Same null/length treatment the preserved const char* overloads use: a null payload is
// a zero-length one, and the length is bounded by the working buffer.
    return publish(topic, (const uint8_t*)payload,
                   payload ? strnlen(payload, this->bufferSize) : 0, retained, qos);
}

boolean PubSubClient::publish(const MqttPublishRequest& request) {
    return publish(request, nullptr);
}

boolean PubSubClient::publish(const MqttPublishRequest& request, uint16_t* packetIdOut) {
    if (packetIdOut != nullptr) {
        *packetIdOut = 0;
    }
    if (!connected()) {
        return false;
    }
    if ((request.topic == nullptr) ||
        ((request.payload == nullptr) && (request.plength != 0))) {
        return false;
    }
    const bool mqtt5 = MQTT_RUNTIME_IS_5();
// QoS domain gate (Requirements 8.3, 8.18). Runtime protocol level 5 accepts 0..2;
// runtime level-4 fallback accepts only 0. Acknowledged delivery depends on the MQTT5
// acknowledgement dispatch and abandonment sweep, both gated by `mqtt5`, so refusing
// QoS 1/2 before framing prevents an outstanding slot that runtime fallback cannot complete.
#ifdef USE_MQTT_QOS
    const uint8_t maxAllowedQos = mqtt5 ? 2 : 0;
#else
// Outbound QoS 1/2 is not compiled in (USE_MQTT_QOS undefined): QoS 0 is the only accepted
// level, exactly as runtime level-4 fallback. A QoS 1/2 request is refused before framing.
    const uint8_t maxAllowedQos = 0;
    (void)mqtt5;  // only consulted by the QoS 1/2 gates compiled out above
#endif  // USE_MQTT_QOS
    if (request.qos > maxAllowedQos) {
        return false;
    }

#ifdef USE_MQTT_QOS
// Receive Maximum is broker credit for unacknowledged outbound QoS 1/2 PUBLISHes only;
// subscriptions share the identifier table but never consume it. Admission is deliberately
// nonblocking: at capacity, reject before Packet Identifier allocation, assembly, retention,
// transport output, loop() or delay(). The caller may retry after its normal loop() dispatch
// has consumed an acknowledgement.
    if (mqtt5 && (request.qos > 0)) {
        if (this->inFlightCount() >= MQTT_MAX_OUTSTANDING) {
            this->errorState = MqttError::PacketIdUnavailable;
            return false;
        }
        const uint16_t brokerCap = (this->serverReceiveMaximum < MQTT_MAX_INFLIGHT)
                                       ? this->serverReceiveMaximum
                                       : MQTT_MAX_INFLIGHT;
        if (outstandingPublishCount() >= brokerCap) {
            this->errorState = MqttError::ReceiveMaximumReached;
            return false;
        }
    }
#endif  // USE_MQTT_QOS

// Requirement 8.4: the identifier comes from the SHARED allocator, so an outbound QoS 1/2
// publish can never collide with a SUBSCRIBE or an UNSUBSCRIBE. The entry is recorded
// BEFORE the first byte goes out, so a PUBACK/PUBREC that arrives while publish() is still
// writing already has something to correlate to.
//
// Requirement 8.5: no identifier, or no free slot to record the entry in, is a REFUSAL -
// `MqttError::PacketIdUnavailable` and zero transmitted bytes. Both publish and subscription
// operations require a tracked slot: sending any acknowledged operation without one would lose
// its live identifier and make its acknowledgement uncorrelatable. The fixed combined envelope
// is therefore the operative limit for atomic refusal.
    uint16_t packetId = 0;
    MqttOutstandingMessage* entry = nullptr;
#ifdef USE_MQTT_QOS
    if (request.qos > 0) {
        packetId = allocatePacketId();
        if (packetId != 0) {
            entry = claimOutstanding(packetId,
                                     (request.qos == 1) ? MqttOutState::AwaitingPuback
                                                        : MqttOutState::AwaitingPubrec,
                                     request.topic);
        }
        if (entry == nullptr) {
// Nothing has been assembled and nothing written, so the connection is untouched. The
// identifier, if one was handed out, was never recorded and so was never held.
            this->errorState = MqttError::PacketIdUnavailable;
            return false;
        }
        entry->qos = request.qos;
    }
#endif  // USE_MQTT_QOS

// Variable header through the shared assembler, which applies the buffer gate (payload
// included, since this path copies it) and the Remaining Length gate, and returns 0 having
// transmitted nothing when either refuses. `dup` is false: this is a first transmission,
// and the DUP flag belongs to retransmission (Requirement 8.7).
    uint8_t header = 0;
    uint16_t length = buildPublishHeader(request.topic, request.qos, false, request.retained,
                                        packetId, &request.properties, request.useTopicAlias,
                                        request.plength, true, header);
    if (length == 0) {
// Refused by a gate, with the reason already latched by the assembler. The entry must go
// back: no packet was framed, so no acknowledgement will ever name this identifier.
        releaseOutstanding(entry);
        return false;
    }

    for (unsigned int i = 0; i < request.plength; i++) {
        this->buffer[length++] = request.payload[i];
    }

// Requirement 8.19. THIS IS THE ONLY RETENTION SITE IN THE LIBRARY. It is compiled only when
// USE_MQTT_QOS is defined - it IS the outbound QoS 1/2 replay buffer - and within that feature
// it is unconditional: there is no second macro that switches retention off. It sits on the
// buffered publish path deliberately: here the whole message exists in one place, as assembled
// bytes, at one instant. `publish_P()` streams from flash and has no QoS-capable entry point,
// so it never reaches this function and never holds an outstanding entry; `beginPublish()` is
// QoS 0 only, because no QoS-capable streaming API may exist without persistent replay storage.
// `entry` is null for QoS 0, which needs no retention because it needs no acknowledgement.
//
// Retention is the transmission-admission transaction for QoS 1/2. Copy the complete body
// before the first write; budget exhaustion or allocation failure releases this tentative
// outstanding entry, returns a retryable error, and writes no packet byte.
#ifdef USE_MQTT_QOS
    if (mqtt5 && (entry != nullptr) &&
        !retainPacketCopy(entry, header, this->buffer + MQTT_MAX_HEADER_SIZE,
                          (uint16_t)(length - MQTT_MAX_HEADER_SIZE))) {
        releaseOutstanding(entry);
        this->errorState = MqttError::RetentionUnavailable;
        return false;
    }
#endif  // USE_MQTT_QOS

    const boolean sent = write(header, this->buffer, length - MQTT_MAX_HEADER_SIZE);
    if (!sent && (entry != nullptr)) {
// Nothing, or not all, of the packet reached the transport, so the broker will never
// acknowledge this identifier: give the slot back rather than let the deadline collect it.
        releaseOutstanding(entry);
    } else if (sent && (entry != nullptr) && (packetIdOut != nullptr)) {
// Report the Packet Identifier only after the complete initial PUBLISH write succeeds.
// QoS 0 records no entry, so it intentionally leaves the caller's output at zero.
        *packetIdOut = packetId;
    }
    return sent;
}

boolean PubSubClient::publish_P(const char* topic, const char* payload, boolean retained) {
    return publish_P(topic, (const uint8_t*)payload, payload ? strnlen(payload, this->bufferSize) : 0, retained);
}

boolean PubSubClient::publish_P(const char* topic, const uint8_t* payload, unsigned int plength, boolean retained) {
    unsigned int rc = 0;

    if (!connected()) {
        return false;
    }

    if ((topic == nullptr) || ((payload == nullptr) && (plength != 0))) {
        return false;
    }

    // Assemble the variable header behind the reserved header space through the shared
    // assembler - the payload is streamed from flash, not buffered, so it takes no part
    // in the buffer gate - then let buildHeader() emit the Remaining Length in front of
    // it. One assembler and one encoder keep the framing identical across all three
    // publish paths and enforce the 2-byte bound in one place.
    uint8_t header = 0;
    uint16_t length = buildPublishHeader(topic, 0, false, retained, 0, nullptr, false,
                                        plength, false, header);
    if (length == 0) {
        return false;
    }
    size_t hlen = buildHeader(header, this->buffer,
                              (uint32_t)(length - MQTT_MAX_HEADER_SIZE) + plength);
    if (hlen == 0) {
        return false;
    }
    const uint16_t headerLength = length - (MQTT_MAX_HEADER_SIZE - hlen);
    const size_t headerWritten = transportWrite(
        this->buffer + (MQTT_MAX_HEADER_SIZE - hlen), headerLength);
    if (headerWritten != headerLength) {
        transportWriteFailed();
        return false;
    }
    rc = static_cast<unsigned int>(headerWritten);

    for (unsigned int i = 0; i < plength; i++) {
        const size_t written = transportWrite((char)pgm_read_byte_near(payload + i));
        if (written != 1) {
            transportWriteFailed();
            return false;
        }
        rc += static_cast<unsigned int>(written);
    }

    lastOutActivity = millis();
    const boolean sent = (rc == (unsigned int)headerLength + plength);
    return sent;
}

boolean PubSubClient::beginPublish(const char* topic, unsigned int plength, boolean retained) {
    const MqttMessageProperties noProperties;
    return beginPublishWithProperties(topic, plength, retained, noProperties);
}

boolean PubSubClient::beginPublishWithProperties(
    const char* topic, unsigned int plength, boolean retained,
    const MqttMessageProperties& properties) {
    if (connected()) {
        // A second streaming header before endPublish() would irreversibly append to
        // the first packet's declared payload. Refuse it without touching the wire.
        if (this->publishInProgress) {
            return false;
        }
        if (topic == nullptr) {
            return false;
        }
        // Variable header through the shared assembler. The payload is streamed by the
        // caller, so it is not buffered - but its DECLARED length is still what the
        // Remaining Length gate judges, and that gate lives in the assembler. This is
        // the path the 2-byte limit actually makes reachable: a declared plength that
        // would push the Remaining Length above 16,383 is refused here, before the fixed
        // header is transmitted, so the streaming publish never starts (Requirement 8.17).
        //
        // NO Retained_Packet_Body is ever held here, even though retention is unconditional on
        // the buffered path (Requirement 8.19). The payload does not exist in one place on this
        // path: `plength` is a PROMISE, the bytes arrive later through write() and are gone once
        // handed to the socket, and the declared length may be many times the working buffer (a
        // file transfer under USE_MQTT_FILE). Retaining one would mean buffering an arbitrary
        // payload in RAM, which is precisely what this API exists to avoid.
        //
        // `beginPublish` is QoS 0 only. A future QoS-capable streaming entry point is
        // prohibited unless it provides persistent replay storage before its first write;
        // a streamed payload cannot satisfy the buffered QoS 1/2 admission contract.
        uint8_t header = 0;
        uint16_t length = buildPublishHeader(topic, 0, false, retained, 0,
                                            &properties,
                                            false, plength, false, header);
        if (length == 0) {
            return false;
        }
        // Remaining Length is computed in 32-bit arithmetic so an over-limit declared
        // payload is detected, not wrapped; buildHeader() then refuses anything above
        // 16383 and the streaming publish aborts before a single byte goes out.
        uint32_t remaining_length = (uint32_t)(length - MQTT_MAX_HEADER_SIZE) + plength;

        // Send the header and variable length field
        size_t hlen = buildHeader(header, this->buffer, remaining_length);

        if (hlen == 0) {   // Remaining Length above 16383, nothing has been sent yet
            return false;
        }

        const uint16_t declaredHeaderLength =
            length - (MQTT_MAX_HEADER_SIZE - static_cast<uint16_t>(hlen));
        const uint16_t rc = transportWrite(
            this->buffer + (MQTT_MAX_HEADER_SIZE - hlen), declaredHeaderLength);
        if (rc != declaredHeaderLength) {
            transportWriteFailed();
            return false;
        }

        lastOutActivity = millis();
        this->publishInProgress = true;
        this->publishBytesExpected = plength;
        this->publishBytesWritten = 0;
        return true;
    }
    return false;
}

int PubSubClient::endPublish() {
    if (!this->publishInProgress) {
        return (this->errorState == MqttError::WriteFailed) ? 0 : 1;
    }

    const bool complete = (this->publishBytesWritten == this->publishBytesExpected) &&
                          (this->_client != nullptr) && (this->_client->connected() != 0);
    this->publishInProgress = false;
    this->publishBytesExpected = 0;
    this->publishBytesWritten = 0;
    if (!complete) {
        transportWriteFailed();
        return 0;
    }
    return 1;
}

size_t PubSubClient::write(uint8_t data) {
    if ((_client == nullptr) || (_client->connected() == 0)) {
        return 0;
    }
    const size_t rc = transportWrite(data);
    if (this->publishInProgress) {
        this->publishBytesWritten += rc;
    }
    if (rc != 1) {
        transportWriteFailed();
        return rc;
    }
    lastOutActivity = millis();
    return rc;
}

size_t PubSubClient::write(const uint8_t *buffer, size_t size) {
    if ((_client == nullptr) || (_client->connected() == 0)) {
        return 0;
    }
    const size_t rc = transportWrite(buffer, size);
    if (this->publishInProgress) {
        this->publishBytesWritten += rc;
    }
    if (rc != size) {
        transportWriteFailed();
        return rc;
    }
    if (rc != 0) {
        lastOutActivity = millis();
    }
    return rc;
}

// --- Variable Byte Integer codec --------------------------------------------
//
// Single implementation of the MQTT length encoding, shared by the fixed-header
// Remaining Length and (in MQTT 5.0) every Properties field length. Size classes
// are exactly the specification's.

#if __cplusplus < 201703L
// Pre-C++17 a constexpr static data member still needs an out-of-line definition
// if it is ever ODR-used (e.g. bound to a reference).
constexpr uint32_t PubSubClient::MQTT_VBI_MAX;
#endif

// Two size classes only: 1 byte for 0..127, 2 bytes for 128..16383. 16383 is the
// maximum encodable value; anything above it is refused with no byte emitted.
uint8_t PubSubClient::variableByteIntegerSize(uint32_t value) {
    if (value <= 127u)         { return 1; }
    if (value <= MQTT_VBI_MAX) { return 2; }
    return 0;   // out of range, not encodable
}

uint8_t PubSubClient::encodeVariableByteInteger(uint32_t value, uint8_t* out) {
    uint8_t llen = variableByteIntegerSize(value);
// Out of range (or no destination): emit nothing at all, so a caller that aborts
// on the 0 return leaves the output buffer untouched.
    if ((llen == 0) || (out == nullptr)) {
        return 0;
    }
    uint32_t len = value;
    for (uint8_t i = 0; i < llen; i++) {
        uint8_t digit = len & 0x7F;   // digit = len % 128
        len >>= 7;                    // len = len / 128
        if (len > 0) {
            digit |= 0x80;            // continuation bit
        }
        out[i] = digit;
    }
    return llen;
}

bool PubSubClient::decodeVariableByteInteger(const uint8_t* buf, uint32_t avail,
                                             uint32_t& value, uint8_t& bytesUsed) {
    value = 0;
    bytesUsed = 0;
    if (buf == nullptr) {
        return false;
    }
    uint32_t result = 0;
    uint32_t multiplier = 1;
// At most 2 bytes: a 2nd byte that still sets the continuation bit implies a 3rd
// byte, which is malformed for this codec. Every read is bounds-checked against
// `avail` first, so a truncated field never reads past the received bytes.
    for (uint8_t i = 0; i < 2; i++) {
        if (i >= avail) {
            return false;   // truncated within the available bytes
        }
        uint8_t encoded = buf[i];
        result += (uint32_t)(encoded & 0x7F) * multiplier;
        multiplier <<= 7;
        if ((encoded & 0x80) == 0) {
            // MQTT 5 validation and fallback evidence require the shortest VBI form.
            // The canonical check remains active in this MQTT5-capable implementation.
            if ((i != 0) && (result < 128u)) {
                return false;
            }
            value = result;
            bytesUsed = (uint8_t)(i + 1);
            return true;
        }
    }
    return false;   // 3rd continuation byte implied
}

// --- MQTT 5.0 property identifier table ------------------------------------
//
// Every identifier MQTT 5.0 defines, 0x01..0x2A, 27 rows. Identifiers this client never
// emits are still listed: an inbound Properties field may legally carry them, and a row
// is what lets the reader skip a value of the right width instead of misparsing the rest
// of the field. An identifier NOT in this table is a protocol error (Requirement 3.7).
//
// `packets` is the set of contexts the identifier is legal in, and an identifier appearing
// in a context outside its mask is also a protocol error (Requirement 3.7). `multiple`
// marks the identifiers that may repeat within one Properties field; every other
// identifier repeating is a protocol error (Requirement 3.8).
//
// On ESP8266 the table lives in flash. It is ~162 bytes of pure constant data with no
// relocations, so there is no reason to spend RAM on it.
#if defined(ESP8266)
#define MQTT_PROP_TABLE_STORAGE PROGMEM
#else
#define MQTT_PROP_TABLE_STORAGE
#endif

static const MqttPropertyDef PROPERTY_TABLE[] MQTT_PROP_TABLE_STORAGE = {
//   id     value type                 legal contexts                                                multiple
    {0x01, MqttPropType::Byte,       MQTT_CTX_PUBLISH | MQTT_CTX_WILL,                                false},  // Payload Format Indicator
    {0x02, MqttPropType::FourByte,   MQTT_CTX_PUBLISH | MQTT_CTX_WILL,                                false},  // Message Expiry Interval (parse only)
    {0x03, MqttPropType::Utf8,       MQTT_CTX_PUBLISH | MQTT_CTX_WILL,                                false},  // Content Type
    {0x08, MqttPropType::Utf8,       MQTT_CTX_PUBLISH | MQTT_CTX_WILL,                                false},  // Response Topic
    {0x09, MqttPropType::Binary,     MQTT_CTX_PUBLISH | MQTT_CTX_WILL,                                false},  // Correlation Data
    {0x0B, MqttPropType::VarInt,     MQTT_CTX_PUBLISH | MQTT_CTX_SUBSCRIBE,                           true },  // Subscription Identifier (parse only; repeats in PUBLISH)
    {0x11, MqttPropType::FourByte,   MQTT_CTX_CONNECT | MQTT_CTX_CONNACK | MQTT_CTX_DISCONNECT,       false},  // Session Expiry Interval (parse only)
    {0x12, MqttPropType::Utf8,       MQTT_CTX_CONNACK,                                                false},  // Assigned Client Identifier
    {0x13, MqttPropType::TwoByte,    MQTT_CTX_CONNACK,                                                false},  // Server Keep Alive
// Enhanced Authentication (Requirements 3.11, 16.5, 16.6): these two rows exist only
// to be RECOGNIZED so the identifier can be rejected at dispatch, before its value's
// encoding is even consulted - `type` is `None`, `packets` is 0 (legal in no packet
// context, so propertyLegalIn() refuses them the same way an unknown identifier
// would), and `multiple` is false. This is what keeps 0x16 (Binary Data on the wire)
// from ever reaching the Binary decode path, leaving Correlation Data (0x09) as the
// only identifier that path ever sees.
    {0x15, MqttPropType::None,       0,                                                                false},  // Authentication Method - rejected, never emitted
    {0x16, MqttPropType::None,       0,                                                                false},  // Authentication Data - rejected, never emitted
    {0x17, MqttPropType::Byte,       MQTT_CTX_CONNECT,                                                false},  // Request Problem Information
    {0x18, MqttPropType::FourByte,   MQTT_CTX_WILL,                                                   false},  // Will Delay Interval (parse only)
    {0x19, MqttPropType::Byte,       MQTT_CTX_CONNECT,                                                false},  // Request Response Information
    {0x1A, MqttPropType::Utf8,       MQTT_CTX_CONNACK,                                                false},  // Response Information
    {0x1C, MqttPropType::Utf8,       MQTT_CTX_CONNACK | MQTT_CTX_DISCONNECT,                          false},  // Server Reference
    {0x1F, MqttPropType::Utf8,       MQTT_CTX_CONNACK | MQTT_CTX_PUBACK | MQTT_CTX_PUBREC |
                                     MQTT_CTX_PUBREL | MQTT_CTX_PUBCOMP | MQTT_CTX_SUBACK |
                                     MQTT_CTX_UNSUBACK | MQTT_CTX_DISCONNECT | MQTT_CTX_AUTH,         false},  // Reason String
    {0x21, MqttPropType::TwoByte,    MQTT_CTX_CONNECT | MQTT_CTX_CONNACK,                             false},  // Receive Maximum
    {0x22, MqttPropType::TwoByte,    MQTT_CTX_CONNECT | MQTT_CTX_CONNACK,                             false},  // Topic Alias Maximum
    {0x23, MqttPropType::TwoByte,    MQTT_CTX_PUBLISH,                                                false},  // Topic Alias
    {0x24, MqttPropType::Byte,       MQTT_CTX_CONNACK,                                                false},  // Maximum QoS
    {0x25, MqttPropType::Byte,       MQTT_CTX_CONNACK,                                                false},  // Retain Available
    {0x26, MqttPropType::StringPair, MQTT_CTX_CONNECT | MQTT_CTX_CONNACK | MQTT_CTX_PUBLISH |
                                     MQTT_CTX_WILL | MQTT_CTX_PUBACK | MQTT_CTX_PUBREC |
                                     MQTT_CTX_PUBREL | MQTT_CTX_PUBCOMP | MQTT_CTX_SUBSCRIBE |
                                     MQTT_CTX_SUBACK | MQTT_CTX_UNSUBSCRIBE | MQTT_CTX_UNSUBACK |
                                     MQTT_CTX_DISCONNECT | MQTT_CTX_AUTH,                             true },  // User Property
    {0x27, MqttPropType::FourByte,   MQTT_CTX_CONNECT | MQTT_CTX_CONNACK,                             false},  // Maximum Packet Size
    {0x28, MqttPropType::Byte,       MQTT_CTX_CONNACK,                                                false},  // Wildcard Subscription Available
    {0x29, MqttPropType::Byte,       MQTT_CTX_CONNACK,                                                false},  // Subscription Identifier Available
    {0x2A, MqttPropType::Byte,       MQTT_CTX_CONNACK,                                                false}   // Shared Subscription Available
};

static const uint8_t PROPERTY_TABLE_COUNT =
    (uint8_t)(sizeof(PROPERTY_TABLE) / sizeof(PROPERTY_TABLE[0]));

#if defined(ESP8266)
// Copy-out target for the flash-resident table. A returned definition is read by the
// caller before the next lookup - the property reader looks an identifier up, uses the
// row, then moves on - so one scratch row is enough and costs 6 bytes of RAM instead of
// the whole table. Not reentrant, which matches the rest of the client: all packet
// assembly and parsing happens on the single caller thread.
static MqttPropertyDef s_propertyDefScratch;
#endif

uint8_t PubSubClient::propertyDefCount() {
    return PROPERTY_TABLE_COUNT;
}

const MqttPropertyDef* PubSubClient::findPropertyDef(uint8_t id) {
// Linear scan: 27 rows, and the identifiers are not contiguous (0x04..0x07, 0x0A,
// 0x0C..0x10, 0x14, 0x1B, 0x1D, 0x1E and 0x20 are undefined), so a table indexed by id
// would waste more space than the scan costs time.
    for (uint8_t i = 0; i < PROPERTY_TABLE_COUNT; i++) {
#if defined(ESP8266)
// The row is in flash: read the identifier byte through pgm_read_byte (the struct stride
// is 6, so the field is not 32-bit aligned and a direct dereference is not safe), then
// copy the whole matching row into RAM before handing it back.
        if (pgm_read_byte(&PROPERTY_TABLE[i].id) == id) {
            memcpy_P(&s_propertyDefScratch, &PROPERTY_TABLE[i], sizeof(MqttPropertyDef));
            return &s_propertyDefScratch;
        }
#else
        if (PROPERTY_TABLE[i].id == id) {
            return &PROPERTY_TABLE[i];
        }
#endif
    }
    return nullptr;   // not an MQTT 5.0 property identifier
}

// --- Properties field writer -----------------------------------------------
//
// The writer only ever appends, and every append is preceded by a bounds check, so
// the sizing pass and the emit pass walk exactly the same decisions: whatever the
// sizing pass accepted with a given capacity, the emit pass accepts too. That
// equivalence is what lets writeProperties() trust bodyLength() as the value of the
// length prefix it writes before the body exists (Requirement 13.2).

bool MqttPropertyWriter::reserve(uint32_t need) {
// Sticky: once an entry has been refused, every later entry is refused too, so a
// body is never left with a hole in it or with its entries reordered.
    if (this->overflow) {
        return false;
    }
    uint32_t after = (uint32_t)this->len + need;
// Two independent bounds: the caller's capacity, and the largest Property Length a
// Variable Byte Integer can carry in this library. A body above MQTT_VBI_MAX could
// not be length-prefixed at all.
    if ((after > (uint32_t)this->capacity) || (after > PubSubClient::MQTT_VBI_MAX)) {
        this->overflow = true;
        return false;
    }
    return true;
}

bool MqttPropertyWriter::measure(const char* s, uint16_t& outLen) const {
    outLen = 0;
    if (s == nullptr) {
        return false;
    }
// A property string is length-prefixed with 2 bytes, so 65535 is the hard ceiling.
// strnlen bounded one past it distinguishes "exactly 65535" from "longer".
    size_t n = strnlen(s, 0x10000u);
    if (n > 0xFFFFu) {
        return false;
    }
    outLen = (uint16_t)n;
    return true;
}

void MqttPropertyWriter::put(uint8_t b) {
// Sizing mode counts without writing; emit mode writes. `len` advances identically
// in both, which is the whole point of the two-pass scheme.
    if (this->buf != nullptr) {
        this->buf[this->len] = b;
    }
    this->len++;
}

void MqttPropertyWriter::putBlob(const uint8_t* d, uint16_t n) {
    if ((this->buf != nullptr) && (n > 0)) {
        memcpy(this->buf + this->len, d, n);
    }
    this->len = (uint16_t)(this->len + n);
}

// Authentication Method (0x15) and Authentication Data (0x16) are refused
// unconditionally, before any byte of the entry is measured or reserved: this is
// the writing side's half of Requirements 3.11, 16.5, so no add*() overload - and
// therefore no emitter reachable through writeProperties() - can ever place either
// identifier into a transmitted Properties field. Treated exactly like any other
// refusal (sticky `overflow`, no partial write), so the caller's single
// overflowed() check catches it along with every other reason an entry might not
// fit.
bool MqttPropertyWriter::rejectAuthProperty(uint8_t id) {
    if ((id == MQTT_PROP_AUTHENTICATION_METHOD) || (id == MQTT_PROP_AUTHENTICATION_DATA)) {
        this->overflow = true;
        return true;
    }
    return false;
}

bool MqttPropertyWriter::addByte(uint8_t id, uint8_t v) {
    if (rejectAuthProperty(id)) { return false; }
    if (!reserve(2)) { return false; }
    put(id);
    put(v);
    return true;
}

bool MqttPropertyWriter::addTwoByte(uint8_t id, uint16_t v) {
    if (rejectAuthProperty(id)) { return false; }
    if (!reserve(3)) { return false; }
    put(id);
    put((uint8_t)(v >> 8));
    put((uint8_t)(v & 0xFF));
    return true;
}

bool MqttPropertyWriter::addFourByte(uint8_t id, uint32_t v) {
    if (rejectAuthProperty(id)) { return false; }
    if (!reserve(5)) { return false; }
    put(id);
    put((uint8_t)(v >> 24));
    put((uint8_t)(v >> 16));
    put((uint8_t)(v >> 8));
    put((uint8_t)(v & 0xFF));
    return true;
}

bool MqttPropertyWriter::addVarInt(uint8_t id, uint32_t v) {
    if (rejectAuthProperty(id)) { return false; }
    uint8_t vlen = PubSubClient::variableByteIntegerSize(v);
// Not encodable as a Variable Byte Integer at all. Treated as a refusal like any
// other, so the caller's single overflowed() check catches it (Requirement 4.5).
    if (vlen == 0) {
        this->overflow = true;
        return false;
    }
    if (!reserve((uint32_t)1 + vlen)) { return false; }
    uint8_t tmp[2];
    PubSubClient::encodeVariableByteInteger(v, tmp);
    put(id);
    putBlob(tmp, vlen);
    return true;
}

bool MqttPropertyWriter::addString(uint8_t id, const char* s) {
    if (rejectAuthProperty(id)) { return false; }
    uint16_t slen = 0;
    if (!measure(s, slen)) {
        this->overflow = true;
        return false;
    }
    if (!reserve((uint32_t)3 + slen)) { return false; }
    put(id);
    put((uint8_t)(slen >> 8));
    put((uint8_t)(slen & 0xFF));
    putBlob((const uint8_t*)s, slen);
    return true;
}

bool MqttPropertyWriter::addBinary(uint8_t id, const uint8_t* d, uint16_t len) {
// Binary Data may legally be empty, but a null pointer with a non-zero length is a
// caller error and a null pointer at all means "no value", which the caller should
// not have asked to emit. Authentication Data (0x16) is rejected before either of
// those checks, since the whole identifier is refused regardless of what value
// would have accompanied it.
    if (rejectAuthProperty(id)) { return false; }
    if (d == nullptr) {
        this->overflow = true;
        return false;
    }
    if (!reserve((uint32_t)3 + len)) { return false; }
    put(id);
    put((uint8_t)(len >> 8));
    put((uint8_t)(len & 0xFF));
    putBlob(d, len);
    return true;
}

bool MqttPropertyWriter::addPair(uint8_t id, const char* k, const char* v) {
// User Property (0x26) is the only StringPair identifier this client emits, so
// `id` here is never 0x15/0x16 in practice - but the check costs nothing and keeps
// the refusal uniform across every add*() overload rather than leaving addPair()
// as an unguarded exception.
    if (rejectAuthProperty(id)) { return false; }
    uint16_t klen = 0;
    uint16_t vlen = 0;
    if (!measure(k, klen) || !measure(v, vlen)) {
        this->overflow = true;
        return false;
    }
// 1 identifier byte + two 2-byte length prefixes + both string bodies. The two
// passes must agree to the byte: the emit pass runs with `capacity` equal to the
// body length the sizing pass reported, so an over-reservation here would refuse
// the last entry in emit mode only.
    if (!reserve((uint32_t)5 + klen + vlen)) { return false; }
    put(id);
    put((uint8_t)(klen >> 8));
    put((uint8_t)(klen & 0xFF));
    putBlob((const uint8_t*)k, klen);
    put((uint8_t)(vlen >> 8));
    put((uint8_t)(vlen & 0xFF));
    putBlob((const uint8_t*)v, vlen);
    return true;
}

// True when `id` may legally appear in `packetType`. `packetType` is the MQTT packet
// type number (CONNECT = 1 ... AUTH = 15), or 0 for the Will Properties
// pseudo-context, which is exactly how MqttPropCtx numbers its bits.
static bool propertyLegalIn(uint8_t id, uint8_t packetType) {
    if (packetType > 15) {
        return false;
    }
    const MqttPropertyDef* def = PubSubClient::findPropertyDef(id);
    if (def == nullptr) {
        return false;
    }
    return (def->packets & (uint16_t)(1u << packetType)) != 0;
}

// The single description of what an outbound property set emits, run once per pass so
// the sizing pass and the emit pass cannot drift apart. Order is the design's:
// Payload Format Indicator, Content Type, Response Topic, Correlation Data, then the
// User Properties in the order the caller supplied them (Requirements 11.3, 11.5, 11.8,
// 11.9).
static void emitPropertySet(const MqttMessageProperties& props, uint8_t packetType,
                            MqttPropertyWriter& w) {
    if (props.hasPayloadFormat && propertyLegalIn(MQTT_PROP_PAYLOAD_FORMAT_INDICATOR, packetType)) {
        w.addByte(MQTT_PROP_PAYLOAD_FORMAT_INDICATOR, props.payloadFormat);
    }
    if ((props.contentType != nullptr) && propertyLegalIn(MQTT_PROP_CONTENT_TYPE, packetType)) {
        w.addString(MQTT_PROP_CONTENT_TYPE, props.contentType);
    }
    if ((props.responseTopic != nullptr) &&
        propertyLegalIn(MQTT_PROP_RESPONSE_TOPIC, packetType)) {
        w.addString(MQTT_PROP_RESPONSE_TOPIC, props.responseTopic);
    }
// Binary Data, at whatever length the caller supplied: there is no per-property size
// limit on sending (Requirement 11.10). The 16-byte cap belongs to the inbound copy
// into Correlation_Data_Storage; here only the buffer gate (through reserve()) and the
// Remaining Length gate in the assembler bound the value.
    if ((props.correlationData != nullptr) &&
        propertyLegalIn(MQTT_PROP_CORRELATION_DATA, packetType)) {
        w.addBinary(MQTT_PROP_CORRELATION_DATA, props.correlationData, props.correlationDataLen);
    }
    if ((props.userPropertyCount > 0) && (props.userPropertyKeys != nullptr) &&
        (props.userPropertyValues != nullptr) && propertyLegalIn(MQTT_PROP_USER_PROPERTY, packetType)) {
        for (uint8_t i = 0; i < props.userPropertyCount; i++) {
            w.addPair(MQTT_PROP_USER_PROPERTY, props.userPropertyKeys[i], props.userPropertyValues[i]);
        }
    }
}

uint16_t writeProperties(const MqttMessageProperties& props, uint8_t packetType,
                         uint8_t* buf, uint16_t pos, uint16_t capacity) {
    if ((buf == nullptr) || (pos >= capacity)) {
        return pos;   // nowhere to write even the length byte
    }
    uint16_t avail = (uint16_t)(capacity - pos);

// Pass 1 - sizing. Nothing is written. The body capacity offered is what remains
// after the smallest possible length prefix, so a set that only fits because the
// prefix was ignored is rejected here rather than half-emitted below.
    MqttPropertyWriter sizer(nullptr, (uint16_t)(avail - 1));
    emitPropertySet(props, packetType, sizer);
    if (sizer.overflowed()) {
        return pos;   // does not fit: nothing written, caller aborts (Requirement 13.2)
    }
    uint16_t bodyLen = sizer.bodyLength();

    uint8_t plen = PubSubClient::variableByteIntegerSize(bodyLen);
    if ((plen == 0) || ((uint32_t)plen + bodyLen > avail)) {
        return pos;   // the prefix pushes it over the edge
    }

// Pass 2 - emit the body at its final position, then the length prefix in front of
// it. The prefix width is already known exactly, so no memmove and no guess.
    MqttPropertyWriter emitter(buf + pos + plen, bodyLen);
    emitPropertySet(props, packetType, emitter);
// Defensive: the two passes make the same decisions under the same bounds, so this
// cannot differ. If it ever did, refusing here still leaves `pos` as the caller's
// position, so nothing the caller goes on to frame includes these bytes.
    if (emitter.overflowed() || (emitter.bodyLength() != bodyLen)) {
        return pos;
    }
    PubSubClient::encodeVariableByteInteger(bodyLen, buf + pos);

// An empty set lands here with bodyLen == 0 and plen == 1, i.e. the single byte 0x00
// (Requirements 1.2, 1.3, 6.3, 9.1, 9.3).
    return (uint16_t)(pos + plen + bodyLen);
}

// --- PUBLISH property extension: outbound Topic Alias -----------------------
// MQTT 5.0's Topic Alias is connection state, not caller-supplied message state, so it
// is appended here after the normal request properties. The two-pass writer preserves the
// all-or-nothing Properties-field contract while allowing an alias property to share the
// same field.
static void emitPublishPropertySet(const MqttMessageProperties& props, uint16_t topicAlias,
                                   MqttPropertyWriter& writer) {
    emitPropertySet(props, (uint8_t)(MQTTPUBLISH >> 4), writer);
    if (topicAlias != 0) {
        writer.addTwoByte(MQTT_PROP_TOPIC_ALIAS, topicAlias);
    }
}

static uint16_t writePublishProperties(const MqttMessageProperties* props, uint16_t topicAlias,
                                       uint8_t* buf, uint16_t pos, uint16_t capacity) {
    if ((buf == nullptr) || (pos >= capacity)) {
        return pos;
    }
    MqttMessageProperties empty;
    const MqttMessageProperties& set = (props != nullptr) ? *props : empty;
    const uint16_t avail = (uint16_t)(capacity - pos);

    MqttPropertyWriter sizer(nullptr, (uint16_t)(avail - 1));
    emitPublishPropertySet(set, topicAlias, sizer);
    if (sizer.overflowed()) {
        return pos;
    }
    const uint16_t bodyLen = sizer.bodyLength();
    const uint8_t prefixLen = PubSubClient::variableByteIntegerSize(bodyLen);
    if ((prefixLen == 0) || ((uint32_t)prefixLen + bodyLen > avail)) {
        return pos;
    }

    MqttPropertyWriter emitter(buf + pos + prefixLen, bodyLen);
    emitPublishPropertySet(set, topicAlias, emitter);
    if (emitter.overflowed() || (emitter.bodyLength() != bodyLen)) {
        return pos;
    }
    PubSubClient::encodeVariableByteInteger(bodyLen, buf + pos);
    return (uint16_t)(pos + prefixLen + bodyLen);
}

// --- Properties field reader -----------------------------------------------
//
// A zero-copy visitor walk. Nothing is copied out of `buf` and nothing is allocated:
// string, binary and string-pair values are handed to the visitor as a pointer plus a
// length into the caller's own buffer (Requirement 13.1). That is what makes inbound
// property support cost nothing but stack, and it is why an MqttPropertyView is only
// valid for the duration of the visitor call.
//
// The bounds discipline is the safety-critical part (Requirements 3.5, 13.4, 13.5).
// Two frames of reference are kept apart deliberately:
//
//   `avail`   bytes ACTUALLY RECEIVED - the hard wall. Nothing at or beyond
//             buf[avail] is ever read, whatever the packet declares.
//   `bodyEnd` prefix + the DECLARED Property Length, validated to sit at or before
//             `avail` before a single entry byte is touched. Entry-level checks work
//             against this, so an entry that runs past the declared body is rejected
//             (Requirement 3.6) even when received bytes happen to follow it.
//
// Order of checks, each one strictly before the read it authorises:
//   1. Property Length Variable Byte Integer decodes within `avail` and in <= 2 bytes
//      (Requirements 3.3, 4.6, 4.7)                          -> MalformedPacket
//   2. prefix + declared length <= `avail` (Requirements 3.5, 13.5) -> MalformedPacket
//   2.5. per entry: identifier is Authentication Method (0x15) or Authentication Data
//      (0x16), in ANY packetType, checked at identifier dispatch before the value's
//      encoding is consulted (Requirements 3.11, 16.5, 16.6)  -> ProtocolError (0x8C)
//   3. per entry: identifier is in the table and legal for `packetType` (3.7)
//      -> ProtocolError
//   4. per entry: identifier is not a repeat of a single-occurrence one (3.8)
//      -> ProtocolError
//   5. per entry, Correlation Data (0x09) only: the declared Binary Data length is
//      <= MQTT_CORRELATION_DATA_MAX (16) (Requirements 3.10, 11.11, 11.12, 13.7, 13.8)
//      -> ProtocolError, and this precedes the width/bounds check of step 6 so an
//      over-cap value is never measured against `bodyEnd` as if it might be copied
//   6. per entry: the value implied by the identifier's type fits before `bodyEnd`
//      (Requirement 3.6)                                     -> MalformedPacket
//
// Steps 2.5 through 5 precede step 6 within an entry, which is a refinement of the
// design's validation table rather than a departure from it: the table orders the
// *categories* of failure across the field, but the width of an entry's value is
// DEFINED by its identifier, so the value cannot be bounds-checked until the identifier
// has been resolved. The visible consequence is that an entry with an unknown or
// contextually illegal identifier, an Enhanced Authentication identifier, or an
// over-cap Correlation Data length, reports ProtocolError even when its value is also
// truncated.
//
// Step 2.5 is checked BEFORE step 3's table lookup, even though `PROPERTY_TABLE`'s own
// rows for 0x15/0x16 (`packets == 0`) would already make step 3 refuse them: the
// explicit check is what lets this function report the more specific Reason Code 0x8C
// (Bad authentication method) through `outReasonCode` instead of the generic 0x82 step
// 3 would otherwise imply, and it unconditionally keeps 0x16 (Binary Data on the wire)
// from ever reaching the Binary decode at step 6, so Correlation Data (0x09) remains
// the only identifier that path ever sees (Requirement 3.9).
//
// Step 5 is this reader's other identifier-specific check outside the generic table
// walk. It exists here - inside decodeProperties(), before any caller-supplied visitor
// runs - rather than inside the PUBLISH-only collector (inboundPropertyCollector) that
// performs the actual memcpy, so the rejection applies to every packetType carrying a
// Properties field that legally admits Correlation Data, not only PUBLISH, and so no
// visitor ever sees an over-cap Correlation Data entry to begin with.
MqttError decodeProperties(const uint8_t* buf, uint32_t avail, uint8_t packetType,
                           MqttPropertyVisitor visit, void* ctx, uint32_t& consumed,
                           uint8_t* outReasonCode) {
    consumed = 0;

// Step 1. The shared codec refuses a field truncated within `avail` and one that
// implies a 3rd byte, so a null `buf` or `avail == 0` lands here too.
    uint32_t bodyLen = 0;
    uint8_t  plen = 0;
    if (!PubSubClient::decodeVariableByteInteger(buf, avail, bodyLen, plen)) {
        return MqttError::MalformedPacket;
    }

// Step 2. `plen <= avail` holds because the decode succeeded within `avail`, so the
// subtraction cannot wrap. Comparing this way rather than `plen + bodyLen > avail`
// keeps the arithmetic away from any overflow question.
    if (bodyLen > (uint32_t)(avail - plen)) {
        return MqttError::MalformedPacket;
    }
    const uint32_t bodyEnd = (uint32_t)plen + bodyLen;

// One bit per identifier. Identifiers run to 0x2A, so 64 bits cover every one of them
// and duplicate detection needs no table scan (Requirement 3.8).
    uint64_t seenMask = 0;

    uint32_t pos = plen;
    while (pos < bodyEnd) {
// The loop condition is the bounds check for the identifier byte itself.
        const uint8_t id = buf[pos];
        pos++;

// Step 2.5 (Enhanced Authentication rejection, Requirements 3.11, 16.5, 16.6). Checked
// at IDENTIFIER DISPATCH - immediately once `id` is known, before propertyLegalIn()
// and before any width/bounds check of the value - in ANY packetType, with no
// per-context reasoning at all: `PROPERTY_TABLE`'s rows for 0x15/0x16 already carry
// `packets == 0`, so propertyLegalIn() below would refuse them anyway, but that path
// alone would only ever report the generic 0x82. This explicit check is what lets the
// caller emit the more specific 0x8C (Bad authentication method) instead, and it is
// what keeps 0x16 (Binary Data on the wire) from EVER reaching the Binary decode at
// step 6 - Correlation Data (0x09) remains the only identifier that path ever sees.
// The value is never decoded either way: this check comes before Step 3's table
// lookup and well before the switch on `def->type`.
        if ((id == MQTT_PROP_AUTHENTICATION_METHOD) || (id == MQTT_PROP_AUTHENTICATION_DATA)) {
            if (outReasonCode != nullptr) {
                *outReasonCode = PubSubClient::MQTT_RC_BAD_AUTH_METHOD;   // 0x8C
            }
            return MqttError::ProtocolError;
        }

// Step 3. propertyLegalIn() answers both halves - unknown identifier and identifier
// outside its legal contexts - from the one table that also drives emission.
        if (!propertyLegalIn(id, packetType)) {
            return MqttError::ProtocolError;
        }
        const MqttPropertyDef* def = PubSubClient::findPropertyDef(id);
        if (def == nullptr) {
            return MqttError::ProtocolError;   // unreachable: propertyLegalIn said yes
        }

// Step 4. `id` is in the table, so it is at most 0x2A and the shift is well defined.
        const uint64_t bit = (uint64_t)1u << id;
        if (!def->multiple && ((seenMask & bit) != 0)) {
            return MqttError::ProtocolError;
        }
        seenMask |= bit;

// Every width below - including the Step 5 check just below - is compared against
// what is left of the DECLARED body before the corresponding bytes are read.
        MqttPropertyView view;
        view.id   = id;
        view.type = def->type;
        uint32_t remaining = bodyEnd - pos;

// Step 5. Correlation Data (0x09) is the one Binary Data identifier this client
// handles, and its declared value length is capped at MQTT_CORRELATION_DATA_MAX (16)
// bytes on the wire (Requirements 3.10, 11.11, 11.12). This check reads only the
// 2-byte length prefix - itself bounds-checked against `remaining` before it is read -
// and precedes any width/bounds check of the value itself (step 6, the `switch` below),
// so an over-cap declared length is rejected before a single byte of the value is
// examined, let alone copied into the client's fixed Correlation_Data_Storage
// (Requirements 13.7, 13.8). Placed inside the shared reader rather than the
// PUBLISH-only collector, this reaches every packetType whose Properties field may
// legally carry Correlation Data.
        if (id == MQTT_PROP_CORRELATION_DATA) {
            if (remaining < 2) { return MqttError::MalformedPacket; }
            const uint16_t n = (uint16_t)(((uint16_t)buf[pos] << 8) | (uint16_t)buf[pos + 1]);
            if (n > PubSubClient::MQTT_CORRELATION_DATA_MAX) {
                return MqttError::ProtocolError;
            }
        }

// Step 6. Every width below is compared against what is left of the DECLARED body
// before the corresponding bytes are read.
        switch (def->type) {
            case MqttPropType::Byte: {
                if (remaining < 1) { return MqttError::MalformedPacket; }
                view.value = buf[pos];
                pos += 1;
                break;
            }
            case MqttPropType::TwoByte: {
                if (remaining < 2) { return MqttError::MalformedPacket; }
                view.value = ((uint32_t)buf[pos] << 8) | (uint32_t)buf[pos + 1];
                pos += 2;
                break;
            }
            case MqttPropType::FourByte: {
                if (remaining < 4) { return MqttError::MalformedPacket; }
                view.value = ((uint32_t)buf[pos]     << 24) |
                             ((uint32_t)buf[pos + 1] << 16) |
                             ((uint32_t)buf[pos + 2] <<  8) |
                              (uint32_t)buf[pos + 3];
                pos += 4;
                break;
            }
            case MqttPropType::VarInt: {
// The codec is handed only what is left of the declared body, so a Variable Byte
// Integer straddling the body end is malformed rather than read out of the field.
                uint32_t v = 0;
                uint8_t  used = 0;
                if (!PubSubClient::decodeVariableByteInteger(buf + pos, remaining, v, used)) {
                    return MqttError::MalformedPacket;
                }
                view.value = v;
                pos += used;
                break;
            }
            case MqttPropType::Utf8:
            case MqttPropType::Binary: {
// 2-byte length prefix, then that many bytes. Checked in that order: the prefix must
// be present before it can be read, and its value must fit before the body is viewed.
                if (remaining < 2) { return MqttError::MalformedPacket; }
                const uint16_t n = (uint16_t)(((uint16_t)buf[pos] << 8) | (uint16_t)buf[pos + 1]);
                if ((uint32_t)n + 2 > remaining) { return MqttError::MalformedPacket; }
// Zero copy: the value is the caller's own bytes. A zero-length value yields a
// one-past-the-value pointer, which is well defined and never dereferenced.
                view.data = buf + pos + 2;
                view.len  = n;
                pos += (uint32_t)2 + n;
                break;
            }
            case MqttPropType::None: {
// Unreachable: Step 2.5 above returns before `def` is even looked up for
// Authentication Method (0x15) or Authentication Data (0x16), the only two
// identifiers this type is ever assigned to. Present only so the switch stays
// exhaustive over MqttPropType.
                return MqttError::ProtocolError;
            }
            case MqttPropType::StringPair: {
                if (remaining < 2) { return MqttError::MalformedPacket; }
                const uint16_t klen = (uint16_t)(((uint16_t)buf[pos] << 8) | (uint16_t)buf[pos + 1]);
                if ((uint32_t)klen + 2 > remaining) { return MqttError::MalformedPacket; }
                view.data = buf + pos + 2;
                view.len  = klen;
                pos += (uint32_t)2 + klen;

// The value half is checked against what the key half left, so a pair whose value
// runs past the body end is malformed rather than partially visited.
                remaining = bodyEnd - pos;
                if (remaining < 2) { return MqttError::MalformedPacket; }
                const uint16_t vlen = (uint16_t)(((uint16_t)buf[pos] << 8) | (uint16_t)buf[pos + 1]);
                if ((uint32_t)vlen + 2 > remaining) { return MqttError::MalformedPacket; }
                view.data2 = buf + pos + 2;
                view.len2  = vlen;
                pos += (uint32_t)2 + vlen;
                break;
            }
        }

        if ((visit != nullptr) && !visit(view, ctx)) {
// Visitor abort. The field's extent is already known and already proven to lie within
// `avail`, so reporting it as consumed lets the caller resume at the next wire field.
            consumed = bodyEnd;
            return MqttError::None;
        }
    }

// `pos == bodyEnd` exactly: every entry ended on a boundary the declared length agrees
// with. An empty field lands here with bodyEnd == plen == 1.
    consumed = bodyEnd;
    return MqttError::None;
}

// length is the MQTT Remaining Length, encoded through the shared codec as 1-2
// bytes (max 16383). The parameter stays 32-bit so an over-limit request is
// detected and refused rather than silently wrapping at the call site.
size_t PubSubClient::buildHeader(uint8_t header, uint8_t* buf, uint32_t length) {
    uint8_t lenBuf[2];
    uint8_t llen = encodeVariableByteInteger(length, lenBuf);

// A length above 16383 does not fit the 2-byte encoding. Emitting a truncated
// value would desynchronise the connection, so report failure and let the caller
// abort having transmitted nothing.
    if (llen == 0) {
        return 0;
    }

// The header is built at the END of the first MQTT_MAX_HEADER_SIZE bytes: the fixed
// header byte sits immediately before the Remaining Length bytes, which end at
// buf[MQTT_MAX_HEADER_SIZE - 1].
    buf[MQTT_MAX_HEADER_SIZE-1-llen] = header;
    for (uint8_t i=0;i<llen;i++) {
        buf[MQTT_MAX_HEADER_SIZE-llen+i] = lenBuf[i];
    }
    return llen+1; // Full header size is variable length bit plus the 1-byte fixed header
}

bool PubSubClient::exceedsServerMaximumPacketSize(uint32_t remainingLength) {
    if (!MQTT_RUNTIME_IS_5() || (this->serverMaxPacketSize == 0)) {
        return false;  // CONNACK omitted Maximum Packet Size: no outbound limit
    }
    const uint8_t lengthBytes = variableByteIntegerSize(remainingLength);
    if ((lengthBytes == 0) || ((uint32_t)1 + lengthBytes + remainingLength >
                               this->serverMaxPacketSize)) {
        this->errorState = MqttError::PacketTooLarge;
        return true;
    }
    return false;
}

boolean PubSubClient::write(uint8_t header, uint8_t* buf, uint16_t length) {
    if (_client == nullptr) {
        return false;
    }
    const uint8_t hlen = buildHeader(header, buf, length);
    if (hlen == 0) {   // Remaining Length could not be encoded, send nothing
        return false;
    }

#ifdef MQTT_MAX_TRANSFER_SIZE
    uint8_t* writeBuf = buf + (MQTT_MAX_HEADER_SIZE - hlen);
    uint16_t bytesRemaining = length + hlen;
    while (bytesRemaining > 0) {
        const uint8_t bytesToWrite = (bytesRemaining > MQTT_MAX_TRANSFER_SIZE)
            ? MQTT_MAX_TRANSFER_SIZE : bytesRemaining;
        const uint16_t rc = transportWrite(writeBuf, bytesToWrite);
        if (rc != bytesToWrite) {
            transportWriteFailed();
            return false;
        }
        bytesRemaining -= rc;
        writeBuf += rc;
    }
#else
    const uint16_t expected = length + hlen;
    const uint16_t rc = transportWrite(buf + (MQTT_MAX_HEADER_SIZE - hlen), expected);
    if (rc != expected) {
        transportWriteFailed();
        return false;
    }
#endif

    lastOutActivity = millis();
    return true;
}

// The MQTT 5.0 Subscription Options byte (Requirement 9.2). A default-constructed set
// yields 0x00 and a set carrying only a Maximum QoS yields that QoS, matching the trailing
// requested-QoS byte used by runtime level-4 fallback. Existing overloads therefore remain
// byte-identical. Bits 6-7 are never set.
static uint8_t subscriptionOptionsByte(const MqttSubscribeOptions& options) {
    uint8_t b = (uint8_t)(options.maxQos & 0x03);                 // bits 0-1 Maximum QoS
    if (options.noLocal) {
        b |= 0x04;                                               // bit 2  No Local
    }
    if (options.retainAsPublished) {
        b |= 0x08;                                               // bit 3  Retain As Published
    }
    b |= (uint8_t)((options.retainHandling & 0x03) << 4);         // bits 4-5 Retain Handling
    return b;
}

boolean PubSubClient::sendSubscription(uint8_t packetType, const char* topic,
                                      const MqttSubscribeOptions* options,
                                      uint16_t* packetIdOut) {
    if (packetIdOut != nullptr) {
        *packetIdOut = 0;
    }
// Requirement 9.7: a null topic is rejected BEFORE any length computation, so strnlen()
// is never handed a null pointer and no packet is emitted.
    if (topic == nullptr) {
        return false;
    }
    const bool mqtt5 = MQTT_RUNTIME_IS_5();
    if (options != nullptr) {
// Requirements 9.8, 9.9: Maximum QoS 0..2 is accepted at runtime level 5 and 0..1
// during runtime level-4 fallback, which has no QoS 2. Anything above the active
// protocol level's maximum is refused before a byte is assembled.
        const uint8_t maxAllowedQos = mqtt5 ? 2 : 1;
        if (options->maxQos > maxAllowedQos) {
            return false;
        }
        if (mqtt5) {
            if (options->retainHandling > 2) {
                return false;
            }
        } else if (options->noLocal || options->retainAsPublished ||
                   (options->retainHandling != 0)) {
// MQTT 3.1.1 has no Subscription Options byte: bits 2-7 of its requested-QoS byte are
// reserved and must be zero. A caller asking for one of those behaviors is refused rather
// than silently served a packet that does not carry the request.
            return false;
        }
    }
    const size_t topicLength = strnlen(topic, this->bufferSize);
// Bytes needed around the filter: header(5) + Packet Identifier(2) + topic-length(2),
// plus the Subscription Options byte for SUBSCRIBE and, at runtime level 5, the one-byte
// empty Properties field. Runtime level-4 fallback keeps the long-standing bound:
// 10 + topicLength for SUBSCRIBE, 9 + topicLength for UNSUBSCRIBE.
    const uint8_t propertyBytes = mqtt5 ? 1u : 0u;
    const uint8_t optionBytes = (options != nullptr) ? 1u : 0u;
    const size_t overhead = 9u + optionBytes + propertyBytes;
    if (this->bufferSize < overhead + topicLength) {
        // The whole packet must fit in the single working buffer. Refuse before
        // allocating an identifier or writing anything to the transport.
        this->errorState = MqttError::BufferTooSmall;
        return false;
    }

// The total wire size is the fixed header, the 1-2 byte Remaining Length field and this
// body. Check the broker's declared cap before allocating an identifier or recording an
// outstanding subscription, so a refused request changes no connection state.
    const uint32_t remainingLength = (uint32_t)4 + propertyBytes +
                                     topicLength + optionBytes;
    if ((variableByteIntegerSize(remainingLength) == 0) ||
        exceedsServerMaximumPacketSize(remainingLength)) {
        this->errorState = MqttError::PacketTooLarge;
        return false;
    }
    if (connected()) {
        if (mqtt5 && (this->inFlightCount() >= MQTT_MAX_OUTSTANDING)) {
            this->errorState = MqttError::PacketIdUnavailable;
            return false;
        }
        // Leave room in the buffer for header and variable length field
        uint16_t length = MQTT_MAX_HEADER_SIZE;
// Requirement 8.4: the identifier comes from the SHARED allocator, so a subscription can
// never collide with an outbound QoS 1/2 publish. The rotor is still `nextMsgId` and it
// still advances by one per call, so with an empty table - every call during runtime
// level-4 fallback, and the overwhelmingly common level-5 case - the emitted identifier
// is exactly what `nextMsgId++` produced before.
        const uint16_t packetId = allocatePacketId();
        if (packetId == 0) {
// Unreachable: a bounded table cannot hold all 65535 identifiers (see the static_assert
// on `outstanding`). Handled rather than asserted because 0 is not a legal Packet
// Identifier, so there is no packet to frame - and refusing here transmits nothing.
            return false;
        }
        MqttOutstandingMessage* entry = nullptr;
        if (mqtt5) {
// A live SUBSCRIBE/UNSUBSCRIBE must own a table entry BEFORE its first byte is written.
// The fixed MQTT_MAX_PENDING_SUBSCRIPTIONS margin reserves room for Tasmota's documented
// reconnect burst; a full tracked envelope is a retryable atomic refusal, never eviction or
// an untracked wire operation (Task 18.6 / Finding 6).
            const MqttOutState state = (packetType == MQTTSUBSCRIBE)
                                         ? MqttOutState::AwaitingSuback
                                         : MqttOutState::AwaitingUnsuback;
            entry = claimOutstanding(packetId, state, topic);
            if (entry == nullptr) {
                this->errorState = MqttError::PacketIdUnavailable;
                return false;
            }
            entry->qos = (options != nullptr) ? options->maxQos : 0;
        }
        this->buffer[length++] = (packetId >> 8);
        this->buffer[length++] = (packetId & 0xFF);
        if (mqtt5) {
// Requirements 9.1, 9.3: the Properties field sits after the Packet Identifier and before
// the first Topic Filter, in both packet types. This client has no SUBSCRIBE or
// UNSUBSCRIBE property to send, so the field is always the empty one - a Property Length
// of 0, encoded as the single byte 0x00. The capacity for it was reserved in `overhead`.
            this->buffer[length++] = 0x00;
        }
        length = writeString(topic, this->buffer,length);
        if (options != nullptr) {
// Requirement 9.2: exactly one Subscription Options byte, immediately after the filter.
            this->buffer[length++] = subscriptionOptionsByte(*options);
        }
// The 0x02 in the fixed header is the mandatory reserved-bit value for SUBSCRIBE and
// UNSUBSCRIBE, not a requested QoS - which is why it is spelled MQTTQOS1 here.
        const boolean sent = write(packetType|MQTTQOS1,this->buffer,length-MQTT_MAX_HEADER_SIZE);
        if (!sent && (entry != nullptr)) {
// Nothing (or not all) of the packet reached the transport, so no acknowledgement will
// ever correlate to this identifier: give the slot back.
            releaseOutstanding(entry);
        } else if (sent && (packetIdOut != nullptr)) {
// Report the Packet Identifier only after the complete packet has reached the transport.
            *packetIdOut = packetId;
        }
        return sent;
    }
    return false;
}

boolean PubSubClient::subscribe(const char* topic) {
    return subscribe(topic, (uint8_t)0);
}

boolean PubSubClient::subscribe(const char* topic, uint8_t qos) {
    MqttSubscribeOptions options;
    options.maxQos = qos;
    return sendSubscription(MQTTSUBSCRIBE, topic, &options, nullptr);
}

boolean PubSubClient::subscribe(const char* topic, const MqttSubscribeOptions& options) {
    return subscribe(topic, options, nullptr);
}

boolean PubSubClient::subscribe(const char* topic, const MqttSubscribeOptions& options,
                                uint16_t* packetIdOut) {
    return sendSubscription(MQTTSUBSCRIBE, topic, &options, packetIdOut);
}

boolean PubSubClient::unsubscribe(const char* topic) {
    return sendSubscription(MQTTUNSUBSCRIBE, topic, nullptr, nullptr);
}

boolean PubSubClient::unsubscribeWithPacketId(const char* topic, uint16_t* packetIdOut) {
// No Subscription Options byte: UNSUBSCRIBE carries bare Topic Filters.
    return sendSubscription(MQTTUNSUBSCRIBE, topic, nullptr, packetIdOut);
}

// --- Outstanding operation table -------------------------------------------

MqttOutstandingMessage* PubSubClient::claimOutstanding(uint16_t packetId, MqttOutState state,
                                                      const char* topic) {
    for (uint8_t i = 0; i < MQTT_MAX_OUTSTANDING; i++) {
        MqttOutstandingMessage& slot = this->outstanding[i];
        if (slot.state == MqttOutState::Free) {
            slot.packetId = packetId;
            slot.state = state;
            slot.qos = 0;
            slot.reasonCode = 0;
            slot.timedOutReported = false;
// Stamped here, not by the caller: this is the instant the entry starts waiting, so it is
// the instant the local timeout diagnostic measures from. The entry and Packet Identifier
// remain held after that diagnostic until a terminal acknowledgement or session teardown.
            slot.sentAt = millis();
// The FULL topic/filter, never a prefix (Requirement 13.10). One short heap buffer per
// occupied slot, taken here - once per operation - and never on the packet path.
            slot.topic = (topic != nullptr) ? topic : "";
            return &slot;
        }
    }
    return nullptr;
}

MqttOutstandingMessage* PubSubClient::findOutstanding(uint16_t packetId) {
// 0 is not a legal Packet Identifier, so it matches nothing rather than matching a free
// slot whose identifier happens to be 0.
    if (packetId == 0) {
        return nullptr;
    }
    for (uint8_t i = 0; i < MQTT_MAX_OUTSTANDING; i++) {
        MqttOutstandingMessage& slot = this->outstanding[i];
        if ((slot.state != MqttOutState::Free) && (slot.packetId == packetId)) {
            return &slot;
        }
    }
    return nullptr;
}

void PubSubClient::releaseOutstanding(MqttOutstandingMessage* entry) {
    if (entry == nullptr) {
        return;
    }
// The acknowledgement sequence has ended - completed, refused or abandoned - so there is
// nothing left that could ever be retransmitted: the retained body goes back to the heap and
// its bytes back to the budget (Requirement 13.11). Every release path in the library funnels
// through here, so this one line is what keeps the budget honest.
#ifdef USE_MQTT_QOS
    if (MQTT_RUNTIME_IS_5()) {
        releasePacketCopy(*entry);
    }
#endif  // USE_MQTT_QOS
    entry->state = MqttOutState::Free;
    entry->packetId = 0;
    entry->timedOutReported = false;
    entry->sentAt = 0;
// `topic` is deliberately NOT cleared: a MqttSubscriptionResult::topicFilter taken from it
// stays readable until this slot is claimed again, which is the lifetime documented on that
// struct. Clearing it here would blank the filter exactly when a driver reads
// lastSubscriptionResult() after loop() returned.
}

void PubSubClient::clearOutstanding(bool preserveResumable) {
    for (uint8_t i = 0; i < MQTT_MAX_OUTSTANDING; i++) {
        MqttOutstandingMessage& slot = this->outstanding[i];
        if (MQTT_RUNTIME_IS_5()) {
// Every held operation belongs to local Session State, even when MQTT defines no packet to
// replay for it. Preserve the complete state only while Task 18.7's caller has proved that
// the next CONNECT names the same persistent session. Clean Start and every identity mismatch
// pass preserveResumable=false, preventing an old identifier or retained byte from reaching a
// different session.
            if (preserveResumable && (slot.state != MqttOutState::Free)) {
                continue;
            }
#ifdef USE_MQTT_QOS
// Cleared, so any body it does hold is released rather than leaked.
            releasePacketCopy(slot);
#endif  // USE_MQTT_QOS
        }
        slot.packetId = 0;
        slot.state = MqttOutState::Free;
        slot.qos = 0;
        slot.reasonCode = 0;
        slot.timedOutReported = false;
        slot.sentAt = 0;
        slot.topic.clear();
    }
// Every retained filter is now empty, so the last reported result must not keep pointing
// at one as though it still named a filter.
    this->lastSubResult.topicFilter = nullptr;
}

// --- Packet Identifier allocation (Requirements 8.4, 8.5) ------------------

uint16_t PubSubClient::allocatePacketId() {
// At most MQTT_MAX_OUTSTANDING identifiers are held, so among MQTT_MAX_OUTSTANDING + 1
// distinct candidates at least one is free. Each step advances the rotor, so a rejected
// candidate is never retried and issued identifiers remain monotonic modulo the wrap.
    for (uint16_t attempt = 0; attempt <= MQTT_MAX_OUTSTANDING; attempt++) {
        this->nextMsgId++;
        if (this->nextMsgId == 0) {
// 65535 -> 1: 0 is not a legal Packet Identifier, so the rotor skips it on every wrap.
            this->nextMsgId = 1;
        }
        if (findOutstanding(this->nextMsgId) == nullptr) {
            return this->nextMsgId;
        }
    }
// Unreachable with a bounded table; see the contract on the declaration.
    return 0;
}

void PubSubClient::releasePacketId(uint16_t id) {
// The table IS the held-identifier set, so releasing the identifier and freeing the entry
// that holds it are the same act. findOutstanding() already treats 0 and unknown
// identifiers as "nothing matches", and releaseOutstanding() tolerates a null entry.
    releaseOutstanding(findOutstanding(id));
}

// --- In-flight retention and retransmission (Requirements 8.7, 8.19 - 8.24) -
//
// Compiled only when USE_MQTT_QOS is defined: this whole section is the outbound QoS 1/2
// replay buffer and retransmission machinery. Once QoS 1/2 is offered at all, retransmission
// cannot be a separate toggle underneath it, because a caller cannot retransmit on its own
// behalf - the public API hands out neither the Packet Identifier nor the assembled bytes - so
// retransmission is this library's job or it does not exist. Entry points are additionally
// reached only while `MQTT_RUNTIME_IS_5()` is true, so runtime level-4 fallback bypasses this
// machinery. [MQTT-4.4.0-1] forbids resending at any moment other than the one
// retransmitOutstanding() implements.
#ifdef USE_MQTT_QOS

boolean PubSubClient::holdsRetainedBody(const MqttOutstandingMessage& slot) {
    return (slot.packetBody != nullptr) &&
           ((slot.state == MqttOutState::AwaitingPuback) ||
            (slot.state == MqttOutState::AwaitingPubrec));
}

boolean PubSubClient::resendsOnSessionResume(const MqttOutstandingMessage& slot) {
// Two disjoint kinds, resending two different packets:
//
//   - a PUBLISH replay from the retained body, for a PUBLISH still unacknowledged
//     (Requirements 8.7, 8.22);
//   - a PUBREL resend, for an entry whose PUBREC arrived and whose PUBREL went out but whose
//     PUBCOMP never came. It is assembled from the Packet Identifier alone, so it needs no
//     retained body and costs no budget (Requirement 8.23).
//
// Everything else - a QoS 0 publish (which holds no entry at all), a subscription, a streaming
// publish, a publish the budget would not stretch to - has nothing to resend and is therefore
// abandoned by a teardown like any other entry.
    return holdsRetainedBody(slot) || (slot.state == MqttOutState::AwaitingPubcomp);
}
#endif  // USE_MQTT_QOS

void PubSubClient::clearSessionIdentity() {
    const bool fallbackSelected =
        this->sessionIdentity.protocolLevel == MQTT_VERSION_3_1_1;
    this->sessionIdentity.endpointDomain.clear();
    this->sessionIdentity.clientId.clear();
    this->sessionIdentity.valid = false;
    this->sessionIdentity.endpointIsDomain = false;
    memset(this->sessionIdentity.endpointAddress, 0,
           sizeof(this->sessionIdentity.endpointAddress));
    this->sessionIdentity.endpointPort = 0;
    this->sessionIdentity.protocolLevel = fallbackSelected
                                            ? MQTT_VERSION_3_1_1 : 0;
    this->sessionIdentity.cleanSession = true;
    this->sessionIdentity.sessionExpiryInterval = 0;
}

void PubSubClient::captureSessionIdentity(const char* clientId, bool cleanSession) {
    clearSessionIdentity();
    if (cleanSession || (clientId == nullptr)) {
        return;
    }

    this->sessionIdentity.endpointIsDomain = (this->domain.length() != 0);
    if (this->sessionIdentity.endpointIsDomain) {
        this->sessionIdentity.endpointDomain = this->domain;
    } else {
        for (uint8_t i = 0; i < 4; ++i) {
            this->sessionIdentity.endpointAddress[i] = this->ip[i];
        }
    }
    this->sessionIdentity.endpointPort = this->port;
    this->sessionIdentity.clientId = clientId;
    this->sessionIdentity.protocolLevel = MQTT_RUNTIME_PROTOCOL();
    this->sessionIdentity.cleanSession = false;
    this->sessionIdentity.sessionExpiryInterval = this->persistentSessionExpiryInterval;

// String assignment can fail on a constrained device. A failed identity capture must
// conservatively disable resumption rather than treating a truncated/empty identity as
// a match for a later peer.
    const bool endpointCopied = !this->sessionIdentity.endpointIsDomain ||
                                (this->sessionIdentity.endpointDomain == this->domain);
    this->sessionIdentity.valid = endpointCopied &&
                                  (this->sessionIdentity.clientId == clientId);
    if (!this->sessionIdentity.valid) {
        clearSessionIdentity();
    }
}

bool PubSubClient::sessionIdentityMatches(const char* clientId, bool cleanSession) const {
    if ((clientId == nullptr) || cleanSession || !this->sessionIdentity.valid ||
        this->sessionIdentity.cleanSession ||
        (this->sessionIdentity.sessionExpiryInterval == 0) ||
        (this->sessionIdentity.protocolLevel != MQTT_RUNTIME_PROTOCOL()) ||
        (this->sessionIdentity.endpointPort != this->port) ||
        (this->sessionIdentity.sessionExpiryInterval !=
         this->persistentSessionExpiryInterval) ||
        !(this->sessionIdentity.clientId == clientId)) {
        return false;
    }

    const bool endpointIsDomain = (this->domain.length() != 0);
    if (this->sessionIdentity.endpointIsDomain != endpointIsDomain) {
        return false;
    }
    if (endpointIsDomain) {
        return this->sessionIdentity.endpointDomain == this->domain;
    }
    for (uint8_t i = 0; i < 4; ++i) {
        if (this->sessionIdentity.endpointAddress[i] != this->ip[i]) {
            return false;
        }
    }
    return true;
}

bool PubSubClient::hasResumableLocalSessionState(const char* clientId,
                                                 bool cleanSession) const {
// A successfully established matching persistent identity is itself evidence of compatible
// local Session State: the broker may retain subscriptions even when this client currently
// has no replayable outbound packet. Outstanding entries, when present, are already bound to
// this identity and are preserved or cleared with it.
    return sessionIdentityMatches(clientId, cleanSession);
}

#ifdef USE_MQTT_QOS
boolean PubSubClient::retainPacketCopy(MqttOutstandingMessage* entry, uint8_t headerByte,
                                      const uint8_t* body, uint16_t bodyLen) {
    if ((entry == nullptr) || (body == nullptr) || (bodyLen == 0) ||
        (entry->packetBody != nullptr)) {
        return false;
    }

// This is the pre-write half of the QoS 1/2 admission transaction. Check the budget before
// allocating, then allocate and copy before publishing any member or accounting state. A false
// return leaves the tentative entry and the retention budget byte-for-byte unchanged.
    if ((uint32_t)this->inflightCopyBytes + (uint32_t)bodyLen >
        (uint32_t)MQTT_INFLIGHT_COPY_BUDGET) {
        return false;
    }

    uint8_t* copy = (uint8_t*)malloc(bodyLen);
    if (copy == nullptr) {
        return false;
    }
    memcpy(copy, body, bodyLen);
    entry->packetBody = copy;
    entry->packetBodyLen = bodyLen;
    entry->headerByte = headerByte;
    this->inflightCopyBytes += bodyLen;
    return true;
}

void PubSubClient::releasePacketCopy(MqttOutstandingMessage& entry) {
    if (entry.packetBody == nullptr) {
        return;
    }
    free(entry.packetBody);
    entry.packetBody = nullptr;
// Give the bytes back before zeroing the length, so the budget can never drift.
    this->inflightCopyBytes -= entry.packetBodyLen;
    entry.packetBodyLen = 0;
    entry.headerByte = 0;
}

void PubSubClient::replayFailure(MqttError error, uint16_t packetId) {
// A replay may have written a prefix of its packet. Do not append DISCONNECT to that
// desynchronized stream; record precisely which replay failed, close silently, and let
// resetConnectionState() preserve only state that remains eligible for a future Session
// Present decision.
    this->errorState = error;
    this->errorReasonCode = 0;
    this->errorPacketId = packetId;
    if (this->_client != nullptr) {
        this->_client->flush();
        this->_client->stop();
    }
    this->resetConnectionState();
}

bool PubSubClient::retransmitOutstanding(const char* clientId, bool cleanSession) {
// Slots are visited in ascending table order. The first failing replay terminates the
// connection attempt, so later slots are never handed to the transport.
    for (uint8_t i = 0; i < MQTT_MAX_OUTSTANDING; i++) {
        MqttOutstandingMessage& slot = this->outstanding[i];
        if (!resendsOnSessionResume(slot)) {
            continue;
        }

// Task 18.7 normally cleared these slots before CONNECT when the identity changed. Keep
// this per-entry guard as defense in depth: retained bytes must never reach a peer unless
// the exact persistent-session identity that owns them still matches this CONNECT.
        if (!this->sessionIdentityMatches(clientId, cleanSession)) {
            replayFailure(MqttError::ProtocolError, slot.packetId);
            return false;
        }

        if (slot.state == MqttOutState::AwaitingPubcomp) {
// PUBREL's successful short form has Remaining Length 2 and total wire size 4. Check the
// newly negotiated broker limit and the current working buffer before its first byte.
            if ((this->buffer == nullptr) || (this->bufferSize < 4)) {
                replayFailure(MqttError::BufferTooSmall, slot.packetId);
                return false;
            }
            if (exceedsServerMaximumPacketSize(2)) {
                this->errorPacketId = slot.packetId;
                replayFailure(this->errorState, slot.packetId);
                return false;
            }
            this->errorPacketId = slot.packetId;
            if (!sendAcknowledgement(MQTTPUBREL, slot.packetId, 0)) {
// sendAcknowledgement() has already recorded WriteFailed and silently closed a short
// write. Its reset preserves replayable state, and the Packet Identifier was latched above.
                return false;
            }
            slot.sentAt = millis();
            continue;
        }

// The only other resumable state is an unacknowledged QoS 1/2 PUBLISH holding its complete
// body. A corrupt slot is a local framing failure, never a reason to skip it and replay a
// later entry.
        if (!holdsRetainedBody(slot)) {
            replayFailure(MqttError::ProtocolError, slot.packetId);
            return false;
        }
        if ((this->buffer == nullptr) ||
            ((uint32_t)MQTT_MAX_HEADER_SIZE + (uint32_t)slot.packetBodyLen >
             (uint32_t)this->bufferSize)) {
            replayFailure(MqttError::BufferTooSmall, slot.packetId);
            return false;
        }
        if (exceedsServerMaximumPacketSize(slot.packetBodyLen)) {
            this->errorPacketId = slot.packetId;
            replayFailure(this->errorState, slot.packetId);
            return false;
        }

// Bit 3 is the sole difference from the initial PUBLISH. The retained body already owns the
// original Packet Identifier and Properties, so replay needs neither caller data nor allocation.
        const uint8_t header = (uint8_t)(slot.headerByte | 0x08);
        memcpy(this->buffer + MQTT_MAX_HEADER_SIZE, slot.packetBody, slot.packetBodyLen);
        this->errorPacketId = slot.packetId;
        if (!write(header, this->buffer, slot.packetBodyLen)) {
// write() records WriteFailed and closes on a short write. Framing was prevalidated above,
// so no later replay can follow either kind of failure.
            return false;
        }
        slot.sentAt = millis();
    }
    return true;
}
#endif  // USE_MQTT_QOS

void PubSubClient::discardRetransmittable() {
// Session Present 0 definitively ends every held operation, including SUBSCRIBE/UNSUBSCRIBE
// states that had no packet eligible for automatic replay. Report each through the generic
// diagnostic accessors first, then clearOutstanding(false) frees every entry and its
// retained body.
    for (uint8_t i = 0; i < MQTT_MAX_OUTSTANDING; i++) {
        const MqttOutstandingMessage& slot = this->outstanding[i];
        if (slot.state != MqttOutState::Free) {
            reportAbandoned(slot);
        }
    }
    clearOutstanding(false);
}

#ifdef USE_MQTT_QOS
uint16_t PubSubClient::outstandingPublishCount() const {
    uint16_t count = 0;
    for (uint8_t i = 0; i < MQTT_MAX_OUTSTANDING; i++) {
        const MqttOutState state = this->outstanding[i].state;
        if ((state == MqttOutState::AwaitingPuback) ||
            (state == MqttOutState::AwaitingPubrec) ||
            (state == MqttOutState::AwaitingPubcomp)) {
            count++;
        }
    }
    return count;
}
#endif  // USE_MQTT_QOS

uint16_t PubSubClient::inFlightCount() const {
    uint16_t count = 0;
    for (uint8_t i = 0; i < MQTT_MAX_OUTSTANDING; i++) {
        if (this->outstanding[i].state != MqttOutState::Free) {
            count++;
        }
    }
    return count;
}

void PubSubClient::reportAbandoned(const MqttOutstandingMessage& entry) {
// Requirement 8.6: the entry's state is retained until its acknowledgement sequence
// completes OR it is abandoned. This is the report for the second case, and it is
// deliberately the same two accessors a failing SUBACK Reason Code is reported through -
// which identifier, and with which code. `errorState` is NOT set: one operation being
// abandoned is not a transport failure, and overwriting it would blank the reason a
// protocolFailure() is tearing the connection down in the first place.
    this->errorPacketId = entry.packetId;
    this->errorReasonCode = MQTT_RC_UNSPECIFIED_ERROR;
}

void PubSubClient::abandonExpiredOutstanding(unsigned long now) {
// MQTT_ACK_TIMEOUT of 0 disables local timeout diagnostics, the same way `keepAlive == 0`
// disables the keepalive. A timeout is NOT a terminal exchange event: it may publish one
// diagnostic, but the entry and its Packet Identifier remain held for a late valid ACK.
    if (MQTT_ACK_TIMEOUT == 0) {
        return;
    }
    const unsigned long deadline = (unsigned long)MQTT_ACK_TIMEOUT * 1000UL;
    for (uint8_t i = 0; i < MQTT_MAX_OUTSTANDING; i++) {
        MqttOutstandingMessage& slot = this->outstanding[i];
        if (slot.state == MqttOutState::Free) {
            continue;
        }
// Unsigned difference of the sampled stamps, so the comparison stays correct across a
// millis() wrap. `timedOutReported` prevents repeated loop sweeps from repeatedly
// overwriting the caller-visible diagnostic for the same still-live exchange.
        if (!slot.timedOutReported && ((now - slot.sentAt) > deadline)) {
            reportAbandoned(slot);
            slot.timedOutReported = true;
        }
    }
}

// --- PUBACK correlation for outbound QoS 1 (Requirements 7.3, 7.4, 8.16) ---
#ifdef USE_MQTT_QOS

void PubSubClient::reportPublishAck(uint16_t packetId, uint8_t reasonCode) {
    MqttOutstandingMessage* entry = findOutstanding(packetId);
// Requirement 7.8: an acknowledgement whose Packet Identifier matches no outstanding
// message is discarded with the state of every outstanding message left unchanged. The
// state check is part of that same discard rather than an extra rule: one allocator serves
// PUBLISH, SUBSCRIBE and UNSUBSCRIBE, so an identifier is held by exactly one operation,
// and a PUBACK naming a SUBSCRIBE's identifier is the broker acknowledging a QoS 1 PUBLISH
// this client never sent. Reporting it would attribute a publish result to an operation
// that has none, and releasing the entry would drop a subscription still waiting for its
// SUBACK.
    if ((entry == nullptr) || (entry->state != MqttOutState::AwaitingPuback)) {
        return;
    }
// Requirements 7.3, 7.4: the numeric Reason Code - already defaulted to 0x00 by the parser
// when the short form omitted it - is exposed to the caller together with the identifier it
// belongs to, through the same pair of accessors reportAbandoned() and reportSubscriptionAck()
// use, so a driver reads "which operation, and with which code" in one place. Latched for
// every code and not only for the failure range: the completion of Requirement 8.16 is not
// observable otherwise, and a caller that wants to confirm the broker took responsibility
// for a message has to be able to read the success code too.
    entry->reasonCode = reasonCode;
    this->errorPacketId = packetId;
    this->errorReasonCode = reasonCode;
// Requirement 8.16: the Outstanding_Message is complete and its identifier goes back into
// the allocator's free set - which, the table being the held-identifier set, is the same act
// as freeing the entry. Doing this for a Reason Code of 0x80 or above exactly as for a
// success code is what makes such a code TERMINAL ([MQTT-4.4.0-2]): the PUBLISH counts as
// acknowledged rather than as still in flight, and no entry survives for a retransmission to
// pick up. `errorState` is deliberately untouched - a broker refusing one message is a
// normal answer, not a transport failure, and overwriting it would blank the reason a
// protocolFailure() may be tearing the connection down for.
    releaseOutstanding(entry);
}

// --- Outbound QoS 2 handshake (Requirements 8.12, 8.14) --------------------

boolean PubSubClient::reportPublishRec(uint16_t packetId, uint8_t reasonCode,
                                      unsigned long now) {
    MqttOutstandingMessage* entry = findOutstanding(packetId);
// Requirement 7.8: an acknowledgement correlating to nothing is discarded with the state of
// every outstanding message left unchanged. The state half of the guard is what makes
// Requirement 8.12's "send a PUBREL" mean exactly one PUBREL: once the PUBREL has gone out
// the entry sits in AwaitingPubcomp, so a duplicate PUBREC for the same identifier finds no
// entry in AwaitingPubrec and is discarded rather than answered a second time. It also keeps
// a PUBREC naming a SUBSCRIBE's or a QoS 1 PUBLISH's identifier - the broker acknowledging a
// QoS 2 PUBLISH this client never sent - from disturbing that operation.
    if ((entry == nullptr) || (entry->state != MqttOutState::AwaitingPubrec)) {
        return false;
    }
    if (reasonCode >= 0x80) {
// Requirement 8.14: the broker refused the message, so the handshake stops here. NO PUBREL
// is written - there is nothing left to release - the identifier goes back to the allocator,
// and the received code plus that released identifier are what the caller reads, through the
// same two accessors a failing PUBACK, a failing SUBACK and an abandoned entry report
// through. `errorState` is deliberately untouched: one refused message is a normal broker
// answer, not a transport failure.
        entry->reasonCode = reasonCode;
        this->errorPacketId = packetId;
        this->errorReasonCode = reasonCode;
        releaseOutstanding(entry);
        return false;
    }
// Requirement 8.12: a Reason Code below 0x80 means the broker has taken responsibility for
// the message, and the client answers with PUBREL carrying the SAME identifier. Framed by
// the shared acknowledgement assembler, which sets the mandatory fixed-header flags 0x02 for
// this packet type (Requirement 7.7) and uses the short form for the success Reason Code
// 0x00 (Requirement 7.6).
    if (!sendAcknowledgement(MQTTPUBREL, packetId, 0)) {
// The transport refused the bytes, so no PUBREL reached the broker and no PUBCOMP can
// follow. The entry stays in AwaitingPubrec on purpose: the deadline sweep or the teardown
// then abandons it and reports the released identifier, which is the truth, whereas moving
// it to AwaitingPubcomp would leave it waiting for an answer to a packet that was never
// sent.
        return false;
    }
// The exchange has advanced but is NOT complete: the entry keeps its slot and its
// identifier until the PUBCOMP arrives. The Reason Code is recorded on the entry only -
// latching a mid-handshake success into lastReasonCode() would overwrite a diagnostic the
// caller may not have read yet, and Requirement 8.12's observable outcome is the PUBREL and
// the eventual completion.
    entry->state = MqttOutState::AwaitingPubcomp;
    entry->reasonCode = reasonCode;
    entry->timedOutReported = false;
// The PUBLISH is done as far as the broker is concerned, so its Retained_Packet_Body holds
// nothing worth replaying: released here, which gives its bytes back to the
// MQTT_INFLIGHT_COPY_BUDGET at the earliest honest moment rather than pinning them for the
// whole PUBCOMP wait. What this entry resends on a resumed session is a PUBREL assembled from
// its Packet Identifier alone, at no budget cost (Requirement 8.23).
    releasePacketCopy(*entry);
// Restamped so the MQTT_ACK_TIMEOUT window applies to the wait for the PUBCOMP rather than
// being consumed by the time the PUBREC already took.
    entry->sentAt = now;
    return true;
}

void PubSubClient::reportPublishComp(uint16_t packetId, uint8_t reasonCode) {
    MqttOutstandingMessage* entry = findOutstanding(packetId);
// Requirement 7.8 again, and the mirror of the PUBREC guard: only an entry whose PUBREL this
// client has actually sent can be completed by a PUBCOMP. A PUBCOMP for an identifier still
// in AwaitingPubrec would be the broker skipping a step, and one naming a subscription's
// identifier would drop an operation still waiting for its SUBACK.
    if ((entry == nullptr) || (entry->state != MqttOutState::AwaitingPubcomp)) {
        return;
    }
// Requirement 8.12: the exchange completes here. Reported exactly as reportPublishAck()
// reports a QoS 1 completion - which identifier, and with which code, the parser having
// already defaulted an absent Reason Code to 0x00 - and the entry is released so its
// identifier returns to the allocator's free set. Latched for every code, success included,
// because completion is not observable otherwise.
    entry->reasonCode = reasonCode;
    this->errorPacketId = packetId;
    this->errorReasonCode = reasonCode;
    releaseOutstanding(entry);
}
#endif  // USE_MQTT_QOS

// --- SUBACK / UNSUBACK reporting (Requirements 9.4, 9.5, 9.6) --------------

void PubSubClient::reportSubscriptionAck(uint8_t packetType, uint16_t packetId,
                                        const uint8_t* codes, uint32_t codeCount) {
    MqttOutstandingMessage* entry = findOutstanding(packetId);
    const bool isSuback = (packetType == MQTTSUBACK);
    const MqttOutState expectedState = isSuback ? MqttOutState::AwaitingSuback
                                                : MqttOutState::AwaitingUnsuback;
// A SUBACK/UNSUBACK completes exactly the operation it acknowledges. An identifier can be
// held by a PUBLISH or by the opposite subscription operation, so matching the identifier
// alone would report a spurious result and release a still-live exchange. The dispatcher
// has already proven the packet structurally and semantically valid; this final gate keeps
// its one Reason Code from changing any observable state unless the pending operation type
// also matches (Task 18.5 / Finding 1).
    if ((codeCount != 1) || (entry == nullptr) || (entry->state != expectedState)) {
        return;
    }
// Requirement 9.5: the FULL filter this client sent for `packetId`, straight out of the
// matching entry's retained String.
    const char* filter = entry->topic.c_str();

    for (uint32_t i = 0; i < codeCount; i++) {
        const uint8_t rc = codes[i];
        MqttSubscriptionResult result;
        result.packetId = packetId;
        result.reasonCode = rc;
        result.topicFilter = filter;
        result.unsubscribe = !isSuback;
// Requirement 9.4: a SUBACK reports success ONLY for the granted-QoS codes 0x00, 0x01 and
// 0x02 - never unconditionally, and not merely "below 0x80": the codes between 0x03 and
// 0x7F grant no QoS, so they are not a granted subscription either.
// Requirement 9.6: on UNSUBACK, 0x00 (Success) and 0x11 (No subscription existed) are both
// non-failures - the subscription is gone in either case.
        result.granted = isSuback ? (rc <= 0x02) : ((rc == 0x00) || (rc == 0x11));
        this->lastSubResult = result;
        if (!result.granted) {
// Requirement 14.2 diagnostics: which identifier failed, and with which Reason Code. This
// does NOT close the connection - a rejected subscription is a normal broker answer, not a
// protocol error.
            this->errorPacketId = packetId;
            this->errorReasonCode = rc;
        }
        if (entry != nullptr) {
            entry->reasonCode = rc;
        }
        MqttSubscribeCallback callbackSnapshot = this->subscribeCallback;
        void* contextSnapshot = this->subscribeCallbackCtx;
        if (callbackSnapshot != nullptr) {
// The stored pair is snapped consistently before invocation. A reentrant callback may
// replace it for future results without changing the context used by this call.
            callbackSnapshot(result, contextSnapshot);
        }
    }
// The operation is complete either way: the broker has answered. Releasing keeps the
// retained filter intact so lastSubscriptionResult() stays readable (see
// releaseOutstanding).
    releaseOutstanding(entry);
}

PubSubClient& PubSubClient::setSubscribeCallback(MqttSubscribeCallback cb, void* ctx) {
    this->subscribeCallback = cb;
    this->subscribeCallbackCtx = ctx;
    return *this;
}

boolean PubSubClient::socketWritable() {
// "Writable" is deliberately shallow: the transport still exists and still reports a
// live connection. Anything deeper (a probe write) would itself be a side effect, and
// a socket that reports connected but refuses the bytes is indistinguishable from one
// whose DISCONNECT was dropped in flight - both are already conformant outcomes.
    if ((_client == nullptr) || (this->buffer == nullptr)) {
        return false;
    }
    return _client->connected() != 0;
}

void PubSubClient::sendDisconnect(uint8_t reasonCode, bool shortForm) {
    if ((_client == nullptr) || (this->buffer == nullptr)) {
        return;
    }
    size_t expected = 0;
    if (shortForm) {
// `E0 00` - the MQTT 5.0 short form, byte-for-byte identical to the normal DISCONNECT
// emitted during runtime level-4 fallback (Requirements 10.3, 10.4).
        if (this->bufferSize < 2) {
            return;
        }
        this->buffer[0] = MQTTDISCONNECT;
        this->buffer[1] = 0;
        expected = 2;
    } else {
// `E0 01 <rc>` - the error form, Properties omitted. It is emitted only while
// `MQTT_RUNTIME_IS_5()` is true, never after runtime level-4 fallback.
        if (this->bufferSize < 3) {
            return;
        }
        this->buffer[0] = MQTTDISCONNECT;
        this->buffer[1] = 1;
        this->buffer[2] = reasonCode;
        expected = 3;
    }
    transportWrite(this->buffer, expected);
}

boolean PubSubClient::sendAcknowledgement(uint8_t packetType, uint16_t packetId,
                                         uint8_t reasonCode) {
    if ((_client == nullptr) || (this->buffer == nullptr)) {
        return false;
    }
// Requirement 7.2 / 7.6: the short form - Remaining Length 2, no Reason Code byte, no
// Properties field - whenever the Reason Code is 0x00, which is every acknowledgement
// this client emits. Its four bytes are byte-for-byte identical at runtime protocol
// levels 5 and 4, so routing that path through here changes nothing on the wire.
//
// A nonzero Reason Code needs the MQTT5 3-byte form; `MQTT_RUNTIME_IS_5()` prevents it
// from being framed after runtime level-4 fallback.
    const boolean shortForm = (!MQTT_RUNTIME_IS_5()) || (reasonCode == 0);
    const uint16_t total = shortForm ? 4 : 5;
    const uint32_t remainingLength = (uint32_t)(total - 2);
    if (exceedsServerMaximumPacketSize(remainingLength)) {
        return false;
    }
    if (this->bufferSize < total) {
        return false;
    }
    uint8_t header = packetType;
    if (packetType == MQTTPUBREL) {
// Requirement 7.7: PUBREL's fixed-header flags are 0x02 - the same reserved-bit value
// SUBSCRIBE and UNSUBSCRIBE carry, which is why it is spelled MQTTQOS1 there too. It is
// a mandatory constant, not a requested QoS.
        header |= MQTTQOS1;
    }
    this->buffer[0] = header;
    this->buffer[1] = (uint8_t)(total - 2);            // Remaining Length: 2 or 3
    this->buffer[2] = (uint8_t)(packetId >> 8);
    this->buffer[3] = (uint8_t)(packetId & 0xFF);
    if (!shortForm) {
        this->buffer[4] = reasonCode;
    }
    const size_t written = transportWrite(this->buffer, total);
    if (written != total) {
        transportWriteFailed();
        return false;
    }
    return true;
}

void PubSubClient::resetConnectionState() {
    _state = MQTT_DISCONNECTED;
    // No streaming publish may survive a transport teardown. Its wire frame is
    // either complete already or irrecoverably partial and must never resume.
    publishInProgress = false;
    publishBytesExpected = 0;
    publishBytesWritten = 0;
    // A stale outstanding ping would otherwise time out the next connection immediately
    pingOutstanding = false;

    lastInActivity = lastOutActivity = millis();

    if (MQTT_RUNTIME_IS_5()) {
// The negotiated limits govern transmission on the connection being torn down, so they
// must not survive it (Requirements 5.7, 14.2). Cleared here as well as at the top of
// connect() - the attempt boundary Requirement 5.7 names - so a limit cannot leak into
// the window between a teardown and the next attempt either.
        resetNegotiatedLimits();
        clearTopicAliases();
// Local Session State can survive this transport teardown only for a previously established
// persistent identity with a nonzero expiry. When it survives, every held operation remains
// pending until the next CONNACK; only replayable publish states are emitted again.
        const bool preserveResumable = this->sessionIdentity.valid &&
                                      !this->sessionIdentity.cleanSession &&
                                      (this->sessionIdentity.sessionExpiryInterval != 0);
// If no persistent session can survive, every outstanding operation is definitively
// abandoned here. Suppress this generic diagnostic when the teardown already recorded a more
// specific protocol or transport error.
        if (!preserveResumable && (this->errorState == MqttError::None)) {
            for (uint8_t i = 0; i < MQTT_MAX_OUTSTANDING; i++) {
                const MqttOutstandingMessage& slot = this->outstanding[i];
                if (slot.state != MqttOutState::Free) {
                    reportAbandoned(slot);
                }
            }
        }
        clearOutstanding(preserveResumable);
        if (!preserveResumable) {
            clearSessionIdentity();
            clearInboundQos2();
        }
    }

// Extension point for the remaining per-connection state (Requirement 14.2). Each of the
// structures below is introduced by a later task and MUST be cleared here as it lands, so
// that no value survives into the next connection:
//   - the packet-wide receive deadline (task 14 / F-03)
//   - the outstanding (unacknowledged) outbound message table
//   - the inbound QoS 2 packet identifier registry
//   - the outbound topic alias table
// The negotiated limits latched from CONNACK (server keep alive, maximum packet size,
// topic alias maximum, receive maximum) are cleared above, in the MQTT_RUNTIME_IS_5() block.
// Only state that exists today is cleared above.
//
// `sessionPresent` and `connackReasonCode` are deliberately NOT cleared here, and the
// design note that listed them alongside the negotiated limits is superseded on that
// one point. This function runs on disconnect() and on every protocolFailure() as well
// as on teardown, whereas Requirement 1.6 keeps the CONNACK outcome readable "until the
// next connection attempt begins". They are cleared by resetConnackState(), called at
// the top of connect(), which is exactly that boundary. The negotiated limits differ
// because they govern transmission on the connection being torn down and must not leak
// into the next one, so clearing them early is safe; the CONNACK outcome is a
// diagnostic, and clearing it early loses information.
}

uint8_t PubSubClient::defaultReasonCode(MqttError error) {
// The reason-code selection table of the design. `ProtocolError` maps to 0x82 here
// because that is the common case (illegal property, packet type a client may not
// receive); the AUTH trigger passes 0x8C explicitly.
    switch (error) {
        case MqttError::MalformedPacket:       return MQTT_RC_MALFORMED_PACKET;       // 0x81
        case MqttError::ProtocolError:         return MQTT_RC_PROTOCOL_ERROR;         // 0x82
        case MqttError::ReceiveMaximumReached: return MQTT_RC_RECEIVE_MAX_EXCEEDED;   // 0x93
        case MqttError::PacketTooLarge:        return MQTT_RC_PACKET_TOO_LARGE;       // 0x95
        default:                               return 0;
    }
}

void PubSubClient::protocolFailure(MqttError error, uint8_t reasonCode) {
// Record first: the diagnostics must survive the close, and they are what the driver
// reads after loop() reports the failure (Requirement 14.2).
    this->errorState = error;
    this->errorReasonCode = reasonCode;

    if (MQTT_RUNTIME_IS_5()) {
// Requirement 10.5: exactly one DISCONNECT carrying the error Reason Code while the
// connection is writable. Requirement 10.6: not a single byte when it is not.
        if (socketWritable()) {
            sendDisconnect(reasonCode, false);
        }
    }

    if (_client != nullptr) {
        _client->flush();
        _client->stop();
    }

    resetConnectionState();
}

void PubSubClient::disconnect(bool disconnect_package) {
    if (_client != nullptr) {
      if (disconnect_package) {
        sendDisconnect(MQTT_RC_NORMAL_DISCONNECTION, true);
      }
      _client->flush();
      _client->stop();
    }
    resetConnectionState();
}

uint16_t PubSubClient::writeString(const char* string, uint8_t* buf, uint16_t pos) {
// Defense in depth: every caller already validates that the string fits, but this
// function used to walk the string to its NUL with no bound at all. Verify the 2-byte
// length prefix plus the payload fit the buffer and leave `pos` untouched otherwise,
// so a mistake in a caller can no longer corrupt the heap.
    if (string == nullptr) {
        return pos;
    }
    size_t slen = strnlen(string, this->bufferSize);
    if ((slen > 0xFFFF) || ((size_t)pos + 2 + slen > (size_t)this->bufferSize)) {
        return pos;
    }
    buf[pos++] = (uint8_t)(slen >> 8);
    buf[pos++] = (uint8_t)(slen & 0xFF);
    memcpy(buf + pos, string, slen);
    return pos + slen;
}

boolean PubSubClient::connected() {
    boolean rc;

// A client without a working buffer cannot be considered connected: every packet
// exchange goes through `buffer`, so treat a failed allocation as disconnected.
    if ((_client == NULL) || (this->buffer == NULL)) {
        this->_state = MQTT_DISCONNECTED;

        rc = false;
    } else {
        rc = (int)_client->connected();
        if (!rc) {
            if (this->_state == MQTT_CONNECTED) {
                this->_state = MQTT_CONNECTION_LOST;
                _client->flush();
                _client->stop();

                pingOutstanding = false;
                if (MQTT_RUNTIME_IS_5() && (!this->sessionIdentity.valid ||
                    (this->sessionIdentity.sessionExpiryInterval == 0))) {
// Clean Start, explicit zero expiry, or a failed identity capture leaves no session that can
// be resumed safely after transport loss. Release local state immediately rather than
// allowing the next CONNECT to mistake it for a compatible identity.
                    for (uint8_t i = 0; i < MQTT_MAX_OUTSTANDING; ++i) {
                        if (this->outstanding[i].state != MqttOutState::Free) {
                            reportAbandoned(this->outstanding[i]);
                        }
                    }
                    clearOutstanding(false);
                    clearInboundQos2();
                    clearSessionIdentity();
                }
            }
        } else {
            return this->_state == MQTT_CONNECTED;
        }
    }
    return rc;
}

PubSubClient& PubSubClient::setServer(uint8_t * ip, uint16_t port) {
    IPAddress addr(ip[0],ip[1],ip[2],ip[3]);
    return setServer(addr,port);
}

PubSubClient& PubSubClient::setServer(IPAddress ip, uint16_t port) {
    this->ip = ip;
    this->port = port;

    this->domain = "";

    return *this;
}

PubSubClient& PubSubClient::setServer(const char * domain, uint16_t port) {
// A null host, or a String assignment that ran out of memory, used to leave an empty
// domain behind - connect() would then silently fall back to the (unrelated) IP
// address. Clear the port instead so connect() fails cleanly.
    this->domain = "";
    if (domain != nullptr) {
        this->domain = domain;
        if (this->domain.length() != strlen(domain)) {   // out of memory
            this->domain = "";
            this->port = 0;
            return *this;
        }
    }
    this->port = port;

    return *this;
}

PubSubClient& PubSubClient::setCallback(MQTT_CALLBACK_SIGNATURE) {
    this->callback = callback;
    return *this;
}

PubSubClient& PubSubClient::setClient(Client& client) {
    this->_client = &client;
    return *this;
}

PubSubClient& PubSubClient::setStream(Stream& stream) {
    this->stream = &stream;
    return *this;
}

int PubSubClient::state() const {
    return this->_state;
}

boolean PubSubClient::setBufferSize(uint16_t size) {
    if (size == 0) {
        // Cannot set it back to 0
        return false;
    }
// Commit bufferSize only after a successful (re)allocation. Previously bufferSize
// was set even when malloc() failed, leaving the object reporting a non-zero
// capacity while buffer == nullptr, which later operations would dereference.
    if (this->bufferSize == 0) {
        uint8_t* newBuffer = (uint8_t*)malloc(size);
        if (newBuffer == NULL) {
            return false;
        }
        this->buffer = newBuffer;
    } else {
        uint8_t* newBuffer = (uint8_t*)realloc(this->buffer, size);
        if (newBuffer == NULL) {
            return false;
        }
        this->buffer = newBuffer;
    }
    this->bufferSize = size;
    return true;
}

uint16_t PubSubClient::getBufferSize() const {
    return this->bufferSize;
}

PubSubClient& PubSubClient::setKeepAlive(uint16_t keepAlive) {
    this->keepAlive = keepAlive;
    if (MQTT_RUNTIME_IS_5()) {
// Requirement 5.7: remember what was requested, so the default for an absent Server
// Keep Alive - "the requested keepalive kept" - is still restorable after a broker has
// overwritten `keepAlive` with its own value. Every constructor routes through here, so
// this is also where the initial value comes from.
        this->keepAliveRequested = keepAlive;
    }
    return *this;
}
PubSubClient& PubSubClient::setSocketTimeout(uint16_t timeout) {
    this->socketTimeout = timeout;
    // Keep the default packet deadline aligned with the historical socket
    // timeout until callers opt into a distinct packet budget.
    if (!this->packetTimeoutConfigured) {
        this->packetTimeout = timeout;
    }
    return *this;
}

PubSubClient& PubSubClient::setPacketTimeout(uint16_t seconds) {
    this->packetTimeout = seconds;
    this->packetTimeoutConfigured = true;
    return *this;
}
#endif  // MQTT_VERSION == MQTT_VERSION_5_0