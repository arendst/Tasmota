## Migration Information
See [wiki migration path](https://github.com/arendst/Sonoff-Tasmota/wiki/Upgrade#migration-path) for instructions how to migrate to a major version. Pay attention to the following version breaks due to dynamic settings updates:

1. Migrate to **Sonoff-Tasmota 3.9.x**
2. Migrate to **Sonoff-Tasmota 4.x**
3. Migrate to **Sonoff-Tasmota 5.14**
4. Migrate to **Sonoff-Tasmota 6.x**

## Release notes
### Core version 2.3.0 vs 2.4.2
This release is based on ESP8266/Arduino library core 2.3.0 (again) as some people encountered wifi related issues on core 2.4.2. For others core 2.4.2 is working just fine. Both version are available from http://thehackbox.org/tasmota/release/

### Change in default initial configuration tool
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
52 Teckin         | Teckin SP20 Wifi Smart Switch with Energy Monitoring
53 AplicWDP303075 | Aplic WDP 303075 CSL Wifi Smart Switch with Energy Monitoring
54 Tuya Dimmer    | MIUO (and other Tuya based) Wifi Dimmer for Incandescent Lights and Led
55 Gosund SP1 v23 | Gosund SP1 v2.3 Wifi Smart Switch with Energy Monitoring
56 ARMTR Dimmer   | ARMtronix Wifi dimmer for Incandescent Lights and Led
57 SK03 Outdoor   | SK03 Outdoor Wifi Smart Switch with Energy Monitoring
58 PS-16-DZ       | PS-16-DZ  Wifi dimmer for Incandescent Lights and Led
59 Teckin US      | Teckin US and ZooZee SA102 Wifi Smart Switch with Energy Monitoring
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

### Available Features and Sensors

| Feature or Sensor     | minimal | basic | classic | sonoff | knx  | sensors | display | Remarks
|-----------------------|---------|-------|---------|--------|------|---------|---------|--------
| MY_LANGUAGE en-GB     | x | x | x | x | x | x | x |
| USE_WPS               | - | - | x | - | - | - | - | WPS
| USE_SMARTCONFIG       | - | - | x | - | - | - | - | SmartConfig
| USE_ARDUINO_OTA       | - | - | - | - | - | - | - |
| USE_DOMOTICZ          | - | - | x | x | x | x | - |
| USE_HOME_ASSISTANT    | - | - | - | x | x | x | - |
| USE_MQTT_TLS          | - | - | - | - | - | - | - |
| USE_KNX               | - | - | - | - | x | - | - |
| USE_WEBSERVER         | x | x | x | x | x | x | x | WifiManager
| USE_EMULATION         | - | x | x | x | - | x | - |
| USE_DISCOVERY         | - | - | x | x | x | x | x |
| WEBSERVER_ADVERTISE   | - | - | x | x | x | x | x |
| MQTT_HOST_DISCOVERY   | - | - | x | x | x | x | x |
| USE_TIMERS            | - | x | - | x | x | x | x |
| USE_TIMERS_WEB        | - | x | - | x | x | x | x |
| USE_SUNRISE           | - | x | - | x | x | x | x |
| USE_RULES             | - | x | - | x | x | x | x |
| USE_EXPRESSION        | - | - | - | - | - | - | - |
|                       |   |   |   |   |   |   |   |
| USE_ADC_VCC           | x | x | x | x | x | - | x |
| USE_DS18B20           | - | - | - | - | - | - | - | Single sensor
| USE_DS18x20           | - | - | x | x | x | x | x | Multiple sensors
| USE_DS18x20_LEGACY    | - | - | - | - | - | - | - | Multiple sensors
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
| USE_DISPLAY           | - | - | - | - | - | - | x |
| USE_DISPLAY_LCD       | - | - | - | - | - | - | x |
| USE_DISPLAY_SSD1306   | - | - | - | - | - | - | x |
| USE_DISPLAY_MATRIX    | - | - | - | - | - | - | x |
| USE_DISPLAY_ILI9341   | - | - | - | - | - | - | x |
| USE_DISPLAY_EPAPER_29 | - | - | - | - | - | - | x | Disabled for core 2.3.0

## Changelog
Version 6.5.0 20190319
 * Remove commands SetOption14 and SetOption63 as it has been superseded by command Interlock
 * Remove command SetOption35 0-255 for mDNS start-up delay (#4793)
 * Remove support for MQTT_LIBRARY_TYPE, MQTT_ARDUINOMQTT and MQTT_TASMOTAMQTT (#5474)
 * Change webserver content handling from single String to small Chunks increasing RAM
 * Change code use of boolean to bool and byte to uint8_t
 * Change code uint8_t flags to bool flags
 * Change sonoff_template.h layout regarding optional module flags like ADC0
 * Change sonoff_template.h module lay-out by removing non-configurable GPIOs
 * Change button driver making it modular
 * Change switch driver making it modular and introduce input filter (#4665, #4724)
 * Change switch input detection by optimizing switch debounce (#4724)
 * Change web authentication (#4865)
 * Change image name BE_MINIMAL to FIRMWARE_MINIMAL and USE_xyz to FIRMWARE_xyz (#5106)
 * Change GUI weblog from XML to plain text solving possible empty screens (#5154)
 * Fix most compiler warnings
 * Fix Display exception 28 when JSON value is NULL received
 * Fix epaper driver (#4785)
 * Fix HAss Sensor Discovery Software Watchdog restart (#4831, #4988)
 * Fix allowable MAX_RULE_VARS to 16 (#4933)
 * Fix mDNS addService (#4938, #4951)
 * Fix HAss discovery of MHZ19(B) sensors (#4992)
 * Fix some exceptions and watchdogs due to lack of stack space (#5215)
 * Fix GUI wifi password acception starting with asteriks (*) (#5231, #5242)
 * Fix command WebSend intermittent results (#5273, #5304)
 * Fix additional characters in fallbacktopic, hostname and mqttclient on core 2.5.0 (#5359, #5417)
 * Fix Energy TotalStartTime when commands EnergyReset0 and/or EnergyReset3 used (#5373)
 * Fix DS18S20 temperature calculation (#5375)
 * Fix float calculations in range from 0 to -1 (#5386)
 * Fix exception on GUI Configure Logging and Configure Other (#5424)
 * Add commands PowerCal, VoltageCal and CurrentCal for HLW8012, HJL01 and BL0937 based energy sensors
 * Add command SerialDelimiter 128 to filter reception of only characters between ASCII 32 and 127 (#5131)
 * Add command SSerialSend5 \<hexdata\> to SerialBridge
 * Add command Interlock 0 / 1 / 1,2 3,4 .. to control interlock ON/OFF and add up to 8 relays in 1 to 4 interlock groups (#4910, #5014)
 * Add command Template 255 to copy module configuration over to current active template and store as user template named Merged (#5371)
 * Add command WifiConfig 7 to allow reset of device in AP mode without admin password (#5297)
 * Add command SetOption36 to control boot loop default restoration (#4645, #5063)
 * Add command SetOption37 for RGBCW color mapping (#5326)
 * Add command SetOption55 0/1 and define MDNS_ENABLE to disable/enable mDNS (#4793, #4923)
 * Add command SetOption62 0/1 to disable retain on Button or Switch hold messages (#5299)
 * Add support for Smanergy KA10 Smart Wall Socket with Energy monitoring
 * Add support for commands in sensor drivers
 * Add support for MAX31855 K-Type thermocouple sensor using softSPI (#4764)
 * Add support for Near Field Communication (NFC) controller PN532 using Serial (#4791, #5162)
 * Add support for OBI Power Socket 2 (#4829)
 * Add support for YTF IR Bridge (#4855)
 * Add support for Mi LED Desk Lamp with rotary switch (#4887)
 * Add support for Digoo DG-SP202 Smart Socket with Energy monitoring (#4891)
 * Add support for MAX44009 Ambient Light sensor (#4907)
 * Add support for inverted buttons and inverted buttons without pullup (#4914)
 * Add support for Luminea ZX2820 Smart Socket with Energy monitoring (#4921)
 * Add support for multiple ADS1115 I2C devices (#5083)
 * Add support for online template change using command Template or GUI Configure Other (#5177)
 * Add support for Korean language translations (#5344)
 * Add support for sensor SCD30 (#5434)
 * Add parameter CFG_HOLDER to status 1 message (#5206)
 * Add SetOption32 until SetOption49 diagnostic information to Status 3 report as replacement for second property value in SetOption property name
 * Add Resolution property to Status 3 report providing previous SetOption second value property
 * Add property MqttCount to status 6 message representing number of Mqtt re-connections
 * Add property LinkCount to state and status 11 message representing number of Wifi Link re-connections
 * Add property Downtime to state and status 11 message representing the duration of wifi connection loss
 * Add variable %timestamp% to rules (#4749)
 * Add rule support for "==", "!=" ">=" and "<=" (#5122)
 * Add rule expression enabled by define USE_EXPRESSION in my_user_config.h (#5210)
 * Add Power status functionality to LED2 when configured leaving LED1 for Link status indication
 * Add user configuration of HLW8012 and HJL-01/BL0937 Energy Monitoring as used in Sonoff Pow and many Tuya based devices
 * Add user configuration of MCP39F501 Energy Monitoring as used in Shelly2
 * Add online template configuration using both commands and Configure Template menu option in GUI
 * Add (S)SerialSend3 escape sequence \x to allow hexadecimal byte value (#3560, #4947)
 * Add define DS18B20_INTERNAL_PULLUP to select internal input pullup when only one DS18B20 sensor is connected eliminating external resistor (#4738)
 * Add button control when no relay configured (#4682)
 * Add startup delay of 4 seconds to button control (#4829)
 * Add core version conditional compile options to provided PWM files (#4917)
 * Add resiliency to saved Settings (#5065)
 * Add MHZ19 Temperature as Domoticz Temperature selection (#5128)
 * Add HAss status sensor (#5139)
 * Add status message to former declined group commands (#5145)
 * Add 0x to IRRemote (SetOption29) and RCSwitch (SetOption28) received hexadecimal data (#5431)
