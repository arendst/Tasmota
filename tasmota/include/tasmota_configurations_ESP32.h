/*
  tasmota_configurations_ESP32.h - ESP32 only Configurations for Tasmota

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

#ifndef _TASMOTA_CONFIGURATIONS_ESP32_H_
#define _TASMOTA_CONFIGURATIONS_ESP32_H_

#ifdef ESP32
#include "sdkconfig.h"

/*********************************************************************************************\
 * [tasmota32x-safeboot.bin]
 * Provide an image with useful supported sensors enabled
 *
 * Is a copy of FIRMWARE_MINIMAL with some additional features enabled
\*********************************************************************************************/

#ifdef FIRMWARE_SAFEBOOT

#ifndef CODE_IMAGE_STR
  #define CODE_IMAGE_STR "safeboot"
#endif

#undef FIRMWARE_LITE                            // Disable tasmota-lite with no sensors
#undef FIRMWARE_SENSORS                         // Disable tasmota-sensors with useful sensors enabled
#undef FIRMWARE_KNX_NO_EMULATION                // Disable tasmota-knx with KNX but without Emulation
#undef FIRMWARE_DISPLAYS                        // Disable tasmota-display with display drivers enabled
#undef FIRMWARE_IR                              // Disable tasmota-ir with IR full protocols activated
#undef FIRMWARE_WEBCAM
#undef FIRMWARE_BLUETOOTH
#undef FIRMWARE_LVGL
#undef FIRMWARE_TASMOTA32

/*-------------------------------------------------------------------------------------------*\
 * Start copy of all undefines from FIRMWARE_MINIMAL
\*-------------------------------------------------------------------------------------------*/

#undef USE_IMPROV                                // Disable support for IMPROV serial protocol as used by esp-web-tools (+2k code)
#undef USE_TASMESH                               // Disable Tasmota Mesh using ESP-NOW (+11k code)
#undef USE_ARDUINO_OTA                           // Disable support for Arduino OTA
#undef USE_INFLUXDB                              // Disable influxdb support (+5k code)
#undef USE_DOMOTICZ                              // Disable Domoticz
#undef USE_HOME_ASSISTANT                        // Disable Home Assistant
#undef USE_TASMOTA_DISCOVERY                     // Disable Tasmota Discovery support (+2k code)
#undef USE_TELEGRAM                              // Disable support for Telegram protocol (+49k code, +7.0k mem and +4.8k additional during connection handshake)
//#undef USE_MQTT_TLS                              // Disable TLS support won't work as the MQTTHost is not set
#undef USE_KNX                                   // Disable KNX IP Protocol Support
#undef USE_DALI                                  // Disable support for DALI gateway (+5k code)
//#undef USE_WEBSERVER                             // Disable Webserver
#undef USE_GPIO_VIEWER                           // Enable GPIO Viewer to see realtime GPIO states (+5k6 code)
#undef USE_ENHANCED_GUI_WIFI_SCAN                // Disable wifi scan output with BSSID (+0k5 code)
#undef USE_WEBSEND_RESPONSE                      // Disable command WebSend response message (+1k code)
#undef USE_EMULATION                             // Disable Wemo or Hue emulation
#undef USE_EMULATION_HUE                         // Disable Hue Bridge emulation for Alexa (+14k code, +2k mem common)
#undef USE_EMULATION_WEMO                        // Disable Belkin WeMo emulation for Alexa (+6k code, +2k mem common)
#undef USE_CUSTOM                                // Disable Custom features
#undef USE_DISCOVERY                             // Disable Discovery services for both MQTT and web server
#undef USE_TIMERS                                // Disable support for up to 16 timers
#undef USE_TIMERS_WEB                            // Disable support for timer webpage
#undef USE_SUNRISE                               // Disable support for Sunrise and sunset tools
#undef USE_PING                                  // Disable Ping command (+2k code)
#undef USE_UNISHOX_COMPRESSION                   // Disable support for string compression in Rules or Scripts
#undef USE_RULES                                 // Disable support for rules
#undef USE_SCRIPT                                // Disable support for script

// -- Optional modules ----------------------------
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
#undef USE_SHELLY_DIMMER                         // Disable support for Shelly Dimmer (+3k code)

// -- Optional light modules ----------------------
#undef USE_LIGHT                                 // Disable support for lights

#undef USE_COUNTER                               // Disable counters
#define USE_ADC_VCC                              // Display Vcc in Power status. Disable for use as Analog input on selected devices
#undef USE_DS18x20                               // Disable DS18x20 sensor

#undef USE_I2C                                   // Disable all I2C sensors and devices
#undef USE_SPI                                   // Disable all SPI devices
#undef USE_DISPLAY                               // Disable Display support

#undef USE_MHZ19                                 // Disable support for MH-Z19 CO2 sensor
#undef USE_SENSEAIR                              // Disable support for SenseAir K30, K70 and S8 CO2 sensor
#undef USE_PMS5003                               // Disable support for PMS5003 and PMS7003 particle concentration sensor
#undef USE_NOVA_SDS                              // Disable support for SDS011 and SDS021 particle concentration sensor
#undef USE_HPMA                                  // Disable support for Honeywell HPMA115S0 particle concentration sensor
#undef USE_SR04                                  // Disable support for HC-SR04 ultrasonic devices (+1k code)
#undef USE_ME007                                 // Disable support for ME007 ultrasonic devices (+1k5 code)
#undef USE_DYP                                   // Disable support for DYP ME-007 ultrasonic distance sensor, serial port version (+0k5 code)
#undef USE_SERIAL_BRIDGE                         // Disable support for software Serial Bridge
#undef USE_MODBUS_BRIDGE                         // Disable support for software Modbus Bridge (+3k code)
#undef USE_TCP_BRIDGE                            // DIsable support for Serial to TCP bridge (+1.3k code)
#undef USE_MP3_PLAYER                            // Disable DFPlayer Mini MP3 Player RB-DFR-562 commands: play, volume and stop
#undef USE_AZ7798                                // Disable support for AZ-Instrument 7798 CO2 datalogger
#undef USE_PN532_HSU                             // Disable support for PN532 using HSU (Serial) interface (+1k8 code, 140 bytes mem)
#undef USE_ZIGBEE                                // Disable serial communication with Zigbee CC2530 flashed with ZNP
#undef USE_RDM6300                               // Disable support for RDM6300 125kHz RFID Reader (+0k8)
#undef USE_IBEACON                               // Disable support for bluetooth LE passive scan of ibeacon devices (uses HM17 module)
#undef USE_GPS                                   // Disable support for GPS and NTP Server for becoming Stratus 1 Time Source (+ 3.1kb flash, +132 bytes RAM)
#undef USE_HM10                                  // (ESP8266 only) Disable support for HM-10 as a BLE-bridge for the LYWSD03 (+5k1 code)
#undef USE_BLE_ESP32                             // (ESP32 only) Disable support for native BLE on ESP32 - use new driver
#undef USE_MI_ESP32                              // (ESP32 only) Disable support for ESP32 as a BLE-bridge (+9k2 mem, +292k flash)
#undef USE_HRXL                                  // Disable support for MaxBotix HRXL-MaxSonar ultrasonic range finders (+0k7)
#undef USE_TASMOTA_CLIENT                        // Disable support for Arduino Uno/Pro Mini via serial interface including flashing (+2k3 code, 44 mem)
#undef USE_OPENTHERM                             // Disable support for OpenTherm (+15k code)
#undef USE_MIEL_HVAC                             // Disable support for Mitsubishi Electric HVAC serial interface (+5k code)
#undef USE_PROJECTOR_CTRL                        // Disable support for LCD/DLP Projector serial control interface
#undef USE_AS608                                 // Disable support for AS608 optical and R503 capacitive fingerprint sensor (+3k4 code)
#undef USE_TFMINIPLUS                            // Disable support for TFmini Plus (TFmini, TFmini-S) LiDAR modules via UART interface (+0k8)
#undef USE_HRG15                                 // Disable support for Hydreon RG-15 Solid State Rain sensor (+1k5 code)
#undef USE_VINDRIKTNING                          // Disable support for IKEA VINDRIKTNING particle concentration sensor (+0k6 code)
#undef USE_LOX_O2                                // Disable support for LuminOx LOX O2 Sensor

#undef USE_ENERGY_SENSOR                         // Disable energy sensors

