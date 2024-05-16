#if !defined(_RADIOLIB_HAL_H)
#define _RADIOLIB_HAL_H

#include <stdint.h>
#include <stddef.h>

#include "BuildOpt.h"

/*!
  \class RadioLibHal
  \brief Hardware abstraction library base interface.
*/
class RadioLibHal {
  public:

    // values for pin modes, levels and change directions
    // these tell RadioLib how are different logic states represented on a given platform

    /*!
      \brief Value to be used as the "input" GPIO direction.
    */
    const uint32_t GpioModeInput;

    /*!
      \brief Value to be used as the "output" GPIO direction.
    */
    const uint32_t GpioModeOutput;
    
    /*!
      \brief Value to be used as the "low" GPIO level.
    */
    const uint32_t GpioLevelLow;
    
    /*!
      \brief Value to be used as the "high" GPIO level.
    */
    const uint32_t GpioLevelHigh;
    
    /*!
      \brief Value to be used as the "rising" GPIO level change direction.
    */
    const uint32_t GpioInterruptRising;
    
    /*!
      \brief Value to be used as the "falling" GPIO level change direction.
    */
    const uint32_t GpioInterruptFalling;

    /*!
      \brief Default constructor.
      \param input Value to be used as the "input" GPIO direction.
      \param output Value to be used as the "output" GPIO direction.
      \param low Value to be used as the "low" GPIO level.
      \param high Value to be used as the "high" GPIO level.
      \param rising Value to be used as the "rising" GPIO level change direction.
      \param falling Value to be used as the "falling" GPIO level change direction.
    */
    RadioLibHal(const uint32_t input, const uint32_t output, const uint32_t low, const uint32_t high, const uint32_t rising, const uint32_t falling);

    // pure virtual methods - these must be implemented by the hardware abstraction for RadioLib to function

    /*!
      \brief GPIO pin mode (input/output/...) configuration method.
      Must be implemented by the platform-specific hardware abstraction!
      \param pin Pin to be changed (platform-specific).
      \param mode Mode to be set (platform-specific).
    */
    virtual void pinMode(uint32_t pin, uint32_t mode) = 0;

    /*!
      \brief Digital write method.
      Must be implemented by the platform-specific hardware abstraction!
      \param pin Pin to be changed (platform-specific).
      \param value Value to set (platform-specific).
    */
    virtual void digitalWrite(uint32_t pin, uint32_t value) = 0;

    /*!
      \brief Digital read method.
      Must be implemented by the platform-specific hardware abstraction!
      \param pin Pin to be changed (platform-specific).
      \returns Value read on the pin (platform-specific).
    */
    virtual uint32_t digitalRead(uint32_t pin) = 0;
    
    /*!
      \brief Method to attach function to an external interrupt.
      Must be implemented by the platform-specific hardware abstraction!
      \param interruptNum Interrupt number to attach to (platform-specific).
      \param interruptCb Interrupt service routine to execute.
      \param mode Rising/falling mode (platform-specific).
    */
    virtual void attachInterrupt(uint32_t interruptNum, void (*interruptCb)(void), uint32_t mode) = 0;

    /*!
      \brief Method to detach function from an external interrupt.
      Must be implemented by the platform-specific hardware abstraction!
      \param interruptNum Interrupt number to detach from (platform-specific).
    */
    virtual void detachInterrupt(uint32_t interruptNum) = 0;

    /*!
      \brief Blocking wait function.
      Must be implemented by the platform-specific hardware abstraction!
      \param ms Number of milliseconds to wait.
    */
    virtual void delay(unsigned long ms) = 0;
    
    /*!
      \brief Blocking microsecond wait function.
      Must be implemented by the platform-specific hardware abstraction!
      \param us Number of microseconds to wait.
    */
    virtual void delayMicroseconds(unsigned long us) = 0;
    
    /*!
      \brief Get number of milliseconds since start.
      Must be implemented by the platform-specific hardware abstraction!
      \returns Number of milliseconds since start.
    */
    virtual unsigned long millis() = 0;
    
    /*!
      \brief Get number of microseconds since start.
      Must be implemented by the platform-specific hardware abstraction!
      \returns Number of microseconds since start.
    */
    virtual unsigned long micros() = 0;
    
    /*!
      \brief Measure the length of incoming digital pulse in microseconds.
      Must be implemented by the platform-specific hardware abstraction!
      \param pin Pin to measure on (platform-specific).
      \param state Pin level to monitor (platform-specific).
      \param timeout Timeout in microseconds.
      \returns Pulse length in microseconds, or 0 if the pulse did not start before timeout.
    */
    virtual long pulseIn(uint32_t pin, uint32_t state, unsigned long timeout) = 0;

    /*!
      \brief SPI initialization method.
    */
    virtual void spiBegin() = 0;

    /*!
      \brief Method to start SPI transaction.
    */
    virtual void spiBeginTransaction() = 0;

    /*!
      \brief Method to transfer buffer over SPI.
      \param out Buffer to send.
      \param len Number of data to send or receive.
      \param in Buffer to save received data into.
    */
    virtual void spiTransfer(uint8_t* out, size_t len, uint8_t* in) = 0;

    /*!
      \brief Method to end SPI transaction.
    */
    virtual void spiEndTransaction() = 0;

    /*!
      \brief SPI termination method.
    */
    virtual void spiEnd() = 0;

    // virtual methods - these may or may not exists on a given platform
    // they exist in this implementation, but do nothing

    /*!
      \brief Module initialization method.
      This will be called by all radio modules at the beginning of startup.
      Can be used to e.g., initialize SPI interface.
    */
    virtual void init();

    /*!
      \brief Module termination method.
      This will be called by all radio modules when the destructor is called.
      Can be used to e.g., stop SPI interface.
    */
    virtual void term();

    /*!
      \brief Method to produce a square-wave with 50% duty cycle ("tone") of a given frequency at some pin.
      \param pin Pin to be used as the output.
      \param frequency Frequency of the square wave.
      \param duration Duration of the tone in ms. When set to 0, the tone will be infinite.
    */
    virtual void tone(uint32_t pin, unsigned int frequency, unsigned long duration = 0);

    /*!
      \brief Method to stop producing a tone.
      \param pin Pin which is currently producing the tone.
    */
    virtual void noTone(uint32_t pin);
    
    /*!
      \brief Yield method, called from long loops in multi-threaded environment (to prevent blocking other threads).
    */
    virtual void yield();
    
    /*!
      \brief Function to convert from pin number to interrupt number.
      \param pin Pin to convert from.
      \returns The interrupt number of a given pin.
    */
    virtual uint32_t pinToInterrupt(uint32_t pin);
};

#endif
