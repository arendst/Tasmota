# Matter 1.4.1 Detailed Gap Analysis for Tasmota Implementation

## Document Information
- **Analysis Date**: January 2026
- **Matter Spec Version**: 1.4.1 (March 17, 2025)
- **Tasmota Implementation**: Berry Matter module
- **Purpose**: Identify gaps between Matter 1.4.1 specification and current Tasmota implementation

---

## Executive Summary

The Tasmota Matter implementation is well-architected and provides solid support for Matter 1.0/1.1/1.2 device types. The current `DataModelRevision` returns `17` (Matter 1.2). This analysis identifies gaps against Matter 1.4.1 specification, prioritized by implementation value for Tasmota users.

**Key Findings:**
1. **HIGH PRIORITY**: Energy measurement clusters (0x0090, 0x0091) - Tasmota has extensive energy monitoring but not exposed via Matter
2. **HIGH PRIORITY**: Thermostat cluster (0x0201) - Referenced in code but no plugin exists
3. **MEDIUM PRIORITY**: Power Source cluster (0x002F) - Important for battery devices
4. **MEDIUM PRIORITY**: Door Lock cluster (0x0101) - Common device type not implemented
5. **LOW PRIORITY**: Various Matter 1.4 new device types (Water Heater, EV Charger, etc.)

---

## Part 1: Device Type Comparison

### 1.1 Currently Implemented Device Types

| Device Type | ID | Plugin File | Status | Notes |
|-------------|-----|-------------|--------|-------|
| Root Node | 0x0016 | `Matter_Plugin_1_Root.be` | ✅ Complete | All infrastructure clusters |
| Aggregator | 0xFF00 | `Matter_Plugin_1_Aggregator.be` | ✅ Complete | Bridge mode support |
| On/Off Light | 0x0100 | `Matter_Plugin_2_Light0.be` | ✅ Complete | |
| Dimmable Light | 0x0101 | `Matter_Plugin_3_Light1.be` | ✅ Complete | |
| Color Temperature Light | 0x010C | `Matter_Plugin_4_Light2.be` | ✅ Complete | |
| Extended Color Light | 0x010D | `Matter_Plugin_4_Light3.be` | ✅ Complete | |
| On/Off Plug-in Unit | 0x010A | `Matter_Plugin_3_OnOff.be` | ✅ Complete | |
| Fan | 0x002B | `Matter_Plugin_2_Fan.be` | ✅ Complete | |
| Window Covering | 0x0202 | `Matter_Plugin_2_Shutter.be` | ✅ Complete | Lift + Tilt |
| Temperature Sensor | 0x0302 | `Matter_Plugin_3_Sensor_Temp.be` | ✅ Complete | |
| Humidity Sensor | 0x0307 | `Matter_Plugin_3_Sensor_Humidity.be` | ✅ Complete | |
| Pressure Sensor | 0x0305 | `Matter_Plugin_3_Sensor_Pressure.be` | ✅ Complete | |
| Illuminance Sensor | 0x0106 | `Matter_Plugin_3_Sensor_Illuminance.be` | ✅ Complete | |
| Occupancy Sensor | 0x0107 | `Matter_Plugin_3_Sensor_Occupancy.be` | ✅ Complete | |
| Contact Sensor | 0x0015 | `Matter_Plugin_3_Sensor_Contact.be` | ✅ Complete | |
| Flow Sensor | 0x0306 | `Matter_Plugin_3_Sensor_Flow.be` | ✅ Complete | |
| Generic Switch | 0x000F | `Matter_Plugin_2_Sensor_GenericSwitch_Btn.be` | ✅ Complete | Button |
| Air Quality Sensor | 0x002C | `Matter_Plugin_2_Sensor_Air_Quality.be` | ✅ Complete | |
| Rain Sensor | 0x0044 | `Matter_Plugin_3_Sensor_Rain.be` | ✅ Complete | |
| Water Leak Detector | 0x0043 | `Matter_Plugin_3_Sensor_Waterleak.be` | ✅ Complete | |

