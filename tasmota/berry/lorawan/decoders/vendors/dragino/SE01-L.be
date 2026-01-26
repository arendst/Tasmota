# LoRaWAN Decoder file for Dragino SE01-LB/LS Soil Sensor
# URL: https://www.dragino.com/products/agriculture-weather-station/item/277-se01-lb.html
# File Name: SE01-L.be
#
# References
#  User Manual:    https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/SE01-LB_LoRaWAN_Soil%20Moisture%26EC_Sensor_User_Manual/
#  TTN Device Repository:https://github.com/TheThingsNetwork/lorawan-devices/blob/master/vendor/dragino/lse01-121.js

import string

if !global.se01LNodes      # data survive to decoder reload
  global.se01LNodes = {}
end

class LwDecoSE01L
  static def decodeUplink(Name, Node, RSSI, FPort, Bytes)
    var data = {"Device":"Dragino SE01-LB/LS"}
    
    var valid_values = false
    var last_seen = 1451602800
    var battery_last_seen = 1451602800
    var battery = 1000
    var rssi = RSSI

    var temp = 1000
    var conductivity=0
    var moisture=0
    var dielectric=0
    var mod 	
    var i_flag # 0: Normal uplink packet, 1: Interrupt Uplink Packet.
    var s_flag # 0: No sensor was identified, 1: The sensor has been identified	
	
    if global.se01LNodes.find(Node)
      last_seen         = global.se01LNodes.item(Node)[2]
      battery_last_seen = global.se01LNodes.item(Node)[3]
      battery           = global.se01LNodes.item(Node)[4]
      rssi              = global.se01LNodes.item(Node)[5]
	    temp              = global.se01LNodes.item(Node)[6]
      conductivity      = global.se01LNodes.item(Node)[7]
      moisture          = global.se01LNodes.item(Node)[8]
      dielectric        = global.se01LNodes.item(Node)[9]	  
    end
	
    ## SENSOR DATA ##
    #e.g. 0f5a    0ccc    0000     079d 0000 10
    #     Battery ExtTemp Moisture Temp EC   Mode,Flags
    if 2 == FPort && ( Bytes.size() == 11 || Bytes.size() == 15) 
      last_seen = tasmota.rtc('local')

      battery_last_seen = tasmota.rtc('local')	  
      battery=((Bytes[0]<<8 | Bytes[1]) & 0x3FFF)/1000.0   ##Battery,units:V
      s_flag = (Bytes[10] >> 4) & 0x01
      i_flag = Bytes[10] & 0x0f
      mod=(Bytes[10]>>7)&0x01

      if 0==mod    #Default mode
        moisture=((Bytes[4]<<8 | Bytes[5])/100.0) ##moisture,units:%
        conductivity=Bytes[8]<<8 | Bytes[9]
        var value=Bytes[6]<<8 | Bytes[7]
	    if((value & 0x8000)>>15 == 0)
         temp=(value/100.0)
        else 
         temp=((value-0xFFFF)/100.0)
        end
        data.insert("Mode", "Default") 
        data.insert("Temp", temp) 
		
      else         #Raw Data mode
        conductivity=Bytes[4]<<8 | Bytes[5]
        moisture = Bytes[6]<<8 | Bytes[7]
	    dielectric = ((Bytes[8]<<8 | Bytes[9])/10.0)
        data.insert("Mode", "Raw") 	
        data.insert("DielectricConstant", dielectric) 
      end	  
	  
      data.insert("BattV",battery)
      data.insert("Moisture", moisture) 
      data.insert("Conductivity", conductivity) 
      data.insert("i_flag", i_flag) 
      data.insert("s_flag", s_flag) 
	  
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
      if global.se01LNodes.find(Node)
        global.se01LNodes.remove(Node)
      end
      #                         sensor[0]   [1]   [2]        [3]                [4]      [5]   [6]   [7]           [8]       [9]         [10]
      global.se01LNodes.insert(Node, [Name, Node, last_seen, battery_last_seen, battery, RSSI, temp, conductivity, moisture, dielectric, mod])
    end

    return data
  end #decodeUplink()

  static def add_web_sensor()
    var msg = ""
    for sensor: global.se01LNodes
      var name = sensor[0]
      if string.find(name, "SE01-L") > -1                           # If LoRaWanName contains SE01-L use SE01-L-<node>
        name = string.format("SE01-L-%i", sensor[1])
      end
      var name_tooltip = "Dragino SE01-L"
      var last_seen = sensor[2]
      var battery_last_seen = sensor[3]
      var battery = sensor[4]
      var rssi = sensor[5]
      msg += lwdecode.header(name, name_tooltip, battery, battery_last_seen, rssi, last_seen)

      # Sensors
      var temp = sensor[6]
      var conductivity = sensor[7]
      var moisture = sensor[8]
      var dielectric = sensor[9]
      var mod = sensor[10]
	  
      msg += "<tr class='htr'><td colspan='4'>&#9478;"              # |
      if mod
 	    msg += string.format(" &kappa; %.1f", dielectric )            # Kappa - dielectric
        msg += string.format(" &#x1F4A7;&#xFE0F; %u", moisture)     # Raindrop - moisture
        msg += string.format(" &sigma; %u", conductivity)           # Sigma - conductivity 
        msg += " (raw)"                                  
      else
        msg += string.format(" &#x2600;&#xFE0F; %.1f°C", temp)      # Sunshine/Color - Temperature
        msg += string.format(" &#x1F4A7;&#xFE0F; %.1f%%", moisture) # Raindrop/Color - moisture
        msg += string.format(" &sigma; %uuS/cm", conductivity)      # Sigma - conductivity 
      end    
  
      msg += "{e}"                                                  # = </td></tr>
    end
    return msg
  end #add_web_sensor()
end #class

LwDeco = LwDecoSE01L
