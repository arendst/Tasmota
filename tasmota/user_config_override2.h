/*
  user_config_override.h - user configuration overrides my_user_config.h for Tasmota

  Copyright (C) 2020  Theo Arends

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

// force the compiler to show a warning to confirm that this file is included
#warning **** user_config_override.h: Using Settings from this File ****

/*****************************************************************************************************\
 * USAGE:
 *   To modify the stock configuration without changing the my_user_config.h file:
 *   (1) copy this file to "user_config_override.h" (It will be ignored by Git)
 *   (2) define your own settings below
 *   (3) for platformio:
 *         All done.
 *       for Arduino IDE:
 *         enable define USE_CONFIG_OVERRIDE in my_user_config.h
 ******************************************************************************************************
 * ATTENTION:
 *   - Changes to SECTION1 PARAMETER defines will only override flash settings if you change define CFG_HOLDER.
 *   - Expect compiler warnings when no ifdef/undef/endif sequence is used.
 *   - You still need to update my_user_config.h for major define USE_MQTT_TLS.
 *   - All parameters can be persistent changed online using commands via MQTT, WebConsole or Serial.
\*****************************************************************************************************/

/*
Examples :

// -- Master parameter control --------------------
#undef  CFG_HOLDER
#define CFG_HOLDER        4617                   // [Reset 1] Change this value to load SECTION1 configuration parameters to flash

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
#define WIFI_IP_ADDRESS   MY_IP                  // Set to 0.0.0.0 for using DHCP or enter a static IP address
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

#define UPGRADE_V8_MIN                          // drop support for pre-v8.0

/*********************************************************************************************\
 * Undef all modules first, then enable one by one if needed
\*********************************************************************************************/

#undef USE_ARDUINO_OTA                           // Disable support for Arduino OTA
#undef USE_DOMOTICZ                              // Disable Domoticz
#undef USE_HOME_ASSISTANT                        // Disable Home Assistant
#undef USE_MQTT_TLS                              // Disable TLS support won't work as the MQTTHost is not set
#undef USE_KNX                                   // Disable KNX IP Protocol Support
#undef USE_WEBSERVER                             // Disable Webserver
#undef USE_WEBSEND_RESPONSE                      // Disable command WebSend response message (+1k code)
#undef USE_EMULATION                             // Disable Wemo or Hue emulation
#undef USE_EMULATION_HUE                         // Disable Hue Bridge emulation for Alexa (+14k code, +2k mem common)
#undef USE_EMULATION_WEMO                        // Disable Belkin WeMo emulation for Alexa (+6k code, +2k mem common)
#undef USE_CUSTOM                                // Disable Custom features
#undef USE_DISCOVERY                             // Disable Discovery services for both MQTT and web server
#undef USE_TIMERS                                // Disable support for up to 16 timers
#undef USE_TIMERS_WEB                            // Disable support for timer webpage
#undef USE_SUNRISE                               // Disable support for Sunrise and sunset tools
#undef USE_RULES                                 // Disable support for rules
#undef USE_SCRIPT                                  // Add support for script (+17k code)

// -- Optional modules -------------------------
#undef ROTARY_V1                                 // Disable support for MI Desk Lamp
#undef USE_SONOFF_RF                             // Disable support for Sonoff Rf Bridge (+3k2 code)
  #undef USE_RF_FLASH                            // Disable support for flashing the EFM8BB1 chip on the Sonoff RF Bridge. C2CK must be connected to GPIO4, C2D to GPIO5 on the PCB
#undef USE_SONOFF_SC                             // Disable support for Sonoff Sc (+1k1 code)
#undef USE_TUYA_MCU                              // Disable support for Tuya Serial MCU
#undef USE_ARMTRONIX_DIMMERS                     // Disable support for Armtronix Dimmers (+1k4 code)
#undef USE_PS_16_DZ                              // Disable support for PS-16-DZ Dimmer and Sonoff L1 (+2k code)
#undef USE_SONOFF_IFAN                           // Disable support for Sonoff iFan02 and iFan03 (+2k code)
#undef USE_BUZZER                                // Disable support for a buzzer (+0k6 code)
#undef USE_ARILUX_RF                             // Disable support for Arilux RF remote controller
#undef USE_SHUTTER                               // Disable Shutter support for up to 4 shutter with different motortypes (+6k code)
#undef USE_DEEPSLEEP                             // Disable support for deepsleep (+1k code)
#undef USE_EXS_DIMMER                            // Disable support for EX-Store WiFi Dimmer
#undef USE_HOTPLUG                               // Disable support for HotPlug
#undef USE_DEVICE_GROUPS                         // Disable support for device groups (+3k5 code)
#undef USE_PWM_DIMMER                            // Disable support for MJ-SD01/acenx/NTONPOWER PWM dimmers (+4k5 code)
#undef USE_PWM_DIMMER_REMOTE                     // Disbale support for remote switches to PWM Dimmer
#undef USE_KEELOQ                                // Disable support for Jarolift rollers by Keeloq algorithm (+4k5 code)
#undef USE_SONOFF_D1                             // Disable support for Sonoff D1 Dimmer (+0k7 code)
#undef USE_DGR_LIGHT_SEQUENCE                    // Disable support for device group light sequencing (requires USE_DEVICE_GROUPS) (+0k2 code)

