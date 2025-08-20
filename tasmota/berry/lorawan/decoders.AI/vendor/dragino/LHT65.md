# Dragino LHT65 LoRaWAN Decoder

## Device Information
- **Manufacturer**: Dragino
- **Model**: LHT65
- **Type**: Temperature & Humidity Sensor with 9 External Sensor Types
- **LoRaWAN Version**: v1.0.3
- **Regions**: CN470, EU433, KR920, US915, EU868, AS923, AU915, IN865
- **Official Reference**: https://www.dragino.com/products/lora-lorawan-end-node/item/151-lht65.html

## Implementation Details
- **Driver Version**: 1.0.0
- **Generated**: 2025-08-20
- **Coverage**: 1/1 uplinks implemented, 7/7 downlinks implemented
- **Framework**: v2.2.8
- **Template**: v2.3.0

## Expected UI Examples

### Example 1: Normal Operation (No External Sensor)
```
┌─────────────────────────────────────┐
│ 🏠 LHT65-slot1  Dragino LHT65       │
│ 🔋 2.50V 📶 -75dBm ⏱️ 2m ago       │
├─────────────────────────────────────┤
│ 🌡️ 26.2°C 💧 60.0%                │
│ 🔋 Good                             │
└─────────────────────────────────────┘
```

### Example 2: With External Temperature Sensor
```
┌─────────────────────────────────────┐
│ 🏠 LHT65-slot1  Dragino LHT65       │
│ 🔋 2.50V 📶 -75dBm ⏱️ 2m ago       │
├─────────────────────────────────────┤
│ 🌡️ 26.2°C 💧 60.0% 🔗 28.2°C      │
│ 🔗 E1 Temperature 🔌 Connected      │
└─────────────────────────────────────┘
```

### Example 3: Illumination Sensor
```
┌─────────────────────────────────────┐
│ 🏠 LHT65-slot1  Dragino LHT65       │
│ 🔋 2.50V 📶 -75dBm ⏱️ 2m ago       │
├─────────────────────────────────────┤
│ 🌡️ 26.2°C 💧 60.0% ☀️ 1000lx      │
│ 🔗 E5 Illumination 🔌 Connected     │
└─────────────────────────────────────┘
```

### Example 4: Interrupt Sensor Triggered
```
┌─────────────────────────────────────┐
│ 🏠 LHT65-slot1  Dragino LHT65       │
│ 🔋 2.50V 📶 -75dBm ⏱️ 1m ago       │
├─────────────────────────────────────┤
│ 🌡️ 26.2°C 💧 60.0%                │
│ 🔗 E4 Interrupt 🔌 Connected ⚡ Triggered│
└─────────────────────────────────────┘
```

## Command Reference

**Test Commands**:
| Command | Description | Usage | Example |
|---------|-------------|-------|---------|
| LwLHT65TestUI<slot> | UI scenarios | `<scenario>` | `LwLHT65TestUI1 normal` |

**Control Commands**:
| Command | Description | Usage | Downlink Hex |
|---------|-------------|-------|---------------|
| LwLHT65SetInterval<slot> | Set interval | `<seconds>` | `01xxxxxx` |
| LwLHT65ExtSensor<slot> | Set external sensor | `<type>[,param][,timeout]` | `A2xxxx` |
| LwLHT65ProbeID<slot> | DS18B20 probe ID | `enable/disable` | `A801/A800` |
| LwLHT65SetTime<slot> | Set system time | `<unix_timestamp>` | `30xxxxxxxx` |
| LwLHT65TimeSync<slot> | Time sync mode | `auto/manual` | `2801/2800` |
| LwLHT65ClearFlash<slot> | Clear datalog | (no params) | `A301` |
| LwLHT65Poll<slot> | Poll datalog | `<start>,<end>,<int>` | `31xxxxxxxxx` |

## Usage Examples

```bash
# Test scenarios
LwLHT65TestUI1 normal         # Normal operation
LwLHT65TestUI1 ext_temp       # With E1 temperature
LwLHT65TestUI1 illumination   # E5 light sensor
LwLHT65TestUI1 interrupt      # E4 interrupt triggered

# Control commands
LwLHT65SetInterval1 1800      # 30-minute interval
LwLHT65ExtSensor1 1           # Set to E1 temperature
LwLHT65ExtSensor1 5           # Set to E5 illumination
LwLHT65Poll1 1692549000,1692552600,30  # Poll datalog
```

## Testing

```bash
# Normal operation
LwSimulate1 -75,2,C00A0C0A580200000000

# With external temperature sensor
LwSimulate1 -75,2,C00A0C0A58020A1A0100

# Illumination sensor
LwSimulate1 -75,2,C00A0C0A580205E8030080

# Interrupt triggered
LwSimulate1 -75,2,C00A0C0A58020481000000
```

## Changelog
- v1.0.0 (2025-08-20): Initial generation from cached MAP specification
