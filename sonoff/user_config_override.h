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

// Examples
#ifdef CFG_HOLDER
#undef CFG_HOLDER
#endif
#define CFG_HOLDER             0x29261210


#ifdef STA_SSID1
#undef STA_SSID1
#endif
#define STA_SSID1              "WLAN-7490"

#ifdef STA_PASS1
#undef STA_PASS1
#endif
#define STA_PASS1              "71244940091830385955"

#ifdef MQTT_USER
#undef MQTT_USER
#endif
#define MQTT_USER            "gemu"       // [MqttUser] Optional user

#ifdef MQTT_PASS
#undef MQTT_PASS
#endif
#define MQTT_PASS            "Katerschatz2017"       // [MqttPassword] Optional password

#ifdef MQTT_HOST
#undef MQTT_HOST
#endif
#define MQTT_HOST            "192.168.178.47"


#define USE_SUNRISE
//#define USE_SGP30
//#undef USE_TIMERS                               // Add support for up to 16 timers (+2k2 code)
//#undef USE_TIMERS_WEB                         // Add timer webpage support (+4k5 code)
#undef USE_SPI
#undef USE_MHZ19
#undef USE_SENSEAIR
#undef USE_PMS5003
#undef USE_IR_REMOTE
#undef USE_IR_RECEIVE

/*
#undef USE_SPI
//#undef USE_I2C
#undef USE_SHT                                // Add I2C emulating code for SHT1X sensor (+1k4 code)
#undef USE_SHT3X                              // Add I2C code for SHT3x sensor (+0k6 code)
#undef USE_HTU                                // Add I2C code for HTU21/SI7013/SI7020/SI7021 sensor (+1k5 code)
//#undef USE_BMP                                // Add I2C code for BMP085/BMP180/BMP280/BME280 sensor (+4k code)
#undef USE_BME680                           // Add additional support for BME680 sensor using Adafruit Sensor and BME680 libraries (+6k code)
#undef USE_BH1750

#undef USE_SML                             // smart message language (smart meter)
#undef USE_EBUS                           // ebus wolf
#undef USE_SGP30                          // env sensor

#undef USE_TIMERS                               // Add support for up to 16 timers (+2k2 code)
#undef USE_TIMERS_WEB                         // Add timer webpage support (+4k5 code)


#undef USE_MHZ19
#undef USE_SENSEAIR
#undef USE_PMS5003
#undef USE_IR_REMOTE
#undef USE_IR_RECEIVE
#undef USE_WS2812
#undef USE_ARILUX_RF
#undef USE_NOVA_SDS
*/

#endif  // _USER_CONFIG_OVERRIDE_H_