// -- Optional light modules ----------------------
#undef USE_LIGHT                                 // Enable Dimmer/Light support
#undef USE_WS2812                                // Disable WS2812 Led string using library NeoPixelBus (+5k code, +1k mem, 232 iram) - Disable by //
#undef USE_MY92X1                                // Disable support for MY92X1 RGBCW led controller as used in Sonoff B1, Ailight and Lohas
#undef USE_SM16716                               // Disable support for SM16716 RGB LED controller (+0k7 code)
#undef USE_SM2135                                // Disable support for SM2135 RGBCW led control as used in Action LSC (+0k6 code)
#undef USE_SONOFF_L1                             // Disable support for Sonoff L1 led control
#undef USE_ELECTRIQ_MOODL                        // Disable support for ElectriQ iQ-wifiMOODL RGBW LED controller
#undef USE_LIGHT_PALETTE                         // Disable support for color palette (+0k9 code)
#undef USE_SHELLY_DIMMER                         // Disable support for Shelly Dimmer (+3k code)

#undef USE_COUNTER                               // Disable counters
#undef USE_ADC_VCC                              // Display Vcc in Power status. Disable for use as Analog input on selected devices
#undef USE_DS18x20                               // Disable DS18x20 sensor
#undef USE_I2C                                   // Enable I2C, zbbridge uses i2c EEprom 
#undef USE_SPI                                   // Disable all SPI devices
#undef USE_DISPLAY                               // Disable Display support
#undef USE_MHZ19                                 // Disable support for MH-Z19 CO2 sensor
#undef USE_SENSEAIR                              // Disable support for SenseAir K30, K70 and S8 CO2 sensor
#undef USE_PMS5003                               // Disable support for PMS5003 and PMS7003 particle concentration sensor
#undef USE_NOVA_SDS                              // Disable support for SDS011 and SDS021 particle concentration sensor
#undef USE_HPMA                                  // Disable support for Honeywell HPMA115S0 particle concentration sensor
#undef USE_SERIAL_BRIDGE                         // Disable support for software Serial Bridge
#undef USE_MP3_PLAYER                            // Disable DFPlayer Mini MP3 Player RB-DFR-562 commands: play, volume and stop
#undef USE_AZ7798                                // Disable support for AZ-Instrument 7798 CO2 datalogger
#undef USE_PN532_HSU                             // Disable support for PN532 using HSU (Serial) interface (+1k8 code, 140 bytes mem)
#undef USE_ZIGBEE                                // Disable serial communication with Zigbee CC2530 flashed with ZNP
#undef USE_RDM6300                               // Disable support for RDM6300 125kHz RFID Reader (+0k8)
#undef USE_IBEACON                               // Disable support for bluetooth LE passive scan of ibeacon devices (uses HM17 module)
#undef USE_GPS                                   // Disable support for GPS and NTP Server for becoming Stratus 1 Time Source (+ 3.1kb flash, +132 bytes RAM)
#undef USE_HM10                                  // (ESP8266 only) Disable support for HM-10 as a BLE-bridge for the LYWSD03 (+5k1 code)
#undef USE_MI_ESP32                              // (ESP32 only) Disable support for ESP32 as a BLE-bridge (+9k2 mem, +292k flash)
#undef USE_HRXL                                  // Disable support for MaxBotix HRXL-MaxSonar ultrasonic range finders (+0k7)
#undef USE_TASMOTA_CLIENT                        // Disable support for Arduino Uno/Pro Mini via serial interface including flashing (+2k3 code, 44 mem)
#undef USE_OPENTHERM                             // Disable support for OpenTherm (+15k code)

