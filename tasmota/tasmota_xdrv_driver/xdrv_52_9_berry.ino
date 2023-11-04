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
#include "berry_tasmota.h"
#ifdef USE_MATTER_DEVICE
  #include "berry_matter.h"
#endif
#include "be_vm.h"
#include "ZipReadFS.h"
#include "ccronexpr.h"

extern "C" {
  extern void be_load_custom_libs(bvm *vm);
  extern void be_tracestack(bvm *vm);
}

const char kBrCommands[] PROGMEM = D_PRFX_BR "|"    // prefix
  D_CMND_BR_RUN "|" D_CMND_BR_RESTART
  ;

void (* const BerryCommand[])(void) PROGMEM = {
  CmndBrRun, CmndBrRestart
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
    AddLog(LOG_LEVEL_DEBUG, D_LOG_BERRY "Error be_top is non zero=%d", top);
  }
}

/*********************************************************************************************\
 * Memory handler
 * Use PSRAM if available
\*********************************************************************************************/
extern "C" {
  void *berry_malloc(size_t size);
  void *berry_realloc(void *ptr, size_t size);
#ifdef USE_BERRY_PSRAM
  void *berry_malloc(size_t size) {
    return special_malloc(size);
  }
  void *berry_realloc(void *ptr, size_t size) {
    return special_realloc(ptr, size);
  }
  void *berry_calloc(size_t num, size_t size) {
    return special_calloc(num, size);
  }
#else
  void *berry_malloc(size_t size) {
    return malloc(size);
  }
  void *berry_realloc(void *ptr, size_t size) {
    return realloc(ptr, size);
  }
  void *berry_calloc(size_t num, size_t size) {
    return calloc(num, size);
  }
#endif // USE_BERRY_PSRAM


  void *berry_malloc32(size_t size) {
  #ifdef USE_BERRY_IRAM
    return special_malloc32(size);
  #else
    return NULL;    /* return NULL to indicate that IRAM is not enabled */
  #endif
  }

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
  bool save_rules_busy = berry.rules_busy;
  bool exec_rule = !save_rules_busy;       // true if the rule is executed, false if we only record the value
  // if (berry.rules_busy) { return false; }
  berry.rules_busy = true;
  bool serviced = false;
  serviced = callBerryEventDispatcher(teleperiod ? "tele" : "rule", nullptr, exec_rule, event ? event : XdrvMailbox.data);
  berry.rules_busy = save_rules_busy;
  return serviced;     // TODO event not handled
}

size_t callBerryGC(void) {
  return callBerryEventDispatcher(PSTR("gc"), nullptr, 0, nullptr);
}

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
        be_error_pop_all(berry.vm);             // clear Berry stack
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

