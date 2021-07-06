/*
  xsns_75_prometheus.ino - Web based information for Tasmota

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

#ifdef USE_PROMETHEUS
/*********************************************************************************************\
 * Prometheus support
\*********************************************************************************************/

#define XSNS_75                    75

const char *UnitfromType(const char *type)  // find unit for measurment type
{
  if (strcmp(type, "time") == 0) {
    return "seconds";
  }
  if (strcmp(type, "temperature") == 0 || strcmp(type, "dewpoint") == 0) {
    return "celsius";
  }
  if (strcmp(type, "pressure") == 0) {
    return "hpa";
  }
  if (strcmp(type, "voltage") == 0) {
    return "volts";
  }
  if (strcmp(type, "current") == 0) {
    return "amperes";
  }
  if (strcmp(type, "mass") == 0) {
    return "grams";
  }
  if (strcmp(type, "carbondioxide") == 0) {
    return "ppm";
  }
  if (strcmp(type, "humidity") == 0) {
    return "percentage";
  }
  if (strcmp(type, "id") == 0) {
    return "untyped";
  }
  return "";
}

String FormatMetricName(const char *metric) {  // cleanup spaces and uppercases for Prmetheus metrics conventions
  String formatted = metric;
  formatted.toLowerCase();
  formatted.replace(" ", "_");
  formatted.replace(".", "_");
  return formatted;
}

void HandleMetrics(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP "Prometheus"));

  WSContentBegin(200, CT_PLAIN);

  char parameter[FLOATSZ];

  // Pseudo-metric providing metadata about the running firmware version.
  WSContentSend_P(PSTR("# TYPE tasmota_info gauge\ntasmota_info{version=\"%s\",image=\"%s\",build_timestamp=\"%s\"} 1\n"),
                  TasmotaGlobal.version, TasmotaGlobal.image_name, GetBuildDateAndTime().c_str());
  WSContentSend_P(PSTR("# TYPE tasmota_uptime_seconds gauge\ntasmota_uptime_seconds %d\n"), TasmotaGlobal.uptime);
  WSContentSend_P(PSTR("# TYPE tasmota_boot_count counter\ntasmota_boot_count %d\n"), Settings->bootcount);
  WSContentSend_P(PSTR("# TYPE tasmota_flash_writes_total counter\ntasmota_flash_writes_total %d\n"), Settings->save_flag);


  // Pseudo-metric providing metadata about the WiFi station.
  WSContentSend_P(PSTR("# TYPE tasmota_wifi_station_info gauge\ntasmota_wifi_station_info{bssid=\"%s\",ssid=\"%s\"} 1\n"), WiFi.BSSIDstr().c_str(), WiFi.SSID().c_str());

  // Wi-Fi Signal strength
  WSContentSend_P(PSTR("# TYPE tasmota_wifi_station_signal_dbm gauge\ntasmota_wifi_station_signal_dbm{mac_address=\"%s\"} %d\n"), WiFi.BSSIDstr().c_str(), WiFi.RSSI());

  if (!isnan(TasmotaGlobal.temperature_celsius)) {
    dtostrfd(TasmotaGlobal.temperature_celsius, Settings->flag2.temperature_resolution, parameter);
    WSContentSend_P(PSTR("# TYPE tasmota_global_temperature_celsius gauge\ntasmota_global_temperature_celsius %s\n"), parameter);
  }
  if (TasmotaGlobal.humidity != 0) {
    dtostrfd(TasmotaGlobal.humidity, Settings->flag2.humidity_resolution, parameter);
    WSContentSend_P(PSTR("# TYPE tasmota_global_humidity gauge\ntasmota_global_humidity_percentage %s\n"), parameter);
  }
  if (TasmotaGlobal.pressure_hpa != 0) {
    dtostrfd(TasmotaGlobal.pressure_hpa, Settings->flag2.pressure_resolution, parameter);
    WSContentSend_P(PSTR("# TYPE tasmota_global_pressure_hpa gauge\ntasmota_global_pressure_hpa %s\n"), parameter);
  }

  // Pseudo-metric providing metadata about the free memory.
  #ifdef ESP32
    int32_t freeMaxMem = 100 - (int32_t)(ESP_getMaxAllocHeap() * 100 / ESP_getFreeHeap());
    WSContentSend_PD(PSTR("# TYPE tasmota_memory_bytes gauge\ntasmota_memory_bytes{memory=\"Ram\"} %d\n"), ESP_getFreeHeap());
    WSContentSend_PD(PSTR("# TYPE tasmota_memory_ratio gauge\ntasmota_memory_ratio{memory=\"Fragmentation\"} %d)"), freeMaxMem / 100);
    if (psramFound()) {
      WSContentSend_P(PSTR("# TYPE tasmota_memory_bytes gauge\ntasmota_memory_bytes{memory=\"Psram\"} %d\n"), ESP.getFreePsram() );
    }
  #else // ESP32
    WSContentSend_PD(PSTR("# TYPE tasmota_memory_bytes gauge\ntasmota_memory_bytes{memory=\"ram\"} %d\n"), ESP_getFreeHeap());
  #endif // ESP32

