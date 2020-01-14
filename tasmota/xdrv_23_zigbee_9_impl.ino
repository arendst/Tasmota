/*
  xdrv_23_zigbee.ino - zigbee support for Tasmota

  Copyright (C) 2020  Theo Arends and Stephan Hadinger

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

#ifdef USE_ZIGBEE

#define XDRV_23                    23

const uint32_t ZIGBEE_BUFFER_SIZE = 256;  // Max ZNP frame is SOF+LEN+CMD1+CMD2+250+FCS = 255
const uint8_t  ZIGBEE_SOF = 0xFE;
const uint8_t  ZIGBEE_SOF_ALT = 0xFF;

#include <TasmotaSerial.h>
TasmotaSerial *ZigbeeSerial = nullptr;

const char kZigbeeCommands[] PROGMEM = "|"
  D_CMND_ZIGBEEZNPSEND "|" D_CMND_ZIGBEE_PERMITJOIN "|"
  D_CMND_ZIGBEE_STATUS "|" D_CMND_ZIGBEE_RESET "|" D_CMND_ZIGBEE_SEND "|"
  D_CMND_ZIGBEE_PROBE "|" D_CMND_ZIGBEE_READ "|" D_CMND_ZIGBEEZNPRECEIVE
  ;

void (* const ZigbeeCommand[])(void) PROGMEM = {
  &CmndZigbeeZNPSend, &CmndZigbeePermitJoin,
  &CmndZigbeeStatus, &CmndZigbeeReset, &CmndZigbeeSend,
  &CmndZigbeeProbe, &CmndZigbeeRead, &CmndZigbeeZNPReceive
  };

int32_t ZigbeeProcessInput(class SBuffer &buf) {
  if (!zigbee.state_machine) { return -1; }     // if state machine is stopped, send 'ignore' message

  // apply the receive filter, acts as 'startsWith()'
  bool recv_filter_match = true;
  bool recv_prefix_match = false;      // do the first 2 bytes match the response
  if ((zigbee.recv_filter) && (zigbee.recv_filter_len > 0)) {
    if (zigbee.recv_filter_len >= 2) {
      recv_prefix_match = false;
      if ( (pgm_read_byte(&zigbee.recv_filter[0]) == buf.get8(0)) &&
           (pgm_read_byte(&zigbee.recv_filter[1]) == buf.get8(1)) ) {
        recv_prefix_match = true;
      }
    }

    for (uint32_t i = 0; i < zigbee.recv_filter_len; i++) {
      if (pgm_read_byte(&zigbee.recv_filter[i]) != buf.get8(i)) {
        recv_filter_match = false;
        break;
      }
    }

    AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_ZIGBEE "ZigbeeProcessInput: recv_prefix_match = %d, recv_filter_match = %d"), recv_prefix_match, recv_filter_match);
  }

  // if there is a recv_callback, call it now
  int32_t res = -1;         // default to ok
                            // res  =  0   - proceed to next state
                            // res  >  0   - proceed to the specified state
                            // res  = -1  - silently ignore the message
                            // res <= -2 - move to error state
  // pre-compute the suggested value
  if ((zigbee.recv_filter) && (zigbee.recv_filter_len > 0)) {
    if (!recv_prefix_match) {
      res = -1;    // ignore
    } else {  // recv_prefix_match
      if (recv_filter_match) {
        res = 0;     // ok
      } else {
        if (zigbee.recv_until) {
          res = -1;  // ignore until full match
        } else {
          res = -2;  // error, because message is expected but wrong value
        }
      }
    }
  } else {    // we don't have any filter, ignore message by default
    res = -1;
  }

  if (recv_prefix_match) {
    if (zigbee.recv_func) {
      res = (*zigbee.recv_func)(res, buf);
    }
  }
  if (-1 == res) {
    // if frame was ignored up to now
    if (zigbee.recv_unexpected) {
      res = (*zigbee.recv_unexpected)(res, buf);
    }
  }
  AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_ZIGBEE "ZigbeeProcessInput: res = %d"), res);

  // change state accordingly
  if (0 == res) {
    // if ok, continue execution
    zigbee.state_waiting = false;
  } else if (res > 0) {
    ZigbeeGotoLabel(res);     // if >0 then go to specified label
  } else if (-1 == res) {
    // -1 means ignore message
    // just do nothing
  } else {
    // any other negative value means error
    ZigbeeGotoLabel(zigbee.on_error_goto);
  }
}

void ZigbeeInput(void)
{
	static uint32_t zigbee_polling_window = 0;
	static uint8_t fcs = ZIGBEE_SOF;
	static uint32_t zigbee_frame_len = 5;		// minimal zigbee frame lenght, will be updated when buf[1] is read
  // Receive only valid ZNP frames:
  // 00 - SOF = 0xFE
  // 01 - Length of Data Field - 0..250
  // 02 - CMD1 - first byte of command
  // 03 - CMD2 - second byte of command
  // 04..FD - Data Field
  // FE (or last) - FCS Checksum

  while (ZigbeeSerial->available()) {
    yield();
    uint8_t zigbee_in_byte = ZigbeeSerial->read();
		AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("ZigbeeInput byte=%d len=%d"), zigbee_in_byte, zigbee_buffer->len());

		if (0 == zigbee_buffer->len()) {  // make sure all variables are correctly initialized
			zigbee_frame_len = 5;
			fcs = ZIGBEE_SOF;
      // there is a rare race condition when an interrupt occurs when receiving the first byte
      // in this case the first bit (lsb) is missed and Tasmota receives 0xFF instead of 0xFE
      // We forgive this mistake, and next bytes are automatically resynchronized
      if (ZIGBEE_SOF_ALT == zigbee_in_byte) {
        AddLog_P2(LOG_LEVEL_INFO, PSTR("ZigbeeInput forgiven first byte %02X (only for statistics)"), zigbee_in_byte);
        zigbee_in_byte = ZIGBEE_SOF;
      }
		}

    if ((0 == zigbee_buffer->len()) && (ZIGBEE_SOF != zigbee_in_byte)) {
      // waiting for SOF (Start Of Frame) byte, discard anything else
      AddLog_P2(LOG_LEVEL_INFO, PSTR("ZigbeeInput discarding byte %02X"), zigbee_in_byte);
      continue;     // discard
    }

    if (zigbee_buffer->len() < zigbee_frame_len) {
			zigbee_buffer->add8(zigbee_in_byte);
      zigbee_polling_window = millis();                               // Wait for more data
			fcs ^= zigbee_in_byte;
    }

		if (zigbee_buffer->len() >= zigbee_frame_len) {
      zigbee_polling_window = 0;                                      // Publish now
      break;
    }

    // recalculate frame length
    if (02 == zigbee_buffer->len()) {
      // We just received the Lenght byte
      uint8_t len_byte = zigbee_buffer->get8(1);
      if (len_byte > 250)  len_byte = 250;    // ZNP spec says len is 250 max

      zigbee_frame_len = len_byte + 5;        // SOF + LEN + CMD1 + CMD2 + FCS = 5 bytes overhead
    }
  }

  if (zigbee_buffer->len() && (millis() > (zigbee_polling_window + ZIGBEE_POLLING))) {
    char hex_char[(zigbee_buffer->len() * 2) + 2];
		ToHex_P((unsigned char*)zigbee_buffer->getBuffer(), zigbee_buffer->len(), hex_char, sizeof(hex_char));

    AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_ZIGBEE "Bytes follow_read_metric = %0d"), ZigbeeSerial->getLoopReadMetric());
		// buffer received, now check integrity
		if (zigbee_buffer->len() != zigbee_frame_len) {
			// Len is not correct, log and reject frame
      AddLog_P2(LOG_LEVEL_INFO, PSTR(D_JSON_ZIGBEEZNPRECEIVED ": received frame of wrong size %s, len %d, expected %d"), hex_char, zigbee_buffer->len(), zigbee_frame_len);
		} else if (0x00 != fcs) {
			// FCS is wrong, packet is corrupt, log and reject frame
      AddLog_P2(LOG_LEVEL_INFO, PSTR(D_JSON_ZIGBEEZNPRECEIVED ": received bad FCS frame %s, %d"), hex_char, fcs);
		} else {
			// frame is correct
			//AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR(D_JSON_ZIGBEEZNPRECEIVED ": received correct frame %s"), hex_char);

			SBuffer znp_buffer = zigbee_buffer->subBuffer(2, zigbee_frame_len - 3);	// remove SOF, LEN and FCS

			ToHex_P((unsigned char*)znp_buffer.getBuffer(), znp_buffer.len(), hex_char, sizeof(hex_char));
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_ZIGBEE D_JSON_ZIGBEEZNPRECEIVED " %s"),
                                 hex_char);

			// now process the message
      ZigbeeProcessInput(znp_buffer);
		}
		zigbee_buffer->setLen(0);		// empty buffer
  }
}

/********************************************************************************************/

