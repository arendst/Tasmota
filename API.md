<img src="/tools/logo/TASMOTA_FullLogo_Vector.svg" alt="Logo" align="right" height="76"/>

# Basic API information

Tasmota can easily be extended by developers using provided function pointers as callback Ids. This document lists the available callback function Ids. See the wiki (https://github.com/arendst/Tasmota/wiki/Sensor-API) for more information.

Callback availability can be checked by searching for either XdrvCall, XsnsCall, XdspCall and XnrgCall.

## Driver, Sensor and Energy Callback Ids
The following table lists Callback Ids and their availability for a Driver, Sensor or Energy service.

Callback Id                 | Bool | Version  | xdrv | xsns | xnrg | Description
----------------------------|------|----------|------|------|------|----------------------------------
FUNC_SETTINGS_OVERRIDE      |      | 6.2.1.19 |  x   |      |      | Override start-up settings
FUNC_MODULE_INIT            |  x   | 6.2.1.17 |  x   |      |      | Init module specific parameters
FUNC_PRE_INIT               |      |          |  x   |      |  x   | Once GPIO have been established
FUNC_INIT                   |      |          |  x   |  x   |  x   | At end of initialisation
FUNC_LOOP                   |      |          |  x   |      |      | In main loop
FUNC_EVERY_50_MSECOND       |      |          |  x   |  x   |      |
FUNC_EVERY_100_MSECOND      |      |          |  x   |  x   |      |
FUNC_EVERY_200_MSECOND      |      |          |      |  x   |  x   |
FUNC_EVERY_250_MSECOND      |      |          |  x   |      |      |
FUNC_EVERY_SECOND           |      |          |  x   |  x   |  x   |
FUNC_PREP_BEFORE_TELEPERIOD |      |          |      |  x   |      | Deprecated. Use a FUNC_EVERY_
FUNC_JSON_APPEND            |      |          |      |  x   |      | Extend teleperiod JSON text
FUNC_WEB_APPEND             |      |          |      |  x   |      | Extend webgui ajax info
FUNC_SAVE_BEFORE_RESTART    |      |          |      |  x   |      | Just before a planned restart
FUNC_COMMAND                |  x   |          |  x   |  x   |      | When a command is not recognized
FUNC_COMMAND_DRIVER         |  x   | 6.4.1.21 |  x   |      |      | When command Driver\<id\> is executed
FUNC_COMMAND_SENSOR         |  x   | 6.4.1.21 |      |  x   |      | When command Sensor\<id\> is executed
FUNC_MQTT_SUBSCRIBE         |      | 5.12.0k  |  x   |      |      | At end of MQTT subscriptions
FUNC_MQTT_INIT              |      | 5.12.0k  |  x   |      |      | Once at end of MQTT connection
FUNC_MQTT_DATA              |  x   | 5.12.0k  |  x   |      |      | Before decoding command
FUNC_SET_POWER              |      |          |  x   |      |      | Before setting relays
FUNC_SET_DEVICE_POWER       |  x   | 6.2.1.18 |  x   |      |      | Set relay
FUNC_SHOW_SENSOR            |      |          |  x   |      |      | When FUNC_JSON_APPEND completes
FUNC_RULES_PROCESS          |  x   | 6.0.0    |  x   |      |      | Process specific rule
FUNC_SERIAL                 |  x   |          |  x   |      |  x   | Process serial data
FUNC_FREE_MEM               |      |          |  x   |      |      | Show free memory for debugging
FUNC_BUTTON_PRESSED         |  x   | 6.2.1.18 |  x   |      |      | When a button is pressed
FUNC_WEB_ADD_BUTTON         |      | 6.2.1.14 |  x   |  x   |      | Add a Configuration Button to GUI
FUNC_WEB_ADD_MAIN_BUTTON    |      | 6.2.1.14 |  x   |  x   |      | Add a main button to GUI
FUNC_WEB_ADD_HANDLER        |      | 6.2.1.14 |  x   |  x   |      | Add a webserver handler

## Display Call back Ids
The following table lists all Callback Ids for a Display service.

Callback Id                   | Bool | Version  | Description
------------------------------|------|----------|---------------------
FUNC_DISPLAY_INIT_DRIVER      |      | 6.1.1.7  |
FUNC_DISPLAY_INIT             |      | 6.1.1.7  |
FUNC_DISPLAY_EVERY_50_MSECOND |      | 6.1.1.7  |
FUNC_DISPLAY_EVERY_SECOND     |      | 6.1.1.7  |
FUNC_DISPLAY_MODEL            |  x   | 6.1.1.7  |
FUNC_DISPLAY_MODE             |      | 6.1.1.7  |
FUNC_DISPLAY_POWER            |      | 6.1.1.7  |
FUNC_DISPLAY_CLEAR            |      | 6.1.1.7  |
FUNC_DISPLAY_DRAW_FRAME       |      | 6.1.1.7  |
FUNC_DISPLAY_DRAW_HLINE       |      | 6.1.1.7  |
FUNC_DISPLAY_DRAW_VLINE       |      | 6.1.1.7  |
FUNC_DISPLAY_DRAW_LINE        |      | 6.1.1.7  |
FUNC_DISPLAY_DRAW_CIRCLE      |      | 6.1.1.7  |
FUNC_DISPLAY_FILL_CIRCLE      |      | 6.1.1.7  |
FUNC_DISPLAY_DRAW_RECTANGLE   |      | 6.1.1.7  |
FUNC_DISPLAY_FILL_RECTANGLE   |      | 6.1.1.7  |
FUNC_DISPLAY_TEXT_SIZE        |      | 6.1.1.7  |
FUNC_DISPLAY_FONT_SIZE        |      | 6.1.1.7  |
FUNC_DISPLAY_ROTATION         |      | 6.1.1.7  |
FUNC_DISPLAY_DRAW_STRING      |      | 6.1.1.7  |
FUNC_DISPLAY_ONOFF            |      | 6.1.1.7  |

## Init sequence
The following list contains the callback init sequence

CFG: Loaded from flash at FB, Count 1581
xdrv - FUNC_SETTINGS_OVERRIDE
xdrv - FUNC_PIN_STATE
xsns - FUNC_PIN_STATE
xdrv - FUNC_MODULE_INIT
xlgt - FUNC_MODULE_INIT
xdrv - FUNC_PRE_INIT
xnrg - FUNC_PRE_INIT
SRC: Restart
xdrv - FUNC_SET_POWER
xlgt - FUNC_SET_CHANNELS
xdrv - FUNC_SET_DEVICE_POWER
Project tasmota Wemos 2 Version 7.0.0.3(tasmota)-STAGE
xdrv - FUNC_INIT
xsns - FUNC_INIT
I2C: ADS1115 found at 0x48
xdrv - FUNC_LOOP
xsns - FUNC_LOOP
xdrv - FUNC_EVERY_50_MSECOND
xlgt - FUNC_SET_CHANNELS
xsns - FUNC_EVERY_50_MSECOND
xdrv - FUNC_EVERY_100_MSECOND
xsns - FUNC_EVERY_100_MSECOND
xdrv - FUNC_EVERY_250_MSECOND
xsns - FUNC_EVERY_250_MSECOND
xdrv - FUNC_EVERY_SECOND
xsns - FUNC_EVERY_SECOND
WIF: Attempting connection...
WIF: Network (re)scan started...
WIF: Attempting connection...
WIF: Attempting connection...
WIF: Attempting connection...
WIF: Network 0, AP1, SSId indebuurt1, Channel 1, BSSId 24:D3:F2:97:C0:A1, RSSI -86, Encryption 1
WIF: Network 1, AP2, SSId indebuurt2, Channel 5, BSSId A0:AB:1B:7D:42:AC, RSSI -42, Encryption 1
WIF: Network 2, AP-, SSId indebuurt3, Channel 12, BSSId 60:E3:27:58:77:E6, RSSI -84, Encryption 1
WIF: Connecting to AP2 indebuurt2 in mode 11N as wemos2...
WIF: Attempting connection...
WIF: Attempting connection...
WIF: Attempting connection...
WIF: Connected
xdrv - FUNC_WEB_ADD_HANDLER
xsns - FUNC_WEB_ADD_HANDLER
HTP: Web server active on wemos2 with IP address 192.168.2.191
NTP: Drift 0, (UTC) Wed Nov 06 13:57:08 2019, (DST) Sun Mar 31 02:00:00 2019, (STD) Sun Oct 27 03:00:00 2019
APP: Boot Count 500
MQT: Attempting connection...
MQT: Connected
MQT: tele/wemos2/LWT = Online (retained)
MQT: cmnd/wemos2/POWER =
MQT: Subscribe to cmnd/wemos2/#
MQT: Subscribe to cmnd/sonoffs/#
MQT: Subscribe to cmnd/DVES_15568C_fb/#
xdrv - FUNC_MQTT_SUBSCRIBE
MQT: tele/wemos2/INFO1 = {"Module":"Generic","Version":"7.0.0.3(tasmota)","FallbackTopic":"cmnd/DVES_15568C_fb/","GroupTopic":"cmnd/sonoffs/"}
MQT: tele/wemos2/INFO2 = {"WebServerMode":"Admin","Hostname":"wemos2","IPAddress":"192.168.2.191"}
MQT: tele/wemos2/INFO3 = {"RestartReason":"Software/System restart"}
MQT: stat/wemos2/RESULT = {"POWER1":"OFF"}
MQT: stat/wemos2/POWER1 = OFF
MQT: stat/wemos2/RESULT = {"POWER2":"ON"}
MQT: stat/wemos2/POWER2 = ON
xdrv - FUNC_MQTT_INIT
CFG: Saved to flash at FA, Count 1582, Bytes 4096