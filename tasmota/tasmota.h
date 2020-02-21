/*
  tasmota.h - Master header file for Tasmota

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

#ifndef _TASMOTA_H_
#define _TASMOTA_H_

/*********************************************************************************************\
 * Performance ROM (PROGMEM) vs RAM (RODATA)
\*********************************************************************************************/

#define XFUNC_PTR_IN_ROM                    // Enable for keeping tables in ROM (PROGMEM) which seem to have access issues on some flash types

/*********************************************************************************************\
 * Default sensor states
\*********************************************************************************************/

#define CODE_IMAGE 0
#define CODE_IMAGE_STR "tasmota"

#define USE_LIGHT                           // Enable light control
#define USE_ENERGY_SENSOR                   // Use energy sensors (+14k code)
#define USE_HLW8012                         // Use energy sensor for Sonoff Pow and WolfBlitz
#define USE_CSE7766                         // Use energy sensor for Sonoff S31 and Pow R2

/*********************************************************************************************\
 * Power Type
\*********************************************************************************************/

typedef unsigned long power_t;              // Power (Relay) type
const uint32_t POWER_MASK = 0xffffffffUL;   // Power (Relay) full mask

/*********************************************************************************************\
 * Constants
\*********************************************************************************************/

// Changes to the following MAX_ defines will impact settings layout
const uint8_t MAX_SWITCHES = 8;             // Max number of switches
const uint8_t MAX_RELAYS = 8;               // Max number of relays
const uint8_t MAX_INTERLOCKS = 4;           // Max number of interlock groups (MAX_RELAYS / 2)
const uint8_t MAX_LEDS = 4;                 // Max number of leds
const uint8_t MAX_KEYS = 4;                 // Max number of keys or buttons
const uint8_t MAX_PWMS = 5;                 // Max number of PWM channels
const uint8_t MAX_COUNTERS = 4;             // Max number of counter sensors
const uint8_t MAX_TIMERS = 16;              // Max number of Timers
const uint8_t MAX_PULSETIMERS = 8;          // Max number of supported pulse timers
const uint8_t MAX_DOMOTICZ_IDX = 4;         // Max number of Domoticz device, key and switch indices
const uint8_t MAX_DOMOTICZ_SNS_IDX = 12;    // Max number of Domoticz sensors indices
const uint8_t MAX_KNX_GA = 10;              // Max number of KNX Group Addresses to read that can be set
const uint8_t MAX_KNX_CB = 10;              // Max number of KNX Group Addresses to write that can be set
const uint8_t MAX_XNRG_DRIVERS = 32;        // Max number of allowed energy drivers
const uint8_t MAX_XDSP_DRIVERS = 32;        // Max number of allowed display drivers
const uint8_t MAX_XDRV_DRIVERS = 96;        // Max number of allowed driver drivers
const uint8_t MAX_XSNS_DRIVERS = 96;        // Max number of allowed sensor drivers
const uint8_t MAX_I2C_DRIVERS = 96;         // Max number of allowed i2c drivers
const uint8_t MAX_SHUTTERS = 4;             // Max number of shutters
const uint8_t MAX_PCF8574 = 8;              // Max number of PCF8574 devices
const uint8_t MAX_RULE_SETS = 3;            // Max number of rule sets of size 512 characters
const uint16_t MAX_RULE_SIZE = 512;         // Max number of characters in rules

// Changes to the following MAX_ defines need to be in line with enum SettingsTextIndex
const uint8_t MAX_MQTT_PREFIXES = 3;        // Max number of MQTT prefixes (cmnd, stat, tele)
const uint8_t MAX_SSIDS = 2;                // Max number of SSIDs
const uint8_t MAX_STATE_TEXT = 4;           // Max number of State names (OFF, ON, TOGGLE, HOLD)
const uint8_t MAX_NTP_SERVERS = 3;          // Max number of NTP servers
const uint8_t MAX_RULE_MEMS = 16;           // Max number of saved vars
const uint8_t MAX_FRIENDLYNAMES = 8;        // Max number of Friendly names
const uint8_t MAX_BUTTON_TEXT = 16;         // Max number of GUI button labels

