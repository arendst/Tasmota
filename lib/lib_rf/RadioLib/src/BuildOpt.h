#if !defined(_RADIOLIB_BUILD_OPTIONS_H)
#define _RADIOLIB_BUILD_OPTIONS_H

/* RadioLib build configuration options */

/*
 * Debug output enable.
 * Warning: Debug output will slow down the whole system significantly.
 *          Also, it will result in larger compiled binary.
 * Levels: basic - only main info
 *         protocol - mainly LoRaWAN stuff, but other protocols as well
 *         SPI - full transcript of all SPI communication
 */
#if !defined(RADIOLIB_DEBUG_BASIC)
  #define RADIOLIB_DEBUG_BASIC (0)
#endif
#if !defined(RADIOLIB_DEBUG_PROTOCOL)
  #define RADIOLIB_DEBUG_PROTOCOL (0)
#endif
#if !defined(RADIOLIB_DEBUG_SPI)
  #define RADIOLIB_DEBUG_SPI (0)
#endif

// set which output port should be used for debug output
// may be Serial port (on Arduino) or file like stdout or stderr (on generic platforms)
#if !defined(RADIOLIB_DEBUG_PORT)
  #define RADIOLIB_DEBUG_PORT   Serial
#endif

/*
 * Comment to disable "paranoid" SPI mode, or set RADIOLIB_SPI_PARANOID to 0
 * Every write to an SPI register using SPI set function will be verified by a subsequent read operation.
 * This improves reliability, but slightly slows down communication.
 * Note: Enabled by default.
 */
#if !defined(RADIOLIB_SPI_PARANOID)
  #define RADIOLIB_SPI_PARANOID (1)
#endif

/*
 * Comment to disable parameter range checking
 * RadioLib will check provided parameters (such as frequency) against limits determined by the device manufacturer.
 * It is highly advised to keep this macro defined, removing it will allow invalid values to be set,
 * possibly leading to bricked module and/or program crashing.
 * Note: Enabled by default.
 */
#if !defined(RADIOLIB_CHECK_PARAMS)
  #define RADIOLIB_CHECK_PARAMS (1)
#endif

/*
 * SX127x errata fix enable
 * Warning: SX127x errata fix has been reported to cause issues with LoRa bandwidths lower than 62.5 kHz.
 *          It should only be enabled if you really are observing some errata-related issue.
 * Note: Disabled by default.
 */
#if !defined(RADIOLIB_FIX_ERRATA_SX127X)
  #define RADIOLIB_FIX_ERRATA_SX127X (0)
#endif

/*
 * God mode enable - all methods and member variables in all classes will be made public, thus making them accessible from Arduino code.
 * Warning: Come on, it's called GOD mode - obviously only use this if you know what you're doing.
 *          Failure to heed the above warning may result in bricked module.
 */
#if !defined(RADIOLIB_GODMODE)
  #define RADIOLIB_GODMODE (0)
#endif

/*
 * Low-level hardware access enable
 * This will make some hardware methods like SPI get/set accessible from the user sketch - think of it as "god mode lite"
 * Warning: RadioLib won't stop you from writing invalid stuff into your device, so it's quite easy to brick your module with this.
 */
#if !defined(RADIOLIB_LOW_LEVEL)
  #define RADIOLIB_LOW_LEVEL (0)
#endif

/*
 * Enable pre-defined modules when using RadioShield, disabled by default.
 */
#if !defined(RADIOLIB_RADIOSHIELD)
  #define RADIOLIB_RADIOSHIELD  (0)
#endif

/*
 * Enable interrupt-based timing control
 * For details, see https://github.com/jgromes/RadioLib/wiki/Interrupt-Based-Timing
 */
#if !defined(RADIOLIB_INTERRUPT_TIMING)
  #define RADIOLIB_INTERRUPT_TIMING  (0)
#endif

/*
 * Enable static-only memory management: no dynamic allocation will be performed.
 * Warning: Large static arrays will be created in some methods. It is not advised to send large packets in this mode.
 */
#if !defined(RADIOLIB_STATIC_ONLY)
  #define RADIOLIB_STATIC_ONLY  (0)
