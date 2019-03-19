/*
  tr-TR.h - localization for Turkish - Turkey for Sonoff-Tasmota

  Copyright (C) 2019  Ali Sait Teke and Theo Arends

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

#ifndef _LANGUAGE_TR_TR_H_
#define _LANGUAGE_TR_TR_H_

/*************************** ATTENTION *******************************\
 *
 * Due to memory constraints only UTF-8 is supported.
 * To save code space keep text as short as possible.
 * Time and Date provided by SDK can not be localized (yet).
 * Use online command StateText to translate ON, OFF, HOLD and TOGGLE.
 * Use online command Prefix to translate cmnd, stat and tele.
 *
 * Updated until v6.1.1
\*********************************************************************/

//#define LANGUAGE_MODULE_NAME         // Enable to display "Module Generic" (ie Spanish), Disable to display "Generic Module" (ie English)

#define LANGUAGE_LCID 1055
// HTML (ISO 639-1) Language Code
#define D_HTML_LANGUAGE "tr"

// "2017-03-07T11:08:02" - ISO8601:2004
#define D_YEAR_MONTH_SEPARATOR "."
#define D_MONTH_DAY_SEPARATOR "."
#define D_DATE_TIME_SEPARATOR "T"
#define D_HOUR_MINUTE_SEPARATOR ":"
#define D_MINUTE_SECOND_SEPARATOR ":"

#define D_DAY3LIST "PazPztSalCarPerCumCmt"
#define D_MONTH3LIST "OckSubMarNisMayHazTemAguEylEkmKasAra"

// Non JSON decimal separator
#define D_DECIMAL_SEPARATOR "."

