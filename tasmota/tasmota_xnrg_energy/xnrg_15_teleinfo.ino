/*
  xnrg_15_Teleinfo.ino - Teleinfo support for Tasmota

  Copyright (C) 2021  Charles-Henri Hallard

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
 * Denky D4 ESP Pico V3 Teleinfo Template
 * {"NAME":"DenkyD4","GPIO":[32,0,0,0,1,0,0,0,0,1,0,1,0,0,0,0,0,640,608,0,0,450,449,448,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
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

#define D_NAME_TELEINFO "Teleinfo"

// Json Command
//const char S_JSON_TELEINFO_COMMAND_STRING[] PROGMEM = "{\"" D_NAME_TELEINFO "\":{\"%s\":%s}}";
//const char S_JSON_TELEINFO_COMMAND_NVALUE[] PROGMEM = "{\"" D_NAME_TELEINFO "\":{\"%s\":%d}}";
const char TELEINFO_COMMAND_SETTINGS[] PROGMEM = "TIC: Settings Mode:%s, RX:%s, EN:%s, Raw:%s, Skip:%d, Limit:%d, Stats:%d";

#define MAX_TINFO_COMMAND_NAME 16+1 // Change this if one of the following kTInfo_Commands is higher then 16 char
const char kTInfo_Commands[] PROGMEM  = "historique|standard|noraw|full|changed|skip|limit|stats";

enum TInfoCommands {            // commands for Console
  CMND_TELEINFO_HISTORIQUE=0,   // Set Legacy mode
  CMND_TELEINFO_STANDARD,       // Set Standard Mode
  CMND_TELEINFO_RAW_DISABLE,    // Disable Raw frame sending
  CMND_TELEINFO_RAW_FULL,       // Enable all RAW frame send
  CMND_TELEINFO_RAW_CHANGE,     // Enable only changed values RAW frame send
  CMND_TELEINFO_SKIP,           // Set number of frame to skip when raw mode is enabled
  CMND_TELEINFO_LIMIT,          // Limit RAW frame to values subject to fast change (Power, Current, ...), TBD
  CMND_TELEINFO_STATS           // Show / clear / Enable TIC reception errors stats
};


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
    "|BASE|HC..|EJP|BBR"
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

// Legacy mode Received current tariff values
const char kTarifValue[] PROGMEM =
    "|TH..|HC..|HP.."
    "|HN..|PM.."
    "|HCJB|HCJW|HCJR"
    "|HPJB|HPJW|HPJR"
    ;

// legacy mode tariff displayed name
const char kTarifName[] PROGMEM =
    "|Toutes|Creuses|Pleines"
    "|Normales|Pointe Mobile"
    "|Creuses Bleu|Creuses Blanc|Creuse Rouges"
    "|Pleines Bleu|Pleines Blanc|Pleines Rouges"
    ;

// contract name for standard mode LGTF 
#define TELEINFO_STD_CONTRACT_BASE  PSTR("BASE")
#define TELEINFO_STD_CONTRACT_HCHP  PSTR("HC")
#define TELEINFO_STD_CONTRACT_BBR   PSTR("BBR")
#define TELEINFO_STD_CONTRACT_EJP   PSTR("EJP")

// current tariff values for standard mode (Tarif en cours)
//#define TELEINFO_STD_TARIFF_BASE    PSTR("BASE")
//#define TELEINFO_STD_TARIFF_HC      PSTR("CREUSE")
//#define TELEINFO_STD_TARIFF_HP      PSTR("PLEINE")


// Label used to do some post processing and/or calculation
enum TInfoLabel{
    LABEL_BASE = 1,
    LABEL_ADCO, LABEL_ADSC,
    LABEL_HCHC, LABEL_HCHP, LABEL_EAST, LABEL_EASF01, LABEL_EASF02,
    LABEL_HCJB,LABEL_HPJB,LABEL_HCJW,LABEL_HPJW,LABEL_HCJR,LABEL_HPJR,
    LABEL_EASF03, LABEL_EASF04, LABEL_EASF05, LABEL_EASF06,
    LABEL_OPTARIF, LABEL_NGTF, LABEL_ISOUSC, LABEL_PREF, LABEL_PTEC, LABEL_LTARF, LABEL_NTARF,
    LABEL_PAPP, LABEL_SINSTS, LABEL_SINSTS1, LABEL_SINSTS2, LABEL_SINSTS3, LABEL_IINST, LABEL_IINST1, LABEL_IINST2, LABEL_IINST3, LABEL_IRMS1, LABEL_IRMS2, LABEL_IRMS3,
    LABEL_TENSION, LABEL_URMS1, LABEL_URMS2, LABEL_URMS3,
    LABEL_IMAX, LABEL_IMAX1, LABEL_IMAX2, LABEL_IMAX3, LABEL_PMAX, LABEL_SMAXSN,
    LABEL_DEMAIN,LABEL_MSG1,LABEL_MSG2,LABEL_STGE,
    LABEL_END
};

const char kLabel[] PROGMEM =
    "|BASE|ADCO|ADSC"
    "|HCHC|HCHP|EAST|EASF01|EASF02"
    "|BBRHCJB|BBRHPJB|BBRHCJW|BBRHPJW|BBRHCJR|BBRHPJR"
    "|EASF03|EASF04|EASF05|EASF06"
    "|OPTARIF|NGTF|ISOUSC|PREF|PTEC|LTARF|NTARF"
    "|PAPP|SINSTS|SINSTS1|SINSTS2|SINSTS3|IINST|IINST1|IINST2|IINST3|IRMS1|IRMS2|IRMS3"
    "|TENSION|URMS1|URMS2|URMS3"
    "|IMAX|IMAX1|IMAX2|IMAX3|PMAX|SMAXSN"
    "|DEMAIN|MSG1|MSG2|STGE"
    ;

// Blacklisted label from telemetry
// Each label shoud be enclosed by pipe
const char kLabelBlacklist[]
// declared as progmem for ESP8266 just crash and reset on strstr()
#ifndef ESP8266
PROGMEM
#endif
    =
    "|PJOURF+1"
    "|MSG1"
    "|PPOINTE"
    "|"
    ;

#define TELEINFO_SERIAL_BUFFER_STANDARD       1536      // Receive buffer size for Standard mode
#define TELEINFO_SERIAL_BUFFER_HISTORIQUE      512      // Receive buffer size for Legacy mode
#define TELEINFO_PROCESS_BUFFER                 32      // Local processing buffer

TInfo tinfo; // Teleinfo object
TasmotaSerial *TInfoSerial = nullptr;
_Mode_e tinfo_mode = TINFO_MODE_HISTORIQUE;
uint8_t tic_rx_pin = NOT_A_PIN;
char serialNumber[13] = ""; // Serial number is 12 char long
bool tinfo_found = false;
int serial_buffer_size;
uint32_t total_wh;
uint32_t status_register;
int contrat;
int tarif;
int isousc;
int raw_skip;

/*********************************************************************************************/
#ifdef USE_WEBSERVER
const char HTTP_ENERGY_ID_TELEINFO[] PROGMEM =  "{s}ID{m}%s{e}" ;
const char HTTP_ENERGY_INDEX_TELEINFO[] PROGMEM =  "{s}%s{m}%s " D_UNIT_WATTHOUR "{e}" ;
const char HTTP_ENERGY_INDEX_TELEINFO_SELECT[] PROGMEM =  "{s}%s{m}%s " D_UNIT_WATTHOUR "%c{e}" ;
const char HTTP_ENERGY_PAPP_TELEINFO[] PROGMEM =  "{s}" D_POWERUSAGE_APPARENT "{m}%d " D_UNIT_VA "{e}" ;
const char HTTP_ENERGY_PAPP1_TELEINFO[] PROGMEM =  "{s}" D_POWERUSAGE_APPARENT " p1{m}%d " D_UNIT_VA "{e}" ;
const char HTTP_ENERGY_PAPP2_TELEINFO[] PROGMEM =  "{s}" D_POWERUSAGE_APPARENT " p2{m}%d " D_UNIT_VA "{e}" ;
const char HTTP_ENERGY_PAPP3_TELEINFO[] PROGMEM =  "{s}" D_POWERUSAGE_APPARENT " p3{m}%d " D_UNIT_VA "{e}" ;
//const char HTTP_ENERGY_IINST_TELEINFO[] PROGMEM =  "{s}" D_CURRENT "%s{m}%d " D_UNIT_AMPERE "{e}" ;
const char HTTP_ENERGY_TARIF_TELEINFO_STD[] PROGMEM = "{s}" D_CURRENT_TARIFF "{m}%s{e}" ;
const char HTTP_ENERGY_TARIF_TELEINFO_HISTO[] PROGMEM = "{s}" D_CURRENT_TARIFF "{m}Heures %s{e}" ;
const char HTTP_ENERGY_MSG_TELEINFO_STD[] PROGMEM = "{s}Message %d{m}%s{e}" ;
const char HTTP_ENERGY_CONTRAT_TELEINFO[] PROGMEM =  "{s}" D_CONTRACT "{m}%s %d" D_UNIT_AMPERE "{e}" ;
const char HTTP_ENERGY_LOAD_TELEINFO[] PROGMEM =  "{s}" D_POWER_LOAD "{m}%d" D_UNIT_PERCENT "{e}" ;
const char HTTP_ENERGY_IMAX_TELEINFO[] PROGMEM =  "{s}" D_MAX_CURRENT "{m}%d" D_UNIT_AMPERE "{e}" ;
const char HTTP_ENERGY_IMAX3_TELEINFO[] PROGMEM =  "{s}" D_MAX_CURRENT "{m}%d / %d / %d " D_UNIT_AMPERE "{e}" ;
const char HTTP_ENERGY_PMAX_TELEINFO[] PROGMEM =  "{s}" D_MAX_POWER "{m}%d " D_UNIT_VA "{e}" ;
const char HTTP_ENERGY_PMAX3_TELEINFO[] PROGMEM =  "{s}" D_MAX_POWER "{m}%d / %d / %d " D_UNIT_VA "{e}" ;
const char HTTP_ENERGY_LABEL_VALUE[] PROGMEM =  "{s}%s{m}%s{e}" ;
const char HTTP_ENERGY_LOAD_BAR[] PROGMEM = "<tr><div style='margin:4px;padding:0px;background-color:#ddd;border-radius:4px;'>"
                                            "<div style='font-size:0.75rem;font-weight:bold;padding:0px;text-align:center;border:1px solid #bbb;border-radius:4px;color:#444;background-color:%s;width:%d%%;'>"
                                            "%d%%</div>"
                                            "</div></tr>";
