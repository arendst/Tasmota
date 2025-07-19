# LoRaWAN Decoder file for an example DEVICE
#
# References
#  Manufacturer Reference: https://xxxxxx    <== URL TO Manufacturer's documentation explaining how to decode raw data
#  TTN Device Repository:  https://github.com/TheThingsNetwork/lorawan-devices/blob/master/vendor/xxxxxx/DEVICE

# import Berry modules, if needed.
# the string module is not needed in this decoder; shown as an example. 
import string

# define a global map for storage of GUI parameters
global.DEVICENodes = {}

# Declare a new Class
# The Class name should follow this format:  LwDecoXXXX  where XXXX is the DEVICE
class LwDecoDEVICE

# Define this function exactly as below. 
#  Name: decodeUplink     Must use this name, and arguments
#  Arguments:  FPort      The Fport number used by the End Device for this packet of data
#              Bytes      The Raw Data Payload
  static def decodeUplink(Node, FPort, Bytes)
 
# Create the data structure (a Berry 'map'), and populate with the VENDOR & DEVICE names 
    var data = {"Device":"VENDOR DEVICE"}
    data.insert("Node", Node)

    var valid_values = false
    var gui_value    
# For each Fport used by the DEVICE:
#   write a decoder that continues to populate the data structure by parsing the Raw Data Payload 

    if 2 == FPort && 11 == Bytes.size()                #Example: For this device, Data from Fport 2 should have 11 bytes
      data.insert("LABEL1", Bytes[0] | Bytes[1] <<8 )    #Example Numerical value = Bytes[1]*256 + Bytes[0]
      gui_value = Bytes[0] | Bytes[1] <<8
      data.insert("LABEL2", "TEXT VALUE")                #Example Text value


      var valid_values = true

    else
      # Ignore other Fports
    end #Fport

    if valid_values
      if global.DEVICENodes.find(Node)
        global.DEVICENodes.remove(Node)
      end
      global.DEVICENodes.insert(Node, [Node, gui_value])
    end

    return data
  end #decodeUplink()

  static def add_web_sensor()
    var msg = ""
    for sensor: global.DEVICENodes
      msg += string.format("{s}DEVICE_%i Gui value{m}%i{e}",
                           sensor[0], sensor[1])
    end

    return msg
  end #add_web_sensor()

end #class

# Set LwDeco variable to the new Class
LwDeco = LwDecoDEVICE    # LwDeco: Do not Change
                         # Value:  MUST match the class name defined above.
