# Matter 1.4.1 Device Library Specification (Compact)
Version: 1.4.1 | Date: 2025-03-17 | Source: 23-27351-006

## Notation

### C/S Column (Client/Server)
- S = Server cluster (device implements the cluster)
- C = Client cluster (device can invoke commands on other devices)

### Conformance Column
- M = Mandatory (must implement)
- O = Optional (may implement)
- P = Provisional (not yet certifiable)
- X = Prohibited (must not implement)
- D = Deprecated (should not implement in new designs)
- [Condition] = Required if condition is true
- O.a+ = At least one of the group marked with same letter required
- desc = See description for details
- ! = Negation (e.g., !CustomNetworkConfig = required if condition is false)
- | = OR (e.g., SIT | LIT = required if either condition is true)
- & = AND (e.g., Matter & PIN = required if both conditions are true)

## 1. Base Device Type
All device types inherit these requirements.

### 1.1 Conditions
**Certification**: Matter, Zigbee, ZHA, ZSE, GP, SuZi
**Protocol**: Ethernet, Wi-Fi, Thread, TCP, UDP, IP, IPv4, IPv6
**Interface**: LanguageLocale, TimeLocale, UnitLocale
**Capability**: SIT (Short Idle Time ICD), LIT (Long Idle Time ICD), Active
**Device Class**: Node, App, Simple, Dynamic, Composed
**Endpoint Class**: Client, Server, Duplicate, BridgedPowerSourceInfo

### 1.2 Base Clusters (Matter)
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x001D | Descriptor | S | M |
| 0x001E | Binding | S | Simple & Client |
| 0x0040 | Fixed Label | S | O |
| 0x0041 | User Label | S | O |

### 1.3 Element Override
Descriptor.TagList feature: Required if Duplicate condition

---
## 2. Utility Device Types

### 2.1 Root Node (0x0016)
Class: Node | Scope: Node | Rev: 3

**Conditions**: CustomNetworkConfig, ManagedAclAllowed

**Device Type Requirements**:
| ID | Name | Conformance |
|----|------|-------------|
| 0x0011 | Power Source | O |

**Clusters**:
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0028 | Basic Information | S | M |
| 0x001F | Access Control | S | M |
| 0x002E | Power Source Configuration | S | O, D |
| 0x0038 | Time Synchronization | S | O |
| 0x003F | Group Key Management | S | M |
| 0x0030 | General Commissioning | S | M |
| 0x0031 | Network Commissioning | S | !CustomNetworkConfig |
| 0x003C | Administrator Commissioning | S | M |
| 0x003E | Node Operational Credentials | S | M |
| 0x002B | Localization Configuration | S | LanguageLocale |
| 0x002C | Time Format Localization | S | TimeLocale |
| 0x002D | Unit Localization | S | UnitLocale |
| 0x0033 | General Diagnostics | S | M |
| 0x0032 | Diagnostic Logs | S | O |
| 0x0034 | Software Diagnostics | S | O |
| 0x0037 | Ethernet Network Diagnostics | S | [Ethernet] |
| 0x0036 | Wi-Fi Network Diagnostics | S | [Wi-Fi] |
| 0x0035 | Thread Network Diagnostics | S | [Thread] |
| 0x0046 | ICD Management | S | SIT \| LIT |

**Element Overrides**:
- Access Control: MNGD feature desc [ManagedAclAllowed]
- ICD Management: LongIdleTimeSupport feature [LIT]

**Composition**: PartsList contains all other endpoints (full-family pattern)

### 2.2 Power Source (0x0011)
Class: Utility | Scope: Node | Rev: 1
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x002F | Power Source | S | M |

### 2.3 OTA Requestor (0x0012)
Class: Utility | Scope: Node | Rev: 1
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x002A | OTA Software Update Requestor | S | M |
| 0x0029 | OTA Software Update Provider | C | M |

### 2.4 OTA Provider (0x0014)
Class: Utility | Scope: Node | Rev: 1
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x002A | OTA Software Update Requestor | C | O |
| 0x0029 | OTA Software Update Provider | S | M |

### 2.5 Bridged Node (0x0013)
Class: Utility | Scope: Endpoint | Rev: 3
**Conditions**: FabricSynchronizedNode (Commissioner Control with FabricSync bit + Matter Node)

**Device Type Requirements**:
| ID | Name | Conformance |
|----|------|-------------|
| 0x0011 | Power Source | O |

**Clusters**:
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0039 | Bridged Device Basic Information | S | M |
| 0x002E | Power Source Configuration | S | BridgedPowerSourceInfo, D |
| 0x002F | Power Source | S | BridgedPowerSourceInfo |
| 0x0750 | Ecosystem Information | S | FabricSynchronizedNode, O |
| 0x003C | Administrator Commissioning | S | FabricSynchronizedNode |

**Composition**: Separate Endpoints or One Endpoint pattern

### 2.6 Electrical Sensor (0x0510)
Class: Utility | Scope: Endpoint | Rev: 1
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x009C | Power Topology | S | M |
| 0x0090 | Electrical Power Measurement | S | O.a+ |
| 0x0091 | Electrical Energy Measurement | S | O.a+ |

### 2.7 Device Energy Management (0x050D)
Class: Utility | Scope: Endpoint | Rev: 2
**Conditions**: ControllableESA (accepts adjustment commands)

| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0098 | Device Energy Management | S | M |
| 0x009F | Device Energy Management Mode | S | ControllableESA, O |

**Element Overrides** (DEM cluster features for ControllableESA.a+):
PowerAdjustment, StartTimeAdjustment, Pausable, ForecastAdjustment, ConstraintBasedAdjustment

### 2.8 Secondary Network Interface (0x0019)
Class: Utility | Scope: Endpoint | Rev: 1
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0031 | Network Commissioning | S | M |
| 0x0037 | Ethernet Network Diagnostics | S | [Ethernet] |
| 0x0036 | Wi-Fi Network Diagnostics | S | [Wi-Fi] |
| 0x0035 | Thread Network Diagnostics | S | [Thread] |

