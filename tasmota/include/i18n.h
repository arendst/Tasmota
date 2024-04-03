/*
  i18n.h - internationalization for Tasmota

  Copyright (C) 2021  Theo Arends

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

#ifndef _I18N_H_
#define _I18N_H_

/*********************************************************************************************\
 * All text used in Commands, MQTT, JSON messages only in English (#1473)
 *
 * (At this location they can/may be overruled by language files using undefs)
\*********************************************************************************************/

#define D_JSON_ABORTED "Aborted"
#define D_JSON_ACK "Ack"
#define D_JSON_ACTIVE "Active"
#define D_JSON_ADDRESS "Address"
#define D_JSON_AHUM "aHumidity"
#define D_JSON_AIRQUALITY "AirQuality"
#define D_JSON_ANALOG_INPUT "Analog"
#define D_JSON_AP "AP"                   // Access Point
#define D_JSON_APMAC_ADDRESS "APMac"
#define D_JSON_APPENDED "Appended"
#define D_JSON_BAUDRATE "Baudrate"
#define D_JSON_BLINK "Blink"
#define D_JSON_BLOCKED_LOOP "Blocked Loop"
#define D_JSON_BLUE "Blue"
#define D_JSON_BOOTVERSION "Boot"
#define D_JSON_BOOTCOUNT "BootCount"
#define D_JSON_BSSID "BSSId"
#define D_JSON_BUTTON "Button"
#define D_JSON_BUILDDATETIME "BuildDateTime"
#define D_JSON_CHANNEL "Channel"
#define D_JSON_CO2 "CarbonDioxide"
#define D_JSON_COMMAND "Command"
#define D_JSON_CONFIDENCE "Confidence"
#define D_JSON_CONFIG_HOLDER "CfgHolder"
#define D_JSON_CONNECT_FAILED "Connect failed"
#define D_JSON_CONNECTION_LOST "Connection lost"
#define D_JSON_COREVERSION "Core"
#define D_JSON_COUNT "Count"
#define D_JSON_COUNTER "Counter"
#define D_JSON_CURRENT "Current"         // As in Voltage and Current
#define D_JSON_CURRENT_NEUTRAL "CurrentNeutral"
#define D_JSON_DARKNESS "Darkness"
#define D_JSON_DATA "Data"
#define D_JSON_DEWPOINT "DewPoint"
#define D_JSON_DO "DisolvedOxygen"
#define D_JSON_DISTANCE "Distance"
#define D_JSON_DNSSERVER "DNSServer"
#define D_JSON_DONE "Done"
#define D_JSON_DOWNTIME "Downtime"
#define D_JSON_EC "EC"
#define D_JSON_ECO2 "eCO2"
#define D_JSON_EMPTY "Empty"
#define D_JSON_ENDDST "EndDST"           // End Daylight Savings Time
#define D_JSON_ENERGY "Energy"
#define D_JSON_ERASE "Erase"
#define D_JSON_ERROR "Error"
#define D_JSON_EVENT "Event"
#define D_JSON_EVERY "Every"
#define D_JSON_EXPORT "Export"
#define D_JSON_EXPORT_ACTIVE "ExportActive"
#define D_JSON_EXPORT_REACTIVE "ExportReactive"
#define D_JSON_EXPORT_POWER "ExportPower"
#define D_JSON_FAILED "Failed"
#define D_JSON_FALLBACKTOPIC "FallbackTopic"
#define D_JSON_FEATURES "Features"
#define D_JSON_FLASHCHIPID "FlashChipId"
#define D_JSON_FLASHMODE "FlashMode"
#define D_JSON_FLASHSIZE "FlashSize"
#define D_JSON_FLOWRATE "FlowRate"
#define D_JSON_FREEMEMORY "Free"
#define D_JSON_FREQUENCY "Frequency"
#define D_JSON_FROM "from"
#define D_JSON_GAS "Gas"
#define D_JSON_GATEWAY "Gateway"
#define D_JSON_GREEN "Green"
#define D_JSON_GROUPS "Groups"
#define D_JSON_HALLEFFECT "HallEffect"
#define D_JSON_HALTING "Halting"
#define D_JSON_HEAPSIZE "Heap"
#define D_JSON_HIGH "High"
#define D_JSON_HOST_NOT_FOUND "Host not found"
#define D_JSON_FILE_NOT_FOUND "File not found"
#define D_JSON_OTHER_HTTP_ERROR "Other http error"
#define D_JSON_HSBCOLOR "HSBColor"
#define D_JSON_HUMIDITY "Humidity"
#define D_JSON_ID "Id"
#define D_JSON_ILLUMINANCE "Illuminance"
#define D_JSON_IMPORT_ACTIVE "ImportActive"
#define D_JSON_IMPORT_POWER "ImportPower"
#define D_JSON_IMPORT_REACTIVE "ImportReactive"
#define D_JSON_INFRARED "Infrared"
#define D_JSON_INVALID_FILE_TYPE "Invalid filetype or buffer"
#define D_JSON_IP6_GLOBAL "IP6Global"
#define D_JSON_IP6_LOCAL "IP6Local"
#define D_JSON_UNKNOWN "Unknown"
#define D_JSON_LIGHT "Light"
#define D_JSON_LINK_COUNT "LinkCount"
#define D_JSON_LOCAL_TIME "Local"
#define D_JSON_LOW "Low"
#define D_JSON_MAC "Mac"
#define D_JSON_MASK "Mask"
#define D_JSON_MD5_MISMATCH "MD5 mismatch"
#define D_JSON_MEMORY_ERROR "Memory error"
#define D_JSON_MINIMAL "minimal"
#define D_JSON_MODEL "Model"
#define D_JSON_MOISTURE "Moisture"
#define D_JSON_MQTT_COUNT "MqttCount"
#define D_JSON_NULL "null"
#define D_JSON_NO "No"
#define D_JSON_NOISE "Noise"
#define D_JSON_NONE "None"
#define D_JSON_OR "or"
#define D_JSON_ORP "ORP"
#define D_JSON_O2 "Oxygen"
#define D_JSON_PERIOD "Period"
#define D_JSON_PH "pH"
#define D_JSON_PHASE_ANGLE "PhaseAngle"
#define D_JSON_POWERFACTOR "Factor"
#define D_JSON_POWERMAX "MaxPower"
#define D_JSON_POWERUSAGE "Power"
#define D_JSON_ACTIVE_POWERUSAGE "ActivePower"
#define D_JSON_APPARENT_POWERUSAGE "ApparentPower"
#define D_JSON_REACTIVE_POWERUSAGE "ReactivePower"
#define D_JSON_RANGE "Range"
#define D_JSON_PRESSURE "Pressure"
#define D_JSON_PRESSUREATSEALEVEL "SeaPressure"
#define D_JSON_PRESSURE_UNIT "PressureUnit"
#define D_JSON_PROBETEMPERATURE "ProbeTemperature"
#define D_JSON_PROGRAMFLASHSIZE "ProgramFlashSize"
#define D_JSON_PROGRAMSIZE "ProgramSize"
#define D_JSON_STACKLOWMARK "StackLowMark"
#define D_JSON_PSRMAXMEMORY "PsrMax"
#define D_JSON_PSRFREEMEMORY "PsrFree"
#define D_JSON_PUMP "Pumped"
#define D_JSON_RED "Red"
#define D_JSON_REFERENCETEMPERATURE "ReferenceTemperature"
#define D_JSON_REMAINING "Remaining"
#define D_JSON_RESET "Reset"
#define D_JSON_RESISTANCE "Resistance"
#define D_JSON_RESOLUTION "Resolution"
#define D_JSON_RESTARTING "Restarting"
#define D_JSON_RESTARTREASON "RestartReason"
#define D_JSON_RSSI "RSSI"
#define D_JSON_RUNTIME "Runtime"
#define D_JSON_SAVEADDRESS "SaveAddress"
#define D_JSON_SAVECOUNT "SaveCount"
#define D_JSON_SAVED "Saved"
#define D_JSON_SAVESTATE "SaveState"
#define D_JSON_SDKVERSION "SDK"
#define D_JSON_SELECTED "selected"
#define D_JSON_SERIALRECEIVED "SerialReceived"
#define D_JSON_SET "Set"
#define D_JSON_SIGNAL "Signal"
#define D_JSON_SIZE "Size"
#define D_JSON_SPEED "Speed"
#define D_JSON_SPEED_UNIT "SpeedUnit"
#define D_JSON_SSID "SSId"
#define D_JSON_STAGE "Stage"
#define D_JSON_STARTDST "StartDST"       // Start Daylight Savings Time
#define D_JSON_STARTED "Started"
#define D_JSON_STARTUPUTC "StartupUTC"
#define D_JSON_STATUS "Status"
#define D_JSON_SUBNETMASK "Subnetmask"
#define D_JSON_SUCCESSFUL "Successful"
#define D_JSON_SUNRISE "Sunrise"
#define D_JSON_SUNSET "Sunset"
#define D_JSON_SWITCH "Switch"
#define D_JSON_SYNC "Sync"
#define D_JSON_TEMPERATURE "Temperature"
#define D_JSON_TEMPERATURE_UNIT "TempUnit"
#define D_JSON_TIME "Time"
#define D_JSON_TODAY "Today"
#define D_JSON_TODAY_SUM_EXPORT "TodaySumExport"
#define D_JSON_TODAY_SUM_IMPORT "TodaySumImport"
#define D_JSON_TOTAL "Total"
#define D_JSON_TOTAL_USAGE "TotalUsage"
#define D_JSON_TOTAL_REACTIVE "TotalReactive"
#define D_JSON_TOTAL_START_TIME "TotalStartTime"
#define D_JSON_TVOC "TVOC"
#define D_JSON_TYPE "Type"
#define D_JSON_UID "UID"
#define D_JSON_UPTIME "Uptime"
#define D_JSON_UTC_TIME "UTC"
#define D_JSON_UV_INDEX "UvIndex"
#define D_JSON_UV_INDEX_TEXT "UvIndexText"
#define D_JSON_UV_LEVEL "UvLevel"
#define D_JSON_UV_POWER "UvPower"
#define D_JSON_VCC "Vcc"
#define D_JSON_VERSION "Version"
#define D_JSON_VOLTAGE "Voltage"
#define D_JSON_VOLUME "Volume"
#define D_JSON_WEIGHT "Weight"
#define D_JSON_WIFI "Wifi"
#define D_JSON_WIFI_MODE "Mode"
#define D_JSON_NOT_STARTED "Not Started"
#define D_JSON_SCANNING "Scanning"
#define D_JSON_TESTING "Testing"
#define D_JSON_BUSY "Busy"
#define D_JSON_ENCRYPTION "Encryption"
#define D_JSON_WRONG "Wrong"
#define D_JSON_WRONG_PARAMETERS "Wrong parameters"
#define D_JSON_YESTERDAY "Yesterday"
#define D_JSON_ZERO_POINT_CALIBRATION "Zero Point Calibration"
#define D_JSON_PV1_VOLTAGE "Pv1Voltage"
#define D_JSON_PV1_CURRENT "Pv1Current"
#define D_JSON_PV1_POWER "Pv1Power"
#define D_JSON_PV2_VOLTAGE "Pv2Voltage"
#define D_JSON_PV2_CURRENT "Pv2Current"
#define D_JSON_PV2_POWER "Pv2Power"
#define D_JSON_SOLAR_POWER "SolarPower"
#define D_JSON_USAGE "Usage"
#define D_JSON_IMPORT "Import"
#define D_JSON_EXPORT "Export"
#define D_JSON_TOTAL_ACTIVE "TotalActive"
#define D_JSON_RESETTABLE_TOTAL_ACTIVE "ResetTotalActive"
#define D_JSON_SIGNALSTRENGTH "SignalStrength"
#define D_JSON_CHIPTEMPERATURE "ChipTemperature"
#define D_JSON_RAW "Raw"
#define D_JSON_MX "XaxisInduction"
#define D_JSON_MY "YaxisInduction"
#define D_JSON_MZ "ZaxisInduction"
#define D_JSON_MAGNETICFLD "MagneticInduction"
#define D_JSON_ALERT "Alert"
#define D_JSON_BATTPERCENT "BatteryPercentage"
#define D_RSLT_ENERGY "ENERGY"
#define D_RSLT_HASS_STATE "HASS_STATE"
#define D_RSLT_INFO "INFO"
#define D_RSLT_MARGINS "MARGINS"
#define D_RSLT_POWER "POWER"
#define D_RSLT_RESULT "RESULT"
#define D_RSLT_SENSOR "SENSOR"
#define D_RSLT_STATE "STATE"
#define D_RSLT_UPTIME "UPTIME"
#define D_RSLT_WARNING "WARNING"
#define D_RSLT_COMMAND "COMMAND"