#undef USE_DHT                                   // Disable support for DHT11, AM2301 (DHT21, DHT22, AM2302, AM2321) and SI7021 Temperature and Humidity sensor
#undef USE_MAX31855                              // Disable MAX31855 K-Type thermocouple sensor using softSPI
#undef USE_MAX31865                              // Disable support for MAX31865 RTD sensors using softSPI
#undef USE_LMT01                                 // Disable support for TI LMT01 temperature sensor, count pulses on single GPIO (+0k5 code)
#undef USE_IR_REMOTE                             // Disable IR driver
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
#undef USE_PROMETHEUS                            // Disable support for https://prometheus.io/ metrics exporting over HTTP /metrics endpoint
#undef DEBUG_THEO                                // Disable debug code
#undef USE_DEBUG_DRIVER                          // Disable debug code
#undef USE_AC_ZERO_CROSS_DIMMER                  // Disable support for AC_ZERO_CROSS_DIMMER

/*-------------------------------------------------------------------------------------------*\
 * End copy of all undefines from FIRMWARE_MINIMAL
 *
 * Start FIRMWARE_SAFEBOOT specific additions
\*-------------------------------------------------------------------------------------------*/

#define FIRMWARE_MINIMAL

#undef USE_ESP32_SENSORS
#undef USE_UFILESYS
#undef GUI_TRASH_FILE
#undef GUI_EDIT_FILE
#undef USE_PING
#undef USE_AUTOCONF
#undef USE_BERRY

#define USE_TLS
#define USE_WEBSERVER
#define USE_WEBCLIENT
#define USE_WEBCLIENT_HTTPS

#undef USE_ESP32_WDT                                  // disable watchdog on SAFEBOOT until more testing is done

#if CONFIG_FREERTOS_UNICORE || CONFIG_IDF_TARGET_ESP32S3
#if CONFIG_ETH_ENABLED                               // Check for Ethernet support in Arduino libs
//  #undef USE_MQTT_TLS
//  #define USE_SERIAL_BRIDGE                        // Add support for software Serial Bridge console Tee (+4.5k code)
  #define USE_SPI                                    // Make SPI Ethernet adapters useable (+124 bytes)
  #define USE_ETHERNET
#endif  // CONFIG_ETH_ENABLED
#endif  // CONFIG_FREERTOS_UNICORE || CONFIG_IDF_TARGET_ESP32S3

#endif  // FIRMWARE_SAFEBOOT

/*********************************************************************************************\
 * [tasmota32-webcam.bin]
 * Provide an image with useful supported sensors enabled
\*********************************************************************************************/

#ifdef FIRMWARE_WEBCAM

#ifndef CODE_IMAGE_STR
  #define CODE_IMAGE_STR "webcam"
#endif

#define USE_WEBCAM
#define ENABLE_RTSPSERVER
#define USE_SPI
#define USE_SDCARD

#define USE_I2C
#undef USE_HOME_ASSISTANT
#define USE_TASMOTA_DISCOVERY                    // Enable Tasmota Discovery support (+2k code)
#undef USE_DOMOTICZ
#undef USE_SERIAL_BRIDGE
#undef ROTARY_V1
#undef USE_IR_REMOTE
#undef USE_ADC
#undef USE_AC_ZERO_CROSS_DIMMER
#undef USE_PWM_DIMMER
#undef USE_PWM_DIMMER_REMOTE
#undef USE_TUYA_MCU
#undef USE_EMULATION_HUE
#undef USE_EMULATION_WEMO
#undef USE_BUZZER
#undef USE_ARILUX_RF
#undef USE_DS18x20
#undef USE_BMP
#undef USE_DHT
#undef USE_BH1750
#undef USE_WS2812
#undef USE_ENERGY_SENSOR
#undef USE_SHUTTER
#undef USE_DEVICE_GROUPS
//#undef USE_BERRY                                 // Disable Berry scripting language
#undef USE_MI_ESP32                             // (ESP32 only) Disable support for ESP32 as a BLE-bridge (+9k2 mem, +292k flash)
#undef USE_BLE_ESP32
#endif  // FIRMWARE_WEBCAM

/*********************************************************************************************\
 * [tasmota32-bluetooth.bin]
 * Provide an image with BLE support
\*********************************************************************************************/

#ifdef FIRMWARE_BLUETOOTH

#ifndef CODE_IMAGE_STR
  #define CODE_IMAGE_STR "bluetooth"
#endif

#undef MODULE
#define MODULE                 WEMOS             // [Module] Select default module from tasmota_template.h
#undef FALLBACK_MODULE
#define FALLBACK_MODULE        WEMOS             // [Module2] Select default module on fast reboot where USER_MODULE is user template

#define USE_INFLUXDB                             // Enable influxdb support (+5k code)
#undef USE_HOME_ASSISTANT
#define USE_TASMOTA_DISCOVERY

#define USE_SDCARD

#ifndef USE_BERRY_ULP                              // potential performance gains with ULP
  #define USE_ADC                                  // so do not use common ADC funtions in that case
#endif
//#undef USE_BERRY                                 // Disable Berry scripting language

#define USE_ETHERNET                             // Add support for ethernet (+20k code)
#ifndef USE_MI_EXT_GUI
  #define USE_BLE_ESP32                            // Enable full BLE driver
  #define USE_EQ3_ESP32
#endif // USE_MI_EXT_GUI
#define USE_MI_ESP32                             // (ESP32 only) Add support for ESP32 as a BLE-bridge (+9k2 mem, +292k flash)

#endif  // FIRMWARE_BLUETOOTH

/*********************************************************************************************\
 * [tasmota32-lvgl.bin]
 * Provide an image which includes uDisplay and LVGL driver
\*********************************************************************************************/

#ifdef FIRMWARE_LVGL

#ifndef CODE_IMAGE_STR
  #define CODE_IMAGE_STR "lvgl-haspmota"
#endif

#undef MODULE
#define MODULE                 WEMOS             // [Module] Select default module from tasmota_template.h
#undef FALLBACK_MODULE
#define FALLBACK_MODULE        WEMOS             // [Module2] Select default module on fast reboot where USER_MODULE is user template

#define USE_MATTER_DEVICE
#undef USE_DOMOTICZ
#undef USE_HOME_ASSISTANT
#define USE_TASMOTA_DISCOVERY                    // Enable Tasmota Discovery support (+2k code)

#define USE_WEBCLIENT_HTTPS

#define USE_I2S
#define USE_SPI
#define USE_LVGL
  #undef SET_ESP32_STACK_SIZE
  #define SET_ESP32_STACK_SIZE (24 * 1024)
#define USE_DISPLAY
#define USE_MPU_ACCEL
#define USE_RTC_CHIPS                            // Enable RTC chip support and NTP server - Select only one
  #define USE_BM8563
#define USE_MLX90614
#define USE_UNIVERSAL_DISPLAY
#define USE_UNIVERSAL_TOUCH
//#define USE_XPT2046
//#define USE_FT5206
//#define USE_GT911
//#define USE_CST816S
#define USE_DISPLAY_LVGL_ONLY

//#undef USE_DISPLAY_MODES1TO5
#undef USE_DISPLAY_LCD
#undef USE_DISPLAY_MATRIX
#undef USE_DISPLAY_SEVENSEG

#define USE_ENHANCED_GUI_WIFI_SCAN

#define USE_SDCARD

#define ROTARY_V1                                // Add support for Rotary Encoder as used in MI Desk Lamp

#define USE_TUYA_MCU                             // Add support for Tuya Serial MCU
#ifndef TUYA_DIMMER_ID
  #define TUYA_DIMMER_ID       0                 // Default dimmer Id
#endif
#undef USE_ARMTRONIX_DIMMERS                    // Disable support for Armtronix Dimmers (+1k4 code)
//#undef USE_PS_16_DZ                             // Disable support for PS-16-DZ Dimmer (+2k code)
#undef USE_SONOFF_IFAN                          // Disable support for Sonoff iFan02 and iFan03 (+2k code)
#define USE_BUZZER                               // Add support for a buzzer (+0k6 code)
//#undef USE_ARILUX_RF                            // Disable support for Arilux RF remote controller (+0k8 code, 252 iram (non 2.3.0))
//#define USE_DEEPSLEEP                            // Add support for deepsleep (+1k code)
#undef USE_EXS_DIMMER                           // Disable support for EX-Store WiFi Dimmer
//#define USE_HOTPLUG                              // Add support for sensor HotPlug
//#undef USE_DEVICE_GROUPS                        // Disable support for device groups (+5k6 code)
#undef USE_PWM_DIMMER                           // Disable support for MJ-SD01/acenx/NTONPOWER PWM dimmers (+4k5 code)
#undef USE_KEELOQ                               // Disable support for Jarolift rollers by Keeloq algorithm (+4k5 code)
#undef USE_SONOFF_D1                            // Disable support for Sonoff D1 Dimmer (+0k7 code)
#undef USE_SHELLY_DIMMER                        // Disable support for Shelly Dimmer (+3k code)

