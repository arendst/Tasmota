/*
  tasmota_configurations.h - Configurations for Tasmota

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

#ifndef _TASMOTA_CONFIGURATIONS_H_
#define _TASMOTA_CONFIGURATIONS_H_

/*********************************************************************************************\
 * [tasmota-sensors.bin]
 * Provide an image with useful supported sensors enabled
\*********************************************************************************************/

#ifdef FIRMWARE_SENSORS

#ifndef CODE_IMAGE_STR
  #define CODE_IMAGE_STR "sensors"
#endif

#undef USE_DISCOVERY                             // Disable mDNS (+8k code or +23.5k code with core 2_5_x, +0.3k mem)

// -- Optional modules ----------------------------
#define ROTARY_V1                                // Add support for Rotary Encoder as used in MI Desk Lamp
#define USE_SONOFF_RF                            // Add support for Sonoff Rf Bridge (+3k2 code)
  #define USE_RF_FLASH                           // Add support for flashing the EFM8BB1 chip on the Sonoff RF Bridge. C2CK must be connected to GPIO4, C2D to GPIO5 on the PCB (+2k7 code)
#define USE_SONOFF_SC                            // Add support for Sonoff Sc (+1k1 code)
#define USE_TUYA_MCU                             // Add support for Tuya Serial MCU
#ifndef TUYA_DIMMER_ID
  #define TUYA_DIMMER_ID       0                 // Default dimmer Id
#endif
#define USE_TUYAMCUBR
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
#undef USE_SONOFF_D1                            // Disable support for Sonoff D1 Dimmer (+0k7 code)
#undef USE_SHELLY_DIMMER                        // Disable support for Shelly Dimmer (+3k code)

// -- Optional light modules ----------------------
#define USE_LIGHT                                // Add Dimmer/Light support
#define USE_WS2812                               // WS2812 Led string using library NeoPixelBus (+5k code, +1k mem, 232 iram) - Disable by //
//  #define USE_WS2812_DMA                         // DMA supports only GPIO03 (= Serial RXD) (+1k mem). When USE_WS2812_DMA is enabled expect Exceptions on Pow
#ifndef USE_WS2812_HARDWARE
  #define USE_WS2812_HARDWARE  NEO_HW_WS2812     // Hardware type (NEO_HW_WS2812, NEO_HW_WS2812X, NEO_HW_WS2813, NEO_HW_SK6812, NEO_HW_LC8812, NEO_HW_APA106, NEO_HW_P9813)
#endif
#ifndef USE_WS2812_CTYPE
  #define USE_WS2812_CTYPE     NEO_GRB           // Color type (NEO_RGB, NEO_GRB, NEO_BRG, NEO_RBG, NEO_RGBW, NEO_GRBW)
#endif
#define USE_MY92X1                               // Add support for MY92X1 RGBCW led controller as used in Sonoff B1, Ailight and Lohas
#define USE_SM16716                              // Add support for SM16716 RGB LED controller (+0k7 code)
#define USE_SM2135                               // Add support for SM2135 RGBCW led control as used in Action LSC (+0k6 code)
#define USE_SM2335                               // Add support for SM2335 RGBCW led control as used in Switchbot Bulb
#define USE_BP5758D                              // Add support for BP5758D RGBCW led control as used in some Tuya lightbulbs (+0k8 code)
#define USE_SONOFF_L1                            // Add support for Sonoff L1 led control
#define USE_ELECTRIQ_MOODL                       // Add support for ElectriQ iQ-wifiMOODL RGBW LED controller
#define USE_LIGHT_PALETTE                        // Add support for color palette (+0k9 code)

