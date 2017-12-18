/*
  pl-PL.h - localization for Polish without fonetick - Poland for Sonoff-Tasmota

  Copyright (C) 2017  Theo Arends (translated by roblad - Robert L.)

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

#ifndef _LANGUAGE_PL_PL_H_
#define _LANGUAGE_PL_PL_H_

/*************************** ATTENTION *******************************\
 *
 * Due to memory constraints only UTF-8 is supported.
 * To save code space keep text as short as possible.
 * Time and Date provided by SDK can not be localized (yet).
 * Use online command StateText to translate ON, OFF, HOLD and TOGGLE.
 * Use online command Prefix to translate cmnd, stat and tele.
 *
\*********************************************************************/

// "2017-03-07T11:08:02" - ISO8601:2004
#define D_YEAR_MONTH_SEPARATOR "-"
#define D_MONTH_DAY_SEPARATOR "-"
#define D_DATE_TIME_SEPARATOR "T"
#define D_HOUR_MINUTE_SEPARATOR ":"
#define D_MINUTE_SECOND_SEPARATOR ":"

#define D_DAY3LIST "PonWtoSroCzwPiaSobNie"
#define D_MONTH3LIST "StyLutMarKwiMajCzeLipSieWrzPazLisGru"

// Non JSON decimal separator
#define D_DECIMAL_SEPARATOR ","

