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


const char kZbCommands[] PROGMEM = D_PRFX_ZB "|"    // prefix
  D_CMND_ZIGBEEZNPSEND "|" D_CMND_ZIGBEE_PERMITJOIN "|"
  D_CMND_ZIGBEE_STATUS "|" D_CMND_ZIGBEE_RESET "|" D_CMND_ZIGBEE_SEND "|"
  D_CMND_ZIGBEE_PROBE "|" D_CMND_ZIGBEEZNPRECEIVE "|"
  D_CMND_ZIGBEE_FORGET "|" D_CMND_ZIGBEE_SAVE "|" D_CMND_ZIGBEE_NAME "|"
  D_CMND_ZIGBEE_BIND "|" D_CMND_ZIGBEE_UNBIND "|" D_CMND_ZIGBEE_PING "|" D_CMND_ZIGBEE_MODELID "|"
  D_CMND_ZIGBEE_LIGHT "|" D_CMND_ZIGBEE_RESTORE "|" D_CMND_ZIGBEE_BIND_STATE "|"
  D_CMND_ZIGBEE_CONFIG
  ;

void (* const ZigbeeCommand[])(void) PROGMEM = {
  &CmndZbZNPSend, &CmndZbPermitJoin,
  &CmndZbStatus, &CmndZbReset, &CmndZbSend,
  &CmndZbProbe, &CmndZbZNPReceive,
  &CmndZbForget, &CmndZbSave, &CmndZbName,
  &CmndZbBind, &CmndZbUnbind, &CmndZbPing, &CmndZbModelId,
  &CmndZbLight, &CmndZbRestore, &CmndZbBindState,
  &CmndZbConfig,
  };

//
// Called at event loop, checks for incoming data from the CC2530
//
void ZigbeeInputLoop(void)
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
		//AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("ZbInput byte=%d len=%d"), zigbee_in_byte, zigbee_buffer->len());

		if (0 == zigbee_buffer->len()) {  // make sure all variables are correctly initialized
			zigbee_frame_len = 5;
			fcs = ZIGBEE_SOF;
      // there is a rare race condition when an interrupt occurs when receiving the first byte
      // in this case the first bit (lsb) is missed and Tasmota receives 0xFF instead of 0xFE
      // We forgive this mistake, and next bytes are automatically resynchronized
      if (ZIGBEE_SOF_ALT == zigbee_in_byte) {
        AddLog_P2(LOG_LEVEL_INFO, PSTR("ZbInput forgiven first byte %02X (only for statistics)"), zigbee_in_byte);
        zigbee_in_byte = ZIGBEE_SOF;
      }
		}

    if ((0 == zigbee_buffer->len()) && (ZIGBEE_SOF != zigbee_in_byte)) {
      // waiting for SOF (Start Of Frame) byte, discard anything else
      AddLog_P2(LOG_LEVEL_INFO, PSTR("ZbInput discarding byte %02X"), zigbee_in_byte);
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
      Response_P(PSTR("{\"" D_JSON_ZIGBEEZNPRECEIVED "\":\"%s\"}"), hex_char);
      if (Settings.flag3.tuya_serial_mqtt_publish) {
        MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR));
        XdrvRulesProcess();
      } else {
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_ZIGBEE "%s"), mqtt_data);
      }
			// now process the message
      ZigbeeProcessInput(znp_buffer);
		}
		zigbee_buffer->setLen(0);		// empty buffer
  }
}

/********************************************************************************************/

// Initialize internal structures
void ZigbeeInit(void)
{
  // Check if settings in Flash are set
  if (0 == Settings.zb_channel) {
    AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "Initializing Zigbee parameters from defaults"));
    Settings.zb_ext_panid = USE_ZIGBEE_EXTPANID;
    Settings.zb_precfgkey_l = USE_ZIGBEE_PRECFGKEY_L;
    Settings.zb_precfgkey_h = USE_ZIGBEE_PRECFGKEY_H;
    Settings.zb_pan_id = USE_ZIGBEE_PANID;
    Settings.zb_channel = USE_ZIGBEE_CHANNEL;
    Settings.zb_free_byte = 0;
  }
  // update commands with the current settings
  Z_UpdateConfig(Settings.zb_channel, Settings.zb_pan_id, Settings.zb_ext_panid, Settings.zb_precfgkey_l, Settings.zb_precfgkey_h);

// AddLog_P2(LOG_LEVEL_INFO, PSTR("ZigbeeInit Mem1 = %d"), ESP_getFreeHeap());
  zigbee.active = false;
  if (PinUsed(GPIO_ZIGBEE_RX) && PinUsed(GPIO_ZIGBEE_TX)) {
		AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_ZIGBEE "GPIOs Rx:%d Tx:%d"), Pin(GPIO_ZIGBEE_RX), Pin(GPIO_ZIGBEE_TX));
    // if seriallog_level is 0, we allow GPIO 13/15 to switch to Hardware Serial
    ZigbeeSerial = new TasmotaSerial(Pin(GPIO_ZIGBEE_RX), Pin(GPIO_ZIGBEE_TX), seriallog_level ? 1 : 2, 0, 256);   // set a receive buffer of 256 bytes
    ZigbeeSerial->begin(115200);
    if (ZigbeeSerial->hardwareSerial()) {
      ClaimSerial();
      uint32_t aligned_buffer = ((uint32_t)serial_in_buffer + 3) & ~3;
			zigbee_buffer = new PreAllocatedSBuffer(sizeof(serial_in_buffer) - 3, (char*) aligned_buffer);
		} else {
// AddLog_P2(LOG_LEVEL_INFO, PSTR("ZigbeeInit Mem2 = %d"), ESP_getFreeHeap());
			zigbee_buffer = new SBuffer(ZIGBEE_BUFFER_SIZE);
// AddLog_P2(LOG_LEVEL_INFO, PSTR("ZigbeeInit Mem3 = %d"), ESP_getFreeHeap());
		}
    zigbee.active = true;
		zigbee.init_phase = true;			// start the state machine
    zigbee.state_machine = true;      // start the state machine
    ZigbeeSerial->flush();
  }
// AddLog_P2(LOG_LEVEL_INFO, PSTR("ZigbeeInit Mem9 = %d"), ESP_getFreeHeap());
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

// Do a factory reset of the CC2530
const unsigned char ZIGBEE_FACTORY_RESET[] PROGMEM =
  { Z_SREQ | Z_SAPI, SAPI_WRITE_CONFIGURATION, CONF_STARTUP_OPTION, 0x01 /* len */, 0x01 /* STARTOPT_CLEAR_CONFIG */};
//"2605030101";  // Z_SREQ | Z_SAPI, SAPI_WRITE_CONFIGURATION, CONF_STARTUP_OPTION, 0x01 len, 0x01 STARTOPT_CLEAR_CONFIG
void CmndZbReset(void) {
  if (ZigbeeSerial) {
    switch (XdrvMailbox.payload) {
    case 1:
      ZigbeeZNPSend(ZIGBEE_FACTORY_RESET, sizeof(ZIGBEE_FACTORY_RESET));
      eraseZigbeeDevices();
      restart_flag = 2;
      ResponseCmndChar_P(PSTR(D_JSON_ZIGBEE_CC2530 " " D_JSON_RESET_AND_RESTARTING));
      break;
    default:
      ResponseCmndChar_P(PSTR(D_JSON_ONE_TO_RESET));
    }
  }
}

