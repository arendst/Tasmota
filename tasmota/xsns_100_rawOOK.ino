/*
  xsns_100_rawOOK.ino - WT450H, LaCrosse, Infactory, ??? 
                        temperature and humidity sensor support for Tasmota

  Copyright (C) 2020  Thorsten Pohlmann  tmo.tp@pohlmaenner.com

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

#ifdef USE_433_RAW

/*********************************************************************************************\
 * Decoding of OOK (on-off-keying) based temperature/humidity sensors from various 
 * weather stations sending at 433.92MHz and forwarding them via MQTT.
 * Just set the pin connected to the receiver to "OOK RX"
 * 
 * Required hardware:
 * Tested with:
 *   -- A Sonoff-RF-Bridge (433MHz) can be used with a minor hardware-patch, no Portisch-Firmware needed.
 *      Also the "normal" or Portisch features for decoding/sending are unharmed. 
 *     - Wire a 160...680 Ohm resistor from pin 10 of the EFM8BB1 to any free gpio of the ESP. 
 *       This might be gpio 4/5 if the USB lines have been cut (R2), or one of the free gpio 12/14 
 *       pads on the bottom-side. Im using gpio 4.
 *       See https://github.com/xoseperez/espurna/wiki/Hardware-Itead-Sonoff-RF-Bridge---Direct-Hack
 *  
 *   -- A CC1101 module (attached to a D1 mini) (#define USE_CC1101)
 *      using the Driver-lib from ELECHOUSE/lSatan
 *      With this you are not fixed to 433.92MHz, but any the CC1101 is capable, including 868MHz
 *      (I used a 868MHz module for 433MHz which is no problem since we are only receiving. 
 *       Maybe the sensitivity is not as good as with a 433MHz module)
 *      The pins for the CC1101 are fixed:
 *        SCK_PIN   = D1 Mini: D5 (GPIO 14)
 *        MISO_PIN  = D1 Mini: D6 (GPIO 12)
 *        MOSI_PIN  = D1 Mini: D7 (GPIO 13)
 *        SS_PIN    = D1 Mini: D8 (GPIO 15)
 *        GDO0      = any free gpio ("OOK RX")
 * 
 * Not tested, but should work with any 433MHz receiver providing a "carrier" signal
 * 
 * Commands: sensor100 debug 0..3 0: no debug output
 *                                1: pulse-duration output for protocol reverse-engineering
 *                                2: output from the various OOK decoders
 *                                3: 1+2   
 * 
 *           #if USE_CC1101:
 *           sensor100 freq xxx.yyy    set CC1101 to xxx.yyy MHz
 * 
 * Implemented decoder:
 * ====================
 * 
 * 1. WT450H (temp, hum, batt)
 * 2. LaCrosse TX3 (temp, hum)
 * 3. Infactory (Pearl) (temp, hum, batt)
 * 
 * New decoder:
 * ====================
 * 
 * 1.: implement: int your_protocol_decoder(bool initial_ook, uint16_t* pulses, int len) 
 *                                               initial_ook: pin state during 1st pulse
 *                                               return: != 0 if pulses contained valid data
 * 
 *            - you may reuse one of the OOK decoders (time-pulses -> 1/0)
 *            int decodeOOKxxx(int min_bits, uint16_t* pulses, int len, ...)
 *                             min_bits: minimum number of valid bits in a row needed for protocol parser
 *                             return: offset to pulses where parsing ended
 * 
 *            - stores bits as 0/1 in ook_data.data, number of bit: ook_data.num_data
 *            - returns at 1st invalid pulse (if enough bits in ook_data)
 *            - may then be called again by decoder with proper offset
 * 
 *            use mqttStart/mqttEnd() to send data
 *                           
 * 
 * 2.: Add your_protocol_decoder() to checkPulses()
 * 
 * 
 * 
 *********************************************************************************************/

#define XSNS_100         100

#ifdef USE_CC1101
/*
  To use a CC1101 board in ELECHOUSE/lSatan OOK-mode connect it like 
  SCK_PIN   = D1 Mini: D5 (GPIO 14)
  MISO_PIN  = D1 Mini: D6 (GPIO 12)
  MOSI_PIN  = D1 Mini: D7 (GPIO 13)
  SS_PIN    = D1 Mini: D8 (GPIO 15)
  GDO0      = pin template config "OOK RX"
*/

  #include "../lib/SmartRC-CC1101-Driver-Lib/ELECHOUSE_CC1101_SRC_DRV.h"
  static float cc1101_freq = 433.866;

  static bool use_cc1101 = false;
#endif

static uint8_t raw433_pin = 0; 

static uint8_t pin_interrupt = 0;

static void ICACHE_RAM_ATTR interruptHandler();