// Common
#define D_ABORTED "Anulowany"
#define D_ACTIVE "Aktywny"
#define D_ADDRESS "Adres"
#define D_ADMIN "Admin"
#define D_AIR_QUALITY "Jakosc powietrza"
#define D_AIRQUALITY "JakPowietrza"
#define D_AP "AP"                    // Access Point
#define D_APMAC_ADDRESS "APMac"
#define D_APPENDED "Dolaczony"
#define D_AS "jak"
#define D_AUTO "AUTO"
#define D_BAUDRATE "Szybkosc transmisji"
#define D_BLINK "Miganie"
#define D_BLINKOFF "MiganieWyl"
#define D_BOOTVERSION "Uruchomienie"
#define D_BOOT_COUNT "Licznik uruchomienia"
#define D_BOOTCOUNT "LiczUruchomienia"
#define D_BRIGHTLIGHT "Jasny"
#define D_BUILDDATETIME "ZbudowanoDataCzas"
#define D_BUTTON "Przycisk"
#define D_BY "by"                    // Written by me
#define D_CELSIUS "Celsiusza"
#define D_CO2 "CO2"
#define D_CODE "kod"                 // Button code
#define D_COLDLIGHT "Zimny"
#define D_COMMAND "Komenda"
#define D_CONNECTED "Polaczony"
#define D_COREVERSION "Wersja"
#define D_COUNT "Licz"
#define D_COUNTER "Licznik"
#define D_CURRENT "Biezacy"          // As in Voltage and Current
#define D_DATA "Data"
#define D_DARKLIGHT "Ciemny"
#define D_DEBUG "Debug"
#define D_DISABLED "Zablokowany"
#define D_DNS_SERVER "Server DNS"
#define D_DNSSERVER "ServerDNS"
#define D_DONE "Wykonane"
#define D_DST_TIME "DST"
#define D_EMPTY "Pusty"
#define D_EMULATION "Emulacja"
#define D_ENABLED "Otwarty"
#define D_ENDDST "KoniecDST"           // End Daylight Savings Time
#define D_ERASE "Nadpisz"
#define D_ERROR "Blad"
#define D_EVERY "Kazdy"
#define D_FAHRENHEIT "Fahrenheita"
#define D_FAILED "Bledny"
#define D_FALLBACK "Awaryjny"
#define D_FALLBACK_TOPIC "Zastepczy temat"
#define D_FALLBACKTOPIC "ZastepczyTemat"
#define D_FALSE "Falsz"
#define D_FILE "Plik"
#define D_FLASHMODE "FlashMode"
#define D_FLASHSIZE "FlashWielkosc"
#define D_FREE_MEMORY "Wolna pamiec"
#define D_FREEMEMORY "Wolne"
#define D_FROM "z"
#define D_GAS "Gas"
#define D_GATEWAY "Brama"
#define D_GROUP "Grupa"
#define D_HEAPSIZE "Stos"
#define D_HIGH "Wysoki"
#define D_HOST "Serwer"
#define D_HOSTNAME "Nazwa serwera"
#define D_HUMIDITY "Wilgotnosc"
#define D_ID "ID"
#define D_ILLUMINANCE "Oswietlenie"
#define D_IMMEDIATE "Natychmiastowe"      // Button immediate
#define D_INDEX "Indeks"
#define D_INFO "Informacja"
#define D_INITIALIZED "Zainicjowany"
#define D_IP_ADDRESS "Adres IP"
#define D_LIGHT "Swiatlo"
#define D_LOCAL_TIME "Lokalny"
#define D_LOW "Niski"
#define D_LWT "LWT"
#define D_MAC "Mac"
#define D_MASK "Maska"
#define D_MODULE "Modul"
#define D_MQTT "MQTT"
#define D_MULTI_PRESS "wielokrotne-nacisniecie"
#define D_NO "Nie"
#define D_NOISE "Halas"
#define D_NONE "Brak"
#define D_OFF "Wylaczony"
#define D_OFFLINE "Nieaktywny"
#define D_OK "Ok"
#define D_ON "Wlaczony"
#define D_ONLINE "Aktywny"
#define D_OR "lub"
#define D_PASSWORD "Haslo"
#define D_PERIOD "Okres"
#define D_PORT "Port"
#define D_POWER_FACTOR "Wspolczynik mocy"
#define D_POWERFACTOR "WspolczynikMocy"
#define D_POWERUSAGE "Moc"
#define D_PRESSURE "Cisnienie"
#define D_PRESSUREATSEALEVEL "Cisnienie na poziomie morza"
#define D_PROGRAM_FLASH_SIZE "Wielkosc programu flash"
#define D_PROGRAMFLASHSIZE "ProgramFlashSize"
#define D_PROGRAM_SIZE "Wielkosc programu"
#define D_PROGRAMSIZE "ProgramSize"
#define D_PROJECT "Projekt"
#define D_RECEIVED "Otrzymany"
#define D_RESTART "Restart"
#define D_RESTARTING "Restartowanie"
#define D_RESTART_REASON "Przyczyna restartu"
#define D_RESTARTREASON "PrzyczynaRestartu"
#define D_RESTORE "Przywracanie"
#define D_RETAINED "Zachowane"
#define D_SAVE "Zapisz"
#define D_SAVEADDRESS "AdresZapisu"
#define D_SAVECOUNT "LicznikZapisu"
#define D_SAVESTATE "StanZapisu"
#define D_SDKVERSION "SDK"
#define D_SELECTED "Wybrany"
#define D_SENSOR "Czujnik"
#define D_SERIAL "Serial"
#define D_BYTES "Bajtow"
#define D_SSID "SSID"
#define D_START "Start"
#define D_STARTED "Uruchomiony"
#define D_STARTDST "StartDST"        // Start Daylight Savings Time
#define D_STD_TIME "STD"
#define D_STOP "Stop"
#define D_SUBNET_MASK "Maska podsieci"
#define D_SUBNETMASK "MaskaPodsieci"
#define D_SUBSCRIBE_TO "Subskrybuj do"
#define D_SUCCESSFUL "Powodzenie"
#define D_SWITCH "Przelacznik"
#define D_SYNC "Sync"
#define D_SYS "System "                  // Sys log
#define D_TEMPERATURE "Temperatura"
#define D_TEMPERATURE_UNIT "JednTemp"
#define D_TIME "Czas"
#define D_TO "do"
#define D_TODAY "Dzisiaj"
#define D_TOGGLE "Przelacz"
#define D_TOPIC "Temat"
#define D_TOTAL "Suma"
#define D_TRANSMIT "Wyslij"
#define D_TRUE "Prawda"
#define D_TYPE "Typ"
#define D_UNKNOWN "Nieznany"
#define D_UPGRADE "aktualizacji"
#define D_UPLOAD "Wgraj"
#define D_UPTIME "Uptime"
#define D_USER "Uzytkownik"
#define D_UTC_TIME "UTC"
#define D_UV_LEVEL "Poziom UV"
#define D_VCC "VCC"
#define D_VERSION "Wersja"
#define D_VOLTAGE "Napiecie"
#define D_WARMLIGHT "Nagrzanie"
#define D_WEB "Web"                  // Web log
#define D_WEB_SERVER "Web Server"
#define D_WIFI "Wifi"
#define D_WRONG "Bledny"
#define D_YESTERDAY "Wczoraj"

// settings.ino
#define D_SAVED_TO_FLASH_AT "Zapisane do pamieci flash w"
#define D_LOADED_FROM_FLASH_AT "Wczytane z pamieci flash z"
#define D_USE_DEFAULTS "Uzyj domyslnych"
#define D_ERASED_SECTOR "Wymazany sektor"

