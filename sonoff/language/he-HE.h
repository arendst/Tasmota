/*
  he-HE.h - localization for Hebrew - Israel for Sonoff-Tasmota

  Copyright (C) 2018  Yuval Mejahez

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

#ifndef _LANGUAGE_HE_HE_H_
#define _LANGUAGE_HE_HE_H_

/*************************** ATTENTION *******************************\
 *
 * Due to memory constraints only UTF-8 is supported.
 * To save code space keep text as short as possible.
 * Time and Date provided by SDK can not be localized (yet).
 * Use online command StateText to translate ON, OFF, HOLD and TOGGLE.
 * Use online command Prefix to translate cmnd, stat and tele.
 *
 * Updated until v5.14.0b
\*********************************************************************/

//#define LANGUAGE_MODULE_NAME         // Enable to display "Module Generic" (ie Spanish), Disable to display "Generic Module" (ie English)

#define LANGUAGE_LCID 1037
// HTML (ISO 639-1) Language Code
#define D_HTML_LANGUAGE "he"

// "2017-03-07T11:08:02" - ISO8601:2004
#define D_YEAR_MONTH_SEPARATOR "-"
#define D_MONTH_DAY_SEPARATOR "-"
#define D_DATE_TIME_SEPARATOR "T"
#define D_HOUR_MINUTE_SEPARATOR ":"
#define D_MINUTE_SECOND_SEPARATOR ":"

#define D_DAY3LIST "SunMonTueWedThuFriSat"
#define D_MONTH3LIST "JanFebMarAprMayJunJulAugSepOctNovDec"

// Non JSON decimal separator
#define D_DECIMAL_SEPARATOR "."

