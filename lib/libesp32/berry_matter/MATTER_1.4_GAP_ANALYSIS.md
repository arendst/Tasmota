# Matter 1.4/1.4.1 Gap Analysis for Tasmota Implementation

## Executive Summary

This document analyzes the gaps between Tasmota's current Matter implementation and the Matter 1.4/1.4.1 specification. The current implementation appears to target Matter 1.0/1.1 with some 1.2 features.

## Current Implementation Status

### Version Indicators
- **DataModelRevision**: Returns `17` (Matter 1.2 - Basic Information cluster attribute 0x0000)
- **InteractionModelRevision**: Returns `11` (Matter 1.2+ - in IM messages)
- **Cluster Revisions**: Various, ranging from 1-7 depending on cluster

### Supported Device Types

| Device Type | Status | Notes |
|-------------|--------|-------|
| Root Node (0x0016) | ✅ Supported | |
| Aggregator (0xFF00) | ✅ Supported | Bridge mode |
| On/Off Light (0x0100) | ✅ Supported | |
| Dimmable Light (0x0101) | ✅ Supported | |
| Color Temperature Light (0x010C) | ✅ Supported | |
| Extended Color Light (0x010D) | ✅ Supported | |
| On/Off Plug-in Unit (0x010A) | ✅ Supported | |
| Temperature Sensor (0x0302) | ✅ Supported | |
| Humidity Sensor (0x0307) | ✅ Supported | |
| Pressure Sensor (0x0305) | ✅ Supported | |
| Illuminance Sensor (0x0106) | ✅ Supported | |
| Occupancy Sensor (0x0107) | ✅ Supported | |
| Contact Sensor (0x0015) | ✅ Supported | |
| Flow Sensor (0x0306) | ✅ Supported | |
| Window Covering (0x0202) | ✅ Supported | Shutter/Tilt |
| Fan (0x002B) | ✅ Supported | |
| Generic Switch (0x000F) | ✅ Supported | Button |
| Air Quality Sensor | ✅ Supported | |
| Rain Sensor | ✅ Supported | |
| Water Leak Detector | ✅ Supported | |

### Supported Clusters

| Cluster | ID | Status | Notes |
|---------|-----|--------|-------|
| Descriptor | 0x001D | ✅ | |
| Access Control | 0x001F | ✅ | |
| Basic Information | 0x0028 | ✅ | |
| OTA Software Update Requestor | 0x002A | ⚠️ Partial | Stub implementation |
| Localization Configuration | 0x002B | ✅ | |
| Time Format Localization | 0x002C | ✅ | |
| General Commissioning | 0x0030 | ✅ | |
| Network Commissioning | 0x0031 | ⚠️ Partial | Limited attributes |
| Diagnostic Logs | 0x0032 | ⚠️ Stub | No attributes |
| General Diagnostics | 0x0033 | ✅ | |
| Software Diagnostics | 0x0034 | ⚠️ Stub | No attributes |
| Time Synchronization | 0x0038 | ⚠️ Partial | Basic time only |
| Administrator Commissioning | 0x003C | ✅ | |
| Node Operational Credentials | 0x003E | ✅ | |
| Group Key Management | 0x003F | ⚠️ Stub | TODO |
| On/Off | 0x0006 | ✅ | |
| Level Control | 0x0008 | ✅ | |
| Color Control | 0x0300 | ✅ | |
| Window Covering | 0x0102 | ✅ | |
| Fan Control | 0x0202 | ✅ | |
| Temperature Measurement | 0x0402 | ✅ | |
| Pressure Measurement | 0x0403 | ✅ | |
| Flow Measurement | 0x0404 | ✅ | |
| Humidity Measurement | 0x0405 | ✅ | |
| Occupancy Sensing | 0x0406 | ✅ | |
| Illuminance Measurement | 0x0400 | ✅ | |
| Boolean State | 0x0045 | ✅ | |
| Generic Switch | 0x003B | ✅ | |

---

## Matter 1.4 New Features - Gap Analysis

