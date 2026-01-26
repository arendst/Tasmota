# Matter Plugin Class Hierarchy Documentation

**Last Updated**: January 23, 2026  
**Matter Version**: 1.4.1 Compliant  
**Data Model Revision**: 18 (Matter 1.3+)

## Overview

This document describes the complete class hierarchy for Matter endpoint plugins in the Berry Matter implementation. The hierarchy follows a structured approach with base classes providing common functionality and specialized classes implementing specific device behaviors.

**Recent Updates (Matter 1.4.1)**:
- ✅ All device type revisions updated to Matter 1.4.1 Device Library specifications
- ✅ Data Model Revision updated from 17 (Matter 1.2) to 18 (Matter 1.3+)
- ✅ Migrated from Scenes (0x0005) to Scenes Management (0x0062) cluster
- ✅ Extended Color Light now supports both HueSaturation AND ColorTemperature modes
- ✅ Added RemainingTime attribute to Color Control clusters
- ✅ Added Unit Localization cluster (0x002D) to Root Node
- ✅ Updated all cluster revisions to Matter 1.4.1 specifications

## Class Hierarchy Tree

```
Matter_Plugin (Base Class)
├── Matter_Plugin_Root
├── Matter_Plugin_Aggregator  
└── Matter_Plugin_Device
    ├── Matter_Plugin_Light0
    │   ├── Matter_Plugin_Light1
    │   │   ├── Matter_Plugin_Light2
    │   │   └── Matter_Plugin_Light3
    │   ├── Matter_Plugin_OnOff
    │   ├── Matter_Plugin_Bridge_Light0
    │   │   └── Matter_Plugin_Bridge_OnOff
    │   ├── Matter_Plugin_Bridge_Light1
    │   ├── Matter_Plugin_Bridge_Light2
    │   ├── Matter_Plugin_Bridge_Light3
    │   ├── Matter_Plugin_Virt_Light0
    │   ├── Matter_Plugin_Virt_Light1
    │   ├── Matter_Plugin_Virt_Light2
    │   ├── Matter_Plugin_Virt_Light3
    │   ├── Matter_Plugin_Virt_OnOff
    │   ├── Matter_Plugin_Zigbee_Light0
    │   ├── Matter_Plugin_Zigbee_Light1
    │   └── Matter_Plugin_Zigbee_Light2
    ├── Matter_Plugin_Fan
    │   └── Matter_Plugin_Virt_Fan
    ├── Matter_Plugin_Shutter
    │   └── Matter_Plugin_ShutterTilt
    ├── Matter_Plugin_Sensor
    │   ├── Matter_Plugin_Sensor_Temp
    │   │   ├── Matter_Plugin_Bridge_Sensor_Temp
    │   │   ├── Matter_Plugin_Virt_Sensor_Temp
    │   │   └── Matter_Plugin_Zigbee_Temperature
    │   ├── Matter_Plugin_Sensor_Humidity
    │   │   ├── Matter_Plugin_Bridge_Sensor_Humidity
    │   │   ├── Matter_Plugin_Virt_Sensor_Humidity
    │   │   └── Matter_Plugin_Zigbee_Humidity
    │   ├── Matter_Plugin_Sensor_Pressure
    │   │   ├── Matter_Plugin_Bridge_Sensor_Pressure
    │   │   ├── Matter_Plugin_Virt_Sensor_Pressure
    │   │   └── Matter_Plugin_Zigbee_Pressure
    │   ├── Matter_Plugin_Sensor_Illuminance
    │   │   ├── Matter_Plugin_Bridge_Sensor_Illuminance
    │   │   └── Matter_Plugin_Virt_Sensor_Illuminance
    │   └── Matter_Plugin_Sensor_Flow
    │       ├── Matter_Plugin_Bridge_Sensor_Flow
    │       └── Matter_Plugin_Virt_Sensor_Flow
    ├── Matter_Plugin_Sensor_Boolean
    │   ├── Matter_Plugin_Sensor_Contact
    │   │   ├── Matter_Plugin_Bridge_Sensor_Contact
    │   │   └── Matter_Plugin_Virt_Sensor_Contact
    │   ├── Matter_Plugin_Sensor_Occupancy
    │   │   ├── Matter_Plugin_Bridge_Sensor_Occupancy
    │   │   ├── Matter_Plugin_Virt_Sensor_Occupancy
    │   │   └── Matter_Plugin_Zigbee_Occupancy
    │   ├── Matter_Plugin_Sensor_OnOff
    │   ├── Matter_Plugin_Sensor_Rain
    │   │   ├── Matter_Plugin_Bridge_Sensor_Rain
    │   │   └── Matter_Plugin_Virt_Sensor_Rain
    │   └── Matter_Plugin_Sensor_Waterleak
    │       ├── Matter_Plugin_Bridge_Sensor_Waterleak
    │       └── Matter_Plugin_Virt_Sensor_Waterleak
    ├── Matter_Plugin_Sensor_Air_Quality
    │   ├── Matter_Plugin_Bridge_Sensor_Air_Quality
    │   └── Matter_Plugin_Virt_Sensor_Air_Quality
    └── Matter_Plugin_Sensor_GenericSwitch_Btn
```

