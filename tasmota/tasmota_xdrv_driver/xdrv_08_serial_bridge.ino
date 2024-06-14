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
 * 
 * SBaudrate <baudrate>          - Set baudrate
 * SBaudrate 9600
 * SSerialConfig <config>        - Set serial config
 * SSerialConfig 8N1
 * SSerialBuffer <size>          - Set serial receive buffer
 * SSerialSend "Hello Tiger"     - Send "Hello Tiger\n"
 * SSerialSend                   - Set to text decoding
 * SSerialSend1 "Hello Tiger"    - Send "Hello Tiger\n"
 * SSerialSend2 "Hello Tiger"    - Send "Hello Tiger"
 * SSerialSend3 "Hello Tiger"    - Send "Hello Tiger\f"
 * SSerialSend4                  - Set to binary decoding
 * SSerialSend4 "Hello Tiger"    - Send "Hello Tiger" and set to binary decoding
 * SSerialSend5 "AA004566"       - Send "AA004566" as hex values
 * SSerialSend6 "72,101,108,108" - Send decimals as hex values
 * SSerialMode 0                 - Default mode (Console tee off, 115200bps, 8N1)
 * SSerialMode 1                 - Enable console tee
 * SSerialMode 2                 - Enable dump of serial data in log level 4
 * SSerialMode 3                 - Enable Sonoff WTS01 sensor decoding (ESP32 only)
 * SerialDelimiter 255           - Receive data between 32 and 127 only
 * SerialDelimiter 254           - Receive hexadecimal data
 * SerialDelimiter 128           - Receive no data between 32 and 127
\*********************************************************************************************/

#define XDRV_08                    8
#define HARDWARE_FALLBACK          2

#define USE_SERIAL_BRIDGE_TEE

#ifdef ESP32
#define USE_SERIAL_BRIDGE_WTS01
#endif  // ESP32

#ifdef SERIAL_BRIDGE_BUFFER_SIZE
const uint16_t SERIAL_BRIDGE_BUFSIZE = SERIAL_BRIDGE_BUFFER_SIZE;
#else
#ifdef ESP8266
const uint16_t SERIAL_BRIDGE_BUFSIZE = MIN_INPUT_BUFFER_SIZE;     // 256
#else
const uint16_t SERIAL_BRIDGE_BUFSIZE = INPUT_BUFFER_SIZE;         // 800
#endif  // ESP32
#endif  // SERIAL_BRIDGE_BUFFER_SIZE

enum SerialBridgeModes { SB_NONE, SB_TEE, SB_DUMP, SB_WTS01 };

const char kSerialBridgeCommands[] PROGMEM = "|"  // No prefix
  D_CMND_SSERIALSEND "|" D_CMND_SBAUDRATE "|" D_CMND_SSERIALBUFFER "|" D_CMND_SSERIALCONFIG "|" D_CMND_SSERIALMODE;

void (* const SerialBridgeCommand[])(void) PROGMEM = {
  &CmndSSerialSend, &CmndSBaudrate, &CmndSSerialBuffer, &CmndSSerialConfig, &CmndSSerialMode };

#include <TasmotaSerial.h>
TasmotaSerial *SerialBridgeSerial = nullptr;

char *serial_bridge_buffer = nullptr;