//
// Same code for `ZbZNPSend` and `ZbZNPReceive`
// building the complete message (intro, length)
//
void CmndZbZNPSendOrReceive(bool send)
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
      // Command was `ZbZNPSend`
      ZigbeeZNPSend(buf.getBuffer(), buf.len());
    } else {
      // Command was `ZbZNPReceive`
      ZigbeeProcessInput(buf);
    }
  }
  ResponseCmndDone();
}

// For debug purposes only, simulates a message received
void CmndZbZNPReceive(void)
{
  CmndZbZNPSendOrReceive(false);
}

void CmndZbZNPSend(void)
{
  CmndZbZNPSendOrReceive(true);
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
		//AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("ZNPSend SOF %02X"), ZIGBEE_SOF);
		ZigbeeSerial->write(data_len);
		//AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("ZNPSend LEN %02X"), data_len);
		for (uint32_t i = 0; i < len; i++) {
			uint8_t b = pgm_read_byte(msg + i);
			ZigbeeSerial->write(b);
			fcs ^= b;
			//AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("ZNPSend byt %02X"), b);
		}
		ZigbeeSerial->write(fcs);			// finally send fcs checksum byte
		//AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("ZNPSend FCS %02X"), fcs);
  }
	// Now send a MQTT message to report the sent message
	char hex_char[(len * 2) + 2];
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_ZIGBEE D_JSON_ZIGBEEZNPSENT " %s"),
                               		ToHex_P(msg, len, hex_char, sizeof(hex_char)));
}

//
// Internal function, send the low-level frame
// Input:
// - shortaddr: 16-bits short address, or 0x0000 if group address
// - groupaddr: 16-bits group address, or 0x0000 if unicast using shortaddr
// - clusterIf: 16-bits cluster number
// - endpoint:  8-bits target endpoint (source is always 0x01), unused for group addresses. Should not be 0x00 except when sending to group address.
// - cmdId:     8-bits ZCL command number
// - clusterSpecific: boolean, is the message general cluster or cluster specific, used to create the FC byte of ZCL
// - msg:       pointer to byte array, payload of ZCL message (len is following), ignored if nullptr
// - len:       length of the 'msg' payload
// - needResponse: boolean, true = we ask the target to respond, false = the target should not respond
// - transacId: 8-bits, transation id of message (should be incremented at each message), used both for Zigbee message number and ZCL message number
// Returns: None
//
void ZigbeeZCLSend_Raw(uint16_t shortaddr, uint16_t groupaddr, uint16_t clusterId, uint8_t endpoint, uint8_t cmdId, bool clusterSpecific, uint16_t manuf, const uint8_t *msg, size_t len, bool needResponse, uint8_t transacId) {

  SBuffer buf(32+len);
  buf.add8(Z_SREQ | Z_AF);          // 24
  buf.add8(AF_DATA_REQUEST_EXT);    // 02
  if (BAD_SHORTADDR == shortaddr) {        // if no shortaddr we assume group address
    buf.add8(Z_Addr_Group);         // 01
    buf.add64(groupaddr);           // group address, only 2 LSB, upper 6 MSB are discarded
    buf.add8(0xFF);                 // dest endpoint is not used for group addresses
  } else {
    buf.add8(Z_Addr_ShortAddress);  // 02
    buf.add64(shortaddr);           // dest address, only 2 LSB, upper 6 MSB are discarded
    buf.add8(endpoint);             // dest endpoint
  }
  buf.add16(0x0000);                // dest Pan ID, 0x0000 = intra-pan
  buf.add8(0x01);                   // source endpoint
  buf.add16(clusterId);
  buf.add8(transacId);              // transacId
  buf.add8(0x30);                   // 30 options
  buf.add8(0x1E);                   // 1E radius

  buf.add16(3 + len + (manuf ? 2 : 0));
  buf.add8((needResponse ? 0x00 : 0x10) | (clusterSpecific ? 0x01 : 0x00) | (manuf ? 0x04 : 0x00));                 // Frame Control Field
  if (manuf) {
    buf.add16(manuf);               // add Manuf Id if not null
  }
  buf.add8(transacId);              // Transaction Sequance Number
  buf.add8(cmdId);
  if (len > 0) {
    buf.addBuffer(msg, len);        // add the payload
  }

  ZigbeeZNPSend(buf.getBuffer(), buf.len());
}

/********************************************************************************************/
//
// High-level function
// Send a command specified as an HEX string for the workload.
// The target endpoint is computed if zero, i.e. sent to the first known endpoint of the device.
// If cluster-specific, a timer may be set calling `zigbeeSetCommandTimer()`, for ex to coalesce attributes or Aqara presence sensor
//
// Inputs:
// - shortaddr: 16-bits short address, or 0x0000 if group address
// - groupaddr: 16-bits group address, or 0x0000 if unicast using shortaddr
// - endpoint:  8-bits target endpoint (source is always 0x01), if 0x00, it will be guessed from ZbStatus information (basically the first endpoint of the device)
// - clusterSpecific: boolean, is the message general cluster or cluster specific, used to create the FC byte of ZCL
// - clusterIf: 16-bits cluster number
// - param:     pointer to HEX string for payload, should not be nullptr
// Returns: None
//
void zigbeeZCLSendStr(uint16_t shortaddr, uint16_t groupaddr, uint8_t endpoint, bool clusterSpecific, uint16_t manuf,
                       uint16_t cluster, uint8_t cmd, const char *param) {
  size_t size = param ? strlen(param) : 0;
  SBuffer buf((size+2)/2);    // actual bytes buffer for data

  if (param) {
    while (*param) {
      uint8_t code = parseHex_P(&param, 2);
      buf.add8(code);
    }
  }

  if ((0 == endpoint) && (BAD_SHORTADDR != shortaddr)) {
    // endpoint is not specified, let's try to find it from shortAddr, unless it's a group address
    endpoint = zigbee_devices.findFirstEndpoint(shortaddr);
    //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ZbSend: guessing endpoint 0x%02X"), endpoint);
  }
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ZbSend: shortaddr 0x%04X, groupaddr 0x%04X, cluster 0x%04X, endpoint 0x%02X, cmd 0x%02X, data %s"),
    shortaddr, groupaddr, cluster, endpoint, cmd, param);

  if ((0 == endpoint) && (BAD_SHORTADDR != shortaddr)) {     // endpoint null is ok for group address
    AddLog_P2(LOG_LEVEL_INFO, PSTR("ZbSend: unspecified endpoint"));
    return;
  }

  // everything is good, we can send the command
  ZigbeeZCLSend_Raw(shortaddr, groupaddr, cluster, endpoint, cmd, clusterSpecific, manuf, buf.getBuffer(), buf.len(), true, zigbee_devices.getNextSeqNumber(shortaddr));
  // now set the timer, if any, to read back the state later
  if (clusterSpecific) {
    zigbeeSetCommandTimer(shortaddr, groupaddr, cluster, endpoint);
  }
}

