<img src="/tools/logo/TASMOTA_FullLogo_Vector.svg" alt="Logo" align="right" height="76"/>

# Basic API information

Tasmota can easily be extended by developers using provided function pointers as callback Ids. This document lists the available callback function Ids. Read [Sensor API](https://tasmota.github.io/docs/Sensor-API) for more information.

Callback availability can be checked by searching for either XdrvCall, XsnsCall, XdspCall, XnrgCall and XlgtCall.

## Driver, Sensor, Energy and Light Callback Ids

The following table lists Callback Ids and their availability for a Driver, Sensor or Energy service.

Callback Id                 | Bool | xdrv | xsns | xnrg | xlgt | Description
----------------------------|------|------|------|------|------|----------------------------------
FUNC_SETTINGS_OVERRIDE      |      |  x   |      |      |      | Override start-up settings
FUNC_PIN_STATE              |  x   |  1   |  2   |      |      | At GPIO configuration
FUNC_MODULE_INIT            |  x   |  1   |      |      |  2   | Init module specific parameters
FUNC_PRE_INIT               |      |  1   |      |  2   |      | Once GPIO have been established
FUNC_INIT                   |      |  1   |  3   |  2   |      | At end of initialisation
FUNC_LOOP                   |      |  1   |  2   |      |      | In main loop
FUNC_EVERY_50_MSECOND       |      |  1   |  2   |      |      |
FUNC_EVERY_100_MSECOND      |      |  1   |  2   |      |      |
FUNC_EVERY_200_MSECOND      |      |      |      |  x   |      |
FUNC_EVERY_250_MSECOND      |      |  1   |  3   |  2   |      |
FUNC_EVERY_SECOND           |      |  1   |  2   |      |      |
FUNC_SAVE_AT_MIDNIGHT       |      |      |  x   |      |      | At midnight
FUNC_SAVE_BEFORE_RESTART    |      |  2   |  1   |      |      | Just before a planned restart
FUNC_AFTER_TELEPERIOD       |      |  2   |  1   |      |      | At end of teleperiod
FUNC_JSON_APPEND            |      |  2   |  1   |  3   |      | Extend teleperiod JSON text
FUNC_WEB_SENSOR             |      |  2   |  1   |  3   |      | Add sensor data to web GUI
FUNC_COMMAND                |  x   |  1   |  2   |  3   |  4   | When a command is not recognized
FUNC_COMMAND_DRIVER         |  x   |  x   |      |      |      | When command Driver\<id\> is executed
FUNC_COMMAND_SENSOR         |  x   |      |  x   |      |      | When command Sensor\<id\> is executed
FUNC_MQTT_SUBSCRIBE         |      |  x   |      |      |      | At end of MQTT subscriptions
FUNC_MQTT_INIT              |      |  x   |      |      |      | Once at end of MQTT connection
FUNC_MQTT_DATA              |  x   |  x   |      |      |      | Before decoding command
FUNC_SET_POWER              |      |  x   |      |      |      | Before setting relays
FUNC_SET_DEVICE_POWER       |  x   |  x   |      |      |      | Set relay
FUNC_SHOW_SENSOR            |      |  x   |      |      |      | When FUNC_JSON_APPEND completes
FUNC_ANY_KEY                |      |  x   |      |      |      |
FUNC_ENERGY_EVERY_SECOND    |      |      |      |  x   |      |
FUNC_ENERGY_RESET           |      |      |      |  x   |      |
FUNC_RULES_PROCESS          |  x   |  x   |      |      |      | Process specific rule
FUNC_SERIAL                 |  x   |  1   |      |  2   |  3   | Process serial data
FUNC_FREE_MEM               |      |  x   |      |      |      | Show free memory for debugging
FUNC_BUTTON_PRESSED         |  x   |  x   |      |      |      | When a button is pressed
FUNC_WEB_ADD_BUTTON         |      |  1   |  2   |      |      | Add a Configuration Button to GUI
FUNC_WEB_ADD_MAIN_BUTTON    |      |  1   |  2   |      |      | Add a main button to GUI
FUNC_WEB_ADD_HANDLER        |      |  1   |  2   |      |      | Add a webserver handler
FUNC_SET_CHANNELS           |      |  2   |      |      |  1   |
FUNC_SET_SCHEME             |      |      |      |      |  x   |

The numbers represent the sequence of execution

## Display Call back Ids

The following table lists all Callback Ids for a Display service.

Callback Id                   | Bool | Description
------------------------------|------|---------------------
FUNC_DISPLAY_INIT_DRIVER      |      |
FUNC_DISPLAY_INIT             |      |
FUNC_DISPLAY_EVERY_50_MSECOND |      |
FUNC_DISPLAY_EVERY_SECOND     |      |
FUNC_DISPLAY_MODEL            |  x   |
FUNC_DISPLAY_MODE             |      |
FUNC_DISPLAY_POWER            |      |
FUNC_DISPLAY_CLEAR            |      |
FUNC_DISPLAY_DRAW_FRAME       |      |
FUNC_DISPLAY_DRAW_HLINE       |      |
FUNC_DISPLAY_DRAW_VLINE       |      |
FUNC_DISPLAY_DRAW_LINE        |      |
FUNC_DISPLAY_DRAW_CIRCLE      |      |
FUNC_DISPLAY_FILL_CIRCLE      |      |
FUNC_DISPLAY_DRAW_RECTANGLE   |      |
FUNC_DISPLAY_FILL_RECTANGLE   |      |
FUNC_DISPLAY_TEXT_SIZE        |      |
FUNC_DISPLAY_FONT_SIZE        |      |
FUNC_DISPLAY_ROTATION         |      |
FUNC_DISPLAY_DRAW_STRING      |      |
FUNC_DISPLAY_ONOFF            |      |

## Init sequence

The following list shows a typical callback init sequence

```
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
```
