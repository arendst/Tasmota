/*
  PubSubClient.h - A simple client for MQTT.

  TasmotaPubSub - Tasmota fork of the PubSubClient MQTT library.

  Original author:
    Nick O'Leary - http://knolleary.net
  Tasmota fork maintained by Theo Arends and the Tasmota contributors.

  SPDX-FileCopyrightText: 2008-2020 Nicholas O'Leary
  SPDX-FileCopyrightText: 2020-2025 Theo Arends and Tasmota contributors

  SPDX-License-Identifier: MIT
*/

#ifndef TASMOTA_PUBSUBCLIENT_5_H
#define TASMOTA_PUBSUBCLIENT_5_H

#include <Arduino.h>
#include "IPAddress.h"
#include "Client.h"
#include "Stream.h"
#include "LList.h"

// MQTT_MAX_PACKET_SIZE : Maximum packet size. Override with setBufferSize().
#ifndef MQTT_MAX_PACKET_SIZE
//#define MQTT_MAX_PACKET_SIZE 256
#define MQTT_MAX_PACKET_SIZE 1200     // Tasmota v8.1.0.8
#endif

// MQTT_KEEPALIVE : keepAlive interval in Seconds. Override with setKeepAlive()
#ifndef MQTT_KEEPALIVE
#define MQTT_KEEPALIVE 15
#endif

// MQTT_SOCKET_TIMEOUT: socket timeout interval in Seconds. Override with setSocketTimeout()
#ifndef MQTT_SOCKET_TIMEOUT
#define MQTT_SOCKET_TIMEOUT 15
#endif

// USE_MQTT_QOS : opt-in to OUTBOUND QoS 1/2 acknowledged delivery.
//
// Tasmota publishes at QoS 0 in almost every real deployment, so this MQTT5-capable client
// is QoS-0-only for outbound PUBLISH by default. Defining USE_MQTT_QOS compiles in the full
// outbound QoS 1/2 machinery: the Packet Identifier lifecycle for publishes, the
// Retained_Packet_Body replay buffers and MQTT_INFLIGHT_COPY_BUDGET accounting, PUBLISH
// retransmission on a resumed session, the PUBACK/PUBREC/PUBCOMP handshakes, and the
// Receive Maximum back-pressure gate. Leaving it undefined drops all of that code and the
// per-instance replay storage, which is a material flash and RAM saving.
//
// What is NOT affected by this switch, and is always compiled:
//   - QoS 0 publishing (the four classic overloads and the MqttPublishRequest path),
//   - the MQTT 5.0 message property set (Response Topic, Correlation Data, User Properties),
//   - SUBSCRIBE / UNSUBSCRIBE and their SUBACK/UNSUBACK correlation,
//   - inbound PUBLISH delivery at QoS 0, 1 and 2 (broker -> client), which is the receive
//     side and independent of whether this client itself publishes at QoS > 0.
//
// With USE_MQTT_QOS undefined, a publish requested at QoS 1 or 2 is refused before a byte is
// framed (publish() returns false), exactly as runtime level-4 fallback already refuses it.

// MQTT_MAX_INFLIGHT : maximum outbound QoS 1/2 PUBLISHes awaiting acknowledgement.
// It is deliberately independent of the tracked subscription capacity below: the broker's
// Receive Maximum applies to publishes, not SUBSCRIBE/UNSUBSCRIBE operations. Only consulted
// when USE_MQTT_QOS is defined.
#ifndef MQTT_MAX_INFLIGHT
#define MQTT_MAX_INFLIGHT 4
#endif

// MQTT_MAX_PENDING_SUBSCRIPTIONS : tracked SUBSCRIBE/UNSUBSCRIBE exchanges held in addition
// to MQTT_MAX_INFLIGHT publishes. Tasmota's fixed non-Mesh reconnect burst is nine operations
// (six core filters plus up to three extension operations); the default twelve leaves a
// three-operation margin. Tasmesh peers are runtime-sized, so attempts beyond this fixed
// envelope are atomically refused instead of sent without an identifier record. Each occupied
// entry has fixed metadata plus one Arduino String holding its full filter/topic.
#ifndef MQTT_MAX_PENDING_SUBSCRIPTIONS
#define MQTT_MAX_PENDING_SUBSCRIPTIONS 12
#endif

#ifdef USE_MQTT_QOS
#define MQTT_MAX_OUTSTANDING (MQTT_MAX_INFLIGHT + MQTT_MAX_PENDING_SUBSCRIPTIONS)
#else
// Without outbound QoS 1/2 the outstanding table tracks only SUBSCRIBE/UNSUBSCRIBE
// exchanges: no publish ever holds a slot, so the MQTT_MAX_INFLIGHT publish reservation is
// dropped from the total and the per-slot record loses its replay fields (see
// MqttOutstandingMessage).
#define MQTT_MAX_OUTSTANDING (MQTT_MAX_PENDING_SUBSCRIPTIONS)
#endif

// MQTT_MAX_INBOUND_INFLIGHT : size of the inbound QoS 2 packet identifier registry.
// Also the value declared to the broker as Receive Maximum in CONNECT.
#ifndef MQTT_MAX_INBOUND_INFLIGHT
#define MQTT_MAX_INBOUND_INFLIGHT 8
#endif

// MQTT_SUBACK_TOPIC_LEN has been REMOVED. It sized a fixed char[] holding a prefix of
// the topic filter retained per outstanding entry, i.e. it was a truncation limit rather
// than a memory bound. The outstanding entry now retains the FULL filter in an Arduino
// String (Requirement 13.10), so a failing SUBACK/UNSUBACK always names the complete
// filter and there is no length to configure.

// MQTT_MAX_TOPIC_ALIASES : maximum node count of the outbound topic alias table.
// 0 disables aliasing. The node count, not a topic-byte limit, bounds memory: every
// entry owns its complete topic in an Arduino String.
#ifndef MQTT_MAX_TOPIC_ALIASES
#define MQTT_MAX_TOPIC_ALIASES 0
#endif

// MQTT_INFLIGHT_COPY_BUDGET : total bytes across every Retained_Packet_Body held at one
// time (Requirement 8.20). Only meaningful when USE_MQTT_QOS is defined; without it there is
// no outbound QoS 1/2 path, nothing is ever retained, and this macro is not referenced.
//
// WITHIN the USE_MQTT_QOS feature this is a TRANSMISSION-ADMISSION CEILING, not a second
// on/off selector: once outbound QoS 1/2 is compiled in, retention of the assembled body of
// a QoS 1/2 PUBLISH is unconditional (Requirement 8.19). Overriding this to 0 does not
// disable retention selectively - it refuses every QoS 1/2 buffered publish before its first
// write with MqttError::RetentionUnavailable. QoS 0 is unaffected. To drop the whole feature
// and its per-instance replay storage, leave USE_MQTT_QOS undefined instead.
//
// WHY RETENTION IS NOT OPTIONAL ONCE QoS IS ENABLED. Retransmitting an unacknowledged QoS 1/2
// PUBLISH requires the packet's bytes to still exist, and [MQTT-4.4.0-1] permits exactly one
// moment to resend them: reconnecting with Clean Start 0 to a session the broker still holds.
// A caller cannot do it instead - the public API exposes no re-transmit entry point, the
// Packet Identifier was allocated internally and never handed out, and the DUP-flagged replay
// must otherwise be byte-identical to a packet the caller never saw assembled. So while QoS
// 1/2 is offered at all, retransmission is this library's responsibility or it does not exist;
// that is why retention cannot be a separate toggle underneath the feature.
//
// WHAT IS RETAINED: the ASSEMBLED PACKET BODY - Topic Name, Packet Identifier, Properties
// field and payload, i.e. everything after the Remaining Length - together with the
// fixed-header byte, rather than the payload alone. Requirement 8.7 asks for a
// retransmission that differs from the original in the DUP flag and nothing else, and the
// property set is BORROWED for the duration of assembly (see MqttMessageProperties), so it
// is gone by the time a retransmission could re-frame from it. Retaining the assembled body
// makes the resend byte-identical by construction: flip bit 3 of the fixed header and write
// the bytes back out. It also means the original Packet Identifier is reused with no work at
// all, because it is part of those bytes.
//
// BUFFERED QoS 1/2 PUBLISHES ARE ATOMIC. Reserve the budget and allocate/copy the complete
// body before the first transport write. If either step fails, the tentative table slot and
// Packet Identifier are released, budget accounting is unchanged, the connection remains
// usable, and no byte reaches the transport. A PUBREL resend for an entry awaiting PUBCOMP
// costs no budget because it carries only its Packet Identifier (Requirement 8.23).
//
// ONLY the buffered publish path is QoS-capable. `beginPublish()` and `publish_P()` are QoS 0
// only; a future QoS 1/2 streaming entry point is prohibited unless it first provides
// persistent replay storage equivalent to this retained body.
//
// WHY 2048. With no ceiling the worst case is MQTT_MAX_INFLIGHT (4) times the largest
// assembled body the working buffer admits (bufferSize less MQTT_MAX_HEADER_SIZE, ~1195
// bytes at the 1200-byte default), i.e. ~4.8 kB - a quarter of the 15-25 kB of fragmented
// free heap an ESP8266 typically has. 2048 holds that worst case near a TENTH of the same
// heap, sits ABOVE the largest single assembled body so a lone large publish is never denied
// retention by an exhausted budget, and still admits all four in-flight slots at once for the
// sub-512-byte telemetry and state payloads the driver actually sends.
#ifdef USE_MQTT_QOS
#ifndef MQTT_INFLIGHT_COPY_BUDGET
#define MQTT_INFLIGHT_COPY_BUDGET 2048
#endif
#endif  // USE_MQTT_QOS

// MQTT_ACK_TIMEOUT : seconds before a still-unacknowledged entry reports one local timeout
// diagnostic. The entry and Packet Identifier remain held until terminal acknowledgement or
// definitive session termination; this value never authorizes identifier reuse.
#ifndef MQTT_ACK_TIMEOUT
#define MQTT_ACK_TIMEOUT MQTT_SOCKET_TIMEOUT
#endif

// MQTT_PACKET_TIMEOUT : packet-wide receive deadline in seconds.
#ifndef MQTT_PACKET_TIMEOUT
#define MQTT_PACKET_TIMEOUT MQTT_SOCKET_TIMEOUT
#endif

// MQTT_PERSISTENT_SESSION_EXPIRY_INTERVAL : the MQTT 5 Session Expiry Interval
// emitted for the legacy `cleanSession == false` CONNECT mode. 0xFFFFFFFF preserves
// the indefinite, MQTT 3.1.1-style persistence Tasmota expects from mqtt_persistent.
// A runtime value of zero is also legal: it deliberately requests Clean Start 0 while
// expiring the Session when the network connection closes.
#ifndef MQTT_PERSISTENT_SESSION_EXPIRY_INTERVAL
#define MQTT_PERSISTENT_SESSION_EXPIRY_INTERVAL 0xFFFFFFFFUL
#endif

// MQTT_MAX_TRANSFER_SIZE : limit how much data is passed to the network client
//  in each write call. Needed for the Arduino Wifi Shield. Leave undefined to
//  pass the entire MQTT packet in each write call.
//#define MQTT_MAX_TRANSFER_SIZE 80

// Possible values for client.state()
#define MQTT_CONNECTION_TIMEOUT     -4
#define MQTT_CONNECTION_LOST        -3
#define MQTT_CONNECT_FAILED         -2
#define MQTT_DISCONNECTED           -1
#define MQTT_CONNECTED               0
#define MQTT_CONNECT_BAD_PROTOCOL    1
#define MQTT_CONNECT_BAD_CLIENT_ID   2
#define MQTT_CONNECT_UNAVAILABLE     3
#define MQTT_CONNECT_BAD_CREDENTIALS 4
#define MQTT_CONNECT_UNAUTHORIZED    5

