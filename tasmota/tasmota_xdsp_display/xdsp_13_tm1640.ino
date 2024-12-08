/*
  xdsp_13_tm1640.ino - TM1640B LED display controller support for Tasmota

  Copyright (C) 2024  Stefan Oskamp

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

#ifdef USE_DISPLAY
#ifdef USE_DISPLAY_TM1640
/*********************************************************************************************\
  This driver enables the display of the current time, numbers (both integers and floats) and 
  basic text on the IoTTimer clock.

  Template {"NAME":"IoTTimer Lo","GPIO":[32,33,0,34,3872,1312,0,0,10944,10912,640,480,608,4768],"FLAG":0,"BASE":18}

  In addition, it is possible to set brightness (seven levels plus off) and clear the display.

  To use, compile Tasmota with USE_DISPLAY, USE_DISPLAY_TM1640 and USE_IOTTIMER, or build the tasmota-display
  firmware.

  Either use following template:
  Template {"NAME":"IoTTimer Lo","GPIO":[32,33,0,34,3872,1312,0,0,10944,10912,640,480,608,4768],"FLAG":0,"BASE":18}

  or configure manually:
  For the IoTTimer clock assign the pins as follows from Tasmota's GUI:

  GPIO12 --> "TM1640 DIN"
  GPIO13 --> "TM1640 CLK"

  Once the GPIO configuration is saved and the ESP8266/ESP32 module restarts, set the Display
  Model to 13 and Display Mode to 0 using the command "Backlog DisplayModel 13 ; DisplayMode 0;"
  Before using it, set the Display Type to 1 (for IOTTIMER) using the "DisplayType 1" command.

  After the ESP8266 restarts again, turn ON the display with the command "Power 1"

  Now, the following "Display" commands can be used:


  DisplayClear

                               Clears the display, command: "DisplayClear"


  DisplayFloat          num

                               Clears and then displays float (with decimal point)  command e.g., "DisplayFloat 12.3"
                               See function description below for more details.


  DisplayFloatNC        num

                               Same as DisplayFloatNC


  DisplayClock  1|2|3|4

                               Displays a clock.
                               Commands "DisplayClock 1"     // 12 hr format
                                        "DisplayClock 2"     // 24 hr format
                                        "DisplayClock 3"     // 12-hour without seconds
                                        "DisplayClock 4"     // 24-hour without seconds


In addition, if you compile using USE_DISPLAY_MODES1TO5, setting DisplayMode to 1 shows the time,
setting it to 2 shows the date and setting it to 3 alternates between time and date (using "DisplayRefresh [1..7]" 
for the time and seconds you want to show the time before displaying the date)

\*********************************************************************************************/

#define XDSP_13                    13

#define TM1640_CMD_DATA_AUTO     0x40
#define TM1640_CMD_DATA_FIXED    0x44
#define TM1640_CMD_DISPLAY       0x80
#define TM1640_CMD_ADDRESS       0xC0

#define TM1640_CLOCK_DELAY          1    // uSec

#define LEVEL_MIN                   0
#define LEVEL_MAX                 100

enum tm1640_display_options_types {
  TM1640_DEFAULT,
  TM1640_IOTTIMER     // IOTTIMER WiFi clock
};

typedef struct Tm1640_t {
  int8_t clock_pin;
  int8_t data_pin;
  bool show_clock;
  bool clock_24;
  bool clock_seconds;
} Tm1640_t;

Tm1640_t* Tm1640 = nullptr;

/*********************************************************************************************\
 * TM1640 low level functions
\*********************************************************************************************/

void TM1640Start (void) {
  digitalWrite(Tm1640->data_pin, LOW);
  digitalWrite(Tm1640->clock_pin, LOW);
  delayMicroseconds(TM1640_CLOCK_DELAY);
}

void TM1640Stop (void) {
  digitalWrite(Tm1640->clock_pin, HIGH);
  digitalWrite(Tm1640->data_pin, HIGH);
  delayMicroseconds(TM1640_CLOCK_DELAY);
}

void TM1640Send(uint8_t data) {
	for (uint32_t i = 0; i < 8; i++) {          // 8 bits
    digitalWrite(Tm1640->data_pin, data & 1 ? HIGH : LOW);
    delayMicroseconds(TM1640_CLOCK_DELAY);
    data >>= 1;
    digitalWrite(Tm1640->clock_pin, HIGH);
    delayMicroseconds(TM1640_CLOCK_DELAY);
    digitalWrite(Tm1640->clock_pin, LOW);
    delayMicroseconds(TM1640_CLOCK_DELAY);
  }
  digitalWrite(Tm1640->data_pin, LOW);
  delayMicroseconds(TM1640_CLOCK_DELAY);
}

