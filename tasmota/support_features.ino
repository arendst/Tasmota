/*
  support_features.ino - feature support for Tasmota

  Copyright (C) 2020  Theo Arends

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

void GetFeatures(void)
{
  feature_drv1 = 0x00000000;

#ifdef USE_ENERGY_MARGIN_DETECTION
  feature_drv1 |= 0x00000001;  // xdrv_03_energy.ino
#endif
#ifdef USE_LIGHT
  feature_drv1 |= 0x00000002;  // tasmota.ino, xdrv_04_light.ino
#endif
#ifdef USE_I2C
  feature_drv1 |= 0x00000004;  // tasmota.ino
#endif
#ifdef USE_SPI
  feature_drv1 |= 0x00000008;  // tasmota.ino
#endif
#ifdef USE_DISCOVERY
  feature_drv1 |= 0x00000010;  // tasmota.ino
#endif
#ifdef USE_ARDUINO_OTA
  feature_drv1 |= 0x00000020;  // tasmota.ino
#endif
#ifdef USE_MQTT_TLS
  feature_drv1 |= 0x00000040;  // xdrv_02_mqtt.ino
#endif
#ifdef USE_WEBSERVER
  feature_drv1 |= 0x00000080;  // xdrv_01_webserver.ino
#endif
#ifdef WEBSERVER_ADVERTISE
  feature_drv1 |= 0x00000100;  // xdrv_01_webserver.ino
#endif
#ifdef USE_EMULATION_HUE
  feature_drv1 |= 0x00000200;  // xdrv_20_hue.ino
#endif
#if (MQTT_LIBRARY_TYPE == MQTT_PUBSUBCLIENT)
  feature_drv1 |= 0x00000400;  // xdrv_02_mqtt.ino
#endif
#if (MQTT_LIBRARY_TYPE == MQTT_TASMOTAMQTT)
//  feature_drv1 |= 0x00000800;  // xdrv_02_mqtt.ino
#endif
#if (MQTT_LIBRARY_TYPE == MQTT_ESPMQTTARDUINO)      // Obsolete since 6.2.1.11
//  feature_drv1 |= 0x00001000;  // xdrv_02_mqtt.ino
#endif
#ifdef MQTT_HOST_DISCOVERY
  feature_drv1 |= 0x00002000;  // xdrv_02_mqtt.ino
#endif
#ifdef USE_ARILUX_RF
  feature_drv1 |= 0x00004000;  // xdrv_04_light.ino
#endif
#if defined(USE_LIGHT) && defined(USE_WS2812)
  feature_drv1 |= 0x00008000;  // xdrv_04_light.ino
#endif
#ifdef USE_WS2812_DMA
  feature_drv1 |= 0x00010000;  // xdrv_04_light.ino
#endif
#if defined(USE_IR_REMOTE) || defined(USE_IR_REMOTE_FULL)
  feature_drv1 |= 0x00020000;  // xdrv_05_irremote.ino
#endif
#ifdef USE_IR_HVAC
  feature_drv1 |= 0x00040000;  // xdrv_05_irremote.ino
#endif
#ifdef USE_IR_RECEIVE
  feature_drv1 |= 0x00080000;  // xdrv_05_irremote.ino
#endif
#ifdef USE_DOMOTICZ
  feature_drv1 |= 0x00100000;  // xdrv_07_domoticz.ino
#endif
#ifdef USE_DISPLAY
  feature_drv1 |= 0x00200000;  // xdrv_13_display.ino
#endif
#ifdef USE_HOME_ASSISTANT
  feature_drv1 |= 0x00400000;  // xdrv_12_home_assistant.ino
#endif
#ifdef USE_SERIAL_BRIDGE
  feature_drv1 |= 0x00800000;  // xdrv_08_serial_bridge.ino
#endif
#ifdef USE_TIMERS
  feature_drv1 |= 0x01000000;  // xdrv_09_timers.ino
#endif
#ifdef USE_SUNRISE
  feature_drv1 |= 0x02000000;  // xdrv_09_timers.ino
#endif
#ifdef USE_TIMERS_WEB
  feature_drv1 |= 0x04000000;  // xdrv_09_timers.ino
#endif
#ifdef USE_RULES
  feature_drv1 |= 0x08000000;  // xdrv_10_rules.ino
#endif
#ifdef USE_KNX
  feature_drv1 |= 0x10000000;  // xdrv_11_knx.ino
#endif
#ifdef USE_WPS
  feature_drv1 |= 0x20000000;  // support.ino - removed with version 6.6.0.21
#endif
#ifdef USE_SMARTCONFIG
  feature_drv1 |= 0x40000000;  // support.ino - removed with version 6.6.0.21
#endif
#ifdef USE_ENERGY_POWER_LIMIT
  feature_drv1 |= 0x80000000;  // xdrv_03_energy.ino
#endif

/*********************************************************************************************/

  feature_drv2 = 0x00000000;