#define MQTTCONNECT     1 << 4  // Client request to connect to Server
#define MQTTCONNACK     2 << 4  // Connect Acknowledgment
#define MQTTPUBLISH     3 << 4  // Publish message
#define MQTTPUBACK      4 << 4  // Publish Acknowledgment
#define MQTTPUBREC      5 << 4  // Publish Received (assured delivery part 1)
#define MQTTPUBREL      6 << 4  // Publish Release (assured delivery part 2)
#define MQTTPUBCOMP     7 << 4  // Publish Complete (assured delivery part 3)
#define MQTTSUBSCRIBE   8 << 4  // Client Subscribe request
#define MQTTSUBACK      9 << 4  // Subscribe Acknowledgment
#define MQTTUNSUBSCRIBE 10 << 4 // Client Unsubscribe request
#define MQTTUNSUBACK    11 << 4 // Unsubscribe Acknowledgment
#define MQTTPINGREQ     12 << 4 // PING Request
#define MQTTPINGRESP    13 << 4 // PING Response
#define MQTTDISCONNECT  14 << 4 // Client is Disconnecting
#define MQTTAUTH        15 << 4 // Authentication exchange (MQTT 5.0)

#define MQTTQOS0        (0 << 1)
#define MQTTQOS1        (1 << 1)
#define MQTTQOS2        (2 << 1)

// Maximum size of fixed header and variable length size header
#define MQTT_MAX_HEADER_SIZE 5

// Minimum working-buffer capacities for an empty CONNECT. These include the five
// reserved header bytes used during assembly, the protocol name/level, flags,
// keepalive, and the empty Client Identifier. MQTT 5.0 additionally includes its
// mandatory nine-byte CONNECT Properties field (one-byte length plus Receive Maximum
// and Maximum Packet Size). setBufferSize() may accept smaller allocations, but
// connect() refuses them atomically before transmitting a byte.
#define MQTT_CONNECT_MINIMUM_BUFFER_SIZE_3_1_1 17
#define MQTT_CONNECT_MINIMUM_BUFFER_SIZE_5_0   26
#define MQTT_CONNECT_MINIMUM_BUFFER_SIZE MQTT_CONNECT_MINIMUM_BUFFER_SIZE_5_0

// The inbound message callback. Unconditionally a std::function: Tasmota only ever
// builds this library for ESP8266 / ESP32, so the plain-function-pointer branch this
// used to carry behind `#if defined(ESP8266) || defined(ESP32)` was dead code in every
// real build, and the host test harness compiles with -DESP32 for exactly that reason.
// Keeping one form removes a configuration that was never exercised.
//
// This is the ONE std::function in the library, and it is deliberate: consumers bind
// capturing lambdas to it (see the Berry MQTT client in xdrv_52_3_berry_mqttclient.ino),
// Requirement 12.1 preserves this signature exactly, and it is assigned once at init
// rather than on the packet path. Every callback and visitor ADDED by the MQTT 5.0 work
// is a plain function pointer with an opaque void* ctx instead - see MqttPropertyVisitor,
// MqttUserPropertyVisitor and MqttSubscribeCallback.
#include <functional>
#define MQTT_CALLBACK_SIGNATURE std::function<void(char*, uint8_t*, unsigned int)> callback

#define CHECK_STRING_LENGTH(l,s) if (l+2+strnlen(s, this->bufferSize) > this->bufferSize) {_client->stop();return false;}

// Cause of the most recent failure, readable through PubSubClient::lastError().
// Version-neutral: the codecs and bounds checks report these at either runtime
// protocol level. Reset to `None` at the start of every connection attempt.
enum class MqttError : uint8_t {
   None = 0,
   NotConnected,            // no usable connection for the requested operation
   BufferTooSmall,          // assembled packet would not fit the working buffer
   PacketTooLarge,          // Remaining Length > MQTT_VBI_MAX, or over a negotiated cap
   MalformedPacket,         // unparseable wire structure
   ProtocolError,           // parseable but semantically illegal
   PacketIdUnavailable,     // no free packet identifier
   ReceiveMaximumReached,   // broker's Receive Maximum reached
   RetentionUnavailable,    // QoS 1/2 replay body cannot be retained; retry later
   WriteFailed,             // socket accepted fewer bytes than declared
   ReceiveTimeout,          // packet-wide receive deadline expired
   ServerDisconnect         // broker sent DISCONNECT
};

// --- Borrowed views into the working buffer (Requirements 11.4, 11.7, 12.4) -
//
// A value read out of an inbound packet, as a pointer and a length INTO THE CLIENT'S
// SINGLE WORKING BUFFER. Nothing is copied and nothing is allocated (Requirement 13.1),
// which is what keeps inbound property access off the allocator on the packet path.
//
// LIFETIME: a view is valid from the moment the message callback is invoked until the
// NEXT INBOUND PACKET IS PARSED - the next readPacket() overwrites the buffer these
// pointers address. The natural usage is therefore to read the values INSIDE the
// callback; between callbacks they stay readable until the next loop() consumes a
// packet. A consumer that needs a value for longer must copy it out itself.
//
// A zero-length value is still PRESENT: `data` points one past the value, which is
// well defined and never dereferenced, so `present()` distinguishes "the property was
// received and its value is empty" from "the property was absent".
struct MqttStringView {
   const char* data = nullptr;
   uint16_t    len  = 0;
   bool present() const { return this->data != nullptr; }
};

// There is no MqttBinaryView: Correlation Data is the only Binary Data value this
// client handles, and it is served from the fixed Correlation_Data_Storage below
// rather than borrowed from the working buffer (Requirements 3.9, 11.11, 13.7, 13.8).

// --- Inbound message property set (Requirements 11.7, 12.4, 12.5, 12.6) ----
//
// The MQTT 5.0 properties of the MOST RECENTLY parsed inbound PUBLISH. The string
// members are MqttStringView, so the lifetime note above applies to every one of them.
// Correlation Data is the one exception: it is copied into fixed client-owned storage
// rather than borrowed, so its lifetime is not tied to the working buffer at all (see
// `correlationDataStorage` below).
//
// `valid` is the no-data indication of Requirement 12.5: it is cleared at the start of
// every inbound PUBLISH parse and set only after the packet's property region has
// parsed cleanly. The PUBLISH envelope (`qos`, `retained`, `duplicate`, `packetId`) is
// populated alongside the properties before the existing message callback runs.
// So the accessor reads `valid == false` with an empty set before any PUBLISH has been
// delivered, and `valid == true` with every property field absent after a PUBLISH that
// carried no properties (Requirement 12.6).
//
// `userPropertyCount` is how many User Property entries the packet carried; the entries
// themselves are NOT stored here - there is no fixed-size cache to truncate them.
// forEachInboundUserProperty() re-walks the buffer to enumerate them (Requirement 11.4).
//
// Always declared so the MQTT5-capable API remains stable. Runtime level-4 fallback
// has no Properties field on the wire and never populates this, so it reads as an empty
// set with `valid == false` there.
struct MqttInboundProperties {
   bool           valid            = false;   // false until a PUBLISH has been parsed
   uint8_t        qos              = 0;       // fixed-header QoS, 0..2
   bool           retained         = false;   // fixed-header RETAIN flag
   bool           duplicate        = false;   // fixed-header DUP flag
   uint16_t       packetId         = 0;       // 0 for QoS 0
   bool           hasPayloadFormat = false;
   uint8_t        payloadFormat    = 0;       // 0 = unspecified bytes, 1 = UTF-8
   MqttStringView contentType;
   MqttStringView responseTopic;
   // Correlation Data, served from Correlation_Data_Storage (Requirement 11.11).
   // `correlationData` points at the client's fixed 16-byte member below - NOT into
   // the working buffer - and is null when the property was absent. The length is
   // always 0..16; a declared inbound length above 16 never reaches here, because it
   // closed the connection as a protocol error (Requirements 3.10, 11.12).
   bool           hasCorrelationData = false;
   const uint8_t* correlationData    = nullptr;
   uint8_t        correlationDataLen = 0;
   uint16_t       topicAlias        = 0;      // 0 = no Topic Alias received
   uint8_t        userPropertyCount = 0;
};

// Called once per inbound User Property, in RECEIVED ORDER. Returning false aborts the
// walk, and forEachInboundUserProperty() then reports false to its own caller.
//
// A plain function pointer with an opaque context, mirroring MqttPropertyVisitor: the
// walk sits on the packet path and must not allocate, so this is deliberately not a
// std::function. `key` and `value` are borrowed views - see the lifetime note above.
typedef bool (*MqttUserPropertyVisitor)(MqttStringView key, MqttStringView value, void* ctx);

// --- Subscription options (Requirements 9.2, 9.8, 9.9) ---------------------
//
// The MQTT 5.0 Subscription Options byte, one per Topic Filter, written immediately
// after the filter it applies to:
//
//   bits 0-1  Maximum QoS          0..2
//   bit  2    No Local
//   bit  3    Retain As Published
//   bits 4-5  Retain Handling      0..2
//   bits 6-7  Reserved             always 0
//
// A DEFAULT-CONSTRUCTED set encodes to the single byte 0x00, which is exactly the
// trailing requested-QoS byte runtime level-4 fallback writes for `subscribe(topic)`
// - and, for `subscribe(topic, qos)`, to the byte `qos`. So routing the existing
// overloads through this struct changes no emitted byte at either runtime level.
//
// Always declared for the MQTT5-capable API, but runtime level 4 has no Subscription
// Options byte: bits 2-7 of its requested-QoS byte are reserved and must be zero. Runtime
// level-4 fallback therefore ACCEPTS only a set whose flags are all at their defaults and
// whose Maximum QoS is 0 or 1, and rejects anything else rather than emitting a byte the
// level-4 wire format does not define.
struct MqttSubscribeOptions {
   uint8_t maxQos            = 0;       // 0..2 (0..1 in runtime level-4 fallback)
   bool    noLocal           = false;   // runtime protocol level 5 only
   bool    retainAsPublished = false;   // runtime protocol level 5 only
   uint8_t retainHandling    = 0;       // 0..2, runtime protocol level 5 only
};

// --- Subscription outcome (Requirements 9.4, 9.5, 9.6) ---------------------
//
// One per-topic Reason Code from a SUBACK or an UNSUBACK. `subscribe()` and
// `unsubscribe()` return as soon as the packet is written, long before the broker
// answers, so the outcome is surfaced here instead of through their return value.
//
// `granted` is the per-packet-type success test, NOT simply `reasonCode < 0x80`:
//   SUBACK   - success ONLY for the granted-QoS codes 0x00, 0x01 and 0x02
//              (Requirement 9.4: "only when the Reason Code indicates a granted QoS")
//   UNSUBACK - 0x00 (Success) and 0x11 (No subscription existed) are both non-failures
//
// LIFETIME of `topicFilter`: it points at the buffer of the Arduino String in which the
// outstanding entry retains the FULL, untruncated filter this client sent for
// `packetId`. It is valid for the duration of the callback and until that outstanding
// entry is released or reused - the same borrowed-view rule the inbound property
// accessors already document. A caller that needs the filter for longer must copy it.
// It is null when the acknowledgement's Packet Identifier matched no outstanding entry,
// in which case the Reason Code is still reported but names no filter.
struct MqttSubscriptionResult {
   uint16_t    packetId    = 0;
   uint8_t     reasonCode  = 0;         // granted QoS 0x00/0x01/0x02, or >= 0x80 failure
   const char* topicFilter = nullptr;   // borrowed; see the lifetime note above
   bool        granted     = false;
   bool        unsubscribe = false;     // true for an UNSUBACK result
};

// Called once per per-topic Reason Code in a received SUBACK or UNSUBACK, in wire order.
// The `ctx` handed to setSubscribeCallback() is passed back UNCHANGED, which is how a
// caller reaches its instance state without a capturing lambda - a plain function pointer
// with an opaque context, exactly like MqttPropertyVisitor and MqttUserPropertyVisitor.
typedef void (*MqttSubscribeCallback)(const MqttSubscriptionResult& result, void* ctx);

