// Copyright 2009 Ken Shirriff
// Copyright 2015 Mark Szabo
// Copyright 2017 David Conran
#ifndef IRSEND_H_
#define IRSEND_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include "IRremoteESP8266.h"

// Originally from https://github.com/shirriff/Arduino-IRremote/
// Updated by markszabo (https://github.com/markszabo/IRremoteESP8266) for
// sending IR code on ESP8266

#if TEST || UNIT_TEST
#define VIRTUAL virtual
#else
#define VIRTUAL
#endif

// Constants
// Offset (in microseconds) to use in Period time calculations to account for
// code excution time in producing the software PWM signal.
// Value was calculated on Wemos D1 mini using v2.4.1 with v2.4.0 ESP core
const int8_t kPeriodOffset = -5;
const uint8_t kDutyDefault = 50;  // Percentage
const uint8_t kDutyMax = 100;     // Percentage
// delayMicroseconds() is only accurate to 16383us.
// Ref: https://www.arduino.cc/en/Reference/delayMicroseconds
const uint16_t kMaxAccurateUsecDelay = 16383;

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
  void send(uint16_t type, uint64_t data, uint16_t nbits);
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
  uint32_t encodeSony(uint16_t nbits, uint16_t command, uint16_t address,
                      uint16_t extended = 0);
#endif
#if SEND_SHERWOOD
  void sendSherwood(uint64_t data, uint16_t nbits = kSherwoodBits,
                    uint16_t repeat = kSherwoodMinRepeat);
#endif
#if SEND_SAMSUNG
  void sendSAMSUNG(uint64_t data, uint16_t nbits = kSamsungBits,
                   uint16_t repeat = kNoRepeat);
  uint32_t encodeSAMSUNG(uint8_t customer, uint8_t command);
#endif
#if SEND_SAMSUNG_AC
  void sendSamsungAC(unsigned char data[],
                     uint16_t nbytes = kSamsungAcStateLength,
                     uint16_t repeat = kNoRepeat);
#endif
#if SEND_LG
  void sendLG(uint64_t data, uint16_t nbits = kLgBits,
              uint16_t repeat = kNoRepeat);
  void sendLG2(uint64_t data, uint16_t nbits = kLgBits,
               uint16_t repeat = kNoRepeat);
  uint32_t encodeLG(uint16_t address, uint16_t command);
#endif
#if (SEND_SHARP || SEND_DENON)
  uint32_t encodeSharp(uint16_t address, uint16_t command,
                       uint16_t expansion = 1, uint16_t check = 0,
                       bool MSBfirst = false);
  void sendSharp(uint16_t address, uint16_t command,
                 uint16_t nbits = kSharpBits, uint16_t repeat = kNoRepeat);
  void sendSharpRaw(uint64_t data, uint16_t nbits = kSharpBits,
                    uint16_t repeat = kNoRepeat);
#endif
#if SEND_JVC
  void sendJVC(uint64_t data, uint16_t nbits = kJvcBits,
               uint16_t repeat = kNoRepeat);
  uint16_t encodeJVC(uint8_t address, uint8_t command);
#endif
#if SEND_DENON
  void sendDenon(uint64_t data, uint16_t nbits = DENON_BITS,
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
  void sendPanasonic64(uint64_t data, uint16_t nbits = kPanasonicBits,
                       uint16_t repeat = kNoRepeat);
  void sendPanasonic(uint16_t address, uint32_t data,
                     uint16_t nbits = kPanasonicBits,
                     uint16_t repeat = kNoRepeat);
  uint64_t encodePanasonic(uint16_t manufacturer, uint8_t device,
                           uint8_t subdevice, uint8_t function);
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
                  uint16_t repeat = kNoRepeat);
#endif
#if SEND_WHYNTER
  void sendWhynter(uint64_t data, uint16_t nbits = kWhynterBits,
                   uint16_t repeat = kNoRepeat);
#endif
#if SEND_MITSUBISHI
  void sendMitsubishi(uint64_t data, uint16_t nbits = kMitsubishiBits,
                      uint16_t repeat = kMitsubishiMinRepeat);
#endif
#if SEND_MITSUBISHI2
  void sendMitsubishi2(uint64_t data, uint16_t nbits = kMitsubishiBits,
                       uint16_t repeat = kMitsubishiMinRepeat);
#endif
#if SEND_MITSUBISHI_AC
  void sendMitsubishiAC(unsigned char data[],
                        uint16_t nbytes = kMitsubishiACStateLength,
                        uint16_t repeat = kMitsubishiACMinRepeat);
#endif
#if SEND_FUJITSU_AC
  void sendFujitsuAC(unsigned char data[], uint16_t nbytes,
                     uint16_t repeat = kFujitsuAcMinRepeat);
