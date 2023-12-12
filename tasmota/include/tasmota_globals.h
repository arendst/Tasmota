/*
  tasmota_globals.h - Function prototypes and global configurations for Tasmota

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

#include <esp-knx-ip.h> // KNX Header files have to be global else compile fails -> lib/headers
#ifdef USE_KNX
void KNX_CB_Action(message_t const &msg, void *arg);
#endif  // USE_KNX

void WifiShutdown(bool option = false);
void DomoticzTempHumPressureSensor(float temp, float hum, float baro = -1);
char* ToHex_P(const unsigned char * in, size_t insz, char * out, size_t outsz, char inbetween = '\0');
extern "C" void custom_crash_callback(struct rst_info * rst_info, uint32_t stack, uint32_t stack_end);
extern "C" void resetPins();
extern "C" int startWaveformClockCycles(uint8_t pin, uint32_t highCcys, uint32_t lowCcys,
  uint32_t runTimeCcys, int8_t alignPhase, uint32_t phaseOffsetCcys, bool autoPwm);
extern "C" void setTimer1Callback(uint32_t (*fn)());
#ifdef USE_SERIAL_BRIDGE
void SerialBridgePrintf(PGM_P formatP, ...);
#endif
#ifdef USE_INFLUXDB
void InfluxDbProcess(bool use_copy = false);
#endif

#ifdef ESP32
#if CONFIG_IDF_TARGET_ESP32       // ESP32/PICO-D4
#ifdef USE_ETHERNET
IPAddress EthernetLocalIP(void);
char* EthernetHostname(void);
String EthernetMacAddress(void);
#endif  // USE_ETHERNET
#endif  // CONFIG_IDF_TARGET_ESP32
#endif  // ESP32

/*********************************************************************************************\
 * Preconfigured configurations
\*********************************************************************************************/

#include "include/tasmota_configurations.h"            // Preconfigured configurations

/*-------------------------------------------------------------------------------------------*\
 * ESP8266 and ESP32 build time definitions
\*-------------------------------------------------------------------------------------------*/

// created in pio-tools/pre_source_dir.py
#if defined(CONFIG_TASMOTA_FLASHMODE_OPI)
  #define D_TASMOTA_FLASHMODE "OPI"
#elif (CONFIG_TASMOTA_FLASHMODE_QIO)
  #define D_TASMOTA_FLASHMODE "QIO"
#elif defined(CONFIG_TASMOTA_FLASHMODE_QOUT)
   #define D_TASMOTA_FLASHMODE "QOUT"
#elif defined(CONFIG_TASMOTA_FLASHMODE_DIO)
  #define D_TASMOTA_FLASHMODE "DIO"
#elif defined(CONFIG_TASMOTA_FLASHMODE_DOUT)
  #define D_TASMOTA_FLASHMODE "DOUT"
#else
#error "Please add missing flashmode definition in the lines above!" // could be upcoming octal modes
#endif // value check of CONFIG_TASMOTA_FLASHMODE

/*********************************************************************************************\
 * ESP8266 specific parameters
\*********************************************************************************************/

#ifdef ESP8266

#ifndef MODULE
#define MODULE                      SONOFF_BASIC   // [Module] Select default model
#endif
#ifndef FALLBACK_MODULE
#define FALLBACK_MODULE             SONOFF_BASIC   // [Module2] Select default module on fast reboot where USER_MODULE is user template
#endif

#ifndef ARDUINO_ESP8266_RELEASE
#define ARDUINO_CORE_RELEASE        "STAGE"
#else
#define ARDUINO_CORE_RELEASE        ARDUINO_ESP8266_RELEASE
#endif  // ARDUINO_ESP8266_RELEASE

#ifndef USE_ADC_VCC
#define USE_ADC
#else
#undef USE_ADC
#endif

#endif  // ESP8266

/*********************************************************************************************\
 * ESP32 specific parameters
\*********************************************************************************************/