#endif

// set the size of static arrays to use
#if !defined(RADIOLIB_STATIC_ARRAY_SIZE)
  #define RADIOLIB_STATIC_ARRAY_SIZE   (256)
#endif

/*
 * Uncomment on boards whose clock runs too slow or too fast
 * Set the value according to the following scheme:
 * Enable timestamps on your terminal
 * Print something to terminal, wait 1000 milliseconds, print something again
 * If the difference is e.g. 1014 milliseconds between the prints, set this value to 14
 * Or, for more accuracy, wait for 100,000 milliseconds and divide the total drift by 100
 */
#if !defined(RADIOLIB_CLOCK_DRIFT_MS)
  //#define RADIOLIB_CLOCK_DRIFT_MS                         (0)
#endif

#if ARDUINO >= 100
  // Arduino build
  #include "Arduino.h"
  #define RADIOLIB_BUILD_ARDUINO
#else
  // generic build
  #include <stdio.h>
  #define RADIOLIB_BUILD_GENERIC
#endif

#if defined(RADIOLIB_BUILD_ARDUINO)
/*
 * Platform-specific configuration.
 *
 * RADIOLIB_PLATFORM - platform name, used in debugging to quickly check the correct platform is detected.
 * RADIOLIB_NC - alias for unused pin, usually the largest possible value of uint8_t.
 * RADIOLIB_DEFAULT_SPI - default SPIClass instance to use.
 * RADIOLIB_NONVOLATILE - macro to place variable into program storage (usually Flash).
 * RADIOLIB_NONVOLATILE_READ_BYTE - function/macro to read variables saved in program storage (usually Flash).
 * RADIOLIB_TYPE_ALIAS - construct to create an alias for a type, usually vai the `using` keyword.
 * RADIOLIB_TONE_UNSUPPORTED - some platforms do not have tone()/noTone(), which is required for AFSK.
 * RADIOLIB_BUILTIN_MODULE - some platforms have a builtin radio module on fixed pins, this macro is used to specify that pinout.
 *
 * In addition, some platforms may require RadioLib to disable specific drivers (such as ESP8266).
 *
 * Users may also specify their own configuration by uncommenting the RADIOLIB_CUSTOM_ARDUINO,
 * and then specifying all platform parameters in the section below. This will override automatic
 * platform detection.
 */

  // uncomment to enable custom platform definition
  //#define RADIOLIB_CUSTOM_ARDUINO

