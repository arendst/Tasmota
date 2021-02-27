#pragma once

// For those platforms/methods that support dynamic channel setting
enum NeoBusChannel
{
    NeoBusChannel_0,
    NeoBusChannel_1,
    NeoBusChannel_2,

// NRF52x has only 3 or 4 channels of PWM
#if defined(ARDUINO_ARCH_NRF52840)

#if defined(NRF_PWM3)
    NeoBusChannel_3
#endif

// ESP32 has either 8 or 4 channels (S2 has only 4)
#elif defined(ARDUINO_ARCH_ESP32)

    NeoBusChannel_3,

#if !defined(CONFIG_IDF_TARGET_ESP32S2)
    NeoBusChannel_4,
    NeoBusChannel_5,
    NeoBusChannel_6,
    NeoBusChannel_7,
#endif // CONFIG_IDF_TARGET_ESP32S2
#endif // ARDUINO_ARCH_ESP32
};