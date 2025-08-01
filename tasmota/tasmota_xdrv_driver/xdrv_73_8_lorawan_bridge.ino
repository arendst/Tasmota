/*
  xdrv_73_8_lorawan_bridge.ino - LoRaWan EU868/AU915 support for Tasmota

  SPDX-FileCopyrightText: 2024 Theo Arends, Rob Clark

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_SPI_LORA
#ifdef USE_LORAWAN_BRIDGE
/*********************************************************************************************\
 * EU868 and AU915 LoRaWan bridge support
 * 
 * The goal of the LoRaWan Bridge is to receive from joined LoRaWan End-Devices or Nodes and
 *  provide decrypted MQTT data.
 * 
 * EU868 LoRaWan uses at minimum alternating 3 frequencies and 6 spreadingfactors (datarate or DR)
 *  which makes the use of single fixed frequency and spreadingfactor hardware like
 *  SX127x (LiliGo T3, M5 LoRa868 or RFM95W) or SX126x (LiLiGo T3S3) a challenge.
 * AU915 LoRaWan uses different frequencies for upload and download.
 * This driver uses one fixed frequency and spreadingfactor trying to tell the End-Device to do
 *  the same using Over The Air Activation (OTAA). In some cases the End-Device needs to be
 *  (serial) configured to use a single channel and fixed datarate.
 * 
 * To be able to do this:
 *  - Tasmota needs a filesystem to store persistent data (#define USE_UFILESYS)
 *  - Tasmota Lora has to be configured for one of the three EU868 supported frequencies and
 *     spreadingfactor using command
 *      LoRaConfig 2 
 *     or individual commands
 *      LoRaConfig {"Frequency":868.1} (or 868.3 and 868.5)
 *      LoRaConfig {"SpreadingFactor":9} (or 7..12 equals LoRaWan DataRate 5..0)
 *      LoRaConfig {"Bandwidth":125}
 *      LoRaConfig {"CodingRate4":5}
 *      LoRaConfig {"SyncWord":52}
 *  - Tasmota Lora has to be configured for AU915 using command
 *     LoRaConfig 42[,<channel>]
 *  - LoRaWan has to be enabled (#define USE_LORAWAN_BRIDGE) and configured for the End-Device
 *     32 character AppKey using command LoRaWanAppKey <vendor provided appkey>
 *  - The End-Device needs to start it's LoRaWan join process as documented by vendor.
 *     This will configure the LoRanWan bridge with the needed information. The bridge will also
 *     try to configure the device for single frequency and datarate. The process can take
 *     over 5 minutes as the device transmits at different frequencies and DR.
 *  - If all's well MQTT data will appear when the End-Device sends it's data.
 *  - If a device does not support Adaptive Datarate Control (ADR) the device needs to be set to
 *     a fixed frequency and datarate externally. As an example the Dragino LDS02 needs to be
 *     configured before joining using it's serial interface with the following commands
 *      Password 123456
 *      AT+CHS=868100000
 *      AT+CADR=0
 *      AT+CDATARATE=3
 * 
 * The current driver supports decoding for Dragino LDS02 Door/Window sensor and MerryIoT
 *  DW10 Door/Window sensor. Both battery powered.
 * 
 * The MQTT output can be user defined with SetOption commands like the Zigbee driver
 *    SetOption89    - Distinct MQTT topics per device (1) (#7835)
 *    SetOption100   - Remove LwReceived form JSON message (1)
 *    SetOption118   - Move LwReceived from JSON message and into the subtopic replacing "SENSOR"
 *    SetOption119   - Remove the device addr from json payload, can be used with LoRaWanName
 *                      where the addr is already known from the topic
 *    SetOption144   - Include time in `LwReceived` messages like other sensors (SO100 0 and SO118 0)
 *    LoRaOption3    - Disable driver decoding (1) and provide decrypted payload for decoding
 * 
 * The LoRaWanBridge is default off. Enable it with command
 *    LoRaWanBridge 1
\*********************************************************************************************/

bool LoraWanAddNode(void) {
  if (Lora->nodes < TAS_LORAWAN_ENDNODES) {
    Lora->settings.end_node[Lora->nodes] = (LoraEndNode_t*)calloc(sizeof(LoraEndNode_t), 1);  // Need calloc to reset registers to 0/false
    if (Lora->settings.end_node[Lora->nodes]) { 
      Lora->nodes++;
#ifdef USE_LORA_DEBUG
      AddLog(LOG_LEVEL_DEBUG, PSTR("LOR: Allocated %d bytes for node %d at %08X"), 
        sizeof(LoraEndNode_t), Lora->nodes, Lora->settings.end_node[Lora->nodes -1]);
#endif  // USE_LORA_DEBUG
      return true;
    }
  }
  return false;
}

/*********************************************************************************************\
 * Driver Settings load and save
\*********************************************************************************************/

#ifdef USE_UFILESYS

#define D_JSON_APPKEY   "AppKey"
#define D_JSON_DEVEUI   "DevEUI"
#define D_JSON_DEVNONCE "DevNonce"
#define D_JSON_FCNTUP   "FCntUp"
#define D_JSON_FCNTDOWN "FCntDown"
#define D_JSON_FLAGS    "Flags"

bool LoraWanLoadData(void) {
  char key[12];                                      // Max 99 nodes (drvset73_1 to drvset73_99)
  for (uint32_t n = 0; n < TAS_LORAWAN_ENDNODES; n++) {
    snprintf_P(key, sizeof(key), PSTR(XDRV_73_KEY "_%d"), n +1);

    String json = UfsJsonSettingsRead(key);
    if (json.length() == 0) { continue; }            // Only load used slots

    // {"AppKey":"00000000000000000000000000000000","DevEUIh":1234567890,"DevEUIl":1234567890,"DevNonce":0,"FCntUp":0,"FCntDown":0,"Flags":0,"NAME":"","DCDR":""}
    JsonParser parser((char*)json.c_str());
    JsonParserObject root = parser.getRootObject();
    if (!root) { continue; }                         // Only load used slots

    if (!LoraWanAddNode()) { break; }                // Unable to allocate memory

    const char* app_key = nullptr;
    app_key = root.getStr(PSTR(D_JSON_APPKEY), nullptr);
    if (app_key && (strlen(app_key))) {
      HexToBytes(app_key, Lora->settings.end_node[n]->AppKey, TAS_LORAWAN_AES128_KEY_SIZE);
    }
    Lora->settings.end_node[n]->DevEUIh = root.getUInt(PSTR(D_JSON_DEVEUI "h"), Lora->settings.end_node[n]->DevEUIh);
    Lora->settings.end_node[n]->DevEUIl = root.getUInt(PSTR(D_JSON_DEVEUI "l"), Lora->settings.end_node[n]->DevEUIl);
    Lora->settings.end_node[n]->DevNonce = root.getUInt(PSTR(D_JSON_DEVNONCE), Lora->settings.end_node[n]->DevNonce);
    Lora->settings.end_node[n]->FCntUp = root.getUInt(PSTR(D_JSON_FCNTUP), Lora->settings.end_node[n]->FCntUp);
    Lora->settings.end_node[n]->FCntDown = root.getUInt(PSTR(D_JSON_FCNTDOWN), Lora->settings.end_node[n]->FCntDown);
    Lora->settings.end_node[n]->flags = root.getUInt(PSTR(D_JSON_FLAGS), Lora->settings.end_node[n]->flags);
    const char* ctemp = root.getStr(PSTR(D_JSON_NAME), nullptr);
    if (ctemp) { Lora->settings.end_node[n]->name = ctemp; }
    ctemp = root.getStr(PSTR(D_JSON_DCDR), nullptr);
    if (ctemp) { Lora->settings.end_node[n]->decoder = ctemp; }
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR("CFG: LoraWan loaded %d node(s)"), Lora->nodes);
  return true;
}

