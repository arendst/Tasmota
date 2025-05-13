# LoRaWAN Decoder file for an example DEVICE
#
# References
#  Manufacturer Reference: https://xxxxxx    <== URL TO Manufacturer's documentation explaining how to decode raw data
#  TTN Device Repository:  https://github.com/TheThingsNetwork/lorawan-devices/blob/master/vendor/xxxxxx/DEVICE

# import Berry modules, if needed.
# the string module is not needed in this decoder; shown as an example. 
import string

# Declare a new Class
# The Class name should follow this format:  LwDecoXXXX  where XXXX is the DEVICE
class LwDecoDEVICE

# Define this function exactly as below. 
#  Name: decodeUplink     Must use this name, and arguments
#  Arguments:  FPort      The Fport number used by the End Device for this packet of data
#              Bytes      The Raw Data Payload
 static def decodeUplink(FPort, Bytes)
 
# Create the data structure (a Berry 'map'), and populate with the VENDOR & DEVICE names 
  var data = {"Device":"VENDOR DEVICE"}
    
# For each Fport used by the DEVICE:
#   write a decoder that continues to populate the data structure by parsing the Raw Data Payload 

  if  2 == FPort && 11 == Bytes.size()                #Example: For this device, Data from Fport 2 should have 11 bytes
   data.insert("LABEL1", Bytes[0] | Bytes[1] <<8 )    #Example Numerical value = Bytes[1]*256 + Bytes[0]
   data.insert("LABEL2", "TEXT VALUE")                #Example Text value

  else
   # Ignore other Fports
  end #Fport

 return data
 end #decodeUplink()
end #class

# Set LwDeco variable to the new Class
LwDeco = LwDecoDEVICE    # LwDeco: Do not Change
                         # Value:  MUST match the class name defined above.
