#-------------------------------------------------------------
# LoRaWAN Decoder for Browan EBL-OC
# Generated: 2025-08-14 | Version: 1.0.0 | Revision: 1
#-------------------------------------------------------------
#
# Device: Browan Open/Close Sensor EBL (Hall-Effect door/window sensor with environmental monitoring)
# Product URL: https://www.browan.com/products-detail/OpenClose-Sensor-EBL-LoRaWAN/
# 
# Features:
#   - Hall-Effect magnetic field detection for door/window state
#   - Temperature and humidity environmental monitoring
#   - Vibration detection with sensitivity levels
#   - Tilt detection for tamper alerts
#   - Button for manual interaction
#   - 3-year battery life with low power LoRaWAN
#
# Uplink Payload Format (FPort 120, 9 bytes):
#   Byte[0]: Status Flags (bit-field)
#     - Bit 0: Door/Window state (0=closed, 1=open)
#     - Bit 1: Button press event (0=not pressed, 1=pressed)
#     - Bit 2: Tamper/vibration detection (0=normal, 1=detected)
#     - Bit 3: Tilt detection (0=normal, 1=tilted)
#     - Bits 4-7: Reserved for future use
#   Byte[1]: Battery voltage offset (0-255)
#     - Actual voltage = (21 + value) * 0.1V
#     - Range: 2.1V to 27.6V
#   Byte[2]: Temperature (signed int8, -128 to 127°C)
#   Byte[3]: Humidity (unsigned int8, 0-100 %RH)
#   Byte[4-5]: Last door open duration in minutes (little-endian uint16)
#   Byte[6-8]: Total door open events counter (little-endian uint24)
#
# Display Strategy:
#   - Single-line optimized with emoji-first approach
#   - Priority: Door state → Environment → Security alerts
#   - Smart visibility: Only show alerts when active
#   - Compact counters for event tracking
#
# Emoji Reference (per emoji-reference.md):
#   🔓 (U+1F513): Door/window open
#   🔒 (U+1F512): Door/window closed
#   🌡️ (U+1F321): Temperature measurement
#   💧 (U+1F4A7): Humidity percentage
#   ⚠️ (U+26A0): Tamper/vibration warning
#   📦 (U+1F4E6): Tilt detection
#   🔘 (U+1F518): Button press indicator
#   ⏱️ (U+23F1): Event counter
#
# Changelog:
#   v1.0.0 (2025-08-14): Initial generation from PDF specification

import string

# Global persistent storage for node states (survives decoder reload)
if !global.eblOcNodes
  global.eblOcNodes = {}
end