// --- Outstanding (unacknowledged) outbound operations ----------------------
//
// One entry per outbound operation that holds an allocated Packet Identifier and is
// waiting for its acknowledgement. The table exists so an acknowledgement can be
// CORRELATED back to what this client sent - which for SUBACK/UNSUBACK is the topic
// filter a failing Reason Code refers to (Requirement 9.5).
//
// SCOPE NOTE: the full machinery - the rotoring packet-identifier allocator, the
// acknowledgement deadline, in-flight counting and the outbound QoS 1/2 publish states -
// belongs to the acknowledged-delivery work. What is implemented here is the subset
// SUBACK/UNSUBACK correlation strictly requires: the entry shape, the fixed slot table,
// and claim / find / release. The PUBLISH-side states are already named so that layer
// adds behavior rather than reshaping this.
enum class MqttOutState : uint8_t {
   Free = 0,            // slot unused
   AwaitingPuback,      // outbound QoS 1 PUBLISH sent
   AwaitingPubrec,      // outbound QoS 2 PUBLISH sent
   AwaitingPubcomp,     // PUBREL sent
   AwaitingSuback,      // SUBSCRIBE sent
   AwaitingUnsuback     // UNSUBSCRIBE sent
};

// The SLOT COUNT is fixed at MQTT_MAX_OUTSTANDING: MQTT_MAX_INFLIGHT slots cover
// QoS 1/2 PUBLISH exchanges and MQTT_MAX_PENDING_SUBSCRIPTIONS additional slots keep every
// sent SUBSCRIBE/UNSUBSCRIBE live through its terminal acknowledgement. What is NOT fixed is
// `topic`: it holds the FULL filter or topic in an Arduino String rather than a truncated prefix
// in a char[] (Requirement 13.10), assigned once when the packet is sent and cleared when the
// table is cleared. That is one short heap buffer per OCCUPIED slot, taken once per subscribe /
// publish and never per packet, so Requirement 13.1's no-per-message-allocation rule is
// untouched.
struct MqttOutstandingMessage {
   uint16_t      packetId   = 0;
   MqttOutState  state      = MqttOutState::Free;
   uint8_t       qos        = 0;
   uint8_t       reasonCode = 0;
   bool          timedOutReported = false;  // diagnostic sent; the live identifier remains held
   unsigned long sentAt     = 0;
   String        topic;                 // the FULL filter/topic, for failure reporting
// --- Retained_Packet_Body (Requirements 8.19, 8.7, 13.11) ------------------
// Present only when USE_MQTT_QOS is defined: these fields are the outbound QoS 1/2 replay
// buffer, so a QoS-0-only build omits them and the per-slot record is that much smaller.
// When compiled in they are used for acknowledged publishes at runtime protocol level 5
// only; level-4 fallback accepts only QoS 0. MQTT_INFLIGHT_COPY_BUDGET is a byte ceiling on
// the total held, not a selector within the feature - see its comment.
//
// `packetBody` owns a heap copy of the assembled PUBLISH body - Topic Name, Packet
// Identifier, Properties field, payload - and `headerByte` is that packet's fixed-header
// byte. Buffered QoS 1/2 admission creates an entry only after this copy is complete, so a
// null body is valid only for QoS 0, subscriptions, or an AwaitingPubcomp entry whose PUBLISH
// has already been accepted by the broker.
//
// A null body does NOT prevent an entry awaiting PUBCOMP from resending its PUBREL: that
// packet is assembled from its Packet Identifier alone, at no budget cost.
//
// Freed by releaseOutstanding() when the acknowledgement sequence ends or the entry is
// abandoned, when the entry advances to AwaitingPubcomp and the PUBLISH is therefore done,
// by clearOutstanding() for every entry it clears, and by the client's destructor.
#ifdef USE_MQTT_QOS
   uint8_t*      packetBody    = nullptr;
   uint16_t      packetBodyLen = 0;
   uint8_t       headerByte    = 0;
#endif  // USE_MQTT_QOS
};

struct MqttTopicAliasEntry {
   uint16_t alias = 0;
   String   topic;  // complete topic: aliasing must never retain a truncated prefix
};

// --- Outbound message property set (Requirements 11.3, 11.5, 11.6, 11.8, 11.9) ---
//
// What a caller supplies for an outbound message. Everything is a borrowed pointer:
// the set is consumed synchronously during packet assembly, so nothing is copied and
// nothing is owned (Requirement 13.1). `userPropertyKeys`/`userPropertyValues` are
// parallel arrays of `userPropertyCount` entries, emitted in the order supplied.
//
// Response Topic and Correlation Data are independent MQTT 5 properties. A request may
// provide a Response Topic without Correlation Data, while a response publishes TO that
// topic and echoes Correlation Data without carrying another Response Topic. Each non-null
// pointer therefore emits its own property independently.
//
// Defined unconditionally because MqttPublishRequest holds one BY VALUE. The definition
// declares storage but emits no wire byte by itself; runtime level-4 fallback has no
// Properties field and therefore ignores this property set during packet assembly.
struct MqttMessageProperties {
   bool           hasPayloadFormat    = false;   // emit Payload Format Indicator at all?
   uint8_t        payloadFormat       = 0;       // 0 = unspecified bytes, 1 = UTF-8
   const char*    contentType         = nullptr;
   const char*    responseTopic       = nullptr;
   // Correlation Data, borrowed for the duration of assembly. There is NO outbound
   // size limit: any length that fits the packet - i.e. that passes the buffer gate
   // and the Remaining Length gate - is emitted as-is, on a best-effort basis
   // (Requirement 11.10). MQTT_CORRELATION_DATA_MAX (16) is an INBOUND-only bound,
   // the fixed extent of Correlation_Data_Storage that a received value is copied
   // into (Requirements 11.11, 11.12, 13.7); it has no bearing on what is sent, which
   // is why this length is a uint16_t rather than a uint8_t.
   const uint8_t* correlationData     = nullptr;
   uint16_t       correlationDataLen  = 0;
   const char* const* userPropertyKeys   = nullptr;
   const char* const* userPropertyValues = nullptr;
   uint8_t        userPropertyCount   = 0;
};

// --- Full-form publish request (Requirements 8.2, 8.3, 2.6, 11.1) ----------
//
// The one publish entry point that reaches everything an outbound PUBLISH can carry:
// the requested QoS, the MQTT 5.0 message property set, and the topic-alias opt-in.
// The two shorter QoS-capable overloads are conveniences that fill one of these in
// and hand it to `publish(const MqttPublishRequest&)`.
//
// Every member is a borrowed pointer or a scalar and the whole request is consumed
// synchronously, so constructing one costs a few bytes of the caller's stack frame and
// no allocation (Requirement 13.1). Default-constructed, it is a QoS 0 publish of an
// empty payload with no properties - i.e. exactly what the four preserved `publish`
// overloads have always sent.
//
// `qos` is 0, 1 or 2 at runtime protocol level 5. Runtime level-4 fallback accepts only
// 0 (Requirement 8.18): outbound acknowledged delivery is an MQTT 5 capability, and the
// acknowledgement dispatch and abandonment sweep that carry QoS 1/2 to completion run
// only at level 5. A QoS 1/2 publish at level 4 would hold a table slot that nothing could
// ever free, so it is refused before a byte is assembled (see publish() for the argument
// in full).
//
// `useTopicAlias` is the caller's opt-in to Requirement 11.1. The alias table is a
// later task's work; until it lands, the flag is accepted and the full Topic Name is
// always emitted, which is the "table full" behavior of Requirement 11.2 and is always
// a legal encoding.
struct MqttPublishRequest {
   const char*    topic         = nullptr;
   const uint8_t* payload       = nullptr;
   unsigned int   plength       = 0;
   bool           retained      = false;
   uint8_t        qos           = 0;         // 0..2 (0 only in runtime level-4 fallback)
   bool           useTopicAlias = false;     // Requirement 11.1
   MqttMessageProperties properties;         // Requirements 11.3 .. 11.9
};

// --- MQTT 5.0 property model (Requirement 3) -------------------------------
// Always compiled as part of this MQTT5-capable client and used for MQTT 5 property
// emission and parsing. Runtime level-4 fallback has no Properties fields on the wire.

// How a property identifier's value is encoded on the wire:
//   Byte       1 byte
//   TwoByte    2 bytes, big-endian
//   FourByte   4 bytes, big-endian
//   VarInt     Variable Byte Integer, 1..2 bytes in this library
//   Utf8       2-byte length + that many bytes
//   Binary     2-byte length + that many bytes
//   StringPair two consecutive Utf8-encoded strings (key then value)
//   None       no encoding at all: the two Enhanced Authentication identifiers
//              (Authentication Method 0x15, Authentication Data 0x16) exist in
//              PROPERTY_TABLE only to be refused, in both directions, so they carry
//              no value type, no packet context and no multiplicity (Requirements
//              3.11, 16.5, 16.6)
enum class MqttPropType : uint8_t { Byte, TwoByte, FourByte, VarInt, Utf8, Binary, StringPair, None };

// Packet contexts for MqttPropertyDef::packets: one bit per MQTT packet type number,
// CONNECT = 1 through AUTH = 15. Packet type 0 is reserved by the specification and its
// bit is reused here for the "Will Properties" pseudo-context, which is a Properties
// field in the CONNECT payload rather than a packet type of its own.
enum MqttPropCtx : uint16_t {
   MQTT_CTX_WILL        = 1u <<  0,   // Will Properties (CONNECT payload), pseudo-context
   MQTT_CTX_CONNECT     = 1u <<  1,
   MQTT_CTX_CONNACK     = 1u <<  2,
   MQTT_CTX_PUBLISH     = 1u <<  3,
   MQTT_CTX_PUBACK      = 1u <<  4,
   MQTT_CTX_PUBREC      = 1u <<  5,
   MQTT_CTX_PUBREL      = 1u <<  6,
   MQTT_CTX_PUBCOMP     = 1u <<  7,
   MQTT_CTX_SUBSCRIBE   = 1u <<  8,
   MQTT_CTX_SUBACK      = 1u <<  9,
   MQTT_CTX_UNSUBSCRIBE = 1u << 10,
   MQTT_CTX_UNSUBACK    = 1u << 11,
   MQTT_CTX_PINGREQ     = 1u << 12,   // no properties exist for PINGREQ
   MQTT_CTX_PINGRESP    = 1u << 13,   // no properties exist for PINGRESP
   MQTT_CTX_DISCONNECT  = 1u << 14,
   MQTT_CTX_AUTH        = 1u << 15
};

// One row of the property identifier table: what the identifier is, how its value is
// encoded, which contexts it is legal in, and whether it may repeat within a single
// Properties field. The table is the single source of truth for both directions,
// validation (Requirements 3.7, 3.8) and emission.
//
// The two Enhanced Authentication rows (0x15 Authentication Method, 0x16
// Authentication Data) are the one deliberate exception to "every row describes a
// property this client handles": `type` is `MqttPropType::None`, `packets` is 0 (legal
// in no context) and `multiple` is false, because those rows exist only so the
// identifier is *recognized* well enough to be rejected at dispatch - the value's
// encoding is never consulted (Requirements 3.11, 16.5, 16.6).
struct MqttPropertyDef {
   uint8_t      id;        // 0x01 .. 0x2A (a single byte in MQTT 5.0)
   MqttPropType type;      // value encoding; `None` for the two rejected identifiers
   uint16_t     packets;   // bitmask of MqttPropCtx values; 0 for the two rejected identifiers
   bool         multiple;  // may appear more than once in one Properties field
};

// --- Property identifiers this client emits --------------------------------
// Named so the writer's call sites read as intent rather than as magic bytes.
#define MQTT_PROP_PAYLOAD_FORMAT_INDICATOR 0x01
#define MQTT_PROP_CONTENT_TYPE             0x03
#define MQTT_PROP_RESPONSE_TOPIC           0x08
#define MQTT_PROP_CORRELATION_DATA         0x09
#define MQTT_PROP_SERVER_KEEP_ALIVE        0x13
#define MQTT_PROP_RECEIVE_MAXIMUM          0x21
#define MQTT_PROP_TOPIC_ALIAS_MAXIMUM      0x22
#define MQTT_PROP_TOPIC_ALIAS              0x23
#define MQTT_PROP_USER_PROPERTY            0x26
#define MQTT_PROP_MAXIMUM_PACKET_SIZE      0x27