#undef USE_ENERGY_SENSOR                         // Disable energy sensors
#undef USE_ADE7953                               // Disable ADE7953 Energy monitor as used on Shelly 2.5 (I2C address 0x38) (+1k5)
#undef USE_PZEM004T                              // Disable PZEM004T energy sensor
#undef USE_PZEM_AC                               // Disable PZEM014,016 Energy monitor
#undef USE_PZEM_DC                               // Disable PZEM003,017 Energy monitor
#undef USE_MCP39F501                             // Disable MCP39F501 Energy monitor as used in Shelly 2
#undef USE_SDM120                                // Disable support for Eastron SDM120-Modbus energy meter
#undef USE_SDM630                                // Disable support for Eastron SDM630-Modbus energy monitor (+0k6 code)
#undef USE_DDS2382                               // Disable support for Hiking DDS2382 Modbus energy monitor (+0k6 code)
#undef USE_DDSU666                               // Disable support for Chint DDSU666 Modbus energy monitor (+0k6 code)
#undef USE_SOLAX_X1                              // Disable support for Solax X1 series Modbus log info (+3k1 code)
#undef USE_LE01MR                                // Disable support for F&F LE-01MR Modbus energy meter (+2k code)
#undef USE_TELEINFO                              // Disable support for French Energy Provider metering telemetry


#undef USE_DHT                                   // Disable support for DHT11, AM2301 (DHT21, DHT22, AM2302, AM2321) and SI7021 Temperature and Humidity sensor
#undef USE_MAX31855                              // Disable MAX31855 K-Type thermocouple sensor using softSPI
#undef USE_MAX31865                              // Disable support for MAX31865 RTD sensors using softSPI
#undef USE_IR_REMOTE                             // Disable IR driver
#undef USE_IR_RECEIVE

#undef USE_SR04                                  // Disable support for for HC-SR04 ultrasonic devices
#undef USE_TM1638                                // Disable support for TM1638 switches copying Switch1 .. Switch8
#undef USE_HX711                                 // Disable support for HX711 load cell
#undef USE_TX20_WIND_SENSOR                      // Disable support for La Crosse TX20 anemometer
#undef USE_TX23_WIND_SENSOR                      // Disable support for La Crosse TX23 anemometer
#undef USE_WINDMETER                             // Disable support for analog anemometer (+2k2 code)
#undef USE_RC_SWITCH                             // Disable support for RF transceiver using library RcSwitch
#undef USE_RF_SENSOR                             // Disable support for RF sensor receiver (434MHz or 868MHz) (+0k8 code)
#undef USE_HRE                                   // Disable support for Badger HR-E Water Meter (+1k4 code)
#undef USE_A4988_STEPPER                         // Disable support for A4988_Stepper
#undef USE_THERMOSTAT                            // Disable support for Thermostat
#undef USE_DYP                                   // Disable support for DYP ME-007 ultrasonic distance sensor, serial port version (+0k5 code)
#undef DEBUG_THEO                                // Disable debug code
#undef USE_DEBUG_DRIVER                          // Disable debug code
#undef USE_ENHANCED_GUI_WIFI_SCAN

/*********************************************************************************************\
 * Enable the standard modules common to all configurations
\*********************************************************************************************/

// -- Standard options ------------------------
#define USE_WEBSERVER                             // Webserver
#define USE_WEBSEND_RESPONSE                      // command WebSend response message (+1k code)
#define USE_EMULATION                             // Wemo or Hue emulation
#define USE_EMULATION_HUE                         // Hue Bridge emulation for Alexa (+14k code, +2k mem common)
#define USE_EMULATION_WEMO                        // Belkin WeMo emulation for Alexa (+6k code, +2k mem common)
#define USE_TIMERS                                // support for up to 16 timers
#define USE_TIMERS_WEB                            // support for timer webpage
#define USE_SUNRISE                               // support for Sunrise and sunset tools
#define USE_RULES                                 // support for rules

#define USE_ADC_VCC                              // Display Vcc in Power status. Disable for use as Analog input on selected devices

#define USE_LIGHT                                 // Enable Dimmer/Light support


