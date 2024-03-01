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

This release will be supported from ESP8266/Arduino library Core version **2.7.6** due to reported security and stability issues on previous Core version. This will also support gzipped binaries.

This release will be supported from ESP32/Arduino library Core version **2.0.14**.

Support of ESP8266 Core versions before 2.7.6 and ESP32 Core versions before 2.0.14 have been removed.

## Support of TLS

In addition to TLS using fingerprints now also user supplied CA certs, AWS IoT and Azure IoT is supported. Read [full documentation](https://tasmota.github.io/docs/AWS-IoT)

## Initial configuration tools

For initial configuration this release supports Webserver based **WifiManager** or **Serial** based command interface.

## Initial installation

Easy initial installation of Tasmota can be performed using the [Tasmota WebInstaller](https://tasmota.github.io/install/).

## Provided Binary Downloads

### ESP8266 or ESP8285 based
The following binary downloads have been compiled with ESP8266/Arduino library core version **2.7.6**.

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
- https://github.com/arendst/Tasmota-firmware/tree/main/release-firmware
- http://ota.tasmota.com/tasmota/release

Historical binaries can be downloaded from
- http://ota.tasmota.com/tasmota/release-13.3.0

The latter links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota/release/tasmota.bin.gz``

### ESP32, ESP32-C3, ESP32-S2 and ESP32-S3 based
The following binary downloads have been compiled with ESP32/Arduino library core version **2.0.14**.

- **tasmota32.bin** = The Tasmota version with most drivers including additional sensors and KNX for 4M+ flash.  **RECOMMENDED RELEASE BINARY**
- **tasmota32xy.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-C3/S2/S3 and 4M+ flash.
- **tasmota32xycdc.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-C3/S2/S3 with serial over embedded USB CDC only and 4M+ flash.
- **tasmota32solo1.bin** = The Tasmota version with most drivers including additional sensors and KNX for single core ESP32 and 4M+ flash.
- **tasmota32-AD.bin** to **tasmota32-VN.bin** = The Tasmota version in different languages for 4M+ flash.
- **tasmota32-bluetooth.bin** = The Bluetooth version adds BLE support for 4M+ flash.
- **tasmota32-display.bin** = The Display version without Energy Monitoring but adds display support for 4M+ flash.
- **tasmota32-ir.bin** = The InfraRed Receiver and transmitter version allowing all available protocols provided by library IRremoteESP8266 but without most other features for 4M+ flash.
- **tasmota32-lvgl.bin** = The LVGL version adds Light and Versatile Graphics Library (LVGL) display support for 4M+ flash.
- **tasmota32-nspanel.bin** = The Sonoff NSPanel Smart Scene Wall Switch version with HASPmota display support.
- **tasmota32-webcam.bin** = The Webcam version adds webcam support for 4M+ flash.
- **tasmota32-zbbridgepro.bin** - The Sonoff Zigbee Bridge Pro version with CC2652P firmware load support.

### ESP32-C2 and ESP32-C6 based
The following binary downloads have been compiled with ESP32/Arduino library core version **3.0.0-alpha3**.

- **tasmota32c2-arduino30.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-C2 and 4M+ flash.
- **tasmota32c6cdc-arduino30.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-C6 with serial over embedded USB CDC or hardware USB and 4M+ flash.

Latest released binaries can be downloaded from
- https://github.com/arendst/Tasmota-firmware/tree/main/release-firmware
- https://ota.tasmota.com/tasmota32/release

Historical binaries can be downloaded from
- https://ota.tasmota.com/tasmota32/release-13.3.0

The latter links can be used for OTA upgrades too like ``OtaUrl https://ota.tasmota.com/tasmota32/release/tasmota32.bin``

## Additional information

[List](MODULES.md) of embedded modules.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog v13.4.0.2
### Added
- Experimental support for LoRa
- Support for AMS5915/AMS6915 temperature and pressure sensors [#20814](https://github.com/arendst/Tasmota/issues/20814)
- Show calculated heat index if temperature and humidity is available with ``#define USE_HEAT_INDEX`` [#4771](https://github.com/arendst/Tasmota/issues/4771)
- IR support data larger than 64 bits [#20831](https://github.com/arendst/Tasmota/issues/20831)
- TasMesh support for LWT messages [#20392](https://github.com/arendst/Tasmota/issues/20392)
- QMC5883l check for overflow and scale reading [#20643](https://github.com/arendst/Tasmota/issues/20643)
- Berry explicit error log when memory allocation fails [#20807](https://github.com/arendst/Tasmota/issues/20807)
- Berry `path.rename()` [#20840](https://github.com/arendst/Tasmota/issues/20840)
- HASPmota `p<x>b<y>.delete` to delete an object [#20735](https://github.com/arendst/Tasmota/issues/20735)
- LVGL and HASPmota typicons font [#20742](https://github.com/arendst/Tasmota/issues/20742)

### Breaking Changed
- Drop support for old (insecure) fingerprint format [#20842](https://github.com/arendst/Tasmota/issues/20842)

### Changed
- ESP32 Core3 platform update from 2024.01.12 to 2024.02.10 [#20730](https://github.com/arendst/Tasmota/issues/20730)
- NeoPool webUI pH alarms (4 & 5) completed (#20743)[#20743](https://github.com/arendst/Tasmota/issues/20743)
- Prevent shutter MQTT broadcast with activated ShutterLock [#20827](https://github.com/arendst/Tasmota/issues/20827)
- Berry class `int64` made immutable [#20727](https://github.com/arendst/Tasmota/issues/20727)
- Matter reduce memory usage when reading with wildcards [#20809](https://github.com/arendst/Tasmota/issues/20809)
- LVGL make lv_touch_3_buttons more responsive [#20728](https://github.com/arendst/Tasmota/issues/20728)
- HASPmota fix and improve demo with pixel-perfect fonts [#20734](https://github.com/arendst/Tasmota/issues/20734)
- HASPmota more attributes [#20744](https://github.com/arendst/Tasmota/issues/20744)

### Fixed
- Shutter inverted using internal commands [#20752](https://github.com/arendst/Tasmota/issues/20752)
- ESP32 PWM activity on unconfigured PWM GPIOs [#20732](https://github.com/arendst/Tasmota/issues/20732)
- Berry Memory leak in `import re` [#20823](https://github.com/arendst/Tasmota/issues/20823)
- Berry bug when parsing ternary operator [#20839](https://github.com/arendst/Tasmota/issues/20839)
- HASPmota PSRAM memory leak [#20818](https://github.com/arendst/Tasmota/issues/20818)

### Removed
