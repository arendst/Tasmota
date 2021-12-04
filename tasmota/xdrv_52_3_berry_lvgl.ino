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
#include "be_ctypes.h"
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
//   - '[lv_event_cb]' -> callback type, still prefixed with '^' to mark that it is cb
//
void be_check_arg_type(bvm *vm, int32_t arg_start, int32_t argc, const char * arg_type, int32_t p[8]);
void be_check_arg_type(bvm *vm, int32_t arg_start, int32_t argc, const char * arg_type, int32_t p[8]) {
  bool arg_type_check = (arg_type != nullptr);      // is type checking activated
  int32_t arg_idx = 0;    // position in arg_type string
  char type_short_name[32];

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
        case '(':
        case '^':
          {
            uint32_t prefix = 0;
            if (arg_type[arg_idx] == '^') {
              type_short_name[0] = '^';
              type_short_name[1] = 0;
              prefix = 1;
            }
            uint32_t offset = 0;
            arg_idx++;
            while (arg_type[arg_idx + offset] != ')' && arg_type[arg_idx + offset] != '^' && arg_type[arg_idx + offset] != 0 && offset+prefix+1 < sizeof(type_short_name)) {
              type_short_name[offset+prefix] = arg_type[arg_idx + offset];
              type_short_name[offset+prefix+1] = 0;
              offset++;
            }
            if (arg_type[arg_idx + offset] == 0) {
              arg_type = nullptr;   // no more parameters, stop iterations
            }
            arg_idx += offset + 1;
          }
          break;
        case 0:
          arg_type = nullptr;   // stop iterations
          break;
      }
    }
    // AddLog(LOG_LEVEL_INFO, ">> be_call_c_func arg %i, type %s", i, arg_type_check ? type_short_name : "<null>");
    p[i] = be_convert_single_elt(vm, i + arg_start, arg_type_check ? type_short_name : nullptr, p[0]);
  }

  // check if we are missing arguments
  if (arg_type != nullptr && arg_type[arg_idx] != 0) {
    berry_log_P("Missing arguments, remaining type '%s'", &arg_type[arg_idx]);
  }
}

typedef int32_t (*fn_any_callable)(int32_t p0, int32_t p1, int32_t p2, int32_t p3,
                                   int32_t p4, int32_t p5, int32_t p6, int32_t p7);
