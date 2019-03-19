/*
  ru-RU.h - localization for Russian - Rissia for Sonoff-Tasmota

  Copyright (C) 2019  Theo Arends / roman-vn

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

#ifndef _LANGUAGE_RU_RU_H_
#define _LANGUAGE_RU_RU_H_

/*************************** ATTENTION *******************************\
 *
 * Due to memory constraints only UTF-8 is supported.
 * To save code space keep text as short as possible.
 * Time and Date provided by SDK can not be localized (yet).
 * Use online command StateText to translate ON, OFF, HOLD and TOGGLE.
 * Use online command Prefix to translate cmnd, stat and tele.
 *
 * Updated until v5.12.0b
\*********************************************************************/

//#define LANGUAGE_MODULE_NAME         // Enable to display "Module Generic" (ie Spanish), Disable to display "Generic Module" (ie English)

#define LANGUAGE_LCID 1049
// HTML (ISO 639-1) Language Code
#define D_HTML_LANGUAGE "ru"

// "2017-03-07T11:08:02" - ISO8601:2004
#define D_YEAR_MONTH_SEPARATOR "-"
#define D_MONTH_DAY_SEPARATOR "-"
#define D_DATE_TIME_SEPARATOR "T"
#define D_HOUR_MINUTE_SEPARATOR ":"
#define D_MINUTE_SECOND_SEPARATOR ":"

#define D_DAY3LIST "ВскПонВтрСрдЧетПятСуб"
#define D_MONTH3LIST "ЯнвФевМарАпрМайИюнИюлАвгСенОктНояДек"

// Non JSON decimal separator
#define D_DECIMAL_SEPARATOR ","