// Parse "Report", "Write" or "Response" attribute
// Operation is one of: ZCL_REPORT_ATTRIBUTES (0x0A), ZCL_WRITE_ATTRIBUTES (0x02) or ZCL_READ_ATTRIBUTES_RESPONSE (0x01)
void ZbSendReportWrite(const JsonObject &val_pubwrite, uint16_t device, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint, uint16_t manuf, uint32_t operation) {
  SBuffer buf(200);       // buffer to store the binary output of attibutes

  if (nullptr == XdrvMailbox.command) {
    XdrvMailbox.command = (char*) "";             // prevent a crash when calling ReponseCmndChar and there was no previous command
  }

  // iterate on keys
  for (JsonObject::const_iterator it=val_pubwrite.begin(); it!=val_pubwrite.end(); ++it) {
    const char *key = it->key;
    const JsonVariant &value = it->value;

    uint16_t attr_id = 0xFFFF;
    uint16_t cluster_id = 0xFFFF;
    uint8_t  type_id = Znodata;
    int16_t  multiplier = 1;        // multiplier to adjust the key value
    float    val_f = 0.0f;          // alternative value if multiplier is used

    // check if the name has the format "XXXX/YYYY" where XXXX is the cluster, YYYY the attribute id
    // alternative "XXXX/YYYY%ZZ" where ZZ is the type (for unregistered attributes)
    char * delimiter = strchr(key, '/');
    char * delimiter2 = strchr(key, '%');
    if (delimiter) {
      cluster_id = strtoul(key, &delimiter, 16);
      if (!delimiter2) {
        attr_id = strtoul(delimiter+1, nullptr, 16);
      } else {
        attr_id = strtoul(delimiter+1, &delimiter2, 16);
        type_id = strtoul(delimiter2+1, nullptr, 16);
      }
    }
    // AddLog_P2(LOG_LEVEL_DEBUG, PSTR("cluster_id = 0x%04X, attr_id = 0x%04X"), cluster_id, attr_id);

    // do we already know the type, i.e. attribute and cluster are also known
    if (Znodata == type_id) {
      // scan attributes to find by name, and retrieve type
      for (uint32_t i = 0; i < ARRAY_SIZE(Z_PostProcess); i++) {
        const Z_AttributeConverter *converter = &Z_PostProcess[i];
        bool match = false;
        uint16_t local_attr_id = pgm_read_word(&converter->attribute);
        uint16_t local_cluster_id = CxToCluster(pgm_read_byte(&converter->cluster_short));
        uint8_t  local_type_id = pgm_read_byte(&converter->type);
        int16_t  local_multiplier = pgm_read_word(&converter->multiplier);
        // AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Try cluster = 0x%04X, attr = 0x%04X, type_id = 0x%02X"), local_cluster_id, local_attr_id, local_type_id);

        if (delimiter) {
          if ((cluster_id == local_cluster_id) && (attr_id == local_attr_id)) {
            type_id = local_type_id;
            break;
          }
        } else if (converter->name) {
          // AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Comparing '%s' with '%s'"), attr_name, converter->name);
          if (0 == strcasecmp_P(key, converter->name)) {
            // match
            cluster_id = local_cluster_id;
            attr_id = local_attr_id;
            type_id = local_type_id;
            multiplier = local_multiplier;
            break;
          }
        }
      }
    }

    // Buffer ready, do some sanity checks
    // AddLog_P2(LOG_LEVEL_DEBUG, PSTR("cluster_id = 0x%04X, attr_id = 0x%04X, type_id = 0x%02X"), cluster_id, attr_id, type_id);
    if ((0xFFFF == attr_id) || (0xFFFF == cluster_id)) {
      Response_P(PSTR("{\"%s\":\"%s'%s'\"}"), XdrvMailbox.command, PSTR("Unknown attribute "), key);
      return;
    }
    if (Znodata == type_id) {
      Response_P(PSTR("{\"%s\":\"%s'%s'\"}"), XdrvMailbox.command, PSTR("Unknown attribute type for attribute "), key);
      return;
    }

    if (0xFFFF == cluster) {
      cluster = cluster_id;       // set the cluster for this packet
    } else if (cluster != cluster_id) {
      ResponseCmndChar_P(PSTR("No more than one cluster id per command"));
      return;
    }
    // apply multiplier if needed
    bool use_val = true;
    if ((0 != multiplier) && (1 != multiplier)) {
      val_f = value;
      if (multiplier > 0) {         // inverse of decoding
        val_f = val_f / multiplier;
      } else {
        val_f = val_f * multiplier;
      }
      use_val = false;
    }
    // push the value in the buffer
    int32_t res = encodeSingleAttribute(buf, use_val ? value : *(const JsonVariant*)nullptr, val_f, attr_id, type_id, operation == ZCL_READ_ATTRIBUTES_RESPONSE); // force status if Reponse
    if (res < 0) {
      Response_P(PSTR("{\"%s\":\"%s'%s' 0x%02X\"}"), XdrvMailbox.command, PSTR("Unsupported attribute type "), key, type_id);
      return;
    }
  }

  // did we have any attribute?
  if (0 == buf.len()) {
    ResponseCmndChar_P(PSTR("No attribute in list"));
    return;
  }

  // all good, send the packet
  ZigbeeZCLSend_Raw(device, groupaddr, cluster, endpoint, operation, false /* not cluster specific */, manuf, buf.getBuffer(), buf.len(), false /* noresponse */, zigbee_devices.getNextSeqNumber(device));
  ResponseCmndDone();
}

