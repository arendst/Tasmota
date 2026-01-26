/*
  xdrv_51_bs814a2.ino - BS814A-2 touch buttons support for Tasmota

  Copyright (C) 2021 Peter Franck and Theo Arends

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

#ifdef USE_BS814A2
/*********************************************************************************************\
  THE PLAN [tm]:                        OUTCOME:
  ==============                        ========
  appear in a dropdown                  (BS814 CLK, BS814 DAT)
  select 2 pins (clk, data)             DONE
  poll data pin for low state           UNRELIABLE
  generate high-speed clock (25kHz)     DONE
  poll data bits on clock plateau       RISING EDGE
  update data & report every 50 ms      DONE
  error & retry logic (if reqired)      BASIC
  MQTT message & rules hook             DONE
  test on actual Hardware               DONE

  THE PROTOCOL [tm]:
  ==================
  see Holtek-Semicon BS814A-2 datasheet for details

  *********************
  * About this driver *
  *********************

  This driver implements the documented protocol of the Holtek BS814A-2 touch controller.
  The protocol encodes the bitmap of touched keys in a syncronous serial data stream and
  adds a 3-bit checksum to it. The Chip is supposed to alert the MCU of new key input by
  pulling the data line low. This mechanism would have been nice but it didn't function
  reliablly in my hardware sample. So I decided to poll the touch keys every 50 ms regard-
  less of a change signal. The rest of the driver logic follows pretty much my previous
  touch driver FTC532.

  Usage:
  ------
  This driver does not actually switch anything. It is a pure "rules" driver that solely emits
  {"BS814":{"KEYS":"XX"}} JSON messages to be used in a rule or by an MQTT broker. "XX" stands
  for the hexadecimal (big endian) representation of a bitmap of keys currently touched, where
  e.g. "00" means "no key touched" while "03" means "keys 1 and 2 touched simultaneously".
  Selecting "BS814 CLK" on one GPIO and "BS814 DAT" on another GPIO will awake the driver.
  This driver can only be selected once.

  NOTE: Key hex output is sent in 2 digits although it would fit in one.
        The reason is a provision for BS818A-2 8-key controllers.
\*********************************************************************************************/

#define XDRV_51                   51

#define BS814_KEYS_MAX            4         // no. of keys supported

// Timing in microseconds
#define BS814_BIT                 40        // serial bit time
#define BS814_PULSE               20        // clock pulse width
#define BS814_ERR_WAIT            6000      // minimum time before retry
#define BS814_FREQ                25000     // max bitrate

struct BS814 {
  uint8_t keys            = 0;              // bitmap of active & old keys: [ooookkkk]
  bool present            = false;          // driver initialized
#ifdef DEBUG_BS814_DRIVER
  uint16_t e_level        = 0;              // level disagree error
  uint16_t e_cksum        = 0;              // checksum errror
  uint16_t e_stp          = 0;              // stop bit error
  bool valid              = false;          // did we ever receive valid data?
#endif  // DEBUG_BS814_DRIVER
} Bs814;

const char Bs814_json[] PROGMEM = "\"BS814\":{\"KEYS\":\"";

extern "C" {
  void os_delay_us(uint32_t);
}

void bs814_init(void) {                     // Initialize
  if (!PinUsed(GPIO_BS814_CLK) || !PinUsed(GPIO_BS814_DAT)) { return; }
  pinMode(Pin(GPIO_BS814_DAT), INPUT_PULLUP);
  pinMode(Pin(GPIO_BS814_CLK), OUTPUT);
  Bs814.present = true;
  bs814_read();
}

void bs814_read(void) {                     // Poll touch keys
  uint8_t frame    = 0;
  uint8_t checksum = 0;
  uint8_t bitp;
  bool bitval;

  // generate clock signal & sample frame
  for (bitp = 0; bitp < 2 * BS814_KEYS_MAX; ++bitp) {
    digitalWrite(Pin(GPIO_BS814_CLK), LOW);
    os_delay_us(BS814_PULSE);
    digitalWrite(Pin(GPIO_BS814_CLK), HIGH);
    bitval = digitalRead(Pin(GPIO_BS814_DAT));
#ifdef DEBUG_BS814_DRIVER
    if (bitval != digitalRead(Pin(GPIO_BS814_DAT))) {               // value consistent?
      ++Bs814.e_level;
    }
#endif  // DEBUG_BS814_DRIVER
    frame |= (bitval << bitp);
    if (bitp < 2 * BS814_KEYS_MAX - 1) {                            // stop bit
      if (bitp < BS814_KEYS_MAX) {
        checksum += bitval;                                         // checksum key bits
      }
      os_delay_us(BS814_PULSE);
    }
  }
  // validate frame
  if (BS814_KEYS_MAX - checksum != ((frame >> 4) & 0x7)) {          // checksum error
#ifdef DEBUG_BS814_DRIVER
    ++Bs814.e_cksum;
    AddLog(LOG_LEVEL_DEBUG, PSTR("CKS=%02X CFR=%02X"), checksum, (frame >> 4) & 0x7);
#endif  // DEBUG_BS814_DRIVER
    return;
  }
  if ((frame & 0x80) == 0) {                                        // stop bit error
#ifdef DEBUG_BS814_DRIVER
    ++Bs814.e_stp;
#endif  // DEBUG_BS814_DRIVER
    return;
  }
  Bs814.keys = (Bs814.keys & 0xF0) | (~frame & 0xF);                // frame logic inverted
#ifdef DEBUG_BS814_DRIVER
  Bs814.valid = true;
#endif  // DEBUG_BS814_DRIVER
}

void bs814_update(void) {                   // Usually called every 50 ms
  bs814_read();
  if ((Bs814.keys & 0xF) != (Bs814.keys >> 4)) {
#ifdef DEBUG_BS814_DRIVER
    AddLog(LOG_LEVEL_DEBUG, PSTR("BS814: KEY=%0X OLD=%0X LVL=%u CKS=%u STP=%u OK=%u CLK=%u DAT=%u"),
             Bs814.keys & 0xF, Bs814.keys >> 4, Bs814.e_level, Bs814.e_cksum, Bs814.e_stp,
             Bs814.valid, Pin(GPIO_BS814_CLK), Pin(GPIO_BS814_DAT));
#endif  // DEBUG_BS814_DRIVER
    bs814_publish();
    Bs814.keys = (Bs814.keys << 4) | (Bs814.keys & 0xF);
  }
}

void bs814_show() {
  ResponseAppend_P(PSTR(",%s%02X\"}"), Bs814_json, Bs814.keys & 0xF);   // tele json
}

void bs814_publish(void) {
  Response_P(PSTR("{%s%02X\"}}"), Bs814_json, Bs814.keys & 0xF);        // mqtt & rules
  MqttPublishTeleSensor();
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv51(uint32_t function) {
  bool result = false;

  if (FUNC_INIT == function) {
    // Initialize driver
    bs814_init();
  } else if (Bs814.present) {
    switch (function) {
      // timed callback functions
      case FUNC_EVERY_50_MSECOND:
        bs814_update();
        break;
      // Generate JSON telemetry string
      case FUNC_JSON_APPEND:
        bs814_show();
        break;
      case FUNC_ACTIVE:
        result = true;
        break;
      }
  }
  // Return bool result
  return result;
}

#endif  // USE_BS814A2
