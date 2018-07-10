// Copyright 2009 Ken Shirriff
// Copyright 2015 Mark Szabo
// Copyright 2017 David Conran

#include "IRsend.h"
#ifndef UNIT_TEST
#include <Arduino.h>
#else
#define __STDC_LIMIT_MACROS
#include <stdint.h>
#endif
#include <algorithm>
#ifdef UNIT_TEST
#include <cmath>
#endif
#include "IRtimer.h"

// Originally from https://github.com/shirriff/Arduino-IRremote/
// Updated by markszabo (https://github.com/markszabo/IRremoteESP8266) for
// sending IR code on ESP8266

// IRsend ----------------------------------------------------------------------
// Create an IRsend object.
//
// Args:
//   IRsendPin:  Which GPIO pin to use when sending an IR command.
//   inverted:   *DANGER* Optional flag to invert the output. (default = false)
//               e.g. LED is illuminated when GPIO is LOW rather than HIGH.
//               Setting this to something other than the default could
//               easily destroy your IR LED if you are overdriving it.
//               Unless you *REALLY* know what you are doing, don't change this.
// Returns:
//   An IRsend object.
IRsend::IRsend(uint16_t IRsendPin, bool inverted) : IRpin(IRsendPin),
    periodOffset(PERIOD_OFFSET) {
  if (inverted) {
    outputOn = LOW;
    outputOff = HIGH;
  } else {
    outputOn = HIGH;
    outputOff = LOW;
  }
}

// Enable the pin for output.
void IRsend::begin() {
#ifndef UNIT_TEST
  pinMode(IRpin, OUTPUT);
#endif
}

// Turn off the IR LED.
void IRsend::ledOff() {
#ifndef UNIT_TEST
  digitalWrite(IRpin, outputOff);
#endif
}

// Calculate the period for a given frequency. (T = 1/f)
//
// Args:
//   freq: Frequency in Hz.
//   use_offset: Should we use the calculated offset or not?
// Returns:
//   nr. of uSeconds.
uint32_t IRsend::calcUSecPeriod(uint32_t hz, bool use_offset) {
  if (hz == 0) hz = 1;  // Avoid Zero hz. Divide by Zero is nasty.
  uint32_t period = (1000000UL + hz/2) / hz;  // The equiv of round(1000000/hz).
  // Apply the offset and ensure we don't result in a <= 0 value.
  if (use_offset)
    return std::max((uint32_t) 1, period + periodOffset);
  else
    return std::max((uint32_t) 1, period);
}

// Set the output frequency modulation and duty cycle.
//
// Args:
//   freq: The freq we want to modulate at. Assumes < 1000 means kHz else Hz.
//   duty: Percentage duty cycle of the LED. e.g. 25 = 25% = 1/4 on, 3/4 off.
//
// Note:
//   Integer timing functions & math mean we can't do fractions of
//   microseconds timing. Thus minor changes to the freq & duty values may have
//   limited effect. You've been warned.
void IRsend::enableIROut(uint32_t freq, uint8_t duty) {
  // Can't have more than 100% duty cycle.
  duty = std::min(duty, (uint8_t) 100);
  if (freq < 1000)  // Were we given kHz? Supports the old call usage.
    freq *= 1000;
  uint32_t period = calcUSecPeriod(freq);
  // Nr. of uSeconds the LED will be on per pulse.
  onTimePeriod = (period * duty) / 100;
  // Nr. of uSeconds the LED will be off per pulse.
  offTimePeriod = period - onTimePeriod;
}

