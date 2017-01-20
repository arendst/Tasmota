/*********************************************************************************************\
 * Template parameters
\*********************************************************************************************/

#define GPIO_SENSOR_START  0
#define GPIO_NONE          0    // Not used
#define GPIO_DHT11         1    // DHT11
#define GPIO_DHT21         2    // DHT21
#define GPIO_AM2301        2    // AM2301
#define GPIO_DHT22         3    // DHT22
#define GPIO_AM2302        3    // AM2302
#define GPIO_AM2321        3    // AM2321
#define GPIO_DSB           4    // Single wire DS18B20 or DS18S20
#define GPIO_I2C_SCL       5    // I2C SCL
#define GPIO_I2C_SDA       6    // I2C SDA
#define GPIO_WS2812        7    // WS2812 Led string
#define GPIO_SWT1          8    // User connected external switches
#define GPIO_SENSOR_END    9

#define GPIO_SWT2          9
#define GPIO_SWT3          10
#define GPIO_SWT4          11
#define GPIO_KEY1          12   // Button usually connected to GPIO0
#define GPIO_KEY2          13
#define GPIO_KEY3          14
#define GPIO_KEY4          15
#define GPIO_REL1          16   // Relays
#define GPIO_REL2          17
#define GPIO_REL3          18
#define GPIO_REL4          19
#define GPIO_REL1_INV      20
#define GPIO_REL2_INV      21
#define GPIO_REL3_INV      22
#define GPIO_REL4_INV      23
#define GPIO_LED1          24   // Leds
#define GPIO_LED2          25
#define GPIO_LED3          26
#define GPIO_LED4          27
#define GPIO_LED1_INV      28
#define GPIO_LED2_INV      29
#define GPIO_LED3_INV      30
#define GPIO_LED4_INV      31
#define GPIO_PWM0          32   // Cold
#define GPIO_PWM1          33   // Warm
#define GPIO_PWM2          34   // Red (swapped with Blue from original)
#define GPIO_PWM3          35   // Green
#define GPIO_PWM4          36   // Blue (swapped with Red from original)
#define GPIO_RXD           37   // Serial interface
#define GPIO_TXD           38   // Serial interface
#define GPIO_HLW_SEL       39   // HLW8012 Sel output (Sonoff Pow)
#define GPIO_HLW_CF1       40   // HLW8012 CF1 voltage / current (Sonoff Pow)
#define GPIO_HLW_CF        41   // HLW8012 CF power (Sonoff Pow)
#define GPIO_USER          42   // User configurable
#define GPIO_MAX           43

/********************************************************************************************/

enum module_t {SONOFF_BASIC, SONOFF_RF, SONOFF_SV, SONOFF_TH, SONOFF_DUAL, SONOFF_POW, SONOFF_4CH, S20, SLAMPHER, SONOFF_TOUCH, SONOFF_LED, CH1, CH4, MOTOR, ELECTRODRAGON, USER_TEST, MAXMODULE};

/********************************************************************************************/

#define MAX_GPIO_PIN       17   // Number of supported GPIO

#define DHT11              11
#define DHT21              21
#define AM2301             21
#define DHT22              22
#define AM2302             22
#define AM2321             22

typedef struct MYIO {
  uint8_t      io[MAX_GPIO_PIN];
} myio;

typedef struct MYTMPLT {
  char         name[16];
  myio         gp;  
} mytmplt;

const char sensors[GPIO_SENSOR_END][8] PROGMEM =
  { "None", "DHT11", "AM2301", "DHT22", "DS18x20", "I2C SCL", "I2C SDA", "WS2812", "Switch" };

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
     0, 0, 0, 0, 0, 0, 0,
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
     0, 0, 0, 0, 0, 0, 0, 0, 0,
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
     0,
     GPIO_REL3,        // GPIO04 Sonoff 4CH Red Led and Relay 3 (0 = Off, 1 = On)
     GPIO_REL2,        // GPIO05 Sonoff 4CH Red Led and Relay 2 (0 = Off, 1 = On)
     0,
     GPIO_USER,        // GPIO07 Optional sensor
     GPIO_USER,        // GPIO08 Optional sensor
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
     GPIO_PWM3,        // GPIO04 Green light
     GPIO_PWM2,        // GPIO05 Red light
     0, 0, 0, 0, 0, 0,
     GPIO_PWM0,        // GPIO12 Cold light
     GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off)
     GPIO_PWM1,        // GPIO14 Warm light
     GPIO_PWM4,        // GPIO15 Blue light
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
  { "User Test",       // Sonoff Basic User Test
     GPIO_KEY1,        // GPIO00 Button
     0, 0,
     GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
     GPIO_USER,        // GPIO04 Optional sensor
     0, 0, 0, 0, 0, 0, 0,
     GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
     GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off)
     GPIO_USER,        // GPIO14 Optional sensor
     0, 0
  }
};

