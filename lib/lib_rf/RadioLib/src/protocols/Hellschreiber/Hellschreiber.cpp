#include "Hellschreiber.h"

#if !RADIOLIB_EXCLUDE_HELLSCHREIBER

HellClient::HellClient(PhysicalLayer* phy) {
  phyLayer = phy;
  lineFeed = " ";
  #if !RADIOLIB_EXCLUDE_AFSK
  audioClient = nullptr;
  #endif
}

#if !RADIOLIB_EXCLUDE_AFSK
HellClient::HellClient(AFSKClient* audio) {
  phyLayer = audio->phyLayer;
  lineFeed = " ";
  audioClient = audio;
}
#endif

int16_t HellClient::begin(float base, float rate) {
  // calculate 24-bit frequency
  baseFreqHz = base;
  baseFreq = (base * 1000000.0) / phyLayer->getFreqStep();

  // calculate "pixel" duration
  pixelDuration = 1000000.0/rate;

  // configure for direct mode
  return(phyLayer->startDirect());
}

size_t HellClient::printGlyph(uint8_t* buff) {
  // print the character
  Module* mod = phyLayer->getMod();
  bool transmitting = false;
  for(uint8_t mask = 0x40; mask >= 0x01; mask >>= 1) {
    for(int8_t i = RADIOLIB_HELL_FONT_HEIGHT - 1; i >= 0; i--) {
        uint32_t start = mod->hal->micros();
        if((buff[i] & mask) && (!transmitting)) {
          transmitting = true;
          transmitDirect(baseFreq, baseFreqHz);
        } else if((!(buff[i] & mask)) && (transmitting)) {
          transmitting = false;
          standby();
        }
        mod->waitForMicroseconds(start, pixelDuration);
    }
  }

  // make sure transmitter is off
  standby();

  return(1);
}

void HellClient::setInversion(bool inv) {
  invert = inv;
}

size_t HellClient::write(uint8_t b) {
  // convert to position in font buffer
  uint8_t pos = b;
  if((pos >= ' ') && (pos <= '_')) {
    pos -= ' ';
  } else if((pos >= 'a') && (pos <= 'z')) {
    pos -= (2*' ');
  } else {
    return(0);
  }

  // fetch character from flash
  uint8_t buff[RADIOLIB_HELL_FONT_WIDTH];
  buff[0] = 0x00;
  for(uint8_t i = 0; i < RADIOLIB_HELL_FONT_WIDTH - 2; i++) {
    buff[i + 1] = RADIOLIB_NONVOLATILE_READ_BYTE(&HellFont[pos][i]);
  }
  buff[RADIOLIB_HELL_FONT_WIDTH - 1] = 0x00;

  // print the character
  return(printGlyph(buff));
}

int16_t HellClient::transmitDirect(uint32_t freq, uint32_t freqHz) {
  #if !RADIOLIB_EXCLUDE_AFSK
  if(audioClient != nullptr) {
    return(audioClient->tone(freqHz));
  }
  #endif
  return(phyLayer->transmitDirect(freq));
}

int16_t HellClient::standby() {
  #if !RADIOLIB_EXCLUDE_AFSK
  if(audioClient != nullptr) {
    return(audioClient->noTone(invert));
  }
  #endif
  return(phyLayer->standby(RADIOLIB_STANDBY_WARM));
}

#endif
