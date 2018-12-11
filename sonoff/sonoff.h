/*
  sonoff.h - Master header file for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

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

#ifndef _SONOFF_H_
#define _SONOFF_H_

/*********************************************************************************************\
 * Performance ROM (PROGMEM) vs RAM (RODATA)
\*********************************************************************************************/

//#define XFUNC_PTR_IN_ROM                    // Enable for keeping tables in ROM (PROGMEM) which seem to have access issues on some flash types

/*********************************************************************************************\
 * Default sensor states
\*********************************************************************************************/

#define CODE_IMAGE 0

#define USE_DHT                             // Default DHT11 sensor needs no external library
#define USE_ENERGY_SENSOR                   // Use energy sensors (+14k code)
#define USE_HLW8012                         // Use energy sensor for Sonoff Pow and WolfBlitz
#define USE_CSE7766                         // Use energy sensor for Sonoff S31 and Pow R2

/*********************************************************************************************\
 * Power Type
\*********************************************************************************************/

typedef unsigned long power_t;              // Power (Relay) type
#define POWER_MASK             0xffffffffUL // Power (Relay) full mask

/*********************************************************************************************\
 * Defines
\*********************************************************************************************/

// Changes to the following MAX_ defines will impact settings layout
#define MAX_SWITCHES           8            // Max number of switches
#define MAX_RELAYS             8            // Max number of relays
#define MAX_LEDS               4            // Max number of leds
#define MAX_KEYS               4            // Max number of keys or buttons
#define MAX_PWMS               5            // Max number of PWM channels
#define MAX_COUNTERS           4            // Max number of counter sensors
#define MAX_TIMERS             16           // Max number of Timers
#define MAX_PULSETIMERS        8            // Max number of supported pulse timers
#define MAX_FRIENDLYNAMES      4            // Max number of Friendly names
#define MAX_DOMOTICZ_IDX       4            // Max number of Domoticz device, key and switch indices
#define MAX_DOMOTICZ_SNS_IDX   12           // Max number of Domoticz sensors indices
#define MAX_KNX_GA             10           // Max number of KNX Group Addresses to read that can be set
#define MAX_KNX_CB             10           // Max number of KNX Group Addresses to write that can be set
#define MAX_XNRG_DRIVERS       32           // Max number of allowed energy drivers
#define MAX_XDSP_DRIVERS       32           // Max number of allowed display drivers
#define MAX_XDRV_DRIVERS       96           // Max number of allowed driver drivers
#define MAX_XSNS_DRIVERS       96           // Max number of allowed sensor drivers
#define MAX_RULE_MEMS          5            // Max number of saved vars
#define MAX_RULE_SETS          3            // Max number of rule sets of size 512 characters
#define MAX_RULE_SIZE          512          // Max number of characters in rules

// Changes to the following defines have no impact on settings layout
#define MAX_RULE_TIMERS        8            // Max number of rule timers (4 bytes / timer)
#define MAX_RULE_VARS          5            // Max number of rule variables (10 bytes / variable)

#define MAX_FAN_SPEED          4            // Max number of iFan02 fan speeds (0 .. 3)

#define MQTT_TOKEN_PREFIX      "%prefix%"   // To be substituted by mqtt_prefix[x]
#define MQTT_TOKEN_TOPIC       "%topic%"    // To be substituted by mqtt_topic, mqtt_grptopic, mqtt_buttontopic, mqtt_switchtopic
#define MQTT_TOKEN_HOSTNAME    "%hostname%" // To be substituted by mqtt_topic, mqtt_grptopic, mqtt_buttontopic, mqtt_switchtopic
#define MQTT_TOKEN_ID          "%id%"       // To be substituted by mqtt_topic, mqtt_grptopic, mqtt_buttontopic, mqtt_switchtopic

#define WIFI_HOSTNAME          "%s-%04d"    // Expands to <MQTT_TOPIC>-<last 4 decimal chars of MAC address>

