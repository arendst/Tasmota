# Sonoff-Tasmota (KNX_MOD)

This is a [MOD](https://github.com/ascillato/Sonoff-Tasmota_KNX) for Sonoff-Tasmota to integrate KNX Protocol to its features.

This MOD requires the KNX Library [ascillato/ESP-KNX-IP_Lite](https://github.com/ascillato/ESP-KNX-IP_Lite)

Basic Functionality is Available. **Work in progress**.

-----------------------------------------------------------------------------------------------------------------------------------
## Road Map ##

**For Sonoff-Tasmota_KNX:**
- [x] Add Web Menu (uses the one provided by the library)
- [x] Add Feature to Receive telegrams and modify Relay Status
- [x] Add Feature to Receive telegrams from multiple Group Addresses to modify just one relay status (useful for scenes)
- [x] Add Feature to Send telegrams of relay status change
- [ ] Add Feature to Send telegrams of one relay status to multiple Group Addresses (useful for scenes)
- [x] Add Feature to Send telegrams of button pressed
- [x] Add Feature to receive telegrams to toggle relay status
- [ ] Add Feature to read Temperature from Tasmota
- [x] Add Feature to send Temperature by a set interval (tasmota teleperiod)
- [x] Add Feature to receive command to read temperature
- [ ] Add Feature to recognize Tasmota config to show the same amount of relays, buttons, etc
- [ ] Add Feature to Save Config
- [ ] Add Feature to Load Config
- [ ] Change Web Menu to a Tasmota one to save FLASH and RAM
- [ ] Complete all the language files with keys
- [ ] Optimize code to reduce Flash and RAM

**For ESP_KNX_IP_LITE:**
- [x] Add Functions to control library from code and not only by Web Menu
- [ ] Complete needed functions for Tasmota
- [ ] Add WebUI Functions
- [ ] Change all *_id_t to config_id_t so as to have control over webpage order
- [ ] Mod GA_Register function to be similar to CB_Register (needed for Send telegrams of one relay status to multiple Group Addresses)
- [ ] Optimize code to reduce Flash and RAM

-----------------------------------------------------------------------------------------------------------------------------------

## Modifications to [Original Tasmota](https://github.com/arendst/Sonoff-Tasmota) ##

* Adding the file _/sonoff/xdrv_10_KNX.ino_ 
* Add the entry `#define USE_KNX` on _/sonoff/user_config.h_
* Add entries to the file _/sonoff/webserver.ino_
* Add entries to the file _/sonoff/sonoff.ino_
* Add entries to the file _/sonoff/sonoff.h_
* Add entries to the file _/sonoff/settings.h_
* Add entries to sensor files
* Add entries to language files

Up to now, enabling KNX uses +14.7k of code

There is **NO CONFLICT** with MQTT, Home Assistant, Web, etc. Tests show fast response of all features running at same time.

This version is kept in sync with the Original Sonoff-Tasmota

-----------------------------------------------------------------------------------------------------------------------------------

## Contributors to this Project ##

* Adrian Scillato [ascillato](https://github.com/ascillato)
* Sisamiwe [sisamiwe](https://github.com/sisamiwe) - Thanks for the guide on using KNX.
* Nico Weichbrodt [envy](https://github.com/envy) - Thanks for the patience and help with the modifications to ESP_KNX_IP.

-----------------------------------------------------------------------------------------------------------------------------------

## Sonoff-Tasmota

Alternative Firmware for 'ESP8266 based Devices' like **Sonoff** by [iTead Studio](https://www.itead.cc/), with control under _Serial_, _Web_, _KNX_ and _MQTT_, and allowing 'Over the Air' (OTA) firmware updates. Written using Arduino IDE and PlatformIO.

[![GitHub version](https://img.shields.io/github/release/arendst/Sonoff-Tasmota.svg)](https://github.com/arendst/Sonoff-Tasmota/releases/latest)
[![GitHub download](https://img.shields.io/github/downloads/arendst/Sonoff-Tasmota/total.svg)](https://github.com/arendst/Sonoff-Tasmota/releases/latest)
[![License](https://img.shields.io/github/license/arendst/Sonoff-Tasmota.svg)](https://github.com/arendst/Sonoff-Tasmota/blob/development/LICENSE.txt)

If you like **Sonoff Tasmota**, give it a star, or fork it and contribute! 
[![GitHub stars](https://img.shields.io/github/stars/arendst/Sonoff-Tasmota.svg?style=social&label=Star)](https://github.com/arendst/Sonoff-Tasmota/stargazers) 
[![GitHub forks](https://img.shields.io/github/forks/arendst/Sonoff-Tasmota.svg?style=social&label=Fork)](https://github.com/arendst/Sonoff-Tasmota/network)

### Development:
[![Build Status](https://img.shields.io/travis/arendst/Sonoff-Tasmota.svg)](https://travis-ci.org/arendst/Sonoff-Tasmota)

Current version is **5.12.0i** - See [sonoff/_releasenotes.ino](https://github.com/arendst/Sonoff-Tasmota/blob/development/sonoff/_releasenotes.ino) for change information.

### Quick install

Download one of the released binaries from https://github.com/arendst/Sonoff-Tasmota/releases and flash it to your hardware as documented in the wiki.

### Important User Compilation Information

If you want to compile Sonoff-Tasmota yourself keep in mind the following:

- Only Flash Mode **DOUT** is supported. Do not use Flash Mode DIO / QIO / QOUT as it might seem to brick your device. See [Wiki](https://github.com/arendst/Sonoff-Tasmota/wiki/Theo's-Tasmota-Tips) for background information.
- Sonoff-Tasmota uses a 1M linker script WITHOUT spiffs for optimal code space. If you compile using ESP/Arduino library 2.3.0 then download the provided new linker script to your Arduino IDE or Platformio base folder. Later version of ESP/Arduino library already contain the correct linker script. See [Wiki > Prerequisite](https://github.com/arendst/Sonoff-Tasmota/wiki/Prerequisite).
- To make compile time changes to Sonoff-Tasmota it can use the ``user_config_override.h`` file. It assures keeping your settings when you download and compile a new version. To use ``user_config.override.h`` you will have to make a copy of the provided ``user_config.override_sample.h`` file and add your setting overrides. To enable the override file you will need to use a compile define as documented in the ``user_config_override_sample.h`` file.

### Version Information

- Sonoff-Tasmota provides all (Sonoff) modules in one file and starts with module Sonoff Basic.
- Once uploaded select module using the configuration webpage or the commands ```Modules``` and ```Module```.
- After reboot select config menu again or use commands ```GPIOs``` and ```GPIO``` to change GPIO with desired sensor.

<img src="https://github.com/arendst/arendst.github.io/blob/master/media/sonoffbasic.jpg" width="250" align="right" />

See [Wiki](https://github.com/arendst/Sonoff-Tasmota/wiki) for more information.<br />
See [Community](https://groups.google.com/d/forum/sonoffusers) for forum and more user experience.

The following devices are supported:
- [iTead Sonoff Basic](https://www.itead.cc/smart-home/sonoff-wifi-wireless-switch-1.html)
- [iTead Sonoff RF](https://www.itead.cc/smart-home/sonoff-rf.html)
- [iTead Sonoff SV](https://www.itead.cc/smart-home/sonoff-sv.html)<img src="https://github.com/arendst/arendst.github.io/blob/master/media/sonoff_th.jpg" width="250" align="right" />
- [iTead Sonoff TH10/TH16 with temperature sensor](https://www.itead.cc/smart-home/sonoff-th.html)
- [iTead Sonoff Dual (R2)](https://www.itead.cc/smart-home/sonoff-dual.html)
- [iTead Sonoff Pow](https://www.itead.cc/smart-home/sonoff-pow.html)
- [iTead Sonoff 4CH](https://www.itead.cc/smart-home/sonoff-4ch.html)
- [iTead Sonoff 4CH Pro](https://www.itead.cc/smart-home/sonoff-4ch-pro.html)
- [iTead S20 Smart Socket](https://www.itead.cc/smart-socket.html)
- [Sonoff S22 Smart Socket](https://github.com/arendst/Sonoff-Tasmota/issues/627)
- [iTead Sonoff S31 Smart Socket with Energy Monitoring](https://www.itead.cc/sonoff-s31.html)
- [iTead Slampher](https://www.itead.cc/slampher.html)
- [iTead Sonoff Touch](https://www.itead.cc/sonoff-touch.html)
- [iTead Sonoff T1](https://www.itead.cc/sonoff-t1.html)
- [iTead Sonoff SC](https://www.itead.cc/sonoff-sc.html)
- [iTead Sonoff Led](https://www.itead.cc/sonoff-led.html)<img src="https://github.com/arendst/arendst.github.io/blob/master/media/sonoff4ch.jpg" height="250" align="right" />
- [iTead Sonoff BN-SZ01 Ceiling Led](https://www.itead.cc/bn-sz01.html)
- [iTead Sonoff B1](https://www.itead.cc/sonoff-b1.html)
- [iTead Sonoff RF Bridge 433](https://www.itead.cc/sonoff-rf-bridge-433.html)
- [iTead Sonoff Dev](https://www.itead.cc/sonoff-dev.html)
- [iTead 1 Channel Switch 5V / 12V](https://www.itead.cc/smart-home/inching-self-locking-wifi-wireless-switch.html)
- [iTead Motor Clockwise/Anticlockwise](https://www.itead.cc/smart-home/motor-reversing-wifi-wireless-switch.html)
- [Electrodragon IoT Relay Board](http://www.electrodragon.com/product/wifi-iot-relay-board-based-esp8266/)
- AI Light or any my9291 compatible RGBW LED bulb
- H801 PWM LED controller
- [MagicHome PWM LED controller](https://github.com/arendst/Sonoff-Tasmota/wiki/MagicHome-LED-strip-controller)
- AriLux AL-LC01, AL-LC06 and AL-LC11 PWM LED controller
- [Supla device - Espablo-inCan mod. for electrical Installation box](https://forum.supla.org/viewtopic.php?f=33&t=2188)
- [Luani HVIO board](https://luani.de/projekte/esp8266-hvio/)
- Wemos D1 mini, NodeMcu and Ledunia

### License

This program is licensed under GPL-3.0
