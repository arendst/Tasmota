/*
  xdrv_05_irremote.ino - infra red support for Sonoff-Tasmota

  Copyright (C) 2019  Heiko Krupp, Lazar Obradovic and Theo Arends

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
 * IR Remote send and receive using IRremoteESP8266 library
\*********************************************************************************************/

#define XDRV_05             5

#include <IRremoteESP8266.h>

enum IrErrors { IE_NO_ERROR, IE_INVALID_RAWDATA, IE_INVALID_JSON, IE_SYNTAX_IRSEND, IE_SYNTAX_IRHVAC };

enum IrRemoteCommands { CMND_IRSEND, CMND_IRHVAC };
const char kIrRemoteCommands[] PROGMEM = D_CMND_IRSEND "|" D_CMND_IRHVAC ;

// Based on IRremoteESP8266.h enum decode_type_t
const char kIrRemoteProtocols[] PROGMEM =
  "UNKNOWN|RC5|RC6|NEC|SONY|PANASONIC|JVC|SAMSUNG|WHYNTER|AIWA_RC_T501|LG|SANYO|MITSUBISHI|DISH|SHARP";

#ifdef USE_IR_HVAC

#include <ir_Mitsubishi.h>
#include <ir_Fujitsu.h>

enum IrHvacVendors { VNDR_TOSHIBA, VNDR_MITSUBISHI, VNDR_LG, VNDR_FUJITSU };
const char kIrHvacVendors[] PROGMEM = "Toshiba|Mitsubishi|LG|Fujitsu" ;

// HVAC TOSHIBA_
const uint16_t HVAC_TOSHIBA_HDR_MARK = 4400;
const uint16_t HVAC_TOSHIBA_HDR_SPACE = 4300;
const uint16_t HVAC_TOSHIBA_BIT_MARK = 543;
const uint16_t HVAC_TOSHIBA_ONE_SPACE = 1623;
const uint16_t HVAC_MISTUBISHI_ZERO_SPACE = 472;
const uint16_t HVAC_TOSHIBA_RPT_MARK = 440;
const uint16_t HVAC_TOSHIBA_RPT_SPACE = 7048; // Above original iremote limit
const uint8_t HVAC_TOSHIBA_DATALEN = 9;

// HVAC LG
const uint8_t HVAC_LG_DATALEN = 7;

IRMitsubishiAC *mitsubir = nullptr;

const char kFanSpeedOptions[] = "A12345S";
const char kHvacModeOptions[] = "HDCA";
#endif  // USE_IR_HVAC

/*********************************************************************************************\
 * IR Send
\*********************************************************************************************/

#include <IRsend.h>

IRsend *irsend = nullptr;
bool irsend_active = false;

void IrSendInit(void)
{
  irsend = new IRsend(pin[GPIO_IRSEND]); // an IR led is at GPIO_IRSEND
  irsend->begin();

#ifdef USE_IR_HVAC
  mitsubir = new IRMitsubishiAC(pin[GPIO_IRSEND]);
#endif //USE_IR_HVAC
}

char* IrUint64toHex(uint64_t value, char *str, uint16_t bits)
{
  ulltoa(value, str, 16);  // Get 64bit value

  int fill = 8;
  if ((bits > 3) && (bits < 65)) {
    fill = bits / 4;  // Max 16
    if (bits % 4) { fill++; }
  }
  int len = strlen(str);
  fill -= len;
  if (fill > 0) {
    memmove(str + fill, str, len +1);
    memset(str, '0', fill);
  }
  memmove(str + 2, str, strlen(str) +1);
  str[0] = '0';
  str[1] = 'x';
  return str;
}

#ifdef USE_IR_RECEIVE
/*********************************************************************************************\
 * IR Receive
\*********************************************************************************************/

const bool IR_RCV_SAVE_BUFFER = false;         // false = do not use buffer, true = use buffer for decoding
const uint32_t IR_TIME_AVOID_DUPLICATE = 500;  // Milliseconds

#include <IRrecv.h>

IRrecv *irrecv = nullptr;

unsigned long ir_lasttime = 0;

