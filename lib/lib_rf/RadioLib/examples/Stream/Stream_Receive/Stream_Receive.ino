/*
   RadioLib Stream Receive Example

   This example shows how to receive data in "Stream" mode.
   In this mode, arbitrary length of data may be sent, up to
   "infinite" continuous transmission between two devices.

   Caveats:
    - CRC of the payload is not supported
    - the length of the payload must be known in advance

   Modules that can be used for Stream are:
    - SX127x/RFM9x (FSK mode only)
    - RF69
    - SX1231

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx127xrfm9x---lora-modem

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

// include the library
#include <RadioLib.h>

// SX1278 has the following connections:
// NSS pin:   10
// DIO0 pin:  2
// RESET pin: 9
// DIO1 pin:  3
SX1278 radio = new Module(10, 2, 9, 3);

// or using RadioShield
// https://github.com/jgromes/RadioShield
//SX1278 radio = RadioShield.ModuleA;

void setup() {
  Serial.begin(9600);

  // initialize SX1278 with default settings
  Serial.print(F("[SX1278] Initializing ... "));
  int state = radio.beginFSK();
  
  // when using one of the non-LoRa modules for Stream transmit
  // (RF69, CC1101, Si4432 etc.), use the basic begin() method
  // int state = radio.begin();
  
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // set the function that will be called
  // when receive buffer is full
  radio.setFifoFullAction(fifoGet);

  // fixed packet length mode is required
  radio.fixedPacketLengthMode(0);

  // start listening for packets
  Serial.print(F("[SX1278] Starting to listen ... "));
  state = radio.startReceive();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // if needed, 'listen' mode can be disabled by calling
  // any of the following methods:
  //
  // radio.standby()
  // radio.sleep()
  // radio.transmit();
  // radio.receive();
  // radio.readData();
  // radio.scanChannel();
}

// flag to indicate that a packet was received
volatile bool receivedFlag = false;

// how many bytes are there in total
const int totalLength = 512;

// counter to keep track of how many bytes have been received so far
volatile int receivedLength = 0;

// buffer to save the received data into
volatile uint8_t rxBuffer[totalLength + 1];

// this function is called when the radio receive buffer
// is full and ready to be read
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
#if defined(ESP8266) || defined(ESP32)
  ICACHE_RAM_ATTR
#endif
void fifoGet(void) {
  // set the flag when we receive the full packet
  receivedFlag = radio.fifoGet(rxBuffer, totalLength, &receivedLength);
}

void loop() {
  // check if the flag is set
  if(receivedFlag) {
    // packet was successfully received
    Serial.println(F("[SX1278] Received packet!"));

    // print data of the packet
    Serial.print(F("[SX1278] Data:\t\t"));
    Serial.println((char*)rxBuffer);
      
    // reset flag
    receivedFlag = false;
    receivedLength = 0;
    
    // put module back to listen mode
    radio.startReceive();
  }
}
