/*
  tasmota_template_ESP32.h - ESP32 template settings for Tasmota

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

#ifndef _TASMOTA_TEMPLATE_ESP32_H_
#define _TASMOTA_TEMPLATE_ESP32_H_

#ifdef ESP32

// Hardware has no ESP32
#undef USE_TUYA_DIMMER
#undef USE_PWM_DIMMER
#undef USE_EXS_DIMMER
#undef USE_ARMTRONIX_DIMMERS
#undef USE_SONOFF_RF
#undef USE_SONOFF_SC
#undef USE_SONOFF_IFAN
#undef USE_SONOFF_L1
#undef USE_SONOFF_D1
#undef USE_RF_FLASH

// Not ported (yet)
#undef USE_DISCOVERY
#undef USE_ADC_VCC    // Needs to be ported
#undef USE_DEEPSLEEP
#undef USE_MY92X1
#undef USE_TUYA_MCU
#undef USE_PS_16_DZ

enum UserSelectablePins {
  GPIO_NONE,                           // Not used
  GPIO_KEY1, GPIO_KEY1_NP, GPIO_KEY1_INV, GPIO_KEY1_INV_NP, // 4 x Button
  GPIO_SWT1, GPIO_SWT1_NP,             // 8 x User connected external switches
  GPIO_REL1, GPIO_REL1_INV,            // 8 x Relays
  GPIO_LED1, GPIO_LED1_INV,            // 4 x Leds
  GPIO_CNTR1, GPIO_CNTR1_NP,           // 4 x Counter
  GPIO_PWM1, GPIO_PWM1_INV,            // 5 x PWM
  GPIO_BUZZER, GPIO_BUZZER_INV,        // Buzzer
  GPIO_LEDLNK, GPIO_LEDLNK_INV,        // Link led
  GPIO_I2C_SCL, GPIO_I2C_SDA,          // Software I2C
  GPIO_SPI_MISO, GPIO_SPI_MOSI, GPIO_SPI_CLK, GPIO_SPI_CS, GPIO_SPI_DC,        // Hardware SPI
  GPIO_SSPI_MISO, GPIO_SSPI_MOSI, GPIO_SSPI_SCLK, GPIO_SSPI_CS, GPIO_SSPI_DC,  // Software SPI
  GPIO_BACKLIGHT,                      // Display backlight control
  GPIO_OLED_RESET,                     // OLED Display Reset
  GPIO_IRSEND, GPIO_IRRECV,            // IR interface
  GPIO_RFSEND, GPIO_RFRECV,            // RF interface
  GPIO_DHT11, GPIO_DHT22, GPIO_SI7021, GPIO_DHT11_OUT,  // DHT11, DHT21, DHT22, AM2301, AM2302, AM2321
  GPIO_DSB, GPIO_DSB_OUT,              // DS18B20 or DS18S20
  GPIO_WS2812,                         // WS2812 Led string
  GPIO_MHZ_TXD, GPIO_MHZ_RXD,          // MH-Z19 Serial interface
  GPIO_PZEM0XX_TX, GPIO_PZEM004_RX, GPIO_PZEM016_RX, GPIO_PZEM017_RX, // PZEM Serial Modbus interface
  GPIO_SAIR_TX, GPIO_SAIR_RX,          // SenseAir Serial interface
  GPIO_PMS5003_TX, GPIO_PMS5003_RX,    // Plantower PMS5003 Serial interface
  GPIO_SDS0X1_TX, GPIO_SDS0X1_RX,      // Nova Fitness SDS011 Serial interface
  GPIO_SBR_TX, GPIO_SBR_RX,            // Serial Bridge Serial interface
  GPIO_SR04_TRIG, GPIO_SR04_ECHO,      // SR04 interface
  GPIO_SDM120_TX, GPIO_SDM120_RX,      // SDM120 Serial interface
  GPIO_SDM630_TX, GPIO_SDM630_RX,      // SDM630 Serial interface
  GPIO_TM16CLK, GPIO_TM16DIO, GPIO_TM16STB,  // TM1638 interface
  GPIO_MP3_DFR562,                     // RB-DFR-562, DFPlayer Mini MP3 Player
  GPIO_HX711_SCK, GPIO_HX711_DAT,      // HX711 Load Cell interface
  GPIO_TX2X_TXD_BLACK,                 // TX20/TX23 Transmission Pin
  GPIO_TUYA_TX, GPIO_TUYA_RX,          // Tuya Serial interface
  GPIO_MGC3130_XFER, GPIO_MGC3130_RESET,  // MGC3130 interface
  GPIO_RF_SENSOR,                      // Rf receiver with sensor decoding
  GPIO_AZ_TXD, GPIO_AZ_RXD,            // AZ-Instrument 7798 Serial interface
  GPIO_MAX31855CS, GPIO_MAX31855CLK, GPIO_MAX31855DO,  // MAX31855 Serial interface
  GPIO_NRG_SEL, GPIO_NRG_SEL_INV, GPIO_NRG_CF1, GPIO_HLW_CF, GPIO_HJL_CF,  // HLW8012/HJL-01/BL0937 energy monitoring
  GPIO_MCP39F5_TX, GPIO_MCP39F5_RX, GPIO_MCP39F5_RST,  // MCP39F501 Energy monitoring (Shelly2)
  GPIO_PN532_TXD, GPIO_PN532_RXD,      // PN532 NFC Serial interface
  GPIO_SM16716_CLK, GPIO_SM16716_DAT, GPIO_SM16716_SEL,  // SM16716 SELECT
  GPIO_DI, GPIO_DCKI,                  // my92x1 PWM controller
  GPIO_CSE7766_TX, GPIO_CSE7766_RX,    // CSE7766 Serial interface (S31 and Pow R2)
  GPIO_ARIRFRCV, GPIO_ARIRFSEL,        // Arilux RF Receive input
  GPIO_TXD, GPIO_RXD,                  // Serial interface
  GPIO_ROT1A, GPIO_ROT1B, GPIO_ROT2A, GPIO_ROT2B,  // Rotary switch
  GPIO_HRE_CLOCK, GPIO_HRE_DATA,       // HR-E Water Meter
  GPIO_ADE7953_IRQ,                    // ADE7953 IRQ
  GPIO_SOLAXX1_TX, GPIO_SOLAXX1_RX,    // Solax Inverter Serial interface
  GPIO_ZIGBEE_TX, GPIO_ZIGBEE_RX,      // Zigbee Serial interface
  GPIO_RDM6300_RX,                     // RDM6300 RX
  GPIO_IBEACON_TX, GPIO_IBEACON_RX,    // HM17 IBEACON Serial interface
  GPIO_A4988_DIR, GPIO_A4988_STP, GPIO_A4988_ENA,  // A4988 interface
  GPIO_A4988_MS1, GPIO_A4988_MS2, GPIO_A4988_MS3,  // A4988 microstep
  GPIO_DDS2382_TX, GPIO_DDS2382_RX,    // DDS2382 Serial interface
  GPIO_DDSU666_TX, GPIO_DDSU666_RX,    // DDSU666 Serial interface
  GPIO_SM2135_CLK, GPIO_SM2135_DAT,    // SM2135 PWM controller
  GPIO_DEEPSLEEP,                      // Kill switch for deepsleep
  GPIO_EXS_ENABLE,                     // EXS MCU Enable
  GPIO_TASMOTASLAVE_TXD, GPIO_TASMOTASLAVE_RXD,      // Slave Serial interface
  GPIO_TASMOTASLAVE_RST, GPIO_TASMOTASLAVE_RST_INV,  // Slave Reset
  GPIO_HPMA_RX, GPIO_HPMA_TX,          // Honeywell HPMA115S0 Serial interface
  GPIO_GPS_RX, GPIO_GPS_TX,            // GPS Serial interface
  GPIO_HM10_RX, GPIO_HM10_TX,          // HM10-BLE-Mijia-bridge Serial interface
  GPIO_LE01MR_RX, GPIO_LE01MR_TX,      // F&F LE-01MR energy meter
  GPIO_CC1101_GDO0, GPIO_CC1101_GDO2,  // CC1101 Serial interface
  GPIO_HRXL_RX,                        // Data from MaxBotix HRXL sonar range sensor
  GPIO_ELECTRIQ_MOODL_TX,              // ElectriQ iQ-wifiMOODL Serial TX
  GPIO_AS3935,
  ADC0_INPUT,                          // Analog input
  ADC0_TEMP,                           // Analog Thermistor
  ADC0_LIGHT,                          // Analog Light sensor
  ADC0_BUTTON, ADC0_BUTTON_INV,        // Analog Button
  ADC0_RANGE,                          // Analog Range
  ADC0_CT_POWER,                       // ANalog Current
  GPIO_WEBCAM_PWDN, GPIO_WEBCAM_RESET, GPIO_WEBCAM_XCLK,  // Webcam
  GPIO_WEBCAM_SIOD, GPIO_WEBCAM_SIOC,  // Webcam I2C
  GPIO_WEBCAM_DATA,
  GPIO_WEBCAM_VSYNC, GPIO_WEBCAM_HREF, GPIO_WEBCAM_PCLK,
  GPIO_WEBCAM_PSCLK,
  GPIO_WEBCAM_HSD,
  GPIO_WEBCAM_PSRCS,
  GPIO_BOILER_OT_RX, GPIO_BOILER_OT_TX,  // OpenTherm Boiler TX pin
  GPIO_WINDMETER_SPEED,                // WindMeter speed counter pin
  GPIO_KEY1_TC,                        // Touch pin as button
  GPIO_BL0940_RX,                      // BL0940 serial interface
  GPIO_TELEINFO_RX,                    // Teleinfo telemetry data receive pin
  GPIO_TELEINFO_ENABLE,                // Teleinfo Enable Receive Pin
  GPIO_SENSOR_END };

enum ProgramSelectablePins {
//  GPIO_FIX_START = 254,
  GPIO_FIX_START = 2046,
  GPIO_USER,           // User configurable needs to be 2047
  GPIO_MAX };

// Text in webpage Module Parameters and commands GPIOS and GPIO
const char kSensorNames[] PROGMEM =
  D_SENSOR_NONE "|"
  D_SENSOR_BUTTON "|" D_SENSOR_BUTTON "_n|" D_SENSOR_BUTTON "_i|" D_SENSOR_BUTTON "_in|"
  D_SENSOR_SWITCH "|" D_SENSOR_SWITCH "_n|"
  D_SENSOR_RELAY "|" D_SENSOR_RELAY "_i|"
  D_SENSOR_LED "|" D_SENSOR_LED "_i|"
  D_SENSOR_COUNTER "|" D_SENSOR_COUNTER "_n|"
  D_SENSOR_PWM "|" D_SENSOR_PWM "_i|"
  D_SENSOR_BUZZER "|" D_SENSOR_BUZZER "_i|"
  D_SENSOR_LED_LINK "|" D_SENSOR_LED_LINK "_i|"
  D_SENSOR_I2C_SCL "|" D_SENSOR_I2C_SDA "|"
  D_SENSOR_SPI_MISO "|" D_SENSOR_SPI_MOSI "|" D_SENSOR_SPI_CLK "|" D_SENSOR_SPI_CS "|" D_SENSOR_SPI_DC "|"
  D_SENSOR_SSPI_MISO "|" D_SENSOR_SSPI_MOSI "|" D_SENSOR_SSPI_SCLK "|" D_SENSOR_SSPI_CS "|" D_SENSOR_SSPI_DC "|"
  D_SENSOR_BACKLIGHT "|" D_SENSOR_OLED_RESET "|"
  D_SENSOR_IRSEND "|" D_SENSOR_IRRECV "|"
  D_SENSOR_RFSEND "|" D_SENSOR_RFRECV "|"
  D_SENSOR_DHT11 "|" D_SENSOR_AM2301 "|" D_SENSOR_SI7021 "|" D_SENSOR_DHT11 "_o|"
  D_SENSOR_DS18X20 "|" D_SENSOR_DS18X20 "_o|"
  D_SENSOR_WS2812 "|"
  D_SENSOR_MHZ_TX "|" D_SENSOR_MHZ_RX "|"
  D_SENSOR_PZEM0XX_TX "|" D_SENSOR_PZEM004_RX "|" D_SENSOR_PZEM016_RX "|" D_SENSOR_PZEM017_RX "|"
  D_SENSOR_SAIR_TX "|" D_SENSOR_SAIR_RX "|"
  D_SENSOR_PMS5003_TX "|" D_SENSOR_PMS5003_RX "|"
  D_SENSOR_SDS0X1_TX "|" D_SENSOR_SDS0X1_RX "|"
  D_SENSOR_SBR_TX "|" D_SENSOR_SBR_RX "|"
  D_SENSOR_SR04_TRIG "|" D_SENSOR_SR04_ECHO "|"
  D_SENSOR_SDM120_TX "|" D_SENSOR_SDM120_RX "|"
  D_SENSOR_SDM630_TX "|" D_SENSOR_SDM630_RX "|"
  D_SENSOR_TM1638_CLK "|" D_SENSOR_TM1638_DIO "|" D_SENSOR_TM1638_STB "|"
  D_SENSOR_DFR562 "|"
  D_SENSOR_HX711_SCK "|" D_SENSOR_HX711_DAT "|"
  D_SENSOR_TX2X_TX "|"
  D_SENSOR_TUYA_TX "|" D_SENSOR_TUYA_RX "|"
  D_SENSOR_MGC3130_XFER "|" D_SENSOR_MGC3130_RESET "|"
  D_SENSOR_RF_SENSOR "|"
  D_SENSOR_AZ_TX "|" D_SENSOR_AZ_RX "|"
  D_SENSOR_MAX31855_CS "|" D_SENSOR_MAX31855_CLK "|" D_SENSOR_MAX31855_DO "|"
  D_SENSOR_NRG_SEL "|" D_SENSOR_NRG_SEL "_i|" D_SENSOR_NRG_CF1 "|" D_SENSOR_HLW_CF "|" D_SENSOR_HJL_CF "|"
  D_SENSOR_MCP39F5_TX "|" D_SENSOR_MCP39F5_RX "|" D_SENSOR_MCP39F5_RST "|"
  D_SENSOR_PN532_TX "|" D_SENSOR_PN532_RX "|"
  D_SENSOR_SM16716_CLK "|" D_SENSOR_SM16716_DAT "|" D_SENSOR_SM16716_POWER "|"
  D_SENSOR_MY92X1_DI "|" D_SENSOR_MY92X1_DCKI "|"
  D_SENSOR_CSE7766_TX "|" D_SENSOR_CSE7766_RX "|"
  D_SENSOR_ARIRFRCV "|" D_SENSOR_ARIRFSEL "|"
  D_SENSOR_TXD "|" D_SENSOR_RXD "|"
  D_SENSOR_ROTARY "_1a|" D_SENSOR_ROTARY "_1b|" D_SENSOR_ROTARY "_2a|" D_SENSOR_ROTARY "_2b|"
  D_SENSOR_HRE_CLOCK "|" D_SENSOR_HRE_DATA "|"
  D_SENSOR_ADE7953_IRQ "|"
  D_SENSOR_SOLAXX1_TX "|" D_SENSOR_SOLAXX1_RX "|"
  D_SENSOR_ZIGBEE_TXD "|" D_SENSOR_ZIGBEE_RXD "|"
  D_SENSOR_RDM6300_RX "|"
  D_SENSOR_IBEACON_TX "|" D_SENSOR_IBEACON_RX "|"
  D_SENSOR_A4988_DIR "|" D_SENSOR_A4988_STP "|" D_SENSOR_A4988_ENA "|" D_SENSOR_A4988_MS1 "|" D_SENSOR_A4988_MS2 "|" D_SENSOR_A4988_MS3 "|"
  D_SENSOR_DDS2382_TX "|" D_SENSOR_DDS2382_RX "|"
  D_SENSOR_DDSU666_TX "|" D_SENSOR_DDSU666_RX "|"
  D_SENSOR_SM2135_CLK "|" D_SENSOR_SM2135_DAT "|"
  D_SENSOR_DEEPSLEEP "|" D_SENSOR_EXS_ENABLE "|"
  D_SENSOR_SLAVE_TX "|" D_SENSOR_SLAVE_RX "|" D_SENSOR_SLAVE_RESET "|" D_SENSOR_SLAVE_RESET "_i|"
  D_SENSOR_HPMA_RX "|" D_SENSOR_HPMA_TX "|"
  D_SENSOR_GPS_RX "|" D_SENSOR_GPS_TX "|"
  D_SENSOR_HM10_RX "|" D_SENSOR_HM10_TX "|"
  D_SENSOR_LE01MR_RX "|" D_SENSOR_LE01MR_TX "|"
  D_SENSOR_CC1101_GDO0 "|" D_SENSOR_CC1101_GDO2 "|"
  D_SENSOR_HRXL_RX "|"
  D_SENSOR_ELECTRIQ_MOODL "|"
  D_SENSOR_AS3935 "|"
  D_ANALOG_INPUT "|"
  D_TEMPERATURE "|" D_LIGHT "|"
  D_SENSOR_BUTTON "|" D_SENSOR_BUTTON "_i|"
  D_RANGE "|"
  D_CT_POWER "|"
  D_GPIO_WEBCAM_PWDN "|" D_GPIO_WEBCAM_RESET "|" D_GPIO_WEBCAM_XCLK "|"
  D_GPIO_WEBCAM_SIOD "|" D_GPIO_WEBCAM_SIOC "|"
  D_GPIO_WEBCAM_DATA "|"
  D_GPIO_WEBCAM_VSYNC "|" D_GPIO_WEBCAM_HREF "|" D_GPIO_WEBCAM_PCLK "|"
  D_GPIO_WEBCAM_PSCLK "|"
  D_GPIO_WEBCAM_HSD "|"
  D_GPIO_WEBCAM_PSRCS "|"
  D_SENSOR_BOILER_OT_RX "|" D_SENSOR_BOILER_OT_TX "|"
  D_SENSOR_WINDMETER_SPEED "|" D_SENSOR_BUTTON "_tc|"
  D_SENSOR_BL0940_RX "|"
  D_SENSOR_TELEINFO_RX "|" D_SENSOR_TELEINFO_ENABLE
  ;

const char kSensorNamesFixed[] PROGMEM =
  D_SENSOR_USER;

#define MAX_WEBCAM_DATA  8
#define MAX_WEBCAM_HSD   3

const uint16_t kGpioNiceList[] PROGMEM = {
  GPIO_NONE,                            // Not used
  AGPIO(GPIO_KEY1) + MAX_KEYS,          // Buttons
  AGPIO(GPIO_KEY1_NP) + MAX_KEYS,
  AGPIO(GPIO_KEY1_INV) + MAX_KEYS,
  AGPIO(GPIO_KEY1_INV_NP) + MAX_KEYS,
  AGPIO(GPIO_SWT1) + MAX_SWITCHES,      // User connected external switches
  AGPIO(GPIO_SWT1_NP) + MAX_SWITCHES,
  AGPIO(GPIO_REL1) + MAX_RELAYS,        // Relays
  AGPIO(GPIO_REL1_INV) + MAX_RELAYS,
  AGPIO(GPIO_LED1) + MAX_LEDS,          // Leds
  AGPIO(GPIO_LED1_INV) + MAX_LEDS,
#ifdef USE_COUNTER
  AGPIO(GPIO_CNTR1) + MAX_COUNTERS,     // Counters
  AGPIO(GPIO_CNTR1_NP) + MAX_COUNTERS,
#endif
  AGPIO(GPIO_PWM1) + MAX_PWMS,          // RGB   Red   or C  Cold White
  AGPIO(GPIO_PWM1_INV) + MAX_PWMS,
#ifdef USE_BUZZER
  AGPIO(GPIO_BUZZER),                   // Buzzer
  AGPIO(GPIO_BUZZER_INV),               // Inverted buzzer
#endif
  AGPIO(GPIO_LEDLNK),                   // Link led
  AGPIO(GPIO_LEDLNK_INV),               // Inverted link led
#ifdef USE_I2C
  AGPIO(GPIO_I2C_SCL),                  // I2C SCL
  AGPIO(GPIO_I2C_SDA),                  // I2C SDA
#endif
#ifdef USE_SPI
  AGPIO(GPIO_SPI_MISO),       // SPI MISO
  AGPIO(GPIO_SPI_MOSI),       // SPI MOSI
  AGPIO(GPIO_SPI_CLK),        // SPI Clk
  AGPIO(GPIO_SPI_CS),         // SPI Chip Select
  AGPIO(GPIO_SPI_DC),         // SPI Data Direction
  AGPIO(GPIO_SSPI_MISO),      // Software SPI Master Input Slave Output
  AGPIO(GPIO_SSPI_MOSI),      // Software SPI Master Output Slave Input
  AGPIO(GPIO_SSPI_SCLK),      // Software SPI Serial Clock
  AGPIO(GPIO_SSPI_CS),        // Software SPI Chip Select
  AGPIO(GPIO_SSPI_DC),        // Software SPI Data or Command
#endif
#ifdef USE_DISPLAY
  AGPIO(GPIO_BACKLIGHT),      // Display backlight control
  AGPIO(GPIO_OLED_RESET),     // OLED Display Reset
#endif

  AGPIO(GPIO_TXD),            // Serial interface
  AGPIO(GPIO_RXD),            // Serial interface

#ifdef USE_DHT
  AGPIO(GPIO_DHT11),          // DHT11
  AGPIO(GPIO_DHT22),          // DHT21, DHT22, AM2301, AM2302, AM2321
  AGPIO(GPIO_SI7021),         // iTead SI7021
  AGPIO(GPIO_DHT11_OUT),      // Pseudo Single wire DHT11, DHT21, DHT22, AM2301, AM2302, AM2321
#endif
#ifdef USE_DS18x20
  AGPIO(GPIO_DSB),            // Single wire DS18B20 or DS18S20
  AGPIO(GPIO_DSB_OUT),        // Pseudo Single wire DS18B20 or DS18S20
#endif

// Light
#ifdef USE_LIGHT
#ifdef USE_WS2812
  AGPIO(GPIO_WS2812),         // WS2812 Led string
#endif
#ifdef USE_ARILUX_RF
  AGPIO(GPIO_ARIRFRCV),       // AriLux RF Receive input
  AGPIO(GPIO_ARIRFSEL),       // Arilux RF Receive input selected
#endif
#ifdef USE_MY92X1
  AGPIO(GPIO_DI),             // my92x1 PWM input
  AGPIO(GPIO_DCKI),           // my92x1 CLK input
#endif  // USE_MY92X1
#ifdef USE_SM16716
  AGPIO(GPIO_SM16716_CLK),    // SM16716 CLOCK
  AGPIO(GPIO_SM16716_DAT),    // SM16716 DATA
  AGPIO(GPIO_SM16716_SEL),    // SM16716 SELECT
#endif  // USE_SM16716
#ifdef USE_SM2135
  AGPIO(GPIO_SM2135_CLK),     // SM2135 CLOCK
  AGPIO(GPIO_SM2135_DAT),     // SM2135 DATA
#endif  // USE_SM2135
#ifdef USE_TUYA_MCU
  AGPIO(GPIO_TUYA_TX),        // Tuya Serial interface
  AGPIO(GPIO_TUYA_RX),        // Tuya Serial interface
#endif
#ifdef USE_EXS_DIMMER
  AGPIO(GPIO_EXS_ENABLE),     // EXS MCU Enable
#endif
#ifdef USE_ELECTRIQ_MOODL
  AGPIO(GPIO_ELECTRIQ_MOODL_TX),
#endif
#endif  // USE_LIGHT

#if defined(USE_IR_REMOTE) || defined(USE_IR_REMOTE_FULL)
  AGPIO(GPIO_IRSEND),         // IR remote
#if defined(USE_IR_RECEIVE) || defined(USE_IR_REMOTE_FULL)
  AGPIO(GPIO_IRRECV),         // IR receiver
#endif
#endif

#ifdef USE_RC_SWITCH
  AGPIO(GPIO_RFSEND),         // RF transmitter
  AGPIO(GPIO_RFRECV),         // RF receiver
#endif
#ifdef USE_RF_SENSOR
  AGPIO(GPIO_RF_SENSOR),      // Rf receiver with sensor decoding
#endif
#ifdef USE_SR04
  AGPIO(GPIO_SR04_TRIG),      // SR04 Tri/TXgger pin
  AGPIO(GPIO_SR04_ECHO),      // SR04 Ech/RXo pin
#endif
#ifdef USE_TM1638
  AGPIO(GPIO_TM16CLK),        // TM1638 Clock
  AGPIO(GPIO_TM16DIO),        // TM1638 Data I/O
  AGPIO(GPIO_TM16STB),        // TM1638 Strobe
#endif
#ifdef USE_HX711
  AGPIO(GPIO_HX711_SCK),      // HX711 Load Cell clock
  AGPIO(GPIO_HX711_DAT),      // HX711 Load Cell data
#endif

// Energy sensors
#ifdef USE_ENERGY_SENSOR
#ifdef USE_HLW8012
  AGPIO(GPIO_NRG_SEL),        // HLW8012/HLJ-01 Sel output (1 = Voltage)
  AGPIO(GPIO_NRG_SEL_INV),    // HLW8012/HLJ-01 Sel output (0 = Voltage)
  AGPIO(GPIO_NRG_CF1),        // HLW8012/HLJ-01 CF1 voltage / current
  AGPIO(GPIO_HLW_CF),         // HLW8012 CF power
  AGPIO(GPIO_HJL_CF),         // HJL-01/BL0937 CF power
#endif
#if defined(USE_I2C) && defined(USE_ADE7953)
  AGPIO(GPIO_ADE7953_IRQ),    // ADE7953 IRQ
#endif
#ifdef USE_CSE7766
  AGPIO(GPIO_CSE7766_TX),     // CSE7766 Serial interface (S31 and Pow R2)
  AGPIO(GPIO_CSE7766_RX),     // CSE7766 Serial interface (S31 and Pow R2)
#endif
#ifdef USE_MCP39F501
  AGPIO(GPIO_MCP39F5_TX),     // MCP39F501 Serial interface (Shelly2)
  AGPIO(GPIO_MCP39F5_RX),     // MCP39F501 Serial interface (Shelly2)
  AGPIO(GPIO_MCP39F5_RST),    // MCP39F501 Reset (Shelly2)
#endif
#if defined(USE_PZEM004T) || defined(USE_PZEM_AC) || defined(USE_PZEM_DC)
  AGPIO(GPIO_PZEM0XX_TX),     // PZEM0XX Serial interface
#endif
#ifdef USE_PZEM004T
  AGPIO(GPIO_PZEM004_RX),     // PZEM004T Serial interface
#endif
#ifdef USE_PZEM_AC
  AGPIO(GPIO_PZEM016_RX),     // PZEM-014,016 Serial Modbus interface
#endif
#ifdef USE_PZEM_DC
  AGPIO(GPIO_PZEM017_RX),     // PZEM-003,017 Serial Modbus interface
#endif
#ifdef USE_SDM120
  AGPIO(GPIO_SDM120_TX),      // SDM120 Serial interface
  AGPIO(GPIO_SDM120_RX),      // SDM120 Serial interface
#endif
#ifdef USE_SDM630
  AGPIO(GPIO_SDM630_TX),      // SDM630 Serial interface
  AGPIO(GPIO_SDM630_RX),      // SDM630 Serial interface
#endif
#ifdef USE_DDS2382
  AGPIO(GPIO_DDS2382_TX),     // DDS2382 Serial interface
  AGPIO(GPIO_DDS2382_RX),     // DDS2382 Serial interface
#endif
#ifdef USE_DDSU666
  AGPIO(GPIO_DDSU666_TX),     // DDSU666 Serial interface
  AGPIO(GPIO_DDSU666_RX),     // DDSU666 Serial interface
#endif  // USE_DDSU666
#ifdef USE_SOLAX_X1
  AGPIO(GPIO_SOLAXX1_TX),     // Solax Inverter tx pin
  AGPIO(GPIO_SOLAXX1_RX),     // Solax Inverter rx pin
#endif // USE_SOLAX_X1
#ifdef USE_LE01MR
  AGPIO(GPIO_LE01MR_TX),     // F7F LE-01MR energy meter tx pin
  AGPIO(GPIO_LE01MR_RX),     // F7F LE-01MR energy meter rx pin
#endif // IFDEF:USE_LE01MR
#ifdef USE_BL0940
  AGPIO(GPIO_BL0940_RX),     // BL0940 Serial interface
#endif
#endif  // USE_ENERGY_SENSOR

// Serial
#ifdef USE_SERIAL_BRIDGE
  AGPIO(GPIO_SBR_TX),         // Serial Bridge Serial interface
  AGPIO(GPIO_SBR_RX),         // Serial Bridge Serial interface
#endif
#ifdef USE_ZIGBEE
  AGPIO(GPIO_ZIGBEE_TX),      // Zigbee Serial interface
  AGPIO(GPIO_ZIGBEE_RX),      // Zigbee Serial interface
#endif
#ifdef USE_MHZ19
  AGPIO(GPIO_MHZ_TXD),        // MH-Z19 Serial interface
  AGPIO(GPIO_MHZ_RXD),        // MH-Z19 Serial interface
#endif
#ifdef USE_SENSEAIR
  AGPIO(GPIO_SAIR_TX),        // SenseAir Serial interface
  AGPIO(GPIO_SAIR_RX),        // SenseAir Serial interface
#endif
#ifdef USE_NOVA_SDS
  AGPIO(GPIO_SDS0X1_TX),      // Nova Fitness SDS011 Serial interface
  AGPIO(GPIO_SDS0X1_RX),      // Nova Fitness SDS011 Serial interface
#endif
#ifdef USE_HPMA
  AGPIO(GPIO_HPMA_TX),        // Honeywell HPMA115S0 Serial interface
  AGPIO(GPIO_HPMA_RX),        // Honeywell HPMA115S0 Serial interface
#endif
#ifdef USE_PMS5003
  AGPIO(GPIO_PMS5003_TX),     // Plantower PMS5003 Serial interface
  AGPIO(GPIO_PMS5003_RX),     // Plantower PMS5003 Serial interface
#endif
#if defined(USE_TX20_WIND_SENSOR) || defined(USE_TX23_WIND_SENSOR)
  AGPIO(GPIO_TX2X_TXD_BLACK), // TX20/TX23 Transmission Pin
#endif
#ifdef USE_WINDMETER
  GPIO_WINDMETER_SPEED,
#endif
#ifdef USE_MP3_PLAYER
  AGPIO(GPIO_MP3_DFR562),     // RB-DFR-562, DFPlayer Mini MP3 Player Serial interface
#endif
#ifdef USE_AZ7798
  AGPIO(GPIO_AZ_TXD),         // AZ-Instrument 7798 CO2 datalogger Serial interface
  AGPIO(GPIO_AZ_RXD),         // AZ-Instrument 7798 CO2 datalogger Serial interface
#endif
#ifdef USE_PN532_HSU
  AGPIO(GPIO_PN532_TXD),      // PN532 HSU Tx
  AGPIO(GPIO_PN532_RXD),      // PN532 HSU Rx
#endif
#ifdef USE_TASMOTA_SLAVE
  AGPIO(GPIO_TASMOTASLAVE_TXD),     // Tasmota Slave TX
  AGPIO(GPIO_TASMOTASLAVE_RXD),     // Tasmota Slave RX
  AGPIO(GPIO_TASMOTASLAVE_RST),     // Tasmota Slave Reset
  AGPIO(GPIO_TASMOTASLAVE_RST_INV), // Tasmota Slave Reset Inverted
#endif
#ifdef USE_RDM6300
  AGPIO(GPIO_RDM6300_RX),
#endif
#ifdef USE_IBEACON
  AGPIO(GPIO_IBEACON_TX),
  AGPIO(GPIO_IBEACON_RX),
#endif
#ifdef USE_GPS
  AGPIO(GPIO_GPS_TX),         // GPS serial interface
  AGPIO(GPIO_GPS_RX),         // GPS serial interface
#endif
#ifdef USE_HM10
  AGPIO(GPIO_HM10_TX),         // GPS serial interface
  AGPIO(GPIO_HM10_RX),         // GPS serial interface
#endif
#ifdef USE_OPENTHERM
  GPIO_BOILER_OT_TX,
  GPIO_BOILER_OT_RX,
#endif

#ifdef USE_MGC3130
  AGPIO(GPIO_MGC3130_XFER),
  AGPIO(GPIO_MGC3130_RESET),
#endif
#ifdef USE_MAX31855
  AGPIO(GPIO_MAX31855CS),     // MAX31855 Serial interface
  AGPIO(GPIO_MAX31855CLK),    // MAX31855 Serial interface
  AGPIO(GPIO_MAX31855DO),     // MAX31855 Serial interface
#endif
#ifdef ROTARY_V1
  AGPIO(GPIO_ROT1A),          // Rotary switch1 A Pin
  AGPIO(GPIO_ROT1B),          // Rotary switch1 B Pin
  AGPIO(GPIO_ROT2A),          // Rotary switch2 A Pin
  AGPIO(GPIO_ROT2B),          // Rotary switch2 B Pin
#endif
#ifdef USE_HRE
  AGPIO(GPIO_HRE_CLOCK),
  AGPIO(GPIO_HRE_DATA),
#endif
#ifdef USE_A4988_STEPPER
  AGPIO(GPIO_A4988_DIR),     // A4988 direction pin
  AGPIO(GPIO_A4988_STP),     // A4988 step pin
  // folowing are not mandatory
  AGPIO(GPIO_A4988_ENA),     // A4988 enabled pin
  AGPIO(GPIO_A4988_MS1),     // A4988 microstep pin1
  AGPIO(GPIO_A4988_MS2),     // A4988 microstep pin2
  AGPIO(GPIO_A4988_MS3),     // A4988 microstep pin3
#endif
#ifdef USE_DEEPSLEEP
  AGPIO(GPIO_DEEPSLEEP),
#endif
#ifdef USE_KEELOQ
  AGPIO(GPIO_CC1101_GDO0),    // CC1101 pin for RX
  AGPIO(GPIO_CC1101_GDO2),    // CC1101 pin for RX
#endif
#ifdef USE_HRXL
  AGPIO(GPIO_HRXL_RX),
#endif
#ifdef USE_AS3935
  AGPIO(GPIO_AS3935),
#endif
#ifdef USE_TELEINFO
  AGPIO(GPIO_TELEINFO_RX),
  AGPIO(GPIO_TELEINFO_ENABLE),
#endif
/*
#ifndef USE_ADC_VCC
  AGPIO(ADC0_INPUT),          // Analog input
  AGPIO(ADC0_TEMP),           // Thermistor
  AGPIO(ADC0_LIGHT),          // Light sensor
  AGPIO(ADC0_BUTTON),         // Button
  AGPIO(ADC0_BUTTON_INV),
  AGPIO(ADC0_RANGE),          // Range
  AGPIO(ADC0_CT_POWER),       // Current
#endif
*/
#ifdef USE_WEBCAM
  AGPIO(GPIO_WEBCAM_PWDN),
  AGPIO(GPIO_WEBCAM_RESET),
  AGPIO(GPIO_WEBCAM_XCLK),
  AGPIO(GPIO_WEBCAM_SIOD),
  AGPIO(GPIO_WEBCAM_SIOC),
  AGPIO(GPIO_WEBCAM_DATA) + MAX_WEBCAM_DATA,
  AGPIO(GPIO_WEBCAM_VSYNC),
  AGPIO(GPIO_WEBCAM_HREF),
  AGPIO(GPIO_WEBCAM_PCLK),
  AGPIO(GPIO_WEBCAM_PSCLK),
  AGPIO(GPIO_WEBCAM_HSD) + MAX_WEBCAM_HSD,
  AGPIO(GPIO_WEBCAM_PSRCS),
