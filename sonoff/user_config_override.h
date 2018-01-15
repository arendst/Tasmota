/*
  user_config_override.h - user configuration overrides user_config.h for Sonoff-Tasmota

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

#ifdef CFG_HOLDER
#undef CFG_HOLDER
#endif
#define CFG_HOLDER             0x20161210

#ifdef MODULE
#undef MODULE
#endif
#define MODULE                 SONOFF_B1


#ifdef STA_SSID1
#undef STA_SSID1
#endif
#define STA_SSID1              "Vanilla"

#ifdef STA_PASS1
#undef STA_PASS1
#endif
#define STA_PASS1              "VInternetLtd1"

#ifdef SERIAL_LOG_LEVEL
#undef SERIAL_LOG_LEVEL
#endif
#define SERIAL_LOG_LEVEL       LOG_LEVEL_NONE

#ifdef WEB_LOG_LEVEL
#undef WEB_LOG_LEVEL
#endif
#define WEB_LOG_LEVEL         LOG_LEVEL_NONE


#ifdef MQTT_PASS
#undef MQTT_PASS
#endif
#define MQTT_PASS            "tasmota123AbC"


#ifdef MQTT_HOST
#undef MQTT_HOST
#endif
#define MQTT_HOST            "10.0.0.100"

#ifdef MQTT_USER
#undef MQTT_USER
#endif
#define MQTT_USER            "tasmota"

#ifdef MQTT_PASS
#undef MQTT_PASS
#endif
#define MQTT_PASS            "tasmota123AbC"

#ifdef MQTT_TOPIC
#undef MQTT_TOPIC
#endif
#define MQTT_TOPIC            "bedroom/front"

#ifdef MQTT_FULLTOPIC
#undef MQTT_FULLTOPIC
#endif
#define MQTT_FULLTOPIC       "tasmota/%prefix%/b1/%topic%/"

#ifdef USE_DOMOTICZ
#undef USE_DOMOTICZ
#endif

#ifdef USE_DISCOVERY
#undef USE_DISCOVERY
#endif

#ifdef WEB_PASSWORD
#undef WEB_PASSWORD
#endif
#define WEB_PASSWORD         "vanilla"

#ifdef USE_EMULATION
#undef USE_EMULATION
#endif

#ifdef APP_LEDSTATE
#undef APP_LEDSTATE
#endif
#define APP_LEDSTATE         LED_OFF

#ifdef APP_TIMEZONE
#undef APP_TIMEZONE
#endif
#define APP_TIMEZONE         0

#ifdef USE_I2C
#undef USE_I2C
#endif

#ifdef USE_SPI
#undef USE_SPI
#endif

#ifdef USE_MHZ19
#undef USE_MHZ19
#endif

#ifdef USE_SENSEAIR
#undef USE_SENSEAIR
#endif

#ifdef USE_IR_REMOTE
#undef USE_IR_REMOTE
#endif

#ifdef USE_WS2812
#undef USE_WS2812
#endif

#ifdef USE_ARILUX_RF
#undef USE_ARILUX_RF
#endif






#endif  // _USER_CONFIG_OVERRIDE_H_
