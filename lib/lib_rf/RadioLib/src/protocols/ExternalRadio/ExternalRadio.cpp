#include "ExternalRadio.h"

#if defined(RADIOLIB_BUILD_ARDUINO)
ExternalRadio::ExternalRadio(uint32_t pin) : PhysicalLayer(1, 0) {
  mod = new Module(RADIOLIB_NC, RADIOLIB_NC, RADIOLIB_NC, pin);
  mod->hal->pinMode(pin, mod->hal->GpioModeOutput);
  this->prevFrf = 0;
}
#endif

ExternalRadio::ExternalRadio(RadioLibHal *hal, uint32_t pin) : PhysicalLayer(1, 0) {
  mod = new Module(hal, RADIOLIB_NC, RADIOLIB_NC, RADIOLIB_NC, pin);
  mod->hal->pinMode(pin, mod->hal->GpioModeOutput);
  this->prevFrf = 0;
}

Module* ExternalRadio::getMod() {
  return(mod);
}

int16_t ExternalRadio::setFrequencyDeviation(float freqDev) {
  (void)freqDev;
  return(RADIOLIB_ERR_NONE);
}

int16_t ExternalRadio::setDataShaping(uint8_t sh) {
  (void)sh;
  return(RADIOLIB_ERR_NONE);
}

int16_t ExternalRadio::setEncoding(uint8_t encoding) {
  (void)encoding;
  return(RADIOLIB_ERR_NONE);
}

int16_t ExternalRadio::transmitDirect(uint32_t frf) {
  if(frf != this->prevFrf) {
    uint32_t val = this->mod->hal->GpioLevelLow;
    if(frf > this->prevFrf) {
      val = this->mod->hal->GpioLevelHigh;
    }
    this->prevFrf = frf;
    this->mod->hal->digitalWrite(this->mod->getGpio(), val);
  }
  
  return(RADIOLIB_ERR_NONE);
}