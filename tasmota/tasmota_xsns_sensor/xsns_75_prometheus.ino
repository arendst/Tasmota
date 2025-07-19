/*
  xsns_75_prometheus.ino - Web based information for Tasmota

  Copyright (C) 2021  @marius, @mhansen, @hansmi

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
 *
 * The text format for metrics, labels and values is documented at [1]. Only
 * the UTF-8 text encoding is supported. [2] describes how metrics and labels
 * should be named.
 *
 * [1]
 * https://github.com/prometheus/docs/blob/master/content/docs/instrumenting/exposition_formats.md
 * [2]
 * https://github.com/prometheus/docs/blob/master/content/docs/practices/naming.md
 *
\*********************************************************************************************/

#define XSNS_75                    75

// Find appropriate unit for measurement type.
const char *UnitfromType(const char *type)
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

// Replace spaces and periods in metric name to match Prometheus metrics
// convention.
String FormatMetricName(const char *metric) {
  String formatted = metric;
  formatted.toLowerCase();
  formatted.replace(" ", "_");
  formatted.replace(".", "_");
  return formatted;
}

const uint8_t
  kPromMetricGauge = _BV(0),
  kPromMetricCounter = _BV(1),
  kPromMetricTypeMask = kPromMetricGauge | kPromMetricCounter;

// Format and send a Prometheus metric to the client. Use flags to configure
// the type. Labels must be supplied in tuples of two character array pointers
// and terminated by nullptr.
void WritePromMetric(const char *name, uint8_t flags, const char *value, va_list labels) {
  PGM_P const prefix = PSTR("tasmota_");
  PGM_P tmp;
  String lval;

  switch (flags & kPromMetricTypeMask) {
  case kPromMetricGauge:
    tmp = PSTR("gauge");
    break;
  case kPromMetricCounter:
    tmp = PSTR("counter");
    break;
  default:
    tmp = nullptr;
    break;
  }

  if (tmp != nullptr) {
    WSContentSend_P(PSTR("# TYPE %s%s %s\n"), prefix, name, tmp);
  }

  WSContentSend_P(PSTR("%s%s{"), prefix, name);

  for (const char *sep = PSTR(""); ; sep = PSTR(",")) {
    if ((tmp = va_arg(labels, PGM_P)) == nullptr) {
      break;
    }

    // A few label values are stored in PROGMEM. The _P functions, e.g.
    // snprintf_P, support both program and heap/stack memory with the "%s"
    // format on ESP8266/ESP32. Casting the pointer to __FlashStringHelper has
    // the same effect with String::operator=.
    if (!(lval = va_arg(labels, const __FlashStringHelper *))) {
      break;
    }

    // Labels can be any sequence of UTF-8 characters, but backslash,
    // double-quote and line feed must be escaped.
    lval.replace("\\", "\\\\");
    lval.replace("\"", "\\\"");
    lval.replace("\n", "\\n");

    WSContentSend_P(PSTR("%s%s=\"%s\""), sep, tmp, lval.c_str());
  }

  WSContentSend_P(PSTR("} %s\n"), value);
}

void WritePromMetricInt32(const char *name, uint8_t flags, const int32_t value, ...) {
  char str[16];

  snprintf_P(str, sizeof(str), PSTR("%d"), value);

  va_list labels;
  va_start(labels, value);
  WritePromMetric(name, flags, str, labels);
  va_end(labels);
}

void WritePromMetricDec(const char *name, uint8_t flags, double number, unsigned char prec, ...) {
  char value[FLOATSZ];

  // Prometheus always requires "." as the decimal separator.
  dtostrfd(number, prec, value);

  va_list labels;
  va_start(labels, prec);
  WritePromMetric(name, flags, value, labels);
  va_end(labels);
}

void WritePromMetricStr(const char *name, uint8_t flags, const char *value, ...) {
  va_list labels;
  va_start(labels, value);
  WritePromMetric(name, flags, value, labels);
  va_end(labels);
}

