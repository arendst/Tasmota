/*
  tasmota_template_ESP32_final.h - template settings for Tasmota

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

#ifndef _TASMOTA_TEMPLATE_ESP32_FINAL_H_
#define _TASMOTA_TEMPLATE_ESP32_FINAL_H_

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
  GPIO_NONE,           // Not used
  GPIO_KEY1,           // 4 x Button usually connected to GPIO0
  GPIO_KEY1_NP,
  GPIO_KEY1_INV,
  GPIO_KEY1_INV_NP,
  GPIO_SWT1,           // 8 x User connected external switches
  GPIO_SWT1_NP,
  GPIO_REL1,           // 8 x Relays
  GPIO_REL1_INV,
  GPIO_LED1,           // 4 x Leds
  GPIO_LED1_INV,
  GPIO_CNTR1,          // 4 x Counter
  GPIO_CNTR1_NP,
  GPIO_PWM1,           // 5 x PWM
  GPIO_PWM1_INV,
  GPIO_BUZZER,         // Buzzer
  GPIO_BUZZER_INV,     // Inverted buzzer
  GPIO_LEDLNK,         // Link led
  GPIO_LEDLNK_INV,     // Inverted link led
  GPIO_I2C_SCL,        // I2C SCL
  GPIO_I2C_SDA,        // I2C SDA
  GPIO_SPI_MISO,       // SPI MISO
  GPIO_SPI_MOSI,       // SPI MOSI
  GPIO_SPI_CLK,        // SPI Clk
  GPIO_SPI_CS,         // SPI Chip Select
  GPIO_SPI_DC,         // SPI Data Direction
  GPIO_SSPI_MISO,      // Software SPI Master Input Slave Output
  GPIO_SSPI_MOSI,      // Software SPI Master Output Slave Input
  GPIO_SSPI_SCLK,      // Software SPI Serial Clock
  GPIO_SSPI_CS,        // Software SPI Chip Select
  GPIO_SSPI_DC,        // Software SPI Data or Command
  GPIO_BACKLIGHT,      // Display backlight control
  GPIO_OLED_RESET,     // OLED Display Reset
  GPIO_IRSEND,         // IR remote
  GPIO_IRRECV,         // IR receiver
  GPIO_RFSEND,         // RF transmitter
  GPIO_RFRECV,         // RF receiver
  GPIO_DHT11,          // DHT11
  GPIO_DHT22,          // DHT21, DHT22, AM2301, AM2302, AM2321
  GPIO_SI7021,         // iTead SI7021
  GPIO_DHT11_OUT,      // Pseudo Single wire DHT11, DHT21, DHT22, AM2301, AM2302, AM2321
  GPIO_DSB,            // Single wire DS18B20 or DS18S20
  GPIO_DSB_OUT,        // Pseudo Single wire DS18B20 or DS18S20
  GPIO_WS2812,         // WS2812 Led string
  GPIO_MHZ_TXD,        // MH-Z19 Serial interface
  GPIO_MHZ_RXD,        // MH-Z19 Serial interface
  GPIO_PZEM0XX_TX,     // PZEM0XX Serial interface
  GPIO_PZEM004_RX,     // PZEM004T Serial interface
  GPIO_PZEM016_RX,     // PZEM-014,016 Serial Modbus interface
  GPIO_PZEM017_RX,     // PZEM-003,017 Serial Modbus interface
  GPIO_SAIR_TX,        // SenseAir Serial interface
  GPIO_SAIR_RX,        // SenseAir Serial interface
  GPIO_PMS5003_TX,     // Plantower PMS5003 Serial interface
  GPIO_PMS5003_RX,     // Plantower PMS5003 Serial interface
  GPIO_SDS0X1_TX,      // Nova Fitness SDS011 Serial interface
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
  GPIO_MP3_DFR562,     // RB-DFR-562, DFPlayer Mini MP3 Player
  GPIO_HX711_SCK,      // HX711 Load Cell clock
  GPIO_HX711_DAT,      // HX711 Load Cell data
  GPIO_TX2X_TXD_BLACK, // TX20/TX23 Transmission Pin
  GPIO_TUYA_TX,        // Tuya Serial interface
  GPIO_TUYA_RX,        // Tuya Serial interface
  GPIO_MGC3130_XFER,   // MGC3130 Transfer
  GPIO_MGC3130_RESET,  // MGC3130 Reset
  GPIO_RF_SENSOR,      // Rf receiver with sensor decoding
  GPIO_AZ_TXD,         // AZ-Instrument 7798 Serial interface
  GPIO_AZ_RXD,         // AZ-Instrument 7798 Serial interface
  GPIO_MAX31855CS,     // MAX31855 Serial interface
  GPIO_MAX31855CLK,    // MAX31855 Serial interface
  GPIO_MAX31855DO,     // MAX31855 Serial interface
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
  GPIO_ARIRFSEL,       // Arilux RF Receive input selected
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
  GPIO_HM10_RX,        // HM10-BLE-Mijia-bridge serial interface
  GPIO_HM10_TX,        // HM10-BLE-Mijia-bridge serial interface
  GPIO_LE01MR_RX,      // F&F LE-01MR energy meter
  GPIO_LE01MR_TX,      // F&F LE-01MR energy meter
  GPIO_CC1101_GDO0,    // CC1101 pin for RX
  GPIO_CC1101_GDO2,    // CC1101 pin for RX
  GPIO_HRXL_RX,        // Data from MaxBotix HRXL sonar range sensor
  GPIO_ELECTRIQ_MOODL_TX, // ElectriQ iQ-wifiMOODL Serial TX
  GPIO_AS3935,
/*
  ADC0_INPUT,          // Analog input
  ADC0_TEMP,           // Thermistor
  ADC0_LIGHT,          // Light sensor
  ADC0_BUTTON,         // Button
  ADC0_BUTTON_INV,
  ADC0_RANGE,          // Range
  ADC0_CT_POWER,       // Current
*/
  GPIO_SENSOR_END };