#define USE_COUNTER                              // Enable counters
#undef USE_ADC_VCC                               // Add Analog input on selected devices
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
//#define USE_SGP4X                              // [I2cDriver82] Enable SGP41 sensor (I2C address 0x59) (+7k2 code)
#define USE_SEN5X                              // [I2cDriver76] Enable SEN5X sensor (I2C address 0x69) (+3k code)
//#define USE_SI1145                             // [I2cDriver19] Enable SI1145/46/47 sensor (I2C address 0x60) (+1k code)
#define USE_LM75AD                             // [I2cDriver20] Enable LM75AD sensor (I2C addresses 0x48 - 0x4F) (+0k5 code)
//#define USE_APDS9960                           // [I2cDriver21] Enable APDS9960 Proximity Sensor (I2C address 0x39). Disables SHT and VEML6070 (+4k7 code)
//#define USE_MCP230xx                           // [I2cDriver22] Enable MCP23008/MCP23017 - Must define I2C Address in #define USE_MCP230xx_ADDR below - range 0x20 - 0x27 (+4k7 code)
//#define USE_MCP23XXX_DRV                       // [I2cDriver77] Enable MCP23xxx support as virtual switch/button/relay (+3k(I2C)/+5k(SPI) code)
//#define USE_PCA9685                            // [I2cDriver1] Enable PCA9685 I2C HW PWM Driver - Must define I2C Address in #define USE_PCA9685_ADDR below - range 0x40 - 0x47 (+1k4 code)
//#define USE_PCA9685_V2                         // [I2cDriver1] Enable PCA9685 I2C HW PWM Driver - Must define I2C Address in #define USE_PCA9685_ADDR below - range 0x40 - 0x47 (+3k4 code)
//#define USE_MPR121                             // [I2cDriver23] Enable MPR121 controller (I2C addresses 0x5A, 0x5B, 0x5C and 0x5D) in input mode for touch buttons (+1k3 code)
#define USE_CCS811                             // [I2cDriver24] Enable CCS811 sensor (I2C address 0x5A) (+2k2 code)
//#define USE_CCS811_V2                          // [I2cDriver24] Enable CCS811 sensor (I2C addresses 0x5A and 0x5B) (+2k8 code)
//#define USE_ENS16x                             // [I2cDriver85] Enable ENS160 or ENS161 sensor (I2C addresses 0x52 and 0x53) (+3k1 of code and 524 of RAM)
//#define USE_ENS210                             // [I2cDriver86] Enable ENS210 sensor (I2C addresses 0x43 and 0x44) (+4k0 of code and 944 of RAM)
//#define USE_MPU6050                            // [I2cDriver25] Enable MPU6050 sensor (I2C address 0x68 AD0 low or 0x69 AD0 high) (+3K3 of code and 188 Bytes of RAM)
//#define USE_MGC3130                            // [I2cDriver27] Enable MGC3130 Electric Field Effect Sensor (I2C address 0x42) (+2k7 code, 0k3 mem)
//#define USE_MAX44009                           // [I2cDriver28] Enable MAX44009 Ambient Light sensor (I2C addresses 0x4A and 0x4B) (+0k8 code)
#define USE_SCD30                              // [I2cDriver29] Enable Sensiron SCd30 CO2 sensor (I2C address 0x61) (+3k3 code)
//#define USE_SCD40                              // [I2cDriver62] Enable Sensiron SCd40 CO2 sensor (I2C address 0x62) (+3k5 code)
//#define USE_SPS30                              // [I2cDriver30] Enable Sensiron SPS30 particle sensor (I2C address 0x69) (+1.7 code)
//#define USE_ADE7880                            // [I2cDriver65] Enable ADE7880 Energy monitor as used on Shelly 3EM (I2C address 0x38) (+3k8)
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
//#define USE_MPU_ACCEL                          // [I2cDriver58] Enable MPU6886/MPU9250 - found in M5Stack - support both I2C buses on ESP32 (I2C address 0x68) (+2k code)
//#define USE_AM2320                             // [I2cDriver60] Enable AM2320 temperature and humidity Sensor (I2C address 0x5C) (+1k code)
//#define USE_T67XX                              // [I2cDriver61] Enable Telaire T67XX CO2 sensor (I2C address 0x15) (+1k3 code)
//#define USE_HDC2010                            // [I2cDriver64] Enable HDC2010 temperature/humidity sensor (I2C address 0x40) (+1k5 code)
//#define USE_DS3502                             // [I2CDriver67] Enable DS3502 digital potentiometer (I2C address 0x28 - 0x2B) (+0k4 code)
//#define USE_HYT                                // [I2CDriver68] Enable HYTxxx temperature and humidity sensor (I2C address 0x28) (+0k5 code)
//#define USE_LUXV30B                            // [I2CDriver70] Enable RFRobot SEN0390 LuxV30b ambient light sensor (I2C address 0x4A) (+0k5 code)
//#define USE_PMSA003I                           // [I2cDriver78] Enable PMSA003I Air Quality Sensor (I2C address 0x12) (+1k8 code)
//#define USE_GDK101                             // [I2cDriver79] Enable GDK101 sensor (I2C addresses 0x18 - 0x1B) (+1k2 code)
//#define USE_TC74                               // [I2cDriver80] Enable TC74 sensor (I2C addresses 0x48 - 0x4F) (+1k code)
//#define USE_PCA9557                            // [I2cDriver81] Enable PCA9557 8-bit I/O Expander (I2C addresses 0x18 - 0x1F) (+2k5 code)
//#define USE_MAX17043                           // [I2cDriver83] Enable MAX17043 fuel-gauge systems Lipo batteries sensor (I2C address 0x36) (+0k9 code)
//#define USE_AMSX915                            // [I2CDriver86] Enable AMS5915/AMS6915 pressure/temperature sensor (+1k2 code)
//#define USE_SPL06_007                          // [I2cDriver87] Enable SPL06_007 pressure and temperature sensor (I2C addresses 0x76) (+2k5 code)
//#define USE_MS5837                             // [I2cDriver91] Enable MS5837 sensor (I2C address 0x76) (+2k7 code)

//#define USE_RTC_CHIPS                          // Enable RTC chip support and NTP server - Select only one
//  #define USE_DS3231                           // [I2cDriver26] Enable DS3231 RTC (I2C address 0x68) (+1k2 code)
//  #define USE_BM8563                           // [I2cDriver59] Enable BM8563 RTC - found in M5Stack - support both I2C buses on ESP32 (I2C address 0x51) (+2.5k code)
//  #define USE_PCF85363                         // [I2cDriver66] Enable PCF85363 RTC - found Shelly 3EM (I2C address 0x51) (+0k7 code)

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
  //#define PMS_MODEL_PMS5003T                     // Enable support for PMSx003T models that report temperature and humidity (needs the USE_PMS5003 above)
#define USE_NOVA_SDS                             // Add support for SDS011 and SDS021 particle concentration sensor (+0k7 code)
#define USE_HPMA                                 // Add support for Honeywell HPMA115S0 particle concentration sensor
#define USE_SR04                                 // Add support for HC-SR04 ultrasonic devices (+1k code)
//#define USE_ME007                                // Add support for ME007 ultrasonic devices (+1k5 code)
//#define USE_DYP                                  // Add support for DYP ME-007 ultrasonic distance sensor, serial port version (+0k5 code)
#define USE_SERIAL_BRIDGE                        // Add support for software Serial Bridge (+2k code)
//#define USE_MODBUS_BRIDGE                        // Add support for software Modbus Bridge (+3k code)
#define USE_MP3_PLAYER                           // Use of the DFPlayer Mini MP3 Player RB-DFR-562 commands: play, volume and stop
//#define USE_AZ7798                               // Add support for AZ-Instrument 7798 CO2 datalogger
#define USE_PN532_HSU                            // Add support for PN532 using HSU (Serial) interface (+1k8 code, 140 bytes mem)
//#define USE_ZIGBEE                               // Enable serial communication with Zigbee CC2530 flashed with ZNP
#define USE_RDM6300                              // Add support for RDM6300 125kHz RFID Reader (+0k8)
#define USE_IBEACON                              // Add support for bluetooth LE passive scan of ibeacon devices (uses HM17 module)
//#define USE_GPS                                  // Add support for GPS and NTP Server for becoming Stratus 1 Time Source (+ 3.1kb flash, +132 bytes RAM)
#define USE_HM10                                 // (ESP8266 only) Add support for HM-10 as a BLE-bridge for the LYWSD03 (+5k1 code)
#ifdef ESP32
//  #define USE_BLE_ESP32                            // (ESP32 only) Add support for native BLE on ESP32 - use new driver
//  #define USE_MI_ESP32                             // (ESP32 only) Add support for ESP32 as a BLE-bridge (+9k2 mem, +292k flash)
#endif
#define USE_HRXL                                 // Add support for MaxBotix HRXL-MaxSonar ultrasonic range finders (+0k7)
//#define USE_TASMOTA_CLIENT                       // Add support for Arduino Uno/Pro Mini via serial interface including flashing (+2k3 code, 44 mem)
//#define USE_OPENTHERM                            // Add support for OpenTherm (+15k code)
//#define USE_MIEL_HVAC                            // Add support for Mitsubishi Electric HVAC serial interface (+5k code)
//#define USE_PROJECTOR_CTRL                       // Add support for LCD/DLP Projector serial control interface (+2k code)
//  #define USE_PROJECTOR_CTRL_NEC                 // Use codes for NEC
//  #define USE_PROJECTOR_CTRL_OPTOMA              // Use codes for OPTOMA
//#define USE_AS608                                // Add support for AS608 optical and R503 capacitive fingerprint sensor (+3k4 code)
//#define USE_TFMINIPLUS                           // Add suppoer for TFmini Plus (TFmini, TFmini-S) LiDAR modules via UART interface
//#define USE_HRG15                                // Add support for Hydreon RG-15 Solid State Rain sensor (+1k5 code)

