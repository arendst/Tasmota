# Milesight WS101 Protocol MAP
# Generated: 2025-08-15 | PDF Version: v1.0
# Source: WS101 LoRaWAN User Guide

device_info:
  vendor: Milesight
  model: WS101
  type: Smart Button
  lorawan_version: 1.0.3
  regions: [EU868, US915, AU915, AS923, KR920, IN865, RU864]
  official_link: https://www.milesight.com/iot/product/lorawan-sensor/ws101

uplinks:
  - port: 85
    type: Data
    name: "Button Press Events"
    channels:
      - id: 0x01
        type: 0x00
        name: "Button Press Type"
        size: 1
        values:
          0x01: "Single Press"
          0x02: "Double Press"
          0x03: "Long Press"
          
      - id: 0x01
        type: 0x75
        name: "Battery"
        size: 1
        unit: "%"
        range: [0, 100]
        
      - id: 0xFF
        type: 0x01
        name: "Protocol Version"
        size: 1
        
      - id: 0xFF
        type: 0x09
        name: "Hardware Version"
        size: 2
        
      - id: 0xFF
        type: 0x0A
        name: "Software Version"
        size: 2
        
      - id: 0xFF
        type: 0x0B
        name: "Power On Event"
        size: 1
        
      - id: 0xFF
        type: 0x0F
        name: "Device Class"
        size: 1
        values:
          0x00: "Class A"
          0x01: "Class B"
          0x02: "Class C"
          
      - id: 0xFF
        type: 0x16
        name: "Serial Number"
        size: 8
        
      - id: 0xFF
        type: 0xFE
        name: "Reset Event"
        size: 1
        values:
          0x00: "POR"
          0x01: "BOR"
          0x02: "WDT"
          0x03: "CMD"
          
      - id: 0xFF
        type: 0xFF
        name: "TSL Version"
        size: 2
        
      - id: 0xFE
        type: 0x02
        name: "Reporting Interval"
        size: 2
        unit: "seconds"
        
      # Acknowledgment channels
      - id: 0xFE
        type: 0x03
        name: "Interval ACK"
        size: 2
        
      - id: 0xFE
        type: 0x10
        name: "Reboot ACK"
        size: 1

downlinks:
  - command: "Set Interval"
    hex_prefix: "FE02"
    parameters:
      - name: "seconds"
        type: "uint16"
        range: [60, 65535]
        endian: "little"
        
  - command: "Button Mode"
    hex_prefix: "FF21"
    parameters:
      - name: "mode"
        type: "uint8"
        values:
          single: "01"
          double: "02"
          long: "03"
          
  - command: "Reboot"
    hex_prefix: "FF10"
    parameters:
      - name: "reboot"
        type: "fixed"
        value: "FF"

special_handling:
  crc_required: false
  reset_events: true
  button_events: true
  battery_monitoring: true
  
measurement_units:
  battery: "%"
  rssi: "dBm"