### 1.2 Device Types NOT Implemented (Matter 1.4.1 Spec)

#### HIGH PRIORITY (High value for Tasmota users)

| Device Type | ID | Priority | Reason |
|-------------|-----|----------|--------|
| **Thermostat** | 0x0301 | 🔴 HIGH | Tasmota has thermostat support, cluster 0x0201 referenced in code |
| **Electrical Sensor** | 0x0510 | 🔴 HIGH | New in Matter 1.4, Tasmota has extensive energy monitoring |
| **Door Lock** | 0x000A | 🔴 HIGH | Common device type, cluster 0x0101 revision defined |
| **Dimmable Plug-In Unit** | 0x010B | 🟡 MEDIUM | Extension of existing OnOff plugin |
| **Pump** | 0x0303 | 🟡 MEDIUM | Tasmota supports pump control |

#### MEDIUM PRIORITY (Matter 1.4 new features)

| Device Type | ID | Priority | Reason |
|-------------|-----|----------|--------|
| **Water Heater** | 0x050F | 🟡 MEDIUM | Matter 1.4 device type |
| **Air Purifier** | 0x002D | 🟡 MEDIUM | Fan + filter monitoring |
| **Smoke CO Alarm** | 0x0076 | 🟡 MEDIUM | Safety device |
| **Water Freeze Detector** | 0x0041 | 🟡 MEDIUM | Boolean state device |
| **Water Valve** | 0x0042 | 🟡 MEDIUM | Valve control |

#### LOW PRIORITY (Specialized/Complex)

| Device Type | ID | Priority | Reason |
|-------------|-----|----------|--------|
| EV Charger (EVSE) | 0x050C | 🟢 LOW | Complex, specialized hardware |
| Battery Storage | - | 🟢 LOW | Matter 1.4, specialized |
| Solar Power | - | 🟢 LOW | Matter 1.4, specialized |
| Heat Pump | - | 🟢 LOW | Matter 1.4, complex HVAC |
| Video Player | 0x0023/0x0028 | 🟢 LOW | Media devices, complex |
| Speaker | 0x0022 | 🟢 LOW | Media device |
| Robotic Vacuum | 0x0074 | 🟢 LOW | Complex operational states |

---

## Part 2: Cluster Comparison

### 2.1 Currently Implemented Clusters

#### Infrastructure Clusters (Root Node)

| Cluster | ID | Status | Revision | Notes |
|---------|-----|--------|----------|-------|
| Descriptor | 0x001D | ✅ | 2 | |
| Access Control | 0x001F | ✅ | 1 | |
| Basic Information | 0x0028 | ✅ | 2 | DataModelRevision=17 |
| General Commissioning | 0x0030 | ✅ | 1 | |
| Network Commissioning | 0x0031 | ⚠️ Partial | 1 | Limited attributes |
| Diagnostic Logs | 0x0032 | ⚠️ Stub | 1 | No attributes |
| General Diagnostics | 0x0033 | ✅ | 1 | |
| Software Diagnostics | 0x0034 | ⚠️ Stub | 1 | No attributes |
| Time Synchronization | 0x0038 | ⚠️ Partial | 2 | Basic time only |
| Administrator Commissioning | 0x003C | ✅ | 1 | |
| Node Operational Credentials | 0x003E | ✅ | 1 | |
| Group Key Management | 0x003F | ⚠️ Stub | 2 | TODO in code |
| ICD Management | 0x0046 | ✅ | 3 | SIT mode, no CIP/LITS |
| Localization Configuration | 0x002B | ✅ | 1 | |
| Time Format Localization | 0x002C | ✅ | 1 | |
| OTA Software Update Requestor | 0x002A | ⚠️ Stub | 1 | Returns empty/false |

#### Application Clusters

