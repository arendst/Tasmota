#include "Hal.h"

RadioLibHal::RadioLibHal(const uint32_t input, const uint32_t output, const uint32_t low, const uint32_t high, const uint32_t rising, const uint32_t falling)
    : GpioModeInput(input),
      GpioModeOutput(output),
      GpioLevelLow(low),
      GpioLevelHigh(high),
      GpioInterruptRising(rising),
      GpioInterruptFalling(falling) {}

void RadioLibHal::init() {

}

void RadioLibHal::term() {

}

void RadioLibHal::tone(uint32_t pin, unsigned int frequency, unsigned long duration) {
  (void)pin;
  (void)frequency;
  (void)duration;
}

void RadioLibHal::noTone(uint32_t pin) {
  (void)pin;
}

void RadioLibHal::yield() {

}

uint32_t RadioLibHal::pinToInterrupt(uint32_t pin) {
  return(pin);
}

void RadioLibHal::readPersistentStorage(uint32_t addr, uint8_t* buff, size_t len) {
  // these are only needed for some protocols, so it's not needed to have them by default
  (void)addr;
  (void)buff;
  (void)len;
}

void RadioLibHal::writePersistentStorage(uint32_t addr, uint8_t* buff, size_t len) {
  // these are only needed for some protocols, so it's not needed to have them by default
  (void)addr;
  (void)buff;
  (void)len;
}

void RadioLibHal::wipePersistentStorage() {
  uint8_t dummy = 0;
  for(size_t i = 0; i < RADIOLIB_HAL_PERSISTENT_STORAGE_SIZE; i++) {
    this->writePersistentStorage(RADIOLIB_HAL_PERSISTENT_STORAGE_BASE + i, &dummy, sizeof(uint8_t));
  }
}

uint32_t RadioLibHal::getPersistentAddr(uint32_t id) {
  return(RadioLibPersistentParamTable[id]);
}

template<typename T>
void RadioLibHal::setPersistentParameter(uint32_t id, T val, uint32_t offset) {
  uint8_t *ptr = (uint8_t*)&val;
  this->writePersistentStorage(RADIOLIB_HAL_PERSISTENT_STORAGE_BASE + RadioLibPersistentParamTable[id] + offset, ptr, sizeof(T));
}

template void RadioLibHal::setPersistentParameter(uint32_t id, uint8_t val, uint32_t offset);
template void RadioLibHal::setPersistentParameter(uint32_t id, uint16_t val, uint32_t offset);
template void RadioLibHal::setPersistentParameter(uint32_t id, uint32_t val, uint32_t offset);

template<typename T>
T RadioLibHal::getPersistentParameter(uint32_t id) {
  T val = 0;
  uint8_t *ptr = (uint8_t*)&val;
  this->readPersistentStorage(RADIOLIB_HAL_PERSISTENT_STORAGE_BASE + RadioLibPersistentParamTable[id], ptr, sizeof(T));
  return(val);
}

template uint8_t RadioLibHal::getPersistentParameter(uint32_t id);
template uint16_t RadioLibHal::getPersistentParameter(uint32_t id);
template uint32_t RadioLibHal::getPersistentParameter(uint32_t id);
