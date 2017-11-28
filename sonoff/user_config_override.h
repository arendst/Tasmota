/*
  user_config_override.h - user configuration overrides user_config.h for Sonoff-Tasmota

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

#ifndef _USER_CONFIG_OVERRIDE_H_
#define _USER_CONFIG_OVERRIDE_H_

/*****************************************************************************************************\
 * ATTENTION: - Changes to most PARAMETER defines will only override flash settings if you change
 *              define CFG_HOLDER.
 *            - Expect compiler warnings when no ifdef/undef/endif sequence is used.
 *            - You still need to update user_config.h for major defines MODULE and USE_MQTT_TLS.
 *            - Changing MODULE defines are not being tested for validity as they are in user_config.h.
 *            - Most parameters can be changed online using commands via MQTT, WebConsole or serial.
 *            - So I see no use in this but anyway, your on your own.
\*****************************************************************************************************/

// Examples
//#ifdef CFG_HOLDER
//#undef CFG_HOLDER
//#endif
//#define CFG_HOLDER             0x20161210

//#ifdef STA_SSID1
//#undef STA_SSID1
//#endif
//#define STA_SSID1              "yourssid1"
#ifdef CFG_HOLDER
#undef CFG_HOLDER
#endif
#define CFG_HOLDER             0x20161213       // [Reset 1] Change this value to load following default configuration parameters
// -- Wifi ----------------------------------------
#ifdef STA_SSID1
#undef STA_SSID1

#endif
#define STA_SSID1              "BodeNet_Keller"  // [Ssid1] Wifi SSID
#ifdef STA_PASS1
#undef STA_PASS1
#endif
#define STA_PASS1              "BettyUndStefa"          // [Password1] Wifi password
#ifdef STA_SSID2
#undef STA_SSID2
#endif
#define STA_SSID2              "SchreiberWLAN"  // [Ssid1] Wifi SSID
#ifdef STA_PASS2
#undef STA_PASS2
#endif
#define STA_PASS2              "Me1nGast"          // [Password1] Wifi password
#ifdef WIFI_CONFIG_TOOL
#undef WIFI_CONFIG_TOOL
#endif
#define WIFI_CONFIG_TOOL       WIFI_MANAGER      // [WifiConfig] Default tool if wifi fails to connect

// -- Syslog --------------------------------------
#ifdef SYS_LOG_HOST
#undef SYS_LOG_HOST
#endif
#define SYS_LOG_HOST           "192.168.178.42"          // [LogHost] (Linux) syslog host

#ifdef BE_MINIMAL
#define MQTT_USE               0
#endif

#ifndef BE_MINIMAL
#define USE_MQTT_TLS                             // EXPERIMENTAL Use TLS for MQTT connection (+53k code, +20k mem) - Disable by //
#ifdef MQTT_USE
#undef MQTT_USE
#endif
#define MQTT_USE               1

#ifdef MQTT_HOST
#undef MQTT_HOST
#endif
#define MQTT_HOST            "elocm.homeserver.com"    // [MqttHost]

#ifdef MQTT_FINGERPRINT
#undef MQTT_FINGERPRINT
#endif
#define MQTT_FINGERPRINT     "EF:2A:0F:3E:87:95:6E:F0:4B:A6:F4:38:73:29:1A:14:7E:A9:A1:B7"  // [MqttFingerprint]

#ifdef MQTT_PORT
#undef MQTT_PORT
#endif
#define MQTT_PORT            8883                // [MqttPort] MQTT TLS port

#ifdef MQTT_USER
#undef MQTT_USER
#endif
#define MQTT_USER            "sonoff"         // [MqttUser] Mandatory user

#ifdef MQTT_PASS
#undef MQTT_PASS
#endif
#define MQTT_PASS            "c1q6pGUyc0Zk"            // [MqttPassword] Mandatory password


#undef MQTT_BUTTON_RETAIN
#undef MQTT_POWER_RETAIN
#undef MQTT_SWITCH_RETAIN
#undef MQTT_TOPIC_RETAIN

#define MQTT_BUTTON_RETAIN     1                 // [ButtonRetain] Button may send retain flag (0 = off, 1 = on)
#define MQTT_POWER_RETAIN      1                 // [PowerRetain] Power status message may send retain flag (0 = off, 1 = on)
#define MQTT_SWITCH_RETAIN     1                 // [SwitchRetain] Switch may send retain flag (0 = off, 1 = on)
#define MQTT_TOPIC_RETAIN      1                 // [TopicRetain] Topics may send retain flag (0 = off, 1 = on)

#undef SUB_PREFIX
#undef PUB_PREFIX
#undef PUB_PREFIX2
#undef USE_ADC_VCC

#define SUB_PREFIX             "hm/setting"           // Sonoff devices subscribe to:- SUB_PREFIX/MQTT_TOPIC and SUB_PREFIX/MQTT_GRPTOPIC
#define PUB_PREFIX             "hm/status"            // Sonoff devices publish to:- PUB_PREFIX/MQTT_TOPIC
#define PUB_PREFIX2            "hm/status"            // Sonoff devices publish telemetry data to:- PUB_PREFIX2/MQTT_TOPIC/UPTIME, POWER/LIGHT and TIME
#endif // BE_MINIMAL


//#ifdef USE_DOMOTICZ
//#undef USE_DOMOTICZ                             // Enable Domoticz (+7k code, +0.3k mem) - Disable by //
//#endif

#ifdef USE_DISCOVERY
#undef USE_DISCOVERY                            // Enable mDNS for the following services (+8k code, +0.3k mem) - Disable by //
#endif

#ifdef WEBSERVER_ADVERTISE
#undef WEBSERVER_ADVERTISE                    // Provide access to webserver by name <Hostname>.local/
#endif

#define USE_DS18x20                              // Optional using OneWire library for multiple DS18B20 and/or DS18S20 (+2k code)

//#undef USE_I2C                                  // I2C using library wire (+10k code, 0.2k mem) - Disable by //
#undef USE_BH1750                             // Add I2C code for BH1750 sensor
#define USE_BMP                                // Add I2C code for BMP/BME280 sensor
#undef USE_HTU                                // Add I2C code for HTU21/SI7013/SI7020/SI7021 sensor
#undef USE_IR_REMOTE                            // Send IR remote commands using library IRremoteESP8266 and ArduinoJson (+3k code, 0.3k mem)
#undef USE_IR_RECEIVE
#define USE_WS2812
#undef USE_SHT
#define USE_ADS1115
//#undef USE_CHIRP
#define USE_PCF8574



// -- Application ---------------------------------
#undef APP_TIMEZONE
#define APP_TIMEZONE           99                 // [Timezone] +1 hour (Amsterdam) (-12 .. 12 = hours from UTC, 99 = use TIME_DST/TIME_STD)





#endif  // _USER_CONFIG_OVERRIDE_H_