void ZigbeeInit(void)
{
  zigbee.active = false;
  if ((pin[GPIO_ZIGBEE_RX] < 99) && (pin[GPIO_ZIGBEE_TX] < 99)) {
		AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("Zigbee: GPIOs Rx:%d Tx:%d"), pin[GPIO_ZIGBEE_RX], pin[GPIO_ZIGBEE_TX]);
    // if seriallog_level is 0, we allow GPIO 13/15 to switch to Hardware Serial
    ZigbeeSerial = new TasmotaSerial(pin[GPIO_ZIGBEE_RX], pin[GPIO_ZIGBEE_TX], seriallog_level ? 1 : 2, 0, 256);   // set a receive buffer of 256 bytes
    ZigbeeSerial->begin(115200);
    if (ZigbeeSerial->hardwareSerial()) {
      ClaimSerial();
      uint32_t aligned_buffer = ((uint32_t)serial_in_buffer + 3) & ~3;
			zigbee_buffer = new PreAllocatedSBuffer(sizeof(serial_in_buffer) - 3, (char*) aligned_buffer);
		} else {
			zigbee_buffer = new SBuffer(ZIGBEE_BUFFER_SIZE);
		}
    zigbee.active = true;
		zigbee.init_phase = true;			// start the state machine
    zigbee.state_machine = true;      // start the state machine
    ZigbeeSerial->flush();
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

uint32_t strToUInt(const JsonVariant &val) {
  // if the string starts with 0x, it is considered Hex, otherwise it is an int
  if (val.is<unsigned int>()) {
    return val.as<unsigned int>();
  } else {
    if (val.is<const char*>()) {
      String sval = val.as<String>();
      return strtoull(sval.c_str(), nullptr, 0);
    }
  }
  return 0;   // couldn't parse anything
}

const unsigned char ZIGBEE_FACTORY_RESET[] PROGMEM =
  { Z_SREQ | Z_SAPI, SAPI_WRITE_CONFIGURATION, CONF_STARTUP_OPTION, 0x01 /* len */, 0x01 /* STARTOPT_CLEAR_CONFIG */};
//"2605030101";  // Z_SREQ | Z_SAPI, SAPI_WRITE_CONFIGURATION, CONF_STARTUP_OPTION, 0x01 len, 0x01 STARTOPT_CLEAR_CONFIG
// Do a factory reset of the CC2530
void CmndZigbeeReset(void) {
  if (ZigbeeSerial) {
    switch (XdrvMailbox.payload) {
    case 1:
      ZigbeeZNPSend(ZIGBEE_FACTORY_RESET, sizeof(ZIGBEE_FACTORY_RESET));
      restart_flag = 2;
      ResponseCmndChar(D_JSON_ZIGBEE_CC2530 " " D_JSON_RESET_AND_RESTARTING);
      break;
    default:
      ResponseCmndChar(D_JSON_ONE_TO_RESET);
    }
  }
}

void CmndZigbeeStatus(void) {
  if (ZigbeeSerial) {
    String dump = zigbee_devices.dump(XdrvMailbox.index, XdrvMailbox.payload);
    Response_P(PSTR("{\"%s%d\":%s}"), XdrvMailbox.command, XdrvMailbox.index, dump.c_str());
  }
}

void CmndZigbeeZNPSendOrReceive(bool send)
{
  if (ZigbeeSerial && (XdrvMailbox.data_len > 0)) {
    uint8_t code;

    char *codes = RemoveSpace(XdrvMailbox.data);
    int32_t size = strlen(XdrvMailbox.data);

		SBuffer buf((size+1)/2);

    while (size > 1) {
      char stemp[3];
      strlcpy(stemp, codes, sizeof(stemp));
      code = strtol(stemp, nullptr, 16);
			buf.add8(code);
      size -= 2;
      codes += 2;
    }
    if (send) {
      ZigbeeZNPSend(buf.getBuffer(), buf.len());
    } else {
      ZigbeeProcessInput(buf);
    }
  }
  ResponseCmndDone();
}

// For debug purposes only, simulates a message received
void CmndZigbeeZNPReceive(void)
{
  CmndZigbeeZNPSendOrReceive(false);
}

void CmndZigbeeZNPSend(void)
{
  CmndZigbeeZNPSendOrReceive(true);
}

void ZigbeeZNPSend(const uint8_t *msg, size_t len) {
	if ((len < 2) || (len > 252)) {
		// abort, message cannot be less than 2 bytes for CMD1 and CMD2
		AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_JSON_ZIGBEEZNPSENT ": bad message len %d"), len);
		return;
	}
	uint8_t data_len = len - 2;		// removing CMD1 and CMD2

  if (ZigbeeSerial) {
		uint8_t fcs = data_len;

		ZigbeeSerial->write(ZIGBEE_SOF);		// 0xFE
		AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("ZNPSend SOF %02X"), ZIGBEE_SOF);
		ZigbeeSerial->write(data_len);
		AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("ZNPSend LEN %02X"), data_len);
		for (uint32_t i = 0; i < len; i++) {
			uint8_t b = pgm_read_byte(msg + i);
			ZigbeeSerial->write(b);
			fcs ^= b;
			AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("ZNPSend byt %02X"), b);
		}
		ZigbeeSerial->write(fcs);			// finally send fcs checksum byte
		AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("ZNPSend FCS %02X"), fcs);
  }
	// Now send a MQTT message to report the sent message
	char hex_char[(len * 2) + 2];
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_ZIGBEE D_JSON_ZIGBEEZNPSENT " %s"),
                               		ToHex_P(msg, len, hex_char, sizeof(hex_char)));
}

