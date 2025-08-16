# AI-Generated LoRaWAN Driver List

## Overview
This document maintains a comprehensive list of all AI-generated drivers for the LwDecode framework with version tracking and generation dates.

## Versioning Strategy
- v<major>.<minor>.<fix>
  - **major**: Increased only when official sensor specs change from vendor (starts at 1)
  - **minor**: Increased only when fresh regeneration is requested (resets to 0 on major change)
  - **fix**: Increased on all other cases (resets to 0 on minor change)
- All publish dates must be greater than 2025-08-13 (framework start date)

## Driver Registry

### Dragino
| Model | Version | Date | Channels | Model Version | Prompt Ver | Description |
|-------|---------|------|----------|---------------|------------|-------------|
| D2x | 1.0.0 | 2025-08-16 | 15/15 | D20/D20S/D22/D23-LB/LS | v2.1.9 | Multi-probe temperature sensor series |

### Milesight
| Model | Version | Date | Channels | Model Version | Prompt Ver | Description |
|-------|---------|------|----------|---------------|------------|-------------|
| AM300 | 1.1.0 | 2025-08-15 | 20/20 | AM300(L) Series | v2.1.8 | 8-in-1 indoor air quality monitor |
| WS101 | 1.0.0 | 2025-08-15 | 8/8 | WS101 | v2.1.8 | Smart button with multiple press types |
| WS202 | 1.1.0 | 2025-08-15 | 9/9 | WS202 | v2.1.8 | PIR & Light sensor for motion and illuminance detection |
| WS301 | 1.1.0 | 2025-08-15 | 10/10 | WS301 | v2.1.8 | Magnetic door/window sensor |
| WS523 | 2.0.0 | 2025-08-15 | 33/33 | WS523 | v2.1.8 | Portable smart socket with power monitoring |
| WS52x | 2.0.0 | 2025-08-15 | 33/33 | WS52x Series | v2.1.8 | Smart socket series with comprehensive power monitoring |

## Coverage Statistics

### By Vendor
- **Dragino**: 1 driver, 15 total channels
- **Milesight**: 6 drivers, 113 total channels

### Total
- **Drivers**: 7
- **Channels**: 128
- **Coverage**: 100% (all documented channels implemented)

## Technical Standards

All drivers follow these standards:
- Berry 0.1.10+ syntax compliance
- Tasmota 13.0+ API compatibility
- LwSensorFormatter_cls() framework usage
- No Berry reserved words as variables
- Emoji-first display format
- Memory optimization (<500 lines per driver)
- Complete channel coverage (100%)
- Global node storage for multi-device support
- Test command registration with auto-cleanup
- Node management commands (stats, clear)

## Driver Features

### Core Features (All Drivers)
- ✅ Complete uplink decoding (100% coverage)
- ✅ Global persistent node storage
- ✅ Recovery after driver reload
- ✅ Device trend tracking (battery, power, energy)
- ✅ Device reset detection
- ✅ Last seen timestamp tracking
- ✅ Test command with port-specific payloads
- ✅ Node statistics and management

### Display Features
- ✅ Emoji-first single-line format
- ✅ Dynamic value display (only show when relevant)
- ✅ Age indicator for stale data (>1 hour)
- ✅ Status indicators for alerts/errors
- ✅ Compact notation for large numbers

## Validation Checklist

Each driver has been validated for:
- ✅ No use of Berry reserved words ('type', 'class', etc.)
- ✅ Correct LwSensorFormatter_cls() usage
- ✅ Complete uplink decoding coverage
- ✅ Proper error handling with try/except blocks
- ✅ Memory-efficient implementation
- ✅ Consistent emoji usage per reference guide
- ✅ Global node storage initialization
- ✅ Test command registration with cleanup
- ✅ Documentation with test examples for ALL uplink types

## Changelog

### 2025-08-15: Template v2.1.9 Command Prefix Update
- Updated AI template to v2.1.9 with "Lw" prefix requirement
- ALL commands now must start with "Lw" prefix for consistency
- Repository cleanup: removed deprecated drivers from other vendors
- Updated driver list to reflect current repository state
- All Milesight drivers updated to latest template v2.1.8
- Enhanced framework integration across all drivers
- Improved global storage handling
- Added comprehensive downlink commands

### Previous Changes
- v2.0.0 complete regenerations for WS52x and WS523
- Framework v2.0.0 standardization
- Global node storage implementation
- Test command registration

---
*Last Updated: 2025-08-15 - Repository cleanup and driver list recreation*

---

*Author: [ZioFabry](https://github.com/ZioFabry)*