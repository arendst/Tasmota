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

This release will be supported from ESP32/Arduino library Core version **2.0.10**.

Support of ESP8266 Core versions before 2.7.4.9 and ESP32 Core versions before 2.0.10 have been removed.

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
- http://ota.tasmota.com/tasmota/release-12.5.0

The latter links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota/release/tasmota.bin.gz``

### ESP32, ESP32-C3, ESP32-S2 and ESP32-S3 based
The following binary downloads have been compiled with ESP32/Arduino library core version **2.0.10**.

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
- https://ota.tasmota.com/tasmota32/release-12.5.0

The latter links can be used for OTA upgrades too like ``OtaUrl https://ota.tasmota.com/tasmota32/release/tasmota32.bin``

## Additional information

[List](MODULES.md) of embedded modules.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog v12.5.0.4
### Added
- Command ``SetOption152 0/1`` to select two (0 = default) pin bistable or one (1) pin latching relay control [#18386](https://github.com/arendst/Tasmota/issues/18386)
- Command ``I2cScan0`` to scan both busses on ESP32 with one command
- Command ``WifiPower 0`` to enable dynamic wifi power based on RSSI by @TD-er [#15443](https://github.com/arendst/Tasmota/issues/15443)
- Command ``WifiPower 1`` to restore default wifi power
- Support for TC74 temperature sensor by Michael Loftis [#18042](https://github.com/arendst/Tasmota/issues/18042)
- Support for GM861 1D and 2D bar code reader [#18399](https://github.com/arendst/Tasmota/issues/18399)
- Support for PCA9557 8-bit I/O expander [#18632](https://github.com/arendst/Tasmota/issues/18632)
- Display descriptor for ST7735 128x160 display [#18741](https://github.com/arendst/Tasmota/issues/18741)
- Zigbee support for air sensors [#18665](https://github.com/arendst/Tasmota/issues/18665)
- ESP32 command ``Shuttersetup`` for "Shelly 2.5 pro" automatic calibration and setup (experimental)
- ESP32 Enhanced Shutterbuttons functionality to control tilt position, additionally incr/decr possible to position and tilt.
- Berry RS256 crypto algorithm (RSASSA-MCKS1_v1-5 with SHA256) used for JWT [#18763](https://github.com/arendst/Tasmota/issues/18763)
- Berry `tcpclientasync` class for non-blocking TCP client
- Berry add `set_lsb_justified(bool)` to `AudioOutputI2S` [#18774](https://github.com/arendst/Tasmota/issues/18774)
- Berry `string.format()` now automatically converts type according to format [#18890](https://github.com/arendst/Tasmota/issues/18890)
- Berry add global function `format` as a simpler syntax to `string.format` [#18925](https://github.com/arendst/Tasmota/issues/18925)
- HASPmota `meta` attribute and improved `berry_run` [#18685](https://github.com/arendst/Tasmota/issues/18685)
- Matter sensors Humidity, Pressure, Illuminance [#18441](https://github.com/arendst/Tasmota/issues/18441)
- Matter allow `Matter#Initialized` rule once the device is configured [#18451](https://github.com/arendst/Tasmota/issues/18451)
- Matter UI to change endpoints configuration [#18498](https://github.com/arendst/Tasmota/issues/18498)
- Matter support for Shutters with Tilt [#18509](https://github.com/arendst/Tasmota/issues/18509)
- Matter support for async HTTP used for bridged devices and remote relays [#18656](https://github.com/arendst/Tasmota/issues/18656)
- Matter bridge for ESP8266 remote endpoints (experimental) [#18734](https://github.com/arendst/Tasmota/issues/18734)
- Matter support for Occupancy via Switch (experimental) [#18742](https://github.com/arendst/Tasmota/issues/18742)
- Matter ability to add or remove endpoint in bridge mode (code only) [#18790](https://github.com/arendst/Tasmota/issues/18790)
- Matter add controller's Vendor Name to logs and UI [#18794](https://github.com/arendst/Tasmota/issues/18794)
- Matter redesigned UI [#18855](https://github.com/arendst/Tasmota/issues/18855)
- Matter add support for Contact Sensor [#18882](https://github.com/arendst/Tasmota/issues/18882)
- Matter add friendly-name (NodeLabel) to each endpoint [#18897](https://github.com/arendst/Tasmota/issues/18897)

### Breaking Changed
- Change command ``FileUpload`` index binary data detection from >199 to >299
- Matter relay number starts at 1 instead of 0 to match Tasmota numbering

### Changed
- AdafruitFingerprint library from v2.0.4 to v2.1.0
- IRremoteESP8266 library from v2.8.4 to v2.8.5
- ESP32 Framework (Core) from v2.0.7 to v2.0.10
- InfluxDb resolves DNS name before request [#18015](https://github.com/arendst/Tasmota/issues/18015)
- Refactored Zero Cross Dimmer [#18481](https://github.com/arendst/Tasmota/issues/18481)
- Energy power delta report delayed by two seconds allowing hardware to stabilize [#17751](https://github.com/arendst/Tasmota/issues/17751)
- Shutter sliders in WEBGUI automatically appear and disappear during configuration and update during movement [#18701](https://github.com/arendst/Tasmota/issues/18701)
- Berry `webclient.url_encode()` is now a static class method, no change required to existing code [#18775](https://github.com/arendst/Tasmota/issues/18775)

### Fixed
- ESP8266 no update on Energy Export Active regression from v12.3.1.3
- NovaSDS GUI values [#18444](https://github.com/arendst/Tasmota/issues/18444)
- LED PWM ac_dimmer curve was wrongly applied instead of Gamma regression from v12.2.0.5 [#18666](https://github.com/arendst/Tasmota/issues/18666)
- Shutter bootloop using more than 4 shutters [#18673](https://github.com/arendst/Tasmota/issues/18673)
- Inverted shutter now reflect status also in WEBGUI and several minor fixes to make "inverted" consistant [#18701](https://github.com/arendst/Tasmota/issues/18701)
- Interaction of ``SetOption92``, ``VirtualCT``, and ``RGBWWTable`` [#18768](https://github.com/arendst/Tasmota/issues/18768)
- Freeze BMP readings before deepsleep [#18720](https://github.com/arendst/Tasmota/issues/18720)
- NeoPool NPFiltration switch result [#18871](https://github.com/arendst/Tasmota/issues/18871)
- ESP32 Partition_Manager.tapp
- ESP32 InfluxDb initial connection delays using HTTPClient [#18015](https://github.com/arendst/Tasmota/issues/18015)
- ESP32 AIThinker webcam issues [#18652](https://github.com/arendst/Tasmota/issues/18652)
- ESP32 SPI initialization for MFRC522 [#18711](https://github.com/arendst/Tasmota/issues/18711)
- ESP32 Neopixel busy time adjustment [#18723](https://github.com/arendst/Tasmota/issues/18723)
- HASPmota event when value is non-integer [#18229](https://github.com/arendst/Tasmota/issues/18229)
- Berry a rare condition when a GC causes a memory corruption
- Berry rules for string comparisons [#18464](https://github.com/arendst/Tasmota/issues/18464)
- Berry parser error with upvals in closures [#18902](https://github.com/arendst/Tasmota/issues/18902)
- Zigbee attributes handling in Berry mapping [#18747](https://github.com/arendst/Tasmota/issues/18747)
- Zigbee regression with ``SetOption101`` [#18884](https://github.com/arendst/Tasmota/issues/18884)
- Matter fabric provisioning from CASE session for iOS 16.5 [#18709](https://github.com/arendst/Tasmota/issues/18709)
