/*
  tasmota_template.h - template settings for Tasmota

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

#ifndef _TASMOTA_TEMPLATE_H_
#define _TASMOTA_TEMPLATE_H_

// User selectable GPIO functionality
// ATTENTION: Only add at the end of this list just before GPIO_SENSOR_END
//            Then add the same name(s) in a nice location in array kGpioNiceList
enum UserSelectablePins {
  GPIO_NONE,           // Not used
  GPIO_DHT11,          // DHT11
  GPIO_DHT22,          // DHT21, DHT22, AM2301, AM2302, AM2321
  GPIO_SI7021,         // iTead SI7021
  GPIO_DSB,            // Single wire DS18B20 or DS18S20
  GPIO_I2C_SCL,        // I2C SCL
  GPIO_I2C_SDA,        // I2C SDA
  GPIO_WS2812,         // WS2812 Led string
  GPIO_IRSEND,         // IR remote
  GPIO_SWT1,           // User connected external switches
  GPIO_SWT2,
  GPIO_SWT3,
  GPIO_SWT4,
  GPIO_SWT5,
  GPIO_SWT6,
  GPIO_SWT7,
  GPIO_SWT8,
  GPIO_KEY1,           // Button usually connected to GPIO0
  GPIO_KEY2,
  GPIO_KEY3,
  GPIO_KEY4,
  GPIO_REL1,           // Relays
  GPIO_REL2,
  GPIO_REL3,
  GPIO_REL4,
  GPIO_REL5,
  GPIO_REL6,
  GPIO_REL7,
  GPIO_REL8,
  GPIO_REL1_INV,
  GPIO_REL2_INV,
  GPIO_REL3_INV,
  GPIO_REL4_INV,
  GPIO_REL5_INV,
  GPIO_REL6_INV,
  GPIO_REL7_INV,
  GPIO_REL8_INV,
  GPIO_PWM1,           // RGB   Red   or C  Cold White
  GPIO_PWM2,           // RGB   Green or CW Warm White
  GPIO_PWM3,           // RGB   Blue
  GPIO_PWM4,           // RGBW  (Cold) White
  GPIO_PWM5,           // RGBCW Warm White
  GPIO_CNTR1,
  GPIO_CNTR2,
  GPIO_CNTR3,
  GPIO_CNTR4,
  GPIO_PWM1_INV,       // RGB   Red   or C  Cold White
  GPIO_PWM2_INV,       // RGB   Green or CW Warm White
  GPIO_PWM3_INV,       // RGB   Blue
  GPIO_PWM4_INV,       // RGBW  (Cold) White
  GPIO_PWM5_INV,       // RGBCW Warm White
  GPIO_IRRECV,         // IR receiver
  GPIO_LED1,           // Leds
  GPIO_LED2,
  GPIO_LED3,
  GPIO_LED4,
  GPIO_LED1_INV,
  GPIO_LED2_INV,
  GPIO_LED3_INV,
  GPIO_LED4_INV,
  GPIO_MHZ_TXD,        // MH-Z19 Serial interface
  GPIO_MHZ_RXD,        // MH-Z19 Serial interface
  GPIO_PZEM0XX_TX,     // PZEM0XX Serial interface
  GPIO_PZEM004_RX,     // PZEM004T Serial interface
  GPIO_SAIR_TX,        // SenseAir Serial interface
  GPIO_SAIR_RX,        // SenseAir Serial interface
  GPIO_SPI_CS,         // SPI Chip Select
  GPIO_SPI_DC,         // SPI Data Direction
  GPIO_BACKLIGHT,      // Display backlight control
  GPIO_PMS5003,        // Plantower PMS5003 Serial interface
  GPIO_SDS0X1_RX,      // Nova Fitness SDS011 Serial interface
  GPIO_SBR_TX,         // Serial Bridge Serial interface
  GPIO_SBR_RX,         // Serial Bridge Serial interface
  GPIO_SR04_TRIG,      // SR04 Trigger/TX pin
  GPIO_SR04_ECHO,      // SR04 Echo/RX pin
  GPIO_SDM120_TX,      // SDM120 Serial interface
  GPIO_SDM120_RX,      // SDM120 Serial interface
  GPIO_SDM630_TX,      // SDM630 Serial interface
  GPIO_SDM630_RX,      // SDM630 Serial interface
  GPIO_TM16CLK,        // TM1638 Clock
  GPIO_TM16DIO,        // TM1638 Data I/O
  GPIO_TM16STB,        // TM1638 Strobe
  GPIO_SWT1_NP,        // User connected external switches
  GPIO_SWT2_NP,
  GPIO_SWT3_NP,
  GPIO_SWT4_NP,
  GPIO_SWT5_NP,
  GPIO_SWT6_NP,
  GPIO_SWT7_NP,
  GPIO_SWT8_NP,
  GPIO_KEY1_NP,        // Button usually connected to GPIO0
  GPIO_KEY2_NP,
  GPIO_KEY3_NP,
  GPIO_KEY4_NP,
  GPIO_CNTR1_NP,
  GPIO_CNTR2_NP,
  GPIO_CNTR3_NP,
  GPIO_CNTR4_NP,
  GPIO_PZEM016_RX,     // PZEM-014,016 Serial Modbus interface
  GPIO_PZEM017_RX,     // PZEM-003,017 Serial Modbus interface
  GPIO_MP3_DFR562,     // RB-DFR-562, DFPlayer Mini MP3 Player
  GPIO_SDS0X1_TX,      // Nova Fitness SDS011 Serial interface
  GPIO_HX711_SCK,      // HX711 Load Cell clock
  GPIO_HX711_DAT,      // HX711 Load Cell data
  GPIO_TX2X_TXD_BLACK, // TX20/TX23 Transmission Pin
  GPIO_RFSEND,         // RF transmitter
  GPIO_RFRECV,         // RF receiver
  GPIO_TUYA_TX,        // Tuya Serial interface
  GPIO_TUYA_RX,        // Tuya Serial interface
  GPIO_MGC3130_XFER,   // MGC3130 Transfer
  GPIO_MGC3130_RESET,  // MGC3130 Reset
  GPIO_SSPI_MISO,      // Software SPI Master Input Slave Output
  GPIO_SSPI_MOSI,      // Software SPI Master Output Slave Input
  GPIO_SSPI_SCLK,      // Software SPI Serial Clock
  GPIO_SSPI_CS,        // Software SPI Chip Select
  GPIO_SSPI_DC,        // Software SPI Data or Command
  GPIO_RF_SENSOR,      // Rf receiver with sensor decoding
  GPIO_AZ_TXD,         // AZ-Instrument 7798 Serial interface
  GPIO_AZ_RXD,         // AZ-Instrument 7798 Serial interface
  GPIO_MAX31855CS,     // MAX31855 Serial interface
  GPIO_MAX31855CLK,    // MAX31855 Serial interface
  GPIO_MAX31855DO,     // MAX31855 Serial interface
  GPIO_KEY1_INV,       // Inverted buttons
  GPIO_KEY2_INV,
  GPIO_KEY3_INV,
  GPIO_KEY4_INV,
  GPIO_KEY1_INV_NP,    // Inverted buttons without pull-up
  GPIO_KEY2_INV_NP,
  GPIO_KEY3_INV_NP,
  GPIO_KEY4_INV_NP,
  GPIO_NRG_SEL,        // HLW8012/HLJ-01 Sel output (1 = Voltage)
  GPIO_NRG_SEL_INV,    // HLW8012/HLJ-01 Sel output (0 = Voltage)
  GPIO_NRG_CF1,        // HLW8012/HLJ-01 CF1 voltage / current
  GPIO_HLW_CF,         // HLW8012 CF power
  GPIO_HJL_CF,         // HJL-01/BL0937 CF power
  GPIO_MCP39F5_TX,     // MCP39F501 Serial interface (Shelly2)
  GPIO_MCP39F5_RX,     // MCP39F501 Serial interface (Shelly2)
  GPIO_MCP39F5_RST,    // MCP39F501 Reset (Shelly2)
  GPIO_PN532_TXD,      // PN532 NFC Serial Tx
  GPIO_PN532_RXD,      // PN532 NFC Serial Rx
  GPIO_SM16716_CLK,    // SM16716 CLOCK
  GPIO_SM16716_DAT,    // SM16716 DATA
  GPIO_SM16716_SEL,    // SM16716 SELECT
  GPIO_DI,             // my92x1 PWM input
  GPIO_DCKI,           // my92x1 CLK input
  GPIO_CSE7766_TX,     // CSE7766 Serial interface (S31 and Pow R2) - Not used anymore 20200121
  GPIO_CSE7766_RX,     // CSE7766 Serial interface (S31 and Pow R2)
  GPIO_ARIRFRCV,       // AriLux RF Receive input
  GPIO_TXD,            // Serial interface
  GPIO_RXD,            // Serial interface
  GPIO_ROT1A,          // Rotary switch1 A Pin
  GPIO_ROT1B,          // Rotary switch1 B Pin
  GPIO_ROT2A,          // Rotary switch2 A Pin
  GPIO_ROT2B,          // Rotary switch2 B Pin
  GPIO_HRE_CLOCK,      // Clock/Power line for HR-E Water Meter
  GPIO_HRE_DATA,       // Data line for HR-E Water Meter
  GPIO_ADE7953_IRQ,    // ADE7953 IRQ
  GPIO_LEDLNK,         // Link led
  GPIO_LEDLNK_INV,     // Inverted link led
  GPIO_ARIRFSEL,       // Arilux RF Receive input selected
  GPIO_BUZZER,         // Buzzer
  GPIO_BUZZER_INV,     // Inverted buzzer
  GPIO_OLED_RESET,     // OLED Display Reset
  GPIO_SOLAXX1_TX,     // Solax Inverter tx pin
  GPIO_SOLAXX1_RX,     // Solax Inverter rx pin
  GPIO_ZIGBEE_TX,      // Zigbee Serial interface
  GPIO_ZIGBEE_RX,      // Zigbee Serial interface
  GPIO_RDM6300_RX,     // RDM6300 RX
  GPIO_IBEACON_TX,     // HM17 IBEACON TX
  GPIO_IBEACON_RX,     // HM17 IBEACON RX
  GPIO_A4988_DIR,      // A4988 direction pin
  GPIO_A4988_STP,      // A4988 step pin
  GPIO_A4988_ENA,      // A4988 enabled pin
  GPIO_A4988_MS1,      // A4988 microstep pin1
  GPIO_A4988_MS2,      // A4988 microstep pin2
  GPIO_A4988_MS3,      // A4988 microstep pin3
  GPIO_DDS2382_TX,     // DDS2382 Serial interface
  GPIO_DDS2382_RX,     // DDS2382 Serial interface
  GPIO_DDSU666_TX,     // DDSU666 Serial interface
  GPIO_DDSU666_RX,     // DDSU666 Serial interface
  GPIO_SM2135_CLK,     // SM2135 Clk
  GPIO_SM2135_DAT,     // SM2135 Dat
  GPIO_DEEPSLEEP,      // Kill switch for deepsleep
  GPIO_EXS_ENABLE,     // EXS MCU Enable
  GPIO_TASMOTASLAVE_TXD,     // Slave TX
  GPIO_TASMOTASLAVE_RXD,     // Slave RX
  GPIO_TASMOTASLAVE_RST,     // Slave Reset Pin
  GPIO_TASMOTASLAVE_RST_INV, // Slave Reset Inverted
  GPIO_HPMA_RX,        // Honeywell HPMA115S0 Serial interface
  GPIO_HPMA_TX,        // Honeywell HPMA115S0 Serial interface
  GPIO_GPS_RX,         // GPS serial interface
  GPIO_GPS_TX,         // GPS serial interface
  GPIO_DSB_OUT,        // Pseudo Single wire DS18B20 or DS18S20
  GPIO_DHT11_OUT,      // Pseudo Single wire DHT11, DHT21, DHT22, AM2301, AM2302, AM2321
  GPIO_HM10_RX,        // HM10-BLE-Mijia-bridge serial interface
  GPIO_HM10_TX,        // HM10-BLE-Mijia-bridge serial interface
  GPIO_LE01MR_RX,      // F&F LE-01MR energy meter
  GPIO_LE01MR_TX,      // F&F LE-01MR energy meter
  GPIO_CC1101_GDO0,    // CC1101 pin for RX
  GPIO_CC1101_GDO2,    // CC1101 pin for RX
  GPIO_HRXL_RX,       // Data from MaxBotix HRXL sonar range sensor
  GPIO_ELECTRIQ_MOODL_TX, // ElectriQ iQ-wifiMOODL Serial TX
  GPIO_SENSOR_END };

// Programmer selectable GPIO functionality
enum ProgramSelectablePins {
  GPIO_FIX_START = 251,
  GPIO_SPI_MISO,       // SPI MISO library fixed pin GPIO12
  GPIO_SPI_MOSI,       // SPI MOSI library fixed pin GPIO13
  GPIO_SPI_CLK,        // SPI Clk library fixed pin GPIO14
  GPIO_USER,           // User configurable needs to be 255
  GPIO_MAX };

// Text in webpage Module Parameters and commands GPIOS and GPIO
const char kSensorNames[] PROGMEM =
  D_SENSOR_NONE "|"
  D_SENSOR_DHT11 "|" D_SENSOR_AM2301 "|" D_SENSOR_SI7021 "|"
  D_SENSOR_DS18X20 "|"
  D_SENSOR_I2C_SCL "|" D_SENSOR_I2C_SDA "|"
  D_SENSOR_WS2812 "|"
  D_SENSOR_IRSEND "|"
  D_SENSOR_SWITCH "1|" D_SENSOR_SWITCH "2|" D_SENSOR_SWITCH "3|" D_SENSOR_SWITCH "4|" D_SENSOR_SWITCH "5|" D_SENSOR_SWITCH "6|" D_SENSOR_SWITCH "7|" D_SENSOR_SWITCH "8|"
  D_SENSOR_BUTTON "1|" D_SENSOR_BUTTON "2|" D_SENSOR_BUTTON "3|" D_SENSOR_BUTTON "4|"
  D_SENSOR_RELAY "1|" D_SENSOR_RELAY "2|" D_SENSOR_RELAY "3|" D_SENSOR_RELAY "4|" D_SENSOR_RELAY "5|" D_SENSOR_RELAY "6|" D_SENSOR_RELAY "7|" D_SENSOR_RELAY "8|"
  D_SENSOR_RELAY "1i|" D_SENSOR_RELAY "2i|" D_SENSOR_RELAY "3i|" D_SENSOR_RELAY "4i|" D_SENSOR_RELAY "5i|" D_SENSOR_RELAY "6i|" D_SENSOR_RELAY "7i|" D_SENSOR_RELAY "8i|"
  D_SENSOR_PWM "1|" D_SENSOR_PWM "2|" D_SENSOR_PWM "3|" D_SENSOR_PWM "4|" D_SENSOR_PWM "5|"
  D_SENSOR_COUNTER "1|" D_SENSOR_COUNTER "2|" D_SENSOR_COUNTER "3|" D_SENSOR_COUNTER "4|"
  D_SENSOR_PWM "1i|" D_SENSOR_PWM "2i|" D_SENSOR_PWM "3i|" D_SENSOR_PWM "4i|" D_SENSOR_PWM "5i|"
  D_SENSOR_IRRECV "|"
  D_SENSOR_LED "1|" D_SENSOR_LED "2|" D_SENSOR_LED "3|" D_SENSOR_LED "4|"
  D_SENSOR_LED "1i|" D_SENSOR_LED "2i|" D_SENSOR_LED "3i|" D_SENSOR_LED "4i|"
  D_SENSOR_MHZ_TX "|" D_SENSOR_MHZ_RX "|"
  D_SENSOR_PZEM0XX_TX "|" D_SENSOR_PZEM004_RX "|"
  D_SENSOR_SAIR_TX "|" D_SENSOR_SAIR_RX "|"
  D_SENSOR_SPI_CS "|" D_SENSOR_SPI_DC "|" D_SENSOR_BACKLIGHT "|"
  D_SENSOR_PMS5003 "|" D_SENSOR_SDS0X1_RX "|"
  D_SENSOR_SBR_TX "|" D_SENSOR_SBR_RX "|"
  D_SENSOR_SR04_TRIG "|" D_SENSOR_SR04_ECHO "|"
  D_SENSOR_SDM120_TX "|" D_SENSOR_SDM120_RX "|"
  D_SENSOR_SDM630_TX "|" D_SENSOR_SDM630_RX "|"
  D_SENSOR_TM1638_CLK "|" D_SENSOR_TM1638_DIO "|" D_SENSOR_TM1638_STB "|"
  D_SENSOR_SWITCH "1n|" D_SENSOR_SWITCH "2n|" D_SENSOR_SWITCH "3n|" D_SENSOR_SWITCH "4n|" D_SENSOR_SWITCH "5n|" D_SENSOR_SWITCH "6n|" D_SENSOR_SWITCH "7n|" D_SENSOR_SWITCH "8n|"
  D_SENSOR_BUTTON "1n|" D_SENSOR_BUTTON "2n|" D_SENSOR_BUTTON "3n|" D_SENSOR_BUTTON "4n|"
  D_SENSOR_COUNTER "1n|" D_SENSOR_COUNTER "2n|" D_SENSOR_COUNTER "3n|" D_SENSOR_COUNTER "4n|"
  D_SENSOR_PZEM016_RX "|" D_SENSOR_PZEM017_RX "|"
  D_SENSOR_DFR562 "|" D_SENSOR_SDS0X1_TX "|"
  D_SENSOR_HX711_SCK "|" D_SENSOR_HX711_DAT "|"
  D_SENSOR_TX2X_TX "|"
  D_SENSOR_RFSEND "|" D_SENSOR_RFRECV "|"
  D_SENSOR_TUYA_TX "|" D_SENSOR_TUYA_RX "|"
  D_SENSOR_MGC3130_XFER "|" D_SENSOR_MGC3130_RESET "|"
  D_SENSOR_SSPI_MISO "|" D_SENSOR_SSPI_MOSI "|" D_SENSOR_SSPI_SCLK "|" D_SENSOR_SSPI_CS "|" D_SENSOR_SSPI_DC "|"
  D_SENSOR_RF_SENSOR "|"
  D_SENSOR_AZ_TX "|" D_SENSOR_AZ_RX "|"
  D_SENSOR_MAX31855_CS "|" D_SENSOR_MAX31855_CLK "|" D_SENSOR_MAX31855_DO "|"
  D_SENSOR_BUTTON "1i|" D_SENSOR_BUTTON "2i|" D_SENSOR_BUTTON "3i|" D_SENSOR_BUTTON "4i|"
  D_SENSOR_BUTTON "1in|" D_SENSOR_BUTTON "2in|" D_SENSOR_BUTTON "3in|" D_SENSOR_BUTTON "4in|"
  D_SENSOR_NRG_SEL "|" D_SENSOR_NRG_SEL "i|" D_SENSOR_NRG_CF1 "|" D_SENSOR_HLW_CF "|" D_SENSOR_HJL_CF "|"
  D_SENSOR_MCP39F5_TX "|" D_SENSOR_MCP39F5_RX "|" D_SENSOR_MCP39F5_RST "|"
  D_SENSOR_PN532_TX "|" D_SENSOR_PN532_RX "|"
  D_SENSOR_SM16716_CLK "|" D_SENSOR_SM16716_DAT "|" D_SENSOR_SM16716_POWER "|"
  D_SENSOR_MY92X1_DI "|" D_SENSOR_MY92X1_DCKI "|"
  D_SENSOR_CSE7766_TX "|" D_SENSOR_CSE7766_RX "|"
  D_SENSOR_ARIRFRCV "|" D_SENSOR_TXD "|" D_SENSOR_RXD "|"
  D_SENSOR_ROTARY "1a|" D_SENSOR_ROTARY "1b|" D_SENSOR_ROTARY "2a|" D_SENSOR_ROTARY "2b|"
  D_SENSOR_HRE_CLOCK "|" D_SENSOR_HRE_DATA "|"
  D_SENSOR_ADE7953_IRQ "|"
  D_SENSOR_LED_LINK "|" D_SENSOR_LED_LINK "i|"
  D_SENSOR_ARIRFSEL "|"
  D_SENSOR_BUZZER "|" D_SENSOR_BUZZER "i|"
  D_SENSOR_OLED_RESET "|"
  D_SENSOR_SOLAXX1_TX "|" D_SENSOR_SOLAXX1_RX "|"
  D_SENSOR_ZIGBEE_TXD "|" D_SENSOR_ZIGBEE_RXD "|"
  D_SENSOR_RDM6300_RX "|"
  D_SENSOR_IBEACON_TX "|" D_SENSOR_IBEACON_RX "|"
  D_SENSOR_A4988_DIR "|" D_SENSOR_A4988_STP "|" D_SENSOR_A4988_ENA "|" D_SENSOR_A4988_MS1 "|" D_SENSOR_A4988_MS2 "|" D_SENSOR_A4988_MS3 "|"
  D_SENSOR_DDS2382_TX "|" D_SENSOR_DDS2382_RX "|"
  D_SENSOR_DDSU666_TX "|" D_SENSOR_DDSU666_RX "|"
  D_SENSOR_SM2135_CLK "|" D_SENSOR_SM2135_DAT "|"
  D_SENSOR_DEEPSLEEP "|" D_SENSOR_EXS_ENABLE "|"
  D_SENSOR_SLAVE_TX "|" D_SENSOR_SLAVE_RX "|" D_SENSOR_SLAVE_RESET "|" D_SENSOR_SLAVE_RESET "i|"
  D_SENSOR_HPMA_RX "|" D_SENSOR_HPMA_TX "|"
  D_SENSOR_GPS_RX "|" D_SENSOR_GPS_TX "|"
  D_SENSOR_DS18X20 "o|" D_SENSOR_DHT11 "o|"
  D_SENSOR_HM10_RX "|" D_SENSOR_HM10_TX "|"
  D_SENSOR_LE01MR_RX "|" D_SENSOR_LE01MR_TX "|"
  D_SENSOR_CC1101_GDO0 "|" D_SENSOR_CC1101_GDO2 "|"
  D_SENSOR_HRXL_RX "|"
  D_SENSOR_ELECTRIQ_MOODL
  ;

const char kSensorNamesFixed[] PROGMEM =
  D_SENSOR_SPI_MISO "|" D_SENSOR_SPI_MOSI "|" D_SENSOR_SPI_CLK "|"
  D_SENSOR_USER;

const uint8_t kGpioNiceList[] PROGMEM = {
  GPIO_NONE,           // Not used
  GPIO_KEY1,           // Buttons
  GPIO_KEY1_NP,
  GPIO_KEY1_INV,
  GPIO_KEY1_INV_NP,
  GPIO_KEY2,
  GPIO_KEY2_NP,
  GPIO_KEY2_INV,
  GPIO_KEY2_INV_NP,
  GPIO_KEY3,
  GPIO_KEY3_NP,
  GPIO_KEY3_INV,
  GPIO_KEY3_INV_NP,
  GPIO_KEY4,
  GPIO_KEY4_NP,
  GPIO_KEY4_INV,
  GPIO_KEY4_INV_NP,
  GPIO_SWT1,           // User connected external switches
  GPIO_SWT1_NP,
  GPIO_SWT2,
  GPIO_SWT2_NP,
  GPIO_SWT3,
  GPIO_SWT3_NP,
  GPIO_SWT4,
  GPIO_SWT4_NP,
  GPIO_SWT5,
  GPIO_SWT5_NP,
  GPIO_SWT6,
  GPIO_SWT6_NP,
  GPIO_SWT7,
  GPIO_SWT7_NP,
  GPIO_SWT8,
  GPIO_SWT8_NP,
  GPIO_REL1,           // Relays
  GPIO_REL1_INV,
  GPIO_REL2,
  GPIO_REL2_INV,
  GPIO_REL3,
  GPIO_REL3_INV,
  GPIO_REL4,
  GPIO_REL4_INV,
  GPIO_REL5,
  GPIO_REL5_INV,
  GPIO_REL6,
  GPIO_REL6_INV,
  GPIO_REL7,
  GPIO_REL7_INV,
  GPIO_REL8,
  GPIO_REL8_INV,
  GPIO_LED1,           // Leds
  GPIO_LED1_INV,
  GPIO_LED2,
  GPIO_LED2_INV,
  GPIO_LED3,
  GPIO_LED3_INV,
  GPIO_LED4,
  GPIO_LED4_INV,
  GPIO_LEDLNK,         // Link led
  GPIO_LEDLNK_INV,     // Inverted link led
  GPIO_PWM1,           // RGB   Red   or C  Cold White
  GPIO_PWM1_INV,
  GPIO_PWM2,           // RGB   Green or CW Warm White
  GPIO_PWM2_INV,
  GPIO_PWM3,           // RGB   Blue
  GPIO_PWM3_INV,
  GPIO_PWM4,           // RGBW  (Cold) White
  GPIO_PWM4_INV,
  GPIO_PWM5,           // RGBCW Warm White
  GPIO_PWM5_INV,
#ifdef USE_COUNTER
  GPIO_CNTR1,          // Counters
  GPIO_CNTR1_NP,
  GPIO_CNTR2,
  GPIO_CNTR2_NP,
  GPIO_CNTR3,
  GPIO_CNTR3_NP,
  GPIO_CNTR4,
  GPIO_CNTR4_NP,
#endif
#ifdef USE_BUZZER
  GPIO_BUZZER,         // Buzzer
  GPIO_BUZZER_INV,     // Inverted buzzer
#endif
  GPIO_TXD,            // Serial interface
  GPIO_RXD,            // Serial interface
#ifdef USE_I2C
  GPIO_I2C_SCL,        // I2C SCL
  GPIO_I2C_SDA,        // I2C SDA
#endif
#ifdef USE_SPI
  GPIO_SPI_CS,         // SPI Chip Select
  GPIO_SPI_DC,         // SPI Data Direction
  GPIO_SSPI_MISO,      // Software SPI Master Input Slave Output
  GPIO_SSPI_MOSI,      // Software SPI Master Output Slave Input
  GPIO_SSPI_SCLK,      // Software SPI Serial Clock
  GPIO_SSPI_CS,        // Software SPI Chip Select
  GPIO_SSPI_DC,        // Software SPI Data or Command
#endif
#ifdef USE_DISPLAY
  GPIO_BACKLIGHT,      // Display backlight control
  GPIO_OLED_RESET,     // OLED Display Reset
#endif
#ifdef USE_DHT
  GPIO_DHT11,          // DHT11
  GPIO_DHT22,          // DHT21, DHT22, AM2301, AM2302, AM2321
  GPIO_SI7021,         // iTead SI7021
  GPIO_DHT11_OUT,      // Pseudo Single wire DHT11, DHT21, DHT22, AM2301, AM2302, AM2321
#endif
#ifdef USE_DS18x20
  GPIO_DSB,            // Single wire DS18B20 or DS18S20
  GPIO_DSB_OUT,        // Pseudo Single wire DS18B20 or DS18S20
#endif

// Light
#ifdef USE_LIGHT
#ifdef USE_WS2812
  GPIO_WS2812,         // WS2812 Led string
#endif
#ifdef USE_ARILUX_RF
  GPIO_ARIRFRCV,       // AriLux RF Receive input
  GPIO_ARIRFSEL,       // Arilux RF Receive input selected
#endif
#ifdef USE_MY92X1
  GPIO_DI,             // my92x1 PWM input
  GPIO_DCKI,           // my92x1 CLK input
#endif  // USE_MY92X1
#ifdef USE_SM16716
  GPIO_SM16716_CLK,    // SM16716 CLOCK
  GPIO_SM16716_DAT,    // SM16716 DATA
  GPIO_SM16716_SEL,    // SM16716 SELECT
#endif  // USE_SM16716
#ifdef USE_SM2135
  GPIO_SM2135_CLK,     // SM2135 CLOCK
  GPIO_SM2135_DAT,     // SM2135 DATA
#endif  // USE_SM2135
#ifdef USE_TUYA_MCU
  GPIO_TUYA_TX,        // Tuya Serial interface
  GPIO_TUYA_RX,        // Tuya Serial interface
#endif
#ifdef USE_EXS_DIMMER
  GPIO_EXS_ENABLE,     // EXS MCU Enable
#endif
#ifdef USE_ELECTRIQ_MOODL
  GPIO_ELECTRIQ_MOODL_TX,
#endif
#endif  // USE_LIGHT

#if defined(USE_IR_REMOTE) || defined(USE_IR_REMOTE_FULL)
  GPIO_IRSEND,         // IR remote
#if defined(USE_IR_RECEIVE) || defined(USE_IR_REMOTE_FULL)
  GPIO_IRRECV,         // IR receiver
#endif
#endif

#ifdef USE_RC_SWITCH
  GPIO_RFSEND,         // RF transmitter
  GPIO_RFRECV,         // RF receiver
#endif
#ifdef USE_RF_SENSOR
  GPIO_RF_SENSOR,      // Rf receiver with sensor decoding
#endif
#ifdef USE_SR04
  GPIO_SR04_TRIG,      // SR04 Tri/TXgger pin
  GPIO_SR04_ECHO,      // SR04 Ech/RXo pin
#endif
#ifdef USE_TM1638
  GPIO_TM16CLK,        // TM1638 Clock
  GPIO_TM16DIO,        // TM1638 Data I/O
  GPIO_TM16STB,        // TM1638 Strobe
#endif
#ifdef USE_HX711
  GPIO_HX711_SCK,      // HX711 Load Cell clock
  GPIO_HX711_DAT,      // HX711 Load Cell data
#endif

// Energy sensors
#ifdef USE_ENERGY_SENSOR
#ifdef USE_HLW8012
  GPIO_NRG_SEL,        // HLW8012/HLJ-01 Sel output (1 = Voltage)
  GPIO_NRG_SEL_INV,    // HLW8012/HLJ-01 Sel output (0 = Voltage)
  GPIO_NRG_CF1,        // HLW8012/HLJ-01 CF1 voltage / current
  GPIO_HLW_CF,         // HLW8012 CF power
  GPIO_HJL_CF,         // HJL-01/BL0937 CF power
#endif
#if defined(USE_I2C) && defined(USE_ADE7953)
  GPIO_ADE7953_IRQ,    // ADE7953 IRQ
#endif
#ifdef USE_CSE7766
  GPIO_CSE7766_TX,     // CSE7766 Serial interface (S31 and Pow R2)
  GPIO_CSE7766_RX,     // CSE7766 Serial interface (S31 and Pow R2)
#endif
#ifdef USE_MCP39F501
  GPIO_MCP39F5_TX,     // MCP39F501 Serial interface (Shelly2)
  GPIO_MCP39F5_RX,     // MCP39F501 Serial interface (Shelly2)
  GPIO_MCP39F5_RST,    // MCP39F501 Reset (Shelly2)
#endif
#if defined(USE_PZEM004T) || defined(USE_PZEM_AC) || defined(USE_PZEM_DC)
  GPIO_PZEM0XX_TX,     // PZEM0XX Serial interface
#endif
#ifdef USE_PZEM004T
  GPIO_PZEM004_RX,     // PZEM004T Serial interface
#endif
#ifdef USE_PZEM_AC
  GPIO_PZEM016_RX,     // PZEM-014,016 Serial Modbus interface
#endif
#ifdef USE_PZEM_DC
  GPIO_PZEM017_RX,     // PZEM-003,017 Serial Modbus interface
#endif
#ifdef USE_SDM120
  GPIO_SDM120_TX,      // SDM120 Serial interface
  GPIO_SDM120_RX,      // SDM120 Serial interface
#endif
#ifdef USE_SDM630
  GPIO_SDM630_TX,      // SDM630 Serial interface
  GPIO_SDM630_RX,      // SDM630 Serial interface
#endif
#ifdef USE_DDS2382
  GPIO_DDS2382_TX,     // DDS2382 Serial interface
  GPIO_DDS2382_RX,     // DDS2382 Serial interface
#endif
#ifdef USE_DDSU666
  GPIO_DDSU666_TX,     // DDSU666 Serial interface
  GPIO_DDSU666_RX,     // DDSU666 Serial interface
#endif  // USE_DDSU666
#ifdef USE_SOLAX_X1
  GPIO_SOLAXX1_TX,     // Solax Inverter tx pin
  GPIO_SOLAXX1_RX,     // Solax Inverter rx pin
#endif // USE_SOLAX_X1
#ifdef USE_LE01MR
  GPIO_LE01MR_RX,     // F7F LE-01MR energy meter rx pin
  GPIO_LE01MR_TX,     // F7F LE-01MR energy meter tx pin
#endif // IFDEF:USE_LE01MR
#endif  // USE_ENERGY_SENSOR

// Serial
#ifdef USE_SERIAL_BRIDGE
  GPIO_SBR_TX,         // Serial Bridge Serial interface
  GPIO_SBR_RX,         // Serial Bridge Serial interface
#endif
#ifdef USE_ZIGBEE
  GPIO_ZIGBEE_TX,      // Zigbee Serial interface
  GPIO_ZIGBEE_RX,      // Zigbee Serial interface
#endif
#ifdef USE_MHZ19
  GPIO_MHZ_TXD,        // MH-Z19 Serial interface
  GPIO_MHZ_RXD,        // MH-Z19 Serial interface
#endif
#ifdef USE_SENSEAIR
  GPIO_SAIR_TX,        // SenseAir Serial interface
  GPIO_SAIR_RX,        // SenseAir Serial interface
#endif
#ifdef USE_NOVA_SDS
  GPIO_SDS0X1_TX,      // Nova Fitness SDS011 Serial interface
  GPIO_SDS0X1_RX,      // Nova Fitness SDS011 Serial interface
#endif
#ifdef USE_HPMA
  GPIO_HPMA_TX,      // Honeywell HPMA115S0 Serial interface
  GPIO_HPMA_RX,      // Honeywell HPMA115S0 Serial interface
#endif
#ifdef USE_PMS5003
  GPIO_PMS5003,        // Plantower PMS5003 Serial interface
#endif
#if defined(USE_TX20_WIND_SENSOR) || defined(USE_TX23_WIND_SENSOR)
  GPIO_TX2X_TXD_BLACK, // TX20/TX23 Transmission Pin
#endif
#ifdef USE_MP3_PLAYER
  GPIO_MP3_DFR562,     // RB-DFR-562, DFPlayer Mini MP3 Player Serial interface
#endif
#ifdef USE_AZ7798
  GPIO_AZ_TXD,         // AZ-Instrument 7798 CO2 datalogger Serial interface
  GPIO_AZ_RXD,         // AZ-Instrument 7798 CO2 datalogger Serial interface
#endif
#ifdef USE_PN532_HSU
  GPIO_PN532_TXD,      // PN532 HSU Tx
  GPIO_PN532_RXD,      // PN532 HSU Rx
#endif
#ifdef USE_TASMOTA_SLAVE
  GPIO_TASMOTASLAVE_TXD,     // Tasmota Slave TX
  GPIO_TASMOTASLAVE_RXD,     // Tasmota Slave RX
  GPIO_TASMOTASLAVE_RST,     // Tasmota Slave Reset
  GPIO_TASMOTASLAVE_RST_INV, // Tasmota Slave Reset Inverted
#endif
#ifdef USE_RDM6300
  GPIO_RDM6300_RX,
#endif
#ifdef USE_IBEACON
  GPIO_IBEACON_RX,
  GPIO_IBEACON_TX,
#endif
#ifdef USE_GPS
  GPIO_GPS_RX,         // GPS serial interface
  GPIO_GPS_TX,         // GPS serial interface
#endif
#ifdef USE_HM10
  GPIO_HM10_RX,         // GPS serial interface
  GPIO_HM10_TX,         // GPS serial interface
#endif

#ifdef USE_MGC3130
  GPIO_MGC3130_XFER,
  GPIO_MGC3130_RESET,
#endif
#ifdef USE_MAX31855
  GPIO_MAX31855CS,     // MAX31855 Serial interface
  GPIO_MAX31855CLK,    // MAX31855 Serial interface
  GPIO_MAX31855DO,     // MAX31855 Serial interface
#endif
#ifdef ROTARY_V1
  GPIO_ROT1A,          // Rotary switch1 A Pin
  GPIO_ROT1B,          // Rotary switch1 B Pin
  GPIO_ROT2A,          // Rotary switch2 A Pin
  GPIO_ROT2B,          // Rotary switch2 B Pin
#endif
#ifdef USE_HRE
  GPIO_HRE_CLOCK,
  GPIO_HRE_DATA,
#endif
#ifdef USE_A4988_STEPPER
  GPIO_A4988_DIR,     // A4988 direction pin
  GPIO_A4988_STP,     // A4988 step pin
  // folowing are not mandatory
  GPIO_A4988_ENA,     // A4988 enabled pin
  GPIO_A4988_MS1,     // A4988 microstep pin1
  GPIO_A4988_MS2,     // A4988 microstep pin2
  GPIO_A4988_MS3,     // A4988 microstep pin3
#endif
#ifdef USE_DEEPSLEEP
  GPIO_DEEPSLEEP,
#endif
#ifdef USE_KEELOQ
  GPIO_CC1101_GDO0,    // CC1101 pin for RX
  GPIO_CC1101_GDO2,    // CC1101 pin for RX
#endif
#ifdef USE_HRXL
  GPIO_HRXL_RX,
#endif
};

/********************************************************************************************/