#define D_LOG_SOME_SETTINGS_RESET "Some settings have been reset"

// Commands tasmota.ino
#define D_CMND_BACKLOG "Backlog"
#define D_CMND_JSON "Json"
#define D_CMND_DELAY "Delay"
#define D_CMND_NODELAY "NoDelay"
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
  #define D_STATUS12_STATUS "STK"
  #define D_STATUS13_SHUTTER "SHT"
#define D_CMND_STATE "State"
#define D_CMND_POWER "Power"
#define D_CMND_TIMEDPOWER "TimedPower"
#define D_CMND_FANSPEED "FanSpeed"
#define D_CMND_POWERONSTATE "PowerOnState"
#define D_CMND_PULSETIME "PulseTime"
#define D_CMND_BLINKTIME "BlinkTime"
#define D_CMND_BLINKCOUNT "BlinkCount"
#define D_CMND_SETSENSOR "SetSensor"
#define D_CMND_SENSOR "Sensor"
#define D_CMND_DRIVER "Driver"
#define D_CMND_SAVEDATA "SaveData"
#define D_CMND_SETOPTION "SetOption"
#define D_CMND_SO "SO"
#define D_CMND_TEMPERATURE_RESOLUTION "TempRes"
#define D_CMND_HUMIDITY_RESOLUTION "HumRes"
#define D_CMND_PRESSURE_RESOLUTION "PressRes"
#define D_CMND_POWER_RESOLUTION "WattRes"
#define D_CMND_VOLTAGE_RESOLUTION "VoltRes"
#define D_CMND_FREQUENCY_RESOLUTION "FreqRes"
#define D_CMND_CURRENT_RESOLUTION "AmpRes"
#define D_CMND_ENERGY_RESOLUTION "EnergyRes"
#define D_CMND_WEIGHT_RESOLUTION "WeightRes"
#define D_CMND_MODULE "Module"
#define D_CMND_MODULES "Modules"
#define D_CMND_ADC "ADC"
#define D_CMND_ADCS "ADCs"
#define D_CMND_GPIO "GPIO"
  #define D_JSON_NOT_SUPPORTED "Not supported"
#define D_CMND_GPIOS "GPIOs"
#define D_CMND_GPIOREAD "GPIORead"
#define D_CMND_PWM "PWM"
#define D_CMND_PWMFREQUENCY "PWMFrequency"
#define D_CMND_PWMRANGE "PWMRange"
#define D_CMND_BUTTONDEBOUNCE "ButtonDebounce"
#define D_CMND_SWITCHDEBOUNCE "SwitchDebounce"
#define D_CMND_SWITCHTEXT "SwitchText"
#define D_CMND_SLEEP "Sleep"
#define D_CMND_UPLOAD "Upload"
#define D_CMND_UPGRADE "Upgrade"
  #define D_JSON_ONE_OR_GT "1 or >%s to upgrade"