const char HTTP_ENERGY_STATS_TELEINFO[] PROGMEM =   "{s}Bad Checksum{m}%d{e}" 
                                                    "{s}Wrong Size{m}%d{e}" 
                                                    "{s}Bad Format{m}%d{e}" 
                                                    "{s}Interruption{m}%d{e}" ;
#endif  // USE_WEBSERVER



/* ======================================================================
Function: getValueFromLabelIndex
Purpose : return label value from label index
Input   : label index to search for
Output  : value filled
Comments: -
====================================================================== */
char * getValueFromLabelIndex(int labelIndex, char * value)
{
    if (!value) {
        return nullptr;
    }
    char labelName[17];
    *value = '\0';

    // Get the label name
    GetTextIndexed(labelName, sizeof(labelName), labelIndex, kLabel);
    // Get value of label name
    tinfo.valueGet(labelName, value) ;
    
    // Standard mode has values with space before/after
    if (tinfo_mode==TINFO_MODE_STANDARD) {
        Trim(value);
    }

    return *value ? value : nullptr;
}

/* ======================================================================
Function: ADPSCallback
Purpose : called by library when we detected a ADPS on any phased
Input   : phase number
            0 for ADPS (monophase)
            1 for ADIR1 triphase
            2 for ADIR2 triphase
            3 for ADIR3 triphase
          label (different on Normal, 3 phases, historique or standard)
Output  : -
Comments: should have been initialised with a
          tinfo.attachADPSCallback(ADPSCallback())
====================================================================== */
void ADPSCallback(uint8_t phase, char * label)
{
    // n = phase number 1 to 3
    if (phase == 0){
        phase = 1;
    }

    Response_P(PSTR("{\"TIC\":{\"%s\":%i}}"), label, phase );
    // Publish adding ADCO serial number into the topic
    MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR(D_RSLT_SENSOR), false);

    AddLog(LOG_LEVEL_INFO, PSTR("ADPS on phase %d"), phase);
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

    char labelName[17];
    // Find the label index
    for ( ilabel = 1 ; ilabel < LABEL_END ; ilabel++) {
        GetTextIndexed(labelName, sizeof(labelName), ilabel, kLabel);
        if (!strcmp(labelName, me->name)) {
            break;
        }
    }

    // We found valid label
    if (ilabel<LABEL_END) {

        // First values that needs to have energy object updated (in all case)
        // Voltage V (not present on all Smart Meter)
        if ( ilabel == LABEL_TENSION || ilabel == LABEL_URMS1 || ilabel == LABEL_URMS2 || ilabel == LABEL_URMS3)
        {
            Energy->voltage_available = true;
            float volt = (float) atoi(me->value);
            AddLog(LOG_LEVEL_DEBUG, PSTR("TIC: Voltage %s=%s, now %d"), me->name, me->value, (int) volt);

            if ( ilabel == LABEL_URMS2) {
                Energy->voltage[1] = volt;
            } else if ( ilabel == LABEL_URMS3) {
                Energy->voltage[2] = volt;
            } else {
                Energy->voltage[0] = volt;
            }
        }

        // Current I phase 1 to 3
        else if (ilabel == LABEL_IINST
                    || ilabel == LABEL_IINST1 || ilabel == LABEL_IRMS1
                    || ilabel == LABEL_IINST2 || ilabel == LABEL_IRMS2
                    || ilabel == LABEL_IINST3 || ilabel == LABEL_IRMS3  )
        {
            Energy->current_available = true;
            float current = (float) atoi(me->value);
            AddLog(LOG_LEVEL_DEBUG, PSTR("TIC: Current %s=%s, now %d"), me->name, me->value, (int) current);

            if (ilabel == LABEL_IINST2 || ilabel == LABEL_IRMS2) {
                Energy->current[1]  = current;
            } else if (ilabel == LABEL_IINST3 || ilabel == LABEL_IRMS3) {
                Energy->phase_count = 3;
                Energy->current[2] = current;
            } else {
                Energy->current[0] = current;
            }
        }

        // Power P
        else if (ilabel == LABEL_PAPP || ilabel == LABEL_SINSTS || ilabel == LABEL_SINSTS1 || ilabel == LABEL_SINSTS2 || ilabel == LABEL_SINSTS3)
        {
            float power = (float) atoi(me->value);
            AddLog(LOG_LEVEL_DEBUG, PSTR("TIC: Power %s=%s, now %d"), me->name, me->value, (int) power);

            if (ilabel == LABEL_PAPP || ilabel == LABEL_SINSTS1 || (ilabel == LABEL_SINSTS && Energy->phase_count == 1)) {
                Energy->active_power[0] = Energy->apparent_power[0] = power;
            } else if (ilabel == LABEL_SINSTS2) {
                Energy->active_power[1] = Energy->apparent_power[1] = power;
            } else if (ilabel == LABEL_SINSTS3) {
                Energy->active_power[2] = Energy->apparent_power[2] = power;
            }
        }

        // Ok now not so real time values Does this value is new or changed?
        else if (flags & (TINFO_FLAGS_ADDED | TINFO_FLAGS_UPDATED) ) {

            if (flags & TINFO_FLAGS_ADDED)   { c = '#';  }
            if (flags & TINFO_FLAGS_UPDATED) { c = '*';  }
            AddLog(LOG_LEVEL_DEBUG, PSTR("TIC: [%d]%c %s=%s"), ilabel, c , me->name, me->value);

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
                AddLog(LOG_LEVEL_DEBUG, PSTR("TIC: Tariff changed, now '%s' (%d)"), me->value, tarif);
            }

            // Current tariff (standard)
            else if (ilabel == LABEL_LTARF)
            {
                AddLog(LOG_LEVEL_DEBUG, PSTR("TIC: Tariff name changed, now '%s'"), me->value);
            }
            // Current tariff index (standard)
            // This is the index on pointer counter (not used just for information)
            else if (ilabel == LABEL_NTARF)
            {
                int index = atoi(me->value);
                AddLog(LOG_LEVEL_DEBUG, PSTR("TIC: Tariff index changed, now '%d'"), index);
            }

            // Wh indexes (legacy)
            else if ( ilabel == LABEL_HCHC || ilabel == LABEL_HCHP || ilabel == LABEL_BASE)
            {
                char value[32];
                uint32_t hc = 0;
                uint32_t hp = 0;

                // Base, un seul index
                if (ilabel == LABEL_BASE) {
                    total_wh = atol(me->value);
                    AddLog(LOG_LEVEL_DEBUG, PSTR("TIC: Base:%ld"), total_wh);
                // Heures creuses/pleines calculer total
                } else {
                    // Heures creuses get heures pleines
                    if (ilabel == LABEL_HCHC) {
                        hc = atol(me->value);
                        if ( getValueFromLabelIndex(LABEL_HCHP, value) ) {
                            hp = atol(value) ;
                        }

                    // Heures pleines, get heures creuses
                    } else if (ilabel == LABEL_HCHP) {
                        hp = atol(me->value);
                        if ( getValueFromLabelIndex(LABEL_HCHC, value) ) {
                            hc = atol(value) ;
                        }
                    }
                    if (hc>0 && hp>0) {
                        total_wh = hc + hp;
                    }
                    AddLog(LOG_LEVEL_DEBUG, PSTR("TIC: HC:%ld  HP:%ld  Total:%ld"), hc, hp, total_wh);
                }

                AddLog (LOG_LEVEL_DEBUG_MORE, PSTR ("TIC: Total counter updated to %ld Wh"), total_wh);
                if (total_wh>0) {
                    Energy->total[0] = (float) total_wh / 1000.0f;
                    Energy->import_active[0] = Energy->total[0];
                    //Energy->import_active[0] = (float)total/1000.0f;
                    //EnergyUpdateTotal();
                    AddLog (LOG_LEVEL_DEBUG_MORE, PSTR ("TIC: import_active[0]=%.3fKWh"), Energy->import_active[0] );
                }
            }

            // Wh total index (all contract)
            else if ( ilabel == LABEL_EAST)
            {
                total_wh = atol(me->value);
                Energy->total[0] = (float) total_wh / 1000.0f;
                Energy->import_active[0] = Energy->total[0];
                AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("TIC: Total:%ldWh"), total_wh);
            }

            // Wh indexes (standard)
            else if ( ilabel == LABEL_EASF01)
            {
                AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("TIC: HC:%ld"), atol(me->value));
            }
            else if ( ilabel == LABEL_EASF02)
            {
                AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("TIC: HP:%ld"), atol(me->value));
            }

            // Contract subscribed (legacy)
            else if (ilabel == LABEL_OPTARIF)
            {
                char contrat_value[] = "    "; // 4 spaces
                // Find the contract index
                for (contrat = CONTRAT_BAS ; contrat < CONTRAT_END ; contrat++) {
                    GetTextIndexed(contrat_value, sizeof(contrat_value), contrat, kContratValue);
                    if (strstr(me->value, contrat_value)) {
                        break;
                    }
                }
                AddLog(LOG_LEVEL_DEBUG, PSTR("TIC: Contract changed, now '%s' (%d)"), me->value, contrat);
            }
            // Contract subscribed (standard is in clear text in value)
            else if (ilabel == LABEL_NGTF)
            {
                if (strstr_P(me->value, TELEINFO_STD_CONTRACT_BASE)) {
                    contrat = CONTRAT_BAS;
                } else if (strstr_P(me->value, TELEINFO_STD_CONTRACT_HCHP)) {
                    contrat = CONTRAT_HC;
                } else if (strstr_P(me->value, TELEINFO_STD_CONTRACT_BBR)) {
                    contrat = CONTRAT_BBR;
                } else if (strstr_P(me->value, TELEINFO_STD_CONTRACT_EJP)) {
                    contrat = CONTRAT_EJP;
                }

                AddLog(LOG_LEVEL_DEBUG, PSTR("TIC: Contract changed, now '%s'"), me->value);
            }

            // Contract subscribed (I Max)
            else if (ilabel == LABEL_ISOUSC)
            {
                isousc = atoi( me->value);
                AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("TIC: ISousc set to %d"), isousc);
            }

            // Contract subscribed (Power in KVA)
            else if (ilabel == LABEL_PREF)
            {
                // Convert KVA to A
                isousc  = atoi( me->value) * 5  ;

                AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("TIC: ISousc set to %d"), isousc);
            }

            // Serial Number of device
            else if (ilabel == LABEL_ADCO || ilabel == LABEL_ADSC)
            {
                strcpy(serialNumber, me->value);
                AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("TIC: %s set to %s"), me->name, serialNumber);
            }
            // Status
            else if (ilabel == LABEL_STGE)
            {
                status_register = strtol(me->value, nullptr, 16);
                AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("Status Resister : %s set to %08X"), me->name, status_register);
            }
            
        }
    }
}