// Common
#define D_ADMIN "Admin"
#define D_AIR_QUALITY "Hava Kalitesi"
#define D_AP "AP"                    // Access Point
#define D_AS "as"
#define D_AUTO "OTOMATIK"
#define D_BLINK "Blink"
#define D_BLINKOFF "BlinkOff"
#define D_BOOT_COUNT "Yeniden başlama sayısı"
#define D_BRIGHTLIGHT "Işık"
#define D_BSSID "BSSId"
#define D_BUTTON "Buton"
#define D_BY "by"                    // Written by me
#define D_BYTES "Bayt"
#define D_CELSIUS "Derece"
#define D_CHANNEL "Channel"
#define D_CO2 "Karbon dioksit"
#define D_CODE "kod"                // Button code
#define D_COLDLIGHT "Soğuk"
#define D_COMMAND "Komut"
#define D_CONNECTED "Bağlandı"
#define D_COUNT "Sayı"
#define D_COUNTER "Sayaç"
#define D_CURRENT "Current"          // As in Voltage and Current
#define D_DATA "Data"
#define D_DARKLIGHT "Karanlık"
#define D_DEBUG "Hata Ayıklama"
#define D_DISABLED "Etkin Değil"
#define D_DISTANCE "Mesage"
#define D_DNS_SERVER "DNS Sunucu"
#define D_DONE "Tamam"
#define D_DST_TIME "DST"
#define D_ECO2 "eCO2"
#define D_EMULATION "Emülasyon"
#define D_ENABLED "Etkin"
#define D_ERASE "Temizle"
#define D_ERROR "Hata"
#define D_FAHRENHEIT "Fahrenayt"
#define D_FAILED "Başlarısız"
#define D_FALLBACK "Geri iletim"
#define D_FALLBACK_TOPIC "Geri İletim Topiği"
#define D_FALSE "False"
#define D_FILE "Dosya"
#define D_FREE_MEMORY "Boş Hafıza"
#define D_FREQUENCY "Frekans"
#define D_GAS "Gas"
#define D_GATEWAY "Geçit"
#define D_GROUP "Grup"
#define D_HOST "Host"
#define D_HOSTNAME "Host Adresi"
#define D_HUMIDITY "Nem"
#define D_ILLUMINANCE "Aydınlık"
#define D_IMMEDIATE "derhal"      // Button immediate
#define D_INDEX "Index"
#define D_INFO "Bilgi"
#define D_INFRARED "Kızılötesi"
#define D_INITIALIZED "Başlatıldı"
#define D_IP_ADDRESS "IP Adresi"
#define D_LIGHT "Işık"
#define D_LWT "LWT"
#define D_MODULE "Modül"
#define D_MQTT "MQTT"
#define D_MULTI_PRESS "multi-press"
#define D_NOISE "Noise"
#define D_NONE "None"
#define D_OFF "Off"
#define D_OFFLINE "Çevirimdışı"
#define D_OK "Tamam"
#define D_ON "On"
#define D_ONLINE "Çevirimiçi"
#define D_PASSWORD "Şifre"
#define D_PORT "Port"
#define D_POWER_FACTOR "Güç Faktörü"
#define D_POWERUSAGE "Güç"
#define D_POWERUSAGE_ACTIVE "Aktif Güç"
#define D_POWERUSAGE_APPARENT "Görünen Güç"
#define D_POWERUSAGE_REACTIVE "Reaktif Güç"
#define D_PRESSURE "Basınç"
#define D_PRESSUREATSEALEVEL "Deniz Seviyesi Basıncı"
#define D_PROGRAM_FLASH_SIZE "Yazılım Flash Boyutu"
#define D_PROGRAM_SIZE "Yazılım Boyutu"
#define D_PROJECT "Proje"
#define D_RAIN "Rain"
#define D_RECEIVED "Alınan"
#define D_RESTART "Yeniden Başlat"
#define D_RESTARTING "Yeniden Başlatılıyor"
#define D_RESTART_REASON "Yeniden Başlatma Sebebi"
#define D_RESTORE "restore"
#define D_RETAINED "tutulan"
#define D_RULE "Kural"
#define D_SAVE "Kaydet"
#define D_SENSOR "Sensör"
#define D_SSID "SSId"
#define D_START "Başlat"
#define D_STD_TIME "STD"
#define D_STOP "Durdur"
#define D_SUBNET_MASK "Altağ Geçidi Maskesi"
#define D_SUBSCRIBE_TO "Abone olunan"
#define D_UNSUBSCRIBE_FROM "Unsubscribe from"
#define D_SUCCESSFUL "Başarıyla Tamamlandı"
#define D_SUNRISE "Gün doğumu"
#define D_SUNSET "Gün batımı"
#define D_TEMPERATURE "Sıcaklık"
#define D_TO "den"
#define D_TOGGLE "Geçiş Tuşu"
#define D_TOPIC "Başlık"
#define D_TRANSMIT "İletim"
#define D_TRUE "True"
#define D_TVOC "TVOC"
#define D_UPGRADE "yükseltme"
#define D_UPLOAD "Yükleme"
#define D_UPTIME "Açık Kalma Süresi"
#define D_USER "Kullanıcı"
#define D_UTC_TIME "UTC"
#define D_UV_INDEX "UV Indeksi"
#define D_UV_INDEX_1 "Low"
#define D_UV_INDEX_2 "Mid"
#define D_UV_INDEX_3 "High"
#define D_UV_INDEX_4 "Danger"
#define D_UV_INDEX_5 "BurnL1/2"
#define D_UV_INDEX_6 "BurnL3"
#define D_UV_INDEX_7 "OoR"
#define D_UV_LEVEL "UV Seviyesi"
#define D_UV_POWER "UV Power"
#define D_VERSION "Versiyon"
#define D_VOLTAGE "Voltaj"
#define D_WEIGHT "Weight"
#define D_WARMLIGHT "Sıcak"
#define D_WEB_SERVER "Web Sunucusu"

// sonoff.ino
#define D_WARNING_MINIMAL_VERSION "UYARI Bu versiyon ayarların kalıcı olarak kaydedilmesine olanak sağlamıyor"
#define D_LEVEL_10 "seviye 1-0"
#define D_LEVEL_01 "seviye 0-1"
#define D_SERIAL_LOGGING_DISABLED "Seri port loglaması kapalı"
#define D_SYSLOG_LOGGING_REENABLED "Sistem loglaması tekrar aktif"

#define D_SET_BAUDRATE_TO "Baud hızını şu şekilde değiştir"
#define D_RECEIVED_TOPIC "Alınan Başlık"
#define D_DATA_SIZE "Veri Büyüklüğü"
#define D_ANALOG_INPUT "Analog"

