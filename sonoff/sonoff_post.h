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

/*********************************************************************************************\
 * Function declarations
\*********************************************************************************************/

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

/*********************************************************************************************\
 * Default global defines
\*********************************************************************************************/

#ifndef MODULE
#define MODULE                 SONOFF_BASIC   // [Module] Select default model
#endif

#define USE_DHT                               // Default DHT11 sensor needs no external library
#define USE_ENERGY_SENSOR                     // Use energy sensors

/*********************************************************************************************\
 * [sonoff-allsensors.bin]
 * Provide an image with all supported sensors enabled
\*********************************************************************************************/

#ifdef USE_ALL_SENSORS

#define USE_ADC_VCC                           // Display Vcc in Power status. Disable for use as Analog input on selected devices
#define USE_DS18x20                           // For more than one DS18x20 sensors with id sort, single scan and read retry (+1k3 code)
//#define USE_DS18x20_LEGACY                     // For more than one DS18x20 sensors with dynamic scan using library OneWire (+1k5 code)
#define USE_I2C                               // I2C using library wire (+10k code, 0k2 mem, 124 iram)
#define USE_SHT                               // Add I2C emulating code for SHT1X sensor (+1k4 code)
#define USE_SHT3X                             // Add I2C code for SHT3x sensor (+0k6 code)
#define USE_HTU                               // Add I2C code for HTU21/SI7013/SI7020/SI7021 sensor (+1k5 code)
#define USE_LM75AD                            // Add I2C code for LM75AD sensor (+0k5 code)
#define USE_BMP                               // Add I2C code for BMP085/BMP180/BMP280/BME280 sensor (+4k code)
  #define USE_BME680                          // Add additional support for BME680 sensor using Bosch BME680 library (+4k code)
#define USE_SGP30                             // Add I2C code for SGP30 sensor (+1k1 code)
#define USE_BH1750                            // Add I2C code for BH1750 sensor (+0k5 code)
#define USE_VEML6070                          // Add I2C code for VEML6070 sensor (+0k5 code)
#define USE_TSL2561                           // Add I2C code for TSL2561 sensor using library Adafruit TSL2561 Arduino (+1k2 code)
//#define USE_SI1145                            // Add I2C code for SI1145/46/47 sensor (+1k code)
#define USE_ADS1115                           // Add I2C code for ADS1115 16 bit A/D converter based on Adafruit ADS1x15 library (no library needed) (+0k7 code)
//#define USE_ADS1115_I2CDEV                    // Add I2C code for ADS1115 16 bit A/D converter using library i2cdevlib-Core and i2cdevlib-ADS1115 (+2k code)
#define USE_INA219                            // Add I2C code for INA219 Low voltage and current sensor (+1k code)
#define USE_MGS                               // Add I2C code for Xadow and Grove Mutichannel Gas sensor using library Multichannel_Gas_Sensor (+10k code)
//#define USE_APDS9960                          // Add I2C code for APDS9960 Proximity Sensor. Disables SHT and VEML6070 (+4k7 code)
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
#define USE_SDM120                            // Add support for Eastron SDM120-Modbus energy meter (+1k7 code)
#define USE_SDM630                            // Add support for Eastron SDM630-Modbus energy meter (+2k code)
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
#endif  // USE_ALL_SENSORS

/*********************************************************************************************\
 * [sonoff-classic.bin]
 * Provide an image close to version 5.12.0 but still within 499k program space to allow one time OTA
\*********************************************************************************************/

#ifdef USE_CLASSIC

#undef USE_KNX                                // Disable KNX IP Protocol Support
#undef USE_TIMERS                             // Disable support for up to 16 timers
#undef USE_TIMERS_WEB                         // Disable support for timer webpage
#undef USE_SUNRISE                            // Disable support for Sunrise and sunset tools
#undef USE_RULES                              // Disable support for rules
#undef USE_LM75AD                             // Disable sensor
#undef USE_BME680                             // Disable sensor
#undef USE_SGP30                              // Disable sensor
#undef USE_SENSEAIR                           // Disable support for SenseAir K30, K70 and S8 CO2 sensor
#undef USE_NOVA_SDS                           // Disable support for SDS011 and SDS021 particle concentration sensor
#undef USE_PZEM004T                           // Disable PZEM004T energy sensor
#undef USE_IR_RECEIVE                         // Disable support for IR receiver
#undef USE_SERIAL_BRIDGE                      // Disable support for software Serial Bridge
#undef USE_SDM120                             // Disable support for Eastron SDM120-Modbus energy meter
#undef USE_SDM630                             // Disable support for Eastron SDM630-Modbus energy meter
#undef USE_ARILUX_RF                          // Disable support for Arilux RF remote controller
#undef USE_SR04                               // Disable support for for HC-SR04 ultrasonic devices
#undef USE_TM1638                             // Disable support for TM1638 switches copying Switch1 .. Switch8
#undef USE_RF_FLASH                           // Disable support for flashing the EFM8BB1 chip on the Sonoff RF Bridge. C2CK must be connected to GPIO4, C2D to GPIO5 on the PCB
#undef DEBUG_THEO                             // Disable debug code
#undef USE_DEBUG_DRIVER                       // Disable debug code
#endif  // USE_CLASSIC

