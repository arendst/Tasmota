# Dragino SE01-LB LoRaWAN Decoder

## Device Information
- **Manufacturer**: Dragino
- **Model**: SE01-LB/LS
- **Type**: Soil Moisture & EC Sensor
- **LoRaWAN Version**: 1.0.3
- **Region**: EU868/US915/CN470/AS923/AU915/IN865/KR920/EU433
- **Official Reference**: 
  - [Product Page](https://www.dragino.com/products/agriculture-weather-station/item/277-se01-lb.html)
  - [User Manual](https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/SE01-LB_LoRaWAN_Soil%20Moisture%26EC_Sensor_User_Manual/)
  - [Official Decoder](https://github.com/dragino/dragino-end-node-decoder/tree/main/)

## Implementation Details
- **Driver Version**: 1.0.0
- **Generated**: 2025-08-20
- **Coverage**: 28/28 uplinks implemented, 10/10 downlinks implemented
- **Framework**: LwDecode v2.2.9
- **Template**: v2.3.3

## Expected UI Examples

Based on the device capabilities and typical usage scenarios:

### Example 1: Normal Soil Monitoring
```
┌─────────────────────────────────────┐
│ 🌱 SE01LB-slot1  Dragino SE01-LB   │
│ 🔋 3.2V 📶 -78dBm ⏱️ 2m ago        │
├─────────────────────────────────────┤
│ 💧 25.4% 🌡️ 22.5°C ⚡ 3261µS/cm   │
│ 🌡️ 21.2°C ⚙️ Cal                   │
└─────────────────────────────────────┘
```

### Example 2: Dry Soil Alert
```
┌─────────────────────────────────────┐
│ 🌱 SE01LB-slot1  Dragino SE01-LB   │
│ 🔋 3.1V 📶 -85dBm ⏱️ 5m ago        │
├─────────────────────────────────────┤
│ 💧 5.5% 🌡️ 45.7°C ⚡ 2272µS/cm    │
│ 🌡️ 19.9°C ⚙️ Cal                   │
└─────────────────────────────────────┘
```

### Example 3: Wet Soil Conditions
```
┌─────────────────────────────────────┐
│ 🌱 SE01LB-slot1  Dragino SE01-LB   │
│ 🔋 3.2V 📶 -80dBm ⏱️ 1m ago        │
├─────────────────────────────────────┤
│ 💧 82.0% 🌡️ 58.9°C ⚡ 7372µS/cm   │
│ 🌡️ 23.0°C ⚙️ Cal                   │
└─────────────────────────────────────┘
```

### Example 4: Counting Mode
```
┌─────────────────────────────────────┐
│ 🌱 SE01LB-slot1  Dragino SE01-LB   │
│ 🔋 3.2V 📶 -78dBm ⏱️ 30s ago       │
├─────────────────────────────────────┤
│ 💧 25.4% 🌡️ 22.5°C ⚡ 3261µS/cm   │
│ 🌡️ 21.2°C ⚙️ Cal 🔢 1000          │
└─────────────────────────────────────┘
```

### Example 5: Device Status
```
┌─────────────────────────────────────┐
│ 🌱 SE01LB-slot1  Dragino SE01-LB   │
│ 🔋 3.2V 📶 -70dBm ⏱️ 15s ago       │
├─────────────────────────────────────┤
│ 💾 2.0.0 📡 CN470                  │
└─────────────────────────────────────┘
```

### Example 6: External Sensor Issues
```
┌─────────────────────────────────────┐
│ 🌱 SE01LB-slot1  Dragino SE01-LB   │
│ 🔋 3.2V 📶 -78dBm ⏱️ 2m ago        │
├─────────────────────────────────────┤
│ 💧 25.4% 🌡️ 22.5°C ⚡ 3261µS/cm   │
│ ⚠️ DS18B20 Disc                    │
└─────────────────────────────────────┘
```

## Command Reference

**Test Commands** (slot = driver slot 1-16):
| Command | Description | Usage | Example |
|---------|-------------|-------|---------|
| LwSE01LBTestUI<slot> | UI scenarios | `<scenario>` | `LwSE01LBTestUI2 normal` |

**Control Commands** (slot = driver slot 1-16):
| Command | Description | Usage | Downlink Hex |
|---------|-------------|-------|---------------|
| LwSE01LBInterval<slot> | Set interval | `<seconds>` | `01XXXXXX` |
| LwSE01LBReset<slot> | Device reset | (no params) | `04FF` |
| LwSE01LBConfirm<slot> | Confirm mode | `on/off` | `05000001/05000000` |
| LwSE01LBInterrupt<slot> | Interrupt mode | `0/3` | `06000000/06000003` |
| LwSE01LBPower5V<slot> | 5V duration | `<milliseconds>` | `07XXXX` |
| LwSE01LBSetCount<slot> | Set count | `<count>` | `09XXXXXXXX` |
| LwSE01LBMode<slot> | Working mode | `default/raw` | `0A00/0A01` |
| LwSE01LBCountMode<slot> | Count mode | `interrupt/counting` | `1000/1001` |
| LwSE01LBStatus<slot> | Request status | (no params) | `2601` |
| LwSE01LBPollLog<slot> | Poll datalog | `<start>,<end>,<int>` | `31XXXXXXXXXXXXXXXXXX` |

## Usage Examples

### Driver in Slot 2:
```bash
# Test realistic scenarios 
LwSE01LBTestUI2 normal       # Normal soil conditions
LwSE01LBTestUI2 dry          # Dry soil alert  
LwSE01LBTestUI2 wet          # Wet soil conditions
LwSE01LBTestUI2 lowbatt      # Low battery warning
LwSE01LBTestUI2 counting     # Counting mode
LwSE01LBTestUI2 raw          # Raw mode values
LwSE01LBTestUI2 status       # Device status
LwSE01LBTestUI2 disconn      # Sensor disconnected

# Control device (sends to all nodes managed by this driver instance)
LwSE01LBInterval2 1800       # Set 30-minute interval
LwSE01LBMode2 default        # Set to calibrated mode
LwSE01LBCountMode2 counting  # Enable counting mode
LwSE01LBSetCount2 1000       # Start counting from 1000
LwSE01LBStatus2              # Request device status

# Node-specific management
LwSE01LBNodeStats SE01LB-2   # Get stats for node
LwSE01LBClearNode SE01LB-2   # Clear data for node
```

## Testing

### Expected Responses
```json
// Port 2 - Normal soil data (MOD=0, interrupt mode)
{
  "rssi": -75,
  "fport": 2,
  "ds18b20_temperature": 21.2,
  "soil_moisture": 25.36,
  "soil_temperature": 22.54,
  "soil_conductivity": 3261,
  "battery_voltage": 3247,
  "battery_v": 3.247,
  "working_mode": 0,
  "interrupt_triggered": false,
  "sensor_identified": true
}

// Port 5 - Device status
{
  "rssi": -75,
  "fport": 5,
  "sensor_model": 38,
  "firmware_version": "2.0.0",
  "frequency_band": "CN470",
  "sub_band": 0,
  "battery_voltage": 3247,
  "battery_v": 3.247
}
```

## Integration Example
```berry
# Add to autoexec.be
load("LwDecode.be")
load("SE01-LB.be")

# The driver auto-registers as LwDeco
# Web UI will automatically show sensor data
# Test command `LwSE01LBTestUI<slot> <scenario>` is available in console
# Downlink commands LwSE01LB* are available in console
```

## Changelog
```
- v1.0.0 (2025-08-20): Initial generation from wiki specification
  - Complete uplink coverage: 28 channels across 6 payload types
  - Complete downlink coverage: 10 commands for device control
  - Support for MOD=0/1 modes (calibrated/raw values)
  - Support for interrupt/counting modes
  - External DS18B20 temperature sensor support
  - Device status and datalog functionality
  - Comprehensive test scenarios and UI examples
```