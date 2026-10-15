# mp3_shine_esp32
Shine MP3 encoder for ESP32 - Last updated FEB 2019 - Compatibility with current IDF unknown!!

This is a 'port' of the old Shine MP3 encoder originally written for ARM or MIPS arch devices, quite a long time ago.

Memory allocation has been optimised for the ESP32
Some asm has been added to boost performance in the ESP32 arch

The are leftovers of the beginings of a dual core implementation but this was not required in the end, the code has not been cleaned up.

You should init the encoder ASAP in your code as the encoder needs large contiguous chunks of RAM.

A sample FreeRTOS task has been included as a guide. Change it to suit your own needs.

For full 48000Hz Joint Stereo performance on one core your will need to compile with the -Os optimisation option.

I will try to get an HTTP MP3 streaming demo up at some stage, but I'm currently working 80 hour weeks so don't hold your breath.

Any questions feel free to ask.

Cheers!
-fkn

Limitations

The encoding algorithm is rather simple. In particular, it does not have any Psychoacoustic Model.

A bit of history

This code was dug out from the dusty crates of those times before internet and github. It apparently was created by Gabriel Bouvigne sometime around the end of the 20th century. The encoder was converted circa 2001 by Pete Everett to fixed-point arithmetic for the RISC OS. Latest we know, Patrick Roberts had worked on the code to make it multi-platform and more library oriented. That was around 2006.
