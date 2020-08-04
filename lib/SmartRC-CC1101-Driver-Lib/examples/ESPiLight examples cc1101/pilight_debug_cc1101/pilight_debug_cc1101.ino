/*
 Basic ESPiLight pilight_debug example

 This example mimic the output of the piligh_debug tool.

 https://github.com/puuu/espilight
 https://github.com/LSatan/SmartRC-CC1101-Driver-Lib
----------------------------------------------------------
  Mod by Little Satan. Have Fun!
----------------------------------------------------------
*/
#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <ESPiLight.h>

#define PULSE_DIV 34

int RECEIVER_PIN;  // any intterupt able pin
int TRANSMITTER_PIN;

ESPiLight rf(TRANSMITTER_PIN);  // use -1 to disable transmitter

unsigned int normalize(unsigned int i, unsigned int pulselen) {
  double x;
  x = (double)i / pulselen;

  return (unsigned int)(round(x));
}

// callback function. It is called on successfully received and parsed rc signal
void rfRawCallback(const uint16_t* pulses, size_t length) {
  uint16_t pulse;
  uint16_t pulselen = pulses[length - 1] / PULSE_DIV;
  if (pulselen > 25) {
    for (unsigned int i = 3; i < length; i++) {
      if ((pulses[i] / pulselen) >= 2) {
        pulse = pulses[i];
        break;
      }
    }
    if (normalize(pulse, pulselen) > 0 && length > 25) {
      /* Print everything */
      Serial.println("--[RESULTS]--");
      Serial.println();
      Serial.print("time:\t\t");
      Serial.print(millis());
      Serial.println(" ms");
      Serial.println("hardware:\tESPiLight");
      Serial.print("pulse:\t\t");
      Serial.println(normalize(pulse, pulselen));
      Serial.print("rawlen:\t\t");
      Serial.println(length);
      Serial.printf("pulselen:\t");
      Serial.println(pulselen);
      Serial.println();
      Serial.println("Raw code:");
      for (unsigned int i = 0; i < length; i++) {
        Serial.print(pulses[i]);
        Serial.print(" ");
      }
      Serial.println();
    }
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
  
  // set callback funktion for raw messages
  rf.setPulseTrainCallBack(rfRawCallback);
  // inittilize receiver
  rf.initReceiver(RECEIVER_PIN);

  Serial.println(
      "Press and hold one of the buttons on your remote or wait until");
  Serial.println("another device such as a weather station has sent new codes");
  Serial.println(
      "The debugger will automatically reset itself after one second of");
  Serial.println(
      "failed leads. It will keep running until you explicitly stop it.");
}

void loop() {
  // process input queue and may fire calllback
  rf.loop();
  delay(10);
}
