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

void LoraDefaults(uint32_t region = TAS_LORA_REGION_EU868) {
  Lora->settings.region           = region;
  switch (region) {
    case TAS_LORA_REGION_AU915:
      Lora->settings.frequency    = TAS_LORA_AU915_FREQUENCY;
      break;
    default:  // TAS_LORA_REGION_EU868
      Lora->settings.frequency    = TAS_LORA_FREQUENCY;
  }  
  Lora->settings.bandwidth        = TAS_LORA_BANDWIDTH;
  Lora->settings.spreading_factor = TAS_LORA_SPREADING_FACTOR;
  Lora->settings.coding_rate      = TAS_LORA_CODING_RATE;
  Lora->settings.sync_word        = TAS_LORA_SYNC_WORD;
  Lora->settings.output_power     = TAS_LORA_OUTPUT_POWER;
  Lora->settings.preamble_length  = TAS_LORA_PREAMBLE_LENGTH;
  Lora->settings.current_limit    = TAS_LORA_CURRENT_LIMIT;
  Lora->settings.implicit_header  = TAS_LORA_HEADER;
  Lora->settings.crc_bytes        = TAS_LORA_CRC_BYTES;
}

/*********************************************************************************************/

void LoraSettings2Json(bool show = false) {
  if (show) {
    char region[8];
    ResponseAppend_P(PSTR("\"" D_JSON_REGION "\":\"%s\""), GetTextIndexed(region, sizeof(region), Lora->settings.region, kLoraRegions));
  } else {
    ResponseAppend_P(PSTR("\"" D_JSON_REGION "\":%d"), Lora->settings.region);      // enum 0 = EU868, 1 = AU915
  }
#ifdef USE_LORAWAN_BRIDGE
  if (show && (bitRead(Lora->settings.flags, TAS_LORA_FLAG_BRIDGE_ENABLED))) {
    LoRaWanRadioInfo_t Rx1Info;
    LoraWanRadioInfo(TAS_LORAWAN_RADIO_RX1, &Rx1Info, LoraWanFrequencyToChannel());  // Get Rx1Info with values used for RX1 transmit window. (Region specific, calculated from Uplink radio settings)
    LoRaWanRadioInfo_t Rx2Info;
    LoraWanRadioInfo(TAS_LORAWAN_RADIO_RX2, &Rx2Info, LoraWanFrequencyToChannel());  // Get Rx2Info 
    ResponseAppend_P(PSTR(",\"" D_JSON_FREQUENCY "\":[%1_f,%1_f,%1_f]"), &Lora->settings.frequency, &Rx1Info.frequency, &Rx2Info.frequency);  // xxx.x MHz
    ResponseAppend_P(PSTR(",\"" D_JSON_BANDWIDTH "\":[%1_f,%1_f,%1_f]"), &Lora->settings.bandwidth, &Rx1Info.bandwidth, &Rx2Info.bandwidth);  // xxx.x kHz
    ResponseAppend_P(PSTR(",\"" D_JSON_SPREADING_FACTOR "\":[%d,%d,%d]"), Lora->settings.spreading_factor, Rx1Info.spreading_factor, Rx2Info.spreading_factor);
  } else
#endif  // USE_LORAWAN_BRIDGE
  {
    ResponseAppend_P(PSTR(",\"" D_JSON_FREQUENCY "\":%1_f"), &Lora->settings.frequency);           // xxx.x MHz
    ResponseAppend_P(PSTR(",\"" D_JSON_BANDWIDTH "\":%1_f"), &Lora->settings.bandwidth);           // xxx.x kHz
    ResponseAppend_P(PSTR(",\"" D_JSON_SPREADING_FACTOR "\":%d"), Lora->settings.spreading_factor);
  }
  ResponseAppend_P(PSTR(",\"" D_JSON_CODINGRATE4 "\":%d"), Lora->settings.coding_rate);
  ResponseAppend_P(PSTR(",\"" D_JSON_SYNCWORD "\":%d"), Lora->settings.sync_word);
  ResponseAppend_P(PSTR(",\"" D_JSON_OUTPUT_POWER "\":%d"), Lora->settings.output_power);          // dBm
  ResponseAppend_P(PSTR(",\"" D_JSON_PREAMBLE_LENGTH "\":%d"), Lora->settings.preamble_length);    // symbols
  ResponseAppend_P(PSTR(",\"" D_JSON_CURRENT_LIMIT "\":%1_f"), &Lora->settings.current_limit);     // xx.x mA (Overcurrent Protection - OCP)
  ResponseAppend_P(PSTR(",\"" D_JSON_IMPLICIT_HEADER "\":%d"), Lora->settings.implicit_header);    // 0 = explicit
  ResponseAppend_P(PSTR(",\"" D_JSON_CRC_BYTES "\":%d"), Lora->settings.crc_bytes);                // bytes
}

