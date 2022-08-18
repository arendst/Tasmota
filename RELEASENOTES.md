<img src="https://github.com/arendst/Tasmota/blob/master/tools/logo/TASMOTA_FullLogo_Vector.svg" alt="Logo" align="right" height="76"/>

# RELEASE NOTES

## Migration Information

**This version removes support for direct migration from versions before v8.1.0 (Doris)**

See [migration path](https://tasmota.github.io/docs/Upgrading#migration-path) for instructions how to migrate to a major version.

**Do not upgrade from minimal to minimal version. It will most likely fail at some point and will require flashing via serial.** If you do have to use minimal versions, always OTA to a full version of the same release before applying next minimal version.

Pay attention to the following version breaks due to dynamic settings updates:

1. Migrate to **Sonoff-Tasmota 3.9.x**
2. Migrate to **Sonoff-Tasmota 4.x**
3. Migrate to **Sonoff-Tasmota 5.14**
4. Migrate to **Sonoff-Tasmota 6.7.1** (http://ota.tasmota.com/tasmota/release_6.7.1/sonoff.bin) - NOTICE underscore as a dash is not supported in older versions
5. Migrate to **Tasmota 7.2.0** (http://ota.tasmota.com/tasmota/release-7.2.0/tasmota.bin)

--- Major change in parameter storage layout ---

6. Migrate to **Tasmota 8.5.1** (http://ota.tasmota.com/tasmota/release-8.5.1/tasmota.bin)

--- Major change in internal GPIO function representation ---

7. Migrate to **Tasmota 9.1** (http://ota.tasmota.com/tasmota/release-9.1.0/tasmota.bin.gz)
8. Upgrade to **latest release** (http://ota.tasmota.com/tasmota/release/tasmota.bin.gz)

While fallback or downgrading is common practice it was never supported due to Settings additions or changes in newer releases. Starting with release **v9.1.0 Imogen** the internal GPIO function representation has changed in such a way that fallback is only possible to the latest GPIO configuration before installing **v9.1.0**.

## Supported Core versions

This release will be supported from ESP8266/Arduino library Core version **2.7.4.9** due to reported security and stability issues on previous Core version. This will also support gzipped binaries.

This release will be supported from ESP32/Arduino library Core version **2.0.4.1**.

Support of ESP8266 Core versions before 2.7.4.9 and ESP32 Core versions before 2.0.4.1 have been removed.

## Support of TLS

In addition to TLS using fingerprints now also user supplied CA certs, AWS IoT and Azure IoT is supported. Read [full documentation](https://tasmota.github.io/docs/AWS-IoT)

## Initial configuration tools

For initial configuration this release supports Webserver based **WifiManager** or **Serial** based command interface.

## Initial installation

Easy initial installation of Tasmota can be performed using the [Tasmota WebInstaller](https://tasmota.github.io/install/).

## Provided Binary Downloads

### ESP8266 or ESP8285 based
The following binary downloads have been compiled with ESP8266/Arduino library core version **2.7.4.9**.

- **tasmota.bin** = The Tasmota version with most drivers for 1M+ flash. **RECOMMENDED RELEASE BINARY**
- **tasmota4M.bin** = The Tasmota version with most drivers and filesystem for 4M+ flash.
- **tasmota-AF.bin** to **tasmota-VN.bin** = The Tasmota version in different languages for 1M+ flash.
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
- http://ota.tasmota.com/tasmota/release-12.0.2

The latter links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota/release/tasmota.bin.gz``

### ESP32, ESP32-C3, ESP32-S2 and ESP32-S3 based
The following binary downloads have been compiled with ESP32/Arduino library core version **2.0.4.1**.

- **tasmota32.bin** = The Tasmota version with most drivers including additional sensors and KNX for 4M+ flash.  **RECOMMENDED RELEASE BINARY**
- **tasmota32xy.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-C3/S2/S3 and 4M+ flash.
- **tasmota32xycdc.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-C3/S2/S3 with serial over embedded USB CDC only and 4M+ flash.
- **tasmota32solo1.bin** = The Tasmota version with most drivers including additional sensors and KNX for single core ESP32 and 4M+ flash.
- **tasmota32-AF.bin** to **tasmota32-VN.bin** = The Tasmota version in different languages for 4M+ flash.
- **tasmota32-bluetooth.bin** = The Bluetooth version adds BLE support for 4M+ flash.
- **tasmota32-display.bin** = The Display version without Energy Monitoring but adds display support for 4M+ flash.
- **tasmota32-ir.bin** = The InfraRed Receiver and transmitter version allowing all available protocols provided by library IRremoteESP8266 but without most other features for 4M+ flash.
- **tasmota32-lvgl.bin** = The LVGL version adds Light and Versatile Graphics Library (LVGL) display support for 4M+ flash.
- **tasmota32-nspanel.bin** = The Sonoff NSPanel Smart Scene Wall Switch version with HASPmota display support.
- **tasmota32-webcam.bin** = The Webcam version adds webcam support for 4M+ flash.
- **tasmota32-zbbridgepro.bin** - The Sonoff Zigbee Bridge Pro version with CC2652P firmware load support.

Latest released binaries can be downloaded from
- https://github.com/arendst/Tasmota-firmware/tree/main/release-firmware
- http://ota.tasmota.com/tasmota32/release

Historical binaries can be downloaded from
- http://ota.tasmota.com/tasmota32/release-12.0.2

The latter links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota32/release/tasmota32.bin``

## Additional information

[List](MODULES.md) of embedded modules.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog v12.1.0 Patricia
### Added
- Command ``SetOption45 1..250`` to change default bistable latching relay pulse length of 40 milliseconds
- Command ``SetOption144 1`` includes a timestamp in zigbee `ZbReceived` messages
- Command ``GlobalTemp2 1..250`` to select Global Temperature source indexed from teleperiod occurance data [#15834](https://github.com/arendst/Tasmota/issues/15834)
- Command ``GlobalHum2 1..250`` to select Global Humidity source indexed from teleperiod occurance data [#15834](https://github.com/arendst/Tasmota/issues/15834)
- Command ``GlobalPress2 1..250`` to select Global Pressure source indexed from teleperiod occurance data [#15834](https://github.com/arendst/Tasmota/issues/15834)
- Command ``Sleep2 !`` to cancel pending one-shot speed setting [#15954](https://github.com/arendst/Tasmota/issues/15954)
- Commands ``WifiScan`` and ``WifiTest`` [#16141](https://github.com/arendst/Tasmota/issues/16141)
- Support for 5-channel light dimmer driver SM2335 used in SwitchBot Color Bulbs [#15839](https://github.com/arendst/Tasmota/issues/15839)
- Support for Sonoff POWR3xxD and THR3xxD [#15856](https://github.com/arendst/Tasmota/issues/15856)
- Support for bistable (latching) relays mixed with monostable relays using GPIO Relay_b or Relay_bi as used by Sonoff POWR320D and THR320D
- Support for Modbus bridge adding commands ``ModbusSend``, ``ModbusBaudrate`` and ``ModbusSerialConfig`` [#16013](https://github.com/arendst/Tasmota/issues/16013)
- Support for multiple ``IRsend`` GPIOs [#16138](https://github.com/arendst/Tasmota/issues/16138)
- Support for Catalan language translations by Albert Gonzalez [#16145](https://github.com/arendst/Tasmota/issues/16145)
- Zigbee added recording of when the battery was last reported [#16146](https://github.com/arendst/Tasmota/issues/16146)
- Zigbee add Battery auto-probe (can be disabled with ``SetOption143 1``) [#16148](https://github.com/arendst/Tasmota/issues/16148)
- ESP32 Support for Ultra Low Power (ULP) coprocessor via Berry by Christian Staars [#15916](https://github.com/arendst/Tasmota/issues/15916)

### Breaking Changed

### Changed
- TasmotaModbus library from v3.4.0 to v3.5.0 [#16245](https://github.com/arendst/Tasmota/issues/16245)
- ESP32 Arduino core from v2.0.3 to v2.0.4.1 [#15940](https://github.com/arendst/Tasmota/issues/15940)
- ESP32 LVGL library from v8.2.0 to v8.3.0 [#16019](https://github.com/arendst/Tasmota/issues/16019)
- Driver DHT v6 consolidation for both ESP8266 and ESP32 to support SI7021, THS01 and MS01 on ESP32 [#15856](https://github.com/arendst/Tasmota/issues/15856)

### Fixed
- Restore EnergyToday after using command ``restart 2`` and power cycle [#16118](https://github.com/arendst/Tasmota/issues/16118)
- ESP32 SendMail not working over ethernet [#15794](https://github.com/arendst/Tasmota/issues/15794)

### Removed
- Arduino IDE support
