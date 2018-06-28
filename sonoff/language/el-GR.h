/*
  el-GR.h - localization for Greek - Greece for Sonoff-Tasmota

  Copyright (C) 2018  Kan3Nas

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
 * Updated until v5.12.0n
\*********************************************************************/

//#define LANGUAGE_MODULE_NAME         // Enable to display "Module Generic" (ie Spanish), Disable to display "Generic Module" (ie English)

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
#define D_ADMIN "Admin"
#define D_AIR_QUALITY "Ποιότητα Αέρα"
#define D_AP "AP"                    // Access Point
#define D_AS "as"
#define D_AUTO "ΑΥΤΟΜΑΤΟ"
#define D_BLINK "Αναβοσβήνει"
#define D_BLINKOFF "Δεν αναβοσβήνει"
#define D_BOOT_COUNT "Αριθμός εκκινήσεων"
#define D_BRIGHTLIGHT "Λαμπερό"
#define D_BUTTON "Κουμπί"
#define D_BY "από"                    // Written by me
#define D_BYTES "Bytes"
#define D_CELSIUS "Κελσίου"
#define D_CO2 "Διοξείδιο του άνθρακα"
#define D_CODE "κώδικας"                // Button code
#define D_COLDLIGHT "Ψυχρός"
#define D_COMMAND "Εντολή"
#define D_CONNECTED "Συνδεδεμένο"
#define D_COUNT "Μέτρημα"
#define D_COUNTER "Μετρητής"
#define D_CURRENT "Τάση ρεύματος"          // As in Voltage and Current
#define D_DATA "Δεδομένα"
#define D_DARKLIGHT "Σκούρο"
#define D_DEBUG "Debug"
#define D_DISABLED "Απενεργοποιημένο"
#define D_DISTANCE "Distance"
#define D_DNS_SERVER "Διακομιστής DNS"
#define D_DONE "Εγινε"
#define D_DST_TIME "DST"
#define D_ECO2 "eCO2"
#define D_EMULATION "Emulation"
#define D_ENABLED "Ενεργό"
#define D_ERASE "Σβήσιμο"
#define D_ERROR "Σφάλμα"
#define D_FAHRENHEIT "Φαρενάιτ"
#define D_FAILED "Αποτυχία"
#define D_FALLBACK "Εφεδρικό"
#define D_FALLBACK_TOPIC "Εφεδρικό θέμα"
#define D_FALSE "Λάθος"
#define D_FILE "Αρχείο"
#define D_FREE_MEMORY "Ελεύθερη Μνήμη"
#define D_FREQUENCY "Frequency"
#define D_GAS "Γκάζι"
#define D_GATEWAY "Πύλη"
#define D_GROUP "Ομάδα"
#define D_HOST "Host"
#define D_HOSTNAME "Hostname"
#define D_HUMIDITY "Υγρασία"
#define D_ILLUMINANCE "Illuminance"
#define D_IMMEDIATE "Φωτεινότητα"      // Button immediate
#define D_INDEX "Κατάλογος"
#define D_INFO "Πληροφορίες"
#define D_INFRARED "Infrared"
#define D_INITIALIZED "Αρχικό"
#define D_IP_ADDRESS "IP Address"
#define D_LIGHT "Light"
#define D_LWT "LWT"
#define D_MODULE "Module"
#define D_MQTT "MQTT"
#define D_MULTI_PRESS "πολλαπλή πίεση"
#define D_NOISE "θόρυβος"
#define D_NONE "Κανένα"
#define D_OFF "Κλειστό"
#define D_OFFLINE "Offline"
#define D_OK "Ok"
#define D_ON "Ανοιχτό"
#define D_ONLINE "Online"
#define D_PASSWORD "ΚωδικόςPassword"
#define D_PORT "Πόρτα"
#define D_POWER_FACTOR "Παράγοντας ισχύος"
#define D_POWERUSAGE "Ισχύης"
#define D_POWERUSAGE_ACTIVE "Active Power"
#define D_POWERUSAGE_APPARENT "Apparent Power"
#define D_POWERUSAGE_REACTIVE "Reactive Power"
#define D_PRESSURE "Πίεση"
#define D_PRESSUREATSEALEVEL "Πίεση στην επιφάνεια της Θάλασσας"
#define D_PROGRAM_FLASH_SIZE "Μέγεθος Προγράμματος Flash"
#define D_PROGRAM_SIZE "Μέγεθος Προγράμματος"
#define D_PROJECT "Έργο"
#define D_RECEIVED "Λήψη"
#define D_RESTART "Επανεκκίνηση"
#define D_RESTARTING "Επανεκκίνηση"
#define D_RESTART_REASON "Λόγος Επανεκκίνησης"
#define D_RESTORE "επαναφορά"
#define D_RETAINED "διατηρούνται"
#define D_RULE "Κανόνας"
#define D_SAVE "Αποθήκευση"
#define D_SENSOR "Σένσορας"
#define D_SSID "SSId"
#define D_START "Έναρξη"
#define D_STD_TIME "STD"
#define D_STOP "Στοπ"
#define D_SUBNET_MASK "Μάσκα υποδικτύου"
#define D_SUBSCRIBE_TO "Εγγραφή στο"
#define D_SUCCESSFUL "Επιτυχής"
#define D_SUNRISE "Σούρουπο"
#define D_SUNSET "Ηλιοβασίλεμα"
#define D_TEMPERATURE "Θερμοκρασία"
#define D_TO "στο"
#define D_TOGGLE "Εναλλαγή"
#define D_TOPIC "Θέμα"
#define D_TRANSMIT "Μετάδοση"
#define D_TRUE "Σωστό"
#define D_TVOC "TVOC"
#define D_UPGRADE "αναβάθμιση"
#define D_UPLOAD "Ανέβασμα"
#define D_UPTIME "Uptime"
#define D_USER "Χρήστης"
#define D_UTC_TIME "UTC"
#define D_UV_INDEX "UV Index"
#define D_UV_LEVEL "Επίπεδο UV"
#define D_VERSION "Έκδοση"
#define D_VOLTAGE "Τάση"
#define D_WARMLIGHT "Ζεστό"
#define D_WEB_SERVER "Web διακομιστής"