extern "C" {

  void lv_init_set_member(bvm *vm, int index, void * ptr);
  
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
        if (class_idx >= 0) {
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
    // AddLog(LOG_LEVEL_INFO, ">> be_call_c_func, func=%p, return_type=%s, arg_type=%s", func, return_type ? return_type : "", arg_type ? arg_type : "");
    int32_t p[8] = {0,0,0,0,0,0,0,0};
    int32_t argc = be_top(vm); // Get the number of arguments

    // the following describe the active payload for the C function (start and count)
    // this is because the `init()` constructor first arg is not passed to the C function
    int32_t arg_start = 1;      // start with standard values
    int32_t arg_count = argc;

    // check if we call a constructor, in this case we store the return type into the new object
    // check if we call a constructor with a comptr as first arg
    if (return_type && return_type[0] == '+') {
      if (argc > 1 && be_iscomptr(vm, 2)) {
        lv_obj_t * obj = (lv_obj_t*) be_tocomptr(vm, 2);
        lv_init_set_member(vm, 1, obj);
        be_return_nil(vm);
      } else {
        // we need to discard the first arg
        arg_start++;
        arg_count--;
      }
    }

    fn_any_callable f = (fn_any_callable) func;
    // AddLog(LOG_LEVEL_INFO, ">> before be_check_arg_type argc=%i - %i", arg_count, arg_start);
    be_check_arg_type(vm, arg_start, arg_count, arg_type, p);
    // AddLog(LOG_LEVEL_INFO, ">> be_call_c_func(%p) - %p,%p,%p,%p,%p - %s", f, p[0], p[1], p[2], p[3], p[4], return_type ? return_type : "NULL");
    int32_t ret = (*f)(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
    // AddLog(LOG_LEVEL_INFO, ">> be_call_c_func, ret = %p", ret);
    if ((return_type == nullptr) || (strlen(return_type) == 0))       { be_return_nil(vm); }  // does not return
    else if (return_type[0] == '+') {
      lv_obj_t * obj = (lv_obj_t*) ret;
      lv_init_set_member(vm, 1, obj);
      be_return_nil(vm);
    }
    else if (strlen(return_type) == 1) {
      switch (return_type[0]) {
        case '.':   // fallback next
        case 'i':   be_pushint(vm, ret); break;
        case 'b':   be_pushbool(vm, ret);  break;
        case 's':   be_pushstring(vm, (const char*) ret);  break;
        case 'c':   be_pushint(vm, ret); break; // TODO missing 'c' general callback type
        default:    be_raise(vm, "internal_error", "Unsupported return type"); break;
      }
      be_return(vm);
    } else { // class name
      // AddLog(LOG_LEVEL_INFO, ">> be_call_c_func, create_obj ret=%i return_type=%s", ret, return_type);
      be_find_class(vm, return_type);
      be_pushcomptr(vm, (void*) ret);         // stack = class, ptr
      be_pushcomptr(vm, (void*) -1);         // stack = class, ptr, -1
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
    uint16_t *p = (uint16_t*) &col;
    return *p;
  }

  // lv_color
  // First arg is a 24 bits RGB color
  // If first arg is `nil` second arg is the native value of color
  int lco_init(bvm *vm) {
    int argc = be_top(vm);
    uint32_t color32 = 0x000000;    // default to black

    if (argc > 1) {
      color32 = be_toint(vm, 2);
    }
    lv_color_t lv_color = lv_color_hex(color32);
    if (argc > 2 && be_toint(vm, 3) == -1) {
      lv_color.full = be_toint(vm, 2);
    }
    be_pushint(vm, lv_color_to_uint32(lv_color));
    be_setmember(vm, 1, "_p");
    be_return_nil(vm);
  }

  int lco_tostring(bvm *vm) {
    lv_color_t lv_color = {};
    be_getmember(vm, 1, "_p");
    uint32_t ntv_color = be_toint(vm, -1);
    lv_color = lv_color_from_uint32(ntv_color);
    uint32_t color = lv_color_to32(lv_color) & 0xFFFFFF;
    be_pop(vm, 1);  // remove attribute
    char s[48];
    snprintf(s, sizeof(s), "lv_color(0x%06x - native:0x%04x)", color, ntv_color);
    be_pushnstring(vm, s, strlen(s)); /* make escape string from buffer */
    be_return(vm);
  }

  int lco_toint(bvm *vm) {
    lv_color_t lv_color = {};
    be_getmember(vm, 1, "_p");
    uint32_t ntv_color = be_toint(vm, -1);
    be_pushint(vm, ntv_color);
    be_return(vm);
  }


  /*********************************************************************************************\
   * Support for lv
  \*********************************************************************************************/
  // Get the `_p` member of instance at `index`
  void * lv_get_arg(bvm *vm, int index) {
    void * ret = nullptr;
    if (be_isinstance(vm, index)) {
      be_getmember(vm, index, "_p");
      ret = be_tocomptr(vm, -1);
      be_pop(vm, 1);  // remove _p attribute
    }
    return ret;
  }

  // called during init, set the `_p` member with the pointer
  void lv_init_set_member(bvm *vm, int index, void * ptr) {
    be_pushcomptr(vm, ptr);
    be_setmember(vm, index, "_p");
    be_pop(vm, 1);
  }

  // function is (void) -> lv_obt_t*
  typedef lv_obj_t* (*fn_lvobj__void)(void);  // f() -> newly created lv_obj()
  int lv0_lvobj__void_call(bvm *vm, fn_lvobj__void func) {
    lv_obj_t * obj = (*func)();
    be_find_class(vm, "lv.lv_obj");
    be_pushcomptr(vm, (void*) -1);         // stack = class, -1
    be_pushcomptr(vm, (void*) obj);         // stack = class, -1, ptr
    be_call(vm, 2);                 // instanciate, stack = instance (don't call init() )
    be_pop(vm, 2);                  // stack = instance
    be_return(vm);
  }
  
  /*********************************************************************************************\
   * Support for lv_fonts
  \*********************************************************************************************/
  // load font by name on file-system
  int lv0_load_font(bvm *vm) {
    int argc = be_top(vm);
    if (argc == 1 && be_isstring(vm, 1)) {
      lv_font_t * font = lv_font_load(be_tostring(vm, 1));
      if (font != nullptr) {
        be_find_class(vm, "lv.lv_font");
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

  /*********************************************************************************************\
   * Support for Freetype fonts
  \*********************************************************************************************/
  // load freetype font by name in file-system
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
        be_find_class(vm, "lv.lv_font");
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

  /*********************************************************************************************\
   * Support for embedded fonts in Flash
  \*********************************************************************************************/
  // We create tables for Font matching
  // Size of `0` indicates end of table
  typedef struct {
    int16_t size;
    const lv_font_t *font;
  } lv_font_table_t;

  typedef struct {
    const char * name;
    const lv_font_table_t * table;
  } lv_font_names_t;

  // Montserrat Font
  const lv_font_table_t lv_montserrat_fonts[] = {
  #if LV_FONT_MONTSERRAT_8
    {  8, &lv_font_montserrat_8 },
  #endif
  #if LV_FONT_MONTSERRAT_10
    { 10, &lv_font_montserrat_10 },
  #endif
  #if LV_FONT_MONTSERRAT_12
    { 12, &lv_font_montserrat_12 },
  #endif
  #if LV_FONT_MONTSERRAT_14
    { 14, &lv_font_montserrat_14 },
  #endif
  #if LV_FONT_MONTSERRAT_16
    { 16, &lv_font_montserrat_16 },
  #endif
  #if LV_FONT_MONTSERRAT_18
    { 18, &lv_font_montserrat_18 },
  #endif
  #if LV_FONT_MONTSERRAT_20
    { 20, &lv_font_montserrat_20 },
  #endif
  #if LV_FONT_MONTSERRAT_22
    { 22, &lv_font_montserrat_22 },
  #endif
  #if LV_FONT_MONTSERRAT_24
    { 24, &lv_font_montserrat_24 },
  #endif
  #if LV_FONT_MONTSERRAT_26
    { 26, &lv_font_montserrat_26 },
  #endif
  #if LV_FONT_MONTSERRAT_28
    { 28, &lv_font_montserrat_28 },
  #endif
  #if LV_FONT_MONTSERRAT_28_COMPRESSED
    { 28, &lv_font_montserrat_28_compressed, },
  #endif
  #if LV_FONT_MONTSERRAT_30
    { 30, &lv_font_montserrat_30 },
  #endif
  #if LV_FONT_MONTSERRAT_32
    { 32, &lv_font_montserrat_32 },
  #endif
  #if LV_FONT_MONTSERRAT_34
    { 34, &lv_font_montserrat_34 },
  #endif
  #if LV_FONT_MONTSERRAT_36
    { 36, &lv_font_montserrat_36 },
  #endif
  #if LV_FONT_MONTSERRAT_38
    { 38, &lv_font_montserrat_38 },
  #endif
  #if LV_FONT_MONTSERRAT_40
    { 40, &lv_font_montserrat_40 },
  #endif
  #if LV_FONT_MONTSERRAT_42
    { 42, &lv_font_montserrat_42 },
  #endif
  #if LV_FONT_MONTSERRAT_44
    { 44, &lv_font_montserrat_44 },
  #endif
  #if LV_FONT_MONTSERRAT_46
    { 46, &lv_font_montserrat_46 },
  #endif
  #if LV_FONT_MONTSERRAT_48
    { 48, &lv_font_montserrat_48 },
  #endif
    { 0, nullptr}
  };

  // Seg7 Font
  const lv_font_table_t lv_seg7_fonts[] = {
    {  8, &seg7_8 },
    { 10, &seg7_10 },
    { 12, &seg7_12 },
    { 14, &seg7_14 },
    { 16, &seg7_16 },
    { 18, &seg7_18 },
    { 20, &seg7_20 },
    { 24, &seg7_24 },
    { 28, &seg7_28 },
    { 36, &seg7_36 },
    { 48, &seg7_48 },
  };

  // robotocondensed-latin1
  const lv_font_table_t lv_robotocondensed_fonts[] = {
#if ROBOTOCONDENSED_REGULAR_12_LATIN1
    { 12, &robotocondensed_regular_12_latin1 },
#endif
#if ROBOTOCONDENSED_REGULAR_14_LATIN1
    { 14, &robotocondensed_regular_14_latin1 },
#endif
#if ROBOTOCONDENSED_REGULAR_16_LATIN1
    { 16, &robotocondensed_regular_16_latin1 },
#endif
#if ROBOTOCONDENSED_REGULAR_20_LATIN1
    { 20, &robotocondensed_regular_20_latin1 },
#endif
#if ROBOTOCONDENSED_REGULAR_22_LATIN1
    { 22, &robotocondensed_regular_22_latin1 },
#endif
#if ROBOTOCONDENSED_REGULAR_24_LATIN1
    { 24, &robotocondensed_regular_24_latin1 },
#endif
#if ROBOTOCONDENSED_REGULAR_28_LATIN1
    { 28, &robotocondensed_regular_28_latin1 },
#endif
#if ROBOTOCONDENSED_REGULAR_32_LATIN1
    { 32, &robotocondensed_regular_32_latin1 },
#endif
#if ROBOTOCONDENSED_REGULAR_36_LATIN1
    { 36, &robotocondensed_regular_36_latin1 },
#endif
#if ROBOTOCONDENSED_REGULAR_38_LATIN1
    { 38, &robotocondensed_regular_38_latin1 },
#endif
#if ROBOTOCONDENSED_REGULAR_40_LATIN1
    { 40, &robotocondensed_regular_40_latin1 },
#endif
#if ROBOTOCONDENSED_REGULAR_44_LATIN1
    { 44, &robotocondensed_regular_44_latin1 },
#endif
#if ROBOTOCONDENSED_REGULAR_48_LATIN1
    { 48, &robotocondensed_regular_48_latin1 },
#endif
  };

  // register all included fonts
  const lv_font_names_t lv_embedded_fonts[] = {
    { "montserrat", lv_montserrat_fonts },
    { "seg7", lv_seg7_fonts },
#ifdef USE_LVGL_OPENHASP
    { "robotocondensed", lv_robotocondensed_fonts },
#endif
    { nullptr, nullptr}
  };

  // If size is zero, it is read at arg 1
  int lv_load_embedded_font(bvm *vm, const char * name, int16_t size) {
    if (0 == size) {
      if (be_top(vm) >= 1 && be_isint(vm, 1)) {
        size = be_toindex(vm, 1);
      }
    }
    if (name == nullptr || 0 == size) {
      be_raise(vm, "value_error", "");
    }
    // first look for font
    const lv_font_names_t * font_name_cursor = lv_embedded_fonts;
    for (font_name_cursor = lv_embedded_fonts; font_name_cursor->name; font_name_cursor++) {
      if (strcmp(name, font_name_cursor->name) == 0) break;   // found
    }
    if (font_name_cursor->name == nullptr) {
      be_raisef(vm, "value_error", "unknown font '%s'", name);
    }
    // scan for font size
    const lv_font_table_t * font_entry = font_name_cursor->table;
    for (font_entry = font_name_cursor->table; font_entry->size; font_entry++) {
      if (font_entry->size == size) break;    // found
    }
    if (font_entry->size == 0) {
      be_raisef(vm, "value_error", "unknown font size '%s-%i'", name, size);
    }
    
    be_find_class(vm, "lv.lv_font");
    be_pushcomptr(vm, (void*)font_entry->font);
    be_call(vm, 1);
    be_pop(vm, 1);
    be_return(vm);
  }

  int lv0_load_montserrat_font(bvm *vm) {
    return lv_load_embedded_font(vm, "montserrat", 0);
  }

  int lv0_load_seg7_font(bvm *vm) {
    return lv_load_embedded_font(vm, "seg7", 0);
  }

  int lv0_load_robotocondensed_latin1_font(bvm *vm) {
#ifdef USE_LVGL_OPENHASP
    return lv_load_embedded_font(vm, "robotocondensed", 0);
#endif // USE_LVGL_OPENHASP
    be_raise(vm, kTypeError, nullptr);
  }

  /*********************************************************************************************\
   * Tasmota Logo
  \*********************************************************************************************/
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

  extern const be_ctypes_class_by_name_t be_ctypes_lvgl_classes[];
  extern const size_t be_ctypes_lvgl_classes_size;

  int lv0_member(bvm *vm);
  int lv0_member(bvm *vm) {
    // first try the standard way
    if (be_module_member(vm, lv0_constants, lv0_constants_size)) {
      be_return(vm);
    }
    // try alternative members
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 1 && be_isstring(vm, 1)) {
      const char * needle = be_tostring(vm, 1);
      int32_t idx;

      // search for a class with this name
      char cl_prefixed[32];
      snprintf(cl_prefixed, sizeof(cl_prefixed), "lv_%s", needle);    // we try both actual name and prefixed with `lv_` so both `lv.obj` and `lv.lv_obj` work
      idx = bin_search(cl_prefixed, &lv_classes[0].name, sizeof(lv_classes[0]), lv_classes_size);
      if (idx < 0) {
        idx = bin_search(needle, &lv_classes[0].name, sizeof(lv_classes[0]), lv_classes_size);
      }
      if (idx >= 0) {
        // we did have a match
        be_pushntvclass(vm, lv_classes[idx].cl);
        be_return(vm);
      }
      // same search for ctypes
      idx = bin_search(cl_prefixed, &be_ctypes_lvgl_classes[0].name, sizeof(be_ctypes_lvgl_classes[0]), be_ctypes_lvgl_classes_size);
      if (idx < 0) {
        idx = bin_search(needle, &be_ctypes_lvgl_classes[0].name, sizeof(be_ctypes_lvgl_classes[0]), be_ctypes_lvgl_classes_size);
      }
      if (idx >= 0) {
        // we did have a match
        be_pushntvclass(vm, be_ctypes_lvgl_classes[idx].cl);
        be_return(vm);
      }

      // search for a method with this name
      idx = bin_search(needle, &lv_func[0].name, sizeof(lv_func[0]), lv_func_size);
      if (idx >= 0) {
        const lvbe_call_c_t * method = &lv_func[idx];
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
    }
    be_return_nil(vm);
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
        uconfig = be_tostring(vm, 1);
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
  void lvbe_encoder_with_keys_read(lv_indev_drv_t * drv, lv_indev_data_t*data);

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

    be_find_class(vm, "lv.lv_indev");
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

  void lvbe_encoder_with_keys_read(lv_indev_drv_t * drv, lv_indev_data_t *data){
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
    data->continue_reading = false;
    for (/* continue where we left */; i < 3; i++) {
      if (lvbe.btn[i].state_changed()) {
        data->continue_reading = true;
      }
    }
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
   * Generalized tostring method, shows class and _p value
  \*********************************************************************************************/
  int lvx_tostring(bvm *vm) {
    lv_obj_t * obj = (lv_obj_t*) lv_get_arg(vm, 1);
    const char * classname = be_classname(vm, 1);
    char s[48];
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
      if (style == nullptr) {
          be_throw(vm, BE_MALLOC_FAIL);
      }
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
    snprintf(fname, sizeof(fname), "/screenshot-%d.bmp", Rtc.utc_time);
    File f = dfsp->open(fname, "w");
    if (f) {
      glue->setScreenshotFile(&f);

      uint32_t bmp_width = lv_disp_get_hor_res(nullptr);
      uint32_t bmp_height = lv_disp_get_ver_res(nullptr);

      // write BMP header
      static const uint8_t bmp_sign[] = { 0x42, 0x4d };   // BM = Windows
      f.write(bmp_sign, sizeof(bmp_sign));
      size_t bmp_size = bmp_width * bmp_height * LV_COLOR_DEPTH / 8 + 0x44;
      f.write((uint8_t*)&bmp_size, sizeof(bmp_size));
      uint32_t zero = 0;
      f.write((uint8_t*) &zero, sizeof(zero));  // reserved 4-bytes
      uint32_t bmp_offset_to_pixels = 0x44;  // TODO
      f.write((uint8_t*) &bmp_offset_to_pixels, sizeof(bmp_offset_to_pixels));

      // DIB Header BITMAPINFOHEADER
      size_t bmp_dib_header_size = 52;                    // BITMAPV2INFOHEADER
      f.write((uint8_t*) &bmp_dib_header_size, sizeof(bmp_dib_header_size));

      f.write((uint8_t*) &bmp_width, sizeof(bmp_width));
      f.write((uint8_t*) &bmp_height, sizeof(bmp_height));

      // rest of header
      // BITMAPV2INFOHEADER = 52 bytes header, 40 bytes sub-header
      static const uint8_t bmp_dib_header[] = {
        0x01, 0x00,                       // planes
          16, 0x00,                       // bits per pixel = 16
        0x03, 0x00, 0x00, 0x00,           // compression = BI_BITFIELDS uncrompressed
        0x00, 0x00, 0x00, 0x00,           // Image size, 0 is valid for BI_RGB (uncompressed) TODO
        0x00, 0x00, 0x00, 0x00,           // X pixels per meter
        0x00, 0x00, 0x00, 0x00,           // Y pixels per meter
        0x00, 0x00, 0x00, 0x00,           // Colors in table
        0x00, 0x00, 0x00, 0x00,           // Important color count

        // RGB masks
        0x00, 0xF8, 0x00, 0x00,           // Red channel mask
        0xE0, 0x07, 0x00, 0x00,           // Green channel mask
        0x1F, 0x00, 0x00, 0x00,           // Blue channel mask

        0x00, 0x00,                       // Padding to align on 4 bytes boundary
      };
      f.write(bmp_dib_header, sizeof(bmp_dib_header));
      // now we can write the pixels array

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
