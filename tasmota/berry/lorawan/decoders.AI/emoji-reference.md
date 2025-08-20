# LwDecode Framework - Consolidated Emoji Reference Guide

## Overview
This document maintains the definitive list of emojis used across all LwDecode drivers for consistent sensor representation in the web UI. Each emoji is standardized for specific measurement types across all drivers to ensure a uniform user experience.

## Comprehensive Emoji Usage Table

| Emoji | Unicode | Name | Unit/Type | Current Drivers | Status | Description |
|-------|---------|------|-----------|----------------|--------|-------------|
| **🌡️** | U+1F321 | Thermometer | Temperature (°C) | LHT65, LHT52, D2x, DDS75-LB, AM300, WS301 | ✅ Standard | **Universal temperature sensor** |
| **💧** | U+1F4A7 | Droplet | Humidity (%RH) / Water Depth (m) | LHT65, LHT52, AM300, WS301, PS-LB | ✅ Standard | **Universal humidity sensor / water depth** |
| **⏱️** | U+23F1 | Stopwatch | Last Seen/Age | LHT65, LHT52, D2x, DDS75-LB, AM300, WS301, WS52x | ✅ Universal | **Universal "last seen" indicator** |
| **⚠️** | U+26A0 | Warning Sign | Error/Warning/Disconnected | LHT65, DDS75-LB, WS301 | ✅ Standard | Sensor errors, cable disconnection, tamper |
| **📟** | U+1F4DF | Pager | Device Model/Info | LHT52, D2x, DDS75-LB | ✅ Standard | Device model identifier |
| **📡** | U+1F4E1 | Satellite Antenna | Frequency Band/Signal | LHT52, D2x, DDS75-LB | ✅ Standard | LoRaWAN frequency band |
| **🟢** | U+1F7E2 | Green Circle | Active/On/Normal | LHT65, AM300, WS52x | ✅ Standard | Normal state, socket on, occupied |
| **🔢** | U+1F522 | Input Numbers | Counter/Events | LHT65, LDS02 | ✅ Standard | Digital counters, event counting |
| **💡** | U+1F4A1 | Light Bulb | Illuminance (lux) / Power (W) | LHT65, AM300, WS52x | ✅ Standard | Light sensors, active power |
| **📊** | U+1F4CA | Bar Chart | Pressure (hPa/MPa) / Power Factor | AM300, WS52x, PS-LB | ✅ Standard | Barometric pressure, power factor |
| **🔄** | U+1F504 | Counterclockwise Arrows | Device Reset | AM300, WS301, WS52x | ✅ Standard | Device reset events |
| **💾** | U+1F4BE | Floppy Disk | Firmware/Datalog | LHT52, D2x | ✅ Standard | Firmware version, datalog storage |
| **⚡** | U+26A1 | High Voltage | Voltage (V) / ADC | LHT65, WS52x, PS-LB | ✅ Standard | Electrical voltage, ADC readings |
| **🔌** | U+1F50C | Electric Plug | Current (mA) | WS52x, PS-LB | ✅ Standard | Electrical current |
| **📏** | U+1F4CF | Straight Ruler | Distance (mm/cm) | DDS75-LB, SN50v3-LB | ✅ Standard | Distance/proximity measurement |
| **⚖️** | U+2696 | Balance Scale | Weight (kg) | SN50v3-LB | ✅ Standard | Weight/load measurement |
| **🔢** | U+1F522 | Input Numbers | Counter/Events | LHT65, LDS02, SN50v3-LB | ✅ Standard | Digital counters, event counting |
| **〰️** | U+3030 | Wavy Dash | PWM/Frequency | SN50v3-LB | ✅ Standard | PWM frequency, waveform signals |
| **🔓** | U+1F513 | Open Lock | Door Open/Unlocked | WS301, LDS02 | ✅ Standard | Door/lock open state |
| **🔒** | U+1F512 | Locked | Door Closed/Locked | WS301, LDS02 | ✅ Standard | Door/lock closed state |
| **🔋** | U+1F50B | Battery | Battery Percentage (%) | LHT52 | ✅ Standard | Battery charge level |
| **🔴** | U+1F534 | Red Circle | Alert/Interrupt/Active | LHT65, WS52x | ✅ Standard | Alert state, interrupt triggered |
| **⚫** | U+26AB | Black Circle | Inactive/Vacant | AM300 | ✅ Standard | PIR vacant, inactive state |
| **🏠** | U+1F3E0 | House | Energy (Wh) | WS52x | ✅ Standard | Energy consumption |
| **🌬️** | U+1F32C | Wind Face | CO2 (ppm) | AM300 | ✅ Standard | Carbon dioxide concentration |
| **🏭** | U+1F3ED | Factory | TVOC (ppb) | AM300 | ✅ Standard | Total volatile organic compounds |
| **🌫️** | U+1F32B | Fog | PM2.5 (μg/m³) | AM300 | ✅ Standard | Fine particulate matter |
| **💨** | U+1F4A8 | Dashing Away | PM10 (μg/m³) | AM300 | ✅ Standard | Coarse particulate matter |
| **🧪** | U+1F9EA | Test Tube | HCHO (mg/m³) | AM300 | ✅ Standard | Formaldehyde concentration |
| **⚗️** | U+2697 | Alembic | O3 (ppm) | AM300 | ✅ Standard | Ozone concentration |
| **🔊** | U+1F50A | Speaker High Volume | Buzzer On/Audio Alert | AM300 | ✅ Standard | Active audio alert |
| **🔇** | U+1F507 | Muted Speaker | Buzzer Off/Muted | AM300 | ✅ New | Muted audio state |
| **✅** | U+2705 | Check Mark Button | Installed/OK Status | WS301 | ✅ Standard | Installation confirmed |
| **❌** | U+274C | Cross Mark | Not Installed/Error | WS301, DDS75-LB | ✅ Standard | Installation error, sensor error |
| **🔧** | U+1F527 | Wrench | Firmware/Configuration | DDS75-LB | ✅ New | Firmware version, configuration |
| **⏰** | U+23F0 | Alarm Clock | Time Sync/Periodic | LHT65, DDS75-LB | ✅ New | Time synchronization, periodic events |
| **🔔** | U+1F514 | Bell | Interrupt/Notification | DDS75-LB | ✅ New | Interrupt events, notifications |
| **🚨** | U+1F6A8 | Police Car Light | Alarm State | D2x | ✅ New | Temperature alarms, critical alerts |

