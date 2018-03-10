/*
  cz-CZ.h - localization for Czech without fonetick - Czech for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends (translated by vs)

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

#ifndef _LANGUAGE_CZ_CZ_H_
#define _LANGUAGE_CZ_CZ_H_

/*************************** ATTENTION *******************************\
 *
 * Due to memory constraints only UTF-8 is supported.
 * To save code space keep text as short as possible.
 * Time and Date provided by SDK can not be localized (yet).
 * Use online command StateText to translate ON, OFF, HOLD and TOGGLE.
 * Use online command Prefix to translate cmnd, stat and tele.
 *
\*********************************************************************/

//#define LANGUAGE_MODULE_NAME         // Enable to display "Module Generic" (ie Spanish), Disable to display "Generic Module" (ie English)

// "2017-03-07T11:08:02" - ISO8601:2004
#define D_YEAR_MONTH_SEPARATOR "-"
#define D_MONTH_DAY_SEPARATOR "-"
#define D_DATE_TIME_SEPARATOR "T"
#define D_HOUR_MINUTE_SEPARATOR ":"
#define D_MINUTE_SECOND_SEPARATOR ":"

#define D_DAY3LIST "PonUteStrCtvPatSobNed"
#define D_MONTH3LIST "LedUnoBreDubKveCerCevSrpZarRijLisPro"

// Non JSON decimal separator
#define D_DECIMAL_SEPARATOR ","

// Common
#define D_ADMIN "Admin"
#define D_AIR_QUALITY "Kvalita vzduchu"
#define D_AP "AP"                    // Access Point
#define D_AS "jako"
#define D_AUTO "AUTO"
#define D_BLINK "Blikani"
#define D_BLINKOFF "BlikaniVyp"
#define D_BOOT_COUNT "Pocitadlo spusteni"
#define D_BRIGHTLIGHT "Svetly"
#define D_BUTTON "Tlacitko"
#define D_BY "by"                    // Written by me
#define D_BYTES "Bytu"
#define D_CELSIUS "Celsia"
#define D_CO2 "Kyslicniku uhliciteho"
#define D_CODE "kod"                 // Button code
#define D_COLDLIGHT "Studeny"
#define D_COMMAND "Prikaz"
#define D_CONNECTED "Pripojeny"
#define D_COUNT "Pocitej"
#define D_COUNTER "Pocitadlo"
#define D_CURRENT "Aktualni"          // As in Voltage and Current
#define D_DATA "Data"
#define D_DARKLIGHT "Tmavy"
#define D_DEBUG "Debug"
#define D_DISABLED "Zablokovany"
#define D_DNS_SERVER "Server DNS"
#define D_DONE "Provedeno"
#define D_DST_TIME "DST"
#define D_EMULATION "Emulace"
#define D_ENABLED "Otevrene"
#define D_ERASE "Vymaz"
#define D_ERROR "Chyba"
#define D_FAHRENHEIT "Fahrenheita"
#define D_FAILED "Chybny"
#define D_FALLBACK "Nahradni"
#define D_FALLBACK_TOPIC "Nahradni topic"
#define D_FALSE "Nepravda"
#define D_FILE "Soubor"
#define D_FREE_MEMORY "Volna pamet"
#define D_GAS "Gas"
#define D_GATEWAY "Brana"
#define D_GROUP "Skupina"
#define D_HOST "Server"
#define D_HOSTNAME "Nazev serveru"
#define D_HUMIDITY "Vlhkost"
#define D_ILLUMINANCE "Osvetleni"
#define D_IMMEDIATE "Okamzite"      // Button immediate
#define D_INDEX "Index"
#define D_INFO "Informace"
#define D_INITIALIZED "Inicializovany"
#define D_IP_ADDRESS "Adresa IP"
#define D_LIGHT "Svetlo"
#define D_LWT "LWT"
#define D_MODULE "Modul"
#define D_MQTT "MQTT"
#define D_MULTI_PRESS "nekolikere-stisknuti"
#define D_NOISE "Hluk"
#define D_NONE "Neni"
#define D_OFF "Vypnuty"
#define D_OFFLINE "Neaktivni"
#define D_OK "Ok"
#define D_ON "Sepnuty"
#define D_ONLINE "Aktivni"
#define D_PASSWORD "Heslo"
#define D_PORT "Port"
#define D_POWER_FACTOR "Soucinitel prikonu"
#define D_POWERUSAGE "Prikon"
#define D_PRESSURE "Tlak"
#define D_PRESSUREATSEALEVEL "Tlak na urovni hladiny more"
#define D_PROGRAM_FLASH_SIZE "Velikost programu flash"
#define D_PROGRAM_SIZE "Velikost programu"
#define D_PROJECT "Projekt"
#define D_RECEIVED "Obdrzeny"
#define D_RESTART "Restart"
#define D_RESTARTING "Restartovani"
#define D_RESTART_REASON "Pricina restartu"
#define D_RESTORE "Nacteni"
#define D_RETAINED "Ulozene"
#define D_SAVE "Uloz"
#define D_SENSOR "Sensor"
#define D_SSID "SSID"
#define D_START "Start"
#define D_STD_TIME "STD"
#define D_STOP "Stop"
#define D_SUBNET_MASK "Maska podsite"
#define D_SUBSCRIBE_TO "Prihlas se do"
#define D_SUCCESSFUL "uspesne."
#define D_TEMPERATURE "Teplota"
#define D_TO "do"
#define D_TOGGLE "Prepni"
#define D_TOPIC "Topic"
#define D_TRANSMIT "Odesli"
#define D_TRUE "Pravda"
#define D_UPGRADE "aktualizace"
#define D_UPLOAD "Nahrani..."
#define D_UPTIME "Uptime"
#define D_USER "Uzivatel"
#define D_UTC_TIME "UTC"
#define D_UV_LEVEL "Uroven UV"
#define D_VERSION "Verze"
#define D_VOLTAGE "Napeti"
#define D_WARMLIGHT "Ohrev"
#define D_WEB_SERVER "Web Server"

