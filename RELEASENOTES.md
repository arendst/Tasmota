<img src="https://github.com/arendst/Tasmota/blob/master/tools/logo/TASMOTA_FullLogo_Vector.svg" alt="Logo" align="right" height="76"/>

# RELEASE NOTES

## Migration Information

See [migration path](https://tasmota.github.io/docs/Upgrading#migration-path) for instructions how to migrate to a major version. Pay attention to the following version breaks due to dynamic settings updates:

1. Migrate to **Sonoff-Tasmota 3.9.x**
2. Migrate to **Sonoff-Tasmota 4.x**
3. Migrate to **Sonoff-Tasmota 5.14**
4. Migrate to **Sonoff-Tasmota 6.x**
5. Migrate to **Tasmota 7.x**

--- Major change in parameter storage layout ---

6. Migrate to **Tasmota 8.1**
7. Migrate to **Tasmota 8.x**

While fallback or downgrading is common practice it was never supported due to Settings additions or changes in newer releases. Starting with release **v8.1.0 Doris** the Settings are re-allocated in such a way that fallback is only allowed and possible to release **v7.2.0 Constance**. Once at v7.2.0 you're on your own when downgrading even further.

## Supported Core versions

This release will be supported from ESP8266/Arduino library Core version **2.7.4.1** due to reported security and stability issues on previous Core version. This will also support gzipped binaries.

Support of Core versions before 2.7.1 has been removed.

## Support of TLS

To save resources when TLS is enabled mDNS needs to be disabled. In addition to TLS using fingerprints now also user supplied CA certs and AWS IoT is supported. Read [full documentation](https://tasmota.github.io/docs/AWS-IoT)

## Initial configuration tools

For initial configuration this release supports Webserver based **WifiManager** or **Serial** based command interface only. Support for **WPS** and **SmartConfig** has been removed.

## Provided Binary Downloads

The following binary downloads have been compiled with ESP8266/Arduino library core version **2.7.4.1**.

- **tasmota.bin** = The Tasmota version with most drivers. **RECOMMENDED RELEASE BINARY**
- **tasmota-BG.bin** to **tasmota-TW.bin** = The Tasmota version in different languages.
- **tasmota-lite.bin** = The Lite version without most drivers and sensors.
- **tasmota-knx.bin** = The Knx version without some features but adds KNX support.
- **tasmota-sensors.bin** = The Sensors version adds more useful sensors.
- **tasmota-ir** = The InfraRed Receiver and transmitter version allowing all available protocols provided by library IRremoteESP8266 but without most other features.
- **tasmota-display.bin** = The Display version without Energy Monitoring but adds display support.
- **tasmota-zbbridge.bin** = The dedicated Sonoff Zigbee Bridge version.
- **tasmota-minimal.bin** = The Minimal version allows intermediate OTA uploads to support larger versions and does NOT change any persistent parameter. This version **should NOT be used for initial installation**.

The attached binaries can also be downloaded from http://ota.tasmota.com/tasmota/release for ESP8266 or http://ota.tasmota.com/tasmota32/release for ESP32. The links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota/release/tasmota.bin``

[List](MODULES.md) of embedded modules.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog

### Version 8.5.1 Hannah

- Fix energy total counters (#9263, #9266)
- Fix crash in ``ZbRestore``
- Fix reset BMP sensors when executing command ``SaveData`` and define USE_DEEPSLEEP enabled (#9300)
- Fix ``status 0`` message when using define USE_MQTT_TLS due to small log buffer (#9305)
- Fix ``status 13`` exception 9 when more than one shutter is configured
- Fix ``status 13`` json message
- Fix Shelly 2.5 higher temperature regression from 8.2.0.1 (#7991)
- Change replace ArduinoJson with JSMN for JSON parsing
- Change ``WakeUp`` uses 256 steps instead of 100 (#9241)
- Add command ``SetOption110 1`` to disable Zigbee auto-config when pairing new devices
- Add command ``SetOption111 1`` to enable frequency output for buzzer GPIO (#8994)
- Add command ``SetOption112 1`` to enable friendly name in zigbee topic (use with SetOption89)
- Add ``#define USE_MQTT_AWS_IOT_LIGHT`` for password based AWS IoT authentication
- Add ``#define MQTT_LWT_OFFLINE`` and ``#define MQTT_LWT_ONLINE`` to user_config.h (#9395)
- Add new shutter modes (#9244)
- Add Zigbee auto-config when pairing
- Add support for MLX90640 IR array temperature sensor by Christian Baars
- Add support for VL53L1X time of flight sensor by Johann Obermeier
