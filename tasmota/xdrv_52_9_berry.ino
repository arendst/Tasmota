/*
  xdrv_52_9_berry.ino - Berry scripting language

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

#define XDRV_52             52

#include <berry.h>

const char kBrCommands[] PROGMEM = D_PRFX_BR "|"    // prefix
  D_CMND_BR_RUN "|" D_CMND_BR_RESET
  ;

void (* const BerryCommand[])(void) PROGMEM = {
  CmndBrRun, CmndBrReset,
  };

class BerrySupport {
public:
  bvm *vm = nullptr;                    // berry vm
  bool rules_busy = false;              // are we already processing rules, avoid infinite loop
  const char *fname = nullptr;    // name of berry function to call
  int32_t     fret = 0;
};
BerrySupport berry;

//
// Sanity Check for be_top()
//
// Checks that the Berry stack is empty, if not print a Warning and empty it
//
void checkBeTop(void) {
  int32_t top = be_top(berry.vm);
  if (top != 0) {
    be_pop(berry.vm, top);   // TODO should not be there
    AddLog(LOG_LEVEL_ERROR, D_LOG_BERRY "Error be_top is non zero=%d", top);
  }
}

/*********************************************************************************************\
 * Handlers for Berry calls and async
 * 
\*********************************************************************************************/
// // call a function (if exists) of type void -> void
// void callBerryFunctionVoid_berry(const char * fname) {
//   berry.fret = 0;
//   callBerryFunctionVoid(berry.fname);
// }

bool callBerryRule(void) {
  if (berry.rules_busy) { return false; }
  berry.rules_busy = true;
  char * json_event = TasmotaGlobal.mqtt_data;
  bool serviced = false;

  checkBeTop();
  be_getglobal(berry.vm, "_exec_rules");
  if (!be_isnil(berry.vm, -1)) {

    // {
    //   String event_saved = TasmotaGlobal.mqtt_data;
    //   // json_event = {"INA219":{"Voltage":4.494,"Current":0.020,"Power":0.089}}
    //   // json_event = {"System":{"Boot":1}}
    //   // json_event = {"SerialReceived":"on"} - invalid but will be expanded to {"SerialReceived":{"Data":"on"}}
    //   char *p = strchr(json_event, ':');
    //   if ((p != NULL) && !(strchr(++p, ':'))) {  // Find second colon
    //     event_saved.replace(F(":"), F(":{\"Data\":"));
    //     event_saved += F("}");
    //     // event_saved = {"SerialReceived":{"Data":"on"}}
    //   }
    //   be_pushstring(berry.vm, event_saved.c_str());
    // }
    be_pushstring(berry.vm, TasmotaGlobal.mqtt_data);
    int ret = be_pcall(berry.vm, 1);
    serviced = be_tobool(berry.vm, 1);
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_BERRY "Event (%s) serviced=%d"), TasmotaGlobal.mqtt_data, serviced);
    be_pop(berry.vm, 2);    // remove function object
  } else {
    be_pop(berry.vm, 1);    // remove nil object
  }
  checkBeTop();
  berry.rules_busy = false;

  return serviced;     // TODO event not handled
}

bool callBerryCommand(void) {
  const char * command = nullptr;

  checkBeTop();
  be_getglobal(berry.vm, "_exec_cmd");
  if (!be_isnil(berry.vm, -1)) {
    be_pushstring(berry.vm, XdrvMailbox.topic);
    be_pushint(berry.vm, XdrvMailbox.index);
    be_pushstring(berry.vm, XdrvMailbox.data);
    int ret = be_pcall(berry.vm, 3);
    command = be_tostring(berry.vm, 3);
    strlcpy(XdrvMailbox.topic, command, CMDSZ);
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_BERRY "Command (%s) serviced=%d"), XdrvMailbox.command, serviced);
    be_pop(berry.vm, 4);    // remove function object
  } else {
    be_pop(berry.vm, 1);    // remove nil object
  }
  checkBeTop();

  return command != nullptr;     // TODO event not handled
}

