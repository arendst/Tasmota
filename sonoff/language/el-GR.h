/*
  el-GR.h - localization for Greek - Greece for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends (translated by Nick Galfas)

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

#ifndef _LANGUAGE_EL_GR_H_
#define _LANGUAGE_EL_GR_H_

/*************************** ATTENTION *******************************\
 *
 * Due to memory constraints only UTF-8 is supported.
 * To save code space keep text as short as possible.
 * Time and Date provided by SDK can not be localized (yet).
 * Use online command StateText to translate ON, OFF, HOLD and TOGGLE.
 * Use online command Prefix to translate cmnd, stat and tele.
 *
 * Updated until v6.3.0
\*********************************************************************/

#define LANGUAGE_MODULE_NAME         // Enable to display "Module Generic" (ie Spanish), Disable to display "Generic Module" (ie English)

#define LANGUAGE_LCID 1032
// HTML (ISO 639-1) Language Code
#define D_HTML_LANGUAGE "el"

// "2017-03-07T11:08:02" - ISO8601:2004
#define D_YEAR_MONTH_SEPARATOR "-"
#define D_MONTH_DAY_SEPARATOR "-"
#define D_DATE_TIME_SEPARATOR "T"
#define D_HOUR_MINUTE_SEPARATOR ":"
#define D_MINUTE_SECOND_SEPARATOR ":"

#define D_DAY3LIST "ΚυρΔευΤριΤετΠεμΠαρΣαβ"
#define D_MONTH3LIST "ΙανΦεβΜαρΑπρΜαϊΙονΙολΑυγΣεπΟκτΝοεΔεκ"

// Non JSON decimal separator
#define D_DECIMAL_SEPARATOR ","

