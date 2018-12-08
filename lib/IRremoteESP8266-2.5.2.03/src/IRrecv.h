// Copyright 2009 Ken Shirriff
// Copyright 2015 Mark Szabo
// Copyright 2015 Sebastien Warin
// Copyright 2017 David Conran

#ifndef IRRECV_H_
#define IRRECV_H_

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <stddef.h>
#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include "IRremoteESP8266.h"

// Constants
const uint16_t kHeader = 2;        // Usual nr. of header entries.
const uint16_t kFooter = 2;        // Usual nr. of footer (stop bits) entries.
const uint16_t kStartOffset = 1;   // Usual rawbuf entry to start from.
#define MS_TO_USEC(x) (x * 1000U)  // Convert milli-Seconds to micro-Seconds.
// Marks tend to be 100us too long, and spaces 100us too short
// when received due to sensor lag.
const uint16_t kMarkExcess = 50;
const uint16_t kRawBuf = 100;  // Default length of raw capture buffer
const uint64_t kRepeat = UINT64_MAX;
// Default min size of reported UNKNOWN messages.
const uint16_t kUnknownThreshold = 6;

// receiver states
const uint8_t kIdleState = 2;
const uint8_t kMarkState = 3;
const uint8_t kSpaceState = 4;
const uint8_t kStopState = 5;
const uint8_t kTolerance = 25;  // default percent tolerance in measurements.
const uint16_t kRawTick = 2;    // Capture tick to uSec factor.
#define RAWTICK kRawTick  // Deprecated. For legacy user code support only.
// How long (ms) before we give up wait for more data?
// Don't exceed kMaxTimeoutMs without a good reason.
// That is the capture buffers maximum value size. (UINT16_MAX / kRawTick)
// Typically messages/protocols tend to repeat around the 100ms timeframe,
// thus we should timeout before that to give us some time to try to decode
// before we need to start capturing a possible new message.
// Typically 15ms suits most applications. However, some protocols demand a
// higher value. e.g. 90ms for XMP-1 and some aircon units.
const uint8_t kTimeoutMs = 15;  // In MilliSeconds.
#define TIMEOUT_MS kTimeoutMs   // For legacy documentation.
const uint16_t kMaxTimeoutMs = kRawTick * (UINT16_MAX / MS_TO_USEC(1));

// Use FNV hash algorithm: http://isthe.com/chongo/tech/comp/fnv/#FNV-param
const uint32_t kFnvPrime32 = 16777619UL;
const uint32_t kFnvBasis32 = 2166136261UL;

#if DECODE_AC
// Hitachi AC is the current largest state size.
const uint16_t kStateSizeMax = kHitachiAc2StateLength;
#else
// Just define something
const uint16_t kStateSizeMax = 0;
#endif

// Types
// information for the interrupt handler
typedef struct {
  uint8_t recvpin;   // pin for IR data from detector
  uint8_t rcvstate;  // state machine
  uint16_t timer;    // state timer, counts 50uS ticks.
  uint16_t bufsize;  // max. nr. of entries in the capture buffer.
  uint16_t *rawbuf;  // raw data
  // uint16_t is used for rawlen as it saves 3 bytes of iram in the interrupt
  // handler. Don't ask why, I don't know. It just does.
  uint16_t rawlen;   // counter of entries in rawbuf.
  uint8_t overflow;  // Buffer overflow indicator.
  uint8_t timeout;   // Nr. of milliSeconds before we give up.
} irparams_t;

// results from a data match
typedef struct {
  bool success;   // Was the match successful?
  uint64_t data;  // The data found.
  uint16_t used;  // How many buffer positions were used.
} match_result_t;

// Classes

// Results returned from the decoder
class decode_results {
 public:
  decode_type_t decode_type;  // NEC, SONY, RC5, UNKNOWN
  // value, address, & command are all mutually exclusive with state.
  // i.e. They MUST NOT be used at the same time as state, so we can use a union
  // structure to save us a handful of valuable bytes of memory.
  union {
    struct {
      uint64_t value;    // Decoded value
      uint32_t address;  // Decoded device address.
      uint32_t command;  // Decoded command.
    };
    uint8_t state[kStateSizeMax];  // Multi-byte results.
  };
  uint16_t bits;              // Number of bits in decoded value
  volatile uint16_t *rawbuf;  // Raw intervals in .5 us ticks
  uint16_t rawlen;            // Number of records in rawbuf.
  bool overflow;
  bool repeat;  // Is the result a repeat code?
};

