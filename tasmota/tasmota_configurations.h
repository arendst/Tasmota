/*
  tasmota_configurations.h - Configurations for Tasmota

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

#ifndef _TASMOTA_CONFIGURATIONS_H_
#define _TASMOTA_CONFIGURATIONS_H_

/*********************************************************************************************\
 * [tasmota-sensors.bin]
 * Provide an image with useful supported sensors enabled
\*********************************************************************************************/

#ifdef FIRMWARE_SENSORS

#undef CODE_IMAGE_STR
#define CODE_IMAGE_STR "sensors"

#undef USE_DISCOVERY                             // Disable mDNS (+8k code or +23.5k code with core 2_5_x, +0.3k mem)

// -- Optional modules ----------------------------
//#define ROTARY_V1                                // Add support for MI Desk Lamp
#define USE_SONOFF_RF                            // Add support for Sonoff Rf Bridge (+3k2 code)
  #define USE_RF_FLASH                           // Add support for flashing the EFM8BB1 chip on the Sonoff RF Bridge. C2CK must be connected to GPIO4, C2D to GPIO5 on the PCB (+2k7 code)
#define USE_SONOFF_SC                            // Add support for Sonoff Sc (+1k1 code)
#define USE_TUYA_MCU                             // Add support for Tuya Serial MCU
#ifndef TUYA_DIMMER_ID
  #define TUYA_DIMMER_ID       0                 // Default dimmer Id
#endif
#undef USE_ARMTRONIX_DIMMERS                    // Disable support for Armtronix Dimmers (+1k4 code)
#undef USE_PS_16_DZ                             // Disable support for PS-16-DZ Dimmer (+2k code)
#undef USE_SONOFF_IFAN                          // Disable support for Sonoff iFan02 and iFan03 (+2k code)
#define USE_BUZZER                               // Add support for a buzzer (+0k6 code)
#undef USE_ARILUX_RF                            // Disable support for Arilux RF remote controller (+0k8 code, 252 iram (non 2.3.0))
#undef USE_SHUTTER                              // Disable Shutter support for up to 4 shutter with different motortypes (+6k code)
#define USE_DEEPSLEEP                            // Add support for deepsleep (+1k code)
#undef USE_EXS_DIMMER                           // Disable support for EX-Store WiFi Dimmer
//#define USE_HOTPLUG                              // Add support for sensor HotPlug
//#undef USE_DEVICE_GROUPS                        // Disable support for device groups (+5k6 code)
#undef USE_PWM_DIMMER                           // Disable support for MJ-SD01/acenx/NTONPOWER PWM dimmers (+4k5 code)
#undef USE_KEELOQ                               // Disable support for Jarolift rollers by Keeloq algorithm (+4k5 code)
#undef USE_SONOFF_D1                            // DIsable support for Sonoff D1 Dimmer (+0k7 code)

// -- Optional light modules ----------------------
#define USE_LIGHT                                // Add Dimmer/Light support
#define USE_WS2812                               // WS2812 Led string using library NeoPixelBus (+5k code, +1k mem, 232 iram) - Disable by //
//  #define USE_WS2812_DMA                         // DMA supports only GPIO03 (= Serial RXD) (+1k mem). When USE_WS2812_DMA is enabled expect Exceptions on Pow
#ifndef USE_WS2812_HARDWARE
  #define USE_WS2812_HARDWARE  NEO_HW_WS2812     // Hardware type (NEO_HW_WS2812, NEO_HW_WS2812X, NEO_HW_WS2813, NEO_HW_SK6812, NEO_HW_LC8812, NEO_HW_APA106)
#endif
#ifndef USE_WS2812_CTYPE
  #define USE_WS2812_CTYPE     NEO_GRB           // Color type (NEO_RGB, NEO_GRB, NEO_BRG, NEO_RBG, NEO_RGBW, NEO_GRBW)
#endif
#define USE_MY92X1                               // Add support for MY92X1 RGBCW led controller as used in Sonoff B1, Ailight and Lohas
#define USE_SM16716                              // Add support for SM16716 RGB LED controller (+0k7 code)
#define USE_SM2135                               // Add support for SM2135 RGBCW led control as used in Action LSC (+0k6 code)
#define USE_SONOFF_L1                            // Add support for Sonoff L1 led control
#define USE_ELECTRIQ_MOODL                       // Add support for ElectriQ iQ-wifiMOODL RGBW LED controller
#define USE_LIGHT_PALETTE                        // Add support for color palette (+0k9 code)

#define USE_COUNTER                              // Enable counters
#undef USE_ADC_VCC                               // Add Analog input on selected devices
#define USE_DS18x20                              // Add support for DS18x20 sensors with id sort, single scan and read retry (+1k3 code)

#define USE_I2C                                  // I2C using library wire (+10k code, 0k2 mem, 124 iram)
#define USE_SHT                                  // Add I2C emulating code for SHT1X sensor (+1k4 code)
#define USE_HTU                                  // Add I2C code for HTU21/SI7013/SI7020/SI7021 sensor (+1k5 code)
#define USE_BMP                                  // Add I2C code for BMP085/BMP180/BMP280/BME280 sensor (+4k code)
  #define USE_BME680                             // Add additional support for BME680 sensor using Bosch BME680 library (+4k code)