#ifdef USE_CONFIG_OVERRIDE
  feature_drv2 |= 0x00000001;  // user_config(_override).h
#endif
#ifdef FIRMWARE_MINIMAL
  feature_drv2 |= 0x00000002;  // user_config(_override).h
#endif
#ifdef FIRMWARE_SENSORS
  feature_drv2 |= 0x00000004;  // user_config(_override).h
#endif
#ifdef FIRMWARE_CLASSIC
  feature_drv2 |= 0x00000008;  // user_config(_override).h
#endif
#ifdef FIRMWARE_KNX_NO_EMULATION
  feature_drv2 |= 0x00000010;  // user_config(_override).h
#endif
#ifdef USE_DISPLAY_MODES1TO5
  feature_drv2 |= 0x00000020;  // xdrv_13_display.ino
#endif
#ifdef USE_DISPLAY_GRAPH
  feature_drv2 |= 0x00000040;  // xdrv_13_display.ino
#endif
#ifdef USE_DISPLAY_LCD
  feature_drv2 |= 0x00000080;  // xdsp_01_lcd.ino
#endif
#ifdef USE_DISPLAY_SSD1306
  feature_drv2 |= 0x00000100;  // xdsp_02_ssd1306.ino
#endif
#ifdef USE_DISPLAY_MATRIX
  feature_drv2 |= 0x00000200;  // xdsp_03_matrix.ino
#endif
#ifdef USE_DISPLAY_ILI9341
  feature_drv2 |= 0x00000400;  // xdsp_04_ili9341.ino
#endif
#ifdef USE_DISPLAY_EPAPER_29
  feature_drv2 |= 0x00000800;  // xdsp_05_epaper.ino
#endif
#ifdef USE_DISPLAY_SH1106
  feature_drv2 |= 0x00001000;  // xdsp_06_sh1106.ino
#endif
#ifdef USE_MP3_PLAYER
  feature_drv2 |= 0x00002000;  // xdrv_14_mp3.ino
#endif
#ifdef USE_PCA9685
  feature_drv2 |= 0x00004000;  // xdrv_15_pca9685.ino
#endif
#if defined(USE_LIGHT) && defined(USE_TUYA_MCU)
  feature_drv2 |= 0x00008000;  // xdrv_16_tuyadimmer.ino
#endif
#ifdef USE_RC_SWITCH
  feature_drv2 |= 0x00010000;  // xdrv_17_rcswitch.ino
#endif
#if defined(USE_LIGHT) && defined(USE_ARMTRONIX_DIMMERS)
  feature_drv2 |= 0x00020000;  // xdrv_18_armtronixdimmer.ino
#endif
#if defined(USE_LIGHT) && defined(USE_SM16716)
  feature_drv2 |= 0x00040000;  // xdrv_04_light.ino
#endif
#ifdef USE_SCRIPT
  feature_drv2 |= 0x00080000;  // xdrv_10_scripter.ino
#endif
#ifdef USE_EMULATION_WEMO
  feature_drv2 |= 0x00100000;  // xdrv_21_wemo.ino
#endif
#ifdef USE_SONOFF_IFAN
  feature_drv2 |= 0x00200000;  // xdrv_22_sonoff_ifan.ino
#endif
#ifdef USE_ZIGBEE
  feature_drv2 |= 0x00400000;  // xdrv_23_zigbee.ino
#endif
#ifdef NO_EXTRA_4K_HEAP
  feature_drv2 |= 0x00800000;  // sonoff_post.h
#endif
#ifdef VTABLES_IN_IRAM
  feature_drv2 |= 0x01000000;  // platformio.ini
#endif
#ifdef VTABLES_IN_DRAM
  feature_drv2 |= 0x02000000;  // platformio.ini
#endif
#ifdef VTABLES_IN_FLASH
  feature_drv2 |= 0x04000000;  // platformio.ini
#endif
#ifdef PIO_FRAMEWORK_ARDUINO_LWIP_HIGHER_BANDWIDTH
  feature_drv2 |= 0x08000000;  // platformio.ini