// support.ino
#define D_OSWATCH "osWatch"
#define D_BLOCKED_LOOP "Döngü bloke edildi"
#define D_WPS_FAILED_WITH_STATUS "WPSconfig FAILED with status"
#define D_ACTIVE_FOR_3_MINUTES "3 dakika aktif"
#define D_FAILED_TO_START "başlatma başarılı olamadı"
#define D_PATCH_ISSUE_2186 "Patch issue 2186"
#define D_CONNECTING_TO_AP "AP'ye bağlan"
#define D_IN_MODE "modunda"
#define D_CONNECT_FAILED_NO_IP_ADDRESS "IP adresi alınamadığı için bağlantı kurulamadı"
#define D_CONNECT_FAILED_AP_NOT_REACHED "Ap'ye ulaşılamadı"
#define D_CONNECT_FAILED_WRONG_PASSWORD "Girilen parolayı AP kabul etmedi"
#define D_CONNECT_FAILED_AP_TIMEOUT "AP'ye bağlanılırken süre aşımı oluştu"
#define D_ATTEMPTING_CONNECTION "Yeniden bağlanılıyor..."
#define D_CHECKING_CONNECTION "Bağlantı kontrol ediliyor..."
#define D_QUERY_DONE "Soru tamamlandı. MQTT servisleri bulundu"
#define D_MQTT_SERVICE_FOUND "Üzerinden MQTT serivisi tespit edildi"
#define D_FOUND_AT "(bulundu)"
#define D_SYSLOG_HOST_NOT_FOUND "Syslog hostu bulunamadı"

// settings.ino
#define D_SAVED_TO_FLASH_AT "Bellekten (Flash) kaydedidi:"
#define D_LOADED_FROM_FLASH_AT "Bellekten (Flash) okundu"
#define D_USE_DEFAULTS "Varsayılanları Kullan"
#define D_ERASED_SECTOR "Sektör temizlendi"

// xdrv_02_webserver.ino
#define D_NOSCRIPT "To use Tasmota, please enable JavaScript"
#define D_MINIMAL_FIRMWARE_PLEASE_UPGRADE "Donanım yazılımı çok düşük<br/>lütfen yükseltin"
#define D_WEBSERVER_ACTIVE_ON "Web sunucusu aktif"
#define D_WITH_IP_ADDRESS "IP adres ile"
#define D_WEBSERVER_STOPPED "Web sunucusu durdu"
#define D_FILE_NOT_FOUND "Dosya Bulunamadı"
#define D_REDIRECTED "Portala yönlendirildiniz"
#define D_WIFIMANAGER_SET_ACCESSPOINT_AND_STATION "Wifi yöneticisi AP olarak ayarlandı ve istasyon olarak bekletiliyor"
#define D_WIFIMANAGER_SET_ACCESSPOINT "Wifi yöneticisi AP olarak ayarlandı"
#define D_TRYING_TO_CONNECT "Cihaz ağa bağlanmaya çalışıyor"

#define D_RESTART_IN "Burada tekrar başlat"
#define D_SECONDS "saniye"
#define D_DEVICE_WILL_RESTART "Birkaç saniye içerisinde cihaz tekrar başlatılacak"
#define D_BUTTON_TOGGLE "Toggle"
#define D_CONFIGURATION "Konfigürasyon"
#define D_INFORMATION "Bilgi"
#define D_FIRMWARE_UPGRADE "Cihaz yazılımını Güncelle"
#define D_CONSOLE "Konsol"
#define D_CONFIRM_RESTART "Yeniden Başlatmayı Onayla"

#define D_CONFIGURE_MODULE "Modül Ayarlarını Değiştir"
#define D_CONFIGURE_WIFI "WiFi'i Ayarlarını Değiştir"
#define D_CONFIGURE_MQTT "MQTT'yi Ayarlarını Değiştir"
#define D_CONFIGURE_DOMOTICZ "Domoticz Ayarlarını Değiştir"
#define D_CONFIGURE_LOGGING "Loglama Ayarlarını Değiştir"
#define D_CONFIGURE_OTHER "Diğer Ayarları Değiştir"
#define D_CONFIRM_RESET_CONFIGURATION "Resetleme Ayarlarını Onayla"
#define D_RESET_CONFIGURATION "Tüm Ayarları Resetle"
#define D_BACKUP_CONFIGURATION "Ayarları Yedekle"
#define D_RESTORE_CONFIGURATION "Ayarları Geri Yükle"
#define D_MAIN_MENU "Ana Menü"

#define D_MODULE_PARAMETERS "Modül parametreleri"
#define D_MODULE_TYPE "Modul türü"
#define D_PULLUP_ENABLE "No Button/Switch pull-up"
#define D_GPIO "GPIO"
#define D_SERIAL_IN "Serial In"
#define D_SERIAL_OUT "Serial Out"