// Common
#define D_ADMIN "Διαχειριστής"
#define D_AIR_QUALITY "Ποιότητα αέρα"
#define D_AP "AP"                    // Access Point
#define D_AS "ως"
#define D_AUTO "ΑΥΤΟΜΑΤΟ"
#define D_BLINK "Blink"
#define D_BLINKOFF "BlinkOff"
#define D_BOOT_COUNT "Καταμέτρηση εκκινήσεων"
#define D_BRIGHTLIGHT "Φωτεινό"
#define D_BSSID "BSSId"
#define D_BUTTON "Πλήκτρο"
#define D_BY "από"                    // Written by me
#define D_BYTES "Bytes"
#define D_CELSIUS "Κελσίου"
#define D_CHANNEL "Κανάλι"
#define D_CO2 "Διοξείδιο του άνθρακα"
#define D_CODE "κώδικας"                // Button code
#define D_COLDLIGHT "Ψυχρό"
#define D_COMMAND "Εντολή"
#define D_CONNECTED "Συνδεδεμένο"
#define D_COUNT "Μέτρηση"
#define D_COUNTER "Μετρητής"
#define D_CURRENT "Τάση"          // As in Voltage and Current
#define D_DATA "Δεδομένα"
#define D_DARKLIGHT "Σκοτεινό"
#define D_DEBUG "Debug"
#define D_DISABLED "Ανενεργό"
#define D_DISTANCE "Απόσταση"
#define D_DNS_SERVER "Διακομιστής DNS"
#define D_DONE "Ολοκληρώθηκε"
#define D_DST_TIME "DST"
#define D_ECO2 "eCO2"
#define D_EMULATION "Εξομoίωση"
#define D_ENABLED "Ενεργό"
#define D_ERASE "Διαγραφή"
#define D_ERROR "Σφάλμα"
#define D_FAHRENHEIT "Φαρενάιτ"
#define D_FAILED "Απέτυχε"
#define D_FALLBACK "Fallback"
#define D_FALLBACK_TOPIC "Fallback Topic"
#define D_FALSE "Ψευδές"
#define D_FILE "Αρχείο"
#define D_FREE_MEMORY "Ελεύθερη μνήμη"
#define D_FREQUENCY "Συχνότητα"
#define D_GAS "Αέριο"
#define D_GATEWAY "Πύλη"
#define D_GROUP "Ομάδα"
#define D_HOST "Host"
#define D_HOSTNAME "Hostname"
#define D_HUMIDITY "Υγρασία"
#define D_ILLUMINANCE "Φωτεινότητα"
#define D_IMMEDIATE "άμεσο"      // Button immediate
#define D_INDEX "Κατάλογος"
#define D_INFO "Πληροφορίες"
#define D_INFRARED "Υπέρυθρο"
#define D_INITIALIZED "Αρχικοποιήθηκε"
#define D_IP_ADDRESS "Διεύθυνση IP"
#define D_LIGHT "Φως"
#define D_LWT "LWT"
#define D_MODULE "Μονάδα"
#define D_MQTT "MQTT"
#define D_MULTI_PRESS "ανίχνευση για πολλαπλά πατήματα"
#define D_NOISE "Θόρυβος"
#define D_NONE "Κανένα"
#define D_OFF "Off"
#define D_OFFLINE "Offline"
#define D_OK "Ok"
#define D_ON "On"
#define D_ONLINE "Online"
#define D_PASSWORD "Κωδικός"
#define D_PORT "Θύρα"
#define D_POWER_FACTOR "Συντελεστής Ισχύος"
#define D_POWERUSAGE "Ισχύς"
#define D_POWERUSAGE_ACTIVE "Ενεργός ισχύς"
#define D_POWERUSAGE_APPARENT "Φαινόμενη ισχύς"
#define D_POWERUSAGE_REACTIVE "Άεργη ισχύς"
#define D_PRESSURE "Πίεση"
#define D_PRESSUREATSEALEVEL "Πίεση στo επίπεδο θάλασσας"
#define D_PROGRAM_FLASH_SIZE "Μέγεθος προγράμματος στη Flash"
#define D_PROGRAM_SIZE "Μέγεθος προγράμματος"
#define D_PROJECT "Έργο"
#define D_RAIN "Rain"
#define D_RECEIVED "Ελήφθη"
#define D_RESTART "Επανεκκίνηση"
#define D_RESTARTING "Επανεκκινεί"
#define D_RESTART_REASON "Αιτία επανεκκίνησης"
#define D_RESTORE "επαναφορά"
#define D_RETAINED "διακράτηση"
#define D_RULE "Κανόνας"
#define D_SAVE "Αποθήκευση"
#define D_SENSOR "Αισθητήρας"
#define D_SSID "SSId"
#define D_START "Εκκίνηση"
#define D_STD_TIME "STD"
#define D_STOP "Τερματισμός"
#define D_SUBNET_MASK "Μάσκα υποδικτύου"
#define D_SUBSCRIBE_TO "Εγγραφή στο"
#define D_SUCCESSFUL "Επιτυχές"
#define D_SUNRISE "Σούρουπο"
#define D_SUNSET "Ηλιοβασίλεμα"
#define D_TEMPERATURE "Θερμοκρασία"
#define D_TO "έως"
#define D_TOGGLE "Εναλλαγή"
#define D_TOPIC "Topic"
#define D_TRANSMIT "Μετάδοση"
#define D_TRUE "Αληθές"
#define D_TVOC "TVOC"
#define D_UPGRADE "αναβάθμιση"
#define D_UPLOAD "Ανέβασμα"
#define D_UPTIME "Χρόνος λειτουργίας"
#define D_USER "Χρήστης"
#define D_UTC_TIME "UTC"
#define D_UV_INDEX "Δείκτης UV"
#define D_UV_INDEX_1 "Χαμηλό"
#define D_UV_INDEX_2 "Μέτριο"
#define D_UV_INDEX_3 "Υψηλό"
#define D_UV_INDEX_4 "Κίνδυνος"
#define D_UV_INDEX_5 "Έγκαυμα L1/2"
#define D_UV_INDEX_6 "Έγκαυμα L3"
#define D_UV_INDEX_7 "OoR"
#define D_UV_LEVEL "Επίπεδο UV"
#define D_UV_POWER "Ένταση UV"
#define D_VERSION "Έκδοση"
#define D_VOLTAGE "Τάση"
#define D_WEIGHT "Βάρος"
#define D_WARMLIGHT "Θερμό"
#define D_WEB_SERVER "Διακομιστής Web"