bool LoraWanSaveData(void) {
  bool result = true;                                // Return true if no Endnodes
  for (uint32_t n = 0; n < Lora->nodes; n++) {
    if (Lora->settings.end_node[n]->AppKey[0] > 0) { // Only save used slots
      Response_P(PSTR("{\"" XDRV_73_KEY "_%d\":{\"" D_JSON_APPKEY "\":\"%16_H\""
                                              ",\"" D_JSON_DEVEUI "h\":%lu,\"" D_JSON_DEVEUI "l\":%lu"
                                              ",\"" D_JSON_DEVNONCE "\":%u"
                                              ",\"" D_JSON_FCNTUP "\":%u,\"" D_JSON_FCNTDOWN "\":%u"
                                              ",\"" D_JSON_FLAGS "\":%u"
                                              ",\"" D_JSON_NAME "\":\"%s\""
                                              ",\"" D_JSON_DCDR "\":\"%s\"}}"),
        n +1,
        Lora->settings.end_node[n]->AppKey,
        Lora->settings.end_node[n]->DevEUIh, Lora->settings.end_node[n]->DevEUIl,
        Lora->settings.end_node[n]->DevNonce,
        Lora->settings.end_node[n]->FCntUp, Lora->settings.end_node[n]->FCntDown,
        Lora->settings.end_node[n]->flags,
        (Lora->settings.end_node[n]->name) ? Lora->settings.end_node[n]->name.c_str() : "",
        (Lora->settings.end_node[n]->decoder) ? Lora->settings.end_node[n]->decoder.c_str() : "");
      result &= UfsJsonSettingsWrite(ResponseData());
    }
  }
  return result;
}

void LoraWanDeleteData(void) {
  char key[12];                                      // Max 99 nodes (drvset73_1 to drvset73_99)
  for (uint32_t n = 0; n < TAS_LORAWAN_ENDNODES; n++) {
    snprintf_P(key, sizeof(key), PSTR(XDRV_73_KEY "_%d"), n +1);
    UfsJsonSettingsDelete(key);                      // Use defaults
  }
}

uint32_t LoraWanGetCfgCrc(void) {
  uint32_t crc32 = 0;
  for (uint32_t n = 0; n < Lora->nodes; n++) {
    if (Lora->settings.end_node[n]->AppKey[0] > 0) { // Only save used slots
      crc32 += GetCfgCrc32((uint8_t*)Lora->settings.end_node[n], sizeof(LoraEndNode_t));
    }
  }
  return crc32;
}
#endif  // USE_UFILESYS

/*********************************************************************************************/

/*
For LoraWan EU bands, the Uplink/Downlink (TX/RX) frequencies can be the same.
For Others, same Uplink/Downlink (TX/RX) frequencies may not be allowed.
See: https://lora-alliance.org/wp-content/uploads/2020/11/RP_2-1.0.2.pdf
*/

uint32_t LoraWanFrequencyToChannel(void) {
  // Determines the channel from the current Uplink LoraSettings
  uint32_t channel = 0;

  switch (Lora->settings.region) {
    case TAS_LORA_REGION_AU915: 
      // Return 0 .. 71
      if ((Lora->settings.frequency < 915.2) || (Lora->settings.frequency > 927.8)) {
        Lora->settings.frequency = TAS_LORAWAN_AU915_FREQUENCY_UP1;  // Fix initial frequency
        Lora->settings.bandwidth = TAS_LORAWAN_AU915_BANDWIDTH_UP1;  // Fix initial bandwidth
      }
      else if (125.0 == Lora->settings.bandwidth) {
        float fFrequencyDiff = Lora->settings.frequency - TAS_LORAWAN_AU915_FREQUENCY_UP1;
        channel = (0.01 + (fFrequencyDiff / 0.2));  // 0.01 to fix rounding errors
      } else {
        float fFrequencyDiff = Lora->settings.frequency - TAS_LORAWAN_AU915_FREQUENCY_UP2;
        channel = 64 + ((0.01 + (fFrequencyDiff / 1.6)));
      }
      break;
    default:  // TAS_LORA_REGION_EU868
      // EU863-870 (EU868) JoinReq message frequencies are 868.1, 868.3 and 868.5
      uint32_t frequency = (Lora->settings.frequency * 10);
      uint32_t channel = 250;
      if (8681 == frequency) {
        channel = 0;
      }
      else if (8683 == frequency) {
        channel = 1;
      }
      else if (8685 == frequency) {
        channel = 2;
      }
      if (250 == channel) {
        Lora->settings.frequency = 868.1;
        Lora->Config(false);
        channel = 0;
      }
  }
  return channel; 
}

/*****************************************************************************
     LoraWanRadioInfo() 
       Some regional profiles use different radio profiles for the Uplink, RX1, and RX2 transmissions

       Get radio profiles for the Uplink, and RX1 & RX2 downlink transmissions
                     RX1 & RX2 profiles are derived from Lora->settings
     
****************************************************************************/ 
const uint8_t RX1DRs[] PROGMEM = {8,9,10,11,12,13,13};                 // DR0..6
const uint8_t SF[]     PROGMEM = {12,11,10,9,8,7,8,0,12,11,10,9,8,7};  // DR0..13

