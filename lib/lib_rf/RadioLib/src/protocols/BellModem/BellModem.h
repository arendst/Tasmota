#if !defined(_RADIOLIB_BELL_MODEM_H)
#define _RADIOLIB_BELL_MODEM_H

#include "../../TypeDef.h"
#include "../../Module.h"
#if defined(RADIOLIB_BUILD_ARDUINO)
#include "../../ArduinoHal.h"
#endif

#if !RADIOLIB_EXCLUDE_BELL

#include "../PhysicalLayer/PhysicalLayer.h"
#include "../AFSK/AFSK.h"
#include "../Print/Print.h"
#include "../Print/ITA2String.h"

/*!
  \struct BellModem_t
  \brief Definition of the Bell-compatible modem.
*/
struct BellModem_t {
  /*!
    \brief Frequency of the mark tone.
  */
  int16_t freqMark;

  /*!
    \brief Frequency of the space tone.
  */
  int16_t freqSpace;

  /*!
    \brief Baud rate.
  */
  int16_t baudRate;

  /*!
    \brief Frequency of the mark tone when replying.
  */
  int16_t freqMarkReply;

  /*!
    \brief Frequency of the space tone when replying.
  */
  int16_t freqSpaceReply;
};

// currently implemented Bell modems
extern const struct BellModem_t Bell101;
extern const struct BellModem_t Bell103;
extern const struct BellModem_t Bell202;

/*!
  \class BellClient
  \brief Client for Bell modem communication. The public interface is the same as Arduino Serial.
*/
class BellClient: public AFSKClient, public RadioLibPrint {
  
  public:
    
    /*!
      \brief Whether the modem is replying.
      On some modems, the replying station has different tone frequencies.
    */
    bool reply;

    /*!
      \brief Default constructor.
      \param phy Pointer to the wireless module providing PhysicalLayer communication.
      \param pin The GPIO pin at which the tones will be generated.
    */
    explicit BellClient(PhysicalLayer* phy, uint32_t pin);

    /*!
      \brief Audio-client constructor. Can be used when AFSKClient instance already exists.
      \param aud Audio client to use.
    */
    BellClient(AFSKClient* aud);

    /*!
      \brief Initialization method.
      \param modem Definition of the Bell modem to use for communication.
      \returns \ref status_codes
    */
    int16_t begin(const BellModem_t& modem);

    /*!
      \brief Set Bell modem.
      \param modem Definition of the Bell modem to use for communication.
      \returns \ref status_codes
    */
    int16_t setModem(const BellModem_t& modem);

    /*!
      \brief Set correction coefficient for tone length.
      \param correction Timing correction factor, used to adjust the length of tones.
      Less than 1.0 leads to shorter tones, defaults to 1.0 (no correction).
      \returns \ref status_codes
    */
    int16_t setCorrection(float corr);

    /*!
      \brief Write one byte. Implementation of interface of the RadioLibPrint/Print class.
      \param b Byte to write.
      \returns 1 if the byte was written, 0 otherwise.
    */
    size_t write(uint8_t b);

    /*!
      \brief Set the modem to idle (ready to transmit).
    */
    int16_t idle();

    /*!
      \brief Set the modem to standby (transmitter off).
    */
    int16_t standby();

#if !RADIOLIB_GODMODE
  private:
#endif
    BellModem_t modemType;
    float correction = 1.0;
    uint16_t toneLen = 0;
    bool autoStart = true;

};

#endif

#endif
