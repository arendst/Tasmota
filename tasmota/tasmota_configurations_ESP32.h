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

/*********************************************************************************************\
 * [tasmota32-webcam.bin]
 * Provide an image with useful supported sensors enabled
\*********************************************************************************************/

#ifdef FIRMWARE_WEBCAM

#undef CODE_IMAGE_STR
#define CODE_IMAGE_STR "webcam"

#define USE_WEBCAM
#define USE_UFILESYS
#define USE_SDCARD
  #define GUI_TRASH_FILE
#define USE_SPI
#undef  USE_MI_ESP32                             // (ESP32 only) Disable support for ESP32 as a BLE-bridge (+9k2 mem, +292k flash)
#endif  // FIRMWARE_WEBCAM

/*********************************************************************************************\
 * [tasmota32-odroidgo.bin]
 * Provide an image with useful supported sensors enabled for Odroid Go
\*********************************************************************************************/

#ifdef FIRMWARE_ODROID_GO

#undef CODE_IMAGE_STR
#define CODE_IMAGE_STR "odroid-go"

#undef MODULE
#define MODULE                 ODROID_GO         // [Module] Select default module from tasmota_template.h
#undef FALLBACK_MODULE
#define FALLBACK_MODULE        ODROID_GO         // [Module2] Select default module on fast reboot where USER_MODULE is user template

#define USE_ODROID_GO                            // Add support for Odroid Go
#define USE_UFILESYS
#define USE_SDCARD
  #define GUI_TRASH_FILE

#define USE_BERRY                                // Enable Berry scripting language
  #define USE_BERRY_PSRAM                        // Allocate Berry memory in PSRAM if PSRAM is connected - this might be slightly slower but leaves main memory intact

#define USE_ADC
#define USE_SPI
  #define USE_DISPLAY                            // Add SPI Display Support (+2k code)
    #define USE_DISPLAY_ILI9341                  // [DisplayModel 4] Enable ILI9341 Tft 480x320 display (+19k code)
//#define USE_BLE_ESP32                            // Enable new BLE driver
//#define USE_MI_ESP32                             // (ESP32 only) Add support for ESP32 as a BLE-bridge (+9k2 mem, +292k flash)
#endif  // FIRMWARE_ODROID_GO

/*********************************************************************************************\
 * [tasmota32-core2.bin]
 * Provide an image with useful supported sensors enabled for M5stack core2
\*********************************************************************************************/

#ifdef FIRMWARE_M5STACK_CORE2

#undef CODE_IMAGE_STR
#define CODE_IMAGE_STR "core2"

#undef MODULE
#define MODULE                 M5STACK_CORE2     // [Module] Select default module from tasmota_template.h
#undef FALLBACK_MODULE
#define FALLBACK_MODULE        M5STACK_CORE2     // [Module2] Select default module on fast reboot where USER_MODULE is user template

#define USE_M5STACK_CORE2                        // Add support for M5Stack Core2
  #define USE_I2S_SAY_TIME
  #define USE_I2S_WEBRADIO
#define USE_UFILESYS
#define USE_SDCARD
  #define GUI_TRASH_FILE

#define USE_BERRY                                // Enable Berry scripting language
  #define USE_BERRY_PSRAM                        // Allocate Berry memory in PSRAM if PSRAM is connected - this might be slightly slower but leaves main memory intact

#define USE_I2C
  #define USE_BMA423
  #define USE_MPU6886
#define USE_SPI
  #define USE_DISPLAY
    #define USE_DISPLAY_ILI9342
    #define JPEG_PICTS
    #define USE_FT5206
    #define USE_TOUCH_BUTTONS
      #define MAX_TOUCH_BUTTONS 16
#define USE_SENDMAIL
#define USE_ESP32MAIL

//#define USE_SCRIPT                               // Add support for script (+17k code)
// Script related defines
#ifdef USE_SCRIPT
  #undef USE_RULES
  #define MAXVARS 75
  #define MAXSVARS 15
  #define MAXFILT 10
  #define UFSYS_SIZE 8192
  #define USE_SCRIPT_TASK
  #define LARGE_ARRAYS
  #define SCRIPT_LARGE_VNBUFF
  #define USE_SCRIPT_GLOBVARS
  #define USE_SCRIPT_SUB_COMMAND
  #define USE_ANGLE_FUNC
  #define SCRIPT_FULL_WEBPAGE
  #define SCRIPT_GET_HTTPS_JP
  #define USE_GOOGLE_CHARTS
#endif  // USE_SCRIPT
#endif  // FIRMWARE_M5STACK_CORE2

