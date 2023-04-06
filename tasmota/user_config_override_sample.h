/*
  user_config_override.h - user configuration overrides my_user_config.h for Tasmota

  Copyright (C) 2021  Theo Arends

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
 * USAGE:
 *   To modify the stock configuration without changing the my_user_config.h file:
 *   (1) copy this file to "user_config_override.h" (It will be ignored by Git)
 *   (2) define your own settings below
 *
 ******************************************************************************************************
 * ATTENTION:
 *   - Changes to SECTION1 PARAMETER defines will only override flash settings if you change define CFG_HOLDER.
 *   - Expect compiler warnings when no ifdef/undef/endif sequence is used.
 *   - You still need to update my_user_config.h for major define USE_MQTT_TLS.
 *   - All parameters can be persistent changed online using commands via MQTT, WebConsole or Serial.
\*****************************************************************************************************/

// -- Master parameter control --------------------
#undef  CFG_HOLDER
#define CFG_HOLDER        4618                   // [Reset 1] Change this value to load SECTION1 configuration parameters to flash

// -- Project -------------------------------------
#undef PROJECT
#define PROJECT           "ESP32-DEBIT-PAC"         	 // PROJECT is used as the default topic delimiter

#ifdef ESP32
	#define USER_TEMPLATE 		"{\"NAME\":\"ESP32_FLOWSENSOR_PAC\",\"GPIO\":[0,0,0,0,0,8992,0,0,224,225,226,0,0,0,0,0,0,0,0,0,0,0,0,8993,0,0,0,0,8995,8994,8997,8996,0,0,0,0],\"FLAG\":0,\"BASE\":1}"
#endif  // ESP32

// -- Setup your own Wifi settings  ---------------
// You might even pass some parameters from the command line ----------------------------
// Ie:  export PLATFORMIO_BUILD_FLAGS='-DUSE_CONFIG_OVERRIDE -DMY_IP="192.168.1.99" -DMY_GW="192.168.1.1" -DMY_DNS="192.168.1.1"'

// -- Syslog --------------------------------------

// -- Setup your own MQTT settings  ---------------
#undef  MQTT_HOST
#define MQTT_HOST         "192.168.0.5"          // [MqttHost]
#undef  MQTT_PORT
#define MQTT_PORT         1883                   // [MqttPort] MQTT port (10123 on CloudMQTT)
#undef  MQTT_USER
#define MQTT_USER         "fdaubercy"       	 // [MqttUser] MQTT user
#undef  MQTT_PASS
#define MQTT_PASS         "Lune5676"       		 // [MqttPassword] MQTT password
#undef  MQTT_TOPIC
#define MQTT_TOPIC        "PAC/debits"   		 // [Topic] unique MQTT device topic including (part of) device MAC address
#undef  FRIENDLY_NAME
#define FRIENDLY_NAME     "Débits PAC"           // [FriendlyName] Friendlyname up to 32 characters used by webpages and Alexa
#undef EMULATION
#define EMULATION         EMUL_HUE               // [Emulation] Select Belkin WeMo (single relay/light) or Hue Bridge emulation (multi relay/light) (EMUL_NONE, EMUL_WEMO or EMUL_HUE)

// -- mDNS ----------------------------------------
#undef MDNS_ENABLED
#define MDNS_ENABLED      true                   // [SetOption55] Use mDNS (false = Disable, true = Enable)

// -- Time - Up to three NTP servers in your region
#undef NTP_SERVER1
#define NTP_SERVER1      "pool.ntp.org"        // [NtpServer1] Select first NTP server by name or IP address (135.125.104.101, 2001:418:3ff::53)
#undef NTP_SERVER2
#define NTP_SERVER2      "europe.pool.ntp.org" // [NtpServer2] Select second NTP server by name or IP address (192.36.143.134, 2a00:2381:19c6::100)
#undef NTP_SERVER3
#define NTP_SERVER3      "nl.pool.ntp.org"     // [NtpServer3] Select third NTP server by name or IP address (46.249.42.13, 2603:c022:c003:c900::4)

// -- Location ------------------------------------
#undef LATITUDE
#define LATITUDE               50.4109163         // [Latitude] Your location to be used with sunrise and sunset
#undef LONGITUDE 
#define LONGITUDE              3.0858052          // [Longitude] Your location to be used with sunrise and sunset

// -- Application ---------------------------------
#undef APP_TIMEZONE
#define APP_TIMEZONE           99                 // [Timezone] +1 hour (Amsterdam) (-13 .. 14 = hours from UTC, 99 = use TIME_DST/TIME_STD)

/*********************************************************************************************\
 * END OF SECTION 1
 *
 * SECTION 2
 * - Enable a feature by removing both // in front of it
 * - Disable a feature by preceding it with //
\*********************************************************************************************/

// -- Localization --------------------------------
  // If non selected the default en-GB will be used
#define MY_LANGUAGE            fr_FR           // French in France

// -- ESP-NOW -------------------------------------
#define USE_TASMESH                              // Enable Tasmota Mesh using ESP-NOW (+11k code)

// -- Ping ----------------------------------------
  #define USE_PING                                 // Enable Ping command (+2k code)
  
