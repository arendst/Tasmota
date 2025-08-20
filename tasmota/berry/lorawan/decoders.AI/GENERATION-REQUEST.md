# LoRaWAN Driver Generation Request Form
## Version: 2.3.3 | Framework: LwDecode | Platform: Tasmota Berry

---

## 📋 GENERATION REQUEST

### Request Type
- [ ] **NEW** - Generate from PDF specification (first time)
- [ ] **REGENERATE** - Fresh generation from existing MAP file
- [ ] **UPDATE** - Partial update/fix existing driver
- [ ] **FROM_URL** - Generate from online documentation URL
- [ ] **FROM_MAP** - Generate only from MAP cache file (skip PDF parsing)

### Generation Mode
- [ ] **FULL** - Complete driver with all features
- [ ] **MINIMAL** - Essential features only (uplinks + basic downlinks)
- [ ] **UPLINK_ONLY** - Decoder only, no downlink commands
- [ ] **CUSTOM** - Specific feature selection (fill Custom Features section)

---

## 🏭 DEVICE INFORMATION

### Basic Details
```yaml
vendor: ""                    # e.g., "Milesight", "Dragino", "Elsys"
model: ""                     # e.g., "WS52x", "LHT65", "ERS-CO2"
type: ""                      # e.g., "Environmental Sensor", "Smart Socket", "Water Meter"
description: ""               # Brief device description for documentation
```

### Official References
```yaml
homepage: ""                  # Official product page URL
userguide: ""                 # User manual/datasheet URL
decoder_reference: ""         # Official decoder/example code URL
firmware_version: ""          # Target firmware version (if specific)
lorawan_version: ""           # e.g., "1.0.3", "1.1.0"
regions: []                   # e.g., ["EU868", "US915", "AS923"]
```

---

## 📄 SOURCE DOCUMENTATION

### PDF Specification
```yaml
pdf_filename: ""              # Local PDF filename in project
pdf_version: ""               # Document version/revision
pdf_date: ""                  # Document date (YYYY-MM-DD)
pages_relevant: ""            # e.g., "15-32" or "all" for parsing focus
```

### Online Documentation
```yaml
documentation_url: ""         # Alternative to PDF
api_reference_url: ""         # API/protocol documentation
example_payloads_url: ""      # Sample payload documentation
```

### Existing Resources
```yaml
map_file_exists: false        # true if [MODEL]-MAP.md already exists
use_cached_map: false         # true to skip PDF parsing, use MAP only
previous_driver_version: ""   # Previous version for changelog reference
```

---

## ⚙️ GENERATION PREFERENCES

### Feature Selection
```yaml
include_features:
  uplink_decoding: true       # Decode all uplink payload types
  downlink_commands: true     # Generate Tasmota downlink commands
  test_ui_scenarios: true     # Generate TestUI command with scenarios
  node_management: true       # Node statistics and management
  battery_tracking: true      # Battery voltage history
  reset_detection: true       # Device reset event tracking
  configuration_sync: true    # Track device configuration state
  error_recovery: true        # Robust error handling
  performance_optimization: true  # Memory and speed optimizations

exclude_features:
  unused_channels: []         # List of channel IDs to skip (if any)
  unsupported_downlinks: []   # List of downlink commands to skip
  legacy_modes: false         # Skip old firmware compatibility
  debug_logging: false        # Exclude verbose debug output
```

### UI Customization
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

## 🎯 CUSTOM REQUIREMENTS

### Special Handling
```yaml
special_requirements:
  custom_crc_validation: false    # Implement custom CRC checking
  signed_value_handling: []       # List of signed parameters
  endianness_override: {}         # Override default endianness: {"param": "big"}
  unit_conversions: {}            # Custom unit conversions
  threshold_monitoring: []        # Parameters to monitor for thresholds
  custom_validation_rules: {}     # Parameter-specific validation
```

### Performance Requirements
```yaml
performance_targets:
  max_decode_time_ms: 50         # Maximum acceptable decode time
  memory_optimization: "normal"   # "minimal", "normal", "generous"
  stack_usage_limit: 200         # Berry stack levels limit
  payload_size_limit: 255        # Maximum expected payload size
```

### Integration Requirements
```yaml
integration_needs:
  mqtt_topic_override: ""        # Custom MQTT topic pattern
  home_assistant_discovery: true # HA auto-discovery support
  influxdb_formatting: false    # InfluxDB-compatible field names
  prometheus_metrics: false     # Prometheus metric exposition
  custom_web_ui_elements: []     # Additional UI components needed
```

---

## 🧪 TESTING CONFIGURATION