void LoraJson2Settings(JsonParserObject root) {
  Lora->settings.region = root.getUInt(PSTR(D_JSON_REGION), Lora->settings.region);
  Lora->settings.frequency = root.getFloat(PSTR(D_JSON_FREQUENCY), Lora->settings.frequency);
  Lora->settings.bandwidth = root.getFloat(PSTR(D_JSON_BANDWIDTH), Lora->settings.bandwidth);
  Lora->settings.spreading_factor = root.getUInt(PSTR(D_JSON_SPREADING_FACTOR), Lora->settings.spreading_factor);
  Lora->settings.coding_rate = root.getUInt(PSTR(D_JSON_CODINGRATE4), Lora->settings.coding_rate);
  Lora->settings.sync_word = root.getUInt(PSTR(D_JSON_SYNCWORD), Lora->settings.sync_word);
  Lora->settings.output_power = root.getUInt(PSTR(D_JSON_OUTPUT_POWER), Lora->settings.output_power);
  Lora->settings.preamble_length = root.getUInt(PSTR(D_JSON_PREAMBLE_LENGTH), Lora->settings.preamble_length);
  Lora->settings.current_limit = root.getFloat(PSTR(D_JSON_CURRENT_LIMIT), Lora->settings.current_limit);
  Lora->settings.implicit_header = root.getUInt(PSTR(D_JSON_IMPLICIT_HEADER), Lora->settings.implicit_header);
  Lora->settings.crc_bytes = root.getUInt(PSTR(D_JSON_CRC_BYTES), Lora->settings.crc_bytes);
}

/*********************************************************************************************\
 * Driver Settings load and save
\*********************************************************************************************/

#ifdef USE_UFILESYS
#define XDRV_73_KEY           "drvset73"

bool LoraLoadData(void) {
  char key[] = XDRV_73_KEY;
  String json = UfsJsonSettingsRead(key);
  if (json.length() == 0) { return false; }

  // {"Crc":1882268982,"Flags":0,"Frequency":868.1,"Bandwidth":125.0,"SpreadingFactor":7,"CodingRate4":5,"SyncWord":52,"OutputPower":10,"PreambleLength":8,"CurrentLimit":60.0,"ImplicitHeader":0,"CrcBytes":2}
  JsonParser parser((char*)json.c_str());
  JsonParserObject root = parser.getRootObject();
  if (!root) { return false; }

  Lora->settings.crc32 = root.getUInt(PSTR("Crc"), Lora->settings.crc32);
  Lora->settings.flags = root.getUInt(PSTR("Flags"), Lora->settings.flags);
  LoraJson2Settings(root);

#ifdef USE_LORAWAN_BRIDGE
  if (!LoraWanLoadData()) {
    return false;
  }
#endif  // USE_LORAWAN_BRIDGE

  return true;
}

bool LoraSaveData(void) {
  Response_P(PSTR("{\"" XDRV_73_KEY "\":{"
                   "\"Crc\":%u,"
                   "\"Flags\":%u,"),
                   Lora->settings.crc32,
                   Lora->settings.flags);
  LoraSettings2Json(false);
  ResponseAppend_P(PSTR("}}"));

  if (!UfsJsonSettingsWrite(ResponseData())) {
    return false;
  }
#ifdef USE_LORAWAN_BRIDGE
  if (!LoraWanSaveData()) {
    return false;
  }
#endif  // USE_LORAWAN_BRIDGE
  return true;
}

