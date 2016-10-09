/* 2.0.4 20161009
 * Add MQTT_BUTTON_RETAIN, SAVE_DATA and SAVE_STATE defines to user_config.h
 * Update ButtonRetain to remove retained message(s) from broker when turned off
 * Add Retain for second relay on Sonoff Dual
 * Provide power status messages with device topic index if requested
 * 
 * 2.0.3 20161008
 * Update wifi initialization
 * Add command BUTTONRETAIN for optional MQTT retain on button press
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
 * Add command SAVEDATA to control parameter save (for flash wear afficionados)
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
 * Removed Wifi AP mode
 * Add test for Arduino IDE version >= 1.6.8
 * Fix RTC time sync code
 * 
 * 1.0.5  20160310
 * Initial public release
 * Show debug info by selecting option from IDE Tools Debug port: Serial
 */
