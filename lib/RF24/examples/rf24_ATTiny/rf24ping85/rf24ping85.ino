/*
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
version 2 as published by the Free Software Foundation.

    rf24ping85.ino by tong67 ( https://github.com/tong67 )
    This is an example of how to use the RF24 class to communicate with ATtiny85 and other node.
    Write this sketch to an ATtiny85. It will act like the 'transmit' mode of GettingStarted.ino
    Write GettingStarted.ino sketch to UNO (or other board or RPi) and put the node in 'receiver' mode.
    The ATtiny85 will transmit a counting number every second starting from 1.
    The ATtiny85 uses the tiny-core by CodingBadly (https://code.google.com/p/arduino-tiny/)
    When direct use of 3v3 does not work (UNO boards have bad 3v3 line) use 5v with LED (1.8V ~ 2.2V drop)
    For low power consumption solutions floating pins (SCK and MOSI) should be pulled high or low with eg. 10K

    ** Hardware configuration **
    ATtiny25/45/85 Pin map with CE_PIN 3 and CSN_PIN 4
                                 +-\/-+
                   NC      PB5  1|o   |8  Vcc --- nRF24L01  VCC, pin2 --- LED --- 5V
    nRF24L01  CE, pin3 --- PB3  2|    |7  PB2 --- nRF24L01  SCK, pin5
    nRF24L01 CSN, pin4 --- PB4  3|    |6  PB1 --- nRF24L01 MOSI, pin7
    nRF24L01 GND, pin1 --- GND  4|    |5  PB0 --- nRF24L01 MISO, pin6
                                 +----+

    ATtiny25/45/85 Pin map with CE_PIN 3 and CSN_PIN 3 => PB3 and PB4 are free to use for application
    Circuit idea from http://nerdralph.blogspot.ca/2014/01/nrf24l01-control-with-3-attiny85-pins.html
    Original RC combination was 1K/100nF. 22K/10nF combination worked better.
	For best settletime delay value in RF24::csn() the timingSearch3pin.ino scatch can be used.
    This configuration is enabled when CE_PIN and CSN_PIN are equal, e.g. both 3
    Because CE is always high the power consumption is higher than for 5 pins solution
                                                                                            ^^
                                 +-\/-+           nRF24L01   CE, pin3 ------|              //
                           PB5  1|o   |8  Vcc --- nRF24L01  VCC, pin2 ------x----------x--|<|-- 5V
                   NC      PB3  2|    |7  PB2 --- nRF24L01  SCK, pin5 --|<|---x-[22k]--|  LED
                   NC      PB4  3|    |6  PB1 --- nRF24L01 MOSI, pin6  1n4148 |
    nRF24L01 GND, pin1 -x- GND  4|    |5  PB0 --- nRF24L01 MISO, pin7         |
                        |        +----+                                       |
                        |-----------------------------------------------||----x-- nRF24L01 CSN, pin4 
                                                                       10nF

    ATtiny24/44/84 Pin map with CE_PIN 8 and CSN_PIN 7
	Schematic provided and successfully tested by Carmine Pastore (https://github.com/Carminepz)
                                  +-\/-+
    nRF24L01  VCC, pin2 --- VCC  1|o   |14 GND --- nRF24L01  GND, pin1
                            PB0  2|    |13 AREF
                            PB1  3|    |12 PA1
                            PB3  4|    |11 PA2 --- nRF24L01   CE, pin3
                            PB2  5|    |10 PA3 --- nRF24L01  CSN, pin4
                            PA7  6|    |9  PA4 --- nRF24L01  SCK, pin5
    nRF24L01 MOSI, pin7 --- PA6  7|    |8  PA5 --- nRF24L01 MISO, pin6
                                  +----+
*/

// CE and CSN are configurable, specified values for ATtiny85 as connected above
#define CE_PIN 3
#define CSN_PIN 4
//#define CSN_PIN 3 // uncomment for ATtiny85 3 pins solution

#include "RF24.h"

RF24 radio(CE_PIN, CSN_PIN);

byte addresses[][6] = {
  "1Node","2Node"};
unsigned long payload = 0;

void setup() {
  // Setup and configure rf radio
  radio.begin(); // Start up the radio
  radio.setAutoAck(1); // Ensure autoACK is enabled
  radio.setRetries(15,15); // Max delay between retries & number of retries
  radio.openWritingPipe(addresses[1]); // Write to device address '2Node'
  radio.openReadingPipe(1,addresses[0]); // Read on pipe 1 for device address '1Node'
  radio.startListening(); // Start listening
}

void loop(void){
  
  radio.stopListening(); // First, stop listening so we can talk.
  payload++;
  radio.write( &payload, sizeof(unsigned long) );
  radio.startListening(); // Now, continue listening

    unsigned long started_waiting_at = micros(); // Set up a timeout period, get the current microseconds
  boolean timeout = false; // Set up a variable to indicate if a response was received or not

  while ( !radio.available() ){ // While nothing is received
    if (micros() - started_waiting_at > 200000 ){ // If waited longer than 200ms, indicate timeout and exit while loop
      timeout = true;
      break;
    }

  }

  if ( !timeout ){ // Describe the results
    unsigned long got_time; // Grab the response, compare, and send to debugging spew
    radio.read( &got_time, sizeof(unsigned long) );
  }

  // Try again 1s later
  delay(1000);
}