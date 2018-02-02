/*
  xsns_16PZEM004T.ino - PZEM004T power monitor

  Copyright (C) 2018  Palich2000

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

#ifdef USE_SERIAL
#ifdef USE_PZEM_004T
/*********************************************************************************************\
 * Peacefair PZEM-004T Energy monitor
 *
 *
 * https://github.com/olehs/PZEM004T
\*********************************************************************************************/

#include "PZEM004T.h"

IPAddress PZEM004T_ip(192, 168, 1, 1);
PZEM004T * pzem = NULL;

bool PZEM004TInit() {
    AddLog_P(LOG_LEVEL_INFO, __FUNCTION__);
    return(serial_flg != 0);
}

boolean PZEM004TPrep() {
    AddLog_P(LOG_LEVEL_INFO, __FUNCTION__);

    if (!pzem) {
        pzem = new PZEM004T(pin[GPIO_SERIAL_RX], pin[GPIO_SERIAL_TX]);
    }
    return(serial_flg != 0);
}

void PZEM004TShowJSON() {
    AddLog_P(LOG_LEVEL_INFO, __FUNCTION__);
    if ((pzem) && (serial_flg != 0)) {
        float r;
        static float voltage = 0.0;
        static float current = 0.0;
        static float power = 0.0;
        static float energy = 0.0;

        char svoltage[10];
        char scurrent[10];
        char spower[10];
        char senergy[10];

        r = pzem->voltage(PZEM004T_ip);
        if (r >= 0) {
            voltage = r;
        }

        r = pzem->current(PZEM004T_ip);
        if (r >= 0) {
            current = r;
        }

        r = pzem->power(PZEM004T_ip);
        if (r >= 0) {
            power = r;
        }

        r = pzem->energy(PZEM004T_ip);
        if (r >= 0) {
            energy = r;
        }

        dtostrfd(energy, Settings.flag2.energy_resolution, senergy);
        dtostrfd(power, Settings.flag2.wattage_resolution, spower);
        dtostrfd(voltage, Settings.flag2.voltage_resolution, svoltage);
        dtostrfd(current, Settings.flag2.current_resolution, scurrent);

        snprintf_P(mqtt_data, sizeof(mqtt_data),
                   PSTR("%s,\"PZEM004T\":{\"" D_TOTAL "\":%s,\"" D_POWERUSAGE "\":%s,\"" D_VOLTAGE "\":%s,\"" D_CURRENT "\":%s}"),
                   mqtt_data, senergy, spower, svoltage, scurrent);
    }
}

#ifdef MQTT_USE
void PZEM004TShowMQTT() {
    AddLog_P(LOG_LEVEL_INFO, __FUNCTION__);
}
#endif //  MQTT_USE

#ifdef USE_WEBSERVER
const char HTTP_ENERGY_PZEM004T[] PROGMEM =
    "{s}" D_VOLTAGE "{m}%s " D_UNIT_VOLT "{e}"
    "{s}" D_CURRENT "{m}%s " D_UNIT_AMPERE "{e}"
    "{s}" D_POWERUSAGE "{m}%s " D_UNIT_WATT "{e}"
    "{s}" D_POWER_FACTOR "{m}%s{e}"
    "{s}" D_ENERGY_TODAY  "{m}%s " D_UNIT_KILOWATTHOUR "{e}"
    "{s}" D_ENERGY_YESTERDAY "{m}%s " D_UNIT_KILOWATTHOUR "{e}"
    "{s}" D_ENERGY_TOTAL "{m}%s " D_UNIT_KILOWATTHOUR "{e}";      // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

#ifdef USE_WEBSERVER
void PZEM004TShowWEB() {
    AddLog_P(LOG_LEVEL_INFO, __FUNCTION__);
    if ((serial_flg != 0)) {
        char svoltage[10] = "0.0";
        char scurrent[10] = "0.0";
        char swatts[10] = "0.0";
        char spower_factor[10] = "0.0";
        char sdaily_energy[10] = "0.0";
        char syesterday_energy[10] = "0.0";
        char stotal_energy[10] = "0.0";
        snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_ENERGY_PZEM004T, svoltage, scurrent, swatts, spower_factor, sdaily_energy, syesterday_energy, stotal_energy);
    }
}
#endif  // USE_WEBSERVER



/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_16

boolean Xsns16(byte function) {
    boolean result = false;
    if (serial_flg) {
        switch (function) {
        case FUNC_XSNS_INIT:
            result = PZEM004TInit();
            break;
        case FUNC_XSNS_PREP:
            result = PZEM004TPrep();
            break;
        case FUNC_XSNS_JSON_APPEND:
            PZEM004TShowJSON();
            break;
#ifdef MQTT_USE
        case FUNC_XSNS_MQTT_SHOW:
            PZEM004TShowMQTT();
            break;
#endif //MQTT_USE
#ifdef USE_WEBSERVER
        case FUNC_XSNS_WEB:
            PZEM004TShowWEB();
            break;
#endif  // USE_WEBSERVER
        }
    }
    return result;
}

#endif  // USE_PZEM_004T
#endif  // USE_SERIAL