void IrReceiveUpdateThreshold()
{
  if (Settings.param[P_IR_UNKNOW_THRESHOLD] < 6) { Settings.param[P_IR_UNKNOW_THRESHOLD] = 6; }
  irrecv->setUnknownThreshold(Settings.param[P_IR_UNKNOW_THRESHOLD]);
}

void IrReceiveInit(void)
{
  // an IR led is at GPIO_IRRECV
  irrecv = new IRrecv(pin[GPIO_IRRECV], IR_RCV_BUFFER_SIZE, IR_RCV_TIMEOUT, IR_RCV_SAVE_BUFFER);
  irrecv->setUnknownThreshold(Settings.param[P_IR_UNKNOW_THRESHOLD]);
  irrecv->enableIRIn();                  // Start the receiver

  //  AddLog_P(LOG_LEVEL_DEBUG, PSTR("IrReceive initialized"));
}

void IrReceiveCheck(void)
{
  char sirtype[14];  // Max is AIWA_RC_T501
  int8_t iridx = 0;

  decode_results results;

  if (irrecv->decode(&results)) {
    char hvalue[64];
    IrUint64toHex(results.value, hvalue, results.bits);  // Get 64bit value as hex 0x00123456

    AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_IRR "Echo %d, RawLen %d, Overflow %d, Bits %d, Value %s, Decode %d"),
              irsend_active, results.rawlen, results.overflow, results.bits, hvalue, results.decode_type);

    unsigned long now = millis();
//    if ((now - ir_lasttime > IR_TIME_AVOID_DUPLICATE) && (UNKNOWN != results.decode_type) && (results.bits > 0)) {
    if (!irsend_active && (now - ir_lasttime > IR_TIME_AVOID_DUPLICATE)) {
      ir_lasttime = now;

      iridx = results.decode_type;
      if ((iridx < 0) || (iridx > 14)) { iridx = 0; }  // UNKNOWN
      char svalue[64];
      if (Settings.flag.ir_receive_decimal) {
        ulltoa(results.value, svalue, 10);
      } else {
        snprintf_P(svalue, sizeof(svalue), PSTR("\"%s\""), hvalue);
      }
      Response_P(PSTR("{\"" D_JSON_IRRECEIVED "\":{\"" D_JSON_IR_PROTOCOL "\":\"%s\",\"" D_JSON_IR_BITS "\":%d,\"" D_JSON_IR_DATA "\":%s"),
        GetTextIndexed(sirtype, sizeof(sirtype), iridx, kIrRemoteProtocols), results.bits, svalue);

      if (Settings.flag3.receive_raw) {
        ResponseAppend_P(PSTR(",\"" D_JSON_IR_RAWDATA "\":["));
        uint16_t i;
        for (i = 1; i < results.rawlen; i++) {
          if (i > 1) { ResponseAppend_P(PSTR(",")); }
          uint32_t usecs;
          for (usecs = results.rawbuf[i] * kRawTick; usecs > UINT16_MAX; usecs -= UINT16_MAX) {
            ResponseAppend_P(PSTR("%d,0,"), UINT16_MAX);
          }
          ResponseAppend_P(PSTR("%d"), usecs);
          if (strlen(mqtt_data) > sizeof(mqtt_data) - 40) { break; }  // Quit if char string becomes too long
        }
        uint16_t extended_length = results.rawlen - 1;
        for (uint32_t j = 0; j < results.rawlen - 1; j++) {
          uint32_t usecs = results.rawbuf[j] * kRawTick;
          // Add two extra entries for multiple larger than UINT16_MAX it is.
          extended_length += (usecs / (UINT16_MAX + 1)) * 2;
        }
        ResponseAppend_P(PSTR("],\"" D_JSON_IR_RAWDATA "Info\":[%d,%d,%d]"), extended_length, i -1, results.overflow);
      }

      ResponseAppend_P(PSTR("}}"));
      MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_IRRECEIVED));

      if (iridx) {
        XdrvRulesProcess();
#ifdef USE_DOMOTICZ
        unsigned long value = results.value | (iridx << 28);  // [Protocol:4, Data:28]
        DomoticzSensor(DZ_COUNT, value);                      // Send data as Domoticz Counter value
#endif  // USE_DOMOTICZ
      }
    }

    irrecv->resume();
  }
}
#endif // USE_IR_RECEIVE

