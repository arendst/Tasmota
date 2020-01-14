/*
TMRh20 2014

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/** Reliably transmitting large volumes of data with a low signal or in noisy environments
 * This example demonstrates data transfer functionality with the use of auto-retry
 and auto-reUse functionality enabled. This sketch demonstrates how a user can extend
 the auto-retry functionality to any chosen time period, preventing data loss and ensuring
 the consistency of data.
 
 This sketh demonstrates use of the writeBlocking() functionality, and extends the standard
 retry functionality of the radio. Payloads will be auto-retried until successful or the
 extended timeout period is reached.
 */



#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

/*************  USER Configuration *****************************/

RF24 radio(7,8);                        // Set up nRF24L01 radio on SPI bus plus pins 7 & 8
unsigned long timeoutPeriod = 3000;     // Set a user-defined timeout period. With auto-retransmit set to (15,15) retransmission will take up to 60ms and as little as 7.5ms with it set to (1,15).
                                        // With a timeout period of 1000, the radio will retry each payload for up to 1 second before giving up on the transmission and starting over

/***************************************************************/

const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };   // Radio pipe addresses for the 2 nodes to communicate.

byte data[32];                           //Data buffer

volatile unsigned long counter;
unsigned long rxTimer,startTime, stopTime, payloads = 0;  
bool TX=1,RX=0,role=0, transferInProgress = 0;


void setup(void) {

  Serial.begin(115200);
  printf_begin();

  radio.begin();                           // Setup and configure rf radio
  radio.setChannel(1);                     // Set the channel
  radio.setPALevel(RF24_PA_LOW);           // Set PA LOW for this demonstration. We want the radio to be as lossy as possible for this example.
  radio.setDataRate(RF24_1MBPS);           // Raise the data rate to reduce transmission distance and increase lossiness
  radio.setAutoAck(1);                     // Ensure autoACK is enabled
  radio.setRetries(2,15);                  // Optionally, increase the delay between retries. Want the number of auto-retries as high as possible (15)
  radio.setCRCLength(RF24_CRC_16);         // Set CRC length to 16-bit to ensure quality of data
  radio.openWritingPipe(pipes[0]);         // Open the default reading and writing pipe
  radio.openReadingPipe(1,pipes[1]);       
  
  radio.startListening();                 // Start listening
  radio.printDetails();                   // Dump the configuration of the rf unit for debugging
  
  printf("\n\rRF24/examples/Transfer Rates/\n\r");
  printf("*** PRESS 'T' to begin transmitting to the other node\n\r");
  
  randomSeed(analogRead(0));              //Seed for random number generation  
  for(int i=0; i<32; i++){
     data[i] = random(255);               //Load the buffer with random data
  }
  radio.powerUp();                        //Power up the radio

  
}



void loop(void){


  if(role == TX){    
    delay(2000);                                              // Pause for a couple seconds between transfers    
    printf("Initiating Extended Timeout Data Transfer\n\r");

    unsigned long cycles = 1000;                              // Change this to a higher or lower number. This is the number of payloads that will be sent.   
    
    unsigned long transferCMD[] = {'H','S',cycles };          // Indicate to the other radio that we are starting, and provide the number of payloads that will be sent 
    radio.writeFast(&transferCMD,12);                         // Send the transfer command
    if(radio.txStandBy(timeoutPeriod)){                       // If transfer initiation was successful, do the following
   
        startTime = millis();                                 // For calculating transfer rate
        boolean timedOut = 0;                                 // Boolean for keeping track of failures
      
        for(int i=0; i<cycles; i++){                          // Loop through a number of cycles
          data[0] = i;                                        // Change the first byte of the payload for identification
       
          if(!radio.writeBlocking(&data,32,timeoutPeriod)){   // If retries are failing and the user defined timeout is exceeded
              timedOut = 1;                                   // Indicate failure
              counter = cycles;                               // Set the fail count to maximum
              break;                                          // Break out of the for loop
          }  
        }    
  
   
        stopTime = millis();                                  // Capture the time of completion or failure
   
       //This should be called to wait for completion and put the radio in standby mode after transmission, returns 0 if data still in FIFO (timed out), 1 if success
       if(timedOut){ radio.txStandBy(); }                     //Partially blocking standby, blocks until success or max retries. FIFO flushed if auto timeout reached
       else{ radio.txStandBy(timeoutPeriod);     }            //Standby, block until FIFO empty (sent) or user specified timeout reached. FIFO flushed if user timeout reached.
   
   }else{                                             
       Serial.println("Communication not established");       //If unsuccessful initiating transfer, exit and retry later
   } 
    
   float rate = cycles * 32 / (stopTime - startTime);         //Display results:
    
   Serial.print("Transfer complete at "); Serial.print(rate); printf(" KB/s \n\r");
   Serial.print(counter);
   Serial.print(" of ");
   Serial.print(cycles); Serial.println(" Packets Failed to Send");
   counter = 0;   
    
   }
  
  
  
if(role == RX){  
  
  if(!transferInProgress){                       // If a bulk data transfer has not been started
     if(radio.available()){                      
        radio.read(&data,32);                    //Read any available payloads for analysis

        if(data[0] == 'H' && data[4] == 'S'){    // If a bulk data transfer command has been received
          payloads = data[8];                    // Read the first two bytes of the unsigned long. Need to read the 3rd and 4th if sending more than 65535 payloads
          payloads |= data[9] << 8;              // This is the number of payloads that will be sent
          counter = 0;                           // Reset the payload counter to 0
          transferInProgress = 1;                // Indicate it has started
          startTime = rxTimer = millis();        // Capture the start time to measure transfer rate and calculate timeouts
        }
     }
  }else{
     if(radio.available()){                     // If in bulk transfer mode, and a payload is available
       radio.read(&data,32);                    // Read the payload
       rxTimer = millis();                      // Reset the timeout timer
       counter++;                               // Keep a count of received payloads
     }else
     if(millis() - rxTimer > timeoutPeriod){    // If no data available, check the timeout period
       Serial.println("Transfer Failed");       // If per-payload timeout exceeeded, end the transfer
       transferInProgress = 0; 
     }else
     if(counter >= payloads){                   // If the specified number of payloads is reached, transfer is completed
      startTime = millis() - startTime;         // Calculate the total time spent during transfer
      float numBytes = counter*32;              // Calculate the number of bytes transferred
      Serial.print("Rate: ");                   // Print the transfer rate and number of payloads
      Serial.print(numBytes/startTime);
      Serial.println(" KB/s");
      printf("Payload Count: %d \n\r", counter);
      transferInProgress = 0;                   // End the transfer as complete
    }     
  }
  
   
  }
  
  //
  // Change roles
  //

  if ( Serial.available() )
  {
    char c = toupper(Serial.read());
    if ( c == 'T' && role == RX )
    {
      printf("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK\n\r");
      radio.openWritingPipe(pipes[1]);
      radio.openReadingPipe(1,pipes[0]);
      radio.stopListening();
      role = TX;                  // Become the primary transmitter (ping out)
    }
    else if ( c == 'R' && role == TX )
    {
      radio.openWritingPipe(pipes[0]);
      radio.openReadingPipe(1,pipes[1]); 
      radio.startListening();
      printf("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK\n\r");      
      role = RX;                // Become the primary receiver (pong back)
    }
  }
}


