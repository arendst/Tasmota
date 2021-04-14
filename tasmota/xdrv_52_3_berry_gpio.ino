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


/*********************************************************************************************\
 * Native functions mapped to Berry functions
 * 
 * import gpio
 * 
 * 
\*********************************************************************************************/
extern "C" {

  int gp_pin_mode(bvm *vm);
  int gp_pin_mode(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 2 && be_isint(vm, 1) && be_isint(vm, 2)) {
      int32_t pin = be_toint(vm, 1);
      int32_t mode = be_toint(vm, 2);
      if (pin >= 0) {
        pinMode(pin, mode);
      }
      be_return_nil(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int gp_digital_write(bvm *vm);
  int gp_digital_write(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 2 && be_isint(vm, 1) && be_isint(vm, 2)) {
      int32_t pin = be_toint(vm, 1);
      int32_t val = be_toint(vm, 2);
      if (pin >= 0) {
        digitalWrite(pin, val);
      }
      be_return_nil(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }
  
  int gp_digital_read(bvm *vm);
  int gp_digital_read(bvm *vm)  {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 1 && be_isint(vm, 1)) {
      int32_t pin = be_toint(vm, 1);
      if (pin >= 0) {
        int32_t ret = digitalRead(pin);
        be_pushint(vm, ret);
      } else {
        be_pushnil(vm);
      }
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

// Tasmota specific

  int gp_pin_used(bvm *vm);
  int gp_pin_used(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 1 && argc <= 2 && be_isint(vm, 1)) {
      int32_t pin = be_toint(vm, 1);
      int32_t index = 0;
      if (argc == 2 && be_isint(vm, 2)) {
        index = be_toint(vm, 2);
      }
      bool ret = PinUsed(pin, index);
      be_pushbool(vm, ret);
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int gp_pin(bvm *vm);
  int gp_pin(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 1 && argc <= 2 && be_isint(vm, 1)) {
      int32_t pin = be_toint(vm, 1);
      int32_t index = 0;
      if (argc == 2 && be_isint(vm, 2)) {
        index = be_toint(vm, 2);
      }
      int32_t ret = Pin(pin, index);
      be_pushint(vm, ret);
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

}



#endif  // USE_BERRY
