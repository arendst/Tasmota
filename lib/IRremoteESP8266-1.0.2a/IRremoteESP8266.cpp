 /***************************************************
 * IRremote for ESP8266
 *
 * Based on the IRremote library for Arduino by Ken Shirriff
 * Version 0.11 August, 2009
 * Copyright 2009 Ken Shirriff
 * For details, see
 *   http://arcfn.com/2009/08/multi-protocol-infrared-remote-library.html
 *
 * Modified by Paul Stoffregen <paul@pjrc.com> to support other boards and
 *   timers
 * Modified by Mitra Ardron <mitra@mitra.biz>
 * Added Sanyo and Mitsubishi controllers
 * Modified Sony to spot the repeat codes that some Sony's send
 *
 * Interrupt code based on NECIRrcv by Joe Knapp
 * http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1210243556
 * Also influenced by
 *   http://zovirl.com/2008/11/12/building-a-universal-remote-with-an-arduino/
 *
 * JVC and Panasonic protocol added by Kristian Lauszus (Thanks to zenwheel and
 *   other people at the original blog post)
 * LG added by Darryl Smith (based on the JVC protocol)
 * Whynter A/C ARC-110WD added by Francesco Meschia
 * Global Cache IR format sender added by Hisham Khalifa
 *   (http://www.hishamkhalifa.com)
 * Coolix A/C / heatpump added by bakrus
 * Denon: sendDenon, decodeDenon added by Massimiliano Pinto
 *   (from https://github.com/z3t0/Arduino-IRremote/blob/master/ir_Denon.cpp)
 * Kelvinator A/C and Sherwood added by crankyoldgit
 * Mitsubishi A/C added by crankyoldgit
 *     (derived from https://github.com/r45635/HVAC-IR-Control)
 *
 * Updated by markszabo (https://github.com/markszabo/IRremoteESP8266) for
 *   sending IR code on ESP8266
 * Updated by Sebastien Warin (http://sebastien.warin.fr) for receiving IR code
 *   on ESP8266
 *
 *  GPL license, all text above must be included in any redistribution
 ****************************************************/

#include "IRremoteESP8266.h"
#include "IRremoteInt.h"
#include "IRKelvinator.h"
#include "IRMitsubishiAC.h"

// These versions of MATCH, MATCH_MARK, and MATCH_SPACE are only for debugging.
// To use them, set DEBUG in IRremoteInt.h
// Normally macros are used for efficiency
#ifdef DEBUG
int MATCH(int measured, int desired) {
  Serial.print("Testing: ");
  Serial.print(TICKS_LOW(desired), DEC);
  Serial.print(" <= ");
  Serial.print(measured, DEC);
  Serial.print(" <= ");
  Serial.println(TICKS_HIGH(desired), DEC);
  return measured >= TICKS_LOW(desired) && measured <= TICKS_HIGH(desired);
}

int MATCH_MARK(int measured_ticks, int desired_us) {
  Serial.print("Testing mark ");
  Serial.print(measured_ticks * USECPERTICK, DEC);
  Serial.print(" vs ");
  Serial.print(desired_us, DEC);
  Serial.print(": ");
  Serial.print(TICKS_LOW(desired_us + MARK_EXCESS), DEC);
  Serial.print(" <= ");
  Serial.print(measured_ticks, DEC);
  Serial.print(" <= ");
  Serial.println(TICKS_HIGH(desired_us + MARK_EXCESS), DEC);
  return measured_ticks >= TICKS_LOW(desired_us + MARK_EXCESS) &&
      measured_ticks <= TICKS_HIGH(desired_us + MARK_EXCESS);
}

int MATCH_SPACE(int measured_ticks, int desired_us) {
  Serial.print("Testing space ");
  Serial.print(measured_ticks * USECPERTICK, DEC);
  Serial.print(" vs ");
  Serial.print(desired_us, DEC);
  Serial.print(": ");
  Serial.print(TICKS_LOW(desired_us - MARK_EXCESS), DEC);
  Serial.print(" <= ");
  Serial.print(measured_ticks, DEC);
  Serial.print(" <= ");
  Serial.println(TICKS_HIGH(desired_us - MARK_EXCESS), DEC);
  return measured_ticks >= TICKS_LOW(desired_us - MARK_EXCESS) &&
      measured_ticks <= TICKS_HIGH(desired_us - MARK_EXCESS);
}
#else
int MATCH(int measured, int desired) {return measured >= TICKS_LOW(desired) &&
    measured <= TICKS_HIGH(desired);}
int MATCH_MARK(int measured_ticks, int desired_us)
    {return MATCH(measured_ticks, (desired_us + MARK_EXCESS));}
int MATCH_SPACE(int measured_ticks, int desired_us)
    {return MATCH(measured_ticks, (desired_us - MARK_EXCESS));}
// Debugging versions are in IRremote.cpp
#endif

// IRtimer ---------------------------------------------------------------------
// This class performs a simple time in useconds since instantiated.
// Handles when the system timer wraps around (once).

IRtimer::IRtimer() {
  reset();
}

void IRtimer::reset() {
  start = micros();
}

uint32_t IRtimer::elapsed() {
  uint32_t now = micros();
  if (start <= now)  // Check if the system timer has wrapped.
    return (now - start);  // No wrap.
  else
    return (0xFFFFFFFF - start + now);  // Has wrapped.
}

// IRsend ----------------------------------------------------------------------

IRsend::IRsend(int IRsendPin) {
	IRpin = IRsendPin;
}

void IRsend::begin() {
	pinMode(IRpin, OUTPUT);
}
// Generic method for sending data that is common to most protocols.
// Default to transmitting the Most Significant Bit (MSB) first.
void IRsend::sendData(uint16_t onemark, uint32_t onespace,
                      uint16_t zeromark, uint32_t zerospace,
                      uint32_t data, uint8_t nbits, bool MSBfirst) {
  if (MSBfirst)  // Send the MSB first.
    for (uint32_t mask = 1UL << (nbits - 1);  mask;  mask >>= 1)
      if (data & mask) {  // 1
        mark(onemark);
        space(onespace);
      } else {  // 0
        mark(zeromark);
        space(zerospace);
      }
  else {  // Send the Least Significant Bit (LSB) first / MSB last.
    for (uint8_t bit = 0; bit < nbits; bit++, data >>= 1)
      if (data & 1) {  // 1
        mark(onemark);
        space(onespace);
      } else {  // 0
        mark(zeromark);
        space(zerospace);
      }
  }
}