// Common
#define D_ADMIN "מנהל"
#define D_AIR_QUALITY "איכות אוויר"
#define D_AP "AP"                    // Access Point
#define D_AS "as"
#define D_AUTO "AUTO"
#define D_BLINK "מהבהב"
#define D_BLINKOFF "כיבוי היבהוב"
#define D_BOOT_COUNT "מונה הפעלה מחדש"
#define D_BRIGHTLIGHT "בהירות"
#define D_BSSID "BSSId"
#define D_BUTTON "לחצן"
#define D_BY "by"                    // Written by me
#define D_BYTES "בייט"
#define D_CELSIUS "צלזיוס"
#define D_CHANNEL "ערוץ"
#define D_CO2 "Carbon dioxide"
#define D_CODE "קוד"                // Button code
#define D_COLDLIGHT "קור"
#define D_COMMAND "פקודה"
#define D_CONNECTED "מחובר"
#define D_COUNT "סופר"
#define D_COUNTER "מונה"
#define D_CURRENT "נוכחי"          // As in Voltage and Current
#define D_DATA "נתונים"
#define D_DARKLIGHT "חושך"
#define D_DEBUG "באגים"
#define D_DISABLED "מבוטל"
#define D_DISTANCE "מרחק"
#define D_DNS_SERVER "DNS שרת"
#define D_DONE "סיים"
#define D_DST_TIME "DST"
#define D_ECO2 "eCO2"
#define D_EMULATION "הדמיה"
#define D_ENABLED "מאופשר"
#define D_ERASE "מחיקה"
#define D_ERROR "שגיאה"
#define D_FAHRENHEIT "פרנהייט"
#define D_FAILED "נכשל"
#define D_FALLBACK "חזרה"
#define D_FALLBACK_TOPIC "נושא לחזרה"
#define D_FALSE "שגוי"
#define D_FILE "קובץ"
#define D_FREE_MEMORY "זכרון פנוי"
#define D_FREQUENCY "תדר"
#define D_GAS "גז"
#define D_GATEWAY "שער"
#define D_GROUP "קבוצה"
#define D_HOST "מארח"
#define D_HOSTNAME "שם מארח"
#define D_HUMIDITY "לחות"
#define D_ILLUMINANCE "רמת חשיפה"
#define D_IMMEDIATE "מידי"      // Button immediate
#define D_INDEX "אינדקס"
#define D_INFO "מידע"
#define D_INFRARED "אינפרא"
#define D_INITIALIZED "מאותחל"
#define D_IP_ADDRESS "IP כתובת"
#define D_LIGHT "אור"
#define D_LWT "LWT"
#define D_MODULE "מודול"
#define D_MQTT "MQTT"
#define D_MULTI_PRESS "multi-press"
#define D_NOISE "רעש"
#define D_NONE "כלום"
#define D_OFF "כבוי"
#define D_OFFLINE "מנותק"
#define D_OK "אוקיי"
#define D_ON "פועל"
#define D_ONLINE "מחובר"
#define D_PASSWORD "סיסמא"
#define D_PORT "פורט"
#define D_POWER_FACTOR "גורם כוח"
#define D_POWERUSAGE "כוח"
#define D_POWERUSAGE_ACTIVE "כוח פעיל"
#define D_POWERUSAGE_APPARENT "כוח לכאורה"
#define D_POWERUSAGE_REACTIVE "כוח תגובתי"
#define D_PRESSURE "לחץ"
#define D_PRESSUREATSEALEVEL "לחץ ימי"
#define D_PROGRAM_FLASH_SIZE "גודל תוכנית פלאש"
#define D_PROGRAM_SIZE "גודל תוכנית"
#define D_PROJECT "פרויקט"
#define D_RAIN "Rain"
#define D_RECEIVED "התקבל"
#define D_RESTART "איתחול"
#define D_RESTARTING "הפעלה מחדש"
#define D_RESTART_REASON "סיבת הפעלה מחדש"
#define D_RESTORE "שחזור"
#define D_RETAINED "שמור"
#define D_RULE "חוק"
#define D_SAVE "שמירה"
#define D_SENSOR "חיישן"
#define D_SSID "SSId"
#define D_START "התחלה"
#define D_STD_TIME "STD"
#define D_STOP "עצירה"
#define D_SUBNET_MASK "רשת מסכת משנה"
#define D_SUBSCRIBE_TO "הרשם ל"
#define D_SUCCESSFUL "הצליח"
#define D_SUNRISE "זריחה"
#define D_SUNSET "שקיעה"
#define D_TEMPERATURE "טמפרטורה"
#define D_TO "ל"
#define D_TOGGLE "מתג"
#define D_TOPIC "נושא"
#define D_TRANSMIT "עבר"
#define D_TRUE "נכון"
#define D_TVOC "TVOC"
#define D_UPGRADE "שדרוג"
#define D_UPLOAD "העלאה"
#define D_UPTIME "זמן עליה"
#define D_USER "משתמש"
#define D_UTC_TIME "UTC"
#define D_UV_INDEX "UV אינדקס"
#define D_UV_INDEX_1 "Low"
#define D_UV_INDEX_2 "Mid"
#define D_UV_INDEX_3 "High"
#define D_UV_INDEX_4 "Danger"
#define D_UV_INDEX_5 "BurnL1/2"
#define D_UV_INDEX_6 "BurnL3"
#define D_UV_INDEX_7 "OoR"         // Out of Range
#define D_UV_LEVEL "UV רמת"
#define D_UV_POWER "UV Power"
#define D_VERSION "גרסה"
#define D_VOLTAGE "מתח"
#define D_WEIGHT "Weight"
#define D_WARMLIGHT "חום"
#define D_WEB_SERVER "Web שרת"

// sonoff.ino
#define D_WARNING_MINIMAL_VERSION "WARNING This version does not support persistent settings"
#define D_LEVEL_10 "level 1-0"
#define D_LEVEL_01 "level 0-1"
#define D_SERIAL_LOGGING_DISABLED "Serial logging disabled"
#define D_SYSLOG_LOGGING_REENABLED "Syslog logging re-enabled"

#define D_SET_BAUDRATE_TO "Set Baudrate to"
#define D_RECEIVED_TOPIC "Received Topic"
#define D_DATA_SIZE "Data Size"
#define D_ANALOG_INPUT "אנלוגי"