// sonoff.ino
#define D_WARNING_MINIMAL_VERSION "WARNING This version does not support persistent settings"
#define D_LEVEL_10 "uroven 1-0"
#define D_LEVEL_01 "uroven 0-1"
#define D_SERIAL_LOGGING_DISABLED "Vypnute logovani na seriovem portu"
#define D_SYSLOG_LOGGING_REENABLED "Obnoven zapis do Syslog"

#define D_SET_BAUDRATE_TO "Nastaveni rychlosti prenosu na"
#define D_RECEIVED_TOPIC "Obdrzeny topic"
#define D_DATA_SIZE "Velikost dat"
#define D_ANALOG_INPUT "Analogovy vstup"

#define D_FINGERPRINT "Verifikuj otisk TLS..."
#define D_TLS_CONNECT_FAILED_TO "Nepripojeno TLS do"
#define D_RETRY_IN "Zopakuji za"
#define D_VERIFIED "Zverifikovano otisk"
#define D_INSECURE "Nespravne pripojeni z duvodu chybneho otisku TLS"
#define D_CONNECT_FAILED_TO "Spojeni se nepodarilo navazat"

// support.ino
#define D_OSWATCH "osWatch"
#define D_BLOCKED_LOOP "Zablokovani smycky"
#define D_WPS_FAILED_WITH_STATUS "Chyba WPSconfig se statusem"
#define D_ACTIVE_FOR_3_MINUTES "aktivni 3 minuty"
#define D_FAILED_TO_START "nezdarilo se spustit"
#define D_PATCH_ISSUE_2186 "Chyba 2186"
#define D_CONNECTING_TO_AP "Pripojovani k AP"
#define D_IN_MODE "v modu"
#define D_CONNECT_FAILED_NO_IP_ADDRESS "Chyba pripojeni, nebyla obdrzena IP adresa"
#define D_CONNECT_FAILED_AP_NOT_REACHED "Chyba pripojeni, nedostupny AP"
#define D_CONNECT_FAILED_WRONG_PASSWORD "Chyba pripojeni, nespravne heslo pro AP"
#define D_CONNECT_FAILED_AP_TIMEOUT "Chyba pripojeni, uplynul AP timeout"
#define D_ATTEMPTING_CONNECTION "Zkouska pripojeni..."
#define D_CHECKING_CONNECTION "Zkouska spojeni..."
#define D_QUERY_DONE "Vyslan dotaz. Nalezena sluzba MQTT"
#define D_MQTT_SERVICE_FOUND "Sluzba MQTT byla nalezena"
#define D_FOUND_AT "znalezeno v"
#define D_SYSLOG_HOST_NOT_FOUND "Syslog Host nebyl nalezen"

