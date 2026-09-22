/*
  xdrv_52_3_berry_mqtt.ino - Berry scripting language, native fucnctions

  Copyright (C) 2021 Stephan Hadinger, Berry language by Guan Wenliang https://github.com/Skiars/berry

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef USE_BERRY

#include "berry.h"

// Keep every helper inside this linkage block. Arduino's sketch preprocessor otherwise
// emits helper prototypes before this tab's berry.h include, where bvm is still unknown.
extern "C" {

#if MQTT_VERSION == MQTT_VERSION_5_0
  // Insert helpers used to expose inbound MQTT 5.0 properties to Berry via mqtt.metadata().
  void be_mqtt_map_insert_nstring(bvm* vm, int32_t map, const char* key,
                                        const char* value, size_t len) {
    be_pushstring(vm, key);
    be_pushnstring(vm, value, len);
    be_data_insert(vm, map);
    be_pop(vm, 2);
  }

  void be_mqtt_map_insert_bytes(bvm* vm, int32_t map, const char* key,
                                      const uint8_t* value, size_t len) {
    be_pushstring(vm, key);
    be_pushbytes(vm, value, len);
    be_data_insert(vm, map);
    be_pop(vm, 2);
  }
#endif  // MQTT_VERSION == MQTT_VERSION_5_0

  int32_t be_mqtt_publish(struct bvm *vm);
  int32_t be_mqtt_respond(struct bvm *vm);
  int32_t be_mqtt_is_request(struct bvm *vm);
  int32_t be_mqtt_protocol(struct bvm *vm);
  int32_t be_mqtt_metadata(struct bvm *vm);
  int32_t be_mqtt_last_error(struct bvm *vm);

  // Berry: `mqtt.publish(topic, payload [, retain, start, len, loglevel]) -> nil`
  // Classic publish; also backs `tasmota.publish`. Runs the local rule engine on string
  // payloads. Raises on bad arguments.
  int32_t be_mqtt_publish(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top >= 3 && be_isstring(vm, 2) && (be_isstring(vm, 3) || be_isbytes(vm, 3))) {  // 2 mandatory string arguments
      bool retain = false;
      int32_t payload_start = 0;
      int32_t len = -1;   // send all of it
      bool is_binary = be_isbytes(vm, 3);       // is this a binary payload (or false = string)
      int32_t loglevel = LOG_LEVEL_INFO;
      if (top >= 4 && !be_isnil(vm, 4)) { retain = be_tobool(vm, 4); }
      if (top >= 5 && !be_isnil(vm, 5)) {
        if (!is_binary) { be_raise(vm, "argument_error", "start and len are not allowed with string payloads"); }
        payload_start = be_toint(vm, 5);
        if (payload_start < 0) { payload_start = 0; }
      }
      if (top >= 6 && !be_isnil(vm, 6)) { len = be_toint(vm, 6); }
      if (top >= 7 && !be_isnil(vm, 7)) { loglevel = be_toint(vm, 7); }
      const char * topic = be_tostring(vm, 2);
      const char * payload = nullptr;
      size_t payload_len = 0;

      if (be_isstring(vm, 3)) {
        payload = be_tostring(vm, 3);
        payload_len = strlen(payload);
      } else {
        payload = (const char *) be_tobytes(vm, 3, &payload_len);
      }
      if (!payload) { be_raise(vm, "value_error", "Empty payload"); }

      // adjust start and len
      if (payload_start >= payload_len) { len = 0; }              // send empty packet
      else if (len < 0) { len = payload_len - payload_start; }    // send all packet, adjust len
      else if (payload_start + len > payload_len) { len = payload_len - payload_start; }    // len is too long, adjust
      // adjust start
      payload = payload + payload_start;

      be_pop(vm, be_top(vm));   // clear stack to avoid any indirect warning message in subsequent calls to Berry

      MqttPublishPayload(topic, payload, is_binary ? len : 0 /*if string don't send length*/, retain, loglevel);
      if (!is_binary) {
        XdrvRulesProcess(0, payload);  // Process rules on berry publish
      }

      be_return_nil(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: `mqtt.respond(payload) -> bool`
  // Reply to the MQTT 5.0 request currently being handled: publishes `payload` to the
  // request's Response Topic, echoing its Correlation Data, QoS 0 and non-retained.
  // Returns true if a response was sent, false when there is no active request (see
  // `mqtt.is_request()`), on an MQTT 3.1.1 broker, or on a bad payload argument.
  int32_t be_mqtt_respond(struct bvm *vm) {
    const int32_t top = be_top(vm);
    const uint8_t* payload = nullptr;
    size_t payload_len = 0;
    bool has_payload = false;
    if (top >= 2) {
      if (be_isstring(vm, 2)) {
        payload = reinterpret_cast<const uint8_t*>(be_tostring(vm, 2));
        payload_len = be_strlen(vm, 2);
        has_payload = true;
      } else if (be_isbytes(vm, 2)) {
        payload = static_cast<const uint8_t*>(be_tobytes(vm, 2, &payload_len));
        has_payload = true;
      }
    }

    bool sent = false;
#if MQTT_VERSION == MQTT_VERSION_5_0
    if (has_payload && (payload_len <= UINT_MAX) &&
        ((payload != nullptr) || (payload_len == 0))) {
      sent = MqttRespond(payload, (unsigned int)payload_len);
    }
#else
    (void)has_payload;
    (void)payload;
    (void)payload_len;
#endif
    be_pushbool(vm, sent);
    be_return(vm);
  }

  // Berry: `mqtt.is_request() -> bool`
  // True while handling an inbound MQTT 5.0 message that carried a Response Topic, i.e. a
  // request that can be answered with `mqtt.respond()`. Always false on MQTT 3.1.1.
  int32_t be_mqtt_is_request(struct bvm *vm) {
#if MQTT_VERSION == MQTT_VERSION_5_0
    be_pushbool(vm, MqttIsRequest());
#else
    be_pushbool(vm, bfalse);
#endif
    be_return(vm);
  }

  int32_t be_mqtt_protocol(struct bvm *vm) {
#if MQTT_VERSION == MQTT_VERSION_5_0
    be_pushint(vm, (MqttClient.protocolVersion() == MQTT_VERSION_5_0) ? 5 : 4);
#else
    be_pushint(vm, 4);
#endif
    be_return(vm);
  }

  int32_t be_mqtt_metadata(struct bvm *vm) {
    be_newobject(vm, "map");
#if MQTT_VERSION == MQTT_VERSION_5_0
    if (MqttClient.protocolVersion() == MQTT_VERSION_5_0) {
      const MqttInboundProperties& properties = MqttClient.inboundProperties();
      const int32_t map = be_absindex(vm, -1);
      be_map_insert_int(vm, "qos", properties.qos);
      be_map_insert_bool(vm, "retain", properties.retained);
      if (properties.responseTopic.present()) {
        be_mqtt_map_insert_nstring(vm, map, "response_topic",
                                   properties.responseTopic.data,
                                   properties.responseTopic.len);
      }
      if (properties.hasCorrelationData) {
        be_mqtt_map_insert_bytes(vm, map, "correlation_data",
                                 properties.correlationData,
                                 properties.correlationDataLen);
      }
    }
#endif
    be_pop(vm, 1);
    be_return(vm);
  }

  int32_t be_mqtt_last_error(struct bvm *vm) {
#if MQTT_VERSION == MQTT_VERSION_5_0
    if (MqttClient.protocolVersion() == MQTT_VERSION_5_0) {
      be_newobject(vm, "map");
      be_map_insert_int(vm, "error", (uint8_t)MqttClient.lastError());
      be_map_insert_int(vm, "reason_code", MqttClient.lastReasonCode());
      be_map_insert_int(vm, "packet_id", MqttClient.lastPacketId());
      be_map_insert_int(vm, "server_disconnect_reason",
                        MqttClient.serverDisconnectReasonCode());
      be_pop(vm, 1);
      be_return(vm);
    }
#endif
    be_return_nil(vm);
  }

  void be_mqtt_subscribe(const char* topic) {
    if (!topic) { return; }
    MqttSubscribe(topic);
  }

  void be_mqtt_unsubscribe(const char* topic) {
    if (!topic) { return; }
    MqttUnsubscribe(topic);
  }

  bbool be_mqtt_connected(void) {
    return Mqtt.connected;
  }
}

#endif  // USE_BERRY