// -- Rules or Script  ----------------------------
// Select none or only one of the below defines USE_RULES or USE_SCRIPT
#define USE_RULES                                // Add support for rules (+8k code)
  #define USE_EXPRESSION                         // Add support for expression evaluation in rules (+3k2 code, +64 bytes mem)
    #define SUPPORT_IF_STATEMENT                 // Add support for IF statement in rules (+4k2 code, -332 bytes mem)
  #define USER_RULE1 "<Any rule1 data>"          // Add rule1 data saved at initial firmware load or when command reset is executed
  #define USER_RULE2 "<Any rule2 data>"          // Add rule2 data saved at initial firmware load or when command reset is executed
  #define USER_RULE3 "<Any rule3 data>"          // Add rule3 data saved at initial firmware load or when command reset is executed

//#define USE_SCRIPT                               // Add support for script (+17k code)
  //#define USE_SCRIPT_FATFS 4                     // Script: Add FAT FileSystem Support
  #define SUPPORT_MQTT_EVENT                     // Support trigger event with MQTT subscriptions (+3k5 code)

#define USER_BACKLOG "<Any command separated by a semicolon (;)>"  // Add commands executed at firmware load or when command reset is executed

// -- Optional modules ----------------------------
#undef ROTARY_V1                                // Add support for Rotary Encoder as used in MI Desk Lamp (+0k8 code)
  #undef ROTARY_MAX_STEPS     //10                // Rotary step boundary
#undef USE_SONOFF_RF                            // Add support for Sonoff Rf Bridge (+3k2 code)
  #undef USE_RF_FLASH                           // Add support for flashing the EFM8BB1 chip on the Sonoff RF Bridge. C2CK must be connected to GPIO4, C2D to GPIO5 on the PCB (+2k7 code)
#undef USE_SONOFF_SC                            // Add support for Sonoff Sc (+1k1 code)
#undef USE_TUYA_MCU                             // Add support for Tuya Serial MCU
  #undef TUYA_DIMMER_ID       //0                 // Default dimmer Id
  #undef USE_TUYA_TIME                          // Add support for Set Time in Tuya MCU
#undef USE_TUYAMCUBR                            // Add support for TuyaMCU Bridge
#undef USE_SHUTTER                              // Add Shutter support for up to 4 shutter with different motortypes (+11k code)

// -- Optional light modules ----------------------
#undef USE_WS2812                               // WS2812 Led string using library NeoPixelBus (+5k code, +1k mem, 232 iram) - Disable by //
  #undef USE_WS2812_HARDWARE  //NEO_HW_WS2812     // Hardware type (NEO_HW_WS2812, NEO_HW_WS2812X, NEO_HW_WS2813, NEO_HW_SK6812, NEO_HW_LC8812, NEO_HW_APA106, NEO_HW_P9813)
  #undef  USE_WS2812_CTYPE     //NEO_GRB           // Color type (NEO_RGB, NEO_GRB, NEO_BRG, NEO_RBG, NEO_RGBW, NEO_GRBW)
  
// -- Internal Analog input -----------------------
#define USE_ADC_VCC                              // Display Vcc in Power status. Disable for use as Analog input on selected devices

// -- One wire sensors ----------------------------
#undef USE_DS18x20                              // Add support for DS18x20 sensors with id sort, single scan and read retry (+2k6 code)

// -- I2C sensors ---------------------------------
#undef USE_I2C                                  // I2C using library wire (+10k code, 0k2 mem, 124 iram)

// -- SPI sensors ---------------------------------
#undef USE_SPI                                  // Hardware SPI using GPIO12(MISO), GPIO13(MOSI) and GPIO14(CLK) in addition to two user selectable GPIOs(CS and DC)

// -- IR Remote features - subset of IR protocols --------------------------
#undef USE_IR_REMOTE                            // Send IR remote commands using library IRremoteESP8266 (+4k3 code, 0k3 mem, 48 iram)

  // Enable IR devoder via GPIO `IR Recv` - always enabled if `USE_IR_REMOTE_FULL`
  #undef USE_IR_RECEIVE                         // Support for IR receiver (+7k2 code, 264 iram)

// -- Other sensors/drivers -----------------------
#define USE_FLOWRATEMETER                        // Add support for water flow meter YF-DN50 and similary (+1k7 code)
#ifdef USE_FLOWRATEMETER
  #define FLOWRATEMETER1                        "Débitmètre d'Entrée - Cuisine"   
  #define FLOWRATEMETER2                        "Débitmètre de Salon - Salle à manger"
  #define FLOWRATEMETER3                        "Débitmètre de Salle de Bain"
  #define FLOWRATEMETER4                        "Débitmètre de Chambre Parentale"
  #define FLOWRATEMETER5                        "Débitmètre de Chambre Axel"
  #define FLOWRATEMETER6                        "Débitmètre de Chambre Camille"          

  #define D_JSON_FLOWRATEMETER                 "debitmetre" 
#endif

/*********************************************************************************************\
 * ESP32 only features
\*********************************************************************************************/
#define USE_SONOFF_SPM                           // Add support for ESP32 based Sonoff Smart Stackable Power Meter (+11k code)

#endif  // _USER_CONFIG_OVERRIDE_H_
