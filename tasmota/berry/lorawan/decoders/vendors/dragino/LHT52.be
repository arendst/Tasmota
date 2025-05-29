# LoRaWAN Decoder file for Dragino LHT52
#
# References
#  User Manual:     https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/LHT52%20-%20LoRaWAN%20Temperature%20%26%20Humidity%20Sensor%20User%20Manual/
#  TTN Device Repository: https://github.com/TheThingsNetwork/lorawan-devices/blob/master/vendor/dragino/lht52.js

import string

global.lht52Nodes = {}

class LwDecoLHT52
  static def decodeUplink(Node, RSSI, FPort, Bytes)
    var data = {"Device":"Dragino LHT52"}
    data.insert("Node", Node)

    var valid_values = false
    var last_seen
    var battery_last_seen
    var battery = 1000
    var rssi = RSSI
    var temp_int
    var humidity
    var temp_ext = 1000
    if global.lht52Nodes.find(Node)
      last_seen = global.lht52Nodes.item(Node)[1]
      battery_last_seen = global.lht52Nodes.item(Node)[2]
      battery = global.lht52Nodes.item(Node)[3]
      rssi = global.lht52Nodes.item(Node)[4]
      temp_int = global.lht52Nodes.item(Node)[5]
      humidity = global.lht52Nodes.item(Node)[6]
      temp_ext = global.lht52Nodes.item(Node)[7]
    end
    ## SENSOR DATA ##
    if 2 == FPort && Bytes.size() == 11
      last_seen = tasmota.rtc('local')

      var TempC
         TempC = Bytes[0] << 8 | Bytes[1]
      if Bytes[0] > 0x7F
        TempC -= 0x10000
      end
      TempC /= 100.0
      data.insert("TempC_Internal", TempC)
      temp_int = TempC

      TempC = Bytes[4] << 8 | Bytes[5]
      if 0x7FFF == TempC
        data.insert("Ext_SensorConnected", false)
      else 
        data.insert("Ext_SensorConnected", true)
        if Bytes[4] > 0x7F
          TempC -= 0x10000
        end
        TempC /= 100.0
        data.insert("TempC_External", TempC)
        temp_ext = TempC
      end
   
      data.insert("Hum_Internal", ((Bytes[2] << 8) | Bytes[3]) / 10.0)   
      humidity = ((Bytes[2] << 8) | Bytes[3]) / 10.0
      data.insert("Ext_SensorType", Bytes[6])
      var epoch = (Bytes[7] << 24) | (Bytes[8] << 16) | (Bytes[9] << 8) | Bytes[10]
      data.insert("Systimestamp",tasmota.time_str(epoch))
      valid_values = true

    ## STATUS DATA ##
    elif 5 == FPort && Bytes.size() == 7
      data.insert("Sensor_Model",Bytes[0])
      data.insert("Firmware_Version", f'v{Bytes[1]:%u}.{Bytes[2]>>4:%u}.{Bytes[2]&0xF:%u}')
      data.insert("Freq_Band",LwRegions[Bytes[3]-1])
      data.insert("Sub_Band",Bytes[4])
      data.insert("BattV",((Bytes[5] << 8) | Bytes[6]) / 1000)
      battery_last_seen = tasmota.rtc('local')
      battery = ((Bytes[5] << 8) | Bytes[6]) / 1000
      valid_values = true
    else
    # Ignore other Fports
    end #Fport

    if valid_values
      if global.lht52Nodes.find(Node)
        global.lht52Nodes.remove(Node)
      end
      global.lht52Nodes.insert(Node, [Node, last_seen, battery_last_seen, battery, RSSI, temp_int, humidity, temp_ext])
    end

    return data
  end #decodeUplink()

  static def add_web_sensor()
    var msg = ""
    for sensor: global.lht52Nodes
      # Sensor[0]    [1]        [2]                [3]      [4]   [5]       [6]       [7]
      #       [Node, last_seen, battery_last_seen, battery, RSSI, temp_int, humidity, temp_ext]

      var name = string.format("LHT52-%i", sensor[0])
      var name_tooltip = "Dragino LHT52"
      var battery = sensor[3]
      var battery_last_seen = sensor[2]
      var rssi = sensor[4]
      var last_seen = sensor[1]
      msg += lwdecode.header(name, name_tooltip, battery, battery_last_seen, rssi, last_seen)

      # Sensors
      msg += "<tr class='htr'><td colspan='4'>&#9478;"                    # |
      msg += string.format(" &#x2600;&#xFE0F; %.1f°C", sensor[5])         # Sunshine - Temperature internal
      msg += string.format(" &#x1F4A7; %.1f%%", sensor[6])                # Raindrop - Humidity
      if sensor[7] < 1000
        msg += string.format(" &#x2600;&#xFE0F; ext %.1f°C", sensor[7])   # Sunshine - Temperature external
      end
      msg += "{e}"                                                        # = </td></tr>
    end
    return msg
  end #add_web_sensor()
end #class

LwDeco = LwDecoLHT52
