# How to Use the LoRaWAN Driver Generation Framework

This guide provides example prompts and instructions for generating LoRaWAN drivers using the AI-powered LwDecode framework.

## 🎯 Example Request: Milesight WS301 Door/Window Sensor

```markdown
Generate a LoRaWAN driver for the Milesight WS301 Magnetic Door/Window Sensor using the LwDecode framework.

## Device Information
- **Vendor**: Milesight
- **Model**: WS301
- **Type**: Magnetic Door/Window Sensor
- **Description**: Battery-powered magnetic contact sensor for door/window monitoring with tamper detection

## Generation Request
- **Type**: NEW (Generate from PDF specification)
- **Mode**: FULL (Complete driver with all features)
- **Framework**: Use template v2.3.3 with LwDecode v2.2.8

## Source Documentation
I have the official Milesight WS301 User Guide PDF that contains:
- Complete LoRaWAN protocol specification
- All uplink payload formats (periodic data, door events, device info)
- All downlink commands (reporting interval, device reboot)
- Channel mappings and data structures
- Battery monitoring and tamper detection

[Upload: WS301_UserGuide_v1.3.pdf]

## Expected Features
✅ **Uplinks**: Door state (open/closed), tamper detection, battery monitoring, device info
✅ **Downlinks**: Set reporting interval, device reboot commands  
✅ **UI Display**: Door status with 🔓/🔒 emojis, battery level, last seen
✅ **Events**: Track door open/close events, installation status, tamper alerts
✅ **Storage**: Persistent node data with door state history

## Test Scenarios Needed
- Normal operation (door open/closed cycles)
- Low battery condition
- Tamper detection events  
- Device configuration responses
- Installation verification

Generate complete driver with documentation, test commands, and integration examples.
```

---

## 🎯 Alternative Example: Using Existing MAP File

```markdown
Regenerate the Dragino LHT65 driver using the existing MAP cache file with latest framework improvements.

## Regeneration Request
- **Type**: REGENERATE (Fresh generation from existing MAP file)
- **Device**: Dragino LHT65 Temperature & Humidity Sensor
- **Source**: Use existing LHT65-MAP.md cache file
- **Framework**: Update to template v2.3.3

## Improvements Needed
- Update to latest error handling patterns
- Enhance emoji usage with framework v2.2.8
- Add enhanced test scenarios
- Improve memory optimization for ESP32
- Update documentation with comprehensive examples

## Expected Deliverables
- Updated LHT65.be driver file
- Refreshed LHT65.md documentation
- Updated test payload examples
- Enhanced emoji integration
- Production-ready status validation
```

---

## 🎯 Simple Example: Basic Request

```markdown
Generate a driver for the Elsys ERS-CO2 environmental sensor.

Device: Elsys ERS-CO2 Environmental Sensor
PDF: [Upload ERS-CO2_datasheet.pdf]
Mode: FULL driver with all features
Framework: Latest template v2.3.3

Expected features: Temperature, humidity, CO2, PIR motion, light sensor
Need complete uplink/downlink coverage with emoji-based UI display.
```

---

## 🎯 Quick Request Format

```markdown
**Device**: [Vendor] [Model]
**PDF**: [Upload specification]
**Mode**: [NEW/REGENERATE/UPDATE]
**Features**: [FULL/MINIMAL/CUSTOM]

Generate complete LoRaWAN driver with framework v2.3.3.
```

---

## 📋 Key Elements for Any Request

### Minimum Required:
1. **Device identification** (vendor + model)
2. **Source documentation** (PDF upload or existing MAP file)
3. **Generation type** (NEW, REGENERATE, UPDATE, etc.)

### Recommended Additional:
- **Feature preferences** (UI display, test scenarios)
- **Special requirements** (custom handling, constraints)
- **Framework version** (use latest v2.3.3)
- **Expected deliverables** (documentation level)

### Framework Will Automatically:
- ✅ Extract all uplink/downlink specifications
- ✅ Generate 100% coverage implementation
- ✅ Create emoji-based UI displays
- ✅ Add comprehensive test scenarios
- ✅ Generate complete documentation
- ✅ Validate Berry syntax compliance
- ✅ Optimize for ESP32 memory constraints

---

## 🚀 Getting Started

### Step 1: Load the Framework
```markdown
Please load and use the LoRaWAN driver development framework from:
C:\Project\tasmota\Tasmota\tasmota\berry\lorawan\decoders.AI\DEVELOPER-PROMPT.md

Confirm you've loaded the framework and are ready to generate drivers.
```

### Step 2: Provide Device Information
Choose one of the example formats above and customize for your device:
- Specify vendor and model
- Upload PDF specification or mention existing MAP file
- Choose generation type (NEW, REGENERATE, UPDATE)
- Set feature preferences

### Step 3: Review Generated Output
The framework will automatically create:
1. **Driver Code** at `vendor/[manufacturer]/[MODEL].be`
2. **Documentation** at `vendor/[manufacturer]/[MODEL].md`
3. **MAP Cache** at `vendor/[manufacturer]/[MODEL]-MAP.md`
4. **Generation Report** at `vendor/[manufacturer]/[MODEL]-REPORT.md`
5. **Updated References** if new emojis or patterns are used

---

## 📝 Generation Types

### NEW - First Time Generation
- Generate from manufacturer PDF specification
- Extract all protocol details
- Create complete driver implementation
- Generate MAP cache file for future use