void IRsend::sendCOOLIX(unsigned long data, int nbits) {
  // Set IR carrier frequency
  enableIROut(38);
  // Header
  mark(COOLIX_HDR_MARK);
  space(COOLIX_HDR_SPACE);
  // Data
  // Sending 3 bytes of data. Each byte first being sent straight, then followed
  // by an inverted version.
  unsigned long COOLIXmask;
  bool invert = 0;  // Initializing
  for (int j = 0; j < COOLIX_NBYTES * 2; j++) {
    for (int i = nbits; i > nbits-8; i--) {
      // Type cast necessary to perform correct for the one byte above 16bit
      COOLIXmask = (unsigned long) 1 << (i-1);
      if (data & COOLIXmask) {  // 1
        mark(COOLIX_BIT_MARK);
        space(COOLIX_ONE_SPACE);
      } else {  // 0
        mark(COOLIX_BIT_MARK);
        space(COOLIX_ZERO_SPACE);
      }
    }
    // Inverts all of the data each time we need to send an inverted byte
    data ^= 0xFFFFFFFF;
    invert = !invert;
    // Subtract 8 from nbits each time we switch to a new byte.
    nbits -= invert ? 0 : 8;
  }
  // Footer
  mark(COOLIX_BIT_MARK);
  space(COOLIX_ZERO_SPACE);   // Stop bit (0)
  space(COOLIX_HDR_SPACE);    // Pause before repeating
}

void IRsend::sendNEC (unsigned long data, int nbits, unsigned int repeat) {
  // Details about timings can be found at:
  //   http://www.sbprojects.com/knowledge/ir/nec.php

  // Set IR carrier frequency
  enableIROut(38);
  IRtimer usecs = IRtimer();
  // Header
  mark(NEC_HDR_MARK);
  space(NEC_HDR_SPACE);
  // Data
  sendData(NEC_BIT_MARK, NEC_ONE_SPACE, NEC_BIT_MARK, NEC_ZERO_SPACE,
           data, nbits, true);
  // Footer
  mark(NEC_BIT_MARK);
  // Gap to next command.
  space(NEC_MIN_COMMAND_LENGTH - usecs.elapsed());

  // Optional command repeat sequence.
  for (unsigned int i = 0; i < repeat; i++) {
    usecs.reset();
    mark(NEC_HDR_MARK);
    space(NEC_RPT_SPACE);
    mark(NEC_BIT_MARK);
    // Gap till next command.
    space(NEC_MIN_COMMAND_LENGTH - usecs.elapsed());
  }
}

void IRsend::sendLG (unsigned long data, int nbits) {
  // Set IR carrier frequency
  enableIROut(38);
  // Header
  mark(LG_HDR_MARK);
  space(LG_HDR_SPACE);
  mark(LG_BIT_MARK);
  // Data
  for (unsigned long mask = 1UL << (nbits - 1); mask; mask >>= 1) {
    if (data & mask) {  // 1
      space(LG_ONE_SPACE);
      mark(LG_BIT_MARK);
    } else {  // 0
      space(LG_ZERO_SPACE);
      mark(LG_BIT_MARK);
    }
  }
  // Footer
  ledOff();
}

void IRsend::sendWhynter(unsigned long data, int nbits) {
  // Set IR carrier frequency
  enableIROut(38);
  // Header
  mark(WHYNTER_ZERO_MARK);
  space(WHYNTER_ZERO_SPACE);
  mark(WHYNTER_HDR_MARK);
  space(WHYNTER_HDR_SPACE);
  // Data
  for (unsigned long mask = 1UL << (nbits - 1); mask; mask >>= 1) {
    if (data & mask) {  // 1
      mark(WHYNTER_ONE_MARK);
      space(WHYNTER_ONE_SPACE);
    } else {  // 0
      mark(WHYNTER_ZERO_MARK);
      space(WHYNTER_ZERO_SPACE);
    }
  }
  // Footer
  mark(WHYNTER_ZERO_MARK);
  space(WHYNTER_ZERO_SPACE);
}

void IRsend::sendSony(unsigned long data, int nbits, unsigned int repeat) {
  // Send an IR command to a compatible Sony device.
  //
  // Args:
  //   data: IR command to be sent.
  //   nbits: Nr. of bits of the IR command to be sent.
  //   repeat: Nr. of additional times the IR command is to be sent.
  //
  // sendSony() should typically be called with repeat=2 as Sony devices
  // expect the code to be sent at least 3 times.
  //
  // Timings and details are taken from:
  //   http://www.sbprojects.com/knowledge/ir/sirc.php

  enableIROut(40);  // Sony devices use a 40kHz IR carrier frequency.
  IRtimer usecs = IRtimer();

  for (uint16_t i = 0; i <= repeat; i++) {  // Typically loop 3 or more times.
    usecs.reset();
    // Header
    mark(SONY_HDR_MARK);
    space(SONY_HDR_SPACE);
    // Data
    sendData(SONY_ONE_MARK, SONY_HDR_SPACE, SONY_ZERO_MARK, SONY_HDR_SPACE,
             data, nbits, true);
    // Footer
    // The Sony protocol requires us to wait 45ms from start of a code to the
    // start of the next one. A 10ms minimum gap is also required.
    space(max(10000, 45000 - usecs.elapsed()));
  }
  // A space() is always performed last, so no need to turn off the LED.
}

void IRsend::sendRaw(unsigned int buf[], int len, int hz) {
  // Set IR carrier frequency
  enableIROut(hz);
  for (int i = 0; i < len; i++) {
    if (i & 1) { // Odd bit.
      space(buf[i]);
    } else {  // Even bit.
      mark(buf[i]);
    }
  }
  ledOff();
}

// Global Cache format w/o emitter ID or request ID. Starts from hertz,
// followed by number of times to emit (count),
// followed by offset for repeats, followed by code as units of periodic time.
void IRsend::sendGC(unsigned int buf[], int len) {
  int khz = buf[0]/1000; // GC data starts with frequency in Hz.
  enableIROut(khz);
  int periodic_time = 1000/khz;
  int count = buf[1]; // Max 50 as per GC.
  // Data
  for (int i = 0; i < count; i++) {
    // Account for offset if we're repeating, otherwise start at index 3.
    int j = i > 0 ? buf[2] + 2 : 3;
    for (; j < len; j++) {
      // Convert periodic units to microseconds. Minimum is 80 for actual GC
      // units.
      int microseconds = buf[j] * periodic_time;
      if (j & 1) {  // Odd bit.
        // Our codes start at an odd index (not even as with sendRaw).
        mark(microseconds);
      } else {  // Even bit.
        space(microseconds);
      }
    }
  }
  // Footer
  ledOff();
}

// Note: first bit must be a one (start bit)
void IRsend::sendRC5(unsigned long data, int nbits) {
  // Set IR carrier frequency
  enableIROut(36);
  // Header
  mark(RC5_T1);  // First start bit
  space(RC5_T1);  // Second start bit
  mark(RC5_T1);  // Second start bit
  // Data
  for (unsigned long mask = 1UL << (nbits - 1); mask; mask >>= 1) {
    if (data & mask) {  // 1
      space(RC5_T1);  // 1 is space, then mark
      mark(RC5_T1);
    } else {  // 0
      mark(RC5_T1);
      space(RC5_T1);
    }
  }
  // Footer
  ledOff();
}

