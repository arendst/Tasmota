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
#ifdef USE_LVGL

#include <berry.h>
#include "lvgl.h"
#include "be_lvgl.h"
#include "Adafruit_LvGL_Glue.h"

#ifdef USE_LVGL_FREETYPE
#include "esp_task_wdt.h"
#include "lv_freetype.h"
#endif

// Berry easy logging
extern "C" {
  extern void berry_log_C(const char * berry_buf, ...);
}

extern Adafruit_LvGL_Glue * glue;

/********************************************************************
 * Structures used by LVGL_Berry
 *******************************************************************/

class LVBE_button {
public:
  bool pressed = false;       // what is the current state
  bool inverted = false;      // false: button pressed is HIGH, true: button pressed is LOW
  int8_t pin = -1;            // physical GPIO (-1 if unconfigured)

  uint32_t millis_last_state_change = 0; // last millis() time stamp when the state changed, used for debouncing
  const uint32_t debounce_time = 10;     // Needs to stabilize for 10ms before state change

  inline void set_inverted(bool inv) { inverted = inv; }
  inline bool get_inverted(void) const { return inverted; }

  inline bool valid(void) const { return pin >= 0; }

  bool read_gpio(void) const {
    bool cur_state = digitalRead(pin);
    if (inverted) { cur_state = !cur_state; }
    return cur_state;
  }

  void set_gpio(int8_t _pin) {      // is the button pressed
    pin = _pin;
    pressed = read_gpio();
    millis_last_state_change = millis();
  }

  bool state_changed(void) {        // do we need to report a change
    if (!valid()) { return false; }
    if (TimeReached(millis_last_state_change + debounce_time)) {
      // read current state of GPIO after debounce
      if (read_gpio() != pressed) {
        return true;
      }
    }
    return false;
  }

  bool clear_state_changed(void) {  // read and clear the state
    pressed = read_gpio();
    millis_last_state_change = millis();
    return pressed;
  }
};

class LVBE_globals {
public:
  lv_indev_drv_t indev_drv;
  LList<lv_indev_t*> indev_list;
  // input devices
  LVBE_button btn[3];
};
LVBE_globals lvbe;

/********************************************************************
 * Generated code, don't edit
 *******************************************************************/
 // Configuration
#ifndef BE_LV_WIDGET_ARC
#define BE_LV_WIDGET_ARC 1
#endif
#ifndef BE_LV_WIDGET_BAR
#define BE_LV_WIDGET_BAR 1
#endif
#ifndef BE_LV_WIDGET_BTN
#define BE_LV_WIDGET_BTN 1
#endif
#ifndef BE_LV_WIDGET_BTNMATRIX
#define BE_LV_WIDGET_BTNMATRIX 1
#endif
#ifndef BE_LV_WIDGET_CALENDAR
#define BE_LV_WIDGET_CALENDAR 1
#endif
#ifndef BE_LV_WIDGET_CANVAS
#define BE_LV_WIDGET_CANVAS 1
#endif
#ifndef BE_LV_WIDGET_CHART
#define BE_LV_WIDGET_CHART 1
#endif
#ifndef BE_LV_WIDGET_CHECKBOX
#define BE_LV_WIDGET_CHECKBOX 1
#endif
#ifndef BE_LV_WIDGET_CONT
#define BE_LV_WIDGET_CONT 1
#endif
#ifndef BE_LV_WIDGET_CPICKER
#define BE_LV_WIDGET_CPICKER 1
#endif
#ifndef BE_LV_WIDGET_DROPDOWN
#define BE_LV_WIDGET_DROPDOWN 1
#endif
#ifndef BE_LV_WIDGET_GAUGE
#define BE_LV_WIDGET_GAUGE 1
#endif
#ifndef BE_LV_WIDGET_IMG
#define BE_LV_WIDGET_IMG 1
#endif
#ifndef BE_LV_WIDGET_IMGBTN
#define BE_LV_WIDGET_IMGBTN 1
#endif
#ifndef BE_LV_WIDGET_KEYBOARD
#define BE_LV_WIDGET_KEYBOARD 1
#endif
#ifndef BE_LV_WIDGET_LABEL
#define BE_LV_WIDGET_LABEL 1
#endif
#ifndef BE_LV_WIDGET_LED
#define BE_LV_WIDGET_LED 1
#endif
#ifndef BE_LV_WIDGET_LINE
#define BE_LV_WIDGET_LINE 1
#endif
#ifndef BE_LV_WIDGET_LINEMETER
#define BE_LV_WIDGET_LINEMETER 1
#endif
#ifndef BE_LV_WIDGET_LIST
#define BE_LV_WIDGET_LIST 1
#endif
#ifndef BE_LV_WIDGET_MSGBOX
#define BE_LV_WIDGET_MSGBOX 1
#endif
#ifndef BE_LV_WIDGET_OBJMASK
#define BE_LV_WIDGET_OBJMASK 1
#endif
#ifndef BE_LV_WIDGET_TEMPL
#define BE_LV_WIDGET_TEMPL 1
#endif
#ifndef BE_LV_WIDGET_PAGE
#define BE_LV_WIDGET_PAGE 1
#endif
#ifndef BE_LV_WIDGET_ROLLER
#define BE_LV_WIDGET_ROLLER 1
#endif
#ifndef BE_LV_WIDGET_SLIDER
#define BE_LV_WIDGET_SLIDER 1
#endif
#ifndef BE_LV_WIDGET_SPINBOX
#define BE_LV_WIDGET_SPINBOX 1
#endif
#ifndef BE_LV_WIDGET_SPINNER
#define BE_LV_WIDGET_SPINNER 1
#endif
#ifndef BE_LV_WIDGET_SWITCH
#define BE_LV_WIDGET_SWITCH 1
#endif
#ifndef BE_LV_WIDGET_TABLE
#define BE_LV_WIDGET_TABLE 1
#endif
#ifndef BE_LV_WIDGET_TABVIEW
#define BE_LV_WIDGET_TABVIEW 1
#endif
#ifndef BE_LV_WIDGET_TEXTAREA
#define BE_LV_WIDGET_TEXTAREA 1
#endif
#ifndef BE_LV_WIDGET_TILEVIEW
#define BE_LV_WIDGET_TILEVIEW 1
#endif
#ifndef BE_LV_WIDGET_WIN
#define BE_LV_WIDGET_WIN 1
#endif
/********************************************************************/