| Cluster | ID | Status | Revision | Notes |
|---------|-----|--------|----------|-------|
| Identify | 0x0003 | ✅ | 4 | |
| Groups | 0x0004 | ✅ | 4 | |
| Scenes Management | 0x0005/0x0062 | ✅ | 5 | |
| On/Off | 0x0006 | ✅ | 5 | Dead Front behavior |
| Level Control | 0x0008 | ✅ | 5 | On/Off + Lighting features |
| Color Control | 0x0300 | ✅ | 6 | |
| Window Covering | 0x0102 | ✅ | 5 | Lift + PA_LF features |
| Fan Control | 0x0202 | ✅ | 4 | Auto feature |
| Boolean State | 0x0045 | ✅ | 1 | |
| Switch | 0x003B | ✅ | 1 | |
| Temperature Measurement | 0x0402 | ✅ | 4 | |
| Pressure Measurement | 0x0403 | ✅ | 3 | |
| Flow Measurement | 0x0404 | ✅ | 3 | |
| Relative Humidity Measurement | 0x0405 | ✅ | 3 | |
| Occupancy Sensing | 0x0406 | ✅ | 3 | |
| Illuminance Measurement | 0x0400 | ✅ | 3 | |
| Air Quality | 0x005B | ✅ | 1 | |

### 2.2 Clusters NOT Implemented (Gap Analysis)

#### HIGH PRIORITY - Energy Clusters (Matter 1.4)

| Cluster | ID | Priority | Description | Implementation Notes |
|---------|-----|----------|-------------|---------------------|
| **Electrical Power Measurement** | 0x0090 | 🔴 HIGH | Real-time power (W, V, A) | Map to Tasmota energy drivers |
| **Electrical Energy Measurement** | 0x0091 | 🔴 HIGH | Cumulative energy (kWh) | Map to Tasmota energy drivers |

**Tasmota Energy Sources:**
- PZEM-004T, PZEM-016 (Modbus energy meters)
- HLW8012, BL0937, BL0940 (Power monitoring chips)
- ADE7953 (Shelly devices)
- CSE7766 (Sonoff POW)
- SDM120, SDM630 (Modbus meters)

#### HIGH PRIORITY - HVAC Clusters

| Cluster | ID | Priority | Description | Implementation Notes |
|---------|-----|----------|-------------|---------------------|
| **Thermostat** | 0x0201 | 🔴 HIGH | HVAC control | Revision 6 defined in code |
| Thermostat UI Configuration | 0x0204 | 🟡 MEDIUM | Display settings | Revision 2 defined |
| Pump Configuration | 0x0200 | 🟡 MEDIUM | Pump control | Revision 4 defined |

#### HIGH PRIORITY - Closure Clusters

| Cluster | ID | Priority | Description | Implementation Notes |
|---------|-----|----------|-------------|---------------------|
| **Door Lock** | 0x0101 | 🔴 HIGH | Lock/unlock control | Revision 7 defined in code |

#### MEDIUM PRIORITY - Utility Clusters

| Cluster | ID | Priority | Description | Implementation Notes |
|---------|-----|----------|-------------|---------------------|
| **Power Source** | 0x002F | 🟡 MEDIUM | Battery/wired status | Important for portable devices |
| Boolean State Configuration | 0x0080 | 🟡 MEDIUM | Sensor configuration | For water/freeze detectors |
| Valve Configuration | 0x0081 | 🟡 MEDIUM | Valve control | For water valve device |

#### MEDIUM PRIORITY - Matter 1.4 Energy Management

| Cluster | ID | Priority | Description | Implementation Notes |
|---------|-----|----------|-------------|---------------------|
| Device Energy Management | 0x0098 | 🟡 MEDIUM | Energy optimization | Matter 1.4 |
| Device Energy Management Mode | 0x009F | 🟡 MEDIUM | Energy modes | Matter 1.4 |
| Power Topology | 0x009C | 🟡 MEDIUM | Power distribution | Matter 1.4 |
| Energy Preference | 0x009B | 🟡 MEDIUM | User preferences | Matter 1.4 |

