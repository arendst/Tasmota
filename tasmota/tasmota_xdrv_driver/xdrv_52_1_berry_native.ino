/*
  xdrv_52_1_berry_native.ino - Berry scripting language, native fucnctions

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
const char kInternalError[] PROGMEM = "intenal_error";

/*********************************************************************************************\
 * LVGL top level virtual members
 * 
 * Responds to virtual constants
\*********************************************************************************************/
extern "C" {
  #include "be_vm.h"
  // Call error handler and pop all from stack
  void be_error_pop_all(bvm *vm);
  void be_error_pop_all(bvm *vm) {
    if (vm->obshook != NULL) (*vm->obshook)(vm, BE_OBS_PCALL_ERROR);
    be_pop(vm, be_top(vm));       // clear Berry stack
  }

  #include "be_exec.h"
  #include "be_debug.h"
  void be_dumpstack(bvm *vm) {
    int32_t top = be_top(vm);
    AddLog(LOG_LEVEL_DEBUG, "BRY: top=%d", top);
    be_tracestack(vm);
    for (uint32_t i = 1; i <= top; i++) {
      const char * tname = be_typename(vm, i);
      const char * cname = be_classname(vm, i);
      if (be_isstring(vm, i)) {
        cname = be_tostring(vm, i);
      }
      AddLog(LOG_LEVEL_INFO, "BRY: stack[%d] = type='%s' (%s)", i, (tname != nullptr) ? tname : "", (cname != nullptr) ? cname : "");
    }
  }

  // convert to unsigned 8 bits
  static uint8_t to_u8(int32_t v) {
    if (v < 0) { return 0; }
    if (v > 0xFF) { return 0xFF; }
    return v;
  }

  int32_t member_find(bvm *vm, const char *key, int32_t default_value) {
    int32_t ret = default_value;
    if (be_getmember(vm, -1, key)) {
      if (be_isint(vm, -1)) {
        ret = be_toint(vm, -1);
      }
    }
    be_pop(vm, 1);
    return ret;
  }
  static bool map_find(bvm *vm, const char *key)
  {
    be_getmethod(vm, -1, "find");   // look for "find" method of "Map" instance
    be_pushvalue(vm, -2);           // put back instance as first argument (implicit instance)
    be_pushstring(vm, key);         // push string as second argument
    be_call(vm, 2);                 // call wirn 2 parameters (implicit instance and key)
    be_pop(vm, 2);                  // pop 2 arguments, the function is replaced by result
    return !be_isnil(vm, -1);       // true if not 'nil'
  }
  static int32_t get_list_size(bvm *vm) {
    be_getmethod(vm, -1, "size");   // look for "size" method of "list" instance
    be_pushvalue(vm, -2);           // put back instance as first argument (implicit instance)
    be_call(vm, 1);                 // call wirn 2 parameters (implicit instance and key)
    int32_t ret = be_toint(vm, -2);
    be_pop(vm, 2);                  // pop 1 argument and return value
    return ret;
  }
  // get item number `index` from list, index must be valid or raises an exception
  static void get_list_item(bvm *vm, int32_t index) {
    be_getmethod(vm, -1, "item");   // look for "size" method of "list" instance
    be_pushvalue(vm, -2);           // put back instance as first argument (implicit instance)
    be_pushint(vm, index);
    // be_dumpstack(vm);
    be_call(vm, 2);                 // call wirn 2 parameters (implicit instance and key)
    be_pop(vm, 2);                  // pop 2 arguments and return value
  }
}

/*********************************************************************************************\
 * Manage timeout for Berry code
 *
\*********************************************************************************************/
void BrTimeoutStart(void)  {
  berry.timeout = millis() + USE_BERRY_TIMEOUT;
  if (0 == berry.timeout) {
    berry.timeout = 1;    // rare case when value accidentally computes to zero
  }
  yield();
}

void BrTimeoutYield(void) {
  if (0 != berry.timeout) {
    BrTimeoutStart();
  }
}

void BrTimeoutReset(void)  {
  berry.timeout = 0;      // remove timer
}

#endif  // USE_BERRY
