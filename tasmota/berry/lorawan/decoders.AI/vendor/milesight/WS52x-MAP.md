# Milesight WS52x Protocol MAP
# Generated: 2025-08-19 15:30:42 | PDF Version: v1.1
# Source: WS52x_LoRaWAN_Application_Guide.pdf
# Last Updated: 2025-08-19 (Framework v2.2.4 regeneration)

device_info:
  vendor: Milesight
  model: WS52x
  type: Smart Power Socket
  lorawan_version: 1.0.3
  regions: [EU868, US915, AU915, AS923, KR920, IN865, RU864]
  official_link: https://www.milesight.com/iot/product/lorawan-sensor/ws52x

uplinks:
  - port: 85
    type: Data
    name: "Periodic/Event Data"
    channels:
      - id: 0x03
        type: 0x74
        name: "Voltage"
        size: 2
        unit: "V"
        resolution: 0.1
        range: [0, 3276.7]
        
      - id: 0x04
        type: 0x80
        name: "Active Power"
        size: 4
        unit: "W"
        resolution: 1
        range: [-2147483648, 2147483647]
        signed: true
        
      - id: 0x05
        type: 0x81
        name: "Power Factor"
        size: 1
        unit: "%"
        resolution: 1
        range: [0, 100]
        
      - id: 0x06
        type: 0x83
        name: "Energy"
        size: 4
        unit: "Wh"
        resolution: 1
        range: [0, 4294967295]
        
      - id: 0x07
        type: 0xC9
        name: "Current"
        size: 2
        unit: "mA"
        resolution: 1
        range: [0, 65535]
        
      - id: 0x08
        type: 0x70
        name: "Socket State"
        size: 1
        values:
          0x00: "OFF"
          0x01: "ON"
          
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
        type: 0x24
        name: "OC Alarm Config"
        size: 2
        
      - id: 0xFF
        type: 0x25
        name: "Button Lock Config"
        size: 2
        
      - id: 0xFF
        type: 0x26
        name: "Power Recording Config"
        size: 1
        
      - id: 0xFF
        type: 0x2F
        name: "LED Config"
        size: 1
        
      - id: 0xFF
        type: 0x30
        name: "OC Protection Config"
        size: 2
        
      - id: 0xFF
        type: 0x3F
        name: "Power Outage Event"
        size: 1
        
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
        unit: "minutes"
        
      # Acknowledgment channels
      - id: 0xFE
        type: 0x03
        name: "Interval ACK"
        size: 2
        
      - id: 0xFE
        type: 0x10
        name: "Reboot ACK"
        size: 1
        
      - id: 0xFE
        type: 0x22
        name: "Delay Task ACK"
        size: 4
        
      - id: 0xFE
        type: 0x23
        name: "Delete Task ACK"
        size: 2

downlinks:
  - command: "Socket Control"
    hex_prefix: "08"
    parameters:
      - name: "state"
        type: "boolean"
        values:
          on: "FF"
          off: "00"
          
  - command: "Set Interval"
    hex_prefix: "FE02"
    parameters:
      - name: "minutes"
        type: "uint16"
        range: [1, 65535]
        endian: "little"
        
  - command: "OC Alarm"
    hex_prefix: "FF24"
    parameters:
      - name: "enabled"
        type: "boolean"
        values:
          on: "01"
          off: "00"
      - name: "threshold"
        type: "uint8"
        range: [1, 30]
        unit: "A"
        
  - command: "OC Protection"
    hex_prefix: "FF30"
    parameters:
      - name: "enabled"
        type: "boolean"
        values:
          on: "01"
          off: "00"
      - name: "threshold"
        type: "uint8"
        range: [1, 30]
        unit: "A"
        
  - command: "Button Lock"
    hex_prefix: "FF25"
    parameters:
      - name: "locked"
        type: "boolean"
        values:
          on: "0080"
          off: "0000"
          
  - command: "LED Mode"
    hex_prefix: "FF2F"
    parameters:
      - name: "enabled"
        type: "boolean"
        values:
          on: "01"
          off: "00"
          
  - command: "Power Recording"
    hex_prefix: "FF26"
    parameters:
      - name: "enabled"
        type: "boolean"
        values:
          on: "01"
          off: "00"
          
  - command: "Reset Energy"
    hex_prefix: "FF27"
    parameters:
      - name: "reset"
        type: "fixed"
        value: "00"
        
  - command: "Status Enquiry"
    hex_prefix: "FF28"
    parameters:
      - name: "request"
        type: "fixed"
        value: "00"
        
  - command: "Reboot"
    hex_prefix: "FF10"
    parameters:
      - name: "reboot"
        type: "fixed"
        value: "FF"
        
  - command: "Delay Task"
    hex_prefix: "FE22"
    parameters:
      - name: "seconds"
        type: "uint32"
        range: [0, 4294967295]
        endian: "little"
        
  - command: "Delete Task"
    hex_prefix: "FE23"
    parameters:
      - name: "task_number"
        type: "uint16"
        range: [0, 65535]
        endian: "little"

special_handling:
  crc_required: false
  reset_events: true
  power_monitoring: true
  signed_values:
    - "active_power"
  
measurement_units:
  voltage: "V"
  current: "mA"
  active_power: "W"
  energy: "Wh"
  power_factor: "%"
  rssi: "dBm"

# Framework Integration Notes
framework_version: "2.2.4"
driver_version: "1.3.0"
last_regenerated: "2025-08-19"
uplink_coverage: "25/25 channels"
downlink_coverage: "12/12 commands"
