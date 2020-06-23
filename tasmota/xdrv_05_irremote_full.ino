/*
  xdrv_05_irremote_full.ino - complete integration of IRremoteESP8266 for Tasmota

  Copyright (C) 2020  Heiko Krupp, Lazar Obradovic, Theo Arends, Stephan Hadinger

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

#ifdef USE_IR_REMOTE_FULL
/*********************************************************************************************\
 * IR Remote send and receive using IRremoteESP8266 library
\*********************************************************************************************/

#define XDRV_05             5

#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <IRac.h>

enum IrErrors { IE_RESPONSE_PROVIDED, IE_NO_ERROR, IE_INVALID_RAWDATA, IE_INVALID_JSON, IE_SYNTAX_IRSEND, IE_SYNTAX_IRHVAC,
                IE_UNSUPPORTED_HVAC, IE_UNSUPPORTED_PROTOCOL };

const char kIrRemoteCommands[] PROGMEM = "|"
  D_CMND_IRHVAC "|" D_CMND_IRSEND ; // No prefix

void (* const IrRemoteCommand[])(void) PROGMEM = {
  &CmndIrHvac, &CmndIrSend };

/*********************************************************************************************\
 * IR Send
\*********************************************************************************************/

IRsend *irsend = nullptr;
bool irsend_active = false;

void IrSendInit(void)
{
  irsend = new IRsend(Pin(GPIO_IRSEND)); // an IR led is at GPIO_IRSEND
  irsend->begin();
}

// from https://stackoverflow.com/questions/2602823/in-c-c-whats-the-simplest-way-to-reverse-the-order-of-bits-in-a-byte
// First the left four bits are swapped with the right four bits. Then all adjacent pairs are swapped and then all adjacent single bits. This results in a reversed order.
uint8_t reverseBitsInByte(uint8_t b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

// reverse bits in each byte
uint64_t reverseBitsInBytes64(uint64_t b) {
  union {
    uint8_t b[8];
    uint64_t i;
  } a;
  a.i = b;
  for (uint32_t i=0; i<8; i++) {
    a.b[i] = reverseBitsInByte(a.b[i]);
  }
  return a.i;
}

/*********************************************************************************************\
 * IR Receive
\*********************************************************************************************/

const bool IR_FULL_RCV_SAVE_BUFFER = false;         // false = do not use buffer, true = use buffer for decoding
const uint32_t IR_TIME_AVOID_DUPLICATE = 500;  // Milliseconds

// Below is from IRrecvDumpV2.ino
// As this program is a special purpose capture/decoder, let us use a larger
// than normal buffer so we can handle Air Conditioner remote codes.
const uint16_t IR_FULL_BUFFER_SIZE = 1024;

// Some A/C units have gaps in their protocols of ~40ms. e.g. Kelvinator
// A value this large may swallow repeats of some protocols
const uint8_t IR__FULL_RCV_TIMEOUT = 50;

IRrecv *irrecv = nullptr;

unsigned long ir_lasttime = 0;

void IrReceiveUpdateThreshold(void)
{
  if (irrecv != nullptr) {
    if (Settings.param[P_IR_UNKNOW_THRESHOLD] < 6) { Settings.param[P_IR_UNKNOW_THRESHOLD] = 6; }
    irrecv->setUnknownThreshold(Settings.param[P_IR_UNKNOW_THRESHOLD]);
  }
}

void IrReceiveInit(void)
{
  // an IR led is at GPIO_IRRECV
  irrecv = new IRrecv(Pin(GPIO_IRRECV), IR_FULL_BUFFER_SIZE, IR__FULL_RCV_TIMEOUT, IR_FULL_RCV_SAVE_BUFFER);
  irrecv->setUnknownThreshold(Settings.param[P_IR_UNKNOW_THRESHOLD]);
  irrecv->enableIRIn();                  // Start the receiver
}

String sendACJsonState(const stdAc::state_t &state) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json[D_JSON_IRHVAC_VENDOR] = typeToString(state.protocol);
  json[D_JSON_IRHVAC_MODEL] = state.model;
  json[D_JSON_IRHVAC_POWER] = IRac::boolToString(state.power);
  json[D_JSON_IRHVAC_MODE] = IRac::opmodeToString(state.mode);
  // Home Assistant wants mode to be off if power is also off & vice-versa.
  if (state.mode == stdAc::opmode_t::kOff || !state.power) {
    json[D_JSON_IRHVAC_MODE] = IRac::opmodeToString(stdAc::opmode_t::kOff);
    json[D_JSON_IRHVAC_POWER] = IRac::boolToString(false);
  }
  json[D_JSON_IRHVAC_CELSIUS] = IRac::boolToString(state.celsius);
  if (floorf(state.degrees) == state.degrees) {
    json[D_JSON_IRHVAC_TEMP] = floorf(state.degrees);       // integer
  } else {
    json[D_JSON_IRHVAC_TEMP] = RawJson(String(state.degrees, 1));  // non-integer, limit to only 1 sub-digit
  }
  json[D_JSON_IRHVAC_FANSPEED] = IRac::fanspeedToString(state.fanspeed);
  json[D_JSON_IRHVAC_SWINGV] = IRac::swingvToString(state.swingv);
  json[D_JSON_IRHVAC_SWINGH] = IRac::swinghToString(state.swingh);
  json[D_JSON_IRHVAC_QUIET] = IRac::boolToString(state.quiet);
  json[D_JSON_IRHVAC_TURBO] = IRac::boolToString(state.turbo);
  json[D_JSON_IRHVAC_ECONO] = IRac::boolToString(state.econo);
  json[D_JSON_IRHVAC_LIGHT] = IRac::boolToString(state.light);
  json[D_JSON_IRHVAC_FILTER] = IRac::boolToString(state.filter);
  json[D_JSON_IRHVAC_CLEAN] = IRac::boolToString(state.clean);
  json[D_JSON_IRHVAC_BEEP] = IRac::boolToString(state.beep);
  json[D_JSON_IRHVAC_SLEEP] = state.sleep;

  String payload = "";
  payload.reserve(200);
  json.printTo(payload);
  return payload;
}