// settings.ino
#define D_SAVED_TO_FLASH_AT "Ulozeno do pameti flash v"
#define D_LOADED_FROM_FLASH_AT "Stazeno z pameti flash z"
#define D_USE_DEFAULTS "Pouzij defaultni hodnoty"
#define D_ERASED_SECTOR "Smazany sektor"

// webserver.ino
#define D_MINIMAL_FIRMWARE_PLEASE_UPGRADE "Oprogramowanie MINIMAL - prosze uaktualnic"
#define D_WEBSERVER_ACTIVE_ON "Aktivni Web server"
#define D_WITH_IP_ADDRESS "na adrese IP"
#define D_WEBSERVER_STOPPED "Web server zastaven"
#define D_FILE_NOT_FOUND "Soubor nebyl nalezen"
#define D_REDIRECTED "Presmerovani na vlastni portal"
#define D_WIFIMANAGER_SET_ACCESSPOINT_AND_STATION "Wi-Fi manager nastavi AP a zachova STA"
#define D_WIFIMANAGER_SET_ACCESSPOINT "Wi-Fi manager nastavi Access Point"
#define D_TRYING_TO_CONNECT "Zkousim pripojit zarizeni k siti"

#define D_RESTART_IN "Restart"
#define D_SECONDS "sekund"
#define D_DEVICE_WILL_RESTART "Zarizeni bude restartovano behem nekolika sekund"
#define D_BUTTON_TOGGLE "Prepinac"
#define D_CONFIGURATION "Nastaveni"
#define D_INFORMATION "Informace"
#define D_FIRMWARE_UPGRADE "Aktualizace firmware"
#define D_CONSOLE "Konzole"
#define D_CONFIRM_RESTART "Potvrzeni restartu"

#define D_CONFIGURE_MODULE "Nastaveni modulu"
#define D_CONFIGURE_WIFI "Nastaveni WiFi"
#define D_CONFIGURE_MQTT "Nastaveni MQTT"
#define D_CONFIGURE_DOMOTICZ "Nastaveni Domoticz"
#define D_CONFIGURE_LOGGING "Nastaveni logovani"
#define D_CONFIGURE_OTHER "Jina nastaveni"
#define D_CONFIRM_RESET_CONFIGURATION "Potvrzeni resetu nastaveni"
#define D_RESET_CONFIGURATION "Reset nastaveni"
#define D_BACKUP_CONFIGURATION "Zaloha nastaveni"
#define D_RESTORE_CONFIGURATION "Obnoveni nastaveni"
#define D_MAIN_MENU "Hlavni menu"

#define D_MODULE_PARAMETERS "Parametry modulu"
#define D_MODULE_TYPE "Typ modulu"
#define D_GPIO "GPIO"
#define D_SERIAL_IN "Serial In"
#define D_SERIAL_OUT "Serial Out"

