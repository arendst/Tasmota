### ILI9488 Arduino Library
This library is for support for the 320x480 tft controller over 4 wire SPI. It is based heavily on the [Adafruit_ILI9341](https://github.com/adafruit/Adafruit_ILI9341) library and is designed to work with the [Adafruit_GFX library](https://github.com/adafruit/Adafruit-GFX-Library). 

I have made some heavy modifications, as the typical Adafruit TFT libraries are designed to work with 16bit color (RGB565), and the ILI9488 can only do 24bit (RGB888) color in 4 wire SPI mode. You can still use the library EXACTLY like you would for 16bit mode color, the colors are converted before sending to the display. What this means is, things will be slower than normal. Not only do you have to write twice as many pixels as a normal 240x320 display, 153,600px (320x480) vs 76,800px (240x320), but you also have to do a lightweight conversion on each color, and write 3 bytes vs 2bytes per pixel.

For this reason, I do not recommend an AVR based Arduino for this library, although it will still work. I highly recommend a faster microcontroller based on ARM such as the Teensy, [STM32duino](https://github.com/rogerclarkmelbourne/Arduino_STM32), Arduino Zero, or the Arduing Due.

On the STM32duino, DMA is supported and is therefore much faster. 