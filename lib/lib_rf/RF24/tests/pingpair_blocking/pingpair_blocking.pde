/*
 Copyright (C) 2011 James Coliz, Jr. <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

//
// Test version of RF24, exposes some protected interface
//

class RF24Test: public RF24
{
public: RF24Test(int a, int b): RF24(a,b) {}
};


//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 8 & 9

RF24Test radio(48,49);

// sets the role of this unit in hardware.  Connect to GND to be the 'pong' receiver
// Leave open to be the 'ping' transmitter
const int role_pin = 5;

//
// Topology
//

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

//
// Role management
//
// Set up role.  This sketch uses the same software for all the nodes
// in this system.  Doing so greatly simplifies testing.  The hardware itself specifies
// which node it is.
//
// This is done through the role_pin
//

// The various roles supported by this sketch
typedef enum { role_ping_out = 1, role_pong_back } role_e;

// The debug-friendly names of those roles
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};

// The role of the current running sketch
role_e role;

//
// Test state
//

bool done; //*< Are we done with the test? */
bool passed; //*< Have we passed the test? */
bool notified; //*< Have we notified the user we're done? */
const int num_needed = 10; //*< How many success/failures until we're done? */
int receives_remaining = num_needed; //*< How many ack packets until we declare victory? */
int failures_remaining = num_needed; //*< How many more failed sends until we declare failure? */
const int interval = 100; //*< ms to wait between sends */

char configuration = '1'; //*< Configuration key, one char sent in by the test framework to tell us how to configure, this is the default */

void one_ok(void)
{
  // Have we received enough yet?
  if ( ! --receives_remaining )
  {
    done = true;
    passed = true;
  }
}

void one_failed(void)
{
  // Have we failed enough yet?
  if ( ! --failures_remaining )
  {
    done = true;
    passed = false;
  }
}

void setup(void)
{
  //
  // Role
  //

  // set up the role pin
  pinMode(role_pin, INPUT);
  digitalWrite(role_pin,HIGH);
  delay(20); // Just to get a solid reading on the role pin

  // read the address pin, establish our role
  if ( digitalRead(role_pin) )
    role = role_ping_out;
  else
    role = role_pong_back;

  //
  // Print preamble
  //

  Serial.begin(115200);
  printf_begin();
  printf("\n\rRF24/tests/pingpair_blocking/\n\r");
  printf("ROLE: %s\n\r",role_friendly_name[role]);

  //
  // get test config
  //

  printf("+READY press any key to start\n\r\n\r");

  while (! Serial.available() ) {}
  configuration = Serial.read();
  printf("Configuration\t = %c\n\r",configuration);
   
  //
  // Setup and configure rf radio
  //

  radio.begin();

  //
  // Open pipes to other nodes for communication
  //

  // This simple sketch opens two pipes for these two nodes to communicate
  // back and forth.
  // Open 'our' pipe for writing
  // Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)

  if ( role == role_ping_out )
  {
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);
  }
  else
  {
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);
  }

  //
  // Start listening
  //

  radio.startListening();

  //
  // Dump the configuration of the rf unit for debugging
  //

  radio.printDetails();
  
  if ( role == role_pong_back )
    printf("\n\r+OK ");
}

void loop(void)
{
  //
  // Ping out role.  Repeatedly send the current time
  //

  if (role == role_ping_out)
  {
    // First, stop listening so we can talk.
    radio.stopListening();

    // Take the time, and send it.  This will block until complete
    unsigned long time = millis();
    printf("Now sending %lu...",time);
    radio.write( &time, sizeof(unsigned long) );

    // Now, continue listening
    radio.startListening();

    // Wait here until we get a response, or timeout (250ms)
    unsigned long started_waiting_at = millis();
    bool timeout = false;
    while ( ! radio.available() && ! timeout )
      if (millis() - started_waiting_at > 200 )
        timeout = true;

    // Describe the results
    if ( timeout )
    {
      printf("Failed, response timed out.\n\r");
      one_failed();
    }
    else
    {
      // Grab the response, compare, and send to debugging spew
      unsigned long got_time;
      radio.read( &got_time, sizeof(unsigned long) );

      // Spew it
      printf("Got response %lu, round-trip delay: %lu\n\r",got_time,millis()-got_time);
      one_ok();
    }

    // Try again  later
    delay(250);
  }

  //
  // Pong back role.  Receive each packet, dump it out, and send it back
  //

  if ( role == role_pong_back )
  {
    // if there is data ready
    if ( radio.available() )
    {
      // Dump the payloads until we've gotten everything
      unsigned long got_time;
      bool done = false;
      while (radio.available())
      {
        // Fetch the payload, and see if this was the last one.
        radio.read( &got_time, sizeof(unsigned long) );
      }
	// Delay just a little bit to let the other unit
	// make the transition to receiver
	//delay(20);
      //}

      // First, stop listening so we can talk
      radio.stopListening();
      
              // Spew it
        printf("Got payload %lu...",got_time);

      // Send the final one back.
      radio.write( &got_time, sizeof(unsigned long) );

      // Now, resume listening so we catch the next packets.
      radio.startListening();
      printf("Sent response.\n\r");

    }
  }
  
  //
  // Stop the test if we're done and report results
  //
  if ( done && ! notified )
  {
    notified = true;

    printf("\n\r+OK ");
    if ( passed )
      printf("PASS\n\r\n\r");
    else
      printf("FAIL\n\r\n\r");
  }
}
// vim:cin:ai:sts=2 sw=2 ft=cpp