/*********************************************************************************************\
 * [sonoff-knx.bin]
 * Provide a dedicated KNX image allowing enough code and memory space
\*********************************************************************************************/

#ifdef USE_KNX_NO_EMULATION

#ifndef USE_KNX
#define USE_KNX                               // Enable KNX IP Protocol Support (+23k code, +3k3 mem)
#endif
#undef USE_EMULATION                          // Disable Belkin WeMo and Hue Bridge emulation for Alexa (-16k code, -2k mem)
#endif  // USE_KNX_NO_EMULATION

/*********************************************************************************************\
 * Mandatory define for DS18x20 if changed by above image selections
\*********************************************************************************************/

#if defined(USE_DS18x20) || defined(USE_DS18x20_LEGACY)
#else
#define USE_DS18B20                           // Default DS18B20 sensor needs no external library
#endif

/*********************************************************************************************\
 * [sonoff-minimal.bin]
 * Provide the smallest image possible while still enabling a webserver for intermediate image load
\*********************************************************************************************/

#ifdef BE_MINIMAL

#undef USE_ENERGY_SENSOR                      // Disable energy sensors
#undef USE_ARDUINO_OTA                        // Disable support for Arduino OTA
#undef USE_WPS                                // Disable support for WPS as initial wifi configuration tool
#undef USE_DOMOTICZ                           // Disable Domoticz
#undef USE_HOME_ASSISTANT                     // Disable Home Assistant
#undef USE_MQTT_TLS                           // Disable TLS support won't work as the MQTTHost is not set
#undef USE_KNX                                // Disable KNX IP Protocol Support
//#undef USE_WEBSERVER                          // Disable Webserver
#undef USE_DISCOVERY                          // Disable Discovery services for both MQTT and web server
#undef USE_EMULATION                          // Disable Wemo or Hue emulation
#undef USE_TIMERS                             // Disable support for up to 16 timers
#undef USE_TIMERS_WEB                         // Disable support for timer webpage
#undef USE_SUNRISE                            // Disable support for Sunrise and sunset tools
#undef USE_RULES                              // Disable support for rules
#undef USE_DHT                                // Disable internal DHT sensor
#undef USE_DS18x20                            // Disable DS18x20 sensor
#undef USE_DS18x20_LEGACY                     // Disable DS18x20 sensor
#undef USE_DS18B20                            // Disable internal DS18B20 sensor
#undef USE_I2C                                // Disable all I2C sensors and devices
#undef USE_SPI                                // Disable all SPI devices
#undef USE_DISPLAY                            // Disable Display support
#undef USE_MHZ19                              // Disable support for MH-Z19 CO2 sensor
#undef USE_SENSEAIR                           // Disable support for SenseAir K30, K70 and S8 CO2 sensor
#undef USE_PMS5003                            // Disable support for PMS5003 and PMS7003 particle concentration sensor
#undef USE_NOVA_SDS                           // Disable support for SDS011 and SDS021 particle concentration sensor
#undef USE_PZEM004T                           // Disable PZEM004T energy sensor
#undef USE_SERIAL_BRIDGE                      // Disable support for software Serial Bridge
#undef USE_SDM120                             // Disable support for Eastron SDM120-Modbus energy meter
#undef USE_SDM630                             // Disable support for Eastron SDM630-Modbus energy meter
#undef USE_IR_REMOTE                          // Disable IR driver
#undef USE_WS2812                             // Disable WS2812 Led string
#undef USE_ARILUX_RF                          // Disable support for Arilux RF remote controller
#undef USE_SR04                               // Disable support for for HC-SR04 ultrasonic devices
#undef USE_TM1638                             // Disable support for TM1638 switches copying Switch1 .. Switch8
#undef USE_RF_FLASH                           // Disable support for flashing the EFM8BB1 chip on the Sonoff RF Bridge. C2CK must be connected to GPIO4, C2D to GPIO5 on the PCB
#undef DEBUG_THEO                             // Disable debug code
#undef USE_DEBUG_DRIVER                       // Disable debug code
#endif  // BE_MINIMAL

/*********************************************************************************************\
 * Mandatory defines satisfying possible disabled defines
\*********************************************************************************************/

#ifndef USE_WPS                               // See https://github.com/esp8266/Arduino/pull/4889
#undef NO_EXTRA_4K_HEAP                       // Allocate 4k heap for WPS in ESP8166/Arduino core v2.4.2 (was always allocated in previous versions)
#endif

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