#define D_WIFI_PARAMETERS "Parametry WiFi"
#define D_SCAN_FOR_WIFI_NETWORKS "Scan siti WiFi"
#define D_SCAN_DONE "Scan dokoncen"
#define D_NO_NETWORKS_FOUND "Sit nebyla nalezena"
#define D_REFRESH_TO_SCAN_AGAIN "Zopakovat scan"
#define D_DUPLICATE_ACCESSPOINT "Kopie AP"
#define D_SKIPPING_LOW_QUALITY "Preskoceni z duvodu kvality signalu"
#define D_RSSI "RSSI"
#define D_WEP "WEP"
#define D_WPA_PSK "WPA PSK"
#define D_WPA2_PSK "WPA2 PSK"
#define D_AP1_SSID "AP1 SSID"
#define D_AP1_PASSWORD "Heslo AP1"
#define D_AP2_SSID "AP2 SSID"
#define D_AP2_PASSWORD "Heslo AP2"

#define D_MQTT_PARAMETERS "Parametry MQTT"
#define D_CLIENT "Klient"
#define D_FULL_TOPIC "Cely topic"

#define D_LOGGING_PARAMETERS "Volby logovani"
#define D_SERIAL_LOG_LEVEL "Serial uroven logu"
#define D_WEB_LOG_LEVEL "Webova uroven logu"
#define D_SYS_LOG_LEVEL "Systemova uroven logu"
#define D_MORE_DEBUG "Vice debug informaci"
#define D_SYSLOG_HOST "Syslog host"
#define D_SYSLOG_PORT "Syslog port"
#define D_TELEMETRY_PERIOD "Interval telemetrie"

#define D_OTHER_PARAMETERS "Jine parametry"
#define D_WEB_ADMIN_PASSWORD "Heslo Web administratora"
#define D_MQTT_ENABLE "MQTT aktivni"
#define D_FRIENDLY_NAME "Friendly Name"
#define D_BELKIN_WEMO "Belkin WeMo"
#define D_HUE_BRIDGE "Hue Bridge"
#define D_SINGLE_DEVICE "single device"
#define D_MULTI_DEVICE "multi device"

#define D_SAVE_CONFIGURATION "Uloz nastaveni"
#define D_CONFIGURATION_SAVED "Nastaveni ulozena"
#define D_CONFIGURATION_RESET "Nastaveni resetovano"

#define D_PROGRAM_VERSION "Verze programu"
#define D_BUILD_DATE_AND_TIME "Datum a cas kompilace"
#define D_CORE_AND_SDK_VERSION "Verze Core/SDK"
#define D_FLASH_WRITE_COUNT "Pocet zapisu do pameti"
#define D_MAC_ADDRESS "Adres Mac"
#define D_MQTT_HOST "Host MQTT"
#define D_MQTT_PORT "Port MQTT"
#define D_MQTT_CLIENT "Klient MQTT"
#define D_MQTT_USER "Uzivatel MQTT"
#define D_MQTT_TOPIC "Topic MQTT"
#define D_MQTT_GROUP_TOPIC "Topic skupiny MQTT"
#define D_MQTT_FULL_TOPIC "Cely topic MQTT"
#define D_MDNS_DISCOVERY "Ziskavani mDNS"
#define D_MDNS_ADVERTISE "Rozesilani mDNS"
#define D_ESP_CHIP_ID "ID systemu ESP"
#define D_FLASH_CHIP_ID "ID systemu pameti flash"
#define D_FLASH_CHIP_SIZE "Velikost flash"
#define D_FREE_PROGRAM_SPACE "Volne misto programu"