void ZigbeeZCLSend(uint16_t dtsAddr, uint16_t clusterId, uint8_t endpoint, uint8_t cmdId, bool clusterSpecific, const uint8_t *msg, size_t len, bool disableDefResp, uint8_t transacId) {
  SBuffer buf(25+len);
  buf.add8(Z_SREQ | Z_AF);        // 24
  buf.add8(AF_DATA_REQUEST);      // 01
  buf.add16(dtsAddr);
  buf.add8(endpoint);             // dest endpoint
  buf.add8(0x01);                 // source endpoint
  buf.add16(clusterId);
  buf.add8(transacId);                 // transacId
  buf.add8(0x30);                 // 30 options
  buf.add8(0x1E);                 // 1E radius

  buf.add8(3 + len);
  buf.add8((disableDefResp ? 0x10 : 0x00) | (clusterSpecific ? 0x01 : 0x00));                 // Frame Control Field
  buf.add8(transacId);            // Transaction Sequance Number
  buf.add8(cmdId);
  if (len > 0) {
    buf.addBuffer(msg, len);             // add the payload
  }

  ZigbeeZNPSend(buf.getBuffer(), buf.len());
}

inline int8_t hexValue(char c) {
  if ((c >= '0') && (c <= '9')) {
    return c - '0';
  }
  if ((c >= 'A') && (c <= 'F')) {
    return 10 + c - 'A';
  }
  if ((c >= 'a') && (c <= 'f')) {
    return 10 + c - 'a';
  }
  return -1;
}