#define USE_LIGHT_PALETTE                        // Add support for color palette (+0k9 code)
#undef USE_EMULATION_WEMO
//#undef USE_EMULATION_HUE

#define USE_WS2812

#define USE_DS18x20                              // Add support for DS18x20 sensors with id sort, single scan and read retry (+1k3 code)

#define USE_ENERGY_SENSOR                      // Add energy to support Shelly Pro 4PM display (+38k code)
#define USE_SHELLY_PRO

#define USE_I2C                                  // I2C using library wire (+10k code, 0k2 mem, 124 iram)
#undef USE_MLX90614

//#define USE_SHT                                // [I2cDriver8] Enable SHT1X sensor (+1k4 code)
#define USE_HTU                                // [I2cDriver9] Enable HTU21/SI7013/SI7020/SI7021 sensor (I2C address 0x40) (+1k5 code)
#define USE_BMP                                // [I2cDriver10] Enable BMP085/BMP180/BMP280/BME280 sensors (I2C addresses 0x76 and 0x77) (+4k4 code)
//  #define USE_BME68X                           // Enable support for BME680/BME688 sensor using Bosch BME68x library (+6k9 code)
//#define USE_BH1750                             // [I2cDriver11] Enable BH1750 sensor (I2C address 0x23 or 0x5C) (+0k5 code)
//#define USE_VEML6070                           // [I2cDriver12] Enable VEML6070 sensor (I2C addresses 0x38 and 0x39) (+1k5 code)
//#define USE_VEML6075                           // [I2cDriver49] Enable VEML6075 UVA/UVB/UVINDEX Sensor (I2C address 0x10) (+2k1 code)
//#define USE_VEML7700                           // [I2cDriver50] Enable VEML7700 Ambient Light sensor (I2C addresses 0x10) (+4k5 code)
//#define USE_ADS1115                            // [I2cDriver13] Enable ADS1115 16 bit A/D converter (I2C address 0x48, 0x49, 0x4A or 0x4B) based on Adafruit ADS1x15 library (no library needed) (+0k7 code)
//#define USE_INA219                             // [I2cDriver14] Enable INA219 (I2C address 0x40, 0x41 0x44 or 0x45) Low voltage and current sensor (+1k code)
//#define USE_INA226                             // [I2cDriver35] Enable INA226 (I2C address 0x40, 0x41 0x44 or 0x45) Low voltage and current sensor (+2k3 code)
//#define USE_SHT3X                              // [I2cDriver15] Enable SHT3x (I2C address 0x44 or 0x45) or SHTC3 (I2C address 0x70) sensor (+0k7 code)
//#define USE_TSL2561                            // [I2cDriver16] Enable TSL2561 sensor (I2C address 0x29, 0x39 or 0x49) using library Joba_Tsl2561 (+2k3 code)
//#define USE_TSL2591                            // [I2cDriver40] Enable TSL2591 sensor (I2C address 0x29) using library Adafruit_TSL2591 (+1k6 code)
//#define USE_MGS                                // [I2cDriver17] Enable Xadow and Grove Mutichannel Gas sensor using library Multichannel_Gas_Sensor (+10k code)
//#define USE_SGP30                              // [I2cDriver18] Enable SGP30 sensor (I2C address 0x58) (+1k1 code)
//#define USE_SGP40                              // [I2cDriver69] Enable SGP40 sensor (I2C address 0x59) (+1k4 code)
//#define USE_SGP4X                              // [I2cDriver82] Enable SGP41 sensor (I2C address 0x59) (+7k2 code)
//#define USE_SEN5X                              // [I2cDriver76] Enable SEN5X sensor (I2C address 0x69) (+3k code)
//#define USE_SI1145                             // [I2cDriver19] Enable SI1145/46/47 sensor (I2C address 0x60) (+1k code)
//#define USE_LM75AD                             // [I2cDriver20] Enable LM75AD sensor (I2C addresses 0x48 - 0x4F) (+0k5 code)
//#define USE_APDS9960                           // [I2cDriver21] Enable APDS9960 Proximity Sensor (I2C address 0x39). Disables SHT and VEML6070 (+4k7 code)
//#define USE_MCP230xx                           // [I2cDriver22] Enable MCP23008/MCP23017 - Must define I2C Address in #define USE_MCP230xx_ADDR below - range 0x20 - 0x27 (+4k7 code)
#define USE_MCP23XXX_DRV                       // [I2cDriver77] Enable MCP23xxx support as virtual switch/button/relay (+3k(I2C)/+5k(SPI) code)
//#define USE_PCA9685                            // [I2cDriver1] Enable PCA9685 I2C HW PWM Driver - Must define I2C Address in #define USE_PCA9685_ADDR below - range 0x40 - 0x47 (+1k4 code)
//#define USE_PCA9685_V2                         // [I2cDriver1] Enable PCA9685 I2C HW PWM Driver - Must define I2C Address in #define USE_PCA9685_ADDR below - range 0x40 - 0x47 (+3k4 code)
//#define USE_MPR121                             // [I2cDriver23] Enable MPR121 controller (I2C addresses 0x5A, 0x5B, 0x5C and 0x5D) in input mode for touch buttons (+1k3 code)
//#define USE_CCS811                             // [I2cDriver24] Enable CCS811 sensor (I2C address 0x5A) (+2k2 code)
//#define USE_CCS811_V2                          // [I2cDriver24] Enable CCS811 sensor (I2C addresses 0x5A and 0x5B) (+2k8 code)
//#define USE_ENS16x                             // [I2cDriver85] Enable ENS160 and ENS161 sensor (I2C addresses 0x52 and 0x53) (+3k1 of code and 524 of RAM)
//#define USE_ENS210                             // [I2cDriver86] Enable ENS210 sensor (I2C addresses 0x43 and 0x44) (+4k0 of code and 944 of RAM)
//#define USE_MPU6050                            // [I2cDriver25] Enable MPU6050 sensor (I2C address 0x68 AD0 low or 0x69 AD0 high) (+3K3 of code and 188 Bytes of RAM)
//#define USE_MGC3130                            // [I2cDriver27] Enable MGC3130 Electric Field Effect Sensor (I2C address 0x42) (+2k7 code, 0k3 mem)
//#define USE_MAX44009                           // [I2cDriver28] Enable MAX44009 Ambient Light sensor (I2C addresses 0x4A and 0x4B) (+0k8 code)
//#define USE_SCD30                              // [I2cDriver29] Enable Sensiron SCd30 CO2 sensor (I2C address 0x61) (+3k3 code)
//#define USE_SCD40                              // [I2cDriver62] Enable Sensiron SCd40 CO2 sensor (I2C address 0x62) (+3k3 code)
//#define USE_SPS30                              // [I2cDriver30] Enable Sensiron SPS30 particle sensor (I2C address 0x69) (+1.7 code)
#define USE_ADE7953                            // [I2cDriver7] Enable ADE7953 Energy monitor as used on Shelly 2.5 (I2C address 0x38) (+1k5)
//#define USE_VL53L0X                            // [I2cDriver31] Enable VL53L0x time of flight sensor (I2C address 0x29) (+4k code)
//#define USE_VL53L1X                            // [I2cDriver54] Enable VL53L1X time of flight sensor (I2C address 0x29) using Pololu VL53L1X library (+2k9 code)
//#define USE_TOF10120                           // [I2cDriver57] Enable TOF10120 time of flight sensor (I2C address 0x52) (+0k6 code)
//#define USE_MLX90614                           // [I2cDriver32] Enable MLX90614 ir temp sensor (I2C address 0x5a) (+0.6k code)
//#define USE_CHIRP                              // [I2cDriver33] Enable CHIRP soil moisture sensor (variable I2C address, default 0x20)
//#define USE_PAJ7620                            // [I2cDriver34] Enable PAJ7620 gesture sensor (I2C address 0x73) (+2.5k code)
//#define USE_PCF8574                            // [I2cDriver2] Enable PCF8574 I/O Expander (I2C addresses 0x20 - 0x26 and 0x39 - 0x3F) (+2k1 code)
//  #define USE_PCF8574_MODE2                    // Enable Mode2 virtual relays/buttons/switches (+2k3 code)
//  #define USE_PCF8574_SENSOR                   // Enable Mode1 inputs and outputs in SENSOR message (+0k2 code)
//  #define USE_PCF8574_DISPLAYINPUT             // Enable Mode1 inputs display in Web page (+0k2 code)
//  #define USE_PCF8574_MQTTINPUT                // Enable Mode1 MQTT message & rule process on input change detection : stat/%topic%/PCF8574_INP = {"Time":"2021-03-07T16:19:23+01:00","PCF8574-1_INP":{"D1":1}} (+0k5 code)
//#define USE_HIH6                               // [I2cDriver36] Enable Honeywell HIH Humidity and Temperature sensor (I2C address 0x27) (+0k6)
//#define USE_DHT12                              // [I2cDriver41] Enable DHT12 humidity and temperature sensor (I2C address 0x5C) (+0k7 code)
//#define USE_DS1624                             // [I2cDriver42] Enable DS1624, DS1621 temperature sensor (I2C addresses 0x48 - 0x4F) (+1k2 code)
//#define USE_AHT1x                              // [I2cDriver43] Enable AHT10/15 humidity and temperature sensor (I2C address 0x38, 0x39) (+0k8 code)
//#define USE_AHT2x                              // [I2cDriver43] Enable AHT20 instead of AHT1x humidity and temperature sensor (I2C address 0x38) (+0k8 code)
//#define USE_WEMOS_MOTOR_V1                     // [I2cDriver44] Enable Wemos motor driver V1 (I2C addresses 0x2D - 0x30) (+0k7 code)
  #define WEMOS_MOTOR_V1_ADDR  0x30              // Default I2C address 0x30
  #define WEMOS_MOTOR_V1_FREQ  1000              // Default frequency
