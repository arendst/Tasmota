# Watteco Bob Assistant Protocol MAP
# Generated: 2025-08-20 21:10:15 | Framework Version: 2.3.3
# Source: TTN Device Repository (GitHub)

device_info:
  vendor: Watteco
  model: Bob Assistant
  type: Vibration Sensor
  lorawan_version: 1.0.2
  regions: [EU863-870]
  official_link: https://www.watteco.com/products/bob-assistant-and-movee-solutions/

uplinks:
  - port: 1
    type: 0x72
    name: "Report Frame"
    payload_size: 27
    channels:
      - name: "Battery Percentage"
        size: 1
        unit: "%"
        resolution: 0.787
        position: 17
      - name: "Anomaly Level"
        size: 1
        unit: "%"
        resolution: 0.787
        position: 1
      - name: "Temperature"
        size: 1
        unit: "°C"
        offset: -30
        position: 5
      - name: "Vibration Level"
        size: 3
        unit: "g"
        complex_formula: true
        position: 8-10
      - name: "Peak Frequency Index"
        size: 1
        range: [1, 32]
        position: 11
      - name: "Operating Time"
        size: 1
        unit: "hours"
        formula: "bytes[2]*2/127"
        position: 2
      - name: "Report Period"
        size: 1
        unit: "minutes"
        complex_calc: true
        position: 6
      - name: "Alarm Count"
        size: 1
        unit: "count"
        position: 4

  - port: 1
    type: 0x61
    name: "Alarm Frame"
    payload_size: 40
    channels:
      - name: "Temperature"
        size: 1
        unit: "°C"
        offset: -30
        position: 2
      - name: "Vibration Level"
        size: 3
        unit: "g"
        complex_formula: true
        position: 4-6
      - name: "Anomaly Level"
        size: 1
        unit: "%"
        resolution: 0.787
        position: 1
      - name: "FFT Data"
        size: 32
        array: true
        count: 32
        position: 8-39

  - port: 1
    type: 0x6C
    name: "Learning Frame"
    payload_size: 40
    channels:
      - name: "Temperature"
        size: 1
        unit: "°C"
        offset: -30
        position: 6
      - name: "Learning Percentage"
        size: 1
        unit: "%"
        position: 1
      - name: "Learning From Scratch"
        size: 1
        boolean: true
        position: 7
      - name: "Vibration Level"
        size: 3
        unit: "g"
        complex_formula: true
        position: 2-4
      - name: "Peak Frequency Index"
        size: 1
        range: [1, 32]
        position: 5
      - name: "Peak Frequency"
        size: 1
        unit: "Hz"
        formula: "(bytes[5]+1)*800/256"
        position: 5
      - name: "FFT Data"
        size: 32
        array: true
        count: 32
        position: 8-39

  - port: 1
    type: 0x53
    name: "State Frame"
    payload_size: 3
    channels:
      - name: "State"
        size: 1
        enum: true
        values:
          100: "Sensor start"
          101: "Sensor stop"
          125: "Machine stop"
          126: "Machine start"
        position: 1
      - name: "Battery Percentage"
        size: 1
        unit: "%"
        resolution: 0.787
        position: 2

downlinks: []

special_handling:
  complex_vibration_calc: true
  fft_analysis: true
  time_stamped_data: true
  machine_learning: true
  anomaly_detection: true
  
measurement_units:
  temperature: "°C"
  vibration: "g"
  frequency: "Hz"
  percentage: "%"
  time: "hours"
  anomaly: "%"

device_features:
  vibration_sensor: "3-axis accelerometer"
  temperature_sensor: "Built-in"
  battery_type: "AA replaceable"
  operating_temp: "-20 to 60°C"
  ip_rating: "IP68"
  weight: "75g"
  dimensions: "79x76x23mm"
  lorawan_class: "A"
  predictive_maintenance: true
  rgb_led: true
  push_button: true