size_t callBerryGC(void) {
  size_t ram_used = 0;
  checkBeTop();
  be_getglobal(berry.vm, "_gc");
  if (!be_isnil(berry.vm, -1)) {
    int ret = be_pcall(berry.vm, 0);
    ram_used = be_toint(berry.vm, 1);
    be_pop(berry.vm, 1);    // remove function object
  } else {
    be_pop(berry.vm, 1);    // remove nil object
  }
  checkBeTop();

  return ram_used;
}

// void callBerryMqttData(void) {
//   AddLog(LOG_LEVEL_INFO, D_LOG_BERRY "callBerryMqttData");
//   if (nullptr == berry.vm) { return; }
//   if (XdrvMailbox.data_len < 1) {
//     return;
//   }
//   const char * topic = XdrvMailbox.topic;
//   const char * payload = XdrvMailbox.data;

//   checkBeTop();
//   be_getglobal(berry.vm, "mqtt_data_dispatch");
//   if (!be_isnil(berry.vm, -1)) {
//     be_pushstring(berry.vm, topic);
//     be_pushstring(berry.vm, payload);
//     be_pcall(berry.vm, 0);
//     be_pop(berry.vm, 3);    // remove function object
//   } else {
//     be_pop(berry.vm, 1);    // remove nil object
//   }
//   checkBeTop();
// }

// call a function (if exists) of type void -> void
void callBerryFunctionVoid(const char * fname) {
  if (nullptr == berry.vm) { return; }
  checkBeTop();
  be_getglobal(berry.vm, fname);
  if (!be_isnil(berry.vm, -1)) {
    be_pcall(berry.vm, 0);
  }
  be_pop(berry.vm, 1);    // remove function or nil object
  checkBeTop();
}

/*********************************************************************************************\
 * VM Init
\*********************************************************************************************/
extern "C" {
  extern size_t be_gc_memcount(bvm *vm);
  extern void be_gc_collect(bvm *vm);
}
void BrReset(void) {
  // clean previous VM if any
  if (berry.vm != nullptr) {
    be_vm_delete(berry.vm);
    berry.vm = nullptr;
  }

  int32_t ret_code1, ret_code2;
  bool berry_init_ok = false;
  do {    
    berry.vm = be_vm_new(); /* create a virtual machine instance */
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_BERRY "Berry VM created, RAM used=%u"), be_gc_memcount(berry.vm));

    // Register functions
    be_regfunc(berry.vm, PSTR("log"), l_logInfo);
    be_regfunc(berry.vm, PSTR("save"), l_save);

    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_BERRY "Berry function registered, RAM used=%u"), be_gc_memcount(berry.vm));

    ret_code1 = be_loadstring(berry.vm, berry_prog);
    if (ret_code1 != 0) {
      AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_BERRY "ERROR: be_loadstring [%s] %s"), be_tostring(berry.vm, -2), be_tostring(berry.vm, -1));
      be_pop(berry.vm, 2);
      break;
    }
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_BERRY "Berry code loaded, RAM used=%u"), be_gc_memcount(berry.vm));
    ret_code2 = be_pcall(berry.vm, 0);
    if (ret_code1 != 0) {
      AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_BERRY "ERROR: be_pcall [%s] %s"), be_tostring(berry.vm, -2), be_tostring(berry.vm, -1));
      be_pop(berry.vm, 2);
      break;
    }
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_BERRY "Berry code ran, RAM used=%u"), be_gc_memcount(berry.vm));
    be_pop(berry.vm, 1);

    be_gc_collect(berry.vm);
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_BERRY "Berry initialized, RAM used=%u"), callBerryGC());
    // AddLog(LOG_LEVEL_INFO, PSTR("Delete Berry VM"));
    // be_vm_delete(vm);
    // AddLog(LOG_LEVEL_INFO, PSTR("After Berry"));

    berry_init_ok = true;
  } while (0);

  if (!berry_init_ok) {
    // free resources
    if (berry.vm != nullptr) {
      be_vm_delete(berry.vm);
      berry.vm = nullptr;
    }
  }
}

