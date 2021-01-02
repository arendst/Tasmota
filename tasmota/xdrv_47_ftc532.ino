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
  de-bouncing for 50 ms             NOT REQUIRED
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
\*********************************************************************************************/

#define XDRV_47                   47

#define FTC532_KEYS_MAX           8

#define FTC532_STATE_WAITING      false
#define FTC532_STATE_READING      true

// Rising edge timing in microseconds
#define FTC532_BIT                377
#define FTC532_SHORT              (FTC532_BIT * 2)
#define FTC532_LONG               (FTC532_BIT * 4)
#define FTC532_IDLE               (FTC532_BIT * 10)
#define FTC532_MAX                (FTC532_BIT * 58)

struct FTC532 {
  volatile uint32_t rxtime;             // ISR timer memory
  volatile uint16_t sample = 0xF0F0;    // buffer for bit-coded time samples
  volatile uint16_t rxbit;              // ISR bit counter
  uint8_t keys = 0;                     // bitmap of active keys
  uint8_t old_keys = 0;                 // previously active keys
  volatile bool state;                  // ISR state
  bool present = false;
#ifdef DEBUG_TASMOTA_DRIVER
  volatile uint16_t errors;             // error counter
  volatile bool valid;                  // did we ever receive valid data?
#endif  // DEBUG_TASMOTA_DRIVER
} Ftc532;

const char ftc532_json[] PROGMEM = "\"FTC532\":{\"KEYS\":\"";

void ICACHE_RAM_ATTR ftc532_ISR(void) {   // Hardware interrupt routine, triggers on rising edge
  uint32_t time = micros();
  uint32_t time_diff = time - Ftc532.rxtime;
  Ftc532.rxtime = time;

  if (Ftc532.state == FTC532_STATE_WAITING) {
    if (time_diff > FTC532_LONG + FTC532_SHORT) {  // new frame
      Ftc532.rxbit = 0;
      Ftc532.state = FTC532_STATE_READING;
    }
    return;
  }             // FTC532_STATE_READING starts here
  if (time_diff > FTC532_LONG + FTC532_BIT) {
#ifdef DEBUG_TASMOTA_DRIVER
    ++Ftc532.errors;                               // frame error
#endif  // DEBUG_TASMOTA_DRIVER
    Ftc532.state = FTC532_STATE_WAITING;
    return;
  }
  if (time_diff > FTC532_SHORT + FTC532_BIT) {
    Ftc532.sample |= (1 << Ftc532.rxbit);          // LONG
  } else {
    Ftc532.sample &= ~(1 << Ftc532.rxbit);         // SHORT
  }
  ++Ftc532.rxbit;
  if (Ftc532.rxbit == FTC532_KEYS_MAX * 2) {       // frame complete
    Ftc532.rxbit = 0;
#ifdef DEBUG_TASMOTA_DRIVER
    Ftc532.valid = true;
#endif  // DEBUG_TASMOTA_DRIVER
    Ftc532.state = FTC532_STATE_WAITING;
  }
}

void ftc532_init(void) {                           // Initialize
  if (!PinUsed(GPIO_FTC532)) { return; }
#ifdef DEBUG_TASMOTA_DRIVER
  Ftc532.errors = 0;
  Ftc532.valid = false;
#endif  // DEBUG_TASMOTA_DRIVER
  Ftc532.state = FTC532_STATE_WAITING;
  Ftc532.rxtime = micros();
  pinMode(Pin(GPIO_FTC532), INPUT_PULLUP);
  attachInterrupt(Pin(GPIO_FTC532), ftc532_ISR, RISING);
  Ftc532.present = true;
}

void ftc532_update(void) {                         // Usually called every 50 ms
#ifdef DEBUG_TASMOTA_DRIVER
//   WARNING: Reduce callback frequency if this code is enabled
//  if ((Ftc532.sample & 0xF) != ((~Ftc532.sample >> 4) & 0xF) || ((Ftc532.sample >> 8) & 0xF) != ((~Ftc532.sample >> 12) & 0xF)) {
//    AddLog_P(LOG_LEVEL_DEBUG, PSTR("FTC: inverted sample does not match %x %x %x %x"),
//             Ftc532.sample & 0xF, (~Ftc532.sample >> 4) & 0xF, (Ftc532.sample >> 8) & 0xF, (~Ftc532.sample >> 12) & 0xF);
//  }
#endif  // DEBUG_TASMOTA_DRIVER
  Ftc532.keys = (Ftc532.sample & 0xF) | ((Ftc532.sample >> 4) & 0xF0);
  if (Ftc532.keys != Ftc532.old_keys) {
#ifdef DEBUG_TASMOTA_DRIVER
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("FTC: SAM=%04X KEY=%02X OLD=%02X ERR=%u OK=%u TIME=%lu Pin=%u"),
             Ftc532.sample, Ftc532.keys, Ftc532.old_keys, Ftc532.errors, Ftc532.valid, Ftc532.rxtime, Pin(GPIO_FTC532));
#endif  // DEBUG_TASMOTA_DRIVER
    ftc532_publish();
    Ftc532.old_keys = Ftc532.keys;
  }
}

void ftc532_show() {
  ResponseAppend_P(PSTR(",%s%02X\"}"), ftc532_json, Ftc532.keys);  // Hex keys need JSON quotes
}

void ftc532_publish(void) {
  Response_P(PSTR("{%s%02X\"}}"), ftc532_json, Ftc532.keys);       // Hex keys need JSON quotes
  MqttPublishTeleSensor();
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv47(uint8_t function) {
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