void TM1640SendData(uint8_t address, uint8_t data) {
  // First, send data command using FIXED addressing:
  TM1640Start();
  TM1640Send(TM1640_CMD_DATA_FIXED);
  TM1640Stop();
  // Then, send address and one data byte:
	TM1640Start();
  TM1640Send(TM1640_CMD_ADDRESS | address);
  TM1640Send(data);
  TM1640Stop();
}

void TM1640SendDataArray(uint8_t address, uint8_t *data, uint8_t count) {
  // First, send data command using AUTO addressing:
  TM1640Start();
  TM1640Send(TM1640_CMD_DATA_AUTO);
  TM1640Stop();
  // Then, send address and all data bytes:
	TM1640Start();
  TM1640Send(TM1640_CMD_ADDRESS | address);
  while (count-- > 0) {
    TM1640Send(*data++);
  }
  TM1640Stop();
}

void TM1640SetBrightness(uint8_t level) {
  // level can be 0 to 8. 
  // 0 means off
  // 
  // Other levels are mapped to TM1640 levels 0 ... 7
  // The mapping to the PWM level is non-linear, according to the data sheet
  // level | TM1640 | PWM
  //     1 |      0 | 1/16
  //     2 |      1 | 2/16
  //     3 |      2 | 4/16
  //     4 |      3 | 10/16
  //     5 |      4 | 11/16
  //     6 |      5 | 12/16
  //     7 |      6 | 13/16
  //     8 |      7 | 14/16
  uint8_t cmd = TM1640_CMD_DISPLAY | (level > 0 ? 0x8 : 0) | ((level - 1) % 8);
  TM1640Start();
  TM1640Send (cmd);  
  TM1640Stop();
}

/*********************************************************************************************\
* Init function
\*********************************************************************************************/

void TM1640Init(void) {
  if (PinUsed(GPIO_TM1640CLK) && PinUsed(GPIO_TM1640DIN)) {
    Tm1640 = (Tm1640_t*)calloc(sizeof(Tm1640_t), 1);    // Need calloc to reset registers to 0/false
    if (nullptr == Tm1640) { return; }

    Tm1640->clock_pin = Pin(GPIO_TM1640CLK);
    Tm1640->data_pin = Pin(GPIO_TM1640DIN);

    pinMode(Tm1640->data_pin, OUTPUT);
    pinMode(Tm1640->clock_pin, OUTPUT);
    digitalWrite(Tm1640->clock_pin, HIGH);
    digitalWrite(Tm1640->data_pin, HIGH);

    Settings->display_model = XDSP_13;

#ifdef USE_IOTTIMER
    Settings->display_options.type = TM1640_IOTTIMER;

    Settings->display_cols[0] = 9;   // 4 (left) + 2 (lower right) + 3 (upper right).
    Settings->display_rows = 1;
    Settings->display_width = Settings->display_cols[0];
    Settings->display_height = Settings->display_rows;

    Tm1640->clock_24 = true;
    Tm1640->clock_seconds = true;

    IoTTimerDim();
    IoTTimerClearDisplay();
#endif  // USE_IOTTIMER

    AddLog(LOG_LEVEL_INFO, PSTR("DSP: TM1640 with %d digits (type %d)"), Settings->display_width, Settings->display_options.type);
  }
}


/*********************************************************************************************\
* IotTimer
\*********************************************************************************************/

#ifdef USE_IOTTIMER

