/*
  xdrv_73_9_lora.ino - LoRa support for Tasmota

  SPDX-FileCopyrightText: 2024 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_SPI
#ifdef USE_SPI_LORA
/*********************************************************************************************\
 * LoRa
\*********************************************************************************************/

#define XDRV_73                 73

/*********************************************************************************************/

void LoraInput(void) {
  if (!Lora.Available()) { return; }

  char data[LORA_MAX_PACKET_LENGTH] = { 0 };
  int packet_size = Lora.Receive(data);
  if (!packet_size) { return; }

  bool raw = Lora.raw;
  // Set raw mode if zeroes within data
  for (uint32_t i = 0; i < packet_size; i++) {
    if (0 == data[i]) {
      raw = true;
      break;
    }
  }
  bool assume_json = (!raw && (data[0] == '{'));
  Response_P(PSTR("{\"LoRaReceived\":"));
  if (assume_json) {
    ResponseAppend_P(data);
  } else {
    ResponseAppend_P(PSTR("\""));
    if (raw) {
      ResponseAppend_P(PSTR("%*_H"), packet_size, data);
    } else {
      ResponseAppend_P(EscapeJSONString(data).c_str());
    }
    ResponseAppend_P(PSTR("\""));
  }
  ResponseAppend_P(PSTR(",\"RSSI\":%1_f,\"SNR\":%1_f}"), &Lora.rssi, &Lora.snr);

  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR("LoRaReceived"));
}

void LoraInit(void) {
  if ((SPI_MOSI_MISO == TasmotaGlobal.spi_enabled) &&
      (PinUsed(GPIO_LORA_CS)) && (PinUsed(GPIO_LORA_RST))) {
#ifdef ESP8266
    SPI.begin();
#endif // ESP8266
#ifdef ESP32
    SPI.begin(Pin(GPIO_SPI_CLK), Pin(GPIO_SPI_MISO), Pin(GPIO_SPI_MOSI), -1);
#endif // ESP32

    Lora.frequency = 868.0;       // MHz
    Lora.bandwidth = 125.0;       // kHz
    Lora.spreading_factor = 9;
    Lora.coding_rate = 7;
    Lora.sync_word = 0x12;
    Lora.output_power = 10;       // dBm
    Lora.preamble_length = 8;     // symbols
    Lora.current_limit = 60.0;    // mA (Overcurrent Protection (OCP))
    Lora.implicit_header = 0;     // explicit
    Lora.crc_bytes = 2;           // bytes

    Lora.enableInterrupt = true;

    char hardware[20];
    if (false) {
    }
#ifdef USE_LORA_SX127X
    else if (PinUsed(GPIO_LORA_DI0)) {
      // SX1276, RFM95W
      if (LoraInitSx127x()) {
        Lora.Config = &LoraConfigSx127x;
        Lora.Available = &LoraAvailableSx127x;
        Lora.Receive = &LoraReceiveSx127x;
        Lora.Send = &LoraSendSx127x;
        strcpy_P(hardware, PSTR("SX127x"));
        Lora.present = true;
      }
    }
#endif  // USE_LORA_SX127X
#ifdef USE_LORA_SX126X
    else if (LoraInitSx126x()) {
      // SX1262, LilyGoT3S3
      Lora.Config = &LoraConfigSx126x;
      Lora.Available = &LoraAvailableSx126x;
      Lora.Receive = &LoraReceiveSx126x;
      Lora.Send = &LoraSendSx126x;
      strcpy_P(hardware, PSTR("SX126x"));
      Lora.present = true;
    }
#endif  // USE_LORA_SX126X
    else {
      strcpy_P(hardware, PSTR("Not"));
    }
    AddLog(LOG_LEVEL_DEBUG, PSTR("LOR: %s initialized"), hardware);
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

#define D_CMND_LORASEND   "Send"
#define D_CMND_LORACONFIG "Config"

const char kLoraCommands[] PROGMEM = "LoRa|"  // Prefix
  D_CMND_LORASEND "|" D_CMND_LORACONFIG;

void (* const LoraCommand[])(void) PROGMEM = {
  &CmndLoraSend, &CmndLoraConfig };

void CmndLoraSend(void) {
  // LoRaSend "Hello Tiger"     - Send "Hello Tiger\n"
  // LoRaSend1 "Hello Tiger"    - Send "Hello Tiger\n"
  // LoRaSend2 "Hello Tiger"    - Send "Hello Tiger"
  // LoRaSend3 "Hello Tiger"    - Send "Hello Tiger\f"
  // LoRaSend4 = LoraSend2
  // LoRaSend5 "AA004566"       - Send "AA004566" as hex values
  // LoRaSend6 "72,101,108,108" - Send decimals as hex values
//  if (XdrvMailbox.index > 9) { XdrvMailbox.index -= 10; }                   // Allows leading spaces (not supported - See support_command/CommandHandler)
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 6)) {
    Lora.raw = (XdrvMailbox.index > 3);                                     // Global flag set even without data
    if (XdrvMailbox.data_len > 0) {
      char data[LORA_MAX_PACKET_LENGTH];
      uint32_t len = (XdrvMailbox.data_len < LORA_MAX_PACKET_LENGTH -1) ? XdrvMailbox.data_len : LORA_MAX_PACKET_LENGTH -2;
      if (1 == XdrvMailbox.index) {                                         // "Hello Tiger\n"
        memcpy(data, XdrvMailbox.data, len);
        data[len++] = '\n';
      }
      else if ((2 == XdrvMailbox.index) || (4 == XdrvMailbox.index)) {      // "Hello Tiger" or "A0"
        memcpy(data, XdrvMailbox.data, len);
      }
      else if (3 == XdrvMailbox.index) {                                    // "Hello\f"
        Unescape(XdrvMailbox.data, &len);
        memcpy(data, XdrvMailbox.data, len);
      }
      else if (5 == XdrvMailbox.index) {                                    // "AA004566" as hex values
        char *p;
        char stemp[3];

        char *codes = RemoveSpace(XdrvMailbox.data);
        int size = strlen(XdrvMailbox.data);

        len = 0;
        while (size > 1) {
          strlcpy(stemp, codes, sizeof(stemp));
          data[len++] = strtol(stemp, &p, 16);
          if (len > LORA_MAX_PACKET_LENGTH -2) { break; }
          size -= 2;
          codes += 2;
        }
      }
      else if (6 == XdrvMailbox.index) {                                    // "72,101,108,108"
        char *p;
        uint8_t code;
        char *values = XdrvMailbox.data;
        len = 0;
        for (char* str = strtok_r(values, ",", &p); str; str = strtok_r(nullptr, ",", &p)) {
          data[len++] = (uint8_t)atoi(str);
          if (len > LORA_MAX_PACKET_LENGTH -2) { break; }
        }
      }
      else {
        len = 0;
      }
      if (len) {
        Lora.Send(data, len);
      }
      ResponseCmndDone();
    }
  }
}

