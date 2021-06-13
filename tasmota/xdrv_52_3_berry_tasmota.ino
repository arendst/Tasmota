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
    if (top >= 3 && be_isstring(vm, 2) && be_isstring(vm, 3)) {  // 2 mandatory string arguments
      if (top == 3 || (top == 4 && be_isbool(vm, 4))) {           // 3rd optional argument must be bool
        const char * topic = be_tostring(vm, 2);
        const char * payload = be_tostring(vm, 3);
        bool retain = false;
        if (top == 4) {
          retain = be_tobool(vm, 4);
        }
        Response_P(payload);
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
      be_pop(vm, 2);    // clear the stack before calling, because of re-entrant call to Berry in a Rule
      ExecuteCommand(command, SRC_BERRY);
#ifdef MQTT_DATA_STRING
      be_pushstring(vm, TasmotaGlobal.mqtt_data.c_str());
#else
      be_pushstring(vm, TasmotaGlobal.mqtt_data);
#endif
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

  // Berry: tasmota.memory(timer:int) -> bool
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
      int32_t freeMaxMem = 100 - (int32_t)(ESP_getMaxAllocHeap() * 100 / ESP_getFreeHeap());
      map_insert_int(vm, "frag", freeMaxMem);
      if (psramFound()) {
        map_insert_int(vm, "psram", ESP.getPsramSize() / 1024);
        map_insert_int(vm, "psram_free", ESP.getFreePsram() / 1024);
      }
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
  int32_t l_webSend(bvm *vm);
  int32_t l_webSend(bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 2 && be_isstring(vm, 2)) {
      const char *msg = be_tostring(vm, 2);
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
      WSContentSend_PD(PSTR("%s"), msg);
      be_return_nil(vm); // Return nil when something goes wrong
    }
    be_raise(vm, kTypeError, nullptr);
  }

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


/*********************************************************************************************\
 * Helper function for `Driver` class
 * 
 * get_tasmota() -> tasmota instance from globals
 *   allows to use solidified methods refering to the global object `tasmota`
 * 
\*********************************************************************************************/
extern "C" {

  int32_t d_getTasmotaGlob(struct bvm *vm);
  int32_t d_getTasmotaGlob(struct bvm *vm) {
    be_getglobal(berry.vm, PSTR("tasmota"));
    be_return(vm); // Return
  }

}

#endif  // USE_BERRY
