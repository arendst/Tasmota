# LoRaWAN Decoder file for Dragino LHT52
#
# References
#  User Manual:     https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/LHT52%20-%20LoRaWAN%20Temperature%20%26%20Humidity%20Sensor%20User%20Manual/
#  TTN Device Repository: https://github.com/TheThingsNetwork/lorawan-devices/blob/master/vendor/dragino/lht52.js

import string

global.lht52Nodes = {}

class LwDecoLHT52
  static def decodeUplink(Node, FPort, Bytes)
    var data = {"Device":"Dragino LHT52"}
    data.insert("Node", Node)

    var valid_values = false
    var temp_int
    var humidity
    var temp_ext = 1000
    ## SENSOR DATA ##
    if 2 == FPort && Bytes.size() == 11
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
      data.insert("Bat_mV",(Bytes[5] << 8) | Bytes[6])

    else
    # Ignore other Fports
    end #Fport

    if valid_values
      if global.lht52Nodes.find(Node)
        global.lht52Nodes.remove(Node)
      end
      global.lht52Nodes.insert(Node, [Node, temp_int, humidity, temp_ext])
    end

    return data
  end #decodeUplink()

  static def add_web_sensor()
    var msg = ""
    for sensor: global.lht52Nodes
      msg += string.format("{s}LHT52_%i Temperature{m}%.1f °C{e}"..
                           "{s}LHT52_%i Humidity{m}%.1f %%{e}",
                           sensor[0], sensor[1],
                           sensor[0], sensor[2])
      if sensor[3] < 1000
        msg += string.format("{s}LHT52_%i Temperature ext.{m}%.1f °C{e}",
                             sensor[0], sensor[3])
      end
    end

    return msg
  end
end #class

LwDeco = LwDecoLHT52