#define D_CMND_OTAURL "OtaUrl"
#define D_CMND_SERIALLOG "SerialLog"
#define D_CMND_SYSLOG "SysLog"
#define D_CMND_LOGHOST "LogHost"
#define D_CMND_LOGPORT "LogPort"
#define D_CMND_IPADDRESS "IPAddress"
#define D_CMND_NTPSERVER "NtpServer"
#define D_CMND_AP "Ap"
#define D_CMND_SSID "SSId"
#define D_CMND_PASSWORD "Password"
#define D_CMND_HOSTNAME "Hostname"
#define D_CMND_WIFI "Wifi"
#define D_CMND_WIFISCAN "WiFiScan"
#define D_CMND_WIFITEST "WiFiTest"
#define D_CMND_ETHERNET "Ethernet"
#define D_CMND_WIFICONFIG "WifiConfig"
  #define D_WCFG_0_RESTART "Restart"
  #define D_WCFG_2_WIFIMANAGER "WifiManager"
  #define D_WCFG_4_RETRY "Retry"
  #define D_WCFG_5_WAIT "Wait"
  #define D_WCFG_6_SERIAL "Serial"
  #define D_WCFG_7_WIFIMANAGER_RESET_ONLY "ManagerRst"
#define D_CMND_DNSTIMEOUT "DnsTimeout"
#define D_CMND_DEVICENAME "DeviceName"
#define D_CMND_FRIENDLYNAME "FriendlyName"
#define D_CMND_FN "FN"
#define D_CMND_SWITCHMODE "SwitchMode"
#define D_CMND_INTERLOCK "Interlock"
#define D_CMND_TELEPERIOD "TelePeriod"
#define D_CMND_RESTART "Restart"
  #define D_JSON_ONE_TO_RESTART "1 to restart, 2 to halt"
#define D_CMND_RESET "Reset"
  #define D_JSON_RESET_AND_RESTARTING "Reset and Restarting"
  #define D_JSON_ONE_TO_RESET "1 to reset"
#define D_CMND_TIME "Time"
#define D_CMND_TIMEZONE "Timezone"
#define D_CMND_TIMESTD "TimeStd"
#define D_CMND_TIMEDST "TimeDst"
#define D_CMND_ALTITUDE "Altitude"
#define D_CMND_LEDPOWER "LedPower"
#define D_CMND_LEDSTATE "LedState"
#define D_CMND_LEDMASK "LedMask"
#define D_CMND_LEDPWM_OFF "LedPwmOff"
#define D_CMND_LEDPWM_ON "LedPwmOn"
#define D_CMND_LEDPWM_MODE "LedPwmMode"
#define D_CMND_WIFIPOWER "WifiPower"
#define D_CMND_SPEEDUNIT "SpeedUnit"
#define D_CMND_I2CSCAN "I2CScan"
#define D_CMND_I2CDRIVER "I2CDriver"
#define D_CMND_DEVGROUP_NAME "DevGroupName"
#define D_CMND_DEVGROUP_SEND "DevGroupSend"
#define D_CMND_DEVGROUP_SHARE "DevGroupShare"
#define D_CMND_DEVGROUPSTATUS "DevGroupStatus"
#define D_CMND_DEVGROUP_TIE "DevGroupTie"
#define D_CMND_SERIALBUFFER "SerialBuffer"
#define D_CMND_SERIALSEND "SerialSend"
#define D_CMND_SERIALDELIMITER "SerialDelimiter"
#define D_CMND_BAUDRATE "Baudrate"
#define D_CMND_SERIALCONFIG "SerialConfig"
#define D_CMND_TEMPLATE "Template"
  #define D_JSON_NAME "NAME"
  #define D_JSON_GPIO "GPIO"
  #define D_JSON_FLAG "FLAG"
  #define D_JSON_BASE "BASE"
  #define D_JSON_CMND "CMND"
#define D_CMND_TEMPOFFSET "TempOffset"
#define D_CMND_HUMOFFSET "HumOffset"
#define D_CMND_GLOBAL_TEMP "GlobalTemp"
#define D_CMND_GLOBAL_HUM "GlobalHum"
#define D_CMND_GLOBAL_PRESS "GlobalPress"

#define D_SO_WIFINOSLEEP "WifiNoSleep"

#ifdef ESP32
#define D_CMND_TOUCH_CAL "TouchCal"
#define D_CMND_TOUCH_THRES "TouchThres"
#define D_CMND_TOUCH_NUM "TouchNum"
#define D_CMND_CPU_FREQUENCY "CpuFrequency"
#endif  // ESP32

// Commands xdrv_02_mqtt.ino
#define D_SO_MQTTJSONONLY "MqttJSONOnly"
#define D_SO_MQTTTLS "MqttTLS"
#define D_SO_MQTTTLS_FINGERPRINT "MqttTLSFingerprint"
#define D_SO_MQTTNORETAIN "MqttNoRetain"
#define D_SO_MQTTDETACHRELAY "MqttDetachRelay"
#define D_CMND_MQTTLOG "MqttLog"
#define D_CMND_MQTTHOST "MqttHost"
#define D_CMND_MQTTPORT "MqttPort"
#define D_CMND_MQTTRETRY "MqttRetry"
#define D_CMND_STATETEXT "StateText"
#define D_CMND_MQTTFINGERPRINT "MqttFingerprint"
#define D_CMND_MQTTCLIENT "MqttClient"
#define D_CMND_MQTTUSER "MqttUser"
#define D_CMND_MQTTPASSWORD "MqttPassword"
#define D_CMND_MQTTKEEPALIVE "MqttKeepAlive"
#define D_CMND_MQTTTIMEOUT "MqttTimeout"
#define D_CMND_MQTTWIFITIMEOUT "MqttWifiTimeout"
#define D_CMND_TLSKEY "TLSKey"
#define D_CMND_FULLTOPIC "FullTopic"
#define D_CMND_PREFIX "Prefix"
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
#define D_CMND_INFORETAIN "InfoRetain"
#define D_CMND_STATERETAIN "StateRetain"
#define D_CMND_STATUSRETAIN "StatusRetain"
#define D_CMND_PUBLISH "Publish"
#define D_CMND_FILEUPLOAD "FileUpload"
#define D_CMND_FILEDOWNLOAD "FileDownload"

// Commands xdrv_01_webserver.ino
#define D_CMND_WEBSERVER "Webserver"
  #define D_JSON_WEBSERVER_MODE "WebServerMode"
  #define D_JSON_ACTIVE_FOR "Active for"
  #define D_JSON_ON_DEVICE "on"
  #define D_JSON_WITH_IP_ADDRESS "with IP address"
#define D_CMND_WEBPASSWORD "WebPassword"
#define D_CMND_WEBLOG "WebLog"
#define D_CMND_WEBREFRESH "WebRefresh"
#define D_CMND_WEBSEND "WebSend"
#define D_CMND_WEBQUERY "WebQuery"
#define D_CMND_WEBCOLOR "WebColor"
#define D_CMND_WEBBUTTON "WebButton"
#define D_CMND_WEBCANVAS "WebCanvas"
#define D_CMND_WEBTIME "WebTime"
#define D_CMND_WEBSENSOR "WebSensor"
#define D_CMND_WEBGETCONFIG "WebGetConfig"
#define D_CMND_EMULATION "Emulation"
#define D_CMND_SENDMAIL "Sendmail"
#define D_CMND_CORS "CORS"

// Commands xdrv_03_energy.ino
#define D_CMND_POWERLOW "PowerLow"
#define D_CMND_POWERHIGH "PowerHigh"
#define D_CMND_POWERDELTA "PowerDelta"
#define D_CMND_VOLTAGELOW "VoltageLow"
#define D_CMND_VOLTAGEHIGH "VoltageHigh"
#define D_CMND_CURRENTLOW "CurrentLow"
#define D_CMND_CURRENTHIGH "CurrentHigh"
#define D_CMND_ENERGYDISPLAY "EnergyDisplay"
#define D_CMND_ENERGYCOLS "EnergyCols"
#define D_CMND_ENERGYTODAY "EnergyToday"
#define D_CMND_ENERGYYESTERDAY "EnergyYesterday"
#define D_CMND_ENERGYTOTAL "EnergyTotal"
#define D_CMND_ENERGYUSAGE "EnergyUsage"
#define D_CMND_ENERGYEXPORT "EnergyExport"
#define D_CMND_ENERGYEXPORTACTIVE "EnergyExportActive"
#define D_CMND_POWERSET "PowerSet"
#define D_CMND_VOLTAGESET "VoltageSet"
#define D_CMND_CURRENTSET "CurrentSet"
#define D_CMND_FREQUENCYSET "FrequencySet"
#define D_CMND_ENERGYCONFIG "EnergyConfig"
#define D_CMND_MAXPOWER "MaxPower"
#define D_CMND_MAXPOWERHOLD "MaxPowerHold"
#define D_CMND_MAXPOWERWINDOW "MaxPowerWindow"
  #define D_JSON_MAXPOWERREACHED "MaxPowerReached"
  #define D_JSON_MAXPOWERREACHEDRETRY "MaxPowerReachedRetry"