struct {
  uint32_t polling_window;
  int      in_byte_counter = 0;
  float    temperature;
  bool     raw = false;
} SBridge;

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
  if ((SB_TEE == Settings->sserial_mode) && serial_bridge_buffer) {
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
    if (SB_TEE == Settings->sserial_mode) {                                   // CMND_SSERIALSEND9 - Enable logging tee to serialbridge
      static bool serial_bridge_overrun = false;

      if (isprint(serial_in_byte)) {                                             // Any char between 32 and 127
        if (SBridge.in_byte_counter < SERIAL_BRIDGE_BUFSIZE -1) {                // Add char to string if it still fits
          serial_bridge_buffer[SBridge.in_byte_counter++] = serial_in_byte;
        } else {
          serial_bridge_overrun = true;                                          // Signal overrun but continue reading input to flush until '\n' (EOL)
        }
      }
      else if (serial_in_byte == '\n') {
        serial_bridge_buffer[SBridge.in_byte_counter] = 0;                       // Serial data completed
        TasmotaGlobal.seriallog_level = (Settings->seriallog_level < LOG_LEVEL_INFO) ? (uint8_t)LOG_LEVEL_INFO : Settings->seriallog_level;
        if (serial_bridge_overrun) {
          AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_COMMAND "SSerial buffer overrun"));
        } else {
          AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_COMMAND "%s"), serial_bridge_buffer);
          ExecuteCommand(serial_bridge_buffer, SRC_SSERIAL);
        }
        SBridge.in_byte_counter = 0;
        serial_bridge_overrun = false;
        SerialBridgeSerial->flush();
        return;
      }
    } else {
#endif  // USE_SERIAL_BRIDGE_TEE
      SBridge.raw = (Settings->sserial_mode > SB_TEE) ? true : (254 == Settings->serial_delimiter); // SerialDelimiter
      if ((serial_in_byte > 127) && !SBridge.raw) {                              // Discard binary data above 127 if no raw reception allowed
        SBridge.in_byte_counter = 0;
        SerialBridgeSerial->flush();
        return;
      }
      if (serial_in_byte || SBridge.raw) {                                       // Any char between 1 and 127 or any char (0 - 255)
        bool in_byte_is_delimiter =                                              // Char is delimiter when...
          (((Settings->serial_delimiter < 128) && (serial_in_byte == Settings->serial_delimiter)) || // Any char between 1 and 127 and being delimiter
          ((Settings->serial_delimiter == 128) && !isprint(serial_in_byte))) &&  // Any char not between 32 and 127
          !SBridge.raw;                                                          // In raw mode (CMND_SERIALSEND3) there is never a delimiter

        if ((SBridge.in_byte_counter < SERIAL_BRIDGE_BUFSIZE -1) &&              // Add char to string if it still fits and ...
            !in_byte_is_delimiter) {                                             // Char is not a delimiter
          serial_bridge_buffer[SBridge.in_byte_counter++] = serial_in_byte;
        }

        if ((SBridge.in_byte_counter >= SERIAL_BRIDGE_BUFSIZE -1) ||             // Send message when buffer is full or ...
            in_byte_is_delimiter) {                                              // Char is delimiter
          SBridge.polling_window = 0;                                            // Publish now
          break;
        }
      }
      SBridge.polling_window = millis();                                         // Wait for more data
#ifdef USE_SERIAL_BRIDGE_TEE
    }
#endif  // USE_SERIAL_BRIDGE_TEE
  }

#ifdef USE_SERIAL_BRIDGE_TEE
  if (SB_TEE == Settings->sserial_mode) {                                     // CMND_SSERIALSEND9 - Enable logging tee to serialbridge
    return;
  }
#endif  // USE_SERIAL_BRIDGE_TEE

  if (SBridge.in_byte_counter && (millis() > (SBridge.polling_window + SERIAL_POLLING))) {
    serial_bridge_buffer[SBridge.in_byte_counter] = 0;                           // Serial data completed

    if (SB_NONE == Settings->sserial_mode) {
      bool assume_json = (!SBridge.raw && (serial_bridge_buffer[0] == '{'));
      TasmotaGlobal.serial_skip++;                                               // SetOption35  Skip number of serial messages received (default 0)
      if (TasmotaGlobal.serial_skip > Settings->param[P_SERIAL_SKIP]) {          // Handle intermediate changes to SetOption35
        TasmotaGlobal.serial_skip = 0;
        Response_P(PSTR("{\"" D_JSON_SSERIALRECEIVED "\":"));
        if (assume_json) {
          ResponseAppend_P(serial_bridge_buffer);
        } else {
          ResponseAppend_P(PSTR("\""));
          if (SBridge.raw) {
            ResponseAppend_P(PSTR("%*_H"), SBridge.in_byte_counter, serial_bridge_buffer);
          } else {
            ResponseAppend_P(EscapeJSONString(serial_bridge_buffer).c_str());
          }
          ResponseAppend_P(PSTR("\""));
        }
        ResponseJsonEnd();
        if (Settings->flag6.mqtt_disable_publish ) {  // SetOption147 - If it is activated, Tasmota will not publish SSerialReceived MQTT messages, but it will proccess event trigger rules
          XdrvRulesProcess(0);
        } else {
          MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_SSERIALRECEIVED));
        }
      }
    }

    if (Settings->sserial_mode > SB_TEE) {
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SBR: Rcvd '%*_H'"), SBridge.in_byte_counter, serial_bridge_buffer);
    }

#ifdef USE_SERIAL_BRIDGE_WTS01
    // Sonoff WTS01 9600bps, 8N1 datagram every second
    //  0  1  2  3  4  5  6  7  8
    // 55 01 01 04 01 11 16 12 95
    // header            T  Td Ck  - T = Temperature, Td = Temperature decimal, Ck = Checksum
    if (SB_WTS01 == Settings->sserial_mode) {
      if (9 == SBridge.in_byte_counter) {
        uint32_t *header = (uint32_t*)serial_bridge_buffer;
        if (0x04010155 == *header) {
          SBridge.temperature = (float)serial_bridge_buffer[6] + ((float)serial_bridge_buffer[7] / 100.0f);
        }
      }
    }
