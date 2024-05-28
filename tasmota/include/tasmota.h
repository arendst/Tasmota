/*
  tasmota.h - Master header file for Tasmota

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

#ifndef _TASMOTA_H_
#define _TASMOTA_H_

/*********************************************************************************************\
 * Performance ROM (PROGMEM) vs RAM (RODATA)
\*********************************************************************************************/

#define XFUNC_PTR_IN_ROM                    // Enable for keeping tables in ROM (PROGMEM) which seem to have access issues on some flash types

/*********************************************************************************************\
 * Power Type
\*********************************************************************************************/

typedef uint32_t power_t;                   // Power (Relay) type
const uint32_t POWER_MASK = 0xFFFFFFFFUL;   // Power (Relay) full mask
const uint32_t POWER_SIZE = 32;             // Power (relay) bit count

/*********************************************************************************************\
 * Constants
\*********************************************************************************************/

// Why 28? Because in addition to relays there may be lights and uint32_t bitmap can hold up to 32 devices
#ifdef ESP8266
const uint8_t MAX_RELAYS = 8;               // Max number of relays selectable on GPIO
const uint8_t MAX_INTERLOCKS = 4;           // Max number of interlock groups (up to MAX_INTERLOCKS_SET)
const uint8_t MAX_SWITCHES = 8;             // Max number of switches selectable on GPIO
const uint8_t MAX_KEYS = 8;                 // Max number of keys or buttons selectable on GPIO
#endif  // ESP8266
#ifdef ESP32
const uint8_t MAX_RELAYS = 28;              // Max number of relays selectable on GPIO
const uint8_t MAX_INTERLOCKS = 14;          // Max number of interlock groups (up to MAX_INTERLOCKS_SET)
const uint8_t MAX_SWITCHES = 28;            // Max number of switches selectable on GPIO
const uint8_t MAX_KEYS = 28;                // Max number of keys or buttons selectable on GPIO
#endif  // ESP32
const uint8_t MAX_RELAYS_SET = 32;          // Max number of relays
const uint8_t MAX_KEYS_SET = 32;            // Max number of keys

// Changes to the following MAX_ defines will impact settings layout
const uint8_t MAX_INTERLOCKS_SET = 14;      // Max number of interlock groups (MAX_RELAYS_SET / 2)
const uint8_t MAX_SWITCHES_SET = 28;        // Max number of switches
const uint8_t MAX_LEDS = 4;                 // Max number of leds
const uint8_t MAX_PWMS_LEGACY = 5;          // Max number of PWM channels in first settings block - Legacy limit for ESP8266, but extended for ESP32 (see below)
#ifdef ESP32                                // Max number of PWM channels (total including extended) - ESP32 only
  #if CONFIG_IDF_TARGET_ESP32
  const uint8_t MAX_PWMS = 16;              // ESP32: 16 ledc PWM channels in total - TODO for now
  #elif CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
  const uint8_t MAX_PWMS = 8;               // ESP32S2/S3: 8 ledc PWM channels in total
  #elif CONFIG_IDF_TARGET_ESP32C2 || CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C6
  const uint8_t MAX_PWMS = 6;               // ESP32C2/C3/C6: 6 ledc PWM channels in total
  #else
  const uint8_t MAX_PWMS = 5;               // Unknown - revert to 5 PWM max
  #endif
#else
const uint8_t MAX_PWMS = 5;                 // (not used on ESP8266)
#endif
const uint8_t MAX_COUNTERS = 4;             // Max number of counter sensors
const uint8_t MAX_TIMERS = 16;              // Max number of Timers
const uint8_t MAX_PULSETIMERS = 32;         // Max number of supported pulse timers
const uint8_t MAX_DOMOTICZ_IDX = 4;         // Max number of Domoticz device, key and switch indices
const uint8_t MAX_DOMOTICZ_SNS_IDX = 12;    // Max number of Domoticz sensors indices
const uint8_t MAX_KNX_GA = 10;              // Max number of KNX Group Addresses to read that can be set
const uint8_t MAX_KNX_CB = 10;              // Max number of KNX Group Addresses to write that can be set
const uint8_t MAX_XNRG_DRIVERS = 32;        // Max number of allowed energy drivers
const uint8_t MAX_XDRV_DRIVERS = 96;        // Max number of allowed driver drivers
const uint8_t MAX_XSNS_DRIVERS = 128;       // Max number of allowed sensor drivers
const uint8_t MAX_I2C_DRIVERS = 96;         // Max number of allowed i2c drivers
const uint8_t MAX_SHUTTERS = 4;             // Max number of shutters
const uint8_t MAX_SHUTTER_KEYS = 4;         // Max number of shutter keys or buttons
const uint8_t MAX_PCF8574 = 4;              // Max number of PCF8574 devices
const uint8_t MAX_DS3502 = 4;               // Max number of DS3502 digitsal potentiometer devices
const uint8_t MAX_IRSEND = 16;              // Max number of IRSEND GPIOs
const uint8_t MAX_RULE_SETS = 3;            // Max number of rule sets of size 512 characters
const uint16_t MAX_RULE_SIZE = 512;         // Max number of characters in rules
const uint16_t VL53LXX_MAX_SENSORS = 8;     // Max number of VL53L0X sensors
const uint8_t MAX_SR04 = 3; // Max number of SR04 ultrasonic sensors

#ifdef ESP32
const uint8_t MAX_I2C = 2;                  // Max number of I2C controllers (ESP32 = 2)
const uint8_t MAX_SPI = 2;                  // Max number of Hardware SPI controllers (ESP32 = 2)
const uint8_t MAX_I2S = 2;                  // Max number of Hardware I2S controllers (ESP32 = 2)
  #if CONFIG_IDF_TARGET_ESP32
  const uint8_t MAX_RMT = 8;                // Max number or RMT channels (ESP32 only)
  #elif CONFIG_IDF_TARGET_ESP32S2
  const uint8_t MAX_RMT = 4;                // Max number or RMT channels (ESP32S2 only)
  #elif CONFIG_IDF_TARGET_ESP32S3
  const uint8_t MAX_RMT = 1;                // Max number or RMT channels (ESP32S3 only)
  #elif CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C6
  const uint8_t MAX_RMT = 2;                // Max number or RMT channels (ESP32C3 only)
  #else
  const uint8_t MAX_RMT = 0;                // Max number or RMT channels (0 if unknown)
  #endif
