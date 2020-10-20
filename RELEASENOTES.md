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

--- Major change in internal GPIO function representation ---

8. Migrate to **Tasmota 9.x**

While fallback or downgrading is common practice it was never supported due to Settings additions or changes in newer releases. Starting with release **v9.1.0 Imogen** the internal GPIO function representation has changed in such a way that fallback is only possible to the latest GPIO configuration before installing **v9.1.0**.

## Supported Core versions

This release will be supported from ESP8266/Arduino library Core version **2.7.4.3** due to reported security and stability issues on previous Core version. This will also support gzipped binaries.

Support of Core versions before 2.7.1 has been removed.

## Support of TLS

To save resources when TLS is enabled mDNS needs to be disabled. In addition to TLS using fingerprints now also user supplied CA certs and AWS IoT is supported. Read [full documentation](https://tasmota.github.io/docs/AWS-IoT)

## Initial configuration tools

For initial configuration this release supports Webserver based **WifiManager** or **Serial** based command interface only. Support for **WPS** and **SmartConfig** has been removed.

## Provided Binary Downloads

The following binary downloads have been compiled with ESP8266/Arduino library core version **2.7.4.3**.

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

## Changelog v9.0.0.2
### Added
- Command ``NoDelay`` for immediate backlog command execution by Erik Montnemery (#9544)
- Command ``SwitchMode 15`` sending only MQTT message on switch change (#9596)
- Zigbee command ``ZbData`` for better support of device specific data
- Optional support for Mitsubishi Electric HVAC by David Gwynne (#9237)
- Optional support for Orno WE517-Modbus energy meter by Maxime Vincent (#9353)
- SDM630 three phase ImportActive Energy display when ``#define SDM630_IMPORT`` is enabled by Janusz Kostorz (#9124)
- Optional support for inverted NeoPixelBus data line by enabling ``#define USE_WS2812_INVERTED`` (#8988)
- Support for PWM dimmer color/trigger on tap, SO88 led, DGR WITH_LOCAL flag, multi-press and ledmask by Paul Diem (#9474, #9584)
- Support for stateful ACs using ``StateMode`` in tasmota-ir.bin by Arik Yavilevich (#9472)
- Support for analog buttons indexed within standard button range
- Support for Vietnamese language translations by Tâm.NT
- Support for timers in case of no-sunset permanent day by cybermaus (#9543)
- Support for EZO Ph, ORP and RTD sensors by Christopher Tremblay (#9567, #9585)

### Breaking Changed
- Redesigned ESP8266 GPIO internal representation in line with ESP32 changing ``Template`` layout too
- ``#define MQTT_FINGERPRINT`` from string to hexnumbers (#9570)
- Command ``Status`` output for disabled status types now returns {"Command":"Error"}
- MAX31865 driver to support up to 6 thermocouples selected by ``MX31865 CS`` instead of ``SSPI CS`` (#9103)

### Changed
- Command ``Gpio17`` replaces command ``Adc``
- Command ``Gpios`` replaces command ``Adcs``
- New IR Raw compact format (#9444)
- A4988 optional microstep pin selection
- Pulsetime to allow use for all relays with 8 interleaved so ``Pulsetime1`` is valid for Relay1, Relay9, Relay17 etc. (#9279)
- IRremoteESP8266 library from v2.7.10 to v2.7.11
- NeoPixelBus library from v2.5.0.09 to v2.6.0
- Management of serial baudrate (#9554)
- Rotary driver adjusted accordingly if Mi Desk Lamp module is selected (#9399)

### Fixed
- Ledlink blink when no network connected regression from v8.3.1.4 (#9292)
- Exception 28 due to device group buffer overflow (#9459)
- Shutter timing problem due to buffer overflow in calibration matrix (#9458)
- Light wakeup exception 0 (divide by zero) when ``WakeupDuration`` is not initialised (#9466)
- Thermostat sensor status corruption regression from v8.5.0.1 (#9449)
- Telegram message decoding error regression from v8.5.0.1
- Rule handling of Var or Mem using text regression from v8.5.0.1 (#9540)
- Correct Energy period display shortly after midnight by gominoa (#9536)
- TuyaMcu energy display regression from v8.5.0.1 (#9547)

### Removed
- Support for direct upgrade from Tasmota versions before v7.0
- Auto config update for all Friendlynames and Switchtopic from Tasmota versions before v8.0
- Support for downgrade to versions before 9.0 keeping current GPIO configuration
