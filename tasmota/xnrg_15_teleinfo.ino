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
 * Denky ESP32 Teleinfo Template
 * {"NAME":"Denky (Teleinfo)","GPIO":[1,1,1,1,5664,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1376,1,1,0,0,0,0,1,5632,1,1,1,0,0,1],"FLAG":0,"BASE":1}
 * 
 * Denky (aka WifInfo) ESP8266 Teleinfo Template
 * {"NAME":"WifInfo","GPIO":[7,255,255,208,6,5,255,255,255,255,255,255,255],"FLAG":15,"BASE":18}
 * 
\*********************************************************************************************/

#define XNRG_15            15 

#include "LibTeleinfo.h"
#include <TasmotaSerial.h>

#define TINFO_READ_TIMEOUT 400 

// All contract type
enum TInfoContrat{
    CONTRAT_BAS = 1,  // BASE => Option Base. 
    CONTRAT_HC,       // HC.. => Option Heures Creuses. 
    CONTRAT_EJP,      // EJP. => Option EJP. 
    CONTRAT_BBR,      // BBRx => Option Tempo
    CONTRAT_END
};

// contract displayed name
const char kContratName[] PROGMEM = 
    "|Base|Heures Creuses|EJP|Bleu Blanc Rouge"
    ;

// Received current contract value
const char kContratValue[] PROGMEM = 
    "|BASE|HC..|EJP.|BBR"
    ;

// all tariff type
enum TInfoTarif{
    TARIF_TH = 1,   
    TARIF_HC,  TARIF_HP, 
    TARIF_HN,  TARIF_PM,
    TARIF_CB,  TARIF_CW, TARIF_CR,
    TARIF_PB,  TARIF_PW, TARIF_PR,
    TARIF_END
};

// Received current tariff values
const char kTarifValue[] PROGMEM = 
    "|TH..|HC..|HP.." 
    "|HN..|PM.." 
    "|HCJB|HCJW|HCJR" 
    "|HPJB|HPJW|HPJR" 
    ;

// tariff displayed name
const char kTarifName[] PROGMEM = 
    "|Toutes|Creuses|Pleines" 
    "|Normales|Pointe Mobile" 
    "|Creuses Bleu|Creuses Blanc|Creuse Rouges" 
    "|Pleines Bleu|Pleines Blanc|Pleines Rouges" 
    ;

enum TInfoLabel{
    LABEL_BASE = 1, 
    LABEL_HCHC, LABEL_HCHP,  
    LABEL_OPTARIF, LABEL_ISOUSC, LABEL_PTEC,
    LABEL_PAPP, LABEL_IINST, LABEL_IMAX, LABEL_TENSION,
    LABEL_DEMAIN,
    LABEL_END
};

const char kLabel[] PROGMEM = 
    "|BASE|HCHC|HCHP"
    "|OPTARIF|ISOUSC|PTEC"
    "|PAPP|IINST|IMAX|TENSION"
    "|DEMAIN"
    ;

TInfo tinfo; // Teleinfo object
TasmotaSerial *TInfoSerial = nullptr;
bool tinfo_found = false;
uint8_t contrat;
uint8_t tarif;
uint8_t isousc;

/*********************************************************************************************/

