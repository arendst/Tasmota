/*
Copyright (c) 2017 Heiko Krupp and Theo Arends.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef USE_IR_REMOTE
/*********************************************************************************************\
 * IR Remote send using IRremoteESP8266 library
\*********************************************************************************************/

#include <IRremoteESP8266.h>
#include <IRMitsubishiAC.h>

IRsend *irsend = NULL;
IRMitsubishiAC *mitsubir = NULL;

void ir_send_init(void)
{
  irsend = new IRsend(pin[GPIO_IRSEND]);  // an IR led is at GPIO_IRSEND
  irsend->begin();

#ifdef USE_IR_HVAC
  mitsubir = new IRMitsubishiAC(pin[GPIO_IRSEND]);
#endif //USE_IR_HVAC
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

/*
 * ArduinoJSON IRsend entry used to calculate jsonBuf: JSON_OBJECT_SIZE(3) + 40 = 96
 { "protocol": "SAMSUNG", "bits": 32, "data": 551502015 }
*/

boolean ir_send_command(char *type, uint16_t index, char *dataBufUc, uint16_t data_len, int16_t payload, char *svalue, uint16_t ssvalue)
{
  boolean serviced = true;
  boolean error = false;
  const char *protocol;
  uint8_t  bits = 0;
  uint32_t data = 0;
  
  if (!strcmp(type,"IRSEND")) {
	  if (data_len) {
      StaticJsonBuffer<128> jsonBuf;
      JsonObject &ir_json = jsonBuf.parseObject(dataBufUc);
      if (!ir_json.success()) {
        snprintf_P(svalue, ssvalue, PSTR("{\"IRSend\":\"Invalid JSON\"}"));  // JSON decode failed 
      } else {
        snprintf_P(svalue, ssvalue, PSTR("{\"IRSend\":\"Done\"}"));
        protocol = ir_json["PROTOCOL"];
        bits = ir_json["BITS"];
        data = ir_json["DATA"];
        if (protocol && bits && data) {
          if      (!strcmp(protocol,"NEC"))     irsend->sendNEC(data, bits);
          else if (!strcmp(protocol,"SONY"))    irsend->sendSony(data, bits);
          else if (!strcmp(protocol,"RC5"))     irsend->sendRC5(data, bits);
          else if (!strcmp(protocol,"RC6"))     irsend->sendRC6(data, bits);
          else if (!strcmp(protocol,"DISH"))    irsend->sendDISH(data, bits);
          else if (!strcmp(protocol,"JVC"))     irsend->sendJVC(data, bits, 1);
          else if (!strcmp(protocol,"SAMSUNG")) irsend->sendSAMSUNG(data, bits);
          else {
            snprintf_P(svalue, ssvalue, PSTR("{\"IRSend\":\"Protocol not supported\"}"));
          }
        } else error = true;
      }
    } else error = true;
    if (error) snprintf_P(svalue, ssvalue, PSTR("{\"IRSend\":\"No protocol, bits or data\"}"));
  }
  else {
    serviced = false;  // Unknown command
  }
  return serviced;
}

#ifdef USE_IR_HVAC
/* TOSHIBA HVAC support based on https://github.com/r45635/HVAC-IR-Control */



// HVAC TOSHIBA_
#define HVAC_TOSHIBA_HDR_MARK    4400
#define HVAC_TOSHIBA_HDR_SPACE   4300
#define HVAC_TOSHIBA_BIT_MARK    543
#define HVAC_TOSHIBA_ONE_SPACE   1623
#define HVAC_MISTUBISHI_ZERO_SPACE  472
#define HVAC_TOSHIBA_RPT_MARK    440
#define HVAC_TOSHIBA_RPT_SPACE   7048 // Above original iremote limit
#define HVAC_TOSHIBA_DATALEN 9

boolean ir_hvac_command(char *type, uint16_t index, char *dataBufUc, uint16_t data_len, int16_t payload, char *svalue, uint16_t ssvalue)
{
  boolean serviced = true;
  boolean error = false;
  const char *protocol;
  const char *HVAC_Mode_Str;
  const char *HVAC_FanMode_Str;
  const char *HVAC_Vendor;
  int HVAC_Temp = 21;
  int OFF = true;                  // Example false


  if (!strcmp(type,"IRHVAC")) {
    if (data_len) {
      StaticJsonBuffer<128> jsonBufer;
      JsonObject &root = jsonBufer.parseObject(dataBufUc);
      if (!root.success()) {
        snprintf_P(svalue, ssvalue, PSTR("{\"IRHVAC\":\"Invalid JSON\"}"));  // JSON decode failed
      } else {
        snprintf_P(svalue, ssvalue, PSTR("{\"IRHVAC\":\"Done\"}"));
        HVAC_Mode_Str = root["HVAC_MODE"];
        HVAC_Temp = root["HVAC_TEMP"];
        HVAC_FanMode_Str = root["HVAC_FANMODE"];
        OFF = root["OFF"];
        HVAC_Vendor = root["HVAC_VENDOR"];

        if (HVAC_Vendor == NULL || !strcmp(HVAC_Vendor,"TOSHIBA")) {
          error = ir_hvac_toshiba(HVAC_Mode_Str,HVAC_FanMode_Str,OFF,HVAC_Temp);
        } else if (!strcmp(HVAC_Vendor,"MITSUBISHI")) {
          error = ir_hvac_mitsubishi(HVAC_Mode_Str,HVAC_FanMode_Str,OFF,HVAC_Temp);
        } else {
          error = true;
        }
        
      }
    } else error = true;
    if (error) snprintf_P(svalue, ssvalue, PSTR("{\"IRHVAC\":\"Wrong parameters value for HVAC_Mode and/or HVAC_FanMode and/or HVAC_Vendor\"}"));
  }
  else {
    serviced = false;  // Unknown command
  }
  return serviced;
}


boolean ir_hvac_toshiba(const char *HVAC_Mode_Str,const char *HVAC_FanMode_Str, int OFF, int HVAC_Temp)
{
  unsigned int rawdata[2 + 2*8*HVAC_TOSHIBA_DATALEN + 2];
  byte data[HVAC_TOSHIBA_DATALEN] = { 0xF2, 0x0D, 0x03, 0xFC, 0x01, 0x00, 0x00, 0x00, 0x00 };
  boolean error = false;

          if (HVAC_Mode_Str == NULL || !strcmp(HVAC_Mode_Str,"HVAC_HOT")) //default HVAC_HOT
          {
            data[6] = (byte) B00000011;
          } else if (HVAC_Mode_Str && !strcmp(HVAC_Mode_Str,"HVAC_COLD"))
          {
            data[6] = (byte) B00000001;
          } else if (HVAC_Mode_Str && !strcmp(HVAC_Mode_Str,"HVAC_DRY"))
          {
            data[6] = (byte) B00000010;
          } else if (HVAC_Mode_Str && !strcmp(HVAC_Mode_Str,"HVAC_AUTO"))
          {
            data[6] = (byte) B00000000;
          } else
          {
            error = true;
          }

          if (OFF) {
            data[6] = (byte) 0x07; // Turn OFF HVAC
          }

          if (HVAC_FanMode_Str && !strcmp(HVAC_FanMode_Str,"FAN_SPEED_1"))
          {
            data[6] = data[6] | (byte) B01000000;
          } else if (HVAC_FanMode_Str && !strcmp(HVAC_FanMode_Str,"FAN_SPEED_2"))
          {
            data[6] = data[6] | (byte) B01100000;
          } else if (HVAC_FanMode_Str && !strcmp(HVAC_FanMode_Str,"FAN_SPEED_3"))
          {
            data[6] = data[6] | (byte) B10000000;
          } else if (HVAC_FanMode_Str && !strcmp(HVAC_FanMode_Str,"FAN_SPEED_4"))
          {
            data[6] = data[6] | (byte) B10100000;
          } else if (HVAC_FanMode_Str && !strcmp(HVAC_FanMode_Str,"FAN_SPEED_5"))
          {
            data[6] = data[6] | (byte) B11000000;
          } else if (HVAC_FanMode_Str == NULL || !strcmp(HVAC_FanMode_Str,"FAN_SPEED_AUTO")) //default FAN_SPEED_AUTO
          {
            data[6] = data[6] | (byte) B00000000;
          } else if (HVAC_FanMode_Str && !strcmp(HVAC_FanMode_Str,"FAN_SPEED_SILENT"))
          {
            data[6] = data[6] | (byte) B00000000;
          }
          else
          {
            error = true;
          }


             byte Temp;
             if (HVAC_Temp > 30) { Temp = 30;}
             else if (HVAC_Temp < 17) { Temp = 17; }
             else { Temp = HVAC_Temp; };
             data[5] = (byte) Temp - 17<<4;

             data[HVAC_TOSHIBA_DATALEN-1] = 0;
             for (int x = 0; x < HVAC_TOSHIBA_DATALEN - 1; x++) {
              data[HVAC_TOSHIBA_DATALEN-1] = (byte) data[x] ^ data[HVAC_TOSHIBA_DATALEN -1];  // CRC is a simple bits addition
             }

             int i = 0;
             byte mask = 1;

             //header
             rawdata[i++] = HVAC_TOSHIBA_HDR_MARK;
             rawdata[i++] = HVAC_TOSHIBA_HDR_SPACE;

             //data
             for (int b = 0; b < HVAC_TOSHIBA_DATALEN; b++)
             {
               for (mask = 10000000; mask > 0; mask >>= 1) { //iterate through bit mask
                 if (data[b] & mask) { // Bit ONE
                  rawdata[i++] = HVAC_TOSHIBA_BIT_MARK;
                  rawdata[i++] = HVAC_TOSHIBA_ONE_SPACE;
                 }
                 else { // Bit ZERO
                  rawdata[i++] = HVAC_TOSHIBA_BIT_MARK;
                  rawdata[i++] = HVAC_MISTUBISHI_ZERO_SPACE;
                 }
               }
             }

             //trailer
             rawdata[i++] = HVAC_TOSHIBA_RPT_MARK;
             rawdata[i++] = HVAC_TOSHIBA_RPT_SPACE;

             noInterrupts();
             irsend->sendRaw(rawdata,i,38);
             irsend->sendRaw(rawdata,i,38);
             interrupts();

             return error;
}

boolean ir_hvac_mitsubishi(const char *HVAC_Mode_Str,const char *HVAC_FanMode_Str, int OFF, int HVAC_Temp)
{
    boolean error = false;
    
          mitsubir->stateReset();
          
          if (HVAC_Mode_Str == NULL || !strcmp(HVAC_Mode_Str,"HVAC_HOT")) //default HVAC_HOT
          {
            mitsubir->setMode(MITSUBISHI_AC_HEAT);
          } else if (HVAC_Mode_Str && !strcmp(HVAC_Mode_Str,"HVAC_COLD"))
          {
            mitsubir->setMode(MITSUBISHI_AC_COOL);
          } else if (HVAC_Mode_Str && !strcmp(HVAC_Mode_Str,"HVAC_DRY"))
          {
            mitsubir->setMode(MITSUBISHI_AC_DRY);
          } else if (HVAC_Mode_Str && !strcmp(HVAC_Mode_Str,"HVAC_AUTO"))
          {
            mitsubir->setMode(MITSUBISHI_AC_AUTO);
          } else
          {
            error = true;
          }

          if (OFF) {
            mitsubir->off();
          } else {
            mitsubir->on();
          }

          if (HVAC_FanMode_Str && !strcmp(HVAC_FanMode_Str,"FAN_SPEED_1"))
          {
            mitsubir->setFan(1);
          } else if (HVAC_FanMode_Str && !strcmp(HVAC_FanMode_Str,"FAN_SPEED_2"))
          {
            mitsubir->setFan(2);
          } else if (HVAC_FanMode_Str && !strcmp(HVAC_FanMode_Str,"FAN_SPEED_3"))
          {
            mitsubir->setFan(3);
          } else if (HVAC_FanMode_Str && !strcmp(HVAC_FanMode_Str,"FAN_SPEED_4"))
          {
            mitsubir->setFan(4);
          } else if (HVAC_FanMode_Str && !strcmp(HVAC_FanMode_Str,"FAN_SPEED_5"))
          {
           mitsubir->setFan(5);
          } else if (HVAC_FanMode_Str == NULL || !strcmp(HVAC_FanMode_Str,"FAN_SPEED_AUTO")) //default FAN_SPEED_AUTO
          {
            mitsubir->setFan(MITSUBISHI_AC_FAN_AUTO);
          } else if (HVAC_FanMode_Str && !strcmp(HVAC_FanMode_Str,"FAN_SPEED_SILENT"))
          {
            mitsubir->setFan(MITSUBISHI_AC_FAN_SILENT);
          }
          else
          {
            error = true;
          }

          mitsubir->setTemp(HVAC_Temp);
          mitsubir->setVane(MITSUBISHI_AC_VANE_AUTO);

          mitsubir->send();

}

#endif  //USE_IR_HVAC
#endif  // USE_IR_REMOTE