### 1. New Device Types (Matter 1.4)

| Device Type | Cluster ID | Status | Priority |
|-------------|------------|--------|----------|
| **Battery Storage** | - | ❌ Not Implemented | Medium |
| **Solar Power** | - | ❌ Not Implemented | Medium |
| **Home Router** | - | ❌ Not Implemented | Low |
| **Water Heater** | 0x0094 | ❌ Not Implemented | Medium |
| **Heat Pump** | - | ❌ Not Implemented | Medium |
| **Enhanced EV Charger** | 0x0099 | ❌ Not Implemented | Low |

### 2. New Clusters (Matter 1.4)

| Cluster | ID | Status | Description |
|---------|-----|--------|-------------|
| **Electrical Power Measurement** | 0x0090 | ❌ Not Implemented | Real-time power monitoring |
| **Electrical Energy Measurement** | 0x0091 | ❌ Not Implemented | Energy consumption tracking |
| **Device Energy Management** | 0x0098 | ❌ Not Implemented | Energy optimization |
| **Device Energy Management Mode** | 0x009F | ❌ Not Implemented | Energy modes |
| **Power Topology** | 0x009C | ❌ Not Implemented | Power distribution |
| **Water Heater Management** | 0x0094 | ❌ Not Implemented | Water heater control |
| **Water Heater Mode** | 0x009E | ❌ Not Implemented | Water heater modes |

### 3. Protocol Enhancements (Matter 1.4/1.4.1)

| Feature | Status | Impact |
|---------|--------|--------|
| **Thread 1.3.1 Support** | ❌ Not Implemented | Thread border router improvements |
| **NFC Onboarding** (1.4.1) | ❌ Not Implemented | Simplified commissioning |
| **Multi-device Setup** (1.4.1) | ❌ Not Implemented | Batch commissioning |
| **Enhanced CASE Resumption** | ✅ Implemented | Session resumption works |
| **ICD (Intermittently Connected Device)** | ✅ Implemented | SIT mode supported, LIT partial |

### 4. Security Updates

| Feature | Status | Notes |
|---------|--------|-------|
| PASE (SPAKE2+) | ✅ Compliant | Uses CHIP context prefix |
| CASE (Sigma) | ✅ Compliant | Full implementation |
| Session Resumption | ✅ Compliant | Working |
| Message Counter Validation | ✅ Compliant | Window size 32 |
| AES-CCM Encryption | ✅ Compliant | 16-byte tag |

---

## Detailed Gap Analysis

### HIGH PRIORITY Gaps

#### 1. Energy Measurement Clusters Missing
Tasmota has extensive energy monitoring capabilities (via PZEM, HLW8012, etc.) but these are not exposed via Matter. This is a significant gap since Matter 1.4 specifically adds energy clusters.

**Missing Clusters:**
- Electrical Power Measurement (0x0090)
- Electrical Energy Measurement (0x0091)

**Recommendation**: Create new plugin types:
- `Matter_Plugin_Sensor_Power.be`
- `Matter_Plugin_Sensor_Energy.be`

#### 2. Thermostat Cluster Not Implemented
The cluster revision table references Thermostat (0x0201) but no plugin implements it.

**Impact**: Cannot expose Tasmota thermostat functionality to Matter controllers.

### MEDIUM PRIORITY Gaps

#### 3. ICD (Intermittently Connected Device) Support
**Status**: ✅ IMPLEMENTED (Base SIT mode per Matter 1.4.1 spec)

The ICD Management Cluster (0x0046) is always enabled with base SIT (Short Idle Time) mode, appropriate for mains-powered WiFi devices.

**Implemented Attributes (per spec section 9.17.6):**
- `IdleModeDuration` (0x0000) - 1 second (minimum per spec)
- `ActiveModeDuration` (0x0001) - 300ms (spec default)
- `ActiveModeThreshold` (0x0002) - 300ms (spec default)

