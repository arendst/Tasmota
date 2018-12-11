/*
  pl-PL-d.h - localization for Polish with diacritics - Poland for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends (translated by roblad - Robert L., upgraded by R. Turala)

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

#ifndef _LANGUAGE_PL_PL_D_H_
#define _LANGUAGE_PL_PL_D_H_

/*************************** ATTENTION *******************************\
 *
 * Due to memory constraints only UTF-8 is supported.
 * To save code space keep text as short as possible.
 * Time and Date provided by SDK can not be localized (yet).
 * Use online command StateText to translate ON, OFF, HOLD and TOGGLE.
 * Use online command Prefix to translate cmnd, stat and tele.
 *
 * Updated until v5.12.0d
\*********************************************************************/

//#define LANGUAGE_MODULE_NAME         // Enable to display "Module Generic" (ie Spanish), Disable to display "Generic Module" (ie English)

#define LANGUAGE_LCID 1045
// HTML (ISO 639-1) Language Code
#define D_HTML_LANGUAGE "pl"

// "2017-03-07T11:08:02" - ISO8601:2004
#define D_YEAR_MONTH_SEPARATOR "-"
#define D_MONTH_DAY_SEPARATOR "-"
#define D_DATE_TIME_SEPARATOR "T"
#define D_HOUR_MINUTE_SEPARATOR ":"
#define D_MINUTE_SECOND_SEPARATOR ":"

#define D_DAY3LIST "NiePonWtoŚroCzwPiąSob"
#define D_MONTH3LIST "StyLutMarKwiMajCzeLipSieWrzPaźLisGru"

// Non JSON decimal separator
#define D_DECIMAL_SEPARATOR ","