// sonoff.ino
#define D_LEVEL_10 "poziom 1-0"
#define D_LEVEL_01 "poziom 0-1"
#define D_SERIAL_LOGGING_DISABLED "Wylaczony dziennik na porcie szeregowym"
#define D_SYSLOG_LOGGING_REENABLED "Wznowiono zapis do Syslog"

#define D_SET_BAUDRATE_TO "Ustaw szybkosc transmisji na"
#define D_RECEIVED_TOPIC "Otrzymany temat"
#define D_DATA_SIZE "Wielkosc danych"
#define D_ANALOG_INPUT "Wej_analogowe"

#define D_FINGERPRINT "Weryfikuj odcisk TLS..."
#define D_TLS_CONNECT_FAILED_TO "Nieudane polaczenie TLS do"
#define D_RETRY_IN "Sprobuj ponownie"
#define D_VERIFIED "Zweryfikowano"
#define D_INSECURE "Nieprawidlowe polaczenie z powodu blednego odcisku TLS"
#define D_CONNECT_FAILED_TO "Nie udalo sie nawiazac polaczenia"

// support.ino
#define D_OSWATCH "osWatch"
#define D_BLOCKED_LOOP "Petla zablokowana"
#define D_WPS_FAILED_WITH_STATUS "Blad WPSconfig ze statusem"
#define D_ACTIVE_FOR_1_MINUTE "aktywny 1 minute"
#define D_FAILED_TO_START "nie udalo sie uruchomic"
#define D_PATCH_ISSUE_2186 "Blad latki 2186"
#define D_CONNECTING_TO_AP "Laczenie z AP"
#define D_IN_MODE "w trybie"
#define D_CONNECT_FAILED_NO_IP_ADDRESS "Polaczenie nie powiodlo sie, poniewaz nie otrzymano adresu IP"
#define D_CONNECT_FAILED_AP_NOT_REACHED "Polaczenie nie powiodlo sie jako AP nie mozna osiagnac"
#define D_CONNECT_FAILED_WRONG_PASSWORD "Polaczenie nie powiodlo sie z nieprawidlowym haslem dostepu AP"
#define D_CONNECT_FAILED_AP_TIMEOUT "Nie udalo sie nawiazac polaczenia, limit czasu AP przekroczony"
#define D_ATTEMPTING_CONNECTION "Proba polaczenia..."
#define D_CHECKING_CONNECTION "Sprawdzanie polaczenia..."
#define D_QUERY_DONE "Wykonano zapytanie. Znaleziono uslugi MQTT"
#define D_MQTT_SERVICE_FOUND "Usluga MQTT została znaleziona"
#define D_I2CSCAN_DEVICES_FOUND_AT "Znaleziono urządzenie(a) na"
#define D_FOUND_AT "znalezione w"
#define D_I2CSCAN_UNKNOWN_ERROR_AT "Nieznany blad w"
#define D_I2CSCAN_NO_DEVICES_FOUND "Nie znaleziono urzadzen"
#define D_SYSLOG_HOST_NOT_FOUND "Syslog Host nie znaleziony"

// webserver.ino
#define D_MINIMAL_FIRMWARE_PLEASE_UPGRADE "Oprogramowanie MINIMAL - prosze uaktualnic"
#define D_WEBSERVER_ACTIVE_ON "Aktywny serwer Web"
#define D_WITH_IP_ADDRESS "z adresem IP"
#define D_WEBSERVER_STOPPED "Serwer Web zatrzymany"
#define D_FILE_NOT_FOUND "Plik nie odnaleziony"
#define D_REDIRECTED "Przekierowano do wlasnego portalu"
#define D_WIFIMANAGER_SET_ACCESSPOINT_AND_STATION "Menedzer Wifi ustawia punkt dostepowy i zachowuje stacje"
#define D_WIFIMANAGER_SET_ACCESSPOINT "Menedzer sieci Wi-Fi Access Point"
#define D_TRYING_TO_CONNECT "Probuje polaczyc urzadzenie z siecia"

#define D_RESTART_IN "Zrestartuj"
#define D_SECONDS "sekund"
#define D_DEVICE_WILL_RESTART "Urzadzenie zrestartuje sie za kilka sekund"
#define D_BUTTON_TOGGLE "Przelacznik"
#define D_CONFIGURATION "Konfiguracja"
#define D_INFORMATION "Informacje"
#define D_FIRMWARE_UPGRADE "Uaktualnienie oprogramowania"
#define D_CONSOLE "Konsola"
#define D_CONFIRM_RESTART "Potwierdz restart"

