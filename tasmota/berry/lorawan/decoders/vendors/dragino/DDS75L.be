# LoRaWAN Decoder file for Dragino DDS75-LB/LS
#
# References
#  User Manual:     https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/DDS75-LB_LoRaWAN_Distance_Detection_Sensor_User_Manual/
#  TTN Device Repository: https://github.com/TheThingsNetwork/lorawan-devices/blob/master/vendor/dragino/dds75-lb.js

import string

global.dds75lbNodes = {}

class LwDecoDDS75LB
  static def decodeUplink(Node, RSSI, FPort, Bytes)
    var data = {"Device":"Dragino DDS75-LB/LS"}
    data.insert("Node", Node)

    var valid_values = false
    var last_seen = 1451602800
    var battery_last_seen = 1451602800
    var battery = 1000
    var rssi = RSSI
    var distance = 0

    if global.dds75lbNodes.find(Node)
      last_seen = global.dds75lbNodes.item(Node)[1]
      battery_last_seen = global.dds75lbNodes.item(Node)[2]
      battery = global.dds75lbNodes.item(Node)[3]
      rssi = global.dds75lbNodes.item(Node)[4]
      distance = global.dds75lbNodes.item(Node)[5]
    end

    ## SENSOR DATA ##
    if 2 == FPort && 8 == Bytes.size() &&  0 == ( Bytes[0] & 0x10 )
      last_seen = tasmota.rtc('local')
	  	  
	  battery_last_seen = tasmota.rtc('local')
      battery = ((Bytes[0] << 8) | Bytes[1]) / 1000.0
      data.insert("BattV",battery)
      
	  distance=Bytes[2]<<8 | Bytes[3] 
      data.insert("Distance",distance)
  
      valid_values = true

    ## STATUS DATA ##
    elif 5 == FPort && 7 == Bytes.size()
      data.insert("Sensor_Model",Bytes[0])
      data.insert("Firmware_Version", f'v{Bytes[1]:%u}.{Bytes[2]>>4:%u}.{Bytes[2]&0xF:%u}')
      data.insert("Freq_Band",LwRegions[Bytes[3]-1])
      data.insert("Sub_Band",Bytes[4])
      battery_last_seen = tasmota.rtc('local')
      battery = ((Bytes[5] << 8) | Bytes[6]) / 1000.0
      valid_values = true
    else
    # Ignore other Fports
    end #Fport

    if valid_values
      if global.dds75lbNodes.find(Node)
        global.dds75lbNodes.remove(Node)
      end
      #                                 sensor[0]   [1]        [2]                [3]      [4]   [5]           
      global.dds75lbNodes.insert(Node, [Node,       last_seen, battery_last_seen, battery, RSSI, distance])
    end

    return data
  end #decodeUplink()

  static def add_web_sensor()
    var msg = ""
    for sensor: global.dds75lbNodes
      var name = string.format("DDS75-L-%i", sensor[0])
      var name_tooltip = "Dragino DDS75-L"
      var last_seen = sensor[1]
      var battery_last_seen = sensor[2]
      var battery = sensor[3]
      var rssi = sensor[4]
      msg += lwdecode.header(name, name_tooltip, battery, battery_last_seen, rssi, last_seen)

      # Sensors
      var distance = sensor[5]
      msg += "<tr class='htr'><td colspan='4'>&#9478;"                   # |
      msg += string.format(" &#11123;&#xFE0F; %.0fmm", distance)         # ⭳          
      msg += "{e}"                                                       # = </td></tr>
    end
    return msg
  end #add_web_sensor()
end #class

LwDeco = LwDecoDDS75LB
