// this is an autotest file for the SX126x
// runs on Raspberry Pi with Waveshare LoRaWAN hat

#include <RadioLib/RadioLib.h>
#include "PiHal.h"

#define RADIOLIB_TEST_ASSERT(STATEVAR) { if((STATEVAR) != RADIOLIB_ERR_NONE) { return(-1*(STATEVAR)); } }

PiHal* hal = new PiHal(1);
SX1261 radio = new Module(hal, 7, 17, 22, RADIOLIB_NC);

// the entry point for the program
int main(int argc, char** argv) {
  int state = RADIOLIB_ERR_UNKNOWN;

  state = radio.begin();
  printf("[SX1261] Test:begin() = %d\n", state);
  RADIOLIB_TEST_ASSERT(state);

  state = radio.transmit("Hello World!");
  printf("[SX1261] Test:transmit() = %d\n", state);
  RADIOLIB_TEST_ASSERT(state);

  hal->term();
  return(0);
}