#define D_CONFIGURE_MODULE "Konfiguruj modul"
#define D_CONFIGURE_WIFI "Konfiguruj WiFi"
#define D_CONFIGURE_MQTT "Konfiguruj MQTT"
#define D_CONFIGURE_DOMOTICZ "Konfiguruj Domoticz"
#define D_CONFIGURE_LOGGING "Konfiguruj zapis dziennika"
#define D_CONFIGURE_OTHER "Konfiguruj inne"
#define D_CONFIRM_RESET_CONFIGURATION "Potwierdz reset ustawien"
#define D_RESET_CONFIGURATION "Reset ustawien"
#define D_BACKUP_CONFIGURATION "Kopia ustawien"
#define D_RESTORE_CONFIGURATION "Przywrocenie ustawien"
#define D_MAIN_MENU "Menu glowne"

#define D_MODULE_PARAMETERS "Parametery modulu"
#define D_MODULE_TYPE "Typ modulu"
#define D_GPIO "GPIO"
#define D_SERIAL_IN "Serial In"
#define D_SERIAL_OUT "Serial Out"

#define D_WIFI_PARAMETERS "Parametry WiFi"
#define D_SCAN_FOR_WIFI_NETWORKS "Skanuj sieci WiFi"
#define D_SCAN_DONE "Skan wykonany"
#define D_NO_NETWORKS_FOUND "Nie znaleziono sieci"
#define D_REFRESH_TO_SCAN_AGAIN "Odswiez aby ponownie zeskanowac"
#define D_DUPLICATE_ACCESSPOINT "Kopiuj AP"
#define D_SKIPPING_LOW_QUALITY "Pominieto z powodu niskiej jakości sygnalu"
#define D_RSSI "RSSI"
#define D_WEP "WEP"
#define D_WPA_PSK "WPA PSK"
#define D_WPA2_PSK "WPA2 PSK"
#define D_AP1_SSID "AP1 SSID"
#define D_AP1_PASSWORD "Haslo AP1"
#define D_AP2_SSID "AP2 SSID"
#define D_AP2_PASSWORD "Haslo AP2"

#define D_MQTT_PARAMETERS "Parametery MQTT"
#define D_CLIENT "Klient"
#define D_FULL_TOPIC "Pelen temat"

#define D_LOGGING_PARAMETERS "Opcje dziennika"
#define D_LOG_LEVEL "poziom dziennika"
#define D_MORE_DEBUG "Wiecej informacji debugujacych"
#define D_SYSLOG_HOST "Syslog host"
#define D_SYSLOG_PORT "Syslog port"
#define D_TELEMETRY_PERIOD "Okres telemetrii"

#define D_OTHER_PARAMETERS "Inne parametery"
#define D_WEB_ADMIN_PASSWORD "Haslo administratora Web"
#define D_MQTT_ENABLE "MQTT aktywne"
#define D_FRIENDLY_NAME "Przyjazna nazwa"
#define D_BELKIN_WEMO "Belkin WeMo"
#define D_HUE_BRIDGE "Hue Bridge"
#define D_SINGLE_DEVICE "single device"
#define D_MULTI_DEVICE "multi device"

#define D_SAVE_CONFIGURATION "Zapisz ustawienia"
#define D_CONFIGURATION_SAVED "Ustawienia zapisane"
#define D_CONFIGURATION_RESET "Ustawienia zresetowane"

#define D_PROGRAM_VERSION "Wersja programu"
#define D_BUILD_DATE_AND_TIME "Dzien i godzina kompilacji"
#define D_CORE_AND_SDK_VERSION "Wersja Core/SDK"
#define D_FLASH_WRITE_COUNT "Liczba zapisow do pamieci"
#define D_MAC_ADDRESS "Adres Mac"
#define D_MQTT_HOST "Host MQTT"
#define D_MQTT_PORT "Port MQTT"
#define D_MQTT_CLIENT "Klient MQTT"
#define D_MQTT_USER "Uzytkownik MQTT"
#define D_MQTT_TOPIC "Temat MQTT"
#define D_MQTT_GROUP_TOPIC "Temat grupy MQTT"
#define D_MQTT_FULL_TOPIC "Pelen temat MQTT"
#define D_MDNS_DISCOVERY "Wykrywanie mDNS"
#define D_MDNS_ADVERTISE "Rozglaszanie mDNS"
#define D_ESP_CHIP_ID "ID ukladu ESP"
#define D_FLASH_CHIP_ID "ID ukladu pamieci flash"
#define D_FLASH_CHIP_SIZE "Rozmiar flash"
#define D_FREE_PROGRAM_SPACE "Wolne miejsce programu"

