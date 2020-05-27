/*
  tasmota_globals.h - Function prototypes and global configurations for Tasmota

  Copyright (C) 2020  Theo Arends

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

#ifndef _TASMOTA_GLOBALS_H_
#define _TASMOTA_GLOBALS_H_

/*********************************************************************************************\
 * Function prototypes
\*********************************************************************************************/

// Needed for core 2.3.0 compilation (#6721)
#ifdef __cplusplus
extern "C" {
#endif
#include "user_interface.h"
#ifdef __cplusplus
}
#endif

//#ifdef USE_KNX  // Enabling this will fail compilation. It has no impact if not used. (20180417)
#include <esp-knx-ip.h>
void KNX_CB_Action(message_t const &msg, void *arg);
//#endif  // USE_KNX

void DomoticzTempHumPressureSensor(float temp, float hum, float baro = -1);
char* ToHex_P(const unsigned char * in, size_t insz, char * out, size_t outsz, char inbetween = '\0');
extern "C" void custom_crash_callback(struct rst_info * rst_info, uint32_t stack, uint32_t stack_end);
extern "C" void resetPins();

/*********************************************************************************************\
 * Preconfigured configurations
\*********************************************************************************************/

#include "tasmota_configurations.h"            // Preconfigured configurations

/*********************************************************************************************\
 * Mandatory defines satisfying disabled defines
\*********************************************************************************************/

#ifdef USE_EMULATION_HUE
#define USE_EMULATION
#endif
#ifdef USE_EMULATION_WEMO
#define USE_EMULATION
#endif
                                               // See https://github.com/esp8266/Arduino/pull/4889
#undef NO_EXTRA_4K_HEAP                        // Allocate 4k heap for WPS in ESP8166/Arduino core v2.4.2 (was always allocated in previous versions)

#ifndef USE_SONOFF_RF
#undef USE_RF_FLASH                            // Disable RF firmware flash when Sonoff Rf is disabled
#endif

#ifndef SWITCH_MODE
#define SWITCH_MODE                 TOGGLE     // TOGGLE, FOLLOW or FOLLOW_INV (the wall switch state)
#endif

#ifndef MQTT_FINGERPRINT1
// Set an all-zeros default fingerprint to activate auto-learning on first connection (AWS IoT)
#define MQTT_FINGERPRINT1           "00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00"
#endif
#ifndef MQTT_FINGERPRINT2
#define MQTT_FINGERPRINT2           "A5 02 FF 13 99 9F 8B 39 8E F1 83 4F 11 23 65 0B 32 36 FC 07"
#endif

#ifndef WS2812_LEDS
#define WS2812_LEDS                 30         // [Pixels] Number of LEDs
#endif

#ifdef USE_MQTT_TLS
  const uint16_t WEB_LOG_SIZE = 2000;          // Max number of characters in weblog
#else
  const uint16_t WEB_LOG_SIZE = 4000;          // Max number of characters in weblog
#endif

#if defined(USE_MQTT_TLS) && defined(ARDUINO_ESP8266_RELEASE_2_3_0)
  #error "TLS is no more supported on Core 2.3.0, use 2.4.2 or higher."
#endif

#ifndef MQTT_MAX_PACKET_SIZE
#define MQTT_MAX_PACKET_SIZE        1200       // Bytes
#endif
#ifndef MQTT_KEEPALIVE
#define MQTT_KEEPALIVE              30         // Seconds
#endif
#ifndef MQTT_TIMEOUT
#define MQTT_TIMEOUT                10000      // milli seconds
#endif
#ifndef MQTT_CLEAN_SESSION
#define MQTT_CLEAN_SESSION          1          // 0 = No clean session, 1 = Clean session (default)
#endif

#ifndef MESSZ
//#define MESSZ                       1040     // Max number of characters in JSON message string (Hass discovery and nice MQTT_MAX_PACKET_SIZE = 1200)
#define MESSZ                       (MQTT_MAX_PACKET_SIZE -TOPSZ -7)  // Max number of characters in JSON message string
#endif

