# LoRaWAN Decoder file for Dragino SN50v3-LB/LS
#
# References
#  User Manual:     https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/SN50v3-LB/
#  Codec Repository: https://github.com/dragino/dragino-end-node-decoder/tree/main/SN50_v3-LB

import string

if !global.DrgSN50v3LNodes      # data survive to decoder reload
  global.DrgSN50v3LNodes = {}
end

class LwDecoDrgSN50v3L
  static def decodeUplink(Name, Node, RSSI, FPort, Bytes)
    var data = {"Device":"Dragino SN50v3-L"}
    
    var valid_values = false
    var last_seen = 1451602800
    var battery_last_seen = 1451602800
    var battery = 1000
    var rssi = RSSI
	var WorkingMode =''
    var WorkingModes = ['IIC','Distance', '3ADC+IIC', '3DS18B20','Weight','1Count','3Interrupt','3ADC+1DS18B20','3DS18B20+2Count','PWM','TMP117','Count+SHT31']

    if global.DrgSN50v3LNodes.find(Node)
      last_seen         = global.DrgSN50v3LNodes.item(Node)[2]
      battery_last_seen = global.DrgSN50v3LNodes.item(Node)[3]
      battery           = global.DrgSN50v3LNodes.item(Node)[4]
      rssi              = global.DrgSN50v3LNodes.item(Node)[5]
    end

    ## SENSOR DATA ##
    if 2==FPort && Bytes.size()>10   #Variable length, depending on mode, but always 11 bytes or more
      valid_values = true
      last_seen = tasmota.rtc('local')  
	  
      var mode=(Bytes[6] & 0x7C)>>2
	  if (mode+1) > size(WorkingModes)  mode = 0 end
      WorkingMode = WorkingModes[mode]
	  data.insert("WorkingMode", WorkingMode) #mode in data = 0..11. Mode in documentation = 1..12
   
      battery = (Bytes[0]<<8 | Bytes[1])/1000.0
	  data.insert("BattV", battery)
      battery_last_seen = tasmota.rtc('local')
		
	  ### TBA - handle all of the many cases	
	  if 0==mode  # Mode 1 (default)
	    if((Bytes[2]!=0x7f)||(Bytes[3]!=0xFF)) data.insert('TempC1',(Bytes[2]<<8 | Bytes[3])/10.0) end
        data.insert('Digital_IStatus', (Bytes[6]&0x02)? 'High':'Low')
		data.insert('ADC1_V',(Bytes[4]<<8 | Bytes[5])/1000.0)
		
		data.insert('EXTI_Trigger',(Bytes[6] & 0x01)? 'TRUE':'FALSE')
        data.insert('Door_status' ,(Bytes[6] & 0x80)? 'CLOSE':'OPEN')
			
		if((Bytes[9]<<8 | Bytes[10])==0)
         data.insert('Illum',(Bytes[7]<<8 | Bytes[8]))
        else 
         var noshowTemp = ((Bytes[7]==0x7f)&&(Bytes[8]==0xff))||((Bytes[7]==0xff)&&(Bytes[8]==0xff))
		 if !noshowTemp  data.insert('TempC_SHT',((Bytes[7]<<24>>16 | Bytes[8])/10.0)) end
		end
  
        if((Bytes[9]!=0xff)||(Bytes[10]!=0xff)) data.insert('Hum_SHT',(((Bytes[9]<<8 | Bytes[10])/10.0))) end
  	  
	  end #mode

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
      if global.DrgSN50v3LNodes.find(Node)
        global.DrgSN50v3LNodes.remove(Node)
      end
      #                             sensor[0]    [1]   [2]        [3]                [4]      [5]   [6]
      global.DrgSN50v3LNodes.insert(Node, [Name, Node, last_seen, battery_last_seen, battery, RSSI, WorkingMode])   
    end

    return data
  end #decodeUplink()

  static def add_web_sensor()
    var msg = ""
    for sensor: global.DrgSN50v3LNodes
      var name = sensor[0]
      if string.find(name, "SN50v3-L") > -1                                   # If LoRaWanName contains SN50v3 use SN50v3-<node>
        name = string.format("SN50v3-L-%i", sensor[1])
      end
      var name_tooltip = "Dragino SN50v3-L"
      var last_seen         = sensor[2]
      var battery_last_seen = sensor[3]
      var battery           = sensor[4]
      var rssi              = sensor[5]
      var workingMode       = sensor[6]

      msg += lwdecode.header(name, name_tooltip, battery, battery_last_seen, rssi, last_seen)

      # Sensors
      msg += "<tr class='htr'><td colspan='4'>&#9478;"                   # |

      msg += string.format(" &#xFE0F; %s", workingMode)         
	  
      msg += "{e}"                                                       # = </td></tr>
    end
    return msg
  end #add_web_sensor()
end #class

LwDeco = LwDecoDrgSN50v3L
