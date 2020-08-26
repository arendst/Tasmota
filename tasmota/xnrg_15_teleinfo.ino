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
 * {"NAME":"WifInfo v1.0a","GPIO":[17,255,255,255,6,5,255,255,7,210,255,255,255],"FLAG":15,"BASE":18}
 * {"NAME":"WifInfo","GPIO":[7,255,255,210,6,5,255,255,255,255,255,255,255],"FLAG":15,"BASE":18}
 * 
\*********************************************************************************************/
#define XNRG_15            15 

#include "LibTeleinfo.h"
#include <TasmotaSerial.h>

#define TINFO_READ_TIMEOUT 400 

// All contract type for legacy, standard mode has in clear text
enum TInfoContrat{
    CONTRAT_BAS = 1,  // BASE => Option Base. 
    CONTRAT_HC,       // HC.. => Option Heures Creuses. 
    CONTRAT_EJP,      // EJP. => Option EJP. 
    CONTRAT_BBR,      // BBRx => Option Tempo
    CONTRAT_END
};

// contract displayed name for legacy, standard mode has in clear text
const char kContratName[] PROGMEM = 
    "|Base|Heures Creuses|EJP|Bleu Blanc Rouge"
    ;

// Received current contract value for legacy, standard mode has in clear text
const char kContratValue[] PROGMEM = 
    "|BASE|HC..|EJP.|BBR"
    ;

// all tariff type for legacy, standard mode has in clear text
enum TInfoTarif{
    TARIF_TH = 1,   
    TARIF_HC,  TARIF_HP, 
    TARIF_HN,  TARIF_PM,
    TARIF_CB,  TARIF_CW, TARIF_CR,
    TARIF_PB,  TARIF_PW, TARIF_PR,
    TARIF_END
};

// Received current tariff values 
// for legacy, standard mode has in clear text
const char kTarifValue[] PROGMEM = 
    "|TH..|HC..|HP.." 
    "|HN..|PM.." 
    "|HCJB|HCJW|HCJR" 
    "|HPJB|HPJW|HPJR" 
    ;

// tariff displayed name (for legacy, standard mode has in clear text)
const char kTarifName[] PROGMEM = 
    "|Toutes|Creuses|Pleines" 
    "|Normales|Pointe Mobile" 
    "|Creuses Bleu|Creuses Blanc|Creuse Rouges" 
    "|Pleines Bleu|Pleines Blanc|Pleines Rouges" 
    ;

// Label used to do some post processing and/or calculation
enum TInfoLabel{
    LABEL_BASE = 1, 
    LABEL_ADCO, LABEL_ADSC,
    LABEL_HCHC, LABEL_HCHP, LABEL_EAST, LABEL_EASF01, LABEL_EASF02,
    LABEL_OPTARIF, LABEL_NGTF, LABEL_ISOUSC, LABEL_PREF, LABEL_PTEC, LABEL_LTARF, LABEL_NTARF,
    LABEL_PAPP, LABEL_SINSTS, LABEL_IINST, LABEL_IRMS1, LABEL_TENSION, LABEL_URMS1,
    LABEL_IMAX, LABEL_PMAX, LABEL_SMAXSN,
    LABEL_DEMAIN,
    LABEL_END
};

const char kLabel[] PROGMEM = 
    "|BASE|ADCO|ADSC"
    "|HCHC|HCHP|EAST|EASF01|EASF02"
    "|OPTARIF|NGTF|ISOUSC|PREF|PTEC|LTARF|NTARF"
    "|PAPP|SINSTS|IINST|IRMS1|TENSION|URMS1"
    "|IMAX|PMAX|SMAXSN"
    "|DEMAIN"
    ;

TInfo tinfo; // Teleinfo object
TasmotaSerial *TInfoSerial = nullptr;
_Mode_e tinfo_mode = TINFO_MODE_HISTORIQUE;
char serialNumber[13] = ""; // Serial number is 12 char long
bool tinfo_found = false;
int contrat;
int tarif;
int isousc;

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
Comments: should have been initialised with a
          tinfo.attachADPSCallback(ADPSCallback())