#ifdef ESP32

/*-------------------------------------------------------------------------------------------*\
 * Start ESP32 specific parameters - disable features not present in ESP32
\*-------------------------------------------------------------------------------------------*/

#if CONFIG_IDF_TARGET_ESP32


#else   // Disable features not present in other ESP32 like ESP32C3, ESP32S2, ESP32S3 etc.
#ifdef USE_ETHERNET
#undef USE_ETHERNET                                // All non-ESP32 do not support ethernet
#endif
#endif  // CONFIG_IDF_TARGET_ESP32

/*-------------------------------------------------------------------------------------------*\
 * End ESP32 specific parameters
\*-------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------*\
 * Start ESP32-C3/C6 specific parameters - disable features not present in ESP32-C3/C6
\*-------------------------------------------------------------------------------------------*/

#if CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C6  // ESP32-C3/C6
//#ifdef USE_ETHERNET
//#undef USE_ETHERNET                                // ESP32-C3/C6 does not support ethernet
//#endif

#endif  // CONFIG_IDF_TARGET_ESP32C3/C6

/*-------------------------------------------------------------------------------------------*\
 * End ESP32-C3 specific parameters
\*-------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*\
 * Start ESP32-S2 specific parameters - disable features not present in ESP32-S2
\*-------------------------------------------------------------------------------------------*/

#if CONFIG_IDF_TARGET_ESP32S2                      // ESP32-S2
//#ifdef USE_ETHERNET
//#undef USE_ETHERNET                                // ESP32-S2 does not support ethernet
//#endif
#ifdef USE_BLE_ESP32
#undef USE_BLE_ESP32                               // ESP32-S2 does not support BLE
#endif
#ifdef USE_MI_ESP32
#undef USE_MI_ESP32                                // ESP32-S2 does not support BLE
#endif
#ifdef USE_IBEACON_ESP32
#undef USE_IBEACON_ESP32                           // ESP32-S2 does not support BLE
#endif
#endif  // CONFIG_IDF_TARGET_ESP32S2

/*-------------------------------------------------------------------------------------------*\
 * End ESP32-S2 specific parameters
\*-------------------------------------------------------------------------------------------*/

#ifndef MODULE
#define MODULE                      WEMOS          // [Module] Select default model
#endif
#ifndef FALLBACK_MODULE
#define FALLBACK_MODULE             WEMOS          // [Module2] Select default module on fast reboot where USER_MODULE is user template
#endif

#ifndef ARDUINO_ESP32_RELEASE
#define ARDUINO_CORE_RELEASE        "STAGE"
#else
#define ARDUINO_CORE_RELEASE        ARDUINO_ESP32_RELEASE
#endif  // ARDUINO_ESP32_RELEASE

// Hardware has no ESP32
#undef USE_EXS_DIMMER
#undef USE_ARMTRONIX_DIMMERS
#undef USE_SONOFF_RF
#undef USE_SONOFF_SC
#undef USE_SONOFF_IFAN
#undef USE_SONOFF_L1
#undef USE_SONOFF_D1
#undef USE_SHELLY_DIMMER
#undef USE_RF_FLASH

// Not ported (yet)
#undef USE_PS_16_DZ

#undef USE_HM10                     // Disable support for HM-10 as a BLE-bridge as an alternative is using the internal ESP32 BLE
#undef USE_KEELOQ                   // Disable support for Jarolift rollers by Keeloq algorithm as it's library cc1101 is not compatible with ESP32

#endif  // ESP32

/*********************************************************************************************\
 * Fallback parameters
\*********************************************************************************************/

#if defined(USE_PID) && (!defined(PID_USE_TIMPROP) || (PID_USE_TIMPROP > 0))
#define USE_TIMEPROP
#endif
                                               // See https://github.com/esp8266/Arduino/pull/4889
