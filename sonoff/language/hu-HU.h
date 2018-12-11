/*
  hu-HU.h - localization for Hungarian in Hungary for Sonoff-Tasmota

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

#ifndef _LANGUAGE_HU_HU_H_
#define _LANGUAGE_HU_HU_H_

/*************************** ATTENTION *******************************\
 *
 * Due to memory constraints only UTF-8 is supported.
 * To save code space keep text as short as possible.
 * Time and Date provided by SDK can not be localized (yet).
 * Use online command StateText to translate ON, OFF, HOLD and TOGGLE.
 * Use online command Prefix to translate cmnd, stat and tele.
 *
 * Updated until v5.12.0e
\*********************************************************************/

//#define LANGUAGE_MODULE_NAME         // Enable to display "Module Generic" (ie Spanish), Disable to display "Generic Module" (ie English)

#define LANGUAGE_LCID 1038
// HTML (ISO 639-1) Language Code
#define D_HTML_LANGUAGE "hu"

// "2017-03-07T11:08:02" - ISO8601:2004
#define D_YEAR_MONTH_SEPARATOR "."
#define D_MONTH_DAY_SEPARATOR "."
#define D_DATE_TIME_SEPARATOR " "
#define D_HOUR_MINUTE_SEPARATOR ":"
#define D_MINUTE_SECOND_SEPARATOR ":"

#define D_DAY3LIST "VasHétKedSzeCsüPénSzo"
#define D_MONTH3LIST "JanFebMárÁprMájJúnJúlAugSzeOktNovDec"

// Non JSON decimal separator
#define D_DECIMAL_SEPARATOR "."

