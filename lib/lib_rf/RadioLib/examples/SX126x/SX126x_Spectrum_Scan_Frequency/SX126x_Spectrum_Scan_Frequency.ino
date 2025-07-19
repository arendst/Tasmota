/*
   RadioLib SX126x Spectrum Scan Example

   This example shows how to perform a spectrum power scan using SX126x.
   The output is in the form of scan lines, each line has 33 power bins.
   First power bin corresponds to -11 dBm, the second to -15 dBm and so on.
   Higher number of samples in a bin corresponds to more power received
   at that level. The example performs frequency sweep over a given range.

   To show the results in a plot, run the Python script
   RadioLib/extras/SX126x_Spectrum_Scan/SpectrumScan.py

   WARNING: This functionality is experimental and requires a binary patch
   to be uploaded to the SX126x device. There may be some undocumented
   side effects!

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx126x---lora-modem

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

// include the library
#include <RadioLib.h>

// this file contains binary patch for the SX1262
#include <modules/SX126x/patches/SX126x_patch_scan.h>

// SX1262 has the following connections:
// NSS pin:   10
// DIO1 pin:  2
// NRST pin:  3
// BUSY pin:  9
SX1262 radio = new Module(10, 2, 3, 9);

// frequency range in MHz to scan
const float freqStart = 431;
const float freqEnd = 435;

void setup() {
  Serial.begin(115200);

  // initialize SX1262 FSK modem at the initial frequency
  Serial.print(F("[SX1262] Initializing ... "));
  int state = radio.beginFSK(freqStart);
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

  // upload a patch to the SX1262 to enable spectral scan
  // NOTE: this patch is uploaded into volatile memory,
  //       and must be re-uploaded on every power up
  Serial.print(F("[SX1262] Uploading patch ... "));
  state = radio.uploadPatch(sx126x_patch_scan, sizeof(sx126x_patch_scan));
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

  // configure scan bandwidth to 234.4 kHz
  // and disable the data shaping
  Serial.print(F("[SX1262] Setting scan parameters ... "));
  state = radio.setRxBandwidth(234.3);
  state |= radio.setDataShaping(RADIOLIB_SHAPING_NONE);
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }
}

void loop() {
  // perform scan over the entire frequency range
  float freq = freqStart;
  while(freq <= freqEnd) {
    Serial.print("FREQ ");
    Serial.println(freq, 2);

    // start spectral scan
    // number of samples: 2048 (fewer samples = better temporal resolution)
    Serial.print(F("[SX1262] Starting spectral scan ... "));
    int state = radio.spectralScanStart(2048);
    if(state == RADIOLIB_ERR_NONE) {
      Serial.println(F("success!"));
    } else {
      Serial.print(F("failed, code "));
      Serial.println(state);
      while(true);
    }

    // wait for spectral scan to finish
    while(radio.spectralScanGetStatus() != RADIOLIB_ERR_NONE) {
      delay(10);
    }

    // read the results
    uint16_t results[RADIOLIB_SX126X_SPECTRAL_SCAN_RES_SIZE];
    state = radio.spectralScanGetResult(results);
    if(state == RADIOLIB_ERR_NONE) {
      // we have some results, print it
      Serial.print("SCAN ");
      for(uint8_t i = 0; i < RADIOLIB_SX126X_SPECTRAL_SCAN_RES_SIZE; i++) {
        Serial.print(results[i]);
        Serial.print(',');
      }
      Serial.println(" END");
    }

    // wait a little bit before the next scan
    delay(5);

    // set the next frequency
    // the frequency step should be slightly smaller
    // or the same as the Rx bandwidth set in setup
    freq += 0.2;
    radio.setFrequency(freq);
  }
  
}
