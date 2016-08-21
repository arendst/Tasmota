/* 1.0.23 20160821
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
 * Removed heartbeatflag
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