// User selectable ADC0 functionality
enum UserSelectableAdc0 {
  ADC0_NONE,           // Not used
  ADC0_INPUT,          // Analog input
  ADC0_TEMP,           // Thermistor
  ADC0_LIGHT,          // Light sensor
  ADC0_BUTTON,         // Button
  ADC0_BUTTON_INV,
  ADC0_RANGE,          // Range
  ADC0_CT_POWER,       // Current
//  ADC0_SWITCH,         // Switch
//  ADC0_SWITCH_INV,
  ADC0_END };

// Programmer selectable ADC0 functionality
enum ProgramSelectableAdc0 {
  ADC0_FIX_START = 14,
  ADC0_USER,           // User configurable needs to be 15
  ADC0_MAX };

// Text in webpage Module Parameters and commands ADC
const char kAdc0Names[] PROGMEM =
  D_SENSOR_NONE "|" D_ANALOG_INPUT "|"
  D_TEMPERATURE "|" D_LIGHT "|"
  D_SENSOR_BUTTON "|" D_SENSOR_BUTTON "i|"
  D_RANGE "|"
  D_CT_POWER "|"
//  D_SENSOR_SWITCH "|" D_SENSOR_SWITCH "i|"
  ;

/********************************************************************************************/

#ifdef ESP8266

