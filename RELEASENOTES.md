<img src="https://github.com/arendst/Tasmota/blob/master/tools/logo/TASMOTA_FullLogo_Vector.svg" alt="Logo" align="right" height="76"/>

# RELEASE NOTES

## Migration Information

See [migration path](https://tasmota.github.io/docs/#/Upgrading?id=migration-path) for instructions how to migrate to a major version. Pay attention to the following version breaks due to dynamic settings updates:

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

This release will be supported from ESP8266/Arduino library Core version **2.6.3 + 372a3ec** due to reported security and stability issues on previous Core version. This will also support gzipped binaries.

Although it might still compile on previous Core versions all support will be removed in the near future.

## Support of TLS

To save resources when TLS is enabled mDNS needs to be disabled. In addition to TLS using fingerprints now also user supplied CA certs and AWS IoT is supported. Read [full documentation](https://tasmota.github.io/docs/#/integrations/AWS-IoT)

## Initial configuration tools

For initial configuration this release supports Webserver based **WifiManager** or **Serial** based command interface only. Support for **WPS** and **SmartConfig** has been removed.

## Provided Binary Downloads

The following binary downloads have been compiled with ESP8266/Arduino library core version **2.6.3 + 372a3ec**.

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

### Version 8.1.0.11

- Change default my_user_config.h driver and sensor support removing most sensors and adding most drivers
- Change DHT driver (#7468, #7717)
- Change Lights: simplified gamma correction and 10 bits internal computation
- Change commands ``Prefix``, ``Ssid``, ``StateText``, ``NTPServer``, and ``FriendlyName`` displaying all items
- Change IRremoteESP8266 library updated to v2.7.4
- Change Zigbee command prefix from ``Zigbee*`` to ``Zb*``
- Change MQTT message size with additional 200 characters
- Change display of some date and time messages from "Wed Feb 19 10:45:12 2020" to "2020-02-19T10:45:12"
- Fix Sonoff Bridge, Sc, L1, iFan03 and CSE7766 serial interface to forced speed, config and disable logging
- Fix commands ``Display`` and ``Counter`` from overruling command processing (#7322)
- Fix ``White`` added to light status (#7142)
- Fix Improved fade linearity with gamma correction
- Fix LCD line and column positioning (#7387)
- Fix Display handling of hexadecimal escape characters (#7387)
- Fix ``WakeUp <x>`` ignores provided value (#7473)
- Fix exception 9 restart on log message in Ticker interrupt service routines NTP, Wemos and Hue emulation (#7496)
- Fix ``PowerDelta`` zero power detection (#7515)
- Fix ``RGBWWTable`` ignored (#7572)
- Fix PWM flickering at low levels (#7415)
- Fix Hass sensor discovery part 1/4 by Federico Leoni (#7582, #7548)
- Fix MaxPower functionality (#7647)
- Fix relation between RSSI and signal strength
- Add command ``SetOption79 0/1`` to enable reset of counters at teleperiod time by Andre Thomas (#7355)
- Add command ``SetOption82 0/1`` to limit the CT range for Alexa to 200..380
- Add command ``SetOption84 0/1`` to send AWS IoT device shadow updates (alternative to retained)
- Add commands ``SetOption85 0/1`` and ``DevGroupShare`` supporting UDP Group command using ``GroupTopic`` without MQTT by Paul Diem (#7790)
- Add command ``SetOption86 0/1`` for PWM dimmer to turn brightness LED's off 5 seconds after last change
- Add command ``SetOption87 0/1`` for PWM dimmer to turn red LED on when powered off
- Add command ``SetOption88 0/1`` for PWM dimmer to let buttons control remote devices
- Add command ``SetOption89 0/1`` for Zigbee distinct MQTT topics per device for SENSOR, allowing retained messages (#7835)
- Add command ``ShutterButton <parameters>`` to control shutter(s) by to-scho (#7403)
- Add commands ``SwitchMode 8`` ToggleMulti, ``SwitchMode 9`` FollowMulti and ``SwitchMode 10`` FollowMultiInverted (#7522)
- Add commands ``SwitchMode 11`` PushHoldMulti and ``SwitchMode 12`` PushHoldInverted (#7603)
- Add command ``Buzzer -1`` for infinite mode and command ``Buzzer -2`` for following led mode (#7623)
- Add SerialConfig to ``Status 1``
- Add WifiPower to ``Status 5``
- Add support for DS1624, DS1621 Temperature sensor by Leonid Myravjev
- Add Zigbee attribute decoder for Xiaomi Aqara Cube
- Add support for ``AdcParam`` parameters to control ADC0 Current Transformer Apparent Power formula by Jodi Dillon (#7100)
- Add optional support for Prometheus using file xsns_91_prometheus.ino (#7216)
- Add experimental support for NRF24L01 as BLE-bridge for Mijia Bluetooth sensors by Christian Baars (#7394)
- Add support to BMP driver to enter reset state (sleep enable) when deep sleep is used in Tasmota
- Add support for gzipped binaries
- Add web page sliders when ``SetOption37 128`` is active allowing control of white(s)
- Add most SetOptions as defines to my_user_config.h
- Add SoftwareSerial to CSE7766 driver allowing different GPIOs (#7563)
- Add optional parameter <startcolor> to command ``Scheme <scheme>, <startcolor>`` to control initial start color
- Add rule trigger on one level deeper using syntax with two ``#`` like ``on zigbeereceived#vibration_sensor#aqaracubeside=0 do ...``
- Add support for sensors DS18x20 and DHT family on Shelly 1 and Shelly 1PM using Shelly Add-On adapter (#7469)
- Add support for MI-BLE sensors using HM-10 Bluetooth 4.0 module by Christian Staars (#7683)
- Add BootCount Reset Time as BCResetTime to ``Status 1``
- Add ``ZbZNPReceived``and ``ZbZCLReceived`` being published to MQTT when ``SetOption66 1``
- Add optional Wifi AccessPoint passphrase define WIFI_AP_PASSPHRASE in my_user_config.h (#7690)
- Add support for FiF LE-01MR energy meter by saper-2 (#7584)
- Add initial support for Sensors AHT10 and AHT15 by Martin Wagner (#7596)
- Add support for Wemos Motor Shield V1 by Denis Sborets (#7764)
- Add Zigbee enhanced commands decoding, added ``ZbPing``
- Add Zigbee features and improvements
- Add support for Martin Jerry/acenx/Tessan/NTONPOWER SD0x PWM dimmer switches by Paul Diem (#7791)
- Add support for Jarolift rollers by Keeloq algorithm
- Add support for MaxBotix HRXL-MaxSonar ultrasonic range finders by Jon Little (#7814)
- Add support for Romanian language translations by Augustin Marti
- Add HAss Discovery support for Button and Switch triggers by Federico Leoni (#7901)
- Add support for HDC1080 Temperature and Humidity sensor by Luis Teixeira (#7888)
