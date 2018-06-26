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

#include <IRremoteESP8266.h>

// Based on IRremoteESP8266.h enum decode_type_t
const char kIrRemoteProtocols[] PROGMEM =
  "UNKNOWN|RC5|RC6|NEC|SONY|PANASONIC|JVC|SAMSUNG|WHYNTER|AIWA_RC_T501|LG|SANYO|MITSUBISHI|DISH|SHARP";

#ifdef USE_IR_HVAC

#include <ir_Mitsubishi.h>

// HVAC TOSHIBA_
#define HVAC_TOSHIBA_HDR_MARK 4400
#define HVAC_TOSHIBA_HDR_SPACE 4300
#define HVAC_TOSHIBA_BIT_MARK 543
#define HVAC_TOSHIBA_ONE_SPACE 1623
#define HVAC_MISTUBISHI_ZERO_SPACE 472
#define HVAC_TOSHIBA_RPT_MARK 440
#define HVAC_TOSHIBA_RPT_SPACE 7048 // Above original iremote limit
#define HVAC_TOSHIBA_DATALEN 9

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

#include <IRrecv.h>

#define IR_TIME_AVOID_DUPLICATE 500 // Milliseconds

IRrecv *irrecv = NULL;
unsigned long ir_lasttime = 0;

void IrReceiveInit(void)
{
  irrecv = new IRrecv(pin[GPIO_IRRECV]); // an IR led is at GPIO_IRRECV
  irrecv->enableIRIn();                  // Start the receiver

  //  AddLog_P(LOG_LEVEL_DEBUG, PSTR("IrReceive initialized"));
}

void IrReceiveCheck()
{
  char sirtype[14];  // Max is AIWA_RC_T501
  char stemp[16];
  int8_t iridx = 0;

  decode_results results;

  if (irrecv->decode(&results)) {

    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_IRR "RawLen %d, Bits %d, Value %08X, Decode %d"),
               results.rawlen, results.bits, results.value, results.decode_type);
    AddLog(LOG_LEVEL_DEBUG);

    unsigned long now = millis();
    if ((now - ir_lasttime > IR_TIME_AVOID_DUPLICATE) && (UNKNOWN != results.decode_type) && (results.bits > 0)) {
      ir_lasttime = now;

      iridx = results.decode_type;
      if ((iridx < 0) || (iridx > 14)) {
        iridx = 0;
      }

      if (Settings.flag.ir_receive_decimal) {
        snprintf_P(stemp, sizeof(stemp), PSTR("%u"), (uint32_t)results.value);
      } else {
        snprintf_P(stemp, sizeof(stemp), PSTR("\"%lX\""), (uint32_t)results.value);
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_IRRECEIVED "\":{\"" D_JSON_IR_PROTOCOL "\":\"%s\",\"" D_JSON_IR_BITS "\":%d,\"" D_JSON_IR_DATA "\":%s}}"),
        GetTextIndexed(sirtype, sizeof(sirtype), iridx, kIrRemoteProtocols), results.bits, stemp);

      MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_IRRECEIVED));
      XdrvRulesProcess();
#ifdef USE_DOMOTICZ
      unsigned long value = results.value | (iridx << 28);  // [Protocol:4, Data:28]
      DomoticzSensor(DZ_COUNT, value);                      // Send data as Domoticz Counter value
#endif                                                      // USE_DOMOTICZ
    }

    irrecv->resume();
  }
}
#endif // USE_IR_RECEIVE

#ifdef USE_IR_HVAC
/*********************************************************************************************\
 * IR Heating, Ventilation and Air Conditioning using IRMitsubishiAC library
\*********************************************************************************************/

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

//boolean IrSendCommand(char *type, uint16_t index, char *dataBuf, uint16_t data_len, int16_t payload)
boolean IrSendCommand()
{
  boolean serviced = true;
  boolean error = false;
  char dataBufUc[XdrvMailbox.data_len];
  char protocol_text[20];
  const char *protocol;
  uint32_t bits = 0;
  uint32_t data = 0;

  UpperCase(dataBufUc, XdrvMailbox.data);
  if (!strcasecmp_P(XdrvMailbox.topic, PSTR(D_CMND_IRSEND))) {
    if (XdrvMailbox.data_len) {
      StaticJsonBuffer<128> jsonBuf;
      JsonObject &root = jsonBuf.parseObject(dataBufUc);
      if (!root.success()) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_IRSEND "\":\"" D_JSON_INVALID_JSON "\"}")); // JSON decode failed
      }
      else {
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_IRSEND "\":\"" D_JSON_DONE "\"}"));
        char parm_uc[10];
        protocol = root[UpperCase_P(parm_uc, PSTR(D_JSON_IR_PROTOCOL))];
        bits = root[UpperCase_P(parm_uc, PSTR(D_JSON_IR_BITS))];
        data = strtoul(root[UpperCase_P(parm_uc, PSTR(D_JSON_IR_DATA))], NULL, 0);
        if (protocol && bits) {
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
              snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_IRSEND "\":\"" D_JSON_PROTOCOL_NOT_SUPPORTED "\"}"));
          }
        }
        else {
          error = true;
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
  else if (!strcasecmp_P(XdrvMailbox.topic, PSTR(D_CMND_IRHVAC))) {
    const char *HVAC_Mode;
    const char *HVAC_FanMode;
    const char *HVAC_Vendor;
    int HVAC_Temp = 21;
    boolean HVAC_Power = true;

    if (XdrvMailbox.data_len) {
      StaticJsonBuffer<164> jsonBufer;
      JsonObject &root = jsonBufer.parseObject(dataBufUc);
      if (!root.success()) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_IRHVAC "\":\"" D_JSON_INVALID_JSON "\"}")); // JSON decode failed
      }
      else {
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_IRHVAC "\":\"" D_JSON_DONE "\"}"));
        HVAC_Vendor = root[D_JSON_IRHVAC_VENDOR];
        HVAC_Power = root[D_JSON_IRHVAC_POWER];
        HVAC_Mode = root[D_JSON_IRHVAC_MODE];
        HVAC_FanMode = root[D_JSON_IRHVAC_FANSPEED];
        HVAC_Temp = root[D_JSON_IRHVAC_TEMP];

        //        snprintf_P(log_data, sizeof(log_data), PSTR("IRHVAC: Received Vendor %s, Power %d, Mode %s, FanSpeed %s, Temp %d"),
        //          HVAC_Vendor, HVAC_Power, HVAC_Mode, HVAC_FanMode, HVAC_Temp);
        //        AddLog(LOG_LEVEL_DEBUG);

        if (HVAC_Vendor == NULL || !strcasecmp_P(HVAC_Vendor, PSTR("TOSHIBA"))) {
          error = IrHvacToshiba(HVAC_Mode, HVAC_FanMode, HVAC_Power, HVAC_Temp);
        }
        else if (!strcasecmp_P(HVAC_Vendor, PSTR("MITSUBISHI"))) {
          error = IrHvacMitsubishi(HVAC_Mode, HVAC_FanMode, HVAC_Power, HVAC_Temp);
        }
        else {
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
  else serviced = false; // Unknown command

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_05

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
