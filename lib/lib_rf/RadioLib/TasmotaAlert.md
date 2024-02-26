TasmotaAlert

Action to take in case of new release of RadioLib

20240223

1 - Remove folder `RadioLibs/examples/NonArduino` (Fixes Github vulnerability alerts)
2 - Add ``#define RADIOLIB_EEPROM_UNSUPPORTED`` to file `RadioLib/src/BuildOpt.h` section ``defined(ESP8266)`` and ``defined(ESP32) || defined(ARDUINO_ARCH_ESP32)`` (Fixes safeboot compilation)
3 - Disable in files `RadioLib\src\protocols\LoRaWAN\LoRaWAN.cpp` and `RadioLib\src\protocols\Pager\Pager.cpp` inclusion of ``#include "esp_attr.h"`` (Fixes ESP8266 compilation)

