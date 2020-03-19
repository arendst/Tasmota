## Unreleased (development)

## Released

### 8.2.0 20200321

- Release

### 8.1.0.11 20200313

- Change Zigbee simplification of devices probing, saving Flash and memory
- Add HAss Discovery support for Button and Switch triggers by Federico Leoni (#7901)
- Add support for HDC1080 Temperature and Humidity sensor by Luis Teixeira (#7888)
- Add commands ``SwitchMode 13`` PushOn and ``SwitchMode 14`` PushOnInverted (#7912)
- Add command ``HumOffset -10.0 .. 10.0`` to set global humidity sensor offset (#7934)
- Add Zigbee support for Hue emulation by Stefan Hadinger
- Add Dew Point to Temperature and Humidity sensors
- Add support for ElectriQ iQ-wifiMOODL RGBW light by Ian King (#7947)

### 8.1.0.10 20200227

- Change default my_user_config.h driver and sensor support removing most sensors and adding most drivers
- Change IRremoteESP8266 library updated to v2.7.4
- Revert switchmode 6 according to issue 7778 (#7831)
- Add support for Jarolift rollers by Keeloq algorithm
- Add Zigbee features and improvements and remove support for Zigbee commands starting with ``Zigbee...``
- Add support for MaxBotix HRXL-MaxSonar ultrasonic range finders by Jon Little (#7814)
- Add support for Romanian language translations by Augustin Marti
- Add support for La Crosse TX23 Anemometer by Norbert Richter (#3146, #7765)
- Add command ``SetOption89 0/1`` for Zigbee distinct MQTT topics per device for SENSOR, allowing retained messages (#7835)
- Change Hue emulation code optimization

### 8.1.0.9 20200220

- Revert most wifi connectivity changes introduced in 8.1.0.5 (#7746, #7602, #7621)
- Fix Zigbee auto-increment transaction number (#7757)
- Add initial support for Sensors AHT10 and AHT15 by Martin Wagner (#7596)
- Add support for Wemos Motor Shield V1 by Denis Sborets (#7764)
- Add Zigbee enhanced commands decoding, added ``ZbPing``
- Add commands ``SetOption85 0/1`` and ``DevGroupShare`` supporting UDP Group command using ``GroupTopic`` without MQTT by Paul Diem (#7790)
- Add support for Martin Jerry/acenx/Tessan/NTONPOWER SD0x PWM dimmer switches by Paul Diem (#7791)
- Add command ``SetOption86 0/1`` for PWM dimmer to turn brightness LED's off 5 seconds after last change
- Add command ``SetOption87 0/1`` for PWM dimmer to turn red LED on when powered off
- Add command ``SetOption88 0/1`` for PWM dimmer to let buttons control remote devices

### 8.1.0.8 20200212

- Change MQTT message size with additional 200 characters
- Change some wifi code to attempt faster connection (#7621)
- Change display of some date and time messages from "Wed Feb 19 10:45:12 2020" to "2020-02-19T10:45:12"
- Fix relation between RSSI and signal strength
- Add another new DHT driver based on ESPEasy. The old driver can still be used using define USE_DHT_OLD. The previous new driver can be used with define USE_DHT_V2 (#7717)

### 8.1.0.7 20200210

- Add new DHT driver. The old driver can still be used using define USE_DHT_OLD (#7468)
- Fix wrong encoding of Zigbee persistent data

### 8.1.0.6 20200205

- Fix Hass sensor discovery part 1/4 by Federico Leoni (#7582, #7548)
- Fix MaxPower functionality (#7647)
- Add support for sensors DS18x20 and DHT family on Shelly 1 and Shelly 1PM using Shelly Add-On adapter (#7469)
- Add commands ``SwitchMode 11`` PushHoldMulti and ``SwitchMode 12`` PushHoldInverted (#7603)
- Add command ``Buzzer -1`` for infinite mode and command ``Buzzer -2`` for following led mode (#7623)
- Add support for MI-BLE sensors using HM-10 Bluetooth 4.0 module by Christian Staars (#7683)
- Add BootCount Reset Time as BCResetTime to ``Status 1``
- Add ``ZbZNPReceived``and ``ZbZCLReceived`` being published to MQTT when ``SetOption66 1``
- Add optional Wifi AccessPoint passphrase define WIFI_AP_PASSPHRASE in my_user_config.h (#7690)
- Add support for FiF LE-01MR energy meter by saper-2 (#7584)

### 8.1.0.5 20200126

- Change wifi connectivity stability (#7602)
- Change IRremoteESP8266 library updated to v2.7.3
- Fix PWM flickering at low levels (#7415)
- Add ``SetOption84 0/1`` sends AWS IoT device shadow updates (alternative to retained)
- Add ``ZbBind`` (experimental) and bug fixes

### 8.1.0.4 20200116

- Change Zigbee command prefix from ``Zigbee*`` to ``Zb*``
- Fix ``PowerDelta`` zero power detection (#7515)
- Fix OTA minimal gzipped detection regression from 8.1.0.3
- Fix ``RGBWWTable`` ignored (#7572)
- Add web page sliders when ``SetOption37 128`` is active allowing control of white(s)
- Add Zigbee persistence and friendly names
- Add most SetOptions as defines to my_user_config.h
- Add SoftwareSerial to CSE7766 driver allowing different GPIOs (#7563)
- Add optional parameter ``<startcolor>`` to command ``Scheme <scheme>, <startcolor>`` to control initial start color
- Add rule trigger on one level deeper using syntax with two ``#`` like ``on zigbeereceived#vibration_sensor#aqaracubeside=0 do ...``

### 8.1.0.3 20200106

- Change commands ``Prefix``, ``Ssid``, ``StateText``, ``NTPServer``, and ``FriendlyName`` displaying all items
- Change IRremoteESP8266 library updated to v2.7.2
- Fix ``WakeUp <x>`` ignores provided value (#7473)
- Fix exception 9 restart on log message in Ticker interrupt service routines NTP, Wemos and Hue emulation (#7496)
- Add support for gzipped binaries
- Add ``SwitchMode 8`` ToggleMulti, ``SwitchMode 9`` FollowMulti and ``SwitchMode 10`` FollowMultiInverted (#7522)

### 8.1.0.2 20191230

- Fix LCD line and column positioning (#7387)
- Fix Display handling of hexadecimal escape characters (#7387)
- Fix Improved fade linearity with gamma correction
- Fix wrong gamma correction for Module 48 lights (PWM5 for CT)
- Add support for ``AdcParam`` parameters to control ADC0 Current Transformer Apparent Power formula by Jodi Dillon (#7100)
- Add optional support for Prometheus using file xsns_91_prometheus.ino (#7216)
- Add command ``ShutterButton <parameters>`` to control shutter(s) by to-scho (#7403)
- Add command ``SetOption82 0/1`` to limit the CT range for Alexa to 200..380
- Add experimental support for NRF24L01 as BLE-bridge for Mijia Bluetooth sensors by Christian Baars (#7394)
- Add support to BMP driver to enter reset state (sleep enable) when deep sleep is used in Tasmota

### 8.1.0.1 20191225

- Change Lights: simplified gamma correction and 10 bits internal computation
- Fix Sonoff Bridge, Sc, L1, iFan03 and CSE7766 serial interface to forced speed, config and disable logging
- Fix Serial initialization regression from previous fix
- Fix commands ``Display`` and ``Counter`` from overruling command processing (#7322)
- Fix ``White`` added to light status (#7142)
- Add command ``SetOption79 0/1`` to enable reset of counters at teleperiod time by Andre Thomas (#7355)
- Add SerialConfig to ``Status 1``
- Add WifiPower to ``Status 5``
- Add support for DS1624, DS1621 Temperature sensor by Leonid Myravjev
- Add Zigbee attribute decoder for Xiaomi Aqara Cube

### 8.1.0 20191225

- Release

### 8.0.0.3 20191224

- Version bump due to internal Settings change

### 8.0.0.2 20191223

- Changed Settings variable namings
- Change number of ``FriendlyName``s from 4 to 8
- Add Zigbee better support for Xiaomi Double Switch and Xiaomi Vibration sensor
- Add support for ``AdcParam`` parameters to control ADC0 Moisture formula by Federico Leoni (#7309)
- Add commands ``WebButton1`` until ``WebButton16`` to support user defined GUI button text (#7166)

### 8.0.0.1 20191221

- Change Settings text handling allowing variable length text within a total text pool of 699 characters
- Change Smoother ``Fade`` using 100Hz instead of 20Hz animation (#7179)
- Change number of rule ``Var``s and ``Mem``s from 5 to 16 (#4933)
- Add support for max 150 characters in most command parameter strings (#3686, #4754)
- Add support for GPS as NTP server by Christian Baars and Adrian Scillato
- Add Zigbee coalesce sensor attributes into a single message
- Add Deepsleep start delay based on Teleperiod if ``Teleperiod`` differs from 10 or 300

### 7.2.0 20191221

- Release
- Change basic version string to lite (#7291)
- Fix Arduino IDE compile error (#7277)
- Fix restore ShutterAccuracy, MqttLog, WifiConfig, WifiPower and SerialConfig (#7281)
- Fix no AP on initial install (#7282)
- Fix failing downgrade (#7285)

### 7.1.2.6 20191214

- Change some more Settings locations freeing up space for future single char allowing variable length text
- Change tasmota-basic.bin and FIRMWARE_BASIC to tasmota-lite.bin and FIRMWARE_LITE
- Fix DeepSleep in case there is no wifi by Stefan Bode (#7213)
- Fix Fade would ignore ``savedata 0`` and store to flash anyways (#7262)
- Add Zigbee send automatic ZigbeeRead after sending a command
- Add Zigbee improving Occupancy:false detection for Aqara sensor
- Add fallback support from version 8.x
- Add restriction if fallback firmware is incompatible with settings resulting in unreachable device
- Add support for DHT12 Temperature and Humidity sensor by Stefan Oskamp

### 7.1.2.5 20191213

- Change some Settings locations freeing up space for future single char allowing variable length text
- Add Zigbee support for Xiaomi Aqara Vibration Sensor and Presence Sensor by Stefan Hadinger
- Add Shutter functions ramp up/down and MQTT reporting by Stefan Bode

### 7.1.2.4 20191209

- Change HTTP CORS from command ``SetOption73 0/1`` to ``Cors <cors_domain>`` allowing user control of specific CORS domain by Shantur Rathore (#7066)
- Change GUI Shutter button text to Up and Down Arrows based on PR by Xavier Muller (#7166)
- Change amount of supported DHT sensors from 3 to 4 by Xavier Muller (#7167)
- Revert removal of exception details from MQTT info on restart
- Add Wifi Signal Strength in dBm in addition to RSSI Wifi Experience by Andreas Schultz (#7145)
- Add Yaw, Pitch and Roll support for MPU6050 by Philip Barclay (#7058)
- Add reporting of raw weight to JSON from HX711 to overcome auto-tare functionality by @tobox (#7171)
- Add command ``Sensor34 9 <weight code>`` to set minimum delta to trigger JSON message by @tobox (#7188)
- Fix flashing H801 led at boot by Stefan Hadinger (#7165, #649)
- Fix duplicated ``Backlog`` when using Event inside a Backlog by Adrian Scillato (#7178, #7147)
- Fix Gui Timer when using a negative zero offset of -00:00 by Peter Ooms (#7174)

### 7.1.2.3 20191208

- Change Exception reporting removing exception details from both MQTT info and ``Status 1``. Now consolidated in ``Status 12`` if available.

### 7.1.2.2 20191206

- Remove rule trigger ``tele_power1#state`` due to compatibility
- Add command ``SerialConfig 0..23`` or ``SerialConfig 8N1`` to select Serial Config based in PR by Luis Teixeira (#7108)
- Add save call stack in RTC memory in case of crash, command ``Status 12`` to dump the stack by Stefan Hadinger
- Add Home Assistant force update by Frederico Leoni (#7140, #7074)

### 7.1.2.1 20191206

- Add SML bus decoder syntax support for byte order by Gerhard Mutz (#7112)
- Add rule var ``%topic%`` by Adrian Scillato (#5522)
- Add rule triggers ``tele_power1#state`` and multiple ``tele-wifi1#xxx`` by Adrian Scillato (#7093)
- Add experimental support for stepper motor shutter control by Stefan Bode
- Add optional USE_MQTT_TLS to tasmota-minimal.bin by Bohdan Kmit (#7115)

### 7.1.2 20191206

- Maintenance Release

### 7.1.1.1 20191201

- Fix lost functionality of GPIO9 and GPIO10 on some devices (#7080)
- Fix Zigbee uses Hardware Serial if GPIO 1/3 or GPIO 13/15 and SerialLog 0 (#7071)
- Fix WS2812 power control (#7090)
- Change light color schemes 2, 3 and 4 from color wheel to Hue driven with user Saturation control
- Change log buffer size from 520 to 700 characters accomodating full rule text (#7110)

### 7.1.1 20191201

- Maintenance Release

### 7.1.0.1 20191130

- Fix slider for devices with one or two channels like only white or white/yellow
- Fix TasmotaSlave buffer overrun on Tele
- Fix light scheme 4 speed (#7072)
- Add support for TasmotaSlave executing commands on Tasmota

### 7.1.0 20191129

- Release

### 7.0.0.6 20191122

- Add colorpicker to WebUI by Christian Staars (#6984)
- Change new Fade system much smoother, Speed now up to 40 (#6942, #3714)
- Fix Arduino IDE function prototyping compile error (#6982)
- Change update lib IRremoteESP8266 updated to v2.7.1, -2.7k flash and -1.5k RAM for Tasmota-IR
- Fix auto--power on/off when setting channel to non-zero or zero value, when SetOption68 1
- Fix postpone saving settings to flash until Fade is complete, avoids pause in Fade
- Add command ``SetOption77 0/1`` to keep power on when slider is far left

### 7.0.0.5 20191118

- Fix boot loop regression
- Add command ``TempOffset -12.6 .. 12.6`` to set global temperature sensor offset (#6958)
- Fix check deepsleep for valid values in Settings (#6961)
- Fix Wifi instability when light is on, due to sleep=0 (#6961, #6608)
- Add hardware detection to be overruled with ``SetOption51`` (#6969)

### 7.0.0.4 20191108

- Add command ``WifiPower 0 .. 20.5`` to set Wifi Output Power which will be default set to 17dBm
- Change supported PCF8574 I2C address range to 0x20 - 0x26 allowing other I2C devices with address 0x27 to be used at the same time
- Change supported PCF8574A I2C address range to 0x39 - 0x3F allowing other I2C devices with address 0x38 to be used at the same time
- Change supported MCP230xx I2C address range to 0x20 - 0x26 allowing other I2C devices with address 0x27 to be used at the same time
- Add Keep last channels values when Color command end with '=' (#6799)
- Add support for I2C sensor TLS2591 Light Intensity sensor (#6873)
- Change Kept only NEC/RC5/RC6/HASH IR protocols in standard Tasmota, all other protocols require Tasmota-IR, saving 4K
- Add command ``SetOption76 0/1`` to enable incrementing bootcount when deepsleep is enabled (#6930)
- Change Reset erase end address from as seen by SDK (getFlashChipSize) to full flash size (getFlashChipRealSize)
- Change Zigbee log verbosity reduction

### 7.0.0.3 20191103

- Add command ``I2cDriver`` for I2C driver runtime control using document I2CDEVICES.md
- Fix random crash caused by UPNP flood
- Add support for Honeywell HPMA115S0 particle concentration sensor by David Hunt (#6843)
- Remove driver xsns_12_ads1115_i2cdev replaced by xsns_12_ads1115

### 7.0.0.2 20191102

- Add command ``WebColor19`` to control color of Module and Name (#6811)
- Add support for Honeywell I2C HIH series Humidity and Temperetaure sensor (#6808)
- Fix wrong Dimmer behavior introduced with #6799 when ``SetOption37`` < 128
- Change add DS18x20 support in Tasmota-IR
- Add Zigbee command support, considered as v1.0 for full Zigbee support
- Fix Reduce flash size after change to IRremoteESP8266 v2.7.0

### 7.0.0.1 20191027

- Remove update support for versions before 6.0
- Change default GUI to dark theme
- Add command ``SetOption73 0/1`` to re-enable HTTP Cross-Origin Resource Sharing (CORS) now default disabled (#6767)
- Add frequency to ADE7953 energy monitor as used in Shelly 2.5 by ljakob (#6778)
- Add command ``SetOption74 0/1`` to enable DS18x20 internal pull-up and remove define DS18B20_INTERNAL_PULLUP (#6795)
- Fix better control of RGB/White when ``SetOption37`` >128, added ``Dimmer1`` and ``Dimmer2`` commands (#6714)
- Add hide Alexa objects with friendlyname starting with '$' (#6722, #6762)
- Add command ``SetOption75 0/1`` to switch between grouptopic (0) using fulltopic replacing %topic% or (1) is cmnd/\<grouptopic\> (#6779)
- Change IRremoteESP8266 library to v2.7.0

### 6.7.1.1 20191026

- Change ArduinoSlave to TasmotaSlave
- Add support for Tuya battery powered devices (#6735)
- Change repository name from Sonoff-Tasmota to Tasmota and all code references from Sonoff to Tasmota

### 6.7.1 20191026

- Release
- Fix on energy monitoring devices using PowerDelta Exception0 with epc1:0x4000dce5 = Divide by zero (#6750)
- Fix Script array bug (#6751)

### 6.7.0 20191025

- Release

### 6.6.0.21 20191022

- Remove support for WPS and SmartConfig in favour of Web server (!) based WifiManager (#6680)
- Remove binary sonoff-classic (#6680)
- Remove command ``SetOption2``

### 6.6.0.20 20191018

- Add command ``SetOption65 0/1`` to disable (1) fast power cycle detection fixing unwanted brownout trigger
- Add absolute PowerDelta using command ``PowerDelta 101..32000`` where 101 = 101-100 = 1W, 202 = 202-100 = 102W (#5901)
- Add support for EX-Store WiFi Dimmer V4 (#5856)
- Add ``ZigbeeRead`` command and many improvements (#6095)
- Add ArduinoSlave driver (EXPERIMENTAL)

### 6.6.0.19 20191018

- Replace obsolete xsns_23_sdm120 with xnrg_08_sdm120 and consolidate define USE_SDM120
- Replace obsolete xsns_25_sdm630 with xnrg_10_sdm630 and consolidate define USE_SDM630
- Replace obsolete xsns_49_solaxX1 with xnrg_12_solaxX1 (#6677)

### 6.6.0.18 20191010

- Add command ``DimmerRange`` in Light module to support 2 byte dimming ranges from Tuya
- Add Zigbee additional commands and sending messages to control devices (#6095)
- Fix Rules were not triggered with IR unknown protocol or in sonoff-it (#6629)
- Add define USE_DEEPSLEEP and command ``DeepSleepTime 0 or 10..86400`` (seconds) to enter deepsleep mode (#6638)
- Add define USE_SONOFF_RF to enable/disable Sonoff Rf support (#6648)
- Add incremental beeps to Ifan03 remote control fan speed buttons (#6636)
- Add rule support after every command execution like Fanspeed#Data=2 (#6636)
- Fix handling of ligth channels when pwm_multichannel (Option68) is enabled
- Add WebUI for multiple, independent PWM channels
- Remove default DS18B20 driver and only support define DS18x20 (#6647)
- Add support for PMS3003 dust particle sensor
- Change Sonoff L1 support by adding define USE_SONOFF_L1

### 6.6.0.17 20191009

- Add command ``SetOption34 0..255`` to set backlog delay. Default value is 200 (mSeconds) (#6562)
- Add command ``Gpio 255`` to show physical GPIO configuration of all non-flash pins (#6407)

### 6.6.0.16 20191008

- Change PZEM004T default address mask from 0.0.0.x to 192.168.1.x for legacy reason (#6585)
- Fix PZEM004T, PZEMAC and PZEMDC autodetection (#6585)
- Change light drivers internals to ease management

### 6.6.0.15 20191003

- Change command ``PulseTime`` JSON message format and allow display of all pulsetimer information (#6519)
- Add support for Chint DDSU666 Modbus energy meter by Pablo Zerón
- Add support for SM2135 as used in Action LSC Smart Led E14 (#6495)
- Add command ``SetOption72 0/1`` to switch between software (0) or hardware (1) energy total counter (#6561)
- Add Zigbee tracking of connected devices and auto-probing of Manuf/Model Ids
- Fix better handling of PWM White Temperature mode for Module 48 (#6534)

### 6.6.0.14 20190925

- Change command ``Tariffx`` to allow time entries like 23 (hours), 1320 (minutes) or 23:00. NOTE: As this is development branch previous tariffs are lost! (#6488)
- Remove support for define USE_DS18x20_LEGACY and legacy DS18x20 driver (#6486)
- Add initial support for MQTT logging using command ``MqttLog <loglevel>`` (#6498)
- Add Zigbee more support - collect endpoints and clusters, added ZigbeeDump command
- Add initial support for shutters by Stefan Bode (#288)
- Add command to MCP230xx: ``sensor29 pin,0/1/2`` for OFF/ON/TOGGLE
- Add initial support for PCF8574 I2C I/O Expander (currently output only) by Stefan Bode
- Add command ``SetOption71 0/1`` to switch between different Modbus Active Energy registers on DDS238-2 energy meters (#6531)
- Change command ``SetOption43`` to make it more general. Now supports PS_16_DZ driver too (#6544)
- Change command handling by moving buffers up in chain solving MQTTlog support (#6529)
- Change detection of non-MQTT commands by allowing non-space characters as delimiter (#6540)
- Fix TasmotaSerial: move serial send to IRAM for high speed baud rates

### 6.6.0.13 20190922

- Add command ``EnergyReset4 x,x`` to initialize total usage for two tarrifs
- Add command ``EnergyReset5 x,x`` to initialize total export (or production) for two tarrifs
- Add command ``Sensor34 8,0`` and ``Sensor34 8,1`` to disable/enable JSON message on weight change over 4 gram
- Add JSON array index support to rules evaluation allowing trigger on ENERGY#POWER[2]>0.60 from JSON ..,"Power":[0.00,0.68],.. (#6160)

### 6.6.0.12 20190910

- Redesign command ``Tariff`` to now default to 0 (=disabled) and allowing to set both Standard Time (ST) and Daylight Savings Time (DST) start hour
-  Commands ``Tariff1 22,23`` = Tariff1 (Off-Peak) ST,DST   Tariff2 (Standard) 6,7 = Tariff2 ST,DST   Tariff9 0/1 = Weekend toggle (1 = Off-Peak during weekend)
- Change rename "Data" to "Hash" and limit to 32 bits when receiving UNKNOWN IR protocol (see DECODE_HASH from IRremoteESP8266)
- Add command ``Gpios 255/All`` to show all available GPIO components (#6407)
- Change JSON output format for commands ``Adc``, ``Adcs``, ``Modules``, ``Gpio`` and ``Gpios`` from list to dictionary (#6407)
- Add Zigbee support phase 3 - support for Xiaomi lumi.weather air quality sensor, Osram mini-switch
- Change energy sensors for three phase/channel support
- Add support for Shelly 2.5 dual energy (#6160)
- Add initial support for up to three PZEM-014/-016 on serial modbus connection with addresses 1 (default), 2 and 3 (#2315)
- Add initial support for up to three PZEM-004T on serial connection with addresses x.x.x.1 (default), 2 and 3 (#2315)
- Add initial support for up to three PZEM-003/-017 on serial modbus connection with addresses 1 (default), 2 and 3 (#2315)
- Add driver USE_SDM630_2 as future replacement for USE_SDM630 - Pls test and report
- Add command ``ModuleAddress 1/2/3`` to set Pzem module address when a single module is connected (#2315)

### 6.6.0.11 20190907

- Change Settings crc calculation allowing short term backward compatibility
- Add support for up to 4 INA226 Voltage and Current sensors by Steve Rogers (#6342)
- Change Improve reliability of TasmotaSerial at 115200 bauds and reduce IRAM usage for Stage/pre-2.6
- Add support for A4988 stepper-motor-driver-circuit by Tim Leuschner (#6370)
- Add support for Hiking DDS238-2 Modbus energy meter by Matteo Campanella (#6384)

### 6.6.0.10 20190905

- Redesign Tuya support by Shantur Rathore removing commands SetOption34, 41, 44, 45, 46 and 65 (#6353)
- Add command Reset 99 to reset bootcount to zero (#684, #6351)
- Change command Time 1/2/3 to select JSON time format ISO, ISO + Epoch or Epoch for legacy reason

### 6.6.0.9 20190828

- Change theoretical baudrate range to 300..19660500 bps in 300 increments (#6294)
- Add Full support of all protocols in IRremoteESP8266, to be used on dedicated-IR Tasmota version. Warning: +81k Flash when compiling with USE_IR_REMOTE_FULL
- Add compile time define USE_WS2812_HARDWARE to select hardware type WS2812, WS2812X, WS2813, SK6812, LC8812 or APA106 (DMA mode only)
- Add 'sonoff-ir' pre-packaged IR-dedicated firmware and 'sonoff-ircustom' to customize firmware with IR Full protocol support
- Add Zigbee support phase 2 - cc2530 initialization and basic ZCL decoding
- Add driver USE_SDM120_2 with Domoticz P1 Smart Meter functionality as future replacement for USE_SDM120 - Pls test and report
- Add command Power0 0/1/2/Off/On/Toggle to control all power outputs at once (#6340)
- Add time to more events (#6337)
- Add command Time 1/2/3 to select JSON time format ISO + Epoch, ISO or Epoch

### 6.6.0.8 20190827

- Add Tuya Energy monitoring by Shantur Rathore
- Add phase 1 Domoticz P1 Smart Meter support using energy sensors handled by xdrv_03_energy.ino based on an idea by pablozg
-   Add commands Tariff1 0..23 (start Off-Peak hour), Tariff2 0..23 (start Standard hour) and Tariff3 0/1 (Saturday and Sunday Off-Peak)

### 6.6.0.7 20190825

- Expand Settings area to 4k for future use

### 6.6.0.6 20190819

- Add I2C display driver for SH1106 oled by Gerhard Mutz
- Add SPI display drivers for epaper 4.2 inch, ILI9488 TFT, SSD1351 Color oled and RA8876 TFT by Gerhard Mutz
- Add support for HM17 bluetooth LE passive scan of ibeacon devices by Gerhard Mutz

### 6.6.0.5 20190816

- Add command WebSensor<sensor number> 0/1 to control display of sensor data in web GUI (#6085)
- Change some table locations from RAM to Flash
- Fix wrong telemetry message when SetOption68 1 (#6191)
- Add support for RDM6300 125kHz RFID Reader by Gerhard Mutz

### 6.6.0.4 20190806

- Add support for CHIRP soil moisture sensor by Christian Baars
- Add debug compile features using defines DEBUG_TASMOTA_CORE, DEBUG_TASMOTA_DRIVER and DEBUG_TASMOTA_SENSOR.
-   See DEBUG_CORE_LOG example in sonoff.ino and DEBUG_DRIVER_LOG example in xdrv_09_timers.ino
- Add support for Solax X1 inverter by Pablo Zerón
- Add ZigBee support phase 1 - low level MQTT ZNP messages for CC2530 devices
- Add command Buzzer with optional parameters <number of beeps>,<duration of beep in 100mS steps>,<duration of silence in 100mS steps> enabled when a buzzer is configured (#5988)
- Add support for PAJ7620 gesture sensor by Christian Baars

### 6.6.0.3 20190725

- Change filename of configuration backup from using FriendlyName1 to Hostname solving diacritic issues (#2422)
- Change Store AWS IoT Private Key and Certificate in SPI Flash avoiding device-specific compilations
- Upgrade library IRRemoteEsp8266 to 2.6.4, now using sendPioneer()
- Add support for MAX31865 Thermocouple sensor by Alberto Lopez Siemens
- Add option 0 to Width1 (Marker), Width2 (Second), Width3 (Minute) and Width4 (Hour) disabling display (#6152)
- Add MqttCount metric to STATE (#6155)
- Add define USE_ENERGY_MARGIN_DETECTION to disable Energy Margin and Power Limit detection
- Add define USE_ENERGY_POWER_LIMIT to disable Energy Power Limit detection while Energy Margin detection is active
- Add allow repeat/longpress for IRSend raw, introduced IRSend<r> option (#6074)
- Add SetOption68 to enable multi-channel PWM instead of a single light (#6134)

### 6.6.0.2 20190714

- Change commands Var and Mem to show all parameters when no index is given (#6107)
- Add command SetOption67 0/1 to disable or enable a buzzer as used in iFan03
- Add command DisplayWidth to set pixel width on supported devices
- Add command DisplayHeight to set pixel height on supported devices
- Add support for Sonoff iFan03 as module 71 (#5988)
- Add support for a buzzer
- Add support for IRSend long press ('repeat' feature from IRRemoteESP8266) (#6074)
- Add support for IRHVAC Midea/Komeco protocol (#3227)
- Add support for more IRSend protocols enabled in my_user_config.h
- Add support for IRSend Pioneer protocol (#6100)
- Add Oled reset GPIO option "OLED reset"

### 6.6.0.1 20190708

- Fix Domoticz battery level set to 100 if define USE_ADC_VCC is not used (#6033)
- Fix Force Elliptic Curve for Letsencrypt TLS #6042
- Fix WeMo emulation for 1G echo and 2G echo dot (#6086)
- Fix Xiaomi Philips brightness (#6091)
- Change defines USE_TX20_WIND_SENSOR and USE_RC_SWITCH in my_user_config.h to disable to lower iram usage enabling latest core compilation (#6060, #6062)
- Add blend RGB leds with White leds for better whites (#5895, #5704)
- Add command SetOption41 0..8 to control number of Tuya switches (#6039)
- Add command SetOption42 0..255 to set overtemperature (Celsius only) threshold resulting in power off all on energy monitoring devices. Default setting is 90 (#6036)
- Add command SetOption66 0/1 to enable or disable Tuya dimmer range 255 slider control
- Add command Time to disable NTP and set UTC time as Epoch value if above 1451602800 (=20160101). Time 0 re-enables NTP (#5279)
- Add AZ7798 automatic setting of clock display (#6034)
- Add Epoch and UptimeSec to JSON messages (#6068)
- Add support for up to 4 INA219 sensors (#6046)

### 6.6.0 20190707

- Remove support of TLS on core 2.3.0 and extent support on core 2.4.2 and up
- Remove MQTT uptime message every hour
- Refactor some defines to const
- Refactor webserver HTML input, button, textarea, and select name based on id
- Refactor webserver sensor data collection
- Refactor TLS based on BearSSL, warning breaking change for fingerprints validation
- Refactor management of lights, using classes and integers instead of floats
- Refactor UDP initial message handling from string to char using static memory and add debug info (#5505)
- Refactor IRSend and receive for 64-bit support (#5523)
- Refactor MQTT which might solve issue (#5755)
- Refactor IRSend by using heap when more than 199 values need to be send. May need increase of define MQTT_MAX_PACKET_SIZE too (#5950)
- Refactor double to float in rules, and replaced trigonometric functions from stdlib with smaller versions (#6005)
- Change pubsubclient MQTT_KEEPALIVE from 10 to 30 seconds for AWS IoT support
- Change gamma correction as default behavior, ie "Ledtable 1"
- Change PWM resolution from 8 to 10 bits for low brightness lights
- Change IRSend Panasonic protocol to 64-bit (#5523)
- Change ADC0 to enabled by default in my_user_config.h (#5671)
- Change define USE_EMULATION by USE_EMULATION_HUE and USE_EMULATION_WEMO (#5826)
- Change default PowerDelta from 80% to 0% on new installations (#5858, #5028, #4813, #4130, #4145, #3795, #3778, #3660, #3648)
- Fix display Bug in KNX webmenu for Physical Address
- Fix the Unescape() function and the SendSerial3 behaviour
- Fix webserver multiple Javascript window.onload functionality
- Fix TasmotaSerial at 9600 bps solving DFPlayer comms (#5528)
- Fix Configure Timer Web GUI (#5568)
- Fix Shelly 2.5 I2C address priority issue when VEML6070 code is present by disabling VEML6070 for Shelly 2.5 (#5592)
- Fix use of SerialDelimiter value 128 (#5634)
- Fix Sonoff Pow R2 / S31 invalid energy increments (#5789)
- Fix core 2.5.x ISR not in IRAM exception (#5837)
- Fix Philips Hue emulation Alexa issue by using part of MAC address for LightId (#5849)
- Fix missing white channel for WS2812 (#5869)
- Fix PZem startup issue (#5875)
- Fix exception 9 when syslog is enabled and NTP is just synced (#5917)
- Fix Toggle functionality to button double press when one button and two devices are detected (#5935)
- Fix channel command for dual dimmers (#5940)
- Fix not restoring white value on power off/power on (#5993)
- Add command AdcParam to control ADC0 Temperature and Light formula parameters
- Add command LedMask to assign which relay has access to power LED (#5602, #5612)
- Add extended LED power control using command LedPowerX where X is 1 to 4. Enabled when "LedLink(i)" is configured too (#5709)
- Add command Sensor20 1..255 to change Nova Fitness SDS01 working period in minutes (#5452)
- Add command SetOption38 6..255 to set IRReceive protocol detection sensitivity mimizing UNKNOWN protocols (#5853)
- Add command SetOption39 1..255 to control CSE7766 (Pow R2) or HLW8032 (Blitzwolf SHP5) handling of power loads below 6W. Default setting is 128 (#5756)
- Add command SetOption40 0..250 to disable button functionality if activated for over 0.1 second. Needs SetOption1 1 and SetOption13 0 (#5449)
- Add command SetOption63 0/1 to disable relay state feedback scan at restart (#5594, #5663)
- Add command SetOption64 0/1 to switch between "-" or "_" as sensor index separator impacting DS18X20, DHT, BMP and SHT3X sensor names (#5689)
- Add command SetOption65 0/1 and more Tuya Serial based device support (#5815)
- Add command WebColor to change GUI colors on the fly
- Add support for AWS IoT with TLS 1.2 on core 2.4.2 and up. Full doc here: https://github.com/arendst/Tasmota/wiki/AWS-IoT
- Add support for Badger HR-E Water Meter (#5539)
- Add support for Shelly 2.5 Energy and overtemp Monitoring (#5592)
- Add support for color and colortone for Philips Hue emulation via Alexa (#5600 #4809)
- Add support for Scripts as replacement for Rules. Default disabled but can be enabled in my_user_config.h (#5689)
- Add support for up to four LEDs related to four power outputs. Enabled when "LedLink(i)" is configured too (#5709)
- Add support for Shelly 1PM Template {"NAME":"Shelly 1PM","GPIO":[56,0,0,0,82,134,0,0,0,0,0,21,0],"FLAG":2,"BASE":18} (#5716)
- Add support for SPS30 Particle sensor thanks to Gerhard Mutz (#5830)
- Add support for VL53L0x time of flight sensor. Might interfere with TSL2561 using same I2C address (#5845)
- Add support for Sonoff L1 thanks to reef-actor (#6002)
- Add rule Http#Initialized
- Add rule System#Save executed just before a planned restart
- Add rule support for single JSON value pair like {"SSerialReceived":"on"} by expanding it to {"SSerialReceived":{"Data":"on"}} allowing for trigger SSerialReceived#Data=on (#5638)
- Add define USE_COUNTER to my_user_config.h to save space in sonoff-basic.bin and sonoff-minimal.bin
- Add define USE_DHT to my_user_config.h to save space in sonoff-basic.bin
- Add defines USE_EMULATION_WEMO and USE_EMULATION_HUE to my_user_config.h to control emulation features at compile time (#5826)
- Add Toggle functionality to button double press when more devices are detected
- Add device OverTemp (>73 Celsius) detection to Energy Monitoring devices with temperature sensor powering off all outputs
- Add Tuya Dimmer 10 second heartbeat serial packet required by some Tuya dimmer secondary MCUs
- Add all temperature, humidity and pressure for global access
- Add validation check when loading settings from flash
- Add HX711 weight restore after controlled restart or after power restore just before executing command Sensor34 7 (#5367, #5786)
- Add GUI hexadecimal color options in my_user_config.h (#5586)
- Add alternative IRSend command syntax IRSend raw,\<freq\>,\<header mark\>,\<header space\>,\<bit mark\>,\<zero space\>,\<one space\>,\<bit stream\> (#5610)
- Add user configurable ADC0 to Module and Template configuration compatible with current FLAG options (#5671)
- Add AriLux RF control GPIO option "ALux IrSel" (159) replacing "Led4i" (59) for full LED control (#5709)
- Add LED GPIO option "LedLink" (157) and "LedLinki" (158) to select dedicated link status LED (#5709)
- Add all 5 PWM channels individually adressable with LEDs. (#5741)
- Add reset of Energy values when connection to sensor is lost for over 4 seconds (#5874, #5881)
- Add checkbox to GUI password field enabling visibility during password entry only (#5934)

### 6.5.0 20190319

- Remove commands SetOption14 and SetOption63 as it has been superseded by command Interlock
- Remove command SetOption35 0-255 for mDNS start-up delay (#4793)
- Remove support for MQTT_LIBRARY_TYPE, MQTT_ARDUINOMQTT and MQTT_TASMOTAMQTT (#5474)
- Change webserver content handling from single String to small Chunks increasing RAM
- Change code use of boolean to bool and byte to uint8_t
- Change code uint8_t flags to bool flags
- Change sonoff_template.h layout regarding optional module flags like ADC0
- Change sonoff_template.h module lay-out by removing non-configurable GPIOs
- Change button driver making it modular
- Change switch driver making it modular and introduce input filter (#4665, #4724)
- Change switch input detection by optimizing switch debounce (#4724)
- Change web authentication (#4865)
- Change image name BE_MINIMAL to FIRMWARE_MINIMAL and USE_xyz to FIRMWARE_xyz (#5106)
- Change GUI weblog from XML to plain text solving possible empty screens (#5154)
- Fix most compiler warnings
- Fix Display exception 28 when JSON value is nullptr received
- Fix epaper driver (#4785)
- Fix HAss Sensor Discovery Software Watchdog restart (#4831, #4988)
- Fix allowable MAX_RULE_VARS to 16 (#4933)
- Fix mDNS addService (#4938, #4951)
- Fix HAss discovery of MHZ19(B) sensors (#4992)
- Fix some exceptions and watchdogs due to lack of stack space (#5215)
- Fix GUI wifi password acception starting with asteriks (*) (#5231, #5242)
- Fix command WebSend intermittent results (#5273, #5304)
- Fix additional characters in fallbacktopic, hostname and mqttclient on core 2.5.0 (#5359, #5417)
- Fix Energy TotalStartTime when commands EnergyReset0 and/or EnergyReset3 used (#5373)
- Fix DS18S20 temperature calculation (#5375)
- Fix float calculations in range from 0 to -1 (#5386)
- Fix exception on GUI Configure Logging and Configure Other (#5424)
- Add commands PowerCal, VoltageCal and CurrentCal for HLW8012, HJL01 and BL0937 based energy sensors
- Add command SerialDelimiter 128 to filter reception of only characters between ASCII 32 and 127 (#5131)
- Add command SSerialSend5 \<hexdata\> to SerialBridge
- Add command Interlock 0 / 1 / 1,2 3,4 .. to control interlock ON/OFF and add up to 8 relays in 1 to 4 interlock groups (#4910, #5014)
- Add command Template 255 to copy module configuration over to current active template and store as user template named Merged (#5371)
- Add command WifiConfig 7 to allow reset of device in AP mode without admin password (#5297)
- Add command SetOption36 to control boot loop default restoration (#4645, #5063)
- Add command SetOption37 for RGBCW color mapping (#5326)
- Add command SetOption55 0/1 and define MDNS_ENABLE to disable/enable mDNS (#4793, #4923)
- Add command SetOption62 0/1 to disable retain on Button or Switch hold messages (#5299)
- Add support for Smanergy KA10 Smart Wall Socket with Energy monitoring
- Add support for commands in sensor drivers
- Add support for MAX31855 K-Type thermocouple sensor using softSPI (#4764)
- Add support for Near Field Communication (NFC) controller PN532 using Serial (#4791, #5162)
- Add support for OBI Power Socket 2 (#4829)
- Add support for YTF IR Bridge (#4855)
- Add support for Mi LED Desk Lamp with rotary switch (#4887)
- Add support for Digoo DG-SP202 Smart Socket with Energy monitoring (#4891)
- Add support for MAX44009 Ambient Light sensor (#4907)
- Add support for inverted buttons and inverted buttons without pullup (#4914)
- Add support for Luminea ZX2820 Smart Socket with Energy monitoring (#4921)
- Add support for multiple ADS1115 I2C devices (#5083)
- Add support for online template change using command Template or GUI Configure Other (#5177)
- Add support for Korean language translations (#5344)
- Add support for sensor SCD30 (#5434)
- Add parameter CFG_HOLDER to status 1 message (#5206)
- Add SetOption32 until SetOption49 diagnostic information to Status 3 report as replacement for second property value in SetOption property name
- Add Resolution property to Status 3 report providing previous SetOption second value property
- Add property MqttCount to status 6 message representing number of Mqtt re-connections
- Add property LinkCount to state and status 11 message representing number of Wifi Link re-connections
- Add property Downtime to state and status 11 message representing the duration of wifi connection loss
- Add variable %timestamp% to rules (#4749)
- Add rule support for "==", "!=" ">=" and "<=" (#5122)
- Add rule expression enabled by define USE_EXPRESSION in my_user_config.h (#5210)
- Add Power status functionality to LED2 when configured leaving LED1 for Link status indication
- Add user configuration of HLW8012 and HJL-01/BL0937 Energy Monitoring as used in Sonoff Pow and many Tuya based devices
- Add user configuration of MCP39F501 Energy Monitoring as used in Shelly2
- Add online template configuration using both commands and Configure Template menu option in GUI
- Add (S)SerialSend3 escape sequence \x to allow hexadecimal byte value (#3560, #4947)
- Add define DS18B20_INTERNAL_PULLUP to select internal input pullup when only one DS18B20 sensor is connected eliminating external resistor (#4738)
- Add button control when no relay configured (#4682)
- Add startup delay of 4 seconds to button control (#4829)
- Add core version conditional compile options to provided PWM files (#4917)
- Add resiliency to saved Settings (#5065)
- Add MHZ19 Temperature as Domoticz Temperature selection (#5128)
- Add HAss status sensor (#5139)
- Add status message to former declined group commands (#5145)
- Add 0x to IRRemote (SetOption29) and RCSwitch (SetOption28) received hexadecimal data (#5431)

### 6.4.1 20181224

- Change RAM usage BMP/BME I2C sensors
- Change FallbackTopic from cmnd/\<mqttclient\>/ to cmnd/\<mqttclient\>_fb/ to discriminate from Topic (#1528)
- Change FallbackTopic detection (#4706)
- Change Hass discovery to short MQTT messages as used by Hass 0.81 and up (#4711)
- Change MQTT GUI password handling (#4723)
- Fix possible dtostrf buffer overflows by increasing buffers
- Fix wifi strongest signal detection (#4704)
- Fix Alexa "this value is outside the range of the device". Needs power cycle and Alexa deletion/discovery cycle. (#3159, #4712)
- Add Slovak language file (#4663)
- Add support for AZ-Instrument 7798 CO2 meter/datalogger (#4672)
- Add define WIFI_SOFT_AP_CHANNEL in my_user_config.h to set Soft Access Point Channel number between 1 and 13 as used by Wifi Manager web GUI (#4673)
- Add define USE_MQTT_TLS_CA_CERT for checking MQTT TLS against root ca using Let's Encrypt cert from sonoff_letsencrypt.h - not supported with core 2.3.0 (#4703)

### 6.4.0 20181217

- Change GUI Configure Module by using AJAX for data fetch to cut page size (and memory use) by 40%
     In case of web page errors clear your browser cache or do Page Reload (F5 or Ctrl+R)
- Change enforcing flashmode dout but it is still mandatory
- Change bootcount update (being first) flash write to 10 seconds after restart
- Change display and epaper drivers
- Change command WebSend Host header field from IP address to hostname (#4331)
- Change log buffer size from 512 to 520 to accommodate http sensor data (#4354)
- Change default WIFI_CONFIG_TOOL from WIFI_WAIT to WIFI_RETRY in my_user_config.h (#4400)
- Change webgui refresh time delay for Save Settings and local OTA Upload (#4423)
- Change SR-04 driver to use NewPing library (#4488)
- Change MCP230xx driver to support interrupt retention over teleperiod (#4547)
- Change support for MPU6050 using DMP (#4581)
- Fix unintended function overload of WifiState
- Fix wifi connection errors using wifi disconnect and ESP.reset instead of ESP.restart
- Fix Sonoff Pow R2 and Sonoff S31 Serial interface hang caused by Sonoff Basic R2 driver delay implementation (and possibly core bug)
- Fix MQTT connection error after restart
- Fix wifi re-scan connection baseline
- Fix possible strncat buffer overflows
- Fix intermittent Pzem sensor energy overflow calculation error
- Fix shelly2 ghost switching caused by lack of pull-up inputs (#4255)
- Fix hardware serial pin configuration. To keep using hardware serial swap current Rx/Tx pin configuration only (#4280)
- Fix MqttRetry values above 255 seconds (#4424)
- Fix WifiManager functionality on initial installation (#4433)
- Fix ArduinoOTA for Core 2.5.0 (#4620)
- Add minutes to commands Timezone to allow all possible world timezones
- Add more strict checks for GPIO selections
- Add code image and optional commit number to version
- Add dynamic delay to main loop providing time for wifi background tasks
- Add additional start-up delay during initial wifi connection
- Add support for decoding Theo V2 sensors as documented on https://sidweb.nl using 434MHz RF sensor receiver
- Add support for decoding Alecto V2 sensors like ACH2010, WS3000 and DKW2012 weather stations using 868MHz RF sensor receiver
- Add user definition of defines WIFI_RSSI_THRESHOLD (default 10) and WIFI_RESCAN_MINUTES (default 44)
- Add command SetOption58 0/1 to enable IR raw data info in JSON message (#2116)
- Add command IRSend <frequency>|0,<rawdata1>,<rawdata2>,.. to allow raw data transmission (#2116)
- Add command SetOption56 0/1 to enable wifi network scan and select highest RSSI (#3173)
- Add command SetOption57 0/1 to enable wifi network re-scan every 44 minutes with a rssi threshold of 10 to select highest RSSI (#3173)
- Add support for SDM220 (#3610)
- Add default sleep 1 to sonoff-basic to lower energy consumption (#4217)
- Add wifi status to Tuya (#4221)
- Add delays to reduce CPU usage at boot time (#4233)
- Add command SetOption24 0/1 to select pressure unit as hPa or mmHg (#4241)
- Add optional hardware serial when GPIO13(Rx) and GPIO15(Tx) are selected removing hardware serial from GPIO01(Tx) and GPIO03(Rx) (#4288)
- Add support for Gosund SP1 v2.3 Power Socket with Energy Monitoring (#4297)
- Add support for Armtronix dimmers. See wiki for info (#4321)
- Add to command WebSend option to send a direct path when command starts with a slash (#4329)
- Add support for LG HVac and IrRemote (#4377)
- Add initial support for Hass sensor discovery (#4380)
- Add support for Fujitsu HVac and IrRemote (#4387)
- Add support for I2C MGC3130 Electric Field Effect sensor by Christian Baars (#3774, #4404)
- Add command CalcRes to set number of decimals (0 - 7) used in commands ADD, SUB, MULT and SCALE (#4420)
- Add CPU average load to state message (#4431)
- Add command SetOption59 0/1 to change state topic from tele/STATE to stat/RESULT (#4450)
- Add support for SM Smart Wifi Dimmer PS-16-DZ (#4465)
- Add support for Teckin US Power Socket with Energy Monitoring (#4481)
- Add command SetOption60 0/1 to select dynamic sleep (0) or sleep (1) (#4497)
- Add support for iFan02 Fanspeed in Domoticz using a selector (#4517)
- Add support for GPIO02 for newer Sonoff Basic (#4518)
- Add Announce Switches to MQTT Discovery (#4531)
- Add support for Manzoku Power Strip (#4590)

### 6.3.0 20181030

- Change web Configure Module GPIO drop down list order for better readability
- Change status JSON message providing more switch and retain information
- Change xsns_17_senseair.ino to use TasmotaModbus library
- Change MCP230xx driver
- Change PubSubClient Mqtt library to non-blocking EspEasy version
- Change energy monitoring using energy sensor driver modules
- Change Webserver page handler for easier extension (thx to Adrian Scillato)
- Change pinmode for no-pullup defined switches to pullup when configured as switchmode PUSHBUTTON (=3 and up) (#3896)
- Change default OTA Url to http://thehackbox.org/tasmota/release/sonoff.bin (#4170)
- Remove support for MQTT Client esp-mqtt-arduino by #define MQTT_LIBRARY_TYPE MQTT_ESPMQTTARDUINO
- Remove commands PowerCal, VoltageCal and CurrentCal as more functionality is provided by commands PowerSet, VoltageSet and CurrentSet
- Remove restart after ntpserver change and force NTP re-sync (#3890)
- Fix showing Period Power in energy threshold messages
- Fix header file execution order by renaming user_config.h to my_user_config.h
- Fix some TSL2561 driver issues (#3681)
- Fix KNX PA exception. Regression from 6.2.1 buffer overflow caused by subStr() (#3700, #3710)
- Fix setting and getting color temperature for Philips Hue emulation (#3733)
- Fix ButtonRetain to not use default topic for clearing retain messages (#3737)
- Fix syslog when emulation is selected (#2109, #3784)
- Fix rule trigger POWER1#STATE execution after restart and SetOption0 is 0 (#3856)
- Fix Home Assistant forced light discovery (#3908)
- Fix invalid configuration restores and decode_config.py crc error when savedata = 0 (#3918)
- Fix timer offset -00:00 causing 12:00 hour offset (#3923)
- Fix I2CScan invalid JSON error message (#3925)
- Fix exception when wrong Domoticz JSON message is received (#3963)
- Fix Sonoff Bridge RfRaw receive (#4080, #4085)
- Fix possible wifi connection error (#4044, #4083)
- Fix invalid JSON floating point result from nan (Not a Number) and inf (Infinity) into null (#4147)
- Fix rule mqtt#connected trigger when mqtt is disabled (#4149)
- Add support for LCD, Matrix, TFT and Oled displays
- Add support for Neo Coolcam Wifi Smart Power Plug
- Add support for Michael Haustein ESP Switch
- Add support for MQTT Client based on lwmqtt to be selected by #define MQTT_LIBRARY_TYPE MQTT_ARDUINOMQTT
- Add support for Neo Coolcam Wifi Smart Power Plug
- Add support for Michael Haustein ESP Switch
- Add support for MQTT Client based on lwmqtt to be selected by #define MQTT_LIBRARY_TYPE MQTT_ARDUINOMQTT
- Add support for DS3231 Real Time Clock
- Add support for HX711 Load Cell with optional web GUI scale interface to demonstrate easy GUI plug-in
- Add support for serial 8N2 communication to TasmotaModbus and TasmotaSerial libraries
- Add support for RF transceiving using library RcSwitch (#2702)
- Add support for Shelly 1 and Shelly 2 (#2789)
- Add support for La Crosse TX20 Anemometer (#2654, #3146)
- Add support for MP3 player using DFRobot RB-DFR-562 (#3723)
- Add Support for Xiaomi-Philips Bulbs (#3787)
- Add support for PCA9685 12bit 16pin hardware PWM driver (#3866)
- Add support for EXS Relay V5.0 (#3810)
- Add support for OBI Power Socket (#1988, #3944)
- Add support for Teckin Power Socket with Energy Monitoring (#3950)
- Add support for Pzem-003/017 DC Energy monitoring module (#3694)
- Add support for Pzem-014/016 AC Energy monitoring module (#3694)
- Add support for CSL Aplic WDP 303075 Power Socket with Energy Monitoring (#3991, #3996)
- Add support for Tuya Dimmer (#469, #4075)
- Add command Display to show all settings at once
- Add command SerialSend5 to send raw serial data like "A5074100545293"
- Add command WebRefresh 1000..10000 to control web page refresh in milliseconds. Default is 2345
- Add command WeightRes 0..3 to control display of decimals for kilogram
- Add command RGBWWTable to support color calibration (#3933)
- Add command Reset 4 (reset to defaults but keep wifi params) and Reset 5 (as reset 4 and also erase flash) (#4061)
- Add command SetOption35 0..255 (seconds) to delay mDNS initialization to control possible Wifi connect problems
- Add command SetOption52 0/1 to control display of optional time offset from UTC in JSON messages (#3629, #3711)
- Add command SetOption53 0/1 to toggle gui display of Hostname and IP address (#1006, #2091)
- Add authentication to HTTP web pages
- Add decimals as input to commands PowerSet, VoltageSet and CurrentSet
- Add tools/decode-config.py by Norbert Richter to decode configuration data. See file for information
- Add define USE_DISPLAYS for selecting image sonoff-display
- Add define USE_BASIC for selecting image sonoff-basic without most sensors
- Add auto reload of main web page to some web restarts
- Add TasmotaModbus library as very basic modbus wrapper for TasmotaSerial
- Add more API callbacks and document API.md
- Add Apparent Power and Reactive Power to Energy Monitoring devices (#251)
- Add token %hostname% to command FullTopic (#3018)
- Add Wifi channel number to state message (#3664)
- Add user configurable GPIO02 and GPIO03 on H801 devices (#3692)
- Add toggle function RGBW lights (#3695, #3697)
- Add network information to display start screen (#3704)
- Add sleep to Nova Fitness SDS01X sensor (#2841, #3724, #3749)
- Add Analog input AD0 enabled to sonoff-sensors.bin (#3756, #3757)
- Add power value below 5W to Sonoff Pow R2 and S31 (#3745)
- Add RF Receiver control to module MagicHome to be used on Arilux LC10 (#3792)
- Add userid/password option to decode-status.py (#3796)
- Add delay after restart before processing rule sensor data (#3811)
- Add force_update to Home Assistant discovery (#3873)
- Add rule triggers SWITCH1#BOOT and POWER1#BOOT (#3904, #3910)
- Add Hebrew language file (#3960)
- Add TotalStartTime to Energy JSON message (#3971)
- Add whitespace removal from RfRaw and SerialSend5 (#4020)
- Add support for two BMP/BME sensors (#4195)

### 6.2.1 20180905

- Fix possible ambiguity on command parameters if StateText contains numbers only (#3656)
- Fix Wemo emulation to select the first relay when more than one relay is present (#3657)
- Fix possible exception due to buffer overflow (#3659)
- Fix lost energy today and total energy value after power cycle (#3689)

### 6.2.0 20180901

- Allow user override of define MAX_RULE_VARS and MAX_RULE_TIMERS (#3561)
- Disable wifi sleep for both Esp8266/Arduino core 2.4.1 and 2.4.2 to solve device freeze caused by Espressif SDK bug (#3554)
- Change DS18B20 driver to provide better instant results
- Change some sensor drivers to provide instant results
- Change define USE_ALL_SENSORS to USE_SENSORS as it doesn't contain all sensors due to duplicate I2C addresses
- Change some sensor update timings: AdcEvery 200 -> 250, Senseair 300 -> 250, SDM120 300 -> 250, SDM630 300 -> 250
- Change default Wifi config option from WPS to Wifi Manager if WPS is disabled or Wifi Smartconfig if webserver is disabled or Wifi Serial input if Smartconfig is disabled
- Change SHT1x driver to provide better instant results and fix I2C interference
- Change DHT driver to provide better instant results and add decimals to DHT11 (#3164)
- Change DS18x20 driver to provide better instant results (#3169)
- Change CounterType 1 from milliseconds to microseconds (#3437)
- Change scheduler for better sleep support using Uptime, Delay, PulseTime and TelePeriod, Blinktime (#3581)
- Remove unused functionality from Sonoff-minimal to save space
- Remove WPS and SmartConfig from sonoff-minimal saving 56k code space
- Remove TSL2561 debug message and update library (#2415)
- Remove forced restart when sleep command is executed (#3554)
- Fix invalid response using more than 4 switches and domoticz
- Fix sonoff-minimal not using default settings
- Fix unsecure main webpage update
- Fix DHT driver mixing values for different sensors (#1797)
- Fix EnergyReset3 regression not clearing total energy (#2723)
- Fix rules once regression from v6.1.0 (#3198, #3226)
- Fix command Scale buffer overflow (#3236)
- Fix possible WDT due to long MQTT publish handling (#3313)
- Fix command TimeDst/TimeStd invalid JSON (#3322)
- Fix handling of default names when using names starting with shortcut character ",0,1 or 2 (#3392, #3600, #3618)
- Fix LM75AD I2C sensor detection (#3408)
- Fix iFan02 power on state (#3412, #3530)
- Fix some Pow R2 and S31 checksum errors using optimized re-sync (#3425)
- Fix SDM120 reporting wrong negative values to Domoticz (#3521)
- Fix MQTT reconnection detection when using TasmotaMqtt library (#3558)
- Fix OtaMagic when file path contains a dash (-) (#3563)
- Fix Sonoff Bridge data reception when using Portisch EFM8 firmware using in data buffer length (#3605)
- Add read sensor retry to DS18B20, DS18x20, DHT, SHT1X and HTU21
- Add user selection of Wifi Smartconfig as define USE_SMARTCONFIG in user_config.h
- Add boot loop detection and perform some solutions
- Add wifi and mqtt status led blinkyblinky to be disabled by SetOption31 1. Does not work when LedPower is On (deliberate) (#871, #2230, #3114, #3155)
- Add support for TM1638 switch (#2226)
- Add GPIO options ButtonXn, SwitchXn and CounterXn to select INPUT mode instead of INPUT_PULLUP (#2525)
- Add support for APDS9960 proximity sensor (#3051)
- Add support for MPR121 controller in input mode for touch buttons (#3142)
- Add support for MCP230xx for general purpose input expansion and command Sensor29 (#3188)
- Add default Wifi Configuration tool as define WIFI_CONFIG_NO_SSID in user_config.h if no SSID is configured (#3224)
- Add command Timers 0/1 to globally disable or enable armed timers (#3270)
- Add support for CCS811 sensor (#3309)
- Add Turkish language file (#3332)
- Add command SerialSend4 to send binary serial data (#3345)
- Add initial support for sensor MPU6050 (#3352)
- Add rule triggers Wifi#Connected and Wifi#Disconnected (#3359)
- Add option + to command Rule to concatenate new rule with existing rules (#3365)
- Add message when JavaScript is not enabled in webbrowser (#3388)
- Add build time setting of ButtonTopic and SwitchTopic (#3414)
- Add iFan02 Fanspeed + and Fanspeed - command options (#3415)
- Add Individual HSBColorX commands (#3430, #3615)
- Add output support on MCP23008/MCP23017 (#3436)
- Add modulo option to rules like rule1 on Time#Minute|5 do backlog power on;delay 200;power off endon (#3466)
- Add RGB support for Domoticz (#3547)
- Add all ruletimer values to command RuleTimer result message (#3571)
- Add command Publish2 for publishing retained MQTT messages (#3593)
- Add commands ButtonDebounce 40..1000 and SwitchDebounce 40..1000 to have user control over debounce timing. Default is 50mS (#3594)
- Add RuleX debug options 8,9,10 (StopOnError) to control RuleX execution status after an exception restart (#3607)
- Add rule variables %sunrise%, %sunset%, %uptime% and %time% (#3608)
- Add optional MQTT_TELE_RETAIN to Energy Margins message (#3612, 3614)

### 6.1.1 20180714

- Revert wifi changes (#3177)
- Revert sonoff-minimal removals causing failure of wifi connection (#3177)

### 6.1.0 20180706

- Remove version 3, 4 and pre 5.2 settings auto-upgrade. See https://github.com/arendst/Tasmota/wiki/Upgrading#migration-path
- Change default CFG_HOLDER from 0x20161209 to 4617 (=0x1209) - no impact on default upgrades
- Change number of supported switches from 4 to 8 (#2885, #3086)
- Change BME680 driver from Adafruit to Bosch BME680 library (#2969)
- Fix Pzem004T checksum error
- Fix KNX bug when doing reply of sensors values
- Fix rules induced LWT message
- Fix possible wifi connection problem (#1366)
- Fix some Pow R2 and S31 checksum errors (#1907)
- Fix display selection of un-available GPIO options in Module Configuration webpage (#2718)
- Fix timer re-trigger within one minute after restart (#2744)
- Fix IRSend not accepting data value of 0 by David Conran (#2751)
- Fix vars on rules by Adrian Scillato (#2769)
- Fix bug in KNX menu by Adrian Scillato (#2770)
- Fix anomalies in rules (#2778)
- Fix HUE bridge V1 software version by Heiko Krupp (#2788)
- Fix Hardware Watchdog restart when using event command (#2853)
- Add Ukrainian language file
- Add KNX support for DS18S20 Temperature sensor
- Add CRC to Settings making future upgrades more fail-safe
- Add feature information to Status 4
- Add tools folder with python script decode-status.py for decoding some status fields like SetOption and Features
- Add Slots on the KNX Web Menu to select Group Addess to receive data to trigger rules
- Add two rule sets of 511 characters using commands rule1, rule2 and rule3
- Add Console Commands to send KNX Commands and KNX Values
- Add Slots on the KNX Web Menu to select Group Addess to send data from console commands
- Add Events to trigger rules when a command or read requests is received from KNX
- Add command SetOption30 to enforce Hass discovery as light group (#1784)
- Add support for BlitzWolf BW-SHP2 (and Homecube, Gosund SP1) Energy Monitoring Smart Socket (#2223)
- Add time in minutes to rule Time#Initialized, Time#set and Time#Minute (#2669)
- Add Eastron SDM630 energy meter by Gennaro Tortone (#2735)
- Add KNX communication enhancement by Adrian Scillato (#2742)
- Add KNX energy data by Adrian Scillato (#2750)
- Add rule support for IrReceive and RfReceive (#2758)
- Add python script fw-server.py in tools folder to create a simple OTA server by Gennaro Tortone (#2759)
- Add rule variables %time% for minutes since midnight, %uptime%, %sunrise% and %sunset% giving time in minutes (#2669)
- Add rules %mem1% to %mem5% variable names storing data in flash (#2780)
- Add rules test on %varx% or %memx% (#2780)
- Add optional token %id% substituting the unique MAC address to fulltopic by Michael Graf (#2794)
- Add support for Sonoff S26 Smart Socket (#2808)
- Add command WebSend [<host>(:<port>,<user>:<password>)] <command> (#2821)
- Add increment and decrement value to command Counter (#2838)
- Add support for Sonoff iFan02 as module 44 introducing command FanSpeed 0..3 (#2839)
- Add source information to command execution to be shown with logging option 3 (#2843)
- Add support for uploading Sonoff Bridge firmware found in tools/fw_efm8bb1 folder build by Portisch using Web Gui File Upload (#2886)
- Add command RfRaw to control Portisch firmware features
- Add support for I2C temperature sensor LM75AD (#2909)
- Add option 0 to command Timers disarming all timers (#2962)
- Add performance improvement when updating multiple individual WS2812 pixels (#3007)
- Add command SetOption28 to switch between hex or decimal Sonoff Bridge RF received data format (#3008)
- Add command SetOption29 to switch between hex or decimal IR received data format
- Add decimal values support for commands ADD, SUB, MULT and SCALE (#3083, #3089)
- Add support for bitflags SetOption50 .. SetOption81 (#3118)

### 5.14.0 20180515

- Update language files
- Update TasmotaSerial to 2.0.0 allowing Hardware Serial Fallback when correct connections are configured
- Change command handling
- Change user_config(_override).h defines TIME_STD and TIME_DST
- Change user_config(_override).h otaurl to http://sonoff.maddox.co.uk/tasmota/sonoff.bin (#2588, #2602)
- Fix configuration restore regression from 5.13.1
- Fix compile error when ADC is enabled and Rules are disabled (#2608)
- Fix rule power trigger when no backlog command is used (#2613)
- Fix several timer data input and output errors (#2597, #2620)
- Fix KNX config error (#2628)
- Fix sensor MHZ-19 vanishing data over time (#2659)
- Fix KNX reconnection issue (#2679)
- Fix DST and STD time for Southern Hemisphere by Adrian Scillato (#2684, #2714)
- Add Portuguese in Brazil language file
- Add SetOption26 to enforce use of indexes even when only one relay is present (#1055)
- Add support for sensor SI1145 UV Index / IR / Visible light (#2496)
- Add rule state test for On/Off in addition to 0/1 (#2613)
- Add hardware serial option to MHZ-19 sensor (#2659)
- Add Eastron SDM120 energy meter by Gennaro Tortone (#2694)
- Add user entry DST/STD using commands TimeStd and TimeDst (See wiki for parameter syntax) (#2721)

### 5.13.1 20180501

- Fix JSON buffers size too small for execution in some situations (#2580)
- Fix configuration restore (#2591)
- Add define MODULE for user selecting default model although it preferably should not be changed (#569, #2589)

### 5.13.0 20180430

- Change platformio option sonoff-ds18x20 to sonoff-allsensors enabling ds18x20 and all other sensors in one image
- Change status display of Ssid and SetOption
- Change default option SetOption15 from 0 to 1 providing better initial PWM experience
- Change webpage parameter communication
- Change max number of commands in Backlog from 15 to 30 and ignore commands overflowing
- Change TSL2561 driver to joba library and delete Adafruit library (#1644)
- Change default parameters in user_config.h to undefined for easy installation (#1851)
- Change max user configurable hold time from 10 to 25 seconds (#1851)
- Change Sonoff SC JSON format (#1939)
- Change Polish language to using Diacritics (#2005)
- Change user_config_override usage by providing user_config_override_sample.h (#2228)
- Change MQTT response topic for Energy changes from ENERGY to SENSOR (#2229, #2251)
- Change default Reset configuration time from 4 seconds to 40 seconds on Button hold (#2268)
- Change ESP8266 Analog JSON message from {"Analog0:123"} to {"ANALOG":{"A0:123"}} to accomodate rules (#2560)
- Change Counter JSON message from {"Counter1":0,"Counter3":0} to {"COUNTER":{"C1":0,"C3":0}} to accomodate rules
- Change ADS1115 JSON message from {"ADS1115":{"Analog0":123,"Analog1":123}} to {"ADS1115":{"A0":123,"A1":123}}
- Fix intermittent exception when dns lookup is used while sleep is enabled
- Fix 5.4.0 regression turning off single press after button hold during 4x hold time
- Fix possible wifi connection problem by erasing sdk configuration parameters
- Fix NTP sync to Thu Jan 01 08:00:10 1970 results in uptime 17651+ days (core2.4.1/sdk2.2.1)
- Fix MAX31850 higher temperatures (#1269)
- Fix freeing more code space when emulation is disabled (#1592)
- Fix providing web page configuratin option for Friendly Name when no device (relay or light) is configured (#1850)
- Fix compile error when define HOME_ASSISTANT_DISCOVERY_ENABLE is not set (#1937)
- Fix MQTT TLS fingerprint validation (#2033)
- Fix update temperature on DS18x20 drivers (#2328)
- Fix compile error when not defined USE_TIMERS (#2400)
- Fix configuration filename truncation when it contains spaces (#2484, #2490)
- Fix Energy Today and Yesterday overflow (#2543)
- Add serial debug info
- Add Portuguese language file
- Add Czech language file
- Add Bulgarian language file
- Add Domoticz dust (custom) sensors to PMS5003 and NovaFitness SDS drivers as PM1, PM2.5 and PM10
- Add commands Publish, Rule, RuleTimer and Event. See Wiki about Rule restriction, usage and examples
- Add sonoff-classic, sonoff-allsensors and sonoff-knx
- Add some coloring to important web buttons
- Add support for sensor HC-SR04 ultrasonic (#113, #1964, #2444)
- Add define MQTT_TELE_RETAIN compile option default set to 0 (#1071)
- Add 16 timers using commands Timer and Timers (#1091)
- Add optional Timer configuration webpage to be enabled in user_config.h with define USE_TIMERS_WEB
- Add Multichannel Gas sensor using MultiChannel_Gas_Sensor library (#1245)
- Add Domoticz Battery and RSSI Quality (#1604)
- Add command HSBColor Hue,Sat,Bri (#1642, #2203)
- Add compile time support for WS2812 BRG and RBG led configurations to be defined in user_config.h (#1690)
- Add optional usage of %d or %X suffices in MQTT client to append chipid (#1871)
- Add optional usage of %d or %X suffices in MQTT topic to append chipid (#1871)
- Add optional usage of %d or %04d in ota url to be replaced with chipid (#1871)
- Add Sonoff Bridge command RfKey<x> 5 to show current RF key values either default or learned (#1884)
- Add user configurable serial GPIOs to MagicHome and Arilux modules (#1887)
- Add Russian language file (#1909)
- Add Webserver upload preflight request support (#1927)
- Add Home Assistant clear other device (#1931)
- Add Restart time to Status 1 (#1938)
- Add optional TSL2561 driver using library Joba_Tsl2561 to be enabled in user_config.h with define USE_TSL2561_JOBA (#1951)
- Add support for sensor SHTC3 (#1967)
- Add compiler check for stable lwIP version v1.4 (#1940)
- Add support for multiple SHT3X sensors (#1949, #2110)
- Add always suffix with device number in Mqtt discovery topic (#1962)
- Add support for optional MQTT drivers to be selected in user_config.h (#1992)
- Add optional Arduino OTA support to be enabled in user_config.h (#1998)
- Add diacritics to Polish language file (#2005)
- Add Hungarian language file (#2024)
- Add support for Nova Fitness SDS011 and possibly SDS021 particle concentration sensor (#2070)
- Add single decimal precision to Nova Fitness SDS0x1 sensor values (#2093)
- Add Chinese (Traditional) in Taiwan language file (#2108)
- Add Sonoff SC domoticz support for Sound level as Counter and Air quality (#2118)
- Add a second TLS fingerprint to allow switching keys in TLS mode (#2033, #2102)
- Add display of remaining pulse time to command PulseTime (#2085)
- Add additional time offset to Wifi Retry based on device mac address (#2089)
- Add command Color6 RRGGBB for Clock hour marker color and command Rotation pixels for Clock rotation (#2092)
- Add HTML language header in local language (#2123)
- Add command PowerDelta 0..100 (percentage) to Energy monitoring devices to report on active power load change (#2157)
- Add Restart Reason to Status 1 report (#2161)
- Add command Channel 0..100 to control dimmer value for individual color channels (#2111, #2203)
- Add support for Hardware Serial bridge using commands SerialDelimiter, Baudrate and SerialSend. Supports 8N1 and text only (#2182)
- Add support for Software Serial bridge using commands SerialDelimiter, SBaudrate and SSerialSend. Supports 8N1 and text only (#2190)
- Add support for Zengge WF017 PWM Led strip controller (#2202)
- Add PWM status to command State if PWM enabled (#2203)
- Add all FriendlyNames to Status information (#2208)
- Add Channel status information (#2211)
- Add hexadecimal Data entry to command IrSend using 0x notation (#1290, #2314)
- Add Home Assistant MQTT Discovery for Buttons and change SetOption19 response (#2277)
- Add multiple color entry support for command Led like Led2 120000 001200 000012 setting led2 as Red, Led3 as Green and Led4 as Blue (#2303)
- Add hexadecimal RGB color entry on RGBCW leds (#2304)
- Add support for SGP30 gas and air quality sensor (#2307)
- Add optional Sunrise and Sunset timers with commands Latitide and Longitude to be enabled with define USE_SUNRISE in user_config.h (#2317)
- Add timer sunrise and sunset offset (#2378)
- Add user selectable defines for Sunrise/set Dawn option (#2378)
- Add optional KNX IP Protocol Support (#2402)
- Add random window to timers (#2447)
- Add Greek language file (#2491)
- Add support for Sonoff Pow R2 (#2340)
- Add GPIO_User to GPIO02 for all Sonoff T1 (#2524)

### 5.12.0 20180209

- Change library PubSubClient.h define MQTT_MAX_PACKET_SIZE from 512 to 1000 for Home Assistant  support
- Change relation of define MESSZ being dependent on PubSubClient.h define MQTT_MAX_PACKET_SIZE
- Change command color parameter input checks to less strict for Home Assistant support
- Change command Ina219Mode into command Sensor13
- Change commands HlwPCal, HlwUCal and HlwICal to PowerCal, VoltageCal and CurrentCal to be used for both Pow and S31 calibration
- Change commands HlwPSet, HlwUSet and HlwISet to PowerSet, VoltageSet and CurrentSet to be used for both Pow and S31 calibration
- Change uptime from hour to second resulting in a display of 123T13:45:21 where 123 is days
- Change module name Wemos D1 mini into Generic (#1220)
- Change HTML from width=100% to style=width:100% supporting HTML5 (#1358)
- Change OSWATCH_RESET_TIME (Blocked loop) from 30 to 120 seconds to allow slow networks (#1556)
- Change WIFI_MANAGER_SEC into WIFI_CONFIG_SEC (#1616)
- Change function pointers code to save code space and memory (#1683)
- Change webserver argument processing gaining 5k code space (#1705)
- Change weblog memory usage (#1730, #1793, #1819)
- Update TasmotaSerial library to 1.1.0
- Update language files Italian (#1594), Dutch (#1723) and Spanish (#1722)
- Fix Non-English JSON temperature unit attachement
- Fix Arilux RF induced exception by moving interrupt handler to iram on non ESP8266/Arduino lib v2.3.0
- Fix truncated command names and wrong response for DomoticzSwitchIdx (#1571)
- Fix %-sign issue as printf escape character in Humidity and Sonoff SC (#1579)
- Fix DS18B20 temperature JSON decimal dot (#1561)
- Fix Energy JSON message (#1621)
- Fix IRSend parameter translation (#1636)
- Fix TSL2561 device detection (#1644, #1825)
- Fix BME680 teleperiod resistance measuring (#1647)
- Fix Energy Monitoring Energy Today and Energy Total reading after restart (#1648)
- Fix IRReceive Data value (#1663)
- Fix Energy Monitoring Energy Period roll-over (#1688)
- Fix compiler warnings (#1774)
- Fix command PWM response if no PWM channel is configured (#1783)
- Add locale Decimal Separator to Web sensor page
- Add ColorTemperature to light status message
- Add command PowerOnState option 5 which inverts PulseTime and allows for delayed always on after power on
- Add OtaMagic two step Web server OTA upgrade using filename-minimal image if OTA free space is too small
- Add support for PMS5003 and PMS7003 particle concentration sensor
- Add command SetOption21 1 to allow Energy Monitoring when power is off on Sonoff Pow and Sonoff S31 (#1420)
- Add Chinese language file (#1551)
- Add French language file (#1561)
- Add Spanish language file (#1589)
- Add HTTP Allow Cross Origin removed from ESP8266/Arduino lib v2.4.0 (#1572)
- Add Home Assistant MQTT Discovery for switch and light to be enabled by command SetOption19 1 (#1534) or define HOME_ASSISTANT_DISCOVERY_ENABLE in user_config.h (#1685)
- Add command State to retrieve device state information (same data as teleperiod state and status 11 in slightly different JSON format)
- Add optional login to Webserver AP mode (#1587, #1635)
- Add command Sensor15 2 to start MHZ19(B) Zero Point Calibration (#1643)
- Add support for Sonoff S31 Smart Socket with Power Consumption Detection (#1626)
- Add command SetOption20 to allow update of Dimmer/Color/Ct without turning power on (#1719, #1741)
- Add NTP sync time slot based on chip id (#1773)
- Add cursor pointer to web button (#1836)

### 5.11.1 20180107

- Fix Sonoff Pow command handling (#1542)

### 5.11.0 20180107

- Minor webpage HTML optimizations (#1358)
- Updated German translation (#1438)
- Change Sonoff Pow Energy MQTT data message and consolidate Status 8 into Status 10
- Change ADS1115 default voltage range from +/-2V to +/-6V (#1289)
- Change text to Active for 3 minutes (#1364)
- Change Wemo SetBinaryState to distinguish from GetBinaryState (#1357)
- Change output of HTTP command to valid JSON and Array only (#1363)
- Removed all MQTT, JSON and Command language defines from locale files and set fixed to English (#1473)
- Renamed commands Color2,3,4 to Color3,4,5
- Fix BME280 calculation (#1051)
- Fix Sonoff Bridge missed learned key if learned data contains 0x55 (End of Transmission) flag (#1095, #1294)
- Fix PWM initialization in Dimmer/Color mode (#1321)
- Fix Wemo Emulation (#1357)
- Fix display of build date and time in non-english locale (#1465)
- Fix Wemo and Hue emulation by adding M-Search response delay (#1486)
- Add libraries Adafruit_BME680-1.0.5, Adafruit_Sensor-1.0.2.02, TasmotaSerial-1.0.0 and TSL2561-Arduino-Library
- Add command Color2 to set color while keeping same dimmer value
- Add device function pointers
- Add support for SenseAir S8 CO2 sensor
- Add color led signal to Carbon Dioxide (CO2) sensors using defines CO2_LOW and CO2_HIGH in user_config.h
- Add support for Domoticz Air Quality sensor to be used by MH-Z19(B) and SenseAir sensors
- Add support for PZEM004T energy sensor
- Add support for iTead SI7021 temperature and humidity sensor by consolidating DHT22 into AM2301 and using former DHT22 as SI7021 (#735)
- Add support for BME680 using adafruit libraries (#1212)
- Add support for MH-Z19(B) CO2 sensor (#561, #1248)
- Add multipress support and more user configurable GPIO to Sonoff Dual R2 (#1291)
- Add support for TSL2561 using adafruit library (#661, #1311)
- Add support for SHT3x (#1314)
- Add support for Arilux LC06 (#1414)
- Add Italian language file (#1449)
- Add 2nd Gen Alexa support to Wemo emulation discovery (#1357, #1450)
- Add define for additional number of WS2812 schemes (#1463)

### 5.10.0 20171201

- Upgrade library ArduinoJson to 5.11.2
- Upgrade library IRRemoteEsp8266 to 2.2.1 + 2 commits but disabled some protocols (code size reduction)
- Upgrade library NeoPixelBus to 2.2.9
- Upgrade library OneWire to 2.3.3 + 6 commits and disabled CRC lookup-table (#define ONEWIRE_CRC8_TABLE 0) (code size reduction)
- Update library PubSubClient to 2.6 + 9 commits and additional delay (#790)
- Update core_esp8266_wiring_digital.c to latest (staged) level
- Patch library I2Cdevlib-Core for esp8266-core 2.4.0-rc2 compatibility
- Remove command EnergyReset 1..3 now replaced by EnergyReset1 to EnergyReset3
- Remove spaces in JSON messages (code size reduction)
- Renamed xsns_05_ds18x20.ino to xsns_05_ds18x20_legacy.ino still using library OneWire and providing dynamic sensor scan
- Fix possible iram1_0_seg compile error by shrinking ICACHE_RAM_ATTR code usage
- Fix PWM watchdog timeout if Dimmer is set to 100 or Color set to 0xFF (#1146)
- Fix Sonoff Bridge Learn Mode hang caused by unrecognised RF code (#1181)
- Fix blank console log window by using XML character encoding (#1187)
- Fix wrong response name for command HlwISet (#1214)
- Fix DHT type sensor timeout recognition by distinguish "signal already there" from "timeout" (#1233)
- Add fixed color options 1..12 to command Color
- Add + (plus) and - (minus) to commands Dimmer (+10/-10), Speed and Scheme
- Add + (plus) and - (minus) to command Color to select 1 out of 12 preset colors
- Add + (plus) and - (minus) to command Ct to control ColdWarm led ColorTemperature (+34/-34)
- Add commands EnergyReset1 0..42500, EnergyReset2 0..42500 and EnergyReset3 0..42500000
-  to (Re)set Energy Today, Yesterday or Total respectively in Wh (#406, #685, #1202)
- Add optional ADS1115 driver as alternative for unsupported I2Cdevlib in esp8266-core 2.4.0-rc2
- Add support for INA219 Voltage and Current sensor to be enabled in user_config.h with define USE_INA219
- Add support for Arilux LC11 (Clearing RF home code when selecting no Arilux module)
- Add support for WS2812 RGBW ledstrips to be enabled in user_config.h with define USE_WS2812_CTYPE (#1156)
- Add SettingsSaveAll routine to command SaveData to be used before controlled power down (#1202)
- Add option PUSHBUTTON_TOGGLE (SwitchMode 7) to allow toggling on any switch change (#1221)
- Add new xdrv_05_ds18x20.ino free from library OneWire and add the following features:
-  Add support for DS1822
-  Add forced setting of 12-bit resolution for selected device types (#1222)
-  Add read temperature retry counter (#1215)
-  Fix lost sensors by performing sensor probe at restart only thereby removing dynamic sensor probe (#1215)
-  Fix sensor address sorting using ascending sort on sensor type followed by sensor address
-  Rewrite JSON resulting in shorter message allowing more sensors in default firmware image:
-   "DS18B20-1":{"Id":"00000483C23A","Temperature":19.5},"DS18B20-2":{"Id":"0000048EC44C","Temperature":19.6}
- Add additional define in user_config.h to select either single sensor (defines disabled), new multi sensor (USE_DS18X20) or legacy multi sensor (USE_DS18X20_LEGACY)
- Add clock support for more different pixel counts (#1226)
- Add support for Sonoff Dual R2 (#1249)
- Add FriendlyName to web page tab and add program information to web page footer (#1275)

### 5.9.1 20171107

- Add external sensor function pointer interface to enable easy sensor addition
- Add support for ADS1115 to be enabled in user_config.h and needs libraries i2cdevlib-Core and i2cdevlib-ADS1115 (#338, #660)
- Fix Backup Configuration file download failure by defining proper file size (#1115)
- Fix Exception 26 and empty console screen after usage of command WakeupDuration (#1133)
- Fix some changed iTead web links in README.md (#1137)

### 5.9.0 20171030

- Rewrite code (partly) using Google C++ Style Guide (https://google.github.io/styleguide/cppguide.html)
- Rewrite code by using command lookup tables and javascript (client side) web page expansions
- Change HTML/CSS to enable nicer form field entry
- Change default PWM assignments for H801 RGB(CW) led controller to support optional Color/Dimmer control
-   GPIO04 (W2)    from GPIO_PWM2 to GPIO_USER to be user configurable for GPIO_PWM5 (second White - Warm if W1 is Cold)
-   GPIO12 (Blue)  GPIO_PWM3 no change
-   GPIO13 (Green) from GPIO_PWM4 to GPIO_PWM2
-   GPIO14 (W1)    from GPIO_PWM1 to GPIO_USER to be user configurable for GPIO_PWM4 (first White - Cold or Warm)
-   GPIO15 (Red)   from GPIO_PWM5 to GPIO_PWM1
- Change default PWM assignments for MagicHome RGB(W) led controller to support optional Color/Dimmer control
-   GPIO05 (Green) from GPIO_PWM4 to GPIO_PWM2
-   GPIO12 (Blue)  from GPIO_PWM5 to GPIO_PWM3
-   GPIO13 (White) GPIO_USER to be user configurable for GPIO_PWM4 (White - Cold or Warm)
-   GPIO14 (Red)   from GPIO_PWM3 to GPIO_PWM1
- Change default PWM assignment for Witty Cloud to support optional Color/Dimmer control (#976)
-   GPIO12 (Green) from GPIO_PWM4 to GPIO_PWM2
-   GPIO13 (Blue)  from GPIO_PWM5 to GPIO_PWM3
-   GPIO15 (Red)   from GPIO_PWM3 to GPIO_PWM1
- Change when another module is selected now all GPIO user configuration is removed
- Change command name IRRemote to IRSend (#956)
- Remove Arduino IDE version too low warning as it interferes with platformio.ini platform = espressif8266_stage
- Fix command FullTopic entry when using serial or console interface
- Fix possible UDP syslog blocking
- Fix minimum TelePeriod of 10 seconds set by web page
- Fix command GPIOx JSON response (#897)
- Fix inverted relay power on state (#909)
- Fix compile error when DOMOTICZ_UPDATE_TIMER is not defined (#930)
- Fix alignment of web page items in some browsers (#935)
- Fix setting all saved power settings to Off when SetOption0 (SaveState) = 0 (#955)
- Fix timezone range from -12/12 to -13/13 (#968)
- Fix Southern Hemisphere TIME_STD/TIME_DST (#968)
- Fix TLS MQTT SSL fingerprint test (#970, #808)
- Fix virtual relay status message used with Color/Dimmer control (#989)
- Fix command IRSend and IRHvac case sensitive parameter regression introduced with version 5.8.0 (#993)
- Fix pressure calculation for some BMP versions regression introduced with version 5.8.0i (#974)
- Fix Domoticz Dimmer set to same level not powering on (#945)
- Fix Blocked Loop when erasing large flash using command reset 2 (#1002)
- Fix relay power control when light power control is also configured as regression from 5.8.0 (#1016)
- Fix Mqtt server mDNS lookup only when MqttHost name is empty (#1026)
- Add debug information to MQTT subscribe
- Add translations to I2Cscan
- Add translation to BH1750 unit lx
- Add light scheme options (Color cycle Up, Down, Random) and moving WS2812 schemes up by 3
- Add Domoticz counter sensor to IrReceive representing Received IR Protocol and Data
- Add option 0 to MqttHost to allow empty Mqtt host name
- Add support for Arilux AL-LC01 RGB Led controller (#370)
- Add esp8266 de-blocking to PubSubClient library (#790)
- Add Domoticz sensors for Voltage and Current (#903)
- Add platformio OTA upload support (#928, #934)
- Add warning to webpage when USE_MINIMAL is selected (#929)
- Add smoother movement of hour hand in WS2812 led clock (#936)
- Add support for Magic Home RGBW and some Arilux Led controllers (#940)
- Add command SetOption15 0 (default) for command PWM control or SetOption15 1 for commands Color/Dimmer control to PWM RGB(CW) leds (#941)
- Add Domoticz counter sensor to Sonoff Bridge representing Received RF code (#943)
- Add support for Luani HVIO board (https://luani.de/projekte/esp8266-hvio/) (#953)
- Add PWM initialization after restart (#955)
- Add IR Receiver support. Disable in user_config.h (#956)
- Add support for inverted PWM (#960)
- Add Sea level pressure calculation and Provide command Altitude (#974)
- Add support for up to 8 relays (#995)
- Add commands RfSync, RfLow, RfHigh, RfHost and RfCode to allow sending custom RF codes (#1001)
- Add retain to ENERGY messages controlled by command SensorRetain (#1013)
- Add commands Color2, Color3, Color4, Width2, Width3, Width4 and SetOption16 to set Ws2812 Clock parameters (#1019)
- Add German language file (#1022)
- Add support for connecting to MQTT brokers without userid and/or password (#1023)
- Add support for esp8266 core v2.4.0-rc2 (#1024)
- Add commands PwmRange 1,255..1023 and PwmFrequency 1,100..4000 (#1025)
- Add Polish language file (#1044, #1047)
- Add support for KMC 70011 Power Monitoring Smart Plug (#1045)
- Add support for VEML6070 I2C Ultra Violet level sensor (#1053)
- Add light turn Off Fade (#925)
- Add IrSend command option Panasonic as IrSend {"Protocol":"Panasonic", "Bits":16388, "Data":\<Panasonic data\>}
-   where 16388 is 0x4004 hexadecimal (#1014)
- Add retry counter to DHT11/21/22 sensors (#1082)

### 5.8.0 20170918

- Remove the need for NeoPixelBus library for Hue support
- Consolidate WS2812 into Sonoff Led for flexible future led strip library changes
- Invert WS2812 fade speed to align with Sonoff led (Speed 1 = fast, Speed 8 = slow)
- Remove upper case MQTT receive buffer
- Reduce code and string length for output of commands Modules and GPIOs
- Add Sonoff SC debug information
- Change syslog service
- Removed webserver syslog disable as now no longer needed
- Increased default MQTT message size from 368 to 405 bytes while keeping MQTT_MAX_PACKET_SIZE = 512 (because we can)
- Fix MQTT Offline or Remove MQTT retained topic code
- Fix Domoticz loop when Emulation is selected
- Add blink to WS2812 and Sonoff Led (#643)
- Add option WIFI_WAIT (5) to command WifiConfig to allow connection retry to same AP without restart or update flash (#772, #869)
- Add support for Witty Cloud (#794)
- Add GPIO14 to Sonoff Dual (#797, #839)
- Add support for Yunshan Wifi Relay (#802)
- Add GPIO16 input pulldown (#827)
- Add timeout to DHT and DS18B20 sensors (#852)
- Fix watchdog timeout caused by lack of stack space by moving to heap (#853)
- Allow LogPort and MqttPort up to 65535 and add LogPort tot Status 3 (#859)
- Allow command SwitchTopic in group mode (#861)
- Allow command SwitchMode if no switches are defined (#861)
- Add optional dimmer parameter to command Wakeup for WS2812, AiLight, Sonoff B1, Led and BN-SZ01 (#867)
- Fix basic On, Off, Toggle, Blink and BlinkOff commands when other language is selected (#874)

### 5.7.1 20170909

- Remove leading spaces from MQTT data
- Fix webconsole special character entry
- Allow # as prefix for color value
- Fix Alexa detection and Hue App Update Request (#698, #854)

### 5.7.0 20170907

- Shrink module configuration webpage
- Fix settings order during startup to allow for displaying debug messages
- Fix some string length issues
- Add more string length tests by using strncpy
- Add Ai-Thinker RGBW led (AiLight)
- Add Power check and add PulseTime to power check at startup (#526)
- Add Supla Espablo support (#755)
- Add more precision to Sonoff Pow period and power results using command WattRes 0|1 (#759)
- Add basic internationalization and localization (#763)
- Add more Sonoff Pow range checking (#772)
- Fix invalid JSON (#786, #822)
- Add duplicate check to received RF signal within 2 seconds for Sonoff Bridge (#810)

### 5.6.1 20170818

- Change module list order in webpage
- Fix Sonoff T1 1CH and 2CH configuration (#751)

### 5.6.0 20170818

- Fix Sonoff Pow intermittent exception 0
- Change Sonoff Pow sending Domoticz telemetry data only
- Add Ai-Thinker RGBW led (AiLight) (experimental)
- Add NeoPixelBus library to Sonoff Led for Hue support
- Add user configurable GPIO4 and GPIO5 to module Sonoff Bridge
- Add Sonoff B1 RGBCW led support with command Color RRGGBBCCWW (#676)
- Add command CT 152..500 to Sonoff Led and Sonoff B1 to control Color Temperature
- Add Cold-Warm slider to web page for Sonoff Led and Sonoff B1
- Add CT parameter to Hue
- Add Sonoff T1 support (#582)
- Add AnalogInput0 if configured as Analog Input to webpage (#697, #746)
- Add command SetOption14 0|1 to enable interlock mode (#719, #721)
- Fix Mitsubishi HVAC IR power controll (#740)

### 5.5.2 20170808

- Extent max number of WS2812 pixels from 256 to 512 (#667)
- Add OTA handling if server responds with no update available (#695)
- Removed undocumented command FlashMode (#696)
- Fix compile time error message due to increased message buffer size (#703)

### 5.5.1 20170805

- Fix Sonoff Rf Bridge issues
- Add Sonoff RF Bridge MQTT messages on received and learned RF signal
- Add command VoltRes 0|1 to select voltage resolution to 0.1 V (#654)
- Add averaging to Analog input (#686)
- Add Energy tele data on Sonoff Pow Threshold change (#688)
- Fix inconsistent property names in Messages (#690)

### 5.5.0 20170730

- Reduce code space by removing the following commands as they are replaced by SetOption alternatives:
-   SaveState = SetOption0
-   ButtonRestrict = SetOption1
-   Units = SetOption2
-   MQTT = SetOption3
-   MQTTResponse = SetOption4
-   TempUnit = SetOption8
- Smoothing WS2812 animation poll, invert fade speed and max allowed wakeup time down to 3000 seconds
- Fix initial button press detection
- Add support for Sonoff RF Bridge 433 using command RfKey
- Fix regression from 5.0.7 by increasing message buffer size from 360 to 368 to accomodate 4 x DS18x20 sensors (#637)
- Add GroupTopic to Topic test when using ButtonTopic/SwitchTopic to send either ON/OFF or TOGGLE (#642)
- Adjust HLW calibration limits to accomodate HuaFan device and add commands HlwPSet, HlwUSet and HlwISet (#654)

### 5.4.0 20170725

- Fix command reset regression introduced in 5.2.0
- Increase polling from 0.1 second to 0.05 second
- Add multipress to all buttons
- Fix button 1 double press behaviour on multi relay devices
- Add support for Hua Fan Smart Socket (#479)
- Add support for Sonoff 4ch Pro (#565)
- Add command SetOption13 1 to allow immediate action on single button press
-   (disables multipress, hold and unrestricted commands) (#587)

### 5.3.0 20170715

- Major Hue rewrite which might introduce Alexa problems. If so, initiate an issue
- Add support for Sonoff Led and BN-SZ01 Ceiling Led brightness control to Hue
- Fix Sonoff Led Power, Dimmer and Color MQTT response (#176)
- Add commands Delay and Backlog to allow multiple commands at once separated by ";" (#593)
- Use default flashmode DOUT to solve restart hangs on esp8285 chips (#453, #598)
- Change Web console column width from 99 to 300 (#599)

### 5.2.4 20170703

- Removed flash mode update after selecting different module solving esp8285 related problems
- Add device type flag to sonoff_template.ino
- Change Sonoff Led Wakeup and add support for Sonoff BN-SZ01 Led (#567)

### 5.2.3 20170630

- Change Sonoff Led color conversion code
- Fix SetOption12 handling
- Simplify auto configuration upgrade
- Add option Upgrade \<version_number\> to only upgrade to any higher version (Old PR #213)
- Change FallbackTopic to cmnd/\<MQTTClient\>/\<command\> \<parameter\> bypassing FullTopic and Prefix (#538)

### 5.2.2 20170625

- Add configuration SaveAddress to Status 1 and Information Page
- Change Sonoff Led Color conversion from AtoH to strtol
- Fix possible wrong uploads due to configuration overwrites (#542)
- Fix payload negative numbers (#547)

### 5.2.1 20170622

- Fix Restore Configuration in case of lower version
- Revert auto configuration upgrade allowing easy upgrade which was removed in version 5.2.0
- Fix config auto upgrade from versions below version 4.1.1 (#530)

### 5.2.0 20170619

- Add command SetOption12 1 to disable newly released configuration flash rotate to reduce flash wear
- Fix command CounterDebounce by removing test for active GPIO (#524)
- Add command SetOption33 1..250 to allow user configure POW Max_Power_Retry count (#525)

### 5.1.7 20170616

- Prep removal of SetOptions alternatives
- Restore webpage upgrade error messages removed in 5.1.5
- Add hold button functionality to buttons 2 to 4
- Add command SetOption32 1..100 to set Key Hold Time from 0.1 seconds to 10 seconds (#200)
- Allow slashes in Topic, GroupTopic, ButtonTopic and SwitchTopic (#507)
- Changed webpage form actions from post to get and use relative path url (#434, #522)

### 5.1.6 20170606

- Shrink code
- Removed online configuration of Domoticz In and Domoticz Out MQTT strings
- Removed commands DomoticzInTopic and DomoticzOutTopic
- Add define KEY_HOLD_TIME to configure button hold threshold before sending MQTT Hold message
- Add command StateText4 to configure button MQTT Hold text (= MQTT_CMND_HOLD)
- Add command SetOption11 0|1 to swap pushbutton single and double press functionality (#200)
- Add command SwitchMode<x> 5 (PUSHBUTTONHOLD) and 6 (PUSHBUTTONHOLD_INV) (#489)

### 5.1.5 20170604

- Shrink code in preparation to ESP8266-Arduino 2.4.0-rc1
- Add effect parameter to HUE Device (#464)

### 5.1.4 20170601

- Removed pre-compiled versions from repository as they are available within the release
- Changed HUE Device type to color supporting version (#464)
- Fix compile error when BE_MINIMAL is selected (#467, #476)
- Add multiple compiled versions to release using updated Travis script and platformio.ini (#467)

### 5.1.3 20170520

- Add Domoticz Counter

### 5.1.2 20170519

- Fix Counter/Timer JSON message and update Counter/Timer on webpage
- Fix WS2812 Domoticz related regression issues

### 5.1.1 20170517

- Allow command FullTopic in group mode
- Prepare for more use of RTC memory
- Add independant WS2812 led string power control (#386, #390)
- Add command Counter<x> to control up to four GPIO falling edge interrupt counters or timers (#459)
- Add command CounterType<x> to select between pulse counting or pulse timing
- Add command CounterDebounce to select global counter debounce time in mSec

### 5.1.0 20170513

- Fix Offline/Removal of retained topic when FullTopic is changed
- Add FullTopic to MQTT Configuration and Information web pages
- Add license model GPLv3 (#188)

### 5.0.7 20170511

- Fix possible exception 28 on empty command
- Add command SetOption0 as replacement for SaveState
- Add command SetOption1 as replacement for ButtonRestrict
- Add command SetOption2 as replacement for Units
- Add command SetOption4 as replacement for MqttResponse
- Add command SetOption8 as replacement for TempUnit
- Add command SetOption10 On|Off to select between Offline or Removing previous retained topic (#417, #436)

### 5.0.6 20170510

- Remove hyphen in case of a single DHT sensor connected (#427)
- Add command MqttRetry <seconds> to change default MQTT reconnect retry timer from minimal 10 seconds (#429)

### 5.0.5 20170508

- Add command FullTopic with tokens %topic% (replaced by command Topic value) and
-  %prefix% (replaced by command Prefix<x> values) for more flexible topic definitions (#244)
-  See wiki > MQTT Features https://github.com/arendst/Tasmota/wiki/MQTT-Features for more information

### 5.0.4 20170505

- Add Sonoff Pow Energy Total up to 40 MWh
- Add command EnergyReset 1|2|3 to reset Energy counters (#406)
- Fix Domoticz Energy logging (#411)
- Add command PowerOnState 4 to keep relay always on and disabling all power control (#418)

### 5.0.3 20170504

- Add command SensorRetain on|off to enable retaining of mqtt message tele/sonoff/SENSOR (#74)
- Change WifiConfig timeout from 60 seconds to 180 seconds (#212)
- Change Sonoff Touch command Ledstate functionality by turning led on if power is off (#214)
- Add 4 seconds delay after power on before enabling button to workaround Wemos D1 mini RTS circuit (#380)

### 5.0.2 20170503

- Reset SaveData, SaveState and MqttResponse to default values due to rearranging settings
- Moved some settings to flag area
- Add command TempUnit Celsius|Fahrenheit for selecting Celsius or Fahrenheit (#347)
- Add command TempRes 0..3 for selecting Temperature Resolution (#347)
- Add command HumRes 0..3 for selecting Humidity Resolution (#347)
- Add command PressRes 0..3 for selecting Pressure Resolution (#347)
- Add command EnergyRes 0..5 for selecting Energy Resolution (#347)
- Add "TempUnit":"C|F" to sensor JSON output (#347)
- Add support for up to three DHT type sensors each using a different GPIO (#339, #404)

### 5.0.1 20170429

- Adjust Sonoff SC messages to prepare for display feature
- Move static data from RAM to Flash
- Fix PowerOnState for some devices not reporting "Power on" state (#284, #380, #383)

### 5.0.0 20170425

- Memory status message update
- Fix setting migration to better preserve settings during move (#382)
- Best practice is first doing a Backup Configuration before installing version 5.0.0
- Reset save count after setting move
- Start using new linker script without SPIFFS

### 4.2.0 20170424

- Prepare for SPIFFS removal by moving settings to EEPROM area
- Fix compilation error when webserver is disabled (#378)

### 4.1.3 20170410

- Add user configuarble GPIO to module S20 Socket and Slampher
- Add support for Sonoff SC (#112)
- Set PWM frequency from 1000Hz to 910Hz as used on iTead Sonoff Led firmware (#122)
- Set Sonoff Led unconfigured floating outputs to 0 to reduce exceptions due to power supply instabilities (#122)
- Add Access Point Mac Address to Status 11 and Telemetry (#329)
- Fix DS18B20 negative temperature readings (#334)

### 4.1.2 20170403

- Rename Unrecognised command to Unknown command
- Remove all command lists
- Remove command SmartConfig (superseded by WifiConfig)
- Fix boot loop when selecting module Sonoff 4CH or Sonoff Touch on non ESP8285 hardware
- Add optional support for Toshiba and Mitsubishi HVAC IR control (needs updated IRremote8266 library) (#83, #257)
- Add all configured switches to Domoticz Configuration web page (#305)
- Fix compile error when selecting WS2812 DMA (#313)

### 4.1.1 20170329

- Fix default Telemetry for command Prefix3
- Fix webserver Module parameters for disabled select
- Fix sensor status for enabled switches
- Remove Light as alternative for Power (save code space)
- Remove migration option from pre V3 (code cleanup)
- Remove unofficial SPIFFS support (code cleanup)
- Remove command list when unknown command is entered (save code space)
- Rename Status11 json from StatusPWR to unique StatusSTS
- Rename command Gateway to IPAddres2, Subnetmask to IPAddress3 and DnsServer to IPAddress4 (save code space)
- Add Command MqttResponse to select either command or RESULT topic as response (#258)
- Add command StateText1 to StateText3 to assign MQTT_STATUS_OFF, MQTT_STATUS_ON and MQTT_CMND_TOGGLE respectively (#286)
- Remove restart after IPAddress changes (#292)
- Add support for MAX31850 in xsns_ds18x20.ino (#295)
- Fix possible uptime update misses (#302)

### 4.1.0 20170325

- Change static IP addresses in user_config.h from list (using commas) to string (using dots)
- Unify display result of commands Modules, Module and Gpios
- Rewrite Module selection web page to bring size down from 18651 to 4319 bytes (!) (#234, #240)
- Add basic support for (Lixada) H801 RGBWW controller (#252)
- Add command Prefix1 to Prefix3 to assign SUB_PREFIX, PUB_PREFIX and PUB_PREFIX2 respectively (#255)
- Add static ip addresses to flash (#262)
- Add commands IpAddress, Gateway, Subnetmask and DnsServer to select static ip addresses (#273)

### 4.0.8 20170321

- Fix entering non-numeric webpassword
- Force selection between TLS or Webserver due to memory restraint (#240)
- Allow entering empty string using "0" for selected commands (#242)
- Fix exception when posting commands to web console containing % (#250)

### 4.0.7 20170319

- Increased Sonoff Led PWM frequency from 432 to 1000
- Fix possible watch dog reboot after changing module type on web page
- Fix reporting of GPIO usage from web page
- Fix Sonoff Led blank during firmware upgrade
- Fix Sonoff Led flicker and possible flash corruption by using latest Arduino-esp8266 versions of pwm core files included in sonoff library (#211)
- Add PWM output control with commands PWM1 to PWM5 using user selectable GPIOs (#211)
- Fix exceptions due to low values of commands HlwPCal (10000), HlwUCal (1000) and HlwICal (2500) (#223)
- Add Switch state to sensor status (#227, #233)
- Add user configuarble GPIO to module Sonoff Touch (#228)
- Add define WEB_PORT to user_config.h to change default web server port from 80 (#232)
- Fix failed Ota Firmware upgrade started from Web page (#235)

### 4.0.6 20170316

- Fix to better find device by Wifi hostname
- Fix compile error when some I2C devices are disabled
- Add (experimental) support for SHT1X emulating I2C (#97)
- Add ADC to ElectroDragon (#203)
- Add support for Sonoff Dev (#206)

### 4.0.5 20170314

- Add command Status 11 to show power status with Vcc if define USE_ADC_VCC is enabled (default)
- Add ADC input to Sonoff SV and Wemos D1 mini - Needs recompile with define USE_ADC_VCC disabled (#137)
- Add MQTT host:port to timeout message (#199)

### 4.0.4 20170312

- Add pulse timers for up to 4 relays (#106)
- Fix Sonoff Led power state when dimmer or color is 0 (#176)
- Add command NtpServer<x> to configure up to three NTP servers (#177)
- Delete module User Test as module Wemos D1 mini has same/more user configurable GPIO (#178)
- Add more user configurable GPIO to module ElectroDragon (#183)

### 4.0.3 20170309

- Renamed Module NodeMCU to WeMos D1 mini
- Add GPIO1 as user option to some modules
- Add Buttons, Relays and Leds to user configurable options (#159)
- Add description on Module parameters web page to some well known GPIOs (#107, #171)

### 4.0.2 20170308

- Restore correct seriallog level after Serial logging was disabled
- Add simple dimmer slider to Sonoff Led web page
- Reduced root webpage size by 31%
- Expand Status 2 with Build date/time and core version
- Fix webserver redirection when not in WifiManager mode (#156)
- Add command ButtonRestrict On/Off to restrict access to button hold and button multi press options above 2 (#161)
- Fix DS18S20 negative temperature readings (#165)
- Fix crlf compilation error due to bad syntax (#144, #167)

### 4.0.1 20170305

- Fix char default sizes and set MESSZ to 360 (#143)
- Fix SerialLog setting status
- Disable syslog when emulation is active
- Add DS18B20 web page display refresh

### 4.0.0 20170303

- Add define to remove config migration code for versions below 3.0 (See Wiki-Upgrade-Migration path)
- Free memory by switching from String to char[]
- Raised Sonoff Led PWM frequency from 200Hz to 432Hz in search of stability (hardware watchdog timeouts) (#122)
- Increase message size and suggested minimum MQTT_MAX_PACKET_SIZE to 512 (#114, #124)
- Remove runtime warning message regarding MQTT_MAX_PACKET_SIZE too small as it is now moved to compile time (#124)
- Fix possible panics with web console and http commands while UDP syslog is active (#127)
- Add optional static IP address (#129)
- Add define ENERGY_RESOLUTION in user_config.h to allow user control over precision (#136)

### 3.9.22 20170228

- Update web console
- Fix Status 4 JSON message
- Add Exception info during restart if available
- Add osWatch service to detect loop hangs that might happen during (OTA) upgrades
- Add WiOn support for relay and switch only (#82, #102)
- Allow for user specified relay count up to four in sonoff_template.h (#109)
- Add support for HTU21 compatible I2C sensors SI7013, SI7020 and SI7021 (#118)
- Add NodeMCU or Wemos configuration option (#119)

### 3.9.21 20170224

- Add ajax to web root page and web console (#79)
- Add commands SwitchMode1..4 and enable user switches 2, 3 and 4 (#84, #88)
- Fix MQTT upgrade when webserver is active

### 3.9.20 20170221

- Add minimal basic authentication to Web Admin mode (#87)
- Fix Hue and add HSB support (#89)

### 3.9.19 20170219

- Sonoff Led: Made GPIO04, 05 and 15 available for user
- Sonoff Led: Add commands Fade, Speed, WakupDuration, Wakeup and LedTable

### 3.9.18 20170218

- Fix ledstate 0 to turn off led
- Fix Sonoff Led dimmer range (#16)
- Change Sonoff Led command Dimmer to act on both cold and warm color
- Add Sonoff Led command Color CCWW where CCWW are hexadecimal values fro 00 - FF
- Reduce Sonoff Led flickering by disabling interrupts during flash save and disabling
-   Led during OTA upgrade and Web upload (#16)

### 3.9.17 20170217

- Fix possible ArduinoJSON related memory fragmentation
- Changed console logging using less memory
- Add GPIO04 as user selectable for Sonoff Dual (#75)

### 3.9.16 20170214

- Update latching relay handler
- Add support for IR led using IRremoteESP8266 library (#59)
- Add Hue argument passing using ArduinoJSON library (#59)

### 3.9.15 20170213

- Change JSON float values from string to number according to http://json.org (#56)
- Add support for exs latched relay module https://ex-store.de/ESP8266-WiFi-Relay-V31 (#58)
- Add support for inverted relays
- Changed MAX_LOG_LINES from 70 to 60 to preserve memory

### 3.9.14 20170211

- Add False and True as alternatives for 0/Off and 1/On (#49)
- Fix Status10 JSON format (#52)
- Fix DS18x20 using OneWire library (#53)

### 3.9.13 20170210

- Add FlashChipMode to Status 4
- Removed redundant DHT2 option and code
- Add Sonoff SV GPIO pin 05 configuration (#40)
- Add configuration file backup and restore via web page
- Fix latency due to light_sleep mode even if sleep was set to zero (#50)

### 3.9.12 20170208

- Fix compile error when webserver is disabled (#30)
- Fix possible ESP8285 flash problem by updating Flash Chip Mode to DOUT during OTA upload
- Fix hostname issues by not allowing user entry of string formatting and removing from user_config.h (#36)

### 3.9.11 20170204

- Fix command I2Cscan
- Fix not allowed spaces in Topic, ButtonTopic and SwitchTopic
- Make all TELEMETRY, STATUS and COMMAND message topics unique (#4)
- Advertise command topic to be used by iobroker (#299)
- Fix butten (non)detection if no GPIO_KEY1 is defined (#13)
- Change WeMo serialnumber from 7 decimal chars to 8 hexadecimal chars (#18)
- Update web page with Build Date/Time, Emulation and mDNS Discovery and Advertise information (#21)

### 3.9.10 20170130

- Add WS2812 Color Type selection (RGB or GRB) to user_config.h (#7)
- Hue api changes to support HUE App(s) (#8)

### 3.9.9 20170130

- Add command status 10 showing sensor data
- Fix hlw status messages if hlw is disabled

### 3.9.8 20170130

- Remove GPIO07 and GPIO08 from user selectable (#5)

### 3.9.7 20170129

- Fix possible WS2812 exceptions when using emulation
- Add command Emulation to dynamic configure Belkin WeMo and Hue Bridge for Alexa

### 3.9.6 20170129

- Add dynamic sleep for WS2812 animation (#1)

### 3.9.5 20170128

- Fix error message in case of wrong Domoticz command

### 3.9.4 20170127

- Fix Sonoff Dual Relay switching (#287)

### 3.9.3 20170127

- Add confirmation before Restart via webpage
- Expand Domoticz Configuration webpage with Key, Switch and Sensor Index and
-   add commands DomoticzSwitchIdx and DomoticzSensorIdx (#86) (#174) (#219)
- Fix default DHT11 sensor driver selection
- Fix LedPower status after button press (#279)
- Add command Sleep 0 - 250 mSec for optional light sleep mode to lower energy consumption (#272)
-   (Expect overall button/key/switch misses and wrong values on Sonoff Pow)
- Add Hue brightness extension (#281)
- Fix Hue brightness and change to call by reference (#283)

### 3.9.2 20170124

- Add confirmation before Reset Configuration via webpage (#244)
- Add WS2812 features (see Wiki commands)

### 3.9.1 20170124

- Change PowerOnState function to only trigger when Power On (and not just restart) (#238)
- Move HLW interrupts back to RAM and make WS2812_DMA optional as it generates Exception on Pow (#264)
- Add charset=utf-8 to webpages (#266)
- Update Hue emulation (#268)
- Fix status module number
- Add support for domoticz Dimmer on Sonoff_Led and WS2812
- Fix possible ESP8285 flash problem by updating Flash Chip Mode to DOUT during web upload

### 3.2.6a 20170120

- Fix Sonoff Pow compile error (#255)
- Move HLW interrupts back to ROM (Needed for WS2812 DMA interrupts)
- Removed all IO config from user_config.h as this will be done by commands or webpage
- Removed MessageFormat and supports JSON only except POWER/LIGHT status
- Add command LedPower to control main led (#247)
- Add more FriendlyNames for Hue (#254)
- Add DMA support for WS2812 when using pin 3 while other pins work just as well in my case...
- Add HUE emulation for Alexa (#229)
- Add basic WS2812 support (#229)
- Fix Wemo when MQTT is disabled (#245)
- Revert ButtonTopic and change SwitchTopic1 - 4 to one SwitchTopic
- Rename MqttUnits to Units
- Add Mqtt command to enable/disable MQTT

### 3.2.2a 20170115

- Add dynamic (Sonoff) Module, user GPIO and sensor selection (one size fits (almost) all)
- Add support for Sonoff LED
- Add Seriallog disable after 600 seconds for Sonoff Dual and 4 Channel
- Add ButtonTopic2 - 4, SwitchTopic1 - 4 and SwitchRetain

### 3.2.2 20170113

- Fix PowerOnState 2 functionality after re-applying power (#230)

### 3.2.1 20170113

- Fix some failed command decoding (#228)
- Removed passwords from status messages (#216)

### 3.2.0 20170111

- Add I2C BH1750 sensor (#222)
- Sensor rewrite preparing for online selection

### 3.1.16 20170109

- Fix Domoticz possible error condition
- Remove Wifi password from connection message (#216)
- Add Configure Other menu item to web page (#209)
- Add command FriendlyName, field Friendly Name and define FRIENDLY_NAME to be used by Alexa
-   eliminating current use of MQTT_CLIENT_ID (#209)
- Add friendlyname to webpage replacing former hostname

### 3.1.15 20170108

- Fix Domoticz send key regression with Toggle command

### 3.1.14 20170107

- Add support for command TOGGLE (define MQTT_CMND_TOGGLE) when ButtonTopic is in use and not equal to Topic (#207)

### 3.1.13 20170107

- Fix web console command input when SUB_PREFIX contains '/' (#152)
- Add command response to web command (#200)
- Add option to disable MQTT as define USE_MQTT in user_config.h (#200)

### 3.1.12 20170106

- Add OTA retry to solve possible HTTP transient errors (#204)
- Fix MQTT host discovery

### 3.1.11 20170105

- Add mDNS to advertise webserver as <hostname>.local/

### 3.1.10 20170105

- Fix ButtonTopic when SUB_PREFIX = PUB_PREFIX
- Add workaround for possible MQTT queueing when SUB_PREFIX = PUB_PREFIX
- Add optional MQTT host discovery using define USE_DISCOVERY in user_config.h (#115)

### 3.1.9 20170104

- Fix Power Blink start position (toggled)
- Change PulseTime increments: 1 .. 111 in 0.1 sec (max 11 seconds) and 112 .. 64900 in seconds (= 12 seconds until 18 hours) (#188)
- Add support for SUB_PREFIX = PUB_PREFIX (#190)

### 3.1.8 20170103

- Add retain flag to LWT offline and only send "tele/sonoff/LWT Offline" (#179)
- Change retained LWT Online message to only send "tele/sonoff/LWT Online"

### 3.1.7 20161231

- Add retained message LWT Online when sonoff makes MQTT connection (#179)

### 3.1.6 20161230

- Add blinking using commands BlinkTime, BlinkCount and Power Blink|3|BlinkOff|4 (#165)

### 3.1.5 20161228

- Fix serial space command exception (28)

### 3.1.4 20161227

- Fix MQTT subscribe regression exception (3) (#162)
- Fix serial empty command exception (28)

### 3.1.3 20161225

- Extent Domoticz configuration webpage with optional indices (#153)
- Fix multi relay legacy tele message from tele/sonoff/2/POWER to tele/sonoff/POWER2
- Add support for iTead Motor Clockwise/Anticlockwise

### 3.1.2 20161224

- Extent command PowerOnState with toggle at power on (option 2 is now option 3!) (#156)

### 3.1.1 20161223

- Add support for Sonoff Touch and Sonoff 4CH (#40)
- Update DomoticzIdx and DomoticzKeyIdx with relay/key index (DomoticzIdx1/DomoticzKeyIdx1)
- Add command PowerOnState to control relay(s) at power on (#154)

### 3.1.0 20161221

- Add Sonoff Pow measurement smoothing
- Fix serial command topic preamble error (#151)
- Fix 2.x to 3.x migration inconsistencies (#146)

### 3.0.9 20161218

- Add Sonoff Pow voltage reading when relay is on but no load present (#123)

### 3.0.8 20161218

- Add temperature conversion to Fahrenheit as option in user_config.h (TEMP_CONVERSION) (#145)

### 3.0.7 20161217

- Add user_config_override.h to be used by user to override some defaults in user_config.h (#58)
- Fix Sonoff Pow low power (down to 4W) intermittent measurements (#123)

### 3.0.6 20161217

- Fix MQTT_CLIENT_ID starting with % sign as in "%06X" (#142)
- Add auto power off after PulseTime### 0.1 Sec to relay 1 (#134)

### 3.0.5 20161215

- Add more control over LED with command LedState options (#136, #143)
-   LED_OFF (0), LED_POWER (1), LED_MQTTSUB (2), LED_POWER_MQTTSUB (3), LED_MQTTPUB (4), LED_POWER_MQTTPUB (5), LED_MQTT (6), LED_POWER_MQTT (7)
- Add option WIFI_RETRY (4) to command WifiConfig to allow connection retry to other AP without restart (#73)

### 3.0.4 20161211

- Fix intermittent Domoticz update misses (#133)

### 3.0.3 20161210

- Fix compiler warnings (#132)
- Remove redundant code
- Fix Domoticz pushbutton support

### 3.0.2 20161209

- Add pushbutton to SwitchMode (#130)

### 3.0.1 20161209

- Fix initial config

### 3.0.0 20161208

- Migrate and clean-up flash layout
-   Settings from version 2.x are saved but settings from version 3.x can not be used with version 2.x
- Change SEND_TELEMETRY_RSSI to SEND_TELEMETRY_WIFI and add AP and SSID to telemetry
- Split long JSON messages
- Fix inconsistent status messages
- Fix all status messages to return JSON if enabled
- Remove relay index in cmnd/sonoff/<relay>/POWER now changed
-   to cmnd/sonoff/POWER for single relay units
-   and cmnd/sonoff/POWER<relay> for multi relay units like Sonoff dual
- Add retain option to Power/Light status controlled by command PowerRetain On|Off (#126)

### 2.1.2 20161204

- Add support for second wifi AP (#73)
- Update command WifiConfig
- Fix possible WifiManager hang

### 2.1.1a 20161203

- Fix scan for wifi networks if WeMo is enabled
- Fix syslog setting using web page

### 2.1.1 20161202

- Add support for ElectroDragon second relay and button (only toggle with optional ButtonTopic) (#110)

### 2.1.0 20161202

- Add optional EXPERIMENTAL TLS to MQTT (#49)
- Fix MQTT payload handling (#111)
- Optimzed WeMo code

### 2.0.21a 20161201

- Fix WeMo PowerPlug emulation

### 2.0.21 20161130

- Add Belkin WeMo PowerPlug emulation enabled with USE_WEMO_EMULATION in user_config.h (Heiko Krupp) (#105, #109)

### 2.0.20 20161130

- Relax MQTTClient naming but only allows hexadecimal uppercase numbers (#107)
- Add I2C support with command I2CScan
- Add I2C sensor driver for HTU21 as alternate sensor using TH10/16 connectors (Heiko Krupp) (#105)
- Add I2C sensor driver for BMP085/BMP180/BMP280/BME280 as alternate sensor using TH10/16 connectors

### 2.0.19a 20161127

- Add support for ButtonTopic and ButtonRetain to wall switch function
- Add pullup to SWITCH_PIN and command SwitchMode to syntax

### 2.0.18 20161126

- Add SUB_PREFIX multi level support allowing 'cmnd' or 'cmnd/level2/level3'
- Add wall switch function to GPIO14 and command SwitchMode (Alex Scott) (#103)

### 2.0.17 20161123

- Calibrate HLWPCAL from 12345 to 12530
- Add alternative sensor driver DHT2 using Adafruit DHT library
- Add define MESSAGE_FORMAT to user_config.h
- Throttle console messages
- Shorten JSON messages
- Fix possible Panic
- Fix User mode webserver security

### 2.0.16 20161118

- Add alternative sensor driver DS18x20 using OneWire library (#95)
- Change sensor MQTT message from tele/sonoff/TEMPERATURE to tele/sonoff/DHT/TEMPERATURE or
-   tele/sonoff/DS18B20/TEMPERATURE or tele/sonoff/DS18x20/1/TEMPERATURE
- Add sensors to root webpage and auto refresh every 4 seconds (#92)
- Add optional JSON messageformat to all telemetry data
- Enforce minimum TelePeriod to be 10 seconds
- Fix Energy Yesterday reset after restart
- Add Energy Today restore after controlled restart

### 2.0.15 20161116

- Change TODAY_POWER and PERIOD_POWER to TODAY_ENERGY and PERIOD_ENERGY
- Fix serial regression
- Fix syslog hangs when loghost is unavailable

### 2.0.14 20161115

- Add HLW threshold delay
- Fix HLW intermittent current deviation
- Fix button functionality during wificonfig
- Add CRC check to DS18B20 sensor (#88)

### 2.0.13 20161113

- Add additional upload error code descriptions
- Add PlatformIO support (#80)

### 2.0.12 20161113

- Fix Serial and Web response regression when no MQTT connection available
- Fix Sonoff Dual power telemetric data for second relay
- Removed MQTT password from Information web page
- Hide MQTT password from Configure MQTT web page

### 2.0.11 20161111

- Rewrite button and web toggle code
- Fix NTP sync
- Add HLW calibration commands HLWPCAL, HLWUCAL and HLWICAL (need define USE_POWERCALIBRATION)
- Fix power threshold tests

### 2.0.10 20161109

- Add additional Domoticz define (#63)
- Add defines MQTT_STATUS_ON and MQTT_STATUS_OFF in user_config.h to select status On/Off string
- Fix status response differences (#65)
- Fix divide by zero exception (#70)
- Fix syslog loop exception

### 2.0.9 20161108

- clarify MODULE in user_config.h
- Fix hlw false values

### 2.0.8 20161108

- Add initial status after power on
- Seperate driver files
- Fix hlw code and calibrate Pow
- Move user config defines to user_config.h (#61)

### 2.0.7 20161030

- Make Ticker mandatory
- Add Domoticz support (Increase MQTT_MAX_PACKET_SIZE to 400) (#54)
- Add command MessageFormat 0|1 to select either legacy or JSON output

### 2.0.6 20161024

- Add Sonoff Pow power factor
- Initial support for up to four relays using iTEAD PSB (4Channel)
-   - Currently only supports one button (All buttons behave the same)
-   - Use command MODEL 4 to select four relay option
-     (After first power on it will support 2 relays like Sonoff Dual)
- Fix ledstate
- Add command Status 9 to display Sonoff Pow thresholds
- Add commands PowerLow, PowerHigh, VoltageLow, VoltageHigh, CurrentLow and CurrentHigh for use
-   with Sonoff Pow thresholds

### 2.0.5 20161018

- Add updates to user_config.h - moved SEND_TELEMETRY_DS18B20 and SEND_TELEMETRY_DHT to module area.
-   As Sonoff TH10/16 does not have the logic installed for GPIO04 You'll have to select ONE of both
- Add Sonoff Pow support (experimental until Pow tested)
- Add command Status 8 to display Sonoff Pow energy values
- Add command MqttUnits On|Off to add units to values
- Change web main page header character size
- Change On/Off to ON/OFF status messages to satisfy openHAB
- Change TEMP to TEMPERATURE and HUM to HUMIDITY

### 2.0.4 20161009

- Add MQTT_BUTTON_RETAIN, SAVE_DATA and SAVE_STATE defines to user_config.h (#35)
- Update ButtonRetain to remove retained message(s) from broker when turned off
- Add Retain for second relay on Sonoff Dual
- Provide power status messages with device topic index if requested

### 2.0.3 20161008

- Update wifi initialization
- Add command BUTTONRETAIN for optional MQTT retain on button press (#35)
- Add command SAVESTATE to disable power state save. May be used with MQTT retain

### 2.0.2 20161006

- Fix wifi issue 2186

### 2.0.1 20161002

- Fix button press

### 2.0.0 20161002

- Update Sonoff TH10/16 sensor pins (My TH10 only has GPIO14 connected)
- Add full support for Sonoff dual

### 1.0.35 20160929

- Add more lines to console
- Add timeout and disable MQTT on web upload
- Add command SAVEDATA to control parameter save (for flash wear afficionados) (#30)

### 1.0.34 20160926

- Fix button press six and seven
- Add more information to webserver

### 1.0.33 20160915

- Better WPS error message
- Separate webserver code from support.ino into webserver.ino
- Fix webserver User by removing unwanted restart option

### 1.0.32 20160913

- Add Wifi Protected Setup (WPS) as third option for initial config
- Add command WIFICONFIG replacing deprecated command SMARTCONFIG
- Add option WIFICONFIG 3 to start WPSconfig
- Add option WIFICONFIG 0 to start saved Wifi config tool (WPSconfig, Smartconfig or Wifimanager)
- Change button behaviour - See Wiki

### 1.0.31 20160907

- Fix DS18B20 misread if teleperiod = 2
- Tuned sensor code
- Updated prefered ElectroDragon connection to Relay 1 and Button 1
- Moved SONOFF and ELECTRO_DRAGON port config to user_config.h

### 1.0.30 20160902

- Fix command TELEPERIOD 0
- Add ESP- tag to UDP log message for easy rsyslogd filtering
- Add ElectroDragon (Relay 2 only) functionality. Select with #define MODULE ELECTRO_DRAGON
- Add ? as null message alternative
- Add DHT temperature and humidity telemetry support. Enable with #define SEND_TELEMETRY_DHT
- Add DS18B20 temperature telemetry support. Enable with #define SEND_TELEMETRY_DS18B20
- Restrict HOSTNAME, MQTTCLIENT, TOPIC and BUTTONTOPIC in topic mode only

### 1.0.29 20160831

- Allow UPGRADE, OTAURL, RESTART, RESET, MQTTHOST, MQTTPORT, MQTTUSER, MQTTPASSWORD and WEBSERVER also in group mode

### 1.0.28 20160831

- Add webserver state to status 5
- Add optional PUB_PREFIX2 (tele) for telemetry usage
- Add command TELEPERIOD
- Fix syntax message
- Change memory status display

### 1.0.27 20160831

- Add sketch flash size
- Add console to webserver
- Add command weblog
- Change WifiManager web pages to minimal
- Change display default hostname and MQTT client id in webserver
- Change HTTP command interface to http://sonoff-1234/cm?cmnd=light 2
- Change HEARTBEAT to UPTIME

### 1.0.26 20160829

- Add define USE_WEBSERVER to disable web server code in source
- Add file upload as alternative for ota upload to webserver
- Add information to webserver
- Add command hostname
- Add command logport
- Change HTTP command interface to http://sonoff-1234/cmd?cmnd=light 2
- Change button behaviour with regards to Smartconfig and OTA upload. See README.md
- Enforce default hostname to either "%s-%04d" or user defined without any %
- Enforce default mqtt client id to either "DVES_%06X" or user defined without any %

### 1.0.25 20160822

- Remove config system halts to keep ota available

### 1.0.24 20160821

- Add test for MQTT_SUBTOPIC
- Change log range to LOG_LEVEL_ALL
- Change MQTT introduction messages
- Moved MQTT_MAX_PACKET_SIZE warning message to introduction messages

### 1.0.23 20160821

- Add option USE_SPIFFS to move config from flash to spiffs
- Add webserver with options 0 (off), 1 (user) and 2 (admin)
- Add HTTP command interface (http://sonoff-1234/c?cmnd=light 2)
- Add wifimanager countdown counter
- Add command line webpage
- Add relay control to wifimanager
- Add restart option 99 to force restart
- Fix wifi hostname
- Fix NETBIOS hostname problem by reducing default hostname length
- Fix possible exception if WIFI_HOSTNAME is changed
- Fix upgrade messages
- Reduce memory use by redesigning config routines
- Split syntax message
- Rename define SERIAL_IO to USE_SERIAL

### 1.0.22 20160814

- Add all MQTT parameters for configuration
- Add wifimanager to configure Wifi and MQTT via web server
- Change NTP time handling
- Fix Smartconfig parameter buffer overflow
- Fix PlatformIO warnings

### 1.0.21 20160808

- Remove semaphore as subscription flooding (more than 15 subscriptions per second) is managed by SDK (LmacRxBlk:1)
- Add optional RTC interrupt (define USE_TICKER) to keep RTC synced during subscription flooding
- Remove heartbeatflag

### 1.0.20 20160805

- Add semaphore to handle out of memory when too many subscriptions requested
- Use Daylight Saving (DST) parameters from user_config.h when timezone = 99
- Add status 7 option displaying RTC information
- Add ledstate to status 0

### 1.0.19 20160803

- Fix possible MQTT_CLIENT_ID induced Exception(28)

### 1.0.18 20160803

- Moved Cfg_Default
- Fix negative data handling
- Remove MQTT information from status 1 and add labels to status 1
- Add mac address to status 5
- Add MQTT ClientId, UserId and Password to status 6

### 1.0.17 20160731

- Better variable range checking
- Change ambiguous connection messages
- Add timestamp to serial message

### 1.0.16 20160729

- Moved wifi, rtc, syslog and config to support.ino
- Fixed button action when buttontopic is used. Introduced with 1.0.15
- Better buffer overflow checks (strlcpy)

### 1.0.15 20160728

- Removed pubsubclient config changes from sonoff.ino as it doesn't work
-   reapply MQTT_MAX_PACKET_SIZE 256 and MQTT_KEEPALIVE 120 to PubSubClient.h
- Add status 0 option displaying all status messages
- Change MQTT_MAX_PACKET_SIZE from 1024 to 256
- Add buffer overflow checks (snprintf and strncpy)
- Implemented common string sizes

### 1.0.14 20160722

- Seperate user config from sonoff.ino to user_config.h (pucebaboon)
- Change defaults from sidnas2 to domus1
- Add MQTT status message as status 6 (pucebaboon)
- Add status type to message (pucebaboon)
- Add pubsubclient config changes to sonoff.ino (pucebaboon)

### 1.0.13 20160702

- Add Ledstate 1 option to show power state on led

### 1.0.12 20160529

- Allow disable of button topic using "0"

### 1.0.11 20160524

- Provide button response if MQTT connection lost

### 1.0.10 20160520

- Add optional button topic to assist external MQTT clients
- Change version notation
- Reset default values

### 1.0.9  20160503

- Add more blinks
- Add reset 2 option erasing flash
- Add status 5 option displaying network info
- Add syslog check for Wifi connection
- Resize MqttPublish log array
- Change Wifi smartconfig active from 100 to 60 seconds
- Update Wifi initialization

### 1.0.8  20160430

- Remove use of Wifi config data from SDK
- Add status 3 (syslog info) and status 4 (flash info)
- Add restart option to button (5 quick presses)

### 1.0.7  20160420

- Add UDP syslog support
- Change HOST command to MQTTHOST command
- Add commands SYSLOG, SERIALLOG and LOGHOST
- Change hostname to lower case to distinguise between open-sdk version
- Add support for ESP-12F used in my modified wkaku power socket switch
- Fix timezone command
- Add RTC month names for future use
- Modify button code
- Remove initialization errors by better use of MQTT loop

### 1.0.6  20160406

- Removed Wifi AP mode (#1)
- Add test for Arduino IDE version >= 1.6.8
- Fix RTC time sync code

### 1.0.5  20160310

- Initial public release
- Show debug info by selecting option from IDE Tools Debug port: Serial
