/*
  api_shape_test.cpp - Compile-time lock on the preserved public API signatures
  of PubSubClient (task 1.3).

  Feature: tasmota-pubsub-mqtt5

  Requirement 2.1 says the migration must preserve every Public_API member used
  by the Tasmota driver "without changing name, parameter types, parameter order,
  or return type". Requirement 12.1 says the callback signature
  `void(char* topic, uint8_t* payload, unsigned int length)` must stay exactly as
  it is. Both are compile-time contracts, so this file enforces them at compile
  time rather than at run time:

    - Every preserved public member is addressed through an EXPLICITLY TYPED
      pointer-to-member. The declared pointer type has to match the member's
      declaration exactly - an implicit conversion cannot paper over a changed
      parameter type, a changed parameter order or a changed return type - so any
      future signature drift becomes a compile error in this translation unit.
    - Members that are not overloaded get an additional
      `static_assert(std::is_same<decltype(&PubSubClient::m), ...>)`, which pins
      the type from the other direction as well.
    - The callback contract is locked by deriving the exact declared type of the
      library's callback member from MQTT_CALLBACK_SIGNATURE itself and binding a
      function literally shaped `void(char*, uint8_t*, unsigned int)` to it.

  This file is deliberately NOT wrapped in a TEST_SUITE. Its assertions are
  compile-time, and it must therefore be compiled in BOTH selections (v311 and
  v5). Tagging it `baseline` would exclude it from the v5 binary
  (`make test` runs the v5 binary with -tse=baseline).

  Scope note: this file locks SHAPE only. Observable behavior of the same members
  is characterized by api_surface_test.cpp and the other baseline suites; nothing
  here duplicates that.

  The overload set below is transcribed verbatim from ../src/PubSubClient.h. It is
  the set that actually exists: 5 `connect` overloads (id; id+user+pass;
  id+will...; id+user+pass+will...; and the fully explicit one with
  cleanSession), not 8.
*/

#include <cstddef>
#include <cstdint>
#include <functional>
#include <type_traits>

#include "doctest.h"

#include "Client.h"
#include "IPAddress.h"
#include "Stream.h"
#include "PubSubClient.h"