#define CONFIG_FILE_SIGN       0xA5         // Configuration file signature
#define CONFIG_FILE_XOR        0x5A         // Configuration file xor (0 = No Xor)

#define HLW_PREF_PULSE         12530        // was 4975us = 201Hz = 1000W
#define HLW_UREF_PULSE         1950         // was 1666us = 600Hz = 220V
#define HLW_IREF_PULSE         3500         // was 1666us = 600Hz = 4.545A

#define MQTT_RETRY_SECS        10           // Minimum seconds to retry MQTT connection
#define GLOBAL_VALUES_VALID    300          // Max number of seconds to keep last received values
#define APP_POWER              0            // Default saved power state Off
#define WS2812_MAX_LEDS        512          // Max number of LEDs

#define PWM_RANGE              1023         // 255..1023 needs to be devisible by 256
//#define PWM_FREQ               1000         // 100..1000 Hz led refresh
//#define PWM_FREQ               910          // 100..1000 Hz led refresh (iTead value)
#define PWM_FREQ               880          // 100..1000 Hz led refresh (BN-SZ01 value)
#define PWM_MAX                4000         // [PWM_MAX] Maximum frequency - Default: 4000
#define PWM_MIN                100          // [PWM_MIN] Minimum frequency - Default: 100
                                            //    For Dimmers use double of your mains AC frequecy (100 for 50Hz and 120 for 60Hz)
                                            //    For Controlling Servos use 50 and also set PWM_FREQ as 50 (DO NOT USE THESE VALUES FOR DIMMERS)

#define DEFAULT_POWER_DELTA    80           // Power change percentage
#define MAX_POWER_HOLD         10           // Time in SECONDS to allow max agreed power
#define MAX_POWER_WINDOW       30           // Time in SECONDS to disable allow max agreed power
#define SAFE_POWER_HOLD        10           // Time in SECONDS to allow max unit safe power
#define SAFE_POWER_WINDOW      30           // Time in MINUTES to disable allow max unit safe power
#define MAX_POWER_RETRY        5            // Retry count allowing agreed power limit overflow

#define STATES                 20           // Number of states per second using 50 mSec interval
#define IMMINENT_RESET_FACTOR  10           // Factor to extent button hold time for imminent Reset to default 40 seconds using KEY_HOLD_TIME of 40
#define BOOT_LOOP_TIME         10           // Number of seconds to stop detecting boot loops
#define SYSLOG_TIMER           600          // Seconds to restore syslog_level
#define SERIALLOG_TIMER        600          // Seconds to disable SerialLog
#define OTA_ATTEMPTS           5            // Number of times to try fetching the new firmware

#define INPUT_BUFFER_SIZE      520          // Max number of characters in (serial and http) command buffer
#define CMDSZ                  24           // Max number of characters in command
#define TOPSZ                  100          // Max number of characters in topic string
#define LOGSZ                  520          // Max number of characters in log
#define MIN_MESSZ              893          // Min number of characters in MQTT message

#define SENSOR_MAX_MISS        5            // Max number of missed sensor reads before deciding it's offline

#ifdef USE_MQTT_TLS
  #define WEB_LOG_SIZE         2000         // Max number of characters in weblog
#else
  #define WEB_LOG_SIZE         4000         // Max number of characters in weblog
#endif

#define MAX_BACKLOG            30           // Max number of commands in backlog
#define MIN_BACKLOG_DELAY      2            // Minimal backlog delay in 0.1 seconds

#define SOFT_BAUDRATE          9600         // Default software serial baudrate
#define APP_BAUDRATE           115200       // Default serial baudrate
#define SERIAL_POLLING         100          // Serial receive polling in ms
#define MAX_STATUS             11           // Max number of status lines

#define DRIVER_BOOT_DELAY      1            // Number of milliseconds to retard driver cycles during boot-up time to reduce overall CPU load whilst Wifi is connecting
#define LOOP_SLEEP_DELAY       50           // Lowest number of milliseconds to go through the main loop using delay when needed

