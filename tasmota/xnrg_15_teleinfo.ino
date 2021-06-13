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
const char TELEINFO_COMMAND_SETTINGS[] PROGMEM = "TIC: Settings Mode:%s, Raw:%s, Skip:%d, Limit:%d";

#define MAX_TINFO_COMMAND_NAME 16+1 // Change this if one of the following kTInfo_Commands is higher then 16 char
const char kTInfo_Commands[] PROGMEM  = "historique|standard|noraw|full|changed|skip|limit";

enum TInfoCommands {            // commands for Console
  CMND_TELEINFO_HISTORIQUE=0,   // Set Legacy mode
  CMND_TELEINFO_STANDARD,       // Set Standard Mode
  CMND_TELEINFO_RAW_DISABLE,    // Disable Raw frame sending
  CMND_TELEINFO_RAW_FULL,       // Enable all RAW frame send
  CMND_TELEINFO_RAW_CHANGE,     // Enable only changed values RAW frame send
  CMND_TELEINFO_SKIP,           // Set number of frame to skip when raw mode is enabled
  CMND_TELEINFO_LIMIT           // Limit RAW frame to values subject to fast change (Power, Current, ...), TBD
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

// contract name for standard mode
#define TELEINFO_STD_CONTRACT_BASE  PSTR("BASE")
#define TELEINFO_STD_CONTRACT_HCHP  PSTR("H CREUSE/PLEINE")

// tariff values for standard mode
#define TELEINFO_STD_TARIFF_BASE    PSTR("BASE")
#define TELEINFO_STD_TARIFF_HC      PSTR("HEURE CREUSE")
#define TELEINFO_STD_TARIFF_HP      PSTR("HEURE PLEINE") 


// Label used to do some post processing and/or calculation
enum TInfoLabel{
    LABEL_BASE = 1,
    LABEL_ADCO, LABEL_ADSC,
    LABEL_HCHC, LABEL_HCHP, LABEL_EAST, LABEL_EASF01, LABEL_EASF02,
    LABEL_OPTARIF, LABEL_NGTF, LABEL_ISOUSC, LABEL_PREF, LABEL_PTEC, LABEL_LTARF, LABEL_NTARF,
    LABEL_PAPP, LABEL_SINSTS, LABEL_IINST, LABEL_IINST1, LABEL_IINST2, LABEL_IINST3, LABEL_IRMS1, LABEL_IRMS2, LABEL_IRMS3,  
    LABEL_TENSION, LABEL_URMS1, LABEL_URMS2, LABEL_URMS3,
    LABEL_IMAX, LABEL_IMAX1, LABEL_IMAX2, LABEL_IMAX3, LABEL_PMAX, LABEL_SMAXSN,
    LABEL_DEMAIN,
    LABEL_END
};

const char kLabel[] PROGMEM =
    "|BASE|ADCO|ADSC"
    "|HCHC|HCHP|EAST|EASF01|EASF02"
    "|OPTARIF|NGTF|ISOUSC|PREF|PTEC|LTARF|NTARF"
    "|PAPP|SINSTS|IINST|IINST1|IINST2|IINST3|IRMS1|IRMS2|IRMS3"
    "|TENSION|URMS1|URMS2|URMS3"
    "|IMAX|IMAX1|IMAX2|IMAX3|PMAX|SMAXSN"
    "|DEMAIN"
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
    "|"
    ;

#define TELEINFO_SERIAL_BUFFER_STANDARD        512      // Receive buffer size for Standard mode
#define TELEINFO_SERIAL_BUFFER_HISTORIQUE      512      // Receive buffer size for Legacy mode
#define TELEINFO_PROCESS_BUFFER                 32      // Local processing buffer

TInfo tinfo; // Teleinfo object
TasmotaSerial *TInfoSerial = nullptr;
_Mode_e tinfo_mode = TINFO_MODE_HISTORIQUE;
uint8_t tic_rx_pin = NOT_A_PIN;
char serialNumber[13] = ""; // Serial number is 12 char long
bool tinfo_found = false;
int serial_buffer_size;
int contrat;
int tarif;
int isousc;
int raw_skip;

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
            Energy.voltage_available = true;
            float volt = (float) atoi(me->value);
            AddLog(LOG_LEVEL_DEBUG, PSTR("TIC: Voltage %s=%s, now %d"), me->name, me->value, (int) volt);