// sonoff.ino
#define D_WARNING_MINIMAL_VERSION "ΠΡΟΕΙΔΟΠΟΙΗΣΗ Αυτή η έκδοση δεν αποθηκεύει τις ρυθμίσεις"
#define D_LEVEL_10 "επίπεδο 1-0"
#define D_LEVEL_01 "επίπεδο 0-1"
#define D_SERIAL_LOGGING_DISABLED "Η σειριακής καταγραφή απενεργοποιήθηκε"
#define D_SYSLOG_LOGGING_REENABLED "Η καταγραφή Syslog είναι ενεργοποιημένη"

#define D_SET_BAUDRATE_TO "Ρύθμιση Baudrate σε"
#define D_RECEIVED_TOPIC "Λήψη θεμάτων"
#define D_DATA_SIZE "Μέγεθος δεδομένων"
#define D_ANALOG_INPUT "Αναλογικό"

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
#define D_SAVED_TO_FLASH_AT "Εγινε flash στην θέση"
#define D_LOADED_FROM_FLASH_AT "Φορτώθηκε από την flash στην θέση"
#define D_USE_DEFAULTS "Χρήση προεπιλογών"
#define D_ERASED_SECTOR "Διαγραμμένος τομέας"

// xdrv_02_webserver.ino
#define D_MINIMAL_FIRMWARE_PLEASE_UPGRADE "MINIMAL firmware - παρακαλώ αναβαθμήστε"
#define D_WEBSERVER_ACTIVE_ON "Web διακομιστής ενεργός"
#define D_WITH_IP_ADDRESS "με διεύθυνση IP"
#define D_WEBSERVER_STOPPED "Web διακομιστής σταμάτησε"
#define D_FILE_NOT_FOUND "Το αρχείο δεν βρέθηκε"
#define D_REDIRECTED "Ανακατεύθυνση σε captive portal"
#define D_WIFIMANAGER_SET_ACCESSPOINT_AND_STATION "Wifimanager set AccessPoint and keep Station"
#define D_WIFIMANAGER_SET_ACCESSPOINT "Wifimanager set AccessPoint"
#define D_TRYING_TO_CONNECT "Προσπάθεια σύνδεσης της συσκευής στο δίκτυο"

