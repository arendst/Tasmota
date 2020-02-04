// Copyright 2009 Ken Shirriff
// Copyright 2015 Mark Szabo
// Copyright 2017 David Conran
#ifndef IRSEND_H_
#define IRSEND_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include "IRremoteESP8266.h"

// Originally from https://github.com/shirriff/Arduino-IRremote/
// Updated by markszabo (https://github.com/crankyoldgit/IRremoteESP8266) for
// sending IR code on ESP8266

#if TEST || UNIT_TEST
#define VIRTUAL virtual
#else
#define VIRTUAL
#endif

// Constants
// Offset (in microseconds) to use in Period time calculations to account for
// code excution time in producing the software PWM signal.
#if defined(ESP32)
// Calculated on a generic ESP-WROOM-32 board with v3.2-18 SDK @ 240MHz
const int8_t kPeriodOffset = -2;
#elif (defined(ESP8266) && F_CPU == 160000000L)  // NOLINT(whitespace/parens)
// Calculated on an ESP8266 NodeMCU v2 board using:
// v2.6.0 with v2.5.2 ESP core @ 160MHz
const int8_t kPeriodOffset = -2;
#else  // (defined(ESP8266) && F_CPU == 160000000L)
// Calculated on ESP8266 Wemos D1 mini using v2.4.1 with v2.4.0 ESP core @ 40MHz
const int8_t kPeriodOffset = -5;
#endif  // (defined(ESP8266) && F_CPU == 160000000L)
const uint8_t kDutyDefault = 50;  // Percentage
const uint8_t kDutyMax = 100;     // Percentage
// delayMicroseconds() is only accurate to 16383us.
// Ref: https://www.arduino.cc/en/Reference/delayMicroseconds
const uint16_t kMaxAccurateUsecDelay = 16383;
//  Usecs to wait between messages we don't know the proper gap time.
const uint32_t kDefaultMessageGap = 100000;


namespace stdAc {
  enum class opmode_t {
    kOff  = -1,
    kAuto =  0,
    kCool =  1,
    kHeat =  2,
    kDry  =  3,
    kFan  =  4,
    // Add new entries before this one, and update it to point to the last entry
    kLastOpmodeEnum = kFan,
  };

  enum class fanspeed_t {
    kAuto =   0,
    kMin =    1,
    kLow =    2,
    kMedium = 3,
    kHigh =   4,
    kMax =    5,
    // Add new entries before this one, and update it to point to the last entry
    kLastFanspeedEnum = kMax,
  };

  enum class swingv_t {
    kOff =    -1,
    kAuto =    0,
    kHighest = 1,
    kHigh =    2,
    kMiddle =  3,
    kLow =     4,
    kLowest =  5,
    // Add new entries before this one, and update it to point to the last entry
    kLastSwingvEnum = kLowest,
  };

  enum class swingh_t {
    kOff =     -1,
    kAuto =     0,  // a.k.a. On.
    kLeftMax =  1,
    kLeft =     2,
    kMiddle =   3,
    kRight =    4,
    kRightMax = 5,
    kWide =     6,  // a.k.a. left & right at the same time.
    // Add new entries before this one, and update it to point to the last entry
    kLastSwinghEnum = kWide,
  };

  // Structure to hold a common A/C state.
  typedef struct {
    decode_type_t protocol;
    int16_t model;
    bool power;
    stdAc::opmode_t mode;
    float degrees;
    bool celsius;
    stdAc::fanspeed_t fanspeed;
    stdAc::swingv_t swingv;
    stdAc::swingh_t swingh;
    bool quiet;
    bool turbo;
    bool econo;
    bool light;
    bool filter;
    bool clean;
    bool beep;
    int16_t sleep;
    int16_t clock;
  } state_t;
};  // namespace stdAc


enum fujitsu_ac_remote_model_t {
  ARRAH2E = 1,  // (1) AR-RAH2E, AR-RAC1E, AR-RAE1E (Default)
  ARDB1,        // (2) AR-DB1, AR-DL10 (AR-DL10 swing doesn't work)
  ARREB1E,      // (3) AR-REB1E
  ARJW2,        // (4) AR-JW2  (Same as ARDB1 but with horiz control)
  ARRY4,        // (5) AR-RY4 (Same as AR-RAH2E but with clean & filter)
};