// support.ino
#define D_OSWATCH "osWatch"
#define D_BLOCKED_LOOP "Blocked Loop"
#define D_WPS_FAILED_WITH_STATUS "WPSconfig FAILED with status"
#define D_ACTIVE_FOR_3_MINUTES "active for 3 minutes"
#define D_FAILED_TO_START "failed to start"
#define D_PATCH_ISSUE_2186 "Patch issue 2186"
#define D_CONNECTING_TO_AP "Connecting to AP"
#define D_IN_MODE "in mode"
#define D_CONNECT_FAILED_NO_IP_ADDRESS "Connect failed as no IP address received"
#define D_CONNECT_FAILED_AP_NOT_REACHED "Connect failed as AP cannot be reached"
#define D_CONNECT_FAILED_WRONG_PASSWORD "Connect failed with AP incorrect password"
#define D_CONNECT_FAILED_AP_TIMEOUT "Connect failed with AP timeout"
#define D_ATTEMPTING_CONNECTION "Attempting connection..."
#define D_CHECKING_CONNECTION "Checking connection..."
#define D_QUERY_DONE "Query done. MQTT services found"
#define D_MQTT_SERVICE_FOUND "MQTT service found on"
#define D_FOUND_AT "found at"
#define D_SYSLOG_HOST_NOT_FOUND "Syslog Host not found"

// settings.ino
#define D_SAVED_TO_FLASH_AT "Saved to flash at"
#define D_LOADED_FROM_FLASH_AT "Loaded from flash at"
#define D_USE_DEFAULTS "השתמש בהגדרות ברירת המחדל"
#define D_ERASED_SECTOR "סקטור מחוק"

// xdrv_02_webserver.ino
#define D_NOSCRIPT "JavaScript - כדי להשתמש ב קושחה אסמוטה אנא הפעל"
#define D_MINIMAL_FIRMWARE_PLEASE_UPGRADE "קושחה מינימלית - בבקשה אנא שדרג"
#define D_WEBSERVER_ACTIVE_ON "Web server active on"
#define D_WITH_IP_ADDRESS "with IP address"
#define D_WEBSERVER_STOPPED "Web server stopped"
#define D_FILE_NOT_FOUND "File Not Found"
#define D_REDIRECTED "Redirected to captive portal"
#define D_WIFIMANAGER_SET_ACCESSPOINT_AND_STATION "Wifimanager set AccessPoint and keep Station"
#define D_WIFIMANAGER_SET_ACCESSPOINT "Wifimanager set AccessPoint"
#define D_TRYING_TO_CONNECT "מנסה לחבר את ההתקן לרשת"

#define D_RESTART_IN "הפעלה מחדש תןך"
#define D_SECONDS "שניות"
#define D_DEVICE_WILL_RESTART "ההתקן יופעל מחדש בעוד מס' שניות"
#define D_BUTTON_TOGGLE "מצב"
#define D_CONFIGURATION "הגדרות"
#define D_INFORMATION "מידע"
#define D_FIRMWARE_UPGRADE "שדרוג קושחה"
#define D_CONSOLE "קונסול"
#define D_CONFIRM_RESTART "אישור הפעלה מחדש"

#define D_CONFIGURE_MODULE "הגדרות מודול"
#define D_CONFIGURE_WIFI "WIFI הגדרות"
#define D_CONFIGURE_MQTT "MQTT הגדרות"
#define D_CONFIGURE_DOMOTICZ "Domoticz הגדרות"
#define D_CONFIGURE_LOGGING "הגדרת לוגים"
#define D_CONFIGURE_OTHER "הגדרות שונות"
#define D_CONFIRM_RESET_CONFIGURATION "אישור שינוי הגדרות"
#define D_RESET_CONFIGURATION "אתחול הגדרות"
#define D_BACKUP_CONFIGURATION "גיבוי הגדרות"
#define D_RESTORE_CONFIGURATION "שחזור הגדרות"
#define D_MAIN_MENU "תפריט ראשי"

#define D_MODULE_PARAMETERS "מודול פרמטרים"
#define D_MODULE_TYPE "סוג מודול"
#define D_GPIO " רגל "
#define D_SERIAL_IN "כניסת סריאל"
#define D_SERIAL_OUT "יציאת סריאל"

