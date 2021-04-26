# Adafruit LvGL Glue [![Build Status](https://github.com/adafruit/Adafruit_LvGL_Glue/workflows/Arduino%20Library%20CI/badge.svg)](https://github.com/adafruit/Adafruit_LvGL_Glue/actions)

This Arduino library provides a layer between LittlevGL (a UI library for
embedded systems) and most of Adafruit's color TFT displays (anything that's
a subclass of SPITFT).

Examples show basic use on PyPortal, FeatherWing, CLUE and TFT Gizmo.
Use these as a starting point, see LittlevGL documentation at
docs.littlevgl.com for a thorough explanation of capabilities and use.

# Dependencies
 * [LittlevGL](https://github.com/littlevgl/lvgl)
 * [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
 * [Adafruit HX8357 Library](https://github.com/adafruit/Adafruit_HX8357_Library)
 * [Adafruit ILI9341](https://github.com/adafruit/Adafruit_ILI9341)
 * [Adafruit ST7735 and ST7789 Library](https://github.com/adafruit/Adafruit-ST7735-Library)
 * [Adafruit STMPE610](https://github.com/adafruit/Adafruit_STMPE610)
 * [Adafruit TouchScreen](https://github.com/adafruit/Adafruit_TouchScreen)
 * [Adafruit Zero DMA Library](https://github.com/adafruit/Adafruit_ZeroDMA)
 * [Adafruit ZeroTimer Library](https://github.com/adafruit/Adafruit_ZeroTimer)

# Compatibility
Version 2.0.0 is a breaking change, mostly due to significant structural
changes in the LittlevGL library for Arduino. If you were previously using
an earlier version of this library and/or LittlevGL, both will need updating,
and you should skim the examples and read through the hello_changes example
specifically.

Use on M0 (SAMD21) boards isn't recommended anymore, as LittlevGL has grown.
Simple programs might still work, but it's better to move up to a device
with more RAM -- M4 (SAMD51), nRF52 and ESP32 are currently supported.

# Contributing
Contributions are welcome! Please read our [Code of Conduct](https://github.com/adafruit/Adafruit_LvGL_Glue/blob/master/CODE_OF_CONDUCT.md>)
before contributing to help this project stay welcoming.

## Documentation and doxygen
Documentation is produced by doxygen. Contributions should include documentation for any new code added.

Some examples of how to use doxygen can be found in these guide pages:

https://learn.adafruit.com/the-well-automated-arduino-library/doxygen

https://learn.adafruit.com/the-well-automated-arduino-library/doxygen-tips

Written by Phil Burgess aka Paint Your Dragon for Adafruit Industries.
BSD license, check license.txt for more information
All text above must be included in any redistribution

To install, use the Arduino Library Manager and search for "Adafruit LvGL Glue Library" and install the library.