#define D_UPGRADE_BY_WEBSERVER "Aktualizacja z serwera Web"
#define D_OTA_URL "URL OTA"
#define D_START_UPGRADE "Start aktualizacji"
#define D_UPGRADE_BY_FILE_UPLOAD "Aktualizacja poprzez wgranie pliku"
#define D_UPLOAD_STARTED "Wgrywanie rozpoczete"
#define D_UPGRADE_STARTED "Aktualizacja rozpoczeta"
#define D_UPLOAD_DONE "Wgrywanie zakonczone"
#define D_UPLOAD_ERR_1 "Nie wybrano pliku"
#define D_UPLOAD_ERR_2 "Niewystarczajaca ilosc miejsca"
#define D_UPLOAD_ERR_3 "Magiczny bajt jest inny niz 0xE9"
#define D_UPLOAD_ERR_4 "Rozmiar programu jest wiekszy niz rzeczywisty rozmiar pamieci flash"
#define D_UPLOAD_ERR_5 "Wgrywanie, bufor niezgodnosci stanu porownywanych bitow"
#define D_UPLOAD_ERR_6 "Blad wgrywania. Uruchomiono zapis do dziennika na poziomie 3"
#define D_UPLOAD_ERR_7 "Wgrywanie przerwane"
#define D_UPLOAD_ERR_8 "Bledny plik"
#define D_UPLOAD_ERR_9 "Plik jest za duzy"
#define D_UPLOAD_ERROR_CODE "Blad wgrywania"

#define D_ENTER_COMMAND "Wprowadz polecenie"
#define D_ENABLE_WEBLOG_FOR_RESPONSE "Wlacz poziom 2 zapisu Weblog, jesli oczekiwana jest odpowiedz"
#define D_NEED_USER_AND_PASSWORD "Wymagany uzytkownik=<username>&haslo=<password>"

// xdrv_domoticz.ino
#define D_DOMOTICZ_PARAMETERS "Parametry Domoticz"
#define D_DOMOTICZ_IDX "Idx"
#define D_DOMOTICZ_KEY_IDX "Key idx"
#define D_DOMOTICZ_SWITCH_IDX "Przelacznik idx"
#define D_DOMOTICZ_SENSOR_IDX "Sensor idx"
  #define D_DOMOTICZ_TEMP "Temp"
  #define D_DOMOTICZ_TEMP_HUM "Temp,Wilg"
  #define D_DOMOTICZ_TEMP_HUM_BARO "Temp,Wilg,Cis"
  #define D_DOMOTICZ_POWER_ENERGY "Moc,Energia"
  #define D_DOMOTICZ_ILLUMINANCE "Oswietl."
  #define D_DOMOTICZ_COUNT "Licznik"
  #define D_DOMOTICZ_VOLTAGE "Napiecie"
  #define D_DOMOTICZ_CURRENT "Prad"
#define D_DOMOTICZ_UPDATE_TIMER "Zaktualizuj czasomierz"

// xdrv_irremote.ino
#define D_INVALID_JSON "Invalid JSON"
#define D_PROTOCOL_NOT_SUPPORTED "Protokol nie jest obslugiwany"
#define D_IR_PROTOCOL "PROTOCOL"
#define D_IR_BITS "BITS"
#define D_IR_DATA "DATA"
#define D_IRHVAC_VENDOR "VENDOR"
#define D_IRHVAC_POWER "POWER"
#define D_IRHVAC_MODE "MODE"
#define D_IRHVAC_FANSPEED "FANSPEED"
#define D_IRHVAC_TEMP "TEMP"
#define D_IRRECEIVED "IrReceived"

// xdrv_snfbridge.ino
#define D_RFRECEIVED "RfReceived"
#define D_START_LEARNING "Zacznij nauke"
#define D_SET_TO_DEFAULT "Ustaw domyslne"
#define D_DEFAULT_SENT "Domyslne wyslane"
#define D_LEARNED_SENT "Wyuczone wyslane"
#define D_LEARNING_ACTIVE "Wyuczone aktywne"
#define D_LEARN_FAILED "Uczenie niepowodzenie"
#define D_LEARNED "Wyuczone"
#define D_SAVED "Zapisane"

// xdrv_wemohue.ino
#define D_MULTICAST_DISABLED "Multicast jest wylaczony"
#define D_MULTICAST_REJOINED "Multicast (re)dolaczony"
#define D_MULTICAST_JOIN_FAILED "Multicast nie powiodlo sie"
#define D_FAILED_TO_SEND_RESPONSE "Nie udalo sie wyslac odpowiedzi"

#define D_WEMO "WeMo"
#define D_WEMO_BASIC_EVENT "WeMo podstawowe zdarzenie"
#define D_WEMO_EVENT_SERVICE "WeMo zdarzenie service"
#define D_WEMO_SETUP "WeMo setup"
#define D_RESPONSE_SENT "Odpowiedz wyslana"