#### LOW PRIORITY - Safety/Alarm Clusters

| Cluster | ID | Priority | Description |
|---------|-----|----------|-------------|
| Smoke CO Alarm | 0x005C | 🟢 LOW | Smoke/CO detection |
| Carbon Monoxide Concentration | 0x040C | 🟢 LOW | CO measurement |
| Carbon Dioxide Concentration | 0x040D | 🟢 LOW | CO2 measurement |

#### LOW PRIORITY - Media Clusters

| Cluster | ID | Priority | Description |
|---------|-----|----------|-------------|
| Media Playback | 0x0506 | 🟢 LOW | Play/pause/etc |
| Keypad Input | 0x0509 | 🟢 LOW | Remote control |
| Content Launcher | 0x050A | 🟢 LOW | Launch content |
| Audio Output | 0x050B | 🟢 LOW | Audio routing |
| Channel | 0x0504 | 🟢 LOW | TV channels |

---

## Part 3: Detailed Implementation Requirements

### 3.1 Electrical Power Measurement Cluster (0x0090) - HIGH PRIORITY

**Spec Reference:** Matter 1.4.1 Application Cluster Spec, Section 2.13

**Required Attributes:**
| Attribute | ID | Type | Conformance | Tasmota Mapping |
|-----------|-----|------|-------------|-----------------|
| PowerMode | 0x0000 | enum8 | M | AC for most devices |
| NumberOfMeasurementTypes | 0x0001 | uint8 | M | Count of supported measurements |
| Accuracy | 0x0002 | list[MeasurementAccuracyStruct] | M | From driver specs |
| ActivePower | 0x0008 | int64 (mW) | O | `Energy.Power` * 1000 |
| ReactivePower | 0x000C | int64 (mVAR) | O | `Energy.ReactivePower` * 1000 |
| ApparentPower | 0x0010 | int64 (mVA) | O | `Energy.ApparentPower` * 1000 |
| Voltage | 0x0004 | int64 (mV) | O | `Energy.Voltage` * 1000 |
| Current | 0x0005 | int64 (mA) | O | `Energy.Current` * 1000 |
| Frequency | 0x0006 | int64 (mHz) | O | `Energy.Frequency` * 1000 |
| PowerFactor | 0x0014 | int64 (1/100) | O | `Energy.Factor` * 100 |

**Features (FeatureMap 0xFFFC):**
- DIRC (0x01): Directional measurement
- HARM (0x02): Harmonics measurement
- PWRQ (0x04): Power quality
- POLY (0x08): Polyphase measurement

**Recommended Implementation:**
```berry
# New file: Matter_Plugin_Sensor_Power.be
class Matter_Plugin_Sensor_Power : Matter_Plugin_Sensor
  static var TYPE = "power"
  static var DISPLAY_NAME = "Power Sensor"
  static var TYPES = { 0x0510: 1 }  # Electrical Sensor device type
  static var CLUSTERS = matter.consolidate_clusters(_class, {
    0x0090: [0,1,2,4,5,8,0x14],  # Electrical Power Measurement
  })
  # ... implementation
end
```

### 3.2 Electrical Energy Measurement Cluster (0x0091) - HIGH PRIORITY

**Spec Reference:** Matter 1.4.1 Application Cluster Spec, Section 2.12

**Required Attributes:**
| Attribute | ID | Type | Conformance | Tasmota Mapping |
|-----------|-----|------|-------------|-----------------|
| Accuracy | 0x0000 | MeasurementAccuracyStruct | M | From driver specs |
| CumulativeEnergyImported | 0x0001 | EnergyMeasurementStruct | O | `Energy.Total` |
| CumulativeEnergyExported | 0x0002 | EnergyMeasurementStruct | O | `Energy.ExportActive` |
| PeriodicEnergyImported | 0x0003 | EnergyMeasurementStruct | O | `Energy.Today` |
| PeriodicEnergyExported | 0x0004 | EnergyMeasurementStruct | O | Calculated |
| CumulativeEnergyReset | 0x0005 | CumulativeEnergyResetStruct | O | Reset timestamp |