// A named namespace (not anonymous) so the pointer objects keep external
// linkage where possible and the names cannot collide with any other test TU.
namespace api_shape {

using PSC = PubSubClient;

// --- Requirement 12.1: the callback contract ------------------------------
//
// MQTT_CALLBACK_SIGNATURE is a *declaration* (type + the name `callback`), so the
// only faithful way to recover the type is to declare the member exactly as the
// library does and read its type back. If the library ever adds, removes or
// reorders a callback parameter, the static_assert below fails.
struct CallbackTypeProbe {
    MQTT_CALLBACK_SIGNATURE;
};
using MqttCallbackType = decltype(CallbackTypeProbe::callback);

#if defined(ESP8266) || defined(ESP32)
static_assert(std::is_same<MqttCallbackType,
                           std::function<void(char*, uint8_t*, unsigned int)>>::value,
              "MQTT_CALLBACK_SIGNATURE must stay std::function<void(char*, uint8_t*, unsigned int)>");
#else
static_assert(std::is_same<MqttCallbackType,
                           void (*)(char*, uint8_t*, unsigned int)>::value,
              "MQTT_CALLBACK_SIGNATURE must stay void (*)(char*, uint8_t*, unsigned int)");
#endif

// A function literally shaped like the contract, and an object of the library's
// own callback type bound to it. Both the shape of the function and its
// assignability to MQTT_CALLBACK_SIGNATURE are compile-checked.
void callbackContractShape(char* topic, uint8_t* payload, unsigned int length) {
    (void)topic;
    (void)payload;
    (void)length;
}
static_assert(std::is_same<decltype(callbackContractShape),
                           void(char*, uint8_t*, unsigned int)>::value,
              "the callback contract is void(char*, uint8_t*, unsigned int)");

// Declares `api_shape::callback` with the library's exact callback type.
MQTT_CALLBACK_SIGNATURE = &callbackContractShape;

// --- Requirement 2.1: connect (5 declared overloads) ----------------------

constexpr boolean (PSC::*kConnectId)(const char*) = &PSC::connect;
constexpr boolean (PSC::*kConnectIdUserPass)(const char*, const char*, const char*) =
    &PSC::connect;
constexpr boolean (PSC::*kConnectIdWill)(const char*, const char*, uint8_t, boolean,
                                         const char*) = &PSC::connect;
constexpr boolean (PSC::*kConnectIdUserPassWill)(const char*, const char*, const char*,
                                                 const char*, uint8_t, boolean,
                                                 const char*) = &PSC::connect;
// The overload xdrv_02_9_mqtt.ino calls: ... + cleanSession.
constexpr boolean (PSC::*kConnectIdUserPassWillClean)(const char*, const char*, const char*,
                                                      const char*, uint8_t, boolean,
                                                      const char*, boolean) = &PSC::connect;

// --- Requirement 2.1: lifecycle ------------------------------------------

constexpr void (PSC::*kDisconnect)(bool) = &PSC::disconnect;
constexpr boolean (PSC::*kLoop)() = &PSC::loop;
constexpr boolean (PSC::*kConnected)() = &PSC::connected;
constexpr int (PSC::*kState)() const = &PSC::state;

static_assert(std::is_same<decltype(&PSC::disconnect), void (PSC::*)(bool)>::value,
              "disconnect must stay void(bool)");
static_assert(std::is_same<decltype(&PSC::loop), boolean (PSC::*)()>::value,
              "loop must stay boolean()");
static_assert(std::is_same<decltype(&PSC::connected), boolean (PSC::*)()>::value,
              "connected must stay boolean()");
static_assert(std::is_same<decltype(&PSC::state), int (PSC::*)() const>::value,
              "state must stay int() const");
static_assert(std::is_same<decltype(&PSC::protocolVersion),
                           uint8_t (PSC::*)() const>::value,
              "protocolVersion must stay uint8_t() const");

// --- Requirement 2.1: publish / publish_P --------------------------------

constexpr boolean (PSC::*kPublishStr)(const char*, const char*) = &PSC::publish;
constexpr boolean (PSC::*kPublishStrRetained)(const char*, const char*, boolean) =
    &PSC::publish;
constexpr boolean (PSC::*kPublishBytes)(const char*, const uint8_t*, unsigned int) =
    &PSC::publish;
constexpr boolean (PSC::*kPublishBytesRetained)(const char*, const uint8_t*, unsigned int,
                                                boolean) = &PSC::publish;

constexpr boolean (PSC::*kPublishPStr)(const char*, const char*, boolean) = &PSC::publish_P;
constexpr boolean (PSC::*kPublishPBytes)(const char*, const uint8_t*, unsigned int,
                                         boolean) = &PSC::publish_P;

// --- Requirement 2.1: streaming publish (beginPublish/write/endPublish) ---

constexpr boolean (PSC::*kBeginPublish)(const char*, unsigned int, boolean) =
    &PSC::beginPublish;
constexpr int (PSC::*kEndPublish)() = &PSC::endPublish;
constexpr size_t (PSC::*kWriteByte)(uint8_t) = &PSC::write;
constexpr size_t (PSC::*kWriteBuffer)(const uint8_t*, size_t) = &PSC::write;

static_assert(std::is_same<decltype(&PSC::beginPublish),
                           boolean (PSC::*)(const char*, unsigned int, boolean)>::value,
              "beginPublish must stay boolean(const char*, unsigned int, boolean)");
static_assert(std::is_same<decltype(&PSC::endPublish), int (PSC::*)()>::value,
              "endPublish must stay int()");

// --- Requirement 2.1: subscribe / unsubscribe ----------------------------

constexpr boolean (PSC::*kSubscribe)(const char*) = &PSC::subscribe;
constexpr boolean (PSC::*kSubscribeQos)(const char*, uint8_t) = &PSC::subscribe;
constexpr boolean (PSC::*kUnsubscribe)(const char*) = &PSC::unsubscribe;

static_assert(std::is_same<decltype(&PSC::unsubscribe), boolean (PSC::*)(const char*)>::value,
              "unsubscribe must stay boolean(const char*)");

// --- Requirement 2.1: buffer sizing --------------------------------------

constexpr boolean (PSC::*kSetBufferSize)(uint16_t) = &PSC::setBufferSize;
constexpr uint16_t (PSC::*kGetBufferSize)() const = &PSC::getBufferSize;
constexpr PSC& (PSC::*kSetMaxIncomingPacketSize)(uint32_t) = &PSC::setMaxIncomingPacketSize;
constexpr uint32_t (PSC::*kGetMaxIncomingPacketSize)() const = &PSC::getMaxIncomingPacketSize;

static_assert(std::is_same<decltype(&PSC::setBufferSize), boolean (PSC::*)(uint16_t)>::value,
              "setBufferSize must stay boolean(uint16_t)");
static_assert(std::is_same<decltype(&PSC::getBufferSize), uint16_t (PSC::*)() const>::value,
              "getBufferSize must stay uint16_t() const");
static_assert(std::is_same<decltype(&PSC::setMaxIncomingPacketSize),
                           PSC& (PSC::*)(uint32_t)>::value,
              "setMaxIncomingPacketSize must stay PubSubClient&(uint32_t)");
static_assert(std::is_same<decltype(&PSC::getMaxIncomingPacketSize),
                           uint32_t (PSC::*)() const>::value,
              "getMaxIncomingPacketSize must stay uint32_t() const");

// --- Requirement 2.1: configuration setters ------------------------------

constexpr PSC& (PSC::*kSetServerIp)(IPAddress, uint16_t) = &PSC::setServer;
constexpr PSC& (PSC::*kSetServerBytes)(uint8_t*, uint16_t) = &PSC::setServer;
constexpr PSC& (PSC::*kSetServerDomain)(const char*, uint16_t) = &PSC::setServer;
constexpr PSC& (PSC::*kSetCallback)(MqttCallbackType) = &PSC::setCallback;
constexpr PSC& (PSC::*kSetClient)(Client&) = &PSC::setClient;
constexpr PSC& (PSC::*kSetStream)(Stream&) = &PSC::setStream;
constexpr PSC& (PSC::*kSetKeepAlive)(uint16_t) = &PSC::setKeepAlive;
constexpr PSC& (PSC::*kSetSocketTimeout)(uint16_t) = &PSC::setSocketTimeout;

static_assert(std::is_same<decltype(&PSC::setCallback), PSC& (PSC::*)(MqttCallbackType)>::value,
              "setCallback must keep taking exactly MQTT_CALLBACK_SIGNATURE");
static_assert(std::is_same<decltype(&PSC::setClient), PSC& (PSC::*)(Client&)>::value,
              "setClient must stay PubSubClient&(Client&)");
static_assert(std::is_same<decltype(&PSC::setStream), PSC& (PSC::*)(Stream&)>::value,
              "setStream must stay PubSubClient&(Stream&)");
static_assert(std::is_same<decltype(&PSC::setKeepAlive), PSC& (PSC::*)(uint16_t)>::value,
              "setKeepAlive must stay PubSubClient&(uint16_t)");
static_assert(std::is_same<decltype(&PSC::setSocketTimeout), PSC& (PSC::*)(uint16_t)>::value,
              "setSocketTimeout must stay PubSubClient&(uint16_t)");

// --- Requirement 2.6 / 8.2 / 12.1: MQTT 5 additions ----------------------
//
// These members are additive: they must remain available in both protocol
// selections without weakening any of the preserved signatures locked above.
// Each pointer is explicitly typed so a return, parameter, constness, or
// callback-shape change is a compile error.
constexpr boolean (PSC::*kPublishBytesQos)(const char*, const uint8_t*, unsigned int,
                                           boolean, uint8_t) = &PSC::publish;
constexpr boolean (PSC::*kPublishStrQos)(const char*, const char*, boolean, uint8_t) =
    &PSC::publish;
constexpr boolean (PSC::*kPublishRequest)(const MqttPublishRequest&) = &PSC::publish;
constexpr boolean (PSC::*kSubscribeOptions)(const char*, const MqttSubscribeOptions&) =
    &PSC::subscribe;

constexpr boolean (PSC::*kSessionPresent)() const = &PSC::sessionPresent;
constexpr uint8_t (PSC::*kConnackReasonCode)() const = &PSC::connackReasonCode;
constexpr uint8_t (PSC::*kServerDisconnectReasonCode)() const =
    &PSC::serverDisconnectReasonCode;
constexpr MqttError (PSC::*kLastError)() const = &PSC::lastError;
constexpr uint8_t (PSC::*kLastReasonCode)() const = &PSC::lastReasonCode;
constexpr uint16_t (PSC::*kLastPacketId)() const = &PSC::lastPacketId;
constexpr uint16_t (PSC::*kInFlightCount)() const = &PSC::inFlightCount;
constexpr const MqttInboundProperties& (PSC::*kInboundProperties)() const =
    &PSC::inboundProperties;

// These two callbacks deliberately remain plain function pointers plus an
// opaque context: use explicit probes to prevent a regression to std::function.
bool userPropertyVisitorShape(MqttStringView key, MqttStringView value, void* ctx) {
    (void)key;
    (void)value;
    (void)ctx;
    return true;
}
void subscribeCallbackShape(const MqttSubscriptionResult& result, void* ctx) {
    (void)result;
    (void)ctx;
}
static_assert(std::is_same<MqttUserPropertyVisitor,
                           bool (*)(MqttStringView, MqttStringView, void*)>::value,
              "MqttUserPropertyVisitor must stay a plain function pointer");
static_assert(std::is_same<MqttSubscribeCallback,
                           void (*)(const MqttSubscriptionResult&, void*)>::value,
              "MqttSubscribeCallback must stay a plain function pointer");
static_assert(std::is_same<decltype(&userPropertyVisitorShape),
                           MqttUserPropertyVisitor>::value,
              "user-property visitor shape must match MqttUserPropertyVisitor");
static_assert(std::is_same<decltype(&subscribeCallbackShape),
                           MqttSubscribeCallback>::value,
              "subscription callback shape must match MqttSubscribeCallback");

constexpr bool (PSC::*kForEachInboundUserProperty)(MqttUserPropertyVisitor, void*) const =
    &PSC::forEachInboundUserProperty;
constexpr PSC& (PSC::*kSetSubscribeCallback)(MqttSubscribeCallback, void*) =
    &PSC::setSubscribeCallback;
constexpr MqttSubscriptionResult (PSC::*kLastSubscriptionResult)() const =
    &PSC::lastSubscriptionResult;
constexpr PSC& (PSC::*kSetPacketTimeout)(uint16_t) = &PSC::setPacketTimeout;
constexpr PSC& (PSC::*kSetSessionExpiryInterval)(uint32_t) =
    &PSC::setSessionExpiryInterval;
constexpr uint32_t (PSC::*kGetSessionExpiryInterval)() const =
    &PSC::getSessionExpiryInterval;

constexpr uint8_t (*kEncodeVariableByteInteger)(uint32_t, uint8_t*) =
    &PSC::encodeVariableByteInteger;
constexpr uint8_t (*kVariableByteIntegerSize)(uint32_t) = &PSC::variableByteIntegerSize;
constexpr bool (*kDecodeVariableByteInteger)(const uint8_t*, uint32_t, uint32_t&, uint8_t&) =
    &PSC::decodeVariableByteInteger;

static_assert(std::is_same<decltype(&PSC::forEachInboundUserProperty),
                           bool (PSC::*)(MqttUserPropertyVisitor, void*) const>::value,
              "forEachInboundUserProperty must take a visitor and context pointer");
static_assert(std::is_same<decltype(&PSC::setSubscribeCallback),
                           PSC& (PSC::*)(MqttSubscribeCallback, void*)>::value,
              "setSubscribeCallback must take a callback and context pointer");
static_assert(std::is_same<decltype(&PSC::setPacketTimeout), PSC& (PSC::*)(uint16_t)>::value,
              "setPacketTimeout must stay PubSubClient&(uint16_t)");
static_assert(std::is_same<decltype(&PSC::setSessionExpiryInterval),
                           PSC& (PSC::*)(uint32_t)>::value,
              "setSessionExpiryInterval must stay PubSubClient&(uint32_t)");
static_assert(std::is_same<decltype(&PSC::getSessionExpiryInterval),
                           uint32_t (PSC::*)() const>::value,
              "getSessionExpiryInterval must stay uint32_t() const");
static_assert(std::is_same<decltype(&PSC::encodeVariableByteInteger),
                           uint8_t (*)(uint32_t, uint8_t*)>::value,
              "encodeVariableByteInteger signature changed");
static_assert(std::is_same<decltype(&PSC::variableByteIntegerSize),
                           uint8_t (*)(uint32_t)>::value,
              "variableByteIntegerSize signature changed");
static_assert(std::is_same<decltype(&PSC::decodeVariableByteInteger),
                           bool (*)(const uint8_t*, uint32_t, uint32_t&, uint8_t&)>::value,
              "decodeVariableByteInteger signature changed");

}  // namespace api_shape

