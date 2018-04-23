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

//#ifdef USE_KNX  // Enabling this will fail compilation. It has no impact if not used. (20180417)
#include <esp-knx-ip.h>
void KNX_CB_Action(message_t const &msg, void *arg);
//#endif  // USE_KNX

#define USE_DHT                               // Default DHT11 sensor needs no external library

#ifdef USE_ALL_SENSORS  // ===================== Configure sonoff-xxl.bin =========================
#define USE_ADC_VCC                           // Display Vcc in Power status. Disable for use as Analog input on selected devices
#define USE_DS18x20                           // For more than one DS18x20 sensors with id sort, single scan and read retry (+1k3 code)
//#define USE_DS18x20_LEGACY                     // For more than one DS18x20 sensors with dynamic scan using library OneWire (+1k5 code)
#define USE_I2C                               // I2C using library wire (+10k code, 0k2 mem, 124 iram)
#define USE_SHT                               // Add I2C emulating code for SHT1X sensor (+1k4 code)
#define USE_SHT3X                             // Add I2C code for SHT3x sensor (+0k6 code)
#define USE_HTU                               // Add I2C code for HTU21/SI7013/SI7020/SI7021 sensor (+1k5 code)
#define USE_BMP                               // Add I2C code for BMP085/BMP180/BMP280/BME280 sensor (+4k code)
  #define USE_BME680                          // Add additional support for BME680 sensor using Adafruit Sensor and BME680 libraries (+6k code)
#define USE_SGP30                             // Add I2C code for SGP30 sensor (+1k1 code)
#define USE_BH1750                            // Add I2C code for BH1750 sensor (+0k5 code)
#define USE_VEML6070                          // Add I2C code for VEML6070 sensor (+0k5 code)
#define USE_TSL2561                           // Add I2C code for TSL2561 sensor using library Adafruit TSL2561 Arduino (+1k2 code)
#define USE_ADS1115                           // Add I2C code for ADS1115 16 bit A/D converter based on Adafruit ADS1x15 library (no library needed) (+0k7 code)
//#define USE_ADS1115_I2CDEV                    // Add I2C code for ADS1115 16 bit A/D converter using library i2cdevlib-Core and i2cdevlib-ADS1115 (+2k code)
#define USE_INA219                            // Add I2C code for INA219 Low voltage and current sensor (+1k code)
#define USE_MGS                               // Add I2C code for Xadow and Grove Mutichannel Gas sensor using library Multichannel_Gas_Sensor (+10k code)
#define USE_MHZ19                             // Add support for MH-Z19 CO2 sensor (+2k code)
#define USE_SENSEAIR                          // Add support for SenseAir K30, K70 and S8 CO2 sensor (+2k3 code)
#ifndef CO2_LOW
  #define CO2_LOW              800            // Below this CO2 value show green light (needs PWM or WS2812 RG(B) led and enable with SetOption18 1)
#endif
#ifndef CO2_HIGH
  #define CO2_HIGH             1200           // Above this CO2 value show red light (needs PWM or WS2812 RG(B) led and enable with SetOption18 1)
#endif
#define USE_PMS5003                           // Add support for PMS5003 and PMS7003 particle concentration sensor (+1k3 code)
#define USE_NOVA_SDS                          // Add support for SDS011 and SDS021 particle concentration sensor (+0k7 code)
#define USE_PZEM004T                          // Add support for PZEM004T Energy monitor (+2k code)
#define USE_SERIAL_BRIDGE                     // Add support for software Serial Bridge (+0k8 code)
#define USE_IR_REMOTE                         // Send IR remote commands using library IRremoteESP8266 and ArduinoJson (+4k code, 0k3 mem, 48 iram)
  #define USE_IR_HVAC                         // Support for HVAC system using IR (+2k code)
  #define USE_IR_RECEIVE                      // Support for IR receiver (+5k5 code, 264 iram)
#define USE_WS2812                            // WS2812 Led string using library NeoPixelBus (+5k code, +1k mem, 232 iram) - Disable by //
#ifndef USE_WS2812_CTYPE
  #define USE_WS2812_CTYPE     NEO_GRB        // WS2812 Color type (NEO_RGB, NEO_GRB, NEO_BRG, NEO_RBG, NEO_RGBW, NEO_GRBW)
#endif
//  #define USE_WS2812_DMA                      // DMA supports only GPIO03 (= Serial RXD) (+1k mem). When USE_WS2812_DMA is enabled expect Exceptions on Pow
#define USE_ARILUX_RF                         // Add support for Arilux RF remote controller (+0k8 code, 252 iram (non 2.3.0))
#define USE_SR04                              // Add support for HC-SR04 ultrasonic devices (+1k code)
#endif  // USE_ALL_SENSORS =====================

#if defined(USE_DS18x20) || defined(USE_DS18x20_LEGACY)
#else
#define USE_DS18B20                           // Default DS18B20 sensor needs no external library
#endif

