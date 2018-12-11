/*
  sonoff_template.h - template settings for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

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

#ifndef _SONOFF_TEMPLATE_H_
#define _SONOFF_TEMPLATE_H_

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
  GPIO_SR04_TRIG,      // SR04 Trigger pin
  GPIO_SR04_ECHO,      // SR04 Echo pin
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
  GPIO_TX20_TXD_BLACK, // TX20 Transmission Pin
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
  GPIO_SENSOR_END };

// Programmer selectable GPIO functionality offset by user selectable GPIOs
enum ProgramSelectablePins {
  GPIO_RXD = GPIO_SENSOR_END,  // Serial interface
  GPIO_TXD,            // Serial interface
  GPIO_SPI_MISO,       // SPI MISO library fixed pin GPIO12
  GPIO_SPI_MOSI,       // SPI MOSI library fixed pin GPIO13
  GPIO_SPI_CLK,        // SPI Clk library fixed pin GPIO14
  GPIO_NRG_SEL,        // HLW8012/HLJ-01 Sel output (1 = Voltage)
  GPIO_NRG_SEL_INV,    // HLW8012/HLJ-01 Sel output (0 = Voltage)
  GPIO_NRG_CF1,        // HLW8012/HLJ-01 CF1 voltage / current
  GPIO_HLW_CF,         // HLW8012 CF power
  GPIO_HJL_CF,         // HJL-01/BL0937 CF power
  GPIO_ADC0,           // ADC
  GPIO_DI,             // my92x1 PWM input
  GPIO_DCKI,           // my92x1 CLK input
  GPIO_ARIRFRCV,       // AliLux RF Receive input
  GPIO_USER,           // User configurable
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
  D_SENSOR_TX20_TX "|"
  D_SENSOR_RFSEND "|" D_SENSOR_RFRECV "|"
  D_SENSOR_TUYA_TX "|" D_SENSOR_TUYA_RX "|"
  D_SENSOR_MGC3130_XFER "|" D_SENSOR_MGC3130_RESET "|"
  D_SENSOR_SSPI_MISO "|" D_SENSOR_SSPI_MOSI "|" D_SENSOR_SSPI_SCLK "|" D_SENSOR_SSPI_CS "|" D_SENSOR_SSPI_DC "|"
  D_SENSOR_RF_SENSOR;

/********************************************************************************************/

// Supported hardware modules
enum SupportedModules {
  SONOFF_BASIC,
  SONOFF_RF,
  SONOFF_SV,
  SONOFF_TH,
  SONOFF_DUAL,
  SONOFF_POW,
  SONOFF_4CH,
  SONOFF_S2X,
  SLAMPHER,
  SONOFF_TOUCH,
  SONOFF_LED,
  CH1,
  CH4,
  MOTOR,
  ELECTRODRAGON,
  EXS_RELAY,
  WION,
  WEMOS,
  SONOFF_DEV,
  H801,
  SONOFF_SC,
  SONOFF_BN,
  SONOFF_4CHPRO,
  HUAFAN_SS,
  SONOFF_BRIDGE,
  SONOFF_B1,
  AILIGHT,
  SONOFF_T11,
  SONOFF_T12,
  SONOFF_T13,
  SUPLA1,
  WITTY,
  YUNSHAN,
  MAGICHOME,
  LUANIHVIO,
  KMC_70011,
  ARILUX_LC01,
  ARILUX_LC11,
  SONOFF_DUAL_R2,
  ARILUX_LC06,
  SONOFF_S31,
  ZENGGE_ZF_WF017,
  SONOFF_POW_R2,
  SONOFF_IFAN02,
  BLITZWOLF_BWSHP,
  SHELLY1,
  SHELLY2,
  PHILIPS,
  NEO_COOLCAM,
  ESP_SWITCH,
  OBI,
  TECKIN,
  APLIC_WDP303075,
  TUYA_DIMMER,
  GOSUND,
  ARMTRONIX_DIMMERS,
  SK03_TUYA,
  PS_16_DZ,
  TECKIN_US,
  MANZOKU_EU_4,
  MAXMODULE };

/********************************************************************************************/

#define MAX_GPIO_PIN       18   // Number of supported GPIO

const char PINS_WEMOS[] PROGMEM = "D3TXD4RXD2D1flashcFLFLolD6D7D5D8D0A0";

typedef struct MYIO {
  uint8_t      io[MAX_GPIO_PIN];
} myio;

typedef struct MYTMPLT {
  char         name[15];
  myio         gp;
} mytmplt;

const uint8_t kGpioNiceList[] PROGMEM = {
  GPIO_NONE,           // Not used
  GPIO_KEY1,           // Buttons
  GPIO_KEY1_NP,
  GPIO_KEY2,
  GPIO_KEY2_NP,
  GPIO_KEY3,
  GPIO_KEY3_NP,
  GPIO_KEY4,
  GPIO_KEY4_NP,
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
  GPIO_CNTR1,          // Counters
  GPIO_CNTR1_NP,
  GPIO_CNTR2,
  GPIO_CNTR2_NP,
  GPIO_CNTR3,
  GPIO_CNTR3_NP,
  GPIO_CNTR4,
  GPIO_CNTR4_NP,
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
#endif
  GPIO_DHT11,          // DHT11
  GPIO_DHT22,          // DHT21, DHT22, AM2301, AM2302, AM2321
  GPIO_SI7021,         // iTead SI7021
  GPIO_DSB,            // Single wire DS18B20 or DS18S20
#ifdef USE_WS2812
  GPIO_WS2812,         // WS2812 Led string
#endif
#ifdef USE_IR_REMOTE
  GPIO_IRSEND,         // IR remote
#ifdef USE_IR_RECEIVE
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
  GPIO_SR04_TRIG,      // SR04 Trigger pin
  GPIO_SR04_ECHO,      // SR04 Echo pin
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
#ifdef USE_SERIAL_BRIDGE
  GPIO_SBR_TX,         // Serial Bridge Serial interface
  GPIO_SBR_RX,         // Serial Bridge Serial interface
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
#ifdef USE_PMS5003
  GPIO_PMS5003,        // Plantower PMS5003 Serial interface
#endif
#ifdef USE_TX20_WIND_SENSOR
  GPIO_TX20_TXD_BLACK, // TX20 Transmission Pin
#endif
#ifdef USE_MP3_PLAYER
  GPIO_MP3_DFR562,     // RB-DFR-562, DFPlayer Mini MP3 Player Serial interface
#endif
#ifdef USE_TUYA_DIMMER
  GPIO_TUYA_TX,        // Tuya Serial interface
  GPIO_TUYA_RX,         // Tuya Serial interface
#endif
#ifdef USE_MGC3130
  GPIO_MGC3130_XFER,
  GPIO_MGC3130_RESET
#endif
};