/* ======================================================================
Function: isBlacklistedLabel
Purpose : check is a label is blacklisted for telemetry data
Input   : label name
Output  : true if blacklisted
Comments: -
====================================================================== */
bool isBlacklistedLabel(char * name)
{
    bool bl = false;
    // return strstr( kLabelBlacklist, name)==nullptr ? false : true;
    if ( strstr(kLabelBlacklist, name) ) {
    //if ( strstr("|PJOURF+1|MSG1|ADCO|", name) ) {
        bl = true;
        AddLog(LOG_LEVEL_DEBUG, PSTR("TIC: %s is blacklisted"), name);
    }

    return bl;
}

/* ======================================================================
Function: ResponseAppendTInfo
Purpose : add teleinfo values into JSON response
Input   : 1st separator space if begining of JSON, else comma
        : select if append all data or just changed one
Output  : false if asked for changed value and none has changed else true
Comments: -
====================================================================== */
bool ResponseAppendTInfo(char sep, bool all)
{
    struct _ValueList * me = tinfo.getList();

    char * p ;
    bool isNumber ;
    bool hasValue = false;

    // Loop thru all the teleinfo frame but
    // always check we don't buffer overflow of MQTT data
    while (me->next) {
        // go to next node
        me = me->next;

        if (me->name && me->value && *me->name && *me->value) {

            // Check back checksum in case of any memory corruption
            if (me->checksum==tinfo.calcChecksum(me->name, me->value)) {

                // Does this label blacklisted ?
                if (!isBlacklistedLabel(me->name)) {

                    // Add values only if we want all data or if data has changed
                    if (all || ( Settings->teleinfo.raw_report_changed && (me->flags & (TINFO_FLAGS_UPDATED | TINFO_FLAGS_ADDED | TINFO_FLAGS_ALERT) ) ) ) {

                        isNumber = true;
                        hasValue = true;
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

                        // Avoid unneeded space
                        if (sep == ' ') {
                            ResponseAppend_P( PSTR("\"%s\":"), me->name );
                        } else {
                            ResponseAppend_P( PSTR("%c\"%s\":"), sep, me->name );
                        }

                        if (!isNumber) {
                            // Some values contains space 
                            if (strcmp(me->name, "NGTF")==0 || strcmp(me->name, "LTARF")==0 || strcmp(me->name, "MSG1")==0) {
                                char trimmed_value[strlen(me->value)+1];
                                strcpy(trimmed_value, me->value);
                                ResponseAppend_P( PSTR("\"%s\""), Trim(trimmed_value) );
                            } else {
                                ResponseAppend_P( PSTR("\"%s\""), me->value );
                            }

                        } else {
                            ResponseAppend_P( PSTR("%ld"), atol(me->value));
                        }

                        // Now JSON separator is needed
                        sep =',';
                    }
                }

            } else {
                AddLog(LOG_LEVEL_INFO, PSTR("TIC: bad checksum for %s"), me->name);
            }
        }
    }

    return hasValue;
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
    Energy->data_valid[0] = 0;

    // Deprecated see setOption108
    // send teleinfo MQTT raw data only if setup like that
    if (Settings->teleinfo.raw_send) {
        // Do we need to skip this frame
        if (raw_skip == 0 ) {
            Response_P(PSTR("{\"TIC\":{"));
            // send teleinfo full frame or only changed data
            bool hasData = ResponseAppendTInfo(' ', Settings->teleinfo.raw_report_changed ? false : true );
            ResponseJsonEndEnd();

            // Publish adding ADCO serial number into the topic
            // Need setOption4 to be enabled
            // No need to send empty payload
            if (hasData) {
                MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR(D_RSLT_SENSOR), false);
            }

            // Reset frame skip counter (if 0 it's disabled)
            raw_skip = Settings->teleinfo.raw_skip;
        } else {
            AddLog(LOG_LEVEL_DEBUG, PSTR("TIC: not sending yet, will do in %d frame(s)"), raw_skip);
            raw_skip--;
        }
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
    // If one TInfo RX pin declared use it
    if (PinUsed(GPIO_TELEINFO_RX)) {
        tic_rx_pin = Pin(GPIO_TELEINFO_RX);
        TasmotaGlobal.energy_driver = XNRG_15;
        Energy->voltage_available = false;
        Energy->phase_count = 1;
        // init hardware energy counters
        total_wh = 0;
        Settings->flag3.hardware_energy_total = true;
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
    if (!PinUsed(GPIO_TELEINFO_RX)) { return; }       // ignore if pin not set
    int baudrate;

    // Deprecated SetOption102 - Set Baud rate for Teleinfo serial communication (0 = 1200 or 1 = 9600)
    // now set in bit field TeleinfoCfg
    if (Settings->teleinfo.mode_standard) {
        baudrate = 9600;
        tinfo_mode = TINFO_MODE_STANDARD;
        serial_buffer_size = TELEINFO_SERIAL_BUFFER_STANDARD;
    } else {
        baudrate = 1200;
        tinfo_mode = TINFO_MODE_HISTORIQUE;
        serial_buffer_size = TELEINFO_SERIAL_BUFFER_HISTORIQUE;
    }

    AddLog(LOG_LEVEL_INFO, PSTR("TIC: RX on GPIO%d, baudrate %d"), tic_rx_pin, baudrate);

    // Enable Teleinfo pin used, control it
    if (PinUsed(GPIO_TELEINFO_ENABLE)) {
        int8_t en_pin = Pin(GPIO_TELEINFO_ENABLE);
        pinMode(en_pin, OUTPUT);
        digitalWrite(en_pin, HIGH);
        AddLog(LOG_LEVEL_INFO, PSTR("TIC: Enable with GPIO%d"), en_pin);
    }

#ifdef ESP8266
    // Allow GPIO3 AND GPIO13 with hardware fallback to 2
    // Set buffer to nnn char to support 250ms loop at 9600 baud
    // In case of "on-the-fly" re-init, serial is already there, don't re-allocate
    if (!TInfoSerial) {
        TInfoSerial = new TasmotaSerial(tic_rx_pin, -1, 2, 0, serial_buffer_size);
    }
    //pinMode(rx_pin, INPUT_PULLUP);
#endif  // ESP8266
#ifdef ESP32
    // Set buffer to nnn char to support 250ms loop at 9600 baud
    // In case of "on-the-fly" re-init, serial is already there, don't re-allocate
    if (!TInfoSerial) {
        TInfoSerial = new TasmotaSerial(tic_rx_pin, -1, 1, 0, serial_buffer_size);
    }
#endif  // ESP32

    if (TInfoSerial && TInfoSerial->begin(baudrate, SERIAL_7E1)) {
#ifdef ESP8266
        if (TInfoSerial->hardwareSerial() ) {
            ClaimSerial();

            // This is a hack, looks like begin does not take into account
            // the TS_SERIAL_7E1 configuration so on ESP8266 this is
            // working only on Serial RX pin (Hardware Serial) for now

            //SetSerialConfig(TS_SERIAL_7E1);
            //TInfoSerial->setTimeout(TINFO_READ_TIMEOUT);

            AddLog(LOG_LEVEL_INFO, PSTR("TIC: using hardware serial"));
        } else {
            AddLog(LOG_LEVEL_INFO, PSTR("TIC: using software serial"));
        }
#endif  // ESP8266
#ifdef ESP32
        AddLog(LOG_LEVEL_DEBUG, PSTR("TIC: using hardserial %d"), TInfoSerial->getUart());
#endif
        // Init teleinfo
        tinfo.init(tinfo_mode);
        // Attach needed callbacks
        tinfo.attachADPS(ADPSCallback);
        tinfo.attachData(DataCallback);
        tinfo.attachNewFrame(NewFrameCallback);
        tinfo_found = true;

        if (Settings->teleinfo.raw_send) {
            raw_skip = Settings->teleinfo.raw_skip;
            AddLog(LOG_LEVEL_INFO, PSTR("TIC: Raw mode enabled"));
            if (raw_skip) {
                AddLog(LOG_LEVEL_INFO, PSTR("TIC: Sending only one frame over %d "), raw_skip+1);
            }
        }
    } else {
        AddLog(LOG_LEVEL_INFO, PSTR("TIC: failed init serial"));
    }
}

// 
/* ======================================================================
Function: TInfoSaveBeforeRestart
Purpose : Save data before ESP restart
Input   : -
Output  : -
Comments: -
====================================================================== */
void TInfoSaveBeforeRestart()
{
    // if teleinfo enabled, set it low
    if (PinUsed (GPIO_TELEINFO_ENABLE)) {
        digitalWrite( Pin(GPIO_TELEINFO_ENABLE), LOW);
    }

    // update energy total (in kwh)
    EnergyUpdateTotal();

}

/* ======================================================================
Function: TInfoCmd
Purpose : Tasmota core command engine for Teleinfo commands
Input   : -
Output  : -
Comments: -
====================================================================== */
bool TInfoCmd(void) {
    bool serviced = false;
    char command[CMDSZ];
    //uint8_t name_len = strlen(D_NAME_TELEINFO);

    // At least "EnergyConfig"
    if (CMND_ENERGYCONFIG == Energy->command_code) {

        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("TIC: len %d, data '%s'"), XdrvMailbox.data_len, XdrvMailbox.data ? XdrvMailbox.data : "null" );

        // Just "EnergyConfig" no more parameter
        // Show Teleinfo configuration
        if (XdrvMailbox.data_len == 0) {
            char mode_name[MAX_TINFO_COMMAND_NAME];
            char raw_name[MAX_TINFO_COMMAND_NAME];
            char rx_pin[8] = "None";
            char en_pin[8] = "None" ;
            int index_mode = Settings->teleinfo.mode_standard ? CMND_TELEINFO_STANDARD : CMND_TELEINFO_HISTORIQUE;
            int index_raw = Settings->teleinfo.raw_send ? CMND_TELEINFO_RAW_FULL : CMND_TELEINFO_RAW_DISABLE;
            if (Settings->teleinfo.raw_send && Settings->teleinfo.raw_report_changed) {
                index_raw = CMND_TELEINFO_RAW_CHANGE;
            }
            // Get the mode and raw name
            GetTextIndexed(mode_name, MAX_TINFO_COMMAND_NAME, index_mode, kTInfo_Commands);
            GetTextIndexed(raw_name, MAX_TINFO_COMMAND_NAME, index_raw, kTInfo_Commands);

            if (PinUsed(GPIO_TELEINFO_RX)) {
                sprintf_P(rx_pin, PSTR("GPIO%d"), tic_rx_pin);
            }
            if (PinUsed(GPIO_TELEINFO_ENABLE)) {
                sprintf_P(en_pin, PSTR("GPIO%d"), Pin(GPIO_TELEINFO_ENABLE));
            }

            AddLog(LOG_LEVEL_INFO, TELEINFO_COMMAND_SETTINGS, mode_name, rx_pin, en_pin, raw_name, Settings->teleinfo.raw_skip, Settings->teleinfo.raw_limit, Settings->teleinfo.show_stats);

            serviced = true;

        // At least "EnergyConfig xyz" plus one space and one (or more) char
        // so "EnergyConfig 0" or "EnergyConfig Teleinfo Standard"
        } else if (XdrvMailbox.data_len) {
            // Now point on parameter
            char *pParam = XdrvMailbox.data;
            char *p = pParam;
            char *pValue = nullptr;
            // Check for sub parameter ie : EnergyConfig Teleinfo Skip value
            while(*p) {
                if (*p == ' ') {
                    if (*(p+1)) {
                        // Skip parameter by emptying th string so below getcommandcode works
                        *p++ = 0x00;
                        // Save parameter value for later
                        pValue = p;
                    }
                    break;
                }
                p++;
            }

            int command_code = GetCommandCode(command, sizeof(command), pParam, kTInfo_Commands);

            AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("TIC: param '%s' cmnd %d"), pParam, command_code);

            switch (command_code) {
                case CMND_TELEINFO_STANDARD:
                case CMND_TELEINFO_HISTORIQUE: {
                    char mode_name[MAX_TINFO_COMMAND_NAME];

                    // Get the mode name
                    GetTextIndexed(mode_name, MAX_TINFO_COMMAND_NAME, command_code, kTInfo_Commands);

                    // Only if current settings is different than previous
                    if ( (tinfo_mode==TINFO_MODE_STANDARD && command_code==CMND_TELEINFO_HISTORIQUE) ||
                         (tinfo_mode==TINFO_MODE_HISTORIQUE && command_code==CMND_TELEINFO_STANDARD) ) {
                        // Change mode
                        Settings->teleinfo.mode_standard = command_code == CMND_TELEINFO_STANDARD ? 1 : 0;
                        AddLog(LOG_LEVEL_INFO, PSTR("TIC: '%s' mode"), mode_name);
                        // Re init teleinfo (LibTeleinfo always free linked list on init)
                        TInfoInit();

                        serviced = true;
                    } else {
                        AddLog(LOG_LEVEL_INFO, PSTR("TIC: No change to '%s' mode"), mode_name);
                    }
                }
                break;

                case CMND_TELEINFO_RAW_DISABLE:
                case CMND_TELEINFO_RAW_FULL:
                case CMND_TELEINFO_RAW_CHANGE: {

                   // Enable all RAW frame send
                   char raw_name[MAX_TINFO_COMMAND_NAME];

                    // Get the raw name
                    GetTextIndexed(raw_name, MAX_TINFO_COMMAND_NAME, command_code, kTInfo_Commands);

                    if (command_code == CMND_TELEINFO_RAW_DISABLE) {
                        // disable raw mode
                        Settings->teleinfo.raw_send = 0;
                    } else {
                        // enable raw mode
                        Settings->teleinfo.raw_send = 1;
                        Settings->teleinfo.raw_report_changed = command_code == CMND_TELEINFO_RAW_CHANGE ? 1 : 0;
                    }

                    AddLog(LOG_LEVEL_INFO, PSTR("TIC: Raw to '%s'"), raw_name);
                    serviced = true;
                }
                break;

                case CMND_TELEINFO_SKIP: {
                    // Set Raw mode skip frame number
                    char skip_name[MAX_TINFO_COMMAND_NAME];
                    // Get the raw name
                    GetTextIndexed(skip_name, MAX_TINFO_COMMAND_NAME, command_code, kTInfo_Commands);
                    int l = strlen(skip_name);

                    // At least "EnergyConfig Teleinfo skip" plus one space and one (or more) digit
                    // so "EnergyConfig Skip 0" or "EnergyConfig Skip 123"
                    if ( pValue ) {
                        int value = atoi(pValue);
                        if (value >= 0 && value <= 255) {
                            raw_skip = value;
                            Settings->teleinfo.raw_skip = raw_skip;

                            if (raw_skip ==0) {
                                AddLog(LOG_LEVEL_INFO, PSTR("TIC: Raw no skip"));
                            } else {
                                AddLog(LOG_LEVEL_INFO, PSTR("TIC: Raw each %d frame(s)"), raw_skip+1);
                            }
                            serviced = true;
                        } else {
                            AddLog(LOG_LEVEL_INFO, PSTR("TIC: skip can be 0 to 255"));
                        }
                    } else {
                        AddLog(LOG_LEVEL_INFO, PSTR("TIC: no skip value"));
                    }
                }
                break;

                case CMND_TELEINFO_STATS: {
                    char stats_name[MAX_TINFO_COMMAND_NAME];
                    // Get the raw name
                    GetTextIndexed(stats_name, MAX_TINFO_COMMAND_NAME, command_code, kTInfo_Commands);
                    int l = strlen(stats_name);
                    // At least "EnergyConfig Stats" plus one space and one (or more) digit
                    // so "EnergyConfig Stats" or "EnergyConfig Stats 0"
                    if ( pValue ) {
                        int value = atoi(pValue);
                        if (value==0 || value==1) {
                            Settings->teleinfo.show_stats = value ;
                            AddLog(LOG_LEVEL_INFO, PSTR("TIC: Show stats=%d"), value);
                        } else if (value == 2) {
                            tinfo.clearStats();
                            AddLog(LOG_LEVEL_INFO, PSTR("TIC: Stats cleared"));
                        } else {
                            AddLog(LOG_LEVEL_INFO, PSTR("TIC: bad Stats param '%d'"), value);
                        }
                    }
                    // Show stats 
                    AddLog(LOG_LEVEL_INFO, PSTR("TIC: Frame error CheckSum:%d Size:%d Format:%d Interrupt:%d"), 
                                                tinfo.getChecksumErrorCount(), 
                                                tinfo.getFrameSizeErrorCount(), 
                                                tinfo.getFrameFormatErrorCount(), 
                                                tinfo.getFrameInterruptedCount() );
                }
                break;

                default:
                    AddLog(LOG_LEVEL_INFO, PSTR("TIC: bad cmd param '%s'"), pParam);
                break;

            }
        }
    }
    return serviced ;
}