extern void start_lvgl(const char * uconfig);
extern void lv_ex_get_started_1(void);

/*********************************************************************************************\
 * Calling any LVGL function with auto-mapping
 * 
\*********************************************************************************************/

// check input parameters, and create callbacks if needed
// change values in place
//
// Format:
// - either a lowercase character encoding for a simple type
//   - 'b': bool
//   - 'i': int (int32_t)
//   - 's': string (const char *)
//
// - a class name surroungded by parenthesis
//   - '(lv_button)' -> lv_button class or derived
//
// - a callback, only 6 callbacks supported 0..5
//   - '&1' callback 1
//
void be_check_arg_type(bvm *vm, int32_t argc, const char * arg_type, int32_t p[5]);
void be_check_arg_type(bvm *vm, int32_t argc, const char * arg_type, int32_t p[5]) {
  bool arg_type_check = (arg_type != nullptr);      // is type checking activated
  int32_t arg_idx = 0;    // position in arg_type string
  char type_short_name[16];

  for (uint32_t i = 0; i < argc; i++) {
    type_short_name[0] = 0;   // clear string
    // extract individual type
    if (nullptr != arg_type) {
      switch (arg_type[arg_idx]) {
        case '.':
        case 'a'...'z':
          type_short_name[0] = arg_type[arg_idx];
          type_short_name[1] = 0;
          arg_idx++;
          break;
        case '&':
          type_short_name[0] = arg_type[arg_idx+1];
          type_short_name[1] = 0;
          arg_idx += 2;
          break;
        case '(':
          {
            arg_idx++;
            uint32_t offset = 0;
            while (arg_type[arg_idx + offset] != ')' && arg_type[arg_idx + offset] != 0) {
              type_short_name[offset] = arg_type[arg_idx + offset];
              type_short_name[offset+1] = 0;
              offset++;
            }
            if (arg_type[arg_idx + offset] == 0) {
              arg_type = nullptr;   // stop iterations
            }
            arg_idx += offset + 1;
          }
          break;
        case 0:
          arg_type = nullptr;   // stop iterations
          break;
      }
    }
    // berry_log_P(">> be_call_c_func arg %i, type %s", i, arg_type_check ? type_short_name : "<null>");
    p[i] = be_convert_single_elt(vm, i+1, arg_type_check ? type_short_name : nullptr, p[0]);
  }

  // check if we are missing arguments
  if (arg_type != nullptr && arg_type[arg_idx] != 0) {
    berry_log_P("Missing arguments, remaining type '%s'", &arg_type[arg_idx]);
  }
}

