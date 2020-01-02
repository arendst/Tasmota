/*
 Copyright (C) 2011 James Coliz, Jr. <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Interrupt-driven test for native target
 *
 * This example is the friendliest for the native target because it doesn't do
 * any polling.  Made a slight change to call done() at the end of setup.
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 8 & 9

RF24 radio(8,9);

// sets the role of this unit in hardware.  Connect to GND to be the 'pong' receiver
// Leave open to be the 'ping' transmitter
const short role_pin = 7;

//
// Topology
//

// Single radio pipe address for the 2 nodes to communicate.
const uint64_t pipe = 0xE8E8F0F0E1LL;

//
// Role management
//
// Set up role.  This sketch uses the same software for all the nodes in this
// system.  Doing so greatly simplifies testing.  The hardware itself specifies
// which node it is.
//
// This is done through the role_pin
//

// The various roles supported by this sketch
typedef enum { role_sender = 1, role_receiver } role_e;

// The debug-friendly names of those roles
const char* role_friendly_name[] = { "invalid", "Sender", "Receiver"};

// The role of the current running sketch
role_e role;

// Interrupt handler, check the radio because we got an IRQ
void check_radio(void);

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
    role = role_sender;
  else
    role = role_receiver;

  //
  // Print preamble
  //

  Serial.begin(115200);
  printf_begin();
  printf("\n\rRF24/examples/pingpair_irq/\n\r");
  printf("ROLE: %s\n\r",role_friendly_name[role]);

  //
  // Setup and configure rf radio
  //

  radio.begin();

  // We will be using the Ack Payload feature, so please enable it
  radio.enableAckPayload();

  //
  // Open pipes to other nodes for communication
  //

  // This simple sketch opens a single pipe for these two nodes to communicate
  // back and forth.  One listens on it, the other talks to it.

  if ( role == role_sender )
  {
    radio.openWritingPipe(pipe);
  }
  else
  {
    radio.openReadingPipe(1,pipe);
  }

  //
  // Start listening
  //

  if ( role == role_receiver )
    radio.startListening();

  //
  // Dump the configuration of the rf unit for debugging
  //

  radio.printDetails();

  //
  // Attach interrupt handler to interrupt #0 (using pin 2)
  // on BOTH the sender and receiver
  //

  attachInterrupt(0, check_radio, FALLING);

  //
  // On the native target, this is as far as we get
  //
#if NATIVE
  done();
#endif
}

static uint32_t message_count = 0;

void loop(void)
{
  //
  // Sender role.  Repeatedly send the current time
  //

  if (role == role_sender)
  {
    // Take the time, and send it.
    unsigned long time = millis();
    printf("Now sending %lu\n\r",time);
    radio.startWrite( &time, sizeof(unsigned long) );

    // Try again soon
    delay(2000);
  }

  //
  // Receiver role: Does nothing!  All the work is in IRQ
  //

}

void check_radio(void)
{
  // What happened?
  bool tx,fail,rx;
  radio.whatHappened(tx,fail,rx);

  // Have we successfully transmitted?
  if ( tx )
  {
    if ( role == role_sender )
      printf("Send:OK\n\r");

    if ( role == role_receiver )
      printf("Ack Payload:Sent\n\r");
  }

  // Have we failed to transmit?
  if ( fail )
  {
    if ( role == role_sender )
      printf("Send:Failed\n\r");

    if ( role == role_receiver )
      printf("Ack Payload:Failed\n\r");
  }

  // Transmitter can power down for now, because
  // the transmission is done.
  if ( ( tx || fail ) && ( role == role_sender ) )
    radio.powerDown();

  // Did we receive a message?
  if ( rx )
  {
    // If we're the sender, we've received an ack payload
    if ( role == role_sender )
    {
      radio.read(&message_count,sizeof(message_count));
      printf("Ack:%lu\n\r",(unsigned long)message_count);
    }

    // If we're the receiver, we've received a time message
    if ( role == role_receiver )
    {
      // Get this payload and dump it
      static unsigned long got_time;
      radio.read( &got_time, sizeof(got_time) );
      printf("Got payload %lu\n\r",got_time);

      // Add an ack packet for the next time around.  This is a simple
      // packet counter
      radio.writeAckPayload( 1, &message_count, sizeof(message_count) );
      ++message_count;
    }
  }
}

// vim:ai:cin:sts=2 sw=2 ft=cpp