#if defined(RADIOLIB_CUSTOM_ARDUINO)
  // name for your platform
  #define RADIOLIB_PLATFORM                           "Custom"

  // the following must be defined if the Arduino core does not support tone or yield function
  //#define RADIOLIB_TONE_UNSUPPORTED
  //#define RADIOLIB_YIELD_UNSUPPORTED

  // in addition, the following macros may be defined if the Arduino core differs from the defaults
  #define RADIOLIB_NC                                 (0xFFFFFFFF)
  #define RADIOLIB_DEFAULT_SPI                        SPI
  #define RADIOLIB_DEFAULT_SPI_SETTINGS               SPISettings(2000000, MSBFIRST, SPI_MODE0)
  #define RADIOLIB_NONVOLATILE                        PROGMEM
  #define RADIOLIB_NONVOLATILE_READ_BYTE(addr)        pgm_read_byte(addr)
  #define RADIOLIB_NONVOLATILE_READ_DWORD(addr)       pgm_read_dword(addr)
  #define RADIOLIB_TYPE_ALIAS(type, alias)            using alias = type;

  // you might also have to define these if the Arduino core has some uncommon pin mode/status types
  #define RADIOLIB_ARDUINOHAL_PIN_MODE_CAST
  #define RADIOLIB_ARDUINOHAL_PIN_STATUS_CAST
  #define RADIOLIB_ARDUINOHAL_INTERRUPT_MODE_CAST

  // some of RadioLib drivers may be excluded, to prevent collisions with platforms (or to speed up build process)
  // the following is a complete list of all possible exclusion macros, uncomment any of them to disable that driver
  // NOTE: Some of the exclusion macros are dependent on each other. For example, it is not possible to exclude RF69
  //       while keeping SX1231 (because RF69 is the base class for SX1231). The dependency is always uni-directional,
  //       so excluding SX1231 and keeping RF69 is valid.
  //#define RADIOLIB_EXCLUDE_CC1101           (1)
  //#define RADIOLIB_EXCLUDE_NRF24            (1)
  //#define RADIOLIB_EXCLUDE_RF69             (1)
  //#define RADIOLIB_EXCLUDE_SX1231           (1) // dependent on RADIOLIB_EXCLUDE_RF69
  //#define RADIOLIB_EXCLUDE_SI443X           (1)
  //#define RADIOLIB_EXCLUDE_RFM2X            (1) // dependent on RADIOLIB_EXCLUDE_SI443X
  //#define RADIOLIB_EXCLUDE_SX127X           (1)
  //#define RADIOLIB_EXCLUDE_SX126X           (1)
  //#define RADIOLIB_EXCLUDE_STM32WLX         (1) // dependent on RADIOLIB_EXCLUDE_SX126X
  //#define RADIOLIB_EXCLUDE_SX128X           (1)
  //#define RADIOLIB_EXCLUDE_AFSK             (1)
  //#define RADIOLIB_EXCLUDE_AX25             (1)
  //#define RADIOLIB_EXCLUDE_HELLSCHREIBER    (1)
  //#define RADIOLIB_EXCLUDE_MORSE            (1)
  //#define RADIOLIB_EXCLUDE_RTTY             (1)
  //#define RADIOLIB_EXCLUDE_SSTV             (1)
  //#define RADIOLIB_EXCLUDE_DIRECT_RECEIVE   (1)

#elif defined(__AVR__) && !(defined(ARDUINO_AVR_UNO_WIFI_REV2) || defined(ARDUINO_AVR_NANO_EVERY) || defined(ARDUINO_ARCH_MEGAAVR))
  // Arduino AVR boards (except for megaAVR) - Uno, Mega etc.
  #define RADIOLIB_PLATFORM                           "Arduino AVR"

  #if !(defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__))
  #define RADIOLIB_LOWEND_PLATFORM
  #endif

#elif defined(ESP8266)
  // ESP8266 boards
  #define RADIOLIB_PLATFORM                           "ESP8266"

#elif defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
  #define RADIOLIB_ESP32

  // ESP32 boards
  #define RADIOLIB_PLATFORM                           "ESP32"
  
  // ESP32 doesn't support tone(), but it can be emulated via LED control peripheral
  #define RADIOLIB_TONE_UNSUPPORTED
  #define RADIOLIB_TONE_ESP32_CHANNEL                 (1)

#elif defined(ARDUINO_ARCH_STM32)
  // official STM32 Arduino core (https://github.com/stm32duino/Arduino_Core_STM32)
  #define RADIOLIB_PLATFORM                           "Arduino STM32 (official)"

#elif defined(SAMD_SERIES)
  // Adafruit SAMD boards (M0 and M4)
  #define RADIOLIB_PLATFORM                           "Adafruit SAMD"

#elif defined(ARDUINO_ARCH_SAMD)
  // Arduino SAMD (Zero, MKR, etc.)
  #define RADIOLIB_PLATFORM                           "Arduino SAMD"
  #define RADIOLIB_ARDUINOHAL_PIN_MODE_CAST           (PinMode)
  #define RADIOLIB_ARDUINOHAL_PIN_STATUS_CAST         (PinStatus)
  #define RADIOLIB_ARDUINOHAL_INTERRUPT_MODE_CAST     (PinStatus)  

#elif defined(__SAM3X8E__)
  // Arduino Due
  #define RADIOLIB_PLATFORM                           "Arduino Due"
  #define RADIOLIB_TONE_UNSUPPORTED

#elif (defined(NRF52832_XXAA) || defined(NRF52840_XXAA)) && !defined(ARDUINO_ARDUINO_NANO33BLE)
  // Adafruit nRF52 boards
  #define RADIOLIB_PLATFORM                           "Adafruit nRF52"