uint32_t parseHex(const char **data, size_t max_len = 8) {
  uint32_t ret = 0;
  for (uint32_t i = 0; i < max_len; i++) {
    int8_t v = hexValue(**data);
    if (v < 0) { break; }     // non hex digit, we stop parsing
    ret = (ret << 4) | v;
    *data += 1;
  }
  return ret;
}

void zigbeeZCLSendStr(uint16_t dstAddr, uint8_t endpoint, const char *data) {

  uint16_t cluster = 0x0000;    // 0x0000 is a valid default value
  uint8_t  cmd = ZCL_READ_ATTRIBUTES; // default command is READ_ATTRIBUTES
  bool     clusterSpecific = false;
  // Parse 'cmd' in the form "AAAA_BB/CCCCCCCC" or "AAAA!BB/CCCCCCCC"
  // where AA is the cluster number, BBBB the command number, CCCC... the payload
  // First delimiter is '_' for a global command, or '!' for a cluster specific commanc
  cluster = parseHex(&data, 4);

  // delimiter
  if (('_' == *data) || ('!' == *data)) {
    if ('!' == *data) { clusterSpecific = true; }
    data++;
  } else {
    ResponseCmndChar("Wrong delimiter for payload");
    return;
  }
  // parse cmd number
  cmd = parseHex(&data, 2);

  // move to end of payload
  // delimiter is optional
  if ('/' == *data) { data++; }   // skip delimiter

  size_t size = strlen(data);
  SBuffer buf((size+2)/2);    // actual bytes buffer for data

  while (*data) {
    uint8_t code = parseHex(&data, 2);
    buf.add8(code);
  }

  if (0 == endpoint) {
    // endpoint is not specified, let's try to find it from shortAddr
    endpoint = zigbee_devices.findClusterEndpointIn(dstAddr, cluster);
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ZigbeeSend: guessing endpoint 0x%02X"), endpoint);
  }
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ZigbeeSend: dstAddr 0x%04X, cluster 0x%04X, endpoint 0x%02X, cmd 0x%02X, data %s"),
    dstAddr, cluster, endpoint, cmd, data);

  if (0 == endpoint) {
    AddLog_P2(LOG_LEVEL_INFO, PSTR("ZigbeeSend: unspecified endpoint"));
    return;
  }

  // everything is good, we can send the command
  ZigbeeZCLSend(dstAddr, cluster, endpoint, cmd, clusterSpecific, buf.getBuffer(), buf.len());
  // now set the timer, if any, to read back the state later
  if (clusterSpecific) {
    zigbeeSetCommandTimer(dstAddr, cluster, endpoint);
  }
  ResponseCmndDone();
}

