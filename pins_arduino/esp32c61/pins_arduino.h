#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>
#include "soc/soc_caps.h"

#define PIN_RGB_LED 8
// BUILTIN_LED can be used in new Arduino API digitalWrite() like in Blink.ino
static const uint8_t LED_BUILTIN = SOC_GPIO_PIN_COUNT + PIN_RGB_LED;
#define BUILTIN_LED LED_BUILTIN  // backward compatibility
#define LED_BUILTIN LED_BUILTIN  // allow testing #ifdef LED_BUILTIN
// RGB_BUILTIN and RGB_BRIGHTNESS can be used in new Arduino API rgbLedWrite()
#define RGB_BUILTIN    LED_BUILTIN
#define RGB_BRIGHTNESS 64

static const uint8_t TX = 11;
static const uint8_t RX = 10;

static const uint8_t SDA = 23;
static const uint8_t SCL = 22;

static const uint8_t SS = 25;
static const uint8_t MOSI = 26;
static const uint8_t MISO = 27;
static const uint8_t SCK = 28;

static const uint8_t A0 = 0;
static const uint8_t A1 = 1;
static const uint8_t A2 = 2;
static const uint8_t A3 = 3;

// LP I2C Pins are fixed on ESP32-C61
#define WIRE1_PIN_DEFINED
static const uint8_t SDA1 = 6;
static const uint8_t SCL1 = 7;

#endif /* Pins_Arduino_h */