#define USE_BH1750                               // Add I2C code for BH1750 sensor (+0k5 code)
#define USE_VEML6070                             // Add I2C code for VEML6070 sensor (+0k5 code)
#define USE_ADS1115                              // Add I2C code for ADS1115 16 bit A/D converter based on Adafruit ADS1x15 library (no library needed) (+0k7 code)
#define USE_INA219                               // Add I2C code for INA219 Low voltage and current sensor (+1k code)
//#define USE_INA226                               // Enable INA226 (I2C address 0x40, 0x41 0x44 or 0x45) Low voltage and current sensor (+2k3 code)
#define USE_SHT3X                                // Add I2C code for SHT3x sensor (+0k6 code)
#define USE_TSL2561                              // Add I2C code for TSL2561 sensor using library Adafruit TSL2561 Arduino (+1k2 code)
//#define USE_TSL2591                              // Add I2C code for TSL2591 sensor library Adafruit_TSL2591 (+1k6 code)
#define USE_MGS                                  // Add I2C code for Xadow and Grove Mutichannel Gas sensor using library Multichannel_Gas_Sensor (+10k code)
#define USE_SGP30                                // Add I2C code for SGP30 sensor (+1k1 code)
//#define USE_SI1145                               // Add I2C code for SI1145/46/47 sensor (+1k code)
#define USE_LM75AD                               // Add I2C code for LM75AD sensor (+0k5 code)
//#define USE_APDS9960                             // Add I2C code for APDS9960 Proximity Sensor. Disables SHT and VEML6070 (+4k7 code)
//#define USE_MCP230xx                             // Enable MCP23008/MCP23017 - Must define I2C Address in #define USE_MCP230xx_ADDR below - range 0x20 - 0x27 (+4k7 code)
//  #define USE_MCP230xx_ADDR 0x20                 // Enable MCP23008/MCP23017 I2C Address to use (Must be within range 0x20 through 0x27 - set according to your wired setup)
//  #define USE_MCP230xx_OUTPUT                    // Enable MCP23008/MCP23017 OUTPUT support through sensor29 commands (+1k5 code)
//  #define USE_MCP230xx_DISPLAYOUTPUT             // Enable MCP23008/MCP23017 to display state of OUTPUT pins on Web UI (+0k2 code)
//#define USE_PCA9685                              // Enable PCA9685 I2C HW PWM Driver - Must define I2C Address in #define USE_PCA9685_ADDR below - range 0x40 - 0x47 (+1k4 code)
//  #define USE_PCA9685_ADDR 0x40                  // Enable PCA9685 I2C Address to use (Must be within range 0x40 through 0x47 - set according to your wired setup)
//  #define USE_PCA9685_FREQ 50                    // Define default PWM frequency in Hz to be used (must be within 24 to 1526) - If other value is used, it will rever to 50Hz
//#define USE_MPR121                               // Enable MPR121 controller (I2C addresses 0x5A, 0x5B, 0x5C and 0x5D) in input mode for touch buttons (+1k3 code)
#define USE_CCS811                               // Add I2C code for CCS811 sensor (+2k2 code)
//#define USE_MPU6050                              // Enable MPU6050 sensor (I2C address 0x68 AD0 low or 0x69 AD0 high) (+2k6 code)
//#define USE_DS3231                               // Enable DS3231 external RTC in case no Wifi is avaliable. See docs in the source file (+1k2 code)
//#define USE_MGC3130                              // Enable MGC3130 Electric Field Effect Sensor (I2C address 0x42) (+2k7 code, 0k3 mem)
//#define USE_MAX44009                             // Enable MAX44009 Ambient Light sensor (I2C addresses 0x4A and 0x4B) (+0k8 code)
#define USE_SCD30                                // Enable Sensiron SCd30 CO2 sensor (I2C address 0x61) (+3k3 code)
//#define USE_SPS30                                // Enable Sensiron SPS30 particle sensor (I2C address 0x69) (+1.7 code)
#define USE_ADE7953                              // Enable ADE7953 Energy monitor as used on Shelly 2.5 (I2C address 0x38) (+1k5)
//#define USE_VL53L0X                              // Enable VL53L0x time of flight sensor (I2C address 0x29) (+4k code)
//#define USE_MLX90614                             // Enable MLX90614 ir temp sensor (I2C address 0x5a) (+0.6k code)
//#define USE_CHIRP                                // Enable CHIRP soil moisture sensor (variable I2C address, default 0x20)
//#define USE_PAJ7620                              // Enable PAJ7620 gesture sensor (I2C address 0x73) (+2.5k code)
//#define USE_PCF8574                              // Enable PCF8574 I/O Expander (I2C addresses 0x20 - 0x26 and 0x39 - 0x3F) (+1k9 code)
#define USE_HIH6                                 // Enable Honywell HIH Humidity and Temperature sensor (I2C address 0x27) (+0k6)
#define USE_DHT12                                // Add I2C code for DHT12 temperature and humidity sensor (+0k7 code)
#define USE_DS1624                               // Add I2C code for DS1624, DS1621 sensor
//#define USE_AHT1x                                // Enable AHT10/15 humidity and temperature sensor (I2C address 0x38) (+0k8 code)
#define USE_WEMOS_MOTOR_V1                       // Enable Wemos motor driver V1 (I2C addresses 0x2D - 0x30) (+0k7 code)
  #define WEMOS_MOTOR_V1_ADDR  0x30              // Default I2C address 0x30
  #define WEMOS_MOTOR_V1_FREQ  1000              // Default frequency