#elif defined(ARDUINO_ARC32_TOOLS)
  // Intel Curie
  #define RADIOLIB_PLATFORM                           "Intel Curie"

#elif defined(ARDUINO_AVR_UNO_WIFI_REV2) || defined(ARDUINO_AVR_NANO_EVERY) || defined(PORTDUINO)
  // Arduino megaAVR boards - Uno Wifi Rev.2, Nano Every
  #define RADIOLIB_PLATFORM                           "Arduino megaAVR"

#elif defined(ARDUINO_ARCH_APOLLO3)
  // Sparkfun Apollo3 boards
  #define RADIOLIB_PLATFORM                           "Sparkfun Apollo3"
  #define RADIOLIB_ARDUINOHAL_PIN_MODE_CAST           (Arduino_PinMode)
  #define RADIOLIB_ARDUINOHAL_PIN_STATUS_CAST         (PinStatus)
  #define RADIOLIB_ARDUINOHAL_INTERRUPT_MODE_CAST     (PinStatus)

#elif defined(ARDUINO_ARDUINO_NANO33BLE)
  // Arduino Nano 33 BLE
  #define RADIOLIB_PLATFORM                           "Arduino Nano 33 BLE"
  #define RADIOLIB_ARDUINOHAL_PIN_MODE_CAST           (PinMode)
  #define RADIOLIB_ARDUINOHAL_PIN_STATUS_CAST         (PinStatus)
  #define RADIOLIB_ARDUINOHAL_INTERRUPT_MODE_CAST     (PinStatus)

  // Arduino mbed OS boards have a really bad tone implementation which will crash after a couple seconds
  #define RADIOLIB_TONE_UNSUPPORTED
  #define RADIOLIB_MBED_TONE_OVERRIDE

#elif defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4)
  // Arduino Portenta H7
  #define RADIOLIB_PLATFORM                           "Portenta H7"
  #define RADIOLIB_ARDUINOHAL_PIN_MODE_CAST           (PinMode)
  #define RADIOLIB_ARDUINOHAL_PIN_STATUS_CAST         (PinStatus)
  #define RADIOLIB_ARDUINOHAL_INTERRUPT_MODE_CAST     (PinStatus)

  // Arduino mbed OS boards have a really bad tone implementation which will crash after a couple seconds
  #define RADIOLIB_TONE_UNSUPPORTED
  #define RADIOLIB_MBED_TONE_OVERRIDE

#elif defined(__STM32F4__) || defined(__STM32F1__)
  // Arduino STM32 core by Roger Clark (https://github.com/rogerclarkmelbourne/Arduino_STM32)
  #define RADIOLIB_PLATFORM                           "STM32duino (unofficial)"
  #define RADIOLIB_ARDUINOHAL_PIN_MODE_CAST           (WiringPinMode)
  #define RADIOLIB_ARDUINOHAL_INTERRUPT_MODE_CAST     (ExtIntTriggerMode)

#elif defined(ARDUINO_ARCH_MEGAAVR)
  // MegaCoreX by MCUdude (https://github.com/MCUdude/MegaCoreX)
  #define RADIOLIB_PLATFORM                           "MegaCoreX"

#elif defined(ARDUINO_ARCH_MBED_RP2040)
  // Raspberry Pi Pico (official mbed core)
  #define RADIOLIB_PLATFORM                           "Raspberry Pi Pico"
  #define RADIOLIB_ARDUINOHAL_PIN_MODE_CAST           (PinMode)
  #define RADIOLIB_ARDUINOHAL_PIN_STATUS_CAST         (PinStatus)
  #define RADIOLIB_ARDUINOHAL_INTERRUPT_MODE_CAST     (PinStatus)

  // Arduino mbed OS boards have a really bad tone implementation which will crash after a couple seconds
  #define RADIOLIB_TONE_UNSUPPORTED
  #define RADIOLIB_MBED_TONE_OVERRIDE