#define MAX_GPIO_PIN       17   // Number of supported GPIO
#define MIN_FLASH_PINS     4    // Number of flash chip pins unusable for configuration (GPIO6, 7, 8 and 11)

const char PINS_WEMOS[] PROGMEM = "D3TXD4RXD2D1flashcFLFLolD6D7D5D8D0A0";

#else  // ESP32

// esp32 has more pins
#define USER_MODULE        255
#define MAX_GPIO_PIN       44   // Number of supported GPIO
#define MIN_FLASH_PINS     4    // Number of flash chip pins unusable for configuration (GPIO6, 7, 8 and 11)

const char PINS_WEMOS[] PROGMEM = "00010203040506070809101112131415161718192021222324252627282930313233343536373839";

#endif  // ESP8266

/********************************************************************************************/

typedef struct MYIO {
  uint8_t      io[MAX_GPIO_PIN];
} myio;

typedef struct MYCFGIO {
  uint8_t      io[MAX_GPIO_PIN - MIN_FLASH_PINS];
} mycfgio;

#define GPIO_FLAG_USED           0  // Currently two flags used

#define GPIO_FLAG_SPARE04       16
#define GPIO_FLAG_SPARE05       32
#define GPIO_FLAG_SPARE06       64
#define GPIO_FLAG_SPARE07      128