// Common
#define D_ADMIN "Admin"
#define D_AIR_QUALITY "Levegő minőség"
#define D_AP "AP"                    // Access Point
#define D_AS "mint"
#define D_AUTO "AUTO"
#define D_BLINK "Villogás"
#define D_BLINKOFF "Villogás Ki"
#define D_BOOT_COUNT "Bootolások száma"
#define D_BRIGHTLIGHT "Max. fényerő"
#define D_BSSID "BSSId"
#define D_BUTTON "Gomb"
#define D_BY "tőle:"                    // Written by me
#define D_BYTES "Byte-ok"
#define D_CELSIUS "Celsius"
#define D_CHANNEL "Channel"
#define D_CO2 "Szén-dioxid"
#define D_CODE "kód"                // Button code
#define D_COLDLIGHT "Hideg"
#define D_COMMAND "Parancs"
#define D_CONNECTED "Csatlakozva"
#define D_COUNT "Szám"
#define D_COUNTER "Számláló"
#define D_CURRENT "Áram"          // As in Voltage and Current
#define D_DATA "Adat"
#define D_DARKLIGHT "Min. fényerő"
#define D_DEBUG "Debug"
#define D_DISABLED "Letiltva"
#define D_DISTANCE "Distance"
#define D_DNS_SERVER "DNS Szerver"
#define D_DONE "Kész"
#define D_DST_TIME "DST"
#define D_ECO2 "eCO2"
#define D_EMULATION "Emuláció"
#define D_ENABLED "Engedélyezve"
#define D_ERASE "Törlés"
#define D_ERROR "Hiba"
#define D_FAHRENHEIT "Fahrenheit"
#define D_FAILED "Sikertelen"
#define D_FALLBACK "Fallback"
#define D_FALLBACK_TOPIC "Fallback Téma"
#define D_FALSE "Hamis"
#define D_FILE "File"
#define D_FREE_MEMORY "Szabad Memória"
#define D_FREQUENCY "Frequency"
#define D_GAS "Gáz"
#define D_GATEWAY "Gateway"
#define D_GROUP "Csoport"
#define D_HOST "Host"
#define D_HOSTNAME "Hostname"
#define D_HUMIDITY "Páratartalom"
#define D_ILLUMINANCE "Megvilágítás"
#define D_IMMEDIATE "azonnali"      // Button immediate
#define D_INDEX "Index"
#define D_INFO "Info"
#define D_INFRARED "Infrared"
#define D_INITIALIZED "Inicializálva"
#define D_IP_ADDRESS "IP Cím"
#define D_LIGHT "Fény"
#define D_LWT "LWT"
#define D_MODULE "Modul"
#define D_MQTT "MQTT"
#define D_MULTI_PRESS "több lenyomás"
#define D_NOISE "Zaj"
#define D_NONE "nincs"
#define D_OFF "Ki"
#define D_OFFLINE "Offline"
#define D_OK "Ok"
#define D_ON "Be"
#define D_ONLINE "Online"
#define D_PASSWORD "Jelszó"
#define D_PORT "Port"
#define D_POWER_FACTOR "Teljesítmény tényező"
#define D_POWERUSAGE "Energiafelhasználás"
#define D_POWERUSAGE_ACTIVE "Active Power"
#define D_POWERUSAGE_APPARENT "Apparent Power"
#define D_POWERUSAGE_REACTIVE "Reactive Power"
#define D_PRESSURE "Nyomás"
#define D_PRESSUREATSEALEVEL "Tengerszinti nyomás"
#define D_PROGRAM_FLASH_SIZE "Program Flash Méret"
#define D_PROGRAM_SIZE "Program Méret"
#define D_PROJECT "Projekt"
#define D_RAIN "Rain"
#define D_RECEIVED "Érkezett"
#define D_RESTART "Újraindítás"
#define D_RESTARTING "Újraindítás"
#define D_RESTART_REASON "Újraindítás oka:"
#define D_RESTORE "Visszaállítás"
#define D_RETAINED "mentve"
#define D_RULE "Rule"
#define D_SAVE "Mentés"
#define D_SENSOR "Szenzor"
#define D_SSID "SSId"
#define D_START ""
#define D_STD_TIME "STD"
#define D_STOP "Leállítás"
#define D_SUBNET_MASK "Subnet Mask"
#define D_SUBSCRIBE_TO "Feliratkozás a"
#define D_SUCCESSFUL "Sikeres"
#define D_SUNRISE "Sunrise"
#define D_SUNSET "Sunset"
#define D_TEMPERATURE "Hőmérséklet"
#define D_TO "-nak"
#define D_TOGGLE "Toggle"
#define D_TOPIC "Téma"
#define D_TRANSMIT "Továbbít"
#define D_TRUE "Igaz"
#define D_TVOC "TVOC"
#define D_UPGRADE "frissítés"
#define D_UPLOAD "Feltöltés"
#define D_UPTIME "Üzemidő"
#define D_USER "Felhasználó"
#define D_UTC_TIME "UTC"
#define D_UV_INDEX "UV Index"
#define D_UV_INDEX_1 "Low"
#define D_UV_INDEX_2 "Mid"
#define D_UV_INDEX_3 "High"
#define D_UV_INDEX_4 "Danger"
#define D_UV_INDEX_5 "BurnL1/2"
#define D_UV_INDEX_6 "BurnL3"
#define D_UV_INDEX_7 "OoR"
#define D_UV_LEVEL "UV Szint"
#define D_UV_POWER "UV Power"
#define D_VERSION "Verzió"
#define D_VOLTAGE "Feszültség"
#define D_WEIGHT "Weight"
#define D_WARMLIGHT "Meleg"
#define D_WEB_SERVER "Web Szerver"

// sonoff.ino
#define D_WARNING_MINIMAL_VERSION "VIGYÁZZ Ez a verzió nem támogat tartós beállításokat"
#define D_LEVEL_10 "szint 1-0"
#define D_LEVEL_01 "szint 0-1"
#define D_SERIAL_LOGGING_DISABLED "Serial logolás kikapcsolva"
#define D_SYSLOG_LOGGING_REENABLED "Syslog logolás újra-engedélyezve"

#define D_SET_BAUDRATE_TO "Baudrate beállítása"
#define D_RECEIVED_TOPIC "Érkezett Téma"
#define D_DATA_SIZE "Adat Méret"
#define D_ANALOG_INPUT "Analóg"

