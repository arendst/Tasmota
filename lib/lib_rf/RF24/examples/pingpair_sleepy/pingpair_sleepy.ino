/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 
 TMRh20 2014 - Updates to the library allow sleeping both in TX and RX modes:
      TX Mode: The radio can be powered down (.9uA current) and the Arduino slept using the watchdog timer
      RX Mode: The radio can be left in standby mode (22uA current) and the Arduino slept using an interrupt pin
 */

/**
 * Example RF Radio Ping Pair which Sleeps between Sends
 *
 * This is an example of how to use the RF24 class to create a battery-
 * efficient system.  It is just like the GettingStarted_CallResponse example, but the
 * ping node powers down the radio and sleeps the MCU after every
 * ping/pong cycle, and the receiver sleeps between payloads.
 *
 * Write this sketch to two different nodes,
 * connect the role_pin to ground on one.  The ping node sends the current
 * time to the pong node, which responds by sending the value back.  The ping
 * node can then see how long the whole cycle took.
 */

#include <SPI.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"


// Set up nRF24L01 radio on SPI bus plus pins 7 & 8
RF24 radio(7,8);

// sets the role of this unit in hardware.  Connect to GND to be the 'pong' receiver
// Leave open to be the 'ping' transmitter
const int role_pin = 5;

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };   // Radio pipe addresses for the 2 nodes to communicate.

// Role management
// Set up role.  This sketch uses the same software for all the nodes
// in this system.  Doing so greatly simplifies testing.  The hardware itself specifies
// which node it is.

// The various roles supported by this sketch
typedef enum { role_ping_out = 1, role_pong_back } role_e;

// The debug-friendly names of those roles
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};

// The role of the current running sketch
role_e role;


// Sleep declarations
typedef enum { wdt_16ms = 0, wdt_32ms, wdt_64ms, wdt_128ms, wdt_250ms, wdt_500ms, wdt_1s, wdt_2s, wdt_4s, wdt_8s } wdt_prescalar_e;

void setup_watchdog(uint8_t prescalar);
void do_sleep(void);

const short sleep_cycles_per_transmission = 4;
volatile short sleep_cycles_remaining = sleep_cycles_per_transmission;



void setup(){

  // set up the role pin
  pinMode(role_pin, INPUT);
  digitalWrite(role_pin,HIGH);
  delay(20); // Just to get a solid reading on the role pin

  // read the address pin, establish our role
  if ( digitalRead(role_pin) )
    role = role_ping_out;
  else
    role = role_pong_back;

  Serial.begin(115200);
  printf_begin();
  Serial.print(F("\n\rRF24/examples/pingpair_sleepy/\n\rROLE: "));
  Serial.println(role_friendly_name[role]);

  // Prepare sleep parameters
  // Only the ping out role uses WDT.  Wake up every 4s to send a ping
  //if ( role == role_ping_out )
    setup_watchdog(wdt_4s);

  // Setup and configure rf radio

  radio.begin();

  // Open pipes to other nodes for communication

  // This simple sketch opens two pipes for these two nodes to communicate
  // back and forth.
  // Open 'our' pipe for writing
  // Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)

  if ( role == role_ping_out ) {
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);
  } else {
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);
  }

  // Start listening
  radio.startListening();

  // Dump the configuration of the rf unit for debugging
  //radio.printDetails();
}

void loop(){

  
  if (role == role_ping_out)  {                     // Ping out role.  Repeatedly send the current time
    radio.powerUp();                                // Power up the radio after sleeping
    radio.stopListening();                          // First, stop listening so we can talk.
                         
    unsigned long time = millis();                  // Take the time, and send it.                     
    Serial.print(F("Now sending... "));
    Serial.println(time);
    
    radio.write( &time, sizeof(unsigned long) );

    radio.startListening();                         // Now, continue listening
    
    unsigned long started_waiting_at = millis();    // Wait here until we get a response, or timeout (250ms)
    bool timeout = false;
    while ( ! radio.available()  ){
        if (millis() - started_waiting_at > 250 ){  // Break out of the while loop if nothing available
          timeout = true;
          break;
        }
    }
    
    if ( timeout ) {                                // Describe the results
        Serial.println(F("Failed, response timed out."));
    } else {
        unsigned long got_time;                     // Grab the response, compare, and send to debugging spew
        radio.read( &got_time, sizeof(unsigned long) );
    
        printf("Got response %lu, round-trip delay: %lu\n\r",got_time,millis()-got_time);
    }

    // Shut down the system
    delay(500);                     // Experiment with some delay here to see if it has an effect
                                    // Power down the radio.  
    radio.powerDown();              // NOTE: The radio MUST be powered back up again manually

                                    // Sleep the MCU.
      do_sleep();


  }


  // Pong back role.  Receive each packet, dump it out, and send it back
  if ( role == role_pong_back ) {
    
    if ( radio.available() ) {                                  // if there is data ready
      
        unsigned long got_time;
        while (radio.available()) {                             // Dump the payloads until we've gotten everything
          radio.read( &got_time, sizeof(unsigned long) );       // Get the payload, and see if this was the last one.
                                                                // Spew it.  Include our time, because the ping_out millis counter is unreliable
          printf("Got payload %lu @ %lu...",got_time,millis()); // due to it sleeping
        }
     
        radio.stopListening();                                  // First, stop listening so we can talk
        radio.write( &got_time, sizeof(unsigned long) );        // Send the final one back.
        Serial.println(F("Sent response."));
        radio.startListening();                                 // Now, resume listening so we catch the next packets.
    } else {
        Serial.println(F("Sleeping"));
        delay(50);                                             // Delay so the serial data can print out
        do_sleep();

    }
  }
}

void wakeUp(){
  sleep_disable();
}

// Sleep helpers

//Prescaler values
// 0=16ms, 1=32ms,2=64ms,3=125ms,4=250ms,5=500ms
// 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec

void setup_watchdog(uint8_t prescalar){

  uint8_t wdtcsr = prescalar & 7;
  if ( prescalar & 8 )
    wdtcsr |= _BV(WDP3);
  MCUSR &= ~_BV(WDRF);                      // Clear the WD System Reset Flag
  WDTCSR = _BV(WDCE) | _BV(WDE);            // Write the WD Change enable bit to enable changing the prescaler and enable system reset
  WDTCSR = _BV(WDCE) | wdtcsr | _BV(WDIE);  // Write the prescalar bits (how long to sleep, enable the interrupt to wake the MCU
}

ISR(WDT_vect)
{
  //--sleep_cycles_remaining;
  Serial.println(F("WDT"));
}

void do_sleep(void)
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  sleep_enable();
  attachInterrupt(0,wakeUp,LOW);
  WDTCSR |= _BV(WDIE);
  sleep_mode();                        // System sleeps here
                                       // The WDT_vect interrupt wakes the MCU from here
  sleep_disable();                     // System continues execution here when watchdog timed out  
  detachInterrupt(0);  
  WDTCSR &= ~_BV(WDIE);  
}
