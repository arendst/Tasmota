

#ifndef dali_h
#define dali_h

//timer scaling factors for different transmission speeds
#define MAN_300 0
#define MAN_600 1
#define MAN_1200 2
#define MAN_2400 3
#define MAN_4800 4
#define MAN_9600 5
#define MAN_19200 6
#define MAN_38400 7

/*
Timer 2 in the ATMega328 and Timer 1 in a ATtiny85 is used to find the time between
each transition coming from the demodulation circuit.
Their setup is for sampling the input in regular intervals.
For practical reasons we use power of 2 timer prescaller for sampling, 
for best timing we use pulse lenght as integer multiple of sampling speed.
We chose to sample every 8 ticks, and pulse lenght of 48 ticks 
thats 6 samples per pulse, lower sampling rate (3) will not work well for 
innacurate clocks (like internal oscilator) higher sampling rate (12) will
cause too much overhead and will not work at higher transmission speeds.
This gives us 16000000Hz/48/256 = 1302 pulses per second (so it's not really 1200) 
At different transmission speeds or on different microcontroller frequencies, clock prescaller is adjusted 
to be compatible with those values. We allow about 50% clock speed difference both ways
allowing us to transmit even with up to 100% in clock speed difference
*/

// DALI coomands
#define BROADCAST_DP 0b11111110
#define BROADCAST_C 0b11111111
#define ON_DP 0b11111110
#define OFF_DP 0b00000000
#define ON_C 0b00000101
#define OFF_C 0b00000000
# define QUERY_STATUS 0b10010000
# define RESET 0b00100000


//setup timing for transmitter
#define HALF_BIT_INTERVAL 1666 





#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include <pins_arduino.h>
#endif

class Dali
{
  public:
	Dali(); //the constructor
    void setTxPin(uint8_t pin); //set the arduino digital pin for transmit. 
    void setRxAnalogPin(uint8_t pin); //set the arduino digital pin for receive.
    void workAround1MhzTinyCore(uint8_t a = 1); //apply workaround for defect in tiny Core library for 1Mhz
    void setupTransmit(uint8_t pin); //set up transmission
	void setupAnalogReceive(uint8_t pin);
    void transmit(uint8_t cmd1, uint8_t cmd2); //transmit 16 bits of data
	void scanShortAdd(); //scan for short address
	void busTest(); // bus test
	void initialisation(); //initialization of new luminaries
	bool cmdCheck(String & input, int & cmd1, int & cmd2);
	uint8_t receive(); //get response

	int minResponseLevel(); 
	int maxResponseLevel();
    
    uint8_t speedFactor;
    uint16_t delay1;
    uint16_t delay2;
	uint16_t period;
	String errorMsg; //error message of last operation
	bool msgMode; //0 - get only response from dali bus to COM; 1 - response with text (comments)
	bool getResponse;
	uint8_t RxAnalogPin;

	long daliTimeout = 20000; //us, DALI response timeout
	int analogLevel = 870; //analog border level (less - "0"; more - "1")
	


    
  private:
	
	void sendByte(uint8_t b); //transmit 8 bits of data
	void sendBit(int b); //transmit 1 bit of data
	void sendZero(void); //transmit "0"
    void sendOne(void); //transmit "1"
   	void splitAdd(long input, uint8_t &highbyte, uint8_t &middlebyte, uint8_t &lowbyte); //split random address 
	
	
	int readBinaryString(const char *s);

	uint8_t TxPin;
	
    uint8_t applyWorkAround1Mhz;
	uint8_t rxAnalogPin = 0;

};//end of class Dali

// Cant really do this as a real C++ class, since we need to have
// an ISR
extern "C"
{
    
    
   }

extern Dali dali;

#endif
