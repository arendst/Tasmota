# LwDecode Framework - Emoji Reference Guide

## Overview
This document maintains a comprehensive list of emojis used across all LwDecode drivers for consistent sensor representation in the web UI. Each emoji should be used consistently for the same measurement type across all drivers.

## Emoji Usage Table

| Emoji | Unicode | Name | Unit/Type | AI-Generated Drivers | Legacy Drivers | Description |
|-------|---------|------|-----------|---------------------|----------------|-------------|
| 🌡️ | U+1F321 | Thermometer | Temperature (°C) | LHT65, AM300, DW10 | D20, LHT52, SE01-L | Modern temperature display |
| ☀️ | U+2600 | Sun | Temperature (°C) | - | D20, LHT52, LHT65, SE01-L, DW10 | Legacy temperature display |
| 💧 | U+1F4A7 | Droplet | Humidity (%RH) | LHT65, AM300, DW10 | LHT52, LHT65, SE01-L, DW10 | Relative humidity percentage |
| 🟢 | U+1F7E2 | Green Circle | Status - Active/Occupied/Good | LHT65, AM300, WS52X | WS522 | PIR occupied, socket on, sensor OK |
| ⚫ | U+26AB | Black Circle | Status - Inactive/Vacant | AM300, WS52X | WS522 | PIR vacant, socket off |
| 🔴 | U+1F534 | Red Circle | Status - Active/Alert | LHT65 | Walker | Triggered/Alert state |
| ⚪ | U+26AA | White Circle | Status - Inactive/Off | - | Walker, SN50v3L | Tracking off, working mode |
| 💡 | U+1F4A1 | Light Bulb | Illuminance (lux) / Power (W) | LHT65, AM300 | WS522 | Light level or power |
| 🌬️ | U+1F32C | Wind Face | CO2 (ppm) / Air Quality | AM300 | - | Carbon dioxide concentration |
| 🏭 | U+1F3ED | Factory | TVOC (level/μg/m³) | AM300 | - | Total volatile organic compounds |
| 📊 | U+1F4CA | Bar Chart | Pressure (hPa) / Data Display | AM300 | WS522 | Barometric pressure |
| 🧪 | U+1F9EA | Test Tube | HCHO (mg/m³) / EC (µS/cm) / Chemical Analysis | AM300, SE01-LB | - | Formaldehyde / Soil conductivity |
| 🌫️ | U+1F32B | Fog | PM2.5 (μg/m³) | AM300 | - | Fine particulate matter |
| 💨 | U+1F4A8 | Dashing Away | PM10 (μg/m³) | AM300 | - | Coarse particulate matter |
| ⚗️ | U+2697 | Alembic | O3 (ppm) / Dielectric (κ) / Chemical Compounds | AM300, SE01-LB | - | Ozone / Dielectric constant |
| 🔊 | U+1F50A | Speaker High Volume | Buzzer Status / Audio Alert | AM300 | - | Audio alert status |
| 📦 | U+1F4E6 | Package | Tilt Detection | DW10 | - | Tilt/orientation sensor |
| ⭳ | U+2B73 | Heavy Large Circle | Distance/Depth (mm/cm) | - | DDS75L, PS-L-I5 | Distance measurement |
| 🎯 | U+1F3AF | Direct Hit | GPS Position/Location | - | Walker | GPS coordinates |
| ⛰️ | U+26F0 | Mountain | Altitude (m) | - | Walker | Altitude measurement |
| 📡 | U+1F4E1 | Satellite Antenna | Antenna/Signal | - | Walker | Antenna identifier |
| 📍 | U+1F4CD | Round Pushpin | Position/Location | - | Walker | Position marker |
| ⏱️ | U+23F1 | Stopwatch | Counter/Timer | DW10 | Walker | Event counter |
| 🔘 | U+1F518 | Radio Button | Button Press | DW10 | WS101 | Button state indicator |
| 🚫 | U+1F6AB | No Entry Sign | PIR Motion Detected | - | WS202 | Motion sensor active |
| 🆓 | U+1F193 | Free Button | PIR No Motion | - | WS202 | Motion sensor inactive |
| 🌕 | U+1F315 | Full Moon | Dark/No Light | - | WS202 | Light sensor - dark |
| 🌞 | U+1F31E | Sun with Face | Light Detected | - | WS202 | Light sensor - bright |
| ✅ | U+2705 | Check Mark Button | Installed/OK Status | WS301 | - | Installation confirmed |
| ❌ | U+274C | Cross Mark | Not Installed/Error | WS301 | - | Installation error |
| 🔓 | U+1F513 | Open Lock | Door Open/Unlocked | WS301, DW10 | LDS02, LHT65, DW10 | Door open state |
| 🔒 | U+1F512 | Locked | Door Closed/Locked | WS301, DW10 | LDS02, LHT65, DW10 | Door closed state |
| ⚡ | U+26A1 | High Voltage | Voltage (V) | LHT65 | WS522, WS52x | Electrical voltage |
| 🔌 | U+1F50C | Electric Plug | Current (mA) | - | WS522, WS52x | Electrical current |
| 🏠 | U+1F3E0 | House | Energy (Wh) | - | WS522, WS52x | Energy consumption |
| ⚠️ | U+26A0 | Warning Sign | Error/Warning/Tamper | LHT65, DW10 | - | Sensor error, disconnection, or tamper alert |
| 🔢 | U+1F522 | Input Numbers | Counter/Counting | LHT65 | - | Digital counter values |