//#define USE_HDC1080                              // Enable HDC1080 temperature/humidity sensor
#define USE_IAQ                                  // [I2cDriver46] Enable iAQ-core air quality sensor (I2C address 0x5a) (+0k6 code)
#define USE_AS3935                               // [I2cDriver48] Enable AS3935 Franklin Lightning Sensor (I2C address 0x03) (+5k4 code)

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
#define USE_SERIAL_BRIDGE                        // Add support for software Serial Bridge (+0k8 code)
#define USE_MP3_PLAYER                           // Use of the DFPlayer Mini MP3 Player RB-DFR-562 commands: play, volume and stop
  #define MP3_VOLUME           10                // Set the startup volume on init, the range can be 0..30(max)
//#define USE_AZ7798                               // Add support for AZ-Instrument 7798 CO2 datalogger
#define USE_PN532_HSU                            // Add support for PN532 using HSU (Serial) interface (+1k8 code, 140 bytes mem)
#define USE_RDM6300                              // Add support for RDM6300 125kHz RFID Reader (+0k8)
#define USE_IBEACON                              // Add support for bluetooth LE passive scan of ibeacon devices (uses HM17 module)
//#define USE_GPS                                  // Add support for GPS and NTP Server for becoming Stratus 1 Time Source (+ 3.1kb flash, +132 bytes RAM)
#define USE_HM10                                 // (ESP8266 only) Add support for HM-10 as a BLE-bridge for the LYWSD03 (+5k1 code)
//#define USE_MI_ESP32                             // (ESP32 only) Add support for ESP32 as a BLE-bridge (+9k2 mem, +292k flash)
#define USE_HRXL                                 // Add support for MaxBotix HRXL-MaxSonar ultrasonic range finders (+0k7)

#define USE_ENERGY_SENSOR                        // Add energy sensors (-14k code)
#define USE_PZEM004T                             // Add support for PZEM004T Energy monitor (+2k code)
#define USE_PZEM_AC                              // Add support for PZEM014,016 Energy monitor (+1k1 code)
#define USE_PZEM_DC                              // Add support for PZEM003,017 Energy monitor (+1k1 code)
#define USE_MCP39F501                            // Add support for MCP39F501 Energy monitor as used in Shelly 2 (+3k1 code)
#define USE_SDM120                               // Add support for Eastron SDM120-Modbus energy monitor (+1k1 code)
#define USE_SDM630                               // Add support for Eastron SDM630-Modbus energy monitor (+0k6 code)
#define USE_DDS2382                              // Add support for Hiking DDS2382 Modbus energy monitor (+0k6 code)
#define USE_DDSU666                              // Add support for Chint DDSU666 Modbus energy monitor (+0k6 code)
//#define USE_SOLAX_X1                             // Add support for Solax X1 series Modbus log info (+3k1 code)
//#define USE_LE01MR                               // Add support for F&F LE-01MR modbus energy meter (+2k code)

#define USE_DHT                                  // Add support for DHT11, AM2301 (DHT21, DHT22, AM2302, AM2321) and SI7021 Temperature and Humidity sensor
#define USE_MAX31855                             // Add support for MAX31855 K-Type thermocouple sensor using softSPI
//#define USE_MAX31865                             // Add support for MAX31865 RTD sensors using softSPI
#define USE_IR_REMOTE                            // Send IR remote commands using library IRremoteESP8266 and ArduinoJson (+4k code, 0k3 mem, 48 iram)
  #define USE_IR_RECEIVE                         // Support for IR receiver (+5k5 code, 264 iram)

//#define USE_ZIGBEE                               // Enable serial communication with Zigbee CC2530 flashed with ZNP

#define USE_SR04                                 // Add support for HC-SR04 ultrasonic devices (+1k code)
#define USE_TM1638                               // Add support for TM1638 switches copying Switch1 .. Switch8 (+1k code)
#define USE_HX711                                // Add support for HX711 load cell (+1k5 code)
//#define USE_HX711_GUI                            // Add optional web GUI to HX711 as scale (+1k8 code)
//#define USE_TX20_WIND_SENSOR                     // Add support for La Crosse TX20 anemometer (+2k6/0k8 code)
//#define USE_TX23_WIND_SENSOR                     // Add support for La Crosse TX23 anemometer (+2k7/1k code)
#define USE_RC_SWITCH                            // Add support for RF transceiver using library RcSwitch (+2k7 code, 460 iram)
#define USE_RF_SENSOR                            // Add support for RF sensor receiver (434MHz or 868MHz) (+0k8 code)
//  #define USE_THEO_V2                            // Add support for decoding Theo V2 sensors as documented on https://sidweb.nl using 434MHz RF sensor receiver (+1k4 code)
  #define USE_ALECTO_V2                          // Add support for decoding Alecto V2 sensors like ACH2010, WS3000 and DKW2012 using 868MHz RF sensor receiver (+1k7 code)