void CmndZigbeeSend(void) {
  // ZigbeeSend { "device":"0x1234", "endpoint":"0x03", "send":{"Power":1} }
  // ZigbeeSend { "device":"0x1234", "endpoint":"0x03", "send":{"Power":"3"} }
  // ZigbeeSend { "device":"0x1234", "endpoint":"0x03", "send":{"Power":"0xFF"} }
  // ZigbeeSend { "device":"0x1234", "endpoint":"0x03", "send":{"Power":null} }
  // ZigbeeSend { "device":"0x1234", "endpoint":"0x03", "send":{"Power":false} }
  // ZigbeeSend { "device":"0x1234", "endpoint":"0x03", "send":{"Power":true} }
  // ZigbeeSend { "device":"0x1234", "endpoint":"0x03", "send":{"Power":"true"} }
  // ZigbeeSend { "device":"0x1234", "endpoint":"0x03", "send":{"ShutterClose":null} }
  // ZigbeeSend { "devicse":"0x1234", "endpoint":"0x03", "send":{"Power":1} }
  // ZigbeeSend { "device":"0x1234", "endpoint":"0x03", "send":{"Color":"1,2"} }
  // ZigbeeSend { "device":"0x1234", "endpoint":"0x03", "send":{"Color":"0x1122,0xFFEE"} }
  if (zigbee.init_phase) { ResponseCmndChar(D_ZIGBEE_NOT_STARTED); return; }
  DynamicJsonBuffer jsonBuf;
  JsonObject &json = jsonBuf.parseObject(XdrvMailbox.data);
  if (!json.success()) { ResponseCmndChar(D_JSON_INVALID_JSON); return; }

  // params
  static char delim[] = ", ";     // delimiters for parameters
  uint16_t device = 0xFFFF;       // 0xFFFF is broadcast, so considered valid
  uint8_t  endpoint = 0x00;       // 0x00 is invalid for the dst endpoint
  String   cmd_str = "";          // the actual low-level command, either specified or computed

  const JsonVariant &val_device = getCaseInsensitive(json, PSTR("device"));
  if (nullptr != &val_device) { device = strToUInt(val_device); }
  const JsonVariant &val_endpoint = getCaseInsensitive(json, PSTR("endpoint"));
  if (nullptr != &val_endpoint) { endpoint = strToUInt(val_endpoint); }
  const JsonVariant val_cmd = getCaseInsensitive(json, PSTR("Send"));
  if (nullptr != &val_cmd) {
    // probe the type of the argument
    // If JSON object, it's high level commands
    // If String, it's a low level command
    if (val_cmd.is<JsonObject>()) {
      // we have a high-level command
      JsonObject &cmd_obj = val_cmd.as<JsonObject&>();
      int32_t cmd_size = cmd_obj.size();
      if (cmd_size > 1) {
        Response_P(PSTR("Only 1 command allowed (%d)"), cmd_size);
        return;
      } else if (1 == cmd_size) {
        // We have exactly 1 command, parse it
        JsonObject::iterator it = cmd_obj.begin();    // just get the first key/value
        String key = it->key;
        JsonVariant& value = it->value;
        uint32_t x = 0, y = 0, z = 0;

        const __FlashStringHelper* tasmota_cmd = zigbeeFindCommand(key.c_str());
        if (tasmota_cmd) {
          cmd_str = tasmota_cmd;
        } else {
          Response_P(PSTR("Unrecognized zigbee command: %s"), key.c_str());
          return;
        }

        // parse the JSON value, depending on its type fill in x,y,z
        if (value.is<bool>()) {
          x = value.as<bool>() ? 1 : 0;
        } else if (value.is<unsigned int>()) {
          x = value.as<unsigned int>();
        } else {
          // if non-bool or non-int, trying char*
          const char *s_const = value.as<const char*>();
          if (s_const != nullptr) {
            char s[strlen(s_const)+1];
            strcpy(s, s_const);
            if ((nullptr != s) && (0x00 != *s)) {     // ignore any null or empty string, could represent 'null' json value
              char *sval = strtok(s, delim);
              if (sval) {
                x = ZigbeeAliasOrNumber(sval);
                sval = strtok(nullptr, delim);
                if (sval) {
                  y = ZigbeeAliasOrNumber(sval);
                  sval = strtok(nullptr, delim);
                  if (sval) {
                    z = ZigbeeAliasOrNumber(sval);
                  }
                }
              }
            }
          }
        }

        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ZigbeeSend: command_template = %s"), cmd_str.c_str());
        cmd_str = zigbeeCmdAddParams(cmd_str.c_str(), x, y, z);   // fill in parameters
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ZigbeeSend: command_final    = %s"), cmd_str.c_str());
      } else {
        // we have zero command, pass through until last error for missing command
      }
    } else if (val_cmd.is<char*>()) {
      // low-level command
      cmd_str = val_cmd.as<String>();
    } else {
      // we have an unsupported command type, just ignore it and fallback to missing command
    }

    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ZigbeeCmd_actual: ZigbeeZCLSend {\"device\":\"0x%04X\",\"endpoint\":%d,\"send\":\"%s\"}"),
              device, endpoint, cmd_str.c_str());
    zigbeeZCLSendStr(device, endpoint, cmd_str.c_str());
  } else {
    Response_P(PSTR("Missing zigbee 'Send'"));
    return;
  }

}

