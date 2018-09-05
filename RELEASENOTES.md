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
| ESP/Arduino lib v2.3.0         | 340k    | 477k    | 473k   | 492k | 497k    |
| ESP/Arduino lib v2.4.2         | 360k    | 491k    | 491k   | 509k | 513k    | No sleep
|                                |   |   |   |   |   |
| MY_LANGUAGE en-GB              | x | x | x | x | x |
| USE_WPS                        | - | x | - | - | - | WPS
| USE_SMARTCONFIG                | - | x | - | - | - | SmartConfig
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
 * Allow user override of define MAX_RULE_VARS and MAX_RULE_TIMERS (#3561)
 * Disable wifi sleep for both Esp8266/Arduino core 2.4.1 and 2.4.2 to solve device freeze caused by Espressif SDK bug (#3554)
 * Change DS18B20 driver to provide better instant results
 * Change some sensor drivers to provide instant results
 * Change define USE_ALL_SENSORS to USE_SENSORS as it doesn't contain all sensors due to duplicate I2C addresses
 * Change some sensor update timings: AdcEvery 200 -> 250, Senseair 300 -> 250, SDM120 300 -> 250, SDM630 300 -> 250
 * Change default Wifi config option from WPS to Wifi Manager if WPS is disabled or Wifi Smartconfig if webserver is disabled or Wifi Serial input if Smartconfig is disabled
 * Change SHT1x driver to provide better instant results and fix I2C interference
 * Change DHT driver to provide better instant results and add decimals to DHT11 (#3164)
 * Change DS18x20 driver to provide better instant results (#3169)
 * Change CounterType 1 from milliseconds to microseconds (#3437)
 * Change scheduler for better sleep support using Uptime, Delay, PulseTime and TelePeriod, Blinktime (#3581)
 * Remove unused functionality from Sonoff-minimal to save space
 * Remove WPS and SmartConfig from sonoff-minimal saving 56k code space
 * Remove TSL2561 debug message and update library (#2415)
 * Remove forced restart when sleep command is executed (#3554)
 * Fix invalid response using more than 4 switches and domoticz
 * Fix sonoff-minimal not using default settings
 * Fix unsecure main webpage update
 * Fix DHT driver mixing values for different sensors (#1797)
 * Fix EnergyReset3 regression not clearing total energy (#2723)
 * Fix rules once regression from v6.1.0 (#3198, #3226)
 * Fix command Scale buffer overflow (#3236)
 * Fix possible WDT due to long MQTT publish handling (#3313)
 * Fix command TimeDst/TimeStd invalid JSON (#3322)
 * Fix handling of default names when using names starting with shortcut character ",0,1 or 2 (#3392, #3600, #3618)
 * Fix LM75AD I2C sensor detection (#3408)
 * Fix iFan02 power on state (#3412, #3530)
 * Fix some Pow R2 and S31 checksum errors using optimized re-sync (#3425)
 * Fix SDM120 reporting wrong negative values to Domoticz (#3521)
 * Fix MQTT reconnection detection when using TasmotaMqtt library (#3558)
 * Fix OtaMagic when file path contains a dash (-) (#3563)
 * Fix Sonoff Bridge data reception when using Portisch EFM8 firmware using in data buffer length (#3605)
 * Add read sensor retry to DS18B20, DS18x20, DHT, SHT1X and HTU21
 * Add user selection of Wifi Smartconfig as define USE_SMARTCONFIG in user_config.h
 * Add boot loop detection and perform some solutions
 * Add wifi and mqtt status led blinkyblinky to be disabled by SetOption31 1. Does not work when LedPower is On (deliberate) (#871, #2230, #3114, #3155)
 * Add support for TM1638 switch (#2226)
 * Add GPIO options ButtonXn, SwitchXn and CounterXn to select INPUT mode instead of INPUT_PULLUP (#2525)
 * Add support for APDS9960 proximity sensor (#3051)
 * Add support for MPR121 controller in input mode for touch buttons (#3142)
 * Add support for MCP230xx for general purpose input expansion and command Sensor29 (#3188)
 * Add default Wifi Configuration tool as define WIFI_CONFIG_NO_SSID in user_config.h if no SSID is configured (#3224)
 * Add command Timers 0/1 to globally disable or enable armed timers (#3270)
 * Add support for CCS811 sensor (#3309)
 * Add Turkish language file (#3332)
 * Add command SerialSend4 to send binary serial data (#3345)
 * Add initial support for sensor MPU6050 (#3352)
 * Add rule triggers Wifi#Connected and Wifi#Disconnected (#3359)
 * Add option + to command Rule to concatenate new rule with existing rules (#3365)
 * Add message when JavaScript is not enabled in webbrowser (#3388)
 * Add build time setting of ButtonTopic and SwitchTopic (#3414)
 * Add iFan02 Fanspeed + and Fanspeed - command options (#3415)
 * Add Individual HSBColorX commands (#3430, #3615)
 * Add output support on MCP23008/MCP23017 (#3436)
 * Add modulo option to rules like rule1 on Time#Minute|5 do backlog power on;delay 200;power off endon (#3466)
 * Add RGB support for Domoticz (#3547)
 * Add all ruletimer values to command RuleTimer result message (#3571)
 * Add command Publish2 for publishing retained MQTT messages (#3593)
 * Add commands ButtonDebounce 40..1000 and SwitchDebounce 40..1000 to have user control over debounce timing. Default is 50mS (#3594)
 * Add RuleX debug options 8,9,10 (StopOnError) to control RuleX execution status after an exception restart (#3607)
 * Add rule variables %sunrise%, %sunset%, %uptime% and %time% (#3608)
 * Add optional MQTT_TELE_RETAIN to Energy Margins message (#3612, 3614)
