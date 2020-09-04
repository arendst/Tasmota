/*
 Copyright (C) 2012 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Example Nordic FOB Receiver 
 *
 * This is an example of how to use the RF24 class to receive signals from the
 * Sparkfun Nordic FOB.  Thanks to Kirk Mower for providing test hardware.
 *
 * See blog post at http://maniacbug.wordpress.com/2012/01/08/nordic-fob/
 */

#include <SPI.h>
#include <RF24.h>
#include "nRF24L01.h"
#include "printf.h"

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10

RF24 radio(9,10);

//
// Payload
//

struct payload_t
{
  uint8_t buttons;
  uint16_t id;
  uint8_t empty;
};

const char* button_names[] = { "Up", "Down", "Left", "Right", "Center" }; 
const int num_buttons = 5;

//
// Forward declarations
//

uint16_t flip_endian(uint16_t in);

//
// Setup
//

void setup(void)
{
  //
  // Print preamble
  //

  Serial.begin(115200);
  printf_begin();
  printf("\r\nRF24/examples/nordic_fob/\r\n");

  //
  // Setup and configure rf radio according to the built-in parameters
  // of the FOB.
  //

  radio.begin();
  radio.setChannel(2);
  radio.setPayloadSize(4);
  radio.setAutoAck(false);
  radio.setCRCLength(RF24_CRC_8);
  radio.openReadingPipe(1,0xE7E7E7E7E7LL);

  //
  // Start listening
  //

  radio.startListening();

  //
  // Dump the configuration of the rf unit for debugging
  //

  radio.printDetails();
}

//
// Loop
//

void loop(void)
{
  //
  // Receive each packet, dump it out
  //

    // if there is data ready
    if ( radio.available() )
    {
      // Get the packet from the radio
      payload_t payload;
      radio.read( &payload, sizeof(payload) );

      // Print the ID of this message.  Note that the message
      // is sent 'big-endian', so we have to flip it.
      printf("#%05u Buttons ",flip_endian(payload.id));

      // Print the name of each button 
      int i = num_buttons;
      while (i--)
      {
	if ( ! ( payload.buttons & _BV(i) ) )
	{
	  printf("%s ",button_names[i]);
	}
      }

      // If no buttons, print None
      if ( payload.buttons == _BV(num_buttons) - 1 )
	printf("None");

      printf("\r\n");
    }
}

//
// Helper functions
//

// Change a big-endian word into a little-endian
uint16_t flip_endian(uint16_t in)
{
  uint16_t low = in >> 8;
  uint16_t high = in << 8;

  return high | low;
}

// vim:cin:ai:sts=2 sw=2 ft=cpp
