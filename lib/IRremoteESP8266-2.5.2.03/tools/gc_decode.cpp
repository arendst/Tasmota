// Quick and dirty tool to decode GlobalCache (GC) codes
// and ProntoHex codes
// Copyright 2017 Jorge Cisneros

#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
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
  std::cerr << "Usage: " << name << " [-raw] <global_code>" << std::endl
            << "Usage: " << name << " -prontohex [-raw] <prontohex_code>"
            << std::endl;
}

int main(int argc, char *argv[]) {
  int argv_offset = 1;
  bool dumpraw = false;
  bool prontohex = false;

  // Check the invocation/calling usage.
  if (argc < 2 || argc > 4) {
    usage_error(argv[0]);
    return 1;
  }
  if (strncmp("-prontohex", argv[argv_offset], 10) == 0) {
    prontohex = true;
    argv_offset++;
  }

  if (strncmp("-raw", argv[argv_offset], 4) == 0) {
    dumpraw = true;
    argv_offset++;
  }
  if (argc - argv_offset != 1) {
    usage_error(argv[0]);
    return 1;
  }

  uint16_t gc_test[kMaxGcCodeLength];
  int index = 0;
  char *pch;
  char *saveptr1;
  char *sep = const_cast<char *>(",");
  int codebase = 10;

  if (prontohex) {
    sep = const_cast<char *>(" ");
    codebase = 16;
  }

  pch = strtok_r(argv[argv_offset], sep, &saveptr1);
  while (pch != NULL && index < kMaxGcCodeLength) {
    str_to_uint16(pch, &gc_test[index], codebase);
    pch = strtok_r(NULL, sep, &saveptr1);
    index++;
  }

  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();
  irsend.reset();

  if (prontohex) {
    irsend.sendPronto(gc_test, index);
  } else {
    irsend.sendGC(gc_test, index);
  }
  irsend.makeDecodeResult();
  irrecv.decode(&irsend.capture);

  std::cout << "Code length " << index << std::endl
            << "Code type      " << irsend.capture.decode_type << " ("
            << typeToString(irsend.capture.decode_type) << ")" << std::endl
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

  if (dumpraw || irsend.capture.decode_type == UNKNOWN) irsend.dumpRawResult();

  return 0;
}