#define D_CMND_SAFEPOWER "SafePower"
#define D_CMND_SAFEPOWERHOLD "SafePowerHold"
#define D_CMND_SAFEPOWERWINDOW "SafePowerWindow"
  #define D_JSON_POWERMONITOR "PowerMonitor"
#define D_CMND_MAXENERGY "MaxEnergy"
#define D_CMND_MAXENERGYSTART "MaxEnergyStart"
  #define D_JSON_ENERGYMONITOR "EnergyMonitor"
  #define D_JSON_MAXENERGYREACHED "MaxEnergyReached"

// Commands xdrv_04_light.ino
#define D_SO_CHANNELREMAP "ChannelRemap"    // SO37
#define D_SO_MULTIPWM "MultiPWM"            // SO68
#define D_SO_ALEXACTRANGE "AlexaCTRange"    // SO82
#define D_SO_POWERONFADE "PowerOnFade"      // SO91
#define D_SO_PWMCT "PWMCT"                  // SO92
#define D_SO_WHITEBLEND "WhiteBlend"        // SO105
#define D_CMND_CHANNEL "Channel"
#define D_CMND_COLOR "Color"
#define D_CMND_COLORTEMPERATURE "CT"
#define D_CMND_DIMMER "Dimmer"
#define D_CMND_DIMMER_RANGE "DimmerRange"
#define D_CMND_DIMMER_STEP "DimmerStep"
#define D_CMND_HSBCOLOR "HSBColor"
#define D_CMND_VIRTUALCT "VirtualCT"
#define D_CMND_CTRANGE "CTRange"
#define D_CMND_LED "Led"
#define D_CMND_LEDTABLE "LedTable"
#define D_CMND_FADE "Fade"
#define D_CMND_PALETTE "Palette"
#define D_CMND_PIXELS "Pixels"
#define D_CMND_STEPPIXELS "StepPixels"
#define D_CMND_ARTNET "ArtNet"
#define D_CMND_ARTNET_CONFIG "ArtNetConfig"
#define D_SO_ARTNET_AUTORUN "ArtNetAutorun"
#define D_CMND_RGBWWTABLE "RGBWWTable"
#define D_CMND_ROTATION "Rotation"
#define D_CMND_SCHEME "Scheme"
#define D_CMND_SEQUENCE_OFFSET "SequenceOffset"
#define D_CMND_SPEED "Speed"
#define D_CMND_WAKEUP "Wakeup"
#define D_CMND_WAKEUPDURATION "WakeUpDuration"
#define D_CMND_WHITE "White"
#define D_CMND_WIDTH "Width"

// Commands xdrv_05_irremote.ino
#define D_CMND_IRSEND "IRSend"
  #define D_JSON_INVALID_JSON "Invalid JSON"
  #define D_JSON_INVALID_RAWDATA "Invalid RawData"
  #define D_JSON_NO_BUFFER_SPACE "No buffer space"
  #define D_JSON_PROTOCOL_NOT_SUPPORTED "Protocol not supported"
  #define D_JSON_IR_PROTOCOL "Protocol"
  #define D_JSON_IR_BITS "Bits"
  #define D_JSON_IR_DATA "Data"
  #define D_JSON_IR_DATALSB "DataLSB"
  #define D_JSON_IR_HASH "Hash"
  #define D_JSON_IR_RAWDATA "RawData"
  #define D_JSON_IR_REPEAT "Repeat"
  #define D_JSON_IR_CHANNEL "Channel"
#define D_CMND_IRHVAC "IRHVAC"
  #define D_JSON_IRHVAC_VENDOR "Vendor"
  #define D_JSON_IRHVAC_PROTOCOL "Protocol"
  #define D_JSON_IRHVAC_MODEL "Model"
  #define D_JSON_IRHVAC_COMMAND "Command"
  #define D_JSON_IRHVAC_POWER "Power"
  #define D_JSON_IRHVAC_MODE "Mode"
  #define D_JSON_IRHVAC_FANSPEED "FanSpeed"
  #define D_JSON_IRHVAC_TEMP "Temp"
  #define D_JSON_IRHVAC_CELSIUS "Celsius"
  #define D_JSON_IRHVAC_SWINGV "SwingV"
  #define D_JSON_IRHVAC_SWINGH "SwingH"
  #define D_JSON_IRHVAC_LIGHT "Light"
  #define D_JSON_IRHVAC_BEEP "Beep"
  #define D_JSON_IRHVAC_ECONO "Econo"
  #define D_JSON_IRHVAC_FILTER "Filter"
  #define D_JSON_IRHVAC_TURBO "Turbo"
  #define D_JSON_IRHVAC_QUIET "Quiet"
  #define D_JSON_IRHVAC_CLEAN "Clean"
  #define D_JSON_IRHVAC_SLEEP "Sleep"
  #define D_JSON_IRHVAC_CLOCK "Clock"
  #define D_JSON_IRHVAC_IFEEL "iFeel"
  #define D_JSON_IRHVAC_CONFIG_KEY "ConfigKey"
  #define D_JSON_IRHVAC_CONFIG_VALUE "ConfigValue"
  #define D_JSON_IRHVAC_SENSOR_TEMP "SensorTemp"
  #define D_JSON_IRHVAC_STATE_MODE "StateMode"
    #define D_JSON_IRHVAC_STATE_MODE_SEND_ONLY "SendOnly"
    #define D_JSON_IRHVAC_STATE_MODE_STORE_ONLY "StoreOnly"
    #define D_JSON_IRHVAC_STATE_MODE_SEND_STORE "SendStore"
  #define D_JSON_IRHVAC_INCREMENTAL "Incremental"
#define D_JSON_IRRECEIVED "IrReceived"

// Commands xdrv_06_snfbridge.ino
#define D_CMND_RFCODE "RfCode"
#define D_CMND_RFHIGH "RfHigh"
#define D_CMND_RFHOST "RfHost"
#define D_CMND_RFKEY "RfKey"
  #define D_JSON_START_LEARNING "Start learning"
  #define D_JSON_SET_TO_DEFAULT "Set to default"
  #define D_JSON_DEFAULT_SENT "Default sent"
  #define D_JSON_LEARNED_SENT "Learned sent"
  #define D_JSON_LEARNING_ACTIVE "Learning active"
  #define D_JSON_LEARN_FAILED "Learn failed"
  #define D_JSON_LEARNED "Learned"
#define D_CMND_RFLOW "RfLow"
#define D_CMND_RFSYNC "RfSync"
  #define D_JSON_RFRECEIVED "RfReceived"
#define D_CMND_RFRAW "RfRaw"

// Commands xdrv_08_serial_bridge.ino
#define D_CMND_SSERIALSEND "SSerialSend"
#define D_CMND_SBAUDRATE "SBaudrate"
#define D_CMND_SSERIALBUFFER "SSerialBuffer"
#define D_CMND_SSERIALCONFIG "SSerialConfig"
  #define D_JSON_SSERIALRECEIVED "SSerialReceived"

// Commands xdrv_09_timers.ino
#define D_CMND_TIMER "Timer"
  #define D_JSON_TIMER_ARM "Enable"
  #define D_JSON_TIMER_MODE "Mode"
  #define D_JSON_TIMER_TIME "Time"
  #define D_JSON_TIMER_WINDOW "Window"
  #define D_JSON_TIMER_DAYS "Days"
  #define D_JSON_TIMER_REPEAT "Repeat"
  #define D_JSON_TIMER_OUTPUT "Output"
  #define D_JSON_TIMER_ACTION "Action"
  #define D_JSON_TIMER_NO_DEVICE "No GPIO as output configured"
