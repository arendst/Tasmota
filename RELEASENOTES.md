<img src="https://github.com/arendst/Tasmota/blob/master/tools/logo/TASMOTA_FullLogo_Vector.svg" alt="Logo" align="right" height="76"/>

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

This release will be supported from ESP32/Arduino library Core version **2.0.5**.

Support of ESP8266 Core versions before 2.7.4.9 and ESP32 Core versions before 2.0.5 have been removed.

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
- **tasmota4M.bin** = The Tasmota version with most drivers and filesystem for 4M+ flash.
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
- http://ota.tasmota.com/tasmota/release-12.1.1

The latter links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota/release/tasmota.bin.gz``

### ESP32, ESP32-C3, ESP32-S2 and ESP32-S3 based
The following binary downloads have been compiled with ESP32/Arduino library core version **2.0.5**.

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
- http://ota.tasmota.com/tasmota32/release

Historical binaries can be downloaded from
- http://ota.tasmota.com/tasmota32/release-12.1.1

The latter links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota32/release/tasmota32.bin``

## Additional information

[List](MODULES.md) of embedded modules.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog v12.1.1.2
### Added
- Command ``SetOption46 0..255`` to add 0..255 * 10 milliseconds power on delay before initializing I/O [#15438](https://github.com/arendst/Tasmota/issues/15438)
- Command ``SetOption146 1`` to enable display of ESP32 internal temperature
- Command ``SetOption46 0..255`` to add 0..255 * 10 milliseconds power on delay before initializing I/O [#15438](https://github.com/arendst/Tasmota/issues/15438)
- Command ``StatusRetain`` [#11109](https://github.com/arendst/Tasmota/issues/11109)
- Command ``UrlFetch <url>`` to download a file to filesystem
- Command ``DspSpeed 2..127`` to control message rotation speed on display of POWR3xxD and THR3xxD
- Command ``DspLine<1|2> <index>,<unit>,<index>,<unit>,...`` to select message(s) on display of POWR3xxD and THR3xxD
- Support for SGP40 gas and air quality sensor [#16341](https://github.com/arendst/Tasmota/issues/16341)
- Support for Modbus writing using ModbusBridge by JeroenSt [#16351](https://github.com/arendst/Tasmota/issues/16351)
- Support for DFRobot SEN0390 V30B ambient light sensor [#16105](https://github.com/arendst/Tasmota/issues/16105)
- Zigbee device plugin mechanism with commands ``ZbLoad``, ``ZbUnload`` and ``ZbLoadDump`` [#16252](https://github.com/arendst/Tasmota/issues/16252)
- Zigbee basic support for Green Power [#16407](https://github.com/arendst/Tasmota/issues/16407)
- Flowrate meter flow amount/duration, show values in table format [#16385](https://github.com/arendst/Tasmota/issues/16385)
- Support of optional file calib.dat on ADE7953 based energy monitors like Shelly EM [#16486](https://github.com/arendst/Tasmota/issues/16486)
- Support for Ethernet in ESP32 safeboot firmware [#16388](https://github.com/arendst/Tasmota/issues/16388)
- ESP32-S3 support for internal temperature sensor
- Berry has persistent MQTT subscriptions: auto-subscribe at (re)connection
- Berry automated solidification of code

### Breaking Changed

### Changed
- ESP32 NimBLE library from v1.3.6 to v1.4.0
- IRremoteESP8266 library from v2.8.2 to v2.8.4
- Tasmota Core32 from 2.0.4.1 to 2.0.5
- TasmotaModbus library from v3.5.0 to v3.6.0 [#16351](https://github.com/arendst/Tasmota/issues/16351)
- Button debouncing V3 by adopting switch debounce code [#16339](https://github.com/arendst/Tasmota/issues/16339)
- Thermostat max allowed temperature from 100 to 200C [#16363](https://github.com/arendst/Tasmota/issues/16363)
- Using command ``SerialBuffer`` raise max allowed buffer size to 2048 characters [#16374](https://github.com/arendst/Tasmota/issues/16374)
- Zigbee report unprocessed attributes
- ESP32 Increase number of button GPIOs from 8 to 28 [#16518](https://github.com/arendst/Tasmota/issues/16518)
- ESP32 Platformio one Platform for all Tasmota frameworks Core32 2.0.5 [#16644](https://github.com/arendst/Tasmota/issues/16644)

### Fixed
- RTC not detected when lights are present [#16242](https://github.com/arendst/Tasmota/issues/16242)
- DNS lookup for .local domains [#16273](https://github.com/arendst/Tasmota/issues/16273)
- Button response delay regression from v12.0.2.4 [#16319](https://github.com/arendst/Tasmota/issues/16319)
- Lost module name in GUI regression from v12.0.2.4 - 20220803 [#16324](https://github.com/arendst/Tasmota/issues/16324)
- Removed whitespace from JSON values with no decimals [#16365](https://github.com/arendst/Tasmota/issues/16365)

### Removed
