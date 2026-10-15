[![Documentation Status](https://readthedocs.org/projects/epdiy/badge/?version=latest)](https://epdiy.readthedocs.io/en/latest/?badge=latest) [![Matrix](https://img.shields.io/matrix/epdiy-general:matrix.vroland.de?label=Matrix%20Chat)](https://matrix.to/#/!GUXWriqsBKkWyXzsBK:matrix.vroland.de?via=matrix.vroland.de) [![JoinSlack](https://img.shields.io/badge/Join%20us-on%20Slack-blueviolet.svg)](https://join.slack.com/t/epdiy/shared_invite/zt-189eo7328-bs94cfB~eXPbLYAD1rKQcg)

EPDiy E-Paper Driver
=======================================

EPDiy is a driver board which talks to affordable E-Paper (or E-Ink) screens, which are usually sold as replacement screens for E-Book readers. Why are they interesting?

* Easy on the eyes and paper-like aesthetics
* No power consumption when not updating
* Sunlight-readable

Ready-made DIY modules for this size and with 4bpp (16 Grayscale) color support are currently quite expensive and / or slow.
The EPDiy controller can drive the bare display modules, e.g. from old e-Readers, which are available for 20$ (small) / 30$ (large) on ebay!
Additionally, since it is based on the ESP32S3 (V7) / ESP32 (V2-V6) microcontroller, it features WiFi and Bluetooth connectivity.

The EPDiy driver board targets a range of E-Paper displays, as shown in the table below.
As the driving method for all matrix-based E-ink displays is more or less the same, only the right connector and timings are needed.
The current V7 board has three different display connectors, other display will require an adapter board.

The controller is optimized for the use with LiPo batteries, featuring a LiPo charger and ultra-low deep sleep current.

This project supports a driver for the ESP-IDF and Arduino. For installation instructions, please refer to the [documentation](https://epdiy.readthedocs.io/en/latest/getting_started.html#getting-your-board).
Note that for epdiy V7, update speeds are significantly lower when using the Arduino IDE, because it does not allow to change
the sub-optimal cache configuration.

Get Inspired
------------

The `examples` directory contains some example applications like a weather station or a screen diagnostic test. 
If you want to build something more useful though, how about:

- A serial terminal for connecting to a raspberry pi: [video](https://cdn.hackaday.io/files/1681937195969312/terminal_demo.mp4) [repository](https://github.com/vroland/epdiy-terminal)]
- A Music Player Daemon (MPD) dashboard: [repository](https://github.com/vroland/epdiy-mpd)]
- An e-Paper picture frame: [video](https://www.youtube.com/watch?v=r7AcNQsSZUw)
- And more to come!

Building It
-----------

On the [EPDiy Hardware Page](https://vroland.github.io/epdiy-hardware/), you'll find a list of all boards and variants, adapters, and helpers.
Next to each board, there are manufacturing files (gerbers), Bill of Materials (BoM), part placement files,
and 3D models ready to use!

![demo image](doc/source/img/hardware_page.png)

For ordering from JLCPCB for example, ordering is as easy as downloading the zipped gerbers, BoM, and placement file
and uploading them. The process is very similar for other manufacturers, check your vendor's documentation for details.
Don't forget to oder adapters if the board doesn't have connectors for your specific display.

The current latest version is epdiy V7, beased on the ESP32S3. 
Older versions are also available on the hardware page.


#### Contributing Hardware

Want to contribute your own board variant or adapter? 
Check out the [epdiy-hardware repository](https://github.com/vroland/epdiy-hardware) for instructions.
 

Gettings Started
----------------

For instructions and more information visit the [documentation](https://epdiy.readthedocs.io/en/latest/getting_started.html)!

Join the Discussion
----------------

 - [![Matrix](https://img.shields.io/matrix/epdiy-general:matrix.vroland.de?label=Join%20Matrix)](https://matrix.to/#/!GUXWriqsBKkWyXzsBK:matrix.vroland.de?via=matrix.vroland.de) Matrix Community: +epdiy:matrix.vroland.de
 - Slack: See badge

Displays
--------

|Name      |Size |Resolution|Compatible|Connector|Pin count|Compatible since pcb version|Notes
| --:      | --: | --:       | --:         | --:                  | --: | --: | --: |
| ED060SC4 | 6"  | 800 x 600<br>167 PPI | yes, tested | FH26W-39S-0.3SHW(60) | 39  | v2  |     |
|ED097OC4|9.7"|1200 x 825<br>150 PPI|yes, tested|XF2M-3315-1A|33|v2|Cheap, inferior contrast
|ED097TC2|9.7"|1200 x 825<br>150 PPI|yes, tested|XF2M-3315-1A|33|v2|Slightly higher price, better contrast
|ED097OC1|9.7"|1200 x 825<br>150 PPI|yes (should work)|XF2M-3315-1A|33|v2|Cheap, inferior performance
|ED047TC1|4.7"|960 x 540<br>234 PPI|yes, tested|40-pin|40|LILYGO 4.7" EPD|Supported only by 4.7" e-paper board by LILYGO
| ED050SC5 | 5" | 600 x 800<br>200 PPI | yes, tested       | THD0510-33CL-GF | 33 | v5 |
| ED050SC3 | 5" | 600 x 800<br>200 PPI | yes (should work) | THD0510-33CL-GF | 33 | v5 |
| ED052TC4 | 5.2" | 1280 x 780<br>??? PPI | yes (should work) | WP27D-P050VA3 | 50 | v5 |
| ED133UT2 | 13.3" | 1600 x 1200<br>150 PPI | yes, tested | adapter board | 39 | v2 | Adapter Board required, also PENG133D
| ED060XC3 | 6" | 758 x 1024<br>212 PPI | yes, tested | THD0515-34CL-SN | 34 | v5 | Cheapest, good contrast and resolution
| ED060XD4 | 6"  | 758 x 1024<br>212 PPI | yes, tested | THD0515-34CL-SN | 34 | v5 |
| ED060XC5 | 6"  | 758 x 1024<br>212 PPI | yes (should work as ED060XC3) | THD0515-34CL-SN | 34 | v5 |
| ED060XD6 | 6"  | 758 x 1024<br>212 PPI | yes (should work as ED060XC3) | THD0515-34CL-SN | 34 | v5 |
| ED060XH2 | 6"  | 758 x 1024<br>212 PPI | yes (should work as ED060XC3) | THD0515-34CL-SN | 34 | v5 |
| ED060XC9 | 6"  | 758 x 1024<br>212 PPI | yes (should work as ED060XC3) | THD0515-34CL-SN | 34 | v5 |
| ED060KD1 | 6"  | 1072 x 1448<br>300 PPI | yes (should work as ED060XC3) | THD0515-34CL-SN | 34 | v5 |
| ED060KC1 | 6"  | 1072 x 1448<br>300 PPI | yes (should work as ED060XC3) | THD0515-34CL-SN | 34 | v5 |
| ED060SCF | 6" | 600 x 800<br>167 PPI | yes, tested | THD0515-34CL-SN | 34 | v5 | Different flex cable shape
| ED060SCN | 6" | 600 x 800<br>167 PPI | yes (should work as ED060XC3) | THD0515-34CL-SN | 34 | v5 | Different flex cable shape
| ED060SCP | 6" | 600 x 800<br>167 PPI | yes (should work as ED060XC3) | THD0515-34CL-SN | 34 | v5 | Different flex cable shape
| ED060SC7 | 6" | 600 x 800<br>167 PPI | yes (should work) | AXT334124 | 34 | v5 | connector dropped in v6
| ED060SCG | 6"  | 600 x 800<br>167 PPI | yes (should work) | AXT334124 | 34 | v5 | connector dropped in v6
| ED060SCE | 6"  | 600 x 800<br>167 PPI | yes (should work) | AXT334124 | 34 | v5 | connector dropped in v6
| ED060SCM | 6"  | 600 x 800<br>167 PPI | yes (should work) | AXT334124 | 34 | v5 | connector dropped in v6
| ED060SCT | 6"  | 600 x 800<br>167 PPI | yes, tested       | AXT334124 | 34 | v5 | connector dropped in v6
| ED078KC1 | 7.8"  | 1872 x 1404<br>300 PPI | yes, tested     | FH12-40S-0.5SH | 40 | v7 | 16 data lines


Troubleshooting
---------------

The following list is compiled from past experiences and GitHub issues:
 * **The displayed image displays only part of the image.** Make sure to select the correct display type as described [here](https://epdiy.readthedocs.io/en/latest/getting_started.html#selecting-a-display-type).
 * **The existing image fades / darkens when updating a partial screen region.** Make sure the VCOM voltage is [calibrated](https://epdiy.readthedocs.io/en/latest/getting_started.html#calibrate-vcom) for your specific display.
 * **The second third of the image is replaced with the last third.** This seems to be a timing issue we could not yet quite figure out the reason for. For a workarround or suggestions please [join the discussion](https://github.com/vroland/epdiy/issues/15).
 * **The ESP does not boot correctly when external periperals are connected.** Make sure not to pull GPIO12 high during boot, as it is a strapping pin internal voltage selection (https://github.com/vroland/epdiy/issues/17).
 * **The ESP power consumption in deep sleep is too high.** Add `rtc_gpio_isolate(GPIO_NUM_12);` to your solution. See also [Configuring IOs (Deep-sleep Only)](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/sleep_modes.html?highlight=rtc_gpio_isolate#configuring-ios-deep-sleep-only).

LilyGo Boards
---------------
There are several differences with these boards.
One particular one is the way the LilyGo handles power to the display the official lilygo code has two states.
This is now handled in epdiy in a different way to the lilygo code.
**epd_poweroff()** completely turns the power off to the display and the other peripherals of the lilygo.
The new function **epd_powerdown()** keeps the peripherals on (this allows the touch functions to continue to work). 
**epd_poweroff() should allways be called before sleeping the system**
You can still use touch to wake the screen with the following.
In Arduino it works like this.
`epd_poweroff();`

 `epd_deinit();`

 `esp_sleep_enable_ext1_wakeup(GPIO_SEL_13, ESP_EXT1_WAKEUP_ANY_HIGH);`

 `esp_deep_sleep_start();`

More on E-Paper Displays
------------------------

 * List of E-Paper devices and their screens: https://e-ink-reader.ru/chg_dsp_en.php (thanks, @mcer12)
 * Reverse engineering of display pins, possible starting point for alternative drivers: https://linux-sunxi.org/PocketBook_Touch_Lux_3 (thanks, @mcer12)
 * Reverse engineering and explanation on driving EPDs: http://essentialscrap.com/eink/index.html
 * Earlier take on driving 6" EPDs with an ESP32, basis of original power circuit: http://spritesmods.com/?art=einkdisplay&page=1
 * Related Hackaday projects: https://hackaday.io/project/21607 https://hackaday.io/project/11537

If you know any other useful resources or have suggestions, please comment on #31!

Hackaday Project
----------------

For more details, see the project page on Hackaday: https://hackaday.io/project/168193-epdiy-976-e-paper-controller

![demo image](doc/source/img/demo.jpg)
![board front](doc/source/img/v6.jpg)

Licenses
--------

The weather example is Copyright (c) David Bird 2018 (except for minor modifications). Please refer to `examples/weather/README.md` for details.

The board and schematic are licensed under a <a rel="license" href="https://creativecommons.org/licenses/by-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/80x15.png" /></a> <a rel="license" href="https://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0 International License</a>.

Firmware and remaining examples are licensed under the terms of the GNU Lesser GPL version 3.
Utilities are licensed under the terms of the MIT license.
