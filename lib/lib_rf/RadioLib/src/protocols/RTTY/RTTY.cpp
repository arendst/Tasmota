#include "RTTY.h"

#include <math.h>

#if !RADIOLIB_EXCLUDE_RTTY

RTTYClient::RTTYClient(PhysicalLayer* phy) {
  phyLayer = phy;
  lineFeed = "\r\n";
  #if !RADIOLIB_EXCLUDE_AFSK
  audioClient = nullptr;
  #endif
}

#if !RADIOLIB_EXCLUDE_AFSK
RTTYClient::RTTYClient(AFSKClient* audio) {
  phyLayer = audio->phyLayer;
  lineFeed = "\r\n";
  audioClient = audio;
}
#endif

int16_t RTTYClient::begin(float base, uint32_t shift, uint16_t rate, uint8_t enc, uint8_t stopBits) {
  // save configuration
  RadioLibPrint::encoding = enc;
  stopBitsNum = stopBits;
  baseFreqHz = base;
  shiftFreqHz = shift;

  // calculate duration of 1 bit
  bitDuration = (uint32_t)1000000/rate;

  // calculate module carrier frequency resolution
  uint32_t step = round(phyLayer->getFreqStep());

  // check minimum shift value
  if(shift < step / 2) {
    return(RADIOLIB_ERR_INVALID_RTTY_SHIFT);
  }

  // round shift to multiples of frequency step size
  if(shift % step < (step / 2)) {
    shiftFreq = shift / step;
  } else {
    shiftFreq = (shift / step) + 1;
  }

  // calculate 24-bit frequency
  baseFreq = (base * 1000000.0) / phyLayer->getFreqStep();

  // configure for direct mode
  return(phyLayer->startDirect());
}

void RTTYClient::idle() {
  mark();
}

size_t RTTYClient::write(uint8_t b) {
  uint8_t dataBitsNum = 0;
  switch(RadioLibPrint::encoding) {
    case RADIOLIB_ASCII:
      dataBitsNum = 7;
      break;
    case RADIOLIB_ASCII_EXTENDED:
      dataBitsNum = 8;
      break;
    case RADIOLIB_ITA2:
      dataBitsNum = 5;
      break;
    default:
      return(0);
  }
  space();

  uint16_t maxDataMask = 0x01 << (dataBitsNum - 1);
  for(uint16_t mask = 0x01; mask <= maxDataMask; mask <<= 1) {
    if(b & mask) {
      mark();
    } else {
      space();
    }
  }

  for(uint8_t i = 0; i < stopBitsNum; i++) {
    mark();
  }

  return(1);
}

void RTTYClient::mark() {
  Module* mod = phyLayer->getMod();
  uint32_t start = mod->hal->micros();
  transmitDirect(baseFreq + shiftFreq, baseFreqHz + shiftFreqHz);
  mod->waitForMicroseconds(start, bitDuration);
}

void RTTYClient::space() {
  Module* mod = phyLayer->getMod();
  uint32_t start = mod->hal->micros();
  transmitDirect(baseFreq, baseFreqHz);
  mod->waitForMicroseconds(start, bitDuration);
}

int16_t RTTYClient::transmitDirect(uint32_t freq, uint32_t freqHz) {
  #if !RADIOLIB_EXCLUDE_AFSK
  if(audioClient != nullptr) {
    return(audioClient->tone(freqHz));
  }
  #endif
  return(phyLayer->transmitDirect(freq));
}

int16_t RTTYClient::standby() {
  // ensure everything is stopped in interrupt timing mode
  Module* mod = phyLayer->getMod();
  mod->waitForMicroseconds(0, 0);
  #if !RADIOLIB_EXCLUDE_AFSK
  if(audioClient != nullptr) {
    return(audioClient->noTone());
  }
  #endif
  return(phyLayer->standby());
}

#endif
