#if !defined(_RADIOLIB_PHYSICAL_LAYER_H)
#define _RADIOLIB_PHYSICAL_LAYER_H

#include "../../TypeDef.h"
#include "../../Module.h"

// data rate structure interpretation in case LoRa is used
struct LoRaRate_t {
  uint8_t spreadingFactor;
  float bandwidth;
  uint8_t codingRate;
};

// data rate structure interpretation in case FSK is used
struct FSKRate_t {
  float bitRate;
  float freqDev;
};

// common data rate
union DataRate_t {
  LoRaRate_t lora;
  FSKRate_t fsk;
};

/*!
  \class PhysicalLayer

  \brief Provides common interface for protocols that run on %LoRa/FSK modules, such as RTTY or LoRaWAN.
  Also extracts some common module-independent methods. Using this interface class allows to use the protocols
  on various modules without much code duplicity. Because this class is used mainly as interface,
  all of its virtual members must be implemented in the module class.
*/
class PhysicalLayer {
  public:

    // constructor

    /*!
      \brief Default constructor.
      \param step Frequency step of the synthesizer in Hz.
      \param maxLen Maximum length of packet that can be received by the module.
    */
    PhysicalLayer(float step, size_t maxLen);

    // basic methods

    #if defined(RADIOLIB_BUILD_ARDUINO)
    /*!
      \brief Arduino Flash String transmit method.
      \param str Pointer to Arduino Flash String that will be transmitted.
      \param addr Node address to transmit the packet to. Only used in FSK mode.
      \returns \ref status_codes
    */
    int16_t transmit(__FlashStringHelper* fstr, uint8_t addr = 0);

    /*!
      \brief Arduino String transmit method.
      \param str Address of Arduino string that will be transmitted.
      \param addr Node address to transmit the packet to. Only used in FSK mode.
      \returns \ref status_codes
    */
    int16_t transmit(String& str, uint8_t addr = 0);
    #endif

    /*!
      \brief C-string transmit method.
      \param str C-string that will be transmitted.
      \param addr Node address to transmit the packet to. Only used in FSK mode.
      \returns \ref status_codes
    */
    int16_t transmit(const char* str, uint8_t addr = 0);

    /*!
      \brief Binary transmit method. Must be implemented in module class.
      \param data Binary data that will be transmitted.
      \param len Length of binary data to transmit (in bytes).
      \param addr Node address to transmit the packet to. Only used in FSK mode.
      \returns \ref status_codes
    */
    virtual int16_t transmit(uint8_t* data, size_t len, uint8_t addr = 0);

    #if defined(RADIOLIB_BUILD_ARDUINO)
    /*!
      \brief Arduino String receive method.
      \param str Address of Arduino String to save the received data.
      \param len Expected number of characters in the message. Leave as 0 if expecting a unknown size packet
      \returns \ref status_codes
    */
    int16_t receive(String& str, size_t len = 0);
    #endif

    /*!
      \brief Sets module to sleep.
      \returns \ref status_codes
    */
    virtual int16_t sleep();

    /*!
      \brief Sets module to standby.
      \returns \ref status_codes
    */
    virtual int16_t standby();

    /*!
      \brief Sets module to a specific standby mode.
      \returns \ref status_codes
    */
    virtual int16_t standby(uint8_t mode);

    /*!
      \brief Sets module to received mode using its default configuration.
      \returns \ref status_codes
    */
    virtual int16_t startReceive();

    /*!
      \brief Interrupt-driven receive method. A DIO pin will be activated when full packet is received. 
      Must be implemented in module class.
      \param timeout Raw timeout value. Some modules use this argument to specify operation mode
      (single vs. continuous receive).
      \param irqFlags Sets the IRQ flags.
      \param irqMask Sets the mask of IRQ flags that will trigger the DIO pin.
      \param len Packet length, needed for some modules under special circumstances (e.g. LoRa implicit header mode).
      \returns \ref status_codes
    */
    virtual int16_t startReceive(uint32_t timeout, uint16_t irqFlags, uint16_t irqMask, size_t len);

    /*!
      \brief Binary receive method. Must be implemented in module class.
      \param data Pointer to array to save the received binary data.
      \param len Number of bytes that will be received. Must be known in advance for binary transmissions.
      \returns \ref status_codes
    */
    virtual int16_t receive(uint8_t* data, size_t len);

