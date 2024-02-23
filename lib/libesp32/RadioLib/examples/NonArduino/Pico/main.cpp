/*
  RadioLib Non-Arduino Raspberry Pi Pico library example

  Licensed under the MIT License

  Copyright (c) 2024 Cameron Goddard

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

// define pins to be used
#define SPI_PORT spi0
#define SPI_MISO 4
#define SPI_MOSI 3
#define SPI_SCK 2

#define RFM_NSS 26
#define RFM_RST 22
#define RFM_DIO0 14
#define RFM_DIO1 15

#include <pico/stdlib.h>

// include the library
#include <RadioLib.h>

// include the hardware abstraction layer
#include "PicoHal.h"

// create a new instance of the HAL class
PicoHal* hal = new PicoHal(SPI_PORT, SPI_MISO, SPI_MOSI, SPI_SCK);

// now we can create the radio module
// NSS pin:  26
// DIO0 pin:  14
// RESET pin:  22
// DIO1 pin:  15
SX1276 radio = new Module(hal, RFM_NSS, RFM_DIO0, RFM_RST, RFM_DIO1);

int main() {
  // initialize just like with Arduino
  printf("[SX1276] Initializing ... ");
  int state = radio.begin();
  if (state != RADIOLIB_ERR_NONE) {
    printf("failed, code %d\n", state);
    return(1);
  }
  printf("success!\n");

  // loop forever
  for(;;) {
    // send a packet
    printf("[SX1276] Transmitting packet ... ");
    state = radio.transmit("Hello World!");
    if(state == RADIOLIB_ERR_NONE) {
      // the packet was successfully transmitted
      printf("success!\n");

      // wait for a second before transmitting again
      hal->delay(1000);

    } else {
      printf("failed, code %d\n", state);

    }

  }

  return(0);
}