#elif defined(ARDUINO_ARCH_RP2040)
  // Raspberry Pi Pico (unofficial core)
  #define RADIOLIB_PLATFORM                           "Raspberry Pi Pico (unofficial)"
  #define RADIOLIB_ARDUINOHAL_PIN_MODE_CAST           (PinMode)
  #define RADIOLIB_ARDUINOHAL_PIN_STATUS_CAST         (PinStatus)
  #define RADIOLIB_ARDUINOHAL_INTERRUPT_MODE_CAST     (PinStatus)

#elif defined(__ASR6501__) || defined(ARDUINO_ARCH_ASR650X) || defined(DARDUINO_ARCH_ASR6601)
  // CubeCell
  #define RADIOLIB_PLATFORM                           "CubeCell"
  #define RADIOLIB_DEFAULT_SPI_SETTINGS               SPISettings(1000000, MSBFIRST, SPI_MODE0) // see issue #709
  #define RADIOLIB_ARDUINOHAL_PIN_MODE_CAST           (PINMODE)
  #define RADIOLIB_ARDUINOHAL_INTERRUPT_MODE_CAST     (IrqModes)

  // provide an easy access to the on-board module
  #include "board-config.h"
  #define RADIOLIB_BUILTIN_MODULE                      RADIO_NSS, RADIO_DIO_1, RADIO_RESET, RADIO_BUSY

  // CubeCell doesn't seem to define nullptr, let's do something like that now
  #define nullptr                                     NULL

  // ... and also defines pinMode() as a macro, which is by far the stupidest thing I have seen on Arduino
  #undef pinMode

  // ... and uses an outdated GCC which does not support type aliases
  #define RADIOLIB_TYPE_ALIAS(type, alias)            typedef class type alias;

  // ... and it also has no tone(). This platform was designed by an idiot.
  #define RADIOLIB_TONE_UNSUPPORTED

  // ... AND as the (hopefully) final nail in the coffin, IT F*CKING DEFINES YIELD() AS A MACRO THAT DOES NOTHING!!!
  #define RADIOLIB_YIELD_UNSUPPORTED
  #if defined(yield)
  #undef yield
  #endif

#elif defined(RASPI)
  // RaspiDuino framework (https://github.com/me-no-dev/RasPiArduino)
  #define RADIOLIB_PLATFORM                           "RasPiArduino"

  // let's start off easy - no tone on this platform, that can happen
  #define RADIOLIB_TONE_UNSUPPORTED

  // hmm, no yield either - weird on something like Raspberry PI, but sure, we can handle it
  #define RADIOLIB_YIELD_UNSUPPORTED

  // aight, getting to the juicy stuff - PGM_P seems missing, that's the first time
  #define PGM_P                                       const char *

  // ... and for the grand finale, we have millis() and micros() DEFINED AS MACROS!
  #if defined(millis)
  #undef millis
  inline unsigned long millis() { return((unsigned long)(STCV / 1000)); };
  #endif

  #if defined(micros)
  #undef micros
  inline unsigned long micros() { return((unsigned long)(STCV)); };
  #endif

#elif defined(TEENSYDUINO)
  // Teensy
  #define RADIOLIB_PLATFORM                           "Teensy"

#elif defined(ARDUINO_ARCH_RENESAS)
  // Arduino Renesas (UNO R4)
  #define RADIOLIB_PLATFORM                           "Arduino Renesas (UNO R4)"
  #define RADIOLIB_ARDUINOHAL_PIN_MODE_CAST           (PinMode)
  #define RADIOLIB_ARDUINOHAL_PIN_STATUS_CAST         (PinStatus)
  #define RADIOLIB_ARDUINOHAL_INTERRUPT_MODE_CAST     (PinStatus)

#else
  // other Arduino platforms not covered by the above list - this may or may not work
  #define RADIOLIB_PLATFORM                           "Unknown Arduino"
  #define RADIOLIB_UNKNOWN_PLATFORM