// Caller needs to take care of flipping the toggle bit
void IRsend::sendRC6(unsigned long data, int nbits) {
  // Set IR carrier frequency
  enableIROut(36);
  // Header
  mark(RC6_HDR_MARK);
  space(RC6_HDR_SPACE);
  mark(RC6_T1);  // Start bit
  space(RC6_T1);
  int t;
  // Data
  for (unsigned long i = 0, mask = 1UL << (nbits - 1); mask; i++, mask >>= 1) {
    // The fourth bit we send is a "double width trailer bit".
    if (i == 3) {
      // double-wide trailer bit
      t = 2 * RC6_T1;
    } else {
      t = RC6_T1;
    }
    if (data & mask) {  // 1
      mark(t);
      space(t);
    } else {  // 0
      space(t);
      mark(t);
    }
  }
  // Footer
  ledOff();
}

void IRsend::sendPanasonic(unsigned int address, unsigned long data) {
  // Set IR carrier frequency
  enableIROut(35);
  // Header
  mark(PANASONIC_HDR_MARK);
  space(PANASONIC_HDR_SPACE);
  // Address (16 bits)
  sendData(PANASONIC_BIT_MARK, PANASONIC_ONE_SPACE,
           PANASONIC_BIT_MARK, PANASONIC_ZERO_SPACE,
           address, 16, true);
  // Data (32 bits)
  sendData(PANASONIC_BIT_MARK, PANASONIC_ONE_SPACE,
           PANASONIC_BIT_MARK, PANASONIC_ZERO_SPACE,
           data, 32, true);
  // Footer
  mark(PANASONIC_BIT_MARK);
  ledOff();
}

void IRsend::sendJVC(unsigned long data, int nbits, int repeat) {
  // Set IR carrier frequency
  enableIROut(38);
  // Header
  if (!repeat) {
    mark(JVC_HDR_MARK);
    space(JVC_HDR_SPACE);
  }
  // Data
  for (unsigned long mask = 1UL << (nbits - 1); mask; mask >>= 1) {
    if (data & mask) {  // 1
       mark(JVC_BIT_MARK);
       space(JVC_ONE_SPACE);
    } else {  // 0
       mark(JVC_BIT_MARK);
       space(JVC_ZERO_SPACE);
    }
  }
  // Footer
  mark(JVC_BIT_MARK);
  ledOff();
}

void IRsend::sendSAMSUNG(unsigned long data, int nbits) {
  // Set IR carrier frequency
  enableIROut(38);
  // Header
  mark(SAMSUNG_HDR_MARK);
  space(SAMSUNG_HDR_SPACE);
  // Data
  for (unsigned long mask = 1UL << (nbits - 1); mask; mask >>= 1) {
    if (data & mask) {  // 1
      mark(SAMSUNG_BIT_MARK);
      space(SAMSUNG_ONE_SPACE);
    } else { // 0
      mark(SAMSUNG_BIT_MARK);
      space(SAMSUNG_ZERO_SPACE);
    }
  }
  // Footer
  mark(SAMSUNG_BIT_MARK);
  ledOff();
}

// Denon, from https://github.com/z3t0/Arduino-IRremote/blob/master/ir_Denon.cpp
void IRsend::sendDenon (unsigned long data,  int nbits) {
  // Set IR carrier frequency
  enableIROut(38);
  // Header
  mark(DENON_HDR_MARK);
  space(DENON_HDR_SPACE);
  // Data
  for (unsigned long mask = 1UL << (nbits - 1);  mask;  mask >>= 1) {
    if (data & mask) {  // 1
      mark (DENON_BIT_MARK);
      space(DENON_ONE_SPACE);
    } else {  // 0
      mark (DENON_BIT_MARK);
      space(DENON_ZERO_SPACE);
    }
  }
  // Footer
  mark(DENON_BIT_MARK);
  ledOff();
}

void IRsend::mark(unsigned int usec) {
  // Sends an IR mark for the specified number of microseconds.
  // The mark output is modulated at the PWM frequency.
  IRtimer usecTimer = IRtimer();
  while (usecTimer.elapsed() < usec) {
    digitalWrite(IRpin, HIGH);
    delayMicroseconds(halfPeriodicTime);
    digitalWrite(IRpin, LOW);
    // e.g. 38 kHz -> T = 26.31 microsec (periodic time), half of it is 13
    delayMicroseconds(halfPeriodicTime);
  }
}

void IRsend::ledOff() {
  digitalWrite(IRpin, LOW);
}

/* Leave pin off for time (given in microseconds) */
void IRsend::space(unsigned long time) {
  // Sends an IR space for the specified number of microseconds.
  // A space is no output, so the PWM output is disabled.
  ledOff();
  if (time == 0) return;
  if (time <= 16383)  // delayMicroseconds is only accurate to 16383us.
    delayMicroseconds(time);
  else {
    // Invoke a delay(), where possible, to avoid triggering the WDT.
    delay(time / 1000UL);  // Delay for as many whole ms as we can.
    delayMicroseconds((int) time % 1000UL);  // Delay the remaining sub-msecond.
  }
}

void IRsend::enableIROut(int khz) {
  // Enables IR output.
  // The khz value controls the modulation frequency in kilohertz.

  // T = 1/f but we need T/2 in microsecond and f is in kHz
  halfPeriodicTime = 500/khz;
}


/* Sharp and DISH support by Todd Treece
( http://unionbridge.org/design/ircommand )

The Dish send function needs to be repeated 4 times, and the Sharp function
has the necessary repeat built in because of the need to invert the signal.

Sharp protocol documentation:
http://www.sbprojects.com/knowledge/ir/sharp.htm

Here are the LIRC files that I found that seem to match the remote codes
from the oscilloscope:

Sharp LCD TV:
http://lirc.sourceforge.net/remotes/sharp/GA538WJSA

DISH NETWORK (echostar 301):
http://lirc.sourceforge.net/remotes/echostar/301_501_3100_5100_58xx_59xx

For the DISH codes, only send the last for characters of the hex.
i.e. use 0x1C10 instead of 0x0000000000001C10 which is listed in the
linked LIRC file.
*/

void IRsend::sendSharpRaw(unsigned long data, int nbits) {
  // Set IR carrier frequency
  enableIROut(38);
  // Sending codes in bursts of 3 (normal, inverted, normal) makes transmission
  // much more reliable. That's the exact behaviour of CD-S6470 remote control.
  for (int n = 0; n < 3; n++) {
    // Data
    for (unsigned long mask = 1UL << (nbits - 1);  mask;  mask >>= 1) {
      if (data & mask) {  // 1
        mark(SHARP_BIT_MARK);
        space(SHARP_ONE_SPACE);
      } else {  // 0
        mark(SHARP_BIT_MARK);
        space(SHARP_ZERO_SPACE);
      }
    }
    // Footer
    mark(SHARP_BIT_MARK);
    space(SHARP_ZERO_SPACE);
    delay(40);

    data = data ^ SHARP_TOGGLE_MASK;
  }
}

// Sharp send compatible with data obtained through decodeSharp
void IRsend::sendSharp(unsigned int address, unsigned int command) {
  sendSharpRaw((address << 10) | (command << 2) | 2, 15);
}

void IRsend::sendDISH(unsigned long data, int nbits) {
  // Set IR carrier frequency
  enableIROut(56);
  // Header
  mark(DISH_HDR_MARK);
  space(DISH_HDR_SPACE);
  for (int i = 0; i < nbits; i++) {
    if (data & DISH_TOP_BIT) {
      mark(DISH_BIT_MARK);
      space(DISH_ONE_SPACE);
    } else {
      mark(DISH_BIT_MARK);
      space(DISH_ZERO_SPACE);
    }
    data <<= 1;
  }
  // Footer
  ledOff();
}