// Simplified version of event loop. Just call `tasmota.fast_loop()`
// `every_5ms` is a flag to wait at least 5ms between calss to `tasmota.fast_loop()`
void callBerryFastLoop(bool every_5ms) {
  static uint32_t fast_loop_last_call = 0;
  bvm *vm = berry.vm;

  if (nullptr == vm) { return; }

  uint32_t now = millis();
  if (every_5ms) {
    if (!TimeReached(fast_loop_last_call + USE_BERRY_FAST_LOOP_SLEEP_MS /* 5ms */)) { return; }
  }
  fast_loop_last_call = now;

  // TODO - can we make this dereferencing once for all?
  if (be_getglobal(vm, "tasmota")) {
    if (be_getmethod(vm, -1, "fast_loop")) {
      be_pushvalue(vm, -2); // add instance as first arg
      BrTimeoutStart();
      int32_t ret = be_pcall(vm, 1);
      if (ret != 0) {
        be_error_pop_all(berry.vm);             // clear Berry stack
      }
      BrTimeoutReset();
      be_pop(vm, 1);
    }
    be_pop(vm, 1);  // remove method
  }
  be_pop(vm, 1);  // remove instance object
  be_pop(vm, be_top(vm));   // clean
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
    case BE_OBS_PCALL_ERROR:    // error after be_pcall
      {
        int32_t top = be_top(vm);
        // check if we have two strings for an Exception
        if (top >= 2 && be_isstring(vm, -1) && be_isstring(vm, -2)) {
          berry_log_C(PSTR(D_LOG_BERRY "Exception> '%s' - %s"), be_tostring(berry.vm, -2), be_tostring(berry.vm, -1));
          be_tracestack(vm);
        } else {
          be_dumpstack(vm);
        }
      }
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
        size_t slots_used_before_gc = va_arg(param, size_t);
        size_t slots_allocated_before_gc = va_arg(param, size_t);
        size_t slots_used_after_gc = va_arg(param, size_t);
        size_t slots_allocated_after_gc = va_arg(param, size_t);
        AddLog(LOG_LEVEL_DEBUG_MORE, D_LOG_BERRY "GC from %i to %i bytes, objects freed %i/%i (in %d ms) - slots from %i/%i to %i/%i",
                                vm_usage, vm_usage2, vm_freed, vm_scanned, gc_elapsed,
                                slots_used_before_gc, slots_allocated_before_gc,
                                slots_used_after_gc, slots_allocated_after_gc);

#ifdef UBE_BERRY_DEBUG_GC
        // Add more in-deptch metrics
        AddLog(LOG_LEVEL_DEBUG_MORE, D_LOG_BERRY "GC timing (us) 1:%i 2:%i 3:%i 4:%i 5:%i total:%i",
            vm->micros_gc1 - vm->micros_gc0,
            vm->micros_gc2 - vm->micros_gc1,
            vm->micros_gc3 - vm->micros_gc2,
            vm->micros_gc4 - vm->micros_gc3,
            vm->micros_gc5 - vm->micros_gc4,
            vm->micros_gc5 - vm->micros_gc0
        );
        AddLog(LOG_LEVEL_DEBUG_MORE, D_LOG_BERRY "GC by type "
            "string:%i class:%i proto:%i instance:%i map:%i "
            "list:%i closure:%i ntvclos:%i module:%i comobj:%i",
            vm->gc_mark_string,
            vm->gc_mark_class,
            vm->gc_mark_proto,
            vm->gc_mark_instance,
            vm->gc_mark_map,
            vm->gc_mark_list,
            vm->gc_mark_closure,
            vm->gc_mark_ntvclos,
            vm->gc_mark_module,
            vm->gc_mark_comobj
        );
#endif
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
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_BERRY "Stack resized from %i to %i bytes"), stack_before, stack_after);
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
  if (berry.vm) {
    // trigger a gc first
    be_gc_collect(berry.vm);
    ResponseAppend_P(PSTR(",\"Berry\":{\"HeapUsed\":%u,\"Objects\":%u}"),
      berry.vm->gc.usage / 1024, berry.vm->counter_gc_kept);
  }
}