//#define USE_HDC1080                            // [I2cDriver45] Enable HDC1080 temperature/humidity sensor (I2C address 0x40) (+1k5 code)
//#define USE_IAQ                                // [I2cDriver46] Enable iAQ-core air quality sensor (I2C address 0x5a) (+0k6 code)
//#define USE_AS3935                             // [I2cDriver48] Enable AS3935 Franklin Lightning Sensor (I2C address 0x03) (+5k4 code)
//#define USE_MCP9808                            // [I2cDriver51] Enable MCP9808 temperature sensor (I2C addresses 0x18 - 0x1F) (+0k9 code)
//#define USE_HP303B                             // [I2cDriver52] Enable HP303B temperature and pressure sensor (I2C address 0x76 or 0x77) (+6k2 code)
//#define USE_MLX90640                           // [I2cDriver53] Enable MLX90640 IR array temperature sensor (I2C address 0x33) (+20k code)
//#define USE_EZOPH                              // [I2cDriver55] Enable support for EZO's pH sensor (+0k3 code) - Shared EZO code required for any EZO device (+1k2 code)
//#define USE_EZOORP                             // [I2cDriver55] Enable support for EZO's ORP sensor (+0k3 code) - Shared EZO code required for any EZO device (+1k2 code)
//#define USE_EZORTD                             // [I2cDriver55] Enable support for EZO's RTD sensor (+0k2 code) - Shared EZO code required for any EZO device (+1k2 code)
//#define USE_EZOHUM                             // [I2cDriver55] Enable support for EZO's HUM sensor (+0k3 code) - Shared EZO code required for any EZO device (+1k2 code)
//#define USE_EZOEC                              // [I2cDriver55] Enable support for EZO's EC sensor (+0k3 code) - Shared EZO code required for any EZO device (+1k2 code)
//#define USE_EZOCO2                             // [I2cDriver55] Enable support for EZO's CO2 sensor (+0k2 code) - Shared EZO code required for any EZO device (+1k2 code)
//#define USE_EZOO2                              // [I2cDriver55] Enable support for EZO's O2 sensor (+0k3 code) - Shared EZO code required for any EZO device (+1k2 code)
//#define USE_EZOPRS                             // [I2cDriver55] Enable support for EZO's PRS sensor (+0k7 code) - Shared EZO code required for any EZO device (+1k2 code)
//#define USE_EZOFLO                             // [I2cDriver55] Enable support for EZO's FLO sensor (+0k4 code) - Shared EZO code required for any EZO device (+1k2 code)
//#define USE_EZODO                              // [I2cDriver55] Enable support for EZO's DO sensor (+0k3 code) - Shared EZO code required for any EZO device (+1k2 code)
//#define USE_EZORGB                             // [I2cDriver55] Enable support for EZO's RGB sensor (+0k5 code) - Shared EZO code required for any EZO device (+1k2 code)
//#define USE_EZOPMP                             // [I2cDriver55] Enable support for EZO's PMP sensor (+0k3 code) - Shared EZO code required for any EZO device (+1k2 code)
//#define USE_SEESAW_SOIL                        // [I2cDriver56] Enable Capacitice Soil Moisture & Temperature Sensor (I2C addresses 0x36 - 0x39) (+1k3 code)
//#define USE_MPU_ACCEL                          // [I2cDriver58] Enable MPU6886/MPU9250 - found in M5Stack - support both I2C buses on ESP32 (I2C address 0x68) (+2k code)
//#define USE_AM2320                             // [I2cDriver60] Enable AM2320 temperature and humidity Sensor (I2C address 0x5C) (+1k code)
//#define USE_T67XX                              // [I2cDriver61] Enable Telaire T67XX CO2 sensor (I2C address 0x15) (+1k3 code)
//#define USE_DS3502                             // [I2CDriver67] Enable DS3502 digital potentiometer (I2C address 0x28 - 0x2B) (+0k4 code)
//#define USE_HYT                                // [I2CDriver68] Enable HYTxxx temperature and humidity sensor (I2C address 0x28) (+0k5 code)
//#define USE_LUXV30B                            // [I2CDriver70] Enable RFRobot SEN0390 LuxV30b ambient light sensor (I2C address 0x4A) (+0k5 code)
//#define USE_PMSA003I                           // [I2cDriver78] Enable PMSA003I Air Quality Sensor (I2C address 0x12) (+1k8 code)
//#define USE_GDK101                             // [I2cDriver79] Enable GDK101 sensor (I2C addresses 0x18 - 0x1B) (+1k2 code)
//#define USE_MS5837                             // [I2cDriver91] Enable MS5837 sensor (I2C address 0x76) (+2k7 code)

//#define USE_RTC_CHIPS                          // Enable RTC chip support and NTP server - Select only one
//  #define USE_DS3231                           // [I2cDriver26] Enable DS3231 RTC (I2C address 0x68) (+1k2 code)
//  #define USE_BM8563                           // [I2cDriver59] Enable BM8563 RTC - found in M5Stack - support both I2C buses on ESP32 (I2C address 0x51) (+2.5k code)
//  #define USE_PCF85363                         // [I2cDriver66] Enable PCF85363 RTC - found Shelly 3EM (I2C address 0x51) (+0k7 code)

//#define USE_MHZ19                                // Add support for MH-Z19 CO2 sensor (+2k code)
//#define USE_SENSEAIR                             // Add support for SenseAir K30, K70 and S8 CO2 sensor (+2k3 code)
//#define USE_CM110x                               // Add support for CM110x CO2 sensors (+2k7 code)
#ifndef CO2_LOW
  #define CO2_LOW              800               // Below this CO2 value show green light (needs PWM or WS2812 RG(B) led and enable with SetOption18 1)
#endif
#ifndef CO2_HIGH
  #define CO2_HIGH             1200              // Above this CO2 value show red light (needs PWM or WS2812 RG(B) led and enable with SetOption18 1)
#endif
//#define USE_PMS5003                              // Add support for PMS5003 and PMS7003 particle concentration sensor (+1k3 code)
  //#define PMS_MODEL_PMS3003                      // Enable support of PMS3003 instead of PMS5003/PMS7003 (needs the USE_PMS5003 above)
  //#define PMS_MODEL_PMS5003T                     // Enable support for PMSx003T models that report temperature and humidity (needs the USE_PMS5003 above)