### REGENERATE - Fresh from MAP Cache
- Use existing [MODEL]-MAP.md cache file
- Skip PDF parsing, use cached specifications
- Apply latest framework improvements
- Update to newest template version

### UPDATE - Partial Changes
- Fix specific issues or add features
- Maintain existing functionality
- Target specific improvements only
- Preserve version history

### FROM_URL - Online Documentation
- Generate from online specification URLs
- Parse web-based protocol documentation
- Extract from manufacturer websites
- Create complete implementation

---

## 🎨 UI Customization Options

### Display Preferences
```yaml
display_preferences:
  single_line_preferred: true    # Prefer single-line sensor display
  multi_line_for_alerts: true    # Use multiple lines for alerts/events
  custom_emojis: {}             # Custom emoji mappings: {"param": "🎯"}
  hide_technical_info: false    # Hide firmware versions, serial numbers
  emphasize_alerts: true        # Highlight alert conditions
  battery_prominance: "normal"  # "hidden", "normal", "prominent"
  rssi_display: "icon"          # "hidden", "icon", "numeric", "both"
```

### Command Preferences
```yaml
command_configuration:
  command_prefix: "Lw"          # Standard prefix for all commands
  include_aliases: false        # Generate short command aliases
  parameter_validation: "strict" # "strict", "lenient", "none"
  error_responses: "detailed"   # "detailed", "simple", "silent"
  backwards_compatibility: false # Support old command formats
```

---

## 🧪 Testing Features

### Automatic Test Generation
The framework generates comprehensive test scenarios:
- **Normal Operation**: Typical sensor readings
- **Alert Conditions**: Threshold exceeded scenarios
- **Low Battery**: Battery warning states
- **Device Events**: Reset, configuration, errors
- **Edge Cases**: Boundary values and error conditions

### Test Commands
Every generated driver includes:
```berry
# Test UI with different scenarios
Lw[MODEL]TestUI<slot> normal     # Normal operation
Lw[MODEL]TestUI<slot> alert      # Alert condition
Lw[MODEL]TestUI<slot> low        # Low battery
Lw[MODEL]TestUI<slot> config     # Configuration mode

# Direct simulation
LwSimulate<slot> -75,85,FF01AABB  # rssi,fport,hex_payload
```

---

## 🔧 Advanced Features

### Custom Requirements
```yaml
special_requirements:
  custom_crc_validation: false    # Implement custom CRC checking
  signed_value_handling: []       # List of signed parameters
  endianness_override: {}         # Override default endianness
  unit_conversions: {}            # Custom unit conversions
  threshold_monitoring: []        # Parameters to monitor for thresholds
```

### Performance Targets
```yaml
performance_targets:
  max_decode_time_ms: 50         # Maximum acceptable decode time
  memory_optimization: "normal"   # "minimal", "normal", "generous"
  stack_usage_limit: 200         # Berry stack levels limit
  payload_size_limit: 255        # Maximum expected payload size
```

---

## 📊 Framework Statistics

### Current Status
- **Total Drivers**: 12 (Dragino: 6, Milesight: 6)
- **Total Channels**: 182 with 100% coverage
- **Framework Version**: 2.2.8 (Enhanced error handling)
- **Template Version**: 2.3.3 (Latest with critical constraints)
- **Production Ready**: All drivers tested and validated

### Quality Assurance
- ✅ Berry syntax compliance (v1.2.0 constraints)
- ✅ ESP32 memory optimization
- ✅ Framework integration validation
- ✅ Emoji standardization (35+ mappings)
- ✅ Error handling and recovery
- ✅ Multi-node support with persistence

---

## 🎓 Best Practices

### When Requesting Generation
1. **Be Specific**: Provide exact vendor and model names
2. **Include Documentation**: Upload complete PDF specifications
3. **State Requirements**: Mention any special needs or constraints
4. **Choose Appropriate Mode**: NEW for first time, REGENERATE for updates
5. **Review Framework Version**: Use latest template v2.3.3

### For Custom Devices
1. **Check Existing Drivers**: Review similar devices for patterns
2. **Specify Unique Features**: Highlight device-specific capabilities
3. **Provide Examples**: Include sample payloads if available
4. **Document Constraints**: Mention any protocol limitations
5. **Request Test Scenarios**: Specify realistic usage patterns

---

## 🔗 Framework Resources

### Documentation Files
- **README.md** - Framework overview and quick start
- **DEVELOPER-PROMPT.md** - Complete AI prompt (v2.3.3)
- **FRAMEWORK.md** - Implementation details and API reference
- **BERRY-CUSTOM-LANGUAGE-REFERENCE.md** - Berry syntax constraints
- **emoji-reference.md** - Standardized emoji mappings
- **GENERATED-DRIVER-LIST.md** - Current driver registry

### Example Drivers
- **vendor/dragino/** - 6 environmental and sensor drivers
- **vendor/milesight/** - 6 smart sensor and socket drivers
- **All drivers** - Complete uplink/downlink coverage examples

---

**Simply provide the device information and PDF specification using one of the example formats above, and the framework will handle the complete driver generation process!**

---

*HOW-TO-USE Guide Version: 1.0.0 | Framework Version: 2.3.3*  
*Created: 2025-08-20 | Compatible with LwDecode v2.2.8*

---

*Author: [ZioFabry](https://github.com/ZioFabry)*