#define D_WIFI_PARAMETERS "Wifi parametreleri"
#define D_SCAN_FOR_WIFI_NETWORKS "Çevredeki Wifi Networklerini Tara"
#define D_SCAN_DONE "Tarama tamamladı"
#define D_NO_NETWORKS_FOUND "Herhangi wifi ağı bulunamadı"
#define D_REFRESH_TO_SCAN_AGAIN "Tekrar tarama - yenile"
#define D_DUPLICATE_ACCESSPOINT "AP'yi çoğalt"
#define D_SKIPPING_LOW_QUALITY "Düşük kalitedekileri dikkate alma"
#define D_RSSI "RSSI"
#define D_WEP "WEP"
#define D_WPA_PSK "WPA PSK"
#define D_WPA2_PSK "WPA2 PSK"
#define D_AP1_SSID "AP1 Adı (SSId)"
#define D_AP1_PASSWORD "AP1 Parolası"
#define D_AP2_SSID "AP2 SSId"
#define D_AP2_PASSWORD "AP2 Parolası"

#define D_MQTT_PARAMETERS "MQTT parametreleri"
#define D_CLIENT "İstemci"
#define D_FULL_TOPIC "Tüm Başlık"

#define D_LOGGING_PARAMETERS "Loglama parametreleri"
#define D_SERIAL_LOG_LEVEL "Serial log seviyesi"
#define D_WEB_LOG_LEVEL "Web log seviyesi"
#define D_SYS_LOG_LEVEL "Syslog seviyesi"
#define D_MORE_DEBUG "Hata ayıklama devamı"
#define D_SYSLOG_HOST "Syslog host"
#define D_SYSLOG_PORT "Syslog port"
#define D_TELEMETRY_PERIOD "Telemetri peryodu"

#define D_OTHER_PARAMETERS "Diğer parametreler"
#define D_TEMPLATE "Template"
#define D_ACTIVATE "Activate"
#define D_WEB_ADMIN_PASSWORD "Web Yönetici Şifresi"
#define D_MQTT_ENABLE "MQTT aktif"
#define D_FRIENDLY_NAME "Kullanıcı Dostu İsim"
#define D_BELKIN_WEMO "Belkin WeMo"
#define D_HUE_BRIDGE "Hue Bridge"
#define D_SINGLE_DEVICE "tekli cihaz"
#define D_MULTI_DEVICE "çoklu cihaz"

#define D_CONFIGURE_TEMPLATE "Configure Template"
#define D_TEMPLATE_PARAMETERS "Template parameters"
#define D_TEMPLATE_NAME "Name"
#define D_BASE_TYPE "Based on"
#define D_TEMPLATE_FLAGS "Options"
#define D_ALLOW_ADC0 "ADC0 input"
#define D_ALLOW_PULLUP "User pull-up selection"

#define D_SAVE_CONFIGURATION "Ayarları Kaydet"
#define D_CONFIGURATION_SAVED "Ayarlar kaydedildi"
#define D_CONFIGURATION_RESET "Ayarlar resetlendi"

#define D_PROGRAM_VERSION "Yazılım versiyonu"
#define D_BUILD_DATE_AND_TIME "Derleme Tarihi ve Saati"
#define D_CORE_AND_SDK_VERSION "Core/SDK Versiyonu"
#define D_FLASH_WRITE_COUNT "Belleğe (flash) Yazma Sayısı"
#define D_MAC_ADDRESS "MAC Adresi"
#define D_MQTT_HOST "MQTT Host"
#define D_MQTT_PORT "MQTT Port"
#define D_MQTT_CLIENT "MQTT İstemcisi"
#define D_MQTT_USER "MQTT Kullanıcısı"
#define D_MQTT_TOPIC "MQTT Topiği"
#define D_MQTT_GROUP_TOPIC "MQTT Grup Topiği"
#define D_MQTT_FULL_TOPIC "MQTT Full Topik"
#define D_MDNS_DISCOVERY "mDNS Keşfi"
#define D_MDNS_ADVERTISE "mDNS Yayını"
#define D_ESP_CHIP_ID "ESP Chip Id"
#define D_FLASH_CHIP_ID "Flash Chip Id"
#define D_FLASH_CHIP_SIZE "Flash Boyutu"
#define D_FREE_PROGRAM_SPACE "Boşta Yazılım Alanı Boyutu"

