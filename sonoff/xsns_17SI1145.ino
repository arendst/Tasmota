/*
  xsns_17SI1145.ino - SI1145 sunlight sensor

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

#ifdef USE_SI1145
/*********************************************************************************************\
 * Sunlight Sensor  http://wiki.seeed.cc/Grove-Sunlight_Sensor/
 *
 *
 * library https://github.com/Seeed-Studio/Grove_Sunlight_Sensor
\*********************************************************************************************/

#include "SI114X.h"

SI114X * SI1145 = NULL;

bool search_and_init_SI1145() {
    byte address = 0x60; // Default SI1145 address
    byte error;
    static bool complete_flag = false;

    if (complete_flag) return(NULL != SI1145);

    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (0 == error) {
        snprintf_P(log_data, sizeof(log_data), PSTR(" SI1145 sensor found at: 0x%02x"), address);
        AddLog(LOG_LEVEL_INFO);
	SI1145 = new SI114X();
	if (SI1145) {
	    if (!SI1145->Begin()) {
		snprintf_P(log_data, sizeof(log_data), PSTR(" SI1145 sensor not ready."));
		AddLog(LOG_LEVEL_ERROR);
	    }
	    else {
		snprintf_P(log_data, sizeof(log_data), PSTR(" SI1145 sensor ready."));
		AddLog(LOG_LEVEL_INFO);
	    }
	}
    } else {
        snprintf_P(log_data, sizeof(log_data), PSTR(" SI1145 sensor not found at: 0x%02x"), address);
        AddLog(LOG_LEVEL_ERROR);
    }
    complete_flag = true;
    return(NULL != SI1145);
}

bool SI1145Init() {
    return(true);
//    return(search_and_init_SI1145());
}

boolean SI1145Prep() {
    return(search_and_init_SI1145());
}

bool SI1145Read(float * visible, float * IR, float * UV) {
    if (SI1145) {
	if (visible) *visible = SI1145->ReadVisible();
	if (SI1145->GetTransmissionError()) {
	    snprintf_P(log_data, sizeof(log_data), PSTR(" Error reading Visible: %d"), SI1145->GetTransmissionError());
	    AddLog(LOG_LEVEL_ERROR);
	}
	if (IR) *IR = SI1145->ReadIR();
	if (SI1145->GetTransmissionError()) {
	    snprintf_P(log_data, sizeof(log_data), PSTR(" Error reading IR: %d"), SI1145->GetTransmissionError());
	    AddLog(LOG_LEVEL_ERROR);
	}
	if (UV) *UV =(float)SI1145->ReadUV()/100.0;
	if (SI1145->GetTransmissionError()) {
	    snprintf_P(log_data, sizeof(log_data), PSTR(" Error reading UV: %d"), SI1145->GetTransmissionError());
	    AddLog(LOG_LEVEL_ERROR);
	}
	return(true);
    }
    return(false);
}

void SI1145ShowJSON() {
    if (SI1145) {
        float visible;
        float IR;
        float UV;

        if (SI1145Read(&visible, &IR, &UV)) {
            char svisible[10];
            char sIR[10];
            char sUV[10];

            dtostrfd(visible, 2, svisible);
            dtostrfd(IR, 2, sIR);
            dtostrfd(UV, 4, sUV);

            snprintf_P(mqtt_data, sizeof(mqtt_data),
                    PSTR("%s,\"SI1145\":{\"" "Light" "\":%s,\"" "IR" "\":%s,\"" "UV" "\":%s}"),
                    mqtt_data, svisible, sIR, sUV);
        }
    }
}

#ifdef MQTT_USE
void SI1145ShowMQTT() {
}
#endif //  MQTT_USE

#ifdef USE_WEBSERVER
const char HTTP_SI1145[] PROGMEM =
    "%s"
    "{s} Light {m}%s lum {e}"
    "{s} IR    {m}%s lum {e}"
    "{s} UV    {m}%s lum {e}";      // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

#ifdef USE_WEBSERVER
void SI1145ShowWEB() {
    if (SI1145) {
        float visible;
        float IR;
        float UV;

        if (SI1145Read(&visible, &IR, &UV)) {
            char svisible[10];
            char sIR[10];
            char sUV[10];

            dtostrfd(visible, 2, svisible);
            dtostrfd(IR, 2, sIR);
            dtostrfd(UV, 4, sUV);

            snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SI1145, mqtt_data,  svisible, sIR, sUV);
        }
    }
}
#endif  // USE_WEBSERVER



/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_17

boolean Xsns17(byte function) {
    boolean result = false;
        switch (function) {
        case FUNC_XSNS_INIT:
            result = SI1145Init();
            break;
        case FUNC_XSNS_PREP:
            result = SI1145Prep();
            break;
        case FUNC_XSNS_JSON_APPEND:
            SI1145ShowJSON();
            break;
#ifdef MQTT_USE
        case FUNC_XSNS_MQTT_SHOW:
            SI1145ShowMQTT();
            break;
#endif //MQTT_USE
#ifdef USE_WEBSERVER
        case FUNC_XSNS_WEB:
            SI1145ShowWEB();
            break;
#endif  // USE_WEBSERVER
        }
    return result;
}

#endif  // USE_SI1145