// From https://github.com/mharizanov/Daikin-AC-remote-control-over-the-Internet/tree/master/IRremote
void IRsend::sendDaikin(unsigned char daikin[]) {
  sendDaikinChunk(daikin, 8,0);
  delay(29);
  sendDaikinChunk(daikin, 19,8);
}

void IRsend::sendDaikinChunk(unsigned char buf[], int len, int start) {
  int data2;
  // Set IR carrier frequency
  enableIROut(38);
  // Header
  mark(DAIKIN_HDR_MARK);
  space(DAIKIN_HDR_SPACE);
  // Data
  for (int i = start; i < start+len; i++) {
    data2=buf[i];

    for (int j = 0; j < 8; j++) {
      if ((1 << j & data2)) {
        mark(DAIKIN_ONE_MARK);
        space(DAIKIN_ONE_SPACE);
      } else {
        mark(DAIKIN_ZERO_MARK);
        space(DAIKIN_ZERO_SPACE);
      }
    }
  }
  // Footer
  mark(DAIKIN_ONE_MARK);
  space(DAIKIN_ZERO_SPACE);
}

void IRsend::sendKelvinator(unsigned char data[]) {
  uint8_t i = 0;
  // Set IR carrier frequency
  enableIROut(38);
  // Header #1
  mark(KELVINATOR_HDR_MARK);
  space(KELVINATOR_HDR_SPACE);
  // Data (command)
  // Send the first command data (4 bytes)
  for (; i < 4; i++)
    sendData(KELVINATOR_BIT_MARK, KELVINATOR_ONE_SPACE, KELVINATOR_BIT_MARK,
             KELVINATOR_ZERO_SPACE, data[i], 8, false);
  // Send Footer for the command data (3 bits (B010))
  sendData(KELVINATOR_BIT_MARK, KELVINATOR_ONE_SPACE, KELVINATOR_BIT_MARK,
           KELVINATOR_ZERO_SPACE, KELVINATOR_CMD_FOOTER, 3, false);
  // Send an interdata gap.
  mark(KELVINATOR_BIT_MARK);
  space(KELVINATOR_GAP_SPACE);
  // Data (options)
  // Send the 1st option chunk of data (4 bytes).
  for (; i < 8; i++)
    sendData(KELVINATOR_BIT_MARK, KELVINATOR_ONE_SPACE, KELVINATOR_BIT_MARK,
             KELVINATOR_ZERO_SPACE, data[i], 8, false);
  // Send a double data gap to signify we are starting a new command sequence.
  mark(KELVINATOR_BIT_MARK);
  space(KELVINATOR_GAP_SPACE * 2);
  // Header #2
  mark(KELVINATOR_HDR_MARK);
  space(KELVINATOR_HDR_SPACE);
  // Data (command)
  // Send the 2nd command data (4 bytes).
  // Basically an almost identical repeat of the earlier command data.
  for (; i < 12; i++)
    sendData(KELVINATOR_BIT_MARK, KELVINATOR_ONE_SPACE, KELVINATOR_BIT_MARK,
             KELVINATOR_ZERO_SPACE, data[i], 8, false);
  // Send Footer for the command data (3 bits (B010))
  sendData(KELVINATOR_BIT_MARK, KELVINATOR_ONE_SPACE, KELVINATOR_BIT_MARK,
           KELVINATOR_ZERO_SPACE, KELVINATOR_CMD_FOOTER, 3, false);
  // Send an interdata gap.
  mark(KELVINATOR_BIT_MARK);
  space(KELVINATOR_GAP_SPACE);
  // Data (options)
  // Send the 2nd option chunk of data (4 bytes).
  // Unlike the commands, definately not a repeat of the earlier option data.
  for (; i < KELVINATOR_STATE_LENGTH; i++)
    sendData(KELVINATOR_BIT_MARK, KELVINATOR_ONE_SPACE, KELVINATOR_BIT_MARK,
             KELVINATOR_ZERO_SPACE, data[i], 8, false);
  // Footer
  mark(KELVINATOR_BIT_MARK);
  ledOff();
}

void IRsend::sendSherwood(unsigned long data, int nbits, unsigned int repeat) {
  // Sherwood remote codes appear to be NEC codes with a manditory repeat code.
  // i.e. repeat should be >= 1.
  sendNEC(data, nbits, max(1, repeat));
}

void IRsend::sendMitsubishiACChunk(uint8_t data) {
  // send a chunk(byte) of Mitsubishi AC data
  for (uint8_t bit = 0; bit < 8; bit++, data >>= 1) {
    if (data & B1) {  // 1
      mark(MITSUBISHI_AC_BIT_MARK);
      space(MITSUBISHI_AC_ONE_SPACE);
    } else {  // 0
      mark(MITSUBISHI_AC_BIT_MARK);
      space(MITSUBISHI_AC_ZERO_SPACE);
    }
  }
}

void IRsend::sendMitsubishiAC(unsigned char data[]) {
  // Set IR carrier frequency
  enableIROut(38);
  // Mitsubishi AC remote sends the packet twice.
  for (uint8_t count = 0; count < 2; count++) {
    // Header
    mark(MITSUBISHI_AC_HDR_MARK);
    space(MITSUBISHI_AC_HDR_SPACE);
    // Data
    for (uint8_t i = 0; i < MITSUBISHI_AC_STATE_LENGTH; i++)
      sendMitsubishiACChunk(data[i]);
    // Footer
    mark(MITSUBISHI_AC_RPT_MARK);
    space(MITSUBISHI_AC_RPT_SPACE);
  }
}
// ---------------------------------------------------------------


//IRRecv------------------------------------------------------

extern "C" {
	#include "user_interface.h"
	#include "gpio.h"
}

static ETSTimer timer;
volatile irparams_t irparams;

static void ICACHE_RAM_ATTR read_timeout(void *arg __attribute__((unused))) {
  os_intr_lock();
  if (irparams.rawlen) {
    irparams.rcvstate = STATE_STOP;
  }
	os_intr_unlock();
}

static void ICACHE_RAM_ATTR gpio_intr() {
  uint32_t now = system_get_time();
  uint32_t gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
  static uint32_t start = 0;

  os_timer_disarm(&timer);
  GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status);

  if (irparams.rawlen >= RAWBUF) {
    irparams.overflow = true;
    irparams.rcvstate = STATE_STOP;
  }

  if (irparams.rcvstate == STATE_STOP) {
    return;
  }

  if (irparams.rcvstate == STATE_IDLE) {
    irparams.overflow = false;
    irparams.rcvstate = STATE_MARK;
    irparams.rawbuf[irparams.rawlen++] = 1;
  } else {
    if (now < start)
      irparams.rawbuf[irparams.rawlen++] = (0xFFFFFFFF - start + now) / USECPERTICK + 1;
    else
      irparams.rawbuf[irparams.rawlen++] = (now - start) / USECPERTICK + 1;
  }

  start = now;
  #define ONCE 0
  os_timer_arm(&timer, 15, ONCE);
}

