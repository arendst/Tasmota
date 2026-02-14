<picture>
  <source media="(prefers-color-scheme: dark)" srcset="./tools/logo/TASMOTA_FullLogo_Vector_White.svg">
  <img alt="Logo" src="./tools/logo/TASMOTA_FullLogo_Vector.svg" align="right" height="76">
</picture>

# RELEASE NOTES

## Migration Information

**This version removes support for direct migration from versions before v8.1.0 (Doris)**

See [migration path](https://tasmota.github.io/docs/Upgrading#migration-path) for instructions how to migrate to a major version.

**Do not upgrade from minimal to minimal version. It will most likely fail at some point and will require flashing via serial.** If you do have to use minimal versions, always OTA to a full version of the same release before applying next minimal version.

Pay attention to the following version breaks due to dynamic settings updates:

1. Migrate to **Sonoff-Tasmota 3.9.x**
2. Migrate to **Sonoff-Tasmota 4.x**
3. Migrate to **Sonoff-Tasmota 5.14** (http://ota.tasmota.com/tasmota/release_5.14.0/sonoff.bin) - NOTICE underscore as a dash is not supported in older versions
4. Migrate to **Sonoff-Tasmota 6.7.1** (http://ota.tasmota.com/tasmota/release_6.7.1/sonoff.bin) - NOTICE underscore as a dash is not supported in older versions
5. Migrate to **Tasmota 7.2.0** (http://ota.tasmota.com/tasmota/release-7.2.0/tasmota.bin)

--- Major change in parameter storage layout ---

6. Migrate to **Tasmota 8.5.1** (http://ota.tasmota.com/tasmota/release-8.5.1/tasmota.bin)

--- Major change in internal GPIO function representation ---

7. Migrate to **Tasmota 9.1** (http://ota.tasmota.com/tasmota/release-9.1.0/tasmota.bin.gz)
8. Upgrade to **latest release** (http://ota.tasmota.com/tasmota/release/tasmota.bin.gz)

While fallback or downgrading is common practice it was never supported due to Settings additions or changes in newer releases. Starting with release **v9.1.0 Imogen** the internal GPIO function representation has changed in such a way that fallback is only possible to the latest GPIO configuration before installing **v9.1.0**.

## Supported Core versions

This release will be supported from ESP8266/Arduino library Core version **2.7.8** due to reported security and stability issues on previous Core version. This will also support gzipped binaries.

This release will be supported from ESP32/Arduino library Core version **v3.1.10**.

Support of ESP8266 Core versions before 2.7.8 and ESP32 Core versions before v3.1.10 have been removed.

## Initial configuration tools

For initial configuration this release supports Webserver based **WifiManager** or **Serial** based command interface.

## Initial installation

Easy initial installation of Tasmota can be performed using the [Tasmota WebInstaller](https://tasmota.github.io/install/).

## Provided Binary Downloads

### ESP8266 or ESP8285 based
The following binary downloads have been compiled with ESP8266/Arduino library core version **2.7.8**.

- **tasmota.bin** = The Tasmota version with most drivers for 1M+ flash. **RECOMMENDED RELEASE BINARY**
- **tasmota-4M.bin** = The Tasmota version with most drivers and filesystem for 4M+ flash.
- **tasmota-AD.bin** to **tasmota-VN.bin** = The Tasmota version in different languages for 1M+ flash.
- **tasmota-lite.bin** = The Lite version without most drivers and sensors for 1M+ flash.
- **tasmota-knx.bin** = The Knx version without some features but adds KNX support for 1M+ flash.
- **tasmota-sensors.bin** = The Sensors version adds more useful sensors for 1M+ flash.
- **tasmota-ir.bin** = The InfraRed Receiver and transmitter version allowing all available protocols provided by library IRremoteESP8266 but without most other features for 1M+ flash.
- **tasmota-display.bin** = The Display version without Energy Monitoring but adds display support for 1M+ flash.
- **tasmota-zbbridge.bin** = The dedicated Sonoff Zigbee Bridge version for 2M+ flash.
- **tasmota-zigbee.bin** = The dedicated cc25xx Zigbee Bridge version for 4M+ flash.

Above binaries are also available as gzipped version allowing faster uploads.

Latest released binaries can be downloaded from
- https://github.com/arendst/Tasmota-firmware/tree/firmware/release-firmware
- http://ota.tasmota.com/tasmota/release

Historical binaries can be downloaded from
- http://ota.tasmota.com/tasmota/release-15.2.0

The latter links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota/release/tasmota.bin.gz``

### ESP32, ESP32-C2, ESP32-C3, ESP32-C5, ESP32-C6, ESP32-P4, ESP32-S2 and ESP32-S3 based
The following binary downloads have been compiled with ESP32/Arduino library core version **v3.1.10**.

- **tasmota32.bin** = The Tasmota version with most drivers including additional sensors and KNX for 4M+ flash.  **RECOMMENDED RELEASE BINARY**
- **tasmota32solo1.bin** = The Tasmota version with most drivers including additional sensors and KNX for single core ESP32 and 4M+ flash.
- **tasmota32c2.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-C2 with serial and 4M+ flash.
- **tasmota32c3.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-C3 with USB HWCDC and fallback to serial and 4M+ flash.
- **tasmota32c5.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-C5 with USB HWCDC and fallback to serial and 4M+ flash.
- **tasmota32c6.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-C6 with USB HWCDC and fallback to serial and 4M+ flash.
- **tasmota32p4.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-P4 with USB HWCDC and fallback to serial and 4M+ flash.
- **tasmota32s2.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-S2 with serial and 4M+ flash.
- **tasmota32s2cdc.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-S2 with serial over embedded USB CDC only and 4M+ flash.
- **tasmota32s3.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-S3 with USB HWCDC and fallback to serial and 4M+ flash.
- **tasmota32-AD.bin** to **tasmota32-VN.bin** = The Tasmota version in different languages for 4M+ flash.
- **tasmota32-bluetooth.bin** = The Bluetooth version adds BLE support for 4M+ flash.
- **tasmota32-display.bin** = The Display version without Energy Monitoring but adds display support for 4M+ flash.
- **tasmota32-ir.bin** = The InfraRed Receiver and transmitter version allowing all available protocols provided by library IRremoteESP8266 but without most other features for 4M+ flash.
- **tasmota32-lvgl.bin** = The LVGL version adds Light and Versatile Graphics Library (LVGL) display support for 4M+ flash.
- **tasmota32-nspanel.bin** = The Sonoff NSPanel Smart Scene Wall Switch version with HASPmota display support.
- **tasmota32-webcam.bin** = The Webcam version adds webcam support for 4M+ flash.
- **tasmota32-zbbridgepro.bin** - The Sonoff Zigbee Bridge Pro version with CC2652P firmware load support.

Latest released binaries can be downloaded from
- https://github.com/arendst/Tasmota-firmware/tree/firmware/release-firmware
- https://ota.tasmota.com/tasmota32/release

Historical binaries can be downloaded from
- https://ota.tasmota.com/tasmota32/release-15.2.0

The latter links can be used for OTA upgrades too like ``OtaUrl https://ota.tasmota.com/tasmota32/release/tasmota32.bin``

## Additional information

[List](MODULES.md) of embedded modules.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog v15.2.0.5
### Added
- Support for TCA9554 8-bit I/O expander mutually exclusive with PCA9557
- Support for Adafruit I2C QT Rotary Encoder [#24270](https://github.com/arendst/Tasmota/issues/24270)
- Support SPI bus2 for Ethernet and SDCard [#24433](https://github.com/arendst/Tasmota/issues/24433)
- Improv USB CDC connected devices
- Dingtian `SetOption81 1` to invert input and `SetOption133 1` to invert output [#24364](https://github.com/arendst/Tasmota/issues/24364)
- Zigbee support for `int24` type [#24334](https://github.com/arendst/Tasmota/issues/24334)
- Berry `path.listdir("file.tapp#")` to list directory inside '.tapp' archives [#24367](https://github.com/arendst/Tasmota/issues/24367)

### Changed
- ESP8266 platform update from 2025.10.00 to 2026.01.00 [#24354](https://github.com/arendst/Tasmota/issues/24354)
- ESP32 Platform from 2025.12.30 to 2026.02.30, Framework (Arduino Core) from v3.1.7 to v3.1.10 and IDF from v5.3.4.251205 to v5.3.4.260127 [#24444](https://github.com/arendst/Tasmota/issues/24444)
- Vid6608 library from v1.0.2 to v1.0.3 [#24218](https://github.com/arendst/Tasmota/issues/24218)
- IRremoteESP8266 library from v2.8.6 to v2.8.6-ca474a6 [#24226](https://github.com/arendst/Tasmota/issues/24226)
- Refactor Adafruit Seesaw soil driver [#24270](https://github.com/arendst/Tasmota/issues/24270)
- LM75AD output when no valid reading received from 0 to null [#24263](https://github.com/arendst/Tasmota/issues/24263)
- Dingtian define `DINGTIAN_INPUTS_INVERTED` replaced by `SetOption81` [#24364](https://github.com/arendst/Tasmota/issues/24364)
- Update Zigbee WebUI [#24224](https://github.com/arendst/Tasmota/issues/24224)
- JPEG decoder [#24368](https://github.com/arendst/Tasmota/issues/24368)

### Fixed
- ESP8266 TasmotaSerial flush receive buffer on executing `TasmotaSerial.flush();`
- ESP8266 KNX unwanted reply [#24267](https://github.com/arendst/Tasmota/issues/24267)
- ESP32-P4 unique MQTT Client based on hosted MCU MAC address
- ScrubDNS() function [#23886](https://github.com/arendst/Tasmota/issues/23886)
- GDK101 power on detection [#24242](https://github.com/arendst/Tasmota/issues/24242)
- Udisplay backlight with SPI displays [#24277](https://github.com/arendst/Tasmota/issues/24277)
- Udisplay SPI driver display dimension parameters regression from v15.1.0.1 [#24376](https://github.com/arendst/Tasmota/issues/24376)
- Display ST77xx and Parallel issues introduced after refactor UDisplay [#24368](https://github.com/arendst/Tasmota/issues/24368)
- DALI watchdog exception
- TuyaMCU v1 exception 28 regression from v15.1.0.1 reverted PR24063 [#24220](https://github.com/arendst/Tasmota/issues/24220)
- ESP32 BearSSL hw accelerated SHA1 [#24397](https://github.com/arendst/Tasmota/issues/24397)
- ESP32 BLE not starting [#24240](https://github.com/arendst/Tasmota/issues/24240)
- Zigbee compilation warning in Berry mapping [#24333](https://github.com/arendst/Tasmota/issues/24333)

### Removed
- Berry `animate` to be replaced with `animation` framework [#24241](https://github.com/arendst/Tasmota/issues/24241)
- AlpineJS 2.8.2 - not needed anymore [#24427](https://github.com/arendst/Tasmota/issues/24427)