/*
  (specifically for its use in the IOTTIMER WiFi clock)

  The WiFi LED clock called IOTTIMER has the following characteristics:
   - Controlled by an ESP-12F
   - Display with four 35 mm (1 12/32 in), two 21 mm (26/32 in), and three 12 mm (~1/2 in),
     seven-segment LED digits, plus special symbols (alarm, AM/PM)
   - TM1640B LED controller
   - R8010 RTC with CR1220 battery
   - Temperature sensor M1601
   - Ambient light sensor (analog voltage)
   - Buzzer
   - Three buttons on the backside
   - USB C port for power supply (only)

  The TM1640B chip is a controller for a sixteen-digit seven-segment (plus dot) LED display.
  It is also sometimes used to control a 16 x 8 LED matrix. The controller is controlled 
  through a proprietary two-wire serial interface bearing some similarities with I2C. The 
  two wires are called CLK and DIN. We use two GPIO pins and one-microsecond sleeps to 
  implement the required timing.

  The wiring of the LEDs in the IOTTIMER clock has been optimized for a simple routing of the
  traces on the display board. The enumeration of the digit segments is non-standard, but
  consistent across all digits. The bigger digits have two LEDs per segment, controlled by
  separate digit lines of the LED controller. From the software perspective, they appear as
  two layers of four digits each.

  The brightness of the LEDs can be controlled in seven steps (plus off). In theory, the
  brightness of the segments with two LEDs could be set in fifteen levels (plus off).
  To keep things simple and to avoid brightness gradients within segments, both LEDs of a
  segment will always be set to the same level.

  The intention of this display driver (together with the drivers for the other components)
  is to be able to use the IOTTIMER as an alarm clock that can be fully integrated in your
  home automation using Tasmota and rules.

  This driver is not a generic TM1640B driver as use cases of the TM1640B in different 
  devices will differ significantly.
*/






#define IOTTIMER_DIGITS            16
#define IOTTIMER_DOT_BIT            2

static unsigned char IoTTimerDisplay[IOTTIMER_DIGITS];

// Wiring of the LEDs (per digit):
//
//    Seg#        Bit         Hex
//     07         06          40
//   08  01     07  00      80  01
//     02         01          02
//   06  04     05  03      20  08
//     05   03    04   02     10   04
//
// Font as per wiring:
static const byte IoTTimerFont[128] {
//0x00  0x01  0x02  0x03  0x04  0x05  0x06  0x07  0x08  0x09  0x0A  0x0B  0x0C  0x0D  0x0E  0x0F
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x00
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x10
// SP    !     "     #     $     %     &     '     (     )     *     +     ,     -     .     /
  0x00, 0xA0, 0x81, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x59, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, // 0x20
//0     1     2     3     4     5     6     7     8     9      :     ;     <     =     >     ?
  0xF9, 0x09, 0x73, 0x5B, 0x8B, 0xDA, 0xFA, 0x49, 0xFB, 0xDB, 0x00, 0x00, 0x00, 0x12, 0x00, 0x63, // 0x30
// @     A     B     C     D     E     F     G     H     I     J     K     L     M     N     O
  0x00, 0xEB, 0xBA, 0xF0, 0x3B, 0xF2, 0xE2, 0xFA, 0xAB, 0x09, 0x19, 0x00, 0xB0, 0x00, 0xE9, 0xF9, // 0x40
// P     Q     R     S     T     U     V     W     X     Y     Z     [     \     ]     ^(°)  _
  0xE3, 0xAB, 0x22, 0xDA, 0xB2, 0xB9, 0x00, 0x00, 0x00, 0x4B, 0x00, 0xF0, 0x00, 0x59, 0xC3, 0x10, // 0x50 
// `=°   a     b     c     d     e     f     g     h     i     j     k     l     m      n     o
  0x01, 0x7B, 0xBA, 0x32, 0x3B, 0xF3, 0xE2, 0xDB, 0xAA, 0x08, 0x19, 0x00, 0x09, 0x00, 0x2A, 0x3A, // 0x60
// p     q     r     s     t     u     v     w     x     y     z      {     |     }    ~     DEL
  0xE3, 0xAB, 0x22, 0xDA, 0xB2, 0x38, 0x00, 0x00, 0x00, 0x4B, 0x00, 0x0B, 0x09, 0xA2, 0x00, 0x00  // 0x70
};


void IoTTimerDim(void)
{
  TM1640SetBrightness (changeUIntScale(GetDisplayDimmer(), 0, 100, 0, 8));
}


void IoTTimerDisplayOn (void)
{
  IoTTimerDim();
}


void IoTTimerDisplayOff (void)
{
  TM1640SetBrightness (0);
}


void IoTTimerDisplayOnOff(void)
{
  if (disp_power) {
    IoTTimerDisplayOn();
  }
  else {
    IoTTimerDisplayOff();
  }
}


void IoTTimerClearDisplay (void)
{
  for (int i = 0; i < IOTTIMER_DIGITS; i++) {
	  IoTTimerDisplay[i] = 0;
  }
  TM1640SendDataArray(0, IoTTimerDisplay, IOTTIMER_DIGITS);
}


/*********************************************************************************************\
* Init function
\*********************************************************************************************/
void IoTTimerInit(uint8_t mode)
{
  switch(mode) {
    case DISPLAY_INIT_MODE:
      IoTTimerDim();
      IoTTimerClearDisplay();
      break;
    case DISPLAY_INIT_PARTIAL:
    case DISPLAY_INIT_FULL:
      IoTTimerDim();
      IoTTimerClearDisplay();
      break;
  }
}

