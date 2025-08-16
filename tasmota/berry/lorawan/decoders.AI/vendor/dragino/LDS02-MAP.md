# Dragino LDS02 Protocol MAP
# Generated: 2025-08-16 00:45:43 | PDF Version: v1.8.2
# Source: LDS02_LoRaWAN_Door_Sensor_User_Manual

device_info:
  vendor: Dragino
  model: LDS02
  type: Door Sensor
  lorawan_version: 1.0.3
  regions: [CN470, EU433, KR920, US915, EU868, AS923, AU915, IN865, CN779]
  official_link: https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/LDS02%20-%20LoRaWAN%20Door%20Sensor%20User%20Manual/

uplinks:
  - port: 10
    type: 0x01
    name: "Sensor Data"
    payload_size: 10
    channels:
      - offset: 0
        size: 2
        name: "Battery & Door Status"
        format: "((byte[1] << 8) | byte[0])"
        battery_formula: "((raw & 0x3FFF))"
        battery_unit: "mV"
        door_status_formula: "((raw & 0x8000) >> 15)"
        door_values:
          0: "Closed"
          1: "Open"
      - offset: 2
        size: 1
        name: "Mode"
        unit: "mode"
        values:
          0x01: "Normal Mode"
      - offset: 3
        size: 3
        name: "Total Door Open Events"
        unit: "count"
        formula: "(byte[5] << 16) | (byte[4] << 8) | byte[3]"
        range: [0, 16777215]
      - offset: 6
        size: 3
        name: "Last Door Open Duration"
        unit: "minutes"
        formula: "(byte[8] << 16) | (byte[7] << 8) | byte[6]"
        range: [0, 16777215]
      - offset: 9
        size: 1
        name: "Alarm Status"
        unit: "boolean"
        formula: "(byte[9] & 0x01)"
        values:
          0: "No alarm"
          1: "Timeout alarm"

  - port: 7
    type: 0x02
    name: "EDC Mode Data"
    payload_size: 5
    channels:
      - offset: 0
        size: 2
        name: "Battery & EDC Mode"
        format: "((byte[1] << 8) | byte[0])"
        battery_formula: "((raw & 0x3FFF))"
        battery_unit: "mV"
        edc_mode_formula: "((raw & 0x8000) >> 15)"
        edc_values:
          0: "Close count mode"
          1: "Open count mode"
      - offset: 2
        size: 3
        name: "Event Count"
        unit: "count"
        formula: "(byte[4] << 16) | (byte[3] << 8) | byte[2]"
        range: [0, 16777215]

downlinks:
  - command: "Set Transmit Interval"
    hex_prefix: "01"
    parameters:
      - name: "interval_seconds"
        type: "uint32"
        range: [1, 4294967295]
        endian: "little"
        format: "01{interval:08X}"

  - command: "Set EDC Mode"
    hex_prefix: "02"
    parameters:
      - name: "mode"
        type: "uint8"
        values:
          open_count: 1
          close_count: 0
      - name: "count_threshold"
        type: "uint32"
        range: [1, 4294967295]
        endian: "little"
        format: "02{mode:02X}{count:08X}"

  - command: "Reset Device"
    hex_prefix: "04"
    parameters:
      - name: "reset_command"
        type: "fixed"
        values:
          reset: "04FF"

  - command: "Set Confirmed Mode"
    hex_prefix: "05"
    parameters:
      - name: "mode"
        type: "uint8"
        values:
          unconfirmed: "0500"
          confirmed: "0501"

  - command: "Clear Counting"
    hex_prefix: "A6"
    parameters:
      - name: "clear_command"
        type: "fixed"
        values:
          clear: "A601"

  - command: "Enable/Disable Alarm"
    hex_prefix: "A7"
    parameters:
      - name: "alarm_mode"
        type: "uint8"
        values:
          disable: "A700"
          enable: "A701"

  - command: "Control ADR/DR"
    hex_prefix: "A8"
    parameters:
      - name: "adr_enable"
        type: "uint8"
        values:
          disable: 0
          enable: 1
      - name: "data_rate"
        type: "uint8"
        range: [0, 15]
        format: "A8{adr:02X}{dr:02X}"

  - command: "Set Alarm Timeout"
    hex_prefix: "A9"
    parameters:
      - name: "status_monitor"
        type: "uint8"
        values:
          disable: 0
          open_timeout: 1
      - name: "timeout_seconds"
        type: "uint16"
        range: [0, 65535]
        endian: "big"
        format: "A9{status:02X}{timeout:04X}"

  - command: "Set Count Value"
    hex_prefix: "AA"
    parameters:
      - name: "edc_mode"
        type: "uint8"
        optional: true
        values:
          close: 0
          open: 1
      - name: "count_value"
        type: "uint24"
        range: [0, 16777215]
        endian: "big"
        format: "AA{mode:02X}{count:06X}"

special_handling:
  crc_required: false
  reset_events: false
  battery_monitoring: true
  signed_values: []
  alarm_timeout: true
  edc_mode: true
  event_counting: true
  
measurement_units:
  door_status: "boolean"
  battery: "mV"
  event_count: "count"
  duration: "minutes"
  alarm: "boolean"

operating_ranges:
  temperature: [-10, 50]  # Standard AAA battery
  temperature_extended: [-40, 60]  # Special AAA battery (Energizer L92)
  battery_voltage: [2100, 3600]  # mV
  
power_specs:
  battery_type: "2 x AAA LR03"
  battery_life: "16,000 to 70,000 uplinks"
  low_power_mode: true
  
sensor_specs:
  detection_method: "Magnetic reed switch"
  detection_gap: "~10mm threshold"
  response_time: "Immediate"
  
features:
  keep_alive: "24 hours default"
  event_triggered: "Door open/close"
  timeout_alarm: "Configurable"
  edc_mode: "Event-driven counting (v1.8.2+)"
  count_initialization: "Configurable initial values"
  alarm_disable: "Battery saving mode"
