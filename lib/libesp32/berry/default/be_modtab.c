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
be_extern_native_module(undefined);

/* Berry extensions */
be_extern_native_module(cb);

/* Tasmota specific */
be_extern_native_module(python_compat);
be_extern_native_module(re);
be_extern_native_module(mqtt);
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
be_extern_native_module(hue_ntv);
be_extern_native_module(hue_bridge);
be_extern_native_module(uuid);
be_extern_native_module(animate);
be_extern_native_module(partition_core);
be_extern_native_module(crc);
be_extern_native_module(crypto);
be_extern_native_module(ULP);
be_extern_native_module(TFL);
be_extern_native_module(mdns);
#ifdef USE_ZIGBEE
be_extern_native_module(zigbee);
#endif // USE_ZIGBEE
// BLE
be_extern_native_module(MI32);
be_extern_native_module(BLE);
#ifdef USE_LVGL
be_extern_native_module(lv);
be_extern_native_module(lv_extra);
be_extern_native_module(lv_tasmota);
#ifdef USE_LVGL_HASPMOTA
be_extern_native_module(haspmota);
#endif // USE_LVGL_HASPMOTA
#endif // USE_LVGL
#ifdef USE_MATTER_DEVICE
be_extern_native_module(matter);
#endif // USE_MATTER_DEVICE

/* user-defined modules declare start */

/* user-defined modules declare end */

/* module list declaration */
BERRY_LOCAL const bntvmodule_t* const be_module_table[] = {
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
    &be_native_module(undefined),

    &be_native_module(re),
#ifdef TASMOTA
    /* Berry extensions */
    &be_native_module(cb),

    /* user-defined modules register start */
    
    &be_native_module(python_compat),
    &be_native_module(path),
    &be_native_module(mqtt),
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
#if defined(USE_EMULATION) && defined(USE_EMULATION_HUE)
    &be_native_module(hue_ntv),
    &be_native_module(hue_bridge),
#endif

    &be_native_module(uuid),
#ifdef USE_UNISHOX_COMPRESSION
    &be_native_module(unishox),
#endif // USE_UNISHOX_COMPRESSION
    &be_native_module(animate),

#ifdef USE_LVGL
    &be_native_module(lv),
    &be_native_module(lv_extra),
    &be_native_module(lv_tasmota),
#ifdef USE_LVGL_HASPMOTA
    &be_native_module(haspmota),
#endif // USE_LVGL_HASPMOTA
#endif // USE_LVGL
#ifdef USE_ENERGY_SENSOR
    &be_native_module(energy),
#endif // USE_ENERGY_SENSOR
#ifdef USE_WEBSERVER
    &be_native_module(webserver),
#endif // USE_WEBSERVER
#ifdef USE_ZIGBEE
    &be_native_module(zigbee),
#endif // USE_ZIGBEE
    &be_native_module(flash),
    &be_native_module(partition_core),
    &be_native_module(crc),
    &be_native_module(crypto),
#if defined(USE_BERRY_ULP) && ((CONFIG_IDF_TARGET_ESP32) || defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32S3))
    &be_native_module(ULP),
#endif // USE_BERRY_ULP
#if defined(USE_BERRY_TF_LITE)
    &be_native_module(TFL),
#endif //USE_BERRY_TF_LITE
#if defined(USE_MI_ESP32) && !defined(USE_BLE_ESP32)
    &be_native_module(MI32),
    &be_native_module(BLE),
#endif //USE_MI_ESP32
#ifdef USE_DISCOVERY
    &be_native_module(mdns),
#endif // USE_DISCOVERY
#ifdef USE_MATTER_DEVICE
    &be_native_module(matter),
#endif // USE_MATTER_DEVICE
#endif // TASMOTA
    /* user-defined modules register end */
    NULL /* do not remove */
};

