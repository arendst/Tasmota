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

In addition to TLS using fingerprints now also user supplied CA certs, AWS IoT and Azure IoT is supported. Read [full documentation](https://tasmota.github.io/docs/AWS-IoT)

## Initial configuration tools

For initial configuration this release supports Webserver based **WifiManager** or **Serial** based command interface only. Support for **WPS** and **SmartConfig** has been removed.

## Provided Binary Downloads

### ESP8266 or ESP8285 based
The following binary downloads have been compiled with ESP8266/Arduino library core version **2.7.4.9**.

- **tasmota.bin** = The Tasmota version with most drivers for 1M+ flash. **RECOMMENDED RELEASE BINARY**
- **tasmota-AF.bin** to **tasmota-VN.bin** = The Tasmota version in different languages for 1M+ flash.
- **tasmota-lite.bin** = The Lite version without most drivers and sensors for 1M+ flash.
- **tasmota-knx.bin** = The Knx version without some features but adds KNX support for 1M+ flash.
- **tasmota-sensors.bin** = The Sensors version adds more useful sensors for 1M+ flash.
- **tasmota-ir.bin** = The InfraRed Receiver and transmitter version allowing all available protocols provided by library IRremoteESP8266 but without most other features for 1M+ flash.
- **tasmota-display.bin** = The Display version without Energy Monitoring but adds display support for 1M+ flash.
- **tasmota-zbbridge.bin** = The dedicated Sonoff Zigbee Bridge version for 2M+ flash.
- **tasmota-zigbee.bin** = The dedicated cc25xx Zigbee Bridge version for 4M+ flash.
- **tasmota-minimal.bin** = The Minimal version allows intermediate OTA uploads to support larger versions and does NOT change any persistent parameter for 1M+ flash. This version **should NOT be used for initial installation**.

Above binaries are also available as gzipped version allowing faster uploads.

Latest released binaries can be downloaded from
- https://github.com/arendst/Tasmota/tree/release-firmware
- http://ota.tasmota.com/tasmota/release

Historical binaries can be downloaded from
- http://ota.tasmota.com/tasmota/release-9.5.0

The latter links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota/release/tasmota.bin.gz``

### ESP32 based
The following binary downloads have been compiled with ESP32/Arduino library core version **1.0.6**.

- **tasmota32.bin** = The Tasmota version with most drivers including additional sensors and KNX for 4M+ flash.  **RECOMMENDED RELEASE BINARY**
- **tasmota32_8M.bin** = The Tasmota version with most drivers including additional sensors and KNX for 8M+ flash.
- **tasmota32_16M.bin** = The Tasmota version with most drivers including additional sensors and KNX for 16M+ flash.
- **tasmota32solo1.bin** = The Tasmota version with most drivers including additional sensors and KNX for single core ESP32 and 4M+ flash.
- **tasmota32-AF.bin** to **tasmota32-VN.bin** = The Tasmota version in different languages for 4M+ flash.
- **tasmota32-ir.bin** = The InfraRed Receiver and transmitter version allowing all available protocols provided by library IRremoteESP8266 but without most other features for 4M+ flash.
- **tasmota32-display.bin** = The Display version without Energy Monitoring but adds display support for 4M+ flash.
- **tasmota32-webcam.bin** = The Webcam version adds webcam support for 4M+ flash.
- **tasmota32-bluetooth.bin** = The Bluetooth version adds BLE support for 4M+ flash.
- **tasmota32-odroidgo.bin** = The Odroid-Go version is specifically tailored to Odroid_go hardware for 8M+ flash.
- **tasmota32-core2.bin** = The Core2 version is specifically tailored to M5Stack Core2 hardware for 8M+ flash.

Latest released binaries can be downloaded from
- https://github.com/arendst/Tasmota/tree/release-firmware
- http://ota.tasmota.com/tasmota32/release

Historical binaries can be downloaded from
- http://ota.tasmota.com/tasmota32/release-9.5.0

The latter links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota32/release/tasmota32.bin``

## Additional information

[List](MODULES.md) of embedded modules.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog v9.5.0 Michael (Rossi)
### Added
- Command ``Status0`` providing all status information on a single line
- Command ``TuyaSend5`` for hex string [#12211](https://github.com/arendst/Tasmota/issues/12211)
- Command ``MqttWifiTimeout 100..20000`` to control MQTT Wi-Fi connection timeout default set to 200 mS [#12222](https://github.com/arendst/Tasmota/issues/12222)
- Commands ``Color2`` and ``Dimmer4`` to allow retaining brightness ratio between white and color channels when setting dimmer for linked lights [#12072](https://github.com/arendst/Tasmota/issues/12072)
- Extend command ``Wifi`` with Wi-Fi Mode Control [#12292](https://github.com/arendst/Tasmota/issues/12292)
- Defines ``USER_RULE1``, ``USER_RULE2`` and ``USER_RULE3`` to store rules at compile time
- Define ``USER_BACKLOG`` to store commands at compile time to be executed at firmware load or when executing command ``reset``
- Support for optional ``Template`` JSON fieldpair ``"CMND":"<any template related command>|<any template related command>|..."`` [#11788](https://github.com/arendst/Tasmota/issues/11788)
- ESP32 pulldown buttons ``Button_d`` and ``Button_id`` and switches ``Switch_d`` [#10814](https://github.com/arendst/Tasmota/issues/10814)
- Support for MQTT using Azure IoT Hub by Kevin Saye [#11906](https://github.com/arendst/Tasmota/issues/11906)
- Make Telegram command ``TmState`` persistent [#11965](https://github.com/arendst/Tasmota/issues/11965)
- Zigbee binary supporting cc25xx hardware on ESP8266 4M flash hardware [#11872](https://github.com/arendst/Tasmota/issues/11872)
- Zigbee firmware for Tube's Zigbee coordinator based on EFR32 and ESP32
- Zigbee firmware 6.7.9 for Sonoff ZBBridge
- LVGL support for 3 buttons as rotary encoder [#12035](https://github.com/arendst/Tasmota/issues/12035)
- LVGL support for touchscreen [#12039](https://github.com/arendst/Tasmota/issues/12039)
- LVGL support for PSRAM [#12062](https://github.com/arendst/Tasmota/issues/12062)
- LVGL support for TrueType fonts via FreeType library [#12087](https://github.com/arendst/Tasmota/issues/12087)
- LVGL support for PNG images [#12148](https://github.com/arendst/Tasmota/issues/12148)
- Allow Home Assistant discovery of MCP2300xx output as relay [#12037](https://github.com/arendst/Tasmota/issues/12037)
- Preliminary support for Esp32C3 - RiscV based
- Support for voltage and current monitoring when using Shelly dimmer 2 hardware [#11988](https://github.com/arendst/Tasmota/issues/11988)
- Support for Azure Device Provisioning Service for IoT Hub [#12056](https://github.com/arendst/Tasmota/issues/12056)
- Show new IP after the Wi-Fi Initial Config [#12091](https://github.com/arendst/Tasmota/issues/12091)
- Update Sugar Valley Neopool driver [#12171](https://github.com/arendst/Tasmota/issues/12171)
- Acer projector support [#12190](https://github.com/arendst/Tasmota/issues/12190)
- I2S and Interrupt GPIO types [#12192](https://github.com/arendst/Tasmota/issues/12192)
- Update OpenTherm driver [#12195](https://github.com/arendst/Tasmota/issues/12195)
- Support for BM8563 RTC chip (I2C) found in M5Stack Core2 and M5StickC [#12199](https://github.com/arendst/Tasmota/issues/12199)

### Changed
- IRremoteESP8266 library from v2.7.16 to v2.7.18
- NeoPixelBus library from v2.6.1.4 to v2.6.3 stage
- Shelly Dimmer 1 and 2 stm32 firmware from v51.5 to v51.6
- Allow longer MQTT response messages by removing fixed memory buffer with size 1040 to heap allocated buffer
- Command ``Timers`` layout of JSON message changed to single line
- Command ``Gpio`` layout of JSON message changed to single line
- Command ``Modules`` layout of JSON message changed to single line
- ESP32 increase log buffer from 4k to 6k to support longer messages
- Move Settings from DRAM to heap
- Refactor platformio configurations by Jason2866
- WifiManager save Wi-Fi configuration from settings, do it only once [#12242](https://github.com/arendst/Tasmota/issues/12242)
- Improving SI7021 reading reliability by adjusting timers [#12256](https://github.com/arendst/Tasmota/issues/12256)
- Refactor ESP32 partition selection, now via boards [#12257](https://github.com/arendst/Tasmota/issues/12257)
- Use correct template for Home Assistant light [#12317](https://github.com/arendst/Tasmota/issues/12317)

### Fixed
- Wrong flash size detection when saving Zigbee device information on ESP8266 [#11870](https://github.com/arendst/Tasmota/issues/11870)
- Prometheus metrics parse error on DS18x20 [#11931](https://github.com/arendst/Tasmota/issues/11931)
- DS18x20 name search id [#11958](https://github.com/arendst/Tasmota/issues/11958)
- Avoid erasing of Zigbee data if zigbee is not started [#11961](https://github.com/arendst/Tasmota/issues/11961)
- Zigbee XModem retries [#11967](https://github.com/arendst/Tasmota/issues/11967)
- Teleinfo standard mode and blacklist feature crash [#11991](https://github.com/arendst/Tasmota/issues/11991)
- ESP32 Hue light [#12005](https://github.com/arendst/Tasmota/issues/12005)
- Map received CCT channels back in DevGroups [#12044](https://github.com/arendst/Tasmota/issues/12044)
- Increase TLS minimum stack thunk to 3800 bytes [#12063](https://github.com/arendst/Tasmota/issues/12063)
- Delay discovery of PZEM sensors [#12076](https://github.com/arendst/Tasmota/issues/12076)
- PING race condition breaks JSON in rule [#12106](https://github.com/arendst/Tasmota/issues/12106)
- Support Tuya powermeter >6500W [#12115](https://github.com/arendst/Tasmota/issues/12115)
- Zigbee max end-device [#12159](https://github.com/arendst/Tasmota/issues/12159)
- Prevent keep state MCP230xx output fast toggle on reboot [#12264](https://github.com/arendst/Tasmota/issues/12264)
- Tuya data type 2 read as 32 bit integer (instead of 16 bit) [#12282](https://github.com/arendst/Tasmota/issues/12282)

### Noted
- ESP32 single core **tasmota32solo1.bin** binary can only be uploaded using the GUI as OTA upload will trigger the watchdog timer
