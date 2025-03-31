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

This release will be supported from ESP32/Arduino library Core version **v3.1.1.250203**.

Support of ESP8266 Core versions before 2.7.8 and ESP32 Core versions before v3.1.1.250203 have been removed.

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
- http://ota.tasmota.com/tasmota/release-14.5.0

The latter links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota/release/tasmota.bin.gz``

### ESP32, ESP32-C2, ESP32-C3, ESP32-C6, ESP32-S2 and ESP32-S3 based
The following binary downloads have been compiled with ESP32/Arduino library core version **v3.1.1.250203**.

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
- https://ota.tasmota.com/tasmota32/release-14.5.0

The latter links can be used for OTA upgrades too like ``OtaUrl https://ota.tasmota.com/tasmota32/release/tasmota32.bin``

## Additional information

[List](MODULES.md) of embedded modules.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog v14.5.0.3
### Added
- Extend command `GPIO` with different display options and allowing updating of module GPIO's in one go
- Support Vango Technologies V924x ultralow power, single-phase, power measurement [#23127](https://github.com/arendst/Tasmota/issues/23127)
- Support for HLK-LD2402 24GHz smart wave motion sensor [#23133](https://github.com/arendst/Tasmota/issues/23133)
- Telnet server using command `Telnet <0|1|port>[,<IP filter>]`
- Allow acl in mqtt when client certificate is in use with `#define USE_MQTT_CLIENT_CERT` [#22998](https://github.com/arendst/Tasmota/issues/22998)
- Berry experimental driver for AXP2101 for M5Core2v1.1 [#23039](https://github.com/arendst/Tasmota/issues/23039)
- Berry `tasmota.when_network_up()` and simplified Matter using it [#23057](https://github.com/arendst/Tasmota/issues/23057)
- Berry `introspect.solidified()` to know if a Berry object is solidified or in RAM [#23063](https://github.com/arendst/Tasmota/issues/23063)
- Berry `global.undef()` to undefine a global variable [#23073](https://github.com/arendst/Tasmota/issues/23073)
- Berry load `.tapp` files in `/.extensions/` then in `/` [#23113](https://github.com/arendst/Tasmota/issues/23113)
- Berry `re.dump()` [#23162](https://github.com/arendst/Tasmota/issues/23162)
- Berry `bytes.add()` now accepts 3-bytes values [#23200](https://github.com/arendst/Tasmota/issues/23200)
- Matter prepare for ICD cluster [#23158](https://github.com/arendst/Tasmota/issues/23158)
- LVGL experimental mirroring of display on Web UI [#23041](https://github.com/arendst/Tasmota/issues/23041)
- HASPmota autostart when `pages.jsonl` exists [#23181](https://github.com/arendst/Tasmota/issues/23181)

### Breaking Changed
- Berry remove `Leds.create_matrix` from the standard library waiting for reimplementation [#23114](https://github.com/arendst/Tasmota/issues/23114)

### Changed
- ESP32 Platform from 2025.02.30 to 2025.03.30, Framework (Arduino Core) from v3.1.1.250203 to v3.1.3.250302 and IDF from v5.3.2.250120 to 5.3.2.250228 [#23088](https://github.com/arendst/Tasmota/issues/23088)
- Output of commands `GPIO` and `GPIOs` swapped
- RCSwitch `RCSWITCH_SEPARATION_LIMIT` from 4100 to 3600
- GPIOViewer from v1.6.1 to v1.6.2 (No functional change)
- ESP8266 enable FTP for >= 4MB variants [#23120](https://github.com/arendst/Tasmota/issues/23120)
- ESP32 enable webcam version 2 [#18732](https://github.com/arendst/Tasmota/issues/18732)
- Berry update flasher for Sonoff ZBBridge Pro [#23136](https://github.com/arendst/Tasmota/issues/23136)
- Berry `re` now accepts `bytes()` as precompiled patterns, added `re.compilebytes()` [#23149](https://github.com/arendst/Tasmota/issues/23149)
- LVGL, prepare for HASPmota theme, change: no-grow when clicked, DPI set to 160 [#23040](https://github.com/arendst/Tasmota/issues/23040)
- LVGL Mirroring add checkbox to enable/disable the feature (in the iterim for a better solution) [#23047](https://github.com/arendst/Tasmota/issues/23047)
- Leds Panel add checkbox to enable/disable the feature (in the iterim for a better solution) [#23048](https://github.com/arendst/Tasmota/issues/23048)

### Fixed
- Too many zeros in RCSwitch received data regression from v14.4.1.4 [#23050](https://github.com/arendst/Tasmota/issues/23050)
- INA226 driver fixes [#23197](https://github.com/arendst/Tasmota/issues/23197)
- ESP32 receive incomplete serial data over 128 bytes [#23156](https://github.com/arendst/Tasmota/issues/23156)
- ESP32 intermittent exception on WiFi AP cannot be reached [#23115](https://github.com/arendst/Tasmota/issues/23115)
- ESP32-C3 WiFi sleep [#23096](https://github.com/arendst/Tasmota/issues/23096)
- Berry prevent `import` from hiding a solidified class [#23112](https://github.com/arendst/Tasmota/issues/23112)

### Removed
