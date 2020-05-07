// Copyright 2009 Ken Shirriff
// Copyright 2015 Mark Szabo
// Copyright 2015 Sebastien Warin
// Copyright 2017, 2019 David Conran

#include "IRrecv.h"
#include <stddef.h>
#ifndef UNIT_TEST
#if defined(ESP8266)
extern "C" {
#include <gpio.h>
#include <user_interface.h>
}
#endif  // ESP8266
#include <Arduino.h>
#endif
#include <algorithm>
#ifdef UNIT_TEST
#include <cassert>
#endif  // UNIT_TEST
#include "IRremoteESP8266.h"
#include "IRutils.h"

#ifdef UNIT_TEST
#undef ICACHE_RAM_ATTR
#define ICACHE_RAM_ATTR
#endif

#ifndef USE_IRAM_ATTR
#if defined(ESP8266)
#define USE_IRAM_ATTR ICACHE_RAM_ATTR
#endif  // ESP8266
#if defined(ESP32)
#define USE_IRAM_ATTR IRAM_ATTR
#endif  // ESP32
#endif  // USE_IRAM_ATTR

#define ONCE 0

// Updated by David Conran (https://github.com/crankyoldgit) for receiving IR
// code on ESP32
// Updated by Sebastien Warin (http://sebastien.warin.fr) for receiving IR code
// on ESP8266
// Updated by markszabo (https://github.com/crankyoldgit/IRremoteESP8266) for
// sending IR code on ESP8266

// Globals
#ifndef UNIT_TEST
#if defined(ESP8266)
static ETSTimer timer;
#endif  // ESP8266
#if defined(ESP32)
static hw_timer_t * timer = NULL;
#endif  // ESP32
#endif  // UNIT_TEST

#if defined(ESP32)
portMUX_TYPE irremote_mux = portMUX_INITIALIZER_UNLOCKED;
#endif  // ESP32
volatile irparams_t irparams;
irparams_t *irparams_save;  // A copy of the interrupt state while decoding.

#ifndef UNIT_TEST
#if defined(ESP8266)
static void USE_IRAM_ATTR read_timeout(void *arg __attribute__((unused))) {
  os_intr_lock();
#endif  // ESP8266
#if defined(ESP32)
static void USE_IRAM_ATTR read_timeout(void) {
  portENTER_CRITICAL(&irremote_mux);
#endif  // ESP32
  if (irparams.rawlen) irparams.rcvstate = kStopState;
#if defined(ESP8266)
  os_intr_unlock();
#endif  // ESP8266
#if defined(ESP32)
  portEXIT_CRITICAL(&irremote_mux);
#endif  // ESP32
}

static void USE_IRAM_ATTR gpio_intr() {
  uint32_t now = micros();
  static uint32_t start = 0;

#if defined(ESP8266)
  uint32_t gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
  os_timer_disarm(&timer);
  GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status);
#endif  // ESP8266

  // Grab a local copy of rawlen to reduce instructions used in IRAM.
  // This is an ugly premature optimisation code-wise, but we do everything we
  // can to save IRAM.
  // It seems referencing the value via the structure uses more instructions.
  // Less instructions means faster and less IRAM used.
  // N.B. It saves about 13 bytes of IRAM.
  uint16_t rawlen = irparams.rawlen;

  if (rawlen >= irparams.bufsize) {
    irparams.overflow = true;
    irparams.rcvstate = kStopState;
  }

  if (irparams.rcvstate == kStopState) return;

  if (irparams.rcvstate == kIdleState) {
    irparams.rcvstate = kMarkState;
    irparams.rawbuf[rawlen] = 1;
  } else {
    if (now < start)
      irparams.rawbuf[rawlen] = (UINT32_MAX - start + now) / kRawTick;
    else
      irparams.rawbuf[rawlen] = (now - start) / kRawTick;
  }
  irparams.rawlen++;

  start = now;

#if defined(ESP8266)
  os_timer_arm(&timer, irparams.timeout, ONCE);
#endif  // ESP8266
#if defined(ESP32)
  timerWrite(timer, 0);  // Reset the timeout.
  timerAlarmEnable(timer);
#endif  // ESP32
}
#endif  // UNIT_TEST

// Start of IRrecv class -------------------