====================================================================== */
void ADPSCallback(uint8_t phase)
{
    // n = phase number 1 to 3
    if (phase == 0){
        phase = 1;
    }

    Response_P(PSTR("{"));
    ResponseAppend_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"%s\":{\"ADPS\":%i}}"), serialNumber, phase );
    ResponseJsonEnd();

    // Publish adding ADCO serial number into the topic
    MqttPublishPrefixTopic_P(RESULT_OR_TELE, serialNumber, false);

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

        if (flags & TINFO_FLAGS_ADDED)   { c = '#';  }
        if (flags & TINFO_FLAGS_UPDATED) { c = '*';  }
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: [%d]%c %s=%s"), ilabel, c , me->name, me->value);

        if (ilabel<LABEL_END) {

            // Current tariff (legacy)
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

            // Current tariff (standard is in clear text in value)
            else if (ilabel == LABEL_LTARF)
            {
                AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: Tarif name changed, now '%s'"), me->value);
            } 
            // Current tariff (standard index is is in clear text in value)
            else if (ilabel == LABEL_NTARF)
            {
                tarif = atoi(me->value);
                AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: Tarif index changed, now '%d'"), tarif);
            } 


            // Voltage V (not present on all Smart Meter)
            else if ( ilabel == LABEL_TENSION || ilabel == LABEL_URMS1)
            {
                Energy.voltage_available = true;
                Energy.voltage[0]  = (float) atoi(me->value);
                AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: Voltage %s, now %d"), me->value, (int) Energy.voltage[0]);
            }

            // Current I
            else if (ilabel == LABEL_IINST || ilabel == LABEL_IRMS1)
            {
                Energy.current[0]  = (float) atoi(me->value);
                AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: Current %s, now %d"), me->value, (int) Energy.current[0]);
            }

            // Power P
            else if (ilabel == LABEL_PAPP || ilabel == LABEL_SINSTS)
            {
                Energy.active_power[0]  = (float) atoi(me->value);;
                AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: Power %s, now %d"), me->value, (int)  Energy.active_power[0]);
            }

            // Wh indexes (legacy)
            else if ( ilabel == LABEL_HCHC || ilabel == LABEL_HCHP || ilabel == LABEL_BASE)
            {
                char value[32];
                uint32_t hc = 0;
                uint32_t hp = 0;
                uint32_t total = 0;

                // Base, un seul index
                if (ilabel == LABEL_BASE) {
                    total = atoi(me->value);
                    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: Base:%u"), total);
                // Heures creuses/pleines calculer total
                } else {
                    // Heures creuses get heures pleines
                    if (ilabel == LABEL_HCHC) {
                        hc = atoi(me->value);
                        if ( getValueFromLabelIndex(LABEL_HCHP, value) ) { 
                            hp = atoi(value);
                        }
                    
                    // Heures pleines, get heures creuses
                    } else if (ilabel == LABEL_HCHP) {
                        hp = atoi(me->value);
                        if ( getValueFromLabelIndex(LABEL_HCHC, value) ) { 
                            hc = atoi(value);
                        }
                    }
                    total = hc + hp;
                    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: HC:%u  HP:%u  Total:%u"), hc, hp, total);
                }

                if (!Settings.flag4.teleinfo_rawdata) { 
                    EnergyUpdateTotal(total/1000.0f, true);  
                }
            }

            // Wh total index (standard) 
            else if ( ilabel == LABEL_EAST)
            {
                uint32_t total = atoi(me->value);
                if (!Settings.flag4.teleinfo_rawdata) { 
                    EnergyUpdateTotal(total/1000.0f, true);  
                }
                AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: Total:%uWh"), total);
            }

            // Wh indexes (standard)
            else if ( ilabel == LABEL_EASF01)
            {
                AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: HC:%u"),  atoi(me->value));
            }
            else if ( ilabel == LABEL_EASF02)
            {
                AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: HP:%u"),  atoi(me->value));
            }

            // Contract subscribed (legacy)
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
            // Contract subscribed (standard is in clear text in value)
            else if (ilabel == LABEL_NGTF)
            {
                AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: Contract changed, now '%s'"), me->value);
            } 

            // Contract subscribed (Power)
            else if (ilabel == LABEL_ISOUSC || ilabel == LABEL_PREF)
            {
                isousc = atoi( me->value);
                AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: ISousc set to %d"), isousc);
            } 

            // Serial Number of device
            else if (ilabel == LABEL_ADCO || ilabel == LABEL_ADSC)
            {
                strcpy(serialNumber, me->value);
                AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: %s set to %s"), me->name, serialNumber);
            } 

        }
    }

 


}

