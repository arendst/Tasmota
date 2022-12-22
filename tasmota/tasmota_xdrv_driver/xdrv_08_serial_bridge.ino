/*
  xdrv_08_serial_bridge.ino - serial bridge support for Tasmota

  Copyright (C) 2021  Theo Arends and Dániel Zoltán Tolnai

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

#ifdef USE_SERIAL_BRIDGE
/*********************************************************************************************\
 * Serial Bridge using Software Serial library (TasmotaSerial)
\*********************************************************************************************/

#define XDRV_08                    8
#define HARDWARE_FALLBACK          2

#define USE_SERIAL_BRIDGE_TEE

#ifdef SERIAL_BRIDGE_BUFFER_SIZE
const uint16_t SERIAL_BRIDGE_BUFSIZE = SERIAL_BRIDGE_BUFFER_SIZE;
#else
#ifdef ESP8266
const uint16_t SERIAL_BRIDGE_BUFSIZE = MIN_INPUT_BUFFER_SIZE;     // 256
#else
const uint16_t SERIAL_BRIDGE_BUFSIZE = INPUT_BUFFER_SIZE;         // 800
#endif  // ESP32
#endif  // SERIAL_BRIDGE_BUFFER_SIZE

const char kSerialBridgeCommands[] PROGMEM = "|"  // No prefix
  D_CMND_SSERIALSEND "|" D_CMND_SBAUDRATE "|" D_CMND_SSERIALBUFFER "|" D_CMND_SSERIALCONFIG;

void (* const SerialBridgeCommand[])(void) PROGMEM = {
  &CmndSSerialSend, &CmndSBaudrate, &CmndSSerialBuffer, &CmndSSerialConfig };

#include <TasmotaSerial.h>
TasmotaSerial *SerialBridgeSerial = nullptr;

unsigned long serial_bridge_polling_window = 0;
char *serial_bridge_buffer = nullptr;
int serial_bridge_in_byte_counter = 0;
bool serial_bridge_raw = false;

/********************************************************************************************/

bool SetSSerialBegin(void) {
  return SerialBridgeSerial->begin(Settings->sbaudrate * 300, ConvertSerialConfig(Settings->sserial_config));  // Reinitialize serial port with new baud rate
}

void SetSSerialConfig(uint32_t serial_config) {
  if (serial_config > TS_SERIAL_8O2) {
    serial_config = TS_SERIAL_8N1;
  }
  if (serial_config != Settings->sserial_config) {
    Settings->sserial_config = serial_config;
    SetSSerialBegin();
  }
}

void SerialBridgePrintf(PGM_P formatP, ...) {
#ifdef USE_SERIAL_BRIDGE_TEE
  if (Settings->sbflag1.serbridge_console && serial_bridge_buffer) {
    va_list arg;
    va_start(arg, formatP);
    char* data = ext_vsnprintf_malloc_P(formatP, arg);
    va_end(arg);
    if (data == nullptr) { return; }

//    SerialBridgeSerial->printf(data);  // This resolves "MqttClientMask":"DVES_%06X" into "DVES_000002"
    SerialBridgeSerial->print(data);  // This does not resolve "DVES_%06X"
    free(data);
  }
#endif  // USE_SERIAL_BRIDGE_TEE
}

/********************************************************************************************/

void SerialBridgeInput(void) {
  while (SerialBridgeSerial->available()) {
    yield();
    uint8_t serial_in_byte = SerialBridgeSerial->read();

#ifdef USE_SERIAL_BRIDGE_TEE
    if (Settings->sbflag1.serbridge_console) {
      static bool serial_bridge_overrun = false;

      if (isprint(serial_in_byte)) {                                             // Any char between 32 and 127
        if (serial_bridge_in_byte_counter < SERIAL_BRIDGE_BUFSIZE -1) {          // Add char to string if it still fits
          serial_bridge_buffer[serial_bridge_in_byte_counter++] = serial_in_byte;
        } else {
          serial_bridge_overrun = true;                                          // Signal overrun but continue reading input to flush until '\n' (EOL)
        }
      }
      else if (serial_in_byte == '\n') {
        serial_bridge_buffer[serial_bridge_in_byte_counter] = 0;                 // Serial data completed
        TasmotaGlobal.seriallog_level = (Settings->seriallog_level < LOG_LEVEL_INFO) ? (uint8_t)LOG_LEVEL_INFO : Settings->seriallog_level;
        if (serial_bridge_overrun) {
          AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_COMMAND "SSerial buffer overrun"));
        } else {
          AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_COMMAND "%s"), serial_bridge_buffer);
          ExecuteCommand(serial_bridge_buffer, SRC_SSERIAL);
        }
        serial_bridge_in_byte_counter = 0;
        serial_bridge_overrun = false;
        SerialBridgeSerial->flush();
        return;
      }
    } else {
#endif  // USE_SERIAL_BRIDGE_TEE
      serial_bridge_raw = (254 == Settings->serial_delimiter);
      if ((serial_in_byte > 127) && !serial_bridge_raw) {                        // Discard binary data above 127 if no raw reception allowed
        serial_bridge_in_byte_counter = 0;
        SerialBridgeSerial->flush();
        return;
      }
      if (serial_in_byte || serial_bridge_raw) {                                 // Any char between 1 and 127 or any char (0 - 255)
        bool in_byte_is_delimiter =                                              // Char is delimiter when...
          (((Settings->serial_delimiter < 128) && (serial_in_byte == Settings->serial_delimiter)) || // Any char between 1 and 127 and being delimiter
          ((Settings->serial_delimiter == 128) && !isprint(serial_in_byte))) &&  // Any char not between 32 and 127
          !serial_bridge_raw;                                                    // In raw mode (CMND_SERIALSEND3) there is never a delimiter

        if ((serial_bridge_in_byte_counter < SERIAL_BRIDGE_BUFSIZE -1) &&        // Add char to string if it still fits and ...
            !in_byte_is_delimiter) {                                             // Char is not a delimiter
          serial_bridge_buffer[serial_bridge_in_byte_counter++] = serial_in_byte;
        }

        if ((serial_bridge_in_byte_counter >= SERIAL_BRIDGE_BUFSIZE -1) ||       // Send message when buffer is full or ...
            in_byte_is_delimiter) {                                              // Char is delimiter
          serial_bridge_polling_window = 0;                                      // Publish now
          break;
        }
      }
      serial_bridge_polling_window = millis();                                   // Wait for more data
#ifdef USE_SERIAL_BRIDGE_TEE
    }