#define USE_ENERGY_SENSOR                        // Add energy sensors (-14k code)
#define USE_PZEM004T                             // Add support for PZEM004T Energy monitor (+2k code)
#define USE_PZEM_AC                              // Add support for PZEM014,016 Energy monitor (+1k1 code)
#define USE_PZEM_DC                              // Add support for PZEM003,017 Energy monitor (+1k1 code)
#define USE_MCP39F501                            // Add support for MCP39F501 Energy monitor as used in Shelly 2 (+3k1 code)
#define USE_SDM72                                // Add support for Eastron SDM72-Modbus energy monitor (+0k3 code)
#define USE_SDM120                               // Add support for Eastron SDM120-Modbus energy monitor (+1k1 code)
//#define USE_SDM230                               // Add support for Eastron SDM230-Modbus energy monitor (+?? code)
#define USE_SDM630                               // Add support for Eastron SDM630-Modbus energy monitor (+0k6 code)
#define USE_DDS2382                              // Add support for Hiking DDS2382 Modbus energy monitor (+0k6 code)
#define USE_DDSU666                              // Add support for Chint DDSU666 Modbus energy monitor (+0k6 code)
//#define USE_SOLAX_X1                             // Add support for Solax X1 series Modbus log info (+3k1 code)
//#define USE_LE01MR                               // Add support for F&F LE-01MR modbus energy meter (+2k code)
//#define USE_TELEINFO                             // Add support for French Energy Provider metering telemetry (+5k2 code, +168 RAM + SmartMeter LinkedList Values RAM)
//#define USE_WE517                                // Add support for Orno WE517-Modbus energy monitor (+1k code)
//#define USE_MODBUS_ENERGY                        // Add support for generic modbus energy monitor using a user file in rule space (+5k)

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
#define USE_LOX_O2                               // Add support for LuminOx LOX O2 Sensor (+0k8 code)
#undef DEBUG_THEO                                // Disable debug code
#undef USE_DEBUG_DRIVER                          // Disable debug code
#endif  // FIRMWARE_SENSORS

/*********************************************************************************************\
 * [tasmota-knx.bin]
 * Provide a dedicated KNX image allowing enough code and memory space
\*********************************************************************************************/

#ifdef FIRMWARE_KNX_NO_EMULATION

#ifndef CODE_IMAGE_STR
  #define CODE_IMAGE_STR "knx"
#endif

#ifndef USE_KNX
#define USE_KNX                                  // Enable KNX IP Protocol Support (+23k code, +3k3 mem)
#endif
#undef USE_EMULATION                             // Disable Belkin WeMo and Hue Bridge emulation for Alexa (-16k code, -2k mem)
#undef USE_EMULATION_HUE                         // Disable Hue Bridge emulation for Alexa (+14k code, +2k mem common)
#undef USE_EMULATION_WEMO                        // Disable Belkin WeMo emulation for Alexa (+6k code, +2k mem common)
#undef USE_DEEPSLEEP                             // Disable support for deepsleep (+1k code)
#undef USE_DEVICE_GROUPS                         // Disable support for device groups (+3k5 code)
#undef USE_BLE_ESP32                             // (ESP32 only) Disable support for native BLE on ESP32 - use new driver
#undef USE_MI_ESP32                              // (ESP32 only) Disable support for ESP32 as a BLE-bridge (+9k2 mem, +292k flash)

#undef USE_PWM_DIMMER_REMOTE                     // Disbale support for remote switches to PWM Dimmer
#undef DEBUG_THEO                                // Disable debug code
#undef USE_DEBUG_DRIVER                          // Disable debug code
#endif  // FIRMWARE_KNX_NO_EMULATION

/*********************************************************************************************\
 * [tasmota-display.bin]
 * Provide an image with display drivers enabled
\*********************************************************************************************/

#ifdef FIRMWARE_DISPLAYS

#ifndef CODE_IMAGE_STR
  #define CODE_IMAGE_STR "display"
#endif

#undef USE_EMULATION                             // Disable Belkin WeMo and Hue Bridge emulation for Alexa (-16k code, -2k mem)
#undef USE_EMULATION_HUE                         // Disable Hue Bridge emulation for Alexa (+14k code, +2k mem common)
#undef USE_EMULATION_WEMO                        // Disable Belkin WeMo emulation for Alexa (+6k code, +2k mem common)
#undef USE_DOMOTICZ                              // Disable Domoticz
#undef USE_HOME_ASSISTANT                        // Disable Home Assistant
#define USE_TASMOTA_DISCOVERY                    // Enable Tasmota Discovery support (+2k code)

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
#undef USE_SHELLY_DIMMER                         // Disable support for Shelly Dimmer (+3k code)

