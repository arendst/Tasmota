#include "SSTV.h"
#if !RADIOLIB_EXCLUDE_SSTV

const SSTVMode_t Scottie1 {
  .visCode = RADIOLIB_SSTV_SCOTTIE_1,
  .width = 320,
  .height = 256,
  .scanPixelLen = 432,
  .numTones = 7,
  .tones = {
    { .type = tone_t::GENERIC,    .len = 1500, .freq = 1500 },
    { .type = tone_t::SCAN_GREEN, .len = 0,    .freq = 0    },
    { .type = tone_t::GENERIC,    .len = 1500, .freq = 1500 },
    { .type = tone_t::SCAN_BLUE,  .len = 0,    .freq = 0    },
    { .type = tone_t::GENERIC,    .len = 9000, .freq = 1200 },
    { .type = tone_t::GENERIC,    .len = 1500, .freq = 1500 },
    { .type = tone_t::SCAN_RED,   .len = 0,    .freq = 0    }
  }
};

const SSTVMode_t Scottie2 {
  .visCode = RADIOLIB_SSTV_SCOTTIE_2,
  .width = 320,
  .height = 256,
  .scanPixelLen = 275,
  .numTones = 7,
  .tones = {
    { .type = tone_t::GENERIC,    .len = 1500, .freq = 1500 },
    { .type = tone_t::SCAN_GREEN, .len = 0,    .freq = 0    },
    { .type = tone_t::GENERIC,    .len = 1500, .freq = 1500 },
    { .type = tone_t::SCAN_BLUE,  .len = 0,    .freq = 0    },
    { .type = tone_t::GENERIC,    .len = 9000, .freq = 1200 },
    { .type = tone_t::GENERIC,    .len = 1500, .freq = 1500 },
    { .type = tone_t::SCAN_RED,   .len = 0,    .freq = 0    }
  }
};

const SSTVMode_t ScottieDX {
  .visCode = RADIOLIB_SSTV_SCOTTIE_DX,
  .width = 320,
  .height = 256,
  .scanPixelLen = 1080,
  .numTones = 7,
  .tones = {
    { .type = tone_t::GENERIC,    .len = 1500, .freq = 1500 },
    { .type = tone_t::SCAN_GREEN, .len = 0,    .freq = 0    },
    { .type = tone_t::GENERIC,    .len = 1500, .freq = 1500 },
    { .type = tone_t::SCAN_BLUE,  .len = 0,    .freq = 0    },
    { .type = tone_t::GENERIC,    .len = 9000, .freq = 1200 },
    { .type = tone_t::GENERIC,    .len = 1500, .freq = 1500 },
    { .type = tone_t::SCAN_RED,   .len = 0,    .freq = 0    }
  }
};

const SSTVMode_t Martin1 {
  .visCode = RADIOLIB_SSTV_MARTIN_1,
  .width = 320,
  .height = 256,
  .scanPixelLen = 458,
  .numTones = 8,
  .tones = {
    { .type = tone_t::GENERIC,    .len = 4862, .freq = 1200 },
    { .type = tone_t::GENERIC,    .len = 572,  .freq = 1500 },
    { .type = tone_t::SCAN_GREEN, .len = 0,    .freq = 0    },
    { .type = tone_t::GENERIC,    .len = 572,  .freq = 1500 },
    { .type = tone_t::SCAN_BLUE,  .len = 0,    .freq = 0    },
    { .type = tone_t::GENERIC,    .len = 572,  .freq = 1500 },
    { .type = tone_t::SCAN_RED,   .len = 0,    .freq = 0    },
    { .type = tone_t::GENERIC,    .len = 572,  .freq = 1500 }
  }
};

const SSTVMode_t Martin2 {
  .visCode = RADIOLIB_SSTV_MARTIN_2,
  .width = 320,
  .height = 256,
  .scanPixelLen = 229,
  .numTones = 8,
  .tones = {
    { .type = tone_t::GENERIC,    .len = 4862, .freq = 1200 },
    { .type = tone_t::GENERIC,    .len = 572,  .freq = 1500 },
    { .type = tone_t::SCAN_GREEN, .len = 0,    .freq = 0    },
    { .type = tone_t::GENERIC,    .len = 572,  .freq = 1500 },
    { .type = tone_t::SCAN_BLUE,  .len = 0,    .freq = 0    },
    { .type = tone_t::GENERIC,    .len = 572,  .freq = 1500 },
    { .type = tone_t::SCAN_RED,   .len = 0,    .freq = 0    },
    { .type = tone_t::GENERIC,    .len = 572,  .freq = 1500 }
  }
};