#define USE_HRE                                  // Add support for Badger HR-E Water Meter (+1k4 code)
//#define USE_A4988_STEPPER                        // Add support for A4988/DRV8825 stepper-motor-driver-circuit (+10k5 code)
//#define USE_ARDUINO_SLAVE                        // Add support for Arduino Uno/Pro Mini via serial interface including flashing (+2k3 code, 44 mem)
//#define USE_WINDMETER                            // Add support for analog anemometer
#undef DEBUG_THEO                                // Disable debug code
#undef USE_DEBUG_DRIVER                          // Disable debug code
#endif  // FIRMWARE_SENSORS

/*********************************************************************************************\
 * [tasmota-knx.bin]
 * Provide a dedicated KNX image allowing enough code and memory space
\*********************************************************************************************/

#ifdef FIRMWARE_KNX_NO_EMULATION

#undef CODE_IMAGE_STR
#define CODE_IMAGE_STR "knx"

#ifndef USE_KNX
#define USE_KNX                                  // Enable KNX IP Protocol Support (+23k code, +3k3 mem)
#endif
#undef USE_EMULATION                             // Disable Belkin WeMo and Hue Bridge emulation for Alexa (-16k code, -2k mem)
#undef USE_EMULATION_HUE                         // Disable Hue Bridge emulation for Alexa (+14k code, +2k mem common)
#undef USE_EMULATION_WEMO                        // Disable Belkin WeMo emulation for Alexa (+6k code, +2k mem common)
#undef USE_DEEPSLEEP                             // Disable support for deepsleep (+1k code)
#undef USE_DEVICE_GROUPS                         // Disable support for device groups (+3k5 code)
#undef USE_PWM_DIMMER_REMOTE                     // Disbale support for remote switches to PWM Dimmer
#undef DEBUG_THEO                                // Disable debug code
#undef USE_DEBUG_DRIVER                          // Disable debug code
#endif  // FIRMWARE_KNX_NO_EMULATION

/*********************************************************************************************\
 * [tasmota-display.bin]
 * Provide an image with display drivers enabled
\*********************************************************************************************/

#ifdef FIRMWARE_DISPLAYS

#undef CODE_IMAGE_STR
#define CODE_IMAGE_STR "display"

#undef USE_EMULATION                             // Disable Belkin WeMo and Hue Bridge emulation for Alexa (-16k code, -2k mem)
#undef USE_EMULATION_HUE                         // Disable Hue Bridge emulation for Alexa (+14k code, +2k mem common)
#undef USE_EMULATION_WEMO                        // Disable Belkin WeMo emulation for Alexa (+6k code, +2k mem common)
#undef USE_DOMOTICZ                              // Disable Domoticz
#undef USE_HOME_ASSISTANT                        // Disable Home Assistant

// -- Optional modules ----------------------------
#undef ROTARY_V1                                 // Disable support for MI Desk Lamp
#undef USE_SONOFF_RF                             // Disable support for Sonoff Rf Bridge (+3k2 code)
  #undef USE_RF_FLASH                            // Disable support for flashing the EFM8BB1 chip on the Sonoff RF Bridge. C2CK must be connected to GPIO4, C2D to GPIO5 on the PCB
#undef USE_SONOFF_SC                             // Disable support for Sonoff Sc (+1k1 code)
//#undef USE_TUYA_MCU                              // Disable support for Tuya Serial MCU
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

#undef USE_ENERGY_SENSOR                         // Disable energy sensors (-14k code)
  #undef USE_PZEM004T                            // Disable PZEM004T energy sensor
  #undef USE_PZEM_AC                             // Disable PZEM014,016 Energy monitor
  #undef USE_PZEM_DC                             // Disable PZEM003,017 Energy monitor
  #undef USE_MCP39F501                           // Disable MCP39F501 Energy monitor as used in Shelly 2
  #undef USE_SDM120                              // Disable support for Eastron SDM120-Modbus energy meter
  #undef USE_SDM630                              // Disable support for Eastron SDM630-Modbus energy monitor (+0k6 code)
  #undef USE_DDS2382                             // Disable support for Hiking DDS2382 Modbus energy monitor (+0k6 code)
  #undef USE_DDSU666                             // Disable support for Chint DDSU666 Modbus energy monitor (+0k6 code)
  #undef USE_SOLAX_X1                            // Disable support for Solax X1 series Modbus log info (+3k1 code)
  #undef USE_LE01MR                              // Disable support for F&F LE-01MR Modbus energy meter (+2k code)

#define USE_I2C                                  // I2C using library wire (+10k code, 0k2 mem, 124 iram)
  #define USE_DISPLAY                            // Add I2C Display Support (+2k code)
    #define USE_DISPLAY_MODES1TO5                // Enable display mode 1 to 5 in addition to mode 0
    #define USE_DISPLAY_LCD                      // [DisplayModel 1] Enable Lcd display (I2C addresses 0x27 and 0x3F) (+6k code)
    #define USE_DISPLAY_SSD1306                  // [DisplayModel 2] Enable SSD1306 Oled 128x64 display (I2C addresses 0x3C and 0x3D) (+16k code)
    #define USE_DISPLAY_MATRIX                   // [DisplayModel 3] Enable 8x8 Matrix display (I2C adresseses see below) (+11k code)
    #define USE_DISPLAY_SEVENSEG                 // [DisplayModel 11] [I2cDriver47] Enable sevenseg display (I2C addresses 0x70 - 0x77) (<+11k code)
    #define USE_DISPLAY_SH1106                   // [DisplayModel 7] Enable SH1106 Oled 128x64 display (I2C addresses 0x3C and 0x3D)

