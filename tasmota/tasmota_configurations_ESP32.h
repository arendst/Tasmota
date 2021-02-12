/*
  tasmota_configurations_ESP32.h - ESP32 only Configurations for Tasmota

  Copyright (C) 2021  Theo Arends

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

#ifndef _TASMOTA_CONFIGURATIONS_ESP32_H_
#define _TASMOTA_CONFIGURATIONS_ESP32_H_

#ifdef ESP32

/*********************************************************************************************\
 * [tasmota32-webcam.bin]
 * Provide an image with useful supported sensors enabled
\*********************************************************************************************/

#ifdef FIRMWARE_WEBCAM

#undef CODE_IMAGE_STR
#define CODE_IMAGE_STR "webcam"

#define USE_WEBCAM
#undef  USE_MI_ESP32                             // (ESP32 only) Disable support for ESP32 as a BLE-bridge (+9k2 mem, +292k flash)
#endif  // FIRMWARE_WEBCAM

/*********************************************************************************************\
 * [tasmota32-odroidgo.bin]
 * Provide an image with useful supported sensors enabled for Odroid Go
\*********************************************************************************************/

#ifdef FIRMWARE_ODROID_GO

#undef CODE_IMAGE_STR
#define CODE_IMAGE_STR "odroid-go"

#undef MODULE
#define MODULE                 ODROID_GO         // [Module] Select default module from tasmota_template.h
#undef FALLBACK_MODULE
#define FALLBACK_MODULE        ODROID_GO         // [Module2] Select default module on fast reboot where USER_MODULE is user template

#define USE_ODROID_GO                            // Add support for Odroid Go
#define USE_UFILESYS
#define USE_SDCARD
  #define GUI_TRASH_FILE
#define USE_ADC
#define USE_SPI
  #define USE_DISPLAY                            // Add SPI Display Support (+2k code)
    #define USE_DISPLAY_ILI9341                  // [DisplayModel 4] Enable ILI9341 Tft 480x320 display (+19k code)
//#define USE_BLE_ESP32                            // Enable new BLE driver
//#define USE_MI_ESP32                             // (ESP32 only) Add support for ESP32 as a BLE-bridge (+9k2 mem, +292k flash)
#endif  // FIRMWARE_ODROID_GO

/*********************************************************************************************\
 * [tasmota32-core2.bin]
 * Provide an image with useful supported sensors enabled for M5stack core2
\*********************************************************************************************/

#ifdef FIRMWARE_M5STACK_CORE2

#undef CODE_IMAGE_STR
#define CODE_IMAGE_STR "core2"

#undef MODULE
#define MODULE                 M5STACK_CORE2     // [Module] Select default module from tasmota_template.h
#undef FALLBACK_MODULE
#define FALLBACK_MODULE        M5STACK_CORE2     // [Module2] Select default module on fast reboot where USER_MODULE is user template

#define USE_M5STACK_CORE2                        // Add support for M5Stack Core2
  #define USE_I2S_SAY_TIME
  #define USE_I2S_WEBRADIO
  #define USE_MPU6886
#define USE_UFILESYS
#define USE_SDCARD
  #define GUI_TRASH_FILE
#define USE_I2C
  #define USE_BMA423
#define USE_SPI
  #define USE_DISPLAY
    #define USE_DISPLAY_ILI9342
    #define JPEG_PICTS
    #define USE_FT5206
    #define USE_TOUCH_BUTTONS
      #define MAX_TOUCH_BUTTONS 16
#define USE_SENDMAIL
#define USE_ESP32MAIL

//#define USE_SCRIPT                               // Add support for script (+17k code)
// Script related defines
#ifdef USE_SCRIPT
  #undef USE_RULES
  #define MAXVARS 75
  #define MAXSVARS 15
  #define MAXFILT 10
  #define UFSYS_SIZE 8192
  #define USE_SCRIPT_TASK
  #define LARGE_ARRAYS
  #define SCRIPT_LARGE_VNBUFF
  #define USE_SCRIPT_GLOBVARS
  #define USE_SCRIPT_SUB_COMMAND
  #define USE_ANGLE_FUNC
  #define SCRIPT_FULL_WEBPAGE
  #define SCRIPT_GET_HTTPS_JP
  #define USE_GOOGLE_CHARTS
#endif  // USE_SCRIPT
#endif  // FIRMWARE_M5STACK_CORE2

/*********************************************************************************************\
 * [tasmota32-bluetooth.bin]
 * Provide an image with BLE support
\*********************************************************************************************/

#ifdef FIRMWARE_BLUETOOTH

#undef CODE_IMAGE_STR
#define CODE_IMAGE_STR "bluetooth"

#undef MODULE
#define MODULE                 WEMOS             // [Module] Select default module from tasmota_template.h
#undef FALLBACK_MODULE
#define FALLBACK_MODULE        WEMOS             // [Module2] Select default module on fast reboot where USER_MODULE is user template

#define USE_UFILESYS
#define USE_SDCARD
  #define GUI_TRASH_FILE
#define USE_ADC
#define USE_BLE_ESP32                            // Enable new BLE driver
#define USE_MI_ESP32                             // (ESP32 only) Add support for ESP32 as a BLE-bridge (+9k2 mem, +292k flash)
#endif  // FIRMWARE_BLUETOOTH

#endif  // ESP32

#endif  // _TASMOTA_CONFIGURATIONS_ESP32_H_
