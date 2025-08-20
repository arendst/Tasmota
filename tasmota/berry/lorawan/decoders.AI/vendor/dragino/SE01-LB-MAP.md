# Dragino SE01-LB Protocol MAP
# Generated: 2025-08-20 14:42:35 | PDF Version: Not specified
# Source: https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/SE01-LB_LoRaWAN_Soil%20Moisture%26EC_Sensor_User_Manual/

device_info:
  vendor: Dragino
  model: SE01-LB
  type: Soil Moisture & EC Sensor
  lorawan_version: 1.0.3
  regions: [CN470, EU433, KR920, US915, EU868, AS923, AU915, IN865]
  official_link: https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/SE01-LB_LoRaWAN_Soil%20Moisture%26EC_Sensor_User_Manual/

uplinks:
  - port: 2
    type: "MOD=0 interrupt mode"
    name: "Periodic Data Interrupt"
    size: 11
    channels:
      - name: "DS18B20 Temperature"
        size: 2
        unit: "°C"
        resolution: 0.1
        range: [-40, 85]
        encoding: "signed int16 / 10"
        notes: "External temperature sensor, 327.67°C when disconnected"
      - name: "Soil Moisture"
        size: 2
        unit: "%"
        resolution: 0.01
        range: [0, 100]
        encoding: "uint16 / 100"
        notes: "FDR method with temp/EC compensation"
      - name: "Soil Temperature"
        size: 2
        unit: "°C"
        resolution: 0.01
        range: [-40, 85]
        encoding: "signed int16 / 100"
        notes: "Built-in soil temperature sensor"
      - name: "Soil Conductivity"
        size: 2
        unit: "uS/cm"
        resolution: 1
        range: [0, 20000]
        encoding: "uint16"
        notes: "Electrical conductivity with temp compensation"
      - name: "Battery Voltage"
        size: 2
        unit: "mV"
        resolution: 1
        range: [2500, 3600]
        encoding: "uint16"
        notes: "Li-SOCI2 battery voltage"
      - name: "MOD & Flags"
        size: 1
        unit: "flags"
        resolution: 1
        range: [0, 255]
        encoding: "bitfield"
        notes: "MOD, i_flag, s_flag encoded"

  - port: 2
    type: "MOD=0 counting mode"
    name: "Periodic Data Counting"
    size: 15
    channels:
      - name: "DS18B20 Temperature"
        size: 2
        unit: "°C"
        resolution: 0.1
        range: [-40, 85]
        encoding: "signed int16 / 10"
      - name: "Soil Moisture"
        size: 2
        unit: "%"
        resolution: 0.01
        range: [0, 100]
        encoding: "uint16 / 100"
      - name: "Soil Temperature"
        size: 2
        unit: "°C"
        resolution: 0.01
        range: [-40, 85]
        encoding: "signed int16 / 100"
      - name: "Soil Conductivity"
        size: 2
        unit: "uS/cm"
        resolution: 1
        range: [0, 20000]
        encoding: "uint16"
      - name: "Battery Voltage"
        size: 2
        unit: "mV"
        resolution: 1
        range: [2500, 3600]
        encoding: "uint16"
      - name: "MOD & Flags"
        size: 1
        unit: "flags"
        resolution: 1
        range: [0, 255]
        encoding: "bitfield"
      - name: "Count Value"
        size: 4
        unit: "count"
        resolution: 1
        range: [0, 4294967295]
        encoding: "uint32"

  - port: 2
    type: "MOD=1 interrupt mode"
    name: "Raw Value Interrupt"
    size: 11
    channels:
      - name: "Reserve Temperature"
        size: 2
        unit: "raw"
        notes: "Reserved, ignore"
      - name: "Soil Conductivity Raw"
        size: 2
        unit: "raw"
        notes: "Raw conductivity with temp compensation"
      - name: "Soil Moisture Raw"
        size: 2
        unit: "raw"
        notes: "Raw AD value of moisture"
      - name: "Dielectric Constant Raw"
        size: 2
        unit: "raw"
        notes: "Raw dielectric constant"
      - name: "Battery Voltage"
        size: 2
        unit: "mV"
        resolution: 1
        range: [2500, 3600]
        encoding: "uint16"
      - name: "MOD & Flags"
        size: 1
        unit: "flags"
        resolution: 1
        range: [0, 255]
        encoding: "bitfield"

  - port: 2
    type: "MOD=1 counting mode"
    name: "Raw Value Counting"
    size: 15
    channels:
      - name: "Reserve Temperature"
        size: 2
        unit: "raw"
        notes: "Reserved, ignore"
      - name: "Soil Conductivity Raw"
        size: 2
        unit: "raw"
        notes: "Raw conductivity with temp compensation"
      - name: "Soil Moisture Raw"
        size: 2
        unit: "raw"
        notes: "Raw AD value of moisture"
      - name: "Dielectric Constant Raw"
        size: 2
        unit: "raw"
        notes: "Raw dielectric constant"
      - name: "Battery Voltage"
        size: 2
        unit: "mV"
        resolution: 1
        range: [2500, 3600]
        encoding: "uint16"
      - name: "MOD & Flags"
        size: 1
        unit: "flags"
        resolution: 1
        range: [0, 255]
        encoding: "bitfield"
      - name: "Count Value"
        size: 4
        unit: "count"
        resolution: 1
        range: [0, 4294967295]
        encoding: "uint32"

  - port: 5
    type: "device status"
    name: "Device Status"
    size: 7
    channels:
      - name: "Sensor Model"
        size: 1
        unit: "hex"
        fixed_value: 0x26
        notes: "Always 0x26 for SE01-LB"
      - name: "Firmware Version"
        size: 2
        unit: "version"
        encoding: "uint16 BCD"
        notes: "e.g., 0x0200 = v2.0.0"
      - name: "Frequency Band"
        size: 1
        unit: "band"
        encoding: "enum"
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
      - name: "Sub-Band"
        size: 1
        unit: "subband"
        encoding: "uint8"
        notes: "0x00-0x08 for US915/AU915, 0x0B-0x0C for CN470, 0x00 for others"
      - name: "Battery Voltage"
        size: 2
        unit: "mV"
        resolution: 1
        range: [2500, 3600]
        encoding: "uint16"

  - port: 3
    type: "datalog"
    name: "Datalog Data"
    size: "variable"
    channels:
      - name: "MOD=0 Datalog Entry"
        size: 11
        fields:
          - name: "Humidity"
            size: 2
            unit: "%"
            encoding: "uint16 / 100"
          - name: "Temperature"
            size: 2
            unit: "°C"
            encoding: "signed int16 / 100"
          - name: "Soil Conductivity"
            size: 2
            unit: "uS/cm"
            encoding: "uint16"
          - name: "MOD & Level"
            size: 1
            unit: "flags"
            encoding: "bitfield"
          - name: "Unix Timestamp"
            size: 4
            unit: "timestamp"
            encoding: "uint32"
      - name: "MOD=1 Datalog Entry"
        size: 11
        fields:
          - name: "Soil Dielectric Constant"
            size: 2
            unit: "raw"
            encoding: "uint16"
          - name: "Raw Water Soil"
            size: 2
            unit: "raw"
            encoding: "uint16"
          - name: "Raw Conduct Soil"
            size: 2
            unit: "raw"
            encoding: "uint16"
          - name: "MOD & Level"
            size: 1
            unit: "flags"
            encoding: "bitfield"
          - name: "Unix Timestamp"
            size: 4
            unit: "timestamp"
            encoding: "uint32"