// Probe a specific device to get its endpoints and supported clusters
void CmndZigbeeProbe(void) {
  if (zigbee.init_phase) { ResponseCmndChar(D_ZIGBEE_NOT_STARTED); return; }
  char dataBufUc[XdrvMailbox.data_len + 1];
  UpperCase(dataBufUc, XdrvMailbox.data);
  RemoveSpace(dataBufUc);
  if (strlen(dataBufUc) < 3) { ResponseCmndChar("Invalid destination"); return; }

  // TODO, for now ignore friendly names
  uint16_t shortaddr = strtoull(dataBufUc, nullptr, 0);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("CmndZigbeeScan: short addr 0x%04X"), shortaddr);

  // everything is good, we can send the command
  Z_SendActiveEpReq(shortaddr);
  ResponseCmndDone();
}

// Send an attribute read command to a device, specifying cluster and list of attributes
void CmndZigbeeRead(void) {
  // ZigbeeRead {"Device":"0xF289","Cluster":0,"Endpoint":3,"Attr":5}
  // ZigbeeRead {"Device":"0xF289","Cluster":"0x0000","Endpoint":"0x0003","Attr":"0x0005"}
  // ZigbeeRead {"Device":"0xF289","Cluster":0,"Endpoint":3,"Attr":[5,6,7,4]}
  if (zigbee.init_phase) { ResponseCmndChar(D_ZIGBEE_NOT_STARTED); return; }
  DynamicJsonBuffer jsonBuf;
  JsonObject &json = jsonBuf.parseObject(XdrvMailbox.data);
  if (!json.success()) { ResponseCmndChar(D_JSON_INVALID_JSON); return; }

  // params
  uint16_t device = 0xFFFF;       // 0xFFFF is braodcast, so considered valid
  uint16_t cluster = 0x0000;      // default to general cluster
  uint8_t  endpoint = 0x00;       // 0x00 is invalid for the dst endpoint
  size_t   attrs_len = 0;
  uint8_t* attrs = nullptr;       // empty string is valid


  const JsonVariant &val_device = getCaseInsensitive(json, PSTR("Device"));
  if (nullptr != &val_device) { device = strToUInt(val_device); }
  const JsonVariant val_cluster = getCaseInsensitive(json, PSTR("Cluster"));
  if (nullptr != &val_cluster) { cluster = strToUInt(val_cluster); }
  const JsonVariant &val_endpoint = getCaseInsensitive(json, PSTR("Endpoint"));
  if (nullptr != &val_endpoint) { endpoint = strToUInt(val_endpoint); }

  const JsonVariant &val_attr = getCaseInsensitive(json, PSTR("Read"));
  if (nullptr != &val_attr) {
    uint16_t val = strToUInt(val_attr);
    if (val_attr.is<JsonArray>()) {
      JsonArray& attr_arr = val_attr;
      attrs_len = attr_arr.size() * 2;
      attrs = new uint8_t[attrs_len];

      uint32_t i = 0;
      for (auto value : attr_arr) {
        uint16_t val = strToUInt(value);
        attrs[i++] = val & 0xFF;
        attrs[i++] = val >> 8;
      }
    } else {
      attrs_len = 2;
      attrs = new uint8_t[attrs_len];
      attrs[0] = val & 0xFF;    // little endian
      attrs[1] = val >> 8;
    }
  }

  ZigbeeZCLSend(device, cluster, endpoint, ZCL_READ_ATTRIBUTES, false, attrs, attrs_len, false /* we do want a response */);

  if (attrs) { delete[] attrs; }
  ResponseCmndDone();
}

