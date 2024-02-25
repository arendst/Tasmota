#include "BellModem.h"
#if !RADIOLIB_EXCLUDE_BELL

const struct BellModem_t Bell101 {
  .freqMark = 1270,
  .freqSpace = 1070,
  .baudRate = 110,
  .freqMarkReply = 2225,
  .freqSpaceReply = 2025,
};

const struct BellModem_t Bell103 {
  .freqMark = 1270,
  .freqSpace = 1070,
  .baudRate = 300,
  .freqMarkReply = 2225,
  .freqSpaceReply = 2025,
};

const struct BellModem_t Bell202 {
  .freqMark = 1200,
  .freqSpace = 2200,
  .baudRate = 1200,
  .freqMarkReply = 1200,
  .freqSpaceReply = 2200,
};

BellClient::BellClient(PhysicalLayer* phy, uint32_t pin) : AFSKClient(phy, pin) {
  this->reply = false;
}

BellClient::BellClient(AFSKClient* aud) : AFSKClient(aud) {
  this->reply = false;
}

int16_t BellClient::begin(const BellModem_t& modem) {
  int16_t state = setModem(modem);
  RADIOLIB_ASSERT(state);

  state = phyLayer->startDirect();
  return(state);
}

int16_t BellClient::setModem(const BellModem_t& modem) {
  this->modemType = modem;
  this->toneLen = (1000000.0/(float)this->modemType.baudRate)*this->correction;
  return(RADIOLIB_ERR_NONE);
}

int16_t BellClient::setCorrection(float corr) {
  this->correction = corr;
  return(RADIOLIB_ERR_NONE);
}

size_t BellClient::write(uint8_t b) {
  // first get the frequencies
  uint16_t toneMark = this->modemType.freqMark;
  uint16_t toneSpace = this->modemType.freqSpace;
  if(this->reply) {
    toneMark = this->modemType.freqMarkReply;
    toneMark = this->modemType.freqSpaceReply;
  }

  // get the Module pointer to access HAL
  Module* mod = this->phyLayer->getMod();

  if(this->autoStart) {
    phyLayer->transmitDirect();
  }

  // iterate over the bits and set correct frequencies
  for(uint16_t mask = 0x80; mask >= 0x01; mask >>= 1) {
    uint32_t start = mod->hal->micros();
    if(b & mask) {
      this->tone(toneMark, false);
    } else {
      this->tone(toneSpace, false);
    }
    mod->waitForMicroseconds(start, this->toneLen);
  }
  
  if(this->autoStart) {
    phyLayer->standby();
  }
  return(1);
}

int16_t BellClient::idle() {
  this->autoStart = false;
  return(phyLayer->transmitDirect());
}

int16_t BellClient::standby() {
  this->autoStart = true;
  return(phyLayer->standby());
}

#endif
