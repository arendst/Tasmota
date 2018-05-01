# Sonoff-Tasmota KNX (Development Repository)
## Now integrated in [Sonoff-Tasmota](https://github.com/arendst/Sonoff-Tasmota)!

[Sonoff-Tasmota_KNX](https://github.com/ascillato/Sonoff-Tasmota_KNX) is a modification for [Sonoff-Tasmota](https://github.com/arendst/Sonoff-Tasmota) to add a basic functionality of the [KNX IP Protocol](https://www.knx.org/knx-en/index.php).

[![GitHub version](https://img.shields.io/github/release/ascillato/Sonoff-Tasmota_KNX.svg)](https://github.com/ascillato/Sonoff-Tasmota_KNX/releases/latest) [![GitHub download](https://img.shields.io/github/downloads/ascillato/Sonoff-Tasmota_KNX/total.svg)](https://github.com/ascillato/Sonoff-Tasmota_KNX/releases/latest) [![License](https://img.shields.io/github/license/ascillato/Sonoff-Tasmota_KNX.svg)](https://github.com/ascillato/Sonoff-Tasmota_KNX/blob/development/LICENSE.txt)

If you like **Sonoff Tasmota KNX**, give it a star, or fork it and contribute!
[![GitHub stars](https://img.shields.io/github/stars/ascillato/Sonoff-Tasmota_KNX.svg?style=social&label=Star)](https://github.com/ascillato/Sonoff-Tasmota_KNX/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/ascillato/Sonoff-Tasmota_KNX.svg?style=social&label=Fork)](https://github.com/ascillato/Sonoff-Tasmota_KNX/network)

## KNX Explanation ##

[<img src="https://www.knx.org/wGlobal/wGlobal/layout/images/knx-logo.png" />](https://www.knx.org/knx-en/knx/association/what-is-knx/index.php)

The [KNX IP Protocol](https://www.knx.org/knx-en/knx/association/what-is-knx/index.php) is an _international open standard_ for smart homes and smart buildings automation. It is a decentralized system. Each device can talk directly to each other without the need of a central controller or server. Any panel or server is just for telesupervision and for sending requests. KNX IP Protocol uses a UDP multicast on _224.0.23.12 : 3671_, so there is no need for a KNX Router unless you want to communicate to KNX Devices that are not in the WIFI Network (Twisted Pair, RF, Powerline).

Each device has a physical address (like a fixed IP) as **1 . 1 . 0** and that address is used for configuration purposes.

Each device can be configured with group addresses as **2 / 2 / 1** and that address can be used for sending/receiving commands.
So, for example, if 2 devices that are configured with the **2 / 2 / 1** for turning on/off their outputs, and other device send _Turn ON_ command to **2 / 2 / 1**, both devices will turn on their outputs.

## Integration ##

Several home automation systems have KNX support. For example, [Home Assistant](https://github.com/home-assistant/home-assistant) has a [XKNX Python Library](https://github.com/XKNX/xknx) to connect to KNX devices using a KNX Router. If you don't have a **KNX Router**, you can use a **Software KNX Router** like [KNXd](https://github.com/knxd/knxd) on the same Raspberry Pi than Home Assistant. KNXd is used by Home Assistant for reading this UDP Multicast, although KNXd has other cool features that need extra hardware like connect to KNX devices by Twister Pair, Power Line or RF.

If you use the ETS (KNX Configurator Software) you can add any Sonoff Tasmota KNX as a dummy device.

## Requirement ##

* [ESP KNX IP Library](https://github.com/envy/esp-knx-ip). A copy of the library is also available [here](https://github.com/ascillato/Sonoff-Tasmota_KNX/tree/development/lib/esp-knx-ip-0.5.0).

It is recommended to compile with version 2.3.0 of the esp8266 board libraries. With v2.4.0 and v2.4.1 there are some issues related to sleep command.

## Usage Examples ##

There are multiple possible configurations. Here are explained just a few as example. The options for selecting relays, buttons, sensors, etc. are only available if were configured on _Configure Module Menu_.

To configure KNX, enter on the Configuration Menu of Sonoff-Tasmota and select Configure KNX.

<img src="https://github.com/ascillato/Sonoff-Tasmota_KNX/blob/development/.github/Config_Menu.jpg" />
<img src="https://github.com/ascillato/Sonoff-Tasmota_KNX/blob/development/.github/KNX_menu.jpg" />

**1) Setting Several Sonoff to be controlled as one by a Home Automation System:**

We can set one of the group address to be the same in all the devices so as to turn them on or off at the same time.
In this case, so as to inform the status of all the relays to the Automation System, just one of the devices have to be configured as the responder. If you use the same Group Address for sending and receiving, you have to take into account not to make loops.

DEVICE 1

<img src="https://github.com/ascillato/Sonoff-Tasmota_KNX/blob/development/.github/1.jpg" />

DEVICE 2

<img src="https://github.com/ascillato/Sonoff-Tasmota_KNX/blob/development/.github/2.jpg" />

**2) Setting 2 Sonoff to be linked as stair lights:**

We can set one device to send the status of its output and another to read that and follow. And the second device can send the status of its button and the first device will toggle. With this configuration we can avoid to make a loop.

DEVICE 1

<img src="https://github.com/ascillato/Sonoff-Tasmota_KNX/blob/development/.github/3.jpg" />

DEVICE 2

<img src="https://github.com/ascillato/Sonoff-Tasmota_KNX/blob/development/.github/4.jpg" />

**3) Setting a button as initiator of a scene:**

Just setting one device to send the push of a button, and the rest just use that value to turn them on. In this case, there is no toggle. Every time the button is pushed, the turn on command is sent.

DEVICE 1

<img src="https://github.com/ascillato/Sonoff-Tasmota_KNX/blob/development/.github/5.jpg" />

DEVICE 2

<img src="https://github.com/ascillato/Sonoff-Tasmota_KNX/blob/development/.github/6.jpg" />

**4) Setting a Temperature sensor:**

We can configure to send the value of temperature or humidity every teleperiod. This teleperiod can be configured. See Sonoff Tasmota [wiki](https://github.com/arendst/Sonoff-Tasmota/wiki/Commands). It is recommended also to set the reply temperature address.

<img src="https://github.com/ascillato/Sonoff-Tasmota_KNX/blob/development/.github/7.jpg" />

## Development Road Map ##

**For Sonoff-Tasmota_KNX:**
- [x] Add Web Menu
- [x] Add Feature to Receive telegrams and modify Relay Status
- [x] Add Feature to Receive telegrams from multiple Group Addresses to modify just one relay status (useful for scenes)
- [x] Add Feature to Send telegrams of relay status change
- [x] Add Feature to Send telegrams of one relay status to multiple Group Addresses (useful for scenes)
- [x] Add Feature to Send telegrams of button pressed
- [x] Add Feature to receive telegrams to toggle relay status
- [x] Add Feature to read Temperature, Humidity from Tasmota
- [x] Add Feature to send Temperature, Humidity by a set interval (tasmota teleperiod)
- [x] Add Feature to receive command to read temperature, Humidity
- [x] Add Feature to recognize Tasmota config to show the same number of relays, buttons, etc.
- [x] Add Feature to Save Config
- [x] Add Feature to Load Config
- [x] Add Log Info
- [x] Complete all the language files with keys
- [ ] Optimize code to reduce Flash and RAM

## Modifications to Sonoff-Tasmota ##

* Adding the file _/sonoff/xdrv_11_KNX.ino_
* Add the entry `#define USE_KNX` on _/sonoff/user_config.h_
* Add entries to the file _/sonoff/webserver.ino_
* Add entries to the file _/sonoff/sonoff.ino_
* Add entries to the file _/sonoff/sonoff.h_
* Add entries to the file _/sonoff/settings.h_
* Add entries to sensor files
* Add entries to language files

Up to now, enabling KNX uses +23k of code and +3k3 of memory.

There is **NO CONFLICT** with MQTT, Home Assistant, Web, etc. Tests show fast response of all features running at same time.

## Contributors ##

* Adrian Scillato [ascillato](https://github.com/ascillato)
* Sisamiwe [sisamiwe](https://github.com/sisamiwe) - Thanks for the guide on using KNX.
* Nico Weichbrodt [envy](https://github.com/envy) - Thanks for the patience and help with the modifications to ESP_KNX_IP.

-----------------------------------------------------------------------------------------------------------------------------------

[<img src="https://github.com/ascillato/Sonoff-Tasmota_KNX/blob/development/.github/donate.jpg" width="250" />](https://www.paypal.me/ascillato)

-----------------------------------------------------------------------------------------------------------------------------------

## Sonoff-Tasmota

Alternative firmware for _ESP8266 based devices_ like [iTead](https://www.itead.cc/) _**Sonoff**_ with **web**, **timers**, 'Over The Air' (**OTA**) firmware updates and **sensors support**, allowing control under **Serial**, **HTTP**, **MQTT** and **KNX**, so as to be used on **Smart Home Systems**. Written for Arduino IDE and PlatformIO.

[![GitHub version](https://img.shields.io/github/release/arendst/Sonoff-Tasmota.svg)](https://github.com/arendst/Sonoff-Tasmota/releases/latest)
[![GitHub download](https://img.shields.io/github/downloads/arendst/Sonoff-Tasmota/total.svg)](https://github.com/arendst/Sonoff-Tasmota/releases/latest)
[![License](https://img.shields.io/github/license/arendst/Sonoff-Tasmota.svg)](https://github.com/arendst/Sonoff-Tasmota/blob/development/LICENSE.txt)

If you like **Sonoff-Tasmota**, give it a star, or fork it and contribute!
[![GitHub stars](https://img.shields.io/github/stars/arendst/Sonoff-Tasmota.svg?style=social&label=Star)](https://github.com/arendst/Sonoff-Tasmota/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/arendst/Sonoff-Tasmota.svg?style=social&label=Fork)](https://github.com/arendst/Sonoff-Tasmota/network)

### Development
[![Build Status](https://img.shields.io/travis/arendst/Sonoff-Tasmota.svg)](https://travis-ci.org/arendst/Sonoff-Tasmota)

Current version is **5.13.0a** - See [sonoff/_releasenotes.ino](https://github.com/arendst/Sonoff-Tasmota/blob/development/sonoff/_releasenotes.ino) for change information.

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
- [iTead Sonoff Pow with Energy Monitoring](https://www.itead.cc/smart-home/sonoff-pow.html)
- [iTead Sonoff Pow R2 with Energy Monitoring](https://www.itead.cc/sonoff-pow-r2.html)
- [iTead Sonoff 4CH (R2)](https://www.itead.cc/smart-home/sonoff-4ch.html)
- [iTead Sonoff 4CH Pro (R2)](https://www.itead.cc/smart-home/sonoff-4ch-pro.html)
- [iTead S20 Smart Socket](https://www.itead.cc/smart-socket.html)
- [Sonoff S22 Smart Socket](https://github.com/arendst/Sonoff-Tasmota/issues/627)
- [iTead Sonoff S31 Smart Socket with Energy Monitoring](https://www.itead.cc/sonoff-s31.html)
- [iTead Slampher](https://www.itead.cc/slampher.html)
- [iTead Sonoff Touch](https://www.itead.cc/sonoff-touch.html)
- [iTead Sonoff T1](https://www.itead.cc/sonoff-t1.html)
- [iTead Sonoff SC](https://www.itead.cc/sonoff-sc.html)
- [iTead Sonoff Led](https://www.itead.cc/sonoff-led.html)<img src="https://github.com/arendst/arendst.github.io/blob/master/media/sonoff4chpror2.jpg" height="250" align="right" />
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

### Firmware release information
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
| USE_BMP                        | x | x | - | x | x |
| USE_BME680                     | - | - | - | - | x |
| USE_SGP30                      | x | - | - | x | x |
| USE_BH1750                     | x | x | - | x | x |
| USE_VEML6070                   | - | - | - | - | x |
| USE_TSL2561                    | - | - | - | - | x |
| USE_ADS1115                    | - | - | - | - | x |
| USE_ADS1115_I2CDEV             | - | - | - | - | - |
| USE_INA219                     | - | - | - | - | x |
| USE_MGS                        | - | - | - | - | x |
| USE_SPI                        | - | - | - | - | - |
| USE_MHZ19                      | x | x | - | x | x |
| USE_SENSEAIR                   | x | x | - | x | x |
| USE_PMS5003                    | x | x | - | x | x |
| USE_NOVA_SDS                   | x | - | - | x | x |
| USE_PZEM004T                   | x | x | - | x | x |
| USE_SERIAL_BRIDGE              | x | - | - | x | x |
| USE_IR_REMOTE                  | x | x | - | x | x |
| USE_IR_HVAC                    | - | - | - | - | x |
| USE_IR_RECEIVE                 | x | - | - | x | x |
| USE_WS2812                     | x | x | - | x | x |
| USE_WS2812_DMA                 | - | - | - | - | - |
| USE_ARILUX_RF                  | x | x | - | x | x |
| USE_SR04                       | x | - | - | x | x |

#### Typical file size

| ESP/Arduino library version    | sonoff | classic | minimal | knx  | allsensors |
|--------------------------------|--------|---------|---------|------|------------|
| ESP/Arduino lib v2.3.0         | 526k   | 488k    | 427k    | 535k | 549k       |
| ESP/Arduino lib v2.4.0         | 531k   | 496k    | 435k    | 540k | 552k       |
| ESP/Arduino lib v2.4.1         | 534k   | 499k    | 437k    | 543k | 555k       |

### Contribute
You can contribute to Sonoff-Tasmota by
- providing Pull Requests (Features, Proof of Concepts, Language files or Fixes)
- testing new released features and report issues
- donating to acquire hardware for testing and implementing or out of gratitude

[![donate](https://img.shields.io/badge/donate-PayPal-blue.svg)](https://paypal.me/tasmota)

### Credits

#### Libraries used
Libraries used with Sonoff-Tasmota are:
- [ESP8266 core for Arduino](https://github.com/esp8266/Arduino)
- [Adafruit BME680](https://github.com/adafruit/Adafruit_BME680)
- [Adafruit Sensor](https://github.com/adafruit/Adafruit_Sensor)
- [Adafruit SGP30](https://github.com/adafruit/Adafruit_SGP30)
- [ArduinoJson](https://arduinojson.org/)
- [Esp8266MqttClient](https://github.com/tuanpmt/ESP8266MQTTClient)
- [esp-knx-ip](https://github.com/envy/esp-knx-ip)
- [esp-mqtt-arduino](https://github.com/i-n-g-o/esp-mqtt-arduino)
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
- reloxx13 for his [SonWeb](https://github.com/reloxx13/SonWEB) management tool
- Joachim Banzhaf for his TSL2561 library and driver
- Gijs Noorlander for his MHZ19 and SenseAir drivers
- Emontnemery for his HomeAssistant Discovery concept and many code tuning tips
- Aidan Mountford for his HSB support
- Daniel Ztolnai for his Serial Bridge implementation
- Gerhard Mutz for his SGP30 and Sunrise/Sunset driver
- Nuno Ferreira for his HC-SR04 driver
- Adrian Scillato for his (security)fixes and implementing and maintaining KNX
- Raymond Mouthaan for managing Wemos Wiki information
- Norbert Richter, Frogmore42 and Jason2866 for providing many issue answers
- Many more providing Tips, Pocs or PRs

### License

This program is licensed under GPL-3.0
