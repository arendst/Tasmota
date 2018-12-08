/*
  xdrv_05_irremote.ino - infra red support for Sonoff-Tasmota

  Copyright (C) 2018  Heiko Krupp, Lazar Obradovic and Theo Arends

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
#define HVAC_TOSHIBA_HDR_MARK 4400
#define HVAC_TOSHIBA_HDR_SPACE 4300
#define HVAC_TOSHIBA_BIT_MARK 543
#define HVAC_TOSHIBA_ONE_SPACE 1623
#define HVAC_MISTUBISHI_ZERO_SPACE 472
#define HVAC_TOSHIBA_RPT_MARK 440
#define HVAC_TOSHIBA_RPT_SPACE 7048 // Above original iremote limit
#define HVAC_TOSHIBA_DATALEN 9

// HVAC LG
#define HVAC_LG_DATALEN 7

IRMitsubishiAC *mitsubir = NULL;

const char kFanSpeedOptions[] = "A12345S";
const char kHvacModeOptions[] = "HDCA";
#endif

/*********************************************************************************************\
 * IR Send
\*********************************************************************************************/

#include <IRsend.h>

IRsend *irsend = NULL;

void IrSendInit(void)
{
  irsend = new IRsend(pin[GPIO_IRSEND]); // an IR led is at GPIO_IRSEND
  irsend->begin();

#ifdef USE_IR_HVAC
  mitsubir = new IRMitsubishiAC(pin[GPIO_IRSEND]);
#endif //USE_IR_HVAC
}

#ifdef USE_IR_RECEIVE
/*********************************************************************************************\
 * IR Receive
\*********************************************************************************************/

#define IR_RCV_SAVE_BUFFER      0            // 0 = do not use buffer, 1 = use buffer for decoding

#define IR_TIME_AVOID_DUPLICATE 500          // Milliseconds

#include <IRrecv.h>

IRrecv *irrecv = NULL;

unsigned long ir_lasttime = 0;

void IrReceiveInit(void)
{
  // an IR led is at GPIO_IRRECV
  irrecv = new IRrecv(pin[GPIO_IRRECV], IR_RCV_BUFFER_SIZE, IR_RCV_TIMEOUT, IR_RCV_SAVE_BUFFER);
  irrecv->setUnknownThreshold(IR_RCV_MIN_UNKNOWN_SIZE);
  irrecv->enableIRIn();                  // Start the receiver

  //  AddLog_P(LOG_LEVEL_DEBUG, PSTR("IrReceive initialized"));
}

void IrReceiveCheck(void)
{
  char sirtype[14];  // Max is AIWA_RC_T501
  char stemp[16];
  int8_t iridx = 0;

  decode_results results;

  if (irrecv->decode(&results)) {

    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_IRR "RawLen %d, Overflow %d, Bits %d, Value %08X, Decode %d"),
               results.rawlen, results.overflow, results.bits, results.value, results.decode_type);
    AddLog(LOG_LEVEL_DEBUG);

    unsigned long now = millis();