void LoraDeleteData(void) {
  char key[] = XDRV_73_KEY;
  UfsJsonSettingsDelete(key);  // Use defaults
#ifdef USE_LORAWAN_BRIDGE
  LoraWanDeleteData();
#endif  // USE_LORAWAN_BRIDGE
}
#endif  // USE_UFILESYS

/*********************************************************************************************/

void LoraSettingsLoad(bool erase) {
  // Called from FUNC_PRE_INIT (erase = 0) once at restart
  // Called from FUNC_RESET_SETTINGS (erase = 1) after command reset 4, 5, or 6

  // *** Start init default values in case key is not found ***
  memset(&Lora->settings, 0x00, sizeof(LoraSettings_t));
  // Init any other parameter in struct LoraSettings
  LoraDefaults();
  // *** End Init default values ***

#ifndef USE_UFILESYS
  AddLog(LOG_LEVEL_DEBUG, PSTR("CFG: Lora use defaults as file system not enabled"));
#else
  // Try to load key
  if (erase) {
    LoraDeleteData();
  }
  else if (LoraLoadData()) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CFG: Lora loaded from file"));
  }
  else {
    // File system not ready: No flash space reserved for file system
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("CFG: Lora use defaults as file system not ready or key not found"));
  }
#endif  // USE_UFILESYS
}

void LoraSettingsSave(void) {
  // Called from FUNC_SAVE_SETTINGS every SaveData second and at restart
#ifdef USE_UFILESYS
  if (Lora->delay_settings_save) {       // Delay settings update when expecting cascading changes
    Lora->delay_settings_save--;
    return;
  }
  uint32_t crc32 = GetCfgCrc32((uint8_t*)&Lora->settings +4, sizeof(LoraSettings_t) -4);  // Skip crc32
#ifdef USE_LORAWAN_BRIDGE
  crc32 += LoraWanGetCfgCrc();
#endif  // USE_LORAWAN_BRIDGE
  if (crc32 != Lora->settings.crc32) {
    Lora->settings.crc32 = crc32;
    if (LoraSaveData()) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("CFG: Lora saved to file"));
    } else {
      // File system not ready: No flash space reserved for file system
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("CFG: Lora ERROR File system not ready or unable to save file"));
    }
  }
#endif  // USE_UFILESYS
}

/*********************************************************************************************/

bool LoraSend(uint8_t* data, uint32_t len, bool invert) {
  uint32_t lora_time = millis();         // Time is important for LoRaWan RX windows
  bool result = Lora->Send(data, len, invert);
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("LOR: Send (%u) '%*_H', Invert %d, Freq %3_f, BW %1_f, SF %d, TimeToSend %d ms"),
    lora_time, len, data, invert, &Lora->settings.frequency, &Lora->settings.bandwidth, Lora->settings.spreading_factor, TimePassedSince(lora_time));
  return result;
}

void LoraInput(void) {
  if (!Lora->Available()) { return; }

  char data[TAS_LORA_MAX_PACKET_LENGTH] = { 0 };
  int packet_size = Lora->Receive(data);
  if (!packet_size) { return; }
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("LOR: Rcvd (%u) '%*_H', Freq %3_f, BW %1_f, SF %d, RSSI %1_f, SNR %1_f"),
    Lora->receive_time, packet_size, data, &Lora->settings.frequency, &Lora->settings.bandwidth, Lora->settings.spreading_factor, &Lora->rssi, &Lora->snr);
#ifdef USE_LORAWAN_BRIDGE
  if (bitRead(Lora->settings.flags, TAS_LORA_FLAG_BRIDGE_ENABLED)) {
    if (LoraWanInput((uint8_t*)data, packet_size)) {
      return;   
    }
  }