// Common
#define D_ADMIN "Admin"
#define D_AIR_QUALITY "Jakość powietrza"
#define D_AP "AP"                    // Access Point
#define D_AS "jak"
#define D_AUTO "AUTO"
#define D_BLINK "Miganie"
#define D_BLINKOFF "MiganieWył"
#define D_BOOT_COUNT "Licznik uruchomienia"
#define D_BRIGHTLIGHT "Jasny"
#define D_BSSID "BSSId"
#define D_BUTTON "Przycisk"
#define D_BY "przez"                    // Written by me
#define D_BYTES "Bajtow"
#define D_CELSIUS "Celsiusza"
#define D_CHANNEL "Kanał"
#define D_CO2 "Dwutlenku węgla"
#define D_CODE "kod"                 // Button code
#define D_COLDLIGHT "Zimny"
#define D_COMMAND "Komenda"
#define D_CONNECTED "Połączony"
#define D_COUNT "Licz"
#define D_COUNTER "Licznik"
#define D_CURRENT "Bieżący"          // As in Voltage and Current
#define D_DATA "Data"
#define D_DARKLIGHT "Ciemny"
#define D_DEBUG "Debug"
#define D_DISABLED "Zablokowany"
#define D_DISTANCE "Odległość"
#define D_DNS_SERVER "Serwer DNS"
#define D_DONE "Wykonane"
#define D_DST_TIME "DST"
#define D_ECO2 "eCO2"
#define D_EMULATION "Emulacja"
#define D_ENABLED "Otwarty"
#define D_ERASE "Nadpisz"
#define D_ERROR "Błąd"
#define D_FAHRENHEIT "Fahrenheita"
#define D_FAILED "Błędny"
#define D_FALLBACK "Awaryjny"
#define D_FALLBACK_TOPIC "Zastępczy temat"
#define D_FALSE "Fałsz"
#define D_FILE "Plik"
#define D_FREE_MEMORY "Wolna pamięć"
#define D_FREQUENCY "Frequency"
#define D_GAS "Gas"
#define D_GATEWAY "Brama"
#define D_GROUP "Grupa"
#define D_HOST "Serwer"
#define D_HOSTNAME "Nazwa serwera"
#define D_HUMIDITY "Wilgotność"
#define D_ILLUMINANCE "Oświetlenie"
#define D_IMMEDIATE "Natychmiastowe"      // Button immediate
#define D_INDEX "Indeks"
#define D_INFO "Informacja"
#define D_INFRARED "Podczerwień"
#define D_INITIALIZED "Zainicjowany"
#define D_IP_ADDRESS "Adres IP"
#define D_LIGHT "Światło"
#define D_LWT "LWT"
#define D_MODULE "Moduł"
#define D_MQTT "MQTT"
#define D_MULTI_PRESS "wielokrotne-naciśnięcie"
#define D_NOISE "Hałas"
#define D_NONE "Brak"
#define D_OFF "Wyłączony"
#define D_OFFLINE "Nieaktywny"
#define D_OK "Ok"
#define D_ON "Włączony"
#define D_ONLINE "Aktywny"
#define D_PASSWORD "Hasło"
#define D_PORT "Port"
#define D_POWER_FACTOR "Współczynik mocy"
#define D_POWERUSAGE "Moc"
#define D_POWERUSAGE_ACTIVE "Czynna Moc"
#define D_POWERUSAGE_APPARENT "Pozorna Moc"
#define D_POWERUSAGE_REACTIVE "Reaktywna Moc"
#define D_PRESSURE "Ciśnienie"
#define D_PRESSUREATSEALEVEL "Ciśnienie na poziomie morza"
#define D_PROGRAM_FLASH_SIZE "Wielkość programu flash"
#define D_PROGRAM_SIZE "Wielkość programu"
#define D_PROJECT "Projekt"
#define D_RAIN "Rain"
#define D_RECEIVED "Otrzymany"
#define D_RESTART "Restart"
#define D_RESTARTING "Restartowanie"
#define D_RESTART_REASON "Przyczyna restartu"
#define D_RESTORE "Przywracanie"
#define D_RETAINED "Zachowane"
#define D_RULE "Rule"
#define D_SAVE "Zapisz"
#define D_SENSOR "Czujnik"
#define D_SSID "SSID"
#define D_START "Start"
#define D_STD_TIME "STD"
#define D_STOP "Stop"
#define D_SUBNET_MASK "Maska podsieci"
#define D_SUBSCRIBE_TO "Subskrybuj do"
#define D_SUCCESSFUL "Powodzenie"
#define D_SUNRISE "Wschód słońca"
#define D_SUNSET "Zachód słońca"
#define D_TEMPERATURE "Temperatura"
#define D_TO "do"
#define D_TOGGLE "Przełącz"
#define D_TOPIC "Temat"
#define D_TRANSMIT "Wyślij"
#define D_TRUE "Prawda"
#define D_TVOC "TVOC"
#define D_UPGRADE "aktualizacji"
#define D_UPLOAD "Wgraj"
#define D_UPTIME "Czas pracy"
#define D_USER "Użytkownik"
#define D_UTC_TIME "UTC"
#define D_UV_INDEX "UV Index"
#define D_UV_INDEX_1 "Niski"
#define D_UV_INDEX_2 "Średni"
#define D_UV_INDEX_3 "Wysoki"
#define D_UV_INDEX_4 "Niebezpieczny"
#define D_UV_INDEX_5 "BurnL1/2"
#define D_UV_INDEX_6 "BurnL3"
#define D_UV_INDEX_7 "OoR"
#define D_UV_LEVEL "Poziom UV"
#define D_UV_POWER "UV Power"
#define D_VERSION "Wersja"
#define D_VOLTAGE "Napięcie"
#define D_WEIGHT "Weight"
#define D_WARMLIGHT "Nagrzanie"
#define D_WEB_SERVER "Web Serwer"

// sonoff.ino
#define D_WARNING_MINIMAL_VERSION "UWAGA Ta wersja nie obsługuje zapisu ustawień"
#define D_LEVEL_10 "poziom 1-0"
#define D_LEVEL_01 "poziom 0-1"
#define D_SERIAL_LOGGING_DISABLED "Wyłączony dziennik na porcie szeregowym"
#define D_SYSLOG_LOGGING_REENABLED "Wznowiono zapis do Syslog"