#define USE_SPI                                  // Hardware SPI using GPIO12(MISO), GPIO13(MOSI) and GPIO14(CLK) in addition to two user selectable GPIOs(CS and DC)
    #define USE_DISPLAY_ILI9341                  // [DisplayModel 4] Enable ILI9341 Tft 480x320 display (+19k code)
#ifndef ARDUINO_ESP8266_RELEASE_2_3_0            // There is not enough spare RAM with core 2.3.0 to support the following
    #define USE_DISPLAY_EPAPER_29                // [DisplayModel 5] Enable e-paper 2.9 inch display (+19k code)
    #define USE_DISPLAY_EPAPER_42                // [DisplayModel 6] Enable e-paper 4.2 inch display
//    #define USE_DISPLAY_ILI9488                  // [DisplayModel 8]
//    #define USE_DISPLAY_SSD1351                  // [DisplayModel 9]
//    #define USE_DISPLAY_RA8876                   // [DisplayModel 10]
#endif

#undef DEBUG_THEO                                // Disable debug code
#undef USE_DEBUG_DRIVER                          // Disable debug code
#endif  // FIRMWARE_DISPLAYS

/*********************************************************************************************\
 * [tasmota-ir.bin]
 * Provide a dedicated image with IR full protocol support, with limited additional features
\*********************************************************************************************/

#ifdef FIRMWARE_IR

#undef CODE_IMAGE_STR
#define CODE_IMAGE_STR "ir"

#undef USE_EMULATION
#undef USE_EMULATION_HUE                         // Disable Hue emulation - only for lights and relays
#undef USE_EMULATION_WEMO                        // Disable Wemo emulation - only for relays

//#undef USE_DOMOTICZ                              // Disable Domoticz
//#undef USE_HOME_ASSISTANT                        // Disable Home Assistant
//#undef USE_KNX                                   // Disable KNX IP Protocol Support
//#undef USE_CUSTOM                                // Disable Custom features
//#undef USE_TIMERS                                // Disable support for up to 16 timers
//#undef USE_TIMERS_WEB                            // Disable support for timer webpage
//#undef USE_SUNRISE                               // Disable support for Sunrise and sunset tools
//#undef USE_RULES                                 // Disable support for rules
#undef USE_DISCOVERY                             // Disable mDNS for the following services (+8k code or +23.5k code with core 2_5_x, +0.3k mem)

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

// -- Optional light modules ----------------------
//#undef USE_LIGHT                                 // Also disable all Dimmer/Light support
#undef USE_WS2812                                // Disable WS2812 Led string using library NeoPixelBus (+5k code, +1k mem, 232 iram) - Disable by //
#undef USE_MY92X1                                // Disable support for MY92X1 RGBCW led controller as used in Sonoff B1, Ailight and Lohas
#undef USE_SM16716                               // Disable support for SM16716 RGB LED controller (+0k7 code)
#undef USE_SM2135                                // Disable support for SM2135 RGBCW led control as used in Action LSC (+0k6 code)
#undef USE_SONOFF_L1                             // Disable support for Sonoff L1 led control
#undef USE_ELECTRIQ_MOODL                        // Disable support for ElectriQ iQ-wifiMOODL RGBW LED controller
#undef USE_LIGHT_PALETTE                         // Disable support for color palette (+0k9 code)

#undef USE_ENERGY_SENSOR                         // Disable energy sensors (-14k code)
  #undef USE_PZEM004T                            // Disable PZEM004T energy sensor
  #undef USE_PZEM_AC                             // Disable PZEM014,016 Energy monitor
  #undef USE_PZEM_DC                             // Disable PZEM003,017 Energy monitor
  #undef USE_MCP39F501                           // Disable MCP39F501 Energy monitor as used in Shelly 2
  #undef USE_SDM120                              // Disable support for Eastron SDM120-Modbus energy meter
  #undef USE_SDM630                              // Disable support for Eastron SDM630-Modbus energy monitor (+0k6 code)
  #undef USE_DDS2382                             // Disable support for Hiking DDS2382 Modbus energy monitor (+0k6 code)
  #undef USE_DDSU666                             // Disable support for Chint DDSU666 Modbus energy monitor (+0k6 code)
  #undef USE_SOLAX_X1                            // Disable support for Solax X1 series Modbus log info (+3k1 code)
  #undef USE_LE01MR                              // Disable support for F&F LE-01MR Modbus energy meter (+2k code)

//#undef USE_DS18x20                               // Disable support for DS18x20 sensors with id sort, single scan and read retry (+1k3 code)

#undef USE_I2C                                   // Disable all I2C sensors
#undef USE_SPI                                   // Disable all SPI devices

#undef USE_DISPLAY                               // Disable support for displays

