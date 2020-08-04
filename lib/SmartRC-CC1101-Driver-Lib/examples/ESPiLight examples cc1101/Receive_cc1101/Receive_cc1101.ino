/*
 Basic ESPilight receive example

 https://github.com/puuu/espilight
 https://github.com/LSatan/SmartRC-CC1101-Driver-Lib
 ----------------------------------------------------------
 Mod by Little Satan. Have Fun!
 ----------------------------------------------------------
*/
#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <ESPiLight.h>

int RECEIVER_PIN;  // any intterupt able pin
int TRANSMITTER_PIN;

ESPiLight rf(TRANSMITTER_PIN);  // use -1 to disable transmitter

// callback function. It is called on successfully received and parsed rc signal
void rfCallback(const String &protocol, const String &message, int status,
                size_t repeats, const String &deviceID) {
  Serial.print("RF signal arrived [");
  Serial.print(protocol);  // protocoll used to parse
  Serial.print("][");
  Serial.print(deviceID);  // value of id key in json message
  Serial.print("] (");
  Serial.print(status);  // status of message, depending on repeat, either:
                         // FIRST   - first message of this protocoll within the
                         //           last 0.5 s
                         // INVALID - message repeat is not equal to the
                         //           previous message
                         // VALID   - message is equal to the previous message
                         // KNOWN   - repeat of a already valid message
  Serial.print(") ");
  Serial.print(message);  // message in json format
  Serial.println();

  // check if message is valid and process it
  if (status == VALID) {
    Serial.print("Valid message: [");
    Serial.print(protocol);
    Serial.print("] ");
    Serial.print(message);
    Serial.println();
  }
}

void setup() {
  Serial.begin(115200);

#ifdef ESP32
RECEIVER_PIN = 4; TRANSMITTER_PIN = 2;  // for esp32! Receiver on GPIO pin 4. Transmit on GPIO pin 2.
#elif ESP8266
RECEIVER_PIN = 4; TRANSMITTER_PIN = 5;  // for esp8266! Receiver on pin 4 = D2. Transmit on pin 5 = D1.
#else
RECEIVER_PIN = 0; TRANSMITTER_PIN = 6;  // for Arduino! Receiver on interrupt 0 => that is pin #2. Transmit on pin 6.
#endif

//CC1101 Settings:                (Settings with "//" are optional!)
  ELECHOUSE_cc1101.Init();            // must be set to initialize the cc1101!
//ELECHOUSE_cc1101.setRxBW(16);      // set Receive filter bandwidth (default = 812khz) 1 = 58khz, 2 = 67khz, 3 = 81khz, 4 = 101khz, 5 = 116khz, 6 = 135khz, 7 = 162khz, 8 = 203khz, 9 = 232khz, 10 = 270khz, 11 = 325khz, 12 = 406khz, 13 = 464khz, 14 = 541khz, 15 = 650khz, 16 = 812khz.
//ELECHOUSE_cc1101.setPA(10);       // set TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12)   Default is max!
  ELECHOUSE_cc1101.setMHZ(433.92); // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
  ELECHOUSE_cc1101.SetRx();       // set Receive on
 
  // set callback funktion
  rf.setCallback(rfCallback);
  // inittilize receiver
  rf.initReceiver(RECEIVER_PIN);
}

void loop() {
  // process input queue and may fire calllback
  rf.loop();
  delay(10);
}
