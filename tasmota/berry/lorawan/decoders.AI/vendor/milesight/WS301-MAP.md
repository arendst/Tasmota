# Milesight WS301 Protocol MAP
# Generated: 2025-08-20 17:33:15 | PDF Version: V1.3
# Source: https://resource.milesight.com/milesight/iot/document/ws301-user-guide-en.pdf

device_info:
  vendor: Milesight
  model: WS301
  type: Magnetic Contact Switch
  lorawan_version: 1.0.2/1.0.3
  regions: [EU868, US915, AU915, CN470]
  official_link: https://resource.milesight.com/milesight/iot/document/ws301-user-guide-en.pdf
  frequency_bands: [868M, 915M]
  battery: 1200mAh replaceable
  range: 15km
  detection_range: 15mm (plane), 7.5mm (height)

uplinks:
  - port: 85
    type: "device_info"
    name: "Device Information"
    trigger: "device power on, join network"
    channels:
      - id: 0xFF
        type: 0x01
        name: "Protocol Version" 
        size: 1
        unit: ""
        format: "01=>V1"
      - id: 0xFF
        type: 0x08
        name: "Device Serial Number"
        size: 6
        unit: ""
        format: "hex string"
      - id: 0xFF
        type: 0x09
        name: "Hardware Version"
        size: 2
        unit: ""
        format: "0140=>V1.4"
      - id: 0xFF
        type: 0x0A
        name: "Software Version"
        size: 2
        unit: ""
        format: "0114=>V1.14"
      - id: 0xFF
        type: 0x0B
        name: "Power On Event"
        size: 1
        unit: ""
        format: "FF=>power on"
      - id: 0xFF
        type: 0x0F
        name: "Device Type"
        size: 1
        unit: ""
        format: "00=>Class A, 01=>Class B, 02=>Class C"
        
  - port: 85
    type: "sensor_data"
    name: "Sensor Data"
    trigger: "reporting interval, status change"
    channels:
      - id: 0x01
        type: 0x75
        name: "Battery Level"
        size: 1
        unit: "%"
        range: [0, 100]
        format: "UINT8"
      - id: 0x03
        type: 0x00
        name: "Magnet Status"
        size: 1
        unit: ""
        values:
          0x00: "Close"
          0x01: "Open"
      - id: 0x04
        type: 0x00
        name: "Tamper Status"
        size: 1
        unit: ""
        values:
          0x00: "Installed"
          0x01: "Un-installed"

downlinks:
  - command: "Set Reporting Interval"
    hex_prefix: "FF03"
    port: 85
    parameters:
      - name: "interval"
        type: "uint16"
        unit: "seconds"
        range: [60, 64800]
        endian: "little"
        format: "2 bytes"

special_handling:
  crc_required: false
  reset_events: true
  battery_monitoring: true
  tamper_detection: true
  magnetic_sensitivity: "15mm plane distance, 7.5mm height difference"
  led_indicators: true
  nfc_configuration: true
  d2d_support: true
  power_on_events: true
  
measurement_units:
  battery_level: "%"
  reporting_interval: "seconds"
  detection_distance: "mm"

device_features:
  magnetic_contact: true
  tamper_detection: true
  battery_powered: true
  nfc_config: true
  led_indicators: true
  milesight_d2d: true
  long_range: "15km"
  low_power: true
  class_a_only: true

installation_notes:
  mounting: "3M tape or screws"
  alignment: "notch to notch orientation required"
  max_distance: "15mm plane, 7.5mm height"
  battery_life: "depends on reporting interval"
  temperature_range: "operating range specified"

configuration_methods:
  - nfc_mobile: "Milesight ToolBox App"
  - nfc_reader: "Dedicated NFC reader"
  - ttl_interface: "Internal TTL interface"

test_payloads:
  device_info: "FF0BFF FF0101 FF086538B2232131 FF090140 FF0A0114 FF0F00"
  sensor_normal: "017564 030000 040000"
  sensor_open: "017564 030001 040000"
  sensor_tamper: "017564 030000 040001"
  low_battery: "01750A 030001 040000"

payload_examples:
  - name: "Power On + Device Info"
    hex: "FF0BFF FF0101 FF086538B2232131 FF090140 FF0A0114 FF0F00"
    description: "Device startup with version info"
  - name: "Door Closed, Installed, 100% Battery"
    hex: "017564 030000 040000"
    description: "Normal operation state"
  - name: "Door Open, Installed, 100% Battery"
    hex: "017564 030001 040000" 
    description: "Door opened event"
  - name: "Door Closed, Tamper Detected, 100% Battery"
    hex: "017564 030000 040001"
    description: "Tamper/uninstall detected"
  - name: "Door Open, Low Battery (10%)"
    hex: "01750A 030001 040000"
    description: "Low battery warning with door open"
