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
    TARIF_HN,       // BBRx => Option Tempo
    TARIF_PM,       // Heures de Pointe Mobile. 
    TARIF_CB,       // Heures Creuses Jours Bleus. 
    TARIF_CW,       // Heures Creuses Jours Blancs (White). 
    TARIF_CR,       // Heures Creuses Jours Rouges. 
    TARIF_PB,       // Heures Pleines Jours Bleus. 
    TARIF_PW,       // Heures Pleines Jours Blancs (White). 
    TARIF_PR        // Heures Pleines Jours Rouges. 
};

// Label received 
const char LABEL_HCHC[] PROGMEM = "HCHC";
const char LABEL_HCHP[] PROGMEM = "HCHP";
const char LABEL_PTEC[] PROGMEM = "PTEC";
const char LABEL_PAPP[] PROGMEM = "PAPP";
const char LABEL_IINST[] PROGMEM = "IINST";
const char LABEL_TENSION[] PROGMEM = "TENSION";

// Some Values with string to compare to
const char VALUE_HCDD[] PROGMEM = "HC..";

const char kTARIF_TH[] PROGMEM = "Toutes";
const char kTARIF_HC[] PROGMEM = "Creuses";
const char kTARIF_HP[] PROGMEM = "Pleines";
const char kTARIF_HN[] PROGMEM = "Normales";
const char kTARIF_PM[] PROGMEM = "Pointe Mobile";
const char kTARIF_CB[] PROGMEM = "Creuses Bleu";
const char kTARIF_CW[] PROGMEM = "Creuses Blanc";
const char kTARIF_CR[] PROGMEM = "Creuses Rouge";
const char kTARIF_PB[] PROGMEM = "Pleines Bleu";
const char kTARIF_PW[] PROGMEM = "Pleines Blanc";
const char kTARIF_PR[] PROGMEM = "Pleines Rouge";

const char * kTtarifNames[] PROGMEM = { 
    kTARIF_TH, 
    kTARIF_HC, kTARIF_HP, 
    kTARIF_HN, kTARIF_PM,
    kTARIF_CB, kTARIF_CW, kTARIF_CR, kTARIF_PB, kTARIF_PW, kTARIF_PR
};

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
        if (!strcmp_P(LABEL_PTEC, me->name))
        {
            if (!strcmp_P("TH..", me->name)) { tarif = TARIF_TH; }
            else if (!strcmp_P("HC..", me->name)) { tarif = TARIF_HC; }
            else if (!strcmp_P("HP..", me->name)) { tarif = TARIF_HP; }
            else if (!strcmp_P("HN..", me->name)) { tarif = TARIF_HN; }
            else if (!strcmp_P("PM..", me->name)) { tarif = TARIF_PM; }
            else if (!strcmp_P("HCJB", me->name)) { tarif = TARIF_CB; }
            else if (!strcmp_P("HCJW", me->name)) { tarif = TARIF_CW; }
            else if (!strcmp_P("HCJR", me->name)) { tarif = TARIF_CR; }
            else if (!strcmp_P("HPJB", me->name)) { tarif = TARIF_PB; }
            else if (!strcmp_P("HPJW", me->name)) { tarif = TARIF_PW; }
            else if (!strcmp_P("HPJR", me->name)) { tarif = TARIF_PR; }

            AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: Tarif changed, now '%s' (%d)"), me->value, tarif);
        } 
        // Voltage V (not present on all Smart Meter)
        else if (!strcmp_P(LABEL_TENSION, me->name))
        {
             Energy.voltage_available = true;
             int i = atoi(me->value);
             Energy.voltage[0]  = (float) atoi(me->value);

            // Update current
            if (Energy.voltage_available && Energy.voltage[0]) {
                Energy.current[0] = Energy.active_power[0] / Energy.voltage[0] ;
            }

             AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: Voltage %s, now %d"), me->value, i);
        }
        // Current I
        else if (!strcmp_P(LABEL_IINST, me->name))
        {
             if (!Energy.voltage_available) {
                int i = atoi(me->value);
                Energy.current[0]  = (float) atoi(me->value);
             } else if (Energy.voltage[0]) {
                Energy.current[0] = Energy.active_power[0] / Energy.voltage[0] ;
             }

             AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: Current %s, now %d"), me->value, (int) Energy.current[0]);
        }
        // Current P
        else if (!strcmp_P(LABEL_PAPP, me->name))
        {
             int papp = atoi(me->value);
             Energy.active_power[0] = (float) atoi(me->value);
             AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: Power %s, now %d"), me->value, papp);

            // Update current
            if (Energy.voltage_available && Energy.voltage[0]) {
                Energy.current[0] = Energy.active_power[0] / Energy.voltage[0] ;
            }
        }
        // kWh indexes
        else if (!strcmp_P(LABEL_HCHC, me->name) || !strcmp(LABEL_HCHP, me->name))
        {
            char value[32];
            unsigned long hc = 0;
            unsigned long hp = 0;
            unsigned long total = 0;

            if ( tinfo.valueGet_P(LABEL_HCHC, value) ) { hc = atol(value);}
            if ( tinfo.valueGet_P(LABEL_HCHP, value) ) { hp = atol(value);}
            total = hc+hp;
           
            EnergyUpdateTotal(total/1000.0f, true);  
            AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: HC:%ld  HP:%ld  Total:%ld"), hc, hp, total);
        }
    }
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: %c %s=%s"),c , me->name, me->value);
}