String sendIRJsonState(const struct decode_results &results) {
  String json("{");
  json += "\"" D_JSON_IR_PROTOCOL "\":\"";
  json += typeToString(results.decode_type);
  json += "\",\"" D_JSON_IR_BITS "\":";
  json += results.bits;

  if (hasACState(results.decode_type)) {
    json += ",\"" D_JSON_IR_DATA "\":\"0x";
    json += resultToHexidecimal(&results);
    json += "\"";
  } else {
    if (UNKNOWN != results.decode_type) {
      json += ",\"" D_JSON_IR_DATA "\":";
    } else {
      json += ",\"" D_JSON_IR_HASH "\":";
    }
    if (Settings.flag.ir_receive_decimal) {  // SetOption29 - IR receive data format
      char svalue[32];
      ulltoa(results.value, svalue, 10);
      json += svalue;
    } else {
      char hvalue[64];
      if (UNKNOWN != results.decode_type) {
        Uint64toHex(results.value, hvalue, results.bits);  // Get 64bit value as hex 0x00123456
        json += "\"0x";
        json += hvalue;
        json += "\",\"" D_JSON_IR_DATALSB "\":\"0x";
        Uint64toHex(reverseBitsInBytes64(results.value), hvalue, results.bits);  // Get 64bit value as hex 0x00123456, LSB
        json += hvalue;
        json += "\"";
      } else {    // UNKNOWN
        Uint64toHex(results.value, hvalue, 32);  // Unknown is always 32 bits
        json += "\"0x";
        json += hvalue;
        json += "\"";
      }
    }
  }
  json += ",\"" D_JSON_IR_REPEAT "\":";
  json += results.repeat;

  stdAc::state_t ac_result;
  if (IRAcUtils::decodeToState(&results, &ac_result, nullptr)) {
    // we have a decoded state
    json += ",\"" D_CMND_IRHVAC "\":";
    json += sendACJsonState(ac_result);
  }

  return json;
}

