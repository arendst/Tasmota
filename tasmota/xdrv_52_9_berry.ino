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

extern "C" {
  extern void be_load_custom_libs(bvm *vm);
}

const char kBrCommands[] PROGMEM = D_PRFX_BR "|"    // prefix
  D_CMND_BR_RUN "|" D_CMND_BR_RESET
  ;

void (* const BerryCommand[])(void) PROGMEM = {
  CmndBrRun, CmndBrReset,
  };

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
#else
  void *berry_malloc(uint32_t size) {
    return malloc(size);
  }
  void *berry_realloc(void *ptr, size_t size) {
    return realloc(ptr, size);
  }
#endif // USE_BERRY_PSRAM

}


/*********************************************************************************************\
 * Handlers for Berry calls and async
 * 
\*********************************************************************************************/
// // call a function (if exists) of type void -> void

bool callBerryRule(void) {
  if (berry.rules_busy) { return false; }
  berry.rules_busy = true;
  char * json_event = TasmotaGlobal.mqtt_data;
  bool serviced = false;
  serviced = callBerryEventDispatcher(PSTR("rule"), nullptr, 0, TasmotaGlobal.mqtt_data);
  berry.rules_busy = false;
  return serviced;     // TODO event not handled
}

size_t callBerryGC(void) {
  return callBerryEventDispatcher(PSTR("gc"), nullptr, 0, nullptr);
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
int32_t callBerryEventDispatcher(const char *type, const char *cmd, int32_t idx, const char *payload) {
  int32_t ret = 0;

  if (nullptr == berry.vm) { return ret; }
  checkBeTop();
  be_getglobal(berry.vm, PSTR("tasmota"));
  if (!be_isnil(berry.vm, -1)) {
    be_getmethod(berry.vm, -1, PSTR("event"));
    if (!be_isnil(berry.vm, -1)) {
      be_pushvalue(berry.vm, -2); // add instance as first arg
      be_pushstring(berry.vm, type != nullptr ? type : "");
      be_pushstring(berry.vm, cmd != nullptr ? cmd : "");
      be_pushint(berry.vm, idx);
      be_pushstring(berry.vm, payload != nullptr ? payload : "{}");  // empty json
      be_pcall(berry.vm, 5);   // 5 arguments
      be_pop(berry.vm, 5);
      if (be_isint(berry.vm, -1) || be_isbool(berry.vm, -1)) {
        if (be_isint(berry.vm, -1)) { ret = be_toint(berry.vm, -1); }
        if (be_isbool(berry.vm, -1)) { ret = be_tobool(berry.vm, -1); }
      }
    }
    be_pop(berry.vm, 1);  // remove method
  }
  be_pop(berry.vm, 1);  // remove instance object
  checkBeTop();
  return ret;
}

/*********************************************************************************************\
 * VM Observability
\*********************************************************************************************/
void BerryObservability(bvm *vm, int32_t event...);
void BerryObservability(bvm *vm, int32_t event...) {
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
        AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_BERRY "GC from %i to %i bytes (in %d ms)"), vm_usage, vm_usage2, gc_elapsed);
      }
      break;
    default: break;
  }
  va_end(param);
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
    be_set_obs_hook(berry.vm, &BerryObservability);
    be_load_custom_libs(berry.vm);
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_BERRY "Berry VM created, RAM used=%u"), be_gc_memcount(berry.vm));

    // Register functions
    // be_regfunc(berry.vm, PSTR("log"), l_logInfo);
    // be_regfunc(berry.vm, PSTR("save"), l_save);

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
      be_pop(berry.vm, 1);
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


