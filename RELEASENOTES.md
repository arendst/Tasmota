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

This release will be supported from ESP32/Arduino library Core version **v3.1.3.250411**.

Support of ESP8266 Core versions before 2.7.8 and ESP32 Core versions before v3.1.3.250411 have been removed.

## Support of TLS

In addition to TLS using fingerprints now also user supplied CA certs, AWS IoT and Azure IoT is supported. Read [full documentation](https://tasmota.github.io/docs/AWS-IoT)

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
- http://ota.tasmota.com/tasmota/release-14.6.0

The latter links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota/release/tasmota.bin.gz``

### ESP32, ESP32-C2, ESP32-C3, ESP32-C6, ESP32-S2 and ESP32-S3 based
The following binary downloads have been compiled with ESP32/Arduino library core version **v3.1.3.250411**.

- **tasmota32.bin** = The Tasmota version with most drivers including additional sensors and KNX for 4M+ flash.  **RECOMMENDED RELEASE BINARY**
- **tasmota32solo1.bin** = The Tasmota version with most drivers including additional sensors and KNX for single core ESP32 and 4M+ flash.
- **tasmota32s2.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-S2 with serial and 4M+ flash.
- **tasmota32s2cdc.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-S2 with serial over embedded USB CDC only and 4M+ flash.
- **tasmota32s3.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-S3 with USB HWCDC and fallback to serial and 4M+ flash.
- **tasmota32c2.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-C2 with serial and 4M+ flash.
- **tasmota32c3.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-C3 with USB HWCDC and fallback to serial and 4M+ flash.
- **tasmota32c6.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-C6 with USB HWCDC and fallback to serial and 4M+ flash.
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
- https://ota.tasmota.com/tasmota32/release-14.6.0

The latter links can be used for OTA upgrades too like ``OtaUrl https://ota.tasmota.com/tasmota32/release/tasmota32.bin``

## Additional information

[List](MODULES.md) of embedded modules.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog v14.6.0.2
### Added
- Provide serial upload port from VSC to PIO [#23436](https://github.com/arendst/Tasmota/issues/23436)
- Command `JsonPP 0..7` to enable (>0) JSON Pretty Print on user interfaces and set number of indents
- Command `JsonPP <command>|backlog <command>;...` to enable JSON PP only once
- Support for multi channel AU915-928 LoRaWanBridge by Rob Clark [#23372](https://github.com/arendst/Tasmota/issues/23372)
- Support for LoRaWan Rx1 and Rx2 profiles [#23394](https://github.com/arendst/Tasmota/issues/23394)
- Allow temporary change of DisplayDimmer [#23406](https://github.com/arendst/Tasmota/issues/23406)
- WebUI status line for MQTT and TLS, added `FUNC_WEB_STATUS_LEFT` and `FUNC_WEB_STATUS_RIGHT` event [#23354](https://github.com/arendst/Tasmota/issues/23354)
- WebUI heap status [#23356](https://github.com/arendst/Tasmota/issues/23356)
- Optional Wifi strength indicator in WebUI status line [#23352](https://github.com/arendst/Tasmota/issues/23352)
- Wireguard VPN [#23347](https://github.com/arendst/Tasmota/issues/23347)
- HASPmota `antiburn()` [#23400](https://github.com/arendst/Tasmota/issues/23400)
- HASPmota auto-dimming when no touch [#23425](https://github.com/arendst/Tasmota/issues/23425)

### Breaking Changed

### Changed
- ESP32 Platform from 2025.04.30 to 2025.05.30, Framework (Arduino Core) from v3.1.3.250411 to v3.1.3.250504 and IDF from v5.3.2.250403 to v5.3.3.250501 [#23404](https://github.com/arendst/Tasmota/issues/23404)
- GPIOViewer from v1.6.2 to v1.6.3 (No functional change)
- Allow command `WebRefresh` minimum from 1000 to 400 mSec

### Fixed
- DNS setting with `IPAddress4/5` not persisted [#23426](https://github.com/arendst/Tasmota/issues/23426)
- NimBLE log_level definition conflict [#23366](https://github.com/arendst/Tasmota/issues/23366)
- Berry `bytes().asstring()` now truncates a string if buffer contains NULL [#23311](https://github.com/arendst/Tasmota/issues/23311)
- Berry string literals containing NULL are truncated [#23312](https://github.com/arendst/Tasmota/issues/23312)
- Berry `display.touch_update` wrongly applies resistive calibration [#23363](https://github.com/arendst/Tasmota/issues/23363)
- Berry `introspect.module()` failed to load modules in files [#23376](https://github.com/arendst/Tasmota/issues/23376)
- Matter and mDNS can be enabled at the same time [#23373](https://github.com/arendst/Tasmota/issues/23373)
- Haspmota `haspmota.parse()` page parsing [#23403](https://github.com/arendst/Tasmota/issues/23403)

### Removed