IRrecv::IRrecv(int recvpin) {
  irparams.recvpin = recvpin;
}

// initialization
void IRrecv::enableIRIn() {
  // initialize state machine variables
  irparams.rcvstate = STATE_IDLE;
  irparams.rawlen = 0;

  // Initialize timer
  os_timer_disarm(&timer);
  os_timer_setfn(&timer, (os_timer_func_t *)read_timeout, NULL);

  // Attach Interrupt
  attachInterrupt(irparams.recvpin, gpio_intr, CHANGE);
}

void IRrecv::disableIRIn() {
  os_timer_disarm(&timer);
  detachInterrupt(irparams.recvpin);
}

void IRrecv::resume() {
  irparams.rcvstate = STATE_IDLE;
  irparams.rawlen = 0;
}

// Decodes the received IR message
// Returns true if is data ready
// Results of decoding are stored in results
bool IRrecv::decode(decode_results *results) {
  results->rawbuf = irparams.rawbuf;
  results->rawlen = irparams.rawlen;
  results->overflow = irparams.overflow;

  if (irparams.rcvstate != STATE_STOP) {
    return false;
  }

#ifdef DEBUG
  Serial.println("Attempting NEC decode");
#endif
  if (decodeNEC(results)) {
    return true;
  }

#ifdef DEBUG
  Serial.println("Attempting Sony decode");
#endif
  if (decodeSony(results)) {
    return true;
  }
  /*
#ifdef DEBUG
  Serial.println("Attempting Sanyo decode");
#endif
  if (decodeSanyo(results)) {
    return true;
  }*/
#ifdef DEBUG
  Serial.println("Attempting Mitsubishi decode");
#endif
  if (decodeMitsubishi(results)) {
    return true;
  }
#ifdef DEBUG
  Serial.println("Attempting RC5 decode");
#endif
  if (decodeRC5(results)) {
    return true;
  }
#ifdef DEBUG
  Serial.println("Attempting RC6 decode");
#endif
  if (decodeRC6(results)) {
    return true;
  }
#ifdef DEBUG
  Serial.println("Attempting Panasonic decode");
#endif
  if (decodePanasonic(results)) {
    return true;
  }
#ifdef DEBUG
  Serial.println("Attempting LG decode");
#endif
  if (decodeLG(results)) {
      return true;
  }
#ifdef DEBUG
  Serial.println("Attempting JVC decode");
#endif
  if (decodeJVC(results)) {
    return true;
  }
#ifdef DEBUG
  Serial.println("Attempting SAMSUNG decode");
#endif
  if (decodeSAMSUNG(results)) {
    return true;
  }
#ifdef DEBUG
  Serial.println("Attempting Whynter decode");
#endif
  if (decodeWhynter(results)) {
    return true;
  }
  // decodeHash returns a hash on any input.
  // Thus, it needs to be last in the list.
  // If you add any decodes, add them before this.
  if (decodeHash(results)) {
    return true;
  }
  // Throw away and start over
  resume();
  return false;
}

// NECs have a repeat only 4 items long
bool IRrecv::decodeNEC(decode_results *results) {
  long data = 0;
  int offset = 1; // Skip initial space
  // Initial mark
  if (!MATCH_MARK(results->rawbuf[offset], NEC_HDR_MARK)) {
    return false;
  }
  offset++;
  // Check for repeat
  if (irparams.rawlen == 4 &&
    MATCH_SPACE(results->rawbuf[offset], NEC_RPT_SPACE) &&
    MATCH_MARK(results->rawbuf[offset+1], NEC_BIT_MARK)) {
    results->bits = 0;
    results->value = REPEAT;
    results->decode_type = NEC;
    return true;
  }
  if (irparams.rawlen < 2 * NEC_BITS + 4) {
    return false;
  }
  // Initial space
  if (!MATCH_SPACE(results->rawbuf[offset], NEC_HDR_SPACE)) {
    return false;
  }
  offset++;
  for (int i = 0; i < NEC_BITS; i++) {
    if (!MATCH_MARK(results->rawbuf[offset], NEC_BIT_MARK)) {
      return false;
    }
    offset++;
    if (MATCH_SPACE(results->rawbuf[offset], NEC_ONE_SPACE)) {
      data = (data << 1) | 1;
    } else if (MATCH_SPACE(results->rawbuf[offset], NEC_ZERO_SPACE)) {
      data <<= 1;
    } else {
      return false;
    }
    offset++;
  }
  // Success
  results->bits = NEC_BITS;
  results->value = data;
  results->decode_type = NEC;
  return true;
}

bool IRrecv::decodeSony(decode_results *results) {
  long data = 0;
  if (irparams.rawlen < 2 * SONY_BITS + 2) {
    return false;
  }
  int offset = 0; // Dont skip first space, check its size

  /*
  // Some Sony's deliver repeats fast after first
  // unfortunately can't spot difference from of repeat from two fast clicks
  if (results->rawbuf[offset] < SONY_DOUBLE_SPACE_USECS) {
    // Serial.print("IR Gap found: ");
    results->bits = 0;
    results->value = REPEAT;
    results->decode_type = SANYO;
    return true;
  }*/
  offset++;

  // Initial mark
  if (!MATCH_MARK(results->rawbuf[offset], SONY_HDR_MARK)) {
    return false;
  }
  offset++;

  while (offset + 1 < irparams.rawlen) {
    if (!MATCH_SPACE(results->rawbuf[offset], SONY_HDR_SPACE)) {
      break;
    }
    offset++;
    if (MATCH_MARK(results->rawbuf[offset], SONY_ONE_MARK)) {
      data = (data << 1) | 1;
    } else if (MATCH_MARK(results->rawbuf[offset], SONY_ZERO_MARK)) {
      data <<= 1;
    } else {
      return false;
    }
    offset++;
  }

  // Success
  results->bits = (offset - 1) / 2;
  if (results->bits < 12) {
    results->bits = 0;
    return false;
  }
  results->value = data;
  results->decode_type = SONY;
  return true;
}

bool IRrecv::decodeWhynter(decode_results *results) {
  long data = 0;

  if (irparams.rawlen < 2 * WHYNTER_BITS + 6) {
     return false;
  }

  int offset = 1; // Skip first space


  // sequence begins with a bit mark and a zero space
  if (!MATCH_MARK(results->rawbuf[offset], WHYNTER_BIT_MARK)) {
    return false;
  }
  offset++;
  if (!MATCH_SPACE(results->rawbuf[offset], WHYNTER_ZERO_SPACE)) {
    return false;
  }
  offset++;

  // header mark and space
  if (!MATCH_MARK(results->rawbuf[offset], WHYNTER_HDR_MARK)) {
    return false;
  }
  offset++;
  if (!MATCH_SPACE(results->rawbuf[offset], WHYNTER_HDR_SPACE)) {
    return false;
  }
  offset++;

  // data bits
  for (int i = 0; i < WHYNTER_BITS; i++) {
    if (!MATCH_MARK(results->rawbuf[offset], WHYNTER_BIT_MARK)) {
      return false;
    }
    offset++;
    if (MATCH_SPACE(results->rawbuf[offset], WHYNTER_ONE_SPACE)) {
      data = (data << 1) | 1;
    } else if (MATCH_SPACE(results->rawbuf[offset],WHYNTER_ZERO_SPACE)) {
      data <<= 1;
    } else {
      return false;
    }
    offset++;
  }

  // trailing mark
  if (!MATCH_MARK(results->rawbuf[offset], WHYNTER_BIT_MARK)) {
    return false;
  }
  // Success
  results->bits = WHYNTER_BITS;
  results->value = data;
  results->decode_type = WHYNTER;
  return true;
}

