#include "ArduinoHal.h"

#if defined(RADIOLIB_BUILD_ARDUINO)

ArduinoHal::ArduinoHal(): RadioLibHal(INPUT, OUTPUT, LOW, HIGH, RISING, FALLING), spi(&RADIOLIB_DEFAULT_SPI), initInterface(true) {}

ArduinoHal::ArduinoHal(SPIClass& spi, SPISettings spiSettings): RadioLibHal(INPUT, OUTPUT, LOW, HIGH, RISING, FALLING), spi(&spi), spiSettings(spiSettings) {}

void ArduinoHal::init() {
  if(initInterface) {
    spiBegin();
  }
}

void ArduinoHal::term() {
  if(initInterface) {
    spiEnd();
  }
}

void inline ArduinoHal::pinMode(uint32_t pin, uint32_t mode) {
  if(pin == RADIOLIB_NC) {
    return;
  }
  ::pinMode(pin, RADIOLIB_ARDUINOHAL_PIN_MODE_CAST mode);
}

void inline ArduinoHal::digitalWrite(uint32_t pin, uint32_t value) {
  if(pin == RADIOLIB_NC) {
    return;
  }
  ::digitalWrite(pin, RADIOLIB_ARDUINOHAL_PIN_STATUS_CAST value);
}

uint32_t inline ArduinoHal::digitalRead(uint32_t pin) {
  if(pin == RADIOLIB_NC) {
    return 0;
  }
  return(::digitalRead(pin));
}

void inline ArduinoHal::attachInterrupt(uint32_t interruptNum, void (*interruptCb)(void), uint32_t mode) {
  if(interruptNum == RADIOLIB_NC) {
    return;
  }
  ::attachInterrupt(interruptNum, interruptCb,  RADIOLIB_ARDUINOHAL_INTERRUPT_MODE_CAST mode);
}

void inline ArduinoHal::detachInterrupt(uint32_t interruptNum) {
  if(interruptNum == RADIOLIB_NC) {
    return;
  }
  ::detachInterrupt(interruptNum);
}

void inline ArduinoHal::delay(unsigned long ms) {
#if !defined(RADIOLIB_CLOCK_DRIFT_MS)
  ::delay(ms);
#else
  ::delay(ms * 1000 / (1000 + RADIOLIB_CLOCK_DRIFT_MS));
#endif
}

void inline ArduinoHal::delayMicroseconds(unsigned long us) {
#if !defined(RADIOLIB_CLOCK_DRIFT_MS)
  ::delayMicroseconds(us);
#else
  ::delayMicroseconds(us * 1000 / (1000 + RADIOLIB_CLOCK_DRIFT_MS));
#endif
}

unsigned long inline ArduinoHal::millis() {
#if !defined(RADIOLIB_CLOCK_DRIFT_MS)
  return(::millis());
#else
  return(::millis() * 1000 / (1000 + RADIOLIB_CLOCK_DRIFT_MS));
#endif
}

unsigned long inline ArduinoHal::micros() {
#if !defined(RADIOLIB_CLOCK_DRIFT_MS)
  return(::micros());
#else
  return(::micros() * 1000 / (1000 + RADIOLIB_CLOCK_DRIFT_MS));
#endif
}

long inline ArduinoHal::pulseIn(uint32_t pin, uint32_t state, unsigned long timeout) {
  if(pin == RADIOLIB_NC) {
    return 0;
  }
  return(::pulseIn(pin, state, timeout));
}

void inline ArduinoHal::spiBegin() {
  spi->begin();
}

void inline ArduinoHal::spiBeginTransaction() {
  spi->beginTransaction(spiSettings);
}

void ArduinoHal::spiTransfer(uint8_t* out, size_t len, uint8_t* in) {
  for(size_t i = 0; i < len; i++) {
    in[i] = spi->transfer(out[i]);
  }
}

void inline ArduinoHal::spiEndTransaction() {
  spi->endTransaction();
}

void inline ArduinoHal::spiEnd() {
  spi->end();
}

void inline ArduinoHal::tone(uint32_t pin, unsigned int frequency, unsigned long duration) {
  #if !defined(RADIOLIB_TONE_UNSUPPORTED)
    if(pin == RADIOLIB_NC) {
      return;
    }
    ::tone(pin, frequency, duration);
  #elif defined(RADIOLIB_ESP32)
    // ESP32 tone() emulation
    (void)duration;
    if(prev == -1) {
      #if !defined(ESP_IDF_VERSION) || (ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5,0,0))
      ledcAttachPin(pin, RADIOLIB_TONE_ESP32_CHANNEL);
      #endif
    }
    if(prev != frequency) {
      #if !defined(ESP_IDF_VERSION) || (ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5,0,0))
      ledcWriteTone(RADIOLIB_TONE_ESP32_CHANNEL, frequency);
      #else
      ledcWriteTone(pin, frequency);
      #endif
    }
    prev = frequency;
  #elif defined(RADIOLIB_MBED_TONE_OVERRIDE)
    // better tone for mbed OS boards
    (void)duration;
    if(!pwmPin) {
      pwmPin = new mbed::PwmOut(digitalPinToPinName(pin));
    }
    pwmPin->period(1.0 / frequency);
    pwmPin->write(0.5);
  #else
    (void)pin;
    (void)frequency;
    (void)duration;
  #endif
}

void inline ArduinoHal::noTone(uint32_t pin) {
  #if !defined(RADIOLIB_TONE_UNSUPPORTED) and defined(ARDUINO_ARCH_STM32)
    if(pin == RADIOLIB_NC) {
      return;
    }
    ::noTone(pin, false);
  #elif !defined(RADIOLIB_TONE_UNSUPPORTED)
    if(pin == RADIOLIB_NC) {
      return;
    }
    ::noTone(pin);
  #elif defined(RADIOLIB_ESP32)
    if(pin == RADIOLIB_NC) {
      return;
    }
    // ESP32 tone() emulation
    #if !defined(ESP_IDF_VERSION) || (ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5,0,0))
    ledcDetachPin(pin);
    ledcWrite(RADIOLIB_TONE_ESP32_CHANNEL, 0);
    #else
    ledcDetach(pin);
    ledcWrite(pin, 0);
    #endif
    prev = -1;
  #elif defined(RADIOLIB_MBED_TONE_OVERRIDE)
    if(pin == RADIOLIB_NC) {
      return;
    }
    // better tone for mbed OS boards
    (void)pin;
    pwmPin->suspend();
  #else
    (void)pin;
  #endif
}

void inline ArduinoHal::yield() {
  #if !defined(RADIOLIB_YIELD_UNSUPPORTED)
//  ::yield();
  ::delay(0);
  #endif
}

uint32_t inline ArduinoHal::pinToInterrupt(uint32_t pin) {
  return(digitalPinToInterrupt(pin));
}

#endif
