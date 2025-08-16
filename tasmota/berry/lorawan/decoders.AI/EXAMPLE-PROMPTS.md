# LoRaWAN Decoder AI Prompt Library
# Version: 1.0.0 | Last Updated: 2025-08-16
# Framework: LwDecode for Tasmota Berry

---

## 📋 Overview

This document provides a comprehensive library of AI prompts for managing LoRaWAN decoders, framework operations, and development workflows. Each prompt is categorized and includes expected outcomes.

---

## 🏗️ Driver Management

### Driver Creation
```
Generate a new LoRaWAN decoder for [VENDOR] [MODEL] from the attached PDF specification. Use template v2.1.9 with complete uplink/downlink coverage, Lw command prefix, and enhanced error handling.
```

```
Create a minimal decoder for [VENDOR] [MODEL] - I only need basic uplink decoding for channels 0x03 (temperature) and 0x01 (battery). Skip downlinks and use simplified display.
```

### Driver Regeneration
```
Regenerate [VENDOR] [MODEL] driver from the existing MAP file using template v[VERSION]. Maintain version history and update changelog.
```

```
Upgrade [MODEL] driver to template v2.1.9 - preserve existing functionality but add enhanced error handling and Lw command prefix.
```

### Framework Upgrade
```
Upgrade all Milesight drivers to framework v1.8.0 - add enhanced error handling, safe decoder loading, and performance monitoring.
```

```
Update [MODEL].be to use new LwSensorFormatter features from framework v[VERSION]. Preserve existing display format but enhance with new capabilities.
```

---

## 📊 Versioning & Documentation

### Version Management
```
Increment [MODEL] to v[X.Y.Z] and update GENERATED-DRIVER-LIST.md with new version, generation date, and prompt version used.
```

```
Create version history table for [MODEL] showing all versions, dates, changes, and framework versions used.
```

### Cross-Reference Updates
```
Update all documentation references to [OLD_COMMAND] with [NEW_COMMAND] across vendor/milesight/ directory.
```

```
Synchronize version numbers between [MODEL].be header, [MODEL].md documentation, and GENERATED-DRIVER-LIST.md entry.
```

---

## 🔧 Code Patching

### Single Driver Patches
```
Fix command naming in [MODEL].be - add "Lw" prefix to all Tasmota commands and update relative markdown documentation.
```

```
Add battery trend tracking to [MODEL] driver using global storage pattern. Store last 10 readings and provide statistics.
```

### Cross-Driver Patches
```
Apply consistent emoji usage across all Milesight drivers - use 🌡️ for temperature, 💧 for humidity, 🔋 for battery.
```

```
Update all drivers to use lwdecode.log_error() instead of print() for error reporting. Maintain same error context.
```

### Vendor-Wide Operations
```
Standardize all Milesight drivers to use consistent global storage pattern: [VENDOR]_[MODEL]_nodes with same node data structure.
```

```
Update all vendor/milesight/ drivers to template v2.1.9 requirements - Lw prefix, enhanced error handling, safe operations.
```

---

## 🎨 Display & UI Management

### Emoji Management
```
Update emoji-reference.md with new mappings for air quality sensors: CO2 🌬️, TVOC 🏭, PM2.5 🌫️, PM10 💨.
```

```
Standardize motion detection emojis across all PIR sensors - use 🚶 for motion detected, 🚫 for no motion.
```

### Display Optimization
```
Optimize [MODEL] display for single-line format - use emoji-only display, remove text labels, prioritize most important sensors.
```

```
Add multi-line display support to [MODEL] for devices with 8+ sensors. Group related sensors and use next_line() transitions.
```

---

## 🏭 Vendor Operations

### Milesight Specific
```
Add new Milesight WS303 temperature/humidity sensor - use WS301 as template but replace door sensor with environmental monitoring.
```

```
Create Milesight vendor summary - list all supported models, total channel coverage, common features, and development roadmap.
```

### Multi-Vendor Operations
```
Create vendor comparison table showing decoder capabilities, channel coverage, and framework compatibility across all supported vendors.
```

```
Implement vendor-agnostic battery monitoring pattern that works across all vendors with different battery reporting formats.
```

---

## 📑 Documentation Management

### Documentation Refresh
```
Update README.md with current framework v1.8.0 features - enhanced error handling, safe operations, performance monitoring.
```