/*********************************************************************************************\
 * [tasmota32-bluetooth.bin]
 * Provide an image with BLE support
\*********************************************************************************************/

#ifdef FIRMWARE_BLUETOOTH

#undef CODE_IMAGE_STR
#define CODE_IMAGE_STR "bluetooth"

#undef MODULE
#define MODULE                 WEMOS             // [Module] Select default module from tasmota_template.h
#undef FALLBACK_MODULE
#define FALLBACK_MODULE        WEMOS             // [Module2] Select default module on fast reboot where USER_MODULE is user template

#define USE_UFILESYS
#define USE_SDCARD
  #define GUI_TRASH_FILE
#define USE_ADC
#define USE_BLE_ESP32                            // Enable new BLE driver
#define USE_MI_ESP32                             // (ESP32 only) Add support for ESP32 as a BLE-bridge (+9k2 mem, +292k flash)
#endif  // FIRMWARE_BLUETOOTH

/*********************************************************************************************\
 * [tasmota32.bin]
 * Provide an image which includes KNX and Sensors
\*********************************************************************************************/

#ifdef FIRMWARE_TASMOTA32

#define USE_ENHANCED_GUI_WIFI_SCAN

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
#undef USE_PWM_DIMMER                           // Disable support for MJ-SD01/acenx/NTONPOWER PWM dimmers (+4k5 code)
#undef USE_KEELOQ                               // Disable support for Jarolift rollers by Keeloq algorithm (+4k5 code)
#undef USE_SONOFF_D1                            // Disable support for Sonoff D1 Dimmer (+0k7 code)
#undef USE_SHELLY_DIMMER                        // Disable support for Shelly Dimmer (+3k code)

#define USE_LIGHT_PALETTE                        // Add support for color palette (+0k9 code)

#define USE_HALLEFFECT                           // Add support for internal Hall Effcet sensor connected to GPIO36 and GPIO39
#define USE_DS18x20                              // Add support for DS18x20 sensors with id sort, single scan and read retry (+1k3 code)

#define USE_I2C                                  // I2C using library wire (+10k code, 0k2 mem, 124 iram)
#define USE_SHT                                // [I2cDriver8] Enable SHT1X sensor (+1k4 code)
#define USE_HTU                                // [I2cDriver9] Enable HTU21/SI7013/SI7020/SI7021 sensor (I2C address 0x40) (+1k5 code)
#define USE_BMP                                // [I2cDriver10] Enable BMP085/BMP180/BMP280/BME280 sensors (I2C addresses 0x76 and 0x77) (+4k4 code)
  #define USE_BME680                           // Enable support for BME680 sensor using Bosch BME680 library (+4k code)
