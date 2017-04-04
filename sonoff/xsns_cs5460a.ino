/*
Copyright (c) 2016 Theo Arends.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef USE_CS5460A
/*********************************************************************************************\
 * CS5460A - Power Meter
 *
 * Source: Victor Ferrer https://github.com/vicfergar/Sonoff-MQTT-OTA-Arduino
\*********************************************************************************************/

#include <RingBuf.h>

#define BITS_PER_REGISTER 32            // Number of bits of read cycle
#define INTER_FRAME_MS 35		            // Goal to achieve about 35ms timer interval
#define BUFSIZE 10                      // Grab last 10 frames

#define VOLTAGE_RANGE 0.250					    // Full scale V channel voltage
#define CURRENT_RANGE 0.050					    // Full scale I channel voltage (PGA 50x instead of 10x)
#define VOLTAGE_DIVIDER 450220.0/220.0	// Input voltage channel divider (R1+R2/R2)
#define CURRENT_SHUNT 620					      // Empirically obtained multiplier to scale Vshunt drop to I    
#define FLOAT24 16777216.0					    // 2^24 (converts to float24)
#define POWER_MULTIPLIER 1 / 512.0			// Energy->Power divider; not sure why, but seems correct. Datasheet not clear about this.

#define VOLTAGE_MULTIPLIER   (float)  (1 / FLOAT24 * VOLTAGE_RANGE * VOLTAGE_DIVIDER)
#define CURRENT_MULTIPLIER   (float)  (1 / FLOAT24 * CURRENT_RANGE * CURRENT_SHUNT)

typedef enum
{
   WAIT_SYNC = 0,
   WAIT_READY,
   READ_VRMS,
   READ_IRMS,
   READ_E
} CSReadStates;

typedef struct
{
  uint32_t voltage;
  uint32_t current;
  uint32_t energy;
  uint32_t dt;
} CSRawFrame;

float cs_voltage = 0;
float cs_current = 0;
float cs_truePower = 0;
float cs_powerFactor = 0;
int cs_lostDataCount = 0;
int cs_fullBufferWarnings = 0;

unsigned long cs_kWhtoday;

int _clkPin;
int _sdoPin;
uint32_t _sniffRegister = 0;
uint8_t _bitsForNextData;
CSReadStates _readState = WAIT_SYNC;
CSRawFrame _sniffFrame;
unsigned long _lastClockTime = 0;
unsigned long _lastDataFrameTime = 0;
RingBuf *_dataBuf = RingBuf_new(sizeof(CSRawFrame), BUFSIZE);

#ifndef USE_WS2812_DMA  // Collides with Neopixelbus but solves exception
void clockISR() ICACHE_RAM_ATTR;
void timerOVF() ICACHE_RAM_ATTR;
#endif  // USE_WS2812_DMA

void cs5460_initialize(int clkPin, int sdoPin)
{
	_clkPin = clkPin;
	_sdoPin = sdoPin;

  // Set the CLK-pin and MISO-pin as inputs
  pinMode(_clkPin, INPUT);
  pinMode(_sdoPin, INPUT);
  
	// Setting up interrupt ISR, trigger function "clockISR()" when INT0 (CLK) is rising
	attachInterrupt(digitalPinToInterrupt(_clkPin), clockISR, RISING);
}

void clockISR()
{
   unsigned long currentClockTime = millis();
  
	 uint32_t elapsed = currentClockTime - _lastClockTime;
   _lastClockTime = currentClockTime;

  if (elapsed > INTER_FRAME_MS && _readState != READ_E) { // Read E register can take a while for high consumption
    if (_readState != WAIT_SYNC) cs_lostDataCount++;
    
    // Went ~35ms without a clock pulse, probably in an inter-frame period; reset sync
    _readState = WAIT_READY;
    _bitsForNextData = 2 * BITS_PER_REGISTER; // Next valid register comes after 2 read cycles
  }
  else if (_readState == WAIT_SYNC) {
    return;
  }

	// Shift one bit to left and store MISO-value (0 or 1)
	_sniffRegister = (_sniffRegister << 1) | digitalRead(_sdoPin);
  _bitsForNextData--;

	if (_bitsForNextData == 0) {
		if (_readState == WAIT_READY) {
			// Check Status register is has conversion ready ( DRDY=1, ID=15 )
			_readState = (_sniffRegister & 0x00800000) ? READ_VRMS : WAIT_READY;
			_bitsForNextData = 2 * BITS_PER_REGISTER; // Next valid register comes after 2 read cycles
		}
    else if (_readState == READ_VRMS){
      _readState = READ_IRMS;
      _bitsForNextData = BITS_PER_REGISTER; // Next cycle
      _sniffFrame.voltage = _sniffRegister;
    }
    else if (_readState == READ_IRMS){
      _readState = READ_E;
      _bitsForNextData = BITS_PER_REGISTER; // Next cycle
      _sniffFrame.current = _sniffRegister;
    }
    else if (_readState == READ_E){
      _readState = WAIT_SYNC;
      _sniffFrame.energy = _sniffRegister;
      _sniffFrame.dt = currentClockTime - _lastDataFrameTime;
      _lastDataFrameTime = currentClockTime;

      // All useful data read. Save in data buffer
      if (_dataBuf->add(_dataBuf, &_sniffFrame) < 0)
      {
        cs_fullBufferWarnings++;
      }
    }
	}
}

