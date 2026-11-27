#if defined(ESP8266)

#include <cstdint>
#include <c_types.h>

extern "C" int ICACHE_FLASH_ATTR __wrap_rijndaelKeySetupEnc(uint32_t round_keys[], const uint8_t cipher_key[], int key_bits) {
  if (key_bits != 128) {
    return -1;
  }
  using RomKeySetup = void (*)(uint32_t[], const uint8_t[]);
  constexpr uint32_t rom_key_setup_address = 0x40009300;
  reinterpret_cast<RomKeySetup>(rom_key_setup_address)(round_keys, cipher_key);
  return 10;
}

#endif