// Sentinel value for unknown memory metrics, chosen to unlikely match actual
// values.
const uint32_t kPromMemoryUnknown = 0xFFFFFFFF - 1;

// Write metrics providing information about used and available memory.
void WritePromMemoryMetrics(const char *type, uint32_t size, uint32_t avail, uint32_t max_alloc) {
  if (size != kPromMemoryUnknown) {
    WritePromMetricInt32(PSTR("memory_size_bytes"), kPromMetricGauge, size,
      PSTR("memory"), type, nullptr);
  }

  WritePromMetricInt32(PSTR("memory_free_bytes"), kPromMetricGauge, avail,
    PSTR("memory"), type, nullptr);

  if (max_alloc != kPromMemoryUnknown) {
    // The largest contiguous free memory block, useful for checking
    // fragmentation.
    WritePromMetricInt32(PSTR("memory_max_alloc_bytes"), kPromMetricGauge, max_alloc,
      PSTR("memory"), type, nullptr);
  }
}

void HandleMetrics(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP "Prometheus"));

  WSContentBegin(200, CT_PLAIN);

  char namebuf[64];

  // Pseudo-metric providing metadata about the running firmware version.
  WritePromMetricInt32(PSTR("info"), kPromMetricGauge, 1,
    PSTR("version"), TasmotaGlobal.version,
    PSTR("image"), TasmotaGlobal.image_name,
    PSTR("build_timestamp"), GetBuildDateAndTime().c_str(),
    PSTR("devicename"), SettingsText(SET_DEVICENAME),
    PSTR("friendlyname"), SettingsText(SET_FRIENDLYNAME1),
    nullptr);

  WritePromMetricInt32(PSTR("uptime_seconds"), kPromMetricGauge, TasmotaGlobal.uptime, nullptr);
  WritePromMetricInt32(PSTR("boot_count"), kPromMetricCounter, Settings->bootcount, nullptr);
  WritePromMetricInt32(PSTR("flash_writes_total"), kPromMetricCounter, Settings->save_flag, nullptr);

  // Pseudo-metric providing metadata about the WiFi station.
  WritePromMetricInt32(PSTR("wifi_station_info"), kPromMetricGauge, 1,
    PSTR("bssid"), WiFi.BSSIDstr().c_str(),
    PSTR("ssid"), WiFi.SSID().c_str(),
    nullptr);

  // Wi-Fi Signal strength
  WritePromMetricInt32(PSTR("wifi_station_signal_dbm"), kPromMetricGauge, WiFi.RSSI(),
    PSTR("mac_address"), WiFi.BSSIDstr().c_str(),
    nullptr);

  if (!isnan(TasmotaGlobal.temperature_celsius)) {
    WritePromMetricDec(PSTR("global_temperature_celsius"), kPromMetricGauge,
      TasmotaGlobal.temperature_celsius, Settings->flag2.temperature_resolution,
      nullptr);
  }

  if (TasmotaGlobal.humidity != 0) {
    WritePromMetricDec(PSTR("global_humidity_percentage"), kPromMetricGauge,
      TasmotaGlobal.humidity, Settings->flag2.humidity_resolution,
      nullptr);
  }

  if (TasmotaGlobal.pressure_hpa != 0) {
    WritePromMetricDec(PSTR("global_pressure_hpa"), kPromMetricGauge,
      TasmotaGlobal.pressure_hpa, Settings->flag2.pressure_resolution,
      nullptr);
  }

  WritePromMemoryMetrics(PSTR("heap"),
#ifdef ESP32
    ESP.getHeapSize(),
#else
    kPromMemoryUnknown,
#endif
    ESP_getFreeHeap(),
#ifdef ESP32
    ESP_getMaxAllocHeap()
#else
    kPromMemoryUnknown
#endif
  );

#ifdef ESP32
  if (UsePSRAM()) {
    WritePromMemoryMetrics(PSTR("psram"), ESP.getPsramSize(),
      ESP.getFreePsram(), ESP.getMaxAllocPsram());
  }