// Common
#define D_ADMIN "Admin"
#define D_AIR_QUALITY "Качество воздуха"
#define D_AP "AP"                    // Access Point
#define D_AS "как"
#define D_AUTO "АВТО"
#define D_BLINK "Мигать"
#define D_BLINKOFF "Не Мигать"
#define D_BOOT_COUNT "Количество загрузок"
#define D_BRIGHTLIGHT "Яркость"
#define D_BSSID "BSSId"
#define D_BUTTON "Кнопка"
#define D_BY "by"                    // Written by me
#define D_BYTES "Байт"
#define D_CELSIUS "Цельсия"
#define D_CHANNEL "Channel"
#define D_CO2 "Углекислый газ"
#define D_CODE "код"                // Button code
#define D_COLDLIGHT "Холодный"
#define D_COMMAND "Команда"
#define D_CONNECTED "Соединен"
#define D_COUNT "Подсчет"
#define D_COUNTER "Счетчик"
#define D_CURRENT "Ток"          // As in Voltage and Current
#define D_DATA "Данные"
#define D_DARKLIGHT "Темный"
#define D_DEBUG "Отладка"
#define D_DISABLED "Блокирован"
#define D_DISTANCE "Distance"
#define D_DNS_SERVER "DNS Сервер"
#define D_DONE "Выполнено"
#define D_DST_TIME "DST"
#define D_ECO2 "eCO2"
#define D_EMULATION "Эмуляция"
#define D_ENABLED "Активно"
#define D_ERASE "Стирать"
#define D_ERROR "Ошибка"
#define D_FAHRENHEIT "Фаренгейт"
#define D_FAILED "Неудачно"
#define D_FALLBACK "Обратная связь"
#define D_FALLBACK_TOPIC "Топик обратной связи"
#define D_FALSE "Ложно"
#define D_FILE "Файл"
#define D_FREE_MEMORY "Свободная память"
#define D_FREQUENCY "Frequency"
#define D_GAS "Газ"
#define D_GATEWAY "Шлюз"
#define D_GROUP "Группа"
#define D_HOST "Хост"
#define D_HOSTNAME "Имя Хоста"
#define D_HUMIDITY "Влажность"
#define D_ILLUMINANCE "Освещенность"
#define D_IMMEDIATE "немедленно"      // Button immediate
#define D_INDEX "Индекс"
#define D_INFO "Инфо"
#define D_INFRARED "Infrared"
#define D_INITIALIZED "Инициализировано"
#define D_IP_ADDRESS "IP Адрес"
#define D_LIGHT "Свет"
#define D_LWT "LWT"
#define D_MODULE "Модуль"
#define D_MQTT "MQTT"
#define D_MULTI_PRESS "многократное нажатие"
#define D_NOISE "Шум"
#define D_NONE "Нет"
#define D_OFF "Выкл"
#define D_OFFLINE "Офф-лайн"
#define D_OK "Ок"
#define D_ON "Вкл"
#define D_ONLINE "Он-лайн"
#define D_PASSWORD "Пароль"
#define D_PORT "Порт"
#define D_POWER_FACTOR "Коэффициент Мощности"
#define D_POWERUSAGE "Мощность"
#define D_POWERUSAGE_ACTIVE "Active Power"
#define D_POWERUSAGE_APPARENT "Apparent Power"
#define D_POWERUSAGE_REACTIVE "Reactive Power"
#define D_PRESSURE "Давление"
#define D_PRESSUREATSEALEVEL "Давление на уровне моря"
#define D_PROGRAM_FLASH_SIZE "Размер Flash для программ"
#define D_PROGRAM_SIZE "Размер программы "
#define D_PROJECT "Проект"
#define D_RAIN "Rain"
#define D_RECEIVED "Получено"
#define D_RESTART "Перезапуск"
#define D_RESTARTING "Перезапуск"
#define D_RESTART_REASON "Причина перезагрузки"
#define D_RESTORE "восстановление"
#define D_RETAINED "нераспред."
#define D_RULE "Rule"
#define D_SAVE "Сохранить"
#define D_SENSOR "Датчик"
#define D_SSID "SSId"
#define D_START "Старт"
#define D_STD_TIME "STD"
#define D_STOP "Стоп"
#define D_SUBNET_MASK "Маска Подсети"
#define D_SUBSCRIBE_TO "Подписаться на"
#define D_UNSUBSCRIBE_FROM "Unsubscribe from"
#define D_SUCCESSFUL "Успешно"
#define D_SUNRISE "Sunrise"
#define D_SUNSET "Sunset"
#define D_TEMPERATURE "Температура"
#define D_TO "до"
#define D_TOGGLE "Переключить"
#define D_TOPIC "Топик"
#define D_TRANSMIT "Передать"
#define D_TRUE "Истина"
#define D_TVOC "TVOC"
#define D_UPGRADE "обновление"
#define D_UPLOAD "Загрузить"
#define D_UPTIME "Uptime"
#define D_USER "Пользователь"
#define D_UTC_TIME "UTC"
#define D_UV_INDEX "UV Index"
#define D_UV_INDEX_1 "Low"
#define D_UV_INDEX_2 "Mid"
#define D_UV_INDEX_3 "High"
#define D_UV_INDEX_4 "Danger"
#define D_UV_INDEX_5 "BurnL1/2"
#define D_UV_INDEX_6 "BurnL3"
#define D_UV_INDEX_7 "OoR"
#define D_UV_LEVEL "УФ уровень"
#define D_UV_POWER "UV Power"
#define D_VERSION "Версия"
#define D_VOLTAGE "Напряжение"
#define D_WEIGHT "Weight"
#define D_WARMLIGHT "Тепло"
#define D_WEB_SERVER "Web сервер"

// sonoff.ino
#define D_WARNING_MINIMAL_VERSION "ПРЕДУПРЕЖДЕНИЕ Эта версия не поддерживает персистентные настройки"
#define D_LEVEL_10 "уровень 1-0"
#define D_LEVEL_01 "уровень 0-1"
#define D_SERIAL_LOGGING_DISABLED "Serial logging отключен"
#define D_SYSLOG_LOGGING_REENABLED "Syslog logging включен"

#define D_SET_BAUDRATE_TO "Установить скорость передачи (Baudrate)"
#define D_RECEIVED_TOPIC "Полученный Топик"
#define D_DATA_SIZE "Размер данных"
#define D_ANALOG_INPUT "Аналоговый вход"

