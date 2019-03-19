/*
  sk-SK.h - localization for Slovak with diacritics - Slovak for Sonoff-Tasmota

  Copyright (C) 2019  Vladimír Jendroľ

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

#ifndef _LANGUAGE_SK_SK_H_
#define _LANGUAGE_SK_SK_H_

/*************************** ATTENTION *******************************\
 *
 * Due to memory constraints only UTF-8 is supported.
 * To save code space keep text as short as possible.
 * Time and Date provided by SDK can not be localized (yet).
 * Use online command StateText to translate ON, OFF, HOLD and TOGGLE.
 * Use online command Prefix to translate cmnd, stat and tele.
 *
 * Updated until v6.2.1.14
\*********************************************************************/

//#define LANGUAGE_MODULE_NAME         // Enable to display "Module Generic" (ie Spanish), Disable to display "Generic Module" (ie English)

#define LANGUAGE_LCID 1029
// HTML (ISO 639-1) Language Code
#define D_HTML_LANGUAGE "sk"

// "2017-03-07T11:08:02" - ISO8601:2004
#define D_YEAR_MONTH_SEPARATOR "-"
#define D_MONTH_DAY_SEPARATOR "-"
#define D_DATE_TIME_SEPARATOR "T"
#define D_HOUR_MINUTE_SEPARATOR ":"
#define D_MINUTE_SECOND_SEPARATOR ":"

#define D_DAY3LIST "Ne Po Ut St Št Pi So"
#define D_MONTH3LIST "JanFebMarAprMájJúnJúlAugSepOktNovDec"

// Non JSON decimal separator
#define D_DECIMAL_SEPARATOR ","

