// make sure this is always compiled
#include "TypeDef.h"

#if !defined(_RADIOLIB_ARDUINOHAL_H)
#define _RADIOLIB_ARDUINOHAL_H

// this file only makes sense for Arduino builds
#if defined(RADIOLIB_BUILD_ARDUINO)

#if defined(RADIOLIB_MBED_TONE_OVERRIDE)
#include "mbed.h"
#endif

#include "Hal.h"

#include <SPI.h>

/*!
  \class ArduinoHal
  \brief Arduino default hardware abstraction library implementation.
  This class can be extended to support other Arduino platform or change behaviour of the default implementation.
*/
class ArduinoHal : public RadioLibHal {
  public:
    /*!
      \brief Arduino Hal constructor. Will use the default SPI interface and automatically initialize it.
    */
    ArduinoHal();

    /*!
      \brief Arduino Hal constructor. Will not attempt SPI interface initialization.
      \param spi SPI interface to be used, can also use software SPI implementations.
      \param spiSettings SPI interface settings.
    */
    ArduinoHal(SPIClass& spi, SPISettings spiSettings = RADIOLIB_DEFAULT_SPI_SETTINGS);

    // implementations of pure virtual RadioLibHal methods
    void pinMode(uint32_t pin, uint32_t mode) override;
    void digitalWrite(uint32_t pin, uint32_t value) override;
    uint32_t digitalRead(uint32_t pin) override;
    void attachInterrupt(uint32_t interruptNum, void (*interruptCb)(void), uint32_t mode) override;
    void detachInterrupt(uint32_t interruptNum) override;
    void delay(unsigned long ms) override;
    void delayMicroseconds(unsigned long us) override;
    unsigned long millis() override;
    unsigned long micros() override;
    long pulseIn(uint32_t pin, uint32_t state, unsigned long timeout) override;
    void spiBegin() override;
    void spiBeginTransaction() override;
    void spiTransfer(uint8_t* out, size_t len, uint8_t* in) override;
    void spiEndTransaction() override;
    void spiEnd() override;

    // implementations of virtual RadioLibHal methods
    void init() override;
    void term() override;
    void tone(uint32_t pin, unsigned int frequency, unsigned long duration = 0) override;
    void noTone(uint32_t pin) override;
    void yield() override;
    uint32_t pinToInterrupt(uint32_t pin) override;

#if !RADIOLIB_GODMODE
  private:
#endif
    SPIClass* spi = NULL;
    SPISettings spiSettings = RADIOLIB_DEFAULT_SPI_SETTINGS;
    bool initInterface = false;

    #if defined(RADIOLIB_MBED_TONE_OVERRIDE)
    mbed::PwmOut *pwmPin = NULL;
    #endif

    #if defined(RADIOLIB_ESP32)
    int32_t prev = -1;
    #endif
};

#endif

#endif