#endif  // USE_SERIAL_BRIDGE_WTS01

    SBridge.in_byte_counter = 0;
  }
}

/********************************************************************************************/

void SerialBridgeInit(void) {
  if (PinUsed(GPIO_SBR_RX) || PinUsed(GPIO_SBR_TX)) {
//    SerialBridgeSerial = new TasmotaSerial(Pin(GPIO_SBR_RX), Pin(GPIO_SBR_TX), HARDWARE_FALLBACK);  // Default TM_SERIAL_BUFFER_SIZE (=64) size
    SerialBridgeSerial = new TasmotaSerial(Pin(GPIO_SBR_RX), Pin(GPIO_SBR_TX), HARDWARE_FALLBACK, 0, MIN_INPUT_BUFFER_SIZE);  // 256
    if (SetSSerialBegin()) {
      if (SerialBridgeSerial->hardwareSerial()) {
        ClaimSerial();
        serial_bridge_buffer = TasmotaGlobal.serial_in_buffer;  // Use idle serial buffer to save RAM
      } else {
        serial_bridge_buffer = (char*)(malloc(SERIAL_BRIDGE_BUFSIZE));
      }
#ifdef ESP32
      AddLog(LOG_LEVEL_DEBUG, PSTR("SBR: Serial UART%d"), SerialBridgeSerial->getUart());
#endif
      SerialBridgeSerial->flush();
      SerialBridgePrintf("\r\n");
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndSSerialSend(void) {
  if (Settings->sserial_mode > SB_DUMP) { return; }                         // Not supported in receive only mode
  if (XdrvMailbox.index > 9) { XdrvMailbox.index -= 10; }
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 6)) {
    SBridge.raw = (XdrvMailbox.index > 3);
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

void CmndSSerialMode(void) {
  // SSerialMode 0  - Default mode (Tee off, 115200bps, 8N1)
  // SSerialMode 1  - Toggle console tee
  // SSerialMode 2  - Dump hex data to log level 4 for analysis
  // SSerialMode 3  - Sonoff WTS01 decode
  if (XdrvMailbox.payload >= 0) {
    switch (XdrvMailbox.payload) {
      case SB_NONE:
        Settings->sserial_mode = XdrvMailbox.payload;
  //      Settings->serial_delimiter = 255;
        Settings->sbaudrate = 115200 / 300;        // 115200bps
        SetSSerialConfig(3);                       // 8N1
        break;
#ifdef USE_SERIAL_BRIDGE_TEE
      case SB_TEE:
        Settings->sserial_mode = XdrvMailbox.payload;
        break;
#endif  // USE_SERIAL_BRIDGE_TEE
      case SB_DUMP:
        Settings->sserial_mode = XdrvMailbox.payload;
        break;
#ifdef USE_SERIAL_BRIDGE_WTS01
      case SB_WTS01:
        Settings->sserial_mode = XdrvMailbox.payload;
        Settings->sbaudrate = 9600 / 300;          // 9600bps
        SetSSerialConfig(3);                       // 8N1
        break;
#endif  // USE_SERIAL_BRIDGE_WTS01
      default:
        return;                                    // Not supported
    }
  }
  ResponseCmndNumber(Settings->sserial_mode);
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void SerialBridgeShow(bool json) {
#ifdef USE_SERIAL_BRIDGE_WTS01
  if (SB_WTS01 == Settings->sserial_mode) {
    float temperature = ConvertTemp(SBridge.temperature);
    if (json) {
      ResponseAppend_P(PSTR(",\"WTS01\":{\"" D_JSON_TEMPERATURE "\":%*_f}"), Settings->flag2.temperature_resolution, &temperature);
  #ifdef USE_DOMOTICZ
      if (0 == TasmotaGlobal.tele_period) { DomoticzFloatSensor(DZ_TEMP, temperature); }
  #endif  // USE_DOMOTICZ
  #ifdef USE_KNX
      if (0 == TasmotaGlobal.tele_period) { KnxSensor(KNX_TEMPERATURE, temperature); }
  #endif  // USE_KNX
  #ifdef USE_WEBSERVER
    } else {
      WSContentSend_Temp("WTS01", temperature);
  #endif  // USE_WEBSERVER
    }
  }
#endif  // USE_SERIAL_BRIDGE_WTS01
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
      case FUNC_JSON_APPEND:
        SerialBridgeShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        SerialBridgeShow(0);
        break;
#endif  // USE_WEBSERVER
      case FUNC_COMMAND:
        result = DecodeCommand(kSerialBridgeCommands, SerialBridgeCommand);
        break;
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif // USE_SERIAL_BRIDGE
