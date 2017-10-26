/*
  sonoff_template.h - template settings for Sonoff-Tasmota

  Copyright (C) 2017  Theo Arends

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

// User selectable GPIO functionality
enum upins_t {
  GPIO_NONE,           // Not used
  GPIO_DHT11,          // DHT11
  GPIO_DHT21,          // DHT21, AM2301
  GPIO_DHT22,          // DHT22, AM2302, AM2321
  GPIO_DSB,            // Single wire DS18B20 or DS18S20
  GPIO_I2C_SCL,        // I2C SCL
  GPIO_I2C_SDA,        // I2C SDA
  GPIO_WS2812,         // WS2812 Led string
  GPIO_IRSEND,         // IR remote
  GPIO_SWT1,           // User connected external switches
  GPIO_SWT2,
  GPIO_SWT3,
  GPIO_SWT4,
  GPIO_KEY1,           // Button usually connected to GPIO0
  GPIO_KEY2,
  GPIO_KEY3,
  GPIO_KEY4,
  GPIO_REL1,           // Relays
  GPIO_REL2,
  GPIO_REL3,
  GPIO_REL4,
  GPIO_REL1_INV,
  GPIO_REL2_INV,
  GPIO_REL3_INV,
  GPIO_REL4_INV,
  GPIO_LED1,           // Leds
  GPIO_LED2,
  GPIO_LED3,
  GPIO_LED4,
  GPIO_LED1_INV,
  GPIO_LED2_INV,
  GPIO_LED3_INV,
  GPIO_LED4_INV,
  GPIO_PWM1,           // Sonoff Led Cold
  GPIO_PWM2,           // Sonoff Led Warm
  GPIO_PWM3,           // Red (swapped with Blue from original)
  GPIO_PWM4,           // Green
  GPIO_PWM5,           // Blue (swapped with Red from original)
  GPIO_CNTR1,
  GPIO_CNTR2,
  GPIO_CNTR3,
  GPIO_CNTR4,
  GPIO_SENSOR_END };

// Text in webpage Module Parameters and commands GPIOS and GPIO
const char sensors[GPIO_SENSOR_END][9] PROGMEM = {
  D_SENSOR_NONE,
  D_SENSOR_DHT11,
  D_SENSOR_AM2301,
  D_SENSOR_DHT22,
  D_SENSOR_DS18X20,
  D_SENSOR_I2C_SCL,
  D_SENSOR_I2C_SDA,
  D_SENSOR_WS2812,
  D_SENSOR_IRREMOTE,
  D_SENSOR_SWITCH "1",
  D_SENSOR_SWITCH "2",
  D_SENSOR_SWITCH "3",
  D_SENSOR_SWITCH "4",
  D_SENSOR_BUTTON "1",
  D_SENSOR_BUTTON "2",
  D_SENSOR_BUTTON "3",
  D_SENSOR_BUTTON "4",
  D_SENSOR_RELAY "1",
  D_SENSOR_RELAY "2",
  D_SENSOR_RELAY "3",
  D_SENSOR_RELAY "4",
  D_SENSOR_RELAY "1I",
  D_SENSOR_RELAY "2I",
  D_SENSOR_RELAY "3I",
  D_SENSOR_RELAY "4I",
  D_SENSOR_LED "1",
  D_SENSOR_LED "2",
  D_SENSOR_LED "3",
  D_SENSOR_LED "4",
  D_SENSOR_LED "1I",
  D_SENSOR_LED "2I",
  D_SENSOR_LED "3I",
  D_SENSOR_LED "4I",
  D_SENSOR_PWM "1",
  D_SENSOR_PWM "2",
  D_SENSOR_PWM "3",
  D_SENSOR_PWM "4",
  D_SENSOR_PWM "5",
  D_SENSOR_COUNTER "1",
  D_SENSOR_COUNTER "2",
  D_SENSOR_COUNTER "3",
  D_SENSOR_COUNTER "4"
  };

// Programmer selectable GPIO functionality offset by user selectable GPIOs
enum fpins_t {
  GPIO_RXD = GPIO_SENSOR_END,  // Serial interface
  GPIO_TXD,            // Serial interface
  GPIO_HLW_SEL,        // HLW8012 Sel output (Sonoff Pow)
  GPIO_HLW_CF1,        // HLW8012 CF1 voltage / current (Sonoff Pow)
  GPIO_HLW_CF,         // HLW8012 CF power (Sonoff Pow)
  GPIO_ADC0,           // ADC
  GPIO_DI,             // my92x1 PWM input
  GPIO_DCKI,           // my92x1 CLK input
  GPIO_USER,           // User configurable
  GPIO_MAX };

/********************************************************************************************/

