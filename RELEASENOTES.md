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

This release will be supported from ESP32/Arduino library Core version **3.0.0**.

Support of ESP8266 Core versions before 2.7.6 and ESP32 Core versions before 3.0.0 have been removed.

### Known issues with v14.0.0

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
- http://ota.tasmota.com/tasmota/release-14.0.0

The latter links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota/release/tasmota.bin.gz``

### ESP32, ESP32-C2, ESP32-C3, ESP32-C6, ESP32-S2 and ESP32-S3 based
The following binary downloads have been compiled with ESP32/Arduino library core version **3.0.0**.

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
- https://ota.tasmota.com/tasmota32/release-14.0.0

The latter links can be used for OTA upgrades too like ``OtaUrl https://ota.tasmota.com/tasmota32/release/tasmota32.bin``

## Additional information

[List](MODULES.md) of embedded modules.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog v14.0.0.2
### Added
- Optional command ``WebRun`` (as WebQuery extension) [#21364](https://github.com/arendst/Tasmota/issues/21364)
- Support for Knx dimmer and color [#21434](https://github.com/arendst/Tasmota/issues/21434)
- Berry add reuse of methods for interface-like code reuse [#21500](https://github.com/arendst/Tasmota/issues/21500)
- Support for Matter 1.3 Water leak detectors [#21456](https://github.com/arendst/Tasmota/issues/21456)

### Breaking Changed

### Changed
- ESP32 Core3 platform update from 2024.05.11 to 2024.05.12 [#21493](https://github.com/arendst/Tasmota/issues/21493)
- GPIOViewer from v1.5.2 to v1.5.3
- On universal display remove default backlight power if a PWM channel is used for backlight
- Support W5500 SPI ethernet using four SPI GPIOs only without IRQ and RESET
- ESP32 compiler option from `target-align` to `no-target-align` [#21407](https://github.com/arendst/Tasmota/issues/21407)
- ESP32 I2S audio improvements [#21433](https://github.com/arendst/Tasmota/issues/21433)
- Berry binary compiled with gcc [#21426](https://github.com/arendst/Tasmota/issues/21426)
- Berry allow easy solidification of external Berry [#21430](https://github.com/arendst/Tasmota/issues/21430)
- Berry change internal storage of parent class for methods [#21490](https://github.com/arendst/Tasmota/issues/21490)

### Fixed
- Domoticz re-subscribe on MQTT reconnect. Regression from v13.4.0.3 [#21281](https://github.com/arendst/Tasmota/issues/21281)
- Watchdog restart or freeze if ``displaytext`` is more than 128 characters [#21401](https://github.com/arendst/Tasmota/issues/21401)
- Wrong timeout in `WebQuery` and `webclient` since Core3 [#21442](https://github.com/arendst/Tasmota/issues/21442)
- UfsServe watchdog on large folder [#21486](https://github.com/arendst/Tasmota/issues/21486)
- SML compile error when median-filter is disabled [#21495](https://github.com/arendst/Tasmota/issues/21495)
- Hydreon RG15 malformed JSON string [#21508](https://github.com/arendst/Tasmota/issues/21508)
- Telegram TLS fingerprint, remove CA validation [#21514](https://github.com/arendst/Tasmota/issues/21514)
- Zigbee crash when removing `ZbName` [#21449](https://github.com/arendst/Tasmota/issues/21449)
- Avoid connection errors when switching to safeboot to upload OTA firmware [#21428](https://github.com/arendst/Tasmota/issues/21428)
- ESP32 BLE fix scanning [#21451](https://github.com/arendst/Tasmota/issues/21451)
- I2S APLL not supported on all SOCs [#21483](https://github.com/arendst/Tasmota/issues/21483)
- Webradio crash with invalid url [#21446](https://github.com/arendst/Tasmota/issues/21446)
- Berry Leds matrix alternate more and error about 'bri' attribute [#21431](https://github.com/arendst/Tasmota/issues/21431)
- Matter auto-fix IPv6 link-local zone id when network reconnects [#21494](https://github.com/arendst/Tasmota/issues/21494)

### Removed
- Support of old insecure fingerprint algorithm. Deprecated since v8.4.0 [#21417](https://github.com/arendst/Tasmota/issues/21417)