#ifdef BE_MINIMAL  // ========================== Configure sonoff-minimal.bin =====================
#ifdef USE_MQTT_TLS
#undef USE_MQTT_TLS                           // Disable TLS support won't work as the MQTTHost is not set
#endif
#ifdef USE_DISCOVERY
#undef USE_DISCOVERY                          // Disable Discovery services for both MQTT and web server
#endif
#ifdef USE_DOMOTICZ
#undef USE_DOMOTICZ                           // Disable Domoticz
#endif
#ifdef USE_HOME_ASSISTANT
#undef USE_HOME_ASSISTANT                     // Disable Home Assistant
#endif
#ifdef USE_KNX
#undef USE_KNX                                // Disable KNX IP Protocol Support
#endif
//#ifdef USE_WEBSERVER
//#undef USE_WEBSERVER                          // Disable Webserver
//#endif
#ifdef USE_EMULATION
#undef USE_EMULATION                          // Disable Wemo or Hue emulation
#endif
#ifdef USE_TIMERS
#undef USE_TIMERS                             // Disable support for up to 16 timers
#endif
#ifdef USE_SUNRISE
#undef USE_SUNRISE                            // Disable support for Sunrise and sunset tools
#endif
#ifdef USE_RULES
#undef USE_RULES                              // Disable support for rules
#endif
#ifdef USE_DHT
#undef USE_DHT                                // Disable internal DHT sensor
#endif
#ifdef USE_DS18x20
#undef USE_DS18x20                            // Disable DS18x20 sensor
#endif
#ifdef USE_DS18B20
#undef USE_DS18B20                            // Disable internal DS18B20 sensor
#endif
#ifdef USE_I2C
#undef USE_I2C                                // Disable all I2C sensors and devices
#endif
#ifdef USE_SPI
#undef USE_SPI                                // Disable all SPI devices
#endif
#ifdef USE_DISPLAY
#undef USE_DISPLAY                            // Disable Display support
#endif
#ifdef USE_MHZ19
#undef USE_MHZ19                              // Disable support for MH-Z19 CO2 sensor
#endif
#ifdef USE_SENSEAIR
#undef USE_SENSEAIR                           // Disable support for SenseAir K30, K70 and S8 CO2 sensor
#endif
#ifdef USE_PMS5003
#undef USE_PMS5003                            // Disable support for PMS5003 and PMS7003 particle concentration sensor
#endif
#ifdef USE_NOVA_SDS
#undef USE_NOVA_SDS                           // Disable support for SDS011 and SDS021 particle concentration sensor
#endif
#ifdef USE_PZEM004T
#undef USE_PZEM004T                           // Disable PZEM004T energy sensor
#endif
#ifdef USE_SERIAL_BRIDGE
#undef USE_SERIAL_BRIDGE                      // Disable support for software Serial Bridge
#endif
#ifdef USE_IR_REMOTE
#undef USE_IR_REMOTE                          // Disable IR driver
#endif
#ifdef USE_WS2812
#undef USE_WS2812                             // Disable WS2812 Led string
#endif
#ifdef USE_ARILUX_RF
#undef USE_ARILUX_RF                          // Disable support for Arilux RF remote controller
#endif
#ifdef USE_SR04
#undef USE_SR04                               // Disable support for for HC-SR04 ultrasonic devices
#endif
#ifdef DEBUG_THEO
#undef DEBUG_THEO                             // Disable debug code
#endif
#endif  // BE_MINIMAL ==========================

#ifndef SWITCH_MODE
#define SWITCH_MODE            TOGGLE         // TOGGLE, FOLLOW or FOLLOW_INV (the wall switch state)
#endif

#ifndef MQTT_FINGERPRINT1
#define MQTT_FINGERPRINT1      "A5 02 FF 13 99 9F 8B 39 8E F1 83 4F 11 23 65 0B 32 36 FC 07"
#endif

#ifndef MQTT_FINGERPRINT2
#define MQTT_FINGERPRINT2      "A5 02 FF 13 99 9F 8B 39 8E F1 83 4F 11 23 65 0B 32 36 FC 07"
#endif

#ifndef WS2812_LEDS
#define WS2812_LEDS            30             // [Pixels] Number of LEDs
#endif

#ifndef MQTT_MAX_PACKET_SIZE
#define MQTT_MAX_PACKET_SIZE   1000
#endif
#ifndef MQTT_KEEPALIVE
#define MQTT_KEEPALIVE         15
#endif

#ifndef MESSZ
//#define MESSZ                  405          // Max number of characters in JSON message string (6 x DS18x20 sensors)
//#define MESSZ                  893          // Max number of characters in JSON message string (Hass discovery and nice MQTT_MAX_PACKET_SIZE = 1000)
#define MESSZ                  (MQTT_MAX_PACKET_SIZE -TOPSZ -7)  // Max number of characters in JSON message string (6 x DS18x20 sensors)
#endif

//#include <core_version.h>                   // Arduino_Esp8266 version information (ARDUINO_ESP8266_RELEASE and ARDUINO_ESP8266_RELEASE_2_3_0)
#ifndef ARDUINO_ESP8266_RELEASE
#define ARDUINO_ESP8266_RELEASE "STAGE"
#endif

#endif  // _SONOFF_POST_H_