// Modulate the IR LED for the given period (usec) and at the duty cycle set.
//
// Args:
//   usec: The period of time to modulate the IR LED for, in microseconds.
// Returns:
//   Nr. of pulses actually sent.
//
// Note:
//   The ESP8266 has no good way to do hardware PWM, so we have to do it all
//   in software. There is a horrible kludge/brilliant hack to use the second
//   serial TX line to do fairly accurate hardware PWM, but it is only
//   available on a single specific GPIO and only available on some modules.
//   e.g. It's not available on the ESP-01 module.
//   Hence, for greater compatibility & choice, we don't use that method.
// Ref:
//   https://www.analysir.com/blog/2017/01/29/updated-esp8266-nodemcu-backdoor-upwm-hack-for-ir-signals/
uint16_t IRsend::mark(uint16_t usec) {
  uint16_t counter = 0;
  IRtimer usecTimer = IRtimer();
  // Cache the time taken so far. This saves us calling time, and we can be
  // assured that we can't have odd math problems. i.e. unsigned under/overflow.
  uint32_t elapsed = usecTimer.elapsed();

  while (elapsed < usec) {  // Loop until we've met/exceeded our required time.
#ifndef UNIT_TEST
    digitalWrite(IRpin, outputOn);  // Turn the LED on.
    // Calculate how long we should pulse on for.
    // e.g. Are we to close to the end of our requested mark time (usec)?
    delayMicroseconds(std::min((uint32_t) onTimePeriod, usec - elapsed));
    digitalWrite(IRpin, outputOff);  // Turn the LED off.
#endif
    counter++;
    if (elapsed + onTimePeriod >= usec)
      return counter;  // LED is now off & we've passed our allotted time.
    // Wait for the lesser of the rest of the duty cycle, or the time remaining.
#ifndef UNIT_TEST
    delayMicroseconds(std::min(usec - elapsed - onTimePeriod,
                               (uint32_t) offTimePeriod));
#endif
    elapsed = usecTimer.elapsed();  // Update & recache the actual elapsed time.
  }
  return counter;
}

// Turn the pin (LED) off for a given time.
// Sends an IR space for the specified number of microseconds.
// A space is no output, so the PWM output is disabled.
//
// Args:
//   time: Time in microseconds (us).
void IRsend::space(uint32_t time) {
  ledOff();
  if (time == 0) return;
#ifndef UNIT_TEST
  // delayMicroseconds is only accurate to 16383us.
  // Ref: https://www.arduino.cc/en/Reference/delayMicroseconds
  if (time <= 16383) {
    delayMicroseconds(time);
  } else {
    // Invoke a delay(), where possible, to avoid triggering the WDT.
    delay(time / 1000UL);  // Delay for as many whole milliseconds as we can.
    // Delay the remaining sub-millisecond.
    delayMicroseconds(static_cast<uint16_t>(time % 1000UL));
  }
#endif
}

// Calculate & set any offsets to account for execution times.
//
// Args:
//   hz: The frequency to calibrate at >= 1000Hz. Default is 38000Hz.
//
// Status:  ALPHA / Untested.
//
// NOTE:
//   This will generate an 65535us mark() IR LED signal.
//   This only needs to be called once, if at all.
void IRsend::calibrate(uint16_t hz) {
  if (hz < 1000)  // Were we given kHz? Supports the old call usage.
    hz *= 1000;
  periodOffset = 0;  // Turn off any existing offset while we calibrate.
  enableIROut(hz);
  IRtimer usecTimer = IRtimer();  // Start a timer *just* before we do the call.
  uint16_t pulses = mark(UINT16_MAX);  // Generate a PWM of 65,535 us. (Max.)
  uint32_t timeTaken = usecTimer.elapsed();  // Record the time it took.
  // While it shouldn't be necessary, assume at least 1 pulse, to avoid a
  // divide by 0 situation.
  pulses = std::max(pulses, (uint16_t) 1U);
  uint32_t calcPeriod = calcUSecPeriod(hz);  // e.g. @38kHz it should be 26us.
  // Assuming 38kHz for the example calculations:
  // In a 65535us pulse, we should have 2520.5769 pulses @ 26us periods.
  // e.g. 65535.0us / 26us = 2520.5769
  // This should have caused approx 2520 loops through the main loop in mark().
  // The average over that many interations should give us a reasonable
  // approximation at what offset we need to use to account for instruction
  // execution times.
  //
  // Calculate the actual period from the actual time & the actual pulses
  // generated.
  double_t actualPeriod = (double_t) timeTaken / (double_t) pulses;
  // Store the difference between the actual time per period vs. calculated.
  periodOffset = (int8_t) ((double_t) calcPeriod - actualPeriod);
}