#define D_SET_BAUDRATE_TO "Ustaw szybkość transmisji na"
#define D_RECEIVED_TOPIC "Otrzymany temat"
#define D_DATA_SIZE "Wielkość danych"
#define D_ANALOG_INPUT "Wej_analogowe"

// support.ino
#define D_OSWATCH "osWatch"
#define D_BLOCKED_LOOP "Pętla zablokowana"
#define D_WPS_FAILED_WITH_STATUS "Błąd WPSconfig ze statusem"
#define D_ACTIVE_FOR_3_MINUTES "aktywny 3 minuty"
#define D_FAILED_TO_START "nie udało się uruchomić"
#define D_PATCH_ISSUE_2186 "Błąd łatki 2186"
#define D_CONNECTING_TO_AP "Łączenie z AP"
#define D_IN_MODE "w trybie"
#define D_CONNECT_FAILED_NO_IP_ADDRESS "Połączenie nie powiodło sie, ponieważ nie otrzymano adresu IP"
#define D_CONNECT_FAILED_AP_NOT_REACHED "Połączenie nie powiodło sie jako AP nie można osiągnać"
#define D_CONNECT_FAILED_WRONG_PASSWORD "Połączenie nie powiodło sie z nieprawidlowym hasłem dostepu AP"
#define D_CONNECT_FAILED_AP_TIMEOUT "Nie udało się nawiązac połączenia, limit czasu AP przekroczony"
#define D_ATTEMPTING_CONNECTION "Próba połączenia..."
#define D_CHECKING_CONNECTION "Sprawdzanie połączenia..."
#define D_QUERY_DONE "Wykonano zapytanie. Znaleziono uslugi MQTT"
#define D_MQTT_SERVICE_FOUND "Usługa MQTT została znaleziona"
#define D_FOUND_AT "znalezione w"
#define D_SYSLOG_HOST_NOT_FOUND "Syslog Host nie znaleziony"

// settings.ino
#define D_SAVED_TO_FLASH_AT "Zapisane do pamięci flash w"
#define D_LOADED_FROM_FLASH_AT "Wczytane z pamięci flash z"
#define D_USE_DEFAULTS "Użyj domyślnych"
#define D_ERASED_SECTOR "Wymazany sektor"

// xdrv_02_webserver.ino
#define D_NOSCRIPT "Aby korzystać z Tasmota, włącz obsługę JavaScript"
#define D_MINIMAL_FIRMWARE_PLEASE_UPGRADE "Oprogramowanie MINIMAL - proszę uaktualnić"
#define D_WEBSERVER_ACTIVE_ON "Aktywny serwer Web"
#define D_WITH_IP_ADDRESS "z adresem IP"
#define D_WEBSERVER_STOPPED "Serwer Web zatrzymany"
#define D_FILE_NOT_FOUND "Plik nie odnaleziony"
#define D_REDIRECTED "Przekierowano do własnego portalu"
#define D_WIFIMANAGER_SET_ACCESSPOINT_AND_STATION "Menedżer Wifi ustawia punkt dostępowy i zachowuje stacje"
#define D_WIFIMANAGER_SET_ACCESSPOINT "Menedżer sieci Wi-Fi Access Point"
#define D_TRYING_TO_CONNECT "Próbuję połączyć urzadzenie z siecią"

#define D_RESTART_IN "Zrestartuj"
#define D_SECONDS "sekund"
#define D_DEVICE_WILL_RESTART "Urządzenie zrestartuje sie za kilka sekund"
#define D_BUTTON_TOGGLE "Przełącznik"
#define D_CONFIGURATION "Konfiguracja"
#define D_INFORMATION "Informacje"
#define D_FIRMWARE_UPGRADE "Uaktualnienie oprogramowania"
#define D_CONSOLE "Konsola"
#define D_CONFIRM_RESTART "Potwierdź restart"

