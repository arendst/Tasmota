/*
  xdrv_52_berry.ino - Berry scripting language

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
#include <csetjmp>

const size_t BERRY_STACK =   4096;      // size for the alternate stack for continuation

const char kBrCommands[] PROGMEM = D_PRFX_BR "|"    // prefix
  D_CMND_BR_RUN "|" D_CMND_BR_RESET
  ;

void (* const BerryCommand[])(void) PROGMEM = {
  CmndBrRun, CmndBrReset,
  };

/*********************************************************************************************\
 * Async mode for Berry VM
 * 
 * We enhance the berry language with `yield()` and `wait(ms)` functions.
 * When called, the VM is frozen and control is given back to Tasmota. Then Tasmota
 * at next tick or when the time is reached, resumes the VM.
 * 
 * This is based on coroutines scheme, similar to the contiuation stack of ESP8266.
 * The basic concept is that Tasmota records a longjump target including current stack position
 * and return address.
 * The Berry VM is then called with an alternate stack so that we can switch from both stacks
 * and keep the callchain intact.
 * 
 * High level view:
 * - Tasmota records a return vector with `setjmp`
 * - Tasmota changes replaces the native stack with an alternate stack pre-allocated on the heap
 * - Tasmota calls the Berry VM with `be_pcall`
 * - During the flow of Berry VM, the user code calls `yield()` or `wait(ms)`
 * - Corresponding native function is called (still on alternate stack)
 * - Native function records VM resume target with `setjmp`
 * - and gives back function to Tasmota via `longjmp`.
 *   Note: `longjmp` restores at the same time the native stack.
 * 
 * Note: trampoline functions relies on global variables, since stack variable don't work anymore
 * when replacing stack.
 * 
\*********************************************************************************************/

class BerrySupport {
public:
  bvm *vm = nullptr;                    // berry vm
  bool rules_busy = false;              // are we already processing rules, avoid infinite loop
#ifdef USE_BERRY_ASYNC
  // Alternate stack for the Berry VM
  uint8_t *stack_alloc = nullptr;       // stack malloc address
  uint8_t *stack = nullptr;             // alternate stack for continuation (top of stack)
  // longjmp vectors to yield from Tasmota to VM and reverse
  bool    ta_cont_ok = false;           // is the Tasmota continuation address valid?
  bool    vm_cont_ok = false;           // is the VM continuation address valid?
  jmp_buf ta_cont;                  // continuation structure for the longjump back to Tasmota
  jmp_buf vm_cont;           
  // used by trampoline to call be_pcall()
#endif // USE_BERRY_ASYNC
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
    be_return_nil(vm); // Return nil when something goes wrong
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
    be_return_nil(vm); // Return nil when something goes wrong
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
    be_return_nil(vm); // Return nil when something goes wrong
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
    be_return_nil(vm); // Return nil when something goes wrong
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
    be_return_nil(vm); // Return nil when something goes wrong
  }
}



// Berry: `printStack() -> nul`
// print stack pointer
// int32_t l_printStack(bvm *vm) {
//   int r = 0;
//   AddLog(LOG_LEVEL_INFO, PSTR("Trampo: stack = 0x%08X"), &r);
//   be_return(vm);
// }

// Yield
int32_t l_yield(struct bvm *vm) {
#ifdef USE_BERRY_ASYNC
  if (berry.ta_cont_ok) {                  // if no ta_cont address, then ignore
    if (setjmp(berry.vm_cont) == 0) {   // record the current state
      berry.vm_cont_ok = true;
      longjmp(berry.ta_cont, -1);           // give back control to Tasmota
    }
  }
  berry.vm_cont_ok = false;             // from now, berry.vm_cont is no more valid
#endif // USE_BERRY_ASYNC
  be_return(vm);
}

// be_native_module_attr_table(esp) {
//   be_native_module_function("getFreeHeap", l_getFreeHeap),
// };
// be_define_native_module(math, nullptr);


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

void test_input(void) {
  int i = 0;
  AddLog(LOG_LEVEL_INFO, PSTR("test_input stack = 0x%08X"), &i);
  callBerryFunctionVoid("noop");
}

int be_pcall_with_alt_stack() {
  berry.fret = be_pcall(berry.vm, 0);
  return berry.fret;
}

void printStack(void) {
  int r = 0;
  AddLog(LOG_LEVEL_INFO, PSTR("Trampo: stack = 0x%08X"), &r);
}