const uint8_t kModuleNiceList[MAXMODULE] PROGMEM = {
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
  SONOFF_LED,          // Sonoff Light Devices
  SONOFF_BN,
  SONOFF_B1,           // Sonoff Light Bulbs
  SLAMPHER,
  SONOFF_SC,           // Sonoff Environmemtal Sensor
  SONOFF_IFAN02,       // Sonoff Fan
  SONOFF_BRIDGE,       // Sonoff Bridge
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
  BLITZWOLF_BWSHP,    // Socket Relay Devices with Energy Monitoring
  TECKIN,
  TECKIN_US,
  APLIC_WDP303075,
  GOSUND,
  SK03_TUYA,
  NEO_COOLCAM,        // Socket Relay Devices
  OBI,
  MANZOKU_EU_4,
  ESP_SWITCH,         // Switch Devices
  TUYA_DIMMER,        // Dimmer Devices
  ARMTRONIX_DIMMERS,
  PS_16_DZ,
  H801,               // Light Devices
  MAGICHOME,
  ARILUX_LC01,
  ARILUX_LC06,
  ARILUX_LC11,
  ZENGGE_ZF_WF017,
  HUAFAN_SS,
  KMC_70011,
  AILIGHT,            // Light Bulbs
  PHILIPS,
  WITTY,              // Development Devices
  WEMOS
};

