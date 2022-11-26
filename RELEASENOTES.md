<img src="https://github.com/arendst/Tasmota/blob/master/tools/logo/TASMOTA_FullLogo_Vector.svg" alt="Logo" align="right" height="76"/>

# RELEASE NOTES

## Migration Information

**This version removes support for direct migration from versions before v8.1.0 (Doris)**

See [migration path](https://tasmota.github.io/docs/Upgrading#migration-path) for instructions how to migrate to a major version.

**Do not upgrade from minimal to minimal version. It will most likely fail at some point and will require flashing via serial.** If you do have to use minimal versions, always OTA to a full version of the same release before applying next minimal version.

Pay attention to the following version breaks due to dynamic settings updates:

1. Migrate to **Sonoff-Tasmota 3.9.x**
2. Migrate to **Sonoff-Tasmota 4.x**
3. Migrate to **Sonoff-Tasmota 5.14** (http://ota.tasmota.com/tasmota/release_5.14.0/sonoff.bin) - NOTICE underscore as a dash is not supported in older versions
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

This release will be supported from ESP32/Arduino library Core version **2.0.5.3**.

Support of ESP8266 Core versions before 2.7.4.9 and ESP32 Core versions before 2.0.5.3 have been removed.

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
- **tasmota-AD.bin** to **tasmota-VN.bin** = The Tasmota version in different languages for 1M+ flash.
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
- http://ota.tasmota.com/tasmota/release-12.2.0

The latter links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota/release/tasmota.bin.gz``

### ESP32, ESP32-C3, ESP32-S2 and ESP32-S3 based
The following binary downloads have been compiled with ESP32/Arduino library core version **2.0.5.3**.

- **tasmota32.bin** = The Tasmota version with most drivers including additional sensors and KNX for 4M+ flash.  **RECOMMENDED RELEASE BINARY**
- **tasmota32xy.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-C3/S2/S3 and 4M+ flash.
- **tasmota32xycdc.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-C3/S2/S3 with serial over embedded USB CDC only and 4M+ flash.
- **tasmota32solo1.bin** = The Tasmota version with most drivers including additional sensors and KNX for single core ESP32 and 4M+ flash.
- **tasmota32-AD.bin** to **tasmota32-VN.bin** = The Tasmota version in different languages for 4M+ flash.
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
- http://ota.tasmota.com/tasmota32/release-12.2.0

The latter links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota32/release/tasmota32.bin``

## Additional information

[List](MODULES.md) of embedded modules.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog v12.2.0.5
### Added
- Command ``SetOption35 0..255`` to skip number of received messages in Serial Bridge (default 0) [#17140](https://github.com/arendst/Tasmota/issues/17140)
- Command ``SetOption47 1..255`` to delay power on relay state in seconds reducing power surge. ``SO47 1`` delays until network connected. ``SO47 2`` delays until mqtt connected
- Command ``RgxClients`` for range extender clients list [#17048](https://github.com/arendst/Tasmota/issues/17048)
- Command ``RgxPort [tcp|udp], gateway_port, client_mac, client_port`` for range extender port forwardings [#17092](https://github.com/arendst/Tasmota/issues/17092)
- Command ``SSerialBuffer 256..SERIAL_BRIDGE_BUFFER_SIZE`` to change serial bridge rx buffer size [#17120](https://github.com/arendst/Tasmota/issues/17120)
- Command ``SwitchMode 16`` sending only MQTT message on inverted switch change [#17028](https://github.com/arendst/Tasmota/issues/17028)
- Command NeoPool ``NPFiltration 2`` toggle [#16859](https://github.com/arendst/Tasmota/issues/16859)
- Optional define ``SERIAL_BRIDGE_BUFFER_SIZE`` to set Serial Bridge internal buffer size (Default ESP8266 = 256, ESP32 = 800)
- Support for two phase power calibration using commands ``PowerSet2``, ``VoltageSet2`` and ``CurrentSet2``
- Support for HLK-LD2410 24GHz smart wave motion sensor
- Support for Shelly Pro 1/1PM and 2/2PM [#16773](https://github.com/arendst/Tasmota/issues/16773)
- Support for up to four DS18x20 GPIOs by md5sum-as [#16833](https://github.com/arendst/Tasmota/issues/16833)
- Support for Digital Addressable Lighting Interface (DALI) by Andrei Kazmirtsuk [#16938](https://github.com/arendst/Tasmota/issues/16938)
- Support for NTAG2xx tags read and write on PN532 NFC reader [#16939](https://github.com/arendst/Tasmota/issues/16939)
- Support for Plantower PMSx003T AQI models with temperature and humidity [#16971](https://github.com/arendst/Tasmota/issues/16971)
- Support for BP1658CJ RGBCW led bulbs like Orein OS0100411267 by Cossid [#17011](https://github.com/arendst/Tasmota/issues/17011)
- Support for Dingtian x595 shift register based relay boards by Barbudor [#17032](https://github.com/arendst/Tasmota/issues/17032)
- Support for HMC5883L 3-Axis Digital Compass sensor by Andreas Achtzehn [#17069](https://github.com/arendst/Tasmota/issues/17069)
- WS2812 and Light ArtNet DMX control over UDP port 6454 [#17059](https://github.com/arendst/Tasmota/issues/17059)
- Teleinfo TEMPO (BBR) contract [#17160](https://github.com/arendst/Tasmota/issues/17160)
- Berry ``bytes().setbytes()`` method [#16892](https://github.com/arendst/Tasmota/issues/16892)
- Berry ``bytes().reverse()`` method [#16977](https://github.com/arendst/Tasmota/issues/16977)
- Zigbee router firmware for Sonoff ZBBridgePro [#16900](https://github.com/arendst/Tasmota/issues/16900)
- ESP32 Support for DMX ArtNet Led matrix animations [#16984](https://github.com/arendst/Tasmota/issues/16984)
- ESP32 DS18x20 parasitic power usage when defining W1_PARASITE_POWER [#17112](https://github.com/arendst/Tasmota/issues/17112)

### Breaking Changed
- Redesign distance sensors VL53LXX, TOF10120, HRXL and DYP to use cm instead of mm [#17021](https://github.com/arendst/Tasmota/issues/17021)

### Changed
- ESP32 Framework (Core) from v2.0.5 to v2.0.5.3
- ESP32 LVGL library from v8.3.2 to v8.3.3 (no functional change)
- ESP32 NimBLE library from v1.4.0 to v1.4.1 [#16775](https://github.com/arendst/Tasmota/issues/16775)
- Serial Bridge default internal serial rx buffer size from 64 to 256 [#17120](https://github.com/arendst/Tasmota/issues/17120)
- DS18x20 ``DS18Alias`` to ``DS18Sens`` [#16833](https://github.com/arendst/Tasmota/issues/16833)
- Compiling with reduced boards manifests in favour of Autoconfig [#16848](https://github.com/arendst/Tasmota/issues/16848)
- ADE7953 monitoring from instant power to accumulated energy [#16941](https://github.com/arendst/Tasmota/issues/16941)
- TuyaMcu rewrite by btsimonh [#17051](https://github.com/arendst/Tasmota/issues/17051)
- WS2812 sends signal to only ``Pixels`` leds instead of sending to 512 leds [#17055](https://github.com/arendst/Tasmota/issues/17055)
- AC PWM dimmer lineair power distribution [#17177](https://github.com/arendst/Tasmota/issues/17177)
- Zigbee improved Aqara plug support and completed cluster 0x0702 [#17073](https://github.com/arendst/Tasmota/issues/17073)

### Fixed
- Serial bridge default serial configuration from 5N1 to 8N1 regression from v10.1.0.3
- BP5758D red channel corruption regression from v12.1.1.6 [#16850](https://github.com/arendst/Tasmota/issues/16850)
- Deduplicate code and fix %timer n% rule regression from v12.2.0 [#16914](https://github.com/arendst/Tasmota/issues/16914)
- Serial initialization for baudrate and config [#16970](https://github.com/arendst/Tasmota/issues/16970)
- ModbusBridge buffer overflow [#16979](https://github.com/arendst/Tasmota/issues/16979)
- ModbusBridge baudrates over 76500 baud [#17106](https://github.com/arendst/Tasmota/issues/17106)
- SenseAir S8 module detection [#17033](https://github.com/arendst/Tasmota/issues/17033)

### Removed
- Define ``USE_PN532_DATA_RAW`` from NFC reader [#16939](https://github.com/arendst/Tasmota/issues/16939)
