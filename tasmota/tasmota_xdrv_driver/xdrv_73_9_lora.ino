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

/*
For EU bands, the Uplink/Downlink (TX/RX) frequencies can be the same.
For Others,  same Uplink/Downlink (TX/RX) frequencies may not be allowed.
See: https://lora-alliance.org/wp-content/uploads/2020/11/RP_2-1.0.2.pdf
*/
enum LoRaBand_t {
  TAS_LORA_BAND_EU868, 
  TAS_LORA_BAND_AU915
};

//LoraBands: One for each enum above
const char * const LoraBands[] PROGMEM = {
	"EU868",
	"AU915"
};

String LoraBandName(uint32_t bandIndex){
  uint32_t uNumBands = sizeof(LoraBands)/sizeof(LoraBands[0]);
  if(bandIndex>(uNumBands-1)) bandIndex = uNumBands - 1;
  return String(LoraBands[bandIndex]);
}



/*
 Sets LoRa values
*/
void LoraDefaults(uint32_t uBand = TAS_LORA_BAND_EU868, bool bRX=true, uint32_t channel=0){
  switch(uBand){
    case TAS_LORA_BAND_AU915:
    AddLog(LOG_LEVEL_DEBUG, PSTR("DBGROB:LoraDefaults(AU915) uBand=%u bRX=%u channel=%u"),uBand, bRX, channel);
     /*    ref: https://lora-alliance.org/wp-content/uploads/2020/11/RP_2-1.0.2.pdf page 47
    
     DR0  LoRa: SF12 / 125 kHz 
     DR1  LoRa: SF11 / 125 kHz 
     DR2  LoRa: SF10 / 125 kHz   <-- JOIN REQUEST 
     DR3  LoRa: SF9  / 125 kHz 
     DR4  LoRa: SF8  / 125 kHz 
     DR5  LoRa: SF7  / 125 kHz 
     DR6  LoRa: SF8  / 500 kHz        Same as DR12
     DR7  LR-FHSS CR1/3: 1.523 MHz OCW 162
     DR8  LoRa: SF12 / 500 kHz
     DR9  LoRa: SF11 / 500 kHz 
     DR10 LoRa: SF10 / 500 kHz 
     DR11 LoRa: SF9  / 500 kHz 
     DR12 LoRa: SF8  / 500 kHz        Same as DR6
     DR13 LoRa: SF7  / 500 kHz 

     UPLINK (RX) CHANNELS
     There are 72 channels
       0-63: DR0 to 5. Starting 915.2, incrementing by 0.2 Mhz to 927.8  <-- JOIN REQUEST
      64-71: DR6     . Starting 915.9, incrementing by 1.6 MHz to 927.1

     DOWNLINK (TX) CHANNELS
     There are 8 channels
       0-7: DR8 to 13. Starting 923.3, incrementing by 0.6 MHz to 927.5

     After an uplink:  Downlink (TX) link subchannel = Uplink (RX) Channel Number modulo 8   
     e.g. --Uplink (RX)--     --Downlink (TX)--
          Freq    Channel     Channel Frequency
          915.2   0           0       923.3
          927.8   63          7       927.1

     After an uplink:  
     Downlink DR for RX1 must follow this table
     Uplink  Downlink
     DR0     DR8
     DR1     DR8
     DR2     DR10   <----- channels 1-62
     DR3     DR11
     DR4     DR12
     DR5     DR13
     DR6     DR13  <------ channels 63-71

     Downlink DR for RX2 must be DR8
    
     */
     float   frequency;
     float   bandwidth;
     uint8_t spreading_factor;

     if(bRX){
      if(channel>71) channel=71;
      if(channel < 64){
        frequency        = TAS_LORA_AU915_FREQUENCY_UP1 + (channel * 0.2);
        bandwidth        = TAS_LORA_AU915_BANDWIDTH_UP1;              //DR2
        spreading_factor = TAS_LORA_AU915_SPREADING_FACTOR_UP1;        //DR2
      } else {
        frequency        = TAS_LORA_AU915_FREQUENCY_UP2 + (channel * 1.6);
        bandwidth        = TAS_LORA_AU915_BANDWIDTH_UP2;                //DR6
        spreading_factor = TAS_LORA_AU915_SPREADING_FACTOR_UP2;         //DR6
      }

     } else {
      // TX
      // Reference: https://lora-alliance.org/wp-content/uploads/2020/11/lorawan_regional_parameters_v1.0.3reva_0.pdf)
      // Assume this is in response to an uplink RX, so we already know RX freq & bw & sf
      // TX channel depends on RX freq
      // DR for RX1 depends on incoming DR (and RX1DROffset)
      // DR for RX2 is fixed ar DR8 
      //
      // Tasmota does not support different RX1 & RX2 DRs, so just use DR8 and rely on RX2 arriving at end device OK.
      //
      float    fFrequencyDiff;
      uint32_t uRxChannel;
      if(125.0 == Lora->settings.bandwidth){
        fFrequencyDiff=Lora->settings.frequency - TAS_LORA_AU915_FREQUENCY_UP1;
        uRxChannel = (uint32_t)(fFrequencyDiff / 0.2);
      } else {
        fFrequencyDiff=Lora->settings.frequency - TAS_LORA_AU915_FREQUENCY_UP2;
        uRxChannel = 64 + (uint32_t)(fFrequencyDiff / 1.6);
      }
      channel = uRxChannel%8;
      AddLog(LOG_LEVEL_DEBUG, PSTR("DBGROB:LoraDefaults(AU915) TX: uRxChannel=%u, new TX channel:%u"),uRxChannel,channel);

      //Should really have different DR for RX1 and RX2. But ... just rely on RX2 for now.
      frequency        = TAS_LORA_AU915_FREQUENCY_DN + (channel * 0.6);
      bandwidth        = TAS_LORA_AU915_BANDWIDTH_DN;               //DR8
      spreading_factor = TAS_LORA_AU915_SPREADING_FACTOR_DN;        //DR8
     }

     Lora->settings.frequency        = frequency;
     Lora->settings.bandwidth        = bandwidth;
     Lora->settings.spreading_factor = spreading_factor;
     Lora->settings.coding_rate      = TAS_LORA_AU915_CODING_RATE;
     Lora->settings.sync_word        = TAS_LORA_AU915_SYNC_WORD;
     Lora->settings.output_power     = TAS_LORA_AU915_OUTPUT_POWER;
     Lora->settings.preamble_length  = TAS_LORA_AU915_PREAMBLE_LENGTH;
     Lora->settings.current_limit    = TAS_LORA_AU915_CURRENT_LIMIT;
     Lora->settings.implicit_header  = TAS_LORA_AU915_HEADER;
     Lora->settings.crc_bytes        = TAS_LORA_AU915_CRC_BYTES;
     Lora->settings.band             = TAS_LORA_BAND_AU915;
     break;
    
    default: 
     Lora->settings.frequency = TAS_LORA_FREQUENCY;
     Lora->settings.bandwidth = TAS_LORA_BANDWIDTH;
     Lora->settings.spreading_factor = TAS_LORA_SPREADING_FACTOR;
     Lora->settings.coding_rate = TAS_LORA_CODING_RATE;
     Lora->settings.sync_word = TAS_LORA_SYNC_WORD;
     Lora->settings.output_power = TAS_LORA_OUTPUT_POWER;
     Lora->settings.preamble_length = TAS_LORA_PREAMBLE_LENGTH;
     Lora->settings.current_limit = TAS_LORA_CURRENT_LIMIT;
     Lora->settings.implicit_header = TAS_LORA_HEADER;
     Lora->settings.crc_bytes = TAS_LORA_CRC_BYTES;
     Lora->settings.band      = TAS_LORA_BAND_EU868;
  }
}

