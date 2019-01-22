// ---------------------------------------------------------------------------
// Created by Tim Eckel - teckel@leethost.com
//
// See NewPing.h for license, purpose, syntax, version history, links, etc.
// ---------------------------------------------------------------------------

#include "NewPing.h"


// ---------------------------------------------------------------------------
// NewPing constructor
// ---------------------------------------------------------------------------

NewPing::NewPing(uint8_t trigger_pin, uint8_t echo_pin, unsigned int max_cm_distance) {
#if DO_BITWISE == true
	_triggerBit = digitalPinToBitMask(trigger_pin); // Get the port register bitmask for the trigger pin.
	_echoBit = digitalPinToBitMask(echo_pin);       // Get the port register bitmask for the echo pin.

	_triggerOutput = portOutputRegister(digitalPinToPort(trigger_pin)); // Get the output port register for the trigger pin.
	_echoInput = portInputRegister(digitalPinToPort(echo_pin));         // Get the input port register for the echo pin.

	_triggerMode = (uint8_t *) portModeRegister(digitalPinToPort(trigger_pin)); // Get the port mode register for the trigger pin.
#else
	_triggerPin = trigger_pin;
	_echoPin = echo_pin;
#endif

	set_max_distance(max_cm_distance); // Call function to set the max sensor distance.

#if (defined (__arm__) && (defined (TEENSYDUINO) || defined(PARTICLE))) || DO_BITWISE != true
	pinMode(echo_pin, INPUT);     // Set echo pin to input (on Teensy 3.x (ARM), pins default to disabled, at least one pinMode() is needed for GPIO mode).
	pinMode(trigger_pin, OUTPUT); // Set trigger pin to output (on Teensy 3.x (ARM), pins default to disabled, at least one pinMode() is needed for GPIO mode).
#endif

#if defined (ARDUINO_AVR_YUN)
	pinMode(echo_pin, INPUT);     // Set echo pin to input for the Arduino Yun, not sure why it doesn't default this way.
#endif

#if ONE_PIN_ENABLED != true && DO_BITWISE == true
	*_triggerMode |= _triggerBit; // Set trigger pin to output.
#endif
}


// ---------------------------------------------------------------------------
// Standard ping methods
// ---------------------------------------------------------------------------

unsigned int NewPing::ping(unsigned int max_cm_distance) {
	if (max_cm_distance > 0) set_max_distance(max_cm_distance); // Call function to set a new max sensor distance.

	if (!ping_trigger()) return NO_ECHO; // Trigger a ping, if it returns false, return NO_ECHO to the calling function.

#if URM37_ENABLED == true
	#if DO_BITWISE == true
		while (!(*_echoInput & _echoBit))             // Wait for the ping echo.
	#else
		while (!digitalRead(_echoPin))                // Wait for the ping echo.
	#endif
			if (micros() > _max_time) return NO_ECHO; // Stop the loop and return NO_ECHO (false) if we're beyond the set maximum distance.
#else
	#if DO_BITWISE == true
		while (*_echoInput & _echoBit)                // Wait for the ping echo.
	#else
		while (digitalRead(_echoPin))                 // Wait for the ping echo.
	#endif
			if (micros() > _max_time) return NO_ECHO; // Stop the loop and return NO_ECHO (false) if we're beyond the set maximum distance.
#endif

	return (micros() - (_max_time - _maxEchoTime) - PING_OVERHEAD); // Calculate ping time, include overhead.
}


unsigned long NewPing::ping_cm(unsigned int max_cm_distance) {
	unsigned long echoTime = NewPing::ping(max_cm_distance); // Calls the ping method and returns with the ping echo distance in uS.
#if ROUNDING_ENABLED == false
	return (echoTime / US_ROUNDTRIP_CM);              // Call the ping method and returns the distance in centimeters (no rounding).
#else
	return NewPingConvert(echoTime, US_ROUNDTRIP_CM); // Convert uS to centimeters.
#endif
}


unsigned long NewPing::ping_in(unsigned int max_cm_distance) {
	unsigned long echoTime = NewPing::ping(max_cm_distance); // Calls the ping method and returns with the ping echo distance in uS.
#if ROUNDING_ENABLED == false
	return (echoTime / US_ROUNDTRIP_IN);              // Call the ping method and returns the distance in inches (no rounding).
#else
	return NewPingConvert(echoTime, US_ROUNDTRIP_IN); // Convert uS to inches.
#endif
}


