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

### Browan
| Model | Version | Date | Channels | Description |
|-------|---------|------|----------|-------------|
| EBL-OC | 1.0.0 | 2025-08-14 | 5/5 | Open/Close sensor with environmental monitoring |

### Dragino
| Model | Version | Date | Channels | Description |
|-------|---------|------|----------|-------------|
| LHT65 | 1.0.0 | 2025-08-14 | 12/12 | Temperature, humidity, GPIO, ADC sensor |
| SE01-LB | 1.0.0 | 2025-08-14 | 8/8 | Soil monitoring with EC and dielectric |

### Milesight
| Model | Version | Date | Channels | Description |
|-------|---------|------|----------|-------------|
| AM300 | 1.1.0 | 2025-08-15 | 20/20 | 8-in-1 indoor air quality monitor |
| WS202 | 1.0.0 | 2025-08-14 | 9/9 | PIR & Light sensor for motion and illuminance detection |
| WS301 | 1.0.0 | 2025-08-14 | 10/10 | Magnetic door/window sensor |
| WS523 | 1.0.0 | 2025-08-14 | 33/33 | Smart socket with power monitoring |
| WS52x | 2.0.0 | 2025-08-15 | 33/33 | Smart socket series - REGENERATED with template v2.1.8 |

## Coverage Statistics

### By Vendor
- **Browan**: 1 driver, 5 total channels
- **Dragino**: 2 drivers, 20 total channels  
- **Milesight**: 5 drivers, 105 total channels

### Total
- **Drivers**: 8
- **Channels**: 130
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
- ✅ Battery trend tracking
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

### 2025-08-15: WS52x Complete Regeneration
- WS52x updated to v2.0.0 with complete regeneration using template v2.1.8
- Enhanced framework integration with v1.8.0 error handling
- Complete uplink/downlink coverage (33/33 channels, 12/12 commands)
- Added comprehensive power monitoring formatters
- Improved global storage handling with power/energy trends

### 2025-08-14: Framework v2.0.0 Standardization
- All drivers renumbered to v1.0.0 following new versioning strategy
- Standardized publish date to 2025-08-14 (post-framework start)
- Added global node storage to all drivers
- Added test command registration to all drivers
- Enhanced documentation with complete test examples

---
*Last Updated: 2025-08-15 - WS52x v2.0.0 regeneration*

---

*Author: [ZioFabry](https://github.com/ZioFabry)*