#endif
#ifdef PIO_FRAMEWORK_ARDUINO_LWIP2_LOW_MEMORY
  feature_drv2 |= 0x10000000;  // platformio.ini
#endif
#ifdef PIO_FRAMEWORK_ARDUINO_LWIP2_HIGHER_BANDWIDTH
  feature_drv2 |= 0x20000000;  // platformio.ini
#endif
#ifdef DEBUG_THEO
  feature_drv2 |= 0x40000000;  // xdrv_99_debug.ino
#endif
#ifdef USE_DEBUG_DRIVER
  feature_drv2 |= 0x80000000;  // xdrv_99_debug.ino
#endif

/*********************************************************************************************/

  feature_sns1 = 0x00000000;

#ifdef USE_COUNTER
  feature_sns1 |= 0x00000001;  // xsns_01_counter.ino
#endif
#ifdef USE_ADC_VCC
  feature_sns1 |= 0x00000002;  // xsns_02_analog.ino
#endif
#ifdef USE_ENERGY_SENSOR
  feature_sns1 |= 0x00000004;  // xdrv_03_energy.ino
#endif
#ifdef USE_PZEM004T
  feature_sns1 |= 0x00000008;  // xnrg_03_pzem004t.ino
#endif
#ifdef USE_DS18B20
  feature_sns1 |= 0x00000010;  // xsns_05_ds18b20.ino - no more support since 6.6.0.18
#endif
#ifdef USE_DS18x20_LEGACY
  feature_sns1 |= 0x00000020;  // xsns_05_ds18x20_legacy.ino - no more support since 6.6.0.14
#endif
#ifdef USE_DS18x20
  feature_sns1 |= 0x00000040;  // xsns_05_ds18x20.ino
#endif
#ifdef USE_DHT
  feature_sns1 |= 0x00000080;  // xsns_06_dht.ino
#endif
#ifdef USE_SHT
  feature_sns1 |= 0x00000100;  // xsns_07_sht1x.ino
#endif
#ifdef USE_HTU
  feature_sns1 |= 0x00000200;  // xsns_08_htu21.ino
#endif
#ifdef USE_BMP
  feature_sns1 |= 0x00000400;  // xsns_09_bmp.ino
#endif
#ifdef USE_BME680
  feature_sns1 |= 0x00000800;  // xsns_09_bmp.ino - BME680
#endif
#ifdef USE_BH1750
  feature_sns1 |= 0x00001000;  // xsns_10_bh1750.ino
#endif
#ifdef USE_VEML6070
  feature_sns1 |= 0x00002000;  // xsns_11_veml6070.ino
#endif
#ifdef USE_ADS1115_I2CDEV
  feature_sns1 |= 0x00004000;  // xsns_12_ads1115_i2cdev.ino
#endif
#ifdef USE_ADS1115
  feature_sns1 |= 0x00008000;  // xsns_12_ads1115.ino
#endif
#ifdef USE_INA219
  feature_sns1 |= 0x00010000;  // xsns_13_ina219.ino
#endif
#ifdef USE_SHT3X
  feature_sns1 |= 0x00020000;  // xsns_14_sht3x.ino
#endif
#ifdef USE_MHZ19
  feature_sns1 |= 0x00040000;  // xsns_15_mhz19.ino
#endif
#ifdef USE_TSL2561
  feature_sns1 |= 0x00080000;  // xsns_16_tsl2561.ino
#endif
#ifdef USE_SENSEAIR
  feature_sns1 |= 0x00100000;  // xsns_17_senseair.ino
#endif
#ifdef USE_PMS5003
  feature_sns1 |= 0x00200000;  // xsns_18_pms5003.ino
#endif
#ifdef USE_MGS
  feature_sns1 |= 0x00400000;  // xsns_19_mgs.ino
#endif
#ifdef USE_NOVA_SDS
  feature_sns1 |= 0x00800000;  // xsns_20_novasds.ino
#endif
#ifdef USE_SGP30
  feature_sns1 |= 0x01000000;  // xsns_21_sgp30.ino
#endif
#ifdef USE_SR04
  feature_sns1 |= 0x02000000;  // xsns_22_sr04.ino
#endif
#ifdef USE_SDM120
  feature_sns1 |= 0x04000000;  // xnrg_08_sdm120.ino
#endif
#ifdef USE_SI1145
  feature_sns1 |= 0x08000000;  // xsns_24_si1145.ino
