#if defined(ESP8266)

#include <cstdint>
#include <c_types.h>

extern "C" const uint32_t Te0[];

static inline uint32_t rotate_right(uint32_t value, unsigned shift) {
  return (value >> shift) | (value << (32 - shift));
}

extern "C" void ICACHE_FLASH_ATTR __wrap_aes_encrypt(void *context, const uint8_t *input, uint8_t *output) {
  const uint32_t *round_keys = static_cast<const uint32_t *>(context);
  uint32_t state[4];
  uint32_t next[4];

  for (unsigned column = 0; column < 4; ++column) {
    state[column] = (uint32_t(input[4 * column]) << 24) |
                    (uint32_t(input[4 * column + 1]) << 16) |
                    (uint32_t(input[4 * column + 2]) << 8) |
                    uint32_t(input[4 * column + 3]);
    state[column] ^= round_keys[column];
  }

  for (unsigned round = 1; round < 10; ++round) {
    for (unsigned column = 0; column < 4; ++column) {
      next[column] = Te0[state[column] >> 24] ^
                     rotate_right(Te0[(state[(column + 1) & 3] >> 16) & 0xff], 8) ^
                     rotate_right(Te0[(state[(column + 2) & 3] >> 8) & 0xff], 16) ^
                     rotate_right(Te0[state[(column + 3) & 3] & 0xff], 24) ^
                     round_keys[round * 4 + column];
    }
    for (unsigned column = 0; column < 4; ++column) {
      state[column] = next[column];
    }
  }

  for (unsigned column = 0; column < 4; ++column) {
    uint32_t result = (Te0[state[column] >> 24] & 0x00ff0000) << 8;
    result |= Te0[(state[(column + 1) & 3] >> 16) & 0xff] & 0x00ff0000;
    result |= (Te0[(state[(column + 2) & 3] >> 8) & 0xff] & 0x00ff0000) >> 8;
    result |= (Te0[state[(column + 3) & 3] & 0xff] & 0x00ff0000) >> 16;
    result ^= round_keys[40 + column];
    output[4 * column] = result >> 24;
    output[4 * column + 1] = result >> 16;
    output[4 * column + 2] = result >> 8;
    output[4 * column + 3] = result;
  }
}

#endif
