# Dragino SN50v3-LB Protocol MAP
# Generated: 2025-08-20 15:26:42 | PDF Version: Not specified
# Source: https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/SN50v3-LB/

device_info:
  vendor: Dragino
  model: SN50v3-LB
  type: Generic LoRaWAN Sensor Node
  lorawan_version: 1.0.3
  regions: [CN470, EU433, KR920, US915, EU868, AS923, AU915, IN865]
  official_link: https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/SN50v3-LB/

uplinks:
  - port: 5
    type: "device status"
    name: "Device Status"
    size: 7
    channels:
      - name: "Sensor Model"
        size: 1
        unit: "hex"
        fixed_value: 0x1C
        notes: "Always 0x1C for SN50v3-LB"
      - name: "Firmware Version"
        size: 2
        unit: "version"
        encoding: "uint16 BCD"
        notes: "e.g., 0x0100 = v1.0.0"
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

  - port: 2
    type: "MOD=1 default"
    name: "Default Mode"
    size: 11
    channels:
      - name: "Battery Voltage"
        size: 2
        unit: "mV"
        resolution: 1
        range: [2500, 3600]
        encoding: "uint16"
      - name: "DS18B20 Temperature"
        size: 2
        unit: "°C"
        resolution: 0.1
        range: [-40, 85]
        encoding: "signed int16 / 10"
        notes: "Connected to PC13 pin"
      - name: "ADC Value"
        size: 2
        unit: "raw"
        resolution: 1
        range: [0, 4095]
        encoding: "uint16"
        notes: "PA4 pin, 0.1V-1.1V range"
      - name: "Digital Input & Flags"
        size: 1
        unit: "flags"
        encoding: "bitfield"
        notes: "PB15 digital input, PA8 interrupt, working mode"
      - name: "SHT20/SHT31 Temperature"
        size: 2
        unit: "°C"
        resolution: 0.1
        range: [-40, 85]
        encoding: "signed int16 / 10"
        notes: "I2C sensor or BH1750 illumination"
      - name: "SHT20/SHT31 Humidity"
        size: 2
        unit: "%"
        resolution: 0.1
        range: [0, 100]
        encoding: "uint16 / 10"

  - port: 2
    type: "MOD=2 distance"
    name: "Distance Mode"
    size: 11
    channels:
      - name: "Battery Voltage"
        size: 2
        unit: "mV"
        encoding: "uint16"
      - name: "DS18B20 Temperature"
        size: 2
        unit: "°C"
        encoding: "signed int16 / 10"
      - name: "ADC Value"
        size: 2
        unit: "raw"
        encoding: "uint16"
      - name: "Digital Input & Flags"
        size: 1
        unit: "flags"
        encoding: "bitfield"
      - name: "Distance"
        size: 2
        unit: "mm"
        resolution: 1
        range: [240, 6000]
        encoding: "uint16"
        notes: "LIDAR-Lite V3HP or ultrasonic sensor"
      - name: "Reserved"
        size: 2
        unit: "reserved"
        notes: "Unused in distance mode"

  - port: 2
    type: "MOD=3 3ADC+I2C"
    name: "3 ADC + I2C Mode"
    size: 12
    channels:
      - name: "ADC1 Value"
        size: 2
        unit: "raw"
        encoding: "uint16"
        notes: "PA4 pin"
      - name: "ADC2 Value"
        size: 2
        unit: "raw"
        encoding: "uint16"
        notes: "PA5 pin"
      - name: "ADC3 Value"
        size: 2
        unit: "raw"
        encoding: "uint16"
        notes: "PA8 pin"
      - name: "Digital Interrupt"
        size: 1
        unit: "flags"
        encoding: "bitfield"
        notes: "PB15 interrupt status"
      - name: "I2C Temperature"
        size: 2
        unit: "°C"
        encoding: "signed int16 / 10"
      - name: "I2C Humidity"
        size: 2
        unit: "%"
        encoding: "uint16 / 10"
      - name: "Battery Voltage"
        size: 1
        unit: "scaled"
        encoding: "uint8"

  - port: 2
    type: "MOD=4 3DS18B20"
    name: "3 DS18B20 Mode"
    size: 11
    channels:
      - name: "Battery Voltage"
        size: 2
        unit: "mV"
        encoding: "uint16"
      - name: "DS18B20 Temperature 1"
        size: 2
        unit: "°C"
        encoding: "signed int16 / 10"
        notes: "PC13 pin"
      - name: "ADC Value"
        size: 2
        unit: "raw"
        encoding: "uint16"
        notes: "PA4 pin"
      - name: "Digital Input & Flags"
        size: 1
        unit: "flags"
        encoding: "bitfield"
      - name: "DS18B20 Temperature 2"
        size: 2
        unit: "°C"
        encoding: "signed int16 / 10"
        notes: "PB9 pin"
      - name: "DS18B20 Temperature 3"
        size: 2
        unit: "°C"
        encoding: "signed int16 / 10"
        notes: "PB8 pin"

  - port: 2
    type: "MOD=5 weight"
    name: "Weight Measurement (HX711)"
    size: 11
    channels:
      - name: "Battery Voltage"
        size: 2
        unit: "mV"
        encoding: "uint16"
      - name: "DS18B20 Temperature"
        size: 2
        unit: "°C"
        encoding: "signed int16 / 10"
      - name: "ADC Value"
        size: 2
        unit: "raw"
        encoding: "uint16"
      - name: "Digital Input & Flags"
        size: 1
        unit: "flags"
        encoding: "bitfield"
      - name: "Weight"
        size: 4
        unit: "g"
        resolution: 1
        range: [0, 4294967295]
        encoding: "uint32"

  - port: 2
    type: "MOD=6 counting"
    name: "Counting Mode"
    size: 11
    channels:
      - name: "Battery Voltage"
        size: 2
        unit: "mV"
        encoding: "uint16"
      - name: "DS18B20 Temperature"
        size: 2
        unit: "°C"
        encoding: "signed int16 / 10"
      - name: "ADC Value"
        size: 2
        unit: "raw"
        encoding: "uint16"
      - name: "Digital Input"
        size: 1
        unit: "flags"
        encoding: "bitfield"
        notes: "PB15 pin"
      - name: "Count Value"
        size: 4
        unit: "count"
        resolution: 1
        range: [0, 4294967295]
        encoding: "uint32"
        notes: "PA8 interrupt count"

  - port: 2
    type: "MOD=7 3interrupt"
    name: "Three Interrupt Mode"
    size: 9
    channels:
      - name: "Battery Voltage"
        size: 2
        unit: "mV"
        encoding: "uint16"
      - name: "DS18B20 Temperature"
        size: 2
        unit: "°C"
        encoding: "signed int16 / 10"
      - name: "ADC Value"
        size: 2
        unit: "raw"
        encoding: "uint16"
        notes: "PA5 pin"
      - name: "Digital Interrupt 1"
        size: 1
        unit: "flags"
        encoding: "bitfield"
        notes: "PA8 pin"
      - name: "Digital Interrupt 2"
        size: 1
        unit: "flags"
        encoding: "bitfield"
        notes: "PA4 pin"
      - name: "Digital Interrupt 3"
        size: 1
        unit: "flags"
        encoding: "bitfield"
        notes: "PB15 pin"

  - port: 2
    type: "MOD=8 3ADC+DS18B20"
    name: "3 ADC + 1 DS18B20 Mode"
    size: 11
    channels:
      - name: "Battery Voltage"
        size: 2
        unit: "mV"
        encoding: "uint16"
      - name: "DS18B20 Temperature"
        size: 2
        unit: "°C"
        encoding: "signed int16 / 10"
      - name: "ADC1 Value"
        size: 2
        unit: "raw"
        encoding: "uint16"
        notes: "PA4 pin"
      - name: "Digital Interrupt"
        size: 1
        unit: "flags"
        encoding: "bitfield"
        notes: "PB15 pin"
      - name: "ADC2 Value"
        size: 2
        unit: "raw"
        encoding: "uint16"
        notes: "PA5 pin"
      - name: "ADC3 Value"
        size: 2
        unit: "raw"
        encoding: "uint16"
        notes: "PA8 pin"

  - port: 2
    type: "MOD=9 3DS18B20+2count"
    name: "3 DS18B20 + 2 Count Mode"
    size: 17
    channels:
      - name: "Battery Voltage"
        size: 2
        unit: "mV"
        encoding: "uint16"
      - name: "DS18B20 Temperature 1"
        size: 2
        unit: "°C"
        encoding: "signed int16 / 10"
      - name: "DS18B20 Temperature 2"
        size: 2
        unit: "°C"
        encoding: "signed int16 / 10"
      - name: "Digital Interrupt"
        size: 1
        unit: "flags"
        encoding: "bitfield"
      - name: "DS18B20 Temperature 3"
        size: 2
        unit: "°C"
        encoding: "signed int16 / 10"
      - name: "Count 1"
        size: 4
        unit: "count"
        encoding: "uint32"
        notes: "PA8 pin"
      - name: "Count 2"
        size: 4
        unit: "count"
        encoding: "uint32"
        notes: "PA4 pin"

  - port: 2
    type: "MOD=10 PWM"
    name: "PWM Input/Output Mode"
    size: 11
    channels:
      - name: "Battery Voltage"
        size: 2
        unit: "mV"
        encoding: "uint16"
      - name: "DS18B20 Temperature"
        size: 2
        unit: "°C"
        encoding: "signed int16 / 10"
      - name: "ADC Value"
        size: 2
        unit: "raw"
        encoding: "uint16"
      - name: "PWM Settings"
        size: 1
        unit: "flags"
        encoding: "bitfield"
      - name: "Pulse Period"
        size: 2
        unit: "us/ms"
        encoding: "uint16"
        notes: "Depends on PWMSET setting"
      - name: "High Level Duration"
        size: 2
        unit: "us/ms"
        encoding: "uint16"
        notes: "For duty cycle calculation"

  - port: 2
    type: "MOD=11 TEMP117"
    name: "TEMP117 Mode"
    size: 11
    channels:
      - name: "Battery Voltage"
        size: 2
        unit: "mV"
        encoding: "uint16"
      - name: "DS18B20 Temperature"
        size: 2
        unit: "°C"
        encoding: "signed int16 / 10"
      - name: "ADC Value"
        size: 2
        unit: "raw"
        encoding: "uint16"
      - name: "Digital Input & Flags"
        size: 1
        unit: "flags"
        encoding: "bitfield"
      - name: "TEMP117 Temperature"
        size: 2
        unit: "°C"
        encoding: "high precision"
        notes: "I2C TEMP117 sensor"
      - name: "Reserved"
        size: 2
        unit: "reserved"

  - port: 2
    type: "MOD=12 count+SHT31"
    name: "Count + SHT31 Mode"
    size: 11
    channels:
      - name: "Battery Voltage"
        size: 2
        unit: "mV"
        encoding: "uint16"
      - name: "SHT31 Temperature"
        size: 2
        unit: "°C"
        encoding: "signed int16 / 10"
      - name: "SHT31 Humidity"
        size: 2
        unit: "%"
        encoding: "uint16 / 10"
      - name: "Digital Input"
        size: 1
        unit: "flags"
        encoding: "bitfield"
        notes: "PB15 pin"
      - name: "Count Value"
        size: 4
        unit: "count"
        encoding: "uint32"
        notes: "PA8 pin"