#undef NO_EXTRA_4K_HEAP                        // Allocate 4k heap for WPS in ESP8166/Arduino core v2.4.2 (was always allocated in previous versions)

#ifndef USE_SONOFF_RF
#undef USE_RF_FLASH                            // Disable RF firmware flash when Sonoff Rf is disabled
#endif

#ifndef USE_ZIGBEE
#undef USE_ZIGBEE_EZSP                         // Disable Zigbee EZSP firmware flash
#endif

#ifndef USE_LIGHT
#undef SHELLY_FW_UPGRADE                       // Disable Shelly Dimmer firmware flash when lights are disabled
#endif
#ifndef USE_SHELLY_DIMMER
#undef SHELLY_FW_UPGRADE                       // Disable Shelly Dimmer firmware flash when Shelly Dimmer is disabled
#endif

#ifndef APP_INTERLOCK_MODE
#define APP_INTERLOCK_MODE     false           // [Interlock] Relay interlock mode
#endif
#ifndef APP_INTERLOCK_GROUP_1
#define APP_INTERLOCK_GROUP_1  0xFF            // [Interlock] Relay bitmask for interlock group 1 - Legacy support using all relays in one interlock group
#endif
#ifndef APP_INTERLOCK_GROUP_2
#define APP_INTERLOCK_GROUP_2  0x00            // [Interlock] Relay bitmask for interlock group 2
#endif
#ifndef APP_INTERLOCK_GROUP_3
#define APP_INTERLOCK_GROUP_3  0x00            // [Interlock] Relay bitmask for interlock group 3
#endif
#ifndef APP_INTERLOCK_GROUP_4
#define APP_INTERLOCK_GROUP_4  0x00            // [Interlock] Relay bitmask for interlock group 4
#endif

#ifndef SWITCH_MODE
#define SWITCH_MODE                 TOGGLE     // TOGGLE, FOLLOW or FOLLOW_INV (the wall switch state)
#endif

#ifndef MQTT_FINGERPRINT1
// Set an all-zeros default fingerprint to activate auto-learning on first connection (AWS IoT)
#define MQTT_FINGERPRINT1      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00  // [MqttFingerprint1] (auto-learn)
#endif
#ifndef MQTT_FINGERPRINT2           // SHA1('')
#define MQTT_FINGERPRINT2      0xDA,0x39,0xA3,0xEE,0x5E,0x6B,0x4B,0x0D,0x32,0x55,0xBF,0xEF,0x95,0x60,0x18,0x90,0xAF,0xD8,0x07,0x09  // [MqttFingerprint2] (invalid)
#endif

#ifndef WS2812_LEDS
#define WS2812_LEDS                 30         // [Pixels] Number of LEDs
#endif

#if defined(ARDUINO_ESP8266_RELEASE_2_3_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_1) || defined(ARDUINO_ESP8266_RELEASE_2_4_2) || defined(ARDUINO_ESP8266_RELEASE_2_5_0) || defined(ARDUINO_ESP8266_RELEASE_2_5_1) || defined(ARDUINO_ESP8266_RELEASE_2_5_2)
  #error "Arduino ESP8266 Core versions before 2.7.1 are not supported"
#endif

#define UFS_FILE_WRITE              "w"
#define UFS_FILE_READ               "r"
#define FS_FILE_WRITE               "w"
#define FS_FILE_READ                "r"
#define FS_FILE_APPEND              "a"

#define TASM_FILE_SETTINGS          "/.settings"       // Settings binary blob
#define TASM_FILE_SETTINGS_LKG      "/.settings.lkg"   // Last Known Good Settings binary blob
#define TASM_FILE_DRIVER            "/.drvset%03d"
#define TASM_FILE_SENSOR            "/.snsset%03d"
#define TASM_FILE_TLSKEY            "/tlskey"          // TLS private key
#define TASM_FILE_ZIGBEE_LEGACY_V2  "/zb"              // Zigbee devices information blob, legacy v2
#define TASM_FILE_ZIGBEE            "/zbv4"            // Zigbee devices information blob, now v4
#define TASM_FILE_ZIGBEE_DATA       "/zbdata"          // Zigbee last known values of devices
#define TASM_FILE_AUTOEXEC          "/autoexec.bat"    // Commands executed after restart
#define TASM_FILE_CONFIG            "/config.sys"      // Settings executed after restart

