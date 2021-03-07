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

const char kTypeError[] PROGMEM = "type_error";
/*********************************************************************************************\
 * Native functions mapped to Berry functions
 * 
 * log(msg:string [,log_level:int]) ->nil
 * 
 * import tasmota
 * 
 * tasmota.getfreeheap() -> int
 * tasmota.publish(topic:string, payload:string[, retain:bool]) -> nil
 * tasmota.cmd(command:string) -> string
 * tasmota.getoption(index:int) -> int
 * tasmota.millis([delay:int]) -> int
 * tasmota.timereached(timer:int) -> bool
 * tasmota.yield() -> nil
 * 
\*********************************************************************************************/
extern "C" {
  // Berry: `tasmota.publish(topic, payload [,retain]) -> nil``
  //
  int32_t l_publish(struct bvm *vm);
  int32_t l_publish(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top >= 2 && be_isstring(vm, 1) && be_isstring(vm, 2)) {  // 2 mandatory string arguments
      if (top == 2 || (top == 3 && be_isbool(vm, 3))) {           // 3rd optional argument must be bool
        const char * topic = be_tostring(vm, 1);
        const char * payload = be_tostring(vm, 2);
        bool retain = false;
        if (top == 3) {
          retain = be_tobool(vm, 3);
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
    if (top == 1 && be_isstring(vm, 1)) {  // only 1 argument of type string accepted
      const char * command = be_tostring(vm, 1);
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
    if (top == 0 || (top == 1 && be_isint(vm, 1))) {  // only 1 argument of type string accepted
      uint32_t delay = 0;
      if (top == 1) {
        delay = be_toint(vm, 1);
      }
      uint32_t ret_millis = millis() + delay;
      be_pushint(vm, ret_millis);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: tasmota.getoption(index:int) -> int
  //
  int32_t l_getoption(struct bvm *vm);
  int32_t l_getoption(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 1 && be_isint(vm, 1)) {
      uint32_t opt = GetOption(be_toint(vm, 1));
      be_pushint(vm, opt);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: tasmota.timereached(timer:int) -> bool
  //
  int32_t l_timereached(struct bvm *vm);
  int32_t l_timereached(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 1 && be_isint(vm, 1)) {  // only 1 argument of type string accepted
      uint32_t timer = be_toint(vm, 1);
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
    if (top == 1 && be_isint(vm, 1)) {  // only 1 argument of type string accepted
      uint32_t timer = be_toint(vm, 1);
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
    be_return(vm);
  }

  // Berry: `save(file:string, f:closure) -> bool`
  int32_t l_save(struct bvm *vm);
  int32_t l_save(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top ==2 && be_isstring(vm, 1) && be_isclosure(vm, 2)) {  // only 1 argument of type string accepted
      const char *fname = be_tostring(vm, 1);
      int32_t ret = be_savecode(vm, fname);
      be_pushint(vm, ret);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int32_t l_respCmnd(bvm *vm);
  int32_t l_respCmnd(bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 1) {
      const char *msg = be_tostring(vm, 1);
      Response_P("%s", msg);
      be_return_nil(vm); // Return nil when something goes wrong
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int32_t l_respCmndStr(bvm *vm);
  int32_t l_respCmndStr(bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 1) {
      const char *msg = be_tostring(vm, 1);
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
    if (top == 1 && be_isstring(vm, 1)) {
      const char *msg = be_tostring(vm, 1);
      strlcpy(XdrvMailbox.command, msg, CMDSZ);
      be_return_nil(vm); // Return nil when something goes wrong
    }
    be_raise(vm, kTypeError, nullptr);
  }


  static void map_insert_int(bvm *vm, const char *key, int value)
  {
    be_pushstring(vm, key);
    be_pushint(vm, value);
    be_data_insert(vm, -3);
    be_pop(vm, 2);
  }
  static void map_insert_bool(bvm *vm, const char *key, bool value)
  {
    be_pushstring(vm, key);
    be_pushbool(vm, value);
    be_data_insert(vm, -3);
    be_pop(vm, 2);
  }
  static void map_insert_str(bvm *vm, const char *key, const char *value)
  {
    be_pushstring(vm, key);
    be_pushstring(vm, value);
    be_data_insert(vm, -3);
    be_pop(vm, 2);
  }

  // get light
  int32_t l_getlight(bvm *vm);
  int32_t l_getlight(bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 0 || (top == 1 && be_isint(vm, 1))) {
      int32_t light_num = 0;
      if (top > 0) {
        light_num = be_toint(vm, 1);
      }
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
        uint8_t channelsb[LST_MAX];
        char rgbcw[12] = {0};
        char rgbcwb[12] = {0};
        light_state.getChannelsRaw(channels);
        light_state.getChannels(channelsb);

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
          }
          if (!light_controller.isCTRGBLinked()) {
            if (light_num == 0) {
              data_present = true;    // valid combination
              if (subtype > LST_RGB) { subtype = LST_RGB; } // limit to RGB
            }
            if ((light_num == 1) && subtype > LST_RGB) {
              data_present = true;    // valid combination
              subtype = subtype - LST_RGB;
              chanidx = 3;            // skip first 3 channels
            }
          }

          if (data_present) {
            // see ResponseLightState()
            map_insert_bool(vm, "power", (bool)(Light.power & 1));
            map_insert_int(vm, "bri", light_state.getBri());


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
            if (subtype > LST_NONE) {
              for (uint32_t i=0; i < subtype; i++) {
                snprintf_P(rgbcw, sizeof(rgbcw), PSTR("%s%02X"), rgbcw, channels[i+chanidx]);
                snprintf_P(rgbcwb, sizeof(rgbcwb), PSTR("%s%02X"), rgbcwb, channelsb[i+chanidx]);
              }
              map_insert_str(vm, "channels", rgbcw);
              map_insert_str(vm, "channelsb", rgbcwb);
              // map_insert_bool(vm, "gamma", Settings.light_correction);
            }
          }
        } else { // Light.pwm_multi_channels
          if ((light_num >= 0) && (light_num < LST_MAX)) {
            data_present = true;
            map_insert_bool(vm, "power", Light.power & (1 << light_num));
            map_insert_int(vm, "bri", Light.current_color[light_num]);
            snprintf_P(rgbcw, sizeof(rgbcw), PSTR("%02X"), channels[light_num]);
            snprintf_P(rgbcwb, sizeof(rgbcwb), PSTR("%02X"), channelsb[light_num]);
            map_insert_str(vm, "channels", rgbcw);
            map_insert_str(vm, "channelsb", rgbcwb);
          }
        }

        be_pop(vm, 1);
        if (data_present) {
          be_return(vm); // Return
        } else {
          be_return_nil(vm);    // no data, return nil instead of empty map
        }
      } else {
        be_return_nil(vm);
      }
    }
    be_raise(vm, kTypeError, nullptr);
  }

}

/*********************************************************************************************\
 * Native functions mapped to Berry functions
 * 
 * import wire
 * 
 * wire.getfreeheap() -> int
 * 
\*********************************************************************************************/
extern "C" {
  // Berry: `begintransmission(address:int) -> nil`
  int32_t b_wire_begintransmission(struct bvm *vm);
  int32_t b_wire_begintransmission(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 1 && be_isint(vm, 1)) {  // only 1 argument of type string accepted
      int32_t address = be_toint(vm, 1);
      Wire.beginTransmission(address);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: `endtransmission([stop:bool]) -> nil`
  int32_t b_wire_endtransmission(struct bvm *vm);
  int32_t b_wire_endtransmission(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 0 || (top == 1 && be_isbool(vm, 1))) {  // only 1 argument of type string accepted
      bool stop = true;
      if (top == 1) {
        stop = be_tobool(vm, 1);
      }
      uint32_t ret = Wire.endTransmission(stop);
      be_pushint(vm, ret);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: `requestfrom(address:int, quantity:int [stop:bool = true]) -> nil`
  int32_t b_wire_requestfrom(struct bvm *vm);
  int32_t b_wire_requestfrom(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if ( (top == 2 || (top == 3 && be_isbool(vm, 3))) 
         && be_isint(vm, 1) && be_isint(vm, 2) ) {
      int32_t address = be_toint(vm, 1);
      int32_t quantity = be_toint(vm, 2);
      bool stop = true;
      if (top == 3) {
        stop = be_tobool(vm, 3);
      }
      Wire.requestFrom((uint16_t)address, (uint8_t)quantity, stop);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: `available() -> bool`
  int32_t b_wire_available(struct bvm *vm);
  int32_t b_wire_available(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 0) {
      size_t available = Wire.available();
      be_pushint(vm, available);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: `write(value:int | s:string) -> nil`
  int32_t b_wire_write(struct bvm *vm);
  int32_t b_wire_write(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 1 && (be_isint(vm, 1) || be_isstring(vm, 1))) {
      if (be_isint(vm, 1)) {
        int32_t value = be_toint(vm, 1);
        Wire.write(value);
      } else if (be_isstring(vm, 1)) {
        const char * s = be_tostring(vm, 1);
        Wire.write(s);
      } else {
        be_return_nil(vm);
      }
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: `read() -> int`
  int32_t b_wire_read(struct bvm *vm);
  int32_t b_wire_read(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 0) {
      int32_t value = Wire.read();
      be_pushint(vm, value);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int32_t b_wire_scan(struct bvm *vm);
  int32_t b_wire_scan(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 0) {
      be_newobject(vm, "list");
      for (uint8_t address = 1; address <= 127; address++) {
        Wire.beginTransmission(address);
        int32_t error = Wire.endTransmission();
        if (0 == error) {
          be_pushint(vm, address);
          be_data_push(vm, -2);
          be_pop(vm, 1);
        }
      }
      be_pop(vm, 1);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: `validwrite(address:int, reg:int, val:int, size:int) -> bool or nil`
  int32_t b_wire_validwrite(struct bvm *vm);
  int32_t b_wire_validwrite(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 4 && be_isint(vm, 1) && be_isint(vm, 2) && be_isint(vm, 3) && be_isint(vm, 4)) {
      uint8_t addr = be_toint(vm, 1);
      uint8_t reg = be_toint(vm, 2);
      uint8_t val = be_toint(vm, 3);
      uint8_t size = be_toint(vm, 4);
      bool ok = I2cWrite(addr, reg, val, size);
      be_pushbool(vm, ok);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: `validread(address:int, reg:int, size:int) -> int or nil`
  int32_t b_wire_validread(struct bvm *vm);
  int32_t b_wire_validread(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 3 && be_isint(vm, 1) && be_isint(vm, 2) && be_isint(vm, 3)) {
      uint8_t addr = be_toint(vm, 1);
      uint8_t reg = be_toint(vm, 2);
      uint8_t size = be_toint(vm, 3);
      bool ok = I2cValidRead(addr, reg, size);
      if (ok) {
        be_pushint(vm, i2c_buffer);
      } else {
        be_pushnil(vm);
      }
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }
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
  int32_t l_logInfo(struct bvm *vm);
  int32_t l_logInfo(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top >= 1 && be_isstring(vm, 1)) {  // only 1 argument of type string accepted
      const char * msg = be_tostring(vm, 1);
      uint32_t log_level = LOG_LEVEL_INFO;
      if (top >= 2 && be_isint(vm, 2)) {
        log_level = be_toint(vm, 2);
        if (log_level > LOG_LEVEL_DEBUG_MORE) { log_level = LOG_LEVEL_DEBUG_MORE; }
      }
      AddLog(log_level, PSTR("%s"), msg);
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
}

// called as a replacement to Berry `print()`
void berry_log(const char * berry_buf);
void berry_log(const char * berry_buf) {
  AddLog(LOG_LEVEL_INFO, PSTR("%s"), berry_buf);
}


#endif  // USE_BERRY