// sonoff.ino
#define D_WARNING_MINIMAL_VERSION "ΠΡΟΕΙΔΟΠΟΙΗΣΗ Αυτή η έκδοση δεν αποθηκεύει τις ρυθμίσεις"
#define D_LEVEL_10 "επίπεδο 1-0"
#define D_LEVEL_01 "επίπεδο 0-1"
#define D_SERIAL_LOGGING_DISABLED "Η καταγραφή σειριακής θύρας είναι απενεργοποιημένη"
#define D_SYSLOG_LOGGING_REENABLED "Η καταγραφή Syslog επαναενεργοποιήθηκε"

#define D_SET_BAUDRATE_TO "Ορισμός Baudrate σε"
#define D_RECEIVED_TOPIC "Received Topic"
#define D_DATA_SIZE "Μέγεθος δεδομένων"
#define D_ANALOG_INPUT "Αναλογικό"

// support.ino
#define D_OSWATCH "osWatch"
#define D_BLOCKED_LOOP "Blocked Loop"
#define D_WPS_FAILED_WITH_STATUS "Το WPSconfig ΑΠΕΤΥΧΕ με status"
#define D_ACTIVE_FOR_3_MINUTES "ενεργό για 3 λεπτά"
#define D_FAILED_TO_START "απέτυχε να εκκινήσει"
#define D_PATCH_ISSUE_2186 "Patch issue 2186"
#define D_CONNECTING_TO_AP "Συνδέεται στο AP"
#define D_IN_MODE "σε mode"
#define D_CONNECT_FAILED_NO_IP_ADDRESS "Αποτυχία σύνδεσης, δεν απονεμήθηκε διεύθυνση IP"
#define D_CONNECT_FAILED_AP_NOT_REACHED "Αποτυχία σύνδεσης, δεν ανταποκρίνεται το AP"
#define D_CONNECT_FAILED_WRONG_PASSWORD "Αποτυχία σύνδεσης, λάθος κωδικός για το AP"
#define D_CONNECT_FAILED_AP_TIMEOUT "Αποτυχία σύνδεσης, λήξη ορίου απόκρισης από το AP"
#define D_ATTEMPTING_CONNECTION "Προσπάθεια για σύνδεση..."
#define D_CHECKING_CONNECTION "Έλεγχος σύνδεσης..."
#define D_QUERY_DONE "Ολοκλήρωση ερωτήματος. Βρέθηκε υπηρεσία MQTT"
#define D_MQTT_SERVICE_FOUND "Βρέθηκε υπηρεσία MQTT στο"
#define D_FOUND_AT "βρέθηκε στο"
#define D_SYSLOG_HOST_NOT_FOUND "Δε βρέθηκε Syslog Host"

// settings.ino
#define D_SAVED_TO_FLASH_AT "Αποθηκεύτηκε από τη Flash στο"
#define D_LOADED_FROM_FLASH_AT "Φορτώθηκε από τη Flash στο"
#define D_USE_DEFAULTS "Χρήση προεπιλογών"
#define D_ERASED_SECTOR "Διαγραφή τομέα"

// webserver.ino
#define D_NOSCRIPT "To use Tasmota, please enable JavaScript"
#define D_MINIMAL_FIRMWARE_PLEASE_UPGRADE "MINIMAL firmware - παρακαλώ αναβαθμίστε"
#define D_WEBSERVER_ACTIVE_ON "Ενεργός διακομιστής Web στο"
#define D_WITH_IP_ADDRESS "με διεύθυνση IP"
#define D_WEBSERVER_STOPPED "Ο διακομιστής Web σταμάτησε"
#define D_FILE_NOT_FOUND "Δε βρέθηκε το αρχείο"
#define D_REDIRECTED "Ανακατεύθυνση στο captive portal"
#define D_WIFIMANAGER_SET_ACCESSPOINT_AND_STATION "Wifimanager set AccessPoint and keep Station"
#define D_WIFIMANAGER_SET_ACCESSPOINT "Wifimanager set AccessPoint"
#define D_TRYING_TO_CONNECT "Προσπάθεια σύνδεσης της συσκευής σε ασύρματο δίκτυο"

