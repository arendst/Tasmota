# Dragino LHT65 Protocol MAP
# Generated: 2025-08-16 12:00:00 UTC | PDF Version: v1.8.3
# Source: LHT65_Temperature_Humidity_Sensor_UserManual_v1.8.5.pdf

device_info:
  vendor: Dragino
  model: LHT65
  type: Temperature & Humidity Sensor
  lorawan_version: 1.0.3
  firmware_version: v1.8.5
  manual_version: v1.8.3
  regions: [CN470, EU433, KR920, US915, EU868, AS923, AU915, IN865]
  official_link: https://www.dragino.com/products/lora-lorawan-end-node/item/151-lht65.html
  manual_link: https://www.dragino.com/downloads/downloads/LHT65/UserManual/LHT65_Temperature_Humidity_Sensor_UserManual_v1.8.5.pdf

uplinks:
  - port: 2
    type: 0x00
    name: "Standard Payload"
    description: "11-byte payload with battery, temperature, humidity, and external sensor"
    interval: 1200  # 20 minutes default
    payload_structure:
      - offset: 0
        size: 2
        name: "BAT"
        description: "Battery status (bits 15:14) and voltage (bits 13:0)"
        format: "uint16"
        decoding: |
          status = (value >> 14) & 0x03
          voltage_mv = value & 0x3FFF
          status_map: 00=Ultra Low(<=2.50v), 01=Low(2.50-2.55v), 10=OK(2.55-2.65v), 11=Good(>=2.65v)
      - offset: 2
        size: 2
        name: "Built-In Temperature"
        description: "SHT20 temperature sensor reading"
        format: "int16"
        unit: "°C"
        resolution: 0.01
        range: [-40, 125]
        decoding: "temperature = value / 100.0 (handle signed values)"
      - offset: 4
        size: 2
        name: "Built-in Humidity"
        description: "SHT20 humidity sensor reading"
        format: "uint16"
        unit: "%RH"
        resolution: 0.1
        range: [0, 100]
        decoding: "humidity = value / 10.0"
      - offset: 6
        size: 1
        name: "Ext #"
        description: "External sensor type identifier"
        format: "uint8"
        values:
          0x00: "No external sensor"
          0x01: "E1 Temperature Sensor (DS18B20)"
          0x04: "E4 Interrupt Sensor"
          0x05: "E5 Illumination Sensor"
          0x06: "E6 ADC Sensor"
          0x07: "E7 Counting Sensor (16-bit)"
          0x08: "E7 Counting Sensor (32-bit)"
          0x09: "E1 with Unix Timestamp (Datalog mode)"
      - offset: 7
        size: 4
        name: "Ext value"
        description: "External sensor data (format depends on Ext # type)"
        format: "varies"
        ext_formats:
          0x01: # E1 Temperature
            temperature: "int16 at offset 0, /100.0 for °C, range -55°C to 125°C"
            unused: "2 bytes at offset 2 (meaningless)"
            error_value: "0x7FFF = 327.67°C indicates no sensor connected"
          0x04: # E4 Interrupt
            byte0: "bit 7: cable status (0=disconnected, 1=connected)"
            byte0: "bit 6: interrupt flag (0=normal, 1=interrupt triggered)"
            byte0: "bit 0: pin level (0=low, 1=high)"
            unused: "3 bytes (reserved)"
          0x05: # E5 Illumination
            illuminance: "uint16 at offset 0, range 0-65535 lux"
            cable_status: "bit 7 of byte 2 (0=disconnected, 1=connected)"
            unused: "remaining bytes"
          0x06: # E6 ADC
            voltage: "uint16 at offset 0, range 0-3300mV (ADC reading)"
            cable_status: "bit 7 of byte 2 (0=disconnected, 1=connected)"
            unused: "remaining bytes"
          0x07: # E7 Counting 16-bit
            count: "uint16 at offset 0, event counter (max FFFF then rollover)"
            cable_status: "bit 7 of byte 2 (0=disconnected, 1=connected)"
            unused: "remaining bytes"
          0x08: # E7 Counting 32-bit
            count: "uint32 at offset 0, event counter (max FFFFFFFF then rollover)"
          0x09: # E1 with timestamp (special 11-byte format)
            ext_temp: "int16 at offset 0, /100.0 for external temperature"
            builtin_temp: "int16 at offset 2, /100.0 for built-in temperature"
            bat_hum: "uint16 at offset 4, bits 15:14=battery status, bits 11:0=humidity"
            status_ext: "uint8 at offset 6, bits 7:4=status flags, bits 3:0=0x09"
            timestamp: "uint32 at offset 7, Unix timestamp"

