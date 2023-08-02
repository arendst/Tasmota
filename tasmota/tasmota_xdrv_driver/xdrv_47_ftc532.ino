/*
  xdrv_47_ftc532.ino - FTC532 touch buttons support for Tasmota

  Copyright (C) 2021  Peter Franck and Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef USE_FTC532
/*********************************************************************************************\
  THE PLAN [tm]:                    OUTCOME:
  ==============                    ========
  appear in a dropdown              (D_SENSOR_FTC532 "FTC532")
  select pin                        (GPIO_FTC532)
  attach interrupt to pin           DONE
  ISR updating all 8 inputs         DONE
  de-bouncing for 50 ms             DONE
  change report every 250 ms        REPORTS EVERY 50MS
  Webserver display "00001001"      DONE & REMOVED
  MQTT message                      DONE
  Rules hook                        DONE
  detach interrupt before restart   POINTLESS

  THE PROTOCOL [tm]:
  ==================
  LEAD-IN = 3015 µs HIGH, 755 µs LOW
  S = 377 µs HIGH,  377 µs LOW
  L = 377 µs HIGH, 1130 µs LOW

           GROUP1   GROUP2
  ---------------------------
  ALL OFF: SSSSLLLL SSSSLLLL
   1 ON  : LSSSSLLL SSSSLLLL
   2 ON  : SLSSLSLL SSSSLLLL
   3 ON  : SSLSLLSL SSSSLLLL
   8 ON  : SSSSLLLL SSSLLLLS
  123 ON : LLLSSSSL SSSSLLLL

  Timing of an ALL OFF frame in clock cycles T=377µs, triggering on rising edge:
  IDLE-2222444422224444-IDLE

  *********************
  * About this driver *
  *********************

  This driver implements the reverse engineered protocol of the FTC532 touch controller.
  The protocol encodes the bitmap of touched keys in variable length pulses comprising a
  fixed length frame. These frames are then sent out continuously from the FTC532 chip.
  The first version of this driver was working fine on well behaved hardware. After being
  released to the field and installed on crappy hardware in noisy environments it developed
  a habit of random 'ghost' switchings at night, much to the chagrin of some users.
  This is almost a re-write containing more timing (and other) checks in order to detect
  and mitigate various incarnations of noise.
  If you should still experience 'ghost switching' issues a solution may be increasing
  FTC532_DEBOUNCE to 2 or higher. That will enable the de-bouncing code, at the expense of
  'snappiness' of touch reactions. Higher values will accumulate more samples in 50 ms steps
  before actually firing the rules trigger. It will eat a few bytes off your RAM and Flash
  budget, too.

  Usage:
  ------
  This driver does not actually switch anything. It is a pure "rules" driver that solely emits
  {"FTC532":{"KEYS":"XX"}} JSON messages to be used in a rule or by an MQTT broker. "XX" stands
  for the hexadecimal (big endian) representation of a bitmap of keys currently touched, where
  e.g. "00" means "no key touched" while "03" means "keys 1 and 2 touched simultaneously".
  Selecting "FTC532" on a GPIO will awake the driver. This driver can only be selected once.


\*********************************************************************************************/

#define XDRV_47                   47

#define FTC532_DEBOUNCE           0     // no. of cycles, < 2 disables the code
#define FTC532_KEYS               4     // number of key pins on chip
#define FTC532_KEYS_MAX           8     // number of key slots in protocol

#define FTC532_STATE_WAITING      0x1
#define FTC532_STATE_READING      0x2
#define FTC532_STATE_COMPLETE     0x4

// Rising edge timing in microseconds
#define FTC532_BIT                377
#define FTC532_NOISE              (FTC532_BIT * 3 / 2)
#define FTC532_SHORT              (FTC532_BIT * 2)
#define FTC532_LONG               (FTC532_BIT * 4)
#define FTC532_IDLE               (FTC532_BIT * 10)
#define FTC532_MAX                (FTC532_BIT * 58)

struct FTC532 {
  volatile uint32_t rxtime;             // ISR timer memory
  volatile uint16_t tsmp    = 0;        // buffer for bit-coded time samples
  volatile uint16_t sample  = 0xF0F0;   // valid samples
  volatile uint16_t rxbit;              // ISR bit counter
  volatile uint16_t state;              // ISR state
  uint8_t keys              = 0;        // bitmap of active keys
  uint8_t old_keys          = 0;        // previously active keys
  bool present              = false;    // driver active
#if FTC532_DEBOUNCE > 1
  uint8_t key_cnt           = 0;        // used to de-bounce
#endif  // FTC532_DEBOUNCE > 1
#ifdef DEBUG_FTC532
  volatile uint16_t e_inv   = 0;        // inverted key error counter
  volatile uint16_t e_frame = 0;        // frame error counter
  volatile uint16_t e_noise = 0;        // noise detection counter
  volatile bool valid       = 0;        // did we ever receive valid data?
#endif  // DEBUG_FTC532
} Ftc532;