void CmndLoraConfig(void) {
  // LoRaConfig                                       - Show all parameters
  // LoRaConfig {"Frequency":868.0,"Bandwidth":125.0} - Enter float parameters
  // LoRaConfig {"SyncWord":18}                       - Enter decimal parameter (=0x12)
  if (XdrvMailbox.data_len > 0) {
    JsonParser parser(XdrvMailbox.data);
    JsonParserObject root = parser.getRootObject();
    if (root) { 
      Lora.frequency = root.getFloat(PSTR(D_JSON_FREQUENCY), Lora.frequency);
      Lora.bandwidth = root.getFloat(PSTR(D_JSON_BANDWIDTH), Lora.bandwidth);
      Lora.spreading_factor = root.getUInt(PSTR(D_JSON_SPREADING_FACTOR), Lora.spreading_factor);
      Lora.coding_rate = root.getUInt(PSTR(D_JSON_CODINGRATE4), Lora.coding_rate);
      Lora.sync_word = root.getUInt(PSTR(D_JSON_SYNCWORD), Lora.sync_word);
      Lora.output_power = root.getUInt(PSTR(D_JSON_OUTPUT_POWER), Lora.output_power);
      Lora.preamble_length = root.getUInt(PSTR(D_JSON_PREAMBLE_LENGTH), Lora.preamble_length);
      Lora.current_limit = root.getFloat(PSTR(D_JSON_CURRENT_LIMIT), Lora.current_limit);
      Lora.implicit_header = root.getUInt(PSTR(D_JSON_IMPLICIT_HEADER), Lora.implicit_header);
      Lora.crc_bytes = root.getUInt(PSTR(D_JSON_CRC_BYTES), Lora.crc_bytes);
      Lora.Config();
    }
  }
  ResponseCmnd();  // {"LoRaConfig":
  ResponseAppend_P(PSTR("{\"" D_JSON_FREQUENCY "\":%1_f"), &Lora.frequency);             // xxx.x MHz
  ResponseAppend_P(PSTR(",\"" D_JSON_BANDWIDTH "\":%1_f"), &Lora.bandwidth);             // xxx.x kHz
  ResponseAppend_P(PSTR(",\"" D_JSON_SPREADING_FACTOR "\":%d"), Lora.spreading_factor);
  ResponseAppend_P(PSTR(",\"" D_JSON_CODINGRATE4 "\":%d"), Lora.coding_rate);
  ResponseAppend_P(PSTR(",\"" D_JSON_SYNCWORD "\":%d"), Lora.sync_word);
  ResponseAppend_P(PSTR(",\"" D_JSON_OUTPUT_POWER "\":%d"), Lora.output_power);          // dBm
  ResponseAppend_P(PSTR(",\"" D_JSON_PREAMBLE_LENGTH "\":%d"), Lora.preamble_length);    // symbols
  ResponseAppend_P(PSTR(",\"" D_JSON_CURRENT_LIMIT "\":%1_f"), &Lora.current_limit);     // xx.x mA (Overcurrent Protection - OCP)
  ResponseAppend_P(PSTR(",\"" D_JSON_IMPLICIT_HEADER "\":%d"), Lora.implicit_header);    // 0 = explicit
  ResponseAppend_P(PSTR(",\"" D_JSON_CRC_BYTES "\":%d}}"), Lora.crc_bytes);              // bytes
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv73(uint32_t function) {
  bool result = false;

  if (FUNC_INIT == function) {
    LoraInit();
  }
  else if (Lora.present) {
    switch (function) {
      case FUNC_LOOP:
      case FUNC_SLEEP_LOOP:
        LoraInput();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kLoraCommands, LoraCommand);
        break;
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif  // USE_SPI_LORA
#endif  // USE_SPI
