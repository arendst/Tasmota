<img src="https://github.com/arendst/Tasmota/blob/master/tools/logo/TASMOTA_FullLogo_Vector.svg" alt="Logo" align="right" height="76"/>

# RELEASE NOTES

## Migration Information

**This version removes support for direct migration from versions before v8.1.0 (Doris)**

See [migration path](https://tasmota.github.io/docs/Upgrading#migration-path) for instructions how to migrate to a major version. Pay attention to the following version breaks due to dynamic settings updates:

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

In addition to TLS using fingerprints now also user supplied CA certs and AWS IoT is supported. Read [full documentation](https://tasmota.github.io/docs/AWS-IoT)

## Initial configuration tools

For initial configuration this release supports Webserver based **WifiManager** or **Serial** based command interface only. Support for **WPS** and **SmartConfig** has been removed.

## Provided Binary Downloads

### ESP8266 or ESP8285 based
The following binary downloads have been compiled with ESP8266/Arduino library core version **2.7.4.9**.

- **tasmota.bin** = The Tasmota version with most drivers. **RECOMMENDED RELEASE BINARY**
- **tasmota-BG.bin** to **tasmota-TW.bin** = The Tasmota version in different languages.
- **tasmota-lite.bin** = The Lite version without most drivers and sensors.
- **tasmota-knx.bin** = The Knx version without some features but adds KNX support.
- **tasmota-sensors.bin** = The Sensors version adds more useful sensors.
- **tasmota-ir.bin** = The InfraRed Receiver and transmitter version allowing all available protocols provided by library IRremoteESP8266 but without most other features.
- **tasmota-display.bin** = The Display version without Energy Monitoring but adds display support.
- **tasmota-zbbridge.bin** = The dedicated Sonoff Zigbee Bridge version.
- **tasmota-minimal.bin** = The Minimal version allows intermediate OTA uploads to support larger versions and does NOT change any persistent parameter. This version **should NOT be used for initial installation**.

Above binaries are also available as gzipped version allowing faster uploads.

The attached binaries can also be downloaded from http://ota.tasmota.com/tasmota/release. The links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota/release/tasmota.bin.gz``

### ESP32 based
The following binary downloads have been compiled with ESP32/Arduino library core version **1.0.5**.

- **tasmota32.bin** = The Tasmota version with most drivers. **RECOMMENDED RELEASE BINARY**
- **tasmota32-BG.bin** to **tasmota32-TW.bin** = The Tasmota version in different languages.
- **tasmota32-lite.bin** = The Lite version without most drivers and sensors.
- **tasmota32-knx.bin** = The Knx version without some features but adds KNX support.
- **tasmota32-sensors.bin** = The Sensors version adds more useful sensors.
- **tasmota32-ir.bin** = The InfraRed Receiver and transmitter version allowing all available protocols provided by library IRremoteESP8266 but without most other features.
- **tasmota32-display.bin** = The Display version without Energy Monitoring but adds display support.
- **tasmota32-webcam.bin** = The Webcam version adds webcam support.
- **tasmota32-bluetooth.bin** = The Bluetooth version adds BLE support.
- **tasmota32-odroidgo.bin** = The Odroid-Go version is specifically tailored to Odroid_go hardware.
- **tasmota32-core2.bin** = The Core2 version is specifically tailored to M5Stack Core2 hardware.

The attached binaries can also be downloaded from http://ota.tasmota.com/tasmota32/release. The links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota32/release/tasmota32.bin``

[List](MODULES.md) of embedded modules.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog v9.3.1.2
### Added
- Commands ``MqttKeepAlive 1..100`` to set Mqtt Keep Alive timer (default 30) and ``MqttTimeout 1..100`` to set Mqtt Socket Timeout (default 4) [#5341](https://github.com/arendst/Tasmota/issues/5341)
- Command ``Sensor80 1 <0..7>`` to control MFRC522 RFID antenna gain from 18dB (0) to 48dB (7) [#11073](https://github.com/arendst/Tasmota/issues/11073)
- Commands ``DisplayType`` to select sub-modules where implemented and ``DisplayInvert`` to select inverted display where implemented
- Support for SML VBUS [#11125](https://github.com/arendst/Tasmota/issues/11125)
- Support for NEC and OPTOMA LCD/DLP Projector serial power control by Jan Bubík [#11145](https://github.com/arendst/Tasmota/issues/11145)
- Support for XPT2046 touch screen digitizer on ILI9341 display by nonix [#11159](https://github.com/arendst/Tasmota/issues/11159)
- Support for zigbee lumi.sensor_wleak [#11200](https://github.com/arendst/Tasmota/issues/11200)
- Support for CSE7761 energy monitor as used in ESP32 based Sonoff Dual R3 Pow [#10793](https://github.com/arendst/Tasmota/issues/10793)
- Support for Frequency monitoring and zero-cross detection on CSE7761 (Sonoff Dual R3)
- Support for TM1638 seven segment display by Ajith Vasudevan [#11031](https://github.com/arendst/Tasmota/issues/11031)
- Support for MAX7219 seven segment display by Ajith Vasudevan [#11387](https://github.com/arendst/Tasmota/issues/11387)
- Support for MPU6886 on primary or secondary I2C bus
- Allow MCP230xx pinmode from output to input [#11104](https://github.com/arendst/Tasmota/issues/11104)
- Berry improvements [#11163](https://github.com/arendst/Tasmota/issues/11163)
- Extent compile time SetOptions support [#11204](https://github.com/arendst/Tasmota/issues/11204)
- ESP32 Extent BLE [#11212](https://github.com/arendst/Tasmota/issues/11212)
- ESP32 support for WS2812 hardware driver via RMT or I2S
- ESP32 support for secondary I2C controller
- ESP32 support for internal Hall Effect sensor connected to both GPIO36 and GPIO39 only

### Changed
- TasmotaSerial library from v3.2.0 to v3.3.0
- PubSubClient library from EspEasy v2.7.12 to Tasmota v2.8.12
- IRremoteESP8266 library from v2.7.15 to v2.7.16
- ESP32 core library from v1.0.5-rc6 to v1.0.5
- TuyaMcu dimmer timeout [#11121](https://github.com/arendst/Tasmota/issues/11121)
- Rename epaper 42 commands [#11222](https://github.com/arendst/Tasmota/issues/11222)
- DeepSleep announcement topic [#11223](https://github.com/arendst/Tasmota/issues/11223)

### Fixed
- PN532 on ESP32 Serial flush both Tx and Rx buffers [#10910](https://github.com/arendst/Tasmota/issues/10910)
- Light scheme related color changes [#11041](https://github.com/arendst/Tasmota/issues/11041)
- Refactor acceleration function for shutter stepper and servo [#11088](https://github.com/arendst/Tasmota/issues/11088)
- LM75AD detection on different addresses [#11096](https://github.com/arendst/Tasmota/issues/11096)
- Timer loop when console is scrolled up regression from v9.3.0 [#11108](https://github.com/arendst/Tasmota/issues/11108)
- Display exception when no file system is present [#11125](https://github.com/arendst/Tasmota/issues/11125)
- Scripter and SML fixes [#11150](https://github.com/arendst/Tasmota/issues/11150)
- Zigbee exception when bad frame is received [#11192](https://github.com/arendst/Tasmota/issues/11192)
- ESP32 flash script for Odroid and Core2 [#11227](https://github.com/arendst/Tasmota/issues/11227)
- ESP32 WS2812 bitbang support [#11248](https://github.com/arendst/Tasmota/issues/11248)
- DS18x20 driver timing issue (#11270)