            if ( ilabel == LABEL_URMS2) {  
                Energy.voltage[1] = volt;
            } else if ( ilabel == LABEL_URMS3) {
                Energy.voltage[2] = volt;
            } else {
                Energy.voltage[0] = volt;
            }
        }

        // Current I phase 1 to 3
        else if (ilabel == LABEL_IINST 
                    || ilabel == LABEL_IINST1 || ilabel == LABEL_IRMS1
                    || ilabel == LABEL_IINST2 || ilabel == LABEL_IRMS2
                    || ilabel == LABEL_IINST3 || ilabel == LABEL_IRMS3  )
        {
            Energy.current_available = true;
            float current = (float) atoi(me->value);
            AddLog(LOG_LEVEL_DEBUG, PSTR("TIC: Current %s=%s, now %d"), me->name, me->value, (int) current);

            if (ilabel == LABEL_IINST2 || ilabel == LABEL_IRMS2) {
                Energy.current[1]  = current;
            } else if (ilabel == LABEL_IINST3 || ilabel == LABEL_IRMS3) {
                Energy.phase_count = 3;
                Energy.current[2] = current;
            } else {
                Energy.current[0] = current;
            }
        }

        // Power P
        else if (ilabel == LABEL_PAPP || ilabel == LABEL_SINSTS)
        {
            Energy.active_power[0]  = (float) atoi(me->value);;
            AddLog(LOG_LEVEL_DEBUG, PSTR("TIC: Power %s, now %d"), me->value, (int)  Energy.active_power[0]);
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
                if (!strcmp_P(me->value, TELEINFO_STD_TARIFF_BASE)) {
                    tarif = TARIF_TH;
                } else if (!strcmp_P(me->value, TELEINFO_STD_TARIFF_HC)) {
                    tarif = TARIF_HC;
                } else if (!strcmp_P(me->value, TELEINFO_STD_TARIFF_HP)) {
                    tarif = TARIF_HP;
                }
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
                uint32_t total = 0;

                // Base, un seul index
                if (ilabel == LABEL_BASE) {
                    total = atoi(me->value);
                    AddLog(LOG_LEVEL_DEBUG, PSTR("TIC: Base:%u"), total);
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
                    AddLog(LOG_LEVEL_DEBUG, PSTR("TIC: HC:%u  HP:%u  Total:%u"), hc, hp, total);
                }

                EnergyUpdateTotal(total/1000.0f, true);
            }

            // Wh total index (standard)
            else if ( ilabel == LABEL_EAST)
            {
                uint32_t total = atoi(me->value);
                if (contrat != CONTRAT_BAS) {
                    EnergyUpdateTotal(total/1000.0f, true);
                    AddLog(LOG_LEVEL_DEBUG, PSTR("TIC: Total:%uWh"), total);
                }
            }

            // Wh indexes (standard)
            else if ( ilabel == LABEL_EASF01)
            {
                if (contrat == CONTRAT_BAS) {
                    EnergyUpdateTotal(atoi(me->value)/1000.0f, true);
                }
                AddLog(LOG_LEVEL_DEBUG, PSTR("TIC: HC:%u"),  atoi(me->value));
            }
            else if ( ilabel == LABEL_EASF02)
            {
                AddLog(LOG_LEVEL_DEBUG, PSTR("TIC: HP:%u"),  atoi(me->value));
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
                AddLog(LOG_LEVEL_DEBUG, PSTR("TIC: Contract changed, now '%s' (%d)"), me->value, contrat);
            }
            // Contract subscribed (standard is in clear text in value)
            else if (ilabel == LABEL_NGTF)
            {
                if (!strcmp_P(me->value, TELEINFO_STD_CONTRACT_BASE)) {
                    contrat = CONTRAT_BAS;
                } else if (!strcmp_P(me->value, TELEINFO_STD_CONTRACT_HCHP)) {
                    contrat = CONTRAT_HC;
                }

                AddLog(LOG_LEVEL_DEBUG, PSTR("TIC: Contract changed, now '%s'"), me->value);
            }

            // Contract subscribed (Power)
            else if (ilabel == LABEL_ISOUSC || ilabel == LABEL_PREF)
            {
                isousc = atoi( me->value);
                AddLog(LOG_LEVEL_DEBUG, PSTR("TIC: ISousc set to %d"), isousc);
            }

            // Serial Number of device
            else if (ilabel == LABEL_ADCO || ilabel == LABEL_ADSC)
            {
                strcpy(serialNumber, me->value);
                AddLog(LOG_LEVEL_DEBUG, PSTR("TIC: %s set to %s"), me->name, serialNumber);
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
Function: responseDumpTInfo
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
                        ResponseAppend_P( PSTR("\"%s\""), me->value );
                    } else {
                        ResponseAppend_P( PSTR("%d"), atoi(me->value));
                    }

                    // Now JSON separator is needed
                    sep =',';
                }
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
    Energy.data_valid[0] = 0;

    // Deprecated see setOption108
    // send teleinfo raw data only if setup like that
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
    } else {
        // Case we are on denky V4 board without any TInfo RX pin selected 
        #ifdef ARDUINO_DENKY_PICOV3
            tic_rx_pin = 8;
            AddLog(LOG_LEVEL_INFO, PSTR("TIC: Denky D4 board, RX on GPIO%d"), tic_rx_pin);
        #endif
    }

    // Enable teleinfo
    if (tic_rx_pin != NOT_A_PIN) {
        TasmotaGlobal.energy_driver = XNRG_15;
        Energy.voltage_available = false;
        Energy.phase_count = 1;
    } else {
        AddLog(LOG_LEVEL_ERROR, PSTR("TIC: Device has no RX pin"));
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
    } else  {
        AddLog(LOG_LEVEL_INFO, PSTR("TIC: always enabled"));
    }

