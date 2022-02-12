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
4. Migrate to **Sonoff-Tasmota 6.7.1** (http://ota.tasmota.com/tasmota/release-6.7.1/)
5. Migrate to **Tasmota 7.2.0** (http://ota.tasmota.com/tasmota/release-7.2.0/)

--- Major change in parameter storage layout ---

6. Migrate to **Tasmota 8.5.1** (http://ota.tasmota.com/tasmota/release-8.5.1/)

--- Major change in internal GPIO function representation ---

7. Migrate to **Tasmota 9.1** (http://ota.tasmota.com/tasmota/release-9.1.0/)
8. Upgrade to **latest release** (http://ota.tasmota.com/tasmota/release/)

While fallback or downgrading is common practice it was never supported due to Settings additions or changes in newer releases. Starting with release **v9.1.0 Imogen** the internal GPIO function representation has changed in such a way that fallback is only possible to the latest GPIO configuration before installing **v9.1.0**.

## Supported Core versions

This release will be supported from ESP8266/Arduino library Core version **2.7.4.9** due to reported security and stability issues on previous Core version. This will also support gzipped binaries.

This release will be supported from ESP32/Arduino library Core version **2.0.2.1**.

Support of ESP8266 Core versions before 2.7.4.9 and ESP32 Core versions before 2.0.2 have been removed.

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
- http://ota.tasmota.com/tasmota/release-11.0.0

The latter links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota/release/tasmota.bin.gz``

### ESP32 based
The following binary downloads have been compiled with ESP32/Arduino library core version **2.0.2.1**.

- **tasmota32.bin** = The Tasmota version with most drivers including additional sensors and KNX for 4M+ flash.  **RECOMMENDED RELEASE BINARY**
- **tasmota32c3.bin** = The Tasmota version with most drivers including additional sensors and KNX for ESP32-C3 and 4M+ flash.
- **tasmota32solo1.bin** = The Tasmota version with most drivers including additional sensors and KNX for single core ESP32 and 4M+ flash.
- **tasmota32-AF.bin** to **tasmota32-VN.bin** = The Tasmota version in different languages for 4M+ flash.
- **tasmota32-ir.bin** = The InfraRed Receiver and transmitter version allowing all available protocols provided by library IRremoteESP8266 but without most other features for 4M+ flash.
- **tasmota32-display.bin** = The Display version without Energy Monitoring but adds display support for 4M+ flash.
- **tasmota32-lvgl.bin** = The LVGL version adds Light and Versatile Graphics Library (LVGL) display support for 4M+ flash.
- **tasmota32-webcam.bin** = The Webcam version adds webcam support for 4M+ flash.
- **tasmota32-bluetooth.bin** = The Bluetooth version adds BLE support for 4M+ flash.

Latest released binaries can be downloaded from
- https://github.com/arendst/Tasmota-firmware/tree/main/release-firmware
- http://ota.tasmota.com/tasmota32/release

Historical binaries can be downloaded from
- http://ota.tasmota.com/tasmota32/release-11.0.0

The latter links can be used for OTA upgrades too like ``OtaUrl http://ota.tasmota.com/tasmota32/release/tasmota32.bin``

## Additional information

[List](MODULES.md) of embedded modules.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog v11.0.0 Olivia
### Added
- Command ``Json {<Tasmota commands>}`` to enable input of any command as JSON tokens [#14568](https://github.com/arendst/Tasmota/issues/14568)
- Command ``SetOption44 1..100`` to set base tolerance percentage for matching incoming IR messages (default 25, max 100) [#14555](https://github.com/arendst/Tasmota/issues/14555)
- Command ``SetOption134 1`` to disable PWM auto-phasing for lights by default (new behavior) [#14590](https://github.com/arendst/Tasmota/issues/14590)
- Command ``SSerialConfig <serialconfig>`` to change Serial Bridge configuration
- Command ``SspmDisplay 1`` to display Sonoff SPM energy data in GUI for relays powered on only
- Command ``SspmEnergyTotal<relay>`` to (p)reset Sonoff SPM total energy without today's energy
- Command ``SspmEnergyYesterday<relay>`` to (p)reset Sonoff SPM energy yesterday
- Command ``SspmHistory<relay>`` to retrieve Sonoff SPM daily energy up to last six month (as defined by ARM firmware)
- Command ``SspmIAmHere<relay>`` to (faintly) blink Sonoff SPM-4Relay module error light of requested relay
- Command ``SspmLog<relay> [x]`` to retrieve Sonoff SPM relay power state change and cause logging
- Command ``SspmMap 2,1,..`` to map Sonoff SPM scanned module to physical module [#14281](https://github.com/arendst/Tasmota/issues/14281)
- Command ``SspmOverload<relay> <options>`` to set Sonoff SPM overload criteria for any relay
- Command ``SspmScan`` to rescan Sonoff SPM modbus
- Command ``WebTime <start_pos>,<end_pos>`` to show part of date and/or time in web gui based on "2017-03-07T11:08:02-07:00"
- Commands for ESP32 ethernet configuration ``EthIpAddress``, ``EthGateway``, ``EthSubnetmask``, ``EthDnsServer1`` and ``EthDnsServer2`` [#14385](https://github.com/arendst/Tasmota/issues/14385)
- Support for Eastron SDM230 modBus energy meter [#13443](https://github.com/arendst/Tasmota/issues/13443)
- PWM Dimmer two button support [#13993](https://github.com/arendst/Tasmota/issues/13993)
- Support for Linkind dimmer as GPIO ``Option A6`` [#14004](https://github.com/arendst/Tasmota/issues/14004)
- DDP schemes for light and WS2812 [#14017](https://github.com/arendst/Tasmota/issues/14017)
- Device Group Send full status item [#14045](https://github.com/arendst/Tasmota/issues/14045)
- Support for MAX7219 Dot Matrix displays [#14091](https://github.com/arendst/Tasmota/issues/14091)
- Experimental ADE7953 (Shelly EM) reset on restart [#14261](https://github.com/arendst/Tasmota/issues/14261)
- Solax X1 negative temperature support [#14278](https://github.com/arendst/Tasmota/issues/14278)
- Solax X1 modbus RTS support and offline status [#14305](https://github.com/arendst/Tasmota/issues/14305)
- Tasmota favicon to webbrowser tab [#14322](https://github.com/arendst/Tasmota/issues/14322)
- Support for BME688 with latest Bosch-Sensor-API library [#14513](https://github.com/arendst/Tasmota/issues/14513)
- Rule variable %color% [#14572](https://github.com/arendst/Tasmota/issues/14572)
- Rule variables %timer1% to %timer16% [#14619](https://github.com/arendst/Tasmota/issues/14619)
- Support for MQ analog sensor for air quality by Francesco Adriani [#14581](https://github.com/arendst/Tasmota/issues/14581)
- ESP32 single binary firmware [#14239](https://github.com/arendst/Tasmota/issues/14239)
- ESP32 disable serial console when 3 (ESP32) or 2 (Other models) serial interfaces are requested [#14487](https://github.com/arendst/Tasmota/issues/14487)
- ESP32 support for TuyaMcu
- ESP32 Berry features
- ESP32 support for USE_PWM_DIMMER as GPIO ``Option E1``
- ESP32 increase PWM channels to 16
- ESP32 auto-configuration
- ESP32 initial support for ESP32S3 with support for 38 configurable GPIOs

### Breaking Changed

### Changed
- ESP32 core library from v2.0.2 to v2.0.2.1
- PubSubClient library from v2.8.12 to v2.8.13
- TasmotaSerial library from v3.3.0 to v3.4.0
- TasmotaModbus library from v1.2.0 to v3.4.0
- ESP8266Audio library from v1.9.2 to v1.9.5
- ESP8266SAM library from v1.0 to v1.0.1
- BME68x-Sensor-API library from v3.5.9 to v4.4.7
- SR04 driver to support US-100
- Mitsubishi HVAC temperature resolution [#13936](https://github.com/arendst/Tasmota/issues/13936)
- Remove restriction of topic must differ from mqttclient [#14019](https://github.com/arendst/Tasmota/issues/14019)
- ESP32 Set stack size with ``#define SET_ESP32_STACK_SIZE``, added ``StackLowMark`` metrics
- ESP32 Berry stores compiled bytecode into IRAM, freeing space in heap [#14307](https://github.com/arendst/Tasmota/issues/14307)

### Fixed
- Intermittent exceptions and heap corruption due to PubSubClient library buffer overflow [#13700](https://github.com/arendst/Tasmota/issues/13700)
- EZOO2 sensor message format [#14000](https://github.com/arendst/Tasmota/issues/14000)
- DHT support negative temperatures on different hardware [#14173](https://github.com/arendst/Tasmota/issues/14173)
- Hardware serial parity and stop bits support [#14212](https://github.com/arendst/Tasmota/issues/14212)
- Edit file for SD card [#14229](https://github.com/arendst/Tasmota/issues/14229)
- Scripter memory corruption [#14268](https://github.com/arendst/Tasmota/issues/14268)
- ESP32 Webcam exception during flashwrites
- ESP32 LedPwmMode exception [#14073](https://github.com/arendst/Tasmota/issues/14073)
- ESP32 Compile error when I2S_Audio is enabled [#14095](https://github.com/arendst/Tasmota/issues/14095)
- ESP32 Provide proper OTA_URL for tasmota32solo1 [#14202](https://github.com/arendst/Tasmota/issues/14202)
- ESP32 OneWire-Stickbreaker (DS18x20) library support for ESP32C3 and ESP32S2 [#14338](https://github.com/arendst/Tasmota/issues/14338)

### Removed
- ESP32 consolidate odroidgo and core2 binaries into tasmota32-lvgl using Berry features