void IoTTimerDrawStringAt(uint32_t x, uint32_t y, const char *str, uint32_t color = 0, uint32_t flag = 0);
void IoTTimerDrawStringAt(uint32_t x, uint32_t y, const char *str, uint32_t color, uint32_t flag) {
  //    displaytext [x] = 0123456789
  // Considers display as 1111223334 where 1111 is large white display,
  //                                         22 is small white display,
  //                                        333 is green display,
  //                                          4 is "1" = pm, "2" = alarm, "3" = both
  // Following also works - notice scattered dot(.), colon(:), minus(-) or plus(+)
  // displaytext 12:34:56.7.8.9ab  - Show all lights including "pm" (=a) and "alarm" (=b)
  // displaytext 12:34:56          - Show time
  // displaytext 05-11-24          - Show date
  // displaytext [x6]12.3          - Show value in green leds
  // displaytext [ztS]             - Clear display and show current time with seconds

  bool alarm = false;
  bool pm = false;
  bool dot_left_up = false;
  bool dot_left_dn = false;
  bool dot_right_dn = false;
  bool dot_right_up_left = false;
  bool dot_right_up_right = false;
  bool dash_left = false;
  bool dash_right = false;

  char chr;
  uint32_t idx = x;
  const char *pos = str;
  while (*pos) {
    chr = *pos & 0x7F;                                         // We only support 0 to 127
    switch (idx) {
      case 0:
        IoTTimerDisplay[12] = IoTTimerDisplay[13] = IoTTimerFont[chr];  // col 0
        break;
      case 1:
        IoTTimerDisplay[14] = IoTTimerDisplay[15] = IoTTimerFont[chr];  // col 1
        break;
      case 2:
        if (('.' == chr) || (':' == chr) || ('-' == chr) || ('+' == chr)) {
          if ('.' == chr) {
            dot_left_dn = true;
          }
          else if (':' == chr) {
            dot_left_up = true;
            dot_left_dn = true;
          }
          else if ('-' == chr) {
            dash_left = true;
            dash_right = true;
          }
          else if ('+' == chr) {
            dot_left_up = true;
            dot_left_dn = true;
            dash_left = true;
            dash_right = true;
          }
          idx--;
        } else {
          IoTTimerDisplay[4]  = IoTTimerDisplay[5] = IoTTimerFont[chr];  // col 2
        }
        break;
      case 3:
        IoTTimerDisplay[11] = IoTTimerDisplay[1] = IoTTimerFont[chr];    // col 3
        break;
      case 4:
        if (('.' == chr) || (':' == chr) || ('-' == chr)) {
          idx--;                                               // Skip
        } else {
          IoTTimerDisplay[6] = IoTTimerFont[chr];              // col 4
        }
        break;
      case 5:
        IoTTimerDisplay[7] = IoTTimerFont[chr];                // col 5
        break;
      case 6:
        if ('.' == chr) {
          dot_right_dn = true;
          idx--;
        } else {
          // Upper right (green)
          IoTTimerDisplay[9] = IoTTimerFont[chr];              // col 6
        }
        break;
      case 7:
        if ('.' == chr) {
          dot_right_up_left = true;
          idx--;
        } else {
          IoTTimerDisplay[10] = IoTTimerFont[chr];             // col 7
        }
        break;
      case 8:
        if ('.' == chr) {
          dot_right_up_right  = true;
          idx--;
        } else {
          IoTTimerDisplay[8] = IoTTimerFont[chr];              // col 8
        }
        break;
      case 9:                                                  // col 9
        if (chr & 0x01) {                                      // 1, A, a
          pm = true;
        }
        if (chr & 0x02) {                                      // 2, B, b
          alarm = true;
        }
        break;
    }
    idx++;
    pos++;
  }

  // Dots and dash
  if (alarm) {
    IoTTimerDisplay[12] |= 1 << IOTTIMER_DOT_BIT;              // Alarm symbol
  }
  if (pm) {
    IoTTimerDisplay[13] |= 1 << IOTTIMER_DOT_BIT;              // PM
  }
  if (dot_left_up) {
    IoTTimerDisplay[14] |= 1 << IOTTIMER_DOT_BIT;              // Upper dot
  }
  if (dot_left_dn) {
    IoTTimerDisplay[4]  |= 1 << IOTTIMER_DOT_BIT;              // Lower dot
  }
  if (dot_right_dn) {
    IoTTimerDisplay[7]  |= 1 << IOTTIMER_DOT_BIT;              // Blue dot
  }
  if (dot_right_up_left) {
    IoTTimerDisplay[10] |= 1 << IOTTIMER_DOT_BIT;              // Green dot left
  }
  if (dot_right_up_right) {
    IoTTimerDisplay[8]  |= 1 << IOTTIMER_DOT_BIT;              // Green dot right
  }
  if (dash_left) {
    IoTTimerDisplay[5]  |= 1 << IOTTIMER_DOT_BIT;
  }
  if (dash_right) {
    IoTTimerDisplay[15] |= 1 << IOTTIMER_DOT_BIT;
  }

  TM1640SendDataArray(0, IoTTimerDisplay, IOTTIMER_DIGITS);
}


