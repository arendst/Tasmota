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

## Changelog v13.2.0.2
### Added
- Command ``GpioRead`` to show input state [#19810](https://github.com/arendst/Tasmota/issues/19810)
- I2C bus2 support to iAQ core sensor [#19799](https://github.com/arendst/Tasmota/issues/19799)
- I2C bus2 support to HTU temperature and humidity sensor
- I2C bus2 support to BH1750 ambient light sensor
- I2C bus2 support to ADS1115 A/D Converter
- I2C bus2 support to SHTxX temperature and humidity sensor
- I2C bus2 support to HYTxxx temperature and humidity sensor
- I2C bus2 support to SI1145/6/7 Ultra violet index and light sensor
- I2C bus2 support to LM75AD temperature sensor
- Support for Winsen XH03x dust particle sensors using USE_PMS5003 and PMS_MODEL_ZH03X [#19850](https://github.com/arendst/Tasmota/issues/19850)
- ST7735S display.ini for 1.44 inch 128x128 red SPI display [#19862](https://github.com/arendst/Tasmota/issues/19862)
- Scripter TCP client [#19914](https://github.com/arendst/Tasmota/issues/19914)
- NeoPool hydrolysis setpoint and max
- NeoPool command ``NPFiltrationSpeed`` to set non-standard filtration type speed [#19857](https://github.com/arendst/Tasmota/issues/19857)
- NeoPool command ``SetOption157`` to enable output of sensitive data [#19857](https://github.com/arendst/Tasmota/issues/19857)
- NeoPool enhancements for HomeAssistant [#19857](https://github.com/arendst/Tasmota/issues/19857)
- Support for ESP32-C2 and ESP32-C6 using Arduino core v3
- ESP32 core v3 auto TasConsole USB or Serial connection by @staars
- Berry ``debug.gcdebug()`` to enable GC debugging [#19936](https://github.com/arendst/Tasmota/issues/19936)
- HASPmota add styling properties [#19912](https://github.com/arendst/Tasmota/issues/19912)
- Matter flow sensor suppor [#19852](https://github.com/arendst/Tasmota/issues/19852)

### Breaking Changed
- NeoPool SENSOR topic ``Power`` renamed to ``Powerunit`` [#19857](https://github.com/arendst/Tasmota/issues/19857)

### Changed
- Increase MAX_HUE_DEVICES to 32 [#19820](https://github.com/arendst/Tasmota/issues/19820)
- NeoPool ``NPHydrolysis`` percent and unit [#19924](https://github.com/arendst/Tasmota/issues/19924)
- MI32 updates [#19893](https://github.com/arendst/Tasmota/issues/19893)
- ESP32 keep FileSystem intact when over flashing with VSC [#19816](https://github.com/arendst/Tasmota/issues/19816)
- Matter Light0 now accept a Relay number [#19809](https://github.com/arendst/Tasmota/issues/19809)
- Matter update hierarchy of plugins [#19915](https://github.com/arendst/Tasmota/issues/19915)

### Fixed
- Compile USE_PID [#19890](https://github.com/arendst/Tasmota/issues/19890)
- Scripter timer issue [#19914](https://github.com/arendst/Tasmota/issues/19914)
- NeoPool filtration mode display [#19801](https://github.com/arendst/Tasmota/issues/19801)
- Zero-Cross Dimmer for ESP32 with Core3 [#19929](https://github.com/arendst/Tasmota/issues/19929)
- ESP32 I2C allow bus2 support when bus1 is not enabled
- ESP32 IR receive with Arduino Core 3 [#19904](https://github.com/arendst/Tasmota/issues/19904)

### Removed
