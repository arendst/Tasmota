# Dragino LHT65 Temperature & Humidity Sensor Driver
# Driver Version: 2.0.0
# Device Support: LHT65 with E1-E9 external sensors
# Protocol: LoRaWAN Class A
#
# Revision History:
# v1.0.0 - Initial implementation with E1-E9 sensor support
# v2.0.0 - Complete rewrite with latest LwDecode framework
#        - Optimized memory usage and parsing logic
#        - Enhanced error handling and validation
#        - Improved UI display with consistent emoji usage

class LwDecode_LHT65
  var hashCheck
  var name
  var node
  var last_data
  var last_update
  var last_battery_update
  
  def init()
    self.hashCheck = false  # Process all messages for testing
    self.name = nil
    self.node = nil
    self.last_data = {}
    self.last_update = 0
    self.last_battery_update = 0
  end
  
  def decodeUplink(name, node, rssi, fport, payload)
    self.name = name
    self.node = node
    
    var data = {"fport": fport}
    
    # Minimum payload size check
    if payload.size() < 6
      return {"error": "Invalid payload size"}
    end
    
    # Check for timestamp mode (E9) - 11 bytes with specific format
    if payload.size() == 11 && (payload[6] & 0x0F) == 0x09
      # Timestamp mode: ExtTemp(2) + BuiltTemp(2) + BatStatusHum(2) + StatusExt(1) + UnixTime(4)
      
      # External temperature (bytes 0-1)
      var ext_temp_raw = payload[0] | (payload[1] << 8)
      if ext_temp_raw == 0x7FFF
        data["ext_temp_error"] = "No probe"
      else
        if ext_temp_raw & 0x8000
          ext_temp_raw -= 0x10000  # Handle negative
        end
        data["ext_temperature"] = ext_temp_raw / 100.0
      end
      
      # Built-in temperature (bytes 2-3)
      var temp_raw = payload[2] | (payload[3] << 8)
      if temp_raw & 0x8000
        temp_raw -= 0x10000
      end
      data["temperature"] = temp_raw / 100.0
      
      # Battery status & humidity (bytes 4-5)
      var bat_hum = payload[4] | (payload[5] << 8)
      var bat_status = (bat_hum >> 14) & 0x03
      data["battery_status"] = bat_status
      data["humidity"] = (bat_hum & 0x3FFF) / 10.0
      
      # Status byte (byte 6)
      var status = payload[6]
      data["poll_message"] = (status & 0x20) != 0
      data["sync_time_ok"] = (status & 0x10) != 0
      data["time_request"] = (status & 0x08) != 0
      data["ext_sensor"] = status & 0x0F
      
      # Unix timestamp (bytes 7-10)
      data["timestamp"] = payload[7] | (payload[8] << 8) | (payload[9] << 16) | (payload[10] << 24)
      
    else
      # Standard mode: BAT(2) + Temp(2) + Hum(2) + [Ext(1) + ExtVal(4)]
      
      # Battery (bytes 0-1)
      var battery_raw = payload[0] | (payload[1] << 8)
      var bat_status = (battery_raw >> 14) & 0x03
      var bat_voltage = battery_raw & 0x3FFF
      
      data["battery_status"] = bat_status
      data["battery_mv"] = bat_voltage
      data["battery_v"] = bat_voltage / 1000.0
      
      # Temperature (bytes 2-3)
      var temp_raw = payload[2] | (payload[3] << 8)
      if temp_raw & 0x8000
        temp_raw -= 0x10000
      end
      data["temperature"] = temp_raw / 100.0
      
      # Humidity (bytes 4-5)
      var hum_raw = payload[4] | (payload[5] << 8)
      data["humidity"] = hum_raw / 10.0
      
      # External sensor (if present)
      if payload.size() >= 11
        var ext_type = payload[6]
        data["ext_sensor"] = ext_type
        
        # External value (bytes 7-10)
        var ext_val = payload[7] | (payload[8] << 8) | (payload[9] << 16) | (payload[10] << 24)
        
        if ext_type == 0x01 || ext_type == 0x03  # E1/E3 Temperature
          var temp_val = ext_val & 0xFFFF
          if temp_val == 0x7FFF
            data["ext_temp_error"] = "No probe"
          else
            if temp_val & 0x8000
              temp_val -= 0x10000
            end
            data["ext_temperature"] = temp_val / 100.0
          end
          
        elif ext_type == 0x04  # E4 Interrupt
          data["cable_connected"] = (ext_val & 0x01) != 0
          data["interrupt_level"] = (ext_val & 0x02) != 0
          data["interrupt_status"] = (ext_val & 0x04) != 0
          
        elif ext_type == 0x05  # E5 Illumination
          data["cable_connected"] = (ext_val & 0x01) != 0
          if data["cable_connected"]
            data["illumination"] = (ext_val >> 8) & 0xFFFF
          end
          
        elif ext_type == 0x06  # E6 ADC
          data["cable_connected"] = (ext_val & 0x01) != 0
          if data["cable_connected"]
            data["adc_mv"] = (ext_val >> 8) & 0xFFFF
            data["adc_v"] = data["adc_mv"] / 1000.0
          end
          
        elif ext_type == 0x07  # E7 Counter 16-bit
          data["cable_connected"] = (ext_val & 0x01) != 0
          if data["cable_connected"]
            data["count_16"] = (ext_val >> 8) & 0xFFFF
          end
          
        elif ext_type == 0x08  # E8 Counter 32-bit
          data["cable_connected"] = (ext_val & 0x01) != 0
          if data["cable_connected"]
            data["count_32"] = ext_val >> 8
          end
        end
      end
    end
    
    # Store data and update timestamp
    self.last_data = data
    self.last_update = tasmota.rtc('local')
    if data.find("battery_status") != nil || data.find("battery_mv") != nil
      self.last_battery_update = tasmota.rtc('local')
    end
    
    return data
  end
  
  def add_web_sensor()
    if !self.last_data || self.last_update == 0 return nil end
    
    var msg = LwSensorFormatter()
    
    # Header with battery info
    var battery = 1000  # Default no battery
    if self.last_data.find("battery_v") != nil
      battery = self.last_data["battery_v"]
    elif self.last_data.find("battery_status") != nil
      # Map status to percentage: 0=5%, 1=25%, 2=75%, 3=95%
      var status_map = [5, 25, 75, 95]
      battery = 100000 + status_map[self.last_data["battery_status"]]
    end
    
    msg = lwdecode.header(
      self.name ? self.name : format("LHT65-%d", self.node),
      "Dragino LHT65 Temperature & Humidity Sensor",
      battery,
      self.last_battery_update,
      1000,  # No RSSI display
      self.last_update
    )
    
    msg += LwSensorFormatter()
      .start_line()
    
    # Built-in sensors
    if self.last_data.find("temperature") != nil
      msg += LwSensorFormatter()
        .add_sensor("empty", format("%.1f", self.last_data["temperature"]), "Temperature", "🌡️")
        .add_sensor("string", "°C", nil, nil)
        .get_msg()
    end
    
    if self.last_data.find("humidity") != nil
      msg += LwSensorFormatter()
        .add_sensor("empty", format(" %.1f", self.last_data["humidity"]), "Humidity", "💧")
        .add_sensor("string", "%", nil, nil)
        .get_msg()
    end
    
    # External sensor display
    var ext_type = self.last_data.find("ext_sensor", 0)
    
    if ext_type == 0x01 || ext_type == 0x03 || ext_type == 0x09  # Temperature sensors
      if self.last_data.find("ext_temperature") != nil
        msg += LwSensorFormatter()
          .add_sensor("empty", format(" %.1f", self.last_data["ext_temperature"]), "External Temp", "🌡️")
          .add_sensor("string", "°C", nil, nil)
          .get_msg()
      elif self.last_data.find("ext_temp_error") != nil
        msg += LwSensorFormatter()
          .add_sensor("string", " No probe", "External Temp", "⚠️")
          .get_msg()
      end
      
    elif ext_type == 0x04  # Interrupt
      var connected = self.last_data.find("cable_connected", false)
      if connected
        var status = self.last_data.find("interrupt_status", false)
        var icon = status ? "🔴" : "🟢"
        var text = status ? "Triggered" : "Normal"
        msg += LwSensorFormatter()
          .add_sensor("string", " " + text, "Interrupt", icon)
          .get_msg()
      else
        msg += LwSensorFormatter()
          .add_sensor("string", " Disconnected", "Interrupt", "⚠️")
          .get_msg()
      end
      
    elif ext_type == 0x05  # Illumination
      var connected = self.last_data.find("cable_connected", false)
      if connected && self.last_data.find("illumination") != nil
        msg += LwSensorFormatter()
          .add_sensor("empty", format(" %d", self.last_data["illumination"]), "Light", "💡")
          .add_sensor("string", "lux", nil, nil)
          .get_msg()
      else
        msg += LwSensorFormatter()
          .add_sensor("string", " Disconnected", "Light", "⚠️")
          .get_msg()
      end
      
    elif ext_type == 0x06  # ADC
      var connected = self.last_data.find("cable_connected", false)
      if connected && self.last_data.find("adc_v") != nil
        msg += LwSensorFormatter()
          .add_sensor("empty", format(" %.3f", self.last_data["adc_v"]), "ADC", "⚡")
          .add_sensor("string", "V", nil, nil)
          .get_msg()
      else
        msg += LwSensorFormatter()
          .add_sensor("string", " Disconnected", "ADC", "⚠️")
          .get_msg()
      end
      
    elif ext_type == 0x07  # Counter 16-bit
      var connected = self.last_data.find("cable_connected", false)
      if connected && self.last_data.find("count_16") != nil
        msg += LwSensorFormatter()
          .add_sensor("empty", format(" %d", self.last_data["count_16"]), "Counter", "🔢")
          .get_msg()
      else
        msg += LwSensorFormatter()
          .add_sensor("string", " Disconnected", "Counter", "⚠️")
          .get_msg()
      end
      
    elif ext_type == 0x08  # Counter 32-bit
      var connected = self.last_data.find("cable_connected", false)
      if connected && self.last_data.find("count_32") != nil
        msg += LwSensorFormatter()
          .add_sensor("empty", format(" %d", self.last_data["count_32"]), "Counter", "🔢")
          .get_msg()
      else
        msg += LwSensorFormatter()
          .add_sensor("string", " Disconnected", "Counter", "⚠️")
          .get_msg()
      end
    end
    
    # Show timestamp if available
    if self.last_data.find("timestamp") != nil
      msg += LwSensorFormatter()
        .add_sensor("string", format(" %d", self.last_data["timestamp"]), "Unix Time", "🕐")
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
LwDeco = LwDecode_LHT65()