// Generic method for sending data that is common to most protocols.
// Will send leading or trailing 0's if the nbits is larger than the number
// of bits in data.
//
// Args:
//   onemark:    Nr. of usecs for the led to be pulsed for a '1' bit.
//   onespace:   Nr. of usecs for the led to be fully off for a '1' bit.
//   zeromark:   Nr. of usecs for the led to be pulsed for a '0' bit.
//   zerospace:  Nr. of usecs for the led to be fully off for a '0' bit.
//   data:       The data to be transmitted.
//   nbits:      Nr. of bits of data to be sent.
//   MSBfirst:   Flag for bit transmission order. Defaults to MSB->LSB order.
void IRsend::sendData(uint16_t onemark, uint32_t onespace,
                      uint16_t zeromark, uint32_t zerospace,
                      uint64_t data, uint16_t nbits, bool MSBfirst) {
  if (nbits == 0)  // If we are asked to send nothing, just return.
    return;
  if (MSBfirst) {  // Send the MSB first.
    // Send 0's until we get down to a bit size we can actually manage.
    while (nbits > sizeof(data) * 8) {
      mark(zeromark);
      space(zerospace);
      nbits--;
    }
    // Send the supplied data.
    for (uint64_t mask = 1ULL << (nbits - 1);  mask;  mask >>= 1)
      if (data & mask) {  // Send a 1
        mark(onemark);
        space(onespace);
      } else {  // Send a 0
        mark(zeromark);
        space(zerospace);
      }
  } else {  // Send the Least Significant Bit (LSB) first / MSB last.
    for (uint16_t bit = 0; bit < nbits; bit++, data >>= 1)
      if (data & 1) {  // Send a 1
        mark(onemark);
        space(onespace);
      } else {  // Send a 0
        mark(zeromark);
        space(zerospace);
      }
  }
}

// Send a raw IRremote message.
//
// Args:
//   buf: An array of uint16_t's that has microseconds elements.
//   len: Nr. of elements in the buf[] array.
//   hz:  Frequency to send the message at. (kHz < 1000; Hz >= 1000)
//
// Status: STABLE / Known working.
//
// Notes:
//   Even elements are Mark times (On), Odd elements are Space times (Off).
//
// Ref:
//   examples/IRrecvDumpV2/IRrecvDumpV2.ino
void IRsend::sendRaw(uint16_t buf[], uint16_t len, uint16_t hz) {
  // Set IR carrier frequency
  enableIROut(hz);
  for (uint16_t i = 0; i < len; i++) {
    if (i & 1) {  // Odd bit.
      space(buf[i]);
    } else {  // Even bit.
      mark(buf[i]);
    }
  }
  ledOff();  // We potentially have ended with a mark(), so turn of the LED.
}

#ifndef UNIT_TEST
void IRsend::send(uint16_t type, uint64_t data, uint16_t nbits) {
  switch (type) {
#if SEND_NEC
    case NEC: sendNEC(data, nbits); break;
#endif
#if SEND_SONY
    case SONY: sendSony(data, nbits); break;
#endif
#if SEND_RC5
    case RC5: sendRC5(data, nbits); break;
#endif
#if SEND_RC6
    case RC6: sendRC6(data, nbits); break;
#endif
#if SEND_DISH
    case DISH: sendDISH(data, nbits); break;
#endif
#if SEND_JVC
    case JVC: sendJVC(data, nbits); break;
#endif
#if SEND_SAMSUNG
    case SAMSUNG: sendSAMSUNG(data, nbits); break;
#endif
#if SEND_LG
    case LG: sendLG(data, nbits); break;
#endif
#if SEND_WHYNTER
    case WHYNTER: sendWhynter(data, nbits); break;
#endif
#if SEND_COOLIX
    case COOLIX: sendCOOLIX(data, nbits); break;
#endif
#if SEND_DENON
    case DENON: sendDenon(data, nbits); break;
#endif
#if SEND_SHERWOOD
    case SHERWOOD: sendSherwood(data, nbits); break;
#endif
#if SEND_RCMM
    case RCMM: sendRCMM(data, nbits); break;
#endif
#if SEND_MITSUBISHI
    case MITSUBISHI: sendMitsubishi(data, nbits); break;
#endif
#if SEND_SHARP
    case SHARP: sendSharpRaw(data, nbits); break;
#endif
#if SEND_AIWA_RC_T501
    case AIWA_RC_T501: sendAiwaRCT501(data, nbits); break;
#endif
  }
}
#endif
