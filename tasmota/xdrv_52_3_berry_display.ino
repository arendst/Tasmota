/*
  xdrv_52_3_berry_display.ino - Berry scripting language, native fucnctions

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
#ifdef USE_DISPLAY

#include <berry.h>

#ifdef USE_UNIVERSAL_DISPLAY
Renderer *Init_uDisplay(const char *desc);
#endif //  USE_UNIVERSAL_DISPLAY

/*********************************************************************************************\
 * Native functions mapped to Berry functions
 * 
 * import display
 * display.start(string) -> comptr or nil if failed
 * 
\*********************************************************************************************/
extern "C" {

  int be_disp_start(bvm *vm);
  int be_disp_start(bvm *vm) {
#ifdef USE_UNIVERSAL_DISPLAY
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 1 && be_isstring(vm, 1)) {
      const char * desc = be_tostring(vm, 1);
      // remove all objects on stack to avoid warnings in subsequent calls to Berry
      be_pop(vm, argc);
      Renderer * renderer = Init_uDisplay(desc);
      if (renderer) {
        be_pushcomptr(vm, renderer);
      } else {
        be_pushnil(vm);
      }
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
#else // USE_UNIVERSAL_DISPLAY
    be_raise(vm, "internal_error", "universal display driver not present");
#endif // USE_UNIVERSAL_DISPLAY
  }
}

#endif // USE_DISPLAY
#endif  // USE_BERRY
