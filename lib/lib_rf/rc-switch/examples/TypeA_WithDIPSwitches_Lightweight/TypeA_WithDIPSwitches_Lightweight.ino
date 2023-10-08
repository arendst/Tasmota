/*
  This is a minimal sketch without using the library at all but only works for
  the 10 pole dip switch sockets. It saves a lot of memory and thus might be 
  very useful to use with ATTinys :)
  
  https://github.com/sui77/rc-switch/
*/

int RCLpin = 7;

void setup() {
    pinMode(RCLpin, OUTPUT);
}

void loop() {
    RCLswitch(0b010001000001);  // DIPs an Steckdose: 0100010000 An:01
    delay(2000);

    RCLswitch(0b010001000010);  // DIPs an Steckdose: 0100010000 Aus:10
    delay(2000);    
}

void RCLswitch(uint16_t code) {
    for (int nRepeat=0; nRepeat<6; nRepeat++) {
        for (int i=4; i<16; i++) {
            RCLtransmit(1,3);
            if (((code << (i-4)) & 2048) > 0) {
                RCLtransmit(1,3);
            } else {
                RCLtransmit(3,1);
            }
        }
        RCLtransmit(1,31);    
    }
}

void RCLtransmit(int nHighPulses, int nLowPulses) {
    digitalWrite(RCLpin, HIGH);
    delayMicroseconds( 350 * nHighPulses);
    digitalWrite(RCLpin, LOW);
    delayMicroseconds( 350 * nLowPulses);
}

