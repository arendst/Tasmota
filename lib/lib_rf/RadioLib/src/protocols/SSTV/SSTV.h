#if !defined(_RADIOLIB_SSTV_H)
#define _RADIOLIB_SSTV_H

#include "../../TypeDef.h"

#if !RADIOLIB_EXCLUDE_SSTV

#include "../PhysicalLayer/PhysicalLayer.h"
#include "../AFSK/AFSK.h"

// the following implementation is based on information from
// http://www.barberdsp.com/downloads/Dayton%20Paper.pdf

// VIS codes
#define RADIOLIB_SSTV_SCOTTIE_1                                 60
#define RADIOLIB_SSTV_SCOTTIE_2                                 56
#define RADIOLIB_SSTV_SCOTTIE_DX                                76
#define RADIOLIB_SSTV_MARTIN_1                                  44
#define RADIOLIB_SSTV_MARTIN_2                                  40
#define RADIOLIB_SSTV_WRASSE_SC2_180                            55
#define RADIOLIB_SSTV_PASOKON_P3                                113
#define RADIOLIB_SSTV_PASOKON_P5                                114
#define RADIOLIB_SSTV_PASOKON_P7                                115

// SSTV tones in Hz
#define RADIOLIB_SSTV_TONE_LEADER                               1900
#define RADIOLIB_SSTV_TONE_BREAK                                1200
#define RADIOLIB_SSTV_TONE_VIS_1                                1100
#define RADIOLIB_SSTV_TONE_VIS_0                                1300
#define RADIOLIB_SSTV_TONE_BRIGHTNESS_MIN                       1500
#define RADIOLIB_SSTV_TONE_BRIGHTNESS_MAX                       2300

// calibration header timing in us
#define RADIOLIB_SSTV_HEADER_LEADER_LENGTH                      300000
#define RADIOLIB_SSTV_HEADER_BREAK_LENGTH                       10000
#define RADIOLIB_SSTV_HEADER_BIT_LENGTH                         30000

/*!
  \struct tone_t
  \brief Structure to save data about tone.
*/
struct tone_t {

  /*!
    \brief Tone type: GENERIC for sync and porch tones, SCAN_GREEN, SCAN_BLUE and SCAN_RED for scan lines.
  */
  enum {
    GENERIC = 0,
    SCAN_GREEN,
    SCAN_BLUE,
    SCAN_RED
  } type;

  /*!
    \brief Length of tone in us, set to 0 for picture scan tones.
  */
  uint32_t len;

  /*!
    \brief Frequency of tone in Hz, set to 0 for picture scan tones.
  */
  uint16_t freq;
};

/*!
  \struct SSTVMode_t
  \brief Structure to save data about supported SSTV modes.
*/
struct SSTVMode_t {

  /*!
    \brief Unique VIS code of the SSTV mode.
  */
  uint8_t visCode;

  /*!
    \brief Picture width in pixels.
  */
  uint16_t width;

  /*!
    \brief Picture height in pixels.
  */
  uint16_t height;

  /*!
    \brief Pixel scan length in us.
  */
  uint16_t scanPixelLen;

  /*!
    \brief Number of tones in each transmission line. Picture scan data is considered single tone.
  */
  uint8_t numTones;

  /*!
    \brief Sequence of tones in each transmission line. This is used to create the correct encoding sequence.
  */
  tone_t tones[8];
};

// all currently supported SSTV modes
extern const SSTVMode_t Scottie1;
extern const SSTVMode_t Scottie2;
extern const SSTVMode_t ScottieDX;
extern const SSTVMode_t Martin1;
extern const SSTVMode_t Martin2;
extern const SSTVMode_t Wrasse;
extern const SSTVMode_t PasokonP3;
extern const SSTVMode_t PasokonP5;
extern const SSTVMode_t PasokonP7;

/*!
  \class SSTVClient
  \brief Client for SSTV transmissions.
*/
class SSTVClient {
  public:
    /*!
      \brief Constructor for 2-FSK mode.
      \param phy Pointer to the wireless module providing PhysicalLayer communication.
    */
    explicit SSTVClient(PhysicalLayer* phy);

    #if !RADIOLIB_EXCLUDE_AFSK
    /*!
      \brief Constructor for AFSK mode.
      \param audio Pointer to the AFSK instance providing audio.
    */
    explicit SSTVClient(AFSKClient* audio);
    #endif

    // basic methods

    /*!
      \brief Initialization method for 2-FSK.
      \param base Base "0 Hz tone" RF frequency to be used in MHz.
      \param mode SSTV mode to be used. Currently supported modes are Scottie1, Scottie2, 
      ScottieDX, Martin1, Martin2, Wrasse, PasokonP3, PasokonP5 and PasokonP7.
      \returns \ref status_codes
    */
    int16_t begin(float base, const SSTVMode_t& mode);

    #if !RADIOLIB_EXCLUDE_AFSK
    /*!
      \brief Initialization method for AFSK.
      \param mode SSTV mode to be used. Currently supported modes are Scottie1, Scottie2,
      ScottieDX, Martin1, Martin2, Wrasse, PasokonP3, PasokonP5 and PasokonP7.
      \returns \ref status_codes
    */
    int16_t begin(const SSTVMode_t& mode);
    #endif

    /*!
      \brief Set correction coefficient for tone length.
      \param correction Timing correction factor, used to adjust the length of timing pulses.
      Less than 1.0 leads to shorter timing pulses, defaults to 1.0 (no correction).
      \returns \ref status_codes
    */
    int16_t setCorrection(float correction);

    /*!
      \brief Sends out tone at 1900 Hz.
    */
    void idle();

    /*!
      \brief Sends synchronization header for the SSTV mode set in begin method.
    */
    void sendHeader();

    /*!
      \brief Sends single picture line in the currently configured SSTV mode.
      \param imgLine Image line to send, in 24-bit RGB. It is up to the user to ensure that
      imgLine has enough pixels to send it in the current SSTV mode.
    */
    void sendLine(uint32_t* imgLine);

    /*!
      \brief Get picture height of the currently configured SSTV mode.
      \returns Picture height of the currently configured SSTV mode in pixels.
    */
    uint16_t getPictureHeight() const;

#if !RADIOLIB_GODMODE
  private:
#endif
    PhysicalLayer* phyLayer;
    #if !RADIOLIB_EXCLUDE_AFSK
    AFSKClient* audioClient;
    #endif

    uint32_t baseFreq = 0;
    SSTVMode_t txMode = Scottie1;
    bool firstLine = true;

    void tone(float freq, uint32_t len = 0);
};

#endif

#endif
