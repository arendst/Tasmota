/*
   RadioLib FSK4 Transmit Example

   This example sends an example FSK-4 'Horus Binary' message
   using SX1278's FSK modem.

   This signal can be demodulated using a SSB demodulator (SDR or otherwise),
   and horusdemodlib: https://github.com/projecthorus/horusdemodlib/wiki

   Other modules that can be used for FSK4:
    - SX127x/RFM9x
    - RF69
    - SX1231
    - CC1101
    - SX126x
    - nRF24
    - Si443x/RFM2x
    - SX128x

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration

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

// create FSK4 client instance using the FSK module
FSK4Client fsk4(&radio);

// An encoded Horus Binary telemetry packet.
// Refer here for packet format information:
// https://github.com/projecthorus/horusdemodlib/wiki/2---Modem-Details#horus-binary-v1-mode-4-fsk
// After demodulation, deinterleaving, and descrambling, this results in a packet:
// 00000001172D0000000000000000D20463010AFF2780
// This decodes to the Habitat-compatible telemetry string:
// $$4FSKTEST,0,01:23:45,0.00000,0.00000,1234,99,1,10,5.00*ABCD
int horusPacketLen = 45;
byte horusPacket[] = {
  0x45, 0x24, 0x24, 0x48, 0x2F, 0x12, 0x16, 0x08, 0x15, 0xC1,
  0x49, 0xB2, 0x06, 0xFC, 0x92, 0xEB, 0x93, 0xD7, 0xEE, 0x5D,
  0x35, 0xA0, 0x91, 0xDA, 0x8D, 0x5F, 0x85, 0x6B, 0x63, 0x03,
  0x6B, 0x60, 0xEA, 0xFE, 0x55, 0x9D, 0xF1, 0xAB, 0xE5, 0x5E,
  0xDB, 0x7C, 0xDB, 0x21, 0x5A, 0x19
};

void setup() {
  Serial.begin(9600);

  // initialize SX1278 with default settings
  Serial.print(F("[SX1278] Initializing ... "));
  int state = radio.beginFSK();

  // when using one of the non-LoRa modules for FSK4
  // (RF69, CC1101, Si4432 etc.), use the basic begin() method
  // int state = radio.begin();

  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

  // initialize FSK4 client
  // NOTE: FSK4 frequency shift will be rounded
  //       to the nearest multiple of frequency step size.
  //       The exact value depends on the module:
  //         SX127x/RFM9x - 61 Hz
  //         RF69 - 61 Hz
  //         CC1101 - 397 Hz
  //         SX126x - 1 Hz
  //         nRF24 - 1000000 Hz
  //         Si443x/RFM2x - 156 Hz
  //         SX128x - 198 Hz
  Serial.print(F("[FSK4] Initializing ... "));
  // low ("space") frequency:     434.0 MHz
  // frequency shift:             270 Hz
  // baud rate:                   100 baud
  state = fsk4.begin(434.0, 270, 100);
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

  // sometimes, it may be needed to set some manual corrections
  // this can be done for tone frequencies,
  // as well as tone lengths
  /*
    // set frequency shift offsets to -120, 60, 0 and 60 Hz and decrease tone length to 95%
    int offsets[4] = { -120, -60, 0, 60 };
    Serial.print(F("[FSK4] Setting corrections ... "));
    state = fsk4.setCorrection(offsets, 0.95);
    if(state == RADIOLIB_ERR_NONE) {
      Serial.println(F("success!"));
    } else {
      Serial.print(F("failed, code "));
      Serial.println(state);
      while(true);
    }
  */
}

void loop() {
  Serial.print(F("[FSK4] Sending FSK4 data packet ... "));

  // send out idle condition for 1000 ms
  fsk4.idle();
  delay(1000);

  // FSK4Client supports binary write methods

  // send some bytes as a preamble
  for(int i = 0; i < 8; i++) {
    fsk4.write(0x1B);
  }

  // now send the encoded packet
  fsk4.write(horusPacket, horusPacketLen);

  Serial.println(F("done!"));

  // wait for a second before transmitting again
  delay(1000);
}