#endif

  // set the default values for all macros
  // these will be applied if they were not defined above
  #if !defined(RADIOLIB_NC)
    #define RADIOLIB_NC                                 (0xFFFFFFFF)
  #endif

  #if !defined(RADIOLIB_DEFAULT_SPI)
    #define RADIOLIB_DEFAULT_SPI                        SPI
  #endif

  #if !defined(RADIOLIB_DEFAULT_SPI_SETTINGS)
    #define RADIOLIB_DEFAULT_SPI_SETTINGS               SPISettings(2000000, MSBFIRST, SPI_MODE0)
  #endif

  #if !defined(RADIOLIB_NONVOLATILE)
    #define RADIOLIB_NONVOLATILE                        PROGMEM
  #endif

  #if !defined(RADIOLIB_NONVOLATILE_PTR)
    #define RADIOLIB_NONVOLATILE_PTR                    PGM_P
  #endif

  #if !defined(RADIOLIB_NONVOLATILE_READ_BYTE)
    #define RADIOLIB_NONVOLATILE_READ_BYTE(addr)        pgm_read_byte(addr)
  #endif

  #if !defined(RADIOLIB_NONVOLATILE_READ_DWORD)
    #define RADIOLIB_NONVOLATILE_READ_DWORD(addr)       pgm_read_dword(addr)
  #endif

  #if !defined(RADIOLIB_TYPE_ALIAS)
    #define RADIOLIB_TYPE_ALIAS(type, alias)            using alias = type;
  #endif

  #if !defined(RADIOLIB_ARDUINOHAL_PIN_MODE_CAST)
    #define RADIOLIB_ARDUINOHAL_PIN_MODE_CAST
  #endif

  #if !defined(RADIOLIB_ARDUINOHAL_PIN_STATUS_CAST)
    #define RADIOLIB_ARDUINOHAL_PIN_STATUS_CAST
  #endif

  #if !defined(RADIOLIB_ARDUINOHAL_INTERRUPT_MODE_CAST)
    #define RADIOLIB_ARDUINOHAL_INTERRUPT_MODE_CAST
  #endif

#else
  // generic non-Arduino platform
  #define RADIOLIB_PLATFORM                           "Generic"

  #define RADIOLIB_NC                                 (0xFF)
  #define RADIOLIB_NONVOLATILE
  #define RADIOLIB_NONVOLATILE_READ_BYTE(addr)        (*((uint8_t *)(void *)(addr)))
  #define RADIOLIB_NONVOLATILE_READ_DWORD(addr)       (*((uint32_t *)(void *)(addr)))
  #define RADIOLIB_TYPE_ALIAS(type, alias)            using alias = type;

  #if !defined(RADIOLIB_DEBUG_PORT)
    #define RADIOLIB_DEBUG_PORT                       stdout
  #endif

  #define DEC 10
  #define HEX 16
  #define OCT 8
  #define BIN 2

  #include <stdint.h>

#endif

// This only compiles on STM32 boards with SUBGHZ module, but also
// include when generating docs
#if (!defined(ARDUINO_ARCH_STM32) || !defined(SUBGHZSPI_BASE)) && !defined(DOXYGEN)
  #define RADIOLIB_EXCLUDE_STM32WLX (1)
#endif

// set the global debug mode flag
#if RADIOLIB_DEBUG_BASIC || RADIOLIB_DEBUG_PROTOCOL || RADIOLIB_DEBUG_SPI
  #define RADIOLIB_DEBUG  (1)
#else
  #define RADIOLIB_DEBUG  (0)
#endif