downlinks:
  - command: "Set Transmit Interval"
    hex_prefix: "01"
    parameters:
      - name: "interval_seconds"
        type: "uint24"
        range: [1, 16777215]
        endian: "big"
        description: "Uplink interval in seconds"
    examples:
      - hex: "0100003C"
        description: "Set interval to 60 seconds"
      - hex: "0100001E" 
        description: "Set interval to 30 seconds"
    at_command: "AT+TDC"

  - command: "Set External Sensor Mode"
    hex_prefix: "A2"
    parameters:
      - name: "sensor_type"
        type: "uint8"
        values:
          0x01: "E1 Temperature"
          0x04: "E4 Interrupt"
          0x05: "E5 Illumination"
          0x06: "E6 ADC"
          0x07: "E7 Counting 16-bit"
          0x08: "E7 Counting 32-bit"
          0x09: "E1 with timestamp"
      - name: "mode_parameter"
        type: "uint8"
        optional: true
        values:
          interrupt_modes:
            0x01: "Both rising and falling"
            0x02: "Falling only"
            0x03: "Rising only"
          count_modes:
            0x00: "Count on falling"
            0x01: "Count on rising"
      - name: "timeout_ms"
        type: "uint16"
        optional: true
        range: [0, 65535]
        description: "ADC sensor power timeout (E6 only)"
    examples:
      - hex: "A201"
        description: "Set to E1 temperature sensor"
      - hex: "A20401"
        description: "Set interrupt both edges"
      - hex: "A20603E8"
        description: "Set ADC with 1000ms timeout"
      - hex: "A20701"
        description: "Set counting on rising edge"
    at_command: "AT+EXT"

  - command: "Enable/Disable DS18B20 Probe ID"
    hex_prefix: "A8"
    parameters:
      - name: "enable"
        type: "boolean"
        values:
          enabled: "01"
          disabled: "00"
    examples:
      - hex: "A800"
        description: "Disable probe ID uplink"
      - hex: "A801"
        description: "Enable probe ID uplink (every 24h)"
    at_command: "AT+PID"

  - command: "Set System Time"
    hex_prefix: "30"
    parameters:
      - name: "unix_timestamp"
        type: "uint32"
        endian: "big"
        description: "Unix timestamp in seconds"
    examples:
      - hex: "306007806000"
        description: "Set timestamp to 1611104352"
    at_command: "AT+TIMESTAMP"

  - command: "Set Time Sync Mode"
    hex_prefix: "28"
    parameters:
      - name: "sync_mode"
        type: "boolean"
        values:
          manual: "00"
          auto_lorawan_mac: "01"
    examples:
      - hex: "2800"
        description: "Manual time sync"
      - hex: "2801"
        description: "Auto sync via LoRaWAN MAC command"
    at_command: "AT+SYNCMOD"

  - command: "Set Time Sync Interval"
    hex_prefix: "29"
    parameters:
      - name: "interval_days"
        type: "uint8"
        range: [1, 255]
        description: "Time sync interval in days"
    examples:
      - hex: "290A"
        description: "Sync every 10 days"
    at_command: "AT+SYNCTDC"

  - command: "Clear Flash Record"
    hex_prefix: "A3"
    parameters:
      - name: "confirm"
        type: "constant"
        value: "01"
    examples:
      - hex: "A301"
        description: "Clear all datalog records"
    at_command: "AT+CLRDTA"

  - command: "Poll Sensor Data"
    hex_prefix: "31"
    parameters:
      - name: "start_timestamp"
        type: "uint32"
        endian: "big"
        description: "Start time (Unix timestamp)"
      - name: "end_timestamp"
        type: "uint32"
        endian: "big"
        description: "End time (Unix timestamp)"
      - name: "uplink_interval"
        type: "uint8"
        range: [5, 255]
        description: "Uplink interval in seconds for poll responses"
    examples:
      - hex: "315FC5F3505FC6016005"
        description: "Poll data from specific time range with 5s interval"
    description: "Request historical datalog entries"

special_handling:
  crc_required: false
  reset_events: false
  battery_monitoring: true
  signed_values:
    - "builtin_temperature"
    - "ext_temperature"
  error_detection:
    - sensor_disconnection: true
    - cable_status_monitoring: true
    - invalid_readings: true
  datalog_support: true
  alarm_mode: true
  
measurement_units:
  temperature: "°C"
  humidity: "%RH"
  voltage: "mV"
  illuminance: "lux"
  adc_voltage: "mV"
  count: "events"
  timestamp: "Unix seconds"

sensor_ranges:
  builtin_temperature: [-40, 125]
  builtin_humidity: [0, 100]
  ext_temperature: [-55, 125]
  illuminance: [0, 65535]
  adc_voltage: [0, 3300]
  battery_voltage: [2450, 3600]

resolution:
  builtin_temperature: 0.01
  builtin_humidity: 0.1
  ext_temperature: 0.0625
  illuminance: 1
  adc_voltage: 1
  battery_voltage: 1
