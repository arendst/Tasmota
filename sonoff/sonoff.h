/*
  sonoff.h - Sonoff-Tasmota master header

  Copyright (C) 2017  Theo Arends

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

/*********************************************************************************************\
 * Power Type
\*********************************************************************************************/

typedef unsigned long power_t;              // Power (Relay) type
#define POWER_MASK             0xffffffffUL // Power (Relay) full mask

/*********************************************************************************************\
 * Defines
\*********************************************************************************************/

// Changes to the following MAX_ defines will impact settings layout
#define MAX_RELAYS             8            // Max number of relays
#define MAX_LEDS               4            // Max number of leds
#define MAX_KEYS               4            // Max number of keys or buttons
#define MAX_SWITCHES           4            // Max number of switches
#define MAX_PWMS               5            // Max number of PWM channels
#define MAX_COUNTERS           4            // Max number of counter sensors
#define MAX_PULSETIMERS        8            // Max number of supported pulse timers
#define MAX_FRIENDLYNAMES      4            // Max number of Friendly names
#define MAX_DOMOTICZ_IDX       4            // Max number of Domoticz device, key and switch indices

#define MODULE                 SONOFF_BASIC // [Module] Select default model

#define MQTT_TOKEN_PREFIX      "%prefix%"   // To be substituted by mqtt_prefix[x]
#define MQTT_TOKEN_TOPIC       "%topic%"    // To be substituted by mqtt_topic, mqtt_grptopic, mqtt_buttontopic, mqtt_switchtopic

#define WIFI_HOSTNAME          "%s-%04d"    // Expands to <MQTT_TOPIC>-<last 4 decimal chars of MAC address>

#define CONFIG_FILE_SIGN       0xA5         // Configuration file signature
#define CONFIG_FILE_XOR        0x5A         // Configuration file xor (0 = No Xor)

#define HLW_PREF_PULSE         12530        // was 4975us = 201Hz = 1000W
#define HLW_UREF_PULSE         1950         // was 1666us = 600Hz = 220V
#define HLW_IREF_PULSE         3500         // was 1666us = 600Hz = 4.545A

#define MQTT_RETRY_SECS        10           // Minimum seconds to retry MQTT connection
#define APP_POWER              0            // Default saved power state Off
#define WS2812_MAX_LEDS        512          // Max number of LEDs

#define PWM_RANGE              1023         // 255..1023 needs to be devisible by 256
//#define PWM_FREQ               1000         // 100..1000 Hz led refresh
//#define PWM_FREQ               910          // 100..1000 Hz led refresh (iTead value)
#define PWM_FREQ               880          // 100..1000 Hz led refresh (BN-SZ01 value)

#define MAX_POWER_HOLD         10           // Time in SECONDS to allow max agreed power (Pow)
#define MAX_POWER_WINDOW       30           // Time in SECONDS to disable allow max agreed power (Pow)
#define SAFE_POWER_HOLD        10           // Time in SECONDS to allow max unit safe power (Pow)
#define SAFE_POWER_WINDOW      30           // Time in MINUTES to disable allow max unit safe power (Pow)
#define MAX_POWER_RETRY        5            // Retry count allowing agreed power limit overflow (Pow)

#define STATES                 20           // State loops per second
#define SYSLOG_TIMER           600          // Seconds to restore syslog_level
#define SERIALLOG_TIMER        600          // Seconds to disable SerialLog
#define OTA_ATTEMPTS           10           // Number of times to try fetching the new firmware

#define INPUT_BUFFER_SIZE      250          // Max number of characters in (serial) command buffer
#define CMDSZ                  20           // Max number of characters in command
#define TOPSZ                  100          // Max number of characters in topic string
#ifdef USE_MQTT_TLS
  #define MAX_LOG_LINES        10           // Max number of lines in weblog
#else
  #define MAX_LOG_LINES        20           // Max number of lines in weblog
#endif
#define MAX_BACKLOG            16           // Max number of commands in backlog (chk blogidx and blogptr code)
#define MIN_BACKLOG_DELAY      2            // Minimal backlog delay in 0.1 seconds

#define APP_BAUDRATE           115200       // Default serial baudrate
#define MAX_STATUS             11           // Max number of status lines

/*********************************************************************************************\
 * Enumeration
\*********************************************************************************************/

enum week_t  {Last, First, Second, Third, Fourth};
enum dow_t   {Sun=1, Mon, Tue, Wed, Thu, Fri, Sat};
enum month_t {Jan=1, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec};
enum hemis_t {North, South};
enum log_t   {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE, LOG_LEVEL_ALL};  // SerialLog, Syslog, Weblog
enum wifi_t  {WIFI_RESTART, WIFI_SMARTCONFIG, WIFI_MANAGER, WIFI_WPSCONFIG, WIFI_RETRY, WIFI_WAIT, MAX_WIFI_OPTION};  // WifiConfig
enum swtch_t {TOGGLE, FOLLOW, FOLLOW_INV, PUSHBUTTON, PUSHBUTTON_INV, PUSHBUTTONHOLD, PUSHBUTTONHOLD_INV, MAX_SWITCH_OPTION};  // SwitchMode
enum led_t   {LED_OFF, LED_POWER, LED_MQTTSUB, LED_POWER_MQTTSUB, LED_MQTTPUB, LED_POWER_MQTTPUB, LED_MQTT, LED_POWER_MQTT, MAX_LED_OPTION};  // LedState
enum emul_t  {EMUL_NONE, EMUL_WEMO, EMUL_HUE, EMUL_MAX};  // Emulation

enum butt_t  {PRESSED, NOT_PRESSED};
enum opt_t   {P_HOLD_TIME, P_MAX_POWER_RETRY, P_MAX_PARAM8};   // Index in sysCfg.param