// I think this is a Sanyo decoder - serial = SA 8650B
// Looks like Sony except for timings, 48 chars of data and time/space different
bool IRrecv::decodeSanyo(decode_results *results) {
  long data = 0;
  if (irparams.rawlen < 2 * SANYO_BITS + 2) {
    return false;
  }
  int offset = 1; // Skip first space


  // Initial space
  /* Put this back in for debugging - note can't use #DEBUG as if Debug on we don't see the repeat cos of the delay
  Serial.print("IR Gap: ");
  Serial.println( results->rawbuf[offset]);
  Serial.println( "test against:");
  Serial.println(results->rawbuf[offset]);
  */

  if (results->rawbuf[offset] < SANYO_DOUBLE_SPACE_USECS) {
    // Serial.print("IR Gap found: ");
    results->bits = 0;
    results->value = REPEAT;
    results->decode_type = SANYO;
    return true;
  }
  offset++;

  // Initial mark
  if (!MATCH_MARK(results->rawbuf[offset], SANYO_HDR_MARK)) {
    return false;
  }
  offset++;

  // Skip Second Mark
  if (!MATCH_MARK(results->rawbuf[offset], SANYO_HDR_MARK)) {
    return false;
  }
  offset++;

  while (offset + 1 < irparams.rawlen) {
    if (!MATCH_SPACE(results->rawbuf[offset], SANYO_HDR_SPACE)) {
      break;
    }
    offset++;
    if (MATCH_MARK(results->rawbuf[offset], SANYO_ONE_MARK)) {
      data = (data << 1) | 1;
    } else if (MATCH_MARK(results->rawbuf[offset], SANYO_ZERO_MARK)) {
      data <<= 1;
    } else {
      return false;
    }
    offset++;
  }

  // Success
  results->bits = (offset - 1) / 2;
  if (results->bits < 12) {
    results->bits = 0;
    return false;
  }
  results->value = data;
  results->decode_type = SANYO;
  return true;
}

// Looks like Sony except for timings, 48 chars of data and time/space different
bool IRrecv::decodeMitsubishi(decode_results *results) {
  // Serial.print("?!? decoding Mitsubishi:");Serial.print(irparams.rawlen);
  // Serial.print(" want "); Serial.println( 2 * MITSUBISHI_BITS + 2);
  long data = 0;
  if (irparams.rawlen < 2 * MITSUBISHI_BITS + 2) {
    return false;
  }
  int offset = 1; // Skip first space
  // Initial space
  /* Put this back in for debugging - note can't use #DEBUG as if Debug on we
     don't see the repeat cos of the delay
  Serial.print("IR Gap: ");
  Serial.println( results->rawbuf[offset]);
  Serial.println( "test against:");
  Serial.println(results->rawbuf[offset]);
  */
  /* Not seeing double keys from Mitsubishi
  if (results->rawbuf[offset] < MITSUBISHI_DOUBLE_SPACE_USECS) {
    // Serial.print("IR Gap found: ");
    results->bits = 0;
    results->value = REPEAT;
    results->decode_type = MITSUBISHI;
    return true;
  }
  */

  offset++;

  // Typical
  // 14200 7 41 7 42 7 42 7 17 7 17 7 18 7 41 7 18 7 17 7 17 7 18 7 41 8 17 7 17 7 18 7 17 7

  // Initial Space
  if (!MATCH_MARK(results->rawbuf[offset], MITSUBISHI_HDR_SPACE)) {
    return false;
  }
  offset++;
  while (offset + 1 < irparams.rawlen) {
    if (MATCH_MARK(results->rawbuf[offset], MITSUBISHI_ONE_MARK)) {
      data = (data << 1) | 1;
    } else if (MATCH_MARK(results->rawbuf[offset], MITSUBISHI_ZERO_MARK)) {
      data <<= 1;
    } else {
      // Serial.println("A"); Serial.println(offset); Serial.println(results->rawbuf[offset]);
      return false;
    }
    offset++;
    if (!MATCH_SPACE(results->rawbuf[offset], MITSUBISHI_HDR_SPACE)) {
      // Serial.println("B"); Serial.println(offset); Serial.println(results->rawbuf[offset]);
      break;
    }
    offset++;
  }

  // Success
  results->bits = (offset - 1) / 2;
  if (results->bits < MITSUBISHI_BITS) {
    results->bits = 0;
    return false;
  }
  results->value = data;
  results->decode_type = MITSUBISHI;
  return true;
}

// Gets one undecoded level at a time from the raw buffer.
// The RC5/6 decoding is easier if the data is broken into time intervals.
// E.g. if the buffer has MARK for 2 time intervals and SPACE for 1,
// successive calls to getRClevel will return MARK, MARK, SPACE.
// offset and used are updated to keep track of the current position.
// t1 is the time interval for a single bit in microseconds.
// Returns -1 for error (measured time interval is not a multiple of t1).
int IRrecv::getRClevel(decode_results *results, int *offset, int *used,
                       int t1) {
  if (*offset >= results->rawlen) {
    // After end of recorded buffer, assume SPACE.
    return SPACE;
  }
  int width = results->rawbuf[*offset];
  int val = ((*offset) % 2) ? MARK : SPACE;
  int correction = (val == MARK) ? MARK_EXCESS : - MARK_EXCESS;

  int avail;
  if (MATCH(width, t1 + correction)) {
    avail = 1;
  } else if (MATCH(width, 2*t1 + correction)) {
    avail = 2;
  } else if (MATCH(width, 3*t1 + correction)) {
    avail = 3;
  } else {
    return -1;
  }

  (*used)++;
  if (*used >= avail) {
    *used = 0;
    (*offset)++;
  }
#ifdef DEBUG
  if (val == MARK) {
    Serial.println("MARK");
  } else {
    Serial.println("SPACE");
  }
#endif
  return val;
}

bool IRrecv::decodeRC5(decode_results *results) {
  if (irparams.rawlen < MIN_RC5_SAMPLES + 2) {
    return false;
  }
  int offset = 1; // Skip gap space
  long data = 0;
  int used = 0;
  // Get start bits
  if (getRClevel(results, &offset, &used, RC5_T1) != MARK) return false;
  if (getRClevel(results, &offset, &used, RC5_T1) != SPACE) return false;
  if (getRClevel(results, &offset, &used, RC5_T1) != MARK) return false;
  int nbits;
  for (nbits = 0; offset < irparams.rawlen; nbits++) {
    int levelA = getRClevel(results, &offset, &used, RC5_T1);
    int levelB = getRClevel(results, &offset, &used, RC5_T1);
    if (levelA == SPACE && levelB == MARK) {
      // 1 bit
      data = (data << 1) | 1;
    } else if (levelA == MARK && levelB == SPACE) {
      // zero bit
      data <<= 1;
    } else {
      return false;
    }
  }

  // Success
  results->bits = nbits;
  results->value = data;
  results->decode_type = RC5;
  return true;
}

