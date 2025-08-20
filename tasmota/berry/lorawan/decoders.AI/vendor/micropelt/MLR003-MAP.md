# Micropelt MLR003 Protocol MAP
# Generated: 2025-08-20 20:50:35 | Framework Version: 2.3.3
# Source: TTN Device Repository (GitHub)

device_info:
  vendor: Micropelt
  model: MLR003
  type: Thermostatic Radiator Valve
  lorawan_version: 1.0.3
  regions: [EU863-870]
  official_link: https://micropelt.atlassian.net/wiki/spaces/MH/pages/19300575/MLR003RiEU61-07+Title+Page

uplinks:
  - port: 1
    name: "Device Data"
    payload_size: 11-12
    channels:
      - name: "Current Valve Position"
        size: 1
        unit: "%"
        range: [0, 100]
        position: 0
      - name: "Flow Temperature"
        size: 1
        unit: "°C"
        resolution: 0.5
        position: 2
      - name: "Ambient Temperature"
        size: 1
        unit: "°C"
        resolution: 0.25
        position: 4
      - name: "Storage Voltage"
        size: 1
        unit: "V"
        resolution: 0.02
        position: 6
      - name: "Energy Flags"
        size: 1
        bit_flags: true
        position: 5
      - name: "Current Consumed"
        size: 1
        unit: "µA"
        resolution: 10
        position: 7
      - name: "Current Generated"
        size: 1
        unit: "µA"
        resolution: 10
        position: 8
      - name: "User Mode"
        size: 1
        bit_field: [0, 2]
        position: 9
      - name: "User Value"
        size: 1
        variable_unit: true
        position: 10
      - name: "Used Temperature"
        size: 1
        unit: "°C"
        resolution: 0.25
        position: 11
        optional: true

  - port: 2
    name: "Device Version"
    payload_size: 6
    channels:
      - name: "REV"
        size: 1
        format: "version"
        position: 0
      - name: "Hardware Version"
        size: 1
        format: "version"
        position: 1
      - name: "Firmware Version"
        size: 4
        format: "date_version"
        position: 2-5

  - port: 3
    name: "Motor Range"
    payload_size: 1
    channels:
      - name: "Motor Range"
        size: 1
        unit: "mm"
        lookup_table: true
        position: 0

  - port: 4
    name: "Spreading Factor"
    payload_size: 1
    channels:
      - name: "Spreading Factor"
        size: 1
        enum: ["SF7", "SF8"]
        position: 0

  - port: 5
    name: "Opening Point Detection"
    payload_size: 2
    channels:
      - name: "Opening Percent"
        size: 1
        bit_field: [0, 6]
        unit: "%"
        position: 0
      - name: "OPD Status"
        size: 1
        bit_field: [0, 2]
        enum_values: true
        position: 1

  - port: 6
    name: "Temperature Drop Detection"
    payload_size: 1
    channels:
      - name: "TDD Configuration"
        size: 1
        bit_flags: true
        position: 0

  - port: 7
    name: "PID Parameters"
    payload_size: 7
    channels:
      - name: "P Coefficient"
        size: 1
        range: [0, 255]
        position: 0
      - name: "I Coefficient"
        size: 1
        resolution: 0.02
        position: 1
      - name: "D Coefficient"
        size: 1
        resolution: 0.2
        position: 2
      - name: "Closed Percent"
        size: 1
        unit: "%"
        position: 4
      - name: "D Coefficient Closed"
        size: 1
        resolution: 0.2
        position: 5
      - name: "Offset Percent"
        size: 1
        unit: "%"
        position: 6

  - port: 8
    name: "Flow Offset"
    payload_size: 1
    channels:
      - name: "Flow Offset"
        size: 1
        unit: "°C"
        resolution: 0.25
        signed: true
        position: 0

  - port: 9
    name: "External Temperature Sensor"
    payload_size: 2
    channels:
      - name: "Sensor Expiry"
        size: 1
        unit: "minutes"
        resolution: 5
        position: 0
      - name: "Temperature Source"
        size: 1
        enum: ["Internal", "External"]
        position: 1

  - port: 15
    name: "Device Status"
    payload_size: 1
    channels:
      - name: "Device Status"
        size: 1
        bit_flags: true
        position: 0