#undef USE_ENERGY_SENSOR                         // Disable energy sensors (-14k code)
  #undef USE_PZEM004T                            // Disable PZEM004T energy sensor
  #undef USE_PZEM_AC                             // Disable PZEM014,016 Energy monitor
  #undef USE_PZEM_DC                             // Disable PZEM003,017 Energy monitor
  #undef USE_MCP39F501                           // Disable MCP39F501 Energy monitor as used in Shelly 2
  #undef USE_SDM72                               // Disable support for Eastron SDM72-Modbus energy meter
  #undef USE_SDM120                              // Disable support for Eastron SDM120-Modbus energy meter
  #undef USE_SDM230                              // Disable support for Eastron SDM230-Modbus energy monitor (+?? code)
  #undef USE_SDM630                              // Disable support for Eastron SDM630-Modbus energy monitor (+0k6 code)
  #undef USE_DDS2382                             // Disable support for Hiking DDS2382 Modbus energy monitor (+0k6 code)
  #undef USE_DDSU666                             // Disable support for Chint DDSU666 Modbus energy monitor (+0k6 code)
  #undef USE_SOLAX_X1                            // Disable support for Solax X1 series Modbus log info (+3k1 code)
  #undef USE_LE01MR                              // Disable support for F&F LE-01MR Modbus energy meter (+2k code)
  #undef USE_TELEINFO                            // Disable support for French Energy Provider metering telemetry
  #undef USE_IEM3000                             // Disable support for Schneider Electric iEM3000-Modbus series energy monitor (+0k8 code)
  #undef USE_WE517                               // Disable support for Orno WE517-Modbus energy monitor (+1k code)
  #undef USE_MODBUS_ENERGY                       // Disable support for generic modbus energy monitor using a user file in rule space (+5k)
#undef USE_BLE_ESP32                             // (ESP32 only) Disable support for native BLE on ESP32 - use new driver
#undef USE_MI_ESP32                              // (ESP32 only) Disable support for ESP32 as a BLE-bridge (+9k2 mem, +292k flash)

#define USE_DISPLAY                              // Add Display Support (+2k code)
  #define USE_DISPLAY_TM1637                     // [DisplayModel 15] Enable TM1637 module
  #define USE_DISPLAY_MAX7219                    // [DisplayModel 19] Enable MAX7219 7-segment module

#define USE_TM1638                               // Add support for TM1638 switches copying Switch1 .. Switch8 (+1k code)

#define USE_I2C                                  // I2C using library wire (+10k code, 0k2 mem, 124 iram)
  #define USE_DISPLAY_MODES1TO5                  // Enable display mode 1 to 5 in addition to mode 0
  #define USE_DISPLAY_LCD                        // [DisplayModel 1] Enable Lcd display (I2C addresses 0x27 and 0x3F) (+6k code)
  // REMOVED - #define USE_DISPLAY_SSD1306                    // [DisplayModel 2] Enable SSD1306 Oled 128x64 display (I2C addresses 0x3C and 0x3D) (+16k code)
  #define USE_DISPLAY_MATRIX                     // [DisplayModel 3] Enable 8x8 Matrix display (I2C adresseses see below) (+11k code)
  #define USE_DISPLAY_SEVENSEG                   // [DisplayModel 11] [I2cDriver47] Enable sevenseg display (I2C addresses 0x70 - 0x77) (<+11k code)
  // REMOVED - #define USE_DISPLAY_SH1106                     // [DisplayModel 7] Enable SH1106 Oled 128x64 display (I2C addresses 0x3C and 0x3D)
//  #define USE_DISPLAY_TM1650                     // [DisplayModel 20] [I2cDriver74] Enable TM1650 display (I2C addresses 0x24 - 0x27 and 0x34 - 0x37)

#define USE_SPI                                  // Hardware SPI using GPIO12(MISO), GPIO13(MOSI) and GPIO14(CLK) in addition to two user selectable GPIOs(CS and DC)
  // #define USE_DISPLAY_ILI9341                    // [DisplayModel 4] Enable ILI9341 Tft 480x320 display (+19k code)
  // #define USE_DISPLAY_EPAPER_29                  // [DisplayModel 5] Enable e-paper 2.9 inch display (+19k code)
  // #define USE_DISPLAY_EPAPER_42                  // [DisplayModel 6] Enable e-paper 4.2 inch display
  // #define USE_DISPLAY_ILI9488                    // [DisplayModel 8]
  // #define USE_DISPLAY_SSD1351                    // [DisplayModel 9]
  // #define USE_DISPLAY_RA8876                     // [DisplayModel 10]
  // REMOVED - #define USE_DISPLAY_ST7789                     // [DisplayModel 12] Enable ST7789 module

#define USE_UNIVERSAL_DISPLAY
#define USE_UNIVERSAL_TOUCH

#undef DEBUG_THEO                                // Disable debug code
#undef USE_DEBUG_DRIVER                          // Disable debug code
#endif  // FIRMWARE_DISPLAYS

/*********************************************************************************************\
 * [tasmota-ir.bin]
 * Provide a dedicated image with IR full protocol support, with limited additional features
\*********************************************************************************************/

#ifdef FIRMWARE_IR

#ifndef CODE_IMAGE_STR
  #define CODE_IMAGE_STR "ir"
#endif

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

// -- IR options ----------------------------
#define USE_IR_REMOTE                            // Enable IR remote commands using library IRremoteESP8266
#define USE_IR_REMOTE_FULL                       // Support all IR protocols from IRremoteESP8266

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
#undef USE_SM2335                                // Disable support for SM2335 RGBCW led control as used in Switchbot Bulb
#undef USE_BP5758D                               // Disable support for BP5758D RGBCW led control as used in some Tuya lightbulbs (+0k8 code)
#undef USE_SONOFF_L1                             // Disable support for Sonoff L1 led control
#undef USE_ELECTRIQ_MOODL                        // Disable support for ElectriQ iQ-wifiMOODL RGBW LED controller
#undef USE_LIGHT_PALETTE                         // Disable support for color palette (+0k9 code)
#undef USE_SHELLY_DIMMER                         // Disable support for Shelly Dimmer (+3k code)

#undef USE_COUNTER                               // Disable counters
#define USE_ADC_VCC                              // Display Vcc in Power status. Disable for use as Analog input on selected devices
#undef USE_DS18x20                               // Disable DS18x20 sensor

