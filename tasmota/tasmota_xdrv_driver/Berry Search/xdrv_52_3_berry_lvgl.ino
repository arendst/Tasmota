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
#include "be_mapping.h"
#include "be_ctypes.h"
#include "lv_berry.h"
#include "Adafruit_LvGL_Glue.h"

// Berry easy logging
extern "C" {
  extern void berry_log_C(const char * berry_buf, ...);
  extern const be_ntv_class_def_t lv_classes[];
  extern const size_t lv_classes_size;
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

extern "C" {

  
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


  /*********************************************************************************************\
   * Support for Freetype fonts
  \*********************************************************************************************/
  // load freetype font by name in file-system
  int lv0_load_freetype_font(bvm *vm) {
#ifdef USE_LVGL_FREETYPE
    int argc = be_top(vm);
    if (argc == 3 && be_isstring(vm, 1) && be_isint(vm, 2) && be_isint(vm, 3)) {
      lv_ft_info_t info = {};
      info.name = be_tostring(vm, 1);
      info.weight = be_toint(vm, 2);
      info.style = be_toint(vm, 3);
      lv_ft_font_init(&info);
      lv_font_t * font = info.font;

      if (font != nullptr) {
        be_find_global_or_module_member(vm, "lv.lv_font");
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

  // unscii fonts
  const lv_font_table_t lv_unscii_fonts[] = {
  #if LV_FONT_UNSCII_8
    {  8, &lv_font_unscii_8 },
  #endif
  #if LV_FONT_UNSCII_16
    { 16, &lv_font_unscii_16 },
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
    { 0, nullptr}
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
    { 0, nullptr}
  };

  // register all included fonts
  const lv_font_names_t lv_embedded_fonts[] = {
    { "montserrat", lv_montserrat_fonts },
    { "seg7", lv_seg7_fonts },
    { "unscii", lv_unscii_fonts},
#ifdef USE_LVGL_HASPMOTA
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
    
    be_find_global_or_module_member(vm, "lv.lv_font");
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
#ifdef USE_LVGL_HASPMOTA
    return lv_load_embedded_font(vm, "robotocondensed", 0);
#endif // USE_LVGL_HASPMOTA
    be_raise(vm, kTypeError, nullptr);
  }

  int lv0_load_font_embedded(bvm *vm) {
    if ((be_top(vm) >= 2) && (be_isstring(vm, 1)) && (be_isint(vm, 2))) {
      return lv_load_embedded_font(vm, be_tostring(vm, 1), be_toint(vm, 2));
    }
    be_return_nil(vm);
  }

  /*********************************************************************************************\
   * Tasmota Logo
  \*********************************************************************************************/
  #include "lvgl_berry/tasmota_logo_64_truecolor_alpha.h"

  void lv_img_set_tasmota_logo(lv_obj_t * img) {
    lv_img_set_src(img, &tasmota_logo_64_truecolor);
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

      // call lv.splash_remove() to kill any current splash screen
      if (be_getglobal(vm, "lv")) {
        if (be_getmember(vm, -1, "splash_remove")) {
          // call it
          be_call(vm, 0);
        }
        be_pop(vm, 1);
      }
      be_pop(vm, 1);

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
    // berry_log_C("lv0_register_button_encoder argc=%d inverted=%d", argc, be_tobool(vm, 1));
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
    berry_log_C(D_LOG_LVGL "Button Rotary encoder using GPIOs %d,%d,%d%s", btn0, btn1, btn2, inverted ? " (inverted)" : "");

    lv_indev_drv_init(&lvbe.indev_drv);
    lvbe.indev_drv.type = LV_INDEV_TYPE_ENCODER;
    lvbe.indev_drv.read_cb = lvbe_encoder_with_keys_read;

    lv_indev_t * indev = lv_indev_drv_register(&lvbe.indev_drv);
    lvbe.indev_list.addHead(indev);   // keep track of indevs

    be_find_global_or_module_member(vm, "lv.lv_indev");
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
        // berry_log_C("Button event key %d state %d,%d", data->key, state, data->state);
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
      size_t bmp_size = bmp_width * bmp_height * LV_COLOR_DEPTH / 8 + 0x42;
      f.write((uint8_t*)&bmp_size, sizeof(bmp_size));
      uint32_t zero = 0;
      f.write((uint8_t*) &zero, sizeof(zero));  // reserved 4-bytes
      uint32_t bmp_offset_to_pixels = 0x42;  // TODO
      f.write((uint8_t*) &bmp_offset_to_pixels, sizeof(bmp_offset_to_pixels));

      size_t bmp_dib_header_size = 0x28;
      f.write((uint8_t*) &bmp_dib_header_size, sizeof(bmp_dib_header_size));

      f.write((uint8_t*) &bmp_width, sizeof(bmp_width));
      f.write((uint8_t*) &bmp_height, sizeof(bmp_height));

      // rest of header
      // BITMAPV2INFOHEADER = 52 bytes header, 40 bytes sub-header
      static const uint8_t bmp_dib_header1[] = {
        0x01, 0x00,                       // planes
          16, 0x00,                       // bits per pixel = 16
        0x03, 0x00, 0x00, 0x00,           // compression = BI_BITFIELDS uncrompressed
      };

      static const uint8_t bmp_dib_header2[] = {
        0xC4, 0xE0, 0x00, 0x00,           // X pixels per meter
        0xC4, 0xE0, 0x00, 0x00,           // Y pixels per meter
        0x00, 0x00, 0x00, 0x00,           // Colors in table
        0x00, 0x00, 0x00, 0x00,           // Important color count

        // RGB masks
        0x00, 0xF8, 0x00, 0x00,           // Red channel mask
        0xE0, 0x07, 0x00, 0x00,           // Green channel mask
        0x1F, 0x00, 0x00, 0x00,           // Blue channel mask
      };
      f.write(bmp_dib_header1, sizeof(bmp_dib_header1));
      f.write((uint8_t*)&bmp_size, sizeof(bmp_size));
      f.write(bmp_dib_header2, sizeof(bmp_dib_header2));
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

#else // USE_LVGL

//   // define weak aliases
//   int32_t b_nrg_read(struct bvm *vm) __attribute__ ((weak, alias ("b_wire_energymissing")));



#endif // USE_LVGL

#endif  // USE_BERRY