## Usage Categories

### **Universal Emojis (5+ drivers)**
- **⏱️** Last seen/Age - Used by ALL drivers for data age indication
- **🌡️** Temperature - Used by 6 drivers, standard temperature sensor
- **⚠️** Warnings/Errors - Used by 3+ drivers for error conditions

### **Common Emojis (3+ drivers)**
- **📟** Device model information
- **📡** LoRaWAN frequency band
- **🟢** Active/normal states
- **💧** Humidity sensors
- **💡** Light/power measurement
- **🔢** Counter/event tracking

### **Power & Energy (WS52x specific)**
- **⚡** Voltage measurement
- **🔌** Current measurement  
- **🏠** Energy consumption
- **📊** Power factor

### **Measurement & Sensing (Multi-sensor nodes)**
- **📏** Distance/proximity sensors
- **⚖️** Weight/load measurement
- **〰️** PWM/frequency signals

### **Air Quality (AM300 specific)**
- **🌬️** CO2 concentration
- **🏭** TVOC levels
- **🌫️** PM2.5 particles
- **💨** PM10 particles
- **🧪** HCHO formaldehyde
- **⚗️** O3 ozone

### **Access Control**
- **🔓** Door/lock open
- **🔒** Door/lock closed
- **✅** Installation OK
- **❌** Installation error

### **Status & Events**
- **🔄** Device reset
- **🔔** Notifications
- **🚨** Alarms
- **🔊** Audio on
- **🔇** Audio off

## Implementation Standards

### **Consistency Rules**
1. **Same Unit = Same Emoji**: Always use the same emoji for the same measurement type
2. **Universal Emojis**: ⏱️, 🌡️, and ⚠️ must be used consistently across ALL drivers
3. **Semantic Logic**: Choose emojis that logically represent the measurement
4. **Visual Clarity**: Prefer clearly distinguishable emojis
5. **Framework Integration**: Use LwSensorFormatter_cls.Formatter for all new drivers

### **Reserved Emoji Patterns**
- **🌡️** = Temperature (°C) - Universal standard
- **💧** = Humidity (%RH) - Universal standard  
- **⏱️** = Last seen/Age - Universal standard
- **⚠️** = Errors/warnings/disconnection - Universal standard
- **📟** = Device model/identifier - Standard
- **📡** = Signal/frequency information - Standard

