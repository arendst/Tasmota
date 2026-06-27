# ESP-IDF library to drive Automotive gauge drivers using HW RMT driver

This library implements driver for native ESP-IDF framework
for following driver chips for analog automotive gauges (Switec X25.168, X27.168 and clones) with microstepping support:

* VID6606 (2 motors)
* VID6608 (4 motors)
* VT6608S
* AX1201728SG
* BY8920
* many others

Driver chips with microstepping is the recommended way to drive such motors,
they provide much more relailabe and smooth movement with reduced noise and
to avoid skipping steps.

This library is very similar to [Arduino-vid6608](https://github.com/petrows/arduino-vid6608),
but it implements hardware generator for driver steps sequence,
the [ESP-32 Remote Control Transceiver (RMT)](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/rmt.html):

* No `delay()` call;
* Much better precision movement without interrupts and slow-downs for bi-axial instruments;

Only the ESP-32 is supported. All chips has minimum 2 RMT channels.

This library has following features:

* More precise Datasheet complaince
* Another smoothing method (requires less calculations)
* Optimized homing
* Extended API's

[![PlatformIO Registry](https://badges.registry.platformio.org/packages/petrows/library/esp-32-vid6608-rmt.svg)](https://registry.platformio.org/libraries/petrows/esp-32-vid6608-rmt)

## Chip documentation

See [VID6606 Datasheet (English)](doc/VID6608.pdf).

## Wiring

This library requires that two pins (per drive) are connected to two outputs.

![Datasheet scheme](doc/operation-configuration.png)

* Step pin f(scx): impulse to drive motor to one microstep;
* Direction pin CW/CCW: defines direction ov movement;
* RESET pin: does not controlled by this library. Hold to VDD to enable function (see notes below);

## RESET pin

This library does not control RESET pin, please perform this inside your firmware.

I have problems with some IC's, as they lost function after RESET pin manipulation. [Datasheet](doc/VID6608.pdf) recommends to hold it LOW during boot, and set to HIGH to enable operation, but i recommend just to connect to VDD to be safe.

## Setting zero

Motor is set to zero by moving whole scale and kept bouncing on the one of
dead positions. This library provides optimized way to perform homing: it does
1/2 of scale forward, then full scale backward. This helps to reduce bouncing
like in the classical "full scale back" method.

## Function documentation

See inline documentation in source code: [vid6608.h](src/vid6608.h).

## Compile examples

To compile and flash examples, navigate to example dir and call idf:

```bash
cd examples/gauge-cal
export IDF_TARGET=esp32c6
idf.py flash monitor
```

## Basic example

```cpp
#include "esp32_vid6608_rmt.h"

extern "C" void app_main(void)
{
    vid6608::Config cfg {
        .stepPin   = GPIO_NUM_14,
        .dirPin    = GPIO_NUM_18,
        .maxSteps  = 12 * 320,
    };
    vid6608 driver = vid6608(cfg);

    driver.zero();
    vTaskDelay(pdMS_TO_TICKS(2000));
    driver.setPos(12 * 180);
}
```