const char ftc532_json[] PROGMEM = "\"FTC532\":{\"KEYS\":\"";

void IRAM_ATTR ftc532_ISR(void) {   // Hardware interrupt routine, triggers on rising edge
  uint32_t time = micros();
  uint32_t time_diff = time - Ftc532.rxtime;
  Ftc532.rxtime = time;

  if (Ftc532.state & (FTC532_STATE_WAITING | FTC532_STATE_COMPLETE)) {
    if (time_diff > FTC532_LONG + FTC532_SHORT) {   // new frame
      Ftc532.rxbit = 0;
      if (Ftc532.state & FTC532_STATE_COMPLETE) {
      Ftc532.sample = Ftc532.tsmp;                  // copy completed frame
#ifdef DEBUG_FTC532
      Ftc532.valid = true;
#endif  // DEBUG_FTC532
      }
      Ftc532.state = FTC532_STATE_READING;
      Ftc532.tsmp = 0;
    } else {
      Ftc532.state = FTC532_STATE_WAITING;
    }
    return;
  }
  // FTC532_STATE_READING starts here
  if (time_diff > FTC532_LONG + FTC532_BIT) {
#ifdef DEBUG_FTC532
    ++Ftc532.e_frame;                               // frame error
#endif  // DEBUG_FTC532
    Ftc532.state = FTC532_STATE_WAITING;
    return;
  }
  if (time_diff > FTC532_SHORT + FTC532_BIT) {
    Ftc532.tsmp |= (1 << Ftc532.rxbit);             // LONG
  } else if (time_diff < FTC532_NOISE) {            // NOISE (SHORT now implicitly default)
#ifdef DEBUG_FTC532
    ++Ftc532.e_noise;
#endif  // DEBUG_FTC532
    Ftc532.state = FTC532_STATE_WAITING;
    return;
  }
  ++Ftc532.rxbit;
  if (Ftc532.rxbit == FTC532_KEYS_MAX * 2) {        // frame complete
    Ftc532.state = FTC532_STATE_COMPLETE;
  }
}

void ftc532_init(void) {                            // Initialize
  if (!PinUsed(GPIO_FTC532)) { return; }
  Ftc532.state = FTC532_STATE_WAITING;
  pinMode(Pin(GPIO_FTC532), INPUT_PULLUP);
  Ftc532.rxtime = micros();
  attachInterrupt(Pin(GPIO_FTC532), ftc532_ISR, RISING);
  Ftc532.present = true;
}

void ftc532_update(void) {                          // Usually called every 50 ms
  if ((Ftc532.sample & 0xF0F0) == ((~Ftc532.sample & 0x0F0F) << 4) && (Ftc532.sample >> 8) == 0xF0) {
    Ftc532.keys = Ftc532.sample & 0xF;
    if (Ftc532.keys != Ftc532.old_keys) {
#if FTC532_DEBOUNCE > 1
      if (++Ftc532.key_cnt >= FTC532_DEBOUNCE) {
#endif  // FTC532_DEBOUNCE > 1
#ifdef DEBUG_FTC532
        AddLog(LOG_LEVEL_DEBUG, PSTR("FTC: SAM=%04X KEY=%X OLD=%X INV=%u NOI=%u FRM=%u OK=%u TIME=%lu Pin=%u"),
                 Ftc532.sample, Ftc532.keys, Ftc532.old_keys, Ftc532.e_inv, Ftc532.e_noise, Ftc532.e_frame,
                 Ftc532.valid, Ftc532.rxtime, Pin(GPIO_FTC532));
#endif  // DEBUG_FTC532
        ftc532_publish();
        Ftc532.old_keys = Ftc532.keys;
#if FTC532_DEBOUNCE > 1
        Ftc532.key_cnt = 0;
      }
    } else {
      Ftc532.key_cnt = 0;
#endif  // FTC532_DEBOUNCE > 1
    }
  }
#ifdef DEBUG_FTC532
  else {
    ++Ftc532.e_inv;
    AddLog(LOG_LEVEL_DEBUG, PSTR("FTC: ILL SAM=%04X"), Ftc532.sample);
  }
#endif  // DEBUG_FTC532
}

void ftc532_show() {
  ResponseAppend_P(PSTR(",%s%02X\"}"), ftc532_json, Ftc532.keys);   // Hex keys need JSON quotes
}

void ftc532_publish(void) {
  Response_P(PSTR("{%s%02X\"}}"), ftc532_json, Ftc532.keys);        // Hex keys need JSON quotes
  MqttPublishTeleSensor();
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv47(uint32_t function) {
  bool result = false;

  if (FUNC_INIT == function) {
    // Initialize driver
    ftc532_init();
  } else if (Ftc532.present) {
    switch (function) {
      // timed callback functions
      case FUNC_EVERY_50_MSECOND:
        ftc532_update();
        break;
      // Generate JSON telemetry string
      case FUNC_JSON_APPEND:
        ftc532_show();
        break;
      }
  }
  // Return bool result
  return result;
}

#endif  // USE_FTC532
