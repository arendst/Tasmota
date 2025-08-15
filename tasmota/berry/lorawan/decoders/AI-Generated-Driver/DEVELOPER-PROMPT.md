# LoRaWAN Decoder AI Generation Template
## Version: 2.1.6 | Framework: LwDecode | Platform: Tasmota Berry

---

## 🎯 OBJECTIVE
Generate production-ready Berry driver for LoRaWAN sensors from manufacturer PDF specifications, ensuring 100% uplink/downlink coverage and optimal performance on ESP32 devices.

---

## 📋 PRE-GENERATION CHECKLIST

### Input Requirements
```yaml
required_files:
  - PDF specification (manufacturer datasheet)
  - BERRY_LANGUAGE_REFERENCE.md (loaded)
  - BERRY_TASMOTA.md (loaded)
  - emoji-reference.md (for UI symbols)

environment_paths:
  base: C:\Project\tasmota\Tasmota
  decoders: ${base}/tasmota/berry/lorawan/decoders/
  output: ${decoders}/AI-Generated-Driver/
  vendor: ${output}/vendor/${vendor_lowercase}/
```

### PDF Analysis Tasks
- [ ] Check for existing [MODEL]-MAP.md file first
- [ ] If MAP file exists, use cached data instead of PDF
- [ ] Extract vendor name and model
- [ ] Identify ALL uplink types and port numbers
- [ ] Document payload structures and byte ordering
- [ ] List all channel types and IDs
- [ ] Note CRC/checksum requirements
- [ ] Catalog all measurement units
- [ ] Identify configuration parameters
- [ ] Find reset/status indicators
- [ ] Extract ALL downlink command types
- [ ] Document downlink payload formats
- [ ] List configuration parameters that can be changed
- [ ] Identify control commands (on/off, reset, etc.)
- [ ] Note acknowledgment/confirmation uplinks
- [ ] Generate [MODEL]-MAP.md with extracted data

---

## ⚠️ CRITICAL REQUIREMENTS

### 1. COMPLETE UPLINK/DOWNLINK COVERAGE
```
MANDATORY: Every uplink/downlink type in PDF MUST be implemented
- No skipping documented channels
- No "unused" comments for known data
- All metadata fields required
- All configuration states captured
- Reset events must be tracked
- Device info must be preserved
- All downlink commands must have Tasmota commands
- Acknowledgment uplinks must be decoded
```

### 2. BERRY LANGUAGE CONSTRAINTS
```berry
# FORBIDDEN variable names (Berry reserved words):
type, class, import, def, end, var, return, if, elif, else
for, while, break, continue, true, false, nil, as, do, try
except, raise, static, assert, global, lambda

# REQUIRED naming patterns:
✅ channel_type, dev_type, msg_type, sensor_type
✅ dev_class, node_class, data_class
✅ config_import, var_name, def_name
❌ type, class, import (bare reserved words)

# Framework usage:
✅ var fmt = LwSensorFormatter_cls()  # Correct - instantiate formatter
✅ lwdecode.SendDownlink(...)         # Correct - use module function
❌ var fmt = LwSensorFormatter()      # Wrong - class doesn't exist
❌ def LwSensorFormatter()             # Wrong - don't wrap framework
❌ SendDownlink(...)                   # Wrong - must use lwdecode prefix
```

### 3. MEMORY CONSTRAINTS
```yaml
esp32_limits:
  flash: 4MB
  ram: 320KB
  berry_stack: 256 levels
  
optimization_rules:
  - Reuse objects across iterations
  - Minimize string concatenation
  - Use integer math over floating point
  - Pre-calculate constants
  - Clear unused references immediately
```

---

## 🏗️ DRIVER STRUCTURE