/*********************************************************************************************\
 * VM Init
\*********************************************************************************************/
extern "C" void be_webserver_cb_deinit(bvm *vm);
void BerryInit(void) {
  // clean previous VM if any
  if (berry.vm != nullptr) {
    be_cb_deinit(berry.vm);   // deregister any C callback for this VM
#ifdef USE_WEBSERVER
    be_webserver_cb_deinit(berry.vm);   // deregister C callbacks managed by webserver
#endif // USE_WEBSERVER
    be_vm_delete(berry.vm);
    berry.vm = nullptr;
    berry.web_add_handler_done = false;
    berry.autoexec_done = false;
    berry.repl_active = false;
    berry.rules_busy = false;
    berry.timeout = 0;
  }

  int32_t ret_code1, ret_code2;
  bool berry_init_ok = false;
  do {
    berry.vm = be_vm_new(); /* create a virtual machine instance */
    be_set_obs_hook(berry.vm, &BerryObservability);  /* attach observability hook */
    be_set_obs_micros(berry.vm, (bmicrosfnct)&micros);
    comp_set_named_gbl(berry.vm);  /* Enable named globals in Berry compiler */
    comp_set_strict(berry.vm);  /* Enable strict mode in Berry compiler, equivalent of `import strict` */
    be_set_ctype_func_hanlder(berry.vm, be_call_ctype_func);

    if (UsePSRAM()) {     // if PSRAM is available, raise the max size to 512kb
      berry.vm->bytesmaxsize = 512 * 1024;
    }

    be_load_custom_libs(berry.vm);  // load classes and modules

    // Set the GC threshold to 3584 bytes to avoid the first useless GC
    berry.vm->gc.threshold = 3584;

    ret_code1 = be_loadstring(berry.vm, berry_prog);
    if (ret_code1 != 0) {
      be_error_pop_all(berry.vm);             // clear Berry stack
      break;
    }
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_BERRY "Berry code loaded, RAM used=%u"), be_gc_memcount(berry.vm));
    ret_code2 = be_pcall(berry.vm, 0);
    if (ret_code1 != 0) {
      be_error_pop_all(berry.vm);             // clear Berry stack
      break;
    }
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_BERRY "Berry code ran, RAM used=%u"), be_gc_memcount(berry.vm));
    if (be_top(berry.vm) > 1) {
      be_error_pop_all(berry.vm);             // clear Berry stack
    } else {
      be_pop(berry.vm, 1);
    }

    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_BERRY "Berry initialized, RAM used %u bytes"), callBerryGC());
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
 * BrRestart - restart a fresh new Berry vm, unloading everything from previous VM