// --- Property identifiers this client rejects, in both directions ---------
// Enhanced Authentication is a non-goal in full (Requirements 3.11, 16.3, 16.5,
// 16.6): the client never emits either identifier and closes the connection if
// either arrives, in any packet type. Named so the writer's and reader's rejection
// checks read as intent rather than as magic bytes.
#define MQTT_PROP_AUTHENTICATION_METHOD    0x15
#define MQTT_PROP_AUTHENTICATION_DATA      0x16

// The outbound property set MqttPropertyWriter and writeProperties() consume -
// `MqttMessageProperties` - is defined above because MqttPublishRequest holds one by value.
// See the comment on its definition.

// --- Properties field writer (Requirement 3.1) -----------------------------
//
// Appends MQTT 5.0 property entries (the TLV *body* of a Properties field, without the
// Variable Byte Integer length prefix) to a caller-supplied buffer.
//
// Two modes, selected by the constructor:
//   - `buf == nullptr` is SIZING mode: nothing is written, `bodyLength()` accumulates
//     the exact byte count the body would occupy. This is what lets writeProperties()
//     learn the body length before it has to emit the length prefix, so it needs
//     neither a memmove to insert the prefix afterwards nor a guess at its width.
//   - `buf != nullptr` is EMIT mode: entries are appended at buf[0 .. capacity-1].
//
// `capacity` bounds the body in both modes, so a sizing pass over a set that cannot
// fit reports the overflow instead of returning a length the emit pass could not honor.
//
// Every add() is all-or-nothing: an entry that does not fit writes no byte of itself,
// returns false, and latches overflowed(). Once latched, every later add() is a no-op
// returning false, so a refusal can never leave a half-written or reordered body behind
// (Requirement 13.2).
class MqttPropertyWriter {
public:
   MqttPropertyWriter(uint8_t* buf, uint16_t capacity)
      : buf(buf), capacity(capacity), len(0), overflow(false) {}

   bool addByte    (uint8_t id, uint8_t  v);
   bool addTwoByte (uint8_t id, uint16_t v);
   bool addFourByte(uint8_t id, uint32_t v);
   // Refused when `v` exceeds PubSubClient::MQTT_VBI_MAX, since a Variable Byte Integer
   // above that cannot be encoded (Requirement 4.5).
   bool addVarInt  (uint8_t id, uint32_t v);
   // Refused when `s` (or either half of a pair) is null: a property that was asked for
   // but has no value is a caller error, and silently emitting an empty string would
   // hide it.
   bool addString  (uint8_t id, const char* s);
   bool addBinary  (uint8_t id, const uint8_t* d, uint16_t len);
   bool addPair    (uint8_t id, const char* k, const char* v);

   // Bytes of TLV body appended so far, excluding the Variable Byte Integer prefix.
   uint16_t bodyLength() const { return this->len; }
   // True once any add() has been refused. Sticky.
   bool overflowed() const { return this->overflow; }

private:
   // Room for `need` more body bytes, within both `capacity` and the largest
   // encodable Property Length. Latches `overflow` and returns false otherwise.
   bool reserve(uint32_t need);
   // Length of a property string, refusing null and anything that cannot fit.
   bool measure(const char* s, uint16_t& outLen) const;
   void put(uint8_t b);
   void putBlob(const uint8_t* d, uint16_t n);
   // Refuses Authentication Method (0x15) and Authentication Data (0x16)
   // unconditionally, latching `overflow` like any other refusal, so no add*()
   // overload can ever place either identifier into a transmitted Properties field
   // (Requirements 3.11, 16.5). Called first, before any byte of the entry is
   // measured or reserved.
   bool rejectAuthProperty(uint8_t id);

   uint8_t* buf;        // nullptr = sizing mode
   uint16_t capacity;   // body bytes available
   uint16_t len;        // body bytes used
   bool     overflow;   // sticky refusal flag
};

// Emit a complete Properties field - `<VBI bodyLength><body>` - for `packetType` at
// buf[pos..], and return the new position.
//
// `packetType` is the MQTT packet type number (CONNECT = 1 ... AUTH = 15), or 0 for the
// Will Properties pseudo-context. Only properties the table marks legal for that context
// are emitted; a property supplied for a context it cannot appear in is skipped, so this
// function can never frame a Properties field that the peer would have to reject
// (Requirement 3.7 in the emitting direction).
//
// Runs a sizing pass first, so the length prefix is written at its true width. Returns
// `pos` unchanged - having written nothing at all - when the field would not fit
// `capacity`, so the caller can abort with the buffer and the connection untouched
// (Requirement 13.2). An empty set emits the single byte 0x00 (Requirements 1.2, 1.3,
// 6.3, 9.1, 9.3).
//
// Emission order is fixed: Payload Format Indicator (0x01), Content Type (0x03),
// Response Topic (0x08), Correlation Data (0x09), then every User Property (0x26) in
// the order the caller supplied them (Requirements 11.3, 11.5, 11.8, 11.9).
//
// Response Topic and Correlation Data are emitted independently, allowing both an
// uncorrelated request carrying only a Response Topic and a response that echoes only
// Correlation Data. The sizing and emission passes apply the same independent checks.
uint16_t writeProperties(const MqttMessageProperties& props, uint8_t packetType,
                         uint8_t* buf, uint16_t pos, uint16_t capacity);

// --- Properties field reader (Requirements 3.2, 3.5, 3.6, 3.7, 3.8) --------
//
// One decoded property entry, handed to the visitor.
//
// LIFETIME: `data` and `data2` point INTO the caller's buffer - the client's single
// working buffer, when the caller is the packet parser. Nothing is copied and nothing
// is allocated (Requirement 13.1), so a view is valid only for the duration of the
// visitor call. A visitor that needs a value beyond that must copy it itself.
//
// Which fields carry the value depends on `type`:
//   Byte / TwoByte / FourByte / VarInt   `value`, zero-extended to 32 bits
//   Utf8 / Binary                        `data` + `len` (`len` may be 0)
//   StringPair                           `data` + `len` = key, `data2` + `len2` = value
// Fields that do not apply to the type are zeroed, so a visitor reading the wrong
// one gets a null pointer or a zero rather than stale data.
struct MqttPropertyView {
   uint8_t        id    = 0;
   MqttPropType   type  = MqttPropType::Byte;
   uint32_t       value = 0;         // Byte / TwoByte / FourByte / VarInt
   const uint8_t* data  = nullptr;   // Utf8 / Binary / StringPair key
   uint16_t       len   = 0;
   const uint8_t* data2 = nullptr;   // StringPair value
   uint16_t       len2  = 0;
};

// Called once per decoded entry, in wire order. Returning false aborts the walk (see
// decodeProperties below for exactly what that means). A plain function pointer, not a
// std::function: the reader is on the packet path and must not allocate.
using MqttPropertyVisitor = bool (*)(const MqttPropertyView&, void* ctx);

// Decode `buf[0 .. avail-1]` as a Properties field appearing in `packetType`, where
// `packetType` is the MQTT packet type number (CONNECT = 1 ... AUTH = 15) or 0 for the
// Will Properties pseudo-context. A `packetType` above 15 has no legal property at all,
// so only an empty field decodes for it.
//
// `avail` is the number of bytes ACTUALLY RECEIVED from this point on, not a declared
// length. No byte at or beyond `buf[avail]` is ever read: every length is checked before
// the read it authorises (Requirements 13.4, 13.5).
//
// Returns `MqttError::None` on success, and then sets `consumed` to the Property Length
// prefix bytes plus the declared body bytes - i.e. where the field ends and the next
// wire field begins. On any failure `consumed` is left at 0 and the caller should reject
// the packet; closing the connection is the caller's job, not the reader's.
//
// `visit` may be null, which validates the field without visiting anything.
//
// VISITOR ABORT: when the visitor returns false the walk stops immediately - no further
// entry is decoded or visited - and the call still returns `MqttError::None` with
// `consumed` set to the full field length. That position is safe to resume from because
// the declared body length was bounds-checked against `avail` before the first entry was
// touched. The trade is that entries after the abort point are left unvalidated, so a
// visitor that aborts opts out of the Requirement 3.6 / 3.7 / 3.8 checks on the
// remainder; a visitor that wants the whole field validated returns true throughout. The
// visitor's own `ctx` is where it records that it aborted, since the return value does
// not distinguish an abort from a complete walk.
//
// `outReasonCode`, when non-null, is set to the specific DISCONNECT Reason Code the
// caller should use INSTEAD OF the generic category default, but only for the one
// failure that carries a more specific code than its category: Authentication Method
// (0x15) or Authentication Data (0x16) arriving in ANY packetType is a `ProtocolError`
// like any other illegal-identifier rejection, but it carries Reason Code 0x8C (Bad
// authentication method) rather than the generic 0x82 (Requirements 3.11, 16.5, 16.6).
// Left untouched - not even zeroed - for every other outcome, so a caller must
// initialize it itself and treat "still at its initial value" as "use the category
// default via PubSubClient::defaultReasonCode()".
MqttError decodeProperties(const uint8_t* buf, uint32_t avail, uint8_t packetType,
                           MqttPropertyVisitor visit, void* ctx, uint32_t& consumed,
                           uint8_t* outReasonCode = nullptr);

class PubSubClient : public Print {
public:
   // --- Correlation_Data_Storage (Requirements 3.9, 11.11, 13.7, 13.8) -------
   //
   // The 16-byte cap is a requirement, not a configuration knob: raising it would
   // change what the client accepts on the wire. A constant, deliberately NOT a
   // tunable macro. Declared first among the class's members because
   // `correlationDataStorage` below is sized by it.
   static constexpr uint8_t MQTT_CORRELATION_DATA_MAX = 16;

#ifdef USE_MQTT_DETAILED_LOGGING_BINARY
   // --- Raw wire packet logging hook (USE_MQTT_DETAILED_LOGGING_BINARY) -------
   //
   // Optional diagnostic: when a callback is registered, the client reports every
   // complete packet it reads off the transport and every buffer it writes to the
   // transport, as raw bytes, so the host application (Tasmota's xdrv_02_9_mqtt
   // driver) can hex-dump them. `tx` is true for outbound (sent) bytes and false for
   // inbound (received) bytes; `data`/`len` are only valid for the duration of the
   // call. This never allocates and is compiled out entirely when the macro is off.
   typedef void (*WireLogCallback)(bool tx, const uint8_t* data, uint32_t len);
   void setWireLogCallback(WireLogCallback cb) { this->wireLogCb = cb; }
#endif  // USE_MQTT_DETAILED_LOGGING_BINARY

private:
// All members are default-initialised. The constructors do not assign every field
// (nextMsgId, the activity timestamps, pingOutstanding, port), and `buffer` was left
// dangling when the initial malloc() failed - the destructor then free()d garbage.
   Client* _client = nullptr;
   uint8_t* buffer = nullptr;
   uint16_t bufferSize = 0;
   uint16_t keepAlive = 0;
   uint16_t socketTimeout = 0;
   // One deadline covers receipt of the entire packet, rather than granting every
   // byte a fresh socketTimeout interval. A zero deadline means no packet is being
   // assembled. Until explicitly configured, it follows setSocketTimeout().
   uint16_t packetTimeout = MQTT_PACKET_TIMEOUT;
   unsigned long packetDeadline = 0;
   bool packetTimeoutConfigured = false;
   uint16_t nextMsgId = 0;
   unsigned long lastOutActivity = 0;
   unsigned long lastInActivity = 0;
   bool pingOutstanding = false;

   // State for the beginPublish()/write()/endPublish() streaming contract. A
   // mismatch leaves a truncated PUBLISH on the wire, so it is terminal.
   bool publishInProgress = false;
   size_t publishBytesExpected = 0;
   size_t publishBytesWritten = 0;
   MQTT_CALLBACK_SIGNATURE;
   uint32_t readPacket(uint8_t*);
   boolean readByte(uint8_t * result);
   boolean readByte(uint8_t * result, uint16_t * index);
   bool packetDeadlineExpired() const;
   void abortPacketRead();
   void transportWriteFailed();

