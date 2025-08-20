# Dragino LHT52 LoRaWAN Decoder

## Device Information
- **Manufacturer**: Dragino
- **Model**: LHT52
- **Type**: Temperature & Humidity Sensor
- **LoRaWAN Version**: v1.0.3
- **Regions**: CN470, EU433, KR920, US915, EU868, AS923, AU915, IN865
- **Official Reference**: https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/LHT52%20-%20LoRaWAN%20Temperature%20%26%20Humidity%20Sensor%20User%20Manual/

## Implementation Details
- **Driver Version**: 1.0.0
- **Generated**: 2025-08-20
- **Coverage**: 4/4 uplinks implemented, 11/11 downlinks implemented
- **Framework**: v2.2.8
- **Template**: v2.3.0

## Expected UI Examples

### Example 1: Normal Operation
```
┌─────────────────────────────────────┐
│ 🏠 LHT52-slot1  Dragino LHT52       │
│ 🔋 2.87V 📶 -75dBm ⏱️ 2m ago       │
├─────────────────────────────────────┤
│ 🌡️ 25.0°C 💧 61.0% 🔗 26.6°C      │
└─────────────────────────────────────┘
```

### Example 2: Device Status
```
┌─────────────────────────────────────┐
│ 🏠 LHT52-slot1  Dragino LHT52       │
│ 🔋 2.87V 📶 -75dBm ⏱️ 1m ago       │
├─────────────────────────────────────┤
│ 💿 v1.0 📡 EU868 📶 SB0            │
└─────────────────────────────────────┘
```

### Example 3: Historical Data
```
┌─────────────────────────────────────┐
│ 🏠 LHT52-slot1  Dragino LHT52       │
│ 🔋 2.87V 📶 -75dBm ⏱️ 3m ago       │
├─────────────────────────────────────┤
│ 🌡️ 25.0°C 💧 61.0% 🔗 26.6°C      │
│ 📊 Datalog ⏰ 08/20 15:30          │
└─────────────────────────────────────┘
```

### Example 4: External Sensor ID
```
┌─────────────────────────────────────┐
│ 🏠 LHT52-slot1  Dragino LHT52       │
│ 🔋 2.87V 📶 -75dBm ⏱️ 1m ago       │
├─────────────────────────────────────┤
│ 🔗 DS18B20                          │
└─────────────────────────────────────┘
```

## Command Reference

**Test Commands**:
| Command | Description | Usage | Example |
|---------|-------------|-------|---------|
| LwLHT52TestUI<slot> | UI scenarios | `<scenario>` | `LwLHT52TestUI1 normal` |

**Control Commands**:
| Command | Description | Usage | Downlink Hex |
|---------|-------------|-------|---------------|
| LwLHT52SetInterval<slot> | Set interval | `<ms>` | `01xxxxxxxx` |
| LwLHT52Reset<slot> | Reset device | (no params) | `04FF` |
| LwLHT52Factory<slot> | Factory reset | (no params) | `04FE` |
| LwLHT52Confirm<slot> | Confirmation mode | `enable/disable` | `0501/0500` |
| LwLHT52Subband<slot> | Set sub-band | `<0-8>` | `07xx` |
| LwLHT52ADR<slot> | ADR mode | `enable/disable` | `2201/2200` |
| LwLHT52Status<slot> | Request status | (no params) | `2301` |
| LwLHT52SensorID<slot> | Request sensor ID | (no params) | `2302` |
| LwLHT52Alarm<slot> | Alarm mode | `enable/disable` | `A501/A500` |
| LwLHT52TempCheck<slot> | Temp check interval | `<minutes>` | `A7xxxx` |
| LwLHT52Poll<slot> | Poll datalog | `<start>,<end>,<int>` | `31xxxxxxxxx` |

**Node Management**:
| Command | Description | Usage | 
|---------|-------------|-------|
| LwLHT52NodeStats | Get node stats | `<node_id>` |
| LwLHT52ClearNode | Clear node data | `<node_id>` |

## Usage Examples

```bash
# Test scenarios
LwLHT52TestUI1 normal        # Normal sensor reading
LwLHT52TestUI1 device_info   # Device status info
LwLHT52TestUI1 external      # With external sensor
LwLHT52TestUI1 datalog       # Historical data

# Control commands
LwLHT52SetInterval1 1200000  # 20-minute interval
LwLHT52Alarm1 enable         # Enable temperature alarm
LwLHT52Poll1 1692549000,1692552600,30  # Poll 1-hour range
LwLHT52Status1               # Request device status
```

## Testing

```bash
# Normal sensor data
LwSimulate1 -75,2,BC09600270000001

# Device status
LwSimulate1 -75,5,09010001003A0B

# With external sensor
LwSimulate1 -75,2,BC096002700A1001

# Historical data
LwSimulate1 -75,3,BC096002700A100160E26300

# Sensor ID response
LwSimulate1 -75,4,2801234567890ABC
```

## Changelog
- v1.0.0 (2025-08-20): Initial generation from cached MAP specification
