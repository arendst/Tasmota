#include "LoRaWAN.h"
#include <string.h>
//#if defined(ESP32)
//#include "esp_attr.h"
//#endif

#if !RADIOLIB_EXCLUDE_LORAWAN

#if defined(RADIOLIB_EEPROM_UNSUPPORTED)
  #warning "Persistent storage not supported!"
#endif

// flag to indicate whether there was some action during Rx mode (timeout or downlink)
static volatile bool downlinkAction = false;

// interrupt service routine to handle downlinks automatically
#if defined(ESP8266) || defined(ESP32)
  IRAM_ATTR
#endif
static void LoRaWANNodeOnDownlinkAction(void) {
  downlinkAction = true;
}

uint8_t getDownlinkDataRate(uint8_t uplink, uint8_t offset, uint8_t base, uint8_t min, uint8_t max) {
  int8_t dr = uplink - offset + base;
  if(dr < min) {
    dr = min;
  } else if (dr > max) {
    dr = max;
  }
  return(dr);
}

LoRaWANNode::LoRaWANNode(PhysicalLayer* phy, const LoRaWANBand_t* band, uint8_t subBand) {
  this->phyLayer = phy;
  this->band = band;
  this->rx2 = this->band->rx2;
  this->txPowerMax = this->band->powerMax;
  this->subBand = subBand;
  this->difsSlots = 2;
  this->backoffMax = 6;
  this->enableCSMA = false;
}

void LoRaWANNode::setCSMA(uint8_t backoffMax, uint8_t difsSlots, bool enableCSMA) {
    this->backoffMax = backoffMax;
    this->difsSlots = difsSlots;
    this->enableCSMA = enableCSMA;
}

#if !defined(RADIOLIB_EEPROM_UNSUPPORTED)
void LoRaWANNode::wipe() {
  Module* mod = this->phyLayer->getMod();
  mod->hal->wipePersistentStorage();
}

int16_t LoRaWANNode::restore() {
  // if already joined, ignore
  if(this->activeMode != RADIOLIB_LORAWAN_MODE_NONE) {
    return(this->activeMode);
  }

  Module* mod = this->phyLayer->getMod();

  uint8_t nvm_table_version = mod->hal->getPersistentParameter<uint8_t>(RADIOLIB_EEPROM_TABLE_VERSION_ID);
  // if (RADIOLIB_EEPROM_LORAWAN_TABLE_VERSION > nvm_table_version) {
  //  // set default values for variables that are new or something
  // }
  (void)nvm_table_version;

  // check the mode value
  uint16_t lwMode = mod->hal->getPersistentParameter<uint16_t>(RADIOLIB_EEPROM_LORAWAN_MODE_ID);
  if(lwMode == RADIOLIB_LORAWAN_MODE_NONE) {
    #if RADIOLIB_DEBUG
      RADIOLIB_DEBUG_PRINTLN("mode value not set (no saved session)");
      RADIOLIB_DEBUG_PRINTLN("first 16 bytes of NVM:");
      uint8_t nvmBuff[16];
      mod->hal->readPersistentStorage(mod->hal->getPersistentAddr(0), nvmBuff, 16);
      RADIOLIB_DEBUG_HEXDUMP(nvmBuff, 16);
    #endif
    // the mode value is not set, user will have to do perform the join procedure
    return(RADIOLIB_ERR_NETWORK_NOT_JOINED);
  }
  
  // pull all authentication keys from persistent storage
  this->devAddr = mod->hal->getPersistentParameter<uint32_t>(RADIOLIB_EEPROM_LORAWAN_DEV_ADDR_ID);
  mod->hal->readPersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_APP_S_KEY_ID), this->appSKey, RADIOLIB_AES128_BLOCK_SIZE);
  mod->hal->readPersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_FNWK_SINT_KEY_ID), this->fNwkSIntKey, RADIOLIB_AES128_BLOCK_SIZE);
  mod->hal->readPersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_SNWK_SINT_KEY_ID), this->sNwkSIntKey, RADIOLIB_AES128_BLOCK_SIZE);
  mod->hal->readPersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_NWK_SENC_KEY_ID), this->nwkSEncKey, RADIOLIB_AES128_BLOCK_SIZE);

  // get session parameters
  this->rev          = mod->hal->getPersistentParameter<uint8_t>(RADIOLIB_EEPROM_LORAWAN_VERSION_ID);
  RADIOLIB_DEBUG_PRINTLN("LoRaWAN session: v1.%d", this->rev);
  this->devNonce     = mod->hal->getPersistentParameter<uint16_t>(RADIOLIB_EEPROM_LORAWAN_DEV_NONCE_ID);
  this->joinNonce    = mod->hal->getPersistentParameter<uint32_t>(RADIOLIB_EEPROM_LORAWAN_JOIN_NONCE_ID);
  this->aFcntDown    = mod->hal->getPersistentParameter<uint32_t>(RADIOLIB_EEPROM_LORAWAN_A_FCNT_DOWN_ID);
  this->nFcntDown    = mod->hal->getPersistentParameter<uint32_t>(RADIOLIB_EEPROM_LORAWAN_N_FCNT_DOWN_ID);
  this->confFcntUp   = mod->hal->getPersistentParameter<uint32_t>(RADIOLIB_EEPROM_LORAWAN_CONF_FCNT_UP_ID);
  this->confFcntDown = mod->hal->getPersistentParameter<uint32_t>(RADIOLIB_EEPROM_LORAWAN_CONF_FCNT_DOWN_ID);
  this->adrFcnt      = mod->hal->getPersistentParameter<uint32_t>(RADIOLIB_EEPROM_LORAWAN_ADR_FCNT_ID);
  
  // fcntUp is stored in highly efficient wear-leveling system, so parse it
  this->restoreFcntUp();

  // get the defined channels
  int16_t state = this->restoreChannels();
  RADIOLIB_ASSERT(state);

  // get MAC state
  LoRaWANMacCommand_t cmd = {
    .cid = RADIOLIB_LORAWAN_MAC_LINK_ADR,
    .payload = { 0 }, 
    .len = MacTable[RADIOLIB_LORAWAN_MAC_LINK_ADR].lenDn,
    .repeat = 0,
  };

  // only apply the single ADR command on dynamic bands; fixed bands is done through channel restore
  if(this->band->bandType == RADIOLIB_LORAWAN_BAND_DYNAMIC) {
    mod->hal->readPersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_LINK_ADR_ID), cmd.payload, cmd.len);
    execMacCommand(&cmd, false);
  }

  cmd.cid = RADIOLIB_LORAWAN_MAC_DUTY_CYCLE;
  cmd.len = MacTable[RADIOLIB_LORAWAN_MAC_DUTY_CYCLE].lenDn;
  mod->hal->readPersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_DUTY_CYCLE_ID), cmd.payload, cmd.len);
  execMacCommand(&cmd, false);

  cmd.cid = RADIOLIB_LORAWAN_MAC_RX_PARAM_SETUP;
  cmd.len = MacTable[RADIOLIB_LORAWAN_MAC_RX_PARAM_SETUP].lenDn;
  mod->hal->readPersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_RX_PARAM_SETUP_ID), cmd.payload, cmd.len);
  execMacCommand(&cmd, false);

  cmd.cid = RADIOLIB_LORAWAN_MAC_RX_TIMING_SETUP;
  cmd.len = MacTable[RADIOLIB_LORAWAN_MAC_RX_TIMING_SETUP].lenDn;
  mod->hal->readPersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_RX_TIMING_SETUP_ID), cmd.payload, cmd.len);
  execMacCommand(&cmd, false);

  cmd.cid = RADIOLIB_LORAWAN_MAC_TX_PARAM_SETUP;
  cmd.len = MacTable[RADIOLIB_LORAWAN_MAC_TX_PARAM_SETUP].lenDn;
  mod->hal->readPersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_TX_PARAM_SETUP_ID), cmd.payload, cmd.len);
  execMacCommand(&cmd, false);

  cmd.cid = RADIOLIB_LORAWAN_MAC_ADR_PARAM_SETUP;
  cmd.len = MacTable[RADIOLIB_LORAWAN_MAC_ADR_PARAM_SETUP].lenDn;
  mod->hal->readPersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_ADR_PARAM_SETUP_ID), cmd.payload, cmd.len);
  execMacCommand(&cmd, false);

  cmd.cid = RADIOLIB_LORAWAN_MAC_REJOIN_PARAM_SETUP;
  cmd.len = MacTable[RADIOLIB_LORAWAN_MAC_REJOIN_PARAM_SETUP].lenDn;
  mod->hal->readPersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_REJOIN_PARAM_SETUP_ID), cmd.payload, cmd.len);
  execMacCommand(&cmd, false);

  uint8_t queueBuff[sizeof(LoRaWANMacCommandQueue_t)] = { 0 };
  mod->hal->readPersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_MAC_QUEUE_UL_ID), queueBuff, sizeof(LoRaWANMacCommandQueue_t));
  memcpy(&this->commandsUp, queueBuff, sizeof(LoRaWANMacCommandQueue_t));
  RADIOLIB_DEBUG_PRINTLN("Number of MAC commands: %d", this->commandsUp.numCommands);

  state = this->setPhyProperties();
  RADIOLIB_ASSERT(state);

  // full session is restored, so set joined flag to whichever mode is restored
  this->activeMode = lwMode;

  return(this->activeMode);
}

int16_t LoRaWANNode::restoreFcntUp() {
  Module* mod = this->phyLayer->getMod();

  uint8_t fcntBuffStart = mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_FCNT_UP_ID);
  uint8_t fcntBuffEnd = mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_FCNT_UP_ID + 1);
  uint8_t buffSize = fcntBuffEnd - fcntBuffStart;
  #if RADIOLIB_STATIC_ONLY
  uint8_t fcntBuff[RADIOLIB_STATIC_ARRAY_SIZE];
  #else
  uint8_t* fcntBuff = new uint8_t[buffSize];
  #endif
  mod->hal->readPersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_FCNT_UP_ID), fcntBuff, buffSize);

  // copy the two most significant bytes from the first two bytes
  uint32_t bits_30_22 = (uint32_t)fcntBuff[0];
  uint32_t bits_22_14 = (uint32_t)fcntBuff[1];

  // the next 7 bits must be retrieved from the byte to which was written most recently
  // this is the last byte that has its state bit (most significant bit) set equal to its predecessor
  // we find the first byte that has its state bit different, and subtract one
  uint8_t idx = 2;
  uint8_t state = fcntBuff[idx] >> 7;
  for(; idx < 5; idx++) {
    if(fcntBuff[idx] >> 7 != state) {
      break;
    }
  }
  uint32_t bits_14_7 = (uint32_t)fcntBuff[idx-1] & 0x7F;

  // equally, the last 7 bits must be retrieved from the byte to which was written most recently
  // this is the last byte that has its state bit (most significant bit) set equal to its predecessor
  // we find the first byte that has its state bit different, and subtract one
  idx = 5;
  state = fcntBuff[idx] >> 7;
  for(; idx < buffSize; idx++) {
    if(fcntBuff[idx] >> 7 != state) {
      break;
    }
  }
  uint32_t bits_7_0 = (uint32_t)fcntBuff[idx-1] & 0x7F;
  #if !RADIOLIB_STATIC_ONLY
  delete[] fcntBuff;
  #endif

  this->fcntUp = (bits_30_22 << 22) | (bits_22_14 << 14) | (bits_14_7 << 7) | bits_7_0;
  return(RADIOLIB_ERR_NONE);
}

int16_t LoRaWANNode::restoreChannels() {
  // first do the default channels, in case these are not covered by restored channels
  if(this->band->bandType == RADIOLIB_LORAWAN_BAND_DYNAMIC) {
    this->setupChannelsDyn(false);
  } else {                // RADIOLIB_LORAWAN_BAND_FIXED
    this->setupChannelsFix(this->subBand);
  }

  Module* mod = this->phyLayer->getMod();
  uint8_t bufferZeroes[5] = { 0 };
  if(this->band->bandType == RADIOLIB_LORAWAN_BAND_DYNAMIC) {
    uint8_t numBytesUp = RADIOLIB_LORAWAN_NUM_AVAILABLE_CHANNELS * MacTable[RADIOLIB_LORAWAN_MAC_NEW_CHANNEL].lenDn;
    uint8_t bufferUp[RADIOLIB_LORAWAN_NUM_AVAILABLE_CHANNELS * RADIOLIB_LORAWAN_MAX_MAC_COMMAND_LEN_DOWN] = { 0 };
    mod->hal->readPersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_UL_CHANNELS_ID), bufferUp, numBytesUp);
    
    LoRaWANMacCommand_t cmd = { .cid = RADIOLIB_LORAWAN_MAC_NEW_CHANNEL, .payload = { 0 }, .len = 0, .repeat = 0 };
    for(int i = 0; i < RADIOLIB_LORAWAN_NUM_AVAILABLE_CHANNELS; i++) {
      cmd.len = MacTable[RADIOLIB_LORAWAN_MAC_NEW_CHANNEL].lenDn;
      memcpy(cmd.payload, &(bufferUp[i * cmd.len]), cmd.len);
      if(memcmp(cmd.payload, bufferZeroes, cmd.len) != 0) { // only execute if it is not all zeroes
        cmd.repeat = 1;
        (void)execMacCommand(&cmd, false);
      }
    }

    uint8_t numBytesDn = RADIOLIB_LORAWAN_NUM_AVAILABLE_CHANNELS * MacTable[RADIOLIB_LORAWAN_MAC_DL_CHANNEL].lenDn;
    uint8_t bufferDn[RADIOLIB_LORAWAN_NUM_AVAILABLE_CHANNELS * RADIOLIB_LORAWAN_MAX_MAC_COMMAND_LEN_DOWN] = { 0 };
    mod->hal->readPersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_DL_CHANNELS_ID), bufferDn, numBytesDn);
    
    cmd.cid = RADIOLIB_LORAWAN_MAC_DL_CHANNEL;
    
    for(int i = 0; i < RADIOLIB_LORAWAN_NUM_AVAILABLE_CHANNELS; i++) {
      cmd.len = MacTable[RADIOLIB_LORAWAN_MAC_DL_CHANNEL].lenDn;
      memcpy(cmd.payload, &bufferDn[i * cmd.len], cmd.len);
      if(memcmp(cmd.payload, bufferZeroes, cmd.len) != 0) { // only execute if it is not all zeroes
        (void)execMacCommand(&cmd, false);
      }
    }

  } else {  // RADIOLIB_LORAWAN_BAND_FIXED
    uint8_t numADRCommands = mod->hal->getPersistentParameter<uint8_t>(RADIOLIB_EEPROM_LORAWAN_NUM_ADR_MASKS_ID);
    uint8_t numBytes = numADRCommands * MacTable[RADIOLIB_LORAWAN_MAC_LINK_ADR].lenDn;
    uint8_t buffer[RADIOLIB_LORAWAN_MAX_NUM_ADR_COMMANDS * RADIOLIB_LORAWAN_MAX_MAC_COMMAND_LEN_DOWN] = { 0 };
    mod->hal->readPersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_UL_CHANNELS_ID), buffer, numBytes);
    
    LoRaWANMacCommand_t cmd = {
      .cid = RADIOLIB_LORAWAN_MAC_LINK_ADR,
      .payload = { 0 }, 
      .len = 0,
      .repeat = 0,
    };
    
    for(int i = 0; i < numADRCommands; i++) {
      cmd.len = MacTable[RADIOLIB_LORAWAN_MAC_LINK_ADR].lenDn;
      memcpy(cmd.payload, &buffer[i * cmd.len], cmd.len);
      // there COULD, according to spec, be an all zeroes ADR command - meh
      if(memcmp(cmd.payload, bufferZeroes, cmd.len) != 0) {
        cmd.repeat = (i+1);
        execMacCommand(&cmd, false);
      }
    }
  }
  return(RADIOLIB_ERR_NONE);
}
#endif

void LoRaWANNode::beginCommon(uint8_t joinDr) {
  // in case a new session is started while there is an ongoing session
  // clear the MAC queues completely
  memset(&(this->commandsUp), 0, sizeof(LoRaWANMacCommandQueue_t));
  memset(&(this->commandsDown), 0, sizeof(LoRaWANMacCommandQueue_t));

  LoRaWANMacCommand_t cmd = {
    .cid = RADIOLIB_LORAWAN_MAC_LINK_ADR,
    .payload = { 0 }, 
    .len = MacTable[RADIOLIB_LORAWAN_MAC_LINK_ADR].lenDn,
    .repeat = 0,
  };
  if(this->band->bandType == RADIOLIB_LORAWAN_BAND_DYNAMIC) {
    uint8_t drUp = 0;
    // if join datarate is user-specified and valid, select that value; otherwise use
    if(joinDr != RADIOLIB_LORAWAN_DATA_RATE_UNUSED) {
      if(joinDr >= this->band->txFreqs[0].drMin && joinDr <= this->band->txFreqs[0].drMax) {
        drUp = joinDr;
      } else {
        RADIOLIB_DEBUG_PRINTLN("Datarate %d is not valid (min: %d, max %d) - using default", 
                                joinDr, this->band->txFreqs[0].drMin, this->band->txFreqs[0].drMax);
        joinDr = RADIOLIB_LORAWAN_DATA_RATE_UNUSED;
      }
    } 
    if(joinDr == RADIOLIB_LORAWAN_DATA_RATE_UNUSED) {
      drUp = (this->band->txFreqs[0].drMin + this->band->txFreqs[0].drMax) / 2;
    }
    cmd.payload[0] = (drUp << 4);
  } else {
    uint8_t drJr = this->band->txSpans[0].joinRequestDataRate;
    cmd.payload[0] = (drJr << 4);
  }
  cmd.payload[0] |= 0;                // default to max Tx Power
  cmd.payload[3]  = (1 << 7);         // set the RFU bit, which means that the channel mask gets ignored
  (void)execMacCommand(&cmd);

  cmd.cid = RADIOLIB_LORAWAN_MAC_DUTY_CYCLE;
  cmd.len = MacTable[RADIOLIB_LORAWAN_MAC_DUTY_CYCLE].lenDn;
  uint8_t maxDCyclePower;
  switch(this->band->dutyCycle) {
    case(0):
      maxDCyclePower = 0;
      break;
    case(3600):
      maxDCyclePower = 10;
      break;
    case(36000):
      maxDCyclePower = 7;
      break;
    default:
      maxDCyclePower = 0;
      break;
  }
  cmd.payload[0]  = maxDCyclePower;
  (void)execMacCommand(&cmd);

  cmd.cid = RADIOLIB_LORAWAN_MAC_RX_PARAM_SETUP;
  cmd.len = MacTable[RADIOLIB_LORAWAN_MAC_RX_PARAM_SETUP].lenDn;
  cmd.payload[0]  = (RADIOLIB_LORAWAN_RX1_DR_OFFSET << 4);
  cmd.payload[0] |= this->rx2.drMax; // may be set by user, otherwise band's default upon initialization
  uint32_t rx2Freq = uint32_t(this->rx2.freq * 10000);
  LoRaWANNode::hton<uint32_t>(&cmd.payload[1], rx2Freq, 3);
  (void)execMacCommand(&cmd);

  cmd.cid = RADIOLIB_LORAWAN_MAC_RX_TIMING_SETUP;
  cmd.len = MacTable[RADIOLIB_LORAWAN_MAC_RX_TIMING_SETUP].lenDn;
  cmd.payload[0]  = (RADIOLIB_LORAWAN_RECEIVE_DELAY_1_MS / 1000);
  (void)execMacCommand(&cmd);

  cmd.cid = RADIOLIB_LORAWAN_MAC_TX_PARAM_SETUP;
  cmd.len = MacTable[RADIOLIB_LORAWAN_MAC_TX_PARAM_SETUP].lenDn;
  cmd.payload[0]  = (this->band->dwellTimeDn > 0 ? 1 : 0) << 5;
  cmd.payload[0] |= (this->band->dwellTimeUp > 0 ? 1 : 0) << 4;
  uint8_t maxEIRPRaw;
  switch(this->band->powerMax) {
    case(12):
      maxEIRPRaw = 2;
      break;
    case(14):
      maxEIRPRaw = 4;
      break;
    case(16):
      maxEIRPRaw = 5;
      break;
    case(19):   // this option does not exist for the TxParamSetupReq but will be caught during execution
      maxEIRPRaw = 7;
      break;
    case(30):
      maxEIRPRaw = 13;
      break;
    default:
      maxEIRPRaw = 2;
      break;
  }
  cmd.payload[0] |= maxEIRPRaw;
  (void)execMacCommand(&cmd);

  cmd.cid = RADIOLIB_LORAWAN_MAC_ADR_PARAM_SETUP;
  cmd.len = MacTable[RADIOLIB_LORAWAN_MAC_ADR_PARAM_SETUP].lenDn;
  cmd.payload[0]  = (RADIOLIB_LORAWAN_ADR_ACK_LIMIT_EXP << 4);
  cmd.payload[0] |= RADIOLIB_LORAWAN_ADR_ACK_DELAY_EXP;
  (void)execMacCommand(&cmd);

  cmd.cid = RADIOLIB_LORAWAN_MAC_REJOIN_PARAM_SETUP;
  cmd.len = MacTable[RADIOLIB_LORAWAN_MAC_REJOIN_PARAM_SETUP].lenDn;
  cmd.payload[0]  = (RADIOLIB_LORAWAN_REJOIN_MAX_TIME_N << 4);
  cmd.payload[0] |= RADIOLIB_LORAWAN_REJOIN_MAX_COUNT_N;
  (void)execMacCommand(&cmd);
}