const SSTVMode_t Wrasse {
  .visCode = RADIOLIB_SSTV_WRASSE_SC2_180,
  .width = 320,
  .height = 256,
  .scanPixelLen = 734,
  .numTones = 5,
  .tones = {
    { .type = tone_t::GENERIC,    .len = 5523, .freq = 1200 },
    { .type = tone_t::GENERIC,    .len = 500,  .freq = 1500 },
    { .type = tone_t::SCAN_RED,   .len = 0,    .freq = 0    },
    { .type = tone_t::SCAN_GREEN, .len = 0,    .freq = 0    },
    { .type = tone_t::SCAN_BLUE,  .len = 0,    .freq = 0    }
  }
};

const SSTVMode_t PasokonP3 {
  .visCode = RADIOLIB_SSTV_PASOKON_P3,
  .width = 640,
  .height = 496,
  .scanPixelLen = 208,
  .numTones = 7,
  .tones = {
    { .type = tone_t::GENERIC,    .len = 5208, .freq = 1200 },
    { .type = tone_t::GENERIC,    .len = 1042, .freq = 1500 },
    { .type = tone_t::SCAN_RED,   .len = 0,    .freq = 0    },
    { .type = tone_t::GENERIC,    .len = 1042, .freq = 1500 },
    { .type = tone_t::SCAN_GREEN, .len = 0,    .freq = 0    },
    { .type = tone_t::GENERIC,    .len = 1042, .freq = 1500 },
    { .type = tone_t::SCAN_BLUE,  .len = 0,    .freq = 0    }
  }
};

const SSTVMode_t PasokonP5 {
  .visCode = RADIOLIB_SSTV_PASOKON_P5,
  .width = 640,
  .height = 496,
  .scanPixelLen = 312,
  .numTones = 7,
  .tones = {
    { .type = tone_t::GENERIC,    .len = 7813, .freq = 1200 },
    { .type = tone_t::GENERIC,    .len = 1563, .freq = 1500 },
    { .type = tone_t::SCAN_RED,   .len = 0,    .freq = 0    },
    { .type = tone_t::GENERIC,    .len = 1563, .freq = 1500 },
    { .type = tone_t::SCAN_GREEN, .len = 0,    .freq = 0    },
    { .type = tone_t::GENERIC,    .len = 1563, .freq = 1500 },
    { .type = tone_t::SCAN_BLUE,  .len = 0,    .freq = 0    }
  }
};

const SSTVMode_t PasokonP7 {
  .visCode = RADIOLIB_SSTV_PASOKON_P7,
  .width = 640,
  .height = 496,
  .scanPixelLen = 417,
  .numTones = 7,
  .tones = {
    { .type = tone_t::GENERIC,    .len = 10417, .freq = 1200 },
    { .type = tone_t::GENERIC,    .len = 2083,  .freq = 1500 },
    { .type = tone_t::SCAN_RED,   .len = 0,     .freq = 0    },
    { .type = tone_t::GENERIC,    .len = 2083,  .freq = 1500 },
    { .type = tone_t::SCAN_GREEN, .len = 0,     .freq = 0    },
    { .type = tone_t::GENERIC,    .len = 2083,  .freq = 1500 },
    { .type = tone_t::SCAN_BLUE,  .len = 0,     .freq = 0    }
  }
};

SSTVClient::SSTVClient(PhysicalLayer* phy) {
  phyLayer = phy;
  #if !RADIOLIB_EXCLUDE_AFSK
  audioClient = nullptr;
  #endif
}

#if !RADIOLIB_EXCLUDE_AFSK
SSTVClient::SSTVClient(AFSKClient* audio) {
  phyLayer = audio->phyLayer;
  audioClient = audio;
}
#endif