### 2.9 Joint Fabric Administrator (0x0130)
Class: Utility | Scope: Endpoint | Rev: 1
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0752 | Joint Fabric Datastore | S | M |
| 0x0753 | Joint Fabric PKI | S | M |

---
## 3. Application Device Types Summary

| Category | ID | Device Name |
|----------|-----|-------------|
| Lighting | 0x0100 | On/Off Light |
| Lighting | 0x0101 | Dimmable Light |
| Lighting | 0x010C | Color Temperature Light |
| Lighting | 0x010D | Extended Color Light |
| Plugs/Actuators | 0x010A | On/Off Plug-in Unit |
| Plugs/Actuators | 0x010B | Dimmable Plug-In Unit |
| Plugs/Actuators | 0x010F | Mounted On/Off Control |
| Plugs/Actuators | 0x0110 | Mounted Dimmable Load Control |
| Plugs/Actuators | 0x0303 | Pump |
| Plugs/Actuators | 0x0042 | Water Valve |
| Switches | 0x0103 | On/Off Light Switch |
| Switches | 0x0104 | Dimmer Switch |
| Switches | 0x0105 | Color Dimmer Switch |
| Switches | 0x0840 | Control Bridge |
| Switches | 0x0304 | Pump Controller |
| Switches | 0x000F | Generic Switch |
| Sensors | 0x0015 | Contact Sensor |
| Sensors | 0x0106 | Light Sensor |
| Sensors | 0x0107 | Occupancy Sensor |
| Sensors | 0x0302 | Temperature Sensor |
| Sensors | 0x0305 | Pressure Sensor |
| Sensors | 0x0306 | Flow Sensor |
| Sensors | 0x0307 | Humidity Sensor |
| Sensors | 0x0850 | On/Off Sensor |
| Sensors | 0x0076 | Smoke CO Alarm |
| Sensors | 0x002C | Air Quality Sensor |
| Sensors | 0x0041 | Water Freeze Detector |
| Sensors | 0x0043 | Water Leak Detector |
| Sensors | 0x0044 | Rain Sensor |
| Closures | 0x000A | Door Lock |
| Closures | 0x000B | Door Lock Controller |
| Closures | 0x0202 | Window Covering |
| Closures | 0x0203 | Window Covering Controller |
| HVAC | 0x0301 | Thermostat |
| HVAC | 0x002B | Fan |
| HVAC | 0x002D | Air Purifier |
| Media | 0x0028 | Basic Video Player |
| Media | 0x0023 | Casting Video Player |
| Media | 0x0022 | Speaker |
| Media | 0x0024 | Content App |
| Media | 0x0029 | Casting Video Client |
| Media | 0x002A | Video Remote Control |
| Generic | 0x0027 | Mode Select |
| Generic | 0x000E | Aggregator |
| Robotic | 0x0074 | Robotic Vacuum Cleaner |
| Appliances | 0x0073 | Laundry Washer |
| Appliances | 0x0070 | Refrigerator |
| Appliances | 0x0072 | Room Air Conditioner |
| Appliances | 0x0071 | Temperature Controlled Cabinet |
| Appliances | 0x0075 | Dishwasher |
| Appliances | 0x007C | Laundry Dryer |
| Appliances | 0x0077 | Cook Surface |
| Appliances | 0x0078 | Cooktop |
| Appliances | 0x007B | Oven |
| Appliances | 0x007A | Extractor Hood |
| Appliances | 0x0079 | Microwave Oven |
| Energy | 0x050C | EVSE |
| Energy | 0x050F | Water Heater |
| Energy | 0x0017 | Solar Power |
| Energy | 0x0018 | Battery Storage |
| Energy | 0x0309 | Heat Pump |
| Network | 0x0090 | Network Infrastructure Manager |
| Network | 0x0091 | Thread Border Router |

---
## 4. Lighting Device Types

### 4.1 On/Off Light (0x0100)
Class: Simple | Scope: Endpoint | Rev: 3 | Superset: -
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | M |
| 0x0004 | Groups | S | M |
| 0x0062 | Scenes Management | S | P, M |
| 0x0006 | On/Off | S | M |
| 0x0008 | Level Control | S | O |
| 0x0406 | Occupancy Sensing | C | O |

**Element Overrides**:
- Identify: TriggerEffect cmd M
- Scenes Management: CopyScene cmd P, M
- On/Off: Lighting feature M
- Level Control: OnOff feature M, Lighting feature M, CurrentLevel 1-254, MinLevel 1, MaxLevel 254

### 4.2 Dimmable Light (0x0101)
Class: Simple | Scope: Endpoint | Rev: 3 | Superset: On/Off Light
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | M |
| 0x0004 | Groups | S | M |
| 0x0062 | Scenes Management | S | P, M |
| 0x0006 | On/Off | S | M |
| 0x0008 | Level Control | S | M |
| 0x0406 | Occupancy Sensing | C | O |

**Element Overrides**: Same as On/Off Light

### 4.3 Color Temperature Light (0x010C)
Class: Simple | Scope: Endpoint | Rev: 4 | Superset: Dimmable Light
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | M |
| 0x0004 | Groups | S | M |
| 0x0062 | Scenes Management | S | P, M |
| 0x0006 | On/Off | S | M |
| 0x0008 | Level Control | S | M |
| 0x0300 | Color Control | S | M |
| 0x0406 | Occupancy Sensing | C | O |

**Element Overrides**:
- Same as Dimmable Light plus:
- Color Control: ColorTemperature feature M, RemainingTime attr M

### 4.4 Extended Color Light (0x010D)
Class: Simple | Scope: Endpoint | Rev: 4 | Superset: Color Temperature Light
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | M |
| 0x0004 | Groups | S | M |
| 0x0062 | Scenes Management | S | P, M |
| 0x0006 | On/Off | S | M |
| 0x0008 | Level Control | S | M |
| 0x0300 | Color Control | S | M |
| 0x0406 | Occupancy Sensing | C | O |