typedef union {
  uint8_t data;
  struct {
    uint8_t adc0 : 4;               // Allow ADC0 when define USE_ADC_VCC is disabled
    uint8_t spare04 : 1;
    uint8_t spare05 : 1;
    uint8_t spare06 : 1;
    uint8_t spare07 : 1;
  };
} gpio_flag;

typedef struct MYTMPLT {
  mycfgio      gp;
  gpio_flag    flag;
} mytmplt;

/********************************************************************************************/

#ifdef ESP8266

// Supported hardware modules
enum SupportedModules {
  SONOFF_BASIC, SONOFF_RF, SONOFF_SV, SONOFF_TH, SONOFF_DUAL, SONOFF_POW, SONOFF_4CH, SONOFF_S2X, SLAMPHER, SONOFF_TOUCH,
  SONOFF_LED, CH1, CH4, MOTOR, ELECTRODRAGON, EXS_RELAY, WION, WEMOS, SONOFF_DEV, H801,
  SONOFF_SC, SONOFF_BN, SONOFF_4CHPRO, HUAFAN_SS, SONOFF_BRIDGE, SONOFF_B1, AILIGHT, SONOFF_T11, SONOFF_T12, SONOFF_T13,
  SUPLA1, WITTY, YUNSHAN, MAGICHOME, LUANIHVIO, KMC_70011, ARILUX_LC01, ARILUX_LC11, SONOFF_DUAL_R2, ARILUX_LC06,
  SONOFF_S31, ZENGGE_ZF_WF017, SONOFF_POW_R2, SONOFF_IFAN02, BLITZWOLF_BWSHP, SHELLY1, SHELLY2, PHILIPS, NEO_COOLCAM, ESP_SWITCH,
  OBI, TECKIN, APLIC_WDP303075, TUYA_DIMMER, GOSUND, ARMTRONIX_DIMMERS, SK03_TUYA, PS_16_DZ, TECKIN_US, MANZOKU_EU_4,
  OBI2, YTF_IR_BRIDGE, DIGOO, KA10, ZX2820, MI_DESK_LAMP, SP10, WAGA, SYF05, SONOFF_L1,
  SONOFF_IFAN03, EXS_DIMMER, PWM_DIMMER, SONOFF_D1,
  MAXMODULE};

#define USER_MODULE        255

const char kModuleNames[] PROGMEM =
  "Sonoff Basic|Sonoff RF|Sonoff SV|Sonoff TH|Sonoff Dual|Sonoff Pow|Sonoff 4CH|Sonoff S2X|Slampher|Sonoff Touch|"
  "Sonoff LED|1 Channel|4 Channel|Motor C/AC|ElectroDragon|EXS Relay(s)|WiOn|Generic|Sonoff Dev|H801|"
  "Sonoff SC|Sonoff BN-SZ|Sonoff 4CH Pro|Huafan SS|Sonoff Bridge|Sonoff B1|AiLight|Sonoff T1 1CH|Sonoff T1 2CH|Sonoff T1 3CH|"
  "Supla Espablo|Witty Cloud|Yunshan Relay|MagicHome|Luani HVIO|KMC 70011|Arilux LC01|Arilux LC11|Sonoff Dual R2|Arilux LC06|"
  "Sonoff S31|Zengge WF017|Sonoff Pow R2|Sonoff iFan02|BlitzWolf SHP|Shelly 1|Shelly 2|Xiaomi Philips|Neo Coolcam|ESP Switch|"
  "OBI Socket|Teckin|AplicWDP303075|Tuya MCU|Gosund SP1 v23|ARMTR Dimmer|SK03 Outdoor|PS-16-DZ|Teckin US|Manzoku strip|"
  "OBI Socket 2|YTF IR Bridge|Digoo DG-SP202|KA10|Luminea ZX2820|Mi Desk Lamp|SP10|WAGA CHCZ02MB|SYF05|Sonoff L1|"
  "Sonoff iFan03|EXS Dimmer|PWM Dimmer|Sonoff D1"
  ;

const uint8_t kModuleNiceList[] PROGMEM = {
  SONOFF_BASIC,        // Sonoff Relay Devices
  SONOFF_RF,
  SONOFF_TH,
  SONOFF_DUAL,
  SONOFF_DUAL_R2,
  SONOFF_POW,
  SONOFF_POW_R2,
  SONOFF_4CH,
  SONOFF_4CHPRO,
  SONOFF_S31,          // Sonoff Socket Relay Devices with Energy Monitoring
  SONOFF_S2X,          // Sonoff Socket Relay Devices
  SONOFF_TOUCH,        // Sonoff Switch Devices
  SONOFF_T11,
  SONOFF_T12,
  SONOFF_T13,
#ifdef USE_SONOFF_D1
  SONOFF_D1,           // Sonoff D1
#endif
  SONOFF_LED,          // Sonoff Light Devices
  SONOFF_BN,
#ifdef USE_SONOFF_L1
  SONOFF_L1,
#endif
  SONOFF_B1,           // Sonoff Light Bulbs
  SLAMPHER,
#ifdef USE_SONOFF_SC
  SONOFF_SC,           // Sonoff Environmemtal Sensor
#endif
#ifdef USE_SONOFF_IFAN
  SONOFF_IFAN02,       // Sonoff Fan
  SONOFF_IFAN03,
#endif
#ifdef USE_SONOFF_RF
  SONOFF_BRIDGE,       // Sonoff Bridge
#endif
  SONOFF_SV,           // Sonoff Development Devices
  SONOFF_DEV,
  CH1,                 // Relay Devices
  CH4,
  MOTOR,
  ELECTRODRAGON,
  EXS_RELAY,
  SUPLA1,
  LUANIHVIO,
  YUNSHAN,
  WION,
  SHELLY1,
  SHELLY2,
  BLITZWOLF_BWSHP,     // Socket Relay Devices with Energy Monitoring
  TECKIN,
  TECKIN_US,
  APLIC_WDP303075,
  GOSUND,
  ZX2820,
  SK03_TUYA,
  DIGOO,
  KA10,
  SP10,
  WAGA,
  NEO_COOLCAM,         // Socket Relay Devices
  OBI,
  OBI2,
  MANZOKU_EU_4,
  ESP_SWITCH,          // Switch Devices
#ifdef USE_TUYA_MCU
  TUYA_DIMMER,         // Dimmer Devices
#endif
#ifdef USE_ARMTRONIX_DIMMERS
  ARMTRONIX_DIMMERS,
#endif
#ifdef USE_PS_16_DZ
  PS_16_DZ,
#endif
#ifdef USE_EXS_DIMMER
  EXS_DIMMER,
#endif
#ifdef USE_PWM_DIMMER
  PWM_DIMMER,
#endif
  H801,                // Light Devices
  MAGICHOME,
  ARILUX_LC01,
  ARILUX_LC06,
  ARILUX_LC11,
  ZENGGE_ZF_WF017,
  HUAFAN_SS,
#ifdef ROTARY_V1
  MI_DESK_LAMP,
#endif
  KMC_70011,
  AILIGHT,             // Light Bulbs
  PHILIPS,
  SYF05,
  YTF_IR_BRIDGE,
  WITTY,               // Development Devices
  WEMOS
};