/*********************************************************************************************\
* Displays floating point number in the upper right sub-display of the IOTTIMER.
* Format is always "[n]n[.]n" (negative number is "-n[.]n")
\*********************************************************************************************/
void IoTTimerShowFloat(float f) {
/*
  char buffer[16];
  ext_snprintf_P(buffer, sizeof(buffer),PSTR("%1_f"), &f);
  IoTTimerDrawStringAt(6, 0, buffer);
*/
  bool negative = false;
  float threshold = 99.95;
  if (f < 0.0) {
    f = -f;
    negative = true;
    threshold = 9.95;
  }
  uint8_t precision = 0;
  if (f < threshold) {
    f *= 10.0;
    precision++;
  }
  uint32_t n = (uint32_t) (f + 0.5);
  char buffer[5] = { 0 };
  if (negative) {
    if (n > 99) {
      n = 99;
    }
    buffer[0] = '-';
  } else {
    if (n > 999) {
      n = 999;
    }
    if (n / 100 != 0) {
      buffer[0] = '0' + n / 100;
    }
  }
  buffer[1] = '0' + n % 100 / 10;
  uint32_t idx = 2;
  if (precision == 1) {
    buffer[2] = '.';
    idx++;
  }
  buffer[idx] = '0' + n % 10;
  IoTTimerDrawStringAt(6, 0, buffer);
}


/*********************************************************************************************\
* Update the temperature in the upper right corner.
\*********************************************************************************************/
void IoTTimerUpdateTemperature(void) {
  IoTTimerShowFloat(ConvertTempToFahrenheit(TasmotaGlobal.temperature_celsius));
}


/*********************************************************************************************\
* Adjust the brightness based on the photo diode voltage.
\*********************************************************************************************/
void IoTTimerAdjustBrightness(void) {
  // Max ADC value is 3400 [lx], but that is only reached in direct sun light.
  // 20 is already quite bright.
  // Illuminance value of 0 should map to level 1 (level 0 is off)
  static float filteredLevel = 1.0;
  float level = (float) AdcGetLux(0) / (20.0 / 7.0) + 1.0; 
  if (level > 8.0) level = 8.0;
  if (level < 1.0) level = 1.0;
  filteredLevel = 0.9 * filteredLevel + 0.1 * (float) level;

  TM1640SetBrightness ((int) (filteredLevel + 0.5));
}


#ifdef USE_DISPLAY_MODES1TO5

/*********************************************************************************************\
* Show the current time
\*********************************************************************************************/
void IoTTimerShowTime(void) {
  uint8_t hour = RtcTime.hour;
  uint8_t min = RtcTime.minute;
  uint8_t sec = RtcTime.second;
  uint8_t symbol = 0;

  if (!Tm1640->clock_24) {
    if (hour > 12) {
      hour -= 12;
    }
    if (hour == 0) {
      hour = 12;
    }
    symbol |= 1;
  }

  char buffer[16];
  snprintf_P(buffer, sizeof(buffer), PSTR("%2d:%02d"), hour, min);
  if (Tm1640->clock_seconds) {
    snprintf_P(buffer, sizeof(buffer), PSTR("%s:%02d"), buffer, sec);
  } else {
    snprintf_P(buffer, sizeof(buffer), PSTR("%s  "), buffer);         // Erase seconds in case toggling between date/time
  }
  IoTTimerDrawStringAt(0, 0, buffer);

  if (Settings->timer[0].arm) {
    symbol |= 2;
  }

  snprintf_P(buffer, sizeof(buffer), PSTR("%d"), symbol);
  IoTTimerDrawStringAt(9, 0, buffer);
}