const uint8_t MAX_HUE_DEVICES = 15;         // Max number of Philips Hue device per emulation

const char MQTT_TOKEN_PREFIX[] PROGMEM = "%prefix%";  // To be substituted by mqtt_prefix[x]
const char MQTT_TOKEN_TOPIC[] PROGMEM = "%topic%";    // To be substituted by mqtt_topic, mqtt_grptopic, mqtt_buttontopic, mqtt_switchtopic
const char WIFI_HOSTNAME[] = "%s-%04d";     // Expands to <MQTT_TOPIC>-<last 4 decimal chars of MAC address>

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
const uint16_t PWM_FREQ = 880;              // 100..1000 Hz led refresh (BN-SZ01 value)
const uint16_t PWM_MAX = 4000;              // [PWM_MAX] Maximum frequency - Default: 4000
const uint16_t PWM_MIN = 100;               // [PWM_MIN] Minimum frequency - Default: 100
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
const uint8_t OTA_ATTEMPTS = 5;             // Number of times to try fetching the new firmware

const uint16_t INPUT_BUFFER_SIZE = 520;     // Max number of characters in (serial and http) command buffer
const uint16_t FLOATSZ = 16;                // Max number of characters in float result from dtostrfd (max 32)
const uint16_t CMDSZ = 24;                  // Max number of characters in command
const uint16_t TOPSZ = 151;                 // Max number of characters in topic string
const uint16_t LOGSZ = 700;                 // Max number of characters in log
const uint16_t MIN_MESSZ = 1040;            // Min number of characters in MQTT message (1000 - TOPSZ - 9 header bytes)

const uint8_t SENSOR_MAX_MISS = 5;          // Max number of missed sensor reads before deciding it's offline

const uint8_t MAX_BACKLOG = 30;             // Max number of commands in backlog
const uint32_t MIN_BACKLOG_DELAY = 200;     // Minimal backlog delay in mSeconds

const uint32_t SOFT_BAUDRATE = 9600;        // Default software serial baudrate
const uint32_t APP_BAUDRATE = 115200;       // Default serial baudrate
const uint32_t SERIAL_POLLING = 100;        // Serial receive polling in ms
const uint32_t ZIGBEE_POLLING = 100;        // Serial receive polling in ms
const uint8_t MAX_STATUS = 12;              // Max number of status lines

const uint32_t START_VALID_TIME = 1451602800;  // Time is synced and after 2016-01-01

const uint32_t DRIVER_BOOT_DELAY = 1;       // Number of milliseconds to retard driver cycles during boot-up time to reduce overall CPU load whilst Wifi is connecting
const uint32_t LOOP_SLEEP_DELAY = 50;       // Lowest number of milliseconds to go through the main loop using delay when needed

/*********************************************************************************************\
 * Defines
\*********************************************************************************************/

#define MAX_RULE_TIMERS        8            // Max number of rule timers (4 bytes / timer)
#define MAX_RULE_VARS          16           // Max number of rule variables (33 bytes / variable)

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
enum GetDateAndTimeOptions { DT_LOCAL, DT_UTC, DT_RESTART, DT_ENERGY, DT_BOOTCOUNT };

enum LoggingLevels {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE};

enum WifiConfigOptions {WIFI_RESTART, EX_WIFI_SMARTCONFIG, WIFI_MANAGER, EX_WIFI_WPSCONFIG, WIFI_RETRY, WIFI_WAIT, WIFI_SERIAL, WIFI_MANAGER_RESET_ONLY, MAX_WIFI_OPTION};

enum SwitchModeOptions {TOGGLE, FOLLOW, FOLLOW_INV, PUSHBUTTON, PUSHBUTTON_INV, PUSHBUTTONHOLD, PUSHBUTTONHOLD_INV, PUSHBUTTON_TOGGLE, TOGGLEMULTI,
                        FOLLOWMULTI, FOLLOWMULTI_INV, PUSHHOLDMULTI, PUSHHOLDMULTI_INV, MAX_SWITCH_OPTION};