#define D_CMND_TIMERS "Timers"
#define D_CMND_LATITUDE "Latitude"
#define D_CMND_LONGITUDE "Longitude"
#define D_CMND_SUNRISE "Sunrise"

// Commands xdrv_23_zigbee.ino
#define D_PRFX_ZB "Zb"
#define D_SO_ZIGBEE_NAMEKEY "NameKey"
#define D_SO_ZIGBEE_DEVICETOPIC "DeviceTopic"
#define D_SO_ZIGBEE_NOPREFIX "NoPrefix"
#define D_SO_ZIGBEE_ENDPOINTSUFFIX "EndpointSuffix"
#define D_SO_ZIGBEE_NOAUTOBIND "NoAutoBind"
#define D_SO_ZIGBEE_NAMETOPIC "NameTopic"
#define D_SO_ZIGBEE_ENDPOINTTOPIC "EndpointTopic"
#define D_SO_ZIGBEE_NOAUTOQUERY "NoAutoQuery"
#define D_SO_ZIGBEE_ZBRECEIVEDTOPIC "ReceivedTopic"
#define D_SO_ZIGBEE_OMITDEVICE "OmitDevice"
#define D_CMND_ZIGBEE_PERMITJOIN "PermitJoin"
#define D_CMND_ZIGBEE_STATUS "Status"
#define D_CMND_ZIGBEE_RESET "Reset"
  #define D_JSON_ZIGBEE_CC2530 "CCxxxx"
  #define D_JSON_ZIGBEE_EZSP "EZSP"
#define D_CMND_ZIGBEEZNPRECEIVE "ZNPReceive"      // only for debug
#define D_CMND_ZIGBEE_EZSP_RECEIVE "EZSPReceive"      // only for debug
#define D_CMND_ZIGBEE_EZSP_RECEIVE_RAW "EZSPReceiveRaw"      // only for debug
#define D_CMND_ZIGBEE_EZSP_LISTEN "Listen"      // only for EZSP
#define D_CMND_ZIGBEEZNPSEND "ZNPSend"
#define D_CMND_ZIGBEE_EZSP_SEND "EZSPSend"
#define D_CMND_ZIGBEE_EZSP_SEND_RAW "EZSPSendRaw"
  #define D_JSON_ZIGBEE_STATE "ZbState"
  #define D_JSON_ZIGBEE_ROUTE_ERROR "ZbRouteError"
  #define D_JSON_ZIGBEEZNPRECEIVED "ZbZNPReceived"
  #define D_JSON_ZIGBEE_EZSP_RECEIVED "ZbEZSPReceived"
  #define D_JSON_ZIGBEEZNPSENT "ZbZNPSent"
  #define D_JSON_ZIGBEE_EZSP_SENT "ZbEZSPSent"
  #define D_JSON_ZIGBEE_EZSP_SENT_RAW "ZbEZSPSentRaw"
  #define D_JSON_ZIGBEEZCL_RECEIVED "ZbZCLReceived"
  #define D_JSON_ZIGBEEGP_RECEIVED "ZbGPReceived"
  #define D_JSON_ZIGBEEZCL_RAW_RECEIVED "ZbZCLRawReceived"
  #define D_JSON_ZIGBEE_DEVICE "Device"
  #define D_JSON_ZIGBEE_NAME "Name"
  #define D_JSON_ZIGBEE_NAMES "Names"
  #define D_JSON_ZIGBEE_CONFIRM "ZbConfirm"
#define D_CMND_ZIGBEE_NAME "Name"
#define D_CMND_ZIGBEE_MODELID "ModelId"
  #define D_JSON_ZIGBEE_MODELID "ModelId"
#define D_CMND_ZIGBEE_PROBE "Probe"
#define D_CMND_ZIGBEE_FORGET "Forget"
#define D_CMND_ZIGBEE_INFO "Info"
#define D_CMND_ZIGBEE_SAVE "Save"
  #define D_CMND_ZIGBEE_LINKQUALITY "LinkQuality"
  #define D_CMND_ZIGBEE_CLUSTER "Cluster"
  #define D_CMND_ZIGBEE_BATTPERCENT "BatteryPercentage"
  #define D_CMND_ZIGBEE_ENDPOINT "Endpoint"
  #define D_CMND_ZIGBEE_GROUP "Group"
  #define D_CMND_ZIGBEE_MANUF "Manuf"
  #define D_CMND_ZIGBEE_DEVICE "Device"
#define D_CMND_ZIGBEE_READ "Read"
#define D_CMND_ZIGBEE_SEND "Send"
#define D_CMND_ZIGBEE_WRITE "Write"
#define D_CMND_ZIGBEE_REPORT "Report"
#define D_CMND_ZIGBEE_READ_CONFIG "ReadConfig"
#define D_CMND_ZIGBEE_CONFIG "Config"
#define D_CMND_ZIGBEE_RESPONSE "Response"
  #define D_JSON_ZIGBEE_ZCL_SENT "ZbZCLSent"
#define D_JSON_ZIGBEE_RECEIVED "ZbReceived"
#define D_JSON_ZIGBEE_INFO "ZbInfo"
#define D_CMND_ZIGBEE_BIND "Bind"
  #define D_JSON_ZIGBEE_BIND "ZbBind"
#define D_CMND_ZIGBEE_UNBIND "Unbind"
  #define D_JSON_ZIGBEE_UNBIND "ZbUnbind"
#define D_CMND_ZIGBEE_BIND_STATE "BindState"
  #define D_JSON_ZIGBEE_BIND_STATE "ZbBindState"
#define D_CMND_ZIGBEE_LEAVE "Leave"
#define D_CMND_ZIGBEE_MAP "Map"
  #define D_JSON_ZIGBEE_MAP "ZbMap"
#define D_JSON_ZIGBEE_PARENT "ZbParent"
#define D_CMND_ZIGBEE_PING "Ping"
  #define D_JSON_ZIGBEE_PING "ZbPing"
  #define D_JSON_ZIGBEE_IEEE "IEEEAddr"
#define D_JSON_ZIGBEE_RESPONSE "ZbResponse"
  #define D_JSON_ZIGBEE_CMD "Command"
  #define D_JSON_ZIGBEE_STATUS "Status"
  #define D_JSON_ZIGBEE_STATUS_MSG "StatusMessage"
#define D_CMND_ZIGBEE_LIGHT "Light"
  #define D_JSON_ZIGBEE_LIGHT "Light"
#define D_CMND_ZIGBEE_OCCUPANCY "Occupancy"
  #define D_JSON_ZIGBEE_OCCUPANCY "Occupancy"
#define D_CMND_ZIGBEE_RESTORE "Restore"
#define D_CMND_ZIGBEE_CONFIG "Config"
  #define D_JSON_ZIGBEE_CONFIG "Config"
#define D_CMND_ZIGBEE_DATA "Data"
#define D_CMND_ZIGBEE_SCAN "Scan"
  #define D_JSON_ZIGBEE_SCAN "ZbScan"
#define D_CMND_ZIGBEE_CIE "CIE"
#define D_CMND_ZIGBEE_EMULATION "Emulation"
#define D_CMND_ZIGBEE_ENROLL "Enroll"
#define D_CMND_ZIGBEE_LOAD "Load"
#define D_CMND_ZIGBEE_LOADDUMP "LoadDump"
#define D_CMND_ZIGBEE_UNLOAD "Unload"
#define D_CMND_ZIGBEE_ATTRDUMP "AttrDump"

// Commands xdrv_25_A4988_Stepper.ino
#define D_CMND_MOTOR "MOTOR"
#define D_JSON_MOTOR_MOVE "doMove"
#define D_JSON_MOTOR_ROTATE "doRotate"
#define D_JSON_MOTOR_TURN "doTurn"
#define D_JSON_MOTOR_SPR "setSPR"
#define D_JSON_MOTOR_RPM "setRPM"
#define D_JSON_MOTOR_MIS "setMIS"