// support.ino
#define D_OSWATCH "osWatch"
#define D_BLOCKED_LOOP "Blocked Loop"
#define D_WPS_FAILED_WITH_STATUS "WPSconfig SIKERTELEN státusz:"
#define D_ACTIVE_FOR_3_MINUTES "aktválás 3 percre"
#define D_FAILED_TO_START "sikertelen indítás"
#define D_PATCH_ISSUE_2186 "Patch issue 2186"
#define D_CONNECTING_TO_AP "Csatlakozás az (AP): "
#define D_IN_MODE "be mód"
#define D_CONNECT_FAILED_NO_IP_ADDRESS "Sikertelen csatlakozás, nincs kiosztott IP cím"
#define D_CONNECT_FAILED_AP_NOT_REACHED "Sikertelen csatlakozás, AP nem elérhető"
#define D_CONNECT_FAILED_WRONG_PASSWORD "Sikertelen csatlakozás, hibás AP jelszó"
#define D_CONNECT_FAILED_AP_TIMEOUT "Sikertelen csatlakozás AP időtúllépés miatt"
#define D_ATTEMPTING_CONNECTION "Csatlakozás..."
#define D_CHECKING_CONNECTION "Kapcsolat ellenőrzése..."
#define D_QUERY_DONE "Lekérés kész. MQTT szolgáltatás aktív"
#define D_MQTT_SERVICE_FOUND "élő MQTT szolgáltatás a"
#define D_FOUND_AT "a"
#define D_SYSLOG_HOST_NOT_FOUND "Syslog Host nem található"

// settings.ino
#define D_SAVED_TO_FLASH_AT "Flash-re mentve a"
#define D_LOADED_FROM_FLASH_AT "Flash-ről betöltve a"
#define D_USE_DEFAULTS "Alapértelmezett beáll. használata"
#define D_ERASED_SECTOR "Szektor törlése"

// xdrv_02_webserver.ino
#define D_NOSCRIPT "To use Tasmota, please enable JavaScript"
#define D_MINIMAL_FIRMWARE_PLEASE_UPGRADE "MINIMAL firmware - frissítsd!"
#define D_WEBSERVER_ACTIVE_ON "Web szerver aktív a"
#define D_WITH_IP_ADDRESS "IP címe:"
#define D_WEBSERVER_STOPPED "Webs zerver leállítva"
#define D_FILE_NOT_FOUND "File Nem Található"
#define D_REDIRECTED "Átírányítás captive portálra"
#define D_WIFIMANAGER_SET_ACCESSPOINT_AND_STATION "Wifimanager AccessPoint(AP) és Station(ST) beállítása"
#define D_WIFIMANAGER_SET_ACCESSPOINT "Wifimanager AccessPoint(AP) beállítása"
#define D_TRYING_TO_CONNECT "Csatlakozás a hálózatra..."

#define D_RESTART_IN "Újraindítás..."
#define D_SECONDS "másodperc"
#define D_DEVICE_WILL_RESTART "Az eszköz hamarosan újraindul"
#define D_BUTTON_TOGGLE "Toggle"
#define D_CONFIGURATION "Konfigurációk"
#define D_INFORMATION "Információ"
#define D_FIRMWARE_UPGRADE "Firmware Frissítés"
#define D_CONSOLE "Konzol"
#define D_CONFIRM_RESTART "Újraindítás megerősítése"

#define D_CONFIGURE_MODULE "Eszköz konfiguráció"
#define D_CONFIGURE_WIFI "WiFi konfiguráció"
#define D_CONFIGURE_MQTT "MQTT konfiguráció"
#define D_CONFIGURE_DOMOTICZ "Domoticz konfiguráció"
#define D_CONFIGURE_LOGGING "Logolás konfiguráció"
#define D_CONFIGURE_OTHER "Egyéb konfiguráció"
#define D_CONFIRM_RESET_CONFIGURATION "Konfig resetelés megerősítve?"
#define D_RESET_CONFIGURATION "Konfiguráció reset"
#define D_BACKUP_CONFIGURATION "Konfiguráció backup"
#define D_RESTORE_CONFIGURATION "Konfiguráció visszaállítás"
#define D_MAIN_MENU "Menü"

#define D_MODULE_PARAMETERS "Modul paraméterek"
#define D_MODULE_TYPE "Modul típus"
#define D_GPIO "GPIO"
#define D_SERIAL_IN "Serial In"
#define D_SERIAL_OUT "Serial Out"

