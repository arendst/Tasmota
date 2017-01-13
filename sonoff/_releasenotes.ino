/* 3.2.2 20170113
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
 * Resize mqtt_publish log array
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
