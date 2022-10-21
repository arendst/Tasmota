// Quick and dirty tool to decode mode2 data from LIRC
// Copyright 2018 Brett T. Warden
// based on c2_decode.cpp, Copyright 2017 Jorge Cisneros

// Usage example:
// mode2 -H udp -d 5000 | ./mode2_decode

/* Sample input (alternating space and pulse durations in microseconds):
space 500000
pulse 915
space 793
pulse 488
space 366
pulse 915
space 793
pulse 427
space 500000
*/

#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <string>
#include "IRsend.h"
#include "IRsend_test.h"
#include "IRutils.h"

const uint16_t kMaxGcCodeLength = 10000;

void str_to_uint16(char *str, uint16_t *res, uint8_t base) {
  char *end;
  errno = 0;
  intmax_t val = strtoimax(str, &end, base);
  if (errno == ERANGE || val < 0 || val > UINT16_MAX || end == str ||
      *end != '\0')
    return;
  *res = (uint16_t)val;
}

void usage_error(char *name) {
  std::cerr << "Usage: " << name << " [-raw]" << std::endl;
}

int main(int argc, char *argv[]) {
  bool dumpraw = false;

  // Check the invocation/calling usage.
  if (argc > 2) {
    usage_error(argv[0]);
    return 1;
  }

  if (argc == 2 && strncmp("-raw", argv[1], 4) == 0) {
    dumpraw = true;
  }

  int index = 0;
  std::string line, type;
  std::string pulse = "pulse";
  std::string space = "space";
  int duration;

  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();
  irsend.reset();

  while (getline(std::cin, line)) {
    std::istringstream iss1(line);
    iss1 >> type;
    iss1 >> duration;

    // Clamp duration to int16_t
    if (duration > 0xFFFF) {
      duration = 0xFFFF;
    }
    if (pulse.compare(type) == 0) {
      irsend.mark(duration);
    } else if (space.compare(type) == 0) {
      irsend.space(duration);
    }
    index++;

    if (duration > 20000 || index >= kMaxGcCodeLength) {
      // Skip long spaces at beginning
      if (index > 1) {
        irsend.makeDecodeResult();
        irrecv.decode(&irsend.capture);

        std::cout << "Code length " << index << std::endl
                  << "Code type      " << irsend.capture.decode_type << " ("
                  << typeToString(irsend.capture.decode_type) << ")"
                  << std::endl
                  << "Code bits      " << irsend.capture.bits << std::endl;
        if (hasACState(irsend.capture.decode_type)) {
          std::cout << "State value    0x";
          for (uint16_t i = 0; i < irsend.capture.bits / 8; i++)
            printf("%02X", irsend.capture.state[i]);
          std::cout << std::endl;
        } else {
          std::cout << "Code value     0x" << std::hex << irsend.capture.value
                    << std::endl
                    << "Code address   0x" << std::hex << irsend.capture.address
                    << std::endl
                    << "Code command   0x" << std::hex << irsend.capture.command
                    << std::endl;
        }

        if (dumpraw || irsend.capture.decode_type == UNKNOWN)
          irsend.dumpRawResult();
      }

      irsend.reset();
      index = 0;
    }
  }

  return 0;
}