#define D_HUE "Hue"
#define D_HUE_BRIDGE_SETUP "Hue setup"
#define D_HUE_API_NOT_IMPLEMENTED "Hue API nie zaimplementowane"
#define D_HUE_API "Hue API"
#define D_HUE_POST_ARGS "Hue POST args"
#define D_3_RESPONSE_PACKETS_SENT "3 pakiety odpowiedzi wysylane"

// xsns_03_hlw8012.ino
#define D_MAXPOWERREACHED "MaksMocOsiagnieta"
#define D_MAXPOWERREACHEDRETRY "MaksMocOsiagnietaPonowienie"
#define D_POWERMONITOR "MonitorMocy"
#define D_ENERGYMONITOR "MonitorEnergii"
#define D_MAXENERGYREACHED "MaksEnergiaOsiagnieta"
#define D_ENERGY_TODAY "Energia Dzisiaj"
#define D_ENERGY_YESTERDAY "Energia Wczoraj"
#define D_ENERGY_TOTAL "Energia suma"

// xsns_05_ds18b20.ino
#define D_SENSOR_BUSY "Czujnik DS18x20 zajety"
#define D_SENSOR_CRC_ERROR "Czujnik DS18x20 blad CRC"
#define D_SENSORS_FOUND "Znaleziono Czujnik DS18x20"

// xsns_06_dht.ino
#define D_TIMEOUT_WAITING_FOR "Trwa oczekiwanie"
#define D_START_SIGNAL_LOW "sygnał startowy niski"
#define D_START_SIGNAL_HIGH "sygnał startowy wysoki"
#define D_PULSE "pulse"
#define D_CHECKSUM_FAILURE "Bledna suma kontrolmna"

// xsns_07_sht1x.ino
#define D_SENSOR_DID_NOT_ACK_COMMAND "Czujnik nie otrzymal komendy ACK"
#define D_SHT1X_FOUND "SHT1X znaleziony"

// sonoff_template.h
// Max string length is 8 characters including suffixes
#define D_SENSOR_NONE     "Brak"
#define D_SENSOR_DHT11    "DHT11"
#define D_SENSOR_AM2301   "AM2301"
#define D_SENSOR_SI7021   "SI7021"
#define D_SENSOR_DS18X20  "DS18x20"
#define D_SENSOR_I2C_SCL  "I2C SCL"
#define D_SENSOR_I2C_SDA  "I2C SDA"
#define D_SENSOR_WS2812   "WS2812"
#define D_SENSOR_IRSEND   "IRsend"
#define D_SENSOR_SWITCH   "Przela"   // Suffix "1"
#define D_SENSOR_BUTTON   "Przyci"   // Suffix "1"
#define D_SENSOR_RELAY    "Przek"    // Suffix "1i"
#define D_SENSOR_LED      "Led"      // Suffix "1i"
#define D_SENSOR_PWM      "PWM"      // Suffix "1"
#define D_SENSOR_COUNTER  "Liczni"  // Suffix "1"
#define D_SENSOR_IRRECV   "IRrecv"
#define D_SENSOR_MHZ_RX   "MHZ Rx"
#define D_SENSOR_MHZ_TX   "MHZ Tx"
#define D_SENSOR_SPI_CS   "SPI CS"
#define D_SENSOR_SPI_DC   "SPI DC"
#define D_SENSOR_BACKLIGHT "BLight"

// Units
#define D_UNIT_AMPERE "A"
#define D_UNIT_HOUR "Godz"
#define D_UNIT_KILOOHM "kOhm"
#define D_UNIT_KILOWATTHOUR "kWh"
#define D_UNIT_LUX "lx"
#define D_UNIT_MICROSECOND "us"
#define D_UNIT_MILLIAMPERE "mA"
#define D_UNIT_MILLISECOND "ms"
#define D_UNIT_MINUTE "Min"
#define D_UNIT_PPM "ppm"
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

// Result
#define D_RSLT_RESULT "REZULTAT"
#define D_RSLT_INFO "INFO"
#define D_RSLT_STATE "STAN"
#define D_RSLT_SENSOR "SENSOR"
#define D_RSLT_UPTIME "UPTIME"
#define D_RSLT_MARGINS "MARGINES"
#define D_RSLT_WARNING "OSTRZEZENIE"
#define D_RSLT_ENERGY "ENERGIA"