// Common
#define D_ADMIN "Admin"
#define D_AIR_QUALITY "Kvalita vzduchu"
#define D_AP "AP"                    // Access Point
#define D_AS "ako"
#define D_AUTO "AUTO"
#define D_BLINK "Blikanie"
#define D_BLINKOFF "BlikanieVyp"
#define D_BOOT_COUNT "Počítadlo spustení"
#define D_BRIGHTLIGHT "Svetlý"
#define D_BSSID "BSSId"
#define D_BUTTON "Tlačídlo"
#define D_BY "by"                    // Written by me
#define D_BYTES "Bytov"
#define D_CELSIUS "°C"
#define D_CHANNEL "Kanál"
#define D_CO2 "CO2"
#define D_CODE "kód"                 // Button code
#define D_COLDLIGHT "Studené svetlo"
#define D_COMMAND "Príkaz"
#define D_CONNECTED "...pripojené"
#define D_COUNT "Počítaj"
#define D_COUNTER "Počítadlo"
#define D_CURRENT "Prúd"          // As in Voltage and Current
#define D_DATA "Dáta"
#define D_DARKLIGHT "Tmavý"
#define D_DEBUG "Debug"
#define D_DISABLED "Zablokované"
#define D_DISTANCE "Vzdialenosť"
#define D_DNS_SERVER "Server DNS"
#define D_DONE "Hotovo"
#define D_DST_TIME "DST"
#define D_ECO2 "eCO2"
#define D_EMULATION "Emulácia"
#define D_ENABLED "Otvorený"
#define D_ERASE "Zmaž"
#define D_ERROR "Chyba"
#define D_FAHRENHEIT "Fahrenheita"
#define D_FAILED "chyba"
#define D_FALLBACK "Záložný"
#define D_FALLBACK_TOPIC "Záložný topic"
#define D_FALSE "Nepravda"
#define D_FILE "Súbor"
#define D_FREE_MEMORY "Voľná pamäť"
#define D_FREQUENCY "Frekvencia"
#define D_GAS "Plyn"
#define D_GATEWAY "Predvolená brána"
#define D_GROUP "Skupina"
#define D_HOST "Server"
#define D_HOSTNAME "Názov servera"
#define D_HUMIDITY "Vlhkosť"
#define D_ILLUMINANCE "Osvetlenie"
#define D_IMMEDIATE "Okamžité"      // Button immediate
#define D_INDEX "Index"
#define D_INFO "Informácie"
#define D_INFRARED "Infračervené"
#define D_INITIALIZED "Inicializovaný"
#define D_IP_ADDRESS "Adresa IP"
#define D_LIGHT "Svetlo"
#define D_LWT "LWT"
#define D_MODULE "Modul"
#define D_MQTT "MQTT"
#define D_MULTI_PRESS "multi-stlačenie"
#define D_NOISE "Hluk"
#define D_NONE "Žiadny"
#define D_OFF "Vyp."
#define D_OFFLINE "Neaktívny"
#define D_OK "OK"
#define D_ON "Zap."
#define D_ONLINE "Aktívny"
#define D_PASSWORD "Heslo"
#define D_PORT "Port"
#define D_POWER_FACTOR "Účinník"
#define D_POWERUSAGE "Príkon"
#define D_POWERUSAGE_ACTIVE "Činný príkon"
#define D_POWERUSAGE_APPARENT "Zdanlivý príkon"
#define D_POWERUSAGE_REACTIVE "Jalový príkon"
#define D_PRESSURE "Tlak"
#define D_PRESSUREATSEALEVEL "Tlak na hladine mora"
#define D_PROGRAM_FLASH_SIZE "Veľkosť flash pamäte"
#define D_PROGRAM_SIZE "Veľkosť programu"
#define D_PROJECT "Projekt"
#define D_RAIN "Dážď"
#define D_RECEIVED "Prijatý"
#define D_RESTART "Reštart"
#define D_RESTARTING "Reštartuje sa"
#define D_RESTART_REASON "Príčina reštartu"
#define D_RESTORE "Obnoviť"
#define D_RETAINED "Zachované"
#define D_RULE "Pravidlo"
#define D_SAVE "Ulož"
#define D_SENSOR "Senzor"
#define D_SSID "SSID"
#define D_START "Spustiť"
#define D_STD_TIME "STD"
#define D_STOP "Stop"
#define D_SUBNET_MASK "Maska podsiete"
#define D_SUBSCRIBE_TO "Prihlásiť do"
#define D_UNSUBSCRIBE_FROM "Unsubscribe from"
#define D_SUCCESSFUL "úspešné."
#define D_SUNRISE "Svitanie"
#define D_SUNSET "Súmrak"
#define D_TEMPERATURE "Teplota"
#define D_TO "do"
#define D_TOGGLE "Prepni"
#define D_TOPIC "Topic"
#define D_TRANSMIT "Odošli"
#define D_TRUE "Pravda"
#define D_TVOC "TVOC"
#define D_UPGRADE "aktualizáciu"
#define D_UPLOAD "Nahrávanie..."
#define D_UPTIME "Uptime"
#define D_USER "Používateľ"
#define D_UTC_TIME "UTC"
#define D_UV_INDEX "UV Index"
#define D_UV_INDEX_1 "Nízky"
#define D_UV_INDEX_2 "Stredný"
#define D_UV_INDEX_3 "Vysoký"
#define D_UV_INDEX_4 "Nebezpečný"
#define D_UV_INDEX_5 "Popál1/2"
#define D_UV_INDEX_6 "Popál3"
#define D_UV_INDEX_7 "MimoRozsah"
#define D_UV_LEVEL "úroveň UV"
#define D_UV_POWER "UV Power"
#define D_VERSION "Verzia"
#define D_VOLTAGE "Napätie"
#define D_WEIGHT "Hmotnosť"
#define D_WARMLIGHT "Teplé svetlo"
#define D_WEB_SERVER "Web Server"

// sonoff.ino
#define D_WARNING_MINIMAL_VERSION "UPOZORNENIE Táto verzia nepodporuje trvalé nastavenia"
#define D_LEVEL_10 "úroveň 1-0"
#define D_LEVEL_01 "úroveň 0-1"
#define D_SERIAL_LOGGING_DISABLED "Logovanie na sériovom porte ukončené"
#define D_SYSLOG_LOGGING_REENABLED "Obnovený zápis do Syslog"

#define D_SET_BAUDRATE_TO "Nastaviť rýchlosti prenosu na"
#define D_RECEIVED_TOPIC "Prijatý topic"
#define D_DATA_SIZE "Veľkosť dát"
#define D_ANALOG_INPUT "Analógový vstup"