// Commands xdrv_27_Shutter.ino
#define D_PRFX_SHUTTER "Shutter"
#define D_CMND_SHUTTER_OPEN "Open"
#define D_CMND_SHUTTER_CLOSE "Close"
#define D_CMND_SHUTTER_TOGGLE "Toggle"
#define D_CMND_SHUTTER_TOGGLEDIR "ToggleDir"
#define D_CMND_SHUTTER_UP "Up"
#define D_CMND_SHUTTER_DOWN "Down"
#define D_CMND_SHUTTER_MODE "Mode"
#define D_CMND_SHUTTER_STOPOPEN "StopOpen"
#define D_CMND_SHUTTER_STOPCLOSE "StopClose"
#define D_CMND_SHUTTER_STOPTOGGLE "StopToggle"
#define D_CMND_SHUTTER_STOPTOGGLEDIR "StopToggleDir"
#define D_CMND_SHUTTER_INCDEC "Change"
#define D_CMND_SHUTTER_STOPPOSITION "StopPosition"
#define D_CMND_SHUTTER_STOP "Stop"
#define D_CMND_SHUTTER_POSITION "Position"
#define D_CMND_SHUTTER_OPENTIME "OpenDuration"
#define D_CMND_SHUTTER_CLOSETIME "CloseDuration"
#define D_CMND_SHUTTER_RELAY "Relay"
#define D_CMND_SHUTTER_SETHALFWAY "SetHalfway"
#define D_CMND_SHUTTER_SETCLOSE "SetClose"
#define D_CMND_SHUTTER_SETOPEN "SetOpen"
#define D_CMND_SHUTTER_INVERT "Invert"
#define D_CMND_SHUTTER_CLIBRATION "Calibration"
#define D_CMND_SHUTTER_MOTORDELAY "MotorDelay"
#define D_CMND_SHUTTER_FREQUENCY "Frequency"
#define D_CMND_SHUTTER_BUTTON "Button"
#define D_CMND_SHUTTER_LOCK "Lock"
#define D_CMND_SHUTTER_ENABLEENDSTOPTIME "EnableEndStopTime"
#define D_CMND_SHUTTER_INVERTWEBBUTTONS "InvertWebButtons"
#define D_CMND_SHUTTER_PWMRANGE "PWMRange"
#define D_CMND_SHUTTER_UNITTEST "UnitTest"
#define D_CMND_SHUTTER_TILTCONFIG "TiltConfig"
#define D_CMND_SHUTTER_SETTILT "Tilt"
#define D_CMND_SHUTTER_TILTINCDEC "TiltChange"
#define D_CMND_SHUTTER_MOTORSTOP "MotorStop"
#define D_CMND_SHUTTER_SETUP "Setup"

// Commands xdrv_32_hotplug.ino
#define D_CMND_HOTPLUG "HotPlug"

// Commands xdrv_34_pwm_dimmer.ino
#ifdef USE_PWM_DIMMER
#define D_CMND_BRI_PRESET "BriPreset"
#define D_CMND_PWM_DIMMER_PWMS "PWMDimmerPWMs"
#endif

// Commands xdrv_38_ping.ino
#define D_CMND_PING "Ping"
#define D_JSON_PING "Ping"

// Commands xdrv_42_audio - I2S Audio
#define D_PRFX_I2S "I2S"
#define D_JSON_I2S_CONFIG "Config"


// Commands xdrv_52_berry.ino - Berry scripting language
#define D_PRFX_BR "Br"
#define D_CMND_BR_RUN ""
#define D_CMND_BR_RESTART "Restart"
#define D_BR_NOT_STARTED  "Berry not started"

// Commands xdrv_60_shift595.ino - 74x595 family shift register driver
#define D_CMND_SHIFT595_DEVICE_COUNT "Shift595DeviceCount"

// Commands xdrv_70_hdmi_cec.ino
#define D_PRFX_HDMI "Hdmi"
#define D_CMND_HDMI_SEND "Send"
#define D_CMND_HDMI_SEND_RAW "SendRaw"
#define D_CMND_HDMI_TYPE "Type"
#define D_CMND_HDMI_ADDR "Addr"

// Commands xdrv_89_dali.ino
#define D_CMND_DALI_POWER                 "power"
#define D_CMND_DALI_DIMMER                "dim"

// Commands xsns_02_analog.ino
#define D_CMND_ADCPARAM "AdcParam"

// Commands xsns_05_ds18x20.ino
#define D_CMND_DS_ALIAS "Alias"
#define D_CMND_DS_RESCAN "Rescan"
#define D_CMND_DS_RETRYREAD "RetryRead"

// xsns_70_veml6075.ino
#define D_JSON_UVA_INTENSITY "UvaIntensity"
#define D_JSON_UVB_INTENSITY  "UvbIntensity"
#define D_CMND_VEML6075_POWER "power"
#define D_CMND_VEML6075_DYNAMIC "dynamic"
#define D_CMND_VEML6075_INTTIME "inttime"

// xsns_71_veml7700.ino
#define D_JSON_WHITE_CONTENT "WhiteContent"

// xdrv_92_pipsolar.ino
#define D_CMND_PIP_PREFIX "PipSolar"
#define D_CMND_PIP_QT "QT"
#define D_CMND_PIP_QET "QET"
#define D_CMND_PIP_QEY "QEY"
#define D_CMND_PIP_QEM "QEM"
#define D_CMND_PIP_QED "QED"
#define D_CMND_PIP_QEH "QEH"
#define D_CMND_PIP_DAT "DAT"
#define D_CMND_PIP_POLLVALUES "PollValues"
#define D_CMND_PIP_BAUDRATE "BaudRate"
#define D_CMND_PIP_SERIALCONFIG "SerialConfig"

/********************************************************************************************/

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
#define D_LOG_HRE "HRE: "
#define D_LOG_I2C "I2C: "          // I2C
#define D_LOG_IRR "IRR: "          // Infra Red Received
#define D_LOG_KNX "KNX: "
#define D_LOG_LOG "LOG: "          // Logging
#define D_LOG_MODULE "MOD: "       // Module
#define D_LOG_MDNS "mDN: "         // mDNS
#define D_LOG_MQTT "MQT: "         // MQTT
#define D_LOG_OTHER "OTH: "        // Other
#define D_LOG_RESULT "RSL: "       // Result
#define D_LOG_RFR "RFR: "          // RF Received
#define D_LOG_SERIAL "SER: "       // Serial
#define D_LOG_SHT1 "SHT: "         // SHT1x sensor
#define D_LOG_UPLOAD "UPL: "       // Upload
#define D_LOG_UPNP "UPP: "         // UPnP
#define D_LOG_WIFI "WIF: "         // Wifi
#define D_LOG_ETH "ETH: "          // Ethernet
#define D_LOG_ZIGBEE "ZIG: "       // Zigbee
#define D_LOG_TCP "TCP: "          // TCP bridge
#define D_LOG_BERRY "BRY: "        // Berry scripting language
#define D_LOG_LVGL "LVG: "         // LVGL graphics engine
#define D_LOG_THERMOSTAT "THE: "   // Thermostat driver
#define D_LOG_SENSOR "SNS: "       // Sensor driver

/********************************************************************************************/

#define D_ASTERISK_PWD "****"

#ifndef MY_LANGUAGE
  #include "language/en_GB.h"
#else
  #define QUOTEME(x) QUOTEME_1(x)
  #define QUOTEME_1(x) #x
  #define INCLUDE_FILE(x) QUOTEME(language/x.h)
  #include INCLUDE_FILE(MY_LANGUAGE)
#endif
#ifndef LANGUAGE_LCID
  #define LANGUAGE_LCID 2057  // en_GB
#endif

// Common
const char S_JSON_COMMAND_NVALUE_SPACE_UNIT[] PROGMEM =       "{\"%s\":\"%d %s\"}";
const char S_JSON_COMMAND_LVALUE_SPACE_UNIT[] PROGMEM =       "{\"%s\":\"%lu %s\"}";
const char S_JSON_COMMAND_SVALUE_SPACE_UNIT[] PROGMEM =       "{\"%s\":\"%s %s\"}";

