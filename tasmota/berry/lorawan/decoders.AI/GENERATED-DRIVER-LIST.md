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
| DDS75-LB | 1.0.0 | 2025-08-16 | 8/8 | DDS75-LB/LS | v2.1.8 | Ultrasonic distance detection sensor |
| LDS02 | 1.0.0 | 2025-08-16 | 8/8 | LDS02 | v2.1.8 | Magnetic door sensor with event counting |

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
- **Dragino**: 3 drivers, 31 total channels
- **Milesight**: 6 drivers, 113 total channels

### Total
- **Drivers**: 9
- **Channels**: 144
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

### 2025-08-16: LDS02 Door Sensor Driver Addition
- Added Dragino LDS02 magnetic door sensor driver
- 8 channels implemented with 100% coverage (normal mode + EDC mode)
- 9 downlink commands: interval, EDC mode, reset, confirm, clear, alarm, ADR/DR, timeout, set count
- Magnetic reed switch door detection with ~10mm threshold
- Event counting with cumulative door open events tracking
- Last door open duration monitoring (up to 16.7M minutes)
- Timeout alarm feature for doors left open too long
- EDC mode (Event-Driven Counting) for power optimization
- Battery life: 16,000-70,000 uplinks with 2x AAA batteries
- State change detection (opened/closed/keep-alive events)
- Template v2.1.8 with "Lw" command prefix compliance

### 2025-08-16: DDS75-LB Driver Addition
- Added Dragino DDS75-LB distance detection sensor driver
- 8 channels implemented with 100% coverage (periodic data + device status)
- 5 downlink commands: interval, interrupt, delta detect, status request, poll
- Enhanced error handling for sensor disconnection and invalid readings
- Distance measurement with ultrasonic technology (280mm-7500mm range)
- Support for optional DS18B20 temperature sensor
- Delta detection mode for power-efficient operation
- Datalog feature for network outage recovery
- Template v2.1.8 with "Lw" command prefix compliance

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
*Last Updated: 2025-08-16 - Added LDS02 magnetic door sensor*

---

*Author: [ZioFabry](https://github.com/ZioFabry)*