/*
TMRh20 2014

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/** General Data Transfer Rate Test
 * This example demonstrates basic data transfer functionality with the
 updated library. This example will display the transfer rates acheived using
 the slower form of high-speed transfer using blocking-writes.
 */

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <RF24/RF24.h>
#include <unistd.h>

using namespace std;
//
// Hardware configuration
//

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

/**************************************************************/

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t addresses[2] = { 0xABCDABCD71LL, 0x544d52687CLL };


uint8_t data[32];
unsigned long startTime, stopTime, counter, rxTimer=0;

void intHandler()
{
	//Read as long data is available
	//Single interrupts may be lost if a lot of data comes in.
	while(radio.available())
	{
      radio.read(&data,32);
      counter++;
    }
}

int main(int argc, char** argv){

  bool role_ping_out = 1, role_pong_back = 0;
  bool role = 0;

  // Print preamble:

  cout << "RF24/examples/Transfer/\n";

  radio.begin();                           // Setup and configure rf radio
  radio.setChannel(1);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_1MBPS);
  radio.setAutoAck(1);                     // Ensure autoACK is enabled
  radio.setRetries(2,15);                  // Optionally, increase the delay between retries & # of retries
  radio.setCRCLength(RF24_CRC_8);          // Use 8-bit CRC for performance
  radio.printDetails();
/********* Role chooser ***********/

  printf("\n ************ Role Setup ***********\n");
  string input = "";
  char myChar = {0};
  cout << "Choose a role: Enter 0 for receiver, 1 for transmitter (CTRL+C to exit)\n>";
  getline(cin,input);

  attachInterrupt(23, INT_EDGE_FALLING, intHandler); //Attach interrupt to bcm pin 23

  if(input.length() == 1) {
	myChar = input[0];
	if(myChar == '0'){
		cout << "Role: Pong Back, awaiting transmission " << endl << endl;
	}else{  cout << "Role: Ping Out, starting transmission " << endl << endl;
		role = role_ping_out;
	}
  }
/***********************************/

    if ( role == role_ping_out )    {
      radio.openWritingPipe(addresses[1]);
      radio.openReadingPipe(1,addresses[0]);
	  radio.stopListening();
    } else {
      radio.openWritingPipe(addresses[0]);
      radio.openReadingPipe(1,addresses[1]);
      radio.startListening();
    }


  for(int i=0; i<32; i++){
     data[i] = rand() % 255;               			//Load the buffer with random data
  }

    // forever loop
    while (1)
	{
	if (role == role_ping_out){
		sleep(2);
		printf("Initiating Basic Data Transfer\n\r");

		long int cycles = 10000; 					//Change this to a higher or lower number.
		
		// unsigned long pauseTime = millis();		//Uncomment if autoAck == 1 ( NOACK )
		startTime = millis();
	
		for(int i=0; i<cycles; i++){        		//Loop through a number of cycles
      			data[0] = i;                        //Change the first byte of the payload for identification
      			if(!radio.writeFast(&data,32)){     //Write to the FIFO buffers
        			counter++;                      //Keep count of failed payloads
      			}

				
				//This is only required when NO ACK ( enableAutoAck(0) ) payloads are used
		/*		if(millis() - pauseTime > 3){       // Need to drop out of TX mode every 4ms if sending a steady stream of multicast data
					pauseTime = millis();		    
					radio.txStandBy();				// This gives the PLL time to sync back up	
				}
		*/
		}
		stopTime = millis();

		if(!radio.txStandBy()){ counter+=3; }

  		float numBytes = cycles*32;
   		float rate = numBytes / (stopTime - startTime);

  		printf("Transfer complete at %.2f KB/s \n\r",rate);
   		printf("%lu of %lu Packets Failed to Send\n\r",counter,cycles);
  		counter = 0;

	}


	if(role == role_pong_back)
	{
        if(millis() - rxTimer > 1000){
		  rxTimer = millis();
		  printf("Rate: ");
		  float numBytes = counter*32;
		  printf("%.2f KB/s \n\r",numBytes/1000);
		  printf("Payload Count: %lu \n\r", counter);
  		  counter = 0;
        }
        delay(2);
	}
	
} // loop
} // main
