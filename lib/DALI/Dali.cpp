
#include "Dali.h"



Dali::Dali() //constructor
{
  applyWorkAround1Mhz = 0;
}


void Dali::setTxPin(uint8_t pin)
{
  TxPin = pin; // user sets the digital pin as output
  pinMode(TxPin, OUTPUT); 
  digitalWrite(TxPin, HIGH);
}

void Dali::setRxAnalogPin(uint8_t pin)
{
	RxAnalogPin = pin; // user sets the digital pin as output
}

void Dali::workAround1MhzTinyCore(uint8_t a)
{
  applyWorkAround1Mhz = a;
}

void Dali::setupAnalogReceive(uint8_t pin) 
{
	setRxAnalogPin(pin); // user sets the analog pin as input
}


void Dali::setupTransmit(uint8_t pin)
{
  setTxPin(pin);
  speedFactor = 2;
  //we don't use exact calculation of passed time spent outside of transmitter
  //because of high ovehead associated with it, instead we use this 
  //emprirically determined values to compensate for the time loss
  
  #if F_CPU == 1000000UL
    uint16_t compensationFactor = 88; //must be divisible by 8 for workaround
  #elif F_CPU == 8000000UL
    uint16_t compensationFactor = 12; 
  #else //16000000Mhz
    uint16_t compensationFactor = 4; 
  #endif  

#if (F_CPU == 80000000UL) || (F_CPU == 160000000)   // ESP8266 80MHz or 160 MHz
  delay1 = delay2 = (HALF_BIT_INTERVAL >> speedFactor) - 2;
#else
  delay1 = (HALF_BIT_INTERVAL >> speedFactor) - compensationFactor;
  delay2 = (HALF_BIT_INTERVAL >> speedFactor) - 2;
  period = delay1 + delay2;
  
  #if F_CPU == 1000000UL
    delay2 -= 22; //22+2 = 24 is divisible by 8
    if (applyWorkAround1Mhz) { //definition of micro delay is broken for 1MHz speed in tiny cores as of now (May 2013)
      //this is a workaround that will allow us to transmit on 1Mhz
      //divide the wait time by 8
      delay1 >>= 3;
      delay2 >>= 3;
    }
  #endif
#endif

	}


void Dali::transmit(uint8_t cmd1, uint8_t cmd2) // transmit commands to DALI bus (address byte, command byte)
{
	sendBit(1);
	sendByte(cmd1);
	sendByte(cmd2);
	digitalWrite(TxPin, HIGH);
}


void Dali::sendByte(uint8_t b) 
{
	for (int i = 7; i >= 0; i--) 
	{
		sendBit((b >> i) & 1);
	}
}


void Dali::sendBit(int b) 
{		
 if (b) {
		sendOne();
	}
	else {
		sendZero();
	} 
}


void Dali::sendZero(void)
{
  digitalWrite(TxPin, HIGH);
  delayMicroseconds(delay2);
  digitalWrite(TxPin, LOW);
  delayMicroseconds(delay1);

}


void Dali::sendOne(void)
{
  digitalWrite(TxPin, LOW);
  delayMicroseconds(delay2);
  digitalWrite(TxPin, HIGH);
  delayMicroseconds(delay1);
}


void Dali::busTest() //DALI bus test
{
	int maxLevel;
	int minLevel;
	
	//Luminaries must turn on and turn off. If not, check connection.
	delay(100);
	dali.transmit(BROADCAST_C, OFF_C); //Broadcast ON
	delay(500);
	dali.transmit(BROADCAST_C, ON_C); //Broadcast OFF
	delay(100);
	while (!Serial);
	
	//Receive response from luminaries: max and min level
	dali.transmit(BROADCAST_C, QUERY_STATUS);
	maxLevel = dali.maxResponseLevel();
	dali.transmit(BROADCAST_C, QUERY_STATUS);
	minLevel = dali.minResponseLevel();

	if (dali.msgMode) {
		Serial.print("Min Level: ");
		Serial.print(minLevel);
		Serial.println();
		Serial.print("Max Level: ");
		Serial.print(maxLevel);
		Serial.println();
	}

	dali.analogLevel = (int)(maxLevel + minLevel) / 2;
	
}


void Dali::splitAdd(long input, uint8_t &highbyte, uint8_t &middlebyte, uint8_t &lowbyte) 
{
	highbyte = input >> 16;
	middlebyte = input >> 8;
	lowbyte = input;
}