void LoraWanRadioInfo(uint8_t mode, void* pInfo, uint32_t uChannel = 0) {
  LoRaWanRadioInfo_t* pResult = (LoRaWanRadioInfo_t*) pInfo;
 
  switch (Lora->settings.region) {
    case TAS_LORA_REGION_AU915: {
      //////////////// AU915 ////////////////////
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
        NOTE: Testing with two Dragino end devices shows they do not play nice with channels 64-71
            1) LHT52 will JOIN OK on Ch64, but never sends any sensor messages on same channel
            2) LHT65 will not even broadcast JOIN requests on any of the channels (as required by RP002)
            For this reason, channels above 63 are not permitted.

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

      Downlink
        Reference: https://lora-alliance.org/wp-content/uploads/2020/11/lorawan_regional_parameters_v1.0.3reva_0.pdf)
        Assume this is in response to an uplink RX, so we already know RX freq & bw & sf
        TX channel depends on RX freq
        DR for RX1 depends on incoming DR (and RX1DROffset)
        DR for RX2 is fixed ar DR8 
        
        Tasmota does not support different RX1 & RX2 DRs (yet), so just use DR8 and rely on RX2 arriving at end device OK.
      */
      uint8_t UplinkChannelBand = LoraWanFrequencyToChannel() %8;  // 0..7
      switch (mode) {
        case TAS_LORAWAN_RADIO_UPLINK: {
  //      if (uChannel > 71) uChannel = 71;   See note above
          if (uChannel > 63) uChannel = 63;   
          if (uChannel < 64) {
              (*pResult).frequency        = TAS_LORAWAN_AU915_FREQUENCY_UP1 + (uChannel * 0.2);
              (*pResult).bandwidth        = TAS_LORAWAN_AU915_BANDWIDTH_UP1;               // DR2
              (*pResult).spreading_factor = TAS_LORAWAN_AU915_SPREADING_FACTOR_UP1;        // DR2
          } else {
              (*pResult).frequency        = TAS_LORAWAN_AU915_FREQUENCY_UP2 + ((uChannel-64) * 1.6);
              (*pResult).bandwidth        = TAS_LORAWAN_AU915_BANDWIDTH_UP2;               // DR6
              (*pResult).spreading_factor = TAS_LORAWAN_AU915_SPREADING_FACTOR_UP2;        // DR6
          }
          break;
        }
        case TAS_LORAWAN_RADIO_RX1: {
          // RX1 DR depends on the Uplink settings
          // https://lora-alliance.org/wp-content/uploads/2020/11/lorawan_regional_parameters_v1.0.3reva_0.pdf
          // Page 41
          uint32_t UplinkDR = (125.0 == Lora->settings.bandwidth) ? (12 - Lora->settings.spreading_factor) : 6;  // 0 .. 6
          uint32_t RX1DR    = pgm_read_byte(&RX1DRs[UplinkDR]);
          uint32_t RX1SF    = pgm_read_byte(&SF[RX1DR]);
          float    RX1BW    = (RX1DR > 5) ? 500.0 : 125.0;      
          (*pResult).frequency        = TAS_LORAWAN_AU915_FREQUENCY_DN + (UplinkChannelBand * 0.6);
          (*pResult).bandwidth        = RX1BW;
          (*pResult).spreading_factor = RX1SF;
          break;
        }
        case TAS_LORAWAN_RADIO_RX2: {
          (*pResult).frequency        = TAS_LORAWAN_AU915_FREQUENCY_DN;
          (*pResult).bandwidth        = TAS_LORAWAN_AU915_BANDWIDTH_RX2;
          (*pResult).spreading_factor = TAS_LORAWAN_AU915_SPREADING_FACTOR_RX2;
          break;
        }
//        default:
//          not implemented
      }
      break;
    }
    default: {  // TAS_LORA_REGION_EU868
      switch (mode) {
        case TAS_LORAWAN_RADIO_UPLINK: {
          (*pResult).frequency        = TAS_LORAWAN_FREQUENCY;
          (*pResult).bandwidth        = TAS_LORAWAN_BANDWIDTH;
          (*pResult).spreading_factor = TAS_LORAWAN_SPREADING_FACTOR;
          break;
        }
        case TAS_LORAWAN_RADIO_RX1: {
          // RX1 DR depends on the Uplink settings
          // https://lora-alliance.org/wp-content/uploads/2020/11/lorawan_regional_parameters_v1.0.3reva_0.pdf
          // Page 19
          (*pResult).frequency        = TAS_LORAWAN_FREQUENCY;
          (*pResult).bandwidth        = TAS_LORAWAN_BANDWIDTH_RX1;
          (*pResult).spreading_factor = TAS_LORAWAN_SPREADING_FACTOR_RX1;
          break;
        }
        case TAS_LORAWAN_RADIO_RX2: {
          (*pResult).frequency        = TAS_LORAWAN_FREQUENCY_DN;
          (*pResult).bandwidth        = TAS_LORAWAN_BANDWIDTH_RX2;
          (*pResult).spreading_factor = TAS_LORAWAN_SPREADING_FACTOR_RX2;
          break;
        }
//        default:
//          not implemented
      }
    }
  }
}

bool LoraWanProfile(uint32_t mode) {
  LoRaWanRadioInfo_t RadioInfo;
  LoraWanRadioInfo(mode, &RadioInfo, LoraWanFrequencyToChannel());  // Region specific
  bool changed = ((Lora->settings.frequency != RadioInfo.frequency) ||
                  (Lora->settings.bandwidth != RadioInfo.bandwidth) ||
                  (Lora->settings.spreading_factor != RadioInfo.spreading_factor));
  Lora->settings.frequency = RadioInfo.frequency;
  Lora->settings.bandwidth = RadioInfo.bandwidth;  
  Lora->settings.spreading_factor = RadioInfo.spreading_factor;
  return changed;
}

void LoraWanDefaults(uint32_t region) {
  Lora->settings.region           = region;
  LoraWanProfile(TAS_LORAWAN_RADIO_UPLINK);
  Lora->settings.coding_rate      = TAS_LORAWAN_CODING_RATE;
  Lora->settings.sync_word        = TAS_LORAWAN_SYNC_WORD;
  Lora->settings.output_power     = TAS_LORAWAN_OUTPUT_POWER;
  Lora->settings.preamble_length  = TAS_LORAWAN_PREAMBLE_LENGTH;
  Lora->settings.current_limit    = TAS_LORAWAN_CURRENT_LIMIT;
  Lora->settings.implicit_header  = TAS_LORAWAN_HEADER;
  Lora->settings.crc_bytes        = TAS_LORAWAN_CRC_BYTES;
}

/*********************************************************************************************/

#include <Ticker.h>
Ticker LoraWan_Send_RX1;
Ticker LoraWan_Send_RX2;

void LoRaWanSend(void) {
  if (!Lora->send_request) { return; }
  Lora->send_request = false;
  LoraSend(Lora->send_buffer, Lora->send_buffer_len, true);
  if (Lora->profile_changed) {
    Lora->settings = Lora->backup_settings;          // Restore copy for reception
    if (0 == Lora->send_buffer_step) {
      Lora->Init();                                  // Necessary to re-init the SXxxxx chip in cases where TX/RX frequencies differ
    } else {
      Lora->Config(false);
    }
  }
}

void LoraWanTickerSend(void) {
  // Need to stay here as short as possible to not initiate watchdog exception
  // As SF12 can take over 1 sec do send in loop instead of here
  Lora->send_buffer_step--;
  if (1 == Lora->send_buffer_step) {
    Lora->rx = true;                                 // Always send during RX1
    Lora->receive_time = 0;                          // Reset receive timer
  }
  if (Lora->rx) {                                    // If received in RX1 do not resend in RX2
    if (1 == Lora->send_buffer_step) {
      Lora->profile_changed = LoraWanProfile(TAS_LORAWAN_RADIO_RX1);  // Set Downlink RX1 profile
    } else {
      Lora->profile_changed = LoraWanProfile(TAS_LORAWAN_RADIO_RX2);  // Set Downlink RX2 profile
    }
    if (Lora->profile_changed) {
      Lora->Config(false);
    }
    Lora->send_request = true;                       // Send in loop fixing watchdogs
#ifdef ESP8266
    SleepSkip();                                     // Skip sleep
#else   // ESP32
    LoRaWanSend();
#endif  // ESP8266/ESP32
  }
}

void LoraWanSendResponse(uint8_t* buffer, size_t len, uint32_t lorawan_delay) {
  free(Lora->send_buffer);                           // Free previous buffer (if any)
  Lora->send_buffer = (uint8_t*)malloc(len +1);
  if (nullptr == Lora->send_buffer) { return; }
  memcpy(Lora->send_buffer, buffer, len);
  Lora->send_buffer_len = len;

  Lora->send_request = false;
  Lora->backup_settings = Lora->settings;            // Make a copy;
  Lora->send_buffer_step = 2;                        // Send at RX1 and RX2
  Lora->delay_settings_save = ((lorawan_delay + TAS_LORAWAN_RECEIVE_DELAY2) / 1000) +2;  // Delay settings save

  uint32_t delay_rx1 = lorawan_delay - TimePassedSince(Lora->receive_time);
  LoraWan_Send_RX1.once_ms(delay_rx1, LoraWanTickerSend);
  uint32_t delay_rx2 = 0;                            // Skip RX2 to receive early RX1 response from device 
  if (!bitRead(Lora->settings.flags, TAS_LORA_FLAG_SKIP_RX2)) {
    uint32_t delay_rx2 = delay_rx1 + TAS_LORAWAN_RECEIVE_DELAY2;
    LoraWan_Send_RX2.once_ms(delay_rx2, LoraWanTickerSend);  // Retry after 1000 ms
  }
#ifdef USE_LORA_DEBUG
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOR: About to send '%*_H' in %d and (optional) %d ms"),
    Lora->send_buffer_len, Lora->send_buffer, delay_rx1, delay_rx2);