#define USE_BH1750                             // [I2cDriver11] Enable BH1750 sensor (I2C address 0x23 or 0x5C) (+0k5 code)
#define USE_VEML6070                           // [I2cDriver12] Enable VEML6070 sensor (I2C addresses 0x38 and 0x39) (+1k5 code)
//#define USE_VEML6075                           // [I2cDriver49] Enable VEML6075 UVA/UVB/UVINDEX Sensor (I2C address 0x10) (+2k1 code)
//#define USE_VEML7700                           // [I2cDriver50] Enable VEML7700 Ambient Light sensor (I2C addresses 0x10) (+4k5 code)
#define USE_ADS1115                            // [I2cDriver13] Enable ADS1115 16 bit A/D converter (I2C address 0x48, 0x49, 0x4A or 0x4B) based on Adafruit ADS1x15 library (no library needed) (+0k7 code)
#define USE_INA219                             // [I2cDriver14] Enable INA219 (I2C address 0x40, 0x41 0x44 or 0x45) Low voltage and current sensor (+1k code)
//#define USE_INA226                             // [I2cDriver35] Enable INA226 (I2C address 0x40, 0x41 0x44 or 0x45) Low voltage and current sensor (+2k3 code)
#define USE_SHT3X                              // [I2cDriver15] Enable SHT3x (I2C address 0x44 or 0x45) or SHTC3 (I2C address 0x70) sensor (+0k7 code)
#define USE_TSL2561                            // [I2cDriver16] Enable TSL2561 sensor (I2C address 0x29, 0x39 or 0x49) using library Joba_Tsl2561 (+2k3 code)
//#define USE_TSL2591                            // [I2cDriver40] Enable TSL2591 sensor (I2C address 0x29) using library Adafruit_TSL2591 (+1k6 code)
#define USE_MGS                                // [I2cDriver17] Enable Xadow and Grove Mutichannel Gas sensor using library Multichannel_Gas_Sensor (+10k code)
#define USE_SGP30                              // [I2cDriver18] Enable SGP30 sensor (I2C address 0x58) (+1k1 code)
//#define USE_SI1145                             // [I2cDriver19] Enable SI1145/46/47 sensor (I2C address 0x60) (+1k code)
#define USE_LM75AD                             // [I2cDriver20] Enable LM75AD sensor (I2C addresses 0x48 - 0x4F) (+0k5 code)
//#define USE_APDS9960                           // [I2cDriver21] Enable APDS9960 Proximity Sensor (I2C address 0x39). Disables SHT and VEML6070 (+4k7 code)
//#define USE_MCP230xx                           // [I2cDriver22] Enable MCP23008/MCP23017 - Must define I2C Address in #define USE_MCP230xx_ADDR below - range 0x20 - 0x27 (+4k7 code)
//#define USE_PCA9685                            // [I2cDriver1] Enable PCA9685 I2C HW PWM Driver - Must define I2C Address in #define USE_PCA9685_ADDR below - range 0x40 - 0x47 (+1k4 code)
//#define USE_MPR121                             // [I2cDriver23] Enable MPR121 controller (I2C addresses 0x5A, 0x5B, 0x5C and 0x5D) in input mode for touch buttons (+1k3 code)
#define USE_CCS811                             // [I2cDriver24] Enable CCS811 sensor (I2C address 0x5A) (+2k2 code)
#define USE_MPU6886                              // [I2cDriver??] Enable MPU6886 6-axis MotionTracking sensor (I2C address 0x68)
//#define USE_MPU6050                            // [I2cDriver25] Enable MPU6050 sensor (I2C address 0x68 AD0 low or 0x69 AD0 high) (+3K3 of code and 188 Bytes of RAM)
//#define USE_DS3231                             // [I2cDriver26] Enable DS3231 external RTC in case no Wifi is avaliable. See docs in the source file (+1k2 code)
//#define USE_MGC3130                            // [I2cDriver27] Enable MGC3130 Electric Field Effect Sensor (I2C address 0x42) (+2k7 code, 0k3 mem)
//#define USE_MAX44009                           // [I2cDriver28] Enable MAX44009 Ambient Light sensor (I2C addresses 0x4A and 0x4B) (+0k8 code)
#define USE_SCD30                              // [I2cDriver29] Enable Sensiron SCd30 CO2 sensor (I2C address 0x61) (+3k3 code)
//#define USE_SPS30                              // [I2cDriver30] Enable Sensiron SPS30 particle sensor (I2C address 0x69) (+1.7 code)
#define USE_ADE7953                            // [I2cDriver7] Enable ADE7953 Energy monitor as used on Shelly 2.5 (I2C address 0x38) (+1k5)
//#define USE_VL53L0X                            // [I2cDriver31] Enable VL53L0x time of flight sensor (I2C address 0x29) (+4k code)
//#define USE_VL53L1X                            // [I2cDriver54] Enable VL53L1X time of flight sensor (I2C address 0x29) using Pololu VL53L1X library (+2k9 code)
//#define USE_TOF10120                           // [I2cDriver57] Enable TOF10120 time of flight sensor (I2C address 0x52) (+0k6 code)
//#define USE_MLX90614                           // [I2cDriver32] Enable MLX90614 ir temp sensor (I2C address 0x5a) (+0.6k code)
//#define USE_CHIRP                              // [I2cDriver33] Enable CHIRP soil moisture sensor (variable I2C address, default 0x20)
//#define USE_PAJ7620                            // [I2cDriver34] Enable PAJ7620 gesture sensor (I2C address 0x73) (+2.5k code)
//#define USE_PCF8574                            // [I2cDriver2] Enable PCF8574 I/O Expander (I2C addresses 0x20 - 0x26 and 0x39 - 0x3F) (+1k9 code)
#define USE_HIH6                               // [I2cDriver36] Enable Honeywell HIH Humidity and Temperature sensor (I2C address 0x27) (+0k6)
#define USE_DHT12                              // [I2cDriver41] Enable DHT12 humidity and temperature sensor (I2C address 0x5C) (+0k7 code)
#define USE_DS1624                             // [I2cDriver42] Enable DS1624, DS1621 temperature sensor (I2C addresses 0x48 - 0x4F) (+1k2 code)
//#define USE_AHT1x                              // [I2cDriver43] Enable AHT10/15 humidity and temperature sensor (I2C address 0x38, 0x39) (+0k8 code)
//  #define USE_AHT2x                              // [I2cDriver43] Enable AHT20 instead of AHT1x humidity and temperature sensor (I2C address 0x38) (+0k8 code)
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

