/*
  xdrv_05_irremote_full.ino - complete integration of IRremoteESP8266 for Tasmota

  Copyright (C) 2021  Heiko Krupp, Lazar Obradovic, Theo Arends, Stephan Hadinger

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

/*
Below is the Pyhton3 code to decompress IR comact format.

======================================================================
import re

def ir_expand(ir_compact):
	count = ir_compact.count(',')		# number of occurence of comma

	if count > 1:
		return "Unsupported format"

	if count == 1:
		ir_compact = input.split(',')[1]	# if 1 comma, skip the frequency

	arr = re.findall("(\d+|[A-Za-z])", ir_compact)

	comp_table = []			# compression history table
	arr2 = []				# output

	for elt in arr:
		if len(elt) == 1:
			c = ord(elt.upper()) - ord('A')
			if c >= len(arr): return "Error index undefined"
			arr2.append(comp_table[c])
		else:
			comp_table.append(elt)
			arr2.append(elt)

	out = ",".join(arr2)

	return out
======================================================================
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

// Receiving IR while sending at the same time (i.e. receiving your own signal) was dsiabled in #10041
// At the demand of @pilaGit, you can `#define IR_RCV_WHILE_SENDING 1` to bring back this behavior
#ifndef IR_RCV_WHILE_SENDING
#define IR_RCV_WHILE_SENDING  0
#endif

enum IrErrors { IE_RESPONSE_PROVIDED, IE_NO_ERROR, IE_INVALID_RAWDATA, IE_INVALID_JSON, IE_SYNTAX_IRSEND, IE_SYNTAX_IRHVAC,
                IE_UNSUPPORTED_HVAC, IE_UNSUPPORTED_PROTOCOL, IE_MEMORY };

const char kIrRemoteCommands[] PROGMEM = "|"
  D_CMND_IRHVAC "|" D_CMND_IRSEND ; // No prefix

void (* const IrRemoteCommand[])(void) PROGMEM = {
  &CmndIrHvac, &CmndIrSend };

/*********************************************************************************************\
 * Class used to make a compact IR Raw format.
 *
 * We round timings to the closest 10ms value,
 * and store up to last 26 values with seen.
 * A value already seen is encoded with a letter indicating the position in the table.
\*********************************************************************************************/

class IRRawTable {
public:
  IRRawTable() : timings() {}   // zero initialize the array

  int32_t getTimingForLetter(uint8_t l) const {
    l = toupper(l);
    if ((l < 'A') || (l > 'Z')) { return -1; }
    return timings[l - 'A'];
  }
  uint8_t findOrAdd(uint16_t t) {
    if (0 == t) { return 0; }

    for (uint32_t i=0; i<26; i++) {
      if (timings[i] == t) { return i + 'A'; }
      if (timings[i] == 0) { timings[i] = t; break; }   // add new value
    }
    return 0;   // not found
  }
  void add(uint16_t t) {
    if (0 == t) { return; }

    for (uint32_t i=0; i<26; i++) {
      if (timings[i] == 0) { timings[i] = t; break; }   // add new value
    }
  }

protected:
  uint16_t timings[26];
};

/*********************************************************************************************\
 * IR Send
\*********************************************************************************************/

IRsend *irsend = nullptr;
// some ACs send toggle messages rather than state. we need to help IRremoteESP8266 keep track of the state
// have a flag that is a variable, can be later used to convert this functionality to an option (as in SetOptionXX)
bool irhvac_stateful = true;
stdAc::state_t irac_prev_state; // this implementations only keeps one state so if you use a single tasmota-ir device to command more than one AC it might not work

// different modes on how to handle state when sending HVAC commands. needed for ACs with a differential/toggle protocol.
enum class StateModes { SEND_ONLY, // just send the IR signal, this is the default. expect the state to update when the IR receiver gets the command that IR transmitter sent.
  STORE_ONLY, // just update the state to what is provided, this is when one needs to sync actual and stored states.
  SEND_STORE }; // send IR signal but also update stored state. this is for use cases when there is just one transmitter and there is no receiver in the device.
