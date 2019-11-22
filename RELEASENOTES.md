<img src="https://github.com/arendst/Tasmota/blob/master/tools/logo/TASMOTA_FullLogo_Vector.svg" alt="Logo" align="right" height="76"/>

# RELEASE NOTES

### Sonoff-Tasmota is now Tasmota

## Migration Information

See [wiki migration path](https://github.com/arendst/Tasmota/wiki/Upgrading#migration-path) for instructions how to migrate to a major version. Pay attention to the following version breaks due to dynamic settings updates:

1. Migrate to **Sonoff-Tasmota 3.9.x**
2. Migrate to **Sonoff-Tasmota 4.x**
3. Migrate to **Sonoff-Tasmota 5.14**
4. Migrate to **Sonoff-Tasmota 6.x**
5. Migrate to **Tasmota 7.x**

## Supported Core versions

This release will be supported from ESP8266/Arduino library Core version **2.6.1** due to reported security and stability issues on previous Core version.

Although it might still compile on previous Core versions all support will be removed in the near future.

## Support of TLS

To save resources when TLS is enabled mDNS needs to be disabled. In addition to TLS using fingerprints now also user supplied CA certs and AWS IoT is supported. See full documentation on https://github.com/arendst/Tasmota/wiki/AWS-IoT

## Initial configuration tools

For initial configuration this release supports Webserver based **WifiManager** or **Serial** based command interface only. Support for **WPS** and **SmartConfig** has been removed.

## Supported Modules

Supported [hardware modules.](MODULES.md)

Over 500 additional devices are supported using [templates](TEMPLATES.md).

## Provided Binary Downloads

The following binary downloads have been compiled with ESP8266/Arduino library core version **2.6.1**.

- **tasmota.bin** = The Tasmota version with sensors. **RECOMMENDED RELEASE BINARY**
- **tasmota-BG.bin** to **tasmota-TW.bin** = The Tasmota version in different languages.
- **tasmota-basic.bin** = The Basic version without most sensors.
- **tasmota-knx.bin** = The Knx version without some features but adds KNX support.
- **tasmota-sensors.bin** = The Sensors version adds more useful sensors.
- **tasmota-ir** = The InfraRed Receiver and transmitter version allowing all available protocols provided by library IRremoteESP8266 but without most other features.
- **tasmota-display.bin** = The Display version without Energy Monitoring but adds display support.
- **tasmota-minimal.bin** = The Minimal version allows intermediate OTA uploads to support larger versions and does NOT change any persistent parameter. This version **should NOT be used for initial installation**.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog

### Version 7.1.0 Betty

 - Remove update support for versions before 6.0
 - Remove driver xsns_12_ads1115_i2cdev replaced by xsns_12_ads1115
 - Remove most IR protocols from non dedicated IR firmware except NEC, RC5 and RC6
 - Change repository name from Sonoff-Tasmota to Tasmota and all code references from Sonoff to Tasmota
 - Change default GUI to dark theme
 - Change ArduinoSlave to TasmotaSlave
 - Change IRremoteESP8266 library to v2.7.0
 - Change supported PCF8574 I2C address range to 0x20 - 0x26 allowing other I2C devices with address 0x27 to be used at the same time
 - Change supported PCF8574A I2C address range to 0x39 - 0x3F allowing other I2C devices with address 0x38 to be used at the same time
 - Change supported MCP230xx I2C address range to 0x20 - 0x26 allowing other I2C devices with address 0x27 to be used at the same time
 - Change Reset erase end address from as seen by SDK (getFlashChipSize) to full flash size (getFlashChipRealSize)
 - Fix better control of RGB/White when ``SetOption37`` >128, added ``Dimmer1`` and ``Dimmer2`` commands (#6714)
 - Fix random crash caused by UPNP flood
 - Add support for Tuya battery powered devices (#6735)
 - Add support for Honeywell I2C HIH series Humidity and Temperetaure sensor (#6808)
 - Add support for Honeywell HPMA115S0 particle concentration sensor by David Hunt (#6843)
 - Add support for I2C sensor TLS2591 Light Intensity sensor (#6873)
 - Add command ``SetOption73 0/1`` to re-enable HTTP Cross-Origin Resource Sharing (CORS) now default disabled (#6767)
 - Add command ``SetOption74 0/1`` to enable DS18x20 internal pull-up and remove define DS18B20_INTERNAL_PULLUP (#6795)
 - Add command ``SetOption75 0/1`` to switch between grouptopic (0) using fulltopic replacing %topic% or (1) is cmnd/\<grouptopic\> (#6779)
 - Add command ``SetOption76 0/1`` to enable incrementing bootcount when deepsleep is enabled (#6930)
 - Add command ``I2cDriver`` for I2C driver runtime control using document I2CDEVICES.md
 - Add command ``TempOffset -12.6 .. 12.6`` to set global temperature sensor offset (#6958)
 - Add command ``WebColor19`` to control color of Module and Name (#6811)
 - Add command ``WifiPower 0 .. 20.5`` to set Wifi Output Power which will be default set to 17dBm
 - Add frequency to ADE7953 energy monitor as used in Shelly 2.5 by ljakob (#6778)
 - Add hide Alexa objects with friendlyname starting with '$' (#6722, #6762)
 - Add Zigbee command support, considered as v1.0 for full Zigbee support