//#define USE_SPI                                // Hardware SPI using GPIO12(MISO), GPIO13(MOSI) and GPIO14(CLK) in addition to two user selectable GPIOs(CS and DC)
//#define USE_RC522                              // Add support for MFRC522 13.56Mhz Rfid reader (+6k code)

#define USE_MHZ19                                // Add support for MH-Z19 CO2 sensor (+2k code)
#define USE_SENSEAIR                             // Add support for SenseAir K30, K70 and S8 CO2 sensor (+2k3 code)
#ifndef CO2_LOW
  #define CO2_LOW              800               // Below this CO2 value show green light (needs PWM or WS2812 RG(B) led and enable with SetOption18 1)
#endif
#ifndef CO2_HIGH
  #define CO2_HIGH             1200              // Above this CO2 value show red light (needs PWM or WS2812 RG(B) led and enable with SetOption18 1)
#endif
#define USE_PMS5003                              // Add support for PMS5003 and PMS7003 particle concentration sensor (+1k3 code)
  //#define PMS_MODEL_PMS3003                      // Enable support of PMS3003 instead of PMS5003/PMS7003 (needs the USE_PMS5003 above)
#define USE_NOVA_SDS                             // Add support for SDS011 and SDS021 particle concentration sensor (+0k7 code)
#define USE_HPMA                                 // Add support for Honeywell HPMA115S0 particle concentration sensor
#define USE_SR04                                 // Add support for HC-SR04 ultrasonic devices (+1k code)
//#define USE_DYP                                  // Add support for DYP ME-007 ultrasonic distance sensor, serial port version (+0k5 code)
#define USE_SERIAL_BRIDGE                        // Add support for software Serial Bridge (+0k8 code)
#define USE_MP3_PLAYER                           // Use of the DFPlayer Mini MP3 Player RB-DFR-562 commands: play, volume and stop
  #define MP3_VOLUME           10                // Set the startup volume on init, the range can be 0..30(max)
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

#define USE_ENERGY_SENSOR                        // Add energy sensors (-14k code)
#define USE_PZEM004T                             // Add support for PZEM004T Energy monitor (+2k code)
#define USE_PZEM_AC                              // Add support for PZEM014,016 Energy monitor (+1k1 code)
#define USE_PZEM_DC                              // Add support for PZEM003,017 Energy monitor (+1k1 code)
#define USE_MCP39F501                            // Add support for MCP39F501 Energy monitor as used in Shelly 2 (+3k1 code)
#define USE_SDM72                                // Add support for Eastron SDM72-Modbus energy monitor (+0k3 code)
#define USE_SDM120                               // Add support for Eastron SDM120-Modbus energy monitor (+1k1 code)
#define USE_SDM630                               // Add support for Eastron SDM630-Modbus energy monitor (+0k6 code)
#define USE_DDS2382                              // Add support for Hiking DDS2382 Modbus energy monitor (+0k6 code)
#define USE_DDSU666                              // Add support for Chint DDSU666 Modbus energy monitor (+0k6 code)
//#define USE_SOLAX_X1                             // Add support for Solax X1 series Modbus log info (+3k1 code)
//#define USE_LE01MR                               // Add support for F&F LE-01MR modbus energy meter (+2k code)
//#define USE_TELEINFO                             // Add support for French Energy Provider metering telemetry (+5k2 code, +168 RAM + SmartMeter LinkedList Values RAM)
//#define USE_WE517                                // Add support for Orno WE517-Modbus energy monitor (+1k code)

#define USE_DHT                                  // Add support for DHT11, AM2301 (DHT21, DHT22, AM2302, AM2321) and SI7021 Temperature and Humidity sensor
#define USE_MAX31855                             // Add support for MAX31855 K-Type thermocouple sensor using softSPI
//#define USE_MAX31865                             // Add support for MAX31865 RTD sensors using softSPI
#define USE_IR_REMOTE                            // Send IR remote commands using library IRremoteESP8266 and ArduinoJson (+4k code, 0k3 mem, 48 iram)
  #define USE_IR_RECEIVE                         // Support for IR receiver (+5k5 code, 264 iram)
#define USE_LMT01                                // Add support for TI LMT01 temperature sensor, count pulses on single GPIO (+0k5 code)
//#define USE_WIEGAND                              // Add support for 24/26/32/34 bit RFID Wiegand interface (D0/D1) (+1k7 code)
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

#ifndef USE_KNX
#define USE_KNX                                  // Enable KNX IP Protocol Support (+23k code, +3k3 mem)
#endif

#endif // FIRMWARE_TASMOTA32

#endif  // ESP32
#endif  // _TASMOTA_CONFIGURATIONS_ESP32_H_
