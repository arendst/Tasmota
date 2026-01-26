JPEGDEC<br>
-----------------------------------
Copyright (c) 2020 BitBank Software, Inc.<br>
Written by Larry Bank<br>
larry@bitbanksoftware.com<br>
<br>
**I optimize other people's code for a living. This library is a good example of the kind of work I do for my commercial clients; it contains many unique and clever optimizations that allows it to perform better than anything else available. I'm happy to contribute optimized libraries to the open source community in addition to working on commercial projects. Whatever platform you're using, I can make significant improvements to your native code. Please contact me so that I can show you how.**<br>
<br>
![JPEGDEC](/demo.jpg?raw=true "JPEGDEC")
<br>
I started working with image and video files around 1989 and soon turned my interest into a Document Imaging product based on my own imaging library. Over the years I added support for more and more formats until I had supported all of the standard ones, including DICOM. I sold my Document Imaging business in 1997. I started over with a new library design in 2000 and added even more formats. Over the years, I found uses for my imaging code in projects such as retro gaming. I recently went looking to see if there was a good JPEG viewer for Arduino and only found ones which sacrificed speed to work on MCUs with almost no RAM. I thought it would be possible to create an optimized JPEG decoder that could run on any MCU with at least 20K of RAM, so I started modifying my old JPEG code for this purpose. There are a lot of clever optimizations contained in the code that I kept as trade secrets for many years. I decided that it's time to share the code with the community. This code doesn't look like other JPEG libraries because I wrote it from a "blank sheet". It took many hundreds of hours of work to get it working correctly and find all of the optimizations. If you appreciate my work, please consider sponsoring me.

<br>

Features:<br>
---------<br>
- *New* JPEGDisplay helper class simplifies displaying images on LCDs supported by my bb_spi_lcd display library.
- Supports any MCU with at least 20K of RAM (Cortex-M0+ is the simplest I've tested)
- Optimized for speed; the main limitation will be how fast you can copy the pixels to the display. You can use DMA assisted SPI to help.
- Includes built-in cropping function that's faster than JPEGTRAN.
- JPEG image data can come from memory (FLASH/RAM), SDCard or any media you provide.
- Simple class and callback design allows you to easily add JPEG support to any application.
- The C code doing the heavy lifting is completely portable and has no external dependencies.
- Includes fast downscaling options (1/2, 1/4, 1/8).
- Includes option to detect and decode the embedded Exif thumbnail
- Supports Baseline Huffman images (grayscale or YCbCr)
- Supports thumbnail (DC-only) decoding of progressive JPEG images
- Now with SIMD (ESP32-S3, Arm NEON, X86 SSE2) optimized color conversion
- Includes optional Floyd-Steinberg dithering to 1, 2 or 4-bpp grayscale output; useful for e-paper displays<br>

<br>
<p align="center">
  <img width="600" height="250" src="https://github.com/bitbank2/JPEGDEC/blob/master/perf.jpg?raw=true">
</p>
<br>

**A note about performance**<br>
The chart above gives you an idea of how the code performs on various MCUs with the different scaling options. The test image used is both large in dimension and large in terms of the compressed data size. The two factors that have the most influence on performance are the compressed data size and the output scaling options. This is because the time to decode JPEG data scales mostly linearly with the amount of data to decode. To get the fastest decode speed, choose the lowest acceptable quality for your image; this will produce the smallest compressed data size. When you must use an image that is too large, the scaling options allow you to display it at a lower resolution than the original.<br>

<br>
<p align="center">
  <img width="720" height="568" src="https://github.com/bitbank2/JPEGDEC/blob/master/squirrel_dither.jpg?raw=true">
</p>
<br>

**Floyd-Steinberg dithering**<br>
I added the option to dither the output to 1, 2 or 4-bit per pixel grayscale using the FS error diffusion algorithm. It creates a pleasant output without visible blocking nor repeating patterns. This seemed useful for generating output suitable for high resolution e-paper displays. The image above was decoded from a 800x600 color JPEG and rendered as 2-bits per pixels (4 gray levels) on a 400x300 4.2" e-paper display. See the Wiki for info on how to use this option.

Acquiring JPEG files to play:
----------------------------
You'll notice that the images provided in the test_images folder have been turned into C code. Each byte is now in the form 0xAB so that it can be compiled into your program and stored in FLASH memory alongside your other code. You can use a command line tool I wrote called image_to_c (https://github.com/bitbank2/image_to_c) or an older Linux tool called xxd to convert a binary file into this type of text. Make sure to add a const/PROGMEM modifier in front of the JPEG data array to ensure that it gets written to FLASH and not RAM by your build environment.

The Callback functions:
-----------------------
One of the ways to allow this code to run on any embedded platform was to define a set of callback functions. These isolate the JPEG decoding logic from the display and file I/O. This allows the core code to run on any system, but you need to help it a little. At a minimum, your code must provide a function to draw (or store) each block of image pixels emitted by the library. If you're displaying a JPEG file from memory (RAM or FLASH), this is the only function you need to provide. In the examples folder there are multiple sketches to show how this is done on various display libraries. For reading from SD cards, 4 other functions must be provided: open, close, read, seek. There is an example for implementing these in the examples folder as well.
Note:
If you're using the ESP32 or ESP8266 (or another MCU which uses the Harvard Architecture) and decoding JPEG images stored in RAM or FLASH, you'll need to use the correct open function (openRAM or openFLASH). For MCUs based on the ARM Cortex-M, they are interchangeable.

The API:
--------
Please consult the Wiki for detailed info about each method exposed by the JPEGDEC class.


If you find this code useful, please consider becoming a sponsor or sending a donation.

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=SR4F44J2UR8S4)

## Funding

This project received funding through [NGI Zero Core](https://nlnet.nl/core), a fund established by [NLnet](https://nlnet.nl) with financial support from the European Commission's [Next Generation Internet](https://ngi.eu) program. Learn more at the [NLnet project page](https://nlnet.nl/project/ImageCodes-Optimised).

[<img src="https://nlnet.nl/logo/banner.png" alt="NLnet foundation logo" width="20%" />](https://nlnet.nl)
[<img src="https://nlnet.nl/image/logos/NGI0_tag.svg" alt="NGI Zero Logo" width="20%" />](https://nlnet.nl/core)