#endif  // USE_SERIAL_BRIDGE_TEE
  }

#ifdef USE_SERIAL_BRIDGE_TEE
  if (Settings->sbflag1.serbridge_console) {
    return;
  }
#endif  // USE_SERIAL_BRIDGE_TEE

  if (serial_bridge_in_byte_counter && (millis() > (serial_bridge_polling_window + SERIAL_POLLING))) {
    serial_bridge_buffer[serial_bridge_in_byte_counter] = 0;                   // Serial data completed
    bool assume_json = (!serial_bridge_raw && (serial_bridge_buffer[0] == '{'));

    TasmotaGlobal.serial_skip++;                       // SetOption35  Skip number of serial messages received (default 0)
    if (TasmotaGlobal.serial_skip > Settings->param[P_SERIAL_SKIP]) {  // Handle intermediate changes to SetOption35
      TasmotaGlobal.serial_skip = 0;

      Response_P(PSTR("{\"" D_JSON_SSERIALRECEIVED "\":"));
      if (assume_json) {
        ResponseAppend_P(serial_bridge_buffer);
      } else {
        ResponseAppend_P(PSTR("\""));
        if (serial_bridge_raw) {
          ResponseAppend_P(PSTR("%*_H"), serial_bridge_in_byte_counter, serial_bridge_buffer);
        } else {
          ResponseAppend_P(EscapeJSONString(serial_bridge_buffer).c_str());
        }
        ResponseAppend_P(PSTR("\""));
      }
      ResponseJsonEnd();

      if (Settings->flag6.mqtt_disable_sserialrec ) {  // SetOption147  If it is activated, Tasmota will not publish SSerialReceived MQTT messages, but it will proccess event trigger rules
        XdrvRulesProcess(0);
      } else {
        MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_SSERIALRECEIVED));
      }
    }

    serial_bridge_in_byte_counter = 0;
  }
}

/********************************************************************************************/

