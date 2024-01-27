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

This release will be supported from ESP32/Arduino library Core version **2.0.14**.

Support of ESP8266 Core versions before 2.7.6 and ESP32 Core versions before 2.0.14 have been removed.

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
- https://github.com/arendst/Tasmota-firmware/tree/main/release-firmware
- http://ota.tasmota.com/tasmota/release

Historical binaries can be downloaded from
- http://ota.tasmota.com/tasmota/release-13.3.0

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

### ESP32-C2 and ESP32-C6 based
The following binary downloads have been compiled with ESP32/Arduino library core version **3.0.0-alpha3**.

- **tasmota32c2-arduino30.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-C2 and 4M+ flash.
- **tasmota32c6cdc-arduino30.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-C6 with serial over embedded USB CDC or hardware USB and 4M+ flash.

Latest released binaries can be downloaded from
- https://github.com/arendst/Tasmota-firmware/tree/main/release-firmware
- https://ota.tasmota.com/tasmota32/release

Historical binaries can be downloaded from
- https://ota.tasmota.com/tasmota32/release-13.3.0

The latter links can be used for OTA upgrades too like ``OtaUrl https://ota.tasmota.com/tasmota32/release/tasmota32.bin``

## Additional information

[List](MODULES.md) of embedded modules.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog v13.3.0.4
### Added
- Command ``TimedPower<index> <milliseconds>[,ON|OFF|TOGGLE|BLINK]`` executes ``Power<index> [ON|OFF|TOGGLE|BLINK] `` and after <millisecond> executes ``Power<index> [OFF|ON|TOGGLE|OFF]``
- Display of active drivers using command ``status 4``
- GPIO Viewer to see realtime GPIO states. Enable with define USE_GPIO_VIEWER
- GPIO Viewer user selection of assets website now defaults to `https://ota.tasmota.com/tasmota|tasmota32/gpio_viewer/assets`
- Support for CST816S touch interface [#20213](https://github.com/arendst/Tasmota/issues/20213)
- Support for Sonoff Basic R4 Magic Switch [#20247](https://github.com/arendst/Tasmota/issues/20247)
- Support negative power on BL0942 using index 5..8 [#20322](https://github.com/arendst/Tasmota/issues/20322)
- Support for pipsolar inverter [#20408](https://github.com/arendst/Tasmota/issues/20408)
- Support for HardwareSerial invert [#15461](https://github.com/arendst/Tasmota/issues/15461)
- NeoPool hydrolysis FL1 and Redox flag [#20258](https://github.com/arendst/Tasmota/issues/20258)
- SML support for IM350 [#20474](https://github.com/arendst/Tasmota/issues/20474)
- GUI sensor separators [#20495](https://github.com/arendst/Tasmota/issues/20495)
- ESP32 used UART information
- ESP32 support GPIOViewer when ``define USE_ESP32_GPIO_VIEWER`` is enabled
- ESP32 MI BLE support for Xiaomi LYWSD02MMC [#20381](https://github.com/arendst/Tasmota/issues/20381)
- ESP32 support for Shelly Plus Add-On using DS18x20 or DHT11/AM2301/DHT21/DHT22/AM2302/AM2321/SI7021 on GPIO0/1 (#20580)
 [#20580](https://github.com/arendst/Tasmota/issues/20580)
- ESP32-C3 support for GPIO11 [#18350](https://github.com/arendst/Tasmota/issues/18350)
- Berry GPIO viewer initial version using async webserver [#20416](https://github.com/arendst/Tasmota/issues/20416)
- Berry `introspect.set()` for class attributes [#20339](https://github.com/arendst/Tasmota/issues/20339)
- Berry support for `tcpclientasync` in `tcpserver` [#20401](https://github.com/arendst/Tasmota/issues/20401)
- Berry `tasmota.urlbecload(url:string) -> bool` [#20412](https://github.com/arendst/Tasmota/issues/20412)
- Berry `gpio.read_pwm` and `gpio.read_pwm_resolution` [#20414](https://github.com/arendst/Tasmota/issues/20414)
- Berry `gpio.get_pin_type` and `gpio.ger_pin_type_index` [#20415](https://github.com/arendst/Tasmota/issues/20415)
- Berry `string` to `bytes()` [#20420](https://github.com/arendst/Tasmota/issues/20420)
- Berry button to dynamically load GPIO Viewer with Berry backend [#20424](https://github.com/arendst/Tasmota/issues/20424)
- Berry `debug_panel.tapp` to display real-time heap and wifi rssi [#20436](https://github.com/arendst/Tasmota/issues/20436)
- Berry `webserver.header` to read browser sent headers [#20447](https://github.com/arendst/Tasmota/issues/20447)
- Berry provide lightweight options for `tasmota.wifi/eth/memory/rtc` [#20448](https://github.com/arendst/Tasmota/issues/20448)
- Berry `tasmota.webcolor` [#20454](https://github.com/arendst/Tasmota/issues/20454)
- Berry `debug.caller` [#20470](https://github.com/arendst/Tasmota/issues/20470)
- Berry solidification of strings longer than 255 bytes [#20529](https://github.com/arendst/Tasmota/issues/20529)
- Berry syntax coloring for Notepad++ by FransO [#20541](https://github.com/arendst/Tasmota/issues/20541)
- Berry/Zigbee web hook per device for customized status display [#20542](https://github.com/arendst/Tasmota/issues/20542)
- Zigbee ``ZbEmulation`` to selectively exclude some devices from Hue/Alexa emulation [#20552](https://github.com/arendst/Tasmota/issues/20552)
- LVGL `lv.str_arr` [#20480](https://github.com/arendst/Tasmota/issues/20480)
- LVGL option to add `lv.keyboard` extra widget [#20496](https://github.com/arendst/Tasmota/issues/20496)
- HASPmota `haspmota.page_show()` to change page [#20333](https://github.com/arendst/Tasmota/issues/20333)
- HASPmota type `chart` [#20372](https://github.com/arendst/Tasmota/issues/20372)
- HASPmota support for `min` and `max` attribute in `slider` [#20582](https://github.com/arendst/Tasmota/issues/20582)
- Matter support for password for remote Tasmota devices [#20296](https://github.com/arendst/Tasmota/issues/20296)

### Breaking Changed
- Refactoring of Berry `animate` module for WS2812 Leds [#20236](https://github.com/arendst/Tasmota/issues/20236)

### Changed
- ESP8266 platform update from 2023.04.00 to 2024.01.01 [#20539](https://github.com/arendst/Tasmota/issues/20539)
- ESP8266 Framework (Arduino Core) from v2.7.4.9 to v2.7.6 [#20539](https://github.com/arendst/Tasmota/issues/20539)
- ESP32 Core2 platform update from 2023.11.01 to 2024.01.01 [#20473](https://github.com/arendst/Tasmota/issues/20473)
- ESP32 Core3 platform update from 2024.01.11 to 2024.01.12 [#20576](https://github.com/arendst/Tasmota/issues/20576)
- Library OneWire-Stickbreaker by TasmotaOneWire supporting Shelly Plus Add-On [#20580](https://github.com/arendst/Tasmota/issues/20580)
- Renamed button "Consoles" to "Tools"
- Refactored rule ``Subscribe`` using LList allowing full message size and enabled by default
- Refactored rules USE_EXPRESSION and SUPPORT_IF_STATEMENT replacing LinkedList with arrays and enabled by default
- Support syslog updates every sleep or every second if `#define SYSLOG_UPDATE_SECOND` [#20260](https://github.com/arendst/Tasmota/issues/20260)
- Web file upload response on upload error [#20340](https://github.com/arendst/Tasmota/issues/20340)
- Header `Host` is now collected by Webserver [#20446](https://github.com/arendst/Tasmota/issues/20446)
- Utouch optimizations, rgb i2c init [#20596](https://github.com/arendst/Tasmota/issues/20596)
- Webcam tweaks [#20451](https://github.com/arendst/Tasmota/issues/20451)
- IP stack compatible with new Core3 IPv6 implementation [#20509](https://github.com/arendst/Tasmota/issues/20509)
- Refactored Pio filesystem download script [#20544](https://github.com/arendst/Tasmota/issues/20544)

### Fixed
- CVE-2021-36603 Cross Site Scripting (XSS) vulnerability [#12221](https://github.com/arendst/Tasmota/issues/12221)
- Syslog server warning caused by lack of <PRI> field and hostname starting with 'z' [#14689](https://github.com/arendst/Tasmota/issues/14689)
- Support for Domoticz floor/room topics. Regression from v12.0.1 [#20299](https://github.com/arendst/Tasmota/issues/20299)
- Scripter memory leak in `>w x` [#20473](https://github.com/arendst/Tasmota/issues/20473)
- Zigbee ramdom crash in main page [#20481](https://github.com/arendst/Tasmota/issues/20481)
- ESP8266 IPv6 support [#20539](https://github.com/arendst/Tasmota/issues/20539)
- ESP32 piezo ceramic buzzer doesn't buzz [#20118](https://github.com/arendst/Tasmota/issues/20118)
- ESP32 shutter exception 6 (divide by zero) on ``ShutterMode 4`` [#20524](https://github.com/arendst/Tasmota/issues/20524)
- ESP32 Zigbee Aqara attributes [#20452](https://github.com/arendst/Tasmota/issues/20452)
- ESP32 Audio for Core3, MP3Stream and Shine [#20540](https://github.com/arendst/Tasmota/issues/20540)
- ESP32 Core3 reset GPIOs 16/17 when PSRAM is not used [#20547](https://github.com/arendst/Tasmota/issues/20547)
- LVGL fix type for lv_imgbtn [#20354](https://github.com/arendst/Tasmota/issues/20354)
- Berry claiming UART0 if needed [#20324](https://github.com/arendst/Tasmota/issues/20324)
- Berry assigment to list with negative index [#20537](https://github.com/arendst/Tasmota/issues/20537)
- Berry C mapping, raise an error if too many arguments are sent [#20604](https://github.com/arendst/Tasmota/issues/20604)
- Matter Contact sensor was not triggering any update [#20232](https://github.com/arendst/Tasmota/issues/20232)
- Matter support for Alexa [#20545](https://github.com/arendst/Tasmota/issues/20545)

### Removed
- Max number of 30 backlog entries
