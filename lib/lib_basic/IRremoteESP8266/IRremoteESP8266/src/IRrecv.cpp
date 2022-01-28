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
#endif  // UNIT_TEST
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
#if defined(IRAM_ATTR)
#define USE_IRAM_ATTR IRAM_ATTR
#else  // IRAM_ATTR
#define USE_IRAM_ATTR ICACHE_RAM_ATTR
#endif  // IRAM_ATTR
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
namespace _IRrecv {
static ETSTimer timer;
}  // namespace _IRrecv
#endif  // ESP8266
#if defined(ESP32)
// We need a horrible timer hack for ESP32 Arduino framework < v2.0.0
#if !defined(_ESP32_IRRECV_TIMER_HACK)
// Version check
#if ( defined(ESP_ARDUINO_VERSION_MAJOR) && (ESP_ARDUINO_VERSION_MAJOR >= 2) )
// No need for the hack if we are running version >= 2.0.0
#define _ESP32_IRRECV_TIMER_HACK false
#else  // Version check
// If no ESP_ARDUINO_VERSION_MAJOR is defined, or less than 2, then we are
// using an old ESP32 core, so we need the hack.
#define _ESP32_IRRECV_TIMER_HACK true
#endif  // Version check
#endif  // !defined(_ESP32_IRRECV_TIMER_HACK)

#if _ESP32_IRRECV_TIMER_HACK
// Required structs/types from:
// https://github.com/espressif/arduino-esp32/blob/6b0114366baf986c155e8173ab7c22bc0c5fcedc/cores/esp32/esp32-hal-timer.c#L28-L58
// These are needed to be able to directly manipulate the timer registers from
// inside an ISR. This is very very ugly.
// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1350
// Note: This will need to be updated if it ever changes.
//
// Start of Horrible Hack!
typedef struct {
    union {
        struct {
            uint32_t reserved0:   10;
            uint32_t alarm_en:     1;
            /*When set  alarm is enabled*/
            uint32_t level_int_en: 1;
            /*When set  level type interrupt will be generated during alarm*/
            uint32_t edge_int_en:  1;
            /*When set  edge type interrupt will be generated during alarm*/
            uint32_t divider:     16;
            /*Timer clock (T0/1_clk) pre-scale value.*/
            uint32_t autoreload:   1;
            /*When set  timer 0/1 auto-reload at alarming is enabled*/
            uint32_t increase:     1;
            /*When set  timer 0/1 time-base counter increment.
              When cleared timer 0 time-base counter decrement.*/
            uint32_t enable:       1;
            /*When set  timer 0/1 time-base counter is enabled*/
        };
        uint32_t val;
    } config;
    uint32_t cnt_low;
    /*Register to store timer 0/1 time-base counter current value lower 32
      bits.*/
    uint32_t cnt_high;
    /*Register to store timer 0 time-base counter current value higher 32
      bits.*/
    uint32_t update;
    /*Write any value will trigger a timer 0 time-base counter value update
      (timer 0 current value will be stored in registers above)*/
    uint32_t alarm_low;
    /*Timer 0 time-base counter value lower 32 bits that will trigger the
      alarm*/
    uint32_t alarm_high;
    /*Timer 0 time-base counter value higher 32 bits that will trigger the
      alarm*/
    uint32_t load_low;
    /*Lower 32 bits of the value that will load into timer 0 time-base counter*/
    uint32_t load_high;
    /*higher 32 bits of the value that will load into timer 0 time-base
      counter*/
    uint32_t reload;
    /*Write any value will trigger timer 0 time-base counter reload*/
} hw_timer_reg_t;

typedef struct hw_timer_s {
        hw_timer_reg_t * dev;
        uint8_t num;
        uint8_t group;
        uint8_t timer;
        portMUX_TYPE lock;
} hw_timer_t;
#endif  // _ESP32_IRRECV_TIMER_HACK / End of Horrible Hack.

namespace _IRrecv {
static hw_timer_t * timer = NULL;
}  // namespace _IRrecv
#endif  // ESP32
using _IRrecv::timer;
#endif  // UNIT_TEST

namespace _IRrecv {  // Namespace extension
#if defined(ESP32)
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
#endif  // ESP32
volatile irparams_t params;
irparams_t *params_save;  // A copy of the interrupt state while decoding.
}  // namespace _IRrecv

#if defined(ESP32)
using _IRrecv::mux;
#endif  // ESP32
using _IRrecv::params;
using _IRrecv::params_save;

#ifndef UNIT_TEST
#if defined(ESP8266)
/// Interrupt handler for when the timer runs out.
/// It signals to the library that capturing of IR data has stopped.
/// @param[in] arg Unused. (ESP8266 Only)
static void USE_IRAM_ATTR read_timeout(void *arg __attribute__((unused))) {
  os_intr_lock();
#endif  // ESP8266
/// @cond IGNORE
#if defined(ESP32)
/// Interrupt handler for when the timer runs out.
/// It signals to the library that capturing of IR data has stopped.
/// @note ESP32 version
static void USE_IRAM_ATTR read_timeout(void) {
/// @endcond
  portENTER_CRITICAL(&mux);
#endif  // ESP32
  if (params.rawlen) params.rcvstate = kStopState;
#if defined(ESP8266)
  os_intr_unlock();
#endif  // ESP8266
#if defined(ESP32)
  portEXIT_CRITICAL(&mux);
#endif  // ESP32
}

/// Interrupt handler for changes on the GPIO pin handling incoming IR messages.
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
  uint16_t rawlen = params.rawlen;

  if (rawlen >= params.bufsize) {
    params.overflow = true;
    params.rcvstate = kStopState;
  }

  if (params.rcvstate == kStopState) return;

  if (params.rcvstate == kIdleState) {
    params.rcvstate = kMarkState;
    params.rawbuf[rawlen] = 1;
  } else {
    if (now < start)
      params.rawbuf[rawlen] = (UINT32_MAX - start + now) / kRawTick;
    else
      params.rawbuf[rawlen] = (now - start) / kRawTick;
  }
  params.rawlen++;

  start = now;

#if defined(ESP8266)
  os_timer_arm(&timer, params.timeout, ONCE);
#endif  // ESP8266
#if defined(ESP32)
  // Reset the timeout.
  //
#if _ESP32_IRRECV_TIMER_HACK
  // The following three lines of code are the equiv of:
  //   `timerWrite(timer, 0);`
  // We can't call that routine safely from inside an ISR as that procedure
  // is not stored in IRAM. Hence, we do it manually so that it's covered by
  // USE_IRAM_ATTR in this ISR.
  // @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1350
  // @see https://github.com/espressif/arduino-esp32/blob/6b0114366baf986c155e8173ab7c22bc0c5fcedc/cores/esp32/esp32-hal-timer.c#L106-L110
  timer->dev->load_high = (uint32_t) 0;
  timer->dev->load_low = (uint32_t) 0;
  timer->dev->reload = 1;
  // The next line is the same, but instead replaces:
  //   `timerAlarmEnable(timer);`
  // @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1350
  // @see https://github.com/espressif/arduino-esp32/blob/6b0114366baf986c155e8173ab7c22bc0c5fcedc/cores/esp32/esp32-hal-timer.c#L176-L178
  timer->dev->config.alarm_en = 1;
#else  // _ESP32_IRRECV_TIMER_HACK
  timerWrite(timer, 0);
  timerAlarmEnable(timer);
#endif  // _ESP32_IRRECV_TIMER_HACK
#endif  // ESP32
}
#endif  // UNIT_TEST