void IrReceiveCheck(void)
{
  decode_results results;

  if (irrecv->decode(&results)) {
    uint32_t now = millis();

//    if ((now - ir_lasttime > IR_TIME_AVOID_DUPLICATE) && (UNKNOWN != results.decode_type) && (results.bits > 0)) {
    if (!irsend_active && (now - ir_lasttime > IR_TIME_AVOID_DUPLICATE)) {
      ir_lasttime = now;
      Response_P(PSTR("{\"" D_JSON_IRRECEIVED "\":%s"), sendIRJsonState(results).c_str());

      if (Settings.flag3.receive_raw) {  // SetOption58 - Add IR Raw data to JSON message
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

      ResponseJsonEndEnd();
      MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_IRRECEIVED));

      XdrvRulesProcess();
    }

    irrecv->resume();
  }
}


/*********************************************************************************************\
 * IR Heating, Ventilation and Air Conditioning
\*********************************************************************************************/

// list all supported protocols, either for IRSend or for IRHVAC, separated by '|'
String listSupportedProtocols(bool hvac) {
  String l("");
  bool first = true;
  for (uint32_t i = UNUSED + 1; i <= kLastDecodeType; i++) {
    bool found = false;
    if (hvac) {
      found = IRac::isProtocolSupported((decode_type_t)i);
    } else {
      found = (IRsend::defaultBits((decode_type_t)i) > 0) && (!IRac::isProtocolSupported((decode_type_t)i));
    }
    if (found) {
      if (first) {
        first = false;
      } else {
        l += "|";
      }
      l += typeToString((decode_type_t)i);
    }
  }
  return l;
}

// used to convert values 0-5 to fanspeed_t
const stdAc::fanspeed_t IrHvacFanSpeed[] PROGMEM =  { stdAc::fanspeed_t::kAuto,
                      stdAc::fanspeed_t::kMin, stdAc::fanspeed_t::kLow,stdAc::fanspeed_t::kMedium,
                      stdAc::fanspeed_t::kHigh, stdAc::fanspeed_t::kMax };