// support.ino
#define D_OSWATCH "osWatch"
#define D_BLOCKED_LOOP "Zablokovanie slučky"
#define D_WPS_FAILED_WITH_STATUS "Chyba WPSconfig so statusom"
#define D_ACTIVE_FOR_3_MINUTES "aktívny 3 minúty"
#define D_FAILED_TO_START "nepodarilo sa spustiť"
#define D_PATCH_ISSUE_2186 "Chyba 2186"
#define D_CONNECTING_TO_AP "Pripájanie k AP"
#define D_IN_MODE "v režime"
#define D_CONNECT_FAILED_NO_IP_ADDRESS "Chyba pripojenia, nebola obdržaná IP adresa"
#define D_CONNECT_FAILED_AP_NOT_REACHED "Chyba pripojenia, nedostupný AP"
#define D_CONNECT_FAILED_WRONG_PASSWORD "Chyba pripojenia, nesprávne heslo pre AP"
#define D_CONNECT_FAILED_AP_TIMEOUT "Chyba pripojenia, uplynul AP timeout"
#define D_ATTEMPTING_CONNECTION "Pripájanie..."
#define D_CHECKING_CONNECTION "Skúška spojenia..."
#define D_QUERY_DONE "Vyslanie požiadavky. Nájdená služba MQTT"
#define D_MQTT_SERVICE_FOUND "Služba MQTT bola nájdená"
#define D_FOUND_AT "nájdené v"
#define D_SYSLOG_HOST_NOT_FOUND "Syslog Host nebol nájdený"

// settings.ino
#define D_SAVED_TO_FLASH_AT "Uložené do flash pamäte v"
#define D_LOADED_FROM_FLASH_AT "Stiahnuté z flash pamäte z"
#define D_USE_DEFAULTS "Použi prednastavené hodnoty"
#define D_ERASED_SECTOR "Zmazaný sektor"

// xdrv_02_webserver.ino
#define D_NOSCRIPT "Pre používanie prostredia Tasmota povoľte JavaScript"
#define D_MINIMAL_FIRMWARE_PLEASE_UPGRADE "Firmware MINIMÁLNY<br/>prosím aktualizujte"
#define D_WEBSERVER_ACTIVE_ON "Aktívny Web server"
#define D_WITH_IP_ADDRESS "na IP adrese"
#define D_WEBSERVER_STOPPED "Web server zastavený"
#define D_FILE_NOT_FOUND "Súbor nebol nájdený"
#define D_REDIRECTED "Presmerovanie na vlastný portál"
#define D_WIFIMANAGER_SET_ACCESSPOINT_AND_STATION "Wi-Fi manažér nastaví AP a zachová STA"
#define D_WIFIMANAGER_SET_ACCESSPOINT "Wi-Fi manažér nastaví Access Point"
#define D_TRYING_TO_CONNECT "Skúšam pripojiť zariadenie k sieti"

#define D_RESTART_IN "Reštart"
#define D_SECONDS "sekúnd"
#define D_DEVICE_WILL_RESTART "Zariadenie bude reštartované behom niekoľkých sekúnd"
#define D_BUTTON_TOGGLE "Prepínač"
#define D_CONFIGURATION "Nastavenia"
#define D_INFORMATION "Informácie"
#define D_FIRMWARE_UPGRADE "Aktualizácia firmware"
#define D_CONSOLE "Konzola"
#define D_CONFIRM_RESTART "Potvrdenie reštartu"

#define D_CONFIGURE_MODULE "Nastavenia modulu"
#define D_CONFIGURE_WIFI "Nastavenia WiFi"
#define D_CONFIGURE_MQTT "Nastavenia MQTT"
#define D_CONFIGURE_DOMOTICZ "Nastavenia Domoticz"
#define D_CONFIGURE_LOGGING "Nastavenia logovania"
#define D_CONFIGURE_OTHER "Ostatné nastavenia"
#define D_CONFIRM_RESET_CONFIGURATION "Potvrdenie resetu nastavení"
#define D_RESET_CONFIGURATION "Reset nastavení"
#define D_BACKUP_CONFIGURATION "Záloha nastavení"
#define D_RESTORE_CONFIGURATION "Obnovenie nastavení"
#define D_MAIN_MENU "Hlavné menu"

#define D_MODULE_PARAMETERS "Nastavenia modulu"
#define D_MODULE_TYPE "Typ modulu"
#define D_PULLUP_ENABLE "No Button/Switch pull-up"
#define D_GPIO "GPIO"
#define D_SERIAL_IN "Serial In"
#define D_SERIAL_OUT "Serial Out"

