/*
 Basic ESPiLight transmit example

 https://github.com/puuu/espilight
 https://github.com/LSatan/SmartRC-CC1101-Driver-Lib
 ----------------------------------------------------------
 Mod by Little Satan. Have Fun!
 ----------------------------------------------------------
*/
#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <ESPiLight.h>

int TRANSMITTER_PIN;

void setup() {
  Serial.begin(115200);

#ifdef ESP32
TRANSMITTER_PIN = 2;  // for esp32! Transmit on GPIO pin 2.
#elif ESP8266
TRANSMITTER_PIN = 5;  // for esp8266! Transmit on pin 5 = D1.
#else
TRANSMITTER_PIN = 6;  // for Arduino! Transmit on pin 6.
#endif

 //CC1101 Settings:                (Settings with "//" are optional!)
  ELECHOUSE_cc1101.Init();            // must be set to initialize the cc1101!
//ELECHOUSE_cc1101.setRxBW(16);      // set Receive filter bandwidth (default = 812khz) 1 = 58khz, 2 = 67khz, 3 = 81khz, 4 = 101khz, 5 = 116khz, 6 = 135khz, 7 = 162khz, 8 = 203khz, 9 = 232khz, 10 = 270khz, 11 = 325khz, 12 = 406khz, 13 = 464khz, 14 = 541khz, 15 = 650khz, 16 = 812khz.
//ELECHOUSE_cc1101.setPA(10);       // set TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12)   Default is max!
  ELECHOUSE_cc1101.setMHZ(433.92); // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
  ELECHOUSE_cc1101.SetTx();       // cc1101 set Transmit on
  }

// Toggle state of elro 800 switch evrey 2 s
void loop() {
  ESPiLight rf(TRANSMITTER_PIN);
  rf.send("elro_800_switch", "{\"systemcode\":17,\"unitcode\":1,\"on\":1}");
  delay(2000);
  rf.send("elro_800_switch", "{\"systemcode\":17,\"unitcode\":1,\"off\":1}");
  delay(2000);
}