**Element Overrides**:
- Same as Color Temperature Light plus:
- Color Control: HueSaturation O, EnhancedHue O, ColorLoop O, XY M, ColorTemperature M, RemainingTime M

---
## 5. Smart Plugs/Outlets and Actuators

### 5.1 On/Off Plug-in Unit (0x010A)
Class: Simple | Scope: Endpoint | Rev: 3
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | M |
| 0x0004 | Groups | S | M |
| 0x0062 | Scenes Management | S | P, M |
| 0x0006 | On/Off | S | M |
| 0x0008 | Level Control | S | O |
| 0x0406 | Occupancy Sensing | C | O |

**Element Overrides**: Same as On/Off Light

### 5.2 Dimmable Plug-In Unit (0x010B)
Class: Simple | Scope: Endpoint | Rev: 4
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | M |
| 0x0004 | Groups | S | M |
| 0x0062 | Scenes Management | S | P, M |
| 0x0006 | On/Off | S | M |
| 0x0008 | Level Control | S | M |
| 0x0406 | Occupancy Sensing | C | O |

**Element Overrides**: Same as Dimmable Light

### 5.3 Mounted On/Off Control (0x010F)
Class: Simple | Scope: Endpoint | Rev: 1
Same clusters and overrides as On/Off Plug-in Unit

### 5.4 Mounted Dimmable Load Control (0x0110)
Class: Simple | Scope: Endpoint | Rev: 1
Same clusters and overrides as Dimmable Plug-In Unit

### 5.5 Pump (0x0303)
Class: Simple | Scope: Endpoint | Rev: 3
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0006 | On/Off | S | M |
| 0x0200 | Pump Configuration and Control | S | M |
| 0x0003 | Identify | S | M |
| 0x0008 | Level Control | S | O |
| 0x0004 | Groups | S | O |
| 0x0062 | Scenes Management | S | P, O |
| 0x0402 | Temperature Measurement | S | O |
| 0x0403 | Pressure Measurement | S | O |
| 0x0404 | Flow Measurement | S | O |
| 0x0402 | Temperature Measurement | C | O |
| 0x0403 | Pressure Measurement | C | O |
| 0x0404 | Flow Measurement | C | O |
| 0x0406 | Occupancy Sensing | C | O |

**Level Control**: Controls pump setpoint (Level 1-200 = 0.5-100%, 201-255 = 100%)

### 5.6 Water Valve (0x0042)
Class: Simple | Scope: Endpoint | Rev: 1
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | M |
| 0x0081 | Valve Configuration and Control | S | M |
| 0x0404 | Flow Measurement | S | O |
| 0x0404 | Flow Measurement | C | O |

---
## 6. Switches and Controls

### 6.1 On/Off Light Switch (0x0103)
Class: Simple | Scope: Endpoint | Rev: 3
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | M |
| 0x0003 | Identify | C | M |
| 0x0004 | Groups | C | O |
| 0x0006 | On/Off | C | M |
| 0x0062 | Scenes Management | C | P, O |

### 6.2 Dimmer Switch (0x0104)
Class: Simple | Scope: Endpoint | Rev: 3 | Superset: On/Off Light Switch
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | M |
| 0x0003 | Identify | C | M |
| 0x0004 | Groups | C | O |
| 0x0062 | Scenes Management | C | P, O |
| 0x0006 | On/Off | C | M |
| 0x0008 | Level Control | C | M |

### 6.3 Color Dimmer Switch (0x0105)
Class: Simple | Scope: Endpoint | Rev: 3 | Superset: Dimmer Switch
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | M |
| 0x0003 | Identify | C | M |
| 0x0004 | Groups | C | O |
| 0x0062 | Scenes Management | C | P, O |
| 0x0006 | On/Off | C | M |
| 0x0008 | Level Control | C | M |
| 0x0300 | Color Control | C | M |

### 6.4 Control Bridge (0x0840)
Class: Simple | Scope: Endpoint | Rev: 3
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | M |
| 0x0003 | Identify | C | M |
| 0x0004 | Groups | C | M |
| 0x0062 | Scenes Management | C | P, M |
| 0x0006 | On/Off | C | M |
| 0x0008 | Level Control | C | M |
| 0x0300 | Color Control | C | M |
| 0x0400 | Illuminance Measurement | C | O |
| 0x0406 | Occupancy Sensing | C | O |

### 6.5 Pump Controller (0x0304)
Class: Simple | Scope: Endpoint | Rev: 4
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0006 | On/Off | C | M |
| 0x0200 | Pump Configuration and Control | C | M |
| 0x0003 | Identify | S | M |
| 0x0003 | Identify | C | O |
| 0x0004 | Groups | C | O |
| 0x0008 | Level Control | C | O |
| 0x0062 | Scenes Management | C | P, O |
| 0x0402 | Temperature Measurement | C | O |
| 0x0403 | Pressure Measurement | C | O |
| 0x0404 | Flow Measurement | C | O |

### 6.6 Generic Switch (0x000F)
Class: Simple | Scope: Endpoint | Rev: 3
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | M |
| 0x003B | Switch | S | M |

**Requirements**:
- Send attribute/event updates without delay to subscribed parties
- Multi-switch: Use TagList in Descriptor for disambiguation (Position, Number namespaces)

---
## 7. Sensor Device Types

### 7.1 Contact Sensor (0x0015)
Class: Simple | Scope: Endpoint | Rev: 2
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | M |
| 0x0045 | Boolean State | S | M |
| 0x0080 | Boolean State Configuration | S | O |

**Boolean State**: True=Closed/Contact, False=Open/No Contact

### 7.2 Light Sensor (0x0106)
Class: Simple | Scope: Endpoint | Rev: 3
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | M |
| 0x0004 | Groups | C | [Zigbee] |
| 0x0400 | Illuminance Measurement | S | M |

### 7.3 Occupancy Sensor (0x0107)
Class: Simple | Scope: Endpoint | Rev: 4
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | M |
| 0x0080 | Boolean State Configuration | S | O |
| 0x0406 | Occupancy Sensing | S | M |

