/*
  support_features.ino - feature support for Tasmota

  Copyright (C) 2021  Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*********************************************************************************************\
 * Fill feature list
\*********************************************************************************************/

void ResponseAppendFeatures(void)
{
  static uint32_t feature1 = 0x00000000;
  if (!feature1) {           // Only fill this once
#if defined(USE_ENERGY_SENSOR) && defined(USE_ENERGY_MARGIN_DETECTION)
    feature1 |= 0x00000001;  // xdrv_03_energy.ino
#endif
#ifdef USE_LIGHT
    feature1 |= 0x00000002;  // tasmota.ino, xdrv_04_light.ino
#endif
#ifdef USE_I2C
    feature1 |= 0x00000004;  // tasmota.ino
#endif
#ifdef USE_SPI
    feature1 |= 0x00000008;  // tasmota.ino
#endif
#ifdef USE_DISCOVERY
    feature1 |= 0x00000010;  // tasmota.ino
#endif
#ifdef USE_ARDUINO_OTA
    feature1 |= 0x00000020;  // tasmota.ino
#endif
#ifdef USE_MQTT_TLS
    feature1 |= 0x00000040;  // xdrv_02_mqtt.ino
#endif
#ifdef USE_WEBSERVER
    feature1 |= 0x00000080;  // xdrv_01_webserver.ino
#endif
#if defined(USE_WEBSERVER) && defined(WEBSERVER_ADVERTISE)
    feature1 |= 0x00000100;  // xdrv_01_webserver.ino
#endif
#if defined(USE_WEBSERVER) && defined(USE_EMULATION_HUE)
    feature1 |= 0x00000200;  // xdrv_20_hue.ino
#endif
//#if (MQTT_LIBRARY_TYPE == MQTT_PUBSUBCLIENT)
    feature1 |= 0x00000400;  // xdrv_02_mqtt.ino
//#endif
//#if (MQTT_LIBRARY_TYPE == MQTT_TASMOTAMQTT)
//    feature1 |= 0x00000800;  // xdrv_02_mqtt.ino
//#endif
#ifdef USE_MODBUS_BRIDGE
    feature1 |= 0x00001000;  // xdrv_63_modbus_bridge.ino
#endif
#if defined(USE_DISCOVERY) && defined(MQTT_HOST_DISCOVERY)
    feature1 |= 0x00002000;  // xdrv_02_mqtt.ino
#endif
#if defined(USE_LIGHT) && defined(USE_ARILUX_RF)
    feature1 |= 0x00004000;  // xdrv_04_light.ino
#endif
#if defined(USE_LIGHT) && defined(USE_WS2812)
    feature1 |= 0x00008000;  // xdrv_04_light.ino
#endif
#if defined(USE_LIGHT) && defined(USE_WS2812) && defined(USE_WS2812_DMA)
    feature1 |= 0x00010000;  // xdrv_04_light.ino
#endif
#if defined(USE_IR_REMOTE) || defined(USE_IR_REMOTE_FULL)
    feature1 |= 0x00020000;  // xdrv_05_irremote.ino
#endif
#ifdef USE_IR_HVAC
    feature1 |= 0x00040000;  // xdrv_05_irremote.ino
#endif
#if defined(USE_IR_REMOTE) && defined(USE_IR_RECEIVE)
    feature1 |= 0x00080000;  // xdrv_05_irremote.ino
#endif
#ifdef USE_DOMOTICZ
    feature1 |= 0x00100000;  // xdrv_07_domoticz.ino
#endif
#ifdef USE_DISPLAY
    feature1 |= 0x00200000;  // xdrv_13_display.ino
#endif
#ifdef USE_HOME_ASSISTANT
    feature1 |= 0x00400000;  // xdrv_12_home_assistant.ino
#endif
#ifdef USE_SERIAL_BRIDGE
    feature1 |= 0x00800000;  // xdrv_08_serial_bridge.ino
#endif
#ifdef USE_TIMERS
    feature1 |= 0x01000000;  // xdrv_09_timers.ino
#endif
#if defined(USE_TIMERS) && defined(USE_SUNRISE)
    feature1 |= 0x02000000;  // xdrv_09_timers.ino
#endif
#if defined(USE_TIMERS) && defined(USE_TIMERS_WEB)
    feature1 |= 0x04000000;  // xdrv_09_timers.ino
#endif
#ifdef USE_RULES
    feature1 |= 0x08000000;  // xdrv_10_rules.ino
#endif
#ifdef USE_KNX
    feature1 |= 0x10000000;  // xdrv_11_knx.ino
#endif
#ifdef USE_WPS
    feature1 |= 0x20000000;  // support.ino - removed with version 6.6.0.21
#endif
#ifdef USE_SMARTCONFIG
    feature1 |= 0x40000000;  // support.ino - removed with version 6.6.0.21
#endif
#if defined(USE_ENERGY_SENSOR) && defined(USE_ENERGY_POWER_LIMIT)
    feature1 |= 0x80000000;  // xdrv_03_energy.ino
#endif
  }

  static uint32_t feature2 = 0x00000000;
  if (!feature2) {           // Only fill this once
#ifdef USE_CONFIG_OVERRIDE
    feature2 |= 0x00000001;  // user_config(_override).h
#endif
#ifdef FIRMWARE_MINIMAL
    feature2 |= 0x00000002;  // user_config(_override).h
#endif
#ifdef FIRMWARE_SENSORS
    feature2 |= 0x00000004;  // user_config(_override).h
#endif
#ifdef FIRMWARE_CLASSIC
    feature2 |= 0x00000008;  // user_config(_override).h
#endif
#ifdef FIRMWARE_KNX_NO_EMULATION
    feature2 |= 0x00000010;  // user_config(_override).h
#endif
#if defined(USE_DISPLAY) && defined(USE_DISPLAY_MODES1TO5)
    feature2 |= 0x00000020;  // xdrv_13_display.ino
#endif
#if defined(USE_DISPLAY) && defined(USE_DISPLAY_GRAPH)
    feature2 |= 0x00000040;  // xdrv_13_display.ino
#endif
#if defined(USE_I2C) && defined(USE_DISPLAY) && defined(USE_DISPLAY_LCD)
    feature2 |= 0x00000080;  // xdsp_01_lcd.ino
#endif
#if defined(USE_I2C) && defined(USE_DISPLAY) && defined(USE_DISPLAY_SSD1306)
    feature2 |= 0x00000100;  // xdsp_02_ssd1306.ino
#endif
#if defined(USE_I2C) && defined(USE_DISPLAY) && defined(USE_DISPLAY_MATRIX)
    feature2 |= 0x00000200;  // xdsp_03_matrix.ino
#endif
#if defined(USE_SPI) && defined(USE_DISPLAY) && defined(USE_DISPLAY_ILI9341)
    feature2 |= 0x00000400;  // xdsp_04_ili9341.ino
#endif
#if defined(USE_SPI) && defined(USE_DISPLAY) && defined(USE_DISPLAY_EPAPER_29)
    feature2 |= 0x00000800;  // xdsp_05_epaper.ino
#endif
#if defined(USE_I2C) && defined(USE_DISPLAY) && defined(USE_DISPLAY_SH1106)
    feature2 |= 0x00001000;  // xdsp_07_sh1106.ino
#endif
#ifdef USE_MP3_PLAYER
    feature2 |= 0x00002000;  // xdrv_14_mp3.ino
#endif
#if defined(USE_I2C) && (defined(USE_PCA9685) || defined(USE_PCA9685_V2))
    feature2 |= 0x00004000;  // xdrv_15_pca9685.ino or xdrv_15_pca9685_v2.ino
#endif
#if defined(USE_LIGHT) && defined(USE_TUYA_MCU)
    feature2 |= 0x00008000;  // xdrv_16_tuyadimmer.ino
#endif
#ifdef USE_RC_SWITCH
    feature2 |= 0x00010000;  // xdrv_17_rcswitch.ino
#endif
#if defined(USE_LIGHT) && defined(USE_ARMTRONIX_DIMMERS)
    feature2 |= 0x00020000;  // xdrv_18_armtronixdimmer.ino
#endif
#if defined(USE_LIGHT) && defined(USE_SM16716)
    feature2 |= 0x00040000;  // xdrv_04_light.ino
#endif
#ifdef USE_SCRIPT
    feature2 |= 0x00080000;  // xdrv_10_scripter.ino
#endif
#if defined(USE_WEBSERVER) && defined(USE_EMULATION_WEMO)
    feature2 |= 0x00100000;  // xdrv_21_wemo.ino
#endif
#ifdef USE_SONOFF_IFAN
    feature2 |= 0x00200000;  // xdrv_22_sonoff_ifan.ino
#endif
#ifdef USE_ZIGBEE
    feature2 |= 0x00400000;  // xdrv_23_zigbee.ino
#endif
#ifdef NO_EXTRA_4K_HEAP
    feature2 |= 0x00800000;  // sonoff_post.h
#endif
#ifdef VTABLES_IN_IRAM
    feature2 |= 0x01000000;  // platformio.ini
#endif
#ifdef VTABLES_IN_DRAM
    feature2 |= 0x02000000;  // platformio.ini
#endif
#ifdef VTABLES_IN_FLASH
    feature2 |= 0x04000000;  // platformio.ini
#endif
#ifdef PIO_FRAMEWORK_ARDUINO_LWIP_HIGHER_BANDWIDTH
    feature2 |= 0x08000000;  // platformio.ini
#endif
#ifdef PIO_FRAMEWORK_ARDUINO_LWIP2_LOW_MEMORY
    feature2 |= 0x10000000;  // platformio.ini
#endif
#ifdef PIO_FRAMEWORK_ARDUINO_LWIP2_HIGHER_BANDWIDTH
    feature2 |= 0x20000000;  // platformio.ini
#endif
#ifdef DEBUG_THEO
    feature2 |= 0x40000000;  // xdrv_99_debug.ino
#endif
#ifdef USE_DEBUG_DRIVER
    feature2 |= 0x80000000;  // xdrv_99_debug.ino
#endif
  }

  static uint32_t feature3 = 0x00000000;
  if (!feature3) {           // Only fill this once
#ifdef USE_COUNTER
    feature3 |= 0x00000001;  // xsns_01_counter.ino
#endif
#if defined(USE_ADC_VCC) || defined(USE_ADC)
    feature3 |= 0x00000002;  // xsns_02_analog.ino
#endif
#ifdef USE_ENERGY_SENSOR
    feature3 |= 0x00000004;  // xdrv_03_energy.ino
#endif
#if defined(USE_ENERGY_SENSOR) && defined(USE_PZEM004T)
    feature3 |= 0x00000008;  // xnrg_03_pzem004t.ino
#endif
#ifdef USE_DS18B20
    feature3 |= 0x00000010;  // xsns_05_ds18b20.ino - no more support since 6.6.0.18
#endif
#ifdef USE_DS18x20_LEGACY
    feature3 |= 0x00000020;  // xsns_05_ds18x20_legacy.ino - no more support since 6.6.0.14
#endif
#ifdef USE_DS18x20
    feature3 |= 0x00000040;  // xsns_05_ds18x20.ino
#endif
#ifdef USE_DHT
    feature3 |= 0x00000080;  // xsns_06_dht.ino
#endif
#if defined(USE_I2C) && defined(USE_SHT)
    feature3 |= 0x00000100;  // xsns_07_sht1x.ino
#endif
#if defined(USE_I2C) && defined(USE_HTU)
    feature3 |= 0x00000200;  // xsns_08_htu21.ino
#endif
#if defined(USE_I2C) && defined(USE_BMP)
    feature3 |= 0x00000400;  // xsns_09_bmp.ino
#endif
#if defined(USE_I2C) && defined(USE_BMP) && defined(USE_BME68X)
    feature3 |= 0x00000800;  // xsns_09_bmp.ino - BME68x
#endif
#if defined(USE_I2C) && defined(USE_BH1750)
    feature3 |= 0x00001000;  // xsns_10_bh1750.ino
#endif
#if defined(USE_I2C) && defined(USE_VEML6070)
    feature3 |= 0x00002000;  // xsns_11_veml6070.ino
#endif
#if defined(USE_I2C) && defined(USE_ADS1115_I2CDEV)
    feature3 |= 0x00004000;  // xsns_12_ads1115_i2cdev.ino
#endif
#if defined(USE_I2C) && defined(USE_ADS1115)
    feature3 |= 0x00008000;  // xsns_12_ads1115.ino
#endif
#if defined(USE_I2C) && defined(USE_INA219)
    feature3 |= 0x00010000;  // xsns_13_ina219.ino
#endif
#if defined(USE_I2C) && defined(USE_SHT3X)
    feature3 |= 0x00020000;  // xsns_14_sht3x.ino
#endif
#ifdef USE_MHZ19
    feature3 |= 0x00040000;  // xsns_15_mhz19.ino
#endif
#if defined(USE_I2C) && defined(USE_TSL2561)
    feature3 |= 0x00080000;  // xsns_16_tsl2561.ino
#endif
#ifdef USE_SENSEAIR
    feature3 |= 0x00100000;  // xsns_17_senseair.ino
#endif
#ifdef USE_PMS5003
    feature3 |= 0x00200000;  // xsns_18_pms5003.ino
#endif
#if defined(USE_I2C) && defined(USE_MGS)
    feature3 |= 0x00400000;  // xsns_19_mgs.ino
#endif
#ifdef USE_NOVA_SDS
    feature3 |= 0x00800000;  // xsns_20_novasds.ino
#endif
#if defined(USE_I2C) && defined(USE_SGP30)
    feature3 |= 0x01000000;  // xsns_21_sgp30.ino
#endif
#ifdef USE_SR04
    feature3 |= 0x02000000;  // xsns_22_sr04.ino
#endif
#if defined(USE_ENERGY_SENSOR) && defined(USE_SDM120)
    feature3 |= 0x04000000;  // xnrg_08_sdm120.ino
#endif
#if defined(USE_I2C) && defined(USE_SI1145)
    feature3 |= 0x08000000;  // xsns_24_si1145.ino
#endif
#if defined(USE_ENERGY_SENSOR) && defined(USE_SDM630)
    feature3 |= 0x10000000;  // xnrg_10_sdm630.ino
#endif
#if defined(USE_I2C) && defined(USE_LM75AD)
    feature3 |= 0x20000000;  // xsns_26_lm75ad.ino
#endif
#if defined(USE_I2C) && defined(USE_APDS9960)
    feature3 |= 0x40000000;  // xsns_27_apds9960.ino
#endif
#ifdef USE_TM1638
    feature3 |= 0x80000000;  // xdrv_66_tm1638.ino
#endif
  }

  static uint32_t feature4 = 0x00000000;
  if (!feature4) {           // Only fill this once
#if defined(USE_I2C) && defined(USE_MCP230xx)
    feature4 |= 0x00000001;  // xsns_29_mcp230xx.ino
#endif
#if defined(USE_I2C) && defined(USE_MPR121)
    feature4 |= 0x00000002;  // xsns_30_mpr121.ino
#endif
#if defined(USE_I2C) && (defined(USE_CCS811) || defined(USE_CCS811_V2))
    feature4 |= 0x00000004;  // xsns_31_ccs811.ino or xsns_31_ccs811_v2.ino
#endif
#if defined(USE_I2C) && defined(USE_MPU6050)
    feature4 |= 0x00000008;  // xsns_32_mpu6050.ino
#endif
#if defined(USE_I2C) && defined(USE_MCP230xx) && defined(USE_MCP230xx_OUTPUT)
    feature4 |= 0x00000010;  // xsns_29_mcp230xx.ino
#endif
#if defined(USE_I2C) && defined(USE_MCP230xx) && defined(USE_MCP230xx_DISPLAYOUTPUT)
    feature4 |= 0x00000020;  // xsns_29_mcp230xx.ino
#endif
#if defined(USE_ENERGY_SENSOR) && defined(USE_HLW8012)
    feature4 |= 0x00000040;  // xnrg_01_hlw8012.ino
#endif
#if defined(USE_ENERGY_SENSOR) && defined(USE_CSE7766)
    feature4 |= 0x00000080;  // xnrg_02_cse7766.ino
#endif
#if defined(USE_ENERGY_SENSOR) && defined(USE_MCP39F501)
    feature4 |= 0x00000100;  // xnrg_04_mcp39f501.ino
#endif
#if defined(USE_ENERGY_SENSOR) && defined(USE_PZEM_AC)
    feature4 |= 0x00000200;  // xnrg_05_pzem_ac.ino
#endif
#if defined(USE_I2C) && defined(USE_DS3231)
    feature4 |= 0x00000400;  // xsns_33_ds3231.ino
#endif
#ifdef USE_HX711
    feature4 |= 0x00000800;  // xsns_34_hx711.ino
#endif
#if defined(USE_ENERGY_SENSOR) && defined(USE_PZEM_DC)
    feature4 |= 0x00001000;  // xnrg_06_pzem_dc.ino
#endif
#if defined(USE_TX20_WIND_SENSOR) || defined(USE_TX23_WIND_SENSOR) || defined(USE_WS2300_WIND_SENSOR)
    feature4 |= 0x00002000;  // xsns_35_tx20.ino
#endif
#if defined(USE_I2C) && defined(USE_MGC3130)
    feature4 |= 0x00004000;  // xsns_36_mgc3130.ino
#endif
#ifdef USE_RF_SENSOR
    feature4 |= 0x00008000;  // xsns_37_rfsensor.ino
#endif
#if defined(USE_RF_SENSOR) && defined(USE_THEO_V2)
    feature4 |= 0x00010000;  // xsns_37_rfsensor.ino
#endif
#if defined(USE_RF_SENSOR) && defined(USE_ALECTO_V2)
    feature4 |= 0x00020000;  // xsns_37_rfsensor.ino
#endif
#ifdef USE_AZ7798
    feature4 |= 0x00040000;  // xsns_38_az7798.ino
#endif
#ifdef USE_MAX31855
    feature4 |= 0x00080000;  // xsns_39_max31855.ino
#endif
#ifdef USE_PN532_HSU
    feature4 |= 0x00100000;  // xsns_40_pn532.ino
#endif
#if defined(USE_I2C) && defined(USE_MAX44009)
    feature4 |= 0x00200000;  // xsns_41_max44009.ino
#endif
#if defined(USE_I2C) && defined(USE_SCD30)
    feature4 |= 0x00400000;  // xsns_42_scd30.ino
#endif
#ifdef USE_HRE
    feature4 |= 0x00800000;  // xsns_43_hre.ino
#endif
#if defined(USE_ENERGY_SENSOR) && defined(USE_ADE7953)
    feature4 |= 0x01000000;  // xnrg_07_ade7953.ino
#endif
#if defined(USE_I2C) && defined(USE_SPS30)
    feature4 |= 0x02000000;  // xsns_44_sps30.ino
#endif
#if defined(USE_I2C) && defined(USE_VL53L0X)
    feature4 |= 0x04000000;  // xsns_45_vl53l0x.ino
#endif
#if defined(USE_I2C) && defined(USE_MLX90614)
    feature4 |= 0x08000000;  // xsns_46_MLX90614.ino
#endif
#ifdef USE_MAX31865
    feature4 |= 0x10000000;  // xsns_47-max31865.ino
#endif
#if defined(USE_I2C) && defined(USE_CHIRP)
    feature4 |= 0x20000000;  // xsns_48_chirp.ino
#endif
#if defined(USE_ENERGY_SENSOR) && defined(USE_SOLAX_X1)
    feature4 |= 0x40000000;  // xnrg_12_solaxX1.ino
#endif
#if defined(USE_I2C) && defined(USE_PAJ7620)
    feature4 |= 0x80000000;  // xsns_50_paj7620.ino
#endif
  }

  static uint32_t feature5 = 0x00000000;
  if (!feature5) {           // Only fill this once
#ifdef USE_BUZZER
    feature5 |= 0x00000001;  // xdrv_24_buzzer.ino
#endif
#ifdef USE_RDM6300
    feature5 |= 0x00000002;  // xsns_51_rdm6300.ino
#endif
#ifdef USE_IBEACON
    feature5 |= 0x00000004;  // xsns_52_ibeacon.ino
#endif
#ifdef USE_SML_M
    feature5 |= 0x00000008;  // xsns_53_sml.ino
#endif
#if defined(USE_I2C) && defined(USE_INA226)
    feature5 |= 0x00000010;  // xsns_54_ina226.ino
#endif
#ifdef USE_A4988_STEPPER
    feature5 |= 0x00000020;  // xdrv_25_A4988.ino
#endif
#if defined(USE_ENERGY_SENSOR) && defined(USE_DDS2382)
    feature5 |= 0x00000040;  // xnrg_09_dds2382.ino
#endif
#if defined(USE_LIGHT) && defined(USE_SM2135)
    feature5 |= 0x00000080;  // xdrv_026_sm2135.ino
#endif
#ifdef USE_SHUTTER
    feature5 |= 0x00000100;  // xdrv_027_shutter.ino
#endif
#if defined(USE_I2C) && defined(USE_PCF8574)
    feature5 |= 0x00000200;  // xdrv_028_pcf8574.ino
#endif
#if defined(USE_ENERGY_SENSOR) && defined(USE_DDSU666)
    feature5 |= 0x00000400;  // xnrg_11_ddsu666.ino
#endif
#ifdef USE_DEEPSLEEP
    feature5 |= 0x00000800;  // xdrv_029_deepsleep.ino
#endif
#ifdef USE_SONOFF_SC
    feature5 |= 0x00001000;  // xsns_04_snfsc.ino
#endif
#ifdef USE_SONOFF_RF
    feature5 |= 0x00002000;  // xdrv_06_snfbridge.ino
#endif
#if defined(USE_LIGHT) && defined(USE_SONOFF_L1)
    feature5 |= 0x00004000;  // xlgt_05_sonoff_l1.ino
#endif
#if defined(USE_LIGHT) && defined(USE_EXS_DIMMER)
    feature5 |= 0x00008000;  // xdrv_30_exs_dimmer.ino
#endif
#ifdef USE_TASMOTA_CLIENT
    feature5 |= 0x00010000;  // xdrv_31_tasmota_client.ino
#endif
#if defined(USE_I2C) && defined(USE_HIH6)
    feature5 |= 0x00020000;  // xsns_55_hih_series.ino
#endif
#ifdef USE_HPMA
    feature5 |= 0x00040000;  // xsns_56_hpma.ino
#endif
#if defined(USE_I2C) && defined(USE_TSL2591)
    feature5 |= 0x00080000;  // xsns_57_tsl2591.ino
#endif
#if defined(USE_I2C) && defined(USE_DHT12)
    feature5 |= 0x00100000;  // xsns_58_dht12.ino
#endif
#if defined(USE_I2C) && defined(USE_DS1624)
    feature5 |= 0x00200000;  // xsns_59_ds1624.ino
#endif
#ifdef USE_GPS
    feature5 |= 0x00400000;  // xsns_60_GPS.ino
#endif
#if defined(USE_I2C) && defined(USE_HOTPLUG)
    feature5 |= 0x00800000;  // xdrv_32_hotplug.ino
#endif
#ifdef USE_NRF24
    feature5 |= 0x01000000;  // xsns_33_nrf24l01.ino
#endif
#ifdef USE_MIBLE
    feature5 |= 0x02000000;  // xsns_61_MI_BLE.ino
#endif
#ifdef USE_HM10
    feature5 |= 0x04000000;  // xsns_62_MI_HM10.ino
#endif
#if defined(USE_ENERGY_SENSOR) && defined(USE_LE01MR)
    feature5 |= 0x08000000;  // xnrg_13_fif_le01mr.ino
#endif
#if defined(USE_I2C) && (defined(USE_AHT1x) || defined(USE_AHT2x))
    feature5 |= 0x10000000;  // xsns_63_aht1x.ino
#endif
#if defined(USE_I2C) && defined(USE_WEMOS_MOTOR_V1)
    feature5 |= 0x20000000;  // xdrv_34_wemos_motor_v1.ino
#endif
#ifdef USE_DEVICE_GROUPS
    feature5 |= 0x40000000;  // support_device_groups.ino
#endif
#ifdef USE_PWM_DIMMER
    feature5 |= 0x80000000;  // xdrv_35_pwm_dimmer
#endif
  }

  static uint32_t feature6 = 0x00000000;
  if (!feature6) {           // Only fill this once
#ifdef USE_KEELOQ
    feature6 |= 0x00000001;  // xdrv_36_keeloq.ino
#endif
#ifdef USE_HRXL
    feature6 |= 0x00000002;  // xsns_64_hrxl.ino
#endif
#ifdef USE_SONOFF_D1
    feature6 |= 0x00000004;  // xdrv_37_sonoff_d1.ino
#endif
#if defined(USE_I2C) && defined(USE_HDC1080)
    feature6 |= 0x00000008;  // xsns_65_hdc1080.ino
#endif
#if defined(USE_I2C) && defined(USE_IAQ)
    feature6 |= 0x00000010;  // xsns_66_iAQ.ino
#endif
#if defined(USE_DISPLAY) && defined(USE_DISPLAY_SEVENSEG)
    feature6 |= 0x00000020;  // xdsp_11_sevenseg.ino
#endif
#if defined(USE_I2C) && defined(USE_AS3935)
    feature6 |= 0x00000040;  // xsns_67_as3935.ino
#endif
#ifdef USE_PING
    feature6 |= 0x00000080;  // xdrv_38_ping.ino
#endif
#ifdef USE_WINDMETER
    feature6 |= 0x00000100;  // xsns_68_windmeter.ino
#endif
#ifdef USE_OPENTHERM
    feature6 |= 0x00000200;  // xsns_69_opentherm.ino
#endif
#ifdef USE_THERMOSTAT
    feature6 |= 0x00000400;  // xdrv_39_heating.ino
#endif
#if defined(USE_I2C) && defined(USE_VEML6075)
    feature6 |= 0x00000800;  // xsns_70_veml6075.ino
#endif
#if defined(USE_I2C) && defined(USE_VEML7700)
    feature6 |= 0x00001000;  // xsns_71_veml7700.ino
#endif
#if defined(USE_I2C) && defined(USE_MCP9808)
    feature6 |= 0x00002000;  // xsns_72_mcp9808.ino
#endif
#if defined(USE_ENERGY_SENSOR) && (defined(USE_BL0940) || defined(USE_BL09XX))
    feature6 |= 0x00004000;  // xnrg_14_bl09xx.ino
#endif
#ifdef USE_TELEGRAM
    feature6 |= 0x00008000;  // xdrv_40_telegram.ino
#endif
#if defined(USE_I2C) && defined(USE_HP303B)
    feature6 |= 0x00010000;  // xsns_73_hp303b.ino
#endif
#ifdef USE_TCP_BRIDGE
    feature6 |= 0x00020000;  // xdrv_41_tcp_bridge.ino
#endif
#if defined(USE_ENERGY_SENSOR) && defined(USE_TELEINFO)
    feature6 |= 0x00040000;  // xnrg_15_teleinfo.ino
#endif
#ifdef USE_LMT01
    feature6 |= 0x00080000;  // xsns_74_lmt01.ino
#endif
#ifdef USE_PROMETHEUS
    feature6 |= 0x00100000;  // xsns_75_prometheus.ino
#endif
#if defined(USE_ENERGY_SENSOR) && defined(USE_IEM3000)
    feature6 |= 0x00200000;  // xnrg_16_iem3000.ino
#endif
#ifdef USE_DYP
    feature6 |= 0x00400000;  // xsns_76_dyp.ino
#endif
#ifdef USE_I2S_AUDIO
    feature6 |= 0x00800000;  // xdrv_42_i2s_audio.ino
#endif
#ifdef USE_MLX90640
    feature6 |= 0x01000000;  // xdrv_43_mlx90640.ino
#endif
#if defined(USE_I2C) && defined(USE_VL53L1X)
    feature6 |= 0x02000000;  // xsns_77_vl53l1x.ino
#endif
#ifdef USE_MIEL_HVAC
    feature6 |= 0x04000000;  // xdrv_44_miel_hvac.ino
#endif
#if defined(USE_ENERGY_SENSOR) && defined(USE_WE517)
    feature6 |= 0x08000000;  // xnrg_17_ornowe517.ino
#endif
#if defined(USE_I2C) && defined(USE_EZOPH)
    feature6 |= 0x10000000;  // xsns_78_ezoph.ino
#endif
#if defined(ESP32) && defined(USE_TTGO_WATCH)
    feature6 |= 0x20000000;  // xdrv_83_esp32watch.ino
#endif
#if defined(ESP32) && defined(USE_ETHERNET)
    feature6 |= 0x40000000;  // xdrv_82_ethernet.ino
#endif
#if defined(ESP32) && defined(USE_WEBCAM)
    feature6 |= 0x80000000;  // xdrv_81_webcam.ino
#endif
  }

  static uint32_t feature7 = 0x00000000;
  if (!feature7) {           // Only fill this once
#if defined(USE_I2C) && defined(USE_EZOORP)
    feature7 |= 0x00000001;  // xsns_78_ezoorp.ino
#endif
#if defined(USE_I2C) && defined(USE_EZORTD)
    feature7 |= 0x00000002;  // xsns_78_ezortd.ino
#endif
#if defined(USE_I2C) && defined(USE_EZOHUM)
    feature7 |= 0x00000004;  // xsns_78_ezohum.ino
#endif
#if defined(USE_I2C) && defined(USE_EZOEC)
    feature7 |= 0x00000008;  // xsns_78_ezoec.ino
#endif
#if defined(USE_I2C) && defined(USE_EZOCO2)
    feature7 |= 0x00000010;  // xsns_78_ezoco2.ino
#endif
#if defined(USE_I2C) && defined(USE_EZOO2)
    feature7 |= 0x00000020;  // xsns_78_ezoo2.ino
#endif
#if defined(USE_I2C) && defined(USE_EZOPRS)
    feature7 |= 0x00000040;  // xsns_78_ezoprs.ino
#endif
#if defined(USE_I2C) && defined(USE_EZOFLO)
    feature7 |= 0x00000080;  // xsns_78_ezoflo.ino
#endif
#if defined(USE_I2C) && defined(USE_EZODO)
    feature7 |= 0x00000100;  // xsns_78_ezodo.ino
#endif
#if defined(USE_I2C) && defined(USE_EZORGB)
    feature7 |= 0x00000200;  // xsns_78_ezorgb.ino
#endif
#if defined(USE_I2C) && defined(USE_EZOPMP)
    feature7 |= 0x00000400;  // xsns_78_ezopmp.ino
#endif
#ifdef USE_AS608
    feature7 |= 0x00000800;  // xsns_79_as608.ino
#endif
#if defined(USE_SHELLY_DIMMER)
    feature7 |= 0x00001000;  // xdrv_45_shelly_dimmer.ino
#endif
#ifdef USE_RC522
    feature7 |= 0x00002000;  // xsns_80_mfrc522.ino
#endif
#ifdef USE_FTC532
    feature7 |= 0x00004000;  // xdrv_47_ftc532.ino
#endif
#if defined(USE_SPI) && defined(USE_DISPLAY) && defined(USE_DISPLAY_EPAPER_42)
    feature7 |= 0x00008000;  // xdsp_06_epaper_42.ino
#endif
// #if defined(USE_SPI) && defined(USE_DISPLAY) && defined(USE_DISPLAY_ILI9488)
//     feature7 |= 0x00010000;  // xdsp_08_ILI9488.ino
// #endif
#if defined(USE_SPI) && defined(USE_DISPLAY) && defined(USE_DISPLAY_SSD1351)
    feature7 |= 0x00020000;  // xdsp_09_SSD1351.ino
#endif
#if defined(USE_SPI) && defined(USE_DISPLAY) && defined(USE_DISPLAY_RA8876)
    feature7 |= 0x00040000;  // xdsp_10_RA8876.ino
#endif
#if defined(USE_SPI) && defined(USE_DISPLAY) && defined(USE_DISPLAY_ST7789)
    feature7 |= 0x00080000;  // xdsp_12_ST7789.ino
#endif
#if defined(USE_SPI) && defined(USE_DISPLAY) && defined(USE_DISPLAY_SSD1331)
    feature7 |= 0x00100000;  // xdsp_14_SSD1331.ino
#endif
#ifdef USE_UFILESYS
    feature7 |= 0x00200000;  // xdrv_50_filesystem.ino
#endif
#ifdef USE_TIMEPROP
    feature7 |= 0x00400000;  // xdrv_48_timeprop.ino
#endif
#ifdef USE_PID
    feature7 |= 0x00800000;  // xdrv_49_pid.ino
#endif
#ifdef USE_BS814A2
    feature7 |= 0x01000000;  // xdrv_51_bs814a2.ino
#endif
#if defined(USE_I2C) && defined(USE_SEESAW_SOIL)
    feature7 |= 0x02000000;  // xsns_81_seesaw_soil.ino
#endif
#ifdef USE_WIEGAND
    feature7 |= 0x04000000;  // xsns_82_wiegand.ino
#endif
#ifdef USE_NEOPOOL
    feature7 |= 0x08000000;  // xsns_83_neopool.ino
#endif
#if defined(USE_I2C) && defined(USE_TOF10120)
    feature7 |= 0x10000000;  // xsns_84_tof10120
#endif
#if defined(USE_ENERGY_SENSOR) && defined(USE_SDM72)
    feature7 |= 0x20000000;  // xnrg_18_sdm72.ino
#endif
#if defined(USE_DISPLAY) && defined(USE_DISPLAY_TM1637)
    feature7 |= 0x40000000;
#endif
#ifdef USE_PROJECTOR_CTRL
    feature7 |= 0x80000000;  // xdrv_53_projector_ctrl.ino
#endif
  }

  static uint32_t feature8 = 0x00000000;
  if (!feature8) {           // Only fill this once
#if defined(USE_I2C) && defined(USE_MPU_ACCEL)
    feature8 |= 0x00000001;  // xsns_85_mpu6886.ino
#endif
#ifdef USE_TFMINIPLUS
    feature8 |= 0x00000002;  // xsns_86_tfminiplus.ino
#endif
#if defined(USE_ENERGY_SENSOR) && defined(USE_CSE7761)
    feature8 |= 0x00000004;  // xnrg_19_cse7761.ino
#endif
#ifdef USE_BERRY
    feature8 |= 0x00000008;  // xdrv_52_9_berry.ino
#endif
#if defined(USE_I2C) && defined(USE_BM8563)
    feature8 |= 0x00000010;  // xdrv_56_BM8563_RTC.ino
#endif
#if defined(USE_ENERGY_SENSOR) && defined(USE_ENERGY_DUMMY)
    feature8 |= 0x00000020;  // xnrg_30_dummy.ino
#endif
#if defined(USE_I2C) && defined(USE_AM2320)
    feature8 |= 0x00000040;  // xsns_88_am2320.ino
#endif
#if defined(USE_I2C) && defined(USE_T67XX)
    feature8 |= 0x00000080;  // xsns_89_t67xx.ino
#endif
#if defined(USE_SPI) && defined(USE_MCP2515)
    feature8 |= 0x00000100;  // xsns_87_mcp2515.ino
#endif
#ifdef USE_TASMESH
    feature8 |= 0x00000200;  // xdrv_57_9_tasmesh.ino
#endif
#ifdef USE_WIFI_RANGE_EXTENDER
    feature8 |= 0x00000400;  // xdrv_58_range_extender.ino
#endif
#ifdef USE_INFLUXDB
    feature8 |= 0x00000800;  // xdrv_59_influxdb.ino
#endif
#ifdef USE_HRG15
    feature8 |= 0x00001000;  // xsns_90_hrg15.ino
#endif
#ifdef USE_VINDRIKTNING
    feature8 |= 0x00002000;  // xsns_91_vindriktning.ino
#endif
#if defined(USE_I2C) && defined(USE_SCD40)
    feature8 |= 0x00004000;  // xsns_92_scd40.ino
#endif
#if defined(USE_I2C) && defined(USE_HM330X)
    feature8 |= 0x00008000;  // xsns_93_hm330x.ino
#endif
#if defined(USE_I2C) && defined(USE_HDC2010)
    feature8 |= 0x00010000;  // xsns_94_hdc2010.ino
#endif
#if defined(USE_LIGHT) && defined(USE_LSC_MCSL)
    feature8 |= 0x00020000;  // xlgt_07_lsc_mcsl.ino
#endif
#ifdef USE_SONOFF_SPM
    feature8 |= 0x00040000;  // xdrv_86_esp32_sonoff_spm.ino
#endif
#ifdef USE_SHIFT595
    feature8 |= 0x00080000;  // xdrv_60_shift595.ino
#endif
#ifdef USE_SDM230
    feature8 |= 0x00100000;  // xnrg_21_sdm230.ino
#endif
#ifdef USE_CM110x
    feature8 |= 0x00200000;  // xsns_95_cm110x.ino
#endif
#if defined(USE_ENERGY_SENSOR) && defined(USE_BL6523)
    feature8 |= 0x00400000;  // xnrg_22_bl6523.ino
#endif
#if defined(USE_ENERGY_SENSOR) && defined(USE_I2C) && defined(USE_ADE7880)
    feature8 |= 0x00800000;  // xnrg_23_ade7880.ino
#endif
#if defined(USE_I2C) && defined(USE_PCF85363)
    feature8 |= 0x01000000;  // xdrv_56_rtc_chips.ino
#endif
#if defined(USE_I2C) && defined(USE_DS3502)
    feature8 |= 0x02000000;  // xdrv_61_ds3502.ino
#endif
#ifdef USE_IMPROV
    feature8 |= 0x04000000;  // xdrv_62_improv.ino
#endif
#ifdef USE_FLOWRATEMETER
    feature8 |= 0x08000000;  // xsns_96_flowratemeter.ino
#endif
#if defined(USE_LIGHT) && defined(USE_BP5758D)
    feature8 |= 0x10000000;  // xlgt_08_bp5758d.ino
#endif
#if defined(USE_I2C) && defined(USE_HYT)
    feature8 |= 0x20000000;  // xsns_97_hyt.ino
#endif
#if defined(USE_LIGHT) && defined(USE_SM2335)
    feature8 |= 0x40000000;  // xlgt_09_sm2335.ino
#endif
#ifdef USE_DISPLAY_TM1621_SONOFF
    feature8 |= 0x80000000;  // xdrv_87_esp32_sonoff_tm1621.ino
#endif
  }

  static uint32_t feature9 = 0x00000000;
  if (!feature9) {           // Only fill this once
#if defined(USE_I2C) && defined(USE_SGP40)
    feature9 |= 0x00000001;  // xsns_98_sgp40.ino
#endif
#if defined(USE_I2C) && defined(USE_LUXV30B)
    feature9 |= 0x00000002;  // xsns_99_luxv30b.ino
#endif
#if defined(USE_SPI) && defined(USE_CANSNIFFER)
    feature9 |= 0x00000004;  // xsns_87_can_sniffer.ino
#endif
#if defined(USE_I2C) && defined(USE_QMC5883L)
    feature9 |= 0x00000008;  // xsns_33_qmc5882l.ino
#endif
#if defined(USE_ENERGY_SENSOR) && defined(USE_MODBUS_ENERGY)
    feature9 |= 0x00000010;  // xnrg_29_modbus.ino
#endif
#if defined(USE_SPI) && defined(USE_SHELLY_PRO)
    feature9 |= 0x00000020;  // xdrv_88_esp32_shelly_pro.ino
#endif
#ifdef USE_DALI
    feature9 |= 0x00000040;  // xdrv_89_esp32_dali.ino
#endif
#if defined(USE_LIGHT) && defined(USE_BP1658CJ)
    feature9 |= 0x00000080;  // xlgt_10_bp1658cj.ino
#endif
#ifdef USE_DINGTIAN_RELAY
    feature9 |= 0x00000100;  // xdrv_90_dingtian_relay.ino
#endif
#if defined(USE_I2C) && defined(USE_HMC5883L)
    feature9 |= 0x00000200;  // xsns_101_hmc5883l.ino
#endif
#ifdef USE_LD2410
    feature9 |= 0x00000400;  // xsns_102_ld2410.ino
#endif
#ifdef USE_ME007
    feature9 |= 0x00000800;  // xsns_23_me007.ino
#endif
#if defined(USE_I2C) && defined(USE_DISPLAY) && defined(USE_DISPLAY_TM1650)
    feature9 |= 0x00001000;  // xdsp_20_tm1650.ino
#endif
#if defined(USE_I2C) && defined(USE_PCA9632)
    feature9 |= 0x00002000;  // xdrv_64_pca9632.ino
#endif
#ifdef USE_TUYAMCUBR
    feature9 |= 0x00004000;  // xdrv_65_tuyamcubr.ino
#endif
#if defined(USE_I2C) && defined(USE_SEN5X)
    feature9 |= 0x00008000;  // xsns_103_sen5x.ino
#endif
#if defined(USE_ENERGY_SENSOR) && defined(USE_BIOPDU)
    feature9 |= 0x00010000;  // xnrg_24_biopdu.ino
#endif
#if (defined(USE_I2C) || defined(USE_SPI)) && defined(USE_MCP23XXX_DRV)
    feature9 |= 0x00020000;  // xdrv_67_mcp23xxx.ino
#endif
#if defined(USE_I2C) && defined(USE_PMSA003I)
    feature9 |= 0x00040000;  // xsns_104_pmsa003i.ino
#endif
#ifdef USE_LOX_O2
    feature9 |= 0x00080000;  // xsns_105_lox_o2.ino
#endif
#if defined(USE_I2C) && defined(USE_GDK101)
    feature9 |= 0x00100000;  // xsns_106_gdk101.ino
#endif
#ifdef USE_GM861
    feature9 |= 0x00200000;  // xsns_107_gm861.ino
#endif
#if defined(USE_I2C) && defined(USE_TC74)
    feature9 |= 0x00400000;  // xsns_108_tc74.ino
#endif
#if defined(USE_I2C) && defined(USE_PCA9557)
    feature9 |= 0x00800000;  // xdrv_69_pca9557.ino
#endif
#if defined(USE_I2C) && defined(USE_SGP4X)
    feature9 |= 0x01000000;  // xdrv_109_sgp4x.ino
#endif
#if defined(USE_I2C) && defined(USE_MAX17043)
    feature9 |= 0x02000000;  // xsns_110_max17043
#endif
#if defined(USE_I2C) && defined(USE_ENS16x)
    feature9 |= 0x04000000;  // xsns_111_ens16x.ino
#endif
#if defined(USE_I2C) && defined(USE_ENS210)
    feature9 |= 0x08000000;  // xsns_112_ens210.ino
#endif
#ifdef USE_HC8
    feature9 |= 0x10000000;  // xsns_113_hc8.ino
#endif
//    feature9 |= 0x20000000;
//    feature9 |= 0x40000000;
//    feature9 |= 0x80000000;
  }

/*********************************************************************************************/

  ResponseAppend_P(PSTR(",\"" D_JSON_FEATURES "\":[\"%08X\",\"%08X\",\"%08X\",\"%08X\",\"%08X\",\"%08X\",\"%08X\",\"%08X\",\"%08X\",\"%08X\"]"),
    LANGUAGE_LCID, feature1, feature2, feature3, feature4, feature5, feature6, feature7, feature8, feature9);
}
