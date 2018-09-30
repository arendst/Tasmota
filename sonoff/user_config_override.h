/*
lva configuration overrides  for Sonoff-Tasmota

переопределяем  user_config.h
build_flags =  -DUSE_CONFIG_OVERRIDE

-D_LVA_DEBUG

переопределяет:

#include <core_version.h>                   // Arduino_Esp8266 version information (ARDUINO_ESP8266_RELEASE and ARDUINO_ESP8266_RELEASE_2_3_0)
#include "sonoff_version.h"                 // Sonoff-Tasmota version information
#include "sonoff.h"                         // Enumeration used in user_config.h
#include "user_config.h"                    // Fixed user configurable options

*/

#ifndef _USER_CONFIG_OVERRIDE_H_
#define _USER_CONFIG_OVERRIDE_H_

/*********************************************************************************************\
 * This file consists of TWO sections.
 *
 * SECTION 1:
 * The first section contains PARAMETERS overriding flash settings if define CFG_HOLDER is CHANGED.
 * All parameters can be persistent changed online using commands via MQTT, WebConsole or Serial.
 *   Corresponding MQTT/Serial/Console commands are shown in [brackets]
 *
 * SECTION 2:
 * The second section contains Enabled and Disabled FEATURES allowing different program size.
 *   Changes in this section do NOT need a change of define CFG_HOLDER.
 *
 * ATTENTION:
 * Users are advised to use the user_config_override.h file for most changes.
\*********************************************************************************************/

//#define LOCATION_DACHA
//#define LOCATION_WORK
#define LOCATION_HOME

#ifdef LOCATION_DACHA
  #define SH_SERVER "192.168.10.5" // задаем сервер один раз, точнее надо будет еще в параметре OTA
#elif defined (LOCATION_HOME)
  #define SH_SERVER "192.168.0.1"
#else
  #define SH_SERVER "192.168.1.33"
#endif

#define LVA_POST
//#define KAMIN // Камин, Счетчик в топочной
#define ROOF // чердак
// #define  HOT
//#define SENSOR_SANOFF

//--------------------- sonoff_version.h
// переопределяем sonoff_version.h
#ifdef VERSION
#undef VERSION
#endif
#define VERSION 0x06020107 // подпись в web сервере

#ifdef D_PROGRAMNAME
#undef D_PROGRAMNAME
#define D_PROGRAMNAME "Sonoff-Tasmota+LVA" // подпись в web сервере
#endif

#ifdef D_AUTHOR
#undef D_AUTHOR
#define D_AUTHOR "lva" // подпись в web сервере
#endif

// ------------------------- sonoff.h
#undef SOFT_BAUDRATE
#define SOFT_BAUDRATE 19200 // Default software serial baudrate

// ------------------------ user_config.h
// -- Master parameter control --------------------
#undef CFG_HOLDER
#define CFG_HOLDER 4617

#undef PROJECT
#ifdef KAMIN
  #define PROJECT "Kamin" // здесь надо писать камин, чердак и прочее тк это префикс в mqtt
#elif defined(ROOF)
#define PROJECT "Roof"
#elif defined(HOT)
#define PROJECT "HotPump"
#elif defined(MERURIY)
#define PROJECT "Merkuriy"
#elif defined(SENSOR_SANOFF)
#define PROJECT "SENSOR_SANOFF"
#undef LVA_POST
#else
  #define PROJECT "Test"
#endif


#undef MODULE// [Module] Select default model from sonoff_template.h (Should not be changed)
#define MODULE WEMOS
#undef SAVE_STATE
#define SAVE_STATE 0 // [SetOption0] Save changed power state to Flash (0 = disable, 1 = enable)

#undef STA_SSID1
#undef STA_PASS1
#ifdef WORK
  #define STA_SSID1 "TP5-G-FL" // [Ssid1] Wifi SSID
  #define STA_PASS1 "bcc2813db03419d61cc968bd6b" // [Password1] Wifi password
#else
  #define STA_SSID1 "Point_00" // [Ssid2] Optional alternate AP Wifi SSID
  #define STA_PASS1 "27061970" // [Password2] Optional alternate AP Wifi password
#endif

#undef STA_SSID2
#define STA_SSID2 "Point_00" // [Ssid2] Optional alternate AP Wifi SSID

#undef STA_PASS2
#define STA_PASS2 "27061970" // [Password2] Optional alternate AP Wifi password

#undef SYS_LOG_HOST
#define SYS_LOG_HOST SH_SERVER // [LogHost] (Linux) syslog host

#undef OTA_URL
#define OTA_URL "http://192.168.10.5/ESP/firmware.bin"
//#define OTA_URL "http://"SH_SERVER"/sannoff-lva/firmware.bin"


#undef MQTT_HOST
#define MQTT_HOST SH_SERVER // [MqttHost]

#undef MQTT_USER
#define MQTT_USER "esp" // [MqttUser] Optional user

#undef MQTT_PASS
#define MQTT_PASS "esp" // [MqttPassword] Optional password

// на период отладки по умолчанию 300
#undef TELE_PERIOD
#define TELE_PERIOD 60 // [TelePeriod] Telemetry (0 = disable, 10 - 3600 seconds)

#undef FRIENDLY_NAME
#define FRIENDLY_NAME PROJECT // [FriendlyName] Friendlyname up to 32 characters used by webpages and Alexa

#undef NTP_SERVER1
#define NTP_SERVER1 SH_SERVER // [NtpServer1] Select first NTP server by name or IP address (129.250.35.250)
#undef NTP_SERVER2
#define NTP_SERVER2 "ntp5.stratum1.ru" // [NtpServer2] Select second NTP server by name or IP address (5.39.184.5)
#undef NTP_SERVER3
#define NTP_SERVER3 "ntp4.stratum1.ru" // [NtpServer3] Select third NTP server by name or IP address (93.94.224.67)

#undef LATITUDE
#define LATITUDE 55.887519 // [Latitude] Your location to be used with sunrise and sunset
#undef LONGITUDE
#define LONGITUDE 36.969137 // [Longitude] Your location to be used with sunrise and sunset

#undef TEMP_RESOLUTION
#define TEMP_RESOLUTION 2

#undef MQTT_LIBRARY_TYPE                   // Default MQTT driver for both non-TLS and TLS connections. Blocks network if MQTT server is unavailable.  Use PubSubClient library
#define MQTT_LIBRARY_TYPE MQTT_TASMOTAMQTT // Use TasmotaMqtt library (+4k4 code, +4k mem) - non-TLS only


#undef USE_DOMOTICZ // Enable Domoticz (+6k code, +0.3k mem)

// #undef USE_TIMERS   // Add support for up to 16 timers (+2k2 code)
// #undef USE_TIMERS_WEB // Add timer webpage support (+4k5 code)
// #undef USE_SUNRISE

#undef USE_RULES   // Add support for rules (+4k4 code)

#undef USE_ADC_VCC // Display Vcc in Power status. Disable for use as Analog input on selected devices


/* не все здесь можно выключить, по этому правим user_config.h
бесполезно выключать 
USE_I2C  

дальше остальное делаем в lva_post
*/
//#define BE_MINIMAL // включаем минимальную конфигурацию. webserver полный кастрат, нельзя использовать
#ifdef SENSOR_SANOFF
  #define USE_SENSORS
#else
  #define USE_CLASSIC // и уже своими определениями набираем нужные модули.
#endif // SENSOR_SANOFF

#endif  // _USER_CONFIG_OVERRIDE_H_