#undef USE_ENERGY_SENSOR                         // Disable energy sensors (-14k code)
  #undef USE_PZEM004T                            // Disable PZEM004T energy sensor
  #undef USE_PZEM_AC                             // Disable PZEM014,016 Energy monitor
  #undef USE_PZEM_DC                             // Disable PZEM003,017 Energy monitor
  #undef USE_MCP39F501                           // Disable MCP39F501 Energy monitor as used in Shelly 2
  #undef USE_SDM72                               // Disable support for Eastron SDM72-Modbus energy meter
  #undef USE_SDM120                              // Disable support for Eastron SDM120-Modbus energy meter
  #undef USE_SDM230                              // Disable support for Eastron SDM230-Modbus energy monitor (+?? code)
  #undef USE_SDM630                              // Disable support for Eastron SDM630-Modbus energy monitor (+0k6 code)
  #undef USE_DDS2382                             // Disable support for Hiking DDS2382 Modbus energy monitor (+0k6 code)
  #undef USE_DDSU666                             // Disable support for Chint DDSU666 Modbus energy monitor (+0k6 code)
  #undef USE_SOLAX_X1                            // Disable support for Solax X1 series Modbus log info (+3k1 code)
  #undef USE_LE01MR                              // Disable support for F&F LE-01MR Modbus energy meter (+2k code)
  #undef USE_TELEINFO                            // Disable support for French Energy Provider metering telemetry
  #undef USE_IEM3000                             // Disable support for Schneider Electric iEM3000-Modbus series energy monitor (+0k8 code)
  #undef USE_WE517                               // Disable support for Orno WE517-Modbus energy monitor (+1k code)
  #undef USE_MODBUS_ENERGY                       // Disable support for generic modbus energy monitor using a user file in rule space (+5k)

//#undef USE_DS18x20                               // Disable support for DS18x20 sensors with id sort, single scan and read retry (+1k3 code)

#undef USE_I2C                                   // Disable all I2C sensors
#undef USE_SPI                                   // Disable all SPI devices

#undef USE_DISPLAY                               // Disable support for displays

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
#undef USE_LOX_O2                                // Disable support for LuminOx LOX O2 Sensor

#undef USE_DHT                                   // Disable support for DHT11, AM2301 (DHT21, DHT22, AM2302, AM2321) and SI7021 Temperature and Humidity sensor
#undef USE_MAX31855                              // Disable MAX31855 K-Type thermocouple sensor using softSPI
#undef USE_MAX31865                              // Disable support for MAX31865 RTD sensors using softSPI
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
#undef DEBUG_THEO                                // Disable debug code
#undef USE_DEBUG_DRIVER                          // Disable debug code
#endif  // FIRMWARE_IR


/*********************************************************************************************\
 * [tasmota-zbbridge.bin]
 * Provide an image for the Sonoff Zigbee Bridge
\*********************************************************************************************/

#ifdef FIRMWARE_ZBBRIDGE  // *******************************************************************

#ifndef CODE_IMAGE_STR
  #define CODE_IMAGE_STR "zbbridge"
#endif

#undef MODULE
#define MODULE                 SONOFF_ZB_BRIDGE  // [Module] Select default module from tasmota_template.h
#undef FALLBACK_MODULE
#define FALLBACK_MODULE        SONOFF_ZB_BRIDGE  // [Module2] Select default module on fast reboot where USER_MODULE is user template

#define USE_INFLUXDB                             // Enable influxdb support (+5k code)

#undef  SERIAL_LOG_LEVEL
#define SERIAL_LOG_LEVEL       LOG_LEVEL_NONE    // [SerialLog] (LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE)

#undef USE_ARDUINO_OTA                           // Disable support for Arduino OTA
#define USE_DOMOTICZ                             // Enable Domoticz
#undef USE_HOME_ASSISTANT                        // Disable Home Assistant
#define USE_TASMOTA_DISCOVERY                    // Enable Tasmota Discovery support (+2k code)

#define UPGRADE_V8_MIN                           // do not support upgrading from version below 8.0.0

// -- MQTT - TLS - AWS IoT ------------------------
#ifdef USE_ZBBRIDGE_TLS                            // Enable TLS for ZbBridge
  #define USE_MQTT_TLS                             // Use TLS for MQTT connection (+34.5k code, +7.0k mem and +4.8k additional during connection handshake)
  #define USE_MQTT_AWS_IOT_LIGHT                   // Enable MQTT for AWS IoT in light mode, with user/password instead of private certificate
#endif                                             // USE_ZBBRIDGE_TLS

//#undef USE_KNX                                   // Disable KNX IP Protocol Support
//#undef USE_WEBSERVER                             // Disable Webserver
#undef USE_ENHANCED_GUI_WIFI_SCAN                // Disable wifi scan output with BSSID (+0k5 code)
//#undef USE_WEBSEND_RESPONSE                      // Disable command WebSend response message (+1k code)
#define USE_EMULATION_HUE                        // Enable Hue Bridge emulation for Alexa (+14k code, +2k mem common)
#undef USE_EMULATION_WEMO                        // Disable Belkin WeMo emulation for Alexa (+6k code, +2k mem common)
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
#undef USE_LIGHT                                 // Disable Dimmer/Light support
#undef USE_LIGHT_VIRTUAL_CT                      // Disable support for Virtual White Color Temperature (SO106)
#undef USE_WS2812                                // Disable WS2812 Led string using library NeoPixelBus (+5k code, +1k mem, 232 iram) - Disable by //
#undef USE_MY92X1                                // Disable support for MY92X1 RGBCW led controller as used in Sonoff B1, Ailight and Lohas
#undef USE_SM16716                               // Disable support for SM16716 RGB LED controller (+0k7 code)
#undef USE_SM2135                                // Disable support for SM2135 RGBCW led control as used in Action LSC (+0k6 code)
#undef USE_SM2335                                // Disable support for SM2335 RGBCW led control as used in Switchbot Bulb
#undef USE_BP5758D                               // Disable support for BP5758D RGBCW led control as used in some Tuya lightbulbs (+0k8 code)
#undef USE_SONOFF_L1                             // Disable support for Sonoff L1 led control
#undef USE_ELECTRIQ_MOODL                        // Disable support for ElectriQ iQ-wifiMOODL RGBW LED controller
#undef USE_LIGHT_PALETTE                         // Disable support for color palette (+0k9 code)
#undef USE_SHELLY_DIMMER                         // Disable support for Shelly Dimmer (+3k code)

#undef USE_COUNTER                               // Disable counters
#define USE_ADC_VCC                              // Display Vcc in Power status. Disable for use as Analog input on selected devices
#undef USE_DS18x20                               // Disable DS18x20 sensor
//#undef USE_I2C                                   // Enable I2C, zbbridge uses i2c EEprom
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
#undef USE_LOX_O2                                // Disable support for LuminOx LOX O2 Sensor