int16_t LoRaWANNode::beginOTAA(uint64_t joinEUI, uint64_t devEUI, uint8_t* nwkKey, uint8_t* appKey, uint8_t joinDr, bool force) {
  // check if we actually need to send the join request
  Module* mod = this->phyLayer->getMod();

#if !defined(RADIOLIB_EEPROM_UNSUPPORTED)
  uint16_t checkSum = 0;
  checkSum ^= LoRaWANNode::checkSum16(reinterpret_cast<uint8_t*>(&joinEUI), 8);
  checkSum ^= LoRaWANNode::checkSum16(reinterpret_cast<uint8_t*>(&devEUI), 8);
  checkSum ^= LoRaWANNode::checkSum16(nwkKey, 16);
  checkSum ^= LoRaWANNode::checkSum16(appKey, 16);

  bool validCheckSum = mod->hal->getPersistentParameter<uint16_t>(RADIOLIB_EEPROM_LORAWAN_CHECKSUM_ID) == checkSum;
  bool validMode = mod->hal->getPersistentParameter<uint16_t>(RADIOLIB_EEPROM_LORAWAN_MODE_ID) == RADIOLIB_LORAWAN_MODE_OTAA;

  if(validCheckSum && validMode) {
    if(!force) {
      // the device has joined already, we can just pull the data from persistent storage
      RADIOLIB_DEBUG_PRINTLN("Found existing session; restoring...");
      return(this->restore());

    } else {
      // the credentials are still the same, so restore only DevNonce and JoinNonce
      this->devNonce  = mod->hal->getPersistentParameter<uint16_t>(RADIOLIB_EEPROM_LORAWAN_DEV_NONCE_ID);
      this->joinNonce = mod->hal->getPersistentParameter<uint32_t>(RADIOLIB_EEPROM_LORAWAN_JOIN_NONCE_ID);
    }
  }
  
  // if forced by user, keys are new or changed mode, wipe the previous session
  if(force || !validCheckSum || !validMode) {
    #if RADIOLIB_DEBUG
      RADIOLIB_DEBUG_PRINTLN("Didn't restore session (checksum: %d, mode: %d)", validCheckSum, validMode);
      RADIOLIB_DEBUG_PRINTLN("First 16 bytes of NVM:");
      uint8_t nvmBuff[16];
      mod->hal->readPersistentStorage(mod->hal->getPersistentAddr(0), nvmBuff, 16);
      RADIOLIB_DEBUG_HEXDUMP(nvmBuff, 16);
      RADIOLIB_DEBUG_PRINTLN("Wiping EEPROM and starting a clean session");
    #endif

    this->wipe();
  }
#else
  (void)force;
#endif

  int16_t state = RADIOLIB_ERR_NONE;

  // setup join-request uplink/downlink frequencies and datarates
  if(this->band->bandType == RADIOLIB_LORAWAN_BAND_DYNAMIC) {
    state = this->setupChannelsDyn(true);
  } else {
    state = this->setupChannelsFix(this->subBand);
  }
  RADIOLIB_ASSERT(state);

  // setup all MAC properties to default values  
  this->beginCommon(joinDr);

  // set the physical layer configuration
  state = this->setPhyProperties();
  RADIOLIB_ASSERT(state);

  // select a random pair of Tx/Rx channels
  state = this->selectChannels();
  RADIOLIB_ASSERT(state);

  // configure for uplink with default configuration
  state = this->configureChannel(RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK);
  RADIOLIB_ASSERT(state);

  // increment devNonce as we are sending another join-request
  this->devNonce += 1;

  // build the join-request message
  uint8_t joinRequestMsg[RADIOLIB_LORAWAN_JOIN_REQUEST_LEN];
  
  // set the packet fields
  joinRequestMsg[0] = RADIOLIB_LORAWAN_MHDR_MTYPE_JOIN_REQUEST | RADIOLIB_LORAWAN_MHDR_MAJOR_R1;
  LoRaWANNode::hton<uint64_t>(&joinRequestMsg[RADIOLIB_LORAWAN_JOIN_REQUEST_JOIN_EUI_POS], joinEUI);
  LoRaWANNode::hton<uint64_t>(&joinRequestMsg[RADIOLIB_LORAWAN_JOIN_REQUEST_DEV_EUI_POS], devEUI);
  LoRaWANNode::hton<uint16_t>(&joinRequestMsg[RADIOLIB_LORAWAN_JOIN_REQUEST_DEV_NONCE_POS], this->devNonce);

  // add the authentication code
  uint32_t mic = this->generateMIC(joinRequestMsg, RADIOLIB_LORAWAN_JOIN_REQUEST_LEN - sizeof(uint32_t), nwkKey);
  LoRaWANNode::hton<uint32_t>(&joinRequestMsg[RADIOLIB_LORAWAN_JOIN_REQUEST_LEN - sizeof(uint32_t)], mic);

  // send it
  state = this->phyLayer->transmit(joinRequestMsg, RADIOLIB_LORAWAN_JOIN_REQUEST_LEN);
  this->rxDelayStart = mod->hal->millis();
  RADIOLIB_DEBUG_PRINTLN("Join-request sent <-- Rx Delay start");
  RADIOLIB_ASSERT(state);

  // configure Rx delay for join-accept message - these are re-configured once a valid join-request is received
  this->rxDelays[0] = RADIOLIB_LORAWAN_JOIN_ACCEPT_DELAY_1_MS;
  this->rxDelays[1] = RADIOLIB_LORAWAN_JOIN_ACCEPT_DELAY_2_MS;

  // handle Rx1 and Rx2 windows - returns RADIOLIB_ERR_NONE if a downlink is received
  state = downlinkCommon();
  RADIOLIB_ASSERT(state);

  // build the buffer for the reply data
  uint8_t joinAcceptMsgEnc[RADIOLIB_LORAWAN_JOIN_ACCEPT_MAX_LEN];

  // check received length
  size_t lenRx = this->phyLayer->getPacketLength(true);
  if((lenRx != RADIOLIB_LORAWAN_JOIN_ACCEPT_MAX_LEN) && (lenRx != RADIOLIB_LORAWAN_JOIN_ACCEPT_MAX_LEN - RADIOLIB_LORAWAN_JOIN_ACCEPT_CFLIST_LEN)) {
    RADIOLIB_DEBUG_PRINTLN("joinAccept reply length mismatch, expected %luB got %luB", RADIOLIB_LORAWAN_JOIN_ACCEPT_MAX_LEN, lenRx);
    return(RADIOLIB_ERR_DOWNLINK_MALFORMED);
  }

  // read the packet
  state = this->phyLayer->readData(joinAcceptMsgEnc, lenRx);
  // downlink frames are sent without CRC, which will raise error on SX127x
  // we can ignore that error
  if(state != RADIOLIB_ERR_LORA_HEADER_DAMAGED) {
    RADIOLIB_ASSERT(state);
  }

  // check reply message type
  if((joinAcceptMsgEnc[0] & RADIOLIB_LORAWAN_MHDR_MTYPE_MASK) != RADIOLIB_LORAWAN_MHDR_MTYPE_JOIN_ACCEPT) {
    RADIOLIB_DEBUG_PRINTLN("joinAccept reply message type invalid, expected 0x%02x got 0x%02x", RADIOLIB_LORAWAN_MHDR_MTYPE_JOIN_ACCEPT, joinAcceptMsgEnc[0]);
    return(RADIOLIB_ERR_DOWNLINK_MALFORMED);
  }

  // decrypt the join accept message
  // this is done by encrypting again in ECB mode
  // the first byte is the MAC header which is not encrypted
  uint8_t joinAcceptMsg[RADIOLIB_LORAWAN_JOIN_ACCEPT_MAX_LEN];
  joinAcceptMsg[0] = joinAcceptMsgEnc[0];
  RadioLibAES128Instance.init(nwkKey);
  RadioLibAES128Instance.encryptECB(&joinAcceptMsgEnc[1], RADIOLIB_LORAWAN_JOIN_ACCEPT_MAX_LEN - 1, &joinAcceptMsg[1]);
  
  RADIOLIB_DEBUG_PRINTLN("joinAcceptMsg:");
  RADIOLIB_DEBUG_HEXDUMP(joinAcceptMsg, lenRx);

  // get current JoinNonce from downlink and previous JoinNonce from persistent storage
  uint32_t joinNonceNew = LoRaWANNode::ntoh<uint32_t>(&joinAcceptMsg[RADIOLIB_LORAWAN_JOIN_ACCEPT_JOIN_NONCE_POS], 3);

  RADIOLIB_DEBUG_PRINTLN("JoinNoncePrev: %d, JoinNonce: %d", this->joinNonce, joinNonceNew);
    // JoinNonce received must be greater than the last JoinNonce heard, else error
  if((this->joinNonce > 0) && (joinNonceNew <= this->joinNonce)) {
    return(RADIOLIB_ERR_JOIN_NONCE_INVALID);
  }
  this->joinNonce = joinNonceNew;

  this->homeNetId = LoRaWANNode::ntoh<uint32_t>(&joinAcceptMsg[RADIOLIB_LORAWAN_JOIN_ACCEPT_HOME_NET_ID_POS], 3);
  this->devAddr = LoRaWANNode::ntoh<uint32_t>(&joinAcceptMsg[RADIOLIB_LORAWAN_JOIN_ACCEPT_DEV_ADDR_POS]);

  // check LoRaWAN revision (the MIC verification depends on this)
  uint8_t dlSettings = joinAcceptMsg[RADIOLIB_LORAWAN_JOIN_ACCEPT_DL_SETTINGS_POS];
  this->rev = (dlSettings & RADIOLIB_LORAWAN_JOIN_ACCEPT_R_1_1) >> 7;
  RADIOLIB_DEBUG_PRINTLN("LoRaWAN revision: 1.%d", this->rev);

  // verify MIC
  if(this->rev == 1) {
    // 1.1 version, first we need to derive the join accept integrity key
    uint8_t keyDerivationBuff[RADIOLIB_AES128_BLOCK_SIZE] = { 0 };
    keyDerivationBuff[0] = RADIOLIB_LORAWAN_JOIN_ACCEPT_JS_INT_KEY;
    LoRaWANNode::hton<uint64_t>(&keyDerivationBuff[1], devEUI);
    RadioLibAES128Instance.init(nwkKey);
    RadioLibAES128Instance.encryptECB(keyDerivationBuff, RADIOLIB_AES128_BLOCK_SIZE, this->jSIntKey);

    // prepare the buffer for MIC calculation
    uint8_t micBuff[3*RADIOLIB_AES128_BLOCK_SIZE] = { 0 };
    micBuff[0] = RADIOLIB_LORAWAN_JOIN_REQUEST_TYPE;
    LoRaWANNode::hton<uint64_t>(&micBuff[1], joinEUI);
    LoRaWANNode::hton<uint16_t>(&micBuff[9], this->devNonce);
    memcpy(&micBuff[11], joinAcceptMsg, lenRx);
    
    if(!verifyMIC(micBuff, lenRx + 11, this->jSIntKey)) {
      return(RADIOLIB_ERR_CRC_MISMATCH);
    }
  
  } else {
    // 1.0 version
    if(!verifyMIC(joinAcceptMsg, lenRx, nwkKey)) {
      return(RADIOLIB_ERR_CRC_MISMATCH);
    }

  }

  LoRaWANMacCommand_t cmd = {
    .cid = RADIOLIB_LORAWAN_MAC_RX_PARAM_SETUP,
    .payload = { 0 }, 
    .len = MacTable[RADIOLIB_LORAWAN_MAC_RX_PARAM_SETUP].lenDn,
    .repeat = 0,
  };
  cmd.payload[0] = dlSettings & 0x7F;
  uint32_t rx2Freq = uint32_t(this->rx2.freq * 10000);      // default Rx2 frequency
  LoRaWANNode::hton<uint32_t>(&cmd.payload[1], rx2Freq, 3);
  (void)execMacCommand(&cmd);

  cmd.cid = RADIOLIB_LORAWAN_MAC_RX_TIMING_SETUP;
  cmd.len = MacTable[RADIOLIB_LORAWAN_MAC_RX_TIMING_SETUP].lenDn;
  cmd.payload[0] = joinAcceptMsg[RADIOLIB_LORAWAN_JOIN_ACCEPT_RX_DELAY_POS];
  (void)execMacCommand(&cmd);
  
  // in case of dynamic band, setup the default channels first
  if(this->band->bandType == RADIOLIB_LORAWAN_BAND_DYNAMIC) {
    this->setupChannelsDyn(false);
  }
  // process CFlist if present
  if(lenRx == RADIOLIB_LORAWAN_JOIN_ACCEPT_MAX_LEN) {
    uint8_t cfList[RADIOLIB_LORAWAN_JOIN_ACCEPT_CFLIST_LEN] = { 0 };
    memcpy(&cfList[0], &joinAcceptMsg[RADIOLIB_LORAWAN_JOIN_ACCEPT_CFLIST_POS], RADIOLIB_LORAWAN_JOIN_ACCEPT_CFLIST_LEN);
    this->processCFList(cfList);
  } 
  // if no CFList was received, default or subband are already setup so don't need to do anything else

  // prepare buffer for key derivation
  uint8_t keyDerivationBuff[RADIOLIB_AES128_BLOCK_SIZE] = { 0 };
  LoRaWANNode::hton<uint32_t>(&keyDerivationBuff[RADIOLIB_LORAWAN_JOIN_ACCEPT_JOIN_NONCE_POS], joinNonce, 3);

  // check protocol version (1.0 vs 1.1)
  if(this->rev == 1) {
    // 1.1 version, derive the keys
    LoRaWANNode::hton<uint64_t>(&keyDerivationBuff[RADIOLIB_LORAWAN_JOIN_ACCEPT_JOIN_EUI_POS], joinEUI);
    LoRaWANNode::hton<uint16_t>(&keyDerivationBuff[RADIOLIB_LORAWAN_JOIN_ACCEPT_DEV_NONCE_POS], this->devNonce);
    keyDerivationBuff[0] = RADIOLIB_LORAWAN_JOIN_ACCEPT_APP_S_KEY;

    RadioLibAES128Instance.init(appKey);
    RadioLibAES128Instance.encryptECB(keyDerivationBuff, RADIOLIB_AES128_BLOCK_SIZE, this->appSKey);

    keyDerivationBuff[0] = RADIOLIB_LORAWAN_JOIN_ACCEPT_F_NWK_S_INT_KEY;
    RadioLibAES128Instance.init(nwkKey);
    RadioLibAES128Instance.encryptECB(keyDerivationBuff, RADIOLIB_AES128_BLOCK_SIZE, this->fNwkSIntKey);

    keyDerivationBuff[0] = RADIOLIB_LORAWAN_JOIN_ACCEPT_S_NWK_S_INT_KEY;
    RadioLibAES128Instance.init(nwkKey);
    RadioLibAES128Instance.encryptECB(keyDerivationBuff, RADIOLIB_AES128_BLOCK_SIZE, this->sNwkSIntKey);

    keyDerivationBuff[0] = RADIOLIB_LORAWAN_JOIN_ACCEPT_NWK_S_ENC_KEY;
    RadioLibAES128Instance.init(nwkKey);
    RadioLibAES128Instance.encryptECB(keyDerivationBuff, RADIOLIB_AES128_BLOCK_SIZE, this->nwkSEncKey);

    // enqueue the RekeyInd MAC command to be sent in the next uplink
    LoRaWANMacCommand_t cmd = {
      .cid = RADIOLIB_LORAWAN_MAC_REKEY,
      .payload = { this->rev },
      .len = sizeof(uint8_t),
      .repeat = 0x01 << RADIOLIB_LORAWAN_ADR_ACK_LIMIT_EXP,
    };
    state = pushMacCommand(&cmd, &this->commandsUp);
    RADIOLIB_ASSERT(state);
  
  } else {
    // 1.0 version, just derive the keys
    LoRaWANNode::hton<uint32_t>(&keyDerivationBuff[RADIOLIB_LORAWAN_JOIN_ACCEPT_HOME_NET_ID_POS], this->homeNetId, 3);
    LoRaWANNode::hton<uint16_t>(&keyDerivationBuff[RADIOLIB_LORAWAN_JOIN_ACCEPT_DEV_ADDR_POS], this->devNonce);
    keyDerivationBuff[0] = RADIOLIB_LORAWAN_JOIN_ACCEPT_APP_S_KEY;
    RadioLibAES128Instance.init(nwkKey);
    RadioLibAES128Instance.encryptECB(keyDerivationBuff, RADIOLIB_AES128_BLOCK_SIZE, this->appSKey);

    keyDerivationBuff[0] = RADIOLIB_LORAWAN_JOIN_ACCEPT_F_NWK_S_INT_KEY;
    RadioLibAES128Instance.init(nwkKey);
    RadioLibAES128Instance.encryptECB(keyDerivationBuff, RADIOLIB_AES128_BLOCK_SIZE, this->fNwkSIntKey);

    memcpy(this->sNwkSIntKey, this->fNwkSIntKey, RADIOLIB_AES128_KEY_SIZE);
    memcpy(this->nwkSEncKey, this->fNwkSIntKey, RADIOLIB_AES128_KEY_SIZE);
  
  }

  // reset all frame counters
  this->fcntUp = 0;
  this->aFcntDown = 0;
  this->nFcntDown = 0;
  this->confFcntUp = RADIOLIB_LORAWAN_FCNT_NONE;
  this->confFcntDown = RADIOLIB_LORAWAN_FCNT_NONE;
  this->adrFcnt = 0;

#if !defined(RADIOLIB_EEPROM_UNSUPPORTED)
  // save the activation keys checksum, device address & keys as well as JoinAccept values; these are only ever set when joining
  mod->hal->setPersistentParameter<uint16_t>(RADIOLIB_EEPROM_LORAWAN_CHECKSUM_ID, checkSum);
  mod->hal->setPersistentParameter<uint32_t>(RADIOLIB_EEPROM_LORAWAN_DEV_ADDR_ID, this->devAddr);
  mod->hal->writePersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_APP_S_KEY_ID), this->appSKey, RADIOLIB_AES128_BLOCK_SIZE);
  mod->hal->writePersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_FNWK_SINT_KEY_ID), this->fNwkSIntKey, RADIOLIB_AES128_BLOCK_SIZE);
  mod->hal->writePersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_SNWK_SINT_KEY_ID), this->sNwkSIntKey, RADIOLIB_AES128_BLOCK_SIZE);
  mod->hal->writePersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_NWK_SENC_KEY_ID), this->nwkSEncKey, RADIOLIB_AES128_BLOCK_SIZE);

  // save join-request parameters
  mod->hal->setPersistentParameter<uint32_t>(RADIOLIB_EEPROM_LORAWAN_HOME_NET_ID, this->homeNetId);
  mod->hal->setPersistentParameter<uint32_t>(RADIOLIB_EEPROM_LORAWAN_DEV_NONCE_ID, this->devNonce);
  mod->hal->setPersistentParameter<uint32_t>(RADIOLIB_EEPROM_LORAWAN_JOIN_NONCE_ID, this->joinNonce);

  this->saveSession();

  // everything written to NVM, write current table version to persistent storage and set mode
  mod->hal->setPersistentParameter<uint16_t>(RADIOLIB_EEPROM_TABLE_VERSION_ID, RADIOLIB_EEPROM_TABLE_VERSION);
  mod->hal->setPersistentParameter<uint16_t>(RADIOLIB_EEPROM_LORAWAN_MODE_ID, RADIOLIB_LORAWAN_MODE_OTAA);