#define D_WIFI_PARAMETERS "Wifi paraméterek"
#define D_SCAN_FOR_WIFI_NETWORKS "Wifi hálózat keresése"
#define D_SCAN_DONE "Keresés kész"
#define D_NO_NETWORKS_FOUND "Nincs elérhető hálózat"
#define D_REFRESH_TO_SCAN_AGAIN "Frissíts az újra kereséshez"
#define D_DUPLICATE_ACCESSPOINT "Duplicate AccessPoint"
#define D_SKIPPING_LOW_QUALITY "Kihagyás, alacsony jelminőség"
#define D_RSSI "RSSI"
#define D_WEP "WEP"
#define D_WPA_PSK "WPA PSK"
#define D_WPA2_PSK "WPA2 PSK"
#define D_AP1_SSID "AP1 SSID"
#define D_AP1_PASSWORD "AP1 Jelszó"
#define D_AP2_SSID "AP2 SSID"
#define D_AP2_PASSWORD "AP2 Jelszó"

#define D_MQTT_PARAMETERS "MQTT paraméterek"
#define D_CLIENT "Kliens"
#define D_FULL_TOPIC "Teljes téma"

#define D_LOGGING_PARAMETERS "Logolás paraméterek"
#define D_SERIAL_LOG_LEVEL "Serial logolás szint"
#define D_WEB_LOG_LEVEL "Web logolás szint"
#define D_SYS_LOG_LEVEL "Syslog szint"
#define D_MORE_DEBUG "Részletes debug"
#define D_SYSLOG_HOST "Syslog host"
#define D_SYSLOG_PORT "Syslog port"
#define D_TELEMETRY_PERIOD "Telemetria (sec)"

#define D_OTHER_PARAMETERS "Egyéb paraméterek"
#define D_WEB_ADMIN_PASSWORD "Web Admin Jelszó"
#define D_MQTT_ENABLE "MQTT engedélyezés"
#define D_FRIENDLY_NAME "Név"
#define D_BELKIN_WEMO "Belkin WeMo"
#define D_HUE_BRIDGE "Hue Bridge"
#define D_SINGLE_DEVICE "single device"
#define D_MULTI_DEVICE "multi device"

#define D_SAVE_CONFIGURATION "Konfiguráció mentése"
#define D_CONFIGURATION_SAVED "Konfiguráció elmentve"
#define D_CONFIGURATION_RESET "Konfiguráció visszaállítása"

#define D_PROGRAM_VERSION "Program Verzió"
#define D_BUILD_DATE_AND_TIME "Build Dátum & Idő"
#define D_CORE_AND_SDK_VERSION "Core/SDK Verzió"
#define D_FLASH_WRITE_COUNT "Flashelések száma"
#define D_MAC_ADDRESS "MAC Cím"
#define D_MQTT_HOST "MQTT Host"
#define D_MQTT_PORT "MQTT Port"
#define D_MQTT_CLIENT "MQTT Cliens"
#define D_MQTT_USER "MQTT Felhasználó"
#define D_MQTT_TOPIC "MQTT Téma"
#define D_MQTT_GROUP_TOPIC "MQTT Csoport Téma"
#define D_MQTT_FULL_TOPIC "MQTT Teljes téma"
#define D_MDNS_DISCOVERY "mDNS Láthatóság"
#define D_MDNS_ADVERTISE "mDNS Hírdetés"
#define D_ESP_CHIP_ID "ESP Chip Id"
#define D_FLASH_CHIP_ID "Flash Chip Id"
#define D_FLASH_CHIP_SIZE "Flash Méret"
#define D_FREE_PROGRAM_SPACE "Szabad Program Hely"