#undef USE_ENERGY_SENSOR                         // Disable energy sensors
#undef USE_ADE7880                               // Disable ADE7880 Energy monitor as used on Shelly 3EM (I2C address 0x38) (+3k8)
#undef USE_ADE7953                               // Disable ADE7953 Energy monitor as used on Shelly 2.5 (I2C address 0x38) (+1k5)
#undef USE_PZEM004T                              // Disable PZEM004T energy sensor
#undef USE_PZEM_AC                               // Disable PZEM014,016 Energy monitor
#undef USE_PZEM_DC                               // Disable PZEM003,017 Energy monitor
#undef USE_MCP39F501                             // Disable MCP39F501 Energy monitor as used in Shelly 2
#undef USE_SDM72                                 // Disable support for Eastron SDM72-Modbus energy meter
#undef USE_SDM120                                // Disable support for Eastron SDM120-Modbus energy meter
#undef USE_SDM230                                // Disable support for Eastron SDM230-Modbus energy monitor (+?? code)
#undef USE_SDM630                                // Disable support for Eastron SDM630-Modbus energy monitor (+0k6 code)
#undef USE_DDS2382                               // Disable support for Hiking DDS2382 Modbus energy monitor (+0k6 code)
#undef USE_DDSU666                               // Disable support for Chint DDSU666 Modbus energy monitor (+0k6 code)
#undef USE_SOLAX_X1                              // Disable support for Solax X1 series Modbus log info (+3k1 code)
#undef USE_LE01MR                                // Disable support for F&F LE-01MR Modbus energy meter (+2k code)
#undef USE_TELEINFO                              // Disable support for French Energy Provider metering telemetry
#undef USE_IEM3000                               // Disable support for Schneider Electric iEM3000-Modbus series energy monitor (+0k8 code)
#undef USE_WE517                                 // Disable support for Orno WE517-Modbus energy monitor (+1k code)
#undef USE_MODBUS_ENERGY                         // Disable support for generic modbus energy monitor using a user file in rule space (+5k)

#undef USE_DHT                                   // Disable support for DHT11, AM2301 (DHT21, DHT22, AM2302, AM2321) and SI7021 Temperature and Humidity sensor
#undef USE_MAX31855                              // Disable MAX31855 K-Type thermocouple sensor using softSPI
#undef USE_MAX31865                              // Disable support for MAX31865 RTD sensors using softSPI
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
#undef DEBUG_THEO                                // Disable debug code
#undef USE_DEBUG_DRIVER                          // Disable debug code

#define USE_ZIGBEE
#undef USE_ZIGBEE_ZNP
#define USE_ZIGBEE_EZSP
#define USE_ZIGBEE_EEPROM                        // EEPROM of Sonoff ZBBridge via I2C
#define USE_TCP_BRIDGE
  #define USE_ZIGBEE_CHANNEL  11                 // Zigbee Channel (11-26)
  #define USE_ZIGBEE_COALESCE_ATTR_TIMER 350     // timer to coalesce attribute values (in ms)

#endif  // SONOFF_ZIGBEEBRIDGE ******************************************************************


/*********************************************************************************************\
 * [tasmota-lite.bin]
 * Provide an image without sensors
\*********************************************************************************************/

#ifdef FIRMWARE_LITE

#ifndef CODE_IMAGE_STR
  #define CODE_IMAGE_STR "lite"
#endif

#undef USE_ARDUINO_OTA                           // Disable support for Arduino OTA
#undef USE_DOMOTICZ                              // Disable Domoticz
#undef USE_HOME_ASSISTANT                        // Disable Home Assistant
#define USE_TASMOTA_DISCOVERY                    // Enable Tasmota Discovery support (+2k code)
#undef USE_MQTT_TLS                              // Disable TLS support won't work as the MQTTHost is not set
#undef USE_KNX                                   // Disable KNX IP Protocol Support
#undef USE_DALI                                  // Disable support for DALI gateway (+5k code)
//#undef USE_WEBSERVER                             // Disable Webserver
#undef USE_ENHANCED_GUI_WIFI_SCAN                // Disable wifi scan output with BSSID (+0k5 code)
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
#undef USE_SONOFF_D1                             // Disable support for Sonoff D1 Dimmer (+0k7 code)
#undef USE_SHELLY_DIMMER                         // Disable support for Shelly Dimmer (+3k code)

// -- Optional light modules ----------------------
//#undef USE_LIGHT                                 // Also disable all Dimmer/Light support
#undef USE_WS2812                                // Disable WS2812 Led string using library NeoPixelBus (+5k code, +1k mem, 232 iram) - Disable by //
#undef USE_MY92X1                                // Disable support for MY92X1 RGBCW led controller as used in Sonoff B1, Ailight and Lohas
#undef USE_SM16716                               // Disable support for SM16716 RGB LED controller (+0k7 code)
#undef USE_SM2135                                // Disable support for SM2135 RGBCW led control as used in Action LSC (+0k6 code)
#undef USE_SM2335                                // Disable support for SM2335 RGBCW led control as used in Switchbot Bulb
#undef USE_BP5758D                               // Disable support for BP5758D RGBCW led control as used in some Tuya lightbulbs (+0k8 code)
#undef USE_SONOFF_L1                             // Disable support for Sonoff L1 led control
#undef USE_ELECTRIQ_MOODL                        // Disable support for ElectriQ iQ-wifiMOODL RGBW LED controller
#undef USE_LIGHT_PALETTE                         // Disable support for color palette (+0k9 code)
#undef USE_LIGHT_VIRTUAL_CT                      // Disable support for Virtual White Color Temperature (SO106)
#undef USE_DGR_LIGHT_SEQUENCE                    // Disable support for device group light sequencing (requires USE_DEVICE_GROUPS) (+0k2 code)

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
#undef USE_TCP_BRIDGE                            // Disable support for Serial to TCP bridge (+1.3k code)
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
#undef USE_AS608                                 // Disable support for AS608 optical and R503 capacitive fingerprint sensor (+3k code)
#undef USE_TFMINIPLUS                            // Disable support for TFmini Plus (TFmini, TFmini-S) LiDAR modules via UART interface (+0k8)
#undef USE_HRG15                                 // Disable support for Hydreon RG-15 Solid State Rain sensor (+1k5 code)
#undef USE_VINDRIKTNING                          // Disable support for IKEA VINDRIKTNING particle concentration sensor (+1k code)
#undef USE_LOX_O2                                // Disable support for LuminOx LOX O2 Sensor