static void raw433Init(void)
{
  raw433_pin = Pin(GPIO_433M_RAW);

  if(raw433_pin >= 99)
  {
   // snprintf_P(log_data, sizeof(log_data), "433RAW init: no pin");
   // AddLog(LOG_LEVEL_DEBUG);
    return;
  }

  pinMode(raw433_pin, INPUT);

#if defined USE_CC1101 && defined ESP8266
  // ESP-32 pins?
  // check if the fixed CC1101 pins are being used by other sensors
  if(GetPin(10) + GetPin(11) + GetPin(12) + GetPin(13) == GPIO_NONE)
  {
    use_cc1101 = true;
    ELECHOUSE_cc1101.Init(); // Initialize the cc1101. 

    if(Settings.cc1101ook.freq1000 != 0)
      cc1101_freq = Settings.cc1101ook.freq1000;

    ELECHOUSE_cc1101.SetRx(cc1101_freq); //Sets receive on and changes the frequency.
    snprintf_P(log_data, sizeof(log_data), "CC1101 freq: %i.%03i", (int)cc1101_freq, (int)(cc1101_freq*100)%100);
  }
  else
  {
    snprintf_P(log_data, sizeof(log_data), "CC1101 default pins occupied");
  }
  AddLog(LOG_LEVEL_INFO);
#endif

  snprintf_P(log_data, sizeof(log_data), "433RAW init (pin: %i)", raw433_pin);
  AddLog(LOG_LEVEL_INFO);

  uint8_t interrupt = digitalPinToInterrupt(raw433_pin);
  if (pin_interrupt == interrupt) 
  {
    return;
  }
  
  if (pin_interrupt >= 0) 
  {
    detachInterrupt(pin_interrupt);
  }

  pin_interrupt = interrupt;

  if (interrupt >= 0) 
  {
    attachInterrupt(interrupt, interruptHandler, CHANGE);
  }
}

#ifdef USE_CC1101
const char CC1101_FREQ[] PROGMEM = "{\"CC1101_FREQ\":{%i.%03i}}";
#endif

const char OOK_DEBUG[] PROGMEM = "{\"OOK_DEBUG\":{%u}}";

typedef enum
{
  deb_pulses = 1<<0,
  deb_ook    = 1<<1,
  deb_ookval = 1<<2,
}debug_e;

static unsigned int debug_val = 0; //deb_ook | deb_pulses;

