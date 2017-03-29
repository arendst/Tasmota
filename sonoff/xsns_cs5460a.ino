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
 * Source: Victor Ferrer https://github.com/vicfergar/ESP8266_MQTT
\*********************************************************************************************/

#include <RingBuf.h>

#define REGISTERS_PER_FRAME 3
#define TIMER_RESET_US 20000L				        // Goal to achieve about 10-30ms timer interval
#define BUFSIZE (REGISTERS_PER_FRAME * 10)  // Grab last 10 frames

#define VOLTAGE_RANGE 0.250					    // Full scale V channel voltage
#define CURRENT_RANGE 0.050					    // Full scale I channel voltage (PGA 50x instead of 10x)
#define VOLTAGE_DIVIDER 450220.0/220.0	// Input voltage channel divider (R1+R2/R2)
#define CURRENT_SHUNT 620					      // Empirically obtained multiplier to scale Vshunt drop to I    
#define FLOAT24 16777216.0					    // 2^24 (converts to float24)
#define POWER_MULTIPLIER 1 / 512.0			// Energy->Power divider; not sure why, but seems correct. Datasheet not clear about this.

#define VOLTAGE_MULTIPLIER   (float)  (1 / FLOAT24 * VOLTAGE_RANGE * VOLTAGE_DIVIDER)
#define CURRENT_MULTIPLIER   (float)  (1 / FLOAT24 * CURRENT_RANGE * CURRENT_SHUNT)

class PowerMeter_CS5460
{
public:
	static float voltage;
	static float current;
	static float truePower;
	static float powerFactor;
  static float energy;

	static volatile int lostDataCount;

	static void initialize(int clkPin, int sdoPin);
  static void finalize();
	static bool loop();

private:
	static int _clkPin;
	static int _sdoPin;

	static volatile uint32_t _sniffRegister;
	static volatile uint8_t _bitsForNextData;
	static volatile bool _syncPulse;
	static volatile bool _readyReceived;
	static RingBuf *_dataBuf;

	static void inline resetTimer();
	static void inline clockISR();
	static void inline timerOVF();
};

float PowerMeter_CS5460::voltage = 0;
float PowerMeter_CS5460::current = 0;
float PowerMeter_CS5460::truePower = 0;
float PowerMeter_CS5460::powerFactor = 0;
float PowerMeter_CS5460::energy = 0;
int PowerMeter_CS5460::_clkPin;
int PowerMeter_CS5460::_sdoPin;

volatile int PowerMeter_CS5460::lostDataCount = 0;
volatile uint32_t PowerMeter_CS5460::_sniffRegister = 0;
volatile uint8_t PowerMeter_CS5460::_bitsForNextData;
volatile bool PowerMeter_CS5460::_syncPulse = false;
volatile bool PowerMeter_CS5460::_readyReceived = false;
RingBuf *PowerMeter_CS5460::_dataBuf = RingBuf_new(sizeof(uint32_t), BUFSIZE);

void PowerMeter_CS5460::initialize(int clkPin, int sdoPin)
{
	_clkPin = clkPin;
	_sdoPin = sdoPin;

	// Setting up interrupt ISR on D2 (INT0), trigger function "clockISR()" when INT0 (CLK) is rising
	attachInterrupt(digitalPinToInterrupt(_clkPin), clockISR, RISING);

	// Set the CLK-pin and MISO-pin as inputs
	pinMode(_clkPin, INPUT);
	pinMode(_sdoPin, INPUT);

	// Setup timer0 interrupt
	noInterrupts();
	timer0_isr_init();
	timer0_attachInterrupt(timerOVF);
	resetTimer();
	interrupts();
}

void PowerMeter_CS5460::finalize()
{
  // Detach all interruptions
  detachInterrupt(digitalPinToInterrupt(_clkPin));
  timer0_detachInterrupt();
}

void inline PowerMeter_CS5460::resetTimer()
{
	uint32_t offset = (clockCyclesPerMicrosecond() * TIMER_RESET_US);  // Converts microseconds to ticks
	timer0_write((ESP.getCycleCount() + offset));
}

void inline PowerMeter_CS5460::timerOVF()
{
	// Went ~20ms without a clock pulse, probably in an inter-frame period; reset sync
	_syncPulse = true;
	_readyReceived = false;
	_bitsForNextData = 64;
	resetTimer();
}

void inline PowerMeter_CS5460::clockISR()
{
	PowerMeter_CS5460::resetTimer();

	if (!_syncPulse && !_readyReceived) {
		return;
	}

	// Shift one bit to left and store MISO-value (0 or 1)
	_sniffRegister = (_sniffRegister << 1) | digitalRead(_sdoPin);
	_bitsForNextData--;

	if (_bitsForNextData == 0) {
		if (!_readyReceived) {
			// Check Status register is has conversion ready ( DRDY=1, ID=15 )
			_readyReceived = _sniffRegister == 0x009003C1;
			_bitsForNextData = 64;
		}
		else {
			_syncPulse = false;
			_bitsForNextData = 32;

			// Useful data. Save in data buffer
			if (_dataBuf->add(_dataBuf, (void*)&_sniffRegister) < 0)
			{
				lostDataCount++;
			}
		}
	}
}