### 7.4 Temperature Sensor (0x0302)
Class: Simple | Scope: Endpoint | Rev: 2
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0402 | Temperature Measurement | S | M |
| 0x0003 | Identify | S | M |
| 0x0004 | Groups | C | [Zigbee] |

### 7.5 Pressure Sensor (0x0305)
Class: Simple | Scope: Endpoint | Rev: 2
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0403 | Pressure Measurement | S | M |
| 0x0003 | Identify | S | M |
| 0x0004 | Groups | C | [Zigbee] |

### 7.6 Flow Sensor (0x0306)
Class: Simple | Scope: Endpoint | Rev: 2
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0404 | Flow Measurement | S | M |
| 0x0003 | Identify | S | M |
| 0x0004 | Groups | C | [Zigbee] |

### 7.7 Humidity Sensor (0x0307)
Class: Simple | Scope: Endpoint | Rev: 2
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | M |
| 0x0405 | Relative Humidity Measurement | S | M |
| 0x0004 | Groups | C | [Zigbee] |

### 7.8 On/Off Sensor (0x0850)
Class: Simple | Scope: Endpoint | Rev: 3
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | M |
| 0x0003 | Identify | C | M |
| 0x0004 | Groups | C | O |
| 0x0062 | Scenes Management | C | P, O |
| 0x0006 | On/Off | C | M |
| 0x0008 | Level Control | C | O |
| 0x0300 | Color Control | C | O |

### 7.9 Smoke CO Alarm (0x0076)
Class: Simple | Scope: Endpoint | Rev: 1
**Device Type Requirements**: Power Source (min 1) M
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | M |
| 0x0004 | Groups | S | O |
| 0x005C | Smoke CO Alarm | S | M |
| 0x0405 | Relative Humidity Measurement | S | O |
| 0x0402 | Temperature Measurement | S | O |
| 0x040C | Carbon Monoxide Concentration Measurement | S | O |

### 7.10 Air Quality Sensor (0x002C)
Class: Simple | Scope: Endpoint | Rev: 1
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | M |
| 0x005B | Air Quality | S | M |
| 0x0402 | Temperature Measurement | S | O |
| 0x0405 | Relative Humidity Measurement | S | O |
| 0x040C | Carbon Monoxide Concentration Measurement | S | O |
| 0x040D | Carbon Dioxide Concentration Measurement | S | O |
| 0x0413 | Nitrogen Dioxide Concentration Measurement | S | O |
| 0x0415 | Ozone Concentration Measurement | S | O |
| 0x042B | Formaldehyde Concentration Measurement | S | O |
| 0x042C | PM1 Concentration Measurement | S | O |
| 0x042A | PM2.5 Concentration Measurement | S | O |
| 0x042D | PM10 Concentration Measurement | S | O |
| 0x042F | Radon Concentration Measurement | S | O |
| 0x042E | TVOC Concentration Measurement | S | O |

### 7.11 Water Freeze Detector (0x0041)
Class: Simple | Scope: Endpoint | Rev: 1
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | M |
| 0x0045 | Boolean State | S | M |
| 0x0080 | Boolean State Configuration | S | O |

**Boolean State**: True=Freeze risk, False=No freeze risk
**Element Override**: Boolean State StateChange event M

### 7.12 Water Leak Detector (0x0043)
Class: Simple | Scope: Endpoint | Rev: 1
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | M |
| 0x0045 | Boolean State | S | M |
| 0x0080 | Boolean State Configuration | S | O |

**Boolean State**: True=Leak detected, False=No leak
**Element Override**: Boolean State StateChange event M

### 7.13 Rain Sensor (0x0044)
Class: Simple | Scope: Endpoint | Rev: 1
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | M |
| 0x0045 | Boolean State | S | M |
| 0x0080 | Boolean State Configuration | S | O |

**Boolean State**: True=Rain detected, False=No rain
**Element Override**: Boolean State StateChange event M

---
## 8. Closure Device Types

### 8.1 Door Lock (0x000A)
Class: Simple | Scope: Endpoint | Rev: 3
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | M |
| 0x0004 | Groups | S | X |
| 0x0062 | Scenes Management | S | X |
| 0x0101 | Door Lock | S | M |

**Element Overrides**:
- Access Control: Extension attr [Matter]
- Door Lock: User feature [Matter & (PIN\|RID\|FPG\|FACE\|ALIRO)]
- Door Lock: RFIDCredential feature P, O
- Door Lock: AlarmMask attr [Alarms]

### 8.2 Door Lock Controller (0x000B)
Class: Simple | Scope: Endpoint | Rev: 3
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0004 | Groups | C | O |
| 0x0062 | Scenes Management | C | P, O |
| 0x0038 | Time Synchronization | S | O |
| 0x0101 | Door Lock | C | M |

### 8.3 Window Covering (0x0202)
Class: Simple | Scope: Endpoint | Rev: 3
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | M |
| 0x0004 | Groups | S | Active, O |
| 0x0102 | Window Covering | S | M |

**Element Override**: Window Covering AbsolutePosition feature [Zigbee]

### 8.4 Window Covering Controller (0x0203)
Class: Simple | Scope: Endpoint | Rev: 3
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | O |
| 0x0003 | Identify | C | O |
| 0x0004 | Groups | C | Active, O |
| 0x0102 | Window Covering | C | M |

**Element Override**: Window Covering AbsolutePosition feature [Zigbee]

---
## 9. HVAC Device Types

### 9.1 Thermostat (0x0301)
Class: Simple | Scope: Endpoint | Rev: 4
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | M |
| 0x0004 | Groups | S | Active |
| 0x0201 | Thermostat | S | M |
| 0x0204 | Thermostat User Interface Configuration | S | O |
| 0x009B | Energy Preference | S | O |
| 0x0202 | Fan Control | C | O |
| 0x0402 | Temperature Measurement | C | O |
| 0x0405 | Relative Humidity Measurement | C | O |
| 0x0406 | Occupancy Sensing | C | O |

