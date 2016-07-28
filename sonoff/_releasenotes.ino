/*
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
