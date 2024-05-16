/*
  xdrv_73_8_lorawan_bridge.ino - LoRaWan EU868 support for Tasmota

  SPDX-FileCopyrightText: 2024 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_SPI_LORA
#ifdef USE_LORAWAN_BRIDGE
/*********************************************************************************************\
 * EU868 LoRaWan bridge support
 * 
 * The goal of the LoRaWan Bridge is to receive from joined LoRaWan End-Devices or Nodes and
 *  provide decrypted MQTT data.
 * 
 * EU868 LoRaWan uses at minimum alternating 3 frequencies and 6 spreadingfactors (datarate or DR)
 *  which makes the use of single fixed frequency and spreadingfactor hardware like
 *  SX127x (LiliGo T3, M5 LoRa868 or RFM95W) or SX126x (LiLiGo T3S3) a challenge.
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
 *      LoRaFrequency 868.1 (or 868.3 and 868.5)
 *      LoRaSpreadingFactor 9 (or 7..12 equals LoRaWan DataRate 5..0)
 *      LoRaBandwidth 125
 *      LoRaCodingRate4 5
 *      LoRaSyncWord 52
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
  char key[12];                                    // Max 99 nodes (drvset73_1 to drvset73_99)
  for (uint32_t n = 0; n < TAS_LORAWAN_ENDNODES; n++) {
    snprintf_P(key, sizeof(key), PSTR(XDRV_73_KEY "_%d"), n +1);

    String json = UfsJsonSettingsRead(key);
    if (json.length() == 0) { continue; }          // Only load used slots

    // {"AppKey":"00000000000000000000000000000000","DevEUI","0000000000000000","DevNonce":0,"FCntUp":0,"FCntDown":0,"Flags":0,"NAME":""}
    JsonParser parser((char*)json.c_str());
    JsonParserObject root = parser.getRootObject();
    if (!root) { continue; }                       // Only load used slots

    const char* app_key = nullptr;
    app_key = root.getStr(PSTR(D_JSON_APPKEY), nullptr);
    if (strlen(app_key)) {
      HexToBytes(app_key, Lora->settings.end_node[n].AppKey, TAS_LORAWAN_AES128_KEY_SIZE);
    }
    Lora->settings.end_node[n].DevEUIh = root.getUInt(PSTR(D_JSON_DEVEUI "h"), Lora->settings.end_node[n].DevEUIh);
    Lora->settings.end_node[n].DevEUIl = root.getUInt(PSTR(D_JSON_DEVEUI "l"), Lora->settings.end_node[n].DevEUIl);
    Lora->settings.end_node[n].DevNonce = root.getUInt(PSTR(D_JSON_DEVNONCE), Lora->settings.end_node[n].DevNonce);
    Lora->settings.end_node[n].FCntUp = root.getUInt(PSTR(D_JSON_FCNTUP), Lora->settings.end_node[n].FCntUp);
    Lora->settings.end_node[n].FCntDown = root.getUInt(PSTR(D_JSON_FCNTDOWN), Lora->settings.end_node[n].FCntDown);
    Lora->settings.end_node[n].flags = root.getUInt(PSTR(D_JSON_FLAGS), Lora->settings.end_node[n].flags);
    const char* name = nullptr;
    name = root.getStr(PSTR(D_JSON_NAME), nullptr);
    if (strlen(app_key)) {
      Lora->settings.end_node[n].name = name;
    }
  }
  return true;
}

bool LoraWanSaveData(void) {
  bool result = false;
  for (uint32_t n = 0; n < TAS_LORAWAN_ENDNODES; n++) {
    if (Lora->settings.end_node[n].AppKey[0] > 0) {  // Only save used slots
      Response_P(PSTR("{\"" XDRV_73_KEY "_%d\":{\"" D_JSON_APPKEY "\":\"%16_H\""
                                              ",\"" D_JSON_DEVEUI "h\":%lu,\"" D_JSON_DEVEUI "l\":%lu"
                                              ",\"" D_JSON_DEVNONCE "\":%u"
                                              ",\"" D_JSON_FCNTUP "\":%u,\"" D_JSON_FCNTDOWN "\":%u"
                                              ",\"" D_JSON_FLAGS "\":%u"
                                              ",\"" D_JSON_NAME "\":\"%s\"}}"),
        n +1,
        Lora->settings.end_node[n].AppKey,
        Lora->settings.end_node[n].DevEUIh, Lora->settings.end_node[n].DevEUIl,
        Lora->settings.end_node[n].DevNonce,
        Lora->settings.end_node[n].FCntUp, Lora->settings.end_node[n].FCntDown,
        Lora->settings.end_node[n].flags,
        Lora->settings.end_node[n].name.c_str());
      result = UfsJsonSettingsWrite(ResponseData());
    }
  }
  return result;
}

void LoraWanDeleteData(void) {
  char key[12];                                    // Max 99 nodes (drvset73_1 to drvset73_99)
  for (uint32_t n = 0; n < TAS_LORAWAN_ENDNODES; n++) {
    snprintf_P(key, sizeof(key), PSTR(XDRV_73_KEY "_%d"), n +1);
    UfsJsonSettingsDelete(key);                    // Use defaults
  }
}
#endif  // USE_UFILESYS

/*********************************************************************************************/