\*********************************************************************************************/
void CmndBrRestart(void) {
  if (berry.vm == nullptr) {
    ResponseCmndChar_P("Berry VM not started");
  }
  BerryInit();
  ResponseCmndChar_P("Berry VM restarted");
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

  if (!strcmp_P(script_name, "autoexec.be")) {
    if (Settings->flag6.berry_no_autoexec) {   // SetOption153 - (Berry) Disable autoexec.be on restart (1)
      return;
    }
  }

  be_getglobal(berry.vm, PSTR("load"));
  if (!be_isnil(berry.vm, -1)) {
    be_pushstring(berry.vm, script_name);

    BrTimeoutStart();
    if (be_pcall(berry.vm, 1) != 0) {
      be_error_pop_all(berry.vm);             // clear Berry stack
      return;
    }
    BrTimeoutReset();
    bool loaded = be_tobool(berry.vm, -2);  // did it succeed?
    be_pop(berry.vm, 2);
    if (loaded) {
      AddLog(LOG_LEVEL_INFO, D_LOG_BERRY "Successfully loaded '%s'", script_name);
    } else {
      AddLog(LOG_LEVEL_DEBUG, D_LOG_BERRY "No '%s'", script_name);
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
  const char * ret_val;

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
      be_error_pop_all(berry.vm);             // clear Berry stack
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
      "t.scrollTop=1e8;"
      "sn=t.scrollTop;"
    "}"
    "if(t.scrollTop>=sn){"                // User scrolled back so no updates
      "if(x!=null){x.abort();}"           // Abort if no response within 2 seconds (happens on restart 1)
      "x=new XMLHttpRequest();"
      "x.onreadystatechange=()=>{"
        "if(x.readyState==4&&x.status==200){"
          "var d,t1;"
          "d=x.responseText.split(/" BERRY_CONSOLE_CMD_DELIMITER "/,2);"  // Field separator
          "var d1=d.length>1?d[0]:null;"
          "if(d1){"
            "t1=document.createElement('div');"
            "t1.classList.add('br1');"
            "t1.innerText=d1;"
            "t.appendChild(t1);"
          "}"
          "d1=d.length>1?d[1]:d[0];"
          "if(d1){"
            "t1=document.createElement('div');"
            "t1.classList.add('br2');"
            "t1.innerText=d1;"
            "t.appendChild(t1);"
          "}"
          "t.scrollTop=1e8;"
          "sn=t.scrollTop;"
          "clearTimeout(ft);"
          "lt=setTimeout(l,ltm);" // webrefresh timer....
        "}"
      "};"
      "x.open('GET','bc?c2='+id+o,true);"  // Related to Webserver->hasArg("c2") and WebGetArg("c2", stmp, sizeof(stmp))
      "x.send();"
      "ft=setTimeout(l,2e4);" // fail timeout, triggered 20s after asking for XHR
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
  "<form method='get' id='fo' onsubmit='return l(1);'>"
  "<textarea id='c1' class='br0 bri' rows='4' cols='340' wrap='soft' autofocus required></textarea>"
  "<button type='submit'>Run code (or press 'Enter' twice)</button>"
  "</form>"
#ifdef USE_BERRY_DEBUG
  "<p><form method='post' >"
  "<button type='submit' name='rst' class='bred' onclick=\"if(confirm('Confirm removing endpoint')){clearTimeout(lt);return true;}else{return false;}\">Restart Berry VM (for devs only)</button>"
  "</form></p>"
#endif // USE_BERRY_DEBUG
  ;

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

  if (Webserver->hasArg(F("c2"))) {      // Console refresh requested
    HandleBerryConsoleRefresh();
    return;
  }

  if (Webserver->hasArg(F("rst"))) {      // restart VM
    BerryInit();
    Webserver->sendHeader("Location", "/bc", true);
    Webserver->send(302, "text/plain", "");
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

#endif // USE_WEBSERVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xdrv52(uint32_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_SLEEP_LOOP:
      if (TasmotaGlobal.berry_fast_loop_enabled) {    // call only if enabled at global level
        callBerryFastLoop(true);      // call `tasmota.fast_loop()` optimized for minimal performance impact
      }
      break;
    case FUNC_LOOP:
      if (!berry.autoexec_done) {
        // we generate a synthetic event `autoexec`
        callBerryEventDispatcher(PSTR("autoexec"), nullptr, 0, nullptr);

        BrLoad("autoexec.be");   // run autoexec.be at first tick, so we know all modules are initialized
        berry.autoexec_done = true;

        // check if `web_add_handler` was missed, for example because of Berry VM restart
        if (!berry.web_add_handler_done) {
          bool network_up = WifiHasIP();
#ifdef USE_ETHERNET
          network_up = network_up || EthernetHasIP();
#endif
          if (network_up) {       // if network is already up, send a synthetic event to trigger web handlers
            callBerryEventDispatcher(PSTR("web_add_handler"), nullptr, 0, nullptr);
            berry.web_add_handler_done = true;
          }
        }
      }
      if (TasmotaGlobal.berry_fast_loop_enabled) {    // call only if enabled at global level
        callBerryFastLoop(false);      // call `tasmota.fast_loop()` optimized for minimal performance impact
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
    case FUNC_EVERY_200_MSECOND:
      callBerryEventDispatcher(PSTR("every_200ms"), nullptr, 0, nullptr);
      break;
    case FUNC_EVERY_250_MSECOND:
      callBerryEventDispatcher(PSTR("every_250ms"), nullptr, 0, nullptr);
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
      if (!berry.web_add_handler_done) {
        callBerryEventDispatcher(PSTR("web_add_handler"), nullptr, 0, nullptr);
        berry.web_add_handler_done = true;
      }
      WebServer_on(PSTR("/bc"), HandleBerryConsole);
      break;
#endif // USE_WEBSERVER
    case FUNC_SAVE_BEFORE_RESTART:
      callBerryEventDispatcher(PSTR("save_before_restart"), nullptr, 0, nullptr);
      break;
    case FUNC_WEB_SENSOR:
      callBerryEventDispatcher(PSTR("web_sensor"), nullptr, 0, nullptr);
      break;
    case FUNC_WEB_GET_ARG:
      callBerryEventDispatcher(PSTR("web_get_arg"), nullptr, 0, nullptr);
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
