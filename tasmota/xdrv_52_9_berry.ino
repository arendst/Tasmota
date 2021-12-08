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
#include "be_vm.h"
#include "ZipReadFS.h"

extern "C" {
  extern void be_load_custom_libs(bvm *vm);
  extern void be_tracestack(bvm *vm);
}

const char kBrCommands[] PROGMEM = D_PRFX_BR "|"    // prefix
  D_CMND_BR_RUN
  ;

void (* const BerryCommand[])(void) PROGMEM = {
  CmndBrRun,
  };

int32_t callBerryEventDispatcher(const char *type, const char *cmd, int32_t idx, const char *payload, uint32_t data_len = 0);

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
 * Memory handler
 * Use PSRAM if available
\*********************************************************************************************/
extern "C" {
  void *berry_malloc(uint32_t size);
  void *berry_realloc(void *ptr, size_t size);
#ifdef USE_BERRY_PSRAM
  void *berry_malloc(uint32_t size) {
    return special_malloc(size);
  }
  void *berry_realloc(void *ptr, size_t size) {
    return special_realloc(ptr, size);
  }
  void *berry_calloc(size_t num, size_t size) {
    return special_calloc(num, size);
  }
#else
  void *berry_malloc(uint32_t size) {
    return malloc(size);
  }
  void *berry_realloc(void *ptr, size_t size) {
    return realloc(ptr, size);
  }
  void *berry_calloc(size_t num, size_t size) {
    return calloc(num, size);
  }
#endif // USE_BERRY_PSRAM

  void berry_free(void *ptr) {
    free(ptr);
  }
}


/*********************************************************************************************\
 * Handlers for Berry calls and async
 *
\*********************************************************************************************/
// // call a function (if exists) of type void -> void

// If event == nullptr, then take XdrvMailbox.data
bool callBerryRule(const char *event, bool teleperiod) {
  if (berry.rules_busy) { return false; }
  berry.rules_busy = true;
  char * json_event = XdrvMailbox.data;
  bool serviced = false;
  serviced = callBerryEventDispatcher(teleperiod ? "tele" : "rule", nullptr, 0, event ? event : XdrvMailbox.data);
  berry.rules_busy = false;
  return serviced;     // TODO event not handled
}

size_t callBerryGC(void) {
  return callBerryEventDispatcher(PSTR("gc"), nullptr, 0, nullptr);
}

void BerryDumpErrorAndClear(bvm *vm, bool berry_console);
void BerryDumpErrorAndClear(bvm *vm, bool berry_console) {
  int32_t top = be_top(vm);
  // check if we have two strings for an Exception
  if (top >= 2 && be_isstring(vm, -1) && be_isstring(vm, -2)) {
    if (berry_console) {
      berry_log_C(PSTR(D_LOG_BERRY "Exception> '%s' - %s"), be_tostring(berry.vm, -2), be_tostring(berry.vm, -1));
      be_tracestack(vm);
      top = be_top(vm);   // update top after dump
    } else {
      AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_BERRY "Exception> '%s' - %s"), be_tostring(berry.vm, -2), be_tostring(berry.vm, -1));
      be_tracestack(vm);
    }
  } else {
    be_dumpstack(vm);
  }
  be_pop(vm, top);
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