#ifndef DNS_TIMEOUT
#define DNS_TIMEOUT                 1000       // Milliseconds
#endif

#ifndef MQTT_MAX_PACKET_SIZE
#define MQTT_MAX_PACKET_SIZE        1200       // Bytes
//#define MQTT_MAX_PACKET_SIZE        2048       // Bytes
#endif
#ifndef MQTT_KEEPALIVE
#define MQTT_KEEPALIVE              30         // Seconds
#endif
#ifndef MQTT_SOCKET_TIMEOUT
#define MQTT_SOCKET_TIMEOUT         4          // Seconds
#endif
#ifndef MQTT_WIFI_CLIENT_TIMEOUT
#define MQTT_WIFI_CLIENT_TIMEOUT    200        // Wifi TCP connection timeout (default is 5000 mSec)
#endif
#ifndef MQTT_CLEAN_SESSION
#define MQTT_CLEAN_SESSION          1          // 0 = No clean session, 1 = Clean session (default)
#endif
#ifndef MQTT_DISABLE_SSERIALRECEIVED
#define MQTT_DISABLE_SSERIALRECEIVED 0         // 1 = Disable sserialreceived mqtt messages, 0 = Enable sserialreceived mqtt messages (default)
#endif
#ifndef MQTT_LWT_OFFLINE
#define MQTT_LWT_OFFLINE            "Offline"  // MQTT LWT offline topic message
#endif
#ifndef MQTT_LWT_ONLINE
#define MQTT_LWT_ONLINE             "Online"   // MQTT LWT online topic message
#endif

#ifndef MESSZ
#define MESSZ                       1040       // Max number of characters in JSON message string (Hass discovery and nice MQTT_MAX_PACKET_SIZE = 1200)
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

#ifndef IR_RCV_TOLERANCE
#define IR_RCV_TOLERANCE            25         // Base tolerance percentage for matching incoming IR messages (default 25, max 100)
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
#ifndef DEFAULT_DIMMER_STEP
#define DEFAULT_DIMMER_STEP         10
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

#ifndef WIFI_RGX_STATE
#define WIFI_RGX_STATE              0
#endif
#ifndef WIFI_RGX_NAPT
#define WIFI_RGX_NAPT               0
#endif
#ifndef WIFI_RGX_SSID
#define WIFI_RGX_SSID               ""
#endif
#ifndef WIFI_RGX_PASSWORD
#define WIFI_RGX_PASSWORD           ""
#endif
#ifndef WIFI_RGX_IP_ADDRESS
#define WIFI_RGX_IP_ADDRESS         "192.168.99.1"
#endif
#ifndef WIFI_RGX_SUBNETMASK
#define WIFI_RGX_SUBNETMASK         "255.255.255.0"
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
 * Macros
\*********************************************************************************************/

/*
// Removed from esp8266 core since 20171105
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
*/
#define tmin(a,b) ((a)<(b)?(a):(b))
#define tmax(a,b) ((a)>(b)?(a):(b))

#define nitems(_a) (sizeof((_a)) / sizeof((_a)[0]))

#define STR_HELPER(x) #x
#ifndef STR
#define STR(x) STR_HELPER(x)
#endif

#define AGPIO(x) ((x)<<5)
#define BGPIO(x) ((x)>>5)

