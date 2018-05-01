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

// force the compiler to show a warning to confirm that this file is inlcuded
#warning **** user_config_override.h: Using Settings from this File ****

/*****************************************************************************************************\
 * USAGE:
 *   To modify the stock configuration without changing the user_config.h file:
 *   (1) copy this file to "user_config_override.h" (It will be ignored by Git)
 *   (2) define your own settings below
 *   (3) for platformio:
 *         define USE_CONFIG_OVERRIDE as a build flags.
 *         ie1 : export PLATFORMIO_BUILD_FLAGS='-DUSE_CONFIG_OVERRIDE'
 *         ie2 : enable in file platformio.ini ;build_flags = -Wl,-Tesp8266.flash.1m0.ld -DUSE_CONFIG_OVERRIDE
 *       for Arduino IDE:
 *         enable define USE_CONFIG_OVERRIDE in user_config.h
 ******************************************************************************************************
 * ATTENTION:
 *   - Changes to SECTION1 PARAMETER defines will only override flash settings if you change define CFG_HOLDER.
 *   - Expect compiler warnings when no ifdef/undef/endif sequence is used.
 *   - You still need to update user_config.h for major define USE_MQTT_TLS.
 *   - All parameters can be persistent changed online using commands via MQTT, WebConsole or Serial.
\*****************************************************************************************************/

/*
Examples :

// -- Master parameter control --------------------
#undef  CFG_HOLDER
#define CFG_HOLDER        0x20161209             // [Reset 1] Change this value to load SECTION1 configuration parameters to flash

// -- Setup your own Wifi settings  ---------------
#undef  STA_SSID1
#define STA_SSID1         "YourSSID"             // [Ssid1] Wifi SSID

#undef  STA_PASS1
#define STA_PASS1         "YourWifiPassword"     // [Password1] Wifi password

// -- Setup your own MQTT settings  ---------------
#undef  MQTT_HOST
#define MQTT_HOST         "your-mqtt-server.com" // [MqttHost]

#undef  MQTT_PORT
#define MQTT_PORT         1883                   // [MqttPort] MQTT port (10123 on CloudMQTT)

#undef  MQTT_USER
#define MQTT_USER         "YourMqttUser"         // [MqttUser] Optional user

#undef  MQTT_PASS
#define MQTT_PASS         "YourMqttPass"         // [MqttPassword] Optional password

// You might even pass some parameters from the command line ----------------------------
// Ie:  export PLATFORMIO_BUILD_FLAGS='-DUSE_CONFIG_OVERRIDE -DMY_IP="192.168.1.99" -DMY_GW="192.168.1.1" -DMY_DNS="192.168.1.1"'

#ifdef MY_IP
#undef  WIFI_IP_ADDRESS
#define WIFI_IP_ADDRESS   MY_IP                  // Set to 0.0.0.0 for using DHCP or IP address
#endif

#ifdef MY_GW
#undef  WIFI_GATEWAY
#define WIFI_GATEWAY      MY_GW                  // if not using DHCP set Gateway IP address
#endif

#ifdef MY_DNS
#undef  WIFI_DNS
#define WIFI_DNS          MY_DNS                 // If not using DHCP set DNS IP address (might be equal to WIFI_GATEWAY)
#endif

*/

#define DISPLAY_CONFIG

#ifdef DISPLAY_CONFIG  // ****************************************************************************

#ifdef USE_I2C
  #define USE_DISPLAY                            // Add I2C Display Support for LCD, Oled and up to eigth Matrices (+19k code)
    #define MTX_ADDRESS1       0x71              // [DisplayAddress[1]] I2C address of first 8x8 matrix module
    #define MTX_ADDRESS2       0x74              // [DisplayAddress[2]] I2C address of second 8x8 matrix module
    #define MTX_ADDRESS3       0x75              // [DisplayAddress[3]] I2C address of third 8x8 matrix module
    #define MTX_ADDRESS4       0x72              // [DisplayAddress[4]] I2C address of fourth 8x8 matrix module
    #define MTX_ADDRESS5       0x73              // [DisplayAddress[5]] I2C address of fifth 8x8 matrix module
    #define MTX_ADDRESS6       0x76              // [DisplayAddress[6]] I2C address of sixth 8x8 matrix module
    #define MTX_ADDRESS7       0x00              // [DisplayAddress[7]] I2C address of seventh 8x8 matrix module
    #define MTX_ADDRESS8       0x00              // [DisplayAddress[8]] I2C address of eigth 8x8 matrix module
#endif  // USE_I2C

// -- SPI sensors ---------------------------------
#define USE_SPI                                  // SPI using library TasmotaTFT
#ifdef USE_SPI
  #ifndef USE_DISPLAY
  #define USE_DISPLAY                            // Add SPI Display support for 320x240 and 480x320 TFT
  #endif
#endif  // USE_SPI

#endif  // DISPLAY_CONFIG ****************************************************************************

#endif  // _USER_CONFIG_OVERRIDE_H_