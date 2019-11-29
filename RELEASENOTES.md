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

## Supported Modules

The following hardware modules are supported.

Module            | Description
------------------|-----------------------
01 Sonoff Basic   | Sonoff Basic Wifi Smart Switch
02 Sonoff RF      | Sonoff RF Wifi Smart Switch with RF (434MHz) receiver
03 Sonoff SV      | Sonoff SV Safe Voltage Wifi Smart Switch
04 Sonoff TH      | Sonoff TH10/TH16 Wifi Smart Switch with Sensor connection
05 Sonoff Dual    | Sonoff Dual Wifi Smart Switch
06 Sonoff Pow     | Sonoff Pow Wifi Smart Switch with Energy Monitoring
07 Sonoff 4CH     | Sonoff 4CH 4-gang Wifi Smart Switch
08 Sonoff S2X     | Sonoff S20/S26 Wifi Smart Socket
09 Slampher       | Sonoff Slampher Wifi Smart Light Bulb Socket with RF (434MHz) receiver
10 Sonoff Touch   | Sonoff Touch Wifi Light Switch
11 Sonoff LED     | Sonoff Led Wifi Led Pack (Retired)
12 1 Channel      | 1 Channel Inching/Self Locking Wifi Switch 5V/12V
13 4 Channel      | 4 Channel Inching/Self Locking Wifi Switch (Retired)
14 Motor C/AC     | Motor Clockwise/Antoclockwise Wifi Switch (Retired)
15 ElectroDragon  | Electrodragon Wifi IoT Board
16 EXS Relay(s)   | Electronic Experience Store 1 or 2-gang Wifi Module
17 WiOn           | WiOn Wifi Smart Socket
18 Generic        | Any ESP8266/ESP8285 device like WeMos and NodeMCU
19 Sonoff Dev     | Sonoff Dev Wifi Development Board
20 H801           | H801 Wifi RGBWW Led Controller
21 Sonoff SC      | Sonoff SC Wifi Environmental Monitor
22 Sonoff BN-SZ   | Sonoff BN-SZ01 Wifi Ceiling Led (Retired)
23 Sonoff 4CH Pro | Sonoff 4CH Pro 4-gang Wifi Smart Switch
24 Huafan SS      | HuaFan Wifi Smart Socket
25 Sonoff Bridge  | Sonoff RF (434MHz) transceive to Wifi Bridge
26 Sonoff B1      | Sonoff B1 Wifi RGBWW Led Bulb
27 AiLight        | Ai-Thinker RGBW Led Bulb
28 Sonoff T1 1CH  | Sonoff T1 1-gang Wifi Light Switch
29 Sonoff T1 2CH  | Sonoff T1 2-gang Wifi Light Switch
30 Sonoff T1 3CH  | Sonoff T1 3-gang Wifi Light Switch
31 Supla Espablo  | 2-gang Wifi Module
32 Witty Cloud    | Witty Cloud ESP8266 Wifi Development Board
33 Yunshan Relay  | ESP8266 Wifi Network Relay Module
34 MagicHome      | MagicHome, Flux-light and some Arilux LC10 RGB(W) Led Controller
35 Luani HVIO     | Luani ESP8266 Wifi I/O Module
36 KMC 70011      | KMC Wifi Smart Socket with Energy Monitoring
37 Arilux LC01    | Arilux AL-LC01 RGB Led Controller
38 Arilux LC11    | Arilux AL-LC11 RGBWW Led Controller
39 Sonoff Dual R2 | Sonoff Dual R2 Wifi Smart Switch
40 Arilux LC06    | Arilux AL-LC06 RGB(WW) Led Controller
41 Sonoff S31     | Sonoff S31 Wifi Smart Socket with Energy Monitoring
42 Zengge WF017   | Zengge WF017 Wifi RGB(W) Led Controller
43 Sonoff Pow R2  | Sonoff Pow R2 Wifi Smart Switch with Energy Monitoring
44 Sonoff iFan02  | Sonoff iFan02 Wifi Smart Ceiling Fan with Light
45 BlitzWolf SHP  | BlitzWolf BW-SHP2, BW-SHP6, HomeCube SP1, Gosund SP111, Teckin SP22 Wifi Smart Switch with Energy Monitoring
46 Shelly 1       | Shelly 1 Open Source Wifi Relay Module
47 Shelly 2       | Shelly 2 Wifi 2-gang Relay Module with Energy Monitoring
48 Xiaomi Philips | Xiaomi Philips Wifi WW Led Bulb
49 Neo Coolcam    | Neo Coolcam Wifi Smart Socket
50 ESP Switch     | ESP Switch 4-gang Wifi Switch with Leds
51 OBI Socket     | OBI Wifi Smart Socket
52 Teckin         | Teckin SP22 Wifi Smart Switch with Energy Monitoring
53 AplicWDP303075 | Aplic WDP 303075 CSL Wifi Smart Switch with Energy Monitoring
54 Tuya Dimmer    | MIUO (and other Tuya based) Wifi Dimmer for Incandescent Lights and Led
55 Gosund SP1 v23 | Gosund SP1 v2.3 Wifi Smart Switch with Energy Monitoring
56 ARMTR Dimmer   | ARMtronix Wifi dimmer for Incandescent Lights and Led
57 SK03 Outdoor   | SK03 Outdoor Wifi Smart Switch with Energy Monitoring
58 PS-16-DZ       | PS-16-DZ  Wifi dimmer for Incandescent Lights and Led
59 Teckin US      | Teckin SP20 and ZooZee SA102 Wifi Smart Switch with Energy Monitoring
60 Manzoku strip  | Manzoku Wifi Smart Power Strip with four Relays
61 OBI Socket 2   | OBI 2 Wifi Smart Socket
62 YTF IR Bridge  | YTF Infra Red Wifi Bridge
63 Digoo DG-SP202 | Digoo DG-SP202 Dual Wifi Smart Switch with Energy Monitoring
64 KA10           | Smanergy KA10 Wifi Smart Wall Switch with Energy Monitoring
65 Luminea ZX2820 | Luminea ZX2820 Wifi Smart Switch with Energy Monitoring
66 Mi Desk Lamp   | Mi Desk Lamp with rotary switch and Wifi
67 SP10           | Tuya SP10 Wifi Smart Switch with Energy Monitoring
68 WAGA CHCZ02MB  | WAGA life CHCZ02MB Wifi Smart Switch with Energy Monitoring
69 SYF05          | Sunyesmart SYF05 RGBWW Wifi Led Bulb
70 Sonoff L1      | Sonoff L1 light strip
71 Sonoff iFan03  | Sonoff iFan03 Wifi Smart Ceiling Fan with Light
72 EXS Dimmer     | EXS Wifi Dimmer v4

