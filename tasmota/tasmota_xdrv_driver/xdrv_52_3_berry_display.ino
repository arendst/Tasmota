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
extern Renderer *renderer;
#endif //  USE_UNIVERSAL_DISPLAY

extern "C" {
  /*********************************************************************************************\
   * Native functions mapped to Berry functions
   * 
   * import display
   * display.start(string) -> comptr or nil if failed
   * 
  \*********************************************************************************************/
  int be_ntv_display_start(struct bvm *vm) {
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

  // `display.dimmer([dim:int]) -> int` sets the dimmer of display, value 0..100. If `0` then turn off display. If no arg, read the current value.
  int be_ntv_display_dimmer(struct bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    int32_t dimmer;
    if (argc >= 1) {
      if (!be_isint(vm, 1)) { be_raise(vm, "type_error", "arg must be int"); }
      dimmer = be_toint(vm, 1);
      if ((dimmer < 0) || (dimmer > 100)) { be_raise(vm, "value_error", "value must be in range 0..100"); }
      be_pop(vm, argc);   // clear stack to avoid ripple errors in code called later
      SetDisplayDimmer(dimmer);
      ApplyDisplayDimmer();
    }
    be_pushint(vm, GetDisplayDimmer());
    be_return(vm);
  }

  void be_ntv_display_touch_update(int32_t touches, int32_t raw_x, int32_t raw_y, int32_t gesture) {
  #if defined(USE_UNIVERSAL_TOUCH) || defined(USE_FT5206) || defined(USE_XPT2046) || defined(USE_GT911) || defined(USE_LILYGO47) || defined(USE_TOUCH_BUTTONS)
    Touch_SetStatus(touches, raw_x, raw_y, gesture);
  #endif
  }

  const char* be_ntv_display_driver_name(void) {
  #ifdef USE_UNIVERSAL_DISPLAY
    if (renderer) {
      char* devname = renderer->devname();
      if (devname) {
        return devname;
      }
    }
  #endif
    return "";
  }

  bbool be_ntv_display_started(void) {
  #ifdef USE_UNIVERSAL_DISPLAY
    if (renderer) {
      return true;
    }
  #endif
    return false;
  }
}

#endif // USE_DISPLAY
#endif  // USE_BERRY
