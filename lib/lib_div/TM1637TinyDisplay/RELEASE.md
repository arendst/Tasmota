# Release Notes for TM1637TinyDisplay

## v1.0.0
- Initial Release.

## v1.1.0 
- Added interactive [Animator Tool](https://jasonacox.github.io/TM1637TinyDisplay/examples/7-segment-animator.html) to create frame data for the showAnimation() function.
- Added demo sketch to showcase functions and animation features.

## v1.1.1
- Added yield() call to example Demo sketch for esp8266 watchdog timer.

## v1.2.0
- Added floating number support to showNumber() for TM1637 displays with decimal points.

## v1.3.0
- Converted global digitToSegment[] and asciiToSegment[] lookup tables to PROGMEM space to save RAM.
- Fixed comparison between signed and unsigned integer expressions warning #5 
- Saves ~1.5kB in the ATtiny85 example by using showNumber() instead of sprintf() #6
- Fixed implicit case fallthrough and signed/unsigned comparison warnings #7