#define D_WIFI_PARAMETERS "Nastavenia WiFi"
#define D_SCAN_FOR_WIFI_NETWORKS "Sken WiFi sietí"
#define D_SCAN_DONE "Sken dokončený"
#define D_NO_NETWORKS_FOUND "Sieť nebola nájdená"
#define D_REFRESH_TO_SCAN_AGAIN "Zopakovať sken"
#define D_DUPLICATE_ACCESSPOINT "Kópia AP"
#define D_SKIPPING_LOW_QUALITY "Preskočenie z dôvodu kvality signálu"
#define D_RSSI "RSSI"
#define D_WEP "WEP"
#define D_WPA_PSK "WPA PSK"
#define D_WPA2_PSK "WPA2 PSK"
#define D_AP1_SSID "AP1 SSID"
#define D_AP1_PASSWORD "Heslo AP1"
#define D_AP2_SSID "AP2 SSID"
#define D_AP2_PASSWORD "Heslo AP2"

#define D_MQTT_PARAMETERS "Nastavenia MQTT"
#define D_CLIENT "Klient"
#define D_FULL_TOPIC "Celý topic"

#define D_LOGGING_PARAMETERS "Voľba logovania"
#define D_SERIAL_LOG_LEVEL "Sériová úroveň logu"
#define D_WEB_LOG_LEVEL "Webová úroveň logu"
#define D_SYS_LOG_LEVEL "Systemová úroveň logu"
#define D_MORE_DEBUG "Viac debug informácí"
#define D_SYSLOG_HOST "Syslog host"
#define D_SYSLOG_PORT "Syslog port"
#define D_TELEMETRY_PERIOD "Interval telemetrie"

#define D_OTHER_PARAMETERS "Ostatné nastavenia"
#define D_TEMPLATE "Template"
#define D_ACTIVATE "Activate"
#define D_WEB_ADMIN_PASSWORD "Heslo Web administrátora"
#define D_MQTT_ENABLE "MQTT aktívne"
#define D_FRIENDLY_NAME "Friendly Name"
#define D_BELKIN_WEMO "Belkin WeMo"
#define D_HUE_BRIDGE "Hue Bridge"
#define D_SINGLE_DEVICE "single device"
#define D_MULTI_DEVICE "multi device"

#define D_CONFIGURE_TEMPLATE "Configure Template"
#define D_TEMPLATE_PARAMETERS "Template parameters"
#define D_TEMPLATE_NAME "Name"
#define D_BASE_TYPE "Based on"
#define D_TEMPLATE_FLAGS "Options"
#define D_ALLOW_ADC0 "ADC0 input"
#define D_ALLOW_PULLUP "User pull-up selection"

#define D_SAVE_CONFIGURATION "Ulož nastavenia"
#define D_CONFIGURATION_SAVED "Nastavenia uložené"
#define D_CONFIGURATION_RESET "Nastavenia resetované"

#define D_PROGRAM_VERSION "Verzia programu"
#define D_BUILD_DATE_AND_TIME "Datum a čas kompilácie"
#define D_CORE_AND_SDK_VERSION "Verzia Core/SDK"
#define D_FLASH_WRITE_COUNT "Počet zápisov do pamäte"
#define D_MAC_ADDRESS "Adresa MAC"
#define D_MQTT_HOST "Host MQTT"
#define D_MQTT_PORT "Port MQTT"
#define D_MQTT_CLIENT "Klient MQTT"
#define D_MQTT_USER "Používateľ MQTT"
#define D_MQTT_TOPIC "Topic MQTT"
#define D_MQTT_GROUP_TOPIC "Topic skupiny MQTT"
#define D_MQTT_FULL_TOPIC "Celý topic MQTT"
#define D_MDNS_DISCOVERY "Získavanie mDNS"
#define D_MDNS_ADVERTISE "Rozosielanie mDNS"
#define D_ESP_CHIP_ID "ID systému ESP"
#define D_FLASH_CHIP_ID "ID systému flash pamäte"
#define D_FLASH_CHIP_SIZE "Veľkosť flash"
#define D_FREE_PROGRAM_SPACE "Voľné místo pre program"

