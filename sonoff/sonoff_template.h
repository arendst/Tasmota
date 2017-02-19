/*********************************************************************************************\
 * Template parameters
\*********************************************************************************************/

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
  "Switch" };

// Programmer selectable GPIO functionality offset by user selectable GPIOs
enum fpins_t {
  GPIO_SWT2 = GPIO_SENSOR_END,
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
  GPIO_PWM0,           // Cold
  GPIO_PWM1,           // Warm
  GPIO_PWM2,           // Red (swapped with Blue from original)
  GPIO_PWM3,           // Green
  GPIO_PWM4,           // Blue (swapped with Red from original)
  GPIO_RXD,            // Serial interface
  GPIO_TXD,            // Serial interface
  GPIO_HLW_SEL,        // HLW8012 Sel output (Sonoff Pow)
  GPIO_HLW_CF1,        // HLW8012 CF1 voltage / current (Sonoff Pow)
  GPIO_HLW_CF,         // HLW8012 CF power (Sonoff Pow)
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
  USER_TEST,
  MAXMODULE };

/********************************************************************************************/

#define MAX_GPIO_PIN       17   // Number of supported GPIO

typedef struct MYIO {
  uint8_t      io[MAX_GPIO_PIN];
} myio;

typedef struct MYTMPLT {
  char         name[16];
  myio         gp;  
} mytmplt;

