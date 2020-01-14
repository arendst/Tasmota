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

RF24 radio(22,0);

/********** User Config *********/
// Assign a unique identifier for this node, 0 or 1. Arduino example uses radioNumber 0 by default.
bool radioNumber = 1;
int interruptPin = 23;
/********************************/


// Radio pipe addresses for the 2 nodes to communicate.
const uint8_t addresses[][6] = {"1Node","2Node"};

bool role_ping_out = 1, role_pong_back = 0, role = 0;
uint8_t counter = 1;                                                          // A single byte to keep track of the data being sent back and forth
uint32_t timer = 0;

void intHandler(){
  
  bool tx_ok,tx_fail,rx;
  radio.whatHappened(tx_ok,tx_fail,rx);
  
  if(tx_fail){
      printf("Sending failed.\n\r");
  }
  
  if(role == role_ping_out && tx_ok){
    if(!radio.available()){
      printf("Got blank response. round-trip delay: %u ms\n\r",millis()-timer);
    }
  }
  
  if(role == role_ping_out ){
    while(radio.available() ){
      uint8_t gotByte;
      radio.read( &gotByte, 1 );
      printf("Got response %d, round-trip delay: %u ms\n\r",gotByte,millis()-timer);
      counter++;
    }
  }
    
  if ( role == role_pong_back){
    if(tx_ok) {
        printf("Ack Payload Sent\n");
    }  
    uint8_t pipeNo, gotByte;
    if( radio.available(&pipeNo)){   
      radio.read( &gotByte, 1 );

	  gotByte += 1;
	  radio.writeAckPayload(pipeNo,&gotByte, 1 );
      printf("Loaded next response %d \n\r", gotByte);
   
   }
 }
    
}


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
    
    attachInterrupt(interruptPin, INT_EDGE_FALLING, intHandler); //Attach interrupt to bcm pin 23
    
// forever loop
while (1){


/****************** Ping Out Role ***************************/

  if (role == role_ping_out){                               // Radio is in ping mode

    //uint8_t gotByte;                                      // Initialize a variable for the incoming response

    radio.stopListening();                                  // First, stop listening so we can talk.
    printf("Now sending %d as payload. ",counter);          // Use a simple byte counter as payload
    timer = millis();                                       // Record the current microsecond count

    radio.startWrite(&counter,1,false);                         // Send the counter variable to the other radio
    sleep(1);  // Try again later
  }

/****************** Pong Back Role ***************************/



} //while 1
} //main


