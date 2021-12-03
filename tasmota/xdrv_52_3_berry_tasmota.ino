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
const uint32_t BERRY_MAX_REPL_LOGS = 1024;   // max number of print output recorded when inside REPL

/*********************************************************************************************\
 * Return C callback from index
 *
\*********************************************************************************************/
extern "C" {
  int32_t l_get_cb(struct bvm *vm);
  int32_t l_get_cb(struct bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 2 && be_isint(vm, 2)) {
      int32_t idx = be_toint(vm, 2);
      if (idx >= 0 && idx < ARRAY_SIZE(berry_callback_array)) {
        const berry_callback_t c_ptr = berry_callback_array[idx];
        be_pushcomptr(vm, (void*) c_ptr);
        be_return(vm);
      }
    }
    be_raise(vm, kTypeError, nullptr);
  }
}

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
    if (top >= 3 && be_isstring(vm, 2) && (be_isstring(vm, 3) || be_isinstance(vm, 3))) {  // 2 mandatory string arguments
      if (top == 3 || (top == 4 && be_isbool(vm, 4))) {           // 3rd optional argument must be bool
        const char * topic = be_tostring(vm, 2);
        const char * payload = nullptr;
        size_t payload_len = 0;
        if (be_isstring(vm, 3)) {
          payload = be_tostring(vm, 3);
          payload_len = strlen(payload);
        } else {
          be_getglobal(vm, "bytes"); /* get the bytes class */ /* TODO eventually replace with be_getbuiltin */
          if (be_isderived(vm, 3)) {
            payload = (const char *) be_tobytes(vm, 3, &payload_len);
          }
        }
        bool retain = false;
        if (top == 4) {
          retain = be_tobool(vm, 4);
        }
        if (!payload) { be_raise(vm, "value_error", "Empty payload"); }
        be_pop(vm, be_top(vm));
        MqttPublishPayload(topic, payload, payload_len, retain);
        be_return_nil(vm); // Return
      }
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: `tasmota.publish_result(payload:string, subtopic:string) -> nil``
  //
  int32_t l_publish_result(struct bvm *vm);
  int32_t l_publish_result(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top >= 3 && be_isstring(vm, 2) && be_isstring(vm, 3)) {  // 2 mandatory string arguments
      const char * payload = be_tostring(vm, 2);
      const char * subtopic = be_tostring(vm, 3);
      Response_P(PSTR("%s"), payload);
      be_pop(vm, top);
      MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, subtopic);
      be_return_nil(vm); // Return
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
      be_pop(vm, top);    // clear the stack before calling, because of re-entrant call to Berry in a Rule
      ExecuteCommand(command, SRC_BERRY);
      be_return_nil(vm); // Return
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

  // Berry: tasmota.time_reached(timer:int) -> bool
  //
  int32_t l_rtc(struct bvm *vm);
  int32_t l_rtc(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 1) {  // no argument (instance only)
      be_newobject(vm, "map");
      map_insert_int(vm, "utc", Rtc.utc_time);
      map_insert_int(vm, "local", Rtc.local_time);
      map_insert_int(vm, "restart", Rtc.restart_time);
      map_insert_int(vm, "timezone", Rtc.time_timezone);
      be_pop(vm, 1);
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: tasmota.memory() -> map
  //
  int32_t l_memory(struct bvm *vm);
  int32_t l_memory(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 1) {  // no argument (instance only)
      be_newobject(vm, "map");
      map_insert_int(vm, "flash", ESP.getFlashChipSize() / 1024);
      map_insert_int(vm, "program", ESP_getSketchSize() / 1024);
      map_insert_int(vm, "program_free", ESP.getFreeSketchSpace() / 1024);
      map_insert_int(vm, "heap_free", ESP_getFreeHeap() / 1024);
      map_insert_int(vm, "frag", ESP_getHeapFragmentation());
      if (UsePSRAM()) {
        map_insert_int(vm, "psram", ESP.getPsramSize() / 1024);
        map_insert_int(vm, "psram_free", ESP.getFreePsram() / 1024);
      }
      be_pop(vm, 1);
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: tasmota.wifi() -> map
  //
  int32_t l_wifi(struct bvm *vm);
  int32_t l_wifi(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 1) {  // no argument (instance only)
      be_newobject(vm, "map");
      if (Settings->flag4.network_wifi) {
        int32_t rssi = WiFi.RSSI();
        map_insert_int(vm, "rssi", rssi);
        map_insert_int(vm, "quality", WifiGetRssiAsQuality(rssi));
#if LWIP_IPV6
        String ipv6_addr = WifiGetIPv6();
        if (ipv6_addr != "") {
          map_insert_str(vm, "ip6", ipv6_addr.c_str());
        }
#endif
        if (static_cast<uint32_t>(WiFi.localIP()) != 0) {
          map_insert_str(vm, "mac", WiFi.macAddress().c_str());
          map_insert_str(vm, "ip", WiFi.localIP().toString().c_str());
        }
      }
      be_pop(vm, 1);
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: tasmota.eth() -> map
  //
  int32_t l_eth(struct bvm *vm);
  int32_t l_eth(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 1) {  // no argument (instance only)
      be_newobject(vm, "map");
#ifdef USE_ETHERNET
      if (static_cast<uint32_t>(EthernetLocalIP()) != 0) {
        map_insert_str(vm, "mac", EthernetMacAddress().c_str());
        map_insert_str(vm, "ip", EthernetLocalIP().toString().c_str());
      }
#endif
      be_pop(vm, 1);
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int32_t l_time_dump(bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 2 && be_isint(vm, 2)) {
      time_t ts = be_toint(vm, 2);
      struct tm *t = gmtime(&ts);
      be_newobject(vm, "map");
      map_insert_int(vm, "year", t->tm_year + 1900);
      map_insert_int(vm, "month", t->tm_mon + 1);
      map_insert_int(vm, "day", t->tm_mday);
      map_insert_int(vm, "hour", t->tm_hour);
      map_insert_int(vm, "min", t->tm_min);
      map_insert_int(vm, "sec", t->tm_sec);
      map_insert_int(vm, "weekday", t->tm_wday);
      be_pop(vm, 1);
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int32_t l_strftime(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 3 && be_isstring(vm, 2) && be_isint(vm, 3)) {
      const char * format = be_tostring(vm, 2);
      time_t ts = be_toint(vm, 3);
      struct tm *t = gmtime(&ts);
      char s[64] = {0};
      strftime(s, sizeof(s), format, t);
      be_pushstring(vm, s);
      be_return(vm);
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
    BrTimeoutYield();   // reset timeout
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
      be_pop(vm, top);  // avoid Error be_top is non zero message
      ResponseAppend_P(PSTR("%s"), msg);
      be_return_nil(vm); // Return nil when something goes wrong
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // web append with decimal conversion
  int32_t l_webSend(bvm *vm);
  int32_t l_webSend(bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 2 && be_isstring(vm, 2)) {
      const char *msg = be_tostring(vm, 2);
      be_pop(vm, top);  // avoid Error be_top is non zero message
      WSContentSend_P(PSTR("%s"), msg);
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
      be_pop(vm, top);  // avoid Error be_top is non zero message
      WSContentSend_PD(PSTR("%s"), msg);
      be_return_nil(vm); // Return nil when something goes wrong
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // get power
  int32_t l_getpower(bvm *vm);
  int32_t l_getpower(bvm *vm) {
    power_t pow = TasmotaGlobal.power;
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 2 && be_isint(vm, 2)) {
      pow = be_toint(vm, 2);
    }
    be_newobject(vm, "list");
    for (uint32_t i = 0; i < TasmotaGlobal.devices_present; i++) {
      be_pushbool(vm, bitRead(pow, i));
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
        be_pop(vm, top);  // avoid Error be_top is non zero message
        ExecuteCommandPower(idx + 1, (power) ? POWER_ON : POWER_OFF, SRC_BERRY);
        be_pushbool(vm, power);
        be_return(vm); // Return
      } else {
        be_return_nil(vm);
      }
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // get power
  int32_t l_getswitch(bvm *vm);
  int32_t l_getswitch(bvm *vm) {
    be_newobject(vm, "list");
    for (uint32_t i = 0; i < MAX_SWITCHES; i++) {
      if (PinUsed(GPIO_SWT1, i)) {
        be_pushbool(vm, Switch.virtual_state[i] == PRESSED);
        be_data_push(vm, -2);
        be_pop(vm, 1);
      }
    }
    be_pop(vm, 1);
    be_return(vm); // Return
  }

#ifdef USE_I2C
  // I2C specific
  // Berry: `i2c_enabled(index:int) -> bool` is I2C device enabled
  int32_t l_i2cenabled(struct bvm *vm);
  int32_t l_i2cenabled(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 2 && be_isint(vm, 2)) {
      int32_t index = be_toint(vm, 2);
      be_pop(vm, top);  // avoid Error be_top is non zero message
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

  // Berry: `arvh() -> string`
  // ESP object
  int32_t l_arch(bvm *vm);
  int32_t l_arch(bvm *vm) {
    be_pushstring(vm, ESP32_ARCH);
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

/*********************************************************************************************\
 * Native functions mapped to Berry functions
 *
 * read_sensors(show_sensor:bool) -> string
 *
\*********************************************************************************************/
extern "C" {
  int32_t l_read_sensors(struct bvm *vm);
  int32_t l_read_sensors(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    bool sensor_display = false;    // don't trigger a display by default
    if (top >= 2) {
      sensor_display = be_tobool(vm, 2);
    }
    be_pop(vm, top);    // clear stack to avoid `Error be_top is non zero=1` errors
    ResponseClear();
    if (MqttShowSensor(sensor_display)) {
      // return string
      be_pushstring(vm, ResponseData());
      be_return(vm);
    } else {
      be_return_nil(vm);
    }
  }
}

/*********************************************************************************************\
 * Logging functions
 *
\*********************************************************************************************/
// called as a replacement to Berry `print()`
void berry_log(const char * berry_buf);
void berry_log(const char * berry_buf) {
  const char * pre_delimiter = nullptr;   // do we need to prepend a delimiter if no REPL command
  size_t max_logs = berry.repl_active ? BERRY_MAX_REPL_LOGS : BERRY_MAX_LOGS;
  if (berry.log.log.length() == 0) {
    pre_delimiter = BERRY_CONSOLE_CMD_DELIMITER;
  }
  if (berry.log.log.length() >= BERRY_MAX_LOGS) {
    berry.log.log.remove(berry.log.log.head());
  }
  berry.log.addString(berry_buf, pre_delimiter, "\n");
  AddLog(LOG_LEVEL_INFO, PSTR("%s"), berry_buf);
}

const uint16_t LOGSZ = 128;                 // Max number of characters in log line

extern "C" {
  void berry_log_C(const char * berry_buf, ...) {
    // To save stack space support logging for max text length of 128 characters
    char log_data[LOGSZ];

    va_list arg;
    va_start(arg, berry_buf);
    uint32_t len = ext_vsnprintf_P(log_data, LOGSZ-3, berry_buf, arg);
    va_end(arg);
    if (len+3 > LOGSZ) { strcat(log_data, "..."); }  // Actual data is more
    berry_log(log_data);
  }
}


void berry_log_P(const char * berry_buf, ...) {
  // To save stack space support logging for max text length of 128 characters
  char log_data[LOGSZ];

  va_list arg;
  va_start(arg, berry_buf);
  uint32_t len = ext_vsnprintf_P(log_data, LOGSZ-3, berry_buf, arg);
  va_end(arg);
  if (len+3 > LOGSZ) { strcat(log_data, "..."); }  // Actual data is more
  berry_log(log_data);
}

#endif  // USE_BERRY