**EnergyMeasurementStruct:**
```
{
  Energy: int64 (mWh),
  StartTimestamp: epoch_s (optional),
  EndTimestamp: epoch_s (optional),
  StartSystime: uint64 (optional),
  EndSystime: uint64 (optional)
}
```

**Features:**
- IMPE (0x01): Import energy measurement
- EXPE (0x02): Export energy measurement
- CUME (0x04): Cumulative energy
- PERE (0x08): Periodic energy

**Recommended Implementation:**
```berry
# New file: Matter_Plugin_Sensor_Energy.be
class Matter_Plugin_Sensor_Energy : Matter_Plugin_Sensor
  static var TYPE = "energy"
  static var DISPLAY_NAME = "Energy Sensor"
  static var TYPES = { 0x0510: 1 }  # Electrical Sensor device type
  static var CLUSTERS = matter.consolidate_clusters(_class, {
    0x0091: [0,1,3],  # Electrical Energy Measurement
  })
  # ... implementation
end
```

### 3.3 Thermostat Cluster (0x0201) - HIGH PRIORITY

**Spec Reference:** Matter 1.4.1 Application Cluster Spec, Section 4.3

**Required Attributes:**
| Attribute | ID | Type | Conformance | Tasmota Mapping |
|-----------|-----|------|-------------|-----------------|
| LocalTemperature | 0x0000 | int16 (0.01°C) | M | Current temp sensor |
| OccupiedCoolingSetpoint | 0x0011 | int16 | COOL | Cooling setpoint |
| OccupiedHeatingSetpoint | 0x0012 | int16 | HEAT | Heating setpoint |
| SystemMode | 0x001C | enum8 | M | Off/Auto/Cool/Heat |
| ThermostatRunningState | 0x0029 | bitmap16 | O | Current HVAC state |
| ControlSequenceOfOperation | 0x001B | enum8 | M | Cooling/Heating/Both |

**Features:**
- HEAT (0x01): Heating capability
- COOL (0x02): Cooling capability
- OCC (0x04): Occupancy support
- SCH (0x08): Schedule support (deprecated in Matter 1.4)
- SB (0x10): Setback support
- AUTO (0x20): Auto mode
- LTNE (0x40): Local temperature not exposed

**Commands:**
| Command | ID | Description |
|---------|-----|-------------|
| SetpointRaiseLower | 0x00 | Adjust setpoint |

**Recommended Implementation:**
```berry
# New file: Matter_Plugin_Thermostat.be
class Matter_Plugin_Thermostat : Matter_Plugin_Device
  static var TYPE = "thermostat"
  static var DISPLAY_NAME = "Thermostat"
  static var TYPES = { 0x0301: 4 }  # Thermostat device type rev 4
  static var CLUSTERS = matter.consolidate_clusters(_class, {
    0x0003: [0,1],           # Identify
    0x0201: [0,0x11,0x12,0x1B,0x1C,0x29],  # Thermostat
  })
  static var FEATURE_MAPS = {
    0x0201: 0x23,  # HEAT + COOL + AUTO
  }
  # ... implementation
end
```

### 3.4 Door Lock Cluster (0x0101) - HIGH PRIORITY

**Spec Reference:** Matter 1.4.1 Application Cluster Spec, Section 5.2

**Required Attributes:**
| Attribute | ID | Type | Conformance | Description |
|-----------|-----|------|-------------|-------------|
| LockState | 0x0000 | enum8 | M | NotFullyLocked/Locked/Unlocked/Unlatched |
| LockType | 0x0001 | enum8 | M | DeadBolt/Magnetic/etc |
| ActuatorEnabled | 0x0002 | bool | M | Can actuate |
| OperatingMode | 0x0025 | enum8 | M | Normal/Vacation/Privacy/etc |
| SupportedOperatingModes | 0x0026 | bitmap16 | M | Supported modes |