#ifndef USE_DEVICE_GROUPS
#undef USE_PWM_DIMMER_REMOTE
#undef USE_DGR_LIGHT_SEQUENCE
#endif  // USE_DEVICE_GROUPS

#ifndef DOMOTICZ_UPDATE_TIMER
#define DOMOTICZ_UPDATE_TIMER       0          // [DomoticzUpdateTimer] Send relay status (0 = disable, 1 - 3600 seconds) (Optional)
#endif

#ifndef EMULATION
#define EMULATION                   EMUL_NONE  // [Emulation] Select Belkin WeMo (single relay/light) or Hue Bridge emulation (multi relay/light) (EMUL_NONE, EMUL_WEMO or EMUL_HUE)
#endif

#ifndef MTX_ADDRESS1                           // Add Display Support for up to eigth Matrices
#define MTX_ADDRESS1                0
#endif
#ifndef MTX_ADDRESS2
#define MTX_ADDRESS2                0
#endif
#ifndef MTX_ADDRESS3
#define MTX_ADDRESS3                0
#endif
#ifndef MTX_ADDRESS4
#define MTX_ADDRESS4                0
#endif
#ifndef MTX_ADDRESS5
#define MTX_ADDRESS5                0
#endif
#ifndef MTX_ADDRESS6
#define MTX_ADDRESS6                0
#endif
#ifndef MTX_ADDRESS7
#define MTX_ADDRESS7                0
#endif
#ifndef MTX_ADDRESS8
#define MTX_ADDRESS8                0
#endif

#ifndef HOME_ASSISTANT_DISCOVERY_ENABLE
#define HOME_ASSISTANT_DISCOVERY_ENABLE 0
#endif

#ifndef LATITUDE
#define LATITUDE                    48.858360  // [Latitude] Your location to be used with sunrise and sunset
#endif
#ifndef LONGITUDE
#define LONGITUDE                   2.294442   // [Longitude] Your location to be used with sunrise and sunset
#endif

#ifndef IR_RCV_MIN_UNKNOWN_SIZE
#define IR_RCV_MIN_UNKNOWN_SIZE     6          // Set the smallest sized "UNKNOWN" message packets we actually care about (default 6, max 255)
#endif

#ifndef ENERGY_OVERTEMP
#define ENERGY_OVERTEMP             90         // Overtemp in Celsius
#endif

#ifndef DEFAULT_DIMMER_MAX
#define DEFAULT_DIMMER_MAX          100
#endif
#ifndef DEFAULT_DIMMER_MIN
#define DEFAULT_DIMMER_MIN          0
#endif
#ifndef DEFAULT_LIGHT_DIMMER
#define DEFAULT_LIGHT_DIMMER        10
#endif
#ifndef DEFAULT_LIGHT_COMPONENT
#define DEFAULT_LIGHT_COMPONENT     255
#endif

#ifndef CORS_ENABLED_ALL
#define CORS_ENABLED_ALL            "*"
#endif

#ifndef WORKING_PERIOD
#define WORKING_PERIOD              5          // Working period of the SDS Sensor, Takes a reading every X Minutes
#endif
#ifndef STARTING_OFFSET
#define STARTING_OFFSET             30         // NOVA SDS parameter used in settings
#endif

/*********************************************************************************************\
 * UserConfig related parameters
\*********************************************************************************************/