#endif
#ifdef USE_SDM630
  feature_sns1 |= 0x10000000;  // xnrg_10_sdm630.ino
#endif
#ifdef USE_LM75AD
  feature_sns1 |= 0x20000000;  // xsns_26_lm75ad.ino
#endif
#ifdef USE_APDS9960
  feature_sns1 |= 0x40000000;  // xsns_27_apds9960.ino
#endif
#ifdef USE_TM1638
  feature_sns1 |= 0x80000000;  // xsns_28_tm1638.ino
#endif

/*********************************************************************************************/

  feature_sns2 = 0x00000000;

#ifdef USE_MCP230xx
  feature_sns2 |= 0x00000001;  // xsns_29_mcp230xx.ino
#endif
#ifdef USE_MPR121
  feature_sns2 |= 0x00000002;  // xsns_30_mpr121.ino
#endif
#ifdef USE_CCS811
  feature_sns2 |= 0x00000004;  // xsns_31_ccs811.ino
#endif
#ifdef USE_MPU6050
  feature_sns2 |= 0x00000008;  // xsns_32_mpu6050.ino
#endif
#ifdef USE_MCP230xx_OUTPUT
  feature_sns2 |= 0x00000010;  // xsns_29_mcp230xx.ino
#endif
#ifdef USE_MCP230xx_DISPLAYOUTPUT
  feature_sns2 |= 0x00000020;  // xsns_29_mcp230xx.ino
#endif
#ifdef USE_HLW8012
  feature_sns2 |= 0x00000040;  // xnrg_01_hlw8012.ino
#endif
#ifdef USE_CSE7766
  feature_sns2 |= 0x00000080;  // xnrg_02_cse7766.ino
#endif
#ifdef USE_MCP39F501
  feature_sns2 |= 0x00000100;  // xnrg_04_mcp39f501.ino
#endif
#ifdef USE_PZEM_AC
  feature_sns2 |= 0x00000200;  // xnrg_05_pzem_ac.ino
#endif
#ifdef USE_DS3231
  feature_sns2 |= 0x00000400;  // xsns_33_ds3231.ino
#endif
#ifdef USE_HX711
  feature_sns2 |= 0x00000800;  // xsns_34_hx711.ino
#endif
#ifdef USE_PZEM_DC
  feature_sns2 |= 0x00001000;  // xnrg_06_pzem_dc.ino
#endif
#if defined(USE_TX20_WIND_SENSOR) || defined(USE_TX23_WIND_SENSOR)
  feature_sns2 |= 0x00002000;  // xsns_35_tx20.ino
#endif
#ifdef USE_MGC3130
  feature_sns2 |= 0x00004000;  // xsns_36_mgc3130.ino
#endif
#ifdef USE_RF_SENSOR
  feature_sns2 |= 0x00008000;  // xsns_37_rfsensor.ino
#endif
#ifdef USE_THEO_V2
  feature_sns2 |= 0x00010000;  // xsns_37_rfsensor.ino
#endif
#ifdef USE_ALECTO_V2
  feature_sns2 |= 0x00020000;  // xsns_37_rfsensor.ino
#endif
#ifdef USE_AZ7798
  feature_sns2 |= 0x00040000;  // xsns_38_az7798.ino
#endif
#ifdef USE_MAX31855
  feature_sns2 |= 0x00080000;  // xsns_39_max31855.ino
#endif
#ifdef USE_PN532_HSU
  feature_sns2 |= 0x00100000;  // xsns_40_pn532.ino
#endif
#ifdef USE_MAX44009
  feature_sns2 |= 0x00200000;  // xsns_41_max44009.ino
#endif
#ifdef USE_SCD30
  feature_sns2 |= 0x00400000;  // xsns_42_scd30.ino
#endif
#ifdef USE_HRE
  feature_sns2 |= 0x00800000;  // xsns_43_hre.ino
#endif
#ifdef USE_ADE7953
  feature_sns2 |= 0x01000000;  // xnrg_07_ade7953.ino
#endif
#ifdef USE_SPS30
  feature_sns2 |= 0x02000000;  // xsns_44_sps30.ino
#endif
#ifdef USE_VL53L0X
  feature_sns2 |= 0x04000000;  // xsns_45_vl53l0x.ino
#endif
#ifdef USE_MLX90614
  feature_sns2 |= 0x08000000;  // xsns_46_MLX90614.ino
#endif
#ifdef USE_MAX31865
  feature_sns2 |= 0x10000000;  // xsns_47-max31865.ino
