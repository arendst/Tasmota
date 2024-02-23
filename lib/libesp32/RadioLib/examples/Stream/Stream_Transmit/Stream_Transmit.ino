/*
   RadioLib Stream Transmit Example

   This example shows how to transmit data in "Stream" mode.
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

// save transmission state between loops
int transmissionState = RADIOLIB_ERR_NONE;

// this packet is much longer than would normally fit
// into SX1278's internal buffer
String longPacket = "Lorem ipsum dolor sit amet, consectetur adipiscing elit.\
 Maecenas at urna ut nunc imperdiet laoreet. Aliquam erat volutpat.\
 Etiam mattis mauris vitae posuere tincidunt. In sit amet bibendum nisl,\
 a ultrices lorem. Duis hendrerit ultricies condimentum. Phasellus eget nisi\
 eget massa aliquam bibendum. Pellentesque ante neque, aliquam non diam non,\
 fringilla facilisis ipsum. Morbi in molestie orci. Vestibulum luctus\
 venenatis arcu sit amet pellentesque. Nulla posuere sit amet turpis\
 id pharetra. Curabitur nec.";

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
  // when transmit buffer is empty
  radio.setFifoEmptyAction(fifoAdd);

  // fixed packet length mode is required
  radio.fixedPacketLengthMode(0);

  // start transmitting the long packet
  Serial.print(F("[SX1278] Sending a very long packet ... "));
  transmissionState = radio.startTransmit(longPacket);
}

// flag to indicate we can keep adding more bytes to FIFO
volatile bool fifoEmpty = false;

// flag to indicate that a packet was sent
bool transmittedFlag = false;

// how many bytes are there in total
int totalLength = longPacket.length();

// counter to keep track of how many bytes still need to be sent
int remLength = totalLength;

// this function is called when the radio transmit buffer
// is empty and ready to be refilled
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
#if defined(ESP8266) || defined(ESP32)
  ICACHE_RAM_ATTR
#endif
void fifoAdd(void) {
  // we can send more bytes
  fifoEmpty = true;
}

void loop() {
  if(fifoEmpty) {
    // reset flag
    fifoEmpty = false;

    // add more bytes to the transmit buffer
    uint8_t* txBuffPtr = (uint8_t*)longPacket.c_str();
    transmittedFlag = radio.fifoAdd(txBuffPtr, totalLength, &remLength);
  }

  // check if the previous transmission finished
  if(transmittedFlag) {
    // reset flag
    transmittedFlag = false;

    // reset the counter
    remLength = totalLength;

    if (transmissionState == RADIOLIB_ERR_NONE) {
      // packet was successfully sent
      Serial.println(F("transmission finished!"));

      // NOTE: when using interrupt-driven transmit method,
      //       it is not possible to automatically measure
      //       transmission data rate using getDataRate()

    } else {
      Serial.print(F("failed, code "));
      Serial.println(transmissionState);

    }

    // NOTE: in FSK mode, SX127x will not automatically
    //       turn transmitter off after sending a packet
    //       set mode to standby to ensure we don't jam others
    radio.standby();

    // wait a second before transmitting again
    delay(1000);

    // send another one
    Serial.print(F("[SX1278] Sending another long packet ... "));
    transmissionState = radio.startTransmit(longPacket);
  }
}
