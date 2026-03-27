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

## Changelog v15.3.0.3
### Added
- Support for JSON value pair `"ARCH"` in template being either ESP8266, ESP32, ESP32C2, ESP32C3, ESP32C5, ESP32C6, ESP32H2, ESP32H4, ESP32P4, ESP32S2 or ESP32S3
- Support for Sensirion SCD42 and SCD43 CO2 sensor
- Support for Sensirion STCC4 CO2 sensor
- ESP8266 redesigned I2C Wire driver to support second I2C bus
- Environment sensors CCS811, SCD30, SGP30 and SGP40 second I2C bus support
- Real Time Clocks BM8563, PCF85063 and PCF85363 second I2C bus support
- LCD second I2C bus support
- I2S full duplex, auto rx sample rate [#24469](https://github.com/arendst/Tasmota/issues/24469)
- Sen5x power on delay of 60ms [#24452](https://github.com/arendst/Tasmota/issues/24452)
- Berry add `loglevel` to `mqtt.publish()` [#24551](https://github.com/arendst/Tasmota/issues/24551)

### Breaking Changed
- Renamed SCD40 sensors to their real names like SCD40 to SCD43

### Changed
- ESP8266 platform update from 2026.02.00 to 2026.03.00 [#24547](https://github.com/arendst/Tasmota/issues/24547)
- ESP32 Platform from 2025.02.30 to 2026.03.30, Framework (Arduino Core) from v3.1.9 to v3.1.10 and IDF from v5.3.4.251226 to v5.3.4.260127 [#24547](https://github.com/arendst/Tasmota/issues/24547)
- ESP32-C5/C6/P4 Platform from 2025.03.30 to 2026.03.50, Framework (Arduino Core) from v3.1.10 to v3.3.7 and IDF from v5.3.4.260127 to v5.5.3+ [#24567](https://github.com/arendst/Tasmota/issues/24567)
- Adafruit_BusIO library from v1.11.0 to v1.17.4
- Adafruit_CCS811 library from v1.0.0.14 to v1.1.3
- Adafruit SGP30 library from v1.2.0 to v2.0.3
- Adafruit SGP40 library from v1.1.0 to v1.1.4
- Sensirion Core library from v0.6.0 to v0.7.2
- LVGL library from v9.4.0 to v9.5.0 [#24470](https://github.com/arendst/Tasmota/issues/24470)
- SHT1x software reset I2C bus after initial (un)detection
- NeoPool always output valid sensitive data [#24573](https://github.com/arendst/Tasmota/issues/24573)
- SML suppress MQTT publish until valid meter data received [#24587](https://github.com/arendst/Tasmota/issues/24587)
- Matter improved parameters handling [#24471](https://github.com/arendst/Tasmota/issues/24471)

### Fixed
- ESP8266 heap drain and exception 29 when DHCP provides NTP server [#24515](https://github.com/arendst/Tasmota/issues/24515), [#24566](https://github.com/arendst/Tasmota/issues/24566)
- Crash when shutting down Wifi with `Wifi 0` [#24536](https://github.com/arendst/Tasmota/issues/24536)
- Don't send extraneous `0\r\n\r\n` with non-chunked HTTP/1.0 [#24518](https://github.com/arendst/Tasmota/issues/24518)
- NeoPool possible IntegerDivideByZero [#24578](https://github.com/arendst/Tasmota/issues/24578)
- Do not free BT memory when in use [#24480](https://github.com/arendst/Tasmota/issues/24480)
- Berry avoid `tasmota.wifi()` returning bad values when wifi is turned off [#24505](https://github.com/arendst/Tasmota/issues/24505)
- File upload improvements: `/ufsu` api mode, no interrupts disabling, cleaner confirmation page [#24521](https://github.com/arendst/Tasmota/issues/24521)

### Removed
- ESP8266 I2C Slave support which was never used by Tasmota anyway
- Berry `tasmota.urlbecload()` superseded by Extension Manager [#24493](https://github.com/arendst/Tasmota/issues/24493)
- Berry remove `mdns.stop()` [#24549](https://github.com/arendst/Tasmota/issues/24549)
