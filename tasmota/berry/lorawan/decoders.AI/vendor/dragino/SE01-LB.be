# Dragino SE01-LB/LS Soil Moisture & EC Sensor Driver
# Driver Version: 3.0.0
# Device Support: SE01-LB (Battery), SE01-LS (Solar)
# Protocol: LoRaWAN
#
# Revision History:
# v1.0.0 - Original implementation
# v2.0.0 - Complete rewrite with enhanced emoji display, full feature support
# v3.0.0 - Regenerated with latest LwDecode framework
#        - Optimized parsing and memory usage
#        - Enhanced error handling and validation
#        - Improved UI display with consistent emoji usage

class LwDecode_SE01LB
  var hashCheck
  var name
  var node
  var last_data
  var last_update
  var last_battery_update
  
  def init()
    self.hashCheck = false  # Process all messages
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
    
    # FPort 2: Sensor Data
    if fport == 2
      # Standard payload (11 or 15 bytes)
      if payload.size() == 11 || payload.size() == 15
        # Battery voltage (bytes 0-1)
        var battery_raw = (payload[0] << 8) | payload[1]
        data["battery_mv"] = battery_raw & 0x3FFF  # Remove upper 2 bits
        data["battery_v"] = data["battery_mv"] / 1000.0
        self.last_battery_update = tasmota.rtc('local')
        
        # DS18B20 external temperature (bytes 2-3)
        var ds18b20_raw = (payload[2] << 8) | payload[3]
        if ds18b20_raw == 0x0CCC
          data["ds18b20_error"] = "Not connected"
        else
          if ds18b20_raw & 0x8000
            ds18b20_raw -= 0x10000  # Handle negative
          end
          data["ds18b20_temp"] = ds18b20_raw / 10.0
        end
        
        # Mode & Flags (byte 10)
        var mode_flags = payload[10]
        data["work_mode"] = (mode_flags >> 7) & 0x01
        data["count_mode"] = (mode_flags >> 6) & 0x01
        data["sensor_flag"] = (mode_flags >> 4) & 0x01
        data["interrupt_flag"] = mode_flags & 0x0F
        
        if data["work_mode"] == 0  # Default calibrated mode
          # Soil moisture (bytes 4-5)
          data["soil_moisture"] = ((payload[4] << 8) | payload[5]) / 100.0
          
          # Soil temperature (bytes 6-7)
          var soil_temp_raw = (payload[6] << 8) | payload[7]
          if soil_temp_raw & 0x8000
            soil_temp_raw -= 0x10000
          end
          data["soil_temperature"] = soil_temp_raw / 100.0
          
          # Soil conductivity (bytes 8-9)
          data["soil_conductivity"] = (payload[8] << 8) | payload[9]
          
        else  # Raw mode
          # Raw conductivity (bytes 4-5)
          data["raw_conductivity"] = (payload[4] << 8) | payload[5]
          
          # Raw moisture (bytes 6-7)
          data["raw_moisture"] = (payload[6] << 8) | payload[7]
          
          # Dielectric constant (bytes 8-9)
          data["dielectric"] = ((payload[8] << 8) | payload[9]) / 10.0
        end
        
        # Counter value if in counting mode (bytes 11-14)
        if payload.size() == 15 && data["count_mode"] == 1
          data["counter"] = payload[11] | (payload[12] << 8) | (payload[13] << 16) | (payload[14] << 24)
        end
      end
      
    # FPort 3: Datalog
    elif fport == 3
      if payload.size() >= 11
        # Parse datalog entry based on work mode
        var work_mode = (payload[6] >> 7) & 0x01
        
        if work_mode == 0  # Default mode datalog
          # Moisture (bytes 0-1)
          data["datalog_moisture"] = ((payload[0] << 8) | payload[1]) / 100.0
          
          # Temperature (bytes 2-3)
          var temp_raw = (payload[2] << 8) | payload[3]
          if temp_raw & 0x8000
            temp_raw -= 0x10000
          end
          data["datalog_temperature"] = temp_raw / 100.0
          
          # Conductivity (bytes 4-5)
          data["datalog_conductivity"] = (payload[4] << 8) | payload[5]
          
        else  # Raw mode datalog
          # Dielectric (bytes 0-1)
          data["datalog_dielectric"] = ((payload[0] << 8) | payload[1]) / 10.0
          
          # Raw moisture (bytes 2-3)
          data["datalog_raw_moisture"] = (payload[2] << 8) | payload[3]
          
          # Raw conductivity (bytes 4-5)
          data["datalog_raw_conductivity"] = (payload[4] << 8) | payload[5]
        end
        
        # Mode & PA8 (byte 6)
        data["work_mode"] = work_mode
        data["poll_message"] = (payload[6] & 0x01) == 0x01
        
        # Unix timestamp (bytes 7-10)
        data["timestamp"] = payload[7] | (payload[8] << 8) | (payload[9] << 16) | (payload[10] << 24)
      end
      
    # FPort 5: Status
    elif fport == 5 && payload.size() == 7
      data["sensor_model"] = format("0x%02X", payload[0])
      data["firmware_version"] = format("%d.%d.%d", payload[1], payload[2] >> 4, payload[2] & 0x0F)
      
      # Frequency band
      var freq_bands = ["EU868", "US915", "IN865", "AU915", "KZ865", "RU864", 
                        "AS923", "AS923-1", "AS923-2", "AS923-3", "CN470", 
                        "EU433", "KR920", "MA869"]
      if payload[3] >= 1 && payload[3] <= 14
        data["frequency_band"] = freq_bands[payload[3] - 1]
      end
      
      data["sub_band"] = payload[4]
      data["battery_mv"] = (payload[5] << 8) | payload[6]
      data["battery_v"] = data["battery_mv"] / 1000.0
      self.last_battery_update = tasmota.rtc('local')
    end
    
    # Store data and update timestamp
    self.last_data = data
    self.last_update = tasmota.rtc('local')
    
    return data
  end
  
  def add_web_sensor()
    if !self.last_data || self.last_update == 0 return nil end
    
    var msg = LwSensorFormatter()
    
    # Header with battery info
    var battery = 1000  # Default no battery
    if self.last_data.find("battery_v") != nil
      battery = self.last_data["battery_v"]
    end
    
    msg = lwdecode.header(
      format("SE01-%d", self.node),
      "Dragino SE01-LB/LS Soil Sensor",
      battery,
      self.last_battery_update,
      1000,  # No RSSI display
      self.last_update
    )
    
    msg += LwSensorFormatter()
      .start_line()
    
    # Display based on work mode
    var work_mode = self.last_data.find("work_mode", 0)
    
    if work_mode == 0  # Default calibrated mode
      # Soil moisture
      if self.last_data.find("soil_moisture") != nil
        msg += LwSensorFormatter()
          .add_sensor("empty", format("%.1f", self.last_data["soil_moisture"]), "Soil Moisture", "💧")
          .add_sensor("string", "%", nil, nil)
          .get_msg()
      elif self.last_data.find("datalog_moisture") != nil
        msg += LwSensorFormatter()
          .add_sensor("empty", format("%.1f", self.last_data["datalog_moisture"]), "Moisture (Log)", "💧")
          .add_sensor("string", "%", nil, nil)
          .get_msg()
      end
      
      # Soil temperature
      if self.last_data.find("soil_temperature") != nil
        msg += LwSensorFormatter()
          .add_sensor("empty", format(" %.1f", self.last_data["soil_temperature"]), "Soil Temp", "🌡️")
          .add_sensor("string", "°C", nil, nil)
          .get_msg()
      elif self.last_data.find("datalog_temperature") != nil
        msg += LwSensorFormatter()
          .add_sensor("empty", format(" %.1f", self.last_data["datalog_temperature"]), "Temp (Log)", "🌡️")
          .add_sensor("string", "°C", nil, nil)
          .get_msg()
      end
      
      # Soil conductivity
      if self.last_data.find("soil_conductivity") != nil
        msg += LwSensorFormatter()
          .add_sensor("empty", format(" %d", self.last_data["soil_conductivity"]), "EC", "🧪")
          .add_sensor("string", "µS/cm", nil, nil)
          .get_msg()
      elif self.last_data.find("datalog_conductivity") != nil
        msg += LwSensorFormatter()
          .add_sensor("empty", format(" %d", self.last_data["datalog_conductivity"]), "EC (Log)", "🧪")
          .add_sensor("string", "µS/cm", nil, nil)
          .get_msg()
      end
      
    else  # Raw mode
      # Dielectric constant
      if self.last_data.find("dielectric") != nil
        msg += LwSensorFormatter()
          .add_sensor("empty", format("κ %.1f", self.last_data["dielectric"]), "Dielectric", "⚗️")
          .get_msg()
      elif self.last_data.find("datalog_dielectric") != nil
        msg += LwSensorFormatter()
          .add_sensor("empty", format("κ %.1f", self.last_data["datalog_dielectric"]), "Dielectric (Log)", "⚗️")
          .get_msg()
      end
      
      # Raw moisture
      if self.last_data.find("raw_moisture") != nil
        msg += LwSensorFormatter()
          .add_sensor("empty", format(" %d", self.last_data["raw_moisture"]), "Raw Moisture", "💧")
          .get_msg()
      elif self.last_data.find("datalog_raw_moisture") != nil
        msg += LwSensorFormatter()
          .add_sensor("empty", format(" %d", self.last_data["datalog_raw_moisture"]), "Moisture (Log)", "💧")
          .get_msg()
      end
      
      # Raw conductivity
      if self.last_data.find("raw_conductivity") != nil
        msg += LwSensorFormatter()
          .add_sensor("empty", format(" %d", self.last_data["raw_conductivity"]), "Raw EC", "🧪")
          .get_msg()
      elif self.last_data.find("datalog_raw_conductivity") != nil
        msg += LwSensorFormatter()
          .add_sensor("empty", format(" %d", self.last_data["datalog_raw_conductivity"]), "EC (Log)", "🧪")
          .get_msg()
      end
    end
    
    # DS18B20 external temperature
    if self.last_data.find("ds18b20_temp") != nil
      msg += LwSensorFormatter()
        .add_sensor("empty", format(" %.1f", self.last_data["ds18b20_temp"]), "External", "🌡️")
        .add_sensor("string", "°C", nil, nil)
        .get_msg()
    elif self.last_data.find("ds18b20_error") != nil
      msg += LwSensorFormatter()
        .add_sensor("string", " No probe", "External", "⚠️")
        .get_msg()
    end
    
    # Counter (if in counting mode)
    if self.last_data.find("count_mode") == 1 && self.last_data.find("counter") != nil
      msg += LwSensorFormatter()
        .add_sensor("empty", format(" %d", self.last_data["counter"]), "Counter", "🔢")
        .get_msg()
    end
    
    # Interrupt flag
    if self.last_data.find("interrupt_flag") == 1
      msg += LwSensorFormatter()
        .add_sensor("string", " Triggered", "Interrupt", "🔴")
        .get_msg()
    end
    
    # Sensor detection
    if self.last_data.find("sensor_flag") == 0
      msg += LwSensorFormatter()
        .add_sensor("string", " No sensor", "Status", "⚠️")
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
LwDeco = LwDecode_SE01LB()