#else
const uint8_t MAX_I2C = 0;                  // Max number of I2C controllers (ESP8266 = 0, no choice)
const uint8_t MAX_SPI = 0;                  // Max number of Hardware SPI controllers (ESP8266 = 0, no choice)
const uint8_t MAX_I2S = 0;                  // Max number of Hardware I2S controllers (ESP8266 = 0, no choice)
const uint8_t MAX_RMT = 0;                  // No RMT channel on ESP8266
#endif

// Changes to the following MAX_ defines need to be in line with enum SettingsTextIndex
const uint8_t MAX_MQTT_PREFIXES = 3;        // Max number of MQTT prefixes (cmnd, stat, tele)
const uint8_t MAX_SSIDS = 2;                // Max number of SSIDs
const uint8_t MAX_STATE_TEXT = 4;           // Max number of State names (OFF, ON, TOGGLE, HOLD)
const uint8_t MAX_NTP_SERVERS = 3;          // Max number of NTP servers
const uint8_t MAX_RULE_MEMS = 16;           // Max number of saved vars
const uint8_t MAX_FRIENDLYNAMES = 8;        // Max number of Friendly names
const uint8_t MAX_BUTTON_TEXT = 32;         // Max number of GUI button labels
const uint8_t MAX_GROUP_TOPICS = 4;         // Max number of Group Topics
const uint8_t MAX_DEV_GROUP_NAMES = 4;      // Max number of Device Group names

#ifdef ESP8266
const uint8_t MAX_ADCS = 1;                 // Max number of ESP8266 ADC pins
const uint8_t MAX_SWITCHES_TXT = 8;         // Max number of switches user text
#endif  // ESP8266
#ifdef ESP32
  #if CONFIG_IDF_TARGET_ESP32C2 || CONFIG_IDF_TARGET_ESP32C3
  const uint8_t MAX_ADCS = 5;               // Max number of ESP32-C3 ADC pins (ADC2 pins are unusable with Wifi enabled)
  #elif CONFIG_IDF_TARGET_ESP32C6
  const uint8_t MAX_ADCS = 7;               // Max number of ESP32 ADC pins (ADC2 pins are unusable with Wifi enabled)
  #else   // ESP32
  const uint8_t MAX_ADCS = 8;               // Max number of ESP32 ADC pins (ADC2 pins are unusable with Wifi enabled)
  #endif  // ESP32C3
const uint8_t MAX_SWITCHES_TXT = 28;        // Max number of switches user text
#endif  // ESP32

const uint8_t MAX_HUE_DEVICES = 32;         // Max number of Philips Hue device per emulation
const uint8_t MAX_ROTARIES = 2;             // Max number of Rotary Encoders

const char MQTT_TOKEN_PREFIX[] PROGMEM = "%prefix%";  // To be substituted by mqtt_prefix[x]
const char MQTT_TOKEN_TOPIC[] PROGMEM = "%topic%";    // To be substituted by mqtt_topic, mqtt_grptopic, mqtt_buttontopic, mqtt_switchtopic

const uint8_t CONFIG_FILE_SIGN = 0xA5;      // Configuration file signature
const uint8_t CONFIG_FILE_XOR = 0x5A;       // Configuration file xor (0 = No Xor)

const uint32_t HLW_PREF_PULSE = 12530;      // was 4975us = 201Hz = 1000W
const uint32_t HLW_UREF_PULSE = 1950;       // was 1666us = 600Hz = 220V
const uint32_t HLW_IREF_PULSE = 3500;       // was 1666us = 600Hz = 4.545A

const uint8_t MQTT_RETRY_SECS = 10;         // Minimum seconds to retry MQTT connection
const uint32_t GLOBAL_VALUES_VALID = 300;   // Max number of seconds to keep last received values
const power_t APP_POWER = 0;                // Default saved power state Off
const uint16_t WS2812_MAX_LEDS = 512;       // Max number of LEDs

const uint32_t PWM_RANGE = 1023;            // 255..1023 needs to be devisible by 256
//const uint16_t PWM_FREQ = 1000;             // 100..1000 Hz led refresh
//const uint16_t PWM_FREQ = 910;              // 100..1000 Hz led refresh (iTead value)
const uint16_t PWM_FREQ = 977;              // 100..4000 Hz led refresh
#ifdef ESP32
const uint16_t PWM_MAX = 50000;             // [PWM_MAX] Maximum frequency for ESP32 - Default: 50000
const uint16_t PWM_MIN = 2;                 // [PWM_MIN] Minimum frequency for ESP32 - Default: 2
#else
const uint16_t PWM_MAX = 4000;              // [PWM_MAX] Maximum frequency - Default: 4000
const uint16_t PWM_MIN = 40;                // [PWM_MIN] Minimum frequency - Default: 40
#endif
                                            //    For Dimmers use double of your mains AC frequecy (100 for 50Hz and 120 for 60Hz)
                                            //    For Controlling Servos use 50 and also set PWM_FREQ as 50 (DO NOT USE THESE VALUES FOR DIMMERS)

const uint16_t MAX_POWER_HOLD = 10;         // Time in SECONDS to allow max agreed power
const uint16_t MAX_POWER_WINDOW = 30;       // Time in SECONDS to disable allow max agreed power
const uint16_t SAFE_POWER_HOLD = 10;        // Time in SECONDS to allow max unit safe power
const uint16_t SAFE_POWER_WINDOW = 30;      // Time in MINUTES to disable allow max unit safe power
const uint8_t MAX_POWER_RETRY = 5;          // Retry count allowing agreed power limit overflow

