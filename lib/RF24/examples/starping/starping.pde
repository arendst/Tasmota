/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Example RF Radio Ping Star Group
 *
 * This sketch is a more complex example of using the RF24 library for Arduino.
 * Deploy this on up to six nodes.  Set one as the 'pong receiver' by tying the
 * role_pin low, and the others will be 'ping transmit' units.  The ping units
 * unit will send out the value of millis() once a second.  The pong unit will
 * respond back with a copy of the value.  Each ping unit can get that response
 * back, and determine how long the whole cycle took.
 *
 * This example requires a bit more complexity to determine which unit is which.
 * The pong receiver is identified by having its role_pin tied to ground.
 * The ping senders are further differentiated by a byte in eeprom.
 */

#include <SPI.h>
#include <EEPROM.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10

RF24 radio(9,10);

// sets the role of this unit in hardware.  Connect to GND to be the 'pong' receiver
// Leave open to be the 'pong' receiver.
const int role_pin = 7;

//
// Topology
//

// Radio pipe addresses for the nodes to communicate.  Only ping nodes need
// dedicated pipes in this topology.  Each ping node has a talking pipe
// that it will ping into, and a listening pipe that it will listen for
// the pong.  The pong node listens on all the ping node talking pipes
// and sends the pong back on the sending node's specific listening pipe.

const uint64_t talking_pipes[5] = { 0xF0F0F0F0D2LL, 0xF0F0F0F0C3LL, 0xF0F0F0F0B4LL, 0xF0F0F0F0A5LL, 0xF0F0F0F096LL };
const uint64_t listening_pipes[5] = { 0x3A3A3A3AD2LL, 0x3A3A3A3AC3LL, 0x3A3A3A3AB4LL, 0x3A3A3A3AA5LL, 0x3A3A3A3A96LL };

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
typedef enum { role_invalid = 0, role_ping_out, role_pong_back } role_e;

// The debug-friendly names of those roles
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};

// The role of the current running sketch
role_e role;

//
// Address management
//

// Where in EEPROM is the address stored?
const uint8_t address_at_eeprom_location = 0;

// What is our address (SRAM cache of the address from EEPROM)
// Note that zero is an INVALID address.  The pong back unit takes address
// 1, and the rest are 2-6
uint8_t node_address;

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
  // Address
  //

  if ( role == role_pong_back )
    node_address = 1;
  else
  {
    // Read the address from EEPROM
    uint8_t reading = EEPROM.read(address_at_eeprom_location);

    // If it is in a valid range for node addresses, it is our
    // address.
    if ( reading >= 2 && reading <= 6 )
      node_address = reading;

    // Otherwise, it is invalid, so set our address AND ROLE to 'invalid'
    else
    {
      node_address = 0;
      role = role_invalid;
    }
  }

  //
  // Print preamble
  //

  Serial.begin(115200);
  printf_begin();
  printf("\n\rRF24/examples/starping/\n\r");
  printf("ROLE: %s\n\r",role_friendly_name[role]);
  printf("ADDRESS: %i\n\r",node_address);

  //
  // Setup and configure rf radio
  //

  radio.begin();

  //
  // Open pipes to other nodes for communication
  //

  // The pong node listens on all the ping node talking pipes
  // and sends the pong back on the sending node's specific listening pipe.
  if ( role == role_pong_back )
  {
    radio.openReadingPipe(1,talking_pipes[0]);
    radio.openReadingPipe(2,talking_pipes[1]);
    radio.openReadingPipe(3,talking_pipes[2]);
    radio.openReadingPipe(4,talking_pipes[3]);
    radio.openReadingPipe(5,talking_pipes[4]);
  }

  // Each ping node has a talking pipe that it will ping into, and a listening
  // pipe that it will listen for the pong.
  if ( role == role_ping_out )
  {
    // Write on our talking pipe
    radio.openWritingPipe(talking_pipes[node_address-2]);
    // Listen on our listening pipe
    radio.openReadingPipe(1,listening_pipes[node_address-2]);
  }

  //
  // Start listening
  //

  radio.startListening();

  //
  // Dump the configuration of the rf unit for debugging
  //

  radio.printDetails();

  //
  // Prompt the user to assign a node address if we don't have one
  //

  if ( role == role_invalid )
  {
    printf("\n\r*** NO NODE ADDRESS ASSIGNED *** Send 1 through 6 to assign an address\n\r");
  }
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
      if (millis() - started_waiting_at > 250 )
        timeout = true;

    // Describe the results
    if ( timeout )
    {
      printf("Failed, response timed out.\n\r");
    }
    else
    {
      // Grab the response, compare, and send to debugging spew
      unsigned long got_time;
      radio.read( &got_time, sizeof(unsigned long) );

      // Spew it
      printf("Got response %lu, round-trip delay: %lu\n\r",got_time,millis()-got_time);
    }

    // Try again 1s later
    delay(1000);
  }

  //
  // Pong back role.  Receive each packet, dump it out, and send it back
  //

  if ( role == role_pong_back )
  {
    // if there is data ready
    uint8_t pipe_num;
    if ( radio.available(&pipe_num) )
    {
      // Dump the payloads until we've gotten everything
      unsigned long got_time;
      bool done = false;
      while (!done)
      {
        // Fetch the payload, and see if this was the last one.
        done = radio.read( &got_time, sizeof(unsigned long) );

        // Spew it
        printf("Got payload %lu from node %i...",got_time,pipe_num+1);
      }

      // First, stop listening so we can talk
      radio.stopListening();

      // Open the correct pipe for writing
      radio.openWritingPipe(listening_pipes[pipe_num-1]);

      // Retain the low 2 bytes to identify the pipe for the spew
      uint16_t pipe_id = listening_pipes[pipe_num-1] & 0xffff;

      // Send the final one back.
      radio.write( &got_time, sizeof(unsigned long) );
      printf("Sent response to %04x.\n\r",pipe_id);

      // Now, resume listening so we catch the next packets.
      radio.startListening();
    }
  }

  //
  // Listen for serial input, which is how we set the address
  //
  if (Serial.available())
  {
    // If the character on serial input is in a valid range...
    char c = Serial.read();
    if ( c >= '1' && c <= '6' )
    {
      // It is our address
      EEPROM.write(address_at_eeprom_location,c-'0');

      // And we are done right now (no easy way to soft reset)
      printf("\n\rManually reset address to: %c\n\rPress RESET to continue!",c);
      while(1) ;
    }
  }
}
// vim:ai:ci sts=2 sw=2 ft=cpp