enum gree_ac_remote_model_t {
  YAW1F = 1,  // (1) Ultimate, EKOKAI, RusClimate (Default)
  YBOFB,     // (2) Green, YBOFB2, YAPOF3
};

enum panasonic_ac_remote_model_t {
  kPanasonicUnknown = 0,
  kPanasonicLke = 1,
  kPanasonicNke = 2,
  kPanasonicDke = 3,  // PKR too.
  kPanasonicJke = 4,
  kPanasonicCkp = 5,
  kPanasonicRkr = 6,
};

enum whirlpool_ac_remote_model_t {
  DG11J13A = 1,  // DG11J1-04 too
  DG11J191,
};

enum lg_ac_remote_model_t {
  GE6711AR2853M = 1,  // (1) LG 28-bit Protocol (default)
  AKB75215403,        // (2) LG2 28-bit Protocol
};


// Classes
class IRsend {
 public:
  explicit IRsend(uint16_t IRsendPin, bool inverted = false,
                  bool use_modulation = true);
  void begin();
  void enableIROut(uint32_t freq, uint8_t duty = kDutyDefault);
  VIRTUAL void _delayMicroseconds(uint32_t usec);
  VIRTUAL uint16_t mark(uint16_t usec);
  VIRTUAL void space(uint32_t usec);
  int8_t calibrate(uint16_t hz = 38000U);
  void sendRaw(uint16_t buf[], uint16_t len, uint16_t hz);
  void sendData(uint16_t onemark, uint32_t onespace, uint16_t zeromark,
                uint32_t zerospace, uint64_t data, uint16_t nbits,
                bool MSBfirst = true);
  void sendGeneric(const uint16_t headermark, const uint32_t headerspace,
                   const uint16_t onemark, const uint32_t onespace,
                   const uint16_t zeromark, const uint32_t zerospace,
                   const uint16_t footermark, const uint32_t gap,
                   const uint64_t data, const uint16_t nbits,
                   const uint16_t frequency, const bool MSBfirst,
                   const uint16_t repeat, const uint8_t dutycycle);
  void sendGeneric(const uint16_t headermark, const uint32_t headerspace,
                   const uint16_t onemark, const uint32_t onespace,
                   const uint16_t zeromark, const uint32_t zerospace,
                   const uint16_t footermark, const uint32_t gap,
                   const uint32_t mesgtime, const uint64_t data,
                   const uint16_t nbits, const uint16_t frequency,
                   const bool MSBfirst, const uint16_t repeat,
                   const uint8_t dutycycle);
  void sendGeneric(const uint16_t headermark, const uint32_t headerspace,
                   const uint16_t onemark, const uint32_t onespace,
                   const uint16_t zeromark, const uint32_t zerospace,
                   const uint16_t footermark, const uint32_t gap,
                   const uint8_t *dataptr, const uint16_t nbytes,
                   const uint16_t frequency, const bool MSBfirst,
                   const uint16_t repeat, const uint8_t dutycycle);
  static uint16_t minRepeats(const decode_type_t protocol);
  static uint16_t defaultBits(const decode_type_t protocol);
  bool send(const decode_type_t type, const uint64_t data,
            const uint16_t nbits, const uint16_t repeat = kNoRepeat);
  bool send(const decode_type_t type, const uint8_t state[],
            const uint16_t nbytes);
#if (SEND_NEC || SEND_SHERWOOD || SEND_AIWA_RC_T501 || SEND_SANYO)
  void sendNEC(uint64_t data, uint16_t nbits = kNECBits,
               uint16_t repeat = kNoRepeat);
  uint32_t encodeNEC(uint16_t address, uint16_t command);
#endif
#if SEND_SONY
  // sendSony() should typically be called with repeat=2 as Sony devices
  // expect the code to be sent at least 3 times. (code + 2 repeats = 3 codes)
  // Legacy use of this procedure was to only send a single code so call it with
  // repeat=0 for backward compatibility. As of v2.0 it defaults to sending
  // a Sony command that will be accepted be a device.
  void sendSony(uint64_t data, uint16_t nbits = kSony20Bits,
                uint16_t repeat = kSonyMinRepeat);
  void sendSony38(uint64_t data, uint16_t nbits = kSony20Bits,
                  uint16_t repeat = kSonyMinRepeat + 1);
  uint32_t encodeSony(uint16_t nbits, uint16_t command, uint16_t address,
                      uint16_t extended = 0);
#endif
#if SEND_SHERWOOD
  void sendSherwood(uint64_t data, uint16_t nbits = kSherwoodBits,
                    uint16_t repeat = kSherwoodMinRepeat);
#endif
#if SEND_SAMSUNG
  void sendSAMSUNG(const uint64_t data, const uint16_t nbits = kSamsungBits,
                   const uint16_t repeat = kNoRepeat);
  uint32_t encodeSAMSUNG(const uint8_t customer, const uint8_t command);
#endif
#if SEND_SAMSUNG36
  void sendSamsung36(const uint64_t data, const uint16_t nbits = kSamsung36Bits,
                     const uint16_t repeat = kNoRepeat);
#endif
#if SEND_SAMSUNG_AC
  void sendSamsungAC(const unsigned char data[],
                     const uint16_t nbytes = kSamsungAcStateLength,
                     const uint16_t repeat = kSamsungAcDefaultRepeat);
#endif
#if SEND_LG
  void sendLG(uint64_t data, uint16_t nbits = kLgBits,
              uint16_t repeat = kNoRepeat);
  void sendLG2(uint64_t data, uint16_t nbits = kLgBits,
               uint16_t repeat = kNoRepeat);
  uint32_t encodeLG(uint16_t address, uint16_t command);
#endif
#if (SEND_SHARP || SEND_DENON)
  uint32_t encodeSharp(const uint16_t address, const uint16_t command,
                       const uint16_t expansion = 1, const uint16_t check = 0,
                       const bool MSBfirst = false);
  void sendSharp(const uint16_t address, const uint16_t command,
                 const uint16_t nbits = kSharpBits,
                 const uint16_t repeat = kNoRepeat);
  void sendSharpRaw(const uint64_t data, const uint16_t nbits = kSharpBits,
                    const uint16_t repeat = kNoRepeat);
#endif
#if SEND_SHARP_AC
  void sendSharpAc(const unsigned char data[],
                   const uint16_t nbytes = kSharpAcStateLength,
                   const uint16_t repeat = kSharpAcDefaultRepeat);
#endif  // SEND_SHARP_AC
#if SEND_JVC
  void sendJVC(uint64_t data, uint16_t nbits = kJvcBits,
               uint16_t repeat = kNoRepeat);
  uint16_t encodeJVC(uint8_t address, uint8_t command);
#endif
#if SEND_DENON
  void sendDenon(uint64_t data, uint16_t nbits = kDenonBits,
                 uint16_t repeat = kNoRepeat);
#endif
#if SEND_SANYO
  uint64_t encodeSanyoLC7461(uint16_t address, uint8_t command);
  void sendSanyoLC7461(uint64_t data, uint16_t nbits = kSanyoLC7461Bits,
                       uint16_t repeat = kNoRepeat);
#endif
#if SEND_DISH
  // sendDISH() should typically be called with repeat=3 as DISH devices
  // expect the code to be sent at least 4 times. (code + 3 repeats = 4 codes)
  // Legacy use of this procedure was only to send a single code
  // so use repeat=0 for backward compatibility.
  void sendDISH(uint64_t data, uint16_t nbits = kDishBits,
                uint16_t repeat = kDishMinRepeat);
#endif
#if (SEND_PANASONIC || SEND_DENON)
  void sendPanasonic64(const uint64_t data,
                       const uint16_t nbits = kPanasonicBits,
                       const uint16_t repeat = kNoRepeat);
  void sendPanasonic(const uint16_t address, const uint32_t data,
                     const uint16_t nbits = kPanasonicBits,
                     const uint16_t repeat = kNoRepeat);
  uint64_t encodePanasonic(const uint16_t manufacturer, const uint8_t device,
                           const uint8_t subdevice, const uint8_t function);
#endif
#if SEND_RC5
  void sendRC5(uint64_t data, uint16_t nbits = kRC5XBits,
               uint16_t repeat = kNoRepeat);
  uint16_t encodeRC5(uint8_t address, uint8_t command,
                     bool key_released = false);
  uint16_t encodeRC5X(uint8_t address, uint8_t command,
                      bool key_released = false);
  uint64_t toggleRC5(uint64_t data);
#endif
#if SEND_RC6
  void sendRC6(uint64_t data, uint16_t nbits = kRC6Mode0Bits,
               uint16_t repeat = kNoRepeat);
  uint64_t encodeRC6(uint32_t address, uint8_t command,
                     uint16_t mode = kRC6Mode0Bits);
  uint64_t toggleRC6(uint64_t data, uint16_t nbits = kRC6Mode0Bits);
#endif
#if SEND_RCMM
  void sendRCMM(uint64_t data, uint16_t nbits = kRCMMBits,
                uint16_t repeat = kNoRepeat);
#endif
#if SEND_COOLIX
  void sendCOOLIX(uint64_t data, uint16_t nbits = kCoolixBits,
                  uint16_t repeat = kCoolixDefaultRepeat);
#endif
#if SEND_WHYNTER
  void sendWhynter(uint64_t data, uint16_t nbits = kWhynterBits,
                   uint16_t repeat = kNoRepeat);
#endif
#if SEND_MITSUBISHI
  void sendMitsubishi(uint64_t data, uint16_t nbits = kMitsubishiBits,
                      uint16_t repeat = kMitsubishiMinRepeat);
#endif
#if SEND_MITSUBISHI136
  void sendMitsubishi136(const unsigned char data[],
                         const uint16_t nbytes = kMitsubishi136StateLength,
                         const uint16_t repeat = kMitsubishi136MinRepeat);
#endif
#if SEND_MITSUBISHI112
  void sendMitsubishi112(const unsigned char data[],
                         const uint16_t nbytes = kMitsubishi112StateLength,
                         const uint16_t repeat = kMitsubishi112MinRepeat);
#endif
#if SEND_MITSUBISHI2
  void sendMitsubishi2(uint64_t data, uint16_t nbits = kMitsubishiBits,
                       uint16_t repeat = kMitsubishiMinRepeat);
#endif
#if SEND_MITSUBISHI_AC
  void sendMitsubishiAC(const unsigned char data[],
                        const uint16_t nbytes = kMitsubishiACStateLength,
                        const uint16_t repeat = kMitsubishiACMinRepeat);
#endif
#if SEND_MITSUBISHIHEAVY
  void sendMitsubishiHeavy88(
      const unsigned char data[],
      const uint16_t nbytes = kMitsubishiHeavy88StateLength,
      const uint16_t repeat = kMitsubishiHeavy88MinRepeat);
  void sendMitsubishiHeavy152(
      const unsigned char data[],
      const uint16_t nbytes = kMitsubishiHeavy152StateLength,
      const uint16_t repeat = kMitsubishiHeavy152MinRepeat);
#endif
#if SEND_FUJITSU_AC
  void sendFujitsuAC(const unsigned char data[], const uint16_t nbytes,
                     const uint16_t repeat = kFujitsuAcMinRepeat);
#endif
#if SEND_INAX
  void sendInax(const uint64_t data, const uint16_t nbits = kInaxBits,
                const uint16_t repeat = kInaxMinRepeat);
#endif  // SEND_INAX
#if SEND_GLOBALCACHE
  void sendGC(uint16_t buf[], uint16_t len);
#endif
#if SEND_KELVINATOR
  void sendKelvinator(const unsigned char data[],
                      const uint16_t nbytes = kKelvinatorStateLength,
                      const uint16_t repeat = kKelvinatorDefaultRepeat);
#endif
#if SEND_DAIKIN
  void sendDaikin(const unsigned char data[],
                  const uint16_t nbytes = kDaikinStateLength,
                  const uint16_t repeat = kDaikinDefaultRepeat);
#endif
#if SEND_DAIKIN128
  void sendDaikin128(const unsigned char data[],
                     const uint16_t nbytes = kDaikin128StateLength,
                     const uint16_t repeat = kDaikin128DefaultRepeat);
#endif  // SEND_DAIKIN128
#if SEND_DAIKIN152
  void sendDaikin152(const unsigned char data[],
                     const uint16_t nbytes = kDaikin152StateLength,
                     const uint16_t repeat = kDaikin152DefaultRepeat);
#endif  // SEND_DAIKIN152
#if SEND_DAIKIN160
  void sendDaikin160(const unsigned char data[],
                     const uint16_t nbytes = kDaikin160StateLength,
                     const uint16_t repeat = kDaikin160DefaultRepeat);
#endif  // SEND_DAIKIN160
#if SEND_DAIKIN176
  void sendDaikin176(const unsigned char data[],
                     const uint16_t nbytes = kDaikin176StateLength,
                     const uint16_t repeat = kDaikin176DefaultRepeat);
#endif  // SEND_DAIKIN176
#if SEND_DAIKIN2
  void sendDaikin2(const unsigned char data[],
                   const uint16_t nbytes = kDaikin2StateLength,
                   const uint16_t repeat = kDaikin2DefaultRepeat);
#endif
#if SEND_DAIKIN216
  void sendDaikin216(const unsigned char data[],
                     const uint16_t nbytes = kDaikin216StateLength,
                     const uint16_t repeat = kDaikin216DefaultRepeat);
#endif
#if SEND_AIWA_RC_T501
  void sendAiwaRCT501(uint64_t data, uint16_t nbits = kAiwaRcT501Bits,
                      uint16_t repeat = kAiwaRcT501MinRepeats);
#endif
#if SEND_GREE
  void sendGree(const uint64_t data, const uint16_t nbits = kGreeBits,
                const uint16_t repeat = kGreeDefaultRepeat);
  void sendGree(const uint8_t data[], const uint16_t nbytes = kGreeStateLength,
                const uint16_t repeat = kGreeDefaultRepeat);
#endif
#if SEND_GOODWEATHER
  void sendGoodweather(const uint64_t data,
                       const uint16_t nbits = kGoodweatherBits,
                       const uint16_t repeat = kGoodweatherMinRepeat);
#endif  // SEND_GOODWEATHER
#if SEND_PRONTO
  void sendPronto(uint16_t data[], uint16_t len, uint16_t repeat = kNoRepeat);
#endif
#if SEND_ARGO
  void sendArgo(const unsigned char data[],
                const uint16_t nbytes = kArgoStateLength,
                const uint16_t repeat = kArgoDefaultRepeat);
#endif
#if SEND_TROTEC
  void sendTrotec(const unsigned char data[],
                  const uint16_t nbytes = kTrotecStateLength,
                  const uint16_t repeat = kTrotecDefaultRepeat);
#endif
#if SEND_NIKAI
  void sendNikai(uint64_t data, uint16_t nbits = kNikaiBits,
                 uint16_t repeat = kNoRepeat);
#endif
#if SEND_TOSHIBA_AC
  void sendToshibaAC(const unsigned char data[],
                     const uint16_t nbytes = kToshibaACStateLength,
                     const uint16_t repeat = kToshibaACMinRepeat);
#endif
#if SEND_MIDEA
  void sendMidea(uint64_t data, uint16_t nbits = kMideaBits,
                 uint16_t repeat = kMideaMinRepeat);
#endif
#if SEND_MAGIQUEST
  void sendMagiQuest(uint64_t data, uint16_t nbits = kMagiquestBits,
                     uint16_t repeat = kNoRepeat);
  uint64_t encodeMagiQuest(uint32_t wand_id, uint16_t magnitude);
#endif
#if SEND_LASERTAG
  void sendLasertag(uint64_t data, uint16_t nbits = kLasertagBits,
                    uint16_t repeat = kLasertagMinRepeat);
#endif
#if SEND_CARRIER_AC
  void sendCarrierAC(uint64_t data, uint16_t nbits = kCarrierAcBits,
                     uint16_t repeat = kCarrierAcMinRepeat);
#endif
#if (SEND_HAIER_AC || SEND_HAIER_AC_YRW02)
  void sendHaierAC(const unsigned char data[],
                   const uint16_t nbytes = kHaierACStateLength,
                   const uint16_t repeat = kHaierAcDefaultRepeat);
#endif
#if SEND_HAIER_AC_YRW02
  void sendHaierACYRW02(const unsigned char data[],
                        const uint16_t nbytes = kHaierACYRW02StateLength,
                        const uint16_t repeat = kHaierAcYrw02DefaultRepeat);
#endif
#if SEND_HITACHI_AC
  void sendHitachiAC(const unsigned char data[],
                     const uint16_t nbytes = kHitachiAcStateLength,
                     const uint16_t repeat = kHitachiAcDefaultRepeat);
#endif
#if SEND_HITACHI_AC1
  void sendHitachiAC1(const unsigned char data[],
                      const uint16_t nbytes = kHitachiAc1StateLength,
                      const uint16_t repeat = kHitachiAcDefaultRepeat);
#endif
#if SEND_HITACHI_AC2
  void sendHitachiAC2(const unsigned char data[],
                      const uint16_t nbytes = kHitachiAc2StateLength,
                      const uint16_t repeat = kHitachiAcDefaultRepeat);
#endif
#if SEND_HITACHI_AC424
  void sendHitachiAc424(const unsigned char data[],
                        const uint16_t nbytes = kHitachiAc424StateLength,
                        const uint16_t repeat = kHitachiAcDefaultRepeat);
#endif  // SEND_HITACHI_AC424
#if SEND_GICABLE
  void sendGICable(uint64_t data, uint16_t nbits = kGicableBits,
                   uint16_t repeat = kGicableMinRepeat);
#endif
#if SEND_WHIRLPOOL_AC
  void sendWhirlpoolAC(const unsigned char data[],
                       const uint16_t nbytes = kWhirlpoolAcStateLength,
                       const uint16_t repeat = kWhirlpoolAcDefaultRepeat);
#endif
#if SEND_LUTRON
  void sendLutron(uint64_t data, uint16_t nbits = kLutronBits,
                  uint16_t repeat = kNoRepeat);
#endif
#if SEND_ELECTRA_AC
  void sendElectraAC(const unsigned char data[],
                     const uint16_t nbytes = kElectraAcStateLength,
                     const uint16_t repeat = kNoRepeat);
#endif
#if SEND_PANASONIC_AC
  void sendPanasonicAC(const unsigned char data[],
                       const uint16_t nbytes = kPanasonicAcStateLength,
                       const uint16_t repeat = kPanasonicAcDefaultRepeat);
#endif
#if SEND_PIONEER
  void sendPioneer(const uint64_t data, const uint16_t nbits = kPioneerBits,
                   const uint16_t repeat = kNoRepeat);
  uint64_t encodePioneer(uint16_t address, uint16_t command);
#endif
#if SEND_MWM
  void sendMWM(const unsigned char data[], const uint16_t nbytes,
               const uint16_t repeat = kNoRepeat);
#endif
#if SEND_VESTEL_AC
  void sendVestelAc(const uint64_t data, const uint16_t nbits = kVestelAcBits,
                    const uint16_t repeat = kNoRepeat);
#endif
#if SEND_TCL112AC
  void sendTcl112Ac(const unsigned char data[],
                    const uint16_t nbytes = kTcl112AcStateLength,
                    const uint16_t repeat = kTcl112AcDefaultRepeat);
#endif
#if SEND_TECO
  void sendTeco(const uint64_t data, const uint16_t nbits = kTecoBits,
                const uint16_t repeat = kNoRepeat);
#endif
#if SEND_LEGOPF
  void sendLegoPf(const uint64_t data, const uint16_t nbits = kLegoPfBits,
                  const uint16_t repeat = kLegoPfMinRepeat);
#endif
#if SEND_NEOCLIMA
  void sendNeoclima(const unsigned char data[],
                    const uint16_t nbytes = kNeoclimaStateLength,
                    const uint16_t repeat = kNeoclimaMinRepeat);
#endif  // SEND_NEOCLIMA
#if SEND_AMCOR
  void sendAmcor(const unsigned char data[],
                 const uint16_t nbytes = kAmcorStateLength,
                 const uint16_t repeat = kAmcorDefaultRepeat);
#endif  // SEND_AMCOR


 protected:
#ifdef UNIT_TEST
#ifndef HIGH
#define HIGH 0x1
#endif
#ifndef LOW
#define LOW 0x0
#endif
#endif  // UNIT_TEST
  uint8_t outputOn;
  uint8_t outputOff;
  VIRTUAL void ledOff();
  VIRTUAL void ledOn();
#ifndef UNIT_TEST

 private:
#else
  uint32_t _freq_unittest;
#endif  // UNIT_TEST
  uint16_t onTimePeriod;
  uint16_t offTimePeriod;
  uint16_t IRpin;
  int8_t periodOffset;
  uint8_t _dutycycle;
  bool modulation;
  uint32_t calcUSecPeriod(uint32_t hz, bool use_offset = true);
#if SEND_SONY
  void _sendSony(uint64_t data, uint16_t nbits,
                 uint16_t repeat, uint16_t freq);
#endif
};

#endif  // IRSEND_H_
