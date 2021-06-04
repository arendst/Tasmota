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

/* Tasmota specific */
be_extern_native_module(light);
be_extern_native_module(gpio);
be_extern_native_module(energy);
be_extern_native_module(webserver);
be_extern_native_module(flash);
#ifdef USE_LVGL
be_extern_native_module(lvgl);
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
    /* user-defined modules register start */
    
    &be_native_module(gpio),
#ifdef USE_LIGHT
    &be_native_module(light),
#endif

#ifdef USE_LVGL
    &be_native_module(lvgl),
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
extern void be_load_driverlib(bvm *vm);
extern void be_load_driver_i2c_lib(bvm *vm);
extern void be_load_md5_lib(bvm *vm);

#ifdef USE_LVGL
extern void be_load_lvgl_color_lib(bvm *vm);
extern void be_load_lvgl_font_lib(bvm *vm);
extern void be_load_lv_all_lib(bvm *vm);
#endif// USE_LVGL

/* this code loads the native class definitions */
BERRY_API void be_load_custom_libs(bvm *vm)
{
    (void)vm;   /* prevent a compiler warning */
    
    /* add here custom libs */
#if !BE_USE_PRECOMPILED_OBJECT
    /* be_load_xxxlib(vm); */
#endif
    be_load_tasmota_ntvlib(vm);
    be_load_driverlib(vm);
    be_load_md5_lib(vm);
#ifdef USE_I2C
    be_load_wirelib(vm);
    be_load_driver_i2c_lib(vm);
#endif // USE_I2C
#ifdef USE_LVGL
    // LVGL
    be_load_lvgl_color_lib(vm);
    be_load_lvgl_font_lib(vm);

    be_load_lv_all_lib(vm);
#endif // USE_LVGL
}
#endif
