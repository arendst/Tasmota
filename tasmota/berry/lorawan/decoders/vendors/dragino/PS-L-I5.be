# LoRaWAN Decoder file for Dragino PS-LB/LS - LoRaWAN Air Water Pressure Sensor 
# Model: Immersion type, 0-5m Range (PS-Lx-I5)
#
# References
#  User Manual:     https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/PS-LB%20--%20LoRaWAN%20Pressure%20Sensor/
#  Dragino Repository: https://github.com/dragino/dragino-end-node-decoder/blob/main/PS-LB/PS%20LB%20Chirpstack%20V4%20decoder.txt

import string

if !global.psli5Nodes      # data survive to decoder reload
  global.psli5Nodes = {}
end

class LwDecoPSLI5
  static def decodeUplink(Name, Node, RSSI, FPort, Bytes)
    var data = {"Device":"Dragino PS-LB/LS-I5"}
    
    var valid_values = false
    var last_seen = 1451602800
    var battery_last_seen = 1451602800
    var battery = 1000
    var rssi = RSSI
    var Water_deep_cm = 0
	
    var Probe_mod
    var IDC_input_mA
    var modelRangeCm = 500      # 4mA=0cm, 20mA=500cm

    if global.psli5Nodes.find(Node)
      last_seen         = global.psli5Nodes.item(Node)[2]
      battery_last_seen = global.psli5Nodes.item(Node)[3]
      battery           = global.psli5Nodes.item(Node)[4]
      rssi              = global.psli5Nodes.item(Node)[5]
      Water_deep_cm     = global.psli5Nodes.item(Node)[6]
	  
    end

    ## SENSOR DATA ##
    if 2 == FPort && 9 == Bytes.size()
	  ## eg  0e46 0000       197f 0000 00
	  ##     BATV ProbeModel mA   Volt Int
      last_seen = tasmota.rtc('local')
	  	  
	  battery_last_seen = tasmota.rtc('local')
      battery = ((Bytes[0] << 8) | Bytes[1]) / 1000.0
      data.insert("BattV",battery)
	  
      Probe_mod    =  Bytes[2]   
	  IDC_input_mA = (Bytes[4]<<8 | Bytes[5])/1000.0  
	  
	  if Probe_mod == 0x00    # Immersion Sensor
       if IDC_input_mA <= 4.0 
        Water_deep_cm = 0
       else
        Water_deep_cm = (IDC_input_mA - 4.0) * modelRangeCm / 16.0
	   end
	  end # Probe_mod

      data.insert("WaterDepth_cm" ,Water_deep_cm)
      data.insert("IDC_ma"        ,IDC_input_mA)	
      data.insert("ModelRange_cm" ,modelRangeCm)	  
	  
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
      if global.psli5Nodes.find(Node)
        global.psli5Nodes.remove(Node)
      end
      #                         sensor[0]   [1]   [2]        [3]                [4]      [5]   [6]
      global.psli5Nodes.insert(Node, [Name, Node, last_seen, battery_last_seen, battery, RSSI, Water_deep_cm])
    end

    return data
  end #decodeUplink()

  static def add_web_sensor()
    var msg = ""
    for sensor: global.psli5Nodes
      var name = sensor[0]
      if string.find(name, "PS-L-I5") > -1                               # If LoRaWanName contains PS-L-I5 use PS-L-I5-<node>
        name = string.format("PS-L-I5-%i", sensor[1])
      end
      var name_tooltip = "Dragino PS-L-I5"
      var last_seen = sensor[2]
      var battery_last_seen = sensor[3]
      var battery = sensor[4]
      var rssi = sensor[5]
      msg += lwdecode.header(name, name_tooltip, battery, battery_last_seen, rssi, last_seen)

      # Sensors
      var Water_deep_cm = sensor[6]
      msg += "<tr class='htr'><td colspan='4'>&#9478;"                   # |
      msg += string.format(" &#11123;&#xFE0F; %.1fcm", Water_deep_cm)    # тн│          
      msg += "{e}"                                                       # = </td></tr>
    end
    return msg
  end #add_web_sensor()
end #class

LwDeco = LwDecoPSLI5