### Class Template
```berry
# -------------------------------------------------------------
# Originally Prompted by: [AUTHOR]
#
# LoRaWAN AI-Generated Decoder for [VENDOR] [MODEL]
#
# Generated: [DATE] | Version: [VERSION] | Revision: [REV]
#            by "[PROMPT_BRIEF_TITLE]", v[PROMPT_VERSION]
#
# Official Links
# - Homepage:  [MODEL_HOMEPAGE_LINK]
# - Userguide: [MODEL_USERGUIDE_LINK]
# - Decoder:   [MODEL_OFFICIALDECODER_LINK]
# -------------------------------------------------------------
# [CHANGELOG]
# -------------------------------------------------------------

class LwDecode_[MODEL]
    var hashCheck       # Duplicate payload detection flag (true = skip duplicates)
    var crcCheck        # CRC validation flag (if required by specs)
    var name           # Device name from LoRaWAN
    var node           # Node identifier
    var last_data      # Cached decoded data
    var last_update    # Timestamp of last update
    
    def init()
        self.hashCheck = true   # Enable duplicate detection by default
        self.crcCheck = false   # Set true only if PDF requires CRC validation
        self.name = nil
        self.node = nil
        self.last_data = {}
        self.last_update = 0
        
        # Initialize global node storage (survives decoder reload)
        import global
        if !global.contains("[MODEL]_nodes")
            global.[MODEL]_nodes = {}
        end
        if !global.contains("[MODEL]_cmdInit")
            global.[MODEL]_cmdInit = false
        end
    end
    
    def decodeUplink(name, node, rssi, fport, payload)
        import string
        import global
        var data = {}
        
        # Validate inputs
        if payload == nil || size(payload) < 1
            return nil
        end
        
        try
            # Store device info
            self.name = name
            self.node = node
            data['rssi'] = rssi
            data['fport'] = fport
            
            # Retrieve node history from global storage
            var node_data = global.[MODEL]_nodes.find(node, {})
            
            # Decode based on fport (if device uses port-based protocol)
            if fport == [PORT_NUMBER]
                # Implementation for each uplink type
                [DECODE_LOGIC]
            end
            
            # Update node history in global storage
            node_data['last_data'] = data
            node_data['last_update'] = tasmota.rtc()['local']
            node_data['name'] = name
            
            # Store battery trend if available
            if data.contains('battery_v')
                if !node_data.contains('battery_history')
                    node_data['battery_history'] = []
                end
                # Keep last 10 battery readings
                node_data['battery_history'].push(data['battery_v'])
                if size(node_data['battery_history']) > 10
                    node_data['battery_history'].pop(0)
                end
            end
            
            # Store reset count if detected
            if data.contains('device_reset') && data['device_reset']
                node_data['reset_count'] = node_data.find('reset_count', 0) + 1
                node_data['last_reset'] = tasmota.rtc()['local']
            end
            
            # Implement downlinks if present and create relative tasmota commands
            if !global.contains("[MODEL]_cmdInit") || !global.[MODEL]_cmdInit
                self.register_downlink_commands()
                global.[MODEL]_cmdInit = true
            end

            # Save back to global storage
            global.[MODEL]_nodes[node] = node_data
            
            # Update instance cache
            self.last_data = data
            self.last_update = node_data['last_update']
            
            return data
            
        except .. as e, m
            print(f"LwDecode_[MODEL] error: {m}")
            return nil
        end
    end
    
    def add_web_sensor()
        import global
        
        # Try to use current instance data first
        var data_to_show = self.last_data
        var last_update = self.last_update
        
        # If no instance data, try to recover from global storage
        if size(data_to_show) == 0 && self.node != nil
            var node_data = global.[MODEL]_nodes.find(self.node, {})
            data_to_show = node_data.find('last_data', {})
            last_update = node_data.find('last_update', 0)
        end
        
        if size(data_to_show) == 0 return nil end
        
        import string
        var msg = ""
        var fmt = LwSensorFormatter_cls()
        
        # MANDATORY: Add header line with device info
        var name = self.name
        if name == nil || name == ""
            name = f"[MODEL]-{self.node}"
        end
        var name_tooltip = "[VENDOR] [MODEL]"
        var battery = data_to_show.find('battery_v', 1000)  # Use 1000 if no battery
        var battery_last_seen = last_update
        var rssi = data_to_show.find('rssi', 1000)  # Use 1000 if no RSSI
        
        msg += lwdecode.header(name, name_tooltip, battery, battery_last_seen, rssi, last_update)
        
        # Build display using emoji formatter
        fmt.start_line()
        [DISPLAY_LOGIC]
        fmt.end_line()
        msg += fmt.get_msg()
        
        # Add last seen info if data is old
        if last_update > 0
            var age = tasmota.rtc()['local'] - last_update
            if age > 3600  # Data older than 1 hour
                fmt.start_line()
                fmt.add_status(self.format_age(age), "⏱️", nil)
                fmt.end_line()
                var age_msg = fmt.get_msg()
                if age_msg != nil
                    msg = msg + age_msg
                end
            end
        end
        
        return msg
    end
    
    def format_age(seconds)
        if seconds < 60 return f"{seconds}s ago"
        elif seconds < 3600 return f"{seconds/60}m ago"
        elif seconds < 86400 return f"{seconds/3600}h ago"
        else return f"{seconds/86400}d ago"
        end
    end
    
    # Get node statistics
    def get_node_stats(node_id)
        import global
        var node_data = global.[MODEL]_nodes.find(node_id, nil)
        if node_data == nil return nil end
        
        return {
            'last_update': node_data.find('last_update', 0),
            'reset_count': node_data.find('reset_count', 0),
            'last_reset': node_data.find('last_reset', 0),
            'battery_history': node_data.find('battery_history', []),
            'name': node_data.find('name', 'Unknown')
        }
    end
    
    # Clear node data (for maintenance)
    def clear_node_data(node_id)
        import global
        if global.[MODEL]_nodes.contains(node_id)
            global.[MODEL]_nodes.remove(node_id)
            return true
        end
        return false
    end
    
    # Register downlink commands for device control
    def register_downlink_commands()
        import string
        
        # [MANDATORY: Implement ALL downlink commands from PDF]
        # Each downlink type must have a corresponding Tasmota command
        # Command format: [MODEL]<Function><node_index> <parameters>
        # use the SendDownlinkMap helper function when possible like ON|OFF|0|1 , LOW|MID|HIGH, ENABLED|DISABLE|0|1 
        # use SendDownlink for other use case
        
        # Example: Basic control command (ON/OFF) using SendDownlinkMap
        tasmota.remove_cmd("[MODEL]Control")
        tasmota.add_cmd("[MODEL]Control", def(cmd, idx, payload_str)
            # Format: [MODEL]Control<node> <on|off|1|0>
            # SendDownlinkMap handles validation and node lookup
            return lwdecode.SendDownlinkMap(global.[MODEL]_nodes, cmd, idx, payload_str, { 
                '1|ON':  ['08FF', 'ON' ],     # Maps "1" or "ON" to hex 08FF then return result "ON"
                '0|OFF': ['0800', 'OFF']      # Maps "0" or "OFF" to hex 0800 then return result "OFF"
            })
        end)
        
        # Example: Configuration parameter with range validation
        tasmota.remove_cmd("[MODEL]SetParam")
        tasmota.add_cmd("[MODEL]SetParam", def(cmd, idx, payload_str)
            # Format: [MODEL]SetParam<node> <value>
            var value = int(payload_str)
            if value < [MIN] || value > [MAX]
                return tasmota.resp_cmnd_str(f"Invalid: range {[MIN]}-{[MAX]}")
            end
            
            # Build hex command per PDF specification
            var hex_cmd = f"[PREFIX]{value & 0xFF:02X}{(value >> 8) & 0xFF:02X}"
            return lwdecode.SendDownlink(global.[MODEL]_nodes, cmd, idx, hex_cmd)
        end)
        
        # Example: Multi-parameter command with validation
        tasmota.remove_cmd("[MODEL]Config")
        tasmota.add_cmd("[MODEL]Config", def(cmd, idx, payload_str)
            # Format: [MODEL]Config<node> <param1>,<param2>
            var parts = string.split(payload_str, ',')
            if size(parts) != 2
                return tasmota.resp_cmnd_str("Usage: [MODEL]Config<node> <param1>,<param2>")
            end
            
            var param1 = int(parts[0])
            var param2 = int(parts[1])
            
            # Validate parameters
            if param1 < 0 || param1 > 255
                return tasmota.resp_cmnd_str("Invalid param1: range 0-255")
            end
            
            # Build hex command using framework helpers
            var hex_cmd = f"FE{param1:02X}{param2:02X}"
            return lwdecode.SendDownlink(global.[MODEL]_nodes, cmd, idx, hex_cmd)
        end)
        
        # [Continue implementing ALL downlink types from PDF]
        # Common patterns:
        # - Device reset/reboot
        # - Status request
        # - Threshold settings
        # - Enable/disable features
        # - Calibration commands
        # - Time synchronization
        
        print("[MODEL]: Downlink commands registered")
    end
    
    # Helper to build complex downlink payloads (optional)
    # Use only when lwdecode.SendDownlink/SendDownlinkMap aren't sufficient
    def build_downlink_payload(cmd_type, params)
        import string
        var hex_payload = ""
        
        # Example: Complex multi-field payload
        if cmd_type == "complex_config"
            # Build using framework helpers when available:
            # lwdecode.uint16le(value) - 16-bit little endian
            # lwdecode.uint32le(value) - 32-bit little endian
            # lwdecode.uint16be(value) - 16-bit big endian
            hex_payload = f"FF{params['cmd']:02X}"
            hex_payload += lwdecode.uint16le(params['interval'])
            hex_payload += lwdecode.uint32le(params['threshold'])
        end
        
        return hex_payload
    end
end

# Global instance
LwDeco = LwDecode_[MODEL]()

# Test command registration (recreated on each load)
tasmota.remove_cmd("[MODEL]TestPayload")
tasmota.add_cmd("[MODEL]TestPayload", def(cmd, idx, payload_str)
    # Parse hex string to bytes
    var test_payload = bytes(payload_str)
    
    # Force driver load by LwDecode framework
    var result = LwDeco.decodeUplink("Test[MODEL]", "test_node", -85, idx, test_payload)
    
    if result != nil
        import json
        tasmota.resp_cmnd(json.dump(result))
    else
        tasmota.resp_cmnd_error()
    end
end)

# Node management commands
tasmota.remove_cmd("[MODEL]NodeStats")
tasmota.add_cmd("[MODEL]NodeStats", def(cmd, idx, node_id)
    var stats = LwDeco.get_node_stats(node_id)
    if stats != nil
        import json
        tasmota.resp_cmnd(json.dump(stats))
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

tasmota.remove_cmd("[MODEL]ClearNode")
tasmota.add_cmd("[MODEL]ClearNode", def(cmd, idx, node_id)
    if LwDeco.clear_node_data(node_id)
        tasmota.resp_cmnd_done()
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)
```

