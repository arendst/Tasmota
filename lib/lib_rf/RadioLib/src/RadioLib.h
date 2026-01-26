#if !defined(_RADIOLIB_H)
#define _RADIOLIB_H

/*!
  \mainpage RadioLib Documentation

  Universal wireless communication library for Arduino.

  \par Currently Supported Wireless Modules and Protocols
  - CC1101 FSK module
  - RF69 FSK module
  - Si443x FSK module
  - SX126x LoRa/FSK module
  - SX127x LoRa/FSK module
  - SX128x LoRa/GFSK/BLE/FLRC module
  - SX1231 FSK module
  - PhysicalLayer protocols
    - RTTY (RTTYClient)
    - Morse Code (MorseClient)
    - AX.25 (AX25Client)
    - SSTV (SSTVClient)
    - Hellschreiber (HellClient)
    - 4-FSK (FSK4Client)
    - APRS (APRSClient)

  \par Quick Links
  Documentation for most common methods can be found in its reference page (see the list above).\n
  Some methods (mainly configuration) are also overridden in derived classes, such as SX1272, SX1278, RFM96 etc. for SX127x.\n
  \ref status_codes have their own page.\n
  Some modules implement methods of one or more compatibility layers, loosely based on the ISO/OSI model:
  - PhysicalLayer - FSK and LoRa radio modules

  \see https://github.com/jgromes/RadioLib

  \copyright  Copyright (c) 2019 Jan Gromes
*/

#include "TypeDef.h"
#include "Module.h"

#include "Hal.h"
#if defined(RADIOLIB_BUILD_ARDUINO)
#include "ArduinoHal.h"
#endif


// warnings are printed in this file since BuildOpt.h is compiled in multiple places

// check God mode
#if RADIOLIB_GODMODE
  #warning "God mode active, I hope it was intentional. Buckle up, lads."
#endif

// print debug info
#if RADIOLIB_DEBUG
  #define RADIOLIB_VALUE_TO_STRING(x) #x
  #define RADIOLIB_VALUE(x) RADIOLIB_VALUE_TO_STRING(x)
  #pragma message("\nRadioLib Debug Info\nVersion:  \"" \
  RADIOLIB_VALUE(RADIOLIB_VERSION_MAJOR) "." \
  RADIOLIB_VALUE(RADIOLIB_VERSION_MINOR) "." \
  RADIOLIB_VALUE(RADIOLIB_VERSION_PATCH) "." \
  RADIOLIB_VALUE(RADIOLIB_VERSION_EXTRA) "\"\n" \
  "Platform: " RADIOLIB_VALUE(RADIOLIB_PLATFORM) "\n" \
  "Compiled: " RADIOLIB_VALUE(__DATE__) " " RADIOLIB_VALUE(__TIME__) \
  )
#endif

// check unknown/unsupported platform
#if defined(RADIOLIB_UNKNOWN_PLATFORM)
  #warning "RadioLib might not be compatible with this Arduino board - check supported platforms at https://github.com/jgromes/RadioLib!"
#endif

// print warning for low-end platforms
#if defined(RADIOLIB_LOWEND_PLATFORM)
  #warning "Low-end platform detected, stability issues are likely!"
#endif

#include "modules/CC1101/CC1101.h"
#include "modules/LLCC68/LLCC68.h"
#include "modules/nRF24/nRF24.h"
#include "modules/RF69/RF69.h"
#include "modules/RFM2x/RFM22.h"
#include "modules/RFM2x/RFM23.h"
#include "modules/Si443x/Si4430.h"
#include "modules/Si443x/Si4431.h"
#include "modules/Si443x/Si4432.h"
#include "modules/SX123x/SX1231.h"
#include "modules/SX123x/SX1233.h"
#include "modules/SX126x/SX1261.h"
#include "modules/SX126x/SX1262.h"
#include "modules/SX126x/SX1268.h"
#include "modules/SX126x/STM32WLx.h"
#include "modules/SX127x/SX1272.h"
#include "modules/SX127x/SX1273.h"
#include "modules/SX127x/SX1276.h"
#include "modules/SX127x/SX1277.h"
#include "modules/SX127x/SX1278.h"
#include "modules/SX127x/SX1279.h"
#include "modules/SX128x/SX1280.h"
#include "modules/SX128x/SX1281.h"
#include "modules/SX128x/SX1282.h"

// physical layer protocols
#include "protocols/PhysicalLayer/PhysicalLayer.h"
#include "protocols/AFSK/AFSK.h"
#include "protocols/AX25/AX25.h"
#include "protocols/Hellschreiber/Hellschreiber.h"
#include "protocols/Morse/Morse.h"
#include "protocols/Pager/Pager.h"
#include "protocols/RTTY/RTTY.h"
#include "protocols/SSTV/SSTV.h"
#include "protocols/FSK4/FSK4.h"
#include "protocols/APRS/APRS.h"
#include "protocols/ExternalRadio/ExternalRadio.h"
#include "protocols/Print/Print.h"
#include "protocols/BellModem/BellModem.h"
#include "protocols/LoRaWAN/LoRaWAN.h"

// utilities
#include "utils/CRC.h"
#include "utils/Cryptography.h"

// only create Radio class when using RadioShield
#if RADIOLIB_RADIOSHIELD

// RadioShield pin definitions
#define RADIOSHIELD_CS_A    10
#define RADIOSHIELD_IRQ_A   2
#define RADIOSHIELD_RST_A   9
#define RADIOSHIELD_GPIO_A  8
#define RADIOSHIELD_CS_B    5
#define RADIOSHIELD_IRQ_B   3
#define RADIOSHIELD_RST_B   7
#define RADIOSHIELD_GPIO_B  6

/*!
  \class Radio

  \brief Library control object when using RadioShield.
  Contains two pre-configured "modules", which correspond to the slots on shield.
*/
class Radio {
  public:

    Module* ModuleA;
    Module* ModuleB;

    /*!
      \brief Default constructor. Only used to set ModuleA and ModuleB configuration.
    */
    Radio() {
      ModuleA = new Module(RADIOSHIELD_CS_A, RADIOSHIELD_IRQ_A, RADIOSHIELD_RST_A, RADIOSHIELD_GPIO_A);
      ModuleB = new Module(RADIOSHIELD_CS_B, RADIOSHIELD_IRQ_B, RADIOSHIELD_RST_B, RADIOSHIELD_GPIO_B);
    }

#if RADIOLIB_GODMODE
  private:
#endif

};

Radio RadioShield;
#endif

#endif