#endif

  this->activeMode = RADIOLIB_LORAWAN_MODE_OTAA;

  return(RADIOLIB_ERR_NONE);
}

int16_t LoRaWANNode::beginABP(uint32_t addr, uint8_t* nwkSKey, uint8_t* appSKey, uint8_t* fNwkSIntKey, uint8_t* sNwkSIntKey, bool force) {
#if !defined(RADIOLIB_EEPROM_UNSUPPORTED)
  // only needed for persistent storage
  Module* mod = this->phyLayer->getMod();

  // check if we actually need to restart from a clean session
  uint16_t checkSum = 0;
  checkSum ^= LoRaWANNode::checkSum16(reinterpret_cast<uint8_t*>(&addr), 4);
  checkSum ^= LoRaWANNode::checkSum16(nwkSKey, 16);
  checkSum ^= LoRaWANNode::checkSum16(appSKey, 16);
  if(fNwkSIntKey) { checkSum ^= LoRaWANNode::checkSum16(fNwkSIntKey, 16); }
  if(sNwkSIntKey) { checkSum ^= LoRaWANNode::checkSum16(sNwkSIntKey, 16); }

  bool validCheckSum = mod->hal->getPersistentParameter<uint16_t>(RADIOLIB_EEPROM_LORAWAN_CHECKSUM_ID) == checkSum;
  bool validMode = mod->hal->getPersistentParameter<uint16_t>(RADIOLIB_EEPROM_LORAWAN_MODE_ID) == RADIOLIB_LORAWAN_MODE_ABP;

  if(!force && validCheckSum && validMode) {
    // the device has joined already, we can just pull the data from persistent storage
    RADIOLIB_DEBUG_PRINTLN("Found existing session; restoring...");
    
    return(this->restore());
  } else {
    #if RADIOLIB_DEBUG
      RADIOLIB_DEBUG_PRINTLN("Didn't restore session (checksum: %d, mode: %d)", validCheckSum, validMode);
      RADIOLIB_DEBUG_PRINTLN("First 16 bytes of NVM:");
      uint8_t nvmBuff[16];
      mod->hal->readPersistentStorage(mod->hal->getPersistentAddr(0), nvmBuff, 16);
      RADIOLIB_DEBUG_HEXDUMP(nvmBuff, 16);
      RADIOLIB_DEBUG_PRINTLN("Wiping EEPROM and starting a clean session");
    #endif

    this->wipe();
  }
#else
  (void)force;
#endif

  this->devAddr = addr;
  memcpy(this->appSKey, appSKey, RADIOLIB_AES128_KEY_SIZE);
  memcpy(this->nwkSEncKey, nwkSKey, RADIOLIB_AES128_KEY_SIZE);
  if(fNwkSIntKey) {
    this->rev = 1;
    memcpy(this->fNwkSIntKey, fNwkSIntKey, RADIOLIB_AES128_KEY_SIZE);
  } else {
    memcpy(this->fNwkSIntKey, nwkSKey, RADIOLIB_AES128_KEY_SIZE);
  }
  if(sNwkSIntKey) {
    memcpy(this->sNwkSIntKey, sNwkSIntKey, RADIOLIB_AES128_KEY_SIZE);
  }

  int16_t state = RADIOLIB_ERR_NONE;

  // setup the uplink/downlink channels and initial datarate
  if(this->band->bandType == RADIOLIB_LORAWAN_BAND_DYNAMIC) {
    this->setupChannelsDyn();
  } else {
    this->setupChannelsFix(this->subBand);
  }

  // setup all MAC properties to default values
  this->beginCommon();

  // set the physical layer configuration
  state = this->setPhyProperties();
  RADIOLIB_ASSERT(state);

#if !defined(RADIOLIB_EEPROM_UNSUPPORTED)
  // save the activation keys checksum, device address & keys
  mod->hal->setPersistentParameter<uint16_t>(RADIOLIB_EEPROM_LORAWAN_CHECKSUM_ID, checkSum);
  mod->hal->setPersistentParameter<uint32_t>(RADIOLIB_EEPROM_LORAWAN_DEV_ADDR_ID, this->devAddr);
  mod->hal->writePersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_APP_S_KEY_ID), this->appSKey, RADIOLIB_AES128_BLOCK_SIZE);
  mod->hal->writePersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_FNWK_SINT_KEY_ID), this->fNwkSIntKey, RADIOLIB_AES128_BLOCK_SIZE);
  mod->hal->writePersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_SNWK_SINT_KEY_ID), this->sNwkSIntKey, RADIOLIB_AES128_BLOCK_SIZE);
  mod->hal->writePersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_NWK_SENC_KEY_ID), this->nwkSEncKey, RADIOLIB_AES128_BLOCK_SIZE);

  // save all new frame counters
  this->saveSession();

  // everything written to NVM, write current table version to persistent storage and set mode
  mod->hal->setPersistentParameter<uint16_t>(RADIOLIB_EEPROM_TABLE_VERSION_ID, RADIOLIB_EEPROM_TABLE_VERSION);
  mod->hal->setPersistentParameter<uint16_t>(RADIOLIB_EEPROM_LORAWAN_MODE_ID, RADIOLIB_LORAWAN_MODE_ABP);
#endif

  this->activeMode = RADIOLIB_LORAWAN_MODE_ABP;

  return(RADIOLIB_ERR_NONE);
}

bool LoRaWANNode::isJoined() {
  return(this->activeMode != RADIOLIB_LORAWAN_MODE_NONE);
}

#if !defined(RADIOLIB_EEPROM_UNSUPPORTED)
int16_t LoRaWANNode::saveSession() {
  Module* mod = this->phyLayer->getMod();

  if(mod->hal->getPersistentParameter<uint8_t>(RADIOLIB_EEPROM_LORAWAN_VERSION_ID) != this->rev)
     mod->hal->setPersistentParameter<uint8_t>(RADIOLIB_EEPROM_LORAWAN_VERSION_ID, this->rev);

  // store all frame counters
  if(mod->hal->getPersistentParameter<uint32_t>(RADIOLIB_EEPROM_LORAWAN_A_FCNT_DOWN_ID) != this->aFcntDown)
     mod->hal->setPersistentParameter<uint32_t>(RADIOLIB_EEPROM_LORAWAN_A_FCNT_DOWN_ID, this->aFcntDown);
  
  if(mod->hal->getPersistentParameter<uint32_t>(RADIOLIB_EEPROM_LORAWAN_N_FCNT_DOWN_ID) != this->nFcntDown)
     mod->hal->setPersistentParameter<uint32_t>(RADIOLIB_EEPROM_LORAWAN_N_FCNT_DOWN_ID, this->nFcntDown);
  
  if(mod->hal->getPersistentParameter<uint32_t>(RADIOLIB_EEPROM_LORAWAN_CONF_FCNT_UP_ID) != this->confFcntUp)
     mod->hal->setPersistentParameter<uint32_t>(RADIOLIB_EEPROM_LORAWAN_CONF_FCNT_UP_ID, this->confFcntUp);

  if(mod->hal->getPersistentParameter<uint32_t>(RADIOLIB_EEPROM_LORAWAN_CONF_FCNT_DOWN_ID) != this->confFcntDown)
     mod->hal->setPersistentParameter<uint32_t>(RADIOLIB_EEPROM_LORAWAN_CONF_FCNT_DOWN_ID, this->confFcntDown);
  
  if(mod->hal->getPersistentParameter<uint32_t>(RADIOLIB_EEPROM_LORAWAN_ADR_FCNT_ID) != this->adrFcnt)
     mod->hal->setPersistentParameter<uint32_t>(RADIOLIB_EEPROM_LORAWAN_ADR_FCNT_ID, this->adrFcnt);
  
  // fcntUp is saved using highly efficient wear-leveling as this is by far going to be written most often
  this->saveFcntUp();

  // if there is, or was, any MAC command in the queue, overwrite with the current MAC queue
  uint8_t queueBuff[sizeof(LoRaWANMacCommandQueue_t)] = { 0 };
  mod->hal->readPersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_MAC_QUEUE_UL_ID), queueBuff, sizeof(LoRaWANMacCommandQueue_t));
  LoRaWANMacCommandQueue_t cmdTemp;
  memcpy(&cmdTemp, queueBuff, sizeof(LoRaWANMacCommandQueue_t));
  if(this->commandsUp.numCommands > 0 || cmdTemp.numCommands > 0) {
    memcpy(queueBuff, &this->commandsUp, sizeof(LoRaWANMacCommandQueue_t));
    mod->hal->writePersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_MAC_QUEUE_UL_ID), queueBuff, sizeof(LoRaWANMacCommandQueue_t));
  }

  return(RADIOLIB_ERR_NONE);
}

int16_t LoRaWANNode::saveFcntUp() {
  Module* mod = this->phyLayer->getMod();

  uint8_t fcntBuffStart = mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_FCNT_UP_ID);
  uint8_t fcntBuffEnd = mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_FCNT_UP_ID + 1);
  uint8_t buffSize = fcntBuffEnd - fcntBuffStart;
  #if RADIOLIB_STATIC_ONLY
  uint8_t fcntBuff[RADIOLIB_STATIC_ARRAY_SIZE];
  #else
  uint8_t* fcntBuff = new uint8_t[buffSize];
  #endif
  mod->hal->readPersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_FCNT_UP_ID), fcntBuff, buffSize);

  // we discard the first two bits - your flash will likely be far dead by the time you reach 2^30 uplinks
  // the first two bytes of the remaining 30 bytes are stored straight into storage without additional wear leveling
  // because they hardly ever change
  uint8_t bits_30_22 = (uint8_t)(this->fcntUp >> 22);
  if(fcntBuff[0] != bits_30_22)
    mod->hal->setPersistentParameter<uint8_t>(RADIOLIB_EEPROM_LORAWAN_FCNT_UP_ID, bits_30_22, 0);
  uint8_t bits_22_14 = (uint8_t)(this->fcntUp >> 14);
  if(fcntBuff[1] != bits_22_14)
    mod->hal->setPersistentParameter<uint8_t>(RADIOLIB_EEPROM_LORAWAN_FCNT_UP_ID, bits_22_14, 1);

  // the next 7 bits are stored into one of few indices
  // this index is indicated by the first byte that has its state (most significant bit) different from its predecessor
  // if all have an equal state, restart from the beginning
  // always flip the state bit of the byte that we write to, to indicate that this is the most recently written byte
  uint8_t idx = 2;
  uint8_t state = fcntBuff[idx] >> 7;
  for(; idx < 5; idx++) {
    if(fcntBuff[idx] >> 7 != state) {
      break;
    }
  }
  // check if the last written byte is equal to current, only rewrite if different
  uint8_t bits_14_7 = (this->fcntUp >> 7) & 0x7F;
  if((fcntBuff[idx - 1] & 0x7F) != bits_14_7) {
    // find next index to write
    idx = idx < 5 ? idx : 2;

    // flip the first bit of this byte to indicate that we just wrote here
    bits_14_7 |= (~(fcntBuff[idx] >> 7)) << 7;
    mod->hal->setPersistentParameter<uint8_t>(RADIOLIB_EEPROM_LORAWAN_FCNT_UP_ID, bits_14_7, idx);
  }

  // equally, the last 7 bits are stored into one of many indices
  // this index is indicated by the first byte that has its state (most significant bit) different from its predecessor
  // if all have an equal state, restart from the beginning
  // always flip the state bit of the byte that we write to, to indicate that this is the most recently written byte
  idx = 5;
  state = fcntBuff[idx] >> 7;
  for(; idx < buffSize; idx++) {
    if(fcntBuff[idx] >> 7 != state) {
      break;
    }
  }
  idx = idx < buffSize ? idx : 5;
  uint8_t bits_7_0 = (this->fcntUp >> 0) & 0x7F;

  // flip the first bit of this byte to indicate that we just wrote here
  bits_7_0 |= (~(fcntBuff[idx] >> 7)) << 7;
  mod->hal->setPersistentParameter<uint8_t>(RADIOLIB_EEPROM_LORAWAN_FCNT_UP_ID, bits_7_0, idx);

  return(RADIOLIB_ERR_NONE);
}
#endif  // RADIOLIB_EEPROM_UNSUPPORTED

#if defined(RADIOLIB_BUILD_ARDUINO)
int16_t LoRaWANNode::uplink(String& str, uint8_t port, bool isConfirmed, LoRaWANEvent_t* event) {
  return(this->uplink(str.c_str(), port, isConfirmed, event));
}
#endif

int16_t LoRaWANNode::uplink(const char* str, uint8_t port, bool isConfirmed, LoRaWANEvent_t* event) {
  return(this->uplink((uint8_t*)str, strlen(str), port, isConfirmed, event));
}

