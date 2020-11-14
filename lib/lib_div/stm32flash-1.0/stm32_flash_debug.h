#ifndef STM32_FLASH_DEBUG_H
#define STM32_FLASH_DEBUG_H

#define STM32_FLASH_DEBUG           0

#if STM32_FLASH_DEBUG
    #define DEBUG_MSG(...) AddLog_P2(LOG_LEVEL_DEBUG, __VA_ARGS__)
#else
    #define DEBUG_MSG(...)
#endif

#endif