// support.ino
#define D_OSWATCH "osWatch"
#define D_BLOCKED_LOOP "Блокирующий цикл"
#define D_WPS_FAILED_WITH_STATUS "WPS конфигурация НЕУДАЧНА со статусом"
#define D_ACTIVE_FOR_3_MINUTES "активен в течение 3 минут"
#define D_FAILED_TO_START "не удалось запустить"
#define D_PATCH_ISSUE_2186 "Проблема с исправлением 2186"
#define D_CONNECTING_TO_AP "Подключение к AP"
#define D_IN_MODE "в режиме"
#define D_CONNECT_FAILED_NO_IP_ADDRESS "Ошибка подключения, IP-адрес не получен"
#define D_CONNECT_FAILED_AP_NOT_REACHED "Ошибка соединения, AP не может быть достигнута"
#define D_CONNECT_FAILED_WRONG_PASSWORD "Ошибка соединения, неверный пароль к AP"
#define D_CONNECT_FAILED_AP_TIMEOUT "Ошибка соединения с AP по тайм-ауту"
#define D_ATTEMPTING_CONNECTION "Попытка подключения..."
#define D_CHECKING_CONNECTION "Проверка соединения..."
#define D_QUERY_DONE "Запрос выполнен. Обнаружены службы MQTT"
#define D_MQTT_SERVICE_FOUND "MQTT сервис service найдено"
#define D_FOUND_AT "найдено в"
#define D_SYSLOG_HOST_NOT_FOUND "System лог хост не найден"

// settings.ino
#define D_SAVED_TO_FLASH_AT "Сохранено во флэш-память"
#define D_LOADED_FROM_FLASH_AT "Загружено из флэш-памяти"
#define D_USE_DEFAULTS "Использовать значение по умолчанию"
#define D_ERASED_SECTOR "Стереть сектор"

// xdrv_02_webserver.ino
#define D_NOSCRIPT "To use Tasmota, please enable JavaScript"
#define D_MINIMAL_FIRMWARE_PLEASE_UPGRADE "Прошивка MINIMAL<br/>пожалуйста обновите"
#define D_WEBSERVER_ACTIVE_ON "Веб-сервер активен"
#define D_WITH_IP_ADDRESS "с IP-адресом"
#define D_WEBSERVER_STOPPED "Веб-сервер остановлен"
#define D_FILE_NOT_FOUND "Файл не найден"
#define D_REDIRECTED "Перенаправлено на адаптивный портал"
#define D_WIFIMANAGER_SET_ACCESSPOINT_AND_STATION "Wifi менеджер устанавливает Точку Доступа и сохраняет станцию"
#define D_WIFIMANAGER_SET_ACCESSPOINT "Wifi менеджер установил Точку Доступа"
#define D_TRYING_TO_CONNECT "Попытка подключения устройства к сети"

#define D_RESTART_IN "Перезапуск"
#define D_SECONDS "секунд"
#define D_DEVICE_WILL_RESTART "Устройство будет перезапущено через несколько секунд"
#define D_BUTTON_TOGGLE "Переключить"
#define D_CONFIGURATION "Конфигурация"
#define D_INFORMATION "Информация"
#define D_FIRMWARE_UPGRADE "Обновление прошивки"
#define D_CONSOLE "Консоль"
#define D_CONFIRM_RESTART "Подтвердить перезагрузку"

#define D_CONFIGURE_MODULE "Конфигурация Модуля"
#define D_CONFIGURE_WIFI "Конфигурация WiFi"
#define D_CONFIGURE_MQTT "Конфигурация MQTT"
#define D_CONFIGURE_DOMOTICZ "Конфигурация Domoticz"
#define D_CONFIGURE_LOGGING "Конфигурация Logging"
#define D_CONFIGURE_OTHER "Конфигурация прочее"
#define D_CONFIRM_RESET_CONFIGURATION "Подтвердить Сброс Конфигурации"
#define D_RESET_CONFIGURATION "Сброс Конфигурации"
#define D_BACKUP_CONFIGURATION "Резервное копирование Конфигурации"
#define D_RESTORE_CONFIGURATION "Восстановление Конфигурации"
#define D_MAIN_MENU "Главное Меню"