enum ProgramSelectablePins {
//  GPIO_FIX_START = 254,
  GPIO_FIX_START = 2046,
  GPIO_USER,           // User configurable needs to be 2047
  GPIO_MAX };

// Text in webpage Module Parameters and commands GPIOS and GPIO
const char kSensorNames[] PROGMEM =
  D_SENSOR_NONE "|"
  D_SENSOR_BUTTON "|"
  D_SENSOR_BUTTON "n|"
  D_SENSOR_BUTTON "i|"
  D_SENSOR_BUTTON "in|"
  D_SENSOR_SWITCH "|"
  D_SENSOR_SWITCH "n|"
  D_SENSOR_RELAY "|"
  D_SENSOR_RELAY "i|"
  D_SENSOR_LED "|"
  D_SENSOR_LED "i|"
  D_SENSOR_COUNTER "|"
  D_SENSOR_COUNTER "n|"
  D_SENSOR_PWM "|"
  D_SENSOR_PWM "i|"
  D_SENSOR_BUZZER "|"
  D_SENSOR_BUZZER "i|"
  D_SENSOR_LED_LINK "|" D_SENSOR_LED_LINK "i|"
  D_SENSOR_I2C_SCL "|" D_SENSOR_I2C_SDA "|"
  D_SENSOR_SPI_MISO "|" D_SENSOR_SPI_MOSI "|" D_SENSOR_SPI_CLK "|" D_SENSOR_SPI_CS "|" D_SENSOR_SPI_DC "|"
  D_SENSOR_SSPI_MISO "|" D_SENSOR_SSPI_MOSI "|" D_SENSOR_SSPI_SCLK "|" D_SENSOR_SSPI_CS "|" D_SENSOR_SSPI_DC "|"
  D_SENSOR_BACKLIGHT "|" D_SENSOR_OLED_RESET "|"
  D_SENSOR_IRSEND "|" D_SENSOR_IRRECV "|"
  D_SENSOR_RFSEND "|" D_SENSOR_RFRECV "|"
  D_SENSOR_DHT11 "|" D_SENSOR_AM2301 "|" D_SENSOR_SI7021 "|" D_SENSOR_DHT11 "o|"
  D_SENSOR_DS18X20 "|" D_SENSOR_DS18X20 "o|"
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
  D_SENSOR_NRG_SEL "|" D_SENSOR_NRG_SEL "i|" D_SENSOR_NRG_CF1 "|" D_SENSOR_HLW_CF "|" D_SENSOR_HJL_CF "|"
  D_SENSOR_MCP39F5_TX "|" D_SENSOR_MCP39F5_RX "|" D_SENSOR_MCP39F5_RST "|"
  D_SENSOR_PN532_TX "|" D_SENSOR_PN532_RX "|"
  D_SENSOR_SM16716_CLK "|" D_SENSOR_SM16716_DAT "|" D_SENSOR_SM16716_POWER "|"
  D_SENSOR_MY92X1_DI "|" D_SENSOR_MY92X1_DCKI "|"
  D_SENSOR_CSE7766_TX "|" D_SENSOR_CSE7766_RX "|"
  D_SENSOR_ARIRFRCV "|"
  D_SENSOR_TXD "|" D_SENSOR_RXD "|"
  D_SENSOR_ROTARY "1a|" D_SENSOR_ROTARY "1b|" D_SENSOR_ROTARY "2a|" D_SENSOR_ROTARY "2b|"
  D_SENSOR_HRE_CLOCK "|" D_SENSOR_HRE_DATA "|"
  D_SENSOR_ADE7953_IRQ "|"
  D_SENSOR_ARIRFSEL "|"
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
  D_SENSOR_HM10_RX "|" D_SENSOR_HM10_TX "|"
  D_SENSOR_LE01MR_RX "|" D_SENSOR_LE01MR_TX "|"
  D_SENSOR_CC1101_GDO0 "|" D_SENSOR_CC1101_GDO2 "|"
  D_SENSOR_HRXL_RX "|"
  D_SENSOR_ELECTRIQ_MOODL "|"
  D_SENSOR_AS3935 "|"