---

## 🏛️ GLOBAL NODE STORAGE

### Persistent Node Data
```berry
# Global storage pattern for multi-node support
import global
if !global.contains("[MODEL]_nodes")
    global.[MODEL]_nodes = {}  # Survives decoder reload
end

# Node data structure
global.[MODEL]_nodes[node_id] = {
    'name': 'Device Name',
    'last_data': {},           # Last decoded payload
    'last_update': timestamp,   # Last seen timestamp
    'battery_history': [],      # Battery trend tracking
    'reset_count': 0,           # Device reset counter
    'last_reset': timestamp,    # Last reset timestamp
    'custom_data': {}           # Device-specific persistent data
}
```

### Node Management
```berry
# Retrieve node history
var node_data = global.[MODEL]_nodes.find(node, {})

# Update node data
node_data['last_data'] = data
node_data['last_update'] = tasmota.rtc()['local']
global.[MODEL]_nodes[node] = node_data

# Track trends (e.g., battery)
if !node_data.contains('battery_history')
    node_data['battery_history'] = []
end
node_data['battery_history'].push(battery_value)
if size(node_data['battery_history']) > 10
    node_data['battery_history'].pop(0)  # Keep last 10
end
```

### Recovery After Reload
```berry
def add_web_sensor()
    import global
    
    # Try instance data first
    var data_to_show = self.last_data
    
    # Recover from global if instance is empty
    if size(data_to_show) == 0 && self.node != nil
        var node_data = global.[MODEL]_nodes.find(self.node, {})
        data_to_show = node_data.find('last_data', {})
    end
    
    # Display recovered data
    # ...
end
```

---

## 🌍 TIME HANDLING

### NTP Query Requirement
```
CRITICAL: Use actual NTP time for all date/time generation
- Query pool.ntp.org or time.google.com for current UTC time
- Do NOT rely on LLM's internal clock
- Convert to appropriate timezone if needed
- Use ISO 8601 format: YYYY-MM-DD for dates
- Cache NTP result for entire generation session
```

