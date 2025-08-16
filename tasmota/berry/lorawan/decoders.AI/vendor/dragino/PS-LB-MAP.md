# Dragino PS-LB Protocol MAP
# Generated: 2025-08-16 19:07:25 | PDF Version: Latest
# Source: https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/PS-LB%20--%20LoRaWAN%20Pressure%20Sensor/

device_info:
  vendor: Dragino
  model: PS-LB
  type: Pressure Sensor
  lorawan_version: 1.0.3
  regions: [CN470, EU433, KR920, US915, EU868, AS923, AU915, IN865]
  official_link: https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/PS-LB%20--%20LoRaWAN%20Pressure%20Sensor/

uplinks:
  - port: 5
    type: device_status
    name: "Device Status"
    size: 6
    fields:
      - name: "sensor_model"
        size: 1
        value: 0x16
        description: "PS-LB/LS identifier"
      - name: "firmware_version"
        size: 2
        format: "major.minor.patch"
        description: "Firmware version"
      - name: "frequency_band"
        size: 1
        description: "Frequency band (1=EU868, 2=US915, etc)"
      - name: "sub_band"
        size: 1
        description: "Sub-band configuration"
      - name: "battery_voltage"
        size: 2
        unit: "mV"
        description: "Battery voltage"

  - port: 2
    type: sensor_data
    name: "Sensor Value"
    size: 9
    fields:
      - name: "probe_model"
        size: 2
        description: "Probe type and range configuration"
      - name: "idc_input"
        size: 2
        unit: "mA"
        range: [0, 20]
        resolution: 0.001
        description: "4-20mA current from pressure probe"
      - name: "vdc_input"
        size: 2
        unit: "V"
        range: [0, 30]
        resolution: 0.001
        description: "0-30V voltage input"
      - name: "battery_voltage"
        size: 2
        unit: "mV"
        description: "Battery voltage"
      - name: "status_flags"
        size: 1
        description: "IN1, IN2, INT pin status and packet type"

  - port: 7
    type: multi_data
    name: "Multiple Sensor Data"
    size: variable
    fields:
      - name: "data_count"
        size: 2
        description: "Number of data entries"
      - name: "voltage_values"
        size: variable
        description: "Multiple voltage readings (2 bytes each)"

  - port: 3
    type: datalog
    name: "Datalog Data"
    size: variable
    fields:
      - name: "data_entries"
        size: 11
        repeat: true
        description: "Historical data entries"
        subfields:
          - name: "probe_model"
            size: 2
          - name: "vdc_input"
            size: 2
            unit: "V"
          - name: "idc_input"
            size: 2
            unit: "mA"
          - name: "status_byte"
            size: 1
          - name: "unix_timestamp"
            size: 4

  - port: 2
    type: roc_data
    name: "Report on Change"
    size: 9
    conditions: "When ROC feature enabled"
    fields:
      - name: "probe_model"
        size: 2
      - name: "idc_input"
        size: 2
        unit: "mA"
      - name: "vdc_input"
        size: 2
        unit: "V"
      - name: "battery_voltage"
        size: 2
        unit: "mV"
      - name: "roc_flags"
        size: 1
        description: "ROC trigger flags + IN1/IN2/INT status"

downlinks:
  - command: "Set Transmit Interval"
    hex_prefix: "01"
    parameters:
      - name: "interval_seconds"
        type: "uint24"
        range: [30, 16777215]
        endian: "big"
        description: "TDC interval in seconds"

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

  - command: "Set Output Control"
    hex_prefix: "07"
    parameters:
      - name: "output_type"
        type: "uint8"
        values:
          1: "3V3"
          2: "5V"
          3: "12V"
      - name: "duration_ms"
        type: "uint16"
        range: [0, 65535]
        special_values:
          0: "normally_closed"
          65535: "normally_open"

  - command: "Set Probe Model"
    hex_prefix: "08"
    parameters:
      - name: "probe_config"
        type: "uint16"
        description: "Probe type and range configuration"

  - command: "Set ROC Mode"
    hex_prefix: "09"
    parameters:
      - name: "mode"
        type: "uint8"
        values:
          0: "Disabled"
          1: "Wave alarm"
          2: "Wave alarm with refresh on TDC"
          3: "Threshold alarm"
      - name: "interval"
        type: "uint16"
        unit: "seconds"
        description: "Detection interval"
      - name: "idc_threshold"
        type: "uint16"
        unit: "uA"
        description: "Current change threshold"
      - name: "vdc_threshold"
        type: "uint16"
        unit: "mV"
        description: "Voltage change threshold"

  - command: "Request Device Status"
    hex_prefix: "26"
    parameters:
      - name: "request_type"
        type: "uint8"
        values:
          1: "device_status"

  - command: "Set Multi-Collection Mode"
    hex_prefix: "AE"
    parameters:
      - name: "mode"
        type: "uint8"
        values:
          0: "Disabled"
          1: "VDC collection"
          2: "IDC collection"
      - name: "interval"
        type: "uint16"
        unit: "seconds"
        description: "Collection interval"
      - name: "count"
        type: "uint8"
        range: [1, 120]
        description: "Number of collections"

  - command: "Poll Datalog"
    hex_prefix: "31"
    parameters:
      - name: "start_timestamp"
        type: "uint32"
        description: "Unix timestamp start"
      - name: "end_timestamp"
        type: "uint32"
        description: "Unix timestamp end"
      - name: "uplink_interval"
        type: "uint8"
        range: [5, 255]
        unit: "seconds"
        description: "Response uplink interval"

  - command: "Clear Flash Record"
    hex_prefix: "A3"
    parameters:
      - name: "clear_flag"
        type: "uint8"
        values:
          1: "clear_all"

special_handling:
  crc_required: false
  reset_events: false
  battery_monitoring: true
  roc_feature: true
  datalog_feature: true
  unix_timestamp: true
  probe_types:
    - water_depth: "aa=00, bb=depth_meters"
    - pressure: "aa=01, bb=pressure_type"
    - differential: "aa=02, bb=range_type"

measurement_units:
  current: "mA"
  voltage: "V"
  battery: "mV"
  pressure: "Pa/MPa"
  water_depth: "m"
  time: "unix_timestamp"

status_flags:
  in1_level: "bit3"
  in2_level: "bit2"
  int_level: "bit1"
  int_status: "bit0"
  roc_flags: "bits4-7"

roc_flags:
  idc_decrease: "bit7"
  idc_increase: "bit6"
  vdc_decrease: "bit5"
  vdc_increase: "bit4"

frequency_bands:
  1: "EU868"
  2: "US915"
  3: "IN865"
  4: "AU915"
  5: "KZ865"
  6: "RU864"
  7: "AS923"
  8: "AS923-1"
  9: "AS923-2"
  10: "AS923-3"
  11: "CN470"
  12: "EU433"
  13: "KR920"
  14: "MA869"