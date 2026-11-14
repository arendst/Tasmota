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

This release will be supported from ESP32/Arduino library Core version **v3.3.12**.

Support of ESP8266 Core versions before 2.7.8 and ESP32 Core versions before v3.3.12 have been removed.

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
- http://ota.tasmota.com/tasmota/release-16.0.0

The latter links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota/release/tasmota.bin.gz``

### ESP32, ESP32-C2, ESP32-C3, ESP32-C5, ESP32-C6, ESP32-C61, ESP32-P4, ESP32-P4R3, ESP32-S2 and ESP32-S3 based
The following binary downloads have been compiled with ESP32/Arduino library core version **v3.3.12**.

- **tasmota32.bin** = The Tasmota version with most drivers including additional sensors and KNX for 4M+ flash.  **RECOMMENDED RELEASE BINARY**
- **tasmota32solo1.bin** = The Tasmota version with most drivers including additional sensors and KNX for single core ESP32 and 4M+ flash.
- **tasmota32c2.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-C2 with serial and 4M+ flash.
- **tasmota32c3.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-C3 with USB HWCDC and fallback to serial and 4M+ flash.
- **tasmota32c5.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-C5 with USB HWCDC and fallback to serial and 4M+ flash.
- **tasmota32c6.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-C6 with USB HWCDC and fallback to serial and 4M+ flash.
- **tasmota32c61.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-C61 with USB HWCDC and fallback to serial and 4M+ flash.
- **tasmota32p4.bin** = The Tasmota version with most drivers including additional sensors and KNX for first generation ESP32-P4 with USB HWCDC and fallback to serial and 4M+ flash.
- **tasmota32p4r3.bin** = The Tasmota version with most drivers including additional sensors and KNX for new generation ESP32-P4 with USB HWCDC and fallback to serial and 4M+ flash.
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
- https://ota.tasmota.com/tasmota32/release-16.0.0

The latter links can be used for OTA upgrades too like ``OtaUrl https://ota.tasmota.com/tasmota32/release/tasmota32.bin``

## Additional information