#endif  // USE_LORA_DEBUG
}

/*-------------------------------------------------------------------------------------------*/

size_t LoraWanCFList(uint8_t * CFList, size_t uLen) {
  // Populates CFList for use in JOIN-ACCEPT message to lock device to specific frequencies
  // Returns: Number of bytes added
  if (uLen < 16) return 0;

  uint8_t idx = 0;

  switch (Lora->settings.region) {
    case TAS_LORA_REGION_AU915: {
      uint8_t uChannel   = LoraWanFrequencyToChannel();  // 0..71
      uint8_t uMaskByte  = uChannel /8;              // 0..8
        
      // Add first 10 bytes
      for (uint32_t i = 0; i < 10; i++) {
        CFList[idx++] = (i == uMaskByte) ? (0x01 << uChannel %8) : 0x00;
      }
          
      // Add next 6  bytes
      CFList[idx++] = 0x00;   // RFU
      CFList[idx++] = 0x00;
  
      CFList[idx++] = 0x00;   // RFU
      CFList[idx++] = 0x00;
      CFList[idx++] = 0x00;
  
      CFList[idx++] = 0x01;   // CFListType
      break;
    }
    default: {  // TAS_LORA_REGION_EU868
/*
      uint32_t frequency = (Lora->settings.frequency * 10000);  // 868.1 -> 8681000

      // Add first 15 bytes
      for (uint32_t i = 0; i < 5; i++) {
        CFList[idx++] = frequency;
        CFList[idx++] = frequency >> 8;
        CFList[idx++] = frequency >> 16;
      }
      CFList[idx++] = 0x00;   // CFListType
*/
    }
  }
  return idx;
}

uint32_t LoraWanSpreadingFactorToDataRate(bool downlink) {
  /*
  Returns DLSettings defined as
  Bits  7= RFA
      6:4= RX1DROffset
      3:0= RX2DataRate  DateRate for RX2 window
  */
  switch (Lora->settings.region) {
    case TAS_LORA_REGION_AU915: {
      return downlink ? 8 : 2;                       // AU915 must use DR8 for RX2, and we want to use DR2 for Uplinks
    }
    default: {  // TAS_LORA_REGION_EU868
      if (downlink) {
        return 0;                                    // RX2 = 869.525 MHz, DR0 (SF12, 125kHz)
      } else {
        // Allow only JoinReq message datarates (125kHz bandwidth)
        if (Lora->settings.spreading_factor > 12) {
          Lora->settings.spreading_factor = 12;
        }
        if (Lora->settings.spreading_factor < 7) {
          Lora->settings.spreading_factor = 7;
        }
        Lora->settings.bandwidth = 125;
        return 12 - Lora->settings.spreading_factor;
      }
    }
  }
}

/*********************************************************************************************/

void LoraWanSendAppData(uint32_t node, uint8_t* FRMPayload, uint32_t len) {
  if (len > 15) { return; }                           // M is region specific and depends on DataRate
 
  uint32_t DevAddr = Lora->device_address +node;
  uint16_t FCnt = Lora->settings.end_node[node]->FCntDown++;

  uint8_t Key[TAS_LORAWAN_AES128_KEY_SIZE];
  uint8_t FRMPayload_encrypted[TAS_LORAWAN_AES128_KEY_SIZE +9 +len];
  LoraWanDeriveLegacyAppSKey(node, Key);
  LoraWanEncryptDownlink(Key, DevAddr, FCnt, FRMPayload, len, 0, FRMPayload_encrypted);

  uint32_t data_size = 13 + len;
  uint8_t data[data_size];
//  data[0] = TAS_LORAWAN_MTYPE_UNCONFIRMED_DATA_DOWNLINK << 5;
  data[0] = TAS_LORAWAN_MTYPE_CONFIRMED_DATA_DOWNLINK << 5;  // Dragino suggestion (ACKed in next regular message)
  data[1] = DevAddr;
  data[2] = DevAddr >> 8;
  data[3] = DevAddr >> 16;
  data[4] = DevAddr >> 24;
  data[5] = 0x00;                                    // FCtrl - No FOpts
  data[6] = FCnt;
  data[7] = FCnt >> 8;
  data[8] = 0x01;                                    // FPort no MAC (as used by Dragino)
  for (uint32_t i = 0; i < len; i++) {
    data[9 +i] = FRMPayload_encrypted[i];
  }
  LoraWanDeriveLegacyNwkSKey(node, Key);
  uint32_t MIC = LoraWanComputeLegacyDownlinkMIC(Key, DevAddr, FCnt, data, data_size -4);
  data[data_size -4] = MIC;
  data[data_size -3] = MIC >> 8;
  data[data_size -2] = MIC >> 16;
  data[data_size -1] = MIC >> 24;

  LoraWanSendResponse(data, data_size, 10);
}

/*-------------------------------------------------------------------------------------------*/

void LoraWanSendLinkADRReq(uint32_t node) {
  uint32_t DevAddr = Lora->device_address +node;
  uint16_t FCnt = Lora->settings.end_node[node]->FCntDown++;
  uint8_t NwkSKey[TAS_LORAWAN_AES128_KEY_SIZE];
  LoraWanDeriveLegacyNwkSKey(node, NwkSKey);

  uint8_t data[17];
  data[0] = TAS_LORAWAN_MTYPE_CONFIRMED_DATA_DOWNLINK << 5;
  data[1] = DevAddr;
  data[2] = DevAddr >> 8;
  data[3] = DevAddr >> 16;
  data[4] = DevAddr >> 24;
  data[5] = 0x05;                                    // FCtrl with 5 FOpts
  data[6] = FCnt;
  data[7] = FCnt >> 8;
  data[8] = TAS_LORAWAN_CID_LINK_ADR_REQ;

  // Next 4 bytes are Region Specific
  switch (Lora->settings.region) {
    case TAS_LORA_REGION_AU915: {
      //Ref: https://lora-alliance.org/wp-content/uploads/2020/11/lorawan_regional_parameters_v1.0.3reva_0.pdf 
      //     page 39
      uint8_t uChannel   = LoraWanFrequencyToChannel();  // 0..71
      uint8_t ChMaskCntl = uChannel /16.0;           // 0..4
      uChannel           = uChannel %16;             // 0..15
      uint16_t uMask     = 0x01 << uChannel;

      data[9] =  LoraWanSpreadingFactorToDataRate(false) << 4 | 0x0F;  // Uplink DataRate_TXPower Should be 'DR2' for & 'unchanged' = 0x2F

      data[10] = uMask;                              // ChMask LSB
      data[11] = uMask >> 8;                         // ChMask MSB
      
      data[12] = ChMaskCntl << 4;                    // Redundancy: 
                                                     // bits 7=RFU ------- 6:4=ChMaskCntl ----------- --- 3:0=NbTrans ---
                                                     //      0     000=Mask applies to Channels  0-15 0000 = Use Default
                                                     //            001=Mask applies to Channels 16-31
                                                     //            ....
                                                     //            100=Mask applies to Channels 64-71
      break;
    }
    default: {  // TAS_LORA_REGION_EU868
      data[9]  = LoraWanSpreadingFactorToDataRate(false) << 4 | 0x0F; // Uplink DataRate 3 and unchanged TXPower
      data[10] = 0x01 << LoraWanFrequencyToChannel();  // Single channel
      data[11] = 0x00;
      data[12] = 0x00;                               // ChMaskCntl applies to Channels0..15, NbTrans is default (1)
    }
  }
  
  uint32_t MIC = LoraWanComputeLegacyDownlinkMIC(NwkSKey, DevAddr, FCnt, data, 13);
  data[13] = MIC;
  data[14] = MIC >> 8;
  data[15] = MIC >> 16;
  data[16] = MIC >> 24;

  // A0 F3F51700 05 0000 033F010000 0B2C1B8B
  LoraWanSendResponse(data, sizeof(data), TAS_LORAWAN_RECEIVE_DELAY1);
}