#include <Ticker.h>
Ticker LoraWan_Send;

void LoraWanTickerSend(void) {
  Lora->send_buffer_step--;
  if (1 == Lora->send_buffer_step) {
    Lora->rx = true;                                // Always send during RX1
    Lora->receive_time = 0;                         // Reset receive timer
    LoraWan_Send.once_ms(TAS_LORAWAN_RECEIVE_DELAY2, LoraWanTickerSend);  // Retry after 1000 ms
  }
  if (Lora->rx) {                                   // If received in RX1 do not resend in RX2
    LoraSend(Lora->send_buffer, Lora->send_buffer_len, true);
  }
}

void LoraWanSendResponse(uint8_t* buffer, size_t len, uint32_t lorawan_delay) {
  free(Lora->send_buffer);                          // Free previous buffer (if any)
  Lora->send_buffer = (uint8_t*)malloc(len +1);
  if (nullptr == Lora->send_buffer) { return; }
  memcpy(Lora->send_buffer, buffer, len);
  Lora->send_buffer_len = len;
  Lora->send_buffer_step = 2;                       // Send at RX1 and RX2
  LoraWan_Send.once_ms(lorawan_delay - TimePassedSince(Lora->receive_time), LoraWanTickerSend);
}

/*-------------------------------------------------------------------------------------------*/

uint32_t LoraWanSpreadingFactorToDataRate(void) {
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

uint32_t LoraWanFrequencyToChannel(void) {
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
    Lora->Config();
    channel = 0;
  }
  return channel;
}

/*********************************************************************************************/

