# LoRaWAN Decoder file for MerryIoT DW10 Open/Close
#
# References
#  DW10 Product information: https://www.browan.com/products-detail/OpenClose-Sensor-EBL-LoRaWAN/
#  Browan JS Decoder (TTN):  https://www.browan.com/member/login/?refererUrl=https%3A%2F%2Fwww.browan.com%2Fproducts-detail%2FOpenClose-Sensor-EBL-LoRaWAN%2F

import string

global.dw10Nodes = {}

class LwDecoDW10
  static def decodeUplink(Node, FPort, Bytes)
    var data = {"Device":"MerryIoT DW10"}
    data.insert("Node", Node)

    var valid_values = false
    var door_open
    var button_pressed
    var temperature
    var humidity
    ## SENSOR DATA ##
    if 120 == FPort && Bytes.size() == 9
      door_open = ( Bytes[0] & 0x01 ) ? 1 : 0
      data.insert("DoorOpen", ( door_open ) ? true : false )
      button_pressed = ( Bytes[0] & 0x02 ) ? 1 : 0
      data.insert("ButtonPress", ( button_pressed ) ? true : false )
      data.insert("TamperDetect", ( Bytes[0] & 0x04 ) ? true : false )
      data.insert("TiltDetect", ( Bytes[0] & 0x08 ) ? true : false )
      data.insert("Battery_mV", ( 21 + Bytes[1] ) * 100 )
      data.insert("Temperature_C", Bytes[2])
      temperature = Bytes[2]
      data.insert("Humidity", Bytes[3])
      humidity = Bytes[3]
      data.insert("DoorOpenLastDuration_mins", Bytes[4] | (Bytes[5] << 8))
      data.insert("DoorOpenEvents", Bytes[6] | (Bytes[7] << 8) | (Bytes[8] << 16 ))
      valid_values = true
   
    else
      # Ignore other Fports
    end #Fport

    if valid_values
      if global.dw10Nodes.find(Node)
        global.dw10Nodes.remove(Node)
      end
      global.dw10Nodes.insert(Node, [Node, door_open, button_pressed, temperature, humidity])
    end

    return data
  end #decodeUplink()

  static def add_web_sensor()
    var msg = ""
    for sensor: global.dw10Nodes
      msg += string.format("{s}DW10_%i Door{m}%s{e}"..
                           "{s}DW10_%i Button{m}%s{e}"..
                           "{s}DW10_%i Temperature{m}%.1f °C{e}"..
                           "{s}DW10_%i Humidity{m}%.1f %%{e}",
                           sensor[0], (sensor[1]) ? "Open" : "Closed",
                           sensor[0], (sensor[2]) ? "Pressed" : "Released",
                           sensor[0], sensor[3],
                           sensor[0], sensor[4])
    end
    return msg
  end
end #class

LwDeco = LwDecoDW10
