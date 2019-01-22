/*
  support_features.ino - feature support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

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
  feature_drv1 = 0x00000000;   // xdrv_01_mqtt.ino, xdrv_01_light.ino, xdrv_04_snfbridge.ino

//  feature_drv1 |= 0x00000001;
//  feature_drv1 |= 0x00000002;

#ifdef USE_I2C
  feature_drv1 |= 0x00000004;  // sonoff.ino
#endif
#ifdef USE_SPI
  feature_drv1 |= 0x00000008;  // sonoff.ino
#endif
#ifdef USE_DISCOVERY
  feature_drv1 |= 0x00000010;  // sonoff.ino
#endif
#ifdef USE_ARDUINO_OTA
  feature_drv1 |= 0x00000020;  // sonoff.ino
#endif
#ifdef USE_MQTT_TLS
  feature_drv1 |= 0x00000040;  // sonoff.ino
#endif
#ifdef USE_WEBSERVER
  feature_drv1 |= 0x00000080;  // xdrv_02_webserver.ino
#endif
#ifdef WEBSERVER_ADVERTISE
  feature_drv1 |= 0x00000100;  // xdrv_02_webserver.ino
#endif
#ifdef USE_EMULATION
  feature_drv1 |= 0x00000200;  // xplg_wemohue.ino
#endif
#if (MQTT_LIBRARY_TYPE == MQTT_PUBSUBCLIENT)
  feature_drv1 |= 0x00000400;  // xdrv_01_mqtt.ino
#endif
#if (MQTT_LIBRARY_TYPE == MQTT_TASMOTAMQTT)
  feature_drv1 |= 0x00000800;  // xdrv_01_mqtt.ino
#endif
#if (MQTT_LIBRARY_TYPE == MQTT_ESPMQTTARDUINO)      // Obsolete since 6.2.1.11
  feature_drv1 |= 0x00001000;  // xdrv_01_mqtt.ino
#endif
#ifdef MQTT_HOST_DISCOVERY
  feature_drv1 |= 0x00002000;  // xdrv_01_mqtt.ino
#endif
#ifdef USE_ARILUX_RF
  feature_drv1 |= 0x00004000;  // xdrv_04_light.ino
#endif
#ifdef USE_WS2812
  feature_drv1 |= 0x00008000;  // xdrv_04_light.ino
#endif
#ifdef USE_WS2812_DMA
  feature_drv1 |= 0x00010000;  // xdrv_04_light.ino
#endif
#ifdef USE_IR_REMOTE
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
  feature_drv1 |= 0x20000000;  // support.ino
#endif
#ifdef USE_SMARTCONFIG
  feature_drv1 |= 0x40000000;  // support.ino
#endif
#if (MQTT_LIBRARY_TYPE == MQTT_ARDUINOMQTT)
  feature_drv1 |= 0x80000000;  // xdrv_01_mqtt.ino
#endif

/*********************************************************************************************/

  feature_drv2 = 0x00000000;

#ifdef USE_CONFIG_OVERRIDE
  feature_drv2 |= 0x00000001;  // user_config(_override).h
#endif
#ifdef BE_MINIMAL
  feature_drv2 |= 0x00000002;  // user_config(_override).h
#endif
#ifdef USE_SENSORS
  feature_drv2 |= 0x00000004;  // user_config(_override).h
#endif
#ifdef USE_CLASSIC
  feature_drv2 |= 0x00000008;  // user_config(_override).h
#endif
#ifdef USE_KNX_NO_EMULATION
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
#ifdef USE_TUYA_DIMMER
  feature_drv2 |= 0x00008000;  // xdrv_16_tuyadimmer.ino
#endif
#ifdef USE_RC_SWITCH
  feature_drv2 |= 0x00010000;  // xdrv_17_rcswitch.ino
#endif
#ifdef USE_ARMTRONIX_DIMMERS
  feature_drv2 |= 0x00020000;  // xdrv_18_armtronixdimmer.ino
#endif

//  feature_drv2 |= 0x00040000;
//  feature_drv2 |= 0x00080000;
//  feature_drv2 |= 0x00100000;
//  feature_drv2 |= 0x00200000;
//  feature_drv2 |= 0x00400000;

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

  feature_sns1 = 0x00000000;   // xsns_01_counter.ino, xsns_04_snfsc.ino

//  feature_sns1 |= 0x00000001;

#ifdef USE_ADC_VCC
  feature_sns1 |= 0x00000002;  // support.ino (ADC)
#endif
#ifdef USE_ENERGY_SENSOR
  feature_sns1 |= 0x00000004;  // xdrv_03_energy.ino
#endif
#ifdef USE_PZEM004T
  feature_sns1 |= 0x00000008;  // xnrg_03_pzem004t.ino
#endif
#ifdef USE_DS18B20
  feature_sns1 |= 0x00000010;  // xsns_05_ds18b20.ino
#endif
#ifdef USE_DS18x20_LEGACY
  feature_sns1 |= 0x00000020;  // xsns_05_ds18x20_legacy.ino
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
  feature_sns1 |= 0x04000000;  // xsns_23_sdm120.ino
#endif
#ifdef USE_SI1145
  feature_sns1 |= 0x08000000;  // xsns_24_si1145.ino
#endif
#ifdef USE_SDM630
  feature_sns1 |= 0x10000000;  // xsns_25_sdm630.ino
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
#ifdef USE_TX20_WIND_SENSOR
  feature_sns2 |= 0x00002000;  // xsns_35_tx20.ino
#endif
#ifdef USE_MGC3130
  feature_sns2 |= 0x00004000;  // xsns_36_mgc3130.ino
#endif
#ifdef USE_RF_SENSOR
  feature_sns2 |= 0x00008000;  // xsns_37_rfsensor.ino
#endif
#ifdef USE_THEO_V2
  feature_sns2 |= 0x00010000;
#endif
#ifdef USE_ALECTO_V2
  feature_sns2 |= 0x00020000;
#endif
#ifdef USE_AZ7798
  feature_sns2 |= 0x00040000;
#endif
//  feature_sns2 |= 0x00080000;
//  feature_sns2 |= 0x00100000;
//  feature_sns2 |= 0x00200000;
//  feature_sns2 |= 0x00400000;
//  feature_sns2 |= 0x00800000;
//  feature_sns2 |= 0x01000000;
//  feature_sns2 |= 0x02000000;
//  feature_sns2 |= 0x04000000;
//  feature_sns2 |= 0x08000000;
//  feature_sns2 |= 0x10000000;
//  feature_sns2 |= 0x20000000;
//  feature_sns2 |= 0x40000000;
//  feature_sns2 |= 0x80000000;

}