// -- MQTT - TLS - AWS IoT ------------------------
// Using TLS starting with version v6.5.0.16 compilation will only work using Core 2.4.2 and 2.5.2. No longer supported: 2.3.0
// #define USE_MQTT_TLS                             // Use TLS for MQTT connection (+34.5k code, +7.0k mem and +4.8k additional during connection handshake)
  #define USE_MQTT_TLS_CA_CERT                   // Force full CA validation instead of fingerprints, slower, but simpler to use (+2.2k code, +1.9k mem during connection handshake)
  #define USE_MQTT_TLS_FORCE_EC_CIPHER           // Force Elliptic Curve cipher (higher security) required by some servers (automatically enabled with USE_MQTT_AWS_IOT) (+11.4k code, +0.4k mem)
  #define USE_MQTT_AWS_IOT_LIGHT                 // Enable MQTT for AWS IoT in light mode, with user/password instead of private certificate
  // #define USE_MQTT_AWS_IOT                       // Enable MQTT for AWS IoT - requires a private key (+11.9k code, +0.4k mem)
                                                 //   Note: you need to generate a private key + certificate per device and update 'sonoff/sonoff_aws_iot.cpp'
                                                 //   Full documentation here: https://github.com/arendst/Sonoff-Tasmota/wiki/AWS-IoT

// -- Rules or Script  ----------------------------
// Select none or only one of the below defines
// #undef USE_RULES                                // Add support for rules (+8k code)
// #define USE_SCRIPT                               // Add support for script (+17k code)
//   #define USE_SCRIPT_COMPRESSION
  //#define USE_SCRIPT_FATFS 4                     // Script: Add FAT FileSystem Support
//#define USE_RF_SENSOR                            // Add support for RF sensor receiver (434MHz or 868MHz) (+0k8 code)

//  #define USE_EXPRESSION                         // Add support for expression evaluation in rules (+3k2 code, +64 bytes mem)
//    #define SUPPORT_IF_STATEMENT                 // Add support for IF statement in rules (+4k2 code, -332 bytes mem)
//  #define SUPPORT_MQTT_EVENT                     // Support trigger event with MQTT subscriptions (+3k5 code)


// -- OLED Display  ----------------------------
// #define USE_I2C
  // #define USE_DISPLAY                            // Add I2C Display Support (+2k code)
    #define USE_DISPLAY_MODES1TO5                // Enable display mode 1 to 5 in addition to mode 0
    #undef USE_DISPLAY_LCD                      // [DisplayModel 1] Enable Lcd display (I2C addresses 0x27 and 0x3F) (+6k code)
    #define USE_DISPLAY_SSD1306                  // [DisplayModel 2] Enable SSD1306 Oled 128x64 display (I2C addresses 0x3C and 0x3D) (+16k code)
    #undef USE_DISPLAY_MATRIX                   // [DisplayModel 3] Enable 8x8 Matrix display (I2C adresseses see below) (+11k code)
//#define SHOW_SPLASH	//Enable initialization splash message on the display
//#define USE_AWATCH	//Enables analog watch support
// #define NUM_GRAPHS
// #define USE_GRAPH	//Enable line charts. Also requires NUM_GRAPHS

// -- PN 532 NFC  ----------------------------
//#define USE_PN532_HSU                            // Add support for PN532 using HSU (Serial) interface (+1k8 code, 140 bytes mem)
 #define USE_PN532_CAUSE_EVENTS                 // Cause event execution for PN532_UID= and PN532_DATA=[if defined] (+ 30 bytes code)
 #define USE_PN532_DATA_FUNCTION                // Add sensor40 command support for erase, setting data block content (+1k7 code, 388 bytes mem)
 #define USE_PN532_DATA_RAW                     // Allow DATA block to be used by non-alpha-numberic data (+ 80 bytes code, 48 bytes ram)

// -- Misc  ----------------------------
//#define USE_DEBUG_DRIVER
#undef PWM_MAX_SLEEP
#define PWM_MAX_SLEEP          25    // for LED salon


/*********************************************************************************************\
 * Switch amongs options
\*********************************************************************************************/
#undef _HADINGER_DONE