const uint8_t STATES = 20;                  // Number of states per second using 50 mSec interval
const uint8_t IMMINENT_RESET_FACTOR = 10;   // Factor to extent button hold time for imminent Reset to default 40 seconds using KEY_HOLD_TIME of 40
const uint32_t BOOT_LOOP_TIME = 10;         // Number of seconds to stop detecting boot loops
const uint32_t POWER_CYCLE_TIME = 8;        // Number of seconds to reset power cycle boot loops
const uint16_t SYSLOG_TIMER = 600;          // Seconds to restore syslog_level
const uint16_t SERIALLOG_TIMER = 600;       // Seconds to disable SerialLog
#ifdef ESP8266
const uint8_t OTA_ATTEMPTS = 10;            // Number of times to try fetching the new firmware
#else
const uint8_t OTA_ATTEMPTS = 5;             // Number of times to try fetching the new firmware
#endif  // ESP8266

//const uint16_t INPUT_BUFFER_SIZE = 520;     // Max number of characters in Tasmota serial command buffer
const uint16_t INPUT_BUFFER_SIZE = 800;     // Max number of characters in Tasmota serial command buffer
const uint16_t MIN_INPUT_BUFFER_SIZE = 256;  // Max number of characters in Tasmota serial command buffer
const uint16_t MAX_INPUT_BUFFER_SIZE = 2048; // Max number of characters in Arduino serial command buffer
const uint16_t FLOATSZ = 16;                // Max number of characters in float result from dtostrfd (max 32)
const uint16_t CMDSZ = 24;                  // Max number of characters in command
const uint16_t TOPSZ = 151;                 // Max number of characters in topic string

#ifdef ESP8266
  #ifdef PIO_FRAMEWORK_ARDUINO_MMU_CACHE16_IRAM48_SECHEAP_SHARED
  const uint16_t LOG_BUFFER_SIZE = 6096;      // Max number of characters in logbuffer used by weblog, syslog and mqttlog
  #else
  const uint16_t LOG_BUFFER_SIZE = 4096;      // Max number of characters in logbuffer used by weblog, syslog and mqttlog
  #endif  // PIO_FRAMEWORK_ARDUINO_MMU_CACHE16_IRAM48_SECHEAP_SHARED
#else   // Not ESP8266
const uint16_t LOG_BUFFER_SIZE = 6096;      // Max number of characters in logbuffer used by weblog, syslog and mqttlog
#endif  // ESP8266
const uint16_t MAX_LOGSZ = LOG_BUFFER_SIZE -96;  // Max number of characters in log line - may be overruled which will truncate log entry

const uint8_t SENSOR_MAX_MISS = 5;          // Max number of missed sensor reads before deciding it's offline

const uint32_t MIN_BACKLOG_DELAY = 200;     // Minimal backlog delay in mSeconds

const uint32_t SOFT_BAUDRATE = 9600;        // Default software serial baudrate
const uint32_t APP_BAUDRATE = 115200;       // Default serial baudrate
const uint32_t SERIAL_POLLING = 100;        // Serial receive polling in ms
const uint32_t ZIGBEE_POLLING = 100;        // Serial receive polling in ms
const uint8_t MAX_STATUS = 13;              // Max number of status lines

const uint32_t START_VALID_TIME = 1451602800;  // Time is synced and after 2016-01-01

const uint32_t DRIVER_BOOT_DELAY = 1;       // Number of milliseconds to retard driver cycles during boot-up time to reduce overall CPU load whilst Wifi is connecting
const uint32_t LOOP_SLEEP_DELAY = 50;       // Lowest number of milliseconds to go through the main loop using delay when needed

/*********************************************************************************************\
 * Defines
\*********************************************************************************************/

#define MAX_RULE_TIMERS        8            // Max number of rule timers (4 bytes / timer)
#define MAX_RULE_VARS          16           // Max number of rule variables (33 bytes / variable)

//enum ws2812NeopixelbusFeature { NEO_RGB, NEO_GRB, NEO_BRG, NEO_RBG, NEO_3LED, NEO_RGBW, NEO_GRBW };  // Doesn't work
#define NEO_RGB                0            // Neopixel RGB leds
#define NEO_GRB                1            // Neopixel GRB leds
#define NEO_BRG                2            // Neopixel BRG leds
#define NEO_RBG                3            // Neopixel RBG leds
#define NEO_3LED               4            // Placeholder to test for 4 led types
#define NEO_RGBW               5            // Neopixel RGBW leds
#define NEO_GRBW               6            // Neopixel GRBW leds

#define RGB_REMAP_RGBW         0
#define RGB_REMAP_RBGW         6
#define RGB_REMAP_GRBW         24
#define RGB_REMAP_GBRW         30
#define RGB_REMAP_BRGW         48
#define RGB_REMAP_BGRW         54

#define NEO_HW_WS2812          0            // NeoPixelBus hardware WS2812
#define NEO_HW_WS2812X         1            // NeoPixelBus hardware WS2812x like WS2812b
#define NEO_HW_WS2813          1            // NeoPixelBus hardware WS2813
#define NEO_HW_SK6812          2            // NeoPixelBus hardware SK6812
#define NEO_HW_LC8812          2            // NeoPixelBus hardware LC8812
#define NEO_HW_APA106          3            // NeoPixelBus hardware APA106
#define NEO_HW_P9813           4            // NeoPixelBus hardware P9813

#define MQTT_PUBSUBCLIENT      1            // Mqtt PubSubClient library
#define MQTT_TASMOTAMQTT       2            // Mqtt TasmotaMqtt library based on esp-mqtt-arduino - soon obsolete
#define MQTT_ESPMQTTARDUINO    3            // Mqtt esp-mqtt-arduino library by Ingo Randolf - obsolete but define is present for debugging purposes
#define MQTT_ARDUINOMQTT       4            // Mqtt arduino-mqtt library by Joel Gaehwiler (https://github.com/256dpi/arduino-mqtt)

// Sunrise and Sunset DawnType
#define DAWN_NORMAL            -0.8333
#define DAWN_CIVIL             -6.0
#define DAWN_NAUTIC            -12.0
#define DAWN_ASTRONOMIC        -18.0

