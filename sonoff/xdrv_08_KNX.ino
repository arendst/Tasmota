/*
  xdrv_08_KNX.ino - KNX IP Protocol support for Sonoff-Tasmota

  Copyright (C) 2018  Adrian Scillato

  Based on esp-knx-ip library for KNX/IP communication on an ESP8266
  Author: Nico Weichbrodt <envy>
  Web: https://github.com/envy/esp-knx-ip

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

#ifdef USE_KNX

#include <esp-knx-ip.h>     // Include KNX IP library
                            // esp-knx-ip library modifications:
                            //    esp-knx-ip.h -> Root_prefix "/knx"
                            //                    debug off
                            //    esp-knx-ip-send.cpp -> no checksum
                            //    esp-knx-ip-webserver.cpp -> order and format

address_t physaddr;

void KNXStart()
{
  knx.physical_address_set(knx.PA_to_address(1, 1, 1));  // Set Physical KNX Address of the device
}


void KNXLoop()
{
//  if (Settings.flag.knx_enabled) {
  knx.loop();  // Process knx events
//  }   
}

/*
void KNX_EVERY_SECOND() {
  
}
*/

void KNXSaveSettings()
{
  /*
  char stemp[20];
  char ssensor_indices[6 * MAX_DOMOTICZ_SNS_IDX];
  char tmp[100];

  for (byte i = 0; i < MAX_DOMOTICZ_IDX; i++) {
    snprintf_P(stemp, sizeof(stemp), PSTR("r%d"), i +1);
    WebGetArg(stemp, tmp, sizeof(tmp));
    Settings.domoticz_relay_idx[i] = (!strlen(tmp)) ? 0 : atoi(tmp);
    snprintf_P(stemp, sizeof(stemp), PSTR("k%d"), i +1);
    WebGetArg(stemp, tmp, sizeof(tmp));
    Settings.domoticz_key_idx[i] = (!strlen(tmp)) ? 0 : atoi(tmp);
    snprintf_P(stemp, sizeof(stemp), PSTR("s%d"), i +1);
    WebGetArg(stemp, tmp, sizeof(tmp));
    Settings.domoticz_switch_idx[i] = (!strlen(tmp)) ? 0 : atoi(tmp);
  }
  ssensor_indices[0] = '\0';
  for (byte i = 0; i < DZ_MAX_SENSORS; i++) {
    snprintf_P(stemp, sizeof(stemp), PSTR("l%d"), i +1);
    WebGetArg(stemp, tmp, sizeof(tmp));
    Settings.domoticz_sensor_idx[i] = (!strlen(tmp)) ? 0 : atoi(tmp);
    snprintf_P(ssensor_indices, sizeof(ssensor_indices), PSTR("%s%s%d"), ssensor_indices, (strlen(ssensor_indices)) ? "," : "",  Settings.domoticz_sensor_idx[i]);
  }
  WebGetArg("ut", tmp, sizeof(tmp));
  Settings.domoticz_update_timer = (!strlen(tmp)) ? DOMOTICZ_UPDATE_TIMER : atoi(tmp);

  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DOMOTICZ D_CMND_IDX " %d,%d,%d,%d, " D_CMND_KEYIDX " %d,%d,%d,%d, " D_CMND_SWITCHIDX " %d,%d,%d,%d, " D_CMND_SENSORIDX " %s, " D_CMND_UPDATETIMER " %d"),
    Settings.domoticz_relay_idx[0], Settings.domoticz_relay_idx[1], Settings.domoticz_relay_idx[2], Settings.domoticz_relay_idx[3],
    Settings.domoticz_key_idx[0], Settings.domoticz_key_idx[1], Settings.domoticz_key_idx[2], Settings.domoticz_key_idx[3],
    Settings.domoticz_switch_idx[0], Settings.domoticz_switch_idx[1], Settings.domoticz_switch_idx[2], Settings.domoticz_switch_idx[3],
    ssensor_indices, Settings.domoticz_update_timer);
  AddLog(LOG_LEVEL_INFO);
  */
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_08

boolean Xdrv08(byte function)
{
  boolean result = false;
//  if (Settings.flag.knx_enabled) {
    switch (function) {
      case FUNC_INIT:
        KNXStart();
        break;
      case FUNC_LOOP:
        KNXLoop();
        break;
//      case FUNC_EVERY_SECOND;
//        KNX_EVERY_SECOND();
//        break;                 
//      case FUNC_COMMAND:
//        result = MqttCommand();
//        break;
    }
//  }    
  return result;
}

#endif  // USE_KNX