## Base Classes

### Matter_Plugin (Level 0)
**File**: `Matter_Plugin_0.be`
- **Purpose**: Root base class for all Matter plugins
- **Key Features**:
  - Defines common plugin interface and behavior
  - Provides cluster and command management
  - Implements basic attribute handling
  - Manages update timing and virtual device flags

**Static Properties**:
```berry
static var TYPE = ""                    # Plugin type identifier
static var DISPLAY_NAME = ""            # Human-readable name
static var ARG = ""                     # Configuration argument name
static var UPDATE_TIME = 5000           # Update frequency (ms)
static var VIRTUAL = false              # Virtual device flag
static var BRIDGE = false               # Bridge device flag
static var ZIGBEE = false               # Zigbee device flag
static var CLUSTERS = { 0x001D: [0,1,2,3] }  # Descriptor cluster
```

## Infrastructure Classes (Level 1)

### Matter_Plugin_Root
**File**: `Matter_Plugin_1_Root.be`
- **Type**: `"root"`
- **Display Name**: `"Root node"`
- **Matter Device Type**: `0x0016` (Root node) - **Rev 3** (Matter 1.4.1)
- **Purpose**: Implements core Matter device functionality
- **Key Clusters**:
  - `0x001F`: Access Control
  - `0x0028`: Basic Information (Data Model Rev 18)
  - `0x002B`: Localization Configuration
  - `0x002C`: Time Format Localization
  - `0x002D`: Unit Localization (Matter 1.3+)
  - `0x0030`: General Commissioning
  - `0x0031`: Network Commissioning
  - `0x003C`: Administrator Commissioning
  - `0x003E`: Node Operational Credentials
  - `0x003F`: Group Key Management
  - `0x0046`: ICD Management (SIT mode)

### Matter_Plugin_Aggregator
**File**: `Matter_Plugin_1_Aggregator.be`
- **Type**: `"aggregator"`
- **Display Name**: `"Aggregator"`
- **Matter Device Type**: `0x000E` (Aggregator)
- **Purpose**: Groups multiple endpoints together

### Matter_Plugin_Device
**File**: `Matter_Plugin_1_Device.be`
- **Purpose**: Base class for all physical and virtual devices
- **Key Features**:
  - HTTP remote communication support
  - Bridge device basic information
  - Common device clusters (Identify, Groups, Scenes)
- **Key Clusters**:
  - `0x0039`: Bridged Device Basic Information
  - `0x0003`: Identify
  - `0x0004`: Groups
  - `0x0005`: Scenes

## Lighting Classes (Level 2+)

### Matter_Plugin_Light0
**File**: `Matter_Plugin_2_Light0.be`
- **Type**: `"light0"`
- **Display Name**: `"Light 0 OnOff"`
- **Matter Device Type**: `0x0100` (OnOff Light) - **Rev 3** (Matter 1.4.1)
- **Purpose**: Basic on/off lighting control
- **Key Clusters**: `0x0006` (On/Off), `0x0062` (Scenes Management)
- **Argument**: `"relay"` (Relay number)

### Matter_Plugin_Light1
**File**: `Matter_Plugin_3_Light1.be`
- **Type**: `"light1"`
- **Display Name**: `"Light 1 Dimmer"`
- **Matter Device Type**: `0x0101` (Dimmable Light) - **Rev 3** (Matter 1.4.1)
- **Purpose**: Dimmable lighting control
- **Additional Clusters**: `0x0008` (Level Control), `0x0062` (Scenes Management)

### Matter_Plugin_Light2
**File**: `Matter_Plugin_4_Light2.be`
- **Type**: `"light2"`
- **Display Name**: `"Light 2 Color Temp"`
- **Matter Device Type**: `0x010C` (Color Temperature Light) - **Rev 4** (Matter 1.4.1)
- **Purpose**: Color temperature control
- **Additional Clusters**: `0x0300` (Color Control with RemainingTime), `0x0062` (Scenes Management)
- **Features**: ColorTemperature mode, RemainingTime attribute