// Parse the "Send" attribute and send the command
void ZbSendSend(const JsonVariant &val_cmd, uint16_t device, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint, uint16_t manuf) {
  uint8_t  cmd = 0;
  String   cmd_str = "";          // the actual low-level command, either specified or computed
  const char *cmd_s;                 // pointer to payload string
  bool     clusterSpecific = true;

  static char delim[] = ", ";     // delimiters for parameters
  // probe the type of the argument
  // If JSON object, it's high level commands
  // If String, it's a low level command
  if (val_cmd.is<JsonObject>()) {
    // we have a high-level command
    const JsonObject &cmd_obj = val_cmd.as<const JsonObject&>();
    int32_t cmd_size = cmd_obj.size();
    if (cmd_size > 1) {
      Response_P(PSTR("Only 1 command allowed (%d)"), cmd_size);
      return;
    } else if (1 == cmd_size) {
      // We have exactly 1 command, parse it
      JsonObject::const_iterator it = cmd_obj.begin();    // just get the first key/value
      String key = it->key;
      const JsonVariant& value = it->value;
      uint32_t x = 0, y = 0, z = 0;
      uint16_t cmd_var;
      uint16_t local_cluster_id;

      const __FlashStringHelper* tasmota_cmd = zigbeeFindCommand(key.c_str(), &local_cluster_id, &cmd_var);
      if (tasmota_cmd) {
        cmd_str = tasmota_cmd;
      } else {
        Response_P(PSTR("Unrecognized zigbee command: %s"), key.c_str());
        return;
      }
      // check cluster
      if (0xFFFF == cluster) {
        cluster = local_cluster_id;
      } else if (cluster != local_cluster_id) {
        ResponseCmndChar_P(PSTR("No more than one cluster id per command"));
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

      //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ZbSend: command_template = %s"), cmd_str.c_str());
      if (0xFF == cmd_var) {      // if command number is a variable, replace it with x
        cmd = x;
        x = y;                  // and shift other variables
        y = z;
      } else {
        cmd = cmd_var;          // or simply copy the cmd number
      }
      cmd_str = zigbeeCmdAddParams(cmd_str.c_str(), x, y, z);   // fill in parameters
      //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ZbSend: command_final    = %s"), cmd_str.c_str());
      cmd_s = cmd_str.c_str();
    } else {
      // we have zero command, pass through until last error for missing command
    }
  } else if (val_cmd.is<const char*>()) {
    // low-level command
    cmd_str = val_cmd.as<String>();
    // Now parse the string to extract cluster, command, and payload
    // Parse 'cmd' in the form "AAAA_BB/CCCCCCCC" or "AAAA!BB/CCCCCCCC"
    // where AA is the cluster number, BBBB the command number, CCCC... the payload
    // First delimiter is '_' for a global command, or '!' for a cluster specific command
    const char * data = cmd_str.c_str();
    uint16_t local_cluster_id = parseHex(&data, 4);

    // check cluster
    if (0xFFFF == cluster) {
      cluster = local_cluster_id;
    } else if (cluster != local_cluster_id) {
      ResponseCmndChar_P(PSTR("No more than one cluster id per command"));
      return;
    }

    // delimiter
    if (('_' == *data) || ('!' == *data)) {
      if ('_' == *data) { clusterSpecific = false; }
      data++;
    } else {
      ResponseCmndChar_P(PSTR("Wrong delimiter for payload"));
      return;
    }
    // parse cmd number
    cmd = parseHex(&data, 2);

    // move to end of payload
    // delimiter is optional
    if ('/' == *data) { data++; }   // skip delimiter

    cmd_s = data;
  } else {
    // we have an unsupported command type, just ignore it and fallback to missing command
  }

  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ZigbeeZCLSend device: 0x%04X, group: 0x%04X, endpoint:%d, cluster:0x%04X, cmd:0x%02X, send:\"%s\""),
            device, groupaddr, endpoint, cluster, cmd, cmd_s);
  zigbeeZCLSendStr(device, groupaddr, endpoint, clusterSpecific, manuf, cluster, cmd, cmd_s);
  ResponseCmndDone();
}


// Parse the "Send" attribute and send the command
void ZbSendRead(const JsonVariant &val_attr, uint16_t device, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint, uint16_t manuf) {
  // ZbSend {"Device":"0xF289","Cluster":0,"Endpoint":3,"Read":5}
  // ZbSend {"Device":"0xF289","Cluster":"0x0000","Endpoint":"0x0003","Read":"0x0005"}
  // ZbSend {"Device":"0xF289","Cluster":0,"Endpoint":3,"Read":[5,6,7,4]}
  // ZbSend {"Device":"0xF289","Endpoint":3,"Read":{"ModelId":true}}
  // ZbSend {"Device":"0xF289","Read":{"ModelId":true}}

  // params
  size_t   attrs_len = 0;
  uint8_t* attrs = nullptr;       // empty string is valid

  uint16_t val = strToUInt(val_attr);
  if (val_attr.is<JsonArray>()) {
    const JsonArray& attr_arr = val_attr.as<const JsonArray&>();
    attrs_len = attr_arr.size() * 2;
    attrs = new uint8_t[attrs_len];

    uint32_t i = 0;
    for (auto value : attr_arr) {
      uint16_t val = strToUInt(value);
      attrs[i++] = val & 0xFF;
      attrs[i++] = val >> 8;
    }
  } else if (val_attr.is<JsonObject>()) {
    const JsonObject& attr_obj = val_attr.as<const JsonObject&>();
    attrs_len = attr_obj.size() * 2;
    attrs = new uint8_t[attrs_len];
    uint32_t actual_attr_len = 0;

    // iterate on keys
    for (JsonObject::const_iterator it=attr_obj.begin(); it!=attr_obj.end(); ++it) {
      const char *key = it->key;
      // const JsonVariant &value = it->value;      // we don't need the value here, only keys are relevant

      bool found = false;
      // scan attributes to find by name, and retrieve type
      for (uint32_t i = 0; i < ARRAY_SIZE(Z_PostProcess); i++) {
        const Z_AttributeConverter *converter = &Z_PostProcess[i];
        bool match = false;
        uint16_t local_attr_id = pgm_read_word(&converter->attribute);
        uint16_t local_cluster_id = CxToCluster(pgm_read_byte(&converter->cluster_short));
        // uint8_t  local_type_id = pgm_read_byte(&converter->type);

        if ((converter->name) && (0 == strcasecmp_P(key, converter->name))) {
          // match name
          // check if there is a conflict with cluster
          // TODO
          attrs[actual_attr_len++] = local_attr_id & 0xFF;
          attrs[actual_attr_len++] = local_attr_id >> 8;
          found = true;
          // check cluster
          if (0xFFFF == cluster) {
            cluster = local_cluster_id;
          } else if (cluster != local_cluster_id) {
            ResponseCmndChar_P(PSTR("No more than one cluster id per command"));
            if (attrs) { delete[] attrs; }
            return;
          }
          break;    // found, exit loop
        }
      }
      if (!found) {
        AddLog_P2(LOG_LEVEL_INFO, PSTR("ZIG: Unknown attribute name (ignored): %s"), key);
      }
    }

    attrs_len = actual_attr_len;
  } else {
    attrs_len = 2;
    attrs = new uint8_t[attrs_len];
    attrs[0] = val & 0xFF;    // little endian
    attrs[1] = val >> 8;
  }

  if (attrs_len > 0) {
    ZigbeeZCLSend_Raw(device, groupaddr, cluster, endpoint, ZCL_READ_ATTRIBUTES, false, manuf, attrs, attrs_len, true /* we do want a response */, zigbee_devices.getNextSeqNumber(device));
    ResponseCmndDone();
  } else {
    ResponseCmndChar_P(PSTR("Missing parameters"));
  }

  if (attrs) { delete[] attrs; }
}