#define D_RESTART_IN "Επανεκκίνηση σε"
#define D_SECONDS "δευτερόλεπτα"
#define D_DEVICE_WILL_RESTART "Η συσκευή θα ξεκινήσει ξανά σε λίγα δευτερόλεπτα"
#define D_BUTTON_TOGGLE "Εναλλαγή"
#define D_CONFIGURATION "Ρύθμιση"
#define D_INFORMATION "Πληροφορίες"
#define D_FIRMWARE_UPGRADE "Αναβάθμιση Firmware"
#define D_CONSOLE "Κονσόλα"
#define D_CONFIRM_RESTART "Επιβεβαίωση Επανεκκίνησης"

#define D_CONFIGURE_MODULE "Ρύθμιση Module"
#define D_CONFIGURE_WIFI "Ρύθμιση WiFi"
#define D_CONFIGURE_MQTT "Ρύθμιση MQTT"
#define D_CONFIGURE_DOMOTICZ "Ρύθμιση Domoticz"
#define D_CONFIGURE_LOGGING "Ρύθμιση καταγραφής"
#define D_CONFIGURE_OTHER "Άλλες Ρυθμίσεις"
#define D_CONFIRM_RESET_CONFIGURATION "Confirm Reset Configuration"
#define D_RESET_CONFIGURATION "Επαναφορά Ρυθμίσεων"
#define D_BACKUP_CONFIGURATION "Αντίγραφα Ασφαλείας Ρυθμίσεων"
#define D_RESTORE_CONFIGURATION "Επαναφορά Ρυθμίσεων"
#define D_MAIN_MENU "Κεντρικό Μενού"

#define D_MODULE_PARAMETERS "Παράμετροι Module"
#define D_MODULE_TYPE "Είδος Module"
#define D_GPIO "GPIO"
#define D_SERIAL_IN "Serial In"
#define D_SERIAL_OUT "Serial Out"

#define D_WIFI_PARAMETERS "Παράμετροι Wifi"
#define D_SCAN_FOR_WIFI_NETWORKS "Σάρωση για δίκτυα wifi"
#define D_SCAN_DONE "Ολοκλήρωση σάρωσης"
#define D_NO_NETWORKS_FOUND "Δεν βρέθηκαν δίκτυα"
#define D_REFRESH_TO_SCAN_AGAIN "Ανανέωση για επανάληψη σάρωσης"
#define D_DUPLICATE_ACCESSPOINT "Duplicate AccessPoint"
#define D_SKIPPING_LOW_QUALITY "Παράλειψη λόγω χαμηλού σήματος"
#define D_RSSI "RSSI"
#define D_WEP "WEP"
#define D_WPA_PSK "WPA PSK"
#define D_WPA2_PSK "WPA2 PSK"
#define D_AP1_SSID "AP1 SSId"
#define D_AP1_PASSWORD "AP1 Κωδικός"
#define D_AP2_SSID "AP2 SSId"
#define D_AP2_PASSWORD "AP2 Κωδικός"

#define D_MQTT_PARAMETERS "MQTT Παράμετροι"
#define D_CLIENT "Πελάτης"
#define D_FULL_TOPIC "Full Topic"

#define D_LOGGING_PARAMETERS "Παράμετροι καταγραφής"
#define D_SERIAL_LOG_LEVEL "Επίπεδο καταγραφής Serial"
#define D_WEB_LOG_LEVEL "Επίπεδο καταγραφής Web"
#define D_SYS_LOG_LEVEL "Επίπεδο καταγραφής Syslog"
#define D_MORE_DEBUG "Επιπλέον debug"
#define D_SYSLOG_HOST "Υπολογιστής Syslog"
#define D_SYSLOG_PORT "Πόρτα Syslog"
#define D_TELEMETRY_PERIOD "Χρόνος τηλεμετρίας"

#define D_OTHER_PARAMETERS "Άλλες Παράμετροι"
#define D_WEB_ADMIN_PASSWORD "Web Admin Κωδικός"
#define D_MQTT_ENABLE "MQTT Ενεργό"
#define D_FRIENDLY_NAME "Φιλικό Όνομα"
#define D_BELKIN_WEMO "Belkin WeMo"
#define D_HUE_BRIDGE "Hue Bridge"
#define D_SINGLE_DEVICE "μονή συσκευή"
#define D_MULTI_DEVICE "πολλαπλή συσκευή"