int16_t LoRaWANNode::uplink(uint8_t* data, size_t len, uint8_t port, bool isConfirmed, LoRaWANEvent_t* event) {
  Module* mod = this->phyLayer->getMod();
  
  // check if the Rx windows were closed after sending the previous uplink
  // this FORCES a user to call downlink() after an uplink()
  if(this->rxDelayEnd < this->rxDelayStart) {
    // not enough time elapsed since the last uplink, we may still be in an Rx window
    return(RADIOLIB_ERR_UPLINK_UNAVAILABLE);
  }

  // if adhering to dutyCycle and the time since last uplink + interval has not elapsed, return an error
  if(this->dutyCycleEnabled && this->rxDelayStart + dutyCycleInterval(this->dutyCycle, this->lastToA) > mod->hal->millis()) {
    return(RADIOLIB_ERR_UPLINK_UNAVAILABLE);
  }

  // check destination port
  if(port > 0xDF) {
    return(RADIOLIB_ERR_INVALID_PORT);
  }
  // port 0 is only allowed for MAC-only payloads
  if(port == RADIOLIB_LORAWAN_FPORT_MAC_COMMAND) {
    if (!this->isMACPayload) {
      return(RADIOLIB_ERR_INVALID_PORT);
    }
    // if this is MAC only payload, continue and reset for next uplink
    this->isMACPayload = false;
  }

  int16_t state = RADIOLIB_ERR_NONE;

  // check if there are some MAC commands to piggyback (only when piggybacking onto a application-frame)
  uint8_t foptsLen = 0;
  size_t foptsBufSize = 0;
  if(this->commandsUp.numCommands > 0 && port != RADIOLIB_LORAWAN_FPORT_MAC_COMMAND) {
    // there are, assume the maximum possible FOpts len for buffer allocation
    foptsLen = this->commandsUp.len;
    foptsBufSize = 15;
  }

  // check maximum payload len as defined in phy
  if(len > this->band->payloadLenMax[this->dataRates[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK]]) {
    return(RADIOLIB_ERR_PACKET_TOO_LONG);
    // if testing with TS008 specification verification protocol, don't throw error but clip the message
    // len = this->band->payloadLenMax[this->dataRates[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK]];
  }

  // increase frame counter by one
  this->fcntUp += 1;

  bool adrAckReq = false;
  if(this->adrEnabled) {
    // check if we need to do ADR stuff
    uint32_t adrLimit = 0x01 << this->adrLimitExp;
    uint32_t adrDelay = 0x01 << this->adrDelayExp;
    if((this->fcntUp - this->adrFcnt) >= adrLimit) {
      adrAckReq = true;
    }
    // if we hit the Limit + Delay, try one of three, in order: 
    // set TxPower to max, set DR to min, enable all default channels
    if ((this->fcntUp - this->adrFcnt) == (adrLimit + adrDelay)) {
      uint8_t adrStage = 1;
      while(adrStage != 0) {
        switch(adrStage) {
          case(1): {
              // if the TxPower field has some offset, remove it and switch to maximum power
              if(this->txPowerCur > 0) {
                // set the maximum power supported by both the module and the band
                state = this->setTxPower(this->txPowerMax, true);
                if(state == RADIOLIB_ERR_NONE) {
                  this->txPowerCur = 0;
                  adrStage = 0;                         // successfully did some ADR stuff
                }
              }
              if(adrStage == 1) {                       // if nothing succeeded, proceed to stage 2
                adrStage = 2;
              }
            }
            break;
          case(2): {
              // try to decrease the datarate
              if(this->dataRates[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK] > 0) {
                if(this->setDatarate(this->dataRates[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK] - 1, true) == RADIOLIB_ERR_NONE) {
                  adrStage = 0;                         // successfully did some ADR stuff
                }
              }
              if(adrStage == 2) {                       // if nothing succeeded, proceed to stage 3
                adrStage = 3;
              }
            }
            break;
          case(3): {
              if(this->band->bandType == RADIOLIB_LORAWAN_BAND_DYNAMIC) {
                this->setupChannelsDyn(false);          // revert to default frequencies
              } else {
                // go back to default selected subband
                // hopefully it'll help something, but probably not; at least we tried..
                this->setupChannelsFix(this->subBand);
              }
              adrStage = 0;                             // nothing else to do, so end the cycle
            }
            break;
        }
      }

      // we tried something to improve the range, so increase the ADR frame counter by 'ADR delay'
      this->adrFcnt += adrDelay;
    }
  }

  // configure for uplink
  this->selectChannels();
  state = this->configureChannel(RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK);
  RADIOLIB_ASSERT(state);

  // if dwell time is imposed, calculated expected time on air and cancel if exceeds
  if(this->dwellTimeEnabledUp && this->phyLayer->getTimeOnAir(RADIOLIB_LORAWAN_FRAME_LEN(len, foptsLen) - 16)/1000 > this->dwellTimeUp) {
    return(RADIOLIB_ERR_PACKET_TOO_LONG);
  }

  // build the uplink message
  // the first 16 bytes are reserved for MIC calculation blocks
  size_t uplinkMsgLen = RADIOLIB_LORAWAN_FRAME_LEN(len, foptsBufSize);
  #if RADIOLIB_STATIC_ONLY
  uint8_t uplinkMsg[RADIOLIB_STATIC_ARRAY_SIZE];
  #else
  uint8_t* uplinkMsg = new uint8_t[uplinkMsgLen];
  #endif
  
  // set the packet fields
  if(isConfirmed) {
    uplinkMsg[RADIOLIB_LORAWAN_FHDR_LEN_START_OFFS] = RADIOLIB_LORAWAN_MHDR_MTYPE_CONF_DATA_UP;
    this->confFcntUp = this->fcntUp;
  } else {
    uplinkMsg[RADIOLIB_LORAWAN_FHDR_LEN_START_OFFS] = RADIOLIB_LORAWAN_MHDR_MTYPE_UNCONF_DATA_UP;
  }
  uplinkMsg[RADIOLIB_LORAWAN_FHDR_LEN_START_OFFS] |= RADIOLIB_LORAWAN_MHDR_MAJOR_R1;
  LoRaWANNode::hton<uint32_t>(&uplinkMsg[RADIOLIB_LORAWAN_FHDR_DEV_ADDR_POS], this->devAddr);

  // length of fopts will be added later
  uplinkMsg[RADIOLIB_LORAWAN_FHDR_FCTRL_POS] = 0x00;
  if(this->adrEnabled) {
    uplinkMsg[RADIOLIB_LORAWAN_FHDR_FCTRL_POS] |= RADIOLIB_LORAWAN_FCTRL_ADR_ENABLED;
    if(adrAckReq) {
      uplinkMsg[RADIOLIB_LORAWAN_FHDR_FCTRL_POS] |= RADIOLIB_LORAWAN_FCTRL_ADR_ACK_REQ;
    }
  }

  // if the saved confirm-fcnt is set, set the ACK bit
  bool isConfirmingDown = false;
  if(this->confFcntDown != RADIOLIB_LORAWAN_FCNT_NONE) {
    isConfirmingDown = true;
    uplinkMsg[RADIOLIB_LORAWAN_FHDR_FCTRL_POS] |= RADIOLIB_LORAWAN_FCTRL_ACK;
  }

  LoRaWANNode::hton<uint16_t>(&uplinkMsg[RADIOLIB_LORAWAN_FHDR_FCNT_POS], (uint16_t)this->fcntUp);

  // check if we have some MAC commands to append
  if(foptsLen > 0) {
    // assume maximum possible buffer size
    uint8_t foptsBuff[15];
    uint8_t* foptsPtr = foptsBuff;

    // append all MAC replies into fopts buffer
    int16_t i = 0;
    for (; i < this->commandsUp.numCommands; i++) {
      LoRaWANMacCommand_t cmd = this->commandsUp.commands[i];
      memcpy(foptsPtr, &cmd, 1 + cmd.len);
      foptsPtr += cmd.len + 1;
    }
    RADIOLIB_DEBUG_PRINTLN("Uplink MAC payload (%d commands):", this->commandsUp.numCommands);
    RADIOLIB_DEBUG_HEXDUMP(foptsBuff, foptsLen);

    // pop the commands from back to front
    for (; i >= 0; i--) {
      if(this->commandsUp.commands[i].repeat > 0) {
        this->commandsUp.commands[i].repeat--;
      } else {
        deleteMacCommand(this->commandsUp.commands[i].cid, &this->commandsUp);
      }
    }

    uplinkMsgLen = RADIOLIB_LORAWAN_FRAME_LEN(len, foptsLen);
    uplinkMsg[RADIOLIB_LORAWAN_FHDR_FCTRL_POS] |= foptsLen;

    // encrypt it
    processAES(foptsBuff, foptsLen, this->nwkSEncKey, &uplinkMsg[RADIOLIB_LORAWAN_FHDR_FOPTS_POS], this->fcntUp, RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK, 0x01, true);
    
  }

  // set the port
  uplinkMsg[RADIOLIB_LORAWAN_FHDR_FPORT_POS(foptsLen)] = port;

  // select encryption key based on the target port
  uint8_t* encKey = this->appSKey;
  if(port == RADIOLIB_LORAWAN_FPORT_MAC_COMMAND) {
    encKey = this->nwkSEncKey;
  }

  // encrypt the frame payload
  processAES(data, len, encKey, &uplinkMsg[RADIOLIB_LORAWAN_FRAME_PAYLOAD_POS(foptsLen)], this->fcntUp, RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK, 0x00, true);

  // create blocks for MIC calculation
  uint8_t block0[RADIOLIB_AES128_BLOCK_SIZE] = { 0 };
  block0[RADIOLIB_LORAWAN_BLOCK_MAGIC_POS] = RADIOLIB_LORAWAN_MIC_BLOCK_MAGIC;
  block0[RADIOLIB_LORAWAN_BLOCK_DIR_POS] = RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK;
  LoRaWANNode::hton<uint32_t>(&block0[RADIOLIB_LORAWAN_BLOCK_DEV_ADDR_POS], this->devAddr);
  LoRaWANNode::hton<uint32_t>(&block0[RADIOLIB_LORAWAN_BLOCK_FCNT_POS], this->fcntUp);
  block0[RADIOLIB_LORAWAN_MIC_BLOCK_LEN_POS] = uplinkMsgLen - RADIOLIB_AES128_BLOCK_SIZE - sizeof(uint32_t);

  uint8_t block1[RADIOLIB_AES128_BLOCK_SIZE] = { 0 };
  memcpy(block1, block0, RADIOLIB_AES128_BLOCK_SIZE);
  if(this->confFcntDown != RADIOLIB_LORAWAN_FCNT_NONE) {
    LoRaWANNode::hton<uint16_t>(&block1[RADIOLIB_LORAWAN_BLOCK_CONF_FCNT_POS], (uint16_t)this->confFcntDown);
  }
  block1[RADIOLIB_LORAWAN_MIC_DATA_RATE_POS] = this->dataRates[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK];
  block1[RADIOLIB_LORAWAN_MIC_CH_INDEX_POS] = this->currentChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK].idx;
  
  RADIOLIB_DEBUG_PRINTLN("uplinkMsg pre-MIC:");
  RADIOLIB_DEBUG_HEXDUMP(uplinkMsg, uplinkMsgLen);

  // calculate authentication codes
  memcpy(uplinkMsg, block1, RADIOLIB_AES128_BLOCK_SIZE);
  uint32_t micS = this->generateMIC(uplinkMsg, uplinkMsgLen - sizeof(uint32_t), this->sNwkSIntKey);
  memcpy(uplinkMsg, block0, RADIOLIB_AES128_BLOCK_SIZE);
  uint32_t micF = this->generateMIC(uplinkMsg, uplinkMsgLen - sizeof(uint32_t), this->fNwkSIntKey);

  // check LoRaWAN revision
  if(this->rev == 1) {
    uint32_t mic = ((uint32_t)(micF & 0x0000FF00) << 16) | ((uint32_t)(micF & 0x0000000FF) << 16) | ((uint32_t)(micS & 0x0000FF00) >> 0) | ((uint32_t)(micS & 0x0000000FF) >> 0);
    LoRaWANNode::hton<uint32_t>(&uplinkMsg[uplinkMsgLen - sizeof(uint32_t)], mic);
  } else {
    LoRaWANNode::hton<uint32_t>(&uplinkMsg[uplinkMsgLen - sizeof(uint32_t)], micF);
  }

  RADIOLIB_DEBUG_PRINTLN("uplinkMsg:");
  RADIOLIB_DEBUG_HEXDUMP(uplinkMsg, uplinkMsgLen);

  // perform CSMA if enabled.
  if (enableCSMA) {
    performCSMA();
  }

  // send it (without the MIC calculation blocks)
  state = this->phyLayer->transmit(&uplinkMsg[RADIOLIB_LORAWAN_FHDR_LEN_START_OFFS], uplinkMsgLen - RADIOLIB_LORAWAN_FHDR_LEN_START_OFFS);

  // set the timestamp so that we can measure when to start receiving
  this->rxDelayStart = mod->hal->millis();
  RADIOLIB_DEBUG_PRINTLN("Uplink sent <-- Rx Delay start");

  // calculate Time on Air of this uplink in milliseconds
  this->lastToA = this->phyLayer->getTimeOnAir(uplinkMsgLen - RADIOLIB_LORAWAN_FHDR_LEN_START_OFFS) / 1000;

  #if !RADIOLIB_STATIC_ONLY
  delete[] uplinkMsg;
  #endif
  RADIOLIB_ASSERT(state);
  
  // the downlink confirmation was acknowledged, so clear the counter value
  this->confFcntDown = RADIOLIB_LORAWAN_FCNT_NONE;

  // pass the extra info if requested
  if(event) {
    event->dir = RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK;
    event->confirmed = isConfirmed;
    event->confirming = isConfirmingDown;
    event->datarate = this->dataRates[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK];
    event->freq = currentChannels[event->dir].freq;
    event->power = this->txPowerMax - this->txPowerCur * 2;
    event->fcnt = this->fcntUp;
    event->port = port;
  }

  return(RADIOLIB_ERR_NONE);
}

int16_t LoRaWANNode::downlinkCommon() {
  Module* mod = this->phyLayer->getMod();
  const uint32_t scanGuard = 10;

  // check if there are any upcoming Rx windows
  // if the Rx1 window has already started, you're too late, because most downlinks happen in Rx1
  if(mod->hal->millis() - this->rxDelayStart > (this->rxDelays[0] - scanGuard)) {
    // if between start of Rx1 and end of Rx2, wait until Rx2 closes
    if(mod->hal->millis() - this->rxDelayStart < this->rxDelays[1]) {
      mod->hal->delay(this->rxDelays[1] + this->rxDelayStart - mod->hal->millis());
    }
    // update the end timestamp in case user got stuck between uplink and downlink
    this->rxDelayEnd = mod->hal->millis();
    return(RADIOLIB_ERR_NO_RX_WINDOW);
  }

  // configure for downlink
  int16_t state = this->configureChannel(RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK);
  RADIOLIB_ASSERT(state);

  // downlink messages are sent with inverted IQ
  if(!this->FSK) {
    state = this->phyLayer->invertIQ(true);
    RADIOLIB_ASSERT(state);
  }

  // create the masks that are required for receiving downlinks
  uint16_t irqFlags = 0x0000;
  uint16_t irqMask = 0x0000;
  this->phyLayer->irqRxDoneRxTimeout(irqFlags, irqMask);

  this->phyLayer->setPacketReceivedAction(LoRaWANNodeOnDownlinkAction);

  // perform listening in the two Rx windows
  for(uint8_t i = 0; i < 2; i++) {
    downlinkAction = false;

    // calculate the Rx timeout
    // according to the spec, this must be at least enough time to effectively detect a preamble
    // but pad it a bit on both sides (start and end) to make sure it is wide enough
    uint32_t timeoutHost = this->phyLayer->getTimeOnAir(0) + 2*scanGuard*1000;
    uint32_t timeoutMod  = this->phyLayer->calculateRxTimeout(timeoutHost);

    // wait for the start of the Rx window
    // the waiting duration is shortened a bit to cover any possible timing errors
    uint32_t waitLen = this->rxDelays[i] - (mod->hal->millis() - this->rxDelayStart);
    if(waitLen > scanGuard) {
      waitLen -= scanGuard;
    }
    mod->hal->delay(waitLen);

    // open Rx window by starting receive with specified timeout
    state = this->phyLayer->startReceive(timeoutMod, irqFlags, irqMask, 0);
    RADIOLIB_DEBUG_PRINTLN("Opening Rx%d window (%d us timeout)... <-- Rx Delay end ", i+1, timeoutHost);
    
    // wait for the timeout to complete (and a small additional delay)
    mod->hal->delay(timeoutHost / 1000 + scanGuard / 2);
    RADIOLIB_DEBUG_PRINTLN("closing");

    // check if the IRQ bit for Rx Timeout is set
    if(!this->phyLayer->isRxTimeout()) {
      break;

    } else if(i == 0) {
      // nothing in the first window, configure for the second
      this->phyLayer->standby();
      state = this->phyLayer->setFrequency(this->rx2.freq);
      RADIOLIB_ASSERT(state);

      DataRate_t dataRate;
      findDataRate(this->rx2.drMax, &dataRate);
      state = this->phyLayer->setDataRate(dataRate);
      RADIOLIB_ASSERT(state);
    }
    
  }
  // Rx windows are now closed
  this->rxDelayEnd = mod->hal->millis();

  // if we got here due to a timeout, stop ongoing activities
  if(this->phyLayer->isRxTimeout()) {
    this->phyLayer->standby();  // TODO check: this should be done automagically due to RxSingle?
    if(!this->FSK) {
      this->phyLayer->invertIQ(false);
    }

    return(RADIOLIB_ERR_RX_TIMEOUT);
  }

  // wait for the DIO to fire indicating a downlink is received
  while(!downlinkAction) {
    mod->hal->yield();
  }

  // we have a message, clear actions, go to standby and reset the IQ inversion
  this->phyLayer->standby();  // TODO check: this should be done automagically due to RxSingle?
  this->phyLayer->clearPacketReceivedAction();
  if(!this->FSK) {
    state = this->phyLayer->invertIQ(false);
    RADIOLIB_ASSERT(state);
  }

  return(RADIOLIB_ERR_NONE);
}

#if defined(RADIOLIB_BUILD_ARDUINO)
int16_t LoRaWANNode::downlink(String& str, LoRaWANEvent_t* event) {
  int16_t state = RADIOLIB_ERR_NONE;

  // build a temporary buffer
  // LoRaWAN downlinks can have 250 bytes at most with 1 extra byte for NULL
  size_t length = 0;
  uint8_t data[251];

  // wait for downlink
  state = this->downlink(data, &length, event);
  if(state == RADIOLIB_ERR_NONE) {
    // add null terminator
    data[length] = '\0';

    // initialize Arduino String class
    str = String((char*)data);
  }

  return(state);
}
#endif

