/*
  xdrv_48_bs814a2.ino - BS814A-2 touch buttons support for Tasmota

  Copyright (C) 2020 Peter Franck and Theo Arends

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
  poll data pin for low state           DONE
  generate high-speed clock (25kHz)     DONE
  poll data bits on clock plateau       sample after rising clk edge
  update data & report every 50 ms      DONE
  error & retry logic (if reqired)      TODO
  MQTT message & rules hook             DONE
  test on actual Hardware               TODO

  THE PROTOCOL [tm]:
  ==================
  see Holtek-Semicon BS814A-2 datasheet for details
\*********************************************************************************************/

#define XDRV_48                   48

#define BS814_KEYS_MAX            4

// Timing in microseconds
#define BS814_BIT                 40
#define BS814_PULSE               20
#define BS814_ERR_WAIT            6000
#define BS814_FREQ                25000

#define DEBUG_BS814_DRIVER        // @@@@@@@

struct BS814 {
  uint8_t keys = 0;               // bitmap of active & old keys: [ooookkkk]
  bool present = false;           // driver initialized
#ifdef DEBUG_BS814_DRIVER
  uint16_t errors;                // error counter
  bool valid;                     // did we ever receive valid data?
#endif  // DEBUG_BS814_DRIVER
} Bs814;

const char Bs814_json[] PROGMEM = "\"BS814\":{\"KEYS\":\"";

extern "C" {
  void os_delay_us(uint32_t);
}

void bs814_init(void) {                         // Initialize
  if (!PinUsed(GPIO_BS814_CLK) || !PinUsed(GPIO_BS814_DAT)) { return; }
  pinMode(Pin(GPIO_BS814_DAT), INPUT_PULLUP);
  pinMode(Pin(GPIO_BS814_CLK), OUTPUT);
  Bs814.present = true;
  bs814_read();
}

void bs814_read(void) {                         // Poll touch keys
  uint8_t frame;
  uint8_t bitpos;
  uint8_t bitval;
  uint8_t checksum = 0;
  
  // Data waiting for us?
//@@@@  if (digitalRead(Pin(GPIO_BS814_DAT) == HIGH)) { return; }

  // generate clock signal & read frame on rising edge
  for (bitpos = 0; bitpos < 2 * BS814_KEYS_MAX - 1; ++bitpos) {
    digitalWrite(Pin(GPIO_BS814_CLK), LOW);
    os_delay_us(BS814_PULSE);
    digitalWrite(Pin(GPIO_BS814_CLK), HIGH);
    bitval = digitalRead(Pin(GPIO_BS814_DAT));
    frame |= (bitval << bitpos);
    if (bitpos < BS814_KEYS_MAX) {
      checksum += bitval;
    }
    os_delay_us(BS814_PULSE);
  }
  digitalWrite(Pin(GPIO_BS814_CLK), LOW);
  os_delay_us(BS814_PULSE);
  digitalWrite(Pin(GPIO_BS814_CLK), HIGH);
  bitval = digitalRead(Pin(GPIO_BS814_DAT));
  frame |= (bitval << bitpos);

  // validate frame TODO
  checksum = BS814_KEYS_MAX - checksum;                         // checksum count zeros
  Bs814.keys = (Bs814.keys & 0xF0) | (~frame & 0xF);            // frame logic inverted
}

void bs814_update(void) {                       // Usually called every 50 ms
  bs814_read();
  if ((Bs814.keys & 0xF) != (Bs814.keys >> 4)) {
#ifdef DEBUG_BS814_DRIVER
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("BS814: KEY=%0X OLD=%0X ERR=%u OK=%u CLK=%u DAT=%u"),
             Bs814.keys & 0xF, Bs814.keys >> 4, Bs814.errors, Bs814.valid, Pin(GPIO_BS814_CLK), Pin(GPIO_BS814_DAT));
#endif  // DEBUG_BS814_DRIVER
    bs814_publish();
    Bs814.keys = (Bs814.keys << 4) | (Bs814.keys & 0xF);
  }
}

void bs814_show() {
  ResponseAppend_P(PSTR(",%s%X\"}"), Bs814_json, Bs814.keys & 0xF);   // tele json
}

void bs814_publish(void) {
  Response_P(PSTR("{%s%X\"}}"), Bs814_json, Bs814.keys & 0xF);        // mqtt & rules
  MqttPublishTeleSensor();
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv48(uint8_t function) {
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
      }
  }
  // Return bool result
  return result;
}

#endif  // USE_BS814A2