/* ======================================================================
Function: NewFrameCallback 
Purpose : callback when we received a complete Teleinfo frama
Input   : linked list pointer on the concerned data
Output  : - 
Comments: -
====================================================================== */
void NewFrameCallback(struct _ValueList * me)
{
    // Reset Energy Watchdog
    Energy.data_valid[0] = 0;
}

/* ======================================================================
Function: NewFrameCallback 
Purpose : callback when we received a complete Teleinfo frama
Input   : label to search for
Output  : value filled
Comments: -
====================================================================== */
char * getDataValue_P(const char * label, char * value)
{
    if (!tinfo.valueGet_P(label, value) ) {
        *value = '\0';
    }
    return value;
}



void TInfoDrvInit(void) {
  if (PinUsed(GPIO_TELEINFO_RX)) {
      energy_flg = XNRG_15;
      Energy.voltage_available = false;
      //Energy.current_available = false;
      Energy.type_dc = true;
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
         uint8_t rx_pin = Pin(GPIO_TELEINFO_RX);
         AddLog_P2(LOG_LEVEL_INFO, PSTR("TIC: RX on GPIO%d"), rx_pin);
        // Enable Teleinfo
        if (PinUsed(GPIO_TELEINFO_ENABLE))
        {
            uint8_t en_pin = Pin(GPIO_TELEINFO_ENABLE);
            pinMode(en_pin, OUTPUT);     
            digitalWrite(en_pin, HIGH);
            AddLog_P2(LOG_LEVEL_INFO, PSTR("TIC: Enable with GPIO%d"), en_pin);
        } 
        else 
        {
            AddLog_P2(LOG_LEVEL_INFO, PSTR("TIC: always enabled"));
        }

        TInfoSerial = new TasmotaSerial(rx_pin, -1, 1);

        // pinMode(GPIO_TELEINFO_RX, INPUT_PULLUP);

        // Trick here even using SERIAL_7E1 or TS_SERIAL_7E1
        // this is not working, need to call SetSerialConfig after  
        if (TInfoSerial->begin(TINFO_SPEED))
        {
            #if defined (ESP8266)
            if (TInfoSerial->hardwareSerial() ) {
                ClaimSerial();
                AddLog_P2(LOG_LEVEL_INFO, PSTR("TIC: using hardware serial"));
            } else {
                AddLog_P2(LOG_LEVEL_INFO, PSTR("TIC: using software serial"));
            }

            // This is a dirty hack, looks like begin does not take into account
            // the TS_SERIAL_7E1 configuration so on ESP8266 this is 
            // working only on Serial RX pin (Hardware Serial) for now
            SetSerialConfig(TS_SERIAL_7E1);
            
            TInfoSerial->setTimeout(TINFO_READ_TIMEOUT);

            #elif defined (ESP32)
                AddLog_P2(LOG_LEVEL_INFO, PSTR("TIC: using ESP32 hardware serial"));
                // Waiting TasmotaSerial PR merged to change that 
                //TInfoSerial->reconf(TINFO_SPEED, SERIAL_7E1);
            #endif


            // Init teleinfo
            tinfo.init();

            // Attach needed callbacks
            tinfo.attachADPS(ADPSCallback);
            tinfo.attachData(DataCallback); 
            tinfo.attachNewFrame(NewFrameCallback); 

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

#ifdef USE_WEBSERVER
const char HTTP_ENERGY_INDEX_TELEINFO[] PROGMEM =  "{s}%s{m}%s " D_UNIT_KILOWATTHOUR "{e}" ;
const char HTTP_ENERGY_PAPP_TELEINFO[] PROGMEM =  "{s}" D_POWERUSAGE "{m}%d " D_UNIT_WATT "{e}" ;
const char HTTP_ENERGY_IINST_TELEINFO[] PROGMEM =  "{s}" D_CURRENT "{m}%d " D_UNIT_AMPERE "{e}" ;
const char HTTP_ENERGY_TARIF_TELEINFO[] PROGMEM =  "{s}Tarif{m}%s{e}" ;
#endif  // USE_WEBSERVER

void TInfoShow(bool json)
{
   char value[32];
   // TBD
   if (json)
   {
        if ( tinfo.valueGet_P(LABEL_PTEC, value) ) { 
          ResponseAppend_P(PSTR(",\"" "TARIF" "\":%s"), value);
        }
        if ( tinfo.valueGet_P(LABEL_IINST, value) ) { 
          ResponseAppend_P(PSTR(",\"" D_CURRENT "\":%s"), value);
        }
        if ( tinfo.valueGet_P(LABEL_PAPP, value) ) { 
          ResponseAppend_P(PSTR(",\"" D_POWERUSAGE "\":%s"), value);
        }
        if ( tinfo.valueGet_P(LABEL_HCHC, value) ) { 
          ResponseAppend_P(PSTR(",\"" "HC" "\":%s"), value);
        }
        if ( tinfo.valueGet_P(LABEL_HCHP, value) ) { 
          ResponseAppend_P(PSTR(",\"" "HP" "\":%s"), value);
        }
#ifdef USE_WEBSERVER
   }
   else
   {
       getDataValue_P(LABEL_HCHC, value);
       WSContentSend_PD(HTTP_ENERGY_INDEX_TELEINFO, kTARIF_HC, value);
       getDataValue_P(LABEL_HCHP, value);
       WSContentSend_PD(HTTP_ENERGY_INDEX_TELEINFO, kTARIF_HP, value);
       if (tarif) {
           WSContentSend_PD(HTTP_ENERGY_TARIF_TELEINFO, kTtarifNames[tarif-1]);
       }

       
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