void LoraWanSendMacResponse(uint32_t node, uint8_t* FOpts, uint32_t FCtrl) {
  if (FCtrl > 15) { return; }                                // FOpts = 0..15
 
  uint32_t DevAddr = Lora->device_address +node;
  uint16_t FCnt = Lora->settings.end_node[node]->FCntDown++;
  uint8_t NwkSKey[TAS_LORAWAN_AES128_KEY_SIZE];
  LoraWanDeriveLegacyNwkSKey(node, NwkSKey);

  uint32_t data_size = 12 + FCtrl;
  uint8_t data[data_size];
  data[0] = TAS_LORAWAN_MTYPE_UNCONFIRMED_DATA_DOWNLINK << 5;
  data[1] = DevAddr;
  data[2] = DevAddr >> 8;
  data[3] = DevAddr >> 16;
  data[4] = DevAddr >> 24;
  data[5] = 0x20 + FCtrl;                                    // FCtrl ACK + FOpts
  data[6] = FCnt;
  data[7] = FCnt >> 8;
  for (uint32_t i = 0; i < FCtrl; i++) {
    data[8 +i] = FOpts[i];
  }
  uint32_t MIC = LoraWanComputeLegacyDownlinkMIC(NwkSKey, DevAddr, FCnt, data, data_size -4);
  data[data_size -4] = MIC;
  data[data_size -3] = MIC >> 8;
  data[data_size -2] = MIC >> 16;
  data[data_size -1] = MIC >> 24;

  LoraWanSendResponse(data, data_size, TAS_LORAWAN_RECEIVE_DELAY1);
}

/*-------------------------------------------------------------------------------------------*/