// Commands
#define D_CMND_MQTTHOST "MqttHost"
#define D_CMND_MQTTPORT "MqttPort"
#define D_CMND_MQTTRETRY "MqttRetry"
#define D_CMND_STATETEXT "StateText"
#define D_CMND_MQTTFINGERPRINT "MqttFingerprint"
#define D_CMND_MQTTCLIENT "MqttClient"
#define D_CMND_MQTTUSER "MqttUser"
#define D_CMND_MQTTPASSWORD "MqttPassword"
#define D_CMND_FULLTOPIC "FullTopic"
#define D_CMND_PREFIX "Prefix"
  #define PRFX_MAX_STRING_LENGTH 5
  #define D_CMND "cmnd"
  #define D_STAT "stat"
  #define D_TELE "tele"
#define D_CMND_GROUPTOPIC "GroupTopic"
#define D_CMND_TOPIC "Topic"
#define D_CMND_BUTTONTOPIC "ButtonTopic"
#define D_CMND_SWITCHTOPIC "SwitchTopic"
#define D_CMND_BUTTONRETAIN "ButtonRetain"
#define D_CMND_SWITCHRETAIN "SwitchRetain"
#define D_CMND_POWERRETAIN "PowerRetain"
#define D_CMND_SENSORRETAIN "SensorRetain"
#define D_CMND_BACKLOG "Backlog"
#define D_CMND_DELAY "Delay"
#define D_CMND_STATUS "Status"
  #define D_STATUS1_PARAMETER "PRM"
  #define D_STATUS2_FIRMWARE "FWR"
  #define D_STATUS3_LOGGING "LOG"
  #define D_STATUS4_MEMORY "MEM"
  #define D_STATUS5_NETWORK "NET"
  #define D_STATUS6_MQTT "MQT"
  #define D_STATUS7_TIME "TIM"
  #define D_STATUS8_POWER "PWR"
  #define D_STATUS9_MARGIN "PTH"
  #define D_STATUS10_SENSOR "SNS"
  #define D_STATUS11_STATUS "STS"
#define D_CMND_POWER "Power"
  #define D_RSLT_POWER "POWER"
#define D_CMND_POWERONSTATE "PowerOnState"
#define D_CMND_PULSETIME "PulseTime"
#define D_CMND_BLINKTIME "BlinkTime"
#define D_CMND_BLINKCOUNT "BlinkCount"
#define D_CMND_SAVEDATA "SaveData"
#define D_CMND_SETOPTION "SetOption"
#define D_CMND_TEMPERATURE_RESOLUTION "TempRes"
#define D_CMND_HUMIDITY_RESOLUTION "HumRes"
#define D_CMND_PRESSURE_RESOLUTION "PressRes"
#define D_CMND_POWER_RESOLUTION "WattRes"
#define D_CMND_VOLTAGE_RESOLUTION "VoltRes"
#define D_CMND_CURRENT_RESOLUTION "AmpRes"
#define D_CMND_ENERGY_RESOLUTION "EnergyRes"
#define D_CMND_MODULE "Module"
#define D_CMND_MODULES "Modules"
#define D_CMND_GPIO "GPIO"
  #define D_NOT_SUPPORTED "Not supported"
#define D_CMND_GPIOS "GPIOs"
#define D_CMND_PWM "PWM"
#define D_CMND_PWMFREQUENCY "PWMFrequency"
#define D_CMND_PWMRANGE "PWMRange"
#define D_CMND_COUNTER "Counter"
#define D_CMND_COUNTERTYPE "CounterType"
#define D_CMND_COUNTERDEBOUNCE "CounterDebounce"
#define D_CMND_SLEEP "Sleep"
#define D_CMND_UPLOAD "Upload"
#define D_CMND_UPGRADE "Upgrade"
  #define D_ONE_OR_GT "1 or >%s to upgrade"
#define D_CMND_OTAURL "OtaUrl"
#define D_CMND_SERIALLOG "SerialLog"
#define D_CMND_SYSLOG "SysLog"
#define D_CMND_LOGHOST "LogHost"
#define D_CMND_LOGPORT "LogPort"
#define D_CMND_IPADDRESS "IPAddress"
#define D_CMND_NTPSERVER "NtpServer"
#define D_CMND_AP "Ap"
#define D_CMND_SSID "SSID"
#define D_CMND_PASSWORD "Password"
#define D_CMND_HOSTNAME "Hostname"
#define D_CMND_WIFICONFIG "WifiConfig"
  #define WCFG_MAX_STRING_LENGTH 12
  #define D_WCFG_0_RESTART "Restart"
  #define D_WCFG_1_SMARTCONFIG "SmartConfig"
  #define D_WCFG_2_WIFIMANAGER "WifiManager"
  #define D_WCFG_3_WPSCONFIG "WPSConfig"
  #define D_WCFG_4_RETRY "Retry"
  #define D_WCFG_5_WAIT "Wait"
