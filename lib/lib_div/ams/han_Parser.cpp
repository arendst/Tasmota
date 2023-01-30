#include "han_Parser.h"
#include "Cosem.h"


extern int SML_print(const char *, ...);
#define han_debug SML_print


Han_Parser::Han_Parser(uint16_t (dp)(uint8_t, uint8_t), uint8_t m, uint8_t *key, uint8_t *auth) {
    dispatch = dp;
    meter = m;
    memmove(encryptionKey, key, 16);
    if (auth) {
      memmove(authenticationKey, auth, 16);
    } else {
      memset(authenticationKey, 0, 16);
    }
}

Han_Parser::~Han_Parser(void) {
  if (hdlcParser) delete hdlcParser;
  if (mbusParser) delete mbusParser;
  if (gbtParser) delete gbtParser;
  if (gcmParser) delete gcmParser;
  if (llcParser) delete llcParser;
  if (dlmsParser) delete dlmsParser;
  if (dsmrParser) delete dsmrParser;
}

int Han_Parser::serial_available(void) {
  return dispatch(meter, 0);
}
int Han_Parser::serial_read(void) {
  return dispatch(meter, 1);
}

int16_t Han_Parser::serial_readBytes(uint8_t *buf, uint16_t size) {
  if (size > serial_available()) {
    size = serial_available();
  }
  for (uint16_t cnt = 0; cnt < size; cnt++) {
    buf[cnt] = serial_read();
  }
  return size;
}

bool Han_Parser::readHanPort(uint8_t **out, uint16_t *size) {

	if (!serial_available()) return false;

	// Before reading, empty serial buffer to increase chance of getting first byte of a data transfer
	if (!serialInit) {
		serial_readBytes(hanBuffer, BUF_SIZE_HAN);
		serialInit = true;
		return false;
	}

	DataParserContext ctx = {0};
	int pos = DATA_PARSE_INCOMPLETE;
	// For each byte received, check if we have a complete frame we can handle
	while (serial_available() && pos == DATA_PARSE_INCOMPLETE) {
    yield();
    // If buffer was overflowed, reset
		if (len >= BUF_SIZE_HAN) {
			serial_readBytes(hanBuffer, BUF_SIZE_HAN);
			len = 0;
			han_debug(PSTR("Buffer overflow, resetting"));
			return false;
		}
		hanBuffer[len++] = serial_read();
		ctx.length = len;
		pos = unwrapData((uint8_t *) hanBuffer, ctx);
		if(ctx.type > 0 && pos >= 0) {
			if(ctx.type == DATA_TAG_DLMS) {
				han_debug(PSTR("Received valid DLMS at %d"), pos);
			} else if(ctx.type == DATA_TAG_DSMR) {
				han_debug(PSTR("Received valid DSMR at %d"), pos);
			} else {
				// TODO: Move this so that payload is sent to MQTT
				han_debug(PSTR("Unknown tag %02X at pos %d"), ctx.type, pos);
				len = 0;
				return false;
			}
		}
	}
	if (pos == DATA_PARSE_INCOMPLETE) {
		return false;
	} else if(pos == DATA_PARSE_UNKNOWN_DATA) {
		han_debug(PSTR("Unknown data payload:"));
		len = len + serial_readBytes(hanBuffer + len, BUF_SIZE_HAN - len);
		//debugPrint(hanBuffer, 0, len);
		len = 0;
		return false;
	}

	if (pos == DATA_PARSE_INTERMEDIATE_SEGMENT) {
		len = 0;
		return false;
	} else if (pos < 0) {
		printHanReadError(pos);
		len += serial_readBytes(hanBuffer + len, BUF_SIZE_HAN - len);
		while (serial_available()) serial_read(); // Make sure it is all empty, in case we overflowed buffer above
		len = 0;
		return false;
	}

	// Data is valid, clear the rest of the buffer to avoid tainted parsing
	for (int i = pos + ctx.length; i < BUF_SIZE_HAN; i++) {
		hanBuffer[i] = 0x00;
	}

	//AmsData data;
	char* payload = ((char *) (hanBuffer)) + pos;
	if (ctx.type == DATA_TAG_DLMS) {
		han_debug(PSTR("Using application data:"));

		//if (Debug.isActive(RemoteDebug::VERBOSE)) debugPrint((byte*) payload, 0, ctx.length);

		// Rudimentary detector for L&G proprietary format
		if (payload[0] == CosemTypeStructure && payload[2] == CosemTypeArray && payload[1] == payload[3]) {
			//data = LNG(payload, meterState.getMeterType(), &meterConfig, ctx, &Debug);
		} else {
			// TODO: Split IEC6205675 into DataParserKaifa and DataParserObis. This way we can add other means of parsing, for those other proprietary formats
			//data = IEC6205675(payload, meterState.getMeterType(), &meterConfig, ctx);
		}
	} else if(ctx.type == DATA_TAG_DSMR) {
		//data = IEC6205621(payload);
	}

	*out = hanBuffer + pos;
	*size = ctx.length;
	len = 0;
	return true;
}


