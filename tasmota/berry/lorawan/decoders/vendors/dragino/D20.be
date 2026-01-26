# LoRaWAN Decoder file for Dragino D20/D22/D23 (1,2,3 temp sensor models)
#
# References
#  User Manual:     https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/D20-LBD22-LBD23-LB_LoRaWAN_Temperature_Sensor_User_Manual/
#  TTN Device Repository: https://github.com/TheThingsNetwork/lorawan-devices/blob/master/vendor/dragino/d2x-lb.js

import string

if !global.DrgD20Nodes      # data survive to decoder reload
  global.DrgD20Nodes = {}
end

class LwDecoDrgD20
  static def decodeUplink(Name, Node, RSSI, FPort, Bytes)
    var data = {"Device":"Dragino D20"}
    
    var valid_values = false
    var last_seen = 1451602800
    var battery_last_seen = 1451602800
    var battery = 1000
    var rssi = RSSI
    var tempC1 = 1000
    var tempC2 = 1000
    var tempC3 = 1000
	
    if global.DrgD20Nodes.find(Node)
      last_seen         = global.DrgD20Nodes.item(Node)[2]
      battery_last_seen = global.DrgD20Nodes.item(Node)[3]
      battery           = global.DrgD20Nodes.item(Node)[4]
      rssi              = global.DrgD20Nodes.item(Node)[5]
      tempC1            = global.DrgD20Nodes.item(Node)[6]
      tempC2            = global.DrgD20Nodes.item(Node)[7]										
      tempC3            = global.DrgD20Nodes.item(Node)[8]
    end

    ## SENSOR DATA ##
    if 2 == FPort && Bytes.size() == 11
      last_seen = tasmota.rtc('local')  
      var mode=(Bytes[6] & 0x7C)>>2

      if 3==mode
	battery = (Bytes[0]<<8 | Bytes[1])/1000
	data.insert("BattV", battery)
        battery_last_seen = tasmota.rtc('local')
		
        # 0x07FF = 2047 =  no temp sensor 
		
        tempC1 = Bytes[2] << 8 | Bytes[3]
        if Bytes[2]>0x7F tempC1-=0x10000 end
	  tempC1 /= 10.0
        data.insert("TempC1", tempC1)

        tempC2 = Bytes[7] << 8 | Bytes[8]
        if Bytes[7]>0x7F tempC2-=0x10000 end
	  tempC2 /= 10.0
        data.insert("TempC2", tempC2)

        tempC3 = Bytes[9] << 8 | Bytes[10]
        if Bytes[9]>0x7F tempC3-=0x10000 end
	  tempC3 /= 10.0
        data.insert("TempC3", tempC3)
		
      end
	  
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
      if global.DrgD20Nodes.find(Node)
        global.DrgD20Nodes.remove(Node)
      end
      #                         sensor[0]    [1]   [2]        [3]                [4]      [5]   [6]     [7]     [8]
      global.DrgD20Nodes.insert(Node, [Name, Node, last_seen, battery_last_seen, battery, RSSI, tempC1, tempC2, tempC3])   
    end

    return data
  end #decodeUplink()

  static def add_web_sensor()
    var msg = ""
    for sensor: global.DrgD20Nodes
      var name = sensor[0]
      if string.find(name, "D20") > -1                                   # If LoRaWanName contains D20 use D20-<node>
        name = string.format("D20-%i", sensor[1])
      end
      var name_tooltip = "Dragino D20"
      var last_seen = sensor[2]
      var battery_last_seen = sensor[3]
      var battery = sensor[4]
      var rssi = sensor[5]
      msg += lwdecode.header(name, name_tooltip, battery, battery_last_seen, rssi, last_seen)

      # Sensors
      var tempC1 = sensor[6]
      msg += "<tr class='htr'><td colspan='4'>&#9478;"                   # |
      if tempC1 < 1000
        msg += string.format(" &#x2600;&#xFE0F; %.1f°C", tempC1)         # Sunshine - Temperature 
      end

      var tempC2 = sensor[7]
      if tempC2 < 1000
        msg += string.format(" &#x2600;&#xFE0F; %.1f°C", tempC2)         
      end

      var tempC3 = sensor[8]
      if tempC3 < 1000
        msg += string.format(" &#x2600;&#xFE0F; %.1f°C", tempC3)         
      end
      msg += "{e}"                                                       # = </td></tr>
    end
    return msg
  end #add_web_sensor()
end #class

LwDeco = LwDecoDrgD20
