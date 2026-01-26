#pragma once

// For those platforms/methods that support dynamic channel setting
//
// ESP32   - 8 TX channels
// ESP32S2 - 4 TX channels
// ESP32C3 - 2 TX channels
// NRF52840 - 3 or 4 channels (some variants only have 3)

enum NeoBusChannel
{
    NeoBusChannel_0,
    NeoBusChannel_1,

#if !defined(CONFIG_IDF_TARGET_ESP32C3) && !defined(CONFIG_IDF_TARGET_ESP32C2) && !defined(CONFIG_IDF_TARGET_ESP32C5) && !defined(CONFIG_IDF_TARGET_ESP32C6)

    NeoBusChannel_2,

// NRF52x has only 3 or 4 channels of PWM
#if defined(ARDUINO_ARCH_NRF52840)

#if defined(NRF_PWM3)
    NeoBusChannel_3,
#endif

// ESP32 has either 8, 4, or 2 channels (S2 has only 4, C3 only 2)
#elif defined(ARDUINO_ARCH_ESP32)

    NeoBusChannel_3,

#if !defined(CONFIG_IDF_TARGET_ESP32S2)
    NeoBusChannel_4,
    NeoBusChannel_5,
    NeoBusChannel_6,
    NeoBusChannel_7,
#endif // !defined(CONFIG_IDF_TARGET_ESP32S2)

#endif // !defined(CONFIG_IDF_TARGET_ESP32C3) && !defined(CONFIG_IDF_TARGET_ESP32C2) && !defined(CONFIG_IDF_TARGET_ESP32C6)

#endif // ARDUINO_ARCH_ESP32

    NeoBusChannel_COUNT
};