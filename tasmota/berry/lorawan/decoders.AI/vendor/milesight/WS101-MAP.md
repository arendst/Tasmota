# Milesight WS101 Protocol MAP
# Generated: 2025-08-20 17:45:00 | PDF Version: v1.2
# Source: WS101 LoRaWAN User Guide

device_info:
  vendor: Milesight
  model: WS101
  type: Smart Button
  lorawan_version: 1.0.3
  regions: [EU868, US915, AU915, AS923, KR920, IN865, RU864, CN470]
  official_link: https://www.milesight.com/iot/product/lorawan-sensor/ws101

uplinks:
  - port: 85
    type: Data
    name: "Device Information & Button Events"
    channels:
      # Button events
      - id: 0xFF
        type: 0x2E
        name: "Button Message"
        size: 1
        values:
          0x01: "Mode 1 (Short Press)"
          0x02: "Mode 2 (Long Press)"
          0x03: "Mode 3 (Double Press)"
          
      # Battery monitoring
      - id: 0x01
        type: 0x75
        name: "Battery Level"
        size: 1
        unit: "%"
        range: [0, 100]
        
      # Device information
      - id: 0xFF
        type: 0x01
        name: "Protocol Version"
        size: 1
        values:
          0x01: "V1"
          
      - id: 0xFF
        type: 0x08
        name: "Device Serial Number"
        size: 6
        
      - id: 0xFF
        type: 0x09
        name: "Hardware Version"
        size: 2
        format: "major.minor"
        
      - id: 0xFF
        type: 0x0A
        name: "Software Version"
        size: 2
        format: "major.minor"
        
      - id: 0xFF
        type: 0x0B
        name: "Power On Event"
        size: 0
        description: "Device startup indicator"
        
      - id: 0xFF
        type: 0x0F
        name: "Device Class"
        size: 1
        values:
          0x00: "Class A"
          0x01: "Class B"
          0x02: "Class C"

downlinks:
  - command: "Set Reporting Interval"
    hex_prefix: "FF03"
    parameters:
      - name: "seconds"
        type: "uint16"
        range: [60, 64800]
        endian: "little"
        unit: "seconds"
        
  - command: "Reboot Device"
    hex_prefix: "FF10"
    parameters:
      - name: "reserved"
        type: "fixed"
        value: "FF"
        
  - command: "Set LED Indicator"
    hex_prefix: "FF2F"
    parameters:
      - name: "enable"
        type: "uint8"
        values:
          disable: "00"
          enable: "01"
          
  - command: "Set Double Press Mode"
    hex_prefix: "FF74"
    parameters:
      - name: "enable"
        type: "uint8"
        values:
          disable: "00"
          enable: "01"
          
  - command: "Set Buzzer"
    hex_prefix: "FF3E"
    parameters:
      - name: "enable"
        type: "uint8"
        values:
          disable: "00"
          enable: "01"

special_handling:
  crc_required: false
  reset_events: false
  button_events: true
  battery_monitoring: true
  low_battery_threshold: 10
  default_reporting_interval: 1080
  
measurement_units:
  battery: "%"
  rssi: "dBm"
  interval: "seconds"