    #if defined(RADIOLIB_BUILD_ARDUINO)
    /*!
      \brief Interrupt-driven Arduino String transmit method. Unlike the standard transmit method, this one is non-blocking.
      Interrupt pin will be activated when transmission finishes.
      \param str Address of Arduino String that will be transmitted.
      \param addr Node address to transmit the packet to. Only used in FSK mode.
      \returns \ref status_codes
    */
    int16_t startTransmit(String& str, uint8_t addr = 0);
    #endif

    /*!
      \brief Interrupt-driven Arduino String transmit method. Unlike the standard transmit method, this one is non-blocking.
      Interrupt pin will be activated when transmission finishes.
      \param str C-string that will be transmitted.
      \param addr Node address to transmit the packet to. Only used in FSK mode.
      \returns \ref status_codes
    */
    int16_t startTransmit(const char* str, uint8_t addr = 0);

    /*!
      \brief Interrupt-driven binary transmit method.
      \param data Binary data that will be transmitted.
      \param len Length of binary data to transmit (in bytes).
      \param addr Node address to transmit the packet to. Only used in FSK mode.
      \returns \ref status_codes
    */
    virtual int16_t startTransmit(uint8_t* data, size_t len, uint8_t addr = 0);

    /*!
      \brief Clean up after transmission is done.
      \returns \ref status_codes
    */
    virtual int16_t finishTransmit();

    #if defined(RADIOLIB_BUILD_ARDUINO)
    /*!
      \brief Reads data that was received after calling startReceive method.
      \param str Address of Arduino String to save the received data.
      \param len Expected number of characters in the message. When set to 0, the packet length will be retrieved 
      automatically. When more bytes than received are requested, only the number of bytes requested will be returned.
      \returns \ref status_codes
    */
    int16_t readData(String& str, size_t len = 0);
    #endif

    /*!
      \brief Reads data that was received after calling startReceive method.
      \param data Pointer to array to save the received binary data.
      \param len Number of bytes that will be read. When set to 0, the packet length will be retrieved automatically.
      When more bytes than received are requested, only the number of bytes requested will be returned.
      \returns \ref status_codes
    */
    virtual int16_t readData(uint8_t* data, size_t len);

    /*!
      \brief Enables direct transmission mode on pins DIO1 (clock) and DIO2 (data). Must be implemented in module class.
      While in direct mode, the module will not be able to transmit or receive packets. Can only be activated in FSK mode.
      \param frf 24-bit raw frequency value to start transmitting at. Required for quick frequency shifts in RTTY.
      \returns \ref status_codes
    */
    virtual int16_t transmitDirect(uint32_t frf = 0);

    /*!
      \brief Enables direct reception mode on pins DIO1 (clock) and DIO2 (data). Must be implemented in module class.
      While in direct mode, the module will not be able to transmit or receive packets. Can only be activated in FSK mode.
      \returns \ref status_codes
    */
    virtual int16_t receiveDirect();

    // configuration methods

    /*!
      \brief Sets carrier frequency. Must be implemented in module class.
      \param freq Carrier frequency to be set in MHz.
      \returns \ref status_codes
    */
    virtual int16_t setFrequency(float freq);

    /*!
      \brief Sets FSK bit rate. Only available in FSK mode. Must be implemented in module class.
      \param br Bit rate to be set (in kbps).
      \returns \ref status_codes
    */
    virtual int16_t setBitRate(float br);

    /*!
      \brief Sets FSK frequency deviation from carrier frequency. Only available in FSK mode.
      Must be implemented in module class.
      \param freqDev Frequency deviation to be set (in kHz).
      \returns \ref status_codes
    */
    virtual int16_t setFrequencyDeviation(float freqDev);

    /*!
      \brief Sets GFSK data shaping. Only available in FSK mode. Must be implemented in module class.
      \param sh Shaping to be set. See \ref config_shaping for possible values.
      \returns \ref status_codes
    */
    virtual int16_t setDataShaping(uint8_t sh);

    /*!
      \brief Sets FSK data encoding. Only available in FSK mode. Must be implemented in module class.
      \param enc Encoding to be used. See \ref config_encoding for possible values.
      \returns \ref status_codes
    */
    virtual int16_t setEncoding(uint8_t encoding);

    /*!
      \brief Set IQ inversion. Must be implemented in module class if the module supports it.
      \param enable True to use inverted IQ, false for non-inverted.
      \returns \ref status_codes
    */
    virtual int16_t invertIQ(bool enable);