int16_t LoRaWANNode::downlink(uint8_t* data, size_t* len, LoRaWANEvent_t* event) {
  // handle Rx1 and Rx2 windows - returns RADIOLIB_ERR_NONE if a downlink is received
  int16_t state = downlinkCommon();
  RADIOLIB_ASSERT(state);

  // get the packet length
  size_t downlinkMsgLen = this->phyLayer->getPacketLength();
  RADIOLIB_DEBUG_PRINTLN("Downlink message length: %d", downlinkMsgLen);

  // check the minimum required frame length
  // an extra byte is subtracted because downlink frames may not have a port
  if(downlinkMsgLen < RADIOLIB_LORAWAN_FRAME_LEN(0, 0) - 1 - RADIOLIB_AES128_BLOCK_SIZE) {
    RADIOLIB_DEBUG_PRINTLN("Downlink message too short (%lu bytes)", downlinkMsgLen);
    return(RADIOLIB_ERR_DOWNLINK_MALFORMED);
  }

  // build the buffer for the downlink message
  // the first 16 bytes are reserved for MIC calculation block
  #if !RADIOLIB_STATIC_ONLY
    uint8_t* downlinkMsg = new uint8_t[RADIOLIB_AES128_BLOCK_SIZE + downlinkMsgLen];
  #else
    uint8_t downlinkMsg[RADIOLIB_STATIC_ARRAY_SIZE];
  #endif

  // set the MIC calculation block
  memset(downlinkMsg, 0x00, RADIOLIB_AES128_BLOCK_SIZE);
  downlinkMsg[RADIOLIB_LORAWAN_BLOCK_MAGIC_POS] = RADIOLIB_LORAWAN_MIC_BLOCK_MAGIC;
  LoRaWANNode::hton<uint32_t>(&downlinkMsg[RADIOLIB_LORAWAN_BLOCK_DEV_ADDR_POS], this->devAddr);
  downlinkMsg[RADIOLIB_LORAWAN_BLOCK_DIR_POS] = RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK;
  downlinkMsg[RADIOLIB_LORAWAN_MIC_BLOCK_LEN_POS] = downlinkMsgLen - sizeof(uint32_t);

  // read the data
  state = this->phyLayer->readData(&downlinkMsg[RADIOLIB_AES128_BLOCK_SIZE], downlinkMsgLen);
  // downlink frames are sent without CRC, which will raise error on SX127x
  // we can ignore that error
  if(state == RADIOLIB_ERR_LORA_HEADER_DAMAGED) {
    state = RADIOLIB_ERR_NONE;
  }
  
  if(state != RADIOLIB_ERR_NONE) {
    #if !RADIOLIB_STATIC_ONLY
      delete[] downlinkMsg;
    #endif
    return(state);
  }

  // get the frame counter and set it to the MIC calculation block
  uint16_t fcnt16 = LoRaWANNode::ntoh<uint16_t>(&downlinkMsg[RADIOLIB_LORAWAN_FHDR_FCNT_POS]);
  LoRaWANNode::hton<uint16_t>(&downlinkMsg[RADIOLIB_LORAWAN_BLOCK_FCNT_POS], fcnt16);

  // if this downlink is confirming an uplink, its MIC was generated with the least-significant 16 bits of that fcntUp
  bool isConfirmingUp = false;
  if((downlinkMsg[RADIOLIB_LORAWAN_FHDR_FCTRL_POS] & RADIOLIB_LORAWAN_FCTRL_ACK) && (this->rev == 1)) {
    isConfirmingUp = true;
    LoRaWANNode::hton<uint16_t>(&downlinkMsg[RADIOLIB_LORAWAN_BLOCK_CONF_FCNT_POS], (uint16_t)this->confFcntUp);
  }
  
  RADIOLIB_DEBUG_PRINTLN("downlinkMsg:");
  RADIOLIB_DEBUG_HEXDUMP(downlinkMsg, RADIOLIB_AES128_BLOCK_SIZE + downlinkMsgLen);

  // calculate length of FOpts and payload
  uint8_t foptsLen = downlinkMsg[RADIOLIB_LORAWAN_FHDR_FCTRL_POS] & RADIOLIB_LORAWAN_FHDR_FOPTS_LEN_MASK;
  int payLen = downlinkMsgLen - 8 - foptsLen - sizeof(uint32_t);

  RADIOLIB_DEBUG_PRINTLN("FOpts: %02X", downlinkMsg[RADIOLIB_LORAWAN_FHDR_FCTRL_POS]);

  // in LoRaWAN v1.1, a frame can be a network frame if there is no Application payload
  // i.e., no payload at all (empty frame or FOpts only), or MAC only payload (FPort = 0)
  // TODO "NFCntDown is used for MAC communication on port 0 and when the FPort field is missing"
  // so what about empty frames for ACK? Per TS008, these should be Application downlinks
  bool isAppDownlink = true;
  if(payLen <= 0) {
    if(this->rev == 1) {
      isAppDownlink = false;
    }
  }
  else if(downlinkMsg[RADIOLIB_LORAWAN_FHDR_FPORT_POS(foptsLen)] == RADIOLIB_LORAWAN_FPORT_MAC_COMMAND) {
    foptsLen = payLen - 1;
    if(this->rev == 1) {
      isAppDownlink = false;
    }
  }
  RADIOLIB_DEBUG_PRINTLN("FOptsLen: %d", foptsLen);

  // check the FcntDown value (Network or Application)
  uint32_t fcntDownPrev = 0;
  if (isAppDownlink) {
    fcntDownPrev = this->aFcntDown;
  } else {
    fcntDownPrev = this->nFcntDown;
  }

  RADIOLIB_DEBUG_PRINTLN("fcnt: %d, fcntPrev: %d, isAppDownlink: %d", fcnt16, fcntDownPrev, (int)isAppDownlink);

  // if this is not the first downlink...
  // assume a 16-bit to 32-bit rollover if difference between counters in LSB is smaller than MAX_FCNT_GAP
  // if that isn't the case and the received fcnt is smaller or equal to the last heard fcnt, then error
  uint32_t fcnt32 = fcnt16;
  if(fcntDownPrev > 0) {
    if((fcnt16 <= fcntDownPrev) && ((0xFFFF - (uint16_t)fcntDownPrev + fcnt16) > RADIOLIB_LORAWAN_MAX_FCNT_GAP)) {
      #if !RADIOLIB_STATIC_ONLY
        delete[] downlinkMsg;
      #endif
      if (isAppDownlink) {
        return(RADIOLIB_ERR_A_FCNT_DOWN_INVALID);
      } else {
        return(RADIOLIB_ERR_N_FCNT_DOWN_INVALID);
      }
    } else if (fcnt16 <= fcntDownPrev) {
      uint16_t msb = (fcntDownPrev >> 16) + 1;  // assume a rollover
      fcnt32 |= ((uint32_t)msb << 16);          // add back the MSB part
    }
  }
  
  // check the MIC
  if(!verifyMIC(downlinkMsg, RADIOLIB_AES128_BLOCK_SIZE + downlinkMsgLen, this->sNwkSIntKey)) {
    #if !RADIOLIB_STATIC_ONLY
      delete[] downlinkMsg;
    #endif
    return(RADIOLIB_ERR_CRC_MISMATCH);
  }
  
  // save current fcnt to respective frame counter
  if (isAppDownlink) {
    this->aFcntDown = fcnt32;
  } else {
    this->nFcntDown = fcnt32;
  }

  // if this is a confirmed frame, save the downlink number (only app frames can be confirmed)
  bool isConfirmedDown = false;
  if((downlinkMsg[RADIOLIB_LORAWAN_FHDR_LEN_START_OFFS] & 0xFE) == RADIOLIB_LORAWAN_MHDR_MTYPE_CONF_DATA_DOWN) {
    this->confFcntDown = this->aFcntDown;
    isConfirmedDown = true;
  }

  // check the address
  uint32_t addr = LoRaWANNode::ntoh<uint32_t>(&downlinkMsg[RADIOLIB_LORAWAN_FHDR_DEV_ADDR_POS]);
  if(addr != this->devAddr) {
    RADIOLIB_DEBUG_PRINTLN("Device address mismatch, expected 0x%08X, got 0x%08X", this->devAddr, addr);
    #if !RADIOLIB_STATIC_ONLY
      delete[] downlinkMsg;
    #endif
    return(RADIOLIB_ERR_DOWNLINK_MALFORMED);
  }

  // process FOpts (if there are any)
  if(foptsLen > 0) {
    // there are some Fopts, decrypt them
    #if !RADIOLIB_STATIC_ONLY
      uint8_t* fopts = new uint8_t[RADIOLIB_MAX(RADIOLIB_LORAWAN_FHDR_FOPTS_LEN_MASK, (int)foptsLen)];
    #else
      uint8_t fopts[RADIOLIB_STATIC_ARRAY_SIZE];
    #endif

    // TODO it COULD be the case that the assumed FCnt rollover is incorrect, if possible figure out a way to catch this and retry with just fcnt16
    // if there are <= 15 bytes of FOpts, they are in the FHDR, otherwise they are in the payload
    // in case of the latter, process AES is if it were a normal payload but using the NwkSEncKey
    if(foptsLen <= RADIOLIB_LORAWAN_FHDR_FOPTS_LEN_MASK) {
      uint8_t ctrId = 0x01 + isAppDownlink; // see LoRaWAN v1.1 errata
      processAES(&downlinkMsg[RADIOLIB_LORAWAN_FHDR_FOPTS_POS], (size_t)foptsLen, this->nwkSEncKey, fopts, fcnt32, RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK, ctrId, true);
    } else {
      processAES(&downlinkMsg[RADIOLIB_LORAWAN_FRAME_PAYLOAD_POS(0)], (size_t)foptsLen, this->nwkSEncKey, fopts, fcnt32, RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK, 0x00, true);
    }

    RADIOLIB_DEBUG_PRINTLN("fopts:");
    RADIOLIB_DEBUG_HEXDUMP(fopts, foptsLen);

    bool hasADR = false;
    uint8_t numADR = 0;
    uint8_t lastCID = 0;

    // process the MAC command(s)
    int8_t remLen = foptsLen;
    uint8_t* foptsPtr = fopts;
    while(remLen > 0) {
      uint8_t cid = *foptsPtr;
      uint8_t macLen = getMacPayloadLength(cid);
      if(cid == RADIOLIB_LORAWAN_MAC_LINK_ADR) {
        // if there was an earlier ADR command but it was not the last, ignore it
        if(hasADR && lastCID != RADIOLIB_LORAWAN_MAC_LINK_ADR) {
          RADIOLIB_DEBUG_PRINTLN("Encountered non-consecutive block of ADR commands - skipping");
          remLen -= (macLen + 1);
          foptsPtr += (macLen + 1);
          lastCID = cid;
          continue;
        }
        // otherwise, set ADR flag to true and increase counter
        hasADR = true;
        numADR++;
      }
      if(macLen + 1 > remLen)
        break;
      LoRaWANMacCommand_t cmd = {
        .cid = cid,
        .payload = { 0 },
        .len = macLen,
        .repeat = (cid == RADIOLIB_LORAWAN_MAC_LINK_ADR ? numADR : (uint8_t)0),
      };
      memcpy(cmd.payload, foptsPtr + 1, macLen);
      RADIOLIB_DEBUG_PRINTLN("[%02X]: %02X %02X %02X %02X %02X (%d)",
                              cmd.cid, cmd.payload[0], cmd.payload[1], cmd.payload[2], cmd.payload[3], cmd.payload[4], cmd.len);

      // process the MAC command
      bool sendUp = execMacCommand(&cmd);
      if(sendUp) {
        pushMacCommand(&cmd, &this->commandsUp);
      }

      // processing succeeded, move in the buffer to the next command
      remLen -= (macLen + 1);
      foptsPtr += (macLen + 1);
      lastCID = cid;
      RADIOLIB_DEBUG_PRINTLN("Processed: %d, remaining: %d", (macLen + 1), remLen);
    }

    #if !RADIOLIB_STATIC_ONLY
      delete[] fopts;
    #endif

    RADIOLIB_DEBUG_PRINTLN("MAC response:");
    for (int i = 0; i < this->commandsUp.numCommands; i++) {
      RADIOLIB_DEBUG_HEXDUMP(&(this->commandsUp.commands[i].cid), sizeof(LoRaWANMacCommand_t));
    }

    // if FOptsLen for the next uplink is larger than can be piggybacked onto an uplink, send separate uplink
    if(this->commandsUp.len > 15) {
      size_t foptsBufSize = this->commandsUp.len;
      #if RADIOLIB_STATIC_ONLY
        uint8_t foptsBuff[RADIOLIB_STATIC_ARRAY_SIZE];
      #else
        uint8_t* foptsBuff = new uint8_t[foptsBufSize];
      #endif
      uint8_t* foptsPtr = foptsBuff;
      // append all MAC replies into fopts buffer
      int16_t i = 0;
      for (; i < this->commandsUp.numCommands; i++) {
        LoRaWANMacCommand_t cmd = this->commandsUp.commands[i];
        memcpy(foptsPtr, &cmd, 1 + cmd.len);
        foptsPtr += cmd.len + 1;
      }
      RADIOLIB_DEBUG_PRINTLN("Uplink MAC payload (%d commands):", this->commandsUp.numCommands);
      RADIOLIB_DEBUG_HEXDUMP(foptsBuff, foptsBufSize);

      // pop the commands from back to front
      for (; i >= 0; i--) {
        if(this->commandsUp.commands[i].repeat > 0) {
          this->commandsUp.commands[i].repeat--;
        } else {
          deleteMacCommand(this->commandsUp.commands[i].cid, &this->commandsUp);
        }
      }

      this->isMACPayload = true;
      // temporarily lift dutyCycle restrictions to allow immediate MAC response
      bool prevDC = this->dutyCycleEnabled;
      this->dutyCycleEnabled = false;
      RADIOLIB_DEBUG_PRINTLN("Sending MAC-only uplink .. ");
      state = this->uplink(foptsBuff, foptsBufSize, RADIOLIB_LORAWAN_FPORT_MAC_COMMAND);
      RADIOLIB_DEBUG_PRINTLN(" .. state: %d", state);
      this->dutyCycleEnabled = prevDC;

      #if !RADIOLIB_STATIC_ONLY
        delete[] foptsBuff;
      #endif

      #if RADIOLIB_STATIC_ONLY
        uint8_t strDown[RADIOLIB_STATIC_ARRAY_SIZE];
      #else
        uint8_t* strDown = new uint8_t[this->band->payloadLenMax[this->dataRates[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK]]];
      #endif
      size_t lenDown = 0;
      RADIOLIB_DEBUG_PRINTLN("Receiving after MAC-only uplink .. ");
      state = this->downlink(strDown, &lenDown);
      RADIOLIB_DEBUG_PRINTLN(" .. state: %d", state);
      #if !RADIOLIB_STATIC_ONLY
        delete[] strDown;
      #endif
      RADIOLIB_ASSERT(state);
    }

  }

  // a downlink was received, so reset the ADR counter to this uplink's fcnt
  this->adrFcnt = this->fcntUp;

  // pass the extra info if requested
  if(event) {
    event->dir = RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK;
    event->confirmed = isConfirmedDown;
    event->confirming = isConfirmingUp;
    event->datarate = this->dataRates[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK];
    event->freq = currentChannels[event->dir].freq;
    event->power = this->txPowerMax - this->txPowerCur * 2;
    event->fcnt = isAppDownlink ? this->aFcntDown : this->nFcntDown;
    event->port = downlinkMsg[RADIOLIB_LORAWAN_FHDR_FPORT_POS(foptsLen)];
  }

  // process Application payload (if there is any)
  if(payLen <= 0 || foptsLen > RADIOLIB_LORAWAN_FHDR_FOPTS_MAX_LEN) {
    // no payload
    *len = 0;
    #if !RADIOLIB_STATIC_ONLY
      delete[] downlinkMsg;
    #endif

    return(RADIOLIB_ERR_NONE);
  }

  // there is payload, and so there should be a port too
  // TODO pass the port?
  *len = payLen - 1;

  // TODO it COULD be the case that the assumed rollover is incorrect, then figure out a way to catch this and retry with just fcnt16
  processAES(&downlinkMsg[RADIOLIB_LORAWAN_FRAME_PAYLOAD_POS(foptsLen)], payLen - 1, this->appSKey, data, fcnt32, RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK, 0x00, true);
  
  #if !RADIOLIB_STATIC_ONLY
    delete[] downlinkMsg;
  #endif

  return(RADIOLIB_ERR_NONE);
}

#if defined(RADIOLIB_BUILD_ARDUINO)
int16_t LoRaWANNode::sendReceive(String& strUp, uint8_t port, String& strDown, bool isConfirmed, LoRaWANEvent_t* eventUp, LoRaWANEvent_t* eventDown) {
  // send the uplink
  int16_t state = this->uplink(strUp, port, isConfirmed, eventUp);
  RADIOLIB_ASSERT(state);

  // wait for the downlink
  state = this->downlink(strDown, eventDown);
  return(state);
}
#endif

int16_t LoRaWANNode::sendReceive(const char* strUp, uint8_t port, uint8_t* dataDown, size_t* lenDown, bool isConfirmed, LoRaWANEvent_t* eventUp, LoRaWANEvent_t* eventDown) {
  // send the uplink
  int16_t state = this->uplink(strUp, port, isConfirmed, eventUp);
  RADIOLIB_ASSERT(state);

  // wait for the downlink
  state = this->downlink(dataDown, lenDown, eventDown);
  return(state);
}

int16_t LoRaWANNode::sendReceive(uint8_t* dataUp, size_t lenUp, uint8_t port, uint8_t* dataDown, size_t* lenDown, bool isConfirmed, LoRaWANEvent_t* eventUp, LoRaWANEvent_t* eventDown) {
  // send the uplink
  int16_t state = this->uplink(dataUp, lenUp, port, isConfirmed, eventUp);
  RADIOLIB_ASSERT(state);

  // wait for the downlink
  state = this->downlink(dataDown, lenDown, eventDown);
  return(state);
}

void LoRaWANNode::setDeviceStatus(uint8_t battLevel) {
  this->battLevel = battLevel;
}

uint32_t LoRaWANNode::getFcntUp() {
  return(this->fcntUp);
}

uint32_t LoRaWANNode::getNFcntDown() {
  return(this->nFcntDown);
}

uint32_t LoRaWANNode::getAFcntDown() {
  return(this->aFcntDown);
}

void LoRaWANNode::resetFcntDown() {
  this->nFcntDown = 0;
  this->aFcntDown = 0;
}

uint32_t LoRaWANNode::generateMIC(uint8_t* msg, size_t len, uint8_t* key) {
  if((msg == NULL) || (len == 0)) {
    return(0);
  }

  RadioLibAES128Instance.init(key);
  uint8_t cmac[RADIOLIB_AES128_BLOCK_SIZE];
  RadioLibAES128Instance.generateCMAC(msg, len, cmac);
  return(((uint32_t)cmac[0]) | ((uint32_t)cmac[1] << 8) | ((uint32_t)cmac[2] << 16) | ((uint32_t)cmac[3]) << 24);
}

bool LoRaWANNode::verifyMIC(uint8_t* msg, size_t len, uint8_t* key) {
  if((msg == NULL) || (len < sizeof(uint32_t))) {
    return(0);
  }

  // extract MIC from the message
  uint32_t micReceived = LoRaWANNode::ntoh<uint32_t>(&msg[len - sizeof(uint32_t)]);

  // calculate the expected value and compare
  uint32_t micCalculated = generateMIC(msg, len - sizeof(uint32_t), key);
  if(micCalculated != micReceived) {
    RADIOLIB_DEBUG_PRINTLN("MIC mismatch, expected %08x, got %08x", micCalculated, micReceived);
    return(false);
  }

  return(true);
}

int16_t LoRaWANNode::setPhyProperties() {
  // set the physical layer configuration
  int8_t pwr = this->txPowerMax - this->txPowerCur * 2;
  int16_t state = RADIOLIB_ERR_INVALID_OUTPUT_POWER;
  while(state == RADIOLIB_ERR_INVALID_OUTPUT_POWER) {
    // go from the highest power and lower it until we hit one supported by the module
    state = this->phyLayer->setOutputPower(pwr--);
  }
  RADIOLIB_ASSERT(state);

  uint8_t syncWord[3] = { 0 };
  uint8_t syncWordLen = 0;
  size_t preLen = 0;
  if(this->FSK) {
    preLen = 8*RADIOLIB_LORAWAN_GFSK_PREAMBLE_LEN;
    syncWord[0] = (uint8_t)(RADIOLIB_LORAWAN_GFSK_SYNC_WORD >> 16);
    syncWord[1] = (uint8_t)(RADIOLIB_LORAWAN_GFSK_SYNC_WORD >> 8);
    syncWord[2] = (uint8_t)RADIOLIB_LORAWAN_GFSK_SYNC_WORD;
    syncWordLen = 3;
  
  } else {
    preLen = RADIOLIB_LORAWAN_LORA_PREAMBLE_LEN;
    syncWord[0] = RADIOLIB_LORAWAN_LORA_SYNC_WORD;
    syncWordLen = 1;
  
  }

  state = this->phyLayer->setSyncWord(syncWord, syncWordLen);
  RADIOLIB_ASSERT(state);

  state = this->phyLayer->setPreambleLength(preLen);
  return(state);
}

int16_t LoRaWANNode::setupChannelsDyn(bool joinRequest) {
  RADIOLIB_DEBUG_PRINTLN("Setting up dynamic channels");
  
  size_t num = 0;
  // copy the default defined channels into the first slots (where Tx = Rx)
  for(; num < 3 && this->band->txFreqs[num].enabled; num++) {
    this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][num] = this->band->txFreqs[num];
    this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK][num] = this->band->txFreqs[num];
    RADIOLIB_DEBUG_PRINTLN("Channel UL/DL %d frequency = %f MHz", this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][num].idx, this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][num].freq);
  }

  // if we're about to send a join-request, copy the join-request channels to the next slots
  if(joinRequest) {
    size_t numJR = 0;
    for(; numJR < 3 && this->band->txJoinReq[num].enabled; numJR++, num++) {
      this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][num] = this->band->txFreqs[num];
      this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK][num] = this->band->txFreqs[num];
      RADIOLIB_DEBUG_PRINTLN("Channel UL/DL %d frequency = %f MHz", this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][num].idx, this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][num].freq);
    }
  }

  // clear all remaining channels
  for(; num < RADIOLIB_LORAWAN_NUM_AVAILABLE_CHANNELS; num++) {
    this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][num] = RADIOLIB_LORAWAN_CHANNEL_NONE;
  }
  
  return(RADIOLIB_ERR_NONE);
}