#ifdef USE_ENERGY_SENSOR
  dtostrfd(Energy.voltage[0], Settings->flag2.voltage_resolution, parameter);
  WSContentSend_P(PSTR("# TYPE energy_voltage_volts gauge\nenergy_voltage_volts %s\n"), parameter);
  dtostrfd(Energy.current[0], Settings->flag2.current_resolution, parameter);
  WSContentSend_P(PSTR("# TYPE energy_current_amperes gauge\nenergy_current_amperes %s\n"), parameter);
  dtostrfd(Energy.active_power[0], Settings->flag2.wattage_resolution, parameter);
  WSContentSend_P(PSTR("# TYPE energy_power_active_watts gauge\nenergy_power_active_watts %s\n"), parameter);
  dtostrfd(Energy.daily, Settings->flag2.energy_resolution, parameter);
  WSContentSend_P(PSTR("# TYPE energy_power_kilowatts_daily counter\nenergy_power_kilowatts_daily %s\n"), parameter);
  dtostrfd(Energy.total, Settings->flag2.energy_resolution, parameter);
  WSContentSend_P(PSTR("# TYPE energy_power_kilowatts_total counter\nenergy_power_kilowatts_total %s\n"), parameter);
#endif

  for (uint32_t device = 0; device < TasmotaGlobal.devices_present; device++) {
    power_t mask = 1 << device;
    WSContentSend_P(PSTR("# TYPE relay%d_state gauge\nrelay%d_state %d\n"), device+1, device+1, (TasmotaGlobal.power & mask));
  }

  ResponseClear();
  MqttShowSensor(); //Pull sensor data
  String jsonStr = TasmotaGlobal.mqtt_data;
  JsonParser parser((char *)jsonStr.c_str());
  JsonParserObject root = parser.getRootObject();
  if (root) { // did JSON parsing went ok?
    for (auto key1 : root) {
      JsonParserToken value1 = key1.getValue();
      if (value1.isObject()) {
        JsonParserObject Object2 = value1.getObject();
        for (auto key2 : Object2) {
          JsonParserToken value2 = key2.getValue();
          if (value2.isObject()) {
            JsonParserObject Object3 = value2.getObject();
            for (auto key3 : Object3) {
              const char *value = key3.getValue().getStr(nullptr);
              if (value != nullptr && isdigit(value[0])) {
                String sensor = FormatMetricName(key2.getStr());
                String type = FormatMetricName(key3.getStr());
                const char *unit = UnitfromType(type.c_str());                     //grab base unit corresponding to type
                WSContentSend_P(PSTR("# TYPE tasmota_sensors_%s_%s gauge\ntasmota_sensors_%s_%s{sensor=\"%s\"} %s\n"),
                  type.c_str(), unit, type.c_str(), unit, sensor.c_str(), value);  //build metric as "# TYPE tasmota_sensors_%type%_%unit% gauge\ntasmotasensors_%type%_%unit%{sensor=%sensor%"} %value%""
              }
            }
          } else {
            const char *value = value2.getStr(nullptr);
            if (value != nullptr && isdigit(value[0])) {
              String sensor = FormatMetricName(key1.getStr());
              String type = FormatMetricName(key2.getStr());
              const char *unit = UnitfromType(type.c_str());
              if (strcmp(type.c_str(), "totalstarttime") != 0) {  // this metric causes prometheus of fail
                if (strcmp(type.c_str(), "id") == 0) {            // this metric is NaN, so convert it to a label, see Wi-Fi metrics above
                  WSContentSend_P(PSTR("# TYPE tasmota_sensors_%s_%s gauge\ntasmota_sensors_%s_%s{sensor=\"%s\",id=\"%s\"} 1\n"),
                    type.c_str(), unit, type.c_str(), unit, sensor.c_str(), value);
                } else {
                  WSContentSend_P(PSTR("# TYPE tasmota_sensors_%s_%s gauge\ntasmota_sensors_%s_%s{sensor=\"%s\"} %s\n"),
                    type.c_str(), unit, type.c_str(), unit, sensor.c_str(), value);
                }
              }
            }
          }
        }
      } else {
        const char *value = value1.getStr(nullptr);
        String sensor = FormatMetricName(key1.getStr());
        if (value != nullptr && isdigit(value[0] && strcmp(sensor.c_str(), "time") != 0)) {  //remove false 'time' metric
          WSContentSend_P(PSTR("# TYPE tasmota_sensors_%s gauge\ntasmota_sensors{sensor=\"%s\"} %s\n"), sensor.c_str(), sensor.c_str(), value);
        }
      }
    }
  }

  WSContentEnd();
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns75(uint8_t function) {
  bool result = false;

  switch (function) {
    case FUNC_WEB_ADD_HANDLER:
      WebServer_on(PSTR("/metrics"), HandleMetrics);
      break;
  }
  return result;
}

#endif  // USE_PROMETHEUS