#define D_UPGRADE_BY_WEBSERVER "Aktualizace z Web serveru"
#define D_OTA_URL "URL OTA"
#define D_START_UPGRADE "Start aktualizace"
#define D_UPGRADE_BY_FILE_UPLOAD "Aktualizace nahranim souboru"
#define D_UPLOAD_STARTED "Nahravani zahajeno"
#define D_UPGRADE_STARTED "Zahajeni aktualizace"
#define D_UPLOAD_DONE "Nahravani ukonceno"
#define D_UPLOAD_ERR_1 "Soubor nebyl vybran"
#define D_UPLOAD_ERR_2 "Malo mista"
#define D_UPLOAD_ERR_3 "Magicky byte ma jinou hodnotu nez 0xE9"
#define D_UPLOAD_ERR_4 "Velikost programu je vetsi nez skutecna velikost pameti flash"
#define D_UPLOAD_ERR_5 "Chyba nahravani, nesouhlasi porovnavane bity"
#define D_UPLOAD_ERR_6 "Chyba nahravani. Spusten zapis do logu na urovni 3"
#define D_UPLOAD_ERR_7 "Nahravani preruseno"
#define D_UPLOAD_ERR_8 "Spatny soubor"
#define D_UPLOAD_ERR_9 "Soubor je prilis velky"
#define D_UPLOAD_ERROR_CODE "Chyba nahravani"

#define D_ENTER_COMMAND "Vloz prikaz"
#define D_ENABLE_WEBLOG_FOR_RESPONSE "Zapni uroven 2 zapisu Weblog, pokud je ocekavana odpoved"
#define D_NEED_USER_AND_PASSWORD "Vyzadovany uzivatel=<username>&heslo=<password>"

// xdrv_wemohue.ino
#define D_MULTICAST_DISABLED "Multicast je vypnuty"
#define D_MULTICAST_REJOINED "Multicast opet pripojeny"
#define D_MULTICAST_JOIN_FAILED "Multicast neuspesny"
#define D_FAILED_TO_SEND_RESPONSE "Nepodarilo se odeslat odpoved"

#define D_WEMO "WeMo"
#define D_WEMO_BASIC_EVENT "WeMo zakladni udalost"
#define D_WEMO_EVENT_SERVICE "WeMo servisni udalost"
#define D_WEMO_META_SERVICE "WeMo meta udalost"
#define D_WEMO_SETUP "WeMo setup"
#define D_RESPONSE_SENT "Odpoved odeslana"

#define D_HUE "Hue"
#define D_HUE_BRIDGE_SETUP "Hue setup"
#define D_HUE_API_NOT_IMPLEMENTED "Hue API neni implementovano"
#define D_HUE_API "Hue API"
#define D_HUE_POST_ARGS "Hue POST args"
#define D_3_RESPONSE_PACKETS_SENT "3 pakety odpovedi odeslany"

// xdrv_05_domoticz.ino
#define D_DOMOTICZ_PARAMETERS "Parametry Domoticz"
#define D_DOMOTICZ_IDX "Idx"
#define D_DOMOTICZ_KEY_IDX "Key idx"
#define D_DOMOTICZ_SWITCH_IDX "Spinac idx"
#define D_DOMOTICZ_SENSOR_IDX "Sensor idx"
  #define D_DOMOTICZ_TEMP "Temp"
  #define D_DOMOTICZ_TEMP_HUM "Temp,Vlhk"
  #define D_DOMOTICZ_TEMP_HUM_BARO "Temp,Vlhk,Tlak"
  #define D_DOMOTICZ_POWER_ENERGY "Prikon,Energie"
  #define D_DOMOTICZ_ILLUMINANCE "Osvetleni"
  #define D_DOMOTICZ_COUNT "Pocitadlo"
  #define D_DOMOTICZ_VOLTAGE "Napeti"
  #define D_DOMOTICZ_CURRENT "Proud"
  #define D_DOMOTICZ_AIRQUALITY "AirQuality"
#define D_DOMOTICZ_UPDATE_TIMER "Aktualizace stopek"

// xdrv_03_energy.ino
#define D_ENERGY_TODAY "Energie Dnes"
#define D_ENERGY_YESTERDAY "Energie Vcera"
#define D_ENERGY_TOTAL "Energie Soucet"

// xsns_05_ds18b20.ino
#define D_SENSOR_BUSY "Sensor DS18x20 obsazen"
#define D_SENSOR_CRC_ERROR "Sensor DS18x20 chyba CRC"
#define D_SENSORS_FOUND "Nalezen sensor DS18x20"