Over 500 additional devices are supported using [templates](TEMPLATES.md).

## Provided Binary Downloads

The following binary downloads have been compiled with ESP8266/Arduino library core version **2.6.1**.

- **tasmota.bin** = The Tasmota version with sensors. **RECOMMENDED RELEASE BINARY**
- **tasmota-BG.bin** to **tasmota-TW.bin** = The Tasmota version in different languages.
- **tasmota-basic.bin** = The Basic version without most sensors.
- **tasmota-knx.bin** = The Knx version without some features but adds KNX support.
- **tasmota-sensors.bin** = The Sensors version adds more useful sensors.
- **tasmota-ir** = The InfraRed Receiver and transmitter version allowing all available protocols provided by library IRremoteESP8266 but without most other features.
- **tasmota-display.bin** = The Display version without Energy Monitoring but adds display support.
- **tasmota-minimal.bin** = The Minimal version allows intermediate OTA uploads to support larger versions and does NOT change any persistent parameter. This version **should NOT be used for initial installation**.

[Complete list](BUILDS.md) of available feature and sensors.

## Changelog

### Version 7.1.0 Betty

- Remove update support for versions before 6.0
- Remove driver xsns_12_ads1115_i2cdev replaced by xsns_12_ads1115
- Remove most IR protocols from non dedicated IR firmware except NEC, RC5 and RC6
- Change repository name from Sonoff-Tasmota to Tasmota and all code references from Sonoff to Tasmota
- Change documentation from wiki to [documentation repository](https://tasmota.github.io/docs/) by @Blakadder
- Change default GUI to dark theme
- Change ArduinoSlave to TasmotaSlave
- Change IRremoteESP8266 library to v2.7.1
- Change supported PCF8574 I2C address range to 0x20 - 0x26 allowing other I2C devices with address 0x27 to be used at the same time
- Change supported PCF8574A I2C address range to 0x39 - 0x3F allowing other I2C devices with address 0x38 to be used at the same time
- Change supported MCP230xx I2C address range to 0x20 - 0x26 allowing other I2C devices with address 0x27 to be used at the same time
- Change Reset erase end address from as seen by SDK (getFlashChipSize) to full flash size (getFlashChipRealSize)
- Change new Fade system much smoother, Speed now up to 40 (#6942, #3714)
- Fix better control of RGB/White when ``SetOption37`` >128, added ``Dimmer1`` and ``Dimmer2`` commands (#6714)
- Fix random crash caused by UPNP flood
- Fix check deepsleep for valid values in Settings (#6961)
- Fix Wifi instability when light is on, due to ``Sleep 0`` (#6961, #6608)
- Fix auto-power on/off when setting channel to non-zero or zero value, when ``SetOption68 1``
- Fix postpone saving settings to flash until Fade is complete, avoids pause in Fade
- Add support for Tuya battery powered devices (#6735)
- Add support for Honeywell I2C HIH series Humidity and Temperetaure sensor (#6808)
- Add support for Honeywell HPMA115S0 particle concentration sensor by David Hunt (#6843)
- Add support for I2C sensor TLS2591 Light Intensity sensor (#6873)
- Add command ``SetOption73 0/1`` to re-enable HTTP Cross-Origin Resource Sharing (CORS) now default disabled (#6767)
- Add command ``SetOption74 0/1`` to enable DS18x20 internal pull-up and remove define DS18B20_INTERNAL_PULLUP (#6795)
- Add command ``SetOption75 0/1`` to switch between grouptopic (0) using fulltopic replacing %topic% or (1) is cmnd/\<grouptopic\> (#6779)
- Add command ``SetOption76 0/1`` to enable incrementing bootcount when deepsleep is enabled (#6930)
- Add command ``SetOption77 0/1`` to keep power on when slider is far left
- Add command ``I2cDriver`` for I2C driver runtime control using document I2CDEVICES.md
- Add command ``TempOffset -12.6 .. 12.6`` to set global temperature sensor offset (#6958)
- Add command ``WebColor19`` to control color of Module and Name (#6811)
- Add command ``WifiPower 0 .. 20.5`` to set Wifi Output Power which will be default set to 17dBm
- Add frequency to ADE7953 energy monitor as used in Shelly 2.5 by ljakob (#6778)
- Add hide Alexa objects with friendlyname starting with '$' (#6722, #6762)
- Add Zigbee command support, considered as v1.0 for full Zigbee support
- Add hardware detection to be overruled with ``SetOption51`` (#6969)
- Add Colorpicker to WebUI by Christian Staars (#6984)