#define D_WIFI_PARAMETERS "Wifi פרמטרים"
#define D_SCAN_FOR_WIFI_NETWORKS "סורק עבור רשתות אלחוטיות"
#define D_SCAN_DONE "סריקה הושלמה"
#define D_NO_NETWORKS_FOUND "לא נמצאו רשתות אלחוטיות"
#define D_REFRESH_TO_SCAN_AGAIN "רענן כדי לסרוק שוב"
#define D_DUPLICATE_ACCESSPOINT "נקודות גישה משוכפלות"
#define D_SKIPPING_LOW_QUALITY "מדלג עקב איכות רשת נמוכה"
#define D_RSSI "RSSI"
#define D_WEP "WEP"
#define D_WPA_PSK "WPA PSK"
#define D_WPA2_PSK "WPA2 PSK"
#define D_AP1_SSID "AP1 SSId"
#define D_AP1_PASSWORD "AP1 Password"
#define D_AP2_SSID "AP2 SSId"
#define D_AP2_PASSWORD "AP2 Password"

#define D_MQTT_PARAMETERS "MQTT פרמטרים"
#define D_CLIENT "לקוח"
#define D_FULL_TOPIC "זיהוי מלא"

#define D_LOGGING_PARAMETERS "פרמטרי לוגים"
#define D_SERIAL_LOG_LEVEL "רמת לוג עבור סריאל"
#define D_WEB_LOG_LEVEL "רמת לוג עבור אתר"
#define D_SYS_LOG_LEVEL "Syslog רמת לוג עבור שרת"
#define D_MORE_DEBUG "מיפוי נוסף"
#define D_SYSLOG_HOST "Syslog מארח"
#define D_SYSLOG_PORT "Syslog פורט"
#define D_TELEMETRY_PERIOD "Telemetry period"

#define D_OTHER_PARAMETERS "פרמטרים שונים"
#define D_WEB_ADMIN_PASSWORD "סיסמת מנהל - אתר"
#define D_MQTT_ENABLE "MQTT אפשר"
#define D_FRIENDLY_NAME "שם ידידותי"
#define D_BELKIN_WEMO "Belkin WeMo"
#define D_HUE_BRIDGE "Hue Bridge"
#define D_SINGLE_DEVICE "התקן בודד"
#define D_MULTI_DEVICE "התקנים"

#define D_SAVE_CONFIGURATION "שמירת הגדרות"
#define D_CONFIGURATION_SAVED "הגדרות נשמרו"
#define D_CONFIGURATION_RESET "איפוס הגדרות"

#define D_PROGRAM_VERSION "גירסת תוכנה"
#define D_BUILD_DATE_AND_TIME "Build Date & Time"
#define D_CORE_AND_SDK_VERSION "Core/SDK Version"
#define D_FLASH_WRITE_COUNT "מונה צריבות"
#define D_MAC_ADDRESS "MAC כתובת"
#define D_MQTT_HOST "MQTT מארח"
#define D_MQTT_PORT "MQTT פורט"
#define D_MQTT_CLIENT "MQTT לקוח"
#define D_MQTT_USER "MQTT שם משתמש"
#define D_MQTT_TOPIC "MQTT נושא"
#define D_MQTT_GROUP_TOPIC "MQTT נושא קבוצת"
#define D_MQTT_FULL_TOPIC "MQTT נושא מלא"
#define D_MDNS_DISCOVERY "mDNS זיהוי"
#define D_MDNS_ADVERTISE "mDNS פרסום"
#define D_ESP_CHIP_ID "ESP מס' רכיב"
#define D_FLASH_CHIP_ID "מס' רכיב פלאש"
#define D_FLASH_CHIP_SIZE "גודל זיכרון פלאש"
#define D_FREE_PROGRAM_SPACE "מקום פנוי - תוכנה"

#define D_UPGRADE_BY_WEBSERVER "שדרוג קושחה"
#define D_OTA_URL "OTA כתובת"
#define D_START_UPGRADE "התחל בשדרוג"
#define D_UPGRADE_BY_FILE_UPLOAD "שדרוג דרך קובץ נכשל"
#define D_UPLOAD_STARTED "העלאה מתחילה"
#define D_UPGRADE_STARTED "שדרוג מתחיל"
#define D_UPLOAD_DONE "העלאה הסתיימה"
#define D_UPLOAD_ERR_1 "לא נבחר קובץ"
#define D_UPLOAD_ERR_2 "אין מספיק מקום"
#define D_UPLOAD_ERR_3 "Magic byte is not 0xE9"
#define D_UPLOAD_ERR_4 "גודל קובץ השדרוג גדול מנפח האחסון של הפלאש"
#define D_UPLOAD_ERR_5 "מאגר העלאה לא תקין"
#define D_UPLOAD_ERR_6 "העלאה נכשלה. אפשר רישום 3"
#define D_UPLOAD_ERR_7 "ההעלאה בוטלה"
#define D_UPLOAD_ERR_8 "קובץ שגוי"
#define D_UPLOAD_ERR_9 "קובץ גדול מדי"
#define D_UPLOAD_ERR_10 "נכשלה RF הפעלת שבב"
#define D_UPLOAD_ERR_11 "נכשלה RF מחיקת שבב"
#define D_UPLOAD_ERR_12 "נכשלה RF כתיבת שבב"
#define D_UPLOAD_ERR_13 "נכשלה RF קידוד קושחת שבב"
#define D_UPLOAD_ERROR_CODE "שגיאת קוד העלאה"