#endif  // USE_LORAWAN_BRIDGE
  Lora->receive_time = 0;

  char *rcvd_data = data;
  if (TAS_LORA_REMOTE_COMMAND == data[0]) {
    char *payload = data +1;             // Skip TAS_LORA_REMOTE_COMMAND
    char *command_part;
    char *topic_part = strtok_r(payload, " ", &command_part);
    if (topic_part && command_part) {
      if (!strcasecmp(topic_part, SettingsText(SET_MQTT_TOPIC))) {  // Is it mine
        if (bitRead(Lora->settings.flags, TAS_LORA_COMMAND_ENABLED)) {
          ExecuteCommand(command_part, SRC_REMOTE);
          return;
        }
      } else {
        *--command_part = ' ';           // Restore strtok_r '/0'
      }
    }
    rcvd_data++;                         // Remove TAS_LORA_REMOTE_COMMAND header
    packet_size--;                       // Adjust packet size
  }

  bool raw = Lora->raw;
  // Set raw mode if zeroes within data
  for (uint32_t i = 0; i < packet_size; i++) {
    if (0 == rcvd_data[i]) {
      raw = true;
      break;
    }
  }
  bool assume_json = (!raw && (rcvd_data[0] == '{'));
  Response_P(PSTR("{\"LoRaReceived\":"));
  if (assume_json) {
    ResponseAppend_P(rcvd_data);
  } else {
    ResponseAppend_P(PSTR("\""));
    if (raw) {
      ResponseAppend_P(PSTR("%*_H"), packet_size, rcvd_data);
    } else {
      ResponseAppend_P(EscapeJSONString(rcvd_data).c_str());
    }
    ResponseAppend_P(PSTR("\""));
  }
  ResponseAppend_P(PSTR(",\"RSSI\":%1_f,\"SNR\":%1_f}"), &Lora->rssi, &Lora->snr);

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
//    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
#endif // ESP32

    Lora = (Lora_t*)calloc(sizeof(Lora_t), 1);    // Need calloc to reset registers to 0/false
    if (nullptr == Lora) { return; }
    LoraSettingsLoad(0);
#ifdef USE_LORAWAN_BRIDGE
    LoraWanInit();
#endif  // USE_LORAWAN_BRIDGE

    bool present = false;
    char hardware[20];
    if (false) {
      // Need this as following `else if`s may not be present
    }
#ifdef USE_LORA_SX127X
    else if (PinUsed(GPIO_LORA_DI0)) {
      // SX1276, RFM95W
      if (LoraSx127xInit()) {
        Lora->Config    = &LoraSx127xConfig;
        Lora->Available = &LoraSx127xAvailable;
        Lora->Receive   = &LoraSx127xReceive;
        Lora->Send      = &LoraSx127xSend;
        Lora->Init      = &LoraSx127xInit;
        strcpy_P(hardware, PSTR("SX127x"));
        present = true;
      }
    }
#endif  // USE_LORA_SX127X
#ifdef USE_LORA_SX126X
    else if (PinUsed(GPIO_LORA_DI1) && PinUsed(GPIO_LORA_BUSY)) {
      // SX1262, LilyGoT3S3
      if (LoraSx126xInit()) {
        Lora->Config    = &LoraSx126xConfig;
        Lora->Available = &LoraSx126xAvailable;
        Lora->Receive   = &LoraSx126xReceive;
        Lora->Send      = &LoraSx126xSend;
        Lora->Init      = &LoraSx126xInit;
        strcpy_P(hardware, PSTR("SX126x"));
        present = true;
      }
    }
#endif  // USE_LORA_SX126X

    if (present) {
      AddLog(LOG_LEVEL_INFO, PSTR("LOR: %s initialized"), hardware);
    } else {
      free(Lora);
      Lora = nullptr;
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

#define D_CMND_LORASEND       "Send"
#define D_CMND_LORACONFIG     "Config"
#define D_CMND_LORACOMMAND    "Command"
#define D_CMND_LORAOPTION     "Option"

const char kLoraCommands[] PROGMEM = "LoRa|"  // Prefix
  D_CMND_LORASEND "|" D_CMND_LORACONFIG "|" D_CMND_LORACOMMAND "|" D_CMND_LORAOPTION;

void (* const LoraCommand[])(void) PROGMEM = {
  &CmndLoraSend, &CmndLoraConfig, &CmndLoraCommand, &CmndLoraOption };

void CmndLoraOption(void) {
  // LoraOption1 1 - Enable LoRaWanBridge
  // LoraOption2 1 - Enable LoRaWanBridge Join
  // LoraOption3 1 - Enable LoRaWanBridge decoding
  // LoraOption4 1 - Enable LoRaCommand reception
  // LoraOption5 1 - Skip LoRaWanBridge RX2 window send
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 8)) {
    uint32_t pindex = XdrvMailbox.index -1;
    if (XdrvMailbox.payload >= 0) {
      bitWrite(Lora->settings.flags, pindex, XdrvMailbox.payload);
    }
    ResponseCmndIdxChar(GetStateText(bitRead(Lora->settings.flags, pindex)));
  }
}

