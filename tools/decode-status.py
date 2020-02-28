#!/usr/bin/env python

"""
  decode-status.py - decode status for Tasmota

  Copyright (C) 2020  Theo Arends

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
    ./decode-status.py -d <hostname or IP address> [-u username] [-p password]
        or
    ./decode-status.py -f <JSON status information file>

Example:
    ./decode-status.py -d sonoff1
    ./decode-status.py -d sonoff1 -p 12345678
        or
	./decode-status.py -f status.json
"""

import io
import os.path
import json
import pycurl
import urllib2
from sys import exit
from optparse import OptionParser
from StringIO import StringIO

a_on_off = ["OFF","ON "]

a_setoption = [[
    "Save power state and use after restart",
    "Restrict button actions to single, double and hold",
    "(not used) Show value units in JSON messages",
    "MQTT enabled",
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
    "MQTT serial binary",
    "Convert pressure to mmHg",
    "KNX enabled",
    "Use Power device index on single relay devices",
    "KNX enhancement",
    "RF receive decimal",
    "IR receive decimal",
    "Enforce HASS light group",
    "Do not show Wifi and Mqtt state using Led"
    ],[
    "Key hold time (ms)",
    "Sonoff POW Max_Power_Retry",
    "Backlog delay (ms)",
    "(not used) mDNS delayed start (Sec)",
    "Boot loop retry offset (0 = disable)",
    "RGBWW remap",
    "IR Unknown threshold",
    "CSE7766 invalid power margin",
    "Ignore hold time (s)",
    "(not used) Number of Tuya MCU relays",
    "Over temperature threshold (celsius)",
    "(not used) Tuya MCU max dimmer value",
    "(not used) Tuya MCU voltage Id",
    "(not used) Tuya MCU current Id",
    "(not used) Tuya MCU power Id",
    "(not used) Energy Tariff1 start hour",
    "(not used) Energy Tariff2 start hour",
    "",
    ],[
    "Timers enabled",
    "Generic ESP8285 GPIO enabled",
    "Add UTC time offset to JSON message",
    "Show hostname and IP address in GUI",
    "Apply SetOption20 to Tuya",
    "mDNS enabled",
    "Use wifi network scan at restart",
    "Use wifi network rescan regularly",
    "Add IR raw data to JSON message",
    "Change state topic from tele/STATE to stat/RESULT",
    "Enable normal sleep instead of dynamic sleep",
    "Force local operation when button/switch topic is set",
    "Do not use retain flag on HOLD messages",
    "Do not scan relay power state at restart",
    "Use _ instead of - as sensor index separator",
    "Disable fast power cycle detection for device reset",
    "Enable TuyaMcuReceived messages over Mqtt",
    "Enable buzzer when available",
    "Enable multi-channels PWM instead of Color PWM",
    "(not used) Limits Tuya MCU dimmers to minimum of 10% (25) when enabled",
    "Enable Weekend Energy Tariff",
    "Select different Modbus registers for Active Energy",
    "Enable hardware energy total counter as reference",
    "Enable HTTP CORS",
    "Enable internal pullup for single DS18x20 sensor",
    "GroupTopic replaces %topic% (0) or fixed topic cmnd/grouptopic (1)",
    "Enable incrementing bootcount when deepsleep is enabled",
    "Do not power off if slider moved to far left",
    "Bypass Compatibility check",
    "Enable resetting of counters after telemetry was sent",
    "Enable shutter support",
    "Invert PCF8574 ports"
    ],[
    "Reduced CT range for Alexa",
    "Use FriendlyNames instead of ShortAddresses when possible",
    "(AWS IoT) publish MQTT state to a device shadow",
    "Enable Device Groups",
    "PWM Dimmer Turn brightness LED's off 5 seconds after last change",
    "PWM Dimmer Turn red LED on when powered off",
    "PWM Dimmer Buttons control remote devices",
    "",
    "","","","",
    "","","","",
    "","","","",
    "","","","",
    "","","","",
    "","","",""
    ]]