#endif
  AGPIO(GPIO_KEY1_TC) + MAX_KEYS
};

//********************************************************************************************

#define MAX_GPIO_PIN       40   // Number of supported GPIO
#define MIN_FLASH_PINS     4    // Number of flash chip pins unusable for configuration (GPIO6, 7, 8 and 11)
#define MAX_USER_PINS      36   // MAX_GPIO_PIN - MIN_FLASH_PINS
#define WEMOS_MODULE       0    // Wemos module

//                                  0 1 2 3 4 5 6 7 8 9101112131415161718192021222324252627282930313233343536373839
const char PINS_WEMOS[] PROGMEM = "IOTXIORXIOIOflashcFLFLolIOIOIOIOIOIOIOIOIOIOIOIOIOIOIOIOIOIOIOIOIOIOA6A7A0IoIoA3";

//********************************************************************************************

typedef struct MYIO {
  uint16_t      io[MAX_GPIO_PIN];
} myio;                         // 40 * 2 = 80 bytes

typedef struct MYCFGIO {
  uint16_t      io[MAX_USER_PINS];
} mycfgio;                      // 36 * 2 = 72 bytes

#define GPIO_FLAG_USED       0  // Currently no flags used

typedef union {
  uint16_t data;
  struct {
    uint16_t spare00 : 1;
    uint16_t spare01 : 1;
    uint16_t spare02 : 1;
    uint16_t spare03 : 1;
    uint16_t spare04 : 1;
    uint16_t spare05 : 1;
    uint16_t spare06 : 1;
    uint16_t spare07 : 1;
    uint16_t spare08 : 1;
    uint16_t spare09 : 1;
    uint16_t spare10 : 1;
    uint16_t spare11 : 1;
    uint16_t spare12 : 1;
    uint16_t spare13 : 1;
    uint16_t spare14 : 1;
    uint16_t spare15 : 1;
  };
} gpio_flag;                    // 2 bytes