void SerialBridgeInit(void) {
  if (PinUsed(GPIO_SBR_RX) && PinUsed(GPIO_SBR_TX)) {
//    SerialBridgeSerial = new TasmotaSerial(Pin(GPIO_SBR_RX), Pin(GPIO_SBR_TX), HARDWARE_FALLBACK);  // Default TM_SERIAL_BUFFER_SIZE (=64) size
    SerialBridgeSerial = new TasmotaSerial(Pin(GPIO_SBR_RX), Pin(GPIO_SBR_TX), HARDWARE_FALLBACK, 0, MIN_INPUT_BUFFER_SIZE);  // 256
    if (SetSSerialBegin()) {
      if (SerialBridgeSerial->hardwareSerial()) {
        ClaimSerial();
        serial_bridge_buffer = TasmotaGlobal.serial_in_buffer;  // Use idle serial buffer to save RAM
      } else {
        serial_bridge_buffer = (char*)(malloc(SERIAL_BRIDGE_BUFSIZE));
      }
      SerialBridgeSerial->flush();
      SerialBridgePrintf("\r\n");
#ifdef ESP32
      AddLog(LOG_LEVEL_DEBUG, PSTR("SBR: Serial UART%d"), SerialBridgeSerial->getUart());
#endif
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndSSerialSend(void) {
  if (XdrvMailbox.index > 9) { XdrvMailbox.index -= 10; }
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 6)) {
    serial_bridge_raw = (XdrvMailbox.index > 3);
    Settings->sbflag1.serbridge_console = 0;                                // Disable console Tee
    if (XdrvMailbox.data_len > 0) {
      if (1 == XdrvMailbox.index) {
        SerialBridgeSerial->write(XdrvMailbox.data, XdrvMailbox.data_len);  // "Hello Tiger"
        SerialBridgeSerial->write("\n");                                    // "\n"
      }
      else if ((2 == XdrvMailbox.index) || (4 == XdrvMailbox.index)) {
        SerialBridgeSerial->write(XdrvMailbox.data, XdrvMailbox.data_len);  // "Hello Tiger" or "A0"
      }
      else if (3 == XdrvMailbox.index) {                                    // "Hello\f"
        SerialBridgeSerial->write(Unescape(XdrvMailbox.data, &XdrvMailbox.data_len), XdrvMailbox.data_len);
      }
      else if (5 == XdrvMailbox.index) {
        char *p;
        char stemp[3];
        uint8_t code;

        char *codes = RemoveSpace(XdrvMailbox.data);
        int size = strlen(XdrvMailbox.data);

        while (size > 1) {
          strlcpy(stemp, codes, sizeof(stemp));
          code = strtol(stemp, &p, 16);
          SerialBridgeSerial->write(code);                                  // "AA004566" as hex values
          size -= 2;
          codes += 2;
        }
      }
      else if (6 == XdrvMailbox.index) {
        char *p;
        uint8_t code;
        char *values = XdrvMailbox.data;
        for (char* str = strtok_r(values, ",", &p); str; str = strtok_r(nullptr, ",", &p)) {
          code = (uint8_t)atoi(str);
          SerialBridgeSerial->write(code);                                  // "72,101,108,108"
        }
      }
      ResponseCmndDone();
    }
  }
#ifdef USE_SERIAL_BRIDGE_TEE
  if (9 == XdrvMailbox.index) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
      Settings->sbflag1.serbridge_console = XdrvMailbox.payload;
    }
    ResponseCmndStateText(Settings->sbflag1.serbridge_console);
  }
#endif  // USE_SERIAL_BRIDGE_TEE
}

void CmndSBaudrate(void) {
  if (XdrvMailbox.payload >= 300) {
    XdrvMailbox.payload /= 300;  // Make it a valid baudrate
    Settings->sbaudrate = XdrvMailbox.payload;
    SetSSerialBegin();
  }
  ResponseCmndNumber(Settings->sbaudrate * 300);
}

void CmndSSerialBuffer(void) {
  // Allow non-pesistent serial receive buffer size change
  if (SerialBridgeSerial->hardwareSerial()) {
    // between MIN_INPUT_BUFFER_SIZE and MAX_INPUT_BUFFER_SIZE characters
    CmndSerialBuffer();
  } else {
    // ESP8266 (software serial): between MIN_INPUT_BUFFER_SIZE and SERIAL_BRIDGE_BUFSIZE characters
    // ESP32 (hardware serial only): between MIN_INPUT_BUFFER_SIZE and MAX_INPUT_BUFFER_SIZE characters
    if (XdrvMailbox.data_len > 0) {
      size_t size = XdrvMailbox.payload;
      if (XdrvMailbox.payload < MIN_INPUT_BUFFER_SIZE) {
        size = MIN_INPUT_BUFFER_SIZE;          // 256 / 256
      }
      else if (XdrvMailbox.payload > SERIAL_BRIDGE_BUFSIZE) {
        size = SERIAL_BRIDGE_BUFSIZE;      // 256 / 800
      }
      SerialBridgeSerial->setRxBufferSize(size);
    }
    ResponseCmndNumber(SerialBridgeSerial->getRxBufferSize());
  }
}

void CmndSSerialConfig(void) {
  // See TasmotaSerialConfig for possible options
  // SSerialConfig 0..23 where 3 equals 8N1
  // SSerialConfig 8N1

  if (XdrvMailbox.data_len > 0) {
    if (XdrvMailbox.data_len < 3) {                    // Use 0..23 as serial config option
      if ((XdrvMailbox.payload >= TS_SERIAL_5N1) && (XdrvMailbox.payload <= TS_SERIAL_8O2)) {
        SetSSerialConfig(XdrvMailbox.payload);
      }
    }
    else if ((XdrvMailbox.payload >= 5) && (XdrvMailbox.payload <= 8)) {
      int8_t serial_config = ParseSerialConfig(XdrvMailbox.data);
      if (serial_config >= 0) {
        SetSSerialConfig(serial_config);
      }
    }
  }
  ResponseCmndChar(GetSerialConfig(Settings->sserial_config).c_str());
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv08(uint32_t function) {
  bool result = false;

  if (FUNC_PRE_INIT == function) {
    SerialBridgeInit();
  }
  else if (serial_bridge_buffer) {
    switch (function) {
      case FUNC_LOOP:
      case FUNC_SLEEP_LOOP:
        SerialBridgeInput();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kSerialBridgeCommands, SerialBridgeCommand);
        break;
    }
  }
  return result;
}

#endif // USE_SERIAL_BRIDGE