StateModes strToStateMode(class JsonParserToken token, StateModes def); // declate to prevent errors related to ino files

void IrSendInit(void)
{
  irsend = new IRsend(Pin(GPIO_IRSEND), IR_SEND_INVERTED, IR_SEND_USE_MODULATION); // an IR led is at GPIO_IRSEND
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
const uint32_t IR_TIME_AVOID_DUPLICATE = 50;  // Milliseconds

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
  JsonGeneratorObject json;
  json.add(PSTR(D_JSON_IRHVAC_VENDOR), typeToString(state.protocol));
  json.add(PSTR(D_JSON_IRHVAC_MODEL), state.model);

  json.add(PSTR(D_JSON_IRHVAC_MODE), IRac::opmodeToString(state.mode));
  // Home Assistant wants power to be off if mode is also off.
  if (state.mode == stdAc::opmode_t::kOff) {
    json.add(PSTR(D_JSON_IRHVAC_POWER),  IRac::boolToString(false));
  } else {
    json.add(PSTR(D_JSON_IRHVAC_POWER), IRac::boolToString(state.power));
  }
  json.add(PSTR(D_JSON_IRHVAC_CELSIUS), IRac::boolToString(state.celsius));
  if (floorf(state.degrees) == state.degrees) {
    json.add(PSTR(D_JSON_IRHVAC_TEMP), (int32_t) floorf(state.degrees));       // integer
  } else {
    // TODO can do better here
    json.addStrRaw(PSTR(D_JSON_IRHVAC_TEMP), String(state.degrees, 1).c_str());   // non-integer, limit to only 1 sub-digit
  }

  json.add(PSTR(D_JSON_IRHVAC_FANSPEED), IRac::fanspeedToString(state.fanspeed));
  json.add(PSTR(D_JSON_IRHVAC_SWINGV), IRac::swingvToString(state.swingv));
  json.add(PSTR(D_JSON_IRHVAC_SWINGH), IRac::swinghToString(state.swingh));
  json.add(PSTR(D_JSON_IRHVAC_QUIET), IRac::boolToString(state.quiet));
  json.add(PSTR(D_JSON_IRHVAC_TURBO), IRac::boolToString(state.turbo));
  json.add(PSTR(D_JSON_IRHVAC_ECONO), IRac::boolToString(state.econo));
  json.add(PSTR(D_JSON_IRHVAC_LIGHT), IRac::boolToString(state.light));
  json.add(PSTR(D_JSON_IRHVAC_FILTER), IRac::boolToString(state.filter));
  json.add(PSTR(D_JSON_IRHVAC_CLEAN), IRac::boolToString(state.clean));
  json.add(PSTR(D_JSON_IRHVAC_BEEP), IRac::boolToString(state.beep));
  json.add(PSTR(D_JSON_IRHVAC_SLEEP), state.sleep);

  String payload = json.toString(); // copy string before returning, the original is on the stack
  return payload;
}

void sendIRJsonState(const struct decode_results &results) {
  ResponseAppend_P(PSTR("\"" D_JSON_IR_PROTOCOL "\":\"%s\",\"" D_JSON_IR_BITS "\":%d"),
                  typeToString(results.decode_type).c_str(),
                  results.bits);

  if (hasACState(results.decode_type)) {
    ResponseAppend_P(PSTR(",\"" D_JSON_IR_DATA "\":\"%s\""),
                          resultToHexidecimal(&results).c_str());
  } else {
    ResponseAppend_P(PSTR(",\"%s\":"), UNKNOWN != results.decode_type ? PSTR(D_JSON_IR_DATA) : PSTR(D_JSON_IR_HASH));
    if (Settings.flag.ir_receive_decimal) {  // SetOption29 - IR receive data format
      ResponseAppend_P(PSTR("%u"), (uint32_t) results.value);
    } else {
      if (UNKNOWN != results.decode_type) {
        uint64_t reverse = reverseBitsInBytes64(results.value);
        ResponseAppend_P(PSTR("\"0x%0_X\",\"" D_JSON_IR_DATALSB "\":\"0x%0_X\""),
                         &results.value, &reverse);
      } else {    // UNKNOWN
        ResponseAppend_P(PSTR("\"0x%08X\""), (uint32_t) results.value);  // Unknown is always 32 bits
      }
    }
  }
  ResponseAppend_P(PSTR(",\"" D_JSON_IR_REPEAT "\":%d"), results.repeat);

  stdAc::state_t new_state;
  if (IRAcUtils::decodeToState(&results, &new_state, irhvac_stateful && irac_prev_state.protocol == results.decode_type ? &irac_prev_state : nullptr)) {
    // we have a decoded state
    ResponseAppend_P(PSTR(",\"" D_CMND_IRHVAC "\":%s"), sendACJsonState(new_state).c_str());
    irac_prev_state = new_state; // store for next time
  }
}