#undef USE_MHZ19                                 // Disable support for MH-Z19 CO2 sensor
#undef USE_SENSEAIR                              // Disable support for SenseAir K30, K70 and S8 CO2 sensor
#undef USE_PMS5003                               // Disable support for PMS5003 and PMS7003 particle concentration sensor
#undef USE_NOVA_SDS                              // Disable support for SDS011 and SDS021 particle concentration sensor
#undef USE_HPMA                                  // Disable support for Honeywell HPMA115S0 particle concentration sensor
#undef USE_SERIAL_BRIDGE                         // Disable support for software Serial Bridge
#undef USE_MP3_PLAYER                            // Disable DFPlayer Mini MP3 Player RB-DFR-562 commands: play, volume and stop
#undef USE_AZ7798                                // Disable support for AZ-Instrument 7798 CO2 datalogger
#undef USE_PN532_HSU                             // Disable support for PN532 using HSU (Serial) interface (+1k8 code, 140 bytes mem)
#undef USE_RDM6300                               // Disable support for RDM6300 125kHz RFID Reader (+0k8)
#undef USE_IBEACON                               // Disable support for bluetooth LE passive scan of ibeacon devices (uses HM17 module)
#undef USE_GPS                                   // Disable support for GPS and NTP Server for becoming Stratus 1 Time Source (+ 3.1kb flash, +132 bytes RAM)
#undef USE_HM10                                  // (ESP8266 only) Disable support for HM-10 as a BLE-bridge for the LYWSD03 (+5k1 code)
#undef USE_MI_ESP32                              // (ESP32 only) Disable support for ESP32 as a BLE-bridge (+9k2 mem, +292k flash)
#undef USE_HRXL                                  // Disable support for MaxBotix HRXL-MaxSonar ultrasonic range finders (+0k7)

//#define USE_DHT                                  // Add support for DHT11, AM2301 (DHT21, DHT22, AM2302, AM2321) and SI7021 Temperature and Humidity sensor
#undef USE_MAX31855                              // Disable MAX31855 K-Type thermocouple sensor using softSPI
#undef USE_MAX31865                              // Disable support for MAX31865 RTD sensors using softSPI
#undef USE_SR04                                  // Disable support for for HC-SR04 ultrasonic devices
#undef USE_TM1638                                // Disable support for TM1638 switches copying Switch1 .. Switch8
#undef USE_HX711                                 // Disable support for HX711 load cell
#undef USE_TX20_WIND_SENSOR                      // Disable support for La Crosse TX20 anemometer
#undef USE_TX23_WIND_SENSOR                      // Disable support for La Crosse TX23 anemometer
#undef USE_RC_SWITCH                             // Disable support for RF transceiver using library RcSwitch
#undef USE_RF_SENSOR                             // Disable support for RF sensor receiver (434MHz or 868MHz) (+0k8 code)
#undef USE_HRE                                   // Disable support for Badger HR-E Water Meter (+1k4 code)
#undef USE_A4988_STEPPER                         // Disable support for A4988_Stepper
#undef USE_ARDUINO_SLAVE                         // Disable support for Arduino Uno/Pro Mini via serial interface including flashing (+2k3 code, 44 mem)
#undef DEBUG_THEO                                // Disable debug code
#undef USE_DEBUG_DRIVER                          // Disable debug code
#endif  // FIRMWARE_IR

/*********************************************************************************************\
 * [tasmota-lite.bin]
 * Provide an image without sensors
\*********************************************************************************************/

#ifdef FIRMWARE_LITE

#undef CODE_IMAGE_STR
#define CODE_IMAGE_STR "lite"

#undef APP_SLEEP
#define APP_SLEEP 1                              // Default to sleep = 1 for FIRMWARE_LITE

#undef USE_ARDUINO_OTA                           // Disable support for Arduino OTA
#undef USE_DOMOTICZ                              // Disable Domoticz
#undef USE_HOME_ASSISTANT                        // Disable Home Assistant
#undef USE_MQTT_TLS                              // Disable TLS support won't work as the MQTTHost is not set
#undef USE_KNX                                   // Disable KNX IP Protocol Support
//#undef USE_WEBSERVER                             // Disable Webserver
#undef USE_WEBSEND_RESPONSE                      // Disable command WebSend response message (+1k code)
//#undef USE_EMULATION                             // Disable Wemo or Hue emulation
//#undef USE_EMULATION_HUE                         // Disable Hue Bridge emulation for Alexa (+14k code, +2k mem common)
//#undef USE_EMULATION_WEMO                        // Disable Belkin WeMo emulation for Alexa (+6k code, +2k mem common)
#undef USE_CUSTOM                                // Disable Custom features
#undef USE_DISCOVERY                             // Disable Discovery services for both MQTT and web server
//#undef USE_TIMERS                                // Disable support for up to 16 timers
//#undef USE_TIMERS_WEB                            // Disable support for timer webpage
//#undef USE_SUNRISE                               // Disable support for Sunrise and sunset tools
//#undef USE_RULES                                 // Disable support for rules
#undef USE_SCRIPT                                  // Add support for script (+17k code)

// -- Optional modules -------------------------
#undef ROTARY_V1                                 // Disable support for MI Desk Lamp
#undef USE_SONOFF_RF                             // Disable support for Sonoff Rf Bridge (+3k2 code)
  #undef USE_RF_FLASH                            // Disable support for flashing the EFM8BB1 chip on the Sonoff RF Bridge. C2CK must be connected to GPIO4, C2D to GPIO5 on the PCB
