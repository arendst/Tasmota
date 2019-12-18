<img src="https://github.com/arendst/Tasmota/blob/master/tools/logo/TASMOTA_FullLogo_Vector.svg" alt="Logo" align="right" height="76"/>

# RELEASE NOTES

### Sonoff-Tasmota is now Tasmota

## Migration Information

See [migration path](https://tasmota.github.io/docs/#/Upgrading?id=migration-path) for instructions how to migrate to a major version. Pay attention to the following version breaks due to dynamic settings updates:

1. Migrate to **Sonoff-Tasmota 3.9.x**
2. Migrate to **Sonoff-Tasmota 4.x**
3. Migrate to **Sonoff-Tasmota 5.14**
4. Migrate to **Sonoff-Tasmota 6.x**
5. Migrate to **Tasmota 7.x**

## Supported Core versions

This release will be supported from ESP8266/Arduino library Core version **2.6.1** due to reported security and stability issues on previous Core version.

Although it might still compile on previous Core versions all support will be removed in the near future.

## Support of TLS

To save resources when TLS is enabled mDNS needs to be disabled. In addition to TLS using fingerprints now also user supplied CA certs and AWS IoT is supported. Read [full documentation](https://tasmota.github.io/docs/#/integrations/AWS-IoT)

## Initial configuration tools

For initial configuration this release supports Webserver based **WifiManager** or **Serial** based command interface only. Support for **WPS** and **SmartConfig** has been removed.

## Provided Binary Downloads

The following binary downloads have been compiled with ESP8266/Arduino library core version **2.6.1**.

- **tasmota.bin** = The Tasmota version with sensors. **RECOMMENDED RELEASE BINARY**
- **tasmota-BG.bin** to **tasmota-TW.bin** = The Tasmota version in different languages.
- **tasmota-lite.bin** = The Lite version without most sensors.
- **tasmota-knx.bin** = The Knx version without some features but adds KNX support.
- **tasmota-sensors.bin** = The Sensors version adds more useful sensors.
- **tasmota-ir** = The InfraRed Receiver and transmitter version allowing all available protocols provided by library IRremoteESP8266 but without most other features.
- **tasmota-display.bin** = The Display version without Energy Monitoring but adds display support.
- **tasmota-minimal.bin** = The Minimal version allows intermediate OTA uploads to support larger versions and does NOT change any persistent parameter. This version **should NOT be used for initial installation**.

[List](MODULES.md) of embedded modules.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog

### Version 7.2.0 Constance

- Change Exception reporting removing exception details from ``Status 1`` and consolidated in ``Status 12`` if available
- Change HTTP CORS from command ``SetOption73 0/1`` to ``Cors <cors_domain>`` allowing user control of specific CORS domain by Shantur Rathore (#7066)
- Change GUI Shutter button text to Up and Down Arrows based on PR by Xavier Muller (#7166)
- Change amount of supported DHT sensors from 3 to 4 by Xavier Muller (#7167)
- Change some Settings locations freeing up space for future single char allowing variable length text
- Change tasmota-basic.bin and FIRMWARE_BASIC to tasmota-lite.bin and FIRMWARE_LITE
- Fix flashing H801 led at boot by Stefan Hadinger (#7165, #649)
- Fix duplicated ``Backlog`` when using Event inside a Backlog by Adrian Scillato (#7178, #7147)
- Fix Gui Timer when using a negative zero offset of -00:00 by Peter Ooms (#7174)
- Add command ``SerialConfig 0..23`` or ``SerialConfig 8N1`` to select Serial Config based in PR by Luis Teixeira (#7108)
- Add command ``Sensor34 9 <weight code>`` to set minimum delta to trigger JSON message by @tobox (#7188)
- Add rule var ``%topic%`` by Adrian Scillato (#5522)
- Add rule triggers ``tele-wifi1#xxx`` by Adrian Scillato (#7093)
- Add SML bus decoder syntax support for byte order by Gerhard Mutz (#7112)
- Add experimental support for stepper motor shutter control by Stefan Bode
- Add optional USE_MQTT_TLS to tasmota-minimal.bin by Bohdan Kmit (#7115)
- Add save call stack in RTC memory in case of crash, command ``Status 12`` to dump the stack by Stefan Hadinger
- Add Home Assistant force update by Frederico Leoni (#7140, #7074)
- Add Wifi Signal Strength in dBm in addition to RSSI Wifi Experience by Andreas Schultz (#7145)
- Add Yaw, Pitch and Roll support for MPU6050 by Philip Barclay (#7058)
- Add reporting of raw weight to JSON from HX711 to overcome auto-tare functionality by @tobox (#7171)
- Add Zigbee support for Xiaomi Aqara Vibration Sensor and Presence Sensor by Stefan Hadinger
- Add Shutter functions ramp up/down and MQTT reporting by Stefan Bode
- Add fallback support from version 8.x
