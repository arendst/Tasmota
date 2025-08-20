# AI Powered LwDecode Framework for LoRaWAN Device Decoders

## Overview

LwDecode is a AI Powered Berry framework for Tasmota ESP32 that provides a standardized interface for decoding LoRaWAN sensor payloads. It integrates with Tasmota's LoRaWAN bridge functionality to decode device uplinks, display sensor data in the web UI, and manage downlink commands.

**Enhanced Error Handling**: v2.2.8 introduces comprehensive error handling with stack traces, retry mechanisms, and detailed logging for improved debugging and reliability.

## Architecture

### Core Components

1. **LwDecode.be** (v2.2.8) - Main framework providing:
   - Payload decoding infrastructure
   - Web UI integration with caching
   - MQTT message publishing
   - Downlink command management
   - Dynamic decoder loading and reloading
   - Multi-node support (up to 128 devices)

2. **Device Decoders** - Individual `.be` files implementing sensor-specific logic:
   - Located in `/vendor/<vendor>/<model>.be`
   - Must implement `LwDecode_<MODEL>` class
   - Required methods: `decodeUplink()` and `add_web_sensor()`

3. **LwSensorFormatter_cls** - UI formatting helper integrated in LwDecode.be providing:
   - Standardized sensor display with icons
   - Tooltips and units support
   - Multi-line sensor display
   - Battery and RSSI indicators
   - Status text display with `add_status()` method

## Framework API Reference

### Required Decoder Structure

```berry
class LwDecode_<MODEL>
  var hashCheck      # Duplicate payload detection flag (true = skip duplicates)
  var name           # Device name from LoRaWAN
  var node           # Node identifier
  var last_data      # Cached decoded data
  var last_update    # Timestamp of last update
  var lwdecode       # global instance of the driver
  
  def init()
    self.hashCheck = true   # Enable duplicate detection by default
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
  
  def decodeUplink(name, node, rssi, fport, payload, simulated)
    import string
    import global
    var data = {}
    
    if payload == nil || size(payload) < 1
        return nil
    end
    
    try
      # Store device info
      self.name = name
      self.node = node
      data['rssi'] = rssi
      data['fport'] = fport
      data['simulated'] = simulated
      
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

    except .. as e, m
      node_data['ex'] = e
      node_data['exmsg'] = m
      log(f"[MODEL]: Decode error - {e}: {m}", 1)

    end

    return data
  end
  
  def add_web_sensor()
    import global

    var data_to_show = self.last_data
    var last_update = self.last_update
    
    if size(data_to_show) == 0 && self.node != nil
        var node_data = global.[MODEL]_nodes.find(self.node, {})
        data_to_show = node_data.find('last_data', {})
        last_update = node_data.find('last_update', 0)
    end
    
    if size(data_to_show) == 0 
        return nil 
    end
    
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
    
    var simulated = data_to_show.find('simulated', false) # Simulated payload indicator
    
    # Build display using emoji formatter
    fmt.header(name, name_tooltip, battery, battery_last_seen, rssi, last_update, simulated)
    
    fmt.start_line()

    if data_to_show.contains('socket_state')
      var state_emoji = data_to_show['socket_state'] ? "🟢" : "⚫"
      var state_text = data_to_show['socket_state'] ? "ON" : "OFF"
      fmt.add_sensor("string", state_text, "Socket State", state_emoji)
    end

    if data_to_show.contains('voltage')
      fmt.add_sensor("volt", data_to_show['voltage'], "Voltage", "⚡")
    end

    if data_to_show.contains('current')
      fmt.add_sensor("milliamp", data_to_show['current'], "Current", "🔌")
    end

    if data_to_show.contains('active_power')
      fmt.add_sensor("power", data_to_show['active_power'], "Power", "💡")
    end

    fmt.next_line()

    if data_to_show.contains('energy')
      fmt.add_sensor("energy", data_to_show['energy'], "Energy", "🏠")
    end
    if data_to_show.contains('power_factor')
      fmt.add_sensor("power_factor%", data_to_show['power_factor'], "Power Factor", "📊")
    end
    fmt.end_line()
    
    msg += fmt.get_msg()
    return msg
  end
end

# Required: Set decoder instance
LwDeco = LwDecode_<MODEL>()
```