void LoraWanSendLinkADRReq(uint32_t node) {
  uint32_t DevAddr = Lora->device_address +node;
  uint16_t FCnt = Lora->settings.end_node[node].FCntDown++;
  uint8_t NwkSKey[TAS_LORAWAN_AES128_KEY_SIZE];
  LoraWanDeriveLegacyNwkSKey(node, NwkSKey);

  uint8_t data[17];
  data[0] = TAS_LORAWAN_MTYPE_CONFIRMED_DATA_DOWNLINK << 5;
  data[1] = DevAddr;
  data[2] = DevAddr >> 8;
  data[3] = DevAddr >> 16;
  data[4] = DevAddr >> 24;
  data[5] = 0x05;                                            // FCtrl with 5 FOpts
  data[6] = FCnt;
  data[7] = FCnt >> 8;
  data[8] = TAS_LORAWAN_CID_LINK_ADR_REQ;
  data[9] = LoraWanSpreadingFactorToDataRate() << 4 | 0x0F;  // DataRate 3 and unchanged TXPower
  data[10] = 0x01 << LoraWanFrequencyToChannel();            // Single channel
  data[11] = 0x00;
  data[12] = 0x00;                                           // ChMaskCntl applies to Channels0..15, NbTrans is default (1)

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
  uint16_t FCnt = Lora->settings.end_node[node].FCntDown++;
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

bool LoraWanInput(uint8_t* data, uint32_t packet_size) {
  bool result = false;
  uint32_t MType = data[0] >> 5;                             // Upper three bits (used to be called FType)

  if (TAS_LORAWAN_MTYPE_JOIN_REQUEST == MType) {
    // 0007010000004140A8D64A89710E4140A82893A8AD137F - Dragino
    // 000600000000161600B51F000000161600FDA5D8127912 - MerryIoT
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

    for (uint32_t node = 0; node < TAS_LORAWAN_ENDNODES; node++) {
      uint32_t CalcMIC = LoraWanGenerateMIC(data, 19, Lora->settings.end_node[node].AppKey);
      if (MIC == CalcMIC) {                                  // Valid MIC based on LoraWanAppKey

        AddLog(LOG_LEVEL_DEBUG, PSTR("LOR: JoinEUI %8_H, DevEUIh %08X, DevEUIl %08X, DevNonce %04X, MIC %08X"),
          (uint8_t*)&JoinEUI, DevEUIh, DevEUIl, DevNonce, MIC);

        Lora->settings.end_node[node].DevEUIl = DevEUIl;
        Lora->settings.end_node[node].DevEUIh = DevEUIh;
        Lora->settings.end_node[node].DevNonce = DevNonce;
        Lora->settings.end_node[node].FCntUp = 0;
        Lora->settings.end_node[node].FCntDown = 0;
        bitClear(Lora->settings.end_node[node].flags, TAS_LORAWAN_FLAG_LINK_ADR_REQ);
        if (Lora->settings.end_node[node].name.equals(F("0x0000"))) {
          char name[10];
          ext_snprintf_P(name, sizeof(name), PSTR("0x%04X"), Lora->settings.end_node[node].DevEUIl & 0x0000FFFF);
          Lora->settings.end_node[node].name = name;
        }

        uint32_t JoinNonce = TAS_LORAWAN_JOINNONCE +node;
        uint32_t DevAddr = Lora->device_address +node;
        uint32_t NetID = TAS_LORAWAN_NETID;
        uint8_t join_data[33] = { 0 };
        join_data[0] = TAS_LORAWAN_MTYPE_JOIN_ACCEPT << 5;
        join_data[1] = JoinNonce;
        join_data[2] = JoinNonce >> 8;
        join_data[3] = JoinNonce >> 16;
        join_data[4] = NetID;
        join_data[5] = NetID >> 8;
        join_data[6] = NetID >> 16;
        join_data[7] = DevAddr;
        join_data[8] = DevAddr >> 8;
        join_data[9] = DevAddr >> 16;
        join_data[10] = DevAddr >> 24;
        join_data[11] = LoraWanSpreadingFactorToDataRate();  // DLSettings
        join_data[12] = 1;                                   // RXDelay;

        uint32_t NewMIC = LoraWanGenerateMIC(join_data, 13, Lora->settings.end_node[node].AppKey);
        join_data[13] = NewMIC;
        join_data[14] = NewMIC >> 8;
        join_data[15] = NewMIC >> 16;
        join_data[16] = NewMIC >> 24;
        uint8_t EncData[33];
        EncData[0] = join_data[0];
        LoraWanEncryptJoinAccept(Lora->settings.end_node[node].AppKey, &join_data[1], 16, &EncData[1]);

//        AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: Join %17_H"), join_data);

        // 203106E5000000412E010003017CB31DD4 - Dragino
        // 203206E5000000422E010003016A210EEA - MerryIoT
        LoraWanSendResponse(EncData, 17, TAS_LORAWAN_JOIN_ACCEPT_DELAY1);

        result = true;
        break;
      }
    }
  }
  else if ((TAS_LORAWAN_MTYPE_UNCONFIRMED_DATA_UPLINK == MType) ||
           (TAS_LORAWAN_MTYPE_CONFIRMED_DATA_UPLINK == MType)) {
    //  0     1 2 3 4   5      6 7   8  9   8      9101112131415...     packet_size -4
    // PHYPayload --------------------------------------------------------------
    // MHDR  MACPayload ----------------------------------------------  MIC ----
    // MHDR  FHDR -----------------------  FPort  FRMPayload ---------  MIC ----
    // MHDR  DevAddr   FCtrl  FCnt  FOpts  FPort  FRMPayload ---------  MIC ----
    // 1     4         1      2     0..15  0..1   0..N                  4         - Number of octets
    // Not encrypted ---------------------------  Encrypted ----------  Not encr
    //                                                                            - Dragino
    // 40    412E0100  80     2500         0A     6A6FEFD6A16B0C7AC37B  5F95FABC  - decrypt using AppSKey
    // 80    412E0100  80     2A00         0A     A58EF5E0D1DDE03424F0  6F2D56FA  - decrypt using AppSKey
    // 80    412E0100  80     2B00         0A     8F2F0D33E5C5027D57A6  F67C9DFE  - decrypt using AppSKey
    // 80    909AE100  00     0800         0A     EEC4A52568A346A8684E  F2D4BF05
    // 40    412E0100  A0     1800         00     0395                  2C94B1D8  - FCtrl ADR support, Ack, FPort = 0 -> MAC commands, decrypt using NwkSKey
    // 40    412E0100  A0     7800         00     78C9                  A60D8977  - FCtrl ADR support, Ack, FPort = 0 -> MAC commands, decrypt using NwkSKey
    // 40    F3F51700  20     0100         00     2A7C                  407036A2  - FCtrl No ADR support, Ack, FPort = 0 -> MAC commands, decrypt using NwkSKey, response after LinkADRReq
    //                                                                            - MerryIoT
    // 40    422E0100  80     0400         78     B9C75DF9E8934C6651    A57DA6B1  - decrypt using AppSKey
    // 40    422E0100  80     0100         CC     7C462537AC00C07F99    5500BF2B  - decrypt using AppSKey
    // 40    422E0100  A2     1800  0307   78     29FBF8FD9227729984    8C71E95B  - FCtrl ADR support, Ack, FOptsLen = 2 -> FOpts = MAC, response after LinkADRReq
    // 40    F4F51700  A2     0200  0307   CC     6517D4AB06D32C9A9F    14CBA305  - FCtrl ADR support, Ack, FOptsLen = 2 -> FOpts = MAC, response after LinkADRReq

    uint32_t DevAddr = (uint32_t)data[1] | ((uint32_t)data[2] <<  8) | ((uint32_t)data[3] << 16) | ((uint32_t)data[4] << 24);
    for (uint32_t node = 0; node < TAS_LORAWAN_ENDNODES; node++) {
      if (0 == Lora->settings.end_node[node].DevEUIh)  { continue; }   // No DevEUI so never joined
      if ((Lora->device_address +node) != DevAddr) { continue; }       // Not my device

      uint32_t FCtrl = data[5];
      uint32_t FOptsLen = FCtrl & 0x0F;
      uint32_t FCnt = (Lora->settings.end_node[node].FCntUp & 0xFFFF0000) | data[6] | (data[7] << 8);
      uint8_t* FOpts = &data[8];
      uint32_t FPort = data[8 +FOptsLen];
      uint8_t* FRMPayload = &data[9 +FOptsLen];
      uint32_t payload_len = packet_size -9 -FOptsLen -4;

      uint32_t MIC = (uint32_t)data[packet_size -4] | ((uint32_t)data[packet_size -3] << 8) | ((uint32_t)data[packet_size -2] << 16) | ((uint32_t)data[packet_size -1] << 24);
      uint8_t NwkSKey[TAS_LORAWAN_AES128_KEY_SIZE];
      LoraWanDeriveLegacyNwkSKey(node, NwkSKey);
      uint32_t CalcMIC = LoraWanComputeLegacyUplinkMIC(NwkSKey, DevAddr, FCnt, data, packet_size -4);
      if (MIC != CalcMIC) { continue; }                                  // Same device address but never joined

      bool FCtrl_ADR = bitRead(FCtrl, 7);
      bool FCtrl_ACK = bitRead(FCtrl, 5);
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

      if (Lora->settings.end_node[node].FCntUp <= FCnt) {              // Skip re-transmissions
        Lora->rx = false;                                              // Skip RX2 as this is a response from RX1
        Lora->settings.end_node[node].FCntUp++;
        if (Lora->settings.end_node[node].FCntUp < FCnt) {             // Report missed frames
          uint32_t FCnt_missed = FCnt - Lora->settings.end_node[node].FCntUp;
          AddLog(LOG_LEVEL_DEBUG, PSTR("LOR: Missed frames %d"), FCnt_missed);
          if (FCnt_missed > 1) {                                       // Missed two or more frames
            bitClear(Lora->settings.end_node[node].flags, TAS_LORAWAN_FLAG_LINK_ADR_REQ);  // Resend LinkADRReq
          }
        }
        Lora->settings.end_node[node].FCntUp = FCnt;

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
              bitSet(Lora->settings.end_node[node].flags, TAS_LORAWAN_FLAG_LINK_ADR_REQ);
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
            else { 
              // RFU
            }
            i++;
          }
          if (mac_data_idx > 0) {
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
          LoraWanDecode(&node_data);

          if (0xA840410E == Lora->settings.end_node[node].DevEUIh) {   // Dragino
            // Dragino v1.7 fails to set DR with ADR so set it using serial interface:
            // Password 123456
            // AT+CHS=868100000
            // Start join using reset button
            // AT+CADR=0
            // AT+CDATARATE=3
            bitSet(Lora->settings.end_node[node].flags, TAS_LORAWAN_FLAG_LINK_ADR_REQ);
          }

          if (TAS_LORAWAN_MTYPE_CONFIRMED_DATA_UPLINK == MType) {
            data[0] = TAS_LORAWAN_MTYPE_UNCONFIRMED_DATA_DOWNLINK << 5;
            data[5] |= 0x20;                                           // FCtrl Set ACK bit
            uint16_t FCnt = Lora->settings.end_node[node].FCntDown++;
            data[6] = FCnt;
            data[7] = FCnt >> 8;
            uint32_t MIC = LoraWanComputeLegacyDownlinkMIC(NwkSKey, DevAddr, FCnt, data, packet_size -4);
            data[packet_size -4] = MIC;
            data[packet_size -3] = MIC >> 8;
            data[packet_size -2] = MIC >> 16;
            data[packet_size -1] = MIC >> 24;
            LoraWanSendResponse(data, packet_size, TAS_LORAWAN_RECEIVE_DELAY1);
          }
        }
        if (TAS_LORAWAN_MTYPE_UNCONFIRMED_DATA_UPLINK == MType) {
          if (!bitRead(Lora->settings.end_node[node].flags, TAS_LORAWAN_FLAG_LINK_ADR_REQ) &&
              FCtrl_ADR && !FCtrl_ACK) {
            // Try to fix single channel and datarate
            LoraWanSendLinkADRReq(node);                               // Resend LinkADRReq
          }
        }
      }
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
#define D_CMND_LORAWANAPPKEY  "AppKey"
#define D_CMND_LORAWANNAME    "Name"

const char kLoraWanCommands[] PROGMEM = "LoRaWan|"  // Prefix
  D_CMND_LORAWANBRIDGE "|" D_CMND_LORAWANAPPKEY "|" D_CMND_LORAWANNAME;

void (* const LoraWanCommand[])(void) PROGMEM = {
  &CmndLoraWanBridge, &CmndLoraWanAppKey, &CmndLoraWanName };

void CmndLoraWanBridge(void) {
  // LoraWanBridge   - Show LoraOption1
  // LoraWanBridge 1 - Set LoraOption1 1 = Enable LoraWanBridge
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

void CmndLoraWanAppKey(void) {
  // LoraWanAppKey
  // LoraWanAppKey2 0123456789abcdef0123456789abcdef
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TAS_LORAWAN_ENDNODES)) {
    uint32_t node = XdrvMailbox.index -1;
    if (32 == XdrvMailbox.data_len) {
      HexToBytes(XdrvMailbox.data, Lora->settings.end_node[node].AppKey, TAS_LORAWAN_AES128_KEY_SIZE);
      if (0 == Lora->settings.end_node[node].name.length()) {
        Lora->settings.end_node[node].name = F("0x0000");
      }
    }
    else if (0 == XdrvMailbox.payload) {
      memset(&Lora->settings.end_node[node], 0, sizeof(LoraEndNode_t));
    }
    char appkey[33];
    ext_snprintf_P(appkey, sizeof(appkey), PSTR("%16_H"), Lora->settings.end_node[node].AppKey);
    ResponseCmndIdxChar(appkey);
  }
}

void CmndLoraWanName(void) {
  // LoraWanName
  // LoraWanName 1        - Set to short DevEUI (or 0x0000 if not yet joined)
  // LoraWanName2 LDS02a
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TAS_LORAWAN_ENDNODES)) {
    uint32_t node = XdrvMailbox.index -1;
    if (XdrvMailbox.data_len) {
      if (1 == XdrvMailbox.payload) {
        char name[10];
        ext_snprintf_P(name, sizeof(name), PSTR("0x%04X"), Lora->settings.end_node[node].DevEUIl & 0x0000FFFF);
        Lora->settings.end_node[node].name = name;
      } else {
        Lora->settings.end_node[node].name = XdrvMailbox.data;
      }
    }
    ResponseCmndIdxChar(Lora->settings.end_node[node].name.c_str());
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