/* ======================================================================
Function: TInfoProcess
Purpose : Tasmota callback executed often enough to read serial
Input   : -
Output  : -
Comments: -
====================================================================== */
//#define MEASURE_PERF // Define to enable performance measurments
void TInfoProcess(void)
{
    static char buff[TELEINFO_PROCESS_BUFFER];
    static uint32_t tick_update = 0;
    #ifdef MEASURE_PERF
    static unsigned long max_time = 0;
    unsigned long duration = millis();
    static int max_size = 0;
    int tmp_size = 0;
    #endif

    if (!tinfo_found) {
        return;
    }

    int size = TInfoSerial->read(buff,TELEINFO_PROCESS_BUFFER);
    while ( size ) {
        #ifdef MEASURE_PERF
        tmp_size += size;
        #endif
        // Process as many bytes as available in serial buffer
        for(int i = 0; size; i++, size--)
        {
            buff[i] &= 0x7F;
            // data processing
            tinfo.process(buff[i]);
        }
        size = TInfoSerial->read(buff,TELEINFO_PROCESS_BUFFER);
    }
    #ifdef MEASURE_PERF
    duration = millis()-duration;
    if (duration > max_time) { max_time = duration; AddLog(LOG_LEVEL_INFO,PSTR("TIC: max_time=%lu"), max_time); }
    if (tmp_size > max_size) { max_size = tmp_size; AddLog(LOG_LEVEL_INFO,PSTR("TIC: max_size=%d"), max_size); }
    #endif

    // if needed, update energy total every hour
    if (tick_update++ > 3600 * 4) {
        EnergyUpdateTotal();
        AddLog (LOG_LEVEL_DEBUG, PSTR ("TIC: Total counter updated to %lu Wh"), total_wh);
        tick_update = 0;
    }

}

