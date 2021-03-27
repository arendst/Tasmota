/*
  xdrv_52_3_berry_native.ino - Berry scripting language, native fucnctions

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

#include <berry.h>
#include <Wire.h>

const uint32_t BERRY_MAX_LOGS = 16;   // max number of print output recorded when outside of REPL, used to avoid infinite grow of logs

/*********************************************************************************************\
 * Native functions mapped to Berry functions
 * 
 * log(msg:string [,log_level:int]) ->nil
 * 
 * import tasmota
 * 
 * tasmota.get_free_heap() -> int
 * tasmota.publish(topic:string, payload:string[, retain:bool]) -> nil
 * tasmota.cmd(command:string) -> string
 * tasmota.get_option(index:int) -> int
 * tasmota.millis([delay:int]) -> int
 * tasmota.time_reached(timer:int) -> bool
 * tasmota.yield() -> nil
 * 
 * tasmota.get_light([index:int = 0]) -> map
 * tasmota.get_power([index:int = 0]) -> bool
 * tasmota.set_power(idx:int, power:bool) -> bool or nil
 * tasmota.set_light(idx:int, values:map) -> map
 * 
\*********************************************************************************************/
extern "C" {
  // Berry: `tasmota.publish(topic, payload [,retain]) -> nil``
  //
  int32_t l_publish(struct bvm *vm);
  int32_t l_publish(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top >= 3 && be_isstring(vm, 2) && be_isstring(vm, 3)) {  // 2 mandatory string arguments
      if (top == 3 || (top == 4 && be_isbool(vm, 4))) {           // 3rd optional argument must be bool
        const char * topic = be_tostring(vm, 2);
        const char * payload = be_tostring(vm, 3);
        bool retain = false;
        if (top == 4) {
          retain = be_tobool(vm, 4);
        }
        strlcpy(TasmotaGlobal.mqtt_data, payload, sizeof(TasmotaGlobal.mqtt_data));
        MqttPublish(topic, retain);
        be_return(vm); // Return
      }
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: `tasmota.cmd(command:string) -> string`
  //
  int32_t l_cmd(struct bvm *vm);
  int32_t l_cmd(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 2 && be_isstring(vm, 2)) {  // only 1 argument of type string accepted
      const char * command = be_tostring(vm, 2);
      ExecuteCommand(command, SRC_BERRY);
      be_pushstring(vm, TasmotaGlobal.mqtt_data);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: tasmota.millis([delay:int]) -> int
  //
  int32_t l_millis(struct bvm *vm);
  int32_t l_millis(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 1 || (top == 2 && be_isint(vm, 2))) {  // only 1 argument of type string accepted
      uint32_t delay = 0;
      if (top == 2) {
        delay = be_toint(vm, 2);
      }
      uint32_t ret_millis = millis() + delay;
      be_pushint(vm, ret_millis);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: tasmota.get_option(index:int) -> int
  //
  int32_t l_getoption(struct bvm *vm);
  int32_t l_getoption(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 2 && be_isint(vm, 2)) {
      uint32_t opt = GetOption(be_toint(vm, 2));
      be_pushint(vm, opt);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: tasmota.time_reached(timer:int) -> bool
  //
  int32_t l_timereached(struct bvm *vm);
  int32_t l_timereached(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 2 && be_isint(vm, 2)) {  // only 1 argument of type string accepted
      uint32_t timer = be_toint(vm, 2);
      bool reached = TimeReached(timer);
      be_pushbool(vm, reached);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: tasmota.delay(timer:int) -> nil
  //
  int32_t l_delay(struct bvm *vm);
  int32_t l_delay(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 2 && be_isint(vm, 2)) {  // only 1 argument of type string accepted
      uint32_t timer = be_toint(vm, 2);
      delay(timer);
      be_return_nil(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: `yield() -> nil`
  // ESP object
  int32_t l_yield(bvm *vm);
  int32_t l_yield(bvm *vm) {
    optimistic_yield(10);
    be_return_nil(vm);
  }

  // Berry: tasmota.scale_uint(int * 5) -> int
  //
  int32_t l_scaleuint(struct bvm *vm);
  int32_t l_scaleuint(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 6 && be_isint(vm, 2) && be_isint(vm, 3) && be_isint(vm, 4) && be_isint(vm, 5) && be_isint(vm, 6)) {  // only 1 argument of type string accepted
      int32_t v = be_toint(vm, 2);
      int32_t from1 = be_toint(vm, 3);
      int32_t from2 = be_toint(vm, 4);
      int32_t to1 = be_toint(vm, 5);
      int32_t to2 = be_toint(vm, 6);

      int32_t ret = changeUIntScale(v, from1, from2, to1, to2);
      be_pushint(vm, ret);
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int32_t l_respCmnd(bvm *vm);
  int32_t l_respCmnd(bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 2) {
      const char *msg = be_tostring(vm, 2);
      Response_P("%s", msg);
      be_return_nil(vm); // Return nil when something goes wrong
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int32_t l_respCmndStr(bvm *vm);
  int32_t l_respCmndStr(bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 2) {
      const char *msg = be_tostring(vm, 2);
      ResponseCmndChar(msg);
      be_return_nil(vm); // Return nil when something goes wrong
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int32_t l_respCmndDone(bvm *vm);
  int32_t l_respCmndDone(bvm *vm) {
    ResponseCmndDone();
    be_return_nil(vm);
  }
  
  int32_t l_respCmndError(bvm *vm);
  int32_t l_respCmndError(bvm *vm) {
    ResponseCmndError();
    be_return_nil(vm);
  }

  int32_t l_respCmndFailed(bvm *vm);
  int32_t l_respCmndFailed(bvm *vm) {
    ResponseCmndFailed();
    be_return_nil(vm);
  }

  // update XdrvMailbox.command with actual command
  int32_t l_resolveCmnd(bvm *vm);
  int32_t l_resolveCmnd(bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 2 && be_isstring(vm, 2)) {
      const char *msg = be_tostring(vm, 2);
      strlcpy(XdrvMailbox.command, msg, CMDSZ);
      be_return_nil(vm); // Return nil when something goes wrong
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Response_append
  int32_t l_respAppend(bvm *vm);
  int32_t l_respAppend(bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 2 && be_isstring(vm, 2)) {
      const char *msg = be_tostring(vm, 2);
      ResponseAppend_P(PSTR("%s"), msg);
      be_return_nil(vm); // Return nil when something goes wrong
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // web append with decimal conversion
  int32_t l_webSendDecimal(bvm *vm);
  int32_t l_webSendDecimal(bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 2 && be_isstring(vm, 2)) {
      const char *msg = be_tostring(vm, 2);
      WSContentSend_PD(PSTR("%s"), msg);
      be_return_nil(vm); // Return nil when something goes wrong
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // push the light status object on the vm stack
  void push_getlight(bvm *vm, uint32_t light_num) {
    bool data_present = false;      // do we have relevant data
    be_newobject(vm, "map");
    // check if the light exist
    // TasmotaGlobal.devices_present
    // Light.device
    // Light.subtype
    // Light.pwm_multi_channels
    // light_controller.isCTRGBLinked()

    if (Light.device > 0) {
      // we have a light
      
      uint8_t channels[LST_MAX];
      char s_rgb[8] = {0};         // RGB raw levels
      light_controller.calcLevels(channels);
      uint8_t bri = light_state.getBri();

      // map_insert_int(vm, "_devices_present", TasmotaGlobal.devices_present);
      // map_insert_int(vm, "_light_device", Light.device);
      // map_insert_int(vm, "_light_subtype", Light.subtype);
      // map_insert_int(vm, "_light_multi", Light.pwm_multi_channels);
      // map_insert_int(vm, "_light_linked", light_controller.isCTRGBLinked());

      if (!Light.pwm_multi_channels) {
        uint32_t subtype = Light.subtype;   // virtual sub-type, for SO37 128
        uint32_t chanidx = 0;               // channel offset, for SO37 128


        if (light_controller.isCTRGBLinked() && (light_num == 0)) {
          data_present = true;    // valid combination
          if (subtype >= LST_RGBW) {
            map_insert_str(vm, "colormode", (light_state.getColorMode() & LCM_RGB ? "rgb" : "ct"));
          }
        }
        if (!light_controller.isCTRGBLinked()) {
          if (light_num == 0) {
            data_present = true;    // valid combination
            if (subtype > LST_RGB) { subtype = LST_RGB; } // limit to RGB
            bri = light_state.getBriRGB();
          }
          if ((light_num == 1) && subtype > LST_RGB) {
            data_present = true;    // valid combination
            subtype = subtype - LST_RGB;
            chanidx = 3;            // skip first 3 channels
            bri = light_state.getBriCT();
          }
        }

        if (data_present) {
          // see ResponseLightState()
          map_insert_bool(vm, "power", bitRead(TasmotaGlobal.power, light_num));
          map_insert_int(vm, "bri", bri);

          if (subtype >= LST_RGB) {
            uint16_t hue;
            uint8_t  sat, bri;
            light_state.getHSB(&hue, &sat, &bri);
            map_insert_int(vm, "hue", hue);
            map_insert_int(vm, "sat", sat);
          }
          if ((LST_COLDWARM == subtype) || (LST_RGBW <= subtype)) {
            map_insert_int(vm, "ct", light_state.getCT());
          }
          if (subtype >= LST_RGB) {
            snprintf(s_rgb, sizeof(s_rgb), PSTR("%02X%02X%02X"), channels[0], channels[1], channels[2]);
            map_insert_str(vm, "rgb", s_rgb);
          }
          if (subtype > LST_NONE) {
            map_insert_list_uint8(vm, "channels", &channels[chanidx], subtype);
          }
        }
      } else { // Light.pwm_multi_channels
        if ((light_num >= 0) && (light_num < LST_MAX)) {
          data_present = true;
          map_insert_bool(vm, "power", Light.power & (1 << light_num));
          map_insert_int(vm, "bri", Light.current_color[light_num]);
          map_insert_list_uint8(vm, "channels", &channels[light_num], 1);
        }
      }

      be_pop(vm, 1);
      if (!data_present) {
        be_pop(vm, 1);
        be_pushnil(vm);
      }
    } else {
      be_pop(vm, 1);
      be_pushnil(vm);
    }
  }

  // get light
  int32_t l_getlight(bvm *vm);
  int32_t l_getlight(bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 1 || (top == 2 && be_isint(vm, 2))) {
      int32_t light_num = 0;
      if (top > 1) {
        light_num = be_toint(vm, 2);
      }
      push_getlight(vm, light_num);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // set light
  int32_t l_setlight(bvm *vm);
  int32_t l_setlight(bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top >= 2 && be_isinstance(vm, 2) && (top != 3 || be_isint(vm, 3))) {
      int32_t idx = 0;
      if (top >= 3) {
        idx = be_toint(vm, 3);
        be_pop(vm, 1);    // remove last argument to have the map at the top of stack
      }

      // power
      if (map_find(vm, "power")) {
        bool power = be_tobool(vm, -1);
        bool current_power = bitRead(TasmotaGlobal.power, idx);
        if (power != current_power) {   // only send command if needed
          ExecuteCommandPower(Light.device + idx, (power) ? POWER_ON : POWER_OFF, SRC_BERRY);
        }
      }
      be_pop(vm, 1);

      // ct
      if (map_find(vm, "ct")) {
        int32_t ct = be_toint(vm, -1);
        light_controller.changeCTB(ct, light_state.getBriCT());
      }
      be_pop(vm, 1);

      // hue
      if (map_find(vm, "hue")) {
        int32_t hue = be_toint(vm, -1);
        uint8_t sat;
        uint8_t bri;
        light_state.getHSB(nullptr, &sat, &bri);
        light_controller.changeHSB(hue, sat, bri);
      }
      be_pop(vm, 1);

      // sat
      if (map_find(vm, "sat")) {
        int32_t sat = be_toint(vm, -1);
        uint16_t hue;
        uint8_t bri;
        light_state.getHSB(&hue, nullptr, &bri);
        light_controller.changeHSB(hue, sat, bri);
      }
      be_pop(vm, 1);

      // rgb
      if (map_find(vm, "rgb")) {
        const char * rgb_s = be_tostring(vm, -1);
        SBuffer buf = SBuffer::SBufferFromHex(rgb_s, strlen(rgb_s));
        uint8_t channels[LST_MAX] = {};
        memcpy(channels, buf.buf(), buf.len() > LST_MAX ? LST_MAX : buf.len());
        bool on = false;    // if all are zero, then only set power off
        for (uint32_t i = 0; i < LST_MAX; i++) {
          if (channels[i] != 0) { on = true; }
        }
        if (on) {
          light_controller.changeChannels(channels);
        } else {
          ExecuteCommandPower(idx + 1, POWER_OFF, SRC_BERRY);
        }
      }
      be_pop(vm, 1);

      // channels
      if (map_find(vm, "channels")) {
        if (be_isinstance(vm, -1)) {
          be_getbuiltin(vm, "list");    // add "list" class 
          if (be_isderived(vm, -2)) {
            be_pop(vm, 1);      // remove "list" class from top
            int32_t list_size = get_list_size(vm);
            // AddLog(LOG_LEVEL_INFO, "Instance is list size = %d", list_size);

            uint8_t channels[LST_MAX] = {};     // initialized with all zeroes
            if (list_size > LST_MAX) { list_size = LST_MAX; }   // no more than 5 channels, no need to test for positive, any negative will be discarded by loop
            for (uint32_t i = 0; i < list_size; i++) {
              // be_dumpstack(vm);
              get_list_item(vm, i);
              // be_dumpstack(vm);
              int32_t val = be_toint(vm, -1);
              be_pop(vm, 1);      // remove result from stack
              channels[i] = to_u8(val);

              bool on = false;    // if all are zero, then only set power off
              for (uint32_t i = 0; i < LST_MAX; i++) {
                if (channels[i] != 0) { on = true; }
              }
              if (on) {
                light_controller.changeChannels(channels);
              } else {
                ExecuteCommandPower(idx + 1, POWER_OFF, SRC_BERRY);
              }
            }
          } else {
            be_pop(vm, 1);      // remove "list" class from top
          }
        }
      }
      be_pop(vm, 1);

      // bri is done after channels and rgb
      // bri
      if (map_find(vm, "bri")) {
        int32_t bri = be_toint(vm, -1);
        light_controller.changeBri(bri);
      }
      be_pop(vm, 1);

      push_getlight(vm, idx);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }  // TODO

  // get power
  int32_t l_getpower(bvm *vm);
  int32_t l_getpower(bvm *vm) {
    be_newobject(vm, "list");
    for (uint32_t i = 0; i < TasmotaGlobal.devices_present; i++) {
      be_pushbool(vm, bitRead(TasmotaGlobal.power, i));
      be_data_push(vm, -2);
      be_pop(vm, 1);
    }
    be_pop(vm, 1);
    be_return(vm); // Return
  }

  int32_t l_setpower(bvm *vm);
  int32_t l_setpower(bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 3 && be_isint(vm, 2) && be_isbool(vm, 3)) {
      int32_t idx = be_toint(vm, 2);
      bool power = be_tobool(vm, 3);
      if ((idx >= 0) && (idx < TasmotaGlobal.devices_present)) {
        ExecuteCommandPower(idx + 1, (power) ? POWER_ON : POWER_OFF, SRC_BERRY);
        be_pushbool(vm, power);
        be_return(vm); // Return
      } else {
        be_return_nil(vm);
      }
    }
    be_raise(vm, kTypeError, nullptr);
  }

#ifdef USE_I2C
  // I2C specific
  // Berry: `i2c_enabled(index:int) -> bool` is I2C device enabled
  int32_t l_i2cenabled(struct bvm *vm);
  int32_t l_i2cenabled(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 2 && be_isint(vm, 2)) {
      int32_t index = be_toint(vm, 2);
      bool enabled = I2cEnabled(index);
      be_pushbool(vm, enabled);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }
#else // USE_I2C
  int32_t l_i2cenabled(struct bvm *vm) __attribute__ ((weak, alias ("b_wire_i2cmissing")));
#endif // USE_I2C

}

/*********************************************************************************************\
 * Native functions mapped to Berry functions
 * 
 * log(msg:string [,log_level:int]) ->nil
 * 
\*********************************************************************************************/
extern "C" {
  // Berry: `log(msg:string [,log_level:int]) ->nil`
  // Logs the string at LOG_LEVEL_INFO (loglevel=2)
  // We allow this function to be called as a method or a direct function
  // if the first argument is an instance, we remove it
  int32_t l_logInfo(struct bvm *vm);
  int32_t l_logInfo(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top >= 2 && be_isstring(vm, 2)) {  // only 1 argument of type string accepted
      const char * msg = be_tostring(vm, 2);
      uint32_t log_level = LOG_LEVEL_INFO;
      if (top >= 3 && be_isint(vm, 3)) {
        log_level = be_toint(vm, 3);
        if (log_level > LOG_LEVEL_DEBUG_MORE) { log_level = LOG_LEVEL_DEBUG_MORE; }
      }
      AddLog_P(log_level, PSTR("%s"), msg);
      be_return(vm); // Return
    }
    be_return_nil(vm); // Return nil when something goes wrong
  }

  // Berry: `getFreeHeap() -> int`
  // ESP object
  int32_t l_getFreeHeap(bvm *vm);
  int32_t l_getFreeHeap(bvm *vm) {
    be_pushint(vm, ESP.getFreeHeap());
    be_return(vm);
  }

  // Berry: `save(file:string, f:closure) -> bool`
  int32_t l_save(struct bvm *vm);
  int32_t l_save(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 3 && be_isstring(vm, 2) && be_isclosure(vm, 3)) {  // only 1 argument of type string accepted
      const char *fname = be_tostring(vm, 2);
      int32_t ret = be_savecode(vm, fname);
      be_pushint(vm, ret);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }
}

// called as a replacement to Berry `print()`
void berry_log(const char * berry_buf);
void berry_log(const char * berry_buf) {
  const char * pre_delimiter = nullptr;   // do we need to prepend a delimiter if no REPL command
  if (!berry.repl_active) {
    // if no REPL in flight, we limit the number of logs
    if (berry.log.log.length() == 0) {
      pre_delimiter = BERRY_CONSOLE_CMD_DELIMITER;
    }
    if (berry.log.log.length() >= BERRY_MAX_LOGS) {
      berry.log.log.remove(berry.log.log.head());
    }
  }
  // AddLog(LOG_LEVEL_INFO, PSTR("[Add to log] %s"), berry_buf);
  berry.log.addString(berry_buf, pre_delimiter, "\n");
  AddLog_P(LOG_LEVEL_INFO, PSTR("%s"), berry_buf);
}


#endif  // USE_BERRY
