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
#include "Adafruit_LvGL_Glue.h"

extern Adafruit_LvGL_Glue * glue;

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
    lv_color = lv_color_from_uint32(be_toint(vm, -1));
    uint32_t color = lv_color_to32(lv_color);
    be_pop(vm, 1);  // remove attribute
    char s[32];
    snprintf(s, sizeof(s), "lv_color(0x%x)", color);
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

  #include "lvgl_berry/tasmota_logo_64_truecolor_alpha.h"

  void lv_img_set_tasmota_logo(lv_obj_t * img) {
    lv_img_set_src(img, &tasmota_logo_64_truecolor);
  }

  // lv.start(instance, instance) -> nil
  int lv0_start(bvm *vm);
  int lv0_start(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 1 && be_isstring(vm, 1)) {
      const char * uconfig = be_tostring(vm, 1);
      start_lvgl(uconfig);
      be_return_nil(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // lv.demo() -> nil
  int lv0_demo(bvm *vm);
  int lv0_demo(bvm *vm) {
    lv_ex_get_started_1();
    be_return_nil(vm);
  }

  // lv.scr_act() -> lv_obj() instance
  int lv0_scr_act(bvm *vm)    { return lv0_lvobj__void_call(vm, &lv_scr_act); }
  int lv0_layer_top(bvm *vm)  { return lv0_lvobj__void_call(vm, &lv_layer_top); }
  int lv0_layer_sys(bvm *vm)  { return lv0_lvobj__void_call(vm, &lv_layer_sys); }

  int lv0_get_hor_res(bvm *vm) {
    be_pushint(vm, lv_disp_get_hor_res(lv_disp_get_default()));
    be_return(vm);
  }
  int lv0_get_ver_res(bvm *vm) {
    be_pushint(vm, lv_disp_get_ver_res(lv_disp_get_default()));
    be_return(vm);
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
    // AddLog(LOG_LEVEL_INFO, "argc %d lv_obj %p", argc, obj);
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
