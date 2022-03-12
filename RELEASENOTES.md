<img src="https://github.com/arendst/Tasmota/blob/master/tools/logo/TASMOTA_FullLogo_Vector.svg" alt="Logo" align="right" height="76"/>

# RELEASE NOTES

## Migration Information

**This version removes support for direct migration from versions before v8.1.0 (Doris)**

See [migration path](https://tasmota.github.io/docs/Upgrading#migration-path) for instructions how to migrate to a major version.

**Do not upgrade from minimal to minimal version. It will most likely fail at some point and will require flashing via serial.** If you do have to use minimal versions, always OTA to a full version of the same release before applying next minimal version.

Pay attention to the following version breaks due to dynamic settings updates:

1. Migrate to **Sonoff-Tasmota 3.9.x**
2. Migrate to **Sonoff-Tasmota 4.x**
3. Migrate to **Sonoff-Tasmota 5.14**
4. Migrate to **Sonoff-Tasmota 6.7.1** (http://ota.tasmota.com/tasmota/release_6.7.1/sonoff.bin) - NOTICE underscore as a dash is not supported in older versions
5. Migrate to **Tasmota 7.2.0** (http://ota.tasmota.com/tasmota/release-7.2.0/tasmota.bin)

--- Major change in parameter storage layout ---

6. Migrate to **Tasmota 8.5.1** (http://ota.tasmota.com/tasmota/release-8.5.1/tasmota.bin)

--- Major change in internal GPIO function representation ---

7. Migrate to **Tasmota 9.1** (http://ota.tasmota.com/tasmota/release-9.1.0/tasmota.bin.gz)
8. Upgrade to **latest release** (http://ota.tasmota.com/tasmota/release/tasmota.bin.gz)

While fallback or downgrading is common practice it was never supported due to Settings additions or changes in newer releases. Starting with release **v9.1.0 Imogen** the internal GPIO function representation has changed in such a way that fallback is only possible to the latest GPIO configuration before installing **v9.1.0**.

## Supported Core versions

This release will be supported from ESP8266/Arduino library Core version **2.7.4.9** due to reported security and stability issues on previous Core version. This will also support gzipped binaries.

This release will be supported from ESP32/Arduino library Core version **2.0.2.1**.

Support of ESP8266 Core versions before 2.7.4.9 and ESP32 Core versions before 2.0.2 have been removed.

## Support of TLS

In addition to TLS using fingerprints now also user supplied CA certs, AWS IoT and Azure IoT is supported. Read [full documentation](https://tasmota.github.io/docs/AWS-IoT)

## Initial configuration tools

For initial configuration this release supports Webserver based **WifiManager** or **Serial** based command interface.

## Initial installation

Easy initial installation of Tasmota can be performed using the [Tasmota WebInstaller](https://tasmota.github.io/install/).

## Provided Binary Downloads

### ESP8266 or ESP8285 based
The following binary downloads have been compiled with ESP8266/Arduino library core version **2.7.4.9**.

- **tasmota.bin** = The Tasmota version with most drivers for 1M+ flash. **RECOMMENDED RELEASE BINARY**
- **tasmota-AF.bin** to **tasmota-VN.bin** = The Tasmota version in different languages for 1M+ flash.
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
- http://ota.tasmota.com/tasmota/release-11.0.0

The latter links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota/release/tasmota.bin.gz``

### ESP32 based
The following binary downloads have been compiled with ESP32/Arduino library core version **2.0.2.1**.

- **tasmota32.bin** = The Tasmota version with most drivers including additional sensors and KNX for 4M+ flash.  **RECOMMENDED RELEASE BINARY**
- **tasmota32c3.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-C3 and 4M+ flash.
- **tasmota32solo1.bin** = The Tasmota version with most drivers including additional sensors and KNX for single core ESP32 and 4M+ flash.
- **tasmota32-AF.bin** to **tasmota32-VN.bin** = The Tasmota version in different languages for 4M+ flash.
- **tasmota32-ir.bin** = The InfraRed Receiver and transmitter version allowing all available protocols provided by library IRremoteESP8266 but without most other features for 4M+ flash.
- **tasmota32-display.bin** = The Display version without Energy Monitoring but adds display support for 4M+ flash.
- **tasmota32-lvgl.bin** = The LVGL version adds Light and Versatile Graphics Library (LVGL) display support for 4M+ flash.
- **tasmota32-webcam.bin** = The Webcam version adds webcam support for 4M+ flash.
- **tasmota32-bluetooth.bin** = The Bluetooth version adds BLE support for 4M+ flash.

Latest released binaries can be downloaded from
- https://github.com/arendst/Tasmota-firmware/tree/main/release-firmware
- http://ota.tasmota.com/tasmota32/release

Historical binaries can be downloaded from
- http://ota.tasmota.com/tasmota32/release-11.0.0

The latter links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota32/release/tasmota32.bin``

## Additional information

[List](MODULES.md) of embedded modules.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog v11.0.0.4
### Added
- Command ``SspmMap 0`` to reset Sonoff SPM default mapping
- Command ``TcpConnect <port><ip_address>`` to add client connection mode [#14874](https://github.com/arendst/Tasmota/issues/14874)
- Command ``RfTimeout 100..60000`` to disable duplicate RfReceive. Default 1000 [#15061](https://github.com/arendst/Tasmota/issues/15061)
- Commands ``Sensor12 D0 .. D5, S0 .. S5`` allowing differential or single-ended modes [#15001](https://github.com/arendst/Tasmota/issues/15001)
- NeoPool commands ``NPpHMin``, ``NPpHMax``, ``NPpH``, ``NPRedox``, ``NPHydrolysis``, ``NPIonization``, ``NPChlorine`` and ``NPControl`` [#15015](https://github.com/arendst/Tasmota/issues/15015)
- NeoPool system voltages display
- TasmotaSerial implement ``end()``
- Support for ADE7880 3 phase energy monitor as used in Shelly 3EM [#13515](https://github.com/arendst/Tasmota/issues/13515)
- Support for PCF85363 RTC as used in Shelly 3EM (#13515)
- Full DS3231 integration and synchronisation when using UBX (=GPS), NTP or manual time
- ESP32 Berry always enable rules
- ESP32 Berry bootloop protection
- ESP32 support for BLE Mi scale V1 [#13517](https://github.com/arendst/Tasmota/issues/13517)
- ESP32 integrate Homekit in Bluetooth binary [#14818](https://github.com/arendst/Tasmota/issues/14818)
- ESP32 Berry virtual Alexa hue device [#14833](https://github.com/arendst/Tasmota/issues/14833)
- ESP32 TasmotaSerial uart mapping to support multiple ``begin()`` and implement ``getUart()`` [#14981](https://github.com/arendst/Tasmota/issues/14981)

### Breaking Changed

### Changed
- Adafruit BusIO library from v1.0.10 to v1.11.0
- TasmotaSerial library from v3.4.0 to v3.5.0 [#14981](https://github.com/arendst/Tasmota/issues/14981)
- Sonoff SPM increase max number of relays supported to 32 (8 SPM-4Relay modules)
- Extent number of pulsetimers from 8 to 32 [#8266](https://github.com/arendst/Tasmota/issues/8266)
- ESP32 Arduino core from v2.0.2.2 to v2.0.2.3
- ESP32 LVGL library from v8.1.0 to v8.2.0
- ESP32 NimBLE library from v1.3.3 to v1.3.6
- ESP32 update the internal Berry type system to sync with Skiars Berry repository. No expected impact on code, but .bec files need to be generated again [#14811](https://github.com/arendst/Tasmota/issues/14811)

### Fixed
- SSPM energy yesterday when zero
- GPIO OptionE1 selection regression [#14821](https://github.com/arendst/Tasmota/issues/14821)
- BL0939, BL0940 and BL0942 energy monitoring buffer miscompares resulting in wrong daily energy values regression from v9.5.0.8 [#14829](https://github.com/arendst/Tasmota/issues/14829)
- Wiegand 34-bit rfid reading and presentation [#14834](https://github.com/arendst/Tasmota/issues/14834)
- Orno WE517 power meter phase 2 current reactive [#14841](https://github.com/arendst/Tasmota/issues/14841)

### Removed