const char S_JSON_COMMAND_NVALUE_SVALUE[] PROGMEM =           "{\"%s\":{\"%d\":\"%s\"}}";
const char S_JSON_COMMAND_NVALUE_ACTIVE_NVALUE[] PROGMEM =    "{\"%s\":{\"%d\":{\"" D_JSON_ACTIVE "\":\"%d\"}}}";

const char S_JSON_COMMAND_NVALUE[] PROGMEM =                  "{\"%s\":%d}";
const char S_JSON_COMMAND_LVALUE[] PROGMEM =                  "{\"%s\":%lu}";
const char S_JSON_COMMAND_SVALUE[] PROGMEM =                  "{\"%s\":\"%s\"}";
const char S_JSON_COMMAND_ASTERISK[] PROGMEM =                "{\"%s\":\"" D_ASTERISK_PWD "\"}";
const char S_JSON_COMMAND_XVALUE[] PROGMEM =                  "{\"%s\":%s}";  // %s must provide quotes on non-number

const char S_JSON_COMMAND_INDEX_NVALUE[] PROGMEM =            "{\"%s%d\":%d}";
const char S_JSON_COMMAND_INDEX_LVALUE[] PROGMEM =            "{\"%s%d\":%lu}";
const char S_JSON_COMMAND_INDEX_SVALUE[] PROGMEM =            "{\"%s%d\":\"%s\"}";
const char S_JSON_COMMAND_INDEX_ASTERISK[] PROGMEM =          "{\"%s%d\":\"" D_ASTERISK_PWD "\"}";

const char S_JSON_SENSOR_INDEX_NVALUE[] PROGMEM =             "{\"" D_CMND_SENSOR "%d\":%d}";
const char S_JSON_SENSOR_INDEX_SVALUE[] PROGMEM =             "{\"" D_CMND_SENSOR "%d\":\"%s\"}";

const char S_JSON_DRIVER_INDEX_NVALUE[] PROGMEM =             "{\"" D_CMND_DRIVER "%d\":%d}";
const char S_JSON_DRIVER_INDEX_SVALUE[] PROGMEM =             "{\"" D_CMND_DRIVER "%d\":\"%s\"}";

const char S_JSON_SVALUE_ACTION_SVALUE[] PROGMEM =            "{\"%s\":{\"Action\":\"%s\"}}";

const char JSON_SNS_F_TEMP[] PROGMEM = ",\"%s\":{\"" D_JSON_TEMPERATURE "\":%*_f}";
const char JSON_SNS_TEMP[] PROGMEM = ",\"%s\":{\"" D_JSON_TEMPERATURE "\":%s}";

const char JSON_SNS_ILLUMINANCE[] PROGMEM = ",\"%s\":{\"" D_JSON_ILLUMINANCE "\":%d}";
const char JSON_SNS_MOISTURE[] PROGMEM = ",\"%s\":{\"" D_JSON_MOISTURE "\":%d}";
const char JSON_SNS_RANGE[] PROGMEM = ",\"%s\":{\"" D_JSON_RANGE "\":%d}";

const char JSON_SNS_GNGPM[] PROGMEM = ",\"%s\":{\"" D_JSON_TOTAL_USAGE "\":%s,\"" D_JSON_FLOWRATE "\":%s}";

const char S_LOG_I2C_FOUND_AT[] PROGMEM = D_LOG_I2C "%s " D_FOUND_AT " 0x%x";

const char S_RSLT_POWER[] PROGMEM = D_RSLT_POWER;
const char S_RSLT_RESULT[] PROGMEM = D_RSLT_RESULT;
const char S_RSLT_WARNING[] PROGMEM = D_RSLT_WARNING;
const char S_LWT[] PROGMEM = D_LWT;
const char S_LWT_OFFLINE[] PROGMEM = MQTT_LWT_OFFLINE;

// support.ino
static const char kMonthNames[] = D_MONTH3LIST;

const char kSpeedUnit[] PROGMEM = "|m/s|km/h|kn|mph|ft/s|yd/s";
const float kSpeedConversionFactor[] = {1,            // none
                                        1,            // m/s
                                        3.6,          // km/h
                                        1.943844492,  // kn
                                        2.236936292,  // mph
                                        3.280839895,  // ft/s
                                        1.093613298   // yd/s
                                       };

// xdrv_02_webserver.ino
#ifdef USE_WEBSERVER
// {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
const char HTTP_SNS_HR[]            PROGMEM = "<tr><td colspan=2 style='font-size:2px'><hr/></td></tr>";
const char HTTP_SNS_HR_THIN[]       PROGMEM = "<tr><td colspan=2 style='font-size:2px'><hr size=1/></td></tr>";
const char HTTP_SNS_F_TEMP[]        PROGMEM = "{s}%s "  D_TEMPERATURE         "{m}%*_f " D_UNIT_DEGREE          "%c{e}";
const char HTTP_SNS_F_VOLTAGE[]     PROGMEM = "{s}%s "  D_VOLTAGE             "{m}%*_f " D_UNIT_VOLT              "{e}";
const char HTTP_SNS_F_CURRENT_MA[]  PROGMEM = "{s}%s "  D_CURRENT             "{m}%*_f " D_UNIT_MILLIAMPERE       "{e}";
const char HTTP_SNS_F_DISTANCE_CM[] PROGMEM = "{s}%s "  D_DISTANCE            "{m}%1_f " D_UNIT_CENTIMETER        "{e}";
const char HTTP_SNS_F_NOX[]         PROGMEM = "{s}%s "  D_NOX                 "{m}%*_f "                          "{e}";
const char HTTP_SNS_F_VOC[]         PROGMEM = "{s}%s "  D_VOC                 "{m}%*_f "                          "{e}";
const char HTTP_SNS_F_ABS_HUM[]     PROGMEM = "{s}%s "  D_ABSOLUTE_HUMIDITY   "{m}%*_f " D_UNIT_GRAM_PER_CUBIC_METER "{e}";