//
// Command `ZbSend`
//
// Examples:
// ZbSend {"Device":"0x0000","Endpoint":1,"Write":{"0006/0000":0}}
// ZbSend {"Device":"0x0000","Endpoint":1,"Write":{"Power":0}}
// ZbSend {"Device":"0x0000","Endpoint":1,"Write":{"AqaraRotate":0}}
// ZbSend {"Device":"0x0000","Endpoint":1,"Write":{"AqaraRotate":12.5}}
// ZbSend {"Device":"0x0000","Endpoint":1,"Write":{"006/0000%39":12.5}}
// ZbSend {"Device":"0x0000","Endpoint":1,"Write":{"AnalogInApplicationType":1000000}}
// ZbSend {"Device":"0x0000","Endpoint":1,"Write":{"TimeZone":-1000000}}
// ZbSend {"Device":"0x0000","Endpoint":1,"Write":{"Manufacturer":"Tasmota","ModelId":"Tasmota Z2T Router"}}
void CmndZbSend(void) {
  // ZbSend { "device":"0x1234", "endpoint":"0x03", "send":{"Power":1} }
  // ZbSend { "device":"0x1234", "endpoint":"0x03", "send":{"Power":"3"} }
  // ZbSend { "device":"0x1234", "endpoint":"0x03", "send":{"Power":"0xFF"} }
  // ZbSend { "device":"0x1234", "endpoint":"0x03", "send":{"Power":null} }
  // ZbSend { "device":"0x1234", "endpoint":"0x03", "send":{"Power":false} }
  // ZbSend { "device":"0x1234", "endpoint":"0x03", "send":{"Power":true} }
  // ZbSend { "device":"0x1234", "endpoint":"0x03", "send":{"Power":"true"} }
  // ZbSend { "device":"0x1234", "endpoint":"0x03", "send":{"ShutterClose":null} }
  // ZbSend { "device":"0x1234", "endpoint":"0x03", "send":{"Power":1} }
  // ZbSend { "device":"0x1234", "endpoint":"0x03", "send":{"Color":"1,2"} }
  // ZbSend { "device":"0x1234", "endpoint":"0x03", "send":{"Color":"0x1122,0xFFEE"} }
  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }
  DynamicJsonBuffer jsonBuf;
  const JsonObject &json = jsonBuf.parseObject((const char*) XdrvMailbox.data);
  if (!json.success()) { ResponseCmndChar_P(PSTR(D_JSON_INVALID_JSON)); return; }

  // params
  uint16_t device = BAD_SHORTADDR;    // BAD_SHORTADDR is broadcast, so considered invalid
  uint16_t groupaddr = 0x0000;        // group address valid only if device == BAD_SHORTADDR
  uint16_t cluster = 0xFFFF;          // no default
  uint8_t  endpoint = 0x00;           // 0x00 is invalid for the dst endpoint
  uint16_t manuf = 0x0000;            // Manuf Id in ZCL frame


  // parse "Device" and "Group"
  const JsonVariant &val_device = GetCaseInsensitive(json, PSTR(D_CMND_ZIGBEE_DEVICE));
  if (nullptr != &val_device) {
    device = zigbee_devices.parseDeviceParam(val_device.as<char*>());
    if (BAD_SHORTADDR == device) { ResponseCmndChar_P(PSTR("Invalid parameter")); return; }
  }
  if (BAD_SHORTADDR == device) {     // if not found, check if we have a group
    const JsonVariant &val_group = GetCaseInsensitive(json, PSTR(D_CMND_ZIGBEE_GROUP));
    if (nullptr != &val_group) {
      groupaddr = strToUInt(val_group);
    } else {                  // no device nor group
      ResponseCmndChar_P(PSTR("Unknown device"));
      return;
    }
  }
  // from here, either device has a device shortaddr, or if BAD_SHORTADDR then use group address
  // Note: groupaddr == 0 is valid

  // read other parameters
  const JsonVariant &val_cluster = GetCaseInsensitive(json, PSTR(D_CMND_ZIGBEE_CLUSTER));
  if (nullptr != &val_cluster) { cluster = strToUInt(val_cluster); }
  const JsonVariant &val_endpoint = GetCaseInsensitive(json, PSTR(D_CMND_ZIGBEE_ENDPOINT));
  if (nullptr != &val_endpoint) { endpoint = strToUInt(val_endpoint); }
  const JsonVariant &val_manuf = GetCaseInsensitive(json, PSTR(D_CMND_ZIGBEE_MANUF));
  if (nullptr != &val_manuf) { manuf = strToUInt(val_manuf); }

  // infer endpoint
  if (BAD_SHORTADDR == device) {
    endpoint = 0xFF;                  // endpoint not used for group addresses, so use a dummy broadcast endpoint
  } else if (0 == endpoint) {         // if it was not already specified, try to guess it
    endpoint = zigbee_devices.findFirstEndpoint(device);
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ZIG: guessing endpoint %d"), endpoint);
  }
  if (0 == endpoint) {                // after this, if it is still zero, then it's an error
      ResponseCmndChar_P(PSTR("Missing endpoint"));
      return;
  }
  // from here endpoint is valid and non-zero
  // cluster may be already specified or 0xFFFF

  const JsonVariant &val_cmd = GetCaseInsensitive(json, PSTR(D_CMND_ZIGBEE_SEND));
  const JsonVariant &val_read = GetCaseInsensitive(json, PSTR(D_CMND_ZIGBEE_READ));
  const JsonVariant &val_write = GetCaseInsensitive(json, PSTR(D_CMND_ZIGBEE_WRITE));
  const JsonVariant &val_publish = GetCaseInsensitive(json, PSTR(D_CMND_ZIGBEE_REPORT));
  const JsonVariant &val_response = GetCaseInsensitive(json, PSTR(D_CMND_ZIGBEE_RESPONSE));
  uint32_t multi_cmd = (nullptr != &val_cmd) + (nullptr != &val_read) + (nullptr != &val_write) + (nullptr != &val_publish)+ (nullptr != &val_response);
  if (multi_cmd > 1) {
    ResponseCmndChar_P(PSTR("Can only have one of: 'Send', 'Read', 'Write', 'Report' or 'Reponse'"));
    return;
  }
  // from here we have one and only one command

  if (nullptr != &val_cmd) {
    // "Send":{...commands...}
    // we accept either a string or a JSON object
    ZbSendSend(val_cmd, device, groupaddr, cluster, endpoint, manuf);
  } else if (nullptr != &val_read) {
    // "Read":{...attributes...}, "Read":attribute or "Read":[...attributes...]
    // we accept eitehr a number, a string, an array of numbers/strings, or a JSON object
    ZbSendRead(val_read, device, groupaddr, cluster, endpoint, manuf);
  } else if (nullptr != &val_write) {
    // only KSON object
    if (!val_write.is<JsonObject>()) {
      ResponseCmndChar_P(PSTR("Missing parameters"));
      return;
    }
    // "Write":{...attributes...}
    ZbSendReportWrite(val_write, device, groupaddr, cluster, endpoint, manuf, ZCL_WRITE_ATTRIBUTES);
  } else if (nullptr != &val_publish) {
    // "Report":{...attributes...}
    // only KSON object
    if (!val_publish.is<JsonObject>()) {
      ResponseCmndChar_P(PSTR("Missing parameters"));
      return;
    }
    ZbSendReportWrite(val_publish, device, groupaddr, cluster, endpoint, manuf, ZCL_REPORT_ATTRIBUTES);
  } else if (nullptr != &val_response) {
    // "Report":{...attributes...}
    // only KSON object
    if (!val_response.is<JsonObject>()) {
      ResponseCmndChar_P(PSTR("Missing parameters"));
      return;
    }
    ZbSendReportWrite(val_response, device, groupaddr, cluster, endpoint, manuf, ZCL_READ_ATTRIBUTES_RESPONSE);
  } else {
    Response_P(PSTR("Missing zigbee 'Send', 'Write', 'Report' or 'Response'"));
    return;
  }
}

