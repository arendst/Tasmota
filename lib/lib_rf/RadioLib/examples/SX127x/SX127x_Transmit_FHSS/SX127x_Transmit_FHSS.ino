/*
   RadioLib SX127x Transmit with Frequency Hopping Example

   This example transmits packets using SX1278 LoRa radio module.
   Each packet contains up to 255 bytes of data, in the form of:
    - Arduino String
    - null-terminated char array (C-string)
    - arbitrary binary data (byte array)

   Other modules from SX127x/RFM9x family can also be used.

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx127xrfm9x---lora-modem

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/

   SX127x supports FHSS or Frequency Hopping Spread Spectrum.
   Once a hopping period is set and a transmission is started, the radio
   will begin triggering interrupts every hop period where the radio frequency
   is changed to the next channel.
*/

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

// flag to indicate that a packet was received
volatile bool transmittedFlag = false;

// flag to indicate frequency must be changed
volatile bool fhssChangeFlag = false;

// the channel frequencies can be generated randomly or hard coded
// NOTE: The frequency list MUST be the same on both sides!
float channels[] = { 433.0, 433.4, 433.2, 433.6, 434.0, 433.8 };
int numberOfChannels = sizeof(channels) / sizeof(float);

// counter to keep track of how many frequency hops were performed
int hopsCompleted = 0;

// counter that increments with each sent packet
int packetCounter = 0;

// save transmission state between loops
int transmissionState = RADIOLIB_ERR_NONE;

// this is the packet that will be sent
String longPacket = "Let's create a really long packet to trigger \
lots of hop interrupts. A packet can be up to 255 bytes long. \
This packet is 222 bytes so using sf = 9, bw = 125, timeOnAir is \
1488ms. 1488ms / (9*4.10ms) = 40 hops. Counter: ";

// this function is called when a complete packet
// is transmitted by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
#if defined(ESP8266) || defined(ESP32)
  ICACHE_RAM_ATTR
#endif
void setTxFlag(void) {
  transmittedFlag = true;
}

// this function is called when FhssChangeChannel interrupt occurs
// (at the beginning of each transmission)
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
#if defined(ESP8266) || defined(ESP32)
  ICACHE_RAM_ATTR
#endif
void setFHSSFlag(void) {
  fhssChangeFlag = true;
}

void setup() {
  Serial.begin(9600);

  // begin radio on home channel
  Serial.print(F("[SX1278] Initializing ... "));
  int state = radio.begin(channels[0]);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // set hop period in symbols
  // this will also enable FHSS
  state = radio.setFHSSHoppingPeriod(9);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // set the function to call when transmission is finished
  radio.setDio0Action(setTxFlag, RISING);

  // set the function to call when we need to change frequency
  radio.setDio1Action(setFHSSFlag, RISING);

  // start transmitting the first packet
  Serial.print(F("[SX1278] Sending first packet ... "));
  String packet = longPacket + packetCounter;
  transmissionState = radio.startTransmit(packet);
}

void loop() {
  // check if the transmission flag is set
  if (transmittedFlag == true) {
    // reset flag
    transmittedFlag = false;

    if (transmissionState == RADIOLIB_ERR_NONE) {
      // packet was successfully sent
      Serial.println(F("transmission finished!"));

    } else {
      Serial.print(F("failed, code "));
      Serial.println(transmissionState);

    }

    // The channel is automatically reset to 0 upon completion
    Serial.print(F("[SX1278] Radio is on channel: "));
    Serial.println(radio.getFHSSChannel());

    // print the number of hops it took
    Serial.print(F("[SX1278] Hops completed: "));
    Serial.println(hopsCompleted);

    // reset the counter
    hopsCompleted = 0;

    // return to home channel before the next transaction
    radio.setFrequency(channels[radio.getFHSSChannel() % numberOfChannels]);

    // wait a second before transmitting again
    delay(1000);

    // increment the packet counter
    packetCounter++;

    // send another packet
    Serial.print(F("[SX1278] Sending another packet ... "));
    String packet = longPacket + packetCounter;
    transmissionState = radio.startTransmit(packet);
  }

  // check if we need to do another frequency hop
  if (fhssChangeFlag == true) {
    // we do, change it now
    int state = radio.setFrequency(channels[radio.getFHSSChannel() % numberOfChannels]);
    if (state != RADIOLIB_ERR_NONE) {
      Serial.print(F("[SX1278] Failed to change frequency, code "));
      Serial.println(state);
    }

    // increment the counter
    hopsCompleted++;

    // clear the FHSS interrupt
    radio.clearFHSSInt();

    // we're ready to do another hop, clear the flag
    fhssChangeFlag = false;
  }
}
