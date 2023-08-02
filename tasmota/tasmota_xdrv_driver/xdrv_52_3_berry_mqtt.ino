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

// Mappgin from internal light and a generic `light_state` Berry class

#ifdef USE_BERRY

#include "berry.h"

// Berry: `tasmota.publish(topic, payload [, retain:bool, start:int, len:int]) -> nil``
// is_method is true if called from Tasmota class, false if called from mqtt module
extern "C" {
  int32_t be_mqtt_publish(struct bvm *vm);
  int32_t be_mqtt_publish(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top >= 3 && be_isstring(vm, 2) && (be_isstring(vm, 3) || be_isbytes(vm, 3))) {  // 2 mandatory string arguments
      bool retain = false;
      int32_t payload_start = 0;
      int32_t len = -1;   // send all of it
      bool is_binary = be_isbytes(vm, 3);       // is this a binary payload (or false = string)
      if (top >= 4) { retain = be_tobool(vm, 4); }
      if (top >= 5) {
        if (!is_binary) { be_raise(vm, "argument_error", "start and len are not allowed with string payloads"); }
        payload_start = be_toint(vm, 5);
        if (payload_start < 0) payload_start = 0;
      }
      if (top >= 6) { len = be_toint(vm, 6); }
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

      MqttPublishPayload(topic, payload, is_binary ? len : 0 /*if string don't send length*/, retain);

      be_return_nil(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
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