#define D_CONFIGURE_MODULE "Konfiguruj moduł"
#define D_CONFIGURE_WIFI "Konfiguruj WiFi"
#define D_CONFIGURE_MQTT "Konfiguruj MQTT"
#define D_CONFIGURE_DOMOTICZ "Konfiguruj Domoticz"
#define D_CONFIGURE_LOGGING "Konfiguruj zapis dziennika"
#define D_CONFIGURE_OTHER "Konfiguruj inne"
#define D_CONFIRM_RESET_CONFIGURATION "Potwierdź reset ustawień"
#define D_RESET_CONFIGURATION "Reset ustawień"
#define D_BACKUP_CONFIGURATION "Kopia ustawień"
#define D_RESTORE_CONFIGURATION "Przywrócenie ustawień"
#define D_MAIN_MENU "Menu główne"

#define D_MODULE_PARAMETERS "Parametery modułu"
#define D_MODULE_TYPE "Typ modułu"
#define D_GPIO "GPIO"
#define D_SERIAL_IN "Serial In"
#define D_SERIAL_OUT "Serial Out"

#define D_WIFI_PARAMETERS "Parametry WiFi"
#define D_SCAN_FOR_WIFI_NETWORKS "Skanuj sieci WiFi"
#define D_SCAN_DONE "Skan wykonany"
#define D_NO_NETWORKS_FOUND "Nie znaleziono sieci"
#define D_REFRESH_TO_SCAN_AGAIN "Odswież aby ponownie zeskanowac"
#define D_DUPLICATE_ACCESSPOINT "Kopiuj AP"
#define D_SKIPPING_LOW_QUALITY "Pominięto z powodu niskiej jakości sygnału"
#define D_RSSI "RSSI"
#define D_WEP "WEP"
#define D_WPA_PSK "WPA PSK"
#define D_WPA2_PSK "WPA2 PSK"
#define D_AP1_SSID "AP1 SSID"
#define D_AP1_PASSWORD "Hasło AP1"
#define D_AP2_SSID "AP2 SSID"
#define D_AP2_PASSWORD "Hasło AP2"

#define D_MQTT_PARAMETERS "Parametery MQTT"
#define D_CLIENT "Klient"
#define D_FULL_TOPIC "Pełen temat"

#define D_LOGGING_PARAMETERS "Opcje dziennika"
#define D_SERIAL_LOG_LEVEL "Serial poziom dziennika"
#define D_WEB_LOG_LEVEL "Web poziom dziennika"
#define D_SYS_LOG_LEVEL "System poziom dziennika"
#define D_MORE_DEBUG "Więcej informacji debugujacych"
#define D_SYSLOG_HOST "Syslog host"
#define D_SYSLOG_PORT "Syslog port"
#define D_TELEMETRY_PERIOD "Okres telemetrii"

#define D_OTHER_PARAMETERS "Inne parametery"
#define D_WEB_ADMIN_PASSWORD "Hasło administratora Web"
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
#define D_BUILD_DATE_AND_TIME "Dzień i godzina kompilacji"
#define D_CORE_AND_SDK_VERSION "Wersja Core/SDK"
#define D_FLASH_WRITE_COUNT "Liczba zapisów do pamięci"
#define D_MAC_ADDRESS "Adres Mac"
#define D_MQTT_HOST "Host MQTT"
#define D_MQTT_PORT "Port MQTT"
#define D_MQTT_CLIENT "Klient MQTT"
#define D_MQTT_USER "Uzytkownik MQTT"
#define D_MQTT_TOPIC "Temat MQTT"
#define D_MQTT_GROUP_TOPIC "Temat grupy MQTT"
#define D_MQTT_FULL_TOPIC "Pełen temat MQTT"
#define D_MDNS_DISCOVERY "Wykrywanie mDNS"
#define D_MDNS_ADVERTISE "Rozgłaszanie mDNS"
#define D_ESP_CHIP_ID "ID ukladu ESP"
#define D_FLASH_CHIP_ID "ID układu pamięci flash"
#define D_FLASH_CHIP_SIZE "Rozmiar flash"
#define D_FREE_PROGRAM_SPACE "Wolne miejsce programu"