#define D_RESTART_IN "Επανεκκίνηση σε"
#define D_SECONDS "δευτερόλεπτα"
#define D_DEVICE_WILL_RESTART "Η συσκευή θα επανεκκινήσει σε λίγα δευτερόλεπτα"
#define D_BUTTON_TOGGLE "Εναλλαγή"
#define D_CONFIGURATION "Ρυθμίσεις"
#define D_INFORMATION "Πληροφορίες"
#define D_FIRMWARE_UPGRADE "Αναβάθμιση Firmware"
#define D_CONSOLE "Κονσόλα"
#define D_CONFIRM_RESTART "Επιβεβαίωση επανεκκίνησης"

#define D_CONFIGURE_MODULE "Ρυθμίσεις μονάδας"
#define D_CONFIGURE_WIFI "Ρυθμίσεις WiFi"
#define D_CONFIGURE_MQTT "Ρυθμίσεις MQTT"
#define D_CONFIGURE_DOMOTICZ "Ρυθμίσεις Domoticz"
#define D_CONFIGURE_LOGGING "Ρυθμίσεις καταγραφής"
#define D_CONFIGURE_OTHER "Άλλες ρυθμίσεις"
#define D_CONFIRM_RESET_CONFIGURATION "Επιβεβαίωση αρχικοποίησης στις προεπιλεγμένες ρυθμίσεις"
#define D_RESET_CONFIGURATION "Αρχικοποίηση-reset ρυθμίσεων"
#define D_BACKUP_CONFIGURATION "Αποθήκευση ρυθμίσεων"
#define D_RESTORE_CONFIGURATION "Επαναφορά ρυθμίσεων"
#define D_MAIN_MENU "Κεντρικό μενού"

#define D_MODULE_PARAMETERS "Παράμετροι μονάδας"
#define D_MODULE_TYPE "Τύπος μονάδας"
#define D_GPIO "GPIO"
#define D_SERIAL_IN "Serial In"
#define D_SERIAL_OUT "Serial Out"

#define D_WIFI_PARAMETERS "Παράμετροι Wifi"
#define D_SCAN_FOR_WIFI_NETWORKS "Σάρωση για δίκτυα Wifi"
#define D_SCAN_DONE "Η σάρωση ολοκληρώθηκε"
#define D_NO_NETWORKS_FOUND "Δε βρέθηκαν δίκτυα"
#define D_REFRESH_TO_SCAN_AGAIN "Πατήστε ανανέωση για μια νέα σάρωση"
#define D_DUPLICATE_ACCESSPOINT "Διπλότυπο AccessPoint"
#define D_SKIPPING_LOW_QUALITY "Παράκαμψη λόγω χαμηλής ποιότητας"
#define D_RSSI "RSSI"
#define D_WEP "WEP"
#define D_WPA_PSK "WPA PSK"
#define D_WPA2_PSK "WPA2 PSK"
#define D_AP1_SSID "AP1 SSId"
#define D_AP1_PASSWORD "AP1 Κωδικός"
#define D_AP2_SSID "AP2 SSId"
#define D_AP2_PASSWORD "AP2 Κωδικός"

#define D_MQTT_PARAMETERS "Παράμετροι MQTT"
#define D_CLIENT "Πελάτης"
#define D_FULL_TOPIC "Full Topic"

#define D_LOGGING_PARAMETERS "Παράμετροι καταγραφής"
#define D_SERIAL_LOG_LEVEL "Επίπεδο καταγραφής Σειριακής"
#define D_WEB_LOG_LEVEL "Επίπεδο καταγραφής Web"
#define D_SYS_LOG_LEVEL "Επίπεδο καταγραφής Syslog"
#define D_MORE_DEBUG "More debug"
#define D_SYSLOG_HOST "Εξυπηρετητής Syslog"
#define D_SYSLOG_PORT "Θύρα Syslog"
#define D_TELEMETRY_PERIOD "Περίοδος τηλεμετρίας"

#define D_OTHER_PARAMETERS "Άλλες παράμετροι"
#define D_WEB_ADMIN_PASSWORD "Κωδικός διαχειριστή"
#define D_MQTT_ENABLE "Ενεργοποίηση MQTT"
#define D_FRIENDLY_NAME "Φιλική ονομασία"
#define D_BELKIN_WEMO "Belkin WeMo"
#define D_HUE_BRIDGE "Hue Bridge"
#define D_SINGLE_DEVICE "μονή συσκευή"
#define D_MULTI_DEVICE "πολλαπλές συσκευές"