#ifdef USE_WEBSERVER
/* ======================================================================
Function: TInfoShowBASE
Purpose : Display Base contract on WEB Interface
====================================================================== */
void TInfoShowBASE(void) 
{
    char name[17];
    char value[17];
    if ( tinfo_mode==TINFO_MODE_HISTORIQUE ) {
        if (getValueFromLabelIndex(LABEL_BASE, value) ) {
            GetTextIndexed(name, sizeof(name), LABEL_BASE, kLabel);
            WSContentSend_P(HTTP_ENERGY_INDEX_TELEINFO, name, value);
        }
    } else {
        if (getValueFromLabelIndex(LABEL_EAST, value) ) {
            GetTextIndexed(name, sizeof(name), LABEL_EAST, kLabel);
            WSContentSend_P(HTTP_ENERGY_INDEX_TELEINFO, name, value);
        }
    }
}

/* ======================================================================
Function: TInfoShowHC
Purpose : Display HC/HP contract on WEB Interface
====================================================================== */
void TInfoShowHC(void)  
{
    char name[17];
    char value[17];
    if ( tinfo_mode==TINFO_MODE_HISTORIQUE ) {
        if (getValueFromLabelIndex(LABEL_HCHC, value) ) {
            GetTextIndexed(name, sizeof(name), LABEL_HCHC, kLabel);
            WSContentSend_P(HTTP_ENERGY_INDEX_TELEINFO, name, value);
        }
        if (getValueFromLabelIndex(LABEL_HCHP, value) ) {
            GetTextIndexed(name, sizeof(name), LABEL_HCHP, kLabel);
            WSContentSend_P(HTTP_ENERGY_INDEX_TELEINFO, name, value);
        }
    } else {
        int index = 0;
        if (getValueFromLabelIndex(LABEL_NTARF, value) ) {
            index = atoi(value);
        }
        if (getValueFromLabelIndex(LABEL_EAST, value) ) {
            GetTextIndexed(name, sizeof(name), LABEL_EAST, kLabel);
            WSContentSend_P(HTTP_ENERGY_INDEX_TELEINFO, name, value);
        }
        if (getValueFromLabelIndex(LABEL_EASF01, value) ) {
            GetTextIndexed(name, sizeof(name), LABEL_EASF01, kLabel);
            WSContentSend_P(HTTP_ENERGY_INDEX_TELEINFO_SELECT, name, value, index==1?'*':' ');
        }
        if (getValueFromLabelIndex(LABEL_EASF02, value) ) {
            GetTextIndexed(name, sizeof(name), LABEL_EASF02, kLabel);
            WSContentSend_P(HTTP_ENERGY_INDEX_TELEINFO_SELECT, name, value, index==2?'*':' ');
            //WSContentSend_P(HTTP_ENERGY_INDEX_TELEINFO, name, value);
        }
    }
}
/* ======================================================================
Function: TInfoShowBBR
Purpose : Display Bleu Blanc Rouge contract on WEB Interface
====================================================================== */
void TInfoShowBBR(void) 
{
    char name[17];
    char value[17];
    if ( tinfo_mode==TINFO_MODE_HISTORIQUE ) {
        // Contrat Tempo BBR
        if (getValueFromLabelIndex(LABEL_HCJB, value) ) {
            GetTextIndexed(name, sizeof(name), LABEL_HCJB, kLabel);
            WSContentSend_P(HTTP_ENERGY_INDEX_TELEINFO, name, value);
        }
        if (getValueFromLabelIndex(LABEL_HPJB, value) ) {
            GetTextIndexed(name, sizeof(name), LABEL_HPJB, kLabel);
            WSContentSend_P(HTTP_ENERGY_INDEX_TELEINFO, name, value);
        }
        if (getValueFromLabelIndex(LABEL_HCJW, value) ) {
            GetTextIndexed(name, sizeof(name), LABEL_HCJW, kLabel);
            WSContentSend_P(HTTP_ENERGY_INDEX_TELEINFO, name, value);
        }
        if (getValueFromLabelIndex(LABEL_HPJW, value) ) {
            GetTextIndexed(name, sizeof(name), LABEL_HPJW, kLabel);
            WSContentSend_P(HTTP_ENERGY_INDEX_TELEINFO, name, value);
        }
        if (getValueFromLabelIndex(LABEL_HCJR, value) ) {
            GetTextIndexed(name, sizeof(name), LABEL_HCJR, kLabel);
            WSContentSend_P(HTTP_ENERGY_INDEX_TELEINFO, name, value);
        }
        if (getValueFromLabelIndex(LABEL_HPJR, value) ) {
            GetTextIndexed(name, sizeof(name), LABEL_HPJR, kLabel);
            WSContentSend_P(HTTP_ENERGY_INDEX_TELEINFO, name, value);
        }
        if (getValueFromLabelIndex(LABEL_DEMAIN, value) ) {
            GetTextIndexed(name, sizeof(name), LABEL_DEMAIN, kLabel);
            WSContentSend_P(HTTP_ENERGY_LABEL_VALUE, name, value);
        }
    } else {
        int index = 0;
        if (getValueFromLabelIndex(LABEL_NTARF, value) ) {
            index = atoi(value);
        }
        if (getValueFromLabelIndex(LABEL_EASF01, value) ) {
            GetTextIndexed(name, sizeof(name), LABEL_EASF01, kLabel);
            WSContentSend_P(HTTP_ENERGY_INDEX_TELEINFO_SELECT, name, value, index==1?'*':' ');
        }
        if (getValueFromLabelIndex(LABEL_EASF02, value) ) {
            GetTextIndexed(name, sizeof(name), LABEL_EASF02, kLabel);
            WSContentSend_P(HTTP_ENERGY_INDEX_TELEINFO_SELECT, name, value, index==2?'*':' ');
        }
        if (getValueFromLabelIndex(LABEL_EASF03, value) ) {
            GetTextIndexed(name, sizeof(name), LABEL_EASF03, kLabel);
            WSContentSend_P(HTTP_ENERGY_INDEX_TELEINFO_SELECT, name, value, index==3?'*':' ');
        }
        if (getValueFromLabelIndex(LABEL_EASF04, value) ) {
            GetTextIndexed(name, sizeof(name), LABEL_EASF04, kLabel);
            WSContentSend_P(HTTP_ENERGY_INDEX_TELEINFO_SELECT, name, value, index==4?'*':' ');
        }
        if (getValueFromLabelIndex(LABEL_EASF05, value) ) {
            GetTextIndexed(name, sizeof(name), LABEL_EASF05, kLabel);
            WSContentSend_P(HTTP_ENERGY_INDEX_TELEINFO_SELECT, name, value, index==5?'*':' ');
        }
        if (getValueFromLabelIndex(LABEL_EASF06, value) ) {
            GetTextIndexed(name, sizeof(name), LABEL_EASF06, kLabel);
            WSContentSend_P(HTTP_ENERGY_INDEX_TELEINFO_SELECT, name, value, index==6?'*':' ');
        }
    }

}
#endif

