/*
  zh-CN.h - localization for Chinese (Simplified) - China for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends (translated by killadm)

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

#ifndef _LANGUAGE_ZH_CN_H_
#define _LANGUAGE_ZH_CN_H_

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

#define D_DAY3LIST "SunMonTueWedThuFriSat"
#define D_MONTH3LIST "JanFebMarAprMayJunJulAugSepOctNovDec"

// Non JSON decimal separator
#define D_DECIMAL_SEPARATOR "."

// Common
#define D_ADMIN "Admin"
#define D_AIR_QUALITY "空气质量"
#define D_AP "AP"                    // Access Point
#define D_AS "名称:"
#define D_AUTO "自动"
#define D_BLINK "闪烁"
#define D_BLINKOFF "闪烁关"
#define D_BOOT_COUNT "启动次数"
#define D_BRIGHTLIGHT "亮度"
#define D_BUTTON "按钮"
#define D_BY "by"                    // Written by me
#define D_BYTES "大小:"
#define D_CELSIUS "摄氏"
#define D_CO2 "二氧化碳"
#define D_CODE "代码"                // Button code
#define D_COLDLIGHT "冷"
#define D_COMMAND "命令:"
#define D_CONNECTED "已连接"
#define D_COUNT "数量:"
#define D_COUNTER "Counter"
#define D_CURRENT "电流"          // As in Voltage and Current
#define D_DATA "数据:"
#define D_DARKLIGHT "Dark"
#define D_DEBUG "调试"
#define D_DISABLED "禁用"
#define D_DNS_SERVER "DNS服务器"
#define D_DONE "完成"
#define D_DST_TIME "DST"
#define D_EMULATION "设备模拟"
#define D_ENABLED "启用"
#define D_ERASE "擦除"
#define D_ERROR "错误"
#define D_FAHRENHEIT "华氏"
#define D_FAILED "失败"
#define D_FALLBACK "Fallback"
#define D_FALLBACK_TOPIC "Fallback Topic"
#define D_FALSE "False"
#define D_FILE "文件:"
#define D_FREE_MEMORY "空闲内存"
#define D_GAS "气体"
#define D_GATEWAY "网关"
#define D_GROUP "组:"
#define D_HOST "主机"
#define D_HOSTNAME "主机名"
#define D_HUMIDITY "湿度"
#define D_ILLUMINANCE "照度"
#define D_IMMEDIATE "immediate"      // Button immediate
#define D_INDEX "索引:"
#define D_INFO "信息"
#define D_INITIALIZED "初始化完成"
#define D_IP_ADDRESS "IP地址"
#define D_LIGHT "灯"
#define D_LWT "LWT"
#define D_MODULE "模块"
#define D_MQTT "MQTT"
#define D_MULTI_PRESS "多次按键"
#define D_NOISE "嘈杂"
#define D_NONE "无"
#define D_OFF "关"
#define D_OFFLINE "离线"
#define D_OK "好"
#define D_ON "开"
#define D_ONLINE "在线"
#define D_PASSWORD "密码"
#define D_PORT "端口"
#define D_POWER_FACTOR "功率因数"
#define D_POWERUSAGE "功率"
#define D_PRESSURE "气压"
#define D_PRESSUREATSEALEVEL "海平面气压"
#define D_PROGRAM_FLASH_SIZE "固件 Flash 大小"
#define D_PROGRAM_SIZE "固件大小"
#define D_PROJECT "项目:"
#define D_RECEIVED "已接收"
#define D_RESTART "重启"
#define D_RESTARTING "正在重启"
#define D_RESTART_REASON "重启原因"
#define D_RESTORE "恢复"
#define D_RETAINED "已保留"
#define D_SAVE "保存"
#define D_SENSOR "传感器"
#define D_SSID "名称"
#define D_START "开始"
#define D_STD_TIME "STD"
#define D_STOP "停止"
#define D_SUBNET_MASK "子网掩码"
#define D_SUBSCRIBE_TO "订阅"
#define D_SUCCESSFUL "成功"
#define D_TEMPERATURE "温度"
#define D_TO "to"
#define D_TOGGLE "切换"
#define D_TOPIC "主题"
#define D_TRANSMIT "发送"
#define D_TRUE "True"
#define D_UPGRADE "升级"
#define D_UPLOAD "上传"
#define D_UPTIME "运行时间"
#define D_USER "用户名"
#define D_UTC_TIME "UTC"
#define D_UV_LEVEL "紫外线水平"
#define D_VERSION "版本"
#define D_VOLTAGE "电压"
#define D_WARMLIGHT "暖"
#define D_WEB_SERVER "Web Server"

// sonoff.ino
#define D_WARNING_MINIMAL_VERSION "警告：精简固件不支持配置持久化保存"
#define D_LEVEL_10 "level 1-0"
#define D_LEVEL_01 "level 0-1"
#define D_SERIAL_LOGGING_DISABLED "串口日志已禁用"
#define D_SYSLOG_LOGGING_REENABLED "Syslog 日志已开启"

#define D_SET_BAUDRATE_TO "设置波特率为:"
#define D_RECEIVED_TOPIC "接收到的主题:"
#define D_DATA_SIZE "数据大小:"
#define D_ANALOG_INPUT "Analog"

#define D_FINGERPRINT "验证 TLS 指纹..."
#define D_TLS_CONNECT_FAILED_TO "TLS 连接失败"
#define D_RETRY_IN "重试倒计时:"
#define D_VERIFIED "验证成功"
#define D_INSECURE "指纹无效导致连接不安全"
#define D_CONNECT_FAILED_TO "连接失败:"

// support.ino
#define D_OSWATCH "osWatch"
#define D_BLOCKED_LOOP "Blocked Loop"
#define D_WPS_FAILED_WITH_STATUS "WPSconfig FAILED with status"
#define D_ACTIVE_FOR_3_MINUTES "active for 3 minutes"
#define D_FAILED_TO_START "未能启动"
#define D_PATCH_ISSUE_2186 "Patch issue 2186"
#define D_CONNECTING_TO_AP "连接到 AP"
#define D_IN_MODE "模式:"
#define D_CONNECT_FAILED_NO_IP_ADDRESS "连接失败，因为没有获取到IP地址"
#define D_CONNECT_FAILED_AP_NOT_REACHED "连接失败，无法连接AP"
#define D_CONNECT_FAILED_WRONG_PASSWORD "连接失败，AP密码不正确"
#define D_CONNECT_FAILED_AP_TIMEOUT "连接失败，AP超时"
#define D_ATTEMPTING_CONNECTION "尝试连接..."
#define D_CHECKING_CONNECTION "检查连接..."
#define D_QUERY_DONE "查询完成。 发现MQTT服务"
#define D_MQTT_SERVICE_FOUND "发现MQTT服务:"
#define D_FOUND_AT "found at"
#define D_SYSLOG_HOST_NOT_FOUND "Syslog主机未找到"

// settings.ino
#define D_SAVED_TO_FLASH_AT "保存到 flash:"
#define D_LOADED_FROM_FLASH_AT "从 flash 载入"
#define D_USE_DEFAULTS "使用默认设置"
#define D_ERASED_SECTOR "擦除删除"

// webserver.ino
#define D_MINIMAL_FIRMWARE_PLEASE_UPGRADE "固件版本过低 - 请升级"
#define D_WEBSERVER_ACTIVE_ON "Web服务器:"
#define D_WITH_IP_ADDRESS "IP地址:"
#define D_WEBSERVER_STOPPED "Web 服务器已停止"
#define D_FILE_NOT_FOUND "文件未找到"
#define D_REDIRECTED "重定向到认证页面"
#define D_WIFIMANAGER_SET_ACCESSPOINT_AND_STATION "Wifimanager set AccessPoint and keep Station"
#define D_WIFIMANAGER_SET_ACCESSPOINT "Wifimanager 设置接入点"
#define D_TRYING_TO_CONNECT "尝试将设备连接到网络"

#define D_RESTART_IN "重启需要"
#define D_SECONDS "秒"
#define D_DEVICE_WILL_RESTART "设备将在几分钟内重启"
#define D_BUTTON_TOGGLE "状态切换"
#define D_CONFIGURATION "设置"
#define D_INFORMATION "信息"
#define D_FIRMWARE_UPGRADE "固件升级"
#define D_CONSOLE "控制台"
#define D_CONFIRM_RESTART "确认重启"

#define D_CONFIGURE_MODULE "模块设置"
#define D_CONFIGURE_WIFI "WiFi设置"
#define D_CONFIGURE_MQTT "MQTT设置"
#define D_CONFIGURE_DOMOTICZ "Domoticz设置"
#define D_CONFIGURE_LOGGING "日志设置"
#define D_CONFIGURE_OTHER "其他设置"
#define D_CONFIRM_RESET_CONFIGURATION "确认重置配置"
#define D_RESET_CONFIGURATION "重置配置"
#define D_BACKUP_CONFIGURATION "备份配置"
#define D_RESTORE_CONFIGURATION "还原配置"
#define D_MAIN_MENU "主菜单"

#define D_MODULE_PARAMETERS "模块设置"
#define D_MODULE_TYPE "模块类型"
#define D_GPIO "GPIO"
#define D_SERIAL_IN "串口输入(RX)"
#define D_SERIAL_OUT "串口输出(TX)"

#define D_WIFI_PARAMETERS "Wifi设置"
#define D_SCAN_FOR_WIFI_NETWORKS "扫描无线网络"
#define D_SCAN_DONE "扫描完毕"
#define D_NO_NETWORKS_FOUND "未找到任何网络"
#define D_REFRESH_TO_SCAN_AGAIN "刷新并重试"
#define D_DUPLICATE_ACCESSPOINT "重复AP"
#define D_SKIPPING_LOW_QUALITY "忽略弱信号网络"
#define D_RSSI "RSSI"
#define D_WEP "WEP"
#define D_WPA_PSK "WPA PSK"
#define D_WPA2_PSK "WPA2 PSK"
#define D_AP1_SSID "AP1 名称"
#define D_AP1_PASSWORD "AP1 密码"
#define D_AP2_SSID "AP2 名称"
#define D_AP2_PASSWORD "AP2 密码"

#define D_MQTT_PARAMETERS "MQTT设置"
#define D_CLIENT "客户端"
#define D_FULL_TOPIC "完整主题"

#define D_LOGGING_PARAMETERS "日志设置"
#define D_SERIAL_LOG_LEVEL "串口日志级别"
#define D_WEB_LOG_LEVEL "Web 日志级别"
#define D_SYS_LOG_LEVEL "Syslog 日志级别"
#define D_MORE_DEBUG "全部调试"
#define D_SYSLOG_HOST "Syslog 主机地址"
#define D_SYSLOG_PORT "Syslog 端口"
#define D_TELEMETRY_PERIOD "上报周期"

#define D_OTHER_PARAMETERS "其他设置"
#define D_WEB_ADMIN_PASSWORD "WEB管理密码"
#define D_MQTT_ENABLE "启用MQTT"
#define D_FRIENDLY_NAME "昵称"
#define D_BELKIN_WEMO "贝尔金 WeMo"
#define D_HUE_BRIDGE "飞利浦 Hue 网桥"
#define D_SINGLE_DEVICE "单设备"
#define D_MULTI_DEVICE "多设备"

#define D_SAVE_CONFIGURATION "保存设置"
#define D_CONFIGURATION_SAVED "设置已保存"
#define D_CONFIGURATION_RESET "设置已重置"

#define D_PROGRAM_VERSION "固件版本"
#define D_BUILD_DATE_AND_TIME "编译时间"
#define D_CORE_AND_SDK_VERSION "内核版本"
#define D_FLASH_WRITE_COUNT "刷机次数"
#define D_MAC_ADDRESS "MAC地址"
#define D_MQTT_HOST "MQTT主机"
#define D_MQTT_PORT "MQTT端口"
#define D_MQTT_CLIENT "MQTT客户端"
#define D_MQTT_USER "MQTT用户名"
#define D_MQTT_TOPIC "MQTT 主题(Topic)"
#define D_MQTT_GROUP_TOPIC "MQTT 主题组(Group Topic)"
#define D_MQTT_FULL_TOPIC "MQTT 完整主题(Full Topic)"
#define D_MDNS_DISCOVERY "mDNS 发现"
#define D_MDNS_ADVERTISE "mDNS 广播"
#define D_ESP_CHIP_ID "ESP芯片ID"
#define D_FLASH_CHIP_ID "Flash芯片ID"
#define D_FLASH_CHIP_SIZE "Flash大小"
#define D_FREE_PROGRAM_SPACE "空闲程序空间"

#define D_UPGRADE_BY_WEBSERVER "通过Web升级"
#define D_OTA_URL "OTA地址"
#define D_START_UPGRADE "开始升级"
#define D_UPGRADE_BY_FILE_UPLOAD "通过文件升级"
#define D_UPLOAD_STARTED "开始上传"
#define D_UPGRADE_STARTED "开始升级"
#define D_UPLOAD_DONE "上传完成"
#define D_UPLOAD_ERR_1 "没有选择文件"
#define D_UPLOAD_ERR_2 "没有足够空间"
#define D_UPLOAD_ERR_3 "固件头不是 0xE9"
#define D_UPLOAD_ERR_4 "固件太大"
#define D_UPLOAD_ERR_5 "上传缓冲区不匹配"
#define D_UPLOAD_ERR_6 "上传失败。 启用日志记录 3"
#define D_UPLOAD_ERR_7 "上传取消"
#define D_UPLOAD_ERR_8 "错误的固件"
#define D_UPLOAD_ERR_9 "固件太大"
#define D_UPLOAD_ERROR_CODE "上传错误代码"

#define D_ENTER_COMMAND "输入命令"
#define D_ENABLE_WEBLOG_FOR_RESPONSE "如果预期响应，则启用Weblog 2"
#define D_NEED_USER_AND_PASSWORD "需要 user=<用户名>&password=<密码>"

// xdrv_wemohue.ino
#define D_MULTICAST_DISABLED "组播已禁用"
#define D_MULTICAST_REJOINED "组播已(重新)加入"
#define D_MULTICAST_JOIN_FAILED "组播加入失败"
#define D_FAILED_TO_SEND_RESPONSE "请求发送失败"

#define D_WEMO "WeMo"
#define D_WEMO_BASIC_EVENT "WeMo 基础事件"
#define D_WEMO_EVENT_SERVICE "WeMo 事件服务"
#define D_WEMO_META_SERVICE "WeMo meta 服务"
#define D_WEMO_SETUP "WeMo 设置"
#define D_RESPONSE_SENT "发送请求"

#define D_HUE "Hue"
#define D_HUE_BRIDGE_SETUP "Hue 设置"
#define D_HUE_API_NOT_IMPLEMENTED "Hue API 未实现"
#define D_HUE_API "Hue API"
#define D_HUE_POST_ARGS "Hue POST 参数"
#define D_3_RESPONSE_PACKETS_SENT "3 请求包发送"

// xdrv_05_domoticz.ino
#define D_DOMOTICZ_PARAMETERS "Domoticz 设置"
#define D_DOMOTICZ_IDX "Idx"
#define D_DOMOTICZ_KEY_IDX "Key idx"
#define D_DOMOTICZ_SWITCH_IDX "开关 idx"
#define D_DOMOTICZ_SENSOR_IDX "传感器 idx"
  #define D_DOMOTICZ_TEMP "温度"
  #define D_DOMOTICZ_TEMP_HUM "温度,湿度"
  #define D_DOMOTICZ_TEMP_HUM_BARO "温度,湿度,气压"
  #define D_DOMOTICZ_POWER_ENERGY "功率,电量"
  #define D_DOMOTICZ_ILLUMINANCE "照度"
  #define D_DOMOTICZ_COUNT "数值"
  #define D_DOMOTICZ_VOLTAGE "电压"
  #define D_DOMOTICZ_CURRENT "电流"
  #define D_DOMOTICZ_AIRQUALITY "空气质量"
#define D_DOMOTICZ_UPDATE_TIMER "更新计时器"

// xdrv_03_energy.ino
#define D_ENERGY_TODAY "今日用电量"
#define D_ENERGY_YESTERDAY "昨日用电量"
#define D_ENERGY_TOTAL "总用电量"

// xsns_05_ds18b20.ino
#define D_SENSOR_BUSY "传感器正忙"
#define D_SENSOR_CRC_ERROR "传感器 CRC 校验错误"
#define D_SENSORS_FOUND "发现传感器"

// xsns_06_dht.ino
#define D_TIMEOUT_WAITING_FOR "等待超时"
#define D_START_SIGNAL_LOW "开始低电平"
#define D_START_SIGNAL_HIGH "开始高电平"
#define D_PULSE "pulse"
#define D_CHECKSUM_FAILURE "校验失败"

// xsns_07_sht1x.ino
#define D_SENSOR_DID_NOT_ACK_COMMAND "传感器没有确认命令"
#define D_SHT1X_FOUND "发现 SHT1X 传感器"

// xsns_18_pms5003.ino
#define D_STANDARD_CONCENTRATION "CF-1 PM"     // Standard Particle CF-1 Particle Matter
#define D_ENVIRONMENTAL_CONCENTRATION "PM"     // Environmetal Particle Matter
#define D_PARTICALS_BEYOND "颗粒物直径大于"

// sonoff_template.h
#define D_SENSOR_NONE     "None"
#define D_SENSOR_DHT11    "DHT11"
#define D_SENSOR_AM2301   "AM2301"
#define D_SENSOR_SI7021   "SI7021"
#define D_SENSOR_DS18X20  "DS18x20"
#define D_SENSOR_I2C_SCL  "I2C SCL"
#define D_SENSOR_I2C_SDA  "I2C SDA"
#define D_SENSOR_WS2812   "WS2812"
#define D_SENSOR_IRSEND   "IRsend"
#define D_SENSOR_SWITCH   "Switch"   // Suffix "1"
#define D_SENSOR_BUTTON   "Button"   // Suffix "1"
#define D_SENSOR_RELAY    "Relay"    // Suffix "1i"
#define D_SENSOR_LED      "Led"      // Suffix "1i"
#define D_SENSOR_PWM      "PWM"      // Suffix "1"
#define D_SENSOR_COUNTER  "Counter"  // Suffix "1"
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

// Units
#define D_UNIT_AMPERE "安"
#define D_UNIT_HOUR "时"
#define D_UNIT_KILOOHM "千欧"
#define D_UNIT_KILOWATTHOUR "千瓦时"
#define D_UNIT_LUX "勒克斯"
#define D_UNIT_MICROGRAM_PER_CUBIC_METER "微克/立方米"
#define D_UNIT_MICROMETER "微米"
#define D_UNIT_MICROSECOND "微秒"
#define D_UNIT_MILLIAMPERE "毫安"
#define D_UNIT_MILLISECOND "毫秒"
#define D_UNIT_MINUTE "分"
#define D_UNIT_PARTS_PER_DECILITER "每分升"
#define D_UNIT_PARTS_PER_MILLION "ppm"
#define D_UNIT_PRESSURE "百帕"
#define D_UNIT_SECOND "秒"
#define D_UNIT_SECTORS "扇区"
#define D_UNIT_VOLT "伏"
#define D_UNIT_WATT "瓦"
#define D_UNIT_WATTHOUR "瓦时"

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

#endif  // _LANGUAGE_ZH_CN_H_