#undef USE_ENERGY_SENSOR                         // Disable energy sensors
#undef USE_PZEM004T                              // Disable PZEM004T energy sensor
#undef USE_PZEM_AC                               // Disable PZEM014,016 Energy monitor
#undef USE_PZEM_DC                               // Disable PZEM003,017 Energy monitor
//#undef USE_MCP39F501                             // Disable MCP39F501 Energy monitor as used in Shelly 2
#undef USE_SDM72                                 // Disable support for Eastron SDM72-Modbus energy meter
#undef USE_SDM120                                // Disable support for Eastron SDM120-Modbus energy meter
#undef USE_SDM230                                // Disable support for Eastron SDM230-Modbus energy monitor (+?? code)
#undef USE_SDM630                                // Disable support for Eastron SDM630-Modbus energy monitor (+0k6 code)
#undef USE_DDS2382                               // Disable support for Hiking DDS2382 Modbus energy monitor (+0k6 code)
#undef USE_DDSU666                               // Disable support for Chint DDSU666 Modbus energy monitor (+0k6 code)
#undef USE_SOLAX_X1                              // Disable support for Solax X1 series Modbus log info (+3k1 code)
#undef USE_LE01MR                                // Disable support for F&F LE-01MR Modbus energy meter (+2k code)
//#undef USE_BL09XX                                // Disable support for BL0940 Energy monitor as used in Blitzwolf SHP-10 (+1k6 code)
#undef USE_TELEINFO                              // Disable support for French Energy Provider metering telemetry
#undef USE_IEM3000                               // Disable support for Schneider Electric iEM3000-Modbus series energy monitor (+0k8 code)
#undef USE_BL6523                                // Disable support for BL6523 based energy monitor (+7k code)
#undef USE_WE517                                 // Disable support for Orno WE517-Modbus energy monitor (+1k code)
#undef USE_MODBUS_ENERGY                         // Disable support for generic modbus energy monitor using a user file in rule space (+5k)

#undef USE_DHT                                   // Disable support for DHT11, AM2301 (DHT21, DHT22, AM2302, AM2321) and SI7021 Temperature and Humidity sensor
#undef USE_MAX31855                              // Disable MAX31855 K-Type thermocouple sensor using softSPI
#undef USE_MAX31865                              // Disable support for MAX31865 RTD sensors using softSPI
#undef USE_LMT01                                 // Disable support for TI LMT01 temperature sensor, count pulses on single GPIO (+0k5 code)
#undef USE_WIEGAND                               // Disable support for 24/26/32/34 bit RFID Wiegand interface (D0/D1) (+1k7 code)
#undef USE_IR_REMOTE                             // Disable IR driver

#undef USE_SHIFT595                              // Disable support for 74xx595 8-bit shift registers (+0k7 code)
#undef USE_TM1638                                // Disable support for TM1638 switches copying Switch1 .. Switch8
#undef USE_HX711                                 // Disable support for HX711 load cell
#undef USE_TX20_WIND_SENSOR                      // Disable support for La Crosse TX20 anemometer
#undef USE_TX23_WIND_SENSOR                      // Disable support for La Crosse TX23 anemometer
#undef USE_WINDMETER                             // Disable support for analog anemometer (+2k2 code)
#undef USE_FTC532                                // Disable support for FTC532 touch controller (+0k6 code)
#undef USE_RC_SWITCH                             // Disable support for RF transceiver using library RcSwitch
#undef USE_RF_SENSOR                             // Disable support for RF sensor receiver (434MHz or 868MHz) (+0k8 code)
#undef USE_HRE                                   // Disable support for Badger HR-E Water Meter (+1k4 code)
#undef USE_A4988_STEPPER                         // Disable support for A4988_Stepper
#undef USE_PROMETHEUS                            // Disable support for https://prometheus.io/ metrics exporting over HTTP /metrics endpoint
#undef USE_NEOPOOL                               // Disable support for Sugar Valley NeoPool Controller - also known under brands Hidrolife, Aquascenic, Oxilife, Bionet, Hidroniser, UVScenic, Station, Brilix, Bayrol and Hay (+14k flash, +120 mem)
#undef USE_THERMOSTAT                            // Disable support for Thermostat
#undef DEBUG_THEO                                // Disable debug code
#undef USE_DEBUG_DRIVER                          // Disable debug code
#undef USE_AC_ZERO_CROSS_DIMMER                  // Disable support for AC_ZERO_CROSS_DIMMER
#endif  // FIRMWARE_LITE


/*********************************************************************************************\
 * [tasmota-minimal.bin]
 * Provide the smallest image possible while still enabling a webserver for intermediate image load
\*********************************************************************************************/

#ifndef FIRMWARE_MINICUSTOM
#ifdef FIRMWARE_MINIMAL

#ifndef CODE_IMAGE_STR
  #define CODE_IMAGE_STR "minimal"
#endif

#undef FIRMWARE_LITE                             // Disable tasmota-lite with no sensors
#undef FIRMWARE_SENSORS                          // Disable tasmota-sensors with useful sensors enabled
#undef FIRMWARE_KNX_NO_EMULATION                 // Disable tasmota-knx with KNX but without Emulation
#undef FIRMWARE_DISPLAYS                         // Disable tasmota-display with display drivers enabled
#undef FIRMWARE_IR                               // Disable tasmota-ir with IR full protocols activated

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
#undef USE_GPIO_VIEWER                           // Disable GPIO Viewer to see realtime GPIO states (+5k6 code)
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

#endif  // FIRMWARE_MINIMAL
#endif  // ifndef FIRMWARE_MINICUSTOM


/*********************************************************************************************\
 * [tasmota-minicustom.bin]
 * Make a small minimal custom image possible
\*********************************************************************************************/

#ifdef FIRMWARE_MINICUSTOM
#define FIRMWARE_MINIMAL