//#define USE_NOVA_SDS                             // Add support for SDS011 and SDS021 particle concentration sensor (+0k7 code)
//#define USE_HPMA                                 // Add support for Honeywell HPMA115S0 particle concentration sensor
//#define USE_SR04                                 // Add support for HC-SR04 ultrasonic devices (+1k code)
//#define USE_ME007                                // Add support for ME007 ultrasonic devices (+1k5 code)
//#define USE_DYP                                  // Add support for DYP ME-007 ultrasonic distance sensor, serial port version (+0k5 code)
#define USE_SERIAL_BRIDGE                        // Add support for software Serial Bridge (+2k code)
//#define USE_MODBUS_BRIDGE                        // Add support for software Modbus Bridge (+3k code)
//#define USE_MP3_PLAYER                           // Use of the DFPlayer Mini MP3 Player RB-DFR-562 commands: play, volume and stop
//#define USE_AZ7798                               // Add support for AZ-Instrument 7798 CO2 datalogger
//#define USE_PN532_HSU                            // Add support for PN532 using HSU (Serial) interface (+1k8 code, 140 bytes mem)
//#define USE_ZIGBEE                               // Enable serial communication with Zigbee CC2530 flashed with ZNP
//#define USE_RDM6300                              // Add support for RDM6300 125kHz RFID Reader (+0k8)

#define USE_IR_REMOTE

#define USE_ETHERNET                             // Add support for ethernet (+20k code

#endif  // FIRMWARE_TASMOTA_LVGL *******************************************************************

/*********************************************************************************************\
 * [tasmota32-zbbrdgpro]
 * Provide an image for Sonoff Zigbee Bridge Pro
\*********************************************************************************************/

#ifdef FIRMWARE_ZBBRDGPRO

#ifndef CODE_IMAGE_STR
  #define CODE_IMAGE_STR "zbbrdgpro"
#endif

#undef MODULE
#define MODULE                 WEMOS             // [Module] Select default module from tasmota_template.h
#undef FALLBACK_MODULE
#define FALLBACK_MODULE        WEMOS             // [Module2] Select default module on fast reboot where USER_MODULE is user template

#define USE_INFLUXDB                             // Enable influxdb support (+5k code)

#undef USE_DOMOTICZ
#undef USE_HOME_ASSISTANT
#define USE_TASMOTA_DISCOVERY                    // Enable Tasmota Discovery support (+2k code)

#define USE_WEBCLIENT_HTTPS

#define USE_ZIGBEE
#define USE_TCP_BRIDGE

#define USE_ENHANCED_GUI_WIFI_SCAN

#undef USE_ENERGY_SENSOR                        // Disable support for energy sensors
#undef USE_SHUTTER                              // Disable support for shutter
#undef USE_IR_REMOTE                            // Disable support for IR Remote
#undef USE_ARMTRONIX_DIMMERS                    // Disable support for Armtronix Dimmers (+1k4 code)
#undef USE_PS_16_DZ                             // Disable support for PS-16-DZ Dimmer (+2k code)
#undef USE_SONOFF_IFAN                          // Disable support for Sonoff iFan02 and iFan03 (+2k code)
//#define USE_BUZZER                               // Add support for a buzzer (+0k6 code)
#undef USE_ARILUX_RF                            // Disable support for Arilux RF remote controller (+0k8 code, 252 iram (non 2.3.0))
//#define USE_DEEPSLEEP                            // Add support for deepsleep (+1k code)
#undef USE_EXS_DIMMER                           // Disable support for EX-Store WiFi Dimmer
#undef USE_KEELOQ                               // Disable support for Jarolift rollers by Keeloq algorithm (+4k5 code)
#undef USE_SONOFF_D1                            // Disable support for Sonoff D1 Dimmer (+0k7 code)
#undef USE_SHELLY_DIMMER                        // Disable support for Shelly Dimmer (+3k code)

#define USE_I2C                                  // I2C using library wire (+10k code, 0k2 mem, 124 iram)
#define USE_SPI                                // Hardware SPI using GPIO12(MISO), GPIO13(MOSI) and GPIO14(CLK) in addition to two user selectable GPIOs(CS and DC)

#define USE_ETHERNET                             // Add support for ethernet (+20k code)

//#ifndef USE_KNX
//#define USE_KNX                                  // Enable KNX IP Protocol Support (+23k code, +3k3 mem)
//#endif

#endif // FIRMWARE_ZBBRDGPRO

/*********************************************************************************************\
 * [tasmota32-nspanel]
 * Provide an image for ths Sonoff NsPanel
\*********************************************************************************************/

#ifdef FIRMWARE_NSPANEL

#ifndef CODE_IMAGE_STR
  #define CODE_IMAGE_STR "nspanel"
#endif

#undef MODULE
#define MODULE                 WEMOS             // [Module] Select default module from tasmota_template.h
#undef FALLBACK_MODULE
#define FALLBACK_MODULE        WEMOS             // [Module2] Select default module on fast reboot where USER_MODULE is user template

#define FIRMWARE_TASMOTA32

#endif // FIRMWARE_NSPANEL

/*********************************************************************************************\
 * [tasmota32.bin]
 * Provide an image which includes KNX and Sensors
\*********************************************************************************************/

#ifdef FIRMWARE_TASMOTA32

#ifndef CODE_IMAGE_STR
  #define CODE_IMAGE_STR "tasmota32"
#endif

#define USE_MATTER_DEVICE

#define USE_INFLUXDB                             // Enable influxdb support (+5k code)

#define USE_ENHANCED_GUI_WIFI_SCAN

#define USE_SDCARD

#define ROTARY_V1                                // Add support for Rotary Encoder as used in MI Desk Lamp

#define USE_TUYA_MCU                             // Add support for Tuya Serial MCU
#ifndef TUYA_DIMMER_ID
  #define TUYA_DIMMER_ID       0                 // Default dimmer Id
#endif
#undef USE_ARMTRONIX_DIMMERS                    // Disable support for Armtronix Dimmers (+1k4 code)
//#undef USE_PS_16_DZ                             // Disable support for PS-16-DZ Dimmer (+2k code)
#undef USE_SONOFF_IFAN                          // Disable support for Sonoff iFan02 and iFan03 (+2k code)
#define USE_BUZZER                               // Add support for a buzzer (+0k6 code)
//#undef USE_ARILUX_RF                            // Disable support for Arilux RF remote controller (+0k8 code, 252 iram (non 2.3.0))
#define USE_DEEPSLEEP                            // Add support for deepsleep (+1k code)
#undef USE_EXS_DIMMER                           // Disable support for EX-Store WiFi Dimmer
//#define USE_HOTPLUG                              // Add support for sensor HotPlug
//#undef USE_DEVICE_GROUPS                        // Disable support for device groups (+5k6 code)
//#undef USE_PWM_DIMMER                           // Disable support for MJ-SD01/acenx/NTONPOWER PWM dimmers (+4k5 code)
#undef USE_KEELOQ                               // Disable support for Jarolift rollers by Keeloq algorithm (+4k5 code)
#undef USE_SONOFF_D1                            // Disable support for Sonoff D1 Dimmer (+0k7 code)
#undef USE_SHELLY_DIMMER                        // Disable support for Shelly Dimmer (+3k code)

#define USE_LIGHT_PALETTE                        // Add support for color palette (+0k9 code)
#define USE_LIGHT_ARTNET                         // Add support for DMX/ArtNet via UDP on port 6454 (+3.5k code)
#ifdef CONFIG_IDF_TARGET_ESP32C3
#define USE_MAGIC_SWITCH                         // Add Sonoff MagicSwitch support as implemented in Sonoff Basic R4
#endif

#define USE_DS18x20                              // Add support for DS18x20 sensors with id sort, single scan and read retry (+1k3 code)

#define USE_I2C                                  // I2C using library wire (+10k code, 0k2 mem, 124 iram)
#define USE_SHT                                // [I2cDriver8] Enable SHT1X sensor (+1k4 code)
#define USE_HTU                                // [I2cDriver9] Enable HTU21/SI7013/SI7020/SI7021 sensor (I2C address 0x40) (+1k5 code)
#define USE_BMP                                // [I2cDriver10] Enable BMP085/BMP180/BMP280/BME280 sensors (I2C addresses 0x76 and 0x77) (+4k4 code)
  #define USE_BME68X                           // Enable support for BME680/BME688 sensor using Bosch BME68x library (+6k9 code)