#define D_UPGRADE_BY_WEBSERVER "Web server kullanarak yükselt"
#define D_OTA_URL "OTA Url"
#define D_START_UPGRADE "Yükseltmeyi başlat"
#define D_UPGRADE_BY_FILE_UPLOAD "Dosya kullanrak yükset"
#define D_UPLOAD_STARTED "Yükleme başlatıldı"
#define D_UPGRADE_STARTED "YÜkestlme başlatıldı"
#define D_UPLOAD_DONE "Yükleme Tamamlandı"
#define D_UPLOAD_ERR_1 "Dosya seçilmedi"
#define D_UPLOAD_ERR_2 "Boş yer yok"
#define D_UPLOAD_ERR_3 "Magic byte is not 0xE9"
#define D_UPLOAD_ERR_4 "Yazılımın boyutu gerçek boyutundan fazla"
#define D_UPLOAD_ERR_5 "Upload buffer miscompare"
#define D_UPLOAD_ERR_6 "Yükleme başarısız. Enable logging 3"
#define D_UPLOAD_ERR_7 "Yükleme iptal edildi"
#define D_UPLOAD_ERR_8 "Geçersiz dosya"
#define D_UPLOAD_ERR_9 "File too large"
#define D_UPLOAD_ERR_10 "Failed to init RF chip"
#define D_UPLOAD_ERR_11 "Failed to erase RF chip"
#define D_UPLOAD_ERR_12 "Failed to write to RF chip"
#define D_UPLOAD_ERR_13 "Failed to decode RF firmware"
#define D_UPLOAD_ERROR_CODE "Upload error code"

#define D_ENTER_COMMAND "Komut girişi"
#define D_ENABLE_WEBLOG_FOR_RESPONSE "Enable weblog 2 if response expected"
#define D_NEED_USER_AND_PASSWORD "Need user=<username>&password=<password>"

// xdrv_01_mqtt.ino
#define D_FINGERPRINT "Verify TLS fingerprint..."
#define D_TLS_CONNECT_FAILED_TO "TLS Connect failed to"
#define D_RETRY_IN "Retry in"
#define D_VERIFIED "Verified using Fingerprint"
#define D_INSECURE "Insecure connection due to invalid Fingerprint"
#define D_CONNECT_FAILED_TO "Connect failed to"

// xplg_wemohue.ino
#define D_MULTICAST_DISABLED "Multicast kapalı"
#define D_MULTICAST_REJOINED "Multicast (tekrar)bağlanıldı"
#define D_MULTICAST_JOIN_FAILED "Multicast bağlantısı başarısız"
#define D_FAILED_TO_SEND_RESPONSE "Failed to send response"

#define D_WEMO "WeMo"
#define D_WEMO_BASIC_EVENT "WeMo basic event"
#define D_WEMO_EVENT_SERVICE "WeMo event service"
#define D_WEMO_META_SERVICE "WeMo meta service"
#define D_WEMO_SETUP "WeMo setup"
#define D_RESPONSE_SENT "Response sent"

#define D_HUE "Hue"
#define D_HUE_BRIDGE_SETUP "Hue kurulumu"
#define D_HUE_API_NOT_IMPLEMENTED "Hue API'si entegre edilmedi"
#define D_HUE_API "Hue API"
#define D_HUE_POST_ARGS "Hue POST argümanları"
#define D_3_RESPONSE_PACKETS_SENT "3 response packets sent"

// xdrv_07_domoticz.ino
#define D_DOMOTICZ_PARAMETERS "Domoticz parameters"
#define D_DOMOTICZ_IDX "Idx"
#define D_DOMOTICZ_KEY_IDX "Key idx"
#define D_DOMOTICZ_SWITCH_IDX "Switch idx"
#define D_DOMOTICZ_SENSOR_IDX "Sensor idx"
  #define D_DOMOTICZ_TEMP "Temp"
  #define D_DOMOTICZ_TEMP_HUM "Temp,Hum"
  #define D_DOMOTICZ_TEMP_HUM_BARO "Temp,Hum,Baro"
  #define D_DOMOTICZ_POWER_ENERGY "Power,Energy"
  #define D_DOMOTICZ_ILLUMINANCE "Illuminance"
  #define D_DOMOTICZ_COUNT "Count/PM1"
  #define D_DOMOTICZ_VOLTAGE "Voltage/PM2.5"
  #define D_DOMOTICZ_CURRENT "Current/PM10"
  #define D_DOMOTICZ_AIRQUALITY "AirQuality"
#define D_DOMOTICZ_UPDATE_TIMER "Zamanlayıcıyı güncelle"