downlinks:
  - port: 1
    command: "Main Configuration"
    parameters:
      - name: "userMode"
        type: "enum"
        values: ["Ambient_Temperature", "Valve_Position"]
      - name: "setValue"
        type: "variable"
        range_temp: [0, 40]
        range_valve: [0, 100]
      - name: "roomTemperature"
        type: "uint8"
        range: [0, 40]
      - name: "safetyMode"
        type: "enum"
        values: ["Ambient_Temperature", "Valve_Position"]
      - name: "safetyValue"
        type: "variable"
        range_temp: [0, 40]
        range_valve: [0, 100]
      - name: "radioInterval"
        type: "enum"
        values: [5, 10, 60, 120, 480]
        unit: "minutes"
      - name: "doReferenceRunNow"
        type: "boolean"

  - port: 3
    command: "Motor Range"
    parameters:
      - name: "motor_operating_range"
        type: "enum"
        values: ["0.624", "0.832", "1.040", "1.248", "1.456", "1.664", "1.872", "2.080", "2.288", "2.496", "2.56048"]
        unit: "mm"

  - port: 4
    command: "Spreading Factor"
    parameters:
      - name: "spreading_factor"
        type: "enum"
        values: ["SF7", "SF8"]

  - port: 5
    command: "Opening Point Detection"
    parameters:
      - name: "opening_point_reset"
        type: "boolean"
      - name: "hot_water_availability"
        type: "enum"
        values: ["Use_time_of_year", "OFF", "ON"]
      - name: "slow_harvesting"
        type: "enum"
        values: ["DEFAULT", "DO_OPD_AND_SH", "DO_OPD_ONLY", "DISABLE_OPD_AND_SH"]
      - name: "max_flow_sensor_raw"
        type: "uint8"
        range: [0, 33]

  - port: 6
    command: "Temperature Drop Detection"
    parameters:
      - name: "tdd_action"
        type: "enum"
        values: ["No_Action", "Close_to_0%_for_30_minutes", "Close_to_0%_for_60_minutes"]
      - name: "tdd_beep"
        type: "boolean"
      - name: "tdd_period"
        type: "enum"
        values: [1, 2]

  - port: 7
    command: "PID Parameters"
    parameters:
      - name: "kP"
        type: "uint8"
        range: [0, 255]
      - name: "kI"
        type: "float"
        range: [0, 5.10]
        resolution: 0.02
      - name: "kD"
        type: "float"
        range: [0, 51]
        resolution: 0.2
      - name: "Closed_Percent"
        type: "uint8"
        range: [0, 100]
      - name: "kD_when_closed"
        type: "float"
        range: [0, 51]
        resolution: 0.2
      - name: "Offset_Percent"
        type: "uint8"
        range: [0, 100]

  - port: 8
    command: "Flow Offset"
    parameters:
      - name: "Flow_Raw_Value_Offset"
        type: "float"
        range: [-32.0, 31.75]
        resolution: 0.25

  - port: 9
    command: "External Temperature Sensor"
    parameters:
      - name: "External_temperature_sensor_expiry_minutes"
        type: "uint16"
        range: [0, 1275]
        resolution: 5

  - port: 10
    command: "Room Temperature"
    parameters:
      - name: "Room_Temperature"
        type: "uint8"
        range: [0, 40]

  - port: 11
    command: "Beep"
    parameters:
      - name: "Beep"
        type: "uint8"
        range: [0, 255]

  - port: 15
    command: "Device Control"
    parameters:
      - name: "device_will_operate_if_6_week_reference_run_fails"
        type: "boolean"
      - name: "do_recalibation_now"
        type: "boolean"
      - name: "turn_off_device"
        type: "boolean"

special_handling:
  energy_harvesting: true
  battery_free: true
  variable_payload_size: true
  complex_user_modes: true
  pid_controller: true
  
measurement_units:
  temperature: "°C"
  voltage: "V"
  current: "µA"
  position: "%"
  range: "mm"
  time: "minutes"

device_features:
  energy_harvesting: "Thermoelectric generator (TEG)"
  battery_type: "LTO (maintenance-free)"
  operating_temp: "0-40°C"
  ip_rating: "IP4X"
  weight: "280g"
  valve_thread: "M30x1.5"
  lorawan_class: "A"
  spreading_factor: "SF7BW125"