// main class for receiving IR
class IRrecv {
 public:
  explicit IRrecv(uint16_t recvpin, uint16_t bufsize = kRawBuf,
                  uint8_t timeout = kTimeoutMs,
                  bool save_buffer = false);  // Constructor
  ~IRrecv();                                  // Destructor
  bool decode(decode_results *results, irparams_t *save = NULL);
  void enableIRIn();
  void disableIRIn();
  void resume();
  uint16_t getBufSize();
#if DECODE_HASH
  void setUnknownThreshold(uint16_t length);
#endif
  static bool match(uint32_t measured, uint32_t desired,
                    uint8_t tolerance = kTolerance, uint16_t delta = 0);
  static bool matchMark(uint32_t measured, uint32_t desired,
                        uint8_t tolerance = kTolerance,
                        int16_t excess = kMarkExcess);
  static bool matchSpace(uint32_t measured, uint32_t desired,
                         uint8_t tolerance = kTolerance,
                         int16_t excess = kMarkExcess);
#ifndef UNIT_TEST

 private:
#endif
  irparams_t *irparams_save;
#if DECODE_HASH
  uint16_t unknown_threshold;
#endif
  // These are called by decode
  void copyIrParams(volatile irparams_t *src, irparams_t *dst);
  int16_t compare(uint16_t oldval, uint16_t newval);
  static uint32_t ticksLow(uint32_t usecs, uint8_t tolerance = kTolerance,
                           uint16_t delta = 0);
  static uint32_t ticksHigh(uint32_t usecs, uint8_t tolerance = kTolerance,
                            uint16_t delta = 0);
  bool matchAtLeast(uint32_t measured, uint32_t desired,
                    uint8_t tolerance = kTolerance, uint16_t delta = 0);
  match_result_t matchData(volatile uint16_t *data_ptr, const uint16_t nbits,
                           const uint16_t onemark, const uint32_t onespace,
                           const uint16_t zeromark, const uint32_t zerospace,
                           const uint8_t tolerance = kTolerance,
                           const int16_t excess = kMarkExcess,
                           const bool MSBfirst = true);
  bool decodeHash(decode_results *results);
#if (DECODE_NEC || DECODE_SHERWOOD || DECODE_AIWA_RC_T501 || SEND_SANYO)
  bool decodeNEC(decode_results *results, uint16_t nbits = kNECBits,
                 bool strict = true);
#endif
#if DECODE_SONY
  bool decodeSony(decode_results *results, uint16_t nbits = kSonyMinBits,
                  bool strict = false);
#endif
#if DECODE_SANYO
  // DISABLED due to poor quality.
  // bool decodeSanyo(decode_results *results,
  //                  uint16_t nbits = kSanyoSA8650BBits,
  //                  bool strict = false);
  bool decodeSanyoLC7461(decode_results *results,
                         uint16_t nbits = kSanyoLC7461Bits, bool strict = true);
#endif
#if DECODE_MITSUBISHI
  bool decodeMitsubishi(decode_results *results,
                        uint16_t nbits = kMitsubishiBits, bool strict = true);
#endif
#if DECODE_MITSUBISHI2
  bool decodeMitsubishi2(decode_results *results,
                         uint16_t nbits = kMitsubishiBits, bool strict = true);
#endif
#if DECODE_MITSUBISHI_AC
  bool decodeMitsubishiAC(decode_results *results,
                          uint16_t nbits = kMitsubishiACBits,
                          bool strict = false);
#endif
#if (DECODE_RC5 || DECODE_R6 || DECODE_LASERTAG || DECODE_MWM)
  int16_t getRClevel(decode_results *results, uint16_t *offset, uint16_t *used,
                     uint16_t bitTime, uint8_t tolerance = kTolerance,
                     int16_t excess = kMarkExcess, uint16_t delta = 0,
                     uint8_t maxwidth = 3);
#endif
#if DECODE_RC5
  bool decodeRC5(decode_results *results, uint16_t nbits = kRC5XBits,
                 bool strict = true);
#endif
#if DECODE_RC6
  bool decodeRC6(decode_results *results, uint16_t nbits = kRC6Mode0Bits,
                 bool strict = false);
#endif
#if DECODE_RCMM
  bool decodeRCMM(decode_results *results, uint16_t nbits = kRCMMBits,
                  bool strict = false);
#endif
#if (DECODE_PANASONIC || DECODE_DENON)
  bool decodePanasonic(decode_results *results, uint16_t nbits = kPanasonicBits,
                       bool strict = false,
                       uint32_t manufacturer = kPanasonicManufacturer);
#endif
#if DECODE_LG
  bool decodeLG(decode_results *results, uint16_t nbits = kLgBits,
                bool strict = false);
#endif
#if DECODE_JVC
  bool decodeJVC(decode_results *results, uint16_t nbits = kJvcBits,
                 bool strict = true);
#endif
#if DECODE_SAMSUNG
  bool decodeSAMSUNG(decode_results *results, uint16_t nbits = kSamsungBits,
                     bool strict = true);
#endif
#if DECODE_SAMSUNG_AC
  bool decodeSamsungAC(decode_results *results, uint16_t nbits = kSamsungAcBits,
                       bool strict = true);
#endif
#if DECODE_WHYNTER
  bool decodeWhynter(decode_results *results, uint16_t nbits = kWhynterBits,
                     bool strict = true);
#endif
#if DECODE_COOLIX
  bool decodeCOOLIX(decode_results *results, uint16_t nbits = kCoolixBits,
                    bool strict = true);
#endif
#if DECODE_DENON
  bool decodeDenon(decode_results *results, uint16_t nbits = DENON_BITS,
                   bool strict = true);
#endif
#if DECODE_DISH
  bool decodeDISH(decode_results *results, uint16_t nbits = kDishBits,
                  bool strict = true);
#endif
#if (DECODE_SHARP || DECODE_DENON)
  bool decodeSharp(decode_results *results, uint16_t nbits = kSharpBits,
                   bool strict = true, bool expansion = true);
#endif
#if DECODE_AIWA_RC_T501
  bool decodeAiwaRCT501(decode_results *results,
                        uint16_t nbits = kAiwaRcT501Bits, bool strict = true);
#endif
#if DECODE_NIKAI
  bool decodeNikai(decode_results *results, uint16_t nbits = kNikaiBits,
                   bool strict = true);
#endif
#if DECODE_MAGIQUEST
  bool decodeMagiQuest(decode_results *results, uint16_t nbits = kMagiquestBits,
                       bool strict = true);
#endif
#if DECODE_KELVINATOR
  bool decodeKelvinator(decode_results *results,
                        uint16_t nbits = kKelvinatorBits, bool strict = true);
#endif
#if DECODE_DAIKIN
  bool decodeDaikin(decode_results *results, uint16_t nbits = kDaikinRawBits,
                    bool strict = true);
#endif
#if DECODE_TOSHIBA_AC
  bool decodeToshibaAC(decode_results *results,
                       uint16_t nbytes = kToshibaACBits, bool strict = true);
#endif
#if DECODE_MIDEA
  bool decodeMidea(decode_results *results, uint16_t nbits = kMideaBits,
                   bool strict = true);
#endif
#if DECODE_FUJITSU_AC
  bool decodeFujitsuAC(decode_results *results, uint16_t nbits = kFujitsuAcBits,
                       bool strict = false);
#endif
#if DECODE_LASERTAG
  bool decodeLasertag(decode_results *results, uint16_t nbits = kLasertagBits,
                      bool strict = true);
#endif
#if DECODE_CARRIER_AC
  bool decodeCarrierAC(decode_results *results, uint16_t nbits = kCarrierAcBits,
                       bool strict = true);
#endif
#if DECODE_GREE
  bool decodeGree(decode_results *results, uint16_t nbits = kGreeBits,
                  bool strict = true);
#endif
#if (DECODE_HAIER_AC | DECODE_HAIER_AC_YRW02)
  bool decodeHaierAC(decode_results *results, uint16_t nbits = kHaierACBits,
                     bool strict = true);
#endif
#if DECODE_HAIER_AC_YRW02
  bool decodeHaierACYRW02(decode_results *results,
                          uint16_t nbits = kHaierACYRW02Bits,
                          bool strict = true);
#endif
#if (DECODE_HITACHI_AC || DECODE_HITACHI_AC2)
  bool decodeHitachiAC(decode_results *results, uint16_t nbits = kHitachiAcBits,
                       bool strict = true);
#endif
#if DECODE_HITACHI_AC1
  bool decodeHitachiAC1(decode_results *results,
                        uint16_t nbits = kHitachiAc1Bits, bool strict = true);
#endif
#if DECODE_GICABLE
  bool decodeGICable(decode_results *results, uint16_t nbits = kGicableBits,
                     bool strict = true);
#endif
#if DECODE_WHIRLPOOL_AC
  bool decodeWhirlpoolAC(decode_results *results,
                         uint16_t nbits = kWhirlpoolAcBits, bool strict = true);
#endif
#if DECODE_LUTRON
  bool decodeLutron(decode_results *results, uint16_t nbits = kLutronBits,
                    bool strict = true);
#endif
#if DECODE_ELECTRA_AC
  bool decodeElectraAC(decode_results *results, uint16_t nbits = kElectraAcBits,
                       bool strict = true);
#endif
#if DECODE_PANASONIC_AC
  bool decodePanasonicAC(decode_results *results,
                         uint16_t nbits = kPanasonicAcBits, bool strict = true);
#endif
#if DECODE_PIONEER
  bool decodePioneer(decode_results *results,
                     const uint16_t nbits = kPioneerBits,
                     const bool strict = true);
#endif
#if DECODE_MWM
  bool decodeMWM(decode_results *results, uint16_t nbits = 24,
                 bool strict = true);
#endif
};

#endif  // IRRECV_H_
