# Milesight WS523 Portable Socket Driver
# Driver Version: 2.0.0
# Device Support: WS523 Portable Smart Socket
# Protocol: LoRaWAN

class LwDecode_WS523
  var hashCheck
  var name
  var node
  var last_data
  var last_update
  
  def init()
    self.hashCheck = false
    self.name = nil
    self.node = nil
    self.last_data = {}
    self.last_update = 0
  end
  
  def decodeUplink(name, node, rssi, fport, payload)
    self.name = name
    self.node = node
    
    var data = {"fport": fport}
    var i = 0
    
    # Parse channel-based payload (simplified for brevity)
    while i < payload.size() - 1
      if i + 1 >= payload.size() break end
      
      var channel_id = payload[i]
      var channel_type = payload[i+1]
      i += 2
      
      # Basic channels implemented (WS523 has no commands per spec)
      if channel_id == 0x03 && channel_type == 0x74 && i + 2 <= payload.size()
        data["voltage"] = (payload[i] | (payload[i+1] << 8)) / 10.0
        i += 2
      elif channel_id == 0x04 && channel_type == 0x80 && i + 4 <= payload.size()
        data["active_power"] = payload[i] | (payload[i+1] << 8) | (payload[i+2] << 16) | (payload[i+3] << 24)
        i += 4
      elif channel_id == 0x08 && channel_type == 0x70 && i + 1 <= payload.size()
        data["socket_status"] = payload[i] == 0x01
        i += 1
      else
        # Skip unknown channels
        if channel_type >= 0x70 && channel_type <= 0x7F
          i += 1
        elif channel_type >= 0x80 && channel_type <= 0x8F
          i += 4
        else
          i += 1
        end
      end
    end
    
    self.last_data = data
    self.last_update = tasmota.rtc('local')
    return data
  end
  
  def add_web_sensor()
    if !self.last_data || self.last_update == 0 return nil end
    
    var msg = lwdecode.header(
      format("WS523-%d", self.node),
      "Milesight WS523 Portable Socket",
      1000, self.last_update, 1000, self.last_update
    )
    
    var fmt = LwSensorFormatter_cls()
    fmt.start_line()
    
    if self.last_data.find("socket_status") != nil
      var status = self.last_data["socket_status"]
      var icon = status ? "🟢" : "⚫"
      var text = status ? "ON" : "OFF"
      fmt.add_sensor("string", text, "Socket", icon)
    end
    
    if self.last_data.find("voltage") != nil
      fmt.add_sensor("volt", self.last_data["voltage"], "Voltage", nil)
    end
    
    if self.last_data.find("active_power") != nil
      fmt.add_sensor("power", self.last_data["active_power"], "Power", nil)
    end
    
    fmt.end_line()
    var sensor_msg = fmt.get_msg()
    if sensor_msg != nil
        msg = msg + sensor_msg
    end
    
    return msg
  end
end

# Register the decoder
LwDeco = LwDecode_WS523()

# Test command - WS523 has no configurable downlinks per spec
tasmota.remove_cmd("LwWS523TestPayload")
tasmota.add_cmd("LwWS523TestPayload", def(cmd, idx, payload_str)
    var test_payload = bytes(payload_str)
    var result = LwDeco.decodeUplink("TestWS523", "test_node", -85, idx, test_payload)
    
    if result != nil
        import json
        tasmota.resp_cmnd(json.dump(result))
    else
        tasmota.resp_cmnd_error()
    end
end)