## Implementation Guidelines

### Consistency Rules
1. **Same Unit = Same Emoji**: Always use the same emoji for the same measurement unit across all drivers
2. **Semantic Meaning**: Choose emojis that logically represent the measurement type
3. **Visual Clarity**: Prefer emojis that are clearly distinguishable and widely supported
4. **Minimal Text**: Use emojis instead of text labels whenever possible

### Reserved Emojis
The following emojis are reserved for common sensor types and should be used consistently:

#### Environmental
- 🌡️ Temperature (modern style - preferred for new drivers)
- ☀️ Temperature (legacy style)
- 💧 Humidity (%RH)
- 📊 Pressure (hPa/mbar/Pa)
- 🌬️ Wind speed/Air quality/CO2

#### Power & Energy
- ⚡ Voltage (V)
- 🔌 Current (mA/A)
- 💡 Power (W) / Light (lux)
- 🏠 Energy (Wh/kWh)
- 🔋 Battery level (%)

#### Motion & Status
- 🟢 Active/On/Occupied/Good
- 🔴 Alert/Active/Critical
- ⚫ Inactive/Off/Vacant
- ⚪ Inactive/Off alternative
- 📍 Position/Location/GPS
- 🎯 GPS coordinates

#### Air Quality
- 🏭 TVOC/Industrial emissions
- 🌫️ PM2.5/Fine particles
- 💨 PM10/Coarse particles
- ⚗️ Chemical compounds (O3, etc.)
- 🧪 Laboratory analysis/HCHO

#### Access Control
- 🔓 Door/Lock open
- 🔒 Door/Lock closed
- ✅ Installed/OK status
- ❌ Error/Not installed
- 🔘 Button/Manual control

#### Audio & Alerts
- 🔊 Active audio/Buzzer on
- 🔇 Muted/Buzzer off
- 📢 Announcement/Alert
- 🔔 Notification/Alarm

#### Sensors & Detection
- 🚫 Motion detected
- 🆓 No motion
- 🌕 Dark/No light
- 🌞 Light detected
- ⭳ Distance/Depth measurement

#### Location & Navigation
- 📡 Antenna/Signal
- ⛰️ Altitude/Mountain
- ⏱️ Counter/Timer

#### Error & Status
- ⚠️ Warning/Error/Disconnected
- 🔢 Counter/Digital values

### Usage Instructions
1. **Check this reference** before implementing new emoji usage
2. **Add new entries** when introducing emojis for new sensor types
3. **Update sensor lists** when adding support to existing drivers
4. **Maintain consistency** across different sensor models
5. **Document changes** in version history

### Adding New Emojis
When adding support for a new sensor type:
1. Choose an appropriate emoji following semantic guidelines
2. Add entry to this table with unicode, description, and initial sensor
3. Update other drivers using the same measurement type to use the new emoji
4. Commit reference update with driver implementation

### Migration Notes
- **Temperature**: Modern drivers should use 🌡️ instead of ☀️
- **Status Indicators**: Use 🟢/⚫ for binary states, 🔴/⚪ for special alerts
- **PIR Sensors**: Prefer specific motion emojis (🚫/🆓) over generic circles when applicable
- **Locks/Doors**: Always use 🔓/🔒 for open/closed states
- **Air Quality**: Use specific emojis for different pollutants (🏭 TVOC, 🌫️ PM2.5, 💨 PM10, ⚗️ O3, 🧪 HCHO)

## Current Driver Coverage

### AI-Generated Drivers (26 emojis)
- **Dragino**: LHT65 (8 emojis), SE01-LB (7 emojis)
- **MerryIoT**: DW10 (8 emojis)
- **Milesight**: AM300 (16 emojis), WS301 (4 emojis), WS52X (3 emojis)

### Legacy Drivers  
- **Dragino**: D20, DDS75L, LDS02, LHT52, LHT65, PS-L-I5, SE01-L, SN50v3L
- **Glamos**: Walker
- **MerryIoT**: DW10
- **Milesight**: WS101, WS202, WS522

## Version History
- v1.00: Initial version with AM300 sensor emojis
- v1.01: Complete scan of existing drivers (34 emojis across 4 vendors)
- v1.02: Updated with AI-generated drivers scan (35 total emojis, 21 from AI drivers)
- v1.03: Added SE01-LB driver with soil monitoring emojis (🧪 for EC, ⚗️ for dielectric constant)
- v1.04: Added AI-optimized DW10 driver with tilt detection emoji (📦 for tilt sensor)
- Added comprehensive guidelines and reserved emoji categories
- Documented legacy vs modern emoji preferences
- Added specific air quality and soil measurement emoji patterns

---
*This reference tool is automatically maintained as part of LwDecode driver development*

---

*Author: [ZioFabry](https://github.com/ZioFabry)*