#define D_MODULE_PARAMETERS "Параметры модуля"
#define D_MODULE_TYPE "Тип модуля"
#define D_PULLUP_ENABLE "No Button/Switch pull-up"
#define D_GPIO "GPIO"
#define D_SERIAL_IN "Serial вход"
#define D_SERIAL_OUT "Serial выход"

#define D_WIFI_PARAMETERS "Параметры Wifi"
#define D_SCAN_FOR_WIFI_NETWORKS "Сканирование беспроводных сетей Wifi"
#define D_SCAN_DONE "Сканирование завершено"
#define D_NO_NETWORKS_FOUND "Не найдено сетей"
#define D_REFRESH_TO_SCAN_AGAIN "Обновить для повторного сканирования"
#define D_DUPLICATE_ACCESSPOINT "Дублировать Точку Доступа (AP)"
#define D_SKIPPING_LOW_QUALITY "Пропущено из-за низкого качества"
#define D_RSSI "RSSI"
#define D_WEP "WEP"
#define D_WPA_PSK "WPA PSK"
#define D_WPA2_PSK "WPA2 PSK"
#define D_AP1_SSID "AP1 SSId"
#define D_AP1_PASSWORD "AP1 Пароль"
#define D_AP2_SSID "AP2 SSId"
#define D_AP2_PASSWORD "AP2 Пароль"

#define D_MQTT_PARAMETERS "Параметры MQTT"
#define D_CLIENT "Клиент"
#define D_FULL_TOPIC "Полный Топик"

#define D_LOGGING_PARAMETERS "Параметры Logging"
#define D_SERIAL_LOG_LEVEL "Serial лог уровень"
#define D_WEB_LOG_LEVEL "Web лог уровень"
#define D_SYS_LOG_LEVEL "System лог уровень"
#define D_MORE_DEBUG "Дополнительная информация для отладки"
#define D_SYSLOG_HOST "System лог хост"
#define D_SYSLOG_PORT "System лог порт"
#define D_TELEMETRY_PERIOD "Период телеметрии"

#define D_OTHER_PARAMETERS "Параметры Прочие"
#define D_TEMPLATE "Template"
#define D_ACTIVATE "Activate"
#define D_WEB_ADMIN_PASSWORD "Пароль Web администратора"
#define D_MQTT_ENABLE "MQTT активен"
#define D_FRIENDLY_NAME "Дружественное Имя"
#define D_BELKIN_WEMO "Belkin WeMo"
#define D_HUE_BRIDGE "Hue Bridge"
#define D_SINGLE_DEVICE "одиночное"
#define D_MULTI_DEVICE "мульти"

#define D_CONFIGURE_TEMPLATE "Configure Template"
#define D_TEMPLATE_PARAMETERS "Template parameters"
#define D_TEMPLATE_NAME "Name"
#define D_BASE_TYPE "Based on"
#define D_TEMPLATE_FLAGS "Options"
#define D_ALLOW_ADC0 "ADC0 input"
#define D_ALLOW_PULLUP "User pull-up selection"

#define D_SAVE_CONFIGURATION "Сохранить конфигурацию"
#define D_CONFIGURATION_SAVED "Конфигурация сохранена "
#define D_CONFIGURATION_RESET "Конфигурация сброшена"

#define D_PROGRAM_VERSION "Версия программы"
#define D_BUILD_DATE_AND_TIME "Дата & Время сборки"
#define D_CORE_AND_SDK_VERSION "Версия Core/SDK"
#define D_FLASH_WRITE_COUNT "Кол-во записей Flash"
#define D_MAC_ADDRESS "MAC Адрес"
#define D_MQTT_HOST "MQTT Хост"
#define D_MQTT_PORT "MQTT Порт"
#define D_MQTT_CLIENT "MQTT Клиент ID"
#define D_MQTT_USER "MQTT Пользователь"
#define D_MQTT_TOPIC "MQTT Топик"
#define D_MQTT_GROUP_TOPIC "MQTT Топик группы"
#define D_MQTT_FULL_TOPIC "MQTT Топик полный"
#define D_MDNS_DISCOVERY "mDNS Обнаружение"
#define D_MDNS_ADVERTISE "mDNS Транcляция"
#define D_ESP_CHIP_ID "ID чипа ESP"
#define D_FLASH_CHIP_ID "ID чипа Flash памяти"
#define D_FLASH_CHIP_SIZE "Размер Flash памяти"
#define D_FREE_PROGRAM_SPACE "Свободное пространство программ"

