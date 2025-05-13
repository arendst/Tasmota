# LoRaWAN Decoder file for Dragino LDS02
#
# References
#  LHT52 User Manual:     https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/LDS02%20-%20LoRaWAN%20Door%20Sensor%20User%20Manual/
#  TTN Device Repository: https://github.com/TheThingsNetwork/lorawan-devices/blob/master/vendor/dragino/lds02.js

class LwDecoLDS02

 static def decodeUplink(FPort, Bytes)
  var data = {"Device":"Dragino LDS02"}
    
  ## SENSOR DATA ##
  
  if  10 == FPort && Bytes.size() == 10
   data.insert("DoorOpen", ( Bytes[0] & 0x80 ) ? true : false )
   data.insert("Battery_mV", ( Bytes[1] | (Bytes[0] << 8) & 0x3FFF ))
   data.insert("DoorOpenEvents", Bytes[5] | (Bytes[4] << 8) | (Bytes[3] << 16 ))
   data.insert("DoorOpenLastDuration_mins", Bytes[8] | (Bytes[7] << 8) | (Bytes[6] << 16))
   data.insert("Alarm", (Bytes[9] & 0x01 ) ? true : false)
   
  else
   # Ignore other Fports
  end #Fport

 return data
 end #decodeUplink()
end #class

LwDeco = LwDecoLDS02