// Supported hardware modules
enum module_t {
  SONOFF_BASIC,
  SONOFF_RF,
  SONOFF_SV,
  SONOFF_TH,
  SONOFF_DUAL,
  SONOFF_POW,
  SONOFF_4CH,
  S20,
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
  MAXMODULE };

/********************************************************************************************/

#define MAX_GPIO_PIN       18   // Number of supported GPIO

typedef struct MYIO {
  uint8_t      io[MAX_GPIO_PIN];
} myio;

typedef struct MYTMPLT {
  char         name[15];
  myio         gp;
} mytmplt;

const uint8_t nicelist[MAXMODULE] PROGMEM = {
  SONOFF_BASIC,
  SONOFF_RF,
  SONOFF_TH,
  SONOFF_DUAL,
  SONOFF_POW,
  SONOFF_4CH,
  SONOFF_4CHPRO,
  SONOFF_SV,
  SONOFF_DEV,
  S20,
  SLAMPHER,
  SONOFF_TOUCH,
  SONOFF_T11,
  SONOFF_T12,
  SONOFF_T13,
  SONOFF_SC,
  SONOFF_B1,
  SONOFF_LED,
  SONOFF_BN,
  SONOFF_BRIDGE,
  CH1,
  CH4,
  MOTOR,
  ELECTRODRAGON,
  EXS_RELAY,
  SUPLA1,
  YUNSHAN,
  WION,
  H801,
  HUAFAN_SS,
  AILIGHT,
  WEMOS,
  WITTY
};

