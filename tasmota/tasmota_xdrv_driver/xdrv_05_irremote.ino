/*
  xdrv_05_irremote.ino - infra red support for Tasmota

  Copyright (C) 2021  Heiko Krupp, Lazar Obradovic and Theo Arends

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

#if defined(USE_IR_REMOTE) && !defined(USE_IR_REMOTE_FULL)
/*********************************************************************************************\
 * IR Remote send and receive using IRremoteESP8266 library
\*********************************************************************************************/

#define XDRV_05             5

#include <IRremoteESP8266.h>
#include <IRutils.h>

// Receiving IR while sending at the same time (i.e. receiving your own signal) was dsiabled in #10041
// At the demand of @pilaGit, you can `#define IR_RCV_WHILE_SENDING 1` to bring back this behavior
#ifndef IR_RCV_WHILE_SENDING
#define IR_RCV_WHILE_SENDING  0
#endif

enum IrErrors { IE_NO_ERROR, IE_INVALID_RAWDATA, IE_INVALID_JSON, IE_SYNTAX_IRSEND, IE_PROTO_UNSUPPORTED };

const char kIrRemoteCommands[] PROGMEM = "|" D_CMND_IRSEND ;

// Keep below IrRemoteCommand lines exactly as below as otherwise Arduino IDE prototyping will fail (#6982)
void (* const IrRemoteCommand[])(void) PROGMEM = {
  &CmndIrSend };

// 20220531 renamed as newer arduino core now also has this function
char* ir_ulltoa(unsigned long long value, char *str, int radix)
{
  char digits[64];
  char *dst = str;
  int i = 0;

//  if (radix < 2 || radix > 36) { radix = 10; }

  do {
    int n = value % radix;
    digits[i++] = (n < 10) ? (char)n+'0' : (char)n-10+'A';
    value /= radix;
  } while (value != 0);

  while (i > 0) { *dst++ = digits[--i]; }

  *dst = 0;
  return str;
}

char* Uint64toHex(uint64_t value, char *str, uint16_t bits)
{
  ir_ulltoa(value, str, 16);  // Get 64bit value

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
  return str;
}

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

// Based on IRremoteESP8266.h enum decode_type_t
static const uint8_t MAX_STANDARD_IR = NEC;   // this is the last code mapped to decode_type_t
const char kIrRemoteProtocols[] PROGMEM = "UNKNOWN|RC5|RC6|NEC";

/*********************************************************************************************\
 * IR Send
\*********************************************************************************************/

#include <IRsend.h>

IRsend *irsend = nullptr;

void IrSendInit(void)
{
  irsend = new IRsend(Pin(GPIO_IRSEND), IR_SEND_INVERTED, IR_SEND_USE_MODULATION); // an IR led is at GPIO_IRSEND
  irsend->begin();
}

#ifdef USE_IR_RECEIVE
/*********************************************************************************************\
 * IR Receive
\*********************************************************************************************/

const bool IR_RCV_SAVE_BUFFER = false;         // false = do not use buffer, true = use buffer for decoding

#ifndef IR_TIME_AVOID_DUPLICATE
#define IR_TIME_AVOID_DUPLICATE 50           // Milliseconds
#endif  // IR_TIME_AVOID_DUPLICATE

#include <IRrecv.h>

IRrecv *irrecv = nullptr;

unsigned long ir_lasttime = 0;

void IrReceiveUpdateThreshold(void)
{
  if (irrecv != nullptr) {
    if (Settings->param[P_IR_UNKNOW_THRESHOLD] < 6) { Settings->param[P_IR_UNKNOW_THRESHOLD] = 6; }
    irrecv->setUnknownThreshold(Settings->param[P_IR_UNKNOW_THRESHOLD]);
  }
}

void IrReceiveUpdateTolerance(void)
{
  if (irrecv != nullptr) {
    if (Settings->param[P_IR_TOLERANCE] == 0) { Settings->param[P_IR_TOLERANCE] = IR_RCV_TOLERANCE; }
    if (Settings->param[P_IR_TOLERANCE] > 100) { Settings->param[P_IR_TOLERANCE] = 100; }
    irrecv->setTolerance(Settings->param[P_IR_TOLERANCE]);
  }
}

