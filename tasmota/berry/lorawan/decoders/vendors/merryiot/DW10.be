# LoRaWAN Decoder file for MerryIoT DW10 Open/Close
#
# References
#  DW10 Product information: https://www.browan.com/products-detail/OpenClose-Sensor-EBL-LoRaWAN/
#  Browan JS Decoder (TTN):  https://www.browan.com/member/login/?refererUrl=https%3A%2F%2Fwww.browan.com%2Fproducts-detail%2FOpenClose-Sensor-EBL-LoRaWAN%2F

import string

global.dw10Nodes = {}

class LwDecoDW10
  static def decodeUplink(Node, RSSI, FPort, Bytes)
    var data = {"Device":"MerryIoT DW10"}
    data.insert("Node", Node)

    var valid_values = false
    var last_seen = 1451602800
    var battery_last_seen = 1451602800
    var battery
    var rssi = RSSI
    var door_open
    var door_open_last_seen = 1451602800
    var button_pressed
    var temperature
    var humidity
    if global.dw10Nodes.find(Node)
      door_open = global.dw10Nodes.item(Node)[5]
      door_open_last_seen = global.dw10Nodes.item(Node)[6]
    end
    ## SENSOR DATA ##
    if 120 == FPort && Bytes.size() == 9
      last_seen = tasmota.rtc('local')

      var last_door_open = door_open
      door_open = ( Bytes[0] & 0x01 ) ? 1 : 0
      data.insert("DoorOpen", ( door_open ) ? true : false )
      if last_door_open != door_open
        door_open_last_seen = tasmota.rtc('local')
      end

      button_pressed = ( Bytes[0] & 0x02 ) ? 1 : 0
      data.insert("ButtonPress", ( button_pressed ) ? true : false )
      data.insert("TamperDetect", ( Bytes[0] & 0x04 ) ? true : false )
      data.insert("TiltDetect", ( Bytes[0] & 0x08 ) ? true : false )
      data.insert("BattV", (( 21 + Bytes[1] ) * 100) / 1000.0 )
      battery_last_seen = tasmota.rtc('local')
      battery = (( 21 + Bytes[1] ) * 100) / 1000.0
      data.insert("TemperatureC", Bytes[2])
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
      #                        sensor[0]   [1]        [2]                [3]      [4]   [5]        [6]                  [7]             [8]          [9]
      global.dw10Nodes.insert(Node, [Node, last_seen, battery_last_seen, battery, RSSI, door_open, door_open_last_seen, button_pressed, temperature, humidity])
    end

    return data
  end #decodeUplink()

  static def add_web_sensor()
    var msg = ""
    for sensor: global.dw10Nodes
      var name = string.format("DW10-%i", sensor[0])
      var name_tooltip = "MerryIoT DW10"
      var battery = sensor[3]
      var battery_last_seen = sensor[2]
      var rssi = sensor[4]
      var last_seen = sensor[1]
      msg += lwdecode.header(name, name_tooltip, battery, battery_last_seen, rssi, last_seen)

      # Sensors
      var door_open = sensor[5]
      var door_open_last_seen = sensor[6]
      var button_pressed = sensor[7]
      var temperature = sensor[8]
      var humidity = sensor[9]
      msg += "<tr class='htr'><td colspan='4'>&#9478;"                      # |
      msg += string.format(" &#x2600;&#xFE0F; %.1f°C", temperature)         # Sunshine - Temperature
      msg += string.format(" &#x1F4A7; %.1f%%", humidity)                   # Raindrop - Humidity
      msg += string.format(" %s %s", (door_open) ? "&#x1F513" : "&#x1F512", # Open or Closed lock - Door
                                     lwdecode.dhm(door_open_last_seen))
      msg += "{e}"                                                          # = </td></tr>
    end
    return msg
  end #add_web_sensor()
end #class

LwDeco = LwDecoDW10