**Element Overrides**:
- Thermostat: ScheduleConfiguration feature X, AlarmMask attr X
- Thermostat: GetRelayStatusLog cmd X, GetRelayStatusLogResponse cmd X

### 9.2 Fan (0x002B)
Class: Simple | Scope: Endpoint | Rev: 3
**Device Type Requirements**: Thermostat O
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | M |
| 0x0004 | Groups | S | M |
| 0x0006 | On/Off | S | O |
| 0x0202 | Fan Control | S | M |

**Element Override**: Fan Control FanModeSequence attr Fixed, ReadOnly [Matter]

### 9.3 Air Purifier (0x002D)
Class: Simple | Scope: Endpoint | Rev: 2
**Device Type Requirements**: Thermostat O, Temperature Sensor O, Humidity Sensor O, Air Quality Sensor O
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | M |
| 0x0004 | Groups | S | O |
| 0x0006 | On/Off | S | O |
| 0x0202 | Fan Control | S | M |
| 0x0071 | HEPA Filter Monitoring | S | O |
| 0x0072 | Activated Carbon Filter Monitoring | S | O |

---
## 10. Media Device Types

### 10.1 Basic Video Player (0x0028)
Class: Simple | Scope: Endpoint | Rev: 2
**Conditions**: PhysicalInputs
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0006 | OnOff | S | M |
| 0x0503 | WakeOnLAN | S | O |
| 0x0504 | Channel | S | O |
| 0x0505 | Target Navigator | S | O |
| 0x0506 | Media Playback | S | M |
| 0x0507 | Media Input | S | PhysicalInputs |
| 0x0508 | Low Power | S | O |
| 0x0509 | Keypad Input | S | M |
| 0x050B | Audio Output | S | O |
| 0x050F | Content Control | S | P, O |
| 0x0097 | Messages | S | O |

### 10.2 Casting Video Player (0x0023)
Class: Simple | Scope: Endpoint | Rev: 2
**Conditions**: ContentAppPlatform, PhysicalInputs
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0006 | OnOff | S | M |
| 0x0503 | WakeOnLAN | S | O |
| 0x0504 | Channel | S | O |
| 0x0505 | Target Navigator | S | O |
| 0x0506 | Media Playback | S | M |
| 0x0507 | Media Input | S | PhysicalInputs |
| 0x0508 | Low Power | S | O |
| 0x0509 | Keypad Input | S | M |
| 0x050A | Content Launcher | S | M |
| 0x050B | Audio Output | S | O |
| 0x050C | Application Launcher | S | ContentAppPlatform |
| 0x050E | Account Login | S | O |
| 0x050F | Content Control | S | P, O |
| 0x0097 | Messages | S | O |

**Element Override**: Application Launcher ApplicationPlatform feature M

### 10.3 Speaker (0x0022)
Class: Simple | Scope: Endpoint | Rev: 1
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0006 | OnOff | S | M |
| 0x0008 | Level Control | S | M |

**Note**: OnOff True=unmuted, False=muted; Level=volume

### 10.4 Content App (0x0024)
Class: Simple | Scope: Endpoint | Rev: 2
**Conditions**: ObserverClient
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x001E | Binding | S | ObserverClient |
| 0x0504 | Channel | S | O |
| 0x0505 | Target Navigator | S | O |
| 0x0506 | Media Playback | S | O |
| 0x0509 | Keypad Input | S | M |
| 0x050A | Content Launcher | S | O |
| 0x050C | Application Launcher | S | M |
| 0x050D | Application Basic | S | M |
| 0x050E | Account Login | S | O |
| 0x0510 | Content App Observer | C | ObserverClient |

**Element Override**: Application Launcher ApplicationPlatform feature X
**Composition**: Supports Dynamic Endpoint Allocation

### 10.5 Casting Video Client (0x0029)
Class: Simple | Scope: Endpoint | Rev: 2
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0006 | OnOff | C | M |
| 0x0008 | Level Control | C | O |
| 0x0503 | WakeOnLAN | C | O |
| 0x0504 | Channel | C | O |
| 0x0505 | Target Navigator | C | O |
| 0x0506 | Media Playback | C | O |
| 0x0507 | Media Input | C | O |
| 0x0508 | Low Power | C | O |
| 0x0509 | Keypad Input | C | M |
| 0x050A | Content Launcher | C | M |
| 0x050B | Audio Output | C | O |
| 0x050C | Application Launcher | C | O |
| 0x050D | Application Basic | C | M |
| 0x050E | Account Login | C | O |
| 0x050F | Content Control | C | P, O |
| 0x0510 | Content App Observer | S | O |
| 0x0097 | Messages | C | O |

### 10.6 Video Remote Control (0x002A)
Class: Simple | Scope: Endpoint | Rev: 2
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0006 | OnOff | C | M |
| 0x0008 | Level Control | C | O |
| 0x0503 | WakeOnLAN | C | O |
| 0x0504 | Channel | C | O |
| 0x0505 | Target Navigator | C | O |
| 0x0506 | Media Playback | C | M |
| 0x0507 | Media Input | C | O |
| 0x0508 | Low Power | C | O |
| 0x0509 | Keypad Input | C | M |
| 0x050A | Content Launcher | C | O |
| 0x050B | Audio Output | C | O |
| 0x050C | Application Launcher | C | O |
| 0x050E | Account Login | C | O |
| 0x050F | Content Control | C | P, O |

---
## 11. Generic Device Types

### 11.1 Mode Select (0x0027)
Class: Simple | Scope: Endpoint | Rev: 1
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0050 | Mode Select | S | M |

### 11.2 Aggregator (0x000E)
Class: Simple | Scope: Endpoint | Rev: 2
**Conditions**: FabricSynchronization (Commissioner Control with FabricSync bit)
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0025 | Actions | S | O |
| 0x0003 | Identify | S | O |
| 0x0751 | Commissioner Control | S | FabricSynchronization |

**Composition**: PartsList contains all aggregated endpoints (full-family pattern)

---
## 12. Robotic Device Types