   // Every outbound byte funnels through these two helpers instead of touching
   // `_client->write()` directly, so the raw wire logging hook has a single choke
   // point. With USE_MQTT_DETAILED_LOGGING_BINARY off they are inline pass-throughs
   // with no overhead; with it on they report the bytes before forwarding them.
#ifdef USE_MQTT_DETAILED_LOGGING_BINARY
   WireLogCallback wireLogCb = nullptr;
   size_t transportWrite(const uint8_t* buf, size_t size);
   size_t transportWrite(uint8_t b);
   void   logWirePacket(bool tx, const uint8_t* data, uint32_t len);
#else
   inline size_t transportWrite(const uint8_t* buf, size_t size) { return _client->write(buf, size); }
   inline size_t transportWrite(uint8_t b) { return _client->write(b); }
#endif  // USE_MQTT_DETAILED_LOGGING_BINARY

   boolean write(uint8_t header, uint8_t* buf, uint16_t length);
   uint16_t writeString(const char* string, uint8_t* buf, uint16_t pos);
   // Build up the header ready to send
   // Returns the size of the header, or 0 if `length` cannot be encoded
   // Note: the header is built at the end of the first MQTT_MAX_HEADER_SIZE bytes, so will start
   //       (MQTT_MAX_HEADER_SIZE - <returned size>) bytes into the buffer
   size_t buildHeader(uint8_t header, uint8_t* buf, uint32_t length);   // 32-bit Remaining Length

   // True when a fully framed packet with `remainingLength` bytes after its fixed header
   // would exceed the broker's CONNACK Maximum Packet Size. A zero broker limit means
   // the property was absent and imposes no outbound limit.
   bool exceedsServerMaximumPacketSize(uint32_t remainingLength);

   // --- Outbound PUBLISH assembler (Requirements 6.1, 6.2, 6.3, 8.3, 8.17) ---
   //
   // The single place a PUBLISH variable header is built, shared by publish(),
   // publish_P() and beginPublish(). Assembles Topic Name, Packet Identifier (QoS > 0
   // only) and - in MQTT 5.0 - the Properties field into the working buffer behind the
   // reserved header space, and hands back the fixed-header byte in `headerByte`.
   //
   // Returns the buffer position just past the variable header, or 0 when the packet is
   // refused. Every gate runs before anything reaches the transport, so a refusal
   // transmits nothing and leaves the connection state unchanged (Requirements 13.2,
   // 4.9, 8.17).
   //
   // `payloadLength` is the DECLARED payload byte count - the real payload for the two
   // buffered paths, beginPublish()'s promise for the streaming one. `payloadBuffered`
   // says whether those bytes are additionally copied into the working buffer, which is
   // what separates publish()'s buffer gate from publish_P()/beginPublish()'s.
   uint16_t buildPublishHeader(const char* topic, uint8_t qos, boolean dup,
                               boolean retained, uint16_t packetId,
                               const MqttMessageProperties* props, boolean useTopicAlias,
                               uint32_t payloadLength, boolean payloadBuffered,
                               uint8_t& headerByte);
   IPAddress ip;

   String domain;

   uint16_t port = 0;          // 0 = no usable server configured
   Stream* stream = nullptr;
   int _state = MQTT_DISCONNECTED;

// This MQTT5-capable client starts at protocol level 5 and can retry once at level 4
// after an unsupported-version response. MqttSessionIdentity::protocolLevel stores the
// selected level, avoiding a second protocol field and repeated constructor initialization.
   void selectMqtt311Fallback();

// --- Local MQTT 5 session identity (Task 18.7 / Finding 3) ----------------
// Local replay material belongs to exactly one persistent MQTT session. The identity
// deliberately distinguishes a DNS endpoint from a numeric address even if they might
// resolve to the same peer: retaining too little identity could replay a message to the
// wrong broker, while retaining too much only conservatively declines a replay.
   struct MqttSessionIdentity {
      bool     valid = false;
      bool     endpointIsDomain = false;
      String   endpointDomain;
      uint8_t  endpointAddress[4] = {};
      uint16_t endpointPort = 0;
      String   clientId;
      uint8_t  protocolLevel = 0;
      bool     cleanSession = true;
      uint32_t sessionExpiryInterval = 0;
   } sessionIdentity;

// Configured policy used when the legacy public `cleanSession` argument is false.
// It is stored independently from sessionIdentity because a change takes effect on the
// next CONNECT and must make old replay state incompatible rather than mutating it.
   uint32_t persistentSessionExpiryInterval = MQTT_PERSISTENT_SESSION_EXPIRY_INTERVAL;

   void clearSessionIdentity();
   void captureSessionIdentity(const char* clientId, bool cleanSession);
   bool sessionIdentityMatches(const char* clientId, bool cleanSession) const;

// Hard cap on the total size (fixed header + Remaining Length) of an inbound packet
// that the client will accept, independent of the working buffer allocation.
// 0 = disabled: fall back to the buffer size for non-stream, unbounded for stream.
   uint32_t maxIncomingPacketSize = 0;

// --- Error state (Requirement 14.2) ---------------------------------------
// Diagnostics for the most recent failure. All four are cleared at the start of
// every connection attempt so a value read after a successful connect always
// describes the current connection.
   MqttError errorState = MqttError::None;
   uint8_t   errorReasonCode = 0;
   uint16_t  errorPacketId = 0;
   uint8_t   disconnectReasonCode = 0;

// Clear the error diagnostics. Called at the start of every connection attempt.
   void resetErrorState() {
      this->errorState = MqttError::None;
      this->errorReasonCode = 0;
      this->errorPacketId = 0;
      this->disconnectReasonCode = 0;
   }

// --- CONNACK outcome (Requirements 1.4, 1.5, 1.6) -------------------------
// Latched from the CONNACK variable header: Session Present is ack-flags bit 0, and
// the Reason Code is the byte after it. Both describe the connection that CONNACK
// answered for, so they are readable for its whole lifetime AND after it ends - a
// driver that wants to know why a connect failed, or whether the broker still held
// its session, reads them after connect() has already returned false.
   bool    sessionPresentFlag = false;
   uint8_t connackRc = 0;

// Clear the CONNACK outcome. Called at the START OF A CONNECTION ATTEMPT and nowhere
// else - deliberately NOT from resetConnectionState(), which also runs on disconnect()
// and on every protocolFailure(). Requirement 1.6 says the accessor stays readable
// "until the next connection attempt begins", which is later than "until the
// connection ends": clearing it on teardown would blank the value exactly when the
// driver goes looking for it.
   void resetConnackState() {
      this->sessionPresentFlag = false;
      this->connackRc = 0;
   }

// --- Negotiated limits latched from CONNACK (Requirements 5.1, 5.4, 5.7) --------
//
// The four values the broker may declare in its CONNACK Properties field. Each one is
// per-connection state: it is reset to its MQTT 5.0 default at the start of every
// connection attempt, so a value declared by one broker can never govern the next
// connection (Requirement 5.7). Enforcement of the limits is task 14's job; parsing
// and latching them is this task's.
//
// Server Keep Alive (0x13) has no member of its own: it overwrites `keepAlive`
// directly, which is what makes the existing loop() keepalive arithmetic - including
// the `keepAlive == 0` disable guard - apply to the negotiated value unchanged
// (Requirement 5.1). `keepAliveRequested` is what makes that safe: it holds the value
// setKeepAlive() was given, so the default "requested keepalive kept" is restorable
// after a broker has overwritten `keepAlive` with its own.
   uint16_t keepAliveRequested = 0;
// Maximum Packet Size (0x27). 0 = no limit, which is both the MQTT 5.0 default when
// the property is absent and the sentinel used internally.
   uint32_t serverMaxPacketSize = 0;
// Topic Alias Maximum (0x22). 0 = the broker accepts no alias at all.
   uint16_t serverTopicAliasMax = 0;
// Receive Maximum (0x21). 65535 is the MQTT 5.0 default when the property is absent.
   uint16_t serverReceiveMaximum = 65535;

// --- Outbound topic aliases (Requirements 11.1, 11.2, 5.4) ----------------
// Dynamic nodes cap memory by count; each entry owns its full topic in a String. This
// state is per connection and reset at both the connection-attempt and teardown bounds.
   LList<MqttTopicAliasEntry> topicAliases;

   uint16_t outboundTopicAliasLimit() const;
   MqttTopicAliasEntry* findTopicAlias(const char* topic);
   void clearTopicAliases() { this->topicAliases.reset(); }

// Restore all four to their MQTT 5.0 defaults. Called at the start of every connection
// attempt, and again from resetConnectionState() so nothing survives a teardown.
   void resetNegotiatedLimits() {
      this->keepAlive = this->keepAliveRequested;
      this->serverMaxPacketSize = 0;
      this->serverTopicAliasMax = 0;
      this->serverReceiveMaximum = 65535;
   }

// --- Inbound QoS 2 registry (Requirements 8.10, 8.11, 8.15) --------------
//
// Each nonzero entry is a Packet Identifier whose inbound QoS 2 PUBLISH was
// delivered and acknowledged with PUBREC, but whose matching PUBREL has not yet
// completed the exchange. Keeping this fixed-size registry prevents a duplicate
// PUBLISH from reaching the callback a second time. The CONNECT Receive Maximum
// advertises this exact capacity to the broker. It is persistent local Session State:
// a matching nonzero-expiry session retains it across transport loss until PUBREL or a
// CONNACK with Session Present 0; Clean Start and identity changes clear it before CONNECT.
   uint16_t inboundQos2[MQTT_MAX_INBOUND_INFLIGHT] = {};
   static_assert(MQTT_MAX_INBOUND_INFLIGHT >= 1 && MQTT_MAX_INBOUND_INFLIGHT <= 65535,
                 "MQTT_MAX_INBOUND_INFLIGHT must be in 1..65535");

