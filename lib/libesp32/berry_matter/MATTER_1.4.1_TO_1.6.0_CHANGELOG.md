# Matter 1.4.1 → 1.6.0 Changelog

## Overview

This document tracks major specification changes from Matter 1.4.1 (current Tasmota baseline) through Matter 1.5.0, 1.5.1, and 1.6.0 (target implementation level). Changes are organized by version and domain.

**Document Date:** September 2026  
**Tasmota Current Baseline:** Matter 1.4.1 (DataModelRevision = 18)  
**Implementation Target:** Matter 1.6.0 (DataModelRevision = 20)  
**Scope:** Features & clusters relevant to Tasmota smart home devices on ESP32  
**Note:** Matter 1.6.1 updates (Groupcast finalization, AUX ACL, Media File Management, Audio Control) are listed in "Future Evaluation" section only

---

## Matter 1.5.0 Release

**Release Date:** November 2025  
**GitHub Release:** https://github.com/project-chip/connectedhomeip/releases/tag/v1.5.0.0  
**Official Announcement:** [CSA Matter 1.5 Release](https://csa-iot.org/newsroom/matter-1-5-introduces-cameras-closures-and-enhanced-energy-management-capabilities/)

### Major New Features

#### 1. **Cameras & Video Streaming**
- New device types: Camera (primary), Video Doorbell (0x0143), Audio Doorbell (0x0141), Doorbell (0x0148)
- New clusters: Camera AV Stream Management, WebRTC data channel support
- Features: Multi-stream support, pan-tilt-zoom, detection/privacy zones, continuous/event-based recording
- **Tasmota applicability:** ⏹️ Out of scope (no native camera/codec stack on ESP32)

#### 2. **Unified Closures Architecture** ✅ **IN SCOPE**
- Replaces scattered Window Covering/Door Lock model with modular Closure architecture
- New device types:
  - Closure (0x0230): Parent composite device (window, door, garage door, cabinet, gate, etc.)
  - Closure Panel (0x0231): Child endpoint representing single degree of freedom
- New clusters:
  - **Closure Control (0x0104, Rev 1):** Unified state/control interface (MainState, TargetState, RemainingTime)
  - **Closure Dimension (0x0105, Rev 1):** Controls individual axis (lift, tilt, rotation, modulation)
- Semantic tags enable device differentiation (Window, Covering, Cabinet, etc.)
- **Tasmota applicability:** ✅ High value; Tasmota Shutter relays, RF/IR controllers map naturally

#### 3. **Energy Management Framework** ✅ **IN SCOPE**
- New clusters for real-time energy optimization, pricing, tariff, and carbon awareness
- Key new clusters:
  - **Commodity Price (0x0095, Rev 4):** Gas/Energy/Water pricing (forecasts, tier information)
  - **Commodity Tariff (0x0700, Rev 1):** Tariff schedules, time-of-use rates, rate structure
  - Device Energy Management (0x0098): Power adjustment, demand response, forecasting
  - Energy Preference (0x009B): User energy consumption preferences
  - Power Topology (0x009C): Power distribution model
- **Tasmota applicability:** ✅ High value; extends existing power measurement clusters (0x0090/0x0091) with tariff/pricing awareness; enables smart scheduling of energy-consuming appliances

#### 4. **New Sensor Device Types** ✅ **IN SCOPE**
- **Soil Sensor (0x0045):** Soil moisture and (optional) temperature measurement for gardening/irrigation
  - New cluster: **Soil Measurement (0x0430, Rev 1)** — SoilMoistureMeasurement, SoilTemperatureMeasurement
- **Tasmota applicability:** ✅ Medium value; existing humidity/temperature sensor plugins can extend to soil measurement

#### 5. **Groupcast Cluster (Server Skeleton)**
- New cluster: Groupcast (0x0006) — introduced as skeleton in v1.5.0
- Basis for multicast group communication (fully implemented in v1.6.0, finalized in v1.6.1)
- **Tasmota applicability:** ⏸️ Deferred (Groups cluster today is a stub; Groupcast planned for future release)

#### 6. **NFC-Based Commissioning**
- New feature: Setup payload support for NFC commissioning
- Support for concatenated QR codes
- **Tasmota applicability:** ⏹️ Out of scope (no NFC hardware on typical ESP32 boards)

#### 7. **Administrative Commissioning Enhancements**
- Updated AdminCommissioning cluster (0x003C)
- Enhanced PAKE verifier validation
- **Tasmota applicability:** ✅ Already implemented in current Root Node

#### 8. **Other Alchemy-Generated Clusters** (v1.5.0 infrastructure update)
- WiFi Network Management, RefrigeratorAlarm, MediaInput, KeyInput, LowPower, TimeSync, PowerTopology, ICDManagement, ContentControl, ApplicationLauncher, ApplicationBasic
- **Tasmota applicability:** ⏹️ Mostly out of scope; infra/media-focused

### Summary: Matter 1.5.0
- **New Device Types:** 4 major (Closure, Closure Panel, Soil Sensor, Camera-based Doorbells)
- **New Clusters:** 2 critical (Closure Control, Closure Dimension), 1 sensor (Soil Measurement), 6 energy (Commodity Price/Tariff, Device EM, EVSE, Water Heater, etc.), plus infra clusters
- **Tasmota Target:** Implement Closures, Energy Management (Commodity Price/Tariff), Soil Sensor; defer Groupcast and Cameras

---

## Matter 1.5.1 Release

**Release Date:** ~Q4 2025  
**GitHub Release:** https://github.com/project-chip/connectedhomeip/releases/tag/v1.5.1.0  
**Official Announcement:** [CSA Matter 1.5.1 Release](https://csa-iot.org/newsroom/matter-1-5-1-enhancing-camera-performance-and-expanding-device-flexibility/)

### Major Changes

#### 1. **Camera Performance Enhancements**
- Improved WebRTC frame rate handling
- Dynamic frame rate overrides
- ImageRotation attribute improvements
- **Tasmota applicability:** ⏹️ Out of scope

#### 2. **Device Flexibility Improvements**
- Enhanced superset device type relationships
- Refinements to device composition rules
- **Tasmota applicability:** ✅ General infrastructure benefit (affects how devices can be composed/related)

#### 3. **Energy Management Refinements** ⚠️ **MINOR UPDATES**
- Electrical Energy Measurement (0x0091) accuracy buffer improvements
- EVSE cluster stabilization (non-breaking updates)
- **Tasmota applicability:** ✅ Continue from v1.5.0 roadmap; no new clusters

### Summary: Matter 1.5.1
- Primarily a maintenance/refinement release; no major new clusters or device types
- Camera enhancements (out of scope)
- Tasmota roadmap: Continue v1.5.0 implementation items

---

## Matter 1.6.0 Release

**Release Date:** June 17, 2026  
**GitHub Release:** https://github.com/project-chip/connectedhomeip/releases/tag/v1.6.0.0  
**Commits Since v1.5.0:** 1,910  
**Official Announcement:** Matter 1.6 introduces broad architectural migration and stabilization of key features

### Major Changes

#### 1. **Architectural Migration: Ember → Code-Driven Data Model**
- **Scope:** 25+ clusters migrated from legacy Ember codegen to `DefaultServerCluster` architecture
- **Affected Clusters:** Core infrastructure, sensing/controls, safety, appliances, utilities
  - Examples: Groups, Scenes Management, Occupancy Sensing, Temperature Measurement, Air Quality, Switch, Level Control, Smoke CO Alarm, and others
- **Impact:** Decouples business logic from generated callbacks; cleaner separation of concerns
- **Tasmota applicability:** ✅ Infrastructure benefit; no immediate code changes needed (upstream migration), but future plugins should follow new pattern

#### 2. **Groupcast Protocol Stabilization** ⏸️ **DEFERRED FOR TASMOTA v1.6.0**
- Full protocol implementation and conformance requirements (build flag to disable for constrained devices)
- Integration with Thread Groupcast border-routing
- Comprehensive conformance testing suite
- **Tasmota applicability:** ⏸️ Deferred to v1.6.1+ evaluation (Groups cluster today is stub; requires substantial group membership infrastructure)

#### 3. **Energy Management Expansion** ✅ **IN SCOPE**
- **Energy EVSE Cluster (0x0099):** Stabilized; EV charging control (delegate implementations)
- **Energy EVSE Mode (0x009D):** EVSE mode configuration
- **Water Heater Reference Server:** New reference application (Tasmota out of scope, but cluster specs finalized)
- **Water Heater Management (0x0094), Water Heater Mode (0x009E):** Cluster definitions stabilized
- **Device Energy Management (0x0098):** Power adjustment, demand response, power range adjustments
- **Energy Preference (0x009B):** User preference attributes finalized
- **Commodity Price/Tariff:** Carried forward from v1.5 (no breaking changes)
- **Tasmota applicability:** ✅ Continue implementation of Commodity Price/Tariff from v1.5.0; add Device Energy Management cluster for demand response hints

#### 4. **Low Power & ICD Enhancements**
- Advanced Long Idle Time (LIT) and Short Idle Time (SIT) synchronization
- Check-in notification handling
- Message Reliability Protocol backoff improvements
- **Tasmota applicability:** ✅ Infrastructure benefit for battery-powered devices (current implementation is SIT mode only; no changes needed for WiFi-based Tasmota)

#### 5. **Camera & WebRTC Enhancements**
- WebRTC data channel support with enhanced session lifecycle safety
- PushAV Stream Transport implementation (AV Analytics, session management)
- **Tasmota applicability:** ⏹️ Out of scope

#### 6. **Security & Platform Modernization**
- Initial C++20 build compatibility
- PSA Crypto backend support
- TLS certificate management enhancements
- **Tasmota applicability:** ✅ Upstream infrastructure; no Tasmota action needed

### DataModelRevision

**Matter 1.6.0:** DataModelRevision = **20**  
(Increased from 18 in Matter 1.4.1)

**Update Required in Tasmota:**
```berry
# Matter_Plugin_1_Root.be, Basic Information cluster (0x0028), attribute 0x0000
return tlv_solo.set(0x05, 20)  # was 18
```

### Summary: Matter 1.6.0
- **Key Changes:** Architecture migration (Ember→code-driven), Groupcast stabilization, Energy Management completion, ICD/LIT-SIT sync
- **Tasmota Target:** 
  1. Update DataModelRevision to 20
  2. Complete Access Control & Group Key Management clusters in Root Node
  3. Implement Commodity Price/Tariff clusters and Device Energy Management (from v1.5.0 roadmap)
  4. Add Closures unified architecture
  5. Add Soil Sensor support
  6. Defer Groupcast, Cameras, EV charging, Water Heater (out of scope or lower priority)

---

## Version Comparison Matrix

| Feature | 1.4.1 | 1.5.0 | 1.5.1 | 1.6.0 |
|---|---|---|---|---|
| **Cameras** | ❌ | ✅ New | ✅ Enhanced | ✅ Stable |
| **Closures Unified** | ❌ | ✅ New | ✅ Stable | ✅ Stable |
| **Energy Management** | Partial (0x0090/0x0091) | ✅ New (Pricing/Tariff/EVSE) | ✅ Refined | ✅ Stabilized |
| **Groupcast** | ❌ | ⏸️ Skeleton | ⏸️ Skeleton | ⏸️ Full (disabled by default) |
| **Soil Sensor** | ❌ | ✅ New | ✅ Stable | ✅ Stable |
| **DataModelRevision** | 18 | 18 | 18 | **20** |
| **ICD LIT/SIT** | SIT only | SIT only | SIT only | ✅ Advanced |
| **Architecture** | Ember codegen | Mixed | Mixed | Mostly code-driven |

---

## Deferred to Matter 1.6.1 (Future Evaluation)

The following changes are part of Matter 1.6.1 (released September 2026, PR #73585) and **not included in Tasmota's current v1.6.0 roadmap**:

- **Groupcast finalization:** Access Control conformance, test commands/events, AUX ACL configuration
- **Access Control AUX feature:** New `AccessControlAuxiliaryTypeEnum`, `AuxiliaryAccessUpdated` event (finalized from provisional)
- **Media File Management (0x0511):** New cluster for file-based media storage
- **CommissioningProxy (0x0750):** New cluster for multi-controller commissioning
- **Audio Control:** New cluster for audio routing/output control
- **Group Key Management revision 4:** Removal of deprecated `GroupKeyMulticastPolicy` field
- **Concentration measurement refinements:** Uncertainty attribute constraint removal (v1.6.1 only; no impact if attribute not implemented)

**Tasmota v1.6.1 Evaluation Trigger:** After v1.6.0 is stable, reassess Groupcast readiness and AUX ACL feature value.

---

## Implementation Roadmap for Tasmota

### Phase 1: Root Node Infrastructure (v1.6.0 baseline)
- Update DataModelRevision: 18→20
- Complete Access Control cluster read attributes
- Bump Group Key Management to revision 4, add missing attributes
- **Timeframe:** 1-2 weeks

### Phase 2: Energy Management (v1.6.0, high-value features)
- ⏹️ Device Energy Management (0x0098, Rev 4) — **not applied to plugs**
  - Not required by On/Off Plug-in Unit (0x010A) nor Electrical Sensor (0x0510); it targets Energy Smart Appliances (EVSE, water heater, heat pump, battery, solar) via device type 0x050D (Rev 3)
  - A reporting-only DEM on a plug would expose a static ESAState with no control, bringing no value to controllers
  - Revisit only if a Tasmota plugin models an actual ESA (e.g. a heater/water-heater relay with load control)

### Phase 2b: Tariff & Price (deferred)
- Commodity Price (0x0095, Rev 4) and Commodity Tariff (0x0700, Rev 1)
- **Why deferred:** per the Device Library, both clusters are only hosted by the Electrical Energy Tariff device type (0x0513), which must be a child of a Meter Reference Point (0x0512) endpoint with Identify, and must carry Descriptor TagList semantic tags (Commodity Tariff Commodity/Chronology namespaces, plus Grid/Import/AC in the basic topology). The Root Node also needs the TimeSyncCond condition. Commodity Tariff alone carries 14+ attributes with deeply nested structs.
- **Prerequisites:** Namespace Specification 1.6 (to confirm namespace/tag IDs), a parent/child virtual composition (PartsList override, as in `Matter_Plugin_9_Virt_HVAC.be`), Descriptor TagList support (FeatureMap bit 0 on 0x001D)
- **Data source:** none native in Tasmota; values would be pushed via `MtrUpdate` (rules/scripts/MQTT)

### Phase 3: Closures Unified Architecture (v1.6.0, medium-value features)
- Closure device type (0x0230) with Closure Control (0x0104) and Closure Dimension (0x0105) clusters
- Closure Panel child device type (0x0231)
- Migration path for existing Window Covering/Shutter support
- **Timeframe:** 2-3 weeks
- **Integration:** Replaces/enhances current scattered cluster approach

### Phase 4: Sensors & Doorbells (v1.6.0, low-priority features)
- ✅ Soil Sensor device type (0x0045, Rev 1) with Soil Measurement cluster (0x0430, Rev 1)
  - Plugins: `soil` (`Matter_Plugin_3_Sensor_Soil.be`), `v_soil`, `http_soil`, `mqtt_soil`
  - Reads the Tasmota `Moisture` value (percent), e.g. Adafruit Seesaw soil sensor (xsns_81); filter like `SeeSoil#Moisture`
  - SoilMoistureMeasurementLimits: SoilMoisture (17), 0-100%, single accuracy range with PercentMax 10.00%
  - Optional soil Temperature Measurement (0x0402) not composed on the same endpoint; use a separate `temperature` endpoint
  - Autoconfiguration creates a `soil` endpoint for any sensor reporting `Moisture` (its `Temperature` gets its own `temperature` endpoint)
- Doorbell device type (0x0148) — optional, can defer to maintenance release

### Phase 5: Validation & Compliance Testing (v1.6.0)
- Multi-controller interoperability (Apple Home, Google Home, Alexa, SmartThings, Home Assistant)
- DataModelRevision = 20 verification
- Energy Management pricing signal testing
- Closures control testing
- **Timeframe:** 2 weeks

### Phase 6: Future (v1.6.1+, deprioritized)
- Groupcast protocol implementation (deferred until upstream stabilizes and Tasmota group infrastructure exists)
- Media File Management, Audio Control clusters
- Access Control AUX feature

---

## References

### Official CSA Specifications
- **Matter 1.4.1:** (Current baseline; no URL change from Tasmota docs)
- **Matter 1.5.0 Release:** [GitHub Release](https://github.com/project-chip/connectedhomeip/releases/tag/v1.5.0.0)
- **Matter 1.5.1 Release:** [GitHub Release](https://github.com/project-chip/connectedhomeip/releases/tag/v1.5.1.0)
- **Matter 1.6.0 Release:** [GitHub Release](https://github.com/project-chip/connectedhomeip/releases/tag/v1.6.0.0)
- **Matter 1.6 Application Cluster Specification:** [CSA 23-27350](https://csa-iot.org/wp-content/uploads/2026/06/23-27350-010_Matter-1.6-Application-Cluster-Specification.pdf) (June 16, 2026)
- **Matter 1.6.1 Device Library Specification:** [CSA 23-27351](https://csa-iot.org/wp-content/uploads/2026/09/23-27351-011_Matter-1.6.1-Device-Library-Specification.pdf) (September 16, 2026)

### Future Reference (v1.6.1, not in current scope)
- **Matter 1.6.1 Release:** [GitHub Release](https://github.com/project-chip/connectedhomeip/releases/tag/v1.6.1.0)
- **PR #73585:** [Matter 1.6.1 Device Model Update](https://github.com/project-chip/connectedhomeip/pull/73585)

### Related Tasmota Documentation
- [MATTER_1.6.0_DETAILED_GAP_ANALYSIS.md](MATTER_1.6.0_DETAILED_GAP_ANALYSIS.md) — Comprehensive specification gap analysis
- [ROOT_PLUGIN_COMPLIANCE_ANALYSIS.md](ROOT_PLUGIN_COMPLIANCE_ANALYSIS.md) — Matter 1.4.1→1.6.0 compliance roadmap for Root Node
- [MATTER_1.4.1_DETAILED_GAP_ANALYSIS.md](MATTER_1.4.1_DETAILED_GAP_ANALYSIS.md) — Original 1.4.1 baseline analysis

---

**Document Version:** 1.0  
**Last Updated:** September 2026  
**Maintainer:** Tasmota Matter Module Development  
**Status:** Ready for implementation roadmap (Phases 1-5 planned; Phase 6 deferred to v1.6.1+ evaluation)