////
//
// Selectors
//
////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
#if !defined(_HADINGER_DEV) && !defined(_HADINGER_DENKY) && !defined(_HADINGER_M5STACK) && !defined(_HADINGER_IR_DEV) && !defined(_HADINGER_BULB) && !defined(_HADINGER_IR) && !defined(_HADINGER_ZNP) && !defined(_HADINGER_EZSP)
// #define _HADINGER_DEV       // development testing
// #define _HADINGER_DENKY     // Denky device
#define _HADINGER_32_BERRY   // Berry dev
// #define _HADINGER_M5STACK   // M5Stack device
// #define _HADINGER_IR_DEV       // development testing
// #define _HADINGER_BULB       // configuration for AWS IoT configured switches and bulbs, Hue emulation, should be 160 MHz
// #define _HADINGER_IR         // configuration for AWS IoT IR Blaster
// #define _HADINGER_ZNP        // configuration for CC2530 ZNP coordinator + AWS IoT + Ping
// #define _HADINGER_EZSP       // configuration for EZSP coordinator + AWS IoT + I2C + TCP Bridge
#endif
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _HADINGER_DEV

#define USE_MQTT_TLS                             // Use TLS for MQTT connection (+34.5k code, +7.0k mem and +4.8k additional during connection handshake)
// #define USE_IR_REMOTE

// #define USE_ENERGY_SENSOR
// #define USE_HLW8012
// #define USE_SONOFF_RF

#define USE_ENHANCED_GUI_WIFI_SCAN

#define USE_UFILESYS              // use file system
#define GUI_TRASH_FILE

// #define USE_BERRY
#define USE_ETHERNET
#define USE_TELEINFO

#define USE_WS2812
  #define USE_WS2812_DMA
  // #define USE_WS2812_INVERTED
  // #define USE_WS2812_HARDWARE  NEO_HW_WS2812X     // Hardware type (NEO_HW_WS2812, NEO_HW_WS2812X, NEO_HW_WS2813, NEO_HW_SK6812, NEO_HW_LC8812, NEO_HW_APA106, NEO_HW_P9813)
  // #define USE_WS2812_CTYPE     NEO_GRB           // Color type (NEO_RGB, NEO_GRB, NEO_BRG, NEO_RBG, NEO_RGBW, NEO_GRBW)


#define USE_I2C
#define USE_BMP
#define USE_MPU6886

// #undef USE_RULES                                 // Disable support for rules
// #define USE_SCRIPT                                  // Add support for script (+17k code)
// // #define USE_SCRIPT_FATFS 4                     // Script: Add FAT FileSystem Support

// #define SUPPORT_MQTT_EVENT                     // Support trigger event with MQTT subscriptions (+3k5 code)
// #define USE_MQTT_TLS                             // Use TLS for MQTT connection (+34.5k code, +7.0k mem and +4.8k additional during connection handshake)
// // #undef USE_MQTT_TLS_CA_CERT                   // Force full CA validation instead of fingerprints, slower, but simpler to use (+2.2k code, +1.9k mem during connection handshake)
// // #undef USE_MQTT_TLS_FORCE_EC_CIPHER           // Force Elliptic Curve cipher (higher security) required by some servers (automatically enabled with USE_MQTT_AWS_IOT) (+11.4k code, +0.4k mem)
// // #undef USE_MQTT_AWS_IOT_LIGHT

// // #define USE_TELEGRAM
// // #define USE_MQTT_TLS_CA_CERT
// #define USE_RC_SWITCH
// #define USE_THERMOSTAT
// #define USE_DOMOTICZ
// #define USE_HOME_ASSISTANT
// #define USE_I2C
// #define USE_DISPLAY
#define USE_PING

//  #define USE_EXPRESSION                         // Add support for expression evaluation in rules (+3k2 code, +64 bytes mem)
//    #define SUPPORT_IF_STATEMENT                 // Add support for IF statement in rules (+4k2 code, -332 bytes mem)
//  #define SUPPORT_MQTT_EVENT                     // Support trigger event with MQTT subscriptions (+3k5 code)

#define _HADINGER_DONE "_HADINGER_DEV"
#endif // 
//////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _HADINGER_DENKY

#define USE_ENHANCED_GUI_WIFI_SCAN

#define USE_UFILESYS              // use file system
#define GUI_TRASH_FILE

#define USE_BERRY
#define USE_I2C
// #define USE_ETHERNET
#define USE_TELEINFO
#define USE_ENERGY_SENSOR
#define USE_WS2812

#define SUPPORT_MQTT_EVENT                     // Support trigger event with MQTT subscriptions (+3k5 code)
#define USE_MQTT_TLS                             // Use TLS for MQTT connection (+34.5k code, +7.0k mem and +4.8k additional during connection handshake)
// #undef USE_MQTT_TLS_CA_CERT                   // Force full CA validation instead of fingerprints, slower, but simpler to use (+2.2k code, +1.9k mem during connection handshake)
// #undef USE_MQTT_TLS_FORCE_EC_CIPHER           // Force Elliptic Curve cipher (higher security) required by some servers (automatically enabled with USE_MQTT_AWS_IOT) (+11.4k code, +0.4k mem)
// #undef USE_MQTT_AWS_IOT_LIGHT

