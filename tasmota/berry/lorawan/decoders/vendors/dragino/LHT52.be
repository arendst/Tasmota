# LoRaWAN Decoder file for Dragino LHT52
#
# References
#  User Manual:     https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/LHT52%20-%20LoRaWAN%20Temperature%20%26%20Humidity%20Sensor%20User%20Manual/
#  TTN Device Repository: https://github.com/TheThingsNetwork/lorawan-devices/blob/master/vendor/dragino/lht52.js

import string

if !global.lht52Nodes      # data survive to decoder reload
  global.lht52Nodes = {}
end

class LwDecoLHT52
  static def decodeUplink(Name, Node, RSSI, FPort, Bytes)
    var data = {"Device":"Dragino LHT52"}
    
    var valid_values = false
    var last_seen = 1451602800
    var battery_last_seen = 1451602800
    var battery = 1000
    var rssi = RSSI
    var temp_int = 1000
    var humidity
    var temp_ext = 1000
    if global.lht52Nodes.find(Node)
      last_seen         = global.lht52Nodes.item(Node)[2]
      battery_last_seen = global.lht52Nodes.item(Node)[3]
      battery           = global.lht52Nodes.item(Node)[4]
      rssi              = global.lht52Nodes.item(Node)[5]
      temp_int          = global.lht52Nodes.item(Node)[6]
      humidity          = global.lht52Nodes.item(Node)[7]
      temp_ext          = global.lht52Nodes.item(Node)[8]
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
      data.insert("BattV",((Bytes[5] << 8) | Bytes[6]) / 1000.0)
      battery_last_seen = tasmota.rtc('local')
      battery = ((Bytes[5] << 8) | Bytes[6]) / 1000.0
      valid_values = true
    else
    # Ignore other Fports
    end #Fport

    if valid_values
      if global.lht52Nodes.find(Node)
        global.lht52Nodes.remove(Node)
      end
      #                         sensor[0]   [1]   [2]        [3]                [4]      [5]   [6]       [7]       [8]
      global.lht52Nodes.insert(Node, [Name, Node, last_seen, battery_last_seen, battery, RSSI, temp_int, humidity, temp_ext])
    end

    return data
  end #decodeUplink()

  static def add_web_sensor()
    var msg = ""
    for sensor: global.lht52Nodes
      var name = sensor[0]
      if string.find(name, "LHT52") > -1                                 # If LoRaWanName contains LHT52 use LHT52-<node>
        name = string.format("LHT52-%i", sensor[1])
      end
      var name_tooltip = "Dragino LHT52"
      var last_seen = sensor[2]
      var battery_last_seen = sensor[3]
      var battery = sensor[4]
      var rssi = sensor[5]
      msg += lwdecode.header(name, name_tooltip, battery, battery_last_seen, rssi, last_seen)

      # Sensors
      var temp_int = sensor[6]
      var humidity = sensor[7]
      var temp_ext = sensor[8]
      msg += "<tr class='htr'><td colspan='4'>&#9478;"                   # |
      if temp_int < 1000
        msg += string.format(" &#x2600;&#xFE0F; %.1f°C", temp_int)       # Sunshine - Temperature internal
        msg += string.format(" &#x1F4A7; %.1f%%", humidity)              # Raindrop - Humidity
      end
      if temp_ext < 1000
        msg += string.format(" &#x2600;&#xFE0F; ext %.1f°C", temp_ext)   # Sunshine - Temperature external
      end
      msg += "{e}"                                                       # = </td></tr>
    end
    return msg
  end #add_web_sensor()
end #class

LwDeco = LwDecoLHT52
