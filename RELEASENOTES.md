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

This release will be supported from ESP32/Arduino library Core version **2.0.3**.

Support of ESP8266 Core versions before 2.7.4.9 and ESP32 Core versions before 2.0.3 have been removed.

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
The following binary downloads have been compiled with ESP32/Arduino library core version **2.0.3**.

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

## Changelog v12.0.2 Paul
### Added
- Command ``SetOption139 0/1`` to switch between pressure unit "mmHg" (0) or "inHg" (1) when ``SO24 1`` [#15350](https://github.com/arendst/Tasmota/issues/15350)
- Command ``SetOption140 0/1`` to switch between MQTT Clean Session (0) or Persistent Session (1) [#15530](https://github.com/arendst/Tasmota/issues/15530)
- Command ``SetOption141 1`` to disable display of module name in GUI header
- Command ``SetOption142 1`` to wait 1 second for wifi connection solving some FRITZ!Box modem issues [#14985](https://github.com/arendst/Tasmota/issues/14985)
- Command ``DnsTimeout 100..20000`` to change default DNS timeout from 1000 msec blocking if no DNS server found
- Command ``EnergyExportActive<phase>`` to (p)reset energy export active for supported devices. Currently ADE7880 only [#13515](https://github.com/arendst/Tasmota/issues/13515)
- Command ``IfxRp ""|<policy>`` adds optional InfluxDb Retention Policy [#15513](https://github.com/arendst/Tasmota/issues/15513)
- Command ``SspmDisplay 2`` to display Sonoff SPM energy data in GUI for user tab-selected relay modules [#13447](https://github.com/arendst/Tasmota/issues/13447)
- Command ``SSerialSend9 0/1`` to enable Serial Bridge console Tee for debugging purposes
- Support for Sonoff MS01 soil moisture sensor [#15335](https://github.com/arendst/Tasmota/issues/15335)
- Support for daisy chaining MAX7219 displays [#15345](https://github.com/arendst/Tasmota/issues/15345)
- Support for Sensirion SHT4X using define USE_SHT3X [#15349](https://github.com/arendst/Tasmota/issues/15349)
- Support for Sonoff SPM v1.2.0
- Support for Sonoff Zigbee Bridge Pro by Stephan Hadinger [#15701](https://github.com/arendst/Tasmota/issues/15701)
- Support for Sonoff NSPanel Smart Scene Wall Switch
- Support for flowrate meters like YF-DN50 and similary [#15474](https://github.com/arendst/Tasmota/issues/15474)
- Support for 5-channel light dimmer driver BP5758D used in Tuya bulbs [#15713](https://github.com/arendst/Tasmota/issues/15713)
- Support for HYTxxx temperature and humidity sensor [#15715](https://github.com/arendst/Tasmota/issues/15715)
- Sonoff SPM delayed SetPowerOnState [#13447](https://github.com/arendst/Tasmota/issues/13447)
- ESP32 Command ``Restart 3`` to switch between SafeBoot and Production

### Breaking Changed

### Changed
- Restructured tasmota source directories taking benefit from PlatformIO Core v6.0.2
- Prepare to remove dedicated Home Assistant discovery in favour of Tasmota Discovery and hatasmota
- ESP32 Tasmota SafeBoot with changed partition scheme allowing larger binaries
- ESP32 increase Serial Bridge input buffer from 130 to 520 characters

### Fixed
- Improv initial or erase device installation failing to provide Configure WiFi option
- SCD40 start low power command [#15361](https://github.com/arendst/Tasmota/issues/15361)
- BL09xx negative power presentation [#15374](https://github.com/arendst/Tasmota/issues/15374)
- Possible pin output toggle after power on [#15630](https://github.com/arendst/Tasmota/issues/15630)
- SHT1X driver hangs and wrong values on ESP32 [#15790](https://github.com/arendst/Tasmota/issues/15790)
- Resolving NTP and/or MQTT server names regression from v12.0.0 [#15816](https://github.com/arendst/Tasmota/issues/15816)
- MQTT rc -4 connection regression from v12.0.0 [#15809](https://github.com/arendst/Tasmota/issues/15809)
- ESP32 Arduino Core WiFi timeout is changed from msec to seconds

### Removed
- Arduino IDE support