// setup a subband and its corresponding join-request datarate
// WARNING: subBand starts at 1 (corresponds to all populair schemes)
int16_t LoRaWANNode::setupChannelsFix(uint8_t subBand) {
  RADIOLIB_DEBUG_PRINTLN("Setting up fixed channels");
  // randomly select one of 8 or 9 channels and find corresponding datarate
  uint8_t numChannels = this->band->numTxSpans == 1 ? 8 : 9;
  uint8_t rand = this->phyLayer->random(numChannels) + 1;     // range 1-8 or 1-9
  uint8_t drJR = RADIOLIB_LORAWAN_DATA_RATE_UNUSED;
  if(rand <= 8) {
    drJR = this->band->txSpans[0].joinRequestDataRate;        // if one of the first 8 channels, select datarate of span 0
  } else {
    drJR = this->band->txSpans[1].joinRequestDataRate;        // if ninth channel, select datarate of span 1
  }

  // if no subband is selected by user, cycle through banks of 8 using devNonce value
  if(subBand == 0) {
    uint8_t numBanks8 = this->band->txSpans[0].numChannels / 8;
    subBand = this->devNonce % numBanks8;
  }
  
  // chMask is set for 16 channels at once, so widen the Cntl value
  uint8_t chMaskCntl = (subBand - 1) / 2;   // compensate the 1 offset

  uint8_t numADR = 1;

  LoRaWANMacCommand_t cmd = {
    .cid = RADIOLIB_LORAWAN_MAC_LINK_ADR,
    .payload = { 0 }, 
    .len = MacTable[RADIOLIB_LORAWAN_MAC_LINK_ADR].lenDn,
    .repeat = 0,
  };

  // if there are two channel spans, first set the channel from second span
  if(this->band->numTxSpans == 2) {
    cmd.payload[0]  = (drJR << 4);          // set join-request datarate
    cmd.payload[0] |= 0;                    // set Tx power to maximum
    // enable channel that belongs to this subband
    cmd.payload[1]  = (1 << (subBand - 1));  // set channel mask
    cmd.payload[2]  = 0;
    cmd.payload[3]  = (7 << 4);             // set the chMaskCntl value to all channels off
    cmd.payload[3] |= 0;                    // keep NbTrans the same
    cmd.repeat = numADR++;
    (void)execMacCommand(&cmd, false);
  }

  cmd.len = MacTable[RADIOLIB_LORAWAN_MAC_LINK_ADR].lenDn;
  cmd.payload[0]  = (drJR << 4);            // set join-request datarate
  cmd.payload[0] |= 0;                      // set Tx power to maximum
  // now select the correct bank of 8 channels
  // 0x00 0xFF channel mask for subband = 2, 4.. (even)
  // 0xFF 0x00 channel mask for subband = 1, 3.. (odd)
  if(subBand % 2 == 0) {
    cmd.payload[1] = 0x00;
    cmd.payload[2] = 0xFF;
  } else {
    cmd.payload[1] = 0xFF;
    cmd.payload[2] = 0x00;
  }
  cmd.payload[3]  = (chMaskCntl << 4);      // set the chMaskCntl value
  cmd.payload[3] |= 0;                      // keep NbTrans the same
  cmd.repeat = numADR++;
  (void)execMacCommand(&cmd, false);

  return(RADIOLIB_ERR_NONE);
}

int16_t LoRaWANNode::processCFList(uint8_t* cfList) {
  RADIOLIB_DEBUG_PRINTLN("Processing CFList");
  
  if(this->band->bandType == RADIOLIB_LORAWAN_BAND_DYNAMIC) {
    // retrieve number of existing (default) channels
    size_t num = 0;
    for(int i = 0; i < RADIOLIB_LORAWAN_NUM_AVAILABLE_CHANNELS; i++) {
      if(!this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][i].enabled) {
        break;
      }
      num++;
    }

    LoRaWANMacCommand_t cmd = {
      .cid = RADIOLIB_LORAWAN_MAC_NEW_CHANNEL,
      .payload = { 0 }, 
      .len = 0,
      .repeat = 0,
    };
    // datarate range for all new channels is equal to the default channels
    cmd.payload[4] = (this->band->txFreqs[0].drMax << 4) | this->band->txFreqs[0].drMin;
    for(uint8_t i = 0; i < 5; i++, num++) {
      cmd.len = MacTable[RADIOLIB_LORAWAN_MAC_NEW_CHANNEL].lenDn;
      cmd.payload[0] = num;
      memcpy(&cmd.payload[1], &cfList[i*3], 3);
      (void)execMacCommand(&cmd);
    }
  } else {                // RADIOLIB_LORAWAN_BAND_FIXED
    // complete channel mask received, so clear all existing channels
    for(int i = 0; i < RADIOLIB_LORAWAN_NUM_AVAILABLE_CHANNELS; i++) {
      this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][i] = RADIOLIB_LORAWAN_CHANNEL_NONE;
    }

    LoRaWANMacCommand_t cmd = {
      .cid = RADIOLIB_LORAWAN_MAC_LINK_ADR,
      .payload = { 0 }, 
      .len = 0,
      .repeat = 0,
    };
    cmd.payload[0] = 0xFF;  // same datarate and payload

    // in case of mask-type bands, copy those frequencies that are masked true into the available TX channels
    size_t numChMasks = 3 + this->band->numTxSpans;   // 4 masks for bands with 2 spans, 5 spans for bands with 1 span
    for(size_t chMaskCntl = 0; chMaskCntl < numChMasks; chMaskCntl++) {
      cmd.len = MacTable[RADIOLIB_LORAWAN_MAC_LINK_ADR].lenDn;
      cmd.payload[3] = chMaskCntl << 4;               // NbTrans = 0 -> keep the same
      cmd.repeat = (chMaskCntl + 1);
      memcpy(&cmd.payload[1], &cfList[chMaskCntl*2], 2);
      (void)execMacCommand(&cmd);
    }
    // delete the ADR response
  }

  return(RADIOLIB_ERR_NONE);
}

int16_t LoRaWANNode::selectChannels() {
  // figure out which channel IDs are enabled (chMask may have disabled some) and are valid for the current datarate
  uint8_t numChannels = 0;
  uint8_t channelsEnabled[RADIOLIB_LORAWAN_NUM_AVAILABLE_CHANNELS];
  for(uint8_t i = 0; i < RADIOLIB_LORAWAN_NUM_AVAILABLE_CHANNELS; i++) {
    if(this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][i].enabled) {
      if(this->dataRates[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK] >= this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][i].drMin
         && this->dataRates[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK] <= this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][i].drMax) {
          channelsEnabled[numChannels] = i;
          numChannels++;
      }
    }
  }
  if(numChannels == 0) {
    RADIOLIB_DEBUG_PRINTLN("There are no channels defined - are you in ABP mode with no defined subband?");
    return(RADIOLIB_ERR_INVALID_CHANNEL);
  }
  // select a random ID & channel from the list of enabled and possible channels
  uint8_t channelID = channelsEnabled[this->phyLayer->random(numChannels)];
  this->currentChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK] = this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][channelID];
  
  if(this->band->bandType == RADIOLIB_LORAWAN_BAND_DYNAMIC) {
    // for dynamic bands, the downlink channel is the one matched to the uplink channel
    this->currentChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK] = this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK][channelID];
  
  } else {                // RADIOLIB_LORAWAN_BAND_FIXED
    // for fixed bands, the downlink channel is the uplink channel ID `modulo` number of downlink channels
    LoRaWANChannel_t channelDn;
    channelDn.enabled = true;
    channelDn.idx = this->currentChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK].idx % this->band->rx1Span.numChannels;
    channelDn.freq = this->band->rx1Span.freqStart + channelDn.idx*this->band->rx1Span.freqStep;
    channelDn.drMin = this->band->rx1Span.drMin;
    channelDn.drMax = this->band->rx1Span.drMax;
    this->currentChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK] = channelDn;

  }
  uint8_t drDown = getDownlinkDataRate(this->dataRates[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK], this->rx1DrOffset, this->band->rx1DataRateBase, 
                                       this->currentChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK].drMin, this->currentChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK].drMax);
  this->dataRates[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK] = drDown;

  return(RADIOLIB_ERR_NONE);
}

int16_t LoRaWANNode::setDatarate(uint8_t drUp, bool saveToEeprom) {
  // scan through all enabled channels and check if the requested datarate is available
  bool isValidDR = false;
  for(size_t i = 0; i < RADIOLIB_LORAWAN_NUM_AVAILABLE_CHANNELS; i++) {
    LoRaWANChannel_t *chnl = &(this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][i]);
    if(chnl->enabled) {
      if(drUp >= chnl->drMin && drUp <= chnl->drMax) {
        isValidDR = true;
        break;
      }
    }
  }
  if(!isValidDR) {
    RADIOLIB_DEBUG_PRINTLN("No defined channel allows datarate %d", drUp);
    return(RADIOLIB_ERR_INVALID_DATA_RATE);
  }

  LoRaWANMacCommand_t cmd = {
    .cid = RADIOLIB_LORAWAN_MAC_LINK_ADR,
    .payload = { 0 }, 
    .len = MacTable[RADIOLIB_LORAWAN_MAC_LINK_ADR].lenDn,
    .repeat = 0,
  };
  cmd.payload[0]  = (drUp << 4);
  cmd.payload[0] |= 0x0F;               // keep Tx Power the same
  cmd.payload[3]  = (1 << 7);           // set the RFU bit, which means that the channel mask gets ignored
  cmd.payload[3] |= 0;                  // keep NbTrans the same
  (void)execMacCommand(&cmd, saveToEeprom);

  // check if ACK is set for Tx Power
  if((cmd.payload[0] >> 1) != 1) {
    return(RADIOLIB_ERR_INVALID_DATA_RATE);
  }
  
  return(RADIOLIB_ERR_NONE);
}

void LoRaWANNode::setADR(bool enable) {
  this->adrEnabled = enable;
}

void LoRaWANNode::setDutyCycle(bool enable, uint32_t msPerHour) {
  this->dutyCycleEnabled = enable;
  if(msPerHour <= 0) {
    this->dutyCycle = this->band->dutyCycle;
  } else {
    this->dutyCycle = msPerHour;
  }
}

// given an airtime in milliseconds, calculate the minimum uplink interval
// to adhere to a given dutyCycle
uint32_t LoRaWANNode::dutyCycleInterval(uint32_t msPerHour, uint32_t airtime) {
  if(msPerHour == 0 || airtime == 0) {
    return(0);
  }
  uint32_t oneHourInMs = (uint32_t)60 * (uint32_t)60 * (uint32_t)1000;
  float numPackets = msPerHour / airtime;
  uint32_t delayMs = oneHourInMs / numPackets + 1;  // + 1 to prevent rounding problems
  return(delayMs);
}

uint32_t LoRaWANNode::timeUntilUplink() {
  Module* mod = this->phyLayer->getMod();
  uint32_t nextUplink = this->rxDelayStart + dutyCycleInterval(this->dutyCycle, this->lastToA);
  if(mod->hal->millis() > nextUplink){
    return(0);
  }
  return(nextUplink - mod->hal->millis() + 1);
}

void LoRaWANNode::setDwellTime(bool enable, uint32_t msPerUplink) {
  this->dwellTimeEnabledUp = enable;
  if(msPerUplink <= 0) {
    this->dwellTimeUp = this->band->dwellTimeUp;
  } else {
    this->dwellTimeUp = msPerUplink;
  }
}

uint8_t LoRaWANNode::maxPayloadDwellTime() {
  // configure current datarate
  DataRate_t dr;
  findDataRate(this->dataRates[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK], &dr);
  (void)this->phyLayer->setDataRate(dr);
  uint8_t minPayLen = 0;
  uint8_t maxPayLen = 255;
  uint8_t payLen = (minPayLen + maxPayLen) / 2;
  // do some binary search to find maximum allowed payload length
  while(payLen != minPayLen && payLen != maxPayLen) {
    if(this->phyLayer->getTimeOnAir(payLen) > this->dwellTimeUp) {
      maxPayLen = payLen;
    } else {
      minPayLen = payLen;
    }
    payLen = (minPayLen + maxPayLen) / 2;
  }
  return(payLen - 13);  // fixed 13-byte header
}

int16_t LoRaWANNode::setTxPower(int8_t txPower, bool saveToEeprom) {
  // only allow values within the band's (or MAC state) maximum
  if(txPower > this->txPowerMax) {
    return(RADIOLIB_ERR_INVALID_OUTPUT_POWER);
  }
  // Tx Power is set in steps of two
  // the selected value is rounded down to nearest multiple of two away from txPowerMax
  // e.g. on EU868, max is 16; if 13 is selected then we set to 12
  uint8_t numSteps = (this->txPowerMax - txPower + 1) / (-RADIOLIB_LORAWAN_POWER_STEP_SIZE_DBM);

  LoRaWANMacCommand_t cmd = {
    .cid = RADIOLIB_LORAWAN_MAC_LINK_ADR,
    .payload = { 0 }, 
    .len = MacTable[RADIOLIB_LORAWAN_MAC_LINK_ADR].lenDn,
    .repeat = 0,
  };
  cmd.payload[0]  = 0xF0;               // keep datarate the same
  cmd.payload[0] |= numSteps;           // set the Tx Power
  cmd.payload[3]  = (1 << 7);           // set the RFU bit, which means that the channel mask gets ignored
  cmd.payload[3] |= 0;                  // keep NbTrans the same
  (void)execMacCommand(&cmd, saveToEeprom);

  // check if ACK is set for Tx Power
  if((cmd.payload[0] >> 2) != 1) {
    return(RADIOLIB_ERR_INVALID_OUTPUT_POWER);
  }
  
  return(RADIOLIB_ERR_NONE);
}

int16_t LoRaWANNode::findDataRate(uint8_t dr, DataRate_t* dataRate) {
  uint8_t dataRateBand = this->band->dataRates[dr];

  if(dataRateBand & RADIOLIB_LORAWAN_DATA_RATE_FSK_50_K) {
    dataRate->fsk.bitRate = 50;
    dataRate->fsk.freqDev = 25;
  
  } else {
    uint8_t bw = dataRateBand & 0x0C;
    switch(bw) {
      case(RADIOLIB_LORAWAN_DATA_RATE_BW_125_KHZ):
        dataRate->lora.bandwidth = 125.0;
        break;
      case(RADIOLIB_LORAWAN_DATA_RATE_BW_250_KHZ):
        dataRate->lora.bandwidth = 250.0;
        break;
      case(RADIOLIB_LORAWAN_DATA_RATE_BW_500_KHZ):
        dataRate->lora.bandwidth = 500.0;
        break;
      default:
        dataRate->lora.bandwidth = 125.0;
    }
    
    dataRate->lora.spreadingFactor = ((dataRateBand & 0x70) >> 4) + 6;
    dataRate->lora.codingRate = (dataRateBand & 0x03) + 5;
    RADIOLIB_DEBUG_PRINTLN("DR %d: LORA (SF: %d, BW: %f, CR: %d)", 
                            dataRateBand, dataRate->lora.spreadingFactor, dataRate->lora.bandwidth, dataRate->lora.codingRate);
  }

  return(RADIOLIB_ERR_NONE);
}

int16_t LoRaWANNode::configureChannel(uint8_t dir) {
  // set the frequency
  RADIOLIB_DEBUG_PRINTLN("");
  RADIOLIB_DEBUG_PRINTLN("Channel frequency %cL = %f MHz", dir ? 'D' : 'U', this->currentChannels[dir].freq);
  int state = this->phyLayer->setFrequency(this->currentChannels[dir].freq);
  RADIOLIB_ASSERT(state);

  // if this channel is an FSK channel, toggle the FSK switch
  if(this->band->dataRates[this->dataRates[dir]] == RADIOLIB_LORAWAN_DATA_RATE_FSK_50_K) {
    this->FSK = true;
  } else {
    this->FSK = false;
  }

  DataRate_t dr;
  findDataRate(this->dataRates[dir], &dr);
  state = this->phyLayer->setDataRate(dr);
  RADIOLIB_ASSERT(state);

  if(this->FSK) {
    state = this->phyLayer->setDataShaping(RADIOLIB_SHAPING_1_0);
    RADIOLIB_ASSERT(state);
    state = this->phyLayer->setEncoding(RADIOLIB_ENCODING_WHITENING);
  }

  return(state);
}

bool LoRaWANNode::sendMacCommandReq(uint8_t cid) {
  bool valid = false;
  for(size_t i = 0; i < RADIOLIB_LORAWAN_NUM_MAC_COMMANDS; i++) {
    if(MacTable[i].cid == cid) {
      valid = MacTable[i].user;
    }
  }
  if(!valid)
    return(false);
  
  LoRaWANMacCommand_t cmd = {
    .cid = cid,
    .payload = { 0 }, 
    .len = 0,
    .repeat = 0,
  };
  pushMacCommand(&cmd, &this->commandsUp);
  return(true);
}

int16_t LoRaWANNode::pushMacCommand(LoRaWANMacCommand_t* cmd, LoRaWANMacCommandQueue_t* queue) {
  if(queue->numCommands >= RADIOLIB_LORAWAN_MAC_COMMAND_QUEUE_SIZE) {
    return(RADIOLIB_ERR_COMMAND_QUEUE_FULL);
  }

  memcpy(&queue->commands[queue->numCommands], cmd, sizeof(LoRaWANMacCommand_t));
  queue->numCommands++;
  queue->len += 1 + cmd->len; // 1 byte for command ID, len bytes for payload

  return(RADIOLIB_ERR_NONE);
}

int16_t LoRaWANNode::deleteMacCommand(uint8_t cid, LoRaWANMacCommandQueue_t* queue, uint8_t payload[5]) {
  if(queue->numCommands == 0) {
    return(RADIOLIB_ERR_COMMAND_QUEUE_EMPTY);
  }

  for(size_t index = 0; index < queue->numCommands; index++) {
    if(queue->commands[index].cid == cid) {
      // if a pointer to a payload is supplied, copy the command's payload over
      if(payload) {
        memcpy(payload, queue->commands[index].payload, queue->commands[index].len);
      }
      queue->len -= (1 + queue->commands[index].len); // 1 byte for command ID, len for payload
      // move all subsequent commands one forward in the queue
      if(index < RADIOLIB_LORAWAN_MAC_COMMAND_QUEUE_SIZE - 1) {
        memmove(&queue->commands[index], &queue->commands[index + 1], (RADIOLIB_LORAWAN_MAC_COMMAND_QUEUE_SIZE - index - 1) * sizeof(LoRaWANMacCommand_t));
      }
      // set the latest element to all 0
      memset(&queue->commands[RADIOLIB_LORAWAN_MAC_COMMAND_QUEUE_SIZE - 1], 0x00, sizeof(LoRaWANMacCommand_t));
      queue->numCommands--;
      return(RADIOLIB_ERR_NONE);
    }
  }

  return(RADIOLIB_ERR_COMMAND_QUEUE_ITEM_NOT_FOUND);
}

