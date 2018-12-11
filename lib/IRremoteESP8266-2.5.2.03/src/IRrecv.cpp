// Copyright 2009 Ken Shirriff
// Copyright 2015 Mark Szabo
// Copyright 2015 Sebastien Warin
// Copyright 2017 David Conran

#include "IRrecv.h"
#include <stddef.h>
#ifndef UNIT_TEST
extern "C" {
#include <gpio.h>
#include <user_interface.h>
}
#include <Arduino.h>
#endif
#include <algorithm>
#include "IRremoteESP8266.h"
#include "IRutils.h"

#ifdef UNIT_TEST
#undef ICACHE_RAM_ATTR
#define ICACHE_RAM_ATTR
#endif
// Updated by Sebastien Warin (http://sebastien.warin.fr) for receiving IR code
// on ESP8266
// Updated by markszabo (https://github.com/markszabo/IRremoteESP8266) for
// sending IR code on ESP8266

// Globals
#ifndef UNIT_TEST
static ETSTimer timer;
#endif
volatile irparams_t irparams;
irparams_t *irparams_save;  // A copy of the interrupt state while decoding.

#ifndef UNIT_TEST
static void ICACHE_RAM_ATTR read_timeout(void *arg __attribute__((unused))) {
  os_intr_lock();
  if (irparams.rawlen) irparams.rcvstate = kStopState;
  os_intr_unlock();
}

static void ICACHE_RAM_ATTR gpio_intr() {
  uint32_t now = system_get_time();
  uint32_t gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
  static uint32_t start = 0;

  os_timer_disarm(&timer);
  GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status);

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
#define ONCE 0
  os_timer_arm(&timer, irparams.timeout, ONCE);
}
#endif  // UNIT_TEST

// Start of IRrecv class -------------------

// Class constructor
// Args:
//   recvpin: GPIO pin the IR receiver module's data pin is connected to.
//   bufsize: Nr. of entries to have in the capture buffer. (Default: kRawBuf)
//   timeout: Nr. of milli-Seconds of no signal before we stop capturing data.
//            (Default: kTimeoutMs)
//   save_buffer:  Use a second (save) buffer to decode from. (Def: false)
// Returns:
//   An IRrecv class object.
IRrecv::IRrecv(uint16_t recvpin, uint16_t bufsize, uint8_t timeout,
               bool save_buffer) {
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
  unknown_threshold = kUnknownThreshold;
#endif  // DECODE_HASH
}

// Class destructor
IRrecv::~IRrecv(void) {
  delete[] irparams.rawbuf;
  if (irparams_save != NULL) {
    delete[] irparams_save->rawbuf;
    delete irparams_save;
  }
}

// initialization
void IRrecv::enableIRIn() {
  // initialize state machine variables
  resume();

#ifndef UNIT_TEST
  // Initialize timer
  os_timer_disarm(&timer);
  os_timer_setfn(&timer, reinterpret_cast<os_timer_func_t *>(read_timeout),
                 NULL);

  // Attach Interrupt
  attachInterrupt(irparams.recvpin, gpio_intr, CHANGE);
#endif
}

void IRrecv::disableIRIn() {
#ifndef UNIT_TEST
  os_timer_disarm(&timer);
  detachInterrupt(irparams.recvpin);
#endif
}

