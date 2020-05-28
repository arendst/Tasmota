#ifndef STM32_FLASH_DEBUG_H
#define STM32_FLASH_DEBUG_H

#define STM32_FLASH_DEBUG           0

#if STM32_FLASH_DEBUG
    #define DEBUG_MSG(...) Serial.println(__VA_ARGS__)
#else
    #define DEBUG_MSG(...)
#endif

#endif
