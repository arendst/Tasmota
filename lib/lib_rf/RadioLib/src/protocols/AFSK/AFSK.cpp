#include "AFSK.h"
#if !RADIOLIB_EXCLUDE_AFSK

AFSKClient::AFSKClient(PhysicalLayer* phy, uint32_t pin): outPin(pin) {
  phyLayer = phy;
}

AFSKClient::AFSKClient(AFSKClient* aud) {
  phyLayer = aud->phyLayer;
  outPin = aud->outPin;
}

int16_t AFSKClient::begin() {
  return(phyLayer->startDirect());
}

int16_t AFSKClient::tone(uint16_t freq, bool autoStart) {
  if(freq == 0) {
    return(RADIOLIB_ERR_INVALID_FREQUENCY);
  }

  if(autoStart) {
    int16_t state = phyLayer->transmitDirect();
    RADIOLIB_ASSERT(state);
  }

  Module* mod = phyLayer->getMod();
  mod->hal->tone(outPin, freq);
  return(RADIOLIB_ERR_NONE);
}

int16_t AFSKClient::noTone(bool keepOn) {
  Module* mod = phyLayer->getMod();
  mod->hal->noTone(outPin);
  if(keepOn) {
    return(0);
  }

  return(phyLayer->standby());
}

#endif
