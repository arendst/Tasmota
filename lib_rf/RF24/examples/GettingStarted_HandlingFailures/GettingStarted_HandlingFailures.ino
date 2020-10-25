
/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* but modified to include failure handling.
* 
* The nrf24l01+ radios are fairly reliable devices, but on breadboards etc, with inconsistent wiring, failures may
* occur randomly after many hours to days or weeks. This sketch demonstrates how to handle the various failures and
* keep the radio operational.
* 
* The three main failure modes of the radio include:
* Writing to radio: Radio unresponsive - Fixed internally by adding a timeout to the internal write functions in RF24 (failure handling)
* Reading from radio: Available returns true always - Fixed by adding a timeout to available functions by the user. This is implemented internally in  RF24Network.
* Radio configuration settings are lost - Fixed by monitoring a value that is different from the default, and re-configuring the radio if this setting reverts to the default.
* 
* The printDetails output should appear as follows for radio #0:
* 
* STATUS         = 0x0e RX_DR=0 TX_DS=0 MAX_RT=0 RX_P_NO=7 TX_FULL=0
* RX_ADDR_P0-1   = 0x65646f4e31 0x65646f4e32
* RX_ADDR_P2-5   = 0xc3 0xc4 0xc5 0xc6
* TX_ADDR        = 0x65646f4e31
* RX_PW_P0-6     = 0x20 0x20 0x00 0x00 0x00 0x00
* EN_AA          = 0x3f
* EN_RXADDR      = 0x02
* RF_CH          = 0x4c
* RF_SETUP       = 0x03
* CONFIG         = 0x0f
* DYNPD/FEATURE  = 0x00 0x00
* Data Rate      = 1MBPS
* Model          = nRF24L01+
* CRC Length     = 16 bits
* PA Power       = PA_LOW
*
*Users can use this sketch to troubleshoot radio module wiring etc. as it makes the radios hot-swapable
*
* Updated: 2019 by TMRh20
*/

#include <SPI.h>
#include "RF24.h"
#include "printf.h"

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 0;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
/**********************************************************/

byte addresses[][6] = {"1Node","2Node"};

// Used to control whether this node is sending or receiving
bool role = 0;

/**********************************************************/
//Function to configure the radio
void configureRadio(){

  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
  if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
  
  // Start the radio listening for data
  radio.startListening();
  radio.printDetails();
}

/**********************************************************/

void setup() {
  Serial.begin(115200);
  Serial.println(F("RF24/examples/GettingStarted"));
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));

  printf_begin();
  
  configureRadio();  
}

uint32_t configTimer =  millis();

void loop() {
  
  if(radio.failureDetected){
    radio.failureDetected = false;
    delay(250);
    Serial.println("Radio failure detected, restarting radio");
    configureRadio();        
  }
  //Every 5 seconds, verify the configuration of the radio. This can be done using any 
  //setting that is different from the radio defaults.
  if(millis() - configTimer > 5000){
    configTimer = millis();
    if(radio.getDataRate() != RF24_1MBPS){
      radio.failureDetected = true;
      Serial.print("Radio configuration error detected");
    }
  }
  
/****************** Ping Out Role ***************************/  
if (role == 1)  {
    
    radio.stopListening();                                    // First, stop listening so we can talk.
    
    
    Serial.println(F("Now sending"));

    unsigned long start_time = micros();                             // Take the time, and send it.  This will block until complete
     if (!radio.write( &start_time, sizeof(unsigned long) )){
       Serial.println(F("failed"));
     }
        
    radio.startListening();                                    // Now, continue listening
    
    unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
    boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not
    
    while ( ! radio.available() ){                             // While nothing is received
      if (micros() - started_waiting_at > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
          timeout = true;
          break;
      }      
    }
        
    if ( timeout ){                                             // Describe the results
        Serial.println(F("Failed, response timed out."));
    }else{
        unsigned long got_time;                                 // Grab the response, compare, and send to debugging spew
        
        //Failure Handling:
        uint32_t failTimer = millis();
        while(radio.available()){                               //If available always returns true, there is a problem
          if(millis() - failTimer > 250){
            radio.failureDetected = true;
            Serial.println("Radio available failure detected");
            break;
          }
          radio.read( &got_time, sizeof(unsigned long) );
        }
        unsigned long end_time = micros();
        
        // Spew it
        Serial.print(F("Sent "));
        Serial.print(start_time);
        Serial.print(F(", Got response "));
        Serial.print(got_time);
        Serial.print(F(", Round-trip delay "));
        Serial.print(end_time-start_time);
        Serial.println(F(" microseconds"));
    }

    // Try again 1s later
    delay(1000);
  }



/****************** Pong Back Role ***************************/

  if ( role == 0 )
  {
    unsigned long got_time;
    
    if( radio.available()){
      
      uint32_t failTimer = millis();                                                         // Variable for the received timestamp
      while (radio.available()) {                                 // While there is data ready
        if(millis()-failTimer > 500){
          Serial.println("Radio available failure detected");
          radio.failureDetected = true;
          break;
        }
        radio.read( &got_time, sizeof(unsigned long) );             // Get the payload
      }
     
      radio.stopListening();                                        // First, stop listening so we can talk   
      radio.write( &got_time, sizeof(unsigned long) );              // Send the final one back.      
      radio.startListening();                                       // Now, resume listening so we catch the next packets.     
      Serial.print(F("Sent response "));
      Serial.println(got_time);  
   }
 }




/****************** Change Roles via Serial Commands ***************************/

  if ( Serial.available() )
  {
    char c = toupper(Serial.read());
    if ( c == 'T' && role == 0 ){      
      Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
      role = 1;                  // Become the primary transmitter (ping out)
    
   }else
    if ( c == 'R' && role == 1 ){
      Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));      
       role = 0;                // Become the primary receiver (pong back)
       radio.startListening();
       
    }
  }


} // Loop