int16_t Han_Parser::unwrapData(uint8_t *buf, DataParserContext &context) {
	int16_t ret = 0;
	bool doRet = false;
	uint16_t end = BUF_SIZE_HAN;
	uint8_t tag = (*buf);
	uint8_t lastTag = DATA_TAG_NONE;
	while (tag != DATA_TAG_NONE) {
		int16_t curLen = context.length;
		int8_t res = 0;
		switch(tag) {
			case DATA_TAG_HDLC:
				if (hdlcParser == NULL) hdlcParser = new HDLCParser();
				res = hdlcParser->parse(buf, context);
				break;
			case DATA_TAG_MBUS:
				if (mbusParser == NULL) mbusParser =  new MBUSParser();
				res = mbusParser->parse(buf, context);
				break;
			case DATA_TAG_GBT:
				if (gbtParser == NULL) gbtParser = new GBTParser();
				res = gbtParser->parse(buf, context);
				break;
			case DATA_TAG_GCM:
				if (gcmParser == NULL) gcmParser = new GCMParser(encryptionKey, authenticationKey);
				res = gcmParser->parse(buf, context);
				break;
			case DATA_TAG_LLC:
				if (llcParser == NULL) llcParser = new LLCParser();
				res = llcParser->parse(buf, context);
				break;
			case DATA_TAG_DLMS:
				if (dlmsParser == NULL) dlmsParser = new DLMSParser();
				res = dlmsParser->parse(buf, context);
				if (res >= 0) doRet = true;
				break;
			case DATA_TAG_DSMR:
				if (dsmrParser == NULL) dsmrParser = new DSMRParser();
				res = dsmrParser->parse(buf, context, lastTag != DATA_TAG_NONE);
				if (res >= 0) doRet = true;
				break;
			default:
				han_debug(PSTR("Ended up in default case while unwrapping...(tag %02X)"), tag);
				return DATA_PARSE_UNKNOWN_DATA;
		}
		lastTag = tag;
		if (res == DATA_PARSE_INCOMPLETE) {
			return res;
		}
		if (context.length > end) return false;
		if (Debug) {
			switch(tag) {
				case DATA_TAG_HDLC:
					han_debug(PSTR("HDLC frame:"));
					break;
				case DATA_TAG_MBUS:
					han_debug(PSTR("MBUS frame:"));
					break;
				case DATA_TAG_GBT:
					han_debug(PSTR("GBT frame:"));
					break;
				case DATA_TAG_GCM:
					han_debug(PSTR("GCM frame:"));
					break;
				case DATA_TAG_LLC:
					han_debug(PSTR("LLC frame:"));
					break;
				case DATA_TAG_DLMS:
					han_debug(PSTR("DLMS frame:"));
					break;
				case DATA_TAG_DSMR:
					han_debug(PSTR("DSMR frame:"));
					break;
			}
		}
		if (res == DATA_PARSE_FINAL_SEGMENT) {
			if (tag == DATA_TAG_MBUS) {
				res = mbusParser->write(buf, context);
			}
		}

		if (res < 0) {
			return res;
		}
		buf += res;
		end -= res;
		ret += res;

		// If we are ready to return, do that
		if (doRet) {
			context.type = tag;
			return ret;
		}

		// Use start byte of new buffer position as tag for next round in loop
		tag = (*buf);
	}
	han_debug(PSTR("Got to end of unwrap method..."));
	return DATA_PARSE_UNKNOWN_DATA;
}

void Han_Parser::printHanReadError(int16_t pos) {
		switch(pos) {
			case DATA_PARSE_BOUNDRY_FLAG_MISSING:
				han_debug(PSTR("Boundry flag missing"));
				break;
			case DATA_PARSE_HEADER_CHECKSUM_ERROR:
				han_debug(PSTR("Header checksum error"));
				break;
			case DATA_PARSE_FOOTER_CHECKSUM_ERROR:
				han_debug(PSTR("Frame checksum error"));
				break;
			case DATA_PARSE_INCOMPLETE:
				han_debug(PSTR("Received frame is incomplete"));
				break;
			case GCM_AUTH_FAILED:
				han_debug(PSTR("Decrypt authentication failed"));
				break;
			case GCM_ENCRYPTION_KEY_FAILED:
				han_debug(PSTR("Setting decryption key failed"));
				break;
			case GCM_DECRYPT_FAILED:
				han_debug(PSTR("Decryption failed"));
				break;
			case MBUS_FRAME_LENGTH_NOT_EQUAL:
				han_debug(PSTR("Frame length mismatch"));
				break;
			case DATA_PARSE_INTERMEDIATE_SEGMENT:
				han_debug(PSTR("Intermediate segment received"));
				break;
			case DATA_PARSE_UNKNOWN_DATA:
				han_debug(PSTR("Unknown data format %02X"), hanBuffer[0]);
				break;
			default:
				han_debug(PSTR("Unspecified error while reading data: %d"), pos);
				break;
		}
}