void IRrecv::resume() {
  irparams.rcvstate = kIdleState;
  irparams.rawlen = 0;
  irparams.overflow = false;
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
uint16_t IRrecv::getBufSize() { return irparams.bufsize; }

#if DECODE_HASH
// Set the minimum length we will consider for reporting UNKNOWN message types.
void IRrecv::setUnknownThreshold(uint16_t length) {
  unknown_threshold = length;
}
#endif  // DECODE_HASH

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
// Returns:
//   A boolean indicating if an IR message is ready or not.
bool IRrecv::decode(decode_results *results, irparams_t *save) {
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

#if DECODE_AIWA_RC_T501
  DPRINTLN("Attempting Aiwa RC T501 decode");
  // Try decodeAiwaRCT501() before decodeSanyoLC7461() & decodeNEC()
  // because the protocols are similar. This protocol is more specific than
  // those ones, so should got before them.
  if (decodeAiwaRCT501(results)) return true;
#endif
#if DECODE_SANYO
  DPRINTLN("Attempting Sanyo LC7461 decode");
  // Try decodeSanyoLC7461() before decodeNEC() because the protocols are
  // similar in timings & structure, but the Sanyo one is much longer than the
  // NEC protocol (42 vs 32 bits) so this one should be tried first to try to
  // reduce false detection as a NEC packet.
  if (decodeSanyoLC7461(results)) return true;
#endif
#if DECODE_CARRIER_AC
  DPRINTLN("Attempting Carrier AC decode");
  // Try decodeCarrierAC() before decodeNEC() because the protocols are
  // similar in timings & structure, but the Carrier one is much longer than the
  // NEC protocol (3x32 bits vs 1x32 bits) so this one should be tried first to
  // try to reduce false detection as a NEC packet.
  if (decodeCarrierAC(results)) return true;
#endif
#if DECODE_PIONEER
  DPRINTLN("Attempting Pioneer decode");
  // Try decodePioneer() before decodeNEC() because the protocols are
  // similar in timings & structure, but the Pioneer one is much longer than the
  // NEC protocol (2x32 bits vs 1x32 bits) so this one should be tried first to
  // try to reduce false detection as a NEC packet.
  if (decodePioneer(results)) return true;
#endif
#if DECODE_NEC
  DPRINTLN("Attempting NEC decode");
  if (decodeNEC(results)) return true;
#endif
#if DECODE_SONY
  DPRINTLN("Attempting Sony decode");
  if (decodeSony(results)) return true;
#endif
#if DECODE_MITSUBISHI
  DPRINTLN("Attempting Mitsubishi decode");
  if (decodeMitsubishi(results)) return true;
#endif
#if DECODE_MITSUBISHI_AC
  DPRINTLN("Attempting Mitsubishi AC decode");
  if (decodeMitsubishiAC(results)) return true;
#endif
#if DECODE_MITSUBISHI2
  DPRINTLN("Attempting Mitsubishi2 decode");
  if (decodeMitsubishi2(results)) return true;
#endif
#if DECODE_RC5
  DPRINTLN("Attempting RC5 decode");
  if (decodeRC5(results)) return true;
#endif
#if DECODE_RC6
  DPRINTLN("Attempting RC6 decode");
  if (decodeRC6(results)) return true;
#endif
#if DECODE_RCMM
  DPRINTLN("Attempting RC-MM decode");
  if (decodeRCMM(results)) return true;
#endif
#if DECODE_FUJITSU_AC
  // Fujitsu A/C needs to precede Panasonic and Denon as it has a short
  // message which looks exactly the same as a Panasonic/Denon message.
  DPRINTLN("Attempting Fujitsu A/C decode");
  if (decodeFujitsuAC(results)) return true;
#endif
#if DECODE_DENON
  // Denon needs to precede Panasonic as it is a special case of Panasonic.
  DPRINTLN("Attempting Denon decode");
  if (decodeDenon(results, DENON_48_BITS) || decodeDenon(results, DENON_BITS) ||
      decodeDenon(results, kDenonLegacyBits))
    return true;
#endif
#if DECODE_PANASONIC
  DPRINTLN("Attempting Panasonic decode");
  if (decodePanasonic(results)) return true;
#endif
#if DECODE_LG
  DPRINTLN("Attempting LG (28-bit) decode");
  if (decodeLG(results, kLgBits, true)) return true;
  DPRINTLN("Attempting LG (32-bit) decode");
  // LG32 should be tried before Samsung
  if (decodeLG(results, kLg32Bits, true)) return true;
#endif
#if DECODE_GICABLE
  // Note: Needs to happen before JVC decode, because it looks similar except
  //       with a required NEC-like repeat code.
  DPRINTLN("Attempting GICable decode");
  if (decodeGICable(results)) return true;
#endif
#if DECODE_JVC
  DPRINTLN("Attempting JVC decode");
  if (decodeJVC(results)) return true;
#endif
#if DECODE_SAMSUNG
  DPRINTLN("Attempting SAMSUNG decode");
  if (decodeSAMSUNG(results)) return true;
#endif
#if DECODE_WHYNTER
  DPRINTLN("Attempting Whynter decode");
  if (decodeWhynter(results)) return true;
#endif
#if DECODE_DISH
  DPRINTLN("Attempting DISH decode");
  if (decodeDISH(results)) return true;
#endif
#if DECODE_SHARP
  DPRINTLN("Attempting Sharp decode");
  if (decodeSharp(results)) return true;
#endif
#if DECODE_COOLIX
  DPRINTLN("Attempting Coolix decode");
  if (decodeCOOLIX(results)) return true;
#endif
#if DECODE_NIKAI
  DPRINTLN("Attempting Nikai decode");
  if (decodeNikai(results)) return true;
#endif
#if DECODE_KELVINATOR
  // Kelvinator based-devices use a similar code to Gree ones, to avoid false
  // matches this needs to happen before decodeGree().
  DPRINTLN("Attempting Kelvinator decode");
  if (decodeKelvinator(results)) return true;
#endif
#if DECODE_DAIKIN
  DPRINTLN("Attempting Daikin decode");
  if (decodeDaikin(results)) return true;
#endif
#if DECODE_TOSHIBA_AC
  DPRINTLN("Attempting Toshiba AC decode");
  if (decodeToshibaAC(results)) return true;
#endif
#if DECODE_MIDEA
  DPRINTLN("Attempting Midea decode");
  if (decodeMidea(results)) return true;
#endif
#if DECODE_MAGIQUEST
  DPRINTLN("Attempting Magiquest decode");
  if (decodeMagiQuest(results)) return true;
#endif
/* NOTE: Disabled due to poor quality.
#if DECODE_SANYO
  // The Sanyo S866500B decoder is very poor quality & depricated.
  // *IF* you are going to enable it, do it near last to avoid false positive
  // matches.
  DPRINTLN("Attempting Sanyo SA8650B decode");
  if (decodeSanyo(results))
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
  if (decodeNEC(results, kNECBits, false)) {
    results->decode_type = NEC_LIKE;
    return true;
  }
#endif
#if DECODE_LASERTAG
  DPRINTLN("Attempting Lasertag decode");
  if (decodeLasertag(results)) return true;
#endif
#if DECODE_GREE
  // Gree based-devices use a similar code to Kelvinator ones, to avoid false
  // matches this needs to happen after decodeKelvinator().
  DPRINTLN("Attempting Gree decode");
  if (decodeGree(results)) return true;
#endif
#if DECODE_HAIER_AC
  DPRINTLN("Attempting Haier AC decode");
  if (decodeHaierAC(results)) return true;
#endif
#if DECODE_HAIER_AC_YRW02
  DPRINTLN("Attempting Haier AC YR-W02 decode");
  if (decodeHaierACYRW02(results)) return true;
#endif
#if DECODE_HITACHI_AC2
  // HitachiAC2 should be checked before HitachiAC
  DPRINTLN("Attempting Hitachi AC2 decode");
  if (decodeHitachiAC(results, kHitachiAc2Bits)) return true;
#endif
#if DECODE_HITACHI_AC
  DPRINTLN("Attempting Hitachi AC decode");
  if (decodeHitachiAC(results, kHitachiAcBits)) return true;
#endif
#if DECODE_HITACHI_AC1
  DPRINTLN("Attempting Hitachi AC1 decode");
  if (decodeHitachiAC(results, kHitachiAc1Bits)) return true;
#endif
#if DECODE_WHIRLPOOL_AC
  DPRINTLN("Attempting Whirlpool AC decode");
  if (decodeWhirlpoolAC(results)) return true;
#endif
#if DECODE_SAMSUNG_AC
  DPRINTLN("Attempting Samsung AC (extended) decode");
  // Check the extended size first, as it should fail fast due to longer length.
  if (decodeSamsungAC(results, kSamsungAcExtendedBits, false)) return true;
  // Now check for the more common length.
  DPRINTLN("Attempting Samsung AC decode");
  if (decodeSamsungAC(results, kSamsungAcBits)) return true;
#endif
#if DECODE_ELECTRA_AC
  DPRINTLN("Attempting Electra AC decode");
  if (decodeElectraAC(results)) return true;
#endif
#if DECODE_PANASONIC_AC
  DPRINTLN("Attempting Panasonic AC decode");
  if (decodePanasonicAC(results)) return true;
  DPRINTLN("Attempting Panasonic AC short decode");
  if (decodePanasonicAC(results, kPanasonicAcShortBits)) return true;
#endif
#if DECODE_LUTRON
  DPRINTLN("Attempting Lutron decode");
  if (decodeLutron(results)) return true;
#endif
#if DECODE_MWM
  DPRINTLN("Attempting MWM decode");
  if (decodeMWM(results)) return true;
#endif
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

// Calculate the lower bound of the nr. of ticks.
//
// Args:
//   usecs:  Nr. of uSeconds.
//   tolerance:  Percent as an integer. e.g. 10 is 10%
//   delta:  A non-scaling amount to reduce usecs by.
// Returns:
//   Nr. of ticks.
uint32_t IRrecv::ticksLow(uint32_t usecs, uint8_t tolerance, uint16_t delta) {
  // max() used to ensure the result can't drop below 0 before the cast.
  return ((uint32_t)std::max(
      (int32_t)(usecs * (1.0 - tolerance / 100.0) - delta), 0));
}

// Calculate the upper bound of the nr. of ticks.
//
// Args:
//   usecs:  Nr. of uSeconds.
//   tolerance:  Percent as an integer. e.g. 10 is 10%
//   delta:  A non-scaling amount to increase usecs by.
// Returns:
//   Nr. of ticks.
uint32_t IRrecv::ticksHigh(uint32_t usecs, uint8_t tolerance, uint16_t delta) {
  return ((uint32_t)(usecs * (1.0 + tolerance / 100.0)) + 1 + delta);
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
int16_t IRrecv::compare(uint16_t oldval, uint16_t newval) {
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
  if (results->rawlen < unknown_threshold) return false;
  int32_t hash = kFnvBasis32;
  // 'rawlen - 2' to avoid the look ahead from going out of bounds.
  // Should probably be -3 to avoid comparing the trailing space entry,
  // however it is left this way for compatibility with previously captured
  // values.
  for (uint16_t i = 1; i < results->rawlen - 2; i++) {
    int16_t value = compare(results->rawbuf[i], results->rawbuf[i + 2]);
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
//   tolerance: Percentage error margin to allow. (Def: kTolerance)
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

// End of IRrecv class -------------------
