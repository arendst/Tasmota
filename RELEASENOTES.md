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

Binaries for ESP8266 based devices can be downloaded from http://ota.tasmota.com/tasmota/release. Binaries for ESP32 based devices can be downloaded from http://ota.tasmota.com/tasmota32/release. The base links can be used for OTA upgrades like ``OtaUrl http://ota.tasmota.com/tasmota/release/tasmota.bin``

[List](MODULES.md) of embedded modules.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog

### Version 8.5.0 Hannah

- Remove support for direct upgrade from versions before 6.6.0.11 to versions after 8.4.0.1
- Change references from http://thehackbox.org/tasmota/ to http://ota.tasmota.com/tasmota/
- Change triple-mode TLS via configuration in a single firmware (TLS AWS IoT, Letsencrypt and No-TLS)
- Change White blend mode to using command ``SetOption 105`` instead of ``RGBWWTable``
- Fix ESP32 PWM range
- Fix display power control (#9114)
- Fix energy total counters (#9263, #9266)
- Add command ``SetOption102 0/1`` to set Baud rate for Teleinfo communication (0 = 1200 or 1 = 9600)
- Add command ``SetOption103 0/1`` to set TLS mode when TLS is selected
- Add command ``SetOption104 1`` to disable all MQTT retained messages
- Add command ``SetOption106 1`` to create a virtual White ColorTemp for RGBW lights
- Add command ``SetOption107 0/1`` to select virtual White as (0) Warm or (1) Cold
- Add command ``SetOption108 0/1`` to enable Teleinfo telemetry into Tasmota Energy MQTT (0) or Teleinfo only (1)
- Add command ``SetOption109 1`` to force gen1 Alexa mode, for Echo Dot 2nd gen devices only
- Add command ``Restart 2`` to halt system. Needs hardware reset or power cycle to restart (#9046)
- Add command ``PowerDelta1`` to ``PowerDelta3`` to trigger on up to three phases (#9134)
- Add Zigbee options to ``ZbSend`` ``Config`` and ``ReadCondig``
- Add Zigbee better support for IKEA Motion Sensor
- Add Zigbee web gui widget for Battery and Temp/Humidity/Pressure sensors
- Add Zigbee web ui for power metering plugs
- Add better configuration corruption recovery (#9046)
- Add virtual CT for 4 channels lights, emulating a 5th channel
- Add support for DYP ME007 ultrasonic distance sensor by Janusz Kostorz (#9113)
- Add ESP32 Analog input support for GPIO32 to GPIO39
- Add experimental support for ESP32 TTGO Watch and I2S Audio by Gerhard Mutz
