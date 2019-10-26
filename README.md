<img src="/tools/logo/TASMOTA_FullLogo_Vector.svg" alt="Logo" align="right" height="76"/>

# Sonoff-Tasmota
Alternative firmware for _ESP8266 based devices_ like [iTead](https://www.itead.cc/) _**Sonoff**_ with **web UI, rules and timers, OTA updates, custom device templates and sensor support**. Allows control over **MQTT**, **HTTP**, **Serial** and **KNX** for integrations with smart home systems. Written for Arduino IDE and PlatformIO.

[![GitHub version](https://img.shields.io/github/release/arendst/Sonoff-Tasmota.svg)](https://github.com/arendst/Sonoff-Tasmota/releases/latest)
[![GitHub download](https://img.shields.io/github/downloads/arendst/Sonoff-Tasmota/total.svg)](https://github.com/arendst/Sonoff-Tasmota/releases/latest)
[![License](https://img.shields.io/github/license/arendst/Sonoff-Tasmota.svg)](https://github.com/arendst/Sonoff-Tasmota/blob/master/LICENSE.txt)
[![Chat](https://img.shields.io/discord/479389167382691863.svg)](https://discord.gg/Ks2Kzd4)

If you like **Sonoff-Tasmota**, give it a star, or fork it, and contribute!

[![GitHub stars](https://img.shields.io/github/stars/arendst/Sonoff-Tasmota.svg?style=social&label=Star)](https://github.com/arendst/Sonoff-Tasmota/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/arendst/Sonoff-Tasmota.svg?style=social&label=Fork)](https://github.com/arendst/Sonoff-Tasmota/network)
[![donate](https://img.shields.io/badge/donate-PayPal-blue.svg)](https://paypal.me/tasmota)

See [RELEASENOTES.md](https://github.com/arendst/Sonoff-Tasmota/blob/master/RELEASENOTES.md) for release information.

In addition to the [release webpage](https://github.com/arendst/Sonoff-Tasmota/releases/latest) the binaries can also be downloaded from http://thehackbox.org/tasmota/release/

## Development
[![Dev Version](https://img.shields.io/badge/development%20version-v6.7.1.x-blue.svg)](https://github.com/arendst/Sonoff-Tasmota)
[![Download Dev](https://img.shields.io/badge/download-development-yellow.svg)](http://thehackbox.org/tasmota/)
[![Build Status](https://img.shields.io/travis/arendst/Sonoff-Tasmota.svg)](https://travis-ci.org/arendst/Sonoff-Tasmota)

See [sonoff/_changelog.ino](https://github.com/arendst/Sonoff-Tasmota/blob/development/sonoff/_changelog.ino) for detailed change information.

Unless your Tasmota powered device exhibits a problem or you need to make use of a feature that is not available in the Tasmota version currently installed on your device, leave your device alone - it works so don't make unnecessary changes! If the release version (i.e., the master branch) exhibits unexpected behaviour for your device and configuration, you should upgrade to the latest development version instead to see if your problem is resolved as some bugs in previous releases or development builds may already have been resolved.

The Tasmota development codebase is checked every 1-2 hours for changes. If new commits have been merged and they compile successfuly, new binary files for every variant (excluding non-English languages) will be posted at http://thehackbox.org/tasmota/ (this web address can be used for OTA updates too). The last compiled commit number is also indicated on the same page. It is important to note that these binaries are based on the current development codebase. These commits are tested as much as is possible and are typically quite stable. However, it is infeasible to test on the hundreds of different types of devices with all the available configuration options permitted.

Note that there is a chance, as with any upgrade, that the device may not function as expected. You must always account for the possibility that you may need to flash the device via the serial programming interface if the OTA upgrade fails. Even with the master release, you should always attempt to test the device or a similar prototype before upgrading a device which is in production or is hard to reach. And, as always, make a backup of the device configuration before beginning any firmware update.

## Disclaimer
:warning: **DANGER OF ELECTROCUTION** :warning:

An ESP82xx Wi-Fi device is not a toy. It uses Mains AC so there is a danger of electrocution if not installed properly. If you don't know how to install it, please call an electrician. Remember: _**SAFETY FIRST**_. It is not worth risk to yourself, your family, and your home if you don't know exactly what you are doing. Never try to flash a device using the serial programming interface while it is connected to MAINS AC.

We don't take any responsibility nor liability for using this software nor for the installation or any tips, advice, videos, etc. given by any member of this site or any related site.

## Note
Please do not ask to add new devices unless it requires additional code for new features. If the device is not listed as a module, try using [Templates](https://github.com/arendst/Sonoff-Tasmota/wiki/Templates) first. If it is not listed in the [Tasmota Device Templates Repository](http://blakadder.github.io/templates) create your own [Template](https://github.com/arendst/Sonoff-Tasmota/wiki/Templates#creating-your-template-).

## Quick Install
Download one of the released binaries from https://github.com/arendst/Sonoff-Tasmota/releases and flash it to your hardware as [documented in the wiki](https://github.com/arendst/Sonoff-Tasmota/wiki/Flashing).

## Important User Compilation Information
If you want to compile Sonoff-Tasmota yourself keep in mind the following:

- Only Flash Mode **DOUT** is supported. Do not use Flash Mode DIO / QIO / QOUT as it might seem to brick your device. See [Wiki](https://github.com/arendst/Sonoff-Tasmota/wiki/Flashing) for background information.
- Sonoff-Tasmota uses a 1M linker script WITHOUT spiffs **1M (no SPIFFS)** for optimal code space. If you compile using ESP/Arduino library 2.3.0 then download the provided new linker script to your Arduino IDE or Platformio base folder. Later versions of ESP/Arduino library already contain the correct linker script. See [Wiki > Prerequisites](https://github.com/arendst/Sonoff-Tasmota/wiki/Prerequisites).
- To make compile time changes to Sonoff-Tasmota it can use the ``user_config_override.h`` file. It assures keeping your settings when you download and compile a new version. To use ``user_config.override.h`` you will have to make a copy of the provided ``user_config_override_sample.h`` file and add your setting overrides. To enable the override file you will need to use a compile define as documented in the ``user_config_override_sample.h`` file.

## Configuration Information
Please refer to the Installation and configuration articles in the [wiki](https://github.com/arendst/Sonoff-Tasmota/wiki).

## Migration Information
See [wiki migration path](https://github.com/arendst/Sonoff-Tasmota/wiki/Upgrade#migration-path) for instructions how to migrate to a major version. Pay attention to the following version breaks due to dynamic settings updates:

1. Migrate to **Sonoff-Tasmota 3.9.x**
2. Migrate to **Sonoff-Tasmota 4.x**
3. Migrate to **Sonoff-Tasmota 5.14**
4. Migrate to **Sonoff-Tasmota 6.x**

## Support Information
<img src="https://github.com/arendst/arendst.github.io/blob/master/media/sonoffbasic.jpg" width="250" align="right" />

For a database of supported devices see [Tasmota Device Templates Repository](https://blakadder.github.io/templates)

See [Wiki](https://github.com/arendst/Sonoff-Tasmota/wiki) for use instructions and how-to's.<br />
See [Community](https://groups.google.com/d/forum/sonoffusers) for forum.<br />
Visit [Discord Chat](https://discord.gg/Ks2Kzd4) for discussions and troubleshooting.

## Contribute
You can contribute to Sonoff-Tasmota by
- providing Pull Requests (Features, Proof of Concepts, Language files or Fixes)
- testing new released features and report issues
- donating to acquire hardware for testing and implementing or out of gratitude
- contributing missing documentation for features and devices on our [Wiki](https://github.com/arendst/Sonoff-Tasmota/wiki)

[![donate](https://img.shields.io/badge/donate-PayPal-blue.svg)](https://paypal.me/tasmota)

## Credits

### Libraries Used
Libraries used with Sonoff-Tasmota are:
- [ESP8266 core for Arduino](https://github.com/esp8266/Arduino)
- [Adafruit CCS811](https://github.com/adafruit/Adafruit_CCS811)
- [Adafruit ILI9341](https://github.com/adafruit/Adafruit_ILI9341)
- [Adafruit LED Backpack](https://github.com/adafruit/Adafruit-LED-Backpack-Library)
- [Adafruit MAX31865](https://github.com/adafruit/Adafruit_MAX31865)
- [Adafruit SGP30](https://github.com/adafruit/Adafruit_SGP30)
- Adafruit based SH1106
- [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306)
- Adafruit based SSD1351
- [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library)
- Arduino Hex Parser
- [ArduinoJson](https://arduinojson.org/)
- AT24C256 I2C eeprom
- [Base64](https://github.com/Densaugeo/base64_arduino)
- [Bear SSL](https://github.com/earlephilhower/bearssl-esp8266.git)
- [Bosch BME680](https://github.com/BoschSensortec/BME680_driver)
- [C2 Programmer](http://app.cear.ufpb.br/~lucas.hartmann/tag/efm8bb1/)
- [esp-epaper-29-ws-20171230-gemu](https://github.com/gemu2015/Sonoff-Tasmota/tree/displays/lib)
- [esp-knx-ip](https://github.com/envy/esp-knx-ip)
- FrogmoreScd30
- FT6236
- [I2Cdevlib](https://github.com/jrowberg/i2cdevlib)
- [IRremoteEsp8266](https://github.com/markszabo/IRremoteESP8266)
- [JaretBurkett ILI9488](https://github.com/jaretburkett/ILI9488)
- [JobaTsl2561](https://github.com/joba-1/Joba_Tsl2561)
- [LinkedList](https://github.com/ivanseidel/LinkedList)
- [Liquid Cristal](https://github.com/marcoschwartz/LiquidCrystal_I2C)
- [MultiChannelGasSensor](http://wiki.seeedstudio.com/Grove-Multichannel_Gas_Sensor/)
- [NeoPixelBus](https://github.com/Makuna/NeoPixelBus)
- [NewPing](https://bitbucket.org/teckel12/arduino-new-ping/wiki/Home)
- [OneWire](https://github.com/PaulStoffregen/OneWire)
- [PubSubClient](https://github.com/knolleary/pubsubclient)
- [rc-switch](https://github.com/sui77/rc-switch)
- TasmotaModbus
- TasmotaSerial
- [Vl53l0x](https://github.com/pololu/vl53l0x-arduino)
- Xlatb Ra8876

### People inspiring me
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
- Gerhard Mutz for multiple sensor & display drivers, Sunrise/Sunset, and scripting
- Nuno Ferreira for his HC-SR04 driver
- Adrian Scillato for his (security)fixes and implementing and maintaining KNX
- Gennaro Tortone for implementing and maintaining Eastron drivers
- Raymond Mouthaan for managing Wemos Wiki information
- Norbert Richter for his decode-config.py tool
- Andre Thomas for providing [thehackbox](http://thehackbox.org/tasmota/) OTA support and daily development builds
- Joel Stein, digiblur and Shantur Rathore for their Tuya research and driver
- Frogmore42 and Jason2866 for providing many issue answers
- Blakadder for editing the wiki and providing template management
- Stephan Hadinger for refactoring light driver, enhancing HueEmulation and Zigbee support
- tmo for designing the official Tasmota logo
- Stefan Bode for his Shutter and Deep sleep drivers
- Many more providing Tips, Wips, Pocs, PRs and Donations

## License

This program is licensed under GPL-3.0