bool IRrecv::decodeRC6(decode_results *results) {
  if (results->rawlen < MIN_RC6_SAMPLES) {
    return false;
  }
  int offset = 1; // Skip first space
  // Initial mark
  if (!MATCH_MARK(results->rawbuf[offset], RC6_HDR_MARK)) {
    return false;
  }
  offset++;
  if (!MATCH_SPACE(results->rawbuf[offset], RC6_HDR_SPACE)) {
    return false;
  }
  offset++;
  long data = 0;
  int used = 0;
  // Get start bit (1)
  if (getRClevel(results, &offset, &used, RC6_T1) != MARK) return false;
  if (getRClevel(results, &offset, &used, RC6_T1) != SPACE) return false;
  int nbits;
  for (nbits = 0; offset < results->rawlen; nbits++) {
    int levelA, levelB; // Next two levels
    levelA = getRClevel(results, &offset, &used, RC6_T1);
    if (nbits == 3) {
      // T bit is double wide; make sure second half matches
      if (levelA != getRClevel(results, &offset, &used, RC6_T1)) return false;
    }
    levelB = getRClevel(results, &offset, &used, RC6_T1);
    if (nbits == 3) {
      // T bit is double wide; make sure second half matches
      if (levelB != getRClevel(results, &offset, &used, RC6_T1)) return false;
    }
    if (levelA == MARK && levelB == SPACE) { // reversed compared to RC5
      // 1 bit
      data = (data << 1) | 1;
    } else if (levelA == SPACE && levelB == MARK) {
      // zero bit
      data <<= 1;
    } else {
      return false; // Error
    }
  }
  // Success
  results->bits = nbits;
  results->value = data;
  results->decode_type = RC6;
  return true;
}

bool IRrecv::decodePanasonic(decode_results *results) {
  unsigned long long data = 0;
	int offset = 1;  // Dont skip first space
  if (!MATCH_MARK(results->rawbuf[offset], PANASONIC_HDR_MARK)) {
    return false;
  }
  offset++;
  if (!MATCH_MARK(results->rawbuf[offset], PANASONIC_HDR_SPACE)) {
    return false;
  }
  offset++;
  // decode address
  for (int i = 0; i < PANASONIC_BITS; i++) {
    if (!MATCH(results->rawbuf[offset++], PANASONIC_BIT_MARK)) {
      return false;
    }
    if (MATCH(results->rawbuf[offset],PANASONIC_ONE_SPACE)) {
      data = (data << 1) | 1;
    } else if (MATCH(results->rawbuf[offset],PANASONIC_ZERO_SPACE)) {
      data <<= 1;
    } else {
      return false;
    }
    offset++;
  }
  results->value = (unsigned long)data;
  results->panasonicAddress = (unsigned int)(data >> 32);
  results->decode_type = PANASONIC;
  results->bits = PANASONIC_BITS;
  return true;
}

bool IRrecv::decodeLG(decode_results *results) {
  long data = 0;
	int offset = 1; // Skip first space

  // Initial mark
  if (!MATCH_MARK(results->rawbuf[offset], LG_HDR_MARK)) {
    return false;
  }
  offset++;
  if (irparams.rawlen < 2 * LG_BITS + 1 ) {
    return false;
  }
  // Initial space
  if (!MATCH_SPACE(results->rawbuf[offset], LG_HDR_SPACE)) {
    return false;
  }
  offset++;
  for (int i = 0; i < LG_BITS; i++) {
    if (!MATCH_MARK(results->rawbuf[offset], LG_BIT_MARK)) {
      return false;
    }
    offset++;
    if (MATCH_SPACE(results->rawbuf[offset], LG_ONE_SPACE)) {
      data = (data << 1) | 1;
    } else if (MATCH_SPACE(results->rawbuf[offset], LG_ZERO_SPACE)) {
      data <<= 1;
    } else {
      return false;
    }
    offset++;
  }
  //Stop bit
  if (!MATCH_MARK(results->rawbuf[offset], LG_BIT_MARK)){
    return false;
  }
  // Success
  results->bits = LG_BITS;
  results->value = data;
  results->decode_type = LG;
  return true;
}

bool IRrecv::decodeJVC(decode_results *results) {
  long data = 0;
	int offset = 1; // Skip first space
  // Check for repeat
  if (irparams.rawlen - 1 == 33 &&
      MATCH_MARK(results->rawbuf[offset], JVC_BIT_MARK) &&
      MATCH_MARK(results->rawbuf[irparams.rawlen-1], JVC_BIT_MARK)) {
    results->bits = 0;
    results->value = REPEAT;
    results->decode_type = JVC;
    return true;
  }
  // Initial mark
  if (!MATCH_MARK(results->rawbuf[offset], JVC_HDR_MARK)) {
    return false;
  }
  offset++;
  if (irparams.rawlen < 2 * JVC_BITS + 1 ) {
    return false;
  }
  // Initial space
  if (!MATCH_SPACE(results->rawbuf[offset], JVC_HDR_SPACE)) {
    return false;
  }
  offset++;
  for (int i = 0; i < JVC_BITS; i++) {
    if (!MATCH_MARK(results->rawbuf[offset], JVC_BIT_MARK)) {
      return false;
    }
    offset++;
    if (MATCH_SPACE(results->rawbuf[offset], JVC_ONE_SPACE)) {
      data = (data << 1) | 1;
    } else if (MATCH_SPACE(results->rawbuf[offset], JVC_ZERO_SPACE)) {
      data <<= 1;
    } else {
      return false;
    }
    offset++;
  }
  //Stop bit
  if (!MATCH_MARK(results->rawbuf[offset], JVC_BIT_MARK)) {
    return false;
  }
  // Success
  results->bits = JVC_BITS;
  results->value = data;
  results->decode_type = JVC;
  return true;
}

// SAMSUNGs have a repeat only 4 items long
bool IRrecv::decodeSAMSUNG(decode_results *results) {
  long data = 0;
  int offset = 1;  // Dont skip first space
  // Initial mark
  if (!MATCH_MARK(results->rawbuf[offset], SAMSUNG_HDR_MARK)) {
    return false;
  }
  offset++;
  // Check for repeat
  if (irparams.rawlen == 4 &&
      MATCH_SPACE(results->rawbuf[offset], SAMSUNG_RPT_SPACE) &&
      MATCH_MARK(results->rawbuf[offset+1], SAMSUNG_BIT_MARK)) {
    results->bits = 0;
    results->value = REPEAT;
    results->decode_type = SAMSUNG;
    return true;
  }
  if (irparams.rawlen < 2 * SAMSUNG_BITS + 2) {
    return false;
  }
  // Initial space
  if (!MATCH_SPACE(results->rawbuf[offset], SAMSUNG_HDR_SPACE)) {
    return false;
  }
  offset++;
  for (int i = 0; i < SAMSUNG_BITS; i++) {
    if (!MATCH_MARK(results->rawbuf[offset], SAMSUNG_BIT_MARK)) {
      return false;
    }
    offset++;
    if (MATCH_SPACE(results->rawbuf[offset], SAMSUNG_ONE_SPACE)) {
      data = (data << 1) | 1;
    } else if (MATCH_SPACE(results->rawbuf[offset], SAMSUNG_ZERO_SPACE)) {
      data <<= 1;
    } else {
      return false;
    }
    offset++;
  }
  // Success
  results->bits = SAMSUNG_BITS;
  results->value = data;
  results->decode_type = SAMSUNG;
  return true;
}