void LoraWanDefaults(void) {
  Lora->settings.frequency = TAS_LORAWAN_FREQUENCY;
  Lora->settings.bandwidth = TAS_LORAWAN_BANDWIDTH;
  Lora->settings.spreading_factor = TAS_LORAWAN_SPREADING_FACTOR;
  Lora->settings.coding_rate = TAS_LORAWAN_CODING_RATE;
  Lora->settings.sync_word = TAS_LORAWAN_SYNC_WORD;
  Lora->settings.output_power = TAS_LORAWAN_OUTPUT_POWER;
  Lora->settings.preamble_length = TAS_LORAWAN_PREAMBLE_LENGTH;
  Lora->settings.current_limit = TAS_LORAWAN_CURRENT_LIMIT;
  Lora->settings.implicit_header = TAS_LORAWAN_HEADER;
  Lora->settings.crc_bytes = TAS_LORAWAN_CRC_BYTES;
  Lora->settings.band      = TAS_LORA_BAND_EU868;
}

void LoraSettings2Json(void) {
  ResponseAppend_P(PSTR("\"" D_JSON_FREQUENCY "\":%1_f"), &Lora->settings.frequency);              // xxx.x MHz
  ResponseAppend_P(PSTR(",\"" D_JSON_BANDWIDTH "\":%1_f"), &Lora->settings.bandwidth);             // xxx.x kHz
  ResponseAppend_P(PSTR(",\"" D_JSON_SPREADING_FACTOR "\":%d"), Lora->settings.spreading_factor);
  ResponseAppend_P(PSTR(",\"" D_JSON_CODINGRATE4 "\":%d"), Lora->settings.coding_rate);
  ResponseAppend_P(PSTR(",\"" D_JSON_SYNCWORD "\":%d"), Lora->settings.sync_word);
  ResponseAppend_P(PSTR(",\"" D_JSON_OUTPUT_POWER "\":%d"), Lora->settings.output_power);          // dBm
  ResponseAppend_P(PSTR(",\"" D_JSON_PREAMBLE_LENGTH "\":%d"), Lora->settings.preamble_length);    // symbols
  ResponseAppend_P(PSTR(",\"" D_JSON_CURRENT_LIMIT "\":%1_f"), &Lora->settings.current_limit);     // xx.x mA (Overcurrent Protection - OCP)
  ResponseAppend_P(PSTR(",\"" D_JSON_IMPLICIT_HEADER "\":%d"), Lora->settings.implicit_header);    // 0 = explicit
  ResponseAppend_P(PSTR(",\"" D_JSON_CRC_BYTES "\":%d"), Lora->settings.crc_bytes);                // bytes
  ResponseAppend_P(PSTR(",\"" D_JSON_LORA_BAND "\":\"%s\""), LoraBands[Lora->settings.band]);      // enum 0=EU868,1=AU915   
}