enum LedStateOptions {LED_OFF, LED_POWER, LED_MQTTSUB, LED_POWER_MQTTSUB, LED_MQTTPUB, LED_POWER_MQTTPUB, LED_MQTT, LED_POWER_MQTT, MAX_LED_OPTION};

enum EmulationOptions {EMUL_NONE, EMUL_WEMO, EMUL_HUE, EMUL_MAX};

enum TopicOptions { CMND, STAT, TELE, nu1, RESULT_OR_CMND, RESULT_OR_STAT, RESULT_OR_TELE };

enum ExecuteCommandPowerOptions { POWER_OFF, POWER_ON, POWER_TOGGLE, POWER_BLINK, POWER_BLINK_STOP,
                                  POWER_OFF_NO_STATE = 8, POWER_ON_NO_STATE, POWER_TOGGLE_NO_STATE,
                                  POWER_SHOW_STATE = 16 };
enum SendKeyPowerOptions { POWER_HOLD = 3, POWER_INCREMENT = 4, POWER_INV = 5, POWER_CLEAR = 6, CLEAR_RETAIN = 9 };
enum SendKeyOptions { KEY_BUTTON, KEY_SWITCH };

enum PowerOnStateOptions { POWER_ALL_OFF, POWER_ALL_ON, POWER_ALL_SAVED_TOGGLE, POWER_ALL_SAVED, POWER_ALL_ALWAYS_ON, POWER_ALL_OFF_PULSETIME_ON };

enum ButtonStates { PRESSED, NOT_PRESSED };

enum Shortcuts { SC_CLEAR, SC_DEFAULT, SC_USER };

enum SettingsParamIndex { P_HOLD_TIME, P_MAX_POWER_RETRY, P_BACKLOG_DELAY, P_MDNS_DELAYED_START, P_BOOT_LOOP_OFFSET, P_RGB_REMAP, P_IR_UNKNOW_THRESHOLD,  // SetOption32 .. SetOption38
                          P_CSE7766_INVALID_POWER, P_HOLD_IGNORE, P_ex_TUYA_RELAYS, P_OVER_TEMP,  // SetOption39 .. SetOption42
                          P_ex_DIMMER_MAX, P_ex_TUYA_VOLTAGE_ID, P_ex_TUYA_CURRENT_ID, P_ex_TUYA_POWER_ID,  // SetOption43 .. SetOption46
                          P_ex_ENERGY_TARIFF1, P_ex_ENERGY_TARIFF2,  // SetOption47 .. SetOption48
                          P_MAX_PARAM8 };  // Max is PARAM8_SIZE (18) - SetOption32 until SetOption49

enum DomoticzSensors {DZ_TEMP, DZ_TEMP_HUM, DZ_TEMP_HUM_BARO, DZ_POWER_ENERGY, DZ_ILLUMINANCE, DZ_COUNT, DZ_VOLTAGE, DZ_CURRENT,
                      DZ_AIRQUALITY, DZ_P1_SMART_METER, DZ_SHUTTER, DZ_MAX_SENSORS};

enum Ws2812ClockIndex { WS_SECOND, WS_MINUTE, WS_HOUR, WS_MARKER };
enum Ws2812Color { WS_RED, WS_GREEN, WS_BLUE };

enum LightSubtypes { LST_NONE, LST_SINGLE, LST_COLDWARM, LST_RGB,   LST_RGBW, LST_RGBCW, LST_MAX=5 };   // Do not insert new fields
enum LightTypes    { LT_BASIC, LT_PWM1,    LT_PWM2,      LT_PWM3,   LT_PWM4,  LT_PWM5,  LT_PWM6, LT_PWM7,
                     LT_NU8,   LT_SERIAL1, LT_SERIAL2,   LT_RGB,    LT_RGBW,  LT_RGBWC, LT_NU14, LT_NU15 };  // Do not insert new fields

