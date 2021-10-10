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
const uint8_t kTolerance = 25;   // default percent tolerance in measurements.
const uint8_t kUseDefTol = 255;  // Indicate to use the class default tolerance.
const uint16_t kRawTick = 2;     // Capture tick to uSec factor.
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

// Which of the ESP32 timers to use by default. (0-3)
const uint8_t kDefaultESP32Timer = 3;

#if DECODE_AC
// Hitachi AC is the current largest state size.
const uint16_t kStateSizeMax = kHitachiAc2StateLength;
#else  // DECODE_AC
// Just define something (a uint64_t)
const uint16_t kStateSizeMax = sizeof(uint64_t);
#endif  // DECODE_AC

// Types

/// Information for the interrupt handler
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

/// Results from a data match
typedef struct {
  bool success;   // Was the match successful?
  uint64_t data;  // The data found.
  uint16_t used;  // How many buffer positions were used.
} match_result_t;

// Classes

/// Results returned from the decoder
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

/// Class for receiving IR messages.
class IRrecv {
 public:
#if defined(ESP32)
  explicit IRrecv(const uint16_t recvpin, const uint16_t bufsize = kRawBuf,
                  const uint8_t timeout = kTimeoutMs,
                  const bool save_buffer = false,
                  const uint8_t timer_num = kDefaultESP32Timer);  // Constructor
#else  // ESP32
  explicit IRrecv(const uint16_t recvpin, const uint16_t bufsize = kRawBuf,
                  const uint8_t timeout = kTimeoutMs,
                  const bool save_buffer = false);                // Constructor
#endif  // ESP32
  ~IRrecv(void);                                                  // Destructor
  void setTolerance(const uint8_t percent = kTolerance);
  uint8_t getTolerance(void);
  bool decode(decode_results *results, irparams_t *save = NULL,
              uint8_t max_skip = 0, uint16_t noise_floor = 0);
  void enableIRIn(const bool pullup = false);
  void disableIRIn(void);
  void resume(void);
  uint16_t getBufSize(void);
#if DECODE_HASH
  void setUnknownThreshold(const uint16_t length);
#endif
  bool match(const uint32_t measured, const uint32_t desired,
             const uint8_t tolerance = kUseDefTol,
             const uint16_t delta = 0);
  bool matchMark(const uint32_t measured, const uint32_t desired,
                 const uint8_t tolerance = kUseDefTol,
                 const int16_t excess = kMarkExcess);
  bool matchMarkRange(const uint32_t measured, const uint32_t desired,
                      const uint16_t range = 100,
                      const int16_t excess = kMarkExcess);
  bool matchSpace(const uint32_t measured, const uint32_t desired,
                  const uint8_t tolerance = kUseDefTol,
                  const int16_t excess = kMarkExcess);
  bool matchSpaceRange(const uint32_t measured, const uint32_t desired,
                       const uint16_t range = 100,
                       const int16_t excess = kMarkExcess);
#ifndef UNIT_TEST

