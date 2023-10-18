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

This release will be supported from ESP8266/Arduino library Core version **2.7.4.9** due to reported security and stability issues on previous Core version. This will also support gzipped binaries.

This release will be supported from ESP32/Arduino library Core version **2.0.14**.

Support of ESP8266 Core versions before 2.7.4.9 and ESP32 Core versions before 2.0.14 have been removed.

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
- http://ota.tasmota.com/tasmota/release-13.2.0

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

Latest released binaries can be downloaded from
- https://github.com/arendst/Tasmota-firmware/tree/main/release-firmware
- https://ota.tasmota.com/tasmota32/release

Historical binaries can be downloaded from
- https://ota.tasmota.com/tasmota32/release-13.2.0

The latter links can be used for OTA upgrades too like ``OtaUrl https://ota.tasmota.com/tasmota32/release/tasmota32.bin``

## Additional information

[List](MODULES.md) of embedded modules.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog v13.2.0 Quincy
### Added
- Experimental support for ESP32-C2 and ESP32-C6 using Arduino core v3
- Commands to allow setting of timeprop parameters [#19310](https://github.com/arendst/Tasmota/issues/19310)
- Command ``Mi32Name`` [#19619](https://github.com/arendst/Tasmota/issues/19619)
- Variables ``%power<1..28>%`` and  ``%switch<1..28>%`` to rules [#19331](https://github.com/arendst/Tasmota/issues/19331)
- Support different baudrates on BL0942 energy monitor
- Support for Shelly PlusPMMini, Plus1Mini and Plus1PMMini
- Support for HDMI CEC protocol [#19434](https://github.com/arendst/Tasmota/issues/19434)
- Support for ENS16x (air quality) and ENS210 (temp & RH) sensors [#19479](https://github.com/arendst/Tasmota/issues/19479)
- Support for HC8 CO2 sensor [#19714](https://github.com/arendst/Tasmota/issues/19714)
- Support for non-persistent ``WebButton17`` to ``WebButton32`` [#19580](https://github.com/arendst/Tasmota/issues/19580)
- ESP32 commands ``Ds18Rescan`` and ``Ds18RetryRead`` [#19700](https://github.com/arendst/Tasmota/issues/19700)
- ESP32 support for influxdb access using https [#19582](https://github.com/arendst/Tasmota/issues/19582)
- Berry read and write Counters [#19558](https://github.com/arendst/Tasmota/issues/19558)
- Matter support for Virtual Devices controllable via Rules or Berry [#19520](https://github.com/arendst/Tasmota/issues/19520)

### Breaking Changed
- `Sendmail` upgraded to ESP-Mail-Client v3.4.9 from v1.2.0, using BearSSL instead of MbedTLS [#19460](https://github.com/arendst/Tasmota/issues/19460)
- Removed support for Homekit in favour of Matter [#19738](https://github.com/arendst/Tasmota/issues/19738)

### Changed
- ESP32 Framework (Arduino Core) from v2.0.11 to v2.0.14
- ESP32 LVGL library from v8.3.8 to v8.3.10 (no functional change)
- Display invert setting after tasmota start in uDisplay driver [#19337](https://github.com/arendst/Tasmota/issues/19337)
- Consolidate SGP40 and SGP41 into SGP4x driver [#19560](https://github.com/arendst/Tasmota/issues/19560)
- MAX31855/MAX6675 sensors driver support up to 6 [#19329](https://github.com/arendst/Tasmota/issues/19329)
- Teleinfo use Apparent Power as Active Power approximation [#19756](https://github.com/arendst/Tasmota/issues/19756)
- ESP32 LittleFS updated to version with grow option [#19635](https://github.com/arendst/Tasmota/issues/19635)
- ESP32 I2S audio preparation for Arduino Core v3 [#19637](https://github.com/arendst/Tasmota/issues/19637)
- ESP32 analog from `analogRead()` to calibrated `analogReadMilliVolts()` [#19732](https://github.com/arendst/Tasmota/issues/19732)

### Fixed
- Shutter invert [#19341](https://github.com/arendst/Tasmota/issues/19341) and [#19374](https://github.com/arendst/Tasmota/issues/19374)
- Teleinfo power [#19381](https://github.com/arendst/Tasmota/issues/19381)
- Exception 3 in IRHVAC [#19389](https://github.com/arendst/Tasmota/issues/19389)
- PCF8574 mode 1 with base relays exception 3/28 regression from v12.4.0.4 [#19408](https://github.com/arendst/Tasmota/issues/19408)
- ModbusBridge write memory leak [#19758](https://github.com/arendst/Tasmota/issues/19758)
- Zigbee timezone when device reads LocalTime attribute [#19772](https://github.com/arendst/Tasmota/issues/19772)
- ESP32 DS18x20 driver support extended over GPIO33
- ESP32 Support for IPv6 link-local zones for esp-idf 5.1 (necessary for Matter)
- ESP32 Shutter migration [#19454](https://github.com/arendst/Tasmota/issues/19454)
- ESP32 Shutter multi press button events [#19465](https://github.com/arendst/Tasmota/issues/19465)
- ESP32 Shutter button quad press [#19589](https://github.com/arendst/Tasmota/issues/19589)
- ESP32 shutter frequency [#19717](https://github.com/arendst/Tasmota/issues/19717)
- ESP32 Arduino Core v2 wifi client flush [#19642](https://github.com/arendst/Tasmota/issues/19642)
- ESP32 Partition Wizard grow filesystem support [#19645](https://github.com/arendst/Tasmota/issues/19645)
- ESP32-C3 relay click on restart
- Matter support for Virtual Devices controllable via Rules or Berry [#19520](https://github.com/arendst/Tasmota/issues/19520)

### Removed
- Support for Homekit in favour of Matter [#19738](https://github.com/arendst/Tasmota/issues/19738)
