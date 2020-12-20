<img src="https://github.com/arendst/Tasmota/blob/master/tools/logo/TASMOTA_FullLogo_Vector.svg" alt="Logo" align="right" height="76"/>

# RELEASE NOTES

## Migration Information

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

The following binary downloads have been compiled with ESP8266/Arduino library core version **2.7.4.9**.

- **tasmota.bin** = The Tasmota version with most drivers. **RECOMMENDED RELEASE BINARY**
- **tasmota-BG.bin** to **tasmota-TW.bin** = The Tasmota version in different languages.
- **tasmota-lite.bin** = The Lite version without most drivers and sensors.
- **tasmota-knx.bin** = The Knx version without some features but adds KNX support.
- **tasmota-sensors.bin** = The Sensors version adds more useful sensors.
- **tasmota-ir** = The InfraRed Receiver and transmitter version allowing all available protocols provided by library IRremoteESP8266 but without most other features.
- **tasmota-display.bin** = The Display version without Energy Monitoring but adds display support.
- **tasmota-zbbridge.bin** = The dedicated Sonoff Zigbee Bridge version.
- **tasmota-minimal.bin** = The Minimal version allows intermediate OTA uploads to support larger versions and does NOT change any persistent parameter. This version **should NOT be used for initial installation**.

The attached binaries can also be downloaded from http://ota.tasmota.com/tasmota/release for ESP8266 or http://ota.tasmota.com/tasmota32/release for ESP32. The links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota/release/tasmota.bin.gz``

[List](MODULES.md) of embedded modules.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog v9.2.0 Julie
### Added
- Command ``SetOption115 1`` to enable ESP32 MiBle
- Command ``SetOption116 1`` to disable auto-query of zigbee light devices (avoids network storms with large groups)
- Command ``SetOption117 1`` for light fading to be fixed duration instead of fixed slew rate [#10109](https://github.com/arendst/Tasmota/issues/10109)
- Command ``RfProtocol`` to control RcSwitch receive protocols by BBBits [#10063](https://github.com/arendst/Tasmota/issues/10063)
- Commands ``TuyaRGB``, ``TuyaEnum`` and ``TuyaEnumList`` [#9769](https://github.com/arendst/Tasmota/issues/9769)
- Zigbee command ``ZbInfo`` and prepare support for EEPROM
- Zigbee command ``ZbLeave`` to unpair a device
- Zigbee support for Mi Door and Contact [#9759](https://github.com/arendst/Tasmota/issues/9759)
- Zigbee alarm persistence [#9785](https://github.com/arendst/Tasmota/issues/9785)
- Zigbee persistence of device/sensor data in EEPROM (only ZBBridge)
- Zigbee better support for Tuya Protocol [#10074](https://github.com/arendst/Tasmota/issues/10074)
- Zigbee visual map of network
- TyuaMcu update 2/3 by Federico Leoni [#10004](https://github.com/arendst/Tasmota/issues/10004)
- Support for additional EZO sensors by Christopher Tremblay
- Support for AS608 optical and R503 capacitive fingerprint sensor
- Support for Shelly Dimmer 1 and 2 by James Turton [#9854](https://github.com/arendst/Tasmota/issues/9854)
- Support for common anode sevenseg displays by adding ``#define USE_DISPLAY_SEVENSEG_COMMON_ANODE`` by Ken Sanislo [#9963](https://github.com/arendst/Tasmota/issues/9963)
- Support for multiple WeMo devices by Magic73 [#9208](https://github.com/arendst/Tasmota/issues/9208)
- Support for SPI connected MFRC522 13.56MHz rfid card reader [#9916](https://github.com/arendst/Tasmota/issues/9916)
- Support ESP32 SPIFFS for internal use
- KNX read reply for Power (#9236, #9891)
- Fallback NTP server from x.pool.ntp.org if no ntpservers are configured
- Optional CCloader support for CC25xx Zigbee or CC26xx BLE by Christian Baars [#9970](https://github.com/arendst/Tasmota/issues/9970)
- Letsencrypt R3 in addition to X3 CA [#10086](https://github.com/arendst/Tasmota/issues/10086)

### Breaking Changed
- KNX DPT9 (16-bit float) to DPT14 (32-bit float) by Adrian Scillato [#9811](https://github.com/arendst/Tasmota/issues/9811) [#9888](https://github.com/arendst/Tasmota/issues/9888)

### Changed
- Core library from v2.7.4.5 to v2.7.4.9
- IRremoteESP8266 library from v2.7.12 to v2.7.13
- Shelly Dimmer 1 and 2 stm32 firmware from v51.4 to v51.5
- mDNS has been disabled from all pre-compiled binaries to allow new features
- Sonoff L1 color up scaling and color margin detection [#9545](https://github.com/arendst/Tasmota/issues/9545)
- MQTT Wifi connection timeout from 5000 to 200 mSec [#9886](https://github.com/arendst/Tasmota/issues/9886)
- Force bigger Thunk Stack if 4K RSA even without EC ciphers [#10075](https://github.com/arendst/Tasmota/issues/10075)

### Fixed
- Command ``gpio`` using non-indexed functions regression from v9.1.0 [#9962](https://github.com/arendst/Tasmota/issues/9962)
- NTP fallback server functionality [#9739](https://github.com/arendst/Tasmota/issues/9739)
- Telegram group chatid not supported [#9831](https://github.com/arendst/Tasmota/issues/9831)
- KNX buttons, switches and sensors detection regression from v9.1.0 [#9811](https://github.com/arendst/Tasmota/issues/9811)
- KNX ESP32 UDP mulicastpackage [#9811](https://github.com/arendst/Tasmota/issues/9811)
- GUI MqttUser and MqttPassword updates when TLS is compiled in [#9825](https://github.com/arendst/Tasmota/issues/9825)
- ESP32 TasmotaClient firmware upgrade [#9218](https://github.com/arendst/Tasmota/issues/9218)
- Reset to defaults after 6 hours of DeepSleep [#9993](https://github.com/arendst/Tasmota/issues/9993)
- Backlog timing wraparound [#9995](https://github.com/arendst/Tasmota/issues/9995)
- First LED in addressable string does not fade when using scheme [#10088](https://github.com/arendst/Tasmota/issues/10088)
- Improved Opentherm error handling [#10055](https://github.com/arendst/Tasmota/issues/10055)
- Shutter motordelay stop issue [#10033](https://github.com/arendst/Tasmota/issues/10033)
- Shutter fix overflow on runtime over 100 seconds [#9800](https://github.com/arendst/Tasmota/issues/9800)
- ESP32 CC2530 heap corruption [#10121](https://github.com/arendst/Tasmota/issues/10121)
- ESP32 Analog input div10 rule trigger [#10149](https://github.com/arendst/Tasmota/issues/10149)

### Removed
- Version compatibility check
- PN532 define USE_PN532_CAUSE_EVENTS replaced by generic rule trigger `on pn532#uid=`