// Default module settings
const mytmplt kModules[MAXMODULE] PROGMEM = {
  { "Sonoff Basic",    // Sonoff Basic (ESP8266)
     GPIO_KEY1,        // GPIO00 Button
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
     GPIO_USER,        // GPIO02 Only available on newer Sonoff Basic R2 V1
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     GPIO_USER,        // GPIO04 Optional sensor
     0,                // GPIO05
     0,                // GPIO06 (SD_CLK   Flash)
     0,                // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
     0,                // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
     0,                // GPIO09 (SD_DATA2 Flash QIO)
     0,                // GPIO10 (SD_DATA3 Flash QIO)
     0,                // GPIO11 (SD_CMD   Flash)
     GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off)
     GPIO_USER,        // GPIO14 Optional sensor
     0,                // GPIO15
     0,                // GPIO16
     0                 // ADC0 Analog input
  },
  { "Sonoff RF",       // Sonoff RF (ESP8266)
     GPIO_KEY1,        // GPIO00 Button
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
     0,
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     GPIO_USER,        // GPIO04 Optional sensor
     0,
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off)
     GPIO_USER,        // GPIO14 Optional sensor
     0, 0, 0
  },
  { "Sonoff SV",       // Sonoff SV (ESP8266)
     GPIO_KEY1,        // GPIO00 Button
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
     0,
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     GPIO_USER,        // GPIO04 Optional sensor
     GPIO_USER,        // GPIO05 Optional sensor
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off)
     GPIO_USER,        // GPIO14 Optional sensor
     0, 0,
     GPIO_ADC0         // ADC0 Analog input
  },
  { "Sonoff TH",       // Sonoff TH10/16 (ESP8266)
     GPIO_KEY1,        // GPIO00 Button
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
     0,
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     GPIO_USER,        // GPIO04 Optional sensor
     0,
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off)
     GPIO_USER,        // GPIO14 Optional sensor
     0, 0, 0
  },
  { "Sonoff Dual",     // Sonoff Dual (ESP8266)
     0,
     GPIO_TXD,         // GPIO01 Relay control
     0,
     GPIO_RXD,         // GPIO03 Relay control
     GPIO_USER,        // GPIO04 Optional sensor
     0,
     0, 0, 0, 0, 0, 0, // Flash connection
     0,
     GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off)
     GPIO_USER,        // GPIO14 Optional sensor
     0, 0, 0
  },
  { "Sonoff Pow",      // Sonoff Pow (ESP8266 - HLW8012)
     GPIO_KEY1,        // GPIO00 Button
     0, 0, 0, 0,
     GPIO_NRG_SEL,     // GPIO05 HLW8012 Sel output (1 = Voltage)
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
     GPIO_NRG_CF1,     // GPIO13 HLW8012 CF1 voltage / current
     GPIO_HLW_CF,      // GPIO14 HLW8012 CF power
     GPIO_LED1,        // GPIO15 Blue Led (0 = On, 1 = Off)
     0, 0
  },
  { "Sonoff 4CH",      // Sonoff 4CH (ESP8285)
     GPIO_KEY1,        // GPIO00 Button 1
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
     GPIO_USER,        // GPIO02 Optional sensor
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     GPIO_REL3,        // GPIO04 Sonoff 4CH Red Led and Relay 3 (0 = Off, 1 = On)
     GPIO_REL2,        // GPIO05 Sonoff 4CH Red Led and Relay 2 (0 = Off, 1 = On)
     0, 0, 0,          // Flash connection
     GPIO_KEY2,        // GPIO09 Button 2
     GPIO_KEY3,        // GPIO10 Button 3
     0,                // Flash connection
     GPIO_REL1,        // GPIO12 Red Led and Relay 1 (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off)
     GPIO_KEY4,        // GPIO14 Button 4
     GPIO_REL4,        // GPIO15 Red Led and Relay 4 (0 = Off, 1 = On)
     0, 0
  },
  { "Sonoff S2X",      // Sonoff S20, S22 and S26 Smart Socket (ESP8266)
     GPIO_KEY1,        // GPIO00 Button
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
     GPIO_USER,        // GPIO02 Optional sensor
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     0, 0,
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Green/Blue Led (0 = On, 1 = Off)
     0, 0, 0, 0
  },
  { "Slampher",        // Slampher (ESP8266)
     GPIO_KEY1,        // GPIO00 Button
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
     0,
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     0, 0,
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off)
     0, 0, 0, 0
  },
  { "Sonoff Touch",    // Sonoff Touch (ESP8285)
     GPIO_KEY1,        // GPIO00 Button
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
     0,
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     0, 0,
     0, 0, 0,          // Flash connection
     0, 0,
     0,                // Flash connection
     GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off)
     0, 0, 0, 0
  },
  { "Sonoff LED",      // Sonoff LED (ESP8266)
     GPIO_KEY1,        // GPIO00 Button
     0, 0, 0,
     GPIO_USER,        // GPIO04 Optional sensor (PWM3 Green)
     GPIO_USER,        // GPIO05 Optional sensor (PWM2 Red)
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_PWM1,        // GPIO12 Cold light (PWM0 Cold)
     GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off)
     GPIO_PWM2,        // GPIO14 Warm light (PWM1 Warm)
     GPIO_USER,        // GPIO15 Optional sensor (PWM4 Blue)
     0, 0
  },
  { "1 Channel",       // 1 Channel Inching/Latching Relay using (PSA-B01 - ESP8266 and PSF-B01 - ESP8285)
     GPIO_KEY1,        // GPIO00 Button
     0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off)
     0, 0, 0, 0
  },
  { "4 Channel",       // 4 Channel Inching/Latching Relays (ESP8266)
     0,
     GPIO_TXD,         // GPIO01 Relay control
     0,
     GPIO_RXD,         // GPIO03 Relay control
     0, 0,
     0, 0, 0, 0, 0, 0, // Flash connection
     0,
     GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off)
     0, 0, 0, 0
  },
  { "Motor C/AC",      // Motor Clockwise / Anti clockwise (PSA-B01 - ESP8266)
     GPIO_KEY1,        // GPIO00 Button
     0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off)
     0, 0, 0, 0
  },
  { "ElectroDragon",   // ElectroDragon IoT Relay Board (ESP8266)
     GPIO_KEY2,        // GPIO00 Button 2
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
     GPIO_KEY1,        // GPIO02 Button 1
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     GPIO_USER,        // GPIO04 Optional sensor
     GPIO_USER,        // GPIO05 Optional sensor
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_REL2,        // GPIO12 Red Led and Relay 2 (0 = Off, 1 = On)
     GPIO_REL1,        // GPIO13 Red Led and Relay 1 (0 = Off, 1 = On)
     GPIO_USER,        // GPIO14 Optional sensor
     GPIO_USER,        // GPIO15 Optional sensor
     GPIO_LED1,        // GPIO16 Green/Blue Led (1 = On, 0 = Off)
     GPIO_ADC0         // ADC0   A0 Analog input
  },
  { "EXS Relay(s)",    // ES-Store Latching relay(s) (ESP8266)
                       // https://ex-store.de/ESP8266-WiFi-Relay-V31
                       //   V3.1 Module Pin 1 VCC 3V3, Module Pin 6 GND
                       // https://ex-store.de/2-Kanal-WiFi-WLan-Relay-V5-Blackline-fuer-Unterputzmontage
     GPIO_USER,        // GPIO00 V3.1 Module Pin 8 - V5.0 Module Pin 4
     GPIO_USER,        // GPIO01 UART0_TXD V3.1 Module Pin 2 - V5.0 Module Pin 3
     GPIO_USER,        // GPIO02 V3.1 Module Pin 7
     GPIO_USER,        // GPIO03 UART0_RXD V3.1 Module Pin 3
     GPIO_USER,        // GPIO04 V3.1 Module Pin 10 - V5.0 Module Pin 2
     GPIO_USER,        // GPIO05 V3.1 Module Pin 9 - V5.0 Module Pin 1
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_REL1,        // GPIO12 Relay1 ( 1 = Off)
     GPIO_REL2,        // GPIO13 Relay1 ( 1 = On)
     GPIO_USER,        // GPIO14 V3.1 Module Pin 5 - V5.0 GPIO_REL3_INV Relay2 ( 1 = Off)
     GPIO_LED1,        // GPIO15 V5.0 LED1
     GPIO_USER,        // GPIO16 V3.1 Module Pin 4 - V5.0 GPIO_REL4_INV Relay2 ( 1 = On)
     0
  },
  { "WiOn",            // Indoor Tap (ESP8266)
                       // https://www.amazon.com/gp/product/B00ZYLUBJU/ref=s9_acsd_al_bw_c_x_3_w
     GPIO_USER,        // GPIO00 Optional sensor (pm clock)
     0,
     GPIO_LED1,        // GPIO02 Green Led (1 = On, 0 = Off)
     0, 0, 0,
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_USER,        // GPIO12 Optional sensor (pm data)
     GPIO_KEY1,        // GPIO13 Button
     0,
     GPIO_REL1,        // GPIO15 Relay (0 = Off, 1 = On)
     0, 0
  },
  { "Generic",         // Any ESP8266/ESP8285 device like WeMos and NodeMCU hardware (ESP8266)
     GPIO_USER,        // GPIO00 D3 Wemos Button Shield
     GPIO_USER,        // GPIO01 TX Serial RXD
     GPIO_USER,        // GPIO02 D4 Wemos DHT Shield
     GPIO_USER,        // GPIO03 RX Serial TXD and Optional sensor
     GPIO_USER,        // GPIO04 D2 Wemos I2C SDA
     GPIO_USER,        // GPIO05 D1 Wemos I2C SCL / Wemos Relay Shield (0 = Off, 1 = On) / Wemos WS2812B RGB led Shield
     0, 0, 0,          // Flash connection
     GPIO_USER,        // Flash connection or GPIO09 on ESP8285 only!
     GPIO_USER,        // Flash connection or GPIO10 on ESP8285 only!
     0,                // Flash connection
     GPIO_USER,        // GPIO12 D6
     GPIO_USER,        // GPIO13 D7
     GPIO_USER,        // GPIO14 D5
     GPIO_USER,        // GPIO15 D8
     GPIO_USER,        // GPIO16 D0 Wemos Wake
     GPIO_ADC0         // ADC0   A0 Analog input
  },
  { "Sonoff Dev",      // Sonoff Dev (ESP8266)
     GPIO_KEY1,        // GPIO00 E-FW Button
     GPIO_USER,        // GPIO01 TX Serial RXD and Optional sensor
     0,                // GPIO02
     GPIO_USER,        // GPIO03 RX Serial TXD and Optional sensor
     GPIO_USER,        // GPIO04 Optional sensor
     GPIO_USER,        // GPIO05 Optional sensor
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_USER,        // GPIO12
     GPIO_USER,        // GPIO13 BLUE LED
     GPIO_USER,        // GPIO14 Optional sensor
     0,                // GPIO15
     0,                // GPIO16
     GPIO_ADC0         // ADC0 A0 Analog input
  },
  { "H801",            // Lixada H801 Wifi (ESP8266)
     GPIO_USER,        // GPIO00 E-FW Button
     GPIO_LED1,        // GPIO01 Green LED
     GPIO_USER,        // GPIO02 TX and Optional sensor - Pin next to TX on the PCB
     GPIO_USER,        // GPIO03 RX and Optional sensor - Pin next to GND on the PCB
     GPIO_PWM5,        // GPIO04 W2 - PWM5
     GPIO_LED2_INV,    // GPIO05 Red LED
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_PWM3,        // GPIO12 Blue
     GPIO_PWM2,        // GPIO13 Green
     GPIO_PWM4,        // GPIO14 W1 - PWM4
     GPIO_PWM1,        // GPIO15 Red
     0, 0
  },
  { "Sonoff SC",       // Sonoff SC (ESP8266)
     GPIO_KEY1,        // GPIO00 Button
     GPIO_TXD,         // GPIO01 RXD to ATMEGA328P
     GPIO_USER,        // GPIO02 Optional sensor
     GPIO_RXD,         // GPIO03 TXD to ATMEGA328P
     0, 0,
     0, 0, 0, 0, 0, 0, // Flash connection
     0,
     GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off)
     0, 0, 0, 0
  },
  { "Sonoff BN-SZ",    // Sonoff BN-SZ01 Ceiling led (ESP8285)
     0, 0, 0, 0, 0, 0,
     0, 0, 0,          // Flash connection
     0, 0,
     0,                // Flash connection
     GPIO_PWM1,        // GPIO12 Light
     GPIO_LED1_INV,    // GPIO13 Red Led (0 = On, 1 = Off)
     0, 0,
     0, 0
  },
  { "Sonoff 4CH Pro",  // Sonoff 4CH Pro (ESP8285)
     GPIO_KEY1,        // GPIO00 Button 1
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
     GPIO_USER,        // GPIO02 Optional sensor
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     GPIO_REL3,        // GPIO04 Sonoff 4CH Red Led and Relay 3 (0 = Off, 1 = On)
     GPIO_REL2,        // GPIO05 Sonoff 4CH Red Led and Relay 2 (0 = Off, 1 = On)
     0, 0, 0,          // Flash connection
     GPIO_KEY2,        // GPIO09 Button 2
     GPIO_KEY3,        // GPIO10 Button 3
     0,                // Flash connection
     GPIO_REL1,        // GPIO12 Red Led and Relay 1 (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off)
     GPIO_KEY4,        // GPIO14 Button 4
     GPIO_REL4,        // GPIO15 Red Led and Relay 4 (0 = Off, 1 = On)
     0, 0
  },
  { "Huafan SS",       // Hua Fan Smart Socket (ESP8266) - like Sonoff Pow
     GPIO_LED1_INV,    // GPIO0 Blue Led (0 = On, 1 = Off)
     0, 0,
     GPIO_LED2_INV,    // GPIO3 Red Led (0 = On, 1 = Off)
     GPIO_KEY1,        // GPIO4 Button
     GPIO_REL1_INV,    // GPIO5 Relay (0 = On, 1 = Off)
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_NRG_CF1,     // GPIO12 HLW8012 CF1 voltage / current
     GPIO_NRG_SEL,     // GPIO13 HLW8012 Sel output (1 = Voltage)
     GPIO_HLW_CF,      // GPIO14 HLW8012 CF power
     0, 0, 0
  },
  { "Sonoff Bridge",   // Sonoff RF Bridge 433 (ESP8285)
     GPIO_KEY1,        // GPIO00 Button
     GPIO_TXD,         // GPIO01 RF bridge control
     GPIO_USER,        // GPIO02 Optional sensor
     GPIO_RXD,         // GPIO03 RF bridge control
     GPIO_USER,        // GPIO04 Optional sensor
     GPIO_USER,        // GPIO05 Optional sensor
     0, 0, 0,          // Flash connection
     0, 0,
     0,                // Flash connection
     0,
     GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off)
     0, 0, 0, 0
  },
  { "Sonoff B1",       // Sonoff B1 (ESP8285 - my9231)
     GPIO_KEY1,        // GPIO00 Pad
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor pad
     GPIO_USER,        // GPIO02 Optional sensor SDA pad
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor pad
     0, 0,
     0, 0, 0,          // Flash connection
     0, 0,
     0,                // Flash connection
     GPIO_DI,          // GPIO12 my9231 DI
     0,
     GPIO_DCKI,        // GPIO14 my9231 DCKI
     0, 0, 0
  },
  { "AiLight",         // Ai-Thinker RGBW led (ESP8266 - my9291)
     GPIO_KEY1,        // GPIO00 Pad
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor pad
     GPIO_USER,        // GPIO02 Optional sensor SDA pad
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor pad
     0, 0,
     0, 0, 0,          // Flash connection
     0, 0,
     0,                // Flash connection
     0,
     GPIO_DI,          // GPIO13 my9291 DI
     0,
     GPIO_DCKI,        // GPIO15 my9291 DCKI
     0, 0
  },
  { "Sonoff T1 1CH",   // Sonoff T1 1CH (ESP8285)
     GPIO_KEY1,        // GPIO00 Button 1
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
     GPIO_USER,        // GPIO02 Optional Sensor (J3 Pin 5)
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     0, 0,
     0, 0, 0,          // Flash connection
     0, 0,
     0,                // Flash connection
     GPIO_REL1,        // GPIO12 Blue Led and Relay 1 (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off)
     0, 0, 0, 0
  },
  { "Sonoff T1 2CH",   // Sonoff T1 2CH (ESP8285)
     GPIO_KEY1,        // GPIO00 Button 1
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
     GPIO_USER,        // GPIO02 Optional Sensor (J3 Pin 5)
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     0,
     GPIO_REL2,        // GPIO05 Blue Led and Relay 2 (0 = Off, 1 = On)
     0, 0, 0,          // Flash connection
     GPIO_KEY2,        // GPIO09 Button 2
     0,
     0,                // Flash connection
     GPIO_REL1,        // GPIO12 Blue Led and Relay 1 (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off)
     0, 0, 0, 0
  },
  { "Sonoff T1 3CH",   // Sonoff T1 3CH (ESP8285)
     GPIO_KEY1,        // GPIO00 Button 1
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
     GPIO_USER,        // GPIO02 Optional Sensor (J3 Pin 5)
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     GPIO_REL3,        // GPIO04 Blue Led and Relay 3 (0 = Off, 1 = On)
     GPIO_REL2,        // GPIO05 Blue Led and Relay 2 (0 = Off, 1 = On)
     0, 0, 0,          // Flash connection
     GPIO_KEY2,        // GPIO09 Button 2
     GPIO_KEY3,        // GPIO10 Button 3
     0,                // Flash connection
     GPIO_REL1,        // GPIO12 Blue Led and Relay 1 (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off)
     0, 0, 0, 0
  },
  { "Supla Espablo",   // Supla Espablo (ESP8266)
                       // http://www.wykop.pl/ramka/3325399/diy-supla-do-puszki-instalacyjnej-podtynkowej-supla-org/
     0,                // GPIO00 Flash jumper
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
     GPIO_DSB,         // GPIO02 DS18B20 sensor
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     GPIO_KEY1,        // GPIO04 Button 1
     GPIO_REL1,        // GPIO05 Relay 1 (0 = Off, 1 = On)
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_USER,        // GPIO12 Optional sensor
     GPIO_REL2,        // GPIO13 Relay 2 (0 = Off, 1 = On)
     GPIO_USER,        // GPIO14 Optional sensor
     0,
     GPIO_LED1,        // GPIO16 Led (1 = On, 0 = Off)
     GPIO_ADC0         // ADC0 A0 Analog input
  },
  { "Witty Cloud",     // Witty Cloud Dev Board (ESP8266)
                       // https://www.aliexpress.com/item/ESP8266-serial-WIFI-Witty-cloud-Development-Board-ESP-12F-module-MINI-nodemcu/32643464555.html
     GPIO_USER,        // GPIO00 D3 flash push button on interface board
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
     GPIO_LED1_INV,    // GPIO02 D4 Blue Led (0 = On, 1 = Off) on ESP-12F
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     GPIO_KEY1,        // GPIO04 D2 push button on ESP-12F board
     GPIO_USER,        // GPIO05 D1 optional sensor
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_PWM2,        // GPIO12 D6 RGB LED Green
     GPIO_PWM3,        // GPIO13 D7 RGB LED Blue
     GPIO_USER,        // GPIO14 D5 optional sensor
     GPIO_PWM1,        // GPIO15 D8 RGB LED Red
     GPIO_USER,        // GPIO16 D0 optional sensor
     GPIO_ADC0         // ADC0 A0 Light sensor / Requires USE_ADC_VCC in user_config.h to be disabled
  },
  { "Yunshan Relay",   // Yunshan Wifi Relay (ESP8266)
                       // https://www.ebay.com/p/Esp8266-220v-10a-Network-Relay-WiFi-Module/1369583381
                       // Schematics and Info https://ucexperiment.wordpress.com/2016/12/18/yunshan-esp8266-250v-15a-acdc-network-wifi-relay-module/
     0,                // GPIO00 Flash jumper - Module Pin 8
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor - Module Pin 2
     GPIO_LED1_INV,    // GPIO02 Blue Led (0 = On, 1 = Off) on ESP-12F - Module Pin 7
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor - Module Pin 3
     GPIO_REL1,        // GPIO04 Red Led and Relay (0 = Off, 1 = On) - Module Pin 10
     GPIO_KEY1,        // GPIO05 Blue Led and OptoCoupler input - Module Pin 9
     0, 0, 0, 0, 0, 0, // Flash connection
     0, 0, 0, 0, 0
  },
  { "MagicHome",       // Magic Home (aka Flux-light) (ESP8266) and Arilux LC10 (ESP8285)
                       // https://www.aliexpress.com/item/Magic-Home-Mini-RGB-RGBW-Wifi-Controller-For-Led-Strip-Panel-light-Timing-Function-16million-colors/32686853650.html
     0,
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
     GPIO_LED1_INV,    // GPIO02 Blue onboard LED
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     GPIO_ARIRFRCV,    // GPIO04 IR or RF receiver (optional) (Arilux LC10)
     GPIO_PWM2,        // GPIO05 RGB LED Green
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_PWM3,        // GPIO12 RGB LED Blue
     GPIO_USER,        // GPIO13 RGBW LED White (optional - set to PWM4 for Cold White or Warm White as used on Arilux LC10)
     GPIO_PWM1,        // GPIO14 RGB LED Red
     GPIO_LED2_INV,    // GPIO15 RF receiver control (Arilux LC10)
     0, 0
  },
  { "Luani HVIO",      // ESP8266_HVIO
                       // https://luani.de/projekte/esp8266-hvio/
     0,                // GPIO00 Flash jumper
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
     GPIO_USER,        // GPIO02 Optional sensor / I2C SDA pad
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     GPIO_REL1,        // GPIO04 Relay 1 (0 = Off, 1 = On)
     GPIO_REL2,        // GPIO05 Relay 2 (0 = Off, 1 = On)
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_SWT1,        // GPIO12 External input 1 (0 = On, 1 = Off)
     GPIO_SWT2,        // GPIO13 External input 2 (0 = On, 1 = Off)
     GPIO_USER,        // GPIO14 Optional sensor / I2C SCL pad
     GPIO_LED1,        // GPIO15 Led (1 = On, 0 = Off)
     0,
     GPIO_ADC0         // ADC0 A0 Analog input
  },
  { "KMC 70011",       // KMC 70011
                       // https://www.amazon.com/KMC-Timing-Monitoring-Network-125V-240V/dp/B06XRX2GTQ
     GPIO_KEY1,        // GPIO00 Button
     0, 0, 0,
     GPIO_HLW_CF,      // GPIO04 HLW8012 CF power
     GPIO_NRG_CF1,     // GPIO05 HLW8012 CF1 voltage / current
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_NRG_SEL,     // GPIO12 HLW8012 SEL (1 = Voltage)
     GPIO_LED1_INV,    // GPIO13 Green Led
     GPIO_REL1,        // GPIO14 Relay
     0, 0, 0
  },
  { "Arilux LC01",     // Arilux AL-LC01 (ESP8285)
                       // https://www.banggood.com/nl/ARILUX-AL-LC01-Super-Mini-LED-WIFI-Smart-RGB-Controller-For-RGB-LED-Strip-Light-DC-9-12V-p-1058603.html
                       //  (PwmFrequency 1111Hz)
     GPIO_KEY1,        // GPIO00 Optional Button
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
     GPIO_LED2_INV,    // GPIO02 RF receiver control
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     GPIO_ARIRFRCV,    // GPIO04 IR or RF receiver (optional)
     GPIO_PWM1,        // GPIO05 RGB LED Red
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_PWM2,        // GPIO12 RGB LED Green
     GPIO_PWM3,        // GPIO13 RGB LED Blue
     GPIO_USER,        // GPIO14 RGBW LED White (optional - set to PWM4 for Cold White or Warm White)
     0, 0, 0
  },
  { "Arilux LC11",     // Arilux AL-LC11 (ESP8266)
                       // https://www.banggood.com/nl/ARILUX-AL-LC11-Super-Mini-LED-WIFI-APP-Controller-RF-Remote-Control-For-RGBWW-LED-Strip-DC9-28V-p-1085112.html
                       //  (PwmFrequency 540Hz)
     GPIO_KEY1,        // GPIO00 Optional Button
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
     GPIO_LED2_INV,    // GPIO02 RF receiver control
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     GPIO_PWM2,        // GPIO04 RGB LED Green
     GPIO_PWM1,        // GPIO05 RGB LED Red
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_PWM5,        // GPIO12 RGBCW LED Warm
     GPIO_PWM4,        // GPIO13 RGBW LED Cold
     GPIO_PWM3,        // GPIO14 RGB LED Blue
     GPIO_ARIRFRCV,    // GPIO15 RF receiver input
     0, 0
  },
  { "Sonoff Dual R2",  // Sonoff Dual R2 (ESP8285)
     GPIO_USER,        // GPIO00 Button 0 on header (0 = On, 1 = Off)
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
     0,
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     0,
     GPIO_REL2,        // GPIO05 Relay 2 (0 = Off, 1 = On)
     0, 0, 0,          // Flash connection
     GPIO_USER,        // GPIO09 Button 1 on header (0 = On, 1 = Off)
     GPIO_KEY1,        // GPIO10 Button on casing
     0,                // Flash connection
     GPIO_REL1,        // GPIO12 Relay 1 (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off)
     0, 0, 0, 0
  },
  { "Arilux LC06",     // Arilux AL-LC06 (ESP8285)
                       // https://www.banggood.com/ARILUX-AL-LC06-LED-WIFI-Smartphone-Controller-Romote-5-Channels-DC12-24V-For-RGBWW-Strip-light-p-1061476.html
     GPIO_KEY1,        // GPIO00 Optional Button
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
     GPIO_USER,        // GPIO02 Empty pad
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     GPIO_USER,        // GPIO04 W2 - PWM5
     0,
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_PWM2,        // GPIO12 RGB LED Green
     GPIO_PWM3,        // GPIO13 RGB LED Blue
     GPIO_PWM1,        // GPIO14 RGB LED Red
     GPIO_USER,        // GPIO15 RGBW LED White
     0, 0
  },
  { "Sonoff S31",      // Sonoff S31 (ESP8266 - CSE7766)
     GPIO_KEY1,        // GPIO00 Button
     0,                // GPIO01 Serial RXD 4800 baud 8E1 CSE7766 energy sensor
     0,
     0,                // GPIO03 Serial TXD
     0, 0,
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off)
     0, 0, 0, 0
  },
  { "Zengge WF017",    // Zenggee ZJ-WF017-A (ESP12S))
                       // https://www.ebay.com/p/Smartphone-Android-IOS-WiFi-Music-Controller-for-RGB-5050-3528-LED-Strip-Light/534446632?_trksid=p2047675.l2644
     GPIO_KEY1,        // GPIO00 Optional Button
     0,
     GPIO_USER,        // GPIO02 Empty pad
     0,
     GPIO_USER,        // GPIO04 W2 - PWM5
     0,
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_PWM2,        // GPIO12 RGB LED Green
     GPIO_PWM1,        // GPIO13 RGB LED Red
     GPIO_PWM3,        // GPIO14 RGB LED Blue
     0, 0, 0
  },
  { "Sonoff Pow R2",   // Sonoff Pow R2 (ESP8285 - CSE7766)
     GPIO_KEY1,        // GPIO00 Button
     0,                // GPIO01 Serial RXD 4800 baud 8E1 CSE7766 energy sensor
     0,
     0,                // GPIO03 Serial TXD
     0, 0,
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off)
     0, 0, 0, 0
  },
  { "Sonoff iFan02",   // Sonoff iFan02 (ESP8285)
     GPIO_KEY1,        // GPIO00 WIFI_KEY0 Virtual button 1 as feedback from RC
     GPIO_USER,        // GPIO01 ESP_TXD Serial RXD and Optional sensor
     0,                // GPIO02 ESP_LOG
     GPIO_USER,        // GPIO03 ESP_RXD Serial TXD and Optional sensor
     GPIO_REL3,        // GPIO04 WIFI_O2 Relay 3 (0 = Off, 1 = On) controlling the fan
     GPIO_REL2,        // GPIO05 WIFI_O1 Relay 2 (0 = Off, 1 = On) controlling the fan
     0, 0, 0,          // Flash connection
     GPIO_KEY2,        // GPIO09 WIFI_KEY1 Virtual button 2 as feedback from RC
     GPIO_KEY3,        // GPIO10 WIFI_KEY2 Virtual button 3 as feedback from RC
     0,                // Flash connection
     GPIO_REL1,        // GPIO12 WIFI_O0 Relay 1 (0 = Off, 1 = On) controlling the light
     GPIO_LED1_INV,    // GPIO13 WIFI_CHK Blue Led on PCA (0 = On, 1 = Off)
     GPIO_KEY4,        // GPIO14 WIFI_KEY3 Virtual button 4 as feedback from RC
     GPIO_REL4,        // GPIO15 WIFI_O3 Relay 4 (0 = Off, 1 = On) controlling the fan
     0, 0
  },
  { "BlitzWolf SHP",   // BlitzWolf BW-SHP2 and BW-SHP6 (ESP8285 - BL0937 or HJL-01 Energy Monitoring)
                       // https://www.banggood.com/BlitzWolf-BW-SHP2-Smart-WIFI-Socket-EU-Plug-220V-16A-Work-with-Amazon-Alexa-Google-Assistant-p-1292899.html
                       // https://www.amazon.de/Steckdose-Homecube-intelligente-Verbrauchsanzeige-funktioniert/dp/B076Q2LKHG/ref=sr_1_fkmr0_1
                       // https://www.amazon.de/Intelligente-Stromverbrauch-Fernsteurung-Schaltbare-Energieklasse/dp/B076WZQS4S/ref=sr_1_1
                       // https://www.aliexpress.com/store/product/BlitzWolf-BW-SHP6-EU-Plug-Metering-Version-WIFI-Smart-Socket-220V-240V-10A-Work-with-Amazon/1965360_32945504669.html
     GPIO_LED2_INV,    // GPIO00 Red Led (1 = On, 0 = Off)
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
     GPIO_LED1_INV,    // GPIO02 Blue Led (1 = On, 0 = Off)
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     0,
     GPIO_HJL_CF,      // GPIO05 BL0937 or HJL-01 CF power
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_NRG_SEL_INV, // GPIO12 BL0937 or HJL-01 Sel output (0 = Voltage)
     GPIO_KEY1,        // GPIO13 Button
     GPIO_NRG_CF1,     // GPIO14 BL0937 or HJL-01 CF1 current / voltage
     GPIO_REL1,        // GPIO15 Relay (0 = Off, 1 = On)
     0, 0
  },
  { "Shelly 1",         // Shelly1 Open Source (ESP8266 - 2MB) - https://shelly.cloud/shelly1-open-source/
     0, 0, 0, 0,
     GPIO_REL1,         // GPIO04 Relay (0 = Off, 1 = On)
     GPIO_SWT1_NP,      // GPIO05 SW pin
     0, 0, 0, 0, 0, 0,  // Flash connection
     0, 0, 0, 0, 0, 0
  },
  { "Shelly 2",         // Shelly2 (ESP8266 - 2MB) - https://shelly.cloud/shelly2/
     0,
     GPIO_TXD,          // GPIO01 MCP39F501 Serial input
     0,
     GPIO_RXD,          // GPIO03 MCP39F501 Serial output
     GPIO_REL1,         // GPIO04
     GPIO_REL2,         // GPIO05
     0, 0, 0, 0, 0, 0,  // Flash connection
     GPIO_SWT1,         // GPIO12
     0,
     GPIO_SWT2,         // GPIO14
     0,                 // GPIO15 MCP39F501 Reset
     0, 0
  },
  { "Xiaomi Philips",  // Xiaomi Philips bulb (ESP8266)
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     GPIO_PWM2,        // GPIO12 cold/warm light
     0, 0,
     GPIO_PWM1,        // GPIO15 light intensity
     0, 0
  },
  { "Neo Coolcam",     // Neo Coolcam (ESP8266)
                       // https://www.banggood.com/NEO-COOLCAM-WiFi-Mini-Smart-Plug-APP-Remote-Control-Timing-Smart-Socket-EU-Plug-p-1288562.html?cur_warehouse=CN
     0, 0, 0, 0,
     GPIO_LED1_INV,    // GPIO04 Red Led (0 = On, 1 = Off)
     0,
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
     GPIO_KEY1,        // GPIO13 Button
     0, 0, 0, 0
  },
  { "ESP Switch",      // Michael Haustein 4 channel wall switch (ESP07 = ESP8266)
                       // Use rules for further actions like - rule on power1#state do publish cmnd/other_device/power %value% endon
     GPIO_KEY2,        // GPIO00 Button 2
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
     GPIO_REL3_INV,    // GPIO02 Yellow Led 3 (0 = On, 1 = Off)
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     GPIO_KEY1,        // GPIO04 Button 1
     GPIO_REL2_INV,    // GPIO05 Red Led 2 (0 = On, 1 = Off)
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_REL4_INV,    // GPIO12 Blue Led 4 (0 = On, 1 = Off)
     GPIO_KEY4,        // GPIO13 Button 4
     GPIO_KEY3,        // GPIO14 Button 3
     GPIO_LED1,        // GPIO15 Optional sensor
     GPIO_REL1_INV,    // GPIO16 Green Led 1 (0 = On, 1 = Off)
  },
  { "OBI Socket",      // OBI socket (ESP8266) - https://www.obi.de/hausfunksteuerung/wifi-stecker-schuko/p/2291706
     GPIO_USER,        // GPIO00
     GPIO_USER,        // GPIO01 Serial RXD
     0,
     GPIO_USER,        // GPIO03 Serial TXD
     GPIO_LED1,        // GPIO04 Blue LED
     GPIO_REL1,        // GPIO05 (Relay OFF, but used as Relay Switch)
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_LED2,        // GPIO12 (Relay ON, but set to LOW, so we can switch with GPIO05)
     GPIO_USER,        // GPIO13
     GPIO_KEY1,        // GPIO14 Button
     0,
     GPIO_USER,        // GPIO16
     GPIO_ADC0         // ADC0   A0 Analog input
  },
  { "Teckin",          // https://www.amazon.de/gp/product/B07D5V139R
     0,
     GPIO_KEY1,        // GPIO01 Serial TXD and Button
     0,
     GPIO_LED2_INV,    // GPIO03 Serial RXD and Red Led (0 = On, 1 = Off)
     GPIO_HJL_CF,      // GPIO04 BL0937 or HJL-01 CF power
     GPIO_NRG_CF1,     // GPIO05 BL0937 or HJL-01 CF1 current / voltage
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_NRG_SEL_INV, // GPIO12 BL0937 or HJL-01 Sel output (0 = Voltage)
     GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off)
     GPIO_REL1,        // GPIO14 Relay (0 = Off, 1 = On)
     0, 0, 0
  },
  { "AplicWDP303075",  // Aplic WDP 303075 (ESP8285 - HLW8012 Energy Monitoring)
                       // https://www.amazon.de/dp/B07CNWVNJ2
     0, 0, 0,
     GPIO_KEY1,        // GPIO03 Button
     GPIO_HLW_CF,      // GPIO04 HLW8012 CF power
     GPIO_NRG_CF1,     // GPIO05 HLW8012 CF1 current / voltage
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_NRG_SEL_INV, // GPIO12 HLW8012 CF Sel output (0 = Voltage)
     GPIO_LED1_INV,    // GPIO13 LED (0 = On, 1 = Off)
     GPIO_REL1,        // GPIO14 Relay SRU 5VDC SDA (0 = Off, 1 = On )
     0, 0, 0
  },
  { "Tuya Dimmer",     // Tuya Dimmer (ESP8266 w/ separate MCU dimmer)
                       // https://www.amazon.com/gp/product/B07CTNSZZ8/ref=oh_aui_detailpage_o00_s00?ie=UTF8&psc=1
     GPIO_USER,        // Virtual Button (controlled by MCU)
     GPIO_USER,        // GPIO01 MCU serial control
     GPIO_USER,
     GPIO_USER,        // GPIO03 MCU serial control
     GPIO_USER,
     GPIO_USER,
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_USER,
     GPIO_USER,
     GPIO_USER,        // GPIO14 Green Led
     GPIO_USER,
     GPIO_USER,
     0
  },
  { "Gosund SP1 v23",  // https://www.amazon.de/gp/product/B0777BWS1P
     0,
     GPIO_LED1_INV,    // GPIO01 Serial RXD and LED1 (blue) inv
     0,
     GPIO_KEY1,        // GPIO03 Serial TXD and Button
     GPIO_HJL_CF,      // GPIO04 BL0937 or HJL-01 CF power
     GPIO_NRG_CF1,     // GPIO05 BL0937 or HJL-01 CF1 current / voltage
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_NRG_SEL_INV, // GPIO12 BL0937 or HJL-01 Sel output (0 = Voltage)
     GPIO_LED2_INV,    // GPIO13 LED2 (red) inv
     GPIO_REL1,        // GPIO14 Relay (0 = Off, 1 = On)
     0, 0, 0
  },
  { "ARMTR Dimmer",     // ARMTRONIX Dimmer, one or two channel (ESP8266 w/ separate MCU dimmer)
                        // https://www.tindie.com/products/Armtronix/wifi-ac-dimmer-two-triac-board/
                        // https://www.tindie.com/products/Armtronix/wifi-ac-dimmer-esp8266-one-triac-board-alexaecho/
     GPIO_USER,
     GPIO_TXD,          // GPIO01 MCU serial control
     GPIO_USER,
     GPIO_RXD,          // GPIO03 MCU serial control
     GPIO_USER,
     GPIO_USER,
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_USER,
     GPIO_USER,
     GPIO_USER,
     GPIO_USER,
     GPIO_USER,
     0
  },
  { "SK03 Outdoor",    // Outdoor smart plug with power monitoring HLW8012 chip - https://www.amazon.com/gp/product/B07CG7MBPV
     GPIO_KEY1,        // GPIO00 Button
     0, 0, 0,
     GPIO_HLW_CF,      // GPIO04 HLW8012 CF power
     GPIO_NRG_CF1,     // GPIO05 HLW8012 CF1 current / voltage
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_NRG_SEL_INV, // GPIO12 HLW8012 CF Sel output (0 = Voltage)
     GPIO_LED2_INV,    // GPIO13 Red Led (0 = On, 1 = Off)
     GPIO_LED1_INV,    // GPIO14 Blue Led (0 = On, 1 = Off)
     GPIO_REL1,        // GPIO15 Relay (0 = Off, 1 = On)
     0, 0
  },
  { "PS-16-DZ",        // PS-16-DZ Dimmer (ESP8266 w/ separate Nuvoton MCU dimmer)
                       // https://www.aliexpress.com/item/SM-Smart-WIFI-Wall-Dimmer-Light-Switch-US-Ewelink-APP-Remote-Control-Wi-Fi-Wirele-Work/32871151902.html
     GPIO_USER,
     GPIO_TXD,         // GPIO01 MCU serial control
     GPIO_USER,
     GPIO_RXD,         // GPIO03 MCU serial control
     GPIO_USER,
     GPIO_USER,
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_USER,
     GPIO_LED1,        // GPIO13 WiFi LED
     GPIO_USER,
     GPIO_USER,
     GPIO_USER,
     0
  },
  { "Teckin US",       // Teckin SP20 US with Energy Monitoring
                       // https://www.amazon.com/Outlet-Compatible-Monitoring-Function-Required/dp/B079Q5W22B
                       // https://www.amazon.com/Outlet-ZOOZEE-Monitoring-Function-Compatible/dp/B07J2LR5KN
     GPIO_LED2_INV,    // GPIO00 Red Led (1 = On, 0 = Off)
     0,
     GPIO_LED1_INV,    // GPIO02 Blue Led (1 = On, 0 = Off)
     0,
     GPIO_REL1,        // GPIO04 Relay (0 = Off, 1 = On)
     GPIO_HJL_CF,      // GPIO05 BL0937 or HJL-01 CF power
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_NRG_SEL_INV, // GPIO12 BL0937 or HJL-01 Sel output (0 = Voltage)
     GPIO_KEY1,        // GPIO13 Button
     GPIO_NRG_CF1,     // GPIO14 BL0937 or HJL-01 CF1 current / voltage
     0, 0, 0
  },
  { "Manzoku strip",   // "MANZOKU" labeled power strip, EU version
                       // https://www.amazon.de/Steckdosenleiste-AOFO-Mehrfachsteckdose-Überspannungsschutz-Sprachsteuerung/dp/B07GBSD11P/
                       // https://www.amazon.de/Steckdosenleiste-Geekbes-USB-Anschluss-Kompatibel-gesteuert/dp/B078W23BW9/
     0,                // GPIO00
     0,                // GPIO01 Serial RXD
     0,
     GPIO_KEY1,        // GPIO03 Serial TXD + Button
     GPIO_REL2,        // GPIO04 Relay 2
     GPIO_REL1,        // GPIO05 Relay 1
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_REL3,        // GPIO12 Relay 3
     GPIO_REL4,        // GPIO13 Relay 4
     GPIO_USER,        // GPIO14
     0,
     GPIO_USER,        // GPIO16
     0
  }
};