// Sensor and Commands definition for KNX Driver
#define KNX_TEMPERATURE        17
#define KNX_HUMIDITY           18
#define KNX_ENERGY_VOLTAGE     19
#define KNX_ENERGY_CURRENT     20
#define KNX_ENERGY_POWER       21
#define KNX_ENERGY_POWERFACTOR 22
#define KNX_ENERGY_DAILY       23
#define KNX_ENERGY_YESTERDAY   24
#define KNX_ENERGY_TOTAL       25
#define KNX_SLOT1              26
#define KNX_SLOT2              27
#define KNX_SLOT3              28
#define KNX_SLOT4              29
#define KNX_SLOT5              30
#define KNX_SCENE              31
#define KNX_DIMMER             32   // aka DPT_Scaling 5.001
#define KNX_COLOUR             33   // aka DPT_Colour_RGB 232.600 or DPT_Colour_RGBW 251.600
#define KNX_MAX_device_param   33
#define MAX_KNXTX_CMNDS        5

// XPT2046 resistive touch driver min/max raw values
#define	XPT2046_MINX			192
#define XPT2046_MAXX			3895
#define XPT2046_MINY			346
#define	XPT2046_MAXY			3870

#ifdef ESP32
  #if CONFIG_IDF_TARGET_ESP32S2
    #define MAX_TX_PWR_DBM_11b    195
    #define MAX_TX_PWR_DBM_54g    150
    #define MAX_TX_PWR_DBM_n      130
    #define WIFI_SENSITIVITY_11b  -880
    #define WIFI_SENSITIVITY_54g  -750
    #define WIFI_SENSITIVITY_n    -720
  #elif CONFIG_IDF_TARGET_ESP32S3
    #define MAX_TX_PWR_DBM_11b    210
    #define MAX_TX_PWR_DBM_54g    190
    #define MAX_TX_PWR_DBM_n      185
    #define WIFI_SENSITIVITY_11b  -880
    #define WIFI_SENSITIVITY_54g  -760
    #define WIFI_SENSITIVITY_n    -720
  #elif CONFIG_IDF_TARGET_ESP32C2 || CONFIG_IDF_TARGET_ESP32C3
    #define MAX_TX_PWR_DBM_11b    210
    #define MAX_TX_PWR_DBM_54g    190
    #define MAX_TX_PWR_DBM_n      185
    #define WIFI_SENSITIVITY_11b  -880
    #define WIFI_SENSITIVITY_54g  -760
    #define WIFI_SENSITIVITY_n    -730
  #else
    #define MAX_TX_PWR_DBM_11b    195
    #define MAX_TX_PWR_DBM_54g    160
    #define MAX_TX_PWR_DBM_n      140
    #define WIFI_SENSITIVITY_11b  -880
    #define WIFI_SENSITIVITY_54g  -750
    #define WIFI_SENSITIVITY_n    -700
  #endif
#endif
#ifdef ESP8266
  #define MAX_TX_PWR_DBM_11b    200
  #define MAX_TX_PWR_DBM_54g    170
  #define MAX_TX_PWR_DBM_n      140
  #define WIFI_SENSITIVITY_11b  -910
  #define WIFI_SENSITIVITY_54g  -750
  #define WIFI_SENSITIVITY_n    -720
#endif

/*********************************************************************************************\
 * Enumeration
\*********************************************************************************************/

enum WeekInMonthOptions {Last, First, Second, Third, Fourth};
enum DayOfTheWeekOptions {Sun=1, Mon, Tue, Wed, Thu, Fri, Sat};
enum MonthNamesOptions {Jan=1, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec};
enum HemisphereOptions {North, South};
enum GetDateAndTimeOptions { DT_LOCAL, DT_UTC, DT_LOCALNOTZ, DT_DST, DT_STD, DT_RESTART, DT_BOOTCOUNT, DT_LOCAL_MILLIS };

enum LoggingLevels {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE};

enum InitStates {INIT_NONE, INIT_GPIOS, INIT_DONE};

enum WifiConfigOptions {WIFI_RESTART, EX_WIFI_SMARTCONFIG, WIFI_MANAGER, EX_WIFI_WPSCONFIG, WIFI_RETRY, WIFI_WAIT, WIFI_SERIAL, WIFI_MANAGER_RESET_ONLY, MAX_WIFI_OPTION};

enum WifiTestOptions {WIFI_NOT_TESTING, WIFI_TESTING, WIFI_TEST_FINISHED, WIFI_TEST_FINISHED_BAD};

enum SwitchModeOptions {TOGGLE,                              // 0
                        FOLLOW, FOLLOW_INV,                  // 1, 2   - Follow switch state
                        PUSHBUTTON, PUSHBUTTON_INV,          // 3, 4   - Pushbutton (default 1, 0 = toggle)
                        PUSHBUTTONHOLD, PUSHBUTTONHOLD_INV,  // 5, 6   - Pushbutton with hold (default 1, 0 = toggle, Hold = hold)
                        PUSHBUTTON_TOGGLE,                   // 7      - = 0
                        TOGGLEMULTI,                         // 8      - = 0 with multi toggle
                        FOLLOWMULTI, FOLLOWMULTI_INV,        // 9, 10  - Multi change follow (0 = off, 1 = on, 2x change = hold)
                        PUSHHOLDMULTI, PUSHHOLDMULTI_INV,    // 11, 12 - Pushbutton with dimmer mode
                        PUSHON, PUSHON_INV,                  // 13, 14 - Pushon mode (1 = on, switch off using PulseTime)
                        PUSH_IGNORE, PUSH_IGNORE_INV,        // 15, 16 - Send only MQTT message on switch change
                        MAX_SWITCH_OPTION};

enum LedStateOptions {LED_OFF, LED_POWER, LED_MQTTSUB, LED_POWER_MQTTSUB, LED_MQTTPUB, LED_POWER_MQTTPUB, LED_MQTT, LED_POWER_MQTT, MAX_LED_OPTION};

enum EmulationOptions {EMUL_NONE, EMUL_WEMO, EMUL_HUE, EMUL_MAX};

enum TopicOptions { CMND, STAT, TELE, nu1, RESULT_OR_CMND, RESULT_OR_STAT, RESULT_OR_TELE };