downlinks:
  - command: "Set Transmit Interval"
    hex_prefix: "01"
    parameters:
      - name: "interval_seconds"
        type: "uint24"
        range: [30, 16777215]
        endian: "big"
    example: "0100003C = 60 seconds"

  - command: "Request Device Status"
    hex_prefix: "26"
    parameters:
      - name: "status_request"
        type: "uint8"
        fixed_value: 0x01
    example: "2601 = request status (fport=5)"

  - command: "Set Interrupt Mode (Pre v1.3.4)"
    hex_prefix: "0600"
    parameters:
      - name: "pin_select"
        type: "uint8"
        values:
          0x00: "PA8 pin"
          0x01: "PA4 pin" 
          0x02: "PB15 pin"
      - name: "interrupt_mode"
        type: "uint8"
        values:
          0x00: "Disable"
          0x01: "Rising and falling edge"
          0x02: "Falling edge"
          0x03: "Rising edge"
    example: "06000103 = PA4 pin rising+falling edge"

  - command: "Set Interrupt Mode (Since v1.3.4)"
    hex_prefix: "0600"
    parameters:
      - name: "pin_select"
        type: "uint8"
        values:
          0x00: "PA8 pin"
          0x01: "PA4 pin"
          0x02: "PB15 pin"
      - name: "interrupt_mode"
        type: "uint8"
        values:
          0x00: "Disable"
          0x01: "Rising and falling edge"
          0x02: "Falling edge"
          0x03: "Rising edge"
      - name: "delay_time"
        type: "uint16"
        range: [0, 65535]
        unit: "ms"
        endian: "big"
    example: "060001030BB8 = PA4 falling edge with 3000ms delay"

  - command: "Set 5V Output Duration"
    hex_prefix: "07"
    parameters:
      - name: "duration_ms"
        type: "uint16"
        range: [0, 65535]
        endian: "big"
    example: "0701F4 = 500ms duration"

  - command: "Set Weight Parameters"
    hex_prefix: "08"
    parameters:
      - name: "command_type"
        type: "uint8"
        values:
          0x01: "Zero calibration (WEIGRE)"
          0x02: "Factor calibration (WEIGAP)"
      - name: "factor_value"
        type: "uint24"
        conditional: "command_type == 0x02"
        notes: "Value * 10 for decimal factor"
    example: "0801 = zero calibration, 08020FA0 = factor 400.0"

  - command: "Set Count Value"
    hex_prefix: "09"
    parameters:
      - name: "count_select"
        type: "uint8"
        values:
          0x01: "Count 1 (PA8)"
          0x02: "Count 2 (PA4)"
      - name: "count_value"
        type: "uint32"
        range: [0, 4294967295]
        endian: "big"
    example: "090100000064 = set count 1 to 100"

  - command: "Set Working Mode"
    hex_prefix: "0A"
    parameters:
      - name: "mode"
        type: "uint8"
        values:
          0x01: "Default mode"
          0x02: "Distance mode"
          0x03: "3 ADC + I2C"
          0x04: "3 DS18B20"
          0x05: "Weight (HX711)"
          0x06: "Counting mode"
          0x07: "Three interrupt"
          0x08: "3 ADC + DS18B20"
          0x09: "3 DS18B20 + 2 count"
          0x0A: "PWM mode"
          0x0B: "TEMP117 mode"
          0x0C: "Count + SHT31"
    example: "0A04 = set to 3 DS18B20 mode"

  - command: "Set PWM Output"
    hex_prefix: "0B"
    parameters:
      - name: "frequency_hz"
        type: "uint24"
        range: [5, 100000]
        endian: "big"
      - name: "duty_cycle"
        type: "uint8"
        range: [0, 100]
        unit: "%"
      - name: "output_time"
        type: "uint16"
        range: [0, 65535]
        unit: "ms"
        endian: "big"
    example: "0B0003E8320005 = 1000Hz, 50% duty, 5ms output"

  - command: "Set PWM Setting"
    hex_prefix: "0C"
    parameters:
      - name: "time_unit"
        type: "uint8"
        values:
          0x00: "Microseconds (20Hz-100kHz)"
          0x01: "Milliseconds (5Hz-250Hz)"
    example: "0C00 = microsecond timing"

special_handling:
  crc_required: false
  reset_events: false
  battery_monitoring: true
  signed_values:
    - "ds18b20_temperature"
    - "sht20_temperature"
    - "sht31_temperature"
    - "temp117_temperature"
  
measurement_units:
  battery_voltage: "mV"
  ds18b20_temperature: "°C"
  sht20_temperature: "°C"
  sht31_temperature: "°C"
  temp117_temperature: "°C"
  sht20_humidity: "%"
  sht31_humidity: "%"
  adc_value: "raw"
  distance: "mm"
  weight: "g"
  count_value: "count"
  pulse_period: "us/ms"
  high_level_duration: "us/ms"
  frequency_band: "band"
  firmware_version: "version"