#endif
#ifdef USE_CHIRP
  feature_sns2 |= 0x20000000;  // xsns_48_chirp.ino
#endif
#ifdef USE_SOLAX_X1
  feature_sns2 |= 0x40000000;  // xnrg_12_solaxX1.ino
#endif
#ifdef USE_PAJ7620
  feature_sns2 |= 0x80000000;  // xsns_50_paj7620.ino
#endif

/*********************************************************************************************/

  feature5 = 0x00000000;

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
#ifdef USE_INA226
  feature5 |= 0x00000010;  // xsns_54_ina226.ino
#endif
#ifdef USE_A4988_STEPPER
  feature5 |= 0x00000020;  // xdrv_25_A4988.ino
#endif
#ifdef USE_DDS2382
  feature5 |= 0x00000040;  // xnrg_09_dds2382.ino
#endif
#ifdef USE_SM2135
  feature5 |= 0x00000080;  // xdrv_026_sm2135.ino
#endif
#ifdef USE_SHUTTER
  feature5 |= 0x00000100;  // xdrv_027_shutter.ino
#endif
#ifdef USE_PCF8574
  feature5 |= 0x00000200;  // xdrv_028_pcf8574.ino
#endif
#ifdef USE_DDSU666
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
#ifdef USE_SONOFF_L1
  feature5 |= 0x00004000;  // xlgt_05_sonoff_l1.ino
#endif
#ifdef USE_EXS_DIMMER
  feature5 |= 0x00008000;  // xdrv_30_exs_dimmer.ino
#endif
#ifdef USE_ARDUINO_SLAVE
  feature5 |= 0x00010000;  // xdrv_31_arduino_slave.ino
#endif
#ifdef USE_HIH6
  feature5 |= 0x00020000;  // xsns_55_hih_series.ino
#endif
#ifdef USE_HPMA
  feature5 |= 0x00040000;  // xsns_56_hpma.ino
#endif
#ifdef USE_TSL2591
  feature5 |= 0x00080000;  // xsns_57_tsl2591.ino
#endif
#ifdef USE_DHT12
  feature5 |= 0x00100000;  // xsns_58_dht12.ino
#endif
#ifdef USE_DS1624
  feature5 |= 0x00200000;  // xsns_59_ds1624.ino
#endif
#ifdef USE_GPS
  feature5 |= 0x00400000;  // xsns_60_GPS.ino
#endif
#ifdef USE_HOTPLUG
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
#ifdef USE_LE01MR
  feature5 |= 0x08000000;  // xnrg_13_fif_le01mr.ino
#endif
#ifdef USE_AHT1x
  feature5 |= 0x10000000;  // xsns_63_aht1x.ino
#endif
#ifdef USE_WEMOS_MOTOR_V1
  feature5 |= 0x20000000;  // xdrv_34_wemos_motor_v1.ino
#endif
#ifdef USE_DEVICE_GROUPS
  feature5 |= 0x40000000;  // support_device_groups.ino
#endif
#ifdef USE_PWM_DIMMER
  feature5 |= 0x80000000;  // xdrv_35_pwm_dimmer
#endif

/*********************************************************************************************/

  feature6 = 0x00000000;

//  feature6 |= 0x00000001;
//  feature6 |= 0x00000002;
//  feature6 |= 0x00000004;
//  feature6 |= 0x00000008;

//  feature6 |= 0x00000010;
//  feature6 |= 0x00000020;
//  feature6 |= 0x00000040;
//  feature6 |= 0x00000080;

//  feature6 |= 0x00000100;
//  feature6 |= 0x00000200;
//  feature6 |= 0x00000400;
//  feature6 |= 0x00000800;

//  feature6 |= 0x00001000;
//  feature6 |= 0x00002000;
//  feature6 |= 0x00004000;
//  feature6 |= 0x00008000;

//  feature6 |= 0x00010000;
//  feature6 |= 0x00020000;
//  feature6 |= 0x00040000;
//  feature6 |= 0x00080000;

//  feature6 |= 0x00100000;
//  feature6 |= 0x00200000;
//  feature6 |= 0x00400000;
//  feature6 |= 0x00800000;

//  feature6 |= 0x01000000;
//  feature6 |= 0x02000000;
//  feature6 |= 0x04000000;
//  feature6 |= 0x08000000;

//  feature6 |= 0x10000000;
//  feature6 |= 0x20000000;
//  feature6 |= 0x40000000;
//  feature6 |= 0x80000000;

}