void LoraJson2Settings(JsonParserObject root) {
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

  uint uSize = sizeof(LoraBands)/sizeof(LoraBands[0]);  
  String jsonBand =  root.getStr(PSTR(D_JSON_LORA_BAND));
  jsonBand.toUpperCase();
  for(uint i=0;i<uSize;i++){
   if(!strcmp_P(jsonBand.c_str(), LoraBands[i])){
      Lora->settings.band = i;
      break;
     };
  }
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
  LoraSettings2Json();
  ResponseAppend_P(PSTR("}}"));

  if (!UfsJsonSettingsWrite(ResponseData())) {
    return false;
  }
#ifdef USE_LORAWAN_BRIDGE
  if (!LoraWanSaveData()) {
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("DBGROB: LoraSaveData() LoraWanSaveData() failed"));
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
  uint32_t crc32 = GetCfgCrc32((uint8_t*)&Lora->settings +4, sizeof(LoraSettings_t) -4);  // Skip crc32
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
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("DBGROB: LoraSend() Changing to TX profile. Current Freq:%1_f  "),&Lora->settings.frequency);
  LoraSettings_t RXsettings = Lora->settings;  //make a copy
  LoraDefaults(Lora->settings.band, false);    //Set TX profile
  Lora->Config();

  uint32_t lora_time = millis();         // Time is important for LoRaWan RX windows
  bool result = Lora->Send(data, len, invert);
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("LOR: Send (%u) '%*_H', Invert %d, Time %d"),
    lora_time, len, data, invert, TimePassedSince(lora_time));
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("DBGROB: LoraSend() Changing to RX profile Current Freq:%1_f  "),&Lora->settings.frequency);
  Lora->settings = RXsettings;
  Lora->Config();
  return result;
}