/*
  D_ANALOG_INPUT "|"
  D_TEMPERATURE "|" D_LIGHT "|"
  D_SENSOR_BUTTON "|" D_SENSOR_BUTTON "i|"
  D_RANGE "|"
  D_CT_POWER "|"
*/
  ;

const char kSensorNamesFixed[] PROGMEM =
  D_SENSOR_USER;

const uint16_t kGpioNiceList[] PROGMEM = {
  GPIO_NONE,           // Not used
  GPIO_KEY1,           // Buttons
  GPIO_KEY1_NP,
  GPIO_KEY1_INV,
  GPIO_KEY1_INV_NP,
  GPIO_SWT1,           // User connected external switches
  GPIO_SWT1_NP,
  GPIO_REL1,           // Relays
  GPIO_REL1_INV,
  GPIO_LED1,           // Leds
  GPIO_LED1_INV,
#ifdef USE_COUNTER
  GPIO_CNTR1,          // Counters
  GPIO_CNTR1_NP,
#endif
  GPIO_PWM1,           // RGB   Red   or C  Cold White
  GPIO_PWM1_INV,
#ifdef USE_BUZZER
  GPIO_BUZZER,         // Buzzer
  GPIO_BUZZER_INV,     // Inverted buzzer
#endif
  GPIO_LEDLNK,         // Link led
  GPIO_LEDLNK_INV,     // Inverted link led
#ifdef USE_I2C
  GPIO_I2C_SCL,        // I2C SCL
  GPIO_I2C_SDA,        // I2C SDA
#endif
#ifdef USE_SPI
  GPIO_SPI_MISO,       // SPI MISO
  GPIO_SPI_MOSI,       // SPI MOSI
  GPIO_SPI_CLK,        // SPI Clk
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

  GPIO_TXD,            // Serial interface
  GPIO_RXD,            // Serial interface

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
  GPIO_HPMA_TX,        // Honeywell HPMA115S0 Serial interface
  GPIO_HPMA_RX,        // Honeywell HPMA115S0 Serial interface
#endif
#ifdef USE_PMS5003
  GPIO_PMS5003_TX,     // Plantower PMS5003 Serial interface
  GPIO_PMS5003_RX,     // Plantower PMS5003 Serial interface
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
#ifdef USE_AS3935
  GPIO_AS3935,
#endif
/*
  ADC0_INPUT,          // Analog input
  ADC0_TEMP,           // Thermistor
  ADC0_LIGHT,          // Light sensor
  ADC0_BUTTON,         // Button
  ADC0_BUTTON_INV,
  ADC0_RANGE,          // Range
  ADC0_CT_POWER,       // Current
*/
};

//********************************************************************************************

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

//********************************************************************************************

