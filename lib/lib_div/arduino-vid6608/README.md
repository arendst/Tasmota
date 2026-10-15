# Gauge stepper motor Switec X25.168 driver for VID6608 and clones

![Moving example](doc/output.gif)

This library implements driver for Arduino framework
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

This library is developed by inspiration from [SwitecX25](https://github.com/clearwater/SwitecX25) library, many thanks to author.

This library has following features:

* More precise Datasheet complaince
* Another smoothing method (requires less calculations)
* Optimized homing
* Extended API's

[![PlatformIO Registry](https://badges.registry.platformio.org/packages/petrows/library/vid6608.svg)](https://registry.platformio.org/libraries/petrows/vid6608)

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

## Basic example

Simple code to activate the library.

```cpp
#include <Arduino.h>
#include <vid6608.h>

// standard X25.168 range 315 degrees at 1/3 degree steps
#define STEPS (320 * 12)

#define PIN_STEP 26 // Pin, connected to f(scx)
#define PIN_DIR 27  // Pin. connected to CW/CCW

vid6608 motor1(PIN_STEP, PIN_DIR, STEPS);

unsigned long nextMoveTime = 0;
uint16_t nextMovePos = 0;

void setup(void)
{
  // Run the motor against the stops
  motor1.zero();
  // Plan next move (in loop())
  motor1.moveTo(100);
}

void loop(void)
{
  // the motor only moves when you call update
  motor1.loop();
}
```
