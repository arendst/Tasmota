// do not delete

#ifndef BEARSSL_TASMOTA_CONFIG
#define BEARSSL_TASMOTA_CONFIG

#ifndef __ets__
#define __ets__
#endif

#ifndef ESP32
#ifndef ICACHE_FLASH
#define ICACHE_FLASH
#endif
#else
#ifndef PROGMEM
#define PROGMEM
#endif
#endif

// #ifndef ESP8266
// #define ESP8266
// #endif

#ifndef BR_SLOW_MUL15
#define BR_SLOW_MUL15 1         // shrinks EC code by 8.5k
#endif

#ifndef BR_MAX_RSA_SIZE
#ifdef USE_4K_RSA
#define BR_MAX_RSA_SIZE 4096    // max 4096 bits RSA keys
#else
#define BR_MAX_RSA_SIZE 2048    // max 2048 bits RSA keys
#endif
#endif

#ifndef BR_MAX_EC_SIZE
#define BR_MAX_EC_SIZE 256      // max 256 bits EC keys
#endif

#endif