bool LoraWanInput(uint8_t* data, uint32_t packet_size) {
  bool result = false;
  uint32_t MType = data[0] >> 5;                             // Upper three bits (used to be called FType)

  if (TAS_LORAWAN_MTYPE_JOIN_REQUEST == MType) {
    // 0007010000004140A8D64A89710E4140A82893A8AD137F - Dragino LDS02
    // 0000010000004140A889AD5C17544140A849E1B2CAC27B - Dragino LHT52
    // 000600000000161600B51F000000161600FDA5D8127912 - MerryIoT DW10
    uint64_t JoinEUI = (uint64_t)data[ 1]        | ((uint64_t)data[ 2] <<  8) |
                      ((uint64_t)data[ 3] << 16) | ((uint64_t)data[ 4] << 24) |
                      ((uint64_t)data[ 5] << 32) | ((uint64_t)data[ 6] << 40) |
                      ((uint64_t)data[ 7] << 48) | ((uint64_t)data[ 8] << 56);
    uint32_t DevEUIl = (uint32_t)data[ 9]        | ((uint32_t)data[10] <<  8) |  // Use uint32_t to fix easy JSON migration
                      ((uint32_t)data[11] << 16) | ((uint32_t)data[12] << 24);
    uint32_t DevEUIh = (uint32_t)data[13]        | ((uint32_t)data[14] <<  8) |
                      ((uint32_t)data[15] << 16) | ((uint32_t)data[16] << 24);
    uint16_t DevNonce = (uint16_t)data[17] | ((uint16_t)data[18] <<  8);
    uint32_t MIC = (uint32_t)data[19]        | ((uint32_t)data[20] <<  8) |
                  ((uint32_t)data[21] << 16) | ((uint32_t)data[22] << 24);

    for (uint32_t node = 0; node < Lora->nodes; node++) {
      if (bitRead(Lora->settings.end_node[node]->flags, TAS_LORAWAN_FLAG_DISABLED)) { continue; }  // Skip

      uint32_t CalcMIC = LoraWanGenerateMIC(data, 19, Lora->settings.end_node[node]->AppKey);
      if (MIC == CalcMIC) {                                  // Valid MIC based on LoraWanAppKey

        AddLog(LOG_LEVEL_DEBUG, PSTR("LOR: Node %d, JoinEUI %8_H, DevEUIh %08X, DevEUIl %08X, DevNonce %04X, MIC %08X"),
          node +1, (uint8_t*)&JoinEUI, DevEUIh, DevEUIl, DevNonce, MIC);

        Lora->settings.end_node[node]->DevEUIl = DevEUIl;
        Lora->settings.end_node[node]->DevEUIh = DevEUIh;
        Lora->settings.end_node[node]->DevNonce = DevNonce;
        Lora->settings.end_node[node]->FCntUp = 0;
        Lora->settings.end_node[node]->FCntDown = 0;
        bitClear(Lora->settings.end_node[node]->flags, TAS_LORAWAN_FLAG_LINK_ADR_REQ);
        if (Lora->settings.end_node[node]->name.equals(F("0x0000"))) {
          char name[10];
          ext_snprintf_P(name, sizeof(name), PSTR("0x%04X"), Lora->settings.end_node[node]->DevEUIl & 0x0000FFFF);
          Lora->settings.end_node[node]->name = name;
        }

        uint32_t JoinNonce = TAS_LORAWAN_JOINNONCE +node;
        uint32_t DevAddr = Lora->device_address +node;
        uint32_t NetID = TAS_LORAWAN_NETID;
        uint8_t join_data[33] = { 0 };
        uint8_t join_data_index = 0;

        join_data[join_data_index++] = TAS_LORAWAN_MTYPE_JOIN_ACCEPT << 5;      // [0]
        join_data[join_data_index++] = JoinNonce;
        join_data[join_data_index++] = JoinNonce >> 8;
        join_data[join_data_index++] = JoinNonce >> 16;
        join_data[join_data_index++] = NetID;
        join_data[join_data_index++] = NetID >> 8;
        join_data[join_data_index++] = NetID >> 16;
        join_data[join_data_index++] = DevAddr;
        join_data[join_data_index++] = DevAddr >> 8;
        join_data[join_data_index++] = DevAddr >> 16;
        join_data[join_data_index++] = DevAddr >> 24;
        join_data[join_data_index++] = LoraWanSpreadingFactorToDataRate(true);  // DLSettings - Downlink
        join_data[join_data_index++] = 1;                                       // RXDelay; [12]

        // Add CFList to instruct device to use one channel
        uint8_t CFList[16];
        size_t  CFListSize = LoraWanCFList(CFList, sizeof(CFList));
        uint8_t CFListIdx  = 0;
        while (CFListSize > 0 ) {
          join_data[join_data_index++] = CFList[CFListIdx++];  
          CFListSize--;
        }
        
        uint32_t NewMIC = LoraWanGenerateMIC(join_data, join_data_index, Lora->settings.end_node[node]->AppKey);
        join_data[join_data_index++] = NewMIC;
        join_data[join_data_index++] = NewMIC >> 8;
        join_data[join_data_index++] = NewMIC >> 16;
        join_data[join_data_index++] = NewMIC >> 24;   //[16] or [32]
        uint8_t EncData[33];
        EncData[0] = join_data[0];
        LoraWanEncryptJoinAccept(Lora->settings.end_node[node]->AppKey, &join_data[1], join_data_index-1, &EncData[1]);

        // 203106E5000000412E010003017CB31DD4 - Dragino LDS02
        // 2026B4E06C390AFA1B166D465987F31EC4 - Dragino LHT52
        // 203206E5000000422E010003016A210EEA - MerryIoT DW10
        LoraWanSendResponse(EncData, join_data_index, TAS_LORAWAN_JOIN_ACCEPT_DELAY1);

        result = true;
        break;
      }
    }
  }
  else if ((TAS_LORAWAN_MTYPE_UNCONFIRMED_DATA_UPLINK == MType) ||
           (TAS_LORAWAN_MTYPE_CONFIRMED_DATA_UPLINK == MType)) {
    //  0     1 2 3 4   5      6 7   8  9   8      9101112131415...       packet_size -4
    // PHYPayload ----------------------------------------------------------------
    // MHDR  MACPayload ------------------------------------------------  MIC ----
    // MHDR  FHDR -----------------------  FPort  FRMPayload -----------  MIC ----
    // MHDR  DevAddr   FCtrl  FCnt  FOpts  FPort  FRMPayload -----------  MIC ----
    // 1     4         1      2     0..15  0..1   0..N                    4         - Number of octets
    // Not encrypted ---------------------------  Encrypted ------------  Not encr
    //                                                                              - Dragino
    // 40    412E0100  80     2500         0A     6A6FEFD6A16B0C7AC37B    5F95FABC  - decrypt using AppSKey
    // 80    412E0100  80     2A00         0A     A58EF5E0D1DDE03424F0    6F2D56FA  - decrypt using AppSKey
    // 80    412E0100  80     2B00         0A     8F2F0D33E5C5027D57A6    F67C9DFE  - decrypt using AppSKey
    // 80    909AE100  00     0800         0A     EEC4A52568A346A8684E    F2D4BF05
    // 40    412E0100  A0     1800         00     0395                    2C94B1D8  - FCtrl ADR support   , ADRACKReq=0, FPort = 0 -> MAC commands, decrypt using NwkSKey
    // 40    412E0100  A0     7800         00     78C9                    A60D8977  - FCtrl ADR support   , ADRACKReq=0, FPort = 0 -> MAC commands, decrypt using NwkSKey
    // 40    F3F51700  20     0100         00     2A7C                    407036A2  - FCtrl No ADR support, ADRACKReq=0, FPort = 0 -> MAC commands, decrypt using NwkSKey, response after LinkADRReq
    // 40    8CF0DF00  00     0000         02     077C3ED4A9FF674BB1E986  20A8A878  - No FCtrl, FPort = 2, SN50v3 working mode 1 (Default Mode) 11 octet data
    //
    //                                                                              - MerryIoT
    // 40    422E0100  80     0400         78     B9C75DF9E8934C6651      A57DA6B1  - decrypt using AppSKey
    // 40    422E0100  80     0100         CC     7C462537AC00C07F99      5500BF2B  - decrypt using AppSKey
    // 40    422E0100  A2     1800  0307   78     29FBF8FD9227729984      8C71E95B  - FCtrl ADR support, ADRACKReq=0, FOptsLen = 2 -> FOpts = MAC, response after LinkADRReq
    // 40    F4F51700  A2     0200  0307   CC     6517D4AB06D32C9A9F      14CBA305  - FCtrl ADR support, ADRACKReq=0, FOptsLen = 2 -> FOpts = MAC, response after LinkADRReq

    bool bResponseSent = false;                                        // Make sure do not send multiple responses

    uint32_t DevAddr = (uint32_t)data[1] | ((uint32_t)data[2] <<  8) | ((uint32_t)data[3] << 16) | ((uint32_t)data[4] << 24);
    for (uint32_t node = 0; node < Lora->nodes; node++) {
      if (0 == Lora->settings.end_node[node]->DevEUIh)  { continue; }  // No DevEUI so never joined
      if ((Lora->device_address +node) != DevAddr) { continue; }       // Not my device
      if (bitRead(Lora->settings.end_node[node]->flags, TAS_LORAWAN_FLAG_DISABLED)) { continue; }  // Skip

      uint32_t FCtrl = data[5];
      uint32_t FOptsLen = FCtrl & 0x0F;
      uint32_t FCnt = (Lora->settings.end_node[node]->FCntUp & 0xFFFF0000) | data[6] | (data[7] << 8);
      uint8_t* FOpts = &data[8];
      uint32_t FPort = data[8 +FOptsLen];
      uint8_t* FRMPayload = &data[9 +FOptsLen];
      uint32_t payload_len = packet_size -9 -FOptsLen -4;

      uint32_t MIC = (uint32_t)data[packet_size -4] | ((uint32_t)data[packet_size -3] << 8) | ((uint32_t)data[packet_size -2] << 16) | ((uint32_t)data[packet_size -1] << 24);
      uint8_t NwkSKey[TAS_LORAWAN_AES128_KEY_SIZE];
      LoraWanDeriveLegacyNwkSKey(node, NwkSKey);
      uint32_t CalcMIC = LoraWanComputeLegacyUplinkMIC(NwkSKey, DevAddr, FCnt, data, packet_size -4);
      if (MIC != CalcMIC) { continue; }                                // Same device address but never joined

      bool FCtrl_ADR       = bitRead(FCtrl, 7);
      bool FCtrl_ADRACKReq = bitRead(FCtrl, 6);  // Device is requesting a response, so that it knows comms is still up.
                                                 // else device will eventually enter backoff mode and we loose comms
                                                 // ref: https://lora-alliance.org/wp-content/uploads/2021/11/LoRaWAN-Link-Layer-Specification-v1.0.4.pdf
                                                 //      page 19  
                                                 // In testing with a Dragino LHT52 device, FCtrl_ADRACKReq was set after 64 (0x40) uplinks (= 21.3 hrs)
      bool FCtrl_ACK       = bitRead(FCtrl, 5);
      bool Fctrl_ClassB    = bitRead(FCtrl, 4);                        // Ready to receive scheduled downlink pings (Class B only)

/*
      if ((0 == FOptsLen) && (0 == FOpts[0])) {                          // MAC response
        FOptsLen = payload_len;
        FOpts = &data[9];
        payload_len = 0;
      }
*/
      uint8_t payload_decrypted[TAS_LORAWAN_AES128_KEY_SIZE +9 +payload_len];
      if (payload_len) {
        uint8_t Key[TAS_LORAWAN_AES128_KEY_SIZE];
        if (0 == FPort) {
          LoraWanDeriveLegacyNwkSKey(node, Key);
        } else {
          LoraWanDeriveLegacyAppSKey(node, Key);
        }
        LoraWanDecryptUplink(Key, DevAddr, FCnt, FRMPayload, payload_len, 0, payload_decrypted);
      }

      uint32_t org_payload_len = payload_len;                          // Save for logging
      if ((0 == FOptsLen) && (0 == FOpts[0])) {                        // MAC response in payload only
        FOptsLen = payload_len;                                        // Payload length is MAC length
        FOpts = payload_decrypted;                                     // Payload is encrypted MAC
        payload_len = 0;                                               // Payload is MAC only
      }

#ifdef USE_LORA_DEBUG
      AddLog(LOG_LEVEL_DEBUG, PSTR("LOR: DevAddr %08X, FCtrl %0X, FOptsLen %d, FCnt %d, FOpts %*_H, FPort %d, Payload %*_H, Decrypted %*_H, MIC %08X"),
        DevAddr, FCtrl, FOptsLen, FCnt, FOptsLen, FOpts, FPort, org_payload_len, FRMPayload, org_payload_len, payload_decrypted, MIC);
#endif  // USE_LORA_DEBUG

      if (Lora->settings.end_node[node]->FCntUp <= FCnt) {              // Skip re-transmissions
        Lora->rx = false;                                               // Skip RX2 as this is a response from RX1
        Lora->settings.end_node[node]->FCntUp++;
        if (Lora->settings.end_node[node]->FCntUp < FCnt) {             // Report missed frames
          uint32_t FCnt_missed = FCnt - Lora->settings.end_node[node]->FCntUp;
          AddLog(LOG_LEVEL_DEBUG, PSTR("LOR: Missed frames %d"), FCnt_missed);
          if (FCnt_missed > 1) {                                       // Missed two or more frames
            bitClear(Lora->settings.end_node[node]->flags, TAS_LORAWAN_FLAG_LINK_ADR_REQ);  // Resend LinkADRReq
          }
        }
        Lora->settings.end_node[node]->FCntUp = FCnt;

        if (FOptsLen) {
          uint8_t mac_data[16];
          uint32_t mac_data_idx = 0;
          uint32_t i = 0;
          while (i < FOptsLen) {
            if (TAS_LORAWAN_CID_LINK_CHECK_REQ == FOpts[i]) {
              // Used by end-device to validate it's connectivity to a network
              // Need to send Margin/GWCnt
              if (mac_data_idx < 13) {
                mac_data[mac_data_idx++] = TAS_LORAWAN_CID_LINK_CHECK_ANS;
                int snr = Lora->snr;
                if (snr < 0) { snr = 0; }
                mac_data[mac_data_idx++] = snr;                        // Margin - Demodulation margin
                mac_data[mac_data_idx++] = 1;                          // GwCnt - Gateway count
              }
            }
            else if (TAS_LORAWAN_CID_LINK_ADR_ANS == FOpts[i]) {
              i++;
              uint8_t status = FOpts[i];
              AddLog(LOG_LEVEL_DEBUG, PSTR("LOR: MAC LinkADRAns PowerACK %d, DataRateACK %d, ChannelMaskACK %d"), 
                bitRead(status, 2), bitRead(status, 1), bitRead(status, 0));
              bitSet(Lora->settings.end_node[node]->flags, TAS_LORAWAN_FLAG_LINK_ADR_REQ);
            }
            else if (TAS_LORAWAN_CID_DUTY_CYCLE_ANS == FOpts[i]) {
              i++;
            }
            else if (TAS_LORAWAN_CID_RX_PARAM_SETUP_ANS == FOpts[i]) {
              i++;
            }
            else if (TAS_LORAWAN_CID_DEV_STATUS_ANS == FOpts[i]) {
              i++;
              i++;
            }
            else if (TAS_LORAWAN_CID_NEW_CHANNEL_ANS == FOpts[i]) {
              i++;
            }
            else if (TAS_LORAWAN_CID_RX_TIMING_SETUP_ANS == FOpts[i]) {
            }
            else if (TAS_LORAWAN_CID_TX_PARAM_SETUP_ANS == FOpts[i]) {
            }
            else if (TAS_LORAWAN_CID_DL_CHANNEL_ANS == FOpts[i]) {
            }
            else if (TAS_LORAWAN_CID_DEVICE_TIME_REQ == FOpts[i]) {
              // Used by the end-device to request the current GPS time
              // Need to send epoch/fractional second
              if (mac_data_idx < 10) {
                mac_data[mac_data_idx++] = TAS_LORAWAN_CID_DEVICE_TIME_ANS;
                // ToDo - need to find a way how to calculate the leap seconds
                uint32_t gps_time = UtcTime() -315964800 -18 +1;       // Time at RX1
                mac_data[mac_data_idx++] = gps_time;
                mac_data[mac_data_idx++] = gps_time >> 8;
                mac_data[mac_data_idx++] = gps_time >> 16;
                mac_data[mac_data_idx++] = gps_time >> 24;
//                uint32_t frac_time = RtcMillis();
                mac_data[mac_data_idx++] = 0x00;                       // Fractional-second 1/256 sec
              }
            }
            else if (TAS_LORAWAN_CID_PING_SLOT_INFO_REQ == FOpts[i]) {
              i++;                                                    // PingSlotParam
            }
            else if (TAS_LORAWAN_CID_PING_SLOT_CHANNEL_ANS == FOpts[i]) {
              i++;                                                    // Status
            }
            else if (TAS_LORAWAN_CID_BEACON_FREQ_ANS == FOpts[i]) {
              i++;                                                    // Status
            }
            else { 
              // RFU
            }
            i++;
          }
          if (mac_data_idx > 0) {
            bResponseSent = true;
            LoraWanSendMacResponse(node, mac_data, mac_data_idx);
          }
        }

        if (payload_len) {
          LoraNodeData_t node_data;
          node_data.rssi = Lora->rssi;
          node_data.snr = Lora->snr;
          node_data.payload = payload_decrypted;
          node_data.payload_len = payload_len;
          node_data.node = node;
          node_data.FPort = FPort;
#ifdef USE_LORA_DEBUG
          AddLog(LOG_LEVEL_DEBUG, PSTR("LOR: Decode Node %d, FPort %d, Payload %*_H, RSSI %1_f, SNR %1_f"),
            node_data.node +1, node_data.FPort, node_data.payload_len, node_data.payload, &node_data.rssi, &node_data.snr);
#endif  // USE_LORA_DEBUG
          LoraWanDecode(&node_data);

          if (0xA84041 == Lora->settings.end_node[node]->DevEUIh >> 8) {  // Dragino
            // Dragino v1.7 fails to set DR with ADR so set it using serial interface:
            // Password 123456
            // AT+CHS=868100000
            // Start join using reset button
            // AT+CADR=0
            // AT+CDATARATE=3
            bitSet(Lora->settings.end_node[node]->flags, TAS_LORAWAN_FLAG_LINK_ADR_REQ);
          }

          if (TAS_LORAWAN_MTYPE_CONFIRMED_DATA_UPLINK == MType) {
            data[0] = TAS_LORAWAN_MTYPE_UNCONFIRMED_DATA_DOWNLINK << 5;
            data[5] |= 0x20;                                           // FCtrl Set ACK bit
            uint16_t FCnt = Lora->settings.end_node[node]->FCntDown++;
            data[6] = FCnt;
            data[7] = FCnt >> 8;
            uint32_t MIC = LoraWanComputeLegacyDownlinkMIC(NwkSKey, DevAddr, FCnt, data, packet_size -4);
            data[packet_size -4] = MIC;
            data[packet_size -3] = MIC >> 8;
            data[packet_size -2] = MIC >> 16;
            data[packet_size -1] = MIC >> 24;
            bResponseSent = true;
            LoraWanSendResponse(data, packet_size, TAS_LORAWAN_RECEIVE_DELAY1);
          }
        }
        if (TAS_LORAWAN_MTYPE_UNCONFIRMED_DATA_UPLINK == MType) {
          if (!bitRead(Lora->settings.end_node[node]->flags, TAS_LORAWAN_FLAG_LINK_ADR_REQ) &&
              FCtrl_ADR && !FCtrl_ACK) {
            // Try to fix single channel and datarate
            bResponseSent = true;
            LoraWanSendLinkADRReq(node);                               // Resend LinkADRReq
          }
        }
      
        if (!bResponseSent && FCtrl_ADRACKReq ) {
          // FCtrl_ADRACKReq requires we send ANY Class A message. Here's one prepared earlier.
          LoraWanSendLinkADRReq(node);
        }

      } // Not retransmission
      result = true;
      break;
    }
  }
  Lora->receive_time = 0;
  return result;
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

#define D_CMND_LORAWANBRIDGE  "Bridge"
#define D_CMND_LORAWANNODE    "Node"
#define D_CMND_LORAWANAPPKEY  "AppKey"
#define D_CMND_LORAWANNAME    "Name"
#define D_CMND_LORAWANDECODER "Decoder"
#define D_CMND_LORAWANSEND    "Send"

const char kLoraWanCommands[] PROGMEM = "LoRaWan"  // Prefix
  "|" D_CMND_LORAWANBRIDGE "|" D_CMND_LORAWANNODE "|" D_CMND_LORAWANAPPKEY
  "|" D_CMND_LORAWANNAME "|" D_CMND_LORAWANDECODER "|" D_CMND_LORAWANSEND;

void (* const LoraWanCommand[])(void) PROGMEM = {
  &CmndLoraWanBridge, &CmndLoraWanNode, &CmndLoraWanAppKey,
  &CmndLoraWanName, &CmndLoraWanDecoder, &CmndLoraWanSend };

void CmndLoraWanBridge(void) {
  // Enable LoraWan bridge
  //  LoraWanBridge <0|1>
  //  LoraWanBridge            - Show LoraOption1
  //  LoraWanBridge 1          - Set LoraOption1 1 = Enable LoraWanBridge
  uint32_t pindex = 0;
  if (XdrvMailbox.payload >= 0) {
    bitWrite(Lora->settings.flags, pindex, XdrvMailbox.payload);
  }
#ifdef USE_LORAWAN_TEST
  LoraWanTestKeyDerivation();
  LorWanTestUplinkDownlinkEncryption();
  LorWanTestUplinkDownlinkMIC();
#endif  // USE_LORAWAN_TEST
  ResponseCmndChar(GetStateText(bitRead(Lora->settings.flags, pindex)));
}

void CmndLoraWanNode(void) {
  // Disable joined node (for testing purposes on other gateway)
  //  LoraWanNode<1..16> <0|1>
  //  LoraWanNode              - Show current state
  //  LoraWanNode2 1           - Enable node 2
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= Lora->nodes)) {
    uint32_t node = XdrvMailbox.index -1;
    if (XdrvMailbox.data_len) {
      if (1 == XdrvMailbox.payload) {
        bitClear(Lora->settings.end_node[node]->flags, TAS_LORAWAN_FLAG_DISABLED);  // Enable (clears bit)
      } else {
        bitSet(Lora->settings.end_node[node]->flags, TAS_LORAWAN_FLAG_DISABLED);    // Disable (sets bit)
      }
    }
    ResponseCmnd();
    for (uint32_t i = 0; i < Lora->nodes; i++) {
      ResponseAppend_P(PSTR("%s%s%d"), (i) ? "," : "\"", (bitRead(Lora->settings.end_node[i]->flags, TAS_LORAWAN_FLAG_DISABLED)) ? "!" : "", i +1);
    }
    ResponseAppend_P(PSTR("\"}"));
  }
}

