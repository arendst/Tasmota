# AI Powered LwDecode Framework for LoRaWAN Device Decoders

## Overview

LwDecode is a AI Powered Berry framework for Tasmota ESP32 that provides a standardized interface for decoding LoRaWAN sensor payloads. It integrates with Tasmota's LoRaWAN bridge functionality to decode device uplinks, display sensor data in the web UI, and manage downlink commands.

## Architecture

### Core Components

1. **LwDecode.be** - Main framework providing:
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

3. **LwSensorFormatter** - UI formatting helper providing:
   - Standardized sensor display with icons
   - Tooltips and units support
   - Multi-line sensor display
   - Battery and RSSI indicators
   - Status text display with `add_status()` method

## Framework API Reference

### Required Decoder Structure

```berry
class LwDecode_<MODEL>
  var hashCheck       # Duplicate detection (default: true)
  var name           # Device name
  var node           # Node identifier
  var last_data      # Cached decoded data
  var last_update    # Last update timestamp
  
  def init()
    self.hashCheck = true
    self.name = nil
    self.node = nil
    self.last_data = {}
    self.last_update = 0
    
    # Initialize global node storage
    import global
    if !global.contains("<MODEL>_nodes")
      global.<MODEL>_nodes = {}
    end
  end
  
  def decodeUplink(name, node, rssi, fport, payload)
    var data = {}
    # Decode logic here
    self.name = name
    self.node = node
    self.last_data = data
    return data
  end
  
  def add_web_sensor()
    if size(self.last_data) == 0 return nil end
    
    var msg = ""
    var fmt = LwSensorFormatter_cls()
    
    # MANDATORY: Add header
    var name = self.name ? self.name : f"<MODEL>-{self.node}"
    msg = lwdecode.header(name, "Device Description", 
                         self.last_data.find('battery_v', 1000),
                         self.last_update,
                         self.last_data.find('rssi', 1000),
                         self.last_update)
    
    # Add sensor data
    fmt.start_line()
    fmt.add_sensor("volt", value, "tooltip", "🔋")
    fmt.next_line()  # Use for multi-line
    fmt.add_sensor("power", value, nil, "⚡")
    fmt.end_line()
    msg = msg + fmt.get_msg()
    
    return msg
  end
end

# Required: Set decoder instance
LwDeco = LwDecode_<MODEL>()
```

### Decoder Method Details

#### decodeUplink(name, node, rssi, fport, payload)

**Parameters:**
- `name` (string): Device name from LoRaWanName configuration
- `node` (int): Node number (1-128)
- `rssi` (int): Signal strength in dBm
- `fport` (int): LoRaWAN port number
- `payload` (bytes): Raw payload data

**Returns:** Map containing decoded data

**Example:**
```berry
def decodeUplink(name, node, rssi, fport, payload)
  import global
  var data = {}
  
  # Store device info
  self.name = name
  self.node = node
  data['rssi'] = rssi
  data['fport'] = fport
  
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
- MUST include header via `lwdecode.header()`
- Use `LwSensorFormatter_cls()` for sensor display
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
  
  var msg = ""
  var fmt = LwSensorFormatter_cls()
  
  # Header (MANDATORY)
  var name = self.name ? self.name : f"<MODEL>-{self.node}"
  msg = lwdecode.header(name, "Device Model", 
                       data_to_show.find('battery_v', 1000),
                       last_update,
                       data_to_show.find('rssi', 1000),
                       last_update)
  
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
2. Use `msg = msg + fmt.get_msg()` not `+=`
3. Call methods individually without dots
4. Prefer `next_line()` for multi-line displays
5. Always add header before sensor lines

### Helper Functions

#### lwdecode.header(name, tooltip, battery, battery_ls, rssi, last_seen)
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

#### uint16le(value) / uint32le(value)
Convert integers to little-endian hex strings

```berry
lwdecode.uint16le(value)  # Returns 2-byte hex string
lwdecode.uint32le(value)  # Returns 4-byte hex string
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
4. Reuse formatter instances
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
- `<MODEL>TestPayload<port> <hex>` - Test decoder
- `<MODEL>NodeStats <node>` - Get node statistics
- `<MODEL>ClearNode <node>` - Clear node data

## Debugging

### Enable Debug Logging
```
WebLog 4
```

### Common Issues
1. **Decoder not loading**: Check file path and syntax
2. **No web display**: Verify header is added first
3. **No MQTT**: Check SetOption83/89/100
4. **Downlinks failing**: Verify node registry

### Testing
```berry
# Test in Berry console
load("vendor/vendor/model.be")
var result = LwDeco.decodeUplink("Test", 1, -50, 1, bytes("0102"))
print(result)

# Test via Tasmota console
<MODEL>TestPayload1 0102030405
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
   - Commands: `<MODEL><Function><node>`

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

## License

Part of Tasmota project - See LICENSE file in repository root