#define D_UPGRADE_BY_WEBSERVER "Aktualizacja z serwera Web"
#define D_OTA_URL "URL OTA"
#define D_START_UPGRADE "Start aktualizacji"
#define D_UPGRADE_BY_FILE_UPLOAD "Aktualizacja poprzez wgranie pliku"
#define D_UPLOAD_STARTED "Wgrywanie rozpoczęte"
#define D_UPGRADE_STARTED "Aktualizacja rozpoczęta"
#define D_UPLOAD_DONE "Wgrywanie zakończone"
#define D_UPLOAD_ERR_1 "Nie wybrano pliku"
#define D_UPLOAD_ERR_2 "Niewystarczająca ilość miejsca"
#define D_UPLOAD_ERR_3 "Magiczny bajt jest inny niz 0xE9"
#define D_UPLOAD_ERR_4 "Rozmiar programu jest większy niż rzeczywisty rozmiar pamieci flash"
#define D_UPLOAD_ERR_5 "Wgrywanie, bufor niezgodnosci stanu porównywanych bitow"
#define D_UPLOAD_ERR_6 "Błąd wgrywania. Uruchomiono zapis do dziennika na poziomie 3"
#define D_UPLOAD_ERR_7 "Wgrywanie przerwane"
#define D_UPLOAD_ERR_8 "Błędny plik"
#define D_UPLOAD_ERR_9 "Plik jest za duży"
#define D_UPLOAD_ERR_10 "Błąd inicjacji układu RF"
#define D_UPLOAD_ERR_11 "Błąd kasowania układu RF"
#define D_UPLOAD_ERR_12 "Błąd zapisu układu RF"
#define D_UPLOAD_ERR_13 "Błąd dekodowania oprrogramowania układu RF"
#define D_UPLOAD_ERROR_CODE "Błąd wgrywania"

#define D_ENTER_COMMAND "Wprowadź polecenie"
#define D_ENABLE_WEBLOG_FOR_RESPONSE "Włącz poziom 2 zapisu Weblog, jeśli oczekiwana jest odpowiedź"
#define D_NEED_USER_AND_PASSWORD "Wymagany użytkownik=<username>&hasło=<password>"

// xdrv_01_mqtt.ino
#define D_FINGERPRINT "Weryfikuj odcisk TLS..."
#define D_TLS_CONNECT_FAILED_TO "Nieudane połączenie TLS do"
#define D_RETRY_IN "Spróbuj ponownie"
#define D_VERIFIED "Zweryfikowano odcisku"
#define D_INSECURE "Nieprawidłowe połączenie z powodu błędnego odcisku TLS"
#define D_CONNECT_FAILED_TO "Nie udało się nawiązać połączenia"

// xplg_wemohue.ino
#define D_MULTICAST_DISABLED "Multicast jest wyłączony"
#define D_MULTICAST_REJOINED "Multicast (re)dołączony"
#define D_MULTICAST_JOIN_FAILED "Multicast nie powiodło się"
#define D_FAILED_TO_SEND_RESPONSE "Nie udało się wysłać odpowiedzi"

#define D_WEMO "WeMo"
#define D_WEMO_BASIC_EVENT "WeMo podstawowe zdarzenie"
#define D_WEMO_EVENT_SERVICE "WeMo zdarzenie service"
#define D_WEMO_META_SERVICE "WeMo meta service"
#define D_WEMO_SETUP "WeMo setup"
#define D_RESPONSE_SENT "Odpowiedź wysłana"

#define D_HUE "Hue"
#define D_HUE_BRIDGE_SETUP "Hue setup"
#define D_HUE_API_NOT_IMPLEMENTED "Hue API nie zaimplementowane"
#define D_HUE_API "Hue API"
#define D_HUE_POST_ARGS "Hue POST args"
#define D_3_RESPONSE_PACKETS_SENT "3 pakiety odpowiedzi wysyłane"