#endif
#if SEND_GLOBALCACHE
  void sendGC(uint16_t buf[], uint16_t len);
#endif
#if SEND_KELVINATOR
  void sendKelvinator(unsigned char data[],
                      uint16_t nbytes = kKelvinatorStateLength,
                      uint16_t repeat = kNoRepeat);
#endif
#if SEND_DAIKIN
  void sendDaikin(unsigned char data[], uint16_t nbytes = kDaikinStateLength,
                  uint16_t repeat = kNoRepeat);
  void sendDaikinGapHeader();
#endif
#if SEND_AIWA_RC_T501
  void sendAiwaRCT501(uint64_t data, uint16_t nbits = kAiwaRcT501Bits,
                      uint16_t repeat = kAiwaRcT501MinRepeats);
#endif
#if SEND_GREE
  void sendGree(uint64_t data, uint16_t nbits = kGreeBits,
                uint16_t repeat = kNoRepeat);
  void sendGree(uint8_t data[], uint16_t nbytes = kGreeStateLength,
                uint16_t repeat = kNoRepeat);
#endif
#if SEND_PRONTO
  void sendPronto(uint16_t data[], uint16_t len, uint16_t repeat = kNoRepeat);
#endif
#if SEND_ARGO
  void sendArgo(unsigned char data[], uint16_t nbytes = kArgoStateLength,
                uint16_t repeat = kNoRepeat);
#endif
#if SEND_TROTEC
  void sendTrotec(unsigned char data[], uint16_t nbytes = kTrotecStateLength,
                  uint16_t repeat = kNoRepeat);
#endif
#if SEND_NIKAI
  void sendNikai(uint64_t data, uint16_t nbits = kNikaiBits,
                 uint16_t repeat = kNoRepeat);
#endif
#if SEND_TOSHIBA_AC
  void sendToshibaAC(unsigned char data[],
                     uint16_t nbytes = kToshibaACStateLength,
                     uint16_t repeat = kToshibaACMinRepeat);
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
  void sendHaierAC(unsigned char data[], uint16_t nbytes = kHaierACStateLength,
                   uint16_t repeat = kNoRepeat);
#endif
#if SEND_HAIER_AC_YRW02
  void sendHaierACYRW02(unsigned char data[],
                        uint16_t nbytes = kHaierACYRW02StateLength,
                        uint16_t repeat = kNoRepeat);
#endif
#if SEND_HITACHI_AC
  void sendHitachiAC(unsigned char data[],
                     uint16_t nbytes = kHitachiAcStateLength,
                     uint16_t repeat = kNoRepeat);
#endif
#if SEND_HITACHI_AC1
  void sendHitachiAC1(unsigned char data[],
                      uint16_t nbytes = kHitachiAc1StateLength,
                      uint16_t repeat = kNoRepeat);
#endif
#if SEND_HITACHI_AC2
  void sendHitachiAC2(unsigned char data[],
                      uint16_t nbytes = kHitachiAc2StateLength,
                      uint16_t repeat = kNoRepeat);
#endif
#if SEND_GICABLE
  void sendGICable(uint64_t data, uint16_t nbits = kGicableBits,
                   uint16_t repeat = kGicableMinRepeat);
#endif
#if SEND_WHIRLPOOL_AC
  void sendWhirlpoolAC(unsigned char data[],
                       uint16_t nbytes = kWhirlpoolAcStateLength,
                       uint16_t repeat = kNoRepeat);
#endif
#if SEND_LUTRON
  void sendLutron(uint64_t data, uint16_t nbits = kLutronBits,
                  uint16_t repeat = kNoRepeat);
#endif
#if SEND_ELECTRA_AC
  void sendElectraAC(unsigned char data[],
                     uint16_t nbytes = kElectraAcStateLength,
                     uint16_t repeat = kNoRepeat);
#endif
#if SEND_PANASONIC_AC
  void sendPanasonicAC(unsigned char data[],
                       uint16_t nbytes = kPanasonicAcStateLength,
                       uint16_t repeat = kNoRepeat);
#endif
#if SEND_PIONEER
  void sendPioneer(const uint64_t data, const uint16_t nbits = kPioneerBits,
                   const uint16_t repeat = kNoRepeat);
  uint64_t encodePioneer(uint16_t address, uint16_t command);
#endif
#if SEND_MWM
  void sendMWM(unsigned char data[], uint16_t nbytes,
               uint16_t repeat = kNoRepeat);
#endif

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

 private:
  uint16_t onTimePeriod;
  uint16_t offTimePeriod;
  uint16_t IRpin;
  int8_t periodOffset;
  uint8_t _dutycycle;
  bool modulation;
  uint32_t calcUSecPeriod(uint32_t hz, bool use_offset = true);
};

#endif  // IRSEND_H_
