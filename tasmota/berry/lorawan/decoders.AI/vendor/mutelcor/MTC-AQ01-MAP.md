# Mutelcor MTC-AQ01 Protocol MAP
# Generated: 2025-08-20 20:35:22 | PDF Version: 1.6.0
# Source: Mutelcor-LoRaWAN-Payload-1.6.0.pdf

device_info:
  vendor: Mutelcor
  model: MTC-AQ01
  type: Air Quality Sensor
  lorawan_version: 1.0.3
  regions: [EU863-870, IN865-867, US902-928, AU915-928, AS923-1-4, KR920-923]
  official_link: https://mutelcor.com/lora-air-quality-sensor/

uplinks:
  - port: 1
    type: 0x00
    name: "Heartbeat"
    payload_size: 4
    channels:
      - id: 0x00
        type: 0x00
        name: "Version"
        size: 1
        unit: ""
        value: 2
        position: 0
      - id: 0x01
        type: 0x01
        name: "Battery Voltage"
        size: 2
        unit: "V"
        resolution: 0.01
        range: [2.0, 3.3]
        endian: "big"
        position: 1-2
      - id: 0x02
        type: 0x02
        name: "OpCode"
        size: 1
        unit: ""
        value: 0x00
        position: 3

  - port: 1
    type: 0x03
    name: "Measurements"
    payload_size: 10
    channels:
      - id: 0x00
        type: 0x00
        name: "Version"
        size: 1
        unit: ""
        value: 2
        position: 0
      - id: 0x01
        type: 0x01
        name: "Battery Voltage"
        size: 2
        unit: "V"
        resolution: 0.01
        range: [2.0, 3.3]
        endian: "big"
        position: 1-2
      - id: 0x02
        type: 0x02
        name: "OpCode"
        size: 1
        unit: ""
        value: 0x03
        position: 3
      - id: 0x03
        type: 0x03
        name: "Measurements Bitmask"
        size: 1
        unit: ""
        description: "Bit 0=Temp, Bit 1=RH, Bit 2=Pressure"
        position: 4
      - id: 0x04
        type: 0x04
        name: "Temperature"
        size: 2
        unit: "°C"
        resolution: 0.1
        range: [-40, 85]
        endian: "big"
        signed: true
        position: 5-6
      - id: 0x05
        type: 0x05
        name: "Relative Humidity"
        size: 1
        unit: "%"
        range: [0, 100]
        position: 7
      - id: 0x06
        type: 0x06
        name: "Pressure"
        size: 2
        unit: "Pa"
        resolution: 10
        range: [30000, 110000]
        endian: "big"
        position: 8-9

  - port: 1
    type: 0x05
    name: "Thresholds"
    payload_size: 11
    channels:
      - id: 0x00
        type: 0x00
        name: "Version"
        size: 1
        unit: ""
        value: 2
        position: 0
      - id: 0x01
        type: 0x01
        name: "Battery Voltage"
        size: 2
        unit: "V"
        resolution: 0.01
        range: [2.0, 3.3]
        endian: "big"
        position: 1-2
      - id: 0x02
        type: 0x02
        name: "OpCode"
        size: 1
        unit: ""
        value: 0x05
        position: 3
      - id: 0x03
        type: 0x03
        name: "Measurements Bitmask"
        size: 1
        unit: ""
        description: "Bit 0=Temp, Bit 1=RH, Bit 2=Pressure"
        position: 4
      - id: 0x04
        type: 0x04
        name: "Temperature"
        size: 2
        unit: "°C"
        resolution: 0.1
        range: [-40, 85]
        endian: "big"
        signed: true
        position: 5-6
      - id: 0x05
        type: 0x05
        name: "Relative Humidity"
        size: 1
        unit: "%"
        range: [0, 100]
        position: 7
      - id: 0x06
        type: 0x06
        name: "Pressure"
        size: 2
        unit: "Pa"
        resolution: 10
        range: [30000, 110000]
        endian: "big"
        position: 8-9
      - id: 0x07
        type: 0x07
        name: "Threshold Info"
        size: 1
        unit: ""
        description: "Bit 0-3=Trigger 1-4, Bit 4-7=Stop 1-4"
        position: 10

downlinks:
  - command: "Alert Control"
    hex_prefix: "0250"
    port: 1
    parameters:
      - name: "duration"
        type: "uint8"
        range: [0, 255]
        unit: "minutes"
        description: "0=stop, 1-255=duration"
      - name: "feedbacks"
        type: "uint8"
        optional: true
        description: "Bit 0-3=Feedback 1-4"

  - command: "Device Info Request"
    hex_prefix: "0270"
    port: 1
    parameters: []

  - command: "Configuration Show"
    hex_prefix: "0271"
    port: 1
    parameters:
      - name: "position"
        type: "uint8"
        range: [0, 255]
      - name: "length"
        type: "uint8"
        range: [1, 40]

  - command: "Configuration Update"
    hex_prefix: "0272"
    port: 1
    parameters:
      - name: "position"
        type: "uint8"
        range: [0, 255]
      - name: "length"
        type: "uint8"
        range: [1, 40]
      - name: "content"
        type: "bytes"
        variable_length: true

  - command: "Device Reset"
    hex_prefix: "0290"
    port: 1
    parameters: []

  - command: "Rejoin Network"
    hex_prefix: "0291"
    port: 1
    parameters: []

special_handling:
  crc_required: false
  reset_events: false
  battery_monitoring: true
  signed_values:
    - "temperature"
  
measurement_units:
  temperature: "°C"
  humidity: "%"
  pressure: "Pa"
  voltage: "V"
  
battery_info:
  type: "2x AA Alkaline"
  full_voltage: 3.1
  empty_voltage: 2.4
  life_years: 5

device_variants:
  - model: "MTC-AQ01"
    ip_rating: "IP30"
    description: "Indoor use"
  - model: "MTC-AQ02"
    ip_rating: "IP67"
    description: "Outdoor use"
  - model: "MTC-AQ03"
    ip_rating: "IP54"
    description: "Splash resistant"