#define USE_BH1750                             // [I2cDriver11] Enable BH1750 sensor (I2C address 0x23 or 0x5C) (+0k5 code)
#define USE_VEML6070                           // [I2cDriver12] Enable VEML6070 sensor (I2C addresses 0x38 and 0x39) (+1k5 code)
//#define USE_VEML6075                           // [I2cDriver49] Enable VEML6075 UVA/UVB/UVINDEX Sensor (I2C address 0x10) (+2k1 code)
//#define USE_VEML7700                           // [I2cDriver50] Enable VEML7700 Ambient Light sensor (I2C addresses 0x10) (+4k5 code)
#define USE_ADS1115                            // [I2cDriver13] Enable ADS1115 16 bit A/D converter (I2C address 0x48, 0x49, 0x4A or 0x4B) based on Adafruit ADS1x15 library (no library needed) (+0k7 code)
#define USE_INA219                             // [I2cDriver14] Enable INA219 (I2C address 0x40, 0x41 0x44 or 0x45) Low voltage and current sensor (+1k code)
//#define USE_INA226                             // [I2cDriver35] Enable INA226 (I2C address 0x40, 0x41 0x44 or 0x45) Low voltage and current sensor (+2k3 code)
#define USE_SHT3X                              // [I2cDriver15] Enable SHT3x (I2C address 0x44 or 0x45) or SHTC3 (I2C address 0x70) sensor (+0k7 code)
//#define USE_TSL2561                            // [I2cDriver16] Enable TSL2561 sensor (I2C address 0x29, 0x39 or 0x49) using library Joba_Tsl2561 (+2k3 code)
//#define USE_TSL2591                            // [I2cDriver40] Enable TSL2591 sensor (I2C address 0x29) using library Adafruit_TSL2591 (+1k6 code)
#define USE_MGS                                // [I2cDriver17] Enable Xadow and Grove Mutichannel Gas sensor using library Multichannel_Gas_Sensor (+10k code)
#define USE_SGP30                              // [I2cDriver18] Enable SGP30 sensor (I2C address 0x58) (+1k1 code)
#define USE_SGP40                              // [I2cDriver69] Enable SGP40 sensor (I2C address 0x59) (+1k4 code)
#define USE_SGP4X                              // [I2cDriver82] Enable SGP41 sensor (I2C address 0x59) (+7k2 code)
#define USE_SEN5X                              // [I2cDriver76] Enable SEN5X sensor (I2C address 0x69) (+3k code)
//#define USE_SI1145                             // [I2cDriver19] Enable SI1145/46/47 sensor (I2C address 0x60) (+1k code)
#define USE_LM75AD                             // [I2cDriver20] Enable LM75AD sensor (I2C addresses 0x48 - 0x4F) (+0k5 code)
//#define USE_APDS9960                           // [I2cDriver21] Enable APDS9960 Proximity Sensor (I2C address 0x39). Disables SHT and VEML6070 (+4k7 code)
//#define USE_MCP230xx                           // [I2cDriver22] Enable MCP23008/MCP23017 - Must define I2C Address in #define USE_MCP230xx_ADDR below - range 0x20 - 0x27 (+4k7 code)
#define USE_MCP23XXX_DRV                       // [I2cDriver77] Enable MCP23xxx support as virtual switch/button/relay (+3k(I2C)/+5k(SPI) code)
//#define USE_PCA9685                            // [I2cDriver1] Enable PCA9685 I2C HW PWM Driver - Must define I2C Address in #define USE_PCA9685_ADDR below - range 0x40 - 0x47 (+1k4 code)
//#define USE_PCA9685_V2                         // [I2cDriver1] Enable PCA9685 I2C HW PWM Driver - Must define I2C Address in #define USE_PCA9685_ADDR below - range 0x40 - 0x47 (+3k4 code)
//#define USE_MPR121                             // [I2cDriver23] Enable MPR121 controller (I2C addresses 0x5A, 0x5B, 0x5C and 0x5D) in input mode for touch buttons (+1k3 code)
//#define USE_CCS811                             // [I2cDriver24] Enable CCS811 sensor (I2C address 0x5A) (+2k2 code)
#define USE_CCS811_V2                          // [I2cDriver24] Enable CCS811 sensor (I2C addresses 0x5A and 0x5B) (+2k8 code)
//#define USE_ENS16x                             // [I2cDriver85] Enable ENS160 and ENS161 sensor (I2C addresses 0x52 and 0x53) (+3k1 of code and 524 of RAM)
//#define USE_ENS210                             // [I2cDriver86] Enable ENS210 sensor (I2C addresses 0x43 and 0x44) (+4k0 of code and 944 of RAM)
#define USE_MPU_ACCEL                          // [I2cDriver58] Enable MPU6886, MPU9250 6-axis MotionTracking sensor (I2C address 0x68)
//#define USE_MPU6050                            // [I2cDriver25] Enable MPU6050 sensor (I2C address 0x68 AD0 low or 0x69 AD0 high) (+3K3 of code and 188 Bytes of RAM)
//#define USE_MGC3130                            // [I2cDriver27] Enable MGC3130 Electric Field Effect Sensor (I2C address 0x42) (+2k7 code, 0k3 mem)
//#define USE_MAX44009                           // [I2cDriver28] Enable MAX44009 Ambient Light sensor (I2C addresses 0x4A and 0x4B) (+0k8 code)
#define USE_SCD30                              // [I2cDriver29] Enable Sensiron SCd30 CO2 sensor (I2C address 0x61) (+3k3 code)
#define USE_SCD40                              // [I2cDriver62] Enable Sensiron SCd40 CO2 sensor (I2C address 0x62) (+3k5 code)
//#define USE_SPS30                              // [I2cDriver30] Enable Sensiron SPS30 particle sensor (I2C address 0x69) (+1.7 code)
#define USE_ADE7880                            // [I2cDriver65] Enable ADE7880 Energy monitor as used on Shelly 3EM (I2C address 0x38) (+3k8)
#define USE_ADE7953                            // [I2cDriver7] Enable ADE7953 Energy monitor as used on Shelly 2.5 (I2C address 0x38) (+1k5)
#define USE_VL53L0X                            // [I2cDriver31] Enable VL53L0x time of flight sensor (I2C address 0x29) (+4k code)
//#define USE_VL53L1X                            // [I2cDriver54] Enable VL53L1X time of flight sensor (I2C address 0x29) using Pololu VL53L1X library (+2k9 code)
//#define USE_TOF10120                           // [I2cDriver57] Enable TOF10120 time of flight sensor (I2C address 0x52) (+0k6 code)
//#define USE_MLX90614                           // [I2cDriver32] Enable MLX90614 ir temp sensor (I2C address 0x5a) (+0.6k code)
//#define USE_CHIRP                              // [I2cDriver33] Enable CHIRP soil moisture sensor (variable I2C address, default 0x20)
//#define USE_PAJ7620                            // [I2cDriver34] Enable PAJ7620 gesture sensor (I2C address 0x73) (+2.5k code)
//#define USE_PCF8574                            // [I2cDriver2] Enable PCF8574 I/O Expander (I2C addresses 0x20 - 0x26 and 0x39 - 0x3F) (+2k1 code)
//  #define USE_PCF8574_MODE2                    // Enable Mode2 virtual relays/buttons/switches (+2k3 code)
//  #define USE_PCF8574_SENSOR                   // Enable Mode1 inputs and outputs in SENSOR message (+0k2 code)
//  #define USE_PCF8574_DISPLAYINPUT             // Enable Mode1 inputs display in Web page (+0k2 code)
//  #define USE_PCF8574_MQTTINPUT                // Enable Mode1 MQTT message & rule process on input change detection : stat/%topic%/PCF8574_INP = {"Time":"2021-03-07T16:19:23+01:00","PCF8574-1_INP":{"D1":1}} (+0k5 code)
#define USE_HIH6                               // [I2cDriver36] Enable Honeywell HIH Humidity and Temperature sensor (I2C address 0x27) (+0k6)
#define USE_DHT12                              // [I2cDriver41] Enable DHT12 humidity and temperature sensor (I2C address 0x5C) (+0k7 code)
#define USE_DS1624                             // [I2cDriver42] Enable DS1624, DS1621 temperature sensor (I2C addresses 0x48 - 0x4F) (+1k2 code)
//#define USE_AHT1x                              // [I2cDriver43] Enable AHT10/15 humidity and temperature sensor (I2C address 0x38, 0x39) (+0k8 code)
//#define USE_AHT2x                              // [I2cDriver43] Enable AHT20 instead of AHT1x humidity and temperature sensor (I2C address 0x38) (+0k8 code)
#define USE_WEMOS_MOTOR_V1                     // [I2cDriver44] Enable Wemos motor driver V1 (I2C addresses 0x2D - 0x30) (+0k7 code)
  #define WEMOS_MOTOR_V1_ADDR  0x30              // Default I2C address 0x30
  #define WEMOS_MOTOR_V1_FREQ  1000              // Default frequency
