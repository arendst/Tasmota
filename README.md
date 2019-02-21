## Sonoff-Tasmota

Alternative firmware for _ESP8266 based devices_ like [iTead](https://www.itead.cc/) _**Sonoff**_ with **web**, **timers**, 'Over The Air' (**OTA**) firmware updates and **sensors support**, allowing control under **Serial**, **HTTP**, **MQTT** and **KNX**, so as to be used on **Smart Home Systems**. Written for Arduino IDE and PlatformIO.

[![GitHub version](https://img.shields.io/github/release/arendst/Sonoff-Tasmota.svg)](https://github.com/arendst/Sonoff-Tasmota/releases/latest)
[![GitHub download](https://img.shields.io/github/downloads/arendst/Sonoff-Tasmota/total.svg)](https://github.com/arendst/Sonoff-Tasmota/releases/latest)
[![License](https://img.shields.io/github/license/arendst/Sonoff-Tasmota.svg)](https://github.com/arendst/Sonoff-Tasmota/blob/master/LICENSE.txt)
[![Chat](https://img.shields.io/discord/479389167382691863.svg)](https://discord.gg/Ks2Kzd4)

If you like **Sonoff-Tasmota**, give it a star, or fork it and contribute!

[![GitHub stars](https://img.shields.io/github/stars/arendst/Sonoff-Tasmota.svg?style=social&label=Star)](https://github.com/arendst/Sonoff-Tasmota/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/arendst/Sonoff-Tasmota.svg?style=social&label=Fork)](https://github.com/arendst/Sonoff-Tasmota/network)
[![donate](https://img.shields.io/badge/donate-PayPal-blue.svg)](https://paypal.me/tasmota)

See [RELEASENOTES.md](https://github.com/arendst/Sonoff-Tasmota/blob/master/RELEASENOTES.md) for release information.

In addition to the [release webpage](https://github.com/arendst/Sonoff-Tasmota/releases/latest) the binaries can also be OTA downloaded from http://thehackbox.org/tasmota/release/

### Disclaimer
:warning: **DANGER OF ELECTROCUTION** :warning:

A Sonoff device is not a toy. It uses Mains AC so there is a danger of electrocution if not installed properly. If you don't know how to install it, please call an electrician. Remember: _**SAFETY FIRST**_. It is not worth to risk yourself, your family and your home if you don't know exactly what you are doing. Never try to flash a Sonoff device while it is connected to MAINS AC.

We don't take any responsibility nor liability for using this software nor for the installation or any tips, advice, videos, etc. given by any member of this site or any related site.

### Quick Install
Download one of the released binaries from https://github.com/arendst/Sonoff-Tasmota/releases and flash it to your hardware as documented in the wiki.

### Important User Compilation Information
If you want to compile Sonoff-Tasmota yourself keep in mind the following:

- Only Flash Mode **DOUT** is supported. Do not use Flash Mode DIO / QIO / QOUT as it might seem to brick your device. See [Wiki](https://github.com/arendst/Sonoff-Tasmota/wiki/Theo's-Tasmota-Tips) for background information.
- Sonoff-Tasmota uses a 1M linker script WITHOUT spiffs **1M (no SPIFFS)** for optimal code space. If you compile using ESP/Arduino library 2.3.0 then download the provided new linker script to your Arduino IDE or Platformio base folder. Later version of ESP/Arduino library already contain the correct linker script. See [Wiki > Prerequisite](https://github.com/arendst/Sonoff-Tasmota/wiki/Prerequisite).
- To make compile time changes to Sonoff-Tasmota it can use the ``user_config_override.h`` file. It assures keeping your settings when you download and compile a new version. To use ``user_config.override.h`` you will have to make a copy of the provided ``user_config.override_sample.h`` file and add your setting overrides. To enable the override file you will need to use a compile define as documented in the ``user_config_override_sample.h`` file.

### Version Information
- Sonoff-Tasmota provides all (Sonoff) modules in one file and starts with module Sonoff Basic.
- Once uploaded select module using the configuration webpage or the commands ```Modules``` and ```Module```.
- After reboot select config menu again or use commands ```GPIOs``` and ```GPIO``` to change GPIO with desired sensor.

### Migration Information
See [wiki migration path](https://github.com/arendst/Sonoff-Tasmota/wiki/Upgrade#migration-path) for instructions how to migrate to a major version. Pay attention to the following version breaks due to dynamic settings updates:

1. Migrate to **Sonoff-Tasmota 3.9.x**
2. Migrate to **Sonoff-Tasmota 4.x**
3. Migrate to **Sonoff-Tasmota 5.14**
4. Migrate to **Sonoff-Tasmota 6.x**

### Support Information
<img src="https://github.com/arendst/arendst.github.io/blob/master/media/sonoffbasic.jpg" width="250" align="right" />

See [Wiki](https://github.com/arendst/Sonoff-Tasmota/wiki) for more information.<br />
See [Community](https://groups.google.com/d/forum/sonoffusers) for forum.<br />
See [Chat](https://discord.gg/Ks2Kzd4) for more user experience.

The following devices are supported:
- [iTead Sonoff Basic (R2)](https://www.itead.cc/smart-home/sonoff-wifi-wireless-switch-1.html)
- [iTead Sonoff RF](https://www.itead.cc/smart-home/sonoff-rf.html)
- [iTead Sonoff SV](https://www.itead.cc/smart-home/sonoff-sv.html)<img src="https://github.com/arendst/arendst.github.io/blob/master/media/sonoff_th.jpg" width="250" align="right" />
- [iTead Sonoff TH10/TH16 with temperature sensor](https://www.itead.cc/smart-home/sonoff-th.html)
- [iTead Sonoff Dual (R2)](https://www.itead.cc/smart-home/sonoff-dual.html)
- [iTead Sonoff Pow with Energy Monitoring](https://www.itead.cc/smart-home/sonoff-pow.html)
- [iTead Sonoff Pow R2 with Energy Monitoring](https://www.itead.cc/sonoff-pow-r2.html)
- [iTead Sonoff 4CH (R2)](https://www.itead.cc/smart-home/sonoff-4ch.html)
- [iTead Sonoff 4CH Pro (R2)](https://www.itead.cc/smart-home/sonoff-4ch-pro.html)
- [iTead Sonoff S20 Smart Socket](https://www.itead.cc/smart-socket.html)
- [Sonoff S22 Smart Socket](https://github.com/arendst/Sonoff-Tasmota/issues/627)
- [iTead Sonoff S26 Smart Socket](https://www.itead.cc/sonoff-s26-wifi-smart-plug.html)
- [iTead Sonoff S31 Smart Socket with Energy Monitoring](https://www.itead.cc/sonoff-s31.html)
- [iTead Slampher](https://www.itead.cc/slampher.html)
- [iTead Sonoff Touch](https://www.itead.cc/sonoff-touch.html)
- [iTead Sonoff T1](https://www.itead.cc/sonoff-t1.html)
- [iTead Sonoff SC](https://www.itead.cc/sonoff-sc.html)
- [iTead Sonoff Led](https://www.itead.cc/sonoff-led.html)<img src="https://github.com/arendst/arendst.github.io/blob/master/media/sonoff4chpror2.jpg" height="250" align="right" />
- [iTead Sonoff BN-SZ01 Ceiling Led](https://www.itead.cc/bn-sz01.html)
- [iTead Sonoff B1](https://www.itead.cc/sonoff-b1.html)
- [iTead Sonoff iFan02](https://www.itead.cc/sonoff-ifan02-wifi-smart-ceiling-fan-with-light.html)
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
- [BlitzWolf BW-SHP2 Smart Socket with Energy Monitoring](https://www.banggood.com/BlitzWolf-BW-SHP2-Smart-WIFI-Socket-EU-Plug-220V-16A-Work-with-Amazon-Alexa-Google-Assistant-p-1292899.html)<img src="https://github.com/arendst/arendst.github.io/blob/master/media/shelly2_small_250a.png" width="250" align="right" />
- [Luani HVIO board](https://luani.de/projekte/esp8266-hvio/)
- [Wemos D1 mini](https://wiki.wemos.cc/products:d1:d1_mini)
- [HuaFan Smart Socket](https://github.com/arendst/Sonoff-Tasmota/wiki/HuaFan-Smart-Socket)
- [Hyleton-313 Smart Plug](https://github.com/arendst/Sonoff-Tasmota/wiki/Hyleton-313-Smart-Plug)
- [Allterco Shelly 1](https://shelly.cloud/shelly1-open-source/)
- [Allterco Shelly 2 with Energy Monitoring](https://shelly.cloud/shelly2/)
- NodeMcu and Ledunia
- [KS-602 based switches like GresaTek, Jesiya, NewRice, Lyasi etc](https://ucexperiment.wordpress.com/2017/11/14/reprogramming-a-lyasi-wifi-wall-switch-with-esp8285/)

### Contribute
You can contribute to Sonoff-Tasmota by
- providing Pull Requests (Features, Proof of Concepts, Language files or Fixes)
- testing new released features and report issues
- donating to acquire hardware for testing and implementing or out of gratitude

[![donate](https://img.shields.io/badge/donate-PayPal-blue.svg)](https://paypal.me/tasmota)

### Credits

#### Libraries Used
Libraries used with Sonoff-Tasmota are:
- [ESP8266 core for Arduino](https://github.com/esp8266/Arduino)
- [Adafruit CCS811](https://github.com/adafruit/Adafruit_CCS811)
- [Adafruit ILI9341](https://github.com/adafruit/Adafruit_ILI9341)
- [Adafruit LED Backpack](https://github.com/adafruit/Adafruit-LED-Backpack-Library)
- [Adafruit SGP30](https://github.com/adafruit/Adafruit_SGP30)
- [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306)
- [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library)
- [ArduinoJson](https://arduinojson.org/)
- [arduino mqtt](https://github.com/256dpi/arduino-mqtt)
- [Bosch BME680](https://github.com/BoschSensortec/BME680_driver)
- [C2 Programmer](http://app.cear.ufpb.br/~lucas.hartmann/tag/efm8bb1/)
- [esp-epaper-29-ws-20171230-gemu](https://github.com/gemu2015/Sonoff-Tasmota/tree/displays/lib)
- [esp-knx-ip](https://github.com/envy/esp-knx-ip)
- [I2Cdevlib](https://github.com/jrowberg/i2cdevlib)
- [IRremoteEsp8266](https://github.com/markszabo/IRremoteESP8266)
- [JobaTsl2561](https://github.com/joba-1/Joba_Tsl2561)
- [Liquid Cristal](https://github.com/marcoschwartz/LiquidCrystal_I2C)
- [MultiChannelGasSensor](http://wiki.seeedstudio.com/Grove-Multichannel_Gas_Sensor/)
- [NeoPixelBus](https://github.com/Makuna/NeoPixelBus)
- [NewPing](https://bitbucket.org/teckel12/arduino-new-ping/wiki/Home)
- [OneWire](https://github.com/PaulStoffregen/OneWire)
- [PubSubClient](https://github.com/knolleary/pubsubclient)
- [rc-switch](https://github.com/sui77/rc-switch)

#### People inspiring me
People helping to keep the show on the road:
- David Lang providing initial issue resolution and code optimizations
- Heiko Krupp for his IRSend, HTU21, SI70xx and Wemo/Hue emulation drivers
- Wiktor Schmidt for Travis CI implementation
- Thom Dietrich for PlatformIO optimizations
- Marinus van den Broek for his EspEasy groundwork
- Pete Ba for more user friendly energy monitor calibration
- Lobradov providing compile optimization tips
- Flexiti for his initial timer implementation
- reloxx13 for his [TasmoAdmin](https://github.com/reloxx13/TasmoAdmin) management tool
- Joachim Banzhaf for his TSL2561 library and driver
- Gijs Noorlander for his MHZ19, SenseAir and updated PubSubClient drivers
- Emontnemery for his HomeAssistant Discovery concept and many code tuning tips
- Aidan Mountford for his HSB support
- Daniel Ztolnai for his Serial Bridge implementation
- Gerhard Mutz for his SGP30, Sunrise/Sunset and display support drivers
- Nuno Ferreira for his HC-SR04 driver
- Adrian Scillato for his (security)fixes and implementing and maintaining KNX
- Gennaro Tortone for implementing and maintaining Eastron drivers
- Raymond Mouthaan for managing Wemos Wiki information
- Norbert Richter for his decode-config.py tool
- Andre Thomas for providing [thehackbox](http://thehackbox.org/tasmota/) OTA support and daily development builds
- Joel Stein and digiblur for their Tuya research and driver
- Frogmore42 and Jason2866 for providing many issue answers
- Many more providing Tips, Pocs or PRs

### License

This program is licensed under GPL-3.0