   bool inboundQos2Contains(uint16_t packetId) const;
   bool registerInboundQos2(uint16_t packetId);
   bool releaseInboundQos2(uint16_t packetId);
   void clearInboundQos2();

// --- Inbound PUBLISH property set (Requirements 11.4, 11.7, 12.4, 12.5, 12.6) ---
//
// The decoded property set of the most recent inbound PUBLISH, plus where that packet's
// Properties field sits in the working buffer so the User Properties can be re-walked
// on demand instead of cached. The extent is kept as an OFFSET rather than a pointer so
// a buffer reallocation cannot leave a dangling one behind: the offset is re-validated
// against `bufferSize` before it is used.
   MqttInboundProperties inboundProps;
   uint16_t inboundPropsOff = 0;   // offset of the Properties field within `buffer`
   uint16_t inboundPropsLen = 0;   // its full width: Property Length prefix + body

// --- Correlation_Data_Storage (Requirements 3.9, 11.11, 13.7, 13.8) --------
//
// Fixed extent, part of the client's own footprint. Never heap-allocated, never
// sized per packet: the byte count of memory used for Correlation Data is identical
// for every message and independent of the value's actual length. `inboundProps.
// correlationData` is set to point at this array (or left null when absent), so the
// value's address is the same for every message and never aliases the packet's
// property region in the working buffer.
   uint8_t correlationDataStorage[MQTT_CORRELATION_DATA_MAX];
   uint8_t correlationDataLen = 0;     // 0..16; meaningful only while hasCorrelationData
   bool    hasCorrelationData = false; // cleared at the start of every inbound PUBLISH parse

// Drop the set back to "nothing delivered". Called at the START of every inbound
// PUBLISH parse, so a packet rejected part-way through can never leave the previous
// packet's values readable as if they were the current ones.
   void resetInboundProperties() {
      this->inboundProps = MqttInboundProperties();
      this->inboundPropsOff = 0;
      this->inboundPropsLen = 0;
      this->correlationDataLen = 0;
      this->hasCorrelationData = false;
   }

// True when the transport can still take bytes, i.e. a DISCONNECT may be emitted
// before closing (Requirement 10.5). False means "close silently" (Requirement 10.6).
   boolean socketWritable();

// Emit a DISCONNECT on the current connection. `shortForm` selects the 2-byte form
// `E0 00` (Reason Code 0x00 implied, including runtime level-4 fallback), otherwise the
// 3-byte error form `E0 01 <reasonCode>` with no Properties. Writability is the caller's
// decision; this helper only frames and writes.
   void sendDisconnect(uint8_t reasonCode, bool shortForm);

// --- Acknowledgement assembler (Requirements 7.1, 7.2, 7.6, 7.7) -----------
//
// The single place a PUBACK, PUBREC, PUBREL or PUBCOMP is framed and written.
// `packetType` is the fixed-header type byte (MQTTPUBACK ... MQTTPUBCOMP).
//
//   Reason Code 0x00 -> short form, Remaining Length 2:  <type> 02 <id hi> <id lo>
//   Reason Code != 0 -> Remaining Length 3:              <type> 03 <id hi> <id lo> <rc>
//
// The short form is what every acknowledgement this client emits uses, since the
// client only ever acknowledges successfully (Requirements 7.1, 7.2, 7.6), so its bytes
// are valid at both runtime protocol levels. The Reason Code byte is emitted only at
// runtime level 5, because Remaining Length 3 is a 5.0-only form; level-4 fallback cannot
// frame it even if a caller asks. Properties are never emitted: this client has none to
// send on an acknowledgement, and the short form is exactly "Reason Code 0x00 and no
// properties needed".
//
// PUBREL always carries fixed-header flags 0x02, which the specification requires
// (Requirement 7.7); the other three carry flags 0.
//
// Returns true when the transport accepted the write. Activity bookkeeping is the
// caller's, so the packet path can keep using its own already-sampled timestamp.
   boolean sendAcknowledgement(uint8_t packetType, uint16_t packetId, uint8_t reasonCode);

// --- SUBSCRIBE / UNSUBSCRIBE assembler (Requirements 9.1, 9.2, 9.3, 9.7) ---
//
// The single place a SUBSCRIBE or an UNSUBSCRIBE is framed and written. `packetType` is
// MQTTSUBSCRIBE or MQTTUNSUBSCRIBE; `options` is non-null for SUBSCRIBE (which carries a
// Subscription Options byte after its filter) and null for UNSUBSCRIBE (which does not).
//
//   fixed header      : <type> | 0x02              (mandatory reserved-bit value)
//   Packet Identifier : 2 bytes, never 0
//   Properties        : runtime protocol level 5 only, between the identifier and the
//                       first filter (Requirements 9.1, 9.3)
//   Topic Filter      : 2-byte length prefix + bytes
//   Subscription Opts : 1 byte, SUBSCRIBE only (Requirement 9.2)
//
// Every gate - null topic, out-of-range QoS, buffer capacity, free outstanding slot -
// runs before anything reaches the transport, so a refusal transmits nothing and leaves
// the connection state unchanged (Requirements 9.7, 9.9).
   boolean sendSubscription(uint8_t packetType, const char* topic,
                            const MqttSubscribeOptions* options,
                            uint16_t* packetIdOut);

// --- Outstanding operation table -------------------------------------------
//
// Fixed tracked-exchange capacity; see MqttOutstandingMessage for the owned topic/filter.
// MQTT_MAX_OUTSTANDING reserves MQTT_MAX_INFLIGHT publish slots plus the documented pending
// subscription envelope. It is always declared, but populated only at runtime protocol level 5.
   MqttOutstandingMessage outstanding[MQTT_MAX_OUTSTANDING];

// The table is the complete set of Packet Identifiers currently HELD, so allocator scans skip
// every live PUBLISH, SUBSCRIBE, and UNSUBSCRIBE exchange, including one whose local timeout
// diagnostic has already been reported. At most MQTT_MAX_OUTSTANDING IDs are held, so the
// rotor finds a free candidate within MQTT_MAX_OUTSTANDING + 1 steps.
//
// The total is capped at 32: four publish slots plus the default twelve tracked subscription
// slots cost a modest fixed record array; each occupied slot additionally owns only its full
// filter/topic String. A count of 0 would declare a zero-length array, and the uint8_t scan
// indices require a bound below 256.
   static_assert(MQTT_MAX_INFLIGHT >= 1 && MQTT_MAX_INFLIGHT <= 32,
                 "MQTT_MAX_INFLIGHT must be in 1..32");
   static_assert(MQTT_MAX_PENDING_SUBSCRIPTIONS >= 9 && MQTT_MAX_OUTSTANDING <= 32,
                 "MQTT_MAX_PENDING_SUBSCRIPTIONS must cover Tasmota's nine fixed operations and keep the total at most 32");

// Take the first free slot and record `packetId`, `state` and the FULL `topic`. Returns
// the entry, or nullptr when every slot is occupied - which the caller turns into a
// refusal that transmits nothing.
   MqttOutstandingMessage* claimOutstanding(uint16_t packetId, MqttOutState state,
                                            const char* topic);
// The occupied entry holding `packetId`, or nullptr when none does. A `packetId` of 0
// never matches: 0 is not a legal Packet Identifier.
   MqttOutstandingMessage* findOutstanding(uint16_t packetId);
// Free `entry` only after its terminal acknowledgement or definitive session termination.
// Its retained `topic` String is deliberately LEFT INTACT so a MqttSubscriptionResult view
// remains readable until this slot is claimed again.
   void releaseOutstanding(MqttOutstandingMessage* entry);
// Free every slot and drop retained topics unless `preserveResumable` is true for a
// transport loss while reconnecting to the same persistent identity. In that case every
// held operation remains local Session State until CONNACK reports whether the broker kept
// the session; only states accepted by resendsOnSessionResume() are transmitted again.
   void clearOutstanding(bool preserveResumable = true);

// --- Packet Identifier allocation (Requirements 8.4, 8.5) ------------------
//
// ONE allocator serves PUBLISH, SUBSCRIBE and UNSUBSCRIBE, which is what makes it
// impossible for a publish to share an identifier with a subscription (Requirement 8.4).
//
// `nextMsgId` keeps its long-standing role as the ROTOR and is reset to 1 on connect.
// Each call advances it - wrapping 65535 -> 1 so 0, which is not a legal Packet Identifier,
// is never yielded - and skips every held entry in the MQTT_MAX_OUTSTANDING table. The scan
// is linear over at most MQTT_MAX_OUTSTANDING entries and runs at most
// MQTT_MAX_OUTSTANDING + 1 times, so allocation stays bounded.
//
// Returns 1..65535. A 0 result is unreachable with the fixed table bound, but callers still
// treat it as an atomic refusal. A free table slot is distinct from a free identifier; both a
// QoS publish and a SUBSCRIBE/UNSUBSCRIBE are refused before writing when the tracked envelope
// is full, so no live exchange is sent untracked or loses its identifier.
   uint16_t allocatePacketId();
// Give `id` back for reuse: the table is the held-identifier set, so releasing an
// identifier means freeing the entry that holds it. A `id` of 0, or one no entry holds,
// is a no-op.
   void releasePacketId(uint16_t id);

// --- In-flight retention and retransmission (Requirements 8.7, 8.19 - 8.24) -
//
// Compiled only when USE_MQTT_QOS is defined - this IS the outbound QoS 1/2 replay buffer
// and retransmission machinery. The helpers are additionally reached only on the runtime
// level-5 paths, so level-4 fallback neither retains nor retransmits packet bodies.
#ifdef USE_MQTT_QOS

// Total bytes currently retained across every entry holding a copy, i.e. what
// MQTT_INFLIGHT_COPY_BUDGET bounds.
   uint32_t inflightCopyBytes = 0;

// Retain `body[0 .. bodyLen-1]` - the assembled PUBLISH body - plus its fixed-header byte on
// `entry`, so the message can be retransmitted byte-identically later.
//
// Returns true only after the complete copy is allocated and initialized, at which point the
// budget is committed. Returns false without modifying `entry` or the budget when the body
// would exceed MQTT_INFLIGHT_COPY_BUDGET or allocation fails; the caller atomically releases
// its tentative slot and identifier before returning MqttError::RetentionUnavailable.
   boolean retainPacketCopy(MqttOutstandingMessage* entry, uint8_t headerByte,
                            const uint8_t* body, uint16_t bodyLen);

// Drop `entry`'s retained copy, if it holds one, and give its bytes back to the budget.
// Idempotent.
   void releasePacketCopy(MqttOutstandingMessage& entry);

// True when `slot` holds everything a PUBLISH REPLAY needs AND the PUBLISH itself is still
// unacknowledged - the two states in which [MQTT-4.4.0-1] says a PUBLISH is resent. An entry
// in AwaitingPubcomp answers false here: its PUBLISH has already been acknowledged by PUBREC,
// so there is nothing worth replaying, and what that entry resends instead is a PUBREL - see
// resendsOnSessionResume().
   static boolean holdsRetainedBody(const MqttOutstandingMessage& slot);

// True when `slot` has SOMETHING to resend after a CONNACK reporting Session Present 1.
// Two kinds qualify, and they resend different packets:
//
//   - a PUBLISH replay, i.e. holdsRetainedBody() (Requirements 8.7, 8.22);
//   - a PUBREL resend, i.e. any entry in AwaitingPubcomp - whose PUBREL this client already
//     sent and whose PUBCOMP never came. It needs no retained body, so it qualifies
//     independently of the PUBLISH retention budget (Requirement 8.23).
//
// Other held operations still belong to persistent local Session State and retain their
// identifiers across a compatible transport loss, but MQTT defines no automatic replay for
// them. clearOutstanding() preserves them until CONNACK definitively disposes the session.
   static boolean resendsOnSessionResume(const MqttOutstandingMessage& slot);
#endif  // USE_MQTT_QOS

// True after this client established a nonzero-expiry persistent session with the complete
// matching endpoint/client/protocol/policy identity. The identity is sufficient even with no
// replayable packet: the broker may still hold subscriptions that make Session Present
// meaningful. An explicit zero expiry can never qualify after transport loss.
   bool hasResumableLocalSessionState(const char* clientId, bool cleanSession) const;

#ifdef USE_MQTT_QOS
// Resend every entry resendsOnSessionResume() accepts: a retained PUBLISH body with the DUP
// flag set and its original Packet Identifier (Requirement 8.7), or a PUBREL carrying the
// entry's Packet Identifier for an entry awaiting PUBCOMP (Requirement 8.23). Called from ONE
// place only: immediately after a CONNACK that reports Session Present 1 has put the client
// into MQTT_CONNECTED. Every entry is rechecked against the pre-CONNECT persistent-session
// identity and the newly negotiated Maximum Packet Size. It returns false at the first local
// framing or transport failure and writes no later entry.
   bool retransmitOutstanding(const char* clientId, bool cleanSession);

// Close a failed replay attempt without adding DISCONNECT to a stream that may already hold a
// partial replay. The session-state teardown keeps every matching persistent-session entry
// until a later CONNACK makes its definitive Session Present decision.
   void replayFailure(MqttError error, uint16_t packetId);
#endif  // USE_MQTT_QOS

// Abandon every operation carried across a compatible transport loss. Called when CONNACK
// reports Session Present 0, which definitively says none of those operations can still be
// acknowledged by the broker. Retained packet bodies and held identifiers are released.
   void discardRetransmittable();

// Report `entry` as locally timed out, exposing its identifier and a synthesized failure
// Reason Code through lastPacketId() / lastReasonCode(). This is diagnostic-only: it does not
// close the connection, release the entry, or make the Packet Identifier reusable.
   void reportAbandoned(const MqttOutstandingMessage& entry);

// On the first timeout per entry, report through reportAbandoned() and set its diagnostic
// latch while retaining the live operation and identifier. Called once per loop() with
// loop()'s sampled timestamp; MQTT_ACK_TIMEOUT of 0 disables the diagnostic.
   void abandonExpiredOutstanding(unsigned long now);

#ifdef USE_MQTT_QOS
// Count only outbound QoS 1/2 publications. SUBSCRIBE and UNSUBSCRIBE entries share
// the outstanding table for identifier correlation but do not consume the broker's
// Receive Maximum credit.
   uint16_t outstandingPublishCount() const;

// --- PUBACK correlation for outbound QoS 1 (Requirements 7.3, 7.4, 8.16) ---
//
// Complete the outbound QoS 1 PUBLISH that a received PUBACK acknowledges: expose
// `reasonCode` through lastReasonCode() and `packetId` through lastPacketId() - the same
// two accessors an abandoned entry and a failing SUBACK report through - then release the
// entry and give its identifier back to the allocator (Requirement 8.16).
//
// `reasonCode` is the value the parser produced, with an absent Reason Code already
// defaulted to 0x00 (Requirement 7.3), and it is reported for EVERY code rather than only
// for the failure range: releasing the entry on a code of 0x80 or above exactly as on a
// success code is what makes such a code TERMINAL ([MQTT-4.4.0-2]) - the message counts as
// acknowledged and nothing is left behind for a retransmission to pick up.
//
// A `packetId` no entry holds, or one held by an operation that is not an outbound QoS 1
// PUBLISH awaiting its PUBACK, is discarded with every outstanding message left untouched
// (Requirement 7.8). Does NOT touch `errorState`: a broker refusing one message is a
// normal answer, not a transport failure.
   void reportPublishAck(uint16_t packetId, uint8_t reasonCode);

// --- Outbound QoS 2 handshake (Requirements 8.12, 8.14) --------------------
//
// Step one: a received PUBREC answering the outbound QoS 2 PUBLISH that holds `packetId`.
//
//   `reasonCode` below 0x80 - the broker has taken the message: send exactly ONE PUBREL
//   carrying the same identifier with the mandatory fixed-header flags 0x02 (Requirement
//   7.7), and move the entry to AwaitingPubcomp - the design's `PubrelSent` - where it
//   keeps holding its identifier until the PUBCOMP arrives (Requirement 8.12). The
//   Reason Code is recorded on the entry but NOT latched into lastReasonCode(): the
//   exchange is not over, and a mid-handshake success would only overwrite whatever
//   diagnostic the caller has yet to read. `sentAt` is restamped with `now`, so the
//   MQTT_ACK_TIMEOUT deadline measures the wait for the PUBCOMP rather than counting the
//   time the PUBREC already took.
//
//   `reasonCode` of 0x80 or above - the broker has refused it: NO PUBREL is written, the
//   entry is released and its identifier goes back to the allocator, and the received code
//   together with the released identifier is exposed through lastReasonCode() /
//   lastPacketId() as the failure result (Requirement 8.14).
//
// Returns true only when a PUBREL was accepted by the transport, so the caller can refresh
// its outbound-activity stamp; false on the failure path, on a discard, and when the write
// itself failed.
//
// EXACTLY ONE PUBREL is what the state guard buys: only an entry in AwaitingPubrec is
// advanced, so a duplicate PUBREC arriving after the PUBREL went out is discarded like any
// other uncorrelated acknowledgement (Requirement 7.8) rather than emitting a second one.
// A `packetId` no entry holds, or one held by anything other than an outbound QoS 2 PUBLISH
// awaiting its PUBREC, is discarded with every outstanding message left untouched. If the
// PUBREL write is refused by the transport the entry deliberately STAYS in AwaitingPubrec:
// no PUBREL reached the broker, so no PUBCOMP can follow, and leaving it lets the deadline
// or the teardown abandon it instead of recording a completion that never happened.
   boolean reportPublishRec(uint16_t packetId, uint8_t reasonCode, unsigned long now);

// Step two: the PUBCOMP that ends the exchange (Requirement 8.12). The entry - which must
// be in AwaitingPubcomp, i.e. this client has sent the PUBREL - is completed and its
// identifier released, with the numeric Reason Code exposed through lastReasonCode() and
// the identifier through lastPacketId(), exactly as reportPublishAck() does for QoS 1.
// Anything else is discarded with every outstanding message untouched (Requirement 7.8).
   void reportPublishComp(uint16_t packetId, uint8_t reasonCode);
#endif  // USE_MQTT_QOS

// --- SUBACK / UNSUBACK reporting (Requirements 9.4, 9.5, 9.6) --------------
   MqttSubscribeCallback  subscribeCallback = nullptr;
   void*                  subscribeCallbackCtx = nullptr;
   MqttSubscriptionResult lastSubResult;

// Report the sole Reason Code from a structurally and semantically valid SUBACK or
// UNSUBACK only when its identifier belongs to the expected pending state: SUBACK maps to
// AwaitingSuback and UNSUBACK maps to AwaitingUnsuback. Wrong-type, unknown, and otherwise
// mismatched acknowledgements leave the callback, reported results, and held identifier
// untouched (Task 18.5 / Finding 1).
   void reportSubscriptionAck(uint8_t packetType, uint16_t packetId,
                              const uint8_t* codes, uint32_t codeCount);

// Clear everything that is only meaningful for the connection being torn down, so the
// driver's next connect() starts from a clean slate (Requirement 14.2).
   void resetConnectionState();
public:
   PubSubClient();
   PubSubClient(Client& client);
   PubSubClient(IPAddress, uint16_t, Client& client);
   PubSubClient(IPAddress, uint16_t, Client& client, Stream&);
   PubSubClient(IPAddress, uint16_t, MQTT_CALLBACK_SIGNATURE,Client& client);
   PubSubClient(IPAddress, uint16_t, MQTT_CALLBACK_SIGNATURE,Client& client, Stream&);
   PubSubClient(uint8_t *, uint16_t, Client& client);
   PubSubClient(uint8_t *, uint16_t, Client& client, Stream&);
   PubSubClient(uint8_t *, uint16_t, MQTT_CALLBACK_SIGNATURE,Client& client);
   PubSubClient(uint8_t *, uint16_t, MQTT_CALLBACK_SIGNATURE,Client& client, Stream&);
   PubSubClient(const char*, uint16_t, Client& client);
   PubSubClient(const char*, uint16_t, Client& client, Stream&);
   PubSubClient(const char*, uint16_t, MQTT_CALLBACK_SIGNATURE,Client& client);
   PubSubClient(const char*, uint16_t, MQTT_CALLBACK_SIGNATURE,Client& client, Stream&);