#if RADIOLIB_DEBUG
  #if defined(RADIOLIB_BUILD_ARDUINO)
    #define RADIOLIB_DEBUG_PRINT(...) Module::serialPrintf(__VA_ARGS__)
    #define RADIOLIB_DEBUG_PRINTLN(M, ...) Module::serialPrintf(M "\n", ##__VA_ARGS__)
    #define RADIOLIB_DEBUG_PRINT_LVL(LEVEL, M, ...) Module::serialPrintf(LEVEL "" M, ##__VA_ARGS__)
    #define RADIOLIB_DEBUG_PRINTLN_LVL(LEVEL, M, ...) Module::serialPrintf(LEVEL "" M "\n", ##__VA_ARGS__)

    // some platforms do not support printf("%f"), so it has to be done this way
    #define RADIOLIB_DEBUG_PRINT_FLOAT(LEVEL, VAL, DECIMALS) RADIOLIB_DEBUG_PRINT(LEVEL); RADIOLIB_DEBUG_PORT.print(VAL, DECIMALS)
  #else
    #if !defined(RADIOLIB_DEBUG_PRINT)
      #define RADIOLIB_DEBUG_PRINT(...) fprintf(RADIOLIB_DEBUG_PORT, __VA_ARGS__)
      #define RADIOLIB_DEBUG_PRINT_LVL(LEVEL, M, ...) fprintf(RADIOLIB_DEBUG_PORT, LEVEL "" M, ##__VA_ARGS__)
    #endif
    #if !defined(RADIOLIB_DEBUG_PRINTLN)
      #define RADIOLIB_DEBUG_PRINTLN(M, ...) fprintf(RADIOLIB_DEBUG_PORT, M "\n", ##__VA_ARGS__)
      #define RADIOLIB_DEBUG_PRINTLN_LVL(LEVEL, M, ...) fprintf(RADIOLIB_DEBUG_PORT, LEVEL "" M "\n", ##__VA_ARGS__)
    #endif
    #define RADIOLIB_DEBUG_PRINT_FLOAT(LEVEL, VAL, DECIMALS) RADIOLIB_DEBUG_PRINT(LEVEL "%.3f", VAL)
  #endif

  #define RADIOLIB_DEBUG_HEXDUMP(LEVEL, ...) Module::hexdump(LEVEL, __VA_ARGS__)
#else
  #define RADIOLIB_DEBUG_PRINT(...) {}
  #define RADIOLIB_DEBUG_PRINTLN(...) {}
  #define RADIOLIB_DEBUG_PRINT_FLOAT(VAL, DECIMALS) {}
  #define RADIOLIB_DEBUG_HEXDUMP(...) {}
#endif

#if RADIOLIB_DEBUG_BASIC
  #define RADIOLIB_DEBUG_BASIC_PRINT(...) RADIOLIB_DEBUG_PRINT_LVL("RLB_DBG: ", __VA_ARGS__)
  #define RADIOLIB_DEBUG_BASIC_PRINT_NOTAG(...) RADIOLIB_DEBUG_PRINT_LVL("", __VA_ARGS__)
  #define RADIOLIB_DEBUG_BASIC_PRINTLN(...) RADIOLIB_DEBUG_PRINTLN_LVL("RLB_DBG: ", __VA_ARGS__)
  #define RADIOLIB_DEBUG_BASIC_PRINT_FLOAT(...) RADIOLIB_DEBUG_PRINT_FLOAT("RLB_DBG: ", __VA_ARGS__);
  #define RADIOLIB_DEBUG_BASIC_HEXDUMP(...) RADIOLIB_DEBUG_HEXDUMP("RLB_DBG: ", __VA_ARGS__);
#else
  #define RADIOLIB_DEBUG_BASIC_PRINT(...) {}
  #define RADIOLIB_DEBUG_BASIC_PRINT_NOTAG(...) {}
  #define RADIOLIB_DEBUG_BASIC_PRINTLN(...) {}
  #define RADIOLIB_DEBUG_BASIC_PRINT_FLOAT(...) {}
  #define RADIOLIB_DEBUG_BASIC_HEXDUMP(...) {}
#endif

#if RADIOLIB_DEBUG_PROTOCOL
  #define RADIOLIB_DEBUG_PROTOCOL_PRINT(...) RADIOLIB_DEBUG_PRINT_LVL("RLB_PRO: ", __VA_ARGS__)
  #define RADIOLIB_DEBUG_PROTOCOL_PRINTLN(...) RADIOLIB_DEBUG_PRINTLN_LVL("RLB_PRO: ", __VA_ARGS__)
  #define RADIOLIB_DEBUG_PROTOCOL_PRINT_FLOAT(...) RADIOLIB_DEBUG_PRINT_FLOAT("RLB_PRO: ", __VA_ARGS__);
  #define RADIOLIB_DEBUG_PROTOCOL_HEXDUMP(...) RADIOLIB_DEBUG_HEXDUMP("RLB_PRO: ", __VA_ARGS__);