void IrReceiveInit(void)
{
  // an IR led is at GPIO_IRRECV
  irrecv = new IRrecv(Pin(GPIO_IRRECV), IR_RCV_BUFFER_SIZE, IR_RCV_TIMEOUT, IR_RCV_SAVE_BUFFER);
  irrecv->setUnknownThreshold(Settings->param[P_IR_UNKNOW_THRESHOLD]);
  IrReceiveUpdateTolerance();
  irrecv->enableIRIn();                  // Start the receiver

  //  AddLog(LOG_LEVEL_DEBUG, PSTR("IrReceive initialized"));
}

void IrReceiveCheck(void)
{
  char sirtype[8];  // Max is UNKNOWN
  int8_t iridx = 0;

  decode_results results;

  if (irrecv->decode(&results)) {
    char hvalue[65];  // Max 256 bits

    iridx = results.decode_type;
    if ((iridx < 0) || (iridx > MAX_STANDARD_IR)) { iridx = 0; }  // UNKNOWN

    if (iridx) {
      if (results.bits > 64) {
        // This emulates IRutils resultToHexidecimal and may needs a larger IR_RCV_BUFFER_SIZE
        uint32_t digits2 = results.bits / 8;
        if (results.bits % 8) { digits2++; }
        ToHex_P((unsigned char*)results.state, digits2, hvalue, sizeof(hvalue));  // Get n-bit value as hex 56341200
      } else {
        Uint64toHex(results.value, hvalue, results.bits);  // Get 64bit value as hex 00123456
      }
    } else {
      Uint64toHex(results.value, hvalue, 32);  // UNKNOWN is always 32 bits hash
    }

    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_IRR "RawLen %d, Overflow %d, Bits %d, Value 0x%s, Decode %d"),
              results.rawlen, results.overflow, results.bits, hvalue, results.decode_type);

    unsigned long now = millis();
//    if ((now - ir_lasttime > IR_TIME_AVOID_DUPLICATE) && (UNKNOWN != results.decode_type) && (results.bits > 0)) {
    if (now - ir_lasttime > IR_TIME_AVOID_DUPLICATE) {
      ir_lasttime = now;

      char svalue[64];
      if (Settings->flag.ir_receive_decimal) {  // SetOption29 - IR receive data format
        ir_ulltoa(results.value, svalue, 10);
      } else {
        snprintf_P(svalue, sizeof(svalue), PSTR("\"0x%s\""), hvalue);
      }
      ResponseTime_P(PSTR(",\"" D_JSON_IRRECEIVED "\":{\"" D_JSON_IR_PROTOCOL "\":\"%s\",\"" D_JSON_IR_BITS "\":%d"),
        GetTextIndexed(sirtype, sizeof(sirtype), iridx, kIrRemoteProtocols), results.bits);
      if (iridx) {
        ResponseAppend_P(PSTR(",\"" D_JSON_IR_DATA "\":%s"), svalue);
      } else {
        ResponseAppend_P(PSTR(",\"" D_JSON_IR_HASH "\":%s"), svalue);
      }

      IRRawTable raw_table;
      bool prev_number = false;     // was the previous value a number, meaning we may need a comma prefix
      bool ir_high = true;          // alternate high/low
      // Add raw data in a compact format
      if (Settings->flag3.receive_raw) {  // SetOption58 - Add IR Raw data to JSON message
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
          if (ResponseLength() + 40 > ResponseSize()) { break; }  // Quit if char string becomes too long
        }
        uint16_t extended_length = getCorrectedRawLength(&results);
        ResponseAppend_P(PSTR("\",\"" D_JSON_IR_RAWDATA "Info\":[%d,%d,%d]"), extended_length, i -1, results.overflow);
      }

      ResponseJsonEndEnd();
      MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_IRRECEIVED));

#ifdef USE_DOMOTICZ
      if (iridx) {
        unsigned long value = results.value | (iridx << 28);  // [Protocol:4, Data:28]
        DomoticzSensor(DZ_COUNT, value);                      // Send data as Domoticz Counter value
      }