enum UploadTypes { UPL_TASMOTA = 1, UPL_SETTINGS, UPL_EFM8BB1, UPL_TASMOTACLIENT, UPL_EFR32, UPL_SHD, UPL_CCL, UPL_UFSFILE };

enum ExecuteCommandPowerOptions { POWER_OFF, POWER_ON, POWER_TOGGLE, POWER_BLINK, POWER_BLINK_STOP, POWER_OFF_FORCE,
                                  POWER_OFF_NO_STATE = 8, POWER_ON_NO_STATE, POWER_TOGGLE_NO_STATE,
                                  POWER_SHOW_STATE = 16 };
enum SendKeyPowerOptions { POWER_HOLD = 3, POWER_INCREMENT = 4, POWER_INV = 5, POWER_CLEAR = 6, POWER_RELEASE = 7,
                           POWER_100 = 8, CLEAR_RETAIN = 9, POWER_DELAYED = 10 };
enum SendKeyOptions { KEY_BUTTON, KEY_SWITCH };
enum SendKeyMultiClick { SINGLE = 10, DOUBLE = 11, TRIPLE = 12, QUAD = 13, PENTA = 14};

enum PowerOnStateOptions { POWER_ALL_OFF, POWER_ALL_ON, POWER_ALL_SAVED_TOGGLE, POWER_ALL_SAVED, POWER_ALL_ALWAYS_ON, POWER_ALL_OFF_PULSETIME_ON };

enum ButtonStates { PRESSED, NOT_PRESSED };

enum Shortcuts { SC_CLEAR, SC_DEFAULT, SC_USER };

enum SO32_49Index { P_HOLD_TIME,              // SetOption32 - (Button/Switch) Key hold time detection in decaseconds (default 40)
                    P_MAX_POWER_RETRY,        // SetOption33 - (Energy) Maximum number of retries before deciding power limit overflow (default 5)
                    P_BACKLOG_DELAY,          // SetOption34 - (Backlog) Minimal delay in milliseconds between executing backlog commands (default 200)
                    P_SERIAL_SKIP,            // SetOption35 - (SerialBridge) Skip number of serial messages received (default 0)
                    P_BOOT_LOOP_OFFSET,       // SetOption36 - (Restart) Number of restarts to start detecting boot loop (default 1)
                    P_RGB_REMAP,              // SetOption37 - (Light) RGB and White channel separation (default 0)
                    P_IR_UNKNOW_THRESHOLD,    // SetOption38 - (IR) Set the smallest sized "UNKNOWN" message packets we actually care about (default 6, max 255)
                    P_CSE7766_INVALID_POWER,  // SetOption39 - (CSE7766) Number of invalid power measurements before declaring it invalid allowing low load measurments (default 128)
                    P_HOLD_IGNORE,            // SetOption40 - (Button/Shutter) Ignore button change in seconds (default 0)
                    P_ARP_GRATUITOUS,         // SetOption41 - (Wifi) Interval in seconds between gratuitous ARP requests (default 60)
                    P_OVER_TEMP,              // SetOption42 - (Energy) Turn all power off at or above this temperature (default 90C)
                    P_ROTARY_MAX_STEP,        // SetOption43 - (Rotary) Rotary step boundary (default 10)
                    P_IR_TOLERANCE,           // SetOption44 - (IR) Base tolerance percentage for matching incoming IR messages (default 25, max 100)
                    P_BISTABLE_PULSE,         // SetOption45 - (Bistable) Pulse time for two coil bistable latching relays (default 40)
                    P_POWER_ON_DELAY,         // SetOption46 - (PowerOn) Add delay of 10 x value milliseconds at power on
                    P_POWER_ON_DELAY2,        // SetOption47 - (PowerOn) Add delay of value seconds at power on before activating relays
                    P_DUMMY_RELAYS,           // SetOption48 - (Energy) Support energy dummy relays
                    P_SO49_FREE               // SetOption49
                  };  // Max is PARAM8_SIZE (18) - SetOption32 until SetOption49

enum DomoticzSensors {DZ_TEMP, DZ_TEMP_HUM, DZ_TEMP_HUM_BARO, DZ_POWER_ENERGY, DZ_ILLUMINANCE, DZ_COUNT, DZ_VOLTAGE, DZ_CURRENT,
                      DZ_AIRQUALITY, DZ_P1_SMART_METER, DZ_SHUTTER, DZ_MAX_SENSORS};

enum Ws2812ClockIndex { WS_SECOND, WS_MINUTE, WS_HOUR, WS_MARKER };
enum Ws2812Color { WS_RED, WS_GREEN, WS_BLUE };

enum LightSubtypes { LST_NONE, LST_SINGLE, LST_COLDWARM, LST_RGB,   LST_RGBW, LST_RGBCW, LST_MAX=5 };   // Do not insert new fields
enum LightTypes    { LT_BASIC, LT_PWM1,    LT_PWM2,      LT_PWM3,   LT_PWM4,  LT_PWM5,  LT_PWM6, LT_PWM7,
                     LT_NU8,   LT_SERIAL1, LT_SERIAL2,   LT_RGB,    LT_RGBW,  LT_RGBWC, LT_NU14, LT_NU15 };  // Do not insert new fields