enum XsnsFunctions {FUNC_SETTINGS_OVERRIDE, FUNC_PIN_STATE, FUNC_MODULE_INIT, FUNC_PRE_INIT, FUNC_INIT,
                    FUNC_LOOP, FUNC_EVERY_50_MSECOND, FUNC_EVERY_100_MSECOND, FUNC_EVERY_200_MSECOND, FUNC_EVERY_250_MSECOND, FUNC_EVERY_SECOND,
                    FUNC_SAVE_AT_MIDNIGHT, FUNC_SAVE_BEFORE_RESTART,
                    FUNC_AFTER_TELEPERIOD, FUNC_JSON_APPEND, FUNC_WEB_SENSOR, FUNC_COMMAND, FUNC_COMMAND_SENSOR, FUNC_COMMAND_DRIVER,
                    FUNC_MQTT_SUBSCRIBE, FUNC_MQTT_INIT, FUNC_MQTT_DATA,
                    FUNC_SET_POWER, FUNC_SET_DEVICE_POWER, FUNC_SHOW_SENSOR, FUNC_ANY_KEY,
                    FUNC_ENERGY_EVERY_SECOND, FUNC_ENERGY_RESET,
                    FUNC_RULES_PROCESS, FUNC_SERIAL, FUNC_FREE_MEM, FUNC_BUTTON_PRESSED,
                    FUNC_WEB_ADD_BUTTON, FUNC_WEB_ADD_MAIN_BUTTON, FUNC_WEB_ADD_HANDLER, FUNC_SET_CHANNELS, FUNC_SET_SCHEME, FUNC_HOTPLUG_SCAN,
                    FUNC_DEVICE_GROUP_REQUEST };

enum AddressConfigSteps { ADDR_IDLE, ADDR_RECEIVE, ADDR_SEND };

enum SettingsTextIndex { SET_OTAURL,
                         SET_MQTTPREFIX1, SET_MQTTPREFIX2, SET_MQTTPREFIX3,
                         SET_STASSID1, SET_STASSID2,
                         SET_STAPWD1, SET_STAPWD2,
                         SET_HOSTNAME, SET_SYSLOG_HOST,
                         SET_WEBPWD, SET_CORS,
                         SET_MQTT_HOST, SET_MQTT_CLIENT,
                         SET_MQTT_USER, SET_MQTT_PWD,
                         SET_MQTT_FULLTOPIC, SET_MQTT_TOPIC,
                         SET_MQTT_BUTTON_TOPIC, SET_MQTT_SWITCH_TOPIC, SET_MQTT_GRP_TOPIC,
                         SET_STATE_TXT1, SET_STATE_TXT2, SET_STATE_TXT3, SET_STATE_TXT4,
                         SET_NTPSERVER1, SET_NTPSERVER2, SET_NTPSERVER3,
                         SET_MEM1, SET_MEM2, SET_MEM3, SET_MEM4, SET_MEM5, SET_MEM6, SET_MEM7, SET_MEM8,
                         SET_MEM9, SET_MEM10, SET_MEM11, SET_MEM12, SET_MEM13, SET_MEM14, SET_MEM15, SET_MEM16,
                         SET_FRIENDLYNAME1, SET_FRIENDLYNAME2, SET_FRIENDLYNAME3, SET_FRIENDLYNAME4,
                         SET_FRIENDLYNAME5, SET_FRIENDLYNAME6, SET_FRIENDLYNAME7, SET_FRIENDLYNAME8,
                         SET_BUTTON1, SET_BUTTON2, SET_BUTTON3, SET_BUTTON4, SET_BUTTON5, SET_BUTTON6, SET_BUTTON7, SET_BUTTON8,
                         SET_BUTTON9, SET_BUTTON10, SET_BUTTON11, SET_BUTTON12, SET_BUTTON13, SET_BUTTON14, SET_BUTTON15, SET_BUTTON16,
                         SET_MQTT_GRP_TOPIC2, SET_MQTT_GRP_TOPIC3, SET_MQTT_GRP_TOPIC4,
                         SET_MAX };

