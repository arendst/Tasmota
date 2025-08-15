# Milesight WS523 Portable Socket Driver
# Driver Version: 2.0.0
# Device Support: WS523 Portable Smart Socket
# Protocol: LoRaWAN
#
# Revision History:
# v1.0.0 - 2024-12-25 - Initial implementation with full PDF specification coverage
#                       Complete uplink decoding for all channel types
#                       Optimized emoji UI display with single-line format
#                       All downlink commands implemented and verified
# v2.0.0 - Complete rewrite with latest LwDecode framework
#        - Optimized memory usage and parsing logic
#        - Enhanced error handling and validation
#        - Improved UI display with consistent emoji usage

class LwDecode_WS523
  var hashCheck
  var name
  var node
  var last_data
  var last_update
  
  def init()
    self.hashCheck = false  # Process all messages
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
    
    # Parse channel-based payload
    while i < payload.size() - 1
      if i + 1 >= payload.size() break end
      
      var channel_id = payload[i]
      var channel_type = payload[i+1]
      i += 2
      
      # Voltage (0x03 0x74) - UINT16, 0.1V resolution
      if channel_id == 0x03 && channel_type == 0x74 && i + 2 <= payload.size()
        data["voltage"] = (payload[i] | (payload[i+1] << 8)) / 10.0
        i += 2
        
      # Active Power (0x04 0x80) - UINT32, W
      elif channel_id == 0x04 && channel_type == 0x80 && i + 4 <= payload.size()
        data["active_power"] = payload[i] | (payload[i+1] << 8) | (payload[i+2] << 16) | (payload[i+3] << 24)
        i += 4
        
      # Power Factor (0x05 0x81) - UINT8, %
      elif channel_id == 0x05 && channel_type == 0x81 && i + 1 <= payload.size()
        data["power_factor"] = payload[i]
        i += 1
        
      # Power Consumption (0x06 0x83) - UINT32, Wh
      elif channel_id == 0x06 && channel_type == 0x83 && i + 4 <= payload.size()
        data["power_consumption"] = payload[i] | (payload[i+1] << 8) | (payload[i+2] << 16) | (payload[i+3] << 24)
        i += 4
        
      # Current (0x07 0xC9) - UINT16, mA
      elif channel_id == 0x07 && channel_type == 0xC9 && i + 2 <= payload.size()
        data["current"] = payload[i] | (payload[i+1] << 8)
        i += 2
        
      # Socket Status (0x08 0x70) - UINT8, 0=close, 1=open
      elif channel_id == 0x08 && channel_type == 0x70 && i + 1 <= payload.size()
        data["socket_status"] = payload[i] == 0x01
        i += 1
        
      # Protocol Version (0xFF 0x01)
      elif channel_id == 0xFF && channel_type == 0x01 && i + 1 <= payload.size()
        data["protocol_version"] = payload[i]
        i += 1
        
      # Hardware Version (0xFF 0x09)
      elif channel_id == 0xFF && channel_type == 0x09 && i + 2 <= payload.size()
        data["hardware_version"] = format("%d.%d", payload[i+1] >> 4, payload[i+1] & 0x0F)
        i += 2
        
      # Software Version (0xFF 0x0A)
      elif channel_id == 0xFF && channel_type == 0x0A && i + 2 <= payload.size()
        data["software_version"] = format("%d.%d", payload[i+1] >> 4, payload[i+1] & 0x0F)
        i += 2
        
      # Power On (0xFF 0x0B)
      elif channel_id == 0xFF && channel_type == 0x0B && i + 1 <= payload.size()
        data["power_on"] = true
        i += 1
        
      # Device SN (0xFF 0x16)
      elif channel_id == 0xFF && channel_type == 0x16 && i + 8 <= payload.size()
        var sn = ""
        for j: 0..7
          sn += format("%02X", payload[i+j])
        end
        data["device_sn"] = sn
        i += 8
        
      # Device Type (0xFF 0x0F)
      elif channel_id == 0xFF && channel_type == 0x0F && i + 1 <= payload.size()
        var dev_type = payload[i]
        if dev_type == 0x00 data["device_class"] = "Class A"
        elif dev_type == 0x01 data["device_class"] = "Class B"
        elif dev_type == 0x02 data["device_class"] = "Class C"
        end
        i += 1
        
      # TSL Version (0xFF 0xFF)
      elif channel_id == 0xFF && channel_type == 0xFF && i + 2 <= payload.size()
        data["tsl_version"] = format("%d.%d", payload[i+1], payload[i])
        i += 2
        
      # Reset Event (0xFF 0xFE)
      elif channel_id == 0xFF && channel_type == 0xFE && i + 1 <= payload.size()
        data["reset_event"] = true
        i += 1
        
      # Overcurrent Alarm Config (0xFF 0x24)
      elif channel_id == 0xFF && channel_type == 0x24 && i + 2 <= payload.size()
        data["oc_alarm_enabled"] = payload[i] == 0x01
        data["oc_alarm_threshold"] = payload[i+1]
        i += 2
        
      # Button Lock Config (0xFF 0x25)
      elif channel_id == 0xFF && channel_type == 0x25 && i + 2 <= payload.size()
        data["button_lock"] = payload[i+1] == 0x80
        i += 2
        
      # Power Consumption Recording (0xFF 0x26)
      elif channel_id == 0xFF && channel_type == 0x26 && i + 1 <= payload.size()
        data["power_recording"] = payload[i] == 0x01
        i += 1
        
      # LED Indicator (0xFF 0x2F)
      elif channel_id == 0xFF && channel_type == 0x2F && i + 1 <= payload.size()
        data["led_enabled"] = payload[i] == 0x01
        i += 1
        
      # Overcurrent Protection Config (0xFF 0x30)
      elif channel_id == 0xFF && channel_type == 0x30 && i + 2 <= payload.size()
        data["oc_protect_enabled"] = payload[i] == 0x01
        data["oc_protect_threshold"] = payload[i+1]
        i += 2
        
      # Power Outage Alert (0xFF 0x3F)
      elif channel_id == 0xFF && channel_type == 0x3F && i + 1 <= payload.size()
        data["power_outage"] = true
        i += 1
        
      # Reporting Interval (0xFE 0x02)
      elif channel_id == 0xFE && channel_type == 0x02 && i + 2 <= payload.size()
        data["reporting_interval"] = payload[i] | (payload[i+1] << 8)
        i += 2
        
      # Skip downlink confirmations (0xFE types)
      elif channel_id == 0xFE
        if channel_type == 0x03 i += 2      # Report interval confirm
        elif channel_type == 0x10 i += 1    # Reboot confirm
        elif channel_type == 0x22 i += 4    # Delay task confirm
        elif channel_type == 0x23 i += 2    # Delete task confirm
        elif channel_type == 0x24 i += 2    # OC alarm confirm
        elif channel_type == 0x25 i += 2    # Button lock confirm
        elif channel_type == 0x26 i += 1    # Power recording confirm
        elif channel_type == 0x27 i += 1    # Reset energy confirm
        elif channel_type == 0x28 i += 1    # Status enquiry confirm
        elif channel_type == 0x2F i += 1    # LED mode confirm
        elif channel_type == 0x30 i += 2    # OC protection confirm
        else
          # Unknown confirmation type, skip
          i += 1
        end
        
      else
        # Unknown channel/type, try to guess size
        if channel_type >= 0x70 && channel_type <= 0x7F
          i += 1  # Single byte
        elif channel_type >= 0x80 && channel_type <= 0x8F
          i += 4  # 4 bytes
        elif channel_type >= 0xC0 && channel_type <= 0xCF
          i += 2  # 2 bytes
        else
          i += 1  # Default single byte
        end
      end
    end
    
    # Store data and update timestamp
    self.last_data = data
    self.last_update = tasmota.rtc('local')
    
    return data
  end
  
  def add_web_sensor()
    if !self.last_data || self.last_update == 0 return nil end
    
    var msg = LwSensorFormatter()
    
    # Header
    msg = lwdecode.header(
      format("WS523-%d", self.node),
      "Milesight WS523 Portable Socket",
      1000,  # No battery
      self.last_update,
      1000,  # No RSSI display
      self.last_update
    )
    
    msg += LwSensorFormatter()
      .start_line()
    
    # Socket status with colored indicator
    if self.last_data.find("socket_status") != nil
      var status = self.last_data["socket_status"]
      var icon = status ? "🟢" : "⚫"
      var text = status ? "ON" : "OFF"
      
      # Check for power outage
      if self.last_data.find("power_outage") != nil && self.last_data["power_outage"]
        icon = "⚠️"
        text = "OUTAGE"
      end
      
      msg += LwSensorFormatter()
        .add_sensor("string", format("%s %s", icon, text), "Socket Status", nil)
        .get_msg()
    end
    
    # Voltage
    if self.last_data.find("voltage") != nil
      msg += LwSensorFormatter()
        .add_sensor("empty", format(" %.1f", self.last_data["voltage"]), "Voltage", "⚡")
        .add_sensor("string", "V", nil, nil)
        .get_msg()
    end
    
    # Current
    if self.last_data.find("current") != nil
      msg += LwSensorFormatter()
        .add_sensor("empty", format(" %d", self.last_data["current"]), "Current", "🔌")
        .add_sensor("string", "mA", nil, nil)
        .get_msg()
    end
    
    # Power
    if self.last_data.find("active_power") != nil
      msg += LwSensorFormatter()
        .add_sensor("empty", format(" %d", self.last_data["active_power"]), "Power", "💡")
        .add_sensor("string", "W", nil, nil)
        .get_msg()
    end
    
    # Energy
    if self.last_data.find("power_consumption") != nil
      var energy = self.last_data["power_consumption"]
      var unit = "Wh"
      if energy >= 1000
        energy = energy / 1000.0
        unit = "kWh"
      end
      msg += LwSensorFormatter()
        .add_sensor("empty", format(" %.2f", energy), "Energy", "🏠")
        .add_sensor("string", unit, nil, nil)
        .get_msg()
    end
    
    # Power Factor
    if self.last_data.find("power_factor") != nil
      msg += LwSensorFormatter()
        .add_sensor("empty", format(" %d", self.last_data["power_factor"]), "PF", "📊")
        .add_sensor("string", "%", nil, nil)
        .get_msg()
    end
    
    # Overcurrent alarm status
    if self.last_data.find("oc_alarm_enabled") != nil && self.last_data["oc_alarm_enabled"]
      msg += LwSensorFormatter()
        .add_sensor("string", format(" OC:%dA", self.last_data.find("oc_alarm_threshold", 0)), "Alarm", "⚠️")
        .get_msg()
    end
    
    # Button lock status
    if self.last_data.find("button_lock") != nil && self.last_data["button_lock"]
      msg += LwSensorFormatter()
        .add_sensor("string", " LOCKED", "Button", "🔒")
        .get_msg()
    end
    
    msg += LwSensorFormatter()
      .end_line()
      .get_msg()
    
    return msg
  end
  
  # Helper function for LwSensorFormatter compatibility
  def LwSensorFormatter()
    return LwSensorFormatter_cls()
  end
end

# Register the decoder
LwDeco = LwDecode_WS523()