#define D_UPGRADE_BY_WEBSERVER "Aktualizácia z Web serveru"
#define D_OTA_URL "URL OTA"
#define D_START_UPGRADE "Spustiť aktualizáciu"
#define D_UPGRADE_BY_FILE_UPLOAD "Aktualizácia nahraním súboru"
#define D_UPLOAD_STARTED "Nahrávanie spustené"
#define D_UPGRADE_STARTED "Aktualizácia spustená"
#define D_UPLOAD_DONE "Nahrávanie ukončené"
#define D_UPLOAD_ERR_1 "Súbor nebol vybraný"
#define D_UPLOAD_ERR_2 "Málo miesta"
#define D_UPLOAD_ERR_3 "Magický byte má hodnotu inú než 0xE9"
#define D_UPLOAD_ERR_4 "Veľkosť programu je väčšia než skutočná veľkosť flash pamäte"
#define D_UPLOAD_ERR_5 "Chyba nahrávania, nesúhlasia porovnávané bity"
#define D_UPLOAD_ERR_6 "Chyba nahrávania. Spustený zápis do logu na úrovni 3"
#define D_UPLOAD_ERR_7 "Nahrávanie prerušené"
#define D_UPLOAD_ERR_8 "Nesprávny súbor"
#define D_UPLOAD_ERR_9 "Súbor je príliš veľký"
#define D_UPLOAD_ERR_10 "Chyba inicializácie RF chipu"
#define D_UPLOAD_ERR_11 "Chyba zmazania RF chipu"
#define D_UPLOAD_ERR_12 "Chyba pri zápise do RF chipu"
#define D_UPLOAD_ERR_13 "Chyba dekódovania RF firmwaru"
#define D_UPLOAD_ERROR_CODE "Chyba nahrávania"

#define D_ENTER_COMMAND "Vlož príkaz"
#define D_ENABLE_WEBLOG_FOR_RESPONSE "Zapni úroveň 2 zápisu Weblog, ak je očekávaná odpoveď"
#define D_NEED_USER_AND_PASSWORD "Vyžadovaný používateľ=<username>&heslo=<password>"

// xdrv_01_mqtt.ino
#define D_FINGERPRINT "Verifikuj odtlačok TLS..."
#define D_TLS_CONNECT_FAILED_TO "Nepripojené TLS do"
#define D_RETRY_IN "Zopakujem o"
#define D_VERIFIED "Overený odtlačok "
#define D_INSECURE "Nesprávne pripojenie z dôvodu chybného odtlačku TLS"
#define D_CONNECT_FAILED_TO "Nepodarilo sa nadviazať spojenie"

// xplg_wemohue.ino
#define D_MULTICAST_DISABLED "Multicast je vypnutý"
#define D_MULTICAST_REJOINED "Multicast opäť pripojený"
#define D_MULTICAST_JOIN_FAILED "Multicast neúspešný"
#define D_FAILED_TO_SEND_RESPONSE "Nepodarilo sa odoslať odpoveď"

#define D_WEMO "WeMo"
#define D_WEMO_BASIC_EVENT "WeMo základná udalosť"
#define D_WEMO_EVENT_SERVICE "WeMo servisná udalosť"
#define D_WEMO_META_SERVICE "WeMo meta udalosť"
#define D_WEMO_SETUP "WeMo setup"
#define D_RESPONSE_SENT "Odpoveď odoslaná"

#define D_HUE "Hue"
#define D_HUE_BRIDGE_SETUP "Hue setup"
#define D_HUE_API_NOT_IMPLEMENTED "Hue API nie je implementované"
#define D_HUE_API "Hue API"
#define D_HUE_POST_ARGS "Hue POST args"
#define D_3_RESPONSE_PACKETS_SENT "3 pakety odpovede odoslané"

// xdrv_07_domoticz.ino
#define D_DOMOTICZ_PARAMETERS "Nastavenia Domoticz"
#define D_DOMOTICZ_IDX "Idx"
#define D_DOMOTICZ_KEY_IDX "Key idx"
#define D_DOMOTICZ_SWITCH_IDX "Spinac idx"
#define D_DOMOTICZ_SENSOR_IDX "Sensor idx"
  #define D_DOMOTICZ_TEMP "Temp"
  #define D_DOMOTICZ_TEMP_HUM "Temp,Vlhk"
  #define D_DOMOTICZ_TEMP_HUM_BARO "Temp,Vlhk,Tlak"
  #define D_DOMOTICZ_POWER_ENERGY "Príkon,Energia"
  #define D_DOMOTICZ_ILLUMINANCE "Osvetlenie"
  #define D_DOMOTICZ_COUNT "Počítadlo/PM1"
  #define D_DOMOTICZ_VOLTAGE "Napätie/PM2,5"
  #define D_DOMOTICZ_CURRENT "Prúd/PM10"
  #define D_DOMOTICZ_AIRQUALITY "AirQuality"
