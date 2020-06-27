/*
  xsns_91_prometheus.ino - Web based information for Tasmota

  Copyright (C) 2020  Theo Arends

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

#define XSNS_91                    91

void HandleMetrics(void)
{
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, PSTR("Prometheus"));

  WSContentBegin(200, CT_PLAIN);


  char parameter[FLOATSZ];

  WSContentSend_P(PSTR("# TYPE tasmota_uptime_seconds gauge\ntasmota_uptime_seconds %d\n"), uptime);

  if (!isnan(global_temperature)) {
    dtostrfd(global_temperature, Settings.flag2.temperature_resolution, parameter);
    WSContentSend_P(PSTR("# TYPE global_temperature gauge\nglobal_temperature %s\n"), parameter);
  }
  if (global_humidity != 0) {
    dtostrfd(global_humidity, Settings.flag2.humidity_resolution, parameter);
    WSContentSend_P(PSTR("# TYPE global_humidity gauge\nglobal_humidity %s\n"), parameter);
  }
  if (global_pressure != 0) {
    dtostrfd(global_pressure, Settings.flag2.pressure_resolution, parameter);
    WSContentSend_P(PSTR("# TYPE global_pressure gauge\nglobal_pressure %s\n"), parameter);
  }

#ifdef USE_ENERGY_SENSOR
  dtostrfd(Energy.voltage[0], Settings.flag2.voltage_resolution, parameter);
  WSContentSend_P(PSTR("# TYPE voltage gauge\nvoltage %s\n"), parameter);
  dtostrfd(Energy.current[0], Settings.flag2.current_resolution, parameter);
  WSContentSend_P(PSTR("# TYPE current gauge\ncurrent %s\n"), parameter);
  dtostrfd(Energy.active_power[0], Settings.flag2.wattage_resolution, parameter);
  WSContentSend_P(PSTR("# TYPE active_power gauge\nactive_power %s\n"), parameter);
  dtostrfd(Energy.daily, Settings.flag2.energy_resolution, parameter);
  WSContentSend_P(PSTR("# TYPE energy_daily gauge\nenergy_daily %s\n"), parameter);
  dtostrfd(Energy.total, Settings.flag2.energy_resolution, parameter);
  WSContentSend_P(PSTR("# TYPE energy_total counter\nenergy_total %s\n"), parameter);
#endif

/*
  // Alternative method using the complete sensor JSON data
  // For prometheus it may need to be decoded to # TYPE messages
  mqtt_data[0] = '\0';
  MqttShowSensor();
  char json[strlen(mqtt_data) +1];
  snprintf_P(json, sizeof(json), mqtt_data);

  // Do your Prometheus specific processing here.
  // Look at function DisplayAnalyzeJson() in file xdrv_13_display.ino as an example how to decode the JSON message

  WSContentSend_P(json);
*/

  WSContentEnd();
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns91(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_WEB_ADD_HANDLER:
      Webserver->on("/metrics", HandleMetrics);
      break;
  }
  return result;
}

#endif  // USE_PROMETHEUS