   ~PubSubClient();

// The class owns and frees `buffer`; a copy would share ownership and lead to a
// double-free / use-after-free. Copying is therefore disabled.
   PubSubClient(const PubSubClient&) = delete;
   PubSubClient& operator=(const PubSubClient&) = delete;

   // --- Variable Byte Integer codec ---------------------------------------
   //
   // The MQTT length encoding used by the fixed-header Remaining Length and, in
   // MQTT 5.0, by every Properties field length. Public statics so the encoding
   // can be exercised directly without touching private state.

   // Largest encodable value: this codec is deliberately hard-limited to 2 bytes
   // (0x3FFF). Anything above it is refused outright rather than framed, so a
   // truncated or wrapped length can never reach the wire.
   static constexpr uint32_t MQTT_VBI_MAX = 16383u;

   // Encode `value` into `out` (which must have room for 2 bytes). Returns the
   // number of bytes written (1..2), or 0 when `value` exceeds MQTT_VBI_MAX - in
   // which case NOTHING is written to `out`.
   static uint8_t encodeVariableByteInteger(uint32_t value, uint8_t* out);

   // Number of bytes `value` would occupy: 1 for 0..127, 2 for 128..16383,
   // 0 when out of range.
   static uint8_t variableByteIntegerSize(uint32_t value);

   // Decode from buf[0..avail-1]. Returns true and sets `value`/`bytesUsed` on
   // success. Returns false, reading no byte at or beyond `avail`, when the field
   // is truncated within `avail` or when a 3rd byte is implied (the 2nd byte still
   // has its continuation bit set).
   static bool decodeVariableByteInteger(const uint8_t* buf, uint32_t avail,
                                         uint32_t& value, uint8_t& bytesUsed);

   // --- MQTT 5.0 property identifier table --------------------------------
   //
   // Look up `id` in the property table. Returns the definition, or nullptr when `id`
   // is not an MQTT 5.0 property identifier - which the reader treats as a protocol
   // error (Requirement 3.7). Legality per context is then `def->packets & <ctx bit>`,
   // and `def->multiple` answers the duplicate question (Requirement 3.8).
   //
   // The table itself is file-private to PubSubClient.cpp and lives in PROGMEM on
   // ESP8266. The returned pointer is valid until the next call on that target,
   // because the matching row is copied out of flash into a single scratch object;
   // callers read the definition and do not retain the pointer.
   static const MqttPropertyDef* findPropertyDef(uint8_t id);

   // Number of identifiers in the table (27), so a caller - and the test suite - can
   // sweep the table without seeing its storage.
   static uint8_t propertyDefCount();

   PubSubClient& setServer(IPAddress ip, uint16_t port);
   PubSubClient& setServer(uint8_t * ip, uint16_t port);
   PubSubClient& setServer(const char * domain, uint16_t port);
   PubSubClient& setCallback(MQTT_CALLBACK_SIGNATURE);
   PubSubClient& setClient(Client& client);
   PubSubClient& setStream(Stream& stream);
   PubSubClient& setKeepAlive(uint16_t keepAlive);
   PubSubClient& setSocketTimeout(uint16_t timeout);
   // Session Expiry Interval used by a legacy `cleanSession == false` CONNECT. The
   // default is 0xFFFFFFFF (indefinite persistence); zero is preserved as an explicit
   // Clean Start 0 / immediate-expiry request rather than replaced by the default.
   PubSubClient& setSessionExpiryInterval(uint32_t seconds) {
      this->persistentSessionExpiryInterval = seconds;
      return *this;
   }
   uint32_t getSessionExpiryInterval() const {
      return this->persistentSessionExpiryInterval;
   }
   // Set the whole-packet receive deadline in seconds. This is independent of
   // per-byte socket waits once explicitly configured.
   PubSubClient& setPacketTimeout(uint16_t seconds);

   boolean setBufferSize(uint16_t size);
   uint16_t getBufferSize() const;

   // Set the maximum accepted inbound packet size (total wire bytes). 0 disables the
   // cap. When set, a larger declared packet closes the connection instead of being
   // drained/streamed. Useful to bound stream-mode input and tune DoS resistance.
   PubSubClient& setMaxIncomingPacketSize(uint32_t size) { this->maxIncomingPacketSize = size; return *this; }
   uint32_t getMaxIncomingPacketSize() const { return this->maxIncomingPacketSize; }

   // Effective wire protocol for this instance. It starts at MQTT 5.0 and returns MQTT
   // 3.1.1 after explicit unsupported-version fallback records runtime protocol level 4.
   uint8_t protocolVersion() const {
      return (this->sessionIdentity.protocolLevel == MQTT_VERSION_3_1_1)
                ? MQTT_VERSION_3_1_1 : MQTT_VERSION_5_0;
   }

   boolean connect(const char* id);
   boolean connect(const char* id, const char* user, const char* pass);
   boolean connect(const char* id, const char* willTopic, uint8_t willQos, boolean willRetain, const char* willMessage);
   boolean connect(const char* id, const char* user, const char* pass, const char* willTopic, uint8_t willQos, boolean willRetain, const char* willMessage);
   boolean connect(const char* id, const char* user, const char* pass, const char* willTopic, uint8_t willQos, boolean willRetain, const char* willMessage, boolean cleanSession);

   void disconnect(bool disconnect_package = false);

   boolean publish(const char* topic, const char* payload);
   boolean publish(const char* topic, const char* payload, boolean retained);
   boolean publish(const char* topic, const uint8_t * payload, unsigned int plength);
   boolean publish(const char* topic, const uint8_t * payload, unsigned int plength, boolean retained);

   // --- QoS-capable publish (Requirements 8.1, 8.2, 8.3, 8.5, 2.6) --------
   //
   // NEW members, added alongside the four preserved overloads above without touching
   // any of them (Requirement 2.6). The requested QoS is the DISTINGUISHING parameter,
   // so no existing call site becomes ambiguous: the 5-argument form has an arity the
   // preserved set does not, and the 4-argument form takes a `const char*` payload where
   // the only preserved 4-argument overload takes a `const uint8_t*` - and neither
   // pointer type converts implicitly to the other.
   //
   // `qos` is 0, 1 or 2; anything above 2 is refused with no byte transmitted
   // (Requirements 8.3, 8.18). Runtime level-4 fallback accepts 0 only - see
   // MqttPublishRequest for why.
   //
   // For QoS 1 and 2 a Packet Identifier is allocated from the SHARED allocator and an
   // Outstanding_Message entry is recorded BEFORE the first byte goes out, so an
   // acknowledgement can never arrive with nothing to correlate to (Requirement 8.4).
   // When no entry can be recorded the publish is refused with `lastError()` reporting
   // `MqttError::PacketIdUnavailable` and nothing is transmitted (Requirement 8.5).
   boolean publish(const char* topic, const uint8_t * payload, unsigned int plength, boolean retained, uint8_t qos);
   boolean publish(const char* topic, const char* payload, boolean retained, uint8_t qos);
   // The full form: QoS plus the MQTT 5.0 message property set plus the topic-alias
   // opt-in. Every other buffered publish overload funnels into these. The preserved
   // one-argument member remains available for member-function-pointer callers. In the
   // output form, `packetIdOut` is initialized to 0 and receives a QoS 1/2 Packet Identifier
   // only after the complete initial PUBLISH has been accepted by the transport.
   boolean publish(const MqttPublishRequest& request);
   boolean publish(const MqttPublishRequest& request, uint16_t* packetIdOut);