### 12.1 Robotic Vacuum Cleaner (0x0074)
Class: Simple | Scope: Endpoint | Rev: 3
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | M |
| 0x0054 | RVC Run Mode | S | M |
| 0x0055 | RVC Clean Mode | S | O |
| 0x0061 | RVC Operational State | S | M |
| 0x0150 | Service Area | S | O |

**Operation Flow**:
- Start: RVC Run Mode CurrentMode=Idle, RVC Op State=Stopped/Paused/Docked/Charging → ChangeToMode(Cleaning)
- Pause: RVC Op State in Pause-compatible state → Pause cmd
- Resume: RVC Op State in Resume-compatible state + CurrentMode=Cleaning → Resume cmd
- Stop: CurrentMode=Cleaning → ChangeToMode(Idle)

---
## 13. Appliances Device Types

### 13.1 Laundry Washer (0x0073)
Class: Simple | Scope: Endpoint | Rev: 1
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | O |
| 0x0051 | Laundry Washer Mode | S | O |
| 0x0006 | On/Off | S | O |
| 0x0053 | Laundry Washer Controls | S | O |
| 0x0056 | Temperature Control | S | O |
| 0x0060 | Operational State | S | M |

**Element Overrides**:
- On/Off: DeadFrontBehavior feature M
- Laundry Washer Mode: StartUpMode attr X, OnOff feature X

### 13.2 Refrigerator (0x0070)
Class: Simple | Scope: Endpoint | Rev: 2
**Conditions**: Cooler (at least one endpoint with Temperature Control)
**Device Type Requirements**: Temperature Controlled Cabinet (min 1) M
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | O |
| 0x0052 | Refrigerator And Temperature Controlled Cabinet Mode | S | O |
| 0x0057 | Refrigerator Alarm | S | O |

**Element Overrides**:
- Refrigerator Mode: StartUpMode attr X, OnOff feature X

### 13.3 Room Air Conditioner (0x0072)
Class: Simple | Scope: Endpoint | Rev: 2
**Device Type Requirements**: Temperature Sensor O, Humidity Sensor O
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | M |
| 0x0004 | Groups | S | O |
| 0x0062 | Scenes Management | S | P, O |
| 0x0006 | On/Off | S | M |
| 0x0201 | Thermostat | S | M |
| 0x0202 | Fan Control | S | O |
| 0x0204 | Thermostat User Interface Configuration | S | O |
| 0x0402 | Temperature Measurement | S | O |
| 0x0405 | Relative Humidity Measurement | S | O |

**Element Overrides**:
- On/Off: DeadFrontBehavior feature M
- Thermostat User Interface Configuration: KeypadLockout attr O

### 13.4 Temperature Controlled Cabinet (0x0071)
Class: Simple | Scope: Endpoint | Rev: 3
**Conditions**: Cooler (cooling), Heater (heating) - mutually exclusive
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0056 | Temperature Control | S | M |
| 0x0402 | Temperature Measurement | S | O |
| 0x0052 | Refrigerator and TCC Mode | S | [Cooler] |
| 0x0049 | Oven Mode | S | [Heater] |
| 0x0048 | Oven Cavity Operational State | S | [Heater] |

**Element Overrides**:
- Refrigerator Mode: StartUpMode X, OnOff feature X
- Oven Mode: StartUpMode X, OnOff feature X
- Oven Cavity Op State: Pause cmd X, Resume cmd X

### 13.5 Dishwasher (0x0075)
Class: Simple | Scope: Endpoint | Rev: 1
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | O |
| 0x0006 | On/Off | S | O |
| 0x0056 | Temperature Control | S | O |
| 0x0059 | Dishwasher Mode | S | O |
| 0x005D | Dishwasher Alarm | S | O |
| 0x0060 | Operational State | S | M |

**Element Overrides**:
- On/Off: DeadFrontBehavior feature M
- Dishwasher Mode: StartUpMode attr X, OnOff feature X

### 13.6 Laundry Dryer (0x007C)
Class: Simple | Scope: Endpoint | Rev: 1
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | O |
| 0x0051 | Laundry Washer Mode | S | O |
| 0x0006 | On/Off | S | O |
| 0x004A | Laundry Dryer Controls | S | O |
| 0x0056 | Temperature Control | S | O |
| 0x0060 | Operational State | S | M |

**Element Overrides**:
- On/Off: DeadFrontBehavior feature M
- Laundry Washer Mode: StartUpMode attr X, OnOff feature X

### 13.7 Cook Surface (0x0077)
Class: Simple | Scope: Endpoint | Rev: 1
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0056 | Temperature Control | S | O.a+ |
| 0x0402 | Temperature Measurement | S | O.a+ |
| 0x0006 | On/Off | S | O |

**Element Override**: On/Off OffOnly feature M (safety)

### 13.8 Cooktop (0x0078)
Class: Simple | Scope: Endpoint | Rev: 1
**Device Type Requirements**: Cook Surface (min 1) O
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | O |
| 0x0006 | On/Off | S | M |

**Element Override**: On/Off OffOnly feature M (safety)

### 13.9 Oven (0x007B)
Class: Simple | Scope: Endpoint | Rev: 2
**Conditions**: Heater (at least one endpoint with Temperature Control)
**Device Type Requirements**: Temperature Controlled Cabinet (min 1) M, Cooktop (max 1) O
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | O |

### 13.10 Extractor Hood (0x007A)
Class: Simple | Scope: Endpoint | Rev: 1
**Device Type Requirements**: On/Off Light+ O (superset)
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | O |
| 0x0071 | HEPA Filter Monitoring | S | O |
| 0x0072 | Activated Carbon Filter Monitoring | S | O |
| 0x0202 | Fan Control | S | M |

**Element Overrides**: Fan Control Rocking X, Wind X, AirflowDirection X

### 13.11 Microwave Oven (0x0079)
Class: Simple | Scope: Endpoint | Rev: 1
**Device Type Requirements**: On/Off Light+ O (surface light for over-range)
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | O |
| 0x0060 | Operational State | S | M |
| 0x0202 | Fan Control | S | O |
| 0x005E | Microwave Oven Mode | S | M |
| 0x005F | Microwave Oven Control | S | M |