//    if ((now - ir_lasttime > IR_TIME_AVOID_DUPLICATE) && (UNKNOWN != results.decode_type) && (results.bits > 0)) {
    if (now - ir_lasttime > IR_TIME_AVOID_DUPLICATE) {
      ir_lasttime = now;

      iridx = results.decode_type;
      if ((iridx < 0) || (iridx > 14)) {
        iridx = 0;  // UNKNOWN
      }
      if (Settings.flag.ir_receive_decimal) {
        snprintf_P(stemp, sizeof(stemp), PSTR("%u"), (uint32_t)results.value);
      } else {
        snprintf_P(stemp, sizeof(stemp), PSTR("\"%lX\""), (uint32_t)results.value);
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_IRRECEIVED "\":{\"" D_JSON_IR_PROTOCOL "\":\"%s\",\"" D_JSON_IR_BITS "\":%d,\"" D_JSON_IR_DATA "\":%s"),
        GetTextIndexed(sirtype, sizeof(sirtype), iridx, kIrRemoteProtocols), results.bits, stemp);

      if (Settings.flag3.receive_raw) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"" D_JSON_IR_RAWDATA "\":["), mqtt_data);
        uint16_t i;
        for (i = 1; i < results.rawlen; i++) {
          if (i > 1) { snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,"), mqtt_data); }
          uint32_t usecs;
          for (usecs = results.rawbuf[i] * kRawTick; usecs > UINT16_MAX; usecs -= UINT16_MAX) {
            snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%d,0,"), mqtt_data, UINT16_MAX);
          }
          snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%d"), mqtt_data, usecs);
          if (strlen(mqtt_data) > sizeof(mqtt_data) - 40) { break; }  // Quit if char string becomes too long
        }
        uint16_t extended_length = results.rawlen - 1;
        for (uint16_t j = 0; j < results.rawlen - 1; j++) {
          uint32_t usecs = results.rawbuf[j] * kRawTick;
          // Add two extra entries for multiple larger than UINT16_MAX it is.
          extended_length += (usecs / (UINT16_MAX + 1)) * 2;
        }
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s],\"" D_JSON_IR_RAWDATA "Info\":[%d,%d,%d]"), mqtt_data, extended_length, i -1, results.overflow);
      }

      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}}"), mqtt_data);
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

