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

This release will be supported from ESP32/Arduino library Core version **v3.1.0.240926**.

Support of ESP8266 Core versions before 2.7.8 and ESP32 Core versions before v3.1.0.240926 have been removed.

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
- http://ota.tasmota.com/tasmota/release-14.3.0

The latter links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota/release/tasmota.bin.gz``

### ESP32, ESP32-C2, ESP32-C3, ESP32-C6, ESP32-S2 and ESP32-S3 based
The following binary downloads have been compiled with ESP32/Arduino library core version **v3.1.0.240926**.

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
- https://ota.tasmota.com/tasmota32/release-14.3.0

The latter links can be used for OTA upgrades too like ``OtaUrl https://ota.tasmota.com/tasmota32/release/tasmota32.bin``

## Additional information

[List](MODULES.md) of embedded modules.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog v14.3.0.5
### Added
- DALI support for short addresses (gear) and groups
- DALI command `DaliGear` to set max found gear to speed up scan response
- DALI command `DaliGroup` to add gear to groups
- DALI command `DaliTarget` to set light control broadcast, group number or gear number
- DALI command `DaliGroupSliders 0..16` to show GUI group sliders with feedback disabling `DaliLight`
- DALI inverted signal configuration using GPIO DALI RX_i/TX_i
- Support for I2C over Serial [#22444](https://github.com/arendst/Tasmota/issues/22444)
- Support KNX for scripts [#22429](https://github.com/arendst/Tasmota/issues/22429)
- Support deep sleep (standby) for VL53L0X [#22441](https://github.com/arendst/Tasmota/issues/22441)
- Support for Shelly DALI Dimmer Gen3
- Support for HLK-LD2410S 24GHz smart wave motion sensor [#22253](https://github.com/arendst/Tasmota/issues/22253)
- Support for US AQI and EPA AQI in PMS5003x sensors [#22294](https://github.com/arendst/Tasmota/issues/22294)
- Support for MS5837 pressure and temperature sensor [#22376](https://github.com/arendst/Tasmota/issues/22376)
- HLK-LD2410 Engineering mode [#21880](https://github.com/arendst/Tasmota/issues/21880)
- Mitsubishi Electric HVAC Operation time for MiElHVAC [#22334](https://github.com/arendst/Tasmota/issues/22334)
- Mitsubishi Electric HVAC Outdoor Temperature for MiElHVAC [#22345](https://github.com/arendst/Tasmota/issues/22345)
- Mitsubishi Electric HVAC Compressor Frequency for MiElHVAC [#22347](https://github.com/arendst/Tasmota/issues/22347)
- Mitsubishi Electric HVAC Auto Clear Remote Temp for MiElHVAC [#22370](https://github.com/arendst/Tasmota/issues/22370)
- SolaxX1 Meter mode [#22330](https://github.com/arendst/Tasmota/issues/22330)
- ESP32 MI32 legacy add config operations [#22458](https://github.com/arendst/Tasmota/issues/22458)
- BLE track devices with RPA [#22300](https://github.com/arendst/Tasmota/issues/22300)
- Berry add I2C read16/write16 supporting Little Endian [#22448](https://github.com/arendst/Tasmota/issues/22448)
- Berry drivers for PCA9535 (generic and in SenseCAP D1) [#22451](https://github.com/arendst/Tasmota/issues/22451)
- HASPmota `haspmota.get_pages()` to get the sorted list of pages [#22358](https://github.com/arendst/Tasmota/issues/22358)

### Breaking Changed

### Changed
- ESP32 Platform from 2024.09.30 to 2024.11.30, Framework (Arduino Core) from v3.1.0.240926 to v3.1.0.241030 and IDF to 5.3.1.241024 [#22384](https://github.com/arendst/Tasmota/issues/22384)
- ESP32 LVGL library from v9.2.0 to v9.2.2 [#22385](https://github.com/arendst/Tasmota/issues/22385)
- Redesign GUI adding feedback to buttons, shutters and lights
- Unit (k)VAr(h) to (k)var(h) [#22435](https://github.com/arendst/Tasmota/issues/22435)
- AHT1X/AHT2X/AHT3X ready for virtual I2C [#22427](https://github.com/arendst/Tasmota/issues/22427)
- SGP4X ready for virtual I2C [#22427](https://github.com/arendst/Tasmota/issues/22427)
- SCD40 reduce logging levels [#22443](https://github.com/arendst/Tasmota/issues/22443)
- SCD40 ready for virtual I2C [#22443](https://github.com/arendst/Tasmota/issues/22443)
- Refactored `i2c_enabled` as array [#22387](https://github.com/arendst/Tasmota/issues/22387)
- DALI renamed commands `DaliCommission` to `DaliScan` and `DaliWeb` to `DaliLight`
- DALI set Tasmota light control as default
- Shutter optimized behavior to publish shutter data with sensor request [#22353](https://github.com/arendst/Tasmota/issues/22353)
- HASPmota support for page delete and object updates [#22311](https://github.com/arendst/Tasmota/issues/22311)

### Fixed
- FUNC_COMMAND linked list command buffer corruption by shutter driver
- Alexa Hue with multiple devices [#22383](https://github.com/arendst/Tasmota/issues/22383)
- Mitsubishi Electric HVAC Standby Stage for MiElHVAC [#22430](https://github.com/arendst/Tasmota/issues/22430)
- EQ3 TRV firmware version 1.46 fails if the default true is used in subscribe on the notify characteristic [#22328](https://github.com/arendst/Tasmota/issues/22328)
- Ethernet on -DFRAMEWORK_ARDUINO_ITEAD framework regression from v14.3.0 [#22367](https://github.com/arendst/Tasmota/issues/22367)
- ESP32 Arduino Core IPv6 zones used by Matter [#22378](https://github.com/arendst/Tasmota/issues/22378)
- ESP32, ESP32-S2 and ESP32-S3 re-enable touch buttons [#22446](https://github.com/arendst/Tasmota/issues/22446)
- ESP32-S3 UART output mode for Tx [#22426](https://github.com/arendst/Tasmota/issues/22426)

### Removed