const char HTTP_SNS_HUM[]           PROGMEM = "{s}%s "  D_HUMIDITY            "{m}%s " D_UNIT_PERCENT             "{e}";
const char HTTP_SNS_DEW[]           PROGMEM = "{s}%s "  D_DEWPOINT            "{m}%s " D_UNIT_DEGREE            "%c{e}";
const char HTTP_SNS_PRESSURE[]      PROGMEM = "{s}%s "  D_PRESSURE            "{m}%s "                          "%s{e}";
const char HTTP_SNS_SEAPRESSURE[]   PROGMEM = "{s}%s "  D_PRESSUREATSEALEVEL  "{m}%s "                          "%s{e}";
const char HTTP_SNS_ANALOG[]        PROGMEM = "{s}%s "  D_ANALOG_INPUT      "%d{m}%d"                             "{e}";
const char HTTP_SNS_ILLUMINANCE[]   PROGMEM = "{s}%s "  D_ILLUMINANCE         "{m}%d " D_UNIT_LUX                 "{e}";
const char HTTP_SNS_CO2[]           PROGMEM = "{s}%s "  D_CO2                 "{m}%d " D_UNIT_PARTS_PER_MILLION   "{e}";
const char HTTP_SNS_CO2EAVG[]       PROGMEM = "{s}%s "  D_ECO2                "{m}%d " D_UNIT_PARTS_PER_MILLION   "{e}";
const char HTTP_SNS_GALLONS[]       PROGMEM = "{s}%s "  D_TOTAL_USAGE         "{m}%s " D_UNIT_GALLONS             "{e}";
const char HTTP_SNS_GPM[]           PROGMEM = "{s}%s "  D_FLOW_RATE           "{m}%s " D_UNIT_GALLONS_PER_MIN     "{e}";
const char HTTP_SNS_MOISTURE[]      PROGMEM = "{s}%s "  D_MOISTURE            "{m}%d " D_UNIT_PERCENT             "{e}";
const char HTTP_SNS_RANGE_CHR[]     PROGMEM = "{s}%s "  D_RANGE               "{m}%s"                             "{e}";
const char HTTP_SNS_RANGE[]         PROGMEM = "{s}%s "  D_RANGE               "{m}%d"                             "{e}";
const char HTTP_SNS_HALL_EFFECT[]   PROGMEM = "{s}%s "  D_HALL_EFFECT         "{m}%d"                             "{e}";
const char HTTP_SNS_PH[]            PROGMEM = "{s}%s "  D_PH                  "{m}%s "                            "{e}";
const char HTTP_SNS_MQ[]            PROGMEM = "{s}"     D_MQ"-%s"             "{m}%s " D_UNIT_PARTS_PER_MILLION   "{e}";
const char HTTP_SNS_ORP[]           PROGMEM = "{s}%s "  D_ORP                 "{m}%s " D_UNIT_MILLIVOLT           "{e}";
const char HTTP_SNS_EC[]            PROGMEM = "{s}%s "  D_EC                  "{m}%s " D_UNIT_MICROSIEMENS_PER_CM "{e}";
const char HTTP_SNS_O2[]            PROGMEM = "{s}%s "  D_O2                  "{m}%s " D_UNIT_PERCENT             "{e}";
const char HTTP_SNS_LITERS[]        PROGMEM = "{s}%s "  D_VOLUME              "{m}%s " D_UNIT_LITERS              "{e}";
const char HTTP_SNS_LPM[]           PROGMEM = "{s}%s "  D_FLOW_RATE           "{m}%s " D_UNIT_LITERS_PER_MIN      "{e}";
const char HTTP_SNS_DO[]            PROGMEM = "{s}%s "  D_DO                  "{m}%s " D_UNIT_MILIGRAMS_PER_LITER "{e}";
const char HTTP_SNS_COLOR_RED[]     PROGMEM = "{s}%s "  D_COLOR_RED           "{m}%u "                            "{e}";
const char HTTP_SNS_COLOR_GREEN[]   PROGMEM = "{s}%s "  D_COLOR_GREEN         "{m}%u "                            "{e}";
const char HTTP_SNS_COLOR_BLUE[]    PROGMEM = "{s}%s "  D_COLOR_BLUE          "{m}%u "                            "{e}";
const char HTTP_SNS_MILLILITERS[]   PROGMEM = "{s}%s "  D_VOLUME              "{m}%s " D_UNIT_MILLILITERS         "{e}";
const char HTTP_SNS_GAS[]           PROGMEM = "{s}%s "  D_GAS                 "{m}%d " D_UNIT_PERCENT          "LEL{e}";
const char HTTP_SNS_SOC[]           PROGMEM = "{s}%s "  D_SOC                 "{m}%d " D_UNIT_PERCENT             "{e}";
const char HTTP_SNS_SOH[]           PROGMEM = "{s}%s "  D_SOH                 "{m}%d " D_UNIT_PERCENT             "{e}";

const char HTTP_SNS_STANDARD_CONCENTRATION[] PROGMEM =        "{s}%s " D_STANDARD_CONCENTRATION      " %s " D_UNIT_MICROMETER "{m}%d " D_UNIT_MICROGRAM_PER_CUBIC_METER "{e}";
const char HTTP_SNS_ENVIRONMENTAL_CONCENTRATION[] PROGMEM =   "{s}%s " D_ENVIRONMENTAL_CONCENTRATION " %s " D_UNIT_MICROMETER "{m}%d " D_UNIT_MICROGRAM_PER_CUBIC_METER "{e}";
const char HTTP_SNS_F_ENVIRONMENTAL_CONCENTRATION[] PROGMEM = "{s}%s " D_ENVIRONMENTAL_CONCENTRATION " %s " D_UNIT_MICROMETER "{m}%1_f " D_UNIT_MICROGRAM_PER_CUBIC_METER "{e}";
const char HTTP_SNS_PARTICALS_BEYOND[] PROGMEM =              "{s}%s " D_PARTICALS_BEYOND            " %s " D_UNIT_MICROMETER "{m}%d " D_UNIT_PARTS_PER_DECILITER       "{e}";
const char HTTP_SNS_AVG_RAD_DOSE[]     PROGMEM =              "{s}%s " D_AVG_RAD_DOSE                " %s " D_UNIT_MINUTE     "{m}%d.%02d " D_UNIT_US_H                  "{e}";

const char HTTP_SNS_VOLTAGE[]             PROGMEM = "{s}" D_VOLTAGE                 "{m}%s " D_UNIT_VOLT          "{e}";
const char HTTP_SNS_CURRENT[]             PROGMEM = "{s}" D_CURRENT                 "{m}%s " D_UNIT_AMPERE        "{e}";
const char HTTP_SNS_CURRENT_N[]           PROGMEM = "{s}" D_CURRENT_NEUTRAL         "{m}%s " D_UNIT_AMPERE        "{e}";
const char HTTP_SNS_POWER[]               PROGMEM = "{s}" D_POWERUSAGE_ACTIVE       "{m}%s " D_UNIT_WATT          "{e}";
const char HTTP_SNS_IMPORT_POWER[]        PROGMEM = "{s}" D_IMPORT_POWER            "{m}%s " D_UNIT_WATT          "{e}";
const char HTTP_SNS_EXPORT_POWER[]        PROGMEM = "{s}" D_EXPORT_POWER            "{m}%s " D_UNIT_WATT          "{e}";
const char HTTP_SNS_MAX_POWER[]           PROGMEM = "{s}" D_MAX_POWER               "{m}%s " D_UNIT_WATT          "{e}";
const char HTTP_SNS_POWER_TOTAL[]         PROGMEM = "{s}" D_POWERUSAGE_ACTIVE_TOTAL "{m}%s " D_UNIT_WATT          "{e}";
const char HTTP_SNS_POWERUSAGE_APPARENT[] PROGMEM = "{s}" D_POWERUSAGE_APPARENT     "{m}%s " D_UNIT_VA            "{e}";
const char HTTP_SNS_POWERUSAGE_REACTIVE[] PROGMEM = "{s}" D_POWERUSAGE_REACTIVE     "{m}%s " D_UNIT_VAR           "{e}";
const char HTTP_SNS_POWER_FACTOR[]        PROGMEM = "{s}" D_POWER_FACTOR            "{m}%s                         {e}";
const char HTTP_SNS_ENERGY_TODAY[]        PROGMEM = "{s}" D_ENERGY_TODAY            "{m}%s " D_UNIT_KILOWATTHOUR  "{e}";
const char HTTP_SNS_ENERGY_YESTERDAY[]    PROGMEM = "{s}" D_ENERGY_YESTERDAY        "{m}%s " D_UNIT_KILOWATTHOUR  "{e}";
const char HTTP_SNS_ENERGY_TOTAL[]        PROGMEM = "{s}" D_ENERGY_TOTAL            "{m}%s " D_UNIT_KILOWATTHOUR  "{e}";
const char HTTP_SNS_EXPORT_ACTIVE[]       PROGMEM = "{s}" D_EXPORT_ACTIVE           "{m}%s " D_UNIT_KILOWATTHOUR  "{e}";
const char HTTP_SNS_TOTAL_ACTIVE[]        PROGMEM = "{s}" D_TOTAL_ACTIVE            "{m}%s " D_UNIT_KILOWATTHOUR  "{e}";
const char HTTP_SNS_RSTTBL_TOTAL_ACTIVE[] PROGMEM = "{s}" D_RESETTABLE_TOTAL_ACTIVE "{m}%s " D_UNIT_KILOWATTHOUR  "{e}";
const char HTTP_SNS_IMPORT_REACTIVE[]     PROGMEM = "{s}" D_IMPORT_REACTIVE         "{m}%s " D_UNIT_KWARH         "{e}";
const char HTTP_SNS_EXPORT_REACTIVE[]     PROGMEM = "{s}" D_EXPORT_REACTIVE         "{m}%s " D_UNIT_KWARH         "{e}";
const char HTTP_SNS_TOTAL_REACTIVE[]      PROGMEM = "{s}" D_TOTAL_REACTIVE          "{m}%s " D_UNIT_KWARH         "{e}";
const char HTTP_SNS_PHASE_ANGLE[]         PROGMEM = "{s}" D_PHASE_ANGLE             "{m}%s " D_UNIT_ANGLE         "{e}";
#endif  // USE_WEBSERVER

#endif  // _I18N_H_
