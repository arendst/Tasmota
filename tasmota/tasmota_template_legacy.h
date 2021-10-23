/*
  tasmota_template_legacy.h - template settings for Tasmota

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

#ifndef _TASMOTA_TEMPLATE_LEGACY_H_
#define _TASMOTA_TEMPLATE_LEGACY_H_

#ifdef ESP8266
//********************************************************************************************
// User selectable GPIO functionality
// ATTENTION: No additions are supported
enum LegacyUserSelectablePins {
  GPI8_NONE,           // Not used
  GPI8_DHT11,          // DHT11
  GPI8_DHT22,          // DHT21, DHT22, AM2301, AM2302, AM2321
  GPI8_SI7021,         // iTead SI7021
  GPI8_DSB,            // Single wire DS18B20 or DS18S20
  GPI8_I2C_SCL,        // I2C SCL
  GPI8_I2C_SDA,        // I2C SDA
  GPI8_WS2812,         // WS2812 Led string
  GPI8_IRSEND,         // IR remote
  GPI8_SWT1,           // User connected external switches
  GPI8_SWT2,
  GPI8_SWT3,
  GPI8_SWT4,
  GPI8_SWT5,
  GPI8_SWT6,
  GPI8_SWT7,
  GPI8_SWT8,
  GPI8_KEY1,           // Button usually connected to GPIO0
  GPI8_KEY2,
  GPI8_KEY3,
  GPI8_KEY4,
  GPI8_REL1,           // Relays
  GPI8_REL2,
  GPI8_REL3,
  GPI8_REL4,
  GPI8_REL5,
  GPI8_REL6,
  GPI8_REL7,
  GPI8_REL8,
  GPI8_REL1_INV,
  GPI8_REL2_INV,
  GPI8_REL3_INV,
  GPI8_REL4_INV,
  GPI8_REL5_INV,
  GPI8_REL6_INV,
  GPI8_REL7_INV,
  GPI8_REL8_INV,
  GPI8_PWM1,           // RGB   Red   or C  Cold White
  GPI8_PWM2,           // RGB   Green or CW Warm White
  GPI8_PWM3,           // RGB   Blue
  GPI8_PWM4,           // RGBW  (Cold) White
  GPI8_PWM5,           // RGBCW Warm White
  GPI8_CNTR1,
  GPI8_CNTR2,
  GPI8_CNTR3,
  GPI8_CNTR4,
  GPI8_PWM1_INV,       // RGB   Red   or C  Cold White
  GPI8_PWM2_INV,       // RGB   Green or CW Warm White
  GPI8_PWM3_INV,       // RGB   Blue
  GPI8_PWM4_INV,       // RGBW  (Cold) White
  GPI8_PWM5_INV,       // RGBCW Warm White
  GPI8_IRRECV,         // IR receiver
  GPI8_LED1,           // Leds
  GPI8_LED2,
  GPI8_LED3,
  GPI8_LED4,
  GPI8_LED1_INV,
  GPI8_LED2_INV,
  GPI8_LED3_INV,
  GPI8_LED4_INV,
  GPI8_MHZ_TXD,        // MH-Z19 Serial interface
  GPI8_MHZ_RXD,        // MH-Z19 Serial interface
  GPI8_PZEM0XX_TX,     // PZEM0XX Serial interface
  GPI8_PZEM004_RX,     // PZEM004T Serial interface
  GPI8_SAIR_TX,        // SenseAir Serial interface
  GPI8_SAIR_RX,        // SenseAir Serial interface
  GPI8_SPI_CS,         // SPI Chip Select
  GPI8_SPI_DC,         // SPI Data Direction
  GPI8_BACKLIGHT,      // Display backlight control
  GPI8_PMS5003_RX,     // Plantower PMS5003 Serial interface
  GPI8_SDS0X1_RX,      // Nova Fitness SDS011 Serial interface
  GPI8_SBR_TX,         // Serial Bridge Serial interface
  GPI8_SBR_RX,         // Serial Bridge Serial interface
  GPI8_SR04_TRIG,      // SR04 Trigger/TX pin
  GPI8_SR04_ECHO,      // SR04 Echo/RX pin
  GPI8_SDM120_TX,      // SDM120 Serial interface
  GPI8_SDM120_RX,      // SDM120 Serial interface
  GPI8_SDM630_TX,      // SDM630 Serial interface
  GPI8_SDM630_RX,      // SDM630 Serial interface
  GPI8_TM16CLK,        // TM1638 Clock
  GPI8_TM16DIO,        // TM1638 Data I/O
  GPI8_TM16STB,        // TM1638 Strobe
  GPI8_SWT1_NP,        // User connected external switches
  GPI8_SWT2_NP,
  GPI8_SWT3_NP,
  GPI8_SWT4_NP,
  GPI8_SWT5_NP,
  GPI8_SWT6_NP,
  GPI8_SWT7_NP,
  GPI8_SWT8_NP,
  GPI8_KEY1_NP,        // Button usually connected to GPIO0
  GPI8_KEY2_NP,
  GPI8_KEY3_NP,
  GPI8_KEY4_NP,
  GPI8_CNTR1_NP,
  GPI8_CNTR2_NP,
  GPI8_CNTR3_NP,
  GPI8_CNTR4_NP,
  GPI8_PZEM016_RX,     // PZEM-014,016 Serial Modbus interface
  GPI8_PZEM017_RX,     // PZEM-003,017 Serial Modbus interface
  GPI8_MP3_DFR562,     // RB-DFR-562, DFPlayer Mini MP3 Player
  GPI8_SDS0X1_TX,      // Nova Fitness SDS011 Serial interface
  GPI8_HX711_SCK,      // HX711 Load Cell clock
  GPI8_HX711_DAT,      // HX711 Load Cell data
  GPI8_TX2X_TXD_BLACK, // TX20/TX23 Transmission Pin
  GPI8_RFSEND,         // RF transmitter
  GPI8_RFRECV,         // RF receiver
  GPI8_TUYA_TX,        // Tuya Serial interface
  GPI8_TUYA_RX,        // Tuya Serial interface
  GPI8_MGC3130_XFER,   // MGC3130 Transfer
  GPI8_MGC3130_RESET,  // MGC3130 Reset
  GPI8_SSPI_MISO,      // Software SPI Master Input Client Output
  GPI8_SSPI_MOSI,      // Software SPI Master Output Client Input
  GPI8_SSPI_SCLK,      // Software SPI Serial Clock
  GPI8_SSPI_CS,        // Software SPI Chip Select
  GPI8_SSPI_DC,        // Software SPI Data or Command
  GPI8_RF_SENSOR,      // Rf receiver with sensor decoding
  GPI8_AZ_TXD,         // AZ-Instrument 7798 Serial interface
  GPI8_AZ_RXD,         // AZ-Instrument 7798 Serial interface
  GPI8_MAX31855CS,     // MAX31855 Serial interface
  GPI8_MAX31855CLK,    // MAX31855 Serial interface
  GPI8_MAX31855DO,     // MAX31855 Serial interface
  GPI8_KEY1_INV,       // Inverted buttons
  GPI8_KEY2_INV,
  GPI8_KEY3_INV,
  GPI8_KEY4_INV,
  GPI8_KEY1_INV_NP,    // Inverted buttons without pull-up
  GPI8_KEY2_INV_NP,
  GPI8_KEY3_INV_NP,
  GPI8_KEY4_INV_NP,
  GPI8_NRG_SEL,        // HLW8012/HLJ-01 Sel output (1 = Voltage)
  GPI8_NRG_SEL_INV,    // HLW8012/HLJ-01 Sel output (0 = Voltage)
  GPI8_NRG_CF1,        // HLW8012/HLJ-01 CF1 voltage / current
  GPI8_HLW_CF,         // HLW8012 CF power
  GPI8_HJL_CF,         // HJL-01/BL0937 CF power
  GPI8_MCP39F5_TX,     // MCP39F501 Serial interface (Shelly2)
  GPI8_MCP39F5_RX,     // MCP39F501 Serial interface (Shelly2)
  GPI8_MCP39F5_RST,    // MCP39F501 Reset (Shelly2)
  GPI8_PN532_TXD,      // PN532 NFC Serial Tx
  GPI8_PN532_RXD,      // PN532 NFC Serial Rx
  GPI8_SM16716_CLK,    // SM16716 CLOCK
  GPI8_SM16716_DAT,    // SM16716 DATA
  GPI8_SM16716_SEL,    // SM16716 SELECT
  GPI8_DI,             // my92x1 PWM input
  GPI8_DCKI,           // my92x1 CLK input
  GPI8_CSE7766_TX,     // CSE7766 Serial interface (S31 and Pow R2) - Not used anymore 20200121
  GPI8_CSE7766_RX,     // CSE7766 Serial interface (S31 and Pow R2)
  GPI8_ARIRFRCV,       // AriLux RF Receive input
  GPI8_TXD,            // Serial interface
  GPI8_RXD,            // Serial interface
  GPI8_ROT1A,          // Rotary switch1 A Pin
  GPI8_ROT1B,          // Rotary switch1 B Pin
  GPI8_ROT2A,          // Rotary switch2 A Pin
  GPI8_ROT2B,          // Rotary switch2 B Pin
  GPI8_HRE_CLOCK,      // Clock/Power line for HR-E Water Meter
  GPI8_HRE_DATA,       // Data line for HR-E Water Meter
  GPI8_ADE7953_IRQ,    // ADE7953 IRQ
  GPI8_LEDLNK,         // Link led
  GPI8_LEDLNK_INV,     // Inverted link led
  GPI8_ARIRFSEL,       // Arilux RF Receive input selected
  GPI8_BUZZER,         // Buzzer
  GPI8_BUZZER_INV,     // Inverted buzzer
  GPI8_OLED_RESET,     // OLED Display Reset
  GPI8_SOLAXX1_TX,     // Solax Inverter tx pin
  GPI8_SOLAXX1_RX,     // Solax Inverter rx pin
  GPI8_ZIGBEE_TX,      // Zigbee Serial interface
  GPI8_ZIGBEE_RX,      // Zigbee Serial interface
  GPI8_RDM6300_RX,     // RDM6300 RX
  GPI8_IBEACON_TX,     // HM17 IBEACON TX
  GPI8_IBEACON_RX,     // HM17 IBEACON RX
  GPI8_A4988_DIR,      // A4988 direction pin
  GPI8_A4988_STP,      // A4988 step pin
  GPI8_A4988_ENA,      // A4988 enabled pin
  GPI8_A4988_MS1,      // A4988 microstep pin1
  GPI8_A4988_MS2,      // A4988 microstep pin2
  GPI8_A4988_MS3,      // A4988 microstep pin3
  GPI8_DDS2382_TX,     // DDS2382 Serial interface
  GPI8_DDS2382_RX,     // DDS2382 Serial interface
  GPI8_DDSU666_TX,     // DDSU666 Serial interface
  GPI8_DDSU666_RX,     // DDSU666 Serial interface
  GPI8_SM2135_CLK,     // SM2135 Clk
  GPI8_SM2135_DAT,     // SM2135 Dat
  GPI8_DEEPSLEEP,      // Kill switch for deepsleep
  GPI8_EXS_ENABLE,     // EXS MCU Enable
  GPI8_TASMOTACLIENT_TXD,     // Client TX
  GPI8_TASMOTACLIENT_RXD,     // Client RX
  GPI8_TASMOTACLIENT_RST,     // Client Reset Pin
  GPI8_TASMOTACLIENT_RST_INV, // Client Reset Inverted
  GPI8_HPMA_RX,        // Honeywell HPMA115S0 Serial interface
  GPI8_HPMA_TX,        // Honeywell HPMA115S0 Serial interface
  GPI8_GPS_RX,         // GPS serial interface
  GPI8_GPS_TX,         // GPS serial interface
  GPI8_DSB_OUT,        // Pseudo Single wire DS18B20 or DS18S20
  GPI8_DHT11_OUT,      // Pseudo Single wire DHT11, DHT21, DHT22, AM2301, AM2302, AM2321
  GPI8_HM10_RX,        // HM10-BLE-Mijia-bridge serial interface
  GPI8_HM10_TX,        // HM10-BLE-Mijia-bridge serial interface
  GPI8_LE01MR_RX,      // F&F LE-01MR energy meter
  GPI8_LE01MR_TX,      // F&F LE-01MR energy meter
  GPI8_CC1101_GDO0,    // CC1101 pin for RX
  GPI8_CC1101_GDO2,    // CC1101 pin for RX
  GPI8_HRXL_RX,       // Data from MaxBotix HRXL sonar range sensor
  GPI8_ELECTRIQ_MOODL_TX, // ElectriQ iQ-wifiMOODL Serial TX
  GPI8_AS3935,         // Franklin Lightning Sensor
  GPI8_PMS5003_TX,     // Plantower PMS5003 Serial interface
  GPI8_BOILER_OT_RX,   // OpenTherm Boiler RX pin
  GPI8_BOILER_OT_TX,   // OpenTherm Boiler TX pin
  GPI8_WINDMETER_SPEED,  // WindMeter speed counter pin
  GPI8_BL0940_RX,      // BL0940 serial interface
  GPI8_TCP_TX,         // TCP Serial bridge
  GPI8_TCP_RX,         // TCP Serial bridge
  GPI8_TELEINFO_RX,    // TELEINFO serial interface
  GPI8_TELEINFO_ENABLE,// TELEINFO Enable PIN
  GPI8_LMT01,          // LMT01 input counting pin
  GPI8_IEM3000_TX,     // IEM3000 Serial interface
  GPI8_IEM3000_RX,     // IEM3000 Serial interface
  GPI8_ZIGBEE_RST,     // Zigbee reset
  GPI8_DYP_RX,
  GPI8_SENSOR_END };

// Programmer selectable GPIO functionality
enum LegacyProgramSelectablePins {
  GPI8_FIX_START = 251,
  GPI8_SPI_MISO,       // SPI MISO library fixed pin GPIO12
  GPI8_SPI_MOSI,       // SPI MOSI library fixed pin GPIO13
  GPI8_SPI_CLK,        // SPI Clk library fixed pin GPIO14
  GPI8_USER,           // User configurable needs to be 255
  GPI8_MAX };

// Indexed by LegacyUserSelectablePins to convert legacy (8-bit) GPIOs
const uint16_t kGpioConvert[] PROGMEM = {
  GPIO_NONE,
  AGPIO(GPIO_DHT11),          // DHT11
  AGPIO(GPIO_DHT22),          // DHT21, DHT22, AM2301, AM2302, AM2321
  AGPIO(GPIO_SI7021),         // iTead SI7021
  AGPIO(GPIO_DSB),            // Single wire DS18B20 or DS18S20
  AGPIO(GPIO_I2C_SCL),        // I2C SCL
  AGPIO(GPIO_I2C_SDA),        // I2C SDA
  AGPIO(GPIO_WS2812),         // WS2812 Led string
  AGPIO(GPIO_IRSEND),         // IR remote
  AGPIO(GPIO_SWT1),           // Switch
  AGPIO(GPIO_SWT1) +1,
  AGPIO(GPIO_SWT1) +2,
  AGPIO(GPIO_SWT1) +3,
  AGPIO(GPIO_SWT1) +4,
  AGPIO(GPIO_SWT1) +5,
  AGPIO(GPIO_SWT1) +6,
  AGPIO(GPIO_SWT1) +7,
  AGPIO(GPIO_KEY1),           // Button
  AGPIO(GPIO_KEY1) +1,
  AGPIO(GPIO_KEY1) +2,
  AGPIO(GPIO_KEY1) +3,
  AGPIO(GPIO_REL1),           // Relay
  AGPIO(GPIO_REL1) +1,
  AGPIO(GPIO_REL1) +2,
  AGPIO(GPIO_REL1) +3,
  AGPIO(GPIO_REL1) +4,
  AGPIO(GPIO_REL1) +5,
  AGPIO(GPIO_REL1) +6,
  AGPIO(GPIO_REL1) +7,
  AGPIO(GPIO_REL1_INV),       // Relay inverted
  AGPIO(GPIO_REL1_INV) +1,
  AGPIO(GPIO_REL1_INV) +2,
  AGPIO(GPIO_REL1_INV) +3,
  AGPIO(GPIO_REL1_INV) +4,
  AGPIO(GPIO_REL1_INV) +5,
  AGPIO(GPIO_REL1_INV) +6,
  AGPIO(GPIO_REL1_INV) +7,
  AGPIO(GPIO_PWM1),           // PWM
  AGPIO(GPIO_PWM1) +1,
  AGPIO(GPIO_PWM1) +2,
  AGPIO(GPIO_PWM1) +3,
  AGPIO(GPIO_PWM1) +4,
  AGPIO(GPIO_CNTR1),          // Counter
  AGPIO(GPIO_CNTR1) +1,
  AGPIO(GPIO_CNTR1) +2,
  AGPIO(GPIO_CNTR1) +3,
  AGPIO(GPIO_PWM1_INV),       // PWM inverted
  AGPIO(GPIO_PWM1_INV) +1,
  AGPIO(GPIO_PWM1_INV) +2,
  AGPIO(GPIO_PWM1_INV) +3,
  AGPIO(GPIO_PWM1_INV) +4,
  AGPIO(GPIO_IRRECV),         // IR receive
  AGPIO(GPIO_LED1),           // Led
  AGPIO(GPIO_LED1) +1,
  AGPIO(GPIO_LED1) +2,
  AGPIO(GPIO_LED1) +3,
  AGPIO(GPIO_LED1_INV),       // Led inverted
  AGPIO(GPIO_LED1_INV) +1,
  AGPIO(GPIO_LED1_INV) +2,
  AGPIO(GPIO_LED1_INV) +3,
  AGPIO(GPIO_MHZ_TXD),        // MH-Z19 Serial interface
  AGPIO(GPIO_MHZ_RXD),
  AGPIO(GPIO_PZEM0XX_TX),     // PZEM0XX Serial interface
  AGPIO(GPIO_PZEM004_RX),     // PZEM004T Serial interface
  AGPIO(GPIO_SAIR_TX),        // SenseAir Serial interface
  AGPIO(GPIO_SAIR_RX),        // SenseAir Serial interface
  AGPIO(GPIO_SPI_CS),         // SPI Chip Select
  AGPIO(GPIO_SPI_DC),         // SPI Data Direction
  AGPIO(GPIO_BACKLIGHT),      // Display backlight control
  AGPIO(GPIO_PMS5003_RX),     // Plantower PMS5003 Serial interface
  AGPIO(GPIO_SDS0X1_RX),      // Nova Fitness SDS011 Serial interface
  AGPIO(GPIO_SBR_TX),         // Serial Bridge Serial interface
  AGPIO(GPIO_SBR_RX),         // Serial Bridge Serial interface
  AGPIO(GPIO_SR04_TRIG),      // SR04 Tri/TXgger pin
  AGPIO(GPIO_SR04_ECHO),      // SR04 Ech/RXo pin
  AGPIO(GPIO_SDM120_TX),      // SDM120 Serial interface
  AGPIO(GPIO_SDM120_RX),      // SDM120 Serial interface
  AGPIO(GPIO_SDM630_TX),      // SDM630 Serial interface
  AGPIO(GPIO_SDM630_RX),      // SDM630 Serial interface
  AGPIO(GPIO_TM1638CLK),      // TM1638 Clock
  AGPIO(GPIO_TM1638DIO),      // TM1638 Data I/O
  AGPIO(GPIO_TM1638STB),      // TM1638 Strobe
  AGPIO(GPIO_SWT1_NP),        // Switch no pullup
  AGPIO(GPIO_SWT1_NP) +1,
  AGPIO(GPIO_SWT1_NP) +2,
  AGPIO(GPIO_SWT1_NP) +3,
  AGPIO(GPIO_SWT1_NP) +4,
  AGPIO(GPIO_SWT1_NP) +5,
  AGPIO(GPIO_SWT1_NP) +6,
  AGPIO(GPIO_SWT1_NP) +7,
  AGPIO(GPIO_KEY1_NP),        // Button no pullup
  AGPIO(GPIO_KEY1_NP) +1,
  AGPIO(GPIO_KEY1_NP) +2,
  AGPIO(GPIO_KEY1_NP) +3,
  AGPIO(GPIO_CNTR1_NP),       // Counter no pullup
  AGPIO(GPIO_CNTR1_NP) +1,
  AGPIO(GPIO_CNTR1_NP) +2,
  AGPIO(GPIO_CNTR1_NP) +3,
  AGPIO(GPIO_PZEM016_RX),     // PZEM-014,016 Serial Modbus interface
  AGPIO(GPIO_PZEM017_RX),     // PZEM-003,017 Serial Modbus interface
  AGPIO(GPIO_MP3_DFR562),     // RB-DFR-562, DFPlayer Mini MP3 Player Serial interface
  AGPIO(GPIO_SDS0X1_TX),      // Nova Fitness SDS011 Serial interface
  AGPIO(GPIO_HX711_SCK),      // HX711 Load Cell clock
  AGPIO(GPIO_HX711_DAT),      // HX711 Load Cell data
  AGPIO(GPIO_TX2X_TXD_BLACK), // TX20/TX23 Transmission Pin
  AGPIO(GPIO_RFSEND),         // RF transmitter
  AGPIO(GPIO_RFRECV),         // RF receiver
  AGPIO(GPIO_TUYA_TX),        // Tuya Serial interface
  AGPIO(GPIO_TUYA_RX),        // Tuya Serial interface
  AGPIO(GPIO_MGC3130_XFER),
  AGPIO(GPIO_MGC3130_RESET),
  AGPIO(GPIO_SSPI_MISO),      // Software SPI Master Input Client Output
  AGPIO(GPIO_SSPI_MOSI),      // Software SPI Master Output Client Input
  AGPIO(GPIO_SSPI_SCLK),      // Software SPI Serial Clock
  AGPIO(GPIO_SSPI_CS),        // Software SPI Chip Select
  AGPIO(GPIO_SSPI_DC),        // Software SPI Data or Command
  AGPIO(GPIO_RF_SENSOR),      // Rf receiver with sensor decoding
  AGPIO(GPIO_AZ_TXD),         // AZ-Instrument 7798 CO2 datalogger Serial interface
  AGPIO(GPIO_AZ_RXD),         // AZ-Instrument 7798 CO2 datalogger Serial interface
  AGPIO(GPIO_MAX31855CS),     // MAX31855 Serial interface
  AGPIO(GPIO_MAX31855CLK),    // MAX31855 Serial interface
  AGPIO(GPIO_MAX31855DO),     // MAX31855 Serial interface
  AGPIO(GPIO_KEY1_INV),       // Button inverted
  AGPIO(GPIO_KEY1_INV) +1,
  AGPIO(GPIO_KEY1_INV) +2,
  AGPIO(GPIO_KEY1_INV) +3,
  AGPIO(GPIO_KEY1_INV_NP),    // Button inverted no pullup
  AGPIO(GPIO_KEY1_INV_NP) +1,
  AGPIO(GPIO_KEY1_INV_NP) +2,
  AGPIO(GPIO_KEY1_INV_NP) +3,
  AGPIO(GPIO_NRG_SEL),        // HLW8012/HLJ-01 Sel output (1 = Voltage)
  AGPIO(GPIO_NRG_SEL_INV),    // HLW8012/HLJ-01 Sel output (0 = Voltage)
  AGPIO(GPIO_NRG_CF1),        // HLW8012/HLJ-01 CF1 voltage / current
  AGPIO(GPIO_HLW_CF),         // HLW8012 CF power
  AGPIO(GPIO_HJL_CF),         // HJL-01/BL0937 CF power
  AGPIO(GPIO_MCP39F5_TX),     // MCP39F501 Serial interface (Shelly2)
  AGPIO(GPIO_MCP39F5_RX),     // MCP39F501 Serial interface (Shelly2)
  AGPIO(GPIO_MCP39F5_RST),    // MCP39F501 Reset (Shelly2)
  AGPIO(GPIO_PN532_TXD),      // PN532 HSU Tx
  AGPIO(GPIO_PN532_RXD),      // PN532 HSU Rx
  AGPIO(GPIO_SM16716_CLK),    // SM16716 CLOCK
  AGPIO(GPIO_SM16716_DAT),    // SM16716 DATA
  AGPIO(GPIO_SM16716_SEL),    // SM16716 SELECT
  AGPIO(GPIO_DI),             // my92x1 PWM input
  AGPIO(GPIO_DCKI),           // my92x1 CLK input
  AGPIO(GPIO_CSE7766_TX),     // CSE7766 Serial interface (S31 and Pow R2)
  AGPIO(GPIO_CSE7766_RX),     // CSE7766 Serial interface (S31 and Pow R2)
  AGPIO(GPIO_ARIRFRCV),       // AriLux RF Receive input
  AGPIO(GPIO_TXD),            // Serial interface
  AGPIO(GPIO_RXD),            // Serial interface
  AGPIO(GPIO_ROT1A),          // Rotary A Pin
  AGPIO(GPIO_ROT1B),          // Rotary B Pin
  AGPIO(GPIO_ROT1A) +1,       // Rotary A Pin
  AGPIO(GPIO_ROT1B) +1,       // Rotary B Pin
  AGPIO(GPIO_HRE_CLOCK),
  AGPIO(GPIO_HRE_DATA),
  AGPIO(GPIO_ADE7953_IRQ),    // ADE7953 IRQ
  AGPIO(GPIO_LEDLNK),         // Link led
  AGPIO(GPIO_LEDLNK_INV),     // Inverted link led
  AGPIO(GPIO_ARIRFSEL),       // Arilux RF Receive input selected
  AGPIO(GPIO_BUZZER),         // Buzzer
  AGPIO(GPIO_BUZZER_INV),     // Inverted buzzer
  AGPIO(GPIO_OLED_RESET),     // OLED Display Reset
  AGPIO(GPIO_SOLAXX1_TX),     // Solax Inverter tx pin
  AGPIO(GPIO_SOLAXX1_RX),     // Solax Inverter rx pin
  AGPIO(GPIO_ZIGBEE_TX),      // Zigbee Serial interface
  AGPIO(GPIO_ZIGBEE_RX),      // Zigbee Serial interface
  AGPIO(GPIO_RDM6300_RX),
  AGPIO(GPIO_IBEACON_TX),
  AGPIO(GPIO_IBEACON_RX),
  AGPIO(GPIO_A4988_DIR),      // A4988 direction pin
  AGPIO(GPIO_A4988_STP),      // A4988 step pin
  AGPIO(GPIO_A4988_ENA),      // A4988 enabled pin
  AGPIO(GPIO_A4988_MS1),      // A4988 microstep pin1
  AGPIO(GPIO_A4988_MS1) +1,   // A4988 microstep pin2
  AGPIO(GPIO_A4988_MS1) +2,   // A4988 microstep pin3
  AGPIO(GPIO_DDS2382_TX),     // DDS2382 Serial interface
  AGPIO(GPIO_DDS2382_RX),     // DDS2382 Serial interface
  AGPIO(GPIO_DDSU666_TX),     // DDSU666 Serial interface
  AGPIO(GPIO_DDSU666_RX),     // DDSU666 Serial interface
  AGPIO(GPIO_SM2135_CLK),     // SM2135 CLOCK
  AGPIO(GPIO_SM2135_DAT),     // SM2135 DATA
  AGPIO(GPIO_DEEPSLEEP),
  AGPIO(GPIO_EXS_ENABLE),     // EXS MCU Enable
  AGPIO(GPIO_TASMOTACLIENT_TXD),     // Tasmota Client TX
  AGPIO(GPIO_TASMOTACLIENT_RXD),     // Tasmota Client RX
  AGPIO(GPIO_TASMOTACLIENT_RST),     // Tasmota Client Reset
  AGPIO(GPIO_TASMOTACLIENT_RST_INV), // Tasmota Client Reset Inverted
  AGPIO(GPIO_HPMA_RX),        // Honeywell HPMA115S0 Serial interface
  AGPIO(GPIO_HPMA_TX),        // Honeywell HPMA115S0 Serial interface
  AGPIO(GPIO_GPS_RX),         // GPS serial interface
  AGPIO(GPIO_GPS_TX),         // GPS serial interface
  AGPIO(GPIO_DSB_OUT),        // Pseudo Single wire DS18B20 or DS18S20
  AGPIO(GPIO_DHT11_OUT),      // Pseudo Single wire DHT11, DHT21, DHT22, AM2301, AM2302, AM2321
  AGPIO(GPIO_HM10_RX),        // GPS serial interface
  AGPIO(GPIO_HM10_TX),        // GPS serial interface
  AGPIO(GPIO_LE01MR_RX),      // F7F LE-01MR energy meter rx pin
  AGPIO(GPIO_LE01MR_TX),      // F7F LE-01MR energy meter tx pin
  AGPIO(GPIO_CC1101_GDO0),    // CC1101 pin for RX
  AGPIO(GPIO_CC1101_GDO2),    // CC1101 pin for RX
  AGPIO(GPIO_HRXL_RX),
  AGPIO(GPIO_ELECTRIQ_MOODL_TX),
  AGPIO(GPIO_AS3935),         // AS3935 IRQ Pin
  AGPIO(GPIO_PMS5003_TX),     // Plantower PMS5003 Serial interface
  AGPIO(GPIO_BOILER_OT_RX),
  AGPIO(GPIO_BOILER_OT_TX),
  AGPIO(GPIO_WINDMETER_SPEED),
  AGPIO(GPIO_BL0940_RX),      // BL0940 Serial interface
  AGPIO(GPIO_TCP_TX),         // TCP Serial bridge
  AGPIO(GPIO_TCP_RX),         // TCP Serial bridge
  AGPIO(GPIO_TELEINFO_RX),
  AGPIO(GPIO_TELEINFO_ENABLE),
  AGPIO(GPIO_LMT01),          // LMT01, count pulses on GPIO
  AGPIO(GPIO_IEM3000_TX),     // IEM3000 Serial interface
  AGPIO(GPIO_IEM3000_RX),     // IEM3000 Serial interface
  AGPIO(GPIO_ZIGBEE_RST),     // Zigbee reset
  AGPIO(GPIO_DYP_RX)
};

/********************************************************************************************/
#endif  // ESP8266

#endif  // _TASMOTA_TEMPLATE_LEGACY_H_