uint32_t IrRemoteCmndIrHvacJson(void)
{
  stdAc::state_t state, prev;
  char parm_uc[12];

  //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("IRHVAC: Received %s"), XdrvMailbox.data);
  char dataBufUc[XdrvMailbox.data_len + 1];
  UpperCase(dataBufUc, XdrvMailbox.data);
  RemoveSpace(dataBufUc);
  if (strlen(dataBufUc) < 8) { return IE_INVALID_JSON; }

  DynamicJsonBuffer jsonBuf;
  JsonObject &json = jsonBuf.parseObject(dataBufUc);
  if (!json.success()) { return IE_INVALID_JSON; }

  // from: https://github.com/crankyoldgit/IRremoteESP8266/blob/master/examples/CommonAcControl/CommonAcControl.ino
  state.protocol = decode_type_t::UNKNOWN;
  state.model = 1;  // Some A/C's have different models. Let's try using just 1.
  state.mode = stdAc::opmode_t::kAuto;  // Run in cool mode initially.
  state.power = false;  // Initially start with the unit off.
  state.celsius = true;  // Use Celsius for units of temp. False = Fahrenheit
  state.degrees = 21.0f;  // 21 degrees.
  state.fanspeed = stdAc::fanspeed_t::kMedium;  // Start with the fan at medium.
  state.swingv = stdAc::swingv_t::kOff;  // Don't swing the fan up or down.
  state.swingh = stdAc::swingh_t::kOff;  // Don't swing the fan left or right.
  state.light = false;  // Turn off any LED/Lights/Display that we can.
  state.beep = false;  // Turn off any beep from the A/C if we can.
  state.econo = false;  // Turn off any economy modes if we can.
  state.filter = false;  // Turn off any Ion/Mold/Health filters if we can.
  state.turbo = false;  // Don't use any turbo/powerful/etc modes.
  state.quiet = false;  // Don't use any quiet/silent/etc modes.
  state.sleep = -1;  // Don't set any sleep time or modes.
  state.clean = false;  // Turn off any Cleaning options if we can.
  state.clock = -1;  // Don't set any current time if we can avoid it.

  UpperCase_P(parm_uc, PSTR(D_JSON_IRHVAC_VENDOR));
  if (json.containsKey(parm_uc)) { state.protocol = strToDecodeType(json[parm_uc]); }
  UpperCase_P(parm_uc, PSTR(D_JSON_IRHVAC_PROTOCOL));
  if (json.containsKey(parm_uc)) { state.protocol = strToDecodeType(json[parm_uc]); }   // also support 'protocol'
  if (decode_type_t::UNKNOWN == state.protocol) { return IE_UNSUPPORTED_HVAC; }
  if (!IRac::isProtocolSupported(state.protocol)) { return IE_UNSUPPORTED_HVAC; }

  // for fan speed, we also support 1-5 values
  UpperCase_P(parm_uc, PSTR(D_JSON_IRHVAC_FANSPEED));
  if (json.containsKey(parm_uc)) {
    uint32_t fan_speed = json[parm_uc];
    if ((fan_speed >= 1) && (fan_speed <= 5)) {
      state.fanspeed = (stdAc::fanspeed_t) pgm_read_byte(&IrHvacFanSpeed[fan_speed]);
    } else {
      state.fanspeed = IRac::strToFanspeed(json[parm_uc]);
    }
  }

  UpperCase_P(parm_uc, PSTR(D_JSON_IRHVAC_MODEL));
  if (json.containsKey(parm_uc)) { state.model = IRac::strToModel(json[parm_uc]); }
  UpperCase_P(parm_uc, PSTR(D_JSON_IRHVAC_MODE));
  if (json.containsKey(parm_uc)) { state.mode = IRac::strToOpmode(json[parm_uc]); }
  UpperCase_P(parm_uc, PSTR(D_JSON_IRHVAC_SWINGV));
  if (json.containsKey(parm_uc)) { state.swingv = IRac::strToSwingV(json[parm_uc]); }
  UpperCase_P(parm_uc, PSTR(D_JSON_IRHVAC_SWINGH));
  if (json.containsKey(parm_uc)) { state.swingh = IRac::strToSwingH(json[parm_uc]); }
  UpperCase_P(parm_uc, PSTR(D_JSON_IRHVAC_TEMP));
  if (json.containsKey(parm_uc)) { state.degrees = json[parm_uc]; }
  // AddLog_P2(LOG_LEVEL_DEBUG, PSTR("model %d, mode %d, fanspeed %d, swingv %d, swingh %d"),
  //             state.model, state.mode, state.fanspeed, state.swingv, state.swingh);

  // decode booleans
  UpperCase_P(parm_uc, PSTR(D_JSON_IRHVAC_POWER));
  if (json.containsKey(parm_uc)) { state.power = IRac::strToBool(json[parm_uc]); }
  UpperCase_P(parm_uc, PSTR(D_JSON_IRHVAC_CELSIUS));
  if (json.containsKey(parm_uc)) { state.celsius = IRac::strToBool(json[parm_uc]); }
  UpperCase_P(parm_uc, PSTR(D_JSON_IRHVAC_LIGHT));
  if (json.containsKey(parm_uc)) { state.light = IRac::strToBool(json[parm_uc]); }
  UpperCase_P(parm_uc, PSTR(D_JSON_IRHVAC_BEEP));
  if (json.containsKey(parm_uc)) { state.beep = IRac::strToBool(json[parm_uc]); }
  UpperCase_P(parm_uc, PSTR(D_JSON_IRHVAC_ECONO));
  if (json.containsKey(parm_uc)) { state.econo = IRac::strToBool(json[parm_uc]); }
  UpperCase_P(parm_uc, PSTR(D_JSON_IRHVAC_FILTER));
  if (json.containsKey(parm_uc)) { state.filter = IRac::strToBool(json[parm_uc]); }
  UpperCase_P(parm_uc, PSTR(D_JSON_IRHVAC_TURBO));
  if (json.containsKey(parm_uc)) { state.turbo = IRac::strToBool(json[parm_uc]); }
  UpperCase_P(parm_uc, PSTR(D_JSON_IRHVAC_QUIET));
  if (json.containsKey(parm_uc)) { state.quiet = IRac::strToBool(json[parm_uc]); }
  UpperCase_P(parm_uc, PSTR(D_JSON_IRHVAC_CLEAN));
  if (json.containsKey(parm_uc)) { state.clean = IRac::strToBool(json[parm_uc]); }

  // optional timer and clock
  UpperCase_P(parm_uc, PSTR(D_JSON_IRHVAC_SLEEP));
  if (json[parm_uc]) { state.sleep = json[parm_uc]; }
  //if (json[D_JSON_IRHVAC_CLOCK]) { state.clock = json[D_JSON_IRHVAC_CLOCK]; }   // not sure it's useful to support 'clock'

  IRac ac(Pin(GPIO_IRSEND));
  bool success = ac.sendAc(state, &prev);
  if (!success) { return IE_SYNTAX_IRHVAC; }

  Response_P(PSTR("{\"" D_CMND_IRHVAC "\":%s}"), sendACJsonState(state).c_str());
  return IE_RESPONSE_PROVIDED;
}