### Matter_Plugin_Light3
**File**: `Matter_Plugin_4_Light3.be`
- **Type**: `"light3"`
- **Display Name**: `"Light 3 RGB"`
- **Matter Device Type**: `0x010D` (Extended Color Light) - **Rev 4** (Matter 1.4.1)
- **Purpose**: Full RGB+CT color control
- **Additional Clusters**: `0x0300` (Color Control - HS + CT modes), `0x0062` (Scenes Management)
- **Features**: HueSaturation AND ColorTemperature modes (both required in 1.4.1), RemainingTime attribute

## Sensor Classes (Level 2+)

### Matter_Plugin_Sensor
**File**: `Matter_Plugin_2_Sensor.be`
- **Purpose**: Base class for all sensor types
- **Key Features**:
  - Sensor value filtering and matching
  - Temperature and pressure unit handling
  - JSON payload parsing

### Matter_Plugin_Sensor_Boolean
**File**: `Matter_Plugin_2_Sensor_Boolean.be`
- **Purpose**: Base class for boolean sensors (contact, occupancy, etc.)
- **Key Features**: Binary state management

## Specific Sensor Implementations

### Temperature Sensors
- **Matter_Plugin_Sensor_Temp**: `"temperature"` → Device Type `0x0302`
- **Clusters**: `0x0402` (Temperature Measurement)

### Humidity Sensors
- **Matter_Plugin_Sensor_Humidity**: `"humidity"` → Device Type `0x0307`
- **Clusters**: `0x0405` (Relative Humidity Measurement)

### Pressure Sensors
- **Matter_Plugin_Sensor_Pressure**: `"pressure"` → Device Type `0x0305`
- **Clusters**: `0x0403` (Pressure Measurement)

### Illuminance Sensors
- **Matter_Plugin_Sensor_Illuminance**: `"illuminance"` → Device Type `0x0106`
- **Clusters**: `0x0400` (Illuminance Measurement)

### Contact Sensors
- **Matter_Plugin_Sensor_Contact**: `"contact"` → Device Type `0x0015`
- **Clusters**: `0x0045` (Boolean State)

### Occupancy Sensors
- **Matter_Plugin_Sensor_Occupancy**: `"occupancy"` → Device Type `0x0107`
- **Clusters**: `0x0406` (Occupancy Sensing)

### Flow Sensors
- **Matter_Plugin_Sensor_Flow**: `"flow"` → Device Type `0x0306`
- **Clusters**: `0x0404` (Flow Measurement)

### Air Quality Sensors
- **Matter_Plugin_Sensor_Air_Quality**: `"airquality"` → Device Type `0x002C`
- **Clusters**: `0x042A` (Air Quality)

## Device Variants

### Bridge Devices (HTTP Remote)
**Prefix**: `Matter_Plugin_Bridge_*`
- **Purpose**: Control remote devices via HTTP
- **Flag**: `BRIDGE = true`
- **Type Prefix**: `"http_*"`
- **Update Time**: 3000ms (3 seconds)
- **Features**: HTTP communication, remote status polling

### Virtual Devices
**Prefix**: `Matter_Plugin_Virt_*`
- **Purpose**: Software-only devices for testing/simulation
- **Flag**: `VIRTUAL = true`
- **Type Prefix**: `"v_*"`
- **Features**: No physical hardware dependency

### Zigbee Devices
**Prefix**: `Matter_Plugin_Zigbee_*`
- **Purpose**: Bridge Zigbee devices to Matter
- **Flag**: `ZIGBEE = true`
- **Type Prefix**: `"z_*"`
- **Features**: Zigbee attribute mapping

## Other Device Types

### Matter_Plugin_Fan
- **Type**: `"fan"`
- **Display Name**: `"Fan"`
- **Matter Device Type**: `0x002B` (Fan)
- **Clusters**: `0x0202` (Fan Control)

### Matter_Plugin_OnOff
- **Type**: `"relay"`
- **Display Name**: `"Relay"`
- **Matter Device Type**: `0x010A` (On/Off Plug-in Unit)
- **Purpose**: Generic on/off control (relays, switches)

### Matter_Plugin_Shutter
- **Type**: `"shutter"`
- **Display Name**: `"Shutter"`
- **Matter Device Type**: `0x0202` (Window Covering)
- **Clusters**: `0x0102` (Window Covering)

