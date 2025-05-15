# LoRaWAN Decoder file for MerryIoT DW10 Open/Close
#
# References
#  DW10 Product information: https://www.browan.com/products-detail/OpenClose-Sensor-EBL-LoRaWAN/
#  Browan JS Decoder (TTN):  https://www.browan.com/member/login/?refererUrl=https%3A%2F%2Fwww.browan.com%2Fproducts-detail%2FOpenClose-Sensor-EBL-LoRaWAN%2F

class LwDecoDW10

 static def decodeUplink(FPort, Bytes)
  var data = {"Device":"MerryIoT DW10"}
    
  ## SENSOR DATA ##
  
  if 120 == FPort && Bytes.size() == 9
   data.insert("DoorOpen", ( Bytes[0] & 0x01 ) ? true : false )
   data.insert("ButtonPress", ( Bytes[0] & 0x02 ) ? true : false )
   data.insert("TamperDetect", ( Bytes[0] & 0x04 ) ? true : false )
   data.insert("TiltDetect", ( Bytes[0] & 0x08 ) ? true : false )
   data.insert("Battery_mV", ( 21 + Bytes[1] ) * 100 )
   data.insert("Temperature_C", Bytes[2])
   data.insert("Humidity", Bytes[3])
   data.insert("DoorOpenLastDuration_mins", Bytes[4] | (Bytes[5] << 8))
   data.insert("DoorOpenEvents", Bytes[6] | (Bytes[7] << 8) | (Bytes[8] << 16 ))
   
  else
   # Ignore other Fports
  end #Fport

 return data
 end #decodeUplink()
end #class

LwDeco = LwDecoDW10