#ifdef USE_BERRY_ASYNC
int32_t callTrampoline(void *func) {
  // Tasmota stack active
  // ----------------------------------
  static int r;
  berry.vm_cont_ok = false;

  if ((r = setjmp(berry.ta_cont)) == 0) {     // capture registers
    // Tasmota stack active
    // ----------------------------------
    // on the first run, we call back ourselves with the alternate stack

    // we clone the return vector and change the stack pointer
    static jmp_buf trampo;
    memmove(trampo, berry.ta_cont, sizeof(berry.ta_cont));

#if defined(ESP8266) || defined(ESP32)
    trampo[1] = (int32_t) berry.stack;      // change stack
#else
    #error "Need CPU specific code for setting alternate stack"
#endif
    longjmp(trampo, (int)func);
    // this part is unreachable (longjmp does not return)
  } else if (r == -1) {
    // Tasmota stack active
    // ----------------------------------
    // the call has completed normally, and `yield` was not called
    berry.ta_cont_ok = false;
    AddLog(LOG_LEVEL_INFO, PSTR("Trampo: old stack restored"));
    // printStack();
  } else {
    // WARNING
    // ALTERNATE stack active
    // - DON'T USE ANY LOCAL VARIABLE HERE
    // -----------------------------------
    // r contains the address of the function to call
    // AddLog(LOG_LEVEL_INFO, "Trampo: new stack reg");
    // printStack();
    berry.ta_cont_ok = true;          // Berry can call back Tasmota thread
    callBerryFunctionVoid("noop");
    AddLog(LOG_LEVEL_INFO, PSTR("Trampo: after callBerryFunctionVoid"));
    // printStack();
    longjmp(berry.ta_cont, -1);
    // this part is unreachable (longjmp does not return)
    // which protects us from accidentally using the alternate stack
    // in regular code
  }
  // Tasmota stack active
  // ----------------------------------
}
#endif // USE_BERRY_ASYNC

// void fake_callBerryFunctionVoid(const char * fname, jmp_buf * env) {
//   (void) setjmp(env);
// }
// void call_callBerryFunctionVoid(const char * fname, jmp_buf * ret_env, ) {
//   callBerryFunctionVoid(fname);
//   longjump(env, 1);
// }

/*********************************************************************************************\
 * Handlers for Berry calls and async
 * 
\*********************************************************************************************/

const char berry_prog[] PROGMEM =
  ""
  //"def func(x) for i:1..x print('a') end end "
  //"def testreal() return str(1.2+1) end "
  //"def noop() log('noop before'); yield(); log('middle after'); yield(); log('noop after'); end "
  //"log(\"foobar\") "

  // auto-import modules
  "import string "
  "import json "
  "import gc "
  "import tasmota "
  // import alias
  "import tasmota as t "

  // add `charsinstring(s:string,c:string) -> int``
  // looks for any char in c, and return the position of the first chat
  // or -1 if not found
  "def charsinstring(s,c) "
    "for i:0..size(s)-1 "
      "for j:0..size(c)-1 "
        "if s[i] == c[j] return i end "
      "end "
    "end "
    "return -1 "
  "end "

  // find a key in map, case insensitive, return actual key or nil if not found
  "def findkeyi(m,keyi) "
    "keyu=string.toupper(keyi) "
    "if classof(m) == map "
      "for k:m.keys() "
        "if string.toupper(k)==keyu || keyi=='?' "
          "return k "
        "end "
      "end "
    "end "
  "end "

  // Rules

  "tasmota._operators='=<>!' "     // operators used in rules
  // Rules comparisong functions
  "tasmota._eqstr=/s1,s2-> str(s1) == str(s2) "
  "tasmota._neqstr=/s1,s2-> str(s1) != str(s2) "
  "tasmota._eq=/f1,f2-> real(f1) == real(f2) "
  "tasmota._neq=/f1,f2-> real(f1) != real(f2) "
  "tasmota._gt=/f1,f2-> real(f1) > real(f2) "
  "tasmota._lt=/f1,f2-> real(f1) < real(f2) "
  "tasmota._ge=/f1,f2-> real(f1) >= real(f2) "
  "tasmota._le=/f1,f2-> real(f1) <= real(f2) "

  "tasmota._op=["
    "['==',tasmota._eqstr],"
    "['!==',tasmota._neqstr],"
    "['=',tasmota._eq],"
    "['!=',tasmota._neq],"
    "['>=',tasmota._ge],"
    "['<=',tasmota._le],"
    "['>',tasmota._gt],"
    "['<',tasmota._lt],"
  "] "
  "tasmota_rules={} "
  "tasmota.rule = def(pat,f) tasmota_rules[pat] = f end "

  // # split the item when there is an operator, returns a list of (left,op,right)
  // # ex: "Dimmer>50" -> ["Dimmer",tasmota_gt,"50"]
  "tasmota.find_op = def (item) "
    "pos = charsinstring(item, tasmota._operators) "
    "if pos>=0 "
      "op_split = string.split(item,pos) "
      // #print(op_split)
      "op_left = op_split[0] "
      "op_rest = op_split[1] "
      // # iterate through operators
      "for op:tasmota._op "
        "if string.find(op_rest,op[0]) == 0 "
          "op_func = op[1] "
          "op_right = string.split(op_rest,size(op[0]))[1] "
          "return [op_left,op_func,op_right] "
        "end "
      "end "
    "end "
    "return [item, nil, nil] "
  "end "

  // Rules trigger if match. return true if match, false if not
  // Note: condition is not yet managed
  "tasmota.try_rule = def (ev, rule, f) "
    "rl_list = tasmota.find_op(rule) "
    "e=ev "
    "rl=string.split(rl_list[0],'#') "
    "for it:rl "
      "found=findkeyi(e,it) "
      "if found == nil "
        "return false "
      "end "
      "e=e[found] "
    "end "
    // # check if condition is true
    "if rl_list[1] "
      // # did we find a function
      "if !rl_list[1](e,rl_list[2]) "
        // # condition is not met
        "return false "
      "end "
    "end "
    "f(e,ev) "
    "return true "
  "end "
  // Run rules, i.e. check each individual rule
  // Returns true if at least one rule matched, false if none
  "tasmota.exec_rules = def (ev_json) "
    "ev = json.load(ev_json) "
    "ret = false "
    "if ev == nil "
      "log('BRY: ERROR, bad json: '+ev_json, 3) "
    "end "
    "for r:tasmota_rules.keys() "
      "ret = tasmota.try_rule(ev,r,tasmota_rules[r]) || ret "
    "end "
    "return ret "
  "end "
  // Not sure how to run `tasmota.exec_rules` from C code, so alias with `_exec_rules()``
  "def _exec_rules(e) return tasmota.exec_rules(e) end "

  // Timers
  "tasmota_timers=[] "
  "tasmota.timer = def (delay,f) tasmota_timers.push([tasmota.millis(delay),f]) end "

  "def _run_deferred() "
    "i=0 "
    "while i<tasmota_timers.size() "
      "if tasmota.timereached(tasmota_timers[i][0]) "
        "f=tasmota_timers[i][1] "
        "tasmota_timers.remove(i) "
        "f() "
      "else "
        "i=i+1 "
      "end "
    "end "
  "end "

  // trigger Garbage Collector
  "gc.collect() "
  // "n = 1;"
  // "def every_second() n = n + 1; if (n % 100 == 10) log('foobar '+str(n)+' free_heap = '+str(tasmota.getfreeheap())) end end; "
  ;