#define D_DOMOTICZ_UPDATE_TIMER "Aktualizácia časovača"

// xdrv_09_timers.ino
#define D_CONFIGURE_TIMER "Nastavenia časovača"
#define D_TIMER_PARAMETERS "Časovač"
#define D_TIMER_ENABLE "Povoľ časovače"
#define D_TIMER_ARM "Aktívne"
#define D_TIMER_TIME "Čas"
#define D_TIMER_DAYS "Dni"
#define D_TIMER_REPEAT "Opakovať"
#define D_TIMER_OUTPUT "Výstup"
#define D_TIMER_ACTION "Napájanie"

// xdrv_10_knx.ino
#define D_CONFIGURE_KNX "Nastavenie KNX"
#define D_KNX_PARAMETERS "KNX parametre"
#define D_KNX_GENERAL_CONFIG "Všeobecné"
#define D_KNX_PHYSICAL_ADDRESS "Fyzická adresa"
#define D_KNX_PHYSICAL_ADDRESS_NOTE "( Musí být jedinečná v sieti KNX )"
#define D_KNX_ENABLE "Povoľ KNX"
#define D_KNX_GROUP_ADDRESS_TO_WRITE "Dáta na odoslanie na skupinové adresy"
#define D_ADD "Pridať"
#define D_DELETE "Zmazať"
#define D_REPLY "Odpoveď"
#define D_KNX_GROUP_ADDRESS_TO_READ "Skupinové adresy pre príjem dát z"
#define D_LOG_KNX "KNX: "
#define D_RECEIVED_FROM "Prijaté z"
#define D_KNX_COMMAND_WRITE "Zapíš"
#define D_KNX_COMMAND_READ "Čítaj"
#define D_KNX_COMMAND_OTHER "Iné"
#define D_SENT_TO "pošli"
#define D_KNX_WARNING "Skupinová adresa ( 0 / 0 / 0 ) je rezervovaná a nemôže byť použitá."
#define D_KNX_ENHANCEMENT "Communication Enhancement"
#define D_KNX_TX_SLOT "KNX TX"
#define D_KNX_RX_SLOT "KNX RX"

// xdrv_03_energy.ino
#define D_ENERGY_TODAY "Spotreba dnes"
#define D_ENERGY_YESTERDAY "Spotreba včera"
#define D_ENERGY_TOTAL "Celková spotreba"

// xsns_05_ds18b20.ino
#define D_SENSOR_BUSY "Sensor DS18x20 obsadený"
#define D_SENSOR_CRC_ERROR "Sensor DS18x20 chyba CRC"
#define D_SENSORS_FOUND "Nanájdený senzor DS18x20"

// xsns_06_dht.ino
#define D_TIMEOUT_WAITING_FOR "Čakanie na"
#define D_START_SIGNAL_LOW "nízký štartovací signál"
#define D_START_SIGNAL_HIGH "vysoký štartovací signál"
#define D_PULSE "impulz"
#define D_CHECKSUM_FAILURE "Chybný kontrolný súčet"

// xsns_07_sht1x.ino
#define D_SENSOR_DID_NOT_ACK_COMMAND "Senzor neobdržal príkaz ACK"
#define D_SHT1X_FOUND "SHT1X nanájdený"

// xsns_18_pms5003.ino
#define D_STANDARD_CONCENTRATION "CF-1 PM"     // Standard Particle CF-1 Particle Matter
#define D_ENVIRONMENTAL_CONCENTRATION "PM"     // Environmetal Particle Matter
#define D_PARTICALS_BEYOND "častíc"

// xsns_32_mpu6050.ino
#define D_AX_AXIS "Accel. os-X"
#define D_AY_AXIS "Accel. os-Y"
#define D_AZ_AXIS "Accel. os-Z"
#define D_GX_AXIS "Gyro os-X"
#define D_GY_AXIS "Gyro os-Y"
#define D_GZ_AXIS "Gyro os-Z"

