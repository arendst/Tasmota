# Dragino LHT52 Protocol MAP
# Generated: 2025-08-16 12:30:45 | PDF Version: v1.0.0
# Source: https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/LHT52%20-%20LoRaWAN%20Temperature%20%26%20Humidity%20Sensor%20User%20Manual/

device_info:
  vendor: Dragino
  model: LHT52
  type: Temperature & Humidity Sensor
  lorawan_version: v1.0.3
  regions: [CN470, EU433, KR920, US915, EU868, AS923, AU915, IN865]
  official_link: https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/LHT52%20-%20LoRaWAN%20Temperature%20%26%20Humidity%20Sensor%20User%20Manual/

uplinks:
  - port: 5
    type: 0x01
    name: "Device Status"
    description: "Uplinked once on join and every 12 hours"
    channels:
      - name: "Sensor Model"
        size: 1
        offset: 0
        value: 0x09
        description: "Always 0x09 for LHT52"
      - name: "Firmware Version"
        size: 2
        offset: 1
        unit: "version"
        description: "e.g., 0x0100 = v1.0.0"
      - name: "Frequency Band"
        size: 1
        offset: 3
        description: "0x01=EU868, 0x02=US915, etc."
      - name: "Sub-band"
        size: 1
        offset: 4
        description: "0x00-0x08 for CN470/AU915/US915, 0x00 others"
      - name: "Battery Voltage"
        size: 2
        offset: 5
        unit: "mV"
        description: "e.g., 0x0B3A = 2874mV"

  - port: 2
    type: 0x02
    name: "Real-time Sensor Value"
    description: "Periodic sensor data, default 20 minutes"
    channels:
      - name: "Temperature"
        size: 2
        offset: 0
        unit: "°C"
        resolution: 0.01
        range: [-40, 125]
        description: "Signed 16-bit, divide by 100"
      - name: "Humidity"
        size: 2
        offset: 2
        unit: "%RH"
        resolution: 0.1
        range: [0, 100]
        description: "Divide by 10"
      - name: "External Temperature"
        size: 2
        offset: 4
        unit: "°C"
        resolution: 0.01
        range: [-55, 125]
        description: "From DS18B20 probe if connected"
      - name: "Extension"
        size: 1
        offset: 6
        description: "0x01=AS-01 Temperature probe"
      - name: "Unix Timestamp"
        size: 4
        offset: 7
        unit: "timestamp"
        description: "Unix timestamp for datalog"

  - port: 3
    type: 0x03
    name: "Datalog Sensor Value"
    description: "Historical data retrieved via downlink"
    channels:
      - name: "Temperature"
        size: 2
        offset: 0
        unit: "°C"
        resolution: 0.01
        description: "Same format as real-time"
      - name: "Humidity"
        size: 2
        offset: 2
        unit: "%RH"
        resolution: 0.1
        description: "Same format as real-time"
      - name: "External Temperature"
        size: 2
        offset: 4
        unit: "°C"
        resolution: 0.01
        description: "From DS18B20 probe"
      - name: "Extension"
        size: 1
        offset: 6
        description: "External sensor type"
      - name: "Unix Timestamp"
        size: 4
        offset: 7
        unit: "timestamp"
        description: "Sampling timestamp"

  - port: 4
    type: 0x04
    name: "DS18B20 ID"
    description: "External sensor ID response"
    channels:
      - name: "DS18B20 ID"
        size: 8
        offset: 0
        description: "64-bit unique sensor ID"

downlinks:
  - command: "Set TDC Time"
    hex_prefix: "01"
    parameters:
      - name: "interval"
        type: "uint32"
        range: [1, 86400000]
        unit: "ms"
        description: "Transmission interval in milliseconds"

  - command: "Reset Device"
    hex_prefix: "04FF"
    parameters: []
    description: "Restart the device"

  - command: "Factory Reset"
    hex_prefix: "04FE"
    parameters: []
    description: "Restore factory settings"

  - command: "Set Confirmation Mode"
    hex_prefix: "05"
    parameters:
      - name: "mode"
        type: "boolean"
        values:
          enable: "01"
          disable: "00"

  - command: "Set Sub-band"
    hex_prefix: "07"
    parameters:
      - name: "subband"
        type: "uint8"
        range: [0, 8]
        description: "Sub-band selection 0-8"

  - command: "Set Network Join Mode"
    hex_prefix: "20"
    parameters:
      - name: "mode"
        type: "boolean"
        values:
          otaa: "01"
          abp: "00"

  - command: "Set ADR"
    hex_prefix: "22"
    parameters:
      - name: "adr"
        type: "boolean"
        values:
          enable: "01"
          disable: "00"

  - command: "Request Device Status"
    hex_prefix: "2301"
    parameters: []
    description: "Request device configuration upload"

  - command: "Request DS18B20 ID"
    hex_prefix: "2302"
    parameters: []
    description: "Request external sensor ID"

  - command: "Set DWELL Time"
    hex_prefix: "25"
    parameters:
      - name: "enable"
        type: "boolean"
        values:
          enable: "01"
          disable: "00"

  - command: "Set Rejoin Interval"
    hex_prefix: "26"
    parameters:
      - name: "minutes"
        type: "uint16"
        range: [1, 65535]
        unit: "minutes"

  - command: "Poll Sensor Data"
    hex_prefix: "31"
    parameters:
      - name: "timestamp_start"
        type: "uint32"
        description: "Unix timestamp start"
      - name: "timestamp_end"
        type: "uint32"
        description: "Unix timestamp end"
      - name: "interval"
        type: "uint8"
        range: [5, 255]
        unit: "seconds"
        description: "Uplink interval for response"

  - command: "Set Device Detection"
    hex_prefix: "32"
    parameters:
      - name: "enable"
        type: "boolean"
        values:
          enable: "01"
          disable: "00"
      - name: "timeout"
        type: "uint16"
        range: [1, 65535]
        unit: "minutes"
        description: "ACK timeout for offline detection"

  - command: "Set Alarm Mode"
    hex_prefix: "A5"
    parameters:
      - name: "enable"
        type: "boolean"
        values:
          enable: "01"
          disable: "00"

  - command: "Set Temperature Check Interval"
    hex_prefix: "A7"
    parameters:
      - name: "interval"
        type: "uint16"
        range: [1, 65535]
        unit: "minutes"
        description: "Temperature alarm check interval"

  - command: "Set Temperature Range"
    hex_prefix: "AA"
    parameters:
      - name: "mode"
        type: "uint8"
        description: "Alarm mode setting"
      - name: "check_interval"
        type: "uint16"
        unit: "minutes"
      - name: "temp_min"
        type: "int16"
        unit: "°C"
        description: "Minimum temperature threshold"
      - name: "temp_max"
        type: "int16"
        unit: "°C"
        description: "Maximum temperature threshold"

special_handling:
  crc_required: false
  reset_events: true
  battery_monitoring: true
  signed_values:
    - "temperature"
    - "external_temperature"
  datalog_feature: true
  alarm_mode: true
  external_sensor_support: true

measurement_units:
  temperature: "°C"
  humidity: "%RH"
  voltage: "mV"
  timestamp: "unix"
  interval: "minutes"
