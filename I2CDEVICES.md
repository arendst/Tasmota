<img src="/tools/logo/TASMOTA_FullLogo_Vector.svg" alt="Logo" align="right" height="76"/>

# Tasmota I2C devices

Tasmota supports several I2C devices but to use them they mostly need to be enabled at compile time to solve possible address conflicts.

## Supported I2C devices
The following table lists the supported I2C devices

Index | Define              | Driver  | Device   | Address(es) | Description
------|---------------------|---------|----------|-------------|-----------------------------------------------
   1  | USE_PCA9685         | xdrv_15 | PCA9685  | 0x40 - 0x47 | 16-channel 12-bit pwm driver
   2  | USE_PCF8574         | xdrv_28 | PCF8574  | 0x20 - 0x27 | 8-bit I/O expander
   2  | USE_PCF8574         | xdrv_28 | PCF8574A | 0x38 - 0x3F | 8-bit I/O expander
   3  | USE_DISPLAY_LCD     | xdsp_01 |          | 0x27, 0x3F  | LCD display
   4  | USE_DISPLAY_SSD1306 | xdsp_02 | SSD1306  | 0x3C - 0x3D | Oled display
   5  | USE_DISPLAY_MATRIX  | xdsp_03 | HT16K33  | 0x70 - 0x77 | 8x8 led matrix
   6  | USE_DISPLAY_SH1106  | xdsp_07 | SH1106   | 0x3C - 0x3D | Oled display
   7  | USE_ADE7953         | xnrg_07 | ADE7953  | 0x38        | Energy monitor
   8  | USE_SHT             | xsns_07 | SHT1X    | Any         | Temperature and Humidity sensor
   9  | USE_HTU             | xsns_08 | HTU21    | 0x40        | Temperature and Humidity sensor
   9  | USE_HTU             | xsns_08 | SI7013   | 0x40        | Temperature and Humidity sensor
   9  | USE_HTU             | xsns_08 | SI7020   | 0x40        | Temperature and Humidity sensor
   9  | USE_HTU             | xsns_08 | SI7021   | 0x40        | Temperature and Humidity sensor
  10  | USE_BMP             | xsns_09 | BMP085   | 0x76 - 0x77 | Pressure and temperature sensor
  10  | USE_BMP             | xsns_09 | BMP180   | 0x76 - 0x77 | Pressure and temperature sensor
  10  | USE_BMP             | xsns_09 | BMP280   | 0x76 - 0x77 | Pressure and temperature sensor
  10  | USE_BMP             | xsns_09 | BME280   | 0x76 - 0x77 | Pressure, temperature and humidity sensor
  10  | USE_BMP             | xsns_09 | BME680   | 0x76 - 0x77 | Pressure, temperature, humidity and gas sensor
  11  | USE_BH1750          | xsns_10 | BH1750   | 0x23, 0x5C  | Ambient light intensity sensor
  12  | USE_VEML6070        | xsns_11 | VEML6070 | 0x38 - 0x39 | Ultra violet light intensity sensor
  13  | USE_ADS1115_I2CDEV  | xsns_12 | ADS1115  | 0x48 - 0x4B | 4-channel 16-bit A/D converter
  13  | USE_ADS1115         | xsns_12 | ADS1115  | 0x48 - 0x4B | 4-channel 16-bit A/D converter
  14  | USE_INA219          | xsns_13 | INA219   | 0x40 - 0x41, 0x44 - 0x45 | Low voltage current sensor
  15  | USE_SHT3X           | xsns_14 | SHT3X    | 0x44 - 0x45 | Temperature and Humidity sensor
  15  | USE_SHT3X           | xsns_14 | SHTC3    | 0x70        | Temperature and Humidity sensor
  16  | USE_TSL2561         | xsns_16 | TSL2561  | 0x29, 0x39, 0x49 | Light intensity sensor
  17  | USE_MGS             | xsns_19 | Grove    | 0x04        | Multichannel gas sensor
  18  | USE_SGP30           | xsns_21 | SGP30    | 0x58        | Gas (TVOC) and air quality sensor
  19  | USE_SI1145          | xsns_24 | SI1145   | 0x60        | Ultra violet index and light sensor
  19  | USE_SI1145          | xsns_24 | SI1146   | 0x60        | Ultra violet index and light sensor
  19  | USE_SI1145          | xsns_24 | SI1147   | 0x60        | Ultra violet index and light sensor
  20  | USE_LM75AD          | xsns_26 | LM75AD   | 0x48 - 0x4F | Temperature sensor
  21  | USE_APDS9960        | xsns_27 | APDS9960 | 0x39        | Proximity ambient light RGB and gesture sensor
  22  | USE_MCP230xx        | xsns_29 | MCP23008 | 0x20 - 0x27 | 16-bit I/O expander
  22  | USE_MCP230xx        | xsns_29 | MCP23017 | 0x20 - 0x27 | 16-bit I/O expander
  23  | USE_MPR121          | xsns_30 | MPR121   | 0x5A - 0x5D | Proximity capacitive touch sensor
  24  | USE_CCS811          | xsns_31 | CCS811   | 0x5A        | Gas (TVOC) and air quality sensor
  25  | USE_MPU6050         | xsns_32 | MPU6050  | 0x68 - 0x69 | 3-axis gyroscope and temperature sensor
  26  | USE_DS3231          | xsns_33 | DS3231   | 0x68        | Real time clock
  27  | USE_MGC3130         | xsns_36 | MGC3130  | 0x42        | Electric field sensor
  28  | USE_MAX44009        | xsns_41 | MAX44009 | 0x4A - 0x4B | Ambient light intensity sensor
  29  | USE_SCD30           | xsns_42 | SCD30    | 0x61        | CO2 sensor
  30  | USE_SPS30           | xsns_44 | SPS30    | 0x69        | Particle sensor
  31  | USE_VL53L0X         | xsns_45 | VL53L0X  | 0x29        | Time-of-flight (ToF) distance sensor
  32  | USE_MLX90614        | xsns_46 | MLX90614 | 0x5A        | Infra red temperature sensor
  33  | USE_CHIRP           | xsns_48 | CHIRP    | 0x20        | Soil moisture sensor
  34  | USE_PAJ7620         | xsns_50 | PAJ7620  | 0x73        | Gesture sensor
  35  | USE_INA226          | xsns_54 | INA226   | 0x40 - 0x41, 0x44 - 0x45 | Low voltage current sensor
  36  | USE_HIH6            | xsns_55 | HIH6130  | 0x27        | Temperature and Humidity sensor