#ifndef COLOR_TEXT
#define COLOR_TEXT                  "#000"     // Global text color - Black
#endif
#ifndef COLOR_BACKGROUND
#define COLOR_BACKGROUND            "#fff"     // Global background color - White
#endif
#ifndef COLOR_FORM
#define COLOR_FORM                  "#f2f2f2"  // Form background color - Greyish
#endif
#ifndef COLOR_INPUT_TEXT
#define COLOR_INPUT_TEXT            "#000"     // Input text color - Black
#endif
#ifndef COLOR_INPUT
#define COLOR_INPUT                 "#fff"     // Input background color - White
#endif
#ifndef COLOR_CONSOLE_TEXT
#define COLOR_CONSOLE_TEXT          "#000"     // Console text color - Black
#endif
#ifndef COLOR_CONSOLE
#define COLOR_CONSOLE               "#fff"     // Console background color - White
#endif
#ifndef COLOR_TEXT_WARNING
#define COLOR_TEXT_WARNING          "#f00"     // Warning text color - Red
#endif
#ifndef COLOR_TEXT_SUCCESS
#define COLOR_TEXT_SUCCESS          "#008000"  // Success text color - Green
#endif
#ifndef COLOR_BUTTON_TEXT
#define COLOR_BUTTON_TEXT           "#fff"     // Button text color - White
#endif
#ifndef COLOR_BUTTON
#define COLOR_BUTTON                "#1fa3ec"  // Button color - Blueish
#endif
#ifndef COLOR_BUTTON_HOVER
#define COLOR_BUTTON_HOVER          "#0e70a4"  // Button color when hovered over - Darker blueish
#endif
#ifndef COLOR_BUTTON_RESET
#define COLOR_BUTTON_RESET          "#d43535"  // Restart/Reset/Delete button color - Redish
#endif
#ifndef COLOR_BUTTON_RESET_HOVER
#define COLOR_BUTTON_RESET_HOVER    "#931f1f"  // Restart/Reset/Delete button color when hovered over - Darker redish
#endif
#ifndef COLOR_BUTTON_SAVE
#define COLOR_BUTTON_SAVE           "#47c266"  // Save button color - Greenish
#endif
#ifndef COLOR_BUTTON_SAVE_HOVER
#define COLOR_BUTTON_SAVE_HOVER     "#5aaf6f"  // Save button color when hovered over - Darker greenish
#endif
#ifndef COLOR_TIMER_TAB_TEXT
#define COLOR_TIMER_TAB_TEXT        "#fff"     // Config timer tab text color - White
#endif
#ifndef COLOR_TIMER_TAB_BACKGROUND
#define COLOR_TIMER_TAB_BACKGROUND  "#999"     // Config timer tab background color - Light grey
#endif
#ifndef COLOR_TITLE_TEXT
#define COLOR_TITLE_TEXT			      COLOR_TEXT // Title text color defaults to global text color either dark or light
#endif

enum WebColors {
  COL_TEXT, COL_BACKGROUND, COL_FORM,
  COL_INPUT_TEXT, COL_INPUT, COL_CONSOLE_TEXT, COL_CONSOLE,
  COL_TEXT_WARNING, COL_TEXT_SUCCESS,
  COL_BUTTON_TEXT, COL_BUTTON, COL_BUTTON_HOVER, COL_BUTTON_RESET, COL_BUTTON_RESET_HOVER, COL_BUTTON_SAVE, COL_BUTTON_SAVE_HOVER,
  COL_TIMER_TAB_TEXT, COL_TIMER_TAB_BACKGROUND, COL_TITLE,
  COL_LAST };

const char kWebColors[] PROGMEM =
  COLOR_TEXT "|" COLOR_BACKGROUND "|" COLOR_FORM "|"
  COLOR_INPUT_TEXT "|" COLOR_INPUT "|" COLOR_CONSOLE_TEXT "|" COLOR_CONSOLE "|"
  COLOR_TEXT_WARNING "|" COLOR_TEXT_SUCCESS "|"
  COLOR_BUTTON_TEXT "|" COLOR_BUTTON "|" COLOR_BUTTON_HOVER "|" COLOR_BUTTON_RESET "|" COLOR_BUTTON_RESET_HOVER "|" COLOR_BUTTON_SAVE "|" COLOR_BUTTON_SAVE_HOVER "|"
  COLOR_TIMER_TAB_TEXT "|" COLOR_TIMER_TAB_BACKGROUND "|" COLOR_TITLE_TEXT;