// #define USE_PING

#define _HADINGER_DONE "_HADINGER_DENKY"
#endif // 
//////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _HADINGER_32_BERRY

#define USE_ENHANCED_GUI_WIFI_SCAN

#define USE_UFILESYS              // use file system
#define GUI_TRASH_FILE

#define USE_BERRY
#define USE_I2C
#define USE_DHT12
#define USE_BMP
#define USE_MPU6886
// #define USE_SHT3X

// #define USE_ETHERNET
// #define USE_TELEINFO
#define USE_ENERGY_SENSOR
#define USE_WS2812
  // #define USE_WS2812_DMA
  #define USE_WS2812_RMT    0
  // #define USE_WS2812_I2S    0
  // #define USE_WS2812_INVERTED


#undef WS2812_LEDS
#define WS2812_LEDS            25                // [Pixels] Number of WS2812 LEDs to start with (max is 512)

#define USE_IR_REMOTE                            // Send IR remote commands using library IRremoteESP8266 and ArduinoJson (+4k code, 0k3 mem, 48 iram)
  // #define USE_IR_RECEIVE                         // Support for IR receiver (+5k5 code, 264 iram)

#define USE_LVGL

#define SUPPORT_MQTT_EVENT                     // Support trigger event with MQTT subscriptions (+3k5 code)
#define USE_MQTT_TLS                             // Use TLS for MQTT connection (+34.5k code, +7.0k mem and +4.8k additional during connection handshake)
// #undef USE_MQTT_TLS_CA_CERT                   // Force full CA validation instead of fingerprints, slower, but simpler to use (+2.2k code, +1.9k mem during connection handshake)
// #undef USE_MQTT_TLS_FORCE_EC_CIPHER           // Force Elliptic Curve cipher (higher security) required by some servers (automatically enabled with USE_MQTT_AWS_IOT) (+11.4k code, +0.4k mem)
// #undef USE_MQTT_AWS_IOT_LIGHT

// #define USE_PING

#define _HADINGER_DONE "_HADINGER_DENKY"
#endif // 
//////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _HADINGER_M5STACK

#define USE_ENHANCED_GUI_WIFI_SCAN

#define USE_UFILESYS              // use file system
#define GUI_TRASH_FILE

#define USE_BERRY
#define USE_ETHERNET
//#define USE_TELEINFO

//#define USE_WS2812

// #define USE_I2S_AUDIO
// #define USE_M5STACK_CORE2  
// #define USE_SCRIPT_FATFS 4  
// #define FAT_SCRIPT_SIZE 8192  
// #define USE_SCRIPT  
#define USE_I2C  
// #define USE_BMA423  
// #define SAY_TIME  
// #define USE_WEBRADIO  
// #define USE_I2S_WEBRADIO  
#define USE_DISPLAY  
#define USE_SPI  

#define USE_MLX90614

// #define USE_DISPLAY_ILI9341
#define USE_DISPLAY_ILI9342
// #define USE_M5STACK_CORE_BASIC
// #define USE_TOUCH_BUTTONS  
// #define JPEG_PICTS  
// #define USE_FT5206  
// #define USE_MPU6886  

#define SUPPORT_MQTT_EVENT                     // Support trigger event with MQTT subscriptions (+3k5 code)
#define USE_MQTT_TLS                             // Use TLS for MQTT connection (+34.5k code, +7.0k mem and +4.8k additional during connection handshake)
// #undef USE_MQTT_TLS_CA_CERT                   // Force full CA validation instead of fingerprints, slower, but simpler to use (+2.2k code, +1.9k mem during connection handshake)
// #undef USE_MQTT_TLS_FORCE_EC_CIPHER           // Force Elliptic Curve cipher (higher security) required by some servers (automatically enabled with USE_MQTT_AWS_IOT) (+11.4k code, +0.4k mem)
// #undef USE_MQTT_AWS_IOT_LIGHT

// #define USE_PING

#define _HADINGER_DONE "_HADINGER_DENKY"
#endif // 
//////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _HADINGER_IR_DEV

