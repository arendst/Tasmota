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

// A single, immutable MQTT control packet represented as its exact wire bytes.
//
// Instances are produced only through the static builder functions below; the
// value can then be handed to MockClient::pushPacket() (inbound scripting) or
// its bytes() compared against decoded outbound bytes.
class MqttPacket {
public:
    // --- Builders ----------------------------------------------------------

    // CONNACK: 0x20 0x02 <ackFlags> <returnCode>.
    // ackFlags bit0 is the Session Present flag; all other bits are zero.
    static MqttPacket connack(uint8_t returnCode, bool sessionPresent = false);

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

    // Structural validator for PUBLISH: verifies the fixed-header high nibble is
    // MQTTPUBLISH, that the decoded Remaining Length equals the actual trailing
    // byte count, and that the 2-byte topic length fits within the body (leaving
    // room for the packet identifier when QoS > 0). Rejects truncated packets.
    static bool isStructurallyValidPublish(const std::vector<uint8_t>& bytes);

    // Decode a Remaining Length starting at `bytes[offset]`. Returns true on
    // success and sets `value` (decoded length) and `bytesConsumed` (1..4). A
    // value needing more than four bytes, or a truncated field, returns false.
    static bool decodeRemainingLength(const std::vector<uint8_t>& bytes,
                                      size_t offset,
                                      uint32_t& value,
                                      size_t& bytesConsumed);
};

#endif  // TASMOTA_PUBSUB_TEST_MQTT_PACKET_H