   boolean publish_P(const char* topic, const char* payload, boolean retained);
   boolean publish_P(const char* topic, const uint8_t * payload, unsigned int plength, boolean retained);
   // Start to publish a message.
   // This API:
   //   beginPublish(...)
   //   one or more calls to write(...)
   //   endPublish()
   // Allows for arbitrarily large payloads to be sent without them having to be copied into
   // a new buffer and held in memory at one time
   // Returns 1 if the message was started successfully, 0 if there was an error
   boolean beginPublish(const char* topic, unsigned int plength, boolean retained);
   // MQTT 5.0 QoS-0 streaming form. The properties are consumed while this call builds
   // the variable header; their borrowed storage need not survive subsequent write().
   boolean beginPublishWithProperties(const char* topic, unsigned int plength,
                                      boolean retained,
                                      const MqttMessageProperties& properties);
   // Finish off this publish message (started with beginPublish)
   // Returns 1 if the packet was sent successfully, 0 if there was an error
   int endPublish();
   // Write a single byte of payload (only to be used with beginPublish/endPublish)
   virtual size_t write(uint8_t);
   // Write size bytes from buffer into the payload (only to be used with beginPublish/endPublish)
   // Returns the number of bytes written
   virtual size_t write(const uint8_t *buffer, size_t size);
   boolean subscribe(const char* topic);
   // Requirements 9.8, 9.9: `qos` may be 0, 1 or 2 at runtime level 5 and 0 or 1
   // during level-4 fallback, which has no QoS 2. Anything above the active level's
   // maximum is rejected with no byte transmitted.
   boolean subscribe(const char* topic, uint8_t qos);
   // Subscribe with the full MQTT 5.0 Subscription Options byte. A default-constructed
   // `options` is identical to `subscribe(topic)`; see MqttSubscribeOptions for what
   // runtime level-4 fallback accepts. The output overload initializes `packetIdOut` to
   // 0 and sets it only after the complete SUBSCRIBE is written.
   boolean subscribe(const char* topic, const MqttSubscribeOptions& options);
   boolean subscribe(const char* topic, const MqttSubscribeOptions& options,
                     uint16_t* packetIdOut);
   boolean unsubscribe(const char* topic);
   // Berry/native bridges can request the sent Packet Identifier without overloading
   // unsubscribe(), whose exact member-function shape is part of the preserved API.
   boolean unsubscribeWithPacketId(const char* topic, uint16_t* packetIdOut);
   boolean loop();
   boolean connected();
   int state() const;

   // --- CONNACK outcome (Requirements 1.4, 1.5, 1.6) ----------------------
   // The Session Present flag the broker returned in CONNACK. False until a CONNACK
   // has been accepted, and cleared at the start of every connection attempt - never
   // on disconnect - so it stays readable for the whole life of the connection it
   // describes and beyond it (Requirement 1.6).
   //
   // Always declared. Runtime level-4 fallback does not latch Session Present, so this
   // remains false there.
   boolean sessionPresent() const { return this->sessionPresentFlag; }

   // The raw CONNACK Reason Code, kept alongside the legacy state() mapping so the
   // exact byte is available when several codes collapse onto one state() value.
   // 0 until a CONNACK has been accepted.
   uint8_t connackReasonCode() const { return this->connackRc; }

   // --- Negotiated limits (Requirements 5.1, 5.4, 5.7) --------------------
   // The limits in force for the current connection: the broker's declared value
   // where the CONNACK carried one, the MQTT 5.0 default otherwise. All four reset
   // at the start of every connection attempt.
   //
   // Always declared. Runtime level-4 fallback has no CONNACK Properties field to read
   // and never overwrites `keepAlive`, so all four read as the defaults there and
   // `getKeepAlive()` is simply what setKeepAlive() was given.
   //
   // The effective keepalive interval in seconds: the broker's Server Keep Alive
   // when it sent one, otherwise the requested value. 0 disables keepalive.
   uint16_t getKeepAlive() const { return this->keepAlive; }
   // Broker-declared Maximum Packet Size in bytes; 0 = no limit declared.
   uint32_t serverMaximumPacketSize() const { return this->serverMaxPacketSize; }
   // Broker-declared Topic Alias Maximum; 0 = no alias may be sent.
   uint16_t serverTopicAliasMaximum() const { return this->serverTopicAliasMax; }
   // Broker-declared Receive Maximum; 65535 when the property was absent.
   uint16_t getServerReceiveMaximum() const { return this->serverReceiveMaximum; }

   // --- Inbound message properties (Requirements 11.7, 12.4, 12.5, 12.6) --
   //
   // The MQTT 5.0 property set of the most recently parsed inbound PUBLISH. The
   // callback signature is untouched (Requirement 12.1): properties arrive through
   // this accessor instead of through extra callback parameters.
   //
   // LIFETIME: every string and binary member is a borrowed view into the working
   // buffer, valid from the moment the callback is invoked until the NEXT INBOUND
   // PACKET IS PARSED. Read them inside the callback, or copy what is needed out.
   //
   // `valid == false` means no PUBLISH has been parsed yet, and the set is empty
   // (Requirement 12.5). `valid == true` with every field absent means the PUBLISH
   // carried no properties (Requirement 12.6).
   //
   // Always declared; runtime level-4 fallback has no Properties field and never
   // populates it, so it always reads back as an empty, invalid set.
   const MqttInboundProperties& inboundProperties() const { return this->inboundProps; }

   // Enumerate every property of the most recently parsed inbound PUBLISH in wire order.
   // The views borrow the working buffer and are valid only for the visitor call. Returning
   // false from the visitor aborts the walk and makes this method return false.
   bool forEachInboundProperty(MqttPropertyVisitor visit, void* ctx = nullptr) const;

   // Enumerate the User Properties of the most recently parsed inbound PUBLISH, in
   // received order, by RE-WALKING the packet's property region in the working buffer.
   // Nothing is cached and nothing is copied, so no entry is ever truncated or dropped
   // however many the packet carried (Requirement 11.4).
   //
   // `visit` is called once per User Property with borrowed key/value views - same
   // lifetime rule as inboundProperties(). Returning false from the visitor aborts the
   // walk immediately.
   //
   // Returns true when the walk ran to completion, including the case of a PUBLISH with
   // no User Property at all. Returns false when the visitor aborted, when `visit` is
   // null, when no PUBLISH has been parsed (`valid == false`), or when the recorded
   // property region no longer lies within the working buffer - which is what a
   // setBufferSize() between the callback and the walk would produce. Always false during
   // runtime level-4 fallback, which has no property region to walk.
   bool forEachInboundUserProperty(MqttUserPropertyVisitor visit, void* ctx) const;

   // --- Subscription outcome (Requirements 9.4, 9.5, 9.6) -----------------
   //
   // subscribe() and unsubscribe() return as soon as the packet is written, so the
   // broker's per-topic Reason Codes arrive later, while loop() is consuming the
   // SUBACK or UNSUBACK. Register a callback to be told, or read the most recent
   // result.
   //
   // `ctx` is stored and handed back to the callback UNCHANGED, which is how a caller
   // reaches its instance state without a capturing lambda or a std::function. Pass a
   // null `cb` to unregister.
   //
   // Always declared. Runtime level-4 fallback does not dispatch SUBACK/UNSUBACK results,
   // so the callback never fires and lastSubscriptionResult() reads back its default there.
   PubSubClient& setSubscribeCallback(MqttSubscribeCallback cb, void* ctx = nullptr);

   // The most recent per-topic Reason Code reported by a SUBACK or UNSUBACK. Returned
   // BY VALUE, so the caller keeps the Reason Code and the flags; `topicFilter` inside
   // it stays a borrowed pointer with the lifetime documented on
   // MqttSubscriptionResult. Reset at the start of every connection attempt.
   MqttSubscriptionResult lastSubscriptionResult() const { return this->lastSubResult; }

   // --- Error diagnostics (Requirement 14.2) ------------------------------
   // What went wrong most recently. `None` until something fails. Cleared at the
   // start of every connection attempt.
   MqttError lastError() const { return this->errorState; }
   // Reason Code that accompanied the failure (0 when none applies).
   uint8_t lastReasonCode() const { return this->errorReasonCode; }
   // Packet Identifier the failure relates to (0 when none applies). Also the identifier
   // released by an abandoned outstanding operation (Requirement 8.6).
   uint16_t lastPacketId() const { return this->errorPacketId; }
   // Reason Code carried by a DISCONNECT received from the broker (0 when none).
   uint8_t serverDisconnectReasonCode() const { return this->disconnectReasonCode; }

   // --- In-flight accounting (Requirements 8.4, 8.5, 8.6) -----------------
   //
   // How many outbound operations currently hold a Packet Identifier and are waiting for
   // their acknowledgement - QoS 1/2 publishes, SUBSCRIBEs and UNSUBSCRIBEs alike. Bounded
   // by MQTT_MAX_OUTSTANDING; timeout diagnostics do not reduce this count.
   uint16_t inFlightCount() const;

   // --- Reason codes this client emits ------------------------------------
   // The DISCONNECT Reason Codes of the selection table. Named so call sites read
   // as intent rather than as magic bytes.
   static constexpr uint8_t MQTT_RC_NORMAL_DISCONNECTION  = 0x00;  // disconnect(), short form
   // Not sent on the wire: the locally synthesized failure code an ABANDONED outstanding
   // operation is reported with (Requirement 8.6). No broker answered it, so there is no
   // received Reason Code to expose, and 0x00 would read as success - so the generic
   // failure code of the specification's own table is used, in the 0x80..0xFF range every
   // other failure report uses.
   static constexpr uint8_t MQTT_RC_UNSPECIFIED_ERROR     = 0x80;  // outstanding entry abandoned
   static constexpr uint8_t MQTT_RC_MALFORMED_PACKET      = 0x81;  // unparseable wire structure
   static constexpr uint8_t MQTT_RC_PROTOCOL_ERROR        = 0x82;  // parseable but illegal
   static constexpr uint8_t MQTT_RC_BAD_AUTH_METHOD       = 0x8C;  // AUTH received
   static constexpr uint8_t MQTT_RC_RECEIVE_MAX_EXCEEDED  = 0x93;  // inbound QoS 2 registry full
   static constexpr uint8_t MQTT_RC_PACKET_TOO_LARGE      = 0x95;  // inbound packet over our cap

   // The reason-code selection table as a function: the code to send for a failure
   // that carries no more specific one. Failures whose code depends on the trigger
   // rather than on the category - `ProtocolError` is 0x82 for an illegal property but
   // 0x8C for an AUTH packet - pass their code to protocolFailure() explicitly.
   // Returns 0 for the values that never close a connection with a reason code.
   static uint8_t defaultReasonCode(MqttError error);

   // The single funnel for every protocol failure (Requirements 10.5, 10.6, 14.1, 14.2):
   // records `error` and `reasonCode` on the diagnostics, emits one `E0 01 <reasonCode>`
   // DISCONNECT when the socket is writable and nothing at all when it is not, closes the
   // connection, and clears the per-connection transient state so the driver's next
   // connect() can proceed normally. Public because it is the library's own error exit and
   // is exercised directly by the host test suite; drivers have no reason to call it.
   void protocolFailure(MqttError error, uint8_t reasonCode);
   // Same, using defaultReasonCode(error).
   void protocolFailure(MqttError error) { protocolFailure(error, defaultReasonCode(error)); }
};

#endif  // TASMOTA_PUBSUBCLIENT_5_H