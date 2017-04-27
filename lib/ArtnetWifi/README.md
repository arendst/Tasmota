# ArtnetWifi

An Art-Net library for Wifi-Arduino's (aka. ESP8266 or ESP32)

Note: this library assumes you are using a wifi module

Based on https://github.com/natcl/Artnet [master](https://github.com/natcl/Artnet/archive/master.zip)

## Installation

Place this in your `~/Documents/Arduino/libraries` folder.

## Examples

Different examples are provided, here is a summary of what each example does.

### ArtnetWifiNeoPixel

This example will receive multiple universes via Art-Net and control a strip of ws2811 leds via Adafruit's [NeoPixel library](https://github.com/adafruit/Adafruit_NeoPixel).

### ArtnetWifiTransmit

This is a simple transmitter. Send 3 byte over into the Art-Net, to make a RGB light ramp-up in white.

# Art-Net

Art-Net(tm) is a trademark of Artistic Licence Holdings Ltd. The Art-Net protocol and associated documentation is copyright Artistic Licence Holdings Ltd.

[Art-Net](http://www.artisticlicence.com/WebSiteMaster/User%20Guides/art-net.pdf)