#define D_SAVE_CONFIGURATION "Αποθήκευση ρυθμίσεων"
#define D_CONFIGURATION_SAVED "Οι ρυθμίσεις αποθηκεύτηκαν"
#define D_CONFIGURATION_RESET "Επαναφορά ρυθμίσεων"

#define D_PROGRAM_VERSION "Έκδοση προγράμματος"
#define D_BUILD_DATE_AND_TIME "Ημερομηνία έκδοσης"
#define D_CORE_AND_SDK_VERSION "Έκδοση Core/SDK"
#define D_FLASH_WRITE_COUNT "Καταμέτρηση εγγραφών στη Flash"
#define D_MAC_ADDRESS "MAC Address"
#define D_MQTT_HOST "MQTT Διακομιστής"
#define D_MQTT_PORT "MQTT Θύρα"
#define D_MQTT_CLIENT "MQTT Πελάτης"
#define D_MQTT_USER "MQTT Χρήστης"
#define D_MQTT_TOPIC "MQTT Topic"
#define D_MQTT_GROUP_TOPIC "MQTT Group Topic"
#define D_MQTT_FULL_TOPIC "MQTT Full Topic"
#define D_MDNS_DISCOVERY "mDNS Discovery"
#define D_MDNS_ADVERTISE "mDNS Advertise"
#define D_ESP_CHIP_ID "ESP Chip Id"
#define D_FLASH_CHIP_ID "Flash Chip Id"
#define D_FLASH_CHIP_SIZE "Μέγεθος Flash"
#define D_FREE_PROGRAM_SPACE "Ελεύθερος χώρος προγράμματος"

#define D_UPGRADE_BY_WEBSERVER "Αναβάθμιση μέσω web server"
#define D_OTA_URL "OTA URL"
#define D_START_UPGRADE "Εκκίνηση αναβάθμισης"
#define D_UPGRADE_BY_FILE_UPLOAD "Αναβάθμιση μέσω μεταφόρτωσης αρχείου"
#define D_UPLOAD_STARTED "Η μεταφόρτωση ξεκίνησε"
#define D_UPGRADE_STARTED "Η αναβάθμιση ξεκίνησε"
#define D_UPLOAD_DONE "Η μεταφόρτωση ολοκληρώθηκε"
#define D_UPLOAD_ERR_1 "Δεν έχει επιλεγεί αρχείο"
#define D_UPLOAD_ERR_2 "Δεν υπάρχει επαρκής χώρος"
#define D_UPLOAD_ERR_3 "Magic byte is not 0xE9"
#define D_UPLOAD_ERR_4 "Το μέγεθος προγράμματος είναι μεγαλύτερο από την πραγματική μνήμη Flash"
#define D_UPLOAD_ERR_5 "Upload buffer miscompare"
#define D_UPLOAD_ERR_6 "Η μεταφόρτωση απέτυχε. Επιλέξτε επίπεδο καταγραφής 3"
#define D_UPLOAD_ERR_7 "Η μεταφόρτωση ματαιώθηκε"
#define D_UPLOAD_ERR_8 "Μη έγκυρο αρχείο"
#define D_UPLOAD_ERR_9 "Πολύ μεγάλο αρχείο"
#define D_UPLOAD_ERR_10 "Αποτυχία αρχικοποίησης RF chip"
#define D_UPLOAD_ERR_11 "Αποτυχία σβησίματος στο RF chip"
#define D_UPLOAD_ERR_12 "Αποτυχία εγγραφής στο RF chip"
#define D_UPLOAD_ERR_13 "Failed to decode RF firmware"
#define D_UPLOAD_ERROR_CODE "Κωδικός λάθους στη μεταφόρτωση"

#define D_ENTER_COMMAND "Εισαγωγή εντολής"
#define D_ENABLE_WEBLOG_FOR_RESPONSE "Ενεργοποιήστε το weblog 2 εαν περιμένετε απόκριση"
#define D_NEED_USER_AND_PASSWORD "Χρειάζεται user=<username>&password=<password>"

