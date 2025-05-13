# Decoder files are modeled on the *.js files found here:
#  https://github.com/TheThingsNetwork/lorawan-devices/tree/master/vendor

var LwRegions = ["EU868", "US915", "IN865","AU915","KZ865","RU864","AS923", "AS923-1","AS923-2","AS923-3"]

import mqtt 
tasmota.cmd('SetOption100 off')
tasmota.cmd('SetOption118 off')
tasmota.cmd('SetOption119 off')
tasmota.cmd('LoRaWanBridge on')
var thisDevice = tasmota.cmd('Status',true)['Status']['Topic']
var LwDecoders = {}
var LwDeco

def LwDecode(topic, idx, data, databytes)
  import json

  var LwData = json.load(data)
  if !LwData.contains('LwReceived') return true end # Processed
  var deviceData = LwData['LwReceived']
  var deviceName = deviceData.keys()()
  var Payload = deviceData[deviceName]['Payload']
  var FPort = deviceData[deviceName]['FPort']
  var decoder = deviceData[deviceName].find('Decoder')
  if !decoder return true end
  
  if !LwDecoders.find(decoder)
   LwDeco = nil
   load(decoder)  #sets LwDeco if found
   if LwDeco LwDecoders.insert(decoder, LwDeco) end
  end 

  if Payload.size() && LwDecoders.find(decoder)
   var decoded = LwDecoders[decoder].decodeUplink(FPort, Payload)	
   var mqttData = {"LwDecoded":{deviceName:decoded}}
   mqtt.publish	(topic, json.dump(mqttData))
  end 

  return true  #processed
end

mqtt.subscribe("tele/" + thisDevice + "/SENSOR",LwDecode)