//
// Command `ZbBind`
//
void ZbBindUnbind(bool unbind) {    // false = bind, true = unbind
  // ZbBind {"Device":"<device>", "Endpoint":<endpoint>, "Cluster":<cluster>, "ToDevice":"<to_device>", "ToEndpoint":<to_endpoint>, "ToGroup":<to_group> }
  // ZbUnbind {"Device":"<device>", "Endpoint":<endpoint>, "Cluster":<cluster>, "ToDevice":"<to_device>", "ToEndpoint":<to_endpoint>, "ToGroup":<to_group> }

  // local endpoint is always 1, IEEE addresses are calculated
  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }
  DynamicJsonBuffer jsonBuf;
  const JsonObject &json = jsonBuf.parseObject((const char*) XdrvMailbox.data);
  if (!json.success()) { ResponseCmndChar_P(PSTR(D_JSON_INVALID_JSON)); return; }

  // params
  uint16_t srcDevice = BAD_SHORTADDR;         // BAD_SHORTADDR is broadcast, so considered invalid
  uint16_t dstDevice = BAD_SHORTADDR;      // BAD_SHORTADDR is broadcast, so considered invalid
  uint64_t dstLongAddr = 0;
  uint8_t  endpoint = 0x00;         // 0x00 is invalid for the src endpoint
  uint8_t  toendpoint = 0x00;       // 0x00 is invalid for the dst endpoint
  uint16_t toGroup = 0x0000;        // group address
  uint16_t cluster  = 0;            // 0xFFFF is invalid
  uint32_t group = 0xFFFFFFFF;      // 16 bits values, otherwise 0xFFFFFFFF is unspecified

  // Information about source device: "Device", "Endpoint", "Cluster"
  //  - the source endpoint must have a known IEEE address
  const JsonVariant &val_device = GetCaseInsensitive(json, PSTR(D_CMND_ZIGBEE_DEVICE));
  if (nullptr != &val_device) {
    srcDevice = zigbee_devices.parseDeviceParam(val_device.as<char*>());
  }
  if ((nullptr == &val_device) || (BAD_SHORTADDR == srcDevice)) { ResponseCmndChar_P(PSTR("Unknown source device")); return; }
  // check if IEEE address is known
  uint64_t srcLongAddr = zigbee_devices.getDeviceLongAddr(srcDevice);
  if (0 == srcLongAddr) { ResponseCmndChar_P(PSTR("Unknown source IEEE address")); return; }
  // look for source endpoint
  const JsonVariant &val_endpoint = GetCaseInsensitive(json, PSTR(D_CMND_ZIGBEE_ENDPOINT));
  if (nullptr != &val_endpoint) { endpoint = strToUInt(val_endpoint); }
  // look for source cluster
  const JsonVariant &val_cluster = GetCaseInsensitive(json, PSTR(D_CMND_ZIGBEE_CLUSTER));
  if (nullptr != &val_cluster) { cluster = strToUInt(val_cluster); }

  // Either Device address
  // In this case the following parameters are mandatory
  //  - "ToDevice" and the device must have a known IEEE address
  //  - "ToEndpoint"
  const JsonVariant &dst_device = GetCaseInsensitive(json, PSTR("ToDevice"));
  if (nullptr != &dst_device) {
    dstDevice = zigbee_devices.parseDeviceParam(dst_device.as<char*>());
    if (BAD_SHORTADDR == dstDevice) { ResponseCmndChar_P(PSTR("Invalid parameter")); return; }
    if (0x0000 == dstDevice) {
      dstLongAddr = localIEEEAddr;
    } else {
      dstLongAddr = zigbee_devices.getDeviceLongAddr(dstDevice);
    }
    if (0 == dstLongAddr) { ResponseCmndChar_P(PSTR("Unknown dest IEEE address")); return; }

    const JsonVariant &val_toendpoint = GetCaseInsensitive(json, PSTR("ToEndpoint"));
    if (nullptr != &val_toendpoint) { toendpoint = strToUInt(val_endpoint); } else { toendpoint = endpoint; }
  }

  // Or Group Address - we don't need a dstEndpoint in this case
  const JsonVariant &to_group = GetCaseInsensitive(json, PSTR("ToGroup"));
  if (nullptr != &to_group) { toGroup = strToUInt(to_group); }

  // make sure we don't have conflicting parameters
  if (&to_group && dstLongAddr) { ResponseCmndChar_P(PSTR("Cannot have both \"ToDevice\" and \"ToGroup\"")); return; }
  if (!&to_group && !dstLongAddr) { ResponseCmndChar_P(PSTR("Missing \"ToDevice\" or \"ToGroup\"")); return; }

  SBuffer buf(34);
  buf.add8(Z_SREQ | Z_ZDO);
  if (unbind) {
    buf.add8(ZDO_UNBIND_REQ);
  } else {
    buf.add8(ZDO_BIND_REQ);
  }
  buf.add16(srcDevice);
  buf.add64(srcLongAddr);
  buf.add8(endpoint);
  buf.add16(cluster);
  if (dstLongAddr) {
    buf.add8(Z_Addr_IEEEAddress);         // DstAddrMode - 0x03 = ADDRESS_64_BIT
    buf.add64(dstLongAddr);
    buf.add8(toendpoint);
  } else {
    buf.add8(Z_Addr_Group);               // DstAddrMode - 0x01 = GROUP_ADDRESS
    buf.add16(toGroup);
  }

  ZigbeeZNPSend(buf.getBuffer(), buf.len());

  ResponseCmndDone();
}

//
// Command ZbBind
//
void CmndZbBind(void) {
  ZbBindUnbind(false);
}

//
// Command ZbBind
//
void CmndZbUnbind(void) {
  ZbBindUnbind(true);
}

//
// Command `ZbBindState`
//
void CmndZbBindState(void) {
  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }
  uint16_t shortaddr = zigbee_devices.parseDeviceParam(XdrvMailbox.data);
  if (BAD_SHORTADDR == shortaddr) { ResponseCmndChar_P(PSTR("Unknown device")); return; }

  SBuffer buf(10);
  buf.add8(Z_SREQ | Z_ZDO);             // 25
  buf.add8(ZDO_MGMT_BIND_REQ);          // 33
  buf.add16(shortaddr);                 // shortaddr
  buf.add8(0);                          // StartIndex = 0

  ZigbeeZNPSend(buf.getBuffer(), buf.len());

  ResponseCmndDone();
}

// Probe a specific device to get its endpoints and supported clusters
void CmndZbProbe(void) {
  CmndZbProbeOrPing(true);
}

//
// Common code for `ZbProbe` and `ZbPing`
//
void CmndZbProbeOrPing(boolean probe) {
  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }
  uint16_t shortaddr = zigbee_devices.parseDeviceParam(XdrvMailbox.data);
  if (BAD_SHORTADDR == shortaddr) { ResponseCmndChar_P(PSTR("Unknown device")); return; }

  // everything is good, we can send the command
  Z_SendIEEEAddrReq(shortaddr);
  if (probe) {
    Z_SendActiveEpReq(shortaddr);
  }
  ResponseCmndDone();
}

// Ping a device, actually a simplified version of ZbProbe
void CmndZbPing(void) {
  CmndZbProbeOrPing(false);
}