class LwDecoEBL_OC
  # Decode uplink payload from Browan EBL Open/Close sensor
  # Parameters:
  #   Name: LoRaWAN device name from configuration
  #   Node: Device EUI or node identifier
  #   RSSI: Received signal strength indicator (dBm)
  #   FPort: LoRaWAN port number (120 for sensor data)
  #   Bytes: Raw payload bytes array
  # Returns: Map of decoded sensor values
  static def decodeUplink(Name, Node, RSSI, FPort, Bytes)
    var data = {"Device":"Browan EBL-OC"}
    
    # Initialize state variables with defaults
    var valid_values = false
    var last_seen = 1451602800  # Unix epoch for never-seen
    var battery_last_seen = 1451602800
    var battery = 0.0
    var rssi = RSSI
    var door_open = 0
    var door_open_last_seen = 1451602800
    var button_pressed = 0
    var tamper_detected = 0
    var tilt_detected = 0
    var temperature = 0
    var humidity = 0
    var door_duration_mins = 0
    var door_events_total = 0
    
    # Restore previous door state from persistent storage
    if global.eblOcNodes.find(Node)
      var stored = global.eblOcNodes.item(Node)
      door_open = stored[6]
      door_open_last_seen = stored[7]
    end
    
    # Process sensor data payload (FPort 120, 9 bytes exactly)
    if FPort == 120 && Bytes.size() == 9
      last_seen = tasmota.rtc('local')
      
      # Byte 0: Parse status flags
      var status_byte = Bytes[0]
      
      # Bit 0: Door/window state
      var last_door_open = door_open
      door_open = (status_byte & 0x01) ? 1 : 0
      data.insert("DoorOpen", door_open == 1)
      
      # Track door state changes for time display
      if last_door_open != door_open
        door_open_last_seen = tasmota.rtc('local')
      end
      
      # Bit 1: Button press event
      button_pressed = (status_byte & 0x02) ? 1 : 0
      data.insert("ButtonPress", button_pressed == 1)
      
      # Bit 2: Tamper/vibration detection
      tamper_detected = (status_byte & 0x04) ? 1 : 0
      data.insert("TamperDetect", tamper_detected == 1)
      
      # Bit 3: Tilt detection
      tilt_detected = (status_byte & 0x08) ? 1 : 0
      data.insert("TiltDetect", tilt_detected == 1)
      
      # Byte 1: Battery voltage calculation
      # Formula: (21 + byte_value) * 0.1V for actual voltage
      var battery_raw = Bytes[1]
      battery = (21 + battery_raw) * 0.1  # Direct calculation in volts
      data.insert("BattV", battery)
      battery_last_seen = tasmota.rtc('local')
      
      # Byte 2: Temperature (signed int8, handle negative values)
      temperature = Bytes[2]
      if temperature > 127  # Two's complement for negative temps
        temperature = temperature - 256
      end
      data.insert("TemperatureC", temperature)
      
      # Byte 3: Humidity (0-100% RH)
      humidity = Bytes[3]
      if humidity > 100  # Validate humidity range
        humidity = 100
      end
      data.insert("Humidity", humidity)
      
      # Bytes 4-5: Last door open duration (little-endian uint16, minutes)
      door_duration_mins = Bytes[4] | (Bytes[5] << 8)
      data.insert("DoorOpenLastDuration_mins", door_duration_mins)
      
      # Bytes 6-8: Total door events counter (little-endian uint24)
      door_events_total = Bytes[6] | (Bytes[7] << 8) | (Bytes[8] << 16)
      data.insert("DoorOpenEvents", door_events_total)
      
      valid_values = true
    end
    # Other FPorts are not defined in EBL specification - ignore silently
    
    # Update persistent storage with new sensor state
    if valid_values
      # Remove old entry if exists
      if global.eblOcNodes.find(Node)
        global.eblOcNodes.remove(Node)
      end
      
      # Store sensor state array:
      # [0] Name, [1] Node, [2] last_seen, [3] battery_last_seen, [4] battery,
      # [5] RSSI, [6] door_open, [7] door_open_last_seen, [8] button_pressed,
      # [9] tamper_detected, [10] tilt_detected, [11] temperature, [12] humidity,
      # [13] door_duration_mins, [14] door_events_total
      global.eblOcNodes.insert(Node, [
        Name, Node, last_seen, battery_last_seen, battery,
        RSSI, door_open, door_open_last_seen, button_pressed,
        tamper_detected, tilt_detected, temperature, humidity,
        door_duration_mins, door_events_total
      ])
    end
    
    return data
  end
  
  # Generate HTML for web UI display - ultra-compact single-line format
  static def add_web_sensor()
    var msg = ""
    
    for sensor: global.eblOcNodes
      # Extract stored sensor data
      var name = sensor[0]
      # Use compact naming for EBL devices
      if string.find(name, "EBL") > -1 || string.find(name, "Browan") > -1
        name = string.format("EBL-%i", sensor[1])
      end
      var name_tooltip = "Browan EBL Open/Close"
      var last_seen = sensor[2]
      var battery_last_seen = sensor[3]
      var battery = sensor[4]
      var rssi = sensor[5]
      
      # Standard LwDecode header with device info
      msg += lwdecode.header(name, name_tooltip, battery, battery_last_seen, rssi, last_seen)
      
      # Extract all sensor values
      var door_open = sensor[6]
      var door_open_last_seen = sensor[7]
      var button_pressed = sensor[8]
      var tamper_detected = sensor[9]
      var tilt_detected = sensor[10]
      var temperature = sensor[11]
      var humidity = sensor[12]
      var door_duration_mins = sensor[13]
      var door_events_total = sensor[14]
      
      # Build ultra-compact single-line display
      msg += "<tr class='htr'><td colspan='4'>&#9478;"  # │ vertical line
      
      # Priority 1: Door state with time since change
      if door_open
        msg += " &#x1F513;"  # 🔓 Open lock
      else
        msg += " &#x1F512;"  # 🔒 Closed lock
      end
      msg += string.format(" %s", lwdecode.dhm(door_open_last_seen))
      
      # Priority 2: Environmental sensors (always show)
      msg += string.format(" &#x1F321;&#xFE0F;%.1f°C", temperature)  # 🌡️
      msg += string.format(" &#x1F4A7;%.0f%%", humidity)  # 💧
      
      # Priority 3: Security alerts (conditional display)
      if tamper_detected
        msg += " &#x26A0;&#xFE0F;"  # ⚠️ Tamper warning
      end
      
      if tilt_detected
        msg += " &#x1F4E6;"  # 📦 Tilt indicator
      end
      
      # Priority 4: Interaction indicator (conditional)
      if button_pressed
        msg += " &#x1F518;"  # 🔘 Button pressed
      end
      
      # Priority 5: Compact counter (if events occurred)
      if door_events_total > 0
        # Use compact format for large numbers
        if door_events_total >= 1000
          msg += string.format(" &#x23F1;&#xFE0F;%.1fk", door_events_total / 1000.0)  # ⏱️
        else
          msg += string.format(" &#x23F1;&#xFE0F;%d", door_events_total)  # ⏱️
        end
      end
      
      msg += "{e}"  # End table row
    end
    
    return msg
  end
end

# Export class for LwDecode framework
LwDeco = LwDecoEBL_OC
