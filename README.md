![Tasmota logo](/tools/logo/TASMOTA_FullLogo_Vector.svg)

Alternative firmware for [ESP8266](https://en.wikipedia.org/wiki/ESP8266) based devices with **easy configuration using webUI, OTA updates, automation using timers or rules, expandability and entirely local control over MQTT, HTTP, Serial or KNX**.
_Written for PlatformIO with limited support for Arduino IDE._

[![GitHub version](https://img.shields.io/github/release/arendst/Tasmota.svg)](https://github.com/arendst/Tasmota/releases/latest)
[![GitHub download](https://img.shields.io/github/downloads/arendst/Tasmota/total.svg)](https://github.com/arendst/Tasmota/releases/latest)
[![License](https://img.shields.io/github/license/arendst/Tasmota.svg)](LICENSE.txt)
[![Chat](https://img.shields.io/discord/479389167382691863.svg)](https://discord.gg/Ks2Kzd4)
[![Gitpod Ready-to-Code](https://img.shields.io/badge/Gitpod-Ready--to--Code-blue?logo=gitpod)](https://gitpod.io/#https://github.com/arendst/Tasmota)

If you like **Tasmota**, give it a star, or fork it and contribute!

[![GitHub stars](https://img.shields.io/github/stars/arendst/Tasmota.svg?style=social&label=Star)](https://github.com/arendst/Tasmota/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/arendst/Tasmota.svg?style=social&label=Fork)](https://github.com/arendst/Tasmota/network)
[![donate](https://img.shields.io/badge/donate-PayPal-blue.svg)](https://paypal.me/tasmota)

See [RELEASENOTES.md](https://github.com/arendst/Tasmota/blob/master/RELEASENOTES.md) for release information.

In addition to the [release webpage](https://github.com/arendst/Tasmota/releases/latest) the binaries can also be downloaded from http://ota.tasmota.com/tasmota/release/

## Development

[![Dev Version](https://img.shields.io/badge/development%20version-v9.2.x.x-blue.svg)](https://github.com/arendst/Tasmota)
[![Download Dev](https://img.shields.io/badge/download-development-yellow.svg)](http://ota.tasmota.com/tasmota/)
[![Tasmota CI](https://github.com/arendst/Tasmota/workflows/Tasmota%20CI/badge.svg)](https://github.com/arendst/Tasmota/actions?query=workflow%3A%22Tasmota+CI%22)
[![Tasmota ESP32 CI](https://github.com/arendst/Tasmota/workflows/Tasmota%20ESP32%20CI/badge.svg)](https://github.com/arendst/Tasmota/actions?query=workflow%3A%22Tasmota+ESP32+CI%22)
[![Build_firmware](https://github.com/arendst/Tasmota/workflows/Build_firmware/badge.svg)](https://github.com/arendst/Tasmota/actions?query=workflow%3ABuild_firmware)

See [CHANGELOG.md](CHANGELOG.md) for detailed change information.

Unless your Tasmota powered device exhibits a problem or you need to make use of a feature that is not available in the Tasmota version currently installed on your device, leave your device alone - it works so don't make unnecessary changes! If the release version (i.e., the master branch) exhibits unexpected behaviour for your device and configuration, you should upgrade to the latest development version instead to see if your problem is resolved as some bugs in previous releases or development builds may already have been resolved.

The Tasmota development codebase is checked every 1-2 hours for changes. If new commits have been merged and they compile successfuly, new binary files for every variant will be posted at http://ota.tasmota.com/tasmota/ (this web address can be used for OTA updates too). It is important to note that these binaries are based on the current development codebase. These commits are tested as much as is possible and are typically quite stable. However, it is infeasible to test on the hundreds of different types of devices with all the available configuration options permitted.

Note that there is a chance, as with any upgrade, that the device may not function as expected. You must always account for the possibility that you may need to flash the device via the serial programming interface if the OTA upgrade fails. Even with the master release, you should always attempt to test the device or a similar prototype before upgrading a device which is in production or is hard to reach. And, as always, make a backup of the device configuration before beginning any firmware update.

## Disclaimer

:warning: **DANGER OF ELECTROCUTION** :warning:

If your device connects to mains electricity (AC power) there is danger of electrocution if not installed properly. If you don't know how to install it, please call an electrician (***Beware:*** certain countries prohibit installation without a licensed electrician present). Remember: _**SAFETY FIRST**_. It is not worth the risk to yourself, your family and your home if you don't know exactly what you are doing. Never tinker or try to flash a device using the serial programming interface while it is connected to MAINS ELECTRICITY (AC power).

We don't take any responsibility nor liability for using this software nor for the installation or any tips, advice, videos, etc. given by any member of this site or any related site.

## Note

Please do not ask to add new devices unless it requires additional code for new features. If the device is not listed as a module, try using [Templates](https://tasmota.github.io/docs/Templates) first. If it is not listed in the [Tasmota Device Templates Repository](http://blakadder.github.io/templates) create your own [Template](https://tasmota.github.io/docs/Templates#creating-your-template).

## Quick Install
Download one of the released binaries from https://github.com/arendst/Tasmota/releases and flash it to your hardware [using our installation guide](https://tasmota.github.io/docs/Getting-Started).

## Important User Compilation Information
If you want to compile Tasmota yourself keep in mind the following:

- Only Flash Mode **DOUT** is supported. Do not use Flash Mode DIO / QIO / QOUT as it might seem to brick your device.
- Tasmota uses a 1M linker script WITHOUT spiffs **1M (no SPIFFS)** for optimal code space.
- To make compile time changes to Tasmota use the `user_config_override.h` file. It assures keeping your custom settings when you download and compile a new version. You have to make a copy from the provided `user_config_override_sample.h` file and add your setting overrides.

## Configuration Information

Please refer to the installation and configuration articles in our [documentation](https://tasmota.github.io/docs).

## Migration Information

See [wiki migration path](https://tasmota.github.io/docs/Upgrading#migration-path) for instructions how to migrate to a major version. Pay attention to the following version breaks due to dynamic settings updates:

1. Migrate to **Sonoff-Tasmota 3.9.x**
2. Migrate to **Sonoff-Tasmota 4.x**
3. Migrate to **Sonoff-Tasmota 5.14**
4. Migrate to **Sonoff-Tasmota 6.7.1** (http://ota.tasmota.com/tasmota/release-6.7.1/)
5. Migrate to **Tasmota 7.2.0** (http://ota.tasmota.com/tasmota/release-7.2.0/)

--- Major change in parameter storage layout ---

6. Migrate to **Tasmota 8.5.1** (http://ota.tasmota.com/tasmota/release-8.5.1/)

--- Major change in internal GPIO function representation ---

7. Migrate to **Tasmota 9.1** (http://ota.tasmota.com/tasmota/release-9.1.0/)

While fallback or downgrading is common practice it was never supported due to Settings additions or changes in newer releases. Starting with version **v9.0.0.1** the internal GPIO function representation has changed in such a way that fallback is only possible to the latest GPIO configuration before installing **v9.0.0.1**.

## Support Information

<img src="https://user-images.githubusercontent.com/5904370/68332933-e6e5a600-00d7-11ea-885d-50395f7239a1.png" width=150 align="right" />

For a database of supported devices see [Tasmota Device Templates Repository](https://templates.blakadder.com)

If you're looking for support on **Tasmota** there are some options available:

### Documentation

* [Documentation Site](https://tasmota.github.io/docs): For information on how to flash Tasmota, configure, use and expand it
* [FAQ and Troubleshooting](https://tasmota.github.io/docs/FAQ/): For information on common problems and solutions.
* [Commands Information](https://tasmota.github.io/docs/Commands): For information on all the commands supported by Tasmota.

### Support's Community

* [Tasmota Discussions](https://github.com/arendst/Tasmota/discussions): For Tasmota usage questions, Feature Requests and Projects.
* [Tasmota Users Chat](https://discord.gg/Ks2Kzd4): For support, troubleshooting and general questions. You have better chances to get fast answers from members of the Tasmota Community.
* [Search in Issues](https://github.com/arendst/Tasmota/issues): You might find an answer to your question by searching current or closed issues.
* [Software Problem Report](https://github.com/arendst/Tasmota/issues/new?template=Bug_report.md): For reporting problems of Tasmota Software.

## Contribute

You can contribute to Tasmota by
- Providing Pull Requests (Features, Proof of Concepts, Language files or Fixes)
- Testing new released features and report issues
- Donating to acquire hardware for testing and implementing or out of gratitude
- Contributing missing [documentation](https://tasmota.github.io/docs) for features and devices

[![donate](https://img.shields.io/badge/donate-PayPal-blue.svg)](https://paypal.me/tasmota)

## Credits

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
- Erik Montnemery for his HomeAssistant Discovery concept and many code tuning tips
- Federico Leoni for continued HomeAssistant Discovery support
- Aidan Mountford for his HSB support
- Daniel Ztolnai for his Serial Bridge implementation
- Gerhard Mutz for multiple sensor & display drivers, Sunrise/Sunset, and scripting
- Nuno Ferreira for his HC-SR04 driver
- Adrian Scillato for his (security)fixes and implementing and maintaining KNX
- Gennaro Tortone for implementing and maintaining Eastron drivers
- Raymond Mouthaan for managing Wemos Wiki information
- Norbert Richter for his [decode-config.py](https://github.com/tasmota/decode-config) tool
- Andre Thomas for providing [thehackbox](http://thehackbox.org/tasmota/) OTA support and daily development builds
- Joel Stein, digiblur and Shantur Rathore for their Tuya research and driver
- Frogmore42 for providing many issue answers
- Jason2866 for platformio support and providing many issue answers
- Blakadder for managing the new document site and providing template management
- Stephan Hadinger for refactoring light driver, enhancing HueEmulation and Zigbee support
- tmo for designing the official Tasmota logo
- Stefan Bode for his Shutter and Deep sleep drivers
- Jacek Ziółkowski for his [TDM](https://github.com/jziolkowski/tdm) management tool and [Tasmotizer](https://github.com/tasmota/tasmotizer) flashing tool
- Christian Staars for NRF24L01 and HM-10 Bluetooth sensor support
- Paul Diem for UDP Group communication support
- Jörg Schüler-Maroldt for his initial ESP32 port
- Javier Arigita for his thermostat driver
- Many more providing Tips, Wips, Pocs, PRs and Donations

## License

This program is licensed under GPL-3.0
