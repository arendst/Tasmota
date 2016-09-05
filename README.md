<img alt="Sonoff" src="https://github.com/arendst/arendst.github.io/blob/master/media/sonoff.jpg" height="200" align="right" /> 
## Sonoff-MQTT-OTA-Arduino
Provide ESP8266 based [itead Sonoff](https://www.itead.cc/sonoff-wifi-wireless-switch.html) with Web, MQTT and 'Over the Air' or OTA firmware using Arduino IDE.

Current version is 1.0.30 - See ```sonoff/_releasenotes``` for change information.

See [Sonoff-MQTT-OTA](https://github.com/arendst/Sonoff-MQTT-OTA) for the ```esp-open-sdk``` version.
## Features
Since it's original conception the following features have evolved:
- Control of Sonoff or [Electrodragon](http://www.electrodragon.com/product/wifi-iot-relay-board-based-esp8266/) relay by Serial, MQTT or HTTP commands
- Multiple Sonoffs can be addressed by ```grouptopic```
- Firmware upload by OTA or via web page upload
- Status messages have been expanded
- UDP syslog messages can be filtered on program name starting with ```ESP-```
- The button can send a different MQTT message defined with ```buttontopic```
- In addition to Smartconfig a Wifi manager is also available
- A web server provides control of Sonoff and contains a firmware upload facility
- Support for Electrodragon controlling relay 2 by button 1 only
- Support for DHTxx on GPIO14 or DS18B20 on GPIO04 temperature sensors
- Telemetry data can be send using optional different prefix from status messages

See [Wiki](https://github.com/arendst/Sonoff-MQTT-OTA-Arduino/wiki) for more information.
