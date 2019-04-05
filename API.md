## Sonoff-Tasmota basic API information
Sonoff-Tasmota can easily be extended by developers using provided function pointers as callback Ids. This document lists the available callback function Ids. See the wiki (https://github.com/arendst/Sonoff-Tasmota/wiki/Sensor-API) for more information.

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