#define SUPPORT_MQTT_EVENT                     // Support trigger event with MQTT subscriptions (+3k5 code)
// #define USE_MQTT_TLS                             // Use TLS for MQTT connection (+34.5k code, +7.0k mem and +4.8k additional during connection handshake)
 
#define _HADINGER_DONE "_HADINGER_IR_DEV"
#endif // 


//////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _HADINGER_BULB

#define USE_MQTT_TLS                             // Use TLS for MQTT connection (+34.5k code, +7.0k mem and +4.8k additional during connection handshake)
#define USE_IR_REMOTE
#define USE_IR_RECEIVE

#define USE_ENERGY_SENSOR
#define USE_HLW8012
#define USE_SONOFF_RF

// #define USE_ENHANCED_GUI_WIFI_SCAN

#define _HADINGER_DONE "_HADINGER_BULB"
#endif // _HADINGER_BULB

//////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _HADINGER_IR

#define USE_MQTT_TLS                             // Use TLS for MQTT connection (+34.5k code, +7.0k mem and +4.8k additional during connection handshake)

#define _HADINGER_DONE "_HADINGER_IR"
#endif // _HADINGER_IR
//////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _HADINGER_ZNP

#define USE_MQTT_TLS                             // Use TLS for MQTT connection (+34.5k code, +7.0k mem and +4.8k additional during connection handshake)
#define USE_ZIGBEE                               // Enable serial communication with Zigbee CC2530 flashed with ZNP
#define USE_PING
#undef USE_LIGHT

// #undef USE_RULES
// #define USE_SCRIPT

#undef USE_LIGHT
#define USE_UFILESYS              // use file system
#define GUI_TRASH_FILE

#undef USE_EMULATION_WEMO                        // optimize and remove Wemo
#undef USE_LIGHT_VIRTUAL_CT                      // Disable support for Virtual White Color Temperature (SO106)

#define _HADINGER_DONE "_HADINGER_ZNP"
#endif // _HADINGER_ZNP
//////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _HADINGER_EZSP

#define USE_MQTT_TLS                             // Use TLS for MQTT connection (+34.5k code, +7.0k mem and +4.8k additional during connection handshake)
#define USE_ZIGBEE                               // Enable serial communication with Zigbee CC2530 flashed with ZNP
#undef USE_ZIGBEE_ZNP                          // Enable ZNP protocol, needed for CC2530 based devices
#define USE_ZIGBEE_EZSP                         // Enable EZSP protocol, needed for EFR32 EmberZNet based devices, like Sonoff Zigbee bridge
#define USE_TCP_BRIDGE
#define USE_I2C                                  // I2C using library wire (+10k code, 0k2 mem, 124 iram)

#undef USE_LIGHT
#define USE_UFILESYS              // use file system
#define GUI_TRASH_FILE

#undef USE_EMULATION_WEMO                        // optimize and remove Wemo
#undef USE_LIGHT_VIRTUAL_CT                      // Disable support for Virtual White Color Temperature (SO106)

#define _HADINGER_DONE "_HADINGER_EZSP"
#endif // _HADINGER_EZSP
//////////////////////////////////////////////////////////////////////////////////////////////



// Fallback for dev Version
#ifndef _HADINGER_DONE

#undef USE_ZIGBEE_ZNP                          // Enable ZNP protocol, needed for CC2530 based devices
#define USE_ZIGBEE_EZSP                         // Enable EZSP protocol, needed for EFR32 EmberZNet based devices, like Sonoff Zigbee bridge
#define USE_TCP_BRIDGE
#define USE_I2C                                  // I2C using library wire (+10k code, 0k2 mem, 124 iram)

#define _HADINGER_DONE "dev"
#endif
//////////////////////////////////////////////////////////////////////////////////////////////


// #define USE_DEVICE_GROUPS                        // Add support for device groups (+5k5 code)
// #define USE_I2C
// #define USE_PN532_HSU                            // Add support for PN532 using HSU (Serial) interface (+1k8 code, 140 bytes mem)
// #define USE_MQTT_TLS                             // Use TLS for MQTT connection (+34.5k code, +7.0k mem and +4.8k additional during connection handshake)
// #define USE_ZIGBEE                               // Enable serial communication with Zigbee CC2530 flashed with ZNP
// #define USE_PING
// #undef USE_RULES_COMPRESSION

// #define USE_FFT

#endif  // _USER_CONFIG_OVERRIDE_H_
