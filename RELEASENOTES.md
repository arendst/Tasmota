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
- http://ota.tasmota.com/tasmota/release-14.2.0

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
- https://ota.tasmota.com/tasmota32/release-14.2.0

The latter links can be used for OTA upgrades too like ``OtaUrl https://ota.tasmota.com/tasmota32/release/tasmota32.bin``

## Additional information

[List](MODULES.md) of embedded modules.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog v14.2.0.6
### Added
- Command ``SetOption69 1`` to enable Serial Bridge inverted Receive [#22000](https://github.com/arendst/Tasmota/issues/22000)
- HX711 optional calibration precision option on command ``Sensor34 2 <weight in gram> <precision>`` where `<precision>` is 1 to 20 [#13983](https://github.com/arendst/Tasmota/issues/13983)
- ESP8266 support for one-wire M1601 temperature sensor on DS18x20 GPIO [#21376](https://github.com/arendst/Tasmota/issues/21376)
- ESP8266 support for I2C CLK on GPIO16 [#22199](https://github.com/arendst/Tasmota/issues/22199)
- Support for I2C M5Unit (Mini)Scales using HX711 driver
- Support for RX8010 RTC as used in IOTTIMER [#21376](https://github.com/arendst/Tasmota/issues/21376)
- Support for BL0906 up to 6 channel energy monitor as used in Athom EM2/EM6 [#22167](https://github.com/arendst/Tasmota/issues/22167)
- Support for Sonoff SPM v1.3.0 [#13447](https://github.com/arendst/Tasmota/issues/13447)
- Energy command ``PowerSet 60,230`` to calibrate both Current and Power with known resistive load of 60W at 230V using calibrated Voltage
- Energy command ``CurrentSet 60,230`` to calibrate both Power and Current with known resistive load of 60W at 230V using calibrated Voltage
- MQTT warning if trying to connect without TLS on a port that normally uses TLS [#22175](https://github.com/arendst/Tasmota/issues/22175)
- Energy Log level 4 message when (Calculated) Apparent Power is less than Active Power indicating wrong calibration [#20653](https://github.com/arendst/Tasmota/issues/20653)
- Support nexus protocol and calculation of separation limit to rc-switch library [#21886](https://github.com/arendst/Tasmota/issues/21886)
- KNX additional KnxTx functions and define KNX_USE_DPT9 [#22071](https://github.com/arendst/Tasmota/issues/22071)
- SML multi TRX line [#22056](https://github.com/arendst/Tasmota/issues/22056)
- ESP8266 experimental support for second I2C bus
- Zigbee Koenkk firmware 20240710 for Sonoff Zigbee ZBPro [#22076](https://github.com/arendst/Tasmota/issues/22076)
- Berry Zigbee improvements to prepare Matter [#22083](https://github.com/arendst/Tasmota/issues/22083)
- Berry virtual Energy driver [#22134](https://github.com/arendst/Tasmota/issues/22134)
- Berry improve `int64` constructor [#22172](https://github.com/arendst/Tasmota/issues/22172)
- Matter support for Zigbee Temperature, Humidity and Pressure sensors [#22084](https://github.com/arendst/Tasmota/issues/22084)
- Matter support for Zigbee Occupancy and Light 0/1/2 (OnOff / Dimmer / White Color Temperature) [#22110](https://github.com/arendst/Tasmota/issues/22110)

### Breaking Changed
- Berry make `energy` modules changes from #21887 backwards compatible [#22046](https://github.com/arendst/Tasmota/issues/22046)

### Changed
- ESP8266 platform update from 2024.06.00 to 2024.09.00 and Framework (Arduino Core) from v2.7.7 to v2.7.8 [#22199](https://github.com/arendst/Tasmota/issues/22199)
- ESP32 platform update from 2024.08.10 to 2024.09.30 and Framework (Arduino Core) from v3.0.4 to v3.1.0.240926 (#22203)[#22203](https://github.com/arendst/Tasmota/issues/22203)
- ESP32 LVGL library from v9.1.0 to v9.2.0 [#22031](https://github.com/arendst/Tasmota/issues/22031)
- GPIOViewer from v1.5.5 to v1.5.6
- Add command entered to command error and command unknown message
- Energy BL09xx command ``CurrentSet`` input changed from Ampere to milliAmpere
- Energy force Apparent Power equals Active Power when (Calculated) Apparent Power is less than Active Power [#20653](https://github.com/arendst/Tasmota/issues/20653)
- Refactored I2C drivers HTU21, BH1750, SHT3x, iAQ and HYT
- Refactor and fix PID sensor (PID_USE_LOCAL_SENSOR) read race condition [#22162](https://github.com/arendst/Tasmota/issues/22162)
- SCD30 Lowered I2C clock from 100k to 50k [#15438](https://github.com/arendst/Tasmota/issues/15438)

### Fixed
- Compilation exception when metrics not found [#22170](https://github.com/arendst/Tasmota/issues/22170)
- Crash when calling TasmotaSerial destructor when initialized with incorrect arguments [#22036](https://github.com/arendst/Tasmota/issues/22036)
- Energy calculation [#20653](https://github.com/arendst/Tasmota/issues/20653)
- SML trx pin error [#22119](https://github.com/arendst/Tasmota/issues/22119)
- Shutter timing registers overflow [#21966](https://github.com/arendst/Tasmota/issues/21966)
- Shutter missing HOLD on shutterbutton [#22108](https://github.com/arendst/Tasmota/issues/22108)
- Shutter remaining issues on shutterinvert [#22120](https://github.com/arendst/Tasmota/issues/22120)
- PZEM continue energy monitoring when one phase fails [#21968](https://github.com/arendst/Tasmota/issues/21968)
- BearSSL panic on ESP8266 in rare conditions [#22017](https://github.com/arendst/Tasmota/issues/22017)
- ModbusBridge request and response logic [#22075](https://github.com/arendst/Tasmota/issues/22075)
- Autoconf prevent 'init.bat' from stopping on empty lines [#22158](https://github.com/arendst/Tasmota/issues/22158)
- Zigbee extend timeout for MCU reboot from 5s to 10s [#22009](https://github.com/arendst/Tasmota/issues/22009)
- Zigbee avoid disabling console serial on ESP32 and improved log messages [#22082](https://github.com/arendst/Tasmota/issues/22082)
- Zigbee flashing CC2562P with latest firmware [#22117](https://github.com/arendst/Tasmota/issues/22117)
- ESP32 Range Extender compile error with core 3.x [#22205](https://github.com/arendst/Tasmota/issues/22205)
- ESP32 Dali compile error with core 3.x [#22214](https://github.com/arendst/Tasmota/issues/22214)
- Berry avoid `readbytes()` from crashing when file is too large [#22057](https://github.com/arendst/Tasmota/issues/22057)
- Berry energy missing attributes [#22116](https://github.com/arendst/Tasmota/issues/22116)
- Berry I2C to prepare M5Stack I2C STM32 based devices [#22143](https://github.com/arendst/Tasmota/issues/22143)
- ESP32-S3 uDisplay force cache writes to RGB display [#22222](https://github.com/arendst/Tasmota/issues/22222)
- LVGL Added OpenHASP icons to font `montserrat-28` [#22048](https://github.com/arendst/Tasmota/issues/22048)
- LVGL compilation of lv_menu [#22188](https://github.com/arendst/Tasmota/issues/22188)
- HASPmota broken `changed` event [#22194](https://github.com/arendst/Tasmota/issues/22194)
- HASPmota error when page '1' is not defined [#22220](https://github.com/arendst/Tasmota/issues/22220)
- Matter fixed UI bug when no endpoints configured [#22008](https://github.com/arendst/Tasmota/issues/22008)
- Matter fix when Rules are disabled [#22016](https://github.com/arendst/Tasmota/issues/22016)
- Matter fail to report Shutter status if no shutter is configured in Tasmota [#22049](https://github.com/arendst/Tasmota/issues/22049)
- Matter fix Waterleak broken after Berry solidification optimisation #21885 [#22052](https://github.com/arendst/Tasmota/issues/22052)

### Removed
- ESP8266 Analog input support using energy driver as only one channel is available
- Berry remove reuse of methods for interface-like code reuse #21500 [#22055](https://github.com/arendst/Tasmota/issues/22055)