//#define USE_HDC1080                            // [I2cDriver45] Enable HDC1080 temperature/humidity sensor (I2C address 0x40) (+1k5 code)
#define USE_IAQ                                // [I2cDriver46] Enable iAQ-core air quality sensor (I2C address 0x5a) (+0k6 code)
#define USE_AS3935                             // [I2cDriver48] Enable AS3935 Franklin Lightning Sensor (I2C address 0x03) (+5k4 code)
//#define USE_MCP9808                            // [I2cDriver51] Enable MCP9808 temperature sensor (I2C addresses 0x18 - 0x1F) (+0k9 code)
//#define USE_HP303B                             // [I2cDriver52] Enable HP303B temperature and pressure sensor (I2C address 0x76 or 0x77) (+6k2 code)
//#define USE_MLX90640                           // [I2cDriver53] Enable MLX90640 IR array temperature sensor (I2C address 0x33) (+20k code)
//#define USE_EZOPH                              // [I2cDriver55] Enable support for EZO's pH sensor (+0k3 code) - Shared EZO code required for any EZO device (+1k2 code)
//#define USE_EZOORP                             // [I2cDriver55] Enable support for EZO's ORP sensor (+0k3 code) - Shared EZO code required for any EZO device (+1k2 code)
//#define USE_EZORTD                             // [I2cDriver55] Enable support for EZO's RTD sensor (+0k2 code) - Shared EZO code required for any EZO device (+1k2 code)
//#define USE_EZOHUM                             // [I2cDriver55] Enable support for EZO's HUM sensor (+0k3 code) - Shared EZO code required for any EZO device (+1k2 code)
//#define USE_EZOEC                              // [I2cDriver55] Enable support for EZO's EC sensor (+0k3 code) - Shared EZO code required for any EZO device (+1k2 code)
//#define USE_EZOCO2                             // [I2cDriver55] Enable support for EZO's CO2 sensor (+0k2 code) - Shared EZO code required for any EZO device (+1k2 code)
//#define USE_EZOO2                              // [I2cDriver55] Enable support for EZO's O2 sensor (+0k3 code) - Shared EZO code required for any EZO device (+1k2 code)
//#define USE_EZOPRS                             // [I2cDriver55] Enable support for EZO's PRS sensor (+0k7 code) - Shared EZO code required for any EZO device (+1k2 code)
//#define USE_EZOFLO                             // [I2cDriver55] Enable support for EZO's FLO sensor (+0k4 code) - Shared EZO code required for any EZO device (+1k2 code)
//#define USE_EZODO                              // [I2cDriver55] Enable support for EZO's DO sensor (+0k3 code) - Shared EZO code required for any EZO device (+1k2 code)
//#define USE_EZORGB                             // [I2cDriver55] Enable support for EZO's RGB sensor (+0k5 code) - Shared EZO code required for any EZO device (+1k2 code)
//#define USE_EZOPMP                             // [I2cDriver55] Enable support for EZO's PMP sensor (+0k3 code) - Shared EZO code required for any EZO device (+1k2 code)
//#define USE_SEESAW_SOIL                        // [I2cDriver56] Enable Capacitice Soil Moisture & Temperature Sensor (I2C addresses 0x36 - 0x39) (+1k3 code)
//#define USE_DS3502                             // [I2CDriver67] Enable DS3502 digital potentiometer (I2C address 0x28 - 0x2B) (+0k4 code)
//#define USE_HYT                                // [I2CDriver68] Enable HYTxxx temperature and humidity sensor (I2C address 0x28) (+0k5 code)
//#define USE_LUXV30B                            // [I2CDriver70] Enable RFRobot SEN0390 LuxV30b ambient light sensor (I2C address 0x4A) (+0k5 code)
//#define USE_PMSA003I                           // [I2cDriver78] Enable PMSA003I Air Quality Sensor (I2C address 0x12) (+1k8 code)
//#define USE_GDK101                             // [I2cDriver79] Enable GDK101 sensor (I2C addresses 0x18 - 0x1B) (+1k2 code)
//#define USE_MS5837                             // [I2cDriver91] Enable MS5837 sensor (I2C address 0x76) (+2k7 code)

//#define USE_RTC_CHIPS                          // Enable RTC chip support and NTP server - Select only one
//  #define USE_DS3231                           // [I2cDriver26] Enable DS3231 RTC (I2C address 0x68) (+1k2 code)
//  #define USE_BM8563                           // [I2cDriver59] Enable BM8563 RTC - found in M5Stack - support both I2C buses on ESP32 (I2C address 0x51) (+2.5k code)
//  #define USE_PCF85363                         // [I2cDriver66] Enable PCF85363 RTC - found Shelly 3EM (I2C address 0x51) (+0k7 code)

#define USE_SPI                                // Hardware SPI using GPIO12(MISO), GPIO13(MOSI) and GPIO14(CLK) in addition to two user selectable GPIOs(CS and DC)
//#define USE_RC522                              // Add support for MFRC522 13.56Mhz Rfid reader (+6k code)
//#define USE_MCP2515                            // Add support for can bus using MCP2515 (+7k code)
//#define USE_CANSNIFFER                         // Add support for can bus sniffer using MCP2515 (+5k code)
#define USE_MCP23XXX_DRV                         // [I2cDriver77] Enable MCP23xxx support as virtual switch/button/relay (+3k(I2C)/+5k(SPI) code)
#define USE_SHELLY_PRO                           // Add support for Shelly Pro
#define USE_SPI_LORA                           // Add support for LoRaSend and LoRaCommand (+4k code)
  #define USE_LORA_SX126X                      // Add driver support for LoRa on SX126x based devices like LiliGo T3S3 Lora32 (+16k code)
  #define USE_LORA_SX127X                      // Add driver support for LoRa on SX127x based devices like M5Stack LoRa868, RFM95W (+5k code)
  #define USE_LORAWAN_BRIDGE                   // Add support for LoRaWan bridge (+8k code)

#define USE_MHZ19                                // Add support for MH-Z19 CO2 sensor (+2k code)
#define USE_SENSEAIR                             // Add support for SenseAir K30, K70 and S8 CO2 sensor (+2k3 code)
#define USE_CM110x                               // Add support for CM110x CO2 sensors (+2k7 code)
#ifndef CO2_LOW
  #define CO2_LOW              800               // Below this CO2 value show green light (needs PWM or WS2812 RG(B) led and enable with SetOption18 1)
#endif
#ifndef CO2_HIGH
  #define CO2_HIGH             1200              // Above this CO2 value show red light (needs PWM or WS2812 RG(B) led and enable with SetOption18 1)
#endif
#define USE_PMS5003                              // Add support for PMS5003 and PMS7003 particle concentration sensor (+1k3 code)
  //#define PMS_MODEL_PMS3003                      // Enable support of PMS3003 instead of PMS5003/PMS7003 (needs the USE_PMS5003 above)
  //#define PMS_MODEL_PMS5003T                     // Enable support for PMSx003T models that report temperature and humidity (needs the USE_PMS5003 above)
#define USE_NOVA_SDS                             // Add support for SDS011 and SDS021 particle concentration sensor (+0k7 code)
#define USE_HPMA                                 // Add support for Honeywell HPMA115S0 particle concentration sensor
#define USE_SR04                                 // Add support for HC-SR04 ultrasonic devices (+1k code)
//#define USE_ME007                                // Add support for ME007 ultrasonic devices (+1k5 code)
//#define USE_DYP                                  // Add support for DYP ME-007 ultrasonic distance sensor, serial port version (+0k5 code)
#define USE_SERIAL_BRIDGE                        // Add support for software Serial Bridge (+2k code)
#define USE_MODBUS_BRIDGE                        // Add support for software Modbus Bridge (+3k code)
#define USE_MP3_PLAYER                           // Use of the DFPlayer Mini MP3 Player RB-DFR-562 commands: play, volume and stop
//#define USE_AZ7798                               // Add support for AZ-Instrument 7798 CO2 datalogger
#define USE_PN532_HSU                            // Add support for PN532 using HSU (Serial) interface (+1k8 code, 140 bytes mem)
//#define USE_ZIGBEE                               // Enable serial communication with Zigbee CC2530 flashed with ZNP
#define USE_RDM6300                              // Add support for RDM6300 125kHz RFID Reader (+0k8)
#define USE_IBEACON                              // Add support for bluetooth LE passive scan of ibeacon devices (uses HM17 module)
//#define USE_GPS                                  // Add support for GPS and NTP Server for becoming Stratus 1 Time Source (+ 3.1kb flash, +132 bytes RAM)

//#define USE_BLE_ESP32                            // (ESP32 only) Add support for native BLE on ESP32 - use new driver
//#define USE_MI_ESP32                             // (ESP32 only) Add support for ESP32 as a BLE-bridge (+9k2 mem, +292k flash)

