/*
TMRh20 2014 - Updated to work with optimized RF24 Arduino library
*/


/**
 * Example for efficient call-response using ack-payloads
 *
 * This example continues to make use of all the normal functionality of the radios including
 * the auto-ack and auto-retry features, but allows ack-payloads to be written optionlly as well.
 * This allows very fast call-response communication, with the responding radio never having to
 * switch out of Primary Receiver mode to send back a payload, but having the option to switch to
 * primary transmitter if wanting to initiate communication instead of respond to a commmunication.
 */

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <RF24/RF24.h>

using namespace std;

//
// Hardware configuration
// Configure the appropriate pins for your connections

/****************** Raspberry Pi ***********************/

// Radio CE Pin, CSN Pin, SPI Speed
// See http://www.airspayce.com/mikem/bcm2835/group__constants.html#ga63c029bd6500167152db4e57736d0939 and the related enumerations for pin information.

// Setup for GPIO 22 CE and CE0 CSN with SPI Speed @ 4Mhz
//RF24 radio(RPI_V2_GPIO_P1_22, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_4MHZ);

// NEW: Setup for RPi B+
//RF24 radio(RPI_BPLUS_GPIO_J8_15,RPI_BPLUS_GPIO_J8_24, BCM2835_SPI_SPEED_8MHZ);

// Setup for GPIO 15 CE and CE0 CSN with SPI Speed @ 8Mhz
RF24 radio(RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ);

/*** RPi Alternate ***/
//Note: Specify SPI BUS 0 or 1 instead of CS pin number.
// See http://tmrh20.github.io/RF24/RPi.html for more information on usage

//RPi Alternate, with MRAA
//RF24 radio(15,0);

//RPi Alternate, with SPIDEV - Note: Edit RF24/arch/BBB/spi.cpp and  set 'this->device = "/dev/spidev0.0";;' or as listed in /dev
//RF24 radio(22,0);


/****************** Linux (BBB,x86,etc) ***********************/

// See http://tmrh20.github.io/RF24/pages.html for more information on usage
// See http://iotdk.intel.com/docs/master/mraa/ for more information on MRAA
// See https://www.kernel.org/doc/Documentation/spi/spidev for more information on SPIDEV

// Setup for ARM(Linux) devices like BBB using spidev (default is "/dev/spidev1.0" )
//RF24 radio(115,0);

//BBB Alternate, with mraa
// CE pin = (Header P9, Pin 13) = 59 = 13 + 46 
//Note: Specify SPI BUS 0 or 1 instead of CS pin number. 
//RF24 radio(59,0);

/********** User Config *********/
// Assign a unique identifier for this node, 0 or 1. Arduino example uses radioNumber 0 by default.
bool radioNumber = 1;

/********************************/


// Radio pipe addresses for the 2 nodes to communicate.
const uint8_t addresses[][6] = {"1Node","2Node"};

bool role_ping_out = 1, role_pong_back = 0, role = 0;
uint8_t counter = 1;                                                          // A single byte to keep track of the data being sent back and forth


int main(int argc, char** argv){


  cout << "RPi/RF24/examples/gettingstarted_call_response\n";
  radio.begin();
  radio.enableAckPayload();               // Allow optional ack payloads
  radio.enableDynamicPayloads();
  radio.printDetails();                   // Dump the configuration of the rf unit for debugging


/********* Role chooser ***********/

  printf("\n ************ Role Setup ***********\n");
  string input = "";
  char myChar = {0};
  cout << "Choose a role: Enter 0 for pong_back, 1 for ping_out (CTRL+C to exit)\n>";
  getline(cin,input);

  if(input.length() == 1) {
	myChar = input[0];
	if(myChar == '0'){
		cout << "Role: Pong Back, awaiting transmission " << endl << endl;
	}else{  cout << "Role: Ping Out, starting transmission " << endl << endl;
		role = role_ping_out;
	}
  }
/***********************************/
  // This opens two pipes for these two nodes to communicate
  // back and forth.
    if ( !radioNumber )    {
      radio.openWritingPipe(addresses[0]);
      radio.openReadingPipe(1,addresses[1]);
    }else{
      radio.openWritingPipe(addresses[1]);
      radio.openReadingPipe(1,addresses[0]);
    }
	radio.startListening();
	radio.writeAckPayload(1,&counter,1);

// forever loop
while (1){


/****************** Ping Out Role ***************************/

  if (role == role_ping_out){                               // Radio is in ping mode

    uint8_t gotByte;                                        // Initialize a variable for the incoming response

    radio.stopListening();                                  // First, stop listening so we can talk.
    printf("Now sending %d as payload. ",counter);          // Use a simple byte counter as payload
    unsigned long time = millis();                          // Record the current microsecond count

    if ( radio.write(&counter,1) ){                         // Send the counter variable to the other radio
        if(!radio.available()){                             // If nothing in the buffer, we got an ack but it is blank
            printf("Got blank response. round-trip delay: %lu ms\n\r",millis()-time);
        }else{
            while(radio.available() ){                      // If an ack with payload was received
                radio.read( &gotByte, 1 );                  // Read it, and display the response time
                printf("Got response %d, round-trip delay: %lu ms\n\r",gotByte,millis()-time);
                counter++;                                  // Increment the counter variable
            }
        }

    }else{        printf("Sending failed.\n\r"); }          // If no ack response, sending failed

    sleep(1);  // Try again later
  }

/****************** Pong Back Role ***************************/

  if ( role == role_pong_back ) {
    uint8_t pipeNo, gotByte;           		        // Declare variables for the pipe and the byte received
    if( radio.available(&pipeNo)){               	// Read all available payloads      
      radio.read( &gotByte, 1 );
													// Since this is a call-response. Respond directly with an ack payload.
	  gotByte += 1;  								// Ack payloads are much more efficient than switching to transmit mode to respond to a call
	  radio.writeAckPayload(pipeNo,&gotByte, 1 );   // This can be commented out to send empty payloads.	  
      printf("Loaded next response %d \n\r", gotByte);
	  delay(900); //Delay after a response to minimize CPU usage on RPi
				  //Expects a payload every second      
   }
 }

} //while 1
} //main