/*
// Call a method of a global object, with n args
// Before: stack must containt n args
// After: stack contains return value or nil if something wrong (args removes)
// returns true is successful, false if object or method not found
bool callMethodObjectWithArgs(const char * objname, const char * method, size_t argc) {
  if (nullptr == berry.vm) { return false; }
  int32_t top = be_top(berry.vm);
  // stacks contains n x arg
  be_getglobal(berry.vm, objname);
  // stacks contains n x arg + object
  if (!be_isnil(berry.vm, -1)) {
    be_getmethod(berry.vm, -1, method);
  // stacks contains n x arg + object + method
    if (!be_isnil(berry.vm, -1)) {
      // reshuffle the entire stack since we want: method + object + n x arg
      be_pushvalue(berry.vm, -1); // add instance as first arg
      // stacks contains n x arg + object + method + method
      be_pushvalue(berry.vm, -3); // add instance as first arg
      // stacks contains n x arg + object + method + method + object
      // now move args 2 slots up to make room for method and object
      for (uint32_t i = 1; i <= argc; i++) {
        be_moveto(berry.vm, -4 - i, -2 - i);
      }
      // stacks contains free + free + n x arg + method + object
      be_moveto(berry.vm, -2, -4 - argc);
      be_moveto(berry.vm, -1, -3 - argc);
      // stacks contains method + object + n x arg + method + object
      be_pop(berry.vm, 2);
      // stacks contains method + object + n x arg
      be_pcall(berry.vm, argc + 1);
      // stacks contains return_val + object + n x arg
      be_pop(berry.vm, argc + 1);
      // stacks contains return_val
      return true;
    }
    be_pop(berry.vm, 1);  // remove method
    // stacks contains n x arg + object
  }
  // stacks contains n x arg + object
  be_pop(berry.vm, argc + 1); // clear stack
  be_pushnil(berry.vm); // put nil object
  return false;
}
*/


// call the event dispatcher from Tasmota object
// if data_len is non-zero, the event is also sent as raw `bytes()` object because the string may lose data
int32_t callBerryEventDispatcher(const char *type, const char *cmd, int32_t idx, const char *payload, uint32_t data_len) {
  int32_t ret = 0;
  bvm *vm = berry.vm;

  if (nullptr == vm) { return ret; }
  checkBeTop();
  be_getglobal(vm, PSTR("tasmota"));
  if (!be_isnil(vm, -1)) {
    be_getmethod(vm, -1, PSTR("event"));
    if (!be_isnil(vm, -1)) {
      be_pushvalue(vm, -2); // add instance as first arg
      be_pushstring(vm, type != nullptr ? type : "");
      be_pushstring(vm, cmd != nullptr ? cmd : "");
      be_pushint(vm, idx);
      be_pushstring(vm, payload != nullptr ? payload : "");  // empty json
      BrTimeoutStart();
      if (data_len > 0) {
        be_pushbytes(vm, payload, data_len);    // if data_len is set, we also push raw bytes
        ret = be_pcall(vm, 6);   // 6 arguments
        be_pop(vm, 1);
      } else {
        ret = be_pcall(vm, 5);   // 5 arguments
      }
      BrTimeoutReset();
      if (ret != 0) {
        BerryDumpErrorAndClear(vm, false);  // log in Tasmota console only
        return ret;
      }
      be_pop(vm, 5);
      if (be_isint(vm, -1) || be_isbool(vm, -1)) {
        if (be_isint(vm, -1)) { ret = be_toint(vm, -1); }
        if (be_isbool(vm, -1)) { ret = be_tobool(vm, -1); }
      }
    }
    be_pop(vm, 1);  // remove method
  }
  be_pop(vm, 1);  // remove instance object
  checkBeTop();
  return ret;
}