**Not Implemented (require optional features):**
- ICDCounter, ClientsSupportedPerFabric, RegisteredClients - require CIP feature
- OperatingMode - requires LITS feature
- UserActiveModeTriggerHint/Instruction - require UAT feature

**Cluster Configuration:**
- Revision: 3 (Matter 1.4.1)
- Feature Map: 0x00 (no optional features)

**mDNS Advertisement:**
- SII=500ms, SAI=300ms (spec defaults)
- ICD key not advertised (per spec: only for LITS-capable devices)

**Impact**: Controllers know the device is always reachable with standard MRP timing.

#### 4. Power Source Cluster Missing
No implementation of Power Source cluster (0x002F) which is important for:
- Battery level reporting
- Power source type indication
- Wired/battery status

#### 5. OTA Update Cluster Incomplete
Current implementation is a stub:
```berry
# Returns empty list and claims update not possible
return TLV.Matter_TLV_array()   # empty list for now TODO
return tlv_solo.set(TLV.BOOL, 0)  # we claim that update is not possible
```

### LOW PRIORITY Gaps

#### 6. Group Key Management Incomplete
Marked as TODO in the code:
```berry
elif cluster == 0x003F              # ========== Group Key Management Cluster 11.2 p.572 ==========
  # TODO
```

#### 7. Diagnostic Logs Cluster Empty
No attributes implemented:
```berry
elif cluster == 0x0032              # ========== Diagnostic Logs Cluster 11.10 p.637 ==========
  # no attributes
```

#### 8. Software Diagnostics Cluster Empty
```berry
elif cluster == 0x0034              # ========== Software Diagnostics Cluster 11.12 p.654 ==========
  # no mandatory attributes - to be added later (maybe)
```

---

## Session/Security Analysis

### PASE Implementation
✅ **Compliant** - Full SPAKE2+ implementation with correct CHIP context prefix.

### CASE Implementation  
✅ **Compliant** - Full Sigma1/Sigma2/Sigma3 protocol with:
- Proper certificate chain validation
- ECDH key exchange
- Session key derivation via HKDF
- Session resumption support

### Known Deviations (Acceptable)
1. **Context Prefix**: Uses `"CHIP PAKE V1 Commissioning"` instead of spec's `"Matter PAKE V1 Commissioning"` - this is correct for interoperability
2. **PBKDF2 Iterations**: Uses minimum 1000 - compliant but minimal security

---

## Recommendations

### Immediate (High Value)

1. **Add Energy Measurement Plugins**
   - Leverage existing Tasmota energy drivers
   - Implement clusters 0x0090 and 0x0091
   - High value for smart home energy monitoring

2. **Add Thermostat Plugin**
   - Implement cluster 0x0201
   - Map to Tasmota thermostat commands

### Short-term

3. **Enable ICD Support**
   - Uncomment and test ICD cluster
   - Important for battery devices

4. **Add Power Source Cluster**
   - Report battery/wired status
   - Important for portable devices

### Long-term

5. **Complete OTA Cluster**
   - Enable Matter-based firmware updates

6. **Add Water Heater Support** (Matter 1.4)
   - New device type for water heater control

---

## Compatibility Notes

The current implementation is **fully compatible** with Matter 1.0/1.1 controllers and should work with Matter 1.4 controllers for supported device types. The gaps primarily affect:

1. New Matter 1.4 device types (not supported)
2. Energy monitoring features (not exposed via Matter)
3. Advanced diagnostics (minimal implementation)

Controllers like Apple Home, Google Home, and Amazon Alexa will work with the current implementation for all supported device types.

---

## Additional Technical Details

### Commissioning Implementation

The commissioning implementation (`Matter_z_Commissioning.be`) is well-structured:

| Feature | Status | Notes |
|---------|--------|-------|
| PASE Window Management | ✅ | 10-minute default timeout |
| QR Code Generation | ✅ | Full MT: format support |
| Manual Pairing Code | ✅ | 11-digit with Verhoeff checksum |
| mDNS PASE Announce | ✅ | `_matterc._udp` with subtypes |
| mDNS Operational | ✅ | `_matter._tcp` per fabric |
| OpenCommissioningWindow | ✅ | Enhanced commissioning |
| OpenBasicCommissioningWindow | ✅ | Basic commissioning |
| RevokeCommissioning | ✅ | Close window |