 private:
#endif
  irparams_t *irparams_save;
  uint8_t _tolerance;
#if defined(ESP32)
  uint8_t _timer_num;
#endif  // defined(ESP32)
#if DECODE_HASH
  uint16_t _unknown_threshold;
#endif
#ifdef UNIT_TEST
  volatile irparams_t *_getParamsPtr(void);
#endif  // UNIT_TEST
  // These are called by decode
  uint8_t _validTolerance(const uint8_t percentage);
  void copyIrParams(volatile irparams_t *src, irparams_t *dst);
  uint16_t compare(const uint16_t oldval, const uint16_t newval);
  uint32_t ticksLow(const uint32_t usecs,
                    const uint8_t tolerance = kUseDefTol,
                    const uint16_t delta = 0);
  uint32_t ticksHigh(const uint32_t usecs,
                     const uint8_t tolerance = kUseDefTol,
                     const uint16_t delta = 0);
  bool matchAtLeast(const uint32_t measured, const uint32_t desired,
                    const uint8_t tolerance = kUseDefTol,
                    const uint16_t delta = 0);
  uint16_t _matchGeneric(volatile uint16_t *data_ptr,
                         uint64_t *result_bits_ptr,
                         uint8_t *result_ptr,
                         const bool use_bits,
                         const uint16_t remaining,
                         const uint16_t required,
                         const uint16_t hdrmark,
                         const uint32_t hdrspace,
                         const uint16_t onemark,
                         const uint32_t onespace,
                         const uint16_t zeromark,
                         const uint32_t zerospace,
                         const uint16_t footermark,
                         const uint32_t footerspace,
                         const bool atleast = false,
                         const uint8_t tolerance = kUseDefTol,
                         const int16_t excess = kMarkExcess,
                         const bool MSBfirst = true);
  match_result_t matchData(volatile uint16_t *data_ptr, const uint16_t nbits,
                           const uint16_t onemark, const uint32_t onespace,
                           const uint16_t zeromark, const uint32_t zerospace,
                           const uint8_t tolerance = kUseDefTol,
                           const int16_t excess = kMarkExcess,
                           const bool MSBfirst = true,
                           const bool expectlastspace = true);
  uint16_t matchBytes(volatile uint16_t *data_ptr, uint8_t *result_ptr,
                      const uint16_t remaining, const uint16_t nbytes,
                      const uint16_t onemark, const uint32_t onespace,
                      const uint16_t zeromark, const uint32_t zerospace,
                      const uint8_t tolerance = kUseDefTol,
                      const int16_t excess = kMarkExcess,
                      const bool MSBfirst = true,
                      const bool expectlastspace = true);
  uint16_t matchGeneric(volatile uint16_t *data_ptr,
                        uint64_t *result_ptr,
                        const uint16_t remaining, const uint16_t nbits,
                        const uint16_t hdrmark, const uint32_t hdrspace,
                        const uint16_t onemark, const uint32_t onespace,
                        const uint16_t zeromark, const uint32_t zerospace,
                        const uint16_t footermark, const uint32_t footerspace,
                        const bool atleast = false,
                        const uint8_t tolerance = kUseDefTol,
                        const int16_t excess = kMarkExcess,
                        const bool MSBfirst = true);
  uint16_t matchGeneric(volatile uint16_t *data_ptr, uint8_t *result_ptr,
                        const uint16_t remaining, const uint16_t nbits,
                        const uint16_t hdrmark, const uint32_t hdrspace,
                        const uint16_t onemark, const uint32_t onespace,
                        const uint16_t zeromark, const uint32_t zerospace,
                        const uint16_t footermark,
                        const uint32_t footerspace,
                        const bool atleast = false,
                        const uint8_t tolerance = kUseDefTol,
                        const int16_t excess = kMarkExcess,
                        const bool MSBfirst = true);
  uint16_t matchGenericConstBitTime(volatile uint16_t *data_ptr,
                                    uint64_t *result_ptr,
                                    const uint16_t remaining,
                                    const uint16_t nbits,
                                    const uint16_t hdrmark,
                                    const uint32_t hdrspace,
                                    const uint16_t one,
                                    const uint32_t zero,
                                    const uint16_t footermark,
                                    const uint32_t footerspace,
                                    const bool atleast = false,
                                    const uint8_t tolerance = kUseDefTol,
                                    const int16_t excess = kMarkExcess,
                                    const bool MSBfirst = true);
  uint16_t matchManchesterData(volatile const uint16_t *data_ptr,
                               uint64_t *result_ptr,
                               const uint16_t remaining,
                               const uint16_t nbits,
                               const uint16_t half_period,
                               const uint16_t starting_balance = 0,
                               const uint8_t tolerance = kUseDefTol,
                               const int16_t excess = kMarkExcess,
                               const bool MSBfirst = true,
                               const bool GEThomas = true);
  uint16_t matchManchester(volatile const uint16_t *data_ptr,
                           uint64_t *result_ptr,
                           const uint16_t remaining,
                           const uint16_t nbits,
                           const uint16_t hdrmark,
                           const uint32_t hdrspace,
                           const uint16_t clock_period,
                           const uint16_t footermark,
                           const uint32_t footerspace,
                           const bool atleast = false,
                           const uint8_t tolerance = kUseDefTol,
                           const int16_t excess = kMarkExcess,
                           const bool MSBfirst = true,
                           const bool GEThomas = true);
  void crudeNoiseFilter(decode_results *results, const uint16_t floor = 0);
  bool decodeHash(decode_results *results);
#if DECODE_VOLTAS
  bool decodeVoltas(decode_results *results,
                         uint16_t offset = kStartOffset,
                         const uint16_t nbits = kVoltasBits,
                         const bool strict = true);
#endif  // DECODE_VOLTAS
#if (DECODE_NEC || DECODE_SHERWOOD || DECODE_AIWA_RC_T501 || DECODE_SANYO)
  bool decodeNEC(decode_results *results, uint16_t offset = kStartOffset,
                 const uint16_t nbits = kNECBits, const bool strict = true);
#endif
#if DECODE_ARGO
  bool decodeArgo(decode_results *results, uint16_t offset = kStartOffset,
                  const uint16_t nbits = kArgoBits, const bool strict = true);
#endif  // DECODE_ARGO
#if DECODE_SONY
  bool decodeSony(decode_results *results, uint16_t offset = kStartOffset,
                  const uint16_t nbits = kSonyMinBits,
                  const bool strict = false);
#endif
#if DECODE_SANYO
  // DISABLED due to poor quality.
  // bool decodeSanyo(decode_results *results, uint16_t offset = kStartOffset,
  //                  uint16_t nbits = kSanyoSA8650BBits,
  //                  bool strict = false);
  bool decodeSanyoLC7461(decode_results *results,
                         uint16_t offset = kStartOffset,
                         const uint16_t nbits = kSanyoLC7461Bits,
                         const bool strict = true);
#endif
#if DECODE_SANYO_AC
  bool decodeSanyoAc(decode_results *results,
                     uint16_t offset = kStartOffset,
                     const uint16_t nbits = kSanyoAcBits,
                     const bool strict = true);
#endif  // DECODE_SANYO_AC
#if DECODE_SANYO_AC88
  bool decodeSanyoAc88(decode_results *results,
                       uint16_t offset = kStartOffset,
                       const uint16_t nbits = kSanyoAc88Bits,
                       const bool strict = true);
#endif  // DECODE_SANYO_AC88
#if DECODE_MITSUBISHI
  bool decodeMitsubishi(decode_results *results, uint16_t offset = kStartOffset,
                        const uint16_t nbits = kMitsubishiBits,
                        const bool strict = true);
#endif
#if DECODE_MITSUBISHI2
  bool decodeMitsubishi2(decode_results *results,
                         uint16_t offset = kStartOffset,
                         const uint16_t nbits = kMitsubishiBits,
                         const bool strict = true);
#endif
#if DECODE_MITSUBISHI_AC
  bool decodeMitsubishiAC(decode_results *results,
                          uint16_t offset = kStartOffset,
                          const uint16_t nbits = kMitsubishiACBits,
                          const bool strict = false);
#endif
#if DECODE_MITSUBISHI136
  bool decodeMitsubishi136(decode_results *results,
                           uint16_t offset = kStartOffset,
                           const uint16_t nbits = kMitsubishi136Bits,
                           const bool strict = true);
#endif
#if DECODE_MITSUBISHI112
  bool decodeMitsubishi112(decode_results *results,
                           uint16_t offset = kStartOffset,
                           const uint16_t nbits = kMitsubishi112Bits,
                           const bool strict = true);
#endif
#if DECODE_MITSUBISHIHEAVY
  bool decodeMitsubishiHeavy(decode_results *results,
                             uint16_t offset = kStartOffset,
                             const uint16_t nbits = kMitsubishiHeavy152Bits,
                             const bool strict = true);
#endif
#if (DECODE_RC5 || DECODE_RC6 || DECODE_LASERTAG || DECODE_MWM)
  int16_t getRClevel(decode_results *results, uint16_t *offset, uint16_t *used,
                     uint16_t bitTime, const uint8_t tolerance = kUseDefTol,
                     const int16_t excess = kMarkExcess,
                     const uint16_t delta = 0, const uint8_t maxwidth = 3);
#endif
#if DECODE_RC5
  bool decodeRC5(decode_results *results, uint16_t offset = kStartOffset,
                 const uint16_t nbits = kRC5XBits,
                 const bool strict = true);
#endif
#if DECODE_RC6
  bool decodeRC6(decode_results *results, uint16_t offset = kStartOffset,
                 const uint16_t nbits = kRC6Mode0Bits,
                 const bool strict = false);
#endif
#if DECODE_RCMM
  bool decodeRCMM(decode_results *results, uint16_t offset = kStartOffset,
                  const uint16_t nbits = kRCMMBits,
                  const bool strict = false);
#endif
#if (DECODE_PANASONIC || DECODE_DENON)
  bool decodePanasonic(decode_results *results, uint16_t offset = kStartOffset,
                       const uint16_t nbits = kPanasonicBits,
                       const bool strict = false,
                       const uint32_t manufacturer = kPanasonicManufacturer);
#endif
#if DECODE_LG
  bool decodeLG(decode_results *results, uint16_t offset = kStartOffset,
                const uint16_t nbits = kLgBits,
                const bool strict = false);
#endif
#if DECODE_INAX
  bool decodeInax(decode_results *results, uint16_t offset = kStartOffset,
                  const uint16_t nbits = kInaxBits,
                  const bool strict = true);
#endif  // DECODE_INAX
#if DECODE_JVC
  bool decodeJVC(decode_results *results, uint16_t offset = kStartOffset,
                 const uint16_t nbits = kJvcBits,
                 const bool strict = true);
#endif
#if DECODE_SAMSUNG
  bool decodeSAMSUNG(decode_results *results, uint16_t offset = kStartOffset,
                     const uint16_t nbits = kSamsungBits,
                     const bool strict = true);
#endif
#if DECODE_SAMSUNG
  bool decodeSamsung36(decode_results *results, uint16_t offset = kStartOffset,
                       const uint16_t nbits = kSamsung36Bits,
                       const bool strict = true);
#endif
#if DECODE_SAMSUNG_AC
  bool decodeSamsungAC(decode_results *results, uint16_t offset = kStartOffset,
                       const uint16_t nbits = kSamsungAcBits,
                       const bool strict = true);
#endif
#if DECODE_WHYNTER
  bool decodeWhynter(decode_results *results, uint16_t offset = kStartOffset,
                     const uint16_t nbits = kWhynterBits,
                     const bool strict = true);
#endif
#if DECODE_COOLIX
  bool decodeCOOLIX(decode_results *results, uint16_t offset = kStartOffset,
                    const uint16_t nbits = kCoolixBits,
                    const bool strict = true);
#endif
#if DECODE_DENON
  bool decodeDenon(decode_results *results, uint16_t offset = kStartOffset,
                   const uint16_t nbits = kDenonBits,
                   const bool strict = true);
#endif
#if DECODE_DISH
  bool decodeDISH(decode_results *results, uint16_t offset = kStartOffset,
                  const uint16_t nbits = kDishBits,
                  const bool strict = true);
#endif
#if (DECODE_SHARP || DECODE_DENON)
  bool decodeSharp(decode_results *results, uint16_t offset = kStartOffset,
                   const uint16_t nbits = kSharpBits,
                   const bool strict = true, const bool expansion = true);
#endif
#if DECODE_SHARP_AC
  bool decodeSharpAc(decode_results *results, uint16_t offset = kStartOffset,
                     const uint16_t nbits = kSharpAcBits,
                     const bool strict = true);
#endif
#if DECODE_AIWA_RC_T501
  bool decodeAiwaRCT501(decode_results *results, uint16_t offset = kStartOffset,
                        const uint16_t nbits = kAiwaRcT501Bits,
                        const bool strict = true);
#endif
#if DECODE_NIKAI
  bool decodeNikai(decode_results *results, uint16_t offset = kStartOffset,
                   const uint16_t nbits = kNikaiBits,
                   const bool strict = true);
#endif
#if DECODE_MAGIQUEST
  bool decodeMagiQuest(decode_results *results, uint16_t offset = kStartOffset,
                       const uint16_t nbits = kMagiquestBits,
                       const bool strict = true);
#endif
#if DECODE_KELVINATOR
  bool decodeKelvinator(decode_results *results, uint16_t offset = kStartOffset,
                        const uint16_t nbits = kKelvinatorBits,
                        const bool strict = true);
#endif
#if DECODE_DAIKIN
  bool decodeDaikin(decode_results *results, uint16_t offset = kStartOffset,
                    const uint16_t nbits = kDaikinBits,
                    const bool strict = true);
#endif
#if DECODE_DAIKIN64
  bool decodeDaikin64(decode_results *results, uint16_t offset = kStartOffset,
                      const uint16_t nbits = kDaikin64Bits,
                      const bool strict = true);
#endif  // DECODE_DAIKIN64
#if DECODE_DAIKIN128
  bool decodeDaikin128(decode_results *results, uint16_t offset = kStartOffset,
                       const uint16_t nbits = kDaikin128Bits,
                       const bool strict = true);
#endif  // DECODE_DAIKIN128
#if DECODE_DAIKIN152
  bool decodeDaikin152(decode_results *results, uint16_t offset = kStartOffset,
                       const uint16_t nbits = kDaikin152Bits,
                       const bool strict = true);
#endif  // DECODE_DAIKIN152
#if DECODE_DAIKIN160
  bool decodeDaikin160(decode_results *results, uint16_t offset = kStartOffset,
                       const uint16_t nbits = kDaikin160Bits,
                       const bool strict = true);
#endif  // DECODE_DAIKIN160
#if DECODE_DAIKIN176
  bool decodeDaikin176(decode_results *results, uint16_t offset = kStartOffset,
                       const uint16_t nbits = kDaikin176Bits,
                       const bool strict = true);
#endif  // DECODE_DAIKIN176
#if DECODE_DAIKIN2
  bool decodeDaikin2(decode_results *results, uint16_t offset = kStartOffset,
                     const uint16_t nbits = kDaikin2Bits,
                     const bool strict = true);
#endif
#if DECODE_DAIKIN216
  bool decodeDaikin216(decode_results *results, uint16_t offset = kStartOffset,
                       const uint16_t nbits = kDaikin216Bits,
                       const bool strict = true);
#endif
#if DECODE_TOSHIBA_AC
  bool decodeToshibaAC(decode_results *results, uint16_t offset = kStartOffset,
                       const uint16_t nbits = kToshibaACBits,
                       const bool strict = true);
#endif
#if DECODE_TROTEC
  bool decodeTrotec(decode_results *results, uint16_t offset = kStartOffset,
                    const uint16_t nbits = kTrotecBits,
                    const bool strict = true);
#endif  // DECODE_TROTEC
#if DECODE_TROTEC_3550
  bool decodeTrotec3550(decode_results *results, uint16_t offset = kStartOffset,
                        const uint16_t nbits = kTrotecBits,
                        const bool strict = true);
#endif  // DECODE_TROTEC_3550
#if DECODE_MIDEA
  bool decodeMidea(decode_results *results, uint16_t offset = kStartOffset,
                   const uint16_t nbits = kMideaBits,
                   const bool strict = true);
#endif  // DECODE_MIDEA
#if DECODE_MIDEA24
  bool decodeMidea24(decode_results *results, uint16_t offset = kStartOffset,
                     const uint16_t nbits = kMidea24Bits,
                     const bool strict = true);
#endif  // DECODE_MIDEA24
#if DECODE_FUJITSU_AC
  bool decodeFujitsuAC(decode_results *results, uint16_t offset = kStartOffset,
                       const uint16_t nbits = kFujitsuAcBits,
                       const bool strict = false);
#endif
#if DECODE_LASERTAG
  bool decodeLasertag(decode_results *results, uint16_t offset = kStartOffset,
                      const uint16_t nbits = kLasertagBits,
                      const bool strict = true);
#endif
#if DECODE_MILESTAG2
  bool decodeMilestag2(decode_results *results, uint16_t offset = kStartOffset,
                       const uint16_t nbits = kMilesTag2ShotBits,
                       const bool strict = true);
#endif
#if DECODE_CARRIER_AC
  bool decodeCarrierAC(decode_results *results, uint16_t offset = kStartOffset,
                       const uint16_t nbits = kCarrierAcBits,
                       const bool strict = true);
#endif  // DECODE_CARRIER_AC
#if DECODE_CARRIER_AC40
  bool decodeCarrierAC40(decode_results *results,
                         uint16_t offset = kStartOffset,
                         const uint16_t nbits = kCarrierAc40Bits,
                         const bool strict = true);
#endif  // DECODE_CARRIER_AC40
#if DECODE_CARRIER_AC64
  bool decodeCarrierAC64(decode_results *results,
                         uint16_t offset = kStartOffset,
                         const uint16_t nbits = kCarrierAc64Bits,
                         const bool strict = true);
#endif  // DECODE_CARRIER_AC64
#if DECODE_GOODWEATHER
  bool decodeGoodweather(decode_results *results,
                         uint16_t offset = kStartOffset,
                         const uint16_t nbits = kGoodweatherBits,
                         const bool strict = true);
#endif  // DECODE_GOODWEATHER
#if DECODE_GREE
  bool decodeGree(decode_results *results, uint16_t offset = kStartOffset,
                  const uint16_t nbits = kGreeBits,
                  const bool strict = true);
#endif
#if (DECODE_HAIER_AC | DECODE_HAIER_AC_YRW02)
  bool decodeHaierAC(decode_results *results, uint16_t offset = kStartOffset,
                     const uint16_t nbits = kHaierACBits,
                     const bool strict = true);
#endif
#if DECODE_HAIER_AC_YRW02
  bool decodeHaierACYRW02(decode_results *results,
                          uint16_t offset = kStartOffset,
                          const uint16_t nbits = kHaierACYRW02Bits,
                          const bool strict = true);
#endif
#if DECODE_HAIER_AC176
  bool decodeHaierAC176(decode_results *results,
                        uint16_t offset = kStartOffset,
                        const uint16_t nbits = kHaierAC176Bits,
                        const bool strict = true);
#endif  // DECODE_HAIER_AC176
#if (DECODE_HITACHI_AC || DECODE_HITACHI_AC2 || DECODE_HITACHI_AC344)
  bool decodeHitachiAC(decode_results *results, uint16_t offset = kStartOffset,
                       const uint16_t nbits = kHitachiAcBits,
                       const bool strict = true, const bool MSBfirst = true);
#endif
#if DECODE_HITACHI_AC1
  bool decodeHitachiAC1(decode_results *results, uint16_t offset = kStartOffset,
                        const uint16_t nbits = kHitachiAc1Bits,
                        const bool strict = true);
#endif
#if DECODE_HITACHI_AC3
  bool decodeHitachiAc3(decode_results *results,
                          uint16_t offset = kStartOffset,
                          const uint16_t nbits = kHitachiAc3Bits,
                          const bool strict = true);
#endif  // DECODE_HITACHI_AC3
#if DECODE_HITACHI_AC424
  bool decodeHitachiAc424(decode_results *results,
                          uint16_t offset = kStartOffset,
                          const uint16_t nbits = kHitachiAc424Bits,
                          const bool strict = true);
#endif  // DECODE_HITACHI_AC424
#if DECODE_GICABLE
  bool decodeGICable(decode_results *results, uint16_t offset = kStartOffset,
                     const uint16_t nbits = kGicableBits,
                     const bool strict = true);
#endif
#if DECODE_WHIRLPOOL_AC
  bool decodeWhirlpoolAC(decode_results *results,
                         uint16_t offset = kStartOffset,
                         const uint16_t nbits = kWhirlpoolAcBits,
                         const bool strict = true);
#endif
#if DECODE_LUTRON
  bool decodeLutron(decode_results *results, uint16_t offset = kStartOffset,
                    const uint16_t nbits = kLutronBits,
                    const bool strict = true);
#endif
#if DECODE_ELECTRA_AC
  bool decodeElectraAC(decode_results *results, uint16_t offset = kStartOffset,
                       const uint16_t nbits = kElectraAcBits,
                       const bool strict = true);
#endif
#if DECODE_PANASONIC_AC
  bool decodePanasonicAC(decode_results *results,
                         uint16_t offset = kStartOffset,
                         const uint16_t nbits = kPanasonicAcBits,
                         const bool strict = true);
#endif  // DECODE_PANASONIC_AC
#if DECODE_PANASONIC_AC32
  bool decodePanasonicAC32(decode_results *results,
                         uint16_t offset = kStartOffset,
                         const uint16_t nbits = kPanasonicAc32Bits,
                         const bool strict = true);
#endif  // DECODE_PANASONIC_AC32
#if DECODE_PIONEER
  bool decodePioneer(decode_results *results, uint16_t offset = kStartOffset,
                     const uint16_t nbits = kPioneerBits,
                     const bool strict = true);
#endif
#if DECODE_MWM
  bool decodeMWM(decode_results *results, uint16_t offset = kStartOffset,
                 const uint16_t nbits = 24,
                 const bool strict = true);
#endif
#if DECODE_VESTEL_AC
  bool decodeVestelAc(decode_results *results, uint16_t offset = kStartOffset,
                      const uint16_t nbits = kVestelAcBits,
                      const bool strict = true);
#endif
#if DECODE_TECO
  bool decodeTeco(decode_results *results, uint16_t offset = kStartOffset,
                  const uint16_t nbits = kTecoBits,
                  const bool strict = false);
#endif
#if DECODE_LEGOPF
  bool decodeLegoPf(decode_results *results, uint16_t offset = kStartOffset,
                    const uint16_t nbits = kLegoPfBits,
                    const bool strict = true);
#endif
#if DECODE_NEOCLIMA
  bool decodeNeoclima(decode_results *results, uint16_t offset = kStartOffset,
                      const uint16_t nbits = kNeoclimaBits,
                      const bool strict = true);
#endif  // DECODE_NEOCLIMA
#if DECODE_AMCOR
  bool decodeAmcor(decode_results *results, uint16_t offset = kStartOffset,
                   const uint16_t nbits = kAmcorBits,
                   const bool strict = true);
#endif  // DECODE_AMCOR
#if DECODE_EPSON
  bool decodeEpson(decode_results *results, uint16_t offset = kStartOffset,
                   const uint16_t nbits = kEpsonBits,
                   const bool strict = true);
#endif  // DECODE_EPSON
#if DECODE_SYMPHONY
  bool decodeSymphony(decode_results *results, uint16_t offset = kStartOffset,
                   const uint16_t nbits = kSymphonyBits,
                   const bool strict = true);
#endif  // DECODE_SYMPHONY
#if DECODE_AIRWELL
  bool decodeAirwell(decode_results *results, uint16_t offset = kStartOffset,
                     const uint16_t nbits = kAirwellBits,
                     const bool strict = true);
#endif  // DECODE_AIRWELL
#if DECODE_DELONGHI_AC
  bool decodeDelonghiAc(decode_results *results, uint16_t offset = kStartOffset,
                     const uint16_t nbits = kDelonghiAcBits,
                     const bool strict = true);
#endif  // DECODE_DELONGHI_AC
#if DECODE_DOSHISHA
  bool decodeDoshisha(decode_results *results, uint16_t offset = kStartOffset,
                      const uint16_t nbits = kDoshishaBits,
                      const bool strict = true);
#endif  // DECODE_DOSHISHA
#if DECODE_MULTIBRACKETS
  bool decodeMultibrackets(decode_results *results,
                           uint16_t offset = kStartOffset,
                           const uint16_t nbits = kMultibracketsBits,
                           const bool strict = true);
#endif  // DECODE_MULTIBRACKETS
#if DECODE_TECHNIBEL_AC
  bool decodeTechnibelAc(decode_results *results,
                         uint16_t offset = kStartOffset,
                         const uint16_t nbits = kTechnibelAcBits,
                         const bool strict = true);
#endif  // DECODE_TECHNIBEL_AC
#if DECODE_CORONA_AC
  bool decodeCoronaAc(decode_results *results, uint16_t offset = kStartOffset,
                      const uint16_t nbits = kCoronaAcBitsShort,
                      const bool strict = true);
#endif  // DECODE_CORONA_AC
#if DECODE_ZEPEAL
  bool decodeZepeal(decode_results *results, uint16_t offset = kStartOffset,
                    const uint16_t nbits = kZepealBits,
                    const bool strict = true);
#endif  // DECODE_ZEPEAL
#if DECODE_METZ
  bool decodeMetz(decode_results *results, uint16_t offset = kStartOffset,
                  const uint16_t nbits = kMetzBits,
                  const bool strict = true);
#endif  // DECODE_METZ
#if DECODE_TRANSCOLD
  bool decodeTranscold(decode_results *results, uint16_t offset = kStartOffset,
                       const uint16_t nbits = kTranscoldBits,
                       const bool strict = true);
#endif  // DECODE_TRANSCOLD
#if DECODE_MIRAGE
  bool decodeMirage(decode_results *results,
                    uint16_t offset = kStartOffset,
                    const uint16_t nbits = kMirageBits,
                    const bool strict = true);
#endif  // DECODE_MIRAGE
#if DECODE_ELITESCREENS
  bool decodeElitescreens(decode_results *results,
                          uint16_t offset = kStartOffset,
                          const uint16_t nbits = kEliteScreensBits,
                          const bool strict = true);
#endif  // DECODE_ELITESCREENS
#if DECODE_ECOCLIM
  bool decodeEcoclim(decode_results *results, uint16_t offset = kStartOffset,
                     const uint16_t nbits = kEcoclimBits,
                     const bool strict = true);
#endif  // DECODE_ECOCLIM
#if DECODE_XMP
  bool decodeXmp(decode_results *results, uint16_t offset = kStartOffset,
                 const uint16_t nbits = kXmpBits, const bool strict = true);
#endif  // DECODE_XMP
#if DECODE_TRUMA
  bool decodeTruma(decode_results *results, uint16_t offset = kStartOffset,
                   const uint16_t nbits = kTrumaBits, const bool strict = true);
#endif  // DECODE_TRUMA
#if DECODE_TEKNOPOINT
  bool decodeTeknopoint(decode_results *results, uint16_t offset = kStartOffset,
                        const uint16_t nbits = kTeknopointBits,
                        const bool strict = true);
#endif  // DECODE_TEKNOPOINT
#if DECODE_KELON
  bool decodeKelon(decode_results *results, uint16_t offset = kStartOffset,
                   const uint16_t nbits = kKelonBits, const bool strict = true);
#endif  // DECODE_KELON
#if DECODE_BOSE
  bool decodeBose(decode_results *results, uint16_t offset = kStartOffset,
                  const uint16_t nbits = kBoseBits, const bool strict = true);
#endif  // DECODE_BOSE
};

#endif  // IRRECV_H_