```
Regenerate [MODEL].md from current driver implementation - extract all channels, commands, test examples, and performance metrics.
```

### Reference Updates
```
Update FRAMEWORK.md with new helper functions added in v1.8.0 - document parameters, return values, and usage examples.
```

```
Create comprehensive testing guide showing payload examples for all [MODEL] uplink types with expected JSON responses.
```

---

## 🧪 Testing & Validation

### Driver Testing
```
Generate comprehensive test suite for [MODEL] - create test payloads for all uplink types, validate all downlink commands, verify error handling.
```

```
Create validation checklist for [MODEL] v[VERSION] - verify Berry syntax, framework usage, channel coverage, command functionality.
```

### Performance Testing
```
Analyze memory usage of [MODEL] driver - identify optimization opportunities, measure decode time, validate stack usage.
```

```
Benchmark all Milesight drivers for decode performance - create comparison table with timing metrics and memory usage.
```

---

## 🔄 Maintenance Operations

### Cleanup Operations
```
Remove deprecated functions from [MODEL] driver - clean up commented code, unused variables, outdated patterns.
```

```
Standardize code formatting across all vendor/milesight/ drivers - consistent indentation, spacing, comment style.
```

### Quality Assurance
```
Code review [MODEL] v[VERSION] - check Berry syntax, framework compliance, error handling, memory efficiency, documentation completeness.
```

```
Validate all Milesight drivers against template v2.1.9 requirements - Lw prefix, error handling, global storage, test commands.
```

---

## 🚀 Advanced Operations

### Framework Development
```
Design new framework feature for automatic payload validation - add CRC checking, size validation, range checking for sensor values.
```

```
Implement framework caching system - cache decoded data between updates, reduce memory allocation, improve performance.
```

### Integration Features
```
Create Home Assistant integration helper for [MODEL] - generate MQTT discovery topics, sensor entities, device metadata.
```

```
Add Prometheus metrics export to framework - track decode times, error rates, node activity, battery levels.
```

---

## 📋 Workflow Patterns

### Development Workflow
```
Full development cycle for new vendor [VENDOR] - create directory structure, implement first driver, establish coding standards, document patterns.
```

```
Emergency patch workflow for critical bug in [MODEL] - identify issue, create fix, test validation, update documentation, deploy.
```

### Release Management
```
Prepare release package for framework v1.8.0 - update all drivers, refresh documentation, validate compatibility, create changelog.
```

```
Create migration guide from framework v1.7.x to v1.8.0 - document breaking changes, provide upgrade instructions, create compatibility matrix.
```

---

## 📝 Template Usage

### Pattern Templates
```
Use [EXISTING_MODEL] as template for new [NEW_MODEL] driver - adapt channel mappings, update device info, modify display format.
```

```
Create driver family template for [VENDOR] sensor series - extract common patterns, create base class, implement inheritance.
```

### Customization Templates
```
Create custom formatter for [SENSOR_TYPE] measurements - define unit, format, emoji, validation rules for reuse across drivers.
```

```
Implement vendor-specific authentication pattern for [VENDOR] encrypted payloads - add decryption, key management, validation.
```

---

## ⚡ Quick Actions

### Rapid Fixes
```
Quick fix: add missing Lw prefix to [COMMAND] in [MODEL] driver and documentation.
```

```
Quick update: change emoji for [SENSOR] from [OLD] to [NEW] across all applicable drivers.
```

### Instant Operations
```
Add [SENSOR_TYPE] formatter to framework: unit=[UNIT], format=[FORMAT], emoji=[EMOJI].
```

```
Update driver registry: [MODEL] v[VERSION] generated [DATE] with template v[TEMPLATE_VERSION].
```

---

## 🎯 Best Practices

### Prompt Guidelines
- Be specific about version numbers and framework versions
- Include expected file paths and naming conventions  
- Specify template version for consistency
- Request validation and testing steps
- Ask for documentation updates when changing code

### Response Validation
- Verify Berry syntax compliance
- Check framework pattern usage
- Validate command naming conventions
- Confirm version consistency across files
- Test generated commands work as expected

---

*This prompt library evolves with framework development. Suggest new patterns as functionality expands.*

---

**Version History:**
- v1.0.0 (2025-08-16): Initial comprehensive prompt library with 50+ categorized examples

---

*Author: [ZioFabry](https://github.com/ZioFabry)*