**Commands:**
| Command | ID | Description |
|---------|-----|-------------|
| LockDoor | 0x00 | Lock the door |
| UnlockDoor | 0x01 | Unlock the door |
| UnlockWithTimeout | 0x03 | Unlock with auto-relock |

**Features:**
- PIN (0x01): PIN credential
- RID (0x02): RFID credential
- FPG (0x04): Fingerprint
- LOG (0x08): Logging
- WDSCH (0x10): Week day schedules
- DPS (0x20): Door position sensor
- FACE (0x40): Face recognition
- COTA (0x80): Credential over-the-air
- USR (0x100): User management
- NOT (0x200): Notification
- YDSCH (0x400): Year day schedules
- HDSCH (0x800): Holiday schedules
- UBOLT (0x1000): Unbolt feature

### 3.5 Power Source Cluster (0x002F) - MEDIUM PRIORITY

**Spec Reference:** Matter Core Spec, Section 11.7

**Required Attributes:**
| Attribute | ID | Type | Conformance | Description |
|-----------|-----|------|-------------|-------------|
| Status | 0x0000 | enum8 | M | Unspecified/Active/Standby/Unavailable |
| Order | 0x0001 | uint8 | M | Priority order |
| Description | 0x0002 | string | M | Human readable |
| BatChargeLevel | 0x000E | enum8 | BAT | OK/Warning/Critical |
| BatPercentRemaining | 0x000C | uint8 | BAT | 0-200 (0.5% steps) |
| BatVoltage | 0x000B | uint32 | BAT | mV |

**Features:**
- WIRED (0x01): Wired power source
- BAT (0x02): Battery power source
- RECHG (0x04): Rechargeable battery
- REPLC (0x08): Replaceable battery

---

## Part 4: Implementation Roadmap

### Phase 1: High Priority (Immediate Value)

1. **Energy Measurement Plugins** (2-3 weeks)
   - Create `Matter_Plugin_Sensor_Power.be`
   - Create `Matter_Plugin_Sensor_Energy.be`
   - Map to existing Tasmota energy drivers
   - Test with PZEM, HLW8012, ADE7953

2. **Thermostat Plugin** (1-2 weeks)
   - Create `Matter_Plugin_Thermostat.be`
   - Map to Tasmota thermostat commands
   - Support heating/cooling modes

3. **Door Lock Plugin** (1 week)
   - Create `Matter_Plugin_Lock.be`
   - Basic lock/unlock functionality
   - Map to Tasmota relay or GPIO

### Phase 2: Medium Priority (Enhanced Features)

4. **Power Source Cluster** (1 week)
   - Add to Root node for battery devices
   - Support wired and battery modes

5. **Complete OTA Cluster** (2 weeks)
   - Enable Matter-based firmware updates
   - Integrate with Tasmota OTA system

6. **Water Valve Device** (1 week)
   - Create `Matter_Plugin_Valve.be`
   - Map to Tasmota relay

### Phase 3: Low Priority (Future)

7. **Matter 1.4 Energy Management**
   - Device Energy Management cluster
   - Energy optimization features

8. **Media Device Types**
   - Video player support
   - Speaker support

---

## Part 5: Code Changes Summary

### New Files Required

| File | Purpose | Priority |
|------|---------|----------|
| `Matter_Plugin_Sensor_Power.be` | Electrical Power Measurement | HIGH |
| `Matter_Plugin_Sensor_Energy.be` | Electrical Energy Measurement | HIGH |
| `Matter_Plugin_Thermostat.be` | Thermostat device | HIGH |
| `Matter_Plugin_Lock.be` | Door Lock device | HIGH |
| `Matter_Plugin_Valve.be` | Water Valve device | MEDIUM |

### Modifications to Existing Files