#define D_SAVE_CONFIGURATION "Αποθήκευση Ρυθμίσεων"
#define D_CONFIGURATION_SAVED "Οι ρυθμίσεις αποθηκεύτηκαν"
#define D_CONFIGURATION_RESET "Επαναφορά ρυθμίσεων"

#define D_PROGRAM_VERSION "Εκδοση προγράμματος"
#define D_BUILD_DATE_AND_TIME "Ημερομηνία και Ώρα Build"
#define D_CORE_AND_SDK_VERSION "Core/SDK Version"
#define D_FLASH_WRITE_COUNT "Μετρητής εγγραφών Flash"
#define D_MAC_ADDRESS "MAC Address"
#define D_MQTT_HOST "MQTT Host"
#define D_MQTT_PORT "MQTT Port"
#define D_MQTT_CLIENT "MQTT Client"
#define D_MQTT_USER "MQTT User"
#define D_MQTT_TOPIC "MQTT Topic"
#define D_MQTT_GROUP_TOPIC "MQTT Group Topic"
#define D_MQTT_FULL_TOPIC "MQTT Full Topic"
#define D_MDNS_DISCOVERY "mDNS Discovery"
#define D_MDNS_ADVERTISE "mDNS Advertise"
#define D_ESP_CHIP_ID "ESP Chip Id"
#define D_FLASH_CHIP_ID "Flash Chip Id"
#define D_FLASH_CHIP_SIZE "Μέγεθος Flash"
#define D_FREE_PROGRAM_SPACE "Ελεύθερος Χώρος Προγράμματος"

#define D_UPGRADE_BY_WEBSERVER "αναβάθμιση από τον διακομιστή web"
#define D_OTA_URL "OTA Url"
#define D_START_UPGRADE "Εκκίνηση αναβάθμισης"
#define D_UPGRADE_BY_FILE_UPLOAD "Αναβάθμιση με ανέβασμα αρχείου"
#define D_UPLOAD_STARTED "Το ανέβασμα ξεκίνησε"
#define D_UPGRADE_STARTED "Η αναβάθμιση ξεκίνησε"
#define D_UPLOAD_DONE "Το ανέβασμα ολοκληρώθηκε"
#define D_UPLOAD_ERR_1 "Δεν έχει επιλεγεί αρχείο"
#define D_UPLOAD_ERR_2 "Δεν υπάρχει αρκετός χώρος"
#define D_UPLOAD_ERR_3 "Magic byte δεν είναι 0xE9"
#define D_UPLOAD_ERR_4 "Το μέγεθος του προγράμματος flash είναι μεγαλύτερο από το πραγματικό μέγεθος του flash"
#define D_UPLOAD_ERR_5 "Upload buffer miscompare"
#define D_UPLOAD_ERR_6 "Το ανέβασμα απέτυχε. Επιλέξτε επίπεδο καταγραφής 3"
#define D_UPLOAD_ERR_7 "Το ανεβάσμα διακόπηκε"
#define D_UPLOAD_ERR_8 "Μη έγκυρο αρχείο"
#define D_UPLOAD_ERR_9 "Το αρχείο είναι πολύ μεγάλο"
#define D_UPLOAD_ERR_10 "Failed to init RF chip"
#define D_UPLOAD_ERR_11 "Failed to erase RF chip"
#define D_UPLOAD_ERR_12 "Failed to write to RF chip"
#define D_UPLOAD_ERR_13 "Failed to decode RF firmware"
#define D_UPLOAD_ERROR_CODE "Κωδικός λάθους ανεβάσματος"

#define D_ENTER_COMMAND "Εισαγωγή εντολής"
#define D_ENABLE_WEBLOG_FOR_RESPONSE "Ενεργοποιήστε το weblog 2 αν περιμένετε απάντηση"
#define D_NEED_USER_AND_PASSWORD "Χρειάζεστε user=<όνομα χρήστη>&password=<κωδικό χρήστη>"