bool PowerMeter_CS5460::loop()
{
	bool frameAvailable = _dataBuf->numElements(_dataBuf) >= REGISTERS_PER_FRAME;

	if (frameAvailable)
	{
		uint32_t rawRegister;

		// read Vrms register
		_dataBuf->pull(_dataBuf, &rawRegister);
		voltage = rawRegister * VOLTAGE_MULTIPLIER;

		// read Irms register
		_dataBuf->pull(_dataBuf, &rawRegister);
		current = rawRegister * CURRENT_MULTIPLIER;

		// read E (energy) register
		_dataBuf->pull(_dataBuf, &rawRegister);
		if (rawRegister & 0x800000) {
			// must sign extend int24 -> int32LE
			rawRegister |= 0xFF000000;
		}
		truePower = ((int32_t)rawRegister) * POWER_MULTIPLIER;

		float apparent_power = voltage * current;
		powerFactor = truePower / apparent_power;
	}

	return frameAvailable;
}












PowerMeter_CS5460 powerMeter;
Ticker tickerCS;

byte cs_seconds, cs_startup;
unsigned long cs_kWhtoday;
uint32_t cs_lasttime;

void cs_init()
{
  if (!sysCfg.hlw_pcal || (sysCfg.hlw_pcal == 4975)) {
    sysCfg.hlw_pcal = HLW_PREF_PULSE;
    sysCfg.hlw_ucal = HLW_UREF_PULSE;
    sysCfg.hlw_ical = HLW_IREF_PULSE;
  }

  cs_kWhtoday = 0;

  powerMeter.initialize(pin[GPIO_CS_CLK], pin[GPIO_CS_SDO]);
  
  tickerCS.attach_ms(200, cs_200mS);
}

void cs_finish()
{
  powerMeter.finalize();
}

void cs_loop()
{
  powerMeter.loop();
}

void cs_200mS()
{
  unsigned long hlw_len, hlw_temp;
  
  cs_kWhtoday = powerMeter.truePower;

  cs_seconds++;
  if (cs_seconds == 5) {
    cs_seconds = 0;

    if (rtc_loctime() == rtc_midnight()) {
      sysCfg.hlw_kWhyesterday = cs_kWhtoday;
      cs_kWhtoday = 0;
    }
    
    if (cs_startup && rtcTime.Valid && (rtcTime.DayOfYear == sysCfg.hlw_kWhdoy)) {
      cs_kWhtoday = sysCfg.hlw_kWhtoday;
      cs_startup = 0;
    }
  }
}

void hlw_savestate()
{
  sysCfg.hlw_kWhdoy = (rtcTime.Valid) ? rtcTime.DayOfYear : 0;
  sysCfg.hlw_kWhtoday = cs_kWhtoday;
}

boolean hlw_readEnergy(byte option, float &ed, uint16_t &e, uint16_t &w, uint16_t &u, float &i, float &c)
{
  ed = powerMeter.energy;
  e = 0;
  w = powerMeter.truePower;
  u = powerMeter.voltage;
  i = powerMeter.current;
  c = powerMeter.powerFactor;
  
  return true;
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
  //power_steady_cntr = 2;
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

  snprintf_P(svalue, ssvalue, PSTR("{\"PowerLow\"}"));

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
  char stemp0[10], stemp1[10], stemp2[10], stemp3[10], speriod[20];
  float ped, pi, pc;
  uint16_t pe, pw, pu;

  hlw_readEnergy(option, ped, pe, pw, pu, pi, pc);
  dtostrf((float)sysCfg.hlw_kWhyesterday / 100000000, 1, ENERGY_RESOLUTION &7, stemp0);
  dtostrf(ped, 1, ENERGY_RESOLUTION &7, stemp1);
  dtostrf(pc, 1, 2, stemp2);
  dtostrf(pi, 1, 3, stemp3);
  snprintf_P(speriod, sizeof(speriod), PSTR(", \"Period\":%d"), pe);
  snprintf_P(svalue, ssvalue, PSTR("%s\"Yesterday\":%s, \"Today\":%s%s, \"Power\":%d, \"Factor\":%s, \"Voltage\":%d, \"Current\":%s}"),
    svalue, stemp0, stemp1, (option) ? speriod : "", pw, stemp2, pu, stemp3);
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
  "<tr><th>Voltage</th><td>%d V</td></tr>"
  "<tr><th>Current</th><td>%s A</td></tr>"
  "<tr><th>Power</th><td>%d W</td></tr>"
  "<tr><th>Power Factor</th><td>%s</td></tr>"
  "<tr><th>Energy Today</th><td>%s kWh</td></tr>"
  "<tr><th>Energy Yesterday</th><td>%s kWh</td></tr>";

String hlw_webPresent()
{
  String page = "";
  char stemp[10], stemp2[10], stemp3[10], stemp4[10], sensor[300];
  float ped, pi, pc;
  uint16_t pe, pw, pu;

  hlw_readEnergy(0, ped, pe, pw, pu, pi, pc);

  dtostrf(pi, 1, 3, stemp);
  dtostrf(pc, 1, 2, stemp2);
  dtostrf(ped, 1, ENERGY_RESOLUTION &7, stemp3);
  dtostrf((float)sysCfg.hlw_kWhyesterday / 100000000, 1, ENERGY_RESOLUTION &7, stemp4);
  snprintf_P(sensor, sizeof(sensor), HTTP_ENERGY_SNS, pu, stemp, pw, stemp2, stemp3, stemp4);
  page += sensor;
  return page;
}
#endif  // USE_WEBSERVER
#endif  // USE_CS5460A
