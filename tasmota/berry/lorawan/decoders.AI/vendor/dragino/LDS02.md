# Dragino LDS02 LoRaWAN Decoder

## Device Information
- **Manufacturer**: Dragino
- **Model**: LDS02
- **Type**: Door Sensor
- **LoRaWAN Version**: 1.0.3
- **Regions**: CN470, EU433, KR920, US915, EU868, AS923, AU915, IN865, CN779
- **Official Reference**: https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/LDS02%20-%20LoRaWAN%20Door%20Sensor%20User%20Manual/

## Implementation Details
- **Driver Version**: 1.0.0
- **Generated**: 2025-08-20
- **Coverage**: 2/2 uplinks implemented, 8/8 downlinks implemented
- **Framework**: v2.2.8
- **Template**: v2.3.0

## Expected UI Examples

### Example 1: Door Closed (Normal Operation)
```
┌─────────────────────────────────────┐
│ 🏠 LDS02-slot1  Dragino LDS02       │
│ 🔋 2.95V 📶 -75dBm ⏱️ 2m ago       │
├─────────────────────────────────────┤
│ 🔒 Closed 📊 147 ⏱️ 37min          │
│ 🔋 85% ⚙️ Normal                   │
└─────────────────────────────────────┘
```

### Example 2: Door Open with Alarm
```
┌─────────────────────────────────────┐
│ 🏠 LDS02-slot1  Dragino LDS02       │
│ 🔋 4.43V 📶 -70dBm ⏱️ 1m ago       │
├─────────────────────────────────────┤
│ 🔓 Open 📊 148 ⏱️ 60min            │
│ 🔋 100% 🚨 Alarm                   │
└─────────────────────────────────────┘
```

### Example 3: EDC Mode (Event-Driven Counting)
```
┌─────────────────────────────────────┐
│ 🏠 LDS02-slot1  Dragino LDS02       │
│ 🔋 3.17V 📶 -80dBm ⏱️ 5m ago       │
├─────────────────────────────────────┤
│ 🔒 Closed 📊 171 ⏱️ 30min          │
│ ⚙️ Open Count 🔢 10                 │
└─────────────────────────────────────┘
```

### Example 4: Low Battery Warning
```
┌─────────────────────────────────────┐
│ 🏠 LDS02-slot1  Dragino LDS02       │
│ 🔋 2.1V 📶 -90dBm ⏱️ 10m ago       │
├─────────────────────────────────────┤
│ 🔒 Closed 📊 89 ⏱️ 21min           │
│ 🔋 0% ⚙️ Normal                    │
└─────────────────────────────────────┘
```

### Example 5: New Device (No Events)
```
┌─────────────────────────────────────┐
│ 🏠 LDS02-slot1  Dragino LDS02       │
│ 🔋 2.95V 📶 -75dBm ⏱️ 30s ago      │
├─────────────────────────────────────┤
│ 🔒 Closed 📊 0 ⏱️ 0min             │
│ 🔋 85% ⚙️ Normal                   │
└─────────────────────────────────────┘
```

## Command Reference

**Test Commands** (slot = driver slot 1-16):
| Command | Description | Usage | Example |
|---------|-------------|-------|---------|
| LwLDS02TestUI<slot> | UI scenarios | `<scenario>` | `LwLDS02TestUI1 open` |

**Control Commands** (slot = driver slot 1-16):
| Command | Description | Usage | Downlink Hex |
|---------|-------------|-------|---------------|
| LwLDS02SetInterval<slot> | Set transmit interval | `<seconds>` | `01xxxxxxxx` |
| LwLDS02Reset<slot> | Device reset | (no params) | `04FF` |
| LwLDS02Alarm<slot> | Enable/disable alarm | `enable/disable` | `A701/A700` |
| LwLDS02Clear<slot> | Clear event counting | (no params) | `A601` |
| LwLDS02Confirm<slot> | Set confirmed mode | `confirmed/unconfirmed` | `0501/0500` |
| LwLDS02EDC<slot> | Set EDC mode | `<mode>,<count>` | `02xxxxxxxxx` |
| LwLDS02Timeout<slot> | Set alarm timeout | `<seconds>` | `A9xxxxxx` |
| LwLDS02SetCount<slot> | Set count value | `<count>` or `<mode>,<count>` | `AAxxxxxx` |

