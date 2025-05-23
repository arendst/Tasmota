# Decoder files are modeled on the *.js files found here:
#  https://github.com/TheThingsNetwork/lorawan-devices/tree/master/vendor

var LwRegions = ["EU868", "US915", "IN865","AU915","KZ865","RU864","AS923", "AS923-1","AS923-2","AS923-3"]
var LwDeco

import mqtt 

class lwdecode_cls
  var thisDevice
  var LwDecoders

  def init()
    self.thisDevice = tasmota.cmd('Status',true)['Status']['Topic']
    self.LwDecoders = {}

    if global.lwdecode_driver
      global.lwdecode_driver.stop() # Let previous instance bail out cleanly
    end
    tasmota.add_driver(global.lwdecode_driver := self)
    tasmota.add_rule("LwReceived", /value, trigger, payload -> self.LwDecode(payload))
  end

  def LwDecode(data)
    import json

    var deviceData = data['LwReceived']
    var deviceName = deviceData.keys()()
    var Node = deviceData[deviceName]['Node']
    var Payload = deviceData[deviceName]['Payload']
    var FPort = deviceData[deviceName]['FPort']
    var decoder = deviceData[deviceName].find('Decoder')
    if !decoder 
      return true
    end
    
    if !self.LwDecoders.find(decoder)
      LwDeco = nil
      load(decoder)  #sets LwDeco if found
      if LwDeco
        self.LwDecoders.insert(decoder, LwDeco)
      end
    end 

    if Payload.size() && self.LwDecoders.find(decoder)
      var topic = "tele/" + self.thisDevice + "/SENSOR"
      var decoded = self.LwDecoders[decoder].decodeUplink(Node, FPort, Payload)	
      var mqttData = {"LwDecoded":{deviceName:decoded}}
      mqtt.publish(topic, json.dump(mqttData))
    end 

    return true  #processed
  end

  #------------------------------------------------------------
  Display sensor value in the web UI and react to button
  Called every WebRefresh time
  ------------------------------------------------------------#
  def web_sensor()
    import string

    var msg = ""
    for decoder: self.LwDecoders
      msg = msg + decoder.add_web_sensor()	
    end
    if msg
      tasmota.web_send_decimal(msg)
    end
  end
end

lwdecode = lwdecode_cls()

tasmota.cmd('SetOption100 off')   # Keep LwReceived in JSON message
tasmota.cmd('SetOption118 off')   # Keep SENSOR as subtopic name
tasmota.cmd('SetOption119 off')   # Keep device address in JSON message
tasmota.cmd('SetOption147 on')    # Hide LwReceived MQTT message but keep rule processing
tasmota.cmd('LoRaWanBridge on')