// define min response level
int Dali::minResponseLevel() 
{

	const uint8_t dalistep = 40; //us
	uint16_t rxmin = 1024;
	uint16_t dalidata;
	long idalistep;

	
	
	for (idalistep = 0; idalistep < dali.daliTimeout; idalistep = idalistep + dalistep) {
		dalidata = analogRead(RxAnalogPin);
		if (dalidata < rxmin) {
			rxmin = dalidata;
		};
		delayMicroseconds(dalistep);
	}
	return rxmin; 
}

// define max response level
int Dali::maxResponseLevel() 
{

	const uint8_t dalistep = 40; //us
	uint16_t rxmax = 0;
	uint16_t dalidata;
	long idalistep;

	
	for (idalistep = 0; idalistep < dali.daliTimeout; idalistep = idalistep + dalistep) {
		dalidata = analogRead(dali.RxAnalogPin);
		if (dalidata > rxmax) {
			rxmax = dalidata;
		};
		delayMicroseconds(dalistep);
	}
	return rxmax;
}


//scan for individual short address
void Dali::scanShortAdd()
{

	const int delayTime = 10;
	const uint8_t start_ind_adress = 0;
	const uint8_t finish_ind_adress = 127;
	uint8_t add_byte;
	uint8_t device_short_add;
	uint8_t response;
		
	dali.transmit(BROADCAST_C, OFF_C); // Broadcast Off
	delay(delayTime);
	
	if (dali.msgMode) {
		Serial.println("Short addresses:");
	}

	for (device_short_add = start_ind_adress; device_short_add <= 63; device_short_add++) {

		add_byte = 1 + (device_short_add << 1); // convert short address to address byte
		
		
		dali.transmit(add_byte, 0xA1);
		
		response = dali.receive();

		// makes it better?
		delay(100);
		
		if (dali.getResponse) {
			
			dali.transmit(add_byte, ON_C); // switch on
			delay(1000);
			dali.transmit(add_byte, OFF_C); // switch off
			delay(1000);

		}
		else {
			response = 0;
		}

		
		
		if (dali.msgMode) {
			Serial.print("BIN: ");
			Serial.print(device_short_add, BIN);
			Serial.print(" ");
			Serial.print("DEC: ");
			Serial.print(device_short_add, DEC);
			Serial.print(" ");
			Serial.print("HEX: ");
			Serial.print(device_short_add, HEX);
			Serial.print(" ");
			if (dali.getResponse) {
				Serial.print("Get response");
			}
			else {
				Serial.print("No response");
			}
			Serial.println();
		}
		else {
			if (dali.getResponse) {
				Serial.println(255, BIN);
			}
			else {
				Serial.println(0, BIN);
			}
			
		}

	}

	dali.transmit(BROADCAST_C, ON_C); // Broadcast On
	Serial.println();
	delay(delayTime);

}


int Dali::readBinaryString(const char *s) 
{
	int result = 0;
	while (*s) {
		result <<= 1;
		if (*s++ == '1') result |= 1;
	}
	return result;
}


bool Dali::cmdCheck(String & input, int & cmd1, int & cmd2) 
{
	bool test = true;

	input.replace(" ", "");   // Delete spaces

	if (input.length() != 16) {
		test = false; //check if command contain 16bit
	}
	else {
		for (int i = 0; i <= input.length() - 1; i++) {
			if ((int)input.charAt(i) == 49 or (int)input.charAt(i) == 48) {}
			else {
				test = false;
			};
		};
	};

	if (test) {
		cmd1 = readBinaryString(input.substring(0, 8).c_str());
		cmd2 = readBinaryString(input.substring(8, 16).c_str());
	}

	return test;
}