// Default module settings
const mytmplt modules[MAXMODULE] PROGMEM = {
  { "Sonoff Basic",    // Sonoff Basic (ESP8266)
     GPIO_KEY1,        // GPIO00 Button
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
     0,                // GPIO02
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
  { "Sonoff Pow",      // Sonoff Pow (ESP8266)
     GPIO_KEY1,        // GPIO00 Button
     0, 0, 0, 0,
     GPIO_HLW_SEL,     // GPIO05 HLW8012 Sel output
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
     GPIO_HLW_CF1,     // GPIO13 HLW8012 CF1 voltage / current
     GPIO_HLW_CF,      // GPIO14 HLW8012 CF power
     GPIO_LED1,        // GPIO15 Green Led (0 = On, 1 = Off)
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
  { "S20 Socket",      // S20 Smart Socket (ESP8266)
     GPIO_KEY1,        // GPIO00 Button
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
     0,
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     0, 0,
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off)
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
  { "EXS Relay",       // Latching relay https://ex-store.de/ESP8266-WiFi-Relay-V31 (ESP8266)
                       //   Module Pin 1 VCC 3V3, Module Pin 6 GND
     GPIO_KEY1,        // GPIO00 Module Pin 8 - Button (firmware flash)
     GPIO_USER,        // GPIO01 Module Pin 2 = UART0_TXD
     GPIO_USER,        // GPIO02 Module Pin 7
     GPIO_USER,        // GPIO03 Module Pin 3 = UART0_RXD
     GPIO_USER,        // GPIO04 Module Pin 10
     GPIO_USER,        // GPIO05 Module Pin 9
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_REL1,        // GPIO12 Relay1 ( 1 = Off)
     GPIO_REL2,        // GPIO13 Relay1 ( 1 = On)
     GPIO_USER,        // GPIO14 Module Pin 5
     0,
     GPIO_USER,        // GPIO16 Module Pin 4
     0
  },
  { "WiOn",            // Indoor Tap https://www.amazon.com/gp/product/B00ZYLUBJU/ref=s9_acsd_al_bw_c_x_3_w (ESP8266)
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
  { "WeMos D1 mini",   // WeMos and NodeMCU hardware (ESP8266)
     GPIO_USER,        // GPIO00 D3 Wemos Button Shield
     GPIO_USER,        // GPIO01 TX Serial RXD
     GPIO_USER,        // GPIO02 D4 Wemos DHT Shield
     GPIO_USER,        // GPIO03 RX Serial TXD and Optional sensor
     GPIO_USER,        // GPIO04 D2 Wemos I2C SDA
     GPIO_USER,        // GPIO05 D1 Wemos I2C SCL / Wemos Relay Shield (0 = Off, 1 = On) / Wemos WS2812B RGB led Shield
     0, 0, 0, 0, 0, 0, // Flash connection
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
     GPIO_LED1_INV,    // GPIO13 BLUE LED
     GPIO_USER,        // GPIO14 Optional sensor
     0,                // GPIO15
     0,                // GPIO16
     GPIO_ADC0         // ADC0 A0 Analog input
  },
  { "H801",            // Lixada H801 Wifi (ESP8266)
     GPIO_KEY1,        // GPIO00 E-FW Button
     GPIO_LED1,        // GPIO01 Green LED
     GPIO_TXD,         // GPIO02 RX - Pin next to TX on the PCB
     GPIO_RXD,         // GPIO03 TX - Pin next to GND on the PCB
     GPIO_PWM2,        // GPIO04 W2
     GPIO_LED2_INV,    // GPIO05 Red LED
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_PWM3,        // GPIO12 Blue
     GPIO_PWM4,        // GPIO13 Green
     GPIO_PWM1,        // GPIO14 W1
     GPIO_PWM5,        // GPIO15 Red
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
     GPIO_HLW_CF1,     // GPIO12 HLW8012 CF1 voltage / current
     GPIO_HLW_SEL,     // GPIO13 HLW8012 Sel output
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
     0,
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
     0,
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
     0,
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
  { "Supla Espablo",   // Supla Espablo (ESP8266) - http://www.wykop.pl/ramka/3325399/diy-supla-do-puszki-instalacyjnej-podtynkowej-supla-org/
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
  { "Witty Cloud",     // Witty Cloud Dev Board (ESP8266) - https://www.aliexpress.com/item/ESP8266-serial-WIFI-Witty-cloud-Development-Board-ESP-12F-module-MINI-nodemcu/32643464555.html
     GPIO_USER,        // GPIO00 D3 flash push button on interface board
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
     GPIO_LED1_INV,    // GPIO02 D4 Blue Led (0 = On, 1 = Off) on ESP-12F
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     GPIO_KEY1,        // GPIO04 D2 push button on ESP-12F board
     GPIO_USER,        // GPIO05 D1 optional sensor
     0, 0, 0, 0, 0, 0, // Flash connection
     GPIO_PWM4,        // GPIO12 D6 RGB LED Green
     GPIO_PWM5,        // GPIO13 D7 RGB LED Blue
     GPIO_USER,        // GPIO14 D5 optional sensor
     GPIO_PWM3,        // GPIO15 D8 RGB LED Red
     GPIO_USER,        // GPIO16 D0 optional sensor
     GPIO_ADC0         // ADC0 A0 Light sensor / Requires USE_ADC_VCC in user_config.h to be disabled
  },
  { "Yunshan Relay",   // Yunshan Wifi Relay (ESP8266) - https://www.ebay.com/p/Esp8266-220v-10a-Network-Relay-WiFi-Module/1369583381
                       // Schematics and Info https://ucexperiment.wordpress.com/2016/12/18/yunshan-esp8266-250v-15a-acdc-network-wifi-relay-module/
     0,                // GPIO00 Flash jumper - Module Pin 8
     GPIO_USER,        // GPIO01 Serial RXD and Optional sensor - Module Pin 2
     GPIO_LED1_INV,    // GPIO02 Blue Led (0 = On, 1 = Off) on ESP-12F - Module Pin 7
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor - Module Pin 3
     GPIO_REL1,        // GPIO04 Red Led and Relay (0 = Off, 1 = On) - Module Pin 10
     GPIO_KEY1,        // GPIO05 Blue Led and OptoCoupler input - Module Pin 9
     0, 0, 0, 0, 0, 0, // Flash connection
     0, 0, 0, 0, 0
  }
};