enum XsnsFunctions { FUNC_SETTINGS_OVERRIDE, FUNC_SETUP_RING1, FUNC_SETUP_RING2, FUNC_PRE_INIT, FUNC_INIT, FUNC_ACTIVE, FUNC_ABOUT_TO_RESTART,
                     FUNC_LOOP, FUNC_SLEEP_LOOP, FUNC_EVERY_50_MSECOND, FUNC_EVERY_100_MSECOND, FUNC_EVERY_200_MSECOND, FUNC_EVERY_250_MSECOND, FUNC_EVERY_SECOND,
                     FUNC_RESET_SETTINGS, FUNC_RESTORE_SETTINGS, FUNC_SAVE_SETTINGS, FUNC_SAVE_AT_MIDNIGHT, FUNC_SAVE_BEFORE_RESTART, FUNC_INTERRUPT_STOP, FUNC_INTERRUPT_START,
                     FUNC_AFTER_TELEPERIOD, FUNC_JSON_APPEND, FUNC_WEB_SENSOR, FUNC_WEB_COL_SENSOR,
                     FUNC_MQTT_SUBSCRIBE, FUNC_MQTT_INIT,
                     FUNC_SET_POWER, FUNC_SHOW_SENSOR, FUNC_ANY_KEY, FUNC_LED_LINK,
                     FUNC_ENERGY_EVERY_SECOND, FUNC_ENERGY_RESET,
                     FUNC_TELEPERIOD_RULES_PROCESS, FUNC_FREE_MEM,
                     FUNC_WEB_ADD_BUTTON, FUNC_WEB_ADD_CONSOLE_BUTTON, FUNC_WEB_ADD_MANAGEMENT_BUTTON, FUNC_WEB_ADD_MAIN_BUTTON,
                     FUNC_WEB_GET_ARG, FUNC_WEB_ADD_HANDLER, FUNC_SET_SCHEME, FUNC_HOTPLUG_SCAN, FUNC_TIME_SYNCED,
                     FUNC_DEVICE_GROUP_ITEM,
                     FUNC_NETWORK_UP, FUNC_NETWORK_DOWN,
                     FUNC_return_result = 200,  // Insert function WITHOUT return results before here. Following functions return results
                     FUNC_PIN_STATE, FUNC_MODULE_INIT, FUNC_ADD_BUTTON, FUNC_ADD_SWITCH, FUNC_BUTTON_PRESSED, FUNC_BUTTON_MULTI_PRESSED,
                     FUNC_SET_DEVICE_POWER,
                     FUNC_MQTT_DATA, FUNC_SERIAL,
                     FUNC_COMMAND, FUNC_COMMAND_SENSOR, FUNC_COMMAND_DRIVER,
                     FUNC_RULES_PROCESS,
                     FUNC_SET_CHANNELS,
                     FUNC_last_function         // Insert functions WITH return results before here
                     };

enum AddressConfigSteps { ADDR_IDLE, ADDR_RECEIVE, ADDR_SEND };

enum SettingsTextIndex { SET_OTAURL,
                         SET_MQTTPREFIX1, SET_MQTTPREFIX2, SET_MQTTPREFIX3,  // MAX_MQTT_PREFIXES
                         SET_STASSID1, SET_STASSID2,  // MAX_SSIDS
                         SET_STAPWD1, SET_STAPWD2,  // MAX_SSIDS
                         SET_HOSTNAME, SET_SYSLOG_HOST,
                         SET_WEBPWD, SET_CORS,
                         SET_MQTT_HOST, SET_MQTT_CLIENT,
                         SET_MQTT_USER, SET_MQTT_PWD,
                         SET_MQTT_FULLTOPIC, SET_MQTT_TOPIC,
                         SET_MQTT_BUTTON_TOPIC, SET_MQTT_SWITCH_TOPIC, SET_MQTT_GRP_TOPIC,
                         SET_STATE_TXT1, SET_STATE_TXT2, SET_STATE_TXT3, SET_STATE_TXT4,  // MAX_STATE_TEXT
                         SET_NTPSERVER1, SET_NTPSERVER2, SET_NTPSERVER3,  // MAX_NTP_SERVERS
                         SET_MEM1, SET_MEM2, SET_MEM3, SET_MEM4, SET_MEM5, SET_MEM6, SET_MEM7, SET_MEM8,
                         SET_MEM9, SET_MEM10, SET_MEM11, SET_MEM12, SET_MEM13, SET_MEM14, SET_MEM15, SET_MEM16,  // MAX_RULE_MEMS
                         SET_FRIENDLYNAME1, SET_FRIENDLYNAME2, SET_FRIENDLYNAME3, SET_FRIENDLYNAME4,
                         SET_FRIENDLYNAME5, SET_FRIENDLYNAME6, SET_FRIENDLYNAME7, SET_FRIENDLYNAME8,  // MAX_FRIENDLYNAMES
                         SET_BUTTON1, SET_BUTTON2, SET_BUTTON3, SET_BUTTON4, SET_BUTTON5, SET_BUTTON6, SET_BUTTON7, SET_BUTTON8,
                         SET_BUTTON9, SET_BUTTON10, SET_BUTTON11, SET_BUTTON12, SET_BUTTON13, SET_BUTTON14, SET_BUTTON15, SET_BUTTON16,  // MAX_BUTTON_TEXT
                         SET_MQTT_GRP_TOPIC2, SET_MQTT_GRP_TOPIC3, SET_MQTT_GRP_TOPIC4,  // MAX_GROUP_TOPICS
                         SET_TEMPLATE_NAME,
                         SET_DEV_GROUP_NAME1, SET_DEV_GROUP_NAME2, SET_DEV_GROUP_NAME3, SET_DEV_GROUP_NAME4,  // MAX_DEV_GROUP_NAMES
                         SET_DEVICENAME,
                         SET_TELEGRAM_TOKEN, SET_TELEGRAM_CHATID,
#ifdef ESP8266
                         SET_ADC_PARAM1,
                         SET_SWITCH_TXT1, SET_SWITCH_TXT2, SET_SWITCH_TXT3, SET_SWITCH_TXT4, SET_SWITCH_TXT5, SET_SWITCH_TXT6, SET_SWITCH_TXT7, SET_SWITCH_TXT8,  // MAX_SWITCHES_TXT
#endif  // ESP8266
#ifdef ESP32
                         SET_ADC_PARAM1, SET_ADC_PARAM2, SET_ADC_PARAM3, SET_ADC_PARAM4, SET_ADC_PARAM5, SET_ADC_PARAM6, SET_ADC_PARAM7, SET_ADC_PARAM8,  // MAX_ADCS
                         SET_SWITCH_TXT1, SET_SWITCH_TXT2, SET_SWITCH_TXT3, SET_SWITCH_TXT4, SET_SWITCH_TXT5, SET_SWITCH_TXT6, SET_SWITCH_TXT7, SET_SWITCH_TXT8,  // MAX_SWITCHES_TXT
                         SET_SWITCH_TXT9, SET_SWITCH_TXT10, SET_SWITCH_TXT11, SET_SWITCH_TXT12, SET_SWITCH_TXT13, SET_SWITCH_TXT14, SET_SWITCH_TXT15, SET_SWITCH_TXT16,  // MAX_SWITCHES_TXT
                         SET_SWITCH_TXT17, SET_SWITCH_TXT18, SET_SWITCH_TXT19, SET_SWITCH_TXT20, SET_SWITCH_TXT21, SET_SWITCH_TXT22, SET_SWITCH_TXT23, SET_SWITCH_TXT24,  // MAX_SWITCHES_TXT
                         SET_SWITCH_TXT25, SET_SWITCH_TXT26, SET_SWITCH_TXT27, SET_SWITCH_TXT28,  // MAX_SWITCHES_TXT
#endif  // ESP32
                         SET_SHD_PARAM,
                         SET_RGX_SSID, SET_RGX_PASSWORD,
                         SET_INFLUXDB_HOST, SET_INFLUXDB_PORT, SET_INFLUXDB_ORG, SET_INFLUXDB_TOKEN, SET_INFLUXDB_BUCKET, SET_INFLUXDB_RP,
                         SET_CANVAS,
                         SET_TELEGRAM_FINGERPRINT,
                         SET_MAX, // limit of texts stored in Settings
                         // Index above are not stored in Settings and should be handled specifically in SettingText()
                         SET_BUTTON17, SET_BUTTON18, SET_BUTTON19, SET_BUTTON20, SET_BUTTON21, SET_BUTTON22, SET_BUTTON23, SET_BUTTON24,
                         SET_BUTTON25, SET_BUTTON26, SET_BUTTON27, SET_BUTTON28, SET_BUTTON29, SET_BUTTON30, SET_BUTTON31, SET_BUTTON32,
                         SET_FINAL_MAX
                         };

