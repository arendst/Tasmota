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

This release will be supported from ESP32/Arduino library Core version **v3.3.8**.

Support of ESP8266 Core versions before 2.7.8 and ESP32 Core versions before v3.3.8 have been removed.

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
- http://ota.tasmota.com/tasmota/release-15.4.0

The latter links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota/release/tasmota.bin.gz``

### ESP32, ESP32-C2, ESP32-C3, ESP32-C5, ESP32-C6, ESP32-P4, ESP32-S2 and ESP32-S3 based
The following binary downloads have been compiled with ESP32/Arduino library core version **v3.3.8**.

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
- https://ota.tasmota.com/tasmota32/release-15.4.0

The latter links can be used for OTA upgrades too like ``OtaUrl https://ota.tasmota.com/tasmota32/release/tasmota32.bin``

## Additional information

[List](MODULES.md) of embedded modules.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog v15.4.0.2
### Added
- Command `SetOption [0..2]` to display SetOption values
- Command `WcResolution 0..24` increasing camera max resolution from 14 to 24
- Support for hostname generation using single-specifier Format() patterns [#24731](https://github.com/arendst/Tasmota/issues/24731)
- Support for Modbus RX Enable GPIO [#24726](https://github.com/arendst/Tasmota/issues/24726)
- Support for multi-byte chars like emojis (💡) in light device toggle buttons [#24482](https://github.com/arendst/Tasmota/issues/24482)
- Support for M5Stack Atom S3R drivers [#24747](https://github.com/arendst/Tasmota/issues/24747)
- Support for uDisplay ST7305 [#24738](https://github.com/arendst/Tasmota/issues/24738)
- Support for hosted MCU different from esp32c6
- MiElHVAC extend support of AirDirection control [#24675](https://github.com/arendst/Tasmota/issues/24675)
- ESP32 VID6608 hardware RMT support for Automotive gauge driver [#24759](https://github.com/arendst/Tasmota/issues/24759)
- Berry add support for pre-processor [#24679](https://github.com/arendst/Tasmota/issues/24679)
- Berry transpose C defines to Berry in `tasmota_defines_for_berry.be` [#24680](https://github.com/arendst/Tasmota/issues/24680)
- Berry RGBW white blend and 10-bit gamma support to Berry LED pixel rendering [#24750](https://github.com/arendst/Tasmota/issues/24750)
- Berry manual tool to compare and verify solidification between C and Python [#24754](https://github.com/arendst/Tasmota/issues/24754)

### Breaking Changed

### Changed
- ESP32 Platform from 2025.04.30 to 2026.05.50, Framework (Arduino Core) from v3.1.11 to v3.3.8.260506 and IDF from v5.3.4.260127 to v5.5.4.260407 [#24718](https://github.com/arendst/Tasmota/issues/24718)
- Increase security by inverting state of `define DISABLE_REFERER_CHK` controlling remote HTTP access which is now default off
- ESP8266 wrap printf and replace with stubs reducing flash size by 6k [#24714](https://github.com/arendst/Tasmota/issues/24714)
- Move autoconf repository to `ota.tasmota.com` [#24754](https://github.com/arendst/Tasmota/issues/24754)
- Berry `format()` now uses internal `ext_snprintf_P()` for floating point formatting [#24725](https://github.com/arendst/Tasmota/issues/24725)
- LVGL splash screen uses default Montserrat-14 instead of Montserrat-20 on small screens [#24735](https://github.com/arendst/Tasmota/issues/24735)

### Fixed
- I80 pushColors swap logic for parallel displays [#24766](https://github.com/arendst/Tasmota/issues/24766)
- NeoPool possible overflow/div-zero errors and Hydrolysis module detection [#24724](https://github.com/arendst/Tasmota/issues/24724)
- Seesaw encoder position tracking in light control mode [#24730](https://github.com/arendst/Tasmota/issues/24730)

### Removed
- `USE_UNIVERSAL_TOUCH` no more forced when `USE_UNIVERSAL_DISPLAY` is enabled [#24743](https://github.com/arendst/Tasmota/issues/24743)