a_features = [[
    "USE_ENERGY_MARGIN_DETECTION","USE_LIGHT","USE_I2C","USE_SPI",
    "USE_DISCOVERY","USE_ARDUINO_OTA","USE_MQTT_TLS","USE_WEBSERVER",
    "WEBSERVER_ADVERTISE","USE_EMULATION_HUE","MQTT_PUBSUBCLIENT","MQTT_TASMOTAMQTT",
    "MQTT_ESPMQTTARDUINO","MQTT_HOST_DISCOVERY","USE_ARILUX_RF","USE_WS2812",
    "USE_WS2812_DMA","USE_IR_REMOTE","USE_IR_HVAC","USE_IR_RECEIVE",
    "USE_DOMOTICZ","USE_DISPLAY","USE_HOME_ASSISTANT","USE_SERIAL_BRIDGE",
    "USE_TIMERS","USE_SUNRISE","USE_TIMERS_WEB","USE_RULES",
    "USE_KNX","USE_WPS","USE_SMARTCONFIG","USE_ENERGY_POWER_LIMIT"
    ],[
    "USE_CONFIG_OVERRIDE","FIRMWARE_MINIMAL","FIRMWARE_SENSORS","FIRMWARE_CLASSIC",
    "FIRMWARE_KNX_NO_EMULATION","USE_DISPLAY_MODES1TO5","USE_DISPLAY_GRAPH","USE_DISPLAY_LCD",
    "USE_DISPLAY_SSD1306","USE_DISPLAY_MATRIX","USE_DISPLAY_ILI9341","USE_DISPLAY_EPAPER",
    "USE_DISPLAY_SH1106","USE_MP3_PLAYER","USE_PCA9685","USE_TUYA_DIMMER",
    "USE_RC_SWITCH","USE_ARMTRONIX_DIMMERS","USE_SM16716","USE_SCRIPT",
    "USE_EMULATION_WEMO","USE_SONOFF_IFAN","USE_ZIGBEE","NO_EXTRA_4K_HEAP",
    "VTABLES_IN_IRAM","VTABLES_IN_DRAM","VTABLES_IN_FLASH","PIO_FRAMEWORK_ARDUINO_LWIP_HIGHER_BANDWIDTH",
    "PIO_FRAMEWORK_ARDUINO_LWIP2_LOW_MEMORY","PIO_FRAMEWORK_ARDUINO_LWIP2_HIGHER_BANDWIDTH","DEBUG_THEO","USE_DEBUG_DRIVER"
    ],[
    "USE_COUNTER","USE_ADC_VCC","USE_ENERGY_SENSOR","USE_PZEM004T",
    "USE_DS18B20","USE_DS18x20_LEGACY","USE_DS18x20","USE_DHT",
    "USE_SHT","USE_HTU","USE_BMP","USE_BME680",
    "USE_BH1750","USE_VEML6070","USE_ADS1115_I2CDEV","USE_ADS1115",
    "USE_INA219","USE_SHT3X","USE_MHZ19","USE_TSL2561",
    "USE_SENSEAIR","USE_PMS5003","USE_MGS","USE_NOVA_SDS",
    "USE_SGP30","USE_SR04","USE_SDM120","USE_SI1145",
    "USE_SDM630","USE_LM75AD","USE_APDS9960","USE_TM1638"
    ],[
    "USE_MCP230xx","USE_MPR121","USE_CCS811","USE_MPU6050",
    "USE_MCP230xx_OUTPUT","USE_MCP230xx_DISPLAYOUTPUT","USE_HLW8012","USE_CSE7766",
    "USE_MCP39F501","USE_PZEM_AC","USE_DS3231","USE_HX711",
    "USE_PZEM_DC","USE_TX20_WIND_SENSOR","USE_MGC3130","USE_RF_SENSOR",
    "USE_THEO_V2","USE_ALECTO_V2","USE_AZ7798","USE_MAX31855",
    "USE_PN532_I2C","USE_MAX44009","USE_SCD30","USE_HRE",
    "USE_ADE7953","USE_SPS30","USE_VL53L0X","USE_MLX90614",
    "USE_MAX31865","USE_CHIRP","USE_SOLAX_X1","USE_PAJ7620"
    ],[
    "USE_BUZZER","USE_RDM6300","USE_IBEACON","USE_SML_M",
    "USE_INA226","USE_A4988_STEPPER","USE_DDS2382","USE_SM2135",
    "USE_SHUTTER","USE_PCF8574","USE_DDSU666","USE_DEEPSLEEP",
    "USE_SONOFF_SC","USE_SONOFF_RF","USE_SONOFF_L1","USE_EXS_DIMMER",
    "USE_ARDUINO_SLAVE","USE_HIH6","USE_HPMA","USE_TSL2591",
    "USE_DHT12","USE_DS1624","USE_GPS","USE_HOTPLUG",
    "USE_NRF24","USE_MIBLE","USE_HM10","USE_LE01MR",
    "USE_AHT1x","USE_WEMOS_MOTOR_V1","USE_DEVICE_GROUPS","USE_PWM_DIMMER"
    ],[
    "USE_KEELOQ","","","",
    "","","","",
    "","","","",
    "","","","",
    "","","","",
    "","","","",
    "","","","",
    "","","",""
    ]]