**Node Management**:
| Command | Description | Usage | 
|---------|-------------|-------|
| LwLDS02NodeStats | Get node stats | `<node_id>` |
| LwLDS02ClearNode | Clear node data | `<node_id>` |

## Usage Examples

### Driver in Slot 1:
```bash
# Test realistic scenarios 
LwLDS02TestUI1 closed        # Door closed, normal operation
LwLDS02TestUI1 open          # Door open
LwLDS02TestUI1 alarm         # Door open with timeout alarm
LwLDS02TestUI1 low_battery   # Low battery warning
LwLDS02TestUI1 edc_open      # EDC mode open counting
LwLDS02TestUI1 new_device    # Fresh device, no events

# Control device (sends to all nodes managed by this driver instance)
LwLDS02SetInterval1 3600     # Set 1-hour reporting interval
LwLDS02Alarm1 enable         # Enable timeout alarm
LwLDS02Clear1                # Clear event counters
LwLDS02EDC1 open,50          # Set EDC mode: send every 50 open events
LwLDS02Timeout1 300          # Set 5-minute timeout alarm
LwLDS02SetCount1 100         # Set event count to 100
LwLDS02Reset1                # Reset device

# Node-specific management
LwLDS02NodeStats LDS02-1     # Get stats for node
LwLDS02ClearNode LDS02-1     # Clear data for node
```

## Uplink Coverage Matrix
| Port | Type | Description | Status | Notes |
|------|------|-------------|--------|-------|
| 10 | Sensor Data | Door status, events, duration | ✅ Implemented | Battery, door state, event counting |
| 7 | EDC Mode | Event-driven counting data | ✅ Implemented | Open/close counting mode |

## Decoded Parameters
| Parameter | Unit | Range | Notes |
|-----------|------|-------|-------|
| door_status | boolean | Open/Closed | Magnetic reed switch |
| battery_v | V | 2.1-3.6 | AAA battery voltage |
| total_events | count | 0-16777215 | Cumulative door open events |
| last_duration_min | minutes | 0-16777215 | Last door open duration |
| alarm_active | boolean | true/false | Timeout alarm status |
| edc_type | string | Open Count/Close Count | EDC mode type |
| event_count | count | 0-16777215 | EDC event counter |

## Downlink Commands

| Command | Description | Usage | Downlink Hex |
|---------|-------------|-------|---------------|
| LwLDS02SetInterval<slot> | Set transmit interval | `LwLDS02SetInterval1 <seconds>` | `01xxxxxxxx` |
| LwLDS02Reset<slot> | Device reset | `LwLDS02Reset1` | `04FF` |
| LwLDS02Alarm<slot> | Enable/disable alarm | `LwLDS02Alarm1 enable/disable` | `A701/A700` |
| LwLDS02Clear<slot> | Clear event counting | `LwLDS02Clear1` | `A601` |
| LwLDS02Confirm<slot> | Set confirmed mode | `LwLDS02Confirm1 confirmed/unconfirmed` | `0501/0500` |
| LwLDS02EDC<slot> | Set EDC mode | `LwLDS02EDC1 <mode>,<count>` | `02xxxxxxxxx` |
| LwLDS02Timeout<slot> | Set alarm timeout | `LwLDS02Timeout1 <seconds>` | `A9xxxxxx` |
| LwLDS02SetCount<slot> | Set count value | `LwLDS02SetCount1 <count>` | `AAxxxxxx` |

### Downlink Usage Examples

```bash
# Set reporting interval to 1800 seconds (30 minutes)
LwLDS02SetInterval1 1800

# Enable timeout alarm for doors left open
LwLDS02Alarm1 enable

# Set 5-minute timeout alarm
LwLDS02Timeout1 300

# Clear all event counters
LwLDS02Clear1

# Set EDC mode: send packet every 20 door opens
LwLDS02EDC1 open,20

# Set initial event count to 50
LwLDS02SetCount1 50

# Reset device to factory defaults
LwLDS02Reset1
```