// xdrv_01_mqtt.ino
#define D_FINGERPRINT "Επιβεβαίωση αποτυπώματος TLS..."
#define D_TLS_CONNECT_FAILED_TO "Αποτυχία σύνδεσης TLS στο"
#define D_RETRY_IN "Νέα προσπάθεια σε"
#define D_VERIFIED "Επιβεβαιώθηκε με χρήση αποτυπώματος TLS"
#define D_INSECURE "Επισφαλής σύνδεση λόγω άκυρου αποτυπώματος TLS"
#define D_CONNECT_FAILED_TO "Αποτυχία σύνδεσης στο"

// xplg_wemohue.ino
#define D_MULTICAST_DISABLED "Απενεργοποιημένο Multicast"
#define D_MULTICAST_REJOINED "Επανασύνδεση Multicast"
#define D_MULTICAST_JOIN_FAILED "Αποτυχία σύνδεσης Multicast"
#define D_FAILED_TO_SEND_RESPONSE "Αποτυχία αποστολής απόκρισης"

#define D_WEMO "WeMo"
#define D_WEMO_BASIC_EVENT "WeMo βασικό event"
#define D_WEMO_EVENT_SERVICE "WeMo υπηρεσία event"
#define D_WEMO_META_SERVICE "WeMo υπηρεσία meta"
#define D_WEMO_SETUP "WeMo ρυθμίσεις"
#define D_RESPONSE_SENT "Αποστολή απάντησης"

#define D_HUE "Hue"
#define D_HUE_BRIDGE_SETUP "Hue ρυθμίσεις"
#define D_HUE_API_NOT_IMPLEMENTED "Δε βρέθηκε εφαρμογή Hue API"
#define D_HUE_API "Hue API"
#define D_HUE_POST_ARGS "Hue POST args"
#define D_3_RESPONSE_PACKETS_SENT "Στάλθηκαν 3 πακέτα απόκρισης"

// xdrv_07_domoticz.ino
#define D_DOMOTICZ_PARAMETERS "Ρυθμίσεις Domoticz"
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
#define D_CONFIGURE_TIMER "Ρυθμίσεις Χρονικών"
#define D_TIMER_PARAMETERS "Παράμετροι χρονικών"
#define D_TIMER_ENABLE "Ενεργοποίηση χρονικών"
#define D_TIMER_ARM "Οπλισμένο"
#define D_TIMER_TIME "Ώρα"
#define D_TIMER_DAYS "Μέρες"
#define D_TIMER_REPEAT "Επανάληψη"
#define D_TIMER_OUTPUT "Έξοδος"
#define D_TIMER_ACTION "Λειτουργία"

// xdrv_10_knx.ino
#define D_CONFIGURE_KNX "Ρυθμίσεις KNX"
#define D_KNX_PARAMETERS "Παράμετροι KNX"
#define D_KNX_GENERAL_CONFIG "Γενικά"
#define D_KNX_PHYSICAL_ADDRESS "Διεύθυνση Μνήμης"
#define D_KNX_PHYSICAL_ADDRESS_NOTE "( Πρέπει να είναι μοναδική στο KNX δίκτυο )"
#define D_KNX_ENABLE "Ενεργοποίηση KNX"
#define D_KNX_GROUP_ADDRESS_TO_WRITE "Αποστολή δεδομένων σε Ομάδα Διευθύνσεων"
#define D_ADD "Προσθήκη"
#define D_DELETE "Διαγραφή"
#define D_REPLY "Επανάληψη"
#define D_KNX_GROUP_ADDRESS_TO_READ "Ομάδα Διευθύνσεων που θα λάβει τα Δεδομένα"
#define D_LOG_KNX "KNX: "
#define D_RECEIVED_FROM "Στάλθηκε από"
#define D_KNX_COMMAND_WRITE "Εγγραφή"
#define D_KNX_COMMAND_READ "Ανάγνωση"
#define D_KNX_COMMAND_OTHER "Άλλο"
#define D_SENT_TO "αποστολή σε"
#define D_KNX_WARNING "Η Ομάδα Διευθύνσεων ( 0 / 0 / 0 ) είναι δεσμευμένη και δεν μπορεί να χρησιμοποιηθεί."
#define D_KNX_ENHANCEMENT "Βελτίωση επικοινωνίας"
#define D_KNX_TX_SLOT "KNX TX"
#define D_KNX_RX_SLOT "KNX RX"