void cs_finish()
{
  // Detach all interruptions
  detachInterrupt(digitalPinToInterrupt(_clkPin));
}

void cs_loop()
{
  CSRawFrame readFrame;

	if (_dataBuf->pull(_dataBuf, &readFrame))
	{
		// read Vrms register
		cs_voltage = readFrame.voltage * VOLTAGE_MULTIPLIER;

		// read Irms register
		cs_current = readFrame.current * CURRENT_MULTIPLIER;

		// read E (energy) register
		if (readFrame.energy & 0x800000) {
			// must sign extend int24 -> int32LE
			readFrame.energy |= 0xFF000000;
		}
		cs_truePower = ((int32_t)readFrame.energy) * POWER_MULTIPLIER;
    cs_kWhtoday += (cs_truePower * readFrame.dt) / 36;

		float apparent_power = cs_voltage * cs_current;
		cs_powerFactor = cs_truePower / apparent_power;
	}
}












Ticker tickerCS;

byte cs_startup;

void cs_init()
{
  cs_kWhtoday = 0;
  cs_startup = 1;

  cs5460_initialize(pin[GPIO_CS_CLK], pin[GPIO_CS_SDO]);
  
  tickerCS.attach_ms(1000, cs_1s);
}

void cs_1s()
{
  if (rtc_loctime() == rtc_midnight()) {
    sysCfg.hlw_kWhyesterday = cs_kWhtoday;
    cs_kWhtoday = 0;
  }
  
  if (cs_startup && rtcTime.Valid && (rtcTime.DayOfYear == sysCfg.hlw_kWhdoy)) {
    cs_kWhtoday = sysCfg.hlw_kWhtoday;
    cs_startup = 0;
  }
}

void hlw_savestate()
{
  sysCfg.hlw_kWhdoy = (rtcTime.Valid) ? rtcTime.DayOfYear : 0;
  sysCfg.hlw_kWhtoday = cs_kWhtoday;
}
/********************************************************************************************/

boolean hlw_margin(byte type, uint16_t margin, uint16_t value, byte &flag, byte &saveflag)
{
  byte change;

  if (!margin) return false;
  change = saveflag;
  if (type) {
    flag = (value > margin);
  } else {
    flag = (value < margin);
  }
  saveflag = flag;
  return (change != saveflag);
}

void hlw_setPowerSteadyCounter(byte value)
{
}

void hlw_margin_chk()
{
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

boolean hlw_command(char *type, uint16_t index, char *dataBuf, uint16_t data_len, int16_t payload, char *svalue, uint16_t ssvalue)
{
  boolean serviced = true; 

  if (!strcmp(type,"POWERLOW")) {
    if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
      sysCfg.hlw_pmin = payload;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"PowerLow\":\"%d%s\"}"), sysCfg.hlw_pmin, (sysCfg.value_units) ? " W" : "");
  }
  else if (!strcmp(type,"POWERHIGH")) {
    if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
      sysCfg.hlw_pmax = payload;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"PowerHigh\":\"%d%s\"}"), sysCfg.hlw_pmax, (sysCfg.value_units) ? " W" : "");
  }
  else if (!strcmp(type,"VOLTAGELOW")) {
    if ((data_len > 0) && (payload >= 0) && (payload < 501)) {
      sysCfg.hlw_umin = payload;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"VoltageLow\":\"%d%s\"}"), sysCfg.hlw_umin, (sysCfg.value_units) ? " V" : "");
  }
  else if (!strcmp(type,"VOLTAGEHIGH")) {
    if ((data_len > 0) && (payload >= 0) && (payload < 501)) {
      sysCfg.hlw_umax = payload;
    }
    snprintf_P(svalue, ssvalue, PSTR("[\"VoltageHigh\":\"%d%s\"}"), sysCfg.hlw_umax, (sysCfg.value_units) ? " V" : "");
  }
  else if (!strcmp(type,"CURRENTLOW")) {
    if ((data_len > 0) && (payload >= 0) && (payload < 16001)) {
      sysCfg.hlw_imin = payload;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"CurrentLow\":\"%d%s\"}"), sysCfg.hlw_imin, (sysCfg.value_units) ? " mA" : "");
  }
  else if (!strcmp(type,"CURRENTHIGH")) {
    if ((data_len > 0) && (payload >= 0) && (payload < 16001)) {
      sysCfg.hlw_imax = payload;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"CurrentHigh\":\"%d%s\"}"), sysCfg.hlw_imax, (sysCfg.value_units) ? " mA" : "");
  }
  else if (!strcmp(type,"HLWPCAL")) {
    if ((data_len > 0) && (payload > 0) && (payload < 32001)) {
      sysCfg.hlw_pcal = (payload > 9999) ? payload : HLW_PREF_PULSE;  // 12530
    }
    snprintf_P(svalue, ssvalue, PSTR("(\"HlwPcal\":\"%d%s\"}"), sysCfg.hlw_pcal, (sysCfg.value_units) ? " uS" : "");
  }
  else if (!strcmp(type,"HLWUCAL")) {
    if ((data_len > 0) && (payload > 0) && (payload < 32001)) {
      sysCfg.hlw_ucal = (payload > 999) ? payload : HLW_UREF_PULSE;  // 1950
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"HlwUcal\":\"%d%s\"}"), sysCfg.hlw_ucal, (sysCfg.value_units) ? " uS" : "");
  }
  else if (!strcmp(type,"HLWICAL")) {
    if ((data_len > 0) && (payload > 0) && (payload < 32001)) {
      sysCfg.hlw_ical = (payload > 2499) ? payload : HLW_IREF_PULSE;  // 3500
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"HlwIcal\":\"%d%s\"}"), sysCfg.hlw_ical, (sysCfg.value_units) ? " uS" : "");
  }
  else {
    serviced = false;
  }
  return serviced;
}