// xdrv_01_mqtt.ino
#define D_FINGERPRINT "Επαλήθευση TLS fingerprint..."
#define D_TLS_CONNECT_FAILED_TO "Αποτυχία TLS σύνδεσης"
#define D_RETRY_IN "Επανάληψη σε"
#define D_VERIFIED "Επαλήθευση με Fingerprint"
#define D_INSECURE "Μη ασφαλής σύνδεση λόγο ακατάλληλου Fingerprint"
#define D_CONNECT_FAILED_TO "Αποτυχία σύνδεσης στο"

// xplg_wemohue.ino
#define D_MULTICAST_DISABLED "Multicast απενεργοποιημένο"
#define D_MULTICAST_REJOINED "Multicast επανασύνδεση"
#define D_MULTICAST_JOIN_FAILED "Αποτυχία σύνδεσης Multicast"
#define D_FAILED_TO_SEND_RESPONSE "Αποτυχία αποστολής απάντησης"

#define D_WEMO "WeMo"
#define D_WEMO_BASIC_EVENT "WeMo βασικό event"
#define D_WEMO_EVENT_SERVICE "WeMo υπηρεσία event"
#define D_WEMO_META_SERVICE "WeMo υπηρεσία meta"
#define D_WEMO_SETUP "WeMo ρυθμίσεις"
#define D_RESPONSE_SENT "Αποστολή απάντησης"

#define D_HUE "Hue"
#define D_HUE_BRIDGE_SETUP "Hue ρυθμίσεις"
#define D_HUE_API_NOT_IMPLEMENTED "Hue API δεν έχει υλοποιηθεί"
#define D_HUE_API "Hue API"
#define D_HUE_POST_ARGS "Hue POST args"
#define D_3_RESPONSE_PACKETS_SENT "3πλο πακέτο απάντησεης στάλθηκε"

// xdrv_07_domoticz.ino
#define D_DOMOTICZ_PARAMETERS "Domoticz παράμετροι"
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
  #define D_DOMOTICZ_AIRQUALITY "Ποιότητα Αέρα"
#define D_DOMOTICZ_UPDATE_TIMER "Ενημέρωση"

// xdrv_09_timers.ino
#define D_CONFIGURE_TIMER "Ρυθμίσεις Χρόνου"
#define D_TIMER_PARAMETERS "Χρονικοί παράμετροι"
#define D_TIMER_ARM "Arm"
#define D_TIMER_TIME "Ωρα"
#define D_TIMER_DAYS "Μέρες"
#define D_TIMER_REPEAT "Επανάληψη"
#define D_TIMER_OUTPUT "Αποτέλεσμα"
#define D_TIMER_ACTION "Εντολή"

// xdrv_10_knx.ino
#define D_CONFIGURE_KNX "Ρυθμίσεις KNX"
#define D_KNX_PARAMETERS "KNX Παράμετροι"
#define D_KNX_GENERAL_CONFIG "Γενικά"
#define D_KNX_PHYSICAL_ADDRESS "Διεύθυνση Μνήμης"
#define D_KNX_PHYSICAL_ADDRESS_NOTE "( Πρέπει να είναι μοναδική στο KNX δίκτυο )"
#define D_KNX_ENABLE "Ενεργοποιημένο KNX"
#define D_KNX_GROUP_ADDRESS_TO_WRITE "Αποστολή δεδομένων σε Ομάδα Διευθύνσεων"
#define D_ADD "Προσθήκη"
#define D_DELETE "Διαγραφή"
#define D_REPLY "Επανάληψη"
#define D_KNX_GROUP_ADDRESS_TO_READ "Ομάδα Διευθύνσεων που θα λάβει τα Δεδομένα"
#define D_LOG_KNX "KNX: "
#define D_RECEIVED_FROM "Στάλθηκε από"
#define D_KNX_COMMAND_WRITE "Εγγραφή"
#define D_KNX_COMMAND_READ "Ανάγνωση"
#define D_KNX_COMMAND_OTHER "Αλλο"
#define D_SENT_TO "αποστολή σε"
#define D_KNX_WARNING "Η Ομάδα Διευθύνσεων ( 0 / 0 / 0 ) είναι δεσμευμένη και δεν μπορεί να χρησιμοποιηθεί."
#define D_KNX_ENHANCEMENT "Communication Enhancement"
#define D_KNX_TX_SLOT "KNX TX"
#define D_KNX_RX_SLOT "KNX RX"