#ifdef ESP8266
    // Allow GPIO3 AND GPIO13 with hardware fallback to 2
    // Set buffer to nnn char to support 250ms loop at 9600 baud
    TInfoSerial = new TasmotaSerial(tic_rx_pin, -1, 2, 0, serial_buffer_size);
    //pinMode(rx_pin, INPUT_PULLUP);
#endif  // ESP8266
#ifdef ESP32
    // Set buffer to nnn char to support 250ms loop at 9600 baud
    TInfoSerial = new TasmotaSerial(tic_rx_pin, -1, 1, 0, serial_buffer_size);
#endif  // ESP32

    // Trick here even using SERIAL_7E1 or TS_SERIAL_7E1
    // this is not working, need to call SetSerialConfig after
    if (TInfoSerial->begin(baudrate)) {


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
        SetSerialConfig(TS_SERIAL_7E1);
        AddLog(LOG_LEVEL_INFO, PSTR("TIC: using ESP32 hardware serial"));
#endif  // ESP32
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
        AddLog(LOG_LEVEL_INFO, PSTR("TIC: Ready"));
    }
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
    if (CMND_ENERGYCONFIG == Energy.command_code) {

        AddLog(LOG_LEVEL_DEBUG, PSTR("TIC: len %d, data '%s'"), XdrvMailbox.data_len, XdrvMailbox.data ? XdrvMailbox.data : "null" );

        // Just "EnergyConfig" no more parameter
        // Show Teleinfo configuration        
        if (XdrvMailbox.data_len == 0) {

            char mode_name[MAX_TINFO_COMMAND_NAME];
            char raw_name[MAX_TINFO_COMMAND_NAME];
            int index_mode = Settings->teleinfo.mode_standard ? CMND_TELEINFO_STANDARD : CMND_TELEINFO_HISTORIQUE;
            int index_raw = Settings->teleinfo.raw_send ? CMND_TELEINFO_RAW_FULL : CMND_TELEINFO_RAW_DISABLE;
            if (Settings->teleinfo.raw_send && Settings->teleinfo.raw_report_changed) {
                index_raw = CMND_TELEINFO_RAW_CHANGE;
            } 
            // Get the mode and raw name
            GetTextIndexed(mode_name, MAX_TINFO_COMMAND_NAME, index_mode, kTInfo_Commands);
            GetTextIndexed(raw_name, MAX_TINFO_COMMAND_NAME, index_raw, kTInfo_Commands);

            AddLog(LOG_LEVEL_INFO, TELEINFO_COMMAND_SETTINGS, mode_name, raw_name, Settings->teleinfo.raw_skip, Settings->teleinfo.raw_limit);

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

            AddLog(LOG_LEVEL_DEBUG, PSTR("TIC: param '%s' cmnd %d"), pParam, command_code);

            switch (command_code) {
                case CMND_TELEINFO_STANDARD:
                case CMND_TELEINFO_HISTORIQUE: {
                    char mode_name[MAX_TINFO_COMMAND_NAME];

                    // Get the mode name
                    GetTextIndexed(mode_name, MAX_TINFO_COMMAND_NAME, command_code, kTInfo_Commands);

                    // Only if current settings is different than previous
                    if ( (tinfo_mode==TINFO_MODE_STANDARD && command_code==CMND_TELEINFO_HISTORIQUE) ||
                         (tinfo_mode==TINFO_MODE_HISTORIQUE && command_code==CMND_TELEINFO_STANDARD) ) {

                        // Cleanup Serial not sure it will works since 
                        // there is no end() or close() on tasmotaserial class
                        if (TInfoSerial) {
                            TInfoSerial->flush();
                            //TInfoSerial->end();
                            free(TInfoSerial);
                        }

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
//const char HTTP_ENERGY_IINST_TELEINFO[] PROGMEM =  "{s}" D_CURRENT "%s{m}%d " D_UNIT_AMPERE "{e}" ;
const char HTTP_ENERGY_TARIF_TELEINFO[] PROGMEM =  "{s}" D_CURRENT_TARIFF "{m}Heures %s{e}" ;
const char HTTP_ENERGY_CONTRAT_TELEINFO[] PROGMEM =  "{s}" D_CONTRACT "{m}%s %d" D_UNIT_AMPERE "{e}" ;
const char HTTP_ENERGY_LOAD_TELEINFO[] PROGMEM =  "{s}" D_POWER_LOAD "{m}%d" D_UNIT_PERCENT "{e}" ;
const char HTTP_ENERGY_IMAX_TELEINFO[] PROGMEM =  "{s}" D_MAX_CURRENT "{m}%d" D_UNIT_AMPERE "{e}" ;
const char HTTP_ENERGY_IMAX3_TELEINFO[] PROGMEM =  "{s}" D_MAX_CURRENT "{m}%d / %d / %d " D_UNIT_AMPERE "{e}" ;
const char HTTP_ENERGY_PMAX_TELEINFO[] PROGMEM =  "{s}" D_MAX_POWER "{m}%d" D_UNIT_WATT "{e}" ;
const char HTTP_ENERGY_PMAX3_TELEINFO[] PROGMEM =  "{s}" D_MAX_POWER "{m}%d / %d / %d " D_UNIT_WATT "{e}" ;
const char HTTP_ENERGY_LOAD_BAR[] PROGMEM = "<tr><div style='margin:4px;padding:0px;background-color:#ddd;border-radius:4px;'>"
                                            "<div style='font-size:0.75rem;font-weight:bold;padding:0px;text-align:center;border:1px solid #bbb;border-radius:4px;color:#444;background-color:%s;width:%d%%;'>"
                                            "%d%%</div>"
                                            "</div></tr>";
#endif  // USE_WEBSERVER

void TInfoShow(bool json)
{
    // Since it's an Energy device , current, voltage and power are
    // already present on the telemetry frame. No need to add here
    // Just add the raw label/values of the teleinfo frame
    if (json)
    {
        // Add new value (not part of TIC JSON Object)
        if (isousc) {
            ResponseAppend_P(PSTR(",\"Load\":%d"),(int) ((Energy.current[0]*100.0f) / isousc));
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

        if (isousc) {
            uint8_t hue;
            uint8_t red, green, blue;
            char phase_color[8];

            for (int i=0; i<Energy.phase_count ; i++ ) {
                percent = (int) ((Energy.current[i]*100.0f) / isousc) ;
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
            if (getValueFromLabelIndex(LABEL_BASE, value) ) {
                GetTextIndexed(name, sizeof(name), LABEL_BASE, kLabel);
                WSContentSend_P(HTTP_ENERGY_INDEX_TELEINFO, name, value);
            }
            if (getValueFromLabelIndex(LABEL_HCHC, value) ) {
                GetTextIndexed(name, sizeof(name), LABEL_HCHC, kLabel);
                WSContentSend_P(HTTP_ENERGY_INDEX_TELEINFO, name, value);
            }
            if (getValueFromLabelIndex(LABEL_HCHP, value) ) {
                GetTextIndexed(name, sizeof(name), LABEL_HCHP, kLabel);
                WSContentSend_P(HTTP_ENERGY_INDEX_TELEINFO, name, value);
            }
            if (Energy.phase_count==3) {
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
                WSContentSend_P(HTTP_ENERGY_TARIF_TELEINFO, name);
            }
            if (contrat && isousc) {
                int percent = (int) ((Energy.current[0]*100.0f) / isousc) ;
                GetTextIndexed(name, sizeof(name), contrat, kContratName);
                WSContentSend_P(HTTP_ENERGY_CONTRAT_TELEINFO, name, isousc);
               //WSContentSend_P(HTTP_ENERGY_LOAD_TELEINFO,  percent);
            }

        } else if (tinfo_mode==TINFO_MODE_STANDARD ) {
            if (getValueFromLabelIndex(LABEL_EAST, value) ) {
                GetTextIndexed(name, sizeof(name), LABEL_EAST, kLabel);
                WSContentSend_P(HTTP_ENERGY_INDEX_TELEINFO, name, value);
            }
            if (getValueFromLabelIndex(LABEL_EASF01, value) ) {
                GetTextIndexed(name, sizeof(name), LABEL_EASF01, kLabel);
                WSContentSend_P(HTTP_ENERGY_INDEX_TELEINFO, name, value);
            }
            if (getValueFromLabelIndex(LABEL_EASF02, value) ) {
                GetTextIndexed(name, sizeof(name), LABEL_EASF02, kLabel);
                WSContentSend_P(HTTP_ENERGY_INDEX_TELEINFO, name, value);
            }
            if (getValueFromLabelIndex(LABEL_SMAXSN, value) ) {
                WSContentSend_P(HTTP_ENERGY_PMAX_TELEINFO, atoi(value));
            }
            if (getValueFromLabelIndex(LABEL_LTARF, value) ) {
                WSContentSend_P(HTTP_ENERGY_TARIF_TELEINFO, value);
            }
            if (getValueFromLabelIndex(LABEL_NGTF, value) ) {
                if (isousc) {
                    WSContentSend_P(HTTP_ENERGY_CONTRAT_TELEINFO, value, isousc);
                }
            }
        }

        // Serial number ADCO or ADSC if found
        if (*serialNumber) {
            WSContentSend_P(HTTP_ENERGY_ID_TELEINFO, serialNumber);
        }

#endif  // USE_WEBSERVER
    }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xnrg15(uint8_t function)
{
    bool result = false;
    switch (function)
    {
        case FUNC_EVERY_250_MSECOND:
            TInfoProcess();
            break;
        case FUNC_COMMAND:
            result = TInfoCmd();
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
    return result;
}

#endif  // USE_TELEINFO
#endif // USE_ENERGY_SENSOR