typedef int32_t (*fn_any_callable)(int32_t p0, int32_t p1, int32_t p2, int32_t p3, int32_t p4);
extern "C" {

  void lv_init_set_member(bvm *vm, int index, void * ptr);
  
  // called programmatically
  int lvx_init_2(bvm *vm, void * func, const char * return_type, const char * arg_type = nullptr);
  int lvx_init_2(bvm *vm, void * func, const char * return_type, const char * arg_type) {
    int argc = be_top(vm);
    lv_obj_t * obj1 = nullptr;
    lv_obj_t * obj2 = nullptr;

    if (argc > 1) {
      obj1 = (lv_obj_t*) be_convert_single_elt(vm, 2);
    }
    if (argc > 2) {
      obj2 = (lv_obj_t*) be_convert_single_elt(vm, 3);
    }
    // AddLog(LOG_LEVEL_INFO, "argc %d obj1 %p obj2 %p", argc, obj1, obj2);
    fn_any_callable f = (fn_any_callable) func;
    // AddLog(LOG_LEVEL_INFO, ">> be_call_c_func(%p) - %p,%p,%p,%p,%p", f, p[0], p[1], p[2], p[3], p[4]);
    lv_obj_t * obj;
    if ((int32_t)obj1 == -1) {  // special semantics of first ptr is -1, then just encapsulate
      obj = obj2;
    } else {                    // otherwise call the LVGL creator
      obj = (lv_obj_t*) (*f)((int32_t)obj1, (int32_t)obj2, 0, 0, 0);
    }
    lv_init_set_member(vm, 1, obj);
    be_return_nil(vm);
  }

  int be_call_c_func(bvm *vm, void * func, const char * return_type, const char * arg_type);

  // native closure to call `be_call_c_func`
  int lvx_call_c(bvm *vm) {
    // berry_log_C("lvx_call_c enter");
    // keep parameters unchanged
    be_getupval(vm, 0, 0);    // if index is zero, it's the current native closure
    void * func = be_tocomptr(vm, -1);
    be_getupval(vm, 0, 1);    // if index is zero, it's the current native closure
    const char * return_type = be_tostring(vm, -1);
    be_getupval(vm, 0, 2);    // if index is zero, it's the current native closure
    const char * arg_type = be_tostring(vm, -1);
    be_pop(vm, 3);            // remove 3 upvals

    // berry_log_C("lvx_call_c %p '%s' <- (%s)", func, return_type, arg_type);
    return be_call_c_func(vm, func, return_type, arg_type);
  }

  // virtual method, arg1: instance, arg2: name of method
  int lvx_member(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 2 && be_isinstance(vm, 1) && be_isstring(vm, 2)) {
      const char * method_name = be_tostring(vm, 2);    // the method we are looking for
      while (be_isinstance(vm, 1)) {
        const char * class_name = be_classname(vm, 1);
        // berry_log_C("lvx_member looking for method '%s' of class '%s'", method_name, class_name);

        // look for class descriptor
        int32_t class_idx = bin_search(class_name, &lv_classes[0].name, sizeof(lv_classes[0]), lv_classes_size);
        if (class_idx < 0) {
          // class not found, abort
          // berry_log_C("lvx_member class not found");
          be_return_nil(vm);
        }
        const lvbe_call_c_t * methods_calls = lv_classes[class_idx].func_table;
        size_t methods_size = lv_classes[class_idx].size;

        int32_t method_idx = bin_search(method_name, methods_calls, sizeof(lvbe_call_c_t), methods_size);
        if (method_idx >= 0) {
          // method found
          const lvbe_call_c_t * method = &methods_calls[method_idx];
          // berry_log_C("lvx_member method found func=%p return_type=%s arg_type=%s", method->func, method->return_type, method->arg_type);
          // push native closure
          be_pushntvclosure(vm, &lvx_call_c, 3);   // 3 upvals

          be_pushcomptr(vm, method->func);
          be_setupval(vm, -2, 0);
          be_pop(vm, 1);

          be_pushstring(vm, method->return_type);
          be_setupval(vm, -2, 1);
          be_pop(vm, 1);

          be_pushstring(vm, method->arg_type);
          be_setupval(vm, -2, 2);
          be_pop(vm, 1);

          // all good
          be_return(vm);
        }

        // get super if any, or nil if none
        be_getsuper(vm, 1);
        be_moveto(vm, -1, 1);
        be_pop(vm, 1);
      }
      // berry_log_C("lvx_member method not found");
      be_return_nil(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int be_call_c_func(bvm *vm, void * func, const char * return_type, const char * arg_type) {
    int32_t p[5] = {0,0,0,0,0};
    int32_t argc = be_top(vm); // Get the number of arguments

    // check if we call a constructor
    if (return_type && return_type[0] == '+') {
      return_type++;    // skip the leading '+'
      return lvx_init_2(vm, func, return_type);
    }

    fn_any_callable f = (fn_any_callable) func;
    be_check_arg_type(vm, argc, arg_type, p);
    // AddLog(LOG_LEVEL_INFO, ">> be_call_c_func(%p) - %p,%p,%p,%p,%p - %s", f, p[0], p[1], p[2], p[3], p[4], return_type);
    int32_t ret = (*f)(p[0], p[1], p[2], p[3], p[4]);
    // AddLog(LOG_LEVEL_INFO, ">> be_call_c_func, ret = %p", ret);
    if ((return_type == nullptr) || (strlen(return_type) == 0))       { be_return_nil(vm); }  // does not return
    else if (strlen(return_type) == 1) {
      switch (return_type[0]) {
        case '.':   // fallback next
        case 'i':   be_pushint(vm, ret); break;
        case 'b':   be_pushbool(vm, ret);  break;
        case 's':   be_pushstring(vm, (const char*) ret);  break;
        default:    be_raise(vm, "internal_error", "Unsupported return type"); break;
      }
      be_return(vm);
    } else { // class name
      // AddLog(LOG_LEVEL_INFO, ">> be_call_c_func, create_obj", ret);
      be_getglobal(vm, return_type);  // stack = class
      be_pushcomptr(vm, (void*) -1);         // stack = class, -1
      be_pushcomptr(vm, (void*) ret);         // stack = class, -1, ptr
      be_call(vm, 2);                 // instanciate with 2 arguments, stack = instance, -1, ptr
      be_pop(vm, 2);                  // stack = instance
      be_return(vm);
    }
  }
}

/*********************************************************************************************\
 * Native functions mapped to Berry functions
 * 
 * import power
 * 
 * power.read() -> map
 * 
\*********************************************************************************************/
extern "C" {

#include "be_exec.h"
#include "be_vm.h"
#include "be_mem.h"

  /*********************************************************************************************\
   * Support for lv_color
  \*********************************************************************************************/
  inline lv_color_t lv_color_from_uint32(uint32_t ucol) {
    lv_color_t * col = (lv_color_t*) &ucol;
    return *col;
  }
  inline uint32_t lv_color_to_uint32(lv_color_t col) {
    uint32_t *p = (uint32_t*) &col;
    return *p;
  }

  // lv_color
  int lco_init(bvm *vm) {
    int argc = be_top(vm);
    uint32_t color = 0x0000;    // default to black
    if (argc > 1 && be_isint(vm, 2)) {
      color = be_toint(vm, 2);
    }
    lv_color_t lv_color = lv_color_hex(color);
    be_pushint(vm, lv_color_to_uint32(lv_color));
    be_setmember(vm, 1, ".p");
    be_return_nil(vm);
  }

  int lco_tostring(bvm *vm) {
    lv_color_t lv_color = {};
    be_getmember(vm, 1, ".p");
    uint32_t ntv_color = be_toint(vm, -1);
    lv_color = lv_color_from_uint32(ntv_color);
    uint32_t color = lv_color_to32(lv_color);
    be_pop(vm, 1);  // remove attribute
    char s[48];
    snprintf(s, sizeof(s), "lv_color(0x%06x - native:0x%04x)", color, ntv_color);
    be_pushnstring(vm, s, strlen(s)); /* make escape string from buffer */
    be_return(vm);
  }


  /*********************************************************************************************\
   * Support for lv
  \*********************************************************************************************/
  // Get the `.p` member of instance at `index`
  void * lv_get_arg(bvm *vm, int index) {
    void * ret = nullptr;
    if (be_isinstance(vm, index)) {
      be_getmember(vm, index, ".p");
      ret = be_tocomptr(vm, -1);
      be_pop(vm, 1);  // remove .p attribute
    }
    return ret;
  }

  // called during init, set the `.p` member with the pointer
  void lv_init_set_member(bvm *vm, int index, void * ptr) {
    if (ptr == nullptr) {
        be_throw(vm, BE_MALLOC_FAIL);
    }
    be_pushcomptr(vm, ptr);
    be_setmember(vm, index, ".p");
    be_pop(vm, 1);
  }

  // function is (void) -> lv_obt_t*
  typedef lv_obj_t* (*fn_lvobj__void)(void);  // f() -> newly created lv_obj()
  int lv0_lvobj__void_call(bvm *vm, fn_lvobj__void func) {
    lv_obj_t * obj = (*func)();
    be_getglobal(vm, LV_OBJ_CLASS); // stack = class
    be_pushcomptr(vm, (void*) -1);         // stack = class, -1
    be_pushcomptr(vm, (void*) obj);         // stack = class, -1, ptr
    be_call(vm, 2);                 // instanciate, stack = instance (don't call init() )
    be_pop(vm, 2);                  // stack = instance
    be_return(vm);
  }
  
  int lv0_load_font(bvm *vm) {
    int argc = be_top(vm);
    if (argc == 1 && be_isstring(vm, 1)) {
      lv_font_t * font = lv_font_load(be_tostring(vm, 1));
      if (font != nullptr) {
        be_getglobal(vm, "lv_font");
        be_pushcomptr(vm, font);
        be_call(vm, 1);
        be_pop(vm, 1);
        be_return(vm);
      } else {
        be_return_nil(vm);
      }
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int lv0_load_freetype_font(bvm *vm) {
#ifdef USE_LVGL_FREETYPE
    int argc = be_top(vm);
    if (argc == 3 && be_isstring(vm, 1) && be_isint(vm, 2) && be_isint(vm, 3)) {
      lv_ft_info_t info;
      info.name = be_tostring(vm, 1);
      info.weight = be_toint(vm, 2);
      info.style = be_toint(vm, 3);
      lv_ft_font_init(&info);
      lv_font_t * font = info.font;

      if (font != nullptr) {
        be_getglobal(vm, "lv_font");
        be_pushcomptr(vm, font);
        be_call(vm, 1);
        be_pop(vm, 1);
        be_return(vm);
      } else {
        be_return_nil(vm);
      }
    }
    be_raise(vm, kTypeError, nullptr);
#else // USE_LVGL_FREETYPE
    be_raise(vm, "feature_error", "FreeType fonts are not available, use '#define USE_LVGL_FREETYPE 1'");
#endif // USE_LVGL_FREETYPE
  }

  int lv0_load_montserrat_font(bvm *vm) {
    int argc = be_top(vm);
    if (argc == 1 && be_isint(vm, 1)) {
      lv_font_t * font = nullptr;
      int32_t   font_size = be_toindex(vm, 1);

      switch (font_size) {

        #if LV_FONT_MONTSERRAT_8
        case  8:
          font = &lv_font_montserrat_8;
          break;
        #endif

        #if LV_FONT_MONTSERRAT_10
        case  10:
          font = &lv_font_montserrat_10;
          break;
        #endif

        #if LV_FONT_MONTSERRAT_12
        case  12:
          font = &lv_font_montserrat_12;
          break;
        #endif

        #if LV_FONT_MONTSERRAT_14
        case  14:
          font = &lv_font_montserrat_14;
          break;
        #endif

        #if LV_FONT_MONTSERRAT_16
        case  16:
          font = &lv_font_montserrat_16;
          break;
        #endif

        #if LV_FONT_MONTSERRAT_18
        case  18:
          font = &lv_font_montserrat_18;
          break;
        #endif

        #if LV_FONT_MONTSERRAT_20
        case  20:
          font = &lv_font_montserrat_20;
          break;
        #endif

        #if LV_FONT_MONTSERRAT_22
        case  22:
          font = &lv_font_montserrat_22;
          break;
        #endif

        #if LV_FONT_MONTSERRAT_24
        case  24:
          font = &lv_font_montserrat_24;
          break;
        #endif

        #if LV_FONT_MONTSERRAT_26
        case  26:
          font = &lv_font_montserrat_26;
          break;
        #endif

        #if LV_FONT_MONTSERRAT_28
        case  28:
          font = &lv_font_montserrat_28;
          break;
        #endif

        #if LV_FONT_MONTSERRAT_30
        case  30:
          font = &lv_font_montserrat_30;
          break;
        #endif

        #if LV_FONT_MONTSERRAT_32
        case  32:
          font = &lv_font_montserrat_32;
          break;
        #endif

        #if LV_FONT_MONTSERRAT_34
        case  34:
          font = &lv_font_montserrat_34;
          break;
        #endif

        #if LV_FONT_MONTSERRAT_36
        case  36:
          font = &lv_font_montserrat_36;
          break;
        #endif

        #if LV_FONT_MONTSERRAT_38
        case  38:
          font = &lv_font_montserrat_38;
          break;
        #endif

        #if LV_FONT_MONTSERRAT_40
        case  40:
          font = &lv_font_montserrat_40;
          break;
        #endif

        #if LV_FONT_MONTSERRAT_42
        case  42:
          font = &lv_font_montserrat_42;
          break;
        #endif

        #if LV_FONT_MONTSERRAT_44
        case  44:
          font = &lv_font_montserrat_44;
          break;
        #endif

        #if LV_FONT_MONTSERRAT_46
        case  46:
          font = &lv_font_montserrat_46;
          break;
        #endif

        #if LV_FONT_MONTSERRAT_48
        case  48:
          font = &lv_font_montserrat_48;
          break;
        #endif

        #if LV_FONT_MONTSERRAT_28_COMPRESSED
        case 28:
          font = &lv_font_montserrat_28_compressed;
          break;
        #endif

        default:
          break;
      }

      if (font != nullptr) {
        be_getglobal(vm, "lv_font");
        be_pushcomptr(vm, font);
        be_call(vm, 1);
        be_pop(vm, 1);
        be_return(vm);
      } else {
        be_return_nil(vm);
      }
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int lv0_load_seg7_font(bvm *vm) {
    int argc = be_top(vm);
    if (argc == 1 && be_isint(vm, 1)) {
      lv_font_t * font = nullptr;
      int32_t   font_size = be_toindex(vm, 1);

      switch (font_size) {
        case  8: font = &seg7_8;  break;
        case 10: font = &seg7_10; break;
        case 12: font = &seg7_12; break;
        case 14: font = &seg7_14; break;
        case 16: font = &seg7_16; break;
        case 18: font = &seg7_18; break;
        case 20: font = &seg7_20; break;
        case 24: font = &seg7_24; break;
        case 28: font = &seg7_28; break;
        case 36: font = &seg7_36; break;
        case 48: font = &seg7_48; break;
        default: break;
      }

      if (font != nullptr) {
        be_getglobal(vm, "lv_font");
        be_pushcomptr(vm, font);
        be_call(vm, 1);
        be_pop(vm, 1);
        be_return(vm);
      } else {
        be_return_nil(vm);
      }
    }
    be_raise(vm, kTypeError, nullptr);
  }

  #include "lvgl_berry/tasmota_logo_64_truecolor_alpha.h"

  void lv_img_set_tasmota_logo(lv_obj_t * img) {
    lv_img_set_src(img, &tasmota_logo_64_truecolor);
  }

  /*********************************************************************************************\
   * LVGL top level virtual members
   * 
   * Responds to virtual constants
  \*********************************************************************************************/

  extern const lvbe_call_c_t lv_func[];
  extern const size_t lv_func_size;

  extern const be_constint_t lv0_constants[];
  extern const size_t lv0_constants_size;

  int lv0_member(bvm *vm);
  int lv0_member(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 1 && be_isstring(vm, 1)) {
      const char * needle = be_tostring(vm, 1);

      int32_t constant_idx = bin_search(needle, &lv0_constants[0].name, sizeof(lv0_constants[0]), lv0_constants_size);
    
      if (constant_idx >= 0) {
        // we did have a match, low == high
        be_pushint(vm, lv0_constants[constant_idx].value);
        be_return(vm);
      } else {
        // search for a method with this name

        int32_t method_idx = bin_search(needle, &lv_func[0].name, sizeof(lv_func[0]), lv_func_size);

        if (method_idx >= 0) {
          const lvbe_call_c_t * method = &lv_func[method_idx];
          // push native closure
          be_pushntvclosure(vm, &lvx_call_c, 3);   // 3 upvals

          be_pushcomptr(vm, method->func);
          be_setupval(vm, -2, 0);
          be_pop(vm, 1);

          be_pushstring(vm, method->return_type);
          be_setupval(vm, -2, 1);
          be_pop(vm, 1);

          be_pushstring(vm, method->arg_type);
          be_setupval(vm, -2, 2);
          be_pop(vm, 1);

          // all good
          be_return(vm);
        } else {
          be_return_nil(vm);
        }
      }
    }
    be_raise(vm, "attribute_error", "module 'lvgl' has no such attribute");
  }

  /*********************************************************************************************\
   * LVGL Start
   * 
   * Calls uDisplay and starts LVGL
  \*********************************************************************************************/
  // lv.start(instance, instance) -> nil
  int lv0_start(bvm *vm);
  int lv0_start(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 0 || (argc == 1 && be_isstring(vm, 1))) {
      const char * uconfig = nullptr;
      if (argc == 1) {
        be_tostring(vm, 1);
      }
      start_lvgl(uconfig);
      be_return_nil(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  /*********************************************************************************************\
   * LVGL Input Devices
   * 
   * Calls uDisplay and starts LVGL
   * 
   * lv.register_button_encoder([inv: bool]) -> nil
  \*********************************************************************************************/
  bool lvbe_encoder_with_keys_read(lv_indev_drv_t * drv, lv_indev_data_t*data);

  int lv0_register_button_encoder(bvm *vm);   // add buttons with encoder logic
  int lv0_register_button_encoder(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    bool inverted = false;
    // berry_log_P("lv0_register_button_encoder argc=%d inverted=%d", argc, be_tobool(vm, 1));
    if (argc >= 1) {
      inverted = be_tobool(vm, 1);    // get the inverted flag
    }
    // we need 3 buttons from the template
    int32_t btn0 = Pin(GPIO_INPUT, 0);
    int32_t btn1 = Pin(GPIO_INPUT, 1);
    int32_t btn2 = Pin(GPIO_INPUT, 2);
    if (btn0 < 0 || btn1 < 0 || btn2 < 0) {
      be_raise(vm, "template_error", "You need to configure GPIO Inputs 1/2/3");
    }
    lvbe.btn[0].set_gpio(btn0);
    lvbe.btn[0].set_inverted(inverted);
    lvbe.btn[1].set_gpio(btn1);
    lvbe.btn[1].set_inverted(inverted);
    lvbe.btn[2].set_gpio(btn2);
    lvbe.btn[2].set_inverted(inverted);
    berry_log_P(D_LOG_LVGL "Button Rotary encoder using GPIOs %d,%d,%d%s", btn0, btn1, btn2, inverted ? " (inverted)" : "");

    lv_indev_drv_init(&lvbe.indev_drv);
    lvbe.indev_drv.type = LV_INDEV_TYPE_ENCODER;
    lvbe.indev_drv.read_cb = lvbe_encoder_with_keys_read;

    lv_indev_t * indev = lv_indev_drv_register(&lvbe.indev_drv);
    lvbe.indev_list.addHead(indev);   // keep track of indevs

    be_getglobal(vm, "lv_indev");   // create an object of class lv_indev with the pointer
    be_pushint(vm, (int32_t) indev);
    be_call(vm, 1);
    be_pop(vm, 1);

    be_return(vm);
  }

  /*********************************************************************************************\
   * LVGL Input Devices - callbacks
  \*********************************************************************************************/

  // typedef struct {
  //   lv_point_t point; /**< For LV_INDEV_TYPE_POINTER the currently pressed point*/
  //   uint32_t key;     /**< For LV_INDEV_TYPE_KEYPAD the currently pressed key*/
  //   uint32_t btn_id;  /**< For LV_INDEV_TYPE_BUTTON the currently pressed button*/
  //   int16_t enc_diff; /**< For LV_INDEV_TYPE_ENCODER number of steps since the previous read*/

  //   lv_indev_state_t state; /**< LV_INDEV_STATE_REL or LV_INDEV_STATE_PR*/
  // } lv_indev_data_t;

  bool lvbe_encoder_with_keys_read(lv_indev_drv_t * drv, lv_indev_data_t*data){
    // scan through buttons if we need to report something
    uint32_t i;
    for (i = 0; i < 3; i++) {
      if (lvbe.btn[i].state_changed()) {
        switch (i) {
          case 0: data->key = LV_KEY_LEFT; break;
          case 1: data->key = LV_KEY_ENTER; break;
          case 2: data->key = LV_KEY_RIGHT; break;
          default: break;
        }
        bool state = lvbe.btn[i].clear_state_changed();
        data->state = state ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
        // berry_log_P("Button event key %d state %d,%d", data->key, state, data->state);
        break;
      }
    }

    // do we have more to report?
    bool more_to_report = false;
    for (/* continue where we left */; i < 3; i++) {
      if (lvbe.btn[i].state_changed()) {
        more_to_report = true;
      }
    }
    return more_to_report;
  }

  /*********************************************************************************************\
   * Support for lv_indev and objects that don't need creator
  \*********************************************************************************************/
  int lv0_init(bvm *vm);
  int lv0_init(bvm *vm) {
    void * obj = nullptr;
    int argc = be_top(vm);
    if (argc > 1) {
      obj = (void*) be_convert_single_elt(vm, 2);
    }
    lv_init_set_member(vm, 1, obj);
    be_return_nil(vm);
  }

  /*********************************************************************************************\
   * Support for lv_obj
  \*********************************************************************************************/
  int lvx_init(bvm *vm);
  int lvx_init(bvm *vm) {
    int argc = be_top(vm);
    lv_obj_t * obj = nullptr;

    if (argc > 1) {
      obj = (lv_obj_t*) be_convert_single_elt(vm, 2);
    }
    // AddLog(LOG_LEVEL_INFO, "argc %d lv_obj %p", argc, obj);
    if (obj == nullptr) {
      obj = lv_obj_create(nullptr, nullptr);
    }
    // AddLog(LOG_LEVEL_INFO, "lv_obj final %p", obj);
    lv_init_set_member(vm, 1, obj);
    be_return_nil(vm);
  }

  int lvx_tostring(bvm *vm) {
    lv_obj_t * obj = (lv_obj_t*) lv_get_arg(vm, 1);
    const char * classname = be_classname(vm, 1);
    char s[32];
    snprintf(s, sizeof(s), "<instance: %s(0x%08X)>", classname, obj);
    be_pushnstring(vm, s, strlen(s)); /* make escape string from buffer */
    be_return(vm);
  }

  /*********************************************************************************************\
   * Support for lv_style
  \*********************************************************************************************/
  int lvs_init(bvm *vm) {
    int argc = be_top(vm);
    lv_style_t * style = nullptr;

    if (argc > 1) {
      style = (lv_style_t*) be_convert_single_elt(vm, 2);
    }
    if (style == nullptr) {
      style = (lv_style_t*) be_malloc(vm, sizeof(lv_style_t));
      if (style != nullptr) {
        lv_style_init(style);
      }
    }
    lv_init_set_member(vm, 1, style);
    be_return_nil(vm);
  }

  int lvs_tostring(bvm *vm) {
    lv_style_t * style1 = (lv_style_t*) lv_get_arg(vm, 1);
    char s[32];
    snprintf(s, sizeof(s), "<instance: lv_style(0x%08X)>", style1);
    be_pushnstring(vm, s, strlen(s)); /* make escape string from buffer */
    be_return(vm);
  }
  /*********************************************************************************************\
   * Screenshot in raw format
  \********************************************************************************************/
  int lv0_screenshot(bvm *vm);
  int lv0_screenshot(bvm *vm) {
    if (!glue) { be_return_nil(vm); }

    char fname[32];
    snprintf(fname, sizeof(fname), "/screenshot-%d.raw", Rtc.utc_time);
    File f = dfsp->open(fname, "w");
    if (f) {
      glue->setScreenshotFile(&f);

      // redraw screen
      lv_obj_invalidate(lv_scr_act());
      lv_refr_now(lv_disp_get_default());            

      glue->stopScreenshot();
      f.close();
    }
    be_pushstring(vm, fname);
    be_return(vm);
  }
}

#include "lvgl_berry/be_lv_c_mapping.h"

#else // USE_LVGL

//   // define weak aliases
//   int32_t b_nrg_read(struct bvm *vm) __attribute__ ((weak, alias ("b_wire_energymissing")));



#endif // USE_LVGL

#endif  // USE_BERRY