#undef USE_SONOFF_SC                             // Disable support for Sonoff Sc (+1k1 code)
//#undef USE_TUYA_MCU                              // Disable support for Tuya Serial MCU
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
//#undef USE_SONOFF_D1                             // Disable support for Sonoff D1 Dimmer (+0k7 code)

// -- Optional light modules ----------------------
//#undef USE_LIGHT                                 // Also disable all Dimmer/Light support
#undef USE_WS2812                                // Disable WS2812 Led string using library NeoPixelBus (+5k code, +1k mem, 232 iram) - Disable by //
#undef USE_MY92X1                                // Disable support for MY92X1 RGBCW led controller as used in Sonoff B1, Ailight and Lohas
#undef USE_SM16716                               // Disable support for SM16716 RGB LED controller (+0k7 code)
#undef USE_SM2135                                // Disable support for SM2135 RGBCW led control as used in Action LSC (+0k6 code)
#undef USE_SONOFF_L1                             // Disable support for Sonoff L1 led control
#undef USE_ELECTRIQ_MOODL                        // Disable support for ElectriQ iQ-wifiMOODL RGBW LED controller
#undef USE_LIGHT_PALETTE                         // Disable support for color palette (+0k9 code)

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
#undef USE_SERIAL_BRIDGE                         // Disable support for software Serial Bridge
#undef USE_MP3_PLAYER                            // Disable DFPlayer Mini MP3 Player RB-DFR-562 commands: play, volume and stop
#undef USE_AZ7798                                // Disable support for AZ-Instrument 7798 CO2 datalogger
#undef USE_PN532_HSU                             // Disable support for PN532 using HSU (Serial) interface (+1k8 code, 140 bytes mem)
#undef USE_RDM6300                               // Disable support for RDM6300 125kHz RFID Reader (+0k8)
#undef USE_IBEACON                               // Disable support for bluetooth LE passive scan of ibeacon devices (uses HM17 module)
#undef USE_GPS                                   // Disable support for GPS and NTP Server for becoming Stratus 1 Time Source (+ 3.1kb flash, +132 bytes RAM)
#undef USE_HM10                                  // (ESP8266 only) Disable support for HM-10 as a BLE-bridge for the LYWSD03 (+5k1 code)
#undef USE_MI_ESP32                              // (ESP32 only) Disable support for ESP32 as a BLE-bridge (+9k2 mem, +292k flash)
#undef USE_HRXL                                  // Disable support for MaxBotix HRXL-MaxSonar ultrasonic range finders (+0k7)

//#undef USE_ENERGY_SENSOR                         // Disable energy sensors
#undef USE_PZEM004T                              // Disable PZEM004T energy sensor
#undef USE_PZEM_AC                               // Disable PZEM014,016 Energy monitor
#undef USE_PZEM_DC                               // Disable PZEM003,017 Energy monitor
//#undef USE_MCP39F501                             // Disable MCP39F501 Energy monitor as used in Shelly 2
#undef USE_SDM120                                // Disable support for Eastron SDM120-Modbus energy meter
#undef USE_SDM630                                // Disable support for Eastron SDM630-Modbus energy monitor (+0k6 code)
#undef USE_DDS2382                               // Disable support for Hiking DDS2382 Modbus energy monitor (+0k6 code)
#undef USE_DDSU666                               // Disable support for Chint DDSU666 Modbus energy monitor (+0k6 code)
#undef USE_SOLAX_X1                              // Disable support for Solax X1 series Modbus log info (+3k1 code)
#undef USE_LE01MR                                // Disable support for F&F LE-01MR Modbus energy meter (+2k code)

#undef USE_DHT                                   // Disable support for DHT11, AM2301 (DHT21, DHT22, AM2302, AM2321) and SI7021 Temperature and Humidity sensor
#undef USE_MAX31855                              // Disable MAX31855 K-Type thermocouple sensor using softSPI
#undef USE_MAX31865                              // Disable support for MAX31865 RTD sensors using softSPI
#undef USE_IR_REMOTE                             // Disable IR driver

#undef USE_ZIGBEE                                // Disable serial communication with Zigbee CC2530 flashed with ZNP

#undef USE_SR04                                  // Disable support for for HC-SR04 ultrasonic devices
#undef USE_TM1638                                // Disable support for TM1638 switches copying Switch1 .. Switch8
#undef USE_HX711                                 // Disable support for HX711 load cell
#undef USE_TX20_WIND_SENSOR                      // Disable support for La Crosse TX20 anemometer
#undef USE_TX23_WIND_SENSOR                      // Disable support for La Crosse TX23 anemometer
#undef USE_RC_SWITCH                             // Disable support for RF transceiver using library RcSwitch
#undef USE_RF_SENSOR                             // Disable support for RF sensor receiver (434MHz or 868MHz) (+0k8 code)
#undef USE_HRE                                   // Disable support for Badger HR-E Water Meter (+1k4 code)
#undef USE_A4988_STEPPER                         // Disable support for A4988_Stepper
#undef USE_ARDUINO_SLAVE                         // Disable support for Arduino Uno/Pro Mini via serial interface including flashing (+2k3 code, 44 mem)
#undef DEBUG_THEO                                // Disable debug code
#undef USE_DEBUG_DRIVER                          // Disable debug code
#endif  // FIRMWARE_BASIC