// xsns_06_dht.ino
#define D_TIMEOUT_WAITING_FOR "Cekani na"
#define D_START_SIGNAL_LOW "signal� startovni nizky"
#define D_START_SIGNAL_HIGH "signal� startovni vysoky"
#define D_PULSE "puls"
#define D_CHECKSUM_FAILURE "Chybny kontrolni soucet"

// xsns_07_sht1x.ino
#define D_SENSOR_DID_NOT_ACK_COMMAND "Sensor neobdrzel prikaz ACK"
#define D_SHT1X_FOUND "SHT1X nalezen"

// xsns_18_pms5003.ino
#define D_STANDARD_CONCENTRATION "CF-1 PM"     // Standard Particle CF-1 Particle Matter
#define D_ENVIRONMENTAL_CONCENTRATION "PM"     // Environmetal Particle Matter
#define D_PARTICALS_BEYOND "Particals"

// sonoff_template.h
#define D_SENSOR_NONE     "Neni"
#define D_SENSOR_DHT11    "DHT11"
#define D_SENSOR_AM2301   "AM2301"
#define D_SENSOR_SI7021   "SI7021"
#define D_SENSOR_DS18X20  "DS18x20"
#define D_SENSOR_I2C_SCL  "I2C SCL"
#define D_SENSOR_I2C_SDA  "I2C SDA"
#define D_SENSOR_WS2812   "WS2812"
#define D_SENSOR_IRSEND   "IRsend"
#define D_SENSOR_SWITCH   "Spinac"   // Suffix "1"
#define D_SENSOR_BUTTON   "Tlacit"   // Suffix "1"
#define D_SENSOR_RELAY    "Rele"    // Suffix "1i"
#define D_SENSOR_LED      "Led"      // Suffix "1i"
#define D_SENSOR_PWM      "PWM"      // Suffix "1"
#define D_SENSOR_COUNTER  "Pocita"  // Suffix "1"
#define D_SENSOR_IRRECV   "IRrecv"
#define D_SENSOR_MHZ_RX   "MHZ Rx"
#define D_SENSOR_MHZ_TX   "MHZ Tx"
#define D_SENSOR_PZEM_RX  "PZEM Rx"
#define D_SENSOR_PZEM_TX  "PZEM Tx"
#define D_SENSOR_SAIR_RX  "SAir Rx"
#define D_SENSOR_SAIR_TX  "SAir Tx"
#define D_SENSOR_SPI_CS   "SPI CS"
#define D_SENSOR_SPI_DC   "SPI DC"
#define D_SENSOR_BACKLIGHT "BkLight"
#define D_SENSOR_PMS5003  "PMS5003"
#define D_SENSOR_SDS0X1   "SDS0X1"

// Units
#define D_UNIT_AMPERE "A"
#define D_UNIT_HOUR "Hod"
#define D_UNIT_KILOOHM "kOhm"
#define D_UNIT_KILOWATTHOUR "kWh"
#define D_UNIT_LUX "lx"
#define D_UNIT_MICROGRAM_PER_CUBIC_METER "ug/m3"
#define D_UNIT_MICROMETER "um"
#define D_UNIT_MICROSECOND "us"
#define D_UNIT_MILLIAMPERE "mA"
#define D_UNIT_MILLISECOND "ms"
#define D_UNIT_MINUTE "Min"
#define D_UNIT_PARTS_PER_DECILITER "ppd"
#define D_UNIT_PARTS_PER_MILLION "ppm"
#define D_UNIT_PRESSURE "hPa"
#define D_UNIT_SECOND "sec"
#define D_UNIT_SECTORS "sektory"
#define D_UNIT_VOLT "V"
#define D_UNIT_WATT "W"
#define D_UNIT_WATTHOUR "Wh"

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

#endif  // _LANGUAGE_CZ_CZ_H_
