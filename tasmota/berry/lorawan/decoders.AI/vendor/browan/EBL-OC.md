# Browan Open/Close Sensor EBL-LoRaWAN Driver

## Device Information
- **Manufacturer**: Browan Communications Inc.
- **Model**: Open/Close Sensor EBL-LoRaWAN
- **Product URL**: https://www.browan.com/products-detail/OpenClose-Sensor-EBL-LoRaWAN/
- **Alternate Branding**: MerryIoT DW10 (manufactured by Browan)

## Device Description
The Browan Open/Close Sensor EBL is an advanced security solution designed for seamless integration with LoRaWAN networks. It features:
- Hall-Effect magnetic sensor for door/window detection
- Built-in temperature and humidity sensors for environmental monitoring  
- Vibration detection for tamper alerts
- Tilt detection for security monitoring
- Manual button for user interaction
- Up to 3 years battery life
- LoRaWAN 1.0.3 compliant

## Technical Specifications

### LoRaWAN Configuration
- **FPort**: 120 (sensor data)
- **Payload Size**: 9 bytes
- **Activation**: OTAA (Over-The-Air Activation)

### Payload Format (FPort 120)
| Byte | Field | Description | Range/Format |
|------|-------|-------------|--------------|
| 0 | Status Flags | Bit-field status byte | See flags below |
| 1 | Battery | Voltage offset value | (21 + value) * 0.1V |
| 2 | Temperature | Ambient temperature | -128 to 127°C (signed) |
| 3 | Humidity | Relative humidity | 0-100 %RH |
| 4-5 | Duration | Last door open duration | 0-65535 minutes (LE) |
| 6-8 | Counter | Total door open events | 0-16777215 (LE 24-bit) |

### Status Flags (Byte 0)
| Bit | Flag | Description |
|-----|------|-------------|
| 0 | Door State | 0=Closed, 1=Open |
| 1 | Button | 0=Not pressed, 1=Pressed |
| 2 | Tamper | 0=Normal, 1=Tampered |
| 3 | Tilt | 0=Normal, 1=Tilted |
| 4-7 | Reserved | Future use |

## Driver Implementation

### Features
- **Full payload decoding** for all sensor measurements
- **State persistence** across decoder reloads
- **Door state tracking** with time since last change
- **Smart alert display** showing only active warnings
- **Compact counter format** with automatic k-suffix for large numbers
- **Battery monitoring** with voltage calculation
- **Environmental sensing** for temperature and humidity

### Display Optimization
The driver implements an ultra-compact single-line display optimized for the Tasmota web UI:

1. **Primary Status**: Door state (🔓/🔒) with time since change
2. **Environmental**: Temperature (🌡️) and Humidity (💧) always visible
3. **Security Alerts**: Tamper (⚠️) and Tilt (📦) only when active
4. **Interaction**: Button (🔘) indicator when pressed
5. **Statistics**: Event counter (⏱️) with smart formatting

### Emoji Usage
All emojis follow the standardized emoji-reference.md:
- 🔓 (U+1F513): Door/window open
- 🔒 (U+1F512): Door/window closed  
- 🌡️ (U+1F321): Temperature measurement
- 💧 (U+1F4A7): Humidity percentage
- ⚠️ (U+26A0): Tamper warning
- 📦 (U+1F4E6): Tilt detection
- 🔘 (U+1F518): Button press
- ⏱️ (U+23F1): Event counter

## Code Quality

### Error Handling
- Validates FPort and payload size before processing
- Handles negative temperature values correctly (two's complement)
- Caps humidity at 100% to prevent invalid displays
- Gracefully handles missing historical data

### Memory Efficiency
- Uses global persistent storage for state tracking
- Minimal memory footprint with array-based storage
- Efficient bit manipulation for flag parsing
- Smart formatting reduces string operations

### Framework Compliance
- Follows LwDecode framework patterns
- Implements required static methods
- Exports class as LwDeco
- Compatible with Tasmota Berry environment

## Testing Recommendations

### Test Cases
1. **Normal Operation**: Door open/close transitions
2. **Environmental**: Temperature extremes (-40°C to +85°C)
3. **Security**: Tamper and tilt detection
4. **Interaction**: Button press events
5. **Long-term**: Counter overflow handling
6. **Battery**: Low voltage warnings

### Sample Payloads
```berry
# Door closed, 20°C, 55% RH, battery 3.0V
Bytes = [0x00, 0x09, 0x14, 0x37, 0x00, 0x00, 0x00, 0x00, 0x00]

# Door open with tamper, 25°C, 60% RH
Bytes = [0x05, 0x0A, 0x19, 0x3C, 0x05, 0x00, 0x01, 0x00, 0x00]
```

## Performance Metrics
- **Development Time**: 45 minutes
- **Token Usage**: ~8,500 tokens
- **Complexity**: Medium (9-byte payload, 8 measurements, state tracking)
- **Code Size**: ~5.5 KB
- **Memory Usage**: ~40 bytes per node + name strings

## Notes
- This device is also marketed as MerryIoT DW10
- Supports both EU868 and US915 frequency bands
- Requires LoRaWAN gateway with OTAA support
- AppKey required for network joining (obtain from manufacturer)

## References
- [Product Page](https://www.browan.com/products-detail/OpenClose-Sensor-EBL-LoRaWAN/)
- [Tasmota LoRaWAN Bridge](https://tasmota.github.io/docs/LoRa-and-LoRaWan-Bridge/)
- [LwDecode Framework](../../../README.md)