// Allow or Deny pairing of new Zigbee devices
void CmndZigbeePermitJoin(void)
{
  if (zigbee.init_phase) { ResponseCmndChar(D_ZIGBEE_NOT_STARTED); return; }
  uint32_t payload = XdrvMailbox.payload;
  if (payload < 0) { payload = 0; }
  if ((99 != payload) && (payload > 1)) { payload = 1; }

  if (1 == payload) {
    ZigbeeGotoLabel(ZIGBEE_LABEL_PERMIT_JOIN_OPEN_60);
  } else if (99 == payload){
    ZigbeeGotoLabel(ZIGBEE_LABEL_PERMIT_JOIN_OPEN_XX);
  } else {
    ZigbeeGotoLabel(ZIGBEE_LABEL_PERMIT_JOIN_CLOSE);
  }
  ResponseCmndDone();
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv23(uint8_t function)
{
  bool result = false;

  if (zigbee.active) {
    switch (function) {
      case FUNC_EVERY_50_MSECOND:
        if (!zigbee.init_phase) {
          zigbee_devices.runTimer();
        }
        break;
      case FUNC_LOOP:
        if (ZigbeeSerial) { ZigbeeInput(); }
				if (zigbee.state_machine) {
					//ZigbeeStateMachine();
          ZigbeeStateMachine_Run();
				}
        break;
      case FUNC_PRE_INIT:
        ZigbeeInit();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kZigbeeCommands, ZigbeeCommand);
        break;
    }
  }
  return result;
}

#endif // USE_ZIGBEE