// Unguarded (no TEST_SUITE) so it is compiled and executed by BOTH selections.
// The checks are trivially true at run time - the real work happened at compile
// time - but touching every pointer keeps -Wall -Wextra quiet about the
// internal-linkage constexpr objects and makes the lock visible in the report.
TEST_CASE("preserved public API signatures are locked at their declared types") {
    using namespace api_shape;

    SUBCASE("connect overloads") {
        CHECK(kConnectId != nullptr);
        CHECK(kConnectIdUserPass != nullptr);
        CHECK(kConnectIdWill != nullptr);
        CHECK(kConnectIdUserPassWill != nullptr);
        CHECK(kConnectIdUserPassWillClean != nullptr);
    }

    SUBCASE("lifecycle") {
        CHECK(kDisconnect != nullptr);
        CHECK(kLoop != nullptr);
        CHECK(kConnected != nullptr);
        CHECK(kState != nullptr);
    }

    SUBCASE("publish and publish_P overloads") {
        CHECK(kPublishStr != nullptr);
        CHECK(kPublishStrRetained != nullptr);
        CHECK(kPublishBytes != nullptr);
        CHECK(kPublishBytesRetained != nullptr);
        CHECK(kPublishPStr != nullptr);
        CHECK(kPublishPBytes != nullptr);
    }

    SUBCASE("streaming publish") {
        CHECK(kBeginPublish != nullptr);
        CHECK(kEndPublish != nullptr);
        CHECK(kWriteByte != nullptr);
        CHECK(kWriteBuffer != nullptr);
    }

    SUBCASE("subscribe and unsubscribe") {
        CHECK(kSubscribe != nullptr);
        CHECK(kSubscribeQos != nullptr);
        CHECK(kUnsubscribe != nullptr);
    }

    SUBCASE("buffer sizing") {
        CHECK(kSetBufferSize != nullptr);
        CHECK(kGetBufferSize != nullptr);
        CHECK(kSetMaxIncomingPacketSize != nullptr);
        CHECK(kGetMaxIncomingPacketSize != nullptr);
    }

    SUBCASE("configuration setters") {
        CHECK(kSetServerIp != nullptr);
        CHECK(kSetServerBytes != nullptr);
        CHECK(kSetServerDomain != nullptr);
        CHECK(kSetCallback != nullptr);
        CHECK(kSetClient != nullptr);
        CHECK(kSetStream != nullptr);
        CHECK(kSetKeepAlive != nullptr);
        CHECK(kSetSocketTimeout != nullptr);
    }

    SUBCASE("MQTT 5 additive API") {
        CHECK(kPublishBytesQos != nullptr);
        CHECK(kPublishStrQos != nullptr);
        CHECK(kPublishRequest != nullptr);
        CHECK(kSubscribeOptions != nullptr);
        CHECK(kSessionPresent != nullptr);
        CHECK(kConnackReasonCode != nullptr);
        CHECK(kServerDisconnectReasonCode != nullptr);
        CHECK(kLastError != nullptr);
        CHECK(kLastReasonCode != nullptr);
        CHECK(kLastPacketId != nullptr);
        CHECK(kInFlightCount != nullptr);
        CHECK(kInboundProperties != nullptr);
        CHECK(kForEachInboundUserProperty != nullptr);
        CHECK(kSetSubscribeCallback != nullptr);
        CHECK(kLastSubscriptionResult != nullptr);
        CHECK(kSetPacketTimeout != nullptr);
        CHECK(kSetSessionExpiryInterval != nullptr);
        CHECK(kGetSessionExpiryInterval != nullptr);
        CHECK(kEncodeVariableByteInteger != nullptr);
        CHECK(kVariableByteIntegerSize != nullptr);
        CHECK(kDecodeVariableByteInteger != nullptr);
    }
}

// The callback contract, checked as a live binding as well: an object of the
// library's own MQTT_CALLBACK_SIGNATURE type, bound to a function literally
// shaped void(char*, uint8_t*, unsigned int) (Requirement 12.1).
TEST_CASE("callback contract type binds a void(char*, uint8_t*, unsigned int) function") {
    REQUIRE(static_cast<bool>(api_shape::callback));

    char topic[] = "t";
    uint8_t payload[] = {'x'};
    api_shape::callback(topic, payload, 1u);   // exercising the exact call shape
    CHECK(true);
}