#define D_ENTER_COMMAND "הקש פקודה"
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
#define D_MULTICAST_DISABLED "Multicast disabled"
#define D_MULTICAST_REJOINED "Multicast (re)joined"
#define D_MULTICAST_JOIN_FAILED "Multicast join failed"
#define D_FAILED_TO_SEND_RESPONSE "Failed to send response"

#define D_WEMO "WeMo"
#define D_WEMO_BASIC_EVENT "WeMo basic event"
#define D_WEMO_EVENT_SERVICE "WeMo event service"
#define D_WEMO_META_SERVICE "WeMo meta service"
#define D_WEMO_SETUP "WeMo setup"
#define D_RESPONSE_SENT "Response sent"

#define D_HUE "Hue"
#define D_HUE_BRIDGE_SETUP "Hue setup"
#define D_HUE_API_NOT_IMPLEMENTED "Hue API not implemented"
#define D_HUE_API "Hue API"
#define D_HUE_POST_ARGS "Hue POST args"
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
#define D_DOMOTICZ_UPDATE_TIMER "Update timer"

// xdrv_09_timers.ino
#define D_CONFIGURE_TIMER "הגדרות תזמון"
#define D_TIMER_PARAMETERS "פרמטרים עבור תזמון"
#define D_TIMER_ENABLE "אפשר תזמון"
#define D_TIMER_ARM "חמש"
#define D_TIMER_TIME "זמן"
#define D_TIMER_DAYS "ימים"
#define D_TIMER_REPEAT "חזרות"
#define D_TIMER_OUTPUT "פלט"
#define D_TIMER_ACTION "פעולה"

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
#define D_ENERGY_TODAY "צריכה יומית"
#define D_ENERGY_YESTERDAY "צריכה בעבר"
#define D_ENERGY_TOTAL "צריכה כללית"

// xsns_05_ds18b20.ino
#define D_SENSOR_BUSY "Sensor busy"
#define D_SENSOR_CRC_ERROR "Sensor CRC error"
#define D_SENSORS_FOUND "Sensors found"

// xsns_06_dht.ino
#define D_TIMEOUT_WAITING_FOR "Timeout waiting for"
#define D_START_SIGNAL_LOW "start signal low"
#define D_START_SIGNAL_HIGH "start signal high"
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

// sonoff_template.h
#define D_SENSOR_NONE     "None"
#define D_SENSOR_DHT11    "DHT11"
#define D_SENSOR_AM2301   "AM2301"
#define D_SENSOR_SI7021   "SI7021"
#define D_SENSOR_DS18X20  "DS18x20"
#define D_SENSOR_I2C_SCL  "I2C SCL"
#define D_SENSOR_I2C_SDA  "I2C SDA"
#define D_SENSOR_WS2812   "WS2812"
#define D_SENSOR_DFR562   "MP3 Player"
#define D_SENSOR_IRSEND   "IRsend"
#define D_SENSOR_SWITCH   "מתג"   // Suffix "1"
#define D_SENSOR_BUTTON   "לחצן"   // Suffix "1"
#define D_SENSOR_RELAY    "ממסר"    // Suffix "1i"
#define D_SENSOR_LED      "לד"      // Suffix "1i"
#define D_SENSOR_PWM      "PWM"      // Suffix "1"
#define D_SENSOR_COUNTER  "מונה"  // Suffix "1"
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

#endif  // _LANGUAGE_HE_HE_H_
