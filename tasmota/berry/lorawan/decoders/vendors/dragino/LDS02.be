# LoRaWAN Decoder file for Dragino LDS02
#
# References
#  LHT52 User Manual:     https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/LDS02%20-%20LoRaWAN%20Door%20Sensor%20User%20Manual/
#  TTN Device Repository: https://github.com/TheThingsNetwork/lorawan-devices/blob/master/vendor/dragino/lds02.js

import string

global.lds02Nodes = {}

class LwDecoLDS02
  static def decodeUplink(Node, FPort, Bytes)
    var data = {"Device":"Dragino LDS02"}
    data.insert("Node", Node)

    var valid_values = false
    var door_open
    ## SENSOR DATA ##
    if 10 == FPort && Bytes.size() == 10
      door_open = ( Bytes[0] & 0x80 ) ? 1 : 0
      data.insert("DoorOpen", ( door_open ) ? true : false)
      data.insert("Battery_mV", ( Bytes[1] | (Bytes[0] << 8) & 0x3FFF ))
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
      global.lds02Nodes.insert(Node, [Node, door_open])
    end

    return data
  end #decodeUplink()

  static def add_web_sensor()
    var msg = ""
    for sensor: global.lds02Nodes
      msg += string.format("{s}LDS02_%i Door{m}%s{e}",
                           sensor[0], (sensor[1]) ? "Open" : "Closed")
    end

    return msg
  end
end #class

LwDeco = LwDecoLDS02