enum SpiInterfaces { SPI_NONE, SPI_MOSI, SPI_MISO, SPI_MOSI_MISO };

enum DevGroupMessageType { DGR_MSGTYP_FULL_STATUS, DGR_MSGTYP_PARTIAL_UPDATE, DGR_MSGTYP_UPDATE, DGR_MSGTYP_UPDATE_MORE_TO_COME, DGR_MSGTYP_UPDATE_DIRECT, DGR_MSGTYPE_UPDATE_COMMAND, DGR_MSGTYPFLAG_WITH_LOCAL = 128 };

enum DevGroupMessageFlag { DGR_FLAG_RESET = 1, DGR_FLAG_STATUS_REQUEST = 2, DGR_FLAG_FULL_STATUS = 4, DGR_FLAG_ACK = 8, DGR_FLAG_MORE_TO_COME = 16, DGR_FLAG_DIRECT = 32, DGR_FLAG_ANNOUNCEMENT = 64, DGR_FLAG_LOCAL = 128 };

enum DevGroupItem { DGR_ITEM_EOL, DGR_ITEM_STATUS, DGR_ITEM_FLAGS,
                    DGR_ITEM_LIGHT_FADE, DGR_ITEM_LIGHT_SPEED, DGR_ITEM_LIGHT_BRI, DGR_ITEM_LIGHT_SCHEME, DGR_ITEM_LIGHT_FIXED_COLOR,
                    DGR_ITEM_BRI_PRESET_LOW, DGR_ITEM_BRI_PRESET_HIGH, DGR_ITEM_BRI_POWER_ON,
                    // Add new 8-bit items before this line
                    DGR_ITEM_LAST_8BIT, DGR_ITEM_MAX_8BIT = 63,
                    //DGR_ITEM_ANALOG1, DGR_ITEM_ANALOG2, DGR_ITEM_ANALOG3, DGR_ITEM_ANALOG4, DGR_ITEM_ANALOG5,
                    // Add new 16-bit items before this line
                    DGR_ITEM_LAST_16BIT, DGR_ITEM_MAX_16BIT = 127,
                    DGR_ITEM_POWER, DGR_ITEM_NO_STATUS_SHARE,
                    // Add new 32-bit items before this line
                    DGR_ITEM_LAST_32BIT, DGR_ITEM_MAX_32BIT = 191,
                    DGR_ITEM_EVENT, DGR_ITEM_COMMAND,
                    // Add new string items before this line
                    DGR_ITEM_LAST_STRING, DGR_ITEM_MAX_STRING = 223,
                    DGR_ITEM_LIGHT_CHANNELS };

enum DevGroupItemFlag { DGR_ITEM_FLAG_NO_SHARE = 1 };

enum DevGroupShareItem { DGR_SHARE_POWER = 1, DGR_SHARE_LIGHT_BRI = 2, DGR_SHARE_LIGHT_FADE = 4, DGR_SHARE_LIGHT_SCHEME = 8,
                         DGR_SHARE_LIGHT_COLOR = 16, DGR_SHARE_DIMMER_SETTINGS = 32, DGR_SHARE_EVENT = 64 };

enum CommandSource { SRC_IGNORE, SRC_MQTT, SRC_RESTART, SRC_BUTTON, SRC_SWITCH, SRC_BACKLOG, SRC_SERIAL, SRC_WEBGUI, SRC_WEBCOMMAND, SRC_WEBCONSOLE, SRC_PULSETIMER,
                     SRC_TIMER, SRC_RULE, SRC_MAXPOWER, SRC_MAXENERGY, SRC_OVERTEMP, SRC_LIGHT, SRC_KNX, SRC_DISPLAY, SRC_WEMO, SRC_HUE, SRC_RETRY, SRC_REMOTE, SRC_SHUTTER,
                     SRC_THERMOSTAT, SRC_CHAT, SRC_TCL, SRC_BERRY, SRC_FILE, SRC_SSERIAL, SRC_USBCONSOLE, SRC_SO47, SRC_SENSOR, SRC_WEB, SRC_MAX };
const char kCommandSource[] PROGMEM = "I|MQTT|Restart|Button|Switch|Backlog|Serial|WebGui|WebCommand|WebConsole|PulseTimer|"
                                      "Timer|Rule|MaxPower|MaxEnergy|Overtemp|Light|Knx|Display|Wemo|Hue|Retry|Remote|Shutter|"
                                      "Thermostat|Chat|TCL|Berry|File|SSerial|UsbConsole|SO47|Sensor|Web";