### Matter_Plugin_ShutterTilt
- **Purpose**: Shutter with tilt control
- **Additional Features**: Tilt angle control

### Matter_Plugin_Sensor_GenericSwitch_Btn
- **Type**: `"gensw_btn"`
- **Display Name**: `"Generic Switch/Button"`
- **Matter Device Type**: `0x000F` (Generic Switch)
- **Clusters**: `0x003B` (Switch)

## Plugin Selection Logic

The Matter implementation uses a hierarchical approach for plugin selection:

1. **Base Functionality**: All plugins inherit from `Matter_Plugin`
2. **Device Category**: Plugins inherit from appropriate level-1 classes (`Root`, `Device`, etc.)
3. **Specific Function**: Plugins inherit from level-2+ classes (`Light0`, `Sensor`, etc.)
4. **Deployment Type**: Final classes specify deployment (Bridge, Virtual, Zigbee)

## Configuration Examples

### Local Light Control
```json
{
  "type": "light1",
  "relay": 1
}
```

### Bridge Light Control
```json
{
  "type": "http_light1",
  "url": "192.168.1.100",
  "relay": 1
}
```

### Virtual Sensor
```json
{
  "type": "v_temperature"
}
```

### Zigbee Device
```json
{
  "type": "z_temperature",
  "zigbee_device": "0x1234"
}
```

## Matter Device Type Mapping

| Plugin Type | Matter Device Type | Hex | Rev | Description |
|-------------|-------------------|-----|-----|-------------|
| root | Root Node | 0x0016 | 3 | Matter root device |
| aggregator | Aggregator | 0x000E | 2 | Endpoint aggregator |
| light0 | OnOff Light | 0x0100 | 3 | Basic on/off light |
| light1 | Dimmable Light | 0x0101 | 3 | Dimmable light |
| light2 | Color Temperature Light | 0x010C | 4 | CT adjustable light |
| light3 | Extended Color Light | 0x010D | 4 | Full RGB+CT light |
| relay | On/Off Plug-in Unit | 0x010A | 3 | Generic relay/switch |
| fan | Fan | 0x002B | 3 | Fan control |
| shutter | Window Covering | 0x0202 | 3 | Window covering |
| temperature | Temperature Sensor | 0x0302 | 2 | Temperature measurement |
| humidity | Humidity Sensor | 0x0307 | 2 | Humidity measurement |
| pressure | Pressure Sensor | 0x0305 | 2 | Pressure measurement |
| illuminance | Light Sensor | 0x0106 | 3 | Illuminance measurement |
| contact | Contact Sensor | 0x0015 | 2 | Contact/door sensor |
| occupancy | Occupancy Sensor | 0x0107 | 4 | Motion/occupancy sensor |
| flow | Flow Sensor | 0x0306 | 2 | Flow measurement |
| airquality | Air Quality Sensor | 0x002C | 1 | Air quality measurement |
| gensw_btn | Generic Switch | 0x000F | 3 | Button/switch |

## Cluster Support Matrix

### Common Clusters (All Devices)
- **0x001D**: Descriptor Cluster (device identification)

### Device-Specific Clusters
- **0x0006**: On/Off (lights, relays, switches)
- **0x0008**: Level Control (dimmable lights)
- **0x0300**: Color Control (color lights)
- **0x0202**: Fan Control (fans)
- **0x0102**: Window Covering (shutters)
- **0x0402**: Temperature Measurement (temperature sensors)
- **0x0405**: Relative Humidity Measurement (humidity sensors)
- **0x0403**: Pressure Measurement (pressure sensors)
- **0x0400**: Illuminance Measurement (light sensors)
- **0x0045**: Boolean State (contact sensors)
- **0x0406**: Occupancy Sensing (occupancy sensors)
- **0x0404**: Flow Measurement (flow sensors)
- **0x042A**: Air Quality (air quality sensors)
- **0x003B**: Switch (buttons/switches)

### Infrastructure Clusters (Root Device)
- **0x001F**: Access Control
- **0x0028**: Basic Information
- **0x0030**: General Commissioning
- **0x0031**: Network Commissioning
- **0x003C**: Administrator Commissioning
- **0x003E**: Node Operational Credentials

This hierarchy provides a flexible and extensible framework for implementing Matter devices across different deployment scenarios while maintaining code reuse and consistent behavior patterns.

---
*Documentation generated: June 27, 2025*
*Based on Berry Matter implementation analysis*
