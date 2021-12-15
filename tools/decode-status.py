#!/usr/bin/env python

"""
  decode-status.py - decode status for Tasmota

  Copyright (C) 2021  Theo Arends

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
   - pip json requests

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
import requests
import urllib
from sys import exit
from optparse import OptionParser

a_on_off = ["OFF","ON "]

a_setoption = [[
    "(Settings) Save power state (1) and use after restart",
    "(Button) Control button single press (1) or multipress (0)",
    "(MQTT) Add global temperature/humidity/pressure info to JSON sensor message",
    "(MQTT) Enable (1)",
    "(MQTT) Switch between RESULT (0) or COMMAND (1)",
    "(MQTT) Retain on Power",
    "(MQTT) Retain on Button",
    "(MQTT) Retain on Switch",
    "(Temperature) Switch between Celsius (0) or Fahrenheit (1)",
    "(MQTT) Retain on Sensor",
    "(MQTT) Control LWT message format, Offline (1) or none (0)",
    "(Button) Swap (1) button single and double press functionality",
    "(Settings) Switch between dynamic (0) or fixed (1) slot flash save location",
    "(Button) Support only single press (1) to speed up button press recognition",
    "(Interlock) Power interlock mode",
    "(Light) Switch between commands PWM (1) or COLOR/DIMMER/CT/CHANNEL (0)",
    "(WS2812) Switch between clockwise (0) or counter-clockwise (1)",
    "(Light) Switch between decimal (1) or hexadecimal (0) output",
    "(Light) Pair light signal (1) with CO2 sensor",
    "(HAss) Control automatic discovery (1) (See SetOption59)",
    "(Light) Control power in relation to Dimmer/Color/Ct changes (1)",
    "(Energy) Show voltage even if powered off (1)",
    "(MQTT) Serial",
    "(MQTT) Serial binary",
    "(Pressure) switch between hPa (0)or mmHg (1) unit",
    "(KNX) Enabled",
    "(MQTT, HAss) Switch between POWER (0) or POWER1 (1)",
    "(KNX) Enhancement",
    "(RF) Receive data format hexadecimal (0) or decimal (1)",
    "(IR) Receive data format hexadecimal (0) or decimal (1)",
    "(HAss) enforce autodiscovery as light (1)",
    "(Wifi, MQTT) Control link led blinking (1)"
    ],[
    "(Button) Key hold time (ms)",
    "(Pow) Sonoff POW Max_Power_Retry",
    "(Backlog) Delay (ms)",
    "(not used) mDNS delayed start (Sec)",
    "(Boot loop) Retry offset (0 = disable)",
    "(Light) RGBWW remap",
    "(IR) Unknown threshold",
    "(CSE7766) invalid power margin",
    "(Button) Ignore hold time (s)",
    "(Wifi) Gratuitous ARP repeat time",
    "(Temperature) Over temperature threshold (celsius)",
    "(Rotary) Max allowed steps",
    "(not used) Tuya MCU voltage Id",
    "(not used) Tuya MCU current Id",
    "(not used) Tuya MCU power Id",
    "(not used) Energy Tariff1 start hour",
    "(not used) Energy Tariff2 start hour",
    "",
    ],[
    "(Timers) Enabled",
    "(GPIO) Enable ESP8285 user GPIO's (1)",
    "(Time) Append timezone to JSON time (1)",
    "(GUI) Show hostname and IP address in GUI main menu (1)",
    "(Tuya) Apply SetOption20 settings to Tuya device (1)",
    "(mDNS) Service on (1) or off (0)",
    "(Wifi) Scan network at restart for configured AP's (1) or used stored AP (0)",
    "(Wifi) Scan network every 44 minutes for configured AP's (1)",
    "(IR) Add IR Raw data to JSON message (1)",
    "(MQTT) Send tele/%topic%/STATE in addition to stat/%topic%/RESULT (1)",
    "(Sleep) Enable normal sleep (1) instead of dynamic sleep (0)",
    "(Button, Switch) Force local operation (1) when button/switch topic is set",
    "(MQTT) Don't use retain flag on HOLD messages (1)",
    "(Power) Don't scan relay power state at restart (1)",
    "(JSON) Enable '_' (1) instead of '-' (0) as sensor index separator",
    "(QPC) Disable (1) fast power cycle detection for device reset",
    "(Tuya) Enable (1) TuyaMcuReceived messages over Mqtt",
    "(Buzzer) Enable (1) buzzer when available",
    "(Light) Enable multi-channels PWM (1) instead of Color PWM (0)",
    "(not used) Limits Tuya dimmers to minimum of 10% (25) when enabled",
    "(Energy) Enable Weekend Energy Tariff",
    "(DDS2382) Select different Modbus registers (1) for Active Energy (#6531)",
    "(Energy) Enable (1) hardware energy total counter as reference (#6561)",
    "(Button) Detach buttons from relays (1) and enable MQTT action state for multipress",
    "(DS18x20) Enable internal pullup (1) for single DS18x20 sensor",
    "(MQTT) GroupTopic replaces %topic% (0) or fixed topic cmnd/grouptopic (1)",
    "(Deepsleep) Enable incrementing bootcount (1) when deepsleep is enabled",
    "(Light) Do not power off (1) if slider moved to far left",
    "(not used) Disable OTA compatibility check",
    "(Counter) Enable resetting of counters (1) after telemetry was sent",
    "(Shutter) Enable shutter support (1)",
    "(PCF8574) Invert all ports on PCF8574 devices (1)"
    ],[
    "(Alexa) Reduced CT range for Alexa (1)",
    "(Zigbee) Use FriendlyNames (1) instead of ShortAddresses (0) when possible",
    "(AWS IoT) publish MQTT state to a device shadow (1)",
    "(DevGroups) Enable Device Groups (1)",
    "(PWM Dimmer) Turn brightness LED's off (1) 5 seconds after last change",
    "(PWM Dimmer) Turn red LED on (1) when powered off",
    "(DevGroups) Enable relays in separate device groups/PWM Dimmer Buttons control remote devices (1)",
    "(MQTT, Zigbee) Distinct MQTT topics per device for Zigbee (1) (#7835)",
    "(MQTT) Disable non-json MQTT response (1)",
    "(Light) Enable light fading at start/power on (1)",
    "(Light) Set PWM Mode from regular PWM (0) to ColorTemp (1) control (Xiaomi Philips ...)",
    "(Compress) Keep uncompressed rules in memory to avoid CPU load of uncompressing at each tick (1)",
    "(MAXxxx) Implement simpler MAX6675 protocol (1) instead of MAX31855 (0)",
    "(Wifi) Enable Wifi",
    "(Eth) Enable Ethernet (ESP32)",
    "(Tuya) Set Baud rate for TuyaMCU serial communication to 9600 (0) or 115200 (1)",
    "(Rotary) Use rules (1) instead of light control (0)",
    "(PWM Dimmer) Enable zerocross dimmer (1)",
    "(Zigbee) Remove ZbReceived form JSON message (1)",
    "(Zigbee) Add the source endpoint as suffix to attributes, ex `Power3` (1) instead of `Power` (0) if sent from endpoint 3",
    "(Teleinfo) Set Baud rate for Teleinfo communication to 1200 (0) or 9600 (1)",
    "(MQTT TLS) Enable TLS mode (1) (requires TLS version)",
    "(MQTT) No Retain (1) - disable all MQTT retained messages, some brokers don't support it: AWS IoT, Losant",
    "(Light) White Blend Mode (1) - used to be `RGBWWTable` last value `0`, now deprecated in favor of this option",
    "(Light) Virtual CT (1) - Creates a virtual White ColorTemp for RGBW lights",
    "(Light) Virtual CT Channel (1) - signals whether the hardware white is cold CW (true) or warm WW (false)",
    "(Teleinfo) Enable Teleinfo + Tasmota Energy device (0) or Teleinfo raw data only (1)",
    "(Alexa) Gen1 mode (1) - if you only have Echo Dot 2nd gen devices",
    "(Zigbee) Disable auto-config (1) when pairing new devices",
    "(Buzzer) Use frequency output (1) for buzzer pin instead of on/off signal (0)",
    "(Zigbee) Use friendly name in zigbee topic (1) (use with SetOption89)",
    "(Rotary) Set dimmer low on rotary dial after power off (1)"
    ],[
    "(Switch) Detach Switches from relays and enable MQTT action state for all the SwitchModes (1)",
    "(ESP32 BLE) Enable ESP32 MI32 BLE (1)",
    "(Zigbee) Disable auto-query of zigbee lights and devices (1)",
    "(Light) run fading at fixed duration instead of fixed slew rate",
    "(Zigbee) Move ZbReceived from JSON message and into the subtopic replacing SENSOR default",
    "(Zigbee) Remove the device addr from json payload, can be used with zb_topic_fname where the addr is already known from the topic",
    "(Zigbee) Append endpoint number to topic if device dependent (use with SetOption89)",
    "(MQTT) Retain on State",
    "(MQTT) Retain on Info",
    "(Wiegand) switch tag number output to hex format (1)",
    "(Wiegand) send key pad stroke as single char (0) or one tag (ending char #) (1)",
    "(Zigbee) Hide bridge topic from zigbee topic (use with SetOption89) (1)",
    "(DS18x20) Enable arithmetic mean over teleperiod for JSON temperature (1)",
    "(Wifi) Keep wifi in no-sleep mode, prevents some occasional unresponsiveness",
    "(Web) Allow access without referer check",
    "(Energy) Show phase information",
    "(Debug) Show heap with logging timestamp",
    "(Tuya) Allow save dimmer = 0 receved by MCU",
    "(TLS) Use fingerprint validation instead of CA based",
    "(Shift595) Invert outputs of 74x595 shift registers",
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
    "USE_TASMOTA_SLAVE","USE_HIH6","USE_HPMA","USE_TSL2591",
    "USE_DHT12","USE_DS1624","USE_GPS","USE_HOTPLUG",
    "USE_NRF24","USE_MIBLE","USE_HM10","USE_LE01MR",
    "USE_AHT1x","USE_WEMOS_MOTOR_V1","USE_DEVICE_GROUPS","USE_PWM_DIMMER"
    ],[
    "USE_KEELOQ","USE_HRXL","USE_SONOFF_D1","USE_HDC1080",
    "USE_IAQ","USE_DISPLAY_SEVENSEG","USE_AS3935","USE_PING",
    "USE_WINDMETER","USE_OPENTHERM","USE_THERMOSTAT","USE_VEML6075",
    "USE_VEML7700","USE_MCP9808","USE_BL09XX","USE_TELEGRAM",
    "USE_HP303B","USE_TCP_BRIDGE","USE_TELEINFO","USE_LMT01",
    "USE_PROMETHEUS","USE_IEM3000","USE_DYP","USE_I2S_AUDIO",
    "USE_MLX90640","USE_VL53L1X","USE_MIEL_HVAC","USE_WE517",
    "USE_EZOPH","USE_TTGO_WATCH","USE_ETHERNET","USE_WEBCAM"
    ],[
    "USE_EZOORP","USE_EZORTD","USE_EZOHUM","USE_EZOEC",
    "USE_EZOCO2","USE_EZOO2","USE_EZOPRS","USE_EZOFLO",
    "USE_EZODO","USE_EZORGB","USE_EZOPMP","USE_AS608",
    "USE_SHELLY_DIMMER","USE_RC522","USE_FTC532","USE_DISPLAY_EPAPER_42",
    "USE_DISPLAY_ILI9488","USE_DISPLAY_SSD1351","USE_DISPLAY_RA8876","USE_DISPLAY_ST7789",
    "USE_DISPLAY_SSD1331","USE_UFILESYS","USE_TIMEPROP","USE_PID",
    "USE_BS814A2","USE_SEESAW_SOIL","USE_WIEGAND","USE_NEOPOOL",
    "USE_TOF10120","USE_SDM72","USE_DISPLAY_TM1637","USE_PROJECTOR_CTRL"
    ],[
    "USE_MPU_ACCEL","USE_TFMINIPLUS","USE_CSE7761","USE_BERRY",
    "USE_BM8563","USE_ENERGY_DUMMY","USE_AM2320","USE_T67XX",
    "USE_MCP2515","USE_TASMESH","USE_WIFI_RANGE_EXTENDER","USE_INFLUXDB",
    "USE_HRG15","USE_VINDRIKTNING","USE_SCD40","USE_HM330X",
    "USE_HDC2010","USE_LSC_MCSL","USE_SONOFF_SPM","USE_SHIFT595",
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
    loginstr = ""
    if options.password is not None:
        loginstr = "user={}&password={}&".format(urllib.parse.quote(options.username), urllib.parse.quote(options.password))
    url = str("http://{}/cm?{}cmnd=status%200".format(options.device, loginstr))
    res = requests.get(url)
    obj = json.loads(res.content)
else:
    jsonfile = options.jsonfile
    with open(jsonfile, "r") as fp:
        obj = json.load(fp)

def StartDecode():
    print ("\n*** decode-status.py v20211205 by Theo Arends and Jacek Ziolkowski ***")

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
                                                      # 8.4.0.2: adds another SetOptions 114..145
                        split_register = [int(register[opt*2:opt*2+2],16) for opt in range(18)] # split register into 18 values

                        for opt_idx, option in enumerate(opt_group):
                            options.append(str("{0:3d} ({1:3d}) {2}".format(i, split_register[opt_idx], option)))
                            i += 1

                if r in (0, 2, 3, 4):                 #registers 1 and 4 hold binary values
                    for opt_idx, option in enumerate(opt_group):
                        i_register = int(register,16)
                        state = (i_register >> opt_idx) & 1
                        options.append(str("{0:3d} ({1}) {2}".format(i, a_on_off[state], option)))
                        i += 1

            print("\nOptions")
            for o in options:
                print("  {}".format(o))

    if "StatusMEM" in obj:
        if "Features" in obj["StatusMEM"]:
            features = []
            maxfeatures = len(obj["StatusMEM"]["Features"]) - 1
            if maxfeatures > len(a_features):
                print("decode-status.py too old, does not support all feature bits")
            maxfeatures = min(maxfeatures, len(a_features))
            for f in range(maxfeatures + 1):
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