#define D_UPGRADE_BY_WEBSERVER "Frissítés web szerverrel"
#define D_OTA_URL "OTA Url"
#define D_START_UPGRADE "Frissítés"
#define D_UPGRADE_BY_FILE_UPLOAD "Frissítés file feltöltéssel"
#define D_UPLOAD_STARTED "Feltöltés elindítva"
#define D_UPGRADE_STARTED "Frissítés elindítva"
#define D_UPLOAD_DONE "Feltöltés kész"
#define D_UPLOAD_ERR_1 "Nincs file kijelölve"
#define D_UPLOAD_ERR_2 "Nincs elég memória"
#define D_UPLOAD_ERR_3 "Magic byte is not 0xE9"
#define D_UPLOAD_ERR_4 "Program flash méret nagyobb a valós flash méretnél"
#define D_UPLOAD_ERR_5 "Feltöltés buffer hiba"
#define D_UPLOAD_ERR_6 "Feltöltés sikertelen. Endegélyezz 3-mas logolást"
#define D_UPLOAD_ERR_7 "Feltöltés megszakítva"
#define D_UPLOAD_ERR_8 "Érvénytelen file"
#define D_UPLOAD_ERR_9 "File túl nagy"
#define D_UPLOAD_ERR_10 "Failed to init RF chip"
#define D_UPLOAD_ERR_11 "Failed to erase RF chip"
#define D_UPLOAD_ERR_12 "Failed to write to RF chip"
#define D_UPLOAD_ERR_13 "Failed to decode RF firmware"
#define D_UPLOAD_ERROR_CODE "Feltöltés hiba kód"

#define D_ENTER_COMMAND "Parancsolj"
#define D_ENABLE_WEBLOG_FOR_RESPONSE "Engedélyezz 2-es weblogolást több információért"
#define D_NEED_USER_AND_PASSWORD "Kell felhasználó=<felhasználónév>&jelszó=<jelszó>"

// xdrv_01_mqtt.ino
#define D_FINGERPRINT "TLS fingerprint hitelesítése..."
#define D_TLS_CONNECT_FAILED_TO "TLS Csatlakozás sikertelen a"
#define D_RETRY_IN "Újrapróbálás"
#define D_VERIFIED "Hitelesítve Fingerprint"
#define D_INSECURE "Nem biztonságos kapcsolat érvénytelen Fingerprint miatt"
#define D_CONNECT_FAILED_TO "Sikertelen csatlakozás a"

// xplg_wemohue.ino
#define D_MULTICAST_DISABLED "Multicast kikapcsolva"
#define D_MULTICAST_REJOINED "Multicast (újra)csatlakozás"
#define D_MULTICAST_JOIN_FAILED "Multicast csatlakozás sikertelen"
#define D_FAILED_TO_SEND_RESPONSE "Nem sikerült választ küldeni"

#define D_WEMO "WeMo"
#define D_WEMO_BASIC_EVENT "WeMo basic event"
#define D_WEMO_EVENT_SERVICE "WeMo event service"
#define D_WEMO_META_SERVICE "WeMo meta service"
#define D_WEMO_SETUP "WeMo beállítás"
#define D_RESPONSE_SENT "Válasz elküldve"

#define D_HUE "Hue"
#define D_HUE_BRIDGE_SETUP "Hue beállítás"
#define D_HUE_API_NOT_IMPLEMENTED "Hue API nincs implementálva"
#define D_HUE_API "Hue API"
#define D_HUE_POST_ARGS "Hue POST args"
#define D_3_RESPONSE_PACKETS_SENT "3 válaszcsomagok elküldve"

// xdrv_07_domoticz.ino
#define D_DOMOTICZ_PARAMETERS "Domoticz paraméterek"
#define D_DOMOTICZ_IDX "Idx"
#define D_DOMOTICZ_KEY_IDX "Key idx"
#define D_DOMOTICZ_SWITCH_IDX "Kapcsoló idx"
#define D_DOMOTICZ_SENSOR_IDX "Szenzor idx"
  #define D_DOMOTICZ_TEMP "Temp"
  #define D_DOMOTICZ_TEMP_HUM "Hőm,Párat"
  #define D_DOMOTICZ_TEMP_HUM_BARO "Hőm,Párat,Légny"
  #define D_DOMOTICZ_POWER_ENERGY "Teljesítmény,Energia"
  #define D_DOMOTICZ_ILLUMINANCE "Fényerő"
  #define D_DOMOTICZ_COUNT "Szám/PM1"
  #define D_DOMOTICZ_VOLTAGE "Feszültség/PM2.5"
  #define D_DOMOTICZ_CURRENT "Áram/PM10"
  #define D_DOMOTICZ_AIRQUALITY "Légminőség"
