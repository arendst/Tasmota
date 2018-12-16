## Migration Information
See [wiki migration path](https://github.com/arendst/Sonoff-Tasmota/wiki/Upgrade#migration-path) for instructions how to migrate to a major version. Pay attention to the following version breaks due to dynamic settings updates:

1. Migrate to **Sonoff-Tasmota 3.9.x**
2. Migrate to **Sonoff-Tasmota 4.x**
3. Migrate to **Sonoff-Tasmota 5.14**
4. Migrate to **Sonoff-Tasmota 6.x**

## Release notes
### Change in default initial configuration tool
Firmware binary **sonoff-classic.bin** supports **WifiManager, Wps and SmartConfig** for initial configuration. The default tool is **Wps**.

To save memory space all other binaries support **WifiManager only**.

See _changelog.ino how to enable them again.
- Define WIFI_CONFIG_TOOL now contains the default behaviour once a SSID has been configured.
- If no SSID is configured making a wifi connection impossible the new define WIFI_CONFIG_NO_SSID will be used.
- While define WIFI_CONFIG_NO_SSID is set to WIFI_WPSCONFIG in my_user_config.h the compiler will check for define USE_WPS and if not enabled WIFI_CONFIG_NO_SSID will default to WIFI_MANAGER using the webserver. If define USE_WEBSERVER is also not enabled WIFI_CONFIG_NO_SSID will default to WIFI_SMARTCONFIG. If define USE_SMARTCONFIG is also not enabled WIFI_CONFIG_NO_SSID will default to a new option WIFI_SERIAL allowing to enter wifi parameters to serial which is always possible.

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

## Provided Binary Downloads
The following binary downloads have been compiled with ESP8266/Arduino library core version **2.4.2** patched with the Alexa fix.

- **sonoff-minimal.bin** = The Minimal version allows intermediate OTA uploads to support larger versions and does NOT change any persistent parameter. This version **should NOT be used for initial installation**.
- **sonoff-classic.bin** = The Classic version allows **initial installation** using either WifiManager, Wps or SmartConfig.
- **sonoff.bin** = The Sonoff version without Wps and SmartConfig configuration but adds more sensors.
- **sonoff-BG.bin** to **sonoff-TW.bin** = The Sonoff version without Wps and SmartConfig configuration in different languages.
- **sonoff-sensors.bin** = The Sensors version without Wps and SmartConfig configuration but adds even more useful sensors.
- **sonoff-display.bin** = The Display version without Wps and SmartConfig configuration but adds display support.
- **sonoff-knx.bin** = The Knx version without Wps and SmartConfig configuration and some other features but adds KNX support.

### Available Features and Sensors

| Feature or Sensor              | minimal | basic | classic | sonoff | knx  | sensors | Remarks
|--------------------------------|---------|-------|---------|--------|------|---------|--------
| ESP/Arduino lib v2.3.0         | 346k    | 429k  | 482k    | 504k   | 522k | 535k    |
| ESP/Arduino lib v2.4.2         | 366k    | 446k  | 496k    | 522k   | 538k | 551k    | No wifi sleep
|                                |   |   |   |   |   |   |
| MY_LANGUAGE en-GB              | x | x | x | x | x | x |
| MQTT_LIBRARY_TYPE PUBSUBCLIENT | x | x | x | x | x | x |
| USE_WPS                        | - | - | x | - | - | - | WPS
| USE_SMARTCONFIG                | - | - | x | - | - | - | SmartConfig
| USE_ARDUINO_OTA                | - | - | - | - | - | - |
| USE_DOMOTICZ                   | - | - | x | x | x | x |
| USE_HOME_ASSISTANT             | - | - | - | x | x | x |
| USE_MQTT_TLS                   | - | - | - | - | - | - |
| USE_KNX                        | - | - | - | - | x | - |
| USE_WEBSERVER                  | x | x | x | x | x | x | WifiManager
| USE_EMULATION                  | - | x | x | x | - | x |
| USE_DISCOVERY                  | - | - | x | x | x | x |
| WEBSERVER_ADVERTISE            | - | - | x | x | x | x |
| MQTT_HOST_DISCOVERY            | - | - | x | x | x | x |
| USE_TIMERS                     | - | x | - | x | x | x |
| USE_TIMERS_WEB                 | - | x | - | x | x | x |
| USE_SUNRISE                    | - | x | - | x | x | x |
| USE_RULES                      | - | x | - | x | x | x |
|                                |   |   |   |   |   |
| USE_ADC_VCC                    | x | x | x | x | x | - |
| USE_DS18B20                    | - | - | - | - | - | - | Single sensor
| USE_DS18x20                    | - | - | x | x | x | x | Multiple sensors
| USE_DS18x20_LEGACY             | - | - | - | - | - | - | Multiple sensors
|                                |   |   |   |   |   |   |
| Feature or Sensor              | minimal | basic | classic | sonoff | knx  | sensors |
| USE_I2C                        | - | - | - | x | x | x |
| USE_SHT                        | - | - | - | x | x | x |
| USE_HTU                        | - | - | - | x | x | x |
| USE_BMP                        | - | - | - | x | x | x |
| USE_BME680                     | - | - | - | - | - | x |
| USE_BH1750                     | - | - | - | x | x | x |
| USE_VEML6070                   | - | - | - | - | - | x |
| USE_ADS1115                    | - | - | - | - | - | x |
| USE_ADS1115_I2CDEV             | - | - | - | - | - | - |
| USE_INA219                     | - | - | - | - | - | x |
| USE_SHT3X                      | - | - | - | x | x | x |
| USE_TSL2561                    | - | - | - | - | - | x |
| USE_MGS                        | - | - | - | - | - | x |
| USE_SGP30                      | - | - | - | x | x | x |
| USE_SI1145                     | - | - | - | - | - | x |
| USE_LM75AD                     | - | - | - | x | x | x |
| USE_APDS9960                   | - | - | - | - | - | - |
| USE_MCP230xx                   | - | - | - | - | - | - |
| USE_PCA9685                    | - | - | - | - | - | - |
| USE_MPR121                     | - | - | - | - | - | - |
| USE_CCS811                     | - | - | - | - | - | - |
| USE_MPU6050                    | - | - | - | - | - | - |
| USE_DS3231                     | - | - | - | - | - | - |
| USE_MGC3130                    | - | - | - | - | - | - |
|                                |   |   |   |   |   |   |
| Feature or Sensor              | minimal | basic | classic | sonoff | knx  | sensors |
| USE_SPI                        | - | - | - | - | - | - |
| USE_MHZ19                      | - | - | - | x | x | x |
| USE_SENSEAIR                   | - | - | - | x | x | x |
| USE_PMS5003                    | - | - | - | x | x | x |
| USE_NOVA_SDS                   | - | - | - | x | x | x |
| USE_PZEM004T                   | - | - | - | x | x | x |
| USE_PZEM_AC                    | - | - | - | x | x | x |
| USE_PZEM_DC                    | - | - | - | x | x | x |
| USE_MCP39F501                  | - | x | - | x | x | x |
| USE_SERIAL_BRIDGE              | - | - | - | x | x | x |
| USE_SDM120                     | - | - | - | - | - | x |
| USE_SDM630                     | - | - | - | - | - | x |
| USE_MP3_PLAYER                 | - | - | - | - | - | x |
| USE_TUYA_DIMMER                | - | x | - | x | x | x |
| USE_ARMTRONIX_DIMMERS          | - | x | - | x | x | x |
| USE_PS_16_DZ                   | - | x | - | x | x | x |
| USE_IR_REMOTE                  | - | - | - | x | x | x |
| USE_IR_HVAC                    | - | - | - | - | - | x |
| USE_IR_RECEIVE                 | - | - | - | x | x | x |
| USE_WS2812                     | - | - | x | x | x | x |
| USE_WS2812_DMA                 | - | - | - | - | - | - |
| USE_ARILUX_RF                  | - | - | - | x | x | x |
| USE_SR04                       | - | - | - | x | x | x |
| USE_TM1638                     | - | - | - | - | - | x |
| USE_HX711                      | - | - | - | x | x | x |
| USE_RF_FLASH                   | - | - | - | x | x | x |
| USE_TX20_WIND_SENSOR           | - | - | - | x | x | x |
| USE_RC_SWITCH                  | - | - | - | x | x | x |
| USE_RF_SENSOR                  | - | - | - | - | - | x | AlectoV2 only
| USE_DISPLAY                    | - | - | - | - | - | - |

## Changelog
Version 6.4.0 20181217
 * Change GUI Configure Module by using AJAX for data fetch to cut page size (and memory use) by 40%.
     In case of web page errors clear your browser cache or do Page Reload (F5 or Ctrl+R)
 * Change enforcing flashmode dout but it is still mandatory
 * Change bootcount update (being first) flash write to 10 seconds after restart
 * Change display and epaper drivers
 * Change command WebSend Host header field from IP address to hostname (#4331)
 * Change log buffer size from 512 to 520 to accommodate http sensor data (#4354)
 * Change default WIFI_CONFIG_TOOL from WIFI_WAIT to WIFI_RETRY in my_user_config.h (#4400)
 * Change webgui refresh time delay for Save Settings and local OTA Upload (#4423)
 * Change SR-04 driver to use NewPing library (#4488)
 * Change MCP230xx driver to support interrupt retention over teleperiod (#4547)
 * Change support for MPU6050 using DMP (#4581)
 * Fix unintended function overload of WifiState
 * Fix wifi connection errors using wifi disconnect and ESP.reset instead of ESP.restart
 * Fix Sonoff Pow R2 and Sonoff S31 Serial interface hang caused by Sonoff Basic R2 driver delay implementation (and possibly core bug)
 * Fix MQTT connection error after restart
 * Fix wifi re-scan connection baseline
 * Fix possible strncat buffer overflows
 * Fix intermittent Pzem sensor energy overflow calculation error
 * Fix shelly2 ghost switching caused by lack of pull-up inputs (#4255)
 * Fix hardware serial pin configuration. To keep using hardware serial swap current Rx/Tx pin configuration only (#4280)
 * Fix MqttRetry values above 255 seconds (#4424)
 * Fix WifiManager functionality on initial installation (#4433)
 * Fix ArduinoOTA for Core 2.5.0 (#4620)
 * Add minutes to commands Timezone to allow all possible world timezones
 * Add more strict checks for GPIO selections
 * Add code image and optional commit number to version
 * Add dynamic delay to main loop providing time for wifi background tasks
 * Add additional start-up delay during initial wifi connection
 * Add support for decoding Theo V2 sensors as documented on https://sidweb.nl using 434MHz RF sensor receiver
 * Add support for decoding Alecto V2 sensors like ACH2010, WS3000 and DKW2012 weather stations using 868MHz RF sensor receiver
 * Add user definition of defines WIFI_RSSI_THRESHOLD (default 10) and WIFI_RESCAN_MINUTES (default 44)
 * Add command SetOption58 0/1 to enable IR raw data info in JSON message (#2116)
 * Add command IRSend \<frequency\>|0,\<rawdata1\>,\<rawdata2\>,.. to allow raw data transmission (#2116)
 * Add command SetOption56 0/1 to enable wifi network scan and select highest RSSI (#3173)
 * Add command SetOption57 0/1 to enable wifi network re-scan every 44 minutes with a rssi threshold of 10 to select highest RSSI (#3173)
 * Add support for SDM220 (#3610)
 * Add default sleep 1 to sonoff-basic to lower energy consumption (#4217)
 * Add wifi status to Tuya (#4221)
 * Add delays to reduce CPU usage at boot time (#4233)
 * Add command SetOption24 0/1 to select pressure unit as hPa or mmHg (#4241)
 * Add optional hardware serial when GPIO13(Rx) and GPIO15(Tx) are selected removing hardware serial from GPIO01(Tx) and GPIO03(Rx) (#4288)
 * Add support for Gosund SP1 v2.3 Power Socket with Energy Monitoring (#4297)
 * Add support for Armtronix dimmers. See wiki for info (#4321)
 * Add to command WebSend option to send a direct path when command starts with a slash (#4329)
 * Add support for LG HVac and IrRemote (#4377)
 * Add initial support for Hass sensor discovery (#4380)
 * Add support for Fujitsu HVac and IrRemote (#4387)
 * Add support for I2C MGC3130 Electric Field Effect sensor by Christian Baars (#3774, #4404)
 * Add command CalcRes to set number of decimals (0 - 7) used in commands ADD, SUB, MULT and SCALE (#4420)
 * Add CPU average load to state message (#4431)
 * Add command SetOption59 0/1 to change state topic from tele/STATE to stat/RESULT (#4450)
 * Add support for SM Smart Wifi Dimmer PS-16-DZ (#4465)
 * Add support for Teckin US Power Socket with Energy Monitoring (#4481)
 * Add command SetOption60 0/1 to select dynamic sleep (0) or sleep (1) (#4497)
 * Add support for iFan02 Fanspeed in Domoticz using a selector (#4517)
 * Add support for GPIO02 for newer Sonoff Basic (#4518)
 * Add Announce Switches to MQTT Discovery (#4531)
 * Add support for Manzoku Power Strip (#4590)
