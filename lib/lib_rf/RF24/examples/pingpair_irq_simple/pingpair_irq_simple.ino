/*
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 
 Created Dec 2014 - TMRh20
 */

/**
 * Example of using interrupts
 *
 * This is a very simple example of using two devices to communicate using interrupts.
 * With multiple devices, each device would need to have a separate reading pipe
 */

#include <SPI.h>
#include "RF24.h"
#include <printf.h>

// Hardware configuration
// Set up nRF24L01 radio on SPI bus plus pins 7 & 8
RF24 radio(7,8);
                                        
// Use the same address for both devices
uint8_t address[] = { "radio" };

// Simple messages to represent a 'ping' and 'pong'
uint8_t ping = 111;
uint8_t pong = 222;

volatile uint32_t round_trip_timer = 0;


/********************** Setup *********************/

void setup(){

  Serial.begin(115200);
  Serial.println(F("Simple pingpair example"));
  Serial.println(F("Send a 'T' via Serial to transmit a single 'ping' "));
  //printf_begin();
  
  // Setup and configure rf radio
  radio.begin();

  // Use dynamic payloads to improve response time
  radio.enableDynamicPayloads();
  radio.openWritingPipe(address);             // communicate back and forth.  One listens on it, the other talks to it.
  radio.openReadingPipe(1,address); 
  radio.startListening();
  
  //radio.printDetails();                             // Dump the configuration of the rf unit for debugging

  attachInterrupt(0, check_radio, LOW);             // Attach interrupt handler to interrupt #0 (using pin 2) on BOTH the sender and receiver
}



/********************** Main Loop *********************/
void loop() {

  if(Serial.available()){
    switch(toupper(Serial.read())){
      case 'T': 
                // Only allow 1 transmission per 45ms to prevent overlapping IRQ/reads/writes
                // Default retries = 5,15 = ~20ms per transmission max
                while(micros() - round_trip_timer < 45000){
                  //delay between writes 
                }
                Serial.print(F("Sending Ping"));
                radio.stopListening();                
                round_trip_timer = micros();
                radio.startWrite( &ping, sizeof(uint8_t),0 );
                break;    
    }
  }  
}

/********************** Interrupt *********************/

void check_radio(void)                                // Receiver role: Does nothing!  All the work is in IRQ
{
  
  bool tx,fail,rx;
  radio.whatHappened(tx,fail,rx);                     // What happened?

 
  // If data is available, handle it accordingly
  if ( rx ){
    
    if(radio.getDynamicPayloadSize() < 1){
      // Corrupt payload has been flushed
      return; 
    }
    // Read in the data
    uint8_t received;
    radio.read(&received,sizeof(received));

    // If this is a ping, send back a pong
    if(received == ping){
      radio.stopListening();
      // Normal delay will not work here, so cycle through some no-operations (16nops @16mhz = 1us delay)
      for(uint32_t i=0; i<130;i++){
         __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
      }
      radio.startWrite(&pong,sizeof(pong),0);
      Serial.print("pong");
    }else    
    // If this is a pong, get the current micros()
    if(received == pong){
      round_trip_timer = micros() - round_trip_timer;
      Serial.print(F("Received Pong, Round Trip Time: "));
      Serial.println(round_trip_timer);
    }
  }

  // Start listening if transmission is complete
  if( tx || fail ){
     radio.startListening(); 
     Serial.println(tx ? F(":OK") : F(":Fail"));
  }  
}