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
#define READ_E_LIMIT_MS 400             // Limit of time to wait for E register reading. Empirically obtained for ~16A
#define STATUS_RDY_MASK 0x00800001      // DRDY=1 (Data Ready) !IC=1 (Invalid Command. Normally logic 1)
#define BUFSIZE 5                       // Grab last 5 frames

#define VOLTAGE_RANGE 0.250					    // Full scale V channel voltage
#define CURRENT_RANGE 0.050					    // Full scale I channel voltage (PGA 50x instead of 10x)
#define VOLTAGE_DIVIDER 450220.0/220.0	// Input voltage channel divider (R1+R2/R2)
#define CURRENT_SHUNT 620					      // Empirically obtained multiplier to scale Vshunt drop to I    
#define FLOAT24 16777216.0					    // 2^24 (converts to float24)
#define POWER_MULTIPLIER 1 / 512.0			// Energy->Power divider; not sure why, but seems correct. Datasheet not clear about this.

#define VOLTAGE_MULTIPLIER   (float)  (1 / FLOAT24 * VOLTAGE_RANGE * VOLTAGE_DIVIDER)
#define CURRENT_MULTIPLIER   (float)  (1 / FLOAT24 * CURRENT_RANGE * CURRENT_SHUNT)

#define MILLIS_TO_HOUR       (float)  (1 / 1000 * 3600)

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
float cs_period = 0;
int cs_lostDataCount = 0;
int cs_fullBufferWarnings = 0;

unsigned long cs_kWhtoday; // W * 10^-5 (deca micro Watt)

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

  if ((elapsed > INTER_FRAME_MS && _readState != READ_E) || elapsed > READ_E_LIMIT_MS) { // Read E register can take a while for high consumption
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
			// Check Status register is has conversion ready
			_readState = (_sniffRegister & STATUS_RDY_MASK) == STATUS_RDY_MASK ? READ_VRMS : WAIT_READY;
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

void cs5460_loop()
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
    cs_period = cs_truePower * readFrame.dt * MILLIS_TO_HOUR;
    cs_kWhtoday += (cs_truePower * readFrame.dt) / 36;

    //char log[LOGSZ];
    //snprintf_P(log, sizeof(log), PSTR("READ: kwh %d vrms %d irms %d e %d dt %d stat %d"), cs_kWhtoday, readFrame.voltage, readFrame.current, readFrame.energy, readFrame.dt, readFrame.stat);
    //addLog(LOG_LEVEL_DEBUG, log);

		float apparent_power = cs_voltage * cs_current;
		cs_powerFactor = cs_truePower / apparent_power;
	}
}












Ticker tickerCS;

byte hlw_pminflg = 0;
byte hlw_pmaxflg = 0;
byte hlw_uminflg = 0;
byte hlw_umaxflg = 0;
byte hlw_iminflg = 0;
byte hlw_imaxflg = 0;

byte cs_startup;
byte power_steady_cntr;

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

  cs5460_loop();
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
  power_steady_cntr = value;
}