[List](MODULES.md) of embedded modules.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog v16.0.0.4
### Added
- Heap Map optional feature fur detailed memory usage at boot [#25186](https://github.com/arendst/Tasmota/issues/25186)
- Device location and local pose with commands `Location`, `LocationRot`, `LocationScale`, `LocationClear` and `SetOption166` stored as JSON in file system or packed in ESP8266 settings [#25244](https://github.com/arendst/Tasmota/issues/25244)
- BME680 IAQ (air quality index) to BMP sensor driver [#25175](https://github.com/arendst/Tasmota/issues/25175)
- TuyaMCU pace outgoing frames so the MCU is never written to back-to-back [#25151](https://github.com/arendst/Tasmota/issues/25151)
- GPIOViewer labels to GPIOs using web app v2.3.4 [#25257](https://github.com/arendst/Tasmota/issues/25257)
- uDisplay support for SPI QSPI displays [#25156](https://github.com/arendst/Tasmota/issues/25156)
- BLE timeout and eQ3 TRV enhancements [#25173](https://github.com/arendst/Tasmota/issues/25173)
- Support MAC OUI wildcard for command MI32Keys in case multiple devices use same bindkey like IoTorero: `mi32keys 0C3D5EFFFFFF=48656c6c6f204254486f6d65210d0a00`
- LVGL add logging before crash when heap allocation fails [#25170](https://github.com/arendst/Tasmota/issues/25170)
- LVGL/HASPmota visual touch pointer (dot under finger) [#25158](https://github.com/arendst/Tasmota/issues/25158)
- Berry `tasmota.wakeup_cause()` mapped to `esp_sleep_get_wakeup_cause()` [#25251](https://github.com/arendst/Tasmota/issues/25251)
- Matter Doorbell (0x0148) device type mapped to a Button, with fabric-scoped Binding storage (Chime client not yet implemented) [#25187](https://github.com/arendst/Tasmota/issues/25187)

### Breaking Changed
- BTHome un-reverse MAC address usage for command `MI32Keys` to make it consistent
- Reduce max number of ZCDimmer PWM channels to 5 due to allocated persistent data size [#25222](https://github.com/arendst/Tasmota/issues/25222)

### Changed
- ESP8266 platform update from 2026.09.00 to 2026.11.00 [#25097](https://github.com/arendst/Tasmota/issues/25097)
- ESP32 Platform from 2026.09.50 to 2026.11.50, Framework (Arduino Core) from v3.3.12 to v3.3.12-2 and IDF v5.5.5 [#25097](https://github.com/arendst/Tasmota/issues/25097)
- OTA give up on a stalled OTA after 30s, not 300 reads [#25177](https://github.com/arendst/Tasmota/issues/25177)
- HttpClientLight keep read timeouts longer than 65s [#25180](https://github.com/arendst/Tasmota/issues/25180)
- GUI allow `;` or `{` in a password saved from the settings page [#25184](https://github.com/arendst/Tasmota/issues/25184)
- Refactor JPEG size retrieval function [#25226](https://github.com/arendst/Tasmota/issues/25226)
- OpenTherm use RMT on ESP32, add command `ot_report` [#25254](https://github.com/arendst/Tasmota/issues/25254)
- ESP32 web login uses own Basic auth instead Arduino [#25210](https://github.com/arendst/Tasmota/issues/25210)
- BLE EQ3-TRV avoid sending of empty status [#25182](https://github.com/arendst/Tasmota/issues/25182)

### Fixed
- Security issues caused by buffer overflows
- OTA and file upload security issue
- Function `subStr()` possible buffer overflow
- TuyaV1 and TuyaV2 out-of-bounds accessing `tuya_fnid_map[]` [#25222](https://github.com/arendst/Tasmota/issues/25222)
- Boot loop when using a literal WIFI_DEFAULT_HOSTNAME [#25205](https://github.com/arendst/Tasmota/issues/25205)
- WifiManager name the setup AP as it is turned on [#25169](https://github.com/arendst/Tasmota/issues/25169)
- VEML7700 lux/white rounding, add new command [#25150](https://github.com/arendst/Tasmota/issues/25150)
- MPR121 sizeof [#25203](https://github.com/arendst/Tasmota/issues/25203)
- AHT20/AHT30 stale readings when used with deep sleep, with optional `#define USE_AHT_DIAG` [#25258](https://github.com/arendst/Tasmota/issues/25258)
- SML ESP8266 serial mode [#25201](https://github.com/arendst/Tasmota/issues/25201)
- DisplayText [pN] command now works when followed by another [pN] [#25188](https://github.com/arendst/Tasmota/issues/25188)
- Missing NUL terminator for stored `ModbusBridgeSend` command data [#25218](https://github.com/arendst/Tasmota/issues/25218)
- ESP32 `Restart -1` fails to crash [#25223](https://github.com/arendst/Tasmota/issues/25223)
- ESP32 `PsrMode` displaying whether PSRAM is `QIO`/`OPI` [#25213](https://github.com/arendst/Tasmota/issues/25213)
- ESP32 `PWMFrequency` < 8 result in WDT exception after restart [#25155](https://github.com/arendst/Tasmota/issues/25155)
- ESP32 steady 38 kHz IR carrier (fixes [#25168](https://github.com/arendst/Tasmota/issues/25168)) [#25176](https://github.com/arendst/Tasmota/issues/25176)
- ESP32 `UfsServe` (`UFILESYS_STATIC_SERVING`) compilation, and reject `..` in served paths [#25211](https://github.com/arendst/Tasmota/issues/25211)
- MiEL HVAC Modbus no response timeouts now serves the RS485 slave from its own task [#25157](https://github.com/arendst/Tasmota/issues/25157)
- MiEL HVAC Modbus mirror total energy to holding register 40027 [#25214](https://github.com/arendst/Tasmota/issues/25214)
- ESP32-S3 RGB display permanent image shift after flash writes, DMA sync guard [#25194](https://github.com/arendst/Tasmota/issues/25194)
- Wireguard reconnect when a dynamic DNS `Endpoint` changes IP [#25204](https://github.com/arendst/Tasmota/issues/25204)
- WebAsync rare crash and race conditions [#25209](https://github.com/arendst/Tasmota/issues/25209)
- Berry `lv.start()` raises an error instead of crashing when the display can't start [#25193](https://github.com/arendst/Tasmota/issues/25193)
- Matter event EpochTimestamp sent as seconds instead of milliseconds, using new Berry `tasmota.rtc_utc_ms()` [#25234](https://github.com/arendst/Tasmota/issues/25234)
- LVGL Panel extension crash [#25165](https://github.com/arendst/Tasmota/issues/25165)
- LVGL Panel improved click handling [#25171](https://github.com/arendst/Tasmota/issues/25171)
- LVGL touch more responsive [#25172](https://github.com/arendst/Tasmota/issues/25172)

### Removed
- Support for migration from versions below v9.1