#define D_CMND_FRIENDLYNAME "FriendlyName"
#define D_CMND_SWITCHMODE "SwitchMode"
#define D_CMND_WEBSERVER "Webserver"
  #define D_WEBSERVER_MODE "WebServerMode"
  #define D_ACTIVE_FOR "Active for"
  #define D_ON_DEVICE "on"
#define D_CMND_WEBPASSWORD "WebPassword"
#define D_CMND_WEBBC1 "WebButtonColor"
#define D_CMND_WEBBC2 "WebButtonColorHover"
#define D_CMND_WEBLOG "WebLog"
#define D_CMND_EMULATION "Emulation"
#define D_CMND_TELEPERIOD "TelePeriod"
#define D_CMND_RESTART "Restart"
  #define D_ONE_TO_RESTART "1 to restart"
#define D_CMND_RESET "Reset"
  #define D_RESET_AND_RESTARTING "Reset and Restarting"
  #define D_ONE_TO_RESET "1 to reset"
#define D_CMND_TIMEZONE "Timezone"
#define D_CMND_ALTITUDE "Altitude"
#define D_CMND_LEDPOWER "LedPower"
#define D_CMND_LEDSTATE "LedState"
#define D_CMND_CFGDUMP "CfgDump"
#define D_CMND_I2CSCAN "I2CScan"
#define D_CMND_INA219MODE "Ina219Mode"
#define D_CMND_EXCEPTION "Exception"

// Commands xdrv_domoticz.ino
#define D_CMND_DOMOTICZ "Domoticz"
#define D_CMND_IDX "Idx"
#define D_CMND_KEYIDX "KeyIdx"
#define D_CMND_SWITCHIDX "SwitchIdx"
#define D_CMND_SENSORIDX "SensorIdx"
#define D_CMND_UPDATETIMER "UpdateTimer"

// Commands xdrv_irremote.ino
#define D_CMND_IRSEND "IRSend"
#define D_CMND_IRHVAC "IRHVAC"

// Commands xdrv_light.ino
#define D_CMND_COLOR "Color"
#define D_CMND_COLORTEMPERATURE "CT"
#define D_CMND_DIMMER "Dimmer"
#define D_CMND_LED "Led"
#define D_CMND_LEDTABLE "LedTable"
#define D_CMND_FADE "Fade"
#define D_CMND_PIXELS "Pixels"
#define D_CMND_SCHEME "Scheme"
#define D_CMND_SPEED "Speed"
#define D_CMND_WAKEUP "Wakeup"
#define D_CMND_WAKEUPDURATION "WakeUpDuration"
#define D_CMND_WIDTH "Width"

// Commands xdrv_snfbridge.ino
#define D_CMND_RFCODE "RfCode"
#define D_CMND_RFHIGH "RfHigh"
#define D_CMND_RFHOST "RfHost"
#define D_CMND_RFKEY "RfKey"
#define D_CMND_RFLOW "RfLow"
#define D_CMND_RFSYNC "RfSync"

// Commands xsns_03_hlw8012.ino
#define D_CMND_POWERLOW "PowerLow"
#define D_CMND_POWERHIGH "PowerHigh"
#define D_CMND_VOLTAGELOW "VoltageLow"
#define D_CMND_VOLTAGEHIGH "VoltageHigh"
#define D_CMND_CURRENTLOW "CurrentLow"
#define D_CMND_CURRENTHIGH "CurrentHigh"
#define D_CMND_ENERGYRESET "EnergyReset"
#define D_CMND_HLWPCAL "HlwPcal"
#define D_CMND_HLWPSET "HlwPset"
#define D_CMND_HLWUCAL "HlwUcal"
#define D_CMND_HLWUSET "HlwUset"
#define D_CMND_HLWICAL "HlwIcal"
#define D_CMND_HLWISET "HlwIset"
#define D_CMND_MAXPOWER "MaxPower"
#define D_CMND_MAXPOWERHOLD "MaxPowerHold"
#define D_CMND_MAXPOWERWINDOW "MaxPowerWindow"
#define D_CMND_SAFEPOWER "SafePower"
#define D_CMND_SAFEPOWERHOLD "SafePowerHold"
#define D_CMND_SAFEPOWERWINDOW "SafePowerWindow"
#define D_CMND_MAXENERGY "MaxEnergy"
#define D_CMND_MAXENERGYSTART "MaxEnergyStart"

#endif  // _LANGUAGE_PL_PL_H_