    /*!
      \brief Set output power. Must be implemented in module class if the module supports it.
      \param power Output power in dBm. The allowed range depends on the module used.
      \returns \ref status_codes
    */
    virtual int16_t setOutputPower(int8_t power);

    /*!
      \brief Set sync word. Must be implemented in module class if the module supports it.
      \param sync Pointer to the sync word.
      \param len Sync word length in bytes. Maximum length depends on the module used.
      \returns \ref status_codes
    */
    virtual int16_t setSyncWord(uint8_t* sync, size_t len);

    /*!
      \brief Set preamble length. Must be implemented in module class if the module supports it.
      \param len Preamble length in bytes. Maximum length depends on the module used.
      \returns \ref status_codes
    */
    virtual int16_t setPreambleLength(size_t len);
    
    /*!
      \brief Set data. Must be implemented in module class if the module supports it.
      \param dr Data rate struct. Interpretation depends on currently active modem (FSK or LoRa).
      \returns \ref status_codes
    */
    virtual int16_t setDataRate(DataRate_t dr);

    /*!
      \brief Check the data rate can be configured by this module. Must be implemented in module class if the module supports it.
      \param dr Data rate struct. Interpretation depends on currently active modem (FSK or LoRa).
      \returns \ref status_codes
    */
    virtual int16_t checkDataRate(DataRate_t dr);

    /*!
      \brief Gets the module frequency step size that was set in constructor.
      \returns Synthesizer frequency step size in Hz.
    */
    float getFreqStep() const;

    /*!
      \brief Query modem for the packet length of received payload. Must be implemented in module class.
      \param update Update received packet length. Will return cached value when set to false.
      \returns Length of last received packet in bytes.
    */
    virtual size_t getPacketLength(bool update = true);

    /*!
      \brief Gets RSSI (Recorded Signal Strength Indicator) of the last received packet.
      \returns RSSI of the last received packet in dBm.
    */
    virtual float getRSSI();

    /*!
      \brief Gets SNR (Signal to Noise Ratio) of the last received packet. Only available for LoRa modem.
      \returns SNR of the last received packet in dB.
    */
    virtual float getSNR();

    /*!
      \brief Get expected time-on-air for a given size of payload
      \param len Payload length in bytes.
      \returns Expected time-on-air in microseconds.
    */
    virtual uint32_t getTimeOnAir(size_t len);

    /*!
      \brief Calculate the timeout value for this specific module / series (in number of symbols or units of time)
      \param timeoutUs Timeout in microseconds to listen for
      \returns Timeout value in a unit that is specific for the used module
    */
    virtual uint32_t calculateRxTimeout(uint32_t timeoutUs);

    /*!
      \brief Create the flags that make up RxDone and RxTimeout used for receiving downlinks
      \param irqFlags The flags for which IRQs must be triggered
      \param irqMask Mask indicating which IRQ triggers a DIO
      \returns \ref status_codes
    */
    virtual int16_t irqRxDoneRxTimeout(uint16_t &irqFlags, uint16_t &irqMask);

    /*!
      \brief Check whether the IRQ bit for RxTimeout is set
      \returns \ref RxTimeout IRQ is set
    */
    virtual bool isRxTimeout();

    /*!
      \brief Interrupt-driven channel activity detection method. interrupt will be activated
      when packet is detected. Must be implemented in module class.
      \returns \ref status_codes
    */
    virtual int16_t startChannelScan();

    /*!
      \brief Read the channel scan result
      \returns \ref status_codes
    */
    virtual int16_t getChannelScanResult();

    /*!
      \brief Check whether the current communication channel is free or occupied. Performs CAD for LoRa modules,
      or RSSI measurement for FSK modules.
      \returns RADIOLIB_CHANNEL_FREE when channel is free,
      RADIOLIB_PREAMBLE_DETECTEDwhen occupied or other \ref status_codes.
    */
    virtual int16_t scanChannel();

    /*!
      \brief Get truly random number in range 0 - max.
      \param max The maximum value of the random number (non-inclusive).
      \returns Random number.
    */
    int32_t random(int32_t max);

    /*!
      \brief Get truly random number in range min - max.
      \param min The minimum value of the random number (inclusive).
      \param max The maximum value of the random number (non-inclusive).
      \returns Random number.
    */
    int32_t random(int32_t min, int32_t max);

    /*!
      \brief Get one truly random byte from RSSI noise. Must be implemented in module class.
      \returns TRNG byte.
    */
    virtual uint8_t randomByte();