void CmndLoraWanAppKey(void) {
  // Configure node 32 digit AppKey
  //  LoraWanAppKey<1..16> <appkey>
  //  LoraWanAppKey            - Show current appkey
  //  LoraWanAppKey2 0123456789abcdef0123456789abcdef
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= Lora->nodes +1)) {
    if (Lora->nodes < XdrvMailbox.index) {
      if (!LoraWanAddNode()) {
        return;                // Memory allocation failed or TAS_LORAWAN_ENDNODES reached
      }
    }
    uint32_t node = XdrvMailbox.index -1;
    if (32 == XdrvMailbox.data_len) {
      HexToBytes(XdrvMailbox.data, Lora->settings.end_node[node]->AppKey, TAS_LORAWAN_AES128_KEY_SIZE);
      if (0 == Lora->settings.end_node[node]->name.length()) {
        Lora->settings.end_node[node]->name = F("0x0000");
      }
    }
    else if (0 == XdrvMailbox.payload) {
      memset(Lora->settings.end_node[node], 0, sizeof(LoraEndNode_t));
    }
    char appkey[33];
    ext_snprintf_P(appkey, sizeof(appkey), PSTR("%16_H"), Lora->settings.end_node[node]->AppKey);
    ResponseCmndIdxChar(appkey);
  }
}