// xsns_34_hx711.ino
#define D_HX_CAL_REMOVE "Odstráňte záťaž"
#define D_HX_CAL_REFERENCE "Vložte referenčnú záťaž"
#define D_HX_CAL_DONE "Skalibrováné"
#define D_HX_CAL_FAIL "Chyba kalibráce"
#define D_RESET_HX711 "Reset váhy"
#define D_CONFIGURE_HX711 "Konfiguráce váhy"
#define D_HX711_PARAMETERS "Parametre váhy"
#define D_ITEM_WEIGHT "Vlastná hmotnosť"
#define D_REFERENCE_WEIGHT "Referenčná hmotnosť"
#define D_CALIBRATE "Kalibruj"
#define D_CALIBRATION "Kalibrácia"

//xsns_35_tx20.ino
#define D_TX20_WIND_DIRECTION "Smer vetra"
#define D_TX20_WIND_SPEED "Rýchlosť vetra"
#define D_TX20_WIND_SPEED_AVG "Priemerná rýchlosť vetra"
#define D_TX20_WIND_SPEED_MAX "Maximálna rýchlosť vetra"
#define D_TX20_NORTH "S"
#define D_TX20_EAST "V"
#define D_TX20_SOUTH "J"
#define D_TX20_WEST "Z"

// sonoff_template.h - keep them as short as possible to be able to fit them in GUI drop down box
#define D_SENSOR_NONE          "Žiaden"
#define D_SENSOR_USER          "User"
#define D_SENSOR_DHT11         "DHT11"
#define D_SENSOR_AM2301        "AM2301"
#define D_SENSOR_SI7021        "SI7021"
#define D_SENSOR_DS18X20       "DS18x20"
#define D_SENSOR_I2C_SCL       "I2C SCL"
#define D_SENSOR_I2C_SDA       "I2C SDA"
#define D_SENSOR_WS2812        "WS2812"
#define D_SENSOR_DFR562        "MP3 Player"
#define D_SENSOR_IRSEND        "IRsend"
#define D_SENSOR_SWITCH        "Spínač"     // Suffix "1"
#define D_SENSOR_BUTTON        "Tlačidlo"   // Suffix "1"
#define D_SENSOR_RELAY         "Relé"       // Suffix "1i"
#define D_SENSOR_LED           "Led"        // Suffix "1i"
#define D_SENSOR_PWM           "PWM"        // Suffix "1",
#define D_SENSOR_COUNTER       "Počítadlo"  // Suffix "1"
#define D_SENSOR_IRRECV        "IRrecv"
#define D_SENSOR_MHZ_RX        "MHZ Rx"
#define D_SENSOR_MHZ_TX        "MHZ Tx"
#define D_SENSOR_PZEM004_RX    "PZEM004 Rx"
#define D_SENSOR_PZEM016_RX    "PZEM016 Rx"
#define D_SENSOR_PZEM017_RX    "PZEM017 Rx"
#define D_SENSOR_PZEM0XX_TX    "PZEM0XX Tx"
#define D_SENSOR_SAIR_RX       "SAir Rx"
#define D_SENSOR_SAIR_TX       "SAir Tx"
#define D_SENSOR_SPI_CS        "SPI CS"
#define D_SENSOR_SPI_DC        "SPI DC"
#define D_SENSOR_BACKLIGHT     "BkLight"
#define D_SENSOR_PMS5003       "PMS5003"
#define D_SENSOR_SDS0X1_RX     "SDS0X1 Rx"
#define D_SENSOR_SDS0X1_TX     "SDS0X1 Tx"
#define D_SENSOR_SBR_RX        "SerBr Rx"
#define D_SENSOR_SBR_TX        "SerBr Tx"
#define D_SENSOR_SR04_TRIG     "SR04 Tri"
#define D_SENSOR_SR04_ECHO     "SR04 Ech"
#define D_SENSOR_SDM120_TX     "SDMx20 Tx"
#define D_SENSOR_SDM120_RX     "SDMx20 Rx"
#define D_SENSOR_SDM630_TX     "SDM630 Tx"
#define D_SENSOR_SDM630_RX     "SDM630 Rx"
#define D_SENSOR_TM1638_CLK    "TM16 CLK"
#define D_SENSOR_TM1638_DIO    "TM16 DIO"
#define D_SENSOR_TM1638_STB    "TM16 STB"
#define D_SENSOR_HX711_SCK     "HX711 SCK"
#define D_SENSOR_HX711_DAT     "HX711 DAT"
#define D_SENSOR_TX20_TX       "TX20"
#define D_SENSOR_RFSEND        "RFSend"
#define D_SENSOR_RFRECV        "RFrecv"
#define D_SENSOR_TUYA_TX       "Tuya Tx"
#define D_SENSOR_TUYA_RX       "Tuya Rx"
#define D_SENSOR_MGC3130_XFER  "MGC3130 Xfr"
#define D_SENSOR_MGC3130_RESET "MGC3130 Rst"
#define D_SENSOR_SSPI_MISO     "SSPI MISO"
#define D_SENSOR_SSPI_MOSI     "SSPI MOSI"
#define D_SENSOR_SSPI_SCLK     "SSPI SCLK"
#define D_SENSOR_SSPI_CS       "SSPI CS"
#define D_SENSOR_SSPI_DC       "SSPI DC"
#define D_SENSOR_RF_SENSOR     "RF Senzor"
#define D_SENSOR_AZ_RX         "AZ Rx"
#define D_SENSOR_AZ_TX         "AZ Tx"
#define D_SENSOR_MAX31855_CS   "MX31855 CS"
#define D_SENSOR_MAX31855_CLK  "MX31855 CLK"
#define D_SENSOR_MAX31855_DO   "MX31855 DO"
#define D_SENSOR_NRG_SEL       "HLWBL SEL"  // Suffix "i"
#define D_SENSOR_NRG_CF1       "HLWBL CF1"
#define D_SENSOR_HLW_CF        "HLW8012 CF"
#define D_SENSOR_HJL_CF        "BL0937 CF"
#define D_SENSOR_MCP39F5_TX    "MCP39F5 Tx"
#define D_SENSOR_MCP39F5_RX    "MCP39F5 Rx"
#define D_SENSOR_MCP39F5_RST   "MCP39F5 Rst"
#define D_SENSOR_CSE7766_TX    "CSE7766 Tx"
#define D_SENSOR_CSE7766_RX    "CSE7766 Rx"
#define D_SENSOR_PN532_TX      "PN532 Tx"
#define D_SENSOR_PN532_RX      "PN532 Rx"
#define D_SENSOR_SM16716_CLK   "SM16716 CLK"
#define D_SENSOR_SM16716_DAT   "SM16716 DAT"
#define D_SENSOR_SM16716_POWER "SM16716 PWR"
#define D_SENSOR_MY92X1_DI     "MY92x1 DI"
#define D_SENSOR_MY92X1_DCKI   "MY92x1 DCKI"
#define D_SENSOR_ARIRFRCV      "ALux IrRcv"
#define D_SENSOR_TXD           "Serial Tx"
#define D_SENSOR_RXD           "Serial Rx"
#define D_SENSOR_ROTARY        "Rotary"     // Suffix "1A"

