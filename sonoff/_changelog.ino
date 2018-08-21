/* 6.1.1.7
 * Add initial display support for Lcd, Oled, Matrix, Tft and e-paper - Need more docs
 * Fix SDM120 reporting wrong negative values to Domoticz (#3521)
 * Fix iFan02 power on state (#3412, #3530)
 * Add display define USE_DISPLAY_MODES1TO5 to select display modes 1 to 5
 * Add command DisplayRotate 0..3 to select persistent display rotation
 *
 * 6.1.1.6
 * Add modulo option to rules like rule1 on Time#Minute|5 do backlog power on;delay 200;power off endon (#3466)
 *
 * 6.1.1.5
 * Fix some Pow R2 and S31 checksum errors using optimized re-sync
 *
 * 6.1.1.4
 * Change version representation from 1.1.1a to 1.1.1.1 for better change reference
 *
 * 6.1.1c
 * Add iFan02 Fanspeed + and Fanspeed - command options (#3415)
 * Fix some Pow R2 and S31 checksum errors (#3425)
 * Change CounterType 1 from milliseconds to microseconds (#3437)
 * Add output support on MCP23008/MCP23017 (#3436)
 * Fix LM75AD I2C sensor detection (#3408)
 * Add message when JavaScript is not enabled in webbrowser (#3388)
 * Fix sonoff-minimal from using default settings
 * Add option + to command Rule to concatenate new rule with existing rules (#3365)
 * Add initial support for sensor MPU6050 (#3352)
 * Add command SerialSend4 to send binary serial data (#3345)
 * Add rule triggers Wifi#Connected and Wifi#Disconnected (#3359)
 * Fix unsecure main webpage update
 * Add Turkish language file (#3332)
 * Fix command TimeDst/TimeStd invalid JSON (#3322)
 * Fix possible WDT due to long MQTT publish handling (#3313)
 * Fix CCS811 temperature and humidity compensation
 * Add support for CCS811 sensor (#3309)
 * Add command Timers 0/1 to globally disable or enable armed timers (#3270)
 *
 * 6.1.1b
 * Add support for MPR121 controller in input mode for touch buttons (#3142)
 * Add support for MCP230xx for general purpose input expansion and command Sensor29 (#3188)
 * Fix command Scale buffer overflow (#3236)
 * Fix rules once regression from v6.1.0 (#3198, #3226)
 * Add default Wifi Configuration tool as define WIFI_CONFIG_NO_SSID in user_config.h if no SSID is configured (#3224)
 * Add user selection of Wifi Smartconfig as define USE_SMARTCONFIG in user_config.h
 * Add user selection of WPS as define USE_WPS in user_config.h in preparation for core v2.4.2 (#3221)
 * Change default Wifi config option from WPS to Wifi Manager if WPS is disabled
 *   or Wifi Smartconfig if webserver is disabled
 *   or Wifi Serial input if Smartconfig is disabled
 * Remove WPS and SmartConfig from sonoff-minimal saving 56k code space
 *
 * 6.1.1a
 * Fix TM1638 compile error (#3212)
 * Add TM1638 switch support (#2226)
 * Fix invalid response using more than 4 switches and domoticz
 * Update sensor drivers to provide instant results
 * Add read sensor retry to DS18B20, DS18x20, DHT, SHT1X and HTU21
 * Change SHT1x driver to provide better instant results
 * Fix DHT driver mixing values for different sensors (#1797)
 * Change DHT driver to provide better instant results and add decimals to DHT11 (#3164)
 * Change DS18x20 driver to provide better instant results (#3169)
 * Change DS18B20 driver to provide better instant results
 * Remove TSL2561 debug message and update library (#2415)
 * Change SHT1x sensor initialization from pre-teleperiod to once during restart to fix I2C interference
 * Add wifi and mqtt status led blinkyblinky to be disabled by SetOption31 1. Does not work when LedPower is On (deliberate) (#871, #2230, #3114, #3155)
 * Add experimental (untested) TM1638 switch support (#2226)
 * Add support for APDS9960 proximity sensor (#3051)
 * Add heap and stack debug information
 * Add debug facilities using optional xdrv_99_debug.ino to enable in user_config.h
 * Remove not needed functionality from Sonoff-minimal to save space
 *
 * 6.1.1 20180714
 * Revert wifi changes (#3177)
 * Revert sonoff-minimal removals causing failure of wifi connection (#3177)
 *
 * 6.1.0 20180706
 * Remove version 3, 4 and pre 5.2 settings auto-upgrade. See https://github.com/arendst/Sonoff-Tasmota/wiki/Upgrade#migration-path
 * Change default CFG_HOLDER from 0x20161209 to 4617 (=0x1209) - no impact on default upgrades
 * Change number of supported switches from 4 to 8 (#2885, #3086)
 * Change BME680 driver from Adafruit to Bosch BME680 library (#2969)
 * Fix Pzem004T checksum error
 * Fix KNX bug when doing reply of sensors values
 * Fix rules induced LWT message
 * Fix possible wifi connection problem (#1366)
 * Fix some Pow R2 and S31 checksum errors (#1907)
 * Fix display selection of un-available GPIO options in Module Configuration webpage (#2718)
 * Fix timer re-trigger within one minute after restart (#2744)
 * Fix IRSend not accepting data value of 0 by David Conran (#2751)
 * Fix vars on rules by Adrian Scillato (#2769)
 * Fix bug in KNX menu by Adrian Scillato (#2770)
 * Fix anomalies in rules (#2778)
 * Fix HUE bridge V1 software version by Heiko Krupp (#2788)
 * Fix Hardware Watchdog restart when using event command (#2853)
 * Add Ukrainian language file
 * Add KNX support for DS18S20 Temperature sensor
 * Add CRC to Settings making future upgrades more fail-safe
 * Add feature information to Status 4
 * Add tools folder with python script decode-status.py for decoding some status fields like SetOption and Features
 * Add Slots on the KNX Web Menu to select Group Addess to receive data to trigger rules
 * Add two rule sets of 511 characters using commands rule1, rule2 and rule3
 * Add Console Commands to send KNX Commands and KNX Values
 * Add Slots on the KNX Web Menu to select Group Addess to send data from console commands
 * Add Events to trigger rules when a command or read requests is received from KNX
 * Add command SetOption30 to enforce Hass discovery as light group (#1784)
 * Add support for BlitzWolf BW-SHP2 (and Homecube, Gosund SP1) Energy Monitoring Smart Socket (#2223)
 * Add time in minutes to rule Time#Initialized, Time#set and Time#Minute (#2669)
 * Add Eastron SDM630 energy meter by Gennaro Tortone (#2735)
 * Add KNX communication enhancement by Adrian Scillato (#2742)
 * Add KNX energy data by Adrian Scillato (#2750)
 * Add rule support for IrReceive and RfReceive (#2758)
 * Add python script fw-server.py in tools folder to create a simple OTA server by Gennaro Tortone (#2759)
 * Add rule variables %time% for minutes since midnight, %uptime%, %sunrise% and %sunset% giving time in minutes (#2669)
 * Add rules %mem1% to %mem5% variable names storing data in flash (#2780)
 * Add rules test on %varx% or %memx% (#2780)
 * Add optional token %id% substituting the unique MAC address to fulltopic by Michael Graf (#2794)
 * Add support for Sonoff S26 Smart Socket (#2808)
 * Add command WebSend [<host>(:<port>,<user>:<password>)] <command> (#2821)
 * Add increment and decrement value to command Counter (#2838)
 * Add support for Sonoff iFan02 as module 44 introducing command FanSpeed 0..3 (#2839)
 * Add source information to command execution to be shown with logging option 3 (#2843)
 * Add support for uploading Sonoff Bridge firmware found in tools/fw_efm8bb1 folder build by Portisch using Web Gui File Upload (#2886)
 * Add command RfRaw to control Portisch firmware features
 * Add support for I2C temperature sensor LM75AD (#2909)
 * Add option 0 to command Timers disarming all timers (#2962)
 * Add performance improvement when updating multiple individual WS2812 pixels (#3007)
 * Add command SetOption28 to switch between hex or decimal Sonoff Bridge RF received data format (#3008)
 * Add command SetOption29 to switch between hex or decimal IR received data format
 * Add decimal values support for commands ADD, SUB, MULT and SCALE (#3083, #3089)
 * Add support for bitflags SetOption50 .. SetOption81 (#3118)
 *
 * 5.14.0 20180515
 * Update language files
 * Update TasmotaSerial to 2.0.0 allowing Hardware Serial Fallback when correct connections are configured
 * Change command handling
 * Change user_config(_override).h defines TIME_STD and TIME_DST
 * Change user_config(_override).h otaurl to http://sonoff.maddox.co.uk/tasmota/sonoff.bin (#2588, #2602)
 * Fix configuration restore regression from 5.13.1
 * Fix compile error when ADC is enabled and Rules are disabled (#2608)
 * Fix rule power trigger when no backlog command is used (#2613)
 * Fix several timer data input and output errors (#2597, #2620)
 * Fix KNX config error (#2628)
 * Fix sensor MHZ-19 vanishing data over time (#2659)
 * Fix KNX reconnection issue (#2679)
 * Fix DST and STD time for Southern Hemisphere by Adrian Scillato (#2684, #2714)
 * Add Portuguese in Brazil language file
 * Add SetOption26 to enforce use of indexes even when only one relay is present (#1055)
 * Add support for sensor SI1145 UV Index / IR / Visible light (#2496)
 * Add rule state test for On/Off in addition to 0/1 (#2613)
 * Add hardware serial option to MHZ-19 sensor (#2659)
 * Add Eastron SDM120 energy meter by Gennaro Tortone (#2694)
 * Add user entry DST/STD using commands TimeStd and TimeDst (See wiki for parameter syntax) (#2721)
 *
 * 5.13.1 20180501
 * Fix JSON buffers size too small for execution in some situations (#2580)
 * Fix configuration restore (#2591)
 * Add define MODULE for user selecting default model although it preferably should not be changed (#569, #2589)
 *
 * 5.13.0 20180430
 * Change platformio option sonoff-ds18x20 to sonoff-allsensors enabling ds18x20 and all other sensors in one image
 * Change status display of Ssid and SetOption
 * Change default option SetOption15 from 0 to 1 providing better initial PWM experience
 * Change webpage parameter communication
 * Change max number of commands in Backlog from 15 to 30 and ignore commands overflowing
 * Change TSL2561 driver to joba library and delete Adafruit library (#1644)
 * Change default parameters in user_config.h to undefined for easy installation (#1851)
 * Change max user configurable hold time from 10 to 25 seconds (#1851)
 * Change Sonoff SC JSON format (#1939)
 * Change Polish language to using Diacritics (#2005)
 * Change user_config_override usage by providing user_config_override_sample.h (#2228)
 * Change MQTT response topic for Energy changes from ENERGY to SENSOR (#2229, #2251)
 * Change default Reset configuration time from 4 seconds to 40 seconds on Button hold (#2268)
 * Change ESP8266 Analog JSON message from {"Analog0:123"} to {"ANALOG":{"A0:123"}} to accomodate rules (#2560)
 * Change Counter JSON message from {"Counter1":0,"Counter3":0} to {"COUNTER":{"C1":0,"C3":0}} to accomodate rules
 * Change ADS1115 JSON message from {"ADS1115":{"Analog0":123,"Analog1":123}} to {"ADS1115":{"A0":123,"A1":123}}
 * Fix intermittent exception when dns lookup is used while sleep is enabled
 * Fix 5.4.0 regression turning off single press after button hold during 4x hold time
 * Fix possible wifi connection problem by erasing sdk configuration parameters
 * Fix NTP sync to Thu Jan 01 08:00:10 1970 results in uptime 17651+ days (core2.4.1/sdk2.2.1)
 * Fix MAX31850 higher temperatures (#1269)
 * Fix freeing more code space when emulation is disabled (#1592)
 * Fix providing web page configuratin option for Friendly Name when no device (relay or light) is configured (#1850)
 * Fix compile error when define HOME_ASSISTANT_DISCOVERY_ENABLE is not set (#1937)
 * Fix MQTT TLS fingerprint validation (#2033)
 * Fix update temperature on DS18x20 drivers (#2328)
 * Fix compile error when not defined USE_TIMERS (#2400)
 * Fix configuration filename truncation when it contains spaces (#2484, #2490)
 * Fix Energy Today and Yesterday overflow (#2543)
 * Add serial debug info
 * Add Portuguese language file
 * Add Czech language file
 * Add Bulgarian language file
 * Add Domoticz dust (custom) sensors to PMS5003 and NovaFitness SDS drivers as PM1, PM2.5 and PM10
 * Add commands Publish, Rule, RuleTimer and Event. See Wiki about Rule restriction, usage and examples
 * Add sonoff-classic, sonoff-allsensors and sonoff-knx
 * Add some coloring to important web buttons
 * Add support for sensor HC-SR04 ultrasonic (#113, #1964, #2444)
 * Add define MQTT_TELE_RETAIN compile option default set to 0 (#1071)
 * Add 16 timers using commands Timer and Timers (#1091)
 * Add optional Timer configuration webpage to be enabled in user_config.h with define USE_TIMERS_WEB
 * Add Multichannel Gas sensor using MultiChannel_Gas_Sensor library (#1245)
 * Add Domoticz Battery and RSSI Quality (#1604)
 * Add command HSBColor Hue,Sat,Bri (#1642, #2203)
 * Add compile time support for WS2812 BRG and RBG led configurations to be defined in user_config.h (#1690)
 * Add optional usage of %d or %X suffices in MQTT client to append chipid (#1871)
 * Add optional usage of %d or %X suffices in MQTT topic to append chipid (#1871)
 * Add optional usage of %d or %04d in ota url to be replaced with chipid (#1871)
 * Add Sonoff Bridge command RfKey<x> 5 to show current RF key values either default or learned (#1884)
 * Add user configurable serial GPIOs to MagicHome and Arilux modules (#1887)
 * Add Russian language file (#1909)
 * Add Webserver upload preflight request support (#1927)
 * Add Home Assistant clear other device (#1931)
 * Add Restart time to Status 1 (#1938)
 * Add optional TSL2561 driver using library Joba_Tsl2561 to be enabled in user_config.h with define USE_TSL2561_JOBA (#1951)
 * Add support for sensor SHTC3 (#1967)
 * Add compiler check for stable lwIP version v1.4 (#1940)
 * Add support for multiple SHT3X sensors (#1949, #2110)
 * Add always suffix with device number in Mqtt discovery topic (#1962)
 * Add support for optional MQTT drivers to be selected in user_config.h (#1992)
 * Add optional Arduino OTA support to be enabled in user_config.h (#1998)
 * Add diacritics to Polish language file (#2005)
 * Add Hungarian language file (#2024)
 * Add support for Nova Fitness SDS011 and possibly SDS021 particle concentration sensor (#2070)
 * Add single decimal precision to Nova Fitness SDS0x1 sensor values (#2093)
 * Add Chinese (Traditional) in Taiwan language file (#2108)
 * Add Sonoff SC domoticz support for Sound level as Counter and Air quality (#2118)
 * Add a second TLS fingerprint to allow switching keys in TLS mode (#2033, #2102)
 * Add display of remaining pulse time to command PulseTime (#2085)
 * Add additional time offset to Wifi Retry based on device mac address (#2089)
 * Add command Color6 RRGGBB for Clock hour marker color and command Rotation pixels for Clock rotation (#2092)
 * Add HTML language header in local language (#2123)
 * Add command PowerDelta 0..100 (percentage) to Energy monitoring devices to report on active power load change (#2157)
 * Add Restart Reason to Status 1 report (#2161)
 * Add command Channel 0..100 to control dimmer value for individual color channels (#2111, #2203)
 * Add support for Hardware Serial bridge using commands SerialDelimiter, Baudrate and SerialSend. Supports 8N1 and text only (#2182)
 * Add support for Software Serial bridge using commands SerialDelimiter, SBaudrate and SSerialSend. Supports 8N1 and text only (#2190)
 * Add support for Zengge WF017 PWM Led strip controller (#2202)
 * Add PWM status to command State if PWM enabled (#2203)
 * Add all FriendlyNames to Status information (#2208)
 * Add Channel status information (#2211)
 * Add hexadecimal Data entry to command IrSend using 0x notation (#1290, #2314)
 * Add Home Assistant MQTT Discovery for Buttons and change SetOption19 response (#2277)
 * Add multiple color entry support for command Led like Led2 120000 001200 000012 setting led2 as Red, Led3 as Green and Led4 as Blue (#2303)
 * Add hexadecimal RGB color entry on RGBCW leds (#2304)
 * Add support for SGP30 gas and air quality sensor (#2307)
 * Add optional Sunrise and Sunset timers with commands Latitide and Longitude to be enabled with define USE_SUNRISE in user_config.h (#2317)
 * Add timer sunrise and sunset offset (#2378)
 * Add user selectable defines for Sunrise/set Dawn option (#2378)
 * Add optional KNX IP Protocol Support (#2402)
 * Add random window to timers (#2447)
 * Add Greek language file (#2491)
 * Add support for Sonoff Pow R2 (#2340)
 * Add GPIO_User to GPIO02 for all Sonoff T1 (#2524)
 *
 * 5.12.0 20180209
 * Change library PubSubClient.h define MQTT_MAX_PACKET_SIZE from 512 to 1000 for Home Assistant  support
 * Change relation of define MESSZ being dependent on PubSubClient.h define MQTT_MAX_PACKET_SIZE
 * Change command color parameter input checks to less strict for Home Assistant support
 * Change command Ina219Mode into command Sensor13
 * Change commands HlwPCal, HlwUCal and HlwICal to PowerCal, VoltageCal and CurrentCal to be used for both Pow and S31 calibration
 * Change commands HlwPSet, HlwUSet and HlwISet to PowerSet, VoltageSet and CurrentSet to be used for both Pow and S31 calibration
 * Change uptime from hour to second resulting in a display of 123T13:45:21 where 123 is days
 * Change module name Wemos D1 mini into Generic (#1220)
 * Change HTML from width=100% to style=width:100% supporting HTML5 (#1358)
 * Change OSWATCH_RESET_TIME (Blocked loop) from 30 to 120 seconds to allow slow networks (#1556)
 * Change WIFI_MANAGER_SEC into WIFI_CONFIG_SEC (#1616)
 * Change function pointers code to save code space and memory (#1683)
 * Change webserver argument processing gaining 5k code space (#1705)
 * Change weblog memory usage (#1730, #1793, #1819)
 * Update TasmotaSerial library to 1.1.0
 * Update language files Italian (#1594), Dutch (#1723) and Spanish (#1722)
 * Fix Non-English JSON temperature unit attachement
 * Fix Arilux RF induced exception by moving interrupt handler to iram on non ESP8266/Arduino lib v2.3.0
 * Fix truncated command names and wrong response for DomoticzSwitchIdx (#1571)
 * Fix %-sign issue as printf escape character in Humidity and Sonoff SC (#1579)
 * Fix DS18B20 temperature JSON decimal dot (#1561)
 * Fix Energy JSON message (#1621)
 * Fix IRSend parameter translation (#1636)
 * Fix TSL2561 device detection (#1644, #1825)
 * Fix BME680 teleperiod resistance measuring (#1647)
 * Fix Energy Monitoring Energy Today and Energy Total reading after restart (#1648)
 * Fix IRReceive Data value (#1663)
 * Fix Energy Monitoring Energy Period roll-over (#1688)
 * Fix compiler warnings (#1774)
 * Fix command PWM response if no PWM channel is configured (#1783)
 * Add locale Decimal Separator to Web sensor page
 * Add ColorTemperature to light status message
 * Add command PowerOnState option 5 which inverts PulseTime and allows for delayed always on after power on
 * Add OtaMagic two step Web server OTA upgrade using filename-minimal image if OTA free space is too small
 * Add support for PMS5003 and PMS7003 particle concentration sensor
 * Add command SetOption21 1 to allow Energy Monitoring when power is off on Sonoff Pow and Sonoff S31 (#1420)
 * Add Chinese language file (#1551)
 * Add French language file (#1561)
 * Add Spanish language file (#1589)
 * Add HTTP Allow Cross Origin removed from ESP8266/Arduino lib v2.4.0 (#1572)
 * Add Home Assistant MQTT Discovery for switch and light to be enabled by command SetOption19 1 (#1534) or define HOME_ASSISTANT_DISCOVERY_ENABLE in user_config.h (#1685)
 * Add command State to retrieve device state information (same data as teleperiod state and status 11 in slightly different JSON format)
 * Add optional login to Webserver AP mode (#1587, #1635)
 * Add command Sensor15 2 to start MHZ19(B) Zero Point Calibration (#1643)
 * Add support for Sonoff S31 Smart Socket with Power Consumption Detection (#1626)
 * Add command SetOption20 to allow update of Dimmer/Color/Ct without turning power on (#1719, #1741)
 * Add NTP sync time slot based on chip id (#1773)
 * Add cursor pointer to web button (#1836)
 *
 * 5.11.1 20180107
 * Fix Sonoff Pow command handling (#1542)
 *
 * 5.11.0 20180107
 * Minor webpage HTML optimizations (#1358)
 * Updated German translation (#1438)
 * Change Sonoff Pow Energy MQTT data message and consolidate Status 8 into Status 10
 * Change ADS1115 default voltage range from +/-2V to +/-6V (#1289)
 * Change text to Active for 3 minutes (#1364)
 * Change Wemo SetBinaryState to distinguish from GetBinaryState (#1357)
 * Change output of HTTP command to valid JSON and Array only (#1363)
 * Removed all MQTT, JSON and Command language defines from locale files and set fixed to English (#1473)
 * Renamed commands Color2,3,4 to Color3,4,5
 * Fix BME280 calculation (#1051)
 * Fix Sonoff Bridge missed learned key if learned data contains 0x55 (End of Transmission) flag (#1095, #1294)
 * Fix PWM initialization in Dimmer/Color mode (#1321)
 * Fix Wemo Emulation (#1357)
 * Fix display of build date and time in non-english locale (#1465)
 * Fix Wemo and Hue emulation by adding M-Search response delay (#1486)
 * Add libraries Adafruit_BME680-1.0.5, Adafruit_Sensor-1.0.2.02, TasmotaSerial-1.0.0 and TSL2561-Arduino-Library
 * Add command Color2 to set color while keeping same dimmer value
 * Add device function pointers
 * Add support for SenseAir S8 CO2 sensor
 * Add color led signal to Carbon Dioxide (CO2) sensors using defines CO2_LOW and CO2_HIGH in user_config.h
 * Add support for Domoticz Air Quality sensor to be used by MH-Z19(B) and SenseAir sensors
 * Add support for PZEM004T energy sensor
 * Add support for iTead SI7021 temperature and humidity sensor by consolidating DHT22 into AM2301 and using former DHT22 as SI7021 (#735)
 * Add support for BME680 using adafruit libraries (#1212)
 * Add support for MH-Z19(B) CO2 sensor (#561, #1248)
 * Add multipress support and more user configurable GPIO to Sonoff Dual R2 (#1291)
 * Add support for TSL2561 using adafruit library (#661, #1311)
 * Add support for SHT3x (#1314)
 * Add support for Arilux LC06 (#1414)
 * Add Italian language file (#1449)
 * Add 2nd Gen Alexa support to Wemo emulation discovery (#1357, #1450)
 * Add define for additional number of WS2812 schemes (#1463)
 *
 * 5.10.0 20171201
 * Upgrade library ArduinoJson to 5.11.2
 * Upgrade library IRRemoteEsp8266 to 2.2.1 + 2 commits but disabled some protocols (code size reduction)
 * Upgrade library NeoPixelBus to 2.2.9
 * Upgrade library OneWire to 2.3.3 + 6 commits and disabled CRC lookup-table (#define ONEWIRE_CRC8_TABLE 0) (code size reduction)
 * Update library PubSubClient to 2.6 + 9 commits and additional delay (#790)
 * Update core_esp8266_wiring_digital.c to latest (staged) level
 * Patch library I2Cdevlib-Core for esp8266-core 2.4.0-rc2 compatibility
 * Remove command EnergyReset 1..3 now replaced by EnergyReset1 to EnergyReset3
 * Remove spaces in JSON messages (code size reduction)
 * Renamed xsns_05_ds18x20.ino to xsns_05_ds18x20_legacy.ino still using library OneWire and providing dynamic sensor scan
 * Fix possible iram1_0_seg compile error by shrinking ICACHE_RAM_ATTR code usage
 * Fix PWM watchdog timeout if Dimmer is set to 100 or Color set to 0xFF (#1146)
 * Fix Sonoff Bridge Learn Mode hang caused by unrecognised RF code (#1181)
 * Fix blank console log window by using XML character encoding (#1187)
 * Fix wrong response name for command HlwISet (#1214)
 * Fix DHT type sensor timeout recognition by distinguish "signal already there" from "timeout" (#1233)
 * Add fixed color options 1..12 to command Color
 * Add + (plus) and - (minus) to commands Dimmer (+10/-10), Speed and Scheme
 * Add + (plus) and - (minus) to command Color to select 1 out of 12 preset colors
 * Add + (plus) and - (minus) to command Ct to control ColdWarm led ColorTemperature (+34/-34)
 * Add commands EnergyReset1 0..42500, EnergyReset2 0..42500 and EnergyReset3 0..42500000
 *  to (Re)set Energy Today, Yesterday or Total respectively in Wh (#406, #685, #1202)
 * Add optional ADS1115 driver as alternative for unsupported I2Cdevlib in esp8266-core 2.4.0-rc2
 * Add support for INA219 Voltage and Current sensor to be enabled in user_config.h with define USE_INA219
 * Add support for Arilux LC11 (Clearing RF home code when selecting no Arilux module)
 * Add support for WS2812 RGBW ledstrips to be enabled in user_config.h with define USE_WS2812_CTYPE (#1156)
 * Add SettingsSaveAll routine to command SaveData to be used before controlled power down (#1202)
 * Add option PUSHBUTTON_TOGGLE (SwitchMode 7) to allow toggling on any switch change (#1221)
 * Add new xdrv_05_ds18x20.ino free from library OneWire and add the following features:
 *  Add support for DS1822
 *  Add forced setting of 12-bit resolution for selected device types (#1222)
 *  Add read temperature retry counter (#1215)
 *  Fix lost sensors by performing sensor probe at restart only thereby removing dynamic sensor probe (#1215)
 *  Fix sensor address sorting using ascending sort on sensor type followed by sensor address
 *  Rewrite JSON resulting in shorter message allowing more sensors in default firmware image:
 *   "DS18B20-1":{"Id":"00000483C23A","Temperature":19.5},"DS18B20-2":{"Id":"0000048EC44C","Temperature":19.6}
 * Add additional define in user_config.h to select either single sensor (defines disabled), new multi sensor (USE_DS18X20) or legacy multi sensor (USE_DS18X20_LEGACY)
 * Add clock support for more different pixel counts (#1226)
 * Add support for Sonoff Dual R2 (#1249)
 * Add FriendlyName to web page tab and add program information to web page footer (#1275)
 *
 * 5.9.1 20171107
 * Add external sensor function pointer interface to enable easy sensor addition
 * Add support for ADS1115 to be enabled in user_config.h and needs libraries i2cdevlib-Core and i2cdevlib-ADS1115 (#338, #660)
 * Fix Backup Configuration file download failure by defining proper file size (#1115)
 * Fix Exception 26 and empty console screen after usage of command WakeupDuration (#1133)
 * Fix some changed iTead web links in README.md (#1137)
 *
 * 5.9.0 20171030
 * Rewrite code (partly) using Google C++ Style Guide (https://google.github.io/styleguide/cppguide.html)
 * Rewrite code by using command lookup tables and javascript (client side) web page expansions
 * Change HTML/CSS to enable nicer form field entry
 * Change default PWM assignments for H801 RGB(CW) led controller to support optional Color/Dimmer control
 *   GPIO04 (W2)    from GPIO_PWM2 to GPIO_USER to be user configurable for GPIO_PWM5 (second White - Warm if W1 is Cold)
 *   GPIO12 (Blue)  GPIO_PWM3 no change
 *   GPIO13 (Green) from GPIO_PWM4 to GPIO_PWM2
 *   GPIO14 (W1)    from GPIO_PWM1 to GPIO_USER to be user configurable for GPIO_PWM4 (first White - Cold or Warm)
 *   GPIO15 (Red)   from GPIO_PWM5 to GPIO_PWM1
 * Change default PWM assignments for MagicHome RGB(W) led controller to support optional Color/Dimmer control
 *   GPIO05 (Green) from GPIO_PWM4 to GPIO_PWM2
 *   GPIO12 (Blue)  from GPIO_PWM5 to GPIO_PWM3
 *   GPIO13 (White) GPIO_USER to be user configurable for GPIO_PWM4 (White - Cold or Warm)
 *   GPIO14 (Red)   from GPIO_PWM3 to GPIO_PWM1
 * Change default PWM assignment for Witty Cloud to support optional Color/Dimmer control (#976)
 *   GPIO12 (Green) from GPIO_PWM4 to GPIO_PWM2
 *   GPIO13 (Blue)  from GPIO_PWM5 to GPIO_PWM3
 *   GPIO15 (Red)   from GPIO_PWM3 to GPIO_PWM1
 * Change when another module is selected now all GPIO user configuration is removed
 * Change command name IRRemote to IRSend (#956)
 * Remove Arduino IDE version too low warning as it interferes with platformio.ini platform = espressif8266_stage
 * Fix command FullTopic entry when using serial or console interface
 * Fix possible UDP syslog blocking
 * Fix minimum TelePeriod of 10 seconds set by web page
 * Fix command GPIOx JSON response (#897)
 * Fix inverted relay power on state (#909)
 * Fix compile error when DOMOTICZ_UPDATE_TIMER is not defined (#930)
 * Fix alignment of web page items in some browsers (#935)
 * Fix setting all saved power settings to Off when SetOption0 (SaveState) = 0 (#955)
 * Fix timezone range from -12/12 to -13/13 (#968)
 * Fix Southern Hemisphere TIME_STD/TIME_DST (#968)
 * Fix TLS MQTT SSL fingerprint test (#970, #808)
 * Fix virtual relay status message used with Color/Dimmer control (#989)
 * Fix command IRSend and IRHvac case sensitive parameter regression introduced with version 5.8.0 (#993)
 * Fix pressure calculation for some BMP versions regression introduced with version 5.8.0i (#974)
 * Fix Domoticz Dimmer set to same level not powering on (#945)
 * Fix Blocked Loop when erasing large flash using command reset 2 (#1002)
 * Fix relay power control when light power control is also configured as regression from 5.8.0 (#1016)
 * Fix Mqtt server mDNS lookup only when MqttHost name is empty (#1026)
 * Add debug information to MQTT subscribe
 * Add translations to I2Cscan
 * Add translation to BH1750 unit lx
 * Add light scheme options (Color cycle Up, Down, Random) and moving WS2812 schemes up by 3
 * Add Domoticz counter sensor to IrReceive representing Received IR Protocol and Data
 * Add option 0 to MqttHost to allow empty Mqtt host name
 * Add support for Arilux AL-LC01 RGB Led controller (#370)
 * Add esp8266 de-blocking to PubSubClient library (#790)
 * Add Domoticz sensors for Voltage and Current (#903)
 * Add platformio OTA upload support (#928, #934)
 * Add warning to webpage when USE_MINIMAL is selected (#929)
 * Add smoother movement of hour hand in WS2812 led clock (#936)
 * Add support for Magic Home RGBW and some Arilux Led controllers (#940)
 * Add command SetOption15 0 (default) for command PWM control or SetOption15 1 for commands Color/Dimmer control to PWM RGB(CW) leds (#941)
 * Add Domoticz counter sensor to Sonoff Bridge representing Received RF code (#943)
 * Add support for Luani HVIO board (https://luani.de/projekte/esp8266-hvio/) (#953)
 * Add PWM initialization after restart (#955)
 * Add IR Receiver support. Disable in user_config.h (#956)
 * Add support for inverted PWM (#960)
 * Add Sea level pressure calculation and Provide command Altitude (#974)
 * Add support for up to 8 relays (#995)
 * Add commands RfSync, RfLow, RfHigh, RfHost and RfCode to allow sending custom RF codes (#1001)
 * Add retain to ENERGY messages controlled by command SensorRetain (#1013)
 * Add commands Color2, Color3, Color4, Width2, Width3, Width4 and SetOption16 to set Ws2812 Clock parameters (#1019)
 * Add German language file (#1022)
 * Add support for connecting to MQTT brokers without userid and/or password (#1023)
 * Add support for esp8266 core v2.4.0-rc2 (#1024)
 * Add commands PwmRange 1,255..1023 and PwmFrequency 1,100..4000 (#1025)
 * Add Polish language file (#1044, #1047)
 * Add support for KMC 70011 Power Monitoring Smart Plug (#1045)
 * Add support for VEML6070 I2C Ultra Violet level sensor (#1053)
 * Add light turn Off Fade (#925)
 * Add IrSend command option Panasonic as IrSend {"Protocol":"Panasonic", "Bits":16388, "Data":<Panasonic data>}
 *   where 16388 is 0x4004 hexadecimal (#1014)
 * Add retry counter to DHT11/21/22 sensors (#1082)
 *
 * 5.8.0 20170918
 * Remove the need for NeoPixelBus library for Hue support
 * Consolidate WS2812 into Sonoff Led for flexible future led strip library changes
 * Invert WS2812 fade speed to align with Sonoff led (Speed 1 = fast, Speed 8 = slow)
 * Remove upper case MQTT receive buffer
 * Reduce code and string length for output of commands Modules and GPIOs
 * Add Sonoff SC debug information
 * Change syslog service
 * Removed webserver syslog disable as now no longer needed
 * Increased default MQTT message size from 368 to 405 bytes while keeping MQTT_MAX_PACKET_SIZE = 512 (because we can)
 * Fix MQTT Offline or Remove MQTT retained topic code
 * Fix Domoticz loop when Emulation is selected
 * Add blink to WS2812 and Sonoff Led (#643)
 * Add option WIFI_WAIT (5) to command WifiConfig to allow connection retry to same AP without restart or update flash (#772, #869)
 * Add support for Witty Cloud (#794)
 * Add GPIO14 to Sonoff Dual (#797, #839)
 * Add support for Yunshan Wifi Relay (#802)
 * Add GPIO16 input pulldown (#827)
 * Add timeout to DHT and DS18B20 sensors (#852)
 * Fix watchdog timeout caused by lack of stack space by moving to heap (#853)
 * Allow LogPort and MqttPort up to 65535 and add LogPort tot Status 3 (#859)
 * Allow command SwitchTopic in group mode (#861)
 * Allow command SwitchMode if no switches are defined (#861)
 * Add optional dimmer parameter to command Wakeup for WS2812, AiLight, Sonoff B1, Led and BN-SZ01 (#867)
 * Fix basic On, Off, Toggle, Blink and BlinkOff commands when other language is selected (#874)
 *
 * 5.7.1 20170909
 * Remove leading spaces from MQTT data
 * Fix webconsole special character entry
 * Allow # as prefix for color value
 * Fix Alexa detection and Hue App Update Request (#698, #854)
 *
 * 5.7.0 20170907
 * Shrink module configuration webpage
 * Fix settings order during startup to allow for displaying debug messages
 * Fix some string length issues
 * Add more string length tests by using strncpy
 * Add Ai-Thinker RGBW led (AiLight)
 * Add Power check and add PulseTime to power check at startup (#526)
 * Add Supla Espablo support (#755)
 * Add more precision to Sonoff Pow period and power results using command WattRes 0|1 (#759)
 * Add basic internationalization and localization (#763)
 * Add more Sonoff Pow range checking (#772)
 * Fix invalid JSON (#786, #822)
 * Add duplicate check to received RF signal within 2 seconds for Sonoff Bridge (#810)
 *
 * 5.6.1 20170818
 * Change module list order in webpage
 * Fix Sonoff T1 1CH and 2CH configuration (#751)
 *
 * 5.6.0 20170818
 * Fix Sonoff Pow intermittent exception 0
 * Change Sonoff Pow sending Domoticz telemetry data only
 * Add Ai-Thinker RGBW led (AiLight) (experimental)
 * Add NeoPixelBus library to Sonoff Led for Hue support
 * Add user configurable GPIO4 and GPIO5 to module Sonoff Bridge
 * Add Sonoff B1 RGBCW led support with command Color RRGGBBCCWW (#676)
 * Add command CT 152..500 to Sonoff Led and Sonoff B1 to control Color Temperature
 * Add Cold-Warm slider to web page for Sonoff Led and Sonoff B1
 * Add CT parameter to Hue
 * Add Sonoff T1 support (#582)
 * Add AnalogInput0 if configured as Analog Input to webpage (#697, #746)
 * Add command SetOption14 0|1 to enable interlock mode (#719, #721)
 * Fix Mitsubishi HVAC IR power controll (#740)
 *
 * 5.5.2 20170808
 * Extent max number of WS2812 pixels from 256 to 512 (#667)
 * Add OTA handling if server responds with no update available (#695)
 * Removed undocumented command FlashMode (#696)
 * Fix compile time error message due to increased message buffer size (#703)
 *
 * 5.5.1 20170805
 * Fix Sonoff Rf Bridge issues
 * Add Sonoff RF Bridge MQTT messages on received and learned RF signal
 * Add command VoltRes 0|1 to select voltage resolution to 0.1 V (#654)
 * Add averaging to Analog input (#686)
 * Add Energy tele data on Sonoff Pow Threshold change (#688)
 * Fix inconsistent property names in Messages (#690)
 *
 * 5.5.0 20170730
 * Reduce code space by removing the following commands as they are replaced by SetOption alternatives:
 *   SaveState = SetOption0
 *   ButtonRestrict = SetOption1
 *   Units = SetOption2
 *   MQTT = SetOption3
 *   MQTTResponse = SetOption4
 *   TempUnit = SetOption8
 * Smoothing WS2812 animation poll, invert fade speed and max allowed wakeup time down to 3000 seconds
 * Fix initial button press detection
 * Add support for Sonoff RF Bridge 433 using command RfKey
 * Fix regression from 5.0.7 by increasing message buffer size from 360 to 368 to accomodate 4 x DS18x20 sensors (#637)
 * Add GroupTopic to Topic test when using ButtonTopic/SwitchTopic to send either ON/OFF or TOGGLE (#642)
 * Adjust HLW calibration limits to accomodate HuaFan device and add commands HlwPSet, HlwUSet and HlwISet (#654)
 *
 * 5.4.0 20170725
 * Fix command reset regression introduced in 5.2.0
 * Increase polling from 0.1 second to 0.05 second
 * Add multipress to all buttons
 * Fix button 1 double press behaviour on multi relay devices
 * Add support for Hua Fan Smart Socket (#479)
 * Add support for Sonoff 4ch Pro (#565)
 * Add command SetOption13 1 to allow immediate action on single button press
 *   (disables multipress, hold and unrestricted commands) (#587)
 *
 * 5.3.0 20170715
 * Major Hue rewrite which might introduce Alexa problems. If so, initiate an issue
 * Add support for Sonoff Led and BN-SZ01 Ceiling Led brightness control to Hue
 * Fix Sonoff Led Power, Dimmer and Color MQTT response (#176)
 * Add commands Delay and Backlog to allow multiple commands at once separated by ";" (#593)
 * Use default flashmode DOUT to solve restart hangs on esp8285 chips (#453, #598)
 * Change Web console column width from 99 to 300 (#599)
 *
 * 5.2.4 20170703
 * Removed flash mode update after selecting different module solving esp8285 related problems
 * Add device type flag to sonoff_template.ino
 * Change Sonoff Led Wakeup and add support for Sonoff BN-SZ01 Led (#567)
 *
 * 5.2.3 20170630
 * Change Sonoff Led color conversion code
 * Fix SetOption12 handling
 * Simplify auto configuration upgrade
 * Add option Upgrade <version_number> to only upgrade to any higher version (Old PR #213)
 * Change FallbackTopic to cmnd/<MQTTClient>/<command> <parameter> bypassing FullTopic and Prefix (#538)
 *
 * 5.2.2 20170625
 * Add configuration SaveAddress to Status 1 and Information Page
 * Change Sonoff Led Color conversion from AtoH to strtol
 * Fix possible wrong uploads due to configuration overwrites (#542)
 * Fix payload negative numbers (#547)
 *
 * 5.2.1 20170622
 * Fix Restore Configuration in case of lower version
 * Revert auto configuration upgrade allowing easy upgrade which was removed in version 5.2.0
 * Fix config auto upgrade from versions below version 4.1.1 (#530)
 *
 * 5.2.0 20170619
 * Add command SetOption12 1 to disable newly released configuration flash rotate to reduce flash wear
 * Fix command CounterDebounce by removing test for active GPIO (#524)
 * Add command SetOption33 1..250 to allow user configure POW Max_Power_Retry count (#525)
 *
 * 5.1.7 20170616
 * Prep removal of SetOptions alternatives
 * Restore webpage upgrade error messages removed in 5.1.5
 * Add hold button functionality to buttons 2 to 4
 * Add command SetOption32 1..100 to set Key Hold Time from 0.1 seconds to 10 seconds (#200)
 * Allow slashes in Topic, GroupTopic, ButtonTopic and SwitchTopic (#507)
 * Changed webpage form actions from post to get and use relative path url (#434, #522)
 *
 * 5.1.6 20170606
 * Shrink code
 * Removed online configuration of Domoticz In and Domoticz Out MQTT strings
 * Removed commands DomoticzInTopic and DomoticzOutTopic
 * Add define KEY_HOLD_TIME to configure button hold threshold before sending MQTT Hold message
 * Add command StateText4 to configure button MQTT Hold text (= MQTT_CMND_HOLD)
 * Add command SetOption11 0|1 to swap pushbutton single and double press functionality (#200)
 * Add command SwitchMode<x> 5 (PUSHBUTTONHOLD) and 6 (PUSHBUTTONHOLD_INV) (#489)
 *
 * 5.1.5 20170604
 * Shrink code in preparation to ESP8266-Arduino 2.4.0-rc1
 * Add effect parameter to HUE Device (#464)
 *
 * 5.1.4 20170601
 * Removed pre-compiled versions from repository as they are available within the release
 * Changed HUE Device type to color supporting version (#464)
 * Fix compile error when BE_MINIMAL is selected (#467, #476)
 * Add multiple compiled versions to release using updated Travis script and platformio.ini (#467)
 *
 * 5.1.3 20170520
 * Add Domoticz Counter
 *
 * 5.1.2 20170519
 * Fix Counter/Timer JSON message and update Counter/Timer on webpage
 * Fix WS2812 Domoticz related regression issues
 *
 * 5.1.1 20170517
 * Allow command FullTopic in group mode
 * Prepare for more use of RTC memory
 * Add independant WS2812 led string power control (#386, #390)
 * Add command Counter<x> to control up to four GPIO falling edge interrupt counters or timers (#459)
 * Add command CounterType<x> to select between pulse counting or pulse timing
 * Add command CounterDebounce to select global counter debounce time in mSec
 *
 * 5.1.0 20170513
 * Fix Offline/Removal of retained topic when FullTopic is changed
 * Add FullTopic to MQTT Configuration and Information web pages
 * Add license model GPLv3 (#188)
 *
 * 5.0.7 20170511
 * Fix possible exception 28 on empty command
 * Add command SetOption0 as replacement for SaveState
 * Add command SetOption1 as replacement for ButtonRestrict
 * Add command SetOption2 as replacement for Units
 * Add command SetOption4 as replacement for MqttResponse
 * Add command SetOption8 as replacement for TempUnit
 * Add command SetOption10 On|Off to select between Offline or Removing previous retained topic (#417, #436)
 *
 * 5.0.6 20170510
 * Remove hyphen in case of a single DHT sensor connected (#427)
 * Add command MqttRetry <seconds> to change default MQTT reconnect retry timer from minimal 10 seconds (#429)
 *
 * 5.0.5 20170508
 * Add command FullTopic with tokens %topic% (replaced by command Topic value) and
 *  %prefix% (replaced by command Prefix<x> values) for more flexible topic definitions (#244)
 *  See wiki > MQTT Features https://github.com/arendst/Sonoff-Tasmota/wiki/MQTT-Features for more information
 *
 * 5.0.4 20170505
 * Add Sonoff Pow Energy Total up to 40 MWh
 * Add command EnergyReset 1|2|3 to reset Energy counters (#406)
 * Fix Domoticz Energy logging (#411)
 * Add command PowerOnState 4 to keep relay always on and disabling all power control (#418)
 *
 * 5.0.3 20170504
 * Add command SensorRetain on|off to enable retaining of mqtt message tele/sonoff/SENSOR (#74)
 * Change WifiConfig timeout from 60 seconds to 180 seconds (#212)
 * Change Sonoff Touch command Ledstate functionality by turning led on if power is off (#214)
 * Add 4 seconds delay after power on before enabling button to workaround Wemos D1 mini RTS circuit (#380)
 *
 * 5.0.2 20170503
 * Reset SaveData, SaveState and MqttResponse to default values due to rearranging settings
 * Moved some settings to flag area
 * Add command TempUnit Celsius|Fahrenheit for selecting Celsius or Fahrenheit (#347)
 * Add command TempRes 0..3 for selecting Temperature Resolution (#347)
 * Add command HumRes 0..3 for selecting Humidity Resolution (#347)
 * Add command PressRes 0..3 for selecting Pressure Resolution (#347)
 * Add command EnergyRes 0..5 for selecting Energy Resolution (#347)
 * Add "TempUnit":"C|F" to sensor JSON output (#347)
 * Add support for up to three DHT type sensors each using a different GPIO (#339, #404)
 *
 * 5.0.1 20170429
 * Adjust Sonoff SC messages to prepare for display feature
 * Move static data from RAM to Flash
 * Fix PowerOnState for some devices not reporting "Power on" state (#284, #380, #383)
 *
 * 5.0.0 20170425
 * Memory status message update
 * Fix setting migration to better preserve settings during move (#382)
 * Best practice is first doing a Backup Configuration before installing version 5.0.0
 * Reset save count after setting move
 * Start using new linker script without SPIFFS
 *
 * 4.2.0 20170424
 * Prepare for SPIFFS removal by moving settings to EEPROM area
 * Fix compilation error when webserver is disabled (#378)
 *
 * 4.1.3 20170410
 * Add user configuarble GPIO to module S20 Socket and Slampher
 * Add support for Sonoff SC (#112)
 * Set PWM frequency from 1000Hz to 910Hz as used on iTead Sonoff Led firmware (#122)
 * Set Sonoff Led unconfigured floating outputs to 0 to reduce exceptions due to power supply instabilities (#122)
 * Add Access Point Mac Address to Status 11 and Telemetry (#329)
 * Fix DS18B20 negative temperature readings (#334)
 *
 * 4.1.2 20170403
 * Rename Unrecognised command to Unknown command
 * Remove all command lists
 * Remove command SmartConfig (superseded by WifiConfig)
 * Fix boot loop when selecting module Sonoff 4CH or Sonoff Touch on non ESP8285 hardware
 * Add optional support for Toshiba and Mitsubishi HVAC IR control (needs updated IRremote8266 library) (#83, #257)
 * Add all configured switches to Domoticz Configuration web page (#305)
 * Fix compile error when selecting WS2812 DMA (#313)
 *
 * 4.1.1 20170329
 * Fix default Telemetry for command Prefix3
 * Fix webserver Module parameters for disabled select
 * Fix sensor status for enabled switches
 * Remove Light as alternative for Power (save code space)
 * Remove migration option from pre V3 (code cleanup)
 * Remove unofficial SPIFFS support (code cleanup)
 * Remove command list when unknown command is entered (save code space)
 * Rename Status11 json from StatusPWR to unique StatusSTS
 * Rename command Gateway to IPAddres2, Subnetmask to IPAddress3 and DnsServer to IPAddress4 (save code space)
 * Add Command MqttResponse to select either command or RESULT topic as response (#258)
 * Add command StateText1 to StateText3 to assign MQTT_STATUS_OFF, MQTT_STATUS_ON and MQTT_CMND_TOGGLE respectively (#286)
 * Remove restart after IPAddress changes (#292)
 * Add support for MAX31850 in xsns_ds18x20.ino (#295)
 * Fix possible uptime update misses (#302)
 *
 * 4.1.0 20170325
 * Change static IP addresses in user_config.h from list (using commas) to string (using dots)
 * Unify display result of commands Modules, Module and Gpios
 * Rewrite Module selection web page to bring size down from 18651 to 4319 bytes (!) (#234, #240)
 * Add basic support for (Lixada) H801 RGBWW controller (#252)
 * Add command Prefix1 to Prefix3 to assign SUB_PREFIX, PUB_PREFIX and PUB_PREFIX2 respectively (#255)
 * Add static ip addresses to flash (#262)
 * Add commands IpAddress, Gateway, Subnetmask and DnsServer to select static ip addresses (#273)
 *
 * 4.0.8 20170321
 * Fix entering non-numeric webpassword
 * Force selection between TLS or Webserver due to memory restraint (#240)
 * Allow entering empty string using "0" for selected commands (#242)
 * Fix exception when posting commands to web console containing % (#250)
 *
 * 4.0.7 20170319
 * Increased Sonoff Led PWM frequency from 432 to 1000
 * Fix possible watch dog reboot after changing module type on web page
 * Fix reporting of GPIO usage from web page
 * Fix Sonoff Led blank during firmware upgrade
 * Fix Sonoff Led flicker and possible flash corruption by using latest Arduino-esp8266 versions
 *   of pwm core files included in sonoff library (#211)
 * Add PWM output control with commands PWM1 to PWM5 using user selectable GPIOs (#211)
 * Fix exceptions due to low values of commands HlwPCal (10000), HlwUCal (1000) and HlwICal (2500) (#223)
 * Add Switch state to sensor status (#227, #233)
 * Add user configuarble GPIO to module Sonoff Touch (#228)
 * Add define WEB_PORT to user_config.h to change default web server port from 80 (#232)
 * Fix failed Ota Firmware upgrade started from Web page (#235)
 *
 * 4.0.6 20170316
 * Fix to better find device by Wifi hostname
 * Fix compile error when some I2C devices are disabled
 * Add (experimental) support for SHT1X emulating I2C (#97)
 * Add ADC to ElectroDragon (#203)
 * Add support for Sonoff Dev (#206)
 *
 * 4.0.5 20170314
 * Add command Status 11 to show power status with Vcc if define USE_ADC_VCC is enabled (default)
 * Add ADC input to Sonoff SV and Wemos D1 mini - Needs recompile with define USE_ADC_VCC disabled (#137)
 * Add MQTT host:port to timeout message (#199)
 *
 * 4.0.4 20170312
 * Add pulse timers for up to 4 relays (#106)
 * Fix Sonoff Led power state when dimmer or color is 0 (#176)
 * Add command NtpServer<x> to configure up to three NTP servers (#177)
 * Delete module User Test as module Wemos D1 mini has same/more user configurable GPIO (#178)
 * Add more user configurable GPIO to module ElectroDragon (#183)
 *
 * 4.0.3 20170309
 * Renamed Module NodeMCU to WeMos D1 mini
 * Add GPIO1 as user option to some modules
 * Add Buttons, Relays and Leds to user configurable options (#159)
 * Add description on Module parameters web page to some well known GPIOs (#107, #171)
 *
 * 4.0.2 20170308
 * Restore correct seriallog level after Serial logging was disabled
 * Add simple dimmer slider to Sonoff Led web page
 * Reduced root webpage size by 31%
 * Expand Status 2 with Build date/time and core version
 * Fix webserver redirection when not in WifiManager mode (#156)
 * Add command ButtonRestrict On/Off to restrict access to button hold and button multi press options above 2 (#161)
 * Fix DS18S20 negative temperature readings (#165)
 * Fix crlf compilation error due to bad syntax (#144, #167)
 *
 * 4.0.1 20170305
 * Fix char default sizes and set MESSZ to 360 (#143)
 * Fix SerialLog setting status
 * Disable syslog when emulation is active
 * Add DS18B20 web page display refresh
 *
 * 4.0.0 20170303
 * Add define to remove config migration code for versions below 3.0 (See Wiki-Upgrade-Migration path)
 * Free memory by switching from String to char[]
 * Raised Sonoff Led PWM frequency from 200Hz to 432Hz in search of stability (hardware watchdog timeouts) (#122)
 * Increase message size and suggested minimum MQTT_MAX_PACKET_SIZE to 512 (#114, #124)
 * Remove runtime warning message regarding MQTT_MAX_PACKET_SIZE too small as it is now moved to compile time (#124)
 * Fix possible panics with web console and http commands while UDP syslog is active (#127)
 * Add optional static IP address (#129)
 * Add define ENERGY_RESOLUTION in user_config.h to allow user control over precision (#136)
 *
 * 3.9.22 20170228
 * Update web console
 * Fix Status 4 JSON message
 * Add Exception info during restart if available
 * Add osWatch service to detect loop hangs that might happen during (OTA) upgrades
 * Add WiOn support for relay and switch only (#82, #102)
 * Allow for user specified relay count up to four in sonoff_template.h (#109)
 * Add support for HTU21 compatible I2C sensors SI7013, SI7020 and SI7021 (#118)
 * Add NodeMCU or Wemos configuration option (#119)
 *
 * 3.9.21 20170224
 * Add ajax to web root page and web console (#79)
 * Add commands SwitchMode1..4 and enable user switches 2, 3 and 4 (#84, #88)
 * Fix MQTT upgrade when webserver is active
 *
 * 3.9.20 20170221
 * Add minimal basic authentication to Web Admin mode (#87)
 * Fix Hue and add HSB support (#89)
 *
 * 3.9.19 20170219
 * Sonoff Led: Made GPIO04, 05 and 15 available for user
 * Sonoff Led: Add commands Fade, Speed, WakupDuration, Wakeup and LedTable
 *
 * 3.9.18 20170218
 * Fix ledstate 0 to turn off led
 * Fix Sonoff Led dimmer range (#16)
 * Change Sonoff Led command Dimmer to act on both cold and warm color
 * Add Sonoff Led command Color CCWW where CCWW are hexadecimal values fro 00 - FF
 * Reduce Sonoff Led flickering by disabling interrupts during flash save and disabling
 *   Led during OTA upgrade and Web upload (#16)
 *
 * 3.9.17 20170217
 * Fix possible ArduinoJSON related memory fragmentation
 * Changed console logging using less memory
 * Add GPIO04 as user selectable for Sonoff Dual (#75)
 *
 * 3.9.16 20170214
 * Update latching relay handler
 * Add support for IR led using IRremoteESP8266 library (#59)
 * Add Hue argument passing using ArduinoJSON library (#59)
 *
 * 3.9.15 20170213
 * Change JSON float values from string to number according to http://json.org (#56)
 * Add support for exs latched relay module https://ex-store.de/ESP8266-WiFi-Relay-V31 (#58)
 * Add support for inverted relays
 * Changed MAX_LOG_LINES from 70 to 60 to preserve memory
 *
 * 3.9.14 20170211
 * Add False and True as alternatives for 0/Off and 1/On (#49)
 * Fix Status10 JSON format (#52)
 * Fix DS18x20 using OneWire library (#53)
 *
 * 3.9.13 20170210
 * Add FlashChipMode to Status 4
 * Removed redundant DHT2 option and code
 * Add Sonoff SV GPIO pin 05 configuration (#40)
 * Add configuration file backup and restore via web page
 * Fix latency due to light_sleep mode even if sleep was set to zero (#50)
 *
 * 3.9.12 20170208
 * Fix compile error when webserver is disabled (#30)
 * Fix possible ESP8285 flash problem by updating Flash Chip Mode to DOUT during OTA upload
 * Fix hostname issues by not allowing user entry of string formatting and removing from user_config.h (#36)
 *
 * 3.9.11 20170204
 * Fix command I2Cscan
 * Fix not allowed spaces in Topic, ButtonTopic and SwitchTopic
 * Make all TELEMETRY, STATUS and COMMAND message topics unique (#4)
 * Advertise command topic to be used by iobroker (#299)
 * Fix butten (non)detection if no GPIO_KEY1 is defined (#13)
 * Change WeMo serialnumber from 7 decimal chars to 8 hexadecimal chars (#18)
 * Update web page with Build Date/Time, Emulation and mDNS Discovery and Advertise information (#21)
 *
 * 3.9.10 20170130
 * Add WS2812 Color Type selection (RGB or GRB) to user_config.h (#7)
 * Hue api changes to support HUE App(s) (#8)
 *
 * 3.9.9 20170130
 * Add command status 10 showing sensor data
 * Fix hlw status messages if hlw is disabled
 *
 * 3.9.8 20170130
 * Remove GPIO07 and GPIO08 from user selectable (#5)
 *
 * 3.9.7 20170129
 * Fix possible WS2812 exceptions when using emulation
 * Add command Emulation to dynamic configure Belkin WeMo and Hue Bridge for Alexa
 *
 * 3.9.6 20170129
 * Add dynamic sleep for WS2812 animation (#1)
 *
 * 3.9.5 20170128
 * Fix error message in case of wrong Domoticz command
 *
 * 3.9.4 20170127
 * Fix Sonoff Dual Relay switching (#287)
 *
 * 3.9.3 20170127
 * Add confirmation before Restart via webpage
 * Expand Domoticz Configuration webpage with Key, Switch and Sensor Index and
 *   add commands DomoticzSwitchIdx and DomoticzSensorIdx (#86) (#174) (#219)
 * Fix default DHT11 sensor driver selection
 * Fix LedPower status after button press (#279)
 * Add command Sleep 0 - 250 mSec for optional light sleep mode to lower energy consumption (#272)
 *   (Expect overall button/key/switch misses and wrong values on Sonoff Pow)
 * Add Hue brightness extension (#281)
 * Fix Hue brightness and change to call by reference (#283)
 *
 * 3.9.2 20170124
 * Add confirmation before Reset Configuration via webpage (#244)
 * Add WS2812 features (see Wiki commands)
 *
 * 3.9.1 20170124
 * Change PowerOnState function to only trigger when Power On (and not just restart) (#238)
 * Move HLW interrupts back to RAM and make WS2812_DMA optional as it generates Exception on Pow (#264)
 * Add charset=utf-8 to webpages (#266)
 * Update Hue emulation (#268)
 * Fix status module number
 * Add support for domoticz Dimmer on Sonoff_Led and WS2812
 * Fix possible ESP8285 flash problem by updating Flash Chip Mode to DOUT during web upload
 *
 * 3.2.6a 20170120
 * Fix Sonoff Pow compile error (#255)
 * Move HLW interrupts back to ROM (Needed for WS2812 DMA interrupts)
 * Removed all IO config from user_config.h as this will be done by commands or webpage
 * Removed MessageFormat and supports JSON only except POWER/LIGHT status
 * Add command LedPower to control main led (#247)
 * Add more FriendlyNames for Hue (#254)
 * Add DMA support for WS2812 when using pin 3 while other pins work just as well in my case...
 * Add HUE emulation for Alexa (#229)
 * Add basic WS2812 support (#229)
 * Fix Wemo when MQTT is disabled (#245)
 * Revert ButtonTopic and change SwitchTopic1 - 4 to one SwitchTopic
 * Rename MqttUnits to Units
 * Add Mqtt command to enable/disable MQTT
 *
 * 3.2.2a 20170115
 * Add dynamic (Sonoff) Module, user GPIO and sensor selection (one size fits (almost) all)
 * Add support for Sonoff LED
 * Add Seriallog disable after 600 seconds for Sonoff Dual and 4 Channel
 * Add ButtonTopic2 - 4, SwitchTopic1 - 4 and SwitchRetain
 *
 * 3.2.2 20170113
 * Fix PowerOnState 2 functionality after re-applying power (#230)
 *
 * 3.2.1 20170113
 * Fix some failed command decoding (#228)
 * Removed passwords from status messages (#216)
 *
 * 3.2.0 20170111
 * Add I2C BH1750 sensor (#222)
 * Sensor rewrite preparing for online selection
 *
 * 3.1.16 20170109
 * Fix Domoticz possible error condition
 * Remove Wifi password from connection message (#216)
 * Add Configure Other menu item to web page (#209)
 * Add command FriendlyName, field Friendly Name and define FRIENDLY_NAME to be used by Alexa
 *   eliminating current use of MQTT_CLIENT_ID (#209)
 * Add friendlyname to webpage replacing former hostname
 *
 * 3.1.15 20170108
 * Fix Domoticz send key regression with Toggle command
 *
 * 3.1.14 20170107
 * Add support for command TOGGLE (define MQTT_CMND_TOGGLE) when ButtonTopic is in use and not equal to Topic (#207)
 *
 * 3.1.13 20170107
 * Fix web console command input when SUB_PREFIX contains '/' (#152)
 * Add command response to web command (#200)
 * Add option to disable MQTT as define USE_MQTT in user_config.h (#200)
 *
 * 3.1.12 20170106
 * Add OTA retry to solve possible HTTP transient errors (#204)
 * Fix MQTT host discovery
 *
 * 3.1.11 20170105
 * Add mDNS to advertise webserver as <hostname>.local/
 *
 * 3.1.10 20170105
 * Fix ButtonTopic when SUB_PREFIX = PUB_PREFIX
 * Add workaround for possible MQTT queueing when SUB_PREFIX = PUB_PREFIX
 * Add optional MQTT host discovery using define USE_DISCOVERY in user_config.h (#115)
 *
 * 3.1.9 20170104
 * Fix Power Blink start position (toggled)
 * Change PulseTime increments: 1 .. 111 in 0.1 sec (max 11 seconds) and 112 .. 64900 in seconds (= 12 seconds until 18 hours) (#188)
 * Add support for SUB_PREFIX = PUB_PREFIX (#190)
 *
 * 3.1.8 20170103
 * Add retain flag to LWT offline and only send "tele/sonoff/LWT Offline" (#179)
 * Change retained LWT Online message to only send "tele/sonoff/LWT Online"
 *
 * 3.1.7 20161231
 * Add retained message LWT Online when sonoff makes MQTT connection (#179)
 *
 * 3.1.6 20161230
 * Add blinking using commands BlinkTime, BlinkCount and Power Blink|3|BlinkOff|4 (#165)
 *
 * 3.1.5 20161228
 * Fix serial space command exception (28)
 *
 * 3.1.4 20161227
 * Fix MQTT subscribe regression exception (3) (#162)
 * Fix serial empty command exception (28)
 *
 * 3.1.3 20161225
 * Extent Domoticz configuration webpage with optional indices (#153)
 * Fix multi relay legacy tele message from tele/sonoff/2/POWER to tele/sonoff/POWER2
 * Add support for iTead Motor Clockwise/Anticlockwise
 *
 * 3.1.2 20161224
 * Extent command PowerOnState with toggle at power on (option 2 is now option 3!) (#156)
 *
 * 3.1.1 20161223
 * Add support for Sonoff Touch and Sonoff 4CH (#40)
 * Update DomoticzIdx and DomoticzKeyIdx with relay/key index (DomoticzIdx1/DomoticzKeyIdx1)
 * Add command PowerOnState to control relay(s) at power on (#154)
 *
 * 3.1.0 20161221
 * Add Sonoff Pow measurement smoothing
 * Fix serial command topic preamble error (#151)
 * Fix 2.x to 3.x migration inconsistencies (#146)
 *
 * 3.0.9 20161218
 * Add Sonoff Pow voltage reading when relay is on but no load present (#123)
 *
 * 3.0.8 20161218
 * Add temperature conversion to Fahrenheit as option in user_config.h (TEMP_CONVERSION) (#145)
 *
 * 3.0.7 20161217
 * Add user_config_override.h to be used by user to override some defaults in user_config.h (#58)
 * Fix Sonoff Pow low power (down to 4W) intermittent measurements (#123)
 *
 * 3.0.6 20161217
 * Fix MQTT_CLIENT_ID starting with % sign as in "%06X" (#142)
 * Add auto power off after PulseTime * 0.1 Sec to relay 1 (#134)
 *
 * 3.0.5 20161215
 * Add more control over LED with command LedState options (#136, #143)
 *   LED_OFF (0), LED_POWER (1), LED_MQTTSUB (2), LED_POWER_MQTTSUB (3), LED_MQTTPUB (4), LED_POWER_MQTTPUB (5), LED_MQTT (6), LED_POWER_MQTT (7)
 * Add option WIFI_RETRY (4) to command WifiConfig to allow connection retry to other AP without restart (#73)
 *
 * 3.0.4 20161211
 * Fix intermittent Domoticz update misses (#133)
 *
 * 3.0.3 20161210
 * Fix compiler warnings (#132)
 * Remove redundant code
 * Fix Domoticz pushbutton support
 *
 * 3.0.2 20161209
 * Add pushbutton to SwitchMode (#130)
 *
 * 3.0.1 20161209
 * Fix initial config
 *
 * 3.0.0 20161208
 * Migrate and clean-up flash layout
 *   Settings from version 2.x are saved but settings from version 3.x can not be used with version 2.x
 * Change SEND_TELEMETRY_RSSI to SEND_TELEMETRY_WIFI and add AP and SSID to telemetry
 * Split long JSON messages
 * Fix inconsistent status messages
 * Fix all status messages to return JSON if enabled
 * Remove relay index in cmnd/sonoff/<relay>/POWER now changed
 *   to cmnd/sonoff/POWER for single relay units
 *   and cmnd/sonoff/POWER<relay> for multi relay units like Sonoff dual
 * Add retain option to Power/Light status controlled by command PowerRetain On|Off (#126)
 *
 * 2.1.2 20161204
 * Add support for second wifi AP (#73)
 * Update command WifiConfig
 * Fix possible WifiManager hang
 *
 * 2.1.1a 20161203
 * Fix scan for wifi networks if WeMo is enabled
 * Fix syslog setting using web page
 *
 * 2.1.1 20161202
 * Add support for ElectroDragon second relay and button (only toggle with optional ButtonTopic) (#110)
 *
 * 2.1.0 20161202
 * Add optional EXPERIMENTAL TLS to MQTT (#49)
 * Fix MQTT payload handling (#111)
 * Optimzed WeMo code
 *
 * 2.0.21a 20161201
 * Fix WeMo PowerPlug emulation
 *
 * 2.0.21 20161130
 * Add Belkin WeMo PowerPlug emulation enabled with USE_WEMO_EMULATION in user_config.h (Heiko Krupp) (#105, #109)
 *
 * 2.0.20 20161130
 * Relax MQTTClient naming but only allows hexadecimal uppercase numbers (#107)
 * Add I2C support with command I2CScan
 * Add I2C sensor driver for HTU21 as alternate sensor using TH10/16 connectors (Heiko Krupp) (#105)
 * Add I2C sensor driver for BMP085/BMP180/BMP280/BME280 as alternate sensor using TH10/16 connectors
 *
 * 2.0.19a 20161127
 * Add support for ButtonTopic and ButtonRetain to wall switch function
 * Add pullup to SWITCH_PIN and command SwitchMode to syntax
 *
 * 2.0.18 20161126
 * Add SUB_PREFIX multi level support allowing 'cmnd' or 'cmnd/level2/level3'
 * Add wall switch function to GPIO14 and command SwitchMode (Alex Scott) (#103)
 *
 * 2.0.17 20161123
 * Calibrate HLWPCAL from 12345 to 12530
 * Add alternative sensor driver DHT2 using Adafruit DHT library
 * Add define MESSAGE_FORMAT to user_config.h
 * Throttle console messages
 * Shorten JSON messages
 * Fix possible Panic
 * Fix User mode webserver security
 *
 * 2.0.16 20161118
 * Add alternative sensor driver DS18x20 using OneWire library (#95)
 * Change sensor MQTT message from tele/sonoff/TEMPERATURE to tele/sonoff/DHT/TEMPERATURE or
 *   tele/sonoff/DS18B20/TEMPERATURE or tele/sonoff/DS18x20/1/TEMPERATURE
 * Add sensors to root webpage and auto refresh every 4 seconds (#92)
 * Add optional JSON messageformat to all telemetry data
 * Enforce minimum TelePeriod to be 10 seconds
 * Fix Energy Yesterday reset after restart
 * Add Energy Today restore after controlled restart
 *
 * 2.0.15 20161116
 * Change TODAY_POWER and PERIOD_POWER to TODAY_ENERGY and PERIOD_ENERGY
 * Fix serial regression
 * Fix syslog hangs when loghost is unavailable
 *
 * 2.0.14 20161115
 * Add HLW threshold delay
 * Fix HLW intermittent current deviation
 * Fix button functionality during wificonfig
 * Add CRC check to DS18B20 sensor (#88)
 *
 * 2.0.13 20161113
 * Add additional upload error code descriptions
 * Add PlatformIO support (#80)
 *
 * 2.0.12 20161113
 * Fix Serial and Web response regression when no MQTT connection available
 * Fix Sonoff Dual power telemetric data for second relay
 * Removed MQTT password from Information web page
 * Hide MQTT password from Configure MQTT web page
 *
 * 2.0.11 20161111
 * Rewrite button and web toggle code
 * Fix NTP sync
 * Add HLW calibration commands HLWPCAL, HLWUCAL and HLWICAL (need define USE_POWERCALIBRATION)
 * Fix power threshold tests
 *
 * 2.0.10 20161109
 * Add additional Domoticz define (#63)
 * Add defines MQTT_STATUS_ON and MQTT_STATUS_OFF in user_config.h to select status On/Off string
 * Fix status response differences (#65)
 * Fix divide by zero exception (#70)
 * Fix syslog loop exception
 *
 * 2.0.9 20161108
 * clarify MODULE in user_config.h
 * Fix hlw false values
 *
 * 2.0.8 20161108
 * Add initial status after power on
 * Seperate driver files
 * Fix hlw code and calibrate Pow
 * Move user config defines to user_config.h (#61)
 *
 * 2.0.7 20161030
 * Make Ticker mandatory
 * Add Domoticz support (Increase MQTT_MAX_PACKET_SIZE to 400) (#54)
 * Add command MessageFormat 0|1 to select either legacy or JSON output
 *
 * 2.0.6 20161024
 * Add Sonoff Pow power factor
 * Initial support for up to four relays using iTEAD PSB (4Channel)
 *   - Currently only supports one button (All buttons behave the same)
 *   - Use command MODEL 4 to select four relay option
 *     (After first power on it will support 2 relays like Sonoff Dual)
 * Fix ledstate
 * Add command Status 9 to display Sonoff Pow thresholds
 * Add commands PowerLow, PowerHigh, VoltageLow, VoltageHigh, CurrentLow and CurrentHigh for use
 *   with Sonoff Pow thresholds
 *
 * 2.0.5 20161018
 * Add updates to user_config.h - moved SEND_TELEMETRY_DS18B20 and SEND_TELEMETRY_DHT to module area.
 *   As Sonoff TH10/16 does not have the logic installed for GPIO04 You'll have to select ONE of both
 * Add Sonoff Pow support (experimental until Pow tested)
 * Add command Status 8 to display Sonoff Pow energy values
 * Add command MqttUnits On|Off to add units to values
 * Change web main page header character size
 * Change On/Off to ON/OFF status messages to satisfy openHAB
 * Change TEMP to TEMPERATURE and HUM to HUMIDITY
 *
 * 2.0.4 20161009
 * Add MQTT_BUTTON_RETAIN, SAVE_DATA and SAVE_STATE defines to user_config.h (#35)
 * Update ButtonRetain to remove retained message(s) from broker when turned off
 * Add Retain for second relay on Sonoff Dual
 * Provide power status messages with device topic index if requested
 *
 * 2.0.3 20161008
 * Update wifi initialization
 * Add command BUTTONRETAIN for optional MQTT retain on button press (#35)
 * Add command SAVESTATE to disable power state save. May be used with MQTT retain
 *
 * 2.0.2 20161006
 * Fix wifi issue 2186
 *
 * 2.0.1 20161002
 * Fix button press
 *
 * 2.0.0 20161002
 * Update Sonoff TH10/16 sensor pins (My TH10 only has GPIO14 connected)
 * Add full support for Sonoff dual
 *
 * 1.0.35 20160929
 * Add more lines to console
 * Add timeout and disable MQTT on web upload
 * Add command SAVEDATA to control parameter save (for flash wear afficionados) (#30)
 *
 * 1.0.34 20160926
 * Fix button press six and seven
 * Add more information to webserver
 *
 * 1.0.33 20160915
 * Better WPS error message
 * Separate webserver code from support.ino into webserver.ino
 * Fix webserver User by removing unwanted restart option
 *
 * 1.0.32 20160913
 * Add Wifi Protected Setup (WPS) as third option for initial config
 * Add command WIFICONFIG replacing deprecated command SMARTCONFIG
 * Add option WIFICONFIG 3 to start WPSconfig
 * Add option WIFICONFIG 0 to start saved Wifi config tool (WPSconfig, Smartconfig or Wifimanager)
 * Change button behaviour - See Wiki
 *
 * 1.0.31 20160907
 * Fix DS18B20 misread if teleperiod = 2
 * Tuned sensor code
 * Updated prefered ElectroDragon connection to Relay 1 and Button 1
 * Moved SONOFF and ELECTRO_DRAGON port config to user_config.h
 *
 * 1.0.30 20160902
 * Fix command TELEPERIOD 0
 * Add ESP- tag to UDP log message for easy rsyslogd filtering
 * Add ElectroDragon (Relay 2 only) functionality. Select with #define MODULE ELECTRO_DRAGON
 * Add ? as null message alternative
 * Add DHT temperature and humidity telemetry support. Enable with #define SEND_TELEMETRY_DHT
 * Add DS18B20 temperature telemetry support. Enable with #define SEND_TELEMETRY_DS18B20
 * Restrict HOSTNAME, MQTTCLIENT, TOPIC and BUTTONTOPIC in topic mode only
 *
 * 1.0.29 20160831
 * Allow UPGRADE, OTAURL, RESTART, RESET, MQTTHOST, MQTTPORT, MQTTUSER, MQTTPASSWORD and WEBSERVER also in group mode
 *
 * 1.0.28 20160831
 * Add webserver state to status 5
 * Add optional PUB_PREFIX2 (tele) for telemetry usage
 * Add command TELEPERIOD
 * Fix syntax message
 * Change memory status display
 *
 * 1.0.27 20160831
 * Add sketch flash size
 * Add console to webserver
 * Add command weblog
 * Change WifiManager web pages to minimal
 * Change display default hostname and MQTT client id in webserver
 * Change HTTP command interface to http://sonoff-1234/cm?cmnd=light 2
 * Change HEARTBEAT to UPTIME
 *
 * 1.0.26 20160829
 * Add define USE_WEBSERVER to disable web server code in source
 * Add file upload as alternative for ota upload to webserver
 * Add information to webserver
 * Add command hostname
 * Add command logport
 * Change HTTP command interface to http://sonoff-1234/cmd?cmnd=light 2
 * Change button behaviour with regards to Smartconfig and OTA upload. See README.md
 * Enforce default hostname to either "%s-%04d" or user defined without any %
 * Enforce default mqtt client id to either "DVES_%06X" or user defined without any %
 *
 * 1.0.25 20160822
 * Remove config system halts to keep ota available
 *
 * 1.0.24 20160821
 * Add test for MQTT_SUBTOPIC
 * Change log range to LOG_LEVEL_ALL
 * Change MQTT introduction messages
 * Moved MQTT_MAX_PACKET_SIZE warning message to introduction messages
 *
 * 1.0.23 20160821
 * Add option USE_SPIFFS to move config from flash to spiffs
 * Add webserver with options 0 (off), 1 (user) and 2 (admin)
 * Add HTTP command interface (http://sonoff-1234/c?cmnd=light 2)
 * Add wifimanager countdown counter
 * Add command line webpage
 * Add relay control to wifimanager
 * Add restart option 99 to force restart
 * Fix wifi hostname
 * Fix NETBIOS hostname problem by reducing default hostname length
 * Fix possible exception if WIFI_HOSTNAME is changed
 * Fix upgrade messages
 * Reduce memory use by redesigning config routines
 * Split syntax message
 * Rename define SERIAL_IO to USE_SERIAL
 *
 * 1.0.22 20160814
 * Add all MQTT parameters for configuration
 * Add wifimanager to configure Wifi and MQTT via web server
 * Change NTP time handling
 * Fix Smartconfig parameter buffer overflow
 * Fix PlatformIO warnings
 *
 * 1.0.21 20160808
 * Remove semaphore as subscription flooding (more than 15 subscriptions per second) is managed by SDK (LmacRxBlk:1)
 * Add optional RTC interrupt (define USE_TICKER) to keep RTC synced during subscription flooding
 * Remove heartbeatflag
 *
 * 1.0.20 20160805
 * Add semaphore to handle out of memory when too many subscriptions requested
 * Use Daylight Saving (DST) parameters from user_config.h when timezone = 99
 * Add status 7 option displaying RTC information
 * Add ledstate to status 0
 *
 * 1.0.19 20160803
 * Fix possible MQTT_CLIENT_ID induced Exception(28)
 *
 * 1.0.18 20160803
 * Moved Cfg_Default
 * Fix negative data handling
 * Remove MQTT information from status 1 and add labels to status 1
 * Add mac address to status 5
 * Add MQTT ClientId, UserId and Password to status 6
 *
 * 1.0.17 20160731
 * Better variable range checking
 * Change ambiguous connection messages
 * Add timestamp to serial message
 *
 * 1.0.16 20160729
 * Moved wifi, rtc, syslog and config to support.ino
 * Fixed button action when buttontopic is used. Introduced with 1.0.15
 * Better buffer overflow checks (strlcpy)
 *
 * 1.0.15 20160728
 * Removed pubsubclient config changes from sonoff.ino as it doesn't work
 *   reapply MQTT_MAX_PACKET_SIZE 256 and MQTT_KEEPALIVE 120 to PubSubClient.h
 * Add status 0 option displaying all status messages
 * Change MQTT_MAX_PACKET_SIZE from 1024 to 256
 * Add buffer overflow checks (snprintf and strncpy)
 * Implemented common string sizes
 *
 * 1.0.14 20160722
 * Seperate user config from sonoff.ino to user_config.h (pucebaboon)
 * Change defaults from sidnas2 to domus1
 * Add MQTT status message as status 6 (pucebaboon)
 * Add status type to message (pucebaboon)
 * Add pubsubclient config changes to sonoff.ino (pucebaboon)
 *
 * 1.0.13 20160702
 * Add Ledstate 1 option to show power state on led
 *
 * 1.0.12 20160529
 * Allow disable of button topic using "0"
 *
 * 1.0.11 20160524
 * Provide button response if MQTT connection lost
 *
 * 1.0.10 20160520
 * Add optional button topic to assist external MQTT clients
 * Change version notation
 * Reset default values
 *
 * 1.0.9  20160503
 * Add more blinks
 * Add reset 2 option erasing flash
 * Add status 5 option displaying network info
 * Add syslog check for Wifi connection
 * Resize MqttPublish log array
 * Change Wifi smartconfig active from 100 to 60 seconds
 * Update Wifi initialization
 *
 * 1.0.8  20160430
 * Remove use of Wifi config data from SDK
 * Add status 3 (syslog info) and status 4 (flash info)
 * Add restart option to button (5 quick presses)
 *
 * 1.0.7  20160420
 * Add UDP syslog support
 * Change HOST command to MQTTHOST command
 * Add commands SYSLOG, SERIALLOG and LOGHOST
 * Change hostname to lower case to distinguise between open-sdk version
 * Add support for ESP-12F used in my modified wkaku power socket switch
 * Fix timezone command
 * Add RTC month names for future use
 * Modify button code
 * Remove initialization errors by better use of MQTT loop
 *
 * 1.0.6  20160406
 * Removed Wifi AP mode (#1)
 * Add test for Arduino IDE version >= 1.6.8
 * Fix RTC time sync code
 *
 * 1.0.5  20160310
 * Initial public release
 * Show debug info by selecting option from IDE Tools Debug port: Serial
 */