#define MAX_GPIO_PIN       40   // Number of supported GPIO
#define MIN_FLASH_PINS     4    // Number of flash chip pins unusable for configuration (GPIO6, 7, 8 and 11)
#define MAX_USER_PINS      36   // MAX_GPIO_PIN - MIN_FLASH_PINS
#define ADC0_PIN           33   // Pin number of ADC0
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
{                                           // WEMOS - Espressif ESP32-DevKitC - Any ESP32 device like WeMos and NodeMCU hardware (ESP32)
  GPIO_USER << 5,            // 0       (I)O                GPIO0, ADC2_CH1, TOUCH1, RTC_GPIO11, CLK_OUT1, EMAC_TX_CLK
  GPIO_USER << 5,            // 1       IO     TXD0         GPIO1, U0TXD, CLK_OUT3, EMAC_RXD2
  GPIO_USER << 5,            // 2       IO                  GPIO2, ADC2_CH2, TOUCH2, RTC_GPIO12, HSPIWP, HS2_DATA0, SD_DATA0
  GPIO_USER << 5,            // 3       IO     RXD0         GPIO3, U0RXD, CLK_OUT2
  GPIO_USER << 5,            // 4       IO                  GPIO4, ADC2_CH0, TOUCH0, RTC_GPIO10, HSPIHD, HS2_DATA1, SD_DATA1, EMAC_TX_ER
  GPIO_USER << 5,            // 5       IO                  GPIO5, VSPICS0, HS1_DATA6, EMAC_RX_CLK
                             // 6       IO                  GPIO6, Flash CLK
                             // 7       IO                  GPIO7, Flash D0
                             // 8       IO                  GPIO8, Flash D1
  GPIO_USER << 5,            // 9       IO                  GPIO9, Flash D2, U1RXD
  GPIO_USER << 5,            // 10      IO                  GPIO10, Flash D3, U1TXD
                             // 11      IO                  GPIO11, Flash CMD
  GPIO_USER << 5,            // 12      (I)O                GPIO12, ADC2_CH5, TOUCH5, RTC_GPIO15, MTDI, HSPIQ, HS2_DATA2, SD_DATA2, EMAC_TXD3       (If driven High, flash voltage (VDD_SDIO) is 1.8V not default 3.3V. Has internal pull-down, so unconnected = Low = 3.3V. May prevent flashing and/or booting if 3.3V flash is connected and pulled high. See ESP32 datasheet for more details.)
  GPIO_USER << 5,            // 13      IO                  GPIO13, ADC2_CH4, TOUCH4, RTC_GPIO14, MTCK, HSPID, HS2_DATA3, SD_DATA3, EMAC_RX_ER
  GPIO_USER << 5,            // 14      IO                  GPIO14, ADC2_CH6, TOUCH6, RTC_GPIO16, MTMS, HSPICLK, HS2_CLK, SD_CLK, EMAC_TXD2
  GPIO_USER << 5,            // 15      (I)O                GPIO15, ADC2_CH3, TOUCH3, MTDO, HSPICS0, RTC_GPIO13, HS2_CMD, SD_CMD, EMAC_RXD3         (If driven Low, silences boot messages from normal boot. Has internal pull-up, so unconnected = High = normal output.)
  GPIO_USER << 5,            // 16      IO                  GPIO16, HS1_DATA4, U2RXD, EMAC_CLK_OUT
  GPIO_USER << 5,            // 17      IO                  GPIO17, HS1_DATA5, U2TXD, EMAC_CLK_OUT_180
  GPIO_USER << 5,            // 18      IO                  GPIO18, VSPICLK, HS1_DATA7
  GPIO_USER << 5,            // 19      IO                  GPIO19, VSPIQ, U0CTS, EMAC_TXD0
  0,                         // 20
  GPIO_USER << 5,            // 21      IO                  GPIO21, VSPIHD, EMAC_TX_EN
  GPIO_USER << 5,            // 22      IO      LED         GPIO22, VSPIWP, U0RTS, EMAC_TXD1
  GPIO_USER << 5,            // 23      IO                  GPIO23, VSPID, HS1_STROBE
  0,                         // 24
  GPIO_USER << 5,            // 25      IO                  GPIO25, DAC_1, ADC2_CH8, RTC_GPIO6, EMAC_RXD0
  GPIO_USER << 5,            // 26      IO                  GPIO26, DAC_2, ADC2_CH9, RTC_GPIO7, EMAC_RXD1
  GPIO_USER << 5,            // 27      IO                  GPIO27, ADC2_CH7, TOUCH7, RTC_GPIO17, EMAC_RX_DV
  0,                         // 28
  0,                         // 29
  0,                         // 30
  0,                         // 31
  GPIO_USER << 5,            // 32      IO                  GPIO32, XTAL_32K_P (32.768 kHz crystal oscillator input), ADC1_CH4, TOUCH9, RTC_GPIO9
  GPIO_USER << 5,            // 33      IO                  GPIO33, XTAL_32K_N (32.768 kHz crystal oscillator output), ADC1_CH5, TOUCH8, RTC_GPIO8
  GPIO_USER << 5,            // 34      I   NO PULLUP       GPIO34, ADC1_CH6, RTC_GPIO4
  GPIO_USER << 5,            // 35      I   NO PULLUP       GPIO35, ADC1_CH7, RTC_GPIO5
  GPIO_USER << 5,            // 36      I   NO PULLUP       GPIO36, SENSOR_VP, ADC_H, ADC1_CH0, RTC_GPIO0
  0,                         // 37          NO PULLUP
  0,                         // 38          NO PULLUP
  GPIO_USER << 5,            // 39      I   NO PULLUP       GPIO39, SENSOR_VN, ADC1_CH3, ADC_H, RTC_GPIO3
  0                          // Flag
};

#endif  // ESP32

#endif  // _TASMOTA_TEMPLATE_ESP32_FINAL_H_