unsigned long NewPing::ping_median(uint8_t it, unsigned int max_cm_distance) {
	unsigned int uS[it], last;
	uint8_t j, i = 0;
	unsigned long t;
	uS[0] = NO_ECHO;

	while (i < it) {
		t = micros();                  // Start ping timestamp.
		last = ping(max_cm_distance);  // Send ping.

		if (last != NO_ECHO) {         // Ping in range, include as part of median.
			if (i > 0) {               // Don't start sort till second ping.
				for (j = i; j > 0 && uS[j - 1] < last; j--) // Insertion sort loop.
					uS[j] = uS[j - 1];                      // Shift ping array to correct position for sort insertion.
			} else j = 0;              // First ping is sort starting point.
			uS[j] = last;              // Add last ping to array in sorted position.
			i++;                       // Move to next ping.
		} else it--;                   // Ping out of range, skip and don't include as part of median.

		if (i < it && micros() - t < PING_MEDIAN_DELAY)
			delay((PING_MEDIAN_DELAY + t - micros()) / 1000); // Millisecond delay between pings.

	}
	return (uS[it >> 1]); // Return the ping distance median.
}


// ---------------------------------------------------------------------------
// Standard and timer interrupt ping method support functions (not called directly)
// ---------------------------------------------------------------------------

boolean NewPing::ping_trigger() {
#if DO_BITWISE == true
	#if ONE_PIN_ENABLED == true
		*_triggerMode |= _triggerBit;  // Set trigger pin to output.
	#endif

	*_triggerOutput &= ~_triggerBit;   // Set the trigger pin low, should already be low, but this will make sure it is.
	delayMicroseconds(4);              // Wait for pin to go low.
	*_triggerOutput |= _triggerBit;    // Set trigger pin high, this tells the sensor to send out a ping.
	delayMicroseconds(10);             // Wait long enough for the sensor to realize the trigger pin is high. Sensor specs say to wait 10uS.
	*_triggerOutput &= ~_triggerBit;   // Set trigger pin back to low.

	#if ONE_PIN_ENABLED == true
		*_triggerMode &= ~_triggerBit; // Set trigger pin to input (when using one Arduino pin, this is technically setting the echo pin to input as both are tied to the same Arduino pin).
	#endif

	#if URM37_ENABLED == true
		if (!(*_echoInput & _echoBit)) return false;            // Previous ping hasn't finished, abort.
		_max_time = micros() + _maxEchoTime + MAX_SENSOR_DELAY; // Maximum time we'll wait for ping to start (most sensors are <450uS, the SRF06 can take up to 34,300uS!)
		while (*_echoInput & _echoBit)                          // Wait for ping to start.
			if (micros() > _max_time) return false;             // Took too long to start, abort.
	#else
		if (*_echoInput & _echoBit) return false;               // Previous ping hasn't finished, abort.
		_max_time = micros() + _maxEchoTime + MAX_SENSOR_DELAY; // Maximum time we'll wait for ping to start (most sensors are <450uS, the SRF06 can take up to 34,300uS!)
		while (!(*_echoInput & _echoBit))                       // Wait for ping to start.
			if (micros() > _max_time) return false;             // Took too long to start, abort.
	#endif
#else
	#if ONE_PIN_ENABLED == true
		pinMode(_triggerPin, OUTPUT); // Set trigger pin to output.
	#endif
	
	digitalWrite(_triggerPin, LOW);   // Set the trigger pin low, should already be low, but this will make sure it is.
	delayMicroseconds(4);             // Wait for pin to go low.
	digitalWrite(_triggerPin, HIGH);  // Set trigger pin high, this tells the sensor to send out a ping.
	delayMicroseconds(10);            // Wait long enough for the sensor to realize the trigger pin is high. Sensor specs say to wait 10uS.
	digitalWrite(_triggerPin, LOW);   // Set trigger pin back to low.

	#if ONE_PIN_ENABLED == true
		pinMode(_triggerPin, INPUT);  // Set trigger pin to input (when using one Arduino pin, this is technically setting the echo pin to input as both are tied to the same Arduino pin).
	#endif

	#if URM37_ENABLED == true
		if (!digitalRead(_echoPin)) return false;               // Previous ping hasn't finished, abort.
		_max_time = micros() + _maxEchoTime + MAX_SENSOR_DELAY; // Maximum time we'll wait for ping to start (most sensors are <450uS, the SRF06 can take up to 34,300uS!)
		while (digitalRead(_echoPin))                           // Wait for ping to start.
			if (micros() > _max_time) return false;             // Took too long to start, abort.
	#else
		if (digitalRead(_echoPin)) return false;                // Previous ping hasn't finished, abort.
		_max_time = micros() + _maxEchoTime + MAX_SENSOR_DELAY; // Maximum time we'll wait for ping to start (most sensors are <450uS, the SRF06 can take up to 34,300uS!)
		while (!digitalRead(_echoPin))                          // Wait for ping to start.
			if (micros() > _max_time) return false;             // Took too long to start, abort.
	#endif
#endif

	_max_time = micros() + _maxEchoTime; // Ping started, set the time-out.
	return true;                         // Ping started successfully.
}


