/*
  xsns_91_sml.ino - SML smart meter interface for Sonoff-Tasmota

  Created by Gerhard Mutz on 07.10.11.
  adapted for Tasmota

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

#ifdef USE_SML

// SML is default
//#define USE_OBIS


#define SML_BSIZ 28

static uint8_t smltbuf1[SML_BSIZ];
static int32_t tpower_in;
static int32_t tpower_out;
static int32_t tpower_curr;
static char obis;

// 2 Richtungszähler EHZ SML 8 bit 9600 baud, binär
// verbrauch total
const unsigned char seq1[8]={0x77,0x07,0x01,0x00,0x01,0x08,0x00,0xff};
// einspeisung total
const unsigned char seq2[8]={0x77,0x07,0x01,0x00,0x02,0x08,0x00,0xff};
// momentaner verbrauch bzw. einspeisung (negativ=einspeisung)
const unsigned char seq3[8]={0x77,0x07,0x01,0x00,0x10,0x07,0x00,0xff};
//hager obis 1-0:2.8.x*255( Zählwerksstand mit 4 nachkommastellen x an Zähler anpassen (hier 1))
// Hager EHZ 361  => 7 bit + parity , 9600 baud, ASCI
// negative Wirkenergie => Einspeisezähler
const unsigned char seqout[]={'1','-','0',':','2','.','8','.','1','*','2','5','5','('};
// positive Wirkenergie => Verbrauchszähler
const unsigned char seqin[]={'1','-','0',':','1','.','8','.','1','*','2','5','5','('};

// get sml binary value
int32_t sml_getvalue(unsigned char *cp) {
short len,unit,scaler,type;
int32_t value;

    // scan for value
    // check status
    len=*cp&0x0f;
    cp+=len;
    // check time
    len=*cp&0x0f;
    cp+=len;
    // check unit
    len=*cp&0x0f;
    unit=*(cp+1);
    cp+=len;
    // check scaler
    len=*cp&0x0f;
    scaler=(signed char)*(cp+1);
    cp+=len;
    // get value
    type=*cp&0x70;
    len=*cp&0x0f;
    type|=len-1;
    cp++;
    switch (type) {
        case 0x51:
            // int8
            value=(signed char)*cp;
            break;
        case 0x52:
            // int16;
            value=((short)*cp<<8)|*(cp+1);
            break;
        case 0x54:
            // int32;
            value=((int32_t)*cp<<24)|((int32_t)*(cp+1)<<16)|((int32_t)*(cp+2)<<8)|(*(cp+3));
            break;
        case 0x55:
            // int32+1;
            cp++;
            value=((int32_t)*cp<<24)|((int32_t)*(cp+1)<<16)|((int32_t)*(cp+2)<<8)|(*(cp+3));
            break;

        case 0x58:
            // int64;
            break;
        case 0x61:
            // uint8;
            value=(unsigned char)*cp;
            break;
        case 0x62:
            // uint16;
            value=((unsigned short)*cp<<8)|(*(cp+1));
            break;
        case 0x64:
            // uint32;
            value=((uint32_t)*cp<<24)|((uint32_t)*(cp+1)<<16)|((uint32_t)*(cp+2)<<8)|(*(cp+3));
            break;
        case 0x68:
            // uint64;
            break;

        default:
            value=999999;
            scaler=0;
            break;
    }

    if (scaler==-1) {
        value/=10;
    }
    return value;
}

// compare sml sequence
int32_t cmpseq(unsigned char *cp,const unsigned char *ccp,char *found,short size,char flag) {
    int16_t count;
    int32_t value;
    for (count=0; count<size; count++) {
      if (flag) {
        // 7 bit compares, remove parity bit
        unsigned char iob=*cp;
        iob=iob&0x7f;
        if (iob!=*ccp) {
            *found=0;
            return 0;
        }
      } else {
        // 8 bit compares
        if (*cp!=*ccp) {
            *found=0;
            return 0;
        }
      }
      cp++;
      ccp++;
    }
    *found=1;
    if (!flag) {
      value=sml_getvalue(cp);
      if (value==999999) *found=0;
      return value;
    } else {
      return 0;
    }
}

static double OBIS_T_out=0;
static double OBIS_T_outl=0;
static uint16_t OBIS_C_out=0;
static double OBIS_T_in=0;
static double OBIS_T_inl=0;
static uint16_t OBIS_C_in=0;

static uint8_t sec_cnt;
uint32_t last_millis;

//#define DUMP2Log

// poll sml source 1
void SML_Poll(void) {
    char found;
    short count;
    int32_t lval;
    sec_cnt++;
#ifdef DUMP2Log
    short index=0;
    while (Serial.available()) {
      log_data[index]=Serial.read()&0x7f;
      index++;
      if (index>=LOGSZ) {
        index=0;
      }
    }
    if (index>0) {
      log_data[index]=0;
      AddLog(LOG_LEVEL_INFO);
    }
#else
    while (Serial.available()) {
        // shift in
        for (count=0; count<sizeof(smltbuf1)-1; count++) {
            smltbuf1[count]=smltbuf1[count+1];
        }
        smltbuf1[sizeof(smltbuf1)-1]=(uint8_t)Serial.read();

        if (obis) {
          // check for obis asci
          // verbrauch
          lval=cmpseq(smltbuf1,seqin,&found,sizeof(seqin),1);
          if (found) {
            // asci
            unsigned char *cp=smltbuf1;
            smltbuf1[sizeof(smltbuf1)-1]=0;
            for (count=0; count<sizeof(smltbuf1)-1; count++) {
              smltbuf1[count]=smltbuf1[count]&0x7f;
            }
            // scan double value kwh
            cp+=sizeof(seqin);
            OBIS_T_in=CharToDouble((char *)cp);
            //SOLAR_C=sec_cnt;
            if (sec_cnt>=10) {
              sec_cnt=0;
              uint32_t cmillis=millis();
              // every 10 seconds
              // value difference
              double vdiff=OBIS_T_in-OBIS_T_inl;
              OBIS_C_in=(double)3600000*vdiff/((cmillis-last_millis)/1000);
              OBIS_T_inl=OBIS_T_in;
              last_millis=cmillis;
            }
            sml_set_timeout();
          }
          // einspeisung
          lval=cmpseq(smltbuf1,seqout,&found,sizeof(seqout),1);
          if (found) {
            // asci
            unsigned char *cp=smltbuf1;
            smltbuf1[sizeof(smltbuf1)-1]=0;
            for (count=0; count<sizeof(smltbuf1)-1; count++) {
              smltbuf1[count]=smltbuf1[count]&0x7f;
            }
            // scan double value kwh
            cp+=sizeof(seqout);
            OBIS_T_out=CharToDouble((char *)cp);
            //SOLAR_C=sec_cnt;
            if (sec_cnt>=10) {
              sec_cnt=0;
              uint32_t cmillis=millis();
              // every 10 seconds
              // value difference
              double vdiff=OBIS_T_out-OBIS_T_outl;
              OBIS_C_out=(double)3600000*vdiff/((cmillis-last_millis)/1000);
              OBIS_T_outl=OBIS_T_out;
              last_millis=cmillis;
            }
            sml_set_timeout();
          }

          tpower_curr=OBIS_C_in-OBIS_C_out;

        } else {
          // check for  EHZ sml binary
          lval=cmpseq(smltbuf1,seq1,&found,sizeof(seq1),0);
          if (found) {
            // in
            tpower_in=lval/1000;
            sml_set_timeout();
          }
          lval=cmpseq(smltbuf1,seq2,&found,sizeof(seq2),0);
          if (found) {
            // out
            tpower_out=lval/1000;
            sml_set_timeout();
          }
          lval=cmpseq(smltbuf1,seq3,&found,sizeof(seq3),0);
          if (found) {
            // current (signed value)
            tpower_curr=lval;
            sml_set_timeout();
          }
        }
    }
#endif
}


const char JSON_SML[] PROGMEM = "%s,\"%s\":{\"" "Total_in" "\":%d,\"" "Total_out" "\":%d,\"" "Power_curr" "\":%d }";
const char JSON_OBIS[] PROGMEM = "%s,\"%s\":{\"" "Total_in" "\":%s,\"" "Total_out" "\":%s,\"" "Power_curr" "\":%d }";


const char SML_BIN[] PROGMEM = "%s"
  "{s}SML " "Total-In: " "{m}%d " "KWh" "{e}"
  "{s}SML " "Total-Out: " "{m}%d " "KWh" "{e}"
  "{s}SML " "Current-In/Out: " "{m}%d " "W" "{e}";

  const char SML_ASCI[] PROGMEM = "%s"
  "{s}OBIS " "Total-In: " "{m}%s " "KWh" "{e}"
  "{s}OBIS " "Total-Out: " "{m}%s " "KWh" "{e}"
  "{s}OBIS " "Current-In/Out: " "{m}%d " "W" "{e}";

void SML_Show(boolean json) {
  if (json) {
    if (obis!=0xff) {
      if (obis) {
        char tpowstr1[32];
        char tpowstr2[32];
        dtostrfd(OBIS_T_in,4,tpowstr1);
        dtostrfd(OBIS_T_out,4,tpowstr2);
        snprintf_P(mqtt_data, sizeof(mqtt_data), JSON_OBIS, mqtt_data, "OBIS", tpowstr1,tpowstr2,tpower_curr);
      } else {
        snprintf_P(mqtt_data, sizeof(mqtt_data), JSON_SML, mqtt_data, "SML", tpower_in,tpower_out,tpower_curr);
      }
    }
  } else {
#ifdef USE_WEBSERVER
    // web interface
    if (obis==1) {
      char tpowstr1[32];
      char tpowstr2[32];
      dtostrfd(OBIS_T_in,4,tpowstr1);
      dtostrfd(OBIS_T_out,4,tpowstr2);
      snprintf_P(mqtt_data, sizeof(mqtt_data), SML_ASCI, mqtt_data,tpowstr1,tpowstr2,tpower_curr);
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), SML_BIN, mqtt_data,tpower_in, tpower_out, tpower_curr);
    }
#endif
  }
}

unsigned char sml_time_cnt;
// LEDPIN if you want to save power, do not use ledpin on devices that have a relais connected to the same pin
// or disconnect the relais in hardware
// sonoff basic has pin 13 to LED only (pin 12 is led +relais)
// type -1 instead to disable LED
#define LEDPIN -1
#define SML_TIMEOUT 10
static void sml_set_timeout() {
  sml_time_cnt=SML_TIMEOUT;
  // led on
#if LEDPIN>=0
  digitalWrite(LEDPIN,HIGH);
#endif
}


void SML_Init(void) {
#ifdef USE_OBIS
  obis=1;
#else
  obis=0; // set asci=1 or binary=0
#endif
  sec_cnt=0;
  SetSerialBaudrate(9600);
  /// led to OUTPUT
#if LEDPIN>=0
  pinMode(LEDPIN,OUTPUT);
  digitalWrite(LEDPIN,LOW);
#endif
  sml_time_cnt=SML_TIMEOUT;
  serial_local=1;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_91

boolean Xsns91(byte function) {
  boolean result = false;
  if (pin[GPIO_SML] < 99) {
    switch (function) {
      case FUNC_INIT:
        SML_Init();
        break;
      case FUNC_EVERY_SECOND:
        if (sml_time_cnt) {
          sml_time_cnt-=1;
          if (sml_time_cnt==0) {
            // timed out
            tpower_curr=0;
            // led off
#if LEDPIN>0
            digitalWrite(LEDPIN,LOW);
#endif
          }
        }
        SML_Poll();
        break;
      case FUNC_JSON_APPEND:
        SML_Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        SML_Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_SML