//
// Command `ZbName`
// Specify, read or erase a Friendly Name
//
void CmndZbName(void) {
  // Syntax is:
  //  ZbName <device_id>,<friendlyname>  - assign a friendly name
  //  ZbName <device_id>                 - display the current friendly name
  //  ZbName <device_id>,                - remove friendly name
  //
  // Where <device_id> can be: short_addr, long_addr, device_index, friendly_name

  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }

  // check if parameters contain a comma ','
  char *p;
  char *str = strtok_r(XdrvMailbox.data, ", ", &p);

  // parse first part, <device_id>
  uint16_t shortaddr = zigbee_devices.parseDeviceParam(XdrvMailbox.data, true);  // in case of short_addr, it must be already registered
  if (BAD_SHORTADDR == shortaddr) { ResponseCmndChar_P(PSTR("Unknown device")); return; }

  if (p == nullptr) {
    const char * friendlyName = zigbee_devices.getFriendlyName(shortaddr);
    Response_P(PSTR("{\"0x%04X\":{\"" D_JSON_ZIGBEE_NAME "\":\"%s\"}}"), shortaddr, friendlyName ? friendlyName : "");
  } else {
    zigbee_devices.setFriendlyName(shortaddr, p);
    Response_P(PSTR("{\"0x%04X\":{\"" D_JSON_ZIGBEE_NAME "\":\"%s\"}}"), shortaddr, p);
  }
}

//
// Command `ZbName`
// Specify, read or erase a ModelId, only for debug purposes
//
void CmndZbModelId(void) {
  // Syntax is:
  //  ZbName <device_id>,<friendlyname>  - assign a friendly name
  //  ZbName <device_id>                 - display the current friendly name
  //  ZbName <device_id>,                - remove friendly name
  //
  // Where <device_id> can be: short_addr, long_addr, device_index, friendly_name

  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }

  // check if parameters contain a comma ','
  char *p;
  char *str = strtok_r(XdrvMailbox.data, ", ", &p);

  // parse first part, <device_id>
  uint16_t shortaddr = zigbee_devices.parseDeviceParam(XdrvMailbox.data, true);  // in case of short_addr, it must be already registered
  if (BAD_SHORTADDR == shortaddr) { ResponseCmndChar_P(PSTR("Unknown device")); return; }

  if (p == nullptr) {
    const char * modelId = zigbee_devices.getModelId(shortaddr);
    Response_P(PSTR("{\"0x%04X\":{\"" D_JSON_ZIGBEE_MODELID "\":\"%s\"}}"), shortaddr, modelId ? modelId : "");
  } else {
    zigbee_devices.setModelId(shortaddr, p);
    Response_P(PSTR("{\"0x%04X\":{\"" D_JSON_ZIGBEE_MODELID "\":\"%s\"}}"), shortaddr, p);
  }
}

//
// Command `ZbLight`
// Specify, read or erase a Light type for Hue/Alexa integration
void CmndZbLight(void) {
  // Syntax is:
  //  ZbLight <device_id>,<x>            - assign a bulb type 0-5
  //  ZbLight <device_id>                - display the current bulb type and status
  //
  // Where <device_id> can be: short_addr, long_addr, device_index, friendly_name

  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }

  // check if parameters contain a comma ','
  char *p;
  char *str = strtok_r(XdrvMailbox.data, ", ", &p);

  // parse first part, <device_id>
  uint16_t shortaddr = zigbee_devices.parseDeviceParam(XdrvMailbox.data, true);  // in case of short_addr, it must be already registered
  if (BAD_SHORTADDR == shortaddr) { ResponseCmndChar_P(PSTR("Unknown device")); return; }

  if (p) {
    int8_t bulbtype = strtol(p, nullptr, 10);
    if (bulbtype > 5)  { bulbtype = 5; }
    if (bulbtype < -1) { bulbtype = -1; }
    zigbee_devices.setHueBulbtype(shortaddr, bulbtype);
  }
  String dump = zigbee_devices.dumpLightState(shortaddr);
  Response_P(PSTR("{\"" D_PRFX_ZB D_CMND_ZIGBEE_LIGHT "\":%s}"), dump.c_str());

  MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_PRFX_ZB D_CMND_ZIGBEE_LIGHT));
  XdrvRulesProcess();
  ResponseCmndDone();
}

//
// Command `ZbForget`
// Remove an old Zigbee device from the list of known devices, use ZigbeeStatus to know all registered devices
//
void CmndZbForget(void) {
  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }
  uint16_t shortaddr = zigbee_devices.parseDeviceParam(XdrvMailbox.data);
  if (BAD_SHORTADDR == shortaddr) { ResponseCmndChar_P(PSTR("Unknown device")); return; }

  // everything is good, we can send the command
  if (zigbee_devices.removeDevice(shortaddr)) {
    ResponseCmndDone();
  } else {
    ResponseCmndChar_P(PSTR("Unknown device"));
  }
}

//
// Command `ZbSave`
// Save Zigbee information to flash
//
void CmndZbSave(void) {
  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }
  saveZigbeeDevices();
  ResponseCmndDone();
}


// Restore a device configuration previously exported via `ZbStatus2``
// Format:
// Either the entire `ZbStatus3` export, or an array or just the device configuration.
// If array, if can contain multiple devices
//   ZbRestore {"ZbStatus3":[{"Device":"0x5ADF","Name":"Petite_Lampe","IEEEAddr":"0x90FD9FFFFE03B051","ModelId":"TRADFRI bulb E27 WS opal 980lm","Manufacturer":"IKEA of Sweden","Endpoints":["0x01","0xF2"]}]}
//   ZbRestore [{"Device":"0x5ADF","Name":"Petite_Lampe","IEEEAddr":"0x90FD9FFFFE03B051","ModelId":"TRADFRI bulb E27 WS opal 980lm","Manufacturer":"IKEA of Sweden","Endpoints":["0x01","0xF2"]}]
//   ZbRestore {"Device":"0x5ADF","Name":"Petite_Lampe","IEEEAddr":"0x90FD9FFFFE03B051","ModelId":"TRADFRI bulb E27 WS opal 980lm","Manufacturer":"IKEA of Sweden","Endpoints":["0x01","0xF2"]}
void CmndZbRestore(void) {
  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }
  DynamicJsonBuffer jsonBuf;
  const JsonVariant json_parsed = jsonBuf.parse((const char*) XdrvMailbox.data);   // const to force a copy of parameter
  const JsonVariant * json = &json_parsed;    // root of restore, to be changed if needed
  bool success = false;

  // check if parsing succeeded
  if (json_parsed.is<JsonObject>()) {
    success = json_parsed.as<const JsonObject&>().success();
  } else if (json_parsed.is<JsonArray>()) {
    success = json_parsed.as<const JsonArray&>().success();
  }
  if (!success) { ResponseCmndChar_P(PSTR(D_JSON_INVALID_JSON)); return; }

  // Check is root contains `ZbStatus<x>` key, if so change the root
  const JsonVariant * zbstatus = &startsWithCaseInsensitive(*json, PSTR("ZbStatus"));
  if (nullptr != zbstatus) {
    json = zbstatus;
  }

  // check if the root is an array
  if (json->is<JsonArray>()) {
    const JsonArray& arr = json->as<const JsonArray&>();
    for (auto elt : arr) {
      // call restore on each item
      int32_t res = zigbee_devices.deviceRestore(elt);
      if (res < 0) {
        ResponseCmndChar_P(PSTR("Restore failed"));
        return;
      }
    }
  } else if (json->is<JsonObject>()) {
    int32_t res = zigbee_devices.deviceRestore(*json);
    if (res < 0) {
      ResponseCmndChar_P(PSTR("Restore failed"));
      return;
    }
    // call restore on a single object
  } else {
    ResponseCmndChar_P(PSTR("Missing parameters"));
    return;
  }
  ResponseCmndDone();
}

