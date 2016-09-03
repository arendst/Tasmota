<img alt="Sonoff" src="https://github.com/arendst/arendst.github.io/blob/master/media/sonoff.jpg" height="200" align="right" /> 
## Sonoff-MQTT-OTA-Arduino
Provide ESP8266 based [itead Sonoff](https://www.itead.cc/sonoff-wifi-wireless-switch.html) with Web, MQTT and 'Over the Air' or OTA firmware using Arduino IDE.

Current version is 1.0.30 - See ```sonoff/_releasenotes``` for change information.

See [Sonoff-MQTT-OTA](https://github.com/arendst/Sonoff-MQTT-OTA) for the ```esp-open-sdk``` version.
## Features
Since it's original conception the following features have evolved:
- Control of sonoff relay by Serial, MQTT or HTTP commands
- Multiple sonoffs can be addressed by ```grouptopic```
- Firmware can be upload via OTA or web upload
- Status messages have been expanded
- UDP syslog messages can be filtered on program starting with ```ESP-```
- The button can send a different MQTT message defined with ```buttontopic```
- In addition to smartconfig a Wifi manager is also available
- A web server provides control of sonoff and contains a firmware upload facility
- Support for ElectroDragon controlling relay 2 by button 1 only
- Support for DHTxx on GPIO14 or DS18B20 on GPIO04 temperature sensors
- Telemetry data can be send using different prefix from status messages
## Prerequisite
Install the ESP8266 Arduino development environment from [esp8266 Arduino](https://github.com/esp8266/Arduino). The software is tested on a Windows PC with Arduino IDE versions 1.6.5r5, 1.6.8, 1.6.9 and 1.6.10 and esp8266 Arduino versions 2.1.0, 2.2.0 and 2.3.0.

- I prefer a standalone version of the IDE allowing easy ESP8266 file manipulation. This can be achieved by downloading the Arduino IDE ZIP file for non admin install. After unzipping and before executing ```arduino.exe``` add an empty directory called ```portable```
- Follow the procedure to install the ESP8266 Arduino development environment
- Copy the ```sonoff``` directory to your sketchfolder

Download and unzip the [pubsubclient](https://github.com/knolleary/pubsubclient) MQTT library into directory ```portable\sketchbook\libraries``` and rename to pubsubclient. Update default values in file ```pubsubclient\src\PubSubClient.h```  
- Change ```MQTT_MAX_PACKET_SIZE``` from 128 to at least 256  
- Change ```MQTT_KEEPALIVE``` from 15 to 120  

Install php and a local web server (ie apache) for OTA and copy directory ```api``` in webroot.

If you want to be able to upload the OTA file from the IDE to your web server perform the following changes to the Arduino IDE environment:

- Copy file ```espupload.py``` to directory ```portable\packages\esp8266\hardware\esp8266\2.3.0\tools``` and change LOCALADDR to refer to your web server
- Replace files ```boards.txt``` and ```platform.txt``` in directory ```portable\packages\esp8266\hardware\esp8266\2.3.0```

This will provide an additional option ```Tools - Upload Using: OTA_upload```.
## Compile and upload
For sonoff select from ```Tools Board Generic ESP8266 Module``` and set the following options:

- Flash Mode: DIO
- Flash Frequency: 40MHz
- Upload Using: Serial
- CPU Frequency: 80MHz
- Flash Size: 1M (64K SPIFFS)
- Debug Port: Disabled
- Debug Level: None
- Reset Method: ck
- Upload Speed: 115200
- Port: Your COM port connected to sonoff

Update ```user_config.h``` with your specific Wifi and MQTT parameters.

Verify source and upload once to sonoff using the serial connection as shown in [Peter Scargill's blog](http://tech.scargill.net/itead-slampher-and-sonoff). 
<img alt="Connection diagram" src="https://github.com/arendst/arendst.github.io/blob/master/media/ProgramESP8266.jpg" height="260" /> 

**Do not connect AC power during the flash cable connection**. 

Verify and upload an OTA image to your web server with option ```Upload Using: OTA_upload```.

Since version 1.0.26 you also may use sonoffs web server and upload the file directly.

Enable debug messages by selecting option Tools Debug Port: Serial.
## Usage
The button on sonoff provides the following features:

- a short press toggles the relay either directly or by sending a MQTT message like ```cmnd/sonoff/light 2```. This will blink the LED twice and sends a MQTT status message like ```stat/sonoff/LIGHT on```
- two short presses toggles the relay. This will blink the LED twice and sends a MQTT status message like ```stat/sonoff/POWER on```
- three short presses start Wifi smartconfig allowing for SSID and Password configuration using an Android mobile phone with the [ESP8266 SmartConfig](https://play.google.com/store/apps/details?id=com.cmmakerclub.iot.esptouch) app. The MQTT server still needs to be configured in the ```user_config.h``` file. The LED will blink during the config period. A single button press during this period will restart sonoff.
- four short presses start Wifi manager providing an Access Point with IP address 192.168.4.1 and a web server allowing the configuration of both Wifi and MQTT parameters. The LED will blink during the config period. A single button press during this period will restart sonoff.
- five short presses will restart sonoff
- six short presses start OTA download of firmware. The green LED is lit during the update
- pressing the button for over four seconds resets settings to defaults as defined in ```user_config.h``` and reboots  sonoff

Sonoff responds to the following MQTT commands:

- the relay can be controlled by ```cmnd/sonoff/power on```, ```cmnd/sonoff/power off``` or ```cmnd/sonoff/power toggle```. The LED will blink twice and sends a MQTT status message like ```stat/sonoff/POWER on```. The same function can be initiated with ```cmnd/sonoff/light on```
- the MQTT topic can be changed with ```cmnd/sonoff/topic sonoff1``` which reboots sonoff and makes it available for MQTT commands like ```cmnd/sonoff1/power on```
- the OTA firmware location can be made known to sonoff by ```cmnd/sonoff/otaurl http://domus1:80/api/sonoff/user1.bin``` where domus1 is your webserver hosting the firmware. Reset to default with ```cmnd/sonoff/otaurl 1```
- upgrade OTA firmware by ```cmnd/sonoff/upgrade 1```
- show status information by ```cmnd/sonoff/status 1```

Most MQTT commands will result in a status feedback like ```stat/sonoff/POWER On```.

Telemetry data wil be send by prefix tele like ```tele/sonoff/TEMP 24.7```
## Commands supported
The firmware supports **serial**, **MQTT** and **Web** Man Machine interface. The serial interface is set to 115200 bps. The MQTT commands are constructed as ```cmnd/sonoff/<command>```. 

The following commands are recognised by both topic and grouptopic:

Command | Version | Description
------- | ------- | -----------
power | | Show current power state as On or Off
power on | | Turn power On
power off | | Turn power Off
power toggle | | Toggle power
power 1 | | Turn power On
power 0 | | Turn power Off
power 2 | | Toggle power
light | | Show current power state as On or Off
light on | | Turn power On
light off | | Turn power Off
light toggle | | Toggle power
light 1 | | Turn power On
light 0 | | Turn power Off
light 2 | | Toggle power
ledstate | 1.0.13 | Show current led state as On or Off
ledstate on | 1.0.13 | Show power state on led
ledstate off | 1.0.13 | Do not show power state on led
ledstate 1 | 1.0.13 | Show power state on led
ledstate 0 | 1.0.13 | Do not show power state on led
ssid | | Show current Wifi SSId
ssid 1 | | Reset Wifi SSId to ```user_config.h``` value and restart
ssid your-ssid | | Set Wifi SSId and restart
password | | Show current Wifi password
password 1 | | Reset Wifi password to ```user_config.h``` value and restart
password your-password | | Set Wifi password and restart
teleperiod | 1.0.28 | Show current telemetry period in seconds
teleperiod off | 1.0.28 | Disable telemetry messages
teleperiod 0 | 1.0.28 | Disable telemetry messages
teleperiod 1 | 1.0.28 | Reset telemetry period to ```user_config.h``` value
teleperiod your-secs | 1.0.28 | Set telemetry period between 2 and 3600 seconds
status | | Show abbreviated status information
status 0 | 1.0.15 | Show all status information
status 1 | | Show more status information
status 2 | | Show firmware information
status 3 | 1.0.8 | Show syslog information
status 4 | 1.0.8 | Show memory information
status 5 | 1.0.9 | Show network information
status 6 | 1.0.14 | Show MQTT information
status 7 | 1.0.20 | Show Real Time Clock information
seriallog 0 | 1.0.7 | Disable serial logging
seriallog 1 | 1.0.7 | Show only error messages
seriallog 2 | 1.0.7 | Show error and info messages
seriallog 3 | 1.0.7 | Show error, info and debug messages
seriallog 4 | 1.0.7 | Show all messages
weblog 0 | 1.0.27 | Disable web logging
weblog 1 | 1.0.27 | Show only error messages
weblog 2 | 1.0.27 | Show error and info messages
weblog 3 | 1.0.27 | Show error, info and debug messages
weblog 4 | 1.0.27 | Show all messages
syslog 0 | 1.0.7 | Disable syslog logging
syslog 1 | 1.0.7 | Show only error messages
syslog 2 | 1.0.7 | Show error and info messages
syslog 3 | 1.0.7 | Show error, info and debug messages
syslog 4 | 1.0.7 | Show all messages
loghost | 1.0.7 | Show current syslog host
loghost 1 | 1.0.7 | Reset syslog host to ```user_config.h``` value
loghost your-host | 1.0.7 | Set syslog host
logport | 1.0.26 | Show current syslog port
logport 1 | 1.0.26 | Reset syslog port to ```user_config.h``` value
logport your-port | 1.0.26 | Set syslog port between 2 and 32766
grouptopic | | Show current MQTT group topic
grouptopic 1 | | Reset MQTT group topic to ```user_config.h``` value and restart
grouptopic your-grouptopic | | Set MQTT group topic and restart
timezone | | Show current timezone
timezone -12 .. 12 | | Set timezone
timezone 99 | 1.0.20 | Use Daylight Saving parameters from ```user_config.h```
restart 1 | | Restart sonoff
restart 99 | 1.0.23 | Force restart sonoff without config save
reset 1 | | Reset sonoff parameters to ```user_config.h``` values and restart
reset 2 | 1.0.9 | Erase flash, reset sonoff parameters to ```user_config.h``` values and restart
mqtthost | | Show current MQTT host
mqtthost 1 | | Reset MQTT host to ```user_config.h``` value and restart
mqtthost your-host | | Set MQTT host and restart
mqttport | 1.0.22 | Show current MQTT port
mqttport 1 | 1.0.22 | Reset MQTT port to ```user_config.h``` value and restart
mqttport your-port | 1.0.22 | Set MQTT port between 2 and 32766 and restart
mqttuser | 1.0.22 | Show current MQTT user name
mqttuser 1 | 1.0.22 | Reset MQTT user name to ```user_config.h``` value and restart
mqttuser your-user | 1.0.22 | Set MQTT user name and restart
mqttpassword | 1.0.22 | Show current MQTT password
mqttpassword 1 | 1.0.22 | Reset MQTT password to ```user_config.h``` value and restart
mqttpassword your-password | 1.0.22 | Set MQTT password and restart
webserver | 1.0.23 | Show current web server state
webserver 0 | 1.0.23 | Stop web server
webserver 1 | 1.0.23 | Start web server in user mode
webserver 2 | 1.0.23 | Start web server in admin mode
otaurl | | Show current otaurl
otaurl 1 | | Reset otaurl to ```user_config.h``` value
otaurl your-otaurl | | Set otaurl
upgrade 1 | | Download ota firmware from your web server and restart
upload 1 | 1.0.23 | Download ota firmware from your web server and restart
smartconfig 1 | | Start smart config for 1 minute
smartconfig 2 | 1.0.22 | Start wifi manager (web server at 192.168.4.1)

The following commands are recognised by topic only:

Command | Version | Description
------- | ------- | -----------
hostname | 1.0.26 | Show current hostname
hostname 1 | 1.0.26 | Reset hostname to ```user_config.h``` value and restart
hostname your-host | 1.0.26 | Set hostname and restart
mqttclient | 1.0.22 | Show current MQTT client
mqttclient 1 | 1.0.22 | Reset MQTT client to ```user_config.h``` value and restart
mqttclient your-client | 1.0.22 | Set MQTT client and restart. May use wildcard %06X to be replaced by last six characters of MAC address
topic | | Show current MQTT topic
topic 1 | | Reset MQTT topic to ```user_config.h``` value and restart
topic your-topic | | Set MQTT topic  AND button topic and restart
buttontopic | 1.0.10 | Show current MQTT button topic
buttontopic 0 | 1.0.10 | Disable use of MQTT button topic
buttontopic 1 | 1.0.10 | Set MQTT button topic to MQTT topic as defined in ```user_config.h```
buttontopic your-topic | 1.0.10 | Set MQTT button topic

If the same topic has been defined to more than one sonoff an individual sonoff can still be addressed by the fall back topic MQTT_CLIENT_ID as defined in user_config.h. The fall back topic will be ```DVES_<last six characters of MAC address>```.
## Tips
- To aid in finding the IP address of sonoff the network name will be ```<MQTT_TOPIC>-<last 4 decimal chars of MAC address>```. So the default name is ```sonoff-1234```. Another option is MQTT command ```status 5```.
- Use the group topic to address several sonoffs with one (restricted) MQTT command.
- Using the Arduino IDE set to 115200 baud and both NL & CR maximum serial output is enabled by command ```seriallog 4```.
- Toggle between Wifi smartconfig and Wifi manager by pressing the button once to restart sonoff.

## Modified kaku power socket switch using ESP-12F
Using parts from itead (5V power supply), aliexpress (Different 5V power supply, ESP-12F and 5V to 3V3 step down) and ebay (5V relay) I modified broken 434MHz kaku power socket switches type PAR-1000 to Wifi wkaku power socket switches.

<img alt="Exterior" src="https://github.com/arendst/arendst.github.io/blob/master/media/wkaku1a.jpg" height="300" /> 
<img alt="Kaku" src="https://github.com/arendst/arendst.github.io/blob/master/media/kaku2a.jpg" height="300" /> 
<img alt="wKaku prototype" src="https://github.com/arendst/arendst.github.io/blob/master/media/wkaku2a.jpg" height="300" /> 
<img alt="wKaku v1.3" src="https://github.com/arendst/arendst.github.io/blob/master/media/wkaku4a.jpg" height="300" /> 