#ifdef USE_DEVICE_GROUPS
#define SendDeviceGroupMessage(DEVICE_INDEX, REQUEST_TYPE, ...) _SendDeviceGroupMessage(DEVICE_INDEX, REQUEST_TYPE, __VA_ARGS__, 0)
uint8_t device_group_count = 0;
bool first_device_group_is_local = true;
#endif  // USE_DEVICE_GROUPS

#ifdef DEBUG_TASMOTA_CORE
#define DEBUG_CORE_LOG(...) AddLog(LOG_LEVEL_DEBUG, __VA_ARGS__)
#else
#define DEBUG_CORE_LOG(...)
#endif
#ifdef DEBUG_TASMOTA_DRIVER
#define DEBUG_DRIVER_LOG(...) AddLog(LOG_LEVEL_DEBUG, __VA_ARGS__)
#else
#define DEBUG_DRIVER_LOG(...)
#endif
#ifdef DEBUG_TASMOTA_SENSOR
#define DEBUG_SENSOR_LOG(...) AddLog(LOG_LEVEL_DEBUG, __VA_ARGS__)
#else
#define DEBUG_SENSOR_LOG(...)
#endif
#ifdef DEBUG_TASMOTA_TRACE
#define DEBUG_TRACE_LOG(...) AddLog(LOG_LEVEL_DEBUG, __VA_ARGS__)
#else
#define DEBUG_TRACE_LOG(...)
#endif

#ifdef USE_DEBUG_DRIVER
#define SHOW_FREE_MEM(WHERE) ShowFreeMem(WHERE);
#else
#define SHOW_FREE_MEM(WHERE)
#endif

#ifndef USE_PROFILING
#undef USE_PROFILE_DRIVER
#undef USE_PROFILE_FUNCTION
#endif

#ifdef USE_PROFILE_DRIVER
#define PROFILE_DRIVER(DRIVER, FUNCTION, START) AddLogDriver(DRIVER, FUNCTION, START)
#else
#define PROFILE_DRIVER(DRIVER, FUNCTION, START)
#endif  // USE_PROFILE_DRIVER

#ifdef USE_PROFILE_FUNCTION
#define PROFILE_FUNCTION(DRIVER, INDEX, FUNCTION, START) AddLogFunction(DRIVER, INDEX, FUNCTION, START)
#else
#define PROFILE_FUNCTION(DRIVER, INDEX, FUNCTION, START)
#endif  // USE_PROFILE_DRIVER

/*********************************************************************************************\
 * Macro for SetOption synonyms
 *
 * SetOption synonyms come first in the list of commands, right after the prefix.
 * They don't need any matching function pointer, since they are handled internally.
 * So don't forget to NOT put pointers in the functions pointers list.
 *
 * The additionnal list of unsigned bytes contains the Option numbers of each synonyms
 * in the same order. The first byte of the list contains the number of synonyms
 * (not including the number itself). The is the number of names to skip to find the first command.
 *
 * As it si cumbersome to calculate the first byte (number of synonyms), we provide the following
 * macro `SO_SYNONYMS(<name>, <list of bytes>)`
 *
 * For example:
 * ```
 *   SO_SYNONYMS(kLightSynonyms,
 *     37, 68, 82, 91, 92, 105,
 *     106,
 *   );
 * ```
 *
 * is equivalent to:
 * ```
 *   const static uint8_t kLightSynonyms[] PROGMEM = {
 *     7,   // number of synonyms, automatically calculated
 *     37, 68, 82, 91, 92, 105,
 *     106,
 *   };
 * ```
 *
 * This comes very handy if you need to adjust the number of synonyms depending on #defines
\*********************************************************************************************/

#define SO_SYNONYMS(N,...) const static uint8_t __syn_array_len_ ## N[] = { __VA_ARGS__ }; /* this first array will not be kept by linker, just used for sizeof() */ const static uint8_t N[] PROGMEM = { sizeof(__syn_array_len_ ## N), __VA_ARGS__ };

/*********************************************************************************************/

#endif  // _TASMOTA_GLOBALS_H_