//
// Command `ZbPermitJoin`
// Allow or Deny pairing of new Zigbee devices
//
void CmndZbPermitJoin(void) {
  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }
  uint32_t payload = XdrvMailbox.payload;
  uint16_t dstAddr = 0xFFFC;            // default addr
  uint8_t  duration = 60;               // default 60s

  if (payload <= 0) {
    duration = 0;
  } else if (99 == payload) {
    duration = 0xFF;                    // unlimited time
  }

  SBuffer buf(34);
  buf.add8(Z_SREQ | Z_ZDO);             // 25
  buf.add8(ZDO_MGMT_PERMIT_JOIN_REQ);   // 36
  buf.add8(0x0F);                       // AddrMode
  buf.add16(0xFFFC);                    // DstAddr
  buf.add8(duration);
  buf.add8(0x00);                       // TCSignificance

  ZigbeeZNPSend(buf.getBuffer(), buf.len());

  ResponseCmndDone();
}

//
// Command `ZbStatus`
//
void CmndZbStatus(void) {
  if (ZigbeeSerial) {
    if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }
    uint16_t shortaddr = zigbee_devices.parseDeviceParam(XdrvMailbox.data);
    if (XdrvMailbox.payload > 0) {
      if (BAD_SHORTADDR == shortaddr) { ResponseCmndChar_P(PSTR("Unknown device")); return; }
    }

    String dump = zigbee_devices.dump(XdrvMailbox.index, shortaddr);
    Response_P(PSTR("{\"%s%d\":%s}"), XdrvMailbox.command, XdrvMailbox.index, dump.c_str());
  }
}

//
// Command `ZbConfig`
//
void CmndZbConfig(void) {
  // ZbConfig
  // ZbConfig {"Channel":11,"PanID":"0x1A63","ExtPanID":"0xCCCCCCCCCCCCCCCC","KeyL":"0x0F0D0B0907050301L","KeyH":"0x0D0C0A0806040200L"}
  uint8_t     zb_channel     = Settings.zb_channel;
  uint16_t    zb_pan_id      = Settings.zb_pan_id;
  uint64_t    zb_ext_panid   = Settings.zb_ext_panid;
  uint64_t    zb_precfgkey_l = Settings.zb_precfgkey_l;
  uint64_t    zb_precfgkey_h = Settings.zb_precfgkey_h;

  // if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }
  RemoveAllSpaces(XdrvMailbox.data);
  if (strlen(XdrvMailbox.data) > 0) {
    DynamicJsonBuffer jsonBuf;
    const JsonObject &json = jsonBuf.parseObject((const char*) XdrvMailbox.data);
    if (!json.success()) { ResponseCmndChar_P(PSTR(D_JSON_INVALID_JSON)); return; }

    // Channel
    const JsonVariant &val_channel = GetCaseInsensitive(json, PSTR("Channel"));
    if (nullptr != &val_channel) { zb_channel = strToUInt(val_channel); }
    if (zb_channel < 11) { zb_channel = 11; }
    if (zb_channel > 26) { zb_channel = 26; }
    // PanID
    const JsonVariant &val_pan_id = GetCaseInsensitive(json, PSTR("PanID"));
    if (nullptr != &val_pan_id) { zb_pan_id = strToUInt(val_pan_id); }
    // ExtPanID
    const JsonVariant &val_ext_pan_id = GetCaseInsensitive(json, PSTR("ExtPanID"));
    if (nullptr != &val_ext_pan_id) { zb_ext_panid = strtoull(val_ext_pan_id.as<const char*>(), nullptr, 0); }
    // KeyL
    const JsonVariant &val_key_l = GetCaseInsensitive(json, PSTR("KeyL"));
    if (nullptr != &val_key_l) { zb_precfgkey_l = strtoull(val_key_l.as<const char*>(), nullptr, 0); }
    // KeyH
    const JsonVariant &val_key_h = GetCaseInsensitive(json, PSTR("KeyH"));
    if (nullptr != &val_key_h) { zb_precfgkey_h = strtoull(val_key_h.as<const char*>(), nullptr, 0); }

    // Check if a parameter was changed after all
    if ( (zb_channel      != Settings.zb_channel) ||
         (zb_pan_id       != Settings.zb_pan_id) ||
         (zb_ext_panid    != Settings.zb_ext_panid) ||
         (zb_precfgkey_l  != Settings.zb_precfgkey_l) ||
         (zb_precfgkey_h  != Settings.zb_precfgkey_h) ) {
      Settings.zb_channel      = zb_channel;
      Settings.zb_pan_id       = zb_pan_id;
      Settings.zb_ext_panid    = zb_ext_panid;
      Settings.zb_precfgkey_l  = zb_precfgkey_l;
      Settings.zb_precfgkey_h  = zb_precfgkey_h;
      restart_flag = 2;    // save and reboot
    }
  }

  // display the current or new configuration
  char hex_ext_panid[20] = "0x";
  Uint64toHex(zb_ext_panid, &hex_ext_panid[2], 64);
  char hex_precfgkey_l[20] = "0x";
  Uint64toHex(zb_precfgkey_l, &hex_precfgkey_l[2], 64);
  char hex_precfgkey_h[20] = "0x";
  Uint64toHex(zb_precfgkey_h, &hex_precfgkey_h[2], 64);

  // {"ZbConfig":{"Channel":11,"PanID":"0x1A63","ExtPanID":"0xCCCCCCCCCCCCCCCC","KeyL":"0x0F0D0B0907050301L","KeyH":"0x0D0C0A0806040200L"}}
  Response_P(PSTR("{\"" D_PRFX_ZB D_JSON_ZIGBEE_CONFIG "\":{"
                  "\"Channel\":%d"
                  ",\"PanID\":\"0x%04X\""
                  ",\"ExtPanID\":\"%s\""
                  ",\"KeyL\":\"%s\""
                  ",\"KeyH\":\"%s\""
                  "}}"),
                  zb_channel, zb_pan_id,
                  hex_ext_panid,
                  hex_precfgkey_l, hex_precfgkey_h);
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
        if (ZigbeeSerial) { ZigbeeInputLoop(); }
				if (zigbee.state_machine) {
          ZigbeeStateMachine_Run();
				}
        break;
      case FUNC_PRE_INIT:
        ZigbeeInit();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kZbCommands, ZigbeeCommand);
        break;
    }
  }
  return result;
}

#endif // USE_ZIGBEE