#endif

#ifdef USE_ENERGY_SENSOR
  WritePromMetricDec(PSTR("energy_voltage_volts"),
    kPromMetricGauge,
    Energy->voltage[0], Settings->flag2.voltage_resolution, nullptr);
  WritePromMetricDec(PSTR("energy_current_amperes"),
    kPromMetricGauge,
    Energy->current[0], Settings->flag2.current_resolution, nullptr);
  WritePromMetricDec(PSTR("energy_power_active_watts"),
    kPromMetricGauge,
    Energy->active_power[0], Settings->flag2.wattage_resolution, nullptr);
  WritePromMetricDec(PSTR("energy_power_kilowatts_daily"),
    kPromMetricCounter,
    Energy->daily_sum, Settings->flag2.energy_resolution, nullptr);
  WritePromMetricDec(PSTR("energy_power_kilowatts_total"),
    kPromMetricCounter,
    Energy->total_sum, Settings->flag2.energy_resolution, nullptr);
#endif

  for (uint32_t device = 0; device < TasmotaGlobal.devices_present; device++) {
    power_t mask = 1 << device;
    snprintf_P(namebuf, sizeof(namebuf), PSTR("relay%d_state"), device + 1);
    WritePromMetricInt32(namebuf, kPromMetricGauge,
      (TasmotaGlobal.power & mask), nullptr);
  }

  ResponseClear();
  MqttShowSensor(true); //Pull sensor data
  String jsonStr = ResponseData();
  JsonParser parser((char *)jsonStr.c_str());
  JsonParserObject root = parser.getRootObject();
  if (root) { // did JSON parsing succeed?
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
              if (value != nullptr && (isdigit(value[0]) || (value[0] == '-') || (value[0] == '.'))) {
                String sensor = FormatMetricName(key2.getStr());
                String type = FormatMetricName(key3.getStr());

                snprintf_P(namebuf, sizeof(namebuf), PSTR("sensors_%s_%s"),
                  type.c_str(), UnitfromType(type.c_str()));
                WritePromMetricStr(namebuf, kPromMetricGauge, value,
                  PSTR("sensor"), sensor.c_str(),
                  nullptr);
              }
            }
          } else {
            const char *value = value2.getStr(nullptr);
            if (value != nullptr && (isdigit(value[0]) || (value[0] == '-') || (value[0] == '.'))) {
              String sensor = FormatMetricName(key1.getStr());
              String type = FormatMetricName(key2.getStr());
              if (strcmp(type.c_str(), "totalstarttime") != 0) {  // this metric causes Prometheus of fail
                snprintf_P(namebuf, sizeof(namebuf), PSTR("sensors_%s_%s"),
                  type.c_str(), UnitfromType(type.c_str()));

                if (strcmp(type.c_str(), "id") == 0) {            // this metric is NaN, so convert it to a label, see Wi-Fi metrics above
                  WritePromMetricInt32(namebuf, kPromMetricGauge, 1,
                    PSTR("sensor"), sensor.c_str(),
                    PSTR("id"), value,
                    nullptr);
                } else {
                  WritePromMetricStr(namebuf, kPromMetricGauge, value,
                    PSTR("sensor"), sensor.c_str(),
                    nullptr);
                }
              }
            }
          }
        }
      } else {
        const char *value = value1.getStr(nullptr);
        String sensor = FormatMetricName(key1.getStr());

        if (value != nullptr && (isdigit(value[0]) || (value[0] == '-') || (value[0] == '.')) && strcmp(sensor.c_str(), "time") != 0) {  //remove false 'time' metric
          WritePromMetricStr(PSTR("sensors"), kPromMetricGauge, value,
            PSTR("sensor"), sensor.c_str(),
            nullptr);
        }
      }
    }
  }

  WSContentEnd();
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns75(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_WEB_ADD_HANDLER:
      WebServer_on(PSTR("/metrics"), HandleMetrics);
      break;
  }
  return result;
}

#endif  // USE_PROMETHEUS