#else
  #define RADIOLIB_DEBUG_PROTOCOL_PRINT(...) {}
  #define RADIOLIB_DEBUG_PROTOCOL_PRINTLN(...) {}
  #define RADIOLIB_DEBUG_PROTOCOL_PRINT_FLOAT(...) {}
  #define RADIOLIB_DEBUG_PROTOCOL_HEXDUMP(...) {}
#endif

#if RADIOLIB_DEBUG_SPI
  #define RADIOLIB_DEBUG_SPI_PRINT(...) RADIOLIB_DEBUG_PRINT_LVL("RLB_SPI: ", __VA_ARGS__)
  #define RADIOLIB_DEBUG_SPI_PRINT_NOTAG(...) RADIOLIB_DEBUG_PRINT_LVL("", __VA_ARGS__)
  #define RADIOLIB_DEBUG_SPI_PRINTLN(...) RADIOLIB_DEBUG_PRINTLN_LVL("RLB_SPI: ", __VA_ARGS__)
  #define RADIOLIB_DEBUG_SPI_PRINTLN_NOTAG(...) RADIOLIB_DEBUG_PRINTLN_LVL("", __VA_ARGS__)
  #define RADIOLIB_DEBUG_SPI_PRINT_FLOAT(...) RADIOLIB_DEBUG_PRINT_FLOAT("RLB_SPI: ", __VA_ARGS__);
  #define RADIOLIB_DEBUG_SPI_HEXDUMP(...) RADIOLIB_DEBUG_HEXDUMP("RLB_SPI: ", __VA_ARGS__);
#else
  #define RADIOLIB_DEBUG_SPI_PRINT(...) {}
  #define RADIOLIB_DEBUG_SPI_PRINT_NOTAG(...) {}
  #define RADIOLIB_DEBUG_SPI_PRINTLN(...) {}
  #define RADIOLIB_DEBUG_SPI_PRINTLN_NOTAG(...) {}
  #define RADIOLIB_DEBUG_SPI_PRINT_FLOAT(...) {}
  #define RADIOLIB_DEBUG_SPI_HEXDUMP(...) {}
#endif


/*!
  \brief A simple assert macro, will return on error.
*/
#define RADIOLIB_ASSERT(STATEVAR) { if((STATEVAR) != RADIOLIB_ERR_NONE) { return(STATEVAR); } }

/*!
  \brief Macro to check variable is within constraints - this is commonly used to check parameter ranges. Requires RADIOLIB_CHECK_RANGE to be enabled
*/
#if RADIOLIB_CHECK_PARAMS
  #define RADIOLIB_CHECK_RANGE(VAR, MIN, MAX, ERR) { if(!(((VAR) >= (MIN)) && ((VAR) <= (MAX)))) { return(ERR); } }
#else
  #define RADIOLIB_CHECK_RANGE(VAR, MIN, MAX, ERR) {}
#endif

#if RADIOLIB_FIX_ERRATA_SX127X
  #define RADIOLIB_ERRATA_SX127X(...) { errataFix(__VA_ARGS__); }
#else
  #define RADIOLIB_ERRATA_SX127X(...) {}
#endif

// these macros are usually defined by Arduino, but some platforms undef them, so its safer to use our own
#define RADIOLIB_MIN(a,b)				((a)<(b)?(a):(b))
#define RADIOLIB_MAX(a,b)				((a)>(b)?(a):(b))
#define RADIOLIB_ABS(x)         ((x)>0?(x):-(x))

// version definitions
#define RADIOLIB_VERSION_MAJOR  6
#define RADIOLIB_VERSION_MINOR  4
#define RADIOLIB_VERSION_PATCH  2
#define RADIOLIB_VERSION_EXTRA  0

#define RADIOLIB_VERSION (((RADIOLIB_VERSION_MAJOR) << 24) | ((RADIOLIB_VERSION_MINOR) << 16) | ((RADIOLIB_VERSION_PATCH) << 8) | (RADIOLIB_VERSION_EXTRA))

#endif