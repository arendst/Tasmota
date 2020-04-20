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

This release will be supported from ESP8266/Arduino library Core version **2.7.0** due to reported security and stability issues on previous Core version. This will also support gzipped binaries.

Although it might still compile on previous Core versions all support will be removed in the near future.

## Support of TLS

To save resources when TLS is enabled mDNS needs to be disabled. In addition to TLS using fingerprints now also user supplied CA certs and AWS IoT is supported. Read [full documentation](https://tasmota.github.io/docs/AWS-IoT)

## Initial configuration tools

For initial configuration this release supports Webserver based **WifiManager** or **Serial** based command interface only. Support for **WPS** and **SmartConfig** has been removed.

## Provided Binary Downloads

The following binary downloads have been compiled with ESP8266/Arduino library core version **2.7.0**.

- **tasmota.bin** = The Tasmota version with most drivers. **RECOMMENDED RELEASE BINARY**
- **tasmota-BG.bin** to **tasmota-TW.bin** = The Tasmota version in different languages.
- **tasmota-lite.bin** = The Lite version without most drivers and sensors.
- **tasmota-knx.bin** = The Knx version without some features but adds KNX support.
- **tasmota-sensors.bin** = The Sensors version adds more useful sensors.
- **tasmota-ir** = The InfraRed Receiver and transmitter version allowing all available protocols provided by library IRremoteESP8266 but without most other features.
- **tasmota-display.bin** = The Display version without Energy Monitoring but adds display support.
- **tasmota-minimal.bin** = The Minimal version allows intermediate OTA uploads to support larger versions and does NOT change any persistent parameter. This version **should NOT be used for initial installation**.

[List](MODULES.md) of embedded modules.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog

### Version 8.3.0 Fred

- Change HM-10 sensor type detection and add features (#7962)
- Change light scheme 2,3,4 cycle time speed from 24,48,72,... seconds to 4,6,12,24,36,48,... seconds (#8034)
- Change remove floating point libs from IRAM
- Change remove MQTT Info messages on restart for DeepSleep Wake (#8044)
- Change IRremoteESP8266 library updated to v2.7.5
- Fix possible Relay toggle on (OTA) restart
- Fix PWM flickering during wifi connection (#8046)
- Fix Zigbee sending wrong Sat value with Hue emulation
- Fix Zigbee crash with Occupancy sensor (#8089)
- Add Zigbee command ``ZbRestore`` to restore device configuration dumped with ``ZbStatus 2``
- Add Zigbee command ``ZbUnbind``
- Add Zigbee command ``ZbBindState`` and ``manuf``attribute
- Add Zigbee command ``ZbConfig`` and configuration in Settings
- Add commands ``CounterDebounceLow`` and ``CounterDebounceHigh`` to control debouncing (#8021)
- Add commands ``NrfPage``, ``NrfIgnore``, ``NrfScan`` and ``NrfBeacon`` to NRF24 Bluetooth driver (#8075)
- Add commands ``GlobalTemp`` and ``GlobalHum`` to init sensor data (#8152)
- Add command ``SetOption41 <x>`` to force sending gratuitous ARP every <x> seconds
- Add command ``SetOption73 1`` for button decoupling and send multi-press and hold MQTT messages by Federico Leoni (#8235)
- Add command ``SetOption90 1`` to disable non-json MQTT messages (#8044)
- Add command ``SetOption91 1`` to enable fading at startup / power on
- Add command ``Sensor10 0/1/2`` to control BH1750 resolution - 0 = High (default), 1 = High2, 2 = Low (#8016)
- Add command ``Sensor10 31..254`` to control BH1750 measurement time which defaults to 69 (#8016)
- Add command ``Sensor18 0..32000`` to control PMS5003 sensor interval to extend lifetime by Gene Ruebsamen (#8128)
- Add command ``DevGroupName`` to specify up to four Device Group Names (#8087)
- Add command ``DevGroupSend`` to send an update to a Device Group (#8093)
- Add command ``Ping`` (#7176)
- Add command ``Palette`` to add the ability to specify a palette of colors (#8150)
- Add support for unreachable (unplugged) Zigbee devices in Philips Hue emulation and Alexa
- Add support for 64x48 SSD1306 OLED (#6740)
- Add support for Seven Segment display using HT16K33 (#8116)
- Add support for up to four MQTT GroupTopics (#8014)
- Add support for longer template names
- Add support for an iAQ sensor (#8107)
- Add support for AS3935 Lightning Sensor by device111 (#8130)
- Add console command history (#7483, #8015)
- Add quick wifi reconnect using saved AP parameters when ``SetOption56 0`` (#3189)
- Add more accuracy to GPS NTP server (#8088)