const uint8_t kDefaultRfCode[9] PROGMEM = { 0x21, 0x16, 0x01, 0x0E, 0x03, 0x48, 0x2E, 0x1A, 0x00 };

enum TasmotaSerialConfig {
  TS_SERIAL_5N1, TS_SERIAL_6N1, TS_SERIAL_7N1, TS_SERIAL_8N1,
  TS_SERIAL_5N2, TS_SERIAL_6N2, TS_SERIAL_7N2, TS_SERIAL_8N2,
  TS_SERIAL_5E1, TS_SERIAL_6E1, TS_SERIAL_7E1, TS_SERIAL_8E1,
  TS_SERIAL_5E2, TS_SERIAL_6E2, TS_SERIAL_7E2, TS_SERIAL_8E2,
  TS_SERIAL_5O1, TS_SERIAL_6O1, TS_SERIAL_7O1, TS_SERIAL_8O1,
  TS_SERIAL_5O2, TS_SERIAL_6O2, TS_SERIAL_7O2, TS_SERIAL_8O2 };

#ifdef ESP8266
const SerConfu8 kTasmotaSerialConfig[] PROGMEM = {
  SERIAL_5N1, SERIAL_6N1, SERIAL_7N1, SERIAL_8N1,
  SERIAL_5N2, SERIAL_6N2, SERIAL_7N2, SERIAL_8N2,
  SERIAL_5E1, SERIAL_6E1, SERIAL_7E1, SERIAL_8E1,
  SERIAL_5E2, SERIAL_6E2, SERIAL_7E2, SERIAL_8E2,
  SERIAL_5O1, SERIAL_6O1, SERIAL_7O1, SERIAL_8O1,
  SERIAL_5O2, SERIAL_6O2, SERIAL_7O2, SERIAL_8O2
};
#endif  // ESP8266
#ifdef ESP32
const uint32_t kTasmotaSerialConfig[] PROGMEM = {
  SERIAL_5N1, SERIAL_6N1, SERIAL_7N1, SERIAL_8N1,
  SERIAL_5N2, SERIAL_6N2, SERIAL_7N2, SERIAL_8N2,
  SERIAL_5E1, SERIAL_6E1, SERIAL_7E1, SERIAL_8E1,
  SERIAL_5E2, SERIAL_6E2, SERIAL_7E2, SERIAL_8E2,
  SERIAL_5O1, SERIAL_6O1, SERIAL_7O1, SERIAL_8O1,
  SERIAL_5O2, SERIAL_6O2, SERIAL_7O2, SERIAL_8O2
};
#endif  // ESP32

enum TuyaSupportedFunctions { TUYA_MCU_FUNC_NONE,
                              TUYA_MCU_FUNC_SWT1 = 1, TUYA_MCU_FUNC_SWT2, TUYA_MCU_FUNC_SWT3, TUYA_MCU_FUNC_SWT4,
                              TUYA_MCU_FUNC_REL1 = 11, TUYA_MCU_FUNC_REL2, TUYA_MCU_FUNC_REL3, TUYA_MCU_FUNC_REL4, TUYA_MCU_FUNC_REL5,
                                TUYA_MCU_FUNC_REL6, TUYA_MCU_FUNC_REL7, TUYA_MCU_FUNC_REL8,
                              TUYA_MCU_FUNC_DIMMER = 21, TUYA_MCU_FUNC_DIMMER2, TUYA_MCU_FUNC_CT, TUYA_MCU_FUNC_RGB, TUYA_MCU_FUNC_WHITE,
                                TUYA_MCU_FUNC_MODESET, TUYA_MCU_FUNC_REPORT1, TUYA_MCU_FUNC_REPORT2,
                              TUYA_MCU_FUNC_POWER = 31, TUYA_MCU_FUNC_CURRENT, TUYA_MCU_FUNC_VOLTAGE, TUYA_MCU_FUNC_BATTERY_STATE, TUYA_MCU_FUNC_BATTERY_PERCENTAGE, TUYA_MCU_FUNC_POWER_COMBINED, TUYA_MCU_FUNC_POWER_TOTAL,
                              TUYA_MCU_FUNC_REL1_INV = 41, TUYA_MCU_FUNC_REL2_INV, TUYA_MCU_FUNC_REL3_INV, TUYA_MCU_FUNC_REL4_INV, TUYA_MCU_FUNC_REL5_INV,
                                TUYA_MCU_FUNC_REL6_INV, TUYA_MCU_FUNC_REL7_INV, TUYA_MCU_FUNC_REL8_INV,
                              TUYA_MCU_FUNC_LOWPOWER_MODE = 51,
                              TUYA_MCU_FUNC_ENUM1 = 61, TUYA_MCU_FUNC_ENUM2, TUYA_MCU_FUNC_ENUM3, TUYA_MCU_FUNC_ENUM4,
                              TUYA_MCU_FUNC_TEMP = 71, TUYA_MCU_FUNC_TEMPSET, TUYA_MCU_FUNC_HUM, TUYA_MCU_FUNC_HUMSET,
                              TUYA_MCU_FUNC_LX = 75, TUYA_MCU_FUNC_TVOC, TUYA_MCU_FUNC_CO2, TUYA_MCU_FUNC_ECO2, TUYA_MCU_FUNC_GAS, TUYA_MCU_FUNC_PM25,
                              TUYA_MCU_FUNC_TIMER1 = 81, TUYA_MCU_FUNC_TIMER2, TUYA_MCU_FUNC_TIMER3, TUYA_MCU_FUNC_TIMER4,
                              TUYA_MCU_FUNC_MOTOR_DIR = 97,
                              TUYA_MCU_FUNC_ERROR = 98,
                              TUYA_MCU_FUNC_DUMMY = 99,
                              TUYA_MCU_FUNC_LAST = 255
                              // IDs from 230 to 234 are reserved for internal use
};

#endif  // _TASMOTA_H_
