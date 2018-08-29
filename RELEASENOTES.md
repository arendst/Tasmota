## Migration Information
See [wiki migration path](https://github.com/arendst/Sonoff-Tasmota/wiki/Upgrade#migration-path) for instructions how to migrate to a major version. Pay attention to the following version breaks due to dynamic settings updates:

1. Migrate to **Sonoff-Tasmota 3.9.x**
2. Migrate to **Sonoff-Tasmota 4.x**
3. Migrate to **Sonoff-Tasmota 5.14**
4. Migrate to **Sonoff-Tasmota 6.x**

## Release notes
### Change in default initial configuration tool
Firmware binary **sonoff-classic.bin** supports **WifiManager, Wps and SmartConfig** for initial configuration. The default tool is **Wps**.

To save memory space all other binaries support **WifiManager only**.

See _changelog.ino how to enable them again.
- Define WIFI_CONFIG_TOOL now contains the default behaviour once a SSID has been configured.
- If no SSID is configured making a wifi connection impossible the new define WIFI_CONFIG_NO_SSID will be used.
- While define WIFI_CONFIG_NO_SSID is set to WIFI_WPSCONFIG in user_config.h the compiler will check for define USE_WPS and if not enabled WIFI_CONFIG_NO_SSID will default to WIFI_MANAGER using the webserver. If define USE_WEBSERVER is also not enabled WIFI_CONFIG_NO_SSID will default to WIFI_SMARTCONFIG. If define USE_SMARTCONFIG is also not enabled WIFI_CONFIG_NO_SSID will default to a new option WIFI_SERIAL allowing to enter wifi parameters to serial which is always possible.

## Provided Binary Downloads
The following binary downloads have been compiled with ESP8266/Arduino library version **2.3.0**

- **sonoff-minimal.bin** = The Minimal version allows intermediate OTA uploads to support larger versions and does NOT change any persistent parameter. This version **should NOT be used for initial installation**.
- **sonoff-classic.bin** = The Classic version allows **initial installation** using either WifiManager, Wps or SmartConfig.
- **sonoff.bin** = The Sonoff version without Wps and SmartConfig configuration but adds more sensors.
- **sonoff-BG.bin** to **sonoff-TW.bin** = The Sonoff version without Wps and SmartConfig configuration in different languages.
- **sonoff-sensors.bin** = The Sensors version without Wps and SmartConfig configuration but adds even more useful sensors.
- **sonoff-knx.bin** = The Knx version without Wps and SmartConfig configuration and some other features but adds KNX support.

See [Tasmota ESP/Arduino library version related issues](https://github.com/arendst/Sonoff-Tasmota/wiki/Theo's-Tasmota-Tips#20180523---relation-tasmota-and-esp8266arduino-core-version) why these files are still released using ESP/Arduino library version v2.3.0.

### Available Features and Sensors

| Feature or Sensor              | minimal | classic | sonoff | knx  | sensors | Remarks
|--------------------------------|---------|---------|--------|------|---------|--------
| ESP/Arduino lib v2.3.0         | 340k    | 476k    | 472k   | 491k | 497k    |
| ESP/Arduino lib v2.4.2         | 360k    | 490k    | 490k   | 508k | 513k    | No sleep
|                                |   |   |   |   |   |
| MY_LANGUAGE en-GB              | x | x | x | x | x |
| USE_WPS                        | - | x | - | - | - |
| USE_SMARTCONFIG                | - | x | - | - | - |
| USE_ARDUINO_OTA                | - | - | - | - | - |
| MQTT_LIBRARY_TYPE PUBSUBCLIENT | x | x | x | x | x |
| USE_DOMOTICZ                   | - | x | x | x | x |
| USE_HOME_ASSISTANT             | - | x | x | x | x |
| USE_MQTT_TLS                   | - | - | - | - | - |
| USE_KNX                        | - | - | - | x | - |
| USE_WEBSERVER                  | x | x | x | x | x | WifiManager
| USE_EMULATION                  | - | x | x | - | x |
| USE_DISCOVERY                  | - | x | x | x | x |
| WEBSERVER_ADVERTISE            | - | x | x | x | x |
| MQTT_HOST_DISCOVERY            | - | x | x | x | x |
| USE_TIMERS                     | - | - | x | x | x |
| USE_TIMERS_WEB                 | - | - | x | x | x |
| USE_SUNRISE                    | - | - | x | x | x |
| USE_RULES                      | - | - | x | x | x |
|                                |   |   |   |   |   |
| USE_ADC_VCC                    | x | x | x | x | x |
| USE_DS18B20                    | - | - | - | - | - | Single sensor
| USE_DS18x20                    | - | x | x | x | x | Multiple sensors
| USE_DS18x20_LEGACY             | - | - | - | - | - | Multiple sensors
|                                |   |   |   |   |   |
| Feature or Sensor              | minimal | classic | sonoff | knx  | sensors |
| USE_I2C                        | - | - | x | x | x |
| USE_SHT                        | - | - | x | x | x |
| USE_HTU                        | - | - | x | x | x |
| USE_BMP                        | - | - | x | x | x |
| USE_BME680                     | - | - | - | - | x |
| USE_BH1750                     | - | - | x | x | x |
| USE_VEML6070                   | - | - | - | - | x |
| USE_ADS1115                    | - | - | - | - | x |
| USE_ADS1115_I2CDEV             | - | - | - | - | - |
| USE_INA219                     | - | - | - | - | x |
| USE_SHT3X                      | - | - | x | x | x |
| USE_TSL2561                    | - | - | - | - | x |
| USE_MGS                        | - | - | - | - | x |
| USE_SGP30                      | - | - | x | x | x |
| USE_SI1145                     | - | - | - | - | x |
| USE_LM75AD                     | - | - | x | x | x |
| USE_APDS9960                   | - | - | - | - | - |
| USE_MCP230xx                   | - | - | - | - | - |
| USE_MPR121                     | - | - | - | - | - |
| USE_CCS811                     | - | - | - | - | - |
| USE_MPU6050                    | - | - | - | - | - |
|                                |   |   |   |   |   |
| Feature or Sensor              | minimal | classic | sonoff | knx  | sensors |
| USE_SPI                        | - | - | - | - | - |
| USE_MHZ19                      | - | - | x | x | x |
| USE_SENSEAIR                   | - | - | x | x | x |
| USE_PMS5003                    | - | - | x | x | x |
| USE_NOVA_SDS                   | - | - | x | x | x |
| USE_PZEM004T                   | - | - | x | x | x |
| USE_SERIAL_BRIDGE              | - | - | x | x | x |
| USE_SDM120                     | - | - | - | - | x |
| USE_SDM630                     | - | - | - | - | x |
| USE_IR_REMOTE                  | - | - | x | x | x |
| USE_IR_HVAC                    | - | - | - | - | x |
| USE_IR_RECEIVE                 | - | - | x | x | x |
| USE_WS2812                     | - | x | x | x | x |
| USE_WS2812_DMA                 | - | - | - | - | - |
| USE_ARILUX_RF                  | - | - | x | x | x |
| USE_SR04                       | - | - | x | x | x |
| USE_TM1638                     | - | - | - | - | - |
| USE_RF_FLASH                   | - | - | x | x | x |

## Changelog
Version 6.2.0 20180901

tbd