/* ======================================================================
Function: getValueFromLabelIndex 
Purpose : return label value from label index
Input   : label index to search for
Output  : value filled
Comments: -
====================================================================== */
char * getValueFromLabelIndex(int labelIndex, char * value)
{
    char labelName[16];
    // Get the label name
    GetTextIndexed(labelName, sizeof(labelName), labelIndex, kLabel);
    // Get value of label name
    return tinfo.valueGet(labelName, value) ;
}

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
  if (phase == 0){
    phase = 1;
  }
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
    int ilabel ;

    // Does this value is new or changed?
    if (flags & (TINFO_FLAGS_ADDED | TINFO_FLAGS_UPDATED) ) {
        char labelName[16];
        // Find the label index
        for ( ilabel = 1 ; ilabel < LABEL_END ; ilabel++) {
            GetTextIndexed(labelName, sizeof(labelName), ilabel, kLabel);
            if (!strcmp(labelName, me->name)) { 
                break;
            }
        }

        // Current tariff 
        if (ilabel == LABEL_PTEC)
        {
            char tarif_value[] = "    "; // 4 spaces
            // Find the tariff index
            for (tarif = TARIF_TH ; tarif < TARIF_END ; tarif++) {
                GetTextIndexed(tarif_value, sizeof(tarif_value), tarif-1, kTarifValue);
                if (!strcmp(tarif_value, me->value)) { 
                    break;
                }
            }
            AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: Tarif changed, now '%s' (%d)"), me->value, tarif);
        } 

        // Voltage V (not present on all Smart Meter)
        else if ( ilabel == LABEL_TENSION)
        {
            Energy.voltage_available = true;
            Energy.voltage[0]  = (float) atoi(me->value);
            // Update current
            if (Energy.voltage_available && Energy.voltage[0]) {
                Energy.current[0] = Energy.active_power[0] / Energy.voltage[0] ;
            }
            AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: Voltage %s, now %d"), me->value, (int) Energy.voltage[0]);
        }

        // Current I
        else if (ilabel == LABEL_IINST)
        {
            if (!Energy.voltage_available) {
                Energy.current[0]  = (float) atoi(me->value);
            } else if (Energy.voltage[0]) {
                Energy.current[0] = Energy.active_power[0] / Energy.voltage[0] ;
            }
            AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: Current %s, now %d"), me->value, (int) Energy.current[0]);
        }

        // Power P
        else if (ilabel == LABEL_PAPP)
        {
            int papp = atoi(me->value);
            AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: Power %s, now %d"), me->value, papp);
            Energy.active_power[0]  = (float) atoi(me->value);
            // Update current
            if (Energy.voltage_available && Energy.voltage[0]) {
            Energy.current[0] = Energy.active_power[0] / Energy.voltage[0] ;
            }
        }

        // Wh indexes
        else if ( ilabel == LABEL_HCHC || ilabel == LABEL_HCHP)
        {
            char value[32];
            uint32_t hc = 0;
            uint32_t hp = 0;
            uint32_t total = 0;

            if ( getValueFromLabelIndex(LABEL_HCHC, value) ) { hc = atoi(value);}
            if ( getValueFromLabelIndex(LABEL_HCHP, value) ) { hp = atoi(value);}
            total = hc + hp;
            EnergyUpdateTotal(total/1000.0f, true);  
            AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: HC:%u  HP:%u  Total:%u"), hc, hp, total);
        }

        // Contract subscribed
        else if (ilabel == LABEL_OPTARIF)
        {
            char contrat_value[] = "    "; // 4 spaces
            // Find the contract index
            for (contrat = CONTRAT_BAS ; contrat < CONTRAT_END ; contrat++) {
                GetTextIndexed(contrat_value, sizeof(contrat_value), contrat, kContratValue);
                if (!strcmp(contrat_value, me->value)) { 
                    break;
                }
            }
            AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: Contract changed, now '%s' (%d)"), me->value, contrat);
        } 

        // Contract subscribed (Power)
        else if (ilabel == LABEL_ISOUSC)
        {
            isousc = atoi( me->value);
            AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: ISousc set to %d"), isousc);
        } 

    }

    if (flags & TINFO_FLAGS_ADDED)   { c = '#';  }
    if (flags & TINFO_FLAGS_UPDATED) { c = '*';  }
    AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("TIC: %c %s=%s"),c , me->name, me->value);
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
Function: TInfoDrvInit 
Purpose : Tasmota core driver init 
Input   : -
Output  : - 
Comments: -
====================================================================== */
void TInfoDrvInit(void) {
  if (PinUsed(GPIO_TELEINFO_RX)) {
      energy_flg = XNRG_15;
      Energy.voltage_available = false;
      //Energy.current_available = false;
      Energy.type_dc = true;
  }
}

