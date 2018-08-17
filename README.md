## Sonoff-Tasmota

Alternative firmware for _ESP8266 based devices_ like [iTead](https://www.itead.cc/) _**Sonoff**_ with **web**, **timers**, 'Over The Air' (**OTA**) firmware updates and **sensors support**, allowing control under **Serial**, **HTTP**, **MQTT** and **KNX**, so as to be used on **Smart Home Systems**. Written for Arduino IDE and PlatformIO.

[![GitHub version](https://img.shields.io/github/release/arendst/Sonoff-Tasmota.svg)](https://github.com/arendst/Sonoff-Tasmota/releases/latest)
[![GitHub download](https://img.shields.io/github/downloads/arendst/Sonoff-Tasmota/total.svg)](https://github.com/arendst/Sonoff-Tasmota/releases/latest)
[![License](https://img.shields.io/github/license/arendst/Sonoff-Tasmota.svg)](https://github.com/arendst/Sonoff-Tasmota/blob/development/LICENSE.txt)

If you like **Sonoff-Tasmota**, give it a star, or fork it and contribute!

[![GitHub stars](https://img.shields.io/github/stars/arendst/Sonoff-Tasmota.svg?style=social&label=Star)](https://github.com/arendst/Sonoff-Tasmota/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/arendst/Sonoff-Tasmota.svg?style=social&label=Fork)](https://github.com/arendst/Sonoff-Tasmota/network)
[![donate](https://img.shields.io/badge/donate-PayPal-blue.svg)](https://paypal.me/tasmota)

### Development
[![Build Status](https://img.shields.io/travis/arendst/Sonoff-Tasmota.svg)](https://travis-ci.org/arendst/Sonoff-Tasmota)

Current version is **6.1.1.x** - See [sonoff/_releasenotes.ino](https://github.com/arendst/Sonoff-Tasmota/blob/development/sonoff/_releasenotes.ino) for release information and [sonoff/_changelog.ino](https://github.com/arendst/Sonoff-Tasmota/blob/development/sonoff/_changelog.ino) for change information.

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
See [Community](https://groups.google.com/d/forum/sonoffusers) for forum and more user experience.

The following devices are supported:
- [iTead Sonoff Basic](https://www.itead.cc/smart-home/sonoff-wifi-wireless-switch-1.html)
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
- [BlitzWolf BW-SHP2 Smart Socket with Energy Monitoring](https://www.banggood.com/BlitzWolf-BW-SHP2-Smart-WIFI-Socket-EU-Plug-220V-16A-Work-with-Amazon-Alexa-Google-Assistant-p-1292899.html)
- [Luani HVIO board](https://luani.de/projekte/esp8266-hvio/)
- Wemos D1 mini, NodeMcu and Ledunia

### Firmware Release Information
Different firmware images are released based on Features and Sensors selection guided by code and memory usage.

- The Minimal version allows intermediate OTA uploads to support larger versions and does NOT change any persistent parameter.
- The Classic version allows single OTA uploads as did the previous Sonoff-Tasmota versions.

#### Available Features and Sensors

| Feature or Sensor              | sonoff | classic | minimal | knx | allsensors |
|--------------------------------|--------|---------|---------|-----|------------|
| MY_LANGUAGE en-GB              | x | x | x | x | x |
| MQTT_LIBRARY_TYPE PUBSUBCLIENT | x | x | x | x | x |
| USE_DOMOTICZ                   | x | x | - | x | x |
| USE_HOME_ASSISTANT             | x | x | - | x | x |
| USE_MQTT_TLS                   | - | - | - | - | - |
| USE_KNX                        | - | - | - | x | - |
| USE_WEBSERVER                  | x | x | x | x | x |
| USE_EMULATION                  | x | x | - | - | x |
| USE_DISCOVERY                  | x | x | - | x | x |
| WEBSERVER_ADVERTISE            | x | x | - | x | x |
| MQTT_HOST_DISCOVERY            | x | x | - | x | x |
| USE_TIMERS                     | x | - | - | x | x |
| USE_TIMERS_WEB                 | x | - | - | x | x |
| USE_SUNRISE                    | x | - | - | x | x |
| USE_RULES                      | x | - | - | x | x |
|                                |   |   |   |   |   |
| USE_ADC_VCC                    | x | x | x | x | x |
| USE_DS18B20                    | x | x | - | x | - |
| USE_DS18x20                    | - | - | - | - | x |
| USE_DS18x20_LEGACY             | - | - | - | - | - |
| USE_I2C                        | x | x | - | x | x |
| USE_SHT                        | x | x | - | x | x |
| USE_SHT3X                      | x | x | - | x | x |
| USE_HTU                        | x | x | - | x | x |
| USE_LM75AD                     | x | - | - | x | x |
| USE_BMP                        | x | x | - | x | x |
| USE_BME680                     | - | - | - | - | x |
| USE_SGP30                      | x | - | - | x | x |
| USE_BH1750                     | x | x | - | x | x |
| USE_VEML6070                   | - | - | - | - | x |
| USE_TSL2561                    | - | - | - | - | x |
| USE_SI1145                     | - | - | - | - | x |
| USE_ADS1115                    | - | - | - | - | x |
| USE_ADS1115_I2CDEV             | - | - | - | - | - |
| USE_INA219                     | - | - | - | - | x |
| USE_MGS                        | - | - | - | - | x |
| USE_SPI                        | - | - | - | - | - |
| USE_MHZ19                      | x | x | - | x | x |
| USE_SENSEAIR                   | x | - | - | x | x |
| USE_PMS5003                    | x | - | - | x | x |
| USE_NOVA_SDS                   | x | - | - | x | x |
| USE_PZEM004T                   | x | - | - | x | x |
| USE_SERIAL_BRIDGE              | x | - | - | x | x |
| USE_SDM120                     | - | - | - | - | x |
| USE_SDM630                     | - | - | - | - | x |
| USE_IR_REMOTE                  | x | x | - | x | x |
| USE_IR_HVAC                    | - | - | - | - | x |
| USE_IR_RECEIVE                 | x | - | - | x | x |
| USE_WS2812                     | x | x | - | x | x |
| USE_WS2812_DMA                 | - | - | - | - | - |
| USE_ARILUX_RF                  | x | - | - | x | x |
| USE_SR04                       | x | - | - | x | x |
| USE_RF_FLASH                   | x | - | - | x | x |

#### Typical file size

| ESP/Arduino library version | sonoff | classic | minimal | knx  | allsensors |
|-----------------------------|--------|---------|---------|------|------------|
| ESP/Arduino lib v2.3.0      | 538k   | 490k    | 407k    | 548k | 562k       |
| ESP/Arduino lib v2.4.0      | 543k   | 498k    | 414k    | 553k | 565k       |
| ESP/Arduino lib v2.4.1      | 544k   | 500k    | 416k    | 555k | 567k       |

See [Tasmota ESP/Arduino library version related issues](https://github.com/arendst/Sonoff-Tasmota/wiki/Theo's-Tasmota-Tips#20180523---relation-tasmota-and-esp8266arduino-core-version) for more information.

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
- [Adafruit SGP30](https://github.com/adafruit/Adafruit_SGP30)
- [ArduinoJson](https://arduinojson.org/)
- [Bosch BME680](https://github.com/BoschSensortec/BME680_driver)
- [C2 Programmer](http://app.cear.ufpb.br/~lucas.hartmann/tag/efm8bb1/)
- [Esp8266MqttClient](https://github.com/tuanpmt/ESP8266MQTTClient)
- [esp-knx-ip](https://github.com/envy/esp-knx-ip)
- [esp-mqtt-arduino](https://github.com/i-n-g-o/esp-mqtt-arduino)
- [ESPAsyncUDP](https://github.com/me-no-dev/ESPAsyncUDP)
- [I2Cdevlib](https://github.com/jrowberg/i2cdevlib)
- [IRremoteEsp8266](https://github.com/markszabo/IRremoteESP8266)
- [JobaTsl2561](https://github.com/joba-1/Joba_Tsl2561)
- [MultiChannelGasSensor](http://wiki.seeedstudio.com/Grove-Multichannel_Gas_Sensor/)
- [NeoPixelBus](https://github.com/Makuna/NeoPixelBus)
- [OneWire](https://github.com/PaulStoffregen/OneWire)
- [PubSubClient](https://github.com/knolleary/pubsubclient)

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
- Gijs Noorlander for his MHZ19 and SenseAir drivers
- Emontnemery for his HomeAssistant Discovery concept and many code tuning tips
- Aidan Mountford for his HSB support
- Daniel Ztolnai for his Serial Bridge implementation
- Gerhard Mutz for his SGP30 and Sunrise/Sunset driver
- Nuno Ferreira for his HC-SR04 driver
- Adrian Scillato for his (security)fixes and implementing and maintaining KNX
- Gennaro Tortone for implementing and maintaining Eastron drivers
- Raymond Mouthaan for managing Wemos Wiki information
- Norbert Richter, Frogmore42 and Jason2866 for providing many issue answers
- Many more providing Tips, Pocs or PRs

### License

This program is licensed under GPL-3.0