**Element Overrides**:
- Operational State: CountdownTime attr M
- Fan Control: Wind X, AirflowDirection X

---
## 14. Energy Device Types

### 14.1 EVSE (0x050C)
Class: Simple | Scope: Endpoint | Rev: 2
**Device Type Requirements**:
| ID | Name | Conformance |
|----|------|-------------|
| 0x0011 | Power Source | min 1, M |
| 0x0510 | Electrical Sensor | min 1, M |
| 0x050D | Device Energy Management | min 1, M |

| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | O |
| 0x0099 | Energy EVSE | S | M |
| 0x009D | Energy EVSE Mode | S | M |
| 0x0402 | Temperature Measurement | S | O |

**Composition Requirements**:
- Electrical Sensor: Both Electrical Energy Measurement and Electrical Power Measurement M
- 3-phase: 3 additional Electrical Sensor endpoints with AC phase semantic tags
- DEM: PowerForecastReporting feature M; PowerAdjustment feature M if V2X supported

### 14.2 Water Heater (0x050F)
Class: Simple | Scope: Endpoint | Rev: 1
**Device Type Requirements**:
| ID | Name | Conformance |
|----|------|-------------|
| 0x0011 | Power Source | O |
| 0x0302 | Temperature Sensor | O |
| 0x0510 | Electrical Sensor | desc |
| 0x050D | Device Energy Management | O |

| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | O |
| 0x0201 | Thermostat | S | M |
| 0x0094 | Water Heater Management | S | M |
| 0x009E | Water Heater Mode | S | M |

**Element Override**: Thermostat Heating feature M
**Note**: If DEM included, Electrical Sensor also required

### 14.3 Solar Power (0x0017)
Class: Simple | Scope: Endpoint | Rev: 1
**Device Type Requirements**:
| ID | Name | Conformance |
|----|------|-------------|
| 0x0011 | Power Source | min 1, M |
| 0x0510 | Electrical Sensor | min 1, M |
| 0x050D | Device Energy Management | O |
| 0x0302 | Temperature Sensor | O |

| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | O |

**Composition Requirements**:
- Power Source: Wired feature M, TagList feature M (Grid tag if connected to premises)
- Electrical Sensor: Electrical Power Measurement M (Voltage M, ActiveCurrent M), Electrical Energy Measurement M (ExportedEnergy M)
- AC: AlternatingCurrent feature M, PolyPhasePower if polyphase
- DC: DirectCurrent feature M
- DEM: PowerAdjustment feature M (if controllable)
- Temperature Sensor: TagList feature M

### 14.4 Battery Storage (0x0018)
Class: Simple | Scope: Endpoint | Rev: 1
**Device Type Requirements**:
| ID | Name | Conformance |
|----|------|-------------|
| 0x0011 | Power Source | min 1, M |
| 0x0510 | Electrical Sensor | min 1, M |
| 0x050D | Device Energy Management | M |
| 0x0302 | Temperature Sensor | O |
| 0x0017 | Solar Power | O |

| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | O |

**Composition Requirements**:
- Power Source: Wired M, Battery M, BatVoltage M, BatPercentRemaining M, BatTimeRemaining M, ActiveBatFaults M, BatCapacity M, BatTimeToFullCharge M, BatChargingCurrent M, ActiveBatChargeFaults M, TagList M (Grid+Battery)
- Electrical Sensor: Electrical Power Measurement M (AlternatingCurrent M, Voltage M, ActiveCurrent M), Electrical Energy Measurement M (ExportedEnergy M)
- DEM: PowerAdjustment feature M

### 14.5 Heat Pump (0x0309)
Class: Simple | Scope: Endpoint | Rev: 1
**Device Type Requirements**:
| ID | Name | Conformance |
|----|------|-------------|
| 0x0011 | Power Source | M |
| 0x0510 | Electrical Sensor | min 1, M |
| 0x050D | Device Energy Management | M |
| 0x0301 | Thermostat | O |
| 0x050F | Water Heater | O |
| 0x0302 | Temperature Sensor | O |

| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0003 | Identify | S | O |
| 0x0201 | Thermostat | C | O |

**Note**: Must include Thermostat devices OR Thermostat client
**Composition Requirements**:
- Power Source: Wired M, TagList M (Grid)
- Electrical Sensor: Electrical Power Measurement M (AlternatingCurrent M, Voltage M, ActiveCurrent M), Electrical Energy Measurement M
- DEM: PowerAdjustment feature M
- Thermostat: TagList M, User Label M
- Temperature Sensor: TagList M

---
## 15. Network Infrastructure Device Types

### 15.1 Network Infrastructure Manager (0x0090)
Class: Simple | Scope: Endpoint | Rev: 1
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0451 | Wi-Fi Network Management | S | M |
| 0x0452 | Thread Border Router Management | S | M |
| 0x0453 | Thread Network Directory | S | M |

**Root Node Element Override**: Access Control MNGD feature desc

**Requirements**:
- Bridged Wi-Fi/Ethernet hub network for IPv6 connectivity
- Ethernet LAN interface part of bridged hub
- Wi-Fi ESS with concurrent 2.4GHz and 5GHz BSSs (6GHz optional)
- Wi-Fi 6+ certification (Wi-Fi 6E if 6GHz, HaLow if sub-1GHz)
- Thread Border Router: Built on Thread 1.3.0+ (1.4.0+ recommended, mandatory in Matter 1.5)

### 15.2 Thread Border Router (0x0091)
Class: Simple | Scope: Endpoint | Rev: 1
**Device Type Requirements**: Secondary Network Interface O
| ID | Cluster | C/S | Conformance |
|----|---------|-----|-------------|
| 0x0035 | Thread Network Diagnostics | S | M |
| 0x0452 | Thread Border Router Management | S | M |
| 0x0453 | Thread Network Directory | S | O |