boolean IrHvacToshiba(const char *HVAC_Mode, const char *HVAC_FanMode, boolean HVAC_Power, int HVAC_Temp)
{
  uint16_t rawdata[2 + 2 * 8 * HVAC_TOSHIBA_DATALEN + 2];
  byte data[HVAC_TOSHIBA_DATALEN] = {0xF2, 0x0D, 0x03, 0xFC, 0x01, 0x00, 0x00, 0x00, 0x00};

  char *p;
  uint8_t mode;

  if (HVAC_Mode == NULL) {
    p = (char *)kHvacModeOptions; // default HVAC_HOT
  }
  else {
    p = strchr(kHvacModeOptions, toupper(HVAC_Mode[0]));
  }
  if (!p) {
    return true;
  }
  data[6] = (p - kHvacModeOptions) ^ 0x03; // HOT = 0x03, DRY = 0x02, COOL = 0x01, AUTO = 0x00

  if (!HVAC_Power) {
    data[6] = (byte)0x07; // Turn OFF HVAC
  }

  if (HVAC_FanMode == NULL) {
    p = (char *)kFanSpeedOptions; // default FAN_SPEED_AUTO
  }
  else {
    p = strchr(kFanSpeedOptions, toupper(HVAC_FanMode[0]));
  }
  if (!p) {
    return true;
  }
  mode = p - kFanSpeedOptions + 1;
  if ((1 == mode) || (7 == mode)) {
    mode = 0;
  }
  mode = mode << 5; // AUTO = 0x00, SPEED = 0x40, 0x60, 0x80, 0xA0, 0xC0, SILENT = 0x00
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
  data[5] = (byte)(Temp - 17) << 4;

  data[HVAC_TOSHIBA_DATALEN - 1] = 0;
  for (int x = 0; x < HVAC_TOSHIBA_DATALEN - 1; x++) {
    data[HVAC_TOSHIBA_DATALEN - 1] = (byte)data[x] ^ data[HVAC_TOSHIBA_DATALEN - 1]; // CRC is a simple bits addition
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
  irsend->sendRaw(rawdata, i, 38);
  irsend->sendRaw(rawdata, i, 38);
  interrupts();

  return false;
}


/*******************
     MITSUBISHI
********************/

boolean IrHvacMitsubishi(const char *HVAC_Mode, const char *HVAC_FanMode, boolean HVAC_Power, int HVAC_Temp)
{
  char *p;
  uint8_t mode;

  mitsubir->stateReset();

  if (HVAC_Mode == NULL) {
    p = (char *)kHvacModeOptions; // default HVAC_HOT
  }
  else {
    p = strchr(kHvacModeOptions, toupper(HVAC_Mode[0]));
  }
  if (!p) {
    return true;
  }
  mode = (p - kHvacModeOptions + 1) << 3; // HOT = 0x08, DRY = 0x10, COOL = 0x18, AUTO = 0x20
  mitsubir->setMode(mode);

  mitsubir->setPower(HVAC_Power);

  if (HVAC_FanMode == NULL) {
    p = (char *)kFanSpeedOptions; // default FAN_SPEED_AUTO
  }
  else {
    p = strchr(kFanSpeedOptions, toupper(HVAC_FanMode[0]));
  }
  if (!p) {
    return true;
  }
  mode = p - kFanSpeedOptions; // AUTO = 0, SPEED = 1 .. 5, SILENT = 6
  mitsubir->setFan(mode);

  mitsubir->setTemp(HVAC_Temp);
  mitsubir->setVane(MITSUBISHI_AC_VANE_AUTO);
  mitsubir->send();

//  snprintf_P(log_data, sizeof(log_data), PSTR("IRHVAC: Mitsubishi Power %d, Mode %d, FanSpeed %d, Temp %d, VaneMode %d"),
//    mitsubir->getPower(), mitsubir->getMode(), mitsubir->getFan(), mitsubir->getTemp(), mitsubir->getVane());
//  AddLog(LOG_LEVEL_DEBUG);

  return false;
}


/*******************
        LG
********************/

boolean IrHvacLG(const char *HVAC_Mode, const char *HVAC_FanMode, boolean HVAC_Power, int HVAC_Temp)
{
  uint32_t LG_Code;
  byte data[HVAC_LG_DATALEN];
  static boolean hvacOn = false;
  char *p;
  uint8_t mode;
  byte Temp;

  // Constant data
  data[0] = 0x08;
  data[1] = 0x08;
  data[2] = 0x00;

  if (!HVAC_Power) {
    data[2] = (byte)0x0C; // Turn OFF HVAC, code 0x88C0051
    data[3] = (byte)0x00;
    data[4] = (byte)0x00;
    data[5] = (byte)0x05;
    data[6] = (byte)0x01;
    hvacOn = false;
  }

  else {

    // Set code for HVAC Mode - data[3]
    if (HVAC_Mode == NULL) {
      p = (char *)kHvacModeOptions; // default HVAC_HOT
    }
    else {
      p = strchr(kHvacModeOptions, toupper(HVAC_Mode[0]));
    }
    if (!p) {
      return true;
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

//    snprintf_P(log_data, sizeof(log_data), PSTR("IRHVAC: HvacMode %s, ModeVal %d, Code %d"), p, mode, data[3]);
//    AddLog(LOG_LEVEL_DEBUG);

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
    data[4] = (byte)(Temp - 15);

    // Set code for HVAC fan mode - data[5]
    if (HVAC_FanMode == NULL) {
      p = (char *)kFanSpeedOptions; // default FAN_SPEED_AUTO
    }
    else {
      p = strchr(kFanSpeedOptions, toupper(HVAC_FanMode[0]));
    }
    if (!p) {
      return true;
    }
    mode = p - kFanSpeedOptions;
    if ((mode == 0) || (mode > 3)) {
      data[5] = 5; // Auto = 0x05
    }
    else {
      data[5] = (mode * 2) - 2; // Low = 0x00, Mid = 0x02, High = 0x04
    }

//    snprintf_P(log_data, sizeof(log_data), PSTR("IRHVAC: FanMode %s, ModeVal %d, Code %d"), p, mode, data[5]);
//    AddLog(LOG_LEVEL_DEBUG);

    // Set CRC code - data[6]
    data[6] = (data[3] + data[4] + data[5]) & 0x0f; // CRC

  }
  // Build LG IR code
  LG_Code = data[0] << 4;
  for (int i = 1; i < 6; i++) {
    LG_Code = (LG_Code + data[i]) << 4;
  }
  LG_Code = LG_Code + data[6];

//  snprintf_P(log_data, sizeof(log_data), PSTR("IRHVAC: LG_Code %d"), LG_Code);
//  AddLog(LOG_LEVEL_DEBUG);

  // Send LG IR Code
  noInterrupts();
  irsend->sendLG(LG_Code, 28);
  interrupts();

  return false;
}


/*******************
      Fujitsu
********************/

boolean IrHvacFujitsu(const char *HVAC_Mode, const char *HVAC_FanMode, boolean HVAC_Power, int HVAC_Temp)
{
  const char kFujitsuHvacModeOptions[] = "HDCAF";

//  snprintf_P(log_data, sizeof(log_data), PSTR("FUJITSU: mode:%s, fan:%s, power:%u, temp:%u"), HVAC_Mode, HVAC_FanMode, HVAC_Power, HVAC_Temp);
//  AddLog(LOG_LEVEL_DEBUG);

  IRFujitsuAC ac(pin[GPIO_IRSEND]);

  if (0 == HVAC_Power) {
    ac.off();
    ac.send();
    return false;
  }

  byte modes[5] = {FUJITSU_AC_MODE_HEAT, FUJITSU_AC_MODE_DRY, FUJITSU_AC_MODE_COOL, FUJITSU_AC_MODE_AUTO, FUJITSU_AC_MODE_FAN};
  byte fanModes[7] = {FUJITSU_AC_FAN_AUTO, FUJITSU_AC_FAN_LOW, FUJITSU_AC_FAN_MED, FUJITSU_AC_FAN_HIGH, FUJITSU_AC_FAN_HIGH, FUJITSU_AC_FAN_HIGH, FUJITSU_AC_FAN_QUIET};
  ac.setCmd(FUJITSU_AC_CMD_TURN_ON);
  ac.setSwing(FUJITSU_AC_SWING_VERT);

  char *p;
  if (NULL == HVAC_Mode) {
    p = (char *)kFujitsuHvacModeOptions;
  }
  else {
    p = strchr(kFujitsuHvacModeOptions, toupper(HVAC_Mode[0]));
  }
  if (!p) {
    return true;
  }
  ac.setMode(modes[p - kFujitsuHvacModeOptions]);

  if (HVAC_FanMode == NULL) {
    p = (char *)kFanSpeedOptions; // default FAN_SPEED_AUTO
  }
  else {
    p = strchr(kFanSpeedOptions, toupper(HVAC_FanMode[0]));
  }
  if (!p) {
    return true;
  }
  ac.setFanSpeed(fanModes[p - kFanSpeedOptions]);

  ac.setTemp(HVAC_Temp);
  ac.send();

  return false;
}

#endif // USE_IR_HVAC

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

boolean IrSendCommand(void)
{
  char command [CMDSZ];
  boolean serviced = true;
  boolean error = false;

  int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic, kIrRemoteCommands);
  if (-1 == command_code) {
    serviced = false;  // Unknown command
  }
  else if (CMND_IRSEND == command_code) {
    if (XdrvMailbox.data_len) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, D_JSON_DONE);

      if (!strstr(XdrvMailbox.data, "{")) {  // If no JSON it must be rawdata
        // IRSend frequency, rawdata, rawdata ...
        char *p;
        char *str = strtok_r(XdrvMailbox.data, ", ", &p);
        uint16_t freq = atoi(str);
        if (!freq) { freq = 38000; }  // Default to 38kHz
        uint16_t count = 0;
        char *q = p;
        for (; *q; count += (*q++ == ','));
        if (count) {  // At least two raw data values
          count++;
          uint16_t raw_array[count];  // It's safe to use stack for up to 240 packets (limited by mqtt_data length)
          byte i = 0;
          for (str = strtok_r(NULL, ", ", &p); str && i < count; str = strtok_r(NULL, ", ", &p)) {
            raw_array[i++] = strtoul(str, NULL, 0);  // Allow decimal (5246996) and hexadecimal (0x501014) input
          }

//          snprintf_P(log_data, sizeof(log_data), PSTR("IRS: Count %d, Freq %d, Arr[0] %d, Arr[count -1] %d"),
//            count, freq, raw_array[0], raw_array[count -1]);
//          AddLog(LOG_LEVEL_DEBUG);

          irsend->sendRaw(raw_array, count, freq);
          if (!count) {
            snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, D_JSON_FAILED);
          }
        }
        else {
          snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, D_JSON_INVALID_RAWDATA);
        }
      }
      else {
        char dataBufUc[XdrvMailbox.data_len];
        UpperCase(dataBufUc, XdrvMailbox.data);
        StaticJsonBuffer<128> jsonBuf;
        JsonObject &root = jsonBuf.parseObject(dataBufUc);
        if (!root.success()) {
          snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, D_JSON_INVALID_JSON);
        }
        else {
          // IRsend { "protocol": "SAMSUNG", "bits": 32, "data": 551502015 }
          char parm_uc[10];
          const char *protocol = root[UpperCase_P(parm_uc, PSTR(D_JSON_IR_PROTOCOL))];
          uint32_t bits = root[UpperCase_P(parm_uc, PSTR(D_JSON_IR_BITS))];
          uint32_t data = strtoul(root[UpperCase_P(parm_uc, PSTR(D_JSON_IR_DATA))], NULL, 0);
          if (protocol && bits) {
            char protocol_text[20];
            int protocol_code = GetCommandCode(protocol_text, sizeof(protocol_text), protocol, kIrRemoteProtocols);

            snprintf_P(log_data, sizeof(log_data), PSTR("IRS: protocol_text %s, protocol %s, bits %d, data %u (0x%lX), protocol_code %d"),
              protocol_text, protocol, bits, data, data, protocol_code);
            AddLog(LOG_LEVEL_DEBUG);

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
                irsend->sendPanasonic(bits, data); break;
              default:
                snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, D_JSON_PROTOCOL_NOT_SUPPORTED);
            }
          }
          else {
            error = true;
          }
        }
      }
    }
    else {
      error = true;
    }
    if (error) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_IRSEND "\":\"" D_JSON_NO " " D_JSON_IR_PROTOCOL ", " D_JSON_IR_BITS " " D_JSON_OR " " D_JSON_IR_DATA "\"}"));
    }
  }