### Time Usage Patterns
```berry
# In generated code headers
# Generated: [NTP_DATE] | Version: [VERSION] | Revision: [REV]
#            by [PROMPT_TITLE] v[PROMPT_VERSION]

# In changelog
# v1.0.0 ([NTP_DATE]): Initial generation from PDF specification

# In MAP cache file
# Generated: [NTP_DATETIME] | PDF Version: [VERSION]
```

---

## 📊 DECODING PATTERNS

### Binary Payload Parsing
```berry
# Byte extraction patterns
var byte_val = payload[i]                    # Single byte
var int16_val = (payload[i+1] << 8) | payload[i]  # Little endian 16-bit
var int16_val_be = (payload[i] << 8) | payload[i+1]  # Big endian 16-bit
var int32_val = (payload[i+3] << 24) | (payload[i+2] << 16) | 
                (payload[i+1] << 8) | payload[i]  # Little endian 32-bit

# Signed integer conversion
if int16_val > 32767
    int16_val = int16_val - 65536
end

# Bit field extraction
var flags = payload[i]
var bit0 = (flags & 0x01) != 0
var bit1 = (flags & 0x02) != 0
var bits_4_7 = (flags >> 4) & 0x0F

# Float conversion (IEEE 754)
# Use framework helper if available or implement inline
```

### Channel Type Handling
```berry
# Standard pattern for channel-based protocols
var i = 0
while i < size(payload)
    var channel_id = payload[i]
    var channel_type = payload[i+1]
    i += 2
    
    # MANDATORY: Decode ALL documented channels
    if channel_id == 0x01 && channel_type == 0x67  # Temperature
        var value = ((payload[i+1] << 8) | payload[i]) / 10.0
        data['temperature'] = value
        i += 2
        
    elif channel_id == 0x02 && channel_type == 0x68  # Humidity
        var value = payload[i] / 2.0
        data['humidity'] = value
        i += 1
        
    # Continue for ALL channels in PDF
    else
        # Log unknown but don't skip
        print(f"Unknown channel: ID={channel_id:02X} Type={channel_type:02X}")
        # Attempt to determine size or break
        break
    end
end
```

### Configuration and Status
```berry
# Always capture device configuration
if channel_type == 0xFF  # Device info channel
    data['fw_version'] = f"{payload[i]}.{payload[i+1]}.{payload[i+2]}"
    data['hw_version'] = f"{payload[i+3]}.{payload[i+4]}"
    data['serial_number'] = string.format("%08X", 
        (payload[i+5]<<24) | (payload[i+6]<<16) | (payload[i+7]<<8) | payload[i+8])
    i += 9
end

# Track reset events
if payload[0] & 0x80
    data['device_reset'] = true
    data['reset_reason'] = payload[1]
end

# Battery status (common pattern)
if channel_type == 0x01  # Battery
    var voltage = ((payload[i+1] << 8) | payload[i]) / 1000.0
    data['battery_v'] = voltage
    data['battery_pct'] = self.voltage_to_percent(voltage)
    i += 2
end
```

---

## 🎨 DISPLAY FORMATTING

### MANDATORY Header Implementation
```berry
def add_web_sensor()
    if size(self.last_data) == 0 return nil end
    
    import string
    var msg = ""
    var fmt = LwSensorFormatter_cls()
    
    # MANDATORY: Header line with device info
    var name = self.name
    if name == nil || name == ""
        name = f"[MODEL]-{self.node}"  # Fallback name format
    end
    var name_tooltip = "[VENDOR] [MODEL]"  # Device description
    
    # Battery: use actual voltage (mV) or 1000 to hide
    var battery = self.last_data.find('battery_v', 1000)
    var battery_last_seen = self.last_update
    
    # RSSI: use actual value or 1000 to hide
    var rssi = self.last_data.find('rssi', 1000)
    
    # Add header (REQUIRED FOR ALL DRIVERS)
    msg += lwdecode.header(name, name_tooltip, battery, battery_last_seen, rssi, self.last_update)
    
    # Then add sensor lines
    fmt.start_line()
    # ... sensor display logic
    fmt.end_line()
    msg += fmt.get_msg()
    
    return msg
end
```

### Emoji Formatter Usage
```berry
def add_web_sensor()
    if size(self.last_data) == 0 return nil end
    
    var msg = ""
    var fmt = LwSensorFormatter_cls()
    
    # MANDATORY: Add header first
    msg = lwdecode.header(name, tooltip, battery, battery_ls, rssi, last_seen)
    
    # Single line format (preferred) - NO CHAINING
    fmt.start_line()
    fmt.add_sensor("volt", self.last_data.find('battery_v'), "Battery", "🔋")
    fmt.add_sensor("string", self.last_data.find('temperature'), "Temp", "🌡️")
    fmt.add_sensor("milliamp", self.last_data.find('current'), "Current", "🔌")
    fmt.add_sensor("power", self.last_data.find('power'), "Power", "💡")
    # For multi-line: use next_line() instead of end_line() + start_line()
    fmt.next_line()  # Preferred for continuing to next line
    fmt.add_sensor("altitude", self.last_data.find('altitude'), "Alt", "⛰")
    fmt.add_status("OK", "✅", "Device operational")
    fmt.end_line()
    msg = msg + fmt.get_msg()  # Get string AFTER building
    
    # Only use start_line() for separate blocks (e.g., alerts)
    if self.last_data.find('device_reset')
        fmt.start_line()  # New separate block
        fmt.add_status("Reset detected", "⚠️", nil)
        fmt.end_line()
        var alert_msg = fmt.get_msg()
        if alert_msg != nil
            msg = msg + alert_msg
        end
    end
    
    return msg
end
```
```

### Available Formatters
```berry
# From LwSensorFormatter_cls.Formatter
"string"         # Text/status display (no unit)
"volt"           # Voltage with V unit
"milliamp"       # Current with mA unit
"power_factor%"  # Power factor with % unit
"power"          # Power with W unit
"energy"         # Energy with Wh unit
"altitude"       # Altitude with mt unit
"empty"          # No display