/*********************************************************************************************\
 * Tasmota Commands
\*********************************************************************************************/
//
// Command `BrRun`
//
void CmndBrRun(void) {
  int32_t ret_code;
  const char * ret_type, * ret_val;

  if (berry.vm == nullptr) { ResponseCmndChar_P(PSTR(D_BR_NOT_STARTED)); return; }

  char br_cmd[XdrvMailbox.data_len+12];
  // encapsulate into a function, copied from `be_repl.c` / `try_return()`
  snprintf_P(br_cmd, sizeof(br_cmd), PSTR("return (%s)"), XdrvMailbox.data);

  checkBeTop();
  do {
    // First try with the `return ()` wrapper
    ret_code = be_loadbuffer(berry.vm, PSTR("input"), br_cmd, strlen(br_cmd));
    if (be_getexcept(berry.vm, ret_code) == BE_SYNTAX_ERROR) {
      be_pop(berry.vm, 2);    // remove exception values
      // if fails, try the direct command
      ret_code = be_loadbuffer(berry.vm, PSTR("input"), XdrvMailbox.data, strlen(XdrvMailbox.data));
    }
    if (0 != ret_code) break;

    ret_code = be_pcall(berry.vm, 0);     // execute code
  } while (0);

  if (0 == ret_code) {
    // code taken from REPL, look first at top, and if nil, look at return value
    if (be_isnil(berry.vm, 0)) {
      ret_val = be_tostring(berry.vm, -1);
    } else {
      ret_val = be_tostring(berry.vm, 0);
    }
    Response_P("{\"" D_PRFX_BR "\":\"%s\"}", ret_val);    // can't use XdrvMailbox.command as it may have been overwritten by subcommand
    be_pop(berry.vm, 1);
  } else {
    Response_P(PSTR("{\"" D_PRFX_BR "\":\"[%s] %s\"}"), be_tostring(berry.vm, -2), be_tostring(berry.vm, -1));
    be_pop(berry.vm, 2);
  }

  checkBeTop();
}

//
// Command `BrReset`
//
void CmndBrReset(void) {
  if (berry.vm == nullptr) { ResponseCmndChar_P(PSTR(D_BR_NOT_STARTED)); return; }

  BrReset();
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xdrv52(uint8_t function)
{
  bool result = false;

  switch (function) {
    //case FUNC_PRE_INIT:
    case FUNC_INIT:
      BrReset();
      break;
    case FUNC_EVERY_50_MSECOND:
      callBerryFunctionVoid(PSTR("_run_deferred"));
      break;
    case FUNC_EVERY_100_MSECOND:
      callBerryFunctionVoid(PSTR("every_100ms"));
      break;
    case FUNC_EVERY_SECOND:
      callBerryFunctionVoid(PSTR("every_second"));
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kBrCommands, BerryCommand);
      if (!result) {
        result = callBerryCommand();
      }
      break;
    // case FUNC_SET_POWER:
    //   break;
    case FUNC_RULES_PROCESS:
      result = callBerryRule();
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_ADD_BUTTON:
      break;
    case FUNC_WEB_ADD_MAIN_BUTTON:

      break;
    case FUNC_WEB_ADD_HANDLER:
      break;
#endif // USE_WEBSERVER
    case FUNC_SAVE_BEFORE_RESTART:
      break;
    case FUNC_MQTT_DATA:
      // callBerryMqttData();
      break;
    case FUNC_WEB_SENSOR:
      break;

    case FUNC_JSON_APPEND:
      break;

    case FUNC_BUTTON_PRESSED:
      break;

    case FUNC_LOOP:
      break;

  }
  return result;
}

#endif  // USE_BERRY