#define D_DOMOTICZ_UPDATE_TIMER "Update időzítő"

// xdrv_09_timers.ino
#define D_CONFIGURE_TIMER "Configure Timer"
#define D_TIMER_PARAMETERS "Timer parameters"
#define D_TIMER_ENABLE "Enable Timers"
#define D_TIMER_ARM "Arm"
#define D_TIMER_TIME "Time"
#define D_TIMER_DAYS "Days"
#define D_TIMER_REPEAT "Repeat"
#define D_TIMER_OUTPUT "Output"
#define D_TIMER_ACTION "Action"

// xdrv_10_knx.ino
#define D_CONFIGURE_KNX "Configure KNX"
#define D_KNX_PARAMETERS "KNX Parameters"
#define D_KNX_GENERAL_CONFIG "General"
#define D_KNX_PHYSICAL_ADDRESS "Physical Address"
#define D_KNX_PHYSICAL_ADDRESS_NOTE "( Must be unique on the KNX network )"
#define D_KNX_ENABLE "Enable KNX"
#define D_KNX_GROUP_ADDRESS_TO_WRITE "Data to Send to Group Addresses"
#define D_ADD "Add"
#define D_DELETE "Delete"
#define D_REPLY "Reply"
#define D_KNX_GROUP_ADDRESS_TO_READ "Group Addresses to Receive Data from"
#define D_LOG_KNX "KNX: "
#define D_RECEIVED_FROM "Received from"
#define D_KNX_COMMAND_WRITE "Write"
#define D_KNX_COMMAND_READ "Read"
#define D_KNX_COMMAND_OTHER "Other"
#define D_SENT_TO "sent to"
#define D_KNX_WARNING "The group address ( 0 / 0 / 0 ) is reserved and can not be used."
#define D_KNX_ENHANCEMENT "Communication Enhancement"
#define D_KNX_TX_SLOT "KNX TX"
#define D_KNX_RX_SLOT "KNX RX"

// xdrv_03_energy.ino
#define D_ENERGY_TODAY "Mai Energia"
#define D_ENERGY_YESTERDAY "Tegnapi Energia"
#define D_ENERGY_TOTAL "Összes Energia"

// xsns_05_ds18b20.ino
#define D_SENSOR_BUSY "Szenzor elfoglalt"
#define D_SENSOR_CRC_ERROR "Szenzor CRC hiba"
#define D_SENSORS_FOUND "Szenzorok megtalálva"

// xsns_06_dht.ino
#define D_TIMEOUT_WAITING_FOR "Timeout waiting for"
#define D_START_SIGNAL_LOW "start signal low"
#define D_START_SIGNAL_HIGH "start signal high"
#define D_PULSE "pulse"
#define D_CHECKSUM_FAILURE "Checksum failure"

// xsns_07_sht1x.ino
#define D_SENSOR_DID_NOT_ACK_COMMAND "Szenzor nem ismerte el(ACK) a parancsot"
#define D_SHT1X_FOUND "SHT1X found"

// xsns_18_pms5003.ino
#define D_STANDARD_CONCENTRATION "CF-1 PM"     // Standard Particle CF-1 Particle Matter
#define D_ENVIRONMENTAL_CONCENTRATION "PM"     // Environmetal Particle Matter
#define D_PARTICALS_BEYOND "Részecskék"

// xsns_32_mpu6050.ino
#define D_AX_AXIS "Accel. X-Axis"
#define D_AY_AXIS "Accel. Y-Axis"
#define D_AZ_AXIS "Accel. Z-Axis"
#define D_GX_AXIS "Gyro X-Axis"
#define D_GY_AXIS "Gyro Y-Axis"
#define D_GZ_AXIS "Gyro Z-Axis"

// xsns_34_hx711.ino
#define D_HX_CAL_REMOVE "Remove weigth"
#define D_HX_CAL_REFERENCE "Load reference weigth"
#define D_HX_CAL_DONE "Calibrated"
#define D_HX_CAL_FAIL "Calibration failed"
#define D_RESET_HX711 "Reset Scale"
#define D_CONFIGURE_HX711 "Configure Scale"
#define D_HX711_PARAMETERS "Scale parameters"
#define D_ITEM_WEIGHT "Item weight"
#define D_REFERENCE_WEIGHT "Reference weigth"
#define D_CALIBRATE "Calibrate"
#define D_CALIBRATION "Calibration"