static bool ook_Command(void)
{
  bool ret = false;
  
  if (XdrvMailbox.data_len == 0) 
  {
    return ret;
  }
  
  int paramcount = 0;

  for (uint32_t ca=0;ca<XdrvMailbox.data_len;ca++) 
  {
    if ((' ' == XdrvMailbox.data[ca]) || ('=' == XdrvMailbox.data[ca]) ||  (',' == XdrvMailbox.data[ca])) 
    { 
      XdrvMailbox.data[ca] = ','; 
      ++paramcount; 
    }
  }

  UpperCase(XdrvMailbox.data, XdrvMailbox.data);
  
  char sub_string[XdrvMailbox.data_len];

#ifdef USE_CC1101
  if (!strcmp(subStr(sub_string, XdrvMailbox.data, ",", 1),"FREQ")) 
  { // Note 1 used for param number
    ret = true;
    if(use_cc1101)
    {
      if(paramcount)
      {
        float f = CharToFloat(subStr(sub_string, XdrvMailbox.data, ",", 2));  // Note 2 used for param number
        if(f > 430)
        {
          Settings.cc1101ook.freq1000 = (int)(f*1000);
          cc1101_freq = f;

          snprintf_P(log_data, sizeof(log_data), "CC1101 set freq: %i.%03i", (int)cc1101_freq, (int)(cc1101_freq*1000)%1000);
          AddLog(LOG_LEVEL_INFO);

          ELECHOUSE_cc1101.SetRx(cc1101_freq); //Sets receive on and changes the frequency.
        }
      }
      snprintf_P(log_data, sizeof(log_data), "CC1101 freq is: %i.%03i", (int)cc1101_freq, (int)(cc1101_freq*1000)%1000);
      AddLog(LOG_LEVEL_INFO);
      Response_P(CC1101_FREQ, (int)cc1101_freq, (int)(cc1101_freq*1000)%1000);
    }
  }
  else
#endif // USE_CC1101
  if (!strcmp(subStr(sub_string, XdrvMailbox.data, ",", 1),"DEBUG")) 
  { // Note 1 used for param number
    ret = true;
    if(paramcount)
    {
      debug_val = (unsigned int)atoi(subStr(sub_string, XdrvMailbox.data, ",", 2));  // Note 2 used for param number
    }
    snprintf_P(log_data, sizeof(log_data), "debug: %u", debug_val);
    AddLog(LOG_LEVEL_INFO);

    Response_P(OOK_DEBUG, debug_val);
  }
  return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MIN_OOKLEN       400
#define MAX_OOKLEN       8000
#define MIN_VALID_PULSES 20

#define MAX_PULSES_NUM   256

typedef struct 
{
  int       num_pulses;
  uint8_t   initial_ook;
  uint16_t  pulse[MAX_PULSES_NUM];
}pulses_t;

// buffer for pulse measurement
static pulses_t pulses_arr[8];

// ring buffer pulse_arr
static int          cur_pulses_rd = 0;
static volatile int cur_pulses_wr = 0;

// interrupt context: 
// - measure time since last pin-change
// - add time to pulse_arr[x]
// - mark pulse_arr[x] as good if >= MIN_VALID_PULSES
static void ICACHE_RAM_ATTR interruptHandler() 
{
  static unsigned long last = micros();
  static int err_cnt = 0;
  static int good_cnt = 0;

  unsigned long now = micros();

  int dif = now - last; // time since last irq
  last = now;

  int trigger = 0;

  pulses_t& p = pulses_arr[cur_pulses_wr]; // current ring buffer entry

  if(dif < MIN_OOKLEN || dif > MAX_OOKLEN) // is a invalid (too short/long) pulse
  {
    if(++err_cnt >= 5) 
    {
      if(good_cnt >= MIN_VALID_PULSES)
      {
        trigger = 1; // if there are enough good pulses in a row mark buffer as valid
      }
      else
      {
        p.num_pulses = 0; // too many errors, restart buffer
        err_cnt = 0;
        good_cnt = 0;
        return;
      }
    }
  }
  else
  {
    err_cnt = 0;
    ++good_cnt;
  }
  
  p.pulse[p.num_pulses++] = dif;

  if(!trigger && p.num_pulses < MAX_PULSES_NUM)
    return;

  err_cnt = 0;
  good_cnt = 0;

  cur_pulses_wr = (cur_pulses_wr+1) & 7; // next buffer 

  int pin = digitalRead(raw433_pin); // current pin state

  pulses_arr[cur_pulses_wr].initial_ook = pin;
  pulses_arr[cur_pulses_wr].num_pulses = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int decodeLaCrosse(bool initial_ook, uint16_t* pulses, int len);
static int decodeWT450H(bool initial_ook, uint16_t* pulses, int len);
static int decodeInfactory(bool initial_ook, uint16_t* pulses, int len);

// user context: scan tru pulse buffers
void checkPulses(void)
{
  // limit parsing to 10 frames (irq may be filling fast)

  for(int t = 0; t < 10; t++)
  {
    if(cur_pulses_wr == cur_pulses_rd)
      return;

    pulses_t& p = pulses_arr[cur_pulses_rd];

    if(debug_val & deb_pulses)
    {
      snprintf_P(log_data, sizeof(log_data), "[%i] pulses: num %u, start: %i", cur_pulses_rd, p.num_pulses, (int)p.initial_ook);
      AddLog(LOG_LEVEL_INFO);

      int pn = p.num_pulses;
      uint16_t* tp = p.pulse;

      while(pn > 0)
      {
        for(int i = 0; i < 32; i++)
        {
          snprintf_P(log_data + 5*i, sizeof(log_data)-5*i, "%4u ", *tp);
          ++tp;
          if(--pn <= 0)
            break;
        }

        AddLog(LOG_LEVEL_INFO);
      }
    }

    cur_pulses_rd = (cur_pulses_rd+1) & 7;

    // add your decoder here...

    if(decodeInfactory(p.initial_ook, p.pulse, p.num_pulses))
      continue;
    if(decodeLaCrosse(p.initial_ook, p.pulse, p.num_pulses))
      continue;
    if(decodeWT450H(p.initial_ook, p.pulse, p.num_pulses))
      continue;
  }
}

///////////////////////////////////////////////////////////////////////////////////

static struct 
{
  char  sensor[100];
  float temp = -1000;
  float hum = -1000;
}last_values;

// Output for tele/sensor and web
void TempHumShow(bool json, bool pass_on)
{
  if(*last_values.sensor == 0)
    return;

  char temperature[33];
  dtostrfd(last_values.temp, Settings.flag2.temperature_resolution, temperature);
  char humidity[33];
  dtostrfd(last_values.hum, Settings.flag2.temperature_resolution, humidity);

  if (json) 
  {
    ResponseAppend_P(PSTR(",\"%s\":{"), last_values.sensor);
    if(last_values.temp != -1000)
      ResponseAppend_P(PSTR("\"" D_JSON_TEMPERATURE "\":%s,\""  ), temperature);
	  
    if(last_values.hum != -1000)
      ResponseAppend_P(PSTR("\"" D_JSON_HUMIDITY "\":%s,\"" ),  humidity);
	  
    ResponseJsonEnd();
#ifdef USE_KNX
    if (pass_on) 
    {
      if(last_values.temp != -1000)
        KnxSensor(KNX_TEMPERATURE, last_values.temp);
	    
      if(last_values.hum != -1000)
        KnxSensor(KNX_HUMIDITY, last_values.hum);
    }
#endif  // USE_KNX
#ifdef USE_WEBSERVER
  } else 
  {
    if(last_values.temp != -1000)
      WSContentSend_PD(HTTP_SNS_TEMP, last_values.sensor, temperature, TempUnit());
	  
    if(last_values.hum != -1000)
      WSContentSend_PD(HTTP_SNS_HUM, last_values.sensor, humidity);
#endif  // USE_WEBSERVER
  }
}

///////////////////////////////////////////////////////////////////


// mqtt output:
// mqtt: /tele/<tasmota_device>/OOK/device-21 = {"Time":"2020-08-02T12:26:40","type":"my decoder","Temperature":27.0,"Humidity":53.0,"BatteryGood":1}
//                                  111111111                                         2222222222

// mqttStart("my decoder", "device-%i", device.addr) // prepare mqtt frame
//            2222222222    111111111
//  optional: mqttTemperature(27.0)                  // add temperature to mqtt frame
//  optional: mqttHumidity(53.0)
//  optional: mqttBatteryGood(true)
// mqttEnd()                                         // send mqtt frame

void mqttStart(const char* type, const char* sensor, ...)
{
  va_list args;
  va_start(args, sensor);

  vsnprintf_P(last_values.sensor, sizeof(last_values.sensor), sensor, args);
  va_end(args);

  ResponseTime_P(PSTR(",\"type\":\"%s\","), type);

#ifdef USE_CC1101
  if(use_cc1101)
    ResponseAppend_P(PSTR("\"Frequency\":%i.%03i,"  ), (int)cc1101_freq, (int)(cc1101_freq*1000) % 1000);
#endif

  last_values.temp = -1000;
  last_values.hum = -1000;
}

void mqttTemperature(float temp)
{
  char str[33];
  dtostrfd(temp, Settings.flag2.temperature_resolution, str);
  ResponseAppend_P(PSTR("\"" D_JSON_TEMPERATURE "\":%s,"  ), str);
  last_values.temp = temp;
}

void mqttHumidity(float hum)
{
  char str[33];
  dtostrfd(hum, Settings.flag2.temperature_resolution, str);
  ResponseAppend_P(PSTR("\"" D_JSON_HUMIDITY "\":%s,"  ), str);
  last_values.hum = hum;
}

void mqttBatteryGood(bool good)
{
  ResponseAppend_P(PSTR("\"BatteryGood\":%i,"  ), good ? 1 : 0);
}

void mqttEnd(void)
{
  int len = strlen(mqtt_data);
  if(len)
    mqtt_data[len-1] = 0; // get rid of last ','

  char prefix[150];
  sprintf(prefix, "OOK/%s", last_values.sensor);

  ResponseJsonEnd();
  MqttPublishPrefixTopic_P(TELE, prefix);
}
///////////////////////////////////////////////////////////////////

// CRC 4 table creation (nibble)
void createCRC4(uint8_t poly, uint8_t arr[16], uint8_t init)
{
  for(int t = 0; t < 16; t++)
  {
    int crc = init;
    int data = t;

    for(int t = 0; t < 4; t++)
		{
			crc <<= 1;
			data <<= 1;

			if((crc & 0x10) != (data & 0x10))
			{
				crc ^= poly;	
			}
		}
    arr[t] = crc;
  }
}

// crc4 NIBBLE (4 bit) calc, you must "crc &= 0x0f" yourself
// data is a 4 bit nibble!
void crc4(uint8_t poly[16], unsigned int data, unsigned int& crc)
{
  int ix = (crc ^ data) & 0xf;

  crc = poly[ix];
}

///////////////////////////////////////////////////////////////////

static struct
{
  int     num_data;
  uint8_t data[MAX_PULSES_NUM];
}ook_data;

///////////////////////////////////////////////////////////////////


static const char S_OOK_DEBUG1[]  PROGMEM = "pulses: %u, ook-bits: %u(min %u)";
//                                           0123456789abcdef0
static const char S_OOK_DEBUG2[]  PROGMEM = "|0              |16              |32              |48              |64              |80";

static void print_ook_debug(char* pr, int pr_len, int len, int min_bits)
{
  if(debug_val & deb_ook)
  {
    if(pr_len >= min_bits/2)
    {
      pr[pr_len] = 0;

      snprintf_P(log_data, sizeof(log_data), S_OOK_DEBUG1, len, ook_data.num_data, min_bits);
      AddLog(LOG_LEVEL_INFO);

      snprintf_P(log_data, sizeof(log_data), "%s", pr);
      AddLog(LOG_LEVEL_INFO);

      snprintf_P(log_data, sizeof(log_data), S_OOK_DEBUG2, pr);
      AddLog(LOG_LEVEL_INFO);
    }
  }
}

#define OOK_DEBUG_INIT char pr[MAX_PULSES_NUM*2+100]; int pr_len = 0


///////////////////////////////////////////////////////////////////

// OOK infactory
// _____                      _____                                          _____
//      |____________________|     |________________________________________|     |____
//|-500-|-------2000---------|-500-|----------------4000--------------------|-500-|----
//|-------------0------------|------------------------1---------------------|----------
// 0: short hi, 2000 low
// 1: short hi, 4000 low
static int decodeOOKI(int min_bits, uint16_t* pulses, int len, int min_hi, int max_hi, int min_0, int max_0, int min_1, int max_1)
{
  ook_data.num_data = 0;
  int s = 0;

  OOK_DEBUG_INIT;

  int ret = 0;

  for(int t = 0; t < len; t++)
  {
    int val = *pulses++;
    ++ret;

    if(s == 0)
    {
      if(val > min_hi && val < max_hi)
      {
     //   pr[pr_len++] = '>';
         s = 1;
      }
      else
      {
        pr[pr_len++] = '<';
        if(ook_data.num_data >= min_bits)
          break;

        pr_len = 0;
        ook_data.num_data = 0;
      }
    }
    else
    {
      if(val >= min_1 && val <= max_1)
      {
        pr[pr_len++] = '1';
        ook_data.data[ook_data.num_data++] = 1;
      }
      else if(val >= min_0 && val <= max_0)
      {
        pr[pr_len++] = '0';
        ook_data.data[ook_data.num_data++] = 0;      
      }
      else
      {
        pr[pr_len++] = 'X';
        if(ook_data.num_data >= min_bits)
          break;

        pr_len = 0;
        ook_data.num_data = 0;
        --t;
      }

      s = 0;
    }
  }

  print_ook_debug(pr, pr_len, len, min_bits);

  return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// OOK WT450H
// ______        ______        ________________                  __
//       |______|      |______|                |________________| 
//|-1000-|-1000-|-1000-|-1000-|------2000------|------2000------|
//|------1------|------1------|--------0-------|--------0-------|
// 0: long pulse (hi-time)
// 1: two short pulses (lo-time)
static int decodeOOK(int min_bits, uint16_t* pulses, int len, int min_1, int max_1, int min_0, int max_0)
{
  ook_data.num_data = 0;
  int s = 0;

  OOK_DEBUG_INIT;

  int ret = 0;

  for(int t = 0; t < len; t++)
  {
    int val = *pulses++;
    ++ret;

    if(val > min_0 && val < max_0)
    {
      pr[pr_len++] = '0';
      ook_data.data[ook_data.num_data++] = 0;
      s = 0;
    }
    else if(val > min_1 && val < max_1)
    {
      if(++s == 2)
      {
        pr[pr_len++] = '1';
        ook_data.data[ook_data.num_data++] = 1;
        s = 0;
      }
    }
    else
    {
      if(val >= max_0)
        pr[pr_len++] = s ? 'h' : 'H';
      else if(val <= min_1)
        pr[pr_len++] = s ? 'l' : 'L';
      else
        pr[pr_len++] = s ? 'm' : 'M';

      if(ook_data.num_data >= min_bits)
        break;

      pr_len = 0;
      ook_data.num_data = 0;
      s = 0;
    }
  }

  print_ook_debug(pr, pr_len, len, min_bits);

  return ret;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

// LaCrosse TX3: https://www.f6fbb.org/domo/sensors/tx3_th.php
// _____            ______________
//      |__________|              |__________|
//|-550-|---1000---|-----1400-----|---1000---|
//|-------1--------|------------0------------|
// 0: long/long 1400/1000
// 1: short/long 550/1000
static int decodeOOK2(int min_bits, uint16_t* pulses, int len, int min_1, int max_1, int break_min, int break_max, int min_0, int max_0)
{
  // increasing values:
  // eg min_1=400, max_1=700, break_min=800, break_max=1200, min_0=1300, max_0=1500

  ook_data.num_data = 0;
  int s = 0;

  OOK_DEBUG_INIT;

  int ret = 0;

  for(int t = 0; t < len; t++)
  {
    int val = *pulses++;
    ++ret;

    if(s == 0) // 1st bit-half
    {
      if(val >= min_1 && val <= max_1)
      {
        ook_data.data[ook_data.num_data] = 1;
        ++s;
      }
      else if(val >= min_0 && val <= max_0)
      {
        ook_data.data[ook_data.num_data] = 0;
        ++s;
      }
      else
      {
        pr[pr_len++] = 'X'; 
        if(ook_data.num_data >= min_bits)
          break;
        pr_len = 0;
        ook_data.num_data = 0;
      }
    }
    else // 2nd bit-half
    {
      if(val >= break_min)
      {
        pr[pr_len++] =  ook_data.data[ook_data.num_data] ? '1' : '0';
        ++ook_data.num_data;

        if(val >= break_max)
        {
          pr[pr_len++] = 'P';
          break;
        }
      }
      else
      {
        --t; // retry as 1st half
        pr[pr_len++] = 'Y';
        if(ook_data.num_data >= min_bits)
          break;
        pr_len = 0;
        ook_data.num_data = 0;
      }

      s = 0;
    }
  }

  print_ook_debug(pr, pr_len, len, min_bits);

  return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct 
{
  bool valid;
  unsigned long when;
  uint8_t type;
  uint8_t addr;
  float   value;
}lacrosse_tx3_t;


static int decodeLaCrosse(bool initial_ook, uint16_t* pulses, int len)
{
  /*
// LaCrosse TX3: https://www.f6fbb.org/domo/sensors/tx3_th.php
  44 bits

  4-bit blocks:
  0000 preamble
  1010 preamble
  xxxx type (0000: temp, 1110: humidity)
  xxxx addr
  xxxP addr + parity (v1+v2+v3)
  tttt v1
  xxxx v2
  cccc v3
  tttt copy of v1
  xxxx copy of v2
  crc1 (nibblesum & 0x0f)

  value = v1*10 + v2 + v3 / 10
  temp = value - 50
  hum = value
  */

  int ret = 0;

  while(len > 0)
  {
    int offset = decodeOOK2(43, pulses, len, 440, 640, 900, 1100, 1200, 1600);
    pulses += offset;
    len -= offset;

    if(ook_data.num_data < 43) 
      continue;

    lacrosse_tx3_t pdata;

    // ook_num = 44 + x
    // loop tru 44 bit-blocks: 0..43, 1..44, x..44+x
    for(int offs = 0; offs <= ook_data.num_data - 43; offs++)
    {
      uint8_t* pos = &ook_data.data[offs];

      // header = 1100

      int parity = 0, t, v;
      unsigned int val;

      #define READ(NUM) val = 0; for(t = 0; t < NUM; t++) {v = *pos++; val <<= 1; val |= v; parity ^= v; }

      pdata.valid = false;

      // 1st bit might get lost in the receiver...    
      READ(7);

      if(val != 0x05) // bin 00000101
      {
        if(val != 0x0a) // 00001010
        {
          if(debug_val & deb_ookval)
          {
            snprintf_P(log_data, sizeof(log_data), "LaCrosse TX3: pos0 != 00001010 (%u%u%u%u(%x)", (val&8)>0, (val&4)>0, (val&2)>0, (val&1)>0, val);
            AddLog(LOG_LEVEL_INFO);
          }
          continue;
        }
        offs += 7;
      }
      else
      {
        val <<= 1; val |= *pos++; 
        if(val != 0x0a) // 00001010
        {
          if(debug_val & deb_ookval)
          {
            snprintf_P(log_data, sizeof(log_data), "LaCrosse TX3: pos0 != 00001010 (%u%u%u%u(%x)", (val&8)>0, (val&4)>0, (val&2)>0, (val&1)>0, val);
            AddLog(LOG_LEVEL_INFO);
          }
          continue;
        }
        offs += 8;
      }

      // pos 8
      unsigned int chk = val; // 0x0a

      READ(4);
      chk += val;

      if(val == 0) // temperature
      {
      }
      else if(val == 0x0e) // humidity
      {
      }
      else
      {
        offs += 12;

        if(debug_val & deb_ookval)
        {
          snprintf_P(log_data, sizeof(log_data), "LaCrosse TX3: nib 3 unknown type(%x)", (val&8)>0, (val&4)>0, (val&2)>0, (val&1)>0, val);
          AddLog(LOG_LEVEL_INFO);
        }
        continue;
      }

      pdata.type = val;

      // pos 12
      READ(4); chk += val;
      pdata.addr = val << 3;

      // pos 16
      READ(4); chk += val;
      pdata.addr |= val >> 1;
      
      int p1 = val & 1;

      // pos 20
      parity = 0;
      READ(4); chk += val;
      unsigned int m = val * 10;

      // pos 24
      READ(4); chk += val;
      m += val;
      pdata.value = m;

      // pos 28
      READ(4); chk += val;
      pdata.value += (float)val / 10;

      if(p1 == parity)
      {
        pdata.valid = true;
      }
      else
      {
        if(debug_val & deb_ookval)
        {
          snprintf_P(log_data, sizeof(log_data), "LaCrosse TX3: parity mismatch");
          AddLog(LOG_LEVEL_INFO);
        }
        continue;
      }
      
      // pos 32
      READ(4); chk += val;
      unsigned int m2 = val * 10;

      // pos 36
      READ(4); chk += val;
      m2 += val;

      if(m != m2)
      {
        if(debug_val & deb_ookval)
        {
          snprintf_P(log_data, sizeof(log_data), "LaCrosse TX3: val1 != val2 (%u, %u)", m, m2);
          AddLog(LOG_LEVEL_INFO);
        }
        continue;
      }

      if(pdata.type == 0) // temp
        pdata.value -= 50;
      
      // pos 40
      READ(4);
      chk &= 0xf;

      if(val != chk)
      {
        if(debug_val & deb_ookval)
        {
          snprintf_P(log_data, sizeof(log_data), "LaCrosse TX3: !chk %02x != %02x", val, chk);
          AddLog(LOG_LEVEL_INFO);
        }
        continue;
      }
      else
      {
        pdata.valid = true;
        pdata.when = micros();
      }

      offs += 43-8;

      const char* type = "Temperature";

      mqttStart(PSTR("LaCrosse TX3"), PSTR("LaCrosse-TX3-%u"), pdata.addr);
      
      if(pdata.type)
      {
        type = "Humidity";
        mqttHumidity(pdata.value);
        //humMqtt((int)pdata.value, PSTR("LaCrosse-TX3-%u"), pdata.addr);
      }
      else
      {
        //tempMqtt(pdata.value, PSTR("LaCrosse-TX3-%u"), pdata.addr);  
        mqttTemperature(pdata.value);
      }

      mqttEnd();

      snprintf_P(log_data, sizeof(log_data), "LaCrosse TX3: type:%u addr:%u %s:%i.%i", pdata.type, pdata.addr, type, (int)pdata.value, (int)(pdata.value*10)%10);
      AddLog(LOG_LEVEL_INFO);

      ++ret;
      break;
    }
  }

  return ret;

  #undef READ
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct xsns_100_rawOOK
{
  bool valid;
  uint8_t id;
  uint8_t channel;
  bool  batt_low;
  float temp;
  float hum;

}infactory_t;


static int decodeInfactory(bool initial_ook, uint16_t* pulses, int len)
{
  /*
  Thanks to rtl_433 
  40bits
  iiii iiii | cccc ub?? | tttt tttt | tttt hhhh | hhhh ??nn
- i: identification // changes on battery switch
- c: CRC-4 // CCITT checksum BUT: cccc is replaced by ??nn; crc = crc4(nibble[0]..nibble[7], crc ^= nibble[8]
- u: unknown // (sometimes set at power-on, but not always)
- b: battery low // flag to indicate low battery voltage
- h: Humidity // BCD-encoded, each nibble is one digit, 'A0' means 100%rH
- t: Temperature // in °F as binary number with one decimal place + 90 °F offset
- n: Channel // Channel number 1 - 3

  */
  static uint8_t poly[16];
  if(poly[1] == 0)
  {
    createCRC4(0x03, poly, 0);
  }

  infactory_t pdata;
 
  int ret = 0;

  while(len > 0)
  {
    int offset = decodeOOKI(40, pulses, len, 400, 800, 1400, 2600, 3400, 4600);
    pulses += offset;
    len -= offset;

    if(ook_data.num_data < 40) 
      continue;

    // ook_num = 40 + x
    // loop tru 40 bit-blocks: 0..39, 1..40, 2..41, x..40+x
  
    for(int offs = 0; offs <= ook_data.num_data - 40; offs++)
    {
      uint8_t* pos = &ook_data.data[offs];

      int t, v;
      unsigned int val, crc = 0;

      uint8_t n[10];

      for(t = 0; t < 10; t++)
      {
        val = 0; 
        for(v = 0; v < 4; v++) 
        {
          val <<= 1; 
          val |= *pos++; 
        } 
        n[t] = val;
      }

      pdata.valid = false;

      unsigned int crc1 = n[2];
      n[2] = n[9]; // cccc = ??nn

      for(t = 0; t < 8; t++)
        crc4(poly, n[t], crc);

      crc ^= n[8];
      crc &= 0xf;

      if(crc1 == crc)
      {
        pdata.valid = true;

        pdata.id = n[0] << 4 + n[1];

        pdata.batt_low = (n[3] & 4) != 0;

        pdata.temp = (n[4] << 8) + (n[5] << 4) + n[6];
        pdata.temp /= 10;
        pdata.temp -= 90; // Fahrenheit
        pdata.temp = (pdata.temp - 32) / 1.8;

        pdata.hum = n[7] * 10 + n[8];

        pdata.channel = n[9] & 3;
      }
      else
      {
        pdata.valid = false;
      }
    }

    if(pdata.valid)
    {
      snprintf_P(log_data, sizeof(log_data), "Infactory: id:%u channel:%u humidity:%i temperature:%i.%i", pdata.id, pdata.channel, (int)pdata.hum, (int)pdata.temp, (int)(pdata.temp*10)%10);
      AddLog(LOG_LEVEL_INFO);

      mqttStart(PSTR("Infactory"), PSTR("Infactory-%u-%u"), pdata.id, pdata.channel);
      mqttTemperature(pdata.temp);
      mqttHumidity(pdata.hum);
      mqttBatteryGood(!pdata.batt_low);
      mqttEnd();

      ++ret;
      break;
    }
  }
  return ret;

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct
{
  bool valid;
  unsigned long when;
  uint8_t house;
  uint8_t channel;
  bool battery_weak;
  float hum;
  float temp;
}pdata_t;

// 1: pulse 800..1200, 0: 1800..2200
static int decodeWT450H(bool initial_ook, uint16_t* pulses, int len)
{
  // Thanks to contribution of Johan Adler and Øyvind Kaurstad. 
  // http://ala-paavola.fi/jaakko/doku.php?id=wt450h
/*
  b00 - b03  (4 bits): Constant, 1100, probably preamble
  b04 - b07  (4 bits): House code 
  b08 - b09  (2 bits): Channel code - 1 
  b10 - b12  (3 bits): Constant, 110 battery ok, 111 battery weak
  b13 - b19  (7 bits): Relative humidity (here 0111011 = 59 %)
  b20 - b34 (15 bits): Temperature fixpoint 8.7 (temp + 50.0) * 128)
  b35 - b35  (1 bit) : Parity (xor of all bits should give 0)
*/

  int ret = 0;

  while(len > 0)
  {
    int offset = decodeOOK(35,  pulses, len, 800, 1200, 1800, 2200);
    pulses += offset;
    len -= offset;

    if(ook_data.num_data < 35)
      continue;
  
    pdata_t pdata;

    // ook_num = 36 + x
    // loop tru 36 bit-blocks: 0..35, 1..36, x..36+x

    for(int offs = 0; offs <= ook_data.num_data - 35; offs++)
    {
   // snprintf_P(log_data, sizeof(log_data), "WT450H: offs %i (%i)", offs, num_bits);
   // AddLog(LOG_LEVEL_INFO);

      uint8_t* pos = &ook_data.data[offs];

      // header = 1100

    // 1st bit might get lost in the receiver...    

      unsigned int val = *pos++; 
      val <<= 1; val |= *pos++; 
      val <<= 1; val |= *pos++; 

      if(val != 6) // bin 110
      {
        if(val != 4) // 100
        {
          if(debug_val & deb_ookval)
          {
            snprintf_P(log_data, sizeof(log_data), "WT450H: pos0 != 100 (%u%u%u%u(%x)", (val&8)>0, (val&4)>0, (val&2)>0, (val&1)>0, val);
            AddLog(LOG_LEVEL_INFO);
          }
          continue;
        }
        offs += 3;
      }
      else
      {
        val <<= 1; val |= *pos++; 
        if(val != 12) // 1100
        {
          if(debug_val & deb_ookval)
          {
            snprintf_P(log_data, sizeof(log_data), "WT450H: pos0 != 1100 (%u%u%u%u(%x)", (val&8)>0, (val&4)>0, (val&2)>0, (val&1)>0, val);
            AddLog(LOG_LEVEL_INFO);
          }
          continue;
        }
        offs += 4;
      }
      
      // header offs 10 = 110
      val = pos[6]; 
      val <<= 1; val |= pos[7]; 
      val <<= 1; val |= pos[8]; 

  //   snprintf_P(log_data, sizeof(log_data), "WT450H: header2: %u%u%u%u(%x)", (val&8)>0, (val&4)>0, (val&2)>0, (val&1)>0, val);
  //   AddLog(LOG_LEVEL_INFO);

      if(val < 6) // 110 or 111
      {
        if(debug_val & deb_ookval)
        {
          snprintf_P(log_data, sizeof(log_data), "WT450H: pos10 != 110 (%u%u%u%u(%x)", (val&8)>0, (val&4)>0, (val&2)>0, (val&1)>0, val);
          AddLog(LOG_LEVEL_INFO);
        }
        continue;
      }

      pdata.battery_weak = val & 1;

      int parity = 0, t, v;

      #define READ(NUM) val = 0; for(t = 0; t < NUM; t++) {v = *pos++; val <<= 1; val |= v; parity ^= v; }

      // 4 bit "house"
      READ(4)
      pdata.house = val;

      // 2 bit "channel"
      READ(2)
      pdata.channel = val;

      pos += 3; // header offs 10

      // 7 bit "humidity"
      READ(7)
      pdata.hum = val;

      // 15 bit "temperature"
      READ(15)

      // is fract 8.7 -> (temp + 50.0) * 128

      pdata.temp = (float)val / 128;
      pdata.temp -= 50;
  /*    
      // wanting xx.y
      val -= 6400;
      pdata.temp_fract = val * 10;
      pdata.temp_fix = val >> 7;
      
      pdata.temp_fract >>= 7;
      pdata.temp_fract %= 10; // 8.7 -> ,x
  */

      if(parity != *pos)
      {
        if(debug_val & deb_ookval)
        {
          snprintf_P(log_data, sizeof(log_data), "WT450H: parity mismatch");
          AddLog(LOG_LEVEL_INFO);
        }

        offs += 12-4;
      }
      else
      {
        pdata.valid = true;
        pdata.when = micros();
        
        snprintf_P(log_data, sizeof(log_data), "WT450H: house:%u channel:%u battery_weak:%i humidity:%i temperature:%i.%i", pdata.house, pdata.channel, (int)pdata.battery_weak, (int)pdata.hum, (int)pdata.temp, (int)(pdata.temp*10)%10);
        AddLog(LOG_LEVEL_INFO);

        mqttStart(PSTR("WT450H"), PSTR("WT450H-%u-%u"), pdata.house, pdata.channel);
        mqttTemperature(pdata.temp);
        mqttHumidity(pdata.hum);
        mqttBatteryGood(!pdata.battery_weak);
        mqttEnd();

        ++ret;
        break;
      }
    }
  }

  return ret;

  #undef READ
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns100(uint8_t function)
{
  if(raw433_pin >= 99)
    return false;

  bool result = false;

  switch (function) 
  {
    case FUNC_EVERY_SECOND:
      checkPulses();
    break;

    case FUNC_JSON_APPEND:
      TempHumShow(true, true);
    break;

#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      TempHumShow(false, false);
    break;
#endif  // USE_WEBSERVER
    case FUNC_INIT:
      raw433Init();
    break;

    case FUNC_COMMAND_SENSOR:
    {
      if (XSNS_100 == XdrvMailbox.index) 
      {
        result = ook_Command();
      }
    }
    break;
  }

  return result;
}

#endif // #ifdef USE_433_RAW
