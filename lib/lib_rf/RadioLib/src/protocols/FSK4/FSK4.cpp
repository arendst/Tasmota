#include "FSK4.h"
#include <math.h>
#if !RADIOLIB_EXCLUDE_FSK4

FSK4Client::FSK4Client(PhysicalLayer* phy) {
  phyLayer = phy;
  #if !RADIOLIB_EXCLUDE_AFSK
  audioClient = nullptr;
  #endif
}

#if !RADIOLIB_EXCLUDE_AFSK
 FSK4Client::FSK4Client(AFSKClient* audio) {
   phyLayer = audio->phyLayer;
   audioClient = audio;
 }
#endif

int16_t FSK4Client::begin(float base, uint32_t shift, uint16_t rate) {
  // save configuration
  baseFreqHz = base;
  shiftFreqHz = shift;

  // calculate duration of 1 bit
  bitDuration = (uint32_t)1000000/rate;

  // calculate carrier shift
  shiftFreq = getRawShift(shift);

  // Write resultant tones into arrays for quick lookup when modulating.
  for(uint8_t i = 0; i < 4; i++) {
    tones[i] = shiftFreq*i;
    tonesHz[i] = shiftFreqHz*i;
  }

  // calculate 24-bit frequency
  baseFreq = (base * 1000000.0) / phyLayer->getFreqStep();

  // configure for direct mode
  return(phyLayer->startDirect());
}

void FSK4Client::idle() {
  // Idle at Tone 0.
  tone(0);
}

int16_t FSK4Client::setCorrection(int16_t offsets[], float length) {
  for(uint8_t i = 0; i < 4; i++) {
    tones[i] += getRawShift(offsets[i]);
    tonesHz[i] += offsets[i];
  }
  bitDuration *= length;
  return(RADIOLIB_ERR_NONE);
}

size_t FSK4Client::write(uint8_t* buff, size_t len) {
  size_t n = 0;
  for(size_t i = 0; i < len; i++) {
    n += FSK4Client::write(buff[i]);
  }
  FSK4Client::standby();
  return(n);
}

size_t FSK4Client::write(uint8_t b) {
  // send symbols MSB first
  for(uint8_t i = 0; i < 4; i++) {
    // Extract 4FSK symbol (2 bits)
    uint8_t symbol = (b & 0xC0) >> 6;

    // Modulate
    FSK4Client::tone(symbol);

    // Shift to next symbol
    b = b << 2;
  }

  return(1);
}

void FSK4Client::tone(uint8_t i) {
  Module* mod = phyLayer->getMod();
  uint32_t start = mod->hal->micros();
  transmitDirect(baseFreq + tones[i], baseFreqHz + tonesHz[i]);
  mod->waitForMicroseconds(start, bitDuration);
}

int16_t FSK4Client::transmitDirect(uint32_t freq, uint32_t freqHz) {
  #if !RADIOLIB_EXCLUDE_AFSK
  if(audioClient != nullptr) {
    return(audioClient->tone(freqHz));
  }
  #endif
  return(phyLayer->transmitDirect(freq));
}

int16_t FSK4Client::standby() {
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

int32_t FSK4Client::getRawShift(int32_t shift) {
  // calculate module carrier frequency resolution
  int32_t step = round(phyLayer->getFreqStep());

  // check minimum shift value
  if(abs(shift) < step / 2) {
    return(0);
  }

  // round shift to multiples of frequency step size
  if(abs(shift) % step < (step / 2)) {
    return(shift / step);
  }
  if(shift < 0) {
    return((shift / step) - 1);
  }
  return((shift / step) + 1);
}

#endif
