#ifndef _USER_CONFIG_OVERRIDE_H_
#define _USER_CONFIG_OVERRIDE_H_

// -- Ethernet (LAN8720 auf dem KC868-A16) --------
#define USE_ETHERNET
  #define ETH_TYPE      0   // ETH_PHY_LAN8720
  #define ETH_ADDR      0   // PHY-Adresse
  #define ETH_CLKMODE   3   // ETH_CLOCK_GPIO17_OUT

// -- I2C (Pflicht für PCF8574) -------------------
#ifndef USE_I2C
#define USE_I2C
#endif

// -- PCF8574 (16 Ein- und Ausgänge) --------------
#define USE_PCF8574
  #define USE_PCF8574_SENSOR        // Status im SENSOR-Message
  #define USE_PCF8574_DISPLAYINPUT  // Eingänge im Webinterface anzeigen
  #define USE_PCF8574_MQTTINPUT     // MQTT bei Eingangsänderung

#endif  // _USER_CONFIG_OVERRIDE_H_
