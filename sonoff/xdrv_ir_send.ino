/*
  xdrv_ir_send.ino - infra red support for Sonoff-Tasmota

  Copyright (C) 2017  Heiko Krupp and Theo Arends

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

#ifdef USE_IR_REMOTE
/*********************************************************************************************\
 * IR Remote send using IRremoteESP8266 library
\*********************************************************************************************/

#ifndef USE_IR_HVAC
  #include <IRremoteESP8266.h>
#else
  #include <IRMitsubishiAC.h>

  // HVAC TOSHIBA_
  #define HVAC_TOSHIBA_HDR_MARK       4400
  #define HVAC_TOSHIBA_HDR_SPACE      4300
  #define HVAC_TOSHIBA_BIT_MARK       543
  #define HVAC_TOSHIBA_ONE_SPACE      1623
  #define HVAC_MISTUBISHI_ZERO_SPACE  472
  #define HVAC_TOSHIBA_RPT_MARK       440
  #define HVAC_TOSHIBA_RPT_SPACE      7048 // Above original iremote limit
  #define HVAC_TOSHIBA_DATALEN        9

  IRMitsubishiAC *mitsubir = NULL;

  const char FANSPEED[] = "A12345S";
  const char HVACMODE[] = "HDCA";
#endif

IRsend *irsend = NULL;

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
 * ArduinoJSON entry used to calculate jsonBuf: JSON_OBJECT_SIZE(3) + 40 = 96
 IRsend:
 { "protocol": "SAMSUNG", "bits": 32, "data": 551502015 }

 IRhvac:
 { "Vendor": "<Toshiba|Mitsubishi>", "Power": <0|1>, "Mode": "<Hot|Cold|Dry|Auto>", "FanSpeed": "<1|2|3|4|5|Auto|Silence>", "Temp": <17..30> }
*/

boolean ir_send_command(char *type, uint16_t index, char *dataBufUc, uint16_t data_len, int16_t payload, char *svalue, uint16_t ssvalue)
{
  boolean serviced = true;
  boolean error = false;
  const char *protocol;
  uint8_t  bits = 0;
  uint32_t data = 0;

  const char *HVAC_Mode;
  const char *HVAC_FanMode;
  const char *HVAC_Vendor;
  int HVAC_Temp = 21;
  boolean HVAC_Power = true;

//  char log[LOGSZ];

  if (!strcmp_P(type,PSTR("IRSEND"))) {
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
          if      (!strcmp_P(protocol,PSTR("NEC")))     irsend->sendNEC(data, bits);
          else if (!strcmp_P(protocol,PSTR("SONY")))    irsend->sendSony(data, bits);
          else if (!strcmp_P(protocol,PSTR("RC5")))     irsend->sendRC5(data, bits);
          else if (!strcmp_P(protocol,PSTR("RC6")))     irsend->sendRC6(data, bits);
          else if (!strcmp_P(protocol,PSTR("DISH")))    irsend->sendDISH(data, bits);
          else if (!strcmp_P(protocol,PSTR("JVC")))     irsend->sendJVC(data, bits, 1);
          else if (!strcmp_P(protocol,PSTR("SAMSUNG"))) irsend->sendSAMSUNG(data, bits);
          else {
            snprintf_P(svalue, ssvalue, PSTR("{\"IRSend\":\"Protocol not supported\"}"));
          }
        } else error = true;
      }
    } else error = true;
    if (error) {
      snprintf_P(svalue, ssvalue, PSTR("{\"IRSend\":\"No protocol, bits or data\"}"));
    }
  }
#ifdef USE_IR_HVAC
  else if (!strcmp_P(type,PSTR("IRHVAC"))) {
    if (data_len) {
      StaticJsonBuffer<164> jsonBufer;
      JsonObject &root = jsonBufer.parseObject(dataBufUc);
      if (!root.success()) {
        snprintf_P(svalue, ssvalue, PSTR("{\"IRHVAC\":\"Invalid JSON\"}"));  // JSON decode failed
      } else {
        snprintf_P(svalue, ssvalue, PSTR("{\"IRHVAC\":\"Done\"}"));
        HVAC_Vendor = root["VENDOR"];
        HVAC_Power = root["POWER"];
        HVAC_Mode = root["MODE"];
        HVAC_FanMode = root["FANSPEED"];
        HVAC_Temp = root["TEMP"];

//        snprintf_P(log, sizeof(log), PSTR("IRHVAC: Received Vendor %s, Power %d, Mode %s, FanSpeed %s, Temp %d"),
//          HVAC_Vendor, HVAC_Power, HVAC_Mode, HVAC_FanMode, HVAC_Temp);
//        addLog(LOG_LEVEL_DEBUG, log);
        
        if (HVAC_Vendor == NULL || !strcmp_P(HVAC_Vendor,PSTR("TOSHIBA"))) {
          error = ir_hvac_toshiba(HVAC_Mode, HVAC_FanMode, HVAC_Power, HVAC_Temp);
        }
        else if (!strcmp_P(HVAC_Vendor,PSTR("MITSUBISHI"))) {
          error = ir_hvac_mitsubishi(HVAC_Mode, HVAC_FanMode, HVAC_Power, HVAC_Temp);
        }
        else  error = true;
      }
    } else error = true;
    if (error) {
      snprintf_P(svalue, ssvalue, PSTR("{\"IRHVAC\":\"Wrong Vendor, Mode and/or FanSpeed\"}"));
    }
  }