/*********************************************************************************************\
 * VM Init
\*********************************************************************************************/
void BrReset(void) {
  // clean previous VM if any
  if (berry.vm != nullptr) {
    be_vm_delete(berry.vm);
    berry.vm = nullptr;
#ifdef USE_BERRY_ASYNC
    berry.ta_cont_ok = false;           // is the Tasmota continuation address valid?
    berry.vm_cont_ok = false;           // is the VM continuation address valid?
#endif // USE_BERRY_ASYNC
  }

  int32_t ret_code1, ret_code2;
  bool berry_init_ok = false;
  do {
#ifdef USE_BERRY_ASYNC
    berry.stack_alloc = (uint8_t*) malloc(BERRY_STACK);      // alternate stack
    berry.stack = berry.stack_alloc + BERRY_STACK;      // top of stack
#endif // USE_BERRY_ASYNC
    
    uint32_t heap_before = ESP.getFreeHeap();
    berry.vm = be_vm_new(); /* create a virtual machine instance */
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_BERRY "Berry VM created, RAM consumed=%u (Heap=%u)"), heap_before - ESP.getFreeHeap(), ESP.getFreeHeap());

    // Register functions
    be_regfunc(berry.vm, PSTR("log"), l_logInfo);
    // be_regfunc(berry.vm, "printStack", l_printStack);
    be_regfunc(berry.vm, PSTR("yield"), l_yield);

    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_BERRY "Berry function registered, RAM consumed=%u (Heap=%u)"), heap_before - ESP.getFreeHeap(), ESP.getFreeHeap());

    ret_code1 = be_loadstring(berry.vm, berry_prog);
    if (ret_code1 != 0) {
      AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_BERRY "ERROR: be_loadstring [%s] %s"), be_tostring(berry.vm, -2), be_tostring(berry.vm, -1));
      be_pop(berry.vm, 2);
      break;
    }
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_BERRY "Berry code loaded, RAM consumed=%u (Heap=%u)"), heap_before - ESP.getFreeHeap(), ESP.getFreeHeap());
    ret_code2 = be_pcall(berry.vm, 0);
    if (ret_code1 != 0) {
      AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_BERRY "ERROR: be_pcall [%s] %s"), be_tostring(berry.vm, -2), be_tostring(berry.vm, -1));
      be_pop(berry.vm, 2);
      break;
    }
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_BERRY "Berry code ran, RAM consumed=%u (Heap=%u)"), heap_before - ESP.getFreeHeap(), ESP.getFreeHeap());
    be_pop(berry.vm, 1);

    // AddLog(LOG_LEVEL_INFO, PSTR("Get function"));
    // AddLog(LOG_LEVEL_INFO, PSTR("BE_TOP = %d"), be_top(berry.vm));

    // AddLog(LOG_LEVEL_INFO, PSTR("Get function"));
    // be_getglobal(vm, PSTR("func"));
    // be_pushint(vm, 3);
    // be_pcall(vm, 1);
    // be_pop(vm, 2);
    // // AddLog(LOG_LEVEL_INFO, PSTR("BE_TOP = %d"), be_top(vm));

    // be_getglobal(vm, "testreal");
    // AddLog(LOG_LEVEL_INFO, PSTR("is_nil -1 = %d"), be_isnil(vm, -1));
    // be_pcall(vm, 0);
    // // AddLog(LOG_LEVEL_INFO, PSTR("is_nil -1 = %d"), be_isnil(vm, -1));
    // AddLog(LOG_LEVEL_INFO, PSTR("to_string -1 = %s"), be_tostring(vm, -1));
    // be_pop(vm, 1);
    // AddLog(LOG_LEVEL_INFO, PSTR("BE_TOP = %d"), be_top(vm));

    // try a non-existant function
    // be_getglobal(vm, "doesnotexist");
    // AddLog(LOG_LEVEL_INFO, PSTR("is_nil -1 = %d"), be_isnil(vm, -1));
    // AddLog(LOG_LEVEL_INFO, PSTR("BE_TOP = %d"), be_top(vm));
    // be_pop(vm, 1);

    // Try
    // callBerryFunctionVoid("noop");
    // callBerryFunctionVoid("noop2");

    // test_input();

    /////////////////////////////////
    // callTrampoline(nullptr);

    // // Try coroutines
    // int jmp_val;
    // if ((jmp_val=setjmp(berry.ta_cont)) == 0) {
    //   AddLog(LOG_LEVEL_INFO, "vm return address = 0x%08X", berry.ta_cont[0]);
    //   AddLog(LOG_LEVEL_INFO, "vm stack  address = 0x%08X", berry.ta_cont[1]);
    //   callTrampoline(nullptr);
    //   // // call routine
    //   // jmp_buf trampoline_env;
    //   // fake_callBerryFunctionVoid("noop", &tasmota_env);
    //   // trampoline_env[0] = call_callBerryFunctionVoid
    // } else {
    //   AddLog(LOG_LEVEL_INFO, "vm return address = 0x%08X", berry.ta_cont[0]);
    //   // we get back control
    // }

