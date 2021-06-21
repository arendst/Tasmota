// Quick and dirty tool to decode Raw Codes, GlobalCache (GC) codes
// and ProntoHex codes
// Copyright 2017 Jorge Cisneros

#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include "IRac.h"
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
  std::cerr << "Usage: " << name << " [-gc] [-rawdump] <global_code>"
            << std::endl
            << "Usage: " << name
            << " -prontohex [-rawdump] [-repeats num] <prontohex_code>"
            << std::endl
            << "Usage: " << name << " -raw [-rawdump] <raw_code>" << std::endl;
}

int main(int argc, char *argv[]) {
  int argv_offset = 1;
  int repeats = 0;
  bool dumpraw = false;
  enum decode_type_t input_type = GLOBALCACHE;
  const uint16_t raw_freq = 38;
  // Check the invocation/calling usage.
  if (argc < 2 || argc > 6) {
    usage_error(argv[0]);
    return 1;
  }
  if (strncmp("-gc", argv[argv_offset], 3) == 0) {
    argv_offset++;
  } else if (strncmp("-prontohex", argv[argv_offset], 10) == 0) {
    input_type = PRONTO;
    argv_offset++;
  } else if (strncmp("-raw", argv[argv_offset], 4) == 0) {
    input_type = RAW;
    argv_offset++;
  }

  if (strncmp("-rawdump", argv[argv_offset], 8) == 0) {
    dumpraw = true;
    argv_offset++;
  }

  if (input_type == PRONTO && strncmp("-repeats", argv[argv_offset], 8) == 0) {
    argv_offset++;
    if (argc - argv_offset <= 1) {
      usage_error(argv[0]);
      return 1;
    }
    repeats = atoi(argv[argv_offset++]);
    if (repeats < 0) {
      usage_error(argv[0]);
      return 1;
    }
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
  if (input_type == PRONTO) {
    sep = const_cast<char *>(" ");
    codebase = 16;
  }

  pch = strtok_r(argv[argv_offset], sep, &saveptr1);
  while (pch != NULL && index < kMaxGcCodeLength) {
    str_to_uint16(pch, &gc_test[index], codebase);
    pch = strtok_r(NULL, sep, &saveptr1);
    index++;
  }

  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();
  irsend.reset();

  switch (input_type) {
    case GLOBALCACHE:
      irsend.sendGC(gc_test, index);
      break;
    case PRONTO:
      irsend.sendPronto(gc_test, index, repeats);
      break;
    case RAW:
      irsend.sendRaw(gc_test, index, raw_freq);
      break;
    default:
      break;
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
    String description = IRAcUtils::resultAcToString(&irsend.capture);
    if (description.length()) {
      std::cout << "Msg Description: " << description.c_str() << std::endl;
    }
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
