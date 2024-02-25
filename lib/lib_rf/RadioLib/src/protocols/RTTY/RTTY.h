#if !defined(_RADIOLIB_RTTY_H)
#define _RADIOLIB_RTTY_H

#include "../../TypeDef.h"

#if !RADIOLIB_EXCLUDE_RTTY

#include "../PhysicalLayer/PhysicalLayer.h"
#include "../AFSK/AFSK.h"
#include "../Print/Print.h"
#include "../Print/ITA2String.h"

/*!
  \class RTTYClient
  \brief Client for RTTY communication. The public interface is the same as Arduino Serial.
*/
class RTTYClient: public RadioLibPrint {
  public:
    /*!
      \brief Constructor for 2-FSK mode.
      \param phy Pointer to the wireless module providing PhysicalLayer communication.
    */
    explicit RTTYClient(PhysicalLayer* phy);

    #if !RADIOLIB_EXCLUDE_AFSK
    /*!
      \brief Constructor for AFSK mode.
      \param audio Pointer to the AFSK instance providing audio.
    */
    explicit RTTYClient(AFSKClient* audio);
    #endif

    // basic methods

    /*!
      \brief Initialization method.
      \param base Base (space) frequency to be used in MHz (in 2-FSK mode), or the space tone frequency in Hz (in AFSK mode)
      \param shift Frequency shift between mark and space in Hz.
      \param rate Baud rate to be used during transmission.
      \param enc Encoding to be used. Defaults to ASCII.
      \param stopBits Number of stop bits to be used.
      \returns \ref status_codes
    */
    int16_t begin(float base, uint32_t shift, uint16_t rate, uint8_t enc = RADIOLIB_ASCII, uint8_t stopBits = 1);

    /*!
      \brief Send out idle condition (RF tone at mark frequency).
    */
    void idle();

    /*!
      \brief Stops transmitting.
      \returns \ref status_codes
    */
    int16_t standby();

    /*!
      \brief Write one byte. Implementation of interface of the RadioLibPrint/Print class.
      \param b Byte to write.
      \returns 1 if the byte was written, 0 otherwise.
    */
    size_t write(uint8_t b);

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
    uint8_t stopBitsNum = 0;

    void mark();
    void space();

    int16_t transmitDirect(uint32_t freq = 0, uint32_t freqHz = 0);
};

#endif

#endif
