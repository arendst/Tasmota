<img src="https://github.com/arendst/Tasmota/blob/master/tools/logo/TASMOTA_FullLogo_Vector.svg" alt="Logo" align="right" height="76"/>

# RELEASE NOTES

## Migration Information

**This version removes support for direct migration from versions before v8.1.0 (Doris)**

See [migration path](https://tasmota.github.io/docs/Upgrading#migration-path) for instructions how to migrate to a major version. Pay attention to the following version breaks due to dynamic settings updates:

**Do not upgrade from minimal to minimal version. It will most likely fail at some point and will require flashing via serial.** If you do have to use minimal versions, always OTA to a full version of the same release before applying next minimal version.

1. Migrate to **Sonoff-Tasmota 3.9.x**
2. Migrate to **Sonoff-Tasmota 4.x**
3. Migrate to **Sonoff-Tasmota 5.14**
4. Migrate to **Sonoff-Tasmota 6.7.1** (http://ota.tasmota.com/tasmota/release-6.7.1/)
5. Migrate to **Tasmota 7.2.0** (http://ota.tasmota.com/tasmota/release-7.2.0/)

--- Major change in parameter storage layout ---

6. Migrate to **Tasmota 8.5.1** (http://ota.tasmota.com/tasmota/release-8.5.1/)

--- Major change in internal GPIO function representation ---

7. Migrate to **Tasmota 9.1** (http://ota.tasmota.com/tasmota/release-9.1.0/)

While fallback or downgrading is common practice it was never supported due to Settings additions or changes in newer releases. Starting with release **v9.1.0 Imogen** the internal GPIO function representation has changed in such a way that fallback is only possible to the latest GPIO configuration before installing **v9.1.0**.

## Supported Core versions

This release will be supported from ESP8266/Arduino library Core version **2.7.4.9** due to reported security and stability issues on previous Core version. This will also support gzipped binaries.

Support of Core versions before 2.7.1 has been removed.

## Support of TLS

In addition to TLS using fingerprints now also user supplied CA certs, AWS IoT and Azure IoT is supported. Read [full documentation](https://tasmota.github.io/docs/AWS-IoT)

## Initial configuration tools

For initial configuration this release supports Webserver based **WifiManager** or **Serial** based command interface only. Support for **WPS** and **SmartConfig** has been removed.

## Initial installation

Easy initial installation of Tasmota can be performed using the [Tasmota WebInstaller](https://arendst.github.io/Tasmota-firmware/).

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
- http://ota.tasmota.com/tasmota/release-10.1.0

The latter links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota/release/tasmota.bin.gz``

### ESP32 based
The following binary downloads have been compiled with ESP32/Arduino library core version **2.0.1.1**.

- **tasmota32.bin** = The Tasmota version with most drivers including additional sensors and KNX for 4M+ flash.  **RECOMMENDED RELEASE BINARY**
- **tasmota32c3.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-C3 and 4M+ flash.
- **tasmota32solo1.bin** = The Tasmota version with most drivers including additional sensors and KNX for single core ESP32 and 4M+ flash.
- **tasmota32-AF.bin** to **tasmota32-VN.bin** = The Tasmota version in different languages for 4M+ flash.
- **tasmota32-ir.bin** = The InfraRed Receiver and transmitter version allowing all available protocols provided by library IRremoteESP8266 but without most other features for 4M+ flash.
- **tasmota32-display.bin** = The Display version without Energy Monitoring but adds display support for 4M+ flash.
- **tasmota32-lvgl.bin** = The LVGL version adds Light and Versatile Graphics Library (LVGL) display support for 4M+ flash.
- **tasmota32-webcam.bin** = The Webcam version adds webcam support for 4M+ flash.
- **tasmota32-bluetooth.bin** = The Bluetooth version adds BLE support for 4M+ flash.
- **tasmota32-odroidgo.bin** = The Odroid-Go version is specifically tailored to Odroid_go hardware for 8M+ flash.
- **tasmota32-core2.bin** = The Core2 version is specifically tailored to M5Stack Core2 hardware for 8M+ flash.

Latest released binaries can be downloaded from
- https://github.com/arendst/Tasmota-firmware/tree/main/release-firmware
- http://ota.tasmota.com/tasmota32/release

Historical binaries can be downloaded from
- http://ota.tasmota.com/tasmota32/release-10.1.0

The latter links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota32/release/tasmota32.bin``

## Additional information

[List](MODULES.md) of embedded modules.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog v10.1.0 Noelle
### Added
- Support for 1 second heartbeat GPIO
- Support for FUNC_BUTTON_MULTI_PRESSED in (light)drivers
- Command ``TcpConfig`` for TCPBridge protocol configuration [#13565](https://github.com/arendst/Tasmota/issues/13565)
- Support for HDC2010 temperature/humidity sensor by Luc Boudreau [#13633](https://github.com/arendst/Tasmota/issues/13633)
- WS2812 scheme 13 stairs effect [#13595](https://github.com/arendst/Tasmota/issues/13595)
- Command ``IfxPeriod `` to overrule ``Teleperiod`` for Influx messages [#13750](https://github.com/arendst/Tasmota/issues/13750)
- Support for GPE Multi color smart light as sold by Action in the Netherlands
- Shutter support for venetian blinds with tilt control
- Support for 74xx595 8-bit shift registers [#13921](https://github.com/arendst/Tasmota/issues/13921)
- ESP32 Autoconfiguration
- ESP32 Preliminary support for Tasmota Apps (.tapp extesions)
- ESP32 OTA over HTTPS
- ESP32 HTTPS support to ``WebQuery``
- ESP32 Berry support for neopixel (WS2812, SK6812)
- ESP32 Berry ``import re`` regex module
- ESP32 Berry add module ``python_compat`` to be closer to Python syntax [#13428](https://github.com/arendst/Tasmota/issues/13428)
- ESP32 Proof of Concept Sonoff SPM with limited functionality (switching and energy monitoring) [#13447](https://github.com/arendst/Tasmota/issues/13447)

### Breaking Changed
- ESP32-S2 TSettings memory usage fixed to 4096 bytes regression from v9.5.0.8

### Changed
- IRremoteESP8266 library from v2.7.20 to v2.8.0
- ESP32 core library from v1.0.7.4 to v2.0.1.1
- ESP32-C3 core library from v2.0.0-post to consolidated ESP32 core library
- ESP32 NimBLE to v.1.3.3
- ESP32 toolchains changed from 8.4.0-2021r1 to 8.4.0-2021r2
- Range conversion edge values
- ESP8266 Gratuitous ARP enabled and set to 60 seconds [#13623](https://github.com/arendst/Tasmota/issues/13623)
- File editor no-wrap [#13427](https://github.com/arendst/Tasmota/issues/13427)
- MQTT TLS dual mode (CA or fingeprint) in same firmware, ``SetOption132 1`` to force fingerprint
- ESP32 Ethernet hostname ending in ``_eth`` to ``-eth`` according to RFC952

### Fixed
- Initial reset RTC memory based variables like EnergyToday and EnergyTotal
- SML compile error [#13441](https://github.com/arendst/Tasmota/issues/13441)
- GUI checkbox MQTT TLS not saved regression from v9.2.0.3 [#13442](https://github.com/arendst/Tasmota/issues/13442)
- Discovery of shutters [#13572](https://github.com/arendst/Tasmota/issues/13572)
- Tuya dimmer range issue [#13849](https://github.com/arendst/Tasmota/issues/13849)
- ESP32 Telegram compile error [#13435](https://github.com/arendst/Tasmota/issues/13435)
- ESP32-C3 OneWire as used by DS18x20 [#13583](https://github.com/arendst/Tasmota/issues/13583)
- ESP32 analog NTC temperature calculation [#13703](https://github.com/arendst/Tasmota/issues/13703)
- ESP32 compile error BLE EQ3 driver with core 2.0.x [#13948](https://github.com/arendst/Tasmota/issues/13948)

### Removed
- ILI9488 driver in favour of Universal Display driver [#13719](https://github.com/arendst/Tasmota/issues/13719)
