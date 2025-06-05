# LoRaWAN Decoder file for Dragino LDS02
#
# References
#  LHT52 User Manual:     https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/LDS02%20-%20LoRaWAN%20Door%20Sensor%20User%20Manual/
#  TTN Device Repository: https://github.com/TheThingsNetwork/lorawan-devices/blob/master/vendor/dragino/lds02.js

import string

global.lds02Nodes = {}

class LwDecoLDS02
  static def decodeUplink(Node, RSSI, FPort, Bytes)
    var data = {"Device":"Dragino LDS02"}
    data.insert("Node", Node)

    var valid_values = false
    var last_seen = 0x7FFFFFFF
    var battery_last_seen = 0x7FFFFFFF
    var battery
    var rssi = RSSI
    var door_open
    ## SENSOR DATA ##
    if 10 == FPort && Bytes.size() == 10
      last_seen = tasmota.rtc('local')
      door_open = ( Bytes[0] & 0x80 ) ? 1 : 0
      data.insert("DoorOpen", ( door_open ) ? true : false)
      data.insert("BattV", ( Bytes[1] | (Bytes[0] << 8) & 0x3FFF ) / 1000.0)
      battery_last_seen = tasmota.rtc('local')
      battery = ( Bytes[1] | (Bytes[0] << 8) & 0x3FFF ) / 1000.0
      data.insert("DoorOpenEvents", Bytes[5] | (Bytes[4] << 8) | (Bytes[3] << 16 ))
      data.insert("DoorOpenLastDuration_mins", Bytes[8] | (Bytes[7] << 8) | (Bytes[6] << 16))
      data.insert("Alarm", (Bytes[9] & 0x01 ) ? true : false)
      valid_values = true
    else
      # Ignore other Fports
    end #Fport

    if valid_values
      if global.lds02Nodes.find(Node)
        global.lds02Nodes.remove(Node)
      end
      global.lds02Nodes.insert(Node, [Node, last_seen, battery_last_seen, battery, RSSI, door_open])
    end

    return data
  end #decodeUplink()

  static def add_web_sensor()
    var msg = ""
    for sensor: global.lds02Nodes
      # Sensor[0]    [1]        [2]                [3]      [4]   [5]
      #       [Node, last_seen, battery_last_seen, battery, RSSI, door_open]

      var name = string.format("LDS02-%i", sensor[0])
      var name_tooltip = "Dragino LDS02"
      var battery = sensor[3]
      var battery_last_seen = sensor[2]
      var rssi = sensor[4]
      var last_seen = sensor[1]
      msg += lwdecode.header(name, name_tooltip, battery, battery_last_seen, rssi, last_seen)

      # Sensors
      msg += "<tr class='htr'><td colspan='4'>&#9478;"                    # |
      msg += string.format(" %s", (sensor[5]) ? "&#x1F513" : "&#x1F512")  # Open or Closed lock - Door
      msg += "{e}"                                                        # = </td></tr>
    end
    return msg
  end #add_web_sensor()
end #class

LwDeco = LwDecoLDS02
