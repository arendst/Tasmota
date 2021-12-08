/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "berry.h"

/* this file contains the declaration of the module table. */

/* default modules declare */
be_extern_native_module(string);
be_extern_native_module(json);
be_extern_native_module(math);
be_extern_native_module(time);
be_extern_native_module(os);
be_extern_native_module(global);
be_extern_native_module(sys);
be_extern_native_module(debug);
be_extern_native_module(gc);
be_extern_native_module(solidify);
be_extern_native_module(introspect);
be_extern_native_module(strict);

/* Tasmota specific */
be_extern_native_module(python_compat);
be_extern_native_module(re);
be_extern_native_module(persist);
be_extern_native_module(autoconf);
be_extern_native_module(tapp);
be_extern_native_module(light);
be_extern_native_module(gpio);
be_extern_native_module(display);
be_extern_native_module(energy);
be_extern_native_module(webserver);
be_extern_native_module(flash);
be_extern_native_module(path);
be_extern_native_module(unishox);
be_extern_native_module(animate);
#ifdef USE_LVGL
be_extern_native_module(lv);
#endif // USE_LVGL

/* user-defined modules declare start */

/* user-defined modules declare end */

/* module list declaration */
BERRY_LOCAL const bntvmodule* const be_module_table[] = {
/* default modules register */
#if BE_USE_STRING_MODULE
    &be_native_module(string),
#endif
#if BE_USE_JSON_MODULE
    &be_native_module(json),
#endif
#if BE_USE_MATH_MODULE
    &be_native_module(math),
#endif
#if BE_USE_TIME_MODULE
    &be_native_module(time),
#endif
#if BE_USE_OS_MODULE
    &be_native_module(os),
#endif
#if BE_USE_GLOBAL_MODULE
    &be_native_module(global),
#endif
#if BE_USE_SYS_MODULE
    &be_native_module(sys),
#endif
#if BE_USE_DEBUG_MODULE
    &be_native_module(debug),
#endif
#if BE_USE_GC_MODULE
    &be_native_module(gc),
#endif
#if BE_USE_SOLIDIFY_MODULE
    &be_native_module(solidify),
#endif
#if BE_USE_INTROSPECT_MODULE
    &be_native_module(introspect),
#endif
#if BE_USE_STRICT_MODULE
    &be_native_module(strict),
#endif
    /* user-defined modules register start */
    
    &be_native_module(python_compat),
    &be_native_module(re),
    &be_native_module(path),
    &be_native_module(persist),
#ifdef USE_AUTOCONF
    &be_native_module(autoconf),
#endif // USE_AUTOCONF
    &be_native_module(tapp),
    &be_native_module(gpio),
#ifdef USE_DISPLAY
    &be_native_module(display),
#endif // USE_DISPLAY
#ifdef USE_LIGHT
    &be_native_module(light),
#endif

#ifdef USE_UNISHOX_COMPRESSION
    &be_native_module(unishox),
#endif // USE_UNISHOX_COMPRESSION
    &be_native_module(animate),

#ifdef USE_LVGL
    &be_native_module(lv),
#endif // USE_LVGL
#ifdef USE_ENERGY_SENSOR
    &be_native_module(energy),
#endif // USE_ENERGY_SENSOR
#ifdef USE_WEBSERVER
    &be_native_module(webserver),
#endif // USE_WEBSERVER
    &be_native_module(flash),


    /* user-defined modules register end */
    NULL /* do not remove */
};

#ifdef ESP32
extern void be_load_tasmota_ntvlib(bvm *vm);
extern void be_load_wirelib(bvm *vm);
extern void be_load_onewirelib(bvm *vm);
extern void be_load_serial_lib(bvm *vm);
extern void be_load_Driver_class(bvm *vm);
extern void be_load_Timer_class(bvm *vm);
extern void be_load_driver_i2c_lib(bvm *vm);
extern void be_load_AXP192_class(bvm *vm);
extern void be_load_md5_lib(bvm *vm);
extern void be_load_webclient_lib(bvm *vm);
extern void be_load_crypto_lib(bvm *vm);
extern void be_load_Leds_ntv_class(bvm *vm);
extern void be_load_Leds_class(bvm *vm);
extern void be_load_Leds_animator_class(bvm *vm);

extern void be_load_ctypes_lib(bvm *vm);
extern void be_load_ctypes_energy_definitions_lib(bvm *vm);

#ifdef USE_I2S_AUDIO_BERRY
extern void be_load_driver_audio_lib(bvm *vm);
#endif

#ifdef USE_LVGL
extern void be_load_lv_color_class(bvm *vm);
extern void be_load_lv_font_class(bvm *vm);
extern void be_load_LVGL_glob_class(bvm *vm);
// custom widgets
extern void be_load_lv_signal_bars_class(bvm *vm);
extern void be_load_lv_wifi_bars_class(bvm *vm);
extern void be_load_lv_wifi_bars_icon_class(bvm *vm);
extern void be_load_lv_signal_arcs_class(bvm *vm);
extern void be_load_lv_wifi_arcs_class(bvm *vm);
extern void be_load_lv_wifi_arcs_icon_class(bvm *vm);
extern void be_load_lv_clock_icon_class(bvm *vm);
#endif// USE_LVGL

/* this code loads the native class definitions */
BERRY_API void be_load_custom_libs(bvm *vm)
{
    (void)vm;   /* prevent a compiler warning */
    
    /* add here custom libs */
#if !BE_USE_PRECOMPILED_OBJECT
    /* be_load_xxxlib(vm); */
#endif
    be_load_Timer_class(vm);
    be_load_tasmota_ntvlib(vm);
    be_load_Driver_class(vm);
    be_load_md5_lib(vm);
    be_load_serial_lib(vm);
    be_load_ctypes_lib(vm);
#ifdef USE_ALEXA_AVS
    be_load_crypto_lib(vm);
#endif
#ifdef USE_I2C
    be_load_wirelib(vm);
    be_load_driver_i2c_lib(vm);
    be_load_AXP192_class(vm);
#endif // USE_I2C
#ifdef USE_ENERGY_SENSOR
    be_load_ctypes_energy_definitions_lib(vm);
#endif // USE_ENERGY_SENSOR
#ifdef USE_WEBCLIENT
    be_load_webclient_lib(vm);
#endif // USE_WEBCLIENT
#if defined(USE_ONEWIRE) || defined(USE_DS18x20)
    be_load_onewirelib(vm);
#endif
#ifdef USE_WS2812
    be_load_Leds_ntv_class(vm);
    be_load_Leds_class(vm);
    be_load_Leds_animator_class(vm);
#endif // USE_WS2812
#ifdef USE_I2S_AUDIO_BERRY
    be_load_driver_audio_lib(vm);
#endif
#ifdef USE_LVGL
    // LVGL
    be_load_lv_color_class(vm);
    be_load_lv_font_class(vm);

    be_load_LVGL_glob_class(vm);
    // custom widgets
    be_load_lv_signal_bars_class(vm);
    be_load_lv_wifi_bars_class(vm);
    be_load_lv_wifi_bars_icon_class(vm);
    be_load_lv_signal_arcs_class(vm);
    be_load_lv_wifi_arcs_class(vm);
    be_load_lv_wifi_arcs_icon_class(vm);
    be_load_lv_clock_icon_class(vm);
#endif // USE_LVGL
}
#endif