#define NO_EXTRA_4K_HEAP                    // Allocate 4k heap for WPS in ESP8166/Arduino core v2.4.2 (was always allocated in previous versions)

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

//enum ws2812NeopixelbusFeature { NEO_RGB, NEO_GRB, NEO_BRG, NEO_RBG, NEO_3LED, NEO_RGBW, NEO_GRBW };  // Doesn't work
#define NEO_RGB                0            // Neopixel RGB leds
#define NEO_GRB                1            // Neopixel GRB leds
#define NEO_BRG                2            // Neopixel BRG leds
#define NEO_RBG                3            // Neopixel RBG leds
#define NEO_3LED               4            // Placeholder to test for 4 led types
#define NEO_RGBW               5            // Neopixel RGBW leds
#define NEO_GRBW               6            // Neopixel GRBW leds

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
#define KNX_ENERGY_START       24
#define KNX_ENERGY_TOTAL       25
#define KNX_SLOT1              26
#define KNX_SLOT2              27
#define KNX_SLOT3              28
#define KNX_SLOT4              29
#define KNX_SLOT5              30
#define KNX_MAX_device_param   30
#define MAX_KNXTX_CMNDS        5

/*********************************************************************************************\
 * Enumeration
\*********************************************************************************************/

enum WeekInMonthOptions {Last, First, Second, Third, Fourth};
enum DayOfTheWeekOptions {Sun=1, Mon, Tue, Wed, Thu, Fri, Sat};
enum MonthNamesOptions {Jan=1, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec};
enum HemisphereOptions {North, South};
enum GetDateAndTimeOptions { DT_LOCAL, DT_UTC, DT_RESTART, DT_ENERGY };

enum LoggingLevels {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE, LOG_LEVEL_ALL};

enum WifiConfigOptions {WIFI_RESTART, WIFI_SMARTCONFIG, WIFI_MANAGER, WIFI_WPSCONFIG, WIFI_RETRY, WIFI_WAIT, WIFI_SERIAL, MAX_WIFI_OPTION};

enum SwitchModeOptions {TOGGLE, FOLLOW, FOLLOW_INV, PUSHBUTTON, PUSHBUTTON_INV, PUSHBUTTONHOLD, PUSHBUTTONHOLD_INV, PUSHBUTTON_TOGGLE, MAX_SWITCH_OPTION};

enum LedStateOptions {LED_OFF, LED_POWER, LED_MQTTSUB, LED_POWER_MQTTSUB, LED_MQTTPUB, LED_POWER_MQTTPUB, LED_MQTT, LED_POWER_MQTT, MAX_LED_OPTION};

enum EmulationOptions {EMUL_NONE, EMUL_WEMO, EMUL_HUE, EMUL_MAX};

enum TopicOptions { CMND, STAT, TELE, nu1, RESULT_OR_CMND, RESULT_OR_STAT, RESULT_OR_TELE };

enum ExecuteCommandPowerOptions { POWER_OFF, POWER_ON, POWER_TOGGLE, POWER_BLINK, POWER_BLINK_STOP, power_nu1, POWER_OFF_NO_STATE, POWER_ON_NO_STATE, power_nu2, POWER_SHOW_STATE };

enum PowerOnStateOptions { POWER_ALL_OFF, POWER_ALL_ON, POWER_ALL_SAVED_TOGGLE, POWER_ALL_SAVED, POWER_ALL_ALWAYS_ON, POWER_ALL_OFF_PULSETIME_ON };

enum ButtonStates { PRESSED, NOT_PRESSED };

enum Shortcuts { SC_CLEAR, SC_DEFAULT, SC_USER };

enum SettingsParmaIndex {P_HOLD_TIME, P_MAX_POWER_RETRY, P_TUYA_DIMMER_ID, P_MDNS_DELAYED_START, P_MAX_PARAM8};  // Max is PARAM8_SIZE (18) - SetOption32 until SetOption49