#define D_UPGRADE_BY_WEBSERVER "Обновление через Веб-сервер"
#define D_OTA_URL "OTA Url"
#define D_START_UPGRADE "Начать обновление"
#define D_UPGRADE_BY_FILE_UPLOAD "Обновление путем загрузки файлов"
#define D_UPLOAD_STARTED "Загрузка началась"
#define D_UPGRADE_STARTED "Обновление началось"
#define D_UPLOAD_DONE "Загрузка завершена"
#define D_UPLOAD_ERR_1 "Файл не выбран"
#define D_UPLOAD_ERR_2 "Недостаточно места"
#define D_UPLOAD_ERR_3 "Magic байт не 0xE9"
#define D_UPLOAD_ERR_4 "Размер прошивки больше, чем реальный размер флеш памяти"
#define D_UPLOAD_ERR_5 "Ошибка загрузки буфера"
#define D_UPLOAD_ERR_6 "Ошибка загрузки. Включено лог уровня 3"
#define D_UPLOAD_ERR_7 "Загрузка прервана"
#define D_UPLOAD_ERR_8 "Файл неверный"
#define D_UPLOAD_ERR_9 "Слишком большой файл"
#define D_UPLOAD_ERR_10 "Failed to init RF chip"
#define D_UPLOAD_ERR_11 "Failed to erase RF chip"
#define D_UPLOAD_ERR_12 "Failed to write to RF chip"
#define D_UPLOAD_ERR_13 "Failed to decode RF firmware"
#define D_UPLOAD_ERROR_CODE "Код ошибки загрузки"

#define D_ENTER_COMMAND "Введите команду"
#define D_ENABLE_WEBLOG_FOR_RESPONSE "Включить Веб лог уровня 2 если ожидается ответ"
#define D_NEED_USER_AND_PASSWORD "Ожидается user=<username>&password=<password>"

// xdrv_01_mqtt.ino
#define D_FINGERPRINT "Проверка TLS Fingerprint..."
#define D_TLS_CONNECT_FAILED_TO "Сбой подключения TLS к"
#define D_RETRY_IN "Повторить"
#define D_VERIFIED "Проверено Fingerprint"
#define D_INSECURE "Небезопасное соединение, недействительный Fingerprint"
#define D_CONNECT_FAILED_TO "Ошибка подключения к"

// xplg_wemohue.ino
#define D_MULTICAST_DISABLED "Multicast отключен"
#define D_MULTICAST_REJOINED "Multicast (ре)соединился"
#define D_MULTICAST_JOIN_FAILED "Multicast ошибка соединения"
#define D_FAILED_TO_SEND_RESPONSE "Не удалось отправить ответ"

#define D_WEMO "WeMo"
#define D_WEMO_BASIC_EVENT "WeMo основное событие"
#define D_WEMO_EVENT_SERVICE "WeMo служба событий"
#define D_WEMO_META_SERVICE "WeMo мета-сервис"
#define D_WEMO_SETUP "WeMo настройка"
#define D_RESPONSE_SENT "Ответ отправлен"

#define D_HUE "Hue"
#define D_HUE_BRIDGE_SETUP "Hue настройка"
#define D_HUE_API_NOT_IMPLEMENTED "Hue API не реализовано"
#define D_HUE_API "Hue API"
#define D_HUE_POST_ARGS "Hue POST args"
#define D_3_RESPONSE_PACKETS_SENT "3 ответных пакета получено"

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
  #define D_DOMOTICZ_VOLTAGE "Voltage/PM2,5"
  #define D_DOMOTICZ_CURRENT "Current/PM10"
  #define D_DOMOTICZ_AIRQUALITY "AirQuality"
