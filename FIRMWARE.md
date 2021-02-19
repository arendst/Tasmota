![Tasmota logo](https://github.com/arendst/Tasmota/blob/development/tools/logo/TASMOTA_FullLogo_Vector.svg)

Alternative firmware for [ESP8266](https://en.wikipedia.org/wiki/ESP8266) based devices with **easy configuration using webUI, OTA updates, automation using timers or rules, expandability and entirely local control over MQTT, HTTP, Serial or KNX**.
_Written for PlatformIO with limited support for Arduino IDE._

[![GitHub version](https://img.shields.io/github/release/arendst/Tasmota.svg)](https://github.com/arendst/Tasmota/releases/latest)
[![GitHub download](https://img.shields.io/github/downloads/arendst/Tasmota/total.svg)](https://github.com/arendst/Tasmota/releases/latest)
[![License](https://img.shields.io/github/license/arendst/Tasmota.svg)](LICENSE.txt)
[![Chat](https://img.shields.io/discord/479389167382691863.svg)](https://discord.gg/Ks2Kzd4)

If you like **Tasmota**, give it a star, or fork it and contribute!

[![GitHub stars](https://img.shields.io/github/stars/arendst/Tasmota.svg?style=social&label=Star)](https://github.com/arendst/Tasmota/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/arendst/Tasmota.svg?style=social&label=Fork)](https://github.com/arendst/Tasmota/network)
[![donate](https://img.shields.io/badge/donate-PayPal-blue.svg)](https://paypal.me/tasmota)

See [CHANGELOG.md](https://github.com/arendst/Tasmota/blob/development/tasmota/CHANGELOG.md) for changes since last release.

## Development

[![Dev Version](https://img.shields.io/badge/development%20version-v9.3.x.x-blue.svg)](https://github.com/arendst/Tasmota)
[![Download Dev](https://img.shields.io/badge/download-development-yellow.svg)](http://ota.tasmota.com/tasmota/)
[![Tasmota CI](https://github.com/arendst/Tasmota/workflows/Tasmota%20CI/badge.svg)](https://github.com/arendst/Tasmota/actions?query=workflow%3A%22Tasmota+CI%22)
[![Tasmota ESP32 CI](https://github.com/arendst/Tasmota/workflows/Tasmota%20ESP32%20CI/badge.svg)](https://github.com/arendst/Tasmota/actions?query=workflow%3A%22Tasmota+ESP32+CI%22)
[![Build_firmware](https://github.com/arendst/Tasmota/workflows/Build_firmware/badge.svg)](https://github.com/arendst/Tasmota/actions?query=workflow%3ABuild_firmware)


Unless your Tasmota powered device exhibits a problem or you need to make use of a feature that is not available in the Tasmota version currently installed on your device, leave your device alone - it works so don't make unnecessary changes! If the release version (i.e., the master branch) exhibits unexpected behaviour for your device and configuration, you should upgrade to the latest development version instead to see if your problem is resolved as some bugs in previous releases or development builds may already have been resolved.

If new commits have been merged and they compile successfuly, new binary files for every variant will be placed here https://github.com/arendst/Tasmota/tree/firmware/firmware (this URL address can NOT be used for OTA updates) It is important to note that these binaries are based on the current development codebase. These commits are tested as much as is possible and are typically quite stable. However, it is infeasible to test on the hundreds of different types of devices with all the available configuration options permitted.

Note that there is a chance, as with any upgrade, that the device may not function as expected. You must always account for the possibility that you may need to flash the device via the serial programming interface if the OTA upgrade fails. Even with the master release, you should always attempt to test the device or a similar prototype before upgrading a device which is in production or is hard to reach. And, as always, make a backup of the device configuration before beginning any firmware update.

## Disclaimer

:warning: **DANGER OF ELECTROCUTION** :warning:

If your device connects to mains electricity (AC power) there is danger of electrocution if not installed properly. If you don't know how to install it, please call an electrician (***Beware:*** certain countries prohibit installation without a licensed electrician present). Remember: _**SAFETY FIRST**_. It is not worth the risk to yourself, your family and your home if you don't know exactly what you are doing. Never tinker or try to flash a device using the serial programming interface while it is connected to MAINS ELECTRICITY (AC power).

We don't take any responsibility nor liability for using this software nor for the installation or any tips, advice, videos, etc. given by any member of this site or any related site.

## Quick Install
Download one of the binaries https://github.com/arendst/Tasmota/tree/firmware/firmware and flash it to your hardware [using our installation guide](https://tasmota.github.io/docs/Getting-Started).