//xsns_35_tx20.ino
#define D_TX20_WIND_DIRECTION "Wind Direction"
#define D_TX20_WIND_SPEED "Wind Speed"
#define D_TX20_WIND_SPEED_AVG "Wind Speed Avg"
#define D_TX20_WIND_SPEED_MAX "Wind Speed Max"
#define D_TX20_NORTH "N"
#define D_TX20_EAST "E"
#define D_TX20_SOUTH "S"
#define D_TX20_WEST "W"

// sonoff_template.h
#define D_SENSOR_NONE     "Nincs"
#define D_SENSOR_DHT11    "DHT11"
#define D_SENSOR_AM2301   "AM2301"
#define D_SENSOR_SI7021   "SI7021"
#define D_SENSOR_DS18X20  "DS18x20"
#define D_SENSOR_I2C_SCL  "I2C SCL"
#define D_SENSOR_I2C_SDA  "I2C SDA"
#define D_SENSOR_WS2812   "WS2812"
#define D_SENSOR_DFR562   "MP3 Player"
#define D_SENSOR_IRSEND   "IRadó"
#define D_SENSOR_SWITCH   "Kapcsoló"   // Suffix "1"
#define D_SENSOR_BUTTON   "Gomb"   // Suffix "1"
#define D_SENSOR_RELAY    "Relé"    // Suffix "1i"
#define D_SENSOR_LED      "Led"      // Suffix "1i"
#define D_SENSOR_PWM      "PWM"      // Suffix "1"
#define D_SENSOR_COUNTER  "Számláló"  // Suffix "1"
#define D_SENSOR_IRRECV   "IRvevő"
#define D_SENSOR_MHZ_RX   "MHZ Rx"
#define D_SENSOR_MHZ_TX   "MHZ Tx"
#define D_SENSOR_PZEM004_RX  "PZEM004 Rx"
#define D_SENSOR_PZEM016_RX  "PZEM016 Rx"
#define D_SENSOR_PZEM017_RX  "PZEM017 Rx"
#define D_SENSOR_PZEM0XX_TX  "PZEM0XX Tx"
#define D_SENSOR_SAIR_RX  "SAir Rx"
#define D_SENSOR_SAIR_TX  "SAir Tx"
#define D_SENSOR_SPI_CS   "SPI CS"
#define D_SENSOR_SPI_DC   "SPI DC"
#define D_SENSOR_BACKLIGHT "Háttérvil"
#define D_SENSOR_PMS5003  "PMS5003"
#define D_SENSOR_SDS0X1_RX "SDS0X1 Rx"
#define D_SENSOR_SDS0X1_TX "SDS0X1 Tx"
#define D_SENSOR_SBR_RX   "SerBr Rx"
#define D_SENSOR_SBR_TX   "SerBr Tx"
#define D_SENSOR_SR04_TRIG "SR04 Tri"
#define D_SENSOR_SR04_ECHO "SR04 Ech"
#define D_SENSOR_SDM120_TX "SDM120/220 Tx"
#define D_SENSOR_SDM120_RX "SDM120/220 Rx"
#define D_SENSOR_SDM630_TX "SDM630 Tx"
#define D_SENSOR_SDM630_RX "SDM630 Rx"
#define D_SENSOR_TM1638_CLK "TM16 CLK"
#define D_SENSOR_TM1638_DIO "TM16 DIO"
#define D_SENSOR_TM1638_STB "TM16 STB"
#define D_SENSOR_HX711_SCK "HX711 SCK"
#define D_SENSOR_HX711_DAT "HX711 DAT"
#define D_SENSOR_TX20_TX "TX20"
#define D_SENSOR_RFSEND "RFSend"
#define D_SENSOR_RFRECV "RFrecv"
#define D_SENSOR_TUYA_TX "Tuya Tx"
#define D_SENSOR_TUYA_RX "Tuya Rx"
#define D_SENSOR_MGC3130_XFER "MGC3130 Xfer"
#define D_SENSOR_MGC3130_RESET "MGC3130 Reset"
#define D_SENSOR_SSPI_MISO "SSPI MISO"
#define D_SENSOR_SSPI_MOSI "SSPI MOSI"
#define D_SENSOR_SSPI_SCLK "SSPI SCLK"
#define D_SENSOR_SSPI_CS   "SSPI CS"
#define D_SENSOR_SSPI_DC   "SSPI DC"
#define D_SENSOR_RF_SENSOR "RF Sensor"