void CmndIrHvac(void)
{
  uint8_t error = IE_SYNTAX_IRHVAC;

  if (XdrvMailbox.data_len) {
    error = IrRemoteCmndIrHvacJson();
  }
  if (error != IE_RESPONSE_PROVIDED) { IrRemoteCmndResponse(error); }    // otherwise response was already provided
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

uint32_t IrRemoteCmndIrSendJson(void)
{
  char parm_uc[12];   // used to convert JSON keys to uppercase
  // ArduinoJSON entry used to calculate jsonBuf: JSON_OBJECT_SIZE(3) + 40 = 96
  // IRsend { "protocol": "RC5", "bits": 12, "data":"0xC86" }
  // IRsend { "protocol": "SAMSUNG", "bits": 32, "data": 551502015 }
  char dataBufUc[XdrvMailbox.data_len + 1];
  UpperCase(dataBufUc, XdrvMailbox.data);
  RemoveSpace(dataBufUc);
  if (strlen(dataBufUc) < 8) { return IE_INVALID_JSON; }

  DynamicJsonBuffer jsonBuf;
  JsonObject &json = jsonBuf.parseObject(dataBufUc);
  if (!json.success()) { return IE_INVALID_JSON; }

  // IRsend { "protocol": "SAMSUNG", "bits": 32, "data": 551502015 }
  // IRsend { "protocol": "NEC", "bits": 32, "data":"0x02FDFE80", "repeat": 2 }
  decode_type_t protocol = decode_type_t::UNKNOWN;
  uint16_t bits = 0;
  uint64_t data;
  uint8_t  repeat = 0;

  UpperCase_P(parm_uc, PSTR(D_JSON_IRHVAC_VENDOR));
  if (json.containsKey(parm_uc)) { protocol = strToDecodeType(json[parm_uc]); }
  UpperCase_P(parm_uc, PSTR(D_JSON_IRHVAC_PROTOCOL));
  if (json.containsKey(parm_uc)) { protocol = strToDecodeType(json[parm_uc]); }   // also support 'protocol'
  if (decode_type_t::UNKNOWN == protocol) { return IE_UNSUPPORTED_PROTOCOL; }

  UpperCase_P(parm_uc, PSTR(D_JSON_IR_BITS));
  if (json.containsKey(parm_uc)) { bits = json[parm_uc]; }
  UpperCase_P(parm_uc, PSTR(D_JSON_IR_REPEAT));
  if (json.containsKey(parm_uc)) { repeat = json[parm_uc]; }
  UpperCase_P(parm_uc, PSTR(D_JSON_IR_DATALSB));    // accept LSB values
  if (json.containsKey(parm_uc)) { data = reverseBitsInBytes64(strtoull(json[parm_uc], nullptr, 0)); }
  UpperCase_P(parm_uc, PSTR(D_JSON_IR_DATA));       // or classical MSB (takes priority)
  if (json.containsKey(parm_uc)) { data = strtoull(json[parm_uc], nullptr, 0); }
  if (0 == bits) { return IE_SYNTAX_IRSEND; }

  // check if the IRSend<x> is greater than repeat, but can be overriden with JSON
  if (XdrvMailbox.index > repeat + 1) { repeat = XdrvMailbox.index - 1; }

  char dvalue[32];
  char hvalue[32];
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("IRS: protocol %d, bits %d, data 0x%s (%s), repeat %d"),
    protocol, bits, ulltoa(data, dvalue, 10), Uint64toHex(data, hvalue, bits), repeat);

  irsend_active = true;     // deactivate receive
  bool success = irsend->send(protocol, data, bits, repeat);

  if (!success) {
      irsend_active = false;
      ResponseCmndChar(D_JSON_PROTOCOL_NOT_SUPPORTED);
  }
  return IE_NO_ERROR;
}

