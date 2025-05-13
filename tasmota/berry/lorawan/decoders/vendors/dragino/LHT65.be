# LoRaWAN Decoder file for Dragino LHT65
#
# References
#  User Manual:     https://www.dragino.com/downloads/downloads/LHT65/UserManual/LHT65_Temperature_Humidity_Sensor_UserManual_v1.8.5.pdf
#  TTN Device Repository: https://github.com/TheThingsNetwork/lorawan-devices/blob/master/vendor/dragino/lht65.js

import string
var LHT65_BatteryStatus = ["Very low <= 2.5V","Low <=2.55V","OK","Good >= 2.65V"]

class LwDecoLHT65
 static def decodeUplink(FPort, Bytes)
  var data = {"Device":"Dragino LHT65"}
  data.insert("poll_message_status",(Bytes[6] & 0x40) >> 6)
  var Ext = Bytes[6] & 0x0F  #External sensor type
  var NoConnect = (Bytes[6] & 0x80) >> 7
    
  ## SENSOR DATA ##
  if  2 == FPort && Bytes.size() == 11
   var TempC 
   if Ext == 9 #Sensor E3, Temperature Sensor, Datalog Mod
    TempC = ((Bytes[0] << 8) | Bytes[1])
	if 0x7FFF == TempC
	 data.insert("Ext_SensorConnected", false)
	else
	 data.insert("Ext_SensorConnected", true)
	 if Bytes[0]>0x7F TempC -= 0x10000  end
	 data.insert("TempC_External", TempC / 100.0)
	end
    data.insert("Bat_status", LHT65_BatteryStatus[Bytes[4] >> 6])
   else 
    data.insert("BatV",(((Bytes[0] << 8) | Bytes[1]) & 0x3fff) / 1000.0)
    data.insert("Bat_status", LHT65_BatteryStatus[Bytes[0] >> 6])
   end
   
   if Ext != 0x0F 
    TempC = ((Bytes[2] << 8) | Bytes[3])
    if Bytes[2]>0x7F TempC -= 0x10000  end	 
    data.insert("TempC_Internal", ( TempC / 100.0))
    data.insert("Hum_Internal"  , (((Bytes[4] << 8) | Bytes[5]) / 10.0))
   end
	
   if NoConnect
    data.insert('No_connect','No connection to external sensor')
   end
   
   if   0==Ext
    data.insert("Ext_sensor", 'No external sensor')
   elif 1==Ext
    data.insert("Ext_sensor",'Temperature Sensor')
    TempC = ((Bytes[7] << 8) | Bytes[8])
    if 0x7FFF == TempC
     data.insert("Ext_SensorConnected", false)
    else
     data.insert("Ext_SensorConnected", true)
     if Bytes[7]>0x7F TempC -= 0x10000  end
     data.insert("TempC_External", TempC / 100.0)
    end		
   elif 4==Ext
    data.insert("Work_mode", 'Interrupt Sensor send')
    data.insert("Exti_pin_level", Bytes[7] ? 'High' : 'Low')
    data.insert("Exti_status", Bytes[8] ? 'True' : 'False')
   elif 5==Ext
    data.insert("Work_mode", 'Illumination Sensor')
    data.insert("ILL_lx", (Bytes[7] << 8) | Bytes[8])
   elif 6==Ext
    data.insert("Work_mode", 'ADC Sensor')
    data.insert("ADC_V", ((Bytes[7] << 8) | Bytes[8]) / 1000.0)
   elif 7==Ext
    data.insert("Work_mode ", 'Interrupt Sensor count')
    data.insert("Exit_count", (Bytes[7] << 8) | Bytes[8])
   elif 8==Ext
    data.insert("Work_mode", 'Interrupt Sensor count')
    data.insert("Exit_count", (Bytes[7] << 24) | (Bytes[8] << 16) | (Bytes[9] << 8) | Bytes[10])
   elif 9==Ext
    data.insert("Work_mode", 'DS18B20 & timestamp')
    data.insert("Systimestamp", (Bytes[7] << 24) | (Bytes[8] << 16) | (Bytes[9] << 8) | Bytes[10])
   elif 15==Ext
    data.insert("Work_mode",'DS18B20ID')
    data.insert("ID",f"{Bytes[2]:%02X}" + f"{Bytes[3]:%02X}" + f"{Bytes[4]:%02X}" + f"{Bytes[5]:%02X}" + f"{Bytes[6]:%02X}" + f"{Bytes[8]:%02X}" + f"{Bytes[9]:%02X}" + f"{Bytes[10]:%02X}" ) 
   else
    data.insert("Ext_sensor", 'Unknown')
   end   
  
  elif 5 == FPort && Bytes.size() == 7
   data.insert("Sensor_Model",Bytes[0])
   data.insert("Firmware_Version", f'v{Bytes[1]:%u}.{Bytes[2]>>4:%u}.{Bytes[2]&0xF:%u}')
   data.insert("Freq_Band",LwRegions[Bytes[3]-1])
   data.insert("Sub_Band",Bytes[4])
   data.insert("Bat_mV",(Bytes[5] << 8) | Bytes[6])
  
  else
   # Ignore other Fports  
  end #Fport 
  
  return data
 end # decodeUplink()
end # class

LwDeco = LwDecoLHT65