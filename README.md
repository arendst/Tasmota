## Sonoff-Tasmota
Provide ESP8266 based Sonoff by [iTead Studio](https://www.itead.cc/) and ElectroDragon IoT Relay with Serial, Web and MQTT control allowing 'Over the Air' or OTA firmware updates using Arduino IDE.

Current version is **5.9.0** - See [sonoff/_releasenotes.ino](https://github.com/arendst/Sonoff-Tasmota/blob/master/sonoff/_releasenotes.ino) for change information.

### ATTENTION All versions

Only Flash Mode DOUT is supported. Do not use Flash Mode DIO / QIO / QOUT as it might seem to brick your device.

See [Wiki](https://github.com/arendst/Sonoff-Tasmota/wiki/Theo's-Tasmota-Tips) for background information.

### ATTENTION Version 5 and up

These versions use a new linker script to free flash memory for future code additions. It moves the settings from Spiffs to Eeprom. If you compile your own firmware download the new linker to your IDE or Platformio base folder. See [Wiki > Prerequisite](https://github.com/arendst/Sonoff-Tasmota/wiki/Prerequisite).

Best practice to implement is:
- Open the webpage to your device
- Perform option ``Backup Configuration``
- Upgrade new firmware using ``Firmware upgrade``
- If configuration conversion fails keep the webpage open and perform ``Restore Configuration``

You should now have a device with 32k more code memory to play with.

### Version Information

- Sonoff-Tasmota provides all (Sonoff) modules in one file and starts with module Sonoff Basic.
- Once uploaded select module using the configuration webpage or the commands ```Modules``` and ```Module```.
- After reboot select config menu again or use commands ```GPIOs``` and ```GPIO``` to change GPIO with desired sensor.

<img src="https://github.com/arendst/arendst.github.io/blob/master/media/sonoffbasic.jpg" width="250" align="right" />

See [Wiki](https://github.com/arendst/Sonoff-Tasmota/wiki) for more information.<br />
See [Community](https://groups.google.com/d/forum/sonoffusers) for forum and more user experience.

The following devices are supported:
- [iTead Sonoff Basic](http://sonoff.itead.cc/en/products/sonoff/sonoff-basic)
- [iTead Sonoff RF](http://sonoff.itead.cc/en/products/sonoff/sonoff-rf)
- [iTead Sonoff SV](https://www.itead.cc/sonoff-sv.html)<img src="https://github.com/arendst/arendst.github.io/blob/master/media/sonoff_th.jpg" width="250" align="right" />
- [iTead Sonoff TH10/TH16 with temperature sensor](http://sonoff.itead.cc/en/products/sonoff/sonoff-th)
- [iTead Sonoff Dual](http://sonoff.itead.cc/en/products/sonoff/sonoff-dual)
- [iTead Sonoff Pow](http://sonoff.itead.cc/en/products/sonoff/sonoff-pow)
- [iTead Sonoff 4CH](http://sonoff.itead.cc/en/products/sonoff/sonoff-4ch)
- [iTead Sonoff 4CH Pro](http://sonoff.itead.cc/en/products/sonoff/sonoff-4ch-pro)
- [iTead S20 Smart Socket](http://sonoff.itead.cc/en/products/residential/s20-socket)
- [iTead Slampher](http://sonoff.itead.cc/en/products/residential/slampher-rf)
- [iTead Sonoff Touch](http://sonoff.itead.cc/en/products/residential/sonoff-touch)
- [iTead Sonoff T1](http://sonoff.itead.cc/en/products/residential/sonoff-t1)
- [iTead Sonoff SC](http://sonoff.itead.cc/en/products/residential/sonoff-sc)
- [iTead Sonoff Led](http://sonoff.itead.cc/en/products/appliances/sonoff-led)<img src="https://github.com/arendst/arendst.github.io/blob/master/media/sonoff4ch.jpg" height="250" align="right" />
- [iTead Sonoff BN-SZ01 Ceiling Led](http://sonoff.itead.cc/en/products/appliances/bn-sz01)
- [iTead Sonoff B1](http://sonoff.itead.cc/en/products/residential/sonoff-b1)
- [iTead Sonoff RF Bridge 433](http://sonoff.itead.cc/en/products/appliances/sonoff-rf-bridge-433)
- [iTead Sonoff Dev](https://www.itead.cc/sonoff-dev.html)
- [iTead 1 Channel Switch 5V / 12V](https://www.itead.cc/smart-home/inching-self-locking-wifi-wireless-switch.html)
- [iTead Motor Clockwise/Anticlockwise](https://www.itead.cc/smart-home/motor-reversing-wifi-wireless-switch.html)
- [Electrodragon IoT Relay Board](http://www.electrodragon.com/product/wifi-iot-relay-board-based-esp8266/)
- [AI Light or any my9291 compatible RGBW LED](http://www.ebay.com/itm/172644855726)


### License

This program is licensed under GPL-3.0