// Class constructor
// Args:
//   recvpin: GPIO pin the IR receiver module's data pin is connected to.
//   bufsize: Nr. of entries to have in the capture buffer. (Default: kRawBuf)
//   timeout: Nr. of milli-Seconds of no signal before we stop capturing data.
//            (Default: kTimeoutMs)
//   save_buffer: Use a second (save) buffer to decode from. (Default: false)
//   timer_num: Which ESP32 timer number to use? ESP32 only, otherwise unused.
//              (Range: 0-3. Default: kDefaultESP32Timer)
// Returns:
//   An IRrecv class object.
#if defined(ESP32)
IRrecv::IRrecv(const uint16_t recvpin, const uint16_t bufsize,
               const uint8_t timeout, const bool save_buffer,
               const uint8_t timer_num) {
  // There are only 4 timers. 0 to 3.
  _timer_num = std::min(timer_num, (uint8_t)3);
#else  // ESP32
IRrecv::IRrecv(const uint16_t recvpin, const uint16_t bufsize,
               const uint8_t timeout, const bool save_buffer) {
#endif  // ESP32
  irparams.recvpin = recvpin;
  irparams.bufsize = bufsize;
  // Ensure we are going to be able to store all possible values in the
  // capture buffer.
  irparams.timeout = std::min(timeout, (uint8_t)kMaxTimeoutMs);
  irparams.rawbuf = new uint16_t[bufsize];
  if (irparams.rawbuf == NULL) {
    DPRINTLN(
        "Could not allocate memory for the primary IR buffer.\n"
        "Try a smaller size for CAPTURE_BUFFER_SIZE.\nRebooting!");
#ifndef UNIT_TEST
    ESP.restart();  // Mem alloc failure. Reboot.
#endif
  }
  // If we have been asked to use a save buffer (for decoding), then create one.
  if (save_buffer) {
    irparams_save = new irparams_t;
    irparams_save->rawbuf = new uint16_t[bufsize];
    // Check we allocated the memory successfully.
    if (irparams_save->rawbuf == NULL) {
      DPRINTLN(
          "Could not allocate memory for the second IR buffer.\n"
          "Try a smaller size for CAPTURE_BUFFER_SIZE.\nRebooting!");
#ifndef UNIT_TEST
      ESP.restart();  // Mem alloc failure. Reboot.
#endif
    }
  } else {
    irparams_save = NULL;
  }
#if DECODE_HASH
  _unknown_threshold = kUnknownThreshold;
#endif  // DECODE_HASH
  _tolerance = kTolerance;
}

// Class destructor
IRrecv::~IRrecv(void) {
  delete[] irparams.rawbuf;
  if (irparams_save != NULL) {
    delete[] irparams_save->rawbuf;
    delete irparams_save;
  }
  disableIRIn();
#if defined(ESP32)
  if (timer != NULL) timerEnd(timer);  // Cleanup the ESP32 timeout timer.
#endif  // ESP32
}

// Set up and (re)start the IR capture mechanism.
//
// Args:
//   pullup: A flag indicating should the GPIO use the internal pullup resistor.
//           (Default: `false`. i.e. No.)
void IRrecv::enableIRIn(const bool pullup) {
  // ESP32's seem to require explicitly setting the GPIO to INPUT etc.
  // This wasn't required on the ESP8266s, but it shouldn't hurt to make sure.
  if (pullup) {
#ifndef UNIT_TEST
    pinMode(irparams.recvpin, INPUT_PULLUP);
  } else {
    pinMode(irparams.recvpin, INPUT);
#endif  // UNIT_TEST
  }
#if defined(ESP32)
  // Initialize the ESP32 timer.
  timer = timerBegin(_timer_num, 80, true);  // 80MHz / 80 = 1 uSec granularity.
  // Set the timer so it only fires once, and set it's trigger in uSeconds.
  timerAlarmWrite(timer, MS_TO_USEC(irparams.timeout), ONCE);
  // Note: Interrupt needs to be attached before it can be enabled or disabled.
  timerAttachInterrupt(timer, &read_timeout, true);
#endif  // ESP32

  // Initialize state machine variables
  resume();

#ifndef UNIT_TEST
#if defined(ESP8266)
  // Initialize ESP8266 timer.
  os_timer_disarm(&timer);
  os_timer_setfn(&timer, reinterpret_cast<os_timer_func_t *>(read_timeout),
                 NULL);
#endif  // ESP8266
  // Attach Interrupt
  attachInterrupt(irparams.recvpin, gpio_intr, CHANGE);
#endif  // UNIT_TEST
}

void IRrecv::disableIRIn(void) {
#ifndef UNIT_TEST
#if defined(ESP8266)
  os_timer_disarm(&timer);
#endif  // ESP8266
#if defined(ESP32)
  timerAlarmDisable(timer);
#endif  // ESP32
  detachInterrupt(irparams.recvpin);
#endif  // UNIT_TEST
}

void IRrecv::resume(void) {
  irparams.rcvstate = kIdleState;
  irparams.rawlen = 0;
  irparams.overflow = false;
#if defined(ESP32)
  timerAlarmDisable(timer);
#endif  // ESP32
}

// Make a copy of the interrupt state & buffer data.
// Needed because irparams is marked as volatile, thus memcpy() isn't allowed.
// Only call this when you know the interrupt handlers won't modify anything.
// i.e. In kStopState.
//
// Args:
//   src: Pointer to an irparams_t structure to copy from.
//   dst: Pointer to an irparams_t structure to copy to.
void IRrecv::copyIrParams(volatile irparams_t *src, irparams_t *dst) {
  // Typecast src and dst addresses to (char *)
  char *csrc = (char *)src;  // NOLINT(readability/casting)
  char *cdst = (char *)dst;  // NOLINT(readability/casting)

  // Save the pointer to the destination's rawbuf so we don't lose it as
  // the for-loop/copy after this will overwrite it with src's rawbuf pointer.
  // This isn't immediately obvious due to typecasting/different variable names.
  uint16_t *dst_rawbuf_ptr;
  dst_rawbuf_ptr = dst->rawbuf;

  // Copy contents of src[] to dst[]
  for (uint16_t i = 0; i < sizeof(irparams_t); i++) cdst[i] = csrc[i];

  // Restore the buffer pointer
  dst->rawbuf = dst_rawbuf_ptr;

  // Copy the rawbuf
  for (uint16_t i = 0; i < dst->bufsize; i++) dst->rawbuf[i] = src->rawbuf[i];
}

// Obtain the maximum number of entries possible in the capture buffer.
// i.e. It's size.
uint16_t IRrecv::getBufSize(void) { return irparams.bufsize; }

#if DECODE_HASH
// Set the minimum length we will consider for reporting UNKNOWN message types.
void IRrecv::setUnknownThreshold(const uint16_t length) {
  _unknown_threshold = length;
}
#endif  // DECODE_HASH


// Set the base tolerance percentage for matching incoming IR messages.
void IRrecv::setTolerance(const uint8_t percent) {
  _tolerance = std::min(percent, (uint8_t)100);
}

// Get the base tolerance percentage for matching incoming IR messages.
uint8_t IRrecv::getTolerance(void) { return _tolerance; }

#if ENABLE_NOISE_FILTER_OPTION
// Remove or merge pulses in the capture buffer that are too short.
// Args:
//   results:  Ptr to the decode_results we are going to filter/modify.
//   floor:  Only allow values in the buffer large than this. (in micro seconds)
void IRrecv::crudeNoiseFilter(decode_results *results, const uint16_t floor) {
  if (floor == 0) return;  // Nothing to do.
  const uint16_t kTickFloor = floor / kRawTick;
  const uint16_t kBufSize = getBufSize();
  uint16_t offset = kStartOffset;
  while (offset < results->rawlen && offset + 2 < kBufSize) {
    uint16_t curr = results->rawbuf[offset];
    uint16_t next = results->rawbuf[offset + 1];
    uint16_t addition = curr + next;
    if (curr < kTickFloor) {  // Is it too short?
      // Shuffle the buffer down. i.e. Remove the mark & space pair.
      // Note: `memcpy()` can't be used as rawbuf is `volatile`.
      for (uint16_t i = offset + 2; i <= results->rawlen && i < kBufSize; i++)
        results->rawbuf[i - 2] = results->rawbuf[i];
      if (offset > 1) {  // There is a previous pair we can add to.
        // Merge this pair into into the previous space.
        results->rawbuf[offset - 1] += addition;
      }
      results->rawlen -= 2;  // Adjust the length.
    } else {
      offset++;  // Move along.
    }
  }
}
#endif  // ENABLE_NOISE_FILTER_OPTION

// Decodes the received IR message.
// If the interrupt state is saved, we will immediately resume waiting
// for the next IR message to avoid missing messages.
// Note: There is a trade-off here. Saving the state means less time lost until
// we can receiving the next message vs. using more RAM. Choose appropriately.
//
// Args:
//   results:  A pointer to where the decoded IR message will be stored.
//   save:  A pointer to an irparams_t instance in which to save
//          the interrupt's memory/state. NULL means don't save it.
//   max_skip:  Maximum Nr. of pulses at the begining of a capture we can skip
//              when attempting to find a protocol we can successfully decode.
//              This parameter can dramatically improve detection of protocols
//              when there is light IR interference just before an incoming IR
//              message, however, it comes at a steep performace price.
//              CAUTION: Increasing this value will dramatically (linnearly)
//                       increase the cpu time & usage to decode protocols.
//                       e.g. 0 -> 1 will be a 2x increase in cpu usage/time.
//                            0 -> 2 will be a 3x increase etc.
//                       If you are going to do this, consider disabling
//                       protocol decoding for protocols you are not expecting.
//              (Default is 0. No skipping.)
//   noise_floor:  Pulses below this size (in usecs) will be removed or merged
//                 prior to any decoding. This is to try to remove noise/poor
//                 readings & slighly increase the chances of a successful
//                 decode but at the cost of data fidelity & integrity.
//                 (Defaults to 0 usecs. i.e. Don't filter; which is safe!)
//                 DANGER: **Here Be Dragons!**
//                   If you set the `filter_floor` value too high, it **WILL**
//                   break decoding of some protocols. You have been warned!
//                   **Any** non-zero value has the potential to **cook** the
//                   captured raw data. i.e. The data is going to lie to you.
//                   It may obscure hardware, circuit, & environment issues thus
//                   making it impossible to support you accurately or
//                   confidently.
//                   Values of <= 50 usecs will probably be safe.
//                   51 - 100 usecs **might** be okay.
//                   100 - 150 usecs is "Danger, Will Robinson!".
//                   150 - 200 usecs expect broken protocols.
//                   At 200+ usecs, you **have** protocols you can't decode!!
//
// Returns:
//   A boolean indicating if an IR message is ready or not.
bool IRrecv::decode(decode_results *results, irparams_t *save,
                    uint8_t max_skip, uint16_t noise_floor) {
  // Proceed only if an IR message been received.
#ifndef UNIT_TEST
  if (irparams.rcvstate != kStopState) return false;
#endif

  // Clear the entry we are currently pointing to when we got the timeout.
  // i.e. Stopped collecting IR data.
  // It's junk as we never wrote an entry to it and can only confuse decoding.
  // This is done here rather than logically the best place in read_timeout()
  // as it saves a few bytes of ICACHE_RAM as that routine is bound to an
  // interrupt. decode() is not stored in ICACHE_RAM.
  // Another better option would be to zero the entire irparams.rawbuf[] on
  // resume() but that is a much more expensive operation compare to this.
  irparams.rawbuf[irparams.rawlen] = 0;

  bool resumed = false;  // Flag indicating if we have resumed.

  // If we were requested to use a save buffer previously, do so.
  if (save == NULL) save = irparams_save;

  if (save == NULL) {
    // We haven't been asked to copy it so use the existing memory.
#ifndef UNIT_TEST
    results->rawbuf = irparams.rawbuf;
    results->rawlen = irparams.rawlen;
    results->overflow = irparams.overflow;
#endif
  } else {
    copyIrParams(&irparams, save);  // Duplicate the interrupt's memory.
    resume();  // It's now safe to rearm. The IR message won't be overridden.
    resumed = true;
    // Point the results at the saved copy.
    results->rawbuf = save->rawbuf;
    results->rawlen = save->rawlen;
    results->overflow = save->overflow;
  }

  // Reset any previously partially processed results.
  results->decode_type = UNKNOWN;
  results->bits = 0;
  results->value = 0;
  results->address = 0;
  results->command = 0;
  results->repeat = false;

#if ENABLE_NOISE_FILTER_OPTION
  crudeNoiseFilter(results, noise_floor);
#endif  // ENABLE_NOISE_FILTER_OPTION
  // Keep looking for protocols until we've run out of entries to skip or we
  // find a valid protocol message.
  for (uint16_t offset = kStartOffset;
       offset <= (max_skip * 2) + kStartOffset;
       offset += 2) {
#if DECODE_AIWA_RC_T501
    DPRINTLN("Attempting Aiwa RC T501 decode");
    // Try decodeAiwaRCT501() before decodeSanyoLC7461() & decodeNEC()
    // because the protocols are similar. This protocol is more specific than
    // those ones, so should go before them.
    if (decodeAiwaRCT501(results, offset)) return true;
#endif
#if DECODE_SANYO
    DPRINTLN("Attempting Sanyo LC7461 decode");
    // Try decodeSanyoLC7461() before decodeNEC() because the protocols are
    // similar in timings & structure, but the Sanyo one is much longer than the
    // NEC protocol (42 vs 32 bits) so this one should be tried first to try to
    // reduce false detection as a NEC packet.
    if (decodeSanyoLC7461(results, offset)) return true;
#endif
#if DECODE_CARRIER_AC
    DPRINTLN("Attempting Carrier AC decode");
    // Try decodeCarrierAC() before decodeNEC() because the protocols are
    // similar in timings & structure, but the Carrier one is much longer than
    // the NEC protocol (3x32 bits vs 1x32 bits) so this one should be tried
    // first to try to reduce false detection as a NEC packet.
    if (decodeCarrierAC(results, offset)) return true;
#endif
#if DECODE_PIONEER
    DPRINTLN("Attempting Pioneer decode");
    // Try decodePioneer() before decodeNEC() because the protocols are
    // similar in timings & structure, but the Pioneer one is much longer than
    // the NEC protocol (2x32 bits vs 1x32 bits) so this one should be tried
    // first to try to reduce false detection as a NEC packet.
    if (decodePioneer(results, offset)) return true;
#endif
#if DECODE_EPSON
  DPRINTLN("Attempting Epson decode");
  // Try decodeEpson() before decodeNEC() because the protocols are
  // similar in timings & structure, but the Epson one is much longer than the
  // NEC protocol (3x32 identical bits vs 1x32 bits) so this one should be tried
  // first to try to reduce false detection as a NEC packet.
  if (decodeEpson(results, offset)) return true;
#endif
#if DECODE_NEC
    DPRINTLN("Attempting NEC decode");
    if (decodeNEC(results, offset)) return true;
#endif
#if DECODE_SONY
    DPRINTLN("Attempting Sony decode");
    if (decodeSony(results, offset)) return true;
#endif
#if DECODE_MITSUBISHI
    DPRINTLN("Attempting Mitsubishi decode");
    if (decodeMitsubishi(results, offset)) return true;
#endif
#if DECODE_MITSUBISHI_AC
    DPRINTLN("Attempting Mitsubishi AC decode");
    if (decodeMitsubishiAC(results, offset)) return true;
#endif
#if DECODE_MITSUBISHI2
    DPRINTLN("Attempting Mitsubishi2 decode");
    if (decodeMitsubishi2(results, offset)) return true;
#endif
#if DECODE_RC5
    DPRINTLN("Attempting RC5 decode");
    if (decodeRC5(results, offset)) return true;
#endif
#if DECODE_RC6
    DPRINTLN("Attempting RC6 decode");
    if (decodeRC6(results, offset)) return true;
#endif
#if DECODE_RCMM
    DPRINTLN("Attempting RC-MM decode");
    if (decodeRCMM(results, offset)) return true;
#endif
#if DECODE_FUJITSU_AC
    // Fujitsu A/C needs to precede Panasonic and Denon as it has a short
    // message which looks exactly the same as a Panasonic/Denon message.
    DPRINTLN("Attempting Fujitsu A/C decode");
    if (decodeFujitsuAC(results, offset)) return true;
#endif
#if DECODE_DENON
    // Denon needs to precede Panasonic as it is a special case of Panasonic.
    DPRINTLN("Attempting Denon decode");
    if (decodeDenon(results, offset, kDenon48Bits) ||
        decodeDenon(results, offset, kDenonBits) ||
        decodeDenon(results, offset, kDenonLegacyBits))
      return true;
#endif
#if DECODE_PANASONIC
    DPRINTLN("Attempting Panasonic decode");
    if (decodePanasonic(results, offset)) return true;
#endif
#if DECODE_LG
    DPRINTLN("Attempting LG (28-bit) decode");
    if (decodeLG(results, offset, kLgBits, true)) return true;
    DPRINTLN("Attempting LG (32-bit) decode");
    // LG32 should be tried before Samsung
    if (decodeLG(results, offset, kLg32Bits, true)) return true;
#endif
#if DECODE_GICABLE
    // Note: Needs to happen before JVC decode, because it looks similar except
    //       with a required NEC-like repeat code.
    DPRINTLN("Attempting GICable decode");
    if (decodeGICable(results, offset)) return true;
#endif
#if DECODE_JVC
    DPRINTLN("Attempting JVC decode");
    if (decodeJVC(results, offset)) return true;
#endif
#if DECODE_SAMSUNG
    DPRINTLN("Attempting SAMSUNG decode");
    if (decodeSAMSUNG(results, offset)) return true;
#endif
#if DECODE_SAMSUNG36
    DPRINTLN("Attempting Samsung36 decode");
    if (decodeSamsung36(results, offset)) return true;
#endif
#if DECODE_WHYNTER
    DPRINTLN("Attempting Whynter decode");
    if (decodeWhynter(results, offset)) return true;
#endif
#if DECODE_DISH
    DPRINTLN("Attempting DISH decode");
    if (decodeDISH(results, offset)) return true;
#endif
#if DECODE_SHARP
    DPRINTLN("Attempting Sharp decode");
    if (decodeSharp(results, offset)) return true;
#endif
#if DECODE_COOLIX
    DPRINTLN("Attempting Coolix decode");
    if (decodeCOOLIX(results, offset)) return true;
#endif
#if DECODE_NIKAI
    DPRINTLN("Attempting Nikai decode");
    if (decodeNikai(results, offset)) return true;
#endif
#if DECODE_KELVINATOR
    // Kelvinator based-devices use a similar code to Gree ones, to avoid false
    // matches this needs to happen before decodeGree().
    DPRINTLN("Attempting Kelvinator decode");
    if (decodeKelvinator(results, offset)) return true;
#endif
#if DECODE_DAIKIN
    DPRINTLN("Attempting Daikin decode");
    if (decodeDaikin(results, offset)) return true;
#endif
#if DECODE_DAIKIN2
    DPRINTLN("Attempting Daikin2 decode");
    if (decodeDaikin2(results, offset)) return true;
#endif
#if DECODE_DAIKIN216
    DPRINTLN("Attempting Daikin216 decode");
    if (decodeDaikin216(results, offset)) return true;
#endif
#if DECODE_TOSHIBA_AC
    DPRINTLN("Attempting Toshiba AC decode");
    if (decodeToshibaAC(results, offset)) return true;
#endif
#if DECODE_MIDEA
    DPRINTLN("Attempting Midea decode");
    if (decodeMidea(results, offset)) return true;
#endif
#if DECODE_MAGIQUEST
    DPRINTLN("Attempting Magiquest decode");
    if (decodeMagiQuest(results, offset)) return true;
#endif
  /* NOTE: Disabled due to poor quality.
#if DECODE_SANYO
    // The Sanyo S866500B decoder is very poor quality & depricated.
    // *IF* you are going to enable it, do it near last to avoid false positive
    // matches.
    DPRINTLN("Attempting Sanyo SA8650B decode");
    if (decodeSanyo(results, offset))
      return true;
#endif
  */
#if DECODE_NEC
    // Some devices send NEC-like codes that don't follow the true NEC spec.
    // This should detect those. e.g. Apple TV remote etc.
    // This needs to be done after all other codes that use strict and some
    // other protocols that are NEC-like as well, as turning off strict may
    // cause this to match other valid protocols.
    DPRINTLN("Attempting NEC (non-strict) decode");
    if (decodeNEC(results, offset, kNECBits, false)) {
      results->decode_type = NEC_LIKE;
      return true;
    }
#endif
#if DECODE_LASERTAG
    DPRINTLN("Attempting Lasertag decode");
    if (decodeLasertag(results, offset)) return true;
#endif
#if DECODE_GREE
    // Gree based-devices use a similar code to Kelvinator ones, to avoid false
    // matches this needs to happen after decodeKelvinator().
    DPRINTLN("Attempting Gree decode");
    if (decodeGree(results, offset)) return true;
#endif
#if DECODE_HAIER_AC
    DPRINTLN("Attempting Haier AC decode");
    if (decodeHaierAC(results, offset)) return true;
#endif
#if DECODE_HAIER_AC_YRW02
    DPRINTLN("Attempting Haier AC YR-W02 decode");
    if (decodeHaierACYRW02(results, offset)) return true;
#endif
#if DECODE_HITACHI_AC424
    // HitachiAc424 should be checked before HitachiAC, HitachiAC2,
    // & HitachiAC184
    DPRINTLN("Attempting Hitachi AC 424 decode");
    if (decodeHitachiAc424(results, offset, kHitachiAc424Bits)) return true;
#endif  // DECODE_HITACHI_AC424
#if DECODE_MITSUBISHI136
    // Needs to happen before HitachiAc3 decode.
    DPRINTLN("Attempting Mitsubishi136 decode");
    if (decodeMitsubishi136(results, offset)) return true;
#endif  // DECODE_MITSUBISHI136
#if DECODE_HITACHI_AC3
    // HitachiAc3 should be checked before HitachiAC & HitachiAC2
    // Attempt normal before the short version.
    DPRINTLN("Attempting Hitachi AC3 decode");
    // Order these in decreasing bit size, as it is more optimal.
    if (decodeHitachiAc3(results, offset, kHitachiAc3Bits) ||
        decodeHitachiAc3(results, offset, kHitachiAc3Bits - 4 * 8) ||
        decodeHitachiAc3(results, offset, kHitachiAc3Bits - 6 * 8) ||
        decodeHitachiAc3(results, offset, kHitachiAc3MinBits + 2 * 8) ||
        decodeHitachiAc3(results, offset, kHitachiAc3MinBits))
      return true;
#endif  // DECODE_HITACHI_AC3
#if DECODE_HITACHI_AC2
    // HitachiAC2 should be checked before HitachiAC
    DPRINTLN("Attempting Hitachi AC2 decode");
    if (decodeHitachiAC(results, offset, kHitachiAc2Bits)) return true;
#endif  // DECODE_HITACHI_AC2
#if DECODE_HITACHI_AC
    DPRINTLN("Attempting Hitachi AC decode");
    if (decodeHitachiAC(results, offset, kHitachiAcBits)) return true;
#endif
#if DECODE_HITACHI_AC1
    DPRINTLN("Attempting Hitachi AC1 decode");
    if (decodeHitachiAC(results, offset, kHitachiAc1Bits)) return true;
#endif
#if DECODE_WHIRLPOOL_AC
    DPRINTLN("Attempting Whirlpool AC decode");
    if (decodeWhirlpoolAC(results, offset)) return true;
#endif
#if DECODE_SAMSUNG_AC
    DPRINTLN("Attempting Samsung AC (extended) decode");
    // Check the extended size first, as it should fail fast due to longer
    // length.
    if (decodeSamsungAC(results, offset, kSamsungAcExtendedBits, false))
      return true;
    // Now check for the more common length.
    DPRINTLN("Attempting Samsung AC decode");
    if (decodeSamsungAC(results, offset, kSamsungAcBits)) return true;
#endif
#if DECODE_ELECTRA_AC
    DPRINTLN("Attempting Electra AC decode");
    if (decodeElectraAC(results, offset)) return true;
#endif
#if DECODE_PANASONIC_AC
    DPRINTLN("Attempting Panasonic AC decode");
    if (decodePanasonicAC(results, offset)) return true;
    DPRINTLN("Attempting Panasonic AC short decode");
    if (decodePanasonicAC(results, offset, kPanasonicAcShortBits)) return true;
#endif
#if DECODE_LUTRON
    DPRINTLN("Attempting Lutron decode");
    if (decodeLutron(results, offset)) return true;
#endif
#if DECODE_MWM
    DPRINTLN("Attempting MWM decode");
    if (decodeMWM(results, offset)) return true;
#endif
#if DECODE_VESTEL_AC
    DPRINTLN("Attempting Vestel AC decode");
    if (decodeVestelAc(results, offset)) return true;
#endif
#if DECODE_MITSUBISHI112 || DECODE_TCL112AC
    // Mitsubish112 and Tcl112 share the same decoder.
    DPRINTLN("Attempting Mitsubishi112/TCL112AC decode");
    if (decodeMitsubishi112(results, offset)) return true;
#endif  // DECODE_MITSUBISHI112 || DECODE_TCL112AC
#if DECODE_TECO
    DPRINTLN("Attempting Teco decode");
    if (decodeTeco(results, offset)) return true;
#endif
#if DECODE_LEGOPF
    DPRINTLN("Attempting LEGOPF decode");
    if (decodeLegoPf(results, offset)) return true;
#endif
#if DECODE_MITSUBISHIHEAVY
    DPRINTLN("Attempting MITSUBISHIHEAVY (152 bit) decode");
    if (decodeMitsubishiHeavy(results, offset, kMitsubishiHeavy152Bits))
      return true;
    DPRINTLN("Attempting MITSUBISHIHEAVY (88 bit) decode");
    if (decodeMitsubishiHeavy(results, offset, kMitsubishiHeavy88Bits))
      return true;
#endif
#if DECODE_ARGO
    DPRINTLN("Attempting Argo decode");
    if (decodeArgo(results, offset)) return true;
#endif  // DECODE_ARGO
#if DECODE_SHARP_AC
    DPRINTLN("Attempting SHARP_AC decode");
    if (decodeSharpAc(results, offset)) return true;
#endif
#if DECODE_GOODWEATHER
    DPRINTLN("Attempting GOODWEATHER decode");
    if (decodeGoodweather(results, offset)) return true;
#endif  // DECODE_GOODWEATHER
#if DECODE_INAX
    DPRINTLN("Attempting Inax decode");
    if (decodeInax(results, offset)) return true;
#endif  // DECODE_INAX
#if DECODE_TROTEC
    DPRINTLN("Attempting Trotec decode");
    if (decodeTrotec(results, offset)) return true;
#endif  // DECODE_TROTEC
#if DECODE_DAIKIN160
    DPRINTLN("Attempting Daikin160 decode");
    if (decodeDaikin160(results, offset)) return true;
#endif  // DECODE_DAIKIN160
#if DECODE_NEOCLIMA
    DPRINTLN("Attempting Neoclima decode");
    if (decodeNeoclima(results, offset)) return true;
#endif  // DECODE_NEOCLIMA
#if DECODE_DAIKIN176
    DPRINTLN("Attempting Daikin176 decode");
    if (decodeDaikin176(results, offset)) return true;
#endif  // DECODE_DAIKIN176
#if DECODE_DAIKIN128
    DPRINTLN("Attempting Daikin128 decode");
    if (decodeDaikin128(results, offset)) return true;
#endif  // DECODE_DAIKIN128
#if DECODE_AMCOR
    DPRINTLN("Attempting Amcor decode");
    if (decodeAmcor(results, offset)) return true;
#endif  // DECODE_AMCOR
#if DECODE_DAIKIN152
    DPRINTLN("Attempting Daikin152 decode");
    if (decodeDaikin152(results, offset)) return true;
#endif  // DECODE_DAIKIN152
#if DECODE_SYMPHONY
    DPRINTLN("Attempting Symphony decode");
    if (decodeSymphony(results, offset)) return true;
#endif  // DECODE_SYMPHONY
#if DECODE_DAIKIN64
    DPRINTLN("Attempting Daikin64 decode");
    if (decodeDaikin64(results, offset)) return true;
#endif  // DECODE_DAIKIN64
#if DECODE_AIRWELL
    DPRINTLN("Attempting Airwell decode");
    if (decodeAirwell(results, offset)) return true;
#endif  // DECODE_AIRWELL
  // Typically new protocols are added above this line.
  }
#if DECODE_HASH
  // decodeHash returns a hash on any input.
  // Thus, it needs to be last in the list.
  // If you add any decodes, add them before this.
  if (decodeHash(results)) {
    return true;
  }
#endif  // DECODE_HASH
  // Throw away and start over
  if (!resumed)  // Check if we have already resumed.
    resume();
  return false;
}

// Convert the tolerance percentage into something valid.
uint8_t IRrecv::_validTolerance(const uint8_t percentage) {
    return (percentage > 100) ? _tolerance : percentage;
}

// Calculate the lower bound of the nr. of ticks.
//
// Args:
//   usecs:  Nr. of uSeconds.
//   tolerance:  Percent as an integer. e.g. 10 is 10%
//   delta:  A non-scaling amount to reduce usecs by.
// Returns:
//   Nr. of ticks.
uint32_t IRrecv::ticksLow(const uint32_t usecs, const uint8_t tolerance,
                          const uint16_t delta) {
  // max() used to ensure the result can't drop below 0 before the cast.
  return ((uint32_t)std::max(
      (int32_t)(usecs * (1.0 - _validTolerance(tolerance) / 100.0) - delta),
      0));
}

// Calculate the upper bound of the nr. of ticks.
//
// Args:
//   usecs:  Nr. of uSeconds.
//   tolerance:  Percent as an integer. e.g. 10 is 10%
//   delta:  A non-scaling amount to increase usecs by.
// Returns:
//   Nr. of ticks.
uint32_t IRrecv::ticksHigh(const uint32_t usecs, const uint8_t tolerance,
                           const uint16_t delta) {
  return ((uint32_t)(usecs * (1.0 + _validTolerance(tolerance) / 100.0)) + 1 +
          delta);
}

// Check if we match a pulse(measured) with the desired within
// +/-tolerance percent and/or +/- a fixed delta range.
//
// Args:
//   measured:  The recorded period of the signal pulse.
//   desired:  The expected period (in useconds) we are matching against.
//   tolerance:  A percentage expressed as an integer. e.g. 10 is 10%.
//   delta:  A non-scaling (+/-) error margin (in useconds).
//
// Returns:
//   Boolean: true if it matches, false if it doesn't.
bool IRrecv::match(uint32_t measured, uint32_t desired, uint8_t tolerance,
                   uint16_t delta) {
  measured *= kRawTick;  // Convert to uSecs.
  DPRINT("Matching: ");
  DPRINT(ticksLow(desired, tolerance, delta));
  DPRINT(" <= ");
  DPRINT(measured);
  DPRINT(" <= ");
  DPRINTLN(ticksHigh(desired, tolerance, delta));
#ifdef UNIT_TEST
  // Sanity checks that we don't have values that cause integer over/underflow.
  // Only performed during testing so there is no performance hit in normal
  // operation.
  assert(ticksLow(desired, tolerance, delta) <= desired);
  // Check if we overflowed.  (UINT32_MAX >> 3 is approx 9 minutes!)
  assert(ticksHigh(desired, tolerance, delta) < UINT32_MAX >> 3);
  // Check if our high mark is below where we started. This could happen.
  // If there is a legit case, then this should be removed.
  assert(ticksHigh(desired, tolerance, delta) >= desired);
#endif  // UNIT_TEST
  return (measured >= ticksLow(desired, tolerance, delta) &&
          measured <= ticksHigh(desired, tolerance, delta));
}

// Check if we match a pulse(measured) of at least desired within
// tolerance percent and/or a fixed delta margin.
//
// Args:
//   measured:  The recorded period of the signal pulse.
//   desired:  The expected period (in useconds) we are matching against.
//   tolerance:  A percentage expressed as an integer. e.g. 10 is 10%.
//   delta:  A non-scaling amount to reduce usecs by.

//
// Returns:
//   Boolean: true if it matches, false if it doesn't.
bool IRrecv::matchAtLeast(uint32_t measured, uint32_t desired,
                          uint8_t tolerance, uint16_t delta) {
  measured *= kRawTick;  // Convert to uSecs.
  DPRINT("Matching ATLEAST ");
  DPRINT(measured);
  DPRINT(" vs ");
  DPRINT(desired);
  DPRINT(". Matching: ");
  DPRINT(measured);
  DPRINT(" >= ");
  DPRINT(ticksLow(std::min(desired, MS_TO_USEC(irparams.timeout)), tolerance,
                  delta));
  DPRINT(" [min(");
  DPRINT(ticksLow(desired, tolerance, delta));
  DPRINT(", ");
  DPRINT(ticksLow(MS_TO_USEC(irparams.timeout), tolerance, delta));
  DPRINTLN(")]");
#ifdef UNIT_TEST
  // Sanity checks that we don't have values that cause integer over/underflow.
  // Only performed during testing so there is no performance hit in normal
  // operation.
  assert(ticksLow(desired, tolerance, delta) <= desired);
  // Check if we overflowed.  (UINT32_MAX >> 3 is approx 9 minutes!)
  assert(ticksHigh(desired, tolerance, delta) < UINT32_MAX >> 3);
  // Check if our high mark is below where we started. This could happen.
  // If there is a legit case, then this should be removed.
  assert(ticksHigh(desired, tolerance, delta) >= desired);
#endif  // UNIT_TEST
  // We really should never get a value of 0, except as the last value
  // in the buffer. If that is the case, then assume infinity and return true.
  if (measured == 0) return true;
  return measured >= ticksLow(std::min(desired, MS_TO_USEC(irparams.timeout)),
                              tolerance, delta);
}

// Check if we match a mark signal(measured) with the desired within
// +/-tolerance percent, after an expected is excess is added.
//
// Args:
//   measured:  The recorded period of the signal pulse.
//   desired:  The expected period (in useconds) we are matching against.
//   tolerance:  A percentage expressed as an integer. e.g. 10 is 10%.
//   excess:  Nr. of useconds.
//
// Returns:
//   Boolean: true if it matches, false if it doesn't.
bool IRrecv::matchMark(uint32_t measured, uint32_t desired, uint8_t tolerance,
                       int16_t excess) {
  DPRINT("Matching MARK ");
  DPRINT(measured * kRawTick);
  DPRINT(" vs ");
  DPRINT(desired);
  DPRINT(" + ");
  DPRINT(excess);
  DPRINT(". ");
  return match(measured, desired + excess, tolerance);
}

// Check if we match a space signal(measured) with the desired within
// +/-tolerance percent, after an expected is excess is removed.
//
// Args:
//   measured:  The recorded period of the signal pulse.
//   desired:  The expected period (in useconds) we are matching against.
//   tolerance:  A percentage expressed as an integer. e.g. 10 is 10%.
//   excess:  Nr. of useconds.
//
// Returns:
//   Boolean: true if it matches, false if it doesn't.
bool IRrecv::matchSpace(uint32_t measured, uint32_t desired, uint8_t tolerance,
                        int16_t excess) {
  DPRINT("Matching SPACE ");
  DPRINT(measured * kRawTick);
  DPRINT(" vs ");
  DPRINT(desired);
  DPRINT(" - ");
  DPRINT(excess);
  DPRINT(". ");
  return match(measured, desired - excess, tolerance);
}

/* -----------------------------------------------------------------------
 * hashdecode - decode an arbitrary IR code.
 * Instead of decoding using a standard encoding scheme
 * (e.g. Sony, NEC, RC5), the code is hashed to a 32-bit value.
 *
 * The algorithm: look at the sequence of MARK signals, and see if each one
 * is shorter (0), the same length (1), or longer (2) than the previous.
 * Do the same with the SPACE signals.  Hash the resulting sequence of 0's,
 * 1's, and 2's to a 32-bit value.  This will give a unique value for each
 * different code (probably), for most code systems.
 *
 * http://arcfn.com/2010/01/using-arbitrary-remotes-with-arduino.html
 */

// Compare two tick values, returning 0 if newval is shorter,
// 1 if newval is equal, and 2 if newval is longer
// Use a tolerance of 20%
uint16_t IRrecv::compare(const uint16_t oldval, const uint16_t newval) {
  if (newval < oldval * 0.8)
    return 0;
  else if (oldval < newval * 0.8)
    return 2;
  else
    return 1;
}

#if DECODE_HASH
/* Converts the raw code values into a 32-bit hash code.
 * Hopefully this code is unique for each button.
 * This isn't a "real" decoding, just an arbitrary value.
 */
bool IRrecv::decodeHash(decode_results *results) {
  // Require at least some samples to prevent triggering on noise
  if (results->rawlen < _unknown_threshold) return false;
  int32_t hash = kFnvBasis32;
  // 'rawlen - 2' to avoid the look ahead from going out of bounds.
  // Should probably be -3 to avoid comparing the trailing space entry,
  // however it is left this way for compatibility with previously captured
  // values.
  for (uint16_t i = 1; i < results->rawlen - 2; i++) {
    uint16_t value = compare(results->rawbuf[i], results->rawbuf[i + 2]);
    // Add value into the hash
    hash = (hash * kFnvPrime32) ^ value;
  }
  results->value = hash & 0xFFFFFFFF;
  results->bits = results->rawlen / 2;
  results->address = 0;
  results->command = 0;
  results->decode_type = UNKNOWN;
  return true;
}
#endif  // DECODE_HASH

// Match & decode the typical data section of an IR message.
// The data value is stored in the least significant bits reguardless of the
// bit ordering requested.
//
// Args:
//   data_ptr: A pointer to where we are at in the capture buffer.
//   nbits:     Nr. of data bits we expect.
//   onemark:   Nr. of uSeconds in an expected mark signal for a '1' bit.
//   onespace:  Nr. of uSeconds in an expected space signal for a '1' bit.
//   zeromark:  Nr. of uSeconds in an expected mark signal for a '0' bit.
//   zerospace: Nr. of uSeconds in an expected space signal for a '0' bit.
//   tolerance: Percentage error margin to allow. (Def: kUseDefTol)
//   excess:  Nr. of useconds. (Def: kMarkExcess)
//   MSBfirst: Bit order to save the data in. (Def: true)
// Returns:
//  A match_result_t structure containing the success (or not), the data value,
//  and how many buffer entries were used.
match_result_t IRrecv::matchData(
    volatile uint16_t *data_ptr, const uint16_t nbits, const uint16_t onemark,
    const uint32_t onespace, const uint16_t zeromark, const uint32_t zerospace,
    const uint8_t tolerance, const int16_t excess, const bool MSBfirst) {
  match_result_t result;
  result.success = false;  // Fail by default.
  result.data = 0;
  for (result.used = 0; result.used < nbits * 2;
       result.used += 2, data_ptr += 2) {
    // Is the bit a '1'?
    if (matchMark(*data_ptr, onemark, tolerance, excess) &&
        matchSpace(*(data_ptr + 1), onespace, tolerance, excess)) {
      result.data = (result.data << 1) | 1;
    } else if (matchMark(*data_ptr, zeromark, tolerance, excess) &&
               matchSpace(*(data_ptr + 1), zerospace, tolerance, excess)) {
      result.data <<= 1;  // The bit is a '0'.
    } else {
      if (!MSBfirst) result.data = reverseBits(result.data, result.used / 2);
      return result;  // It's neither, so fail.
    }
  }
  result.success = true;
  if (!MSBfirst) result.data = reverseBits(result.data, nbits);
  return result;
}

// Match & decode the typical data section of an IR message.
// The bytes are stored at result_ptr. The first byte in the result equates to
// the first byte encountered, and so on.
//
// Args:
//   data_ptr: A pointer to where we are at in the capture buffer.
//   result_ptr: A pointer to where to start storing the bytes we decoded.
//   remaining: The size of the capture buffer are remaining.
//   nbytes:    Nr. of data bytes we expect.
//   onemark:   Nr. of uSeconds in an expected mark signal for a '1' bit.
//   onespace:  Nr. of uSeconds in an expected space signal for a '1' bit.
//   zeromark:  Nr. of uSeconds in an expected mark signal for a '0' bit.
//   zerospace: Nr. of uSeconds in an expected space signal for a '0' bit.
//   tolerance: Percentage error margin to allow. (Def: kUseDefTol)
//   excess:  Nr. of useconds. (Def: kMarkExcess)
//   MSBfirst: Bit order to save the data in. (Def: true)
// Returns:
//  A uint16_t: If successful, how many buffer entries were used. Otherwise 0.
uint16_t IRrecv::matchBytes(volatile uint16_t *data_ptr, uint8_t *result_ptr,
                            const uint16_t remaining, const uint16_t nbytes,
                            const uint16_t onemark, const uint32_t onespace,
                            const uint16_t zeromark, const uint32_t zerospace,
                            const uint8_t tolerance, const int16_t excess,
                            const bool MSBfirst) {
  // Check if there is enough capture buffer to possibly have the desired bytes.
  if (remaining < nbytes * 8 * 2) return 0;  // Nope, so abort.
  uint16_t offset = 0;
  for (uint16_t byte_pos = 0; byte_pos < nbytes; byte_pos++) {
    match_result_t result = matchData(data_ptr + offset, 8, onemark, onespace,
                                      zeromark, zerospace, tolerance, excess,
                                      MSBfirst);
    if (result.success == false) return 0;  // Fail
    result_ptr[byte_pos] = (uint8_t)result.data;
    offset += result.used;
  }
  return offset;
}

// Match & decode a generic/typical IR message.
// The data is stored in result_bits_ptr or result_bytes_ptr depending on flag
// `use_bits`.
// Values of 0 for hdrmark, hdrspace, footermark, or footerspace mean skip
// that requirement.
//
// Args:
//   data_ptr: A pointer to where we are at in the capture buffer.
//   result_bits_ptr: A pointer to where to start storing the bits we decoded.
//   result_bytes_ptr: A pointer to where to start storing the bytes we decoded.
//   use_bits: A flag indicating if we are to decode bits or bytes.
//   remaining: The size of the capture buffer are remaining.
//   nbits:        Nr. of data bits we expect.
//   hdrmark:      Nr. of uSeconds for the expected header mark signal.
//   hdrspace:     Nr. of uSeconds for the expected header space signal.
//   onemark:      Nr. of uSeconds in an expected mark signal for a '1' bit.
//   onespace:     Nr. of uSeconds in an expected space signal for a '1' bit.
//   zeromark:     Nr. of uSeconds in an expected mark signal for a '0' bit.
//   zerospace:    Nr. of uSeconds in an expected space signal for a '0' bit.
//   footermark:   Nr. of uSeconds for the expected footer mark signal.
//   footerspace:  Nr. of uSeconds for the expected footer space/gap signal.
//   atleast:      Is the match on the footerspace a matchAtLeast or matchSpace?
//   tolerance: Percentage error margin to allow. (Def: kUseDefTol)
//   excess:  Nr. of useconds. (Def: kMarkExcess)
//   MSBfirst: Bit order to save the data in. (Def: true)
// Returns:
//  A uint16_t: If successful, how many buffer entries were used. Otherwise 0.
uint16_t IRrecv::_matchGeneric(volatile uint16_t *data_ptr,
                              uint64_t *result_bits_ptr,
                              uint8_t *result_bytes_ptr,
                              const bool use_bits,
                              const uint16_t remaining,
                              const uint16_t nbits,
                              const uint16_t hdrmark,
                              const uint32_t hdrspace,
                              const uint16_t onemark,
                              const uint32_t onespace,
                              const uint16_t zeromark,
                              const uint32_t zerospace,
                              const uint16_t footermark,
                              const uint32_t footerspace,
                              const bool atleast,
                              const uint8_t tolerance,
                              const int16_t excess,
                              const bool MSBfirst) {
  // If we are expecting byte sizes, check it's a factor of 8 or fail.
  if (!use_bits && nbits % 8 != 0)  return 0;
  // Calculate how much remaining buffer is required.
  uint16_t min_remaining = nbits * 2;

  if (hdrmark) min_remaining++;
  if (hdrspace) min_remaining++;
  if (footermark) min_remaining++;
  // Don't need to extend for footerspace because it could be the end of message

  // Check if there is enough capture buffer to possibly have the message.
  if (remaining < min_remaining) return 0;  // Nope, so abort.
  uint16_t offset = 0;

  // Header
  if (hdrmark && !matchMark(*(data_ptr + offset++), hdrmark, tolerance, excess))
    return 0;
  if (hdrspace && !matchSpace(*(data_ptr + offset++), hdrspace, tolerance,
                              excess))
    return 0;

  // Data
  if (use_bits) {  // Bits.
    match_result_t result = IRrecv::matchData(data_ptr + offset, nbits,
                                              onemark, onespace,
                                              zeromark, zerospace, tolerance,
                                              excess, MSBfirst);
    if (!result.success) return 0;
    *result_bits_ptr = result.data;
    offset += result.used;
  } else {  // bytes
    uint16_t data_used = IRrecv::matchBytes(data_ptr + offset, result_bytes_ptr,
                                            remaining - offset, nbits / 8,
                                            onemark, onespace,
                                            zeromark, zerospace, tolerance,
                                            excess, MSBfirst);
    if (!data_used) return 0;
    offset += data_used;
  }
  // Footer
  if (footermark && !matchMark(*(data_ptr + offset++), footermark, tolerance,
                               excess))
    return 0;
  // If we have something still to match & haven't reached the end of the buffer
  if (footerspace && offset < remaining) {
      if (atleast) {
        if (!matchAtLeast(*(data_ptr + offset), footerspace, tolerance, excess))
          return 0;
      } else {
        if (!matchSpace(*(data_ptr + offset), footerspace, tolerance, excess))
          return 0;
      }
      offset++;
  }
  return offset;
}

// Match & decode a generic/typical <= 64bit IR message.
// The data is stored at result_ptr.
// Values of 0 for hdrmark, hdrspace, footermark, or footerspace mean skip
// that requirement.
//
// Args:
//   data_ptr: A pointer to where we are at in the capture buffer.
//   result_ptr: A pointer to where to start storing the bits we decoded.
//   remaining: The size of the capture buffer are remaining.
//   nbits:        Nr. of data bits we expect.
//   hdrmark:      Nr. of uSeconds for the expected header mark signal.
//   hdrspace:     Nr. of uSeconds for the expected header space signal.
//   onemark:      Nr. of uSeconds in an expected mark signal for a '1' bit.
//   onespace:     Nr. of uSeconds in an expected space signal for a '1' bit.
//   zeromark:     Nr. of uSeconds in an expected mark signal for a '0' bit.
//   zerospace:    Nr. of uSeconds in an expected space signal for a '0' bit.
//   footermark:   Nr. of uSeconds for the expected footer mark signal.
//   footerspace:  Nr. of uSeconds for the expected footer space/gap signal.
//   atleast:      Is the match on the footerspace a matchAtLeast or matchSpace?
//   tolerance: Percentage error margin to allow. (Def: kUseDefTol)
//   excess:  Nr. of useconds. (Def: kMarkExcess)
//   MSBfirst: Bit order to save the data in. (Def: true)
// Returns:
//  A uint16_t: If successful, how many buffer entries were used. Otherwise 0.
uint16_t IRrecv::matchGeneric(volatile uint16_t *data_ptr,
                              uint64_t *result_ptr,
                              const uint16_t remaining,
                              const uint16_t nbits,
                              const uint16_t hdrmark,
                              const uint32_t hdrspace,
                              const uint16_t onemark,
                              const uint32_t onespace,
                              const uint16_t zeromark,
                              const uint32_t zerospace,
                              const uint16_t footermark,
                              const uint32_t footerspace,
                              const bool atleast,
                              const uint8_t tolerance,
                              const int16_t excess,
                              const bool MSBfirst) {
  return _matchGeneric(data_ptr, result_ptr, NULL, true, remaining, nbits,
                       hdrmark, hdrspace, onemark, onespace,
                       zeromark, zerospace, footermark, footerspace, atleast,
                       tolerance, excess, MSBfirst);
}

// Match & decode a generic/typical > 64bit IR message.
// The bytes are stored at result_ptr. The first byte in the result equates to
// the first byte encountered, and so on.
// Values of 0 for hdrmark, hdrspace, footermark, or footerspace mean skip
// that requirement.
//
// Args:
//   data_ptr: A pointer to where we are at in the capture buffer.
//   result_ptr: A pointer to where to start storing the bytes we decoded.
//   remaining: The size of the capture buffer are remaining.
//   nbits:        Nr. of data bits we expect.
//   hdrmark:      Nr. of uSeconds for the expected header mark signal.
//   hdrspace:     Nr. of uSeconds for the expected header space signal.
//   onemark:      Nr. of uSeconds in an expected mark signal for a '1' bit.
//   onespace:     Nr. of uSeconds in an expected space signal for a '1' bit.
//   zeromark:     Nr. of uSeconds in an expected mark signal for a '0' bit.
//   zerospace:    Nr. of uSeconds in an expected space signal for a '0' bit.
//   footermark:   Nr. of uSeconds for the expected footer mark signal.
//   footerspace:  Nr. of uSeconds for the expected footer space/gap signal.
//   atleast:      Is the match on the footerspace a matchAtLeast or matchSpace?
//   tolerance: Percentage error margin to allow. (Def: kUseDefTol)
//   excess:  Nr. of useconds. (Def: kMarkExcess)
//   MSBfirst: Bit order to save the data in. (Def: true)
// Returns:
//  A uint16_t: If successful, how many buffer entries were used. Otherwise 0.
uint16_t IRrecv::matchGeneric(volatile uint16_t *data_ptr,
                              uint8_t *result_ptr,
                              const uint16_t remaining,
                              const uint16_t nbits,
                              const uint16_t hdrmark,
                              const uint32_t hdrspace,
                              const uint16_t onemark,
                              const uint32_t onespace,
                              const uint16_t zeromark,
                              const uint32_t zerospace,
                              const uint16_t footermark,
                              const uint32_t footerspace,
                              const bool atleast,
                              const uint8_t tolerance,
                              const int16_t excess,
                              const bool MSBfirst) {
  return _matchGeneric(data_ptr, NULL, result_ptr, false, remaining, nbits,
                       hdrmark, hdrspace, onemark, onespace,
                       zeromark, zerospace, footermark, footerspace, atleast,
                       tolerance, excess, MSBfirst);
}

// Match & decode a Manchester Code <= 64bit IR message.
// The data is stored at result_ptr.
// Values of 0 for hdrmark, hdrspace, footermark, or footerspace mean skip
// that requirement.
//
// Args:
//   data_ptr: A pointer to where we are at in the capture buffer.
//             NOTE: It is assumed to be pointing to a "Mark", not a "Space".
//   result_ptr: A pointer to where to start storing the bits we decoded.
//   remaining: The size of the capture buffer are remaining.
//   nbits:        Nr. of data bits we expect.
//   hdrmark:      Nr. of uSeconds for the expected header mark signal.
//   hdrspace:     Nr. of uSeconds for the expected header space signal.
//   half_period:  Nr. of uSeconds for half the clock's period. (1/2 wavelength)
//   footermark:   Nr. of uSeconds for the expected footer mark signal.
//   footerspace:  Nr. of uSeconds for the expected footer space/gap signal.
//   atleast:      Is the match on the footerspace a matchAtLeast or matchSpace?
//   tolerance: Percentage error margin to allow. (Def: kUseDefTol)
//   excess:  Nr. of useconds. (Def: kMarkExcess)
//   MSBfirst: Bit order to save the data in. (Def: true)
//   GEThomas: Use G.E. Thomas (true/default) or IEEE 802.3 (false) convention?
// Returns:
//   A uint16_t: If successful, how many buffer entries were used. Otherwise 0.
//
// Ref:
//   https://en.wikipedia.org/wiki/Manchester_code
//   http://ww1.microchip.com/downloads/en/AppNotes/Atmel-9164-Manchester-Coding-Basics_Application-Note.pdf
uint16_t IRrecv::matchManchester(volatile const uint16_t *data_ptr,
                                 uint64_t *result_ptr,
                                 const uint16_t remaining,
                                 const uint16_t nbits,
                                 const uint16_t hdrmark,
                                 const uint32_t hdrspace,
                                 const uint16_t half_period,
                                 const uint16_t footermark,
                                 const uint32_t footerspace,
                                 const bool atleast,
                                 const uint8_t tolerance,
                                 const int16_t excess,
                                 const bool MSBfirst,
                                 const bool GEThomas) {
  uint16_t offset = 0;
  uint64_t data = 0;
  uint16_t nr_of_half_periods = GEThomas;
  // 2 per bit, and 4 extra for the timing sync.
  uint16_t expected_half_periods = 2 * nbits + 4;
  bool currentBit = false;

  // Calculate how much remaining buffer is required.
  // Shortest case. Longest case is 2 * nbits.
  uint16_t min_remaining = nbits + 2;

  if (hdrmark) min_remaining++;
  if (hdrspace) min_remaining++;
  if (footermark) min_remaining++;
  // Don't need to extend for footerspace because it could be the end of message

  // Check if there is enough capture buffer to possibly have the message.
  if (remaining < min_remaining) return 0;  // Nope, so abort.

  // Header
  if (hdrmark && !matchMark(*(data_ptr + offset++), hdrmark, tolerance, excess))
    return 0;
  // Manchester Code always has a guaranteed 2x half_period (T2) at the start
  // of the data section. e.g. a sync header. If it is a GEThomas-style, then
  // it is space(T);mark(2xT);space(T), thus we need to check for that space
  // plus any requested "header" space.
  if ((hdrspace || GEThomas) &&
      !matchSpace(*(data_ptr + offset++),
                  hdrspace + ((GEThomas) ? half_period : 0), tolerance, excess))
    return 0;

  // Data
  // Loop until we find a 'long' pulse. This is the timing sync per protocol.
  while ((offset < remaining) && (nr_of_half_periods < expected_half_periods) &&
         !match(*(data_ptr + offset), half_period * 2, tolerance, excess)) {
    // Was it not a short pulse?
    if (!match(*(data_ptr + offset), half_period, tolerance, excess))
      return 0;
    nr_of_half_periods++;
    offset++;
  }

  // Data (cont.)

  // We are now pointing to the first 'long' pulse.
  // Loop through the buffer till we run out of buffer, or nr of half periods.
  while (offset < remaining && nr_of_half_periods < expected_half_periods) {
    // Only if there is enough half_periods left for a long pulse &
    // Is it a 'long' pulse?
    if (nr_of_half_periods < expected_half_periods - 1 &&
        match(*(data_ptr + offset), half_period * 2, tolerance, excess)) {
      // Yes, so invert the value we will append.
      currentBit = !currentBit;
      nr_of_half_periods += 2;  // A 'long' pulse is two half periods.
      offset++;
      // Append the bit value.
      data <<= 1;
      data |= currentBit;
    } else if (match(*(data_ptr + offset), half_period, tolerance, excess)) {
      // or is it part of a 'short' pulse pair?
      nr_of_half_periods++;
      offset++;
      // Look for the second half of the 'short' pulse pair.
      // Do we have enough buffer or nr of half periods?
      if (offset < remaining && nr_of_half_periods < expected_half_periods) {
        // We do, so look for it.
        if (match(*(data_ptr + offset), half_period, tolerance, excess)) {
          // Found it!
          nr_of_half_periods++;
          // No change of the polarity of the bit we will append.
          // Append the bit value.
          data <<= 1;
          data |= currentBit;
          offset++;
        } else {
          // It's not what we expected.
          return 0;
        }
      }
    } else if (nr_of_half_periods == expected_half_periods - 1 &&
               matchAtLeast(*(data_ptr + offset), half_period, tolerance,
                            excess)) {
      // Special case when we are at the end of the expected nr of periods.
      // i.e. The pulse could be merged with the footer.
      nr_of_half_periods++;
      break;
    } else {
      // It's neither, so abort.
      return 0;
    }
  }
  // Did we collect the expected amount of data?
  if (nr_of_half_periods < expected_half_periods) return 0;

  // Footer
  if (footermark &&
      !(matchMark(*(data_ptr + offset), footermark + half_period,
                  tolerance, excess) ||
        matchMark(*(data_ptr + offset), footermark,
                    tolerance, excess)))
    return 0;
  offset++;
  // If we have something still to match & haven't reached the end of the buffer
  if (footerspace && offset < remaining) {
    if (atleast) {
      if (!matchAtLeast(*(data_ptr + offset), footerspace, tolerance, excess))
        return 0;
    } else {
      if (!matchSpace(*(data_ptr + offset), footerspace, tolerance, excess))
        return 0;
    }
    offset++;
  }

  // Clean up and process the data.
  if (!MSBfirst) data = reverseBits(data, nbits);
  // Trim the data to size to remove timing sync.
  *result_ptr = GETBITS64(data, 0, nbits);
  return offset;
}
// End of IRrecv class -------------------
