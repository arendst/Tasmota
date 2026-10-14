# TasmotaPubSub

TasmotaPubSub is Tasmota's hardened fork of Nick O'Leary's
[PubSubClient](https://github.com/knolleary/pubsubclient), for ESP8266 and ESP32.
It preserves the `PubSubClient` class and established method signatures while adding a bounded
MQTT 5 profile, MQTT 3.1.1 compatibility, stricter packet validation, QoS state tracking, and
Tasmota command request/response support. Instances own buffers and replay state and therefore
cannot be copied.

The local `mqtt-v5.0.md` file is the protocol reference. This README describes the profile
implemented by this library; it does not claim support for every optional MQTT 5 feature.

## Protocol selection and compatibility

`PubSubClient.h` is a small compile-time facade. It imports the Tasmota configuration,
defines the two accepted `MQTT_VERSION` values, and includes exactly one implementation
header. When `MQTT_VERSION` is not defined, the library defaults to MQTT 3.1.1.

The selector must be defined project-wide because the sketch/application and both library
sources are separate translation units. For PlatformIO, use a build flag:

```ini
build_flags =
  -DMQTT_VERSION=MQTT_VERSION_5_0
```

Tasmota may instead define `MQTT_VERSION` in `user_config_override.h`, which is imported
through `tasmota_options.h`. Use `MQTT_VERSION_3_1_1` for an explicit level-4-only build.

The implementation files are split by profile:

- `src/PubSubClient_311.h` and `src/PubSubClient_311.cpp` contain the restored compact
  MQTT 3.1.1 implementation.
- `src/PubSubClient_5.h` and `src/PubSubClient_5.cpp` contain the MQTT 5 implementation.

Arduino and PlatformIO compile both `.cpp` files. Each source has a whole-file selector guard,
so only the selected implementation contributes definitions. Only the two values above are
accepted when `MQTT_VERSION` is defined.

An MQTT5-capable `PubSubClient` instance starts at protocol level 5. It permanently selects
level 4 for that instance only after explicit unsupported-version evidence:

- a valid MQTT 5 CONNACK with Reason Code `0x84`; or
- the exact MQTT 3.1.1 refusal `20 02 00 01`.

Transport failures, timeouts, malformed packets, and other refusal codes do not cause a
downgrade. The fresh level-4 attempt uses a new transport connection, and incompatible
MQTT5 session state is discarded before it. `protocolVersion()` reports the effective level.
A dedicated MQTT 3.1.1 build contains no fallback classifier or runtime selection path.

The dedicated MQTT 3.1.1 profile intentionally stays close to the restored implementation,
including its compact API, one-to-four-byte Remaining Length support, and established CONNECT
credential encoding. MQTT 5-only APIs and state are confined to the MQTT 5 profile.

## Implemented MQTT 5 profile

### Framing and validation

- MQTT Variable Byte Integers are deliberately limited to two bytes (`0..16,383`). A packet
  outside that bound is refused before transmission or rejected without an out-of-bounds read.
- Packet and Properties lengths are checked against bytes actually received.
- Fixed-header flags, packet direction/state, Packet Identifiers, packet-specific Reason Codes,
  and property legality/multiplicity are validated before callbacks or state mutation.
- CONNECT and CONNACK handling is transactional: no partial packet or partially validated
  negotiated/session state is committed.
- Partial transport writes are terminal because the MQTT byte stream can no longer be trusted.
- A protocol error sends an MQTT5 DISCONNECT Reason Code when the socket is still writable,
  then closes the transport. Transport failures close silently.
- `disconnect()` aborts the transport by default; `disconnect(true)` sends the normal short
  DISCONNECT packet first.

### Broker limits and connection state

The client applies Server Keep Alive and Receive Maximum, and uses Topic Alias Maximum for
opt-in outbound aliases. Broker Maximum Packet Size is enforced for PUBLISH, subscription,
acknowledgement, and replay packets, but not currently for PINGREQ or DISCONNECT. Negotiated
values are per connection and reset at connection boundaries. CONNACK Maximum QoS, Retain
Available, Wildcard Subscription Available, Subscription Identifier Available, and Shared
Subscription Available values are range-validated but not retained or enforced.

Useful diagnostics include `sessionPresent()`, `connackReasonCode()`, `lastError()`,
`lastReasonCode()`, `lastPacketId()`, and `serverDisconnectReasonCode()`. CONNACK outcome and
MQTT5 properties are meaningful only while the effective protocol is MQTT 5; the level-4-only
and runtime-fallback paths do not latch level-4 CONNACK details.

Define `USE_MQTT_DETAILED_LOGGING` in a Tasmota MQTT5 build to trace outbound PUBLISH packets
and inbound messages at `DEBUG_MORE`, with JSON-safe topic, payload, and property fields. Each
PUBLISH is logged as `MQT: Tx(<qos>) <topic> | <payload> | <properties>`, with the QoS level
always shown in parentheses so the line parses uniformly for every QoS. Inbound messages are
logged as `MQT: Rx <topic> | <payload> | <properties>`. Only PUBLISH is traced (not
PINGREQ/SUBSCRIBE/ACK/DISCONNECT). Both directions are suppressed after an instance falls back
to MQTT 3.1.1, and all related logging code is compile-time excluded when the switch is absent.

A persistent connection (`cleanSession == false`) uses an MQTT5 Session Expiry Interval,
configurable with `setSessionExpiryInterval()` and defaulting to `0xFFFFFFFF`. Local resumable
state is bound to endpoint, port, Client Identifier, effective protocol, and session policy;
it is never replayed to a different identity. Session Present is accepted only when compatible
local state exists. Eligible unacknowledged PUBLISH/PUBREL state is replayed in deterministic
order, and connection establishment fails on the first replay framing or transport failure.

### PUBLISH, QoS, and Packet Identifiers

Outbound QoS 1/2 acknowledged delivery is an opt-in feature, compiled in only when
`USE_MQTT_QOS` is defined. It is off by default because Tasmota publishes at QoS 0 in
practice, and leaving it out removes the replay buffers, retransmission, PUBACK/PUBREC/PUBCOMP
handling, and the Receive Maximum back-pressure gate for a smaller flash and RAM footprint.
Without `USE_MQTT_QOS`, a publish requested at QoS 1 or 2 is refused before framing (returns
false), exactly as the MQTT 3.1.1 fallback refuses it. Everything below in this subsection
applies only when `USE_MQTT_QOS` is compiled in; QoS 0 publishing, subscriptions, PUBLISH
properties, and inbound QoS 0/1/2 delivery are unaffected by the switch.

The established publishing methods remain QoS 0. With `USE_MQTT_QOS`, MQTT5-capable code can
additionally use:

```cpp
MqttPublishRequest request;
request.topic = "tele/device/STATE";
request.payload = payload;
request.plength = payload_len;
request.qos = 1;                 // 0, 1, or 2 in effective MQTT 5 mode
request.retained = false;
request.properties.contentType = "application/json";
MqttClient.publish(request);
```

QoS 1/2 PUBLISH, SUBSCRIBE, and UNSUBSCRIBE share one per-client Packet Identifier namespace.
Acknowledgements must match both the identifier and expected operation state. A local ACK
timeout is diagnostic only: it does not release the identifier or operation.

Buffered QoS 1/2 publishing reserves the Packet Identifier, outstanding slot, and complete
replay body before the first wire byte. The body must fit within `MQTT_INFLIGHT_COPY_BUDGET`
(default 2048 bytes); failure refuses the publish without leaking those resources. This atomic
guarantee does not include optional topic-alias table updates, so aliases remain disabled by
default. Streaming remains QoS 0 because replayable bytes do not exist after they are written.

When broker Receive Maximum credit is exhausted, buffered QoS 1/2 `publish()` may wait up to
`MQTT_ACK_TIMEOUT` while calling `loop()`. That can dispatch inbound callbacks reentrantly; a
zero timeout refuses immediately.

`beginPublishWithProperties()` is the MQTT5-only QoS-0 streaming form. It consumes property
pointers while assembling the header; payload bytes can then be sent with the usual
`write()`/`endPublish()` sequence without fitting the complete payload in the working buffer.
Only one streaming publish may be active, and exactly the declared payload length must be
written; a short or excess stream is terminal because it desynchronizes the connection.

### PUBLISH properties

`MqttMessageProperties` supports Payload Format Indicator, Content Type, Response Topic,
Correlation Data, User Properties, and opt-in outbound Topic Alias use. Response Topic and
Correlation Data are independent: a request can carry only a Response Topic, while a response
normally publishes to that topic and carries only the copied Correlation Data.

Inbound properties are available inside the message callback through `inboundProperties()`,
`forEachInboundProperty()`, and `forEachInboundUserProperty()`. String and User Property views
borrow the client's single working buffer and are valid only until the next inbound packet is
parsed. Copy values that must survive the callback. Inbound Correlation Data uses fixed
client-owned storage, but that storage is reused by the next inbound PUBLISH.

Inbound Correlation Data is limited to 16 bytes. A larger value is a protocol error and is
never truncated. Outbound Correlation Data has no special 16-byte limit; normal buffer,
broker Maximum Packet Size, and Remaining Length limits apply.

Topic Alias support is outbound-only; inbound alias-only PUBLISH reuse is not implemented.
Outbound aliases are disabled by default (`MQTT_MAX_TOPIC_ALIASES == 0`). When enabled, the
table is bounded by that compile-time count and the broker's Topic Alias Maximum. Alias state is
committed during header assembly, before QoS replay retention, so do not enable aliases for
QoS 1/2 without accounting for a later retention failure.

The legacy inbound `setStream()` path is not MQTT5 property-aware: it begins streaming after the
Topic Name and Packet Identifier, before MQTT5 Properties are parsed. Do not use inbound stream
mode with effective MQTT 5; use the bounded callback path instead.

### Deliberately unsupported features

The selected profile does not implement AUTH or Enhanced Authentication. Authentication Method
and Authentication Data are rejected as protocol errors. Subscription Identifier and Message
Expiry properties can be parsed through `forEachInboundProperty()` and logged by Tasmota, but
they are not configured outbound or acted upon. Will Delay is parsed only as a legal property;
it has no application behavior. New semantics must preserve bounded memory, transactional
framing, and the dedicated MQTT 3.1.1 build.

## Tasmota integration

The core integration is in `tasmota/tasmota_xdrv_driver/xdrv_02_9_mqtt.ino` and is compiled only
when `MQTT_VERSION == MQTT_VERSION_5_0`.

For an inbound MQTT5 command carrying Response Topic:

1. `MqttDataHandler` validates the Response Topic (non-empty, no embedded NUL or `+`/`#`,
   shorter than `TOPSZ`) and copies it into an owned `String`. Propagation activates only if
   allocation and the complete copy succeed. Optional Correlation Data is copied into 16-byte
   storage.
2. A scoped `MqttCommandContext` pointer is attached to `XdrvMailbox` across
   `FUNC_MQTT_DATA`, `CommandHandler`, native command dispatch, and synchronous nested commands.
3. The scope saves/restores the previous pointer, so early returns and nested MQTT callbacks do
   not leak one request into another. Backlog, timers, and other later asynchronous work do not
   inherit the context.
4. Each publication routed through the standard `MqttPublishLib` funnel keeps its normal Tasmota
   publication unchanged and attempts a separate, non-retained PUBLISH to the Response Topic.
   This remains two publications even if both topic strings are equal, because the normal state
   publication may be retained while a request response must not be. Direct Azure-method and AWS
   shadow `MqttClient.publish()` calls bypass this behavior. Correlated delivery is best effort:
   failure is logged, while the wrapper returns the normal publication's result.
5. The response copies Correlation Data when present and does not repeat Response Topic as a
   property. Correlation Data without a valid Response Topic does not activate propagation.

This preserves existing `stat/...` state/result behavior while enabling MQTT5 request/response,
including multipart commands. No payload type is inferred: callers should add Payload Format or
Content Type only when they can guarantee those semantics. Arbitrary inbound User Properties are
not echoed automatically.

### Berry API

The Berry `mqtt` module is the scripting front-end to this integration:

- `mqtt.publish(topic, payload [, retain, start, len, loglevel])` — QoS 0 publish (also exposed
  as `tasmota.publish`). Runs the local rule engine on string payloads.
- `mqtt.subscribe(topic, closure)` — subscribe at QoS 0 and route matching messages to
  `closure(topic, idx, payload_str, payload_bytes)`. Registered topics are re-subscribed
  automatically on reconnect.
- `mqtt.unsubscribe(topic)` — unsubscribe from `topic`, or from every registered topic when
  `topic` is `nil`.
- `mqtt.connected() -> bool` and `mqtt.protocol() -> int` (effective level, `5` or `4`).

MQTT 5 request/response, used inside a subscription closure while a request is being handled:

- `mqtt.is_request() -> bool` — true when the inbound message carried a Response Topic, i.e. it
  expects a reply. Always false on an MQTT 3.1.1 broker.
- `mqtt.respond(payload) -> bool` — reply to that request. Publishes `payload` to the request's
  Response Topic, echoing its Correlation Data, QoS 0 and non-retained. The topic and correlation
  are taken from the inbound message; the caller supplies only the payload. Returns `true` if a
  response was sent, `false` when there is no active request (see `mqtt.is_request()`), on MQTT
  3.1.1, or on a bad payload argument.
- `mqtt.metadata() -> map` — inbound MQTT 5 properties of the message being handled: `qos`,
  `retain`, and, when present, `response_topic` and `correlation_data`.
- `mqtt.last_error() -> map` — MQTT 5 diagnostics: `error`, `reason_code`, `packet_id`,
  `server_disconnect_reason`.

```berry
def ping(topic, idx, payload_s, payload_b)
  if mqtt.is_request()
    mqtt.respond("pong")     # replies to the request's Response Topic, echoes Correlation Data
  end
end
mqtt.subscribe("device/ping", ping)
```

`mqtt.respond()` reuses the same scoped request context as the automatic command response above,
but is an explicit, standalone reply for Berry subscription handlers. It publishes through the
non-recursing single-publish, so it does not itself re-trigger the automatic correlated response.
It is intentionally QoS 0 and never retained: a reply is a one-shot correlated message that must
not be stored by the broker for later subscribers of the Response Topic. The outbound Berry
property surface is limited to this request/response use; general User Properties, Content Type,
and Payload Format Indicator are not exposed to Berry even though the library supports them on the
wire.

## Resource model and concurrency

Important defaults are a 1200-byte working packet buffer and, for subscription tracking, twelve
tracked-operation entries (`MQTT_MAX_PENDING_SUBSCRIPTIONS`). When `USE_MQTT_QOS` is compiled
in, the tracked-operation table grows by `MQTT_MAX_INFLIGHT` (four) publish slots to sixteen
entries, each slot gains the Retained_Packet_Body replay fields, and the client adds a
2048-byte replay-copy budget; subscriptions and publishes then share that combined table, so
either can reduce capacity available to the other. Eight inbound QoS2 identifiers exist
regardless, because inbound delivery is independent of the outbound QoS switch. Owned library
strings use Arduino `String`; library sources avoid Standard Library string/container types.

Connection, buffer, callback, protocol, QoS, and session state are per `PubSubClient` instance.
Multiple brokers are supported by using one instance and one underlying `Client`/socket per
broker and servicing every instance's `loop()`. This is cooperative multi-instance support, not
thread safety. Do not call one instance concurrently; on ESP8266, MQTT5 property lookup also uses
a shared non-reentrant scratch row, so externally serialize different instances as well.

## Host validation

The sanitizer-enabled doctest harness is under `tests/`:

```sh
cd tests
make check       # authoritative selector, source scan, MQTT 3.1.1, MQTT5, and no-QoS gate
make baseline    # frozen MQTT 3.1.1 behavior
make mqtt5       # mandatory MQTT5 suite (USE_MQTT_QOS enabled)
make noqos       # MQTT 5.0 built without USE_MQTT_QOS (QoS-0-only production config)
make test        # full run of both protocol binaries
```

See `tests/README.md` for individual targets and sanitizer controls. Protocol changes should pass
`make check` and representative ESP8266/ESP32 firmware builds.

## License

Released under the MIT License. See `LICENSE.txt`.