usage = "usage: decode-status {-d | -f} arg"
parser = OptionParser(usage)
parser.add_option("-d", "--dev", action="store", type="string",
                  dest="device", help="device to retrieve status from")
parser.add_option("-u", "--username", action="store", type="string",
                  dest="username", help="username for login", default="admin")
parser.add_option("-p", "--password", action="store", type="string",
                  dest="password", help="password for login", default=None)
parser.add_option("-f", "--file", metavar="FILE",
                  dest="jsonfile", default="status.json", help="status json file (default: status.json)")
(options, args) = parser.parse_args()

if (options.device):
    buffer = StringIO()
    loginstr = ""
    if options.password is not None:
        loginstr = "user={}&password={}&".format(urllib2.quote(options.username), urllib2.quote(options.password))
    url = str("http://{}/cm?{}cmnd=status%200".format(options.device, loginstr))
    c = pycurl.Curl()
    c.setopt(c.URL, url)
    c.setopt(c.WRITEDATA, buffer)
    c.perform()
    c.close()
    body = buffer.getvalue()
    obj = json.loads(body)
else:
    jsonfile = options.jsonfile
    with open(jsonfile, "r") as fp:
        obj = json.load(fp)

def StartDecode():
    print ("\n*** decode-status.py v20200228 by Theo Arends and Jacek Ziolkowski ***")

#    print("Decoding\n{}".format(obj))

    if "StatusSNS" in obj:
        if "Time" in obj["StatusSNS"]:
            time = str(" from status report taken at {}".format(obj["StatusSNS"]["Time"]))

    if "Status" in obj:
        if "FriendlyName" in obj["Status"]:
            print("Decoding information for device {}{}".format(obj["Status"]["FriendlyName"][0], time))

    if "StatusLOG" in obj:
        if "SetOption" in obj["StatusLOG"]:
            options = []

            i = 0
            for r,opt_group in enumerate(a_setoption):
                register = obj["StatusLOG"]["SetOption"][r]

                if r > 0 and len(obj["StatusLOG"]["SetOption"]) == 2: # old firmware: array consisted only of SetOptions 0..31 and resolution
                    break

                if r == 1:
                    if len(register) == 8:            # pre 6.1.1.14: array consisted of SetOptions 0..31, resolution, and SetOptions 50..81
                        i += 18                       # adjust option index and skip 2nd register
                        continue

                    elif len(register) == 36:         # 6.1.1.14: array consists of SetOptions 0..31, SetOptions 32..49, and SetOptions 50..81
                        split_register = [int(register[opt*2:opt*2+2],16) for opt in range(18)] # split register into 18 values

                        for opt_idx, option in enumerate(opt_group):
                            options.append(str("{0:2d} ({1:3d}) {2}".format(i, split_register[opt_idx], option)))
                            i += 1

                if r in (0, 2, 3): #registers 1 and 3 hold binary values
                    for opt_idx, option in enumerate(opt_group):
                        i_register = int(register,16)
                        state = (i_register >> opt_idx) & 1
                        options.append(str("{0:2d} ({1}) {2}".format(i, a_on_off[state], option)))
                        i += 1

            print("\nOptions")
            for o in options:
                print("  {}".format(o))

    if "StatusMEM" in obj:
        if "Features" in obj["StatusMEM"]:
            features = []
            for f in range(6):
                feature = obj["StatusMEM"]["Features"][f]
                i_feature = int(feature,16)
                if f == 0:
                    features.append(str("Language LCID = {}".format(i_feature & 0xFFFF)))
                else:
                    for i in range(len(a_features[f -1])):
                        if (i_feature >> i) & 1:
                            features.append(a_features[f -1][i])

            features.sort()
            print("\nFeatures")
            for f in features:
                print("  {}".format(f))

if __name__ == "__main__":
    try:
        StartDecode()
    except Exception as e:
        print("E: {}".format(e))