// Start of IRrecv class -------------------

/// Class constructor
/// Args:
/// @param[in] recvpin The GPIO pin the IR receiver module's data pin is
///   connected to.
/// @param[in] bufsize Nr. of entries to have in the capture buffer.
///   (Default: kRawBuf)
/// @param[in] timeout Nr. of milli-Seconds of no signal before we stop
///   capturing data. (Default: kTimeoutMs)
/// @param[in] save_buffer Use a second (save) buffer to decode from.
///   (Default: false)
/// @param[in] timer_num Nr. of the ESP32 timer to use (0 to 3) (ESP32 Only)
#if defined(ESP32)
IRrecv::IRrecv(const uint16_t recvpin, const uint16_t bufsize,
               const uint8_t timeout, const bool save_buffer,
               const uint8_t timer_num) {
  // There are only 4 timers. 0 to 3.
  _timer_num = std::min(timer_num, (uint8_t)3);
#else  // ESP32
/// @cond IGNORE
/// Class constructor
/// Args:
/// @param[in] recvpin The GPIO pin the IR receiver module's data pin is
///   connected to.
/// @param[in] bufsize Nr. of entries to have in the capture buffer.
///   (Default: kRawBuf)
/// @param[in] timeout Nr. of milli-Seconds of no signal before we stop
///   capturing data. (Default: kTimeoutMs)
/// @param[in] save_buffer Use a second (save) buffer to decode from.
///   (Default: false)
IRrecv::IRrecv(const uint16_t recvpin, const uint16_t bufsize,
               const uint8_t timeout, const bool save_buffer) {
/// @endcond
#endif  // ESP32
  params.recvpin = recvpin;
  params.bufsize = bufsize;
  // Ensure we are going to be able to store all possible values in the
  // capture buffer.
  params.timeout = std::min(timeout, (uint8_t)kMaxTimeoutMs);
  params.rawbuf = new uint16_t[bufsize];
  if (params.rawbuf == NULL) {
    DPRINTLN(
        "Could not allocate memory for the primary IR buffer.\n"
        "Try a smaller size for CAPTURE_BUFFER_SIZE.\nRebooting!");
#ifndef UNIT_TEST
    ESP.restart();  // Mem alloc failure. Reboot.
#endif
  }
  // If we have been asked to use a save buffer (for decoding), then create one.
  if (save_buffer) {
    params_save = new irparams_t;
    params_save->rawbuf = new uint16_t[bufsize];
    // Check we allocated the memory successfully.
    if (params_save->rawbuf == NULL) {
      DPRINTLN(
          "Could not allocate memory for the second IR buffer.\n"
          "Try a smaller size for CAPTURE_BUFFER_SIZE.\nRebooting!");
#ifndef UNIT_TEST
      ESP.restart();  // Mem alloc failure. Reboot.
#endif
    }
  } else {
    params_save = NULL;
  }
#if DECODE_HASH
  _unknown_threshold = kUnknownThreshold;
#endif  // DECODE_HASH
  _tolerance = kTolerance;
}

/// Class destructor
/// Cleans up after the object is no longer needed.
/// e.g. Frees up all memory used by the various buffers, and disables any
/// timers or interrupts used.
IRrecv::~IRrecv(void) {
  disableIRIn();
#if defined(ESP32)
  if (timer != NULL) timerEnd(timer);  // Cleanup the ESP32 timeout timer.
#endif  // ESP32
  delete[] params.rawbuf;
  if (params_save != NULL) {
    delete[] params_save->rawbuf;
    delete params_save;
  }
}

/// Set up and (re)start the IR capture mechanism.
/// @param[in] pullup A flag indicating should the GPIO use the internal pullup
/// resistor. (Default: `false`. i.e. No.)
void IRrecv::enableIRIn(const bool pullup) {
  // ESP32's seem to require explicitly setting the GPIO to INPUT etc.
  // This wasn't required on the ESP8266s, but it shouldn't hurt to make sure.
  if (pullup) {
#ifndef UNIT_TEST
    pinMode(params.recvpin, INPUT_PULLUP);
  } else {
    pinMode(params.recvpin, INPUT);
#endif  // UNIT_TEST
  }
#if defined(ESP32)
  // Initialise the ESP32 timer.
  // 80MHz / 80 = 1 uSec granularity.
  timer = timerBegin(_timer_num, 80, true);
  // Set the timer so it only fires once, and set it's trigger in uSeconds.
  timerAlarmWrite(timer, MS_TO_USEC(params.timeout), ONCE);
  // Note: Interrupt needs to be attached before it can be enabled or disabled.
  // Note: EDGE (true) is not supported, use LEVEL (false). Ref: #1713
  // See: https://github.com/espressif/arduino-esp32/blob/caef4006af491130136b219c1205bdcf8f08bf2b/cores/esp32/esp32-hal-timer.c#L224-L227
  timerAttachInterrupt(timer, &read_timeout, false);
#endif  // ESP32

  // Initialise state machine variables
  resume();

#ifndef UNIT_TEST
#if defined(ESP8266)
  // Initialise ESP8266 timer.
  os_timer_disarm(&timer);
  os_timer_setfn(&timer, reinterpret_cast<os_timer_func_t *>(read_timeout),
                 NULL);
#endif  // ESP8266
  // Attach Interrupt
  attachInterrupt(params.recvpin, gpio_intr, CHANGE);
#endif  // UNIT_TEST
}

/// Stop collection of any received IR data.
/// Disable any timers and interrupts.
void IRrecv::disableIRIn(void) {
#ifndef UNIT_TEST
#if defined(ESP8266)
  os_timer_disarm(&timer);
#endif  // ESP8266
#if defined(ESP32)
  timerAlarmDisable(timer);
  timerEnd(timer);
#endif  // ESP32
  detachInterrupt(params.recvpin);
#endif  // UNIT_TEST
}

/// Resume collection of received IR data.
/// @note This is required if `decode()` is successful and `save_buffer` was
///   not set when the class was instanciated.
/// @see IRrecv class constructor
void IRrecv::resume(void) {
  params.rcvstate = kIdleState;
  params.rawlen = 0;
  params.overflow = false;
#if defined(ESP32)
  timerAlarmDisable(timer);
#endif  // ESP32
}

/// Make a copy of the interrupt state & buffer data.
/// Needed because irparams is marked as volatile, thus memcpy() isn't allowed.
/// Only call this when you know the interrupt handlers won't modify anything.
/// i.e. In kStopState.
/// @param[in] src Pointer to an irparams_t structure to copy from.
/// @param[out] dst Pointer to an irparams_t structure to copy to.
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

/// Obtain the maximum number of entries possible in the capture buffer.
/// i.e. It's size.
/// @return The size of the buffer that is in use by the object.
uint16_t IRrecv::getBufSize(void) { return params.bufsize; }

#if DECODE_HASH
/// Set the minimum length we will consider for reporting UNKNOWN message types.
/// @param[in] length Min nr. of mark/space pulses required to be considered.
void IRrecv::setUnknownThreshold(const uint16_t length) {
  _unknown_threshold = length;
}
#endif  // DECODE_HASH


/// Set the base tolerance percentage for matching incoming IR messages.
/// @param[in] percent An integer percentage. (0-100)
void IRrecv::setTolerance(const uint8_t percent) {
  _tolerance = std::min(percent, (uint8_t)100);
}

/// Get the base tolerance percentage for matching incoming IR messages.
/// @return A integer percentage.
uint8_t IRrecv::getTolerance(void) { return _tolerance; }

#if ENABLE_NOISE_FILTER_OPTION
/// Remove or merge pulses in the capture buffer that are too short.
/// @param[in,out] results Ptr to the decode_results we are going to filter.
/// @param[in] floor Only allow values in the buffer large than this.
///   (in microSeconds)
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

/// Decodes the received IR message.
/// If the interrupt state is saved, we will immediately resume waiting
/// for the next IR message to avoid missing messages.
/// @note There is a trade-off here. Saving the state means less time lost until
/// we can receiving the next message vs. using more RAM. Choose appropriately.
/// @param[out] results A PTR to where the decoded IR message will be stored.
/// @param[out] save A PTR to an irparams_t instance in which to save
///   the interrupt's memory/state. NULL means don't save it.
/// @param[in] max_skip Maximum Nr. of pulses at the begining of a capture we
///   can skip when attempting to find a protocol we can successfully decode.
///   This parameter can dramatically improve detection of protocols
///   when there is light IR interference just before an incoming IR
///   message, however, it comes at a steep performace price.
///   (Default is 0. No skipping.)
/// @warning Increasing the `max_skip` value will dramatically (linearly)
///   increase the cpu time & usage to decode protocols.
///   e.g. 0 -> 1 will be a 2x increase in cpu usage/time.
///        0 -> 2 will be a 3x increase etc.
///   If you are going to do this, consider disabling protocol decoding for
///   protocols you are not expecting.
/// @param[in] noise_floor Pulses below this size (in usecs) will be removed or
///   merged prior to any decoding. This is to try to remove noise/poor
///   readings & slightly increase the chances of a successful decode but at the
///   cost of data fidelity & integrity.
///   (Defaults to 0 usecs. i.e. Don't filter; which is safe!)
/// @warning DANGER: **Here Be Dragons!**
///   If you set the `noise_floor` value too high, it **WILL** break decoding
///   of some protocols. You have been warned!
///   **Any** non-zero value has the potential to **cook** the captured raw data
///   i.e. The raw data is going to lie to you.
///   It may obscure hardware, circuit, & environment issues thus making it
///   impossible to support you accurately or confidently.
///     Values of <= 50 usecs will probably be safe.
///     51 - 100 usecs **might** be okay.
///     100 - 150 usecs is "Danger, Will Robinson!".
///     150 - 200 usecs expect broken protocols.
///     At 200+ usecs, you **have** protocols you can't decode!!
/// @return A boolean indicating if an IR message is ready or not.
bool IRrecv::decode(decode_results *results, irparams_t *save,
                    uint8_t max_skip, uint16_t noise_floor) {
  // Proceed only if an IR message been received.
#ifndef UNIT_TEST
  if (params.rcvstate != kStopState) return false;
#endif

  // Clear the entry we are currently pointing to when we got the timeout.
  // i.e. Stopped collecting IR data.
  // It's junk as we never wrote an entry to it and can only confuse decoding.
  // This is done here rather than logically the best place in read_timeout()
  // as it saves a few bytes of ICACHE_RAM as that routine is bound to an
  // interrupt. decode() is not stored in ICACHE_RAM.
  // Another better option would be to zero the entire irparams.rawbuf[] on
  // resume() but that is a much more expensive operation compare to this.
  // However, don't do this if rawbuf is already full as we stomp over the heap.
  // See: https://github.com/crankyoldgit/IRremoteESP8266/issues/1516
  if (!params.overflow) params.rawbuf[params.rawlen] = 0;

  bool resumed = false;  // Flag indicating if we have resumed.

  // If we were requested to use a save buffer previously, do so.
  if (save == NULL) save = params_save;

  if (save == NULL) {
    // We haven't been asked to copy it so use the existing memory.
#ifndef UNIT_TEST
    results->rawbuf = params.rawbuf;
    results->rawlen = params.rawlen;
    results->overflow = params.overflow;
#endif
  } else {
    copyIrParams(&params, save);  // Duplicate the interrupt's memory.
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
#if DECODE_MILESTAG2
    DPRINTLN("Attempting MilesTag2 decode");
  // Try decodeMilestag2() before decodeSony() because the protocols are
  // similar in timings & structure, but the Miles one differs in nbits
  // so this one should be tried first to try to reduce false detection
    if (decodeMilestag2(results, offset, kMilesTag2MsgBits) ||
        decodeMilestag2(results, offset, kMilesTag2ShotBits)) return true;
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
    DPRINTLN("Attempting Coolix 24-bit decode");
    if (decodeCOOLIX(results, offset)) return true;
#endif  // DECODE_COOLIX
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
    DPRINTLN("Attempting Toshiba AC 72bit decode");
    if (decodeToshibaAC(results, offset)) return true;
    DPRINTLN("Attempting Toshiba AC 80bit decode");
    if (decodeToshibaAC(results, offset, kToshibaACBitsLong)) return true;
    DPRINTLN("Attempting Toshiba AC 56bit decode");
    if (decodeToshibaAC(results, offset, kToshibaACBitsShort)) return true;
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
#if DECODE_HAIER_AC176
    DPRINTLN("Attempting Haier AC 176 bit decode");
    if (decodeHaierAC176(results, offset)) return true;
#endif  // DECODE_HAIER_AC176
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
#if DECODE_HITACHI_AC344
    // HitachiAC344 should be checked before HitachiAC
    DPRINTLN("Attempting Hitachi AC344 decode");
    if (decodeHitachiAC(results, offset, kHitachiAc344Bits, true, false))
      return true;
#endif  // DECODE_HITACHI_AC344
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
    if (decodeSamsungAC(results, offset, kSamsungAcExtendedBits)) return true;
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
#if DECODE_TROTEC_3550
    DPRINTLN("Attempting Trotec 3550 decode");
    if (decodeTrotec3550(results, offset)) return true;
#endif  // DECODE_TROTEC_3550
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
#if DECODE_DELONGHI_AC
    DPRINTLN("Attempting Delonghi AC decode");
    if (decodeDelonghiAc(results, offset)) return true;
#endif  // DECODE_DELONGHI_AC
#if DECODE_DOSHISHA
    DPRINTLN("Attempting Doshisha decode");
    if (decodeDoshisha(results, offset)) return true;
#endif  // DECODE_DOSHISHA
#if DECODE_TRUMA
    // Needs to happen before decodeMultibrackets() as they can appear similar.
    DPRINTLN("Attempting Truma decode");
    if (decodeTruma(results, offset)) return true;
#endif  // DECODE_TRUMA
#if DECODE_MULTIBRACKETS
    DPRINTLN("Attempting Multibrackets decode");
    if (decodeMultibrackets(results, offset)) return true;
#endif  // DECODE_MULTIBRACKETS
#if DECODE_CARRIER_AC40
    DPRINTLN("Attempting Carrier 40bit decode");
    if (decodeCarrierAC40(results, offset)) return true;
#endif  // DECODE_CARRIER_AC40
#if DECODE_CARRIER_AC64
    DPRINTLN("Attempting Carrier 64bit decode");
    if (decodeCarrierAC64(results, offset)) return true;
#endif  // DECODE_CARRIER_AC64
#if DECODE_TECHNIBEL_AC
    DPRINTLN("Attempting Technibel AC decode");
    if (decodeTechnibelAc(results, offset)) return true;
#endif  // DECODE_TECHNIBEL_AC
#if DECODE_CORONA_AC
    DPRINTLN("Attempting CoronaAc decode");
    if (decodeCoronaAc(results, offset)) return true;
#endif  // DECODE_CORONA_AC
#if DECODE_MIDEA24
    DPRINTLN("Attempting Midea-Nec decode");
    if (decodeMidea24(results, offset)) return true;
#endif  // DECODE_MIDEA24
#if DECODE_ZEPEAL
    DPRINTLN("Attempting Zepeal decode");
    if (decodeZepeal(results, offset)) return true;
#endif  // DECODE_ZEPEAL
#if DECODE_SANYO_AC
    DPRINTLN("Attempting Sanyo AC decode");
    if (decodeSanyoAc(results, offset)) return true;
#endif  // DECODE_SANYO_AC
#if DECODE_VOLTAS
  DPRINTLN("Attempting Voltas decode");
  if (decodeVoltas(results)) return true;
#endif  // DECODE_VOLTAS
#if DECODE_METZ
    DPRINTLN("Attempting Metz decode");
    if (decodeMetz(results, offset)) return true;
#endif  // DECODE_METZ
#if DECODE_TRANSCOLD
    DPRINTLN("Attempting Transcold decode");
    if (decodeTranscold(results, offset)) return true;
#endif  // DECODE_TRANSCOLD
#if DECODE_MIRAGE
    DPRINTLN("Attempting Mirage decode");
    if (decodeMirage(results, offset)) return true;
#endif  // DECODE_MIRAGE
#if DECODE_ELITESCREENS
    DPRINTLN("Attempting EliteScreens decode");
    if (decodeElitescreens(results, offset)) return true;
#endif  // DECODE_ELITESCREENS
#if DECODE_PANASONIC_AC32
    DPRINTLN("Attempting Panasonic AC (32bit) long decode");
    if (decodePanasonicAC32(results, offset, kPanasonicAc32Bits)) return true;
    DPRINTLN("Attempting Panasonic AC (32bit) short decode");
    if (decodePanasonicAC32(results, offset, kPanasonicAc32Bits / 2))
      return true;
#endif  // DECODE_PANASONIC_AC32
#if DECODE_ECOCLIM
    DPRINTLN("Attempting Ecoclim decode");
    if (decodeEcoclim(results, offset, kEcoclimBits) ||
        decodeEcoclim(results, offset, kEcoclimShortBits)) return true;
#endif  // DECODE_ECOCLIM
#if DECODE_XMP
    DPRINTLN("Attempting XMP decode");
    if (decodeXmp(results, offset, kXmpBits)) return true;
#endif  // DECODE_XMP
#if DECODE_TEKNOPOINT
    DPRINTLN("Attempting Teknopoint decode");
    if (decodeTeknopoint(results, offset)) return true;
#endif  // DECODE_TEKNOPOINT
#if DECODE_KELON
    DPRINTLN("Attempting Kelon decode");
    if (decodeKelon(results, offset)) return true;
#endif  // DECODE_KELON
#if DECODE_SANYO_AC88
    DPRINTLN("Attempting SanyoAc88 decode");
    if (decodeSanyoAc88(results, offset)) return true;
#endif  // DECODE_SANYO_AC88
#if DECODE_BOSE
    DPRINTLN("Attempting Bose decode");
    if (decodeBose(results, offset)) return true;
#endif  // DECODE_BOSE
#if DECODE_ARRIS
    DPRINTLN("Attempting Arris decode");
    if (decodeArris(results, offset)) return true;
#endif  // DECODE_ARRIS
#if DECODE_RHOSS
    DPRINTLN("Attempting Rhoss decode");
    if (decodeRhoss(results, offset)) return true;
#endif  // DECODE_RHOSS
#if DECODE_AIRTON
    DPRINTLN("Attempting Airton decode");
    if (decodeAirton(results, offset)) return true;
#endif  // DECODE_AIRTON
#if DECODE_COOLIX48
    DPRINTLN("Attempting Coolix 48-bit decode");
    if (decodeCoolix48(results, offset)) return true;
#endif  // DECODE_COOLIX48
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

/// Convert the tolerance percentage into something valid.
/// @param[in] percentage An integer percentage.
uint8_t IRrecv::_validTolerance(const uint8_t percentage) {
    return (percentage > 100) ? _tolerance : percentage;
}

/// Calculate the lower bound of the nr. of ticks.
/// @param[in] usecs Nr. of uSeconds.
/// @param[in] tolerance Percent as an integer. e.g. 10 is 10%
/// @param[in] delta A non-scaling amount to reduce usecs by.
/// @return Nr. of ticks.
uint32_t IRrecv::ticksLow(const uint32_t usecs, const uint8_t tolerance,
                          const uint16_t delta) {
  // max() used to ensure the result can't drop below 0 before the cast.
  return ((uint32_t)std::max(
      (int32_t)(usecs * (1.0 - _validTolerance(tolerance) / 100.0) - delta),
      (int32_t)0));
}

/// Calculate the upper bound of the nr. of ticks.
/// @param[in] usecs Nr. of uSeconds.
/// @param[in] tolerance Percent as an integer. e.g. 10 is 10%
/// @param[in] delta A non-scaling amount to increase usecs by.
/// @return Nr. of ticks.
uint32_t IRrecv::ticksHigh(const uint32_t usecs, const uint8_t tolerance,
                           const uint16_t delta) {
  return ((uint32_t)(usecs * (1.0 + _validTolerance(tolerance) / 100.0)) + 1 +
          delta);
}

/// Check if we match a pulse(measured) with the desired within
///   +/-tolerance percent and/or +/- a fixed delta range.
/// @param[in] measured The recorded period of the signal pulse.
/// @param[in] desired The expected period (in usecs) we are matching against.
/// @param[in] tolerance A percentage expressed as an integer. e.g. 10 is 10%.
/// @param[in] delta A non-scaling (+/-) error margin (in useconds).
/// @return A Boolean. true if it matches, false if it doesn't.
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

/// Check if we match a pulse(measured) of at least desired within
///   tolerance percent and/or a fixed delta margin.
/// @param[in] measured The recorded period of the signal pulse.
/// @param[in] desired The expected period (in usecs) we are matching against.
/// @param[in] tolerance A percentage expressed as an integer. e.g. 10 is 10%.
/// @param[in] delta A non-scaling amount to reduce usecs by.
/// @return A Boolean. true if it matches, false if it doesn't.
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
  DPRINT(ticksLow(std::min(desired, MS_TO_USEC(params.timeout)), tolerance,
                  delta));
  DPRINT(" [min(");
  DPRINT(ticksLow(desired, tolerance, delta));
  DPRINT(", ");
  DPRINT(ticksLow(MS_TO_USEC(params.timeout), tolerance, delta));
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
  return measured >= ticksLow(std::min(desired,
                                       (uint32_t)MS_TO_USEC(params.timeout)),
                              tolerance, delta);
}

/// Check if we match a mark signal(measured) with the desired within
///  +/-tolerance percent, after an expected is excess is added.
/// @param[in] measured The recorded period of the signal pulse.
/// @param[in] desired The expected period (in usecs) we are matching against.
/// @param[in] tolerance A percentage expressed as an integer. e.g. 10 is 10%.
/// @param[in] excess A non-scaling amount to reduce usecs by.
/// @return A Boolean. true if it matches, false if it doesn't.
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

/// Check if we match a mark signal(measured) with the desired within a
/// range (in uSeconds) either side of the desired, after an expected is excess
/// is added.
/// @param[in] measured The recorded period of the signal pulse.
/// @param[in] desired The expected period (in usecs) we are matching against.
/// @param[in] range The range limit from desired to accept in uSeconds.
/// @param[in] excess A non-scaling amount to reduce usecs by.
/// @return A Boolean. true if it matches, false if it doesn't.
bool IRrecv::matchMarkRange(const uint32_t measured, const uint32_t desired,
                            const uint16_t range, const int16_t excess) {
  DPRINT("Matching MARK ");
  DPRINT(measured * kRawTick);
  DPRINT(" vs ");
  DPRINT(desired);
  DPRINT(" + ");
  DPRINT(excess);
  DPRINT(". ");
  return match(measured, desired + excess, 0, range);
}

/// Check if we match a space signal(measured) with the desired within
///  +/-tolerance percent, after an expected is excess is removed.
/// @param[in] measured The recorded period of the signal pulse.
/// @param[in] desired The expected period (in usecs) we are matching against.
/// @param[in] tolerance A percentage expressed as an integer. e.g. 10 is 10%.
/// @param[in] excess A non-scaling amount to reduce usecs by.
/// @return A Boolean. true if it matches, false if it doesn't.
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

/// Check if we match a space signal(measured) with the desired within a
/// range (in uSeconds) either side of the desired, after an expected is excess
/// is removed.
/// @param[in] measured The recorded period of the signal pulse.
/// @param[in] desired The expected period (in usecs) we are matching against.
/// @param[in] range The range limit from desired to accept in uSeconds.
/// @param[in] excess A non-scaling amount to reduce usecs by.
/// @return A Boolean. true if it matches, false if it doesn't.
bool IRrecv::matchSpaceRange(const uint32_t measured, const uint32_t desired,
                             const uint16_t range, const int16_t excess) {
  DPRINT("Matching SPACE ");
  DPRINT(measured * kRawTick);
  DPRINT(" vs ");
  DPRINT(desired);
  DPRINT(" - ");
  DPRINT(excess);
  DPRINT(". ");
  return match(measured, desired - excess, 0, range);
}

#if DECODE_HASH
/// Compare two tick values.
/// @param[in] oldval Nr. of ticks.
/// @param[in] newval Nr. of ticks.
/// @return 0 if newval is shorter, 1 if it is equal, & 2 if it is longer.
/// @note Use a tolerance of 20%
uint16_t IRrecv::compare(const uint16_t oldval, const uint16_t newval) {
  if (newval < oldval * 0.8)
    return 0;
  else if (oldval < newval * 0.8)
    return 2;
  else
    return 1;
}

/// Decode any arbitrary IR message into a 32-bit code value.
/// Instead of decoding using a standard encoding scheme
/// (e.g. Sony, NEC, RC5), the code is hashed to a 32-bit value.
///
/// The algorithm: look at the sequence of MARK signals, and see if each one
/// is shorter (0), the same length (1), or longer (2) than the previous.
/// Do the same with the SPACE signals.  Hash the resulting sequence of 0's,
/// 1's, and 2's to a 32-bit value.  This will give a unique value for each
/// different code (probably), for most code systems.
/// @see http://arcfn.com/2010/01/using-arbitrary-remotes-with-arduino.html
/// @note This isn't a "real" decoding, just an arbitrary value.
///   Hopefully this code is unique for each button.
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

/// Match & decode the typical data section of an IR message.
/// The data value is stored in the least significant bits reguardless of the
/// bit ordering requested.
/// @param[in] data_ptr A pointer to where we are at in the capture buffer.
/// @param[in] nbits Nr. of data bits we expect.
/// @param[in] onemark Nr. of uSeconds in an expected mark signal for a '1' bit.
/// @param[in] onespace Nr. of uSecs in an expected space signal for a '1' bit.
/// @param[in] zeromark Nr. of uSecs in an expected mark signal for a '0' bit.
/// @param[in] zerospace Nr. of uSecs in an expected space signal for a '0' bit.
/// @param[in] tolerance Percentage error margin to allow. (Default: kUseDefTol)
/// @param[in] excess Nr. of uSeconds. (Def: kMarkExcess)
/// @param[in] MSBfirst Bit order to save the data in. (Def: true)
///   true is Most Significant Bit First Order, false is Least Significant First
/// @param[in] expectlastspace Do we expect a space at the end of the message?
/// @return A match_result_t structure containing the success (or not), the
///   data value, and how many buffer entries were used.
match_result_t IRrecv::matchData(
    volatile uint16_t *data_ptr, const uint16_t nbits, const uint16_t onemark,
    const uint32_t onespace, const uint16_t zeromark, const uint32_t zerospace,
    const uint8_t tolerance, const int16_t excess, const bool MSBfirst,
    const bool expectlastspace) {
  match_result_t result;
  result.success = false;  // Fail by default.
  result.data = 0;
  if (expectlastspace) {  // We are expecting data with a final space.
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
  } else {  // We are expecting data without a final space.
    // Match all but the last bit, as it may not match easily.
    result = matchData(data_ptr, nbits ? nbits - 1 : 0, onemark, onespace,
                       zeromark, zerospace, tolerance, excess, true, true);
    if (result.success) {
      // Is the bit a '1'?
      if (matchMark(*(data_ptr + result.used), onemark, tolerance, excess))
        result.data = (result.data << 1) | 1;
      else if (matchMark(*(data_ptr + result.used), zeromark, tolerance,
               excess))
        result.data <<= 1;  // The bit is a '0'.
      else
        result.success = false;
      if (result.success) result.used++;
    }
  }
  if (!MSBfirst) result.data = reverseBits(result.data, nbits);
  return result;
}

/// Match & decode the typical data section of an IR message.
/// The bytes are stored at result_ptr. The first byte in the result equates to
/// the first byte encountered, and so on.
/// @param[in] data_ptr A pointer to where we are at in the capture buffer.
/// @param[out] result_ptr A ptr to where to start storing the bytes we decoded.
/// @param[in] remaining The size of the capture buffer remaining.
/// @param[in] nbytes Nr. of data bytes we expect.
/// @param[in] onemark Nr. of uSeconds in an expected mark signal for a '1' bit.
/// @param[in] onespace Nr. of uSecs in an expected space signal for a '1' bit.
/// @param[in] zeromark Nr. of uSecs in an expected mark signal for a '0' bit.
/// @param[in] zerospace Nr. of uSecs in an expected space signal for a '0' bit.
/// @param[in] tolerance Percentage error margin to allow. (Default: kUseDefTol)
/// @param[in] excess Nr. of uSeconds. (Def: kMarkExcess)
/// @param[in] MSBfirst Bit order to save the data in. (Def: true)
///   true is Most Significant Bit First Order, false is Least Significant First
/// @param[in] expectlastspace Do we expect a space at the end of the message?
/// @return If successful, how many buffer entries were used. Otherwise 0.
uint16_t IRrecv::matchBytes(volatile uint16_t *data_ptr, uint8_t *result_ptr,
                            const uint16_t remaining, const uint16_t nbytes,
                            const uint16_t onemark, const uint32_t onespace,
                            const uint16_t zeromark, const uint32_t zerospace,
                            const uint8_t tolerance, const int16_t excess,
                            const bool MSBfirst, const bool expectlastspace) {
  // Check if there is enough capture buffer to possibly have the desired bytes.
  if (remaining + expectlastspace < (nbytes * 8 * 2) + 1)
    return 0;  // Nope, so abort.
  uint16_t offset = 0;
  for (uint16_t byte_pos = 0; byte_pos < nbytes; byte_pos++) {
    bool lastspace = (byte_pos + 1 == nbytes) ? expectlastspace : true;
    match_result_t result = matchData(data_ptr + offset, 8, onemark, onespace,
                                      zeromark, zerospace, tolerance, excess,
                                      MSBfirst, lastspace);
    if (result.success == false) return 0;  // Fail
    result_ptr[byte_pos] = (uint8_t)result.data;
    offset += result.used;
  }
  return offset;
}

/// Match & decode a generic/typical IR message.
/// The data is stored in result_bits_ptr or result_bytes_ptr depending on flag
/// `use_bits`.
/// @note Values of 0 for hdrmark, hdrspace, footermark, or footerspace mean
/// skip that requirement.
///
/// @param[in] data_ptr A pointer to where we are at in the capture buffer.
/// @param[out] result_bits_ptr A pointer to where to start storing the bits we
///    decoded.
/// @param[out] result_bytes_ptr A pointer to where to start storing the bytes
///    we decoded.
/// @param[in] use_bits A flag indicating if we are to decode bits or bytes.
/// @param[in] remaining The size of the capture buffer remaining.
/// @param[in] nbits Nr. of data bits we expect.
/// @param[in] hdrmark Nr. of uSeconds for the expected header mark signal.
/// @param[in] hdrspace Nr. of uSeconds for the expected header space signal.
/// @param[in] onemark Nr. of uSeconds in an expected mark signal for a '1' bit.
/// @param[in] onespace Nr. of uSecs in an expected space signal for a '1' bit.
/// @param[in] zeromark Nr. of uSecs in an expected mark signal for a '0' bit.
/// @param[in] zerospace Nr. of uSecs in an expected space signal for a '0' bit.
/// @param[in] footermark Nr. of uSeconds for the expected footer mark signal.
/// @param[in] footerspace Nr. of uSeconds for the expected footer space/gap
///   signal.
/// @param[in] atleast Is the match on the footerspace a matchAtLeast or
///   matchSpace?
/// @param[in] tolerance Percentage error margin to allow. (Default: kUseDefTol)
/// @param[in] excess Nr. of uSeconds. (Def: kMarkExcess)
/// @param[in] MSBfirst Bit order to save the data in. (Def: true)
///   true is Most Significant Bit First Order, false is Least Significant First
/// @return If successful, how many buffer entries were used. Otherwise 0.
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
  // Calculate if we expect a trailing space in the data section.
  const bool kexpectspace = footermark || (onespace != zerospace);
  // Calculate how much remaining buffer is required.
  uint16_t min_remaining = nbits * 2 - (kexpectspace ? 0 : 1);

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
                                              excess, MSBfirst, kexpectspace);
    if (!result.success) return 0;
    *result_bits_ptr = result.data;
    offset += result.used;
  } else {  // bytes
    uint16_t data_used = IRrecv::matchBytes(data_ptr + offset, result_bytes_ptr,
                                            remaining - offset, nbits / 8,
                                            onemark, onespace,
                                            zeromark, zerospace, tolerance,
                                            excess, MSBfirst, kexpectspace);
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

/// Match & decode a generic/typical <= 64bit IR message.
/// The data is stored at result_ptr.
/// @note Values of 0 for hdrmark, hdrspace, footermark, or footerspace mean
///   skip that requirement.
///
/// @param[in] data_ptr: A pointer to where we are at in the capture buffer.
/// @param[out] result_ptr A ptr to where to start storing the bits we decoded.
/// @param[in] remaining The size of the capture buffer remaining.
/// @param[in] nbits Nr. of data bits we expect.
/// @param[in] hdrmark Nr. of uSeconds for the expected header mark signal.
/// @param[in] hdrspace Nr. of uSeconds for the expected header space signal.
/// @param[in] onemark Nr. of uSeconds in an expected mark signal for a '1' bit.
/// @param[in] onespace Nr. of uSecs in an expected space signal for a '1' bit.
/// @param[in] zeromark Nr. of uSecs in an expected mark signal for a '0' bit.
/// @param[in] zerospace Nr. of uSecs in an expected space signal for a '0' bit.
/// @param[in] footermark Nr. of uSeconds for the expected footer mark signal.
/// @param[in] footerspace Nr. of uSeconds for the expected footer space/gap
///   signal.
/// @param[in] atleast Is the match on the footerspace a matchAtLeast or
///   matchSpace?
/// @param[in] tolerance Percentage error margin to allow. (Default: kUseDefTol)
/// @param[in] excess Nr. of uSeconds. (Def: kMarkExcess)
/// @param[in] MSBfirst Bit order to save the data in. (Def: true)
///   true is Most Significant Bit First Order, false is Least Significant First
/// @return If successful, how many buffer entries were used. Otherwise 0.
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

/// Match & decode a generic/typical > 64bit IR message.
/// The bytes are stored at result_ptr. The first byte in the result equates to
/// the first byte encountered, and so on.
/// @note Values of 0 for hdrmark, hdrspace, footermark, or footerspace mean
///   skip that requirement.
/// @param[in] data_ptr: A pointer to where we are at in the capture buffer.
/// @param[out] result_ptr A ptr to where to start storing the bytes we decoded.
/// @param[in] remaining The size of the capture buffer remaining.
/// @param[in] nbits Nr. of data bits we expect.
/// @param[in] hdrmark Nr. of uSeconds for the expected header mark signal.
/// @param[in] hdrspace Nr. of uSeconds for the expected header space signal.
/// @param[in] onemark Nr. of uSeconds in an expected mark signal for a '1' bit.
/// @param[in] onespace Nr. of uSecs in an expected space signal for a '1' bit.
/// @param[in] zeromark Nr. of uSecs in an expected mark signal for a '0' bit.
/// @param[in] zerospace Nr. of uSecs in an expected space signal for a '0' bit.
/// @param[in] footermark Nr. of uSeconds for the expected footer mark signal.
/// @param[in] footerspace Nr. of uSeconds for the expected footer space/gap
///   signal.
/// @param[in] atleast Is the match on the footerspace a matchAtLeast or
///   matchSpace?
/// @param[in] tolerance Percentage error margin to allow. (Default: kUseDefTol)
/// @param[in] excess Nr. of uSeconds. (Def: kMarkExcess)
/// @param[in] MSBfirst Bit order to save the data in. (Def: true)
///   true is Most Significant Bit First Order, false is Least Significant First
/// @return If successful, how many buffer entries were used. Otherwise 0.
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

/// Match & decode a generic/typical constant bit time <= 64bit IR message.
/// The data is stored at result_ptr.
/// @note Values of 0 for hdrmark, hdrspace, footermark, or footerspace mean
///   skip that requirement.
/// @param[in] data_ptr A pointer to where we are at in the capture buffer.
/// @note `data_ptr` is assumed to be pointing to a "Mark", not a "Space".
/// @param[out] result_ptr A ptr to where to start storing the bits we decoded.
/// @param[in] remaining The size of the capture buffer remaining.
/// @param[in] nbits Nr. of data bits we expect.
/// @param[in] hdrmark Nr. of uSeconds for the expected header mark signal.
/// @param[in] hdrspace Nr. of uSeconds for the expected header space signal.
/// @param[in] one Nr. of uSeconds in an expected mark signal for a '1' bit.
/// @param[in] zero Nr. of uSeconds in an expected mark signal for a '0' bit.
/// @param[in] footermark Nr. of uSeconds for the expected footer mark signal.
/// @param[in] footerspace Nr. of uSeconds for the expected footer space/gap
///   signal.
/// @param[in] atleast Is the match on the footerspace a matchAtLeast or
///   matchSpace?
/// @param[in] tolerance Percentage error margin to allow. (Default: kUseDefTol)
/// @param[in] excess Nr. of uSeconds. (Def: kMarkExcess)
/// @param[in] MSBfirst Bit order to save the data in. (Def: true)
///   true is Most Significant Bit First Order, false is Least Significant First
/// @return If successful, how many buffer entries were used. Otherwise 0.
/// @note Parameters one + zero add up to the total time for a bit.
///   e.g. mark(one) + space(zero) is a `1`, mark(zero) + space(one) is a `0`.
uint16_t IRrecv::matchGenericConstBitTime(volatile uint16_t *data_ptr,
                                          uint64_t *result_ptr,
                                          const uint16_t remaining,
                                          const uint16_t nbits,
                                          const uint16_t hdrmark,
                                          const uint32_t hdrspace,
                                          const uint16_t one,
                                          const uint32_t zero,
                                          const uint16_t footermark,
                                          const uint32_t footerspace,
                                          const bool atleast,
                                          const uint8_t tolerance,
                                          const int16_t excess,
                                          const bool MSBfirst) {
  uint16_t offset = 0;
  uint64_t result = 0;
  // If we expect a footermark, then this can be processed like normal.
  if (footermark)
    return _matchGeneric(data_ptr, result_ptr, NULL, true, remaining, nbits,
                         hdrmark, hdrspace, one, zero, zero, one,
                         footermark, footerspace, atleast,
                         tolerance, excess, MSBfirst);
  // Overwise handle like normal, except for the last bit. and no footer.
  uint16_t bits = (nbits > 0) ? nbits - 1 : 0;  // Make sure we don't underflow.
  offset = _matchGeneric(data_ptr, &result, NULL, true, remaining, bits,
                         hdrmark, hdrspace, one, zero, zero, one, 0, 0, false,
                         tolerance, excess, true);
  if (!offset) return 0;  // Didn't match.
  // Now for the last bit.
  if (remaining <= offset) return 0;  // Not enough buffer.
  result <<= 1;
  bool last_bit = 0;
  // Is the mark a '1' or a `0`?
  if (matchMark(*(data_ptr + offset), one, tolerance, excess)) {  // 1
    last_bit = 1;
    result |= 1;
  } else if (matchMark(*(data_ptr + offset), zero, tolerance, excess)) {  // 0
    last_bit = 0;
  } else {
    return 0;  // It's neither, so fail.
  }
  offset++;
  uint32_t expected_space = (last_bit ? zero : one) + footerspace;
  // If we are not at the end of the buffer, check for at least the expected
  // space value.
  if (remaining > offset) {
    if (atleast) {
      if (!matchAtLeast(*(data_ptr + offset), expected_space, tolerance,
                        excess))
        return false;
    } else {
      if (!matchSpace(*(data_ptr + offset), expected_space, tolerance))
        return false;
    }
    offset++;
  }
  if (!MSBfirst) result = reverseBits(result, nbits);
  *result_ptr = result;
  return offset;
}

/// Match & decode a Manchester Code <= 64bit IR message.
/// The data is stored at result_ptr.
/// @note Values of 0 for hdrmark, hdrspace, footermark, or footerspace mean
///   skip that requirement.
/// @param[in] data_ptr A pointer to where we are at in the capture buffer.
/// @note `data_ptr` is assumed to be pointing to a "Mark", not a "Space".
/// @param[out] result_ptr A ptr to where to start storing the bits we decoded.
/// @param[in] remaining The size of the capture buffer remaining.
/// @param[in] nbits Nr. of data bits we expect.
/// @param[in] hdrmark Nr. of uSeconds for the expected header mark signal.
/// @param[in] hdrspace Nr. of uSeconds for the expected header space signal.
/// @param[in] half_period Nr. of uSeconds for half the clock's period.
///   i.e. 1/2 wavelength
/// @param[in] footermark Nr. of uSeconds for the expected footer mark signal.
/// @param[in] footerspace Nr. of uSeconds for the expected footer space/gap
///   signal.
/// @param[in] atleast Is the match on the footerspace a matchAtLeast or
///   matchSpace?
/// @param[in] tolerance Percentage error margin to allow. (Default: kUseDefTol)
/// @param[in] excess Nr. of uSeconds. (Def: kMarkExcess)
/// @param[in] MSBfirst Bit order to save the data in. (Def: true)
///   true is Most Significant Bit First Order, false is Least Significant First
/// @param[in] GEThomas Use G.E. Thomas (true) or IEEE 802.3 (false) convention?
/// @return If successful, how many buffer entries were used. Otherwise 0.
/// @see https://en.wikipedia.org/wiki/Manchester_code
/// @see http://ww1.microchip.com/downloads/en/AppNotes/Atmel-9164-Manchester-Coding-Basics_Application-Note.pdf
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
  uint16_t bank = 0;
  uint16_t entry = 0;

  // Calculate how much remaining buffer is required.
  // Shortest case is nbits. Longest case is 2 * nbits.
  uint16_t min_remaining = nbits;

  if (hdrmark) min_remaining++;
  if (hdrspace) min_remaining++;
  if (footermark) min_remaining++;
  // Don't need to extend for footerspace because it could be the end of message

  // Check if there is enough capture buffer to possibly have the message.
  if (remaining < min_remaining) return 0;  // Nope, so abort.

  // Header
  if (hdrmark) {
    entry = *(data_ptr + offset++);
    if (!hdrspace) {  // If we have no Header Space ...
      // Do we have a data 'mark' half period merged with the header mark?
      if (matchMark(entry, hdrmark + half_period,
                    tolerance, excess)) {
        // Looks like we do.
        bank = entry * kRawTick - hdrmark;
      } else if (!matchMark(entry, hdrmark, tolerance, excess)) {
        return 0;  // It's not a normal header mark, so fail.
      }
    } else if (!matchMark(entry, hdrmark, tolerance, excess)) {
      return 0;  // It's not a normal header mark, so fail.
    }
  }
  if (hdrspace) {
    entry = *(data_ptr + offset++);
    // Check to see if the header space has merged with a data space half period
    if (matchSpace(entry, hdrspace + half_period, tolerance, excess)) {
      // Looks like we do.
      bank = entry * kRawTick - hdrspace;
    } else if (!matchSpace(entry, hdrspace, tolerance, excess)) {
      return 0;  // It's not a normal header space, so fail.
    }
  }

  if (!match(bank / kRawTick, half_period, tolerance, excess)) bank = 0;
  // Data
  uint16_t used = matchManchesterData(data_ptr + offset, result_ptr,
                                      remaining - offset, nbits, half_period,
                                      bank, tolerance, excess, MSBfirst,
                                      GEThomas);
  if (!used) return 0;  // Data did match.
  offset += used;
  // Footer
  if (footermark &&
      !(matchMark(*(data_ptr + offset), footermark + half_period,
                  tolerance, excess) ||
        matchMark(*(data_ptr + offset), footermark, tolerance, excess)))
    return 0;
  offset++;
  // If we have something still to match & haven't reached the end of the buffer
  if (footerspace && offset < remaining) {
    if (atleast) {
      if (!matchAtLeast(*(data_ptr + offset), footerspace, tolerance, excess))
        return 0;
    } else {
      if (!matchSpace(*(data_ptr + offset), footerspace, tolerance, excess) &&
          !matchSpace(*(data_ptr + offset), footerspace + half_period,
                      tolerance, excess))
        return 0;
    }
    offset++;
  }
  return offset;
}