// xdrv_07_domoticz.ino
#define D_DOMOTICZ_PARAMETERS "Parametry Domoticz"
#define D_DOMOTICZ_IDX "Idx"
#define D_DOMOTICZ_KEY_IDX "Key idx"
#define D_DOMOTICZ_SWITCH_IDX "Przełącznik idx"
#define D_DOMOTICZ_SENSOR_IDX "Sensor idx"
  #define D_DOMOTICZ_TEMP "Temp"
  #define D_DOMOTICZ_TEMP_HUM "Temp,Wilg"
  #define D_DOMOTICZ_TEMP_HUM_BARO "Temp,Wilg,Cis"
  #define D_DOMOTICZ_POWER_ENERGY "Moc,Energia"
  #define D_DOMOTICZ_ILLUMINANCE "Oświetl."
  #define D_DOMOTICZ_COUNT "Licznik/PM1"
  #define D_DOMOTICZ_VOLTAGE "Napięcie/PM2,5"
  #define D_DOMOTICZ_CURRENT "Prąd/PM10"
  #define D_DOMOTICZ_AIRQUALITY "AirQuality"
#define D_DOMOTICZ_UPDATE_TIMER "Zaktualizuj czasomierz"

// xdrv_09_timers.ino
#define D_CONFIGURE_TIMER "Skonfiguruj harmonogram"
#define D_TIMER_PARAMETERS "Parametry harmonogramów"
#define D_TIMER_ENABLE "Włącz Harmonogramy"
#define D_TIMER_ARM "Włącz"
#define D_TIMER_TIME "Czas"
#define D_TIMER_DAYS "Dni"
#define D_TIMER_REPEAT "Powtarzaj"
#define D_TIMER_OUTPUT "Wyjście"
#define D_TIMER_ACTION "Akcja"

// xdrv_10_knx.ino
#define D_CONFIGURE_KNX "Skonfiguruj KNX"
#define D_KNX_PARAMETERS "Parametry KNX"
#define D_KNX_GENERAL_CONFIG "Ogólne"
#define D_KNX_PHYSICAL_ADDRESS "Adres Fizyczny"
#define D_KNX_PHYSICAL_ADDRESS_NOTE "( Musi być unikalny w sieci KNX )"
#define D_KNX_ENABLE "Włącz KNX"
#define D_KNX_GROUP_ADDRESS_TO_WRITE "Dane do wysłania do adresów grupowych"
#define D_ADD "Dodaj"
#define D_DELETE "Usuń"
#define D_REPLY "Odpowiedz"
#define D_KNX_GROUP_ADDRESS_TO_READ "Adresy grupowe do odbioru danych z"
#define D_LOG_KNX "KNX: "
#define D_RECEIVED_FROM "Otrzymane od"
#define D_KNX_COMMAND_WRITE "Zapisz"
#define D_KNX_COMMAND_READ "Czytaj"
#define D_KNX_COMMAND_OTHER "Inne"
#define D_SENT_TO "wysłane do"
#define D_KNX_WARNING "Adres grupy (0/0/0) jest zarezerwowany i nie można go użyć."
#define D_KNX_ENHANCEMENT "Usprawnienie Komunikacji"
#define D_KNX_TX_SLOT "KNX TX"
#define D_KNX_RX_SLOT "KNX RX"

// xdrv_03_energy.ino
#define D_ENERGY_TODAY "Energia Dzisiaj"
#define D_ENERGY_YESTERDAY "Energia Wczoraj"
#define D_ENERGY_TOTAL "Energia suma"

// xsns_05_ds18b20.ino
#define D_SENSOR_BUSY "Czujnik DS18x20 zajęty"
#define D_SENSOR_CRC_ERROR "Czujnik DS18x20 błąd CRC"
#define D_SENSORS_FOUND "Znaleziono Czujnik DS18x20"

// xsns_06_dht.ino
#define D_TIMEOUT_WAITING_FOR "Trwa oczekiwanie"
#define D_START_SIGNAL_LOW "sygnał startowy niski"
#define D_START_SIGNAL_HIGH "sygnał startowy wysoki"
#define D_PULSE "pulse"
#define D_CHECKSUM_FAILURE "Błędna suma kontrolna"