enum DomoticzSensors {DZ_TEMP, DZ_TEMP_HUM, DZ_TEMP_HUM_BARO, DZ_POWER_ENERGY, DZ_ILLUMINANCE, DZ_COUNT, DZ_VOLTAGE, DZ_CURRENT, DZ_AIRQUALITY, DZ_MAX_SENSORS};

enum Ws2812ClockIndex { WS_SECOND, WS_MINUTE, WS_HOUR, WS_MARKER };
enum Ws2812Color { WS_RED, WS_GREEN, WS_BLUE };

enum LightSubtypes { LST_NONE, LST_SINGLE, LST_COLDWARM, LST_RGB,   LST_RGBW, LST_RGBWC };                   // Do not insert new fields
enum LightTypes    { LT_BASIC, LT_PWM1,    LT_PWM2,      LT_PWM3,   LT_PWM4,  LT_PWM5,  LT_PWM6, LT_PWM7,
                     LT_NU8,   LT_SERIAL1, LT_SERIAL2,   LT_WS2812, LT_RGBW,  LT_RGBWC, LT_NU14, LT_NU15 };  // Do not insert new fields

enum LightSchemes {LS_POWER, LS_WAKEUP, LS_CYCLEUP, LS_CYCLEDN, LS_RANDOM, LS_MAX};

enum XsnsFunctions {FUNC_SETTINGS_OVERRIDE, FUNC_MODULE_INIT, FUNC_PRE_INIT, FUNC_INIT,
                    FUNC_LOOP, FUNC_EVERY_50_MSECOND, FUNC_EVERY_100_MSECOND, FUNC_EVERY_200_MSECOND, FUNC_EVERY_250_MSECOND, FUNC_EVERY_SECOND,
                    FUNC_PREP_BEFORE_TELEPERIOD, FUNC_JSON_APPEND, FUNC_WEB_APPEND, FUNC_SAVE_BEFORE_RESTART, FUNC_COMMAND,
                    FUNC_MQTT_SUBSCRIBE, FUNC_MQTT_INIT, FUNC_MQTT_DATA,
                    FUNC_SET_POWER, FUNC_SET_DEVICE_POWER, FUNC_SHOW_SENSOR,
                    FUNC_RULES_PROCESS, FUNC_SERIAL, FUNC_FREE_MEM, FUNC_BUTTON_PRESSED,
                    FUNC_WEB_ADD_BUTTON, FUNC_WEB_ADD_MAIN_BUTTON, FUNC_WEB_ADD_HANDLER};

enum CommandSource { SRC_IGNORE, SRC_MQTT, SRC_RESTART, SRC_BUTTON, SRC_SWITCH, SRC_BACKLOG, SRC_SERIAL, SRC_WEBGUI, SRC_WEBCOMMAND, SRC_WEBCONSOLE, SRC_PULSETIMER,
                     SRC_TIMER, SRC_RULE, SRC_MAXPOWER, SRC_MAXENERGY, SRC_LIGHT, SRC_KNX, SRC_DISPLAY, SRC_WEMO, SRC_HUE, SRC_RETRY, SRC_MAX };
const char kCommandSource[] PROGMEM = "I|MQTT|Restart|Button|Switch|Backlog|Serial|WebGui|WebCommand|WebConsole|PulseTimer|Timer|Rule|MaxPower|MaxEnergy|Light|Knx|Display|Wemo|Hue|Retry";

const uint8_t kDefaultRfCode[9] PROGMEM = { 0x21, 0x16, 0x01, 0x0E, 0x03, 0x48, 0x2E, 0x1A, 0x00 };

const uint8_t kIFan02Speed[MAX_FAN_SPEED][3] = {{6,6,6}, {7,6,6}, {7,7,6}, {7,6,7}};  // Do not use PROGMEM as it fails

/*********************************************************************************************\
 * Extern global variables
\*********************************************************************************************/

extern uint8_t light_device;  // Light device number

#endif  // _SONOFF_H_