#endif  // USE_IR_HVAC
  else {
    serviced = false;  // Unknown command
  }
  return serviced;
}

#ifdef USE_IR_HVAC
boolean ir_hvac_toshiba(const char *HVAC_Mode, const char *HVAC_FanMode, boolean HVAC_Power, int HVAC_Temp)
{
  unsigned int rawdata[2 + 2*8*HVAC_TOSHIBA_DATALEN + 2];
  byte data[HVAC_TOSHIBA_DATALEN] = { 0xF2, 0x0D, 0x03, 0xFC, 0x01, 0x00, 0x00, 0x00, 0x00 };

  char *p;
  char *token;
  uint8_t mode;

  if (HVAC_Mode == NULL) {
    p = (char*)HVACMODE;  // default HVAC_HOT
  } else {
    p = strchr(HVACMODE, HVAC_Mode[0]);
  }
  if (!p) {
    return true;
  }
  data[6] = (p - HVACMODE) ^ 0x03;  // HOT = 0x03, DRY = 0x02, COOL = 0x01, AUTO = 0x00

  if (!HVAC_Power) {
    data[6] = (byte) 0x07; // Turn OFF HVAC
  }

  if (HVAC_FanMode == NULL) {
    p = (char*)FANSPEED;  // default FAN_SPEED_AUTO
  } else {
    p = strchr(FANSPEED, HVAC_FanMode[0]);
  }
  if (!p) {
    return true;
  }
  mode = p - FANSPEED +1;    
  if ((1 == mode) || (7 == mode)) {
    mode = 0;
  }
  mode = mode << 5;       // AUTO = 0x00, SPEED = 0x40, 0x60, 0x80, 0xA0, 0xC0, SILENT = 0x00
  data[6] = data[6] | mode;

  byte Temp;
  if (HVAC_Temp > 30) {
    Temp = 30;
  }
  else if (HVAC_Temp < 17) {
    Temp = 17;
  }
  else {
    Temp = HVAC_Temp;
  }
  data[5] = (byte) Temp - 17 << 4;

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
  for (int b = 0; b < HVAC_TOSHIBA_DATALEN; b++) {
    for (mask = B10000000; mask > 0; mask >>= 1) { //iterate through bit mask
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

  return false;
}

boolean ir_hvac_mitsubishi(const char *HVAC_Mode,const char *HVAC_FanMode, boolean HVAC_Power, int HVAC_Temp)
{
  char *p;
  char *token;
  uint8_t mode;
  char log[LOGSZ];
    
  mitsubir->stateReset();

  if (HVAC_Mode == NULL) {
    p = (char*)HVACMODE;  // default HVAC_HOT
  } else {
    p = strchr(HVACMODE, HVAC_Mode[0]);
  }
  if (!p) {
    return true;
  }
  mode = (p - HVACMODE +1) << 3;  // HOT = 0x08, DRY = 0x10, COOL = 0x18, AUTO = 0x20
  mitsubir->setMode(mode);

  mitsubir->setPower(~HVAC_Power);

  if (HVAC_FanMode == NULL) {
    p = (char*)FANSPEED;  // default FAN_SPEED_AUTO
  } else {
    p = strchr(FANSPEED, HVAC_FanMode[0]);
  }
  if (!p) {
    return true;
  }
  mode = p - FANSPEED;    // AUTO = 0, SPEED = 1 .. 5, SILENT = 6
  mitsubir->setFan(mode);

  mitsubir->setTemp(HVAC_Temp);
  mitsubir->setVane(MITSUBISHI_AC_VANE_AUTO);
  mitsubir->send();
  
//  snprintf_P(log, sizeof(log), PSTR("IRHVAC: Mitsubishi Power %d, Mode %d, FanSpeed %d, Temp %d, VaneMode %d"),
//    mitsubir->getPower(), mitsubir->getMode(), mitsubir->getFan(), mitsubir->getTemp(), mitsubir->getVane());
//  addLog(LOG_LEVEL_DEBUG, log);
  
  return false;
}
#endif  // USE_IR_HVAC
#endif  // USE_IR_REMOTE