void Dali::initialisation() {

	const int delaytime = 10; //ms

	long low_longadd = 0x000000;
	long high_longadd = 0xFFFFFF;
	long longadd = (long)(low_longadd + high_longadd) / 2;
	uint8_t highbyte;
	uint8_t middlebyte;
	uint8_t lowbyte;
	uint8_t short_add = 0;
	uint8_t cmd2;

	delay(delaytime);
	dali.transmit(BROADCAST_C, RESET);
	delay(delaytime);
	dali.transmit(BROADCAST_C, RESET);
	delay(delaytime);
	dali.transmit(BROADCAST_C, OFF_C);
	delay(delaytime);
	dali.transmit(0b10100101, 0b00000000); //initialise
	delay(delaytime);
	dali.transmit(0b10100101, 0b00000000); //initialise
	delay(delaytime);
	dali.transmit(0b10100111, 0b00000000); //randomise
	delay(delaytime);
	dali.transmit(0b10100111, 0b00000000); //randomise

	if (dali.msgMode) {
		Serial.println("Searching fo long addresses:");
	}

	while (longadd <= 0xFFFFFF - 2 and short_add <= 64) {
		while ((high_longadd - low_longadd) > 1) {

			dali.splitAdd(longadd, highbyte, middlebyte, lowbyte); //divide 24bit adress into three 8bit adresses
			delay(delaytime);
			dali.transmit(0b10110001, highbyte); //search HB
			delay(delaytime);
			dali.transmit(0b10110011, middlebyte); //search MB
			delay(delaytime);
			dali.transmit(0b10110101, lowbyte); //search LB
			delay(delaytime);
			dali.transmit(0b10101001, 0b00000000); //compare
			
			if (minResponseLevel() > dali.analogLevel) 
			{
				low_longadd = longadd;
			}
			else 
			{
				high_longadd = longadd;
			}
			
			longadd = (low_longadd + high_longadd) / 2; //center

			if (dali.msgMode) {
				Serial.print("BIN: ");
				Serial.print(longadd + 1, BIN);
				Serial.print(" ");
				Serial.print("DEC: ");
				Serial.print(longadd + 1, DEC);
				Serial.print(" ");
				Serial.print("HEX: ");
				Serial.print(longadd + 1, HEX);
				Serial.println();
			}
			else {
				Serial.println(longadd + 1);
			}
		} // second while


		if (high_longadd != 0xFFFFFF) 
		{
			splitAdd(longadd + 1, highbyte, middlebyte, lowbyte);
			dali.transmit(0b10110001, highbyte); //search HB
			delay(delaytime);
			dali.transmit(0b10110011, middlebyte); //search MB
			delay(delaytime);
			dali.transmit(0b10110101, lowbyte); //search LB
			delay(delaytime);
			dali.transmit(0b10110111, 1 + (short_add << 1)); //program short adress
			delay(delaytime);
			dali.transmit(0b10101011, 0b00000000); //withdraw
			delay(delaytime);
			dali.transmit(1 + (short_add << 1), ON_C);
			delay(1000);
			dali.transmit(1 + (short_add << 1), OFF_C);
			delay(delaytime);
			short_add++;

			if (dali.msgMode) {	
			Serial.println("Assigning a short address");
			}

			high_longadd = 0xFFFFFF;
			longadd = (low_longadd + high_longadd) / 2;

		}
		else {
			if (dali.msgMode) { 
				Serial.println("End"); 
			}
		}
	} // first while


	dali.transmit(0b10100001, 0b00000000);  //terminate
	dali.transmit(BROADCAST_C, ON_C);  //broadcast on
}


uint8_t Dali::receive() {


	
	unsigned long startFuncTime = 0;
	bool previousLogicLevel = 1;
	bool currentLogicLevel = 1;
	uint8_t arrLength = 20;
	int  timeArray[arrLength];
	int i = 0;
	int k = 0;
	bool logicLevelArray[arrLength];
	int response = 0;

	dali.getResponse = false;
	startFuncTime = micros();
	
	// add check for micros overlap here!!!

	while (micros() - startFuncTime < dali.daliTimeout and i < arrLength)
	{
		// geting response
		if (analogRead(dali.RxAnalogPin) > dali.analogLevel) {
			currentLogicLevel = 1;
		}
		else {
			currentLogicLevel = 0;
		}

		if (previousLogicLevel != currentLogicLevel) {
			timeArray[i] = micros() - startFuncTime;
			logicLevelArray[i] = currentLogicLevel;
			previousLogicLevel = currentLogicLevel;
			dali.getResponse = true;
			i++;

		}
	}

		
	
	arrLength = i;

	//decoding to manchester
	for (i = 0; i < arrLength - 1; i++) {
		if ((timeArray[i + 1] - timeArray[i]) > 0.75 * dali.period) {
			for (k = arrLength; k > i; k--) {
				timeArray[k] = timeArray[k - 1];
				logicLevelArray[k] = logicLevelArray[k - 1];
			}
			arrLength++;
			timeArray[i + 1] = (timeArray[i] + timeArray[i + 2]) / 2;
			logicLevelArray[i + 1] = logicLevelArray[i];
		}
	}





	k = 8;

	for (i = 1; i < arrLength; i++) {
		if (logicLevelArray[i] == 1) {
			if ((int)round((timeArray[i] - timeArray[0]) / (0.5 * dali.period)) & 1) {
				response = response + (1 << k);
			}
			k--;
		}
	}

	
	//remove start bit
	response = (uint8_t)response;
	
	return response;

}




	Dali dali;