/*********************************************************************************************\
 * ESP8266 vs ESP32 related parameters
\*********************************************************************************************/

#ifdef ESP8266

#ifndef MODULE
#define MODULE                      SONOFF_BASIC   // [Module] Select default model
#endif

#ifndef ARDUINO_ESP8266_RELEASE
#define ARDUINO_CORE_RELEASE        "STAGE"
#else
#define ARDUINO_CORE_RELEASE        ARDUINO_ESP8266_RELEASE
#endif  // ARDUINO_ESP8266_RELEASE

#endif  // ESP8266

#ifdef ESP32

#ifndef MODULE
#define MODULE                      WEMOS          // [Module] Select default model
#endif

#ifndef ARDUINO_ESP32_RELEASE
#define ARDUINO_CORE_RELEASE        "STAGE"
#else
#define ARDUINO_CORE_RELEASE        ARDUINO_ESP32_RELEASE
#endif  // ARDUINO_ESP32_RELEASE

#undef USE_HM10                     // Disable support for HM-10 as a BLE-bridge as an alternative is using the internal ESP32 BLE
#undef USE_KEELOQ                   // Disable support for Jarolift rollers by Keeloq algorithm as it's library cc1101 is not compatible with ESP32
#undef USE_DISPLAY_ILI9488          // Disable as it's library JaretBurkett_ILI9488-gemu-1.0 is not compatible with ESP32
#undef USE_DISPLAY_SSD1351          // Disable as it's library Adafruit_SSD1351_gemu-1.0 is not compatible with ESP32

#endif  // ESP32

/*********************************************************************************************\
 * Macros
\*********************************************************************************************/

/*
// Removed from esp8266 core since 20171105
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
*/
#define tmin(a,b) ((a)<(b)?(a):(b))
#define tmax(a,b) ((a)>(b)?(a):(b))

#define STR_HELPER(x) #x
#ifndef STR
#define STR(x) STR_HELPER(x)
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

#ifdef ESP8266
#define AGPIO(x) (x)
#else  // ESP32
#define AGPIO(x) (x<<5)
#endif  // ESP8266 - ESP32

#ifdef USE_DEVICE_GROUPS
#define SendDeviceGroupMessage(DEVICE_INDEX, REQUEST_TYPE, ...) _SendDeviceGroupMessage(DEVICE_INDEX, REQUEST_TYPE, __VA_ARGS__, 0)
#define SendLocalDeviceGroupMessage(REQUEST_TYPE, ...) _SendDeviceGroupMessage(0, REQUEST_TYPE, __VA_ARGS__, 0)
uint8_t device_group_count = 0;
#endif  // USE_DEVICE_GROUPS

#ifdef DEBUG_TASMOTA_CORE
#define DEBUG_CORE_LOG(...) AddLog_Debug(__VA_ARGS__)
#else
#define DEBUG_CORE_LOG(...)
#endif
#ifdef DEBUG_TASMOTA_DRIVER
#define DEBUG_DRIVER_LOG(...) AddLog_Debug(__VA_ARGS__)
#else
#define DEBUG_DRIVER_LOG(...)
#endif
#ifdef DEBUG_TASMOTA_SENSOR
#define DEBUG_SENSOR_LOG(...) AddLog_Debug(__VA_ARGS__)
#else
#define DEBUG_SENSOR_LOG(...)
#endif
#ifdef DEBUG_TASMOTA_TRACE
#define DEBUG_TRACE_LOG(...) AddLog_Debug(__VA_ARGS__)
#else
#define DEBUG_TRACE_LOG(...)
#endif

/*********************************************************************************************/

#endif  // _TASMOTA_GLOBALS_H_