/* ======================================================================
Function: responseDumpTInfo 
Purpose : add teleinfo values into JSON response
Input   : 1st separator space if begining of JSON, else comma
Output  : - 
Comments: -
====================================================================== */
void ResponseAppendTInfo(char sep)
{
    struct _ValueList * me = tinfo.getList();

    char * p ;
    boolean isNumber ;

    // Loop thru all the teleinfo frame but
    // always check we don't buffer overflow of MQTT data
    while (me->next) {
        // go to next node
        me = me->next;

        if (me->name && me->value && *me->name && *me->value) {
            isNumber = true;
            p = me->value;

            // Specific treatment serial number don't convert to number later
            if (strcmp(me->name, "ADCO")==0 || strcmp(me->name, "ADSC")==0) {
                isNumber = false;
            } else {
                // check if value is number
                while (*p && isNumber) {
                    if ( *p < '0' || *p > '9' ) {
                        isNumber = false;
                    }
                    p++;
                }
            }

            ResponseAppend_P( PSTR("%c\"%s\":"), sep, me->name );

            if (!isNumber) {
                ResponseAppend_P( PSTR("\"%s\""), me->value );
            } else {
                ResponseAppend_P( PSTR("%d"), atoi(me->value));
            }

            // Now JSON separator is needed 
            sep =',';
        }
    }
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

    // send teleinfo full frame only if setup like that
    // see setOption108
    if (Settings.flag4.teleinfo_rawdata) { 
        Response_P(PSTR("{"));
        ResponseAppendTInfo(' ');
        ResponseJsonEnd();
        // Publish adding ADCO serial number into the topic
        // Need setOption4 to be enabled
        MqttPublishPrefixTopic_P(RESULT_OR_TELE, serialNumber, false);
    }
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
    int baudrate;

    // SetOption102 - Set Baud rate for Teleinfo serial communication (0 = 1200 or 1 = 9600)
    if (Settings.flag4.teleinfo_baudrate) { 
        baudrate = 9600; 
        tinfo_mode = TINFO_MODE_STANDARD;
    }  else {
        baudrate = 1200; 
        tinfo_mode = TINFO_MODE_HISTORIQUE;
    }

    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TIC: inferface speed %d bps"),baudrate);
    
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

#if defined (ESP8266)
        // Allow GPIO3 AND GPIO13 with hardware fallback to 2
        TInfoSerial = new TasmotaSerial(rx_pin, -1, 2);
        //pinMode(rx_pin, INPUT_PULLUP);
#else
        TInfoSerial = new TasmotaSerial(rx_pin, -1, 1);
#endif

        // Trick here even using SERIAL_7E1 or TS_SERIAL_7E1
        // this is not working, need to call SetSerialConfig after  
        if (TInfoSerial->begin(baudrate)) {


#if defined (ESP8266)
            if (TInfoSerial->hardwareSerial() ) {
                ClaimSerial();

                // This is a hack, looks like begin does not take into account
                // the TS_SERIAL_7E1 configuration so on ESP8266 this is 
                // working only on Serial RX pin (Hardware Serial) for now
                
                //SetSerialConfig(TS_SERIAL_7E1);
                //TInfoSerial->setTimeout(TINFO_READ_TIMEOUT);

                AddLog_P2(LOG_LEVEL_INFO, PSTR("TIC: using hardware serial"));
            } else {
                AddLog_P2(LOG_LEVEL_INFO, PSTR("TIC: using software serial"));
            }

#elif defined (ESP32)
            AddLog_P2(LOG_LEVEL_INFO, PSTR("TIC: using ESP32 hardware serial"));
#endif
            // Init teleinfo
            tinfo.init(tinfo_mode);
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
    if (!tinfo_found) {
        return;
    }

    if (TInfoSerial->available()) {
        unsigned long start = millis();
        char c;

        // We received some data, process but never more than 100ms ?
        while (TInfoSerial->available()>8 && millis()-start < 100) {
            // get char
            c = TInfoSerial->read();
            // data processing
            tinfo.process(c & 0x7F);
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
const char HTTP_ENERGY_ID_TELEINFO[] PROGMEM =  "{s}ID{m}%s{e}" ;
const char HTTP_ENERGY_INDEX_TELEINFO[] PROGMEM =  "{s}%s{m}%s " D_UNIT_WATTHOUR "{e}" ;
const char HTTP_ENERGY_PAPP_TELEINFO[] PROGMEM =  "{s}" D_POWERUSAGE "{m}%d " D_UNIT_WATT "{e}" ;
const char HTTP_ENERGY_IINST_TELEINFO[] PROGMEM =  "{s}" D_CURRENT "{m}%d " D_UNIT_AMPERE "{e}" ;
const char HTTP_ENERGY_TARIF_TELEINFO[] PROGMEM =  "{s}" D_CURRENT_TARIFF "{m}Heures %s{e}" ;
const char HTTP_ENERGY_CONTRAT_TELEINFO[] PROGMEM =  "{s}" D_CONTRACT "{m}%s %d" D_UNIT_AMPERE "{e}" ;
const char HTTP_ENERGY_LOAD_TELEINFO[] PROGMEM =  "{s}" D_POWER_LOAD "{m}%d" D_UNIT_PERCENT "{e}" ;
const char HTTP_ENERGY_IMAX_TELEINFO[] PROGMEM =  "{s}" D_MAX_CURRENT "{m}%d" D_UNIT_AMPERE "{e}" ;
const char HTTP_ENERGY_PMAX_TELEINFO[] PROGMEM =  "{s}" D_MAX_POWER "{m}%d" D_UNIT_WATT "{e}" ;
#endif  // USE_WEBSERVER

void TInfoShow(bool json)
{
    // Since it's an Energy device , current, voltage and power are 
    // already present on the telemetry frame. No need to add here
    // Just add the raw label/values of the teleinfo frame
    if (json)
    {
        // Calculated values
        if (isousc) {
            ResponseAppend_P(PSTR(",\"Load\":%d"),(int) ((Energy.current[0]*100.0f) / isousc));
        }

        // add teleinfo full frame only if no teleinfo raw data setup
        if (!Settings.flag4.teleinfo_rawdata) { 
            ResponseAppendTInfo(',');
        }


#ifdef USE_WEBSERVER
    }
    else
    {
        char name[32];
        char value[32];

        if (getValueFromLabelIndex(LABEL_BASE, value) ) {
            GetTextIndexed(name, sizeof(name), LABEL_BASE, kLabel);
            WSContentSend_PD(HTTP_ENERGY_INDEX_TELEINFO, name, value);
        }
        if (getValueFromLabelIndex(LABEL_HCHC, value) ) {
            GetTextIndexed(name, sizeof(name), LABEL_HCHC, kLabel);
            WSContentSend_PD(HTTP_ENERGY_INDEX_TELEINFO, name, value);
        }
        if (getValueFromLabelIndex(LABEL_HCHP, value) ) {
            GetTextIndexed(name, sizeof(name), LABEL_HCHP, kLabel);
            WSContentSend_PD(HTTP_ENERGY_INDEX_TELEINFO, name, value);
        }
        if (getValueFromLabelIndex(LABEL_IMAX, value) ) {
            WSContentSend_PD(HTTP_ENERGY_IMAX_TELEINFO, atoi(value));
        }
        if (getValueFromLabelIndex(LABEL_PMAX, value) ) {
            WSContentSend_PD(HTTP_ENERGY_PMAX_TELEINFO, atoi(value));
        }

        if (tinfo_mode==TINFO_MODE_HISTORIQUE ) {
            if (tarif) {
                GetTextIndexed(name, sizeof(name), tarif-1, kTarifName);
                WSContentSend_PD(HTTP_ENERGY_TARIF_TELEINFO, name);
            }
            if (contrat && isousc) {
                int percent = (int) ((Energy.current[0]*100.0f) / isousc) ;
                GetTextIndexed(name, sizeof(name), contrat, kContratName);
                WSContentSend_PD(HTTP_ENERGY_CONTRAT_TELEINFO, name, isousc);
                WSContentSend_PD(HTTP_ENERGY_LOAD_TELEINFO,  percent);
            }
        } else if (tinfo_mode==TINFO_MODE_STANDARD ) {
            if (getValueFromLabelIndex(LABEL_LTARF, name) ) {
                WSContentSend_PD(HTTP_ENERGY_TARIF_TELEINFO, name);
            }

            if (getValueFromLabelIndex(LABEL_NGTF, name) ) {
                if (isousc) {
                    int percent = (int) ((Energy.current[0]*100.0f) / isousc) ;
                    WSContentSend_PD(HTTP_ENERGY_CONTRAT_TELEINFO, name, isousc);
                    WSContentSend_PD(HTTP_ENERGY_LOAD_TELEINFO,  percent);
                }
            }
        }

        // Serial number ADCO or ADSC if found
        if (*serialNumber) {
            WSContentSend_PD(HTTP_ENERGY_ID_TELEINFO, serialNumber);
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