// xsns_07_sht1x.ino
#define D_SENSOR_DID_NOT_ACK_COMMAND "Czujnik nie otrzymał komendy ACK"
#define D_SHT1X_FOUND "SHT1X znaleziony"

// xsns_18_pms5003.ino
#define D_STANDARD_CONCENTRATION "CF-1 PM"     // Standard Particle CF-1 Particle Matter
#define D_ENVIRONMENTAL_CONCENTRATION "PM"     // Environmetal Particle Matter
#define D_PARTICALS_BEYOND "Cząstki"

// xsns_32_mpu6050.ino
#define D_AX_AXIS "Accel. X-Axis"
#define D_AY_AXIS "Accel. Y-Axis"
#define D_AZ_AXIS "Accel. Z-Axis"
#define D_GX_AXIS "Gyro X-Axis"
#define D_GY_AXIS "Gyro Y-Axis"
#define D_GZ_AXIS "Gyro Z-Axis"

// xsns_34_hx711.ino
#define D_HX_CAL_REMOVE "Usuń wagę"
#define D_HX_CAL_REFERENCE "Załaduj masę referencyjną"
#define D_HX_CAL_DONE "Skalibrowany"
#define D_HX_CAL_FAIL "Błąd Kalibracji"
#define D_RESET_HX711 "Zresetuj Skalę"
#define D_CONFIGURE_HX711 "Skonfiguruj Skalę"
#define D_HX711_PARAMETERS "Parametry Skali"
#define D_ITEM_WEIGHT "Waga przedmiotu"
#define D_REFERENCE_WEIGHT "Waga referencyjna"
#define D_CALIBRATE "Skalibruj"
#define D_CALIBRATION "Kalibrowanie"

//xsns_35_tx20.ino
#define D_TX20_WIND_DIRECTION "Kierunek wiatru"
#define D_TX20_WIND_SPEED "Prędkość wiatru"
#define D_TX20_WIND_SPEED_AVG "Średnia prędkość wiatru"
#define D_TX20_WIND_SPEED_MAX "Maksymalna prędkość wiatru"
#define D_TX20_NORTH "N"
#define D_TX20_EAST "E"
#define D_TX20_SOUTH "S"
#define D_TX20_WEST "W"

// sonoff_template.h
#define D_SENSOR_NONE     "Brak"
#define D_SENSOR_DHT11    "DHT11"
#define D_SENSOR_AM2301   "AM2301"
#define D_SENSOR_SI7021   "SI7021"
#define D_SENSOR_DS18X20  "DS18x20"
#define D_SENSOR_I2C_SCL  "I2C SCL"
#define D_SENSOR_I2C_SDA  "I2C SDA"
#define D_SENSOR_WS2812   "WS2812"
#define D_SENSOR_DFR562   "MP3 Player"
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
#define D_SENSOR_PZEM004_RX  "PZEM004 Rx"
#define D_SENSOR_PZEM016_RX  "PZEM016 Rx"
#define D_SENSOR_PZEM017_RX  "PZEM017 Rx"
#define D_SENSOR_PZEM0XX_TX  "PZEM0XX Tx"
#define D_SENSOR_SAIR_RX  "SAir Rx"
#define D_SENSOR_SAIR_TX  "SAir Tx"
#define D_SENSOR_SPI_CS   "SPI CS"
#define D_SENSOR_SPI_DC   "SPI DC"
#define D_SENSOR_BACKLIGHT "BkLight"
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
#define D_UNIT_HOUR "Godz"
#define D_UNIT_INCREMENTS "inc"
#define D_UNIT_KILOGRAM "kg"
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
#define D_UNIT_MINUTE "Min"
#define D_UNIT_PARTS_PER_BILLION "ppb"
#define D_UNIT_PARTS_PER_DECILITER "ppd"
#define D_UNIT_PARTS_PER_MILLION "ppm"
#define D_UNIT_PRESSURE "hPa"
#define D_UNIT_SECOND "sec"
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

#endif  // _LANGUAGE_PL_PL_D_H_