void BrAutoexec(void) {
  if (berry.vm == nullptr) { return; }

  int32_t ret_code1, ret_code2;
  bool berry_init_ok = false;

  // load 'autoexec.be' or 'autoexec.bec'
  ret_code1 = be_loadstring(berry.vm, berry_autoexec);
  // be_dumpstack(berry.vm);
  if (ret_code1 != 0) {
    be_pop(berry.vm, 2);
    return;
  }
  ret_code2 = be_pcall(berry.vm, 0);
  // be_dumpstack(berry.vm);
  if (ret_code1 != 0) {
    // AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_BERRY "ERROR: be_pcall [%s] %s"), be_tostring(berry.vm, -2), be_tostring(berry.vm, -1));
    be_pop(berry.vm, 1);
    return;
  }
  // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_BERRY "Berry code ran, RAM used=%u"), be_gc_memcount(berry.vm));
  be_pop(berry.vm, 1);
  // be_dumpstack(berry.vm);
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

//
// Command `BrReset`
//
void CmndBrReset(void) {
  if (berry.vm == nullptr) { ResponseCmndChar_P(PSTR(D_BR_NOT_STARTED)); return; }

  BrReset();
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
      ret_code = be_pcall(berry.vm, 0);     // execute code
      // AddLog(LOG_LEVEL_INFO, PSTR(">>>> be_pcall ret=%i"), ret_code);
      if (0 == ret_code) {
        if (!be_isnil(berry.vm, 1)) {
          const char * ret_val = be_tostring(berry.vm, 1);
          berry.log.addString(ret_val, nullptr, "\n");
          // AddLog_P(LOG_LEVEL_INFO, PSTR(">>> %s"), ret_val);
        }
        be_pop(berry.vm, 1);
      }
    }
    if (BE_EXCEPTION == ret_code) {
      be_dumpstack(berry.vm);
      char exception_s[120];
      ext_snprintf_P(exception_s, sizeof(exception_s), PSTR("%s: %s"), be_tostring(berry.vm, -2), be_tostring(berry.vm, -1));
      berry.log.addString(exception_s, nullptr, "\n");
      // AddLog_P(LOG_LEVEL_INFO, PSTR(">>> %s"), exception_s);
      be_pop(berry.vm, 2);
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
      "x.open('GET','bs?c2='+id+o,true);"  // Related to Webserver->hasArg("c2") and WebGetArg("c2", stmp, sizeof(stmp))
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
      "Check the <a href='https://tasmota.github.io/docs/Berry-Scripting/' target='_blank'>documentation</a>."
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
  "<p><form action='bs' method='get'><button>Berry Scripting console</button></form></p>";


void HandleBerryConsoleRefresh(void)
{
  String svalue = Webserver->arg(F("c1"));

  svalue.trim();
  if (svalue.length()) {
    berry.log.reset();          // clear all previous logs
    berry.repl_active = true;   // start recording
    // AddLog_P(LOG_LEVEL_INFO, PSTR("BRY: received command %s"), svalue.c_str());
    berry.log.addString(svalue.c_str(), nullptr, BERRY_CONSOLE_CMD_DELIMITER);

    // Call berry
    BrREPLRun((char*)svalue.c_str());
    berry.repl_active = false;   // don't record further
  }

  WSContentBegin(200, CT_PLAIN);

  if (!berry.log.isEmpty()) {

    WSContentFlush();

    for (auto & l: berry.log.log) {
      _WSContentSend((char*) l);
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
  WSContentSend_P(HTTP_SCRIPT_BERRY_CONSOLE, Settings.web_refresh);
  WSContentSend_P(HTTP_SCRIPT_BERRY_CONSOLE2);
  WSContentSendStyle();
  WSContentFlush();
  _WSContentSend(HTTP_BERRY_STYLE_CMND);
  _WSContentSend(HTTP_BERRY_FORM_CMND);
  WSContentSpaceButton(BUTTON_MAIN);
  WSContentStop();
}

// void HandleBerryConsoleRefresh(void)
// {
//   String svalue = Webserver->arg(F("c1"));
//   if (svalue.length() && (svalue.length() < MQTT_MAX_PACKET_SIZE)) {
//     // TODO run command and store result
//     // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_COMMAND "%s"), svalue.c_str());
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
//   while (GetLog(Settings.weblog_level, &index, &line, &len)) {
//     if (len > sizeof(TasmotaGlobal.mqtt_data) -2) { len = sizeof(TasmotaGlobal.mqtt_data); }
//     char stemp[len +1];
//     strlcpy(stemp, line, len);
//     WSContentSend_P(PSTR("%s%s"), (cflg) ? PSTR("\n") : "", stemp);
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
    //case FUNC_PRE_INIT:
    case FUNC_INIT:
      BrReset();
      break;
    case FUNC_LOOP:
      if (!berry.autoexec_done) {
        BrAutoexec();   // run autoexec.be at first tick, so we know all modules are initialized
        berry.autoexec_done = true;
      }
      break;

    // Berry wide commands and events
    case FUNC_RULES_PROCESS:
      result = callBerryRule();
      break;
    case FUNC_MQTT_DATA:
      result = callBerryEventDispatcher(PSTR("mqtt_data"), XdrvMailbox.topic, 0, XdrvMailbox.data);
      break;
    case FUNC_EVERY_50_MSECOND:
      callBerryEventDispatcher(PSTR("every_50ms"), nullptr, 0, nullptr);
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kBrCommands, BerryCommand);
      if (!result) {
        result = callBerryEventDispatcher(PSTR("cmd"), XdrvMailbox.topic, XdrvMailbox.index, XdrvMailbox.data);
      }
      break;
    
    // Module specific events
    case FUNC_EVERY_100_MSECOND:
      callBerryEventDispatcher(PSTR("every_100ms"), nullptr, 0, nullptr);
      break;
    case FUNC_EVERY_SECOND:
      callBerryEventDispatcher(PSTR("every_second"), nullptr, 0, nullptr);
      break;
    // case FUNC_SET_POWER:
    //   break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_ADD_BUTTON:
      WSContentSend_P(HTTP_BTN_BERRY_CONSOLE);
      callBerryEventDispatcher(PSTR("web_add_button"), nullptr, 0, nullptr);
      break;
    case FUNC_WEB_ADD_MAIN_BUTTON:
      callBerryEventDispatcher(PSTR("web_add_main_button"), nullptr, 0, nullptr);
      break;
    case FUNC_WEB_ADD_HANDLER:
      // callBerryEventDispatcher(PSTR("web_add_handler"), nullptr, 0, nullptr);
      WebServer_on(PSTR("/bs"), HandleBerryConsole);
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