/// Match & decode a Manchester Code data (<= 64bits.
/// @param[in] data_ptr A pointer to where we are at in the capture buffer.
/// @note `data_ptr` is assumed to be pointing to a "Mark", not a "Space".
/// @param[out] result_ptr A ptr to where to start storing the bits we decoded.
/// @param[in] remaining The size of the capture buffer remaining.
/// @param[in] nbits Nr. of data bits we expect.
/// @param[in] half_period Nr. of uSeconds for half the clock's period.
///   i.e. 1/2 wavelength
/// @param[in] tolerance Percentage error margin to allow. (Default: kUseDefTol)
/// @param[in] starting_balance Amount of uSeconds to assume exists prior to
///   the current value pointed too.
/// @param[in] excess Nr. of uSeconds. (Def: kMarkExcess)
/// @param[in] MSBfirst Bit order to save the data in. (Def: true)
///   true is Most Significant Bit First Order, false is Least Significant First
/// @param[in] GEThomas Use G.E. Thomas (true) or IEEE 802.3 (false) convention?
/// @return If successful, how many buffer entries were used. Otherwise 0.
/// @see https://en.wikipedia.org/wiki/Manchester_code
/// @see http://ww1.microchip.com/downloads/en/AppNotes/Atmel-9164-Manchester-Coding-Basics_Application-Note.pdf
/// @todo Clean up and optimise this. It is just "get it working code" atm.
uint16_t IRrecv::matchManchesterData(volatile const uint16_t *data_ptr,
                                     uint64_t *result_ptr,
                                     const uint16_t remaining,
                                     const uint16_t nbits,
                                     const uint16_t half_period,
                                     const uint16_t starting_balance,
                                     const uint8_t tolerance,
                                     const int16_t excess,
                                     const bool MSBfirst,
                                     const bool GEThomas) {
  DPRINTLN("DEBUG: Entered matchManchesterData");
  uint16_t offset = 0;
  uint64_t data = 0;
  uint16_t nr_half_periods = 0;
  const uint16_t expected_half_periods = nbits * 2;
  // Flip the bit if we have a starting balance. ie. Carry over from the header.
  bool currentBit = starting_balance ? !GEThomas : GEThomas;
  const uint16_t raw_half_period = half_period / kRawTick;

  // Calculate how much remaining buffer is required.
  // Shortest case is nbits. Longest case is 2 * nbits.
  uint16_t min_remaining = nbits;

  // Check if there is enough capture buffer to possibly have the message.
  if (remaining < min_remaining) {
    DPRINTLN("DEBUG: Ran out of capture buffer!");
    return 0;  // Nope, so abort.
  }

  // Convert to ticks. Optimisation: Saves on math/extra instructions later.
  uint16_t bank = starting_balance / kRawTick;

  // Data
  // Loop through the buffer till we run out of buffer, or nr of half periods.
  // Possible patterns are:
  // short + short = 1 bit (Add the value of the previous bit again)
  // short + long + short = 2 bits (Add the previous bit again, then flip & add)
  // short + long + long + short = 3 bits (add prev, flip & add, flip & add)
  // We can't start with a long.
  //
  // The general approach is thus:
  //   Check we have a short interval, next or in the bank.
  //   If the next timing value is long, act according and reset the bank to
  //     a short balance.
  //   or
  //   If it is short, act accordingly and declare the bank empty.
  //   Repeat.
  while ((offset < remaining || bank) &&
         nr_half_periods < expected_half_periods) {
    // Get the next entry if we haven't anything existing to process.
    DPRINT("DEBUG: Offset = ");
    DPRINTLN(offset);
    if (!bank) bank = *(data_ptr + offset++);
    DPRINT("DEBUG: Bank = ");
    DPRINTLN(bank * kRawTick);
    // Check if we don't have a short interval.
    DPRINTLN("DEBUG: Checking for short interval");
    if (!match(bank, half_period, tolerance, excess)) {
      DPRINTLN("DEBUG: It is. Exiting");
      return 0;  // Not valid.
    }
    // We've succeeded in matching half a period, so count it.
    nr_half_periods++;
    DPRINT("DEBUG: Half Periods = ");
    DPRINTLN(nr_half_periods);
    // We've now used up our bank, so refill it with the next item, unless we
    // are at the end of the capture buffer.
    // If we are assume a single half period of "space".
    if (offset < remaining) {
      DPRINT("DEBUG: Offset = ");
      DPRINTLN(offset);
      bank = *(data_ptr + offset++);
    } else if (offset == remaining) {
      bank = raw_half_period;
    } else {
      return 0;  // We are out of buffer, so abort!
    }
    DPRINT("DEBUG: Bank = ");
    DPRINTLN(bank * kRawTick);

    // Shift the data along and add our new bit.
    DPRINT("DEBUG: Adding bit: ");
    DPRINTLN((currentBit ? "1" : "0"));
    data <<= 1;
    data |= currentBit;

    // Check if we have a long interval.
    if (match(bank, half_period * 2, tolerance, excess)) {
      // It is, so flip the bit we need to append, and remove a half_period of
      // time from the bank.
      DPRINTLN("DEBUG: long interval detected");
      currentBit = !currentBit;
      bank -= raw_half_period;
    } else if (match(bank, half_period, tolerance, excess)) {
      // It is a short interval, so eat up all the time and move on.
      DPRINTLN("DEBUG: short interval detected");
      bank = 0;
    } else if (nr_half_periods == expected_half_periods - 1 &&
               matchAtLeast(bank, half_period, tolerance, excess)) {
      // We are at the end of the data & it is a short interval, so eat up all
      // the time and move on.
      bank = 0;
      // Reduce the offset as we are at the end of the data doing a
      // matchAtLeast() because  we could be processing part of a footer.
      offset--;
    } else {
      // The length isn't what we expected (neither long or short), so bail.
      return 0;
    }
    nr_half_periods++;
  }

  // Clean up and process the data.
  if (!MSBfirst) data = reverseBits(data, nbits);
  // Trim the data to size.
  *result_ptr = GETBITS64(data, 0, nbits);
  return offset;
}

#if UNIT_TEST
/// Unit test helper to get access to the params structure.
volatile irparams_t *IRrecv::_getParamsPtr(void) {
  return &params;
}
#endif  // UNIT_TEST
// End of IRrecv class -------------------
