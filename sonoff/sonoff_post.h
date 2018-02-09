/*
  sonoff_post.h - Post header file for Sonoff-Tasmota

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

#ifndef _SONOFF_POST_H_
#define _SONOFF_POST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "user_interface.h"

// Function prototypes
void WifiWpsStatusCallback(wps_cb_status status);

#ifdef __cplusplus
}
#endif

#define USE_DHT                             // Default DHT11 sensor needs no external library

#if defined(USE_DS18x20) || defined(USE_DS18x20_LEGACY)
#else
#define USE_DS18B20                         // Default DS18B20 sensor needs no external library
#endif

//#define DEBUG_THEO                          // Add debug code

#ifdef BE_MINIMAL
#ifdef USE_MQTT_TLS
#undef USE_MQTT_TLS                         // Disable TLS support won't work as the MQTTHost is not set
#endif
#ifdef USE_DISCOVERY
#undef USE_DISCOVERY                        // Disable Discovery services for both MQTT and web server
#endif
#ifdef USE_DOMOTICZ
#undef USE_DOMOTICZ                         // Disable Domoticz
#endif
#ifdef USE_HOME_ASSISTANT
#undef USE_HOME_ASSISTANT                   // Disable Home Assistant
#endif
//#ifdef USE_WEBSERVER
//#undef USE_WEBSERVER                        // Disable Webserver
//#endif
#ifdef USE_EMULATION
#undef USE_EMULATION                        // Disable Wemo or Hue emulation
#endif
#ifdef USE_PZEM004T
#undef USE_PZEM004T                         // Disable PZEM004T energy sensor
#endif
#ifdef USE_DS18x20
#undef USE_DS18x20                          // Disable DS18x20 sensor
#endif
#ifdef USE_I2C
#undef USE_I2C                              // Disable all I2C sensors and devices
#endif
#ifdef USE_SPI
#undef USE_SPI                              // Disable all SPI devices
#endif
#ifdef USE_WS2812
#undef USE_WS2812                           // Disable WS2812 Led string
#endif
#ifdef USE_DS18B20
#undef USE_DS18B20                          // Disable internal DS18B20 sensor
#endif
#ifdef USE_DHT
#undef USE_DHT                              // Disable internal DHT sensor
#endif
#ifdef USE_DISPLAY
#undef USE_DISPLAY                          // Disable Display support
#endif
#ifdef USE_MHZ19
#undef USE_MHZ19                            // Disable support for MH-Z19 CO2 sensor
#endif
#ifdef USE_SENSEAIR
#undef USE_SENSEAIR                         // Disable support for SenseAir K30, K70 and S8 CO2 sensor
#endif
#ifdef USE_IR_REMOTE
#undef USE_IR_REMOTE                        // Disable IR driver
#endif
#ifdef USE_ARILUX_RF
#undef USE_ARILUX_RF                        // Disable support for Arilux RF remote controller
#endif
#ifdef DEBUG_THEO
#undef DEBUG_THEO                           // Disable debug code
#endif
#endif  // BE_MINIMAL

#ifndef SWITCH_MODE
#define SWITCH_MODE            TOGGLE       // TOGGLE, FOLLOW or FOLLOW_INV (the wall switch state)
#endif

#ifndef MQTT_FINGERPRINT
#define MQTT_FINGERPRINT       "A5 02 FF 13 99 9F 8B 39 8E F1 83 4F 11 23 65 0B 32 36 FC 07"
#endif

#ifndef WS2812_LEDS
#define WS2812_LEDS            30           // [Pixels] Number of LEDs
#endif

#ifndef MESSZ
//#define MESSZ                  405          // Max number of characters in JSON message string (6 x DS18x20 sensors)
//#define MESSZ                  893          // Max number of characters in JSON message string (Hass discovery and nice MQTT_MAX_PACKET_SIZE = 1000)
#define MESSZ                  (MQTT_MAX_PACKET_SIZE -TOPSZ -7)  // Max number of characters in JSON message string (6 x DS18x20 sensors)
#endif

//#include <core_version.h>                   // Arduino_Esp8266 version information (ARDUINO_ESP8266_RELEASE and ARDUINO_ESP8266_RELEASE_2_3_0)
#ifndef ARDUINO_ESP8266_RELEASE
#define ARDUINO_ESP8266_RELEASE "STAGED"
#endif

#endif  // _SONOFF_POST_H_