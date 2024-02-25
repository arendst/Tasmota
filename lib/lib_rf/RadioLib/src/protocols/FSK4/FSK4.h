#if !defined(_RADIOLIB_FSK4_H)
#define _RADIOLIB_FSK4_H

#include "../../TypeDef.h"

#if !RADIOLIB_EXCLUDE_FSK4

#include "../PhysicalLayer/PhysicalLayer.h"
#include "../AFSK/AFSK.h"

/*!
  \class FSK4Client
  \brief Client for FSK-4 communication. The public interface is the same as Arduino Serial.
*/
class FSK4Client {
  public:
    /*!
      \brief Constructor for FSK-4 mode.
      \param phy Pointer to the wireless module providing PhysicalLayer communication.
    */
    explicit FSK4Client(PhysicalLayer* phy);

    #if !RADIOLIB_EXCLUDE_AFSK
    /*!
      \brief Constructor for AFSK mode.
      \param audio Pointer to the AFSK instance providing audio.
    */
    explicit FSK4Client(AFSKClient* audio);
    #endif

    // basic methods

    /*!
      \brief Initialization method.
      \param base Base (space) frequency to be used in MHz (in FSK-4 mode),
      or the space tone frequency in Hz (in AFSK mode)
      \param shift Frequency shift between each tone in Hz.
      \param rate Baud rate to be used during transmission.
      \returns \ref status_codes
    */
    int16_t begin(float base, uint32_t shift, uint16_t rate);

    /*!
      \brief Send out idle condition (RF tone at mark frequency).
    */
    void idle();

    /*!
      \brief Set correction coefficients for frequencies and tone length.
      \param offsets Four positive or negative correction offsets for audio frequencies in Hz.
      \param length Tone length modifier, defaults to 1.0.
      \returns \ref status_codes
    */
    int16_t setCorrection(int16_t offsets[4], float length = 1.0f);

    /*!
      \brief Transmit binary data.
      \param buff Buffer to transmit.
      \param len Number of bytes to transmit.
      \returns Number of transmitted bytes.
    */
    size_t write(uint8_t* buff, size_t len);

    /*!
      \brief Transmit a single byte.
      \param b Byte to transmit.
      \returns Number of transmitted bytes.
    */
    size_t write(uint8_t b);

    /*!
      \brief Stop transmitting.
      \returns \ref status_codes
    */
    int16_t standby();

#if !RADIOLIB_GODMODE
  private:
#endif
    PhysicalLayer* phyLayer;
    #if !RADIOLIB_EXCLUDE_AFSK
    AFSKClient* audioClient;
    #endif

    uint32_t baseFreq = 0, baseFreqHz = 0;
    uint32_t shiftFreq = 0, shiftFreqHz = 0;
    uint32_t bitDuration = 0;
    uint32_t tones[4];
    uint32_t tonesHz[4];

    void tone(uint8_t i);

    int16_t transmitDirect(uint32_t freq = 0, uint32_t freqHz = 0);
    int32_t getRawShift(int32_t shift);
};

#endif

#endif
