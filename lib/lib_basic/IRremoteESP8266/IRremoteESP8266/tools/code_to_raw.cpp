// Quick and dirty tool to convert a protocol's (hex) codes to raw timings.
// Copyright 2021 David Conran

#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include "IRac.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "IRutils.h"


void usage_error(char *name) {
  std::cerr << "Usage: " << name
            << " --protocol PROTOCOL_NAME"
            << " --code <hexidecimal>"
            << " [--bits 1-" << kStateSizeMax * 8 << "]"
            << " [--timinginfo]"
            << std::endl;
}

int main(int argc, char *argv[]) {
  int argv_offset = 1;
  int repeats = 0;
  uint64_t code = 0;
  uint8_t state[kStateSizeMax] = {0};  // All array elements are set to 0.
  decode_type_t input_type = decode_type_t::UNKNOWN;
  bool timinginfo = false;

  // Check the invocation/calling usage.
  if (argc < 5 || argc > 8) {
    usage_error(argv[0]);
    return 1;
  }

  if (strncmp("--protocol", argv[argv_offset], 11) == 0) {
    argv_offset++;
    input_type = strToDecodeType(argv[argv_offset]);
    switch (input_type) {
      // Unsupported types
      case decode_type_t::UNUSED:
      case decode_type_t::UNKNOWN:
      case decode_type_t::GLOBALCACHE:
      case decode_type_t::PRONTO:
      case decode_type_t::RAW:
        std::cerr << "The protocol specified is not supported by this program."
                  << std::endl;
        return 1;
      default:
        break;
    }
    argv_offset++;
  }

  uint16_t nbits = IRsend::defaultBits(input_type);
  uint16_t stateSize = nbits / 8;
  if (strncmp("--code", argv[argv_offset], 7) == 0) {
    argv_offset++;
    String hexstr = String(argv[argv_offset]);
    uint64_t strOffset = 0;
    if (hexstr.rfind("0x", 0) || hexstr.rfind("0X", 0)) strOffset = 2;

    // Calculate how many hexadecimal characters there are.
    uint64_t hexstrlength = hexstr.length() - strOffset;

    // Ptr to the least significant byte of the resulting state for this
    // protocol.
    uint8_t *statePtr = &state[stateSize - 1];

    // Convert the string into a state array of the correct length.
    for (uint16_t i = 0; i < hexstrlength; i++) {
      // Grab the next least sigificant hexadecimal digit from the string.
      uint8_t c = tolower(hexstr[hexstrlength + strOffset - i - 1]);
      if (isxdigit(c)) {
        if (isdigit(c))
          c -= '0';
        else
          c = c - 'a' + 10;
      } else {
        std::cerr << "Code " << argv[argv_offset]
                  << " contains non-hexidecimal characters." << std::endl;
        return 3;
      }
      if (i % 2 == 1) {  // Odd: Upper half of the byte.
        *statePtr += (c << 4);
        statePtr--;  // Advance up to the next least significant byte of state.
      } else {  // Even: Lower half of the byte.
        *statePtr = c;
      }
    }
    if (!hasACState(input_type))
      code = std::stoull(argv[argv_offset], nullptr, 16);
    argv_offset++;
  }

  if (argc - argv_offset > 0 && strncmp("--bits", argv[argv_offset], 7) == 0) {
    argv_offset++;
    nbits = std::stoul(argv[argv_offset], nullptr, 10);
    if (nbits == 0 && (nbits <= kStateSizeMax * 8)) {
      std::cerr << "Nr. of bits " << argv[argv_offset]
                << " is invalid." << std::endl;
      return 1;
    }
    stateSize = nbits / 8;
    argv_offset++;
  }

  if (argc - argv_offset > 0 &&
      strncmp("--timinginfo", argv[argv_offset], 13) == 0) {
    argv_offset++;
    timinginfo = true;
  }

  if (argc - argv_offset != 0) {
    usage_error(argv[0]);
    return 1;
  }

  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();
  irsend.reset();

  if (hasACState(input_type))  // Is it larger than 64 bits?
    irsend.send(input_type, state, stateSize);
  else
    irsend.send(input_type, code, nbits, repeats);

  irsend.makeDecodeResult();
  irrecv.decode(&irsend.capture);

  std::cout << "Code type:    " << irsend.capture.decode_type << " ("
            << typeToString(irsend.capture.decode_type) << ")" << std::endl
            << "Code bits:    " << irsend.capture.bits << std::endl;
  if (hasACState(irsend.capture.decode_type)) {
    String description = IRAcUtils::resultAcToString(&irsend.capture);
    if (description.length()) {
      std::cout << "Description:  " << description.c_str() << std::endl;
    }
  }

  std::cout << std::endl << resultToSourceCode(&irsend.capture) << std::endl;
  if (timinginfo) std::cout << resultToTimingInfo(&irsend.capture);

  return 0;
}