downlinks:
  - command: "Set Transmit Interval"
    hex_prefix: "01"
    parameters:
      - name: "interval_seconds"
        type: "uint24"
        range: [30, 16777215]
        endian: "big"
        notes: "Minimum 30 seconds"
    example: "0100003C = 60 seconds"

  - command: "Reset Device"
    hex_prefix: "04"
    parameters:
      - name: "reset_flag"
        type: "uint8"
        fixed_value: 0xFF
    example: "04FF = reset device"

  - command: "Set Confirm Mode"
    hex_prefix: "05"
    parameters:
      - name: "confirm_mode"
        type: "uint24"
        values:
          0x000000: "Unconfirmed"
          0x000001: "Confirmed"
    example: "05000001 = enable confirmed mode"

  - command: "Set Interrupt Mode"
    hex_prefix: "06"
    parameters:
      - name: "interrupt_mode"
        type: "uint24"
        values:
          0x000000: "Disabled"
          0x000003: "Rising edge trigger"
    example: "06000003 = rising edge trigger"

  - command: "Set 5V Output Duration"
    hex_prefix: "07"
    parameters:
      - name: "duration_ms"
        type: "uint16"
        range: [0, 65535]
        endian: "big"
    example: "0701F4 = 500ms duration"

  - command: "Set Count Value"
    hex_prefix: "09"
    parameters:
      - name: "count_value"
        type: "uint32"
        range: [0, 4294967295]
        endian: "big"
    example: "09000003E8 = start counting from 1000"

  - command: "Set Working Mode"
    hex_prefix: "0A"
    parameters:
      - name: "mode"
        type: "uint8"
        values:
          0x00: "Default mode (calibrated values)"
          0x01: "Original value (raw ADC)"
    example: "0A01 = original value mode"

  - command: "Set Count Mode"
    hex_prefix: "10"
    parameters:
      - name: "count_mode"
        type: "uint8"
        values:
          0x00: "Interrupt mode"
          0x01: "Counting mode"
    example: "1001 = counting mode"

  - command: "Request Device Status"
    hex_prefix: "26"
    parameters:
      - name: "status_request"
        type: "uint8"
        fixed_value: 0x01
    example: "2601 = request status (fport=5)"

  - command: "Poll Datalog"
    hex_prefix: "31"
    parameters:
      - name: "start_timestamp"
        type: "uint32"
        range: [0, 4294967295]
        endian: "big"
      - name: "end_timestamp"
        type: "uint32"
        range: [0, 4294967295]
        endian: "big"
      - name: "uplink_interval"
        type: "uint8"
        range: [5, 255]
        unit: "seconds"
    example: "31646D84E1646D856C05 = poll data with 5s interval"

special_handling:
  crc_required: false
  reset_events: true
  battery_monitoring: true
  signed_values:
    - "ds18b20_temperature"
    - "soil_temperature"
  
measurement_units:
  ds18b20_temperature: "°C"
  soil_moisture: "%"
  soil_temperature: "°C"
  soil_conductivity: "uS/cm"
  battery_voltage: "mV"
  count_value: "count"
  firmware_version: "version"
  unix_timestamp: "timestamp"