uint32_t IrRemoteCmndIrSendRaw(void)
{
  // IRsend <freq>,<rawdata>,<rawdata> ...
  // or
  // IRsend raw,<freq>,<zero space>,<bit stream> (one space = zero space *2)
  // IRsend raw,<freq>,<zero space>,<zero space multiplier becoming one space>,<bit stream>
  // IRsend raw,<freq>,<zero space>,<one space>,<bit stream>
  // IRsend raw,<freq>,<header mark>,<header space>,<bit mark>,<zero space>,<one space>,<bit stream>

  char *p;
  char *str = strtok_r(XdrvMailbox.data, ", ", &p);
  if (p == nullptr) {
    return IE_INVALID_RAWDATA;
  }

  // repeat
  uint16_t repeat = XdrvMailbox.index > 0 ? XdrvMailbox.index - 1 : 0;

  uint16_t freq = atoi(str);
  if (!freq && (*str != '0')) {                     // First parameter is any string
    uint16_t count = 0;
    char *q = p;
    for (; *q; count += (*q++ == ','));
    if (count < 2) {
      return IE_INVALID_RAWDATA;
    }   // Parameters must be at least 3


#ifdef IR_GC
//ir_gc

  if (strcmp(str, "gc") == 0) {  //if first parameter is gc then we process global cache data else it is raw
       
	uint16_t GC[count+1];
	for (uint32_t i = 0; i <= count; i++) {
	  GC[i] = strtol(strtok_r(nullptr, ", ", &p), nullptr, 0);
      if (!GC[i]) {
         return IE_INVALID_RAWDATA;                
      }
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("DBG: GC value %d"), GC[i]);
    }
    irsend->sendGC(GC, count+1);
		AddLog_P2(LOG_LEVEL_DEBUG, PSTR("DBG: GC sent count %d"), count);
    return IE_NO_ERROR;
  }
//end ir_gc
#endif



    uint16_t parm[count];
    for (uint32_t i = 0; i < count; i++) {
      parm[i] = strtol(strtok_r(nullptr, ", ", &p), nullptr, 0);
      if (!parm[i]) {
        if (!i) {
          parm[0] = 38000;                          // Frequency default to 38kHz
        } else {
          return IE_INVALID_RAWDATA;                // Other parameters may not be 0
        }
      }
    }

    uint16_t i = 0;
    if (count < 4) {
      // IRsend raw,0,889,000000100110000001001
      uint16_t mark = parm[1] *2;                   // Protocol where 0 = t, 1 = 2t (RC5)
      if (3 == count) {
        if (parm[2] < parm[1]) {
          // IRsend raw,0,889,2,000000100110000001001
          mark = parm[1] * parm[2];                 // Protocol where 0 = t1, 1 = t1*t2 (Could be RC5)
        } else {
          // IRsend raw,0,889,1778,000000100110000001001
          mark = parm[2];                           // Protocol where 0 = t1, 1 = t2 (Could be RC5)
        }
      }
      uint16_t raw_array[strlen(p)];                // Bits
      for (; *p; *p++) {
        if (*p == '0') {
          raw_array[i++] = parm[1];                 // Space
        }
        else if (*p == '1') {
          raw_array[i++] = mark;                    // Mark
        }
      }
      irsend_active = true;
      for (uint32_t r = 0; r <= repeat; r++) {
        irsend->sendRaw(raw_array, i, parm[0]);
        if (r < repeat) {         // if it's not the last message
          irsend->space(40000);   // since we don't know the inter-message gap, place an arbitrary 40ms gap
        }
      }
    }
    else if (6 == count) {                          // NEC Protocol
      // IRsend raw,0,8620,4260,544,411,1496,010101101000111011001110000000001100110000000001100000000000000010001100
      uint16_t raw_array[strlen(p)*2+3];            // Header + bits + end
      raw_array[i++] = parm[1];                     // Header mark
      raw_array[i++] = parm[2];                     // Header space
      uint32_t inter_message_32 = (parm[1] + parm[2]) * 3;  // compute an inter-message gap (32 bits)
      uint16_t inter_message = (inter_message_32 > 65000) ? 65000 : inter_message_32;  // avoid 16 bits overflow
      for (; *p; *p++) {
        if (*p == '0') {
          raw_array[i++] = parm[3];                 // Bit mark
          raw_array[i++] = parm[4];                 // Zero space
        }
        else if (*p == '1') {
          raw_array[i++] = parm[3];                 // Bit mark
          raw_array[i++] = parm[5];                 // One space
        }
      }
      raw_array[i++] = parm[3];                     // Trailing mark
      irsend_active = true;
      for (uint32_t r = 0; r <= repeat; r++) {
        irsend->sendRaw(raw_array, i, parm[0]);
        if (r < repeat) {         // if it's not the last message
          irsend->space(inter_message);   // since we don't know the inter-message gap, place an arbitrary 40ms gap
        }
      }
    }
    else {
      return IE_INVALID_RAWDATA;                    // Invalid number of parameters
    }
  } else {
    if (!freq) { freq = 38000; }                    // Default to 38kHz
    uint16_t count = 0;
    char *q = p;
    for (; *q; count += (*q++ == ','));
    if (0 == count) {
      return IE_INVALID_RAWDATA;
    }

    // IRsend 0,896,876,900,888,894,876,1790,874,872,1810,1736,948,872,880,872,936,872,1792,900,888,1734
    count++;
    if (count < 200) {
      uint16_t raw_array[count];  // It's safe to use stack for up to 200 packets (limited by mqtt_data length)
      for (uint32_t i = 0; i < count; i++) {
        raw_array[i] = strtol(strtok_r(nullptr, ", ", &p), nullptr, 0);  // Allow decimal (20496) and hexadecimal (0x5010) input
      }

//      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("DBG: stack count %d"), count);

      irsend_active = true;
      for (uint32_t r = 0; r <= repeat; r++) {
        irsend->sendRaw(raw_array, count, freq);
      }
    } else {
      uint16_t *raw_array = reinterpret_cast<uint16_t*>(malloc(count * sizeof(uint16_t)));
      if (raw_array == nullptr) {
        return IE_INVALID_RAWDATA;
      }

      for (uint32_t i = 0; i < count; i++) {
        raw_array[i] = strtol(strtok_r(nullptr, ", ", &p), nullptr, 0);  // Allow decimal (20496) and hexadecimal (0x5010) input
      }

//     AddLog_P2(LOG_LEVEL_DEBUG, PSTR("DBG: heap count %d"), count);

      irsend_active = true;
      for (uint32_t r = 0; r <= repeat; r++) {
        irsend->sendRaw(raw_array, count, freq);
      }
      free(raw_array);
    }
  }

  return IE_NO_ERROR;
}