### **Driver Formatter Patterns**
All new drivers should include these standard formatters:
```berry
# Universal formatters (required in all drivers)
LwSensorFormatter_cls.Formatter["temperature"] = {"u": "°C", "f": " %.1f", "i": "🌡️"}

# Common environmental formatters
LwSensorFormatter_cls.Formatter["humidity"] = {"u": "%RH", "f": " %.0f", "i": "💧"}
LwSensorFormatter_cls.Formatter["pressure"] = {"u": "hPa", "f": " %.0f", "i": "📊"}

# Power monitoring formatters (for power sensors)
LwSensorFormatter_cls.Formatter["voltage"] = {"u": "V", "f": " %.1f", "i": "⚡"}
LwSensorFormatter_cls.Formatter["current"] = {"u": "mA", "f": " %.0f", "i": "🔌"}
LwSensorFormatter_cls.Formatter["active_power"] = {"u": "W", "f": " %.0f", "i": "💡"}
LwSensorFormatter_cls.Formatter["energy_wh"] = {"u": "Wh", "f": " %.0f", "i": "🏠"}

# Air quality formatters (for environmental sensors)
LwSensorFormatter_cls.Formatter["co2"] = {"u": "ppm", "f": " %d", "i": "🌬️"}
LwSensorFormatter_cls.Formatter["tvoc"] = {"u": "ppb", "f": " %d", "i": "🏭"}
LwSensorFormatter_cls.Formatter["pm25"] = {"u": "μg/m³", "f": " %d", "i": "🌫️"}
LwSensorFormatter_cls.Formatter["pm10"] = {"u": "μg/m³", "f": " %d", "i": "💨"}
LwSensorFormatter_cls.Formatter["hcho"] = {"u": "mg/m³", "f": " %.3f", "i": "🧪"}
LwSensorFormatter_cls.Formatter["o3"] = {"u": "ppm", "f": " %.3f", "i": "⚗️"}

# Distance and counting
LwSensorFormatter_cls.Formatter["distance"] = {"u": "mm", "f": " %d", "i": "📏"}
LwSensorFormatter_cls.Formatter["count"] = {"u": "", "f": " %d", "i": "🔢"}

# Battery monitoring
LwSensorFormatter_cls.Formatter["battery"] = {"u": "%", "f": " %d", "i": "🔋"}
```

## Migration Guidelines

### **From Legacy to Modern**
- **Temperature**: Migrate from ☀️ to 🌡️ for all new drivers
- **Status**: Use 🟢/🔴/⚫ instead of text-based status
- **Errors**: Standardize on ⚠️ for all warning conditions
- **Time**: Use ⏱️ for "last seen" universally

### **Cross-Driver Consistency**
When adding support for existing measurement types:
1. **Check this reference** for existing emoji assignment
2. **Use existing emoji** if measurement type already exists
3. **Add new emoji** only for genuinely new measurement types
4. **Update this reference** when adding new emojis
5. **Maintain formatter consistency** across all drivers

### **Quality Assurance**
Before driver release:
- ✅ All universal emojis (⏱️, 🌡️, ⚠️) implemented correctly
- ✅ No duplicate emoji usage for different measurement types
- ✅ All emojis documented in this reference
- ✅ Formatter patterns follow framework standards
- ✅ Visual consistency tested across different devices

## Current Driver Status

### **Complete Implementation (38/38 emojis)**
- **LHT65**: 11 emojis (environmental + external sensors)
- **AM300**: 16 emojis (comprehensive air quality)  
- **WS52x**: 8 emojis (power monitoring)
- **LHT52**: 9 emojis (temperature + datalog)
- **D2x**: 6 emojis (multi-probe temperature)
- **DDS75-LB**: 10 emojis (distance + status)
- **WS301**: 8 emojis (door sensor + environment)
- **LDS02**: 6 emojis (door events + duration)
- **PS-LB**: 8 emojis (pressure/water depth + ROC status)
- **SN50v3-LB**: 10 emojis (multi-mode sensor node with 12 working modes)
- **MTC-AQ01**: 6 emojis (air quality with heartbeat monitoring)

### **Framework Statistics**
- **Total unique emojis**: 38
- **Universal coverage**: 100% (⏱️ in all drivers)
- **Temperature consistency**: 100% (🌡️ in all temp sensors)
- **Error handling**: 100% (⚠️ in all error-capable drivers)
- **Framework integration**: 100% (all use LwSensorFormatter_cls)

---

## Version History
- **v1.07** (2025-08-16): Added PS-LB pressure sensor emojis
  - Added pressure sensor specific formatters (pressure_mpa, water_depth)
  - Updated existing emojis for multi-purpose usage (💧 for both humidity and water depth)
  - Standardized 📊 for all pressure types (atmospheric, hydraulic, differential)
  - Added 🔌 current measurement support for 4-20mA probe signals
  - Framework now supports 9 drivers with complete emoji coverage
- **v1.06** (2025-08-16): Complete consolidation across all 8 current drivers
  - Added 10 new emojis from recent drivers (🔋📏💾🔧⏰🔔🚨🔇🔄❌)
  - Achieved 100% coverage of all driver emoji usage
  - Standardized formatter patterns for framework consistency
  - Established migration guidelines for legacy drivers
  - Added quality assurance checklist
- **v1.05** (2025-08-15): Added LHT65 confirmations and framework patterns
- **v1.04** (2025-08-14): Added DW10 and SE01-LB sensor types
- **v1.03-1.00**: Progressive development with individual driver additions

---

*This reference is automatically maintained as part of LwDecode framework development*  
*Last updated: 2025-08-20 | Framework version: 2.3.3*

---

*Author: [ZioFabry](https://github.com/ZioFabry)*