# Dragino D2x Series Protocol MAP
# Generated: 2025-08-16 | PDF Version: Manual v1.0
# Source: D20/D20S/D22/D23-LB/LS LoRaWAN Temperature Sensor User Manual

device_info:
  vendor: Dragino
  model: D2x
  type: Temperature Sensor Series
  models: [D20-LB/LS, D20S-LB/LS, D22-LB/LS, D23-LB/LS]
  lorawan_version: 1.0.3
  regions: [CN470, EU433, KR920, US915, EU868, AS923, AU915, IN865]
  official_link: http://wiki.dragino.com/xwiki/bin/view/Main/User Manual for LoRaWAN End Nodes/D20-LBD22-LBD23-LB_LoRaWAN_Temperature_Sensor_User_Manual/

uplinks:
  - port: 5
    type: Device Status
    name: "Device Configuration and Status"
    structure:
      - name: "Sensor Model"
        size: 1
        description: "0x19 for D2x series"
      - name: "Firmware Version"
        size: 2
        description: "Version format: 0x0100 = v1.0.0"
      - name: "Frequency Band"
        size: 1
        values:
          0x01: "EU868"
          0x02: "US915"
          0x03: "IN865"
          0x04: "AU915"
          0x05: "KZ865"
          0x06: "RU864"
          0x07: "AS923"
          0x08: "AS923-1"
          0x09: "AS923-2"
          0x0a: "AS923-3"
          0x0b: "CN470"
          0x0c: "EU433"
          0x0d: "KR920"
          0x0e: "MA869"
      - name: "Sub-band"
        size: 1
        description: "AU915/US915: 0x00-0x08, CN470: 0x0B-0x0C, Others: 0x00"
      - name: "Battery"
        size: 2
        unit: "mV"
        description: "Battery voltage in millivolts"

  - port: 2
    type: Sensor Data
    name: "Temperature Sensor Readings"
    structure:
      - name: "Battery"
        size: 2
        unit: "mV"
        description: "Battery voltage"
      - name: "Temp_Red_or_White"
        size: 2
        unit: "°C"
        resolution: 0.1
        description: "Red probe (D20) or White probe (D22/D23)"
        signed: true
      - name: "Ignore"
        size: 2
        description: "Reserved field"
      - name: "Alarm_Flag_MOD_PA8"
        size: 1
        bits:
          - bit: 0
            name: "Alarm Flag"
            description: "1=Alarm message, 0=Normal"
          - bit: 7
            name: "PA8 Level"
            description: "1=Low, 0=High"
          - bits: "2-6"
            name: "MOD"
            description: "Message type indicator"
      - name: "Temp_White"
        size: 2
        unit: "°C"
        resolution: 0.1
        description: "White probe (D22/D23), 0x7FFF if not present"
        signed: true
      - name: "Temp_Black"
        size: 2
        unit: "°C"
        resolution: 0.1
        description: "Black probe (D23), 0x7FFF if not present"
        signed: true

  - port: 3
    type: Datalog
    name: "Historical Data Retrieval"
    structure:
      - name: "Temp_Black"
        size: 2
        unit: "°C"
        resolution: 0.1
        signed: true
      - name: "Temp_White"
        size: 2
        unit: "°C"
        resolution: 0.1
        signed: true
      - name: "Temp_Red_or_White"
        size: 2
        unit: "°C"
        resolution: 0.1
        signed: true
      - name: "Poll_Alarm_PA8_Flags"
        size: 1
        bits:
          - bit: 0
            name: "No ACK Flag"
            description: "1=Message without ACK"
          - bit: 1
            name: "Poll Flag"
            description: "1=Poll reply message"
          - bit: 7
            name: "PA8 Level"
            description: "1=Low, 0=High"
      - name: "Unix Timestamp"
        size: 4
        description: "Unix timestamp of measurement"

downlinks:
  - command: "Set Transmit Interval"
    hex_prefix: "01"
    parameters:
      - name: "interval"
        type: "uint24"
        range: [30, 16777215]
        unit: "seconds"
        description: "Transmission interval in seconds"

  - command: "Get Device Status"
    hex_prefix: "2601"
    parameters: []
    response_port: 5

  - command: "Set Alarm Threshold (All Probes)"
    hex_prefix: "0B"
    parameters:
      - name: "min_temp"
        type: "int8"
        range: [-128, 127]
        unit: "°C"
        description: "Minimum temperature threshold"
      - name: "max_temp"
        type: "int8"
        range: [-128, 127]
        unit: "°C"
        description: "Maximum temperature threshold"

  - command: "Set Alarm Threshold (Single Probe)"
    hex_prefix: "0B"
    parameters:
      - name: "min_temp"
        type: "int8"
        range: [-128, 127]
        unit: "°C"
      - name: "max_temp"
        type: "int8"
        range: [-128, 127]
        unit: "°C"
      - name: "probe_index"
        type: "uint8"
        values:
          1: "Temperature_Red"
          2: "Temperature_White"
          3: "Temperature_Black"

  - command: "Set Alarm Interval"
    hex_prefix: "0D"
    parameters:
      - name: "interval"
        type: "uint16"
        range: [1, 65535]
        unit: "minutes"
        description: "Minimum interval between alarm messages"

  - command: "Get Alarm Settings"
    hex_prefix: "0E01"
    parameters: []

  - command: "Set Interrupt Mode"
    hex_prefix: "06"
    parameters:
      - name: "mode"
        type: "uint24"
        values:
          0: "Disabled"
          1: "Falling edge"
          2: "Rising edge"
          3: "Both edges"

  - command: "Set Power Output Duration"
    hex_prefix: "07"
    parameters:
      - name: "duration"
        type: "uint16"
        range: [0, 65535]
        unit: "milliseconds"
        description: "5V output duration before sampling"

  - command: "Set Multiple VDC Collection"
    hex_prefix: "AE"
    parameters:
      - name: "enable"
        type: "uint8"
        values:
          0: "Disabled"
          1: "Enabled"
      - name: "interval"
        type: "uint16"
        range: [1, 65535]
        unit: "seconds"
        description: "Collection interval"
      - name: "count"
        type: "uint8"
        range: [1, 120]
        description: "Number of collections per uplink"

  - command: "Poll Historical Data"
    hex_prefix: "31"
    parameters:
      - name: "start_time"
        type: "uint32"
        description: "Unix timestamp start"
      - name: "end_time"
        type: "uint32"
        description: "Unix timestamp end"

special_handling:
  crc_required: false
  temperature_alarm: true
  datalog_feature: true
  signed_temperature: true
  invalid_temperature: 0x7FFF
  multi_probe_support: true
  
model_variants:
  D20:
    probes: 1
    colors: ["Red"]
  D20S:
    probes: 1
    colors: ["Red"]
    soil_optimized: true
  D22:
    probes: 2
    colors: ["White", "Red"]
  D23:
    probes: 3
    colors: ["White", "Red", "Black"]

measurement_units:
  temperature: "°C"
  battery: "mV"
  voltage: "V"
  timestamp: "unix"
  rssi: "dBm"
