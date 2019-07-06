<img src="https://github.com/arendst/Sonoff-Tasmota/blob/master/tools/logo/TASMOTA_FullLogo_Vector.svg" alt="Logo" align="right" height="76"/>

# RELEASE NOTES

## Migration Information
See [wiki migration path](https://github.com/arendst/Sonoff-Tasmota/wiki/Upgrade#migration-path) for instructions how to migrate to a major version. Pay attention to the following version breaks due to dynamic settings updates:

1. Migrate to **Sonoff-Tasmota 3.9.x**
2. Migrate to **Sonoff-Tasmota 4.x**
3. Migrate to **Sonoff-Tasmota 5.14**
4. Migrate to **Sonoff-Tasmota 6.x**

## Support of TLS
TLS support for core 2.3.0 is removed.

TLS is supported on core 2.4.2 and up. To save resources when TLS is enabled mDNS needs to be disabled. In addition to TLS using fingerprints now also user supplied CA certs and AWS IoT is supported. See full documentation on https://github.com/arendst/Sonoff-Tasmota/wiki/AWS-IoT

## Core version 2.3.0 vs 2.4.2 vs 2.5.2
This release is based on ESP8266/Arduino library core 2.3.0 as some people encountered wifi related issues on core 2.4.2 and 2.5.2. For others core 2.4.2 or 2.5.2 is working just fine. All version are available from http://thehackbox.org/tasmota/release/

## Change in default initial configuration tool
Firmware binary **sonoff-classic.bin** supports **WifiManager, Wps and SmartConfig** for initial configuration. The default tool is **Wps**.

To save memory space all other binaries support **WifiManager only**.

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

## Provided Binary Downloads
The following binary downloads have been compiled with ESP8266/Arduino library core version **2.3.0**.

- **sonoff.bin** = The Sonoff version without Wps and SmartConfig configuration but adds more sensors. **RECOMMENDED RELEASE BINARY**
- **sonoff-basic.bin** = The Basic version without Wps and SmartConfig configuration and most sensors.
- **sonoff-classic.bin** = The Classic version allows initial installation using either WifiManager, Wps or SmartConfig.
- **sonoff-BG.bin** to **sonoff-TW.bin** = The Sonoff version without Wps and SmartConfig configuration in different languages.
- **sonoff-knx.bin** = The Knx version without Wps and SmartConfig configuration and some other features but adds KNX support.
- **sonoff-sensors.bin** = The Sensors version without Wps and SmartConfig configuration but adds even more useful sensors.
- **sonoff-display.bin** = The Display version without Wps and SmartConfig configuration and Energy Monitoring but adds display support.
- **sonoff-minimal.bin** = The Minimal version allows intermediate OTA uploads to support larger versions and does NOT change any persistent parameter. This version **should NOT be used for initial installation**.

Core version **2.4.2** binaries can be found at http://thehackbox.org/tasmota/release/020402/

Core version **2.5.2** binaries can be found at http://thehackbox.org/tasmota/release/020502/

## Available Features and Sensors

| Feature or Sensor     | minimal | basic | classic | sonoff | knx  | sensors | display | Remarks
|-----------------------|---------|-------|---------|--------|------|---------|---------|--------
| MY_LANGUAGE en-GB     | x | x | x | x | x | x | x |
| USE_WPS               | - | - | x | - | - | - | - | WPS
| USE_SMARTCONFIG       | - | - | x | - | - | - | - | SmartConfig
| USE_ARDUINO_OTA       | - | - | - | - | - | - | - |
| USE_DOMOTICZ          | - | - | x | x | x | x | - |
| USE_HOME_ASSISTANT    | - | - | - | x | x | x | - |
| USE_MQTT_TLS          | - | - | - | - | - | - | - |
| USE_MQTT_TLS_CA_CERT  | - | - | - | - | - | - | - |
| USE_MQTT_AWS_IOT      | - | - | - | - | - | - | - |
| USE_KNX               | - | - | - | - | x | - | - |
| USE_WEBSERVER         | x | x | x | x | x | x | x | WifiManager
| USE_EMULATION_HUE     | - | x | x | x | - | x | - |
| USE_EMULATION_WEMO    | - | x | x | x | - | x | - |
| USE_DISCOVERY         | - | - | x | x | x | x | x |
| WEBSERVER_ADVERTISE   | - | - | x | x | x | x | x |
| MQTT_HOST_DISCOVERY   | - | - | x | x | x | x | x |
| USE_TIMERS            | - | x | - | x | x | x | x |
| USE_TIMERS_WEB        | - | x | - | x | x | x | x |
| USE_SUNRISE           | - | x | - | x | x | x | x |
| USE_RULES             | - | x | - | x | x | x | x |
| USE_SCRIPT            | - | - | - | - | - | - | - |
| USE_EXPRESSION        | - | - | - | - | - | - | - |
|                       |   |   |   |   |   |   |   |
| USE_ADC_VCC           | x | x | x | - | - | - | - |
| USE_COUNTER           | - | - | - | x | x | x | x |
| USE_DS18B20           | - | - | - | - | - | - | - | Single sensor
| USE_DS18x20           | - | - | x | x | x | x | x | Multiple sensors
| USE_DS18x20_LEGACY    | - | - | - | - | - | - | - | Multiple sensors
| USE_DHT               | - | - | x | x | x | x | x |
|                       |   |   |   |   |   |   |   |
| Feature or Sensor     | minimal | basic | classic | sonoff | knx  | sensors | display | Remarks
| USE_I2C               | - | - | - | x | x | x | x |
| USE_SHT               | - | - | - | x | x | x | x |
| USE_HTU               | - | - | - | x | x | x | x |
| USE_BMP               | - | - | - | x | x | x | x |
| USE_BME680            | - | - | - | - | - | x | - |
| USE_BH1750            | - | - | - | x | x | x | x |
| USE_VEML6070          | - | - | - | - | - | x | - |
| USE_ADS1115           | - | - | - | - | - | x | - |
| USE_ADS1115_I2CDEV    | - | - | - | - | - | - | - |
| USE_INA219            | - | - | - | - | - | x | - |
| USE_SHT3X             | - | - | - | x | x | x | x |
| USE_TSL2561           | - | - | - | - | - | x | - |
| USE_MGS               | - | - | - | - | - | x | - |
| USE_SGP30             | - | - | - | x | x | x | x |
| USE_SI1145            | - | - | - | - | - | - | - |
| USE_LM75AD            | - | - | - | x | x | x | x |
| USE_APDS9960          | - | - | - | - | - | - | - |
| USE_MCP230xx          | - | - | - | - | - | - | - |
| USE_PCA9685           | - | - | - | - | - | - | - |
| USE_MPR121            | - | - | - | - | - | - | - |
| USE_CCS811            | - | - | - | - | - | - | - |
| USE_MPU6050           | - | - | - | - | - | - | - |
| USE_DS3231            | - | - | - | - | - | - | - |
| USE_MGC3130           | - | - | - | - | - | - | - |
| USE_MAX44009          | - | - | - | - | - | - | - |
| USE_SCD30             | - | - | - | - | - | x | - |
| USE_SPS30             | - | - | - | - | - | - | - |
| USE_ADE7953           | - | - | - | x | x | x | x |
| USE_VL53L0X           | - | - | - | - | - | - | - |
| USE_MLX90614          | - | - | - | - | - | - | - |
|                       |   |   |   |   |   |   |   |
| Feature or Sensor     | minimal | basic | classic | sonoff | knx  | sensors | display | Remarks
| USE_SPI               | - | - | - | - | - | - | x |
| USE_MHZ19             | - | - | - | x | x | x | x |
| USE_SENSEAIR          | - | - | - | x | x | x | x |
| USE_PMS5003           | - | - | - | x | x | x | x |
| USE_NOVA_SDS          | - | - | - | x | x | x | x |
| USE_ENERGY_SENSOR     | - | x | x | x | x | x | - |
| USE_PZEM004T          | - | - | - | x | x | x | - |
| USE_PZEM_AC           | - | - | - | x | x | x | - |
| USE_PZEM_DC           | - | - | - | x | x | x | - |
| USE_MCP39F501         | - | x | - | x | x | x | - |
| USE_SERIAL_BRIDGE     | - | - | - | x | x | x | x |
| USE_SDM120            | - | - | - | - | - | x | - |
| USE_SDM630            | - | - | - | - | - | x | - |
| USE_MP3_PLAYER        | - | - | - | - | - | x | - |
| USE_TUYA_DIMMER       | - | x | - | x | x | x | x |
| USE_ARMTRONIX_DIMMERS | - | x | - | x | x | x | x |
| USE_PS_16_DZ          | - | x | - | x | x | x | x |
| USE_AZ7798            | - | - | - | - | - | - | - |
| USE_PN532_HSU         | - | - | - | - | - | x | - |
| USE_IR_REMOTE         | - | - | - | x | x | x | x |
| USE_IR_HVAC           | - | - | - | - | - | x | - |
| USE_IR_RECEIVE        | - | - | - | x | x | x | x |
| USE_WS2812            | - | - | x | x | x | x | x |
| USE_WS2812_DMA        | - | - | - | - | - | - | - |
| USE_ARILUX_RF         | - | - | - | x | x | x | - |
| USE_SR04              | - | - | - | x | x | x | x |
| USE_TM1638            | - | - | - | - | - | x | - |
| USE_HX711             | - | - | - | x | x | x | x |
| USE_RF_FLASH          | - | - | - | x | x | x | - |
| USE_TX20_WIND_SENSOR  | - | - | - | x | x | x | x |
| USE_RC_SWITCH         | - | - | - | x | x | x | x |
| USE_RF_SENSOR         | - | - | - | - | - | x | - | AlectoV2 only
| USE_SM16716           | - | x | x | x | x | x | x |
| USE_HRE               | - | - | - | - | - | x | - |
| USE_DISPLAY           | - | - | - | - | - | - | x |
| USE_DISPLAY_LCD       | - | - | - | - | - | - | x |
| USE_DISPLAY_SSD1306   | - | - | - | - | - | - | x |
| USE_DISPLAY_MATRIX    | - | - | - | - | - | - | x |
| USE_DISPLAY_ILI9341   | - | - | - | - | - | - | x |
| USE_DISPLAY_EPAPER_29 | - | - | - | - | - | - | x | Disabled for core 2.3.0

## Changelog
Version 6.6.0 20190707
 * Remove support of TLS on core 2.3.0 and extent support on core 2.4.2 and up
 * Remove MQTT uptime message every hour
 * Refactor some defines to const
 * Refactor webserver HTML input, button, textarea, and select name based on id
 * Refactor webserver sensor data collection
 * Refactor TLS based on BearSSL, warning breaking change for fingerprints validation
 * Refactor management of lights, using classes and integers instead of floats
 * Refactor UDP initial message handling from string to char using static memory and add debug info (#5505)
 * Refactor ``IRsend`` and receive for 64-bit support (#5523)
 * Refactor MQTT which might solve issue (#5755)
 * Refactor ``IRSend`` by using heap when more than 199 values need to be send. May need increase of define MQTT_MAX_PACKET_SIZE too (#5950)
 * Refactor double to float in rules, and replaced trigonometric functions from stdlib with smaller versions (#6005)
 * Change pubsubclient MQTT_KEEPALIVE from 10 to 30 seconds for AWS IoT support
 * Change gamma correction as default behavior, ie "Ledtable 1"
 * Change PWM resolution from 8 to 10 bits for low brightness lights
 * Change ``IRSend`` Panasonic protocol to 64-bit (#5523)
 * Change ADC0 to enabled by default in my_user_config.h (#5671)
 * Change define USE_EMULATION by USE_EMULATION_HUE and USE_EMULATION_WEMO (#5826)
 * Change default ``PowerDelta`` from 80% to 0% on new installations (#5858, #5028, #4813, #4130, #4145, #3795, #3778, #3660, #3648)
 * Fix display Bug in KNX webmenu for Physical Address
 * Fix the Unescape() function and the ``SendSerial3`` behaviour
 * Fix webserver multiple Javascript window.onload functionality
 * Fix TasmotaSerial at 9600 bps solving DFPlayer comms (#5528)
 * Fix Configure Timer Web GUI (#5568)
 * Fix Shelly 2.5 I2C address priority issue when VEML6070 code is present by disabling VEML6070 for Shelly 2.5 (#5592)
 * Fix use of ``SerialDelimiter`` value 128 (#5634)
 * Fix Sonoff Pow R2 / S31 invalid energy increments (#5789)
 * Fix core 2.5.x ISR not in IRAM exception (#5837)
 * Fix Philips Hue emulation Alexa issue by using part of MAC address for LightId (#5849)
 * Fix missing white channel for WS2812 (#5869)
 * Fix PZem startup issue (#5875)
 * Fix exception 9 when syslog is enabled and NTP is just synced (#5917)
 * Fix Toggle functionality to button double press when one button and two devices are detected (#5935)
 * Fix command ``Channel`` for dual dimmers (#5940)
 * Fix not restoring white value on power off/power on (#5993)
 * Add command ``AdcParam`` to control ADC0 Temperature and Light formula parameters
 * Add command ``LedMask`` to assign which relay has access to power LED (#5602, #5612)
 * Add extended LED power control using command ``LedPowerX`` where X is 1 to 4. Enabled when "LedLink(i)" is configured too (#5709)
 * Add command ``Sensor20 1..255`` to change Nova Fitness SDS01 working period in minutes (#5452)
 * Add command ``SetOption38 6..255`` to set IRReceive protocol detection sensitivity mimizing UNKNOWN protocols (#5853)
 * Add command ``SetOption39 1..255`` to control CSE7766 (Pow R2) or HLW8032 (Blitzwolf SHP5) handling of power loads below 6W. Default setting is 128 (#5756)
 * Add command ``SetOption40 0..250`` to disable button functionality if activated for over 0.1 second. Needs SetOption1 1 and SetOption13 0 (#5449)
 * Add command ``SetOption63 0/1`` to disable relay state feedback scan at restart (#5594, #5663)
 * Add command ``SetOption64 0/1`` to switch between "-" or "_" as sensor index separator impacting DS18X20, DHT, BMP and SHT3X sensor names (#5689)
 * Add command ``SetOption65 0/1`` and more Tuya Serial based device support (#5815)
 * Add command ``WebColor`` to change GUI colors on the fly
 * Add support for AWS IoT with TLS 1.2 on core 2.4.2 and up. Full doc here: https://github.com/arendst/Sonoff-Tasmota/wiki/AWS-IoT
 * Add support for Badger HR-E Water Meter (#5539)
 * Add support for Shelly 2.5 Energy and overtemp Monitoring (#5592)
 * Add support for color and colortone for Philips Hue emulation via Alexa (#5600 #4809)
 * Add support for Scripts as replacement for Rules. Default disabled but can be enabled in my_user_config.h (#5689)
 * Add support for up to four LEDs related to four power outputs. Enabled when "LedLink(i)" is configured too (#5709)
 * Add support for Shelly 1PM Template ``{"NAME":"Shelly 1PM","GPIO":[56,0,0,0,82,134,0,0,0,0,0,21,0],"FLAG":2,"BASE":18}`` (#5716)
 * Add support for SPS30 Particle sensor thanks to Gerhard Mutz (#5830)
 * Add support for VL53L0x time of flight sensor. Might interfere with TSL2561 using same I2C address (#5845)
 * Add support for Sonoff L1 thanks to reef-actor (#6002)
 * Add rule Http#Initialized
 * Add rule System#Save executed just before a planned restart
 * Add rule support for single JSON value pair like {"SSerialReceived":"on"} by expanding it to {"SSerialReceived":{"Data":"on"}} allowing for trigger SSerialReceived#Data=on (#5638)
 * Add define USE_COUNTER to my_user_config.h to save space in sonoff-basic.bin and sonoff-minimal.bin
 * Add define USE_DHT to my_user_config.h to save space in sonoff-basic.bin
 * Add defines USE_EMULATION_WEMO and USE_EMULATION_HUE to my_user_config.h to control emulation features at compile time (#5826)
 * Add Toggle functionality to button double press when more devices are detected
 * Add device OverTemp (>73 Celsius) detection to Energy Monitoring devices with temperature sensor powering off all outputs
 * Add Tuya Dimmer 10 second heartbeat serial packet required by some Tuya dimmer secondary MCUs
 * Add all temperature, humidity and pressure for global access
 * Add validation check when loading settings from flash
 * Add HX711 weight restore after controlled restart or after power restore just before executing command Sensor34 7 (#5367, #5786)
 * Add GUI hexadecimal color options in my_user_config.h (#5586)
 * Add alternative ``IRSend`` command syntax ``IRSend raw,<freq>,<header mark>,<header space>,<bit mark>,<zero space>,<one space>,<bit stream>`` (#5610)
 * Add user configurable ADC0 to Module and Template configuration compatible with current FLAG options (#5671)
 * Add AriLux RF control GPIO option "ALux IrSel" (159) replacing "Led4i" (59) for full LED control (#5709)
 * Add LED GPIO option "LedLink" (157) and "LedLinki" (158) to select dedicated link status LED (#5709)
 * Add all 5 PWM channels individually adressable with LEDs. (#5741)
 * Add reset of Energy values when connection to sensor is lost for over 4 seconds (#5874, #5881)
 * Add checkbox to GUI password field enabling visibility during password entry only (#5934)