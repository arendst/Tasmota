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
 * library https://github.com/olehs/PZEM004T 
\*********************************************************************************************/

#include "PZEM004T.h"

IPAddress PZEM004T_ip(192, 168, 1, 1);
PZEM004T * pzem = NULL;

bool PZEM004TInit() {
    return(PZEM_004T_flg != 0);
}

boolean PZEM004TPrep() {
    static byte pin_GPIO_PZEM004T_RX = 99;
    static byte pin_GPIO_PZEM004T_TX = 99;
    if (PZEM_004T_flg){
        if ((pin_GPIO_PZEM004T_RX != pin[GPIO_PZEM004T_RX]) || (pin_GPIO_PZEM004T_TX != pin[GPIO_PZEM004T_TX])) {
            pin_GPIO_PZEM004T_RX = pin[GPIO_PZEM004T_RX];
            pin_GPIO_PZEM004T_TX = pin[GPIO_PZEM004T_TX];
            if (pzem) {
                delete pzem;
                pzem = NULL;
            }
            // D6 GPIO-12 TX
            // D7 GPIO-13 RX
            snprintf_P(log_data, sizeof(log_data), PSTR(" prepare RX:%d TX:%d"), pin[GPIO_PZEM004T_RX], pin[GPIO_PZEM004T_TX]);
            AddLog(LOG_LEVEL_INFO);
            pzem = new PZEM004T(pin[GPIO_PZEM004T_RX], pin[GPIO_PZEM004T_TX]);
            if (pzem) {
                if (!pzem->setAddress(PZEM004T_ip)) {
                    snprintf_P(log_data, sizeof(log_data), PSTR(" Failed to set address"));
                    AddLog(LOG_LEVEL_INFO);
                }
            }
        }
    }
    return(PZEM_004T_flg != 0);
}

bool PZEM004TRead(float * rvoltage, float * rcurrent, float * rpower, float * renergy) {
    static float voltage = -1;
    static float current = -1;
    static float power = -1;
    static float energy = -1;
    static unsigned long last_get_power = 0;
    if ((pzem) && (PZEM_004T_flg != 0) && (millis() > last_get_power + 5000)) {
        while (1) {
            float r = pzem->voltage(PZEM004T_ip);
            if (r >= 0.0) {
                voltage = r;
            } else {
                //AddLog_P(LOG_LEVEL_INFO, __FUNCTION__,"E1");
                break;
            }
            delay(50);
            r = pzem->current(PZEM004T_ip);
            if (r >= 0.0) {
                current = r;
            } else {
                //AddLog_P(LOG_LEVEL_INFO, __FUNCTION__,"E2");
                //break;
            }
            delay(50);
            r = pzem->power(PZEM004T_ip);
            if (r >= 0.0) {
                power = r;
            } else {
                //AddLog_P(LOG_LEVEL_INFO, __FUNCTION__,"E3");
                //break;
            }
            delay(50);
            r = pzem->energy(PZEM004T_ip);
            if (r >= 0.0) {
                energy = r;
            } else {
                //AddLog_P(LOG_LEVEL_INFO, __FUNCTION__,"E4");
                //break;
            }
            break;
        }
        last_get_power = millis();
    }
    if (rvoltage) *rvoltage = voltage;
    if (rcurrent) *rcurrent = current;
    if (rpower)   *rpower = power;
    if (renergy)  *renergy = energy;
    return (voltage > -1) && (current > -1) && (power > -1) && (energy > -1);
}

void PZEM004TShowJSON() {
    if ((pzem) && (PZEM_004T_flg != 0)) {
        float voltage;
        float current;
        float power;
        float energy;

        if (PZEM004TRead(&voltage, &current, &power, &energy)) {
            char svoltage[10];
            char scurrent[10];
            char spower[10];
            char senergy[10];

            dtostrfd(energy, Settings.flag2.energy_resolution, senergy);
            dtostrfd(power, Settings.flag2.wattage_resolution, spower);
            dtostrfd(voltage, Settings.flag2.voltage_resolution, svoltage);
            dtostrfd(current, Settings.flag2.current_resolution, scurrent);

            snprintf_P(mqtt_data, sizeof(mqtt_data),
                    PSTR("%s,\"PZEM004T\":{\"" D_TOTAL "\":%s,\"" D_POWERUSAGE "\":%s,\"" D_VOLTAGE "\":%s,\"" D_CURRENT "\":%s}"),
                    mqtt_data, senergy, spower, svoltage, scurrent);
        }
    }
}

#ifdef MQTT_USE
void PZEM004TShowMQTT() {
    //AddLog_P(LOG_LEVEL_INFO, __FUNCTION__);
}
#endif //  MQTT_USE

#ifdef USE_WEBSERVER
const char HTTP_ENERGY_PZEM004T[] PROGMEM =
    "%s"
    "{s}" D_VOLTAGE "{m}%s " D_UNIT_VOLT "{e}"
    "{s}" D_CURRENT "{m}%s " D_UNIT_AMPERE "{e}"
    "{s}" D_POWERUSAGE "{m}%s " D_UNIT_WATT "{e}"
    "{s}" D_ENERGY_TOTAL "{m}%s " D_UNIT_KILOWATTHOUR "{e}";      // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

#ifdef USE_WEBSERVER
void PZEM004TShowWEB() {
    if ((pzem) && (PZEM_004T_flg != 0)) {
        float voltage;
        float current;
        float power;
        float energy;

        if (PZEM004TRead(&voltage, &current, &power, &energy)) {
            char svoltage[10];
            char scurrent[10];
            char spower[10];
            char senergy[10];

            dtostrfd(energy, Settings.flag2.energy_resolution, senergy);
            dtostrfd(power, Settings.flag2.wattage_resolution, spower);
            dtostrfd(voltage, Settings.flag2.voltage_resolution, svoltage);
            dtostrfd(current, Settings.flag2.current_resolution, scurrent);
            snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_ENERGY_PZEM004T, mqtt_data, svoltage, scurrent, spower, senergy);
        }
    }
}
#endif  // USE_WEBSERVER



/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_16

boolean Xsns16(byte function) {
    boolean result = false;
    if (PZEM_004T_flg) {
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