void CmndLoraCommand(void) {
  // LoRaCommand <topic_of_lora_receiver> <command>
  // LoRaCommand lorareceiver power 2
  // LoRaCommand lorareceiver publish cmnd/anytopic/power 2
  // LoRaCommand lorareceiver LoRaCommand thisreceiver status
  if (XdrvMailbox.data_len > 0) {
    char data[TAS_LORA_MAX_PACKET_LENGTH] = { 0 };
    XdrvMailbox.data_len++;       // Add Signal CmndLoraCommand to lora receiver
    uint32_t len = (XdrvMailbox.data_len < TAS_LORA_MAX_PACKET_LENGTH -1) ? XdrvMailbox.data_len : TAS_LORA_MAX_PACKET_LENGTH -2;
    data[0] = TAS_LORA_REMOTE_COMMAND;
    strlcpy(data +1, XdrvMailbox.data, len);
    LoraSend((uint8_t*)data, len, false);
    ResponseCmndDone();
  }
}

void CmndLoraSend(void) {
  // LoRaSend "Hello Tiger"     - Send "Hello Tiger\n"
  // LoRaSend                   - Set to text decoding
  // LoRaSend1 "Hello Tiger"    - Send "Hello Tiger\n"
  // LoRaSend2 "Hello Tiger"    - Send "Hello Tiger"
  // LoRaSend3 "Hello Tiger"    - Send "Hello Tiger\f"
  // LoRaSend4                  - Set to binary decoding
  // LoRaSend4 "Hello Tiger"    - Send "Hello Tiger" and set to binary decoding
  // LoRaSend5 "AA004566"       - Send "AA004566" as hex values
  // LoRaSend6 "72,101,108,108" - Send decimals as hex values
  // LoRaSend15 "AA004566"      - Send "AA004566" as hex values with invert IQ
  bool invert = false;
  if (XdrvMailbox.index > 9) { 
    XdrvMailbox.index -= 10;
    invert = true;
  }
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 6)) {
    Lora->raw = (XdrvMailbox.index > 3);                                 // Global flag set even without data
    if (XdrvMailbox.data_len > 0) {
      char data[TAS_LORA_MAX_PACKET_LENGTH] = { 0 };
      uint32_t len = (XdrvMailbox.data_len < TAS_LORA_MAX_PACKET_LENGTH -1) ? XdrvMailbox.data_len : TAS_LORA_MAX_PACKET_LENGTH -2;
#ifdef USE_LORA_DEBUG
      AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: Len %d, Send %*_H"), len, len, XdrvMailbox.data);
#endif
      if (1 == XdrvMailbox.index) {                                      // "Hello Tiger\n"
        memcpy(data, XdrvMailbox.data, len);
        data[len++] = '\n';
      }
      else if ((2 == XdrvMailbox.index) || (4 == XdrvMailbox.index)) {   // "Hello Tiger" or "A0"
        memcpy(data, XdrvMailbox.data, len);
      }
      else if (3 == XdrvMailbox.index) {                                 // "Hello\f"
        Unescape(XdrvMailbox.data, &len);
        memcpy(data, XdrvMailbox.data, len);
      }
      else if (5 == XdrvMailbox.index) {                                 // "AA004566" as hex values
        char *p;
        char stemp[3];

        char *codes = RemoveSpace(XdrvMailbox.data);
        int size = strlen(XdrvMailbox.data);

        len = 0;
        while (size > 1) {
          strlcpy(stemp, codes, sizeof(stemp));
          data[len++] = strtol(stemp, &p, 16);
          if (len > TAS_LORA_MAX_PACKET_LENGTH -2) { break; }
          size -= 2;
          codes += 2;
        }
      }
      else if (6 == XdrvMailbox.index) {                                 // "72,101,108,108"
        char *p;
        uint8_t code;
        char *values = XdrvMailbox.data;
        len = 0;
        for (char* str = strtok_r(values, ",", &p); str; str = strtok_r(nullptr, ",", &p)) {
          data[len++] = (uint8_t)atoi(str);
          if (len > TAS_LORA_MAX_PACKET_LENGTH -2) { break; }
        }
      }
      else {
        len = 0;
      }
      if (len) {
        LoraSend((uint8_t*)data, len, invert);
      }
      ResponseCmndDone();
    }
  }
}

