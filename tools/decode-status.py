#!/usr/bin/env python

"""
  decode-status.py - decode status for Sonoff-Tasmota

  Copyright (C) 2018 Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

Requirements:
   - Python
   - pip json pycurl

Instructions:
    Execute command with option -d to retrieve status report from device or
    get a copy of the status message with http command http://sonoff/cm?cmnd=status%200
    and store it in file status.json

Usage:
    ./decode-status.py -d <hostname or IP address>
        or
    ./decode-status.py -f <JSON status information file>

Example:
    ./decode-status.py -d sonoff1
        or
	./decode-status.py -f status.json
"""

import io
import os.path
import json
import pycurl
from sys import exit
from optparse import OptionParser
from StringIO import StringIO

a_on_off = ["OFF","ON "]

a_setoption = [
    "Save power state and use after restart",
    "Restrict button actions to single, double and hold",
    "Show value units in JSON messages",
    "MQTT",
    "Respond as Command topic instead of RESULT",
    "MQTT retain on Power",
    "MQTT retain on Button",
    "MQTT retain on Switch",
    "Convert temperature to Fahrenheit",
    "MQTT retain on Sensor",
    "MQTT retained LWT to OFFLINE when topic changes",
    "Swap Single and Double press Button",
    "Do not use flash page rotate",
    "Button single press only",
    "Power interlock mode",
    "Do not allow PWM control",
    "Reverse clock",
    "Allow entry of decimal color values",
    "CO2 color to light signal",
    "HASS discovery",
    "Do not control Power with Dimmer",
    "Energy monitoring while powered off",
    "MQTT serial",
    "Rules until 5.14.0b",
    "Rules once mode until 5.14.0b",
    "KNX",
    "Use Power device index on single relay devices",
    "KNX enhancement",
    "RF receive decimal",
    "IR receive decimal",
    "Enforce HASS light group",""]

a_features = [[
    "","","USE_I2C","USE_SPI",
    "USE_DISCOVERY","USE_ARDUINO_OTA","USE_MQTT_TLS","USE_WEBSERVER",
    "WEBSERVER_ADVERTISE","USE_EMULATION","MQTT_PUBSUBCLIENT","MQTT_TASMOTAMQTT",
    "MQTT_ESPMQTTARDUINO","MQTT_HOST_DISCOVERY","USE_ARILUX_RF","USE_WS2812",
    "USE_WS2812_DMA","USE_IR_REMOTE","USE_IR_HVAC","USE_IR_RECEIVE",
    "USE_DOMOTICZ","USE_DISPLAY","USE_HOME_ASSISTANT","USE_SERIAL_BRIDGE",
    "USE_TIMERS","USE_SUNRISE","USE_TIMERS_WEB","USE_RULES",
    "USE_KNX","","",""
    ],[
    "USE_CONFIG_OVERRIDE","BE_MINIMAL","USE_ALL_SENSORS","USE_CLASSIC",
    "USE_KNX_NO_EMULATION","","","",
    "","","","",
    "","","","",
    "","","","",
    "","","","",
    "","","VTABLES_IN_FLASH","PIO_FRAMEWORK_ARDUINO_LWIP_HIGHER_BANDWIDTH",
    "PIO_FRAMEWORK_ARDUINO_LWIP2_LOW_MEMORY","PIO_FRAMEWORK_ARDUINO_LWIP2_HIGHER_BANDWIDTH","DEBUG_THEO","USE_DEBUG_DRIVER"
    ],[
    "","USE_ADC_VCC","USE_ENERGY_SENSOR","USE_PZEM004T",
    "USE_DS18B20","USE_DS18x20_LEGACY","USE_DS18x20","USE_DHT",
    "USE_SHT","USE_HTU","USE_BMP","USE_BME680",
    "USE_BH1750","USE_VEML6070","USE_ADS1115_I2CDEV","USE_ADS1115",
    "USE_INA219","USE_SHT3X","USE_MHZ19","USE_TSL2561",
    "USE_SENSEAIR","USE_PMS5003","USE_MGS","USE_NOVA_SDS",
    "USE_SGP30","USE_SR04","USE_SDM120","USE_SI1145",
    "USE_SDM630","USE_LM75AD","USE_APDS9960","USE_TM1638"
    ],[
    "","","","",
    "","","","",
    "","","","",
    "","","","",
    "","","","",
    "","","","",
    "","","","",
    "","","",""]]

usage = "usage: decode-status {-d | -f} arg"
parser = OptionParser(usage)
parser.add_option("-d", "--dev", action="store", type="string",
                  dest="device", help="device to retrieve status from")
parser.add_option("-f", "--file", metavar="FILE",
                  dest="jsonfile", default="status.json", help="status json file (default: status.json)")
(options, args) = parser.parse_args()

if (options.device):
    buffer = StringIO()
    url = str("http://{}/cm?cmnd=status%200".format(options.device))
    c = pycurl.Curl()
    c.setopt(c.URL, url)
    c.setopt(c.WRITEDATA, buffer)
    c.perform()
    c.close()
    body = buffer.getvalue()
    obj = json.loads(body)
else:
    jsonfile = options.jsonfile
    fp = open(jsonfile, "r")
    obj = json.load(fp)
    fp.close()

def StartDecode():
#    print("Decoding\n{}".format(obj))

    if ("Time" in obj["StatusSNS"]):
        time = str(" from status report taken at {}".format(obj["StatusSNS"]["Time"]))
    if ("FriendlyName" in obj["Status"]):
        print("\nDecoding information for device {}{}".format(obj["Status"]["FriendlyName"][0], time))

    if ("SetOption" in obj["StatusLOG"]):
        options = []
        option = obj["StatusLOG"]["SetOption"][0]
        i_option = int(option,16)
        for i in range(len(a_setoption)):
            if (a_setoption[i]):
                state = (i_option >> i) & 1
                options.append(str("{0:2d} ({1}) {2}".format(i, a_on_off[state], a_setoption[i])))

        print("\nOptions")
        for i in range(len(options)):
            print("  {}".format(options[i]))


    if ("Features" in obj["StatusMEM"]):
        features = []
        for f in range(5):
            feature = obj["StatusMEM"]["Features"][f]
            i_feature = int(feature,16)
            if (f == 0):
                features.append(str("Language LCID = {}".format(i_feature & 0xFFFF)))
            else:
                for i in range(len(a_features[f -1])):
                    if ((i_feature >> i) & 1):
                        features.append(a_features[f -1][i])

        features.sort()
        print("\nFeatures")
        for i in range(len(features)):
            print("  {}".format(features[i]))

if __name__ == "__main__":
    try:
        StartDecode()
    except Exception as e:
        print("E: {}".format(e))