// Default module settings
const mytmplt modules[MAXMODULE] PROGMEM = {
  { "Sonoff Basic",    // Sonoff Basic
     GPIO_KEY1,        // GPIO00 Button
     0, 0,
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     GPIO_USER,        // GPIO04 Optional sensor
     0, 0, 0, 0, 0, 0, 0,
     GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off)
     GPIO_USER,        // GPIO14 Optional sensor
     0, 0
  },
  { "Sonoff RF",       // Sonoff RF
     GPIO_KEY1,        // GPIO00 Button
     0, 0,
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     GPIO_USER,        // GPIO04 Optional sensor
     0, 0, 0, 0, 0, 0, 0,
     GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off)
     GPIO_USER,        // GPIO14 Optional sensor
     0, 0
  },
  { "Sonoff SV",       // Sonoff SV
     GPIO_KEY1,        // GPIO00 Button
     0, 0,
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     GPIO_USER,        // GPIO04 Optional sensor
     GPIO_USER,        // GPIO05 Optional sensor
     0, 0, 0, 0, 0, 0,
     GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off)
     GPIO_USER,        // GPIO14 Optional sensor
     0, 0
  },
  { "Sonoff TH",       // Sonoff TH10/16
     GPIO_KEY1,        // GPIO00 Button
     0, 0,
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     GPIO_USER,        // GPIO04 Optional sensor
     0, 0, 0, 0, 0, 0, 0,
     GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off)
     GPIO_USER,        // GPIO14 Optional sensor
     0, 0
  },
  { "Sonoff Dual",     // Sonoff Dual
     0,
     GPIO_TXD,         // GPIO01 Relay control
     0,
     GPIO_RXD,         // GPIO03 Relay control
     GPIO_USER,        // GPIO04 Optional sensor
     0, 0, 0, 0, 0, 0, 0, 0,
     GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off)
     0, 0, 0
  },
  { "Sonoff Pow",      // Sonoff Pow
     GPIO_KEY1,        // GPIO00 Button
     0, 0, 0, 0,
     GPIO_HLW_SEL,     // GPIO05 HLW8012 Sel output
     0, 0, 0, 0, 0, 0,
     GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
     GPIO_HLW_CF1,     // GPIO13 HLW8012 CF1 voltage / current
     GPIO_HLW_CF,      // GPIO14 HLW8012 CF power
     GPIO_LED1,        // GPIO15 Green Led (0 = On, 1 = Off)
     0
  },
  { "Sonoff 4CH",      // Sonoff 4CH
     GPIO_KEY1,        // GPIO00 Button 1
     0,
     GPIO_USER,        // GPIO02 Optional sensor
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     GPIO_REL3,        // GPIO04 Sonoff 4CH Red Led and Relay 3 (0 = Off, 1 = On)
     GPIO_REL2,        // GPIO05 Sonoff 4CH Red Led and Relay 2 (0 = Off, 1 = On)
     0, 0, 0,
     GPIO_KEY2,        // GPIO09 Button 2
     GPIO_KEY3,        // GPIO10 Button 3
     0,
     GPIO_REL1,        // GPIO12 Red Led and Relay 1 (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off)
     GPIO_KEY4,        // GPIO14 Button 4
     GPIO_REL4,        // GPIO15 Red Led and Relay 4 (0 = Off, 1 = On)
     0
  },
  { "S20 Socket",      // S20 Smart Socket
     GPIO_KEY1,        // GPIO00 Button
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off)
     0, 0, 0
  },
  { "Slampher",        // Slampher
     GPIO_KEY1,        // GPIO00 Button
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off)
     0, 0, 0
  },
  { "Sonoff Touch",    // Sonoff Touch
     GPIO_KEY1,        // GPIO00 Button
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off)
     0, 0, 0
  },
  { "Sonoff LED",      // Sonoff LED
     GPIO_KEY1,        // GPIO00 Button
     0, 0, 0,
     GPIO_USER,        // GPIO04 Optional sensor (PWM3 Green)
     GPIO_USER,        // GPIO05 Optional sensor (PWM2 Red)
     0, 0, 0, 0, 0, 0,
     GPIO_PWM0,        // GPIO12 Cold light
     GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off)
     GPIO_PWM1,        // GPIO14 Warm light
     GPIO_USER,        // GPIO15 Optional sensor (PWM4 Blue)
     0
  },
  { "1 Channel",       // 1 Channel Inching/Latching Relay
     GPIO_KEY1,        // GPIO00 Button
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off)
     0, 0, 0
  },
  { "4 Channel",       // 4 Channel Inching/Latching Relays
     0,
     GPIO_TXD,         // GPIO01 Relay control
     0,
     GPIO_RXD,         // GPIO03 Relay control
     0, 0, 0, 0, 0, 0, 0, 0, 0,
     GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off)
     0, 0, 0
  },
  { "Motor C/AC",      // Motor Clockwise / Anti clockwise
     GPIO_KEY1,        // GPIO00 Button
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off)
     0, 0, 0
  },
  { "ElectroDragon",   // ElectroDragon IoT Relay Board
     GPIO_KEY2,        // GPIO00 Button 2
     0,
     GPIO_KEY1,        // GPIO02 Button 1
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     GPIO_USER,        // GPIO04 Optional sensor
     0, 0, 0, 0, 0, 0, 0,
     GPIO_REL2,        // GPIO12 Red Led and Relay 2 (0 = Off, 1 = On)
     GPIO_REL1,        // GPIO13 Red Led and Relay 1 (0 = Off, 1 = On)
     GPIO_USER,        // GPIO14 Optional sensor
     0,
     GPIO_LED1         // GPIO16 Green/Blue Led (1 = On, 0 = Off)
  },
  { "EXS Relay",       // Latching relay https://ex-store.de/ESP8266-WiFi-Relay-V31
                       //   Module Pin 1 VCC 3V3, Module Pin 6 GND
     GPIO_KEY1,        // GPIO00 Module Pin 8 - Button (firmware flash)
     0,                // GPIO01 Module Pin 2 = UART0_TXD
     GPIO_USER,        // GPIO02 Module Pin 7
     GPIO_USER,        // GPIO03 Module Pin 3 = UART0_RXD
     GPIO_USER,        // GPIO04 Module Pin 10
     GPIO_USER,        // GPIO05 Module Pin 9
     0, 0, 0, 0, 0, 0,
     GPIO_REL1,        // GPIO12 Relay1 ( 1 = Off)
     GPIO_REL2,        // GPIO13 Relay1 ( 1 = On)
     GPIO_USER,        // GPIO14 Module Pin 5
     0,
     GPIO_USER         // GPIO16 Module Pin 4
  },
  { "User Test",       // Sonoff Basic User Test
     GPIO_KEY1,        // GPIO00 Button
     0,
     GPIO_USER,        // GPIO02 Optional sensor
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     GPIO_USER,        // GPIO04 Optional sensor
     GPIO_USER,        // GPIO05 Optional sensor
     0, 0, 0, 0, 0, 0,
     GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off)
     GPIO_USER,        // GPIO14 Optional sensor
     0, 0
  }
};