void NewPing::set_max_distance(unsigned int max_cm_distance) {
#if ROUNDING_ENABLED == false
	_maxEchoTime = min(max_cm_distance + 1, (unsigned int) MAX_SENSOR_DISTANCE + 1) * US_ROUNDTRIP_CM; // Calculate the maximum distance in uS (no rounding).
#else
	_maxEchoTime = min(max_cm_distance, (unsigned int) MAX_SENSOR_DISTANCE) * US_ROUNDTRIP_CM + (US_ROUNDTRIP_CM / 2); // Calculate the maximum distance in uS.
#endif
}


#if TIMER_ENABLED == true && DO_BITWISE == true

	// ---------------------------------------------------------------------------
	// Timer interrupt ping methods (won't work with ATmega128, ATtiny and most non-AVR microcontrollers)
	// ---------------------------------------------------------------------------

	void NewPing::ping_timer(void (*userFunc)(void), unsigned int max_cm_distance) {
		if (max_cm_distance > 0) set_max_distance(max_cm_distance); // Call function to set a new max sensor distance.

		if (!ping_trigger()) return;         // Trigger a ping, if it returns false, return without starting the echo timer.
		timer_us(ECHO_TIMER_FREQ, userFunc); // Set ping echo timer check every ECHO_TIMER_FREQ uS.
	}


	boolean NewPing::check_timer() {
		if (micros() > _max_time) { // Outside the time-out limit.
			timer_stop();           // Disable timer interrupt
			return false;           // Cancel ping timer.
		}

	#if URM37_ENABLED == false
		if (!(*_echoInput & _echoBit)) { // Ping echo received.
	#else
		if (*_echoInput & _echoBit) {    // Ping echo received.
	#endif
			timer_stop();                // Disable timer interrupt
			ping_result = (micros() - (_max_time - _maxEchoTime) - PING_TIMER_OVERHEAD); // Calculate ping time including overhead.
			return true;                 // Return ping echo true.
		}

		return false; // Return false because there's no ping echo yet.
	}


	// ---------------------------------------------------------------------------
	// Timer2/Timer4 interrupt methods (can be used for non-ultrasonic needs)
	// ---------------------------------------------------------------------------

	// Variables used for timer functions
	void (*intFunc)();
	void (*intFunc2)();
	unsigned long _ms_cnt_reset;
	volatile unsigned long _ms_cnt;
	#if defined(__arm__) && (defined (TEENSYDUINO) || defined(PARTICLE))
		IntervalTimer itimer;
	#endif


	void NewPing::timer_us(unsigned int frequency, void (*userFunc)(void)) {
		intFunc = userFunc; // User's function to call when there's a timer event.
		timer_setup();      // Configure the timer interrupt.

	#if defined (__AVR_ATmega32U4__) // Use Timer4 for ATmega32U4 (Teensy/Leonardo).
		OCR4C = min((frequency>>2) - 1, 255); // Every count is 4uS, so divide by 4 (bitwise shift right 2) subtract one, then make sure we don't go over 255 limit.
		TIMSK4 = (1<<TOIE4);                  // Enable Timer4 interrupt.
	#elif defined (__arm__) && defined (TEENSYDUINO) // Timer for Teensy 3.x
		itimer.begin(userFunc, frequency);           // Really simple on the Teensy 3.x, calls userFunc every 'frequency' uS.
	#elif defined (__arm__) && defined (PARTICLE)    // Timer for Particle devices
		itimer.begin(userFunc, frequency, uSec);     // Really simple on the Particle, calls userFunc every 'frequency' uS.
	#else
		OCR2A = min((frequency>>2) - 1, 255); // Every count is 4uS, so divide by 4 (bitwise shift right 2) subtract one, then make sure we don't go over 255 limit.
		TIMSK2 |= (1<<OCIE2A);                // Enable Timer2 interrupt.
	#endif
	}


	void NewPing::timer_ms(unsigned long frequency, void (*userFunc)(void)) {
		intFunc = NewPing::timer_ms_cntdwn;  // Timer events are sent here once every ms till user's frequency is reached.
		intFunc2 = userFunc;                 // User's function to call when user's frequency is reached.
		_ms_cnt = _ms_cnt_reset = frequency; // Current ms counter and reset value.
		timer_setup();                       // Configure the timer interrupt.

	#if defined (__AVR_ATmega32U4__) // Use Timer4 for ATmega32U4 (Teensy/Leonardo).
		OCR4C = 249;           // Every count is 4uS, so 1ms = 250 counts - 1.
		TIMSK4 = (1<<TOIE4);   // Enable Timer4 interrupt.
	#elif defined (__arm__) && defined (TEENSYDUINO)        // Timer for Teensy 3.x
		itimer.begin(NewPing::timer_ms_cntdwn, 1000);       // Set timer to 1ms (1000 uS).
	#elif defined (__arm__) && defined (PARTICLE)           // Timer for Particle
		itimer.begin(NewPing::timer_ms_cntdwn, 1000, uSec); // Set timer to 1ms (1000 uS).
	#else
		OCR2A = 249;           // Every count is 4uS, so 1ms = 250 counts - 1.
		TIMSK2 |= (1<<OCIE2A); // Enable Timer2 interrupt.
	#endif
	}


	void NewPing::timer_stop() { // Disable timer interrupt.
	#if defined (__AVR_ATmega32U4__) // Use Timer4 for ATmega32U4 (Teensy/Leonardo).
		TIMSK4 = 0;
	#elif defined (__arm__) && (defined (TEENSYDUINO) || defined (PARTICLE)) // Timer for Teensy 3.x & Particle
		itimer.end();
	#else
		TIMSK2 &= ~(1<<OCIE2A);
	#endif
	}


	// ---------------------------------------------------------------------------
	// Timer2/Timer4 interrupt method support functions (not called directly)
	// ---------------------------------------------------------------------------

	void NewPing::timer_setup() {
	#if defined (__AVR_ATmega32U4__) // Use Timer4 for ATmega32U4 (Teensy/Leonardo).
		timer_stop(); // Disable Timer4 interrupt.
		TCCR4A = TCCR4C = TCCR4D = TCCR4E = 0;
		TCCR4B = (1<<CS42) | (1<<CS41) | (1<<CS40) | (1<<PSR4); // Set Timer4 prescaler to 64 (4uS/count, 4uS-1020uS range).
		TIFR4 = (1<<TOV4);
		TCNT4 = 0;    // Reset Timer4 counter.
	#elif defined (__AVR_ATmega8__) || defined (__AVR_ATmega16__) || defined (__AVR_ATmega32__) || defined (__AVR_ATmega8535__) // Alternate timer commands for certain microcontrollers.
		timer_stop();                 // Disable Timer2 interrupt.
		ASSR &= ~(1<<AS2);            // Set clock, not pin.
		TCCR2 = (1<<WGM21 | 1<<CS22); // Set Timer2 to CTC mode, prescaler to 64 (4uS/count, 4uS-1020uS range).
		TCNT2 = 0;                    // Reset Timer2 counter.
	#elif defined (__arm__) && (defined (TEENSYDUINO) || defined (PARTICLE))
		timer_stop(); // Stop the timer.
	#else
		timer_stop();        // Disable Timer2 interrupt.
		ASSR &= ~(1<<AS2);   // Set clock, not pin.
		TCCR2A = (1<<WGM21); // Set Timer2 to CTC mode.
		TCCR2B = (1<<CS22);  // Set Timer2 prescaler to 64 (4uS/count, 4uS-1020uS range).
		TCNT2 = 0;           // Reset Timer2 counter.
	#endif
	}


	void NewPing::timer_ms_cntdwn() {
		if (!_ms_cnt--) {            // Count down till we reach zero.
			intFunc2();              // Scheduled time reached, run the main timer event function.
			_ms_cnt = _ms_cnt_reset; // Reset the ms timer.
		}
	}

	#if defined (__AVR_ATmega32U4__) // Use Timer4 for ATmega32U4 (Teensy/Leonardo).
	ISR(TIMER4_OVF_vect) {
		intFunc(); // Call wrapped function.
	}
	#elif defined (__AVR_ATmega8__) || defined (__AVR_ATmega16__) || defined (__AVR_ATmega32__) || defined (__AVR_ATmega8535__) // Alternate timer commands for certain microcontrollers.
	ISR(TIMER2_COMP_vect) {
		intFunc(); // Call wrapped function.
	}
	#elif defined (__arm__)
		// Do nothing...
	#else
	ISR(TIMER2_COMPA_vect) {
		intFunc(); // Call wrapped function.
	}
	#endif


#endif


// ---------------------------------------------------------------------------
// Conversion methods (rounds result to nearest cm or inch).
// ---------------------------------------------------------------------------

unsigned int NewPing::convert_cm(unsigned int echoTime) {
#if ROUNDING_ENABLED == false
	return (echoTime / US_ROUNDTRIP_CM);              // Convert uS to centimeters (no rounding).
#else
	return NewPingConvert(echoTime, US_ROUNDTRIP_CM); // Convert uS to centimeters.
#endif
}


unsigned int NewPing::convert_in(unsigned int echoTime) {
#if ROUNDING_ENABLED == false
	return (echoTime / US_ROUNDTRIP_IN);              // Convert uS to inches (no rounding).
#else
	return NewPingConvert(echoTime, US_ROUNDTRIP_IN); // Convert uS to inches.
#endif
}
