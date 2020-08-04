/*
  Example for analyzing and proposing unknown new protocols

  Requires modified rc-switch branch "protocollessreceiver"
  with ReceivedInverted() function exposed.
  
  https://github.com/Martin-Laclaustra/rc-switch/tree/protocollessreceiver
  https://github.com/LSatan/SmartRC-CC1101-Driver-Lib

  ----------------------------------------------------------
  CC1101 Mod by Little Satan. Have Fun!
  ----------------------------------------------------------
*/

#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <RCSwitch.h>

int pin; // int for Receive pin.

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(9600);

#ifdef ESP32
pin = 4;  // for esp32! Receiver on GPIO pin 4. 
#elif ESP8266
pin = 4;  // for esp8266! Receiver on pin 4 = D2.
#else
pin = 0;  // for Arduino! Receiver on interrupt 0 => that is pin #2
#endif    

//CC1101 Settings:                (Settings with "//" are optional!)
  ELECHOUSE_cc1101.Init();            // must be set to initialize the cc1101!
//ELECHOUSE_cc1101.setRxBW(16);      // set Receive filter bandwidth (default = 812khz) 1 = 58khz, 2 = 67khz, 3 = 81khz, 4 = 101khz, 5 = 116khz, 6 = 135khz, 7 = 162khz, 8 = 203khz, 9 = 232khz, 10 = 270khz, 11 = 325khz, 12 = 406khz, 13 = 464khz, 14 = 541khz, 15 = 650khz, 16 = 812khz.
//ELECHOUSE_cc1101.setPA(10);       // set TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12)   Default is max!
  ELECHOUSE_cc1101.setMHZ(433.92); // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.

  mySwitch.enableReceive(pin);  // Receiver on

  ELECHOUSE_cc1101.SetRx();  // set Receive on
  
}

void loop() {
  if (mySwitch.available()) {
    
    int value = mySwitch.getReceivedValue();
    
    if (value == 0) {
      Serial.print("Unknown encoding");
    } else {
      
      int databuffer[64]; // get a copy of the received timings before they are overwritten
      int numberoftimings = 2 * mySwitch.getReceivedBitlength() + 2;
      if(numberoftimings > 64) numberoftimings = 64;
      for (int i = 0; i < numberoftimings; i++) {
        databuffer[i] = mySwitch.getReceivedRawdata()[i];
      }

      Serial.print("Received ");
      Serial.print( mySwitch.getReceivedValue() );
      Serial.print(" / ");
      Serial.print( mySwitch.getReceivedBitlength() );
      Serial.print("bit ");
      Serial.print("Protocol: ");
      Serial.println( mySwitch.getReceivedProtocol() );
      
      unsigned int databitsoffset = abs( (int)mySwitch.getReceivedLevelInFirstTiming() - (int)mySwitch.getReceivedInverted());
      //Serial.println( mySwitch.getReceivedLevelInFirstTiming() );
      //Serial.println( mySwitch.getReceivedInverted() );
      //Serial.println( databitsoffset );
      unsigned long dataduration = 0;
      for (unsigned int i = 1 + databitsoffset; i < numberoftimings - 1 + databitsoffset; i++) {
        dataduration += databuffer[i];
      }
      Serial.print("data bits of pulse train duration: ");
      Serial.println( dataduration );
      unsigned int averagebitduration = (int)(0.5 + ((double)dataduration)/mySwitch.getReceivedBitlength());
      unsigned int protocolratio = (unsigned int)(0.5 + ((double)(averagebitduration - mySwitch.getReceivedDelay())) / (double)mySwitch.getReceivedDelay());
      Serial.print("proposed protocol: { ");
      Serial.print(mySwitch.getReceivedDelay());
      Serial.print(", { ");
      Serial.print( (databitsoffset==0) ? 
        (int) (0.5 + (double)databuffer[2*mySwitch.getReceivedBitlength()+1]/(double)mySwitch.getReceivedDelay())
        :
        (int) (0.5 + (double)databuffer[0]/(double)mySwitch.getReceivedDelay())
        );
      Serial.print(", ");
      Serial.print( (databitsoffset==0) ?
        (int) (0.5 + (double)databuffer[0]/(double)mySwitch.getReceivedDelay())
        :
        (int) (0.5 + (double)databuffer[1]/(double)mySwitch.getReceivedDelay())
        );
      Serial.print(" }, { ");
      Serial.print("1");
      Serial.print(", ");
      Serial.print(protocolratio);
      Serial.print(" }, { ");
      Serial.print(protocolratio);
      Serial.print(", ");
      Serial.print("1");
      Serial.print(" }, ");
      Serial.print((mySwitch.getReceivedInverted()) ? "true" : "false" );
      Serial.println(" }");

      // raw signal
      Serial.println("====");
      Serial.print("first level ");
      Serial.println((mySwitch.getReceivedLevelInFirstTiming() == 0) ? "down" : "up" );
      for (int i = 0; i < 2*mySwitch.getReceivedBitlength()+2 - 1 + databitsoffset; i++) {
        Serial.print(databuffer[i]);
        Serial.print(" ");
        if((i - databitsoffset) % 16 == 0) Serial.println("");
      }
      if ((2*mySwitch.getReceivedBitlength()+2 - 1 + databitsoffset - 1) % 16 != 0) Serial.println("");
      if (databitsoffset != 1) Serial.println(databuffer[2*mySwitch.getReceivedBitlength()+1]);
      // plot signal in spreadsheet
      Serial.println("====");
     
    }

    mySwitch.resetAvailable();
  }
}