| File | Change | Priority |
|------|--------|----------|
| `Matter_Plugin_0.be` | Add cluster revisions for new clusters | HIGH |
| `Matter_Plugin_1_Root.be` | Add Power Source cluster (optional) | MEDIUM |
| `Matter_Plugin_z_All.be` | Register new plugin classes | HIGH |
| `Matter_z_Autoconf.be` | Auto-detect energy devices | HIGH |

### Cluster Revision Updates

Add to `CLUSTER_REVISIONS` in `Matter_Plugin_0.be`:
```berry
0x002F: 2,  # Power Source
0x0090: 1,  # Electrical Power Measurement
0x0091: 1,  # Electrical Energy Measurement
0x0080: 1,  # Boolean State Configuration
0x0081: 1,  # Valve Configuration and Control
```

### Feature Map Updates

Add to `FEATURE_MAPS` in `Matter_Plugin_0.be`:
```berry
0x0090: 0x00,  # Electrical Power Measurement (base features)
0x0091: 0x05,  # Electrical Energy Measurement (IMPE + CUME)
0x0201: 0x23,  # Thermostat (HEAT + COOL + AUTO)
0x0101: 0x00,  # Door Lock (base features)
0x002F: 0x01,  # Power Source (WIRED)
```

---

## Part 6: DataModel Revision Update

Current implementation returns `DataModelRevision = 17` (Matter 1.2).

To claim Matter 1.4.1 compliance, update to `DataModelRevision = 18` after implementing:
- All mandatory cluster revisions
- Required attribute updates
- New cluster support

**Location:** `Matter_Plugin_1_Root.be`, line ~230:
```berry
# Current:
return tlv_solo.set(TLV.U2, 17)     # 17 = Matter 1.2

# After full 1.4.1 compliance:
return tlv_solo.set(TLV.U2, 18)     # 18 = Matter 1.4
```

---

## Appendix A: Tasmota Energy Driver Mapping

| Tasmota Driver | Supported Measurements | Matter Cluster Mapping |
|----------------|----------------------|----------------------|
| PZEM-004T | V, A, W, Wh, Hz, PF | 0x0090 + 0x0091 |
| HLW8012 | V, A, W, Wh | 0x0090 + 0x0091 |
| BL0937 | V, A, W, Wh | 0x0090 + 0x0091 |
| BL0940 | V, A, W, Wh | 0x0090 + 0x0091 |
| ADE7953 | V, A, W, Wh, VAR, VA, PF | 0x0090 + 0x0091 |
| CSE7766 | V, A, W, Wh | 0x0090 + 0x0091 |
| SDM120 | V, A, W, Wh, Hz, PF, VA, VAR | 0x0090 + 0x0091 |
| SDM630 | 3-phase V, A, W, Wh, Hz, PF | 0x0090 + 0x0091 (POLY) |

**Tasmota Energy JSON Structure:**
```json
{
  "Energy": {
    "TotalStartTime": "2023-01-01T00:00:00",
    "Total": 123.456,
    "Yesterday": 1.234,
    "Today": 0.567,
    "Power": 150,
    "ApparentPower": 160,
    "ReactivePower": 50,
    "Factor": 0.94,
    "Voltage": 230,
    "Current": 0.65,
    "Frequency": 50.0
  }
}
```

---

## Appendix B: Testing Recommendations

### Test Controllers
- Apple Home (iOS 16+)
- Google Home
- Amazon Alexa
- Samsung SmartThings
- Home Assistant (Matter integration)

### Test Scenarios

1. **Energy Measurement**
   - Verify power readings update in real-time
   - Check energy accumulation over time
   - Test with multiple energy drivers

2. **Thermostat**
   - Test heating/cooling mode switching
   - Verify setpoint changes
   - Check temperature reporting

3. **Door Lock**
   - Test lock/unlock commands
   - Verify state reporting
   - Test with physical button override

---

*Document Version: 1.0*
*Last Updated: January 2026*