### Test Scenarios
```yaml
test_configuration:
  realistic_test_data: true      # Generate realistic test payloads
  stress_test_payloads: false    # Include edge-case test data
  regression_test_suite: true    # Full test suite for validation
  performance_benchmarks: false # Include performance test scenarios
  
custom_test_scenarios:          # Define custom test cases
  - name: "normal_operation"
    description: "Typical sensor readings"
    expected_params: ["temperature", "humidity", "battery"]
    
  - name: "alert_condition"  
    description: "Threshold exceeded scenario"
    expected_params: ["temperature", "alert_flag"]
    
  - name: "low_battery"
    description: "Battery below 20%"
    expected_params: ["battery_v", "low_battery_flag"]
```

### Validation Requirements
```yaml
validation_requirements:
  syntax_validation: true        # Berry syntax checking
  framework_compliance: true     # LwDecode framework compliance
  memory_leak_detection: true    # Memory usage validation
  error_path_testing: true       # Test error handling paths
  command_parameter_validation: true # Test all command parameters
  downlink_hex_accuracy: true    # Verify downlink payload accuracy
```

---

## 📝 DOCUMENTATION REQUIREMENTS

### Documentation Level
```yaml
documentation_preferences:
  comprehensive_docs: true       # Full documentation generation
  api_reference: true           # Command reference documentation
  usage_examples: true          # Practical usage examples
  troubleshooting_guide: false  # Common issues and solutions
  integration_guide: true       # Step-by-step integration
  performance_metrics: true     # Include performance data
```

### Output Format
```yaml
output_configuration:
  generate_map_file: true        # Create/update MAP cache file
  generate_md_docs: true         # Generate [MODEL].md documentation
  update_emoji_reference: true   # Update emoji-reference.md if needed
  update_driver_list: true       # Update GENERATED-DRIVER-LIST.md
  create_generation_report: true # Create detailed generation report
```

---

## 🚀 EXECUTION PREFERENCES

### Generation Workflow
```yaml
workflow_preferences:
  silent_generation: true        # Minimize verbose output during generation
  validation_before_output: true # Run full validation before presenting results
  optimization_passes: 2         # Number of optimization iterations
  backup_existing: true          # Backup existing driver if updating
```

### Quality Assurance
```yaml
quality_requirements:
  uplink_coverage: 100          # Percentage of uplinks that must be implemented
  downlink_coverage: 100        # Percentage of downlinks that must be implemented
  test_payload_coverage: 100    # Percentage of test scenarios required
  error_handling_coverage: true # Comprehensive error handling required
  code_review_level: "production" # "development", "staging", "production"
```

---

## 💬 ADDITIONAL NOTES

### Custom Instructions
```
[Add any specific requirements, special considerations, or custom logic needed]

Examples:
- "Device uses non-standard battery calculation formula"
- "Special handling required for channel ID 0xFF"
- "Downlink ACK expected on different port"
- "Custom timestamp format in configuration responses"
- "Multi-part payloads require reassembly"
```

### Known Issues
```
[Document any known limitations or issues with the device/protocol]

Examples:
- "Firmware v1.2.x has incorrect humidity scaling"
- "Port 3 payloads are undocumented but observed"
- "Device resets don't always send reset notification"
- "Some downlinks require specific timing"
```

---

## 📞 GENERATION REQUEST SUMMARY

**Request ID**: `[AUTO-GENERATED]`  
**Submitted**: `[AUTO-TIMESTAMP]`  
**Priority**: `[normal|high|urgent]`  
**Estimated Complexity**: `[simple|moderate|complex]`  
**Expected Deliverables**:
- [ ] Driver file: `vendor/[vendor]/[MODEL].be`
- [ ] Documentation: `vendor/[vendor]/[MODEL].md`
- [ ] MAP cache: `vendor/[vendor]/[MODEL]-MAP.md`
- [ ] Test scenarios: Integrated in driver
- [ ] Generation report: `vendor/[vendor]/[MODEL]-REPORT.md`

---

## ✅ PRE-SUBMISSION CHECKLIST

- [ ] Device information complete
- [ ] Source documentation available/accessible
- [ ] Feature requirements clearly specified
- [ ] Test scenarios defined
- [ ] Special requirements documented
- [ ] Output preferences configured
- [ ] Additional notes added (if applicable)

---

**🤖 AI Generation Ready**: Once this form is complete, submit to the LoRaWAN Decoder AI Generation Template for automated driver creation.

---

*Form Version: 2.3.3 | Compatible with Template Version: 2.3.3*  
*Generated: [TIMESTAMP] | Framework: LwDecode*