// xdrv_03_energy.ino
#define D_ENERGY_TODAY "Ενέργεια σήμερα"
#define D_ENERGY_YESTERDAY "Ενέργεια χθες"
#define D_ENERGY_TOTAL "Ενέργεια συνολικά"

// xsns_05_ds18b20.ino
#define D_SENSOR_BUSY "Ο αισθητήρας είναι απασχολημένος"
#define D_SENSOR_CRC_ERROR "Σφάλμα CRC αισθητήρα"
#define D_SENSORS_FOUND "Βρέθηκαν αισθητήρες"

// xsns_06_dht.ino
#define D_TIMEOUT_WAITING_FOR "Αναμονή Timeout για"
#define D_START_SIGNAL_LOW "χαμηλό αρχικό σήμα"
#define D_START_SIGNAL_HIGH "υψηλό αρχικό σήμα"
#define D_PULSE "παλμός"
#define D_CHECKSUM_FAILURE "Αποτυχία Checksum"

// xsns_07_sht1x.ino
#define D_SENSOR_DID_NOT_ACK_COMMAND "Ο αισθητήρας δεν έστειλε εντολή ACK"
#define D_SHT1X_FOUND "Βρέθηκε SHT1X"

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
#define D_HX_CAL_REMOVE "Απομακρύνετε το βαρίδιο"
#define D_HX_CAL_REFERENCE "Τοποθετήστε το βαρίδιο αναφοράς"
#define D_HX_CAL_DONE "Βαθμονομήθηκε"
#define D_HX_CAL_FAIL "Αποτυχία βαθμονόμησης"
#define D_RESET_HX711 "Αρχικοποίηση ζυγαριάς"
#define D_CONFIGURE_HX711 "Ρύθμιση ζυγαριάς"
#define D_HX711_PARAMETERS "Παράμετροι ζυγαριάς"
#define D_ITEM_WEIGHT "Βάρος αντικειμένου"
#define D_REFERENCE_WEIGHT "Βάρος αναφοράς"
#define D_CALIBRATE "Βαθμονόμησε"
#define D_CALIBRATION "Βαθμονόμηση"

//xsns_35_tx20.ino
#define D_TX20_WIND_DIRECTION "Κατεύθυνση ανέμου"
#define D_TX20_WIND_SPEED "Ταχύτητα ανέμου"
#define D_TX20_WIND_SPEED_AVG "Μέση ταχύτητα ανέμου"
#define D_TX20_WIND_SPEED_MAX "Μέγιστη ταχύτητα ανέμου"
#define D_TX20_NORTH "Β"
#define D_TX20_EAST "Α"
#define D_TX20_SOUTH "Ν"
#define D_TX20_WEST "Δ"

// sonoff_template.h
#define D_SENSOR_NONE     "Κανένα"
#define D_SENSOR_DHT11    "DHT11"
#define D_SENSOR_AM2301   "AM2301"
#define D_SENSOR_SI7021   "SI7021"
#define D_SENSOR_DS18X20  "DS18x20"
#define D_SENSOR_I2C_SCL  "I2C SCL"
#define D_SENSOR_I2C_SDA  "I2C SDA"
#define D_SENSOR_WS2812   "WS2812"
#define D_SENSOR_DFR562   "MP3 Player"
#define D_SENSOR_IRSEND   "IRsend"
#define D_SENSOR_SWITCH   "Διακόπτης" // Suffix "1"
#define D_SENSOR_BUTTON   "Κουμπί"    // Suffix "1"
#define D_SENSOR_RELAY    "Ρελέ"      // Suffix "1i"
#define D_SENSOR_LED      "Led"       // Suffix "1i"
#define D_SENSOR_PWM      "PWM"       // Suffix "1"
#define D_SENSOR_COUNTER  "Μετρητής"  // Suffix "1"
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
#define D_SENSOR_AZ_RX   "AZ Rx"
#define D_SENSOR_AZ_TX   "AZ Tx"

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

#endif  // _LANGUAGE_EL_GR_H_
