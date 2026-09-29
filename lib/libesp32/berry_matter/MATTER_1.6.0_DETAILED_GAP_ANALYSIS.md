# Matter 1.6.0 Detailed Gap Analysis for Tasmota Implementation

## Document Information
- **Analysis Date**: September 2026
- **Matter Spec Version**: 1.6.0 (June 17, 2026)
- **Tasmota Implementation**: Berry Matter module (`lib/libesp32/berry_matter/`)
- **Purpose**: Identify gaps between Matter 1.6.0 specification and current Tasmota implementation, relative to Matter 1.4.1 baseline
- **Scope**: Updates from versions 1.5, 1.5.1, and 1.6.0; **Groupcast (0x0006) explicitly excluded** (remains disabled-by-default upstream; deferred to future Matter 1.6.1+ evaluation)
- **Reference Documents**: 
  - GitHub releases: [v1.5.0.0](https://github.com/project-chip/connectedhomeip/releases/tag/v1.5.0.0), [v1.5.1.0](https://github.com/project-chip/connectedhomeip/releases/tag/v1.5.1.0), [v1.6.0.0](https://github.com/project-chip/connectedhomeip/releases/tag/v1.6.0.0)
  - CSA Specification: Matter 1.6 Application Cluster Specification (23-27350, June 16, 2026)
  - CSA Specification: Matter 1.6.1 Device Library Specification (23-27351, September 16, 2026)

---

## Executive Summary

The Tasmota Matter implementation is currently aligned with **Matter 1.4.1** (DataModelRevision = 18). Progressing to **Matter 1.6.0** (target DataModelRevision = 20) requires implementing:

### Major New Features (Matter 1.5+)

1. **Closures Unified Control** — Refactored from individual cluster models (Window Covering, Door Lock) to modular Closure architecture with Closure Control (0x0104) and Closure Dimension (0x0105) clusters, device type Closure (0x0230) and Closure Panel (0x0231) subtypes
2. **Energy Management Framework** — New clusters for tariff/pricing (Commodity Price 0x0095, Commodity Tariff 0x0700), device efficiency (Device Energy Management 0x0098), EVSE control (Energy EVSE 0x0099, Energy EVSE Mode 0x009D), Water Heater management (0x0094, 0x009E), Energy Preference (0x009B)
3. **Soil Measurement Sensor** — New device type Soil Sensor (0x0045) with cluster Soil Measurement (0x0430)
4. **Device Type Enhancements** — Doorbell device types (0x0148, 0x0141, 0x0143) introduced in Matter 1.5
5. **Architectural Migration** — 25+ clusters migrated from legacy Ember codegen to code-driven `DefaultServerCluster` model (upstream infrastructure change; minimal Tasmota impact)

### Current Compliance Gaps

**HIGH PRIORITY** (significant user value):
- Energy Management clusters missing — Tasmota has extensive energy drivers (PZEM, HLW8012, etc.) but no Matter exposure
- Closures Unified architecture not implemented — refactor from current scattered cluster model

**MEDIUM PRIORITY** (framework evolution):
- Soil Sensor device type and cluster missing (low effort, high semantic value for gardening/irrigation)
- Doorbell device types missing (lower priority than energy/closures)

**LOW PRIORITY** (out of scope for ESP32):
- Cameras/WebRTC (v1.5 addition) — unrealistic for ESP32 Tasmota; Tasmota has no native camera stack
- Media management clusters (v1.6.1) — defer to future evaluation

### DataModelRevision Target
- **Current**: 18 (Matter 1.4.1)
- **Target**: 20 (Matter 1.6.0)
- **Future**: 21 (Matter 1.6.1; Groupcast finalization not planned for this release)

---

## Part 1: New Device Types (Matter 1.5+)

### 1.1 Closures Architecture — NEW UNIFIED MODEL

**Device Types:**

| Device Type | ID | Revision | Role | Superset | Notes |
|---|---|---|---|---|---|
| Closure | 0x0230 | 1 | Parent composite | — | Umbrella for Window Covering, Door Lock, Cabinet, Garage Door variants via semantic tags |
| Closure Panel | 0x0231 | 1 | Child endpoint | — | Sub-component of Closure, represents single degree of freedom (panel, slat, etc.) |

**Key Clusters (Application Cluster Spec 1.6.0):**

| Cluster | ID | Revision | Purpose | Mandatory/Optional |
|---|---|---|---|---|
| Closure Control | 0x0104 | 1 | Unified control interface for closure movement/state | M on Closure |
| Closure Dimension | 0x0105 | 1 | Controls single axis/panel of composed closure | M on Closure Panel |

**Closure Control (0x0104) Attributes:**

| Attribute | ID | Type | Conformance | Description |
|---|---|---|---|---|
| MainState | 0x0000 | enum8 | M | Closed/Open/Jammed/Unknown |
| TargetState | 0x0001 | enum8 | C | Target position after command |
| RemainingTime | 0x0002 | uint16 | O | Milliseconds to reach target |
| ... | ... | ... | ... | ... |

**Closure Dimension (0x0105) Attributes:**

| Attribute | ID | Type | Conformance | Description |
|---|---|---|---|---|
| CurrentPositionLiftPercent100ths | 0x0000 | uint16 | C | Current lift position (0-10000 = 0-100%) |
| CurrentPositionTiltPercent100ths | 0x0001 | uint16 | C | Current tilt position |
| MovementType | 0x0002 | enum8 | M | Translational/Rotational/Modulation |
| ... | ... | ... | ... | ... |

**Relationship**: Closure device (parent) may have 1+ Closure Panel child endpoints, each with Closure Dimension cluster. Closure Control on parent acts as supervisor.

**Implementation Impact on Tasmota:**
- Current `Matter_Plugin_2_Shutter.be` (Window Covering, 0x0202) and `Matter_Plugin_2_Thermostat.be` (Door Lock, 0x0301) use legacy clusters
- Closure architecture allows unified control of: window coverings (shutters), doors, garage doors, cabinets, gates
- Tasmota devices (Shutter relays, RF/IR remote controls) map naturally to Closure model
- ✅ **Garage Door implemented**: `Matter_Plugin_2_GarageDoor.be` (`garage`) exposes Closure (0x0230, Rev 1)
  + Closure Control (0x0104, Rev 1, feature PS) only, reusing the Shutter's `ShutterPosition<x>` /
  ShutterInvert data source but reporting MainState/OverallCurrentState/OverallTargetState instead
  of the legacy Window Covering attributes. Descriptor TagList carries the Closure namespace
  GarageDoor tag. `Matter_Plugin_9_Virt_GarageDoor.be` (`v_garage`) exposes the same Matter model
  with state supplied through `MtrUpdate`. Window Covering (0x0202) is left untouched for shutters/blinds.
- **Remaining effort**: Low-Medium (3-5 days) — Closure Panel (0x0231) + Closure Dimension (0x0105)
  only needed if a future closure requires percentage lift/tilt; Door Lock migration still open

### 1.2 Soil Sensor — NEW SIMPLE DEVICE TYPE

**Device Type:**

| Field | Value |
|---|---|
| ID | 0x0045 |
| Name | Soil Sensor |
| Revision | 1 (not yet bumped in 1.6.1 spec snapshot) |

**Mandatory Clusters:**

| Cluster | ID | Attributes |
|---|---|---|
| Basic Information | 0x0028 | Standard |
| Descriptor | 0x001D | Standard |
| Identify | 0x0003 | Standard |
| Soil Measurement | 0x0430 | soil moisture, temperature (optional) |

**Soil Measurement Cluster (0x0430, Revision 1):**

| Attribute | ID | Type | Conformance | Notes |
|---|---|---|---|---|
| SoilMoistureMeasurement | 0x0000 | MeasurementAccuracyStruct | M | Moisture reading + accuracy bounds |
| SoilMoistureMeasurementLimits | 0x0001 | StructType | O | Min/max thresholds |
| SoilTemperatureMeasurement | 0x0002 | MeasurementAccuracyStruct | O | Temperature (optional; many soil sensors include this) |
| ... | ... | ... | ... | ... |

**Tasmota Mapping:**
- Existing humidity sensors (DHT, BME280, etc.) used in Tasmota can also report soil moisture if connected to soil probe
- Current `Matter_Plugin_3_Sensor_Humidity.be` could be extended or new `Matter_Plugin_3_Sensor_Soil.be` created
- **Estimated effort**: Low (3-5 days) — create new device type plugin, reuse humidity/temperature measurement logic

### 1.3 Doorbell Device Types — SIMPLE, FOR FUTURE

**Device Types (Matter 1.5):**

| Device Type | ID | Revision | Purpose |
|---|---|---|---|
| Doorbell | 0x0148 | 1 | Simple switch; triggers chime |
| Audio Doorbell | 0x0141 | 1 | Doorbell + audio speaker |
| Video Doorbell | 0x0143 | 1 | Doorbell + camera |

**Priority**: Low for Tasmota (Doorbell as On/Off Switch is straightforward, but camera variant unrealistic for ESP32).

---

## Part 2: New Clusters (Matter 1.5+)

### 2.1 Energy Management Framework — HIGH PRIORITY

**Overview Table (Application Cluster Spec Section 9):**

| Cluster | ID | Revision | Conformance | PICS | Purpose |
|---|---|---|---|---|---|
| Commodity Price | 0x0095 | 4 | Base | SEPR | Real-time/forecasted pricing for gas, energy, water |
| Commodity Tariff | 0x0700 | 1 | Base | SETRF | Tariff schedules and rate structure |
| Device Energy Management | 0x0098 | 4 | Base | DEM | Power adjustment, demand response, forecasting |
| Energy EVSE | 0x0099 | — | Base | — | EV charging control (out of scope for Tasmota) |
| Energy EVSE Mode | 0x009D | — | Base | — | EVSE mode switching |
| Water Heater Management | 0x0094 | — | Base | — | Water heater control (out of scope for Tasmota) |
| Water Heater Mode | 0x009E | — | Base | — | Water heater mode |
| Energy Preference | 0x009B | — | Base | — | User energy consumption preferences |

**Priority for Tasmota:**
1. **Commodity Price (0x0095)** — Receive pricing from grid/utility device; enable smart scheduling
2. **Commodity Tariff (0x0700)** — Schedule-based tariff (time-of-use, peak hours, etc.)
3. **Device Energy Management (0x0098)** — Expose existing power measurements (PZEM, HLW8012) + power adjustment hints
4. **Energy EVSE (0x0099)** — Out of scope (Tasmota has no EV charger driver)
5. **Water Heater Management (0x0094)** — Out of scope (niche use case)

**Tasmota Integration Points:**

- **Commodity Price read**: Receive pricing signals from grid/utility controller (e.g., Octopus Agile tariffs in UK)
  - Application: Smart water heater, pool heater, charging scheduler can adjust based on price
- **Device Energy Management**: Extend existing `Matter_Plugin_3_OnOff_Power.be` or create `Matter_Plugin_Sensor_Power.be`
  - Map to Tasmota Energy drivers: PZEM-004T, HLW8012, BL0937, BL0940, ADE7953, CSE7766, SDM120, SDM630
  - Expose: current power (W), cumulative energy (kWh), voltage, current, frequency, power factor
- **Energy Preference**: Tasmota device can expose user preference (e.g., "prefer cheap hours", "minimize carbon footprint")

**Estimated effort:**
- Commodity Price cluster read (no write initially): 1 week
- Commodity Tariff cluster: 1 week
- Device Energy Management: 1-2 weeks
- **Total**: 3-4 weeks

### 2.2 Soil Measurement Cluster

*See Part 1.2 above — standalone device type, low effort.*

### 2.3 Groupcast Cluster (0x0006) — **EXPLICITLY OUT OF SCOPE**

**Status**: Skeleton implemented in upstream Matter (v1.5), becomes full protocol in v1.6.0, finalized in v1.6.1. Disabled by default behind build flag due to footprint on constrained devices.

**Tasmota Status**: Groups cluster (0x0004) is today a **complete stub** with no command implementation (`return nil # TODO`). Group key management and group membership storage don't exist in the codebase.

**Decision**: Groupcast deferred to future evaluation (delta 1.6.1+, after Groupcast is proven stable in v1.6.1). Document only; no implementation planned for this release.

---

## Part 3: Cluster Refinements and Updates

### 3.1 Concentration Measurement Clusters — CONSTRAINT REMOVAL (v1.6.1)

**Note**: This change is part of the v1.6.1 release (PR #73585), **not v1.6.0**. Listed here for completeness in the 1.4.1→1.6+ journey.

**Affected Clusters (9 total):**

| Cluster | ID | Change in v1.6.1 |
|---|---|---|
| Carbon Monoxide Concentration | 0x040C | Uncertainty attribute: constraint removed |
| Carbon Dioxide Concentration | 0x040D | Uncertainty attribute: constraint removed |
| Nitrogen Dioxide Concentration | 0x0413 | Uncertainty attribute: constraint removed |
| Ozone Concentration | 0x0415 | Uncertainty attribute: constraint removed |
| PM2.5 Measurement | 0x042A | Uncertainty attribute: constraint removed |
| Formaldehyde Concentration | 0x042B | Uncertainty attribute: constraint removed |
| PM1.0 Measurement | 0x042C | Uncertainty attribute: constraint removed |
| PM10 Measurement | 0x042D | Uncertainty attribute: constraint removed |
| Total Volatile Organic Compounds | 0x042E | Uncertainty attribute: constraint removed |

**Tasmota Impact**: Minimal. Current implementation in `Matter_Plugin_2_Sensor_Air_Quality.be` does **not implement** Uncertainty attribute at all (neither reads nor validates), so the v1.6.1 change (removing constraints) has zero code impact. If Uncertainty is added in future, no constraint checks are needed.

---

## Part 4: Implementation Roadmap (Phases 1-4)

### Phase 1: Foundations — Update Root Node and Basic Clusters (Weeks 1-2)

**Objective**: Ensure Root Node and infrastructure clusters meet v1.6.0 minimum requirements.

1. **Update DataModelRevision**
   - File: `Matter_Plugin_1_Root.be:799`
   - Change: `return tlv_solo.set(0x05, 18)` → `return tlv_solo.set(0x05, 20)`
   - Effort: <1 day

2. **Complete Access Control Cluster (0x001F) — read missing attributes**
   - Current gap: `read_attribute` doesn't handle 0x001F; only `write_attribute` exists
   - Add three mandatory read-only attributes in `Matter_Plugin_1_Root.be`:
     - 0x0002: SubjectsPerAccessControlEntry (uint16) = 4
     - 0x0003: TargetsPerAccessControlEntry (uint16) = 3
     - 0x0004: AccessControlEntriesPerFabric (uint16) = 4
   - Effort: 2-3 days

3. **Update Group Key Management Cluster to Revision 4**
   - File: `Matter_Plugin_0.be:145`
   - Change: `0x003F: 2` → `0x003F: 4`
   - Complete missing mandatory attributes:
     - 0x0001: GroupTable (list, empty initially)
     - 0x0002: MaxGroupsPerFabric (uint16) = 4
     - 0x0003: MaxGroupKeysPerFabric (uint16) = 3
   - Effort: 3-5 days
   - **Note**: Revision jumps 2→4 because code currently at rev 2, spec moved to rev 4 (no rev 3 implementation exists)

### Phase 2: Energy Management Clusters (Weeks 3-5)

**Objective**: Expose Tasmota's extensive energy drivers through Matter Energy Management framework.

1. ⏹️ **Device Energy Management Cluster (0x0098, Rev 4)** — not applied to plugs
   - Not required by On/Off Plug-in Unit (0x010A) nor Electrical Sensor (0x0510); DEM belongs to Energy Smart Appliances (device type 0x050D, Rev 3: EVSE, water heater, heat pump, battery, solar)
   - Reporting-only DEM on a plug (static ESAState, no adjustment) brings no value to controllers
   - Revisit only if a Tasmota plugin models an actual ESA with load control

### Phase 2b: Commodity Price & Tariff (deferred)

1. **Commodity Price (0x0095, Rev 4)** and **Commodity Tariff (0x0700, Rev 1)**
   - Hosted only by Electrical Energy Tariff (0x0513), a child endpoint of Meter Reference Point (0x0512, with Identify)
   - Requires Descriptor TagList semantic tags (Commodity Tariff namespaces; Grid/Import/AC/Current in the basic topology) and TimeSyncCond on the Root Node
   - Commodity Tariff: 14+ attributes with deeply nested structs (DayEntry, DayPattern, CalendarPeriod, TariffComponent, TariffPeriod)
   - Prerequisites: Namespace Specification 1.6, parent/child virtual composition, TagList support
   - No native Tasmota data source; values would be pushed via `MtrUpdate`

### Phase 3: Closures Unified Architecture (Weeks 6-8)

**Objective**: Refactor Window Covering and Door Lock to new Closure model.

1. ✅ **Closure Device Type (0x0230, Rev 1) — Garage Door** — implemented
   - File: `Matter_Plugin_2_GarageDoor.be` (`garage`)
   - Clusters: Descriptor (TagList: Closure/GarageDoor), Identify (inherited), Closure Control (0x0104, Rev 1, PS)
   - MainState (Stopped/Moving), OverallCurrentState/OverallTargetState (Current/TargetPositionEnum),
     Stop (0x00) and MoveTo (0x01, timed invoke enforced) commands, MovementCompleted and
     SecureStateChanged events, fed by the same `ShutterPosition<x>` / ShutterInvert logic as the
     legacy Shutter plugin; MoveToSignaturePosition maps to fully open
   - Limitation: SecureState (= FullyClosed) follows Tasmota's time-estimated position, not an
     end-stop sensor; moves outside Tasmota (obstruction reversal, car remote, wall button) are
     not detected, so SecureState=true is not a confirmed closed state
   - No Closure Panel child endpoint: garage doors are modelled as a single
     enum-position closure (Closed/Open/Partial), not a percentage lift axis
   - Effort: implemented directly (skipped generic Closure/Closure Panel scaffolding below)

2. **Closure Panel Device Type (0x0231) — Child** (not yet implemented)
   - File: New `Matter_Plugin_Closure_Panel.be` (inherits from `Matter_Plugin_1_Device`)
   - Clusters: Descriptor, Identify, Closure Dimension (0x0105)
   - Needed only if a future closure requires percentage lift/tilt (e.g. gate, blind) composed as parent+panel
   - Effort: 3-5 days (follows Closure pattern)

3. **Migration Path for Existing Devices**
   - Current `Matter_Plugin_2_Shutter.be` (Window Covering, 0x0202) — kept as-is (Option A,
     legacy, widest controller support today) alongside the new `garage` Closure plugin (Option B)
   - Rationale: Window Covering (0x0202) remains the safer choice for shutters/blinds until
     Closure Control (0x0104) support is broader across controllers; Garage Door has no legacy
     Matter device type at all, so it was implemented directly against Closure (0x0230)
   - Reported controller support (unverified): Samsung SmartThings supports Closure Control; Home
     Assistant support is in progress (September 2026)

### Phase 4: Soil Sensor and Doorbells (Weeks 9-10)

**Objective**: Add simple sensor and doorbell device types for completeness.

1. ✅ **Soil Sensor (0x0045, Rev 1)** — implemented
   - Files: `Matter_Plugin_3_Sensor_Soil.be` (`soil`), `Matter_Plugin_9_Virt_Sensor_Soil.be` (`v_soil`), `Matter_Plugin_8_Bridge_Sensor_Soil.be` (`http_soil`, `mqtt_soil`)
   - Soil Measurement (0x0430, Rev 1): SoilMoistureMeasurementLimits + SoilMoistureMeasuredValue, fed by the Tasmota `Moisture` JSON key (percent)
   - Optional Temperature Measurement (0x0402) not composed on the same endpoint

2. **Doorbell Device Type (0x0148) — Optional**
   - File: New `Matter_Plugin_Doorbell.be`
   - Essentially an On/Off Switch; reuse existing On/Off Light logic
   - Effort: 2-3 days
   - Priority: Low (can be deferred to maintenance release)

### Phase 5: Testing and Validation (Weeks 11-12)

**Objective**: Multi-controller interoperability testing; confirm Matter 1.6.0 compliance.

- Test with Apple Home, Google Home, Amazon Alexa, Samsung SmartThings, Home Assistant
- Verify Closures control, Energy Management pricing signals, Soil Sensor readings
- Measure firmware size impact (especially Energy Management clusters)
- DataModelRevision = 20 confirmation

---

## Part 5: Files to Create/Modify

### New Files (Phase 2-4)

| File | Purpose | Priority | Phase |
|---|---|---|---|---|
| `Matter_Plugin_Energy_Price.be` | Commodity Price cluster | DEFERRED | 2b |
| `Matter_Plugin_Energy_Tariff.be` | Commodity Tariff cluster | DEFERRED | 2b |
| `Matter_Plugin_Sensor_Power.be` | Device Energy Management — not applicable to plugs (ESA only) | N/A | 2 |
| `Matter_Plugin_2_GarageDoor.be` | Closure device type (Garage Door, Closure Control only) | DONE | 3 |
| `Matter_Plugin_Closure_Panel.be` | Closure Panel device type (child, percentage lift/tilt closures) | MEDIUM | 3 |
| `Matter_Plugin_3_Sensor_Soil.be` | Soil Sensor device type | DONE | 4 |
| `Matter_Plugin_Doorbell.be` | Doorbell device type | LOW | 4 |

### Files to Modify

| File | Changes | Phase |
|---|---|---|
| `Matter_Plugin_1_Root.be` | DataModelRevision (18→20), complete Access Control read, add GKM attributes | 1 |
| `Matter_Plugin_0.be` | CLUSTER_REVISIONS: 0x003F: 2→4; FEATURE_MAPS updates for new clusters | 1 |
| `Matter_Plugin_z_All.be` | No changes (auto-discovery of new plugins) | — |

---

## Part 6: Delta Summary Table (1.4.1 → 1.6.0)

| Category | 1.4.1 | 1.6.0 | Status | Effort |
|---|---|---|---|---|
| **DataModelRevision** | 18 | 20 | ⚠️ Not updated | <1 day |
| **Closures clusters** | Window Covering (legacy) | Closure Control (Garage Door, `garage`) ✅ Done; Closure Dimension (panel-based closures) still missing | ⚠️ Partial | 3-5 days remaining |
| **Energy Management** | 0x0090/0x0091 only | Add Commodity Price/Tariff, Device EM, EVSE | ❌ Missing | 3-4 weeks |
| **Soil Measurement** | None | Soil Sensor (0x0045) + cluster | ✅ Done | — |
| **Doorbell** | None | 0x0148/0x0141/0x0143 | ❌ Missing | 1 week |
| **Access Control** | Partial | Complete read + rev updates | ⚠️ Partial | 3-5 days |
| **Group Key Management** | Rev 2 | Rev 4 | ⚠️ Outdated | 1 week |
| **Groupcast (0x0006)** | Skeleton | Full protocol (disabled by default) | ⏸️ Out of scope | — |

---

## Part 7: Notes and Deferred Items

### Groupcast Cluster (0x0006) — Explicitly Deferred

Groupcast is **not implemented** in this release cycle because:
1. It exists upstream (v1.5+) but disabled by default (build flag) due to footprint concerns
2. Tasmota's Groups cluster (0x0004) is today a complete stub (no command implementation)
3. Full group membership management infrastructure doesn't exist in the codebase
4. Groupcast is only finalized in v1.6.1 (PR #73585); v1.6.0 has incomplete conformance requirements
5. **Decision**: Defer to future evaluation after Groupcast stabilizes in v1.6.1+

### Cameras and Media Clusters (v1.5+) — Out of Scope

- WebRTC, Camera AV Stream Management, Media Playback (v1.5)
- Media File Management, Audio Control (v1.6.1)
- **Reason**: Unrealistic for ESP32-based Tasmota; would require significant additional infrastructure (H.264 codec, TLS streaming, WebRTC stack)
- **Future**: Document as "Not applicable" in Device Type Library reference section

### EVSE and Water Heater — Out of Scope

- Energy EVSE (0x0099), Energy EVSE Mode (0x009D) — specialized EV charging
- Water Heater Management (0x0094), Water Heater Mode (0x009E) — niche appliances not common in Tasmota base
- **Future**: If requested, design as optional plugins (similar to current Thermostat plugin)

---

## Appendix: References

### CSA Specifications
- [Matter 1.6 Application Cluster Specification (23-27350)](https://csa-iot.org/wp-content/uploads/2026/06/23-27350-010_Matter-1.6-Application-Cluster-Specification.pdf) — June 16, 2026
- [Matter 1.6.1 Device Library Specification (23-27351)](https://csa-iot.org/wp-content/uploads/2026/09/23-27351-011_Matter-1.6.1-Device-Library-Specification.pdf) — September 16, 2026

### GitHub Releases
- [v1.5.0.0](https://github.com/project-chip/connectedhomeip/releases/tag/v1.5.0.0) — Matter 1.5.0 (Cameras, Closures, Energy Management)
- [v1.5.1.0](https://github.com/project-chip/connectedhomeip/releases/tag/v1.5.1.0) — Matter 1.5.1 (Camera enhancements)
- [v1.6.0.0](https://github.com/project-chip/connectedhomeip/releases/tag/v1.6.0.0) — Matter 1.6.0 (Architecture migration, Energy EVSE, Groupcast protocol)
- [v1.6.1.0](https://github.com/project-chip/connectedhomeip/releases/tag/v1.6.1.0) — Matter 1.6.1 (Groupcast finalization, AUX ACL, Media File Management, Audio Control) — **Future evaluation only**

### Related Issues/PRs
- [PR #73585](https://github.com/project-chip/connectedhomeip/pull/73585) — Matter 1.6.1 Device Model Update (Groupcast finalization, AUX ACL, Media/Audio) — **Not in current scope**

---

*Document Version: 1.0 (Matter 1.6.0 Analysis)*  
*Last Updated: September 2026*  
*Baseline: Tasmota Matter 1.4.1 implementation (DataModelRevision = 18)*  
*Target: Tasmota Matter 1.6.0 implementation (DataModelRevision = 20)*  
*Scope: Phases 1-4 roadmap; Groupcast and Cameras deferred*