be_extern_native_class(dyn);
be_extern_native_class(tasmota);
be_extern_native_class(Trigger);
be_extern_native_class(Driver);
be_extern_native_class(ctypes_bytes);
be_extern_native_class(ctypes_bytes_dyn);
be_extern_native_class(serial);
be_extern_native_class(ccronexpr);
be_extern_native_class(tasmota_log_reader);
be_extern_native_class(light_state);
be_extern_native_class(Wire);
be_extern_native_class(I2C_Driver);
be_extern_native_class(AXP192);
be_extern_native_class(AXP202);
be_extern_native_class(OneWire);
be_extern_native_class(Leds_ntv);
be_extern_native_class(Leds);
be_extern_native_class(Leds_animator);
be_extern_native_class(AudioGenerator);
be_extern_native_class(AudioFileSource);
be_extern_native_class(AudioOutputI2S);
be_extern_native_class(AudioGeneratorWAV);
be_extern_native_class(AudioGeneratorMP3);
be_extern_native_class(AudioFileSourceFS);
be_extern_native_class(AudioOpusDecoder);
be_extern_native_class(AudioInputI2S);
be_extern_native_class(md5);
be_extern_native_class(udp);
be_extern_native_class(webclient);
be_extern_native_class(tcpclient);
be_extern_native_class(tcpclientasync);
be_extern_native_class(tcpserver);
be_extern_native_class(energy_struct);
// LVGL core classes
be_extern_native_class(lv_color);
be_extern_native_class(lv_font);
be_extern_native_class(LVGL_glob);
// LVGL custom classes
be_extern_native_class(lv_signal_bars);
be_extern_native_class(lv_wifi_bars);
be_extern_native_class(lv_wifi_bars_icon);
be_extern_native_class(lv_signal_arcs);
be_extern_native_class(lv_wifi_arcs);
be_extern_native_class(lv_wifi_arcs_icon);
be_extern_native_class(lv_clock);
be_extern_native_class(lv_clock_icon);

be_extern_native_class(int64);

BERRY_LOCAL bclass_array be_class_table = {
#ifdef TASMOTA
    /* first list are direct classes */
    &be_native_class(dyn),
    &be_native_class(tasmota),
    &be_native_class(Trigger),
    &be_native_class(Driver),
    &be_native_class(serial),
    &be_native_class(ccronexpr),
    &be_native_class(ctypes_bytes),
    &be_native_class(ctypes_bytes_dyn),
    &be_native_class(tasmota_log_reader),
#ifdef USE_LIGHT
    &be_native_class(light_state),
#endif
#if defined(USE_ONEWIRE) || defined(USE_DS18x20)
    &be_native_class(OneWire),
#endif
#ifdef USE_I2C
    &be_native_class(Wire),
    &be_native_class(I2C_Driver),
    &be_native_class(AXP192),
    &be_native_class(AXP202),
#endif // USE_I2C
    &be_native_class(md5),
#ifdef USE_WEBCLIENT
    &be_native_class(udp),
    &be_native_class(webclient),
    &be_native_class(tcpclient),
    &be_native_class(tcpclientasync),
#endif // USE_WEBCLIENT
#ifdef USE_BERRY_TCPSERVER
    &be_native_class(tcpserver),
#endif // USE_BERRY_TCPSERVER
#ifdef USE_WS2812
    &be_native_class(Leds_ntv),
    &be_native_class(Leds),
    &be_native_class(Leds_animator),
#endif // USE_WS2812
#ifdef USE_ENERGY_SENSOR
    &be_native_class(energy_struct),
#endif // USE_ENERGY_SENSOR

#ifdef USE_LVGL
    &be_native_class(LVGL_glob),
    
    &be_native_class(lv_signal_bars),
    &be_native_class(lv_wifi_bars),
    &be_native_class(lv_wifi_bars_icon),
    &be_native_class(lv_signal_arcs),
    &be_native_class(lv_wifi_arcs),
    &be_native_class(lv_wifi_arcs_icon),
    &be_native_class(lv_clock),
    &be_native_class(lv_clock_icon),
#endif // USE_LVGL

#if defined(USE_I2S_AUDIO_BERRY) && (ESP_IDF_VERSION_MAJOR >= 5)
    &be_native_class(AudioGenerator),
    &be_native_class(AudioFileSource),
    &be_native_class(AudioOutputI2S),
    &be_native_class(AudioGeneratorWAV),
    &be_native_class(AudioGeneratorMP3),
#ifdef USE_UFILESYS
    &be_native_class(AudioFileSourceFS),
#endif // USE_UFILESYS
    &be_native_class(AudioOpusDecoder),
    &be_native_class(AudioInputI2S),
#endif // defined(USE_I2S_AUDIO_BERRY) && (ESP_IDF_VERSION_MAJOR >= 5)
#if defined(USE_BERRY_INT64) || defined(USE_MATTER_DEVICE)
    &be_native_class(int64),
#endif
#endif // TASMOTA
    NULL, /* do not remove */
};

extern void be_load_ctypes_energy_definitions_lib(bvm *vm);

/* this code loads the native class definitions */
BERRY_API void be_load_custom_libs(bvm *vm)
{
    (void)vm;   /* prevent a compiler warning */
    
    /* add here custom libs */
#if !BE_USE_PRECOMPILED_OBJECT
    /* be_load_xxxlib(vm); */
#endif
}