#define D_DOMOTICZ_UPDATE_TIMER "Update timer"

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
#define D_ENERGY_TODAY "Энергия Сегодня"
#define D_ENERGY_YESTERDAY "Энергия Вчера"
#define D_ENERGY_TOTAL "Энергия Всего"

// xsns_05_ds18b20.ino
#define D_SENSOR_BUSY "Датчик DS18x20 занят"
#define D_SENSOR_CRC_ERROR "Датчик DS18x20 - ошибка CRC"
#define D_SENSORS_FOUND "Датчик DS18x20 найден"

// xsns_06_dht.ino
#define D_TIMEOUT_WAITING_FOR "Тайм-аут, в ожидании"
#define D_START_SIGNAL_LOW "сигнал старт низкий"
#define D_START_SIGNAL_HIGH "сигнал старт высокий"
#define D_PULSE "пульс"
#define D_CHECKSUM_FAILURE "Ошибка контрольной суммы"

// xsns_07_sht1x.ino
#define D_SENSOR_DID_NOT_ACK_COMMAND "Датчик не получил команду ACK"
#define D_SHT1X_FOUND "SHT1X найден"

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
#define D_SENSOR_NONE          "-нет-"
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
#define D_SENSOR_SWITCH        "Свич"       // Suffix "1"
#define D_SENSOR_BUTTON        "Кнопка"     // Suffix "1"
#define D_SENSOR_RELAY         "Реле"       // Suffix "1i"
#define D_SENSOR_LED           "Led"        // Suffix "1i"
#define D_SENSOR_PWM           "PWM"        // Suffix "1"
#define D_SENSOR_COUNTER       "Счетчик"    // Suffix "1"
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
#define D_UNIT_AMPERE "А"
#define D_UNIT_CENTIMETER "cm"
#define D_UNIT_HERTZ "Hz"
#define D_UNIT_HOUR "Ч"
#define D_UNIT_INCREMENTS "inc"
#define D_UNIT_KILOGRAM "kg"
#define D_UNIT_KILOMETER_PER_HOUR "km/h"  // or "km/h"
#define D_UNIT_KILOOHM "кОм"
#define D_UNIT_KILOWATTHOUR "кВт"
#define D_UNIT_LUX "лк"
#define D_UNIT_MICROGRAM_PER_CUBIC_METER "мкг/м3"
#define D_UNIT_MICROMETER "мкм"
#define D_UNIT_MICROSECOND "мкс"
#define D_UNIT_MILLIAMPERE "мА"
#define D_UNIT_MILLIMETER "mm"
#define D_UNIT_MILLIMETER_MERCURY "мм рт.ст."
#define D_UNIT_MILLISECOND "мс"
#define D_UNIT_MINUTE "мин"
#define D_UNIT_PARTS_PER_BILLION "ppb"
#define D_UNIT_PARTS_PER_DECILITER "ppd"
#define D_UNIT_PARTS_PER_MILLION "ppm"
#define D_UNIT_PRESSURE "гПа"
#define D_UNIT_SECOND "сек"
#define D_UNIT_SECTORS "секторов"
#define D_UNIT_VA "VA"
#define D_UNIT_VAR "VAr"
#define D_UNIT_VOLT "В"
#define D_UNIT_WATT "Вт"
#define D_UNIT_WATTHOUR "ВтЧ"
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
#define D_PHASE_ANGLE     "Угол фазы"
#define D_IMPORT_ACTIVE   "Импорт активной мощности"
#define D_EXPORT_ACTIVE   "Экспорт активной мощности"
#define D_IMPORT_REACTIVE "Импорт реактивной мощности"
#define D_EXPORT_REACTIVE "Экспорт реактивной мощности"
#define D_TOTAL_REACTIVE  "Итого реактивная мощность"
#define D_UNIT_KWARH      "kVArh"
#define D_UNIT_ANGLE      "Град"

#endif  // _LANGUAGE_RU_RU_H_