bool LoRaWANNode::execMacCommand(LoRaWANMacCommand_t* cmd, bool saveToEeprom) {
  RADIOLIB_DEBUG_PRINTLN("exe MAC CID = %02x, len = %d", cmd->cid, cmd->len);

  Module* mod = this->phyLayer->getMod();
#if defined(RADIOLIB_EEPROM_UNSUPPORTED)
  (void)saveToEeprom;
  (void)mod;
#endif

  if(cmd->cid >= RADIOLIB_LORAWAN_MAC_PROPRIETARY) {
    // TODO call user-provided callback for proprietary MAC commands?
    return(false);
  }

  switch(cmd->cid) {
    case(RADIOLIB_LORAWAN_MAC_RESET): {
      // get the server version
      uint8_t srvVersion = cmd->payload[0];
      RADIOLIB_DEBUG_PRINTLN("Server version: 1.%d", srvVersion);
      if(srvVersion == this->rev) {
        // valid server version, stop sending the ResetInd MAC command
        deleteMacCommand(RADIOLIB_LORAWAN_MAC_RESET, &this->commandsUp);
      }
      return(false);
    } break;

    case(RADIOLIB_LORAWAN_MAC_LINK_CHECK): {
      pushMacCommand(cmd, &this->commandsDown);
      return(false);
    } break;

    case(RADIOLIB_LORAWAN_MAC_LINK_ADR): {
      int16_t state = RADIOLIB_ERR_UNKNOWN;
      // get the ADR configuration
      // per spec, all these configuration should only be set if all ACKs are set, otherwise retain previous state
      // but we don't bother and try to set each individual command
      uint8_t drUp = (cmd->payload[0] & 0xF0) >> 4;
      uint8_t txPower = cmd->payload[0] & 0x0F;
      uint16_t chMask = LoRaWANNode::ntoh<uint16_t>(&cmd->payload[1]);
      uint8_t chMaskCntl = (cmd->payload[3] & 0x70) >> 4;
      uint8_t nbTrans = cmd->payload[3] & 0x0F;
      RADIOLIB_DEBUG_PRINTLN("ADR REQ: dataRate = %d, txPower = %d, chMask = 0x%04x, chMaskCntl = %02x, nbTrans = %d", drUp, txPower, chMask, chMaskCntl, nbTrans);

      // apply the configuration
      uint8_t drAck = 0;
      if(drUp == 0x0F) {  // keep the same
        drAck = 1;

        // replace the 'placeholder' with the current actual value for saving
        cmd->payload[0] = (cmd->payload[0] & 0x0F) | (this->dataRates[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK] << 4);

      } else if (this->band->dataRates[drUp] != RADIOLIB_LORAWAN_DATA_RATE_UNUSED) {
        // check if the module supports this data rate
        DataRate_t dr;
        findDataRate(drUp, &dr);
        state = this->phyLayer->checkDataRate(dr);
        if(state == RADIOLIB_ERR_NONE) {
          uint8_t drDown = getDownlinkDataRate(drUp, this->rx1DrOffset, this->band->rx1DataRateBase,
                                               this->currentChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK].drMin,
                                               this->currentChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK].drMax);
          this->dataRates[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK] = drUp;
          this->dataRates[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK] = drDown;
          drAck = 1;
        } else {
          RADIOLIB_DEBUG_PRINTLN("ADR failed to configure dataRate %d, code %d!", drUp, state);
        }
      
      }

      // try to apply the power configuration
      uint8_t pwrAck = 0;
      if(txPower == 0x0F) {
        pwrAck = 1;

        // replace the 'placeholder' with the current actual value for saving
        cmd->payload[0] = (cmd->payload[0] & 0xF0) | this->txPowerCur;

      } else {
        int8_t pwr = this->txPowerMax - 2*txPower;
        state = RADIOLIB_ERR_INVALID_OUTPUT_POWER;
        while(state == RADIOLIB_ERR_INVALID_OUTPUT_POWER) {
          // go from the highest power and lower it until we hit one supported by the module
          state = this->phyLayer->setOutputPower(pwr--);
        }
        // only acknowledge if the requested datarate was succesfully configured
        if(state == RADIOLIB_ERR_NONE) {
          pwrAck = 1;
          this->txPowerCur = txPower;
        }
      }


      uint8_t chMaskAck = 1;
      // only apply channel mask when the RFU bit is not set
      // (which is set on the internal MAC command when creating new session)
      if((cmd->payload[3] >> 7) == 0) {
        if(this->band->bandType == RADIOLIB_LORAWAN_BAND_DYNAMIC) {
          chMaskAck = (uint8_t)this->applyChannelMaskDyn(chMaskCntl, chMask);

        } else {                // RADIOLIB_LORAWAN_BAND_FIXED
          bool clearChannels = false;
          if(cmd->repeat == 1) {
            // if this is the first ADR command in the queue, clear all saved channels
            // so we can apply the new channel mask
            clearChannels = true;
            RADIOLIB_DEBUG_PRINTLN("ADR mask: clearing channels");
          } else {
            // if this is not the first ADR command, clear the ADR response that was in the queue
            (void)deleteMacCommand(RADIOLIB_LORAWAN_MAC_LINK_ADR, &this->commandsUp);
          }
          chMaskAck = (uint8_t)this->applyChannelMaskFix(chMaskCntl, chMask, clearChannels);

        }
      }

      if(nbTrans == 0) {  // keep the same
        cmd->payload[3] = (cmd->payload[3] & 0xF0) | this->nbTrans; // set current number of retransmissions for saving
      } else {
        this->nbTrans = nbTrans;
      }

#if !defined(RADIOLIB_EEPROM_UNSUPPORTED)
    if(saveToEeprom) {
      uint8_t payLen = MacTable[RADIOLIB_LORAWAN_MAC_LINK_ADR].lenDn;
      if(this->band->bandType == RADIOLIB_LORAWAN_BAND_DYNAMIC) {
        // if RFU bit is set, this is just a change in Datarate or TxPower, so read ADR command and overwrite first byte
        if((cmd->payload[3] >> 7) == 1) {
          mod->hal->readPersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_LINK_ADR_ID) + 1, &(cmd->payload[1]), 3);
        }
        // if there was no channel mask (all zeroes), we should never apply that channel mask, so set RFU bit again
        if(cmd->payload[1] == 0 && cmd->payload[2] == 0) {
          cmd->payload[3] |= (1 << 7);
        }

        // save to the single ADR MAC location
        mod->hal->writePersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_LINK_ADR_ID), &(cmd->payload[0]), payLen);
      
      } else {                // RADIOLIB_LORAWAN_BAND_FIXED
        RADIOLIB_DEBUG_PRINTLN("[1] Repeat: %d, RFU: %d, payload: %02X %02X %02X %02X",
                                cmd->repeat, (cmd->payload[3] >> 7),
                                cmd->payload[0], cmd->payload[1], cmd->payload[2], cmd->payload[3]);
        // if RFU bit is set, this is just a change in Datarate or TxPower
        // so read bytes 1..3 from last stored ADR command into the current MAC payload and re-store it
        if((cmd->payload[3] >> 7) == 1) {
          // read how many ADR masks are already stored
          uint8_t numMacADR = mod->hal->getPersistentParameter<uint8_t>(RADIOLIB_EEPROM_LORAWAN_NUM_ADR_MASKS_ID);
          if(numMacADR > 0) {
            mod->hal->readPersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_UL_CHANNELS_ID) + (numMacADR - 1) * payLen + 1, &(cmd->payload[1]), 3);
            mod->hal->writePersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_UL_CHANNELS_ID) + (numMacADR - 1) * payLen, &(cmd->payload[0]), payLen);
          }
        
        } else {
          // save to the uplink channel location, to the cmd->repeat-th slot of 4 bytes
          mod->hal->writePersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_UL_CHANNELS_ID) + (cmd->repeat - 1) * payLen, &(cmd->payload[0]), payLen);
          // saved an ADR mask, so re-store counter
          mod->hal->setPersistentParameter<uint8_t>(RADIOLIB_EEPROM_LORAWAN_NUM_ADR_MASKS_ID, cmd->repeat);
        }
        RADIOLIB_DEBUG_PRINTLN("[2] Repeat: %d, RFU: %d, payload: %02X %02X %02X %02X",
                                cmd->repeat, (cmd->payload[3] >> 7),
                                cmd->payload[0], cmd->payload[1], cmd->payload[2], cmd->payload[3]);
      }
    }
#endif

      // send the reply
      cmd->len = 1;
      cmd->payload[0] = (pwrAck << 2) | (drAck << 1) | (chMaskAck << 0);
      cmd->repeat = 0;  // discard any repeat value that may have been set
      RADIOLIB_DEBUG_PRINTLN("ADR ANS: status = 0x%02x", cmd->payload[0]);
      return(true);
    } break;

    case(RADIOLIB_LORAWAN_MAC_DUTY_CYCLE): {
      uint8_t maxDutyCycle = cmd->payload[0] & 0x0F;
      RADIOLIB_DEBUG_PRINTLN("Max duty cycle: 1/2^%d", maxDutyCycle);
      if(maxDutyCycle == 0) {
        this->dutyCycle = this->band->dutyCycle;
      } else {
        this->dutyCycle = (uint32_t)60 * (uint32_t)60 * (uint32_t)1000 / (uint32_t)(1UL << maxDutyCycle);
      }

#if !defined(RADIOLIB_EEPROM_UNSUPPORTED)
    if(saveToEeprom) {
      mod->hal->setPersistentParameter<uint8_t>(RADIOLIB_EEPROM_LORAWAN_DUTY_CYCLE_ID, cmd->payload[0]);

    }
#endif

      cmd->len = 0;
      return(true);
    } break;

    case(RADIOLIB_LORAWAN_MAC_RX_PARAM_SETUP): {
      // get the configuration
      this->rx1DrOffset = (cmd->payload[0] & 0x70) >> 4;
      uint8_t rx1OffsAck = 1;
      this->rx2.drMax = cmd->payload[0] & 0x0F;
      uint8_t rx2Ack = 1;
      uint32_t freqRaw = LoRaWANNode::ntoh<uint32_t>(&cmd->payload[1], 3);
      this->rx2.freq = (float)freqRaw/10000.0;
      RADIOLIB_DEBUG_PRINTLN("Rx param REQ: rx1DrOffset = %d, rx2DataRate = %d, freq = %f", this->rx1DrOffset, this->rx2.drMax, this->rx2.freq);
      
      // apply the configuration
      uint8_t chanAck = 0;
      if(this->phyLayer->setFrequency(this->rx2.freq) == RADIOLIB_ERR_NONE) {
        chanAck = 1;
        this->phyLayer->setFrequency(this->currentChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK].freq);
      }

#if !defined(RADIOLIB_EEPROM_UNSUPPORTED)
    if(saveToEeprom) {
      uint8_t payLen = MacTable[RADIOLIB_LORAWAN_MAC_RX_PARAM_SETUP].lenDn;
      mod->hal->writePersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_RX_PARAM_SETUP_ID), &(cmd->payload[0]), payLen);

    }
#endif

      // TODO this should be sent repeatedly until the next downlink
      cmd->len = 1;
      cmd->payload[0] = (rx1OffsAck << 2) | (rx2Ack << 1) | (chanAck << 0);
      RADIOLIB_DEBUG_PRINTLN("Rx param ANS: status = 0x%02x", cmd->payload[0]);
      return(true);
    } break;

    case(RADIOLIB_LORAWAN_MAC_DEV_STATUS): {
      // set the uplink reply
      cmd->len = 2;
      cmd->payload[1] = this->battLevel;
      int8_t snr = this->phyLayer->getSNR();
      cmd->payload[0] = snr & 0x3F;

      RADIOLIB_DEBUG_PRINTLN("DevStatus ANS: status = 0x%02x%02x", cmd->payload[0], cmd->payload[1]);
      return(true);
    } break;

    case(RADIOLIB_LORAWAN_MAC_NEW_CHANNEL): {
      // get the configuration
      uint8_t chIndex = cmd->payload[0];
      uint32_t freqRaw = LoRaWANNode::ntoh<uint32_t>(&cmd->payload[1], 3);
      float freq = (float)freqRaw/10000.0;
      uint8_t maxDr = (cmd->payload[4] & 0xF0) >> 4;
      uint8_t minDr = cmd->payload[4] & 0x0F;
      RADIOLIB_DEBUG_PRINTLN("New channel: index = %d, freq = %f MHz, maxDr = %d, minDr = %d", chIndex, freq, maxDr, minDr);
      uint8_t newChAck = 0;
      uint8_t freqAck = 0;

      this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][chIndex].enabled = true;
      this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][chIndex].idx   = chIndex;
      this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][chIndex].freq  = freq;
      this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][chIndex].drMin = minDr;
      this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][chIndex].drMax = maxDr;
      
      // downlink channel is identical to uplink channel
      this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK][chIndex] = this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][chIndex];
      newChAck = 1;
      
      // check if the frequency is possible
      if(this->phyLayer->setFrequency(this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][chIndex].freq) == RADIOLIB_ERR_NONE) {
        freqAck = 1;
        this->phyLayer->setFrequency(this->currentChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK].freq);
      }
      
      RADIOLIB_DEBUG_PRINTLN("UL: %d %d %5.2f (%d - %d) | DL: %d %d %5.2f (%d - %d)", 
                              this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][chIndex].idx,
                              this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][chIndex].enabled,
                              this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][chIndex].freq,
                              this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][chIndex].drMin,
                              this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][chIndex].drMax,

                              this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK][chIndex].idx,
                              this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK][chIndex].enabled,
                              this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK][chIndex].freq,
                              this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK][chIndex].drMin,
                              this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK][chIndex].drMax
                            );

#if !defined(RADIOLIB_EEPROM_UNSUPPORTED)
    if(saveToEeprom) {
      // save to uplink channels location, to the chIndex-th slot of 5 bytes
      uint8_t payLen = MacTable[RADIOLIB_LORAWAN_MAC_NEW_CHANNEL].lenDn;
      RADIOLIB_DEBUG_PRINTLN("Saving channel:");
      RADIOLIB_DEBUG_HEXDUMP(&(cmd->payload[0]), payLen);
      mod->hal->writePersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_UL_CHANNELS_ID) + chIndex * payLen, &(cmd->payload[0]), payLen);

    }
#endif

      // send the reply
      cmd->len = 1;
      cmd->payload[0] = (newChAck << 1) | (freqAck << 0);

      return(true);
    } break;

    case(RADIOLIB_LORAWAN_MAC_DL_CHANNEL): {
      // get the configuration
      uint8_t chIndex = cmd->payload[0];
      uint32_t freqRaw = LoRaWANNode::ntoh<uint32_t>(&cmd->payload[1], 3);
      float freq = (float)freqRaw/10000.0;
      RADIOLIB_DEBUG_PRINTLN("DL channel: index = %d, freq = %f MHz", chIndex, freq);
      uint8_t freqDlAck = 0;
      uint8_t freqUlAck = 0;
      
      // check if the frequency is possible
      if(this->phyLayer->setFrequency(freq) == RADIOLIB_ERR_NONE) {
        freqDlAck = 1;
        this->phyLayer->setFrequency(this->currentChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK].freq);
      }
      
      // update the downlink frequency
      for(int i = 0; i < RADIOLIB_LORAWAN_NUM_AVAILABLE_CHANNELS; i++) {
        if(this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK][i].idx == chIndex) {
          this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK][i].freq = freq;
          // check if the corresponding uplink frequency is actually set
          if(this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][i].freq > 0) {
            freqUlAck = 1;
          }
        }
      }
      
#if !defined(RADIOLIB_EEPROM_UNSUPPORTED)
    if(saveToEeprom) {
      // save to downlink channels location, to the chIndex-th slot of 4 bytes
      uint8_t payLen = MacTable[RADIOLIB_LORAWAN_MAC_DL_CHANNEL].lenDn;
      mod->hal->writePersistentStorage(mod->hal->getPersistentAddr(RADIOLIB_EEPROM_LORAWAN_DL_CHANNELS_ID) + chIndex * payLen, &(cmd->payload[0]), payLen);

    }
#endif 

      // TODO send this repeatedly until a downlink is received
      cmd->len = 1;
      cmd->payload[0] = (freqUlAck << 1) | (freqDlAck << 0);

      return(true);
    } break;

    case(RADIOLIB_LORAWAN_MAC_RX_TIMING_SETUP): {
      // get the configuration
      uint8_t delay = cmd->payload[0] & 0x0F;
      RADIOLIB_DEBUG_PRINTLN("RX timing: delay = %d sec", delay);
      
      // apply the configuration
      if(delay == 0) {
        delay = 1;
      }
      this->rxDelays[0] = delay * 1000;
      this->rxDelays[1] = this->rxDelays[0] + 1000;

#if !defined(RADIOLIB_EEPROM_UNSUPPORTED)
    if(saveToEeprom) {
      mod->hal->setPersistentParameter<uint8_t>(RADIOLIB_EEPROM_LORAWAN_RX_TIMING_SETUP_ID, cmd->payload[0]);
      
    }
#endif

      // send the reply
      cmd->len = 0;

      // TODO send this repeatedly until a downlink is received
      return(true);
    } break;

    case(RADIOLIB_LORAWAN_MAC_TX_PARAM_SETUP): {
      uint8_t dlDwell = (cmd->payload[0] & 0x20) >> 5;
      uint8_t ulDwell = (cmd->payload[0] & 0x10) >> 4;
      uint8_t maxEirpRaw = cmd->payload[0] & 0x0F;

      // who the f came up with this ...
      const uint8_t eirpEncoding[] = { 8, 10, 12, 13, 14, 16, 18, 20, 21, 24, 26, 27, 29, 30, 33, 36 };
      this->txPowerMax = eirpEncoding[maxEirpRaw];
      RADIOLIB_DEBUG_PRINTLN("TX timing: dlDwell = %d, ulDwell = %d, maxEirp = %d dBm", dlDwell, ulDwell, this->txPowerMax);

      this->dwellTimeEnabledUp = ulDwell ? true : false;
      this->dwellTimeUp = ulDwell ? RADIOLIB_LORAWAN_DWELL_TIME : 0;

      this->dwellTimeEnabledDn = dlDwell ? true : false;
      this->dwellTimeDn = dlDwell ? RADIOLIB_LORAWAN_DWELL_TIME : 0;

#if !defined(RADIOLIB_EEPROM_UNSUPPORTED)
    if(saveToEeprom) {
      mod->hal->setPersistentParameter<uint8_t>(RADIOLIB_EEPROM_LORAWAN_TX_PARAM_SETUP_ID, cmd->payload[0]);
      
    }
#endif

      cmd->len = 0;
      return(true);
    } break;

    case(RADIOLIB_LORAWAN_MAC_REKEY): {
      // get the server version
      uint8_t srvVersion = cmd->payload[0];
      RADIOLIB_DEBUG_PRINTLN("Server version: 1.%d", srvVersion);
      if((srvVersion > 0) && (srvVersion <= this->rev)) {
        // valid server version, stop sending the ReKey MAC command
        deleteMacCommand(RADIOLIB_LORAWAN_MAC_REKEY, &this->commandsUp);
      }
      return(false);
    } break;

    case(RADIOLIB_LORAWAN_MAC_ADR_PARAM_SETUP): {
      this->adrLimitExp = (cmd->payload[0] & 0xF0) >> 4;
      this->adrDelayExp = cmd->payload[0] & 0x0F;
      RADIOLIB_DEBUG_PRINTLN("ADR param setup: limitExp = %d, delayExp = %d", this->adrLimitExp, this->adrDelayExp);

#if !defined(RADIOLIB_EEPROM_UNSUPPORTED)
    if(saveToEeprom) {
      mod->hal->setPersistentParameter<uint8_t>(RADIOLIB_EEPROM_LORAWAN_ADR_PARAM_SETUP_ID, cmd->payload[0]);
      
    }
#endif

      cmd->len = 0;
      return(true);
    } break;

    case(RADIOLIB_LORAWAN_MAC_DEVICE_TIME): {
      pushMacCommand(cmd, &this->commandsDown);
      return(false);
    } break;

    case(RADIOLIB_LORAWAN_MAC_FORCE_REJOIN): {
      // TODO implement this
      uint16_t rejoinReq = LoRaWANNode::ntoh<uint16_t>(&cmd->payload[0]);
      uint8_t period = (rejoinReq & 0x3800) >> 11;
      uint8_t maxRetries = (rejoinReq & 0x0700) >> 8;
      uint8_t rejoinType = (rejoinReq & 0x0070) >> 4;
      uint8_t dr = rejoinReq & 0x000F;
      RADIOLIB_DEBUG_PRINTLN("Force rejoin: period = %d, maxRetries = %d, rejoinType = %d, dr = %d", period, maxRetries, rejoinType, dr);
      (void)period;
      (void)maxRetries;
      (void)rejoinType;
      (void)dr;
      return(false);
    } break;

    case(RADIOLIB_LORAWAN_MAC_REJOIN_PARAM_SETUP): {
      // TODO implement this
      uint8_t maxTime = (cmd->payload[0] & 0xF0) >> 4;
      uint8_t maxCount = cmd->payload[0] & 0x0F;
      RADIOLIB_DEBUG_PRINTLN("Rejoin setup: maxTime = %d, maxCount = %d", maxTime, maxCount);

#if !defined(RADIOLIB_EEPROM_UNSUPPORTED)
    if(saveToEeprom) {
      mod->hal->setPersistentParameter<uint8_t>(RADIOLIB_EEPROM_LORAWAN_REJOIN_PARAM_SETUP_ID, cmd->payload[0]);
      
    }
#endif

      cmd->len = 0;
      cmd->payload[0] = (1 << 1) | 1;

      (void)maxTime;
      (void)maxCount;
      return(true);
    } break;
  }

  return(false);
}