#endif  // USE_DOMOTICZ
    }

    irrecv->resume();
  }
}
#endif // USE_IR_RECEIVE

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
  const char *protocol = root.getStr(PSTR(D_JSON_IR_PROTOCOL), "");
  uint16_t bits = root.getUInt(PSTR(D_JSON_IR_BITS), 0);
  uint64_t data = root.getULong(PSTR(D_JSON_IR_DATA), 0);
  uint16_t repeat = root.getUInt(PSTR(D_JSON_IR_REPEAT), 0);

  // check if the IRSend<x> is great than repeat
  if (XdrvMailbox.index > repeat + 1) {
    repeat = XdrvMailbox.index - 1;
  }
  if (!(protocol && bits)) {
    return IE_SYNTAX_IRSEND;
  }

  char protocol_text[20];
  int protocol_code = GetCommandCode(protocol_text, sizeof(protocol_text), protocol, kIrRemoteProtocols);

  // char dvalue[64];
  // char hvalue[20];
  // AddLog(LOG_LEVEL_DEBUG, PSTR("IRS: protocol_text %s, protocol %s, bits %d, data %s (0x%s), repeat %d, protocol_code %d"),
  //   protocol_text, protocol, bits, ulltoa(data, dvalue, 10), Uint64toHex(data, hvalue, bits), repeat, protocol_code);

#ifdef USE_IR_RECEIVE
  if (!IR_RCV_WHILE_SENDING && (irrecv != nullptr)) { irrecv->pause(); }
#endif  // USE_IR_RECEIVE

  switch (protocol_code) {  // Equals IRremoteESP8266.h enum decode_type_t
#ifdef USE_IR_SEND_RC5
    case RC5:
      irsend->sendRC5(data, bits, repeat); break;
#endif
#ifdef USE_IR_SEND_RC6
    case RC6:
      irsend->sendRC6(data, bits, repeat); break;
#endif
#ifdef USE_IR_SEND_NEC
    case NEC:
      irsend->sendNEC(data, (bits > NEC_BITS) ? NEC_BITS : bits, repeat); break;
#endif
    default:
#ifdef USE_IR_RECEIVE
      if (!IR_RCV_WHILE_SENDING && (irrecv != nullptr)) { irrecv->resume(); }
#endif  // USE_IR_RECEIVE
      return IE_PROTO_UNSUPPORTED;
  }
#ifdef USE_IR_RECEIVE
  if (!IR_RCV_WHILE_SENDING && (irrecv != nullptr)) { irrecv->resume(); }
#endif  // USE_IR_RECEIVE

  return IE_NO_ERROR;
}

void CmndIrSend(void)
{
  uint8_t error = IE_SYNTAX_IRSEND;

  if (XdrvMailbox.data_len) {
    if (strchr(XdrvMailbox.data, '{') == nullptr) {
      error = IE_INVALID_JSON;
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
    case IE_PROTO_UNSUPPORTED:
      ResponseCmndChar(D_JSON_PROTOCOL_NOT_SUPPORTED);
      break;
    case IE_SYNTAX_IRSEND:
      Response_P(PSTR("{\"" D_CMND_IRSEND "\":\"" D_JSON_NO " " D_JSON_IR_PROTOCOL ", " D_JSON_IR_BITS " " D_JSON_OR " " D_JSON_IR_DATA "\"}"));
      break;
    default:  // IE_NO_ERROR
      ResponseCmndDone();
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv05(uint32_t function)
{
  bool result = false;

  if (PinUsed(GPIO_IRSEND) || PinUsed(GPIO_IRRECV)) {
    switch (function) {
      case FUNC_PRE_INIT:
        if (PinUsed(GPIO_IRSEND)) {
          IrSendInit();
        }
#ifdef USE_IR_RECEIVE
        if (PinUsed(GPIO_IRRECV)) {
          IrReceiveInit();
        }
#endif  // USE_IR_RECEIVE
        break;
      case FUNC_EVERY_50_MSECOND:
#ifdef USE_IR_RECEIVE
        if (PinUsed(GPIO_IRRECV)) {
          IrReceiveCheck();  // check if there's anything on IR side
        }
#endif  // USE_IR_RECEIVE
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

#endif // defined(USE_IR_REMOTE) && !defined(USE_IR_REMOTE_FULL)
