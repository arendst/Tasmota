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

### Known issues

Due to the change from ESP32 Arduino Core2/IDF4 to Arduino Core3/IDF5 not all functionality has been restored. The following features are known not to work on ESP32:
- Wifi Range Extender

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
- http://ota.tasmota.com/tasmota/release-13.4.0

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
- https://ota.tasmota.com/tasmota32/release-13.4.0

The latter links can be used for OTA upgrades too like ``OtaUrl https://ota.tasmota.com/tasmota32/release/tasmota32.bin``

## Additional information

[List](MODULES.md) of embedded modules.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog v13.4.1.2
### Added
- PlatformIO target reset [#21292](https://github.com/arendst/Tasmota/issues/21292)
- Command ``Wifi 6`` to enable 11ax on ESP32
- Command ``PowerLock`` to disable power control of selected outputs [#21081](https://github.com/arendst/Tasmota/issues/21081)
- Command ``Publish3`` to send binary data encoded as Hex, disabled in safeboot [#21329](https://github.com/arendst/Tasmota/issues/21329)
- Support for calculated heat index if temperature and humidity is available with ``#define USE_HEAT_INDEX`` [#4771](https://github.com/arendst/Tasmota/issues/4771)
- Support for LoRa and single channel EU863-870 LoRaWanBridge [#17790](https://github.com/arendst/Tasmota/issues/17790)
- Support for AMS5915/AMS6915 temperature and pressure sensors [#20814](https://github.com/arendst/Tasmota/issues/20814)
- Support for LWT messages in TasMesh [#20392](https://github.com/arendst/Tasmota/issues/20392)
- Support IR data larger than 64 bits [#20831](https://github.com/arendst/Tasmota/issues/20831)
- Support for MCP23S08 [#20971](https://github.com/arendst/Tasmota/issues/20971)
- Support for ESP32-S3 120Mhz [#20973](https://github.com/arendst/Tasmota/issues/20973)
- Support Azure iothub direct method [#21013](https://github.com/arendst/Tasmota/issues/21013)
- Support for Domoticz non-persistent ``DzIdx5`` to ``DzIdx32`` and disabling DOMOTICZ_OUT_TOPIC subscribe using command ``DzIdx0 0`` [#21019](https://github.com/arendst/Tasmota/issues/21019)
- Support SPI GPIO configuration for Universal Touch Screen [#21025](https://github.com/arendst/Tasmota/issues/21025)
- Support for SPL06_007 pressure and temperature sensor [#21185](https://github.com/arendst/Tasmota/issues/21185)
- Support for AHT30 Temperature and Humidity Sensor [#19922](https://github.com/arendst/Tasmota/issues/19922)
- Support for compile time hostname with `#define WIFI_DEFAULT_HOSTNAME` (#21236)[#21236](https://github.com/arendst/Tasmota/issues/21236)
- GPIOViewer pin mode support
- Zigbee support for attributes of type `uint48` used by energy monitoring [#20992](https://github.com/arendst/Tasmota/issues/20992)
- QMC5883l check for overflow and scale reading [#20643](https://github.com/arendst/Tasmota/issues/20643)
- TCP Serial bridge GPIO type `TCP Tx En` [#21269](https://github.com/arendst/Tasmota/issues/21269)
- ESP32 esp32_partition_app3904k_fs3392k partition scheme for 8MB ESP32S3 [#21241](https://github.com/arendst/Tasmota/issues/21241)
- ESP32 Compile option disabling PSRam check to avoid "blinking" of GPIO 16/17 at startup [#21282](https://github.com/arendst/Tasmota/issues/21282)
- Berry explicit error log when memory allocation fails [#20807](https://github.com/arendst/Tasmota/issues/20807)
- Berry `path.rename()` [#20840](https://github.com/arendst/Tasmota/issues/20840)
- Berry `string.startswith`, `string.endswith` and `%q` format [#20909](https://github.com/arendst/Tasmota/issues/20909)
- Berry `close()` to class `serial` [#21042](https://github.com/arendst/Tasmota/issues/21042)
- Berry `flash.current_ota` [#21097](https://github.com/arendst/Tasmota/issues/21097)
- Berry `webserver.content_close()` [#21276](https://github.com/arendst/Tasmota/issues/21276)
- Berry wave file recorder [#21315](https://github.com/arendst/Tasmota/issues/21315)
- Berry `after_teleperiod` event matching `FUNC_AFTER_TELEPERIOD` [#21351](https://github.com/arendst/Tasmota/issues/21351)
- LVGL and HASPmota typicons font [#20742](https://github.com/arendst/Tasmota/issues/20742)
- LVGL `lv.draw_label_dsc` and `lv_bar.get_indic_area` [#20936](https://github.com/arendst/Tasmota/issues/20936)
- HASPmota `p<x>b<y>.delete` to delete an object [#20735](https://github.com/arendst/Tasmota/issues/20735)
- HASPmota improve arc and img [#20894](https://github.com/arendst/Tasmota/issues/20894)
- HASPmota support for scale, percentages [#20974](https://github.com/arendst/Tasmota/issues/20974)
- HASPmota `dropdown_list` and fixes [#21208](https://github.com/arendst/Tasmota/issues/21208)
- HASPmota demo of Renaissance Watch for 480x480 displays [#21290](https://github.com/arendst/Tasmota/issues/21290)

### Breaking Changed
- Drop support for old (insecure) fingerprint format [#20842](https://github.com/arendst/Tasmota/issues/20842)
- Removed dedicated touch drivers in favour of Universal Touch driver [#21146](https://github.com/arendst/Tasmota/issues/21146)
- ESP32-C3 OTA binary name from `tasmota32c3cdc.bin` to `tasmota32c3.bin` with USB HWCDC and fallback to serial [#21212](https://github.com/arendst/Tasmota/issues/21212)
- ESP32-C6 OTA binary name from `tasmota32c6cdc.bin` to `tasmota32c6.bin` with USB HWCDC and fallback to serial [#21212](https://github.com/arendst/Tasmota/issues/21212)
- ESP32-S3 OTA binary name from `tasmota32s3cdc.bin` to `tasmota32s3.bin` with USB HWCDC and fallback to serial [#21212](https://github.com/arendst/Tasmota/issues/21212)
- Berry loading .be file does not generated .bec anymore [#21075](https://github.com/arendst/Tasmota/issues/21075)
- LVGL remove embedded typicons font [#20872](https://github.com/arendst/Tasmota/issues/20872)
- LVGL remove `textarea` and `spinbox` from binaries [#20916](https://github.com/arendst/Tasmota/issues/20916)

### Changed
- ESP32 Framework (Arduino Core) from v2.0.14 to v3.0.0
- ESP32 platform update from 2024.01.12 to 2024.05.11 [#21381](https://github.com/arendst/Tasmota/issues/21381)
- ESP32 LVGL library from v9.0.0 to v9.1.0 [#21008](https://github.com/arendst/Tasmota/issues/21008)
- GPIOViewer from v1.5.0 to v1.5.2
- Seriallog set to `SERIAL_LOG_LEVEL` at boot [#21363](https://github.com/arendst/Tasmota/issues/21363)
- TLS Letsencrypt replace R3 CA with long-term ISRG_Root_X1 CA, which works with R3 and R10-R14 [#21352](https://github.com/arendst/Tasmota/issues/21352)
- Command ``EthType`` option selection [#21317](https://github.com/arendst/Tasmota/issues/21317)
- Refactor Platformio script `post_esp32.py` [#20966](https://github.com/arendst/Tasmota/issues/20966)
- SGP4x Domoticz air quality value from raw to computed [#18880](https://github.com/arendst/Tasmota/issues/18880)
- NeoPool webUI pH alarms (4 & 5) completed [#20743](https://github.com/arendst/Tasmota/issues/20743)
- Prevent shutter MQTT broadcast with activated ShutterLock [#20827](https://github.com/arendst/Tasmota/issues/20827)
- Some `display.ini` to utouch [#21029](https://github.com/arendst/Tasmota/issues/21029)
- KNX format of energy to match specifications [#21074](https://github.com/arendst/Tasmota/issues/21074)
- Internal macro `APP_SLEEP` to `TASMOTA_SLEEP` to specify default sleep in ms [#21324](https://github.com/arendst/Tasmota/issues/21324)
- uDisplay fast drawing on RGB displays [#21257](https://github.com/arendst/Tasmota/issues/21257)
- HDMI CEC synchronously sends messages [#21270](https://github.com/arendst/Tasmota/issues/21270)
- Refactor I2S [#21291](https://github.com/arendst/Tasmota/issues/21291)
- Zigbee startup event triggered after plugins are loaded [#21320](https://github.com/arendst/Tasmota/issues/21320)
- Refactor Tensorflow [#21327](https://github.com/arendst/Tasmota/issues/21327)
- ESP32 refactored Wifi for ESP32 Core3 release [#21106](https://github.com/arendst/Tasmota/issues/21106)
- ESP32 WiFi phy modes 11n and 11ax represented as HT20, HT40 and HE20 [#19350](https://github.com/arendst/Tasmota/issues/19350)
- berry.exe (pre-compiled for Windows) updated to latest Berry patches [#21024](https://github.com/arendst/Tasmota/issues/21024)
- Berry class `int64` made immutable [#20727](https://github.com/arendst/Tasmota/issues/20727)
- Matter reduce memory usage when reading with wildcards [#20809](https://github.com/arendst/Tasmota/issues/20809)
- LVGL make lv_touch_3_buttons more responsive [#20728](https://github.com/arendst/Tasmota/issues/20728)
- LVGL optimize fonts and add icons [#20880](https://github.com/arendst/Tasmota/issues/20880)
- LVGL improved readability of montserrat-10 [#20900](https://github.com/arendst/Tasmota/issues/20900)
- HASPmota fix and improve demo with pixel-perfect fonts [#20734](https://github.com/arendst/Tasmota/issues/20734)
- HASPmota more attributes [#20744](https://github.com/arendst/Tasmota/issues/20744)
- HASPmota support for spangroup (styled text) [#20852](https://github.com/arendst/Tasmota/issues/20852)
- HASPmota support for led [#20857](https://github.com/arendst/Tasmota/issues/20857)
- HASPmota moved to a distinct library `lv_haspmota` [#20929](https://github.com/arendst/Tasmota/issues/20929)
- HASPmota solidify server-side [#20938](https://github.com/arendst/Tasmota/issues/20938)

### Fixed
- Filesystem save of JSON settings data
- Fade out on CCT bulb with `SO92 1` [#21159](https://github.com/arendst/Tasmota/issues/21159)
- Shutter inverted using internal commands [#20752](https://github.com/arendst/Tasmota/issues/20752)
- TuyaV2 suppressed dimmer updates from MQTT [#20950](https://github.com/arendst/Tasmota/issues/20950)
- Scripter google char memory leak [#20995](https://github.com/arendst/Tasmota/issues/20995)
- Too restrictive checksum checks in Lib_teleinfo [#21033](https://github.com/arendst/Tasmota/issues/21033)
- Color swap option for rgb displaytext [#21049](https://github.com/arendst/Tasmota/issues/21049)
- NeoPool hydrolysis unit for Hidrolife, Bionet and Generic device [#21098](https://github.com/arendst/Tasmota/issues/21098)
- Neopool prevent possible multiple bus requests [#21267](https://github.com/arendst/Tasmota/issues/21267)
- Async HMDI CEC [#21287](https://github.com/arendst/Tasmota/issues/21287)
- ESP8266 physical button/switch control when no rules activated [#21187](https://github.com/arendst/Tasmota/issues/21187)
- jpeg compile core3 [#21387](https://github.com/arendst/Tasmota/issues/21387)
- ESP32 PWM activity on unconfigured PWM GPIOs [#20732](https://github.com/arendst/Tasmota/issues/20732)
- Avoid unwanted OTA upgrade when safeboot starts for the first time [#21360](https://github.com/arendst/Tasmota/issues/21360)
- BTHome, prep BLE5 [#20989](https://github.com/arendst/Tasmota/issues/20989)
- Conflicting log_level definitions in NimBLE [#21337](https://github.com/arendst/Tasmota/issues/21337)
- Berry Memory leak in `import re` [#20823](https://github.com/arendst/Tasmota/issues/20823)
- Berry bug when parsing ternary operator [#20839](https://github.com/arendst/Tasmota/issues/20839)
- Berry walrus with member or index [#20939](https://github.com/arendst/Tasmota/issues/20939)
- Berry walrus bug when assigning to self [#21015](https://github.com/arendst/Tasmota/issues/21015)
- Berry `web_add_handler` called before `Webserver` is initialized [#21272](https://github.com/arendst/Tasmota/issues/21272)
- Berry `math.inf`, `math.isinf()` and fixed json ouput for `inf` and `nan` [#21304](https://github.com/arendst/Tasmota/issues/21304)
- Matter broken NOCStruct types preventing pairing with HA [#21365](https://github.com/arendst/Tasmota/issues/21365)
- LVGL restore `lv_palette` functions [#21232](https://github.com/arendst/Tasmota/issues/21232)
- LVGL fix memory allocation of flush buffers [#21256](https://github.com/arendst/Tasmota/issues/21256)
- HASPmota PSRAM memory leak [#20818](https://github.com/arendst/Tasmota/issues/20818)
- HASPmota widgets line, btnmatrix, qrcode, bar, checkbox [#20881](https://github.com/arendst/Tasmota/issues/20881)
- HASPmota demo and robotocondensed fonts [#21014](https://github.com/arendst/Tasmota/issues/21014)
- HASPmota dropdown class "options" attribute [#21203](https://github.com/arendst/Tasmota/issues/21203)
- HASPmota `align` attribute and expand PNG cache [#21228](https://github.com/arendst/Tasmota/issues/21228)

### Removed
- Support for ESP32 Arduino Core 2 [#21180](https://github.com/arendst/Tasmota/issues/21180)
- Unused `#define MQTT_DATA_STRING` support
- ILI9341 driver replaced with uDisplay [#21169](https://github.com/arendst/Tasmota/issues/21169)
- SSD1306 driver replaced with uDisplay [#21176](https://github.com/arendst/Tasmota/issues/21176)
- SSD1331 driver replaced with uDisplay [#21177](https://github.com/arendst/Tasmota/issues/21177)
- SSH1106 driver replaced with uDisplay [#21183](https://github.com/arendst/Tasmota/issues/21183)
- SSD1351 driver replaced with uDisplay [#21184](https://github.com/arendst/Tasmota/issues/21184)
- ST7789 driver replaced with uDisplay [#21184](https://github.com/arendst/Tasmota/issues/21184)
- ESP32 IDF 4.4 based I2S code [#21188](https://github.com/arendst/Tasmota/issues/21188)
- Crash recorder from safeboot [#21332](https://github.com/arendst/Tasmota/issues/21332)
- Berry `print "a"` syntax no longer supported [#21048](https://github.com/arendst/Tasmota/issues/21048)
- LVGL disabled vector graphics [#21242](https://github.com/arendst/Tasmota/issues/21242)