#ifdef USE_IR_HVAC
/********************************************************************************************* \
 * IR Heating, Ventilation and Air Conditioning using IRMitsubishiAC library
\*********************************************************************************************/

/*******************
      TOSHIBA
********************/

uint8_t IrHvacToshiba(const char *HVAC_Mode, const char *HVAC_FanMode, bool HVAC_Power, int HVAC_Temp)
{
  uint16_t rawdata[2 + 2 * 8 * HVAC_TOSHIBA_DATALEN + 2];
  uint8_t data[HVAC_TOSHIBA_DATALEN] = {0xF2, 0x0D, 0x03, 0xFC, 0x01, 0x00, 0x00, 0x00, 0x00};

  char *p;
  uint8_t mode;

  if (HVAC_Mode == nullptr) {
    p = (char *)kHvacModeOptions; // default HVAC_HOT
  }
  else {
    p = strchr(kHvacModeOptions, toupper(HVAC_Mode[0]));
  }
  if (!p) {
    return IE_SYNTAX_IRHVAC;
  }
  data[6] = (p - kHvacModeOptions) ^ 0x03; // HOT = 0x03, DRY = 0x02, COOL = 0x01, AUTO = 0x00

  if (!HVAC_Power) {
    data[6] = (uint8_t)0x07; // Turn OFF HVAC
  }

  if (HVAC_FanMode == nullptr) {
    p = (char *)kFanSpeedOptions; // default FAN_SPEED_AUTO
  }
  else {
    p = strchr(kFanSpeedOptions, toupper(HVAC_FanMode[0]));
  }
  if (!p) {
    return IE_SYNTAX_IRHVAC;
  }
  mode = p - kFanSpeedOptions + 1;
  if ((1 == mode) || (7 == mode)) {
    mode = 0;
  }
  mode = mode << 5; // AUTO = 0x00, SPEED = 0x40, 0x60, 0x80, 0xA0, 0xC0, SILENT = 0x00
  data[6] = data[6] | mode;

  uint8_t Temp;
  if (HVAC_Temp > 30) {
    Temp = 30;
  }
  else if (HVAC_Temp < 17) {
    Temp = 17;
  }
  else {
    Temp = HVAC_Temp;
  }
  data[5] = (uint8_t)(Temp - 17) << 4;

  data[HVAC_TOSHIBA_DATALEN - 1] = 0;
  for (uint32_t x = 0; x < HVAC_TOSHIBA_DATALEN - 1; x++) {
    data[HVAC_TOSHIBA_DATALEN - 1] = (uint8_t)data[x] ^ data[HVAC_TOSHIBA_DATALEN - 1]; // CRC is a simple bits addition
  }

  int i = 0;
  uint8_t mask = 1;

  //header
  rawdata[i++] = HVAC_TOSHIBA_HDR_MARK;
  rawdata[i++] = HVAC_TOSHIBA_HDR_SPACE;

  //data
  for (uint32_t b = 0; b < HVAC_TOSHIBA_DATALEN; b++) {
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

//  noInterrupts();
  irsend_active = true;
  irsend->sendRaw(rawdata, i, 38);
  irsend->sendRaw(rawdata, i, 38);
//  interrupts();

  return IE_NO_ERROR;
}


/*******************
     MITSUBISHI
********************/

uint8_t IrHvacMitsubishi(const char *HVAC_Mode, const char *HVAC_FanMode, bool HVAC_Power, int HVAC_Temp)
{
  char *p;
  uint8_t mode;

  mitsubir->stateReset();

  if (HVAC_Mode == nullptr) {
    p = (char *)kHvacModeOptions; // default HVAC_HOT
  }
  else {
    p = strchr(kHvacModeOptions, toupper(HVAC_Mode[0]));
  }
  if (!p) {
    return IE_SYNTAX_IRHVAC;
  }
  mode = (p - kHvacModeOptions + 1) << 3; // HOT = 0x08, DRY = 0x10, COOL = 0x18, AUTO = 0x20
  mitsubir->setMode(mode);

  mitsubir->setPower(HVAC_Power);

  if (HVAC_FanMode == nullptr) {
    p = (char *)kFanSpeedOptions; // default FAN_SPEED_AUTO
  }
  else {
    p = strchr(kFanSpeedOptions, toupper(HVAC_FanMode[0]));
  }
  if (!p) {
    return IE_SYNTAX_IRHVAC;
  }
  mode = p - kFanSpeedOptions; // AUTO = 0, SPEED = 1 .. 5, SILENT = 6
  mitsubir->setFan(mode);

  mitsubir->setTemp(HVAC_Temp);
  mitsubir->setVane(MITSUBISHI_AC_VANE_AUTO);
  mitsubir->send();

//  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("IRHVAC: Mitsubishi Power %d, Mode %d, FanSpeed %d, Temp %d, VaneMode %d"),
//    mitsubir->getPower(), mitsubir->getMode(), mitsubir->getFan(), mitsubir->getTemp(), mitsubir->getVane());

  return IE_NO_ERROR;
}


/*******************
        LG
********************/

uint8_t IrHvacLG(const char *HVAC_Mode, const char *HVAC_FanMode, bool HVAC_Power, int HVAC_Temp)
{
  uint32_t LG_Code;
  uint8_t data[HVAC_LG_DATALEN];
  static bool hvacOn = false;
  char *p;
  uint8_t mode;
  uint8_t Temp;

  // Constant data
  data[0] = 0x08;
  data[1] = 0x08;
  data[2] = 0x00;

  if (!HVAC_Power) {
    data[2] = (uint8_t)0x0C; // Turn OFF HVAC, code 0x88C0051
    data[3] = (uint8_t)0x00;
    data[4] = (uint8_t)0x00;
    data[5] = (uint8_t)0x05;
    data[6] = (uint8_t)0x01;
    hvacOn = false;
  }

  else {

    // Set code for HVAC Mode - data[3]
    if (HVAC_Mode == nullptr) {
      p = (char *)kHvacModeOptions; // default HVAC_HOT
    }
    else {
      p = strchr(kHvacModeOptions, toupper(HVAC_Mode[0]));
    }
    if (!p) {
      return IE_SYNTAX_IRHVAC;
    }
    mode = (p - kHvacModeOptions) ^ 0x03; // HOT = 0x03, DRY = 0x02, COOL = 0x01, AUTO = 0x00
    switch (mode) {
      case 0: // AUTO
        data[3] = 11;
        break;
      case 1: // COOL
        data[3] = 8;
        break;
      case 2: // DRY
        data[3] = 9;
        break;
      case 3: // HOT
        data[3] = 12;
        break;
    }
    if (!hvacOn) {
      data[3] = data[3] & 7; // reset bit3
      hvacOn = true;
    }

//    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("IRHVAC: HvacMode %s, ModeVal %d, Code %d"), p, mode, data[3]);

    // Set code for HVAC temperature - data[4]
    if (HVAC_Temp > 30) {
      Temp = 30;
    }
    else if (HVAC_Temp < 18) {
      Temp = 18;
    }
    else {
      Temp = HVAC_Temp;
    }
    data[4] = (uint8_t)(Temp - 15);

    // Set code for HVAC fan mode - data[5]
    if (HVAC_FanMode == nullptr) {
      p = (char *)kFanSpeedOptions; // default FAN_SPEED_AUTO
    }
    else {
      p = strchr(kFanSpeedOptions, toupper(HVAC_FanMode[0]));
    }
    if (!p) {
      return IE_SYNTAX_IRHVAC;
    }
    mode = p - kFanSpeedOptions;
    if ((mode == 0) || (mode > 3)) {
      data[5] = 5; // Auto = 0x05
    }
    else {
      data[5] = (mode * 2) - 2; // Low = 0x00, Mid = 0x02, High = 0x04
    }

//    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("IRHVAC: FanMode %s, ModeVal %d, Code %d"), p, mode, data[5]);

    // Set CRC code - data[6]
    data[6] = (data[3] + data[4] + data[5]) & 0x0f; // CRC

  }
  // Build LG IR code
  LG_Code = data[0] << 4;
  for (uint32_t i = 1; i < 6; i++) {
    LG_Code = (LG_Code + data[i]) << 4;
  }
  LG_Code = LG_Code + data[6];

//  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("IRHVAC: LG_Code %d"), LG_Code);

  // Send LG IR Code
//  noInterrupts();
  irsend_active = true;
  irsend->sendLG(LG_Code, 28);
//  interrupts();

  return IE_NO_ERROR;
}


/*******************
      Fujitsu
********************/

uint8_t IrHvacFujitsu(const char *HVAC_Mode, const char *HVAC_FanMode, bool HVAC_Power, int HVAC_Temp)
{
  const char kFujitsuHvacModeOptions[] = "HDCAF";

//  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("FUJITSU: mode:%s, fan:%s, power:%u, temp:%u"), HVAC_Mode, HVAC_FanMode, HVAC_Power, HVAC_Temp);

  IRFujitsuAC ac(pin[GPIO_IRSEND]);

  irsend_active = true;

  if (0 == HVAC_Power) {
    ac.off();
    ac.send();
    return IE_NO_ERROR;
  }

  uint8_t modes[5] = {FUJITSU_AC_MODE_HEAT, FUJITSU_AC_MODE_DRY, FUJITSU_AC_MODE_COOL, FUJITSU_AC_MODE_AUTO, FUJITSU_AC_MODE_FAN};
  uint8_t fanModes[7] = {FUJITSU_AC_FAN_AUTO, FUJITSU_AC_FAN_LOW, FUJITSU_AC_FAN_MED, FUJITSU_AC_FAN_HIGH, FUJITSU_AC_FAN_HIGH, FUJITSU_AC_FAN_HIGH, FUJITSU_AC_FAN_QUIET};
  ac.setCmd(FUJITSU_AC_CMD_TURN_ON);
  ac.setSwing(FUJITSU_AC_SWING_VERT);

  char *p;
  if (nullptr == HVAC_Mode) {
    p = (char *)kFujitsuHvacModeOptions;
  }
  else {
    p = strchr(kFujitsuHvacModeOptions, toupper(HVAC_Mode[0]));
  }
  if (!p) {
    return IE_SYNTAX_IRHVAC;
  }
  ac.setMode(modes[p - kFujitsuHvacModeOptions]);

  if (HVAC_FanMode == nullptr) {
    p = (char *)kFanSpeedOptions; // default FAN_SPEED_AUTO
  }
  else {
    p = strchr(kFanSpeedOptions, toupper(HVAC_FanMode[0]));
  }
  if (!p) {
    return IE_SYNTAX_IRHVAC;
  }
  ac.setFanSpeed(fanModes[p - kFanSpeedOptions]);

  ac.setTemp(HVAC_Temp);
  ac.send();

  return IE_NO_ERROR;
}

#endif // USE_IR_HVAC

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

/*
 * ArduinoJSON entry used to calculate jsonBuf: JSON_OBJECT_SIZE(3) + 40 = 96
 IRsend:
 { "protocol": "RC5", "bits": 12, "data":"0xC86" }
 { "protocol": "SAMSUNG", "bits": 32, "data": 551502015 }
 IRhvac:
 { "Vendor": "<Toshiba|Mitsubishi>", "Power": <0|1>, "Mode": "<Hot|Cold|Dry|Auto>", "FanSpeed": "<1|2|3|4|5|Auto|Silence>", "Temp": <17..30> }
*/

bool IrSendCommand(void)
{
  char command [CMDSZ];
  bool serviced = true;
  uint8_t error = IE_NO_ERROR;

  int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic, kIrRemoteCommands);
  if (-1 == command_code) {
    serviced = false;  // Unknown command
  }
  else if (CMND_IRSEND == command_code) {
    if (XdrvMailbox.data_len) {
      Response_P(S_JSON_COMMAND_SVALUE, command, D_JSON_DONE);

      if (strstr(XdrvMailbox.data, "{") == nullptr) {  // If no JSON it must be rawdata
        // IRsend <freq>,<rawdata>,<rawdata> ...
        // or
        // IRsend raw,<freq>,<zero space>,<bit stream> (one space = zero space *2)
        // IRsend raw,<freq>,<zero space>,<zero space multiplier becoming one space>,<bit stream>
        // IRsend raw,<freq>,<zero space>,<one space>,<bit stream>
        // IRsend raw,<freq>,<header mark>,<header space>,<bit mark>,<zero space>,<one space>,<bit stream>
        char *p;
        char *str = strtok_r(XdrvMailbox.data, ", ", &p);
        if (p == nullptr) {
          error = IE_INVALID_RAWDATA;
        } else {
          uint16_t freq = atoi(str);
          if (!freq && (*str != '0')) {                // First parameter is any string
            uint16_t count = 0;
            char *q = p;
            for (; *q; count += (*q++ == ','));
            if (count < 2) {                           // Parameters must be at least 3
              error = IE_INVALID_RAWDATA;
            } else {
              uint16_t parm[count];
              for (uint32_t i = 0; i < count; i++) {
                parm[i] = strtol(strtok_r(nullptr, ", ", &p), nullptr, 0);
                if (!parm[i]) {
                  if (!i) {
                    parm[0] = 38000;                   // Frequency default to 38kHz
                  } else {
                    error = IE_INVALID_RAWDATA;        // Other parameters may not be 0
                    break;
                  }
                }
              }
              if (IE_NO_ERROR == error) {
                uint16_t i = 0;
                if (count < 4) {
                  // IRsend raw,0,889,000000100110000001001
                  uint16_t mark = parm[1] *2;          // Protocol where 0 = t, 1 = 2t (RC5)
                  if (3 == count) {
                    if (parm[2] < parm[1]) {
                      // IRsend raw,0,889,2,000000100110000001001
                      mark = parm[1] * parm[2];        // Protocol where 0 = t1, 1 = t1*t2 (Could be RC5)
                    } else {
                      // IRsend raw,0,889,1778,000000100110000001001
                      mark = parm[2];                  // Protocol where 0 = t1, 1 = t2 (Could be RC5)
                    }
                  }
                  uint16_t raw_array[strlen(p)];       // Bits
                  for (; *p; *p++) {
                    if (*p == '0') {
                      raw_array[i++] = parm[1];        // Space
                    }
                    else if (*p == '1') {
                      raw_array[i++] = mark;           // Mark
                    }
                  }
                  irsend_active = true;
                  irsend->sendRaw(raw_array, i, parm[0]);
                }
                else if (6 == count) {                 // NEC Protocol
                  // IRsend raw,0,8620,4260,544,411,1496,010101101000111011001110000000001100110000000001100000000000000010001100
                  uint16_t raw_array[strlen(p)*2+3];   // Header + bits + end
                  raw_array[i++] = parm[1];            // Header mark
                  raw_array[i++] = parm[2];            // Header space
                  for (; *p; *p++) {
                    if (*p == '0') {
                      raw_array[i++] = parm[3];        // Bit mark
                      raw_array[i++] = parm[4];        // Zero space
                    }
                    else if (*p == '1') {
                      raw_array[i++] = parm[3];        // Bit mark
                      raw_array[i++] = parm[5];        // One space
                    }
                  }
                  raw_array[i++] = parm[3];            // Trailing mark
                  irsend_active = true;
                  irsend->sendRaw(raw_array, i, parm[0]);
                }
                else {
                  error = IE_INVALID_RAWDATA;          // Invalid number of parameters
                }
              }
            }
          } else {
            if (!freq) { freq = 38000; }  // Default to 38kHz
            uint16_t count = 0;
            char *q = p;
            for (; *q; count += (*q++ == ','));
            if (0 == count) {
              error = IE_INVALID_RAWDATA;
            } else {  // At least two raw data values
              // IRsend 0,896,876,900,888,894,876,1790,874,872,1810,1736,948,872,880,872,936,872,1792,900,888,1734
              count++;
              if (count < 200) {
                uint16_t raw_array[count];  // It's safe to use stack for up to 200 packets (limited by mqtt_data length)
                for (uint32_t i = 0; i < count; i++) {
                  raw_array[i] = strtol(strtok_r(nullptr, ", ", &p), nullptr, 0);  // Allow decimal (20496) and hexadecimal (0x5010) input
                }

//              AddLog_P2(LOG_LEVEL_DEBUG, PSTR("DBG: stack count %d"), count);

                irsend_active = true;
                irsend->sendRaw(raw_array, count, freq);
              } else {
                uint16_t *raw_array = reinterpret_cast<uint16_t*>(malloc(count * sizeof(uint16_t)));
                if (raw_array == nullptr) {
                  error = IE_INVALID_RAWDATA;
                } else {
                  for (uint32_t i = 0; i < count; i++) {
                    raw_array[i] = strtol(strtok_r(nullptr, ", ", &p), nullptr, 0);  // Allow decimal (20496) and hexadecimal (0x5010) input
                  }

//              AddLog_P2(LOG_LEVEL_DEBUG, PSTR("DBG: heap count %d"), count);

                  irsend_active = true;
                  irsend->sendRaw(raw_array, count, freq);
                  free(raw_array);
                }
              }
            }
          }
        }
      } else {
        char dataBufUc[XdrvMailbox.data_len];
        UpperCase(dataBufUc, XdrvMailbox.data);
        RemoveSpace(dataBufUc);
        if (strlen(dataBufUc) < 8) {
          error = IE_INVALID_JSON;
        } else {
          StaticJsonBuffer<128> jsonBuf;
          JsonObject &root = jsonBuf.parseObject(dataBufUc);
          if (!root.success()) {
            error = IE_INVALID_JSON;
          } else {
            // IRsend { "protocol": "SAMSUNG", "bits": 32, "data": 551502015 }
            char parm_uc[10];
            const char *protocol = root[UpperCase_P(parm_uc, PSTR(D_JSON_IR_PROTOCOL))];
            uint16_t bits = root[UpperCase_P(parm_uc, PSTR(D_JSON_IR_BITS))];
            uint64_t data = strtoull(root[UpperCase_P(parm_uc, PSTR(D_JSON_IR_DATA))], nullptr, 0);
            if (protocol && bits) {
              char protocol_text[20];
              int protocol_code = GetCommandCode(protocol_text, sizeof(protocol_text), protocol, kIrRemoteProtocols);

              char dvalue[64];
              char hvalue[64];
              AddLog_P2(LOG_LEVEL_DEBUG, PSTR("IRS: protocol_text %s, protocol %s, bits %d, data %s (%s), protocol_code %d"),
                protocol_text, protocol, bits, ulltoa(data, dvalue, 10), IrUint64toHex(data, hvalue, bits), protocol_code);

              irsend_active = true;
              switch (protocol_code) {
                case NEC:
                  irsend->sendNEC(data, (bits > NEC_BITS) ? NEC_BITS : bits); break;
                case SONY:
                  irsend->sendSony(data, (bits > SONY_20_BITS) ? SONY_20_BITS : bits, 2); break;
                case RC5:
                  irsend->sendRC5(data, bits); break;
                case RC6:
                  irsend->sendRC6(data, bits); break;
                case DISH:
                  irsend->sendDISH(data, (bits > DISH_BITS) ? DISH_BITS : bits); break;
                case JVC:
                  irsend->sendJVC(data, (bits > JVC_BITS) ? JVC_BITS : bits, 1); break;
                case SAMSUNG:
                  irsend->sendSAMSUNG(data, (bits > SAMSUNG_BITS) ? SAMSUNG_BITS : bits); break;
                case PANASONIC:
//                  irsend->sendPanasonic(bits, data); break;
                  irsend->sendPanasonic64(data, bits); break;
                default:
                  irsend_active = false;
                  Response_P(S_JSON_COMMAND_SVALUE, command, D_JSON_PROTOCOL_NOT_SUPPORTED);
              }
            } else {
              error = IE_SYNTAX_IRSEND;
            }
          }
        }
      }
    } else {
      error = IE_SYNTAX_IRSEND;
    }
  }
#ifdef USE_IR_HVAC
  else if (CMND_IRHVAC == command_code) {
    const char *HVAC_Mode;
    const char *HVAC_FanMode;
    const char *HVAC_Vendor;
    int HVAC_Temp = 21;
    bool HVAC_Power = true;

    if (XdrvMailbox.data_len) {
      char dataBufUc[XdrvMailbox.data_len];
      UpperCase(dataBufUc, XdrvMailbox.data);
      RemoveSpace(dataBufUc);
      if (strlen(dataBufUc) < 8) {
        error = IE_INVALID_JSON;
      } else {
        StaticJsonBuffer<164> jsonBufer;
        JsonObject &root = jsonBufer.parseObject(dataBufUc);
        if (!root.success()) {
          error = IE_INVALID_JSON;
        } else {
          Response_P(S_JSON_COMMAND_SVALUE, command, D_JSON_DONE);
          HVAC_Vendor = root[D_JSON_IRHVAC_VENDOR];
          HVAC_Power = root[D_JSON_IRHVAC_POWER];
          HVAC_Mode = root[D_JSON_IRHVAC_MODE];
          HVAC_FanMode = root[D_JSON_IRHVAC_FANSPEED];
          HVAC_Temp = root[D_JSON_IRHVAC_TEMP];

  //        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("IRHVAC: Received Vendor %s, Power %d, Mode %s, FanSpeed %s, Temp %d"), HVAC_Vendor, HVAC_Power, HVAC_Mode, HVAC_FanMode, HVAC_Temp);

          char vendor[20];
          int vendor_code = GetCommandCode(vendor, sizeof(vendor), HVAC_Vendor, kIrHvacVendors);
          switch (vendor_code) {
            case VNDR_TOSHIBA:
              error = IrHvacToshiba(HVAC_Mode, HVAC_FanMode, HVAC_Power, HVAC_Temp); break;
            case VNDR_MITSUBISHI:
              error = IrHvacMitsubishi(HVAC_Mode, HVAC_FanMode, HVAC_Power, HVAC_Temp); break;
            case VNDR_LG:
              error = IrHvacLG(HVAC_Mode, HVAC_FanMode, HVAC_Power, HVAC_Temp); break;
            case VNDR_FUJITSU:
              error = IrHvacFujitsu(HVAC_Mode, HVAC_FanMode, HVAC_Power, HVAC_Temp); break;
            default:
              error = IE_SYNTAX_IRHVAC;
          }
        }
      }
    }
    else {
      error = IE_SYNTAX_IRHVAC;
    }
  }
#endif // USE_IR_HVAC
  else serviced = false;  // Unknown command

  switch (error) {
    case IE_INVALID_RAWDATA:
      Response_P(S_JSON_COMMAND_SVALUE, command, D_JSON_INVALID_RAWDATA);
      break;
    case IE_INVALID_JSON:
      Response_P(S_JSON_COMMAND_SVALUE, command, D_JSON_INVALID_JSON);
      break;
    case IE_SYNTAX_IRSEND:
      Response_P(PSTR("{\"" D_CMND_IRSEND "\":\"" D_JSON_NO " " D_JSON_IR_PROTOCOL ", " D_JSON_IR_BITS " " D_JSON_OR " " D_JSON_IR_DATA "\"}"));
      break;
#ifdef USE_IR_HVAC
    case IE_SYNTAX_IRHVAC:
      Response_P(PSTR("{\"" D_CMND_IRHVAC "\":\"" D_JSON_WRONG " " D_JSON_IRHVAC_VENDOR ", " D_JSON_IRHVAC_MODE " " D_JSON_OR " " D_JSON_IRHVAC_FANSPEED "\"}"));
      break;
#endif // USE_IR_HVAC
  }

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv05(uint8_t function)
{
  bool result = false;

  if ((pin[GPIO_IRSEND] < 99) || (pin[GPIO_IRRECV] < 99)) {
    switch (function) {
      case FUNC_PRE_INIT:
        if (pin[GPIO_IRSEND] < 99) {
          IrSendInit();
        }
#ifdef USE_IR_RECEIVE
        if (pin[GPIO_IRRECV] < 99) {
          IrReceiveInit();
        }
#endif  // USE_IR_RECEIVE
        break;
      case FUNC_EVERY_50_MSECOND:
#ifdef USE_IR_RECEIVE
        if (pin[GPIO_IRRECV] < 99) {
          IrReceiveCheck();  // check if there's anything on IR side
        }
#endif  // USE_IR_RECEIVE
        irsend_active = false;  // re-enable IR reception
        break;
      case FUNC_COMMAND:
        if (pin[GPIO_IRSEND] < 99) {
          result = IrSendCommand();
        }
        break;
    }
  }
  return result;
}

#endif // USE_IR_REMOTE