### Fabric Management

The fabric implementation (`Matter_Fabric.be`) supports:

| Feature | Status | Notes |
|---------|--------|-------|
| Max CASE Sessions | 5 | Per fabric limit |
| Group Key Derivation | ✅ | IPK group key |
| Fabric Compression | ✅ | Compressed fabric ID |
| Multi-fabric | ✅ | Multiple controllers |
| Fabric Label | ✅ | User-defined labels |
| Fabric Parent Tracking | ✅ | Nested provisioning |

### Interaction Model

The IM implementation (`Matter_IM.be`) supports:

| Opcode | Name | Status |
|--------|------|--------|
| 0x01 | Status Response | ✅ |
| 0x02 | Read Request | ✅ (optimized) |
| 0x03 | Subscribe Request | ✅ |
| 0x04 | Subscribe Response | ❌ (device mode) |
| 0x05 | Report Data | ❌ (device mode) |
| 0x06 | Write Request | ✅ |
| 0x07 | Write Response | ❌ (device mode) |
| 0x08 | Invoke Request | ✅ (optimized) |
| 0x09 | Invoke Response | ❌ (device mode) |
| 0x0A | Timed Request | ✅ |

Note: Opcodes marked "device mode" are for controller-side operations, not needed for a Matter device.

### mDNS Service Records

**Commissioning (`_matterc._udp`):**
- VP (Vendor+Product)
- D (Discriminator)
- CM (Commissioning Mode)
- T (TCP support = 0)
- SII/SAI (Sleep intervals)
- Subtypes: `_L<discriminator>`, `_S<short>`, `_V<vendor>`, `_CM1`

**Operational (`_matter._tcp`):**
- Instance: `<compressed_fabric>-<device_id>`
- Subtype: `_I<compressed_fabric>`

---

## Matter 1.4.1 Specific Features

Matter 1.4.1 (May 2025) added:

| Feature | Status | Notes |
|---------|--------|-------|
| NFC Onboarding | ❌ | Requires NFC hardware |
| Multi-device Setup | ❌ | Batch commissioning |
| Enhanced Thread Support | ⚠️ | Depends on ESP-IDF |

These features are primarily controller-side or require specific hardware support.

---

## Implementation Quality Assessment

### Strengths
1. **Clean Architecture** - Well-separated concerns (Session, Fabric, Commissioning, IM)
2. **Optimized Paths** - Fast-track processing for common operations
3. **Robust Persistence** - JSON-based fabric/session storage
4. **Good Logging** - Comprehensive debug output
5. **Bridge Support** - Remote device aggregation

### Areas for Improvement
1. **Energy Clusters** - High-value gap for Tasmota's energy monitoring
2. **Thermostat** - Missing despite Tasmota having thermostat support
3. **ICD** - Code exists but disabled
4. **Diagnostics** - Minimal implementation

---

*Analysis Date: January 2026*
*Based on Tasmota Matter Berry implementation*
*Compared against Matter 1.4.1 Core Specification*

---

## Related Documents

- **[MATTER_1.4.1_DETAILED_GAP_ANALYSIS.md](MATTER_1.4.1_DETAILED_GAP_ANALYSIS.md)** - Comprehensive gap analysis with implementation requirements, code examples, and roadmap
- **[MATTER_CLASS_HIERARCHY.md](MATTER_CLASS_HIERARCHY.md)** - Plugin class hierarchy documentation
- **[MATTER_SESSION_ARCHITECTURE.md](MATTER_SESSION_ARCHITECTURE.md)** - PASE/CASE session establishment
- **[MATTER_ICD_IMPLEMENTATION.md](MATTER_ICD_IMPLEMENTATION.md)** - ICD cluster implementation details