// xdrv_09_timers.ino
#define D_CONFIGURE_TIMER "Zamanlayıcı Ayarları"
#define D_TIMER_PARAMETERS "Zamanlayıcı Parametreleri"
#define D_TIMER_ENABLE "Zamanlayıcı Aktif"
#define D_TIMER_ARM "Arm"
#define D_TIMER_TIME "Zaman"
#define D_TIMER_DAYS "Gün"
#define D_TIMER_REPEAT "Tekrar"
#define D_TIMER_OUTPUT "Output"
#define D_TIMER_ACTION "Action"

// xdrv_10_knx.ino
#define D_CONFIGURE_KNX "KNX Ayarları"
#define D_KNX_PARAMETERS "KNX Parametreleri"
#define D_KNX_GENERAL_CONFIG "Genel Ayarlar"
#define D_KNX_PHYSICAL_ADDRESS "Fiziksel Addres"
#define D_KNX_PHYSICAL_ADDRESS_NOTE "( Must be unique on the KNX network )"
#define D_KNX_ENABLE "KNX Aktif"
#define D_KNX_GROUP_ADDRESS_TO_WRITE "Data to Send to Group Addresses"
#define D_ADD "Ekle"
#define D_DELETE "Sil"
#define D_REPLY "Cevapla"
#define D_KNX_GROUP_ADDRESS_TO_READ "Group Addresses to Receive Data from"
#define D_LOG_KNX "KNX: "
#define D_RECEIVED_FROM "Received from"
#define D_KNX_COMMAND_WRITE "Yaz"
#define D_KNX_COMMAND_READ "Oku"
#define D_KNX_COMMAND_OTHER "Diğer"
#define D_SENT_TO "gönder"
#define D_KNX_WARNING "The group address ( 0 / 0 / 0 ) is reserved and can not be used."
#define D_KNX_ENHANCEMENT "Communication Enhancement"
#define D_KNX_TX_SLOT "KNX TX"
#define D_KNX_RX_SLOT "KNX RX"

// xdrv_03_energy.ino
#define D_ENERGY_TODAY "Energy Today"
#define D_ENERGY_YESTERDAY "Energy Yesterday"
#define D_ENERGY_TOTAL "Energy Total"

// xsns_05_ds18b20.ino
#define D_SENSOR_BUSY "Sensör başgül"
#define D_SENSOR_CRC_ERROR "Sensor CRC hatası"
#define D_SENSORS_FOUND "Sensörler bulundu"

// xsns_06_dht.ino
#define D_TIMEOUT_WAITING_FOR "Timeout waiting for"
#define D_START_SIGNAL_LOW "düşük sinyal başlat"
#define D_START_SIGNAL_HIGH "yüksek sinyal başlat"
#define D_PULSE "pulse"
#define D_CHECKSUM_FAILURE "Checksum failure"

// xsns_07_sht1x.ino
#define D_SENSOR_DID_NOT_ACK_COMMAND "Sensor did not ACK command"
#define D_SHT1X_FOUND "SHT1X found"

// xsns_18_pms5003.ino
#define D_STANDARD_CONCENTRATION "CF-1 PM"     // Standard Particle CF-1 Particle Matter
#define D_ENVIRONMENTAL_CONCENTRATION "PM"     // Environmetal Particle Matter
#define D_PARTICALS_BEYOND "Particals"

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

// sonoff_template.h - keep them as short as possible to be able to fit them in GUI drop down box
#define D_SENSOR_NONE          "None"
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
#define D_SENSOR_SWITCH        "Switch"     // Suffix "1"
#define D_SENSOR_BUTTON        "Button"     // Suffix "1"
#define D_SENSOR_RELAY         "Relay"      // Suffix "1i"
#define D_SENSOR_LED           "Led"        // Suffix "1i"
#define D_SENSOR_PWM           "PWM"        // Suffix "1"
#define D_SENSOR_COUNTER       "Counter"    // Suffix "1"
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
#define D_SENSOR_RF_SENSOR     "RF Sensor"
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
#define D_UNIT_HOUR "Hr"
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
#define D_UNIT_SECTORS "sectors"
#define D_UNIT_VA "VA"
#define D_UNIT_VAR "VAr"
#define D_UNIT_VOLT "V"
#define D_UNIT_WATT "W"
#define D_UNIT_WATTHOUR "Wh"
#define D_UNIT_WATT_METER_QUADRAT "W/m²"
#define D_UNIT_HERTZ "Hz"

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

#endif  // _LANGUAGE_TR_TR_H_