/* ======================================================================
Function: TInfoInit 
Purpose : Tasmota core device init 
Input   : -
Output  : - 
Comments: -
====================================================================== */
void TInfoInit(void)
{
    #ifdef USE_TELEINFO_STANDARD
    #define TINFO_SPEED 9600
    #else
    #define TINFO_SPEED 1200
    #endif

    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: inferface speed %d bps"),TINFO_SPEED);
    
    if (PinUsed(GPIO_TELEINFO_RX)) {
         uint8_t rx_pin = Pin(GPIO_TELEINFO_RX);
         AddLog_P2(LOG_LEVEL_INFO, PSTR("TIC: RX on GPIO%d"), rx_pin);

        // Enable Teleinfo pin used, control it
        if (PinUsed(GPIO_TELEINFO_ENABLE)) {
            uint8_t en_pin = Pin(GPIO_TELEINFO_ENABLE);
            pinMode(en_pin, OUTPUT);     
            digitalWrite(en_pin, HIGH);
            AddLog_P2(LOG_LEVEL_INFO, PSTR("TIC: Enable with GPIO%d"), en_pin);
        } else  {
            AddLog_P2(LOG_LEVEL_INFO, PSTR("TIC: always enabled"));
        }

        TInfoSerial = new TasmotaSerial(rx_pin, -1, 1);
        // pinMode(GPIO_TELEINFO_RX, INPUT_PULLUP);

        // Trick here even using SERIAL_7E1 or TS_SERIAL_7E1
        // this is not working, need to call SetSerialConfig after  
        if (TInfoSerial->begin(TINFO_SPEED)) {
            // This is a hack, looks like begin does not take into account
            // the TS_SERIAL_7E1 configuration so on ESP8266 this is 
            // working only on Serial RX pin (Hardware Serial) for now
            SetSerialConfig(TS_SERIAL_7E1);
            TInfoSerial->setTimeout(TINFO_READ_TIMEOUT);

#if defined (ESP8266)
            if (TInfoSerial->hardwareSerial() ) {
                ClaimSerial();
                AddLog_P2(LOG_LEVEL_INFO, PSTR("TIC: using hardware serial"));
            } else {
                AddLog_P2(LOG_LEVEL_INFO, PSTR("TIC: using software serial"));
            }

#elif defined (ESP32)
            AddLog_P2(LOG_LEVEL_INFO, PSTR("TIC: using ESP32 hardware serial"));
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

/* ======================================================================
Function: TInfoEvery250ms 
Purpose : Tasmota callback executed every 250ms
Input   : -
Output  : - 
Comments: -
====================================================================== */
void TInfoEvery250ms(void)
{
    char c;
    if (!tinfo_found)
        return;

    if (TInfoSerial->available()) {
        //AddLog_P2(LOG_LEVEL_INFO, PSTR("TIC: received %d chars"), TInfoSerial->available());
        // We received some data?
        while (TInfoSerial->available()>8) {
            // get char
            c = TInfoSerial->read();
            // data processing
            tinfo.process(c);
        }
    }
}

/* ======================================================================
Function: TInfoShow 
Purpose : Tasmota callback executed to send telemetry or WEB display
Input   : -
Output  : - 
Comments: -
====================================================================== */
#ifdef USE_WEBSERVER
const char HTTP_ENERGY_INDEX_TELEINFO[] PROGMEM =  "{s}%s{m}%s " D_UNIT_WATTHOUR "{e}" ;
const char HTTP_ENERGY_PAPP_TELEINFO[] PROGMEM =  "{s}" D_POWERUSAGE "{m}%d " D_UNIT_WATT "{e}" ;
const char HTTP_ENERGY_IINST_TELEINFO[] PROGMEM =  "{s}" D_CURRENT "{m}%d " D_UNIT_AMPERE "{e}" ;
const char HTTP_ENERGY_TARIF_TELEINFO[] PROGMEM =  "{s}Tarif en cours{m}Heures %s{e}" ;
const char HTTP_ENERGY_CONTRAT_TELEINFO[] PROGMEM =  "{s}Contrat{m}%s %d" D_UNIT_AMPERE "{e}" ;
const char HTTP_ENERGY_LOAD_TELEINFO[] PROGMEM =  "{s}Charge actuelle{m}%d" D_UNIT_PERCENT "{e}" ;
#endif  // USE_WEBSERVER

void TInfoShow(bool json)
{
    char name[32];
    char value[32];

    // Since it's an Energy device , current, voltage and power are 
    // already present on the telemetry frame. No need to add here
    // Just add the specific and missing ones there
    if (json)
    {
        if ( getValueFromLabelIndex(LABEL_PTEC, value) ) { 
            ResponseAppend_P(PSTR(",\"" "TARIF" "\":\"%s\""), value);
        }

        GetTextIndexed(name, sizeof(name), LABEL_ISOUSC, kLabel);
        ResponseAppend_P(PSTR(",\"%s\":%d"), name, isousc);

        if ( getValueFromLabelIndex(LABEL_HCHC, value) ) { 
            GetTextIndexed(name, sizeof(name), LABEL_HCHC, kLabel);
            ResponseAppend_P(PSTR(",\"%s\":\"%u\""), name, atoi(value));
        }
        if ( getValueFromLabelIndex(LABEL_HCHP, value) ) { 
            GetTextIndexed(name, sizeof(name), LABEL_HCHP, kLabel);
            ResponseAppend_P(PSTR(",\"%s\":\"%u\""),name , atoi(value));
        }

        if (isousc) {
            ResponseAppend_P(PSTR(",\"Load\":\"%d\""),(int) ((Energy.current[0]*100.0f) / isousc));
        }

#ifdef USE_WEBSERVER
    }
    else
    {
        if (getValueFromLabelIndex(LABEL_HCHC, value) ) {
            GetTextIndexed(name, sizeof(name), LABEL_HCHC, kLabel);
            WSContentSend_PD(HTTP_ENERGY_INDEX_TELEINFO, name, value);
        }
        if (getValueFromLabelIndex(LABEL_HCHP, value) ) {
            GetTextIndexed(name, sizeof(name), LABEL_HCHP, kLabel);
            WSContentSend_PD(HTTP_ENERGY_INDEX_TELEINFO, name, value);
        }
        if (tarif) {
            GetTextIndexed(name, sizeof(name), tarif-1, kTarifName);
            WSContentSend_PD(HTTP_ENERGY_TARIF_TELEINFO, name,value);
        }
        if (contrat) {
            GetTextIndexed(name, sizeof(name), contrat, kContratName);
            WSContentSend_PD(HTTP_ENERGY_CONTRAT_TELEINFO, name, isousc);
            if (isousc) {
                int percent = (int) ((Energy.current[0]*100.0f) / isousc) ;
                WSContentSend_PD(HTTP_ENERGY_LOAD_TELEINFO,  percent);
            }
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