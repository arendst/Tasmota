/*
 * This sketch simply repeats data received from remote weather sensors made by Cresta.
 * 
 * Setup:
 *  - connect digital output of a 433MHz receiver to digital pin 2 of Arduino.
 *  - connect transmitter input of a 433MHz transmitter to digital pin 11
 *  - An LED on pin 13 will tell you if and when a signal has been received and transmitted.
 *
 * Library:
 * https://github.com/mattwire/arduino-dev/tree/master/libraries/RemoteSensor
 * https://github.com/LSatan/SmartRC-CC1101-Driver-Lib
 *
 */

#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <SensorReceiver.h>
#include <SensorTransmitter.h>

int LED_PIN;
int pinRx; // int for Receive pin.
int pinTx; // int for Transmit pin.

void setup() {

#ifdef ESP32
pinRx = 4; pinTx = 2;  // for esp32! Receiver on GPIO pin 4. Transmit on GPIO pin 2.
LED_PIN = 32; // set led on GPIO pin 32.
#elif ESP8266
pinRx = 4; pinTx = 5;  // for esp8266! Receiver on pin 4 = D2. Transmit on pin 5 = D1.
LED_PIN = 16; // set led on pin 16 = D0.
#else
pinRx = 0; pinTx = 6;  // for Arduino! Receiver on interrupt 0 => that is pin #2. Transmit on pin 6.
LED_PIN = 4;  // set led on pin D4.
#endif 

//CC1101 Settings:                (Settings with "//" are optional!)
  ELECHOUSE_cc1101.Init();            // must be set to initialize the cc1101!
//ELECHOUSE_cc1101.setRxBW(16);      // set Receive filter bandwidth (default = 812khz) 1 = 58khz, 2 = 67khz, 3 = 81khz, 4 = 101khz, 5 = 116khz, 6 = 135khz, 7 = 162khz, 8 = 203khz, 9 = 232khz, 10 = 270khz, 11 = 325khz, 12 = 406khz, 13 = 464khz, 14 = 541khz, 15 = 650khz, 16 = 812khz.
//ELECHOUSE_cc1101.setPA(10);       // set TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12)   Default is max!
  ELECHOUSE_cc1101.setMHZ(433.92); // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.

  pinMode(LED_PIN, OUTPUT);
  
  // Since we're not instantiating SensorTransmitter, but only use the static methods of SensorTransmitter,
  // the pin mode must be set manually.
  pinMode(pinTx, OUTPUT); 

  // When no signal has been received, the LED is lit.
  digitalWrite(LED_PIN, HIGH);   

  // Init the receiver on interrupt pin 0 (digital pin 2).
  // Set the callback to function "retransmit", which is called
  // whenever valid sensor data has been received.
  ELECHOUSE_cc1101.SetRx(); // set Receive on
  SensorReceiver::init(pinRx, retransmit);    
}

void loop() {
}

void retransmit(byte *data) {  
  // Data received

  // Wait a second after a receiving. There's little point for decoding and sending the same signal multiple times.  
  SensorReceiver::disable();
  interrupts(); // delay() requires that interrupts are enabled
  delay(1000);    

  // Flash LED when transmitting. 
  digitalWrite(LED_PIN, HIGH);

  // Transmit signal. Note: this is a static method, no object required!
  ELECHOUSE_cc1101.SetTx(); // set Transmit on
  SensorTransmitter::sendPackage(pinTx, data);

  digitalWrite(LED_PIN, LOW);

  noInterrupts();
  ELECHOUSE_cc1101.SetRx(); // set Receive on
  SensorReceiver::enable();     
}