void IrReceiveCheck(void)
{
  decode_results results;

  if (irrecv->decode(&results)) {
    uint32_t now = millis();

//    if ((now - ir_lasttime > IR_TIME_AVOID_DUPLICATE) && (UNKNOWN != results.decode_type) && (results.bits > 0)) {
    if (now - ir_lasttime > IR_TIME_AVOID_DUPLICATE) {
      ir_lasttime = now;
      Response_P(PSTR("{\"" D_JSON_IRRECEIVED "\":{"));
      sendIRJsonState(results);

      IRRawTable raw_table;
      bool prev_number = false;     // was the previous value a number, meaning we may need a comma prefix
      bool ir_high = true;          // alternate high/low
      // Add raw data in a compact format
      if (Settings.flag3.receive_raw) {  // SetOption58 - Add IR Raw data to JSON message
        ResponseAppend_P(PSTR(",\"" D_JSON_IR_RAWDATA "\":\""));
        size_t rawlen = results.rawlen;
        uint32_t i;

        for (i = 1; i < rawlen; i++) {
          // round to closest 10ms
          uint32_t raw_val_millis = results.rawbuf[i] * kRawTick;
          uint16_t raw_dms = (raw_val_millis*2 + 5) / 10;   // in 5 micro sec steps
          // look if the data is already seen
          uint8_t  letter = raw_table.findOrAdd(raw_dms);
          if (letter) {
            if (!ir_high) { letter = tolower(letter); }
            ResponseAppend_P(PSTR("%c"), letter);
            prev_number = false;
          } else {
            // number
            ResponseAppend_P(PSTR("%c%d"), ir_high ? '+' : '-', (uint32_t)raw_dms * 5);
            prev_number = true;
          }
          ir_high = !ir_high;
          if (strlen(TasmotaGlobal.mqtt_data) > sizeof(TasmotaGlobal.mqtt_data) - 40) { break; }  // Quit if char string becomes too long
        }
        uint16_t extended_length = getCorrectedRawLength(&results);
        ResponseAppend_P(PSTR("\",\"" D_JSON_IR_RAWDATA "Info\":[%d,%d,%d]"), extended_length, i -1, results.overflow);
      }

      ResponseJsonEndEnd();
      MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_IRRECEIVED));
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

bool strToBool(class JsonParserToken token, bool def) {
  if (token.isBool() || token.isNum()) {
    return token.getBool();
  } else if (token.isStr()) {
    return IRac::strToBool(token.getStr());
  } else {
    return def;
  }
}

StateModes strToStateMode(class JsonParserToken token, StateModes def) {
  if (token.isStr()) {
    const char * str = token.getStr();
    if (!strcasecmp_P(str, PSTR(D_JSON_IRHVAC_STATE_MODE_SEND_ONLY)))
      return StateModes::SEND_ONLY;
    else if (!strcasecmp_P(str, PSTR(D_JSON_IRHVAC_STATE_MODE_STORE_ONLY)))
      return StateModes::STORE_ONLY;
    else if (!strcasecmp_P(str, PSTR(D_JSON_IRHVAC_STATE_MODE_SEND_STORE)))
      return StateModes::SEND_STORE;
  }
  return def;
}

// used to convert values 0-5 to fanspeed_t
const stdAc::fanspeed_t IrHvacFanSpeed[] PROGMEM =  { stdAc::fanspeed_t::kAuto,
                      stdAc::fanspeed_t::kMin, stdAc::fanspeed_t::kLow,stdAc::fanspeed_t::kMedium,
                      stdAc::fanspeed_t::kHigh, stdAc::fanspeed_t::kMax };

uint32_t IrRemoteCmndIrHvacJson(void)
{
  stdAc::state_t state;

  //AddLog_P(LOG_LEVEL_DEBUG, PSTR("IRHVAC: Received %s"), XdrvMailbox.data);
  JsonParser parser(XdrvMailbox.data);
  JsonParserObject root = parser.getRootObject();
  if (!root) { return IE_INVALID_JSON; }

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

  JsonParserToken val;
  if (val = root[PSTR(D_JSON_IRHVAC_VENDOR)]) { state.protocol = strToDecodeType(val.getStr()); }
  if (val = root[PSTR(D_JSON_IRHVAC_PROTOCOL)]) { state.protocol = strToDecodeType(val.getStr()); }
  if (decode_type_t::UNKNOWN == state.protocol) { return IE_UNSUPPORTED_HVAC; }
  if (!IRac::isProtocolSupported(state.protocol)) { return IE_UNSUPPORTED_HVAC; }

  // for fan speed, we also support 1-5 values
  JsonParserToken tok_fan_speed = root[PSTR(D_JSON_IRHVAC_FANSPEED)];
  if (tok_fan_speed) {
    uint32_t fan_speed = tok_fan_speed.getUInt();
    if ((fan_speed >= 1) && (fan_speed <= 5)) {
      state.fanspeed = (stdAc::fanspeed_t) pgm_read_byte(&IrHvacFanSpeed[fan_speed]);
    } else {
      state.fanspeed = IRac::strToFanspeed(tok_fan_speed.getStr());
    }
  }

  if (val = root[PSTR(D_JSON_IRHVAC_MODEL)]) { state.model = IRac::strToModel(val.getStr()); }
  if (val = root[PSTR(D_JSON_IRHVAC_MODE)]) { state.mode = IRac::strToOpmode(val.getStr()); }
  if (val = root[PSTR(D_JSON_IRHVAC_SWINGV)]) { state.swingv = IRac::strToSwingV(val.getStr()); }
  if (val = root[PSTR(D_JSON_IRHVAC_SWINGH)]) { state.swingh = IRac::strToSwingH(val.getStr()); }
  state.degrees = root.getFloat(PSTR(D_JSON_IRHVAC_TEMP), state.degrees);
  // AddLog(LOG_LEVEL_DEBUG, PSTR("model %d, mode %d, fanspeed %d, swingv %d, swingh %d"),
  //             state.model, state.mode, state.fanspeed, state.swingv, state.swingh);

  // if and how we should handle the state for IRremote
  StateModes stateMode = StateModes::SEND_ONLY; // default
  if (irhvac_stateful && (val = root[PSTR(D_JSON_IRHVAC_STATE_MODE)])) { stateMode = strToStateMode(val, stateMode); }

  // decode booleans
  state.power   = strToBool(root[PSTR(D_JSON_IRHVAC_POWER)], state.power);
  state.celsius = strToBool(root[PSTR(D_JSON_IRHVAC_CELSIUS)], state.celsius);
  state.light   = strToBool(root[PSTR(D_JSON_IRHVAC_LIGHT)], state.light);
  state.beep    = strToBool(root[PSTR(D_JSON_IRHVAC_BEEP)], state.beep);
  state.econo   = strToBool(root[PSTR(D_JSON_IRHVAC_ECONO)], state.econo);
  state.filter  = strToBool(root[PSTR(D_JSON_IRHVAC_FILTER)], state.filter);
  state.turbo   = strToBool(root[PSTR(D_JSON_IRHVAC_TURBO)], state.turbo);
  state.quiet   = strToBool(root[PSTR(D_JSON_IRHVAC_QUIET)], state.quiet);
  state.clean   = strToBool(root[PSTR(D_JSON_IRHVAC_CLEAN)], state.clean);

  // optional timer and clock
  state.sleep = root.getInt(PSTR(D_JSON_IRHVAC_SLEEP), state.sleep);
  //if (json[D_JSON_IRHVAC_CLOCK]) { state.clock = json[D_JSON_IRHVAC_CLOCK]; }   // not sure it's useful to support 'clock'

  if (!IR_RCV_WHILE_SENDING && (irrecv != nullptr)) { irrecv->disableIRIn(); }
  if (stateMode == StateModes::SEND_ONLY || stateMode == StateModes::SEND_STORE) {
    IRac ac(Pin(GPIO_IRSEND), IR_SEND_INVERTED, IR_SEND_USE_MODULATION);
    bool success = ac.sendAc(state, irhvac_stateful && irac_prev_state.protocol == state.protocol ? &irac_prev_state : nullptr);
    if (!success) { return IE_SYNTAX_IRHVAC; }
  }
  if (stateMode == StateModes::STORE_ONLY || stateMode == StateModes::SEND_STORE) { // store state in memory
    irac_prev_state = state;
  }
  if (!IR_RCV_WHILE_SENDING && (irrecv != nullptr)) { irrecv->enableIRIn(); }

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
  // IRsend { "protocol": "RC5", "bits": 12, "data":"0xC86" }
  // IRsend { "protocol": "SAMSUNG", "bits": 32, "data": 551502015 }
  RemoveSpace(XdrvMailbox.data);    // TODO is this really needed?
  JsonParser parser(XdrvMailbox.data);
  JsonParserObject root = parser.getRootObject();
  if (!root) { return IE_INVALID_JSON; }

  // IRsend { "protocol": "SAMSUNG", "bits": 32, "data": 551502015 }
  // IRsend { "protocol": "NEC", "bits": 32, "data":"0x02FDFE80", "repeat": 2 }
  JsonParserToken value;

  decode_type_t protocol = decode_type_t::UNKNOWN;
  value = root[PSTR(D_JSON_IRHVAC_VENDOR)];
  if (root) { protocol = strToDecodeType(value.getStr()); }
  value = root[PSTR(D_JSON_IRHVAC_PROTOCOL)];
  if (root) { protocol = strToDecodeType(value.getStr()); }
  if (decode_type_t::UNKNOWN == protocol) { return IE_UNSUPPORTED_PROTOCOL; }

  uint16_t bits = root.getUInt(PSTR(D_JSON_IR_BITS), 0);
  uint16_t repeat = root.getUInt(PSTR(D_JSON_IR_REPEAT), 0);

  uint64_t data;
  value = root[PSTR(D_JSON_IR_DATALSB)];
  if (root) { data = reverseBitsInBytes64(value.getULong()); }    // accept LSB values
  value = root[PSTR(D_JSON_IR_DATA)];
  if (value) { data = value.getULong(); }       // or classical MSB (takes priority)
  if (0 == bits) { return IE_SYNTAX_IRSEND; }

  // check if the IRSend<x> is greater than repeat, but can be overriden with JSON
  if (XdrvMailbox.index > repeat + 1) { repeat = XdrvMailbox.index - 1; }

  char dvalue[32];
  char hvalue[32];
  // AddLog(LOG_LEVEL_DEBUG, PSTR("IRS: protocol %d, bits %d, data 0x%s (%s), repeat %d"),
  //   protocol, bits, ulltoa(data, dvalue, 10), Uint64toHex(data, hvalue, bits), repeat);

  if (!IR_RCV_WHILE_SENDING && (irrecv != nullptr)) { irrecv->disableIRIn(); }
  bool success = irsend->send(protocol, data, bits, repeat);
  if (!IR_RCV_WHILE_SENDING && (irrecv != nullptr)) { irrecv->enableIRIn(); }

  if (!success) {
      ResponseCmndChar(D_JSON_PROTOCOL_NOT_SUPPORTED);
  }
  return IE_NO_ERROR;
}

//
// Send Global Cache commands
//
// Input:
//   p: token for strtok_r()
//   count: number of commas in parameters, i.e. it contains count+1 values
//   repeat: number of repeats (0 means no repeat)
//
uint32_t IrRemoteSendGC(char ** pp, uint32_t count, uint32_t repeat) {
  // IRsend gc,1000,2000,2000,1000
  uint16_t GC[count+1];
  for (uint32_t i = 0; i <= count; i++) {
    GC[i] = strtol(strtok_r(nullptr, ",", pp), nullptr, 0);
    if (!GC[i]) { return IE_INVALID_RAWDATA; }
  }
  if (!IR_RCV_WHILE_SENDING && (irrecv != nullptr)) { irrecv->disableIRIn(); }
  for (uint32_t r = 0; r <= repeat; r++) {
    irsend->sendGC(GC, count+1);
  }
  if (!IR_RCV_WHILE_SENDING && (irrecv != nullptr)) { irrecv->enableIRIn(); }
  return IE_NO_ERROR;
}

//
// Send 'raw'
//
uint32_t IrRemoteSendRawFormatted(char ** pp, uint32_t count, uint32_t repeat) {
  if (count < 2) { return IE_INVALID_RAWDATA; }

  // parse frequency
  char * str = strtok_r(nullptr, ",", pp);
  uint16_t freq = parsqeFreq(str);

  // parse parameters from 1 to count-1
  // i.e: IRsend raw,0,889,1778,000000100110000001001 => count = 3, [889,1778]
  uint16_t parm[count-1];     // contains at least 1 value
  for (uint32_t i = 0; i < count-1; i++) {
    parm[i] = strtol(strtok_r(nullptr, ",", pp), nullptr, 0);
    if (0 == parm[i]) { return IE_INVALID_RAWDATA; } // parameters may not be 0
  }

  uint16_t i = 0;
  if (count < 4) {
    // IRsend raw,0,889,000000100110000001001
    // IRsend raw,0,889,2,000000100110000001001
    // IRsend raw,0,889,1778,000000100110000001001
    // IRsend raw,40,900,2000,000000100110000001001
    uint16_t mark, space;
    space = parm[0];
    mark = space * 2;                            // Protocol where 0 = t, 1 = 2t (RC5)
    if (3 == count) {
      if (parm[1] <= 10) {
        // IRsend raw,0,889,2,000000100110000001001
        mark = parm[0] * parm[1];                 // Protocol where 0 = t1, 1 = t1*t2 (Could be RC5)
      } else {
        // IRsend raw,0,889,1778,000000100110000001001
        mark = parm[1];                           // Protocol where 0 = t1, 1 = t2 (Could be RC5)
      }
    }

    // p points to the last parameter
    uint16_t raw_array[strlen(*pp)];                // Bits
    for (; **pp; *(*pp)++) {
      if (**pp == '0') {
        raw_array[i++] = space;                 // Space
      }
      else if (**pp == '1') {
        raw_array[i++] = mark;                    // Mark
      }
    }
    if (!IR_RCV_WHILE_SENDING && (irrecv != nullptr)) { irrecv->disableIRIn(); }
    for (uint32_t r = 0; r <= repeat; r++) {
      // AddLog(LOG_LEVEL_DEBUG, PSTR("sendRaw count=%d, space=%d, mark=%d, freq=%d"), count, space, mark, freq);
      irsend->sendRaw(raw_array, i, freq);
      if (r < repeat) {         // if it's not the last message
        irsend->space(40000);   // since we don't know the inter-message gap, place an arbitrary 40ms gap
      }
    }
    if (!IR_RCV_WHILE_SENDING && (irrecv != nullptr)) { irrecv->enableIRIn(); }
  } else if (6 == count) {                          // NEC Protocol
    // IRsend raw,0,8620,4260,544,411,1496,010101101000111011001110000000001100110000000001100000000000000010001100
    uint16_t raw_array[strlen(*pp)*2+3];            // Header + bits + end
    raw_array[i++] = parm[0];                     // Header mark
    raw_array[i++] = parm[1];                     // Header space
    uint32_t inter_message_32 = (parm[0] + parm[1]) * 3;  // compute an inter-message gap (32 bits)
    uint16_t inter_message = (inter_message_32 > 65000) ? 65000 : inter_message_32;  // avoid 16 bits overflow
    for (; **pp; *(*pp)++) {
      if (**pp == '0') {
        raw_array[i++] = parm[2];                 // Bit mark
        raw_array[i++] = parm[3];                 // Zero space
      }
      else if (**pp == '1') {
        raw_array[i++] = parm[2];                 // Bit mark
        raw_array[i++] = parm[4];                 // One space
      }
    }
    raw_array[i++] = parm[2];                     // Trailing mark
    if (!IR_RCV_WHILE_SENDING && (irrecv != nullptr)) { irrecv->disableIRIn(); }
    for (uint32_t r = 0; r <= repeat; r++) {
      // AddLog(LOG_LEVEL_DEBUG, PSTR("sendRaw %d %d %d %d %d %d"), raw_array[0], raw_array[1], raw_array[2], raw_array[3], raw_array[4], raw_array[5]);
      irsend->sendRaw(raw_array, i, freq);
      if (r < repeat) {         // if it's not the last message
        irsend->space(inter_message);   // since we don't know the inter-message gap, place an arbitrary 40ms gap
      }
    }
    if (!IR_RCV_WHILE_SENDING && (irrecv != nullptr)) { irrecv->enableIRIn(); }
  }
  else { return IE_INVALID_RAWDATA; }                   // Invalid number of parameters
  return IE_NO_ERROR;
}

//
// Parse data as compact or standard form
//
// In:
//   str: the raw format, null terminated string. Cannot be PROGMEM nor be nullptr
//   arr: pointer to uint16_t array to populate, if nullptr then we just count items
//   arr_len: length of destination array, to avoid corrupting data. If arr_len == 0, ignore
uint32_t IrRemoteParseRawCompact(char * str, uint16_t * arr, size_t arr_len) {
  char *p = str;
  size_t i = 0;
  IRRawTable raw_table;

  for (char *p = str; *p; ) {
    int32_t value = -1;
    if ((arr_len > 0) && (i >= arr_len)) { return 0; }     // overflow

    while ((*p == ',') || (*p == '+') || (*p == '-')) { p++; }     // skip ',' '-' '+'
    if ((*p >= '0') && (*p <= '9')) {
      // parse number
      value = strtoul(p, &p, 10);
      raw_table.add(value);
    } else {
      value = raw_table.getTimingForLetter(*p);
      p++;
    }
    if (value < 0) { return 0; }    // invalid
    if (nullptr != arr) {
      arr[i] = value;
    }
    i++;
  }
  return i;
}

//
// Send raw standard
//
// Input:
//   p: token for strtok_r()
//   count: number of commas in parameters, i.e. it contains count+1 values
//   repeat: number of repeats (0 means no repeat)
//
uint32_t IrRemoteSendRawStandard(char ** pp, uint16_t freq, uint32_t count, uint32_t repeat) {
  // IRsend 0,896,876,900,888,894,876,1790,874,872,1810,1736,948,872,880,872,936,872,1792,900,888,1734
  // IRsend 0,+8570-4240+550-1580C-510+565-1565F-505Fh+570gFhIdChIgFeFgFgIhFgIhF-525C-1560IhIkI-520ChFhFhFgFhIkIhIgIgIkIkI-25270A-4225IkIhIgIhIhIkFhIkFjCgIhIkIkI-500IkIhIhIkFhIgIl+545hIhIoIgIhIkFhFgIkIgFgI

  uint16_t * arr = nullptr;
  if (count == 0) {
    // compact format, we need to parse in a first pass to know the number of frames
    count = IrRemoteParseRawCompact(*pp, nullptr, 0);
    if (0 == count) { return IE_INVALID_RAWDATA; }
  } else {
    count++;
  }
  // AddLog(LOG_LEVEL_DEBUG, PSTR("IrRemoteSendRawStandard: count_1 = %d"), count);
  arr = (uint16_t*) malloc(count * sizeof(uint16_t));
  if (nullptr == arr) { return IE_MEMORY; }

  count = IrRemoteParseRawCompact(*pp, arr, count);
  // AddLog(LOG_LEVEL_DEBUG, PSTR("IrRemoteSendRawStandard: count_2 = %d"), count);
  // AddLog(LOG_LEVEL_DEBUG, PSTR("Arr %d %d %d %d %d %d %d %d"), arr[0], arr[1], arr[2], arr[3], arr[4], arr[5], arr[6], arr[7]);
  if (0 == count) { return IE_INVALID_RAWDATA; }

  if (!IR_RCV_WHILE_SENDING && (irrecv != nullptr)) { irrecv->disableIRIn(); }
  for (uint32_t r = 0; r <= repeat; r++) {
    irsend->sendRaw(arr, count, freq);
  }
  if (!IR_RCV_WHILE_SENDING && (irrecv != nullptr)) { irrecv->enableIRIn(); }

  if (nullptr != arr) {
    free(arr);
  }
  return IE_NO_ERROR;
}

// parse the frequency value
uint16_t parsqeFreq(char * str) {
  uint16_t freq = atoi(str);
  if (0 == freq) { freq = 38000; }
  return freq;
}

uint32_t IrRemoteCmndIrSendRaw(void)
{
  // IRsend <freq>,<rawdata>,<rawdata> ...
  // IRsend <freq>,<compact_rawdata>
  // or
  // IRsend raw,<freq>,<zero space>,<bit stream> (one space = zero space *2)
  // IRsend raw,<freq>,<zero space>,<zero space multiplier becoming one space>,<bit stream>
  // IRsend raw,<freq>,<zero space>,<one space>,<bit stream>
  // IRsend raw,<freq>,<header mark>,<header space>,<bit mark>,<zero space>,<one space>,<bit stream>

  // check that there is at least one comma in the parameters
  char *p;
  char *str = strtok_r(XdrvMailbox.data, ",", &p);
  if (p == nullptr) { return IE_INVALID_RAWDATA; }

  // repeat is Index-1, so by default repeat = 0 (no repeat)
  uint16_t repeat = XdrvMailbox.index > 0 ? XdrvMailbox.index - 1 : 0;

  // count commas in parameters, after the first token skipped
  uint16_t count = 0;
  char *q = p;
  for (; *q; count += (*q++ == ','));

  // analyze first parameter
  if (strcasecmp(str, "gc") == 0) {
    // Global Cache protocol
    // IRsend gc,xxx,xxx,...
    return IrRemoteSendGC(&p, count, repeat);
  } else if (strcasecmp(str, "raw") == 0) {
    // IRsend raw,<freq>,<zero space>,<bit stream> (one space = zero space *2)
    // IRsend raw,<freq>,<zero space>,<zero space multiplier becoming one space>,<bit stream>
    // IRsend raw,<freq>,<zero space>,<one space>,<bit stream>
    // IRsend raw,<freq>,<header mark>,<header space>,<bit mark>,<zero space>,<one space>,<bit stream>
    return IrRemoteSendRawFormatted(&p, count, repeat);
  } else {
    // standard raw
    // IRsend <freq>,<rawdata>,<rawdata> ...
    // IRsend <freq>,<compact_rawdata>
    return IrRemoteSendRawStandard(&p, parsqeFreq(str), count, repeat);
  }
}

void CmndIrSend(void)
{
  uint8_t error = IE_SYNTAX_IRSEND;

  if (XdrvMailbox.data_len) {
    if (strchr(XdrvMailbox.data, '{') == nullptr) {
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
    case IE_MEMORY:
      ResponseCmndChar_P(PSTR(D_JSON_MEMORY_ERROR));
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