**Requirements**:
- Thread Border Router per Thread spec
- Built on Thread 1.3.0+ certification (1.4.0+ recommended, mandatory in Matter 1.5)
- If Secondary Network Interface: Thread Border Router Management and Network Commissioning reflect same config, MaxNetworks=1

---
## Appendix: Dead Front Behavior

Appliances with On/Off DeadFrontBehavior feature M:
- Off command → "dead front" state (OnOff=False)
- On command → exit "dead front" state
- In dead front: operational values may be unavailable

**Affected Device Types**: Laundry Washer, Room Air Conditioner, Dishwasher, Laundry Dryer

**Best Effort Values in Dead Front**:
| Cluster | Attribute | Value |
|---------|-----------|-------|
| Mode clusters | CurrentMode | Manufacturer Specific |
| Temperature Control | All | Manufacturer Specific |
| Operational State | PhaseList, CurrentPhase, CountdownTime | null |
| Operational State | OperationalState | Stopped |
| Operational State | OperationalError | No Error |

---
## Appendix: Cluster ID Quick Reference

| ID | Cluster Name |
|----|--------------|
| 0x0003 | Identify |
| 0x0004 | Groups |
| 0x0006 | On/Off |
| 0x0008 | Level Control |
| 0x001D | Descriptor |
| 0x001E | Binding |
| 0x001F | Access Control |
| 0x0025 | Actions |
| 0x0028 | Basic Information |
| 0x0029 | OTA Software Update Provider |
| 0x002A | OTA Software Update Requestor |
| 0x002B | Localization Configuration |
| 0x002C | Time Format Localization |
| 0x002D | Unit Localization |
| 0x002E | Power Source Configuration |
| 0x002F | Power Source |
| 0x0030 | General Commissioning |
| 0x0031 | Network Commissioning |
| 0x0032 | Diagnostic Logs |
| 0x0033 | General Diagnostics |
| 0x0034 | Software Diagnostics |
| 0x0035 | Thread Network Diagnostics |
| 0x0036 | Wi-Fi Network Diagnostics |
| 0x0037 | Ethernet Network Diagnostics |
| 0x0038 | Time Synchronization |
| 0x0039 | Bridged Device Basic Information |
| 0x003B | Switch |
| 0x003C | Administrator Commissioning |
| 0x003E | Node Operational Credentials |
| 0x003F | Group Key Management |
| 0x0040 | Fixed Label |
| 0x0041 | User Label |
| 0x0045 | Boolean State |
| 0x0046 | ICD Management |
| 0x0048 | Oven Cavity Operational State |
| 0x0049 | Oven Mode |
| 0x004A | Laundry Dryer Controls |
| 0x0050 | Mode Select |
| 0x0051 | Laundry Washer Mode |
| 0x0052 | Refrigerator And TCC Mode |
| 0x0053 | Laundry Washer Controls |
| 0x0054 | RVC Run Mode |
| 0x0055 | RVC Clean Mode |
| 0x0056 | Temperature Control |
| 0x0057 | Refrigerator Alarm |
| 0x0059 | Dishwasher Mode |
| 0x005B | Air Quality |
| 0x005C | Smoke CO Alarm |
| 0x005D | Dishwasher Alarm |
| 0x005E | Microwave Oven Mode |
| 0x005F | Microwave Oven Control |
| 0x0060 | Operational State |
| 0x0061 | RVC Operational State |
| 0x0062 | Scenes Management |
| 0x0071 | HEPA Filter Monitoring |
| 0x0072 | Activated Carbon Filter Monitoring |
| 0x0080 | Boolean State Configuration |
| 0x0081 | Valve Configuration and Control |
| 0x0090 | Electrical Power Measurement |
| 0x0091 | Electrical Energy Measurement |
| 0x0094 | Water Heater Management |
| 0x0097 | Messages |
| 0x0098 | Device Energy Management |
| 0x0099 | Energy EVSE |
| 0x009B | Energy Preference |
| 0x009C | Power Topology |
| 0x009D | Energy EVSE Mode |
| 0x009E | Water Heater Mode |
| 0x009F | Device Energy Management Mode |
| 0x0101 | Door Lock |
| 0x0102 | Window Covering |
| 0x0150 | Service Area |
| 0x0200 | Pump Configuration and Control |
| 0x0201 | Thermostat |
| 0x0202 | Fan Control |
| 0x0204 | Thermostat User Interface Configuration |
| 0x0300 | Color Control |
| 0x0400 | Illuminance Measurement |
| 0x0402 | Temperature Measurement |
| 0x0403 | Pressure Measurement |
| 0x0404 | Flow Measurement |
| 0x0405 | Relative Humidity Measurement |
| 0x0406 | Occupancy Sensing |
| 0x040C | Carbon Monoxide Concentration Measurement |
| 0x040D | Carbon Dioxide Concentration Measurement |
| 0x0413 | Nitrogen Dioxide Concentration Measurement |
| 0x0415 | Ozone Concentration Measurement |
| 0x042A | PM2.5 Concentration Measurement |
| 0x042B | Formaldehyde Concentration Measurement |
| 0x042C | PM1 Concentration Measurement |
| 0x042D | PM10 Concentration Measurement |
| 0x042E | TVOC Concentration Measurement |
| 0x042F | Radon Concentration Measurement |
| 0x0451 | Wi-Fi Network Management |
| 0x0452 | Thread Border Router Management |
| 0x0453 | Thread Network Directory |
| 0x0503 | WakeOnLAN |
| 0x0504 | Channel |
| 0x0505 | Target Navigator |
| 0x0506 | Media Playback |
| 0x0507 | Media Input |
| 0x0508 | Low Power |
| 0x0509 | Keypad Input |
| 0x050A | Content Launcher |
| 0x050B | Audio Output |
| 0x050C | Application Launcher |
| 0x050D | Application Basic |
| 0x050E | Account Login |
| 0x050F | Content Control |
| 0x0510 | Content App Observer |
| 0x0750 | Ecosystem Information |
| 0x0751 | Commissioner Control |
| 0x0752 | Joint Fabric Datastore |
| 0x0753 | Joint Fabric PKI |
