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

This release will be supported from ESP32/Arduino library Core version **v3.1.6**.

Support of ESP8266 Core versions before 2.7.8 and ESP32 Core versions before v3.1.6 have been removed.

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
- http://ota.tasmota.com/tasmota/release-15.1.0

The latter links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota/release/tasmota.bin.gz``

### ESP32, ESP32-C2, ESP32-C3, ESP32-C5, ESP32-C6, ESP32-P4, ESP32-S2 and ESP32-S3 based
The following binary downloads have been compiled with ESP32/Arduino library core version **v3.1.6**.

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
- https://ota.tasmota.com/tasmota32/release-15.1.0

The latter links can be used for OTA upgrades too like ``OtaUrl https://ota.tasmota.com/tasmota32/release/tasmota32.bin``

## Additional information

[List](MODULES.md) of embedded modules.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog v15.1.0.3
### Added
- Support for ESP32-P4 rev.3 [#24146](https://github.com/arendst/Tasmota/issues/24118)
- Support for Analog Gauges [#24153](https://github.com/arendst/Tasmota/issues/24153)
- Commands `DaliSend` and `DaliQuery` allow extended commands with prefix for DeviceType defaulting to DT6
- ESP8266 GPIOViewer memory map if enabled with `#define GV_USE_ESPINFO`
- HostedMCU file update using command `HostedLoad <version>|<filename>`
- Scripter array transfer via UFS [#24060](https://github.com/arendst/Tasmota/issues/24060)
- DALI DT8 RGBWAF color support using Tasmota light control
- NeoPool command `NPReadLSB`, `NPReadMSB`, `NPWriteLSB`, `NWriteMSB` for directly read/write LSB/MSB of 16-bit register [#24083](https://github.com/arendst/Tasmota/issues/24083)
- TLS enabled ECDSA by default for ESP8266 [#24009](https://github.com/arendst/Tasmota/issues/24009)
- WS2812 and Berry animation support for reverse-order LED strip [#24138](https://github.com/arendst/Tasmota/issues/24138)
- Berry `cb.free_cb` for extension manager [#24014](https://github.com/arendst/Tasmota/issues/24014)
- Berry `light.get()` direct access to values [#24033](https://github.com/arendst/Tasmota/issues/24033)
- Berry `gc_heap` and `gc_time` to `tasmota.memory()` [#24054](https://github.com/arendst/Tasmota/issues/24054)
- Berry `tcp.write()` add `offset` and `len` [#24076](https://github.com/arendst/Tasmota/issues/24076)

### Breaking Changed

### Changed
- ESP32 Platform from 2025.11.30 to 2025.11.31, Framework (Arduino Core) from v3.1.5 to v3.1.6 and IDF from v5.3.4.250826 to v5.3.4.251110 [#24146](https://github.com/arendst/Tasmota/issues/24118)
- LVGL library from v9.3.0 to v9.4.0 [#24028](https://github.com/arendst/Tasmota/issues/24028)
- JPEGDEC library from v1.8.3 to v1.8.4 [#24120](https://github.com/arendst/Tasmota/issues/24120)
- GPIOViewer from v1.6.3 to v1.7.0
- Refactored library UDisplay [#24007](https://github.com/arendst/Tasmota/issues/24007)
- Increased filesystem file name size from 48 to 50 characters

### Fixed
- InfluxDb receives IPAddress as a value regression from v15.0.1.3 [#24031](https://github.com/arendst/Tasmota/issues/24031)
- Scripter UDP and switch case [#24060](https://github.com/arendst/Tasmota/issues/24060)
- TuyaMCU v1 soft lock when WIFI_SELECT / WIFI_RESET is initiated [#24063](https://github.com/arendst/Tasmota/issues/24063)
- PCA9685 V2 driver PWMTO fading logic and overflow [#24159](https://github.com/arendst/Tasmota/issues/24159)
- DALI protocol errors
- TLS fix ECDSA and add `SetOption165 1` to enable ECDSA in addition to RSA [#24000](https://github.com/arendst/Tasmota/issues/24000)
- ESP32-P4 Hosted MCU updated to v2.6.6 solving WiFi boot issues [#24146](https://github.com/arendst/Tasmota/issues/24118)
- ESP32-Solo1 using pre-compiled Arduino libraries [#24146](https://github.com/arendst/Tasmota/issues/24118)
- Extension Manager exception when `OtaUrl` is not defined or invalid
- Extension Manager Light Theme support and Extensions input field control
- HASPmota exception in `cpicker` (colorwheel) [#24010](https://github.com/arendst/Tasmota/issues/24010)
- HASPmota `scale` and `angle` for images (#24089)[#24089](https://github.com/arendst/Tasmota/issues/24089)

### Removed