// Units
#define D_UNIT_AMPERE "A"
#define D_UNIT_CENTIMETER "cm"
#define D_UNIT_HERTZ "Hz"
#define D_UNIT_HOUR "ó"
#define D_UNIT_INCREMENTS "inc"
#define D_UNIT_KILOGRAM "kg"
#define D_UNIT_KILOMETER_PER_HOUR "km/h"  // or "km/h"
#define D_UNIT_KILOOHM "kOhm"
#define D_UNIT_KILOWATTHOUR "kWh"
#define D_UNIT_LUX "lx"
#define D_UNIT_MICROGRAM_PER_CUBIC_METER "ug/m3"
#define D_UNIT_MICROMETER "um"
#define D_UNIT_MICROSECOND "µs"
#define D_UNIT_MILLIAMPERE "mA"
#define D_UNIT_MILLIMETER "mm"
#define D_UNIT_MILLIMETER_MERCURY "mmHg"
#define D_UNIT_MILLISECOND "ms"
#define D_UNIT_MINUTE "p"
#define D_UNIT_PARTS_PER_BILLION "ppb"
#define D_UNIT_PARTS_PER_DECILITER "ppd"
#define D_UNIT_PARTS_PER_MILLION "ppm"
#define D_UNIT_PRESSURE "hPa"
#define D_UNIT_SECOND "m"
#define D_UNIT_SECTORS "szektorok"
#define D_UNIT_VA "VA"
#define D_UNIT_VAR "VAr"
#define D_UNIT_VOLT "V"
#define D_UNIT_WATT "W"
#define D_UNIT_WATTHOUR "Wh"
#define D_UNIT_WATT_METER_QUADRAT "W/m²"

// Log message prefix
#define D_LOG_APPLICATION "APP: "  // Application
#define D_LOG_BRIDGE "BRG: "       // Bridge
#define D_LOG_CONFIG "CFG: "       // Settings
#define D_LOG_COMMAND "CMD: "      // Command
#define D_LOG_DEBUG "DBG: "        // Debug
#define D_LOG_DHT "DHT: "          // DHT sensor
#define D_LOG_DOMOTICZ "DOM: "     // Domoticz
#define D_LOG_DSB "DSB: "          // DS18xB20 sensor
#define D_LOG_HTTP "HTP: "         // HTTP webserver
#define D_LOG_I2C "I2C: "          // I2C
#define D_LOG_IRR "IRR: "          // Infra Red Received
#define D_LOG_LOG "LOG: "          // Logging
#define D_LOG_MODULE "MOD: "       // Module
#define D_LOG_MDNS "DNS: "         // mDNS
#define D_LOG_MQTT "MQT: "         // MQTT
#define D_LOG_OTHER "OTH: "        // Other
#define D_LOG_RESULT "RSL: "       // Result
#define D_LOG_RFR "RFR: "          // RF Received
#define D_LOG_SERIAL "SER: "       // Serial
#define D_LOG_SHT1 "SHT: "         // SHT1x sensor
#define D_LOG_UPLOAD "UPL: "       // Upload
#define D_LOG_UPNP "UPP: "         // UPnP
#define D_LOG_WIFI "WIF: "         // Wifi

//SDM220
#define D_PHASE_ANGLE     "Phase Angle"
#define D_IMPORT_ACTIVE   "Import Active"
#define D_EXPORT_ACTIVE   "Export Active"
#define D_IMPORT_REACTIVE "Import Reactive"
#define D_EXPORT_REACTIVE "Export Reactive"
#define D_TOTAL_REACTIVE  "Total Reactive"
#define D_UNIT_KWARH      "kVArh"
#define D_UNIT_ANGLE      "Deg"

#endif  // _LANGUAGE_HU_HU_H_