// From https://github.com/mharizanov/Daikin-AC-remote-control-over-the-Internet/tree/master/IRremote
// decoding not actually tested
bool IRrecv::decodeDaikin(decode_results *results) {
  long data = 0;
  int offset = 1; // Skip first space

  if (irparams.rawlen < 2 * DAIKIN_BITS + 4) {
    //return false;
  }

  // Initial mark
  if (!MATCH_MARK(results->rawbuf[offset], DAIKIN_HDR_MARK)) {
      return false;
  }
  offset++;

  if (!MATCH_SPACE(results->rawbuf[offset], DAIKIN_HDR_SPACE)) {
      return false;
  }
  offset++;

  for (int i = 0; i < 32; i++) {
    if (!MATCH_MARK(results->rawbuf[offset], DAIKIN_ONE_MARK)) {
      return false;
    }
    offset++;
    if (MATCH_SPACE(results->rawbuf[offset], DAIKIN_ONE_SPACE)) {
      data = (data << 1) | 1;
    } else if (MATCH_SPACE(results->rawbuf[offset], DAIKIN_ZERO_SPACE)) {
      data <<= 1;
    } else {
      return false;
    }
    offset++;
  }

  unsigned long number = data ; // some number...
  int bits = 32 ; // nr of bits in some number
  unsigned long reversed = 0;
  for ( int b=0 ; b < bits ; b++ ) {
    reversed = ( reversed << 1 ) | ( 0x0001 & ( number >> b ) );
  }

  Serial.print ("Code ");
  Serial.println (reversed,  HEX);

  //==========

  for (int i = 0; i < 32; i++) {
    if (!MATCH_MARK(results->rawbuf[offset], DAIKIN_ONE_MARK)) {
      return false;
    }
    offset++;
    if (MATCH_SPACE(results->rawbuf[offset], DAIKIN_ONE_SPACE)) {
      data = (data << 1) | 1;
    } else if (MATCH_SPACE(results->rawbuf[offset], DAIKIN_ZERO_SPACE)) {
      data <<= 1;
    } else {
      return false;
    }
    offset++;
  }

  number = data ; // some number...
  bits = 32 ; // nr of bits in some number
  reversed = 0;
  for ( int b=0 ; b < bits ; b++ ) {
    reversed = ( reversed << 1 ) | ( 0x0001 & ( number >> b ) );
  }

  //Serial.print ("Code2 ");
  //Serial.println (reversed,  HEX);

  //===========
  if (!MATCH_SPACE(results->rawbuf[offset], 29000)) {
    //Serial.println ("no gap");
	  return false;
  }
  offset++;

  // Success
  results->bits = DAIKIN_BITS;
  results->value = reversed;
  results->decode_type = DAIKIN;
  return true;
}

// Denon, from https://github.com/z3t0/Arduino-IRremote/blob/master/ir_Denon.cpp
bool IRrecv::decodeDenon (decode_results *results) {
	unsigned long data   = 0;  // Somewhere to build our code
	int offset = 1;  // Skip the Gap reading

	// Check we have the right amount of data
	if (irparams.rawlen != 1 + 2 + (2 * DENON_BITS) + 1) {
	  return false;
	}

	// Check initial Mark+Space match
	if (!MATCH_MARK (results->rawbuf[offset++], DENON_HDR_MARK )) {
	  return false;
	}
	if (!MATCH_SPACE(results->rawbuf[offset++], DENON_HDR_SPACE)) {
	  return false;
	}

	// Read the bits in
	for (int i = 0;  i < DENON_BITS;  i++) {
		// Each bit looks like: DENON_MARK + DENON_SPACE_1 -> 1
		//                 or : DENON_MARK + DENON_SPACE_0 -> 0
		if (!MATCH_MARK(results->rawbuf[offset++], DENON_BIT_MARK)) {
		  return false;
		}

		// IR data is big-endian, so we shuffle it in from the right:
		if (MATCH_SPACE(results->rawbuf[offset], DENON_ONE_SPACE)) {
		  data = (data << 1) | 1;
		} else if (MATCH_SPACE(results->rawbuf[offset], DENON_ZERO_SPACE)) {
		  data = (data << 1) | 0;
		} else {
		  return false;
		}
		offset++;
	}

	// Success
	results->bits = DENON_BITS;
	results->value = data;
	results->decode_type = DENON;
	return true;
}


/* -----------------------------------------------------------------------
 * hashdecode - decode an arbitrary IR code.
 * Instead of decoding using a standard encoding scheme
 * (e.g. Sony, NEC, RC5), the code is hashed to a 32-bit value.
 *
 * The algorithm: look at the sequence of MARK signals, and see if each one
 * is shorter (0), the same length (1), or longer (2) than the previous.
 * Do the same with the SPACE signals.  Hszh the resulting sequence of 0's,
 * 1's, and 2's to a 32-bit value.  This will give a unique value for each
 * different code (probably), for most code systems.
 *
 * http://arcfn.com/2010/01/using-arbitrary-remotes-with-arduino.html
 */

// Compare two tick values, returning 0 if newval is shorter,
// 1 if newval is equal, and 2 if newval is longer
// Use a tolerance of 20%
int IRrecv::compare(unsigned int oldval, unsigned int newval) {
  if (newval < oldval * .8) {
    return 0;
  } else if (oldval < newval * .8) {
    return 2;
  } else {
    return 1;
  }
}

// Use FNV hash algorithm: http://isthe.com/chongo/tech/comp/fnv/#FNV-param
#define FNV_PRIME_32 16777619
#define FNV_BASIS_32 2166136261

/* Converts the raw code values into a 32-bit hash code.
 * Hopefully this code is unique for each button.
 * This isn't a "real" decoding, just an arbitrary value.
 */
bool IRrecv::decodeHash(decode_results *results) {
  // Require at least 6 samples to prevent triggering on noise
  if (results->rawlen < 6) {
    return false;
  }
  long hash = FNV_BASIS_32;
  for (int i = 1; i+2 < results->rawlen; i++) {
    int value =  compare(results->rawbuf[i], results->rawbuf[i+2]);
    // Add value into the hash
    hash = (hash * FNV_PRIME_32) ^ value;
  }
  results->value = hash;
  results->bits = 32;
  results->decode_type = UNKNOWN;
  return true;
}

// ---------------------------------------------------------------
