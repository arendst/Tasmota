/*
  xsns_64_hrxl.ino - MaxBotix HRXL serial interface

  Copyright (C) 2021  Jon Little

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

#ifdef USE_HRXL
/*********************************************************************************************\
 * MaxBotix HRXL-MaxSonar ultrasonic range finders
 * Source: https://www.maxbotix.com/documents/HRXL-MaxSonar-WR_Datasheet.pdf
 *
 * Hardware Serial will be selected if GPIO1 = [HRXL Rx]
\*********************************************************************************************/

#define XSNS_64                      64

#include <TasmotaSerial.h>

#define HRXL_READ_TIMEOUT        400 // us; enough for 6 bytes@9600bps

TasmotaSerial *HRXLSerial = nullptr;

uint32_t hrxl_distance_mm = 0;  // distance, mm
bool hrxl_found = false;

/*********************************************************************************************/

void HRXLInit(void)
{
    hrxl_found = false;
    if (PinUsed(GPIO_HRXL_RX))
    {
        HRXLSerial = new TasmotaSerial(Pin(GPIO_HRXL_RX), -1, 1);
        if (HRXLSerial->begin(9600))
        {
            if (HRXLSerial->hardwareSerial())
                ClaimSerial();
            hrxl_found = true;
            HRXLSerial->setTimeout(HRXL_READ_TIMEOUT);
        }
    }
}

void HRXLEverySecond(void)
{
    if (!hrxl_found)
        return;

    int num_read=0;
    int sum=0;
    while (HRXLSerial->available()>5)
    {
        if (HRXLSerial->read() != 'R')
            continue;

        int d = HRXLSerial->parseInt();
        if (d >= 30 && d<=5000)
        {
            sum += d;
            num_read++;
        }
    }
    if (num_read>1)
        hrxl_distance_mm = int(sum / num_read);

}


void HRXLShow(bool json)
{
   char types[5] = "HRXL";
   if (json)
   {
      ResponseAppend_P(PSTR(",\"%s\":{\"" D_DISTANCE "\":%d}"), types, hrxl_distance_mm);
#ifdef USE_WEBSERVER
   }
   else
   {
      WSContentSend_PD(HTTP_SNS_RANGE, types, hrxl_distance_mm);
#endif  // USE_WEBSERVER
   }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns64(uint8_t function)
{
   if (!PinUsed(GPIO_HRXL_RX)) { return false; }

   switch (function)
   {
      case FUNC_INIT:
         HRXLInit();
         break;
      case FUNC_EVERY_SECOND:
         HRXLEverySecond();
         break;
      case FUNC_JSON_APPEND:
         HRXLShow(1);
         break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
         HRXLShow(0);
         break;
#endif  // USE_WEBSERVER
   }
   return false;
}

#endif  // USE_HRXL
