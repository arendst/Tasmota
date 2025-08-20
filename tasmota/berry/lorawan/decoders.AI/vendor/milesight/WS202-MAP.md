# Milesight WS202 Protocol MAP
# Generated: 2025-08-20 17:37:22 | PDF Version: V1.3
# Source: https://resource.milesight.com/milesight/iot/document/ws202-user-guide-en.pdf

device_info:
  vendor: Milesight
  model: WS202
  type: PIR & Light Sensor
  lorawan_version: 1.0.2/1.0.3
  regions: [EU868, US915, AU915, CN470]
  official_link: https://resource.milesight.com/milesight/iot/document/ws202-user-guide-en.pdf
  frequency_bands: [868M, 915M]
  battery: CR123A lithium battery
  detection_range: 6-8m PIR detection
  installation_height: 1.5-2.5m recommended
  detection_angle: 120° horizontal, 100° vertical (normal) / 70° horizontal, 60° vertical (with blocking stickers)

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
    trigger: "reporting interval (30min default), PIR/light status change, low battery"
    channels:
      - id: 0x01
        type: 0x75
        name: "Battery Level"
        size: 1
        unit: "%"
        range: [0, 100]
        format: "UINT8"
        special_notes: "Reports once when <10%"
      - id: 0x03
        type: 0x00
        name: "PIR Status"
        size: 1
        unit: ""
        values:
          0x00: "Vacant"
          0x01: "Occupied"
        notes: "Immediate report on motion detection"
      - id: 0x04
        type: 0x00
        name: "Light Status"
        size: 1
        unit: ""
        values:
          0x00: "Dark"
          0x01: "Bright"
        notes: "Based on configurable threshold"

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
        default: 1800
        notes: "30 minutes default, 1-1080 minutes range"
  - command: "Reboot Device"
    hex_prefix: "FF10"
    port: 85
    parameters:
      - name: "reserved"
        type: "fixed"
        value: "FF"
        format: "1 byte"
        notes: "Always FF for reboot command"

special_handling:
  crc_required: false
  reset_events: true
  battery_monitoring: true
  pir_detection: true
  light_sensor: true
  motion_triggering: "immediate occupied status report"
  idle_timeout: "120s default for vacant status"
  low_battery_threshold: "10%"
  led_indicators: true
  nfc_configuration: true
  d2d_support: true
  power_on_events: true
  illuminance_collection: "configurable enable/disable"
  threshold_settings: "bright/dark configurable"
  blocking_stickers: "for detection angle control"
  
measurement_units:
  battery_level: "%"
  reporting_interval: "seconds"
  idle_time: "seconds"
  detection_range: "meters"
  detection_angle: "degrees"

device_features:
  pir_sensor: true
  light_sensor: true
  battery_powered: true
  nfc_config: true
  led_indicators: true
  milesight_d2d: true
  long_range: "15km"
  low_power: true
  class_a_only: true
  motion_detection: "6-8m range"
  illuminance_threshold: "configurable bright/dark"

installation_notes:
  mounting: "3M tape or screws, wall or ceiling"
  height: "1.5-2.5m from floor"
  detection_area: "ensure no moving objects (trees, fans)"
  blocking: "no curtains or barriers in detection area"
  angle_control: "blocking stickers for 180° coverage per sticker"
  modified_detection: "70° H x 60° V with full blocking"

configuration_methods:
  - nfc_mobile: "Milesight ToolBox App"
  - nfc_reader: "Dedicated NFC reader"
  - ttl_interface: "Internal TTL interface"

test_payloads:
  device_info: "FF0BFF FF0101 FF086538B2232131 FF090100 FF0A0101 FF0F00"
  sensor_normal: "017564 030000 040000"
  sensor_occupied: "017564 030001 040001"
  sensor_vacant: "017564 030000 040001"
  low_battery: "01750A 030001 040000"

payload_examples:
  - name: "Power On + Device Info"
    hex: "FF0BFF FF0101 FF086538B2232131 FF090100 FF0A0101 FF0F00"
    description: "Device startup with version info"
  - name: "Vacant, Dark, 100% Battery"
    hex: "017564 030000 040000"
    description: "Normal vacant state in dark environment"
  - name: "Occupied, Bright, 100% Battery"
    hex: "017564 030001 040001" 
    description: "Motion detected in bright environment"
  - name: "Vacant, Bright, 100% Battery"
    hex: "017564 030000 040001"
    description: "No motion detected in bright environment"
  - name: "Occupied, Dark, Low Battery (10%)"
    hex: "01750A 030001 040000"
    description: "Motion detected with low battery warning"
