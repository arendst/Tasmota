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

This release will be supported from ESP32/Arduino library Core version **3.0.1**.

Support of ESP8266 Core versions before 2.7.6 and ESP32 Core versions before 3.0.1 have been removed.

### Known issues with v14.1.0

Due to the change from ESP32 Arduino Core2/IDF4 to Arduino Core3/IDF5 not all functionality has been restored. The following features are known not to work on ESP32:
- Wifi Range Extender [#21200](https://github.com/arendst/Tasmota/issues/21200)

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
- https://github.com/arendst/Tasmota-firmware/tree/firmware/release-firmware
- http://ota.tasmota.com/tasmota/release

Historical binaries can be downloaded from
- http://ota.tasmota.com/tasmota/release-14.1.0

The latter links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota/release/tasmota.bin.gz``

### ESP32, ESP32-C2, ESP32-C3, ESP32-C6, ESP32-S2 and ESP32-S3 based
The following binary downloads have been compiled with ESP32/Arduino library core version **3.0.1**.

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
- https://ota.tasmota.com/tasmota32/release-14.1.0

The latter links can be used for OTA upgrades too like ``OtaUrl https://ota.tasmota.com/tasmota32/release/tasmota32.bin``

## Additional information

[List](MODULES.md) of embedded modules.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog v14.1.0.2
### Added
- Support for QMP6988 temperature and pressure sensor
- Support for Sonoff WTS01 temperature sensor using SerialBridge in ``SSerialMode 3``
- Extend command ``SetOption147 1`` to disable publish of IRReceived MQTT messages [#21574](https://github.com/arendst/Tasmota/issues/21574)
- Berry solidification of `bytes` instances [#21558](https://github.com/arendst/Tasmota/issues/21558)
- Berry automatic rounding of float to int when calling C mapped functions [#21601](https://github.com/arendst/Tasmota/issues/21601)
- Berry add `math.round` [#21602](https://github.com/arendst/Tasmota/issues/21602)
- Berry `classof` extended to class methods [#21615](https://github.com/arendst/Tasmota/issues/21615)
- HASPmota rounds to nearest int values passed as 'real' [#21599](https://github.com/arendst/Tasmota/issues/21599)
- Matter support for Air Quality sensors [#21559](https://github.com/arendst/Tasmota/issues/21559)
- Matter support for bridged Air Quality [#21597](https://github.com/arendst/Tasmota/issues/21597)
- Matter add internal debug option [#21634](https://github.com/arendst/Tasmota/issues/21634)
- Matter add Fan support (virtual only) [#21637](https://github.com/arendst/Tasmota/issues/21637)
- Matter show event name in logs [#21649](https://github.com/arendst/Tasmota/issues/21649)

### Changed
- ESP8266 platform update from 2024.01.01 to 2024.06.00 [#21668](https://github.com/arendst/Tasmota/issues/21668)
- ESP8266 Framework (Arduino Core) from v2.7.6 to v2.7.7 [#21668](https://github.com/arendst/Tasmota/issues/21668)
- Optional MQTT_TELE_RETAIN to Energy Margins message replaced by ``SensorRetain``
- Display timing splash screen with display modes 1 to 5
- Allow receive or send using SerialBridge
- SerialBridge command ``SSerialSend9`` replaced by ``SSerialMode``
- SML replace vars in descriptor and line [#21622](https://github.com/arendst/Tasmota/issues/21622)
- NeoPool using temperature as only frequently changing value for NPTeleperiod [#21628](https://github.com/arendst/Tasmota/issues/21628)
- NeoPool make compiler setting available by `user_config_override.h` [#21645](https://github.com/arendst/Tasmota/issues/21645)
- ESP32 Core3 platform update from 2024.05.13 to 2024.06.10 [#21569](https://github.com/arendst/Tasmota/issues/21569)
- ESP32 MI32 refactoring, bugfixes, generic device scanning [#21603](https://github.com/arendst/Tasmota/issues/21603)
- ESP32 MI32 improve parser [#21648](https://github.com/arendst/Tasmota/issues/21648)
- Matter refactoring of bridged devices [#21575](https://github.com/arendst/Tasmota/issues/21575)
- Matter filter suffix automatically added for sensors [#21589](https://github.com/arendst/Tasmota/issues/21589)
- Matter support for Rain sensor [#21633](https://github.com/arendst/Tasmota/issues/21633)

### Fixed
- Shutter MQTT on inverted shutter [#21663](https://github.com/arendst/Tasmota/issues/21663)
- Scripter TCP server [#21660](https://github.com/arendst/Tasmota/issues/21660)
- Berry `input()` returns empty string and does not crash [#21565](https://github.com/arendst/Tasmota/issues/21565)
- Matter interverted attributes 0xFFF9 and 0xFFFB [#21636](https://github.com/arendst/Tasmota/issues/21636)
- Matter CASE Sigma1 resumption mode for faster reconnection [#21644](https://github.com/arendst/Tasmota/issues/21644)
- Matter TLV.U8 unsigned encoding [#21672](https://github.com/arendst/Tasmota/issues/21672)
- Matter resumption final ack [#21673](https://github.com/arendst/Tasmota/issues/21673)

### Removed