void CmndLoraWanName(void) {
  // Configure node name
  //  LoraName<1..16> <1|name>
  //  LoraWanName              - Show current name
  //  LoraWanName 1            - Set to short DevEUI (or 0x0000 if not yet joined)
  //  LoraWanName2 LDS02a
  //  LoraWanName2 "           - Clear name
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= Lora->nodes)) {
    uint32_t node = XdrvMailbox.index -1;
    if (XdrvMailbox.data_len) {
      if (1 == XdrvMailbox.payload) {
        char name[10];
        ext_snprintf_P(name, sizeof(name), PSTR("0x%04X"), Lora->settings.end_node[node]->DevEUIl & 0x0000FFFF);
        Lora->settings.end_node[node]->name = name;
      } else {
        Lora->settings.end_node[node]->name = ('"' == XdrvMailbox.data[0]) ? "" : XdrvMailbox.data;
      }
    }
    ResponseCmndIdxChar(Lora->settings.end_node[node]->name.c_str());
  }
}

void CmndLoraWanDecoder(void) {
  // Configure node berry decoder name
  //  LoraWanDecoder<1..16> <decoder name>
  //  LoraWanDecoder LDS02     - Set Dragino LDS02 message decoder for node 1
  //  LoraWanDecoder2 DW10     - Set MerryIoT DW10 message decoder for node 2
  //  LoraWanDecoder2 "        - Clear decoder name
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= Lora->nodes)) {
    uint32_t node = XdrvMailbox.index -1;
    if (XdrvMailbox.data_len) {
      Lora->settings.end_node[node]->decoder = ('"' == XdrvMailbox.data[0]) ? "" : XdrvMailbox.data;
    }
    ResponseCmndIdxChar(Lora->settings.end_node[node]->decoder.c_str());
  }
}

void CmndLoraWanSend(void) {
  // Send downlink command to node. Works in Class C (which uses more power)
  //  LoraWanSend<1..16> <hexdata>
  //  LoraWanSend3 260014      - Set dragino RJTDC time interval to 20 minutes
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= Lora->nodes)) {
    uint32_t node = XdrvMailbox.index -1;
    if (XdrvMailbox.data_len) {
      char *codes = RemoveSpace(XdrvMailbox.data);
      int size = strlen(XdrvMailbox.data);
      if (size > 30) { return; }  // M is region specific and depends on DataRate
      char *p;
      char stemp[3];
      uint8_t code;
      uint8_t data[16];
      uint32_t data_idx = 0;
      while (size > 1) {
        strlcpy(stemp, codes, sizeof(stemp));
        code = strtol(stemp, &p, 16);
        data[data_idx++] = code;  // "260014" as hex values
        size -= 2;
        codes += 2;
      }
      Lora->receive_time = millis();
      LoraWanSendAppData(node, data, data_idx);
      ResponseCmndDone();
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

void LoraWanInit(void) {
  // The Things Network has been assigned a 7-bits "device address prefix" a.k.a. NwkID
  // %0010011. Using that, TTN currently sends NetID 0x000013, and a TTN DevAddr always
  // starts with 0x26 or 0x27
  // Private networks are supposed to used NetID 0x000000.
  Lora->device_address = (TAS_LORAWAN_NETID << 25) | (ESP_getChipId() & 0x01FFFFFF);
}

#endif  // USE_LORAWAN_BRIDGE
#endif  // USE_SPI_LORA