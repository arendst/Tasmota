# Matter ICD (Intermittently Connected Device) Implementation

## Overview

The ICD Management Cluster (0x0046) is always enabled in Tasmota's Matter implementation per Matter 1.4.1 specification. Since Tasmota devices are mains-powered WiFi devices that are always connected, the cluster reports the device as a basic SIT (Short Idle Time) device without optional features.

## What is ICD?

ICD (Intermittently Connected Device) is a Matter feature that allows devices to communicate their availability patterns to controllers. Per Matter 1.4.1 spec section 9.17:

- **SIT (Short Idle Time)**: Devices with idle periods < 15 seconds - used for always-on devices
- **LIT (Long Idle Time)**: Devices with idle periods >= 15 seconds - used for battery-powered sleepy devices

Tasmota devices use **SIT mode** since they are always connected via WiFi.

## Implementation Details

### ICD Management Cluster (0x0046)

The ICD Management Cluster is always present on the Root endpoint (endpoint 0).

Per Matter 1.4.1 spec section 9.17.4, the cluster has optional features:
- **CIP** (Check-In Protocol Support): For LIT devices to notify clients
- **UAT** (User Active Mode Trigger): For user-triggered wake
- **LITS** (Long Idle Time Support): For LIT operation mode
- **DSLS** (Dynamic SIT/LIT Support): For switching between modes

Tasmota implements **none of these optional features** (FeatureMap = 0x00) since it's an always-on device.

### Supported Attributes (per spec section 9.17.6)

| Attribute | ID | Type | Value | Conformance | Description |
|-----------|-----|------|-------|-------------|-------------|
| IdleModeDuration | 0x0000 | uint32 | 1 | Mandatory | 1 second (minimum per spec) |
| ActiveModeDuration | 0x0001 | uint32 | 300 | Mandatory | 300ms (spec default) |
| ActiveModeThreshold | 0x0002 | uint16 | 300 | Mandatory | 300ms (spec default) |

### Attributes NOT Implemented (require optional features)

| Attribute | ID | Conformance | Reason |
|-----------|-----|-------------|--------|
| RegisteredClients | 0x0003 | CIP | Requires Check-In Protocol |
| ICDCounter | 0x0004 | CIP | Requires Check-In Protocol |
| ClientsSupportedPerFabric | 0x0005 | CIP | Requires Check-In Protocol |
| UserActiveModeTriggerHint | 0x0006 | UAT | Requires User Active Mode Trigger |
| UserActiveModeTriggerInstruction | 0x0007 | UAT | Requires User Active Mode Trigger |
| OperatingMode | 0x0008 | LITS | Requires Long Idle Time Support |
| MaximumCheckInBackoff | 0x0009 | CIP | Requires Check-In Protocol |

### Cluster Configuration

- **Cluster Revision**: 3 (Matter 1.4.1)
- **Feature Map**: 0x00 (no optional features)

### mDNS Advertisement

Per Matter 1.4.1 spec section 4.3.4, the following TXT records are included:

| Key | Value | Description |
|-----|-------|-------------|
| SII | 500 | SESSION_IDLE_INTERVAL in ms (spec default) |
| SAI | 300 | SESSION_ACTIVE_INTERVAL in ms (spec default) |

**Note**: The `ICD` key is NOT advertised because per spec: "The key SHALL NOT be provided by a Node that does not support the ICD Long Idle Time operating mode."

### Files Modified

1. **Matter_Plugin_1_Root.be**
   - ICD Management Cluster (0x0046) with attributes [0,1,2]
   - Fixed attribute values per spec defaults

2. **Matter_Plugin_0.be**
   - Cluster revision 3 in CLUSTER_REVISIONS
   - Feature map 0x00 in FEATURE_MAPS

3. **Matter_z_Commissioning.be**
   - SII/SAI values in mDNS PASE announcement (spec defaults)

## Why This Implementation?

1. **Spec Compliance**: Only mandatory attributes are implemented; optional features (CIP, UAT, LITS) are not needed for always-on devices

2. **Simplicity**: No configuration needed since Tasmota devices are always-on WiFi devices

3. **Controller Compatibility**: Controllers can read the ICD cluster to understand the device is always reachable

## Spec References

- Matter 1.4.1 Core Specification
- Section 9.17: ICD Management Cluster
- Section 4.3.4: Common TXT Key/Value Pairs
- Section 4.13.1: Session Parameters

---
*Implementation Date: January 2026*
*Based on Matter 1.4.1 Core Specification*