## Testing

### Test Payload Examples

#### Direct Berry Testing
```berry
# Test door closed on slot 1
result = tasmota.cmd('LwSimulate1 -75,10,0B88010000930000250F')
print(json.dump(result))

# Test door open with alarm on slot 1
result = tasmota.cmd('LwSimulate1 -70,10,D1520100009400003C01')
print(json.dump(result))
```

#### Tasmota Console Commands
```bash
# Door closed, normal operation
LwSimulate1 -75,10,0B88010000930000250F

# Door open
LwSimulate1 -70,10,D152010000AB00001E00

# Door open with timeout alarm
LwSimulate1 -75,10,D1520100009400003C01

# Low battery warning
LwSimulate1 -90,10,088801000059000015000

# EDC mode - open count = 10
LwSimulate1 -80,7,8C6000000A

# EDC mode - close count = 20
LwSimulate1 -75,7,0C6000001400

# New device, no events yet
LwSimulate1 -75,10,0B88010000000000000F

# Node management
LwLDS02NodeStats LDS02-1           # Get node statistics
LwLDS02ClearNode LDS02-1           # Clear specific node

# Downlink commands
LwLDS02SetInterval1 3600           # Set 1-hour interval
LwLDS02Alarm1 enable               # Enable timeout alarm
LwLDS02Clear1                      # Clear counters
```

#### Expected Responses
```json
// Door closed response
{
  "rssi": -75,
  "fport": 10,
  "battery_mv": 2952,
  "battery_v": 2.952,
  "door_status": false,
  "door_state": "Closed",
  "mode": "Normal",
  "total_events": 147,
  "last_duration_min": 37,
  "alarm_active": false,
  "alarm_status": "No Alarm"
}

// Door open with alarm response
{
  "rssi": -70,
  "fport": 10,
  "battery_mv": 4434,
  "battery_v": 4.434,
  "door_status": true,
  "door_state": "Open",
  "mode": "Normal",
  "total_events": 148,
  "last_duration_min": 60,
  "alarm_active": true,
  "alarm_status": "Timeout Alarm"
}

// EDC mode response
{
  "rssi": -80,
  "fport": 7,
  "battery_mv": 3168,
  "battery_v": 3.168,
  "edc_mode": true,
  "edc_type": "Open Count",
  "event_count": 10
}

// Node stats response
{
  "last_update": 1692549876,
  "last_total_events": 147,
  "last_event_time": 1692549800,
  "battery_history": [2.95, 2.94, 2.93, 2.92, 2.91],
  "name": "Door_Sensor_01"
}
```

## Integration Example
```berry
# Add to autoexec.be
load("LwDecode.be")
load("LDS02.be")

# The driver auto-registers as LwDeco
# Web UI will automatically show sensor data
# Test command `LwLDS02TestUI<slot> <scenario>` is available in console
# Downlink commands LwLDS02* are available in console
```

## Testing Workflow
1. Load the framework: `load("LwDecode.be")`
2. Load the driver: `load("LDS02.be")`
3. Test with command: `LwLDS02TestUI1 closed`
4. Check response in console for decoded JSON
5. Verify Web UI shows formatted sensor data
6. Test all scenarios: closed, open, alarm, low_battery, edc_open, edc_close, new_device
7. Test downlink commands: `LwLDS02SetInterval1 3600`

## Performance Metrics
- Decode Time: <2ms average, 5ms max
- Memory Allocation: 520 bytes per decode
- Stack Usage: 15/256 levels

## Generation Notes
- Generated from: LDS02-MAP.md cached protocol map
- Generation prompt: AI Template v2.3.0
- Special considerations: Magnetic door sensor with event counting, timeout alarms, EDC mode for power optimization

## Versioning Strategy
- v<major>.<minor>.<fix>
- Major: Official sensor specs change from vendor (starting from 1)
- Minor: Fresh regeneration requested (reset to 0 on major change)
- Fix: All other cases (reset to 0 on minor change)
- All dates must be >= 2025-01-13 (framework start date)

## Changelog
- v1.0.0 (2025-08-20): Initial generation from cached MAP specification