#ifdef USE_BERRY_ASYNC
    if (berry.vm_cont_ok) {
      printStack();
      AddLog(LOG_LEVEL_INFO, PSTR("Trampo: we need to complete vm exec 1"));
      if (setjmp(berry.ta_cont) == 0) {
        berry.ta_cont_ok = true;
        berry.vm_cont_ok = false;
        AddLog(LOG_LEVEL_INFO, PSTR("Trampo: call exec 1"));
        longjmp(berry.vm_cont, 1);
      }
      berry.ta_cont_ok = false;
      AddLog(LOG_LEVEL_INFO, PSTR("Trampo: returned from exec 1"));
    }
    printStack();

    if (berry.vm_cont_ok) {
      printStack();
      AddLog(LOG_LEVEL_INFO, PSTR("Trampo: we need to complete vm exec 2"));
      if (setjmp(berry.ta_cont) == 0) {
        berry.ta_cont_ok = true;
        berry.vm_cont_ok = false;
        AddLog(LOG_LEVEL_INFO, PSTR("Trampo: call exec 2"));
        longjmp(berry.vm_cont, 1);
      }
      berry.ta_cont_ok = false;
      AddLog(LOG_LEVEL_INFO, PSTR("Trampo: returned from exec 2"));
    }
    printStack();
#endif // USE_BERRY_ASYNC
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_BERRY "Berry initialized, RAM consumed=%u (Heap=%u)"), heap_before - ESP.getFreeHeap(), ESP.getFreeHeap());
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
#ifdef USE_BERRY_ASYNC
    if (berry.stack_alloc != nullptr) {
      free(berry.stack_alloc);
      berry.stack_alloc = nullptr;
    }
#endif // USE_BERRY_ASYNC
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