void hlw_commands(char *svalue, uint16_t ssvalue)
{
  snprintf_P(svalue, ssvalue, PSTR("{\"Commands5\":\"PowerLow, PowerHigh, VoltageLow, VoltageHigh, CurrentLow, CurrentHigh, HlwPcal, HlwUcal, HlwIcal\"}"));
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void hlw_mqttStat(byte option, char* svalue, uint16_t ssvalue)
{
  char stemp0[10], stemp1[10], stemp2[10], stemp3[10], stemp4[10], stemp5[10], speriod[20];

  dtostrf((float)sysCfg.hlw_kWhyesterday / 100000000, 1, ENERGY_RESOLUTION &7, stemp0);
  dtostrf((float)cs_kWhtoday / 100000000, 1, ENERGY_RESOLUTION &7, stemp1);
  dtostrf(cs_truePower, 1, 1, stemp2);
  dtostrf(cs_powerFactor, 1, 2, stemp3);
  dtostrf(cs_voltage, 1, 1, stemp4);
  dtostrf(cs_current, 1, 3, stemp5);
  snprintf_P(speriod, sizeof(speriod), PSTR(", \"Period\":%d"), 0);
  snprintf_P(svalue, ssvalue, PSTR("%s\"Yesterday\":%s, \"Today\":%s%s, \"Power\":%d, \"Factor\":%s, \"Voltage\":%d, \"Current\":%s}"),
    svalue, stemp0, stemp1, (option) ? speriod : "", stemp2, stemp3, stemp4, stemp5);
}

void hlw_mqttPresent()
{
// {"Time":"2017-03-04T13:37:24", "Yesterday":0.013, "Today":0.000, "Period":0, "Power":0, "Factor":0.00, "Voltage":0, "Current":0.000}
  char svalue[200];  // was MESSZ

  snprintf_P(svalue, sizeof(svalue), PSTR("{\"Time\":\"%s\", "), getDateTime().c_str());
  hlw_mqttStat(1, svalue, sizeof(svalue));

//  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/ENERGY"), sysCfg.mqtt_prefix[2], sysCfg.mqtt_topic);
//  mqtt_publish(stopic, svalue);

  mqtt_publish_topic_P(1, PSTR("ENERGY"), svalue);
}

void hlw_mqttStatus(char* svalue, uint16_t ssvalue)
{
  snprintf_P(svalue, ssvalue, PSTR("{\"StatusPWR\":{"));
  hlw_mqttStat(0, svalue, ssvalue);
  snprintf_P(svalue, ssvalue, PSTR("%s}"), svalue);
}

#ifdef USE_WEBSERVER
const char HTTP_ENERGY_SNS[] PROGMEM =
  "<tr><th>Voltage</th><td>%s V</td></tr>"
  "<tr><th>Current</th><td>%s A</td></tr>"
  "<tr><th>Power</th><td>%s W</td></tr>"
  "<tr><th>Power Factor</th><td>%s</td></tr>"
  "<tr><th>Energy Today</th><td>%s kWh</td></tr>"
  "<tr><th>Energy Yesterday</th><td>%s kWh</td></tr>"
  "<tr><th>Lost data</th><td>%d</td></tr>";

String hlw_webPresent()
{
  String page = "";
  char stemp[10], stemp2[10], stemp3[10], stemp4[10], stemp5[10], stemp6[10], sensor[350];
  
  dtostrf(cs_voltage, 1, 1, stemp);
  dtostrf(cs_current, 1, 3, stemp2);
  dtostrf(cs_truePower, 1, 1, stemp3);
  dtostrf(cs_powerFactor, 1, 2, stemp4);
  dtostrf((float)cs_kWhtoday / 100000000, 1, ENERGY_RESOLUTION &7, stemp5);
  dtostrf((float)sysCfg.hlw_kWhyesterday / 100000000, 1, ENERGY_RESOLUTION &7, stemp6);
  snprintf_P(sensor, sizeof(sensor), HTTP_ENERGY_SNS, stemp, stemp2, stemp3, stemp4, stemp5, stemp6, cs_lostDataCount);
  page += sensor;
  return page;
}
#endif  // USE_WEBSERVER
#endif  // USE_CS5460A