typedef struct MYTMPLT {
  mycfgio      gp;              // 72 bytes
  gpio_flag    flag;            // 2 bytes
} mytmplt;                      // 74 bytes

/********************************************************************************************/
// Supported hardware modules
enum SupportedModules {
  WEMOS, ESP32_CAM_AITHINKER,
  MAXMODULE};

#define USER_MODULE        255

const char kModuleNames[] PROGMEM =
  "ESP32-DevKit|ESP32 Cam AiThinker";

// Default module settings
const uint8_t kModuleNiceList[MAXMODULE] PROGMEM = {
  WEMOS,
  ESP32_CAM_AITHINKER
};

const mytmplt kModules PROGMEM =
{                              // WEMOS - Espressif ESP32-DevKitC - Any ESP32 device like WeMos and NodeMCU hardware (ESP32)
  AGPIO(GPIO_USER),            // 0       (I)O                GPIO0, ADC2_CH1, TOUCH1, RTC_GPIO11, CLK_OUT1, EMAC_TX_CLK
  AGPIO(GPIO_USER),            // 1       IO     TXD0         GPIO1, U0TXD, CLK_OUT3, EMAC_RXD2
  AGPIO(GPIO_USER),            // 2       IO                  GPIO2, ADC2_CH2, TOUCH2, RTC_GPIO12, HSPIWP, HS2_DATA0, SD_DATA0
  AGPIO(GPIO_USER),            // 3       IO     RXD0         GPIO3, U0RXD, CLK_OUT2
  AGPIO(GPIO_USER),            // 4       IO                  GPIO4, ADC2_CH0, TOUCH0, RTC_GPIO10, HSPIHD, HS2_DATA1, SD_DATA1, EMAC_TX_ER
  AGPIO(GPIO_USER),            // 5       IO                  GPIO5, VSPICS0, HS1_DATA6, EMAC_RX_CLK
                               // 6       IO                  GPIO6, Flash CLK
                               // 7       IO                  GPIO7, Flash D0
                               // 8       IO                  GPIO8, Flash D1
  AGPIO(GPIO_USER),            // 9       IO                  GPIO9, Flash D2, U1RXD
  AGPIO(GPIO_USER),            // 10      IO                  GPIO10, Flash D3, U1TXD
                               // 11      IO                  GPIO11, Flash CMD
  AGPIO(GPIO_USER),            // 12      (I)O                GPIO12, ADC2_CH5, TOUCH5, RTC_GPIO15, MTDI, HSPIQ, HS2_DATA2, SD_DATA2, EMAC_TXD3       (If driven High, flash voltage (VDD_SDIO) is 1.8V not default 3.3V. Has internal pull-down, so unconnected = Low = 3.3V. May prevent flashing and/or booting if 3.3V flash is connected and pulled high. See ESP32 datasheet for more details.)
  AGPIO(GPIO_USER),            // 13      IO                  GPIO13, ADC2_CH4, TOUCH4, RTC_GPIO14, MTCK, HSPID, HS2_DATA3, SD_DATA3, EMAC_RX_ER
  AGPIO(GPIO_USER),            // 14      IO                  GPIO14, ADC2_CH6, TOUCH6, RTC_GPIO16, MTMS, HSPICLK, HS2_CLK, SD_CLK, EMAC_TXD2
  AGPIO(GPIO_USER),            // 15      (I)O                GPIO15, ADC2_CH3, TOUCH3, MTDO, HSPICS0, RTC_GPIO13, HS2_CMD, SD_CMD, EMAC_RXD3         (If driven Low, silences boot messages from normal boot. Has internal pull-up, so unconnected = High = normal output.)
  AGPIO(GPIO_USER),            // 16      IO                  GPIO16, HS1_DATA4, U2RXD, EMAC_CLK_OUT
  AGPIO(GPIO_USER),            // 17      IO                  GPIO17, HS1_DATA5, U2TXD, EMAC_CLK_OUT_180
  AGPIO(GPIO_USER),            // 18      IO                  GPIO18, VSPICLK, HS1_DATA7
  AGPIO(GPIO_USER),            // 19      IO                  GPIO19, VSPIQ, U0CTS, EMAC_TXD0
  0,                           // 20
  AGPIO(GPIO_USER),            // 21      IO                  GPIO21, VSPIHD, EMAC_TX_EN
  AGPIO(GPIO_USER),            // 22      IO      LED         GPIO22, VSPIWP, U0RTS, EMAC_TXD1
  AGPIO(GPIO_USER),            // 23      IO                  GPIO23, VSPID, HS1_STROBE
  0,                           // 24
  AGPIO(GPIO_USER),            // 25      IO                  GPIO25, DAC_1, ADC2_CH8, RTC_GPIO6, EMAC_RXD0
  AGPIO(GPIO_USER),            // 26      IO                  GPIO26, DAC_2, ADC2_CH9, RTC_GPIO7, EMAC_RXD1
  AGPIO(GPIO_USER),            // 27      IO                  GPIO27, ADC2_CH7, TOUCH7, RTC_GPIO17, EMAC_RX_DV
  0,                           // 28
  0,                           // 29
  0,                           // 30
  0,                           // 31
  AGPIO(GPIO_USER),            // 32      IO                  GPIO32, XTAL_32K_P (32.768 kHz crystal oscillator input), ADC1_CH4, TOUCH9, RTC_GPIO9
  AGPIO(GPIO_USER),            // 33      IO                  GPIO33, XTAL_32K_N (32.768 kHz crystal oscillator output), ADC1_CH5, TOUCH8, RTC_GPIO8
  AGPIO(GPIO_USER),            // 34      I   NO PULLUP       GPIO34, ADC1_CH6, RTC_GPIO4
  AGPIO(GPIO_USER),            // 35      I   NO PULLUP       GPIO35, ADC1_CH7, RTC_GPIO5
  AGPIO(GPIO_USER),            // 36      I   NO PULLUP       GPIO36, SENSOR_VP, ADC_H, ADC1_CH0, RTC_GPIO0
  0,                           // 37          NO PULLUP
  0,                           // 38          NO PULLUP
  AGPIO(GPIO_USER),            // 39      I   NO PULLUP       GPIO39, SENSOR_VN, ADC1_CH3, ADC_H, RTC_GPIO3
  0                            // Flag
};

#endif  // ESP32

#endif  // _TASMOTA_TEMPLATE_ESP32_H_