// Units
#define D_UNIT_AMPERE "A"
#define D_UNIT_CENTIMETER "cm"
#define D_UNIT_HERTZ "Hz"
#define D_UNIT_HOUR "hod"
#define D_UNIT_KILOGRAM "kg"
#define D_UNIT_INCREMENTS "inc"
#define D_UNIT_KILOMETER_PER_HOUR "km/h"  // or "km/h"
#define D_UNIT_KILOOHM "kOhm"
#define D_UNIT_KILOWATTHOUR "kWh"
#define D_UNIT_LUX "lx"
#define D_UNIT_MICROGRAM_PER_CUBIC_METER "ug/m3"
#define D_UNIT_MICROMETER "um"
#define D_UNIT_MICROSECOND "us"
#define D_UNIT_MILLIAMPERE "mA"
#define D_UNIT_MILLIMETER "mm"
#define D_UNIT_MILLIMETER_MERCURY "mmHg"
#define D_UNIT_MILLISECOND "ms"
#define D_UNIT_MINUTE "min"
#define D_UNIT_PARTS_PER_BILLION "ppb"
#define D_UNIT_PARTS_PER_DECILITER "ppd"
#define D_UNIT_PARTS_PER_MILLION "ppm"
#define D_UNIT_PRESSURE "hPa"
#define D_UNIT_SECOND "sek"
#define D_UNIT_SECTORS "sektory"
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

#endif  // _LANGUAGE_SK_SK_H_
