#include "ntohll_ams.h"

uint64_t ntohll_ams(uint64_t x) {
    return (((uint64_t)ntohl((uint32_t)x)) << 32) + ntohl(x >> 32);
}