#ifndef CODE_IMAGE_STR
  #define CODE_IMAGE_STR "mini-custom"
#endif

#undef FIRMWARE_LITE                             // Disable tasmota-lite with no sensors
#undef FIRMWARE_SENSORS                          // Disable tasmota-sensors with useful sensors enabled
#undef FIRMWARE_KNX_NO_EMULATION                 // Disable tasmota-knx with KNX but without Emulation
#undef FIRMWARE_DISPLAYS                         // Disable tasmota-display with display drivers enabled
#undef FIRMWARE_IR                               // Disable tasmota-ir with IR full protocols activated
#undef FIRMWARE_WEBCAM
#undef FIRMWARE_ODROID_GO
#undef FIRMWARE_M5STACK_CORE2
#undef FIRMWARE_BLUETOOTH
#undef FIRMWARE_LVGL
#undef FIRMWARE_TASMOTA32
#undef USE_UFILESYS
#undef GUI_TRASH_FILE
#undef GUI_EDIT_FILE
#undef USE_PING
#undef USE_AUTOCONF
#undef USE_BERRY
#undef USE_WEBCLIENT_HTTPS

#endif  // FIRMWARE_MINICUSTOM


#ifdef ESP32
#include "tasmota_configurations_ESP32.h"
#endif  // ESP32

/*********************************************************************************************\
 * Post-configuration for IRremoteESP8266 protocol selection
\*********************************************************************************************/

// IR_SEND has two mode: minimal or full (USE_IR_REMOTE_FULL)
#ifdef USE_IR_REMOTE_FULL
  // Enable all protocols except PRONTO
  #ifndef _IR_ENABLE_DEFAULT_                    // it is possible to define this value previously in config
    #define _IR_ENABLE_DEFAULT_ true             // Enable all protocols except exluded below
  #endif
  // PRONTO protocol cannot be supported because it requires specific APIs which are not supported in Tasmota
  #define DECODE_PRONTO false                    // Exclude PRONTO protocol
  #define SEND_PRONTO false                      // Exclude PRONTO protocol
#else
  #define _IR_ENABLE_DEFAULT_ false              // disable all protocols by default
  #ifdef USE_IR_REMOTE
    // below are the default IR protocols
    #define DECODE_HASH true
    #ifdef USE_IR_SEND_NEC
      #define SEND_NEC   true                    // Support IRsend NEC protocol
      #define DECODE_NEC true                    // Support IRreceive NEC protocol
    #endif // USE_IR_SEND_NEC
    #ifdef USE_IR_SEND_RC5
      #define SEND_RC5   true                    // Support IRsend Philips RC5 protocol
      #define DECODE_RC5 true                    // Support IRreceive Philips RC5 protocol
    #endif // USE_IR_SEND_RC5
    #ifdef USE_IR_SEND_RC6
      #define SEND_RC6   true                    // Support IRsend Philips RC6 protocol
      #define DECODE_RC6 true                    // Support IRreceive Philips RC6 protocol
    #endif // USE_IR_SEND_RC6
  #endif // USE_IR_REMOTE
#endif // USE_IR_REMOTE_FULL

/*********************************************************************************************\
 * Mandatory defines satisfying disabled defines
\*********************************************************************************************/

#ifndef ESP8266_1M
  #ifndef FIRMWARE_MINIMAL                       // There might be a ESP32-minimal
    #define USE_UFILESYS
      #define GUI_TRASH_FILE
      #define GUI_EDIT_FILE
    #ifdef ESP8266
      #ifndef USE_FTP
        #define USE_FTP
      #endif
      #ifndef USER_FTP
        #define USER_FTP "user"
      #endif
      #ifndef PW_FTP
        #define PW_FTP "pass"
      #endif
    #endif // ESP8266
    #define USE_SPI
    #define USE_SDCARD
    #define USE_PING
  #endif // FIRMWARE_MINIMAL

  #ifdef USE_RULES
    #define USE_EXPRESSION
    #define SUPPORT_IF_STATEMENT
    #define SUPPORT_MQTT_EVENT
  #endif  // USE_RULES
#endif  // NOT ESP8266_1M

#ifdef USE_EMULATION_HUE
#define USE_EMULATION
#endif
#ifdef USE_EMULATION_WEMO
#define USE_EMULATION
#endif
#ifdef USE_EMULATION
#define USE_LIGHT
#endif

#ifdef USE_AC_ZERO_CROSS_DIMMER
#define USE_COUNTER
#define USE_LIGHT
#endif

#ifdef USE_PWM_DIMMER
#define USE_LIGHT
#endif

#ifdef USE_TUYA_MCU
#define USE_LIGHT
#endif

#ifdef USE_ARILUX_RF
#define USE_LIGHT
#endif

// Convert legacy slave to client
#ifdef USE_TASMOTA_SLAVE
#define USE_TASMOTA_CLIENT
#endif
#ifdef USE_TASMOTA_SLAVE_FLASH_SPEED
#define USE_TASMOTA_CLIENT_FLASH_SPEED USE_TASMOTA_SLAVE_FLASH_SPEED
#endif
#ifdef USE_TASMOTA_SLAVE_SERIAL_SPEED
#define USE_TASMOTA_CLIENT_SERIAL_SPEED USE_TASMOTA_SLAVE_SERIAL_SPEED
#endif

#ifdef USE_SCRIPT
#define USE_UNISHOX_COMPRESSION                  // Add support for string compression
#if defined(USE_SML_M) && !defined(NO_USE_SML_DECRYPT)
#ifndef USE_TLS                                  // Add support for TLS as required by SML decryption
#define USE_TLS
#endif
#endif
#endif

#ifdef USE_ZIGBEE
#define USE_UNISHOX_COMPRESSION                  // Add support for string compression
#endif
#ifdef USE_EMULATION_HUE
#define USE_UNISHOX_COMPRESSION                  // Add support for string compression
#endif

#if defined(USE_MQTT_TLS)                        // Enable TLS if required:
  #define USE_TLS                                // flag indicates we need to include TLS code
#endif                                           // USE_MQTT_TLS

/*********************************************************************************************\
 * Default image
\*********************************************************************************************/

#ifndef CODE_IMAGE_STR
  #define CODE_IMAGE_STR "tasmota"
#endif

#endif  // _TASMOTA_CONFIGURATIONS_H_