/*********************************************************************************************\
 * VM Observability
\*********************************************************************************************/
void BerryObservability(bvm *vm, int event...);
void BerryObservability(bvm *vm, int event...) {
  va_list param;
  va_start(param, event);
  static int32_t vm_usage = 0;
  static uint32_t gc_time = 0;

  switch (event)  {
    case BE_OBS_GC_START:
      {
        gc_time = millis();
        vm_usage = va_arg(param, int32_t);
      }
      break;
    case BE_OBS_GC_END:
      {
        int32_t vm_usage2 = va_arg(param, int32_t);
        uint32_t gc_elapsed = millis() - gc_time;
        uint32_t vm_scanned = va_arg(param, uint32_t);
        uint32_t vm_freed = va_arg(param, uint32_t);
        AddLog(LOG_LEVEL_DEBUG, D_LOG_BERRY "GC from %i to %i bytes, objects freed %i/%i (in %d ms)",
                                vm_usage, vm_usage2, vm_freed, vm_scanned, gc_elapsed);
        // make new threshold tighter when we reach high memory usage
        if (!UsePSRAM() && vm->gc.threshold > 20*1024) {
          vm->gc.threshold = vm->gc.usage + 10*1024;    // increase by only 10 KB
        }
      }
      break;
    case BE_OBS_STACK_RESIZE_START:
      {
        int32_t stack_before = va_arg(param, int32_t);
        int32_t stack_after = va_arg(param, int32_t);
        AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_BERRY "Stack resized from %i to %i bytes"), stack_before, stack_after);
      }
      break;
    case BE_OBS_VM_HEARTBEAT:
      {
        // AddLog(LOG_LEVEL_INFO, ">>>: Heartbeat now=%i timeout=%i", millis(), berry.timeout);
        if (berry.timeout) {
          if (TimeReached(berry.timeout)) {
            be_raise(vm, "timeout_error", "Berry code running for too long");
          }
        }
      }
      break;
    default:
      break;
  }
  va_end(param);
}

/*********************************************************************************************\
 * Adde Berry metrics to teleperiod
\*********************************************************************************************/
void BrShowState(void);
void BrShowState(void) {
  // trigger a gc first
  be_gc_collect(berry.vm);
  ResponseAppend_P(PSTR(",\"Berry\":{\"HeapUsed\":%u,\"Objects\":%u}"),
    berry.vm->gc.usage / 1024, berry.vm->counter_gc_kept);
}

/*********************************************************************************************\
 * VM Init
\*********************************************************************************************/
void BerryInit(void) {
  // clean previous VM if any
  if (berry.vm != nullptr) {
    be_vm_delete(berry.vm);
    berry.vm = nullptr;
  }

  int32_t ret_code1, ret_code2;
  bool berry_init_ok = false;
  do {
    berry.vm = be_vm_new(); /* create a virtual machine instance */
    be_set_obs_hook(berry.vm, &BerryObservability);  /* attach observability hook */
    comp_set_named_gbl(berry.vm);  /* Enable named globals in Berry compiler */
    comp_set_strict(berry.vm);  /* Enable strict mode in Berry compiler, equivalent of `import strict` */

    be_load_custom_libs(berry.vm);  // load classes and modules

    // Set the GC threshold to 3584 bytes to avoid the first useless GC
    berry.vm->gc.threshold = 3584;

    ret_code1 = be_loadstring(berry.vm, berry_prog);
    if (ret_code1 != 0) {
      BerryDumpErrorAndClear(berry.vm, false);
      break;
    }
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_BERRY "Berry code loaded, RAM used=%u"), be_gc_memcount(berry.vm));
    ret_code2 = be_pcall(berry.vm, 0);
    if (ret_code1 != 0) {
      BerryDumpErrorAndClear(berry.vm, false);
      break;
    }
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_BERRY "Berry code ran, RAM used=%u"), be_gc_memcount(berry.vm));
    if (be_top(berry.vm) > 1) {
      BerryDumpErrorAndClear(berry.vm, false);
    } else {
      be_pop(berry.vm, 1);
    }

    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_BERRY "Berry initialized, RAM used=%u"), callBerryGC());
    berry_init_ok = true;

    // we generate a synthetic event `autoexec` 
    callBerryEventDispatcher(PSTR("preinit"), nullptr, 0, nullptr);
    
    // Run pre-init
    BrLoad("preinit.be");    // run 'preinit.be' if present
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
 * Execute a script in Flash file-system
 *
 * Two options supported:
 *   berry_preinit: load "preinit.be" to configure the device before driver pre-init and init
 *                  (typically I2C drivers, and AXP192/AXP202 configuration)
 *   berry_autoexec: load "autoexec.be" once all drivers are initialized
