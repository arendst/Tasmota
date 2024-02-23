/*
  RadioLib Non-Arduino Tock Library test application

  Licensed under the MIT License

  Copyright (c) 2023 Alistair Francis <alistair@alistair23.me>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

// include the library
#include <RadioLib.h>

// include the hardware abstraction layer
#include "libtockHal.h"

// the entry point for the program
int main(void) {
  printf("[SX1261] Initialising Radio ... \r\n");

  // create a new instance of the HAL class
  TockHal* hal = new TockHal();

  // now we can create the radio module
  // pinout corresponds to the SparkFun LoRa Thing Plus - expLoRaBLE
  // NSS pin:   0
  // DIO1 pin:  2
  // NRST pin:  4
  // BUSY pin:  1
  Module* tock_module = new Module(hal, RADIO_NSS, RADIO_DIO_1, RADIO_RESET, RADIO_BUSY);
  SX1262* radio = new SX1262(tock_module);

  // Setup the radio
  // The settings here work for the SparkFun LoRa Thing Plus - expLoRaBLE
  radio->XTAL = true;
  int state = radio->begin(915.0);

  if (state != RADIOLIB_ERR_NONE) {
    printf("failed, code %d\r\n", state);
    return 1;
  }
  printf("success!\r\n");

  // loop forever
  for(;;) {
    yield_no_wait();
    // send a packet
    printf("[SX1261] Transmitting\r\n");

    state = radio->transmit("Hello World!");

    if(state == RADIOLIB_ERR_NONE) {
      // the packet was successfully transmitted
      printf("success!\r\n");

      // wait for a second before transmitting again
      hal->delay(1000);
    } else {
      printf("failed, code %d\r\n", state);
    }

  }

  return 0;
}