### Decoder Method Details

#### decodeUplink(name, node, rssi, fport, payload, simulated)

**Parameters:**
- `name` (string): Device name from LoRaWanName configuration
- `node` (int): Node number (1-128)
- `rssi` (int): Signal strength in dBm
- `fport` (int): LoRaWAN port number
- `payload` (bytes): Raw payload data
- `simulated` (bool): Simulated flag indicator

**Returns:** Map containing decoded data

**Example:**
```berry
def decodeUplink(name, node, rssi, fport, payload, simulated)
  import global
  var data = {}
  
  # Store device info
  self.name = name
  self.node = node
  data['rssi'] = rssi
  data['fport'] = fport
  data['simulated'] = simulated
  
  # Decode based on fport
  if fport == 1 && size(payload) >= 4
    data["temperature"] = ((payload[0] | payload[1] << 8) - 32768) / 100.0
    data["humidity"] = (payload[2] | payload[3] << 8) / 100.0
  end
  
  # Update global storage
  var node_data = global.<MODEL>_nodes.find(node, {})
  node_data['last_data'] = data
  node_data['last_update'] = tasmota.rtc()['local']
  global.<MODEL>_nodes[node] = node_data
  
  self.last_data = data
  self.last_update = node_data['last_update']
  
  return data
end
```

#### add_web_sensor()

**Returns:** HTML string for web UI display

**Requirements:**
- Use `LwSensorFormatter_cls()` for sensor display
- MUST include header via `fmt.header()`
- Handle data recovery from global storage

**Example:**
```berry
def add_web_sensor()
  import global
  
  # Try instance data first
  var data_to_show = self.last_data
  var last_update = self.last_update
  
  # Recover from global if needed
  if size(data_to_show) == 0 && self.node != nil
    var node_data = global.<MODEL>_nodes.find(self.node, {})
    data_to_show = node_data.find('last_data', {})
    last_update = node_data.find('last_update', 0)
  end
  
  if size(data_to_show) == 0 return nil end
  
  var simulated = data_to_show.find('simulated', false) # Simulated payload indicator
  
  var msg = ""
  var fmt = LwSensorFormatter_cls()
  
  # Header (MANDATORY)
  var name = self.name ? self.name : f"<MODEL>-{self.node}"
  var name_tooltip = "Device Model"
  var battery = data_to_show.find('battery_v', 1000)
  var battery_last_seen = last_update
  var rssi = data_to_show.find('rssi', 1000)
  var simulated = data_to_show.find('simulated', false)
  
  fmt.header(name, name_tooltip, battery, battery_last_seen, rssi, last_update, simulated)
  
  # Sensor data
  fmt.start_line()
  fmt.add_sensor("volt", data_to_show['voltage'], "Voltage", "⚡")
  fmt.add_sensor("power", data_to_show['power'], "Power", "💡")
  fmt.next_line()  # Continue to next line
  fmt.add_sensor("energy", data_to_show['energy'], "Energy", "🏠")
  fmt.add_status("OK", "✅", "Device operational")
  fmt.end_line()
  msg = msg + fmt.get_msg()
  
  return msg
end
```

### LwSensorFormatter API

#### Formatters
Built-in formatters with units and icons:
- `"string"`: No unit, no icon (for text display)
- `"volt"`: V unit, ⚡ icon
- `"milliamp"`: mA unit, 🔌 icon
- `"power_factor%"`: % unit, 📊 icon
- `"power"`: W unit, 💡 icon
- `"energy"`: Wh unit, 🧮 icon
- `"altitude"`: mt unit, ⛰ icon
- `"temp"`: °C unit, 🌡️ icon
- `"humidity"`: % unit, 💧 icon
- `"co2"`: ppm unit, 💨 icon
- `"distance"`: m unit, 📏 icon
- `"lux"`: lx unit, ☀️ icon
- `"empty"`: No display

#### Methods
- `start_line()`: Begin new sensor line
- `next_line()`: Continue to next line (preferred over end+start)
- `end_line()`: End current line
- `add_sensor(formatter, value, tooltip, alt_icon)`: Add sensor with formatter
- `add_status(text, icon, tooltip)`: Add status text with icon (v1.7.0+)
- `begin_tooltip(text)` / `end_tooltip()`: Add tooltips
- `add_link(title, url, target)`: Add clickable links
- `get_msg()`: Get final HTML string