#if !RADIOLIB_EXCLUDE_AFSK
int16_t SSTVClient::begin(const SSTVMode_t& mode) {
  if(audioClient == nullptr) {
    // this initialization method can only be used in AFSK mode
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  return(begin(0, mode));
}
#endif

int16_t SSTVClient::begin(float base, const SSTVMode_t& mode) {
  // save mode
  txMode = mode;

  // calculate 24-bit frequency
  baseFreq = (base * 1000000.0) / phyLayer->getFreqStep();

  // configure for direct mode
  return(phyLayer->startDirect());
}

int16_t SSTVClient::setCorrection(float correction) {
  // check if mode is initialized
  if(txMode.visCode == 0) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // apply correction factor to all timings
  txMode.scanPixelLen *= correction;
  for(uint8_t i = 0; i < txMode.numTones; i++) {
    txMode.tones[i].len *= correction;
  }
  return(RADIOLIB_ERR_NONE);
}

void SSTVClient::idle() {
  phyLayer->transmitDirect();
  this->tone(RADIOLIB_SSTV_TONE_LEADER);
}

void SSTVClient::sendHeader() {
  // save first header flag for Scottie modes
  firstLine = true;
  phyLayer->transmitDirect();

  // send the first part of header (leader-break-leader)
  this->tone(RADIOLIB_SSTV_TONE_LEADER, RADIOLIB_SSTV_HEADER_LEADER_LENGTH);
  this->tone(RADIOLIB_SSTV_TONE_BREAK, RADIOLIB_SSTV_HEADER_BREAK_LENGTH);
  this->tone(RADIOLIB_SSTV_TONE_LEADER, RADIOLIB_SSTV_HEADER_LEADER_LENGTH);

  // VIS start bit
  this->tone(RADIOLIB_SSTV_TONE_BREAK, RADIOLIB_SSTV_HEADER_BIT_LENGTH);

  // VIS code
  uint8_t parityCount = 0;
  for(uint8_t mask = 0x01; mask < 0x80; mask <<= 1) {
    if(txMode.visCode & mask) {
      this->tone(RADIOLIB_SSTV_TONE_VIS_1, RADIOLIB_SSTV_HEADER_BIT_LENGTH);
      parityCount++;
    } else {
      this->tone(RADIOLIB_SSTV_TONE_VIS_0, RADIOLIB_SSTV_HEADER_BIT_LENGTH);
    }
  }

  // VIS parity
  if(parityCount % 2 == 0) {
    // even parity
    this->tone(RADIOLIB_SSTV_TONE_VIS_0, RADIOLIB_SSTV_HEADER_BIT_LENGTH);
  } else {
    // odd parity
    this->tone(RADIOLIB_SSTV_TONE_VIS_1, RADIOLIB_SSTV_HEADER_BIT_LENGTH);
  }

  // VIS stop bit
  this->tone(RADIOLIB_SSTV_TONE_BREAK, RADIOLIB_SSTV_HEADER_BIT_LENGTH);
}

void SSTVClient::sendLine(uint32_t* imgLine) {
  // check first line flag in Scottie modes
  if(firstLine && ((txMode.visCode == RADIOLIB_SSTV_SCOTTIE_1) || (txMode.visCode == RADIOLIB_SSTV_SCOTTIE_2) || (txMode.visCode == RADIOLIB_SSTV_SCOTTIE_DX))) {
    firstLine = false;

    // send start sync tone
    this->tone(RADIOLIB_SSTV_TONE_BREAK, 9000);
  }

  // send all tones in sequence
  for(uint8_t i = 0; i < txMode.numTones; i++) {
    if((txMode.tones[i].type == tone_t::GENERIC) && (txMode.tones[i].len > 0)) {
      // sync/porch tones
      this->tone(txMode.tones[i].freq, txMode.tones[i].len);
    } else {
      // scan lines
      for(uint16_t j = 0; j < txMode.width; j++) {
        uint32_t color = imgLine[j];
        switch(txMode.tones[i].type) {
          case(tone_t::SCAN_RED):
            color &= 0x00FF0000;
            color >>= 16;
            break;
          case(tone_t::SCAN_GREEN):
            color &= 0x0000FF00;
            color >>= 8;
            break;
          case(tone_t::SCAN_BLUE):
            color &= 0x000000FF;
            break;
          case(tone_t::GENERIC):
            break;
        }
        this->tone(RADIOLIB_SSTV_TONE_BRIGHTNESS_MIN + ((float)color * 3.1372549), txMode.scanPixelLen);
      }
    }
  }
}

uint16_t SSTVClient::getPictureHeight() const {
  return(txMode.height);
}

void SSTVClient::tone(float freq, uint32_t len) {
  Module* mod = phyLayer->getMod();
  uint32_t start = mod->hal->micros();
  #if !RADIOLIB_EXCLUDE_AFSK
  if(audioClient != nullptr) {
    audioClient->tone(freq, false);
  } else {
    phyLayer->transmitDirect(baseFreq + (freq / phyLayer->getFreqStep()));
  }
  #else
  phyLayer->transmitDirect(baseFreq + (freq / phyLayer->getFreqStep()));
  #endif
  mod->waitForMicroseconds(start, len);
}

#endif
