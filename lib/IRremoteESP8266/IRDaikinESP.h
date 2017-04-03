
#include <IRremoteESP8266.h>
#include <Arduino.h>

#define DAIKIN_COOL B011
#define DAIKIN_HEAT B100
#define DAIKIN_FAN B110
#define DAIKIN_AUTO B000
#define DAIKIN_DRY B010

#define DAIKIN_POWERFUL B00000010
#define DAIKIN_SILENT   B00100000

/*
	Daikin AC map
	byte 7= checksum of the first part (and last byte before a 29ms pause)
	byte 13=mode
		b7 = 0
		b6+b5+b4 = Mode
			Modes: b6+b5+b4
			011 = Cool
			100 = Heat (temp 23)
			110 = FAN (temp not shown, but 25)
			000 = Fully Automatic (temp 25)
			010 = DRY (temp 0xc0 = 96 degrees c)
		b3 = 0
		b2 = OFF timer set
		b1 = ON timer set
		b0 = Air Conditioner ON
	byte 14=temp*2   (Temp should be between 18 - 32)
	byte 16=Fan
		FAN control
		b7+b6+b5+b4 = Fan speed
			Fan: b7+b6+b5+b4
			0×30 = 1 bar
			0×40 = 2 bar
			0×50 = 3 bar
			0×60 = 4 bar
			0×70 = 5 bar
			0xa0 = Auto
			0xb0 = Not auto, moon + tree
		b3+b2+b1+b0 = Swing control up/down
			Swing control up/down:
			0000 = Swing up/down off
			1111 = Swing up/down on
	byte 17
			Swing control left/right:
			0000 = Swing left/right off
			1111 = Swing left/right on
	byte 21=Aux  -> Powerful (bit 1), Silent (bit 5)
	byte 24=Aux2 -> Intelligent eye on (bit 7)
	byte 26= checksum of the second part
*/

#define DAIKIN_COMMAND_LENGTH 27

class IRDaikinESP
{
    public:
        IRDaikinESP(int pin);
        //: IRsend(pin){};

        void send();

        void begin();
        void on();
        void off();
        uint8_t getPower();

        void setAux(uint8_t aux);
        uint8_t getAux();

        void setTemp(uint8_t temp);
        uint8_t getTemp();

        void setFan(uint8_t fan);
        uint8_t getFan();

        uint8_t getMode();
        void setMode(uint8_t mode);

        void setSwingVertical(uint8_t swing);
        uint8_t getSwingVertical();
        void setSwingHorizontal(uint8_t swing);
        uint8_t getSwingHorizontal();

    private:
        // # of bytes per command
        unsigned char daikin[DAIKIN_COMMAND_LENGTH] = {
        0x11,0xDA,0x27,0xF0,0x00,0x00,0x00,0x20,
        //0    1    2   3    4    5     6   7
        0x11,0xDA,0x27,0x00,0x00,0x41,0x1E,0x00,
        //8    9   10   11   12    13   14   15
        0xB0,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0x00,0xE3 };
        //16  17    18  19   20    21   22  23   24   25   26

        void checksum();

        IRsend _irsend;
};
