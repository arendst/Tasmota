/*
  xnrg_15_Teleinfo.ino - Teleinfo support for Tasmota

  Copyright (C) 2020 Charles-Henri Hallard

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

#ifdef USE_ENERGY_SENSOR
#ifdef USE_TELEINFO
/*********************************************************************************************\
 * Teleinfo : French energy provider metering telemety data 
 * Source: http://hallard.me/category/tinfo/
 *
 * Hardware Serial will be selected if GPIO1 = [TELEINFO_RX]
\*********************************************************************************************/

#define XNRG_15                  15 

#include "LibTeleinfo.h"
#include <TasmotaSerial.h>

#define TINFO_READ_TIMEOUT 400 

enum TInfoContrat{
  CONTRAT_BAS = 1,  // BASE => Option Base. 
  CONTRAT_HC,       // HC.. => Option Heures Creuses. 
  CONTRAT_EJP,      // EJP. => Option EJP. 
  CONTRAT_BBR       // BBRx => Option Tempo
};

enum TInfoTarif{
  TARIF_TH = 1,   // Toutes les Heures. 
  TARIF_HC,       // Heures Creuses. 
  TARIF_HP,       // Heures Pleines. 
  TARIF_HN        // BBRx => Option Tempo
};

//const char kTARIF_TH[] PROGMEM = "";
//const char kTARIF_HC[] PROGMEM = "Heures Creuses";
//const char kTARIF_HP[] PROGMEM = "Heures Pleines";
//const char kTARIF_HN[] PROGMEM = "Heures Normales";

//const char kTtarifNames[] PROGMEM = { kTARIF_TH, kTARIF_HC, kTARIF_HP, kTARIF_HN };

/*
    strcpy_P(buffer, (char*)pgm_read_dword(&(kTtarifNames[i])));

    if ( label == "PTEC") {
      // La période tarifaire en cours (Groupe "PTEC"), est codée sur 4 caractères 
      // J'ai pris un nombre arbitraire codé dans l'ordre ci-dessous
      if      (value=="TH..") value= 1; 
      else if (value=="HC..") value= 2; 
      else if (value=="HP..") value= 3; /
      else if (value=="HN..") value= 4; 
      else if (value=="PM..") value= 5; // Heures de Pointe Mobile. 
      else if (value=="HCJB") value= 6; // Heures Creuses Jours Bleus. 
      else if (value=="HCJW") value= 7; // Heures Creuses Jours Blancs (White). 
      else if (value=="HCJR") value= 8; // Heures Creuses Jours Rouges. 
      else if (value=="HPJB") value= 9; // Heures Pleines Jours Bleus. 
      else if (value=="HPJW") value= 10;// Heures Pleines Jours Blancs (White). 
      else if (value=="HPJR") value= 11;// Heures Pleines Jours Rouges. 
      else value = 0;
      
*/


TInfo tinfo; // Teleinfo object
TasmotaSerial *TInfoSerial = nullptr;
bool tinfo_found = false;
uint8_t contrat;
uint8_t tarif;

/*********************************************************************************************/

/* ======================================================================
Function: ADPSCallback 
Purpose : called by library when we detected a ADPS on any phased
Input   : phase number 
            0 for ADPS (monophase)
            1 for ADIR1 triphase
            2 for ADIR2 triphase
            3 for ADIR3 triphase
Output  : - 
Comments: should have been initialised in the main sketch with a
          tinfo.attachADPSCallback(ADPSCallback())
====================================================================== */
void ADPSCallback(uint8_t phase)
{
  // n = phase number 1 to 3
  if (phase == 0)
    phase = 1;

   AddLog_P2(LOG_LEVEL_INFO, PSTR("ADPS on phase %d"), phase);
}