// Default module settings
const mytmplt kModules[MAXMODULE] PROGMEM = {
  {                   // SONOFF_BASIC - Sonoff Basic (ESP8266)
    GPIO_KEY1,        // GPIO00 Button
    GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
    GPIO_USER,        // GPIO02 Only available on newer Sonoff Basic R2 V1
    GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
    GPIO_USER,        // GPIO04 Optional sensor
    0,                // GPIO05
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
    GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off) - Link and Power status
    GPIO_USER,        // GPIO14 Optional sensor
    0,                // GPIO15
    0,                // GPIO16
    0                 // ADC0 Analog input
  },
  {                   // SONOFF_RF - Sonoff RF (ESP8266)
    GPIO_KEY1,        // GPIO00 Button
    GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
    GPIO_USER,        // GPIO02 Optional sensor
    GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
    GPIO_USER,        // GPIO04 Optional sensor
    0,
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
    GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off) - Link and Power status
    GPIO_USER,        // GPIO14 Optional sensor
    0, 0, 0
  },
  {                   // SONOFF_SV - Sonoff SV (ESP8266)
    GPIO_KEY1,        // GPIO00 Button
    GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
    0,
    GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
    GPIO_USER,        // GPIO04 Optional sensor
    GPIO_USER,        // GPIO05 Optional sensor
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
    GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off) - Link and Power status
    GPIO_USER,        // GPIO14 Optional sensor
    0, 0,
    ADC0_USER         // ADC0 Analog input
  },
  {                   // SONOFF_TH - Sonoff TH10/16 (ESP8266)
    GPIO_KEY1,        // GPIO00 Button
    GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
    0,
    GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
    GPIO_USER,        // GPIO04 Optional sensor
    0,
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
    GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off) - Link and Power status
    GPIO_USER,        // GPIO14 Optional sensor
    0, 0, 0
  },
  {                   // SONOFF_DUAL - Sonoff Dual (ESP8266)
    0,
    GPIO_TXD,         // GPIO01 Relay control
    0,
    GPIO_RXD,         // GPIO03 Relay control
    GPIO_USER,        // GPIO04 Optional sensor
    0,
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    0,
    GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off) - Link and Power status
    GPIO_USER,        // GPIO14 Optional sensor
    0, 0, 0
  },
  {                   // SONOFF_POW - Sonoff Pow (ESP8266 - HLW8012)
    GPIO_KEY1,        // GPIO00 Button
    0, 0, 0, 0,
    GPIO_NRG_SEL,     // GPIO05 HLW8012 Sel output (1 = Voltage)
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
    GPIO_NRG_CF1,     // GPIO13 HLW8012 CF1 voltage / current
    GPIO_HLW_CF,      // GPIO14 HLW8012 CF power
    GPIO_LED1,        // GPIO15 Blue Led (0 = On, 1 = Off) - Link and Power status
    0, 0
  },
  {                   // SONOFF_4CH - Sonoff 4CH (ESP8285)
    GPIO_KEY1,        // GPIO00 Button 1
    GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
    GPIO_USER,        // GPIO02 Optional sensor
    GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
    GPIO_REL3,        // GPIO04 Sonoff 4CH Red Led and Relay 3 (0 = Off, 1 = On)
    GPIO_REL2,        // GPIO05 Sonoff 4CH Red Led and Relay 2 (0 = Off, 1 = On)
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    GPIO_KEY2,        // GPIO09 Button 2
    GPIO_KEY3,        // GPIO10 Button 3
                      // GPIO11 (SD_CMD   Flash)
    GPIO_REL1,        // GPIO12 Red Led and Relay 1 (0 = Off, 1 = On) - Link and Power status
    GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off)
    GPIO_KEY4,        // GPIO14 Button 4
    GPIO_REL4,        // GPIO15 Red Led and Relay 4 (0 = Off, 1 = On)
    0, 0
  },
  {                   // SONOFF_S2X - Sonoff S20, S22 and S26 Smart Socket (ESP8266)
    GPIO_KEY1,        // GPIO00 Button
    GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
    GPIO_USER,        // GPIO02 Optional sensor
    GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
    0, 0,
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On) - Link and Power status
    GPIO_LED1_INV,    // GPIO13 Green/Blue Led (0 = On, 1 = Off)
    0, 0, 0, 0
  },
  {                   // SLAMPHER - Slampher (ESP8266)
    GPIO_KEY1,        // GPIO00 Button
    GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
    0,
    GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
    0, 0,
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
    GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off) - Link and Power status
    0, 0, 0, 0
  },
  {                   // SONOFF_TOUCH - Sonoff Touch (ESP8285)
    GPIO_KEY1,        // GPIO00 Button
    GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
    0,
    GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
    0, 0,
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09
    0,                // GPIO10
                      // GPIO11 (SD_CMD   Flash)
    GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On) - Link and Power status
    GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off)
    0, 0, 0, 0
  },
  {                   // SONOFF_LED - Sonoff LED (ESP8266)
    GPIO_KEY1,        // GPIO00 Button
    0, 0, 0,
    GPIO_USER,        // GPIO04 Optional sensor (PWM3 Green)
    GPIO_USER,        // GPIO05 Optional sensor (PWM2 Red)
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_PWM1,        // GPIO12 Cold light (PWM0 Cold)
    GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off) - Link and Power status
    GPIO_PWM2,        // GPIO14 Warm light (PWM1 Warm)
    GPIO_USER,        // GPIO15 Optional sensor (PWM4 Blue)
    0, 0
  },
  {                   // CH1 - 1 Channel Inching/Latching Relay using (PSA-B01 - ESP8266 and PSF-B01 - ESP8285)
    GPIO_KEY1,        // GPIO00 Button
    0, 0, 0, 0, 0,
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
    GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off) - Link and Power status
    0, 0, 0, 0
  },
  {                   // CH4 - 4 Channel Inching/Latching Relays (ESP8266)
    0,
    GPIO_TXD,         // GPIO01 Relay control
    0,
    GPIO_RXD,         // GPIO03 Relay control
    0, 0,
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    0,
    GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off) - Link and Power status
    0, 0, 0, 0
  },
  {                   // MOTOR - Motor Clockwise / Anti clockwise (PSA-B01 - ESP8266)
    GPIO_KEY1,        // GPIO00 Button
    0, 0, 0, 0, 0,
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
    GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off) - Link and Power status
    0, 0, 0, 0
  },
  {                   // ELECTRODRAGON - ElectroDragon IoT Relay Board (ESP8266)
    GPIO_KEY2,        // GPIO00 Button 2
    GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
    GPIO_KEY1,        // GPIO02 Button 1
    GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
    GPIO_USER,        // GPIO04 Optional sensor
    GPIO_USER,        // GPIO05 Optional sensor
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_REL2,        // GPIO12 Red Led and Relay 2 (0 = Off, 1 = On)
    GPIO_REL1,        // GPIO13 Red Led and Relay 1 (0 = Off, 1 = On)
    GPIO_USER,        // GPIO14 Optional sensor
    GPIO_USER,        // GPIO15 Optional sensor
    GPIO_LED1,        // GPIO16 Green/Blue Led (1 = On, 0 = Off) - Link and Power status
    ADC0_USER         // ADC0   A0 Analog input
  },
  {                   // EXS_RELAY - ES-Store Latching relay(s) (ESP8266)
                      // https://ex-store.de/ESP8266-WiFi-Relay-V31
                      //   V3.1 Module Pin 1 VCC 3V3, Module Pin 6 GND
                      // https://ex-store.de/2-Kanal-WiFi-WLan-Relay-V5-Blackline-fuer-Unterputzmontage
    GPIO_USER,        // GPIO00 V3.1 Module Pin 8 - V5.0 Module Pin 4
    GPIO_USER,        // GPIO01 UART0_TXD V3.1 Module Pin 2 - V5.0 Module Pin 3
    GPIO_USER,        // GPIO02 V3.1 Module Pin 7
    GPIO_USER,        // GPIO03 UART0_RXD V3.1 Module Pin 3
    GPIO_USER,        // GPIO04 V3.1 Module Pin 10 - V5.0 Module Pin 2
    GPIO_USER,        // GPIO05 V3.1 Module Pin 9 - V5.0 Module Pin 1
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_REL1,        // GPIO12 Relay1 ( 1 = Off)
    GPIO_REL2,        // GPIO13 Relay1 ( 1 = On)
    GPIO_USER,        // GPIO14 V3.1 Module Pin 5 - V5.0 GPIO_REL3_INV Relay2 ( 1 = Off)
    GPIO_LED1,        // GPIO15 V5.0 LED1 - Link and Power status
    GPIO_USER,        // GPIO16 V3.1 Module Pin 4 - V5.0 GPIO_REL4_INV Relay2 ( 1 = On)
    0
  },
  {                   // WION - Indoor Tap (ESP8266)
                      // https://www.amazon.com/gp/product/B00ZYLUBJU/ref=s9_acsd_al_bw_c_x_3_w
    GPIO_USER,        // GPIO00 Optional sensor (pm clock)
    0,
    GPIO_LED1,        // GPIO02 Green Led (1 = On, 0 = Off) - Link and Power status
    0, 0, 0,
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_USER,        // GPIO12 Optional sensor (pm data)
    GPIO_KEY1,        // GPIO13 Button
    0,
    GPIO_REL1,        // GPIO15 Relay (0 = Off, 1 = On)
    0, 0
  },
  {                   // WEMOS - Any ESP8266/ESP8285 device like WeMos and NodeMCU hardware (ESP8266)
    GPIO_USER,        // GPIO00 D3 Wemos Button Shield
    GPIO_USER,        // GPIO01 TX Serial RXD
    GPIO_USER,        // GPIO02 D4 Wemos DHT Shield
    GPIO_USER,        // GPIO03 RX Serial TXD and Optional sensor
    GPIO_USER,        // GPIO04 D2 Wemos I2C SDA
    GPIO_USER,        // GPIO05 D1 Wemos I2C SCL / Wemos Relay Shield (0 = Off, 1 = On) / Wemos WS2812B RGB led Shield
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    GPIO_USER,        // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    GPIO_USER,        // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_USER,        // GPIO12 D6
    GPIO_USER,        // GPIO13 D7
    GPIO_USER,        // GPIO14 D5
    GPIO_USER,        // GPIO15 D8
    GPIO_USER,        // GPIO16 D0 Wemos Wake
    ADC0_USER         // ADC0 A0 Analog input
  },
  {                   // SONOFF_DEV - Sonoff Dev (ESP8266)
    GPIO_KEY1,        // GPIO00 E-FW Button
    GPIO_USER,        // GPIO01 TX Serial RXD and Optional sensor
    0,                // GPIO02
    GPIO_USER,        // GPIO03 RX Serial TXD and Optional sensor
    GPIO_USER,        // GPIO04 Optional sensor
    GPIO_USER,        // GPIO05 Optional sensor
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_USER,        // GPIO12
    GPIO_USER,        // GPIO13 BLUE LED
    GPIO_USER,        // GPIO14 Optional sensor
    0,                // GPIO15
    0,                // GPIO16
    ADC0_USER         // ADC0 A0 Analog input
  },
  {                   // H801 - Lixada H801 Wifi (ESP8266)
    GPIO_USER,        // GPIO00 E-FW Button
    GPIO_LED1,        // GPIO01 Green LED - Link and Power status
    GPIO_USER,        // GPIO02 TX and Optional sensor - Pin next to TX on the PCB
    GPIO_USER,        // GPIO03 RX and Optional sensor - Pin next to GND on the PCB
    GPIO_PWM5,        // GPIO04 W2 - PWM5
    GPIO_LED2_INV,    // GPIO05 Red LED
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_PWM3,        // GPIO12 Blue
    GPIO_PWM2,        // GPIO13 Green
    GPIO_PWM4,        // GPIO14 W1 - PWM4
    GPIO_PWM1,        // GPIO15 Red
    0, 0
  },
  {                   // SONOFF_SC - onoff SC (ESP8266)
    GPIO_KEY1,        // GPIO00 Button
    GPIO_TXD,         // GPIO01 RXD to ATMEGA328P
    GPIO_USER,        // GPIO02 Optional sensor
    GPIO_RXD,         // GPIO03 TXD to ATMEGA328P
    0, 0,
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    0,
    GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off) - Link and Power status
    0, 0, 0, 0
  },
  {                   // SONOFF_BN - Sonoff BN-SZ01 Ceiling led (ESP8285)
    0, 0, 0, 0, 0, 0,
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09
    0,                // GPIO10
                      // GPIO11 (SD_CMD   Flash)
    GPIO_PWM1,        // GPIO12 Light
    GPIO_LED1_INV,    // GPIO13 Red Led (0 = On, 1 = Off) - Link and Power status
    0, 0, 0, 0
  },
  {                   // SONOFF_4CHPRO - Sonoff 4CH Pro (ESP8285)
    GPIO_KEY1,        // GPIO00 Button 1
    GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
    GPIO_USER,        // GPIO02 Optional sensor
    GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
    GPIO_REL3,        // GPIO04 Sonoff 4CH Red Led and Relay 3 (0 = Off, 1 = On)
    GPIO_REL2,        // GPIO05 Sonoff 4CH Red Led and Relay 2 (0 = Off, 1 = On)
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    GPIO_KEY2,        // GPIO09 Button 2
    GPIO_KEY3,        // GPIO10 Button 3
                      // GPIO11 (SD_CMD   Flash)
    GPIO_REL1,        // GPIO12 Red Led and Relay 1 (0 = Off, 1 = On)
    GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off) - Link and Power status
    GPIO_KEY4,        // GPIO14 Button 4
    GPIO_REL4,        // GPIO15 Red Led and Relay 4 (0 = Off, 1 = On)
    0, 0
  },
  {                   // HUAFAN_SS - Hua Fan Smart Socket (ESP8266) - like Sonoff Pow
    GPIO_LEDLNK_INV,  // GPIO00 Blue Led (0 = On, 1 = Off) - Link status
    0, 0,
    GPIO_LED1_INV,    // GPIO03 Red Led (0 = On, 1 = Off) - Power status
    GPIO_KEY1,        // GPIO04 Button
    GPIO_REL1_INV,    // GPIO05 Relay (0 = On, 1 = Off)
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_NRG_CF1,     // GPIO12 HLW8012 CF1 voltage / current
    GPIO_NRG_SEL,     // GPIO13 HLW8012 Sel output (1 = Voltage)
    GPIO_HLW_CF,      // GPIO14 HLW8012 CF power
    0, 0, 0
  },
  {                   // SONOFF_BRIDGE - Sonoff RF Bridge 433 (ESP8285)
    GPIO_KEY1,        // GPIO00 Button
    GPIO_TXD,         // GPIO01 RF bridge control
    GPIO_USER,        // GPIO02 Optional sensor
    GPIO_RXD,         // GPIO03 RF bridge control
    GPIO_USER,        // GPIO04 Optional sensor
    GPIO_USER,        // GPIO05 Optional sensor
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09
    0,                // GPIO10
                      // GPIO11 (SD_CMD   Flash)
    GPIO_USER,        // GPIO12 Optional sensor
    GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off) - Link and Power status
    GPIO_USER,        // GPIO14 Optional sensor
    0, 0, 0
  },
  {                   // SONOFF_B1 - Sonoff B1 (ESP8285 - my9231)
    GPIO_KEY1,        // GPIO00 Pad
    GPIO_USER,        // GPIO01 Serial RXD and Optional sensor pad
    GPIO_USER,        // GPIO02 Optional sensor SDA pad
    GPIO_USER,        // GPIO03 Serial TXD and Optional sensor pad
    0, 0,
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09
    0,                // GPIO10
                      // GPIO11 (SD_CMD   Flash)
    GPIO_DI,          // GPIO12 my9231 DI
    0,
    GPIO_DCKI,        // GPIO14 my9231 DCKI
    0, 0, 0
  },
  {                   // AILIGHT - Ai-Thinker RGBW led (ESP8266 - my9291)
    GPIO_KEY1,        // GPIO00 Pad
    GPIO_USER,        // GPIO01 Serial RXD and Optional sensor pad
    GPIO_USER,        // GPIO02 Optional sensor SDA pad
    GPIO_USER,        // GPIO03 Serial TXD and Optional sensor pad
    0, 0,
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    0,
    GPIO_DI,          // GPIO13 my9291 DI
    0,
    GPIO_DCKI,        // GPIO15 my9291 DCKI
    0, 0
  },
  {                   // SONOFF_T11 - Sonoff T1 1CH (ESP8285)
    GPIO_KEY1,        // GPIO00 Button 1
    GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
    GPIO_USER,        // GPIO02 Optional Sensor (J3 Pin 5)
    GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
    0, 0,
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09
    0,                // GPIO10
                      // GPIO11 (SD_CMD   Flash)
    GPIO_REL1,        // GPIO12 Blue Led and Relay 1 (0 = Off, 1 = On)
    GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off) - Link and Power status
    0, 0, 0, 0
  },
  {                   // SONOFF_T12 - Sonoff T1 2CH (ESP8285)
    GPIO_KEY1,        // GPIO00 Button 1
    GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
    GPIO_USER,        // GPIO02 Optional Sensor (J3 Pin 5)
    GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
    0,
    GPIO_REL2,        // GPIO05 Blue Led and Relay 2 (0 = Off, 1 = On)
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    GPIO_KEY2,        // GPIO09 Button 2
    0,                // GPIO10
                      // GPIO11 (SD_CMD   Flash)
    GPIO_REL1,        // GPIO12 Blue Led and Relay 1 (0 = Off, 1 = On)
    GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off) - Link and Power status
    0, 0, 0, 0
  },
  {                   // SONOFF_T13 - Sonoff T1 3CH (ESP8285)
    GPIO_KEY1,        // GPIO00 Button 1
    GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
    GPIO_USER,        // GPIO02 Optional Sensor (J3 Pin 5)
    GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
    GPIO_REL3,        // GPIO04 Blue Led and Relay 3 (0 = Off, 1 = On)
    GPIO_REL2,        // GPIO05 Blue Led and Relay 2 (0 = Off, 1 = On)
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    GPIO_KEY2,        // GPIO09 Button 2
    GPIO_KEY3,        // GPIO10 Button 3
                      // GPIO11 (SD_CMD   Flash)
    GPIO_REL1,        // GPIO12 Blue Led and Relay 1 (0 = Off, 1 = On)
    GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off) - Link and Power status
    0, 0, 0, 0
  },
  {                   // SUPLA1 - Supla Espablo (ESP8266)
                      // http://www.wykop.pl/ramka/3325399/diy-supla-do-puszki-instalacyjnej-podtynkowej-supla-org/
    0,                // GPIO00 Flash jumper
    GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
#ifdef USE_DS18x20
    GPIO_DSB,         // GPIO02 DS18B20 sensor
#else
    GPIO_USER,        // GPIO02 Optional sensor
#endif
    GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
    GPIO_KEY1,        // GPIO04 Button 1
    GPIO_REL1,        // GPIO05 Relay 1 (0 = Off, 1 = On)
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_USER,        // GPIO12 Optional sensor
    GPIO_REL2,        // GPIO13 Relay 2 (0 = Off, 1 = On)
    GPIO_USER,        // GPIO14 Optional sensor
    0,
    GPIO_LED1,        // GPIO16 Led (1 = On, 0 = Off) - Link and Power status
    ADC0_USER         // ADC0 A0 Analog input
  },
  {                   // WITTY - Witty Cloud Dev Board (ESP8266)
                      // https://www.aliexpress.com/item/ESP8266-serial-WIFI-Witty-cloud-Development-Board-ESP-12F-module-MINI-nodemcu/32643464555.html
    GPIO_USER,        // GPIO00 D3 flash push button on interface board
    GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
    GPIO_LED1_INV,    // GPIO02 D4 Blue Led (0 = On, 1 = Off) on ESP-12F - Link and Power status
    GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
    GPIO_KEY1,        // GPIO04 D2 push button on ESP-12F board
    GPIO_USER,        // GPIO05 D1 optional sensor
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_PWM2,        // GPIO12 D6 RGB LED Green
    GPIO_PWM3,        // GPIO13 D7 RGB LED Blue
    GPIO_USER,        // GPIO14 D5 optional sensor
    GPIO_PWM1,        // GPIO15 D8 RGB LED Red
    GPIO_USER,        // GPIO16 D0 optional sensor
    ADC0_USER         // ADC0 A0 Light sensor / Requires USE_ADC_VCC in user_config.h to be disabled
  },
  {                   // YUNSHAN - Yunshan Wifi Relay (ESP8266)
                      // https://www.ebay.com/p/Esp8266-220v-10a-Network-Relay-WiFi-Module/1369583381
                      // Schematics and Info https://ucexperiment.wordpress.com/2016/12/18/yunshan-esp8266-250v-15a-acdc-network-wifi-relay-module/
    0,                // GPIO00 Flash jumper - Module Pin 8
    GPIO_USER,        // GPIO01 Serial RXD and Optional sensor - Module Pin 2
    GPIO_LED1_INV,    // GPIO02 Blue Led (0 = On, 1 = Off) on ESP-12F - Module Pin 7 - Link and Power status
    GPIO_USER,        // GPIO03 Serial TXD and Optional sensor - Module Pin 3
    GPIO_REL1,        // GPIO04 Red Led and Relay (0 = Off, 1 = On) - Module Pin 10
    GPIO_KEY1,        // GPIO05 Blue Led and OptoCoupler input - Module Pin 9
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    0, 0, 0, 0, 0, 0
  },
  {                   // MAGICHOME - Magic Home (aka Flux-light) (ESP8266) and Arilux LC10 (ESP8285)
                      // https://www.aliexpress.com/item/Magic-Home-Mini-RGB-RGBW-Wifi-Controller-For-Led-Strip-Panel-light-Timing-Function-16million-colors/32686853650.html
    0,
    GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
    GPIO_LED1_INV,    // GPIO02 Blue onboard LED - Link and Power status
    GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
    GPIO_ARIRFRCV,    // GPIO04 IR or RF receiver (optional) (Arilux LC10)
    GPIO_PWM2,        // GPIO05 RGB LED Green
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_PWM3,        // GPIO12 RGB LED Blue
    GPIO_USER,        // GPIO13 RGBW LED White (optional - set to PWM4 for Cold White or Warm White as used on Arilux LC10)
    GPIO_PWM1,        // GPIO14 RGB LED Red
    GPIO_ARIRFSEL,    // GPIO15 RF receiver control (Arilux LC10)
    0, 0
  },
  {                   // LUANIHVIO - ESP8266_HVIO
                      // https://luani.de/projekte/esp8266-hvio/
    0,                // GPIO00 Flash jumper
    GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
    GPIO_USER,        // GPIO02 Optional sensor / I2C SDA pad
    GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
    GPIO_REL1,        // GPIO04 Relay 1 (0 = Off, 1 = On)
    GPIO_REL2,        // GPIO05 Relay 2 (0 = Off, 1 = On)
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_SWT1,        // GPIO12 External input 1 (0 = On, 1 = Off)
    GPIO_SWT2,        // GPIO13 External input 2 (0 = On, 1 = Off)
    GPIO_USER,        // GPIO14 Optional sensor / I2C SCL pad
    GPIO_LED1,        // GPIO15 Led (1 = On, 0 = Off) - Link and Power status
    0,
    ADC0_USER         // ADC0 A0 Analog input
  },
  {                   // KMC_70011 - KMC 70011
                      // https://www.amazon.com/KMC-Timing-Monitoring-Network-125V-240V/dp/B06XRX2GTQ
    GPIO_KEY1,        // GPIO00 Button
    0, 0, 0,
    GPIO_HLW_CF,      // GPIO04 HLW8012 CF power
    GPIO_NRG_CF1,     // GPIO05 HLW8012 CF1 voltage / current
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_NRG_SEL,     // GPIO12 HLW8012 SEL (1 = Voltage)
    GPIO_LED1_INV,    // GPIO13 Green Led - Link and Power status
    GPIO_REL1,        // GPIO14 Relay
    0, 0, 0
  },
  {                   // ARILUX_LC01 - Arilux AL-LC01 (ESP8285)
                      // https://www.banggood.com/nl/ARILUX-AL-LC01-Super-Mini-LED-WIFI-Smart-RGB-Controller-For-RGB-LED-Strip-Light-DC-9-12V-p-1058603.html
                      //  (PwmFrequency 1111Hz)
    GPIO_KEY1,        // GPIO00 Optional Button
    GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
    GPIO_ARIRFSEL,    // GPIO02 RF receiver control
    GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
    GPIO_ARIRFRCV,    // GPIO04 IR or RF receiver (optional)
    GPIO_PWM1,        // GPIO05 RGB LED Red
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_PWM2,        // GPIO12 RGB LED Green
    GPIO_PWM3,        // GPIO13 RGB LED Blue
    GPIO_USER,        // GPIO14 RGBW LED White (optional - set to PWM4 for Cold White or Warm White)
    0, 0, 0
  },
  {                   // ARILUX_LC11 - Arilux AL-LC11 (ESP8266)
                      // https://www.banggood.com/nl/ARILUX-AL-LC11-Super-Mini-LED-WIFI-APP-Controller-RF-Remote-Control-For-RGBWW-LED-Strip-DC9-28V-p-1085112.html
                      //  (PwmFrequency 540Hz)
    GPIO_KEY1,        // GPIO00 Optional Button
    GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
    GPIO_ARIRFSEL,    // GPIO02 RF receiver control
    GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
    GPIO_PWM2,        // GPIO04 RGB LED Green
    GPIO_PWM1,        // GPIO05 RGB LED Red
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_PWM5,        // GPIO12 RGBCW LED Warm
    GPIO_PWM4,        // GPIO13 RGBW LED Cold
    GPIO_PWM3,        // GPIO14 RGB LED Blue
    GPIO_ARIRFRCV,    // GPIO15 RF receiver input
    0, 0
  },
  {                   // SONOFF_DUAL_R2 - Sonoff Dual R2 (ESP8285)
    GPIO_USER,        // GPIO00 Button 0 on header (0 = On, 1 = Off)
    GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
    0,
    GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
    0,
    GPIO_REL2,        // GPIO05 Relay 2 (0 = Off, 1 = On)
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    GPIO_USER,        // GPIO09 Button 1 on header (0 = On, 1 = Off)
    GPIO_KEY1,        // GPIO10 Button on casing
                      // GPIO11 (SD_CMD   Flash)
    GPIO_REL1,        // GPIO12 Relay 1 (0 = Off, 1 = On)
    GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off) - Link and Power status
    0, 0, 0, 0
  },
  {                   // ARILUX_LC06 - Arilux AL-LC06 (ESP8285)
                      // https://www.banggood.com/ARILUX-AL-LC06-LED-WIFI-Smartphone-Controller-Romote-5-Channels-DC12-24V-For-RGBWW-Strip-light-p-1061476.html
    GPIO_KEY1,        // GPIO00 Optional Button
    GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
    GPIO_USER,        // GPIO02 Empty pad
    GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
    GPIO_USER,        // GPIO04 W2 - PWM5
    0,
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_PWM2,        // GPIO12 RGB LED Green
    GPIO_PWM3,        // GPIO13 RGB LED Blue
    GPIO_PWM1,        // GPIO14 RGB LED Red
    GPIO_USER,        // GPIO15 RGBW LED White
    0, 0
  },
  {                   // SONOFF_S31 - Sonoff S31 (ESP8266 - CSE7766)
    GPIO_KEY1,        // GPIO00 Button
    GPIO_CSE7766_TX,  // GPIO01 Serial RXD 4800 baud 8E1 CSE7766 energy sensor
    0,
    GPIO_CSE7766_RX,  // GPIO03 Serial TXD
    0, 0,
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
    GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off) - Link and Power status
    0, 0, 0, 0
  },
  {                   // ZENGGE_ZF_WF017 - Zenggee ZJ-WF017-A (ESP12S))
                      // https://www.ebay.com/p/Smartphone-Android-IOS-WiFi-Music-Controller-for-RGB-5050-3528-LED-Strip-Light/534446632?_trksid=p2047675.l2644
    GPIO_KEY1,        // GPIO00 Optional Button
    0,
    GPIO_USER,        // GPIO02 Empty pad
    0,
    GPIO_USER,        // GPIO04 W2 - PWM5
    0,
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_PWM2,        // GPIO12 RGB LED Green
    GPIO_PWM1,        // GPIO13 RGB LED Red
    GPIO_PWM3,        // GPIO14 RGB LED Blue
    0, 0, 0
  },
  {                   // SONOFF_POW_R2 - Sonoff Pow R2 (ESP8285 - CSE7766)
    GPIO_KEY1,        // GPIO00 Button
    GPIO_CSE7766_TX,  // GPIO01 Serial RXD 4800 baud 8E1 CSE7766 energy sensor
    0,
    GPIO_CSE7766_RX,  // GPIO03 Serial TXD
    0, 0,
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
    GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off) - Link and Power status
    0, 0, 0, 0
  },
  {                   // SONOFF_IFAN02 - Sonoff iFan02 (ESP8285)
    GPIO_KEY1,        // GPIO00 WIFI_KEY0 Virtual button 1 as feedback from RC
    GPIO_USER,        // GPIO01 ESP_TXD Serial RXD and Optional sensor
    0,                // GPIO02 ESP_LOG
    GPIO_USER,        // GPIO03 ESP_RXD Serial TXD and Optional sensor
    GPIO_REL3,        // GPIO04 WIFI_O2 Relay 3 (0 = Off, 1 = On) controlling the fan
    GPIO_REL2,        // GPIO05 WIFI_O1 Relay 2 (0 = Off, 1 = On) controlling the fan
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    GPIO_KEY2,        // GPIO09 WIFI_KEY1 Virtual button 2 as feedback from RC
    GPIO_KEY3,        // GPIO10 WIFI_KEY2 Virtual button 3 as feedback from RC
                      // GPIO11 (SD_CMD   Flash)
    GPIO_REL1,        // GPIO12 WIFI_O0 Relay 1 (0 = Off, 1 = On) controlling the light
    GPIO_LED1_INV,    // GPIO13 WIFI_CHK Blue Led on PCA (0 = On, 1 = Off) - Link and Power status
    GPIO_KEY4,        // GPIO14 WIFI_KEY3 Virtual button 4 as feedback from RC
    GPIO_REL4,        // GPIO15 WIFI_O3 Relay 4 (0 = Off, 1 = On) controlling the fan
    0, 0
  },
  {                   // BLITZWOLF_BWSHP - BlitzWolf BW-SHP2 and BW-SHP6 (ESP8285 - BL0937 or HJL-01 Energy Monitoring)
                      // https://www.banggood.com/BlitzWolf-BW-SHP2-Smart-WIFI-Socket-EU-Plug-220V-16A-Work-with-Amazon-Alexa-Google-Assistant-p-1292899.html
                      // https://www.amazon.de/Steckdose-Homecube-intelligente-Verbrauchsanzeige-funktioniert/dp/B076Q2LKHG/ref=sr_1_fkmr0_1
                      // https://www.amazon.de/Intelligente-Stromverbrauch-Fernsteurung-Schaltbare-Energieklasse/dp/B076WZQS4S/ref=sr_1_1
                      // https://www.aliexpress.com/store/product/BlitzWolf-BW-SHP6-EU-Plug-Metering-Version-WIFI-Smart-Socket-220V-240V-10A-Work-with-Amazon/1965360_32945504669.html
    GPIO_LED1_INV,    // GPIO00 Red Led (1 = On, 0 = Off) - Power status
    GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
    GPIO_LEDLNK_INV,  // GPIO02 Blue Led (1 = On, 0 = Off) - Link status
    GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
    0,
    GPIO_HJL_CF,      // GPIO05 BL0937 or HJL-01 CF power
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_NRG_SEL_INV, // GPIO12 BL0937 or HJL-01 Sel output (0 = Voltage)
    GPIO_KEY1,        // GPIO13 Button
    GPIO_NRG_CF1,     // GPIO14 BL0937 or HJL-01 CF1 current / voltage
    GPIO_REL1,        // GPIO15 Relay (0 = Off, 1 = On)
    0, 0
  },
  {                   // SHELLY1 - Shelly1 Open Source (ESP8266 - 2MB) - https://shelly.cloud/shelly1-open-source/
    GPIO_USER,        // GPIO00 - Can be changed to GPIO_USER, only if Shelly is powered with 12V DC
    GPIO_USER,        // GPIO01 Serial RXD - Can be changed to GPIO_USER, only if Shelly is powered with 12V DC
    0,
    GPIO_USER,        // GPIO03 Serial TXD - Can be changed to GPIO_USER, only if Shelly is powered with 12V DC
    GPIO_REL1,        // GPIO04 Relay (0 = Off, 1 = On)
    GPIO_SWT1_NP,     // GPIO05 SW pin
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    0, 0, 0, 0, 0, 0
  },
  {                   // SHELLY2 - Shelly2 (ESP8266 - 2MB) - https://shelly.cloud/shelly2/
    0,
    GPIO_MCP39F5_TX,  // GPIO01 MCP39F501 Serial input
    0,
    GPIO_MCP39F5_RX,  // GPIO03 MCP39F501 Serial output
    GPIO_REL1,        // GPIO04
    GPIO_REL2,        // GPIO05
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_SWT1,        // GPIO12
    0,
    GPIO_SWT2,        // GPIO14
    GPIO_MCP39F5_RST, // GPIO15 MCP39F501 Reset
    0,
    0
  },
  {                   // PHILIPS - Xiaomi Philips bulb (ESP8266)
    0, 0, 0, 0, 0, 0,
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_PWM2,        // GPIO12 cold/warm light
    0, 0,
    GPIO_PWM1,        // GPIO15 light intensity
    0, 0
  },
  {                   // NEO_COOLCAM - Neo Coolcam (ESP8266)
                      // https://www.banggood.com/NEO-COOLCAM-WiFi-Mini-Smart-Plug-APP-Remote-Control-Timing-Smart-Socket-EU-Plug-p-1288562.html?cur_warehouse=CN
    0, 0, 0, 0,
    GPIO_LED1_INV,    // GPIO04 Red Led (0 = On, 1 = Off) - Link and Power status
    0,
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
    GPIO_KEY1,        // GPIO13 Button
    0, 0, 0, 0
  },
  {                   // ESP_SWITCH - Michael Haustein 4 channel wall switch (ESP07 = ESP8266)
                      // Use rules for further actions like - rule on power1#state do publish cmnd/other_device/power %value% endon
    GPIO_KEY2,        // GPIO00 Button 2
    GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
    GPIO_REL3_INV,    // GPIO02 Yellow Led 3 (0 = On, 1 = Off)
    GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
    GPIO_KEY1,        // GPIO04 Button 1
    GPIO_REL2_INV,    // GPIO05 Red Led 2 (0 = On, 1 = Off)
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_REL4_INV,    // GPIO12 Blue Led 4 (0 = On, 1 = Off)
    GPIO_KEY4,        // GPIO13 Button 4
    GPIO_KEY3,        // GPIO14 Button 3
    GPIO_LED1,        // GPIO15 Optional sensor
    GPIO_REL1_INV,    // GPIO16 Green Led 1 (0 = On, 1 = Off)
    0
  },
  {                   // OBI - OBI socket (ESP8266) - https://www.obi.de/hausfunksteuerung/wifi-stecker-schuko/p/2291706
    GPIO_USER,        // GPIO00
    GPIO_USER,        // GPIO01 Serial RXD
    0,
    GPIO_USER,        // GPIO03 Serial TXD
    GPIO_LED1,        // GPIO04 Blue LED - Link and Power status
    GPIO_REL1,        // GPIO05 (Relay OFF, but used as Relay Switch)
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_LED3,        // GPIO12 (Relay ON, but set to LOW, so we can switch with GPIO05)
    GPIO_USER,        // GPIO13
    GPIO_KEY1,        // GPIO14 Button
    0,
    GPIO_USER,        // GPIO16
    ADC0_USER         // ADC0   A0 Analog input
  },
  {                   // TECKIN - https://www.amazon.de/gp/product/B07D5V139R
    0,
    GPIO_KEY1,        // GPIO01 Serial TXD and Button
    0,
    GPIO_LED1_INV,    // GPIO03 Serial RXD and Red Led (0 = On, 1 = Off) - Power status
    GPIO_HJL_CF,      // GPIO04 BL0937 or HJL-01 CF power
    GPIO_NRG_CF1,     // GPIO05 BL0937 or HJL-01 CF1 current / voltage
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_NRG_SEL_INV, // GPIO12 BL0937 or HJL-01 Sel output (0 = Voltage)
    GPIO_LEDLNK_INV,  // GPIO13 Blue Led (0 = On, 1 = Off) - Link status
    GPIO_REL1,        // GPIO14 Relay (0 = Off, 1 = On)
    0, 0, 0
  },
  {                   // APLIC_WDP303075 - Aplic WDP 303075 (ESP8285 - HLW8012 Energy Monitoring)
                      // https://www.amazon.de/dp/B07CNWVNJ2
    0, 0, 0,
    GPIO_KEY1,        // GPIO03 Button
    GPIO_HLW_CF,      // GPIO04 HLW8012 CF power
    GPIO_NRG_CF1,     // GPIO05 HLW8012 CF1 current / voltage
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_NRG_SEL_INV, // GPIO12 HLW8012 CF Sel output (0 = Voltage)
    GPIO_LED1_INV,    // GPIO13 LED (0 = On, 1 = Off) - Link and Power status
    GPIO_REL1,        // GPIO14 Relay SRU 5VDC SDA (0 = Off, 1 = On )
    0, 0, 0
  },
  {                   // TUYA_DIMMER - Tuya MCU device (ESP8266 w/ separate MCU)
                      // https://www.amazon.com/gp/product/B07CTNSZZ8/ref=oh_aui_detailpage_o00_s00?ie=UTF8&psc=1
    GPIO_USER,        // Virtual Button (controlled by MCU)
    GPIO_USER,        // GPIO01 MCU serial control
    GPIO_USER,
    GPIO_USER,        // GPIO03 MCU serial control
    GPIO_USER,
    GPIO_USER,
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_USER,
    GPIO_USER,
    GPIO_USER,        // GPIO14 Green Led
    GPIO_USER,
    GPIO_USER,
    0
  },
  {                   // GOSUND - https://www.amazon.de/gp/product/B0777BWS1P
    0,
    GPIO_LEDLNK_INV,  // GPIO01 Serial RXD and LED1 (blue) inv - Link status
    0,
    GPIO_KEY1,        // GPIO03 Serial TXD and Button
    GPIO_HJL_CF,      // GPIO04 BL0937 or HJL-01 CF power
    GPIO_NRG_CF1,     // GPIO05 BL0937 or HJL-01 CF1 current / voltage
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_NRG_SEL_INV, // GPIO12 BL0937 or HJL-01 Sel output (0 = Voltage)
    GPIO_LED1_INV,    // GPIO13 LED2 (red) inv - Power status
    GPIO_REL1,        // GPIO14 Relay (0 = Off, 1 = On)
    0, 0, 0
  },
  {                   // ARMTRONIX_DIMMERS - ARMTRONIX Dimmer, one or two channel (ESP8266 w/ separate MCU dimmer)
                      // https://www.tindie.com/products/Armtronix/wifi-ac-dimmer-two-triac-board/
                      // https://www.tindie.com/products/Armtronix/wifi-ac-dimmer-esp8266-one-triac-board-alexaecho/
    GPIO_USER,
    GPIO_TXD,         // GPIO01 MCU serial control
    GPIO_USER,
    GPIO_RXD,         // GPIO03 MCU serial control
    GPIO_USER,
    GPIO_USER,
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_USER,
    GPIO_USER,
    GPIO_USER,
    GPIO_USER,
    GPIO_USER,
    0
  },
  {                   // SK03_TUYA - Outdoor smart plug with power monitoring HLW8012 chip - https://www.amazon.com/gp/product/B07CG7MBPV
    GPIO_KEY1,        // GPIO00 Button
    0, 0, 0,
    GPIO_HLW_CF,      // GPIO04 HLW8012 CF power
    GPIO_NRG_CF1,     // GPIO05 HLW8012 CF1 current / voltage
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_NRG_SEL_INV, // GPIO12 HLW8012 CF Sel output (0 = Voltage)
    GPIO_LED1_INV,    // GPIO13 Red Led (0 = On, 1 = Off) - Power status
    GPIO_LEDLNK_INV,  // GPIO14 Blue Led (0 = On, 1 = Off) - Link status
    GPIO_REL1,        // GPIO15 Relay (0 = Off, 1 = On)
    0, 0
  },
  {                   // PS_16_DZ - PS-16-DZ Dimmer (ESP8266 w/ separate Nuvoton MCU dimmer)
                      // https://www.aliexpress.com/item/SM-Smart-WIFI-Wall-Dimmer-Light-Switch-US-Ewelink-APP-Remote-Control-Wi-Fi-Wirele-Work/32871151902.html
    GPIO_USER,
    GPIO_TXD,         // GPIO01 MCU serial control
    GPIO_USER,
    GPIO_RXD,         // GPIO03 MCU serial control
    GPIO_USER,
    GPIO_USER,
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_USER,
    GPIO_LED1,        // GPIO13 WiFi LED - Link and Power status
    GPIO_USER,
    GPIO_USER,
    GPIO_USER,
    0
  },
  {                   // TECKIN_US - Teckin SP20 US with Energy Monitoring
                      // https://www.amazon.com/Outlet-Compatible-Monitoring-Function-Required/dp/B079Q5W22B
                      // https://www.amazon.com/Outlet-ZOOZEE-Monitoring-Function-Compatible/dp/B07J2LR5KN
    GPIO_LED1_INV,    // GPIO00 Red Led (1 = On, 0 = Off) - Power status
    0,
    GPIO_LEDLNK_INV,  // GPIO02 Blue Led (1 = On, 0 = Off) - Link status
    0,
    GPIO_REL1,        // GPIO04 Relay (0 = Off, 1 = On)
    GPIO_HJL_CF,      // GPIO05 BL0937 or HJL-01 CF power
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_NRG_SEL_INV, // GPIO12 BL0937 or HJL-01 Sel output (0 = Voltage)
    GPIO_KEY1,        // GPIO13 Button
    GPIO_NRG_CF1,     // GPIO14 BL0937 or HJL-01 CF1 current / voltage
    0, 0, 0
  },
  {                   // MANZOKU_EU_4 - "MANZOKU" labeled power strip, EU version
                      // https://www.amazon.de/Steckdosenleiste-AOFO-Mehrfachsteckdose-Ãœberspannungsschutz-Sprachsteuerung/dp/B07GBSD11P/
                      // https://www.amazon.de/Steckdosenleiste-Geekbes-USB-Anschluss-Kompatibel-gesteuert/dp/B078W23BW9/
    0,                // GPIO00
    0,                // GPIO01 Serial RXD
    0,
    GPIO_KEY1,        // GPIO03 Serial TXD + Button
    GPIO_REL2,        // GPIO04 Relay 2
    GPIO_REL1,        // GPIO05 Relay 1
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_REL3,        // GPIO12 Relay 3
    GPIO_REL4,        // GPIO13 Relay 4
    GPIO_USER,        // GPIO14
    0,
    GPIO_USER,        // GPIO16
    0
  },
  {                   // OBI2 - OBI socket (ESP8266) - https://www.obi.de/hausfunksteuerung/wifi-stecker-schuko-2-stueck-weiss/p/4077673
    0,                // GPIO00
    0,                // GPIO01 Serial RXD
    0,
    0,                // GPIO03 Serial TXD
    GPIO_REL1,        // GPIO04 Relay 1
    GPIO_KEY1,        // GPIO05 Button
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_LEDLNK_INV,  // GPIO12 Green LED - Link status
    GPIO_LED1,        // GPIO13 Red LED - Power status
    0, 0, 0, 0
  },
  {                   // YTF_IR_BRIDGE - https://www.aliexpress.com/item/Tuya-universal-Smart-IR-Hub-remote-control-Voice-Control-AC-TV-Work-With-Alexa-Google-Home/32951202513.html
    GPIO_USER,        // GPIO00
    GPIO_USER,        // GPIO01 Serial RXD
    GPIO_USER,        // GPIO02
    GPIO_USER,        // GPIO03 Serial TXD
    GPIO_LED1_INV,    // GPIO04 Blue Led - Link status
    GPIO_IRRECV,      // GPIO05 IR Receiver
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    0,                // GPIO12
    GPIO_KEY1,        // GPIO13 Button
    GPIO_IRSEND,      // GPIO14 IR Transmitter
    0, 0, 0
  },
  {                   // DIGOO - Digoo DG-SP202
                      // https://www.banggood.com/DIGOO-DG-SP202-Dual-EU-Plug-Smart-WIFI-Socket-Individual-Controllable-Energy-Monitor-Remote-Control-Timing-Smart-Home-Outlet-let-p-1375323.html
    GPIO_KEY1,        // GPIO00 Button1
    0,                // GPIO01 Serial RXD
    0,                // GPIO02
    0,                // GPIO03 Serial TXD
    GPIO_HJL_CF,      // GPIO04 BL0937 or HJL-01 CF power
    GPIO_NRG_CF1,     // GPIO05 BL0937 or HJL-01 CF1 current / voltage
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_NRG_SEL_INV, // GPIO12 BL0937 or HJL-01 Sel output (0 = Voltage)
    GPIO_LED1,        // GPIO13 Blue Leds - Link Status
    GPIO_REL2,        // GPIO14 Relay2 (0 = Off, 1 = On) and Red Led
    GPIO_REL1,        // GPIO15 Relay1 (0 = Off, 1 = On) and Red Led
    GPIO_KEY2_NP,     // GPIO16 Button2, externally pulled up
    0
  },
  {                   // KA10 - SMANERGY KA10 (ESP8285 - BL0937 Energy Monitoring) - https://www.amazon.es/dp/B07MBTCH2Y
    0,                // GPIO00
    GPIO_LEDLNK_INV,  // GPIO01 Blue LED - Link status
    0,                // GPIO02
    GPIO_KEY1,        // GPIO03 Button
    GPIO_HJL_CF,      // GPIO04 BL0937 CF power
    GPIO_NRG_CF1,     // GPIO05 BL0937 CF1 voltage / current
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_NRG_SEL_INV, // GPIO12 BL0937 Sel output (1 = Voltage)
    GPIO_LED1,        // GPIO13 Red LED - Power status
    GPIO_REL1,        // GPIO14 Relay 1
    0, 0, 0
  },
  {                   // ZX2820
    GPIO_KEY1,        // GPIO00 Button
    0, 0, 0,
    GPIO_HLW_CF,      // GPIO04 HLW8012 CF power
    GPIO_NRG_CF1,     // GPIO05 HLW8012 CF1 voltage / current
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_NRG_SEL_INV, // GPIO12 HLW8012 SEL (0 = Voltage)
    GPIO_LED1_INV,    // GPIO13 Green Led - Link and Power status
    GPIO_REL1,        // GPIO14 Relay
    0, 0, 0
  },
  {                   // MI_DESK_LAMP - Mi LED Desk Lamp - https://www.mi.com/global/smartlamp/
    0, 0,
    GPIO_KEY1,        // GPIO02 Button
    0,
    GPIO_PWM1,        // GPIO04 Cold White
    GPIO_PWM2,        // GPIO05 Warm White
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_ROT1A,       // GPIO12 Rotary switch A pin
    GPIO_ROT1B,       // GPIO13 Rotary switch B pin
    0, 0, 0, 0
  },
  {                   // SP10 - Tuya SP10 (BL0937 Energy Monitoring)
                      // https://www.aliexpress.com/item/Smart-Mini-WiFi-Plug-Outlet-Switch-Work-With-ForEcho-Alexa-Google-Home-Remote-EU-Smart-Socket/32963670423.html
    0,                // GPIO00
    GPIO_PWM1,        // GPIO01 Nightlight
    0,                // GPIO02
    GPIO_KEY1,        // GPIO03 Button
    GPIO_HJL_CF,      // GPIO04 BL0937 CF power
    GPIO_NRG_CF1,     // GPIO05 BL0937 CF1 voltage / current
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_NRG_SEL_INV, // GPIO12 BL0937 Sel output (1 = Voltage)
    GPIO_LED1,        // GPIO13 Blue LED - Link status
    GPIO_REL1,        // GPIO14 Relay and red LED
    0, 0, 0
  },
  {                   // WAGA - WAGA life CHCZ02MB (HJL-01 Energy Monitoring)
                      // https://www.ebay.com/itm/332595697006
    GPIO_LED1_INV,    // GPIO00 Red LED
    0,                // GPIO01 Serial RXD
    0,                // GPIO02
    GPIO_NRG_SEL_INV, // GPIO03 HJL-01 Sel output (1 = Voltage)
    0,                // GPIO04
    GPIO_HJL_CF,      // GPIO05 HJL-01 CF power
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_REL1,        // GPIO12 Relay
    GPIO_KEY1,        // GPIO13 Button
    GPIO_NRG_CF1,     // GPIO14 HJL-01 CF1 voltage / current
    GPIO_LEDLNK_INV,  // GPIO15 Blue LED - Link status
    0, 0
  },
  {                   // SYF05 - Sunyesmart SYF05 (a.k.a. Fcmila) = TYWE3S + SM16726
                      // Also works with Merkury 904 RGBW Bulbs with 13 set to GPIO_SM16716_SEL
                      // https://www.flipkart.com/fc-mila-bxav-xs-ad-smart-bulb/p/itmf85zgs45fzr7n
                      // https://docs.tuya.com/en/hardware/WiFi-module/wifi-e3s-module.html
                      // http://www.datasheet-pdf.com/PDF/SM16716-Datasheet-Sunmoon-932771
    GPIO_USER,        // GPIO00 N.C.
    0,                // GPIO01 Serial RXD
    GPIO_USER,        // GPIO02 N.C.
    0,                // GPIO03 Serial TXD
    GPIO_SM16716_CLK, // GPIO04 SM16716 Clock
    GPIO_PWM1,        // GPIO05 White
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_USER,        // GPIO12 Alt. White on some devices
    GPIO_USER,        // GPIO13 SM16716 Enable on some devices
    GPIO_SM16716_DAT, // GPIO14 SM16716 Data
    0,                // GPIO15 wired to GND
    GPIO_USER,        // GPIO16 N.C.
    ADC0_USER         // ADC0 A0 Analog input
  },
  {                   // SONOFF_L1 - Sonoff L1 RGB LED controller (ESP8266 w/ separate Nuvoton MCU)
    0,
    GPIO_TXD,         // GPIO01 MCU serial control
    0,
    GPIO_RXD,         // GPIO03 MCU serial control
    0, 0,
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    0,
    GPIO_LED1_INV,    // GPIO13 WiFi Blue Led - Link and Power status
    0, 0, 0, 0
  },
  {                   // SONOFF_IFAN03 - Sonoff iFan03 (ESP8285)
    GPIO_KEY1,        // GPIO00 WIFI_KEY0 Button 1
    GPIO_TXD,         // GPIO01 ESP_TXD Serial RXD connection to P0.5 of RF microcontroller
    0,                // GPIO02 ESP_LOG
    GPIO_RXD,         // GPIO03 ESP_RXD Serial TXD connection to P0.4 of RF microcontroller
    0,                // GPIO04 DEBUG_RX
    0,                // GPIO05 DEBUG_TX
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    GPIO_REL1_INV,    // GPIO09 WIFI_O0 Relay 1 (0 = Off, 1 = On) controlling the light
    GPIO_BUZZER_INV,  // GPIO10 WIFI_O4 Buzzer (0 = Off, 1 = On)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_REL3,        // GPIO12 WIFI_O2 Relay 3 (0 = Off, 1 = On) controlling the fan
    GPIO_LED1_INV,    // GPIO13 WIFI_CHK Blue Led on PCA (0 = On, 1 = Off) - Link and Power status
    GPIO_REL2,        // GPIO14 WIFI_O1 Relay 2 (0 = Off, 1 = On) controlling the fan
    GPIO_REL4,        // GPIO15 WIFI_O3 Relay 4 (0 = Off, 1 = On) controlling the fan
    0, 0
  },
  {                   // EXS_DIMMER - EX-Store WiFi Dimmer v4, two channel (ESP8266 w/ separate MCU dimmer)
                      // https://ex-store.de/2-Kanal-RS232-WiFi-WLan-Dimmer-Modul-V4-fuer-Unterputzmontage-230V-3A
                      // https://ex-store.de/2-Kanal-RS232-WiFi-WLan-Dimmer-Modul-V4-fuer-Unterputzmontage-230V-3A-ESP8266-V12-Stift-und-Buchsenleisten
    0,
    GPIO_TXD,         // GPIO01 MCU serial control
    GPIO_LEDLNK,      // GPIO02 LED Link
    GPIO_RXD,         // GPIO03 MCU serial control
    GPIO_USER,        // GPIO04
    GPIO_USER,        // GPIO05
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_USER,        // GPIO12
    GPIO_EXS_ENABLE,  // GPIO13 EXS MCU Enable
    GPIO_USER,        // GPIO14
    0,                // GPIO15
    0, 0
  },
  {                   // PWM_DIMMER - Support for Martin Jerry/acenx/Tessan/NTONPOWER SD0x PWM
                      // dimmer switches. The brightness of the load for these dimmers is
                      // controlled by a PWM GPIO pin. There are typically power, up & down
                      // buttons and 4 LED's. Examples are:
                      // https://www.amazon.com/dp/B07FXYSVR1
                      // https://www.amazon.com/dp/B07V26Q3VD
                      // https://www.amazon.com/dp/B07K67D43J
                      // https://www.amazon.com/dp/B07TTGFWFM
    GPIO_KEY3,        // GPIO00 Up button
    GPIO_KEY2,        // GPIO01 Down button
    0,                // GPIO02
    GPIO_LED4_INV,    // GPIO03 Level 5 LED
    GPIO_LEDLNK_INV,  // GPIO04 LED Link
    GPIO_LED3_INV,    // GPIO05 Level 4 LED
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_LED2_INV,    // GPIO12 Level 3 LED
    GPIO_PWM1,        // GPIO13 Dimmer PWM
    GPIO_LED1_INV,    // GPIO12 Level 2 LED
    GPIO_KEY1_INV,    // GPIO15 Power button
    GPIO_REL1_INV,    // GPIO16 Power relay/Level 1 LED
    0
  },
  {                   // SONOFF_D1 - Sonoff D1 RF Dimmer 433 (ESP8285)
    GPIO_USER,        // GPIO00 Pad
    GPIO_TXD,         // GPIO01 D1 control
    0,                // GPIO02
    GPIO_RXD,         // GPIO03 D1 control
    0,                // GPIO04 DEBUG_RX
    0,                // GPIO05 DEBUG_TX
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09
    0,                // GPIO10
                      // GPIO11 (SD_CMD   Flash)
    0,                // GPIO12
    GPIO_LED1_INV,    // GPIO13 WiFi Blue Led - Link and Power status
    0, 0, 0, 0
  }
};

#endif  // ESP8266

#ifdef ESP32
#include "tasmota_template_ESP32.h"
#endif  // ESP32

#endif  // _TASMOTA_TEMPLATE_H_
