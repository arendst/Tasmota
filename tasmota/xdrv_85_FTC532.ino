/*
  xdrv_85_FTC532.ino - FTC532 touch buttons support for Tasmota

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

/*
  THE PLAN [tm]:                    OUTCOME:
  ==============                    ========
  appear in a dropdown              (D_SENSOR_FTC532_RX "FTC532 Rx")
  select pin                        (GPIO_FTC532)
  attach interrupt to pin           DONE
  ISR updating all 8 inputs         DONE
  de-bouncing for 50 ms             NOT REQUIRED
  change report every 250 ms        REPORTS EVERY 50MS
  Webserver display "00001001"      DONE
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
*/ 

#ifdef USE_FTC532

#define XDRV_85						85

#define FTC532_KEYS_MAX		8

#define FTC532_STATE_WAITING			0x01
#define FTC532_STATE_READING			0x02

// Rising edge timing in microseconds
#define FTC532_BIT								377
#define FTC532_SHORT							(FTC532_BIT * 2)
#define FTC532_LONG								(FTC532_BIT * 4)
#define FTC532_IDLE								(FTC532_BIT * 10)
#define FTC532_MAX								(FTC532_BIT * 58)

static uint16_t ftc532_initialized = 0;

struct FTC532 {
	volatile uint16_t state;							// ISR state
	volatile uint16_t valid;							// did we ever receive valid data?
	volatile uint16_t errors;							// error counter
	volatile uint16_t sample = 0xF0F0;		// buffer for bit-coded time samples
	uint16_t keys = 0;										// bitmap of active keys
	uint16_t old_keys = 0;								// previously active keys
	volatile uint32_t rxtime;							// ISR timer memory 
	volatile uint16_t rxbit;							// ISR bit counter
} ftc532;

void ICACHE_RAM_ATTR ftc532_ISR(void)			// Hardware interrupt routine, triggers on rising edge
{
	uint32_t time = micros();
	uint32_t time_diff = time - ftc532.rxtime;
	ftc532.rxtime = time;

	if (ftc532.state == FTC532_STATE_WAITING) {
		if (time_diff > FTC532_LONG + FTC532_SHORT) {			// new frame
			ftc532.rxbit = 0;
			ftc532.state = FTC532_STATE_READING;
			return;
		}
	}
	else { 						// FTC532_STATE_READING
		if (ftc532.rxbit >= FTC532_KEYS_MAX * 2 || time_diff > FTC532_LONG + FTC532_BIT) {
			++ftc532.errors;
			ftc532.rxbit = 0;
			ftc532.state = FTC532_STATE_WAITING;
			return;
		}
		else if (time_diff > FTC532_SHORT + FTC532_BIT) ftc532.sample |= (1 << ftc532.rxbit);
		else ftc532.sample &= ~(1 << ftc532.rxbit);
		++ftc532.rxbit;
		if (ftc532.rxbit == FTC532_KEYS_MAX * 2) {
			ftc532.rxbit = 0;
			ftc532.valid = 1;
			ftc532.state = FTC532_STATE_WAITING;						// frame complete
			return;
		}
	}
}

void ftc532_init(void)		// Initialize
{
	if (ftc532_initialized) return;
	ftc532.errors = ftc532.valid = 0;
	ftc532.state = FTC532_STATE_WAITING;
	ftc532.rxtime = micros();
	pinMode(Pin(GPIO_FTC532), INPUT_PULLUP);
	attachInterrupt(Pin(GPIO_FTC532), ftc532_ISR, RISING);
	++ftc532_initialized;
}

void ftc532_exit(void)		// Clean up
{
}

void ftc532_update(void)			// called every 50 ms
{
//		// WARNING: Reduce call frequency if this code is uncommented
//	if ((ftc532.sample & 0xF) != ((~ftc532.sample >> 4) & 0xF) || ((ftc532.sample >> 8) & 0xF) != ((~ftc532.sample >> 12) & 0xF)) {
//		AddLog_P(LOG_LEVEL_ERROR, PSTR("FTC532: inverted sample does not match %x %x %x %x"),
//						 ftc532.sample & 0xF, (~ftc532.sample >> 4) & 0xF, (ftc532.sample >> 8) & 0xF, (~ftc532.sample >> 12) & 0xF);
//	}
	ftc532.keys = (ftc532.sample & 0xF) | ((ftc532.sample >> 4) & 0xF0);
	if (ftc532.keys != ftc532.old_keys) {
		AddLog_P(LOG_LEVEL_DEBUG, PSTR("FTC: SAM=%04X KEY=%02X OLD=%02X ERR=%u OK=%u TIME=%lu Pin=%u"),
						 ftc532.sample, ftc532.keys, ftc532.old_keys, ftc532.errors, ftc532.valid, ftc532.rxtime, Pin(GPIO_FTC532));
		ftc532_publish();
		ftc532.old_keys = ftc532.keys;
	}
}

void ftc532_show(uint32_t json) {
	int i;
	char keystring[FTC532_KEYS_MAX + 1];
	
	if ( ! ftc532_initialized ) return;
  if (json == FUNC_JSON_APPEND) {
    ResponseAppend_P(PSTR(",\"FTC532\":{\"KEYS\":%02X"), ftc532.keys);
    ResponseJsonEnd();
  } else {
		for (i=0; i < FTC532_KEYS_MAX; ++i) keystring[i] = (ftc532.keys & (1 << i)) ? '1' : '0';
		keystring[i]='\0';
    WSContentSend_P(PSTR("<center>FTC532 KEYS: <b>%s</b></center>"), keystring);
  }
}

void ftc532_publish(void) {
	Response_P(PSTR("{\"FTC532\":{\"KEYS\":%02X}}"), ftc532.keys);
	MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR(D_CMND_SENSOR));
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv85(uint8_t function) {
	bool result = false;

	switch (function) {
		// Initialize driver
		case FUNC_INIT:
			ftc532_init();
			break;
		// timed callback functions
		case FUNC_EVERY_SECOND:
      break;
		case FUNC_EVERY_250_MSECOND:
			break;
		case FUNC_EVERY_100_MSECOND:
			break;
		case FUNC_EVERY_50_MSECOND:
			if (ftc532_initialized) ftc532_update();
			break;
		// Generate JSON telemetry string
		case FUNC_JSON_APPEND:
			ftc532_show(FUNC_JSON_APPEND);
			break;
#ifdef USE_WEBSERVER
		// Show sensor data on main web page
		case FUNC_WEB_SENSOR:
			ftc532_show(FUNC_WEB_SENSOR);
			break;
#endif				// USE_WEBSERVER
		case FUNC_SAVE_BEFORE_RESTART:
			ftc532_exit();
			break;
		}
	// Return bool result
	return result;
}

#endif  // USE_FTC532
