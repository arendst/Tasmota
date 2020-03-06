/*
 Copyright (C) 2011 James Coliz, Jr. <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Full test on single RF pair
 *
 * This sketches uses as many RF24 methods as possible in a single test.
 *
 * To operate:
 *  Upload this sketch on two nodes, each with IRQ -> pin 2
 *  One node needs pin 7 -> GND, the other NC.  That's the receiving node
 *  Monitor the sending node's serial output
 *  Look for "+OK PASS" or "+OK FAIL"
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 8 & 9

RF24 radio(7,8);

// sets the role of this unit in hardware.  Connect to GND to be the 'pong' receiver
// Leave open to be the 'ping' transmitter
const short role_pin = 5;

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

//
// Payload
//

const int min_payload_size = 4;
const int max_payload_size = 32;
int payload_size_increments_by = 2;
int next_payload_size = min_payload_size;

char receive_payload[max_payload_size+1]; // +1 to allow room for a terminating NULL char

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

uint8_t pipe_number = 1; // Which pipe to send on.

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

//
// Setup 
//

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
  printf("\n\rRF24/tests/pingpair_test/\n\r");
  printf("ROLE: %s\n\r",role_friendly_name[role]);

  //
  // Read configuration from serial
  //
  // It would be a much better test if this program could accept configuration
  // from the serial port.  Then it would be possible to run the same test under
  // lots of different circumstances.
  //
  // The idea is that we will print "+READY" at this point.  The python script
  // will wait for it, and then send down a configuration script that we
  // execute here and then run with.
  //
  // The test controller will need to configure the receiver first, then go run
  // the test on the sender.
  //

  printf("+READY press any key to start\n\r\n\r");

  while (! Serial.available() ) {}
  configuration = Serial.read();
  printf("Configuration\t = %c\n\r",configuration);

  //
  // Setup and configure rf radio
  //

  radio.begin();

  // We will be using the Ack Payload feature, so please enable it
  radio.enableAckPayload();

  // Config 2 is special radio config
  if (configuration=='2')
  {
    radio.setCRCLength(RF24_CRC_8);
    radio.setDataRate(RF24_250KBPS);
    radio.setChannel(10);
  }
  else
  {
    //Otherwise, default radio config
    
    // Optional: Increase CRC length for improved reliability
    radio.setCRCLength(RF24_CRC_16);

    // Optional: Decrease data rate for improved reliability
    radio.setDataRate(RF24_1MBPS);

    // Optional: Pick a high channel
    radio.setChannel(90);
  }

  // Config 3 is static payloads only
  if (configuration == '3')
  {
    next_payload_size = 16;
    payload_size_increments_by = 0;
    radio.setPayloadSize(next_payload_size);
  }
  else
  {
    // enable dynamic payloads
    radio.enableDynamicPayloads();
  }

  // Config 4 tests out a higher pipe ##
  if (configuration == '4' && role == role_sender)
  {
    // Set top 4 bytes of the address in pipe 1 
    radio.openReadingPipe(1,pipe & 0xFFFFFFFF00ULL);

    // indicate the pipe to use 
    pipe_number = 5;
  }
  else if ( role == role_sender )
  {
    radio.openReadingPipe(5,0);
  }

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
    radio.openReadingPipe(pipe_number,pipe);
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
  delay(50);
  if ( role == role_receiver )
    printf("\n\r+OK ");
}

//
// Print buffer
//
// Printing from the interrupt handler is a bad idea, so we print from there
// to this intermediate buffer
//

char prbuf[1000];
char *prbuf_end = prbuf + sizeof(prbuf);
char *prbuf_in = prbuf;
char *prbuf_out = prbuf;

//
// Loop 
//

static uint32_t message_count = 0;
static uint32_t last_message_count = 0;

void loop(void)
{
  //
  // Sender role.  Repeatedly send the current time
  //

  if (role == role_sender && !done)
  {
    // The payload will always be the same, what will change is how much of it we send.
    static char send_payload[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ789012";

    // First, stop listening so we can talk.
    radio.stopListening();

    // Send it.  This will block until complete
    printf("\n\rNow sending length %i...",next_payload_size);
    radio.startWrite( send_payload, next_payload_size,0 );

    // Update size for next time.
    next_payload_size += payload_size_increments_by;
    if ( next_payload_size > max_payload_size )
      next_payload_size = min_payload_size;
    
    // Try again soon
    delay(interval);
    
    // Timeout if we have not received anything back ever
    if ( ! last_message_count && millis() > interval * 100 )
    {
      printf("No responses received.  Are interrupts connected??\n\r");
      done = true;
    }
  }

  //
  // Receiver role: Does nothing!  All the work is in IRQ
  //
  
  //
  // Spew print buffer
  //

  size_t write_length = prbuf_in - prbuf_out;
  if ( write_length )
  {
    Serial.write(reinterpret_cast<uint8_t*>(prbuf_out),write_length);
    prbuf_out += write_length;
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

void check_radio(void)
{
  // What happened?
  bool tx,fail,rx;
  radio.whatHappened(tx,fail,rx);

  // Have we successfully transmitted?
  if ( tx )
  {
    if ( role == role_sender )
      prbuf_in += sprintf(prbuf_in,"Send:OK ");

    if ( role == role_receiver )
      prbuf_in += sprintf(prbuf_in,"Ack Payload:Sent\n\r");
  }

  // Have we failed to transmit?
  if ( fail )
  {
    if ( role == role_sender )
    {
      prbuf_in += sprintf(prbuf_in,"Send:Failed ");

      // log status of this line
      one_failed();
    }

    if ( role == role_receiver )
      prbuf_in += sprintf(prbuf_in,"Ack Payload:Failed\n\r");
  }

  // Not powering down since radio is in standby mode
  //if ( ( tx || fail ) && ( role == role_sender ) )
    //radio.powerDown();

  // Did we receive a message?
  if ( rx )
  {
    // If we're the sender, we've received an ack payload
    if ( role == role_sender )
    {
      radio.read(&message_count,sizeof(message_count));
      prbuf_in += sprintf(prbuf_in,"Ack:%lu ",message_count);
 
      // is this ack what we were expecting?  to account
      // for failures, we simply want to make sure we get a
      // DIFFERENT ack every time.
      if ( ( message_count != last_message_count ) || ( configuration=='3' && message_count == 16 ) )
      {
	prbuf_in += sprintf(prbuf_in,"OK ");
	one_ok();
      }
      else
      {
	prbuf_in += sprintf(prbuf_in,"FAILED ");
	one_failed();
      }
      last_message_count = message_count;
    }

    // If we're the receiver, we've received a time message
    if ( role == role_receiver )
    {
      // Get this payload and dump it
      size_t len = max_payload_size;
      memset(receive_payload,0,max_payload_size);
      
      if ( configuration == '3' ){
	len = next_payload_size;
      }else{
	len = radio.getDynamicPayloadSize();
      }
      
      radio.read( receive_payload, len );
      
      // Put a zero at the end for easy printing
      receive_payload[len] = 0;

      // Spew it
      prbuf_in += sprintf(prbuf_in,"Recv size=%i val=%s len=%u\n\r",len,receive_payload,strlen(receive_payload));

      // Add an ack packet for the next time around.
      // Here we will report back how many bytes we got this time.
      radio.writeAckPayload( pipe_number, &len, sizeof(len) );

      ++message_count;
    }

  }
}
