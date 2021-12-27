#include "be_constobj.h"

static be_define_const_map_slots(m_liblv_tasmota_map) {
    { be_const_key(seg7_font, -1), be_const_func(lv0_load_seg7_font) },
    { be_const_key(font_seg7, -1), be_const_func(lv0_load_seg7_font) },
    { be_const_key(start, -1), be_const_func(lv0_start) },
    { be_const_key(font_montserrat, -1), be_const_func(lv0_load_montserrat_font) },
    { be_const_key(montserrat_font, -1), be_const_func(lv0_load_montserrat_font) },
    { be_const_key(load_freetype_font, -1), be_const_func(lv0_load_freetype_font) },
    { be_const_key(init, 0), be_const_closure(lv_tasmota_init_closure) },
    { be_const_key(screenshot, 1), be_const_func(lv0_screenshot) },
    { be_const_key(register_button_encoder, -1), be_const_func(lv0_register_button_encoder) },
};

static be_define_const_map(
    m_liblv_tasmota_map,
    9
);

static be_define_const_module(
    m_liblv_tasmota,
    "lv_tasmota"
);

BE_EXPORT_VARIABLE be_define_const_native_module(lv_tasmota);