#ifdef USE_IR_HVAC
  else if (CMND_IRHVAC == command_code) {
    const char *HVAC_Mode;
    const char *HVAC_FanMode;
    const char *HVAC_Vendor;
    int HVAC_Temp = 21;
    boolean HVAC_Power = true;

    if (XdrvMailbox.data_len) {
      char dataBufUc[XdrvMailbox.data_len];
      UpperCase(dataBufUc, XdrvMailbox.data);
      StaticJsonBuffer<164> jsonBufer;
      JsonObject &root = jsonBufer.parseObject(dataBufUc);
      if (!root.success()) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, D_JSON_INVALID_JSON);
      }
      else {
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, D_JSON_DONE);
        HVAC_Vendor = root[D_JSON_IRHVAC_VENDOR];
        HVAC_Power = root[D_JSON_IRHVAC_POWER];
        HVAC_Mode = root[D_JSON_IRHVAC_MODE];
        HVAC_FanMode = root[D_JSON_IRHVAC_FANSPEED];
        HVAC_Temp = root[D_JSON_IRHVAC_TEMP];

//        snprintf_P(log_data, sizeof(log_data), PSTR("IRHVAC: Received Vendor %s, Power %d, Mode %s, FanSpeed %s, Temp %d"),
//          HVAC_Vendor, HVAC_Power, HVAC_Mode, HVAC_FanMode, HVAC_Temp);
//        AddLog(LOG_LEVEL_DEBUG);

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
            error = true;
        }
      }
    }
    else {
      error = true;
    }
    if (error) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_IRHVAC "\":\"" D_JSON_WRONG " " D_JSON_IRHVAC_VENDOR ", " D_JSON_IRHVAC_MODE " " D_JSON_OR " " D_JSON_IRHVAC_FANSPEED "\"}"));
    }
  }
#endif // USE_IR_HVAC
  else serviced = false;  // Unknown command

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

boolean Xdrv05(byte function)
{
  boolean result = false;

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