enum DeviceGroupMessageType { DGR_MSGTYP_FULL_STATUS, DGR_MSGTYP_PARTIAL_UPDATE, DGR_MSGTYP_UPDATE, DGR_MSGTYP_UPDATE_MORE_TO_COME, DGR_MSGTYP_UPDATE_DIRECT, DGR_MSGTYP_REUPDATE };

enum DeviceGroupMessageFlag { DGR_FLAG_RESET = 1, DGR_FLAG_STATUS_REQUEST = 2, DGR_FLAG_FULL_STATUS = 4, DGR_FLAG_ACK = 8, DGR_FLAG_MORE_TO_COME = 16, DGR_FLAG_DIRECT = 32 };

enum DeviceGroupItem { DGR_ITEM_EOL, DGR_ITEM_STATUS,
                       DGR_ITEM_LIGHT_FADE, DGR_ITEM_LIGHT_SPEED, DGR_ITEM_LIGHT_BRI, DGR_ITEM_LIGHT_SCHEME, DGR_ITEM_LIGHT_FIXED_COLOR,
                       DGR_ITEM_BRI_MIN, DGR_ITEM_BRI_PRESET_LOW, DGR_ITEM_BRI_PRESET_HIGH, DGR_ITEM_BRI_POWER_ON,
                       // Add new 8-bit items before this line
                       DGR_ITEM_LAST_8BIT, DGR_ITEM_MAX_8BIT = 63,
                       DGR_ITEM_ACK,
                       DGR_ITEM_ANALOG1, DGR_ITEM_ANALOG2, DGR_ITEM_ANALOG3, DGR_ITEM_ANALOG4, DGR_ITEM_ANALOG5,
                       // Add new 16-bit items before this line
                       DGR_ITEM_LAST_16BIT, DGR_ITEM_MAX_16BIT = 127,
                       DGR_ITEM_POWER,
                       // Add new 32-bit items before this line
                       DGR_ITEM_LAST_32BIT, DGR_ITEM_MAX_32BIT = 191,                       
                       // Add new string items before this line
                       DGR_ITEM_LAST_STRING, DGR_ITEM_MAX_STRING = 223,
                       DGR_ITEM_LIGHT_CHANNELS };

enum DeviceGroupShareItem { DGR_SHARE_POWER = 1, DGR_SHARE_LIGHT_BRI = 2, DGR_SHARE_LIGHT_FADE = 4, DGR_SHARE_LIGHT_SCHEME = 8,
                            DGR_SHARE_LIGHT_COLOR = 16, DGR_SHARE_BRI_MIN = 32 };

enum CommandSource { SRC_IGNORE, SRC_MQTT, SRC_RESTART, SRC_BUTTON, SRC_SWITCH, SRC_BACKLOG, SRC_SERIAL, SRC_WEBGUI, SRC_WEBCOMMAND, SRC_WEBCONSOLE, SRC_PULSETIMER,
                     SRC_TIMER, SRC_RULE, SRC_MAXPOWER, SRC_MAXENERGY, SRC_OVERTEMP, SRC_LIGHT, SRC_KNX, SRC_DISPLAY, SRC_WEMO, SRC_HUE, SRC_RETRY, SRC_REMOTE, SRC_SHUTTER,
                     SRC_MAX };
const char kCommandSource[] PROGMEM = "I|MQTT|Restart|Button|Switch|Backlog|Serial|WebGui|WebCommand|WebConsole|PulseTimer|"
                                      "Timer|Rule|MaxPower|MaxEnergy|Overtemp|Light|Knx|Display|Wemo|Hue|Retry|Remote|Shutter";

const uint8_t kDefaultRfCode[9] PROGMEM = { 0x21, 0x16, 0x01, 0x0E, 0x03, 0x48, 0x2E, 0x1A, 0x00 };

#endif  // _TASMOTA_H_