# Usage: fmt.add_sensor(formatter, value, tooltip, icon)
# For status text: fmt.add_sensor("string", "ON", nil, "🟢")
```

### ⚠️ CRITICAL: Formatter Chain Usage

The LwSensorFormatter_cls uses method chaining. NEVER concatenate with += operator during chain building:

```berry
# ❌ WRONG - Will cause type_error
var msg = ""
msg += fmt.start_line()  # ERROR: Can't concatenate string with instance

# ❌ WRONG - Chaining across multiple lines with dots
fmt.start_line()
    .add_sensor("temp", value, "°C", "🌡️")  # This can fail
    .end_line()

# ✅ CORRECT - Each method on separate line WITHOUT dots
fmt.start_line()
fmt.add_sensor("temp", value, "°C", "🌡️")
fmt.end_line()
var msg = fmt.get_msg()  # Get string AFTER building

# ✅ CORRECT - For multiple blocks
fmt.start_line()
fmt.add_sensor("temp", value, "°C", "🌡️")
fmt.end_line()
var msg = fmt.get_msg()

# Add second block if needed
fmt.start_line()
fmt.add_status("Alert", "⚠️")
fmt.end_line()
msg = msg + fmt.get_msg()  # Use explicit concatenation, not +=
```

### Important Rules for Formatter:
1. **NO chaining with dots** - Call each method separately
2. **NO += operator** - Use `msg = msg + fmt.get_msg()` instead
3. **Call methods individually** - `fmt.start_line()` then `fmt.add_sensor()` then `fmt.end_line()`
4. **Get string at end** - Only call `get_msg()` after all formatting
5. **Test for nil** - Always check if msg is nil before concatenation
6. **Use next_line()** - Prefer `fmt.next_line()` over `fmt.end_line()` + `fmt.start_line()` for multi-line display
7. **Header is mandatory** - Always call `lwdecode.header()` before sensor lines

### Emoji Selection Rules
```yaml
priority_order:
  1. Check emoji-reference.md for existing mapping
  2. Use standard IoT emojis if applicable
  3. Select visually representative emoji
  4. Update reference with justification

common_mappings:
  temperature: 🌡️
  humidity: 💧
  battery: 🔋
  rssi/signal: 📶
  pressure: 🔵
  co2: 💨
  light: 💡
  motion: 🚶
  door: 🚪
  water: 💦
  power: ⚡
  alert: ⚠️
  error: ❌
  ok: ✅
```

---

## 🧪 VALIDATION PATTERNS

### Payload Validation
```berry
# Size validation
if size(payload) < expected_min_size
    print(f"Payload too small: {size(payload)} < {expected_min_size}")
    return nil
end

# CRC validation (only if required by device specification)
if self.crcCheck
    var calculated_crc = self.calculate_crc(payload, 0, size(payload)-2)
    var received_crc = (payload[-1] << 8) | payload[-2]
    if calculated_crc != received_crc
        print(f"CRC mismatch: calc={calculated_crc:04X} recv={received_crc:04X}")
        return nil
    end
end

# Range validation
if temperature < -40 || temperature > 125
    print(f"Temperature out of range: {temperature}")
    # Still store but mark as invalid
    data['temperature'] = temperature
    data['temperature_valid'] = false
end
```

### Helper Functions
```berry
# Battery voltage to percentage (common pattern)
def voltage_to_percent(voltage)
    # Typical LiPo/Li-ion curve
    if voltage >= 4.2 return 100
    elif voltage <= 3.0 return 0
    else
        # Linear approximation
        return int((voltage - 3.0) / 1.2 * 100)
    end
end

# CRC calculation (if needed)
def calculate_crc(data, start, length)
    var crc = 0xFFFF
    for i: start..(start + length - 1)
        crc ^= data[i]
        for j: 0..7
            if crc & 0x0001
                crc = (crc >> 1) ^ 0xA001
            else
                crc = crc >> 1
            end
        end
    end
    return crc
end

# Timestamp formatting
def format_timestamp(ts)
    if ts == 0 return "Never" end
    var now = tasmota.rtc()['local']
    var diff = now - ts
    if diff < 60 return f"{diff}s ago"
    elif diff < 3600 return f"{diff/60}m ago"
    elif diff < 86400 return f"{diff/3600}h ago"
    else return f"{diff/86400}d ago"
    end
end
```

---

## 📝 DOCUMENTATION TEMPLATE

### MAP Cache File ([MODEL]-MAP.md)
```yaml
# [VENDOR] [MODEL] Protocol MAP
# Generated: [NTP_DATETIME] | PDF Version: [VERSION]
# Source: [PDF_FILENAME]

device_info:
  vendor: [VENDOR]
  model: [MODEL]
  type: [SENSOR_TYPE]
  lorawan_version: [VERSION]
  regions: [EU868, US915, etc]
  official_link: [URL]

uplinks:
  - port: 1
    type: 0x01
    name: "Periodic Data"
    channels:
      - id: 0x01
        type: 0x67
        name: "Temperature"
        size: 2
        unit: "°C"
        resolution: 0.1
        range: [-40, 125]
      - id: 0x02
        type: 0x68
        name: "Humidity"
        size: 1
        unit: "%"
        resolution: 0.5
        range: [0, 100]

downlinks:
  - command: "Socket Control"
    hex_prefix: "08"
    parameters:
      - name: "state"
        type: "boolean"
        values:
          on: "FF"
          off: "00"
  - command: "Set Interval"
    hex_prefix: "FE02"
    parameters:
      - name: "minutes"
        type: "uint16"
        range: [1, 65535]
        endian: "little"