void hlw_margin_chk()
{
  char log[LOGSZ], svalue[200];  // was MESSZ
  uint16_t uped, piv;
  boolean flag, jsonflg;

  if (power_steady_cntr) {
    power_steady_cntr--;
    return;
  }

  if (power && (sysCfg.hlw_pmin || sysCfg.hlw_pmax || sysCfg.hlw_umin || sysCfg.hlw_umax || sysCfg.hlw_imin || sysCfg.hlw_imax)) {
    piv = (uint16_t)(cs_current * 1000);

//    snprintf_P(log, sizeof(log), PSTR("HLW: W %d, U %d, I %d"), pw, pu, piv);
//    addLog(LOG_LEVEL_DEBUG, log);

    snprintf_P(svalue, sizeof(svalue), PSTR("{"));
    jsonflg = 0;
    if (hlw_margin(0, sysCfg.hlw_pmin, cs_truePower, flag, hlw_pminflg)) {
      snprintf_P(svalue, sizeof(svalue), PSTR("%s%s\"PowerLow\":\"%s\""), svalue, (jsonflg)?", ":"", getStateText(flag));
      jsonflg = 1;
    }
    if (hlw_margin(1, sysCfg.hlw_pmax, cs_truePower, flag, hlw_pmaxflg)) {
      snprintf_P(svalue, sizeof(svalue), PSTR("%s%s\"PowerHigh\":\"%s\""), svalue, (jsonflg)?", ":"", getStateText(flag));
      jsonflg = 1;
    }
    if (hlw_margin(0, sysCfg.hlw_umin, cs_voltage, flag, hlw_uminflg)) {
      snprintf_P(svalue, sizeof(svalue), PSTR("%s%s\"VoltageLow\":\"%s\""), svalue, (jsonflg)?", ":"", getStateText(flag));
      jsonflg = 1;
    }
    if (hlw_margin(1, sysCfg.hlw_umax, cs_truePower, flag, hlw_umaxflg)) {
      snprintf_P(svalue, sizeof(svalue), PSTR("%s%s\"VoltageHigh\":\"%s\""), svalue, (jsonflg)?", ":"", getStateText(flag));
      jsonflg = 1;
    }
    if (hlw_margin(0, sysCfg.hlw_imin, piv, flag, hlw_iminflg)) {
      snprintf_P(svalue, sizeof(svalue), PSTR("%s%s\"CurrentLow\":\"%s\""), svalue, (jsonflg)?", ":"", getStateText(flag));
      jsonflg = 1;
    }
    if (hlw_margin(1, sysCfg.hlw_imax, piv, flag, hlw_imaxflg)) {
      snprintf_P(svalue, sizeof(svalue), PSTR("%s%s\"CurrentHigh\":\"%s\""), svalue, (jsonflg)?", ":"", getStateText(flag));
      jsonflg = 1;
    }
    if (jsonflg) {
      snprintf_P(svalue, sizeof(svalue), PSTR("%s}"), svalue);
      mqtt_publish_topic_P(1, PSTR("MARGINS"), svalue);
    }
  }
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
  else if (!strcmp(type,"RESETKWH")) {
    cs_kWhtoday = 0;
    snprintf_P(svalue, ssvalue, PSTR("{\"ResetKWh\":\"Done\"}"));
  }
  else {
    serviced = false;
  }
  return serviced;
}

void hlw_commands(char *svalue, uint16_t ssvalue)
{
  snprintf_P(svalue, ssvalue, PSTR("{\"Commands5\":\"PowerLow, PowerHigh, VoltageLow, VoltageHigh, CurrentLow, CurrentHigh, ResetKWh\"}"));
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void hlw_mqttStat(byte option, char* svalue, uint16_t ssvalue)
{
  char sKWHY[10], sKWHT[10], sTruePower[10], sPowerFactor[10], sVoltage[10], sCurrent[10], sPeriod1[10], sPeriod2[20];

  dtostrf((float)sysCfg.hlw_kWhyesterday / 100000000, 1, ENERGY_RESOLUTION &7, sKWHY);
  dtostrf((float)cs_kWhtoday / 100000000, 1, ENERGY_RESOLUTION &7, sKWHT);
  dtostrf(cs_truePower, 1, 1, sTruePower);
  dtostrf(cs_powerFactor, 1, 2, sPowerFactor);
  dtostrf(cs_voltage, 1, 1, sVoltage);
  dtostrf(cs_current, 1, 3, sCurrent);
  dtostrf(cs_period, 1, 3, sPeriod1);
  snprintf_P(sPeriod2, sizeof(sPeriod2), PSTR(", \"Period\":%s"), sPeriod1);
  snprintf_P(svalue, ssvalue, PSTR("%s\"Yesterday\":%s, \"Today\":%s%s, \"Power\":%s, \"Factor\":%s, \"Voltage\":%s, \"Current\":%s}"),
    svalue, sKWHY, sKWHT, (option) ? sPeriod2 : "", sTruePower, sPowerFactor, sVoltage, sCurrent);
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
  char sKWHY[10], sKWHT[10], sTruePower[10], sPowerFactor[10], sVoltage[10], sCurrent[10], sensor[300];

  dtostrf((float)sysCfg.hlw_kWhyesterday / 100000000, 1, ENERGY_RESOLUTION &7, sKWHY);
  dtostrf((float)cs_kWhtoday / 100000000, 1, ENERGY_RESOLUTION &7, sKWHT);
  dtostrf(cs_truePower, 1, 1, sTruePower);
  dtostrf(cs_powerFactor, 1, 2, sPowerFactor);
  dtostrf(cs_voltage, 1, 1, sVoltage);
  dtostrf(cs_current, 1, 3, sCurrent);

  snprintf_P(sensor, sizeof(sensor), HTTP_ENERGY_SNS, sVoltage, sCurrent, sTruePower, sPowerFactor, sKWHT, sKWHY, cs_lostDataCount);
  page += sensor;
  return page;
}
#endif  // USE_WEBSERVER
#endif  // USE_CS5460A
