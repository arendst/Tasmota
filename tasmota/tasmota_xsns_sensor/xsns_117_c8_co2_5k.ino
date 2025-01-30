/*
  xsns_117_c8_co2_5k.ino - SC8-CO2-5K CO2 sensor support for Tasmota

  Copyright (C) 2025  Jeroen Vermeulen

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

#ifdef USE_C8_CO2_5K
/*********************************************************************************************\
 * SC8-CO2-5K - CO2 sensor
 *
 * Inspired by:
 *   https://community.openenergymonitor.org/uploads/short-url/tDiJ3EWtv7OlcHZnNgX3dFv8Cpv.pdf
 *   https://spezifisch.codeberg.page/posts/2022-08-23/co2-sensor-reverse-engineering/
 *   https://github.com/Jaron0211/C8_master
 \********************************************************************************************/

#define XSNS_117 117

#include <TasmotaSerial.h>

#ifndef CO2_LOW
#define CO2_LOW 800 // Below this CO2 value show green light
#endif
#ifndef CO2_HIGH
#define CO2_HIGH 1200 // Above this CO2 value show red light
#endif

#define C8CO2_READ_TIMEOUT 400
const char c8cp2_type[] = "C8-CO2-5K";

static uint8_t c8co2_buffer[16] = {0};
TasmotaSerial *c8co2_serial = nullptr;
uint16_t c8co2_ppm = 0;

void C8CO2_Init()
{
    if (PinUsed(GPIO_C8_CO2_5K_RX))
    {
        int txPin = PinUsed(GPIO_C8_CO2_5K_TX) ? Pin(GPIO_C8_CO2_5K_TX) : -1;
        c8co2_serial = new TasmotaSerial(Pin(GPIO_C8_CO2_5K_RX), txPin, 1);
        if (c8co2_serial->begin(9600))
        {
            if (c8co2_serial->hardwareSerial())
                ClaimSerial();
#ifdef ESP32
            AddLog(LOG_LEVEL_DEBUG, PSTR("C8-CO2-5K: Serial UART%d"), c8co2_serial->getUart());
#endif
        }
        AddLog(LOG_LEVEL_DEBUG, PSTR("C8-CO2-5K: Sensor serial ready"));
    }
}

void C8CO2_EverySecond()
{
    unsigned long start = millis();
    while ((millis() - start) < C8CO2_READ_TIMEOUT && c8co2_serial->available() > 0)
    {
        c8co2_buffer[0] = c8co2_buffer[1];
        c8co2_buffer[1] = c8co2_serial->read();

        if (c8co2_buffer[0] != 0x42 || c8co2_buffer[1] != 0x4D) // Check for header bytes
            continue;

        if (c8co2_serial->readBytes(&c8co2_buffer[2], 14) != 14)
        {
            AddLog(LOG_LEVEL_INFO, PSTR("C8-CO2-5K: ERROR Incomplete data received"));
            return;
        }

        uint8_t sum = 0;
        for (int i = 0; i < 15; i++)
        {
            sum += c8co2_buffer[i];
        }
        if (sum != c8co2_buffer[15])
        {
            AddLog(LOG_LEVEL_INFO, PSTR("C8-CO2-5K: ERROR Checksum mismatch"));
            return;
        }

        c8co2_ppm = (c8co2_buffer[6] << 8) | c8co2_buffer[7];
        AddLog(LOG_LEVEL_DEBUG, PSTR("C8-CO2-5K Sensor: %d ppm"), c8co2_ppm);
#ifdef USE_LIGHT
        LightSetSignal(CO2_LOW, CO2_HIGH, c8co2_ppm);
#endif // USE_LIGHT
        return;
    }
}

void C8CO2_Show(bool json)
{
    if (json)
    {
        ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_CO2 "\":%d}"), c8cp2_type, c8co2_ppm);
#ifdef USE_DOMOTICZ
        if (0 == TasmotaGlobal.tele_period)
        {
            DomoticzSensor(DZ_AIRQUALITY, c8co2_ppm);
        }
#endif // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    }
    else
    {
        WSContentSend_PD(HTTP_SNS_CO2, c8cp2_type, c8co2_ppm);
#endif // USE_WEBSERVER
    }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns117(uint32_t function)
{
    if (FUNC_INIT == function)
    {
        C8CO2_Init();
    }
    else if (c8co2_serial)
    {
        switch (function)
        {
        case FUNC_EVERY_SECOND:
            C8CO2_EverySecond();
            break;
        case FUNC_JSON_APPEND:
            C8CO2_Show(1);
            break;
#ifdef USE_WEBSERVER
        case FUNC_WEB_SENSOR:
            C8CO2_Show(0);
            break;
#endif // USE_WEBSERVER
        }
    }
    return false;
}

#endif // USE_C8_CO2_5K