/*
  Optionals

  { "MagicHome",       // Magic Home (aka Flux-light) (ESP8266)
                       // https://www.aliexpress.com/item/Magic-Home-Mini-RGB-RGBW-Wifi-Controller-For-Led-Strip-Panel-light-Timing-Function-16million-colors/32686853650.html
     0,
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
     GPIO_LED1_INV,    // GPIO02 Blue onboard LED
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     GPIO_USER,        // GPIO04 IR receiver (optional)
     GPIO_PWM2,        // GPIO05 RGB LED Green
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_PWM3,        // GPIO12 RGB LED Blue
     GPIO_USER,        // GPIO13 RGBW LED White (optional - set to PWM4 for Cold White or Warm White)
     GPIO_PWM1,        // GPIO14 RGB LED Red
     0, 0, 0
  },
  { "Arilux LC10",     // Arilux LC10 (ESP8285), RGBW + RF
                       // https://github.com/arendst/Sonoff-Tasmota/wiki/MagicHome-with-ESP8285
                       // https://www.aliexpress.com/item/DC5-24V-Wireless-WIFI-LED-RGB-Controller-RGBW-Controller-IR-RF-Remote-Control-IOS-Android-for/32827253255.html
                       // https://www.aliexpress.com/item/Wifi-LED-RGB-Controler-DC12V-MIni-Wifi-RGB-RGBW-LED-Controller-for-RGB-RGBW-LED-Strip/32673444047.html
     GPIO_USER,        // GPIO00 Optional Button
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
     0,
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor0
     GPIO_ARIRFRCV,    // GPIO04 RF receiver input
     GPIO_PWM2,        // GPIO05 RGB LED Green
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_PWM3,        // GPIO12 RGB LED Blue
     GPIO_PWM4,        // GPIO13 RGBW LED White
     GPIO_PWM1,        // GPIO14 RGB LED Red
     GPIO_LED2_INV,    // GPIO15 RF receiver control
     0, 0
  }

  { "Xenon 3CH",       // Xenon 3CH (ESP8266) - (#1128)
     0, 0, 0,
     GPIO_KEY2,        // GPIO03 Serial TXD and Optional sensor
     GPIO_REL2,        // GPIO04 Relay 2
     GPIO_KEY3,        // GPIO05 Input 2
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_KEY1,        // GPIO12 Key input
     GPIO_REL1,        // GPIO13 Relay 1
     0,
     GPIO_REL3,        // GPIO15 Relay 3
     0, 0
  }

  { "PowStro Basic",   // PowStro (ESP8266) - (#1419)
     0, 0, 0, 0,
     GPIO_REL1,        // GPIO04 Relay (0 = Off, 1 = On)
     0,
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_KEY1,        // GPIO12 Button
     0, 0,
     GPIO_LED1,        // GPIO15 Led (1 = On, 0 = Off)
     0, 0
  }

  { "SMPW701E",        // SM-PW701E WLAN Socket (#1190)
     0, 0, 0, 0,
     GPIO_LED1_INV,    // GPIO04 Blue Led (0 = On, 1 = Off)
     0,                // GPIO05 IR or RF receiver (optional)
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_REL1,        // GPIO12 Relay and Red Led (0 = Off, 1 = On)
     GPIO_KEY1,        // GPIO13 Button
     0, 0, 0, 0
  }

  { "SWA1",            // Smart Plugs (ESP8266)
     0,
     GPIO_USER,        // GPIO01
     0,
     GPIO_USER,        // GPIO03
     GPIO_LED1_INV,    // GPIO04 Blue LED
     GPIO_REL1,        // GPIO05 Red LED and relay
     0, 0, 0, 0, 0, 0, // Flash connection
     0,
     GPIO_KEY1,        // GPIO13 Button (normally GPIO00)
     GPIO_USER,        // GPIO14
     0, 0, 0
  }

  { "MagicHome v2.3",  // Magic Home (aka Flux-light) (ESP8266) (#1353)
     0, 0,
     GPIO_LED1_INV,    // GPIO02 Blue onboard LED
     0,
     GPIO_USER,        // GPIO04 IR receiver (optional)
     GPIO_PWM2,        // GPIO05 RGB LED Green
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_PWM1,        // GPIO12 RGB LED Red
     GPIO_PWM3,        // GPIO13 RGB LED Blue
     0,
     GPIO_PWM4,        // GPIO15 RGBW LED White
     0, 0
  }

  { "Ledunia",         // Ledunia (ESP8266 - 32MB) - http://ledunia.de/
     GPIO_USER,        // GPIO00 (D0)
     GPIO_USER,        // GPIO01 (D7) Serial RXD
     GPIO_USER,        // GPIO02 (D2)
     GPIO_USER,        // GPIO03 (D8) Serial TXD
     GPIO_USER,        // GPIO04 (D4) 4 x WS2812 Leds, (DOUT) Extents WS2812 string
     GPIO_USER,        // GPIO05 (D5) Blue Led
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_USER,        // GPIO12 (D12)
     GPIO_USER,        // GPIO13 (D13)
     GPIO_USER,        // GPIO14 (D14)
     GPIO_USER,        // GPIO15 (D15)
     GPIO_USER,        // GPIO16 (D16)
     0                 // ADC0 Analog input (A0)
  }

  { "Delock 11826",    // Delock 11826 (ESP8285) = Sonoff Basic
     GPIO_KEY1,        // GPIO00 Button
     0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off)
     0, 0, 0, 0
  }

*/

#endif  // _SONOFF_TEMPLATE_H_