/* ======================================================================
Function: DataCallback 
Purpose : callback when we detected new or modified data received
Input   : linked list pointer on the concerned data
          current flags value
Output  : - 
Comments: -
====================================================================== */
void DataCallback(struct _ValueList * me, uint8_t  flags)
{
    char c = ' ';

    // Does this value is new or changed?
    if (flags & (TINFO_FLAGS_ADDED | TINFO_FLAGS_UPDATED) ) 
    {
        if (flags & TINFO_FLAGS_ADDED)   { c = '#';  }
        if (flags & TINFO_FLAGS_UPDATED) { c = '*';  }

        // Current tarif
        if (!strcmp("PTEC", me->name))
        {
            if (!strcmp("TH..", me->name)) { tarif = TARIF_TH; }
            if (!strcmp("HC..", me->name)) { tarif = TARIF_HC; }
            if (!strcmp("HP..", me->name)) { tarif = TARIF_HP; }
            if (!strcmp("HN..", me->name)) { tarif = TARIF_HN; }
            AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: Tarif changed, now '%s' (%d)"), me->value, tarif);
        } 
        // Current I
        else if (!strcmp("IINST", me->name))
        {
             int i = atoi(me->value);
             Energy.current[0]  = (float) i;
             AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: Current changed %s, now %d"), me->value, i);
        }
        // Current P
        else if (!strcmp("PAPP", me->name))
        {
             int papp = atoi(me->value);
             Energy.active_power[0]  = (float) papp;
             AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: Power changed %s, now %d"), me->value, papp);
        }
    }
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: %c %s=%s"),c , me->name, me->value);
}

void TInfoDrvInit(void) {
  if (PinUsed(GPIO_TELEINFO_RX)) {
      energy_flg = XNRG_15;
  }
}

void TInfoInit(void)
{
    #ifdef USE_TELEINFO_STANDARD
    #define TINFO_SPEED 9600
    #else
    #define TINFO_SPEED 1200
    #endif

    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: inferface speed %d bps"),TINFO_SPEED);
    
    if (PinUsed(GPIO_TELEINFO_RX))
    {
         AddLog_P2(LOG_LEVEL_INFO, PSTR("TIC: enable receive on GPIO%d"), GPIO_TELEINFO_RX);
        // Enable Teleinfo
        if (PinUsed(GPIO_TELEINFO_ENABLE))
        {
            pinMode(GPIO_TELEINFO_ENABLE, OUTPUT);     
            digitalWrite(GPIO_TELEINFO_ENABLE, HIGH);
            AddLog_P2(LOG_LEVEL_INFO, PSTR("TIC: enable on GPIO%d"), GPIO_TELEINFO_ENABLE);
        } 
        else 
        {
            AddLog_P2(LOG_LEVEL_INFO, PSTR("TIC: always enabled"));
        }

        TInfoSerial = new TasmotaSerial(Pin(GPIO_TELEINFO_RX), -1, 1);

        // pinMode(GPIO_TELEINFO_RX, INPUT_PULLUP);

        if (TInfoSerial->begin(TINFO_SPEED, SERIAL_7E1))
        {
            if (TInfoSerial->hardwareSerial()) {
                Serial.end();
                Serial.begin(TINFO_SPEED, SERIAL_7E1);
                ClaimSerial();
            }
            TInfoSerial->setTimeout(TINFO_READ_TIMEOUT);

            // Init teleinfo
            tinfo.init();

            // Attach needed callbacks
            tinfo.attachADPS(ADPSCallback);
            tinfo.attachData(DataCallback); 

            tinfo_found = true;
            AddLog_P2(LOG_LEVEL_INFO, PSTR("TIC: Ready"));
        }
    }
}

void TInfoLoop(void)
{
  char c;
  if (!tinfo_found)
    return;

   if (TInfoSerial->available()) {
      //AddLog_P2(LOG_LEVEL_INFO, PSTR("TIC: received %d chars"), TInfoSerial->available());

      // We received some data?
      while (TInfoSerial->available()>8)
      {
            // get char
            c = TInfoSerial->read();

            // data processing
            tinfo.process(c);
      }
   }
}

void TInfoEvery250ms(void)
{
}

void TInfoShow(bool json)
{
   // TBD
   if (json)
   {
      ResponseAppend_P(PSTR(",\"Contrat\":%d,\"Tarif\":%d"),contrat, tarif);

#ifdef USE_WEBSERVER
   }
   else
   {
#endif  // USE_WEBSERVER
   }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg15(uint8_t function)
{

   switch (function)
   {
      case FUNC_LOOP:
         if (TInfoSerial) { TInfoLoop(); }
         break;
      case FUNC_EVERY_250_MSECOND:
         if (uptime > 4) { TInfoEvery250ms(); }
         break;
      case FUNC_JSON_APPEND:
         TInfoShow(1);
         break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
         TInfoShow(0);
         break;
#endif  // USE_WEBSERVER
      case FUNC_INIT:
         TInfoInit();
         break;
      case FUNC_PRE_INIT:
         TInfoDrvInit();
         break;
   }
   return false;
}

#endif  // USE_TELEINFO
#endif // USE_ENERGY_SENSOR
