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
// #ifdef ESP32

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
\*********************************************************************************************/
// Berry: `log(string) -> nil`
// Logs the string at LOG_LEVEL_INFO (loglevel=2)
int32_t l_logInfo(struct bvm *vm) {
  int32_t top = be_top(vm); // Get the number of arguments
  if (top == 1 && be_isstring(vm, 1)) {  // only 1 argument of type string accepted
    const char * msg = be_tostring(vm, 1);
    AddLog(LOG_LEVEL_INFO, D_LOG_BERRY "LOG: %s", msg);
    be_return(vm); // Return
  }
  be_return_nil(vm); // Return nil when something goes wrong
}

// Berry: `getFreeHeap() -> int`
// ESP object
int32_t l_getFreeHeap(bvm *vm) {
  be_pushint(vm, ESP.getFreeHeap());
  be_return(vm);
}

// Berry: `printStack() -> nul`
// print stack pointer
// int32_t l_printStack(bvm *vm) {
//   int r = 0;
//   AddLog(LOG_LEVEL_INFO, PSTR("Trampo: stack = 0x%08X"), &r);
//   be_return(vm);
// }

// Yield
int32_t l_yield(bvm *vm) {
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

// call a function (if exists) of type void -> void
void callBerryFunctionVoid(const char * fname) {
  if (nullptr == berry.vm) { return; }
  checkBeTop();
  be_getglobal(berry.vm, fname);
  if (!be_isnil(berry.vm, -1)) {
    // AddLog(LOG_LEVEL_DEBUG, D_LOG_BERRY "Calling '%s'", fname);
    be_pcall(berry.vm, 0);
    be_pop(berry.vm, 1);    // remove function object
  } else {
    // AddLog(LOG_LEVEL_DEBUG, D_LOG_BERRY "Function '%s' not found", fname);
    be_pop(berry.vm, 1);    // remove nil object
  }
  checkBeTop();
}

void test_input(void) {
  int i = 0;
  AddLog(LOG_LEVEL_INFO, "test_input stack = 0x%08X", &i);
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
    AddLog(LOG_LEVEL_INFO, "Trampo: old stack restored");
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
    AddLog(LOG_LEVEL_INFO, "Trampo: after callBerryFunctionVoid");
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

const char berry_prog[] =
  //"def func(x) for i:1..x print('a') end end "
  //"def testreal() return str(1.2+1) end "
  //"def noop() log('noop before'); yield(); log('middle after'); yield(); log('noop after'); end "
  //"log(\"foobar\") "

  // - def l_getFreeHeap() return 1234 end
  // - def l_log(m) print(m) end
  // Simulate Tasmota module
  "class Tasmota "
    "def getFreeHeap() return l_getFreeHeap() end "
    // "def log(m) return l_log(m) end "
  "end "
  "tasmota = Tasmota() "

  "n = 1;"
  "def every_second() n = n + 1; if (n % 100 == 10) log('foobar '+str(n)+' free_heap = '+str(tasmota.getFreeHeap())) end end; "
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
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_BERRY "Berry VM created, RAM consumed=%u (Heap=%u)"), heap_before - ESP.getFreeHeap(), ESP.getFreeHeap());

    // Register functions
    be_regfunc(berry.vm, "log", l_logInfo);
    be_regfunc(berry.vm, "l_getFreeHeap", l_getFreeHeap);
    // be_regfunc(berry.vm, "printStack", l_printStack);
    be_regfunc(berry.vm, "yield", l_yield);

    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_BERRY "Berry function registered, RAM consumed=%u (Heap=%u)"), heap_before - ESP.getFreeHeap(), ESP.getFreeHeap());

    ret_code1 = be_loadstring(berry.vm, berry_prog);
    if (ret_code1 != 0) {
      AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_BERRY "ERROR: be_loadstring [%s] %s"), be_tostring(berry.vm, -2), be_tostring(berry.vm, -1));
      be_pop(berry.vm, 2);
      break;
    }
    ret_code2 = be_pcall(berry.vm, 0);
    if (ret_code1 != 0) {
      AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_BERRY "ERROR: be_pcall [%s] %s"), be_tostring(berry.vm, -2), be_tostring(berry.vm, -1));
      be_pop(berry.vm, 2);
      break;
    }
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
      AddLog(LOG_LEVEL_INFO, "Trampo: we need to complete vm exec 1");
      if (setjmp(berry.ta_cont) == 0) {
        berry.ta_cont_ok = true;
        berry.vm_cont_ok = false;
        AddLog(LOG_LEVEL_INFO, "Trampo: call exec 1");
        longjmp(berry.vm_cont, 1);
      }
      berry.ta_cont_ok = false;
      AddLog(LOG_LEVEL_INFO, "Trampo: returned from exec 1");
    }
    printStack();

    if (berry.vm_cont_ok) {
      printStack();
      AddLog(LOG_LEVEL_INFO, "Trampo: we need to complete vm exec 2");
      if (setjmp(berry.ta_cont) == 0) {
        berry.ta_cont_ok = true;
        berry.vm_cont_ok = false;
        AddLog(LOG_LEVEL_INFO, "Trampo: call exec 2");
        longjmp(berry.vm_cont, 1);
      }
      berry.ta_cont_ok = false;
      AddLog(LOG_LEVEL_INFO, "Trampo: returned from exec 2");
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
  snprintf_P(br_cmd, sizeof(br_cmd), "return (%s)", XdrvMailbox.data);

  checkBeTop();
  do {
    // First try with the `return ()` wrapper
    ret_code = be_loadbuffer(berry.vm, "input", br_cmd, strlen(br_cmd));
    if (be_getexcept(berry.vm, ret_code) == BE_SYNTAX_ERROR) {
      be_pop(berry.vm, 2);    // remove exception values
      // if fails, try the direct command
      ret_code = be_loadbuffer(berry.vm, "input", XdrvMailbox.data, strlen(XdrvMailbox.data));
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
    Response_P(PSTR("%s"), ret_val);
    be_pop(berry.vm, 1);
  } else {
    Response_P(PSTR("[%s] %s"), be_tostring(berry.vm, -2), be_tostring(berry.vm, -1));
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
    case FUNC_EVERY_100_MSECOND:
      // callBerryFunctionVoid("every_100ms");
    //   ScripterEvery100ms();
      break;
    case FUNC_EVERY_SECOND:
      // callBerryFunctionVoid("every_second");
    //   ScriptEverySecond();
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kBrCommands, BerryCommand);
      break;
    case FUNC_SET_POWER:
      break;
    case FUNC_RULES_PROCESS:
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_ADD_BUTTON:
      break;
    case FUNC_WEB_ADD_MAIN_BUTTON:

      break;
    case FUNC_WEB_ADD_HANDLER:
    //   Webserver->on("/" WEB_HANDLE_SCRIPT, HandleScriptConfiguration);
    //   Webserver->on("/ta",HTTP_POST, HandleScriptTextareaConfiguration);
    //   Webserver->on("/exs", HTTP_POST,[]() { Webserver->sendHeader("Location","/exs");Webserver->send(303);}, script_upload_start);
    //   Webserver->on("/exs", HTTP_GET, ScriptExecuteUploadSuccess);
      break;
#endif // USE_WEBSERVER
    case FUNC_SAVE_BEFORE_RESTART:
    //   if (bitRead(Settings.rule_enabled, 0)) {
    //     Run_Scripter(">R", 2, 0);
    //     Scripter_save_pvars();
    //   }
      break;
    case FUNC_MQTT_DATA:
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

// #endif  // ESP32
#endif  // USE_BERRY