special_handling:
  crc_required: false
  reset_events: true
  battery_monitoring: true
  signed_values:
    - "active_power"
  
measurement_units:
  temperature: "°C"
  humidity: "%"
  voltage: "V"
  current: "mA"
  power: "W"
  energy: "Wh"
```

### Driver Documentation (MODEL.md)
```markdown
# [VENDOR] [MODEL] LoRaWAN Decoder

## Device Information
- **Manufacturer**: [VENDOR]
- **Model**: [MODEL]
- **Type**: [SENSOR_TYPE]
- **LoRaWAN Version**: [VERSION]
- **Region**: [EU868/US915/etc]
- **Official Reference**: [MODEL_LINKS]

## Implementation Details
- **Driver Version**: [VERSION]
- **Generated**: [DATE]
- **Coverage**: [X]/[Y] uplinks implemented, [X]/[Y] downlinks implemented
- **Average Decode Time**: [XX]ms
- **Memory Usage**: [XX] bytes

## Uplink Coverage Matrix
| Port | Type | Description | Status | Notes |
|------|------|-------------|--------|-------|
| 1 | 0x01 | Periodic Data | ✅ Implemented | All channels decoded |
| 2 | 0x02 | Alert Message | ✅ Implemented | Includes threshold data |
| 3 | 0xFF | Configuration | ✅ Implemented | Device info and settings |

## Decoded Parameters
| Parameter | Unit | Range | Notes |
|-----------|------|-------|-------|
| temperature | °C | -40 to 125 | ±0.1°C accuracy |
| humidity | % | 0 to 100 | ±2% accuracy |
| battery_v | V | 0 to 5 | Lithium battery voltage |
| rssi | dBm | -120 to 0 | LoRaWAN signal strength |

## Downlink Commands

| Command | Description | Usage | Downlink Hex |
|---------|-------------|-------|---------------|
| [MODEL]Control | Basic control | `[MODEL]Control1 on/off` | `[HEX]` |
| [MODEL]SetParam | Set parameter | `[MODEL]SetParam1 <value>` | `[HEX]` |
| [MODEL]Config | Configuration | `[MODEL]Config1 <p1> <p2>` | `[HEX]` |
| [MODEL]Reset | Device reset | `[MODEL]Reset1` | `[HEX]` |
| [MODEL]Status | Request status | `[MODEL]Status1` | `[HEX]` |

### Downlink Usage Examples

```
# Control device on node 1
[MODEL]Control1 on

# Set parameter to 60 for node 2
[MODEL]SetParam2 60

# Configure device on node 1
[MODEL]Config1 param1 param2

# Request status from node 3
[MODEL]Status3

# Reset device on node 1
[MODEL]Reset1
```

Note: The node index in the command (e.g., `1` in `[MODEL]Control1`) corresponds to the LoRaWAN node to send the downlink to.

## Testing

### Test Payload Examples

#### Direct Berry Testing
\`\`\`berry
# Test periodic data uplink
var test_payload = bytes("01670110026850FF01020304050607080900")
var result = LwDeco.decodeUplink("TestDevice", "node123", -85, 1, test_payload)
print(json.dump(result))
# Expected: {"temperature": 27.2, "humidity": 40.0, ...}

# Test alert uplink
var alert_payload = bytes("020101670120")
var result = LwDeco.decodeUplink("TestDevice", "node123", -85, 2, alert_payload)
print(json.dump(result))
# Expected: {"alert": true, "temperature": 28.8}
\`\`\`

#### Tasmota Console Commands
\`\`\`
# Test periodic data (on framework LwDecode driver #1)
[MODEL]TestPayload1 01670110026850FF01020304050607080900

# Test alert message (on framework LwDecode driver #2)
[MODEL]TestPayload2 020101670120

# Test configuration data (on framework LwDecode driver #3)
[MODEL]TestPayload3 FF01020304050607080900

# Test battery status (on framework LwDecode driver #4)
[MODEL]TestPayload4 01010FA0

# Test reset event (#5)
[MODEL]TestPayload5 8001

# Test multi-channel data (#1)
[MODEL]TestPayload1 01670110026850036901000467AABB

# Test error conditions
[MODEL]TestPayload1 ""              # Empty payload
[MODEL]TestPayload1 01               # Incomplete payload
[MODEL]TestPayload1 FFFFFFFFFF       # Invalid data

# Node management
[MODEL]NodeStats test_node           # Get node statistics
[MODEL]NodeStats node_12345          # Get specific node stats
[MODEL]ClearNode test_node           # Clear node data
[MODEL]ClearNode node_12345          # Clear specific node

# Downlink commands
[MODEL]Control1 on                   # Turn on device 1
[MODEL]Control1 off                  # Turn off device 1
[MODEL]SetParam2 100                 # Set parameter for device 2
\`\`\`

#### Expected Responses
\`\`\`json
// Port 1 - Periodic data response
{
  "rssi": -85,
  "fport": 1,
  "temperature": 27.2,
  "humidity": 40.0,
  "fw_version": "1.2.3",
  "hw_version": "4.5",
  "serial_number": "06070809"
}

// Port 2 - Alert response
{
  "rssi": -85,
  "fport": 2,
  "alert": true,
  "alert_type": 1,
  "temperature": 28.8
}

// Port 3 - Configuration response  
{
  "rssi": -85,
  "fport": 3,
  "config_version": 1,
  "report_interval": 600,
  "threshold_temp_high": 35.0,
  "threshold_temp_low": 5.0
}

// Node stats response
{
  "last_update": 1699123456,
  "reset_count": 2,
  "last_reset": 1699100000,
  "battery_history": [4.1, 4.0, 3.9, 3.9, 3.8],
  "name": "Test[MODEL]"
}
\`\`\`

### Integration Example
\`\`\`berry
# Add to autoexec.be
load("[MODEL].be")

# The driver auto-registers as LwDeco
# Web UI will automatically show sensor data
# Test command [MODEL]TestPayload is available in console
# Downlink commands [MODEL]* are available in console
\`\`\`

### Testing Workflow
1. Load the driver: `load("[MODEL].be")`
2. Test with command: `[MODEL]TestPayload1 YOUR_HEX_PAYLOAD`
3. Check response in console for decoded JSON
4. Verify Web UI shows formatted sensor data
5. Test all documented uplink types using different port indices
6. Test downlink commands: `[MODEL]Control1 on`

## Performance Metrics
- Decode Time: [X]ms average, [Y]ms max
- Memory Allocation: [X] bytes per decode
- Stack Usage: [X]/256 levels

## Generation Notes
- Generated from: [PDF_FILENAME]
- Generation prompt: AI Template v2.1.3
- Special considerations: [Any device-specific notes]

## Versioning Strategy

- v<major>.<minor>.<fix>
```
<major> increase only when the official sensor specs change from the vendor, starting from 1
<minor> increase only when fresh regeneration is requested, reset to zero when major change
<fix> increase on all other cases, reset to 0 when minor change 
```
- All the date of publish must greater then 2025-01-13 (day of the framework start) 

## Changelog
- v1.0.0 ([DATE]): Initial generation from PDF specification
```