/*********************************************************************************************\
 * [tasmota-minimal.bin]
 * Provide the smallest image possible while still enabling a webserver for intermediate image load
\*********************************************************************************************/

#ifdef FIRMWARE_MINIMAL

#undef CODE_IMAGE_STR
#define CODE_IMAGE_STR "minimal"

#undef FIRMWARE_LITE                            // Disable tasmota-lite with no sensors
#undef FIRMWARE_SENSORS                         // Disable tasmota-sensors with useful sensors enabled
#undef FIRMWARE_KNX_NO_EMULATION                // Disable tasmota-knx with KNX but without Emulation
#undef FIRMWARE_DISPLAYS                        // Disable tasmota-display with display drivers enabled
#undef FIRMWARE_IR                              // Disable tasmota-ir with IR full protocols activated
#undef FIRMWARE_IR_CUSTOM                       // Disable tasmota customizable with special marker to add all IR protocols

#undef USE_ARDUINO_OTA                           // Disable support for Arduino OTA
#undef USE_DOMOTICZ                              // Disable Domoticz
#undef USE_HOME_ASSISTANT                        // Disable Home Assistant
//#undef USE_MQTT_TLS                              // Disable TLS support won't work as the MQTTHost is not set
#undef USE_KNX                                   // Disable KNX IP Protocol Support
//#undef USE_WEBSERVER                             // Disable Webserver
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

// -- Optional light modules ----------------------
#undef USE_LIGHT                                 // Disable support for lights
#undef USE_WS2812                                // Disable WS2812 Led string using library NeoPixelBus (+5k code, +1k mem, 232 iram) - Disable by //
#undef USE_MY92X1                                // Disable support for MY92X1 RGBCW led controller as used in Sonoff B1, Ailight and Lohas
#undef USE_SM16716                               // Disable support for SM16716 RGB LED controller (+0k7 code)
#undef USE_SM2135                                // Disable support for SM2135 RGBCW led control as used in Action LSC (+0k6 code)
#undef USE_SONOFF_L1                             // Disable support for Sonoff L1 led control
#undef USE_ELECTRIQ_MOODL                        // Disable support for ElectriQ iQ-wifiMOODL RGBW LED controller
#undef USE_LIGHT_PALETTE                         // Disable support for color palette (+0k9 code)

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
#undef USE_SERIAL_BRIDGE                         // Disable support for software Serial Bridge
#undef USE_MP3_PLAYER                            // Disable DFPlayer Mini MP3 Player RB-DFR-562 commands: play, volume and stop
#undef USE_AZ7798                                // Disable support for AZ-Instrument 7798 CO2 datalogger
#undef USE_PN532_HSU                             // Disable support for PN532 using HSU (Serial) interface (+1k8 code, 140 bytes mem)
#undef USE_RDM6300                               // Disable support for RDM6300 125kHz RFID Reader (+0k8)
#undef USE_IBEACON                               // Disable support for bluetooth LE passive scan of ibeacon devices (uses HM17 module)
#undef USE_GPS                                   // Disable support for GPS and NTP Server for becoming Stratus 1 Time Source (+ 3.1kb flash, +132 bytes RAM)
#undef USE_HM10                                  // (ESP8266 only) Disable support for HM-10 as a BLE-bridge for the LYWSD03 (+5k1 code)
#undef USE_MI_ESP32                              // (ESP32 only) Disable support for ESP32 as a BLE-bridge (+9k2 mem, +292k flash)
#undef USE_HRXL                                  // Disable support for MaxBotix HRXL-MaxSonar ultrasonic range finders (+0k7)

#undef USE_ENERGY_SENSOR                         // Disable energy sensors
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

#undef USE_DHT                                   // Disable support for DHT11, AM2301 (DHT21, DHT22, AM2302, AM2321) and SI7021 Temperature and Humidity sensor
#undef USE_MAX31855                              // Disable MAX31855 K-Type thermocouple sensor using softSPI
#undef USE_MAX31865                              // Disable support for MAX31865 RTD sensors using softSPI
#undef USE_IR_REMOTE                             // Disable IR driver
#undef USE_SR04                                  // Disable support for for HC-SR04 ultrasonic devices
#undef USE_TM1638                                // Disable support for TM1638 switches copying Switch1 .. Switch8
#undef USE_HX711                                 // Disable support for HX711 load cell
#undef USE_TX20_WIND_SENSOR                      // Disable support for La Crosse TX20 anemometer
#undef USE_TX23_WIND_SENSOR                      // Disable support for La Crosse TX23 anemometer
#undef USE_RC_SWITCH                             // Disable support for RF transceiver using library RcSwitch
#undef USE_RF_SENSOR                             // Disable support for RF sensor receiver (434MHz or 868MHz) (+0k8 code)
#undef USE_HRE                                   // Disable support for Badger HR-E Water Meter (+1k4 code)
#undef USE_A4988_STEPPER                         // Disable support for A4988_Stepper
#undef USE_ARDUINO_SLAVE                         // Disable support for Arduino Uno/Pro Mini via serial interface including flashing (+2k3 code, 44 mem)
#undef DEBUG_THEO                                // Disable debug code
#undef USE_DEBUG_DRIVER                          // Disable debug code
#endif  // FIRMWARE_MINIMAL

#endif  // _TASMOTA_CONFIGURATIONS_H_
