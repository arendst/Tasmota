# Dragino DDS75-LB/LS Protocol MAP
# Generated: 2025-08-16 00:31:13 | PDF Version: v1.3
# Source: DDS75-LB_LoRaWAN_Distance_Detection_Sensor_User_Manual

device_info:
  vendor: Dragino
  model: DDS75-LB/LS
  type: Distance Detection Sensor  
  lorawan_version: 1.0.3
  regions: [CN470, EU433, KR920, US915, EU868, AS923, AU915, IN865]
  official_link: https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/DDS75-LB_LoRaWAN_Distance_Detection_Sensor_User_Manual/

uplinks:
  - port: 1
    type: 0x00
    name: "Periodic Data"
    payload_size: 8
    channels:
      - offset: 0
        size: 2
        name: "Battery Voltage"
        unit: "mV"
        formula: "((byte[1] << 8) | byte[0])"
        range: [2500, 3600]
      - offset: 2
        size: 2
        name: "Distance"
        unit: "mm"
        formula: "((byte[3] << 8) | byte[2])"
        range: [280, 7500]
        special_values:
          0x0000: "No ultrasonic sensor detected"
          0x0014: "Invalid distance (<280mm)"
      - offset: 4
        size: 1
        name: "Digital Interrupt"
        unit: "boolean"
        values:
          0x00: "Normal uplink"
          0x01: "Interrupt triggered uplink"
      - offset: 5
        size: 2
        name: "DS18B20 Temperature"
        unit: "°C"
        formula: "temp = (byte[6] << 8) | byte[5]; if (temp & 0xFC00) temp = (temp - 65536) / 10.0; else temp = temp / 10.0"
        range: [-55, 125]
        optional: true
      - offset: 7
        size: 1
        name: "Sensor Flag"
        unit: "boolean"
        values:
          0x01: "Ultrasonic sensor detected"
          0x00: "No ultrasonic sensor"

  - port: 5
    type: 0x26
    name: "Device Status"
    payload_size: 7
    channels:
      - offset: 0
        size: 1
        name: "Sensor Model"
        unit: "hex"
        values:
          0x27: "DDS75-LB/LS"
      - offset: 1
        size: 2
        name: "Firmware Version"
        unit: "version"
        formula: "v{(byte[2] << 8) | byte[1]:04X}"
      - offset: 3
        size: 1
        name: "Frequency Band"
        unit: "band"
        values:
          0x01: "EU868"
          0x02: "US915"
          0x03: "IN865"
          0x04: "AU915"
          0x05: "KZ865"
          0x06: "RU864"
          0x07: "AS923"
          0x08: "AS923-2"
          0x09: "AS923-3"
          0x0A: "AS923-4"
          0x0B: "CN470"
          0x0C: "EU433"
          0x0D: "KR920"
          0x0E: "MA869"
      - offset: 4
        size: 1
        name: "Sub-band"
        unit: "band"
        range: [0, 12]
      - offset: 5
        size: 2
        name: "Battery Voltage"
        unit: "mV"
        formula: "((byte[6] << 8) | byte[5])"

downlinks:
  - command: "Set Transmit Interval"
    hex_prefix: "01"
    parameters:
      - name: "interval_seconds"
        type: "uint24"
        range: [10, 16777215]
        endian: "little"
        format: "01{interval:06X}"
        
  - command: "Set Interrupt Mode"
    hex_prefix: "06"
    parameters:
      - name: "mode"
        type: "uint24"
        endian: "little"
        values:
          disable: "06000000"
          rising_edge: "06000003"
        format: "06{mode:06X}"
        
  - command: "Set Delta Detect Mode"
    hex_prefix: "FB"
    parameters:
      - name: "mode"
        type: "uint8"
        values:
          normal: 1
          delta: 2
      - name: "sampling_interval"
        type: "uint16"
        range: [1, 65535]
        unit: "seconds"
      - name: "delta_threshold"
        type: "uint16"
        range: [1, 65535]
        unit: "cm"
      - name: "sample_count"
        type: "uint8"
        range: [5, 20]
        format: "FB{mode:02X}{interval:04X}{threshold:04X}{count:02X}"
        
  - command: "Request Device Status"
    hex_prefix: "26"
    parameters:
      - name: "request"
        type: "uint8"
        values:
          status: "2601"

  - command: "Poll Sensor Data"
    hex_prefix: "31"
    parameters:
      - name: "timestamp_start"
        type: "uint32"
        unit: "unix_timestamp"
      - name: "timestamp_end"
        type: "uint32"
        unit: "unix_timestamp"
      - name: "uplink_interval"
        type: "uint8"
        range: [5, 255]
        unit: "seconds"
        format: "31{start:08X}{end:08X}{interval:02X}"

special_handling:
  crc_required: false
  reset_events: false
  battery_monitoring: true
  signed_values:
    - "ds18b20_temperature"
  invalid_readings:
    distance_0x0000: "No sensor detected"
    distance_0x0014: "Invalid reading (<280mm)"
  datalog_feature: true
  delta_detect_mode: true
  
measurement_units:
  distance: "mm"
  temperature: "°C"
  voltage: "mV"
  battery: "mV"
  interrupt: "boolean"
  sensor_flag: "boolean"

accuracy:
  distance: "±(1cm + S*0.3%)"
  temperature: "±0.5°C"
  voltage: "±10mV"

operating_ranges:
  distance: [280, 7500]  # mm
  temperature: [-40, 85]  # °C operating, [-55, 125] sensor
  voltage: [2500, 3600]  # mV

power_modes:
  sleep: "5µA @ 3.3V"
  transmit: "125mA @ 20dBm, 82mA @ 14dBm"
  
battery_specs:
  type: "Li/SOCl2 (LB) or Li-ion+Solar (LS)"
  capacity: "8500mAh (LB) or 3000mAh (LS)"
  life_expectancy: "5 years (LB)"
