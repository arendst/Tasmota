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

This release will be supported from ESP32/Arduino library Core version **v3.1.0.241206**.

Support of ESP8266 Core versions before 2.7.8 and ESP32 Core versions before v3.1.0.241206 have been removed.

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
- http://ota.tasmota.com/tasmota/release-14.4.1

The latter links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota/release/tasmota.bin.gz``

### ESP32, ESP32-C2, ESP32-C3, ESP32-C6, ESP32-S2 and ESP32-S3 based
The following binary downloads have been compiled with ESP32/Arduino library core version **v3.1.0.241206**.

- **tasmota32.bin** = The Tasmota version with most drivers including additional sensors and KNX for 4M+ flash.  **RECOMMENDED RELEASE BINARY**
- **tasmota32solo1.bin** = The Tasmota version with most drivers including additional sensors and KNX for single core ESP32 and 4M+ flash.
- **tasmota32s2.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-S2 with serial and 4M+ flash.
- **tasmota32s2cdc.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-S2 with serial over embedded USB CDC only and 4M+ flash.
- **tasmota32s3.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-S3 with USB HWCDC and fallback to serial and 4M+ flash.
- **tasmota32c2.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-C2 with serial and 4M+ flash.
- **tasmota32c3.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-C2 with USB HWCDC and fallback to serial and 4M+ flash.
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
- https://ota.tasmota.com/tasmota32/release-14.4.1

The latter links can be used for OTA upgrades too like ``OtaUrl https://ota.tasmota.com/tasmota32/release/tasmota32.bin``

## Additional information

[List](MODULES.md) of embedded modules.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog v14.4.1.3
### Added
- Command `SetOption163 1` to disable display of Device name in GUI header
- Command `FileLog 0..4` to enable logging to filesystem using up to 16 rotating log files of 100kB (`#define FILE_LOG_SIZE 100`)
- Command `FileLog 10..14` to enable logging to filesystem using up to 16 log files of 100kB (`#define FILE_LOG_SIZE 100`)
- Command I2sLoop [#22807](https://github.com/arendst/Tasmota/issues/22807)
- Support for PCF85063 RTC [#22727](https://github.com/arendst/Tasmota/issues/22727)
- Support for Senseair S88 CO2 sensor [#22733](https://github.com/arendst/Tasmota/issues/22733)
- Support for ESP32 Two-Wire Automotive Interface (TWAI) or Controller Area Network (CAN) busses
- I2S AAC support for web radio [#22787](https://github.com/arendst/Tasmota/issues/22787)
- I2S Opus stream and file support for opus/aac [#22795](https://github.com/arendst/Tasmota/issues/22795)
- ESP32 TasmotaLED change dynamically the number of pixels [#22754](https://github.com/arendst/Tasmota/issues/22754)
- ESP32 expand `Pixels` with reverse, height and alternate [#22755](https://github.com/arendst/Tasmota/issues/22755)
- Display template for Waveshare ESP32-C6 LCD 1.47 [#22863](https://github.com/arendst/Tasmota/issues/22863)
- Berry `animate.crenel` primitive [#22673](https://github.com/arendst/Tasmota/issues/22673)
- Berry scroll to Leds_matrix [#22693](https://github.com/arendst/Tasmota/issues/22693)
- Berry add unicode encoding to string parsing [#22713](https://github.com/arendst/Tasmota/issues/22713)
- Berry `tasmota.int(v, min, max)` function [#22723](https://github.com/arendst/Tasmota/issues/22723)
- Berry driver for M5Stack 8encoder [#22724](https://github.com/arendst/Tasmota/issues/22724)
- Berry add light_pixels values to `tasmota.settings` [#22762](https://github.com/arendst/Tasmota/issues/22762)
- Berry add `bytes().appendhex()` [#22767](https://github.com/arendst/Tasmota/issues/22767)
- Berry WS2812 real-time Leds panel as app [#22788](https://github.com/arendst/Tasmota/issues/22788)
- Berry `serial.read()` read only `n` bytes [#22835](https://github.com/arendst/Tasmota/issues/22835)
- Berry `tasmota.global.tele_period` and `tasmota.settings.tele_period` [#22865](https://github.com/arendst/Tasmota/issues/22865)

### Breaking Changed

### Changed
- ESP32 Platform from 2024.12.30 to 2025.01.31, Framework (Arduino Core) from v3.1.0.241206 to v3.1.1.250109 and IDF to 5.3.2 [#22832](https://github.com/arendst/Tasmota/issues/22832)
- GPIOViewer from v1.5.6 to v1.6.0 (No functional change)
- Postpone save_data during light animation when fade is Off
- Allow negative values for AdcParam/AdcGpio INPUT, TEMP and RANGE parameters [#22809](https://github.com/arendst/Tasmota/issues/22809)
- Command `Pixels` has backwards compatible arguments fixing #22755 [#22791](https://github.com/arendst/Tasmota/issues/22791)
- ESP32 disable PSRAM check (and on restart some relay toggles) with `#define DISABLE_PSRAMCHECK true` [#21266](https://github.com/arendst/Tasmota/issues/21266)
- Berry bit-shift operators to `int64` [#22709](https://github.com/arendst/Tasmota/issues/22709)
- HASPmota use 'roboto.ttf' for automatic sizing of default font [#22697](https://github.com/arendst/Tasmota/issues/22697)
- HASPmota add 'tag' attribute for free-form JSON [#22698](https://github.com/arendst/Tasmota/issues/22698)
- HASPmota support for `tabview` [#22707](https://github.com/arendst/Tasmota/issues/22707)

### Fixed
- Shutter discovery message regression from v14.4.1 [#22730](https://github.com/arendst/Tasmota/issues/22730)
- ESP32-Cx compilation fails on Windows [#22832](https://github.com/arendst/Tasmota/issues/22832)
- Sonoff SPM `PowerOnState` overrules `SSPMPowerOnState` in mixed 4Relay setup with 4Relay version 1.0.0
- Webcam compilation with `define USE_WEBCAM` but without `define ENABLE_RTSPSERVER` [#22686](https://github.com/arendst/Tasmota/issues/22686)
- Berry Zigbee fix wrong attributes [#22684](https://github.com/arendst/Tasmota/issues/22684)
- Berry walrus operator [#22685](https://github.com/arendst/Tasmota/issues/22685)
- LVGL updated `Antiburn.tapp` [#22699](https://github.com/arendst/Tasmota/issues/22699)
- Matter Air Quality sensor [#22708](https://github.com/arendst/Tasmota/issues/22708)

### Removed