**Important Usage Rules:**
1. NO method chaining with dots across lines
2. Use `msg += fmt.get_msg()` as last to retrive the msg, after the the message buffer will be cleared
3. Call methods individually without dots
4. Prefer `next_line()` for multi-line displays
5. Always add header before sensor lines

### Error Handling and Debugging

#### Enhanced Error Logging (v1.8.0+)
The framework provides comprehensive error handling with detailed stack traces:

```berry
# Detailed error logging with stack traces
lwdecode.log_error(context, error, message, payload_info)

# Safe decoder loading with retry mechanism
var decoder = lwdecode.safe_load_decoder("vendor/model.be")

# Protected property access
var value = lwdecode.get_decoder_property(decoder, "hashCheck", true)

# Validated decoding with input/output checks
var result = lwdecode.safe_decode_uplink(decoder, name, node, rssi, fport, payload)
```

#### Error Categories
- **LOAD**: Decoder file loading and validation errors
- **PROPERTY**: Decoder property access failures
- **DECODE**: Payload decoding and validation errors
- **RELOAD**: Dynamic decoder reloading issues

#### Debugging Features
- Stack trace capture for all exceptions
- Retry mechanism for transient failures (3 attempts)
- Input/output validation for all decoder calls
- Detailed context in error messages with source/line of the exceptions
- Performance timing for decoder operations

#### lwdecode.header(name, tooltip, battery, battery_ls, rssi, last_seen, simulated)
Generates standard device header with battery/RSSI indicators

**Parameters:**
- `battery`: mV (0-5000) or percentage (100000-100100), use 1000 to hide
- `battery_ls`: Battery last seen timestamp
- `rssi`: Signal strength or 1000+ to hide
- `last_seen`: Device last seen timestamp

#### lwdecode.dhm(timestamp) / dhm_tt(timestamp)
Format time difference as "02d", "15h", "30m" with optional tooltip

#### Downlink Commands

Register custom commands in decoder:

```berry
def register_downlink_commands()
  import string
  
  # Simple ON/OFF command
  tasmota.remove_cmd("<MODEL>Control")
  tasmota.add_cmd("<MODEL>Control", def(cmd, idx, payload)
    return lwdecode.SendDownlinkMap(
      global.<MODEL>_nodes, cmd, idx, payload,
      {
        '1|ON':  ['08FF', 'ON'],
        '0|OFF': ['0800', 'OFF']
      }
    )
  end)
  
  # Parameter command with validation
  tasmota.remove_cmd("<MODEL>SetParam")
  tasmota.add_cmd("<MODEL>SetParam", def(cmd, idx, payload)
    var value = int(payload)
    if value < 1 || value > 100
      return tasmota.resp_cmnd_str("Invalid: range 1-100")
    end
    var hex_cmd = f"FE02{value:02X}"
    return lwdecode.SendDownlink(global.<MODEL>_nodes, cmd, idx, hex_cmd)
  end)
end
```

### Utility Functions

#### Encoding Functions
Convert values to hex strings for downlink payloads:

```berry
lwdecode.uint16le(value)   # 16-bit little endian
lwdecode.uint32le(value)   # 32-bit little endian  
lwdecode.uint16be(value)   # 16-bit big endian
lwdecode.uint32be(value)   # 32-bit big endian
lwdecode.int16le(value)    # Signed 16-bit little endian
lwdecode.int32le(value)    # Signed 32-bit little endian
lwdecode.float32le(value)  # IEEE 754 float little endian
lwdecode.float32be(value)  # IEEE 754 float big endian
lwdecode.bcd_encode(value) # Binary Coded Decimal
lwdecode.bcd_decode(hex)   # BCD to integer
lwdecode.crc16_modbus(data) # Modbus CRC16
lwdecode.crc8(data)        # CRC8
lwdecode.pack_bits(array)  # Bit array to bytes
lwdecode.unpack_bits(data) # Bytes to bit array
```

## Data Persistence Pattern

Store sensor data in global maps to survive decoder reloads:

```berry
# Initialize in init()
import global
if !global.<MODEL>_nodes
  global.<MODEL>_nodes = {}
end

# Update in decodeUplink()
var node_data = global.<MODEL>_nodes.find(node, {})
node_data['last_data'] = data
node_data['last_update'] = tasmota.rtc()['local']
node_data['name'] = name

# Track history
if !node_data.contains('battery_history')
  node_data['battery_history'] = []
end
node_data['battery_history'].push(battery_value)
if size(node_data['battery_history']) > 10
  node_data['battery_history'].pop(0)
end

global.<MODEL>_nodes[node] = node_data
```

## Memory Optimization

### Best Practices
1. Use global storage sparingly
2. Implement `hashCheck = false` if duplicates expected
3. Clear unused data: `global.<MODEL>_nodes.remove(node)`
4. Reuse formatter instances only after get_msg() or clear()
5. Use compact variable names

### ESP32 Constraints
- Available RAM: ~100-150KB for Berry
- Recommended decoder size: <10KB source
- Global storage per node: <500 bytes
- Web cache timeout: 5 seconds

## Installation

### Manual Installation
1. Upload decoder to `/vendor/<vendor>/<model>.be`
2. Configure in console:
   ```
   LoRaWanAppKey1 <32-char-key>
   LoRaWanDecoder1 vendor/vendor/model.be
   LoRaWanName1 MySensor
   LoRaWanNode1 1
   ```
3. Restart Berry: `BrRestart`

### Web UI Configuration
Navigate to **Configuration → LoRaWAN** to configure nodes

### Console Commands
- `LwReload` - Reload all decoders
- `LwReload <decoder>` - Reload specific decoder
- `Lw<MODEL>TestUI<node> <scenario>` - Test decoder UI
- `Lw<MODEL>NodeStats <node>` - Get node statistics
- `Lw<MODEL>ClearNode <node>` - Clear node data

## Debugging

### Common Issues
1. **Decoder not loading**: Check file path and syntax
2. **No web display**: Verify header is added first
3. **No MQTT**: Check SetOption83/89/100
4. **Downlinks failing**: Verify node registry

### Testing
```berry
# Test in Berry console
load("LwDecode.be")
load("[MODEL].be")
var result = tasmota.cmd("LwSimulate1 -70,85,0102")
import json
print(json.dump(result))

# Test via Tasmota console
LwSimulate1 -70,85,0102
```

## Framework Settings

Automatically configured:
- `LoraOption3 off` - Disable embedded decoding
- `SetOption100 off` - Keep LwReceived in JSON
- `SetOption118 off` - Keep SENSOR as subtopic
- `SetOption119 off` - Keep device address
- `LoRaWanBridge on` - Enable bridge mode

## AI Agent Guidelines

When implementing decoders:

1. **Analyze Specification**
   - Extract ALL uplink/downlink types
   - Document all channels and measurements
   - Note special encoding

2. **Follow Conventions**
   - Class: `LwDecode_<MODEL>`
   - Global: `global.<MODEL>_nodes`
   - Commands: `Lw<MODEL><Function><node> <args_comma_separated>`

3. **Implementation Requirements**
   - 100% channel coverage
   - All downlink commands
   - Mandatory header in UI
   - Use `next_line()` for multi-line
   - Proper error handling

4. **Optimize Display**
   - Use framework formatters
   - Select appropriate emojis
   - Prefer single/dual line display
   - Show critical status first

5. **Document Thoroughly**
   - Complete test examples
   - All command usage
   - Expected responses

## Version History

- v1.0.0 - Initial framework release
- v1.1.0 - Added LwSensorFormatter
- v1.2.0 - Multi-node support
- v1.3.0 - Dynamic decoder reloading
- v1.4.0 - Web UI caching
- v1.5.0 - Downlink command framework
- v1.6.0 - Hash-based duplicate detection
- v1.7.0 - Added `add_status()` method
- v2.0.0 - Major refactoring and improvements
- v2.1.0 - Enhanced error handling and validation
- v2.2.0 - Added simulation support and UI enhancements
- v2.2.8 - Current version with comprehensive helper functions and simulated payload support

## License

Part of Tasmota project - See LICENSE file in repository root