bool LoRaWANNode::applyChannelMaskDyn(uint8_t chMaskCntl, uint16_t chMask) {
  for(size_t i = 0; i < RADIOLIB_LORAWAN_NUM_AVAILABLE_CHANNELS; i++) {
    if(chMaskCntl == 0) {
      // apply the mask by looking at each channel bit
      RADIOLIB_DEBUG_PRINTLN("ADR channel %d: %d --> %d", this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][i].idx, this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][i].enabled, (chMask >> i) & 0x01);
      if(chMask & (1UL << i)) {
        // if it should be enabled but is not currently defined, stop immediately
        if(this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][i].idx == RADIOLIB_LORAWAN_CHANNEL_INDEX_NONE) {
          return(false);
        }
        this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][i].enabled = true;
      } else {
        this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][i].enabled = false;
      }

    } else if(chMaskCntl == 6) {
      // enable all defined channels
      if(this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][i].idx != RADIOLIB_LORAWAN_CHANNEL_INDEX_NONE) {
        this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][i].enabled = true;
      }
    }
    
  }

  for (int i = 0; i < RADIOLIB_LORAWAN_NUM_AVAILABLE_CHANNELS; i++) {
    RADIOLIB_DEBUG_PRINTLN("UL: %d %d %5.2f (%d - %d) | DL: %d %d %5.2f (%d - %d)",
                            this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][i].idx,
                            this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][i].enabled,
                            this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][i].freq,
                            this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][i].drMin,
                            this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][i].drMax,

                            this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK][i].idx,
                            this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK][i].enabled,
                            this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK][i].freq,
                            this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK][i].drMin,
                            this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK][i].drMax
                          );
  }

  return(true);
}

bool LoRaWANNode::applyChannelMaskFix(uint8_t chMaskCntl, uint16_t chMask, bool clear) {
  RADIOLIB_DEBUG_PRINTLN("mask[%d] = 0x%04x", chMaskCntl, chMask);
  if(clear) {
    for(size_t i = 0; i < RADIOLIB_LORAWAN_NUM_AVAILABLE_CHANNELS; i++) {
      this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][i] = RADIOLIB_LORAWAN_CHANNEL_NONE;
    }
  }
  // find out how many channels have already been configured
  uint8_t idx = 0;
  for(size_t i = 0; i < RADIOLIB_LORAWAN_NUM_AVAILABLE_CHANNELS; i++) {
    if(this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][i].freq > 0) {
      idx++;
    }
  }
  
  if((this->band->numTxSpans == 1 && chMaskCntl <= 5) || (this->band->numTxSpans == 2 && chMaskCntl <= 3)) {
    // select channels from first span
    LoRaWANChannel_t chnl;
    for(uint8_t i = 0; i < 16; i++) {
      uint16_t mask = 1 << i;
      if(mask & chMask) {
        uint8_t chNum = chMaskCntl * 16 + i;  // 0 through 63 or 95
        this->subBand = chNum % 8;            // keep track of configured subband in case we must reset the channels
        chnl.enabled = true;
        chnl.idx   = chNum;
        chnl.freq  = this->band->txSpans[0].freqStart + chNum*this->band->txSpans[0].freqStep;
        chnl.drMin = this->band->txSpans[0].drMin;
        chnl.drMax = this->band->txSpans[0].drMax;
        this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][idx++] = chnl;
        RADIOLIB_DEBUG_PRINTLN("Channel UL %d (%d) frequency = %f MHz", chnl.idx, idx, this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][idx-1].freq);
      }
    }

  }
  if(this->band->numTxSpans == 1 && chMaskCntl == 6) {
    // all channels on (but we revert to user-selected subband)
    this->setupChannelsFix(this->subBand);

  }
  if(this->band->numTxSpans == 2 && chMaskCntl == 4) {
    // select channels from second span
    LoRaWANChannel_t chnl;
    for(uint8_t i = 0; i < 8; i++) {
      uint16_t mask = 1 << i;
      if(mask & chMask) {
        uint8_t chNum = chMaskCntl * 16 + i;  // 64 through 71
        chnl.enabled = true;
        chnl.idx   = chNum;
        chnl.freq  = this->band->txSpans[1].freqStart + i*this->band->txSpans[1].freqStep;
        chnl.drMin = this->band->txSpans[1].drMin;
        chnl.drMax = this->band->txSpans[1].drMax;
        this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][idx++] = chnl;
        RADIOLIB_DEBUG_PRINTLN("Channel UL %d (%d) frequency = %f MHz", chnl.idx, idx-1, this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][idx-1].freq);
      }
    }

  }
  if(this->band->numTxSpans == 2 && chMaskCntl == 5) {
    // a '1' enables a bank of 8 + 1 channels from 1st and 2nd span respectively
    LoRaWANChannel_t chnl;
    for(uint8_t i = 0; i < 8; i++) {
      uint16_t mask = 1 << i;
      if(mask & chMask) {
        // enable bank of 8 channels from first span
        for(uint8_t j = 0; j < 8; i++) {
          uint8_t chNum = i * 8 + j;
          chnl.enabled = true;
          chnl.idx   = chNum;
          chnl.freq  = this->band->txSpans[0].freqStart + chNum*this->band->txSpans[0].freqStep;
          chnl.drMin = this->band->txSpans[0].drMin;
          chnl.drMax = this->band->txSpans[0].drMax;
          this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][idx++] = chnl;
          RADIOLIB_DEBUG_PRINTLN("Channel UL %d (%d) frequency = %f MHz", chnl.idx, idx, this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][idx-1].freq);
        }
        // enable single channel from second span
        uint8_t chNum = 64 + i;
        chnl.enabled = true;
        chnl.idx   = chNum;
        chnl.freq  = this->band->txSpans[1].freqStart + i*this->band->txSpans[1].freqStep;
        chnl.drMin = this->band->txSpans[1].drMin;
        chnl.drMax = this->band->txSpans[1].drMax;
        this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][idx++] = chnl;
        RADIOLIB_DEBUG_PRINTLN("Channel UL %d (%d) frequency = %f MHz", chnl.idx, idx, this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][idx-1].freq);
      }
    }

  }
  if(this->band->numTxSpans == 2 && chMaskCntl == 6) {
    // all channels on (but we revert to selected subband)
    if(this->subBand >= 0) {
      this->setupChannelsFix(this->subBand);
    }
    // a '1' enables a single channel from second span
    LoRaWANChannel_t chnl;
    for(uint8_t i = 0; i < 8; i++) {
      uint16_t mask = 1 << i;
      if(mask & chMask) {
        // enable single channel from second span
        uint8_t chNum = 64 + i;
        chnl.enabled = true;
        chnl.idx   = chNum;
        chnl.freq  = this->band->txSpans[1].freqStart + i*this->band->txSpans[1].freqStep;
        chnl.drMin = this->band->txSpans[1].drMin;
        chnl.drMax = this->band->txSpans[1].drMax;
        this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][idx++] = chnl;
        RADIOLIB_DEBUG_PRINTLN("Channel UL %d (%d) frequency = %f MHz", chnl.idx, idx, this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][idx-1].freq);
      }
    }

  }
  if(this->band->numTxSpans == 2 && chMaskCntl == 7) {
    // all channels off (clear all channels)
    LoRaWANChannel_t chnl = RADIOLIB_LORAWAN_CHANNEL_NONE;
    for(int i = 0; i < RADIOLIB_LORAWAN_NUM_AVAILABLE_CHANNELS; i++) {
      this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][i] = chnl;
      // downlink channels are not defined so don't need to reset
    }
    idx = 0;
    // a '1' enables a single channel from second span
    for(uint8_t i = 0; i < 8; i++) {
      uint16_t mask = 1 << i;
      if(mask & chMask) {
        // enable single channel from second span
        uint8_t chNum = 64 + i;
        chnl.enabled = true;
        chnl.idx   = chNum;
        chnl.freq  = this->band->txSpans[1].freqStart + i*this->band->txSpans[1].freqStep;
        chnl.drMin = this->band->txSpans[1].drMin;
        chnl.drMax = this->band->txSpans[1].drMax;
        this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][idx++] = chnl;
        RADIOLIB_DEBUG_PRINTLN("Channel UL %d (%d) frequency = %f MHz", chnl.idx, idx, this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][idx-1].freq);
      }
    }

  }

  for (int i = 0; i < RADIOLIB_LORAWAN_NUM_AVAILABLE_CHANNELS; i++) {
    RADIOLIB_DEBUG_PRINTLN("UL: %d %d %5.2f (%d - %d) | DL: %d %d %5.2f (%d - %d)",
                            this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][i].idx,
                            this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][i].enabled,
                            this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][i].freq,
                            this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][i].drMin,
                            this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK][i].drMax,

                            this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK][i].idx,
                            this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK][i].enabled,
                            this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK][i].freq,
                            this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK][i].drMin,
                            this->availableChannels[RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK][i].drMax
                          );
  }

  return(true);
}

uint8_t LoRaWANNode::getMacPayloadLength(uint8_t cid) {
  for (LoRaWANMacSpec_t entry : MacTable) {
    if (entry.cid == cid) {
      return entry.lenDn;
    }
  }
  // no idea about the length
  return 0;
}

int16_t LoRaWANNode::getMacLinkCheckAns(uint8_t* margin, uint8_t* gwCnt) {
  uint8_t payload[5];
  int16_t state = deleteMacCommand(RADIOLIB_LORAWAN_LINK_CHECK_REQ, &this->commandsDown, payload);
  RADIOLIB_ASSERT(state);

  if(margin) { *margin = payload[0]; }
  if(gwCnt)  { *gwCnt  = payload[1]; }
  // RADIOLIB_DEBUG_PRINTLN("Link check: margin = %d dB, gwCnt = %d", margin, gwCnt);
  return(RADIOLIB_ERR_NONE);
}

int16_t LoRaWANNode::getMacDeviceTimeAns(uint32_t* gpsEpoch, uint8_t* fraction, bool returnUnix) {
  uint8_t payload[5];
  int16_t state = deleteMacCommand(RADIOLIB_LORAWAN_MAC_DEVICE_TIME, &this->commandsDown, payload);
  RADIOLIB_ASSERT(state);

  if(gpsEpoch) { 
    *gpsEpoch = LoRaWANNode::ntoh<uint32_t>(&payload[0]); 
    if(returnUnix) {
      uint32_t unixOffset = 315964800;
      *gpsEpoch += unixOffset;
    }
  }
  if(fraction) { *fraction = payload[4]; }
  // RADIOLIB_DEBUG_PRINTLN("Network time: gpsEpoch = %d s, delayExp = %f", gpsEpoch, (float)(*fraction)/256.0f);
  return(RADIOLIB_ERR_NONE);
}

uint64_t LoRaWANNode::getDevAddr() {
  return(this->devAddr);
}

// The following function enables LMAC, a CSMA scheme for LoRa as specified 
// in the LoRa Alliance Technical Recommendation #13.
// A user may enable CSMA to provide frames an additional layer of protection from interference.
// https://resources.lora-alliance.org/technical-recommendations/tr013-1-0-0-csma
void LoRaWANNode::performCSMA() {
    
    // Compute initial random back-off. 
    // When BO is reduced to zero, the function returns and the frame is transmitted.
    uint32_t BO = this->phyLayer->random(1, this->backoffMax + 1);
    while (BO > 0) {
        // DIFS: Check channel for DIFS_slots
        bool channelFreeDuringDIFS = true;
        for (uint8_t i = 0; i < this->difsSlots; i++) {
            if (performCAD()) {
                RADIOLIB_DEBUG_PRINTLN("OCCUPIED CHANNEL DURING DIFS");
                channelFreeDuringDIFS = false;
                // Channel is occupied during DIFS, hop to another.
                this->selectChannels();
                break;
            }
        }
        // Start reducing BO counter if DIFS slot was free.
        if (channelFreeDuringDIFS) {
            // Continue decrementing BO with per each CAD reporting free channel.
            while (BO > 0) {
                if (performCAD()) {
                    RADIOLIB_DEBUG_PRINTLN("OCCUPIED CHANNEL DURING BO");
                    // Channel is busy during CAD, hop to another and return to DIFS state again.
                    this->selectChannels();
                    break;  // Exit loop. Go back to DIFS state.
                }
                BO--;  // Decrement BO by one if channel is free
            }
        }
    }
}
bool LoRaWANNode::performCAD() {
    int16_t state = this->phyLayer->scanChannel();
    if ((state == RADIOLIB_PREAMBLE_DETECTED) || (state == RADIOLIB_LORA_DETECTED)) {
        return true; // Channel is busy
    }
    return false; // Channel is free
}

void LoRaWANNode::processAES(uint8_t* in, size_t len, uint8_t* key, uint8_t* out, uint32_t fcnt, uint8_t dir, uint8_t ctrId, bool counter) {
  // figure out how many encryption blocks are there
  size_t numBlocks = len/RADIOLIB_AES128_BLOCK_SIZE;
  if(len % RADIOLIB_AES128_BLOCK_SIZE) {
    numBlocks++;
  }

  // generate the encryption blocks
  uint8_t encBuffer[RADIOLIB_AES128_BLOCK_SIZE] = { 0 };
  uint8_t encBlock[RADIOLIB_AES128_BLOCK_SIZE] = { 0 };
  encBlock[RADIOLIB_LORAWAN_BLOCK_MAGIC_POS] = RADIOLIB_LORAWAN_ENC_BLOCK_MAGIC;
  encBlock[RADIOLIB_LORAWAN_ENC_BLOCK_COUNTER_ID_POS] = ctrId;
  encBlock[RADIOLIB_LORAWAN_BLOCK_DIR_POS] = dir;
  LoRaWANNode::hton<uint32_t>(&encBlock[RADIOLIB_LORAWAN_BLOCK_DEV_ADDR_POS], this->devAddr);
  LoRaWANNode::hton<uint32_t>(&encBlock[RADIOLIB_LORAWAN_BLOCK_FCNT_POS], fcnt);

  // now encrypt the input
  // on downlink frames, this has a decryption effect because server actually "decrypts" the plaintext
  size_t remLen = len;
  for(size_t i = 0; i < numBlocks; i++) {

    if(counter) {
      encBlock[RADIOLIB_LORAWAN_ENC_BLOCK_COUNTER_POS] = i + 1;
    }

    // encrypt the buffer
    RadioLibAES128Instance.init(key);
    RadioLibAES128Instance.encryptECB(encBlock, RADIOLIB_AES128_BLOCK_SIZE, encBuffer);

    // now xor the buffer with the input
    size_t xorLen = remLen;
    if(xorLen > RADIOLIB_AES128_BLOCK_SIZE) {
      xorLen = RADIOLIB_AES128_BLOCK_SIZE;
    }
    for(uint8_t j = 0; j < xorLen; j++) {
      out[i*RADIOLIB_AES128_BLOCK_SIZE + j] = in[i*RADIOLIB_AES128_BLOCK_SIZE + j] ^ encBuffer[j];
    }
    remLen -= xorLen;
  }
}

uint16_t LoRaWANNode::checkSum16(uint8_t *key, uint8_t keyLen) {
  if(keyLen > RADIOLIB_AES128_KEY_SIZE / 2) {
    keyLen = RADIOLIB_AES128_KEY_SIZE / 2;
  }
  uint16_t buf16[RADIOLIB_AES128_KEY_SIZE / 2] = { 0 };
  uint8_t bufLen = keyLen / 2;
  memcpy(buf16, key, keyLen);
  uint16_t checkSum = 0;
  for(int i = 0; i < bufLen; i++) {
    checkSum ^= buf16[i];
  }
  return(checkSum);
}

template<typename T>
T LoRaWANNode::ntoh(uint8_t* buff, size_t size) {
  uint8_t* buffPtr = buff;
  size_t targetSize = sizeof(T);
  if(size != 0) {
    targetSize = size;
  }
  T res = 0;
  for(size_t i = 0; i < targetSize; i++) {
    res |= (uint32_t)(*(buffPtr++)) << 8*i;
  }
  return(res);
}

template<typename T>
void LoRaWANNode::hton(uint8_t* buff, T val, size_t size) {
  uint8_t* buffPtr = buff;
  size_t targetSize = sizeof(T);
  if(size != 0) {
    targetSize = size;
  }
  for(size_t i = 0; i < targetSize; i++) {
    *(buffPtr++) = val >> 8*i;
  }
}

#endif