// xdrv_03_energy.ino
#define D_ENERGY_TODAY "Energy Σήμερα"
#define D_ENERGY_YESTERDAY "Energy Χτες"
#define D_ENERGY_TOTAL "Energy Συνολικά"

// xsns_05_ds18b20.ino
#define D_SENSOR_BUSY "Απασχολημένος Σένσορας"
#define D_SENSOR_CRC_ERROR "CRC λάθος σένσορα"
#define D_SENSORS_FOUND "Βρέθηκε σένσορας"

// xsns_06_dht.ino
#define D_TIMEOUT_WAITING_FOR "Timeout περιμένετε για"
#define D_START_SIGNAL_LOW "εκκίνηση με χαμηλό σήμα"
#define D_START_SIGNAL_HIGH "εκκίνηση με υψηλό σήμα"
#define D_PULSE "παλμός"
#define D_CHECKSUM_FAILURE "Αποτυχία Checksum"

// xsns_07_sht1x.ino
#define D_SENSOR_DID_NOT_ACK_COMMAND "Ο σένσορας δεν έλαβε την εντολή ACK"
#define D_SHT1X_FOUND "Βρέθηκε SHT1X"

// xsns_18_pms5003.ino
#define D_STANDARD_CONCENTRATION "CF-1 PM"     // Standard Particle CF-1 Particle Matter
#define D_ENVIRONMENTAL_CONCENTRATION "PM"     // Environmetal Particle Matter
#define D_PARTICALS_BEYOND "Particals"

// sonoff_template.h
#define D_SENSOR_NONE     "Κανένας"
#define D_SENSOR_DHT11    "DHT11"
#define D_SENSOR_AM2301   "AM2301"
#define D_SENSOR_SI7021   "SI7021"
#define D_SENSOR_DS18X20  "DS18x20"
#define D_SENSOR_I2C_SCL  "I2C SCL"
#define D_SENSOR_I2C_SDA  "I2C SDA"
#define D_SENSOR_WS2812   "WS2812"
#define D_SENSOR_IRSEND   "IRsend"
#define D_SENSOR_SWITCH   "Διακόπτης"   // Suffix "1"
#define D_SENSOR_BUTTON   "Κουμπί"   // Suffix "1"
#define D_SENSOR_RELAY    "Ρελέ"    // Suffix "1i"
#define D_SENSOR_LED      "Led"      // Suffix "1i"
#define D_SENSOR_PWM      "PWM"      // Suffix "1"
#define D_SENSOR_COUNTER  "Μετρητής"  // Suffix "1"
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
#define D_SENSOR_SBR_RX   "SerBr Rx"
#define D_SENSOR_SBR_TX   "SerBr Tx"
#define D_SENSOR_SR04_TRIG "SR04 Tri"
#define D_SENSOR_SR04_ECHO "SR04 Ech"
#define D_SENSOR_SDM120_TX "SDM120 Tx"
#define D_SENSOR_SDM120_RX "SDM120 Rx"
#define D_SENSOR_SDM630_TX "SDM630 Tx"
#define D_SENSOR_SDM630_RX "SDM630 Rx"
#define D_SENSOR_TM1638_CLK "TM16 CLK"
#define D_SENSOR_TM1638_DIO "TM16 DIO"
#define D_SENSOR_TM1638_STB "TM16 STB"

// Units
#define D_UNIT_AMPERE "A"
#define D_UNIT_CENTIMETER "cm"
#define D_UNIT_HERTZ "Hz"
#define D_UNIT_HOUR "Hr"
#define D_UNIT_KILOOHM "kOhm"
#define D_UNIT_KILOWATTHOUR "kWh"
#define D_UNIT_LUX "lx"
#define D_UNIT_MICROGRAM_PER_CUBIC_METER "ug/m3"
#define D_UNIT_MICROMETER "um"
#define D_UNIT_MICROSECOND "us"
#define D_UNIT_MILLIAMPERE "mA"
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

#endif  // _LANGUAGE_EN_GB_H_