---

## 🚀 GENERATION WORKFLOW

### Phase 1: Analysis (Silent)
1. Check for existing [MODEL]-MAP.md cache file
2. If MAP exists, load cached specifications
3. Otherwise, parse PDF for all technical specifications
4. Create uplink type inventory
5. Map channel IDs to parameters
6. Identify special handling requirements
7. Extract ALL downlink commands
8. Document downlink payload formats
9. Check emoji-reference.md for existing mappings
10. Generate or update [MODEL]-MAP.md cache file

### Phase 2: Generation (Silent)
1. Query NTP for accurate current datetime (don't trust LLM time)
2. Generate driver code with 100% uplink coverage
3. Implement ALL downlink commands from PDF
4. Add test command registration with auto-cleanup
5. Register downlink commands with validation
6. Implement all helper functions needed
7. Create comprehensive documentation
8. Generate test payload examples for ALL uplink types
9. Document ALL downlink commands with examples
10. Create Tasmota command examples for each payload type
11. Update emoji-reference.md if needed
12. Update `GENERATED-DRIVER-LIST.md` when needed

### Phase 3: Validation (Silent)
1. Verify no Berry reserved words used
2. Check framework usage correctness
3. Validate all uplinks decoded
4. Validate all downlinks implemented
5. Confirm memory optimizations
6. Test error handling paths
7. Verify command parameter validation
8. Check downlink hex payload accuracy

### Phase 4: Output (Visible)
```markdown
## ✅ Completed
**Driver**: vendor/[vendor]/[MODEL].be
**Documentation**: vendor/[vendor]/[MODEL].md
**MAP Cache**: vendor/[vendor]/[MODEL]-MAP.md
**Emoji Reference**: [updated|no changes needed]
**Uplink Coverage**: [X]/[Y] uplinks implemented
**Downlink Coverage**: [X]/[Y] downlinks implemented
**Performance**: [X]ms average decode time
**Validation**: All checks passed
```

---

## 🔍 QUALITY CHECKLIST

### Code Quality
- [ ] No Berry reserved words as variables
- [ ] LwSensorFormatter_cls() used correctly
- [ ] All channels from PDF decoded
- [ ] Error handling with try/except
- [ ] Memory optimizations applied
- [ ] Comments explain complex logic
- [ ] hashCheck property correctly set (duplicate detection)
- [ ] crcCheck property only if PDF requires it
- [ ] Test command properly registered with cleanup
- [ ] Global node storage initialized
- [ ] Node data persistence implemented
- [ ] Recovery after reload works

### Downlink Implementation
- [ ] All PDF-documented downlinks have Tasmota commands
- [ ] Command names follow pattern [MODEL]<Function>
- [ ] Proper parameter validation and error messages
- [ ] Hex payload generation matches PDF specification
- [ ] Commands registered in register_downlink_commands()
- [ ] Downlink documentation in .md file
- [ ] Command usage examples provided
- [ ] Node index handling implemented (cmd<idx> pattern)
- [ ] Acknowledgment uplinks properly decoded

### Functional Completeness
- [ ] 100% uplink type coverage
- [ ] 100% downlink command coverage
- [ ] All metadata fields captured
- [ ] Configuration states tracked
- [ ] Reset events detected
- [ ] Battery monitoring included
- [ ] RSSI always reported
- [ ] Multi-node support via global storage
- [ ] Historical data tracking (battery, resets)
- [ ] Node statistics accessible

### UI/UX
- [ ] Single line display (preferred)
- [ ] Emojis for all values
- [ ] No text labels
- [ ] Status indicators for alerts
- [ ] Timestamp for last update
- [ ] Clean, readable format
- [ ] Formatter chains built correctly (no += during chain)

### Documentation
- [ ] MAP cache file generated/updated
- [ ] Complete uplink matrix
- [ ] Complete downlink command table
- [ ] Test examples provided for ALL uplink types
- [ ] Tasmota console commands for each payload
- [ ] Downlink command usage examples
- [ ] Expected JSON responses documented
- [ ] Integration guide included
- [ ] Performance metrics documented
- [ ] Changelog maintained with NTP date
- [ ] PDF source referenced

---

## ⚠️ PROPERTY CLARIFICATION

### hashCheck Property
```berry
# CORRECT USAGE - For duplicate payload detection
self.hashCheck = true   # Framework will skip duplicate payloads
self.hashCheck = false  # Framework will process all payloads

# This is NOT for CRC validation!
# The framework uses this to avoid processing the same payload twice
```

### crcCheck Property (Custom)
```berry
# Only add if PDF specifies CRC validation
self.crcCheck = true    # Driver will validate CRC
# Then implement CRC validation in decodeUplink()
if self.crcCheck
    # Your CRC validation code here
end
```

## ❌ COMMON PITFALLS TO AVOID

1. **Using reserved words**: Never use `type`, `class`, etc. as variables
2. **Skipping channels**: Every documented channel must be decoded
3. **Wrong formatter**: Use `LwSensorFormatter_cls()` not `LwSensorFormatter()`
4. **Helper duplication**: Don't recreate framework functions
5. **Memory leaks**: Clear unused references immediately
6. **Missing validation**: Always check payload size and ranges
7. **Incomplete error handling**: Use try/except blocks
8. **Text in UI**: Use emojis only, no text labels
9. **Multi-line abuse**: Keep to single line unless necessary
10. **Documentation gaps**: Include all test examples
11. **Forgetting global storage**: Always initialize global.[MODEL]_nodes
12. **Not handling reload**: Implement recovery from global storage
13. **Losing node history**: Store important trends in global
14. **Missing downlinks**: Every PDF-documented downlink must have a command
15. **Wrong command format**: Use [MODEL]<Function><node> pattern
16. **No parameter validation**: Always validate inputs and ranges
17. **Missing downlink docs**: Document all commands in .md file
18. **Formatter chain errors**: Never use += with formatter chains, only with get_msg() result

---

## 📚 REFERENCE LINKS

- Berry Language: BERRY_LANGUAGE_REFERENCE.md
- Tasmota Integration: BERRY_TASMOTA.md
- Emoji Mappings: emoji-reference.md
- Framework Docs: lwdecode/README.md
- Driver Examples: AI-Generated-Driver/vendor/

---

## 🎓 FRAMEWORK HELPERS

### Available Framework Functions
```berry
# Display formatting
LwSensorFormatter_cls()     # Display formatter class

# Downlink helpers (from lwdecode module)
lwdecode.SendDownlink(nodes, cmd, idx, hex_payload)
    # Send raw hex downlink to node
    # Returns: tasmota response

lwdecode.SendDownlinkMap(nodes, cmd, idx, input, mapping)
    # Send mapped downlink (e.g., ON/OFF -> hex)
    # mapping format: {'1|ON': ['08FF', 'ON'], '0|OFF': ['0800', 'OFF']}
    # Returns: tasmota response

# Integer encoding (returns hex string)
lwdecode.uint16le(value)    # 16-bit little endian
lwdecode.uint32le(value)    # 32-bit little endian
lwdecode.uint16be(value)    # 16-bit big endian
lwdecode.uint32be(value)    # 32-bit big endian
lwdecode.int16le(value)     # Signed 16-bit little endian
lwdecode.int32le(value)     # Signed 32-bit little endian

# Float encoding
lwdecode.float32le(value)   # IEEE 754 float little endian
lwdecode.float32be(value)   # IEEE 754 float big endian

# BCD encoding
lwdecode.bcd_encode(value)   # Integer to BCD hex string
lwdecode.bcd_decode(hex_str) # BCD hex string to integer

# CRC/validation
lwdecode.crc16_modbus(data)  # Calculate Modbus CRC16
lwdecode.crc8(data)          # Calculate CRC8

# Bit manipulation
lwdecode.pack_bits(bit_array)  # Convert bit array to bytes
lwdecode.unpack_bits(bytes)    # Extract bits from bytes
```

### Testing Commands
```berry
# In Tasmota console - Using test command
[MODEL]TestPayload1 YOUR_HEX_PAYLOAD_PORT1
[MODEL]TestPayload2 YOUR_HEX_PAYLOAD_PORT2
[MODEL]TestPayload3 YOUR_HEX_PAYLOAD_PORT3
# Port number in command matches fport parameter

# Downlink commands
[MODEL]Control1 on           # Control device 1
[MODEL]SetParam2 100         # Set parameter for device 2
[MODEL]Config3 p1 p2         # Configure device 3

# Node management commands
[MODEL]NodeStats node_id     # Get statistics for a node
[MODEL]ClearNode node_id     # Clear data for a node

# In Berry console - Direct testing
import lorawan/decoders/vendor/[vendor]/[MODEL]
var test = bytes("YOUR_HEX_PAYLOAD")
var result = LwDeco.decodeUplink("Test", "node", -85, 1, test)
print(json.dump(result))

# Check global storage
import global
print(global.[MODEL]_nodes)
```

---

## 📋 FINAL NOTES

This template ensures:
1. **Complete decoder/encoder implementation** - No uplink/downlink data is lost
2. **Berry compliance** - No syntax errors or reserved word conflicts
3. **Framework integration** - Proper use of LwDecode helpers
4. **Optimal performance** - Memory and CPU efficient
5. **Professional output** - Production-ready code with documentation

Remember: The goal is a **perfect, complete decoder** that handles **100% of the device's capabilities** as documented in the manufacturer's PDF, including ALL uplink decoding and ALL downlink command generation.

---
*Template Version: 2.1.6 | Last Updated: 2025-01-17*

---

*Author: [ZioFabry](https://github.com/ZioFabry)*