\*********************************************************************************************/
void BrLoad(const char * script_name) {
  if (berry.vm == nullptr || TasmotaGlobal.no_autoexec) { return; }   // abort is berry is not running, or bootloop prevention kicked in

  int32_t ret_code1, ret_code2;
  bool berry_init_ok = false;

  be_getglobal(berry.vm, PSTR("load"));
  if (!be_isnil(berry.vm, -1)) {
    be_pushstring(berry.vm, script_name);

    BrTimeoutStart();
    if (be_pcall(berry.vm, 1) != 0) {
      BerryDumpErrorAndClear(berry.vm, false);
      return;
    }
    BrTimeoutReset();
    bool loaded = be_tobool(berry.vm, -2);  // did it succeed?
    be_pop(berry.vm, 2);
    if (loaded) {
      AddLog(LOG_LEVEL_INFO, D_LOG_BERRY "successfully loaded '%s'", script_name);
    } else {
      AddLog(LOG_LEVEL_INFO, D_LOG_BERRY "no '%s'", script_name);
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

    BrTimeoutStart();
    ret_code = be_pcall(berry.vm, 0);     // execute code
    BrTimeoutReset();
  } while (0);

  if (0 == ret_code) {
    // AddLog(LOG_LEVEL_INFO, "run: top=%d", be_top(berry.vm));
    // AddLog(LOG_LEVEL_INFO, "run: type(1)=%s", be_typename(berry.vm, 1));
    // AddLog(LOG_LEVEL_INFO, "run: type(2)=%s", be_typename(berry.vm, 2));

    // code taken from REPL, look first at top, and if nil, look at return value
    // if (!be_isnil(berry.vm, 1)) {
      ret_val = be_tostring(berry.vm, 1);
    // } else {
    //   ret_val = be_tostring(berry.vm, 2);
    // }
    Response_P("{\"" D_PRFX_BR "\":\"%s\"}", EscapeJSONString(ret_val).c_str());    // can't use XdrvMailbox.command as it may have been overwritten by subcommand
    be_pop(berry.vm, 1);
  } else {
    Response_P(PSTR("{\"" D_PRFX_BR "\":\"[%s] %s\"}"), EscapeJSONString(be_tostring(berry.vm, -2)).c_str(), EscapeJSONString(be_tostring(berry.vm, -1)).c_str());
    be_pop(berry.vm, 2);
  }

  checkBeTop();
}

/*********************************************************************************************\
 * Berry console
\*********************************************************************************************/
#ifdef USE_WEBSERVER

void BrREPLRun(char * cmd) {
  if (berry.vm == nullptr) { return; }

  size_t cmd_len = strlen(cmd);
  size_t cmd2_len = cmd_len + 12;
  char * cmd2 = (char*) malloc(cmd2_len);
  do {
    int32_t ret_code;

    snprintf_P(cmd2, cmd2_len, PSTR("return (%s)"), cmd);
    ret_code = be_loadbuffer(berry.vm, PSTR("input"), cmd2, strlen(cmd2));
    // AddLog(LOG_LEVEL_INFO, PSTR(">>>> be_loadbuffer cmd2 '%s', ret=%i"), cmd2, ret_code);
    if (be_getexcept(berry.vm, ret_code) == BE_SYNTAX_ERROR) {
      be_pop(berry.vm, 2);    // remove exception values
      // if fails, try the direct command
      ret_code = be_loadbuffer(berry.vm, PSTR("input"), cmd, cmd_len);
      // AddLog(LOG_LEVEL_INFO, PSTR(">>>> be_loadbuffer cmd1 '%s', ret=%i"), cmd, ret_code);
    }
    if (0 == ret_code) {    // code is ready to run
      BrTimeoutStart();
      ret_code = be_pcall(berry.vm, 0);     // execute code
      BrTimeoutReset();
      // AddLog(LOG_LEVEL_INFO, PSTR(">>>> be_pcall ret=%i"), ret_code);
      if (0 == ret_code) {
        if (!be_isnil(berry.vm, 1)) {
          const char * ret_val = be_tostring(berry.vm, 1);
          berry.log.addString(ret_val, nullptr, "\n");
          // AddLog(LOG_LEVEL_INFO, PSTR(">>> %s"), ret_val);
        }
        be_pop(berry.vm, 1);
      }
    }
    if (BE_EXCEPTION == ret_code) {
      BerryDumpErrorAndClear(berry.vm, true);
      // be_dumpstack(berry.vm);
      // char exception_s[120];
      // ext_snprintf_P(exception_s, sizeof(exception_s), PSTR("%s: %s"), be_tostring(berry.vm, -2), be_tostring(berry.vm, -1));
      // berry.log.addString(exception_s, nullptr, "\n");
      // // AddLog(LOG_LEVEL_INFO, PSTR(">>> %s"), exception_s);
      // be_pop(berry.vm, 2);
    }
  } while(0);

  if (cmd2 != nullptr) {
    free(cmd2);
    cmd2 = nullptr;
  }
  checkBeTop();
}

const char HTTP_SCRIPT_BERRY_CONSOLE[] PROGMEM =
  "var sn=0,id=0,ft,ltm=%d;"                      // Scroll position, Get most of weblog initially
  // Console command history
  "var hc=[],cn=0;"                       // hc = History commands, cn = Number of history being shown

  "function l(p){"                        // Console log and command service
    "var c,cc,o='';"
    "clearTimeout(lt);"
    "clearTimeout(ft);"
    "t=eb('t1');"
    "if(p==1){"
      "c=eb('c1');"                       // Console command id
      "cc=c.value.trim();"
      "if(cc){"
        "o='&c1='+encodeURIComponent(cc);"
        "hc.length>19&&hc.pop();"
        "hc.unshift(cc);"
        "cn=0;"
      "}"
      "c.value='';"
      "t.scrollTop=99999;"
      "sn=t.scrollTop;"
    "}"
    "if(t.scrollTop>=sn){"                // User scrolled back so no updates
      "if(x!=null){x.abort();}"           // Abort if no response within 2 seconds (happens on restart 1)
      "x=new XMLHttpRequest();"
      "x.onreadystatechange=function(){"
        "if(x.readyState==4&&x.status==200){"
          "var d,t1;"
          "d=x.responseText.split(/" BERRY_CONSOLE_CMD_DELIMITER "/);"  // Field separator
          "var d1=d.shift();"
          "if(d1){"
            "t1=document.createElement('div');"
            "t1.classList.add('br1');"
            "t1.innerText=d1;"
            "t.appendChild(t1);"
          "}"
          "d1=d.shift();"
          "if(d1){"
            "t1=document.createElement('div');"
            "t1.classList.add('br2');"
            "t1.innerText=d1;"
            "t.appendChild(t1);"
          "}"
          "t.scrollTop=99999;"
          "sn=t.scrollTop;"
          "clearTimeout(ft);"
          "lt=setTimeout(l,ltm);" // webrefresh timer....
        "}"
      "};"
      "x.open('GET','bc?c2='+id+o,true);"  // Related to Webserver->hasArg("c2") and WebGetArg("c2", stmp, sizeof(stmp))
      "x.send();"
      "ft=setTimeout(l,20000);" // fail timeout, triggered 20s after asking for XHR
    "}else{"
      "lt=setTimeout(l,ltm);" // webrefresh timer....
    "}"
    "c1.focus();"
    "return false;"
  "}"
  "wl(l);"                                // Load initial console text
;                               // Add console command key eventlistener after name has been synced with id (= wl(jd))

const char HTTP_SCRIPT_BERRY_CONSOLE2[] PROGMEM =
  // // Console command history
  // "var hc=[],cn=0;"                       // hc = History commands, cn = Number of history being shown
  "var pc=0;"                                // pc = previous char
  "function h(){"
//    "if(!(navigator.maxTouchPoints||'ontouchstart'in document.documentElement)){eb('c1').autocomplete='off';}"  // No touch so stop browser autocomplete
    "eb('c1').addEventListener('keydown',function(e){"
      "var b=eb('c1'),c=e.keyCode;"       // c1 = Console command id
      "if((38==c||40==c)&&0==this.selectionStart&&0==this.selectionEnd){"
        "b.autocomplete='off';"
        "e.preventDefault();"
        "38==c?(++cn>hc.length&&(cn=hc.length),b.value=hc[cn-1]||''):"   // ArrowUp
        "40==c?(0>--cn&&(cn=0),b.value=hc[cn-1]||''):"                   // ArrowDown
        "0;"
        "this.selectionStart=this.selectionEnd=0;"
      "}"  // ArrowUp or ArrowDown must be a keyboard so stop browser autocomplete
      "if(c==13&&pc==13){"
        "e.preventDefault();"             // prevent 'enter' from being inserted
        "l(1);"
      "}"
      "if(c==9){"
        "e.preventDefault();"
        "var start=this.selectionStart;"
        "var end=this.selectionEnd;"
        // set textarea value to: text before caret + tab + text after caret
        "this.value=this.value.substring(0, start)+\"  \"+this.value.substring(end);"
        // put caret at right position again
        "this.selectionStart=this.selectionEnd=start + 1;"
      "}"
      "pc=c;"                                                          // record previous key
      // "13==c&&(hc.length>19&&hc.pop(),hc.unshift(b.value),cn=0)"       // Enter, 19 = Max number -1 of commands in history
    "});"
  "}"
  "wl(h);";                               // Add console command key eventlistener after name has been synced with id (= wl(jd))

const char HTTP_BERRY_STYLE_CMND[] PROGMEM =
  "<style>"
  ".br1{"   // berry output
    "border-left:dotted 2px #860;"
    "margin-bottom:4px;"
    "margin-top:4px;"
    "padding:1px 5px 1px 18px;"
  "}"
  ".br2{"   // user input
    "padding:0px 5px 0px 5px;"
    "color:#faffff;"
  "}"
  ".br0{"
    // "-moz-appearance: textfield-multiline;"
    // "-webkit-appearance: textarea;"
    "font:medium -moz-fixed;"
    "font:-webkit-small-control;"
    "box-sizing:border-box;"
    "width:100%;"
    "overflow:auto;"
    "resize:vertical;"
    "font-family:monospace;"
    "overflow:auto;"
    "font-size:1em;"
  "}"
  ".bro{"
    // "-moz-appearance: textfield-multiline;"
    // "-webkit-appearance: textarea;"
    "border:1px solid gray;"
    "height:250px;"
    "padding:2px;"
    "background:#222;"
    "color:#fb1;"
    "white-space:pre;"
    "padding:2px 5px 2px 5px;"
  "}"
  ".bri{"
    // "-moz-appearance: textfield-multiline;"
    // "-webkit-appearance: textarea;"
    "border:1px solid gray;"
    "height:60px;"
    "padding:5px;"
    "color:#000000;background:#faffff"
  "}"
  "</style>"
  ;

const char HTTP_BERRY_FORM_CMND[] PROGMEM =
  "<br>"
  "<div contenteditable='false' class='br0 bro' readonly id='t1' cols='340' wrap='off'>"
    "<div class='br1'>Welcome to the Berry Scripting console. "
      "Check the <a href='https://tasmota.github.io/docs/Berry/' target='_blank'>documentation</a>."
    "</div>"
  "</div>"
  // "<textarea readonly id='t1' cols='340' wrap='off'></textarea>"
  // "<br><br>"
  "<form method='get' id='fo' onsubmit='return l(1);'>"
  "<textarea id='c1' class='br0 bri' rows='4' cols='340' wrap='soft' autofocus required></textarea>"
  // "<input id='c1' class='bri' type='text' rows='5' placeholder='" D_ENTER_COMMAND "' autofocus><br>"
  // "<input type='submit' value=\"Run code (or press 'Enter' twice)\">"
  "<button type='submit'>Run code (or press 'Enter' twice)</button>"
  "</form>";

const char HTTP_BTN_BERRY_CONSOLE[] PROGMEM =
  "<p><form action='bc' method='get'><button>Berry Scripting console</button></form></p>";


void HandleBerryConsoleRefresh(void)
{
  String svalue = Webserver->arg(F("c1"));

  svalue.trim();
  if (svalue.length()) {
    berry.log.reset();          // clear all previous logs
    berry.repl_active = true;   // start recording
    // AddLog(LOG_LEVEL_INFO, PSTR("BRY: received command %s"), svalue.c_str());
    berry.log.addString(svalue.c_str(), nullptr, BERRY_CONSOLE_CMD_DELIMITER);

    // Call berry
    BrREPLRun((char*)svalue.c_str());
    berry.repl_active = false;   // don't record further
  }

  WSContentBegin(200, CT_PLAIN);

  if (!berry.log.isEmpty()) {

    WSContentFlush();

    for (auto & l: berry.log.log) {
      _WSContentSend(l.getBuffer());
    }

    berry.log.reset();
  }
  WSContentEnd();
}

void HandleBerryConsole(void)
{
  if (!HttpCheckPriviledgedAccess()) { return; }
  // int i=16;
  // // AddLog(LOG_LEVEL_INFO, PSTR("Size = %d %d"), sizeof(LList_elt<char[12]>), sizeof(LList_elt<char[0]>)+12);
  // LList_elt<char[0]> * elt = (LList_elt<char[0]>*) ::operator new(sizeof(LList_elt<char[0]>) + 12);

  if (Webserver->hasArg(F("c2"))) {      // Console refresh requested
    HandleBerryConsoleRefresh();
    return;
  }

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP "Berry " D_CONSOLE));

  WSContentStart_P(PSTR("Berry " D_CONSOLE));
  WSContentSend_P(HTTP_SCRIPT_BERRY_CONSOLE, Settings->web_refresh);
  WSContentSend_P(HTTP_SCRIPT_BERRY_CONSOLE2);
  WSContentSendStyle();
  WSContentFlush();
  _WSContentSend(HTTP_BERRY_STYLE_CMND);
  _WSContentSend(HTTP_BERRY_FORM_CMND);
  WSContentSpaceButton(BUTTON_MANAGEMENT);
  WSContentStop();
}

