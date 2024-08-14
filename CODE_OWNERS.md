<picture>
  <source media="(prefers-color-scheme: dark)" srcset="./tools/logo/TASMOTA_FullLogo_Vector_White.svg">
  <img alt="Logo" src="./tools/logo/TASMOTA_FullLogo_Vector.svg" align="right" height="76">
</picture>

# Code Owners

In addition to @arendst the following code is mainly owned by:

| Code                         | Owner
|------------------------------|---------------------------
| Tasmota Drivers              |
|                              |
| xdrv_01_webserver            | @arendst
| xdrv_02_mqtt                 | @arendst
| xdrv_03_energy               | @arendst
| xdrv_04_light                | @s-hadinger
| xdrv_05_irremote             | @s-hadinger
| xdrv_06_snfbridge            | @arendst
| xdrv_07_domoticz             | @arendst
| xdrv_08_serial_bridge        | Dániel Zoltán Tolnai
| xdrv_09_timers               | @arendst
| xdrv_10_rules                | @arendst, @barbudor
| xdrv_10_scripter             | @gemu
| xdrv_11_knx                  | @adrian
| xdrv_12_discovery            | @arendst, @effelle, @emontnemery
| xdrv_13_display              | @gemu, @s-hadinger
| xdrv_14_mp3                  | @gemu, @mike2nl
| xdrv_15_pca9685              | Andre Thomas
| xdrv_16_tuyamcu              | @btsimonh
| xdrv_17_rcswitch             |
| xdrv_18_armtronix_dimmers    | @wvdv2002
| xdrv_19_pz16dz_dimmer        | Joel Stein
| xdrv_20_hue                  | @s-hadinger
| xdrv_21_wemo                 | @s-hadinger
| xdrv_22_sonoff_ifan          | @arendst
| xdrv_23_zigbee               | @s-hadinger
| xdrv_24_buzzer               | @arendst
| xdrv_25_stepper              | Tim Leuscher
| xdrv_26_ariluxrf             | @arendst
| xdrv_27_shutter              | @stefanbode
| xdrv_28_pcf8574              | @stefanbode
| xdrv_29_deepsleep            | @stefanbode
| xdrv_30_exs_dimmer           | Andreas Schultz
| xdrv_31_tasmota_client       | Andre Thomas
| xdrv_32_hotplug              | @BASM
| xdrv_33_nrf24l01             | @staars
| xdrv_34_wemos_motor          | Peter Franck
| xdrv_35_pwm_dimmer           | Paul C Diem
| xdrv_36_keeloq               | he-so
| xdrv_37_sonoff_d1            | @arendst
| xdrv_38_ping                 | @s-hadinger
| xdrv_39_thermostat           | Javier Argita
| xdrv_40_telegram             | @arendst
| xdrv_41_tcp_bridge           | @s-hadinger
| xdrv_42_i2s_audio            | @gemu
| xdrv_43_mlx90640             | @staars
| xdrv_44_miel_hvac            | David GWynne
| xdrv_45_shelly_dimmer        | James Turton
| xdrv_46_ccloader             | @staars
| xdrv_47_ftc532               | Peter Franck
| xdrv_48_timeprop             | Colin Law, Thomas Herrmann
| xdrv_49_pid                  | Colin Law, Thomas Herrmann
| xdrv_50_filesystem           | @gemu, @barbudor
| xdrv_51_bs814a2              | Peter Franck
| xdrv_52_berry                | @s-hadinger, @staars (ULP)
| xdrv_53_projector_ctrl       | Jan Bubík
| xdrv_54_lvgl                 | @s-hadinger
| xdrv_55_touch                | @gemu, @s-hadinger
| xdrv_56_rtc_chips            | @arendst, @s-hadinger
| xdrv_57_tasmesh              | @staars
| xdrv_58_range_extender       | @sillyfrog
| xdrv_59_influxdb             | @arendst
| xdrv_60_shift595             | Jacek Ziółkowski
| xdrv_61_ds3502               | f-reiling
| xdrv_62_improv               | @arendst
| xdrv_63_modbus_bridge        | @jeroenst
| xdrv_64_pca9632              | Pascal Heinrich
| xdrv_65_tuyamcubr            | David Gwynne
| xdrv_66_tm1638               | @arendst
| xdrv_67_mcp23xxx             | @arendst
| xdrv_68_zerocrossDimmer      | @stefanbode
| xdrv_69_pca9557              | @cctweaker
| xdrv_70_1_hdmi_cec           | @s-hadinger
| xdrv_71_magic_switch         | @barbudor
| xdrv_72_pipsolar             | @chefpro
| xdrv_73_lora                 | @arendst
| xdrv_74                      |
| xdrv_75                      |
| xdrv_76                      |
| xdrv_77                      |
| xdrv_78                      |
| xdrv_79_esp32_ble            | @staars, @btsimonh
| xdrv_81_esp32_webcam         | @gemu, @philrich
| xdrv_82_esp32_ethernet       | @arendst
| xdrv_83_esp32_watch          | @gemu
| xdrv_85_esp32_ble_eq3_trv    | @btsimonh
| xdrv_86_esp32_sonoff_spm     | @arendst
| xdrv_87_esp32_sonoff_tm1621  | @arendst
| xdrv_88_esp32_shelly_pro     | @arendst
| xdrv_89_esp32_dali           | @eeak
| xdrv_90_esp32_dingtian_relay | @barbudor
| xdrv_91_                     |
| xdrv_92_                     |
| xdrv_93_                     |
| xdrv_94_                     |
|                              |
| xdrv_121_gpioviewer          | @arendst
| xdrv_122_file_settings_demo  | @arendst
| xdrv_122_file_json_settings_demo | @arendst
| xdrv_127_debug               | @arendst
|                              |
| Tasmota Sensors              |
|                              |
| xsns_01_counter              | @arendst, @stefanbode
| xsns_02_analog               | @arendst, @barbudor
| xsns_03_energy               | @arendst
| xsns_04_snfsc                | @arendst
| xsns_05_ds18x20              | @arendst
| xsns_06_dht                  | @arendst
| xsns_07_sht1x                | @arendst
| xsns_08_htu                  | Heiko Krupp
| xsns_09_bmp                  | @arendst
| xsns_10_bh1750               | @arendst
| xsns_11_veml6070             | @mike2nl
| xsns_12_ads1115              | @syssi, @stefanbode
| xsns_13_ina219               | @stefanbode
| xsns_14_sht3x                | Stefan Tibus
| xsns_15_mhz19                | @arendst
| xsns_16_tsl2561              | Joachim Banzhaf
| xsns_17_senseair             | @arendst
| xsns_18_pms5003              | @arendst
| xsns_19_mgs                  | @palich2000
| xsns_20_novasds              | Norbert Richter
| xsns_21_sgp30                | Gerhard Mutz
| xsns_22_sr04                 | Nuno Ferreira, @arendst
| xsns_23_me007                | Mathias Buder
| xsns_24_si1145               |
| xsns_25_spl06-007_sensor     | @rai68
| xsns_26_lm75ad               | Andre Thomas
| xsns_27_apds9960             | Shawn Hymel
| xsns_28_qmp6988              | @arendst
| xsns_29_mcp230xx             | Andre Thomas
| xsns_30_mpr121               | Rene 'Renne' Bartsch
| xsns_31_ccs811               | Gerhard Mutz
| xsns_32_mpu6050              | Oliver Welter
| xsns_33_qmc5883l             | Helge Scheunemann
| xsns_34_hx711                | @arendst
| xsns_35_tx20                 | Thomas Eckerstorfer, Norbert Richter
| xsns_36_mgc3130              | Christian Baars
| xsns_37_rfsensor             | @arendst
| xsns_38_az7798               | @adebeun
| xsns_39_max31855             | Markus Past
| xsns_40_pn532                | Andre Thomas, @md5sum-as
| xsns_41_max44009             | @llagendijk
| xsns_42_scd30                | @frogmore42
| xsns_43_hre                  | Jon Little
| xsns_44_sps30                | Gerhard Mutz
| xsns_45_vl53l0x              | Gerhard Mutz, Adrian Scillato
| xsns_46_mlx90614             | Gerhard Mutz
| xsns_47_max31865             | Alberto Lopez Siemens
| xsns_48_chirp                | Christian Baars
| xsns_49                      |
| xsns_50_paj7620              | Christian Baars
| xsns_51_rdm6300              | Gerhard Mutz
| xsns_52_esp32_ibeacon        | Gerhard Mutz, @btsimonh
| xsns_52_ibeacon              | Gerhard Mutz
| xsns_53_sml                  | Gerhard Mutz
| xsns_54_ina226               | Stephen Rodgers
| xsns_55_hih_series           |
| xsns_56_hpma                 | David Hunt
| xsns_57_tsl2591              | Markus Bösling
| xsns_58_dht12                | Stefan Oskam
| xsns_59_ds1624               | Leonid Myravje
| xsns_60_gps                  | Christian Baars, Adrian Scillato
| xsns_61_mi_nrf24             | Christian Baars
| xsns_62_mi_hm10              | Christian Baars
| xsns_62_esp32_mi             | Christian Baars
| xsns_63_aht1x                | Martin Wagner
| xsns_64_hrxl                 | Jon Little
| xsns_65_hdc1080              | Luis Teixeira
| xsns_66_iaq                  | Christian Baars
| xsns_67_as3935               | Martin Wagner
| xsns_68_windmeter            | Matteo Albinola
| xsns_69_opentherm            | Yuriy Sannikov
| xsns_70_veml6075             | Martin Wagner
| xsns_71_veml7700             | Martin Wagner
| xsns_72_mcp9808              | Martin Wagner
| xsns_73_hp303b               | @rjaakke
| xsns_74_lmt01                | @justifiably
| xsns_75_prometheus           | @marius, @mhansen, @hansmi
| xsns_76_dyp                  | Janusz Kostorz
| xsns_77_vl53l1x              | Rui Marinho, @Jason2866
| xsns_78_ezo                  | Christopher Tremblay
| xsns_79_as608                | @boaschti
| xsns_80_mfrc522              | @arendst
| xsns_81_seesaw_soil          | Wayne Ross, Peter Franck
| xsns_82_wiegand              | Sigurd Leuther
| xsns_83_neopool              | Norbert Richter
| xsns_84_tof10120             | Cyril Pawelko
| xsns_85_mpu6886              | @s-hadinger
| xsns_86_tfminiplus           | Raphael Breiting
| xsns_87_can_sniffer          | @kwiatek6324, Marius Bezuidenhout
| xsns_87_mcp2515              | Marius Bezuidenhout
| xsns_88_am2320               | Lars Wessels
| xsns_89_t67xx                | Alexander Savchenko
| xsns_90_hrg15                | Wouter Breukink
| xsns_91_vindriktning         | Marcel Ritter
| xsns_92_scd40                | @frogmore42, @arnold-n
| xsns_93_hm330x               | @barbudor
| xsns_94_hdc2010              | Luc Boudreau
| xsns_95_cm1107               | @maksim
| xsns_96_flowratemeter        | Norbert Richter
| xsns_97_hyt                  | Thomas Schnittcher, Adjan Kretz
| xsns_98_sgp40                | Jean-Pierre Deschamps
| xsns_99_luxv30b              | Marius Bezuidenhout
| xsns_100_ina3221             | @barbudor
| xsns_101_hmc5883l            | Andreas Achtzehn
| xsns_102_ld2410              | @arendst
| xsns_103_sen5x               | @tyeth
| xsns_104_pmsa003i            | Jean-Pierre Deschamps
| xsns_105_lox_o2              | @ACE1046
| xsns_106_gdk101              | @Szewcson
| xsns_107_gm861               | @arendst
| xsns_108_tc74                | Michael Loftis
| xsns_109_sgp4x               | Andrew Klaus
| xsns_110_max17043            | Vincent de Groot
| xsns_111_ens16x              | Christoph Friese
| xsns_112_ens210              | Christoph Friese
| xsns_113_hc8                 | Daniel Maier
| xsns_114_amsx915             | Bastian Urschel
| xsns_115_wooliis             | Luca Melette
|                              |
| xsns_127_esp32_sensors       | @arendst
|                              |
| Libraries                    |
|                              |
| berry                        | @s-hadinger
| ext-printf                   | @s-hadinger
| jsmn                         | @s-hadinger
| unishox                      | @s-hadinger
| matter                       | @s-hadinger
|                              |
| PlatformIO                   |
|                              |
| all                          | @Jason2866
|                              |