void CmndIrSend(void)
{
  uint8_t error = IE_SYNTAX_IRSEND;

  if (XdrvMailbox.data_len) {
    if (strstr(XdrvMailbox.data, "{") == nullptr) {
      error = IrRemoteCmndIrSendRaw();
    } else {
      error = IrRemoteCmndIrSendJson();
    }
  }
  IrRemoteCmndResponse(error);
}

void IrRemoteCmndResponse(uint32_t error)
{
  switch (error) {
    case IE_INVALID_RAWDATA:
      ResponseCmndChar_P(PSTR(D_JSON_INVALID_RAWDATA));
      break;
    case IE_INVALID_JSON:
      ResponseCmndChar_P(PSTR(D_JSON_INVALID_JSON));
      break;
    case IE_SYNTAX_IRSEND:
      Response_P(PSTR("{\"" D_CMND_IRSEND "\":\"" D_JSON_NO " " D_JSON_IR_BITS " " D_JSON_OR " " D_JSON_IR_DATA "\"}"));
      break;
    case IE_SYNTAX_IRHVAC:
      Response_P(PSTR("{\"" D_CMND_IRHVAC "\":\"" D_JSON_WRONG " " D_JSON_IRHVAC_VENDOR ", " D_JSON_IRHVAC_MODE " " D_JSON_OR " " D_JSON_IRHVAC_FANSPEED "\"}"));
      break;
    case IE_UNSUPPORTED_HVAC:
      Response_P(PSTR("{\"" D_CMND_IRHVAC "\":\"" D_JSON_WRONG " " D_JSON_IRHVAC_VENDOR " (%s)\"}"), listSupportedProtocols(true).c_str());
      break;
    case IE_UNSUPPORTED_PROTOCOL:
      Response_P(PSTR("{\"" D_CMND_IRSEND "\":\"" D_JSON_WRONG " " D_JSON_IRHVAC_PROTOCOL " (%s)\"}"), listSupportedProtocols(false).c_str());
      break;
    default:  // IE_NO_ERROR
      ResponseCmndDone();
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv05(uint8_t function)
{
  bool result = false;

  if (PinUsed(GPIO_IRSEND) || PinUsed(GPIO_IRRECV)) {
    switch (function) {
      case FUNC_PRE_INIT:
        if (PinUsed(GPIO_IRSEND)) {
          IrSendInit();
        }
        if (PinUsed(GPIO_IRRECV)) {
          IrReceiveInit();
        }
        break;
      case FUNC_EVERY_50_MSECOND:
        if (PinUsed(GPIO_IRRECV)) {
          IrReceiveCheck();  // check if there's anything on IR side
        }
        irsend_active = false;  // re-enable IR reception
        break;
      case FUNC_COMMAND:
        if (PinUsed(GPIO_IRSEND)) {
          result = DecodeCommand(kIrRemoteCommands, IrRemoteCommand);
        }
        break;
    }
  }
  return result;
}

#endif // USE_IR_REMOTE_FULL
