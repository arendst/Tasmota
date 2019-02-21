// Quick and dirty tool to decode GlobalCache (GC) codes
// Copyright 2017 Jorge Cisneros

#include <errno.h>
#include <inttypes.h>
#include <string.h>
#include <string>
#include "IRsend.h"
#include "IRsend_test.h"

#define MAX_GC_CODE_LENGHT 512

void str_to_uint16(char *str, uint16_t *res) {
  char *end;
  errno = 0;
  intmax_t val = strtoimax(str, &end, 10);
  if (errno == ERANGE || val < 0 || val > UINT16_MAX ||
    end == str || *end != '\0')
    return;
  *res = (uint16_t) val;
}

std::string encoding(decode_results *results) {
  switch (results->decode_type) {
    default:
    case UNKNOWN:      return "UNKNOWN";       break;
    case NEC:          return "NEC";           break;
    case NEC_LIKE:     return "NEC (non-strict)";  break;
    case SONY:         return "SONY";          break;
    case RC5:          return "RC5";           break;
    case RC5X:         return "RC5X";          break;
    case RC6:          return "RC6";           break;
    case RCMM:         return "RCMM";          break;
    case DISH:         return "DISH";          break;
    case SHARP:        return "SHARP";         break;
    case JVC:          return "JVC";           break;
    case SANYO:        return "SANYO";         break;
    case SANYO_LC7461: return "SANYO_LC7461";  break;
    case MITSUBISHI:   return "MITSUBISHI";    break;
    case SAMSUNG:      return "SAMSUNG";       break;
    case LG:           return "LG";            break;
    case WHYNTER:      return "WHYNTER";       break;
    case AIWA_RC_T501: return "AIWA_RC_T501";  break;
    case PANASONIC:    return "PANASONIC";     break;
    case DENON:        return "DENON";         break;
    case COOLIX:       return "COOLIX";        break;
    case NIKAI:        return "NIKAI";         break;
  }
}

void usage_error(char * name) {
  std::cerr << "Usage: " << name << " [-raw] <global_code>" << std::endl;
}

int main(int argc, char * argv[]) {
  int argv_offset = 1;
  bool dumpraw = false;

  // Check the invocation/calling usage.
  if (argc < 2 || argc > 3) {
    usage_error(argv[0]);
    return 1;
  }
  if (strncmp("-raw", argv[argv_offset], 4) == 0) {
    dumpraw = true;
    argv_offset++;
  }
  if (argc - argv_offset != 1) {
    usage_error(argv[0]);
    return 1;
  }

  uint16_t gc_test[MAX_GC_CODE_LENGHT];
  int index = 0;
  char *pch;
  char *saveptr1;

  pch = strtok_r(argv[argv_offset], ",", &saveptr1);
  while (pch != NULL && index < MAX_GC_CODE_LENGHT) {
    str_to_uint16(pch, &gc_test[index]);
    pch = strtok_r(NULL, ",", &saveptr1);
    index++;
  }

  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();
  irsend.reset();

  irsend.sendGC(gc_test, index);
  irsend.makeDecodeResult();
  irrecv.decode(&irsend.capture);

  std::cout << "Code GC length " << index << std::endl
  << "Code type      " << irsend.capture.decode_type
  << " (" << encoding(&irsend.capture) << ")" << std::endl
  << "Code bits      " << irsend.capture.bits << std::endl
  << "Code value     0x" << std::hex << irsend.capture.value << std::endl
  << "Code address   0x" << std::hex << irsend.capture.address << std::endl
  << "Code command   0x" << std::hex << irsend.capture.command << std::endl;

  if (dumpraw || irsend.capture.decode_type == UNKNOWN)
    irsend.dumpRawResult();

  return 0;
}