#define USE_HRXL                                 // Add support for MaxBotix HRXL-MaxSonar ultrasonic range finders (+0k7)
//#define USE_TASMOTA_CLIENT                       // Add support for Arduino Uno/Pro Mini via serial interface including flashing (+2k3 code, 44 mem)
//#define USE_OPENTHERM                            // Add support for OpenTherm (+15k code)
//#define USE_MIEL_HVAC                            // Add support for Mitsubishi Electric HVAC serial interface (+5k code)
//#define USE_PROJECTOR_CTRL                       // Add support for LCD/DLP Projector serial control interface (+2k code)
//  #define USE_PROJECTOR_CTRL_NEC                 // Use codes for NEC
//  #define USE_PROJECTOR_CTRL_OPTOMA              // Use codes for OPTOMA
//#define USE_AS608                                // Add support for AS608 optical and R503 capacitive fingerprint sensor (+3k4 code)
//#define USE_TFMINIPLUS                           // Add support for TFmini Plus (TFmini, TFmini-S) LiDAR modules via UART interface
//#define USE_HRG15                                // Add support for Hydreon RG-15 Solid State Rain sensor (+1k5 code)

#define USE_ENERGY_SENSOR                        // Add energy sensors (-14k code)
#define USE_PZEM004T                             // Add support for PZEM004T Energy monitor (+2k code)
#define USE_PZEM_AC                              // Add support for PZEM014,016 Energy monitor (+1k1 code)
#define USE_PZEM_DC                              // Add support for PZEM003,017 Energy monitor (+1k1 code)
#define USE_MCP39F501                            // Add support for MCP39F501 Energy monitor as used in Shelly 2 (+3k1 code)
#define USE_SDM72                                // Add support for Eastron SDM72-Modbus energy monitor (+0k3 code)
#define USE_SDM120                               // Add support for Eastron SDM120-Modbus energy monitor (+1k1 code)
#define USE_SDM230                               // Add support for Eastron SDM230-Modbus energy monitor (+?? code)
#define USE_SDM630                               // Add support for Eastron SDM630-Modbus energy monitor (+0k6 code)
#define USE_DDS2382                              // Add support for Hiking DDS2382 Modbus energy monitor (+0k6 code)
#define USE_DDSU666                              // Add support for Chint DDSU666 Modbus energy monitor (+0k6 code)
//#define USE_SOLAX_X1                             // Add support for Solax X1 series Modbus log info (+3k1 code)
//#define USE_LE01MR                               // Add support for F&F LE-01MR modbus energy meter (+2k code)
//#define USE_TELEINFO                             // Add support for French Energy Provider metering telemetry (+5k2 code, +168 RAM + SmartMeter LinkedList Values RAM)
#define USE_WE517                                // Add support for Orno WE517-Modbus energy monitor (+1k code)
#define USE_SONOFF_SPM                           // Add support for ESP32 based Sonoff Smart Stackable Power Meter (+11k code)
#define USE_MODBUS_ENERGY                        // Add support for generic modbus energy monitor using a user file in rule space (+5k code)
//#define USE_BIOPDU                               // Add support for BioPDU 625x12 6-channel energy monitor
#define USE_BL0906                               // Add support for BL0906 up to 6 channel Energy monitor as used in Athom EM2/EM6

#define USE_DHT                                  // Add support for DHT11, AM2301 (DHT21, DHT22, AM2302, AM2321) and SI7021 Temperature and Humidity sensor
#define USE_MAX31855                             // Add support for MAX31855 K-Type thermocouple sensor using softSPI
//#define USE_MAX31865                             // Add support for MAX31865 RTD sensors using softSPI
#define USE_IR_REMOTE                            // Send IR remote commands using library IRremoteESP8266 and ArduinoJson (+4k code, 0k3 mem, 48 iram)
  #define USE_IR_RECEIVE                         // Support for IR receiver (+5k5 code, 264 iram)
#define USE_LMT01                                // Add support for TI LMT01 temperature sensor, count pulses on single GPIO (+0k5 code)
//#define USE_WIEGAND                              // Add support for 24/26/32/34 bit RFID Wiegand interface (D0/D1) (+1k7 code)
#define USE_SHIFT595                             // Add support for 74xx595 8-bit shift registers (+0k7 code)
//  #define SHIFT595_INVERT_OUTPUTS false            // [SetOption133] Don't invert outputs of 74x595 shift register
//  #define SHIFT595_DEVICE_COUNT  1                 // [Shift595DeviceCount] Set the number of connected 74x595 shift registers
#define USE_TM1638                               // Add support for TM1638 switches copying Switch1 .. Switch8 (+1k code)
#define USE_HX711                                // Add support for HX711 load cell (+1k5 code)
//#define USE_HX711_GUI                            // Add optional web GUI to HX711 as scale (+1k8 code)
//#define USE_TX20_WIND_SENSOR                     // Add support for La Crosse TX20 anemometer (+2k6/0k8 code)
//#define USE_TX23_WIND_SENSOR                     // Add support for La Crosse TX23 anemometer (+2k7/1k code)
//#define USE_WINDMETER                            // Add support for analog anemometer (+2k2 code)
#define USE_RC_SWITCH                            // Add support for RF transceiver using library RcSwitch (+2k7 code, 460 iram)
#define USE_RF_SENSOR                            // Add support for RF sensor receiver (434MHz or 868MHz) (+0k8 code)
//  #define USE_THEO_V2                            // Add support for decoding Theo V2 sensors as documented on https://sidweb.nl using 434MHz RF sensor receiver (+1k4 code)
  #define USE_ALECTO_V2                          // Add support for decoding Alecto V2 sensors like ACH2010, WS3000 and DKW2012 using 868MHz RF sensor receiver (+1k7 code)
#define USE_HRE                                  // Add support for Badger HR-E Water Meter (+1k4 code)
//#define USE_A4988_STEPPER                        // Add support for A4988/DRV8825 stepper-motor-driver-circuit (+10k5 code)
//#define USE_THERMOSTAT                           // Add support for Thermostat
#define USE_BP1658CJ                             // Add support for BP1658CJ 5 channel led controller as used in Orein OS0100411267 Bulb
#define USE_ETHERNET                             // Add support for ethernet (+20k code)
#define USE_DISPLAY_TM1621_SONOFF                // Add support for TM1621 display driver used by Sonoff POWR3xxD and THR3xxD
#define USE_LOX_O2                               // Add support for LuminOx LOX O2 Sensor (+0k8 code)

#ifndef USE_KNX
#define USE_KNX                                  // Enable KNX IP Protocol Support (+23k code, +3k3 mem)
#endif
#define USE_DALI                                 // Add support for DALI gateway (+5k code)

#endif // FIRMWARE_TASMOTA32

/*********************************************************************************************\
 * Post-process compile options for Matter
\*********************************************************************************************/

#ifdef USE_MATTER_DEVICE
  #undef  USE_DISCOVERY
  #define USE_DISCOVERY

// Enable all the crypto required by Matter
  #undef  USE_BERRY_CRYPTO_EC_P256
  #define USE_BERRY_CRYPTO_EC_P256
  #undef  USE_BERRY_CRYPTO_HMAC_SHA256
  #define USE_BERRY_CRYPTO_HMAC_SHA256
  #undef  USE_BERRY_CRYPTO_HKDF_SHA256
  #define USE_BERRY_CRYPTO_HKDF_SHA256
  #undef  USE_BERRY_CRYPTO_AES_CCM
  #define USE_BERRY_CRYPTO_AES_CCM
  #undef  USE_BERRY_CRYPTO_AES_CTR
  #define USE_BERRY_CRYPTO_AES_CTR
  #undef  USE_BERRY_CRYPTO_PBKDF2_HMAC_SHA256
  #define USE_BERRY_CRYPTO_PBKDF2_HMAC_SHA256
  #undef  USE_BERRY_CRYPTO_SPAKE2P_MATTER
  #define USE_BERRY_CRYPTO_SPAKE2P_MATTER

#endif // USE_MATTER_DEVICE

/*********************************************************************************************\
 * Post-process for switched off Ethernet support in Arduino static libs
\*********************************************************************************************/

#ifndef CONFIG_ETH_ENABLED
  #undef USE_ETHERNET
  #ifdef FIRMWARE_MINIMAL
    #undef USE_SPI
  #endif  // FIRMWARE_MINIMAL
#endif  // CONFIG_ETH_ENABLED

#endif  // ESP32
#endif  // _TASMOTA_CONFIGURATIONS_ESP32_H_