    /*!
      \brief Configure module parameters for direct modes. Must be called prior to "ham" modes like RTTY or AX.25.
      Only available in FSK mode.
      \returns \ref status_codes
    */
    int16_t startDirect();

    #if !RADIOLIB_EXCLUDE_DIRECT_RECEIVE
    /*!
      \brief Set sync word to be used to determine start of packet in direct reception mode.
      \param syncWord Sync word bits.
      \param len Sync word length in bits. Set to zero to disable sync word matching.
      \returns \ref status_codes
    */
    int16_t setDirectSyncWord(uint32_t syncWord, uint8_t len);

    /*!
      \brief Set interrupt service routine function to call when data bit is received in direct mode.
      Must be implemented in module class.
      \param func Pointer to interrupt service routine.
    */
    virtual void setDirectAction(void (*func)(void));

    /*!
      \brief Function to read and process data bit in direct reception mode. Must be implemented in module class.
      \param pin Pin on which to read.
    */
    virtual void readBit(uint32_t pin);

    /*!
      \brief Get the number of direct mode bytes currently available in buffer.
      \returns Number of available bytes.
    */
    int16_t available();

    /*!
      \brief Forcefully drop synchronization.
    */
    void dropSync();

    /*!
      \brief Get data from direct mode buffer.
      \param drop Drop synchronization on read - next reading will require waiting for the sync word again.
      Defaults to true.
      \returns Byte from direct mode buffer.
    */
    uint8_t read(bool drop = true);
    #endif

    /*!
      \brief Configure DIO pin mapping to get a given signal on a DIO pin (if available).
      \param pin Pin number onto which a signal is to be placed.
      \param value The value that indicates which function to place on that pin. See chip datasheet for details.
      \returns \ref status_codes
    */
    virtual int16_t setDIOMapping(uint32_t pin, uint32_t value);

    /*!
      \brief Sets interrupt service routine to call when a packet is received.
      \param func ISR to call.
    */
    virtual void setPacketReceivedAction(void (*func)(void));

    /*!
      \brief Clears interrupt service routine to call when a packet is received.
    */
    virtual void clearPacketReceivedAction();

    /*!
      \brief Sets interrupt service routine to call when a packet is sent.
      \param func ISR to call.
    */
    virtual void setPacketSentAction(void (*func)(void));

    /*!
      \brief Clears interrupt service routine to call when a packet is sent.
    */
    virtual void clearPacketSentAction();
    
    /*!
      \brief Sets interrupt service routine to call when a channel scan is finished.
      \param func ISR to call.
    */
    virtual void setChannelScanAction(void (*func)(void));

    /*!
      \brief Clears interrupt service routine to call when a channel scan is finished.
    */
    virtual void clearChannelScanAction();

    #if RADIOLIB_INTERRUPT_TIMING

    /*!
      \brief Set function to be called to set up the timing interrupt.
      For details, see https://github.com/jgromes/RadioLib/wiki/Interrupt-Based-Timing
      \param func Setup function to be called, with one argument (pulse length in microseconds).
    */
    void setInterruptSetup(void (*func)(uint32_t));

    /*!
      \brief Set timing interrupt flag.
      For details, see https://github.com/jgromes/RadioLib/wiki/Interrupt-Based-Timing
    */
    void setTimerFlag();

    #endif

#if !RADIOLIB_GODMODE
  protected:
#endif
#if !RADIOLIB_EXCLUDE_DIRECT_RECEIVE
    void updateDirectBuffer(uint8_t bit);
#endif

#if !RADIOLIB_GODMODE
  private:
#endif
    float freqStep;
    size_t maxPacketLength;

    #if !RADIOLIB_EXCLUDE_DIRECT_RECEIVE
    uint8_t bufferBitPos;
    uint8_t bufferWritePos;
    uint8_t bufferReadPos;
    uint8_t buffer[RADIOLIB_STATIC_ARRAY_SIZE];
    uint32_t syncBuffer;
    uint32_t directSyncWord;
    uint8_t directSyncWordLen;
    uint32_t directSyncWordMask;
    bool gotSync;
    #endif

    virtual Module* getMod() = 0;

    // allow specific classes access the private getMod method
    friend class AFSKClient;
    friend class RTTYClient;
    friend class MorseClient;
    friend class HellClient;
    friend class SSTVClient;
    friend class AX25Client;
    friend class FSK4Client;
    friend class PagerClient;
    friend class BellClient;
    friend class FT8Client;
    friend class LoRaWANNode;
};

#endif