/* ======================================================================
Function: TInfoShow
Purpose : Tasmota callback executed to send telemetry or WEB display
Input   : -
Output  : -
Comments: -
====================================================================== */
void TInfoShow(bool json)
{
    // Since it's an Energy device , current, voltage and power are
    // already present on the telemetry frame. No need to add here
    // Just add the raw label/values of the teleinfo frame
    if (json)
    {
        // Add new value (not part of TIC JSON Object)
        if (isousc) {
            ResponseAppend_P(PSTR(",\"Load\":%d"),(int) ((Energy->current[0]*100.0f) / isousc));
        }

        // add teleinfo TIC object
        ResponseAppend_P(PSTR("},\"TIC\":{"));
        ResponseAppendTInfo(' ', true);

#ifdef USE_WEBSERVER
    }
    else
    {
        char name[33];
        char value[33];
        int percent;
        int papp = -1;

        if (isousc) {
            uint8_t hue;
            uint8_t red, green, blue;
            char phase_color[8];

            for (int i=0; i<Energy->phase_count ; i++ ) {
                percent = (int) ((Energy->current[i]*100.0f) / (isousc / Energy->phase_count)) ;
                if (percent > 100) {
                    percent = 100;
                }
                // Gradiant from green (low load), yellow, orange and ending red (high load)
                // Hue from 128 (green) to 0 (red) so reversed from percent
                hue = changeUIntScale(100-percent, 0, 100, 0, 128);
                HsToRgb(hue, 128, &red, &green, &blue);
                snprintf_P(phase_color, sizeof(phase_color), PSTR("#%02X%02X%02X"), red, green, blue);
                WSContentSend_P(HTTP_ENERGY_LOAD_BAR, phase_color, percent, percent);
            }
        }

        if (tinfo_mode==TINFO_MODE_HISTORIQUE ) {
            if ( getValueFromLabelIndex(LABEL_PAPP, value) ) {
                WSContentSend_P(HTTP_ENERGY_PAPP_TELEINFO, atoi(value));
            }
        } else {
            if (getValueFromLabelIndex(LABEL_SINSTS, value) ) {
                WSContentSend_P(HTTP_ENERGY_PAPP_TELEINFO, atoi(value));
            } else if (getValueFromLabelIndex(LABEL_SINSTS1, value)) {
                WSContentSend_P(HTTP_ENERGY_PAPP1_TELEINFO, atoi(value));
            } else if (getValueFromLabelIndex(LABEL_SINSTS2, value)) {
                WSContentSend_P(HTTP_ENERGY_PAPP2_TELEINFO, atoi(value));
            } else if (getValueFromLabelIndex(LABEL_SINSTS3, value)) {
                WSContentSend_P(HTTP_ENERGY_PAPP3_TELEINFO, atoi(value));
            }
        }

        // Show indexes depending on contract
        if ( contrat == CONTRAT_BAS ) {
            TInfoShowBASE();
        } else if ( contrat == CONTRAT_HC ) {
            TInfoShowHC();
        } else if ( contrat == CONTRAT_BBR || contrat == CONTRAT_EJP ) {
            TInfoShowBBR();
        }

        if (tinfo_mode==TINFO_MODE_HISTORIQUE ) {
            if (Energy->phase_count==3) {
                int imax[3];
                for (int i=LABEL_IMAX1; i<=LABEL_IMAX3; i++) {
                    if (getValueFromLabelIndex(i, value) ) {
                        imax[i-LABEL_IMAX1] = atoi(value);
                    }
                }
                WSContentSend_P(HTTP_ENERGY_IMAX3_TELEINFO, imax[0], imax[1], imax[2]);
            } else {
                if (getValueFromLabelIndex(LABEL_IMAX, value) ) {
                    WSContentSend_P(HTTP_ENERGY_IMAX_TELEINFO, atoi(value));
                }
            }

            if (getValueFromLabelIndex(LABEL_PMAX, value) ) {
                WSContentSend_P(HTTP_ENERGY_PMAX_TELEINFO, atoi(value));
            }

            if (tarif) {
                GetTextIndexed(name, sizeof(name), tarif-1, kTarifName);
                if (tinfo_mode==TINFO_MODE_STANDARD ) {
                    WSContentSend_P(HTTP_ENERGY_TARIF_TELEINFO_STD, name);
                } else {
                    WSContentSend_P(HTTP_ENERGY_TARIF_TELEINFO_HISTO, name);
                }
            }
            if (contrat && isousc) {
                int percent = (int) ((Energy->current[0]*100.0f) / isousc) ;
                GetTextIndexed(name, sizeof(name), contrat, kContratName);
                WSContentSend_P(HTTP_ENERGY_CONTRAT_TELEINFO, name, isousc);
               //WSContentSend_P(HTTP_ENERGY_LOAD_TELEINFO,  percent);
            }

        } else if (tinfo_mode==TINFO_MODE_STANDARD ) {
            if (getValueFromLabelIndex(LABEL_SMAXSN, value) ) {
                WSContentSend_P(HTTP_ENERGY_PMAX_TELEINFO, atoi(value));
            }
            if (getValueFromLabelIndex(LABEL_LTARF, value) ) {
                WSContentSend_P(HTTP_ENERGY_TARIF_TELEINFO_STD, Trim(value));
            }
            if (getValueFromLabelIndex(LABEL_NGTF, value) ) {
                if (isousc) {
                    WSContentSend_P(HTTP_ENERGY_CONTRAT_TELEINFO, Trim(value), isousc);
                }
            }
            if (getValueFromLabelIndex(LABEL_MSG1, value) ) {
                WSContentSend_P(HTTP_ENERGY_LABEL_VALUE, PSTR("Message 1"), Trim(value));
            }
            if (getValueFromLabelIndex(LABEL_MSG2, value) ) {
                WSContentSend_P(HTTP_ENERGY_LABEL_VALUE, PSTR("Message 2"), Trim(value));
            }
            WSContentSend_P(HTTP_ENERGY_LABEL_VALUE, PSTR("Contact Sec"), status_register & 0x01 ? "Ouvert":"Fermé");
            if (status_register & 0x08) {
                WSContentSend_P(HTTP_ENERGY_LABEL_VALUE, PSTR("ADPS"), "En cours");
            }
            if (status_register >> 24) {
                char txt[32]; 
                uint8_t sr = status_register >> 24;
                uint8_t val = sr & 0x03;
                if (val) {
                    WSContentSend_P(HTTP_ENERGY_LABEL_VALUE, PSTR("Jour"), val==1?"Bleu":val==2?"Blanc":"Rouge");
                }
                val = (sr >> 2) & 0x03;
                if (val) {
                    WSContentSend_P(HTTP_ENERGY_LABEL_VALUE, PSTR("Demain"), val==1?"Bleu":val==2?"Blanc":"Rouge");
                }
                val = (sr >> 4) & 0x03;
                if (val) {
                    sprintf_P(txt, PSTR("Pointe mobile %d"), val);
                    WSContentSend_P(HTTP_ENERGY_LABEL_VALUE, PSTR("Préavis"), txt);
                }
                val = (sr >> 6) & 0x03;
                if (val) {
                    sprintf_P(txt, PSTR("En cours %d"), val);
                    WSContentSend_P(HTTP_ENERGY_LABEL_VALUE, PSTR("Pointe mobile"), txt);
                }
            }
        }

        // Serial number ADCO or ADSC if found
        if (*serialNumber) {
            WSContentSend_P(HTTP_ENERGY_ID_TELEINFO, serialNumber);
        }

        if (Settings->teleinfo.show_stats) {
            WSContentSend_P(HTTP_ENERGY_STATS_TELEINFO, tinfo.getChecksumErrorCount(), tinfo.getFrameSizeErrorCount()
                                                      , tinfo.getFrameFormatErrorCount(), tinfo.getFrameInterruptedCount());
        }

#endif  // USE_WEBSERVER
    }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xnrg15(uint32_t function)
{
    bool result = false;
    switch (function)
    {
        case FUNC_INIT:
            TInfoInit();
            break;
        case FUNC_SAVE_BEFORE_RESTART:
            if (tinfo_found) { TInfoSaveBeforeRestart(); }
            break;            
        case FUNC_PRE_INIT:
            TInfoDrvInit();
            break;
        case FUNC_EVERY_250_MSECOND:
            if (tinfo_found) { TInfoProcess(); }
            break;
        case FUNC_COMMAND:
            if (tinfo_found) { result = TInfoCmd(); }
            break;

        case FUNC_JSON_APPEND:
            if (tinfo_found) { TInfoShow(1); }
            break;
    #ifdef USE_WEBSERVER
        case FUNC_WEB_SENSOR:
            if (tinfo_found) { TInfoShow(0); }
            break;
    #endif  // USE_WEBSERVER
    }
    return result;
}

#endif  // USE_TELEINFO
#endif // USE_ENERGY_SENSOR
