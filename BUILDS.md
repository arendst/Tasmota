## Available Features and Sensors

ESP8266 / ESP32

l = lite, t = tasmota, k = knx, s = sensors, i = ir, d = display

Note: `minimal` variant is not listed as it shouldn't be used outside of the [upgrading](https://tasmota.github.io/docs/Upgrading/) process.

| Feature or Sensor     | l | t     | k | s | i | d | Remarks
|-----------------------|---|-------|---|---|---|---|--------
| MY_LANGUAGE en_GB     | x | x / x | x | x | x | x |
| USE_UFILESYS          | - | - / x | - | - | - | - |
| USE_ARDUINO_OTA       | - | - / - | - | - | - | - |
| USE_DOMOTICZ          | - | x / x | x | x | x | - |
| USE_HOME_ASSISTANT    | - | x / x | x | x | x | - |
| USE_MQTT_TLS          | - | - / - | - | - | - | - |
| USE_MQTT_AWS_IOT      | - | - / - | - | - | - | - |
| USE_4K_RSA            | - | - / - | - | - | - | - |
| USE_TELEGRAM          | - | - / - | - | - | - | - |
| USE_KNX               | - | - / x | x | - | - | - |
| USE_WEBSERVER         | x | x / x | x | x | x | x |
| USE_JAVASCRIPT_ES6    | - | - / - | - | - | - | - |
| USE_WEBSEND_RESPONSE  | - | - / - | - | - | - | - |
| USE_EMULATION_HUE     | x | x / x | - | x | - | - |
| USE_EMULATION_WEMO    | x | x / x | - | x | - | - |
| USE_DISCOVERY         | - | - / - | - | - | - | - |
| WEBSERVER_ADVERTISE   | - | x / - | x | - | - | x |
| MQTT_HOST_DISCOVERY   | - | - / - | - | - | - | - |
| USE_TIMERS            | x | x / x | x | x | x | x |
| USE_TIMERS_WEB        | x | x / x | x | x | x | x |
| USE_SUNRISE           | x | x / x | x | x | x | x |
| USE_RULES             | x | x / x | x | x | x | x |
| USE_SCRIPT            | - | - / - | - | - | - | - |
| USE_EXPRESSION        | - | - / - | - | - | - | - |
| SUPPORT_IF_STATEMENT  | - | - / - | - | - | - | - |
| USE_HOTPLUG           | - | - / - | - | - | - | - |
| USE_PROMETHEUS        | - | - / - | - | - | - | - |
| USE_PING              | - | - / - | - | - | - | - |
|                       |   |       |   |   |   |   |
| Feature or Sensor     | l | t     | k | s | i | d | Remarks
| ROTARY_V1             | - | x / x | - | x | - | - |
| USE_SONOFF_RF         | - | x / - | x | x | - | - |
| USE_RF_FLASH          | - | x / - | x | x | - | - |
| USE_SONOFF_SC         | - | x / - | x | x | - | - |
| USE_TUYA_MCU          | x | x / - | x | x | - | x |
| USE_ARMTRONIX_DIMMERS | - | x / - | x | - | - | - |
| USE_PS_16_DZ          | - | x / - | x | - | - | - |
| USE_SONOFF_IFAN       | - | x / - | x | - | - | - |
| USE_BUZZER            | - | x / x | x | x | - | - |
| USE_ARILUX_RF         | - | x / - | x | - | - | - |
| USE_SHUTTER           | - | x / x | x | - | - | - |
| USE_DEEPSLEEP         | - | x / x | - | x | - | - |
| USE_EXS_DIMMER        | - | x / - | x | - | - | - |
| USE_DEVICE_GROUPS     | - | x / x | - | - | - | - |
| USE_PWM_DIMMER        | - | x / - | x | - | - | - |
| USE_KEELOQ            | - | - / - | - | - | - | - |
| USE_SONOFF_D1         | - | x / - | x | - | - | - |
| USE_SHELLY_DIMMER     | - | x / - | - | - | - | - |
| USE_AC_ZERO_CROSS_DIMMER | - | x / x | x | x | x | x |
|                       |   |       |   |   |   |   |
| Feature or Sensor     | l | t     | k | s | i | d | Remarks
| USE_LIGHT             | x | x / x | x | x | x | x |
| USE_WS2812            | - | x / x | x | x | - | x |
| USE_WS2812_DMA        | - | - / - | - | - | - | - |
| USE_MY92X1            | - | x / - | x | x | - | x |
| USE_SM16716           | - | x / - | x | x | - | x |
| USE_SM2135            | - | x / - | x | x | - | x |
| USE_SONOFF_L1         | - | x / - | x | x | - | x |
| USE_ELECTRIQ_MOODL    | - | x / - | x | x | - | x |
|                       |   |       |   |   |   |   |
| USE_ENERGY_SENSOR     | x | x / x | x | x | - | - |
| USE_ENERGY_DUMMY      | x | x / x | x | x | - | - |
| USE_PZEM004T          | - | x / x | x | x | - | - |
| USE_PZEM_AC           | - | x / x | x | x | - | - |
| USE_PZEM_DC           | - | x / x | x | x | - | - |
| USE_MCP39F501         | x | x / - | x | x | - | - |
| USE_SDM72             | - | - / x | - | x | - | - |
| USE_SDM120            | - | - / x | - | x | - | - |
| USE_SDM630            | - | - / x | - | x | - | - |
| USE_DDS2382           | - | - / x | - | x | - | - |
| USE_DDSU666           | - | - / x | - | x | - | - |
| USE_SOLAX_X1          | - | - / - | - | - | - | - |
| USE_LE01MR            | - | - / - | - | - | - | - |
| USE_BL09XX            | x | x / x | x | x | - | - |
| USE_TELEINFO          | - | - / - | - | - | - | - |
| USE_IEM3000           | - | - / - | - | - | - | - |
| USE_WE517             | - | - / - | - | - | - | - |
|                       |   |       |   |   |   |   |
| USE_ADC_VCC           | x | - / - | - | - | x | - |
| USE_COUNTER           | - | x / x | x | x | - | x |
| USE_DS18x20           | - | x / x | x | x | - | x |
| USE_DHT               | - | x / x | x | x | - | x |
| USE_MAX31855          | - | - / x | - | x | - | - |
| USE_MAX31865          | - | - / - | - | - | - | - |
| USE_THERMOSTAT        | - | - / - | - | - | - | - |
| USE_LMT01             | - | - / x | - | x | - | - |
|                       |   |       |   |   |   |   |
| Feature or Sensor     | l | t     | k | s | i | d | Remarks
| USE_I2C               | - | x / x | x | x | - | x |
| USE_SHT               | - | - / x | - | x | - | - |
| USE_HTU               | - | - / x | - | x | - | - |
| USE_BMP               | - | - / x | - | x | - | - |
| USE_BME680            | - | - / x | - | x | - | - |
| USE_BH1750            | - | - / x | - | x | - | - |
| USE_VEML6070          | - | - / x | - | x | - | - |
| USE_ADS1115           | - | - / x | - | x | - | - |
| USE_INA219            | - | - / x | - | x | - | - |
| USE_INA226            | - | - / - | - | - | - | - |
| USE_SHT3X             | - | - / x | - | x | - | - |
| USE_TSL2561           | - | - / - | - | - | - | - |
| USE_TSL2591           | - | - / - | - | - | - | - |
| USE_MGS               | - | - / x | - | x | - | - |
| USE_SGP30             | - | - / x | - | x | - | - |
| USE_SI1145            | - | - / - | - | - | - | - |
| USE_LM75AD            | - | - / x | - | x | - | - |
| USE_APDS9960          | - | - / - | - | - | - | - |
| USE_MCP230xx          | - | - / - | - | - | - | - |
| USE_PCA9685           | - | - / - | - | - | - | - |
| USE_MPR121            | - | - / - | - | - | - | - |
| USE_CCS811            | - | - / - | - | x | - | - |
| USE_CCS811_V2         | - | - / x | - | - | - | - |
| USE_MPU6050           | - | - / - | - | - | - | - |
| USE_DS3231            | - | - / - | - | - | - | - |
| USE_MGC3130           | - | - / - | - | - | - | - |
| USE_MAX44009          | - | - / - | - | - | - | - |
| USE_SCD30             | - | - / x | - | x | - | - |
| USE_SCD40             | - | - / x | - | - | - | - |
| USE_SPS30             | - | - / - | - | - | - | - |
| USE_ADE7953           | - | x / x | x | x | - | x |
| USE_VL53L0X           | - | - / x | - | x | - | - |
| USE_MLX90614          | - | - / - | - | - | - | - |
| USE_CHIRP             | - | - / - | - | - | - | - |
| USE_PAJ7620           | - | - / - | - | - | - | - |
| USE_PCF8574           | - | - / - | - | - | - | - |
|                       |   |       |   |   |   |   |
| Feature or Sensor     | l | t     | k | s | i | d | Remarks
| USE_HIH6              | - | - / x | - | x | - | - |
| USE_DHT12             | - | - / x | - | x | - | - |
| USE_DS1624            | - | - / x | - | x | - | - |
| USE_AHT1x             | - | - / - | - | - | - | - |
| USE_HDC1080           | - | - / - | - | - | - | - |
| USE_WEMOS_MOTOR_V1    | - | - / x | - | x | - | - |
| USE_IAQ               | - | - / x | - | x | - | - |
| USE_AS3935            | - | - / x | - | x | - | - |
| USE_VEML6075          | - | - / - | - | - | - | - |
| USE_VEML7700          | - | - / - | - | - | - | - |
| USE_MCP9808           | - | - / - | - | - | - | - |
| USE_HP303B            | - | - / - | - | - | - | - |
| USE_EZOCO2            | - | - / - | - | - | - | - |
| USE_EZODO             | - | - / - | - | - | - | - |
| USE_EZOEC             | - | - / - | - | - | - | - |
| USE_EZOFLO            | - | - / - | - | - | - | - |
| USE_EZOHUM            | - | - / - | - | - | - | - |
| USE_EZOO2             | - | - / - | - | - | - | - |
| USE_EZOORP            | - | - / - | - | - | - | - |
| USE_EZOPH             | - | - / - | - | - | - | - |
| USE_EZOPMP            | - | - / - | - | - | - | - |
| USE_EZOPRS            | - | - / - | - | - | - | - |
| USE_EZORGB            | - | - / - | - | - | - | - |
| USE_EZORTD            | - | - / - | - | - | - | - |
| USE_SEESAW_SOIL       | - | - / - | - | - | - | - |
| USE_TOF10120          | - | - / - | - | - | - | - |
| USE_AM2320            | - | - / - | - | - | - | - |
| USE_T67XX             | - | - / - | - | - | - | - |
| USE_HM330X            | - | - / - | - | - | - | - |
|                       |   |       |   |   |   |   |
| Feature or Sensor     | l | t     | k | s | i | d | Remarks
| USE_SPI               | - | - / - | - | - | - | x |
| USE_RC522             | - | - / - | - | - | - | - |
| USE_MHZ19             | - | - / x | - | x | - | - |
| USE_SENSEAIR          | - | - / x | - | x | - | - |
| USE_PMS5003           | - | - / x | - | x | - | - |
| USE_NOVA_SDS          | - | - / x | - | x | - | - |
| USE_HPMA              | - | - / x | - | x | - | - |
| USE_SERIAL_BRIDGE     | - | x / x | x | x | - | x |
| USE_MP3_PLAYER        | - | - / x | - | x | - | - |
| USE_AZ7798            | - | - / - | - | - | - | - |
| USE_PN532_HSU         | - | - / x | - | x | - | - |
| USE_RDM6300           | - | - / x | - | x | - | - |
| USE_IBEACON           | - | - / x | - | x | - | - |
| USE_GPS               | - | - / - | - | - | - | - |
| USE_HM10              | - | - / - | - | x | - | - |
| USE_HRXL              | - | - / x | - | x | - | - |
| USE_TASMOTA_CLIENT    | - | - / - | - | - | - | - |
| USE_OPENTHERM         | - | - / - | - | - | - | - |
| USE_MIEL_HVAC         | - | - / - | - | - | - | - |
| USE_PROJECTOR_CTRL    | - | - / - | - | - | - | - |
| USE_AS608             | - | - / - | - | - | - | - |
| USE_TCP_BRIDGE        | - | - / - | - | - | - | - | zbbridge
|                       |   |       |   |   |   |   |
| USE_NRF24             | - | - / - | - | - | - | - |
| USE_MIBLE             | - | - / - | - | - | - | - |
| USE_ZIGBEE            | - | - / - | - | - | - | - |
| USE_ZIGBEE_ZNP        | - | - / - | - | - | - | - |
| USE_ZIGBEE_EZSP       | - | - / - | - | - | - | - | Sonoff ZbBridge
|                       |   |       |   |   |   |   |
| USE_IR_REMOTE         | - | x / - | x | x | x | x |
| USE_IR_RECEIVE        | - | x / - | x | x | x | x |
| USE_IR_REMOTE_FULL    | - | - / - | - | - | x | - | Enable ALL protocols
|                       |   |       |   |   |   |   |
| USE_SR04              | - | - / - | - | x | - | - |
| USE_DYP               | - | - / - | - | - | - | - |
| USE_TM1638            | - | - / x | - | x | - | - |
| USE_HX711             | - | - / x | - | x | - | - |
| USE_TX2x_WIND_SENSOR  | - | - / - | - | - | - | - |
| USE_WINDMETER         | - | - / - | - | - | - | - |
| USE_RC_SWITCH         | - | - / x | - | x | - | - |
| USE_RF_SENSOR         | - | - / x | - | x | - | - | AlectoV2 only
| USE_HRE               | - | - / x | - | x | - | - |
| USE_A4988_STEPPER     | - | - / - | - | - | - | - |
| USE_NEOPOOL           | - | - / - | - | - | - | - |
|                       |   |       |   |   |   |   |
| Feature or Sensor     | l | t     | k | s | i | d | Remarks
| USE_DISPLAY           | - | - / - | - | - | - | x |
| USE_DISPLAY_LCD       | - | - / - | - | - | - | x |
| USE_DISPLAY_SSD1306   | - | - / - | - | - | - | x |
| USE_DISPLAY_MATRIX    | - | - / - | - | - | - | x |
| USE_DISPLAY_SH1106    | - | - / - | - | - | - | x |
| USE_DISPLAY_ILI9341   | - | - / - | - | - | - | x |
| USE_DISPLAY_EPAPER_29 | - | - / - | - | - | - | x |
| USE_DISPLAY_EPAPER_42 | - | - / - | - | - | - | x |
| USE_DISPLAY_SSD1351   | - | - / - | - | - | - | x |
| USE_DISPLAY_RA8876    | - | - / - | - | - | - | x |
| USE_DISPLAY_ST7789    | - | - / - | - | - | - | x |
| USE_DISPLAY_TM1637    | - | - / - | - | - | - | x |
|                       |   |       |   |   |   |   |
| USE_FT5206            | - | - / - | - | - | - | - |
| USE_FTC532            | - | - / - | - | - | - | - |
| USE_BS814A2           | - | - / - | - | - | - | - |
|                       |   |       |   |   |   |   |
| ESP32 Feature         | l | t     | k | s | i | d | Remarks
| USE_HALLEFFECT        |   |   / x |   |   |   |   |
| USE_MI_ESP32          |   |   / x |   |   |   |   | See SetOption115
| USE_IBEACON_ESP32     |   |   / - |   |   |   |   |
| USE_WEBCAM            |   |   / - |   |   |   |   |
| USE_ETHERNET          |   |   / - |   |   |   |   |
| USE_I2S_AUDIO         |   |   / - |   |   |   |   |
| USE_TTGO_WATCH        |   |   / - |   |   |   |   |