void CmndLoraConfig(void) {
  // LoRaConfig                                       - Show all parameters
  // LoRaConfig 1                                     - Set EU868 default parameters
  // LoRaConfig 2                                     - Set EU868 default LoRaWan bridge parameters
  // LoRaConfig 41                                    - Set AU915 default parameters
  // LoRaConfig 42                                    - Set AU915 default LoRaWan bridge parameters with channel 0
  // LoRaConfig 42,10                                 - Set AU915 default LoRaWan bridge parameters with channel 10
  // LoRaConfig {"Frequency":868.0,"Bandwidth":125.0} - Enter float parameters
  // LoRaConfig {"SyncWord":18}                       - Enter decimal parameter (=0x12)
  if (XdrvMailbox.data_len > 0) {
    if (XdrvMailbox.payload > 0) {
      uint32_t region = (XdrvMailbox.payload / 10) &0x0F;     // 0 .. 15
      if (region > 9) { region = 0; }                         // 0 .. 9
      uint32_t option = ((XdrvMailbox.payload -1) &0x01) +1;  // Option 1 or 2
      switch (option) {
        case 1: 
          LoraDefaults(region);                               // Default region LoRa values
          break;
#ifdef USE_LORAWAN_BRIDGE
        case 2: {
          switch (region) {
            case TAS_LORA_REGION_AU915: {
              uint32_t parm[2] = { 0, 0 };
              ParseParameters(2, parm);                       // parm[1] will hold channel
              Lora->settings.region = region;                 // Need valid region for LoraWanRadioInfo()
              LoRaWanRadioInfo_t UpInfo;
              LoraWanRadioInfo(TAS_LORAWAN_RADIO_UPLINK, &UpInfo, parm[1]);  // Set uplink frequency based on channel
              Lora->settings.frequency = UpInfo.frequency;       
              break;
            }
            default:  // TAS_LORA_REGION_EU868
              Lora->settings.frequency = TAS_LORAWAN_FREQUENCY;       
          }
          LoraWanDefaults(region);                            // Default region LoRaWan values
          break;
        }
#endif  // USE_LORAWAN_BRIDGE
      }
      Lora->Config(true);
    }
    else {
      JsonParser parser(XdrvMailbox.data);
      JsonParserObject root = parser.getRootObject();
      if (root) { 
        LoraJson2Settings(root);
        Lora->Config(true);
      }
    }
    uint8_t data[1] = { 0 };
    LoraSend(data, 0, false);                                 // Fix init SX12xx after config change
  }
  ResponseCmnd();  // {"LoRaConfig":
  ResponseAppend_P(PSTR("{"));
  LoraSettings2Json(true);
  ResponseAppend_P(PSTR("}}"));
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv73(uint32_t function) {
  bool result = false;

  if (FUNC_INIT == function) {
    LoraInit();
  }
  else if (Lora) {
    switch (function) {
      case FUNC_LOOP:
      case FUNC_SLEEP_LOOP:
#ifdef USE_LORAWAN_BRIDGE
        LoRaWanSend();
#endif  // USE_LORAWAN_BRIDGE
        LoraInput();
        break;
      case FUNC_RESET_SETTINGS:
        LoraSettingsLoad(1);
        break;
      case FUNC_SAVE_SETTINGS:
        LoraSettingsSave();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kLoraCommands, LoraCommand);
#ifdef USE_LORAWAN_BRIDGE
        if (!result) {
          result = DecodeCommand(kLoraWanCommands, LoraWanCommand);
        }
#endif  // USE_LORAWAN_BRIDGE
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