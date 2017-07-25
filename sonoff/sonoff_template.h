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
  "None",
  "DHT11",
  "AM2301",
  "DHT22",
  "DS18x20",
  "I2C SCL",
  "I2C SDA",
  "WS2812",
  "IRremote",
  "Switch1",
  "Switch2",
  "Switch3",
  "Switch4",
  "Button1",
  "Button2",
  "Button3",
  "Button4",
  "Relay1",
  "Relay2",
  "Relay3",
  "Relay4",
  "Relay1I",
  "Relay2I",
  "Relay3I",
  "Relay4I",
  "Led1",
  "Led2",
  "Led3",
  "Led4",
  "Led1I",
  "Led2I",
  "Led3I",
  "Led4I",
  "PWM1",
  "PWM2",
  "PWM3",
  "PWM4",
  "PWM5",
  "Counter1",
  "Counter2",
  "Counter3",
  "Counter4"
  };
  
// Programmer selectable GPIO functionality offset by user selectable GPIOs
enum fpins_t {
  GPIO_RXD = GPIO_SENSOR_END,  // Serial interface
  GPIO_TXD,            // Serial interface
  GPIO_HLW_SEL,        // HLW8012 Sel output (Sonoff Pow)
  GPIO_HLW_CF1,        // HLW8012 CF1 voltage / current (Sonoff Pow)
  GPIO_HLW_CF,         // HLW8012 CF power (Sonoff Pow)
  GPIO_ADC0,           // ADC
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
     0, 0, 0, 0
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
  }
};

