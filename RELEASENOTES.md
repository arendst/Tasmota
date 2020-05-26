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

This release will be supported from ESP8266/Arduino library Core version **2.7.1** due to reported security and stability issues on previous Core version. This will also support gzipped binaries.

Although it might still compile on previous Core versions all support will be removed in the near future.

## Support of TLS

To save resources when TLS is enabled mDNS needs to be disabled. In addition to TLS using fingerprints now also user supplied CA certs and AWS IoT is supported. Read [full documentation](https://tasmota.github.io/docs/AWS-IoT)

## Initial configuration tools

For initial configuration this release supports Webserver based **WifiManager** or **Serial** based command interface only. Support for **WPS** and **SmartConfig** has been removed.

## Provided Binary Downloads

The following binary downloads have been compiled with ESP8266/Arduino library core version **2.7.1**.

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

### Version 8.3.1.2

- Change IRremoteESP8266 library updated to v2.7.7
- Change Adafruit_SGP30 library from v1.0.3 to v1.2.0 (#8519)
- Change Energy JSON Total field from ``"Total":[33.736,11.717,16.978]`` to ``"Total":33.736,"TotalTariff":[11.717,16.978]``
- Change Energy JSON ExportActive field from ``"ExportActive":[33.736,11.717,16.978]`` to ``"ExportActive":33.736,"ExportTariff":[11.717,16.978]``
- Fix escape of non-JSON received serial data (#8329)
- Add command ``Rule0`` to change global rule parameters
- Add command ``Time 4`` to display timestamp using milliseconds (#8537)
- Add commands ``LedPwmOn 0..255``, ``LedPwmOff 0..255`` and ``LedPwmMode1 0/1`` to control led brightness by George (#8491)
- Add support for unique MQTTClient (and inherited fallback topic) by full Mac address using ``mqttclient DVES_%12X`` (#8300)
- Add more functionality to ``Switchmode`` 11 and 12 (#8450)
- Add wildcard pattern ``?`` for JSON matching in rules
- Add support for VEML6075 UVA/UVB/UVINDEX Sensor by device111 (#8432)
- Add support for VEML7700 Ambient light intensity Sensor by device111 (#8432)
- Add Three Phase Export Active Energy to SDM630 driver