void LoraInput(void) {
  if (!Lora->Available()) { return; }

  char data[TAS_LORA_MAX_PACKET_LENGTH] = { 0 };
  int packet_size = Lora->Receive(data);
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("DBGROB: Rcvd packets %u bytes"),packet_size);
  if (!packet_size) { return; }
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("LOR: Rcvd (%u) '%*_H', RSSI %1_f, SNR %1_f"),
    Lora->receive_time, packet_size, data, &Lora->rssi, &Lora->snr);
#ifdef USE_LORAWAN_BRIDGE
  if (bitRead(Lora->settings.flags, TAS_LORA_FLAG_BRIDGE_ENABLED)) {
   AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("DBGROB: Calling(LoRaWanInput())"));

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
    Lora->raw = (XdrvMailbox.index > 3);                                    // Global flag set even without data
    if (XdrvMailbox.data_len > 0) {
      char data[TAS_LORA_MAX_PACKET_LENGTH] = { 0 };
      uint32_t len = (XdrvMailbox.data_len < TAS_LORA_MAX_PACKET_LENGTH -1) ? XdrvMailbox.data_len : TAS_LORA_MAX_PACKET_LENGTH -2;
#ifdef USE_LORA_DEBUG
      AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: Len %d, Send %*_H"), len, len, XdrvMailbox.data);
#endif
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
          if (len > TAS_LORA_MAX_PACKET_LENGTH -2) { break; }
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
  // LoRaConfig 1                                     - Set default parameters
  // LoRaConfig 2                                     - Set default LoRaWan bridge parameters
  // LoRaConfig AU915                                 - Set default for AU915 band
  // LoRaConfig {"Frequency":868.0,"Bandwidth":125.0} - Enter float parameters
  // LoRaConfig {"SyncWord":18}                       - Enter decimal parameter (=0x12)
  if (XdrvMailbox.data_len > 0) {
    String uData =  XdrvMailbox.data;
    uData.toUpperCase();
    
    if (XdrvMailbox.payload == 1) {
      LoraDefaults();
      Lora->Config();
    }
    else if (XdrvMailbox.payload == 2) {
      LoraWanDefaults();
      Lora->Config();
    }
   else if (!strcmp_P(uData.c_str(), PSTR("AU915"))) { 
#ifdef USE_LORA_DEBUG
      AddLog(LOG_LEVEL_DEBUG, PSTR("ROBDBG: Command AU915"));
#endif
      LoraDefaults(TAS_LORA_BAND_AU915);
      Lora->Config();
    }
    else {
      JsonParser parser(XdrvMailbox.data);
      JsonParserObject root = parser.getRootObject();
      if (root) { 
        LoraJson2Settings(root);
        Lora->Config();
      }
    }
  }
  ResponseCmnd();  // {"LoRaConfig":
  ResponseAppend_P(PSTR("{"));
  LoraSettings2Json();
  ResponseAppend_P(PSTR("}}"));

  uint32_t irq_stat = LoRaRadio.getIrqStatus();
  AddLog(LOG_LEVEL_DEBUG, PSTR("DBGROB: SendFlag:%u, ReceivedFlag:%u irq:%04X "), Lora->send_flag,Lora->received_flag, irq_stat);  

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