// void HandleBerryConsoleRefresh(void)
// {
//   String svalue = Webserver->arg(F("c1"));
//   if (svalue.length() && (svalue.length() < MQTT_MAX_PACKET_SIZE)) {
//     // TODO run command and store result
//     // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_COMMAND "%s"), svalue.c_str());
//     // ExecuteWebCommand((char*)svalue.c_str(), SRC_WEBCONSOLE);
//   }

//   char stmp[8];
//   WebGetArg(PSTR("c2"), stmp, sizeof(stmp));
//   uint32_t index = 0;                // Initial start, dump all
//   if (strlen(stmp)) { index = atoi(stmp); }

//   WSContentBegin(200, CT_PLAIN);
//   WSContentSend_P(PSTR("%d}1%d}1"), TasmotaGlobal.log_buffer_pointer, Web.reset_web_log_flag);
//   if (!Web.reset_web_log_flag) {
//     index = 0;
//     Web.reset_web_log_flag = true;
//   }
//   bool cflg = (index);
//   char* line;
//   size_t len;
//   while (GetLog(Settings->weblog_level, &index, &line, &len)) {
//     if (cflg) { WSContentSend_P(PSTR("\n")); }
//     WSContentFlush();
//     Webserver->sendContent(line, len -1);
//     cflg = true;
//   }
//   WSContentSend_P(PSTR("}1"));
//   WSContentEnd();
// }
#endif // USE_WEBSERVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xdrv52(uint8_t function)
{
  bool result = false;

  switch (function) {
    // case FUNC_PRE_INIT: // we start Berry in pre_init so that other modules can call Berry in their init methods
    // // case FUNC_INIT:
    //   BerryInit();
    //   break;
    case FUNC_LOOP:
      if (!berry.autoexec_done) {
        // we generate a synthetic event `autoexec` 
        callBerryEventDispatcher(PSTR("autoexec"), nullptr, 0, nullptr);

        BrLoad("autoexec.be");   // run autoexec.be at first tick, so we know all modules are initialized
        berry.autoexec_done = true;
      }
      break;

    // Berry wide commands and events
    case FUNC_RULES_PROCESS:
      result = callBerryRule(nullptr, false);
      break;
    case FUNC_TELEPERIOD_RULES_PROCESS:
      result = callBerryRule(nullptr, true);
      break;
    case FUNC_MQTT_DATA:
      result = callBerryEventDispatcher(PSTR("mqtt_data"), XdrvMailbox.topic, 0, XdrvMailbox.data, XdrvMailbox.data_len);
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kBrCommands, BerryCommand);
      if (!result) {
        result = callBerryEventDispatcher(PSTR("cmd"), XdrvMailbox.topic, XdrvMailbox.index, XdrvMailbox.data);
      }
      break;

    // Module specific events
    case FUNC_EVERY_50_MSECOND:
      callBerryEventDispatcher(PSTR("every_50ms"), nullptr, 0, nullptr);
      break;
    case FUNC_EVERY_100_MSECOND:
      callBerryEventDispatcher(PSTR("every_100ms"), nullptr, 0, nullptr);
      break;
    case FUNC_EVERY_SECOND:
      callBerryEventDispatcher(PSTR("every_second"), nullptr, 0, nullptr);
      break;
    case FUNC_SET_DEVICE_POWER:
      result = callBerryEventDispatcher(PSTR("set_power_handler"), nullptr, XdrvMailbox.index, nullptr);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_ADD_CONSOLE_BUTTON:
      if (XdrvMailbox.index) {
        XdrvMailbox.index++;
      } else {
        WSContentSend_P(HTTP_BTN_BERRY_CONSOLE);
        callBerryEventDispatcher(PSTR("web_add_button"), nullptr, 0, nullptr);
        callBerryEventDispatcher(PSTR("web_add_console_button"), nullptr, 0, nullptr);
      }
      break;
    case FUNC_WEB_ADD_MAIN_BUTTON:
      callBerryEventDispatcher(PSTR("web_add_main_button"), nullptr, 0, nullptr);
      break;
    case FUNC_WEB_ADD_MANAGEMENT_BUTTON:
      callBerryEventDispatcher(PSTR("web_add_management_button"), nullptr, 0, nullptr);
      break;
    case FUNC_WEB_ADD_BUTTON:
      callBerryEventDispatcher(PSTR("web_add_config_button"), nullptr, 0, nullptr);
      break;
    case FUNC_WEB_ADD_HANDLER:
      callBerryEventDispatcher(PSTR("web_add_handler"), nullptr, 0, nullptr);
      WebServer_on(PSTR("/bc"), HandleBerryConsole);
      break;
#endif // USE_WEBSERVER
    case FUNC_SAVE_BEFORE_RESTART:
      callBerryEventDispatcher(PSTR("save_before_restart"), nullptr, 0, nullptr);
      break;
    case FUNC_WEB_SENSOR:
      callBerryEventDispatcher(PSTR("web_sensor"), nullptr, 0, nullptr);
      break;

    case FUNC_JSON_APPEND:
      callBerryEventDispatcher(PSTR("json_append"), nullptr, 0, nullptr);
      break;

    case FUNC_BUTTON_PRESSED:
      callBerryEventDispatcher(PSTR("button_pressed"), nullptr, 0, nullptr);
      break;


  }
  return result;
}

#endif  // USE_BERRY