/*********************************************************************************************\
* Show the current date
\*********************************************************************************************/
void IoTTimerShowDate(void)
{
  uint8_t left   = RtcTime.day_of_month;
  uint8_t middle = RtcTime.month;
  uint8_t right  = RtcTime.year % 100;

  if (!Tm1640->clock_24) {
    // Use U.S. date format.
    left   = RtcTime.month;
    middle = RtcTime.day_of_month;
  }

  char buffer[16];
  snprintf_P(buffer, sizeof(buffer), PSTR("%02d-%02d-%02d"), left, middle, right);
  IoTTimerDrawStringAt(0, 0, buffer);
}


void IoTTimerRefresh(void) {  // Every second
  if (!disp_power) { return; }

  // Update temperature display content:
  IoTTimerUpdateTemperature();

  // Auto-adjust brightness:
  IoTTimerAdjustBrightness();

  if (Settings->display_mode) {  // Mode 0 is User text
    switch (Settings->display_mode) {
      case 1:  // Time
        IoTTimerShowTime();
        break;
      case 2:  // Date
        IoTTimerShowDate();
        break;
      case 3: // Time/Date
        if (TasmotaGlobal.uptime % Settings->display_refresh)
        {
          IoTTimerShowTime();
        }
        else
        {
          IoTTimerShowDate();
        }
        break;
      case 4: 
      case 5:
        // not in use
        break;
    }
  }
}

#endif  // USE_DISPLAY_MODES1TO5


/*********************************************************************************************\
* Displays a clock.
* Command: DisplayClock 1   // 12-hour format
*          DisplayClock 2   // 24-hour format
*          DisplayClock 3   // 12-hour without seconds
*          DisplayClock 4   // 24-hour without seconds
\*********************************************************************************************/
void CmndIoTTimerClock(void) {
  uint16_t val = XdrvMailbox.payload;

  if (ArgC() == 0)
    val = 0;

  if ((val < 1) || (val > 4))
    return;

  if (val == 1) {
    Tm1640->show_clock = true;
    Tm1640->clock_24 = false;
    Tm1640->clock_seconds = true;
  } 
  else if (val == 2) {
    Tm1640->show_clock = true;
    Tm1640->clock_24 = true;
    Tm1640->clock_seconds = true;
  }
  else if (val == 3) {
    Tm1640->show_clock = true;
    Tm1640->clock_24 = false;
    Tm1640->clock_seconds = false;
  }
  else if (val == 4) {
    Tm1640->show_clock = true;
    Tm1640->clock_24 = true;
    Tm1640->clock_seconds = false;
  } else {
    Tm1640->show_clock = false;
    Tm1640->clock_24 = false;
  }

  IoTTimerClearDisplay();
}


#endif  // USE_IOTTIMER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdsp13(uint32_t function) {
  bool result = false;

  if (FUNC_DISPLAY_INIT_DRIVER == function) {
    TM1640Init();
  }
  else if (Tm1640 && (XDSP_13 == Settings->display_model)) {

#ifdef USE_IOTTIMER

    switch (function) {
      case FUNC_DISPLAY_INIT:
        IoTTimerInit(dsp_init);
        break;
#ifdef USE_DISPLAY_MODES1TO5
      case FUNC_DISPLAY_EVERY_SECOND:
        IoTTimerRefresh();
        break;
#endif  // USE_DISPLAY_MODES1TO5
      case FUNC_DISPLAY_MODEL:
        result = true;
        break;
      case FUNC_DISPLAY_CLOCK:
        CmndIoTTimerClock();
        break;
      case FUNC_DISPLAY_CLEAR:
        IoTTimerClearDisplay();
        break;
      case FUNC_DISPLAY_NUMBER:
      case FUNC_DISPLAY_NUMBERNC:
      case FUNC_DISPLAY_FLOAT:
      case FUNC_DISPLAY_FLOATNC:
        IoTTimerShowFloat(CharToFloat(XdrvMailbox.data));
        break;
      case FUNC_DISPLAY_DIM:
        IoTTimerDim();
        break;
      case FUNC_DISPLAY_POWER:
        IoTTimerDisplayOnOff();
        break;  
      case FUNC_DISPLAY_DRAW_STRING:
        IoTTimerDrawStringAt(dsp_x, dsp_y, dsp_str, dsp_color, dsp_flag);
        break;  
    }

#endif  // USE_IOTTIMER

  }
  return result;
}

#endif  // USE_DISPLAY_TM1640
#endif  // USE_DISPLAY
